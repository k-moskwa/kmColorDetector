/** @file
 * @brief Color tool auxiliary routines.
 * ColorTools.h
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
 * -# https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
 */ 


#ifndef COLORTOOLS_H_
#define COLORTOOLS_H_

#include "common.h"

#include <stdlib.h>

/**
*/
typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} RgbColor8_t;

/**
*/
typedef struct {
	uint8_t h;
	uint8_t s;
	uint8_t v;
} HsvColor8_t;

/**
*/
typedef struct {
	uint16_t r;
	uint16_t g;
	uint16_t b;
} RgbColor16_t;

/**
@param blackLevel
*/
void colorSetBlackReference(RgbColor16_t blackLevel);

/**
@param whiteLevel
*/
void colorSetWhiteReference(RgbColor16_t whiteLevel);

/**
@param colorModels
@param colorModelsAvailable
*/
void colorSetModels(RgbColor8_t *colorModels, uint8_t colorModelsAvailable);

/**
@param source
@param sourceBlackLevel
@param sourceWhiteLevel
*/
uint8_t colorNormalizeSingle(uint16_t source, uint16_t sourceBlackLevel, uint16_t sourceWhiteLevel);

/**
@param sourceColor
*/
uint8_t colorFindNearest(RgbColor8_t sourceColor);

/**
@param sourceColor
*/
RgbColor8_t colorNormalize(RgbColor16_t sourceColor);

/**
@param hsv
*/
RgbColor8_t colorHsvToRgb(HsvColor8_t hsv);

/**
@param rgb
*/
HsvColor8_t colorRgbToHsv(RgbColor8_t rgb);

#endif /* COLORTOOLS_H_ */