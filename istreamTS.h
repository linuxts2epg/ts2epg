/*
 * istreamTS.h
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#ifndef ISTREAMTS_H_
#define ISTREAMTS_H_

#include <iostream>
#include <fstream>
#include "SectionFormat.h"

using namespace std;

class istreamTS {
	istream &ris;
	unsigned char sectionBuf[6144];
	size_t sectionBufLength;
	size_t sectionLength;
public:
	istreamTS(istream &ris);
	~istreamTS();

	istreamTS &operator >>(SectionFormat &section);
//	istreamTS &operator >>(istreamTS &(*pmanipFunc)(istreamTS &));
};

#endif /* ISTREAMTS_H_ */
