//====================================
// Framework
// Application class
//====================================

#include "Application.h"
#include "exMath.h"
#include "ProfileSystem.h"
#include "Resolution.h"
#include "RenderList.h"
#include "UpdateList.h"
#include "InteractiveObjectList.h"
#include "Console.h"
#include "GameFlow.h"
#include "Util.h"
#include "HOScene.h"
#include "BitmapText.h"
#include "ParticleEffect.h"
#include "TextureFactory.h"
#include "Fonts.h"
#include "Hud.h"
#include "SConstants.h"
#include "InventoryBox.h"
#include "Resolution.h"
#include "LangDefs.h"
#include "AwardManager.h"
#include "BuildConstants.h"
#include "KVideo.h"
#include "Award.h"
#include "MainMenuControl.h"
#include "AssetsStringTable.h"
#include "IOStream.h"
#include "MultilineTextBox.h"

#ifdef K_ANDROID
    #include <regex>
    #include "AndroidIo.h"
#endif // ANDROID


#if defined K_WIN32 || defined K_ANDROID
	#include "curl/curl.h"
#endif

#include <signal.h>

extern HudControl* GFHud;
#ifdef K_ANDROID
	#include "MainMenuControl.h"
	extern MainMenuControl* GFMainMenuCtrl;
#endif

	#ifdef K_WIN32
		extern std::wstring AppTitle_Str;
	#else
		extern std::string AppTitle_Str;
    #endif

#ifdef K_WIN32
	std::wstring DADPTR_ER_WStr;
    std::wstring GWRNG_0_WStr;
    std::wstring GWRNG_1_WStr;
	std::wstring GWRNG_2_WStr;
	std::wstring GWRNG_3_WStr;
    std::wstring GWRNG_4_WStr;
    std::wstring GWRNG_5_WStr;
    std::wstring WRNG_WStr;
    std::wstring FATAL_WStr;
    std::wstring LWMMRY_WStr;
#else
	std::string DADPTR_ER_WStr;
    std::string GWRNG_0_WStr;
    std::string GWRNG_1_WStr;
	std::string GWRNG_2_WStr;
	std::string GWRNG_3_WStr;
    std::string GWRNG_4_WStr;
    std::string GWRNG_5_WStr;
    std::string WRNG_WStr;
    std::string FATAL_WStr;
    std::string LWMMRY_WStr;
#endif

#ifdef K_ANDROID
	bool config_debugmode, config_splashskip, config_surveybuild;
	std::string App_f_o_f; //free or full version text will come from Android flavours	
	int config_lang_id, gameversionCode, gameobbversionCode, config_debugres;
	int fOf=123;
	std::string gdeviceName, path_toExternalPicturesDirectory, gameversionName;
	std::string surveyurl, reviewurl;
#endif

#ifdef K_iOS
    int app_video_res=480;
#else
    int app_video_res=1024;
#endif

Application* Application::Instance = NULL;
bool Application::SingleUpdate = false;
SResolution Application::CurrentResolution;

/** Buffer for composing error messages */
char Application::g_szErrMessage[1024];

/** Buffer for sending crash reports by e-mail */
char Application::g_szEnquotedErrMessage[1024];

////////////////////////////////////////////////////////////////////////////////////////////
// !***********************************
// \brief		The initial charater string that the TTF Fonts are initialized to Render.
// \details 	<b>Description:</b> We need to find someway to dynamically load this string 
//				from a file to override this default definition.  The TTF Fonts are initialized
//				to render the characters in this string.  It's important that the characters match
//				the Game Render Texts.

/**
 * Enquote crash report so as to put it in the body of an e-mail
 *
 * \param lpszMessage source report
 * \param lpEnquoteBuf where the enquoted message will be stored
 * \param nEnquoteBufSize maximum size of the enquoted message, including terminal 0
 */

void Application::enquoteMessage (const char *lpszMessage, char *lpEnquoteBuf, size_t nEnquoteBufSize) {
	const char *lpszCurMessage = lpszMessage;
	char *lpCurEnquoteBuf = lpEnquoteBuf;
   char *lpMaxEnquoteBuf;

   if (!nEnquoteBufSize) return;
   lpMaxEnquoteBuf = lpEnquoteBuf + nEnquoteBufSize - 1;

	while (*lpszCurMessage) {
		unsigned char c = *lpszCurMessage++;

		if (c >= 32 && c <= 127) {
         /* Visible ASCII character - store directly */
         if (lpCurEnquoteBuf + 3 < lpMaxEnquoteBuf)
			   *lpCurEnquoteBuf++ = c;
      }
      else {
			char szQuoted[4];

         /* Encode as %nn */

         K_snprintf (szQuoted, 3, "%02x", c);
         szQuoted[3] = 0;

         if ((lpCurEnquoteBuf + 3) < lpMaxEnquoteBuf) {
			   *lpCurEnquoteBuf++ = '%';
			   *lpCurEnquoteBuf++ = szQuoted[0];
			   *lpCurEnquoteBuf++ = szQuoted[1];
         }
		}
	}

	*lpCurEnquoteBuf = 0;
}

/** Built-in string identifiers, NULL-terminated */
const char *g_lpszBuiltinStringId[] = {
   "BUILTIN_ERR_FILE",
   "BUILTIN_REPORT_CAPTION",
   "BUILTIN_ASSERTION",
   "BUILTIN_CRASH",
   "BUILTIN_CRASH_PARAM",
   "BUILTIN_REPORT_BODY",
   "BUILTIN_REPORT_SEND",
   "BUILTIN_REPORT_QUIT",
   "BUILTIN_REPORT_EMAIL_SUBJECT",
   "BUILTIN_REPORT_IMPROVEGAME",
   "BUILTIN_BODY_IMPROVEGAME",
   "BUILTIN_REPORT_TELLFRIEND",   
   "BUILTIN_BODY_TELLFRIEND"
};

/** Built-in string values */
const char *g_lpszBuiltinStringVal[] = {
   /* BUILTIN_ERR_FILE */
   "Error opening file: %s",

   /* BUILTIN_REPORT_CAPTION */
   "Crash report",

   /* BUILTIN_ASSERTION */
   "Assertion failed at %s:%d",

   /* BUILTIN_CRASH */
   "Exception 0x%08x (%s)",

   /* BUILTIN_CRASH_PARAM */
   "Exception data: 0x%08x",

   /* BUILTIN_REPORT_BODY */
   "OS: %s\n"
   "Renderer: %s\n\n"
   "Stack trace:",

   /* BUILTIN_REPORT_SEND */
   "Report to developer",

   /* BUILTIN_REPORT_QUIT */
   "Quit",

   /* BUILTIN_REPORT_EMAIL_SUBJECT */
   "%s crash report",

   /* "BUILTIN_REPORT_IMPROVEGAME */
   "Have suggestions to improve Darkarta?",

   /* "BUILTIN_BODY_IMPROVEGAME */
   "Was playing Darkarta game, felt it could be better and hence this mail.",

   /* BUILTIN_REPORT_TELLFRIEND */
   "Loved playing Darkarta game?",
   
   /* BUILTIN_BODY_TELLFRIEND */
   "Loved playing Darkarta game! Please check it here. http://store.steampowered.com/app/601530"

};


/**
 * Get built-in string, such as fatal error messages. Derive to implement your own messages if needed
 *
 * \param lpszStringId built-in string ID (using ansi encoding)
 *
 * \return string value, using utf-8 encoding
 */

const char *Application::getBuiltinString (const char *lpszStringId) 
{
   long i;

   /* Look for ID in built-in strings */

   for (i = 0; g_lpszBuiltinStringId[i]; i++) {
      if (!strcmp (g_lpszBuiltinStringId[i], lpszStringId))
	  {
		 
         return g_lpszBuiltinStringVal[i];
	  }
   }

   /* Not found */
   K_LOG ("Game: unknown built-in string ID '%s'", lpszStringId);
   return NULL;
}

void my_signal_handler(int signum)
{
    ::signal(signum, SIG_DFL);

	#ifdef _ENABLE_CURL
		GFApp->SendReportToDev();
	#endif
    ::raise(SIGABRT);
}

/**
 * Method called back when a crash or an assertion failure happens
 *
 * \param bIsAssertion true for an assertion failure, false for an exception
 * \param nExceptionCode for an exception, OS exception code
 * \param lpszExceptionName for an exception, textual name of exception code
 * \param lpExceptionAddr for an exception, exception address
 * \param bHasParam for an exception, true if optional exception parameter is provided
 * \param lpParam for an exception, optional parameter
 * \param lpszAssertionFileName for an assertion failure, sourcecode filename where the assertion failed
 * \param nAssertionLine for an assertion failure, line number in filename where the assertion failed
 * \param nStackDepth number of entries in stack trace
 * \param lpStackReturnAddr array of return addresses in stack trace
 * \param lpszStackModule array of module names in stack trace
 */

void Application::crashHandler (bool bIsAssertion,
                          unsigned long nExceptionCode, const char *lpszExceptionName, bool bHasParam, void *lpParam,
                          const char *lpszAssertionFileName, long nAssertionLine,
                          long nStackDepth, void *lpStackReturnAddr[], char *lpszStackModule[]) 
{
   const char *lpszRenderer;
   long i, nResult;
   static char szEntry[1024];   

   if (GFApp->Window) 
   {
      /* Stop sending messages to window */
	#ifndef K_IPHONE
		//iPhones can't have window handlers!!
		//GFApp->Window->setCallBack (NULL);
	#endif
      GFApp->Window->setPTKCallBack (NULL);
   }

   /* Compose message */

   if (bIsAssertion) {
	  KStringFmt (g_szErrMessage, 1024, GFApp->getBuiltinString("BUILTIN_ASSERTION"), lpszAssertionFileName, nAssertionLine);
      KStringCat( g_szErrMessage, "\n", 1024);
   }
   else {
	  KStringFmt (g_szErrMessage, 1024, GFApp->getBuiltinString("BUILTIN_CRASH"), nExceptionCode, lpszExceptionName);
      KStringCat (g_szErrMessage, "\n", 1024);
      if (bHasParam) {
         KStringFmt (szEntry, 256, GFApp->getBuiltinString("BUILTIN_CRASH_PARAM"), (unsigned long) lpParam);
         KStringCat (szEntry, "\n", 1024);
         KStringCat (g_szErrMessage, szEntry, 1024);
      }
   }

   lpszRenderer = "Unknown";
   if (KPTK::_renderer == K_DIRECTX9)
      lpszRenderer = "DirectX9";
   if (KPTK::_renderer == K_DIRECTX)
      lpszRenderer = "DirectX8";
   if (KPTK::_renderer == K_OPENGL)
      lpszRenderer = "OpenGL";

   KStringFmt (szEntry, 256, GFApp->getBuiltinString("BUILTIN_REPORT_BODY"), KMiscTools::getOSVersion (), lpszRenderer);
   KStringCat (szEntry, "\n", 1024);
   KStringCat (g_szErrMessage, szEntry, 1024);
   
   for (i = 0; i < nStackDepth; i++) {
      KStringFmt (szEntry, 1024, "%ld: 0x%08lx (%s)\n", i, (unsigned long) lpStackReturnAddr[i], lpszStackModule[i]);
      KStringCat (g_szErrMessage, szEntry, 1024);
   }

   
   /* Log crash */
   K_LOG ("Game crash:\n\n%s", g_szErrMessage);

   /* Display alert */
   nResult = KMiscTools::alertBox (GFApp->getBuiltinString("BUILTIN_REPORT_CAPTION"), g_szErrMessage,
                                   GFApp->getBuiltinString("BUILTIN_REPORT_SEND"), GFApp->getBuiltinString("BUILTIN_REPORT_QUIT"));
   if (nResult == 0) 
   {
      /* User requested the report to be e-mailed 
      */
		#ifdef _ENABLE_CURL
	  		GFApp->SendReportToDev();
		#else
			KStringFmt (szEntry, 256, GFApp->getBuiltinString("BUILTIN_REPORT_EMAIL_SUBJECT"), (const char*)AppTitle_Str.c_str());
			KStringFmt (g_szEnquotedErrMessage, 1024, "mailto:%s?subject=%s&body=%s",
					"support@tuttifrutti.in", szEntry, g_szErrMessage);
					enquoteMessage (g_szEnquotedErrMessage, g_szErrMessage, 1024);
			KMiscTools::launchURL (g_szErrMessage, true);
		#endif
   }
   SAVEALLPROFILES();     
   /* Exit game */
   exit (0);
}


Application::Application()
{
	GFApp = this;
	Application::Instance = this;	
	IsWndFocus = false;	
	IsTaskBarMinimize = false;	
	AppActive = true;
	IsBestResolution = false;
	#ifdef K_iOS
		lpTextField = NULL;
		IsTextFieldEntered = false;
	#endif
    
    #ifdef K_MACOS
        isCommandLeftDown = false;
        isCommandLeftPressed = false;
        isCommandRightDown = false;
        isCommandRightPressed = false;
    #endif
    
	DesktopWidth = 0;
	DesktopHeight = 0;
	nMonitors = 1;
	IsTurnOffParticles = false;
	FadeAlpha = 0.0f;
	ProjX = ProjY = 0.0f;
	ProjZoom = 1.0f;

	canZoom = false;
    touchPosX2 = 0;
    touchPosY2 = 0;

    IsMacDebuggingOn = false;
    CurrentResolution.Width = -1;
    CurrentResolution.Height = -1;
    CurrentResolution.BPP = -1;
    CurrentResolution.CanWindow = false;

	RefreshInvItems = false;

	pauseBGMask = NULL; 
	
	Quad = NULL;
	Window = NULL;
	#ifdef K_ANDROID
	//  lpTextField = NULL;
		IsTextFieldEntered = false;
		MyVersion = 123;
		userInfo = "----";
		GFApp->forceSaveProfile = false;
	#endif

	//#ifdef JAPANESE_BUILD 
		lpFont20 = NULL;
		lpFont21 = NULL;
		lpFont22 = NULL;
		lpFontSmall = NULL;
		lpFontDiary = NULL;
		lpFontDropShadow = NULL;

	// Config File Read //
	IsDebugMode = false;
	DebugResMode = 0;
	IsSurveyBuild = false;
	IsSkipSplash = false;
	langID = 0;
	//Load from ini........
	KIniReader *iniReader;
	iniReader = new KIniReader ;

	#ifdef K_WIN32
		std::ifstream infile("config.ini");
	#else			
		std::ifstream infile(KMiscTools::makeFilePath("config.ini"));
	#endif
	
	if( infile.good() )
	{
		#ifdef K_WIN32
			bool flag = iniReader->setIni("config.ini");
		#else			
			bool flag = iniReader->setIni( KMiscTools::makeFilePath("config.ini") );
		#endif
		if(!flag)
		{
			// sac: cant use resource from text as we still dont know which Language to fetch the text from, so hard coded this message
			#ifdef K_WIN32
				KMiscTools::messageBox("CONFIG INI File!", "Er00x301- Failed to open Config INI." );			
			#else			
				KMiscTools::messageBox((const char*)"CONFIG INI File!",(const char*)"Er00x301- Failed to open Config INI.");			
			#endif
            //Failed to open
			IsDebugMode = false;
			return;
		}
		else
		{
			IsDebugMode = (int)iniReader->getValue("CONFIG","DEBUGMODE")==1;
			DebugResMode = (int)iniReader->getValue("CONFIG","DEBUGRES");
			IsSurveyBuild = (int)iniReader->getValue("CONFIG","SURVEYBUILD")==1;
			IsSkipSplash = (int)iniReader->getValue("CONFIG","SPLASHSKIP")==1;
			langID = (int)iniReader->getValue("CONFIG","LN");
		}
	}
	#ifdef K_ANDROID
	else
	{
		// reading config.ini will not work in Andorid, so we pass the config parameters from Android resources to our game
        //TODO: get these details from Java through JNI
		IsDebugMode = config_debugmode;
		DebugResMode = 11;//config_debugres;
		IsSurveyBuild = config_surveybuild;
		IsSkipSplash = config_splashskip ;

		langID = config_lang_id;
		pathToExternalDirectory = path_toExternalPicturesDirectory; // path to be used for saving wallpapers to external pictures directory
		gameVersionCode = gameversionCode;
		gameOBBVersionCode = gameobbversionCode;
		gameVersionName = gameversionName;
		deviceName = gdeviceName;
		K_LOG("Android device found is %s",deviceName.c_str());

		/*
		if (App_f_o_f == std::string("Full"))
		{
            MyVersion = 456; // initial init is as Full version, that is the game is build as full version
            //unlocked = true;
        }
		else
        {
		    MyVersion = 123;// initial init is as Free version, that is the game is build as free version, and purchase checks are needed to unlock
            //unlocked = false;
        }
	/*
 		// Check if the data is got correctly from the Android activity
 		char szLogMessage[1024];
		KStringFmt (szLogMessage, 1024,"Configs are %d, %d, %d, %d,%d ", IsDebugMode,DebugResMode, IsSurveyBuild,IsSkipSplash, langID);
		KMiscTools::alertBox ("Darkarta: A Broken Heart's Quest", szLogMessage, "OK", NULL);
	*/

	}
	GFApp->touchZoomFactor = 0;
	#endif
    
#ifdef K_iOS
    GFApp->touchZoomFactor = 0;
    DebugResMode = 11;
#endif
	infile.close();
	delete iniReader;
	// Config File Read //

	/*#ifdef K_MACOS //Sac: for some reason ini files are not been read by mac, so i am forcing this for debug
		IsDebugMode = true;
	// DebugResMode = 9;
	#endif
	*/

#ifndef DISABLE_LANG
	int id = GetAssetLanguage();
	if(id != -1)
		langID = id;
#endif

#ifdef _STEAM_BUILD
	int id = GetAssetLanguage();
	if (id != -1)
		langID = id;
#endif // _STEAM_BUILD

	//Sac: initiate StringTable instance and set language id from the Config.ini
	StringTable::SetMyLangauge(langID);

	// Add pakfile (files.pak or files.mp3[in Android] is really files.zip). "" is for an empty password. You can set a password in the zipfile and pass it here instead 

	/* Load string table file */


	#if defined( _PAKBUILD )//Release build
		{
			std::string szErrMessage;
			 szErrMessage = "Error opening Main Resource file !, please re-download the app, if this error presists, please contact us at info@tuttifrutti.in for further assistance";
			KResourceResult bSuccess;
        #if !defined _STEAM_BUILD
                #if defined (K_ANDROID)
                    #if defined (_AMAZON_BUILD)
                        std::string resFileName = "DAT_";
                        std::string resSuffixExtName = ".mp3";
                        std::string resLangIDName = StringTable::GetMyLangauge();
                        #if defined(_GAMEHOUSE_BUILD)
                            std::string resPUBName = "_GH";
                        #elif defined(_BIGFISH_BUILD)
                            std::string resPUBName = "_BF";
                        #elif defined(_STEAM_BUILD)
                            std::string resPUBName = "_STEAM";
                        #else
                            std::string resPUBName = "";
                        #endif
                        #if !defined (_CEBUILD)
                            resPUBName += std::string("_SE");
                        #endif
                        #if defined (_DEMOBUILD)
                            resPUBName += std::string("_DEMO");
                        #endif
                        std::string resFullNameStr = std::string(resFileName+resLangIDName+resPUBName+resSuffixExtName);
                        bSuccess =     KResource::addArchive (KMiscTools::makeFilePath (resFullNameStr.c_str()), K_RESARCHIVE_REQUIRED, RES_PASSWRD);
                        szErrMessage +=resFullNameStr;
                    #elif defined(_UNLOCKED_FOR_TEST_BUILD) // build as needed by Ajish , totally unlocked CE build to give it for testing to potentially any new publishers
                        std::string resFileName = "DAT_";
                        std::string resSuffixExtName = ".mp3";
                        std::string resLangIDName = StringTable::GetMyLangauge();
                        std::string resFullNameStr = std::string(resFileName+resLangIDName+resSuffixExtName);
                        int bSuccess1 =     KResource::addArchive (KMiscTools::makeFilePath (resFullNameStr.c_str()), K_RESARCHIVE_REQUIRED, RES_PASSWRD);
                        szErrMessage +=std::string(KMiscTools::makeFilePath (resFullNameStr.c_str()));
                        K_LOG("sRES result is %d",bSuccess1);

                    #else
                        std::string obbPath =KSysAndroid::getMainObbExpansionFilePath();
                        std::string actuallyUsedMainOBBVersion = std::to_string(gameOBBVersionCode); // Update this name to the current used eOBB for the game at App store
                        std::string currentMainOBBVersion = std::to_string(gameVersionCode); // Update this name to the current expected OBB for the game
                        std::string currentUsableOBBpath = std::regex_replace( obbPath, std::regex(currentMainOBBVersion), actuallyUsedMainOBBVersion );
                        // Use new release password for Android pack
                        bSuccess = KResource::addArchive (KMiscTools::makeFilePath (currentUsableOBBpath.c_str()), K_RESARCHIVE_REQUIRED, RES_PASSWRD);
                        // TODO: Need to write a way to read Patch obb files also for future releases of any patches, patch version should be taken from the activity through JNI
                    #endif
                #elif defined (K_iOS)
                    std::string resFileName = "DAT_";
                    std::string resSuffixExtName = "iOS.pak";
                    std::string resLangIDName = StringTable::GetMyLangauge();
                    std::string resFullNameStr = std::string(resFileName+/*resLangIDName+*/resSuffixExtName);
                    bSuccess = KResource::addArchive (KMiscTools::makeFilePath (resFullNameStr.c_str()), K_RESARCHIVE_REQUIRED, RES_PASSWRD);
                #elif defined (K_WIN32)
                    std::wstring resFileName = L"DAT";
                    std::wstring resSuffixExtName = L".pak";
                    //std::wstring resLangIDName = FileSystem::s2ws(StringTable::GetMyLangauge());
                    std::wstring resFullNameStr = std::wstring(resFileName+resSuffixExtName);
                    //bSuccess = 	KResource::addArchive (KMiscTools::makeFilePath (FileSystem::ws2s(resFullNameStr).c_str()), K_RESARCHIVE_REQUIRED, RES_PASSWRD);
                    bSuccess = 	KResource::addArchive (FileSystem::ws2s(resFullNameStr).c_str(), K_RESARCHIVE_REQUIRED, RES_PASSWRD);
                #elif defined (K_MACOS)
                    std::string resFileName = "MACDAT_";
                    std::string resSuffixExtName = ".pak";
                    std::string resLangIDName = StringTable::GetMyLangauge();
                    #if defined(_GAMEHOUSE_BUILD)
                        std::string resPUBName = "_GH";
                    #elif defined(_BIGFISH_BUILD)
                        std::string resPUBName = "_BF";
                    #elif defined(_STEAM_BUILD)
                        std::string resPUBName = "_STEAM";
                    #else
                        std::string resPUBName = "_ORIG";
                    #endif
                    #if !defined (_CEBUILD)
                        resPUBName += std::string("_SE");
                    #endif
                    #if defined (_DEMOBUILD)
                        resPUBName += std::string("_DEMO");
                    #endif
                    std::string resFullNameStr = std::string(resFileName+resLangIDName+resPUBName+resSuffixExtName);
                    bSuccess = 	KResource::addArchive (KMiscTools::makeFilePath (resFullNameStr.c_str()), K_RESARCHIVE_REQUIRED, RES_PASSWRD);
                    szErrMessage +=resFullNameStr;
                #else
                    std::string resFileName = "DAT_";
                    std::string resSuffixExtName = ".pak";
                    std::string resLangIDName = StringTable::GetMyLangauge();
                    #if defined(_GAMEHOUSE_BUILD)
                                std::string resPUBName = "_GH";
                    #elif defined(_BIGFISH_BUILD)
                                std::string resPUBName = "_BF";
                    #elif defined(_STEAM_BUILD)
                                std::string resPUBName = "_STEAM";
                    #else
                                std::string resPUBName = "_ORIG";
                    #endif
                    #if !defined (_CEBUILD)
                                resPUBName += std::string("_SE");
                    #endif
                    #if defined (_DEMOBUILD)
                                resPUBName += std::string("_DEMO");
                    #endif
                    std::string resFullNameStr = std::string(resFileName+resLangIDName+resPUBName+resSuffixExtName);
                    bSuccess =     KResource::addArchive (KMiscTools::makeFilePath (resFullNameStr.c_str()), K_RESARCHIVE_REQUIRED, RES_PASSWRD);
                    szErrMessage +=resFullNameStr;
                #endif
        #else
            std::string resFileName = "DAT_";
            std::string resSuffixExtName = ".PAK";
            std::string resLangIDName = StringTable::GetMyLangauge();
            std::string resPUBName = "STEAM";
			/*
            #if !defined (_CEBUILD)
                        resPUBName += std::string("_SE");
            #endif
			*/
            #if defined (_DEMOBUILD)
                        resPUBName += std::string("_DEMO");
            #endif
            std::string resFullNameStr = std::string(resFileName /*+resLangIDName*/ +resPUBName+resSuffixExtName);
            bSuccess =     KResource::addArchive (KMiscTools::makeFilePath (resFullNameStr.c_str()), K_RESARCHIVE_REQUIRED, RES_PASSWRD);
            szErrMessage +=resFullNameStr;
        #endif
            
            
			if (bSuccess !=K_RES_OK) {
					/* Failed to load */
				#ifdef _CEBUILD
					KMiscTools::alertBox ("Darkarta: A Broken Heart's Quest Collector's Edition", szErrMessage.c_str(), "Quit", NULL);
				#else
					KMiscTools::alertBox ("Darkarta: A Broken Heart's Quest", szErrMessage.c_str(), "Quit", NULL);
				#endif
				exit (0);
			}
		}
	#endif

	//Populate all the Game Strings
	StringTable::readStringTable(); // Sac: the path is not needed to be sent anymore
	StringTable::ReadBNRtextSizes();

	AssetsStringTable::readAssetsStringTable();
	AssetsStringTable::ReadBNRtextSizes();

	#ifdef _CEBUILD
		#ifdef K_WIN32	
			std::wstring appname = FileSystem::s2ws(StringTable::getStr("APPNAME"))+L" ";
			std::wstring appnametype = FileSystem::s2ws(StringTable::getStr("APPNAMETYPE"));
			AppTitle_Str = appname+appnametype;
		#else
			std::string appname = StringTable::getStr("APPNAME")+" ";
			std::string appnametype = StringTable::getStr("APPNAMETYPE");	
			AppTitle_Str = std::string(appname+appnametype);		
		#endif
	#else
		#ifdef K_WIN32	
			std::wstring appname = FileSystem::s2ws(StringTable::getStr("APPNAME"))+L" ";
			AppTitle_Str = appname;
		#else
			std::string appname = StringTable::getStr("APPNAME")+" ";
			AppTitle_Str = std::string(appname);
		#endif
	#endif

	#ifdef K_WIN32
		WRNG_WStr = FileSystem::s2ws(StringTable::getStr("WRNG"));			
		FATAL_WStr = FileSystem::s2ws(StringTable::getStr("FATAL"));
		LWMMRY_WStr = FileSystem::s2ws(StringTable::getStr("LWMMRY"));
		DADPTR_ER_WStr = FileSystem::s2ws(StringTable::getStr("DADPTR_ER"));

		GWRNG_0_WStr = FileSystem::s2ws(StringTable::getStr("GWRNG_0"));
		GWRNG_1_WStr = FileSystem::s2ws(StringTable::getStr("GWRNG_1"));
		GWRNG_2_WStr = FileSystem::s2ws(StringTable::getStr("GWRNG_2"));
		GWRNG_3_WStr = FileSystem::s2ws(StringTable::getStr("GWRNG_3"));
		GWRNG_4_WStr = FileSystem::s2ws(StringTable::getStr("GWRNG_4"));

		GWRNG_5_WStr = FileSystem::s2ws(StringTable::getStr("GWRNG_5"));	
	#else
		WRNG_WStr = StringTable::getStr("WRNG");
		FATAL_WStr = StringTable::getStr("FATAL");
		LWMMRY_WStr = StringTable::getStr("LWMMRY");
		DADPTR_ER_WStr = StringTable::getStr("DADPTR_ER");
		GWRNG_0_WStr = StringTable::getStr("GWRNG_0");

		GWRNG_1_WStr = StringTable::getStr("GWRNG_1");
		GWRNG_2_WStr = StringTable::getStr("GWRNG_2");
		GWRNG_3_WStr = StringTable::getStr("GWRNG_3");
		GWRNG_4_WStr = StringTable::getStr("GWRNG_4");
		GWRNG_5_WStr = StringTable::getStr("GWRNG_5");   
	#endif

	ProfileSystem::Instance->Initialize();

	#ifdef K_ANDROID
	/*
		//Sac: After ProfileSystem is Initialised, find if the game is already a Full version or not, incase the free is been bought
		// if the game is still free and not unlocked, the result will be true, if it is full, result will be false.
		if (MyVersion == 456) // If the build version is a full version by default then make all content unlocked already
				UnlockGame();
		// If build version is free version, then check if it is bought (from the game profile system)		
		unlocked = !ProfileSystem::Instance->IsGameVersionMode();
	 */
    
	GFApp->noOfHints = ProfileSystem::Instance->NoOfHints;

	#endif


	if(IsDebugMode)
	{
		Control::MasterTmr->IsShowingFPS = true;
	}
    
	if( CreateGameWindow() )
	{
		//	K_LOG("Game Window created!");
		IsWndFocus = true;
		KSysDebug::catchExceptions(crashHandler);

        #ifdef STRING_TABLE

				//3 truetexts created with specific heights - 20,21 & 22 pixels
				//Else during each draw we need to set heighpix which needs "rendering truetext" per frame!!
			#ifdef K_WIN32	
				std::wstring fontName = 	FileSystem::s2ws(StringTable::getStr("APPFONTNAME"));
				std::wstring fontFileExtname = L"";
				std::wstring fontItalicsName = L"";
				switch(StringTable::GetMyLangaugeID())
				{
					case LN_JAPANESE:
						fontFileExtname = +L"JP.otf";
						break;	
					case LN_KOREAN:
						fontFileExtname = +L"KR.otf";
						break;
					case LN_SIMPLIFIED_CHINESE:
						fontFileExtname = +L".otf";
						break;
					case LN_TRADITIONAL_CHINESE:
						fontFileExtname = +L".otf";
						break;	
					case LN_HINDI:
						fontFileExtname = +L".ttf";
						break;	
					case LN_SPANISH:
					case LN_GERMAN:
					case LN_FRENCH:
					case LN_ITALIAN:
					case LN_PORTUGUESE:
					case LN_BRAZILIAN_PORTUGUESE:
					case LN_RUSSIAN:
					case LN_UKRAINIAN:
					case LN_NEDERLANDS:
					case LN_HUNGARIAN:
					case LN_ENGLISH:
					default:
						fontFileExtname = +L".ttf";
						fontItalicsName = FileSystem::s2ws(StringTable::getStr("APPFONTITALICSNAME"))+L".ttf";		
						fontItalicsName = L"FNT\\"+fontItalicsName;	
						break;
				}

				std::wstring fontFilename = L"FNT\\"+fontName +fontFileExtname;


				if(fontItalicsName!=L"")
					lpFont20 = new KTrueText(FileSystem::charPath(FileSystem::ws2s(fontItalicsName)));	
				else
					lpFont20 = new KTrueText(FileSystem::charPath(FileSystem::ws2s(fontFilename)));	
				lpFont20->setHeightPix(HEIGHTPIX_20);

				lpFontDiary = new KTrueText(FileSystem::charPath(FileSystem::ws2s(fontItalicsName)));
				lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				lpFont21 = new KTrueText(FileSystem::charPath(FileSystem::ws2s(fontFilename)));	
				lpFont21->setHeightPix(HEIGHTPIX_21);
				lpFont22 = new KTrueText(FileSystem::charPath(FileSystem::ws2s(fontFilename)));	
				lpFont22->setHeightPix(HEIGHTPIX_22);
				lpFontSmall = new KTrueText(FileSystem::charPath(FileSystem::ws2s(fontFilename)));	
				lpFontSmall->setHeightPix(HEIGHTPIX_SMALL);
				lpInvPopFont = new KTrueText(KMiscTools::makeFilePath(std::string("Assets\\FNT\\"+StringTable::getStr("INVFONTNAME")+".ttf").c_str()));
				lpInvPopFont->setHeightPix(HEIGHTPIX_INV_POP);
				lpFontDropShadow = new KTrueText(KMiscTools::makeFilePath(std::string("Assets\\FNT\\Exo2-RegularCondensed.ttf").c_str())); //FileSystem::charPath(FileSystem::ws2s(fontFilename)));
				lpFontDropShadow->setHeightPix(HEIGHTPIX_DROP);
				//lpFontDropShadow->setDropShadow(3, 3, 135, 0, 0, 0, 0.5);
			#else
				std::string fontName = 	StringTable::getStr("APPFONTNAME");
				std::string fontFileExtname = "";
				std::string fontItalicsName = "";
				switch(StringTable::GetMyLangaugeID())
				{
					case LN_JAPANESE:
						fontFileExtname = +"JP.otf";
						break;	
					case LN_KOREAN:
						fontFileExtname = +"KR.otf";
						break;
					case LN_SIMPLIFIED_CHINESE:
						fontFileExtname = +"SC.otf";
						break;
					case LN_TRADITIONAL_CHINESE:
						fontFileExtname = +"TC.otf";
						break;	
					case LN_HINDI:
						fontFileExtname = +".ttf";
						break;	
					//case LN_SPANISH:
					case LN_GERMAN:
					case LN_FRENCH:
					case LN_ITALIAN:
					case LN_PORTUGUESE:
					case LN_BRAZILIAN_PORTUGUESE:
					case LN_RUSSIAN:
					case LN_UKRAINIAN:
					case LN_NEDERLANDS:
					case LN_HUNGARIAN:
					case LN_ENGLISH:
					default:
						fontFileExtname = +".ttf";
						fontItalicsName = StringTable::getStr("APPFONTITALICSNAME")+".ttf";		
						fontItalicsName = "FNT\\"+fontItalicsName;	
						break;
				}

				std::string fontFilename = "FNT\\"+fontName +fontFileExtname;
				//	std::string fontbinFilename = fontName +".txt";
				//	std::string fontpngFilename = fontName +".png";
			if(fontItalicsName!="")
				lpFont20 = new KTrueText(FileSystem::charPath(fontItalicsName));	
			else
				lpFont20 = new KTrueText(FileSystem::charPath(fontFilename));	
				lpFont20->setHeightPix(HEIGHTPIX_20);
				
				lpFontDiary = new KTrueText(FileSystem::charPath(fontItalicsName));
				lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				lpFont21 = new KTrueText(FileSystem::charPath(fontFilename));	
				lpFont21->setHeightPix(HEIGHTPIX_21);
				lpFont22 = new KTrueText(FileSystem::charPath(fontFilename));	
				lpFont22->setHeightPix(HEIGHTPIX_22);
				lpFontSmall = new KTrueText(FileSystem::charPath(fontFilename));	
				lpFontSmall->setHeightPix(HEIGHTPIX_SMALL);
				lpInvPopFont = new KTrueText(KMiscTools::makeFilePath(std::string("Assets\\FNT\\"+StringTable::getStr("INVFONTNAME")+".ttf").c_str()));
				lpInvPopFont->setHeightPix(HEIGHTPIX_INV_POP);

				lpFontDropShadow = new KTrueText(KMiscTools::makeFilePath(std::string("Assets\\FNT\\Exo2-RegularCondensed.ttf").c_str())); //FileSystem::charPath(FileSystem::ws2s(fontFilename)));
				lpFontDropShadow->setHeightPix(HEIGHTPIX_DROP);
				//lpFontDropShadow->setDropShadow(3, 3, 135, 0, 0, 0, 0.5);
			#endif
		#endif	

		SetCutSceneVideoResParameters(app_video_res); // Initialize the resolutions and scale for Cut scene videos

		CTexFactory::Instance->LoadImg("UI\\IB.PNG",false,true,true);//24 bit for iPad

		Quad = KPTK::createKGraphic();
		Quad->setTextureQuality(true);	

		pauseBGMask = KPTK::createKGraphic();
		pauseBGMask->setTextureQuality(true);

		Elixir::CreateTrigTables();
	#ifdef K_WIN32
		nMonitors = GetSystemMetrics(SM_CMONITORS);//handle dual monitors!!
	#endif
		//Application update will run at constant 16 - (60 Hz display frequency-> 1000ms/60 frames => ~ 16 ms for 1 frame )
		SynchLogicRate = 16;//1000/Window->getDesktopDisplayFrequency(); //1000/60 => 16.67 ~ 16 
		
		FPSText = NULL;	
		if(IsDebugMode)
		{
			#ifdef K_ANDROID
				gameVersionText = NULL;	
				CreateGameVersionText(); // Show the game version on the screen.
			#endif
			CreateFPS();//loads first font!!
		}		
		
		#ifdef K_ANDROID
				KUIElement::initialize (Window);
		#endif

		AddControl(new GameFlow());
	}

	//CreateWatermark();
#ifdef TOUCH_UI
for(int i = 0; i < 4; i++)
{
	touchRipple[i] = new CObject();
	touchRipple[i]->LoadSprite("UI\\TOUCHRIPPLE.SPR");
	touchRipple[i]->SetZ(5000);
	touchRipple[i]->SetXY(-100, -100);
    touchRipple[i]->SetScale(0.5);
	touchRipple[i]->Show(true);
}
    rippleIndex = 0;
#endif // DEBUG

#ifdef K_iOS
    //Game Center
    KSysiPhone::gameCenterAuthenticatePlayer();
    if(KSysiPhone::isGameCenterPlayerAuthenticated())
    {
    
    }
    if(KSysiPhone::isGameCenterAvailable())
    {
        
    }
#endif
}



/////////////////////////////////////////////////

/* Event handler */
bool myEventHandler (KEvent *lpKEvent) {
  
  #ifdef K_ANDROID
  	bool bProcessed, bSuccess;
	 /* Send event to UI first */
  	bProcessed = KUIElement::dispatchEvent (lpKEvent);
  // 	if (!bProcessed)
        {
      /* Not caught by UI, handle */
	#endif
#ifdef TOUCH_UI
            if(GFApp->_tapped)
            {
                GFApp->_tapped = false;
                GFApp->touched = false;
            }
#endif
      switch (lpKEvent->type) 
	  {
        #if ( defined  K_MACOS)
          case K_EVENT_KEYDOWN:
             // K_LOG("Key pressed is %d ",(long)lpKEvent->ptkKey);
              if(lpKEvent->ptkKey== K_VK_MENU)
              {
                  GFApp->isCommandLeftDown = true;
              }
              else
              {
                  if( GFApp->isCommandLeftDown )
                      GFApp->isCommandLeftPressed = true;
                  GFApp->isCommandLeftDown = false;
              }
              if( lpKEvent->ptkKey== K_VK_ERROR)
              {
                  GFApp->isCommandRightDown = true;
              }
              else
              {
                  if( GFApp->isCommandRightDown )
                      GFApp->isCommandRightPressed = true;
                  GFApp->isCommandRightDown = false;
              }
              if (lpKEvent->ptkKey== K_VK_Q &&(GFApp->isCommandLeftPressed || GFApp->isCommandRightPressed))
              {
                      /* "Command Q" key */
                      /* On the main screen; quit */
                      // GFApp->Window->setQuit (true);
                      //    if(GFMainMenuCtrl!=NULL)
                      //        GFMainMenuCtrl->InitiateExitPopupFromOutSide();
                      //    if(GFMainMenuCtrl!=NULL)GFMainMenuCtrl->ShowMessageBanner("Back key is pressed Down");
                       K_LOG("User Pressed Commnd- Q to quit the game.");
                      GFApp->AppActive = false;
              }
              Control::Input->SetKeydown(lpKEvent->ptkKey);
              break;
          case K_EVENT_KEYUP:
              // K_LOG("Key Up is %d ",(long)lpKEvent->ptkKey);
              if( lpKEvent->ptkKey == K_VK_MENU)
              {
                  GFApp->isCommandLeftDown = false;
              }
              if( lpKEvent->ptkKey == K_VK_ERROR)
              {
                  GFApp->isCommandRightDown = false;
              }
              Control::Input->SetKeyup(lpKEvent->ptkKey);
              break;
        #endif
		#ifdef TOUCH_UI
			case K_EVENT_KEYDOWN:
				if (lpKEvent->ptkKey == K_VK_PRIOR)
				{
						/* "Back" key */
						/* On the main screen; quit */
						// GFApp->Window->setQuit (true);
					//	if(GFMainMenuCtrl!=NULL) 
					//		GFMainMenuCtrl->InitiateExitPopupFromOutSide();
					//	if(GFMainMenuCtrl!=NULL)GFMainMenuCtrl->ShowMessageBanner("Back key is pressed Down");
						if( GFInstance!= NULL)
							if( GFInstance->Flow == fSplash || GFInstance->Flow == fHaveYouCutscene) GFApp->Window->setQuit (true);
						Control::Input->SetKeydown(K_VK_PRIOR);	
					}
				break;
			case K_EVENT_KEYUP:
             if (lpKEvent->ptkKey == K_VK_PRIOR) {
					/* "Back" key */
				//	if(GFMainMenuCtrl!=NULL)GFMainMenuCtrl->ShowMessageBanner("Back key is pressed Up");
						Control::Input->SetKeyup(K_VK_PRIOR);
				}

				break;
			case K_EVENT_TOUCH:
			{				
				if( lpKEvent->touchType == K_TOUCHBEGINS ) //One time reaction
				{
                    GFApp->ripple = true;
                    GFApp->canPan = false;
					GFApp->touchTime = 0;//Time
                    GFApp->touchVelocity = 0;//Velocity
					GFApp->startTouchX = KInput::getMouseX();
					GFApp->startTouchY = KInput::getMouseY();
                    GFApp->flingX = 0;
                    GFApp->flingY = 0;
                    GFApp->touched = true;
					GFApp->_tapped = false;
				}

				if(lpKEvent->touchType == K_TOUCHCONTINUES )
                {
                    GFApp->canPan = true;
					GFApp->touchTime += 1;
                    GFApp->flingX = 0;
                    GFApp->flingY = 0;
                    GFApp->touchVelocity = 0;
                    //GFApp->touched = true;
					GFApp->_tapped = false;
                }
                if(lpKEvent->touchType == K_TOUCHENDS )
                {
                    GFApp->_tapped = true;
					float _dist = std::sqrt(((KInput::getMouseX() - GFApp->startTouchX)*(KInput::getMouseX() - GFApp->startTouchX)) + ((KInput::getMouseY() - GFApp->startTouchY)*(KInput::getMouseY() - GFApp->startTouchY)));
                    if(GFApp->flingTimer > 25)
                    {
                        GFApp->touchVelocity = _dist / GFApp->touchTime;
                        GFApp->flingX = KInput::getMouseX() - GFApp->startTouchX;
                        GFApp->flingY = KInput::getMouseY() - GFApp->startTouchY;


                        float magnitude = std::sqrt(GFApp->flingX * GFApp->flingX + GFApp->flingY * GFApp->flingY);
                        GFApp->flingX = GFApp->flingX / magnitude;
                        GFApp->flingY = GFApp->flingY / magnitude;
                    }
                    else
                    {
                        GFApp->flingX = 0;
                        GFApp->flingY = 0;
                        GFApp->touchVelocity = 0;
                    }

                    if(GFApp->touchVelocity > 50)
                        GFApp->touchVelocity = 50;
                                        
                    GFApp->touched = false;
                    
                }
			}
				break;

	      case K_EVENT_GESTURE_ZOOM:
			GFApp->touchZoomFactor = lpKEvent->zoomFactor;

              GFApp->flingX = 0;
              GFApp->flingY = 0;
              GFApp->touchVelocity = 0;
            break;

			case K_EVENT_ORIENTATION:
				
				/* Device orientation changed, follow it if it changed to landscape or upside-down landscape */
				if (lpKEvent->devOrientation == K_LANDSCAPE || lpKEvent->devOrientation == K_RLANDSCAPE)
					GFApp->Window->setOrientation (lpKEvent->devOrientation);
				break;

			#ifdef K_ANDROID
			case K_EVENT_LOCAL_NOTIFICATION:
					/* Local notification received */
					K_LOG ("Local notification: userdata=%ld", lpKEvent->notificationUserData);
					break;
			case K_EVENT_KEYBOARD_DISMISS:
				/* Virtual keyboard hidden */
				KSysAndroid::setVirtualKeyboardStatus (false);
				break;
			#endif
		#endif
		  case K_EVENT_ACTIVATED:	
			 // Window got focus; resume audio 
			  if( GFApp != NULL )
			  {
				  if( !GFApp->IsWndFocus )
				  {
					  K_LOG("Window Focus Activated");
					 
                      if( GFInstance != NULL  && GFInstance->IsFirstTimeGFLoadingComplete )
                      {
                          GFInstance->PausedImage->Show(false);
                          GFApp->IsTaskBarMinimize = false;
                          GFApp->IsWndFocus = true;
                          if( Control::Audio->Silent )
                              if( GFInstance != NULL )
                                  if( GFInstance->IsPakResLoadDone )
                                      Control::Audio->EndSilence();

					  /*
					  When window is minimized and reactivated, the scene rendertarget seems lost.
					  So whenever window is minimized and got focus, we reload the graphic
					  */
                          GFInstance->ReloadSceneGraphic();
                      }
                }
			  }
			#if  (defined K_ANDROID)
				KSound::resume();
				KSysAndroid::cancelLocalNotifications();
			#endif
			  break;

		  case K_EVENT_DEACTIVATED:			 
			 if( GFApp != NULL )
				 if( GFApp->IsWndFocus )
				 {
					 K_LOG("Window Focus Deactivate Event");
                     if( GFInstance != NULL && GFInstance->IsFirstTimeGFLoadingComplete )
                     {
                          if(GFInstance->CurrentSceneGraphic !=NULL)
                          {
                              if(GFInstance->IsScnRTCreated)
                              {
                                  if(GFInstance->CurrentSceneGraphic->isRenderTargetLost())
                                      K_LOG("CurrentSceneGraphic RenderTargetLost");
                              }
                          }
                     }
                         GFApp->IsWndFocus = false;
					 if( GFInstance != NULL )
						if( GFInstance->IsPakResLoadDone )
						{
							// Window lost focus; suspend audio 
							if( !Control::Audio->Silent )
								 Control::Audio->Silence();	

							//Save current level data - first time & later on every 1 minute gap!!
							unsigned long CurTime = KMiscTools::getMilliseconds();
							if( GFInstance->LastSaveTime == (unsigned long)-1 || 
								(CurTime-GFInstance->LastSaveTime) >= 60000 )
							{			
								GFInstance->LastSaveTime = CurTime;
								if( ProfileSystem::Instance->ProfileSlot >= 0 && ProfileSystem::Instance->ProfileSlot < MAX_PROFILES )
								{
									//ProfileSystem::Instance->SaveProfile(ProfileSystem::Instance->ProfileSlot);	
								}
							}	

							#ifndef DEBUG
							if( GFApp->nMonitors > 1 )
								GFApp->Window->minimize();
							#endif // !DEBUG		
						}					
				 }

				#ifdef K_ANDROID
					KSound::suspend ();
					/* Schedule example local notification in 15 seconds with user data 54321 */
					bSuccess = KSysAndroid::scheduleLocalNotification (0, 0, 0, 15, false, "Please Save your child from Darkarta... time is running out ! ", 54321);
					if (bSuccess)
						K_LOG ("Schedule local notification");
					else
						K_LOG ("Local notifications not supported");
				#endif
			 break;
           
		  case K_EVENT_LOW_MEMORY:
              /*
					KMiscTools::messageBox( (const char*)LWMMRY_WStr.c_str(), 
											(const char*)GWRNG_0_WStr.c_str() );
			  exit(0);
               */
               
			  break;		  
		  
      }   
	#if  (defined K_ANDROID)
	} // this is for the extra IF statement in Android version
	#endif
   return true;
}

void Application::InitRenderer( int DisplayAdapterMode, int BPP )
{
	switch( DisplayAdapterMode )
	{
		case R1366x768:
				IsBestResolution = true;
				CurrentResolution.Width = 1366;
				CurrentResolution.Height = 768;				       
				break;
		case R1360x768:
				IsBestResolution = true;
				CurrentResolution.Width = 1360;
				CurrentResolution.Height = 768;				           
				break;
		case R1280x768:
				IsBestResolution = true;
				CurrentResolution.Width = 1280;
				CurrentResolution.Height = 768;				          
				break;		
		case R1024x768:
				IsBestResolution = true;
				CurrentResolution.Width = 1024;
				CurrentResolution.Height = 768;			
				break;
		case R1280x720:
			    IsBestResolution = true;
				CurrentResolution.Width = 1280;
				CurrentResolution.Height = 720;				          
				break;
		case R800x600: 
				CurrentResolution.Width = 800;
				CurrentResolution.Height = 600;				
                break;		
		case R640x480: 
				CurrentResolution.Width = 640;
				CurrentResolution.Height = 480;				
                break;
        case R1280x800:  // Supported for mac
 	        IsBestResolution = true;
            CurrentResolution.Width = 1280;
            CurrentResolution.Height = 800;
            break;
        case R1152x720:   // Supported for mac
 		    IsBestResolution = true;
            CurrentResolution.Width = 1152;
            CurrentResolution.Height = 720;
            break;
        case R1024x640:   // Supported for mac
            IsBestResolution = true;
            CurrentResolution.Width = 1024;
            CurrentResolution.Height = 640;
            break;
        case R1920x1080:   // Full HD
            IsBestResolution = true;
            CurrentResolution.Width = 1920;
            CurrentResolution.Height = 1080;
            break;
		case R1920x1200:   // Full HD+
 
            IsBestResolution = true;
            CurrentResolution.Width = 1920;
            CurrentResolution.Height = 1200;
            break;
        case R2560x1440:   // QHD
            IsBestResolution = true;
            CurrentResolution.Width = 2560;
            CurrentResolution.Height = 1440;
            break;
        case R3840x2160:   // 4K
            IsBestResolution = true;
            CurrentResolution.Width = 3840;
            CurrentResolution.Height = 2160;
            break;
 	    case R7680x4320:   // 8K
            IsBestResolution = true;
            CurrentResolution.Width = 7680;
            CurrentResolution.Height = 4320;
            break;
       case R1600x900:   // some stupid res
            IsBestResolution = true;
            CurrentResolution.Width = 1600;
            CurrentResolution.Height = 900;
            break;
        case R1344x756:   // some stupid res
            IsBestResolution = true;
            CurrentResolution.Width = 1344;
            CurrentResolution.Height = 756;
            break;            
	}
			CurrentResolution.BPP = BPP;		
		
		ProjZoom = CurrentResolution.Height/MAX_DISPLAY_HEIGHT;//(600/768)
		ProjX = ((CurrentResolution.Width-MAX_DISPLAY_WIDTH)/2)*ProjZoom;
		ProjY = ((MAX_DISPLAY_HEIGHT - CurrentResolution.Height)/2)*ProjZoom;
    ProfileSystem::Instance->ActiveProfile.Resolution = DisplayAdapterMode;
    ProfileSystem::Instance->ActiveProfile.BPP = BPP;     
    MessageTextPos.SetXYZ(float(MAX_DISPLAY_WIDTH/2.0f), 30.0f, 0.0f); // Sac: This should be based on the current Resolution with right ?
  	//  MessageTextPos.SetXYZ(float(CurrentResolution.Width/2.0f), 30.0f, 0.0f);

  	//Force 768p for Android
	#ifdef TOUCH_UI
	ProjZoom = 1;
	ProjX = 0;
	ProjY = 0;
	#endif
        
}

int Application::WhichDisplayMode(int DWidth,int DHeight )
{	
	int Mode = 0;
	//Try to replicate user desktop modes first - 6 modes!!
	if ( DWidth == 1366 && DHeight == 768 ) 
		Mode = R1366x768;
	else if ( DWidth == 1360 && DHeight == 768 ) 
		Mode = R1360x768;
	else if ( DWidth == 1280 && DHeight == 768 ) 
		Mode = R1280x768;
	else if ( DWidth == 1280 && DHeight == 720 ) 
		Mode = R1280x720;
	else if ( DWidth == 1024 && DHeight == 768 ) 
		Mode = R1024x768;
	else if ( DWidth == 800 && DHeight == 600 ) 
		Mode = R800x600;
	else if ( DWidth == 640 && DHeight == 480 ) 
		Mode = R640x480;
    // Check if it is Mac Book Pro resoultions
    else if ( DWidth == 1280 && DHeight == 800 )
        Mode = R1280x800;
    else if ( DWidth == 1152 && DHeight == 720 )
        Mode = R1152x720;
    else if ( DWidth == 1024 && DHeight == 640 )
        Mode = R1024x640;
    //HDs
    else if ( DWidth == 1920 && DHeight == 1080 )  //Full HD
        Mode = R1920x1080;
	else if ( DWidth == 1920 && DHeight == 1200 )  //Full HD+
        Mode = R1920x1200;
    else if ( DWidth == 2560 && DHeight == 1440 ) //QHD
        Mode = R2560x1440;
    else if ( DWidth == 3840 && DHeight == 2160 )  //4K
        Mode = R3840x2160;
    else if ( DesktopWidth == 7680 && DesktopHeight == 4320 ) //8K
        Mode = R7680x4320;

    else if ( DesktopWidth == 1600 && DesktopHeight == 900 ) //some stupid resolution
        Mode = R1600x900;
    else if ( DesktopWidth == 1344 && DesktopHeight == 756 ) //some stupid resolution
        Mode = R1344x756;
	//Even if desktop mode we can't support, user can have supporting modes available 
	else if( (DWidth > 1024 && DHeight >= 768) )
	    Mode = R1366x768;
	//check from 1024
	else if( (DWidth >= 1024 ) )
	    Mode = R1024x768;
	else if( (DWidth >= 800 ) )
	    Mode = R800x600;
	else if( (DWidth >= 640 ) )
	    Mode = R640x480;
	else //iterate through all modes from start!! 
		Mode = R1366x768;	
	return Mode;
}

int Application::WhichDisplayMode() //Internal DesktopWidth and DesktopHeight is used for this
{	
	int Mode = 0;
	//Try to replicate user desktop modes first - 6 modes!!
	if ( DesktopWidth == 1366 && DesktopHeight == 768 ) 
		Mode = R1366x768;
	else if ( DesktopWidth == 1360 && DesktopHeight == 768 ) 
		Mode = R1360x768;
	else if ( DesktopWidth == 1280 && DesktopHeight == 768 ) 
		Mode = R1280x768;
	else if ( DesktopWidth == 1280 && DesktopHeight == 720 ) 
		Mode = R1280x720;
	else if ( DesktopWidth == 1024 && DesktopHeight == 768 ) 
		Mode = R1024x768;
	else if ( DesktopWidth == 800 && DesktopHeight == 600 ) 
		Mode = R800x600;
	else if ( DesktopWidth == 640 && DesktopHeight == 480 ) 
		Mode = R640x480;
    
    // Check if it is Mac Book Pro resoultions
    else if ( DesktopWidth == 1280 && DesktopHeight == 800 )
        Mode = R1280x800;
   else if ( DesktopWidth == 1152 && DesktopHeight == 720 )
        Mode = R1152x720;
    else if ( DesktopWidth == 1024 && DesktopHeight == 640 )
        Mode = R1024x640;
    //HDs
    else if ( DesktopWidth == 1920 && DesktopHeight == 1080 )  //Full HD
        Mode = R1920x1080;
	else if ( DesktopWidth == 1920 && DesktopHeight == 1200 )  //Full HD+
        Mode = R1920x1200;
    else if ( DesktopWidth == 2560 && DesktopHeight == 1440 ) //QHD
        Mode = R2560x1440;
    else if ( DesktopWidth == 3840 && DesktopHeight == 2160 )  //4K
        Mode = R3840x2160;
	
    else if ( DesktopWidth == 7680 && DesktopHeight == 4320 ) //8K
        Mode = R7680x4320;

    else if ( DesktopWidth == 1600 && DesktopHeight == 900 ) //some stupid resolution
        Mode = R1600x900;
    else if ( DesktopWidth == 1344 && DesktopHeight == 756 ) //some stupid resolution
        Mode = R1344x756;

	//Even if desktop mode we can't support, user can have supporting modes available 
	else if( (DesktopWidth > 1024 && DesktopHeight >= 768) )
	    Mode = R1366x768;
	//check from 1024
	else if( (DesktopWidth >= 1024 ) )
	    Mode = R1024x768;
	else if( (DesktopWidth >= 800 ) )
	    Mode = R800x600;
	else if( (DesktopWidth >= 640 ) )
	    Mode = R640x480;
	else //iterate through all modes from start!! 
		Mode = R1366x768;	
	return Mode;
}

void Application::SetDisplayWidthnHeight(int Mode) //Set DesktopWidth and DesktopHeight based on mode
{	
	switch (Mode)
		{
			case R640x480:
				DesktopWidth = 640;
				DesktopHeight = 480;
				break;
			case R800x600:
				DesktopWidth = 800;
				DesktopHeight = 600;
				break;
			case R1024x768:
				DesktopWidth = 1024;
				DesktopHeight = 768;
				break;
			case R1280x768:
				DesktopWidth = 1280;
				DesktopHeight = 768;
				break;
			case R1280x720:
				DesktopWidth = 1280;
				DesktopHeight = 720;
				break;
			case R1360x768:
				DesktopWidth = 1360;
				DesktopHeight = 768;
				break;
			case R1366x768:
				DesktopWidth = 1366;
				DesktopHeight = 768;
				break;
			case R1280x800:  // Supported for mac
				DesktopWidth = 1280;
				DesktopHeight = 800;
				break;
			case R1152x720:   // Supported for mac
				DesktopWidth = 1152;
				DesktopHeight = 720;
				break;
			case R1024x640:   // Supported for mac
				DesktopWidth = 1024;
				DesktopHeight = 640;
				break;
			case R1920x1080:   // Full HD
				DesktopWidth = 1920;
				DesktopHeight = 1080;
				break;
			case R1920x1200:   // Full HD+
				DesktopWidth = 1920;
				DesktopHeight = 1200;
				break;
			case R2560x1440:   // Q HD
				DesktopWidth = 2560;
				DesktopHeight = 1440;
				break;
			case R3840x2160:   // 4K
				DesktopWidth = 3840;
				DesktopHeight = 2160;
				break;
		
			case R7680x4320:   // 8K
				DesktopWidth = 7680;
				DesktopHeight = 4320;
				break;
	
			case R1600x900:   //some stupid resolution
				DesktopWidth = 1600;
				DesktopHeight = 900;
				break;
			case R1344x756:   //some stupid resolution
				DesktopWidth = 1344;
				DesktopHeight = 756;
				break;
			default:
				DesktopWidth = 1366;
				DesktopHeight = 768;
				break;
		}				
}


bool Application::CreateBestWindowedDisplayMode( )
{	
	int Mode = ProfileSystem::Instance->ActiveProfile.Resolution;

    K_LOG("BestWin : depth %d",(int)WindowDepth);
    K_LOG("BestWin : BPP %d",(int)ProfileSystem::Instance->ActiveProfile.BPP);
    K_LOG("BestWin : WINDOWED %b",(bool)ProfileSystem::Instance->ActiveProfile.Windowed);
	switch ( Mode )
	{
        case R1920x1080:  //FullHD
            if( CanWindow(1920,1080) ) 
            {
                if( Window->createGameWindow(1920, 1080, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1920x1080, 32);   return true; }
                else if( Window->createGameWindow(1920, 1080, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1920x1080, 24);   return true; }
                else if( Window->createGameWindow(1920, 1080, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1920x1080, 16);   return true; }
            }
			case R1920x1200:  //FullHD+
            if( CanWindow(1920,1200) ) 
            {
                if( Window->createGameWindow(1920, 1200, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1920x1200, 32);   return true; }
                else if( Window->createGameWindow(1920, 1200, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1920x1200, 24);   return true; }
                else if( Window->createGameWindow(1920, 1200, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1920x1200, 16);   return true; }
            }
        case R2560x1440: //QHD
            if( CanWindow(2560,1440) ) 
            {
                if( Window->createGameWindow(2560, 1440, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R2560x1440, 32);   return true; }
                else if( Window->createGameWindow(2560, 1440, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R2560x1440, 24);   return true; }
                else if( Window->createGameWindow(2560, 1440, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R2560x1440, 16);   return true; }
            }
        case R3840x2160: //4K
            if( CanWindow(3840,2160) )
            {
                if( Window->createGameWindow(3840, 2160, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R3840x2160, 32);   return true; }
                else if( Window->createGameWindow(3840, 2160, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R3840x2160, 24);   return true; }
                else if( Window->createGameWindow(3840, 2160, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R3840x2160, 16);   return true; }
            }	
        case R7680x4320: //8K
            if( CanWindow(7680,4320) ) 
            {
                if( Window->createGameWindow(7680, 4320, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R7680x4320, 32);   return true; }
                else if( Window->createGameWindow(7680, 4320, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R7680x4320, 24);   return true; }
                else if( Window->createGameWindow(7680, 4320, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R7680x4320, 16);   return true; }
            }

        case R1600x900: //some stupid resolution
            if( CanWindow(1600,900) ) 
            {
                if( Window->createGameWindow(1600, 900, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1600x900, 32);   return true; }
                else if( Window->createGameWindow(1600, 900, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1600x900, 24);   return true; }
                else if( Window->createGameWindow(1600, 900, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1600x900, 16);   return true; }
            }
        case R1344x756: //some stupid resolution
            if( CanWindow(1344,756) )
            {
                if( Window->createGameWindow(1344, 756, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1344x756, 32);   return true; }
                else if( Window->createGameWindow(1344, 756, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1344x756, 24);   return true; }
                else if( Window->createGameWindow(1344, 756, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1344x756, 16);   return true; }
            }
		case R1360x768:
			if( CanWindow(1360,768) ) //Window->checkWindowedMode(1360,768) 
			{
				if( Window->createGameWindow(1360, 768, 32, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1360x768, 32);   return true; }
				else if( Window->createGameWindow(1360, 768, 24, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1360x768, 24);   return true; }
				else if( Window->createGameWindow(1360, 768, 16, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1360x768, 16);   return true; }
			}
		case R1280x768:
			if( CanWindow(1280,768) ) //Window->checkWindowedMode(1280,768)
			{
				if( Window->createGameWindow(1280, 768, 32, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1280x768, 32);   return true; }
				else if( Window->createGameWindow(1280, 768, 24, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1280x768, 24);   return true; }
				else if( Window->createGameWindow(1280, 768, 16, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1280x768, 16);   return true; }
			}
		case R1280x720:
			if( CanWindow(1280,720) ) //Window->checkWindowedMode(1280,720)
			{
				if( Window->createGameWindow(1280, 720, 32, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1280x720, 32);   return true; }
				else if( Window->createGameWindow(1280, 720, 24, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1280x720, 24);   return true; }
				else if( Window->createGameWindow(1280, 720, 16, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1280x720, 16);   return true; }
			}
	//Mac book pro resolutions

        case R1280x800:
            if( CanWindow(1280,800) ) //Window->checkWindowedMode(1280,720)
            {
                if( Window->createGameWindow(1280, 800, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1280x800, 32);   return true; }
                else if( Window->createGameWindow(1280, 800, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1280x800, 24);   return true; }
                else if( Window->createGameWindow(1280, 800, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1280x800, 16);   return true; }
            }
        case R1152x720:
            if( CanWindow(1152,720) ) //Window->checkWindowedMode(1280,720)
            {
                if( Window->createGameWindow(1152, 720, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1152x720, 32);   return true; }
                else if( Window->createGameWindow(1152, 720, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1152x720, 24);   return true; }
                else if( Window->createGameWindow(1152, 720, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1152x720, 16);   return true; }
            }
        case R1024x640:
            if( CanWindow(1024,640) ) //Window->checkWindowedMode(1280,720)
            {
                if( Window->createGameWindow(1024, 640, 32, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1024x640, 32);   return true; }
                else if( Window->createGameWindow(1024, 640, 24, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1024x640, 24);   return true; }
                else if( Window->createGameWindow(1024, 640, 16, true, (const char*)AppTitle_Str.c_str() ))
                {    InitRenderer( R1024x640, 16);   return true; }
            }
      
            
 	// rest if resolutions
        case R1024x768:
			if( CanWindow(1024,768)) //Window->checkWindowedMode(1024,768)
			{
				if( Window->createGameWindow(1024, 768, 32, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1024x768, 32);   return true; }
				else if( Window->createGameWindow(1024, 768, 24, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1024x768, 24);   return true; }
				else if( Window->createGameWindow(1024, 768, 16, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1024x768, 16);   return true; }
			}
		case R800x600:
			if( CanWindow(800,600)) //Window->checkWindowedMode(800,600)
			{
				if( Window->createGameWindow(800, 600, 32, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R800x600, 32);   return true; }
				else if( Window->createGameWindow(800, 600, 24, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R800x600, 24);   return true; }
				else if( Window->createGameWindow(800, 600, 16, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R800x600, 16);   return true; }	
			}
		case R640x480:
			if( CanWindow(640,480))  // Window->checkWindowedMode(640,480)
			{
				if( Window->createGameWindow(640, 480, 32, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R640x480, 32);   return true; }
				else if( Window->createGameWindow(640, 480, 24, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R640x480, 24);   return true; }
				else if( Window->createGameWindow(640, 480, 16, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R640x480, 16);   return true; }				
			}
		default:
		case R1366x768:			
			if( CanWindow(1366,768) ) //Window->checkWindowedMode(1366,768)
			{
				if( Window->createGameWindow(1366, 768, 32, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1366x768, 32);   return true; }
				else if( Window->createGameWindow(1366, 768, 24, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1366x768, 24);   return true; }
				else if( Window->createGameWindow(1366, 768, 16, true, (const char*)AppTitle_Str.c_str() ))
				{	InitRenderer( R1366x768, 16);   return true; }
			}
            
	}	
	return false;
}

bool Application::TryToCreateGameWindow( int Width, int Height, int Mode)
{
	if( Window->createGameWindow(Width, Height, 32, false, (const char*)AppTitle_Str.c_str() ))
	{	InitRenderer( Mode, 32);   return true; }				
	//cleanup to ensure 1rst time loading issues shown in Core2Duo
	delete Window;
	Window = KPTK::createKWindow (Renderer,true);
	Window->setRequirements(2048,4*1024*1024,0);//we use 2048 atlas sheets
	Window->setFixedAspectRatio(ProfileSystem::Instance->ActiveProfile.IsWideScreen); //4:3 - no stretch on widescreen displays as we support upto 1366x768 res !!
				
	if( Window->createGameWindow(Width, Height, 24, false, (const char*)AppTitle_Str.c_str() ))
	{	InitRenderer( Mode, 24);   return true; }				
	//cleanup to ensure 1rst time loading issues shown in Core2Duo
	delete Window;
	Window = KPTK::createKWindow (Renderer,true);
	Window->setRequirements(2048,4*1024*1024,0);//we use 2048 atlas sheets
	Window->setFixedAspectRatio(ProfileSystem::Instance->ActiveProfile.IsWideScreen); //4:3 - no stretch on widescreen displays as we support upto 1366x768 res !!
		#ifdef K_ANDROID
	 		// Set device orientation for the game to K_LANDSCAPE or K_RLANDSCAPE at start
 		    Window->setOrientation (K_LANDSCAPE);
		#endif
	return false;
}

//Iterate through needed modes based on gamer's desktop dimensions!!
bool Application::CreateBestFullScreenDisplayMode(  )
{
	int Mode = ProfileSystem::Instance->ActiveProfile.Resolution;

    K_LOG("Checking Best full mode resolution possible ");
    K_LOG("BestFull : depth %d",(int)WindowDepth);
    K_LOG("BestFull : BPP %d",(int)ProfileSystem::Instance->ActiveProfile.BPP);
    K_LOG("BestFull : WINDOWED %d",(bool)ProfileSystem::Instance->ActiveProfile.Windowed);
	switch ( Mode )
	{
		case R1360x768:
			if( TryToCreateGameWindow(1360,768,R1360x768) ) return true;						
		case R1280x768:
			if( TryToCreateGameWindow(1280,768,R1280x768) ) return true;			
		case R1280x720:
			if( TryToCreateGameWindow(1280,720,R1280x720) ) return true;			
		case R1024x768:
			if( TryToCreateGameWindow(1024,768,R1024x768) ) return true;			
		case R800x600:
			if( TryToCreateGameWindow(800,600,R800x600) ) return true;
            //Mac book pro res
        case R1280x800:
            if( TryToCreateGameWindow(1280,800,R1280x800) ) return true;
        case R1152x720:
            if( TryToCreateGameWindow(1152,720,R1152x720) ) return true;
        case R1024x640:
            if( TryToCreateGameWindow(1024,640,R1024x640) ) return true;
        case R1920x1080: //FullHD
            if( TryToCreateGameWindow(1920,1080,R1920x1080) ) return true;
		case R1920x1200: //FullHD+
            if( TryToCreateGameWindow(1920,1200,R1920x1200) ) return true;
        case R2560x1440: //QHD
            if( TryToCreateGameWindow(2560,1440,R2560x1440) ) return true;
        case R3840x2160: //4K
            if( TryToCreateGameWindow(3840,2160,R3840x2160) ) return true;
       
        case R7680x4320: //8K
            if( TryToCreateGameWindow(7680,4320,R7680x4320) ) return true;

        case R1600x900: //some stupid resolution
            if( TryToCreateGameWindow(1600,900,R1600x900) ) return true;
        case R1344x756: //some stupid resolution
            if( TryToCreateGameWindow(1344,756,R1344x756) ) return true;
       
		case R640x480:
			if( Window->createGameWindow(640, 480, 32, false, (const char*)AppTitle_Str.c_str() ))
			{	InitRenderer( R640x480, 32);   return true; }
			else if( Window->createGameWindow(640, 480, 24, false, (const char*)AppTitle_Str.c_str() ))
			{	InitRenderer( R640x480, 24);   return true; }
			else if( Window->createGameWindow(640, 480, 16, false, (const char*)AppTitle_Str.c_str() ))
			{	InitRenderer( R640x480, 16);   return true; }			
		default:
		case R1366x768:	
			if( TryToCreateGameWindow(1366,768,R1366x768) ) return true;				
	}	
	return false;
}

/////////////////////////////////////////////////
bool Application::CreateGameWindow()
{
	Window = NULL;
    /*
#ifdef K_iOS
    
    // Create game window
    Renderer = K_OPENGL;

    Window = KPTK::createKWindow (Renderer, true);   
    Window->createGameWindow (480, 320, 32, true, "", K_LANDSCAPE);
    Window->setPTKCallBack (myEventHandler);
    
    // Enable multitouch
    Window->enableMultiTouch (true);
    return true;
#endif
*/
	//	Renderer = K_DIRECTX9;  Defaulting it to OpenGL and if it is Win32, then to DirectX
 	   Renderer = K_OPENGL;
    //  Window = KPTK::createKWindow (Renderer,true); Sac: Why two time create windows, see below

	#ifdef K_WIN32
	{
	   Renderer = K_DIRECTX9; 
	   Window = KPTK::createKWindow (Renderer,true);
	   if( Window == NULL )
	   {
		   Renderer = K_DIRECTX;
		   Window = KPTK::createKWindow (Renderer,true);
	   }
	}
	#else //Xcode , Linux, Android, Iphone
	{
	   Renderer = K_OPENGL;
	   Window = KPTK::createKWindow (Renderer,true);
        
	}
	#endif
     // for linux using Default value
    WindowDepth = -1;

	#ifdef K_WIN32
	{
		MEMORYSTATUS	memStatus;
		GlobalMemoryStatus(&memStatus);		
		if( (int)(memStatus.dwAvailPhys/1048576.0f) > MAX_LOWEND_PEAKMEMORY )	
			WindowDepth = 32;
		else // 2 cases => GFInstance->IsForceTextures16Bit, < GFInstance->IsPartialCacheMem 
			WindowDepth = -1;//default window depth = Window->getWindowDepth();	16 bit window	
	}
	#endif
    
    #ifdef K_MACOS
            WindowDepth = 32;
    #endif
	
	bool validWindow = ( Window != NULL );
	bool validRes = false;
	if( validWindow )//Create valid display resolution!!
	{		
		Window->setRequirements(2048,4*1024*1024,0);//we use 2048 atlas sheets
		Window->setRectangleTexCap(true);
	
            
            #ifdef K_MACOS
                #ifdef _DEBUG //xCode debug mode handler!!
                    //Widescreen low resolutions!!
                    IsMacDebuggingOn = true;
                #endif
            #endif
            if( ProfileSystem::Instance->ActiveProfile.BPP == -1 )//one time init!!
			{
				ProfileSystem::Instance->ActiveProfile.IsWideScreen = true;//Window->isWidescreenAvailable();//Initialize if aspect ratio > 4:3!!
				DesktopHeight=(int) Window->getDesktopHeight();
				DesktopWidth=(int) Window->getDesktopWidth();
				ProfileSystem::Instance->ActiveProfile.Resolution=WhichDisplayMode(DesktopWidth,DesktopHeight);
				#ifdef K_MACOS
					#ifdef _DEBUG//xCode debug mode handler!!
						ProfileSystem::Instance->ActiveProfile.Windowed = true;
					#else
						ProfileSystem::Instance->ActiveProfile.Windowed = false;
					#endif
				#else
					ProfileSystem::Instance->ActiveProfile.Windowed = false;
				#endif
            }
			else
			{ //Sac: need to see what to do if the user has a new Resolution than saved one
				SetDisplayWidthnHeight(ProfileSystem::Instance->ActiveProfile.Resolution);
				long current_desktopWidth = Window->getDesktopWidth();
				long current_desktopHeight = Window->getDesktopHeight();
				if(current_desktopWidth != DesktopWidth || current_desktopHeight != DesktopHeight )
				{
					DesktopHeight=(int) current_desktopHeight;
					DesktopWidth=(int) current_desktopWidth;
					ProfileSystem::Instance->ActiveProfile.Resolution=WhichDisplayMode(current_desktopWidth,current_desktopHeight);
				}
			}
			
			/* Keep 4:3 ratio when going fullscreen */
			Window->setFixedAspectRatio(ProfileSystem::Instance->ActiveProfile.IsWideScreen); //4:3 - no stretch on widescreen displays as we support upto 1366x768 res !!

			#ifdef K_MACOS
				Window->enableFullscreenWindow (true); /* For fullscreen, use borderless window rather than capturing the display, so that system dialogs show */
			#endif
			//Forced Testing of different resolutions
			if(DebugResMode>0)
			{
				//if(DebugResMode<=eResolution::Resolution_Max)
				if(DebugResMode>=eResolution::Resolution_Max)
				{
					DebugResMode = R1366x768;
				}
				SetDisplayWidthnHeight(DebugResMode);
				ProfileSystem::Instance->ActiveProfile.Resolution = DebugResMode;
				ProfileSystem::Instance->ActiveProfile.BPP = 32;
			//	ProfileSystem::Instance->ActiveProfile.Windowed = true;
			}

			K_LOG("set Window desktopwidth %d desktopheight %d",DesktopWidth, DesktopHeight);	
			#ifdef K_ANDROID
				CurrentResolution.CanWindow = false;
			#else
				CurrentResolution.CanWindow = ProfileSystem::Instance->ActiveProfile.Windowed;
			#endif
            //Load application resolution settings
            if( ProfileSystem::Instance->ActiveProfile.BPP != -1 )
            {
                switch( ProfileSystem::Instance->ActiveProfile.Resolution )
                {
                     case R1360x768:
						//If lastrun_window_resolution can't be supported in current desktop resolution!!
						if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1360, 768) )
				            break;
				        if( Window->createGameWindow(1360, 768, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
						{
							InitRenderer( R1360x768, ProfileSystem::Instance->ActiveProfile.BPP );
							validRes = true;
						}
                        break;
					 case R1280x768:
						//If lastrun_window_resolution can't be supported in current desktop resolution!!
						if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1280, 768) )
				            break;
				        if( Window->createGameWindow(1280, 768, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
						{
							InitRenderer( R1280x768, ProfileSystem::Instance->ActiveProfile.BPP );
							validRes = true;
						}
                        break;
					case R1280x720:
						//If lastrun_window_resolution can't be supported in current desktop resolution!!
						if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1280, 720) )
				            break;
				        if( Window->createGameWindow(1280, 720, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
						{
							InitRenderer( R1280x720, ProfileSystem::Instance->ActiveProfile.BPP );
							validRes = true;
						}
                        break;
                    case R1280x800:  // Supported for mac
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1280, 800) )
                            break;
                        if( Window->createGameWindow(1280, 800, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R1280x800, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
                    case R1152x720:   // Supported for mac
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1152, 720) )
                            break;
                        if( Window->createGameWindow(1152, 720, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R1152x720, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
                    case R1024x640:   // Supported for mac
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1024, 640) )
                            break;
                        if( Window->createGameWindow(1024, 640, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R1024x640, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
                    case R1920x1080:   // Full HD
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1920, 1080) )
                            break;
                        if( Window->createGameWindow(1920, 1080, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R1920x1080, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
					case R1920x1200:   // Full HD+
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1920, 1200) )
                            break;
                        if( Window->createGameWindow(1920, 1200, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R1920x1200, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
                    case R2560x1440:   // QHD
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(2560, 1440) )
                            break;
                        if( Window->createGameWindow(2560, 1440, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R2560x1440, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
                    case R3840x2160:   // 4K
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(3840, 2160) )
                            break;
                        if( Window->createGameWindow(3840, 2160, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R3840x2160, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
						
                    case R7680x4320:   // 8K
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(7680, 4320) )
                            break;
                        if( Window->createGameWindow(7680, 4320, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R7680x4320, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
                    case R1600x900:   // some stupid resolution
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1600, 900) )
                            break;
                        if( Window->createGameWindow(1600, 900, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R1600x900, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
                    case R1344x756:   // some stupid resolution
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
                        if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1344, 756) )
                            break;
                        if( Window->createGameWindow(1344, 756, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
                        {
                            InitRenderer( R1344x756, ProfileSystem::Instance->ActiveProfile.BPP );
                            validRes = true;
                        }
                        break;
                       
                    case R1024x768:
						//If lastrun_window_resolution can't be supported in current desktop resolution!!
						if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1024, 768) )
				            break;
						if( Window->createGameWindow(1024, 768, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
						{
							InitRenderer( R1024x768, ProfileSystem::Instance->ActiveProfile.BPP );
							validRes = true;
						}
						
                        break;
                    case R800x600: 
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
						if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(800, 600) )
							break;
                        {
							if( DesktopWidth >= 1024 && DesktopHeight >= 768 )
								break;//better display mode seems possible!!
                            
							if( Window->createGameWindow(800, 600, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
							{ 
								InitRenderer( R800x600, ProfileSystem::Instance->ActiveProfile.BPP );
								validRes = true;
							}
                            
                            
						}
                        break;
                    case R640x480: 
                        //If lastrun_window_resolution can't be supported in current desktop resolution!!
						if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(640, 480) )						
							break;
						{
							if( DesktopWidth >= 800 && DesktopHeight >= 600 )
								break;//better display mode seems possible!!
							
							if( Window->createGameWindow(640,480, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
							{ 
								InitRenderer( R640x480, ProfileSystem::Instance->ActiveProfile.BPP );
								validRes = true;
							}
						}
						break;
					default:
					case R1366x768:
						//If lastrun_window_resolution can't be supported in current desktop resolution!!
						if( ProfileSystem::Instance->ActiveProfile.Windowed && !Window->checkWindowedMode(1366, 768) )
				            break;
				        if( Window->createGameWindow(1366, 768, ProfileSystem::Instance->ActiveProfile.BPP, ProfileSystem::Instance->ActiveProfile.Windowed, (const char*)AppTitle_Str.c_str() ))
						{
							InitRenderer( R1366x768, ProfileSystem::Instance->ActiveProfile.BPP );
							validRes = true;
						}
                        break;
						
                }
            }
                        
            if( !validRes )
            {
				if( ProfileSystem::Instance->ActiveProfile.Windowed || IsMacDebuggingOn ) //Try to create windowed mode!!
                {
                    validRes = CreateBestWindowedDisplayMode(); //Window
                    if( !validRes )
                    {
                        CurrentResolution.CanWindow = false;
						ProfileSystem::Instance->ActiveProfile.Windowed = false;
                        validRes = CreateBestFullScreenDisplayMode(); //Fullscreen			
                    }
                }
                else
                    validRes = CreateBestFullScreenDisplayMode(); //Fullscreen
            }

			K_LOG("Real desktop windowwidth %d windowheight %d", Window->getActualWindowWidth(), Window->getActualWindowHeight());
	

		
	}
	//		KMiscTools::messageBox((const char*)StringTable::getStr("ErrDisplayAdaptorTitle").c_str(), (const char*)StringTable::getStr("ErrDisplayAdaptorTitle").c_str());

	if( !validWindow || !validRes ) //Can't create window or resolution
	{
		//Window Creation Failed
		delete Window;
		#ifdef K_WIN32
			Window = KPTK::createKWindow(K_DIRECTX, false);
		#else
			Window = KPTK::createKWindow(K_OPENGL, false);
		#endif
		Window->createGameWindow(320, 240, 24, true, (const char*)AppTitle_Str.c_str());		
		
		long maxWidth = 0, maxHeight = 0;
		Window->getMaxTextureSize(maxWidth,maxHeight);
  
		if( maxWidth < 2048 || maxHeight < 2048  )
		{
			#ifdef K_WIN32
				KMiscTools::messageBox((const char*)FileSystem::ws2s(DADPTR_ER_WStr).c_str(),(const char*)FileSystem::ws2s(GWRNG_1_WStr).c_str() );			
			#else			
				KMiscTools::messageBox((const char*)DADPTR_ER_WStr.c_str(),(const char*)GWRNG_1_WStr.c_str() );			
			#endif
            exit(0);
		}
		else if( !Window->getRectangleTexCap() )
		{
			#ifdef K_WIN32
				KMiscTools::messageBox((const char*)FileSystem::ws2s(DADPTR_ER_WStr).c_str(),(const char*)FileSystem::ws2s(GWRNG_2_WStr).c_str() );			
			#else			
				KMiscTools::messageBox((const char*)DADPTR_ER_WStr.c_str(),(const char*)GWRNG_2_WStr.c_str() );			
			#endif
			exit(0);
		}
		else
		{
			#ifdef K_WIN32
				KMiscTools::messageBox((const char*)FileSystem::ws2s(AppTitle_Str).c_str(),(const char*)FileSystem::ws2s(GWRNG_3_WStr).c_str() );			
			#else			
				KMiscTools::messageBox((const char*)AppTitle_Str.c_str(),(const char*)GWRNG_3_WStr.c_str() );			
			#endif
			exit(0);
		}		
	}
	else
	{		
		#ifdef K_ANDROID
			Window->enableMultiTouch(true); // the game doesnt need Multi touch, fact is Multi touch created unnesseary artificats for this game
			/* Catch the 'back' key */
			KSysAndroid::catchBackKey (true);
			/* Cancel local notifications */
			KSysAndroid::cancelLocalNotifications();	
		#endif
		#ifdef K_iOS
			Window->enableMultiTouch(true);
		#endif
		Window->setMaxFrameRate(0); // enable VSync Drawing
		//Window->setMaxFrameRate(60); //FPS

		Window->setClipRect(true, 0, 0, (float)CurrentResolution.Width, (float)CurrentResolution.Height);
		Window->setPTKCallBack (myEventHandler);
		
   
		Control::Input->SetProjectComp( CurrentResolution.Width, CurrentResolution.Height );

		return true;
	}
}

Application::~Application()
{
	GFApp = NULL;//Global access pointer
	//GFInstance = NULL;//Global access pointer

	if( Window != NULL )
	{
		Window->setPTKCallBack(NULL);							 
		Window->terminate();
		delete Window;
		Window = NULL;
	}

	//Console::Destroy();
	if( Quad != NULL )
	{
		delete Quad;
		Quad = NULL;
	}

	if (pauseBGMask != NULL)
	{
		delete pauseBGMask;
		pauseBGMask = NULL;
	}

	if( FPSText != NULL )
	{
		delete FPSText;
		FPSText = NULL;
	}

	#ifdef K_ANDROID
		if( gameVersionText != NULL )
		{
			delete gameVersionText;
			gameVersionText = NULL;
		}
	#endif
	if( lpFont20 != NULL )
		delete lpFont20;
	if( lpFont21 != NULL )
		delete lpFont21;
	if( lpFont22 != NULL )
		delete lpFont22;
	if( lpFontSmall != NULL )
		delete lpFontSmall;
	if( lpInvPopFont != NULL )
		delete lpInvPopFont;
	if (lpFontDiary != NULL)
		delete lpFontDiary;
	if (lpFontDropShadow != NULL)
		delete lpFontDropShadow;
	
}

//Main loop for the application
//Decoupled logic/render
void Application::Process()
{
	while(AppActive)
	{
		if( Window->hasFocus() )
		{
            #ifdef K_MACOS
                isCommandRightPressed = false;
                isCommandLeftPressed = false;
            #endif
			AwardManager::GetInstance()->UpdateElapsedGamePlayTime();
			//Handle taskbar restore audio turn on
			if( !GFApp->IsWndFocus && GFApp->IsTaskBarMinimize )
			{
			  GFApp->IsTaskBarMinimize = false;
			  GFApp->IsWndFocus = true;
			  if( Control::Audio->Silent )
				  if( GFInstance != NULL )
					  if( GFInstance->IsPakResLoadDone )
						  Control::Audio->EndSilence();			 
			  DrawThanks(false);
			}

#ifdef TOUCH_UI
			PinchToZoom();
			TouchRipple();

			if(KInput::getLeftButtonState())
			{
				if(KInput::getMouseY() > 640)
				{
					if(GFHud)
					{
						if(GFHud->InvBoxCtrl)
						{
							GFHud->InvBoxCtrl->NavigateText->Text = "";
							GFHud->InvBoxCtrl->travelArea = -1;
							GFHud->InvBoxCtrl->navTapped = false;
							GFHud->InvBoxCtrl->Navigate->Show(false);
							GFHud->InvBoxCtrl->NavArrow->Show(false);
						}
					}
				}
			}

#endif

			if(IsDebugMode)
			{
				Control::MasterTmr->Update();
				LogicProcess(true);			
				Control::MasterTmr->StartRenderCycle();//Render Phase
				PostUpdate();
				Control::MasterTmr->EndRenderCycle(); 
			}
			else
			{
				Control::MasterTmr->Update();//drops fps calculation
				LogicProcess(true);		 	 //drops process start/end cycles		
				//drops render start cycles
				PostUpdate();				 //drops fps/status display
				//drops render end cycles
			}

			#if (!defined K_WIN32 || defined _USE_STACK_WALKER)
				::signal(SIGSEGV, &my_signal_handler);
				::signal(SIGABRT, &my_signal_handler);
			#endif
		}
		else
		{

#ifndef DEBUG
			//Handle taskbar minimize audio turn off
			if( GFApp->IsWndFocus && !GFApp->IsTaskBarMinimize )
			{
				GFApp->IsTaskBarMinimize = true;
				GFApp->IsWndFocus = false;
				if( GFInstance != NULL )
					if( GFInstance->IsPakResLoadDone )
					{
						// Window lost focus; suspend audio 
						if( !Control::Audio->Silent )
							 Control::Audio->Silence();							
					}

				GFApp->Window->minimize();	
			}

#endif
			DrawThanks(true);
			KMiscTools::pause(20,true);			
		}
	}
	
	HandleForcedWndCloseDeadlocks();//Before saving profile make needed corrections!
	
	//if(IsSurveyBuild)	//Only for BIGFISH
	//{
	//	//GFInstance->CreateThanks();				
	//	//DrawThanks();
	//	SAVEALLPROFILES();
	//	//DrawThanks();//draw to back buffer to make backbuffer = frontbuffer while launching!!
	//	
	//
	//	//KMiscTools::pause(2000,true);//forced
	//	LaunchSurvey();
	//}
	//else
	{
        if( GFInstance != NULL )
        {
        if( GFInstance->Flow == fHOLevel )//Handle alt+f4 forced close invpopup save state!!
			if( GFHud != NULL )
				if( GFHud->InvBoxCtrl != NULL )
					GFHud->InvBoxCtrl->AddPendingInvToInvBox(0);
        }

		//Disabled save at force close to avoid save data corruption 
		//SAVEALLPROFILES();      
		
	}

	exit(0);//close streams & terminate
}

void Application::HandleForcedWndCloseDeadlocks()
{
	switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
	{
		default:
		case Level_1:	//If quitting the scene inbetween raja talk, skip this dialog to avoid deadlock case - 
						// (player gave letter to raja already - so next time he revisits gamer must be able to provide letter again!!)
						if( !ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames )
						{
								//picked letter and already gave to raja!!
								if( ProfileSystem::Instance->ActiveProfile.L1S1_IsInvLetterCreekTaken && 
										!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_CreekLetter] )
								{
									//add this object to end of inv array!!
									ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_CreekLetter] = true;				
									ProfileSystem::Instance->ActiveProfile.InvObjects[ProfileSystem::Instance->ActiveProfile.InvObjectCnt].InvId = eInv_L1S1_CreekLetter;
									ProfileSystem::Instance->ActiveProfile.InvObjects[ProfileSystem::Instance->ActiveProfile.InvObjectCnt].nPiecePicked = 1;
									ProfileSystem::Instance->ActiveProfile.InvObjects[ProfileSystem::Instance->ActiveProfile.InvObjectCnt].MaxPieces = 1;
									ProfileSystem::Instance->ActiveProfile.InvObjectCnt++;
								}
								
						}
					 break;
		case Level_2:break;			
		case Level_3:break;		
		case Level_4:break;		
		case Level_5:break;	
		case Level_6:break;
		case Level_7:break;	
		case Level_8:break;
	}
	
}

void Application::LaunchSurvey()
{
	//Load from ini........
	KIniReader *iniReader;
	iniReader = new KIniReader ;
	iniReader->setIni(KMiscTools::makeFilePath("config.ini")) ;	
	char lpszURL[200]; 
	if( iniReader->getString("SURVEY","URL",lpszURL,200) )
	{
		K_LOG(lpszURL);
		KMiscTools::launchURL(lpszURL,true);//when launches app will switch back-front buffers!!		
	}
	delete iniReader;
}

void Application::PauseForPurchase(bool show){ // Draw thanks is private, so Im creating a public stub function to call the pause when purchase is initiate.
	DrawThanks(show);
}

void Application::DrawThanks(bool show)
{
	if( GFInstance->PausedImage == NULL)
	{
		return;
	}
	const CPoint CamPos(0,0,0);
	GFInstance->PausedImage->Show(show);
	//Window->setWorldView(ProjX, ProjY, 0.0f, ProjZoom, true);//clear prev scene
	if(show)
	{	
		pauseBGMask->drawRect(0, 0, (GFApp->CurrentResolution.Width), (GFApp->CurrentResolution.Height), 0,0,0, 0.5f);
		GFInstance->PausedImage->Render(&CamPos);
	}
	Window->flipBackBuffer();//no edge correction needed
}


void Application::LogicProcess(bool processControl)
{
	
	int CurrentDMS = (int)Control::MasterTmr->RealDMS;
	int LogicDMS;	

	if(IsDebugMode)
	{
		Control::MasterTmr->StartLogicCycle();
	}	

	while(CurrentDMS > 0 )
	{
		if(SingleUpdate == false)
			//Calculate our logic delta
			//Sac: Changed min to g_min
			LogicDMS = g_min(CurrentDMS, SynchLogicRate);
		else
			LogicDMS = CurrentDMS;
		Control::SetLogicRate(LogicDMS);
	
		//Logic Updates
		ApplicationUpdate();

		if( processControl )
			Control::Process();

		//Decrement our real delta
		if(SingleUpdate == false)
			CurrentDMS -= SynchLogicRate;	
		else
			break;
	}

	if(IsDebugMode)
	{
		Control::MasterTmr->EndLogicCycle();
	}	
}

void Application::ProcessMessage(sMessage message)
{
	//We have a special interest in the application exit protocol.
	if( message.Protocol == ApplicationExit )
		AppActive = false;
	else
		Control::ProcessMessage(message);
}

void Application::ApplicationUpdate()
{
	if( Window->isQuit() )
		AppActive = false;
	else
	{
		Control::Input->Update();
		CInteractiveObjectList::GetInstance()->Update();
        CPoint pos = Control::Input->Mpos();
		if( pos.x > (float)(MAX_DISPLAY_WIDTH-CURSOR_WIDTH) )
            pos.x = (float)(MAX_DISPLAY_WIDTH-CURSOR_WIDTH);
        else if( pos.x < 2 )
            pos.x = 2;
       if( pos.y > (float)(MAX_DISPLAY_HEIGHT) )
            pos.y = (float)(MAX_DISPLAY_HEIGHT);
		else if( pos.y < 3 )
            pos.y = 3;
        

		Cursor::SetPos(&pos);

		UpdateList::Instance->Update(Control::LogicRate);		
	}
}



void Application::CreateFPS()
{
	FPSText = new CBitmapText();
	FPSText->SetZ(5000);//above all
	FPSText->SetAlignment(Align_Center);
	
	const CPoint FPSTextPos(float(MAX_DISPLAY_WIDTH/2.0f), 20.0f, 0.0f);
	FPSText->SetPos(&FPSTextPos);		
	FPSText->LoadFont(eFont_Italic24);		
	FPSText->SetText("FPS");
	
	FPSText->SetScale(1.0f);
	FPSText->SetColor(1.0f, 0.98f, 0.21f);//yellow
	FPSText->SetAlpha(1.0f);
	FPSText->Show(true);	
}


	#ifdef K_ANDROID
		void Application::CreateGameVersionText()
		{
			gameVersionText = new CBitmapText();
			gameVersionText->SetZ(5000);//above all
			gameVersionText->SetAlignment(Align_Right);
			
			const CPoint gameVersionTextPos(1300.0f, 750.0f, 0.0f);
			gameVersionText->SetPos(&gameVersionTextPos);		
			gameVersionText->LoadFont(eFont_Italic24);
			gameVersionText->SetText(std::string("v ")+gameVersionName);
			
			gameVersionText->SetScale(1.0f);
			gameVersionText->SetColor(1.0f, 0.98f, 0.21f);//yellow
			gameVersionText->SetAlpha(1.0f);
			gameVersionText->Show(true);	
		}
	#endif

bool Application::ToggleFullscreen()
{
    if( Application::Instance->IsMacDebuggingOn )
        return false;

	#ifdef TOUCH_UI //We dont allow user to toggle from Fullscreen 
        return false;
	#endif
	/*
	When window is changed modes, the scene rendertarget seems lost.
	So whenever window is minimized and got focus, we reload the graphic
	*/
	GFInstance->ReloadSceneGraphic();

	if( ProfileSystem::Instance->ActiveProfile.Windowed )//Fullscreen can be done always!!
	{
		switch( ProfileSystem::Instance->ActiveProfile.Resolution )
		{
			//Best height possible is 768 during fullscreen!! No graphic quality change due to updating!!
            case R3840x2160:   // 4K
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;
           case R2560x1440:   // QHD or 2K
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;
			 case R1920x1200:   // Full HD+
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;
	    	case R1920x1080:   // Full HD
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;
			case R1366x768://always fullscreen
					ProfileSystem::Instance->ActiveProfile.Windowed = false;
					Instance->Window->toggleFullScreen(true);		
					return true;					
            case R1360x768://always fullscreen
					ProfileSystem::Instance->ActiveProfile.Windowed = false;
					Instance->Window->toggleFullScreen(true);		
					return true;	
			case R1280x800:  // Supported for mac
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;			
		    case R1280x768://always fullscreen
					ProfileSystem::Instance->ActiveProfile.Windowed = false;
					Instance->Window->toggleFullScreen(true);		
					return true;					
			 case R1280x720://always fullscreen
					ProfileSystem::Instance->ActiveProfile.Windowed = false;
					Instance->Window->toggleFullScreen(true);		
					return true;					
			case R1024x768://always fullscreen
					ProfileSystem::Instance->ActiveProfile.Windowed = false;
					Instance->Window->toggleFullScreen(true);		
					return true;
            case R1152x720:   // Supported for mac
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;
            case R1024x640:   // Supported for mac
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;
           
            case R7680x4320:   // 8K
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;
            case R1600x900:   // some stupid resolution
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;
            case R1344x756:   // some stupid resolution
                ProfileSystem::Instance->ActiveProfile.Windowed = false;
                Instance->Window->toggleFullScreen(true);
                break;
			//always try to upgrade regardless of dekstop size!! 
			case R800x600:
			case R640x480:				    
					GFInstance->IsUpgradeInfoPopup = true;
				    GFInstance->ActivateGFInfo("Ok","Cancel");//Upgrade							 
					return true;						
		}
	}
	else //fullscreen -> switch to low res windowed mode if needed
	{			
		if( Instance->CanWindow(CurrentResolution.Width,CurrentResolution.Height) )		
		{
			ProfileSystem::Instance->ActiveProfile.Windowed = true;
			Instance->Window->toggleFullScreen(false);		

			return true;		
		}
		else
		{
			switch( ProfileSystem::Instance->ActiveProfile.Resolution )
			{
		       	case R3840x2160:   // 4K
                    if( Instance->DesktopWidth >= 3840 && Instance->DesktopHeight >= 2160 )
                        if( Instance->Window->checkWindowedMode(2560,1440) )
                        {
                            GFInstance->Is3840x2160GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }
              	case R2560x1440:   // QHD
                    if( Instance->DesktopWidth >= 2560 && Instance->DesktopHeight >= 1440 )
                        if( Instance->Window->checkWindowedMode(1920,1200) )
                        {
                            GFInstance->Is2560x1440GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }
				case R1920x1200:   // Full HD+
                    if( Instance->DesktopWidth >= 1920 && Instance->DesktopHeight >= 1200 )
                        if( Instance->Window->checkWindowedMode(1920,1080) )
                        {
                            GFInstance->Is1920x1200GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }
                case R1920x1080:   // Full HD
                    if( Instance->DesktopWidth >= 1920 && Instance->DesktopHeight >= 1080 )
                        if( Instance->Window->checkWindowedMode(1366,768) )
                        {
                            GFInstance->Is1920x1080GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }
				case R1360x768:
					 if( Instance->DesktopWidth >= 1360 && Instance->DesktopHeight >= 768 )
						  if( Instance->Window->checkWindowedMode(1280,768) )		
						  {
							 GFInstance->Is1280x768GFInfoPopup = true;
							 GFInstance->ActivateGFInfo("Ok","Cancel");//degrade							 
							 return true;							 
						  }
				case R1280x768:
					 if( Instance->DesktopWidth >= 1280 && Instance->DesktopHeight >= 768 )
						  if( Instance->Window->checkWindowedMode(1280,720) )		
						  {
							 GFInstance->Is1280x720GFInfoPopup = true;
							 GFInstance->ActivateGFInfo("Ok","Cancel");//degrade							 
							 return true;							 
						  }
                case R1280x800:  // Supported for mac
                    if( Instance->DesktopWidth >= 1280 && Instance->DesktopHeight >= 800 )
                        if( Instance->Window->checkWindowedMode(1280,720) )
                        {
                            GFInstance->Is1280x800GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }
				case R1280x720:
					 if( Instance->DesktopWidth >= 1280 && Instance->DesktopHeight >= 720 )
						  if( Instance->Window->checkWindowedMode(1024,768) )		
						  {
							 GFInstance->Is1024x768GFInfoPopup = true;
							 GFInstance->ActivateGFInfo("Ok","Cancel");//degrade							 
							 return true;							 
						  }
                case R1152x720:   // Supported for mac
                    if( Instance->DesktopWidth >= 1152 && Instance->DesktopHeight >= 720 )
                        if( Instance->Window->checkWindowedMode(800,600) )
                        {
                            GFInstance->Is1152x720GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }

                case R1024x768:
					 if( Instance->DesktopWidth >= 1024 && Instance->DesktopHeight >= 768 )
						  if( Instance->Window->checkWindowedMode(800,600) )		
						  {
							 GFInstance->Is800x600GFInfoPopup = true;
							 GFInstance->ActivateGFInfo("Ok","Cancel");//degrade							 
							 return true;							 
						  }
                case R1024x640:   // Supported for mac
                    if( Instance->DesktopWidth >= 1024 && Instance->DesktopHeight >= 640 )
                        if( Instance->Window->checkWindowedMode(800,600) )
                        {
                            GFInstance->Is1024x640GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }
 
              
			    case R7680x4320:   // 8K
                    if( Instance->DesktopWidth >= 7680 && Instance->DesktopHeight >= 4320 )
                        if( Instance->Window->checkWindowedMode(7680,4320) )
                        {
                            GFInstance->Is7680x4320GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }
                case R1600x900:   // Stupid res
                    if( Instance->DesktopWidth >= 1600 && Instance->DesktopHeight >= 900 )
                        if( Instance->Window->checkWindowedMode(1600,900) )
                        {
                            GFInstance->Is1600x900GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }
                case R1344x756:   // Stupid res
                    if( Instance->DesktopWidth >= 1344 && Instance->DesktopHeight >= 756 )
                        if( Instance->Window->checkWindowedMode(1344,756) )
                        {
                            GFInstance->Is1344x756GFInfoPopup = true;
                            GFInstance->ActivateGFInfo("Ok","Cancel");//degrade
                            return true;
                        }
                    //no break needed
				
                case R800x600 :
					if( Instance->DesktopWidth >= 800 && Instance->DesktopHeight >= 600 )
						  if( Instance->Window->checkWindowedMode(640,480) )
						  {
							  GFInstance->ActivateGFInfo("Ok","Cancel"); //degrade	
							  return true;
						  }
						  
				//can't be downgraded further!! - no reaction!!
				case R640x480:break;
				case R1366x768:
					 if( Instance->DesktopWidth >= 1366 && Instance->DesktopHeight >= 768 )
						  if( Instance->Window->checkWindowedMode(1360,768) )		
						  {
							 GFInstance->Is1360x768GFInfoPopup = true;
							 GFInstance->ActivateGFInfo("Ok","Cancel");//degrade							 
							 return true;							 
						  }
			}
		}
	}
	return false;
}

#define TITLEBAR_MAXHGHT 30
#define TASKBAR_WIN_MAXHGHT 30
#define TASKBAR_MAC_MAXHGHT 63
bool Application::CanWindow(int width, int height)
{	
	#ifdef TOUCH_UI
		return false;
	#endif
	//return (Window->checkWindowedMode(width,height));
	if (DesktopHeight <=768) // skip the process of checking the height if height of the desktop is less than 768
        return true;
    
	int WndXtraHght = TITLEBAR_MAXHGHT;
	//prepare to be windowed!!		
	#ifdef K_WIN32
		WndXtraHght += TASKBAR_WIN_MAXHGHT;
	#else
		WndXtraHght += TASKBAR_MAC_MAXHGHT;
	#endif
			

	return( DesktopWidth  >= width && 
			 //deduce titlebar xtraheight
			DesktopHeight >= (height+WndXtraHght)  );
}


//=========================
// Static Funcs
//=========================

void Application::InstantFadeNormal()
{
	Instance->FadeAlpha = 0;
	Instance->LogicProcess(false);
	Instance->PostUpdate(true);
}
void Application::FadeToNormal(float FadeSpeed)
{
	float time = 1.0f;
	Instance->FadeAlpha = 1.0f;

	while(Instance->FadeAlpha > 0.0f)
	{
		Control::MasterTmr->Update();

		if( time > 0.0f )
		{
			time -= FadeSpeed * (float(Control::MasterTmr->RealDMS) / 1000.0f);
		}
		else
		{
			Instance->FadeAlpha -= FadeSpeed * float(Control::MasterTmr->RealDMS) / 1000.0f;
		//	Instance->FadeAlpha = max(Instance->FadeAlpha, 0.0f);
		// Sac: max is too common to be used as overloaded Macro, so changing it to g_max
			Instance->FadeAlpha = g_max(Instance->FadeAlpha, 0.0f);
		}

		Instance->LogicProcess(false);
		Instance->PostUpdate(true);
	}

	Control::Input->ClearAll();
	Control::MasterTmr->ClearCycles();
}

void Application::FadeToBlack(float FadeSpeed)
{
	Instance->FadeAlpha = 0.0f;

	while(Instance->FadeAlpha < 1.0f)
	{
		Control::MasterTmr->Update();

		Instance->FadeAlpha += FadeSpeed * (float(Control::MasterTmr->RealDMS) / 1000.0f);
		Instance->FadeAlpha = g_min(Instance->FadeAlpha, 1.0f);
		//Sac: changed min to g_min
		Instance->LogicProcess(false);
		Instance->PostUpdate(true);
	}

	Control::Input->ClearAll();
}

void Application::StaticPostUpdate()
{
	Instance->PostUpdate();
}


void Application::UpdateAndWait( float time )
{
	while( time > 0.0f )
	{
		Control::MasterTmr->Update();
		time -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
		Instance->LogicProcess(false);
		Instance->PostUpdate(true);
	}

	Control::Input->ClearAll();
	Control::MasterTmr->ClearCycles();
}

void Application::SetInstanceAlpha(float Alpha)
{
	Instance->FadeAlpha = Alpha;
}

void Application::ClearBackBuffer()
{
	Instance->Window->setWorldView(Instance->ProjX, Instance->ProjY, 0.0f, Instance->ProjZoom, true);//clear
}

void Application::FlipBackBuffer()
{
	Instance->Window->flipBackBuffer();
}

void Application::PostUpdate(bool fade)
{
	/* If we have lost focus by the time we reach here from "Process" function */  
	
	Window->setWorldView(ProjX, ProjY, 0.0f, ProjZoom, false);//no clear as all scenes are full

	if(IsDebugMode)
	{
		CPoint mosPos = Control::Input->Mpos();
		std::string Status;
		Status = "("+MKSTR(mosPos.x)+","+MKSTR(mosPos.y)+")"+
			"FPS: "+MKSTR(Control::MasterTmr->FramesPerSecond)+
           " R: " + MKSTR(Instance->DesktopWidth) +"x"+MKSTR(Instance->DesktopHeight)+
            " AR: " + MKSTR(Window->getActualWindowWidth()) +"x"+MKSTR(Window->getActualWindowHeight())+
			" vdmem: " + MKSTR( (Window->getVidMem() - Window->getAvailVidMem())/1024.0f) +"MB "+
			" obj: " + MKSTR(CRenderList::Instance->VisObjSize())+"["+MKSTR(CRenderList::Instance->ObjSize()) +"]" +
			" tex: " + MKSTR( CTexFactory::Instance->TotalTexSize / 1048576.0f) + " MB" + debugMsg;
		FPSText->SetText(Status);

		if(Control::Input->Kpressed(K_VK_F4))
		{
			if(FPSText->Visible)
				FPSText->Show(false);
			else
				FPSText->Show(true);
			return;
		}

		#ifdef K_ANDROID
		    gameVersionText->SetText(std::string("v ")+gameVersionName); // show the Game's version.
		#endif
		// I want to know why Clipboard is used to copy the mouse position and it uses WIN32 clipboard	
		#ifdef K_WIN32
		{
			//Control C to copy mouse position to clipboard
			if( Control::Input->Kdown(K_VK_L_CONTROL) )
			{
				if( Control::Input->Kdown(K_VK_C) )
				{
					std::string clipdata = (MKSTR(mosPos.x)+","+MKSTR(mosPos.y)+" "); //Last Space is required since it drops last letter somehow
					//copy mouse co-ordinates to clipboard
					OpenClipboard(0);
					EmptyClipboard();	
					HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,clipdata.size());
					if (!hg){
						CloseClipboard();
						return;
					}
					memcpy(GlobalLock(hg),clipdata.c_str(),clipdata.size());
					GlobalUnlock(hg);
					SetClipboardData(CF_TEXT,hg);
					CloseClipboard();
					GlobalFree(hg);
				}
			}
		}
		#endif
	}

#ifdef K_ANDROID
	if(GFApp->forceSaveProfile)
	{
		ProfileSystem::Instance->WriteProfileSystemData();
		GFApp->forceSaveProfile = false;
	}
#endif
		
	
	CRenderList::Instance->Render();	

	#ifdef K_ANDROID
		KUIElement::blitAll (0);
	#endif

	if( fade )
		Quad->drawRect(0,0, 1366,768, 0, 0, 0, FadeAlpha);	
		

	DrawProjEdgeCorrection();

	/* Yield for a bit so that the game doesn't starve the cpu completely */
   KSysThread::sleep (0);
   
   Window->flipBackBuffer();
}

void Application::DrawProjEdgeCorrection()
{
	if( CurrentResolution.Width > 800 )//skip R800x600, R640X480
	{
		if( CurrentResolution.Width < 1024 )//Handle last pixel loss during projection!!
			Quad->drawRect(1023,0,1025,768,0,0,0,1);//rightmost bar
		else if( CurrentResolution.Height == 720 )
			Quad->drawRect(0,767,1366,769,0,0,0,1);//bottommost bar
	}
	else 
	{
		if( Window->getActualWindowWidth() == 1920 && Window->getActualWindowHeight() == 1079 )
		{
			Quad->drawRect(0,1078,1920,1079,0,0,0,1);//bottommost bar
		}
	}
        
}
void Application::ForcePostUpdate()
{
	Window->setWorldView(ProjX, ProjY, 0.0f, ProjZoom, false);

	CRenderList::Instance->Render();	
		
	DrawProjEdgeCorrection();

	Window->flipBackBuffer();
}

void Application::ForceRenderScnHudtoBB()
{
	Window->setWorldView(ProjX, ProjY, 0.0f, ProjZoom, true);
	IsTurnOffParticles = true;
	CRenderList::Instance->Render();
	IsTurnOffParticles = false;
}
void Application::ForceRenderScntoBB()//scn alone - turn off hud from rendering!!
{
	// bool IsJrnShowing = false; //Sac: Unused variable, so commented it
	bool IsMsgBannerShowing = false;
	//	bool IsMapButtonShowing = false; //Sac: Unused variable, so commented it
	//	bool IsHintButtonShowing = false; //Sac: Unused variable, so commented it
	bool IsInvBoxShowing = false;
	bool IsInvBoxItemsShowing[MAX_INVBOX_DISPLAYED_OBJECTS];
	bool IsInvBoxDragging = false;
	
	for(int i=0; i < MAX_INVBOX_DISPLAYED_OBJECTS; ++i)
		IsInvBoxItemsShowing[i] = false;
	if( GFHud )
	{
		//GFHud->ShowJournal(false);	
		
		//IsMapButtonShowing = GFHud->IsMapButtonShowing();

		IsMsgBannerShowing = GFHud->Elements[HE_MessageBanner]->Visible;
		if( IsMsgBannerShowing )
		{
			GFHud->Elements[HE_MessageBanner]->Show(false);
			GFHud->MessageText->Show(false);				
		}	
		//Turn off invbox from blur!!
		IsInvBoxShowing = GFHud->InvBoxCtrl->InvBox->Visible;
		if( IsInvBoxShowing )
		{
			GFHud->InvBoxCtrl->InvBox->Visible = false;
			for(int i=0; i < MAX_INVBOX_DISPLAYED_OBJECTS; i++)
				if( GFHud->InvBoxCtrl->InvBoxObj[i+ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj].HOObj != NULL )
				{
					IsInvBoxItemsShowing[i] = GFHud->InvBoxCtrl->InvBoxObj[i+ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj].HOObj->Visible;
					GFHud->InvBoxCtrl->InvBoxObj[i+ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj].HOObj->Visible = false;
				}
		}
		IsInvBoxDragging = GFHud->InvBoxCtrl->IsDragging;
		if( IsInvBoxDragging && GFHud->InvBoxCtrl->DragIndex != - 1 )		
			GFHud->InvBoxCtrl->InvBoxObj[GFHud->InvBoxCtrl->DragIndex].HOObj->Show(false);
			
	}
	
	Window->setWorldView(ProjX, ProjY, 0.0f, ProjZoom, true);
	IsTurnOffParticles = true;
	CRenderList::Instance->Render();
	IsTurnOffParticles = false;
	if( GFHud )
	{		
		//if( IsJrnShowing )
		//	GFHud->ShowJournal(true);
	
		if( IsMsgBannerShowing )
		{	GFHud->Elements[HE_MessageBanner]->Show(true);
			GFHud->MessageText->Show(true);					
		}
		if( IsInvBoxShowing )
		{
			GFHud->InvBoxCtrl->InvBox->Visible = true;
			for(int i=0; i < MAX_INVBOX_DISPLAYED_OBJECTS; i++)
				if( IsInvBoxItemsShowing[i] && GFHud->InvBoxCtrl->InvBoxObj[i+ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj].HOObj != NULL )				
					GFHud->InvBoxCtrl->InvBoxObj[i+ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj].HOObj->Visible = true;				
		}
		if( IsInvBoxDragging && GFHud->InvBoxCtrl->DragIndex != - 1 )		
			GFHud->InvBoxCtrl->InvBoxObj[GFHud->InvBoxCtrl->DragIndex].HOObj->Show(true);		
	}
}
#ifdef K_ANDROID
	bool Application::isUnlocked()
	{
#ifdef UNLOCKED
		return true;
#else
		return unlocked;
#endif
	}
	void Application::UnlockGame() // sets the game from Free mode to Full mode
	{
		unlocked = true;
		ProfileSystem::Instance->WriteProfileSystemData();
	}

	void Application::SetAndroidAchievement(int award_id)
	{
		JNIEnv *threadEnv = NULL;
		jmethodID methodId;
		androidGetJVM()->GetEnv((void **) &threadEnv, JNI_VERSION_1_4);
		methodId = threadEnv->GetMethodID(androidGetThreadPeerClass(), "SetAccomplishments",
										  "(I)V");
		if (methodId != NULL)
			threadEnv->CallVoidMethod(androidGetThreadCaller(), methodId, award_id);
		else
			K_LOG("Couldnt find the Valid Accomplisments unlocking function");
	}

	void Application::AnalyticsSkipPuzzle(int puzzleID)
	{
        JNIEnv *threadEnv = NULL;
        jmethodID methodId;
        androidGetJVM()->GetEnv((void **) &threadEnv, JNI_VERSION_1_4);
        methodId = threadEnv->GetMethodID(androidGetThreadPeerClass(), "AnalyticsSkipPuzzle",
                                          "(I)V");
        if (methodId != NULL)
            threadEnv->CallVoidMethod(androidGetThreadCaller(), methodId, puzzleID);
        else
			K_LOG("Couldnt find the the valid function");

	}

	void Application::UpdateUserHint(std::string data)
    {
        JNIEnv *threadEnv = NULL;
        jmethodID methodId;
        androidGetJVM()->GetEnv((void **) &threadEnv, JNI_VERSION_1_4);
        methodId = threadEnv->GetMethodID(androidGetThreadPeerClass(), "UpdateUserHint",
                                          "(Ljava/lang/String;)V");
        if (methodId != NULL)
            threadEnv->CallVoidMethod(androidGetThreadCaller(), methodId, threadEnv->NewStringUTF(data.c_str()));
        else
            K_LOG("Couldnt find the the valid function");
    }

	void Application::ShowInterstitialAD()
	{
		//PauseForPurchase(true); //call pause screen
		JNIEnv *threadEnv = NULL;
		jmethodID methodId;
		androidGetJVM()->GetEnv ((void **) &threadEnv, JNI_VERSION_1_4);
		methodId = threadEnv->GetMethodID (androidGetThreadPeerClass(), "showInterstitial","()V");
		if(methodId !=NULL)
			threadEnv->CallVoidMethod (androidGetThreadCaller(), methodId);
		else
			K_LOG("Couldnt find the the valid function to show ads");
	}

void Application::ShowRewardedAD()
{
	//PauseForPurchase(true); //call pause screen
	JNIEnv *threadEnv = NULL;
	jmethodID methodId;
	androidGetJVM()->GetEnv ((void **) &threadEnv, JNI_VERSION_1_4);
	methodId = threadEnv->GetMethodID (androidGetThreadPeerClass(), "showRewardedAD","()V");
	if(methodId !=NULL)
		threadEnv->CallVoidMethod (androidGetThreadCaller(), methodId);
	else
		K_LOG("Couldnt find the the valid function to show ads");
}

	void Application::ShowHintStore()
	{
		//PauseForPurchase(true); //call pause screen
		JNIEnv *threadEnv = NULL;
		jmethodID methodId;
		androidGetJVM()->GetEnv ((void **) &threadEnv, JNI_VERSION_1_4);
		methodId = threadEnv->GetMethodID (androidGetThreadPeerClass(), "ShowHintStore","()V");
		if(methodId !=NULL)
			threadEnv->CallVoidMethod (androidGetThreadCaller(), methodId);
		else
			K_LOG("Couldnt find the the valid function");
	}

void Application::ShowUnlockStore()
{
	//PauseForPurchase(true); //call pause screen
	JNIEnv *threadEnv = NULL;
	jmethodID methodId;
	androidGetJVM()->GetEnv ((void **) &threadEnv, JNI_VERSION_1_4);
	methodId = threadEnv->GetMethodID (androidGetThreadPeerClass(), "ShowUnlockGame","()V");
	if(methodId !=NULL)
		threadEnv->CallVoidMethod (androidGetThreadCaller(), methodId);
	else
		K_LOG("Couldnt find the the valid function");
}

void Application::FetchUserData()
{
	//PauseForPurchase(true); //call pause screen
	JNIEnv *threadEnv = NULL;
	jmethodID methodId;
	androidGetJVM()->GetEnv ((void **) &threadEnv, JNI_VERSION_1_4);
	methodId = threadEnv->GetMethodID (androidGetThreadPeerClass(), "FetchUserData","()V");
	if(methodId !=NULL)
		threadEnv->CallVoidMethod (androidGetThreadCaller(), methodId);
	else
		K_LOG("Couldnt find the the valid function");
}

void Application::SignIN()
{
	//PauseForPurchase(true); //call pause screen
	JNIEnv *threadEnv = NULL;
	jmethodID methodId;
	androidGetJVM()->GetEnv ((void **) &threadEnv, JNI_VERSION_1_4);
	methodId = threadEnv->GetMethodID (androidGetThreadPeerClass(), "SignInToAccount","()V");
	if(methodId !=NULL)
		threadEnv->CallVoidMethod (androidGetThreadCaller(), methodId);
	else
		K_LOG("Couldnt find the the valid function");
}

void Application::SignOUT()
{
	//PauseForPurchase(true); //call pause screen
	JNIEnv *threadEnv = NULL;
	jmethodID methodId;
	androidGetJVM()->GetEnv ((void **) &threadEnv, JNI_VERSION_1_4);
	methodId = threadEnv->GetMethodID (androidGetThreadPeerClass(), "SignOutAccount","()V");
	if(methodId !=NULL)
		threadEnv->CallVoidMethod (androidGetThreadCaller(), methodId);
	else
		K_LOG("Couldnt find the the valid function");
}
	void androidJniSetHints(JNIEnv* env, jobject obj, jstring data)
	{

		const char *_userInfo = env->GetStringUTFChars(data, NULL);

		if(GFApp->noOfHints > ProfileSystem::Instance->NoOfHints)
		{
			GFApp->noOfHints = ProfileSystem::Instance->NoOfHints;
		}

		GFApp->noOfHints += std::atoi(_userInfo);
		if(GFApp->noOfHints != -777)//offline
			ProfileSystem::Instance->NoOfHints = GFApp->noOfHints;
		else
		{
			GFApp->noOfHints = ProfileSystem::Instance->NoOfHints;
		}
		
		//ProfileSystem::Instance->WriteProfileSystemData();
		env->ReleaseStringUTFChars(data, _userInfo);
		//GFApp->UpdateUserHint(to_string(GFApp->noOfHints));
		
	}

	int androidJnigetNoOfHints(JNIEnv* env, jobject obj)
	{
		if(GFApp->noOfHints > ProfileSystem::Instance->NoOfHints)
		{
			GFApp->noOfHints = ProfileSystem::Instance->NoOfHints;
		}
		GFApp->forceSaveProfile = true;
		return (GFApp->noOfHints);
	}

	void androidJniUnlockGame(JNIEnv* env, jobject obj)
	{
		GFApp->unlocked = true;
		GFApp->forceSaveProfile = true;
	}
#endif

void Application::SetCutSceneVideoResParameters(int video_res)
{
	//By default we will have 1024x576 with 4:3 for cutscene videos to run
    switch (video_res)
	{
		case 480:
            // The current video SAR is 720x480, Aspect ratio DAR is 16:9, so the Pixel frame should be 854x480 with PAR of 1:1
            video_width = 720;
            video_height = 480;
            break;

        case 540:
            // The current video SAR is 960x540, Aspect ratio DAR is 16:9, so the Pixel frame should be 960x540 with PAR of 1:1
            video_width = 960;
            video_height = 540;
            break;

		case 576:
            // The current video SAR is 720x576, Aspect ratio DAR is 16:9, so the Pixel frame should be 1024x576
            video_width = 1024;
            video_height = 576;
			break;

        case 406:
            //  The current video SAR is 720x406, Aspect ratio DAR is 16:9, so the Pixel frame should be 722x406 with PAR of 406:405
            video_width = 722;
            video_height = 406;
            break;

		case 848:
            // If it is Android, we will change to the below as the current video res is 848x480 with 16:9
            video_width = 848;
            video_height = 480;
            break;

		case 1024:
		default:
		    //4:3
			video_width = 1024;
			video_height = 576;
			break;

	}

    video_aspectRatio = 1.0f;
    video_negativeHalfWidth = -((int)(video_width/2));
    video_negativeHalfHeight = -((int)(video_height/2));
    video_startX = (int)((MAX_DISPLAY_WIDTH - video_width)/2);   // Used in all cut scenes other than starting one
    video_startY = (int)((MAX_DISPLAY_HEIGHT - video_height)/2); // Used in all cut scenes other than starting one
}
#ifdef _ENABLE_CURL

	#define FROM    "<tuttifrutti.bugreport@gmail.com>"
	#define TO      "<tuttifrutti.bugreport@gmail.com>"
	#define CC      "<support@tuttifrutti.in>"
	#ifdef K_ANDROID
		#define FILENAME_LOG (char*)std::string(ProfileSystem::Instance->UserPath + "log.txt").c_str()
	#else
		#define FILENAME_LOG "log.txt"
	#endif
	#define FILENAME_SAV (char*)std::string(ProfileSystem::Instance->UserPath + MKSTR(ProfileSystem::Instance->ProfileSlot) + ".sav").c_str()
	#define FILENAME_DAT (char*)std::string(ProfileSystem::Instance->UserPath + "data.sav").c_str()

	static const int CHARS= 76;     //Sending 54 chararcters at a time with \r , \n and \0 it becomes 57
	static const int ADD_SIZE= 16;   // ADD_SIZE for TO,FROM,SUBJECT,CONTENT-TYPE,CONTENT-TRANSFER-ENCODING,CONETNT-DISPOSITION and \r\n
	static const int SEND_BUF_SIZE= 54;
	static char (*fileBuf)[CHARS] = NULL;
	
	struct fileBuf_upload_status
	{
	int lines_read;
	};

	std::string splitString(std::string text, int start, int end)
	{
		std::string result;
		end += start;
		for(int i = start; i < end; i++)
		{
			if(text[i] != '\0')
				result += text[i];
			else 
				break;
		}
		return result;
	}

	static const std::string base64_chars = 
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789+/";

	std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for(i = 0; (i <4) ; i++)
			ret += base64_chars[char_array_4[i]];
		i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
		char_array_3[j] = '\0';

		char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

		for (j = 0; (j < i + 1); j++)
		ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
		ret += '=';

	}

	return ret;

	}
	
	size_t read_file()
	{
			size_t len(0),buffer_size(0);
			
			std::string log_encoded_buf ;		
			std::string sav_encoded_buf ;
			std::string dat_encoded_buf ;
			std::string sub_encoded_buf;		

			int message_rows = (int)(GFApp->fromUserEmail.length()/72);

			std::ifstream logFILE;
			logFILE.open(FILENAME_LOG);
			std::stringstream logfileStream;
			logfileStream << logFILE.rdbuf();

			log_encoded_buf = base64_encode((unsigned char*)logfileStream.str().c_str(), logfileStream.str().length());
			int log_rows = (int)(log_encoded_buf.length()/72);

			std::ifstream savFILE;
			savFILE.open(FILENAME_SAV);
			std::stringstream savefileStream;
			savefileStream << savFILE.rdbuf();

			sav_encoded_buf = base64_encode((unsigned char*)savefileStream.str().c_str(), savefileStream.str().length());
			int sav_rows = (int)(sav_encoded_buf.length()/72);

			std::ifstream datFILE;
			datFILE.open(FILENAME_DAT);
			std::stringstream datfileStream;
			datfileStream << datFILE.rdbuf();

			dat_encoded_buf = base64_encode((unsigned char*)datfileStream.str().c_str(), datfileStream.str().length());
			int dat_rows = (int)(dat_encoded_buf.length()/72);

			//Create structure of email to be sent
			fileBuf = new char[ADD_SIZE + message_rows + log_rows + sav_rows + dat_rows + 30][CHARS];

			std::strcpy(fileBuf[len++],"To: " TO "\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			std::strcpy(fileBuf[len++],"From: " FROM "\r\n");
			buffer_size += strlen(fileBuf[len-1]);
            #if defined (K_ANDROID)
             #if defined(_UNLOCKED_FOR_TEST_BUILD)
			    std::strcpy(fileBuf[len++],"Subject: Darkarta Unlocked  UserData Files\r\n");
			    #elif defined(_AMAZON_BUILD)
                std::strcpy(fileBuf[len++],"Subject: Darkarta Amazon UserData Files\r\n");
                #else
                std::strcpy(fileBuf[len++],"Subject: Darkarta Android UserData Files\r\n");
                #endif
            #else
			    std::strcpy(fileBuf[len++],"Subject: Darkarta Win32 UserData Files\r\n");
            #endif
			buffer_size += strlen(fileBuf[len-1]);		

			strcpy(fileBuf[len++],"Content-Type: multipart/mixed;\r\n boundary=\"XXXXXMyBoundry\"\r\n");
			buffer_size += strlen(fileBuf[len-1]);

			strcpy(fileBuf[len++],"Mime-Version: 1.0\r\n\r\n");
			buffer_size += strlen(fileBuf[len-1]);

			strcpy(fileBuf[len++],"This is a multi-part message in MIME format.\r\n\r\n");
			buffer_size += strlen(fileBuf[len-1]);

			strcpy(fileBuf[len++],"--XXXXXMyBoundry\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			
			strcpy(fileBuf[len++],"Content-Type: text/plain; charset=\"UTF-8\"\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			
			strcpy(fileBuf[len++],"Content-Transfer-Encoding: quoted-printable\r\n\r\n");
			buffer_size += strlen(fileBuf[len-1]);

			int pos = 0;

			for(int i = 0; i <= message_rows; i++)
			{
				sub_encoded_buf = "";
				sub_encoded_buf = splitString(GFApp->fromUserEmail, pos*72, 72);
				sub_encoded_buf += "\r\n";
				strcpy(fileBuf[len++], sub_encoded_buf.c_str());
				buffer_size += sub_encoded_buf.size();
				pos++;
			} 

			strcpy(fileBuf[len++],"--XXXXXMyBoundry\r\n");
			buffer_size += strlen(fileBuf[len-1]);     
			strcpy(fileBuf[len++],"Content-Type: application/x-msdownload; name=\"log.txt\"\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			strcpy(fileBuf[len++],"Content-Transfer-Encoding: base64\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			strcpy(fileBuf[len++],"Content-Disposition: attachment; filename=\"log.txt\"\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			strcpy(fileBuf[len++],"\r\n");
			buffer_size += strlen(fileBuf[len-1]);

			pos = 0;		

			for(int i = 0; i <= log_rows-1; i++)
			{
				sub_encoded_buf = "";
				sub_encoded_buf = splitString(log_encoded_buf, pos*72,72);
				sub_encoded_buf += "\r\n";                
				strcpy(fileBuf[len++], sub_encoded_buf.c_str());
				buffer_size += sub_encoded_buf.size();
				pos++;
			} 		
			
			strcpy(fileBuf[len++],"--XXXXXMyBoundry\r\n");
			buffer_size += strlen(fileBuf[len-1]);     		
			std::string temp = "Content-Type: application/x-msdownload; name=\""+ MKSTR(ProfileSystem::Instance->ProfileSlot) + ".sav" + "\"\r\n";
			std::strcpy(fileBuf[len++],temp.c_str());
			buffer_size += strlen(fileBuf[len-1]);
			std::strcpy(fileBuf[len++],"Content-Transfer-Encoding: base64\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			temp = "Content-Disposition: attachment; filename=\"" + MKSTR(ProfileSystem::Instance->ProfileSlot) + ".sav"+ "\"\r\n";
			std::strcpy(fileBuf[len++],temp.c_str());
			buffer_size += strlen(fileBuf[len-1]);
			std::strcpy(fileBuf[len++],"\r\n");
			buffer_size += strlen(fileBuf[len-1]);

			pos = 0;  

			for(int i = 0; i <= sav_rows-1; i++)
			{
				sub_encoded_buf = "";
				sub_encoded_buf = splitString(sav_encoded_buf, pos*72,72);
				sub_encoded_buf += "\r\n";      
				strcpy(fileBuf[len++], sub_encoded_buf.c_str());
				buffer_size += sub_encoded_buf.size();
				pos++;                                           
			} 

			strcpy(fileBuf[len++],"--XXXXXMyBoundry\r\n");
			buffer_size += strlen(fileBuf[len-1]);     
			strcpy(fileBuf[len++],"Content-Type: application/x-msdownload; name=\"data.sav\"\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			strcpy(fileBuf[len++],"Content-Transfer-Encoding: base64\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			strcpy(fileBuf[len++],"Content-Disposition: attachment; filename=\"data.sav\"\r\n");
			buffer_size += strlen(fileBuf[len-1]);
			strcpy(fileBuf[len++],"\r\n");
			buffer_size += strlen(fileBuf[len-1]);

			pos = 0;	

			for(int i = 0; i <= dat_rows-1; i++)
			{
				sub_encoded_buf = "";
				sub_encoded_buf = splitString(dat_encoded_buf, pos*72,72);
				sub_encoded_buf += "\r\n";                
				cout<<sub_encoded_buf;
				strcpy(fileBuf[len++], sub_encoded_buf.c_str());  
				buffer_size += sub_encoded_buf.size();            
				pos++;                                           
			} 

			strcpy(fileBuf[len++],"\r\n--XXXXXMyBoundry--\r\n");
			buffer_size += strlen(fileBuf[len-1]);

			//SAFE_DELETE(b64encode);
			return buffer_size;
	}

	static size_t fileBuf_source(void *ptr, size_t size, size_t nmemb, void *userp)
	{
			struct fileBuf_upload_status *upload_ctx = (struct fileBuf_upload_status *)userp;
			const char *fdata;
	
			if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1))
			{
					return 0;
			}
	
			fdata = fileBuf[upload_ctx->lines_read];
	
			if(strcmp(fdata,""))
			{
					size_t len = strlen(fdata);
					memcpy(ptr, fdata, len);
					upload_ctx->lines_read++;
					return len;
			}
			return 0;
	}

	void Application::SendReportToDev()
	{
		CURL *curl;
	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	struct fileBuf_upload_status file_upload_ctx;
	size_t file_size(0);
	
	file_upload_ctx.lines_read = 0;
	
	curl = curl_easy_init();
	file_size = read_file();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_USERNAME, "tuttifrutti.bugreport@gmail.com");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "Mangoorange4$");
		curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
		curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);
		recipients = curl_slist_append(recipients, TO);
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE, file_size);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, fileBuf_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &file_upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //Dont display Curl Connection data Change 1L to 0
	
		res = curl_easy_perform(curl);
	
		if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}
}
#endif

#ifdef TOUCH_UI
void Application::PinchToZoom()
{
    GFApp->touchPosX = (KInput::getMouseX() / GFApp->ProjZoom) + std::abs((GFApp->ProjX - GFApp->projXMax)/GFApp->ProjZoom);
    GFApp->touchPosY = (KInput::getMouseY() / GFApp->ProjZoom) + std::abs((GFApp->ProjY + GFApp->projYMax)/GFApp->ProjZoom);

    if(std::abs(KInput::getMouseX() - GFApp->touchPosX2) < 50)
        GFApp->zoomOriginDeltaX = KInput::getMouseX() - GFApp->touchPosX2;

    if(std::abs(KInput::getMouseY() - GFApp->touchPosY2) < 50)
        GFApp->zoomOriginDeltaY = KInput::getMouseY() - GFApp->touchPosY2;

    GFApp->touchPosX2 = KInput::getMouseX();
    GFApp->touchPosY2 = KInput::getMouseY();

    float tempZoom = (ProjZoom + (GFApp->touchZoomFactor));

    if(GFApp->touchVelocity > 0)
        GFApp->touchVelocity -= 0.5;
    else
        GFApp->touchVelocity = 0;


    //GFApp->debugMsg = std::to_string(ProfileSystem::Instance->ActiveProfile.CurrentArea);

    //Pinch-to-zoom Exceptions
    if(GFInstance->IsInGameMenuPopup || GFApp->pinchDisabled)
    {
        canZoom = false;
        ProjZoom = 1;
        ProjX = 0;
        ProjY = 0;
    } else
    {
        canZoom = true;
    }

    if(tempZoom >= 1 && canZoom)
    {
        if(tempZoom <= 2)
        {
            if(tempProjZoom == ProjZoom)
            {
                if(flingTimer < 26)
                    flingTimer += 1;
            }
            else
            {
                flingTimer = 0;
            }
            tempProjZoom = ProjZoom;
            ProjZoom = tempZoom;
            if(GFApp->canPan)
            {
                ProjX += (zoomOriginDeltaX);
                ProjY += (-zoomOriginDeltaY);

                //Fling Code
                if(GFApp->touchVelocity > 0 && flingTimer > 25)
                {
                    ProjX += (GFApp->flingX*GFApp->touchVelocity);
                    ProjY -= (GFApp->flingY*GFApp->touchVelocity);
                }
            }
        }
        else
        {
            ProjZoom = 2;
        }
    }
    else
	{
    	ProjZoom = 1;
		ProjX = 0;
		ProjY = 0;
	}

    //Clamp Projections
    float PZ2 = ProjZoom*ProjZoom;
    float PZ3 = ProjZoom*ProjZoom*ProjZoom;
    float offsetCurveX = (-1844 + 3443*ProjZoom - 1936*PZ2 + 334*PZ3) + ((ProjZoom - 1)*55);
    float offsetCurveY = (-1174 + 2188*ProjZoom - 1224*PZ2 + 212*PZ3);

    projXMax = (1366 - (1366 / ProjZoom));
    projYMax = (768 - (768 / ProjZoom));

    if(ProjZoom > 1 && ProjZoom < 2)
    {
        projXMax -= offsetCurveX;
        projYMax -= offsetCurveY;
    }

    if(ProjX > projXMax) ProjX = projXMax;
    if(ProjY < -projYMax) ProjY = -projYMax;
    if(ProjX < -projXMax) ProjX = -projXMax;
    if(ProjY > projYMax) ProjY = projYMax;
}

void Application::TouchRipple()
{
    for(int i = 0; i < 4; i++)
    {
        if (GFApp->touchRipple[i]->pSprite->GetFrame() >= 28)
        {
            GFApp->touchRipple[i]->Show(false);
        }
    }
    if(GFApp->ripple)
    {
        GFApp->ripple = false;
        GFApp->touchRipple[rippleIndex]->SetFrame(0);
        GFApp->touchRipple[rippleIndex]->PlayAnimation(0);
        GFApp->touchRipple[rippleIndex]->pSprite->SetAnimSpeed(0.7);
        GFApp->touchRipple[rippleIndex]->Show(true);
        GFApp->touchRipple[rippleIndex]->SetXY(GFApp->touchPosX, GFApp->touchPosY);

        if(rippleIndex < 3)
            rippleIndex++;
        else
            rippleIndex = 0;
        
        GFApp->touched = true;
    }
}
#endif


void Application::SetAssetLanguage(int id)
{

#ifdef _STEAM_BUILD
	std::string langConfigPath = std::string("lang.txt");
#else
#ifdef K_WIN32
		std::string langConfigPath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/Tuttifrutti_Games/lang.txt");
#else
	#ifdef K_ANDROID
		std::string langConfigPath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/lang.txt");
	#else
		std::string langConfigPath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/Tuttifrutti_Games/lang.txt");
	#endif
#endif
#endif
    CIOStream stream;
    if( !stream.Open(langConfigPath, File_Write))
    {
        K_LOG(std::string("File write access denied for location "+langConfigPath).c_str());
    }
    stream.WriteInt(id);
    stream.Close();
}

int Application::GetAssetLanguage()
{
    int id;
#ifdef _STEAM_BUILD
	std::string langConfigPath = std::string("lang.txt");
#else
#ifdef K_WIN32
	std::string langConfigPath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/Tuttifrutti_Games/lang.txt");
#else
#ifdef K_ANDROID
	std::string langConfigPath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/lang.txt");
#else
	std::string langConfigPath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/Tuttifrutti_Games/lang.txt");
#endif
#endif
#endif
	K_LOG(langConfigPath.c_str());
    CIOStream stream;
	std::fstream *pStream = new std::fstream(langConfigPath.c_str(), std::ios_base::in);
	if (pStream->fail())
	{
		return -1;
	}
    if( !stream.Open(langConfigPath, File_Read))
    {
        K_LOG(std::string("File write access denied for location "+langConfigPath).c_str());
    }
    id = stream.ReadInt();
    stream.Close();
    return id;
}

