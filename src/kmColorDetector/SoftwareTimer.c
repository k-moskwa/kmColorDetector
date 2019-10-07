/*
 * SoftwareTimer.c
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
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "TimerDefs.h"
#include "SoftwareTimer.h"

#define TCC2_TOP TCC_TOP_1

// Internal definition of types.
typedef struct {
	bool enabled;
	SwtValueType currentValue;
	SwtCallback *_timerCallback;
	void *userData;
} swtItem;

// "Private" global variables.
static swtItem _timers[SWT_SIZE_OF];
static volatile SwtValueType _mainInterval = 0;
static volatile uint16_t _softPrescallerInit = 0;
static volatile uint16_t _softPrescallerCurrent = 0;

// "Private" functions.
void timer2SetPeriod(int32_t miliseconds);
void swtInterrupt(void);

// Implementation
void swtInit(int16_t miliseconds) {
	_mainInterval = miliseconds;
	for (int i = 0; i < SWT_SIZE_OF; i++) {
		_timers[i].enabled = false;
		_timers[i]._timerCallback = NULL;
		_timers[i].currentValue = 0;
	}
	/// Timer2 mode 2 - CTC top value OCR2
#if defined(AVR_CPU_SERIES_0)
	TCCR2 |= TCC_2_MODE_2;
#endif
#if defined(AVR_CPU_SERIES_48)
	TCCR2A |= TCC_2_MODE_2;
	TCCR2B |= TCC_2_MODE_2_B;
#endif
	/// Timer/Counter2 Output Compare Match Interrupt Enable
#if defined(AVR_CPU_SERIES_0)
	TIMSK = _BV(OCIE2);
#endif
#if defined(AVR_CPU_SERIES_48)
	TIMSK2 = _BV(OCIE2A);
#endif
	// set prescaler and OCR2
	timer2SetPeriod(miliseconds);
}

void swtDisable(void) {
#if defined(AVR_CPU_SERIES_0)
	TCCR2 &= ~(TCC2_CS_MASK);
#endif
#if defined(AVR_CPU_SERIES_48)
	TCCR2B &= ~(TCC2_CS_MASK);
#endif
}

void swtLoop(void) {
	for (int i = 0; i < SWT_SIZE_OF; i++) {
		if (true == _timers[i].enabled && 0 == _timers[i].currentValue) {
			SwtValueType newCurrent = 0;
			_timers[i]._timerCallback(_timers[i].userData, &newCurrent);
			if (0 == newCurrent) {
				_timers[i].enabled = false;
			}
			_timers[i].currentValue = newCurrent / _mainInterval;
		}
	}
}

void swtRegisterCallback(uint8_t timerNo, void *userData, void (*callback)(void *, SwtValueType *)) {
	_timers[timerNo].enabled = false;
	_timers[timerNo].currentValue = 0;
	_timers[timerNo]._timerCallback = callback;
	_timers[timerNo].userData = userData;
}

void swtUnregisterCallback(uint8_t timerNo) {
	_timers[timerNo].enabled = false;
	_timers[timerNo].currentValue = 0;
	_timers[timerNo]._timerCallback = NULL;
	_timers[timerNo].userData = NULL;
}

void swtStart(uint8_t timerNo, SwtValueType miliseconds) {
	_timers[timerNo].enabled = true;
	_timers[timerNo].currentValue = miliseconds / _mainInterval;
}

void timer2SetPeriod(int32_t miliseconds) {
	int64_t cycles = (int64_t)(F_CPU);
	cycles *= miliseconds;
	cycles /= 1000ULL;
	uint8_t timer2PrescalerSelectBits = 0;
	if (cycles < TCC2_TOP) {
		// no prescaler, full XTAL
		timer2PrescalerSelectBits = TCC2_PRSC_1;
		} else if ((cycles >>= 3) < TCC2_TOP) {
		// prescaler by /8
		timer2PrescalerSelectBits = TCC2_PRSC_8;
		} else if ((cycles >>= 2) < TCC2_TOP) {
		// prescaler by /32
		timer2PrescalerSelectBits = TCC2_PRSC_32;
		} else if ((cycles >>= 1) < TCC2_TOP) {
		// prescaler by /64
		timer2PrescalerSelectBits = TCC2_PRSC_64;
		} else if ((cycles >>= 1) < TCC2_TOP) {
		// prescaler by /128
		timer2PrescalerSelectBits = TCC2_PRSC_128;
		} else if ((cycles >>= 1) < TCC2_TOP) {
		// prescaler by /256
		timer2PrescalerSelectBits = TCC2_PRSC_256;
		} else if ((cycles >>= 2) < TCC2_TOP) {
		// prescaler by /1024
		timer2PrescalerSelectBits = TCC2_PRSC_1024;
		} else {
		// use HW prescaler / 256 & soft prescaler to achieve 24 bit limit
		timer2PrescalerSelectBits = TCC2_PRSC_256;
		cycles = (int64_t)(F_CPU);	
		cycles *= miliseconds;
		cycles /= 1000ULL;
		cycles >>= 8;
		_softPrescallerInit = (cycles >> 8) & 0xFFFF;
		cycles &= 0xFF;
		}
#if defined(AVR_CPU_SERIES_0)
	OCR2 = cycles; // it should be already less than 0xFF
	TCCR2 |= timer2PrescalerSelectBits;
#endif
#if defined(AVR_CPU_SERIES_48)
	OCR2A = cycles; // it should be already less than 0xFF
	TCCR2B |= timer2PrescalerSelectBits;
#endif
	return;
}

void swtInterrupt(void) {
	if (_softPrescallerCurrent == 0) {
		for (int i = 0; i < SWT_SIZE_OF; i++) {
			if (true == _timers[i].enabled && _timers[i].currentValue > 0) {
				_timers[i].currentValue--;
			}
		}
		_softPrescallerCurrent = _softPrescallerInit;
		} else {
		_softPrescallerCurrent--;
	}
}

#if defined(AVR_CPU_SERIES_0)
ISR(TIMER2_COMP_vect) {
	swtInterrupt();
}
#endif
#if defined(AVR_CPU_SERIES_48)
ISR(TIMER2_COMPA_vect) {
	swtInterrupt();
}
#endif
