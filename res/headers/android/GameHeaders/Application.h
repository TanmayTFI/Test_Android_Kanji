//====================================
// Framework
// Application class
//====================================

#ifndef FRMWRK_APPLICATION_DEF
#define FRMWRK_APPLICATION_DEF

#include "iPadDefs.h"
#include "VideoDefs.h"
#include "LangDefs.h"
#include "KPTK.h"
#include "KGame.h"
#ifdef K_ANDROID
	#include "CAndroidUIEditField.cpp"
#endif
#include "BuildConstants.h"

#include "MultilineTextBox.h"

#ifdef K_iOS
        #define _PAKBUILD true
        #define _RELEASEBUILD true
#else
        #ifdef _DEBUG	
			#define FAST_NOSNDCACHE_BUILD true        

            #ifdef K_WIN32
				#pragma comment(lib,"kanji_vs2012_unicode_d.lib") 
            	#pragma comment(lib,"kanjisounddx_vs2012_unicode_d.lib") 
            	#pragma comment(lib,"kanjivideo_vs2012_unicode_d.lib") 	
			#endif
        #else
			#define _PAKBUILD true
            #define _RELEASEBUILD true

            #ifdef K_WIN32	
				#pragma comment(lib,"kanji_vs2012_unicode.lib")
            	#pragma comment(lib,"kanjisounddx_vs2012_unicode.lib")
            	#pragma comment(lib,"kanjivideo_vs2012_unicode.lib")
			#endif

	#endif
#endif

#define CURSOR_WIDTH  43
#define CURSOR_HEIGHT 55 

#include "Control.h"
#include "Resolution.h"	
#include "Point.h"

class CBitmapText;

class Application : public Control
{
	public:	
    
#ifdef K_iOS
        /** iPhone input box with virtual keyboard */
        KSysiPhoneTextField *lpTextField;
        bool IsTextFieldEntered;
#endif
    
#ifdef K_MACOS
    /** Keys for trapping COMMAND keys, mainly needed for COMMAND + Q */
    bool isCommandLeftPressed;
    bool isCommandRightPressed;
    bool isCommandLeftDown;
    bool isCommandRightDown;
    
#endif
    
#ifdef K_ANDROID

		/** Android input box with virtual keyboard */
        bool IsTextFieldEntered;
        KUIElement *lpUIContainer;
        CAndroidUIEditField *lpUIEditField;
		std::string pathToExternalDirectory;
		int MyVersion;
		int gameVersionCode,gameOBBVersionCode;
		std::string deviceName, gameVersionName;
		CBitmapText   *gameVersionText;
        void CreateGameVersionText();
		void SetAndroidAchievement(int award_id);
		bool isUnlocked();  // simply returns true if it is Free
		void UnlockGame(); //Unlocks the game from Free to Full
		void AnalyticsSkipPuzzle(int puzzleID);
		void UpdateUserHint(std::string data);
		void ShowInterstitialAD();
		void ShowRewardedAD();
		void ShowHintStore();
		void ShowUnlockStore();
		void FetchUserData();
		void SignIN();
		void SignOUT();
		std::string userInfo;
		bool unlocked;
		bool forceSaveProfile;
		int noOfHints;




#endif
		int DesktopWidth, DesktopHeight, WindowDepth;
		bool IsWndFocus;		
		bool IsTaskBarMinimize;
        bool IsMacDebuggingOn;
		bool IsTurnOffParticles;
		 /** Buffer for composing error messages */
		static char g_szErrMessage[1024];

		/** Buffer for sending crash reports by e-mail */
		static char g_szEnquotedErrMessage[1024];
		static void enquoteMessage (const char *lpszMessage, char *lpEnquoteBuf, size_t nEnquoteBufSize);
		static void crashHandler (bool bIsAssertion,
                          unsigned long nExceptionCode, const char *lpszExceptionName, bool bHasParam, void *lpParam,
                          const char *lpszAssertionFileName, long nAssertionLine,
                          long nStackDepth, void *lpStackReturnAddr[], char *lpszStackModule[]);
		const char *getBuiltinString (const char *lpszStringId);

	#ifdef _ENABLE_CURL
		void SendReportToDev();
		std::string fromUserEmail;
	#endif
		CBitmapText   *FPSText;
		void CreateFPS();
		bool		IsDebugMode;
		bool		IsSurveyBuild;
		bool		IsSkipSplash;
		int 		langID; //Sac: Language id for loading the right langauge game texts json
		int			DebugResMode;
		bool		AppActive;

		float		video_aspectRatio;
		float		video_startX;
		float		video_startY;
		float		video_width;
		float		video_height;
		float		video_negativeHalfWidth;
		float		video_negativeHalfHeight;

		//Class wide member
		static Application* Instance;

		KWindow*	Window;
		Erenderer	Renderer;
		
		Application();
		~Application();

		KTrueText *lpFont20, *lpFont21, *lpFont22, *lpFontSmall, *lpInvPopFont, *lpFontDiary, *lpFontDropShadow;   	

		//Extra fonts for locatization
		KTrueText *font10;

		//Inherited Methods
		void Process();

		//Class wide method
		static bool ToggleFullscreen();
		bool CanWindow(int width, int height);
		static void FadeToBlack(float FadeSpeed = 10.0f);
		static void FadeToNormal(float FadeSpeed = 10.0f);
		static void InstantFadeNormal();
		static void SetInstanceAlpha(float Alpha);
		static void UpdateAndWait( float time );
		static void ClearBackBuffer();
		static void FlipBackBuffer();
		static void StaticPostUpdate();

		//Resolution controls
		//static void PrepareForFail();
		

		static void SetSingleUpdate(bool f) { SingleUpdate = f; }
		static bool SingleUpdate;
		//Res control
		float ProjX, ProjY, ProjZoom, touchTime, touchVelocity, flingX, flingY, touchPosX, touchPosY, tempProjZoom, flingTimer, touchPosX2, touchPosY2, touchZoomFactor;
		int zoomQuad;
		float projXMax, projYMax;
		float zoomOriginDeltaX, zoomOriginDeltaY, startTouchX, startTouchY;
		bool ripple, canPan, canZoom, pinchDisabled, touched, _tapped;
		void TouchRipple();
		std::string debugMsg;
		CPoint MessageTextPos;
		void ForcePostUpdate();
		void ForceRenderScntoBB();
		void ForceRenderScnHudtoBB();
		int  nMonitors;
		int  SynchLogicRate;

		void PinchToZoom();

		bool IsBestResolution;
        static SResolution CurrentResolution;

		void DrawProjEdgeCorrection();
		void LaunchSurvey();
		void HandleForcedWndCloseDeadlocks();

		bool RefreshInvItems;
		void PauseForPurchase(bool show);

		MultilineTextBox *testBox, *testBox2;
		
#ifdef TOUCH_UI
		CObject *touchRipple[5];
		int rippleIndex;
#endif
	void SetAssetLanguage(int id);

	int GetAssetLanguage();
		
	private:
		//Inherited Methods
		void ProcessMessage(sMessage message);
		
		void SetCutSceneVideoResParameters(int video_res); // sets the Cut Sceen Video Resolition parameters to render in
		
		//Do nothing here
		//This call is made by the base class process
		//but we've already extracted out the necessary
		//actions into ApplicationUpdate()
		void Update(){};
		
		//Application Methods
		void ApplicationUpdate();
		void LogicProcess(bool processControl);
		void PostUpdate(bool fade=false);

		//Used for Fade Properties
		KGraphic* Quad;
		float	  FadeAlpha;

		KGraphic* pauseBGMask;		
		void DrawThanks(bool show);
	
		int WhichDisplayMode(int DesktopWidth,int DesktopHeight );
		int WhichDisplayMode();
		void SetDisplayWidthnHeight(int Mode); 
		bool CreateBestWindowedDisplayMode( );
		bool CreateBestFullScreenDisplayMode( );
		bool TryToCreateGameWindow( int Width, int Height, int Mode);
		void InitRenderer( int DisplayAdapterMode, int BPP );

		bool CreateGameWindow();

};

extern Application* GFApp;

#endif
