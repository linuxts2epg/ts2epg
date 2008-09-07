/*
 * EventInformationTable.cpp
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#include "EventInformationTable.h"

EventInformationTable::EventInformationTable(unsigned char *ptr, size_t length) :
	ExtendedSectionFormat(ptr, length), error(false) {
	transportStreamID = (data[0] << 8) | data[1];
	originalNetworkID = (data[2] << 8) | data[3];
	segmentLastSectionNumber = data[4];
	lastTableID = data[5];
	size_t tableLen = dataLength - 6;
	EventInformation::parseEventInformation(events, data + 6, tableLen, error);
//	printf("okf\n");
}

EventInformationTable::~EventInformationTable() {

}

vector<EventInformation> &EventInformationTable::getEvents()
{
	return events;
}

bool EventInformationTable::hasError() {
	return error;
}
