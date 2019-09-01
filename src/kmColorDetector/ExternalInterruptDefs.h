/*
 * ExternalInterruptDefs.h
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

#ifndef EXTERNALINTERRUPTDEFS_H_
#define EXTERNALINTERRUPTDEFS_H_

#define EXT_INT_DDR DDRD
#define EXT_INT_PORT PORTD
#define EXT_INT_PIN_0 PD2
#define EXT_INT_PIN_1 PD3

#define EXT_INT_CONF_0_LOW_LEVEL		0x00
#define EXT_INT_CONF_0_LOGICAL_CHANGE					_BV(ISC00)
#define EXT_INT_CONF_0_FALLING_EDGE		_BV(ISC01)
#define EXT_INT_CONF_0_RISING_EDGE		_BV(ISC01) |	_BV(ISC00)
#define EXT_INT_CONF_0_ALL_BITS			EXT_INT_CONF_0_RISING_EDGE

#define EXT_INT_CONF_1_LOW_LEVEL		0x00
#define EXT_INT_CONF_1_LOGICAL_CHANGE					_BV(ISC10)
#define EXT_INT_CONF_1_FALLING_EDGE		_BV(ISC11)
#define EXT_INT_CONF_1_RISING_EDGE		_BV(ISC11) |	_BV(ISC10)
#define EXT_INT_CONF_1_ALL_BITS			EXT_INT_CONF_1_RISING_EDGE


#endif /* EXTERNALINTERRUPTDEFS_H_ */