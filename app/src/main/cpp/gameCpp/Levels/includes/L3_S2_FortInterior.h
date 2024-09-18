//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE2 : FortInterior
//====================================

#ifndef DQFYLH_LEVEL3_SCENE2_FORTINTERIOR_DEF
#define DQFYLH_LEVEL3_SCENE2_FORTINTERIOR_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Tutorial.h"
#include "MeshObject.h"
#include "SuperFX.h"
#include "CustomAnimator.h"
#include "CustomAnimGroup.h"
#include "DialogSystem.h"
#include "L3S2_Pop1Thread.h"
#include "L3S2_Pop2Skull.h"
#include "L3S2_Pop3Coconut.h"
#include "L3S2_Pop4Crow.h"
#include "L3S2_Pop5BackDoor.h"
#include "L3S2_Pop6GunPowder.h"
#include "L3S2_Pop7MazeDoor.h"

#include "CVideo.h"

class CHOScene;

enum eL3S2PopEnums
{
	ePop1_Thread,
	ePop2_Skull,
	ePop3_Coconut,
	ePop4Crow,
	ePop5_BackDoor,
	ePop6_GunPowder,
	ePop7_MazeDoor,
	ePopL3S2_Max
};


class L3_S2_FortInterior: public HiddenObjectSystem
{
	public:

		//Scene Elements
		SuperFX *moonprtFX;
		SuperFX *S2crystalprt1FX, *S2crystalprt2FX, *S2crystalprt3FX;
		float musicFadeOutTimer;
		bool isBlastCutscnPlaying;
		CVideo *cv_BlastCutscn;
		float vidTimer;


		// -- Popups -- //
		int ActivePopupID;
		
		Popup*		PopupArray[ePopL3S2_Max];
		int			PopStates[ePopL3S2_Max];
		CPolygon*	PopPolyArray[ePopL3S2_Max];

		void onPopupClosed(Popup * popup);

		L3_S2_FortInterior();
		~L3_S2_FortInterior();

		void Update();
		void Required();
		void SparkleUpdate();
		void TaskPendingUpdate();
		void SceneObjectsUpdate();
		void ProcessSkip();

		void InitPopups();

		void NavigateToScene();

	protected:
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif