/** @file
 * @brief Software time implementation based on AVF Timer2.
 * SoftwareTimer.h
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

#ifndef SOFTWARETIMER_H_
#define SOFTWARETIMER_H_

#include "common.h"

typedef uint16_t SmtValueType;

//#define SWT_USE_TIMER 2
// Defined in milliseconds
#define SWT_INTERVAL 10

#define SWT_SIZE_OF 2

#define SWT_TIMER_0 0
#define SWT_TIMER_1 1
#define SWT_TIMER_2 2
#define SWT_TIMER_3 3
#define SWT_TIMER_4 4
#define SWT_TIMER_5 5
#define SWT_TIMER_6 6
#define SWT_TIMER_7 7
#define SWT_TIMER_8 8
#define SWT_TIMER_9 9

/**
*/#define SWT_USER_DATA(X) (void *)(X)

/**
*/
typedef void SwtCallback(void *, SmtValueType *);

/**
@param microseconds
*/
void swtInit(int32_t microseconds);

/**
*/
void swtLoop(void);

/**
@param timerNo
@param userData
@param callback
*/
void swtRegisterCallback(uint8_t timerNo, void *userData, void (*callback)(void *, SmtValueType *));

/**
@param timerNo
*/
void swtUnregisterCallback(uint8_t timerNo);

/**
@param timerNo
@param interval
*/
void swtStart(uint8_t timerNo, SmtValueType interval);

#endif /* SOFTWARETIMER_H_ */