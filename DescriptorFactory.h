/*
 * DescriptorFactory.h
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#ifndef DESCRIPTORFACTORY_H_
#define DESCRIPTORFACTORY_H_

#include "Descriptor.h"

class DescriptorFactory {
	DescriptorFactory();
	~DescriptorFactory();
public:
	static Descriptor *create(unsigned char *ptr, size_t length, bool &hasError);
};

#endif /* DESCRIPTORFACTORY_H_ */
