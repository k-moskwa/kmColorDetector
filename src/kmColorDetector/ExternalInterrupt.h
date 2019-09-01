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

/**
*/
typedef enum {
	EXT_INT_LOW_LEVEL,
	EXT_INT_LOGICAL_CHANGE,
	EXT_INT_FALLING_EDGE,
	EXT_INT_RISING_EDGE
} ExtIntSense;

/**
*/
typedef enum {
	EXT_INT_0,
	EXT_INT_1
} ExtIntType;

/**
*/
typedef void ExtIntCallback(void *);

/**
@param type
@param sense
@param pullup
@param callback
@param userData
*/
void extIntRegisterCallback(ExtIntType type, ExtIntSense sense, bool pullup, ExtIntCallback *callback, void *userData);

/**
@param type
*/
void extIntDisable(ExtIntType type);

#endif /* EXTERNALINTERRUPT_H_ */