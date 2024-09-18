//====================================
// DQFYLH
// Hidden Object System
// Level 1 - Scene 04 : Bedroom
//====================================

#ifndef DQFYLH_LEVEL1_SCENE4_BEDROOM_DEF
#define DQFYLH_LEVEL1_SCENE4_BEDROOM_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Tutorial.h"
#include "MeshObject.h"
#include "SuperFX.h"
#include "CustomAnimator.h"
#include "CustomAnimGroup.h"
#include "DialogSystem.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

#include "VideoDefs.h" 
#include "KVideo.h"

class CHOScene;

class L1_S4_Bedroom: public HiddenObjectSystem, CRenderListEntity
{
	public:
		//Scene Elements
		CObject *Scn_lightning;
		CObject *Scn_Bg;
		CObject *Scn_SGlowFY;

		DialogSystem *dialogSystem;
		bool isDialogActive;

		CObject *Scn_SlidingDoor, *Scn_SlidingDoorMask;
		CustomAnimator *CAnim_SlideTreasureDoor;
		bool IsSecretTreasureDoorSlidePlaying;
		SuperFX *dustFallVFX;
		
		std::vector<SuperFX*>	VFXArray;
		SuperFX *tableLampVFX, *rightLampVFX, *windowVFX, *hangLampVFX;

		float lightningTimer, nextLightningThreshold, lightFrame;
		float chairSwingTimer;

		bool	IsVidSkippable, IsSkipMouseOver, IsSkipMouseClicked;
		float	SkipShowTimer;
		CObject *skipObj;

		CHOScene *creekTalkScn;

		CVideo *cv_CreekAppear, *cv_CreekTalks;
		CVideo *cv_MayaPushVideo;

		subText subtitles[3], subtitles2[2];
		float time;
		float vidTimer;
		float vidTimer2;
		float vidTimer3;

		int subIndex, subIndex2;
		CBitmapText* subtitleText; int subMax, subMax2;

		bool isCreekVideoPlaying, isCreekIntroPlaying, isCreekOutroPlaying;
		float creekVidStartTimer;

		bool isMayaPushVideoPlaying;

		bool IsZOrderChangeToScene;
		int zOrderScnVideo;
		int zOrderPopVideo;

		// -- Popups -- //
		int ActivePopupID;
		int ActivePop_State;

		CObject *PopOverPopMask;

		CObject *Pop_CloseButton;
		
		CHOScene *Pop1Drawer;
		CHOScene *Pop2KeyFall;
		CHOScene *Pop3BeetleShelf;
		int Popup1_State;
		int Popup2_State;
		int Popup3_State;

		CObject *P1_LeftLockStatic, *P1_RightLockStatic, *P1_SnakeLockAnim, *P1_LeftLockAnim;
		CObject *P1_DrawerClosed, *P1_DrawerOpened, *P1_Diary, *P1_TableTopmask, *P1_FallingKey, *P1_BookKeyMask;
		
		CustomAnimGroup *CAnim_DrawerOpening, *CAnim_DiaryTaking;
		bool IsP1DrawerOpeningPlaying, IsP1DiaryTakingPlaying;
		bool IsP1SnakeKeyAnimPlaying;

		bool isKeyFallSoundPlayed;
		float diaryTakeAnimTimer, diaryTakeDelayTimer;

		CObject *P1_SecretSpace, *P1_BeetleHead, *P1_SmallDoor, *P1_Screw1, *P1_Screw2, *P1_WaxStatic;
		CObject *P1_WaxMeltAnim, *P1_MatchboxInside, *P1_Matchbox, *P1_MatchStick, *P1_MatchStickMask, *P1_MatchSpark, *P1_MatchFlameStick;
		CustomAnimGroup *CAnim_P1MatchStickLighting;
		bool IsP1MatchStickPlaying, IsP1WaxMeltingPlaying;

		CObject *P1_CoinAnim1, *P1_CoinAnim2;
		bool IsCoinAnim1Playing, IsCoinAnim2Playing;

		CObject *P3_TakingBook, *P3_BookOnTable, *P3_BookOpened, *P3_BooksOnShelf, *P3_BookSlide, *P3_BooksOnShelfMoved, *P3_BookLockStatic, *P3_BookLockOpenAnim;
		CObject *P3_KeySet, *P3_ShadowHand, *P3_BeetleFix, *P3_BeetleGlowUp, *P3_BeetleGlowDown;
		CObject *P3_HandWithCode;

		CPolygon *P3BookTakePoly;

		CustomAnimGroup *CAnim_bookTaking, *CAnim_beetleFixing;
		bool IsP3BookTakingAnimPlaying, IsP3BeetleFixAnimPlaying, IsP3LockOpenAnimPlaying;

		std::vector<CustomAnimator*>	custAnimArray;

		float popupCloseDelayTimer;
		// -- Popups -- //

		//Scene Scaling Render Target
		CObject *RT_BgScn;
		float 	bgScaleOffsetX;
		float	bgScaleOffsetY;

		L1_S4_Bedroom();
		~L1_S4_Bedroom();

		//Inherit this CRenderListEntity function to render anything to scene
		void Render(const CPoint* pCam);
		void Update();
		void Required();

		void SparkleUpdate();

		void InitPopups();
		void UpdatePopups(CPoint pos,bool ItemDropped=false);
		void CloseActivePopup(bool isDelayed=false);
		
		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();

		void TakeInventoryItem(EHOInventory ItemID);

		float easingQuadOut(float t,float b,float c,float d);

		void UpdateBgGraphic();
		void EndBgGraphicScaling();

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);

		void TaskPendingUpdate();
		void ProcessNonHOHint();
};

#endif