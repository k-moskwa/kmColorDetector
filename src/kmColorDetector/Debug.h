/** @file
 * @brief Debug and troubleshooting functions.
 * Debug.h
 *
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
 * -# https://atnel2.blogspot.com/2014/04/puapki-programowe-debuger-na-jednej.html
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "common.h"

/**
Initializes all ports to pull-up state.
\b NOTE: This method should be issued first in the main routine
*/
void dbPullUpAllPorts(void);

/**
Initializes the debug routines with specific number of dbStep calls to switch DB_PIN between states.
Following definitions to be set in config.h file @n
#define \b DEBUG_DDR data direction register for debug led (e.g DDRB) @n
#define \b DEBUG_PORT debug led port (e.g PORTB) @n
#define \b DEBUG_PINX debug led pin (e.g PB1) @n
to disable debug feature completely - define \b NO_PIN_DEBUG env variable
@param number of steps to switch debug pin from high to low and from low to high state
*/
void dbInit(void);

/**
Issues next debug by toggling the led of specific number.
@param pin Pin number from 0 to 7. Note that only pins that have defined DEBUG_PINX are active.
*/
void dbToggle(uint8_t pin);

/**
Send current color measure to serial interface if available.
This function uses Serial.h functions.
*/
void dbMeasureToSerial(void);

/**
Send current color measure to LCD if available.
This function uses LiquidCrystal.h functions
*/
void dbMeasureToLCD(void);

#endif /* DEBUG_H_ */
