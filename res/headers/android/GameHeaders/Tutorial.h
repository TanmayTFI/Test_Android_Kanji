//====================================
//DQFULP
// Tutorial
//====================================

#ifndef DQFULP_TUTORIAL_SYS_DEF
#define DQFULP_TUTORIAL_SYS_DEF

#include "HOScene.h"
#include "MultilineText.h"

//Active Tutorial State
enum TutorialState
{
	TUT_Inactive=-1,	//Tutorial Disabled
	TUT00_Prompt,		//Asking for tutorial
	TUT01_TalkWithRaja,
	TUT02_TakeJames,
	TUT03_Journal,
	TUT04_ObjectivesTab,
	TUT05_BagInvTake,
	TUT06_BagZoomLook,
	TUT07_ZipperOpen,
	TUT08_KnifeTakeFromBag,
	TUT09_RosePopupZoom,
	TUT10_UseKnifeOnRose,
	TUT11_MorphObject,
	TUT12_NavigationUp,
	TUT13_HintTutorial,
	TUT14_MapTutorial,
	TUT15_MapDetailsTutorial,
	TUT16_SkipPuzzle,
	TUT17_MemoryCollectible,
	TUT18_ComboInvZoomLook,
	TUT19_ComboInvUseCombine,
	TUT20_HOSparklesOnScene,
	TUT21_Match3Tutorial,
	TUT22_Match3InsideTutorial,
	TUT_MAX
};

extern std::string	TutorialTexts[TUT_MAX];

class CBitmapText;

enum TutTransState{
	TUT_FADEIN,
	TUT_IDLE,
	TUT_FADEOUT,
	TUT_HIDDEN
};

class Tutorial
{
	public:
		int PrevCursorMode;

		MultilineText *MultiTextTutorial;
		float textOffsetY;

		//Tutorial transition
		TutTransState TutTransitionState;
		float TutTransAlpha;
		float TutTransGapTimer;

		bool IsTutFadingOut;
		bool IsNextTutQueued;

		float BtnScaleAngle;

		bool IsActive;
		bool IsVisible;

		//State Control	
		TutorialState State;

		CObject *FrameBg, *BtnSkip, *BtnContinue, *IconBg, *CursorIcon;
		CObject *Arrow1, *Arrow2, *CircleAnim1, *CircleAnim2;

		CObject	*TutorSkipMask;
		
		CHOScene* TutorialScene;
		CHOScene* PromptWindowScn;

		CObject* mask;

		CObject	*BtnFullTut, *BtnSpecialFeatures, *BtnNoTutorial;
		CObject* BtnFullTutTXT, * BtnSpecialFeatureTXT, * BtnNoTutorialTXT;
		CObject* btnTXTYes,* btnTXTNo, *tutHeading;
		CObject* tutPromptMsg;

		bool isPromptWindowShowing;
		bool isSkipPromptWindowActive;

		//in case some other controls clearing the mousemode
		bool isMouseOverTutorUI;

		Tutorial();
		~Tutorial();

		void Deactivate();

		void ShowTutorial(TutorialState TutId);
		void HideTutorial(bool isQuick=false);

		void ShowSkipTutorialWindow();

		void SetTutorialAlpha(float alpha);

		//Inherited method
		void Update();
		void Required();

	protected:
		//protected members
};

#endif