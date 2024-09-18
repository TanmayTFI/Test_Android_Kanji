//====================================
//DQFYLH
// Hidden Object System
//====================================

#ifndef DQFYLH_LEVEL1_SCENE4_PZPAINTING_DEF
#define DQFYLH_LEVEL1_SCENE4_PZPAINTING_DEF

#include "HiddenObjectSystem.h"
#include "HOObject.h"

class CHOScene;
class CObject;

#define MAX_PUZ_PIECES 9

#include "KVideo.h"

class L1_S4_PZPainting : public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Objects
		CObject *LeftLockOpened, *RightLockOpened;

		SuperFX *gearSparksVFX, *endGameVFX, *ambVFXFlies, *ambVFXGlares;

		//Painting Puzzle Objects
		CHOScene *PaintPuzzleScn;

		CObject *LeftLock, *RightLock;
		CObject *LeftLockGlow, *RightLockGlow;

		bool IsPaintingPuzzleSolved;

		CObject	*PuzPieces[MAX_PUZ_PIECES];
		CObject	*Gear1, *Gear2;
		CPoint	*PuzPosArray[MAX_PUZ_PIECES];

		CObject *PuzMouseOverCircle;
		CObject *PuzActivePieces[4];

		CPoint *bzStartPos, *bzEndPos, *bzControlPt;

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		bool IsPuzSliding;
		bool IsEndAnimPlaying;
		bool isLockLeftOpened, isLockRightOpened;
		float endAnimTimer;
		float musicFadeOutTimer;
		int SlidePuzId;
		float startX, startY;
		float endX, endY;
		float tweenTime;

		L1_S4_PZPainting();
		~L1_S4_PZPainting();
		
		void ProcessSkip();
		void ResetPuzzle();
		//Inherited Methods
		void Update();
		void Render(const CPoint* pCam);
		void Required();//add
		void NavigateToScene();

	protected:
		float easing(float t,float b,float c,float d);
		CPoint GetBezierPoint(CPoint* p1,CPoint* c1,CPoint* p2,float t);
		void CalcControlPoint(int startId, int endId);
		void InitPuzzle();

		void ReceiveMessage(int val);

		void TaskPendingUpdate();
		void ProcessNonHOHint();
};

#endif