/*
 * ExtendedEventDescriptor.h
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#ifndef EXTENDEDEVENTDESCRIPTOR_H_
#define EXTENDEDEVENTDESCRIPTOR_H_

#include "Descriptor.h"

#include <vector>
#include <string>

using namespace std;

class ExtendedEventDescriptor: public Descriptor {
	int descriptorNumber;
	int lastDescriptorNumber;
	char languageCode[3];
	size_t lengthOfItems;
	vector<string> itemDescriptionStrings;
	vector<string> itemStrings;
	string textString;
public:
	ExtendedEventDescriptor(unsigned char *ptr, size_t length);
	~ExtendedEventDescriptor();
	void printDescriptorBytes();
};

#endif /* EXTENDEDEVENTDESCRIPTOR_H_ */
