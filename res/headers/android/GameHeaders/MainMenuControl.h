	//====================================
// Framework
// Main Menu Control :)
//====================================

#ifndef DQFULP_NEWMAINMENU_DEF
#define DQFULP_NEWMAINMENU_DEF
#include "MenuSystem.h"
#include "Control.h"
#include "BitmapText.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "SuperFX.h"
#include "Bezier2D.h"
#include "MeshObject.h"
#include "MultilineText.h"
#include "CustomAnimator.h"

//#include "NodeMeshObject.h"

#include "VideoDefs.h" 
#include "KVideo.h"
#include "Polygon.h"
#include "ExtrasMenuControl.h"
#include "ExtrasComicControl.h"

#define MAX_GAMEMODES 4
#define MODE_TEXT_LINES 5

#define NUM_FLYING_PAPS 5

    enum eMainMenuButtons
    {
        MMB_Play,
        MMB_Options,
        MMB_Exit,
        MMB_Extras,
        MMB_Credits,
    #ifdef K_ANDROID
       // MMB_Unlocknow,
    #endif
        MMB_Max
    };

enum MMPopup_State
{
	MMB_Popup_Inactive = -1,	
	MMB_Popup_ExitGame,
	MMB_Popup_SelectAdventure,
	MMB_Popup_SelectMode,
	MMB_Popup_Credits,	
	MMB_Popup_EraseProfile,
	MMB_Popup_Profiles,
	MMB_Popup_EnterYourName,
	MMB_Popup_Options,
	MMB_Popup_ReviewGame
};

enum MMPopup_UI_Result
{
	MMB_Popup_NoInteraction,
	MMB_Popup_MouseOver,
	MMB_Popup_Click
};

class CBitmapText;
class TextBox;
class CObject;

class MainMenuControl : public Control, CRenderListEntity
{
	public:		
		bool isWaitingForMainChapterRevisit;
		float MainChapterRevisitTicks;
		void InitiateExitPopupFromOutSide();

	    //Message Banner
		void UpdateBanner();
		void InitMessageBanner();		
		void TransOutBanner();
		void ShowMessageBanner(const std::string& message, int SFXId = SFX_SOFT_NOTIFY);
		void ShowBannerText(const std::string& keyString, int SFXId = SFX_SOFT_NOTIFY);
		bool HideMessageBanner(void);
		virtual void BannerCloseNotify(){};
		CBitmapText *MessageText;
		float BannerTimer;
		float BannerAlpha;
		float TransOutFactor;
		bool  IsBannerStandStill;
		eMessageBannerState BannerState;
		CObject	  *MessageElement;

		ExtrasComicControl* extrasComicControl;
		void LoadComicBook();
		void OnComicBookClosed();

		ExtrasMenuControl* extrasMenuControl;

		void LoadKidsroom();
		void LoadCutsceneVideoPlayer();
		void LoadAwardsroom();
		void LoadCredits();

	#if (defined K_WIN32 || defined K_ANDROID)
		void CollectUserData();
		std::string CheckKeyPress(std::string inputText);
		void SetFieldFocus();
		CObject *dataBG, *dataButton, *dataOKButton, *dataCancelBtn, *dataCursor;
		CBitmapText *dataBtnTxt;
		bool isCollectDia;
		int messageBoxWidth;
		CBitmapText *emailAddress, *emailMessage, *emailMsgLine1, *emailMsgLine2, *emailMsgLine3, *emailMsgLine4, *emailMsgLine5, *emailMsgLine6, *emailMsgLine7;
		bool keyPressed[42];
		bool emailFieldHasFocus;
	#endif

		void LoadPuzzleHOForExtras(int levelID, int hoPuzzleAreaID);

		MeshObject* testNodeMesh;
		KVideo *lpVid_fire;

		bool isVidFireLoaded;
		float fireLInitPos, fireRInitPos, fireLPos, fireRPos;		

		CObject *scn_Bg, *scn_MomGlow, *scn_MomRays;
		CObject *scn_pillarLeft1, *scn_pillarLeft2, *scn_pillarRight1, *scn_pillarRight2;
		CObject *scn_middlePart, *scn_btnPlay, *scn_btnOptions, *scn_btnExit, *scn_btnProfile;
		CObject *scn_fgStoneLeft, *scn_fgStoneRight;
		CObject *scn_extrasStone, *scn_btnExtras, *scn_btnCredits;
		CObject *scn_groundStone, *scn_crystalGlow, *scn_gameTitle;
		#ifdef K_ANDROID
		CObject* scn_hintStore;
		#endif
		CObject* scn_btnUnlocknow;
		CPoint mouseDiff;
		CPoint mouseTarget;

		CObject *langBG, *langSelectBtn, *langSelectBtnTxt, *langRestartBG, *langRestartOKBtn, *langRestartOKBtnTxt;
		CObject *enBtn, *brBtn, *deBtn, *frBtn, *itBtn, *ndBtn, *ruBtn, *esBtn, *enBtnTxt, *brBtnTxt, *deBtnTxt, *frBtnTxt, *itBtnTxt, *ndBtnTxt, *ruBtnTxt, *esBtnTxt, *langBackBtn, *langBackBtnTxt;
        CBitmapText *langRestartText;

		void ShowLanguageSelection();
		void HideLanguageSelection();
		void UpdateLanguageSelection();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void   Render(const CPoint* pCam); 

		TextBox  *TextEntry;
		std::string NewProfileName;
		bool IsTextEnterKeyPressed;
		bool IsForcedNameEntry;

		CPoint MusicDragOrigin, SoundDragOrigin;
		CPoint EnvirDragOrigin, VoiceDragOrigin;
		bool IsMusicKnobDragStarted, IsSoundKnobDragStarted;
		bool IsEnvirKnobDragStarted, IsVoiceKnobDragStarted;
		bool PlayBtnClicked;
		MainMenuControl();
		~MainMenuControl();

		int PrevCursorMode;
		//Inherited methods
		void ClearMenuTrack();
		void Update();
		void Show(bool f);
		void HighlightActiveProfile( int Slot , bool f);
		void SetProfileSlotTextColors( int i );
		
		bool IsWelcomePlayerFadeIn;
		float WelcomeTextAlpha;
	protected:
		void ProcessMessage(sMessage message);

		MMPopup_State	 PopupState;
		bool IsWaitForShowEnterYourName;
		float		 PopupAlpha,  ProfilePopupAlpha;
		int			 PopupCounter,ProfilePopupCounter;
		int	   CheckPopupButton(CObject *Button);

		int	   CheckPopupButton(CObject* Button, bool scale);
		
		/*
		CBitmapText   *WatermarkText;
		void CreateWatermark();
		*/

		CHOScene	*SelectAdventurePopupScene;
		void UpdateSelectAdventurePopup();
		void ShowSelectAdventureGamePopup();
		CPolygon *PolyMainChapter, *PolyBonusChapter;
		CObject *mainBtnGlow, *bonusBtnGlow, *bonusBtnLock, *comicBookUnlockBtn;
		bool isComicOpenedFromSelectAdvPopup;

		bool isChapterThumbsAnimating;
		float chapThumbAnimTimer;

		bool IsMainChapterSelected;

		CHOScene	*SelectModePopupScene;
		void UpdateSelectModePopup();
		void ShowSelectModePopup();

#ifdef K_ANDROID
		void AndroidSetMode();
#endif
		
		SuperFX *VFXCrystalFg, *VFXCrystalBg, *VFXButton, *VFXSnow;

		CObject *pappus[NUM_FLYING_PAPS];
		float papVelY[NUM_FLYING_PAPS], papVelX[NUM_FLYING_PAPS], papAccelY[NUM_FLYING_PAPS];

		int SelectedMode;
		CRectangle	*ModeRects[MAX_GAMEMODES];
		CObject		*ModeButtons[MAX_GAMEMODES];

		bool IsModeSelectionFromOptions;

		//Custom Mode Stuffs
		CObject		*CustMode_ItemsBg;
		CObject		*CustMode_HintKnob, *CustMode_SkipKnob;
		CObject		*CustMode_HintTime, *CustMode_SkipTime;
		CObject		*CustMode_TickTutor, *CustMode_TickSparkles, *CustMode_TickHOSparkles;
		CObject		*CustMode_TickTextTips, *CustMode_TickHints, *CustMode_TickSkip, *CustMode_TickAwardPopup;

		int		HintTimeInt,SkipTimeInt;//anim
		bool	IsHintEnabled,IsSkipEnabled;
		bool	IsTutorEnabled,IsSparklesEnabled,IsHOSparklesEnabled,IsTextTipsEnabled,IsAwardNotifyEnabled;

		bool	IsHintKnobDragStarted,IsSkipKnobDragStarted;

		CBitmapText  *ModeDescriptionTexts[MODE_TEXT_LINES], *modeNameText;		
		void ModeTexts_Init(std::string bigstring, int OffsetY = 0,eTextAlignment AlignStyle = Align_Center, int OffsetX = 0);
		void ModeTexts_Show( bool f );
		void ModeTexts_SetAlpha( float f );
		float ModeTextsLineWidth;
		void playPendingCollectableLevel();

		CHOScene	*CreditsPopupScene;
		CObject *CreditsBase;
		void UpdateCreditsPopup();
		void ShowCreditsPopup();

		CHOScene	*OptionsPopupScene;
		CObject *modeBtnOver;
		void UpdateOptionsPopup();
		void ShowOptionsPopup();

		CBitmapText* musicVolTxt, * effectsVolTxt, * voiceVolTxt, * diffModeTxt, * customCurTxt, * fullscreenTxt, *languageTXT;

		CHOScene	*EnterYourNamePopupScene;
		void UpdateEnterYourNamePopup();
		void ShowEnterYourNamePopup(int Slot);
        void HandleProfileCorruption();

		CHOScene	*ProfilesPopupScene;
		void UpdateProfilesPopup();
		void ShowProfilesPopup();

		CObject		*ProfNameGlow[MAX_PROFILES];
		CObject		*ProfDeleteButtons[MAX_PROFILES];
		CObject		*ProfNewPlayerTexts[MAX_PROFILES];
		CRectangle	*ProfNameRects[MAX_PROFILES];
		CRectangle	*ProfDeleteButtonRects[MAX_PROFILES];

		CHOScene	*EraseProfilePopupScene;
		void UpdateEraseProfilePopup();
		void ShowEraseProfilePopup(int Slot);
		void InitProfilesPage();

		CHOScene	*ExitGamePopupScene;
		void UpdateExitGamePopup();
		void ShowExitGamePopup();

		CHOScene	*ReviewGamePopupScene;
		void UpdateReviewGamePopup();
		void ShowReviewGamePopup();
		void CheckSocialButtons();
		CObject *LoveItBtn;
		CObject *CouldbeBetterBtn;	
		CObject *LeaveAReviewBtn;	
		CObject *NoThanksBtn;	
		CObject *SendFeedbackBtn;	
		CObject *CommentsBtn;	
		CObject *FacebookBtn;	
		CObject *IGBtn;	
		CObject *TwitterBtn; 
		CObject *LinkedinBtn;
		CObject *PinterestBtn;
		CObject *YTBtn;
		CObject *CLOSEBtn;

        bool IsOverUpsellPage, IsOverAwardsPage;
        CObject *UpsellPage, *AwardsPage;

		CHOScene	*MenuScene;
		CObject *MaskSpr, *MaskSpr2, *ModeMaskSpr;
        bool IsFromProfilesPopup;
		bool	IsProfileNamesEmpty;
        int AwaitingEnterNamePopupSlot;
		CObject		*Buttons[MMB_Max];
		CObject		*ProfileButton;
		CRectangle   ButtonRects[MMB_Max];
		CPoint		BtnVFXPts[MMB_Max];
		CPoint		BtnVFXProfilePos;

		//New UI Buttons
		CObject* SHARE_BTN, * LIKE_BTN, *UNLOCK_BTN, *SIGN_UP_BTN;
		
		bool IsMouseOverWelcomeText;
		CBitmapText	*WelcomePlayerNameText;
		CPoint profileTxtInitPos, profileTxtPos;
		//CBitmapText	*CopyrightText;
		CBitmapText	*ProfileText[MAX_PROFILES];
		CBitmapText	*EraseProfileNameText;

		int iActiveProfileSlot,iInactiveProfileSlot;
		bool IsSlotCreated[MAX_PROFILES], IsSlotDeleted[MAX_PROFILES];

		

		float WelcomeTextAlpha2, MenuAlpha;

		bool IsButtonsFadingIn;
};

extern MainMenuControl* GFMainMenuCtrl;
#endif
