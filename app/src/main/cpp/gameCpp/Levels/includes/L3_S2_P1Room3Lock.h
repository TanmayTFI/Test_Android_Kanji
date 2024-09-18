//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room3Lock
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_P1ROOM3LOCK_DEF
#define DQFYLH_LEVEL3_SCENE3_P1ROOM3LOCK_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"

class CHOScene;

enum L3_S2_P1R3Popups
{
	P1R3_NoPopup = -1,
	P1R3_PipePickPopup,
	P1R3_Door1Popup,
	P1R3_Door2Popup,
	P1R3_Door3Popup
};

class L3_S2_P1Room3Lock: public HiddenObjectSystem
{

	CObject* _stonesClosed;
	CObject* _stonesCleared;
	float musicFadeOutTimer;
	std::vector<CObject*> _doors;

	L3_S2_P1R3Popups _activePopup;
	std::map<L3_S2_P1R3Popups, Popup*> _scenePopups;

	public:

		L3_S2_P1Room3Lock();
		~L3_S2_P1Room3Lock();
		
		float easing(float t,float b,float c,float d);
		void Update();
		void Required();

		void TaskPendingUpdate();
		void NavigateToScene();

	protected:

		void SparkleUpdate();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();

		void OnClosePopup(Popup *popup);
};

#endif