#include <fstream>
#include "LangDefs.h"

#ifndef VIDEO_SUB
#define VIDEO_SUB

struct subText
{
	std::string text;
	float begin;
	float end;
};
#endif

namespace VideoSubtitle
{
	void ReadSubtitle();
	void freeSubtitleTable();

	
	/**
   * Get string from the string table, looking it up by ID. The strings table must be read first
   *
   * \param lpszStringId string ID (using ansi encoding)
   *
   * \return string value, using the string table's encoding
   */
	subText getSub(const char* lpszStringId);
}
