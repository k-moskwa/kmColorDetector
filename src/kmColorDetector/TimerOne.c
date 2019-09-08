/*
 * TimerOne.c
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
 */

#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "TimerOne.h"
#include "Debug.h"

#define TCC1_TOP TCC_TOP_3

static uint8_t _timer1PrescalerSelectBits = 0;
static uint16_t _timer1PwmCycles = 0;
static Timer1Callback *_timerCallback = NULL;
static void *_timer1CallbackUserData = NULL;

void timer1Start(void) {
	TCCR1B |= _timer1PrescalerSelectBits;
}

void timer1Stop(void) {
	TCCR1B &= ~(TCC1_PRSC_1024);
}

void timer1Restart(void) {
	TCNT1 = 0;
}

void timer1SetPeriod(int32_t microseconds) {
	// the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
	int64_t cycles = (int64_t)(F_CPU);
	cycles *= microseconds;
	cycles /= 2000000ULL;
	if (cycles < TCC_TOP_3) {
		// no prescaler, full xtal
		_timer1PrescalerSelectBits = TCC1_PRSC_1;
	} else if ((cycles >>= 3) < TCC_TOP_3) {
		// prescaler by /8
		_timer1PrescalerSelectBits = TCC1_PRSC_8;
	} else if ((cycles >>= 3) < TCC_TOP_3) {
		// prescaler by /64
		_timer1PrescalerSelectBits = TCC1_PRSC_64;
	} else if ((cycles >>= 2) < TCC_TOP_3) {
		// prescaler by /256
		_timer1PrescalerSelectBits = TCC1_PRSC_256;
	} else if ((cycles >>= 2) < TCC1_TOP) {
		// prescaler by /1024
		_timer1PrescalerSelectBits = TCC1_PRSC_1024;
	} else {
		// request was out of bounds, set as maximum
		_timer1PrescalerSelectBits = TCC1_PRSC_1024;
		cycles = TCC1_TOP;
	}
	_timer1PwmCycles = cycles;
	ICR1 = _timer1PwmCycles;
	 // reset prescaler bits
	timer1Stop();
}

void timer1Init(int32_t microseconds) {
	// clear control register A
	// Set Timer1 mode 8 - PWM, Phase and Frequency Correct with ICR1 as top
	TCCR1A = TCC_1_MODE_8_A;
	TCCR1B = TCC_1_MODE_8_B;
	timer1SetPeriod(microseconds);
}

void timer1SetPwmDuty(Tcc1PwmOut pwmOut, uint16_t duty) {
	uint32_t dutyCycle = _timer1PwmCycles * duty;
	dutyCycle >>= 10;
	switch (pwmOut) {
		case TCC1_PWM_OUT_A: {
			OCR1A = dutyCycle;
			break;
		}
		case TCC1_PWM_OUT_B: {
			OCR1B = dutyCycle;
			break;
		}
		// no default
	}
}

void timer1EnablePwm(Tcc1PwmOut pwmOut, uint16_t duty, int32_t microseconds) {
	if (microseconds > 0) {
		timer1SetPeriod(microseconds);
	}
	switch (pwmOut) {
		case TCC1_PWM_OUT_A: {
			// sets data direction register for pwm output pin
			TCC1_PWM_DDR |= _BV(TCC1_PWM_PIN_A);
			// activates the output pin
			TCCR1A |= _BV(COM1A1);
			break;
		}
		case TCC1_PWM_OUT_B: {
			// sets data direction register for pwm output pin
			TCC1_PWM_DDR |= _BV(TCC1_PWM_PIN_B);
			// activates the output pin
			TCCR1A |= _BV(COM1B1);
			break;
		}
		// no default
	}
	timer1SetPwmDuty(pwmOut, duty);
	timer1Start();
}

void timer1DisablePwm(Tcc1PwmOut pwmOut) {
	switch (pwmOut) {
		case TCC1_PWM_OUT_A: {
			// deactivates the output pin
			TCCR1A &= ~_BV(COM1A1);
			break;
		}
		case TCC1_PWM_OUT_B: {
			// deactivates the output pin
			TCCR1A &= ~_BV(COM1B1);
			break;
		}
	// no default
	}
}

void timer1EnableInterrupt(void) {
	TIMSK |= _BV(TOIE1);
	timer1Start();
}

void timer1DisableInterrupt(void) {
	TIMSK &= ~_BV(TOIE1);
}

void timer1RegisterCallback(Timer1Callback *callback, void *userData) {
	_timerCallback = callback;
	_timer1CallbackUserData = userData;
}

void timer1SetCallbackUserData(void *userData)  {
     ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		_timer1CallbackUserData = userData;
	}
}

ISR(TIMER1_OVF_vect) {
	if (_timerCallback != NULL) {
		_timerCallback(_timer1CallbackUserData);
	}
}