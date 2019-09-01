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
 * 
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "common.h"
#include "ColorTools.h"

/**
*/
#define TSC_USER_DATA(X) (void *)(X)

/**
*/
typedef void TscCallback(void *);

/**
*/
void tscInit(void);

/**
*/
void tscLoop(void);

/**
*/
void tscStartMeasure(void);

/**
*/
RgbColor16_t tscGetColor(void);

/**
@param callback
@param userData
*/
void tscRegisterCallbackMeasureFinished(TscCallback *callback, void *userData);


#endif /* SENSOR_H_ */