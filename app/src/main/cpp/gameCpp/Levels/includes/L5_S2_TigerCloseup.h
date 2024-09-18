

#ifndef DQFYLH_L5_S2_TIGER_CLOSEUP_DEF
#define DQFYLH_L5_S2_TIGER_CLOSEUP_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"

#include "L5S2_Pop1Skelton.h"
#include "L5S2_Pop2Log.h"
#include "L5S2_Pop3Grease.h"

enum eL5S2PopEnums
{
	ePop1_L5S2Skelton,
	ePop2_L5S2Log,
	ePop3_L5S2Grease,
	ePopL5S2_Max
};

class L5_S2_TigerCloseup : public HiddenObjectSystem
{
public:
	//Popups
	int ActivePopupID;
	float musicFadeOutTimer;
	SuperFX *S2MoonFXFX;

	Popup*		PopupArray[ePopL5S2_Max];
	int			PopStates[ePopL5S2_Max];
	CPolygon*	PopPolyArray[ePopL5S2_Max];

	void OnPopupClosed(Popup* popup);

	L5_S2_TigerCloseup();
	~L5_S2_TigerCloseup();

	void SparkleUpdate();
	void TaskPendingUpdate();

	void InitPopups();
	void Update();
	void Required();

	void SceneObjectsUpdate();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
