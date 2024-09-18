//===========================================================
// 
// AwardManager
// Sujin
//===========================================================

#include"Award.h"
#include "Profile.h"
#include "GameFlow.h"

#ifndef AWARD_MANAGER_H
#define AWARD_MANAGER_H

enum AwardFunctions
{
	eStartMainStory = 0,
	eCompleteMainStory,
	eCompleteBonusStory,
	eStartHO,
	eCompleteHO,
	eQuitHO,
	eStartMiniGame,
	eCompleteMiniGame,
	eQuitMiniGame, 
	eUseHint,
	eSkipMiniGame,
	eSkipDialogueOrCutScene,
	eCompleteCreeksMansion,
	eUseFastTravell,
	eStartJourneyToNarara,
	eGotOutOftreasureRoom,
	eCollectInventoryItem,
	eCollectMemory,
	eCollectMorphing,
	eSkipTutorial,
	ePick5ItemsWithinTime,
	eCompleteHOWithMatch3,
	eCollectAward,

	eAwardEventMax
};

class AwardInterface;
class AwardManager 
{

private:

	std::vector<Award*> _listOfAwards;
	AwardInterface * _awardInterface;
	long _gameStartTime;
	long _hoItemCollectedTimeDiff[MAX_HOITEMS + 1];
	long _prevHOItemCollectTime;
	long _numOfItemsCollectedInHO;
	std::vector<int> _listOfCompletedAwards;
	C_SYNTHESIZE(bool, _isHO, IsHO);
	


public:

	~AwardManager();

	void Initialize();
	static void DeleteInstance();
	static AwardManager* GetInstance(); 

	void SetAwardInterface(AwardInterface* awardInterface);

	void StartMainStory();
	void CompleteMainStory();
	void CompleteBonusStory();

	void StartHO(int areaId);
	void CompleteHO(int areaId);

	void StartMiniGame(int areaId);
	void CompleteMiniGame(int areaId);

	void UseHint();
	void UseHoHint(int areaId);
	void SkipMiniGame(int areaId);
	void SkipDialogueOrCutScene();

	void CompleteCreeksMansion();

	

	void CollectInventoryItem();
	void CollectMemory();
	void CollectMorphing();
	void SkipTutorial();
	void Pick5ItemsWithinTime();
	void StartExpertMode();
	void ExitExpertMode();

	void CompleteHOWithMatch3(int areaId);

	void CollectAward(AwardCategory awardId);


	bool IsAwardCompleted(AwardCategory awardID);
	int GetCompletedAwardFromQueue();
	void RemoveCompletedAwardFromQueue(int awardId);
	Award * GetAward(AwardCategory awardId);
	void UpdateElapsedGamePlayTime();

	//TODO: temp function for testing
	void UpdateEvents(AwardCategory awardId, int eventCount);

	//Moved up to call from mainmenu
	void HandleGameCompleteWithoutSkip();

protected:


	void HandleHOCompleteWithoutHint(int areaId);
	void HandleHOCompleteWithinTime(int areaId);

	void HandleMiniGameComplete(int areaId);
	void HandleMiniGameCompleteWithinTime(int areaId);


	void HandleGameCompleteWithoutHint();
	void HandleGameCompleteWithoutSkipDialoguesOrCutScenes();
	void HandleGameCompleteWithinTime();

	void HandleCompleteMainStoryOnExpertLevel();

	void UpdateEvents(Award* award, int eventCount);


};





#endif

