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

/// Terminal color definitions.
typedef enum {
/// Black
	  SerTermBlack = 0
/// Red
	, SerTermRed = 1
/// Green
	, SerTermGreen = 2
/// Yellow
	, SerTermYellow = 3
/// Blue
	, SerTermBlue = 4
/// Magenta
	, SerTermMagenta = 5
/// Cyan
	, SerTermCyan = 6
/// White
	, SerTermWhite = 7
/// Dark Gray (only for text)
	, SerTermBrightBlack = 8
/// Bright Red (only for text)
	, SerTermBrightRed = 9
/// Bright Green (only for text)
	, SerTermBrightGreen = 10
/// Bright Yellow (only for text)
	, SerTermBrightYellow = 11
/// Bright Blue (only for text)
	, SerTermBrightBlue = 12
/// Bright Magenta (only for text)
	, SerTermBrightMagenta = 13
/// Bright Cyan (only for text)
	, SerTermBrightCyan = 14
/// Bright White (only for text)
	, SerTermBrightWhite = 15
} SerTermColor;

/**
Simplified initialization of the Hardware Serial interface with specific baud rate.
It uses 8 bit transmission with 1 stop bit and no parity check 
@param baud Speed of transmission.
*/
void serInit(unsigned long baud);

/**
Initialization of the Hardware Serial interface with specific baud rate and configuration.
@param baud Speed of transmission.
@param config Serial configuration of transmission as defined in SerialDefs.h file
*/
void serInitComplete(unsigned long baud, uint8_t config);

/**
Makes sure that all data in transmission serial buffer are transmitted.
Waits until transmission buffer is empty and data are transferred. 
*/
void serFlush(void);

/**
Flushes existing transmission buffer then closes serial transmission,
serial receiver buffer is also cleared.
*/
void serEnd(void);

/**
Returns available bytes in the serial receive buffer.
@result available bytes in the serial receive buffer.
*/
int serAvailable(void);

/**
Peeks for the next character in the serial buffer without changing buffer status.
Returns -1 in case buffer is empty.
@result Next character in serial receive buffer or -1 in case buffer is empty.
*/
int serPeek(void);

/**
Gets the next character in the serial buffer. Returns -1 in case buffer is empty.
@result Next character in serial receive buffer or -1 in case buffer is empty.
*/
int serRead(void);

/**
Define character to be used as line terminator for #serAvailableLines and #serReadLine functions.
Default value is '\n' for regular ASCII transmission. Useful for AT+ commands.
@param terminator single character to be used as line terminator.
*/
void serSetTerminationCharacter(unsigned char terminator);

/**
Returns available lines in serial receive buffer separated 
by character defined in #serSetTerminationCharacter function.
@result available lines in serial receive buffer.
*/
int serAvailableLines(void);

/**
Copies the next line from serial receive buffer to buf variable.
The length of copy is limited to maxLen value to avoid buffer overflows.
@result buf Target buffer to which content of the next line from Serial receive buffer will be copied.
@param maxLen Maximum length of the copied buffer. To be set to the available buffer length.
*/
void serReadLine(char *buf, uint8_t maxLen);

/**
Returns number of available bytes in the serial transmission buffer.
Allows to react to situation ween buffer is full in the main program.
The serial interface automatically handles it also in all write function anyway.
@result Number of bytes currently available in transmission buffer.
*/
int serAvailableForWrite(void);

/**
Writes single character to the serial interface.
@param c Single character to be sent to serial interface.
@result Returns 1 in case character has been sent or buffered.
*/
size_t serWriteChar(uint8_t c);

/**
Sends string terminated by '\0' to serial interface.
@param str String to be sent to serial interface.
*/
void serPrintString(const char *str);

/**
Sends string from program memory terminated by '\0' to serial interface.
@param str String in program memory to be sent to serial interface.
*/
void serPrintString_P(const char *str);

/**
Sends line termination characters to serial interface.
*/
void serPrintLn(void);

/**
Sends string terminated by '\0' to serial interface and ends it with line termination.
@param str String to be sent to serial interface.
*/
void serPrintLnString(const char *str);

/**
Sends string from program memory terminated by '\0' to serial interface
and ends it with line termination.
@param str String in program memory to be sent to serial interface.
*/
void serPrintLnString_P(const char *str);

/**
Sends specific number converted to base value to the serial interface.
The conversion is done by itoa() function.
@param n Numeric value.
@param base Base of the numeric value to be converted to.
*/
void serPrintNumber(unsigned long n, uint8_t base);

/**
Sends specific number as HEX value prefixed with "0x" to serial interface.
@param n Numeric value.
*/
void serPrintHex(unsigned long n);

/**
Sends specific number as DEC value (not prefixed) to serial interface.
@param n Numeric value.
*/
void serPrintDec(unsigned long n);

/**
Sends specific number as OCT value prefixed with "0" to serial interface.
@param n Numeric value.
*/
void serPrintOct(unsigned long n);

/**
Sends len values from binary buffer buf to serial interface.
Receiving binary data from serial interface should be done by 
#serAvailable #serPeek and #serRead functions. 
Parsing needs to be done on the higher level routines.
@param *buf Buffer of the binary data.
@param len Number of bytes to be sent to serial interface.
*/
void serSendBinary(const uint8_t *buf, uint8_t len);

/**
Sends terminal command to clear screen.
*/
void serTermClearScreen(void);

/**
Sends terminal command to position cursor in home location (top left corner).
*/
void serTermCursorHome(void);

/**
Sets ANSI text color in the terminal.
@param color The color of the text from now on.
*/
void serTermTextColor(SerTermColor color);

/**
Sets ANSI background color in the terminal.
@param color The color of the background from now on.
*/
void serTermBackColor(SerTermColor color);

/**
Resets ANSI attributes of background and text color.
*/
void serTermResetAttributes(void);

/**
Sets ANSI text to bold / intensive color.
*/
void serTermBold(void);

/**
Sets ANSI text to faint color.
*/
void serTermFaint(void);

/**
Sets ANSI text to underline.
*/
void serTermUnderline(void);

/**
Sets ANSI text to blink. In PuTTY it changes background color.
*/
void serTermBlink(void);

#endif /* SERIAL_H_ */