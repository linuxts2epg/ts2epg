/*
 * EventInformation.h
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#ifndef EVENTINFORMATION_H_
#define EVENTINFORMATION_H_

#include <ctime>
#include <vector>

#include "Descriptor.h"

using namespace std;

class EventInformation {
	unsigned int eventID;
	time_t startTime;
	int durationHour;
	int durationMinute;
	int durationSecond;
	unsigned char runningStatus;
	bool freeCAMode;
	size_t descriptorsLoopLength;
	vector<Descriptor *> descriptors;
public:
	EventInformation();
	EventInformation(const EventInformation &ei);
	~EventInformation();
	static void parseEventInformation(vector<EventInformation> &events, unsigned char *ptr, size_t length, bool &hasError);
	unsigned int getEventID();
	time_t getStartTime();
	int getDurationHour();
	int getDurationMinute();
	int getDurationSecond();
	size_t getDescriptorsLoopLength();
	vector<Descriptor *> &getDescriptors();
};

#endif /* EVENTINFORMATION_H_ */
