//====================================
//DQFULP
// Extras Menu
// Hidden Object System
//====================================

#ifndef DQFULP_EXTRAS_MENU_DEF
#define DQFULP_EXTRAS_MENU_DEF

#include "Control.h"
#include "BitmapText.h"
#include <string>

#include "CutsceneEnums.h"
#include "ExtrasCutscenePlayer.h"

enum eExtrasPopState
{
	Popup_Disabled=-1,
	Popup_Extras,
	Popup_Extras_ConceptArt,
	Popup_Extras_CutScenes,
	Popup_Extras_HOScenes,
	Popup_Extras_Wallpapers,
	Popup_Extras_Music,
	Popup_Extras_MiniGames
};

enum eExtrasPages
{
	ExtraPage_Concept_Art,
	ExtraPage_CutScenes,
	ExtraPage_HOScenes,
	ExtraPage_Wallpapaers,
	ExtraPage_Music,
	ExtraPage_MiniGames
};

enum ePopupMouseActions
{
	ePopup_NoInteraction,
	ePopup_MouseOver,
	ePopup_Click
};

enum eButtonStates
{
	eBtn_Idle,
	eBtn_Hover,
	eBtn_Click,
	eBtn_Locked
};

enum eZoomState
{
	eZoomInactive,
	eZoomingIn,
	eZoomIdle,
	eZoomingOut
};

enum eWallPaperSaveSize
{
	eSaveInvalid,
	eSave1920,
	eSave1366,
	eSave1024
};

class CObject;
class CHOScene;

#define MAX_MUSIC_TRACKS 12

class ExtrasMenuControl : public Control
{
	public:		
		ExtrasMenuControl(CObject* maskSpr);
		~ExtrasMenuControl();

		void  Show(bool noTransition=false);
		void  Hide(bool noTransition=false);
		
		float PopupAlpha;

		bool isComicBookClicked;

		int PrevCursorMode;

		//Music Menu
		bool isExtrasMusicPlaying;
		int currentMusicPlayingID, selectedMusicID;
		CObject *MusicTrackGlows[MAX_MUSIC_TRACKS];
		CRectangle *MusicTrackRects[MAX_MUSIC_TRACKS];

		int			LastSelectedExtrasBtn;
		int			ExtrasPageSize;
		bool		IsExtrasItemSelected;

		int	   CheckPopupButton(CObject *Button);

		CHOScene	*ExtrasPopupScene;
		CHOScene	*SCN_CutScenes, *SCN_Concepts, *SCN_HOScenes, *SCN_Wallpapaers, *SCN_Music, *SCN_MiniGames;

		CBitmapText *achievementsTxt, *minigamesTxt, *memoriesTxt, *conceptsTxt, *cutscenesTxt, *musicTxt, *hopuzzlesTxt, *wallpapersTxt, *creditsTxt;
		CObject* comicbookTxt;

		CObject* creditsBtn, *comicBtn;

		int PopupCounter;
		void HandleItemClicks(int index );
		void UpdateExtrasPopup();
		void ShowExtrasPopup();
		CBitmapText *CurrentPageText;
		//__ExtraPopupItems
		void PopulateExtraPopupItems(eExtrasPages item);
		void UpdateExtrasItemPagePopup(eExtrasPages currentPage);
		bool isBackButtonSelected;

		//Zoom mode ICONS
		CObject *zoomImageSpr, *zoomDarkBG;
		eZoomState zoomState;
		CPoint zoomOrigin;

		void onZoomImageTweenCompleted();

		void SetupZoomImageAndShow(int itemID, int posIndex);

		void LoadHiddenObjectScene(int ItemId);
		void LoadPuzzleScene(int ItemId);

		eExtrasPopState	 PopupState;

		int  pageIndex;
		eExtrasPages	SelectedExtraPageScene;

		//Inherited method
		void Update();
		void Required();

		

		void SetMemoriesRoomCallback(std::function<void ()> kidroomAreaCallback) {
			_kidroomAreaCallback = kidroomAreaCallback;
		}

		void SetAwardroomCallback(std::function<void ()> awardsAreaCallback) {
			_AwardsAreaCallback = awardsAreaCallback;
		}

		void SetCutsceneVideoCallback(std::function<void ()> cutsceneVideoAreaCallback) {
			_CutsceneVideoCallback = cutsceneVideoAreaCallback;
		}

		void SetLoadComicCallback(std::function<void ()> loadComicCallback) {
			_LoadComicCallback = loadComicCallback;
		}

		void SetHOPuzzleSceneLoadCallback(std::function<void (int, int)> hoPuzzleLoadCallback) {
			_HOPuzzleLoadCallback = hoPuzzleLoadCallback;
		}

		

		void SetShowBannerCallback(std::function<void (std::string, int)> showMessageBannerCallback) {
			_showMessageBannerCallback = showMessageBannerCallback;
		}

		void SetCreditsCallback(std::function<void()> creditsCallback) {
			_CreditsCallback = creditsCallback;
		}

	protected:
		std::function<void ()> _kidroomAreaCallback;
		std::function<void ()> _AwardsAreaCallback;
		std::function<void ()> _CutsceneVideoCallback;
		std::function<void ()> _LoadComicCallback;
		std::function<void ()> _CreditsCallback;
			
		std::function<void (std::string, int)> _showMessageBannerCallback;

		std::function<void (int, int)> _HOPuzzleLoadCallback;

		CObject* _maskSpr;

		CRectangle *ExtraItemsRect[8];
		CRectangle *PageItemRect[6];
		CRectangle *WallPapaerRect1920[3];
		CRectangle *WallPapaerRect1366[3];
		CRectangle *WallPapaerRect1024[3];
		CObject	*ExtrasOver[8];

		bool UnlockStatus[6];
		bool CheckUnlockStatus(int ItemId);
		void UnlockExtrasforFullVersion();
		void UpdateUnlockStatus();
		void OnSaveWallpaperEvent(int itemId, int itemSize);
		void SaveMusicTrack(int itemId);
};

#endif
