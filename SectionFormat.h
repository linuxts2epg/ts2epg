/*
 * SectionFormat.h
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#ifndef SECTIONFORMAT_H_
#define SECTIONFORMAT_H_

#include <cstdio>

class SectionFormat {
protected:
	unsigned char header[3];
	unsigned char *data;
	size_t dataLength;
public:
	SectionFormat();
	SectionFormat(unsigned char *ptr, size_t length);
	virtual ~SectionFormat();
	static size_t getSectionLengthByBuf(unsigned char *ptr);
	unsigned char getTableIdentifier();
	int getSectionSyntaxIndicator();
	size_t getSectionLength();
	unsigned char *getData();
	size_t getDataLength();
};

#endif /* SECTIONFORMAT_H_ */
