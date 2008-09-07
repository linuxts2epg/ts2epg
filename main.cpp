#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstring>
#include <map>

#include "EventInformationTable.h"
#include "TSPacketDemuxer.h"
#include "Descriptor.h"
#include "ShortEventDescriptor.h"

using namespace std;

class Program {
  unsigned int eventID;
  time_t startTime;
  time_t endTime;
  string title;
  string description;
  friend bool time_asc(const Program &, const Program &);
public:
  Program(unsigned int eventID, time_t startTime, int durationHour, 
	  int durationMinute, int durationSecond, string &title,
	  string &description)
    : eventID(eventID),
      startTime(startTime),
      title(title),
      description(description) {
    struct tm *ptime;
    ptime = localtime(&startTime);
    ptime->tm_hour += durationHour;
    ptime->tm_min += durationMinute;
    ptime->tm_sec += durationSecond;
    endTime = mktime(ptime);
  }
  unsigned int getEventID() {
    return eventID;
  }
  time_t getStartTime() {
    return startTime;
  }
  time_t getEndTime() {
    return endTime;
  }
  string &getTitle() {
    return title;
  }
  string &getDescription() {
    return description;
  }
  void printProgram() {
    char startTimeBuf[100];
    char endTimeBuf[100];
    struct tm *ptime;
    ptime = localtime(&startTime);
    strftime(startTimeBuf, 99, "%F %T", ptime);
    ptime = localtime(&endTime);
    strftime(endTimeBuf, 99, "%F %T", ptime);
    printf("%04x:%s:%s\n", eventID, startTimeBuf, endTimeBuf);
    printf("%s\n", title.c_str());
    printf("%s\n", description.c_str());
    printf("\n");
  }
};

bool time_asc(const Program &left, const Program &right) {
  return left.startTime < right.startTime;
}
  
int main(int argc, char *argv[]) {
	if (argc < 3)
		return -1;
	ifstream ifs;
	ifs.open(argv[1], ios::in);
	
	map<unsigned int, Program> programList;
	
	EventInformationTable *pEit = NULL;

	TSPacketDemuxer tsPacketDemuxer(ifs);

	int i;
	for (i = 0; i < atoi(argv[2]); i++) {
	printf("i:%d\n", i);
	pEit = (EventInformationTable *) tsPacketDemuxer.getNext();

	//	printf("Section Length = %04x(%d)\n", pEit->getSectionLength(),
	//			pEit->getSectionLength());
	for (vector<EventInformation>::iterator itr = pEit->getEvents().begin(); itr
			!= pEit->getEvents().end(); itr++) {
	  //		time_t time = itr->getStartTime();
		//printf("EventID = %04x, DesciptorsLoopLength = %04x\nStart Date = %s",
		//				itr->getEventID(), itr->getDescriptorsLoopLength(),
		//				ctime(&time));
	//		printf("Duration = %02d:%02d:%02d\n", itr->getDurationHour(),
		//				itr->getDurationMinute(), itr->getDurationSecond());
//printf("Size of Desriptors = %d\n", itr->getDescriptors().size());
		for (vector<Descriptor *>::iterator dItr =
				itr->getDescriptors().begin(); dItr
				!= itr->getDescriptors().end(); dItr++) {
		  if ((*dItr)->getDescriptorID() == 0x4d || (*dItr)->getDescriptorID() == 0x4e)
		    //			(*dItr)->printDescriptorBytes();
		  if ((*dItr)->getDescriptorID() == 0x4d) {
		    // 短形式イベント記述子なので、番組表に追加
		    ShortEventDescriptor *sed = dynamic_cast<ShortEventDescriptor *>(*dItr);
		    string title(sed->getUEventNameChar());
		    string description(sed->getUTextChar());
		    Program program(itr->getEventID(), itr->getStartTime(), itr->getDurationHour(), itr->getDurationMinute(), itr->getDurationSecond(), title, description);
		    programList.insert(pair<unsigned int, Program>(itr->getEventID(), program));
		  }
		}
	}

	delete pEit;
	}
	ifs.close();

	vector<Program> programs;

	for (map<unsigned int, Program>::iterator itr = programList.begin(); itr != programList.end(); itr++) {
	  programs.push_back(itr->second);
	}

	sort(programs.begin(), programs.end(), time_asc);
	printf("%d\n", programs.size());
	printf("Program Table:\n\n");
	for (vector<Program>::iterator itr = programs.begin(); itr != programs.end(); itr++) {
	  itr->printProgram();
	}

	return 0;
}
