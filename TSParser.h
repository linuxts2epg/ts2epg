/*
 * TSParser.h
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#ifndef TSPARSER_H_
#define TSPARSER_H_

#include <vector>
#include <iostream>

#include "SectionFormat.h"

using namespace std;

class TSParser {
	vector<SectionFormat> sectionFormats;
public:
	TSParser();
	~TSParser();
	void parse(istream &inputStream, int maxCount);
};

#endif /* TSPARSER_H_ */
