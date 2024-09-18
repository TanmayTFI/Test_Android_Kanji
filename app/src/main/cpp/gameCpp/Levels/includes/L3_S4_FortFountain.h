//====================================
// DQFULP
// Hidden Object System
// Author : Faisal
// LEVEL3 - SCENE3 : FortFountain
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_FORTFOUNTAIN_DEF
#define DQFYLH_LEVEL3_SCENE3_FORTFOUNTAIN_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MeshObject.h"

#include "L3S4_Pop2MapWallOne.h"
#include "L3S4_Pop2MapWallTwo.h"
#include "L3S4_Pop2MapWallThree.h"
#include "L3S4_Pop2MapWallFour.h"
#include "L3S4_Pop4Metalsheet.h"

#include "Popup.h"

class CHOScene;

enum eL3S4PopEnums
{
	ePop1_BambooDoor,
	ePop2_1MapWall,
	ePop2_2MapWall,
	ePop2_3MapWall,
	ePop2_4MapWall,
	ePop3_CliffDoor,
	ePop4_MetalSheet,
	ePopL3S4_Max
};

class L3_S4_FortFountain: public HiddenObjectSystem
{
	public:
		float musicFadeOutTimer;
		//Scene Elements
		MeshObject *meshCloth1, *meshCloth2, *meshCloth3;

		std::vector<SuperFX*>	VFXArray;

		SuperFX *hodotsFX, *holinesFX;
		
		//Popups
		int ActivePopupID;

		Popup*		PopupArray[ePopL3S4_Max];
		int			PopStates[ePopL3S4_Max];
		CPolygon*	PopPolyArray[ePopL3S4_Max];

		L3S4_Pop2MapWallOne		*popMapWall1;
		L3S4_Pop2MapWallTwo		*popMapWall2;
		L3S4_Pop2MapWallThree	*popMapWall3;
		L3S4_Pop2MapWallFour	*popMapWall4;
		L3S4_Pop4Metalsheet		*Pop4Scene;
		
		void InitPopups();
		void OnPopupClosed(Popup* popup);

		void LoadNextPopup(Popup* popup, bool loadPrevious);
		
		L3_S4_FortFountain();
		~L3_S4_FortFountain();
		
		void SparkleUpdate();
		void TaskPendingUpdate();
		void SceneObjectsUpdate();		

		void Update();
		void Required();

		void NavigateToScene();

	protected:
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif