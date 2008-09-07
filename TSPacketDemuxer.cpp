/*
 * TSPacketDemuxer.cpp
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#include "TSPacketDemuxer.h"
#include "EventInformationTable.h"

TSPacketDemuxer::TSPacketDemuxer(istream &inStream) :
	inStream(inStream), eitBufferLength(0) {

}

TSPacketDemuxer::~TSPacketDemuxer() {
}

SectionFormat *TSPacketDemuxer::getNext() {
	unsigned char tsBuf[188];
	int tsHeaderLen = 4;

	if (eitBufferLength == 0) {
		// 処理中の途切れたセクション形式のデータがない場合
		// PID=0x12のペイロードユニット開始まで読み飛ばす
		do {
			inStream.read((char *) tsBuf, 188);
			if (tsBuf[0] != 0x47) {
				cerr << "Illegal TS packet" << endl;
			} else {
			}
		} while (!((tsBuf[1] & 0x40) && ((tsBuf[1] & 0x1f) << 8 | tsBuf[2])
				== 0x12 && tsBuf[4] == 0));

		tsHeaderLen = 5;

		// セクション長を取得
		eitLength = SectionFormat::getSectionLengthByBuf(tsBuf + tsHeaderLen);

		// TSペイロードをセクションバッファにコピー
		memcpy(eitBuffer, tsBuf + tsHeaderLen, 188 - tsHeaderLen);

		eitBufferLength = 188 - tsHeaderLen;

//		printf("TS packet start\n");
//		for (int i = 0; i < 188; i++) {
//			printf("%02x ", tsBuf[i]);
//		}
//		printf("\n");
	} else {
		// 現在バッファに溜まっているものを続けて処理
		// sectionBufLength > 0
		// sectionLength != 0 のはず
		eitLength = SectionFormat::getSectionLengthByBuf(eitBuffer);
	}

	tsHeaderLen = 4;

	// セクションバッファ長 >= セクション長 + 3になるまで
	// セクションバッファにデータを読み込む
	while (eitBufferLength < eitLength + 3) {
		// PID=0x12まで飛ばす
		do {
			inStream.read((char *) tsBuf, 188);
			if (tsBuf[0] != 0x47) {
				cerr << "Illegal TS packet" << endl;
			}
		} while (((tsBuf[1] & 0x1f) << 8 | tsBuf[2]) != 0x12);

//		printf("TS packet\n");
//		for (int i = 0; i < 188; i++) {
//			printf("%02x ", tsBuf[i]);
//		}
//		printf("\n");

		// TSペイロードをセクションバッファにコピー
		memcpy(eitBuffer + eitBufferLength, tsBuf + tsHeaderLen, 188
				- tsHeaderLen);
		eitBufferLength += 188 - tsHeaderLen;
	}
//	printf("EIT data\n");
//	for (size_t i = 0; i < eitLength + 3; i++) {
//		printf("%02x ", eitBuffer[i]);
//	}
//	printf("\n");

	SectionFormat *section =
			new EventInformationTable(eitBuffer, eitLength + 3);

	// バッファの残りをバッファの先頭に移動し次に備える
	unsigned char tmpBuf[200];
	size_t remainingBufferLength = eitBufferLength - (eitLength + 3);
	eitBufferLength = remainingBufferLength;
	if (remainingBufferLength > 0) {
		if (*(eitBuffer + eitLength + 3) == 0xff) {
			// 穴埋めパケットが検出されたので、残りバッファは捨てる
			eitBufferLength = 0;
		} else {
			memcpy(tmpBuf, eitBuffer + eitLength + 3, remainingBufferLength);
			memcpy(eitBuffer, tmpBuf, remainingBufferLength);
		}
	}

	if (((EventInformationTable *) section)->hasError()) {
		//エラーがあるので残りバッファは捨てる
		eitBufferLength = 0;
	}

	return section;
}
