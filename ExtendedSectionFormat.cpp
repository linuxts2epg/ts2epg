/*
 * ExtendedSectionFormat.cpp
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#include "ExtendedSectionFormat.h"

#include <cstring>

ExtendedSectionFormat::ExtendedSectionFormat(unsigned char *ptr, size_t length) {
	memcpy(header, ptr, 3);
	memcpy(extendedHeader, ptr + 3, 5);
	data = new unsigned char[length - 9];
	memcpy(data, ptr + 8, length - 12); // CRC 4byte
	dataLength = length - 12;
	crc = *(ptr + length - 4);
}

ExtendedSectionFormat::~ExtendedSectionFormat() {
}

unsigned int ExtendedSectionFormat::getExtendedTableIdentifier()
{
	return (extendedHeader[0] << 8) | extendedHeader[1];
}

unsigned char ExtendedSectionFormat::getVersion()
{
	return (extendedHeader[2] & 0x3f) >> 1;
}

int ExtendedSectionFormat::getCurrentNextIndicator()
{
	return extendedHeader[2] & 0x01;
}

unsigned char ExtendedSectionFormat::getSectionNumber()
{
	return extendedHeader[3];
}

unsigned char ExtendedSectionFormat::getLastSectionNumber()
{
	return extendedHeader[4];
}

bool ExtendedSectionFormat::isValidCRC()
{
	return true;
}












