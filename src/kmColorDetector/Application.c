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
void callbackDebugLed(void *userData, SwtValueType *newTimerValue);
void callbackButton(void *userData, SwtValueType *newTimerValue);
void callbackSensorMeasureReady(void *userData);

// Implementation
void appInit(void) {
	// Initialize all available ports as pull-up inputs
	dbPullUpAllPorts();
#ifndef KMCD_NO_DEBUG
	// In case basic debug enabled - initialize it
	dbInit();
#endif
	// Initialize software callbacks with one of default resolution interval
	swtInit(SWT_INTERVAL_10MS);
	// Initialize TCS3200 Color Sensor
	tscInit();
	// Initialize button routines with specific PIN as defined in config.h
	btnInit(BUTTON_PIN);
#ifndef KMCD_NO_SERIAL_DEBUG
	// In case serial debug option is enabled - 
	// Initialize debug via serial with specific baud rate,
	// clear screen and print initial string.
	serInit(PLYR_SERIAL_BAUD_RATE);
	serTermClearScreen();
	serTermCursorHome();
	serPrintLnString_P(KMCD_INIT_STR);
	serPrintString_P(KMCD_INIT_VERSION);
	serPrintLnString(APP_VERSION);
#else
#ifndef KMCD_NO_DF_PLAYER
	// In case sound module is enabled and serial debug disabled - 
	// Initialize sound module
	sndInit();
#endif
#endif
#ifndef KMCD_NO_LCD
	// In case LCD debug tools are enabled
	// Initialize LCD for debug output
	lcdInit(LCD_COLS, LCD_ROWS, LCD_5x8DOTS);
	lcdBegin();
	// Enable LCD back light
	lcdBacklight();
	// Set cursor the the left top corner
	lcdSetCursor(0, 0);
	// and write application name and version
	lcdPrint(APP_NAME " " APP_VERSION);
#endif
	// Initialize settings
	settingsInit();
	// Get current black reference level of sensor from settings and set it in color tools 
	colorSetBlackReference(settingsGetBlackReference());
	// Get current white reference level of sensor from settings and set it in color tools
	colorSetWhiteReference(settingsGetWhiteReference());
	// Get available color models from settings and set them in colorTools for #colorFindNearest function
	colorSetModels(settingsGetColorModels(), settingsGetAvailableColorModels());

#ifndef KMCD_NO_DEBUG
	// In case basic debug is enabled
	// Register callback routine for debug LED to blink every 0.5s,
	swtRegisterCallback(SWT_TIMER_0, SWT_USER_DATA(DEBUG_MAIN_PIN), callbackDebugLed);
	// and start the software timer
	swtStart(SWT_TIMER_0, DEBUG_BLINK_INTERVAL);
#endif
	// Register callback routine for checking if button has been pressed,
	swtRegisterCallback(SWT_TIMER_1, NULL, callbackButton);
	// and start it with period of 20ms
	swtStart(SWT_TIMER_1, BUTTON_CHECK_INTERVAL);
	// Register callback in the Color Sensor to be called when measure is finished.
	tscRegisterCallbackMeasureFinished(callbackSensorMeasureReady, NULL);
	// Enable interrupts.
	sei();
}

void appLoop(void) {
	// Loop Software Interrupts
	swtLoop();
	// Loop Color Sensor
	tscLoop();
#ifndef KMCD_NO_DF_PLAYER
	// Loop Sound Player
	sndLoop();
#endif
	// Button is looped in one of Software Interrupts
	// Check if button has been pressed since last time
#ifdef KMCD_NO_SERIAL_DEBUG
	if (true == btnPressed()) {
#else
	if (true == btnPressed() || ' ' == serRead()) {
#endif
		// Reset button state
		btnReset();
#ifndef KMCD_NO_DEBUG
		// In case basic LED debug enabled - toggle button LED
		dbToggle(DEBUG_BUTTON_PIN);
#endif
#ifndef KMCD_NO_LCD
		// In case LCD debug enabled - Show information about measure start
		lcdSetCursor(0, 1);
		lcdPrint_P(KMCD_MEASURE_START);
#endif
#ifndef KMCD_NO_SERIAL_DEBUG
		// Send information about measure start to serial debug - if enabled
		serPrintLnString_P(KMCD_MEASURE_START);
#endif
		// Start measure routine for Color Sensor
		tscStartMeasure();
	}
}

// Callbacks
void callbackDebugLed(void *userData, SwtValueType *newTimerValue)  {
	// Get led number from user data
	uint16_t ledNo = (uint16_t)userData;
	// Toggle led with debug functionalities
	dbToggle((uint8_t)ledNo);
	// Set new timer interval, so callback is issued for the next time
	*newTimerValue = DEBUG_BLINK_INTERVAL;
}

void callbackButton(void *userData, SwtValueType *newTimerValue) {
	// Process button loop
	btnLoop();

	if (btnPressed() == false) {
		// this if allows to press the button only once
		// restarts timer once measure is ready in callbackMeasureReady
		*newTimerValue = BUTTON_CHECK_INTERVAL;
	}
}

void callbackSensorMeasureReady(void *userData) {
#ifndef KMCD_NO_DF_PLAYER
	// Get color from Color Sensor after measure is finished,
	// Then normalize it, and find nearest matching color using Color Tools.
	uint8_t colorNumber = colorFindNearest(colorNormalize(tscGetColor()));
	// Set the track number as color + 1 since tracks start from number 1
	// in the DFRobot Mini Player
	sndSetTrack(colorNumber + 1);
#else
#ifndef KMCD_NO_SERIAL_DEBUG
	// Send measure to serial in case serial is enabled
	dbMeasureToSerial();
#endif
#endif
#ifndef KMCD_NO_LCD
	// Send measure to LCD in case LCD is enabled
	dbMeasureToLCD();
#endif
	// Re-enable possibility to start measure with button again
	swtStart(SWT_TIMER_1, BUTTON_CHECK_INTERVAL);
}
