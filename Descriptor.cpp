/*
 * Descriptor.cpp
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#include "Descriptor.h"
#include "DescriptorFactory.h"

Descriptor::Descriptor() {
  //	printf("Descriptor Default Constructor %x\n", this);

}

Descriptor::Descriptor(unsigned char *ptr, size_t length) {
  //printf("Descriptor Constructor %x\n", this);
	descriptorID = ptr[0];
	descriptorLength = ptr[1];
	memcpy(buf, ptr, descriptorLength + 2);
	buflen = descriptorLength + 2;
}

Descriptor::Descriptor(Descriptor &descriptor) {
  //printf("Descriptor Copy Constructor %x\n", this);
	descriptorID = descriptor.descriptorID;
	descriptorLength = descriptor.descriptorLength;
	buflen = descriptorLength + 2;
	memcpy(buf, descriptor.buf, buflen);
}

Descriptor::~Descriptor() {

}

unsigned char Descriptor::getDescriptorID() {
	return descriptorID;
}

unsigned char Descriptor::getDescriptorLength() {
	return descriptorLength;
}

void Descriptor::parseDescriptors(vector<Descriptor *> &descriptors,
		unsigned char *ptr, size_t descriptorsLoopLength, bool &hasError) {
	size_t processedByte = 0;

	while (processedByte < descriptorsLoopLength) {
		size_t descriptorLength = getDescriptorLengthByBuffer(ptr);
		Descriptor *descriptor = DescriptorFactory::create(ptr,
				descriptorLength + 2, hasError);
		//		descriptor->printDescriptorBytes();
		if (descriptor) descriptors.push_back(descriptor);
		processedByte += descriptorLength + 2;
		//		printf("%d, %d\n", processedByte, descriptorsLoopLength);
		ptr += descriptorLength + 2;
	}

	if (processedByte != descriptorsLoopLength) {
		hasError = true;
	}

}

size_t Descriptor::getDescriptorLengthByBuffer(unsigned char *ptr) {
	return ptr[1];
}

void Descriptor::printDescriptorBytes() {
	printf("DescriptorID = %02x, length=0x%02x(%d)\n", descriptorID,
			descriptorLength, descriptorLength);
	for (int i = 0; i < descriptorLength + 2; i++) {
		printf("%02x ", buf[i]);
	}
	printf("\n");
}

unsigned char *Descriptor::getBufPtr() {
	return buf;
}

size_t Descriptor::getBufLen() {
  //    printf("Descriptor(%x)::getBufLen()\n", this);
	return buflen;
}
