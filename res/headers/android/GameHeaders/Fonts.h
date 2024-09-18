//===========================================================
// 
// Framework
//===========================================================

#ifndef FONTS_H
#define FONTS_H
#include "LangDefs.h"

enum EAlignment
{
	eAlign_Center,
	eAlign_Left
};

enum EFont
{
	eFont_25,
	eFont_26,
	eFont_Italic24,//jrn hint	
	eFont_Small,
	eFont_InvPop,
	eFont_Diary,
	eFont_Drop,
 	eFont_Count
};

#ifndef JAPANESE_BUILD
	extern const char* FontImage[eFont_Count];
#endif

#endif
