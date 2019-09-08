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

/// Definition of the software timer next step time value.
typedef uint16_t SwtValueType;

/// User data remapping macro for callback registration
#define SWT_USER_DATA(X) (void *)(X)

/**
Definition of the Software Timer Callback
@param Pointer for void content that is registered in #swtRegisterCallback function
and will be delivered to callback.
@param SwtValueType Need to be altered to the next interval for the Software Timer.
If not changed, this will be the last call of specific software timer, and the next
time it needs to be reissued with #swtStart. This can be useful to define "single shot"
type of software timer.
*/
typedef void SwtCallback(void *, SwtValueType *);

/**
Typical main resolution interval of the software timers defined in milliseconds.
This value can be passed to #swtInit function, but is not used in 
internal routines.
*/
#define SWT_INTERVAL_1MS 1
/// Another typical resolution interval.
#define SWT_INTERVAL_10MS 10

/// To be used as reference to software timer 0
#define SWT_TIMER_0 0
/// To be used as reference to software timer 1
#define SWT_TIMER_1 1
/// To be used as reference to software timer 2
#define SWT_TIMER_2 2
/// To be used as reference to software timer 3
#define SWT_TIMER_3 3
/// To be used as reference to software timer 4
#define SWT_TIMER_4 4
/// To be used as reference to software timer 5
#define SWT_TIMER_5 5
/// To be used as reference to software timer 6
#define SWT_TIMER_6 6
/// To be used as reference to software timer 7
#define SWT_TIMER_7 7
/// To be used as reference to software timer 8
#define SWT_TIMER_8 8
/// To be used as reference to software timer 9
#define SWT_TIMER_9 9

/**
Initialization of the Software Timer routines with main .
\b NOTE: Although even seconds are possible, for typical applications and 
quartz values best option is to use 1ms.
Following definitions to be set in config.h file @n
#define \b SWT_SIZE_OF \\ Number of available software timers. To be adjusted to the needs.@n
@code
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void callbackDebugLed(void *userData, SwtValueType *newTimerValue)  {
	PORTC ^= 0x80;
	*newTimerValue = 500;
}

int main(void)
{
	DDRC |= 0x80;
	PORTC &= ~0x80;

	swtInit(SWT_INTERVAL_1MS);
	swtRegisterCallback(SWT_TIMER_0, 0, callbackDebugLed);
	swtStart(SWT_TIMER_0, 500);
	sei();
	while (true) {
		swtLoop();
		continue;
	}
@endcode
@param milisecondsResolution Main interval of the software timers defined in miliseconds.
*/
void swtInit(int16_t milisecondsResolution);

/**
Stops the all Software Timers. Use #swtInit to start it again.
*/
void swtDisable(void);

/**
To be periodically issued in the main loop.
*/
void swtLoop(void);

/**
Register and enable External Interrupt.
@param timerNo Number of software timer to which callback is registered to. 
Cannot exceed value SWT_SIZE_OF - 1.
@param userData User data void pointer to structure to be delivered to callback "as-is".
@param callback Pointer to the callback function to be issued on the specific interrupt.
*/
void swtRegisterCallback(uint8_t timerNo, void *userData, void (*callback)(void *, SwtValueType *));

/**
Unregisters and stops software timer of specific number
@param timerNo Number of software timer to which callback is registered to.
Cannot exceed value SWT_SIZE_OF - 1.
*/
void swtUnregisterCallback(uint8_t timerNo);

/**
Starts software timer of specific value with defined interval in miliseconds.
@param timerNo Number of software timer to which callback is registered to.
Cannot exceed value SWT_SIZE_OF - 1.
@param miliseconds number of initial miliseconds for the timer to be started.
The value should be multiplication of the value used in the @swtInit function.
*/
void swtStart(uint8_t timerNo, SwtValueType miliseconds);

#endif /* SOFTWARETIMER_H_ */