/*
 * ShortEventDescriptor.cpp
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#include "ShortEventDescriptor.h"
#include "DescriptorFactory.h"
#include "ARIBStringConverter.h"

ShortEventDescriptor::ShortEventDescriptor(unsigned char *ptr, size_t length) :
	Descriptor(ptr, length), eventNameChar(NULL), uEventNameChar(NULL), textChar(NULL), uTextChar(NULL) {
//	printf("ShortEvent constructor %x\n", this);
	memcpy(languageCode, ptr + 2, 3);

	eventNameLength = ptr[5];
	eventNameChar = new char[eventNameLength + 1];
	memcpy(eventNameChar, ptr + 6, eventNameLength);
	eventNameChar[eventNameLength] = '\0';

	textLength = ptr[6 + eventNameLength];
	textChar = new char[textLength + 1];
	memcpy(textChar, ptr + 6 + eventNameLength + 1, textLength);
	textChar[textLength] = '\0';

	// ARIB文字列からUTF-8に変換
	int result;
	//	printf("EventNameLength: %d\n", eventNameLength);
	//for (int i = 0; i < eventNameLength + 1; i++) {
	//  printf("%02x ", (unsigned char) eventNameChar[i]);
	//	}
	//printf("\n");
	//printf("TextLength: %d\n", textLength);
	//for (int i = 0; i < textLength + 1; i++) {
	//  printf("%02x ", (unsigned char) textChar[i]);
	//}
	result = ARIBStringConverter::convert((const char *) eventNameChar, eventNameLength, uEventNameChar, uEventNameLength);
	result = ARIBStringConverter::convert((const char *) textChar, textLength, uTextChar, uTextLength);
}

ShortEventDescriptor::ShortEventDescriptor(ShortEventDescriptor &sed) : Descriptor(sed) {
//	printf("ShortEvent copy constructor %x\n", this);
	memcpy(languageCode, sed.languageCode, 3);
	eventNameLength = sed.eventNameLength;
	if (sed.eventNameChar) {
		eventNameChar = new char[eventNameLength + 1];
		memcpy(eventNameChar, sed.eventNameChar, eventNameLength + 1);
	}
	uEventNameLength = sed.uEventNameLength;
	if (sed.uEventNameChar) {
		uEventNameChar = new char[uEventNameLength + 1];
		memcpy(uEventNameChar, sed.uEventNameChar, uEventNameLength + 1);
	}
	textLength = sed.textLength;
	if (sed.textChar) {
		textChar = new char[textLength + 1];
		memcpy(textChar, sed.textChar, textLength + 1);
	}
	uTextLength = sed.uTextLength;
	if (sed.uTextChar) {
		uTextChar = new char[uTextLength + 1];
		memcpy(uTextChar, sed.uTextChar, uTextLength + 1);
	}

}

ShortEventDescriptor::~ShortEventDescriptor() {
  if (eventNameChar) delete eventNameChar;
  if (uEventNameChar) delete uEventNameChar;
  if (textChar) delete textChar;
  if (uTextChar) delete uTextChar;
}

void ShortEventDescriptor::printDescriptorBytes() {
	printf("ShortEventDescriptorID = %02x, length=0x%02x(%d)\n", descriptorID, descriptorLength, descriptorLength);
	printf("event_name = %s\n", uEventNameChar);
	printf("text = %s\n", uTextChar);
	printf("\n");
}

char *ShortEventDescriptor::getUEventNameChar() {
  return uEventNameChar;
}
char *ShortEventDescriptor::getUTextChar() {
  return uTextChar;
}
