//===========================================================
// STRING TABLE
//===========================================================

#ifndef STRING_TABLE_DEF
#define STRING_TABLE_DEF
#include <string>
#include <sstream>
#include <fstream>
#include "LangDefs.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filereadstream.h"

#define STRING_TABLE true


namespace StringTable
{
	
    void SetMyLangauge(int lang);

    std::string GetMyLangauge();

    int GetMyLangaugeID();

    bool IsMyLangaugeLatin();

	bool IsLanguageMultiByte();

	/**
    * Read strings table in tabbed CSV format, then initialize KText and KTrueText with the string table's encoding
    *
    * \param lpszFileName name of file containing the game's strings
    */
 //   void readStringTable (const char *lpszFileName);
   void readStringTable ();

	/** Free resources used by the strings table */
    void freeStringTable ();

	void ReadBNRtextSizes();

 	int GetLineWidth(std::string key);
	int GetNoOfLines(std::string key);
	int GetFont(std::string key);
	bool IsKeyFound(std::string key);
	 /**
    * Get string from the string table, looking it up by ID. The strings table must be read first
    *
    * \param lpszStringId string ID (using ansi encoding)
    *
    * \return string value, using the string table's encoding
    */
    const char *getString (const char *lpszStringId);
    std::string getStr (const char *lpszStringId);
    std::string getStr (std::string lpszStringId);
}
#endif
