/*
 * Application.c
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

#include "Application.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "Sensor.h"
#include "Buttons.h"
#include "SoftwareTimer.h"
#include "ColorTools.h"
#include "Settings.h"
#include "SoundPlayer.h"
#include "Serial.h"

#include "Debug.h"

#ifndef KMCD_NO_LCD
#include <stdio.h>
#include "LiquidCrystal.h"
#endif

// "private" functions
void callbackDebugLed(void *userData, SmtValueType *newTimerValue);
void callbackButton(void *userData, SmtValueType *newTimerValue);
void callbackSensorMeasureReady(void *userData);

void dbMeasureToSerial(void);

// Implementation
void appInit(void) {
	dbPullUpAllPorts();
#ifndef KMCD_NO_DEBUG
	dbInit();
#endif
	swtInit(100); // 10ms
	tscInit();
	btnInit(BUTTON_PIN);
	sndInit();
#ifndef KMCD_NO_SERIAL_DEBUG
	serInit(PLYR_SERIAL_BAUD_RATE);
#else
#ifndef KMCD_NO_DF_PLAYER
	sndInit();
#endif
#endif

#ifndef KMCD_NO_LCD
	lcdInit(LCD_COLS, LCD_ROWS, LCD_5x8DOTS);
	lcdBegin();
	lcdBacklight();
	lcdSetCursor(0, 0);
	lcdPrint(APP_NAME " " APP_VERSION);
#endif
	settingsInit();
	colorSetBlackReference(settingsGetBlackReference());
	colorSetWhiteReference(settingsGetWhiteReference());
	colorSetModels(settingsGetColorModels(), settingsGetAvailableColorModels());

#ifndef KMCD_NO_DEBUG
	swtRegisterCallback(SWT_TIMER_0, SWT_USER_DATA(DEBUG_MAIN_PIN), callbackDebugLed);
	swtStart(SWT_TIMER_0, DEBUG_BLINK_INTERVAL);
#endif
	swtRegisterCallback(SWT_TIMER_1, NULL, callbackButton);
	swtStart(SWT_TIMER_1, BUTTON_CHECK_INTERVAL);
	tscRegisterCallbackMeasureFinished(callbackSensorMeasureReady, NULL);
	sei();
}

void appLoop(void) {
	swtLoop();
	tscLoop();
	sndLoop();
	if (true == btnPressed()) {
		btnReset();
#ifndef KMCD_NO_DEBUG
		dbToggle(DEBUG_BUTTON_PIN);
#endif
#ifndef KMCD_NO_LCD
		lcdSetCursor(0, 1);
		lcdPrint("Measure Start");
		swtStart(SWT_TIMER_1, BUTTON_CHECK_INTERVAL);
#endif
#ifndef KMCD_NO_SERIAL_DEBUG
		serPrintLnString_P(KMCD_MEASURE_START);
#endif
		tscStartMeasure();
	}
}

// Callbacks
void callbackDebugLed(void *userData, SmtValueType *newTimerValue)  {
	uint16_t ledNo = (uint16_t)userData;
	dbToggle((uint8_t)ledNo);
	*newTimerValue = DEBUG_BLINK_INTERVAL;
}

void callbackButton(void *userData, SmtValueType *newTimerValue) {
	btnLoop();
	if (btnPressed() == false) {
		// this if allows to press the button only once
		// restarts timer once measure is ready in callbackMeasureReady
	}
	*newTimerValue = BUTTON_CHECK_INTERVAL;
}

void callbackSensorMeasureReady(void *userData) {
#ifndef KMCD_NO_DF_PLAYER
	uint8_t colorNumber = colorFindNearest(colorNormalize(tscGetColor()));
	sndSetTrack(colorNumber);
#endif
#ifndef KMCD_NO_SERIAL_DEBUG
	dbMeasureToSerial();
#endif
#ifndef KMCD_NO_LCD
	dbMeasureToLCD();
#endif
}
