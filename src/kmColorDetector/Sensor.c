/*
 * Sensor.c
 *
 *  Created on: Jul 10, 2019
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
 *
 */

#include <stdbool.h>
#include <avr/io.h>

#include "Sensor.h"
#include "ExternalInterrupt.h"
#include "TimerOne.h"
#include "ColorTools.h"

#define TSC_USER_DATA(X) (void *)(X)
typedef void TscCallback(void *);

#define TSC_DEFULT_FREQUENCY_SCALING TSC_PERCENT_20
#define SINGLE_MEASURE_TIME 200000 // 10ms

static RgbColor16_t _tscRGB;

static uint16_t _tscCount = 0;
static bool _tscMeasureReady = false;

static TscCallback *_tscCallback = NULL;
static void *_tscCallbackUserData = NULL;

typedef enum {
	TSC_POWER_DOWN,
	TSC_PERCENT_2,
	TSC_PERCENT_20,
	TSC_PERCENT_100
} TscOutputFrequencyScaling;

typedef enum {
	TSC_PDT_STOP = 0,
	TSC_PDT_RED = 1,
	TSC_PDT_GREEN = 2,
	TSC_PDT_BLUE = 3,
	TSC_PDT_CLEAR = 4
} TscPhotodiodeType;

// "private" functions
void tscSetOutputFrequencyScaling(TscOutputFrequencyScaling frequencyScaling);
void tscSetPhotodiodeType(TscPhotodiodeType photodiodeType);
void tscCountCallback(void *userData);
void tscTimerCallback(void *userData);

void tscSetOutputFrequencyScaling(TscOutputFrequencyScaling frequencyScaling) {
	switch (frequencyScaling) {
		case TSC_POWER_DOWN : {
			TSC_PORT &= ~_BV(TSC_PIN_S0);
			TSC_PORT &= ~_BV(TSC_PIN_S1);
			break;
		}
		case TSC_PERCENT_2 : {
			TSC_PORT &= ~_BV(TSC_PIN_S0);
			TSC_PORT |= _BV(TSC_PIN_S1);
			break;
		}
		case TSC_PERCENT_20 : {
			TSC_PORT |= _BV(TSC_PIN_S0);
			TSC_PORT &= ~_BV(TSC_PIN_S1);
			break;
		}
		case TSC_PERCENT_100 : {
			TSC_PORT |= _BV(TSC_PIN_S0);
			TSC_PORT |= _BV(TSC_PIN_S1);
			break;
		}
	}
}

void tscSetPhotodiodeType(TscPhotodiodeType photodiodeType) {
	switch (photodiodeType) {
		case TSC_PDT_CLEAR : {
			TSC_PORT |= _BV(TSC_PIN_S2);
			TSC_PORT &= ~_BV(TSC_PIN_S3);
			break;
		}
		case TSC_PDT_RED : {
			TSC_PORT &= ~_BV(TSC_PIN_S2);
			TSC_PORT &= ~_BV(TSC_PIN_S3);
			break;
		}
		case TSC_PDT_GREEN : {
			TSC_PORT |= _BV(TSC_PIN_S2);
			TSC_PORT |= _BV(TSC_PIN_S3);
			break;
		}
		case TSC_PDT_BLUE : {
			TSC_PORT &= ~_BV(TSC_PIN_S2);
			TSC_PORT |= _BV(TSC_PIN_S3);
			break;
		}
		default : {
			// intentionally
		}
	}
}

void tscCountCallback(void *userData) {
	_tscCount++;
}

void tscTimerCallback(void *userData) {
	uint16_t filter = (uint16_t)userData;
	TscPhotodiodeType photodiodeType = (TscPhotodiodeType)filter;
	if (TSC_PDT_STOP == photodiodeType) {
		return;
	}
	tscSetPhotodiodeType(photodiodeType);
	switch(photodiodeType) {
		case TSC_PDT_RED : {
			TSC_PORT |= _BV(TSC_PIN_LED);
			break;
		}
		case TSC_PDT_GREEN : {
			_tscRGB.r = _tscCount;
			break;
		}
		case TSC_PDT_BLUE : {
			_tscRGB.g = _tscCount;
			break;
		}
		case TSC_PDT_CLEAR : {
			_tscRGB.b = _tscCount;
			_tscMeasureReady = true;
			TSC_PORT &= ~_BV(TSC_PIN_LED);
			timer1SetCallbackUserData(TSC_PDT_STOP);
			break;
		}
		default : {
			// intentionally
		}
	}
	if (TSC_PDT_CLEAR != photodiodeType) {
		timer1SetCallbackUserData(TIMER1_USER_DATA(filter + 1));
	}
	_tscCount = 0;
}

void tscLoop(void) {
	if (NULL != _tscCallback && true == _tscMeasureReady) {
		_tscMeasureReady = false;
		_tscCallback(_tscCallbackUserData);
		timer1Stop();
		timer1Restart();
		tscSetOutputFrequencyScaling(TSC_POWER_DOWN);
	}
}

void tscInit(void) {
	// Pins S0 to S4 as outputs
	TSC_DDR |= _BV(TSC_PIN_S0) | _BV(TSC_PIN_S1) | _BV(TSC_PIN_S2) | _BV(TSC_PIN_S3);
	// Led pin as output
	TSC_DDR |= _BV(TSC_PIN_LED);
	// Led pin off
	TSC_PORT &= ~_BV(TSC_PIN_LED);

	// Sensor out pin as uC input
	TSC_DDR &= ~_BV(TSC_PIN_OUT);
	// No pull-up for Sensor out pin
	TSC_PORT &= ~_BV(TSC_PIN_OUT);

	timer1Init(SINGLE_MEASURE_TIME);
	extIntRegisterCallback(EXT_INT_0, EXT_INT_RISING_EDGE, false, tscCountCallback, NULL);
	timer1RegisterCallback(tscTimerCallback, TIMER1_USER_DATA(TSC_PDT_STOP));
	tscSetOutputFrequencyScaling(TSC_POWER_DOWN);
}

void tscStartMeasure(void) {
	timer1SetCallbackUserData(TIMER1_USER_DATA(TSC_PDT_RED));
	tscSetOutputFrequencyScaling(TSC_DEFULT_FREQUENCY_SCALING);
	timer1EnableInterrupt();
	timer1Restart();
	timer1Start();
	_tscCount  = 0;
}

RgbColor16_t tscGetColor(void) {
	return _tscRGB;
}

void tscRegisterCallbackMeasureFinished(TscCallback *callback, void *userData) {
	_tscCallbackUserData = userData;
	_tscCallback = callback;
}

void tscUnregisterCallbackMeasureFinished(void) {
	_tscCallbackUserData = NULL;
	_tscCallback = NULL;
}

