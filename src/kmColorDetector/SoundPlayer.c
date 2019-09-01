/*
 * SoundPlayer.c
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

#include "SoundPlayer.h"
#include "Serial.h"

#define SNDPL_CMD_NEXT				0x01
#define SNDPL_CMD_PREVIOUS			0x02
#define SNDPL_CMD_SPECIFY_TRACK		0x03
#define SNDPL_CMD_VOLUME_INCREASE	0x04
#define SNDPL_CMD_VOLUME_DECREASE	0x05
#define SNDPL_CMD_VOLUME_SET		0x06
#define SNDPL_CMD_QUALIZER_SET		0x07
#define SNDPL_CMD_PLAYBACK_MODE_SET	0x08
#define SNDPL_CMD_PLAYBACK_MODE_SRC	0x09
#define SNDPL_CMD_STANDBY			0x0A
#define SNDPL_CMD_POWERUP			0x0B
#define SNDPL_CMD_RESET				0x0C
#define SNDPL_CMD_PLAY				0x0D
#define SNDPL_CMD_PAUSE				0x0E
#define SNDPL_CMD_FOLDER_SET		0x0F
#define SNDPL_CMD_VOLUME_ADJUST		0x10
#define SNDPL_CMD_PLAY_REPEAT		0x11
#define SNDPL_CMD_PLAY_MP3			0x12
#define SNDPL_CMD_STOP				0x16

#define SNDPL_CMD_STAY1				0x3C
#define SNDPL_CMD_STAY2				0x3D
#define SNDPL_CMD_STAY3				0x3E
#define SNDPL_CMD_INIT				0x3F
#define SNDPL_CMD_RESEND			0x40
#define SNDPL_CMD_REPLY				0x41
#define SNDPL_CMD_QUERY_STATUS		0x42
#define SNDPL_CMD_QUERY_VOLUME		0x43
#define SNDPL_CMD_QUERY_EQUALIZER	0x44
#define SNDPL_CMD_QUERY_PLAYBACK_MODE 0x45
#define SNDPL_CMD_QUERY_VERSION		0x46
#define SNDPL_CMD_QUERY_FILES_NO_TF	0x47
#define SNDPL_CMD_QUERY_FILES_NO_UD	0x48
#define SNDPL_CMD_QUERY_FILES_NO_FLASH 0x49
#define SNDPL_CMD_QUERY_CURRENT_UD	0x4B
#define SNDPL_CMD_QUERY_CURRENT_TF	0x4C
#define SNDPL_CMD_QUERY_CURRENT_FLASH 0x4D
#define SNDPL_CMD_KEEP_ON			0x50
#define SNDPL_CMD_QUERY_TRACK_TF	0x51
#define SNDPL_CMD_QUERY_TRACK_UD	0x52
#define SNDPL_CMD_QUERY_TRACK_FLASH	0x53

#define SNDPL_FRAME_POS_START_CODE	0
#define SNDPL_FRAME_POS_VERSION		1
#define SNDPL_FRAME_POS_LENGTH		2
#define SNDPL_FRAME_POS_COMMAND		3
#define SNDPL_FRAME_POS_ACK			4
#define SNDPL_FRAME_POS_NO_RECEIVE	5
#define SNDPL_FRAME_POS_ARGUMENT	6
#define SNDPL_FRAME_POS_CHECKSUM_HI	7
#define SNDPL_FRAME_POS_CHECKSUM_LO	8
#define SNDPL_FRAME_POS_END_CODE	9
#define SNDPL_FRAME_SIZE_OF			10

#define SNDPL_FRAME_START_CODE		0x7E
#define SNDPL_FRAME_END_CODE		0xEF
#define SNDPL_FRAME_VERSION_CODE	0xFF
#define SNDPL_FRAME_DEFAULT_LENGTH	0x06

static uint8_t _txFrame[SNDPL_FRAME_SIZE_OF] = {
	  SNDPL_FRAME_START_CODE
	, SNDPL_FRAME_VERSION_CODE
	, SNDPL_FRAME_DEFAULT_LENGTH
	, 00, 00, 00, 00
	, 00, 00 // checksum 
	, SNDPL_FRAME_END_CODE
	};

static uint8_t _rxFrame[SNDPL_FRAME_SIZE_OF];
static uint8_t _rxFramePos = 0;
static bool _ackRequestActive = false;

// Frame definition
// 7E FF 06 0F 00 01 01 xx xx EF
// 0	->	7E is start code
// 1	->	FF is version
// 2	->	06 is length
// 3	->	0F is command
// 4	->	00 is no receive
// 5~6	->	01 01 is argument
// 7~8	->	checksum = 0 - (uint8_t)( FF+06+0F+00+01+01 ) 
// 9	->	EF is end code

// "private" functions
uint16_t sndCalcChecksum(const uint8_t *frame);
void sndFillChecksum(void);
void sndFillArgument(uint16_t argument);
void sndSendToSerial(void);
void sndSendCmdWithArgument(uint8_t cmd, uint16_t argument);
void sndSendCmd(uint8_t cmd);

// Implementation
void sndInit(void) {
	serInit(PLYR_SERIAL_BAUD_RATE);
}

void sndLoop(void) {
	if (serAvailable() > 0) {
		uint8_t serialData = serRead();
		_rxFrame[_rxFramePos++] = serialData;
		if (SNDPL_FRAME_END_CODE == serialData) {
			_rxFramePos = 0;
			// TODO: call raw callback
		} else if (SNDPL_FRAME_POS_END_CODE == _rxFramePos) {
			_rxFramePos = 0;
		}
	}
}

void sndSetAckRequest(bool ackActive) {
	_ackRequestActive = ackActive;
	_txFrame[SNDPL_FRAME_POS_ACK] = ackActive == true ? 0x01 : 0x00;
}

uint16_t sndCalcChecksum(const uint8_t *frame) {
	uint16_t result = 0;
	int16_t sum = 0;
	for (uint8_t i = SNDPL_FRAME_POS_VERSION; i <= SNDPL_FRAME_POS_ARGUMENT; i++) {
		sum += frame[i];
	}
	// checksum counted from lowest byte only
	//result &= 0xFF;
	// the checksum = 0 - sum of bytes
	sum = 0 - sum;
	result = (uint16_t)sum;
	return result;
}

void sndFillChecksum(void) {
	uint16_t checksum = sndCalcChecksum(_txFrame);
	_txFrame[SNDPL_FRAME_POS_CHECKSUM_LO] = checksum & 0xFF;
	_txFrame[SNDPL_FRAME_POS_CHECKSUM_HI] = checksum >> 0x08;
}

void sndFillArgument(uint16_t argument) {
	_txFrame[SNDPL_FRAME_POS_NO_RECEIVE] = argument >> 0x08;
	_txFrame[SNDPL_FRAME_POS_ARGUMENT] = argument & 0xFF;
}

void sndSendToSerial(void) {
	serSendBinary(_txFrame, SNDPL_FRAME_SIZE_OF);
}

void sndSendCmdWithArgument(uint8_t cmd, uint16_t argument) {
	_txFrame[SNDPL_FRAME_POS_COMMAND] = cmd;
	sndFillArgument(argument);
	sndFillChecksum();
	sndSendToSerial();
}

void sndSendCmd(uint8_t cmd) {
	sndSendCmdWithArgument(cmd, 0x0000);
}

void sndSetTrack(uint16_t trackNumber) {
	sndSendCmdWithArgument(SNDPL_CMD_SPECIFY_TRACK, trackNumber);
}

void sndSetTrackFirst(void) {
	sndSendCmd(SNDPL_CMD_SPECIFY_TRACK);
}

void sndNext(void) {
	sndSendCmd(SNDPL_CMD_NEXT);
}

void sndPrevious(void) {
	sndSendCmd(SNDPL_CMD_PREVIOUS);
}

void sndSetVolume(uint16_t volume) {
	sndSendCmdWithArgument(SNDPL_CMD_VOLUME_SET, volume);
}

void sndSetEqualizer(uint16_t equalizer) {
	sndSendCmdWithArgument(SNDPL_CMD_QUALIZER_SET, equalizer);
}

void sndSetDevice(uint16_t device) {
	sndSendCmdWithArgument(SNDPL_CMD_PLAYBACK_MODE_SRC, device);
}

void sndSleep(void) {
	sndSendCmd(SNDPL_CMD_STANDBY);
}

void sndResetModule(void) {
	sndSendCmd(SNDPL_CMD_RESET);
}

void sndPlay(void) {
	sndSendCmd(SNDPL_CMD_PLAY);
}

void sndPause(void) {
	sndSendCmd(SNDPL_CMD_PAUSE);
}

void sndStop(void) {
	sndSendCmd(SNDPL_CMD_STOP);
}

void sndPlayMp3(uint16_t track) {
	sndSendCmdWithArgument(SNDPL_CMD_PLAY_MP3, track);
}

void sndQueryStatus(void) {
	sndSendCmd(SNDPL_CMD_QUERY_STATUS);
}

void sndQueryVolume(void) {
	sndSendCmd(SNDPL_CMD_QUERY_VOLUME);
}

void sndQueryTracksNumberTF(void) {
	sndSendCmd(SNDPL_CMD_QUERY_FILES_NO_TF);
}

void sndQueryTracksNumberUD(void) {
	sndSendCmd(SNDPL_CMD_QUERY_FILES_NO_UD);
}

void sndQueryTracksNumberFlash(void) {
	sndSendCmd(SNDPL_CMD_QUERY_FILES_NO_FLASH);
}

void sndQueryTracksCurrentTF(void) {
	sndSendCmd(SNDPL_CMD_QUERY_CURRENT_TF);
}

void sndQueryTracksCurrentUD(void) {
	sndSendCmd(SNDPL_CMD_QUERY_CURRENT_UD);
}

void sndQueryTracksCurrentFlash(void) {
	sndSendCmd(SNDPL_CMD_QUERY_CURRENT_FLASH);
}

