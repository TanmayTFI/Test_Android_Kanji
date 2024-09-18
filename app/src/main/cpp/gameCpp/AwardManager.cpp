#include"AwardManager.h"
#include "ProfileSystem.h"
#include "AwardInterface.h"
#include "SteamManager.h"
#include "Application.h"

#define Award_Collect_30_Awards 30
#define Award_HO_Without_Hint_1 1
#define Award_HO_Without_Hint_3 3
#define Award_HO_Without_Hint_5 5
#define Award_HO_Without_Hint_10 10
#define Award_MangoMiniGame_Without_Misthrow 1
#define Award_HO_Within_Time_150 1
#define Award_HO_Within_Time_90	1
#define Award_HO_Within_Time_30 1
#define Award_HO_With_Match3 1
#define Award_Mini_Without_Skip_2 2
#define Award_Mini_Without_Skip_4 4
#define Award_Mini_Without_Skip_6 6
#define Award_Mini_Within_Time_1 1
#define Award_Mini_Jigsaw_Mohenjodaro_3 1
#define Award_Pick5_WithinTime 1
#define Award_Game_without_Skip 1
#define Award_Game_Without_Hint 1
#define Award_Game_Without_Hint_HO 1
#define Award_Game_Without_Skip_CutScene 1
#define Award_Game_Within_Time_10hr 1
#define Award_Game_Within_Time_4hr 1 //TODO : replace this value with shortest game time after testing
#define Award_Start_Journey_Narara 1
#define Award_MainStory_OnExpert 1
#define Award_CreeksMansion_Without_hint 1
#define Award_FastTravell 1
#define Award_TressureRoom 1
#define Award_Collect_Inventory_Item_33 39	//41 items till the EOD = 33 + 8 including multiinv items
#define Award_Collect_Memory_1 1
#define Award_Collect_Memory_All 30
#define Award_Collect_Morphing_1 1
#define Award_Collect_Morphing_All 28
#define Award_Game_Without_Tutorial 1
#define Award_TabletMiniGame_Without_Hint 1
#define Award_Unveiled_Maskedman 1
#define Award_Find_Who_Wreck_Indus 1
#define Award_Visit_4_World 1
#define Award_Save_Sophia 1
#define Award_Tame_Magical_peacock 1
#define Award_Decrypt_Indus_Script 1
#define Award_Complete_Mini_Soldier_Without_Hint 1


#define Time_To_Finish_HO_150 150
#define Time_To_Finish_HO_90 90
#define Time_To_Finish_HO_30 30
#define Time_To_Finish_Game_10Hr 10 * 60 * 60
#define Time_To_Finish_HO_4Hr 4 * 60 * 60
#define Time_To_Finish_MiniGame_60 60

static AwardManager* _instance = NULL;
#pragma region public fuctions


AwardManager* AwardManager::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new AwardManager();
		_instance->Initialize();
	}

	return _instance;
}

void AwardManager::SetAwardInterface(AwardInterface* awardInterface)
{
	_awardInterface = awardInterface;
}

void AwardManager::Initialize()
{
	_gameStartTime = (long) time(0);
	_awardInterface = NULL;

	_isHO = false;
	int numEventsToComplete[MAX_HALL_OF_FAME_AWARDS] = {
		Award_Collect_30_Awards,
		Award_HO_Without_Hint_1,
		Award_HO_Without_Hint_3, 
		Award_HO_Without_Hint_5,
		Award_HO_Without_Hint_10,
		Award_MangoMiniGame_Without_Misthrow,
		Award_HO_Within_Time_150,
		Award_HO_Within_Time_90,
		Award_HO_Within_Time_30,
		Award_HO_With_Match3,
		Award_Mini_Without_Skip_2,
		Award_Mini_Without_Skip_4,
		Award_Mini_Without_Skip_6,
		Award_Mini_Within_Time_1,
		Award_Mini_Jigsaw_Mohenjodaro_3, 
		Award_Pick5_WithinTime, 
		Award_Game_without_Skip,
		Award_Game_Without_Hint,
		Award_Game_Without_Hint_HO,
		Award_Game_Without_Skip_CutScene,
		Award_Game_Within_Time_10hr,
		Award_Game_Within_Time_4hr,//kAward_Game_Within_Time_4hr
		Award_Start_Journey_Narara,
		Award_MainStory_OnExpert,
		Award_CreeksMansion_Without_hint,
		Award_FastTravell,
		Award_TressureRoom,
		Award_Collect_Inventory_Item_33,
		Award_Collect_Memory_1,
		Award_Collect_Memory_All,
		Award_Collect_Morphing_1,
		Award_Collect_Morphing_All,
		Award_Game_Without_Tutorial,
		Award_TabletMiniGame_Without_Hint,
		Award_Unveiled_Maskedman,
		Award_Find_Who_Wreck_Indus,
		Award_Visit_4_World,
		Award_Save_Sophia,
		Award_Tame_Magical_peacock,
		Award_Decrypt_Indus_Script,
		Award_Complete_Mini_Soldier_Without_Hint
	};

	for (int i = 0; i < MAX_HALL_OF_FAME_AWARDS; i++)
	{
		Award * award = NULL;
		award = new Award((AwardCategory) i, numEventsToComplete[i], ProfileSystem::Instance->ActiveProfile.currentNumOfEvents[i]);
		_listOfAwards.push_back(award);
	}




}

void AwardManager::DeleteInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = NULL;

	}
}

AwardManager::~AwardManager()
{
    int size = (int)_listOfAwards.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)
	{
		Award * award = NULL;
		award = _listOfAwards[i];
		if (award)
		{
			delete award;
			_listOfAwards[i] = NULL;
		}
	}
	_listOfAwards.clear();
	_listOfCompletedAwards.clear();
}

bool AwardManager::IsAwardCompleted(AwardCategory awardID)
{
	Award *award = _listOfAwards.at((int)awardID);
	return award->GetISCompleted();
}

int AwardManager::GetCompletedAwardFromQueue()
{
	if (_listOfCompletedAwards.size() > 0)
	{
		return _listOfCompletedAwards.at(0);
	}
	else
	{
		return -1;
	} 
}

void AwardManager::RemoveCompletedAwardFromQueue(int awardId)
{
	auto it = std::find(_listOfCompletedAwards.begin(), _listOfCompletedAwards.end(), awardId);
	if(it != _listOfCompletedAwards.end())
	{
		_listOfCompletedAwards.erase(it);
	}
}

Award * AwardManager::GetAward(AwardCategory awardId)
{
	return _listOfAwards.at((int)awardId);
}

void AwardManager::UpdateEvents(AwardCategory awardId, int eventCount)
{
	Award *award = _listOfAwards.at((int)awardId);
	if( award != NULL )
		UpdateEvents(award, eventCount);
}

void AwardManager::UpdateElapsedGamePlayTime()
{
	long currentTime = (long) time(0);
	long currentSessionTime = currentTime - _gameStartTime;
	_gameStartTime = currentTime;
    int elapsedTime = (int) (ProfileSystem::Instance->ActiveProfile.ElapsedGamePlayTime + currentSessionTime);  //Sac: converted Implicit to explicit

	ProfileSystem::Instance->ActiveProfile.ElapsedGamePlayTime = elapsedTime;
}

void AwardManager::CollectAward(AwardCategory awardId)
{
	Award *award = _listOfAwards.at((int)awardId);
	if( award != NULL )
		UpdateEvents(award, award->GetTotalNumOfEventsToComplete());
}

void AwardManager::StartMainStory()
{

}

void AwardManager::CompleteMainStory()
{
	CollectAward(kAward_Save_Sophia);
	if (ProfileSystem::Instance->ActiveProfile.IsPlayingGameOnExpertMode == 1)
	{
		HandleCompleteMainStoryOnExpertLevel();
	}
}

void AwardManager::CompleteBonusStory()
{
	HandleGameCompleteWithoutSkip();
	HandleGameCompleteWithinTime();
	HandleGameCompleteWithoutHint();
	HandleGameCompleteWithoutSkipDialoguesOrCutScenes();
}

void AwardManager::StartHO(int areaId)
{
	if (ProfileSystem::Instance->ActiveProfile.AreaStartTime[areaId] == 0)
	{
		ProfileSystem::Instance->ActiveProfile.AreaStartTime[areaId] = ProfileSystem::Instance->ActiveProfile.ElapsedGamePlayTime;
	}
}

void AwardManager::CompleteHO(int areaId)
{
	if (!ProfileSystem::Instance->ActiveProfile.isHOCompleted[areaId])
	{
		ProfileSystem::Instance->ActiveProfile.isHOCompleted[areaId] = true;

		if (!ProfileSystem::Instance->ActiveProfile.isHOCompleteWithMatch3[areaId])
		{
			if (!ProfileSystem::Instance->ActiveProfile.isHintUsedAfterHOStart[areaId])
			{
				if (areaId == eArea_L6_S5_TabMiniGame)
				{
					CollectAward(kAward_TabletMiniGame_Without_Hint);
				}
				else if(areaId == eArea_L2_S7_PZSoldiers)
				{
					CollectAward(kAward_Complete_Mini_Soldier_Without_Hint);
				}
				else
				{
					HandleHOCompleteWithoutHint(areaId);
				}
			}

			 if(areaId != eArea_L6_S5_TabMiniGame && areaId != eArea_L2_S7_PZSoldiers)
			{
				HandleHOCompleteWithinTime(areaId);
			}
		}
	}

}


void AwardManager::StartMiniGame(int areaId)
{
	ProfileSystem::Instance->ActiveProfile.isSkipUsedAfterMiniGameStart[areaId] = false;
	if (ProfileSystem::Instance->ActiveProfile.AreaStartTime[areaId] == 0)
	{
		ProfileSystem::Instance->ActiveProfile.AreaStartTime[areaId] = ProfileSystem::Instance->ActiveProfile.ElapsedGamePlayTime;
	}
}

void AwardManager::CompleteMiniGame(int areaId)
{
	if (!ProfileSystem::Instance->ActiveProfile.isMiniGameCompleted[areaId])
	{
		ProfileSystem::Instance->ActiveProfile.isMiniGameCompleted[areaId] = true;
		HandleMiniGameCompleteWithinTime(areaId);
		HandleMiniGameComplete(areaId);
	}

}

///Deprecated...(Not used now  call from Hud)
void AwardManager::UseHoHint(int areaId)
{

}

void AwardManager::UseHint()
{
	ProfileSystem::Instance->ActiveProfile.isHintUsedAfterGameStart = true;
	ProfileSystem::Instance->ActiveProfile.isHintUsedAfterCreeksMansionStart = true;
	if (_isHO)
	{
		ProfileSystem::Instance->ActiveProfile.isHintUsedAfterHOStart[ProfileSystem::Instance->ActiveProfile.CurrentArea] = true;
		ProfileSystem::Instance->ActiveProfile.isHOHintUsedAfterGameStart = true;
	}

}

void AwardManager::SkipMiniGame(int areaId)
{
	ProfileSystem::Instance->ActiveProfile.isMiniGameSkipedAfterGameStart = true;
	ProfileSystem::Instance->ActiveProfile.isSkipUsedAfterMiniGameStart[areaId] = true;
	if (areaId == eArea_L7_S1_PZJigsawYellow || areaId == eArea_L7_S3_PZJigsawRed || areaId == eArea_L7_S4_PZJigsawBlue)
	{
		ProfileSystem::Instance->ActiveProfile.isJigsawPuzzleSkipped = true;
	}
}

void AwardManager::SkipDialogueOrCutScene()
{
	ProfileSystem::Instance->ActiveProfile.isCutSceneSkipedAfterGameStart = true;
}



void AwardManager::CompleteCreeksMansion()
{
	if (!ProfileSystem::Instance->ActiveProfile.isHintUsedAfterCreeksMansionStart)
	{
		Award *award = _listOfAwards.at(kAward_CreeksMansion_Without_hint);
		if( award != NULL )
			UpdateEvents(award, 1);
	}
}




void AwardManager::CollectInventoryItem()
{
	Award *award = _listOfAwards.at(kAward_Collect_Inventory_Item_33);
	if( award != NULL )
		UpdateEvents(award, 1);
}

void AwardManager::CollectMemory()
{
	Award *award = _listOfAwards.at(kAward_Collect_Memory_1);
	if( award != NULL )
		UpdateEvents(award, 1);
	award = _listOfAwards.at(kAward_Collect_Memory_All);
	if( award != NULL )
		UpdateEvents(award, 1);
}

void AwardManager::CollectMorphing()
{
	Award *award = _listOfAwards.at(kAward_Collect_Morphing_1);
	if( award != NULL )
		UpdateEvents(award, 1);
	award = _listOfAwards.at(kAward_Collect_Morphing_All);
	if( award != NULL )
		UpdateEvents(award, 1);
}

void AwardManager::SkipTutorial()
{
	Award* award = _listOfAwards.at(kAward_Game_Without_Tutorial);
	if( award != NULL )
		UpdateEvents(award, 1);
}  



void AwardManager::CompleteHOWithMatch3(int areaId)
{
	ProfileSystem::Instance->ActiveProfile.isHOCompleteWithMatch3[areaId] = true; 
	Award* award = _listOfAwards.at(kAward_HO_With_Match3);
	if( award != NULL )
		UpdateEvents(award, 1);
}

void AwardManager::Pick5ItemsWithinTime()
{
	Award* award = _listOfAwards.at(kAward_Pick5_WithinTime);
	if( award != NULL )
		UpdateEvents(award, 1);
}

void AwardManager::StartExpertMode()
{
	if (ProfileSystem::Instance->ActiveProfile.IsPlayingGameOnExpertMode == -1)
	{
		ProfileSystem::Instance->ActiveProfile.IsPlayingGameOnExpertMode = 1;
	}
}

void AwardManager::ExitExpertMode()
{
	ProfileSystem::Instance->ActiveProfile.IsPlayingGameOnExpertMode = 0;
}
#pragma endregion



#pragma region Protected functions



void AwardManager::HandleHOCompleteWithoutHint(int areaId)
{

	if (!ProfileSystem::Instance->ActiveProfile.isHintUsedAfterHOStart[areaId])
	{
		Award *award = _listOfAwards.at(kAward_HO_Without_Hint_1);
		if( award != NULL )
			UpdateEvents(award, 1);
		award = _listOfAwards.at(kAward_HO_Without_Hint_3);
		if( award != NULL )
			UpdateEvents(award, 1);
		award = _listOfAwards.at(kAward_HO_Without_Hint_5);
		if( award != NULL )
			UpdateEvents(award, 1);
		award = _listOfAwards.at(kAward_HO_Without_Hint_10);
		if( award != NULL )
			UpdateEvents(award, 1);
	}
}

void AwardManager::HandleHOCompleteWithinTime(int areaId)
{
	int currentTime = ProfileSystem::Instance->ActiveProfile.ElapsedGamePlayTime;
	int gamePlayTime = currentTime - ProfileSystem::Instance->ActiveProfile.AreaStartTime[areaId];
//	K_LOG("gamePlayTime = %ld", gamePlayTime);

/*
	Award* award = _listOfAwards.at(kAward_HO_Within_Time_150);
	if( award != NULL )
		if (gamePlayTime <= Time_To_Finish_HO_150)
		{
			UpdateEvents(award, 1);
		}
	award = _listOfAwards.at(kAward_HO_Within_Time_90);
	if( award != NULL )
		if (gamePlayTime <= Time_To_Finish_HO_90)
		{
			UpdateEvents(award, 1);
		}
	award = _listOfAwards.at(kAward_HO_Within_Time_30);
	if( award != NULL )
		if (gamePlayTime <= Time_To_Finish_HO_30)
		{
			UpdateEvents(award, 1);
		}
*/
	Award* award = _listOfAwards.at(kAward_HO_Within_Time_150);
	if( award != NULL )
		if (gamePlayTime <= Time_To_Finish_HO_150 && gamePlayTime > Time_To_Finish_HO_90)
		{
			UpdateEvents(award, 1);
		}
	award = _listOfAwards.at(kAward_HO_Within_Time_90);
	if( award != NULL )
		if (gamePlayTime <= Time_To_Finish_HO_90 && gamePlayTime > Time_To_Finish_HO_30)
		{
			UpdateEvents(award, 1);
		}
	award = _listOfAwards.at(kAward_HO_Within_Time_30);
	if( award != NULL )
		if (gamePlayTime <= Time_To_Finish_HO_30)
		{
			UpdateEvents(award, 1);
		}
}





void AwardManager::HandleMiniGameComplete(int areaId)
{
	Award* award = NULL;
	if (!ProfileSystem::Instance->ActiveProfile.isSkipUsedAfterMiniGameStart[areaId])
	{
		award = _listOfAwards.at(kAward_Mini_Without_Skip_2);
		if( award != NULL )
			UpdateEvents(award, 1);
		award = _listOfAwards.at(kAward_Mini_Without_Skip_4);
		if( award != NULL )
			UpdateEvents(award, 1);
		award = _listOfAwards.at(kAward_Mini_Without_Skip_6);
		if( award != NULL )
			UpdateEvents(award, 1);
	}


	if (!ProfileSystem::Instance->ActiveProfile.isJigsawPuzzleSkipped)
	{
		if (ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted && ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted && ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted)
		{
			award = _listOfAwards.at(kAward_Mini_Jigsaw_Mohenjodaro_3);
			if( award != NULL )
				UpdateEvents(award, 1);
		}
	}

}

void AwardManager::HandleMiniGameCompleteWithinTime(int areaId)
{
	if (!ProfileSystem::Instance->ActiveProfile.isSkipUsedAfterMiniGameStart[areaId])
	{
		int currentTime = ProfileSystem::Instance->ActiveProfile.ElapsedGamePlayTime;
		int gamePlayTime = currentTime - ProfileSystem::Instance->ActiveProfile.AreaStartTime[areaId];

		if (gamePlayTime <= Time_To_Finish_MiniGame_60 )
		{
			Award* award = _listOfAwards.at(kAward_Mini_Within_Time_1);
			if( award != NULL )
				UpdateEvents(award, 1);
		}
	}


}


void AwardManager::HandleGameCompleteWithoutSkip()
{
	if (!ProfileSystem::Instance->ActiveProfile.isMiniGameSkipedAfterGameStart)
	{
		Award* award = _listOfAwards.at(kAward_Game_without_Skip);
		if( award != NULL )
			UpdateEvents(award, 1);
	}
}

void AwardManager::HandleGameCompleteWithoutHint()
{
	if (!ProfileSystem::Instance->ActiveProfile.isHintUsedAfterGameStart)
	{
		Award* award = _listOfAwards.at(kAward_Game_Without_Hint);
		if( award != NULL )
			UpdateEvents(award, 1);
	}

	if (!ProfileSystem::Instance->ActiveProfile.isHOHintUsedAfterGameStart)
	{
		Award* award = _listOfAwards.at(kAward_Game_Without_Hint_HO);
		if( award != NULL )
			UpdateEvents(award, 1);
	}
}



void AwardManager::HandleGameCompleteWithoutSkipDialoguesOrCutScenes()
{
	if (!ProfileSystem::Instance->ActiveProfile.isCutSceneSkipedAfterGameStart)
	{
		Award* award = _listOfAwards.at(kAward_Game_Without_Skip_CutScene);
		if( award != NULL )
			UpdateEvents(award, 1);
	}
}

void AwardManager::HandleGameCompleteWithinTime()
{

	int gamePlayTime = ProfileSystem::Instance->ActiveProfile.ElapsedGamePlayTime;


	Award* award = _listOfAwards.at(kAward_Game_Within_Time_10hr);
	if (gamePlayTime <= Time_To_Finish_Game_10Hr && gamePlayTime > Time_To_Finish_HO_4Hr)
	{
		if( award != NULL )
			UpdateEvents(award, 1);
	}
	award = _listOfAwards.at(kAward_Game_Within_Time_4hr);
	if (gamePlayTime <= Time_To_Finish_HO_4Hr)
	{
		if( award != NULL )
			UpdateEvents(award, 1);
	}
}





void AwardManager::HandleCompleteMainStoryOnExpertLevel()
{
	Award* award = _listOfAwards.at(kAward_MainStory_OnExpert);
	if( award != NULL )
		UpdateEvents(award, 1);
}

void AwardManager::UpdateEvents(Award* award,  int eventCount)
{
	if (!award->GetISCompleted())
	{
		award->UpdateEvents(eventCount);
		if (award->GetISCompleted())
		{
		//	K_LOG("##########################   AWARD %d", award->_awardCategory);
	#ifdef _STEAM_BUILD
			#ifdef _CEBUILD
				SteamManager::GetInstance()->SetSteamAchievement(award->_awardSteamId);			
			#endif
	#endif

	#ifdef K_ANDROID
			#ifdef _CEBUILD
				GFApp->SetAndroidAchievement((int)award->_awardCategory);			
			#endif
	#endif
			if( ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_L1_S1_Beach &&
				ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_L8_S5_VirtualMiniGame &&
				 GFInstance->Flow == fHOLevel)
				 {		// Sac: Added this braces to avoid confusion, as these were not there...
					if (_awardInterface != NULL )
					{
						if (_listOfCompletedAwards.size() == 0)
						{
							_awardInterface->OnAwardCompleted(award->_awardCategory);						
						}
					}

					_listOfCompletedAwards.push_back((int)award->_awardCategory);				
					Award *award2 = _listOfAwards.at(kAward_Collect_30_Awards);
					if( award2 != NULL )
						UpdateEvents(award2, 1);			
				}
			
		}
	}
}
#pragma endregion
