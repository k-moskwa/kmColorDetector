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

/// Normalization result for black referent level
#define COLOR_NORMAL_RESULT_BLACK_LEVEL 0x10
/// Normalization result for defined white reference level
#define COLOR_NORMAL_RESULT_WHITE_LEVEL 0xF0

/**
Definition of structure for storing RGB color in 8 bit unsigned integers.
*/
typedef struct {
	/// Red component.
	uint8_t r;
	/// Green component.
	uint8_t g;
	/// Blue component.
	uint8_t b;
} RgbColor8_t;

/**
Definition of structure for storing RGB color in 16 bit unsigned integers.
*/
typedef struct {
	/// Red component.
	uint16_t r;
	/// Green component.
	uint16_t g;
	/// Blue component.
	uint16_t b;
} RgbColor16_t;

/**
Definition of structure for storing color in HSV model in 8 bit unsigned integers.
*/
typedef struct {
	/// Hue component. 0x00 means 0 degrees, 0xFF means 360 degrees.
	uint8_t h;
	/// Saturation component. 0x00 means no saturation, 0xFF means maximum saturation.
	uint8_t s;
	/// Value component. 0x00 means lowest luminance, 0xFF means highest luminance.
	uint8_t v;
} HsvColor8_t;

/**
Setts the black reference level for calculation of the normalized color value with #colorNormalize function.
@param blackLevel Black reference level defined as 16 bit integer.
*/
void colorSetBlackReference(RgbColor16_t blackLevel);

/**
Setts the white reference level for calculation of the normalized color value, with #colorNormalize function.
@param whiteLevel White reference level defined as 16 bit integer.
*/
void colorSetWhiteReference(RgbColor16_t whiteLevel);

/**
Returns normalized 8 bit RGB color based on provided 16bit RAW RGB 
value and defined black and white reference colors.
Following preprocessor values are used for calculations@n
#define \b #COLOR_NORMAL_RESULT_BLACK_LEVEL as normalization result for black referent level@n
#define \b #COLOR_NORMAL_RESULT_WHITE_LEVEL as normalization result for white referent level@n
@param sourceColor Source 16bit integer RGB color from the sensor that needs to be normalized
@result normalized Normalized color returned as 8bit integer RGB 
*/
RgbColor8_t colorNormalize(RgbColor16_t sourceColor);


/**
Defines array of color models to be used in #colorFindNearest function.
Function does not check the integrity or array size so the array needs to have 
at least #colorModelsAvailable number of elements. @n
\b NOTE!!! Only the reference of the array is stored in the internal structures 
for saving memory, so these values should not be altered after setting it here.
@param colorModels Array of RGB color models with number of elements at least equal colorModelsAvailable.
@param colorModelsAvailable Number of colors in the colorModels array.
*/
void colorSetModels(const RgbColor8_t *colorModels, uint8_t colorModelsAvailable);

/**
Finds nearest color from the color array defined by #colorSetModels functions.
Function uses mean square error for finding nearest color.
@param sourceColor normalized source color.
@result number of color in the color models table that is nearest to provided sourceColor.
*/
uint8_t colorFindNearest(RgbColor8_t sourceColor);

/**
Converts color from 8-bit HSV color model to 8-bit RGB color model 
@param hsv source color in 8-bit HSV color model
@result result color in 8-bit RGB color model
*/
RgbColor8_t colorHsvToRgb(HsvColor8_t hsv);

/**
Converts color from 8-bit RGB color model to 8-bit HSV color model
@param hsv source color in 8-bit RGB color model
@result result color in 8-bit HSV color model
*/
HsvColor8_t colorRgbToHsv(RgbColor8_t rgb);

#endif /* COLORTOOLS_H_ */