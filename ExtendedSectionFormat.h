/*
 * ExtendedSectionFormat.h
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#ifndef EXTENDEDSECTIONFORMAT_H_
#define EXTENDEDSECTIONFORMAT_H_

#include "SectionFormat.h"

class ExtendedSectionFormat: public SectionFormat {
	unsigned char extendedHeader[5];
	int crc;
public:
	ExtendedSectionFormat();
	ExtendedSectionFormat(unsigned char *ptr, size_t length);
	virtual ~ExtendedSectionFormat();
	void parseSectionFormat(unsigned char *prtr, size_t length);
	unsigned int getExtendedTableIdentifier();
	unsigned char getVersion();
	int getCurrentNextIndicator();
	unsigned char getSectionNumber();
	unsigned char getLastSectionNumber();
	bool isValidCRC();
};

#endif /* EXTENDEDSECTIONFORMAT_H_ */
