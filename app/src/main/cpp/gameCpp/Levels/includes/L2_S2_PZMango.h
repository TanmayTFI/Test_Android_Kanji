//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 02 :MANGOPUZZLE
//====================================

#ifndef DQFYLH_LEVEL2_SCENE2_PZMANGO_DEF
#define DQFYLH_LEVEL2_SCENE2_PZMANGO_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Circle.h"
#include "CustomAnimGroup.h"

class CHOScene;

class L2_S2_PZMango: public HiddenObjectSystem
{
	public:
		//Puzzle Elements
		float musicFadeOutTimer;
		CObject *stone;
		CObject *handReady;
		CObject *CollectableMango[3];
		CObject *CollectableMangoBG[3];
		int leafAnimIndex;
		int TMIndex;
		CObject *mangoEFX;
		CObject *LeafFallAnim;
		bool isLeafFallAnimating;

		bool isMangoHitAnimPlaying;

		CustomAnimGroup *CAnim_StoneThrow, *CAnim_StoneThrowComplete;

		int collectedMangoCount;

		CObject *mango1,*mango2,*mango3;
		CObject *hLine,*vLine;

		CObject *activeMango;
		float timerGameOver;

		int ActivePopupID;

		L2_S2_PZMango();
		~L2_S2_PZMango();

		void InitPuzzle();
		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();
		void ProcessSkip();
		void AnimateLines(CObject *mango);
		bool isLineAnimating;
		bool isStoneThrowing;
		bool isMangoDroping;
		float mangoStartY;
		float mangoEndY;
		void showStoneThrowAnim(int num);
		bool isThrowStoneHandAnimation;
		bool isHandAnimation;
		int throwStoneAnimIndex;

		bool isStoneStrew;

		float handReadyTimer;
		void startHandReadyTimer();
		bool isHandReadyTimerOn;
		bool isPuzzleSolved;

		void ThrowMango(CObject *mango,bool isHit);
		bool isHit;
		float swapTime;

		float stoneTargetX;
		float stoneTargetY;

		float stoneOriginalX;
		float stoneOriginalY;
		float stoneRotationStartValue;
		float stoneRotationEndValue;
		
		float easing(float t,float b,float c,float d);
		bool isRotating;
		bool scaleValue;
		
		float tweenTime;
		float rotTime;
		float dropTime;
		float yTarget,yValue;
		float xTarget,xValue;

		float vX;
		float vY;
		float dX;
		float dY;
		float aX;
		float aY;

		float force;
		float gravity;
		float angle;

		float scaleFactor;
		float rotationValue;

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};


#endif

