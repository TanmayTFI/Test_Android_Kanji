//===========================================================
// 
// Framework
//===========================================================

#ifndef RESOURCE_FILE
#define RESOURCE_FILE

#include "Framework.h"

//=====================
// Path Information
//=====================
namespace FileSystem
{
	extern std::string ws2s(const std::wstring& s);
	extern std::wstring s2ws(const std::string& s);
	extern const std::string AssetPath;
	extern		 std::string strPath( std::string File );
	extern		 char* charPath( std::string File );
}

#endif
