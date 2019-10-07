/*
 * ExternalInterrupt.c
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
#include <stdbool.h>
#include <avr/interrupt.h>

#include "ExternalInterrupt.h"
#include "ExternalInterruptDefs.h"

ExtIntCallback *_extIntCallback0 = NULL;
ExtIntCallback *_extIntCallback1 = NULL;
void *_extIntUserData0 = NULL;
void *_extIntUserData1 = NULL;

void extIntRegisterCallback(ExtIntType type, ExtIntSense sense, bool pullup, ExtIntCallback *callback, void *userData) {
	switch (type) {
		case EXT_INT_0 : {
			_extIntCallback0 = callback;
			// Set pin as input (Using for interrupt INT0)
			EXT_INT_DDR &= ~_BV(EXT_INT_PIN_0);
			// Enable PD2 pull-up resistor
			if (pullup == true) {
				EXT_INT_PORT |= _BV(EXT_INT_PIN_0);
				} else {
				EXT_INT_PORT &= ~_BV(EXT_INT_PIN_0);
			}
			EXT_INT_SENSE_REG &= ~(EXT_INT_CONF_0_ALL_BITS);
			switch (sense) {
				case EXT_INT_LOGICAL_CHANGE : {
					EXT_INT_SENSE_REG |= EXT_INT_CONF_0_LOGICAL_CHANGE;
					break;
				}
				case EXT_INT_FALLING_EDGE : {
					EXT_INT_SENSE_REG |= EXT_INT_CONF_0_FALLING_EDGE;
					break;
				}
				case EXT_INT_RISING_EDGE : {
					EXT_INT_SENSE_REG |= EXT_INT_CONF_0_RISING_EDGE;
					break;
				}
				// Low Level
				default : {
					EXT_INT_SENSE_REG |= EXT_INT_CONF_0_LOW_LEVEL;
				}
			}
			EXT_INT_MASK_REG |= _BV(INT0);
			break;
		}
		case EXT_INT_1 : {
			_extIntCallback1 = callback;
			// Set pin as input (Using for interrupt INT0)
			EXT_INT_DDR &= ~_BV(EXT_INT_PIN_1);
			// Enable PD2 pull-up resistor
			if (pullup == true) {
				EXT_INT_PORT |= _BV(EXT_INT_PIN_1);
				} else {
				EXT_INT_PORT &= ~_BV(EXT_INT_PIN_1);
			}
			EXT_INT_SENSE_REG &= ~(EXT_INT_CONF_1_ALL_BITS);
			switch (sense) {
				case EXT_INT_LOGICAL_CHANGE : {
					EXT_INT_SENSE_REG |= EXT_INT_CONF_1_LOGICAL_CHANGE;
					break;
				}
				case EXT_INT_FALLING_EDGE : {
					EXT_INT_SENSE_REG |= EXT_INT_CONF_1_FALLING_EDGE;
					break;
				}
				case EXT_INT_RISING_EDGE : {
					EXT_INT_SENSE_REG |= EXT_INT_CONF_1_RISING_EDGE;
					break;
				}
				// Low Level
				default : {
					EXT_INT_SENSE_REG |= EXT_INT_CONF_1_LOW_LEVEL;
				}
			}
			EXT_INT_MASK_REG |= _BV(INT1);
			break;

		}
		// no default
	}
}

void extIntDisable(ExtIntType type) {
	switch (type) {
		case EXT_INT_0 : {
			EXT_INT_MASK_REG &= ~_BV(INT0);
			break;
		}
		case EXT_INT_1 : {
			EXT_INT_MASK_REG &= ~_BV(INT1);
			break;
		}
		// no default
	}
}

ISR(INT0_vect) {
	if (_extIntCallback0 != NULL) {
		_extIntCallback0(_extIntUserData0);
	}
}

ISR(INT1_vect) {
	if (_extIntCallback1 != NULL) {
		_extIntCallback1(_extIntUserData1);
	}
}
