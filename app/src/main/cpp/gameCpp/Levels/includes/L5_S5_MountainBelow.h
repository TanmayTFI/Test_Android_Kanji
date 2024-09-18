

#ifndef DQFYLH_L5_S5_MOUNTAIN_BELOW_DEF
#define DQFYLH_L5_S5_MOUNTAIN_BELOW_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "MeshObject.h"

#include "L5S5_Pop1BambooLadder.h"
#include "L5S5_Pop2MoonGlass.h"
#include "L5S5_Pop3TopExitDoor.h"
#include "L5S5_Pop4Sack.h"

enum eL5S5PopEnums
{
	ePop1_L5S5Pop1BambooLadder,
	ePop2_L5S5Pop2MoonGlassDoor,
	ePop3_L5S5Pop3TopExitDoor,
	ePop4_L5S5Pop4Sack,
	ePopL5S5_Max
};

class L5_S5_MountainBelow : public HiddenObjectSystem
{
public:
	SuperFX *s5hodotsFX, *s5holinesFX;
	float musicFadeOutTimer;
	//Popups
	int ActivePopupID;

	Popup*		PopupArray[ePopL5S5_Max];
	int			PopStates[ePopL5S5_Max];
	CPolygon*	PopPolyArray[ePopL5S5_Max];

	L5S5_Pop1BambooLadder * Pop1Scene;

	MeshObject* meshWater;
	SuperFX *S5leftcrystalsFX, *S5fliesFX, *S5rightcrystalFX;

	void OnPopupClosed(Popup* popup);

	L5_S5_MountainBelow();
	~L5_S5_MountainBelow();

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
