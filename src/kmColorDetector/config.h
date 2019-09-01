/*
 * config.h
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


#ifndef CONFIG_H_
#define CONFIG_H_

#define KMCD_LOCALE "localeEn.h"

// enable to setup configuration for ATB Prototype board
#define ATB_PROTOTYPE
// disables debug functionalities based on LED (port C, pin 6 & 7)
//#define KMCD_NO_DEBUG
// disables LCD functionalities
//#define KMCD_NO_LCD
// disables debug functionalities based on serial port (speed 9600 baud)
//#define KMCD_NO_SERIAL_DEBUG
// disables DF Player Mini based on serial port (speed 9600 baud)
// NOTE!!! Never enable or connect both Serial Debug and DF Player
// https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
#define KMCD_NO_DF_PLAYER
#define PLYR_SERIAL_BAUD_RATE 9600

#define TSC_DDR    DDRD
#define TSC_PORT PORTD
#define TSC_PIN_OUT        PIN2
#define TSC_PIN_LED        PIN3
#define TSC_PIN_S0        PIN4
#define TSC_PIN_S1        PIN5
#define TSC_PIN_S2        PIN6
#define TSC_PIN_S3        PIN7

#define DEBUG_DDR DDRC
#define DEBUG_PORT PORTC
#define DEBUG_PIN_6 PC6
#define DEBUG_PIN_7 PC7

#define BUTTON_DDR DDRC
#define BUTTON_PORT PORTC
#define BUTTON_PORT_IN PINC
#define BUTTON_PIN PC0

#ifndef ATB_PROTOTYPE
#define LCD_DDR DDRC
#define LCD_PORT PORTC
#else
#define LCD_DDR DDRA
#define LCD_PORT PORTA
#endif
#define LCD_COLS 16
#define LCD_ROWS 2
#define STR_INTERNAL_BUFFERS_SIZE_OF LCD_COLS + 1

#define DEBUG_BLINK_INTERVAL 50 // 500 ms
#define BUTTON_CHECK_INTERVAL 2 // 20 ms

#define KMCD_MAX_COLOR_MODELS 16

#define KMCD_MAGIC "KMCD100"
#define KMCD_MAGIC_LENGTH 8

#define KMCD_NO_EEPROM

#ifdef DEBUG
#define KMCD_NO_LCD
#endif

#endif /* CONFIG_H_ */