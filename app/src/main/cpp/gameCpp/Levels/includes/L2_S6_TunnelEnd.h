//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 06 : Cave End
//====================================

#ifndef DQFYLH_LEVEL2_SCENE6_CAVE_END_DEF
#define DQFYLH_LEVEL2_SCENE6_CAVE_END_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Tutorial.h"
#include "MeshObject.h"
#include "L2S6_Pop1StonesNKnife.h"
#include "L2S6_Pop2CreeksSkeleton.h"
#include "L2S6_Pop5StoneFace.h"
#include "L2S6_Pop6OilLamp.h"
#include "ClickablePolygon.h"

#include "SuperFX.h"
#include "CVideo.h"

enum eL2S6HOPopEnums
{
	eHOS6Pop_Inactive =-1,
	eHOPop1_StonesNKnife,
	eHOPop2_CreeksSkeleton,
	eHOPop5_StoneFace,
	eHOPop6_OilLamp,
	eHOL2S6Pop_Max
};

class CHOScene;

class L2_S6_TunnelEnd: public HiddenObjectSystem
{
	public:
		float musicFadeOutTimer;
		bool IsWoodCutDown, isVideoPlaying;
		SuperFX *holinesFX, *hodotsFX;

		//Objects
		CObject *rootsToCut;

		SuperFX *fogVFXL, *fogVFXR;

		CVideo *lpVidWoodCutScn; float vidTimer;

		//Popups

		L2S6_Pop1StonesNKnife *pop1Stones;
		L2S6_Pop2CreeksSkeleton *pop2Skeleton;
		L2S6_Pop5StoneFace *pop5StoneFace;
		L2S6_Pop6OilLamp *pop6OilLamp;

		ClickablePolygon *pop1ClickPoly;
		ClickablePolygon *pop2ClickPoly;
		ClickablePolygon *pop5ClickPoly;
		ClickablePolygon *pop6ClickPoly;

		Popup *PopUpArray[eHOL2S6Pop_Max];
		int			PopStates[eHOL2S6Pop_Max];
		CPolygon*	PopPolyArray[eHOL2S6Pop_Max];

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

		// -- Popups -- //
		int ActivePopupID;

		L2_S6_TunnelEnd();
		~L2_S6_TunnelEnd();

		void Update();
		void Required();

		void SparkleUpdate();
		void TaskPendingUpdate();
		void SceneObjectsUpdate();
		void CheckForPendingTask();

		void InitPopups();

		float easingQuadOut(float t,float b,float c,float d);

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif