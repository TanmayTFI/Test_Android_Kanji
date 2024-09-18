//====================================
// Framework
//====================================

#ifndef DQFULP_LANG_DEF
#define DQFULP_LANG_DEF

enum BuildLanguages 
{   LN_ENGLISH=0,
	LN_GERMAN=100, 
	LN_SPANISH=200,
	LN_FRENCH=300,
	LN_ITALIAN=400,
	LN_NEDERLANDS=500,
	LN_PORTUGUESE=600,
	LN_BRAZILIAN_PORTUGUESE=700,
	LN_HUNGARIAN=800,
	LN_JAPANESE=101,
	LN_SIMPLIFIED_CHINESE=201,
	LN_TRADITIONAL_CHINESE=301,
	LN_KOREAN=401,
	LN_RUSSIAN=501,
	LN_UKRAINIAN=601,
	LN_ARABIC=701,
	LN_HINDI=801
 };


#define ENGLISH_BUILD true
//#define GERMAN_BUILD true
//#define SPANISH_BUILD true
//#define FRENCH_BUILD true
//#define NEDERLANDS_BUILD true
//#define ITALIAN_BUILD true
//#define PORTUGUESE_BUILD true
//#define JAPANESE_BUILD true


#include "StringTable.h" // Sac: now using only string table for all texts

#if defined( JAPANESE_BUILD )
	#include "StringTable.h" //defines STRING_TABLE
#endif

#endif