/*
 * ARIBStringConverter.h
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#ifndef ARIBSTRINGCONVERTER_H_
#define ARIBSTRINGCONVERTER_H_

#include <cstring>
#include <string>
#include <iconv.h>

using namespace std;

typedef basic_string<wchar_t> wctstring;

class ARIBStringConverter {
	typedef enum {
		UNKNOWN, KANJI, ALPHANUMERIC, HIRAGANA, KATAKANA, MOSAIC_A, MOSAIC_B, MOSAIC_C, MOSAIC_D, PROP_ALPHANUMERIC, PROP_HIRAGANA, PROP_KATAKANA, JIS_X0201_KATAKANA, JIS_KANJI_PLANE_1, JIS_KANJI_PLANE_2, ADDITIONAL_SYMBOLS, MACRO
	} CodeSet;
	static const wctstring *hiraganaTable;
	static const wctstring *katakanaTable;
	CodeSet planeG[4];
	CodeSet *planeGL;
	CodeSet *planeGR;
	const char *buf;
	size_t bufIndex;
	size_t bufLength;
	int convertInternal(char *&utf8str, size_t &utf8length);
	void setCodeSet(int planeIndex, char b, size_t byteLength);
	size_t getByteLengthOfCodeSet(CodeSet codeSet);
	CodeSet byteToCodeSet(char b);
	wctstring getChar1Byte(CodeSet codeSet, char b);
	wctstring getChar2Byte(CodeSet codeSet, char b, char b2);
public:
	ARIBStringConverter(const char *ptr, size_t length);
	~ARIBStringConverter();
	static int convert(const char *ptr, size_t length, char *&utf8str, size_t &utf8length);
	static size_t umb2uwc(wchar_t *wcs, const char *mbs, size_t sz);
	static size_t uwc2umb(char *mbs, const wchar_t *wcs, size_t sz);
};

#endif /* ARIBSTRINGCONVERTER_H_ */
