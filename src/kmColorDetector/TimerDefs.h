 /** @file
 * @brief Common definitions for Timers.
 * TimerDefs.h
 *
 *  Created on: Aug 10, 2019
 *      Author: Krzysztof Moskwa
 *      License: GPL-3.0-or-later
 *
 *  Color detector based on AVR uC
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


#ifndef TIMERDEFS_H_
#define TIMERDEFS_H_

#include "common.h"

#include <avr/io.h>

#define TCC1_PRSC_NONE	0x00
#define TCC1_PRSC_1								_BV(CS10)
#define TCC1_PRSC_8					_BV(CS11)
#define TCC1_PRSC_64				_BV(CS11) |	_BV(CS10)
#define TCC1_PRSC_256	_BV(CS12)
#define TCC1_PRSC_1024	_BV(CS12) |				_BV(CS10)
#define TCC1_EXT_T1_FAL	_BV(CS12) |	_BV(CS11)
#define TCC1_EXT_T1_RIS	_BV(CS12) |	_BV(CS11) |	_BV(CS10)

#define TCC1_PWM_DDR PORTB
#define TCC1_PWM_PIN_A PB1
#define TCC1_PWM_PIN_B PB2


#define TCC2_PRSC_NONE	0x00
#define TCC2_PRSC_1								_BV(CS20)
#define TCC2_PRSC_8					_BV(CS21)
#define TCC2_PRSC_32				_BV(CS21) |	_BV(CS20)
#define TCC2_PRSC_64	_BV(CS22)
#define TCC2_PRSC_128	_BV(CS22) |				_BV(CS20)
#define TCC2_PRSC_256	_BV(CS22) |	_BV(CS21)
#define TCC2_PRSC_1024	_BV(CS22) |	_BV(CS21) |	_BV(CS20)

#define TCC_PWM_PIN_A	PB1
#define TCC_PWM_PIN_B	PB2

#define TCC_PWM_BV_A	_BV(PB1)
#define TCC_PWM_BV_B	_BV(PB2)

#define TCC_TOP_1		0xFF
#define TCC_TOP_2		0x0FFF
#define TCC_TOP_3		0xFFFF

#define TCC_BOTOM		0x00

#endif /* TIMERDEFS_H_ */