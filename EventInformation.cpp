/*
 * EventInformation.cpp
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#include "EventInformation.h"
#include "DescriptorFactory.h"

EventInformation::EventInformation() : descriptors() {
//	printf("EI constructor %x\n", this);

}

EventInformation::EventInformation(const EventInformation &ei) :
	eventID(ei.eventID), startTime(ei.startTime), durationHour(ei.durationHour),
	durationMinute(ei.durationMinute), durationSecond(ei.durationSecond),
	runningStatus(ei.runningStatus), freeCAMode(ei.freeCAMode),
	descriptorsLoopLength(ei.descriptorsLoopLength) {
//	printf("EI copy constructor %x from %x\n", this, &ei);
//	printf("Descriptor v address = %x\n", &descriptors);
//	for (vector<Descriptor *>::iterator itr = ei.descriptors.begin(); itr != ei.descriptors.end(); itr++) {
//		descriptors.push_back(*itr);
//	}
	bool hasError;
	for (int i = 0; i < ei.descriptors.size(); i++) {
		Descriptor *desc = DescriptorFactory::create(ei.descriptors[i]->getBufPtr(), ei.descriptors[i]->getBufLen(), hasError);
//		printf("desc(%x) = DescriptorFactory::create(bufptr())\n", desc);
		descriptors.push_back(desc);
	}
//	printf("EI copy constructor end\n");
}

EventInformation::~EventInformation() {
//	printf("EI desctructor %x\n", this);
//	printf("Descriptor v address = %x\n", &descriptors);
	for (int i = 0; i < descriptors.size(); i++) {
//		printf("%x,", descriptors[i]);
		Descriptor *ptr = descriptors[i];
		delete ptr;
	}
//	printf("\nEI desctructor end\n");
}

int bcd2int(unsigned char bcd) {
	return ((bcd & 0xf0) >> 4) * 10 + (bcd & 0x0f);
}

void EventInformation::parseEventInformation(vector<EventInformation> &events,
		unsigned char *ptr, size_t length, bool &hasError) {
	// Section Information Debug

//	printf("parseEventInformation\n");
	unsigned int index = 0;
//	for (int i = 0; i < length; i++) {
//		printf("%02x ", ptr[i]);
//	}
//	printf("\n");
	while (index < length) {
//		printf("oka\n");
		EventInformation event;
		event.eventID = (ptr[0] << 8) | ptr[1];
		struct tm time = {0};
		time.tm_year = 100;
		time.tm_mon = 0;
		time.tm_mday = 1 + (((ptr[2] << 8) | ptr[3]) - 51544L);
		time.tm_hour = bcd2int(ptr[4]);
		time.tm_min = bcd2int(ptr[5]);
		time.tm_sec = bcd2int(ptr[6]);
//		printf("oka2\n");
		event.startTime = mktime(&time);
		event.durationHour = bcd2int(ptr[7]);
		event.durationMinute = bcd2int(ptr[8]);
		event.durationSecond = bcd2int(ptr[9]);
		event.runningStatus = (ptr[10] & 0xe0) >> 5;
		event.freeCAMode = (ptr[10] & 0x10) ? true : false;
//		printf("oka3\n");
		event.descriptorsLoopLength = (ptr[10] & 0x0f) << 8 | ptr[11];
		Descriptor::parseDescriptors(event.descriptors, ptr + 12, event.descriptorsLoopLength, hasError);
//		for (vector<Descriptor *>::iterator itr = event.descriptors.begin(); itr != event.descriptors.end(); itr++) {
//			(*itr)->printDescriptorBytes();
//		}
//		printf("okb\n");
		events.push_back(event);
//		printf("okc\n");
//		printf("EventID = %d, Start Date = %s", event.eventID, ctime(&event.startTime));
//		printf("Duration = %02d:%02d:%02d\n", event.durationHour, event.durationMinute, event.durationSecond);
		ptr += 12 + event.descriptorsLoopLength;
		index += 12 + event.descriptorsLoopLength;
//		printf("okd %d %d\n",index,length);
	}
//	printf("oke\n");
}

unsigned int EventInformation::getEventID() {
	return eventID;
}

time_t EventInformation::getStartTime() {
	return startTime;
}

int EventInformation::getDurationHour() {
	return durationHour;
}

int EventInformation::getDurationMinute() {
	return durationMinute;
}

int EventInformation::getDurationSecond() {
	return durationSecond;
}

size_t EventInformation::getDescriptorsLoopLength() {
	return descriptorsLoopLength;
}

vector<Descriptor *> &EventInformation::getDescriptors() {
	return descriptors;
}
