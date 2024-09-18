//===========================================================
// STRING TABLE
//===========================================================

#include "StringTable.h"
#include "Resources.h"
#include "LangDefs.h"
#include "BuildConstants.h"
#include "KPTK.h"
#include <sstream>
#include <string>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

namespace StringTable
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
      char *_szStringIdBuffer = NULL;

   /** Buffer containing all loaded identifiers and values */
      char *_szStringValueBuffer = NULL;

     long _nMinAllocatedStringId =0;
     char **_lpszStringId =NULL;
     char ** _lpszStringVal=NULL;
      bool *_bStringValAllocated =NULL;
   

      int myLang=0;

      std::string langPrefix="EN";
      std::string smallLangPrefix="en";

/**
 * Read strings table in tabbed CSV format, then initialize KText and KTrueText with the string table's encoding
 *
 * \param lpszFileName name of file containing the game's strings
 */


void SetMyLangauge(int lang)
		{
			myLang=lang;
			switch (lang)
			{
				case LN_ENGLISH:
					langPrefix ="EN";
               smallLangPrefix ="en";
					break;
				case LN_GERMAN:
					langPrefix ="DE";
               smallLangPrefix ="de";
					break;
				case LN_SPANISH:
					langPrefix ="ES";
               smallLangPrefix ="es";
					break;
				case LN_FRENCH:
					langPrefix ="FR";
               smallLangPrefix ="fr";
					break;
				case LN_ITALIAN:
					langPrefix ="IT";
               smallLangPrefix ="it";
					break;
				case LN_NEDERLANDS:
					langPrefix ="ND";
               smallLangPrefix ="nd";
					break;
            case LN_HUNGARIAN:
					langPrefix ="HU";
               smallLangPrefix ="hu";
					break;
				case LN_PORTUGUESE:
					langPrefix ="PR";
               smallLangPrefix ="pr";
					break;
				case LN_BRAZILIAN_PORTUGUESE:
					langPrefix ="BR";
               smallLangPrefix ="br";
					break;
				case LN_JAPANESE:
					langPrefix ="JP";
               smallLangPrefix ="jp";
					break;
				case LN_SIMPLIFIED_CHINESE:
					langPrefix ="CS";
               smallLangPrefix ="cs";
					break;
				case LN_KOREAN:
					langPrefix ="KR";
               smallLangPrefix ="kr";
					break;
				case LN_TRADITIONAL_CHINESE:
					langPrefix ="CT";
               smallLangPrefix ="ct";
					break;
				case LN_RUSSIAN:
					langPrefix ="RU";
               smallLangPrefix ="ru";
					break;
				case LN_UKRAINIAN:
					langPrefix ="UR";
               smallLangPrefix ="ur";
					break;
				case LN_HINDI:
					langPrefix ="HN";
               smallLangPrefix ="hn";
					break;
				case LN_ARABIC:
					langPrefix ="AB";
               smallLangPrefix ="ab";
					break;   
				default:
					langPrefix ="EN";
               smallLangPrefix ="en";
					break;
			}

		}
bool IsMyLangaugeLatin()
{
      switch (myLang)
		{
	      	case LN_ENGLISH:
			case LN_GERMAN:
			case LN_SPANISH:
			case LN_FRENCH:
			case LN_ITALIAN:
			case LN_NEDERLANDS:
         case LN_HUNGARIAN:
			case LN_PORTUGUESE:
			case LN_BRAZILIAN_PORTUGUESE:
         case LN_RUSSIAN:
			case LN_UKRAINIAN:
            return true;  
            break;
         case LN_JAPANESE:
			case LN_SIMPLIFIED_CHINESE:
			case LN_KOREAN:
			case LN_TRADITIONAL_CHINESE:
			case LN_HINDI:
			case LN_ARABIC:
			default:
            return false;
            break;
		}
}


std::string GetMyLangauge()
		{
		    #if defined (K_ANDROID)

                #if defined (_AMAZON_BUILD)
		          return	smallLangPrefix;
                #elif defined (_UNLOCKED_FOR_TEST_BUILD)
                    return	langPrefix;
		          #else
                  return	langPrefix;
               #endif

            #else
		      return	langPrefix;
            #endif
		}

int GetMyLangaugeID()
		{
			return	myLang;
		}

bool IsLanguageMultiByte()
{
	switch (myLang)
	{	      	
		case LN_HUNGARIAN: return true;
        case LN_RUSSIAN: return true;
		case LN_UKRAINIAN: return true;
        case LN_JAPANESE: return true;
		case LN_NEDERLANDS: return false;
		case LN_SIMPLIFIED_CHINESE: return true;
		case LN_KOREAN: return true;
		case LN_TRADITIONAL_CHINESE: return true;
		case LN_HINDI: return true;
		case LN_ARABIC: return true;
		default: return false;
	}
}


//void readStringTable (const char *lpszFileName) 
void readStringTable () {
   long nTextEncoding=K_UTF8;
   //if(myLang == LN_BRAZILIAN_PORTUGUESE)// || myLang == LN_GERMAN)
	 //  nTextEncoding = K_ANSI;
   unsigned char *lpStrBuffer;
   char *lpCurStringIdBuffer, *lpCurStringValBuffer;
   size_t nStrBufSize, nDestIdBytes, nDestValueBytes;
   long nCurStringNum, nMaxIndex, nCurStrIndex;
   const char *lpszEncoding;
   bool bSuccess;

   /* Load string table file */
   std::string txtResFileStr = "BNR\\GAMETEXTV2_" + langPrefix + ".gtr";

 
   bSuccess = KResource::loadResource (FileSystem::charPath(txtResFileStr), lpStrBuffer, nStrBufSize);


   if (!bSuccess) {
      /* Failed to load */
      char szErrMessage[1024];
#ifdef _DEBUG
	  KStringFmt (szErrMessage, 1024, "Error opening file : %s", txtResFileStr.c_str());
#else
   KStringFmt (szErrMessage, 1024, "Error 0x45678 opening Text resources, Possibliy the data files are courrupted, please reinstall the game. Thank you", txtResFileStr.c_str());
#endif
#ifdef _CEBUILD
			KMiscTools::alertBox ("Darkarta: A Broken Heart's Quest Collector's Edition", szErrMessage, "Quit", NULL);
#else
			KMiscTools::alertBox ("Darkarta: A Broken Heart's Quest", szErrMessage, "Quit", NULL);
#endif
	  
	  /* Exit game */
	  exit (0);
   }
   nMaxIndex = (long) nStrBufSize;

   /* Free existing string table, if any */
   freeStringTable ();

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
      KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
   }

   _nStrings = 0;
   _nMinAllocatedStringId = 0;
   _nTextEncoding = nTextEncoding;
   nDestIdBytes = 0;
   nDestValueBytes = 0;

   while (nCurStrIndex < nMaxIndex) {
      unsigned long c;
      long nPrevIndex;
      c =0;
      /* Skip leading tabs and spaces */
      if (nCurStrIndex < nMaxIndex) {
         do {
            nPrevIndex = nCurStrIndex;
            c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
            if (c != 9 && c != 32) nCurStrIndex = nPrevIndex;
         } while (nCurStrIndex < nMaxIndex && (c == 9 || c == 32));
      }

      /* Check for comment or empty line */
      if (nCurStrIndex < nMaxIndex) {
         nPrevIndex = nCurStrIndex;
         c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nPrevIndex, true);
      }

      if (c == '#' || c == 10 || c == 13) {
         /* It is a comment or an empty line */
         if (c == '#') {
            /* Skip the whole line */
            do {
               nPrevIndex = nCurStrIndex;
               c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
               if (c == 10 || c == 13) nCurStrIndex = nPrevIndex;
            } while (nCurStrIndex < nMaxIndex && (c != 10 && c != 13));
         }

         /* Skip over carriage returns inbetween values */

         if (nCurStrIndex < nMaxIndex) {
            do {
               nPrevIndex = nCurStrIndex;
               c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
               if (c != 10 && c != 13)
                  nCurStrIndex = nPrevIndex;
            } while (nCurStrIndex < nMaxIndex && (c == 10 || c == 13));
         }
      }
      else {
         /* Skip over ID */
         do {
            nPrevIndex = nCurStrIndex;

            c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
            if (nCurStrIndex < nMaxIndex && c != 9 && c != 32 && c != 10 && c != 13)
               nDestIdBytes ++;
         } while (nCurStrIndex < nMaxIndex && c != 9 && c != 32 && c != 10 && c != 13);

         /* Add room for the ID's terminal 0 */
         nDestIdBytes ++;

         /* Skip separation tabs and spaces */
         if (nCurStrIndex < nMaxIndex && (c == 9 || c == 32)) {
            do {
               nPrevIndex = nCurStrIndex;
               c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
               if (c != 9 && c != 32) nCurStrIndex = nPrevIndex;
            } while (nCurStrIndex < nMaxIndex && (c == 9 || c == 32));
         }

         if (nCurStrIndex < nMaxIndex) {
            /* String has a value */
            nPrevIndex = nCurStrIndex;
            c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
            if (c == '"') {
               bool bMultilineDone = false;

               /* Multiline value */
               while (nCurStrIndex < nMaxIndex && !bMultilineDone) {
                  nPrevIndex = nCurStrIndex;
                  c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
                  if (c == '"') {
                     nPrevIndex = nCurStrIndex;
                     if (nCurStrIndex < nMaxIndex)
                        c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
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
                  c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
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
                  c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
                  if (c != 10 && c != 13)
                     nCurStrIndex = nPrevIndex;
               } while (nCurStrIndex < nMaxIndex && (c == 10 || c == 13));
            }
         }

         /* Add room for the value's terminal 0 */
         nDestValueBytes ++;

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
      KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
   }

   _lpszStringId = new char * [_nStrings];
   _lpszStringVal = new char * [_nStrings];
   _bStringValAllocated = new bool [_nStrings];
   _szStringIdBuffer = new char [nDestIdBytes];
   _szStringValueBuffer = new char [nDestValueBytes];

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
            c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
            if (c != 9 && c != 32) nCurStrIndex = nPrevIndex;
         } while (nCurStrIndex < nMaxIndex && (c == 9 || c == 32));
      }

      /* Check for comment or empty line */
      if (nCurStrIndex < nMaxIndex) {
         nPrevIndex = nCurStrIndex;
         c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nPrevIndex, true);
      }

      if (c == '#' || c == 10 || c == 13) {
         /* It is a comment or an empty line */
         if (c == '#') {
            /* Skip the whole line */
            do {
               nPrevIndex = nCurStrIndex;
               c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
               if (c == 10 || c == 13) nCurStrIndex = nPrevIndex;
            } while (nCurStrIndex < nMaxIndex && (c != 10 && c != 13));
         }

         /* Skip over carriage returns inbetween values */

         if (nCurStrIndex < nMaxIndex) {
            do {
               nPrevIndex = nCurStrIndex;
               c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
               if (c != 10 && c != 13)
                  nCurStrIndex = nPrevIndex;
            } while (nCurStrIndex < nMaxIndex && (c == 10 || c == 13));
         }
      }
      else {
         /* Read string ID and convert to ansi, regardless of file encoding */
         do {
            nPrevIndex = nCurStrIndex;

            c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
            if (nCurStrIndex < nMaxIndex && c != 9 && c != 32 && c != 10 && c != 13)
               *lpCurStringIdBuffer++ = (char) ((c > 255) ? '?' : c);
         } while (nCurStrIndex < nMaxIndex && c != 9 && c != 32 && c != 10 && c != 13);
         *lpCurStringIdBuffer++ = 0;

         /* Skip separation tabs and spaces */
         if (nCurStrIndex < nMaxIndex && (c == 9 || c == 32)) {
            do {
               nPrevIndex = nCurStrIndex;
               c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
               if (c != 9 && c != 32) nCurStrIndex = nPrevIndex;
            } while (nCurStrIndex < nMaxIndex && (c == 9 || c == 32));
         }

         _lpszStringVal[nCurStringNum] = lpCurStringValBuffer;
         _bStringValAllocated[nCurStringNum] = false;
         nDstIndex = 0;

         if (nCurStrIndex < nMaxIndex) {
            /* String has a value, read it and keep it in its encoding */
            nPrevIndex = nCurStrIndex;
            c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
            if (c == '"') {
               bool bMultilineDone = false;

               /* Read multiline value */
               while (nCurStrIndex < nMaxIndex && !bMultilineDone) {
                  nPrevIndex = nCurStrIndex;
                  c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
                  if (c == '"') {
                     nPrevIndex = nCurStrIndex;
                     if (nCurStrIndex < nMaxIndex)
                        c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
                     else
                        c = 0;

                     if (c == '"') {
                        /* Double quote - treat as simple quote */
                        KTextFace::encodeChar (nTextEncoding, lpCurStringValBuffer, nDstIndex, c);
                     }
                     else {
                        /* Simple quote - multiline value ends */
                        nCurStrIndex = nPrevIndex;
                        bMultilineDone = true;
                     }
                  }
                  else if (c != 0x0d) {
                     KTextFace::encodeChar (nTextEncoding, lpCurStringValBuffer, nDstIndex, c);
                  }
               }
            }
            else {
               nCurStrIndex = nPrevIndex;

               /* Read single-line value */
               do {
                  nPrevIndex = nCurStrIndex;
                  c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
                  if (c == 10 || c == 13)
                     nCurStrIndex = nPrevIndex;
                  else {
                     KTextFace::encodeChar (nTextEncoding, lpCurStringValBuffer, nDstIndex, c);
                  }
               } while (nCurStrIndex < nMaxIndex && c != 10 && c != 13);
            }

            /* Skip over carriage returns inbetween values */

            if (nCurStrIndex < nMaxIndex) {
               do {
                  nPrevIndex = nCurStrIndex;
                  c = KTextFace::decodeChar (nTextEncoding, (const char *) lpStrBuffer, nCurStrIndex, true);
                  if (c != 10 && c != 13)
                     nCurStrIndex = nPrevIndex;
               } while (nCurStrIndex < nMaxIndex && (c == 10 || c == 13));
            }
         }
         KTextFace::encodeChar (nTextEncoding, lpCurStringValBuffer, nDstIndex, 0);

         if (nTextEncoding == K_UTF16)
            lpCurStringValBuffer += (nDstIndex * 2);
         else
            lpCurStringValBuffer += nDstIndex;

         nCurStringNum++;
      }
   }

   delete [] lpStrBuffer;
   lpStrBuffer = NULL;

   /* Log string table information */
   lpszEncoding = "ansi";
   if (nTextEncoding == K_UTF8)
      lpszEncoding = "utf8";
   if (nTextEncoding == K_UTF16)
      lpszEncoding = "utf16";
   K_LOG ("Game: strings table read, %d entries, %s encoding", _nStrings, lpszEncoding);

   /* Now, initialize KText and KTrueText */
   KText::initializeAsUnicode (nTextEncoding);
//#ifndef K_DISABLE_KTRUETEXT
   KTrueText::initializeAsUnicode (_nStrings, (const char **) _lpszStringVal, nTextEncoding);
//#endif
}





/**
 * Get string from the string table, looking it up by ID. The strings table must be read first
 *
 * \param lpszStringId string ID (using ansi encoding)
 *
 * \return string value, using the string table's encoding
 */

const char *getString (const char *lpszStringId) {
   long i;

   /* Look for ID in strings table */

   for (i = 0; i < _nStrings; i++) {
      if (!strcmp (_lpszStringId[i], lpszStringId))
      {
      //      K_LOG ("Game: string for ID '%s' is '%s'",lpszStringId, _lpszStringVal[i]);
         return _lpszStringVal[i];
      }
   }

   /* Not found */
   K_LOG ("get-string: unknown string ID '%s'", lpszStringId);
   return "";//return NULL; - to avoid crashes!!
}


std::string getStr (const char *lpszStringId) {
   long i;

   /* Look for ID in strings table */

   for (i = 0; i < _nStrings; i++) {
      if (!strcmp (_lpszStringId[i], lpszStringId))
      {
         //   K_LOG ("Game: string for ID '%s' is '%s'",lpszStringId, _lpszStringVal[i]);
            std::string r(_lpszStringVal[i]);
            return r;
      }

   }

   /* Not found */
   K_LOG ("get-str-char: unknown string ID '%s'", lpszStringId);
   return "";//return NULL; - to avoid crashes!!
}


std::string getStr (std::string lpszStringId) {
   long i;

   /* Look for ID in strings table */

   for (i = 0; i < _nStrings; i++) {
      if (!strcmp (_lpszStringId[i], lpszStringId.c_str()))
      {
         //   K_LOG ("Game: string for ID '%s' is '%s'",lpszStringId.c_str(), _lpszStringVal[i]);
            std::string r(_lpszStringVal[i]);
            return r;
      }

   }

   /* Not found */
   K_LOG ("get-str-string: unknown string ID '%s'", lpszStringId.c_str());
   return "";//return NULL; - to avoid crashes!!
}



/*** PRIVATE ****/

/** Free resources used by the strings table */

void freeStringTable () {
 long i;

   if (_szStringValueBuffer) {
      delete [] _szStringValueBuffer;
      _szStringValueBuffer = NULL;
   }

   if (_szStringIdBuffer) {
      delete [] _szStringIdBuffer;
      _szStringIdBuffer = NULL;
   }

   if (_bStringValAllocated) {
      for (i = 0; i < _nStrings; i++) {
         if (_bStringValAllocated[i]) {
            if (_lpszStringVal[i]) {
               delete [] _lpszStringVal[i];
               _lpszStringVal[i] = NULL;
            }
            _bStringValAllocated[i] = false;
         }
      }
      delete [] _bStringValAllocated;
      _bStringValAllocated = NULL;
   }

   for (i = _nMinAllocatedStringId; i < _nStrings; i++) {
      if (_lpszStringId[i]) {
         delete [] _lpszStringId[i];
         _lpszStringId[i] = NULL;
      }
   }

   if (_lpszStringVal) {
      delete [] _lpszStringVal;
      _lpszStringVal = NULL;
   }

   if (_lpszStringId) {
      delete [] _lpszStringId;
      _lpszStringId = NULL;
   }

   _nMinAllocatedStringId = 0;
   _nStrings = 0;
}


	
	rapidjson::Document document;
	void ReadBNRtextSizes()
	{
		std::string Filename = "BNR\\BNRTEXTSIZE_"+StringTable::GetMyLangauge()+".GTR";
        //Filename to uppercase!!
        int strLen = (int) Filename.size();  //Sac: converted Implicit to explicit

        for( int i = 0; i < strLen; i++ )
            Filename[i] = toupper(Filename[i]);

        //Converts unsigned char_array to stream
        unsigned char *lpCompressedData = NULL;
        size_t nCompressedDataSize = 0;
        if(! KResource::loadResource (FileSystem::charPath(Filename), lpCompressedData, nCompressedDataSize) )
        {
            return;
        }
        /* Copy contents and add a terminal 0 */

        char *_lpfileBytes = new char [nCompressedDataSize+1];
        memcpy (_lpfileBytes, lpCompressedData, nCompressedDataSize);//copy unsigned_chararray to const_chararray
        _lpfileBytes[nCompressedDataSize] = 0;
        std::stringstream stream(_lpfileBytes);//create stream from "const"_chararray
        delete [] _lpfileBytes;
        _lpfileBytes = NULL;
        /////////////////////////////////////////////////////////////////////

        if( stream.fail() )
        {
            return;
        }

        //converting stream data to string
        std::string jsonString = stream.str();

        const char* cstr = jsonString.c_str();
        
        document.Parse( cstr );
    
    }

	int GetLineWidth(std::string key)
	{
		int width = -1;
		SizeType numObjects = document["Data"].Size();
		for(SizeType i = 0; i < numObjects; i++)
		{
			if(document["Data"][i]["key"].GetString() == key)
			{
				width = document["Data"][i]["lineWidth"].GetInt();
				break;
			}
		}
		return width;
	}

	int GetNoOfLines(std::string key)
	{
		int lines = -1;
		SizeType numObjects = document["Data"].Size();
		for(SizeType i = 0; i < numObjects; i++)
		{
			if(document["Data"][i]["key"].GetString() == key)
			{
				lines = document["Data"][i]["noOfLines"].GetInt();
				break;
			}
		}
		return lines;
	}

	int GetFont(std::string key)
	{
		int font = 1;
		SizeType numObjects = document["Data"].Size();
		for(SizeType i = 0; i < numObjects; i++)
		{
			if(document["Data"][i]["key"].GetString() == key)
			{
				font = document["Data"][i]["font"].GetInt();
				break;
			}
		}
		return font;
	}

	bool IsKeyFound(std::string key)
	{
		bool flag = false;
        
        SizeType numObjects = document["Data"].Size();
		for(SizeType i = 0; i < numObjects; i++)
		{
			if(document["Data"][i]["key"].GetString() == key)
			{
				flag = true;
				break;
			}
		}
        
		return flag;
	}
}
