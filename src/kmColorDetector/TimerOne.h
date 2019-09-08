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
 * -# https://www.arduinolibraries.info/libraries/timer-one
 * -# https://github.com/PaulStoffregen/TimerOne
 */


#ifndef TIMERONE_H_
#define TIMERONE_H_

#include "common.h"

#include "TimerDefs.h"

/// User data remapping macro for callback registration
#define TIMER1_USER_DATA(X) (void *)(X)

/// Default value of the timer1 interval in microseconds.
#define TIMER1_DEFAULT_MICROSEDONDS 1000000 // 1s

/**
Definition of the Timer1 Callback
@param Pointer for void content that is registered in #timer1RegisterCallback function
and will be delivered to callback.
*/
typedef void Timer1Callback(void *);

/**
Definition of the PWM outputs for timer.
*/
typedef enum {
	/// PWM on output A.
	TCC1_PWM_OUT_A,
	/// PWM on output B.
	TCC1_PWM_OUT_B
} Tcc1PwmOut;

/**
Initializes timer with specific number of microseconds.
\b NOTE: This timer does not start automatically after initialization.
To start it use #timer1Start function.
@param microseconds Number of microseconds between timer intervals.
*/
void timer1Init(int32_t microseconds);

/**
Enables and starts timer. Can be disabled by #timer1Stop function.
*/
void timer1Start(void);

/**
Disables and stops the timer. Can be re-enabled by #timer1Start function.
*/
void timer1Stop(void);

/**
Restarts the timer by setting it's current value to 0.
*/
void timer1Restart(void);

/**
Defines main period of the timer in microseconds.
The internal routines use F_CPU variable for calculations.
@param microseconds Number of microseconds between timer intervals.
*/
void timer1SetPeriod(int32_t microseconds);


/**
Sets PWM duty of specific PWM output to duty value.
@param pwmOut Either TCC1_PWM_OUT_A or TCC1_PWM_OUT_B.
@param duty Duty value in range of 0 to 1024 (full duty); 512 is for 50%/50%
*/
void timer1SetPwmDuty(Tcc1PwmOut pwmOut, uint16_t duty);

/**
Enables PWM on specific PWM output, and sets duty value.
@param pwmOut Either TCC1_PWM_OUT_A or TCC1_PWM_OUT_B.
@param duty Duty value in range of 0 to 1024 (full duty); 512 is for 50%/50%
@param microseconds Number of microseconds of the full PWM cycle.
*/
void timer1EnablePwm(Tcc1PwmOut pwmOut, uint16_t duty, int32_t microseconds);

/**
Disables PWM on specific PWM output.
@param pwmOut Either TCC1_PWM_OUT_A or TCC1_PWM_OUT_B.
*/
void timer1DisablePwm(Tcc1PwmOut pwmOut);

/**
Enables interrupts for Timer1 allowing calls of callback registered with #timer1RegisterCallback.
*/
void timer1EnableInterrupt(void);

/**
Disables interrupts for Timer1.
*/
void timer1DisableInterrupt(void);

/**
Register callback for the Timer1.
In order to make it working explicit call to #timer1Start
and #timer1EnableInterrupt need to be done from program routines.
\b NOTE: The callback is issued within interrupt routine and is 
not buffered, so it should be as short as possible.
@param callback Pointer to the callback function to be issued on the interrupt.
@param userData User data void pointer to structure to be delivered to callback "as-is".
*/
void timer1RegisterCallback(Timer1Callback *callback, void *userData);

/**
Changes user data delivered to the callback.
@param userData User data void pointer to structure to be delivered to callback "as-is".
*/
void timer1SetCallbackUserData(void *userData);

#endif /* TIMERONE_H_ */