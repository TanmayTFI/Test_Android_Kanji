//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 01 : Forest Entrance
//====================================

#ifndef DQFYLH_LEVEL2_SCENE1_CAVE_MOUTH_DEF
#define DQFYLH_LEVEL2_SCENE1_CAVE_MOUTH_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Tutorial.h"
#include "MeshObject.h"
#include "L2S3_Pop1Turmeric.h"
//#include "L2S3_Pop2Lighter.h"
#include "ClickablePolygon.h"

#include "SuperFX.h"
#include "CustomAnimGroup.h"

enum eL2S3HOPopEnums
{
	eHOS3Pop_Inactive =-1,
	eHOPop1_Turmeric,
	eHOPop2_Lighter,
	eHOL2S3Pop_Max
};


class CHOScene;

class L2_S3_CaveMouth: public HiddenObjectSystem
{
	public:
		SuperFX *fogVFXL, *fogVFXR, *puzCompleteGlow;
		//Popups
		float musicFadeOutTimer;
		L2S3_Pop1Turmeric *pop1Turmeric;
		//L2S3_Pop2Lighter *pop2Lighter;

		ClickablePolygon *pop1ClickPoly;
		//ClickablePolygon *pop2ClickPoly;

		Popup *PopUpArray[eHOL2S3Pop_Max];

		void onPopAreaClicked(int popId);
		void onPopAreaMouseOver(int popId);
		void onPopAreaMouseOut(int popId);
		void onPopupClosed(Popup * popup);

		bool isMouseOverPopup;

		//Scene Elements

		CObject *SCN_turmericPlants;
		CObject *Scn_PoojaLamp;
		CObject *SCN_caveRoots;
		CObject *SCN_vines[5];
		CObject *SCN_stone[4];

		// -- Popups -- //
		int ActivePopupID;

		L2_S3_CaveMouth();
		~L2_S3_CaveMouth();

		void Update();
		void Required();

		void SparkleUpdate();
		void TaskPendingUpdate();
		void SceneObjectsUpdate();

		void InitPopups();

		void NavigateToScene();
		
	protected:
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif