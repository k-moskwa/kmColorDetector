/*
 * Settings.c
 *
 *  Created on: Jul 12, 2019
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
 */

#ifndef _TESTS_ENV
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#endif
#include <string.h>
#include <stdbool.h>

#include "config.h"
#include "Settings.h"
#include "ColorTools.h"

typedef struct {
    char magic[8];
    RgbColor16_t blackReference;
    RgbColor16_t whiteReference;
    uint8_t availableColors;
    RgbColor8_t colorModels[KMCD_MAX_COLOR_MODELS];
} SettingsStruct;

typedef struct {
    const char magic[8];
    const RgbColor16_t blackReference;
    const RgbColor16_t whiteReference;
    const uint8_t availableColors;
    const RgbColor8_t colorModels[KMCD_MAX_COLOR_MODELS];
} SettingsStruct_C;

static SettingsStruct _RAMsettings;
#ifndef KMCD_NO_EEPROM
static SettingsStruct _EEPROMsettings EEMEM;
#endif

#ifndef KMCD_NO_EEPROM
static const SettingsStruct _PROGMEMsettings PROGMEM =  = {
      .availableColors = 6
    , .magic = KMCD_MAGIC
    , .blackReference = (RgbColor16_t){.r = 0x00D4, .g = 0x00B8, .b = 0x00D2}
    , .whiteReference = (RgbColor16_t){.r = 0x057B, .g = 0x056E, .b = 0x0693}
    , .colorModels =
    {
    // white
      (RgbColor8_t){.r = 0xFF, .g = 0xFF, .b = 0xFF}
    // black
    , (RgbColor8_t){.r = 0x00, .g = 0x00, .b = 0x00}
    // blue
    , (RgbColor8_t){.r = 0x40, .g = 0x60, .b = 0xA0}
    // green
    , (RgbColor8_t){.r = 0x20, .g = 0x60, .b = 0x50}
    // red
    , (RgbColor8_t){.r = 0xFF, .g = 0x00, .b = 0x00}
    // yellow
    , (RgbColor8_t){.r = 0xFF, .g = 0xFF, .b = 0x00}
    //};
};
#endif

void settingsInit(void) {
#ifndef KMCD_NO_EEPROM
    eeprom_read_block(&_RAMsettings, &_EEPROMsettings, sizeof(_EEPROMsettings));
    if (strncmp(_RAMsettings.magic, KMCD_MAGIC, KMCD_MAGIC_LENGTH) != 0) {
        memcpy_P(&_RAMsettings, &_PROGMEMsettings, sizeof(_RAMsettings));
        eeprom_write_block(&_RAMsettings, &_EEPROMsettings, sizeof(_EEPROMsettings));
    }
#else
    _RAMsettings.availableColors = KMCD_MAX_COLOR_MODELS;
    _RAMsettings.blackReference = (RgbColor16_t){.r = 0x00D4, .g = 0x00B8, .b = 0x00D2};
    _RAMsettings.whiteReference = (RgbColor16_t){.r = 0x057B, .g = 0x056E, .b = 0x0693};
    _RAMsettings.colorModels[0] = (RgbColor8_t){.r = 0xFF, .g = 0xFF, .b = 0xFF}; // white
    _RAMsettings.colorModels[1] = (RgbColor8_t){.r = 0x00, .g = 0x00, .b = 0x00}; // black
    _RAMsettings.colorModels[2] = (RgbColor8_t){.r = 0x40, .g = 0x60, .b = 0xA0}; // blue
    _RAMsettings.colorModels[3] = (RgbColor8_t){.r = 0x40, .g = 0x90, .b = 0x50}; // green
    _RAMsettings.colorModels[4] = (RgbColor8_t){.r = 0xA0, .g = 0x30, .b = 0x30}; // red
    _RAMsettings.colorModels[5] = (RgbColor8_t){.r = 0xFF, .g = 0xFF, .b = 0x50}; // yellow

    //memcpy_P(&_RAMsettings, &_PROGMEMsettings, sizeof(_RAMsettings));
#endif
}

RgbColor8_t *settingsGetColorModels(void) {
    return _RAMsettings.colorModels;
}

RgbColor8_t settingsGetColorModel(uint8_t colorNumber) {
    return _RAMsettings.colorModels[colorNumber];
}

RgbColor16_t settingsGetBlackReference(void) {
    return _RAMsettings.blackReference;
}

void settingsSetBlackReference(RgbColor16_t blackReference) {
    _RAMsettings.blackReference = blackReference;
}

RgbColor16_t settingsGetWhiteReference(void) {
    return _RAMsettings.whiteReference;
}

void settingsSetWhiteReference(RgbColor16_t whiteReference) {
    _RAMsettings.whiteReference = whiteReference;
}

uint8_t settingsGetAvailableColorModels(void) {
    return _RAMsettings.availableColors;
}

void settingsStore(void) {
#ifndef KMCD_NO_EEPROM
    eeprom_write_block(&_RAMsettings, &_EEPROMsettings, sizeof(_EEPROMsettings));
#endif
}
