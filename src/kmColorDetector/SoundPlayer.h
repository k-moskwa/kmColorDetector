/** @file
 * @brief Support for DFRobot Mini Player based on serial protocol
 * SoundPlayer.h
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
 * -# https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
 *
 */ 

#ifndef SOUNDPLAYER_H_
#define SOUNDPLAYER_H_

#include "common.h"
#include <stdint.h>
#include <stdbool.h>

///
#define SNDPL_VOLUME_MIN			0
///
#define SNDPL_VOLUME_MAX			30

///
#define SNDPL_TRACK_NUMBER_MIN		0
///
#define SNDPL_TRACK_NUMBER_MAX		2999

///
#define SNDPL_EQ_NORMAL				0x00
///
#define SNDPL_EQ_POP				0x01
///
#define SNDPL_EQ_ROCK				0x02
///
#define SNDPL_EQ_JAZZ				0x03
///
#define SNDPL_EQ_CLASSIC			0x04
///
#define SNDPL_EQ_BASE				0x05

///
#define SNDPL_PLAYBACK_REPEAT		 0x00
///
#define SNDPL_PLAYBACK_REPEAT_FOLDER 0x01
///
#define SNDPL_PLAYBACK_REPEAT_SINGLE 0x02
///
#define SNDPL_PLAYBACK_RANDOM		 0x03

///
#define SNDPL_PLAYBACK_SRC_UD		0x00
///
#define SNDPL_PLAYBACK_SRC_TF		0x01
///
#define SNDPL_PLAYBACK_SRC_AUX		0x02
///
#define SNDPL_PLAYBACK_SRC_SLEEP	0x03
///
#define SNDPL_PLAYBACK_SRC_FLASH	0x04

/**
*/
void sndInit(void);

/**
*/
void sndLoop(void);

/**
@param ackActive
*/
void sndSetAckRequest(bool ackActive);

/**
@param trackNumber
*/
void sndSetTrack(uint16_t trackNumber);

/**
*/
void sndSetTrackFirst(void);

/**
*/
void sndNext(void);

/**
*/
void sndPrevious(void);

/**
@param volume
*/
void sndSetVolume(uint16_t volume);

/**
@param equalizer
*/
void sndSetEqualizer(uint16_t equalizer);

/**
@param device
*/
void sndSetDevice(uint16_t device);

/**
*/
void sndSleep(void);

/**
*/
void sndResetModule(void);

/**
*/
void sndPlay(void);

/**
*/
void sndPause(void);

/**
*/
void sndStop(void);

/**
@param track
*/
void sndPlayMp3(uint16_t track);

/**
*/
void sndQueryStatus(void);

/**
*/
void sndQueryVolume(void);

/**
*/
void sndQueryTracksNumberTF(void);

/**
*/
void sndQueryTracksNumberUD(void);

/**
*/
void sndQueryTracksNumberFlash(void);

/**
*/
void sndQueryTracksCurrentTF(void);

/**
*/
void sndQueryTracksCurrentUD(void);

/**
*/
void sndQueryTracksCurrentFlash(void);

#endif /* SOUNDPLAYER_H_ */