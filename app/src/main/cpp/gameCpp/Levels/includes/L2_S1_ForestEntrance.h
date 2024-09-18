//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 01 : Forest Entrance
//====================================

#ifndef DQFYLH_LEVEL2_SCENE1_FOREST_ENTRANCE_DEF
#define DQFYLH_LEVEL2_SCENE1_FOREST_ENTRANCE_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Tutorial.h"
#include "MeshObject.h"
#include "L2S1_Pop1HoneyComb.h"
#include "L2S1_Pop2UndergroundDoor.h"
#include "L2S1_Pop3AloeveraPlant.h"
#include "L2S1_Pop4ClearWater.h"
#include "L2S1_Pop5Scrapper.h"
#include "ClickablePolygon.h"
#include "SuperFX.h"
#include "CVideo.h"
#include "VideoDefs.h"
#include "KVideo.h"
#include "AssetsStringTable.h"

enum eL2S1HOPopEnums
{
	eHOS1Pop_Inactive =-1,
	eHOPop1_HoneyComb,
	eHOPop2_UndergroundDoor,
	eHOPop3_AloeveraPlant,
	eHOPop4_CrystalClearWater,
	eHOPop5_Boat,
	eHOS1PopCount
};

class CHOScene;

class L2_S1_ForestEntrance: public HiddenObjectSystem
{
	public:
		
		bool isSwimCutscenePlaying;
		CVideo *cv_swimCutscene;
		float time;
		float vidTimer;

		int subIndex;
		subText subtitles[5];
		CBitmapText* subtitleText; int subMax;
		SuperFX *holinesFX, *hodotsFX;
		SuperFX *BeesFX;
		float musicFadeOutTimer;
		//Objects
		CObject *SCN_cutAloevera;
		CObject *SCN_HoneyCombUncut;
		CObject *SCN_HoneyBee;
		CObject *SCN_Scrapper;

		CObject *SCN_rightPlant;
		CObject *SCN_leftPlant;
		CObject *SCN_mushroom;
		CObject *SCN_lotus;
		CObject *SCN_lotusReflection;
		CObject *SCN_roundLeafset1;
		CObject *SCN_roundLeafset2;
		CObject *SCN_topTreeLeafSet1;
		CObject *SCN_topTreeLeafSet2;
		CObject *SCN_topTreeLeafSet3;
		CObject *SCN_topTreeLeafSet4;
		CObject *SCN_topTreeLeafSet5;
		CObject *SCN_topTreeLeafSet6;
		CObject *SCN_topTreeLeafSet7;
		CObject *SCN_bottomLeafset1;
		CObject *SCN_bottomLeafset2;

		MeshObject *SCN_pondWater;

		//Popups

		L2S1_Pop1HoneyComb *pop1HoneyComb;
		L2S1_Pop2UndergroundDoor *pop2UndergroundDoor;
		L2S1_Pop3AloeveraPlant *pop3AloeveraPlant;
		L2S1_Pop4ClearWater *pop4CrystalClearWater;
		L2S1_Pop5Scrapper *pop5Scrapter;

		ClickablePolygon *pop1ClickPoly;
		ClickablePolygon *pop2ClickPoly;
		ClickablePolygon *pop3ClickPoly;
		ClickablePolygon *pop4ClickPoly;
		ClickablePolygon *pop5ClickPoly;

		Popup *PopUpArray[eHOS1PopCount];

		bool isMouseOverPopup;

		void onPopAreaClicked(int popId);
		void onPopAreaMouseOver(int popId);
		void onPopAreaMouseOut(int popId);
		void onPopupClosed(Popup * popup);

		// -- Popups -- //
		int ActivePopupID;
		

		L2_S1_ForestEntrance();
		~L2_S1_ForestEntrance();

		void Update();
		void Required();

		void SparkleUpdate();
		void TaskPendingUpdate();
		void SceneObjectsUpdate();
		void CheckForPendingTask();

		void InitPopups();
		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif