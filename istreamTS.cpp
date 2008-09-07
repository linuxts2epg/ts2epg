/*
 * istreamTS.cpp
 *
 *  Created on: 2008/09/04
 *      Author: linux_ts2epg
 */

#include "istreamTS.h"
#include "EventInformationTable.h"

#include <cstring>

istreamTS::istreamTS(istream &ris) :
	ris(ris), sectionBufLength(0) {
	// TODO Auto-generated constructor stub

}

istreamTS::~istreamTS() {
	// TODO Auto-generated destructor stub
}

istreamTS &istreamTS::operator >>(SectionFormat &section)
{
	unsigned char tsBuf[188];
	int tsHeaderLen = 4;

	if (sectionBufLength == 0) {
		// 処理中の途切れたセクション形式のデータがない場合
		// PID=0x12のペイロードユニット開始まで読み飛ばす
		do {
			ris.read((char *)tsBuf, 188);
			if (tsBuf[0] != 0x47) {
				cerr << "Illegal TS packet" << endl;
			} else {
			}
		}while (!((tsBuf[1] & 0x40) && ((tsBuf[1] & 0x1f) << 8 | tsBuf[2]) == 0x12) && tsBuf[4] == 0);

		tsHeaderLen = 5;

		// セクション長を取得
		sectionLength = SectionFormat::getSectionLengthByBuf(tsBuf + tsHeaderLen);

		// TSペイロードをセクションバッファにコピー
		memcpy(sectionBuf, tsBuf + tsHeaderLen, 188 - tsHeaderLen);

		sectionBufLength = 188 - tsHeaderLen;
	} else {
		// 現在バッファに溜まっているものを続けて処理
		// sectionBufLength > 0
		// sectionLength != 0 のはず
		sectionLength = SectionFormat::getSectionLengthByBuf(sectionBuf);
	}

	tsHeaderLen = 4;

	// セクションバッファ長 >= セクション長 + 3になるまで
	// セクションバッファにデータを読み込む
	while (sectionBufLength < sectionLength + 3) {
		// PID=0x12まで飛ばす
		do {
			ris.read((char *)tsBuf, 188);
			if (tsBuf[0] != 0x47) {
				cerr << "Illegal TS packet" << endl;
			}
		}while (((tsBuf[1] & 0x1f) << 8 | tsBuf[2]) == 0x12);

		// TSペイロードをセクションバッファにコピー
		memcpy(sectionBuf + sectionBufLength, tsBuf + tsHeaderLen, 188 - tsHeaderLen);
		sectionBufLength += 188 - tsHeaderLen;
	}


	section = new EventInformationTable(sectionBuf, sectionLength + 3);

	// バッファの残りをバッファの先頭に移動し次に備える
	unsigned char tmpBuf[200];
	size_t remainingBufLength = sectionBufLength - (sectionLength + 3);
	if (remainingBufLength> 0) {
		memcpy(tmpBuf, sectionBuf + sectionLength + 3, remainingBufLength);
		memcpy(sectionBuf, tmpBuf, remainingBufLength);
		sectionBufLength = remainingBufLength;
	}

	return *this;
}

