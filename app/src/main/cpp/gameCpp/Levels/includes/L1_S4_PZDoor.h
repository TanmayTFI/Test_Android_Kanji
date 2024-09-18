//====================================
// DQFYLH
// Level 1 - Scene 03 : DOOR PUZZLE
//====================================

#ifndef DQFYLH_LEVEL1_SCENE3_PZDOOR_DEF
#define DQFYLH_LEVEL1_SCENE3_PZDOOR_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperFX.h"

#include "VideoDefs.h" 
#include "KVideo.h"

class CHOScene;

class L1_S4_PZDoor: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements
		CObject *Scn_Bg;

		bool IsZOrderChangeToScene;
		int zOrderScnVideo;
		int zOrderPopVideo;

		int ActivePopupID;
		
		bool isObjectReturning;
		CPoint startPos,endPos;
		CObject *returnObject;
		int objectID;
		float returnTimer;
		float returnEndTimer;
		CObject *pointerInHand;
		bool isObjectInHand;
		
		float Scalefactor;
		L1_S4_PZDoor();
		~L1_S4_PZDoor();
		float musicFadeOutTimer;
		bool isChangeZOrder;

		int Popup1_State;

		void InitPuzzle();

		CPolygon *polyImages[5];
		CPolygon *polyCircles[5];
		CPoint *pointHands[13];
		CObject *hands[13];
		CObject *shapes[10];
		CObject *success[5];
		CObject *images[5];
		CObject *shutters[5];
		CObject *blueShutters[5];
		CObject *circleSelection[5];
		CObject *polySelection[12];
		CObject *locks[5];
		CObject *LockAnim[5];
		CObject *ResetBtn;
		int lockIndex;
		bool isLockAnimationPlaying;
		CPolygon *polyHands[12];
		int shapeRightIndices[5][2];
		bool isHandFixed[5][2];
		int shapeCurrentIndices[5][2];
		bool isHandSelectable[12];
		bool isHandSelected;
		bool isDoorOpened;
		bool isShutterOpening;
		float timerForShutter;
		float timeToOpenShutter;
		float shutterStartYPos,shutterEndYPos;
		CObject *door;
		CObject *DoorOpenAnim;
		bool isDoorOpenAnimPlaying;
		CObject *CoinRotationAnim;
		bool isCoinRotationAnimPlaying;
		CPoint coinRotationPos[5];
		int shutterIndex;
		CPoint path[16];
		int selectedIndex;
		int selectedIndex2;
		int noOfPoints;
		bool isHandMoving;
		float moveTimer;
		float movingTime;
		int pathPointer;
		CObject *movingObject;
		int targetIndex;
		CPoint handsInitialPositions[12];
		bool isHandReversing;
		int handDestinationIndex;
		int solvedIndex;
		bool isPuzzleSolved;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		CObject *keyOnScene;
		CObject *keySel[3];
		CObject *keyPointer[3];
		CPolygon *polyKey[3];
		CPolygon *polyKeyHole;
		CObject *KeyAnim[3];
		bool isKeyAnimPlaying;
		
		CPolygon *getPoly[3];
		CPolygon *setPoly[3];
		CObject *getObject[3];
		CObject *setObject[3];
		std::string mousePointerString[3];

		SuperFX *shutterVFX, *lockVFX, *successVFX;
		
		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);
		void InitPopups();
		void UpdatePopups(CPoint pos,bool ItemDropped=false);
		
		bool IsPopupTransitionActive;
		float PopupAlpha;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();
		void ProcessSkip();
		void ResetPuzzle();

		float easing(float t,float b,float c,float d);

		void NavigateToScene();
	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif