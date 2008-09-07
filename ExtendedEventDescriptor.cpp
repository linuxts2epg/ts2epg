/*
 * ExtendedEventDescriptor.cpp
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#include "ExtendedEventDescriptor.h"
#include "ARIBStringConverter.h"

ExtendedEventDescriptor::ExtendedEventDescriptor(unsigned char *ptr, size_t length) :
Descriptor(ptr, length) {
 descriptorNumber = (ptr[2] & 0xf0) >> 4;
 lastDescriptorNumber = ptr[2] & 0x0f;
	memcpy(languageCode, ptr + 3, 3);
	lengthOfItems = ptr[6];
	size_t index = 0;
	//	for (int i = 0; i < length; i++) printf("%02x ", ptr[i]);
	//	printf("\n");
	while (index < lengthOfItems) {
		char itemDescriptorBuf[256] = {0};
		char itemBuf[256] = {0};
		size_t itemDescriptorLength = ptr[7];
		//		printf("ItemDescriptorLength = %x(%d)\n", itemDescriptorLength, itemDescriptorLength);
		index++;
		memcpy(itemDescriptorBuf, ptr + 7 + index, itemDescriptorLength);
		//		for (int i = 0; i < itemDescriptorLength; i++) printf("%02x ",ptr[7+index+i]);
		//		printf("\n");
		index += itemDescriptorLength;
		size_t itemLength = ptr[7 + index];
		//		printf("ItemLength = %x(%d)\n", itemLength, itemLength);
		index++;
		memcpy(itemBuf, ptr + 7 + index, itemLength);
		//		for (int i = 0; i < itemLength; i++) printf("%02x ",ptr[7+index+i]);
		//		printf("\n");
		index += itemLength;
		int result;
		char *puItemDescriptorBuf = NULL;
		size_t uItemDescriptorLength = 0;
		char *puItemBuf = NULL;
		size_t uItemLength = 0;
		result = ARIBStringConverter::convert((const char *) itemDescriptorBuf, itemDescriptorLength, puItemDescriptorBuf, uItemDescriptorLength);
		result = ARIBStringConverter::convert((const char *) itemBuf, itemLength, puItemBuf, uItemLength);
		//		printf("uItemDescriptor = %s\n", puItemDescriptorBuf);
		//		printf("uItem = %s\n", puItemBuf);
		string itemDescriptorString(puItemDescriptorBuf);
		string itemString(puItemBuf);
		itemDescriptionStrings.push_back(itemDescriptorString);
		itemStrings.push_back(itemString);
		delete puItemDescriptorBuf;
		delete puItemBuf;
	}
	size_t textLength = ptr[9 + index];
	char textBuf[256] = {0};
	memcpy(textBuf, ptr + 10 + index, textLength);
	int result;
	char *puTextBuf = NULL;
	size_t uTextLength = 0;
	result = ARIBStringConverter::convert((const char *) textBuf, textLength, puTextBuf, uTextLength);
	textString.append((const char *) puTextBuf);
	delete puTextBuf;
}

ExtendedEventDescriptor::~ExtendedEventDescriptor() {
	// TODO Auto-generated destructor stub
}

void ExtendedEventDescriptor::printDescriptorBytes	() {
	printf("ExtendedEventDescriptorID = %02x, length=0x%02x(%d)\n", descriptorID, descriptorLength, descriptorLength);
	printf("Descriptor Number = %d/%d (%02x)\n", descriptorNumber, lastDescriptorNumber, buf[2]);
	printf("Length Of Items = %02x(%d)\n", lengthOfItems, lengthOfItems);
	for (size_t i = 0; i < itemDescriptionStrings.size(); i++) {
		printf("Item Count = %d\n", i);
		printf("Item Description = %s\n", itemDescriptionStrings[i].c_str());
		printf("Item = %s\n", itemStrings[i].c_str());
	}
	printf("text = %s\n", textString.c_str());
	printf("\n");
}
