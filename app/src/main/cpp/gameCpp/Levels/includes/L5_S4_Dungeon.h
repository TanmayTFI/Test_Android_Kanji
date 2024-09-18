

#ifndef DQFYLH_L5_S4_DUNGEON_DEF
#define DQFYLH_L5_S4_DUNGEON_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "DialogSystem.h"

enum eL5S4PopEnums
{
	ePop1_Bridge,
	ePop2_Pond,
	ePop3_HandCuff,
	ePopL5S4_Max
};

class L5_S4_Dungeon : public HiddenObjectSystem
{
public:
	//Popups
	float musicFadeOutTimer;
	int ActivePopupID;
	DialogSystem *dialogSystem;
	bool isDialogActive;
	SuperFX *S4MoonPrtFX, *S4waterFallFX, *S4waterFallDownFX, *S4CrystalParticle1FX, *S4CrystalParticle2FX, *S4CrystalParticle3FX;
	SuperFX *S4Lava1FX, *S4Lava2FX, *S4Lava3FX;
	JSONAnimator  *JAnim_Bridge, *JAnim_Anasuya;
	bool isWaterCutscnPlaying;
	CVideo *cv_WaterCutscn;
	float vidTimer;

	Popup*		PopupArray[ePopL5S4_Max];
	int			PopStates[ePopL5S4_Max];
	CPolygon*	PopPolyArray[ePopL5S4_Max];

	void OnPopupClosed(Popup* popup);

	L5_S4_Dungeon();
	~L5_S4_Dungeon();

	void SparkleUpdate();
	void TaskPendingUpdate();

	void InitPopups();
	void Update();
	void Required();
	void OnAnimationComplete(int AnimID);
	void SceneObjectsUpdate();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
