/** @file
 * @brief Project configuration definitions.
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
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/// Default locale file for available translations
#define KMCD_LOCALE "localeEn.h"
//#define KMCD_LOCALE "localePl.h"

/// Enable to setup configuration for ATB Prototype board.
#define ATB_PROTOTYPE
/// disables debug functionalities based on LED (port C, pin 6 & 7).
//#define KMCD_NO_DEBUG
/// Disables LCD functionalities.
//#define KMCD_NO_LCD
/// Disables debug functionalities based on serial port (speed 9600 baud).
//#define KMCD_NO_SERIAL_DEBUG
/// Disables EEPROM settings functionalities.
#define KMCD_NO_EEPROM
/** Disables DF Player Mini based on serial port (speed 9600 baud).@n
\b NOTE!!! Never enable or connect both Serial Debug and DF Player.@n
-# https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
*/
#define KMCD_NO_DF_PLAYER
/// Serial speed for DF Player Mini
#define PLYR_SERIAL_BAUD_RATE 9600

/// Direction register for TCS3200 sensor module
#define TSC_DDR    DDRD
/// Port register for TCS3200 sensor module
#define TSC_PORT PORTD
/// Port pin for OUT wire TCS3200 sensor module
#define TSC_PIN_OUT        PIN2
/// Port pin for LED wire TCS3200 sensor module
#define TSC_PIN_LED        PIN3
/// Port pin for S0 wire TCS3200 sensor module
#define TSC_PIN_S0        PIN4
/// Port pin for S1 wire TCS3200 sensor module
#define TSC_PIN_S1        PIN5
/// Port pin for S2 wire TCS3200 sensor module
#define TSC_PIN_S2        PIN6
/// Port pin for S3 wire TCS3200 sensor module
#define TSC_PIN_S3        PIN7

/// Direction register for debug LED pins
#define DEBUG_DDR DDRC
/// Port register for debug LED pins
#define DEBUG_PORT PORTC
/// LED 6 debug pin
#define DEBUG_PIN_6 PC6
/// LED 7 debug pin
#define DEBUG_PIN_7 PC7
/// Definition of the pin number showing the main status
#define DEBUG_MAIN_PIN DEBUG_PIN_7
/// Definition of the pin number showing the button status
#define DEBUG_BUTTON_PIN DEBUG_PIN_6

/// Direction register for button pin
#define BUTTON_DDR DDRC
/// Port register for button pin
#define BUTTON_PORT PORTC
/// Input port register for button pin
#define BUTTON_PORT_IN PINC
/// Button pin number
#define BUTTON_PIN PC0

#ifndef ATB_PROTOTYPE
/// Direction register for LCD display
#define LCD_DDR DDRC
/// Port register for LCD display
#define LCD_PORT PORTC
#else
// Following settings will be used in case ATB_PROTOTYPE prototype is defined
#define LCD_DDR DDRA
#define LCD_PORT PORTA
#endif

/// Number of columns in LCD display
#define LCD_COLS 16
/// Number of rows in LCD display
#define LCD_ROWS 2
/// Auxiliary value for definitions of the string buffers used for LCD operations.
#define STR_INTERNAL_BUFFERS_SIZE_OF LCD_COLS + 1

/// Interval of the debug blink in Software Timer
#define DEBUG_BLINK_INTERVAL 500 // 500 ms
/// Interval of checking button state in Software Timer
#define BUTTON_CHECK_INTERVAL 20 // 20 ms

/// Maximum available color models
#define KMCD_MAX_COLOR_MODELS 16

/// Magic string for EEPROM settings
#define KMCD_MAGIC "KMCD100"
/// Length of magic string
#define KMCD_MAGIC_LENGTH 8

/// Number of available software timers. To be adjusted to the needs.
#define SWT_SIZE_OF 2

// Automatic calculation of the ports depending on above values.
// Do not alter this part unless you know what you do.
#ifndef NDEBUG
#define KMCD_NO_LCD
#endif

#endif /* CONFIG_H_ */