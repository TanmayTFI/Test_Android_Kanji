//===========================================================
//Control System Includes
//===========================================================
//#ifndef DQFULP_MAIN_DEF
//#define DQFULP_MAIN_DEF

#include "Application.h" //iPad defined
#include "ProfileSystem.h"
//Game Systems Includes
#include "TextureFactory.h"
#include "RenderList.h"
#include "UpdateList.h"
#include "GameFlow.h"
#include "LangDefs.h"
#include "SteamManager.h"
#include "KGame.h"


#ifdef K_WIN32
	#include <direct.h>
#else
	#include <unistd.h>
#endif // WIN32



#ifdef K_ANDROID
    #include "AndroidIo.h"
#endif // ANDROID

// Global var for preference folder
     char _szPathBuf[K_MAXPATH];

	bool startOurGame = false;

//Function Prototypes
void CreateGameSystems();
void DestroyGameSystems();
#ifdef _STEAM_BUILD
    static void SetSteamAchievement(int awardId);
#endif

#ifdef K_WIN32
	#if defined( JAPANESE_BUILD )
		extern std::wstring AppTitle_Str;
	#else
		extern std::wstring AppTitle_Str;
	#endif
#else
	#if defined( JAPANESE_BUILD )
		extern std::string AppTitle_Str;
	#else
		extern std::string AppTitle_Str;
	#endif
#endif

#ifdef K_ANDROID
 	extern bool config_debugmode, config_splashskip, config_surveybuild;
 	extern int config_lang_id, app_video_res,config_debugres;
 	extern std::string gdeviceName, path_toExternalPicturesDirectory, App_f_o_f;
	extern int gameversionCode, gameobbversionCode;
	extern	std::string gameversionName;
	extern	std::string surveyurl, reviewurl;
	// Code to receive the config values from Activity of Android, as we dont use config.ini in Android.
	void androidJniSetConfigVals(JNIEnv* env, jobject obj, int jgameVersionCode,jstring jgameVersionName, int jgameOBBVersionCode,jstring jgameDeviceName, jboolean jdebugmode, int jdebugres, jboolean jsplashskip, jboolean jsurveybuild, int jln_id, int jvidres, jstring jfrf, jstring jsurvey_url, jstring jreview_url, jboolean cameraGrant, jstring jpathExtrn) {
		config_debugmode = jdebugmode;
		config_debugres = jdebugres;
		config_splashskip=jsplashskip;
		config_surveybuild=jsurveybuild;
		config_lang_id = jln_id;
		app_video_res = jvidres;
		gameversionCode = jgameVersionCode;
		gameobbversionCode = jgameOBBVersionCode;
		if (cameraGrant){
			const char *fnameptr = env->GetStringUTFChars(jpathExtrn, NULL);
			path_toExternalPicturesDirectory = std::string(fnameptr);
			env->ReleaseStringUTFChars(jpathExtrn, fnameptr);
		}
		else
		{
			path_toExternalPicturesDirectory ="";
		}

		const char *frfptr = env->GetStringUTFChars(jfrf, NULL); 
		App_f_o_f = std::string(frfptr);
		env->ReleaseStringUTFChars(jfrf, frfptr);

		const char *gdnptr = env->GetStringUTFChars(jgameDeviceName, NULL); 
		gdeviceName = std::string(gdnptr);
		env->ReleaseStringUTFChars(jgameDeviceName, gdnptr);

		const char *versionNameptr = env->GetStringUTFChars(jgameVersionName, NULL); 
		gameversionName = std::string(versionNameptr);
		env->ReleaseStringUTFChars(jgameVersionName, versionNameptr);

		const char *surveyeptr = env->GetStringUTFChars(jsurvey_url, NULL); 
		surveyurl = std::string(surveyeptr);
		env->ReleaseStringUTFChars(jsurvey_url, surveyeptr);

		const char *reviewNameptr = env->GetStringUTFChars(jreview_url, NULL); 
		reviewurl = std::string(reviewNameptr);
		env->ReleaseStringUTFChars(jreview_url, reviewNameptr);

		startOurGame = true;
	}

	void androidJniSetFOrF(JNIEnv* env, jobject obj, int unlockVal) {
		
		if(GFApp != NULL)
		{
			if(GFApp->MyVersion != 456){
				GFApp->MyVersion = (unlockVal==456?456:123);
			}
			if(GFApp->MyVersion == 456)
			{
				//GFApp->UnlockFToF();
			}
		}

	}

	void androidJniCallWaitScreen(JNIEnv* env, jobject obj, jboolean showornot) {
		if(GFApp != NULL) {
			GFApp-> PauseForPurchase(showornot); //call pause screen
		}
	}

	void androidJniGetUserInfo(JNIEnv* env, jobject obj, jstring data, jboolean _isUnlocked)
	{

		//ProfileSystem::Instance->Initialize();

		const char *_userInfo = env->GetStringUTFChars(data, NULL);
        GFApp->noOfHints = std::atoi(_userInfo);
        env->ReleaseStringUTFChars(data, _userInfo);

#ifndef UNLOCKED
        if(GFApp->noOfHints != -777)
            GFApp->unlocked = _isUnlocked;
#else
		GFApp->unlocked = true;
#endif

		//ProfileSystem::Instance->WriteProfileSystemData();
	}
#endif

/* Windows uses WinMain(). Linux, Mac, iPhone and iPad use kanjiMain() */
#ifdef K_WIN32
	INT WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT iCmdShow) {
#else
	int kanjiMain (int argc, char **argv) {
#endif


	//Highlight single instance and turn off rest!!
	if( KPTK::checkGameInstance((const char*)AppTitle_Str.c_str()) )
		return 0;	

	//Seed Random Generation
    //std::cout<<"Inside Main";

	srand((unsigned int) time(NULL));	
	
	/* Use software mouse cursor */
	KInput::hidePointer();

	// Read data files from application bundle (for Mac) 
    #ifndef K_MACOS
        KMiscTools::setFileSource (false);
	#else
		#if !defined _STEAM_BUILD
            KMiscTools::setFileSource (true);
        #else
            KMiscTools::setFileSource (false);
        #endif
	#endif
//#ifndef K_MACOS
#if defined (K_WIN32)
		  //snprintf (_szPathBuf, K_MAXPATH, "%s/log.txt", KMiscTools::getPreferencesFolder());
		//_szPathBuf[K_MAXPATH - 1] = 0;
     strcpy(_szPathBuf,"log.txt");// [not supported outside win32]

		/* Start log */
   		//KStringFmt (_szPathBuf, K_MAXPATH, "%s/log.txt", getStateFolder());
#elif defined (K_ANDROID)

		// log file is stored inside the App in Android, in the location like this, 
		// /data/user/0/in.tuttifrutti.darkarata.abhq.en.full/files/log.txt 
		// or
		// /data/user/0/in.tuttifrutti.darkarata.abhq.en.free/files/log.txt 
		// KMiscTools::getPreferencesFolder() gives /data/user/0/in.tuttifrutti.darkarata.abhq.en.full/files
		std::string GamePath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/");
		KStringFmt (_szPathBuf, K_MAXPATH, "%slog.txt", GamePath.c_str());
	 //  KMiscTools::alertBox ("Log is at", _szPathBuf, "OK", NULL);
#else
		std::string GamePath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/Tuttifrutti_Games/");
     	std::string UserPath = GamePath + std::string("Darkarta_ABHQ_CE/");// Not changing to make backward compatible with all old players
    	KStringFmt (_szPathBuf, K_MAXPATH, "%slog.txt", UserPath.c_str());
#endif
   
     KPTK::enableLog (
		 #ifdef K_WIN32
			_szPathBuf
		 #else
			KMiscTools::makeFilePath(_szPathBuf)
		 #endif
     , "Darkarta", 
	 #ifdef K_ANDROID
	 gameversionName.c_str()
	 #else
	 "1.0"
	 #endif
	 , false);
	 
	#ifdef _DEBUG
		 KResource::logOpens(true);
	#else
		 KResource::logOpens(true);
	#endif


	#ifdef K_ANDROID
		//K_LOG("Language version is %d",config_lang_id);
		K_LOG("Android Version found is %s",KMiscTools::getOSVersion());

		while(!startOurGame) {
			// Stall till game start flag is raised from Java (after downloading of expansion fill happens)
		}

	#endif

	//Allocate game systems (singletons)
	CreateGameSystems();		
	
	
	//Initialize Control systems
	Control::InitializeSystem();
		
	//Create the game application - creates gameflow instance!!
	Application *Game = new Application();		

		
	//Highlight single instance and turn off rest!!
	//Enters into the main loop
	Game->Process();			

	//Deallocate game
	delete Game;

	//Finalize the Control systems
	Control::DestroySystem();

	//Deallocate game systems(singletons)
	DestroyGameSystems();	

#ifdef STRING_TABLE
	StringTable::freeStringTable();
#endif
	//exit(0);//hard exit to avoid any freeing issues!!

	return 0;
}

void CreateGameSystems()
{
	ProfileSystem::Create();
	//ProfileSystem::Instance->Initialize(); //Sac: Initialzing is shifted to Application.cpp as we need the warning strings read first
	
#ifdef _STEAM_BUILD 
	SteamManager::GetInstance();
#endif
	CTexFactory::Create();

	CRenderList::Create();
	UpdateList::Create();

	
}
void DestroyGameSystems()
{
	UpdateList::Destroy();
	CRenderList::Destroy();
	
	CTexFactory::Destroy();

	ProfileSystem::Destroy();

#ifdef _STEAM_BUILD 
	SteamManager::DestroyInstance();
#endif
	
}
        
//#endif
