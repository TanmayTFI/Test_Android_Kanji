//====================================
//DQFULP
// Game Flow Control
//====================================

#ifndef DQFULP_GAMEFLOW_DEF
#define DQFULP_GAMEFLOW_DEF


#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)  
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)  
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)  
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ##__VA_ARGS__)

#define C_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType get##funName(void) const { return varName; }\
public: inline void set##funName(varType var){ varName = var; }

#define C_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: inline varType get##funName(void) const { return varName; }\


#include "Control.h"
#include "DataEnums.h"
#include "HOObject.h"
#include "LangDefs.h"
#include "SuperFX.h"

#include "VideoDefs.h"
#include "KVideo.h"

#include "ProfileSystem.h"




enum _PopupState{ POPUP_SCALEUP, POPUP_OPEN, POPUP_SCALEDOWN, POPUP_CLOSED }; 

enum eGameFlow
{
	fSplash,
	fHaveYouCutscene,
	fRootMenu,
	fStartCutscene,	
	fHOLevel,
	fPromoScene
};

class CHOScene;
class CBackground;
class ParticleEffect;
class CBitmapText;

enum _BufferScenes{ 
	TUTORIAL_SCN, //Unload after use!!
	MAP_SCN, MAP_SCNL2, MAP_SCNL3, MAP_SCNL4, MAP_SCNL5, MAP_SCNL6, MAP_SCNL7, MAP_SCNL8,
	JRN_SCN,
	GUIDE_SCN,
	HO_HUD_SCN,
	INGAMEMENU_PSCN,
	OPTIONS_PSCN,
	MODESELECT_SCN,
	RESTART_DIA,
	RESET_DIA,
	MAX_BUFFER_SCN };

enum _BufferObjects{	INFO_POPUPBASE_OBJ,INFO_WIN,INFO_WINLRG, INFO_OK, INFO_SKIP, INFO_CANCEL,
						FOUNDINV_BASE_OBJ, FOUNDINV_MASK_OBJ, POP_CLOSE_OBJRECT,POP_CLOSE_OBJCIRC,
						INVBOX_BASE_OBJ,INVBOX_LARR_OBJ,INVBOX_RARR_OBJ,INVBOX_LOCK_OBJ,INVBOX_TOOLTXT_OBJ,
						HUD_OBJBG_OBJ, HINT_RECHARGE_FRAME_BG, HINT_NAVIG_CURSOR_OBJ,
						MASK_OBJ, MODEMASK_OBJ, JRNBUTTON_OBJ, MENUBUTTON_OBJ,MAX_BUFFER_OBJ };

enum _BufferSuperFX{ HINTCHARGED_SUPERFX, HINTTRAIL_SUPERFX, HINTTRAILSOFT_SUPERFX,  HINTCIRCLE_SUPERFX, HINTEXPLODE_SUPERFX, HINTGREENCIRCLE_SUPERFX, DIARYFX_SUPERFX, MAX_BUFFER_SUPERFX };

#define MAX_WRONGDROP_MSG  8 

#define MAX_LD_FRAMES 15

#define SAFE_DELETE(PTR) if( PTR != NULL ) { delete PTR; PTR = NULL; } 

class HudControl;
class ParticleEffect;
class InfoPanel;
class GameFlow : public Control
{
	public:	

		CObject *RT_BgScn;
        std::string GamePath;
		bool IsHOPuzzleScene( int eArea );
#ifdef JAPANESE_BUILD
		const char* JapInventoryDescrips( int DescId );
#endif

		int SaveMaskObjZ;
		bool GFInfoLive;
		bool IsDiffModeSelectInfoPopup;
		bool IsWideScreenChangeInfoPopup;
		bool IsUpgradeInfoPopup;
		bool Is1360x768GFInfoPopup;
		bool Is1366x768GFInfoPopup;
		bool Is1280x768GFInfoPopup;
		bool Is1280x720GFInfoPopup;
        bool Is1280x800GFInfoPopup; //Support for mac
        bool Is1152x720GFInfoPopup; //Support for mac
        bool Is1024x640GFInfoPopup; //Support for mac
        bool Is1920x1080GFInfoPopup; //FullHd
		bool Is1920x1200GFInfoPopup; //FullHd+
        bool Is2560x1440GFInfoPopup; //QHd
        bool Is3840x2160GFInfoPopup; //4k
        bool Is7680x4320GFInfoPopup; //8k
        bool Is1600x900GFInfoPopup;
        bool Is1344x756GFInfoPopup; 
    
    
		bool Is1024x768GFInfoPopup;
		bool Is800x600GFInfoPopup;
		InfoPanel* GFInfoControl;
		void InitGFInfoControl();		
		void ActivateGFInfo(std::string b1, std::string b2);
		void ProcessMessage(sMessage message);

		void ReloadSceneGraphic();
		bool IsSceneGraphicReloaded;

		bool IsSplashFullAlpha;
		bool IsPakLoading;
		
		int availablePhyMem;
		bool IsLowFPSMachine;
		bool IsSlowSystem;
		bool IsFullCacheMem, IsPartialCacheMem, IsForceTextures16Bit, IsInsideLevel;
		void CheckAvailablePhyMemory();

		
		unsigned long LastSaveTime;
		bool IsPakResLoadTerminate, IsPakResLoadInited, IsPakResLoadDone;
		int iPakRes;		
		void initPakResLoad();
		void LoadPakRes();
		void WaitTillPakResLoadDone();

		bool	IsErrSoundDeviceInit;
		bool	IsStartCutsceneLoaded;
		
		KVideo *lpStartCutsceneVideo;
        KVideo *lpEndCutsceneVideo;    
		KSound *lpStartCutsceneSndStream;
		
		void LoadStartCutscene();
		void UnloadStartCutscene();

		bool	IsEndCutsceneLoaded;
		
		void LoadEndCutscene();
		void UnloadEndCutscene();

		CObject * CreateSceneScreenshot(CHOScene * scene, bool *_success);
		ParticleEffect *strikeGlowFX;		
		
		bool IsReplayingGame;
		
		CObject *PausedImage;
		CBitmapText *ThanksText[2];
		void CreateThanks();		

		CObject	*GreyAlphaImage;
		bool	IsCutsceneVideoPlayerActive;
		int		activeCutsceneID;

		int PStartX, PStartY, PEndX, PEndY; 
		void SetPopupTween( int StartX, int StartY, int EndX, int EndY );
		int PrevRenderZorder;
		bool IsScnBg1024Res;
		bool IsRTCreated, IsScnRTCreated;
		bool IsRenderingToRT;
		KGraphic *RTGraphic;		
		
		bool IsPrevSingleUpdate;
        bool IsFirstTimeGFLoadingComplete; //Sac: added this because if there is a game window focus lost while starting, the game will crash
		KGraphic *PrevSceneGraphic;
		KGraphic *NewSceneGraphic;
		KGraphic *CurrentSceneGraphic;


		CObject* youFound;
		
		void CreateGreyscaleScene();
		void UpdateBlurScene( float Blur );
		void TurnOffBlurScene();		
		void WrongInvDropMsg( HudControl * MyHud );
	
		int iWrongMsg;
		void CursorInvBoxOffset();
		
		bool IsLvlCommonTracksLoaded;//Not loaded during splash as interfering with cinematic lip-synch 
		void LvlAssetsLoader( );
		void PreloadLoadLevelArea( int LevelArea, int nUpdates );
		void ClearPrevLevelInventories();
		GameFlow();
		~GameFlow();

		SuperFX *BufferSuperFX[MAX_BUFFER_SUPERFX];
		bool IsBufferSuperFXLoaded[MAX_BUFFER_SUPERFX];
		SuperFX *LoadBufferSuperFX( int FxId ); 
		void UnloadBufferSuperFX( int FxId );

		CHOScene *BufferScenes[MAX_BUFFER_SCN];
		bool IsBufferSceneLoaded[MAX_BUFFER_SCN];
		CHOScene *LoadBufferScene( int ScnId ); 
		void UnloadBufferScene( int ScnId );

		CObject *BufferObjects[MAX_BUFFER_OBJ];
		bool IsBufferObjectLoaded[MAX_BUFFER_OBJ];
		CObject *LoadBufferObject( int ObjId ); 
		void UnloadBufferObject( int ObjId );

		CHOObject *BufferLvlInvHOObj[eInv_Max];
		int  iBufferLvl_InvHOObjLoaded;
		void LoadBufferLvlInvHOObj( int iLevel ); 		
		void UnloadBufferLvlInvHOObj();

		
		//Inherited method
		void Update(){};		
		int nPostUpdates;

		bool IsScnElmntsPreloaded[eArea_Max];
		bool IsWatchingPromo; 

		void PostUpdate(int Updates);
		bool IsCacheDataLoaded;
		bool IsActivePopup;
		bool IsInGameMenuPopup;
		void UnloadLevel1Assets();
		void UnloadLevel2Assets();
		void UnloadLevel3Assets();
		void UnloadLevel4Assets();
		void UnloadLevel5Assets();		
		void UnloadLevel6Assets();
		void UnloadLevel7Assets();
		void UnloadLevel8Assets();
		void UnloadAllAssets();

		bool IsCrossFadeTransSet;
		void InitCrossFade();
		void RunCrossFade();
		void InitCurrentSceneGraphic();

        KGraphic *lpLoadingSpr[MAX_LD_FRAMES];
        KGraphic *lpLoadingScnBG;
		void InitScnLoadingSpr();

		CObject * UpdateBgGraphic(CHOScene* scene);


		eGameFlow Flow, PrevFlow;
		eAudio AS_TrackPlaying_in_iPad;
	private:
		
		CObject *LoaderGlowBar, *LoaderBase, *CETitle;
		CHOScene *LoadingScene;
				
		int LoadingFile;
	
		//Inherited method
		bool RemoveControl(int id, int value);
		bool IsGamePuzzle( int eArea );
		bool IsGameRoom( int eArea );

		void AddNextArea();
		void ReturnToMenu();
	#ifdef K_ANDROID
		long _nActualWidth, _nWindowWidth;
   		long _nActualHeight, _nWindowHeight;
		bool overriddenSaveBackBuffer(const char *lpszFilename, long nImageFormat = K_IMAGE_BMP, long nWidth = 0, long nHeight = 0, long nCropX = 0, long nCropY = 0, long nCropWidth = 0, long nCropHeight = 0);			
		unsigned int * overriddenGetBackBuffer(long *gnBWidth, long *gnBHeight, long nWidth = 0, long nHeight = 0, long nCropX = 0, long nCropY = 0, long nCropWidth = 0, long nCropHeight = 0);
	#endif

		
};

extern GameFlow* GFInstance;




#endif
