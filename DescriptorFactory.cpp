/*
 * DescriptorFactory.cpp
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#include "DescriptorFactory.h"
#include "ShortEventDescriptor.h"
#include "ExtendedEventDescriptor.h"


DescriptorFactory::DescriptorFactory() {
}

DescriptorFactory::~DescriptorFactory() {
}

Descriptor *DescriptorFactory::create(unsigned char *ptr, size_t length,
		bool &hasError) {
  hasError = false;
  //  for (int i = 0; i < length; i++) printf(" %02x", ptr[i]);
  //  printf("\n");
	unsigned char descriptorID = ptr[0];
	Descriptor *pDescriptor = NULL;
	switch (descriptorID) {
	case 0x09:
	case 0x0d:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x28:
	case 0x2a:
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x47:
	case 0x48:
	case 0x49:
	case 0x4a:
	case 0x4b:
	case 0x4c:
	case 0x4f:
	case 0x50:
	case 0x51:
	case 0x52:
	case 0x53:
	case 0x54:
	case 0x55:
	case 0x58:
	case 0x63:
	case 0xc0:
	case 0xc1:
	case 0xc2:
	case 0xc3:
	case 0xc4:
	case 0xc5:
	case 0xc6:
	case 0xc7:
	case 0xc8:
	case 0xc9:
	case 0xca:
	case 0xcb:
	case 0xcc:
	case 0xcd:
	case 0xce:
	case 0xcf:
	case 0xd0:
	case 0xd1:
	case 0xd2:
	case 0xd3:
	case 0xd4:
	case 0xd5:
	case 0xd6:
	case 0xd7:
	case 0xd8:
	case 0xd9:
	case 0xda:
	case 0xdb:
	case 0xdc:
	case 0xdd:
	case 0xde:
	case 0xdf:
	case 0xe0:
	case 0xf7:
	case 0xf8:
	case 0xf9:
	case 0xfa:
	case 0xfb:
	case 0xfc:
	case 0xfd:
	case 0xfe:
		pDescriptor = new Descriptor(ptr, length);
		break;

	case 0x4d: // 短形式イベント記述子
		pDescriptor = new ShortEventDescriptor(ptr, length);
		break;

	case 0x4e: // 拡張形式イベント記述子
		pDescriptor = new ExtendedEventDescriptor(ptr, length);
		break;

	default: // 記述子ID異常かも?
		if ((descriptorID >= 0x80 && descriptorID <= 0xbf) || (descriptorID
				>= 0xe1 && descriptorID <= 0xf6)) {
			pDescriptor = new Descriptor(ptr, length);
		} else {
			hasError = true;
		}
	}
	if (hasError) {
	  //	  printf("NULLLLLLLLLLLLLLLL descriptorID=%02x\n",descriptorID);
		return NULL;
	} else {
		return pDescriptor;
	}
}

