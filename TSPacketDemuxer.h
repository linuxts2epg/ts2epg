/*
 * TSPacketDemuxer.h
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#ifndef TSPACKETDEMUXER_H_
#define TSPACKETDEMUXER_H_

#include <iostream>

#include "SectionFormat.h"

using namespace std;

class TSPacketDemuxer {
	istream &inStream;

	// EITのバッファ。他にもいろいろ作らないといけない。
	unsigned char eitBuffer[6000];
	size_t eitBufferLength;
	size_t eitLength;
public:
	TSPacketDemuxer(istream &inStream);
	~TSPacketDemuxer();
	SectionFormat *getNext();
};

#endif /* TSPACKETDEMUXER_H_ */
