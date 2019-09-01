/** @file
 * @brief Hardware timer based on AVR Timer1 with callback and pwm functionalities.
 * TimerOne.h
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


#ifndef TIMERONE_H_
#define TIMERONE_H_

#include "common.h"

#include "TimerDefs.h"

/**
*/
#define TIMER1_USER_DATA(X) (void *)(X)
///
#define TIMER1_DEFAULT_MICROSEDONDS 1000000 // 1s

/**
*/
typedef void Timer1Callback(void *);

/**
*/
typedef enum {
	TCC1_PWM_OUT_A,
	TCC1_PWM_OUT_B
} Tcc1PwmOut;

/**
*/
void timer1Start(void);

/**
*/
void timer1Stop(void);

/**
*/
void timer1Restart(void);

/**
@param microseconds
*/
void timer1SetPeriod(int32_t microseconds);

/**
@param microseconds
*/
void timer1Init(int32_t microseconds);

/**
@param pwmOut
@param duty
*/
void timer1SetPwmDuty(Tcc1PwmOut pwmOut, uint16_t duty);

/**
@param pwmOut
@param duty
@param microseconds
*/
void timer1EnablePwm(Tcc1PwmOut pwmOut, uint16_t duty, int32_t microseconds);

/**
@param pwmOut
*/
void timer1DisablePwm(Tcc1PwmOut pwmOut);

/**
*/
void timer1EnableInterrupt(void);

/**
*/
void timer1DisableInterrupt(void);

/**
@param callback
@param userData
*/
void timer1RegisterCallback(Timer1Callback *callback, void *userData);

/**
@param userData
*/
void timer1SetCallbackUserData(void *userData);

#endif /* TIMERONE_H_ */