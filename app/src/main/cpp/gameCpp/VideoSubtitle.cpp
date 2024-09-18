#include "BitmapText.h"
#include "Fonts.h"
#include "Application.h"
#include "VideoSubtitle.h"
#include "Resources.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filereadstream.h"
namespace VideoSubtitle
{
	/** Encoding of all strings in the string table */
	long _nTextEncoding = K_ANSI;

	/** Number of loaded identifier/value pairs in the string table */
	long _nStrings = 0;

	/** Loaded string identifiers, using ansi encoding */
	 //  const char **_lpszStringId = NULL;

	/** Loaded string values, using the encoding of the string table file */
   //    const char **_lpszStringVal = NULL;

	/** Buffer containing all loaded identifiers and values */
	char* _szStringIdBuffer = NULL;

	/** Buffer containing all loaded identifiers and values */
	char* _szStringValueBuffer = NULL;

	long _nMinAllocatedStringId = 0;
	char** _lpszStringId = NULL;
	char** _lpszStringVal = NULL;
	bool* _bStringValAllocated = NULL;
	int myLang = StringTable::GetMyLangaugeID();


	std::string langPrefix = "EN";
	std::string smallLangPrefix = "en";

	void ReadSubtitle()
	{
		/* Load string table file */
		std::string txtResFileStr = "BNR\\SUBTITLES\\" + langPrefix + ".TXT";

		long nTextEncoding = K_UTF8;
		if (myLang == LN_BRAZILIAN_PORTUGUESE)// || myLang == LN_GERMAN)
			nTextEncoding = K_ANSI;
		unsigned char* lpStrBuffer;
		char* lpCurStringIdBuffer, * lpCurStringValBuffer;
		size_t nStrBufSize, nDestIdBytes, nDestValueBytes;
		long nCurStringNum, nMaxIndex, nCurStrIndex;
		const char* lpszEncoding;
		bool bSuccess;

		/* Load string table file */
		//std::string txtResFileStr = "BNR\\ASSETSTEXT_" + StringTable::GetMyLangauge() + ".gtr";

		bSuccess = KResource::loadResource(FileSystem::charPath(txtResFileStr), lpStrBuffer, nStrBufSize);


		if (!bSuccess) {
			/* Failed to load */
			char szErrMessage[1024];
#ifdef _DEBUG
			KStringFmt(szErrMessage, 1024, "Error opening file : %s", txtResFileStr.c_str());
#else
			KStringFmt(szErrMessage, 1024, "Error 0x45678 opening Text resources, Possibliy the data files are courrupted, please reinstall the game. Thank you", txtResFileStr.c_str());
#endif
#ifdef _CEBUILD
			KMiscTools::alertBox("Darkarta: A Broken Heart's Quest Collector's Edition", szErrMessage, "Quit", NULL);
#else
			KMiscTools::alertBox("Darkarta: A Broken Heart's Quest", szErrMessage, "Quit", NULL);
#endif

			/* Exit game */
			exit(0);
		}
		nMaxIndex = (long)nStrBufSize;

		/* Free existing string table, if any */
		freeSubtitleTable();

		/* Parse contents and split into id/value pairs */
	 /* Sac: Now a days UTF8 encoded files dont have BOM added to it as per updated Unicode standards,
		   // because of this, the below function will return ANSI even if it is UTF8 standards,
		   // so I'm commenting it and forcing the code to treat the incoming text as UTF8
		bSuccess = KTextFace::detectEncoding ((const char *) lpStrBuffer, (long) nStrBufSize, nTextEncoding);
		if (!bSuccess) nTextEncoding = K_ANSI;
	 */

		if (nTextEncoding == K_UTF16_SWAPPED) {
			/* Widechar in the opposite endianness to the system's; flip the rest of the file */
			nCurStrIndex = 2;
			while (nCurStrIndex < nMaxIndex) {
				unsigned char c = lpStrBuffer[nCurStrIndex];

				lpStrBuffer[nCurStrIndex] = lpStrBuffer[nCurStrIndex + 1];
				lpStrBuffer[nCurStrIndex + 1] = c;
				nCurStrIndex += 2;
			}

			nTextEncoding = K_UTF16;
		}

		if (nTextEncoding == K_UTF16)
			nMaxIndex /= 2;

		/* First, count all strings and measure the size required to store them */

		nCurStrIndex = 0;
		if (nTextEncoding == K_UTF8 || nTextEncoding == K_UTF16) {
			/* Skip Byte Order Mark */
			KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
		}

		_nStrings = 0;
		_nMinAllocatedStringId = 0;
		_nTextEncoding = nTextEncoding;
		nDestIdBytes = 0;
		nDestValueBytes = 0;

		while (nCurStrIndex < nMaxIndex) {
			unsigned long c;
			long nPrevIndex;
			c = 0;
			/* Skip leading tabs and spaces */
			if (nCurStrIndex < nMaxIndex) {
				do {
					nPrevIndex = nCurStrIndex;
					c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
					if (c != 9 && c != 32) nCurStrIndex = nPrevIndex;
				} while (nCurStrIndex < nMaxIndex && (c == 9 || c == 32));
			}

			/* Check for comment or empty line */
			if (nCurStrIndex < nMaxIndex) {
				nPrevIndex = nCurStrIndex;
				c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nPrevIndex, true);
			}

			if (c == '#' || c == 10 || c == 13) {
				/* It is a comment or an empty line */
				if (c == '#') {
					/* Skip the whole line */
					do {
						nPrevIndex = nCurStrIndex;
						c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
						if (c == 10 || c == 13) nCurStrIndex = nPrevIndex;
					} while (nCurStrIndex < nMaxIndex && (c != 10 && c != 13));
				}

				/* Skip over carriage returns inbetween values */

				if (nCurStrIndex < nMaxIndex) {
					do {
						nPrevIndex = nCurStrIndex;
						c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
						if (c != 10 && c != 13)
							nCurStrIndex = nPrevIndex;
					} while (nCurStrIndex < nMaxIndex && (c == 10 || c == 13));
				}
			}
			else {
				/* Skip over ID */
				do {
					nPrevIndex = nCurStrIndex;

					c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
					if (nCurStrIndex < nMaxIndex && c != 9 && c != 32 && c != 10 && c != 13)
						nDestIdBytes++;
				} while (nCurStrIndex < nMaxIndex && c != 9 && c != 32 && c != 10 && c != 13);

				/* Add room for the ID's terminal 0 */
				nDestIdBytes++;

				/* Skip separation tabs and spaces */
				if (nCurStrIndex < nMaxIndex && (c == 9 || c == 32)) {
					do {
						nPrevIndex = nCurStrIndex;
						c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
						if (c != 9 && c != 32) nCurStrIndex = nPrevIndex;
					} while (nCurStrIndex < nMaxIndex && (c == 9 || c == 32));
				}

				if (nCurStrIndex < nMaxIndex) {
					/* String has a value */
					nPrevIndex = nCurStrIndex;
					c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
					if (c == '"') {
						bool bMultilineDone = false;

						/* Multiline value */
						while (nCurStrIndex < nMaxIndex && !bMultilineDone) {
							nPrevIndex = nCurStrIndex;
							c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
							if (c == '"') {
								nPrevIndex = nCurStrIndex;
								if (nCurStrIndex < nMaxIndex)
									c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
								else
									c = 0;

								if (c == '"') {
									/* Double quote - treat as simple quote */
									nDestValueBytes += (nCurStrIndex - nPrevIndex);
								}
								else {
									/* Simple quote - multiline value ends */
									nCurStrIndex = nPrevIndex;
									bMultilineDone = true;
								}
							}
							else if (c != 0x0d)
								nDestValueBytes += (nCurStrIndex - nPrevIndex);
						}
					}
					else {
						nCurStrIndex = nPrevIndex;

						do {
							nPrevIndex = nCurStrIndex;
							c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
							if (c == 10 || c == 13)
								nCurStrIndex = nPrevIndex;
							else
								nDestValueBytes += (nCurStrIndex - nPrevIndex);
						} while (nCurStrIndex < nMaxIndex && c != 10 && c != 13);
					}

					/* Skip over carriage returns inbetween values */

					if (nCurStrIndex < nMaxIndex) {
						do {
							nPrevIndex = nCurStrIndex;
							c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
							if (c != 10 && c != 13)
								nCurStrIndex = nPrevIndex;
						} while (nCurStrIndex < nMaxIndex && (c == 10 || c == 13));
					}
				}

				/* Add room for the value's terminal 0 */
				nDestValueBytes++;

				_nStrings++;
			}
		}
		if (nTextEncoding == K_UTF16)
			nDestValueBytes *= 2;

		_nMinAllocatedStringId = _nStrings;

		/* Now, read the string IDs and values */

		nCurStrIndex = 0;
		if (nTextEncoding == K_UTF8 || nTextEncoding == K_UTF16) {
			/* Skip Byte Order Mark */
			KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
		}

		_lpszStringId = new char* [_nStrings];
		_lpszStringVal = new char* [_nStrings];
		_bStringValAllocated = new bool[_nStrings];
		_szStringIdBuffer = new char[nDestIdBytes];
		_szStringValueBuffer = new char[nDestValueBytes];

		nCurStringNum = 0;
		lpCurStringIdBuffer = _szStringIdBuffer;
		lpCurStringValBuffer = _szStringValueBuffer;

		while (nCurStrIndex < nMaxIndex) {
			unsigned long c = 0;
			long nPrevIndex, nDstIndex;

			_lpszStringId[nCurStringNum] = lpCurStringIdBuffer;

			/* Skip leading tabs and spaces */
			if (nCurStrIndex < nMaxIndex) {
				do {
					nPrevIndex = nCurStrIndex;
					c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
					if (c != 9 && c != 32) nCurStrIndex = nPrevIndex;
				} while (nCurStrIndex < nMaxIndex && (c == 9 || c == 32));
			}

			/* Check for comment or empty line */
			if (nCurStrIndex < nMaxIndex) {
				nPrevIndex = nCurStrIndex;
				c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nPrevIndex, true);
			}

			if (c == '#' || c == 10 || c == 13) {
				/* It is a comment or an empty line */
				if (c == '#') {
					/* Skip the whole line */
					do {
						nPrevIndex = nCurStrIndex;
						c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
						if (c == 10 || c == 13) nCurStrIndex = nPrevIndex;
					} while (nCurStrIndex < nMaxIndex && (c != 10 && c != 13));
				}

				/* Skip over carriage returns inbetween values */

				if (nCurStrIndex < nMaxIndex) {
					do {
						nPrevIndex = nCurStrIndex;
						c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
						if (c != 10 && c != 13)
							nCurStrIndex = nPrevIndex;
					} while (nCurStrIndex < nMaxIndex && (c == 10 || c == 13));
				}
			}
			else {
				/* Read string ID and convert to ansi, regardless of file encoding */
				do {
					nPrevIndex = nCurStrIndex;

					c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
					if (nCurStrIndex < nMaxIndex && c != 9 && c != 32 && c != 10 && c != 13)
						*lpCurStringIdBuffer++ = (char)((c > 255) ? '?' : c);
				} while (nCurStrIndex < nMaxIndex && c != 9 && c != 32 && c != 10 && c != 13);
				*lpCurStringIdBuffer++ = 0;

				/* Skip separation tabs and spaces */
				if (nCurStrIndex < nMaxIndex && (c == 9 || c == 32)) {
					do {
						nPrevIndex = nCurStrIndex;
						c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
						if (c != 9 && c != 32) nCurStrIndex = nPrevIndex;
					} while (nCurStrIndex < nMaxIndex && (c == 9 || c == 32));
				}

				_lpszStringVal[nCurStringNum] = lpCurStringValBuffer;
				_bStringValAllocated[nCurStringNum] = false;
				nDstIndex = 0;

				if (nCurStrIndex < nMaxIndex) {
					/* String has a value, read it and keep it in its encoding */
					nPrevIndex = nCurStrIndex;
					c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
					if (c == '"') {
						bool bMultilineDone = false;

						/* Read multiline value */
						while (nCurStrIndex < nMaxIndex && !bMultilineDone) {
							nPrevIndex = nCurStrIndex;
							c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
							if (c == '"') {
								nPrevIndex = nCurStrIndex;
								if (nCurStrIndex < nMaxIndex)
									c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
								else
									c = 0;

								if (c == '"') {
									/* Double quote - treat as simple quote */
									KTextFace::encodeChar(nTextEncoding, lpCurStringValBuffer, nDstIndex, c);
								}
								else {
									/* Simple quote - multiline value ends */
									nCurStrIndex = nPrevIndex;
									bMultilineDone = true;
								}
							}
							else if (c != 0x0d) {
								KTextFace::encodeChar(nTextEncoding, lpCurStringValBuffer, nDstIndex, c);
							}
						}
					}
					else {
						nCurStrIndex = nPrevIndex;

						/* Read single-line value */
						do {
							nPrevIndex = nCurStrIndex;
							c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
							if (c == 10 || c == 13)
								nCurStrIndex = nPrevIndex;
							else {
								KTextFace::encodeChar(nTextEncoding, lpCurStringValBuffer, nDstIndex, c);
							}
						} while (nCurStrIndex < nMaxIndex && c != 10 && c != 13);
					}

					/* Skip over carriage returns inbetween values */

					if (nCurStrIndex < nMaxIndex) {
						do {
							nPrevIndex = nCurStrIndex;
							c = KTextFace::decodeChar(nTextEncoding, (const char*)lpStrBuffer, nCurStrIndex, true);
							if (c != 10 && c != 13)
								nCurStrIndex = nPrevIndex;
						} while (nCurStrIndex < nMaxIndex && (c == 10 || c == 13));
					}
				}
				KTextFace::encodeChar(nTextEncoding, lpCurStringValBuffer, nDstIndex, 0);

				if (nTextEncoding == K_UTF16)
					lpCurStringValBuffer += (nDstIndex * 2);
				else
					lpCurStringValBuffer += nDstIndex;

				nCurStringNum++;
			}
		}

		delete[] lpStrBuffer;
		lpStrBuffer = NULL;

		/* Log string table information */
		lpszEncoding = "ansi";
		if (nTextEncoding == K_UTF8)
			lpszEncoding = "utf8";
		if (nTextEncoding == K_UTF16)
			lpszEncoding = "utf16";
		K_LOG("Game: strings table read, %d entries, %s encoding", _nStrings, lpszEncoding);
	}

	
	/**
	 * Get string from the string table, looking it up by ID. The strings table must be read first
	 *
	 * \param lpszStringId string ID (using ansi encoding)
	 *
	 * \return string value, using the string table's encoding
	 */

	subText getSub(const char* lpszStringId) {
		long i;

		/* Look for ID in strings table */

		for (i = 0; i < _nStrings; i++) {
			if (strcmp(_lpszStringId[i], lpszStringId))
			{
				//      K_LOG ("Game: string for ID '%s' is '%s'",lpszStringId, _lpszStringVal[i]);
				std::string _begin;
				std::string _end;
				std::string _text;

				std::string r(_lpszStringVal[i]);

				bool beginCur = false;
				bool endCur = false;
				for (int j = 0; j < r.length(); j++)
				{
					if (r[j] == '$')
					{
						continue;
					}		
					if (r[j] == '-')
					{
						beginCur = true;
						continue;
					}
					if (r[j] == '*')
					{
						endCur = true;
						continue;
					}
					if (!beginCur)
					{
						_begin += r[j];
						continue;
					}
					if (!endCur)
					{
						_end += r[j];
						continue;
					}
					_text += r[j];
					
				}
				//std::string result = "RESULT:"+ _begin + _end + _text;
				//K_LOG(result.c_str());

				subText result = { _text, std::atof(_begin.c_str()), std::atof(_end.c_str()) };
				return result;
				//return _lpszStringVal[i];
			}
		}

		/* Not found */
		K_LOG("get-string: unknown string ID '%s'", lpszStringId);
		subText result = {"", -1, -1};
		return result;//return NULL; - to avoid crashes!!
	}



	void freeSubtitleTable() {
		long i;

		if (_szStringValueBuffer) {
			delete[] _szStringValueBuffer;
			_szStringValueBuffer = NULL;
		}

		if (_szStringIdBuffer) {
			delete[] _szStringIdBuffer;
			_szStringIdBuffer = NULL;
		}

		if (_bStringValAllocated) {
			for (i = 0; i < _nStrings; i++) {
				if (_bStringValAllocated[i]) {
					if (_lpszStringVal[i]) {
						delete[] _lpszStringVal[i];
						_lpszStringVal[i] = NULL;
					}
					_bStringValAllocated[i] = false;
				}
			}
			delete[] _bStringValAllocated;
			_bStringValAllocated = NULL;
		}

		for (i = _nMinAllocatedStringId; i < _nStrings; i++) {
			if (_lpszStringId[i]) {
				delete[] _lpszStringId[i];
				_lpszStringId[i] = NULL;
			}
		}

		if (_lpszStringVal) {
			delete[] _lpszStringVal;
			_lpszStringVal = NULL;
		}

		if (_lpszStringId) {
			delete[] _lpszStringId;
			_lpszStringId = NULL;
		}

		_nMinAllocatedStringId = 0;
		_nStrings = 0;
	}
}