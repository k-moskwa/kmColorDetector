/** @file
 * @brief Common definitions for External Interrupt functions.
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
 */

#ifndef EXTERNALINTERRUPTDEFS_H_
#define EXTERNALINTERRUPTDEFS_H_

/// Direction register for External Interrupt Pins
#define EXT_INT_DDR DDRD
/// Port register for External Interrupt Pins
#define EXT_INT_PORT PORTD
/// Port pin for External Interrupt 0
#define EXT_INT_PIN_0 PD2
/// Port pin for External Interrupt 1
#define EXT_INT_PIN_1 PD3

/// Definition of the configuration bits for External Interrupt 0 - Low Level Sense
#define EXT_INT_CONF_0_LOW_LEVEL		0x00
/// Definition of the configuration bits for External Interrupt 0 - Logical Change Sense
#define EXT_INT_CONF_0_LOGICAL_CHANGE					_BV(ISC00)
/// Definition of the configuration bits for External Interrupt 0 - Falling Edge Sense
#define EXT_INT_CONF_0_FALLING_EDGE		_BV(ISC01)
/// Definition of the configuration bits for External Interrupt 0 - Rising Edge Sense
#define EXT_INT_CONF_0_RISING_EDGE		_BV(ISC01) |	_BV(ISC00)
/// Mask for all configuration bits of External Interrupt 0
#define EXT_INT_CONF_0_ALL_BITS			EXT_INT_CONF_0_RISING_EDGE

/// Definition of the configuration bits for External Interrupt 1 - Low Level Sense
#define EXT_INT_CONF_1_LOW_LEVEL		0x00
/// Definition of the configuration bits for External Interrupt 1 - Logical Change Sense
#define EXT_INT_CONF_1_LOGICAL_CHANGE					_BV(ISC10)
/// Definition of the configuration bits for External Interrupt 1 - Falling Edge Sense
#define EXT_INT_CONF_1_FALLING_EDGE		_BV(ISC11)
/// Definition of the configuration bits for External Interrupt 1 - Rising Edge Sense
#define EXT_INT_CONF_1_RISING_EDGE		_BV(ISC11) |	_BV(ISC10)
/// Mask for all configuration bits of External Interrupt 1
#define EXT_INT_CONF_1_ALL_BITS			EXT_INT_CONF_1_RISING_EDGE

#if defined(AVR_CPU_SERIES_0)
#define EXT_INT_SENSE_REG MCUR
#define EXT_INT_MASK_REG GICR
#endif

#if defined(AVR_CPU_SERIES_48)
#define EXT_INT_SENSE_REG EICRA
#define EXT_INT_MASK_REG EIMSK
#endif


#endif /* EXTERNALINTERRUPTDEFS_H_ */