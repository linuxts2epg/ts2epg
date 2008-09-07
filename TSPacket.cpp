/*
 * TSPacket.cpp
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#include <cstdio>
#include <cstring>
#include "TSPacket.h"

TSPacket::TSPacket() {

}

TSPacket::~TSPacket() {

}

TSPacket *TSPacket::parseTSPacket(unsigned char *ptr) {
	// 同期バイトチェック
	if (*ptr != 0x47) {
		return NULL;
	}

	TSPacket *packet = new TSPacket();

	// ヘッダーをコピー
	memcpy(packet->header, ptr + 1, 3);

	// ペイロードをコピー
	memcpy(packet->payload, ptr + 4, 184);

	return packet;
}

int TSPacket::hasBitError() {
	return header[0] & 0x80;
}

int TSPacket::isPayloadStart() {
	return header[0] & 0x40;
}

int TSPacket::isPrior() {
	return header[0] & 0x20;
}

unsigned int TSPacket::getPID() {
	return (header[0] & 0x1f) << 8 | header[1];
}

unsigned char TSPacket::getScrambleControl()
{
	return header[2] & 0xc0;
}

unsigned char TSPacket::getAdaptationFieldControl()
{
	return header[2] & 0x30;
}

unsigned char TSPacket::getSequenceNumber() {
	return header[2] & 0x0f;
}

unsigned char (&TSPacket::getPayload())[184] {
	return payload;
}

