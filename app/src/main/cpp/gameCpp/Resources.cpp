//===========================================================
// 
// 
//===========================================================
//===========================================================
//
// Information about Assets Path
// In Debug mode, we allow for the assets to be accessed
// at an indirect level, such that the application can be 
// launched from the IDE. (note: this path may be altered on 
// Mac)
//
// In Release, the assets are assumed to be located
// at the same directory level as the executable.
//
//===========================================================

#include "Resources.h"
#include "Application.h" //iPad defined
#include "LangDefs.h"
#include "iPadDefs.h"

#include "BuildConstants.h"

namespace FileSystem
{

const std::string AssetPath = "Assets/";

#if defined (K_ANDROID)
	int wctomb(char *s, wchar_t wc) { return wcrtomb(s,wc,NULL); }
	int mbtowc(wchar_t *pwc, const char *s, size_t n) { return mbrtowc(pwc, s, n, NULL); }
#endif


	std::wstring s2ws(const std::string& s)
	{
		int len;
		#ifdef K_WIN32
            int slength = (int)s.length() + 1;
            len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		#else	
			wchar_t* buf = new wchar_t[MB_CUR_MAX];
			len = mbtowc(buf, s.c_str(), MB_CUR_MAX);
		#endif
		std::wstring r(buf);
		delete[] buf;
		return r;
	}

	std::string ws2s(const std::wstring& s)
	{
		int len;
		#ifdef K_WIN32
            int slength = (int)s.length() + 1;
            len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
			char* buf = new char[len];
			WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0); 
		#else
			char* buf = new char[MB_CUR_MAX];
			len = wctomb(buf, *s.c_str());
        #endif
		std::string r(buf);
		delete[] buf;
		return r;
	}

	std::string strPath(std::string File)
	{
		return (std::string)( KMiscTools::makeFilePath( (AssetPath + File).c_str() ) );
	}

	char* charPath(std::string File)
	{
      //  char szPathMessage[1024];
      //  KStringFmt (szPathMessage, 1024, "Path for Asset: %s", KMiscTools::makeFilePath ((AssetPath + File).c_str()));
      //  KMiscTools::alertBox ("Darkarta: A Broken Heart's Quest", szPathMessage, "OK", NULL);
		return  KMiscTools::makeFilePath( (AssetPath + File).c_str() ) ;
	}
}
