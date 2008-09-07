/*
 * SectionFormat.cpp
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#include "SectionFormat.h"

#include <cstring>

SectionFormat::SectionFormat() {

}

SectionFormat::SectionFormat(unsigned char *ptr, size_t length) {
	memcpy(header, ptr, 3);
	data = new unsigned char[length - 3];
	memcpy(data, ptr + 3, length - 3);
	dataLength = length - 3;
}

SectionFormat::~SectionFormat() {
	if (data) delete[] data;
}

size_t SectionFormat::getSectionLengthByBuf(unsigned char *ptr)
{
	return (*(ptr + 1) & 0x0f) << 8 | *(ptr + 2);
}

unsigned char SectionFormat::getTableIdentifier()
{
	return header[0];
}

int SectionFormat::getSectionSyntaxIndicator()
{
	return header[1] & 0x80;
}

size_t SectionFormat::getSectionLength()
{
	return (header[1] & 0x0f) << 8 | header[2];
}

unsigned char *SectionFormat::getData()
{
	return data;
}

size_t SectionFormat::getDataLength()
{
	return dataLength;
}







