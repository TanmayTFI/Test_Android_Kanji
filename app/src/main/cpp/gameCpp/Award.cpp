#include"Award.h"
#include"ProfileSystem.h"
Award::Award(AwardCategory awardCategory, int totalNumOfEventsToComplete, int currentNumOfEvents)
{
	_awardCategory = awardCategory;
	_totalNumOfEventsToComplete = totalNumOfEventsToComplete;
	_currentNumOfEvents = currentNumOfEvents;
	_isCompleted = false;
	UpdateEvents(0);

	const char * awarsSteamIds[100] = {

	"Award_Collect_30_Awards",		
	"Award_HO_Without_Hint_1",				
	"Award_HO_Without_Hint_3",				
	"Award_HO_Without_Hint_5",				
	"Award_HO_Without_Hint_10",				
	"Award_MangoMiniGame_Without_Misthrow",	
	"Award_HO_Within_Time_150",				
	"Award_HO_Within_Time_90",				
	"Award_HO_Within_Time_30",				
	"Award_HO_With_Match3",					
	"Award_Mini_Without_Skip_2",				
	"Award_Mini_Without_Skip_4",				
	"Award_Mini_Without_Skip_6",				
	"Award_Mini_Within_Time_1",				
	"Award_Mini_Jigsaw_Mohenjodaro_3",		
	"Award_Pick5_WithinTime",				
	"Award_Game_without_Skip",				
	"Award_Game_Without_Hint",				
	"Award_Game_Without_Hint_HO",			
	"Award_Game_Without_Skip_CutScene",      
	"Award_Game_Within_Time_10hr",			
	"Award_Game_Within_Time_4hr",			
	"Award_Start_Journey_Narara",			
	"Award_MainStory_OnExpert",				
	"Award_CreeksMansion_Without_hint",		
	"Award_FastTravel",						
	"Award_TreasureRoom_Escape",				
	"Award_Collect_Inventory_Item_33",       
	"Award_Collect_Memory_1",				
	"Award_Collect_Memory_All",				
	"Award_Collect_Morphing_1",				
	"Award_Collect_Morphing_All",			
	"Award_Game_Without_Tutorial",			
	"Award_TabletMiniGame_Without_Hint",		
	"Award_Unveiled_Maskedman",				
	"Award_Find_Who_Wreck_Indus",			
	"Award_Visit_4_World",					
	"Award_Save_Sophia",						
	"Award_Tame_Magical_peacock",			
	"Award_Decrypt_Indus_Script",			
	"Award_Complete_Mini_Soldier_Without_Hint",
	"Award_Max"
	};
	_awardSteamId = awarsSteamIds[_awardCategory];
}

bool Award::GetISCompleted()
{
	
	if (_currentNumOfEvents >= _totalNumOfEventsToComplete)
	{
		_isCompleted = true;
	}
		else
	{
		_isCompleted = false;
	}
		
	return _isCompleted;
}

void Award::UpdateEvents(int eventCount)
{
	if (_isCompleted)
	{
		return;
	}
	_currentNumOfEvents += eventCount;
	ProfileSystem::Instance->ActiveProfile.currentNumOfEvents[_awardCategory] = _currentNumOfEvents;
}

int Award::GetTotalNumOfEventsToComplete()
{
	return _totalNumOfEventsToComplete;
}

