
#ifndef DQFYLH_L5_S1_TIGERDISTANT_DEF
#define DQFYLH_L5_S1_TIGERDISTANT_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "MeshObject.h"

#include "L5S1_Pop1Bow.h"
#include "L5S1_Pop2Arrow.h"
#include "L5S1_Pop3Tiger.h"

enum eL5S1PopEnums
{
	ePop1_L5S1Pop1Bow,
	ePop2_L5S1Pop2Arrow,
	ePop3_L5S1Pop3Tiger,

	ePopL5S1_Max
};

class L5_S1_TigerDistant : public HiddenObjectSystem
{
public:
	//Popups
	int ActivePopupID;
	float musicFadeOutTimer;
	Popup*		PopupArray[ePopL5S1_Max];
	int			PopStates[ePopL5S1_Max];
	CPolygon*	PopPolyArray[ePopL5S1_Max];
	L5S1_Pop3Tiger * Pop3Scene; 
	void OnPopupClosed(Popup* popup);

	MeshObject* meshLeaf;
	//JSONAnimator *_JAnim_TigerLoop;

	bool isTigerBannerShown;
	float tigerBannerTimer;

	L5_S1_TigerDistant();
	~L5_S1_TigerDistant();

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
