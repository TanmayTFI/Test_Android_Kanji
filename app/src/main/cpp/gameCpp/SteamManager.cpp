#include "SteamManager.h"
#include "IOStream.h"
#ifdef _STEAM_BUILD 

#ifdef K_WIN32	
	#include "Steam\steam_api.h"
	#include "Steam\steam_gameserver.h"
	#include "Steam\isteaminventory.h"
#else
	#include "Steam/steam_api.h"
	#include "Steam/steam_gameserver.h"
	#include "Steam/isteaminventory.h"
#endif
	#include "KPTK.h"

	//Class wide member and methods
	static SteamManager* _instance = NULL;

	SteamManager::SteamManager()
	{

		_isInit = false;
		_isRequestStatsSuccess = false;

		if(	SteamAPI_Init())
		{
			K_LOG("SteamAPI Init Success");

			_isInit = true;
			_isRequestStatsSuccess = RequestStats();
			if(_isRequestStatsSuccess)
			{
				K_LOG("Request Stats Success");
			}
			else
			{
				K_LOG("Request Stats Failed");
			}

			SetAssetLanguage();

		}
		else
		{
			K_LOG("SteamAPI Init Failed !!");
		}
	}

	bool SteamManager::RequestStats()
	{
		if(_isRequestStatsSuccess)
			return true;

		// Is Steam loaded? If not we can't get stats.
		if ( NULL == SteamUserStats() || NULL == SteamUser() )
		{
			return false;
		}
		// Is the user logged on?  If not we can't get stats.
		if ( !SteamUser()->BLoggedOn() )
		{
			return false;
		}
		// Request user stats.
		return SteamUserStats()->RequestCurrentStats();
	}

	SteamManager* SteamManager::GetInstance()
	{
		if(_instance == NULL)
			_instance = new SteamManager();
		return _instance;
	}

	void SteamManager::DestroyInstance()
	{

		SteamAPI_Shutdown();
		if(_instance != NULL)
		{
			delete _instance;
			_instance = NULL;
		}  

	}

	void SteamManager::TestInventoryItems()
	{
		SteamItemDef_t newItems[2];
		uint32 quantities[2];
		newItems[0] = 100;
		newItems[1] = 101;
		quantities[0] = 1;
		quantities[1] = 1;

		SteamInventoryResult_t s_GenerateRequestResult=-1;

		bool isSteamInvSuccess = SteamInventory()->GenerateItems( &s_GenerateRequestResult, newItems, quantities, 2 );
		if(isSteamInvSuccess)
			K_LOG("SteamInventory success");
		else
			K_LOG("SteamInventory failed");
	
	}


	void SteamManager::SetSteamAchievement(const char* awardID)
	{
		if(!_isInit)
			return;

	#ifdef _DEMOBUILD
		return;
	#endif //_DEMOBUILD

		if(!_isRequestStatsSuccess)
		{
			K_LOG("Request Stats called once again");
			_isRequestStatsSuccess = RequestStats();
		}

		//if( !SteamUserStats()->GetAchievement(awardID, false) )
		{
			SteamUserStats()->SetAchievement(awardID);  
			K_LOG("Set Achievement %s", awardID);
			SteamUserStats()->StoreStats();
		}
	

	}


	bool SteamManager::GetAchievementStatus(const char* awardID)
	{
	#ifdef DEBUG
		if(!_isInit)
			return false;

		return( SteamUserStats()->GetAchievement(awardID, false) ? true: false );  
	#else
		return false;
	#endif // DEBUG

	}

	void SteamManager::SetAssetLanguage()
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
		if (!stream.Open(langConfigPath, File_Write))
		{
			K_LOG(std::string("File write access denied for location " + langConfigPath).c_str());
		}

		//K_LOG(SteamApps()->GetAvailableGameLanguages());

		std::string language = SteamApps()->GetCurrentGameLanguage();

		if (language == "english")
		{
			stream.WriteInt(0);
			K_LOG("english selected");
		}
		else if (language == "dutch")
		{
			stream.WriteInt(500);
			K_LOG("dutch selected");
		}
		else if (language == "french")
		{
			stream.WriteInt(300);
			K_LOG("french selected");
		}
		else if (language == "german")
		{
			stream.WriteInt(100);
			K_LOG("german selected");
		}
		else if (language == "italian")
		{
			stream.WriteInt(400);
			K_LOG("italian selected");
		}
		else if (language == "brazilian")
		{
			stream.WriteInt(700);
			K_LOG("portuguese selected");
		}
		else if (language == "russian")
		{
			stream.WriteInt(501);
			K_LOG("russian selected");
		}
		else stream.Close();
	}

	SteamManager::~SteamManager()
	{
	}


#endif

