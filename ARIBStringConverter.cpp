/*
 * ARIBStringConverter.cpp
 *
 *  Created on: 2008/09/06
 *      Author: linux_ts2epg
 */

#include "ARIBStringConverter.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <clocale>

const wctstring *ARIBStringConverter::hiraganaTable(new wctstring(L"ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん　　　ゝゞー。「」、・"));
const wctstring *ARIBStringConverter::katakanaTable(new wctstring(L"ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶヽヾー。「」、・"));

ARIBStringConverter::ARIBStringConverter(const char *ptr, size_t length) :
	planeGL(&planeG[0]), planeGR(&planeG[2]), buf(ptr), bufIndex(0), bufLength(
			length) {
	planeG[0] = KANJI;
	planeG[1] = ALPHANUMERIC;
	planeG[2] = HIRAGANA;
	planeG[3] = KATAKANA;
}

ARIBStringConverter::~ARIBStringConverter() {
}

int ARIBStringConverter::convert(const char *str, size_t length,
		char *&utf8str, size_t &utf8length) {
	ARIBStringConverter converter(str, length);
	return converter.convertInternal(utf8str, utf8length);
}

int ARIBStringConverter::convertInternal(char *&utf8str, size_t &utf8length) {
	wctstring resultString;
	char utf8buf[1024] = {0};
	size_t utf8buflen = 0;
	while (bufIndex < bufLength) {
		char b = buf[bufIndex];
		//		printf("%3d: %02x", bufIndex, (unsigned char) b);
		if (b == 0x1b) {
			// エスケープシーケンス

			if (bufLength - bufIndex < 2) {
				// 途中でエスケープシーケンスが切れている
				break;
			}

			b = buf[++bufIndex]; // 次のバイトを見る

			if (b >= 0x28 && b <= 0x2b) {
				// 1バイトGセットをG0～G3にセット
				if (bufLength - bufIndex < 2) {
					// 途中でエスケープシーケンスが切れている
					break;
				}
				int planeIndex = b - 0x28;
				b = buf[++bufIndex]; // 次のバイトを見る
				setCodeSet(planeIndex, b, 1);
			} else if (b == 0x24) {
				// 2バイトGセットをG0～G3にセット
				if (bufLength - bufIndex < 2) {
					// 途中でエスケープシーケンスが切れている
					break;
				}
				b = buf[++bufIndex]; // 次のバイトを見る
				if (b >= 0x29 && b <= 0x2b) {
					// G1～G3にセット
					if (bufLength - bufIndex < 2) {
						// 途中でエスケープシーケンスが切れている
						break;
					}
					int planeIndex = b - 0x28;
					b = buf[++bufIndex]; // 次のバイトを見る
					setCodeSet(planeIndex, b, 2);
				} else {
					// G0にセット
					setCodeSet(0, b, 2);
				}
			} else {
				// LS2, LS3, LS1R, LS2R, LS3R
				switch (b) {
				case 0x6e:
					planeGL = &planeG[2];
					//					System.out.println("GL = G2(" + planeG[2] + ")");
					break;
				case 0x6f:
					planeGL = &planeG[3];
					//					System.out.println("GL = G3(" + planeG[3] + ")");
					break;
				case 0x7e:
					planeGR = &planeG[1];
					//					System.out.println("GR = G1(" + planeG[1] + ")");
					break;
				case 0x7d:
					planeGR = &planeG[2];
					//					System.out.println("GR = G2(" + planeG[2] + ")");
					break;
				case 0x7c:
					planeGR = &planeG[3];
					//					System.out.println("GR = G3(" + planeG[3] + ")");
					break;
				}
			}
		} else {
			// エスケープシーケンスではない
			// TODO: シングルシフト実装
			if (b == 0x0f) {
				// LS0
				planeGL = &planeG[0];
				//				System.out.println("GL = G0(" + planeG[0] + ")");
			} else if (b == 0x0e) {
				// LS1
				planeGL = &planeG[1];
				//				System.out.println("GL = G1(" + planeG[1] + ")");
			} else {
				// 多分普通の文字列
				if ((unsigned char) b >= 0x20 && (unsigned char) b <= 0x7f) {
					// GL
					if (getByteLengthOfCodeSet(*planeGL) == 1) {
						// 1バイト
						//						System.out.println(getChar1Byte(planeG[planeGLnum],
						//								b));
						resultString.append(getChar1Byte(*planeGL, b));
					} else {
						// 2バイト
						if (bufLength - bufIndex < 2) {
							// 途中で文字が切れている
							break;
						}
						char b2 = buf[++bufIndex];
						//						System.out.println(getChar2Byte(planeG[planeGLnum],
						//								b, b2));
						resultString.append(getChar2Byte(*planeGL, b, b2));
					}
				} else if ((unsigned char) b >= (unsigned char) 0xa0
						&& (unsigned char) b <= (unsigned char) 0xff) {
					// GR
					if (getByteLengthOfCodeSet(*planeGR) == 1) {
						// 1バイト
						//						System.out.println(getChar1Byte(planeG[planeGRnum],
						//								(byte) (b & 0x7f)));
						resultString.append(getChar1Byte(*planeGR, (b & 0x7f)));
					} else {
						// 2バイト
						if (bufLength - bufIndex < 2) {
							// 途中で文字が切れている
							break;
						}
						char b2 = buf[++bufIndex];
						if ((unsigned char) b2 <= (unsigned char) 0x7f) {
							// GRの2バイトなのに2バイト目がGL
							break;
						}
						//						System.out.println(getChar2Byte(planeG[planeGRnum],
						//								(byte) (b & 0x7f), (byte) (b2 & 0x7f)));
						resultString.append(getChar2Byte(*planeGR, (b & 0x7f),
								(b2 & 0x7f)));
					}
				}
			}
		}
		bufIndex++;
	}

	const wchar_t *resultStringBuf = resultString.c_str();

	uwc2umb(utf8buf, resultStringBuf, 256);
	utf8buflen = strlen(utf8buf);
//	printf("%s\n", utf8buf);
	utf8str = new char[utf8buflen + 1];
	memset(utf8str, 0, utf8buflen);
	strcpy(utf8str, utf8buf);
utf8length = utf8buflen;

	return 0;
}

//	iconv(id, &str, &length, &putf8buf, &utf8buflen);
void ARIBStringConverter::setCodeSet(int planeIndex, char b, size_t byteLength) {
	CodeSet codeSetTo = byteToCodeSet(b);

	// コードセットのバイト系が正しいかチェック
	if (byteLength != getByteLengthOfCodeSet(codeSetTo)) {
		return;
	}
	planeG[planeIndex] = codeSetTo;
	//	printf("plane G%d is set to %s\n", planeIndex, codeSetTo);
}

size_t ARIBStringConverter::getByteLengthOfCodeSet(CodeSet codeSet) {
	switch (codeSet) {
	case KANJI:
	case JIS_KANJI_PLANE_1:
	case JIS_KANJI_PLANE_2:
	case ADDITIONAL_SYMBOLS:
		return 2;
	default:
		return 1;
	}
}

ARIBStringConverter::CodeSet ARIBStringConverter::byteToCodeSet(char b) {
	CodeSet result = UNKNOWN;
	switch (b) {
	case 0x42:
		result = KANJI;
		break;
	case 0x4a:
		result = ALPHANUMERIC;
		break;
	case 0x30:
		result = HIRAGANA;
		break;
	case 0x31:
		result = KATAKANA;
		break;
	case 0x32:
		result = MOSAIC_A;
		break;
	case 0x33:
		result = MOSAIC_B;
		break;
	case 0x34:
		result = MOSAIC_C;
		break;
	case 0x35:
		result = MOSAIC_D;
		break;
	case 0x36:
		result = PROP_ALPHANUMERIC;
		break;
	case 0x37:
		result = PROP_HIRAGANA;
		break;
	case 0x38:
		result = PROP_KATAKANA;
		break;
	case 0x49:
		result = JIS_X0201_KATAKANA;
		break;
	case 0x39:
		result = JIS_KANJI_PLANE_1;
		break;
	case 0x3a:
		result = JIS_KANJI_PLANE_2;
		break;
	case 0x3b:
		result = ADDITIONAL_SYMBOLS;
		break;
	default:
		result = UNKNOWN;
		break;
	}
	return result;
}

void debugprint(wctstring &wstr) {
  char tmp[20] = {0};
  const wchar_t *cstr = wstr.c_str();
  ARIBStringConverter::uwc2umb(tmp, wstr.c_str(), 3);
  printf("%s\n",tmp);
}

wctstring ARIBStringConverter::getChar1Byte(CodeSet codeSet, char b) {
	if (b >= 0x21 && b <= 0x7e) {
		if (codeSet == HIRAGANA) {
		  wctstring substr = hiraganaTable->substr(b - 0x21, 1);
		  //		  debugprint(substr);
			return hiraganaTable->substr(b - 0x21, 1);
		} else if (codeSet == KATAKANA) {
		  wctstring substr = katakanaTable->substr(b - 0x21, 1);
		  //		  debugprint(substr);
			return katakanaTable->substr(b - 0x21, 1);
		} else if (codeSet == ALPHANUMERIC) {
		  wchar_t tmpStr[10] = {0};
			char tmpChar[2] = { b, 0x00 };
			umb2uwc(tmpStr, tmpChar, 1);
			return wctstring(tmpStr);
		}
	}
	return L"$" ;
}

wctstring ARIBStringConverter::getChar2Byte(CodeSet codeSet, char b, char b2) {
	if (codeSet == KANJI || codeSet == JIS_KANJI_PLANE_1) {
		char jisBuf[6] = { 0x1b, 0x24, 0x42, b, b2, 0x00 };
		char *pjisBuf = jisBuf;
		size_t jisBufLen = 5;
		char utf8Buf[10] = {0};
		char *putf8Buf = utf8Buf;
		size_t utf8BufLen = 10;
		iconv_t id;
		id = iconv_open("UTF-8", "ISO-2022-JP");
		int res = iconv(id, &pjisBuf, &jisBufLen, &putf8Buf,
				&utf8BufLen);
		iconv_close(id);
		if (res == 0) {
		  //		  printf("%s\n", utf8Buf);
		  wchar_t wbuf[10] = {0};
			umb2uwc(wbuf, utf8Buf, strlen(utf8Buf));
			wctstring result(wbuf);
			return result;
		} else {
			return L"■" ;
		}
	}
	return L"□";
}

size_t ARIBStringConverter::umb2uwc(wchar_t *wcs, const char *mbs, size_t sz) {
	setlocale(LC_ALL, "ja_JP.UTF-8"); /* ロケールを日本語に設定 */
	return mbstowcs(wcs, mbs, sz);
}

size_t ARIBStringConverter::uwc2umb(char *mbs, const wchar_t *wcs, size_t sz) {
	setlocale(LC_ALL, "ja_JP.UTF-8"); /* ロケールを日本語に設定 */
	return wcstombs(mbs, wcs, sz);
}
