/*
 * Debug.c
 *
 *  Created on: Jul 10, 2019
 *      Author: Krzysztof Moskwa
 *      License: GPL-3.0-or-later
 *
 *  Signal Generator based on AVR ATmega8, AD9833/AD9837 module and LCD display
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

#include <avr/io.h>

#include "Debug.h"
#include "ColorTools.h"
#include "Serial.h"
#include "Sensor.h"

#ifndef KMCD_NO_LCD
#include "LiquidCrystal.h"
#endif

void dbPullUpAllPorts(void) {
    // PULL UP all ports
#ifdef PORTA
    PORTA = 0xFF;
#endif
#ifdef PORTB
    PORTB = 0xFF;
#endif
#ifdef PORTC
    PORTC = 0xFF;
#endif
#ifdef PORTD
    PORTD = 0xFF;
#endif
}

void dbInit(void) {
#ifndef KMCD_NO_DEBUG
#ifdef DEBUG_PIN_0
    DEBUG_DDR |= _BV(DEBUG_PIN_0);
#endif
#ifdef DEBUG_PIN_1
    DEBUG_DDR |= _BV(DEBUG_PIN_1);
#endif
#ifdef DEBUG_PIN_2
    DEBUG_DDR |= _BV(DEBUG_PIN_2);
#endif
#ifdef DEBUG_PIN_3
    DEBUG_DDR |= _BV(DEBUG_PIN_3);
#endif
#ifdef DEBUG_PIN_4
    DEBUG_DDR |= _BV(DEBUG_PIN_4);
#endif
#ifdef DEBUG_PIN_5
    DEBUG_DDR |= _BV(DEBUG_PIN_5);
#endif
#ifdef DEBUG_PIN_6
    DEBUG_DDR |= _BV(DEBUG_PIN_6);
#endif
#ifdef DEBUG_PIN_7
    DEBUG_DDR |= _BV(DEBUG_PIN_7);
#endif
#endif
}

void dbToggle(uint8_t pin) {
#ifndef KMCD_NO_DEBUG
    switch(pin) {
#ifdef DEBUG_PIN_0
        case 0: {
            DEBUG_PORT ^= _BV(DEBUG_PIN_0);
            break;
        }
#endif
#ifdef DEBUG_PIN_1
        case 1: {
            DEBUG_PORT ^= _BV(DEBUG_PIN_1);
            break;
        }
#endif
#ifdef DEBUG_PIN_2
        case 2: {
            DEBUG_PORT ^= _BV(DEBUG_PIN_2);
            break;
        }
#endif
#ifdef DEBUG_PIN_3
        case 3: {
            DEBUG_PORT ^= _BV(DEBUG_PIN_3);
            break;
        }
#endif
#ifdef DEBUG_PIN_4
        case 4: {
            DEBUG_PORT ^= _BV(DEBUG_PIN_4);
            break;
        }
#endif
#ifdef DEBUG_PIN_5
        case 5: {
            DEBUG_PORT ^= _BV(DEBUG_PIN_5);
            break;
        }
#endif
#ifdef DEBUG_PIN_6
        case 6: {
            DEBUG_PORT ^= _BV(DEBUG_PIN_6);
            break;
        }
#endif
#ifdef DEBUG_PIN_7
        case 7: {
            DEBUG_PORT ^= _BV(DEBUG_PIN_7);
            break;
        }
#endif
    }
#endif
}

void dbMeasureToSerial(void)  {
#ifndef KMCD_NO_SERIAL_DEBUG
    RgbColor16_t colorOrg = tscGetColor();
    RgbColor8_t colorNorm = colorNormalize(colorOrg);
    HsvColor8_t colorHsv = colorRgbToHsv(colorNorm);
    uint8_t colorNumber = colorFindNearest(colorNorm);
    char tmpBuffer[40];
    sprintf(tmpBuffer, "colorOrg R:%X, G:%X, B:%X; ", colorOrg.r, colorOrg.g, colorOrg.b);
    serPrintString(tmpBuffer);
    sprintf(tmpBuffer, "colorNorm R:%X, G:%X, B:%X; ", colorNorm.r, colorNorm.g, colorNorm.b);
    serPrintString(tmpBuffer);
    sprintf(tmpBuffer, "colorHSV H:%X, S:%X, V:%X; ", colorHsv.s, colorHsv.h, colorHsv.v);
    serPrintString(tmpBuffer);

    switch (colorNumber) {
        case 0 : {
            serPrintLnString_P(KMCD_COLOR_WHITE);
            break;
        }
        case 1 : {
            serPrintLnString_P(KMCD_COLOR_BLACK);
            break;
        }
        case 2 : {
            serPrintLnString_P(KMCD_COLOR_BLUE);
            break;
        }
        case 3 : {
            serPrintLnString_P(KMCD_COLOR_GREEN);
            break;
        }
        case 4 : {
            serPrintLnString_P(KMCD_COLOR_RED);
            break;
        }
        case 5 : {
            serPrintLnString_P(KMCD_COLOR_YELLOW);
            break;
        }
        case 6 : {
            serPrintLnString_P(KMCD_COLOR_BROWN);
            break;
        }
        case 7 : {
            serPrintLnString_P(KMCD_COLOR_ORANGE);
            break;
        }
    }
#endif
}

void dbMeasureToLCD(void)  {
#ifndef KMCD_NO_LCD
    RgbColor16_t colorOrg = tscGetColor();
    RgbColor8_t colorNorm = colorNormalize(colorOrg);
    uint8_t colorNumber = colorFindNearest(colorNorm);
    char tmpBuffer[40];
    sprintf(tmpBuffer, "R:%X, G:%X, B:%X; ", colorNorm.r, colorNorm.g, colorNorm.b);
    serPrintString(tmpBuffer);

    lcdSetCursor(0, 0);
    lcdPrint(tmpBuffer);
    lcdSetCursor(0, 1);
    lcdPrint_P(PSTR("C:"));
    lcdWrite('0' + colorNumber);
    lcdWrite(' ');
    switch (colorNumber) {
        case 0 : {
            lcdPrint_P(KMCD_COLOR_WHITE);
            break;
        }
        case 1 : {
            lcdPrint_P(KMCD_COLOR_BLACK);
            break;
        }
        case 2 : {
            lcdPrint_P(KMCD_COLOR_BLUE);
            break;
        }
        case 3 : {
            lcdPrint_P(KMCD_COLOR_GREEN);
            break;
        }
        case 4 : {
            lcdPrint_P(KMCD_COLOR_RED);
            break;
        }
        case 5 : {
            lcdPrint_P(KMCD_COLOR_YELLOW);
            break;
        }
        case 6 : {
            lcdPrint_P(KMCD_COLOR_BROWN);
            break;
        }
        case 7 : {
            lcdPrint_P(KMCD_COLOR_ORANGE);
            break;
        }
    }
    lcdFillSpacesToEndOfTheLine();
#endif
}