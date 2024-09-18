
#include <string>
#include <sstream>
#include <fstream>
#include "LangDefs.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filereadstream.h"

#ifndef VIDEO_SUB
#define VIDEO_SUB

struct subText
{
	std::string text;
	float begin;
	float end;
};
#endif

namespace AssetsStringTable
{
	
	/**
    * Read strings table in tabbed CSV format, then initialize KText and KTrueText with the string table's encoding
    *
    * \param lpszFileName name of file containing the game's strings
    */
 //   void readAssetsStringTable (const char *lpszFileName);
   void readAssetsStringTable ();

	/** Free resources used by the strings table */
    void freeAssetsStringTable ();

	void ReadBNRtextSizes();

 	int GetLineSpacing(std::string key);
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

	subText getSub(std::string r);
}
