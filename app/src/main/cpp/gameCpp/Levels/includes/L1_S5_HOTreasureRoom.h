//====================================
// DQFYLH
// Hidden Object System
// Level 1 - Scene 04 :  SECRETDOOR HO
//====================================

#ifndef DQFYLH_LEVEL1_SCENE5_HOTREASUREROOM_DEF
#define DQFYLH_LEVEL1_SCENE5_HOTREASUREROOM_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperFX.h"
#include "Match3.h"

#include "CVideo.h"

#include "CustomAnimator.h"
#include "CustomAnimGroup.h"

#include "HOInventoryBox.h"
#include "InventoryItemInterface.h"

#include "MultilineText.h"

#include "VideoDefs.h"
#include "KVideo.h"
#include "AssetsStringTable.h"

class CHOScene;

typedef enum
{
	eHOItem1_NeckLocket = 0,
	eHOItem2_BookSymbolPartHorn,
	eHOItem3_EagleEye,
	eHOItem4_Clockneedle,
	eHOItem5_DrawerHandle,
	eHOItem6_KeyRingBox,
	eHOItem7_BookSymbolPartHead,
	eHOItem8_TokenBoxPuzzle,
	eHOItem9_Pestle,

	eHOItem_MAX
}HOItemIndex;

typedef enum
{
	HOITEM_LOCKED,
	HOITEM_UNLOCKED,
	HOITEM_TAKEN,
	HOITEM_USED,

	HOITEM_STATE_MAX
}HOItemState;

enum eL1S5HOPopEnums
{
	eHOPop_Inactive =-1,
	eHOPop1_BookLocketPop,
	eHOPop2_ColorSlidePuzzleBox,
	eHOPop3_Clock,
	eHOPop4_StatueHanuman,
	eHOPop5_Eagle,
	eHOPop6_ShelfSwapPuzzle,
	eHOPop_Max
};

enum eL1S5POPSTATES
{
	HOPOP_INACTIVE=-1, HOPOP_SCALEUP, HOPOP_OPEN, HOPOP_SCALEDOWN, HOPOP_CLOSED
};

#define MAX_L1S5HO_P2COLORTOKENS 8

class L1_S5_HOTreasureRoom: public HiddenObjectSystem, CRenderListEntity, InventoryItemInterface
{
	public:
		//Scene Elements
		//CBitmapText *DebugText;

		SuperFX *vfx_goldGlit, *vfx_flies, *vfx_glares, *vfx_collectFX;

		//Popups
		int ActivePopupID;

		bool IsMortarPestleCollectWaiting;
		float mortarCollectTimer;

		CHOScene *Pop_SCNArray[eHOPop_Max];
		int Pop_States[eHOPop_Max];
		CPolygon *Pop_ScnPolyArray[eHOPop_Max];
		CRectangle *DestRect_HOItemArray[eHOItem_MAX];
		CRectangle *ItemRect_HOItemArray[eHOItem_MAX];
		
		//eHOPop4_StatueHanuman
		SuperFX *p4Vfx_locketfix1, *p4Vfx_locketfix2;
		CustomAnimator *CAnim_LocketFix;
		float p4LocketFixFadeInTimer;
		bool p4IsLocketFixAnimPlaying, p4IsHandOpenFadingIn;

		//eHOPop3_Clock
		CustomAnimator *CAnim_ClockNeedleFix;
		float p3NeedleRotateTimer;
		bool p3IsClockNeedleFixPlaying, p3IsClockNeedlesRotating;

		//eHOPop2_ColorSlidePuzzleBox
		CObject *p2BtnLocketArray[5];
		CObject *p2ColorTokenArray[MAX_L1S5HO_P2COLORTOKENS];
		int p2PosContains[MAX_L1S5HO_P2COLORTOKENS];
		int p2SwappingTokenIds[4];
		int p2SwapPosIds[4];
		CPoint p2TokenPosArray[MAX_L1S5HO_P2COLORTOKENS];
		CPoint p2StartObjPt[4], p2EndObjPt[4];
		float p2TokenSwapTimer, p2TotalSwapTime;
		bool p2IsTokenSwapping, p2IsTokenRotating, p2IsFirstSwapObjClicked;
		int p2SwapId1, p2SwapId2, p2Swap1PosId, p2Swap2PosId;

		SuperFX *p2Vfx_PuzEndFX;
		float p2PuzzleEndTimer;
		bool p2IsPuzzleEndPlaying;
		CObject *p2HelpBtn, *p2SkipBtn, *skipText, *p2SkipBtnGlow, *p2SkipBtnActive;
		float p2SkipPuzzleTimer, p2SkipPuzzleThreshold;

		CObject *p2HelpFrameBG;
		MultilineText *p2Helptext;

		//eHOPop1_BookLocketPop
		bool p1IsKeyTurnAnimPlaying;
		float p1KeyTurnTimer;
		CObject *p1KeyTurnAnim;

		bool p1IsWaitingForBookOpen;
		float p1BookOpenWaitTimer;
		SuperFX *p1Vfx_BookOpen;

		CObject *Pop_CloseButton;

		CObject *SCN_bookClosed;
		CObject *SCN_bookLockPart1;
		CObject *SCN_bookLockPart2;
		CObject *SCN_bookLockPart3;
		CObject *SCN_bookLockPartFixed1;
		CObject *SCN_bookLockPartFixed2;
		CObject *SCN_bookLockPartFixed3;
		CObject *SCN_bookLockPartFixed4;
		CObject *SCN_bookOpen;
		CObject *SCN_chestBoxOpen;
		CObject *SCN_chestBoxSymbol;
		CObject *SCN_clockOpen;
		CObject *SCN_clothFolded;
		CObject *SCN_clothOpen;
		CObject *SCN_eagleEye;
		CObject *SCN_eagleEyeInHand;
		CObject *SCN_eagleMouthOpen;
		CObject *SCN_grinderWithPestile;
		CObject *SCN_grinderOnly;
		CObject *SCN_handClosed;
		CObject *SCN_handle;
		CObject *SCN_keyOnBox;
		CObject *SCN_locket;
		CObject *SCN_locketOnScene;
		CObject *SCN_musicboxOpen;
		CObject *SCN_pestle;
		CObject *SCN_ringBox;
		CObject *SCN_ringBoxCoin;
		CObject *SCN_ringBoxOpen;
		CObject *SCN_statueHandOpen;

		CObject *SCN_ItemsOnHand[eHOItem_MAX];
		int objectInHandId;
		bool isObjectInHand;
		
		Match3 *Treasurematch3;
		bool isMatch3Playing;		

		CObject *BtnMatch3;
		CObject *BtnSwitchToHO;

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

	CObject *match3Back, *match3BackText;


	float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		//P1BookHO

		CObject *P1bookLockPart1;
		CObject *P1bookLockPart2;
		
		CObject *P1bookLockPartFixed1;
		CObject *P1bookLockPartFixed2;
		CObject *P1bookLockPartFixed3;
		CObject *P1bookLockPartFixed4;
		
		bool	 isP1bookLockPartSelected;
		
		int		 P1SelectedLockPart;


		void UpdateP1BookHO(CPoint pos);


		//P6_ShelfPieceSwap HO
		CObject *P6_shelfOpen;
		CObject *P6_Key;
		CObject *P6_handleFixed;
		CObject *P6_bookLockPart;
		CObject *P6_SwapPiece[7];
		CObject *P6_ShelfClosed;
		CObject *P6_ShelfBorder;
		CObject *P6_shelfOpenAnim;
		bool	isP6SwapPieceSelected;
		int		P6_SelectedPieceIndex;
		int		p6_prevSelectedPieceIndex;
		int		p6_IndexArray[7];
		int		p6_randomIntegerArray[7];
		CPoint  p6_positionArray[7];
		bool	isP6SwapHOPuzzleSolved;
		bool	isP6SwappingInProgress;
		CustomAnimGroup  *CAnim_P6ShelfOpen;
		bool	isP6ShelfOpenAnimPlaying;
		void InitPopups();
		void InitHOItemsAndHUD();

		HOInventoryBox *HOInvBox;
		void OnInventoryItemClick(int invItemType);
		void UpdateInventoryChangesToProfile(int itemId);

		void OnItemOnHandClickHandler(CPoint pos);

		bool checkForDestAreaClicks(CPoint pos);


		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		void SetupPopupTransition(CHOScene *Popup, float centerX, float centerY);
		void UpdatePopupTransition();
		void SceneObjectsUpdate();

		void UpdatePopups(CPoint pos,bool ItemDropped=false);
		float popupCloseDelayTimer;
		void CloseActivePopup(bool isDelayed=false);
		void PopupClosePostUpdate(int popupId);
		
		void UpdateP6SwapPieceHO(CPoint pos);
		bool CheckP6SwapPieceHoComplete();
		void SwapHOPiecesP6();

		bool	IsVidSkippable, IsSkipMouseOver, IsSkipMouseClicked;
		float	SkipShowTimer;
		CObject *skipObj;
		
		CVideo *cv_LasyaAttack;

		float time, vidTimer;
		int subIndex;
		subText subtitles[3];
		CBitmapText* subtitleText; int subMax;

		bool isLasyaAttackVidPlaying;

		L1_S5_HOTreasureRoom();
		~L1_S5_HOTreasureRoom();
		
		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);

		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();

		void ResetHO();
		
		float easingQuadOut(float t,float b,float c,float d);
		void NavigateToScene();

	protected:


		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif