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

typedef struct {
	bool enabled;
	SmtValueType currentValue;
	SwtCallback *_timer1Callback;
	void *userData;
} swtItem;

static swtItem _timers[SWT_SIZE_OF];

void swtInit(int32_t microseconds) {
	for (int i = 0; i < SWT_SIZE_OF; i++) {
		_timers[i].enabled = false;
		_timers[i]._timer1Callback = NULL;
		_timers[i].currentValue = 0;
	}
	TCCR2 |= _BV(WGM21);
	TCCR2 |= TCC2_PRSC_1024;
	OCR2 = 108;
	TIMSK = _BV(OCIE2);
}

void swtLoop(void) {
	for (int i = 0; i < SWT_SIZE_OF; i++) {
		if (true == _timers[i].enabled && 0 == _timers[i].currentValue) {
			_timers[i]._timer1Callback(_timers[i].userData, &_timers[i].currentValue);
			if (_timers[i].currentValue == 0) {
			_timers[i].enabled  = false;
			}
		}
	}
}

void swtRegisterCallback(uint8_t timerNo, void *userData, void (*callback)(void *, SmtValueType *)) {
	_timers[timerNo].enabled = false;
	_timers[timerNo].currentValue = 0;
	_timers[timerNo]._timer1Callback = callback;
	_timers[timerNo].userData = userData;
}

void swtUnregisterCallback(uint8_t timerNo) {
	_timers[timerNo].enabled = false;
	_timers[timerNo].currentValue = 0;
	_timers[timerNo]._timer1Callback = NULL;
	_timers[timerNo].userData = NULL;
}

void swtStart(uint8_t timerNo, SmtValueType interval) {
	_timers[timerNo].enabled = true;
	_timers[timerNo].currentValue = interval;
}

ISR(TIMER2_COMP_vect) {
	for (int i = 0; i < SWT_SIZE_OF; i++) {
		if (true == _timers[i].enabled && _timers[i].currentValue > 0) {
			_timers[i].currentValue--;
		}
	}
}
