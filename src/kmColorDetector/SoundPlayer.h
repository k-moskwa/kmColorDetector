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

/// Minimal volume of the player for #sndSetVolume function.
#define SNDPL_VOLUME_MIN			0
/// Maximum volume of the player for #sndSetVolume function.
#define SNDPL_VOLUME_MAX			30

/// Minimal track number of the player for #sndSetTrack function.
#define SNDPL_TRACK_NUMBER_MIN		0
/// Maximum track number of the player for #sndSetTrack function.
#define SNDPL_TRACK_NUMBER_MAX		2999

/// Equalizer type normal to be used in #sndSetEqualizer function.
#define SNDPL_EQ_NORMAL				0x00
/// Equalizer type pop to be used in #sndSetEqualizer function.
#define SNDPL_EQ_POP				0x01
/// Equalizer type rock to be used in #sndSetEqualizer function.
#define SNDPL_EQ_ROCK				0x02
/// Equalizer type jazz to be used in #sndSetEqualizer function.
#define SNDPL_EQ_JAZZ				0x03
/// Equalizer type classic to be used in #sndSetEqualizer function.
#define SNDPL_EQ_CLASSIC			0x04
/// Equalizer type base to be used in #sndSetEqualizer function.
#define SNDPL_EQ_BASE				0x05

///
#define SNDPL_PLAYBACK_REPEAT		 0x00
///
#define SNDPL_PLAYBACK_REPEAT_FOLDER 0x01
///
#define SNDPL_PLAYBACK_REPEAT_SINGLE 0x02
///
#define SNDPL_PLAYBACK_RANDOM		 0x03

/// Source device type U-Disk to be used in #sndSetDevice function.
#define SNDPL_PLAYBACK_SRC_UD		0x00
/// Source device type Flash Card to be used in #sndSetDevice function.
#define SNDPL_PLAYBACK_SRC_TF		0x01
/// Source device type AUX to be used in #sndSetDevice function.
#define SNDPL_PLAYBACK_SRC_AUX		0x02
/// Sleep mode of the module that can be used #sndSetDevice function.
#define SNDPL_PLAYBACK_SRC_SLEEP	0x03
/// Source device type Flash to be used in #sndSetDevice function.
#define SNDPL_PLAYBACK_SRC_FLASH	0x04

/**
Initialization of the module.
*/
void sndInit(void);

/**
To be periodically issued in the main loop.
*/
void sndLoop(void);

/**
Enables or disables ACK requests after each command. Disabled by default
@param ackActive If true - the ACK requests are enabled.
*/
void sndSetAckRequest(bool ackActive);

/**
Sets and plays track for WAV files in XX directories, where XX is from 0 to 99
@param trackNumber Number of track.
*/
void sndSetTrack(uint16_t trackNumber);

/**
Sets track to first available and plays it.
*/
void sndSetTrackFirst(void);

/**
Skips to the next track.
*/
void sndNext(void);

/**
Skips to the prevoius track.
*/
void sndPrevious(void);

/**
Sets output volume of the module.
@param volume Output volume.
*/
void sndSetVolume(uint16_t volume);

/**
Sets type of the equalizer of the module.
@param equalizer Equalizer type.
*/
void sndSetEqualizer(uint16_t equalizer);

/**
Sets type of the playback device.
@param device Playback device.
*/
void sndSetDevice(uint16_t device);

/**
Sends module to sleep mode.
*/
void sndSleep(void);

/**
Resets module.
*/
void sndResetModule(void);

/**
Changes state of the player to 'play' mode.
*/
void sndPlay(void);

/**
Changes state of the player to 'pause' mode.
*/
void sndPause(void);

/**
Changes state of the player to 'stop' mode.
*/
void sndStop(void);

/**
Plays mp3 file from 'mp3' directory of specific track number.
@param track Track number of the 'mp3' file.
*/
void sndPlayMp3(uint16_t track);

/**
Asks for status of the module.
*/
void sndQueryStatus(void);

/**
Asks for the current volume of the module.
*/
void sndQueryVolume(void);

/**
Asks for number of tracks available on the flash card.
*/
void sndQueryTracksNumberTF(void);

/**
Asks for number of tracks available on the U-Disk.
*/
void sndQueryTracksNumberUD(void);

/**
Asks for number of tracks available in the flash.
*/
void sndQueryTracksNumberFlash(void);

/**
Asks for current track number played from flash card.
*/
void sndQueryTracksCurrentTF(void);

/**
Asks for current track number played from U-Disk.
*/
void sndQueryTracksCurrentUD(void);

/**
Asks for current track number played from flash.
*/
void sndQueryTracksCurrentFlash(void);

#endif /* SOUNDPLAYER_H_ */