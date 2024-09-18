//====================================
// DQFYLH
// Hidden Object System
// Level 1 - Scene 04 :  SECRETDOOR HO
//====================================

#ifndef DQFYLH_LEVEL1_SCENE4_SECRETDOORHO_DEF
#define DQFYLH_LEVEL1_SCENE4_SECRETDOORHO_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperFX.h"
#include "Match3.h"

#include "CustomAnimator.h"
#include "CustomAnimGroup.h"

#include "VideoDefs.h" 
#include "KVideo.h"

class CHOScene;

class L1_S3_DoorHO: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements
		CHOScene *LocketSCN;

		CObject *hoListPanel, *hoLocket, *hoLocketMask, *hoLocketFixHolder;
		CObject *hoLocketShadow, *hoLocketGlow,  *hoListPanelTake, *hoListPanelTakeGlow;

		CustomAnimGroup *CAnim_LocketPlaceMove, *CAnim_HoListPanelTake;

		bool isLocketPlaceAnimPlaying, isHOListPanelTakePlaying;

		bool isSymbolText;
		float musicFadeOutTimer;
		Match3 *match3;
		bool isMatch3Playing;
		bool isHOSolved;
		float hoTimer;
		
		CObject *rose;
		CObject *crescent;
		CObject *snake;
		CObject *mask;
		CObject *owl;
		CObject *leaves[3];
		CObject *lionhead;
		CObject *Stars[2];
		CObject *tree;
		CObject *goddes;
		CObject *turtle;
		CObject *fish;

		CObject *fishhead;
		CObject *fishtail;
		CObject *keyInside;
		CObject *btmLDoor;
		CObject *btmRDoor;
		CObject *stardoor;

		CObject *gemGlow[4];
		CObject *glowLeft;
		CObject *glowRight;
		CObject *glowTop;
		CObject *glowDown;
		CObject *glowCircle;
		CObject *glowRect;
		CObject *topLDoor;
		CObject *topRDoor;
		CObject *doorgembg;

	CObject *match3Back, *match3BackText;


	CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		CObject *button[7];

		CObject *stardoorOpened;
		CObject *stardoorAnim;
		bool isStarDoorOpening;

		bool isShutterOpening;
		float topLDoorXPosEnd, topLDoorXPosStart;
		float topRDoorXPosEnd, topRDoorXPosStart;
		float btmLDoorXPosEnd, btmLDoorXPosStart;
		float btmRDoorXPosEnd, btmRDoorXPosStart;
		float shutterTimer;

		CObject *txtMatch3;
		CObject *txtMatch3MouseOver;
		CObject *txtPuzzle, *txtPuzzleMouseOver;

		CObject *swapHand[4];
		CObject *swapHand2[2];
		CObject *swapLeverSel[4];
		int swapLeverIndex[2];
		bool isLeverSwaping;
		float swapAngleStart[2];
		float swapAngleEnd[2];

		CObject *movingObjects[2];
		CObject *keyfixed;
		CObject *targetSel[7];
		CObject *LockSel;
		bool isLockSelected;
		int currentLockPos;
		bool isLockMoving;
		float moveTimer;
		CPoint keyStartPos;
		CPoint keyEndPos;

		bool isLockMovedOnce;
		int nextEndPosIndex;

		CObject *lockGlow[7];
		int selectedLockPoints[2];
		int selectedLockPointer;

		CPolygon *polyRoseflower;
		CPolygon *polyCrescent;
		CPolygon *polySnake;
		CPolygon *polyMask;
		CPolygon *polyOwl;
		CPolygon *polyLeaves[3];
		CPolygon *polyLionhead;
		CPolygon *polyFishTail;
		CPolygon *polyFish;
		CPolygon *polyStars[2];
		CPolygon *polyTree;
		CPolygon *polyGodess;
		CPolygon *polyTurtle;
		CPolygon *polyMemoryPuzzle[4];
		CPolygon *polyKey;
		CPolygon *polyKeyHole;

		CPolygon *polyLockTargetNew[7];
		CPolygon *polySwapLever[4];

		int memoryPuzzlePointer;
		bool isMemoryPuzzleReversing;
		bool isMemoryPuzzleSolved;
		float startYPos, endYPos;
		int fromPos,toPos;
		CPoint keyValues[3];
		CObject *swapLeverMouseOver;

		CObject *hintBG;
		CObject *hintImages[12];
		int hintImageIndex;

		bool imageState[12];
		void updateImageState();

		CRectangle hintImageRects[12];

		void checkAllHiddenObjectsFind();

		int memoryPuzzleCorrectOrder[4];

		float swapCurrectAngles[4];
		
		bool isObjectReturning;
		CPoint startPos,endPos;
		CObject *returnObject;
		int objectID;
		float returnTimer;
		float returnEndTimer;
		CObject *pointerInHand;
		bool isObjectInHand;
		bool isBoxSwaping;
		bool isLockPuzzleSolved;

		SuperFX *clickVFX;
		
		L1_S3_DoorHO();
		~L1_S3_DoorHO();
		
		float swapTime;
		
		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);

		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();

		void InitializeText();
		void UpdateText();
		void UpdateLeafStarText();
		void DisableText();
		void setHOTextVisible(bool visible=false);
		void ResetHO();

		CBitmapText *txtRoseFlower,*txtCrescent ,*txtSnake, *txtMask, *txtOwl, *txtLeaves, *txtLionHead, *txtFish, *txtStars, *txtTree, *txtGoddess, *txtTurtle, *txtPuzzleInfo;
		float easing(float t,float b,float c,float d);

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void ShowHintForLocketPuzzle(int lockID);
};

#endif