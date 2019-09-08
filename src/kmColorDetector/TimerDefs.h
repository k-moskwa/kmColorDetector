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
 */


#ifndef TIMERDEFS_H_
#define TIMERDEFS_H_

#include "common.h"

#include <avr/io.h>

/// Clock select of Timer0 - No clock source (Timer/Counter stopped).
#define TCC0_STOP		0x00
/// Clock select of Timer0 - Prescaler 1 / 1
#define TCC0_PRSC_1								_BV(CS00)
/// Clock select of Timer0 - Prescaler 1 / 8
#define TCC0_PRSC_8					_BV(CS01)
/// Clock select of Timer0 - Prescaler 1 / 64
#define TCC0_PRSC_64				_BV(CS01) |	_BV(CS00)
/// Clock select of Timer0 - Prescaler 1 / 256
#define TCC0_PRSC_256	_BV(CS02)
/// Clock select of Timer0 - Prescaler 1 / 1024
#define TCC0_PRSC_1024	_BV(CS02) |				_BV(CS00)
/// External clock source on T0 pin. Clock on falling edge.
#define TCC0_EXT_T0_FAL	_BV(CS02) |	_BV(CS01)
/// External clock source on T0 pin. Clock on rising edge.
#define TCC0_EXT_T0_RIS	_BV(CS02) |	_BV(CS01) |	_BV(CS00)
/// Prescaler mask for Timer2 (allows to clear prescaler)
#define TCC0_CS_MASK	TCC0_EXT_T0_RIS

/// Timer0 mode 0 - Normal top value 0xFF
#define TCC_0_MODE_0			0x00
/// Timer0 mode 1 - PWM, Phase Correct top 0xFF
#define TCC_0_MODE_1							_BV(WGM00)
/// Timer0 mode 2 - CTC top value OCR0
#define TCC_0_MODE_2			_BV(WGM01)
/// Timer0 mode 3 - Fast PWM top value 0xFF
#define TCC_0_MODE_3			_BV(WGM01) |	_BV(WGM00)

/// Normal port operation for compare match, OC0 disconnected
#define TCC0_COMP_OUT_NORMAL	0x00
/// Toggle OC0 on compare match
#define TCC0_COMP_OUT_TOGGLE					_BV(COM00)
/// Clear OC0 on compare match
#define TCC0_COMP_OUT_CLEAR		_BV(COM01)
/// Set OC0 on compare match
#define TCC0_COMP_OUT_SET		_BV(COM01) |	_BV(COM00)

/// Normal port operation for compare match, OC0 disconnected
#define TCC0_PWM_COMP_OUT_NORMAL				0x00
/// Reserver
#define TCC0_PWM_COMP_OUT_RESERVED								_BV(COM00)
/// Clear OC0 on compare match when up-counting, set OC0 on down-counting
#define TCC0_PWM_COMP_OUT_CLEAR_UP_SET_DOWN		_BV(COM01)
/// Set OC0 on compare match when down-counting, clear OC0 on up-counting
#define TCC0_PWM_COMP_OUT_SET_UP_CLEAR_DOWN		_BV(COM01) |	_BV(COM00)

/// Clock select of Timer1 - Disabled
#define TCC1_STOP		0x00
/// Clock select of Timer1 - Prescaler 1 / 1
#define TCC1_PRSC_1								_BV(CS10)
/// Clock select of Timer1 - Prescaler 1 / 8
#define TCC1_PRSC_8					_BV(CS11)
/// Clock select of Timer1 - Prescaler 1 / 64
#define TCC1_PRSC_64				_BV(CS11) |	_BV(CS10)
/// Clock select of Timer1 - Prescaler 1 / 256
#define TCC1_PRSC_256	_BV(CS12)
/// Clock select of Timer1 - Prescaler 1 / 1024
#define TCC1_PRSC_1024	_BV(CS12) |				_BV(CS10)
/// Clock select of Timer1 - Enabled on falling signal
#define TCC1_EXT_T1_FAL	_BV(CS12) |	_BV(CS11)
/// Clock select of Timer1 - Enabled on rising signal
#define TCC1_EXT_T1_RIS	_BV(CS12) |	_BV(CS11) |	_BV(CS10)
/// Prescaler mask for Timer2 (allows to clear prescaler)
#define TCC1_CS_MASK	TCC1_EXT_T1_RIS

/// Timer1 mode 0 - Normal top value 0xFFFF
#define TCC_1_MODE_0_A	0x00
#define TCC_1_MODE_0_B	0x00
/// Timer1 mode 1 - PWM, Phase Correct, 8-bit top value 0x00FF
#define TCC_1_MODE_1_A													_BV(WGM10)
#define TCC_1_MODE_1_B 0x00
/// Timer1 mode 2 - PWM, Phase Correct, 9-bit top value 0x01FF
#define TCC_1_MODE_2_A									_BV(WGM11)
#define TCC_1_MODE_2_B 0x00
/// Timer1 mode 3 - PWM, Phase Correct, 10-bit top value 0x03FF
#define TCC_1_MODE_3_A									_BV(WGM11) |	_BV(WGM10)
#define TCC_1_MODE_3_B 0x00
/// Timer1 mode 4 - CTC with OCR1A as top
#define TCC_1_MODE_4_A 0x00
#define TCC_1_MODE_4_B					_BV(WGM12)
/// Timer1 mode 5 - Fast PWM, 8-bit top value 0x00FF
#define TCC_1_MODE_5_A													_BV(WGM10)
#define TCC_1_MODE_5_B					_BV(WGM12)
/// Timer1 mode 6 - Fast PWM, 9-bit top value 0x01FF
#define TCC_1_MODE_6_A									_BV(WGM11)
#define TCC_1_MODE_6_B					_BV(WGM12)
/// Timer1 mode 7 - Fast PWM, 10-bit top value 0x03FF
#define TCC_1_MODE_7_A									_BV(WGM11) |	_BV(WGM10)
#define TCC_1_MODE_7_B					_BV(WGM12)
/// Timer1 mode 8 - PWM, Phase and Frequency Correct with ICR1 as top
#define TCC_1_MODE_8_A 0x00
#define TCC_1_MODE_8_B	_BV(WGM13)
/// Timer1 mode 9 - PWM, Phase and Frequency Correct with OCR1A as top
#define TCC_1_MODE_9_A													_BV(WGM10)
#define TCC_1_MODE_9_B	_BV(WGM13)
/// Timer1 mode 10 - PWM, Phase Correct with ICR1 as top
#define TCC_1_MODE_A_A									_BV(WGM11)
#define TCC_1_MODE_A_B	_BV(WGM13)
/// Timer1 mode 11 - PWM, Phase Correct with OCR1A as top
#define TCC_1_MODE_B_A									_BV(WGM11) |	_BV(WGM10)
#define TCC_1_MODE_B_B	_BV(WGM13)
/// Timer1 mode 12 - CTC with ICR1 as top
#define TCC_1_MODE_C_A	0x00
#define TCC_1_MODE_C_B	_BV(WGM13) |	_BV(WGM12)
/// Timer1 mode 13 - Reserved
#define TCC_1_MODE_D_A													_BV(WGM10)
#define TCC_1_MODE_D_B	_BV(WGM13) |	_BV(WGM12)
/// Timer1 mode 14 - Fast PWM with ICR1 as top
#define TCC_1_MODE_E_A									_BV(WGM11) |
#define TCC_1_MODE_E_B	_BV(WGM13) |	_BV(WGM12)
/// Timer1 mode 15 - Fast PWM with OCR1A as top
#define TCC_1_MODE_F_A									_BV(WGM11) |	_BV(WGM10)
#define TCC_1_MODE_F_B	_BV(WGM13) |	_BV(WGM12)

/// Normal port operation for compare match
#define TCC1_A_COMP_OUT_NONE	0x00
/// Toggle OC1A on compare match
#define TCC1_A_COMP_OUT_TOGGLE					_BV(COM1A0)
/// Clear OC1A on compare match (set output to low level)
#define TCC1_A_COMP_OUT_CLEAR	_BV(COM1A1)
/// Set OC1A on compare match (set output to high level)
#define TCC1_A_COMP_OUT_SET		_BV(COM1A1) |	_BV(COM1A0)

/// Normal port operation for compare match
#define TCC1_B_COMP_OUT_NONE	0x00
/// Toggle OC1A on compare match
#define TCC1_B_COMP_OUT_TOGGLE					_BV(COM1B0)
/// Clear OC1A on compare match (set output to low level)
#define TCC1_B_COMP_OUT_CLEAR	_BV(COM1B1)
/// Set OC1A on compare match (set output to high level)
#define TCC1_B_COMP_OUT_SET		_BV(COM1B1) |	_BV(COM1B0)

#if defined(__AVR_ATmega8__)
/// Direction register o Timer1 PWM outputs
#define TCC1_PWM_DDR DDRB
/// Port register of Timer1 PWM outputs
#define TCC1_PWM_PORT PORTB
/// PWM pin A of Timer1
#define TCC1_PWM_PIN_A PB1
/// PMW pin B of Timer1
#define TCC1_PWM_PIN_B PB2
/// Timer/Counter1 Input Capture Pin
#define TCC1_ICP1_PIN PB4
///
#define TCC1_T0_PIN PD4
#endif

#if defined(__AVR_ATmega32__)
/// Direction register o Timer1 PWM outputs
#define TCC1_PWM_DDR DDRD
/// Port register of Timer1 PWM outputs
#define TCC1_PWM_PORT PORTD
/// PWM pin A of Timer1
#define TCC1_PWM_PIN_A PD5
/// PMW pin B of Timer1
#define TCC1_PWM_PIN_B PD4
/// Timer/Counter1 Input Capture Pin
#define TCC1_ICP1_PIN PD6
///
#define TCC1_T0_PIN PB0
#endif

/// Byte value of PWM pin A of Timer1
#define TCC_PWM_BV_A	_BV(TCC1_PWM_PIN_A)
/// Byte value of PWM pin B of Timer1
#define TCC_PWM_BV_B	_BV(TCC1_PWM_PIN_B)

/// Clock select of Timer2 - Disabled
#define TCC2_STOP	0x00
/// Clock select of Timer2 - Prescaler 1 / 1
#define TCC2_PRSC_1								_BV(CS20)
/// Clock select of Timer2 - Prescaler 1 / 8
#define TCC2_PRSC_8					_BV(CS21)
/// Clock select of Timer2 - Prescaler 1 / 32
#define TCC2_PRSC_32				_BV(CS21) |	_BV(CS20)
/// Clock select of Timer2 - Prescaler 1 / 64
#define TCC2_PRSC_64	_BV(CS22)
/// Clock select of Timer2 - Prescaler 1 / 128
#define TCC2_PRSC_128	_BV(CS22) |				_BV(CS20)
/// Clock select of Timer2 - Prescaler 1 / 256
#define TCC2_PRSC_256	_BV(CS22) |	_BV(CS21)
/// Clock select of Timer2 - Prescaler 1 / 1024
#define TCC2_PRSC_1024	_BV(CS22) |	_BV(CS21) |	_BV(CS20)
/// Prescaler mask for Timer2 (allows to clear prescaler)
#define TCC2_CS_MASK	TCC2_PRSC_1024

/// Timer2 mode 0 - Normal top value 0xFF
#define TCC_2_MODE_0			0x00
/// Timer2 mode 1 - PWM, Phase Correct top 0xFF
#define TCC_2_MODE_1							_BV(WGM20)
/// Timer2 mode 2 - CTC top value OCR2 
#define TCC_2_MODE_2			_BV(WGM21)
/// Timer2 mode 3 - Fast PWM top value 0xFF
#define TCC_2_MODE_3			_BV(WGM21) |	_BV(WGM20)

/// Normal port operation for compare match, OC2 disconnected
#define TCC2_COMP_OUT_NORMAL	0x00
/// Toggle OC2 on compare match
#define TCC2_COMP_OUT_TOGGLE					_BV(COM20)
/// Clear OC2 on compare match
#define TCC2_COMP_OUT_CLEAR		_BV(COM21)
/// Set OC2 on compare match
#define TCC2_COMP_OUT_SET		_BV(COM21) |	_BV(COM20)

/// Normal port operation for compare match, OC2 disconnected
#define TCC2_PWM_COMP_OUT_NORMAL				0x00
/// Reserver
#define TCC2_PWM_COMP_OUT_RESERVED								_BV(COM20)
/// Clear OC2 on compare match when up-counting, set OC2 on down-counting
#define TCC2_PWM_COMP_OUT_CLEAR_UP_SET_DOWN		_BV(COM21)
/// Set OC2 on compare match when down-counting, clear OC2 on up-counting
#define TCC2_PWM_COMP_OUT_SET_UP_CLEAR_DOWN		_BV(COM21) |	_BV(COM20)

/// Timers top value (255)
#define TCC_TOP_1		0xFF
/// Timers top value (4095)
#define TCC_TOP_2		0x0FFF
/// Timers top value (65535)
#define TCC_TOP_3		0xFFFF
/// Timers top value (1023)
#define TCC_TOP_4		0x03FF
/// Timers top value (511)
#define TCC_TOP_5		0x01FF

/// Timers bottom value
#define TCC_BOTOM		0x00

#endif /* TIMERDEFS_H_ */