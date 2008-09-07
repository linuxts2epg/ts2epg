/*
 * EventInformationTable.h
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#ifndef EVENTINFORMATIONTABLE_H_
#define EVENTINFORMATIONTABLE_H_

#include "ExtendedSectionFormat.h"

#include <vector>

#include "EventInformation.h"

using namespace std;

class EventInformationTable: public ExtendedSectionFormat {
	unsigned int transportStreamID;
	unsigned int originalNetworkID;
	unsigned char segmentLastSectionNumber;
	unsigned char lastTableID;
	size_t eventNumber;
	vector<EventInformation> events;
	bool error;
public:
	EventInformationTable();
	EventInformationTable(unsigned char *ptr, size_t length);
	~EventInformationTable();
	vector<EventInformation> &getEvents();
	bool hasError();
};

#endif /* EVENTINFORMATIONTABLE_H_ */
