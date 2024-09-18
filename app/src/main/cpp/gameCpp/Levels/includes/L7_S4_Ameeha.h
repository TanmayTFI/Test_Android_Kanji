

#ifndef DQFYLH_L7_S4_AMEEHA_DEF
#define DQFYLH_L7_S4_AMEEHA_DEF

//====================================
//  created by : Amal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "DialogSystem.h"
#include "L7S4_Pop1Pooja.h"

enum eL7S4PopEnums
{
	ePopL7S4_Pop1Table,
	ePopL7S4_Pop2Flower,
	ePopL7S4_Max
};

class L7_S4_Ameeha : public HiddenObjectSystem
{
public:
	//Popups
	int ActivePopupID;
	DialogSystem *dialogSystem;
	bool isDialogActive;
	SuperFX *s4BubblesFX;
	float musicFadeOutTimer;

	Popup*		PopupArray[ePopL7S4_Max];
	int			PopStates[ePopL7S4_Max];
	CPolygon*	PopPolyArray[ePopL7S4_Max];
	L7S4_Pop1Pooja* PoojaPopup;

	void OnPopupClosed(Popup* popup);
	MeshObject* waterObj1;
	MeshObject* waterObj2;
	MeshObject* skyObj1;
	MeshObject* skyObj2;
	L7_S4_Ameeha();
	~L7_S4_Ameeha();

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
