/*
 * ShortEventDescriptor.h
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#ifndef SHORTEVENTDESCRIPTOR_H_
#define SHORTEVENTDESCRIPTOR_H_

#include "Descriptor.h"

class ShortEventDescriptor: public Descriptor {
	char languageCode[3];
	size_t eventNameLength; // イベント名長さ
	char *eventNameChar; // イベント名文字列
	size_t uEventNameLength; // UTF-8イベント名長さ
	char *uEventNameChar; // UTF-8イベント名文字列
	size_t textLength; // テキスト長さ
	char *textChar; // テキスト文字列
	size_t uTextLength; // UTF-8テキスト長さ
	char *uTextChar; // UTF-8テキスト文字列
public:
	ShortEventDescriptor(unsigned char *ptr, size_t length);
	ShortEventDescriptor(ShortEventDescriptor &sed);
	virtual ~ShortEventDescriptor();
	virtual void printDescriptorBytes();
	char *getUEventNameChar();
	char *getUTextChar();
};

#endif /* SHORTEVENTDESCRIPTOR_H_ */
