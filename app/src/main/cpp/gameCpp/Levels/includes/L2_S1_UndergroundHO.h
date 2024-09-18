//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 01 :  UNDERGROUND HO
//====================================

#ifndef DQFYLH_LEVEL2_SCENE1_UNDERGROUNDHO_DEF
#define DQFYLH_LEVEL2_SCENE1_UNDERGROUNDHO_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "JSONAnimator.h"
#include "Match3.h"
#include "AwardManager.h"
#include "HOInventoryBox.h"

#include "VideoDefs.h" 
#include "KVideo.h"

class ParticleEffect;
class CHOScene;

namespace L2S1UndergroundHO
{
	enum eHOItemIndex
	{
		eHOItem1_DuckFigure = 0,
		eHOItem2_Spear,
		eHOItem3_BoxDuck,
		eHOItem4_BoxStone,
		eHOItem5_BoxUnderLotus,
		eHOItem6_Screw,
		eHOItem7_Screwdriver,
		eHOItem8_StarKey,
		eHOItem9_PentagonKey,
		eHOItem10_PullHandle,
		eHOItem11_KeyDoor,
		eHOItem12_Stone,

		eHOItem_MAX
	};

	enum eHOItemState
	{
		HOITEM_LOCKED,
		HOITEM_UNLOCKED,
		HOITEM_TAKEN,
		HOITEM_USED,

		HOITEM_STATE_MAX
	}; 
}

enum L2S1HOAnimations
{
	Anim_DuckOpen,
	Anim_SpearRock,
	Anim_KeyFix,
	Anim_KeyTurn,
	Anim_LeftLock, 
	Anim_LeftLever,
	Anim_RightLever,
	Anim_RightLock,
	Anim_LeftDoorOpen,
	Anim_RightDoorOpen,
	Anim_ScrewTighten,
	Anim_BoxDoorOpen,
	Anim_ShelfOpen,
	Anim_RemoveMirror
};

class L2_S1_UndergroundHO: public HiddenObjectSystem, InventoryItemInterface
{
	public:

		CObject *Inventory;
		CObject *MirrorFull;
		CObject *MirrorBroken0;
		CObject *MirrorBroken1;
		CObject *RightDoor;

		CObject *LeftDoor;
		CObject *LeftLock00;
		CObject *LeftLock01;
		CObject *LeftLock02;
		CObject *LeftLever001;

		CObject *RightLock01;
		CObject *LeftLever002;
		CObject *LeftLever003;
		CObject *LeftLever004;
		CObject *LeftLever005;

		CObject *RightLock02;
		CObject *RightLock03;
		CObject *RightLever01;
		CObject *RightLever02;
		CObject *RightLever03;
		CObject *RightLever04;
		CObject *RightLever05;

		CObject *Spear;
		CObject *SpearOnClick;
		CObject *screwdriverInScene;
		CObject *DuckBottom;
		CObject *DuckFull;
		CObject *DuckFullOnClick;
		CObject *BoxInsideRock;
		CObject *DoorHandle;
		
		CObject *KeyBoxLeverbase, *KeyBoxLeverFixed, *KeyBoxLeverTurned;

		CObject *LeftScrew;
		CObject *LightAnimation;
		CObject *Rock01;
		CObject *Rock02;		
		CObject *Box1;
		CObject *DuckHeadAnimation1;
		CObject *DuckHeadAnimation2;
		CObject *DuckHeadAnimation3;

		CObject *StarFixed;
		CObject *PentagonFixed;
		CObject *BoxInsideVessel;
		CObject *Moss01;
		CObject *Moss02;
		CObject *Moss03;
		CObject *KeyMask;
		CObject *LeyInKeyOnClick;
		CObject *KeyInKeyHole1;
		CObject *KeyInKeyHole2;
		CObject *RootAnimation2;
		CObject *RootAnimation3;
		CObject *RootAnimation1;
		CObject *ClothAnimation;
		CObject *Plant0;
		CObject *Plant1;

		CObject *Screwdriver0;
		CObject *Screwdriver1;
		CObject *Screwdriver2;
		CObject *Screwdriver3;
		CObject *Screwdriver4;
		CObject *Screwdriver5;
		CObject *ScrewOnGround;
		CObject *ScrewOnClick;
		CObject *ScrewFit;
		CObject *ScrewMask;

		CObject *BoxClosed;
		CObject *BoxPlaced03;
		CObject *BoxPlaced01;
		CObject *BoxPlaced02;
		CObject *BoxOpenMidle;
		CObject *BoxOpen;
		CObject *Star;
		CObject *StarTurned;
		CObject *StarOnClick;
		CObject *pentagon;
		CObject *pentagonTurned;
		CObject *PentagonOnClick;
		CObject *KeyInsideBox;
		CObject *Shelf, *ShelfMask;
		CObject *DoorHandleInPosition;
		CObject *DoorHandleOnClick;
		CObject *Box1Mask;

		SuperFX *s2horippleFX;
		SuperFX *s1hoflies1FX, *s1hoflies2FX;
		SuperFX *s1holeavesFX;
		SuperFX *s1hoBrokenGlassFX;

	CObject *match3Back, *match3BackText;


	CObject		*SCN_ItemsOnHand[MAX_L2S1_HOOBJECTS];
		CRectangle  *ItemRect_HOItemArray[MAX_L2S1_HOOBJECTS];
		CRectangle  *DestRect_HOItemArray[MAX_L2S1_HOOBJECTS];

		void		InitSceneHOObjects();
		void		InitHOItemsAndHUD();
		void		OnItemOnHandClickHandler(CPoint pos);
		bool		CheckIfItemCanBeUsed();

		HOInventoryBox *HOInvBox;
		void OnInventoryItemClick(int invItemType);
		void UpdateInventoryChangesToProfile(int itemId);

		CObject		*HOScnObjects		[MAX_L2S1_HOOBJECTS];
		CObject		*HOInHandObjects	[MAX_L2S1_HOOBJECTS];
		CBitmapText *HOInvboxTexts		[MAX_L2S1_HOOBJECTS];		
		CPolygon	*DestPolyArray		[MAX_L2S1_HOOBJECTS];
		CPoint		 HOBoxPos			[3];
		CObject		*BoxAtPos			[3];
		CObject		*BoxTweenAtPos		[3];
		CObject		*BoxHilight			[3];

		CPoint		DuckPos;
		CPolygon	*MossPoly, *DuckFullPoly, *GlassPoly, *BoxPolyArray;
		
		bool		 isAnimPlaying;

		//Animations
		JSONAnimator *SpearRockAnim, *KeyFixAnim, *KeyTurnAnim, *LeftLockAnim, *LeftLeverAnim, *RightLeverAnim, *RightLockAnim, *LeftDoorOpenAnim, *RightDoorOpenAnim;
		JSONAnimator *DuckOpenAnim, *ScrewTightenAnim, *BoxOpenAnim, *ShelfOpenAnim, *MirrorRemoveAnim;
		

		bool isHOBoxesAligned;
		bool isHOBoxesPlaced;
		bool isBoxOpenAnimPlaying;
		float musicFadeOutTimer;

		int	SelectedHOBoxIndex[2];
		int SelectedHOBoxPos[2];
		int SelectedHOBoxCount;
		int ObjectInHandID;
		int ActivePopupID;
		int TweenCount;

		bool isObjectInHand;

		L2_S1_UndergroundHO();
		~L2_S1_UndergroundHO();

		void InitSceneElements();

		bool updateSceneInteractions(CPoint pos);

		void Update();
		void Required();
		void SceneObjectsUpdate();
		void NavigateToScene();

	protected:

		//Match 3 code
		Match3 *match3Ctrl;
		CObject *BtnMatch3;
		CObject *BtnSwitchToHO;
		//Match 3

		void UpdateBannerMessages( const CPoint pos );
		void OnAnimationComplete(int index);
		void InitAnimations();
		void OnTweenComplete(CObject *obj);
		void HandleHOComplete();
		
		void OpenHOBoxShelf();
		void ShuffleHOBox(CPoint *pos);
		
		void SetHOBoxAtPosition(int boxIndex, int boxPos);

		bool CheckIfHOBoxesAligned();
		void InitControlPolys();
		void SetSceneAnimations();

		bool CheckIfInvItemCanBeUsed(int hoItemIndex);

		bool CheckAndShowHintForSceneInteractions();
		void ShowHintForHOBoxSwap();
		

		void SparkleUpdate();
		void TaskPendingUpdate();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif