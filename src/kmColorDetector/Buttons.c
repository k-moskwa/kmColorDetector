/*
 * Button.c
 *
 *  Created on: Jul 10, 2019
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
 */

#ifndef _TESTS_ENV
#include <avr/io.h>
#endif
#include <stdbool.h>
#include "config.h"
#include "Buttons.h"

static uint8_t _btnPin = 0;
static bool _btnPreviousState = false;
static bool _btnPressed = false;

// "Private" functions
bool btnGetState(void);

// Implementation
bool btnGetState(void) {
#ifndef _TESTS_ENV
	return bit_is_clear(BUTTON_PORT_IN, _btnPin);
#else
	return false;
#endif
}

void btnInit(uint8_t pin) {
	_btnPin = pin;
	_btnPreviousState = false;
	_btnPressed = false;

	// set button pin as input
	BUTTON_DDR &=~ _BV(_btnPin);
	// pull-up disabled for selected pins cause implemented on HW level
	//BUTTON_PORT &= ~_BV(_btnPin);
	BUTTON_PORT |= _BV(_btnPin);
}

void btnLoop(void) {
	bool currentState = btnGetState();
	if (currentState != _btnPreviousState) {
		if (currentState == false) {
			_btnPressed = true;
		}
		_btnPreviousState = currentState;
	}
}

bool btnPressed(void) {
	return _btnPressed;
}

void btnReset(void) {
	_btnPressed = false;
}
