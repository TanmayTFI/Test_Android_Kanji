

#ifndef DQFYLH_LEVEL8_SCENE2_SUGAR_CANE_PUZZLE_DEF
#define DQFYLH_LEVEL8_SCENE2_SUGAR_CANE_PUZZLE_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "L8_S2_PZBubble.h"
#include "MultilineText.h"

using namespace L8_S1_BubblePuzzle;

enum L8_S2_SceneState
{
	L8_S2_SceneState_WaitingForMagicWand = 0,
	L8_S2_SceneState_MagicWandAnimation,
	L8_S2_SceneState_PlayPuzzle,
	L8_S2_SceneState_PuzzlePlaying,
	L8_S2_SceneState_PuzzleComplete,
	L8_S2_SceneState_PuzzleSceneHide,
	L8_S2_SceneState_CaneGrowing,
	L8_S2_SceneState_WaitingForSword,
	L8_S2_SceneState_SwardPlaying,
	L8_S2_SceneState_SwardPlayingComplete
};


class L8_S2_PZSugarCane : public HiddenObjectSystem, public PZSceneInterface
{
	int _pzCurrentLevel;
	PZBubbleScene *_pzScene;
	L8_S2_SceneState _state;

	JSONAnimator *_janim_WeaponUse;
	JSONAnimator *_janim_SwardUse;

	std::vector<CObject*> _sugerCaneObjParts;
	float _pzSceneHidedelay;

	SuperFX *S2CaneGrowFX;
	MultilineText *pzHelpText;
	
	SuperFX *s2WeaponUseFxFX;

	float musicFadeOutTimer;

public:

	L8_S2_PZSugarCane();
	~L8_S2_PZSugarCane();

	void SparkleUpdate();
	void TaskPendingUpdate();

	void Update();
	void Required();

	void SceneObjectsUpdate();
	void NavigateToScene();

protected:

	void OnLevelComplete(int currentLevel);

	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();

	void OnAnimationComplete(int animId);

	void LoadState();
};

#endif
