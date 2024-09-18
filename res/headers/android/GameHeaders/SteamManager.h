//====================================
// DQFYLH
// Steam Manager
// Created by Sajith
//====================================

#ifndef STEAM_MANAGER_H
	#define STEAM_MANAGER_H

	#include "BuildConstants.h"
	#ifdef _STEAM_BUILD 

		class SteamManager
		{

		private:

			bool _isInit;
			bool _isRequestStatsSuccess;

		public:
			//Singleton Instance
	
			static SteamManager* GetInstance();
			static void DestroyInstance();	

			SteamManager();
			~SteamManager();

			void TestInventoryItems();
			void SetAssetLanguage();
			void SetSteamAchievement(const char* awardID);
			bool RequestStats();
			bool GetAchievementStatus(const char* awardID);
		};

	#endif
#endif
	