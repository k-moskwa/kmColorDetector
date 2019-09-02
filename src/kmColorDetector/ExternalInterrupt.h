/** @file
 * @brief Functions for support for external interrupts.
 * ExternalInterrupt.h
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
 *
 */


#ifndef EXTERNALINTERRUPT_H_
#define EXTERNALINTERRUPT_H_

#include "common.h"

/// Definition of interrupt sense.
typedef enum {
	/// Interrupt on low level state.
	EXT_INT_LOW_LEVEL,
	/// Interrupt on any logical change.
	EXT_INT_LOGICAL_CHANGE,
	/// Interrupt on falling edge.
	EXT_INT_FALLING_EDGE,
	/// Interrupt on rising edge.
	EXT_INT_RISING_EDGE
} ExtIntSense;

/// Definition available interrupts
typedef enum {
	/// External interrupt 0 (available on PIN PD2 for ATmega32)
	EXT_INT_0,
	/// External interrupt 1 (available on PIN PD3 for ATmega32)
	EXT_INT_1
} ExtIntType;

/**
Definition of the External Interrupt Callback
@param Pointer for void content that is registered in #extIntRegisterCallback function 
and will be delivered to callback.
*/
typedef void ExtIntCallback(void *);

/**
Register and enable External Interrupt.
@param type Type of the interrupt, either #EXT_INT_0 or #EXT_INT_1.
@param sense Sense of the signal issuing interrupt.
@param pullup If true, the specific pin is going to be set to pull-up input.
@param callback Pointer to the callback function to be issued on the specific interrupt.
@param userData User data void pointer to structure to be delivered to callback "as-is".
*/
void extIntRegisterCallback(ExtIntType type, ExtIntSense sense, bool pullup, ExtIntCallback *callback, void *userData);

/**
Disable specific external interrupt, so it's not called anymore.
@param type Type of the interrupt, either #EXT_INT_0 or #EXT_INT_1.
*/
void extIntDisable(ExtIntType type);

#endif /* EXTERNALINTERRUPT_H_ */