/** @file
 * @brief Functions for hardware serial implementation
 * Serial.h
 *
 *  Created on: Aug 10, 2019
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
 *  References:
 * -# https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Print.h
 * -# https://github.com/benlaurie/arduino--/blob/master/serial.h
 * -# https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.cpp
 * -# https://appelsiini.net/2011/simple-usart-with-avr-libc/
 * -# https://pidpawel.eu/post/makra-sbi-cbi-tbi-dla-avr.html
 */


#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "SerialDefs.h"

/**
@param terminator
*/
void serSetTerminationCharacter(unsigned char terminator);

/**
@param baud
*/
void serInit(unsigned long baud);

/**
@param baud
@param config
*/
void serInitComplete(unsigned long baud, uint8_t config);

/**
*/
void serFlush(void);

/**
*/
void serEnd(void);

/**
*/
int serAvailable(void);

/**
*/
int serPeek(void);

/**
*/
int serRead(void);

/**
*/
int serAvailableLines(void);

/**
@param buf
@param maxLen
*/
void serReadLine(char *buf, uint8_t maxLen);

/**
*/
int serAvailableForWrite(void);

/**
@param c
*/
size_t serWriteChar(uint8_t c);

/**
@param str
*/
void serPrintString(const char *str);

/**
@param str
*/
void serPrintString_P(const char *str);

/**
*/
void serPrintLn(void);

/**
@param str
*/
void serPrintLnString(const char *str);

/**
@param str
*/
void serPrintLnString_P(const char *str);

/**
@param n
@param base
*/
void serPrintNumber(unsigned long n, uint8_t base);

/**
@param n
*/
void serPrintHex(unsigned long n);

/**
@param n
*/
void serPrintDec(unsigned long n);

/**
@param n
*/
void serPrintOct(unsigned long n);

/**
@param *buf
@param len
*/
void serSendBinary(const uint8_t *buf, uint8_t len);

#endif /* SERIAL_H_ */