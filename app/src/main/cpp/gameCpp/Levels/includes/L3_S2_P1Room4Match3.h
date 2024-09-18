//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj / Faisal
// LEVEL3 - SCENE3 : P1Room4Match3
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_P1ROOM4MATCH3_DEF
#define DQFYLH_LEVEL3_SCENE3_P1ROOM4MATCH3_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#include "Popup.h"

#include "JSONAnimator.h"

class CHOScene;

enum eL3S2M4PopEnums
{
	ePop1_Match3,
	ePop2_Pipe,	
	ePopL3S2M4_Max
};

class L3_S2_P1Room4Match3: public HiddenObjectSystem
{
	public:

		bool isDoorOpenAnimPlaying;
		JSONAnimator* JAnim_DoorsOpening;
		void OnAnimationComplete(int id);

		std::vector<SuperFX*>	VFXArray;

		int ActivePopupID;
		
		float popupCloseDelayTimer;
		
		Popup*		PopupArray[ePopL3S2M4_Max];
		int			PopStates[ePopL3S2M4_Max];
		CPolygon*	PopPolyArray[ePopL3S2M4_Max];

		void ProcessSkip();

		void InitPopups();
		void OnPopupClosed(Popup* popup);

		void SparkleUpdate();
		void TaskPendingUpdate();
		void SceneObjectsUpdate();

		void Update();
		void Required();
		
		L3_S2_P1Room4Match3();
		~L3_S2_P1Room4Match3();

		void NavigateToScene();
		
	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif