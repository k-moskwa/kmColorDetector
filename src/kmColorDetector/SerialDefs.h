/** @file
 * @brief Common definitions for Serial functions.
 * SerialDefs.h
 *
 *  Created on: Aug 25, 2019
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


#ifndef SERIALDEFS_H_
#define SERIALDEFS_H_

/// DEC Base definition for conversion of the values to string.
#define SERIAL_STR_DEC 10
/// HEX Base definition for conversion of the values to string.
#define SERIAL_STR_HEX 16
/// OCT Base definition for conversion of the values to string.
#define SERIAL_STR_OCT 8

/// Serial Asynchronous Operation
#define SERIAL_ASYNC			0x00
/// Serial Synchronous Operation
#define SERIAL_SYNC				_BV(UMSEL)

/// Parity Mode Disabled
#define SERIAL_PARITY_DISABLED	0x00
/// Parity Mode Enabled, Even Parity
#define SERIAL_PARITY_EVEN		_BV(UPM1)
/// Parity Mode Enabled, Odd Parity
#define SERIAL_PARITY_ODD		_BV(UPM1) |		_BV(UPM0)

/// USBS Bit Settings 1-Stop Bit
#define SERIAL_STOP_BITS_1		0x00
/// USBS Bit Settings 2-Stop Bits
#define SERIAL_STOP_BITS_2		_BV(USBS)

/// 5-bit Character Size
#define SERIAL_SIZE_BITS_5_B	0x00
#define SERIAL_SIZE_BITS_5_C	0x00
/// 6-bit Character Size
#define SERIAL_SIZE_BITS_6_B	0x00
#define SERIAL_SIZE_BITS_6_C					_BV(UCSZ00)
/// 7-bit Character Size
#define SERIAL_SIZE_BITS_7_B	0x00
#define SERIAL_SIZE_BITS_7_C	_BV(UCSZ01)
/// 8-bit Character Size
#define SERIAL_SIZE_BITS_8_B	0x00
#define SERIAL_SIZE_BITS_8_C	_BV(UCSZ01) |	_BV(UCSZ00)
/// 9-bit Character Size
#define SERIAL_SIZE_BITS_9_B	_BV(UCSZ02)
#define SERIAL_SIZE_BITS_9_C	_BV(UCSZ01) |	_BV(UCSZ00)

/// 5-bit Characters Size, No Parity, 1-Stop Bit
#define SERIAL_5N1 SERIAL_SIZE_BITS_5_C | SERIAL_ASYNC | SERIAL_PARITY_DISABLED | SERIAL_STOP_BITS_1
/// 6-bit Characters Size, No Parity, 1-Stop Bit
#define SERIAL_6N1 SERIAL_SIZE_BITS_6_C | SERIAL_ASYNC | SERIAL_PARITY_DISABLED | SERIAL_STOP_BITS_1
/// 7-bit Characters Size, No Parity, 1-Stop Bit
#define SERIAL_7N1 SERIAL_SIZE_BITS_7_C | SERIAL_ASYNC | SERIAL_PARITY_DISABLED | SERIAL_STOP_BITS_1
/// 8-bit Characters Size, No Parity, 1-Stop Bit
#define SERIAL_8N1 SERIAL_SIZE_BITS_8_C | SERIAL_ASYNC | SERIAL_PARITY_DISABLED | SERIAL_STOP_BITS_1
/// 5-bit Characters Size, No Parity, 2-Stop Bits
#define SERIAL_5N2 SERIAL_SIZE_BITS_5_C | SERIAL_ASYNC | SERIAL_PARITY_DISABLED | SERIAL_STOP_BITS_2
/// 6-bit Characters Size, No Parity, 2-Stop Bits
#define SERIAL_6N2 SERIAL_SIZE_BITS_6_C | SERIAL_ASYNC | SERIAL_PARITY_DISABLED | SERIAL_STOP_BITS_2
/// 7-bit Characters Size, No Parity, 2-Stop Bits
#define SERIAL_7N2 SERIAL_SIZE_BITS_7_C | SERIAL_ASYNC | SERIAL_PARITY_DISABLED | SERIAL_STOP_BITS_2
/// 8-bit Characters Size, No Parity, 2-Stop Bits
#define SERIAL_8N2 SERIAL_SIZE_BITS_8_C | SERIAL_ASYNC | SERIAL_PARITY_DISABLED | SERIAL_STOP_BITS_2
/// 5-bit Characters Size, Even Parity, 1-Stop Bit
#define SERIAL_5E1 SERIAL_SIZE_BITS_5_C | SERIAL_ASYNC | SERIAL_PARITY_EVEN | SERIAL_STOP_BITS_1
/// 6-bit Characters Size, Even Parity, 1-Stop Bit
#define SERIAL_6E1 SERIAL_SIZE_BITS_6_C | SERIAL_ASYNC | SERIAL_PARITY_EVEN | SERIAL_STOP_BITS_1
/// 7-bit Characters Size, Even Parity, 1-Stop Bit
#define SERIAL_7E1 SERIAL_SIZE_BITS_7_C | SERIAL_ASYNC | SERIAL_PARITY_EVEN | SERIAL_STOP_BITS_1
/// 8-bit Characters Size, Even Parity, 1-Stop Bit
#define SERIAL_8E1 SERIAL_SIZE_BITS_8_C | SERIAL_ASYNC | SERIAL_PARITY_EVEN | SERIAL_STOP_BITS_1
/// 5-bit Characters Size, Even Parity, 2-Stop Bits
#define SERIAL_5E2 SERIAL_SIZE_BITS_5_C | SERIAL_ASYNC | SERIAL_PARITY_EVEN | SERIAL_STOP_BITS_2
/// 6-bit Characters Size, Even Parity, 2-Stop Bits
#define SERIAL_6E2 SERIAL_SIZE_BITS_6_C | SERIAL_ASYNC | SERIAL_PARITY_EVEN | SERIAL_STOP_BITS_2
/// 7-bit Characters Size, Even Parity, 2-Stop Bits
#define SERIAL_7E2 SERIAL_SIZE_BITS_7_C | SERIAL_ASYNC | SERIAL_PARITY_EVEN | SERIAL_STOP_BITS_2
/// 8-bit Characters Size, Even Parity, 2-Stop Bits
#define SERIAL_8E2 SERIAL_SIZE_BITS_8_C | SERIAL_ASYNC | SERIAL_PARITY_EVEN | SERIAL_STOP_BITS_2
/// 5-bit Characters Size, Odd Parity, 1-Stop Bit
#define SERIAL_5O1 SERIAL_SIZE_BITS_5_C | SERIAL_ASYNC | SERIAL_PARITY_ODD  | SERIAL_STOP_BITS_1
/// 6-bit Characters Size, Odd Parity, 1-Stop Bit
#define SERIAL_6O1 SERIAL_SIZE_BITS_6_C | SERIAL_ASYNC | SERIAL_PARITY_ODD  | SERIAL_STOP_BITS_1
/// 7-bit Characters Size, Odd Parity, 1-Stop Bit
#define SERIAL_7O1 SERIAL_SIZE_BITS_7_C | SERIAL_ASYNC | SERIAL_PARITY_ODD  | SERIAL_STOP_BITS_1
/// 8-bit Characters Size, Odd Parity, 1-Stop Bit
#define SERIAL_8O1 SERIAL_SIZE_BITS_8_C | SERIAL_ASYNC | SERIAL_PARITY_ODD  | SERIAL_STOP_BITS_1
/// 5-bit Characters Size, Odd Parity, 2-Stop Bit
#define SERIAL_5O2 SERIAL_SIZE_BITS_5_C | SERIAL_ASYNC | SERIAL_PARITY_ODD  | SERIAL_STOP_BITS_2
/// 6-bit Characters Size, Odd Parity, 2-Stop Bit
#define SERIAL_6O2 SERIAL_SIZE_BITS_6_C | SERIAL_ASYNC | SERIAL_PARITY_ODD  | SERIAL_STOP_BITS_2
/// 7-bit Characters Size, Odd Parity, 2-Stop Bit
#define SERIAL_7O2 SERIAL_SIZE_BITS_7_C | SERIAL_ASYNC | SERIAL_PARITY_ODD  | SERIAL_STOP_BITS_2
/// 8-bit Characters Size, Odd Parity, 2-Stop Bit
#define SERIAL_8O2 SERIAL_SIZE_BITS_8_C | SERIAL_ASYNC | SERIAL_PARITY_ODD  | SERIAL_STOP_BITS_2

// do not alter lines after this one
#if !defined(UPE) && defined(PE)
#define UPE PE
#endif

#if defined(AVR_CPU_SERIES_0)
#define	UPE0	UPE	
#define	UDR0	UDR	
#define RXEN0	RXEN
#define TXEN0	TXEN
#define RXCIE0	RXCIE
#define TXC0	TXC
#define U2X0	U2X
#define MPCM0	MPCM
#define	UDRIE0	UDRIE
#define	UCSR0A	UCSRA
#define	UCSR0B	UCSRB
#define	UCSR0C	UCSRC
#define	UBRR0L	UBRRL
#define	UBRR0H	UBRRH
#define UCSZ00	UCSZ0
#define UCSZ01	UCSZ1
#define UCSZ02	UCSZ2
#define USART0_RX_vect USART_RXC_vect
#define USART0_UDRE_vect USART_UDRE_vect
#endif

#endif /* SERIALDEFS_H_ */