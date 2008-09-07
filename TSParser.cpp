/*
 * TSParser.cpp
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#include <cstring>

#include "TSParser.h"
#include "TSPacket.h"
#include "EventInformationTable.h"

TSParser::TSParser() {

}

TSParser::~TSParser() {
}

void TSParser::parse(istream &inputStream, int maxCount) {
	unsigned char tsPacketBuf[188];
	unsigned char eitSectionBuf[4096];
	int eitSectionBufLength = 0;
	int eitSectionLength = 0;
	int count = 0;
	while (count < maxCount) {
		inputStream.read((char *) tsPacketBuf, 188);
		TSPacket *packet = TSPacket::parseTSPacket(tsPacketBuf);
		if (packet != NULL) {
			if (packet->getPID() == 0x12) {
				// EIT
				if (packet->isPayloadStart() && packet->getPayload()[0] == 0) {
					eitSectionBufLength = 0;
					memcpy(eitSectionBuf, packet->getPayload() + 1, 183);
					eitSectionBufLength += 183;
					eitSectionLength = SectionFormat::getSectionLengthByBuf(
							eitSectionBuf);
				} else {
					memcpy(eitSectionBuf, packet->getPayload(), 184);
					eitSectionBufLength += 184;
				}
				if (eitSectionLength != 0 && eitSectionLength + 3
						<= eitSectionBufLength) {
					if (eitSectionBuf[0] >= 0x4e && eitSectionBuf[0] <= 0x6f) {
						// Section Information Debug
						for (int i = 0; i < eitSectionBufLength; i++) {
							printf("%02x ", eitSectionBuf[i]);
						}
						printf("\n");
						printf("Section Length = %d(%04x)", eitSectionLength,
								eitSectionLength);
						EventInformationTable eit(eitSectionBuf,
								eitSectionLength + 3);
						vector<EventInformation> &events = eit.getEvents();
						for (size_t i = 0; i < events.size(); i++) {
							time_t startTime = events[i].getStartTime();
							printf("EID=%x %s(%02d:%02d:%02d) len=%d\n",
									events[i].getEventID(), ctime(&startTime),
									events[i].getDurationHour(),
									events[i].getDurationMinute(),
									events[i].getDurationSecond(),
									events[i].getDescriptorsLoopLength());
						}
					}
					eitSectionBufLength = 0;
					eitSectionLength = 0;
				}
			}
		}
		delete packet;
		count++;
	}
}

