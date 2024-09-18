

#ifndef DQFYLH_LEVEL_8_SCENE_2_GARDEN_DEF
#define DQFYLH_LEVEL_8_SCENE_2_GARDEN_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "CVideo.h"
#include "JSONAnimator.h"

#include "L8S2_Pop4Lotus.h"
#include "L8S2_Pop6HiddenCompartment.h"

enum eL8S2PopEnums
{
	ePop4_L8S2Lotus,
	ePop6_L8S2Idol,
	ePopL8S2_Max
};

class L8_S2_Garden: public HiddenObjectSystem
{
	CHOScene *sceneMagicGlass;
	bool isGlassInHand;

	CObject *symbols[MAX_L8_S1_MATCHING_SYMBOLS_COUNT];
	CObject *redSymbols[MAX_L8_S1_MATCHING_SYMBOLS_COUNT];
	CObject *silhouttes[(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2) + 1];
	int selectedSymbol;
public:
	L8_S2_Garden();
	~L8_S2_Garden();
	float musicFadeOutTimer;
	//Popups
	int ActivePopupID;

	Popup*		PopupArray[ePopL8S2_Max];
	int			PopStates[ePopL8S2_Max];
	CPolygon*	PopPolyArray[ePopL8S2_Max];

	CObject		*Fog;
	CObject		*Butterflies[3];
	float		ButterflyAnimTimer[3];
	float		ButterflyAnimThreshold[3];

	void OnPopupClosed(Popup* popup);
	bool isPopupActive(int popId);
	void Update();
	void Required();

	CObject *GateClosed, *GateOpened;
	CVideo *cvMammothCutscn;
	float vidTimer;
	bool IsVideoPlaying;

	CPoint currentPos,prevPos, _pos;
	float mouseDx, mouseDy;
	bool LBPressed, isTouchScreen;

	void SkipMagicGlass();
	CObject *SkipBtn, *skipBTN_text;
	void NavigateToScene();

protected:
	JSONAnimator *MammothIdleAnim, *MammothAngryAnim, *MammothEatAnim;
	SuperFX *S2LeafFallFX, *S2LeafFall2FX;
	SuperFX *S2_FountainFX;
	SuperFX *s2_MammothAngryFX;
	CPolygon* GatePoly, *StoneStructurePoly, *SugarCanePoly;

	void OnAnimationComplete(int index);
	void UpdateButterflies();
	void StopParticleEmission();
	void SceneObjectsUpdate();
	void TaskPendingUpdate();
	void InitAnimations();
	void InitPopups();
	void InitControlPolys();
	void ReceiveMessage(int val);
	void ProcessNonHOHint();
	void SparkleUpdate();
	void CheckForPendingTask();
};

#endif
