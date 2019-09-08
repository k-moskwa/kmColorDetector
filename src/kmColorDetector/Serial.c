/*
 * Serial.c
 *
 *  Created on: Jul 10, 2019
 *      Author: Krzysztof Moskwa
 *      License: GPL-3.0-or-later
 *
 *  Color detector based on AVR uC, TCS3200 and DFRobot Mini Player
 *  Copyright (C) 2019  Krzysztof Moskwa
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <util/atomic.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#undef BAUD
#define BAUD PLYR_SERIAL_BAUD_RATE
#include <util/setbaud.h>

#include "SerialDefs.h"
#include "Serial.h"
#include "Debug.h"

#define SERIAL_RX_BUFFER_SIZE 20
#define SERIAL_TX_BUFFER_SIZE SERIAL_RX_BUFFER_SIZE

static volatile uint8_t *_ubrrh = 0;
static volatile uint8_t *_ubrrl = 0;
static volatile uint8_t *_ucsra = 0;
static volatile uint8_t *_ucsrb = 0;
static volatile uint8_t *_ucsrc = 0;
static volatile uint8_t *_udr = 0;

// Has any byte been written to the UART since serInit()
static bool _written = false;
static bool _rxBufferOverflow = false;

static const char _strCrLf[] PROGMEM = "\r\n";
static const char _strPrefixHex[] PROGMEM = "0x";
static const char _strPrefixOct[] PROGMEM = "0";
static unsigned char _rxTerminationChar = '\r';

static uint8_t _rxBuffer[SERIAL_RX_BUFFER_SIZE];
static volatile int _rxBufferGetIndex = 0;
static volatile int _rxBufferPutIndex = 0;
static volatile int _rxBufferLength = 0;
static volatile int _rxBufferLines = 0;

static uint8_t _txBuffer[SERIAL_RX_BUFFER_SIZE];
static volatile int _txBufferGetIndex = 0;
static volatile int _txBufferPutIndex = 0;
static volatile int _txBufferLength = 0;

// Terminal commands
#define SER_TERM_CLEAR_SCREEN PSTR("\e[2J")
#define SER_TERM_CURSOR_HOME PSTR("\e[H")

// Private "functions"
void rxStore(uint8_t c);
int rxAvailable(void);
int rxAvailableForWrite(void);
int rxPeek(void);
int rxRead(void);
void rxFlush(void);

void txStore(uint8_t c);
int txAvailable(void);
int txPeek(void);
int txRead(void);
void txFlush(void);

void serSetup(
volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
volatile uint8_t *ucsrc, volatile uint8_t *udr);

// Interrupt handler
void irqTx(void);
void irqRx(void);

// Internal RX/TX buffer operations
void rxStore(uint8_t c) {
    if (_rxTerminationChar == c) {
        _rxBufferLines++;
    }
    _rxBuffer[_rxBufferPutIndex] = c;
    _rxBufferPutIndex = (_rxBufferPutIndex + 1) % SERIAL_RX_BUFFER_SIZE;
    _rxBufferLength++;
}

int rxAvailable(void) {
    return _rxBufferLength;
}

int rxAvailableForWrite(void) {
    return SERIAL_RX_BUFFER_SIZE - _rxBufferLength;
}

int rxPeek(void) {
    if (0 == _rxBufferLength) {
        return -1;
    } else {
        return _rxBuffer[_rxBufferGetIndex];
    }
}

int rxRead(void) {
    if (0 == _rxBufferLength) {
        return -1;
    } else {
        uint8_t c = _rxBuffer[_rxBufferGetIndex++];
        _rxBufferGetIndex %= SERIAL_RX_BUFFER_SIZE;
        _rxBufferLength--;
        if (_rxTerminationChar == c) {
            _rxBufferLines--;
        }
        return c;
    }
}

void rxFlush(void) {
    _rxBufferLength = 0;
}

void txStore(uint8_t c) {
    _txBuffer[_txBufferPutIndex] = c;
    _txBufferPutIndex = (_txBufferPutIndex + 1) % SERIAL_TX_BUFFER_SIZE;
    _txBufferLength++;
}

int txAvailable(void) {
    return _txBufferLength;
}

int txPeek(void) {
    if (0 == _txBufferLength) {
        return -1;
    } else {
        return _txBuffer[_txBufferGetIndex];
    }
}

int txRead(void) {
    if (0 == _txBufferLength) {
        return -1;
    } else {
        uint8_t c = _txBuffer[_txBufferGetIndex++];
        _txBufferGetIndex %= SERIAL_TX_BUFFER_SIZE;
        _txBufferLength--;
        return c;
    }
}

void txFlush(void) {
    _txBufferLength = 0;
}

// Interrupt handler
void irqTx(void) {
    // If interrupts are enabled, there must be more data in the output
    // buffer. Send the next byte
    if (txAvailable() > 0) {
        unsigned char c = (unsigned char )txRead();
        *_udr = c;
    }

    // clear the TXC bit -- "can be cleared by writing a one to its bit
    // location". This makes sure flush() won't return until the bytes
    // actually got written. Other r/w bits are preserved, and zeros
    // written to the rest.
    *_ucsra = ((*_ucsra) & (_BV(U2X) | _BV(MPCM))) | _BV(TXC);
    if (txAvailable() == 0) {
        // if buffer empty - disable interrupts
        UCSRB &= ~(1<<UDRIE);
    }
}

void irqRx(void) {
    if (bit_is_clear(*_ucsra, UPE)) {
        // No Parity error, read byte and store it in the buffer if there is room
        unsigned char c = *_udr;
        // if we should be storing the received character into the location
        // just before the tail (meaning that the head would advance to the
        // current location of the tail), we're about to overflow the buffer
        // and so we don't write the character or advance the head.
        if (rxAvailableForWrite() > 0) {
            rxStore(c);
        } else {
            // Parity error, read byte but discard it
            // *_udr;
            _rxBufferOverflow = true;
        }
    }
}

// Implementation
void serSetTerminationCharacter(unsigned char terminator) {
	_rxTerminationChar = terminator;
}

void serSetup(
volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
volatile uint8_t *ucsrc, volatile uint8_t *udr) {
    _ubrrh = ubrrh;
    _ubrrl = ubrrl;
    _ucsra = ucsra;
    _ucsrb = ucsrb;
    _ucsrc = ucsrc;
    _udr = udr;
}

void serInitComplete(unsigned long baud, uint8_t config) {
    serSetup(&UBRRH, &UBRRL, &UCSRA, &UCSRB, &UCSRC, &UDR);
    _written = false;

	#define __UBRR F_CPU/16/UART_BAUD-1 
	uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
	if (baud_setting > 0xFFF) {
		baud_setting /= 2;
	    *_ucsra &= ~_BV(U2X);
	} else {
	    *_ucsra |= _BV(U2X);
	}

    *_ubrrh =  baud_setting >> 8;
    *_ubrrl =  baud_setting & 0xFF;

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
    config |= _BV(URSEL); // select UCSRC register (shared with UBRRH)
#endif
    *_ucsrc = config;

    *_ucsrb |= (_BV(RXEN) | _BV(TXEN) | _BV(RXCIE));
}

void serInit(unsigned long baud) {
    serInitComplete(baud, SERIAL_8N1);
}

void serFlush(void) {
    // If we have never written a byte, no need to flush. This special
    // case is needed since there is no way to force the TXC (transmit
    // complete) bit to 1 during initialization
    if (false == _written) {
        return;
    }

    while (bit_is_set(*_ucsrb, UDRIE) || bit_is_clear(*_ucsra, TXC)) {
        if (bit_is_clear(SREG, SREG_I) && bit_is_set(*_ucsrb, UDRIE)) {
            // Interrupts are globally disabled, but the DR empty
            // interrupt should be enabled, so poll the DR empty flag to
            // prevent deadlock
            if (bit_is_set(*_ucsra, UDRE)) {
                irqTx();
            }
        }
    }
}

void serEnd(void) {
    // wait for transmission of outgoing data
    serFlush();
    *_ucsrb &= ~_BV(RXEN);
    *_ucsrb &= ~_BV(TXEN);
    *_ucsrb &= ~_BV(RXCIE);

    rxFlush();
}

int serAvailable(void) {
    return rxAvailable();
}

int serPeek(void) {
    return rxPeek();
}

int serRead(void) {
    return rxRead();
}

int serAvailableLines(void) {
    return _rxBufferLines;
}

void serReadLine(char *buf, uint8_t maxLen) {
    if (serAvailableLines() > 0) {
        char c = 0;
        while (maxLen-- > 0) {
            c = rxRead();
            if ('\n' == c) {
                *buf++ = 0;
            } else if ('\r' == c) {
                *buf++ = 0;
                break;
            } else if (0 == c) {
                continue;
            } else {
                *buf++ = c;
            }
        }
    } else if (true == _rxBufferOverflow) {
        // protect situation when buffer overflow happened
        // in this case data is corrupted anyway
        rxFlush();
    }
}


int serAvailableForWrite(void) {
    return SERIAL_TX_BUFFER_SIZE - _txBufferLength;
}

size_t serWriteChar(uint8_t c) {
    _written = true;
    // If the buffer and the data register is empty, just write the byte
    // to the data register and be done. This shortcut helps
    // significantly improve the effective data-rate at high (> 500kbit/s)
    // bit-rates, where interrupt overhead becomes a slowdown.
    if (0 == txAvailable() && bit_is_set(*_ucsra, UDRE)) {
        // If TXC is cleared before writing UDR and the previous byte
        // completes before writing to UDR, TXC will be set but a byte
        // is still being transmitted causing flush() to return too soon.
        // So writing UDR must happen first.
        // Writing UDR and clearing TC must be done atomically, otherwise
        // interrupts might delay the TXC clear so the byte written to UDR
        // is transmitted (setting TXC) before clearing TXC. Then TXC will
        // be cleared when no bytes are left, causing flush() to hang
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            *_udr = c;
            *_ucsra = ((*_ucsra) & (_BV(U2X) | _BV(MPCM))) | _BV(TXC);
        }
    return 1;
    }
    // If the output buffer is full, there's nothing for it other than to
    // wait for the interrupt handler to empty it a bit
    while (serAvailableForWrite() == 0) {
        if (bit_is_clear(SREG, SREG_I)) {
            // Interrupts are disabled, so we'll have to poll the data
            // register empty flag ourselves. If it is set, pretend an
            // interrupt has happened and call the handler to free up
            // space for us.
            if(bit_is_set(*_ucsra, UDRE)) {
                irqTx();
            } else {
                // nop, the interrupt handler will free up space for us
            }
        }
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // make atomic to prevent execution of ISR between adding
        // new character to the buffer and setting the interrupt flag
        // resulting in buffer retransmission
        txStore(c);
        // enable interrupts
        *_ucsrb |= _BV(UDRIE);
    }
    return 1;
}

void serPrintString(const char *str) {
    const unsigned char *strTmp = (const unsigned char *) str;
    while (0 != *strTmp) {
        serWriteChar(*strTmp++);
    }
}

void serPrintString_P(const char *str) {
    register char c = 0;
    const unsigned char *strTmp = (const unsigned char *) str;
    while (0 != (c = pgm_read_byte(strTmp++))) {
        serWriteChar(c);
    }
}

void serPrintLn(void) {
    serPrintString_P(_strCrLf);
}

void serPrintLnString(const char *str) {
    serPrintString(str);
    serPrintLn();
}

void serPrintLnString_P(const char *str) {
    serPrintString_P(str);
    serPrintLn();
}

void serPrintNumber(unsigned long n, uint8_t base) {
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';
    // prevent crash if called with base == 1

    if(base < 2) {
        base = 10;
    }

    do {
        unsigned long m = n;
        n /= base;
        char c = m - base * n;
        *--str = (c < 10 ? c + '0' : c + 'A' - 10);
    } while (0 != n);
}

void serPrintHex(unsigned long n) {
    serPrintLnString_P(_strPrefixHex);
    serPrintNumber(n, SERIAL_STR_HEX);
}

void serPrintDec(unsigned long n) {
    serPrintNumber(n, SERIAL_STR_DEC);
}

void serPrintOct(unsigned long n) {
    serPrintLnString_P(_strPrefixHex);
    serPrintNumber(n, SERIAL_STR_HEX);
}

void serSendBinary(const uint8_t *buf, uint8_t len) {
    for (int i = 0; i < len; i++) {
        serWriteChar((unsigned char)buf[i]);
    }
}

void serTermClearScreen(void) {
	serPrintString_P(SER_TERM_CLEAR_SCREEN);
}

void serTermCursorHome(void) {
	serPrintString_P(SER_TERM_CURSOR_HOME);
}

ISR( USART_RXC_vect ) {
    irqRx();
}

ISR( USART_UDRE_vect ) {
    irqTx();
}