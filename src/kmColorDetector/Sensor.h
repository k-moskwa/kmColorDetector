 /** @file
 * @brief Functions for TCS3200 Color Sensor.
 * Sensor.h
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
 * -# https://www.waveshare.com/color-sensor.htm
 * -# https://www.waveshare.com/wiki/Color_Sensor
 * -# https://www.waveshare.com/w/upload/6/60/Color-Sensor-UserManual.pdf
 * -# https://www.waveshare.com/w/upload/b/b2/Color-Sensor-Schematic.pdf
 * -# https://www.waveshare.com/wiki/Color-Sensor_Software
 * -# https://botland.com.pl/pl/content/153-wykrywanie-koloru-z-arduino 
 * -# http://www.waveshare.com/wiki/Color-Sensor_Software
 * -# https://github.com/MajicDesigns/MD_TCS230
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "common.h"
#include "ColorTools.h"

/// User data remapping macro for callback registration
#define TSC_USER_DATA(X) (void *)(X)

/**
Definition of the Color Sensor Callback
@param Pointer for void content that is registered in #tscRegisterCallbackMeasureFinished function
and will be delivered to callback.
*/typedef void TscCallback(void *);

/**
Initialize Color Sensor based on TCS3200
Following definitions to be set in config.h file @n
#define \b TSC_DDR  Direction register for TCS3200 sensor module@n
#define \b TSC_PORT PORTD Port register for TCS3200 sensor@n
#define \b TSC_PIN_OUT Port pin for OUT wire TCS320@n
#define \b TSC_PIN_LED Port pin for LED wire TCS320@n
#define \b TSC_PIN_S0 Port pin for S0 wire TCS3200@n
#define \b TSC_PIN_S1 Port pin for S1 wire TCS3200@n
#define \b TSC_PIN_S2 Port pin for S2 wire TCS3200@n
#define \b TSC_PIN_S3 Port pin for S3 wire TCS3200@n
*/
void tscInit(void);

/**
To be periodically issued in the main loop.
*/
void tscLoop(void);

/**
Starts measure process. When measure is finished callback registered in #tscRegisterCallbackMeasureFinished.
Use #tscGetColor to get raw color measure result from sensor.
*/
void tscStartMeasure(void);

/**
When measure is finished, this function returns measured value in the RAW RGB format.
@result RAW RGB measure result.
*/
RgbColor16_t tscGetColor(void);

/**
Register and enable Color Sensor callback issued when measure is ready after calling #tscStartMeasure function.
@param callback Pointer to the callback function to be issued when measure is finished.
@param userData User data void pointer to structure to be delivered to callback "as-is".
*/
void tscRegisterCallbackMeasureFinished(TscCallback *callback, void *userData);

/**
Unregister Color Sensor measure finished callback, so it's not called anymore.
*/
void tscUnregisterCallbackMeasureFinished(void);

#endif /* SENSOR_H_ */