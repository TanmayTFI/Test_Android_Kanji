//====================================
// Framework
// Hud Control
//====================================

#ifndef DQFULP_HUD_DEF
#define DQFULP_HUD_DEF

#include "Control.h"
#include "BuildConstants.h"
#include "DataEnums.h"
#include "HiddenObjectEnums.h"
#include "HOObject.h"
#include "IGMHeader.h"
#include "Journal.h"
#include "StrategyGuide.h"
#include "SuperFX.h"
#include "LangDefs.h"
#include "Tutorial.h"
#include "BitmapText.h"
#include "MultilineText.h"

#include "InventoryBox.h"
#include "ComboInventory.h"
#include "Map.h"
#include "Teleport.h"
#include "AwardInterface.h"
#include "Cheats.h"

#define MODE2_TEXT_LINES 5

#define NUM_PCOCK_FEATHERS 7

#ifdef _CHEATS 
	#define MORPHOBJECT_MINTIME 0
	#define MORPHOBJECT_MAXTIME 1
#else
	#define MORPHOBJECT_MINTIME 5
	#define MORPHOBJECT_MAXTIME 12
#endif

#define REPLAY_MORPHOBJECT_MINTIME 0
#define REPLAY_MORPHOBJECT_MAXTIME 1

enum eHudElements
{
	HE_Menu,
	HE_Map,
	HE_Hint,
	HE_Skip,
	HE_Journal,
	HE_Guide,
	HE_MessageBanner,	
	HE_Max
};

enum eHOState
{
	HOActive,
	HOInactive
};

class CHOScene;
class CObject;
class CBitmapText;
class FoundInvPopup;
class InventoryBox;

class UnlockPopup{
	CObject		*_textBgSpr;
	CBitmapText *_textTitle;
	CBitmapText *_textDescrip;

	SuperFX *_addFX;
	bool	_isShowing;
	float	_alpha;
	//0 - inactive, 1 - fadein, 2 - idle, 3 - fadeout
	int		_state;

	float _delayTimer;
	float _textStaticTime;

public:
	UnlockPopup(SuperFX *addFx);
	~UnlockPopup();

	void SetText(std::string titleText, std::string descriptionText);
	void Update(float ds);
	void ShowPopup(float delay=0);

	void UpdateAlpha();
};

class HudControl : public Control, AwardInterface
{
	public:
		eHOState HOState;

		bool AllowInput;
		
		Journal	 *JournalCtrl;
		bool IsDiaryShowing;

		StrategyGuide *GuideCtrl;
		bool IsGuideShowing;

		bool IsHintRechargeInstant;	

		InventoryBox	 *InvBoxCtrl;

		ComboInvSystem *comboInvSystemCtrl;
		bool IsComboInvPopupShowing;

		CPolygon *hudLockedPoly, *hudMinimizedPoly;
		bool IsMouseOverHUD(const CPoint* pos);

		void SendUpdateSceneMessage();
		void SendNewAreaMessage();

		void InvBox_StopPuzzleMode();
		void InvBox_StartPuzzleMode();
		bool IsPuzzleScene;

		bool IsHOScene;
		bool IsTakeAndUseHOScene;

		SuperFX *orbFX;

		HudControl( int InventorySize);
		virtual ~HudControl();

		void HideHud(bool fading=false);
		void ShowHud(bool fading=false);

		void FadeInOrb();
		void SendOrbFXToObjectPos(CPoint memoryObjectPos);
		void ShowOrbGlowing();
		void HideOrbGlowing();

		bool isHUDVisible;
				
		//Inherited method
		void Update();
		void Required();
		void ProcessMessage(sMessage message);

		//Aux Hud Functions
		bool isMultipleItemsFlag;
		void ShowInventory(std::vector<int>* vecInvIds);
		void ShowInventory(std::vector<int>* vecInvIds, bool isMultipleItems);
		void ShowInventoryDirect(std::vector<int>* vecInvIds,CPoint pos);

		//note: Set Profile taken key before calling this function for the invItemId
		void TakeInventoryItem(int invItemId);
		void TakeInventoryItem(int invItemId, bool isMultipleItems);
		void TakeInventoryItem(int invItemId, CPoint pos);

		void CloseInventory();
		void ReleaseDrag();
		void GlowDragObject(bool glow=true);
		void CancelDrag();

		//itemVal corresponds to inventory enum
		bool GetDropInfo(CPoint& hotspot, int& itemVal);

		bool IsInvBoxOpen();
		bool IsDraggingItem();
		bool Collide(const CPoint* collidepos); // Check a Collision with any HUD piece

		void ShowJrnEntryFX();
		void ShowObjectiveFX(int ObjId, bool IsCompleted=false);
		

		void ShowNewInfoTextBanner();

		void EnableInput(bool f);
		
		int PrevCursorMode;

		void OnAwardCompleted(int awardId);

		int	   CheckPopupButton(CObject *Button);

#ifdef _CEBUILD
		// ----------- Collectables ------------------- //

		bool isMouseOverExtraButton;

		//Morph_Objects
		CObject *morphCollectObj, *morphDummyObj;
		void setMorphObjects(CObject* collectObj, CObject* dummyObj);
		void setMorphObjRect(CRectangle cRect);
		void HideMorphObjects();
		void ShowMorphObjects();
		void HideMorphCollectedObject();
		void ShowMorphCollectedObject();
		CRectangle morphRect;

		float morphTimer, morphTimerThreshold, morphFadeTimer;
		bool isMorphObjectsActive;
		bool isMorphFadingIn, isMorphFadingOut;
		
		bool isMorphObjectVisible, isMorphTutorialActive;

		void ShowMorphObjectCollected();

		CObject *collectMorphFrameBG;
		bool isMorphObjectCollected, isMorphCollectibleFlying, isMorphCollectibleUIShowing;
		CPoint morphObjStartPt, morphObjCtrlPt, morphObjEndPt;
		float morphCollectAnimTime;
		MultilineText *morphText;
		CObject *btnGotoMorphButton;
		bool isMouseOverMorphButton;

		//Memory_Items
		void ShowMemoryObjectCollected_CE(int memId, CPoint* memPos);
		float memoryObjFadeTimer;
		bool isMemoryObjectCollected, isMemoryObjectFading;
		bool isSceneCloseAfterMemoryFade;
		CObject *btnGotoMemoryButton;

		CObject *memoryCollectObj, *memoryPreviewObj;
		CObject *memoryCollectFrameBG, *memoryCollectItemGlow, *memoryCollectTextBG;

		CPoint memoryObjStartPt, memoryObjCtrlPt, memoryObjEndPt;
		float memoryCollectAnimTime;
		bool isMemoryFlying, isMemoryUIShowing;
		MultilineText *MemoryText;
		CObject* textBG_Mask;

		bool isMouseOverMemoryButton;

		//Awards
		CObject *awardIcon;
		int currentAwardId;
		bool isAwardUIShowing, isMouseOverAwardButton, isAwardFadingOut, isAwardUIPaused;
		float awardCollectAnimTime;
		MultilineText *AwardText;
		CObject *awardCollectFrameBG;

		//awardType for sub categories
		void ShowAwardAchieved(int awardId);
		void HideAwardUI();
		void PauseAwardUI(bool paused);

		bool isAwardRemovedFromQueue;

		CObject *btnGotoAwardsButton;
		CObject *awardsHideText, *awardsHideTick;

		CObject *extrasBtnText, *achBtnText, *memoriesBtnText;

		SuperFX *morphVFX, *awardsVFX;
		// ----------- Collectables ------------------- //
#endif 

		SuperFX *DiaryAddVFX, *HintChargeVFX;

		UnlockPopup *characterUnlockPopup;

		bool	IsNewInfoTextShowing;
		float	newInfoTextTimer;
		CObject		*NewInfoTextBgSpr;
		CBitmapText *NewInfoText;

		//Objectives
		CObject		*ObjectiveTextBgSpr;
		CBitmapText *ObjectiveTitle;
		CBitmapText *ObjectiveTextDescrip;

		bool	IsObjectiveTextShowing;
		float	ObjectiveTextsAlpha;
		int		ObjectiveCounter;	//For Fade IN-OUT States

		Tutorial  *Tutor;
#ifdef K_ANDROID
		CBitmapText *noOfHintsText,  *adBtnText, *buyBtnText;
		MultilineTextBox *adViewDialogText;
		CObject *adViewDialog, *viewADButton, *buyHintsButton;
		int adType;
		bool adORBuyHudActive;
		void showADorBuyDialog(bool f, int type);
		CHOScene *adViewDialogScene;
#endif
		CObject	  *Elements[HE_Max];
		float HintVert;
		float hintAngle, hintMaxAngle;
		bool IsHintDischarging, IsHintFullyCharged, IsHintPaused;
		void  InitHintAnim();
		void  UpdateHint();
		void  RestartHintFX();

		void ShowJournalFromMap(bool showCharacterPage=false);
		void ShowMapFromJournal();

		void UnlockCharacter(int id, float delay=0);

		//Hint stuffs
		//0 - Centre feather, 1-3 - Left feathers, 4-6 - Right feathers
		CObject *pCockFeathers[NUM_PCOCK_FEATHERS], *pCockHeadIdle, *pCockHeadWakeup, *pCockHeadWakeIdle;
		bool isPCockWakeupPlaying, isPCockWakeDownPlaying;
		CObject *HintRechargeTextBg;
		CBitmapText *HintRechargeWarnText;
		bool IsRechargeWarningShowing;

		CPolygon *BtnPolyMap, *BtnPolyDiary, *crystalOrbPoly;

		FoundInvPopup *foundInvPopupCtrl;
		
		void BackToMainMenu();
		void  UseCurrentHint();		

		float TransOutFactor;
		eMessageBannerState BannerState;
		float BannerTimer;
		float BannerAlpha;	
		CBitmapText *MessageText;

		void InitMessageBanner();		
		void UpdateBanner();
		void TransOutBanner();		
		bool HideMessageBanner(void);		

		void ShowNothingToDoMsgBanner();

		void CollectMysteryFlower(int areaID, CPoint pos);
		void ShowMemoryObjectCollected_SE(int memID, CPoint *pos);

		// Immediately suppress the message, bool return whether or not there actually was a message to suppress
		void ShowMessageBanner(const std::string& message, int SFXId = SFX_SOFT_NOTIFY);
		void ShowMessageBanner(int InfoTxtId, int SFXId = SFX_SOFT_NOTIFY);

		void ShowBannerText(const std::string& keyString, int SFXId = SFX_SOFT_NOTIFY);
		void ShowBannerText(int InfoTxtId, int SFXId = SFX_SOFT_NOTIFY);
	// This is to show text directly on banner , bypassing the text fetching from string table
		void ShowBannerDirectText(const std::string& textString, int SFXId = SFX_SOFT_NOTIFY);

	//Used to call Pause Menu from outside HUD, mainly when Back key is pressed in ANDROID
		void CallPauseMenu();


		CHOScene  *HudScene;
		CObject *skipText, *hintText, *guideText, *mapText, *menuText;

		TeleportSystem *teleportSystem;

		bool IsMapShowing;
		Map	 *MapControl;
		void ShowMap();

		IGMPopup_State	 PopupState;

		//Hint particle Stuffs
		CObject *HintDirCursor;
		bool IsDirectionalHint;

		SuperFX *HintTrailSuperFX, *HintTrailSoftSuperFX;
		SuperFX *HintCircleSuperFX, *HintExplodeSuperFX;

		SuperFX *MemoryTrailSuperFX, *MemoryTrailSoftSuperFX, *MemoryExplodeSuperFX;
		SuperFX *paptusFX, *paptusglowFX;

		SuperFX *HintGreenCircleFX;

		SuperFX *Hint2FX;

		void  InitHintTrailEmit(float x=0, float y=0, bool isInstantRecharge=false, eCursorAnim cursorId=CA_Normal);
		void  InitHintTrailEmit(const CRectangle *destRect, bool isInstantRecharge=false, eCursorAnim cursorId=CA_Normal);
		void  ShowHintForInvItem(EHOInventory inventoryItemId, bool isInstantRecharge=false, eCursorAnim cursorId=CA_Normal);
		void  ShowHintForInvItem(EHOInventory inventoryItemId, CPoint* destPos, bool isInstantRecharge=false, eCursorAnim cursorId=CA_Normal);
		void  ShowHintForInvItem(EHOInventory inventoryItemId, const CRectangle* destRect, bool isInstantRecharge=false, eCursorAnim cursorId=CA_Normal);
		void  ShowHintForInvItem(EHOInventory inventoryItemId, CPolygon* destPoly, bool isInstantRecharge=false, eCursorAnim cursorId=CA_Normal);
		void  ShowHintForSceneItem(CPoint* destPos);

		bool  ShowHintForComboInvItems();

		float HintTrailTime;
		bool  IsHintTrailRunning;
		bool  IsHintTrailCircling;

		float MemoryTrailTime;
		bool  IsMemoryFXTrailing;

		float HintTrailCirclingTime;
		
		CPoint hintStartPt, hintCtrlPt, hintEndPt;
		CPoint memoryStartPt, memoryCtrlPt, memoryEndPt;

	protected:
		CHOScene *InGameMenuPopupScene;
		CObject		*modeBtnOver;
		CHOScene	*OptionsPopupScene;
		CHOScene *RestartPopup;

		CBitmapText* musicVolTxt, * effectsVolTxt, * voiceVolTxt, * diffModeTxt, * customCurTxt, * fullscreenTxt;

		bool IsRestartPopup;

		void UpdateInGameMenuPopup();
		void ShowInGameMenuPopup();

		void UpdateOptionsPopup();
		void ShowOptionsPopup();

		void ShowRestartPopup();
		void UpdateRestartPopup();

		//Select Mode Stuffs
		CHOScene	*SelectModePopupScene;
		void UpdateSelectModePopup();
		void ShowSelectModePopup();

		int SelectedMode;
		CRectangle	*ModeRects[4];
		CObject		*ModeButtons[4];

		//Custom Mode Stuffs
		int iActiveProfileSlot;
		CObject		*CustMode_ItemsBg;
		CObject		*CustMode_HintKnob, *CustMode_SkipKnob;
		CObject		*CustMode_HintTime, *CustMode_SkipTime;
		CObject		*CustMode_TickTutor, *CustMode_TickSparkles, *CustMode_TickHOSparkles;
		CObject		*CustMode_TickTextTips, *CustMode_TickHints, *CustMode_TickSkip, *CustMode_TickAwardPopup;

		int		HintTimeInt,SkipTimeInt;//anim
		bool	IsHintEnabled,IsSkipEnabled;
		bool	IsTutorEnabled,IsSparklesEnabled,IsHOSparklesEnabled,IsTextTipsEnabled, IsAwardNotifyEnabled;

		bool	IsHintKnobDragStarted,IsSkipKnobDragStarted;

		CBitmapText  *ModeDescriptionTexts[MODE2_TEXT_LINES], *modeNameText;		
		void ModeTexts_Init(std::string bigstring, int OffsetY = 0,eTextAlignment AlignStyle = Align_Center, int OffsetX = 0);
		void ModeTexts_Show( bool f );
		void ModeTexts_SetAlpha( float f );
		float ModeTextsLineWidth;

		CPoint MusicDragOrigin, SoundDragOrigin;
		CPoint EnvirDragOrigin, VoiceDragOrigin;
		bool IsMusicKnobDragStarted, IsSoundKnobDragStarted;
		bool IsEnvirKnobDragStarted, IsVoiceKnobDragStarted;
		
		CObject		*MaskSpr, *ModeMaskSpr;
		CObject		*Buttons[IGMB_Max];
		CRectangle   ButtonRects[IGMB_Max];
		CPoint MainMenuPos;

		
		float		 PopupAlpha;
		int			 PopupCounter;				

		//Internal methods
		void ProcessInteraction(int i);
};

extern HudControl* GFHud;

#endif
