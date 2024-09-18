//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 01 : Forest Entrance
//====================================

#ifndef DQFYLH_LEVEL2_SCENE2_NAGA_TEMPLE_DEF
#define DQFYLH_LEVEL2_SCENE2_NAGA_TEMPLE_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Tutorial.h"
#include "MeshObject.h"
#include "SuperFX.h"
#include "CustomAnimator.h"
#include "CustomAnimGroup.h"
#include "DialogSystem.h"
#include "L2S2_Pop2Frankinscence.h"
#include "L2S2_Pop3Koolam.h"
#include "L2S2_Pop1NagaHead.h"
#include "L2S2_Pop4Pearls.h"
#include "AssetsStringTable.h"
#include "CVideo.h"

class CHOScene;

enum eL2S2PopEnums
{
	ePop1_NagaHead,
	ePop2_Frankinscence,
	ePop3_Koolam,
	ePop4_Pearls,
	ePopS2_Max
};

class L2_S2_NagaTemple: public HiddenObjectSystem
{
	public:
		bool isCutScneVideoPlaying;
		CVideo *cv_nagaTemple;
		float vidTimer;
		float time;
		int subIndex;
		subText subtitles[5];
		CBitmapText* subtitleText; int subMax;
		float lnagaTempleVideoStartTimer;
		float musicFadeOutTimer;
		//Scene Elements
		SuperFX *holinesFX, *hodotsFX;
		SuperFX *moonprtFX;

		// -- Popups -- //
		int ActivePopupID;
		
		Popup*		PopupArray[ePopS2_Max];
		int			PopStates[ePopS2_Max];
		CPolygon*	PopPolyArray[ePopS2_Max];
		
		void onPopupClosed(Popup * popup);

		L2_S2_NagaTemple();
		~L2_S2_NagaTemple();

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