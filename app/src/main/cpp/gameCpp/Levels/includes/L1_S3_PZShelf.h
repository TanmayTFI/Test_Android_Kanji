//====================================
// DQFYLH
// Hidden Object System
// Level 1 - Scene 03 : Foyer Shelf Puzzle
//====================================

#ifndef DQFYLH_LEVEL1_SCENE3_PZSHELF_DEF
#define DQFYLH_LEVEL1_SCENE3_PZSHELF_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "CustomAnimator.h"
#include "Circle.h"
#include "SuperFX.h"

#include "VideoDefs.h"
#include "KVideo.h"

class SuperFX;
class CHOScene;

#define NUMSHELFELEMENTS	18
#define NUMCHANGEELEMENTS	7

class L1_S3_PZShelf: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Puzzle Elements
		CObject *ShelfElements[NUMSHELFELEMENTS];
		CObject *flyerZoomed, *flyerOnScene;

		CustomAnimator *CAnim_flyer, *CAnim_flyerBig;

		CObject *Locks[NUMCHANGEELEMENTS];

		CObject *FullOpenLeftDoor, *FullOpenRightDoor;
		CObject *HalfOpenLeftDoor, *HalfOpenRightDoor;
		CObject *ResetBtn;
		CObject *CountGlow, *CountText, *CountFrameBg;

		CObject *HintPuzBtn, *HintPuzBtnGlow, *HintPuzBtnBar;
		float HintPuzTimer, HintPuzThreshold;

		CObject *ChangeElements[NUMCHANGEELEMENTS];
		CPolygon *ChangePolyrects[NUMCHANGEELEMENTS];
		bool IsChangeFound[NUMCHANGEELEMENTS];
		
		bool IsMooppanFlyerZooming, IsMooppanFlyerZoomedIn, IsFlyerFlyingToDiary;

		bool isBackFromKidsRoom;

		L1_S3_PZShelf();
		~L1_S3_PZShelf();
		float musicFadeOutTimer;
		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);
		void InitPuzzle();
		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();
		void ProcessSkip();
		void ResetPuzzle();
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		SuperFX *clickVFX, *shelfBgVFX, *topGlareVFX;
		SuperFX *hintTrailVFX, *hintCircleVFX;
		float hintTrailTime;
		bool isHintTrailActive;

		CPoint hintStartPt, hintCtrlPt, hintEndPt;
		
		bool isPuzzleSolved;

		int currentChangeId;
		int numChangesFound;

		bool isDoorOpening;

		bool isHalfOpenDoorFadingIn;
		bool isFullOpenDoorFadingIn;

		void SetShelfAlpha(float alpha, bool visible=true);

		int tweenType;
		float LockInitX;
		
		float easing(float t,float b,float c,float d);
		bool isTweening;
		float tweenTime;
		float TotalTime;

		float delayTime;

		float startValue,endValue;
		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);

		void TaskPendingUpdate();
		void ProcessNonHOHint();
};


#endif

