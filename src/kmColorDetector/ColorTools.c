/*
 * ColorTools.c
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

#include <stdbool.h>
#include <stdint.h>

#include "ColorTools.h"


// Number of available color models
#define COLOR_NORMAL_RESULT_RANGE COLOR_NORMAL_RESULT_WHITE_LEVEL - COLOR_NORMAL_RESULT_BLACK_LEVEL

static RgbColor16_t _blackLevel;
static RgbColor16_t _whiteLevel;
static const RgbColor8_t *_colorModels;
static uint8_t _colorModelsSizeOf = 0;

// "private" functions
int32_t colorPow2(int32_t value);
uint8_t colorNormalizeSingle(uint16_t source, uint16_t sourceBlackLevel, uint16_t sourceWhiteLevel);

// Implementation
void colorSetBlackReference(RgbColor16_t blackLevel) {
	_blackLevel = blackLevel;
}

void colorSetWhiteReference(RgbColor16_t whiteLevel) {
	_whiteLevel = whiteLevel;
}

void colorSetModels(const RgbColor8_t *colorModels, uint8_t colorModelsAvailable) {
	_colorModels = colorModels;
	_colorModelsSizeOf = colorModelsAvailable;
}

uint8_t colorNormalizeSingle(uint16_t source, uint16_t sourceBlackLevel, uint16_t sourceWhiteLevel) {
	// result = 
	// (source - sourceBlackLevel) * COLOR_NORMAL_RESULT_RANGE
	// ------------------------------------------------------- + COLOR_NORMAL_RESULT_BLACK_LEVEL
	//          (sourceWhiteLevel - sourceBlackLevel);
	// where COLOR_NORMAL_RESULT_RANGE = COLOR_NORMAL_RESULT_WHITE_LEVEL - COLOR_NORMAL_RESULT_BLACK_LEVEL

	int32_t tmp = source;
	tmp -= sourceBlackLevel;
	tmp *= COLOR_NORMAL_RESULT_RANGE;
	tmp /= (sourceWhiteLevel - sourceBlackLevel);
	tmp += COLOR_NORMAL_RESULT_BLACK_LEVEL;
	tmp = tmp < 0x00 ? 0 : tmp;
	tmp = tmp > 0xFF ? 0xFF : tmp;
	return (uint8_t)tmp;
}

int32_t colorPow2(int32_t value) {
	return value * value;
}

uint32_t colorDifferenceErrorRGB(RgbColor8_t sourceColor, RgbColor8_t modelColor) {
	uint32_t result = 0;
	result += (uint32_t)(colorPow2(abs((int16_t)sourceColor.r - (int16_t)modelColor.r)));
	result += (uint32_t)(colorPow2(abs((int32_t)sourceColor.g - (int32_t)modelColor.g)));
	result += (uint32_t)(colorPow2(abs((int32_t)sourceColor.b - (int32_t)modelColor.b)));
	return result;
}

uint8_t colorFindNearest(RgbColor8_t sourceColor) {
	uint32_t minDifference = UINT32_MAX;
	uint32_t colorDifference = 0;
	uint8_t result = 0;
	for (uint8_t i = 0; i < _colorModelsSizeOf; i++) {
		colorDifference = colorDifferenceErrorRGB(sourceColor, _colorModels[i]);
		if (colorDifference < minDifference) {
			minDifference = colorDifference;
			result = i;
		}
	}
	return result;
}

RgbColor8_t colorNormalize(RgbColor16_t sourceColor) {
	RgbColor8_t result;
	result.r = colorNormalizeSingle(sourceColor.r, _blackLevel.r, _whiteLevel.r);
	result.g = colorNormalizeSingle(sourceColor.g, _blackLevel.g, _whiteLevel.g);
	result.b = colorNormalizeSingle(sourceColor.b, _blackLevel.b, _whiteLevel.b);
	return result;
}

RgbColor8_t colorHsvToRgb(HsvColor8_t hsv) {
	RgbColor8_t rgb;
	uint8_t region, p, q, t;
	uint16_t h, s, v, remainder;

	if (hsv.s == 0) {
		rgb.r = hsv.v;
		rgb.g = hsv.v;
		rgb.b = hsv.v;
		return rgb;
	}

	// converting to 16 bit to prevent overflow
	h = hsv.h;
	s = hsv.s;
	v = hsv.v;

	region = h / 43;
	remainder = (h - (region * 43)) * 6;

	p = (v * (255 - s)) >> 8;
	q = (v * (255 - ((s * remainder) >> 8))) >> 8;
	t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

	switch (region) {
		case 0: {
			rgb.r = v;
			rgb.g = t;
			rgb.b = p;
			break;
		}
		case 1: {
			rgb.r = q;
			rgb.g = v;
			rgb.b = p;
			break;
		}
		case 2: {
			rgb.r = p;
			rgb.g = v;
			rgb.b = t;
			break;
		}
		case 3: {
			rgb.r = p;
			rgb.g = q;
			rgb.b = v;
			break;
		}
		case 4: {
			rgb.r = t;
			rgb.g = p;
			rgb.b = v;
			break;
		}
		default: {
			rgb.r = v;
			rgb.g = p;
			rgb.b = q;
			break;
		}
	}
	return rgb;
}

HsvColor8_t colorRgbToHsv(RgbColor8_t rgb) {
	HsvColor8_t hsv;
	uint8_t rgbMin, rgbMax;

	rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
	rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

	hsv.v = rgbMax;
	if (hsv.v == 0) {
		hsv.h = 0;
		hsv.s = 0;
		return hsv;
	}

	hsv.s = 255 * ((long)(rgbMax - rgbMin)) / hsv.v;
	if (hsv.s == 0) {
		hsv.h = 0;
		return hsv;
	}

	if (rgbMax == rgb.r) {
		hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
		} else if (rgbMax == rgb.g) {
		hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
		} else {
		hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);
	}

	return hsv;
}