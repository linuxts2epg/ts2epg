/*
 * TSPacket.h
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#ifndef TSPACKET_H_
#define TSPACKET_H_

using namespace std;

class TSPacket {
	unsigned char header[3];
	unsigned char payload[184];
public:
	TSPacket();
	~TSPacket();
	static TSPacket *parseTSPacket(unsigned char *ptr);
	int hasBitError();
	int isPayloadStart();
	int isPrior();
	unsigned int getPID();
	unsigned char (&getPayload())[184];
	unsigned char getScrambleControl();
	unsigned char getAdaptationFieldControl();
	unsigned char getSequenceNumber();
};

#endif /* TSPACKET_H_ */
