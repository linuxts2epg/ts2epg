/*
 * Descriptor.h
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

#include <vector>

using namespace std;


class Descriptor {
protected:
	unsigned char buf[4096]; // 仮のバッファー
	size_t buflen;
	unsigned char descriptorID;
	unsigned char descriptorLength;
	Descriptor();
public:
	Descriptor(unsigned char *ptr, size_t length); // バイト列を出すだけの実装
	Descriptor(Descriptor &descriptor);
	virtual ~Descriptor();
	unsigned char getDescriptorID();
	unsigned char getDescriptorLength();
	static void parseDescriptors(vector<Descriptor *> &descriptors, unsigned char *ptr, size_t descriptorsLoopLength, bool &hasError);
	static size_t getDescriptorLengthByBuffer(unsigned char *ptr);
	virtual void printDescriptorBytes();
	unsigned char *getBufPtr();
	size_t getBufLen();
};

#endif /* DESCRIPTOR_H_ */
