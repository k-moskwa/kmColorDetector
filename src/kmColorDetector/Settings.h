/** @file
 * @brief Functions allowing to load and store presets in EEPROM.
 * Settings.h
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
 * References:
 * -# https://www.microchip.com/webdoc/AVRLibcReferenceManual/group__avr__eeprom.html
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "common.h"

#include <stdint.h>

#include "ColorTools.h"
/**
Initializes routines and variable responsible for button operations.
Following definitions to be set in config.h file @n
#define \b KMSG_MAGIC string that defines module and version allowing to preserve and update settings between versions (e.g. "KMSG100")@n
#define \b KMSG_MAGIC_LENGTH number of bytes in magic string defined in KMSG_MAGIC (e.g. 8 in above example last byte is '\0' so it's 8 bytes)@n
NOTE: To preserve EEPROM settings make sure EESAVE fuse bit is correctly defined (EESAVE = 0)@n
In this version once EEPROM is erased or has different magic value than defined in KMSG_MAGIC then values are overwritten with default presets 1 to 4
*/
void settingsInit(void);

/**
Returns preset of presetNumber to waveType and frequency result variables.
NOTE: in case KMSG_NO_EEPROM is defined then values are not taken from EEPROM but just from predefined default presets
@param colorNumber number of preset to be returned (0 to KMCD_MAX_PRESETS)
@result RGB color model of specific number
*/
RgbColor8_t settingsGetColorModel(uint8_t colorNumber);

/**
@result
*/
RgbColor8_t *settingsGetColorModels(void);

/**
@param colorNumber
@param colorModel
*/
void settingsSetColorModel(uint8_t colorNumber, HsvColor8_t colorModel);

/**
@result
*/
RgbColor16_t settingsGetBlackReference(void);

/**
@result
*/
RgbColor16_t settingsGetWhiteReference(void);

/**
@result
*/
uint8_t settingsGetAvailableColorModels(void);

/**
*/
void settingsStore(void);

#endif /* SETTINGS_H_ */