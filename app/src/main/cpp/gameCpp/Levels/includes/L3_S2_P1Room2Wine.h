//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room2Wine
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_P1ROOM2WINE_DEF
#define DQFYLH_LEVEL3_SCENE3_P1ROOM2WINE_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "L3_S2_M2PipePopUp.h"
#include "L3_S2_P1R2CreeperPuzzle.h"

class CHOScene;

enum M1Room2_Popups
{
	R2_NoPopup = 0,
	R2_PipePickPopup,
	R2_CreeperPuzzle
};

class L3_S2_P1Room2Wine: public HiddenObjectSystem, CRenderListEntity
{
	SuperFX *S2M1SmokeFX, *S2M1Smoke2FX, *S2M1Smoke3FX, *S2M2LightFX;
	std::vector<JSONAnimator*> _janim_RemoveVine;
	M1Room2_Popups _activePopup;
	L3_S2_M2PipePopUp *_pipePopup;
	L3_S2_P1R2CreeperPuzzle *_puzzlePopup;
	float musicFadeOutTimer;
public:

		L3_S2_P1Room2Wine();
		~L3_S2_P1Room2Wine();

		void TaskPendingUpdate();
		
private:

		float easing(float t,float b,float c,float d);
		void Render(const CPoint* pCam);
		void Update();
		void Required();
		void NavigateToScene();

protected:

		void SparkleUpdate();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void ProcessSkip();
		void OnClosePopup(Popup *popup);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void OnAnimatonComplete(int animID);
};

#endif