//====================================
//DQFULP
// Game Flow Control
//====================================

#include "Application.h"
#include "GameFlow.h"
#include "Splash.h"
#include "StartCutscene.h"
#include "EndCutscene.h"
#include "MenuSystem.h"
#include "HiddenObjectSystem.h"
#include "HiddenObjectEnums.h"
#include "HOScene.h"
#include "TextureFactory.h"
#include "Background.h"
#include "Util.h"
#include "ParticleEffect.h"
#include "BitmapText.h"
#include "Fonts.h"
#include "Cheats.h"
#include "exMath.h"
#include "Hud.h"
#include "Cursor.h"
#include "GameTextManager.h"
#include "InfoPanel.h"
#include "LangDefs.h"
#include "Constants.h"

#ifdef K_ANDROID
	#include "AndroidIo.h"
#endif


#ifdef _DEBUG
#include "SuperLog.h"
#endif

#ifdef _RELEASEBUILD
#define FULL_GAMECACHE_MEMORY	 950 
#define PARTIAL_GAMECACHE_MEMORY 505 
#else //Debug
#define FULL_GAMECACHE_MEMORY	 200//1024 //Force machine to use minimal memory
#define PARTIAL_GAMECACHE_MEMORY 200//1024 //Force machine to use minimal memory
#endif

#define MAX_16BIT_LOWEND_PEAKMEMORY 300 
//for final game reset to 260


#ifdef _CEBUILD
	#ifdef K_WIN32	
		std::wstring AppTitle_Str = L" Collector's Edition";
	#else
		std::string AppTitle_Str = " Collector's Edition";
	#endif
#else
	#ifdef K_WIN32	
		std::wstring AppTitle_Str =L"";
	#else
		std::string AppTitle_Str = "";
	#endif
#endif

	
#ifdef K_WIN32	
	extern std::wstring WRNG_WStr;
	extern std::wstring FATAL_WStr;
	extern std::wstring LWMMRY_WStr;
	extern std::wstring GWRNG_0_WStr;		
#else
	extern std::string WRNG_WStr;
	extern std::string FATAL_WStr;
	extern std::string LWMMRY_WStr;
	extern std::string GWRNG_0_WStr;		
#endif


#include "KidsRoom.h"
#include "ExtrasCutscenePlayer.h"
#include "AchieveRoom.h"
#include "AwardsRoom.h"
#include "Credits.h"

#include "PromoScene.h"
#include "HaveYouCutScene.h"

//level 1 includes
#ifdef _LEVEL1_ENABLED
#include "L1_S1_Beach.h"
#include "L1_S1_PZKidsPainting.h"
#include "L1_S2_Frontyard.h"
#include "L1_S2_PZFuse.h"
#include "L1_S3_Foyer.h"
#include "L1_S3_PZShelf.h"
#include "L1_S4_PZDoor.h"
#include "L1_S4_Bedroom.h"
#include "L1_S4_PZPainting.h"
#include "L1_S3_DoorHO.h"
#include "L1_S5_TreasureRoom.h"
#include "L1_S5_TunnelDoor.h"
#include "L1_S5_PZDynamiteMaking.h"
#include "L1_S5_HOTreasureRoom.h"
#include "L1_S6_BoatShed.h"
#include "L1_S6_HOBoatShed.h"
#include "L1_S7_Boatdock.h"

#include "EndOfDemoScene.h" 
#endif // _LEVEL1_ENABLED

#ifndef _DEMOBUILD
//Only Includes Level 2 and Above, If this is not a Trail Build (ie Full Version Build)

//level 2 includes
#ifdef _LEVEL2_ENABLED
#include "L2_S1_ForestEntrance.h"
#include "L2_S1_UndergroundHO.h"
#include "L2_S2_NagaTemple.h"
#include "L2_S2_NagaTempleHO.h"
#include "L2_S2_PZMango.h"
#include "L2_S3_CaveMouth.h"
#include "L2_S3_PZStoneBeam.h"
#include "L2_S4_Cave.h"
#include "L2_S4_DeitiesPuzzle.h"
#include "L2_S5_MoopansDen.h"
#include "L2_S5_EagleBlockPuzzle.h"
#include "L2_S6_PZTunnelClock.h"
#include "L2_S6_TunnelEnd.h"
#include "L2_S6_HOWaterfall.h"
#include "L2_S5_PZMedicineMaking.h"
#include "L2_S7_SoldiersPuzzle.h"  
#endif // _LEVEL2_ENABLED

//level 3 includes
#ifdef _LEVEL3_ENABLED
#include "L3_S1_FortEntrance.h"
#include "L3_S1_HOLadder.h"
#include "L3_S1_PZLionHead.h"
#include "L3_S2_FortInterior.h"
#include "L3_S2_P1Room1Start.h"
#include "L3_S2_P1Room2Wine.h"
#include "L3_S2_P1Room3Lock.h"
#include "L3_S2_P1Room4Match3.h"
#include "L3_S2_P1Room5Pipe.h"
#include "L3_S2_P1Room6Pillar.h"
#include "L3_S3_Pool.h"
#include "L3_S3_PZBlock.h"
#include "L3_S3_PZUnderwater.h"
#include "L3_S4_FortFountain.h"
#include "L3_S4_FortFountainHO.h"
#include "L3_S5_Cliff.h"  
#endif // _LEVEL3_ENABLED

//level 4 includes
#ifdef _LEVEL4_ENABLED
#include "L4_S1_SwampEntrance.h"
#include "L4_S1_ShoeHO.h"
#include "L4_S2_RiverSide.h"
#include "L4_S2_AlligatorPuzzle.h"
#include "L4_S2_FanPuzzle.h"
#include "L4_S3_Cave.h"
#include "L4_S3_PZFireMaking.h"
#include "L4_S3_PZDoor.h"
#include "L4_S4_HillTop.h"
#include "L4_S4_BinocularHO.h"
#include "L4_S5_HillSide.h"
#include "L4_S5_PZSwapTiles.h"  
#endif // _LEVEL4_ENABLED


//level 5 includes
#ifdef _LEVEL5_ENABLED
#include "L5_S1_TigerDistant.h"
#include "L5_S2_TigerCloseup.h"
#include "L5_S2_PZBlocks.h"
#include "L5_S3_Pond.h"
#include "L5_S3_PZDungeonDoor.h"
#include "L5_S3_HOUnderwater.h"
#include "L5_S4_Dungeon.h"
#include "L5_S4_PZHandCuff.h"
#include "L5_S5_MountainBelow.h"
#include "L5_S5_HOGanapathi.h"
#include "L5_S5_PZSunClock.h"
#include "L5_S6_MountainTop.h"	
#include "L5_S6_HOTree.h"  
#endif // _LEVEL5_ENABLED


//level 6 includes
#ifdef _LEVEL6_ENABLED
#include "L6_S1_FloatingIsland_1.h" 
#include "L6_S1_DescriptionHO.h"
#include "L6_S2_FloatingIsland_2.h"
#include "L6_S2_SphereHO.h"
#include "L6_S2_PZPineGum.h"
#include "L6_S3_FloatingIsland_3.h"
#include "L6_S3_FakeJames.h"
#include "L6_S3_GlowingHeartHO.h"
#include "L6_S3_PZPathway.h"
#include "L6_S4_FloatingIsland_4.h"
#include "L6_S4_PortalPuzzle.h"
#include "L6_S5_TabMiniGame.h"
#include "L6_S4_PortalChest.h"
#include "L6_S5_MarysDream.h"
#endif // _LEVEL6_ENABLED

//Level 7 includes
#ifdef _LEVEL7_ENABLED
#include "L7_S1_Volcano.h"
#include "L7_S1_PZStarPuzzle.h"
#include "L7_S1_PZJigsawYellow.h"
#include "L7_S2_LasyaPrison.h"
#include "L7_S2_PZEyePuzzle.h"
#include "L7_S2_PZLasyaStatue.h"
#include "L7_S2_HORightHand.h"
#include "L7_S2_HOLeftHand.h"
#include "L7_S2_HOBrokenRightHand.h"
#include "L7_S2_HOBrokenLeftHand.h"
#include "L7_S3_Haven.h"
#include "L7_S3_PZDoorPuzzle.h"
#include "L7_S3_PZJigsawRed.h"
#include "L7_S4_Ameeha.h"
#include "L7_S4_PZJigsawBlue.h"
#include "L7_S5_SophiaEnd.h"
#endif // _LEVEL7_ENABLED

//Level 8 includes
#ifdef _LEVEL8_ENABLED

#include "L8_S1_ShukrasPlace.h"
#include "L8_S1_PZDoor.h"
#include "L8_S1_HODoor.h"
#include "L8_S2_Garden.h"
#include "L8_S2_GearPuzzle.h"
#include "L8_S2_PZGate.h"
#include "L8_S2_PZSugarCane.h"
#include "L8_S3_BanyanTree.h"
#include "L8_S3_BanyanTreeHO.h"
#include "L8_S4_Pool.h"
#include "L8_S4_PZPoolGame.h"
#include "L8_S5_VolcanoLand.h"
#include "L8_S5_VirtualMiniGame.h"

#endif // _LEVEL8_ENABLED


#endif // !_DEMOBUILD

#include "MainMenuControl.h"


#define MAX_GLOWRING_FRAMES 34



GameFlow::GameFlow()
{
    IsFirstTimeGFLoadingComplete =false;
	GFInstance = this;
	PausedImage = NULL;
	IsReplayingGame = false;
	IsLowFPSMachine = false;
	IsSlowSystem = false;
	availablePhyMem = 0;
	IsFullCacheMem = false;
	IsPartialCacheMem = false;
	IsPrevSingleUpdate = false;
	IsForceTextures16Bit = false;
	IsInsideLevel = false;
	IsWatchingPromo = false;

	//Hardcode Title name if LN_PORTUGUESE
#ifdef K_WIN32
	K_LOG(std::string("Title Name : " + FileSystem::ws2s(AppTitle_Str)).c_str());
		if(StringTable::GetMyLangauge() == "BR")
			AppTitle_Str = L"DARKARTA : A jornada de um coração partido";
#endif

#ifdef K_ANDROID
		_nActualWidth=0;
		_nActualHeight = 0;
		_nWindowWidth=0;
		_nWindowHeight = 0;
		
		_nActualWidth = androidGetScreenWidth ();
	_nActualHeight = androidGetScreenHeight ();
		_nWindowWidth = GFApp->DesktopWidth;
	_nWindowHeight = GFApp->DesktopHeight;
	#endif

	#ifdef _CEBUILD
		#ifdef K_WIN32			
			GFApp->Window->setTitle(FileSystem::ws2s(AppTitle_Str).c_str());
		#else
			GFApp->Window->setTitle(AppTitle_Str.c_str());
		#endif
	#else
		#ifdef K_WIN32			
			GFApp->Window->setTitle(FileSystem::ws2s(AppTitle_Str).c_str());
		#else
			GFApp->Window->setTitle(AppTitle_Str.c_str());
		#endif
	#endif

	

	RT_BgScn = new CObject();
	RT_BgScn->LoadSprite("UI\\ZOOMBG.SPR");
	RT_BgScn->SetZ(eZOrder_PopupMask - 10);
	RT_BgScn->SetEnable(false);
	//K_LOG("Zoombg.spr loaded");
    
    lpLoadingScnBG = KPTK::createKGraphic();
    
	#ifdef K_ANDROID
		GamePath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/");
		//InitScnLoadingSpr();
	#endif 

	LastSaveTime = -1;
	AS_TrackPlaying_in_iPad = Audio_Nil;

	SaveMaskObjZ = -1;
	GFInfoLive = false;
	IsUpgradeInfoPopup = false;
	IsDiffModeSelectInfoPopup = false;
	IsWideScreenChangeInfoPopup = false;
	Is1360x768GFInfoPopup = false;
	Is1366x768GFInfoPopup = false;
	Is1280x768GFInfoPopup = false;
	Is1280x720GFInfoPopup = false;
    Is1280x800GFInfoPopup = false; //Support for mac
    Is1152x720GFInfoPopup = false; //Support for mac
    Is1024x640GFInfoPopup = false; //Support for mac
    Is1920x1080GFInfoPopup = false; //Full HD
	Is1920x1200GFInfoPopup = false; //Full HD+
    Is2560x1440GFInfoPopup = false; //2k or QHD
    Is3840x2160GFInfoPopup = false; //4k
    Is7680x4320GFInfoPopup = false; //8K
    Is1600x900GFInfoPopup = false;
    Is1344x756GFInfoPopup = false;
	Is1024x768GFInfoPopup = false;
	Is800x600GFInfoPopup = false;
	GFInfoControl = NULL;

	IsCacheDataLoaded = false;
	IsActivePopup = false;
	IsInGameMenuPopup = false;

	IsCrossFadeTransSet = false;
	IsLvlCommonTracksLoaded = false;

	IsPakResLoadInited = false;	
	IsPakResLoadTerminate = false;
	IsPakResLoadDone = false;
	iPakRes = 0;	
	IsSplashFullAlpha = false;
	IsPakLoading = false;

	iWrongMsg = rand()%MAX_WRONGDROP_MSG;


	for( int i = 0; i < 2; i++ )
		ThanksText[i] = NULL;

	for( int i = 0; i < MAX_BUFFER_SCN; i++ )
	{
		IsBufferSceneLoaded[i] = false;
		BufferScenes[i] = NULL;		
	}
	for( int j = 0; j < MAX_BUFFER_OBJ; j++ )
	{
		IsBufferObjectLoaded[j] = false;
		BufferObjects[j] = NULL;		
	}	
	for( int k = 0; k < MAX_BUFFER_SUPERFX; k++ )
	{
		IsBufferSuperFXLoaded[k] = false;
		BufferSuperFX[k] = NULL;		
	}

	iBufferLvl_InvHOObjLoaded = Level_Nil;
	for( int m = 0; m < eInv_Max; m++ )
		BufferLvlInvHOObj[m] = NULL;


	PrevSceneGraphic = NULL;
	NewSceneGraphic = NULL;

	IsStartCutsceneLoaded = false; 

	lpStartCutsceneVideo = NULL;
	lpEndCutsceneVideo = NULL;	
	lpStartCutsceneSndStream = NULL;

	IsEndCutsceneLoaded = false;

	strikeGlowFX = NULL;
	LoadingScene = NULL;
	GreyAlphaImage = NULL;
	activeCutsceneID = -1;
	IsCutsceneVideoPlayerActive = false;

	IsRenderingToRT = false;
	PrevRenderZorder = 0;
	PStartX = 0;
	PStartY = 0;
	PEndX = 0;
	PEndY = 0;
	RTGraphic =  NULL;
	RTGraphic =  KPTK::createKGraphic();
	#ifdef K_MACOS
		//try a 1152 render target
		IsRTCreated = RTGraphic->makeRenderTarget(1152,720,true,true,true);
	#else
		//try a 1024 render target
		#ifdef K_ANDROID
			IsRTCreated = RTGraphic->makeRenderTarget(1024,768,true,true,true);
		#else
			IsRTCreated = RTGraphic->makeRenderTarget(1024,768,true,true,true);
		#endif
	#endif

	if( !IsRTCreated )
	{
		K_LOG("RTGraphic failed");
		#ifdef K_WIN32
			//FATAL((const char*)L"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
           // WARN((const char*)L"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");

        #else
			//FATAL((const char*)"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
           // WARN((const char*)"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");

        #endif
	}
	IsSceneGraphicReloaded = false;
	IsScnBg1024Res = false;
	CurrentSceneGraphic =  NULL;
	CurrentSceneGraphic =  KPTK::createKGraphic();
	IsScnRTCreated = CurrentSceneGraphic->makeRenderTarget(1366,768,true,true,true);
	if( !IsScnRTCreated )
	{
            //try a 1024 render target
            IsScnRTCreated = CurrentSceneGraphic->makeRenderTarget(1024,768,true,true,true);
            IsScnBg1024Res = true;
     	if( !IsScnRTCreated )
		{
			K_LOG("CurrentSceneGraphic failed, Render Target not supported");
			#ifdef K_WIN32
			//	FATAL((const char*)L"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
			   // WARN((const char*)L"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
            #else
            //  FATAL((const char*)"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
			   // WARN((const char*)"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
            #endif
		}
	}

	LoadingFile = 0;
	nPostUpdates = 0;
	for( int i = 0; i < eArea_Max; i++ )
		IsScnElmntsPreloaded[i] = false;	

	CreateThanks();

	CheckAvailablePhyMemory();

	//Cursor is used in removecontrol and like!!
	CTexFactory::Instance->LoadImg("UI\\CUR.PNG",false,true,true);//24 bit for iPad
	CTexFactory::Instance->LoadImg("UI\\HUD\\RDESC.PNG",false,true,true);//24 bit for iPad

	Cursor::Create();
	//Cursor::Show(false);//turn for splash

	#ifdef _DEBUG
		SLOG::Create();  
	#endif // _DEBUG


	CTexFactory::Instance->LoadImg("LD\\LOADERBG.JPG",false,true,true);
	CTexFactory::Instance->LoadImg("LD\\REDLOADBAR.PNG",false,true,true);
	LoadingScene = new CHOScene();				
	LoadingScene->Load("LD\\LOADSCN.SCN");

	//Localized
	CETitle = new CObject();
	CETitle->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\LD.SPR", 683, 610, 2000);
	CETitle->SetUID("menutitle");
	CETitle->SetScale(0.65);
#ifndef _CEBUILD
	CETitle->PlayAnimation(1);
#endif // !_CEBUILD
	LoadingScene->PushObject(CETitle);

	LoadingScene->Show(false);
	LoadingScene->SetZOff(40000);	
	LoaderBase = LoadingScene->GetObjectByID("Base");//-1
	LoaderGlowBar = LoadingScene->GetObjectByID("loadbar");//1
	//CETitle = LoadingScene->GetObjectByID("menutitle");
	//turn off ce
	//#ifndef _CEBUILD
		//CETitle->PlayAnimation(1);
	//#endif

	//Load splash elements!!
	//Control::Audio->LoadSample(aSFXLogoSplash, AudioSFX);//decoding may take little time!!
	Control::Audio->LoadSample(aSFXMagic1, AudioSFX);
	Control::Audio->LoadSample(aSFXMagic3, AudioSFX);

	#ifdef _RELEASEBUILD //Release build		
		{
			if(!GFApp->IsSkipSplash)
			{
				#ifdef _PUBLISHER_BUILD
					CTexFactory::Instance->LoadImg("SPL\\PUB.JPG",false,true,true);
				#endif
				CTexFactory::Instance->LoadImg("SPL\\TF.PNG",false,true,true);//24 bit for iPad
				Flow = fSplash; //tf_load to splash logos
				AddControl(new SplashSeries());
			}
			else
			{
				IsSplashFullAlpha = true;
				initPakResLoad();
				Flow = fRootMenu; //tf_load to mainmenu
				#ifdef TOUCH_UI
					Cursor::SetInMenu(true);
				#endif
				AddControl(new MenuSystem());
				Cursor::Show(true);
			}
		}
	#else //Debug build!!	
		{
			if(!GFApp->IsSkipSplash)
			{
				#ifdef _PUBLISHER_BUILD
					CTexFactory::Instance->LoadImg("SPL\\PUB.JPG",false,true,true);
				#endif
				CTexFactory::Instance->LoadImg("SPL\\TF.PNG",false,true,true);//24 bit for iPad
				Flow = fSplash; //tf_load to splash logos
				AddControl(new SplashSeries());
			}
			else
			{
				IsSplashFullAlpha = true;
				initPakResLoad();
				Flow = fRootMenu; //tf_load to mainmenu
				#ifdef TOUCH_UI
					Cursor::SetInMenu(true);
				#endif
				AddControl(new MenuSystem());
				Cursor::Show(true);
			}
		}

		PrevFlow = Flow;
	#endif

	ClearPrevLevelInventories();	//to handle prevlevelinv_carryontonextlevel_bugs created due to wrong_inventory!!

    IsFirstTimeGFLoadingComplete = true;
	/*
	youFound = new CObject();
	youFound->LoadSprite("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\YOUFOUND.SPR");
	youFound->SetZ(100000);
	youFound->SetXY(683, 200);
	youFound->Show(true);
	*/
}

void GameFlow::ReloadSceneGraphic()
{

	if(RTGraphic->isRenderTargetLost())
	{
		if (RTGraphic != NULL)
			RTGraphic->freePicture();
		RTGraphic = NULL;
		RTGraphic = KPTK::createKGraphic();
#ifdef K_MACOS
        //try a 1152 render target
        IsRTCreated = RTGraphic->makeRenderTarget(1152,720,true,true,true);
#else
		IsRTCreated = RTGraphic->makeRenderTarget(1024, 768, true, true, true);
#endif
		if (!IsRTCreated) {
			K_LOG("RTGraphic failed");
#ifdef K_WIN32
            //FATAL((const char*)L"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
           // WARN((const char*)L"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");

#else
			//FATAL((const char*)"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
			// WARN((const char*)"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
#endif
		}
	}

	if(CurrentSceneGraphic->isRenderTargetLost()) {
		IsScnBg1024Res = false;
		if (CurrentSceneGraphic != NULL)
			CurrentSceneGraphic->freePicture();
		CurrentSceneGraphic = NULL;
		CurrentSceneGraphic = KPTK::createKGraphic();
		IsScnRTCreated = CurrentSceneGraphic->makeRenderTarget(1366, 768, true, true, true);
		IsSceneGraphicReloaded = true;
		if (!IsScnRTCreated) {
			//try a 1024 render target
			IsScnRTCreated = CurrentSceneGraphic->makeRenderTarget(1024, 768, true, true, true);
			IsScnBg1024Res = true;
			if (!IsScnRTCreated) {
				K_LOG("CurrentSceneGraphic failed, Render Target not supported");
#ifdef K_WIN32
                //	FATAL((const char*)L"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
                   // WARN((const char*)L"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
#else
				//	FATAL((const char*)"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
				//  WARN((const char*)"Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!");
#endif
				IsScnBg1024Res = false;
			}
		}
	}
}


void GameFlow::ProcessMessage(sMessage message)
{
	//Process Message
	if( GFInfoControl != NULL )
		if( message.CallerID == GFInfoControl->ID )
		{
			if( GFInfoControl->IsOKCancelButtons )
			{
				if(message.Value == INFO_CLOSE)
					return;				
				else if( message.Value == INFO_B1 ) //Ok relaunch
				{
					if( IsWideScreenChangeInfoPopup )
					{
						//already handled in mainmenu with a checkbox!!  no further needed!!
					}
					//just try and satisfy with what possible in this machine!! 
					else if( IsUpgradeInfoPopup )//Upgrading....
					{
						ProfileSystem::Instance->ActiveProfile.Windowed = false;
						switch( ProfileSystem::Instance->ActiveProfile.Resolution )
						{
						case R640x480: ProfileSystem::Instance->ActiveProfile.Resolution  = R800x600; break;
						case R800x600: ProfileSystem::Instance->ActiveProfile.Resolution  = R1024x768; break;

							//Iterate from this mode OF POSSIBLE MODES!!
						case R1024x768: 
						case R1280x768: 
						case R1280x720: 
						case R1280x800: 
						case R1360x768: ProfileSystem::Instance->ActiveProfile.Resolution = R1366x768; break;
						case R1366x768: ProfileSystem::Instance->ActiveProfile.Resolution = R1920x1080; break;
						case R1920x1080: ProfileSystem::Instance->ActiveProfile.Resolution = R1920x1200; break;
						case R1920x1200: ProfileSystem::Instance->ActiveProfile.Resolution = R2560x1440; break;
						case R2560x1440: ProfileSystem::Instance->ActiveProfile.Resolution = R3840x2160; break;
						case R3840x2160: ProfileSystem::Instance->ActiveProfile.Resolution = R7680x4320; break;
						}
						ProfileSystem::Instance->ActiveProfile.BPP = 32;
					}
					else 
					{
						//Save relaunch profile
						ProfileSystem::Instance->ActiveProfile.Windowed = true;
						//Only 5 type of downgrades possible!!
						if ( Is7680x4320GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R7680x4320; //8K
						else if ( Is3840x2160GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R3840x2160; //4K
						else if ( Is2560x1440GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R2560x1440; //QHD
						else if ( Is1920x1200GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R1920x1200; //FullHD+
						else if ( Is1920x1080GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R1920x1080; //FullHD
						else if ( Is1600x900GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R1600x900;
						else if ( Is1366x768GFInfoPopup )
							ProfileSystem::Instance->ActiveProfile.Resolution = R1366x768; 
						else if ( Is1360x768GFInfoPopup )
							ProfileSystem::Instance->ActiveProfile.Resolution = R1360x768; 
						else if ( Is1344x756GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R1344x756;
						else if ( Is1280x768GFInfoPopup )
							ProfileSystem::Instance->ActiveProfile.Resolution = R1280x768; 
						else if ( Is1280x720GFInfoPopup )
							ProfileSystem::Instance->ActiveProfile.Resolution = R1280x720;
                        else if ( Is1280x800GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R1280x800;
                       else if ( Is1152x720GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R1152x720;
						else if ( Is1024x768GFInfoPopup )
							ProfileSystem::Instance->ActiveProfile.Resolution = R1024x768; 
                        else if ( Is1024x640GFInfoPopup )
                            ProfileSystem::Instance->ActiveProfile.Resolution = R1024x640;
						else if( Is800x600GFInfoPopup )
							ProfileSystem::Instance->ActiveProfile.Resolution = R800x600; 
						else
							ProfileSystem::Instance->ActiveProfile.Resolution = R640x480; 				
					}

					ProfileSystem::Instance->SaveProfile(ProfileSystem::Instance->ProfileSlot);
					ProfileSystem::Instance->WriteProfileSystemData();
					//Safe exit - expecting user will relaunch soon!!
					exit(0);
				}
				else if( message.Value == INFO_B2 ) //Cancel relaunch
				{
					GFInfoLive = false;


					IsUpgradeInfoPopup = false;
					Is1360x768GFInfoPopup = false;		
					Is1366x768GFInfoPopup = false;				
					Is1280x768GFInfoPopup = false;				
					Is1280x720GFInfoPopup = false;
                    Is1280x800GFInfoPopup = false; //Support for mac
                    Is1152x720GFInfoPopup = false; //Support for mac
                    Is1024x640GFInfoPopup = false; //Support for mac
                    Is1920x1080GFInfoPopup = false; //Full HD
					Is1920x1200GFInfoPopup = false; //Full HD+
                    Is2560x1440GFInfoPopup = false; //2k or QHD
                    Is3840x2160GFInfoPopup = false; //4k
                    Is7680x4320GFInfoPopup = false; //8K
                    Is1600x900GFInfoPopup = false;
                    Is1344x756GFInfoPopup = false;
					Is1024x768GFInfoPopup = false;				
					Is800x600GFInfoPopup = false;				
					GFInfoControl->ExternalClose();
					if( IsWideScreenChangeInfoPopup )//restore flag to old values!!
					{
						//if they cancel exitrelaunch, it will change to widescreen in next launch!!						
						IsWideScreenChangeInfoPopup = false;
						ProfileSystem::Instance->ActiveProfile.IsWideScreen = !ProfileSystem::Instance->ActiveProfile.IsWideScreen;						
					}

					if( IsBufferObjectLoaded[MASK_OBJ] )
						BufferObjects[MASK_OBJ]->SetZ(SaveMaskObjZ);
					SaveMaskObjZ = -1;
					return;
				}			
			}
		}

		switch( message.Protocol )
		{
		case ControlExit:
			{
				if( RemoveControl(message.CallerID, message.Value) )				
					SendMessage(ActionProcessed, ID, 0);				
				else				
					SendMessage(RemovalError, ID, message.CallerID);				
			}
			break;
		default:
			SendMessage(message.Protocol, message.CallerID, message.Value);
		}
}

void GameFlow::InitGFInfoControl()
{
	SaveMaskObjZ = -1;
	GFInfoLive = false;
	Is1360x768GFInfoPopup = false;
	Is1366x768GFInfoPopup = false;
	Is1280x768GFInfoPopup = false;
	Is1280x720GFInfoPopup = false;
    Is1280x800GFInfoPopup = false;  //Support for mac
    Is1152x720GFInfoPopup = false; //Support for mac
    Is1024x640GFInfoPopup = false; //Support for mac
    Is1920x1080GFInfoPopup = false; //Full HD
	Is1920x1200GFInfoPopup = false; //Full HD+
    Is2560x1440GFInfoPopup = false; //2k or QHD
    Is3840x2160GFInfoPopup = false; //4k
    Is7680x4320GFInfoPopup = false; //8K
    Is1600x900GFInfoPopup = false;
    Is1344x756GFInfoPopup = false;
	Is1024x768GFInfoPopup = false;
	Is800x600GFInfoPopup = false;
	GFInfoControl = new InfoPanel();
	AddControl(GFInfoControl);

	GFInfoControl->SetInfoText("Exwindowe",50);//GFInfo window text set

	GFInfoControl->Deactivate();	
}

void GameFlow::ActivateGFInfo(std::string b1, std::string b2)
{
	GFInfoLive = true;
	if( IsBufferObjectLoaded[MASK_OBJ] )
	{
		SaveMaskObjZ = BufferObjects[MASK_OBJ]->ZOrder;
		BufferObjects[MASK_OBJ]->SetZ(24000);	
	}	
	if( IsWideScreenChangeInfoPopup )
	{
		if( ProfileSystem::Instance->ActiveProfile.IsWideScreen )
			GFInstance->GFInfoControl->SetInfoText("Exwidescree",50);
		else
			GFInstance->GFInfoControl->SetInfoText("Exnonwidescree",50);							
	}
	else if( IsDiffModeSelectInfoPopup )
		GFInfoControl->SetInfoText("Excasual",50+5-23);//GFInfo window text set
	else if( IsUpgradeInfoPopup )//Upgrading!!
		GFInfoControl->SetInfoText("Exhighreso",50);//GFInfo window text set
	else 
		GFInfoControl->SetInfoText("Exwindowe",50);//GFInfo window text set			

	GFInfoControl->SetButtons(b1,b2);
	GFInfoControl->Activate();
	GFInfoControl->SetModal(true);
}

void GameFlow::initPakResLoad()
{
	IsPakResLoadInited = true;
}

void GameFlow::UnloadStartCutscene()
{	
	if( lpStartCutsceneVideo != NULL )
	{
		delete lpStartCutsceneVideo; lpStartCutsceneVideo = NULL;	
	}		
	#ifdef ENGLISH_BUILD
		if( lpStartCutsceneSndStream != NULL )
		{
			lpStartCutsceneSndStream->stopSample();
			delete lpStartCutsceneSndStream;
			lpStartCutsceneSndStream = NULL;
		}

	//Control::Audio->UnloadSample(aTrackCutscnGameIntro);
	#else
		Control::Audio->UnloadSample(aTrackCutscnGameIntro);//this track not needed further!!
	#endif
	IsStartCutsceneLoaded = false;
}

void GameFlow::LoadStartCutscene()
{
	if( !IsStartCutsceneLoaded )
	{		
		if( lpStartCutsceneVideo == NULL )
			lpStartCutsceneVideo = new KVideo;
		#ifdef ENGLISH_BUILD
			if( lpStartCutsceneSndStream == NULL )
				if( !ProfileSystem::Instance->ActiveProfile.IsMuted )//Since we won't change settings inbetween!!
				{
					#ifdef K_WIN32
						{
							if( !KSound::g_bErrSoundDeviceInit )
								lpStartCutsceneSndStream = new KSound;
						}
					#else
						lpStartCutsceneSndStream = new KSound;
					#endif
				}

				//Control::Audio->LoadSample(aTrackCutscnGameIntro,AudioTrack,false);
		#else
			Control::Audio->LoadSample(aTrackCutscnGameIntro,AudioTrack,false);
		#endif

	#ifdef ENGLISH_BUILD
		IsStartCutsceneLoaded = lpStartCutsceneVideo->openVideo(KMiscTools::makeFilePath("Data\\START.OGV"),true,NULL,lpStartCutsceneSndStream,false);
	#else
		IsStartCutsceneLoaded = lpStartCutsceneVideo->openVideo(KMiscTools::makeFilePath("Data\\START.OGV"),true,NULL,NULL,false);
	#endif
	
	if (!IsStartCutsceneLoaded ) 
		{
			#ifdef K_WIN32
				KMiscTools::messageBox ("Video loading error!", "Couldn't open Start Video");	
			#else
				KMiscTools::messageBox ((const char*)"Video loading error!", (const char*)"Couldn't open Start Video");	
			#endif
			exit(0);
		}
	}
}

//bool GameFlow::CreateSceneScreenshot(CHOScene * scene, CObject * returnscreenshot)
CObject * GameFlow::CreateSceneScreenshot(CHOScene * scene, bool *_success)
{


	CObject * screenshot = new CObject();
	screenshot->LoadSprite("UI\\ZOOMBG.SPR");
	screenshot->SetZ(0);
	screenshot->SetEnable(false);	

	bool isScnBg1024Res = false;

	KGraphic * rtGraphic =  KPTK::createKGraphic();
	bool isRTGraphic = rtGraphic->makeRenderTarget(1366,768,true,true,true);
	if( !isRTGraphic )
	{
		//try a 1024 render target
		isRTGraphic = rtGraphic->makeRenderTarget(1024,768,true,true,true);
		isScnBg1024Res = true;
		if( !isRTGraphic )
		{
			K_LOG("CurrentSceneGraphic failed, Render Target not supported");
            /*
            #ifdef K_WIN32
				FATAL((const char*)L"Er0x1602- Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!"); 
			#else
				FATAL((const char*)"Er0x1602- Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!"); 
			#endif
             */
			isScnBg1024Res = false;
			
		}
	}
    if(isRTGraphic){
        rtGraphic->clearRenderTarget();
        rtGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS

        if(isScnBg1024Res)
        {
            scene->RenderToTarget(-171.0f);
        }
        else
        {
            scene->RenderToTarget();
        }
        rtGraphic->endRenderToTarget();
        screenshot->pSprite->Image = rtGraphic;
        *_success = true;
		return screenshot;
    }
    *_success = false;	
	return screenshot;
}


void GameFlow::UnloadEndCutscene()
{	
	if( lpEndCutsceneVideo != NULL )
	{
		Control::Audio->UnloadSample(aTrackEndCutscene);//this track not needed further!!
		delete lpEndCutsceneVideo; lpEndCutsceneVideo = NULL;	
	}	
	IsEndCutsceneLoaded = false;
}

void GameFlow::LoadEndCutscene()
{
	if( !IsEndCutsceneLoaded )
	{

		if( lpEndCutsceneVideo == NULL )
			lpEndCutsceneVideo = new KVideo;
		
		IsEndCutsceneLoaded = lpEndCutsceneVideo->openVideo(KMiscTools::makeFilePath("Data\\END.OGV"),true,NULL,NULL,false,true);			
	
		if (!IsEndCutsceneLoaded )
		{
			#ifdef K_WIN32
				KMiscTools::messageBox ("Video loading error!", "Couldn't open End Video");	
			#else
				KMiscTools::messageBox ((const char*)"Video loading error!", (const char*)"Couldn't open End Video");	
			#endif
			exit(0);
		}
	}
	Control::Audio->LoadSample(aTrackEndCutscene,AudioTrack,false);							
}


#define CACHE_DATA_START 59

void GameFlow::LoadPakRes()//Load only from pak
{	
	if( iPakRes < CACHE_DATA_START ) //0..59
	{
		switch( iPakRes ) 
		{		
			//Load too much heavy resources outside to make 3 splash effects uniform
		case 0:  if( !ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed && ProfileSystem::Instance->ActiveProfile.CurrentLevel <= ELevel::Level_1)
					 LoadStartCutscene();
			PostUpdate(1);

			break;
		case 1:CTexFactory::Instance->LoadImg("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\CORE.PNG",false,true, true); PostUpdate(5);break;
		case 2:CTexFactory::Instance->LoadImg("PRT\\STRKGLW.PNG",false,true,true);PostUpdate(5);break;
		case 3:	strikeGlowFX = new ParticleEffect();
			strikeGlowFX->LoadEffectIni("PRT\\strkGlw.INI");
			strikeGlowFX->SetZ(2200);PostUpdate(1);
			break;
		case 4:CTexFactory::Instance->LoadImg("UI\\MASK.PNG",false,true,true);PostUpdate(1);break;//24 bit for iPad
		case 5:	break;

		case 6: CTexFactory::Instance->LoadImg("UI\\CLOSEBTN.PNG",false,true,true);break;
		case 7: CTexFactory::Instance->LoadImg("UI\\POPRECTFRAME.PNG",false,true,true);break;
		case 8: CTexFactory::Instance->LoadImg("UI\\POPCIRCFRAME.PNG",false,true,true);break;
			break;
		case 9:break;//24 bit for iPad

		case 10:GreyAlphaImage = new CObject();
			GreyAlphaImage->LoadSprite("UI\\MASK.SPR");
			GreyAlphaImage->SetXY(683,384);
			GreyAlphaImage->SetScale(2.0);
			GreyAlphaImage->Show(false);
			GreyAlphaImage->SetZ(eZOrder_PopupMask);PostUpdate(1);
			break;
		case 11:break;
		case 12:break;
		case 13:break;
		case 14:break;

		case 15:CTexFactory::Instance->LoadImg("INV\\BASE.PNG",false,true,true);PostUpdate(5);break;//24 bit for iPad
		case 16:break;
		case 17:break;
		case 18:break;
		case 19:CTexFactory::Instance->LoadImg("UI\\PCIRCCLOSE.PNG",false,true,true);PostUpdate(5);break;//24 bit for iPad

		case 20:CTexFactory::Instance->LoadImg("UI\\POPRECTCLOSE.PNG",false,true,true);PostUpdate(5);break;//24 bit for iPad
		case 21:break;//24 bit for iPad //moved to LV1
		case 22:break;//24 bit for iPad
		case 23:break;//24 bit for iPad - CTexFactory::Instance->LoadImg("TUT\\TUT.PNG",false,true,true);
		case 24:break;//24 bit for iPad

		case 25:CTexFactory::Instance->LoadImg("PRT\\GOLDDUST.PNG",false,true,true);PostUpdate(5);break;//24 bit for iPad
		case 26:PostUpdate(5);break;//24 bit for iPad
		case 27:break;//24 bit for iPad
		case 28:CTexFactory::Instance->LoadImg("PRT\\SPARKLEFX.PNG",false,true,true);break;//24 bit for iPad
		case 29:break;//24 bit for iPad

	#ifdef FAST_NOSNDCACHE_BUILD
			IsCacheDataLoaded = true;
			IsPakResLoadTerminate = true;	
	#endif
			break;

			//Loading core sounds effects
		case 30:Control::Audio->LoadSample(aSFXMagic4,AudioSFX);break;
		case 31:Control::Audio->LoadSample(aSFXRefill2, AudioSFX);break;	
		case 32:Control::Audio->LoadSample(aSFXNotifySoft, AudioSFX);break;
		case 33:Control::Audio->LoadSample(aSFXNotifySoft2, AudioSFX);break;
		case 34:Control::Audio->LoadSample(aSFXMouseover,AudioSFX);break;

		case 35:Control::Audio->LoadSample(aSFXCrystal, AudioSFX);break;
		case 36:Control::Audio->LoadSample(aSFXRefill, AudioSFX);break;
		case 37:Control::Audio->LoadSample(aSFXGlassBtn, AudioSFX);break;
		case 38:Control::Audio->LoadSample(aSFXRefill3,AudioSFX);break; 
		case 39:Control::Audio->LoadSample(aSFXFootsteps0, AudioSFX);break;

		case 40:Control::Audio->LoadSample(aSFXFootsteps1, AudioSFX);break;
		case 41:Control::Audio->LoadSample(aSFXFootsteps2, AudioSFX);PostUpdate(5);break;							
		case 42:Control::Audio->LoadSample(aSFXWoodShake, AudioSFX);break;
		case 43:Control::Audio->LoadSample(aSFXInvItemPick,AudioSFX);break;	
		case 44:Control::Audio->LoadSample(aSFXMagic,AudioSFX);break;

		case 45:Control::Audio->LoadSample(aSFXMagic2,AudioSFX);break;
		case 46:Control::Audio->LoadSample(aSFXClick, AudioSFX);break;
		case 47:break; 
		case 48:Control::Audio->LoadSample(aSFXNotifyHard, AudioSFX);break;
		case 49:Control::Audio->LoadSample(aSFXpopup,AudioSFX);break;		

		case 50:break;	
		case 51:break;					
		case 52:break;
		case 53:Control::Audio->LoadSample(aSFXInvWrongDrop,AudioSFX);;break;	
		case 54:Control::Audio->LoadSample(aSFXpopdown,AudioSFX);PostUpdate(5);break;

		case 55:Control::Audio->LoadSample(aSFXFolly,AudioSFX);break;
		case 56:break;
		case 57:Control::Audio->LoadSample(aSFXInvArrowClick,AudioSFX);PostUpdate(5);break;
		case 58:
			//..............................................	
			//..............................................
			//..............................................
			if( !IsFullCacheMem )
				IsPakResLoadTerminate = true;	
			break;
		}
	}
	//CACHE MORE TO MEMORY!! -
	else 
	{		
		switch( iPakRes ) 
		{	
			//CACHE_DATA_START
		case 59: IsCacheDataLoaded = true;
			IsPakResLoadTerminate = true;
			break;
		}  		   
	}	
	iPakRes++;   
}

SuperFX* GameFlow::LoadBufferSuperFX( int FxId )
{
	if( !IsBufferSuperFXLoaded[FxId] )
	{
		switch( FxId )
		{
		default: return NULL;
		case HINTCHARGED_SUPERFX:
			BufferSuperFX[FxId] = new SuperFX("PRT\\UI\\hintcharge.INI", PointSystem::CreateCPoint(1110.0f,690.0f,0), eZOrder_HUD+25);
			break;
		case HINTTRAIL_SUPERFX:
			BufferSuperFX[FxId] = new SuperFX("PRT\\UI\\hinttrail.INI", PointSystem::CreateCPoint(1110.0f,690.0f,0), 5000);
			break;
		case HINTTRAILSOFT_SUPERFX:
			BufferSuperFX[FxId] = new SuperFX("PRT\\UI\\hintsofttrail.INI", PointSystem::CreateCPoint(1110.0f,690.0f,0), 5000);
			break;
		case HINTCIRCLE_SUPERFX:
			BufferSuperFX[FxId] = new SuperFX("PRT\\UI\\hintcirc.INI", PointSystem::CreateCPoint(1110.0f,690.0f,0), 5000);
			break;
		case HINTEXPLODE_SUPERFX:
			BufferSuperFX[FxId] = new SuperFX("PRT\\UI\\hintexplo.INI", PointSystem::CreateCPoint(1110.0f,690.0f,0), 5000);
			break;
		case HINTGREENCIRCLE_SUPERFX:
			BufferSuperFX[FxId] = new SuperFX("PRT\\UI\\hintgreencirc.INI", PointSystem::CreateCPoint(1110.0f,690.0f,0), 5000);
			break;
		case DIARYFX_SUPERFX:
			BufferSuperFX[FxId] = new SuperFX("PRT\\UI\\diaryfx.INI", PointSystem::CreateCPoint(216,652,0), 3000);
			break;
		}
		IsBufferSuperFXLoaded[FxId] = true;
	}
	return BufferSuperFX[FxId];
}

void GameFlow::UnloadBufferSuperFX( int FxId )
{
	if( IsBufferSuperFXLoaded[FxId] )
	{
		BufferSuperFX[FxId]->StopImmediate();
		SAFE_DELETE(BufferSuperFX[FxId]);
		IsBufferSuperFXLoaded[FxId] = false;
	}
}

void GameFlow::UnloadBufferScene( int ScnId )
{
	if( IsBufferSceneLoaded[ScnId] )
	{
		delete BufferScenes[ScnId];
		BufferScenes[ScnId] = NULL;
		IsBufferSceneLoaded[ScnId] = false;
	}	 
}

CHOScene* GameFlow::LoadBufferScene( int ScnId )
{
	if( !IsBufferSceneLoaded[ScnId] )
	{
		switch( ScnId )
		{
		default: return NULL;

		case TUTORIAL_SCN:
		{
			BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("TUT\\TUT.SCN");
			//Localized
			CObject* temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\TUTORIAL.SPR", -10, -80, 10);
			temp->SetUID("TUTORIAL_HEAD");
			BufferScenes[ScnId]->PushObject(temp);
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\SKIP.SPR", -155, 64, 5);
			temp->SetUID("SKIP");
			BufferScenes[ScnId]->PushObject(temp);
		}
			break;

		case MAP_SCN    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\MAPL1.SCN");
			break;
		case MAP_SCNL2    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\MAPL2.SCN");
			break;

		case MAP_SCNL3    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\MAPL3.SCN");
			break;

		case MAP_SCNL4    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\MAPL4.SCN");
			break;

		case MAP_SCNL5    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\MAPL5.SCN");
			break;

		case MAP_SCNL6    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\MAPL6.SCN");
			break;

		case MAP_SCNL7    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\MAPL7.SCN");
			break;

		case MAP_SCNL8    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\MAPL8.SCN");
			break;

		case JRN_SCN    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\DIARY.SCN");
			break;

		case GUIDE_SCN    :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MAPDIA\\GUIDE.SCN");
			break;


		case HO_HUD_SCN  :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("UI\\HUD\\HUD.SCN");
			break;
		case INGAMEMENU_PSCN :
		{
			BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MNU\\IGM\\IGM.SCN");

			//Localized
			CObject* temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\IGM\\MAINMENU.SPR", 687, 519, 1);
			temp->SetUID("mainmenu");
			BufferScenes[ScnId]->PushObject(temp);
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\IGM\\MEMORIES.SPR", 689, 369, 4);
			temp->SetUID("memories");
			BufferScenes[ScnId]->PushObject(temp);
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\IGM\\OPTIONS.SPR", 694, 285, 2);
			temp->SetUID("options");
			BufferScenes[ScnId]->PushObject(temp);
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\IGM\\RESTART.SPR", 689, 444, 5);
			temp->SetUID("restart");
			BufferScenes[ScnId]->PushObject(temp);
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\IGM\\RESUME.SPR", 691, 208, 3);
			temp->SetUID("resume");
			BufferScenes[ScnId]->PushObject(temp);
		}
			break;
		case OPTIONS_PSCN :
		{
			BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MNU\\OP\\OP.SCN");
			CObject * temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\OPTIONS.SPR", 680, 127, BufferScenes[ScnId]->GetObjectByID("BtnOk")->ZOrder + 1);
			temp->SetUID("optiontitletext");
			BufferScenes[ScnId]->PushObject(temp); 
#ifdef TOUCH_UI
			float ok_btn_off = 62;
#else
			float ok_btn_off = 5;
#endif
#ifdef DISABLE_LANG
			ok_btn_off = -10;
#endif
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\OK.SPR", BufferScenes[ScnId]->GetObjectByID("BtnOk")->Pos.x, BufferScenes[ScnId]->GetObjectByID("BtnOk")->Pos.y + ok_btn_off, BufferScenes[ScnId]->GetObjectByID("BtnOk")->ZOrder + 1);
			temp->SetUID("BtnOktext");
			BufferScenes[ScnId]->PushObject(temp);
            BufferScenes[ScnId]->GetObjectByID("BtnOk")->SetY(BufferScenes[ScnId]->GetObjectByID("BtnOk")->Pos.y + ok_btn_off);
			/*
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\CANCEL.SPR", BufferScenes[ScnId]->GetObjectByID("BtnCancel")->Pos.x, BufferScenes[ScnId]->GetObjectByID("BtnCancel")->Pos.y, BufferScenes[ScnId]->GetObjectByID("BtnCancel")->ZOrder + 1);
			temp->SetUID("BtnCanceltext");
			temp->SetScale(0.9f);
			BufferScenes[ScnId]->PushObject(temp);
			*/
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MODE\\TEXTDIFF.SPR", 785, 378, BufferScenes[ScnId]->GetObjectByID("BtnOk")->ZOrder + 1);
			temp->SetUID("DiffModeText");
			BufferScenes[ScnId]->PushObject(temp);
		}
			break;
		case MODESELECT_SCN :
		{
			BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("MNU\\MODE\\MODES.SCN");
			CObject *temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MODE\\CUSTMODEITEMS.SPR", BufferScenes[ScnId]->GetObjectByID("CustModeItems")->Pos.x, BufferScenes[ScnId]->GetObjectByID("CustModeItems")->Pos.y, BufferScenes[ScnId]->GetObjectByID("CustModeItems")->ZOrder + 1);
			temp->SetUID("custommodetext");
			BufferScenes[ScnId]->PushObject(temp);
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\OK.SPR", BufferScenes[ScnId]->GetObjectByID("BtnOk")->Pos.x, BufferScenes[ScnId]->GetObjectByID("BtnOk")->Pos.y, BufferScenes[ScnId]->GetObjectByID("BtnOk")->ZOrder + 1);
			temp->SetUID("BtnOktext");
			BufferScenes[ScnId]->PushObject(temp);
			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MODE\\DIFFICULTYMODE.SPR", BufferScenes[ScnId]->GetObjectByID("BtnOk")->Pos.x, 125, BufferScenes[ScnId]->GetObjectByID("BtnOk")->ZOrder + 1);
			temp->SetUID("titletext");
			BufferScenes[ScnId]->PushObject(temp);
		}
			break;
		case RESTART_DIA :BufferScenes[ScnId] = new CHOScene();
			BufferScenes[ScnId]->Load("UI\\RESTARTDIA.SCN");

			CObject* resetText = new CObject();
			resetText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\RESETCNF.SPR", 670, 308, 2);
			resetText->SetUID("resetText");
			resetText->PlayAnimation(1);
			BufferScenes[ScnId]->PushObject(resetText);

			CObject* mask = new CObject();
			mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
			mask->SetUID("mask");
			mask->SetScale(2);
			BufferScenes[ScnId]->PushObject(mask);

			CObject* btnText = new CObject();
			btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", 590, 420, 5);
			btnText->SetUID("yesBTNText");
			BufferScenes[ScnId]->PushObject(btnText);

			btnText = new CObject();
			btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", 780, 420, 5);
			btnText->SetUID("noBTNText");
			BufferScenes[ScnId]->PushObject(btnText);

			break;

		}
		IsBufferSceneLoaded[ScnId] = true;
	}
	return BufferScenes[ScnId];
}

CObject* GameFlow::LoadBufferObject( int ObjId )
{


	if( !IsBufferObjectLoaded[ObjId] )
	{
		switch( ObjId )
		{
		default: return NULL;
		case INFO_POPUPBASE_OBJ:BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\W1.SPR");
			break;

		case INFO_WIN:		   BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\W1.SPR");
			break; 	
		case INFO_WINLRG:	   BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\W2.SPR");
			break; 	
		case INFO_OK:		   BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("MNU\\OP\\BTNOK.SPR");
			break; 	
		case INFO_SKIP:        BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("MNU\\OP\\BTNCANCEL.SPR");//B_Skip.SPR");
			break;  	

		case INFO_CANCEL:      BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("MNU\\OP\\BTNCANCEL.SPR");
			break;

		case FOUNDINV_BASE_OBJ:BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("INV\\BASE.SPR");
			break; 			

		case FOUNDINV_MASK_OBJ:BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\MASK.SPR");
			BufferObjects[ObjId]->SetXY(683,384);
			BufferObjects[ObjId]->SetScale(2.0);
			break;

		case POP_CLOSE_OBJRECT:BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\PRECTCLOSE.SPR");
			break;

		case POP_CLOSE_OBJCIRC:BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\PCIRCCLOSE.SPR");
			break;


		case INVBOX_LARR_OBJ:   BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\IB\\LEFTARR.SPR");
			BufferObjects[ObjId]->SetZ(eZOrder_INVBOXITEMS+2);
			break; 
		case INVBOX_RARR_OBJ:   BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\IB\\RIGHTARR.SPR");
			BufferObjects[ObjId]->SetZ(eZOrder_INVBOXITEMS+2);
			break; 
		case INVBOX_LOCK_OBJ:   BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\IB\\LOCK.SPR");
			BufferObjects[ObjId]->SetZ(eZOrder_INVBOXBG+1);
			break; 

		case INVBOX_BASE_OBJ:   BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\IB\\INVBOX.SPR");
			BufferObjects[ObjId]->SetZ(eZOrder_INVBOXBG);
			break; 
		case INVBOX_TOOLTXT_OBJ:BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\IB\\INVNAMEFRAME.SPR");
			break;

		case HUD_OBJBG_OBJ:		BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\HUD\\OBJBG.SPR");
			break;

		case HINT_NAVIG_CURSOR_OBJ:		BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\CUR\\CUR_HINTNAV.SPR");
			BufferObjects[ObjId]->SetTint(0.85f,0.85f,0.85f);
			BufferObjects[ObjId]->pSprite->Image->setTextureQuality(true);
			BufferObjects[ObjId]->SetXY(200,200);
			BufferObjects[ObjId]->SetAlpha(0.65f);
			BufferObjects[ObjId]->SetZ(32000);
			break;

		case HINT_RECHARGE_FRAME_BG:	BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadInit("UI\\HUD\\INVITEMTXTBG.SPR",100,100,1940,false); //1840
			break;

			//common
		case MASK_OBJ	:	   BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\MASK.SPR");
			BufferObjects[ObjId]->SetXY(683,384);
			BufferObjects[ObjId]->SetScale(2.0);
			break;
		case MODEMASK_OBJ	:  BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\MASK.SPR");
			BufferObjects[ObjId]->SetXY(683,384);
			BufferObjects[ObjId]->SetScale(2.0);
			break;

		case JRNBUTTON_OBJ	:  BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\HUD\\DIAG.SPR");
			break;
			//common
		case MENUBUTTON_OBJ	:  BufferObjects[ObjId] = new CObject();
			BufferObjects[ObjId]->LoadSprite("UI\\HUD\\MENUG.SPR");
			break;

		}
		IsBufferObjectLoaded[ObjId] = true;
	}
	return BufferObjects[ObjId];
}


void GameFlow::UnloadBufferObject( int ObjId )
{
	if( IsBufferObjectLoaded[ObjId] )
	{
		delete BufferObjects[ObjId];
		BufferObjects[ObjId] = NULL;
		IsBufferObjectLoaded[ObjId] = false;
	}	 
}

////////////////////////////////////////////////////

void GameFlow::LoadBufferLvlInvHOObj( int iLevel )
{
	if( iBufferLvl_InvHOObjLoaded == Level_Nil )
	{
		switch( iLevel )
		{
		case Level_1:
			for ( int ObjId = eInv_L1S1_Bag; ObjId <= eInv_L1S4_BookCode; ObjId++ )
			{
				BufferLvlInvHOObj[ObjId] = new CHOObject(VFX_REDSPARKLE);
				BufferLvlInvHOObj[ObjId]->LoadSprite(InventorySprNames[ObjId]);		
				BufferLvlInvHOObj[ObjId]->Show(false);
			}
			break;
		case Level_2:
			for ( int ObjId = eInv_L2S1_Aloevera; ObjId <= eInv_L2S6_PaperPuzClue; ObjId++ )
			{
				BufferLvlInvHOObj[ObjId] = new CHOObject(VFX_REDSPARKLE);
				BufferLvlInvHOObj[ObjId]->LoadSprite(InventorySprNames[ObjId]);		
				BufferLvlInvHOObj[ObjId]->Show(false);
			}
			break;
		case Level_3:
			for ( int ObjId = eInv_L3S1_Charcoal; ObjId <= eInv_L3S5_Rope; ObjId++ )
			{
				BufferLvlInvHOObj[ObjId] = new CHOObject(VFX_REDSPARKLE);
				BufferLvlInvHOObj[ObjId]->LoadSprite(InventorySprNames[ObjId]);		
				BufferLvlInvHOObj[ObjId]->Show(false);
			}
			break;
		case Level_4:
			for ( int ObjId = eInv_L4_S1_Knife; ObjId <= eInv_L4_CB_BambooWithKnife; ObjId++ )
			{
				BufferLvlInvHOObj[ObjId] = new CHOObject(VFX_REDSPARKLE);
				BufferLvlInvHOObj[ObjId]->LoadSprite(InventorySprNames[ObjId]);		
				BufferLvlInvHOObj[ObjId]->Show(false);
			}
			break;
		case Level_5:
			for ( int ObjId = eInv_L5_S1_ArrowHolder; ObjId <= eInv_L5_S5_StoneEmblem; ObjId++ )
			{
				BufferLvlInvHOObj[ObjId] = new CHOObject(VFX_REDSPARKLE);
				BufferLvlInvHOObj[ObjId]->LoadSprite(InventorySprNames[ObjId]);		
				BufferLvlInvHOObj[ObjId]->Show(false);
			}
			break;
		case Level_6:
			for ( int ObjId = eInv_L6_S1_Mortar; ObjId <= eInv_L6_CB_RopeWithMagnetisedHorseShoe; ObjId++ )
			{
				BufferLvlInvHOObj[ObjId] = new CHOObject(VFX_REDSPARKLE);
				BufferLvlInvHOObj[ObjId]->LoadSprite(InventorySprNames[ObjId]);		
				BufferLvlInvHOObj[ObjId]->Show(false);
			}
			break;
		case Level_7:
			for ( int ObjId = eInv_L7_S1_BlueStarEmblem; ObjId <= eInv_L7_S4_WellGear; ObjId++ )
			{
				BufferLvlInvHOObj[ObjId] = new CHOObject(VFX_REDSPARKLE);
				BufferLvlInvHOObj[ObjId]->LoadSprite(InventorySprNames[ObjId]);		
				BufferLvlInvHOObj[ObjId]->Show(false);
			}
			break;
		case Level_8:
			for ( int ObjId = eInv_L8_S1_MagicGlass; ObjId <= eInv_L8_CB_MagicWandWithoutKey; ObjId++ )
			{
				BufferLvlInvHOObj[ObjId] = new CHOObject(VFX_REDSPARKLE);
				BufferLvlInvHOObj[ObjId]->LoadSprite(InventorySprNames[ObjId]);		
				BufferLvlInvHOObj[ObjId]->Show(false);
			}
			break;
		}
		iBufferLvl_InvHOObjLoaded = iLevel;
	}
}


void GameFlow::UnloadBufferLvlInvHOObj( )
{
	if( iBufferLvl_InvHOObjLoaded != Level_Nil )
	{
		switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
		{
		case Level_1:for ( int ObjId = eInv_L1S1_Bag; ObjId <= eInv_L1S4_BookCode; ObjId++ )
					 {
						 delete BufferLvlInvHOObj[ObjId];
						 BufferLvlInvHOObj[ObjId] = NULL;
					 }
					 break;
		case Level_2:for ( int ObjId = eInv_L2S1_Aloevera; ObjId <= eInv_L2S6_PaperPuzClue; ObjId++ )
					 {
						 delete BufferLvlInvHOObj[ObjId];
						 BufferLvlInvHOObj[ObjId] = NULL;
					 }
					 break;
		case Level_3:for ( int ObjId = eInv_L3S1_Charcoal; ObjId <= eInv_L3S5_Rope; ObjId++ )
					 {
						 delete BufferLvlInvHOObj[ObjId];
						 BufferLvlInvHOObj[ObjId] = NULL;
					 }
					 break;
		case Level_4:for ( int ObjId = eInv_L4_S1_Knife; ObjId <= eInv_L4_CB_BambooWithKnife; ObjId++ )
					 {
						 delete BufferLvlInvHOObj[ObjId];
						 BufferLvlInvHOObj[ObjId] = NULL;
					 }
					 break;
		case Level_5:for ( int ObjId = eInv_L5_S1_ArrowHolder; ObjId <= eInv_L5_S5_StoneEmblem; ObjId++ )
					 {
						 delete BufferLvlInvHOObj[ObjId];
						 BufferLvlInvHOObj[ObjId] = NULL;
					 }
					 break;
		case Level_6:for ( int ObjId = eInv_L6_S1_Mortar; ObjId <= eInv_L6_CB_RopeWithMagnetisedHorseShoe; ObjId++ )
					 {
						 delete BufferLvlInvHOObj[ObjId];
						 BufferLvlInvHOObj[ObjId] = NULL;
					 }
					 break;
		case Level_7:for ( int ObjId = eInv_L7_S1_BlueStarEmblem; ObjId <= eInv_L7_S4_WellGear; ObjId++ )
					 {
						 delete BufferLvlInvHOObj[ObjId];
						 BufferLvlInvHOObj[ObjId] = NULL;
					 }
					 break;
		case Level_8:for ( int ObjId = eInv_L8_S1_MagicGlass; ObjId <= eInv_L8_CB_MagicWandWithoutKey; ObjId++ )
					 {
						 delete BufferLvlInvHOObj[ObjId];
						 BufferLvlInvHOObj[ObjId] = NULL;
					 }
					 break;
		}
		iBufferLvl_InvHOObjLoaded = Level_Nil;
	}	 
}

////////////////////////////////////////////////////



bool GameFlow::IsGameRoom( int eArea )
{
	switch( eArea )
	{
		//Level 1 - demo
	case eArea_L1_S1_Beach:
	case eArea_L1_S2_Frontyard:
	case eArea_L1_S3_Foyer:
	case eArea_L1_S3_PZShelf:
	case eArea_L1_S1_PZKidsPainting:
	case eArea_L1_S2_PZFuse:
	case eArea_L1_S4_PZDoor:
	case eArea_L1_S4_Bedroom:
	case eArea_L1_S4_PZPainting:
	case eArea_L1_S3_DoorHO:
	case eArea_L1_S5_TreasureRoom:
	case eArea_L1_S5_PZDynamiteMaking:
	case eArea_L1_S5_TunnelDoor:
	case eArea_L1_S5_HOTreasureRoom:
	case eArea_L1_S6_HOBoatShed:
	case eArea_L1_S6_BoatShed:
	case eArea_L1_S7_BoatDock:

	case eArea_L2_S1_ForestEntrance:
	case eArea_L2_S1_UndergroundHO:
	case eArea_L2_S2_PZMango:
	case eArea_L2_S2_NagaTemple:
	case eArea_L2_S2_HONagaTemple:
	case eArea_L2_S3_CaveMouth:
	case eArea_L2_S3_PZStoneBeam:
	case eArea_L2_S4_Cave:
	case eArea_L2_S4_PZDeities:
	case eArea_L2_S5_MoopansDen:
	case eArea_L2_S5_PZEagleUnblock:
	case eArea_L2_S5_PZMedicine_Making:
	case eArea_L2_S6_TunnelEnd:
	case eArea_L2_S6_HOWaterfall:
	case eArea_L2_S6_PZTunnelClock:
	case eArea_L2_S7_PZSoldiers:

	case eArea_L3_S1_FortEntrance:
	case eArea_L3_S1_HOLadder:
	case eArea_L3_S1_PZLionHead :
	case eArea_L3_S2_FortInterior:
	case eArea_L3_S2_P1Room1Start:
	case eArea_L3_S2_P1Room2Wine:
	case eArea_L3_S2_P1Room3Lock:
	case eArea_L3_S2_P1Room4Match3:
	case eArea_L3_S2_P1Room5Pipe:
	case eArea_L3_S2_P1Room6Pillar:
	case eArea_L3_S2_P1DeadEnd:
	case eArea_L3_S3_Pool:
	case eArea_L3_S3_PZBlocks:
	case eArea_L3_S3_PZUnderwater:
	case eArea_L3_S4_FortFountain:
	case eArea_L3_S4_FortFountainHO:
	case eArea_L3_S5_Cliff:

	case eArea_L4_S1_SwampEntrance:
	case eArea_L4_S1_ShoeHO:
	case eArea_L4_S2_PZAlligatorHead:
	case eArea_L4_S2_RiverSide:
	case eArea_L4_S3_Cave:
	case eArea_L4_S4_HillTop:
	case eArea_L4_S4_BinocularHO:
	case eArea_L4_S5_HillSide:
	case eArea_L4_S5_PZSwapTile:

		//Level 5
	case eArea_L5_S1_TigerDistant:
	case eArea_L5_S2_TigerCloseup:
	case eArea_L5_S2_PZBlocks:
	case eArea_L5_S3_Pond:
	case eArea_L5_S3_PZDungeonDoor:
	case eArea_L5_S3_HOUnderwater:
	case eArea_L5_S4_Dungeon:
	case eArea_L5_S4_PZHandCuff:
	case eArea_L5_S5_MountainBelow:
	case eArea_L5_S5_HOGanapathi:
	case eArea_L5_S5_PZSunClock:
	case eArea_L5_S6_MountainTop:	
	case eArea_L5_S6_HOTree:

		//Level 6
	case eArea_L6_S1_FloatingIsland1:
	case eArea_L6_S1_DescriptionHO:
	case eArea_L6_S2_FloatingIsland2:
	case eArea_L6_S2_ShereHO:
	case eArea_L6_S2_PZPineGum:
	case eArea_L6_S3_FloatingIsland3:
	case eArea_L6_S3_FakeJames:
	case eArea_L6_S3_GlowingHeartHO:
	case eArea_L6_S3_PZPathway:
	case eArea_L6_S4_FloatingIsland4:
	case eArea_L6_S4_PortalChest:
	case eArea_L6_S4_PortalPuzzle:
	case eArea_L6_S5_MarysDream:
	case eArea_L6_S5_TabMiniGame:

		//Level 7
	case eArea_L7_S1_Volcano:
	case eArea_L7_S1_PZJigsawYellow:
	case eArea_L7_S1_PZStarPuzzle:
	case eArea_L7_S2_LasyaPrison:
	case eArea_L7_S2_HORightHand:
	case eArea_L7_S2_HOLeftHand:
	case eArea_L7_S2_HOBrokenRightHand:
	case eArea_L7_S2_HOBrokenLeftHand:
	case eArea_L7_S2_PZEyePuzzle:
	case eArea_L7_S2_PZLasyaStatue:
	case eArea_L7_S3_Haven:
	case eArea_L7_S3_PZDoorPuzzle:
	case eArea_L7_S3_PZJigsawRed:
	case eArea_L7_S4_Ameeha:
	case eArea_L7_S4_PZJigsawBlue:
	case eArea_L7_S5_SophiaEnd:

		//Level 8
	case eArea_L8_S1_ShukrasPlace:
	case eArea_L8_S1_PZDoorHandPuzzle:
	case eArea_L8_S1_DoorHO:
	case eArea_L8_S2_Garden:
	case eArea_L8_S2_PZGears:
	case eArea_L8_S2_PZGate:
	case eArea_L8_S2_PZSugarCane:
	case eArea_L8_S3_BanyanTree:
	case eArea_L8_S3_BanyanTreeHO:
	case eArea_L8_S4_Pool:
	case eArea_L8_S4_PZPoolGame:
	case eArea_L8_S5_VolcanoLand:
	case eArea_L8_S5_VirtualMiniGame:

		return true;

		//Non game rooms!!
	case eArea_KidsRoom:
	case eArea_ExtrasCutscenePlayer:
	case eArea_ExtrasRoom:
	case eArea_AwardsRoom:
	case eArea_L1_EndOfDemo:
	case eArea_Credits:  
		return false;
	}
	return false;
}

bool GameFlow::IsGamePuzzle( int eArea )
{
	switch( eArea )
	{
		//Level 1 - demo
	case eArea_L1_S2_PZFuse:
	case eArea_L1_S3_PZShelf:
	case eArea_L1_S4_PZPainting:
	case eArea_L2_S2_PZMango:
	case eArea_L2_S3_PZStoneBeam:
	case eArea_L2_S4_PZDeities:
	case eArea_L2_S5_PZEagleUnblock:
	case eArea_L2_S5_PZMedicine_Making:
	case eArea_L2_S6_PZTunnelClock:
	case eArea_L2_S7_PZSoldiers:
	case eArea_L4_S2_PZAlligatorHead:
	case eArea_L4_S2_PZDoor:
	case eArea_L4_S3_PZFireMaking:
	case eArea_L4_S3_PZDoor:
	case eArea_L4_S5_PZSwapTile:
		return true;
	}
	return false;
}

void GameFlow::InitCrossFade()
{
	Cursor::Show(false);
	if( PrevSceneGraphic != NULL )
	{ delete PrevSceneGraphic; PrevSceneGraphic = NULL; }
	PrevSceneGraphic = KPTK::createKGraphic(); 
	PrevSceneGraphic->setTextureQuality(true);
	GFApp->ForceRenderScnHudtoBB();//render new scene to Backbuffer			
	IsCrossFadeTransSet = PrevSceneGraphic->grabBackBuffer(NULL);
}

void GameFlow::InitCurrentSceneGraphic()
{
	Cursor::Show(false);
	if( CurrentSceneGraphic != NULL )
	{ delete CurrentSceneGraphic; CurrentSceneGraphic = NULL; }
	CurrentSceneGraphic = KPTK::createKGraphic(); 
	CurrentSceneGraphic->setTextureQuality(true);	
	//GFApp->ForceRenderScnHudtoBB();//render new scene to Backbuffer			
	//CurrentSceneGraphic->grabBackBuffer(NULL);
	Cursor::Show(true);
}

CObject * GameFlow::UpdateBgGraphic(CHOScene * scene)
{
    if(GFInstance->IsScnRTCreated)
    {
        //GFInstance->CurrentSceneGraphic->clearRenderTarget();
        CurrentSceneGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS
        IsRenderingToRT = true;
    }
        if(GFInstance->IsScnBg1024Res)
            scene->RenderToTarget(-171.0f);
        else
            scene->RenderToTarget();
    if(GFInstance->IsScnRTCreated)
    {
        IsRenderingToRT = false;
        CurrentSceneGraphic->endRenderToTarget();
    }
        //Make Active
        RT_BgScn->pSprite->Image = CurrentSceneGraphic;
    return RT_BgScn;
}



void GameFlow::RunCrossFade()
{
	if( IsCrossFadeTransSet )
	{
		if( NewSceneGraphic != NULL )
		{ delete NewSceneGraphic; NewSceneGraphic = NULL; }
		NewSceneGraphic = KPTK::createKGraphic(); 
		NewSceneGraphic->setTextureQuality(false);
		GFApp->ForceRenderScnHudtoBB();//render new scene to Backbuffer
		IsCrossFadeTransSet = NewSceneGraphic->grabBackBuffer(NULL);	
		if( IsCrossFadeTransSet )
		{
			Control::MasterTmr->ClearCycles();
			float time = 1.0f;
			float Width = PrevSceneGraphic->getWidth();
			float Height = PrevSceneGraphic->getHeight();

			while( time > 0.0f )
			{
				Control::MasterTmr->Update();
				time -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
				if( time < 0.0f )
					time = 0.0f;
				//No projection needed for grabbed BackBuffers!! 
				GFApp->Window->setWorldView(0.0f, 0.0f, 0.0f, 1.0f, true);//clear!!
				PrevSceneGraphic->blitAlphaRectFx(0,0,Width,Height,0,0,0,1.0f,time);
				NewSceneGraphic->blitAlphaRectFx(0,0,Width,Height,0,0,0,1.0f,(1.0f-time));
				GFApp->Window->flipBackBuffer();//no correction needed
			}
			IsCrossFadeTransSet = false;
		}
		if( NewSceneGraphic != NULL )
		{ delete NewSceneGraphic; NewSceneGraphic = NULL; }
		if( PrevSceneGraphic != NULL )
		{ delete PrevSceneGraphic; PrevSceneGraphic = NULL; }
	}
	Cursor::Show(true);
}


bool GameFlow::RemoveControl(int id, int value)
{
	Application::SetSingleUpdate(true);
	bool IsCrossFadingTrans = false;

	if( Flow == fHOLevel )
	{
		if( value == HOSM_ReturnToMenu )
			IsInsideLevel = false;
			//cross transition ony needed inside level
		else if( value == HOSM_NewArea )//&& IsInsideLevel )
			IsCrossFadingTrans = true;//cross fading between all scenes!!
	}

	#ifdef TOUCH_UI
		
		if( Flow == fHOLevel )
		{
			if( value == HOSM_ReturnToMenu )
				Cursor::SetInMenu(true); // dont show cursor in menus
			else if( value == HOSM_NewArea )//&& IsInsideLevel )
				Cursor::SetInMenu(false); // only show cursor inside the game
		}
	#endif
	Cursor::Show(false);

	CursorInvBoxOffset();//Avoid invbox activation during loading time cursor pos!!

	if( IsCrossFadingTrans )
	{
		if( PrevSceneGraphic != NULL )
		{ delete PrevSceneGraphic; PrevSceneGraphic = NULL; }
		PrevSceneGraphic = KPTK::createKGraphic();
        #ifdef K_ANDROID
                PrevSceneGraphic->setTextureQuality(false);
        #else
                PrevSceneGraphic->setTextureQuality(true);
        #endif
		GFApp->ForceRenderScnHudtoBB();//render new scene to Backbuffer
		IsCrossFadingTrans = PrevSceneGraphic->grabBackBuffer(NULL);
	}

	LoadingFile = 0;

	Cursor::SetMode(CA_Normal);
	if( !Control::RemoveControl(id, value) )
	{
		Cursor::Show(true);
		Application::SetSingleUpdate(false);
		Control::MasterTmr->ClearCycles();
		return false;
	}

	if( strikeGlowFX )
		strikeGlowFX->Stop();//Else it will be continued to next scene!!

	IsActivePopup = false;
	IsInGameMenuPopup = false;
	if( GreyAlphaImage )
		GreyAlphaImage->Show(false);

	nPostUpdates = 0;
	switch( Flow )
	{
		case fSplash:
			/*if( ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed )
            {
            Flow = fRootMenu;
            AddControl(new MenuSystem());
            }
            else*/
		{
			Flow = fHaveYouCutscene;
			AddControl(new HaveYouCutScene());
		}
			break;

		case fHaveYouCutscene:
			Flow = fRootMenu;
			#ifdef TOUCH_UI
				Cursor::SetInMenu(true);
			#endif
			AddControl(new MenuSystem());
			break;

		case fPromoScene:
			break;
		case fRootMenu:
		{
			#ifdef TOUCH_UI
				Cursor::SetInMenu(true);
			#endif
			if( IsWatchingPromo )
			{
				CPoint pos = CPoint(898,712,0);
				Cursor::SetPos(&pos);
				KInput::mousePointerTo((long)pos.x,(long)pos.y);//viewer non-disturbing point!!

				Flow = fPromoScene;
				AddControl(new PromoScene());
				ProfileSystem::Instance->ActiveProfile._PrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
				Application::SetSingleUpdate(false);
				Control::MasterTmr->ClearCycles();
				return true;

			}
			else if( ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromMainMenu || ProfileSystem::Instance->ActiveProfile.IsExtrasRoomFromMainMenu || IsCutsceneVideoPlayerActive || ProfileSystem::Instance->ActiveProfile.IsCreditsFromMainMenu)
			{
				if( ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_L1_S1_Beach && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_Credits )
				{
					AddNextArea();
				}
				else
				{
					ReturnToMenu();
					if( GFMainMenuCtrl != NULL )
					{
						if (ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed)
							GFMainMenuCtrl->ShowBannerText("Excreate");
						else
						{
							switch (ProfileSystem::Instance->ActiveProfile.CurrentLevel)
								{
									case Level_1: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_Beach; break;
									case Level_2: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance; break;
									case Level_3: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance; break;
									case Level_4: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance; break;
									case Level_5: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S1_TigerDistant; break;
									case Level_6: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1; break;
									case Level_7: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano; break;
									case Level_8: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace; break;

									default:
									break;
								}
							//GFMainMenuCtrl->ShowBannerText("Excorrupt");
						}
					}
				}
				break;
			}
			else if(ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromExtras)
			{
				if( ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_L1_S1_Beach && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_Credits )
				{
					AddNextArea();
				}
				else
				{
					ReturnToMenu();
					if( GFMainMenuCtrl != NULL )
					{
						if (ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed)
							GFMainMenuCtrl->ShowBannerText("Excreate");
						else
						{
							switch (ProfileSystem::Instance->ActiveProfile.CurrentLevel)
								{
									case Level_1: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_Beach; break;
									case Level_2: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance; break;
									case Level_3: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance; break;
									case Level_4: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance; break;
									case Level_5: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S1_TigerDistant; break;
									case Level_6: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1; break;
									case Level_7: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano; break;
									case Level_8: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace; break;

									default:
									break;
								}
							//GFMainMenuCtrl->ShowBannerText("Excorrupt");
						}
					}
				}
				break;
			}
			else if( !ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed )
			{
				CPoint pos = CPoint(898,712,0);
				Cursor::SetPos(&pos);
				KInput::mousePointerTo((long)pos.x,(long)pos.y);//viewer non-disturbing point!!

				Flow = fStartCutscene;
				///////////////////////////////////////////////////////////////////////////

				AddControl(new StartCutsceneSeries());

				///////////////////////////////////////////////////////////////////////////
				/* Original cursor turned off till end - as we are showing a dupicate cursor for cinematic */
				ProfileSystem::Instance->ActiveProfile._PrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
				Application::SetSingleUpdate(false);
				Control::MasterTmr->ClearCycles();
				return true;
			}
			else //Discard since it's very rare a gamer creates a new profile again - clear unused memory!!
			{
				UnloadStartCutscene();
			}
			//skip cutscene and goto next!! - no BREAK!!
		}

	case fStartCutscene:
		//HERE WE SHOULD PRELOAD!!		
		if( ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_L1_S1_Beach && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_Credits )
		{
			#ifdef TOUCH_UI
				Cursor::SetInMenu(false);
			#endif
			AddNextArea();	
		}
		else 
		{
			#ifdef TOUCH_UI
					Cursor::SetInMenu(true);
			#endif
			ReturnToMenu();	
			if( GFMainMenuCtrl != NULL )
			{
				if (ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed) 
					GFMainMenuCtrl->ShowBannerText("Excreate");		
				else
				{
					switch (ProfileSystem::Instance->ActiveProfile.CurrentLevel)
					{
						case Level_1: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_Beach; break;
						case Level_2: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance; break;
						case Level_3: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance; break;
						case Level_4: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance; break;
						case Level_5: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S1_TigerDistant; break;
						case Level_6: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1; break;
						case Level_7: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano; break;
						case Level_8: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace; break;

						default:
						break;
					}
					//GFMainMenuCtrl->ShowBannerText("Excorrupt");
				}
			}
		}		
		break;


	case fHOLevel:
		if( value == HOSM_ReturnToMenu )
		{				
			#ifdef TOUCH_UI
				Cursor::SetInMenu(true);
			#endif
			ReturnToMenu();	
		}
		else //if( value == HOSM_NewArea )
		{	

			if( ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_L1_S1_Beach && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_Credits )
			{
				#ifdef TOUCH_UI
					Cursor::SetInMenu(false);
				#endif
				AddNextArea();	
			}
			else 
			{
				#ifdef TOUCH_UI
					Cursor::SetInMenu(true);
				#endif
				ReturnToMenu();	
				if( GFMainMenuCtrl != NULL )
				{
					if (ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed) 
						GFMainMenuCtrl->ShowBannerText("Excreate");		
					else
					{
						switch (ProfileSystem::Instance->ActiveProfile.CurrentLevel)
						{
							case Level_1: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_Beach; break;
							case Level_2: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance; break;
							case Level_3: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance; break;
							case Level_4: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance; break;
							case Level_5: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S1_TigerDistant; break;
							case Level_6: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1; break;
							case Level_7: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano; break;
							case Level_8: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace; break;

							default:
							break;
						}
						//GFMainMenuCtrl->ShowBannerText("Excorrupt");
					}
				}
			}			
		}
		break;
	}


	if( nPostUpdates )
		nPostUpdates = 0;
	else if( IsCrossFadingTrans )
	{

		#ifdef TOUCH_UI
		/*
			// Samsung Tab 10 has some problem with backbuffer cross fading, so skip it and show only black screen
			if(GFApp->deviceName != "SM-T595") 
			{

				if( NewSceneGraphic != NULL )
					{ delete NewSceneGraphic; NewSceneGraphic = NULL; }
				NewSceneGraphic = KPTK::createKGraphic();
				NewSceneGraphic->setTextureQuality(false);
				bool tempSuccess = false;
				IsCrossFadingTrans = false;
				

				unsigned int *lpBackBufferPixels,*lpNextBackBufferPixels;
				long i, j, gnBWidth,gnBHeight,gnNWidth, gnNHeight;

				lpBackBufferPixels = overriddenGetBackBuffer(&gnBWidth,&gnBHeight);
			
				//	K_LOG("Backbuffer  = width %d, Height %d",gnBWidth,gnBHeight);
				GFApp->ForceRenderScnHudtoBB();
				lpNextBackBufferPixels = overriddenGetBackBuffer(&gnNWidth,&gnNHeight);
				//		K_LOG("NextBackbuffer  = width %d, Height %d",gnNWidth,gnNHeight);
				if(lpBackBufferPixels !=NULL)
					{
						IsCrossFadingTrans = PrevSceneGraphic->makePictureFromArray(lpBackBufferPixels,gnBWidth,gnBHeight,false,false);
					}
				if(lpNextBackBufferPixels !=NULL)
					{
						IsCrossFadingTrans = NewSceneGraphic->makePictureFromArray(lpNextBackBufferPixels,gnNWidth,gnNHeight,false,false);
					}
				if( IsCrossFadingTrans )
					{
						Control::MasterTmr->ClearCycles();
						float Width = PrevSceneGraphic->getWidth();
						float Height = PrevSceneGraphic->getHeight();
					//		K_LOG("Screen Transition - Width %f, height %f",Width,Height);
						float time = 1.0f;
						while( time > 0.0f )
						{
							Control::MasterTmr->Update();
							time -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
							if( time < 0.0f )
								time = 0.0f;

								if(GFApp->Instance->DesktopWidth == GFApp->Window->getActualWindowWidth()) // possible hack for streched drawing of screen transition
								{
									GFApp->Window->setWorldView(0.0f, 0.0f, 0.0f, 1.0f, false);//no clear!!
									PrevSceneGraphic->blitAlphaRectFx(0,0,Width,Height,0,0,0,1.0f,1.0f);
									NewSceneGraphic->blitAlphaRectFx(0,0,Width,Height,0,0,0,1.0f,(1.0f-time));
								}
								else
								{
									GFApp->Window->setWorldView (GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, false);//no clear!!
									PrevSceneGraphic->stretchAlphaRect(0,0,Width,Height,0,0,MAX_DISPLAY_WIDTH,MAX_DISPLAY_HEIGHT,1.0f);
									NewSceneGraphic->stretchAlphaRect(0,0,Width,Height,0,0,MAX_DISPLAY_WIDTH,MAX_DISPLAY_HEIGHT,(1.0f-time));
								}
							GFApp->Window->flipBackBuffer();//no correction needed
						}
					}
				else
					{
						GFApp->Window->setWorldView (GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, false);
						lpLoadingScnBG->drawRect(0,0,MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT,0,0,0,1);
						GFApp->Window->flipBackBuffer();//no correction needed
					}
			}
			else // if it is Samsung tab 10 then just show black screen when transition, until we figure out why the problem occurs
			{
				GFApp->Window->setWorldView (GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, false);
				lpLoadingScnBG->drawRect(0,0,MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT,0,0,0,1);
				GFApp->Window->flipBackBuffer();//no correction needed
			}
		*/
			GFApp->Window->setWorldView (GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, false);
			lpLoadingScnBG->drawRect(0,0,MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT,0,0,0,1);
			GFApp->Window->flipBackBuffer();//no correction needed

			//	overriddenSaveBackBuffer(std::string(GamePath+"prevBuffer.bmp").c_str());
				/*
				if( NewSceneGraphic != NULL )
				{ delete NewSceneGraphic; NewSceneGraphic = NULL; }
				NewSceneGraphic = KPTK::createKGraphic();
				NewSceneGraphic->setTextureQuality(false);

				GFApp->Window->saveBackBuffer(std::string(GamePath+"prevBuffer.bmp").c_str());
				GFApp->ForceRenderScnHudtoBB();//render new created scene to Backbuffer
				GFApp->Window->saveBackBuffer(std::string(GamePath+"nextBuffer.bmp").c_str());

				PrevSceneGraphic->loadPicture (KMiscTools::makeFilePath (std::string(GamePath+"prevBuffer.bmp").c_str()), false, false);
				IsCrossFadingTrans = NewSceneGraphic->loadPicture (KMiscTools::makeFilePath (std::string(GamePath+"nextBuffer.bmp").c_str()), false, false);

				if( IsCrossFadingTrans )
				{
					Control::MasterTmr->ClearCycles();
					float Width = PrevSceneGraphic->getWidth();
					float Height = PrevSceneGraphic->getHeight();
					float time = 1.0f;

					while( time > 0.0f )
					{
						Control::MasterTmr->Update();
						time -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
						if( time < 0.0f )
							time = 0.0f;

						//No projection needed for grabbed BackBuffers!!
						GFApp->Window->setWorldView(0.0f, 0.0f, 0.0f, 1.0f, false);//no clear!!

						PrevSceneGraphic->stretchAlphaRect(0,0,Width,Height,0,0,Width,Height,1.0f);
						NewSceneGraphic->stretchAlphaRect(0,0,Width,Height,0,0,Width,Height,(1.0f-time));

						GFApp->Window->flipBackBuffer();//no correction needed
					}
				}
				*/

					//float time = 1.0f;
					//int index = 0;
					//int Width = (int)lpLoadingSpr[0]->getWidth(), Height = (int)lpLoadingSpr[0]->getHeight();
					//while (time > 0.0f)
					//{
					//	Control::MasterTmr->Update();
				//		time -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
				//		if (time < 0.0f)
				//			time = 0.0f;

						//if(index > MAX_LD_FRAMES)
							//index = 0;
					// clean the screen with Black, so there is no artifact when loading the next scene.
				//		GFApp->Window->setWorldView (GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, false);

						//Draw Blackscreen
			//			lpLoadingScnBG->drawRect(0,0,MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT,0,0,0,1);

						//Draw Loading
						//lpLoadingScnBG->stretchAlphaRect(0, 0, 1, 1, 0, 0, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, 1.0f);
						//if(index != 30)
							//lpLoadingSpr[index]->blitAlphaRect(0, 0, Width, Height, (MAX_DISPLAY_WIDTH/2)-(Width/2), (MAX_DISPLAY_HEIGHT/2)-(Height/2));
						//index++;

					//	GFApp->Window->flipBackBuffer();//no correction needed
				//	}
		#else
			if( NewSceneGraphic != NULL )
			{ delete NewSceneGraphic; NewSceneGraphic = NULL; }
			NewSceneGraphic = KPTK::createKGraphic();
			NewSceneGraphic->setTextureQuality(true);

			GFApp->ForceRenderScnHudtoBB();//render new created scene to Backbuffer
			IsCrossFadingTrans = NewSceneGraphic->grabBackBuffer(NULL);
			if( IsCrossFadingTrans )
			{
				Control::MasterTmr->ClearCycles();
				float Width = PrevSceneGraphic->getWidth();
				float Height = PrevSceneGraphic->getHeight();
				float time = 1.0f;

				while( time > 0.0f )
				{
					Control::MasterTmr->Update();
					time -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
					if( time < 0.0f )
						time = 0.0f;
					//No projection needed for grabbed BackBuffers!!
					GFApp->Window->setWorldView(0.0f, 0.0f, 0.0f, 1.0f, false);//no clear!!

					PrevSceneGraphic->blitAlphaRectFx(0,0,Width,Height,0,0,0,1.0f,1.0f);
					NewSceneGraphic->blitAlphaRectFx(0,0,Width,Height,0,0,0,1.0f,(1.0f-time));

					GFApp->Window->flipBackBuffer();//no correction needed
				}
			}
		#endif
	}

	if( NewSceneGraphic != NULL)
	{ delete NewSceneGraphic; NewSceneGraphic = NULL; }
	if( PrevSceneGraphic != NULL )
	{ delete PrevSceneGraphic; PrevSceneGraphic = NULL; }

	CursorInvBoxOffset();

	#ifndef _RELEASEBUILD //debug
		{
			Cursor::Show(true);
		}
	#else
		{
			if( Flow != fRootMenu && Flow != fHaveYouCutscene )//For root menu it will turn on after mash done!!
				Cursor::Show(true);
			else if( ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_L1_EndOfDemo)
				Cursor::Show(true);
		}
	#endif

	/* if( Control::Audio->IsPlaying(aSFXLoader) )
	Control::Audio->StopSample(aSFXLoader);*/

	ProfileSystem::Instance->ActiveProfile._PrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;

	if( Flow == fRootMenu )
		IsInsideLevel = false;
	else if( PrevFlow == fHOLevel && Flow == fHOLevel )
		IsInsideLevel = true;//cross transition and similar effects!!
	else //if( Flow == fRootMenu )
		IsInsideLevel = false;


	PrevFlow = Flow;

	Application::SetSingleUpdate(false);
	Control::MasterTmr->ClearCycles();
	return true;
}

bool GameFlow::IsHOPuzzleScene( int eArea )
{
	switch( eArea )
	{
	default: return false;		

		//Level 1
	case eArea_L1_S2_PZFuse: if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsP1LeftFuseFixed || !ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseFixed )
								 return false;
							 else //afterwards full in puzzle mode 
								 return true;
	case eArea_L1_S3_PZShelf:	return true;
	case eArea_L1_S4_PZDoor:	if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsShadowPuzzleSolved) 
									return true;
								else
									return false;

	case eArea_L1_S4_PZPainting:  if( !ProfileSystem::Instance->ActiveProfile.L1S4_PaintingPuzzleSolved)
									  return true; //full puzzle mode!!
								  else //painting puzzle fixed, but 4 coins or glyph code not fixed
								  {
									  return false;//not yet in puzzle mode
								  }
	case eArea_L1_S5_HOTreasureRoom : return true;
		//Level 2 - Check the conditions for puzzle mode - No need since already checking if scene is in puzzle mode
	case eArea_L2_S1_UndergroundHO:
	case eArea_L2_S2_HONagaTemple:
	case eArea_L2_S2_PZMango:
	case eArea_L2_S4_PZDeities:
	case eArea_L2_S5_PZEagleUnblock:
	case eArea_L2_S3_PZStoneBeam: return true;

	case eArea_L2_S6_PZTunnelClock: 
		if( ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed && ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced )
			return true;
		else
			return false;

	case eArea_L3_S3_PZUnderwater: 
		if( ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced )
			return true;
		else
			return false;
	case eArea_L4_S2_PZDoor:
	case eArea_L4_S2_PZAlligatorHead:
		{
			if (!ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved)
			{
				return true;
			}

			return false;
		}
	case eArea_L4_S3_PZFireMaking:
	case eArea_L4_S3_PZDoor:
	case eArea_L4_S5_PZSwapTile:
		return true;

	}
}

void GameFlow::CursorInvBoxOffset()
{
	int CurY = KInput::getMouseY();
	int InvBoxY = GFApp->CurrentResolution.Height-50;//28+12 correction!!
	if( CurY > InvBoxY )//If cursor over invbox then shift slightly up!!
	{
		float CurX = Cursor::GetX();
		Cursor::SetXY(CurX,(float)InvBoxY);
		KInput::mousePointerTo((long)CurX,(long)InvBoxY);//avoid invbox autoopen at init from menu - or prevscene_exit_clk!!
	}
}

void GameFlow::AddNextArea()
{
	//K_LOG("AddNextArea area %d",ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Flow = fHOLevel;

	int testCurrentArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
	// save only if the game is inside the playable levels and not in main areas
	
		if(testCurrentArea != eArea_ExtrasCutscenePlayer || testCurrentArea != eArea_ExtrasRoom || testCurrentArea != eArea_Credits ) 
		{
			if( ProfileSystem::Instance->ProfileSlot >= 0 && ProfileSystem::Instance->ProfileSlot < MAX_PROFILES )
			{
				ProfileSystem::Instance->SaveProfile(ProfileSystem::Instance->ProfileSlot);			
				ProfileSystem::Instance->WriteProfileSystemData();
				K_LOG("Saved profile...  going into adding room");
				if(testCurrentArea != eArea_KidsRoom || testCurrentArea != eArea_AwardsRoom)
					GFApp->RefreshInvItems = true;
			}
		}
	

	#if defined(K_ANDROID) && !defined(_DEMOBUILD)
		if (GFApp->isUnlocked()) //If Full Build
		{
			switch(ProfileSystem::Instance->ActiveProfile.CurrentArea)
			{
				//heavy loadcase!!
				//Large scenes			

				//Level 1 - demo
				#ifdef _LEVEL1_ENABLED
					case eArea_L1_S1_Beach:
						ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_1;
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S1_Beach() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S1_Beach,8); 
						break;
					case eArea_L1_S1_PZKidsPainting:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S1_PZKidsPainting() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S1_PZKidsPainting,6);
						break;
					case eArea_L1_S2_Frontyard:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S2_Frontyard() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S2_Frontyard,5);
						break;

					case eArea_L1_S3_Foyer:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S3_Foyer() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S3_Foyer,6);
						break;
					case eArea_L1_S2_PZFuse:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S2_PZFuse() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S2_PZFuse,2);
						break;

					case eArea_L1_S3_PZShelf:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S3_PZShelf());
						}
						else
							PreloadLoadLevelArea(eArea_L1_S3_PZShelf,1);
						break;

					case eArea_L1_S4_PZDoor:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S4_PZDoor() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S4_PZDoor,3);
						break;

					case eArea_L1_S4_Bedroom:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S4_Bedroom() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S4_Bedroom,3);
						break;

					case eArea_L1_S4_PZPainting:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S4_PZPainting() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S4_PZPainting,2);
						break;

					case eArea_L1_S3_DoorHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S3_DoorHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S3_DoorHO,3);
						break;

					case eArea_L1_S5_TreasureRoom:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S5_TreasureRoom());
						}
						else
							PreloadLoadLevelArea(eArea_L1_S5_TreasureRoom,3);
						break;

					case eArea_L1_S5_TunnelDoor:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S5_TunnelDoor());
						}
						else
							PreloadLoadLevelArea(eArea_L1_S5_TunnelDoor,3);
						break;
					case eArea_L1_S5_PZDynamiteMaking:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S5_PZDynamiteMaking() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S5_PZDynamiteMaking,3);
						break;

					case eArea_L1_S5_HOTreasureRoom:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S5_HOTreasureRoom() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S5_HOTreasureRoom,3);
						break;
					case eArea_L1_S6_HOBoatShed:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S6_HOBoatShed() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S6_HOBoatShed,3);
						break;
					case eArea_L1_S7_BoatDock:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S7_Boatdock() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S7_BoatDock,3);
						break;

					case eArea_L1_S6_BoatShed:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S6_Boatshed() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S6_BoatShed,3);
						break;


					case eArea_L1_EndOfDemo:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new EndOfDemoScene() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_EndOfDemo,3);
						break;  
				#endif // _LEVEL1_ENABLED

				case eArea_KidsRoom:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new KidsRoom() );
					}
					else
						PreloadLoadLevelArea(eArea_KidsRoom,3);
					break;

				case eArea_ExtrasCutscenePlayer:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new ExtrasCutscenePlayer() );
					}
					else
						PreloadLoadLevelArea(eArea_ExtrasCutscenePlayer,3);
					break;

				case eArea_ExtrasRoom:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new ExtrasRoom() );
					}
					else
						PreloadLoadLevelArea(eArea_ExtrasRoom,3);
					break;

				case eArea_AwardsRoom:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new AwardsRoom() );
					}
					else
						PreloadLoadLevelArea(eArea_AwardsRoom,3);
					break;

				case eArea_Credits:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new Credits());
					}
					else
						PreloadLoadLevelArea(eArea_Credits,7);
					break;
						// Level 2
					#ifdef _LEVEL2_ENABLED
						case eArea_L2_S1_ForestEntrance:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S1_ForestEntrance() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S1_ForestEntrance,11);
							break;
						case eArea_L2_S1_UndergroundHO:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S1_UndergroundHO() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S1_UndergroundHO,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S2_NagaTemple:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S2_NagaTemple() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S2_NagaTemple,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S2_PZMango:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S2_PZMango() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S2_PZMango,1);//todo_LoaderUpdates
							break;
						case eArea_L2_S2_HONagaTemple:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S2_NagaTempleHO() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S2_HONagaTemple,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S3_CaveMouth:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S3_CaveMouth() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S3_CaveMouth,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S5_PZMedicine_Making:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S5_PZMedicineMaking());
							}
							else
								PreloadLoadLevelArea(eArea_L2_S5_PZMedicine_Making,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S3_PZStoneBeam:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S3_PZStoneBeam() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S3_PZStoneBeam,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S4_Cave:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S4_Cave() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S4_Cave,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S4_PZDeities:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S4_DeitiesPuzzle());
							}
							else
								PreloadLoadLevelArea(eArea_L2_S4_PZDeities,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S5_MoopansDen:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S5_MoopansDen() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S5_MoopansDen,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S5_PZEagleUnblock:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S5_EagleBlockPuzzle() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S5_PZEagleUnblock,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S6_PZTunnelClock:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S6_PZTunnelClock() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S6_PZTunnelClock,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S6_TunnelEnd:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S6_TunnelEnd() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S6_TunnelEnd,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S6_HOWaterfall:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2_S6_HOWaterfall() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S6_HOWaterfall,1);//todo_LoaderUpdates
							break;

						case eArea_L2_S7_PZSoldiers:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L2S7SoldiersPuzzle() );
							}
							else
								PreloadLoadLevelArea(eArea_L2_S7_PZSoldiers,1);//todo_LoaderUpdates
							break;  
					#endif // _LEVEL2_ENABLED

							//Level 3
					#ifdef _LEVEL3_ENABLED
						case eArea_L3_S1_FortEntrance:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S1_FortEntrance() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S1_FortEntrance,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S1_PZLionHead:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S1_PZLionHead());
							}
							else
								PreloadLoadLevelArea(eArea_L3_S1_PZLionHead,1);//todo_LoaderUpdates
							break;



						case eArea_L3_S1_HOLadder:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S1_HOLadder() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S1_HOLadder,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S2_FortInterior:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S2_FortInterior() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S2_FortInterior,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S2_P1Room1Start:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S2_P1Room1Start() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S2_P1Room1Start,1);//todo_LoaderUpdates
							break;	

						case eArea_L3_S2_P1Room2Wine:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S2_P1Room2Wine() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S2_P1Room2Wine,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S2_P1Room3Lock:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S2_P1Room3Lock() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S2_P1Room3Lock,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S2_P1Room4Match3:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S2_P1Room4Match3() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S2_P1Room4Match3,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S2_P1Room5Pipe:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S2_P1Room5Pipe() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S2_P1Room5Pipe,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S2_P1Room6Pillar:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S2_P1Room6Pillar() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S2_P1Room6Pillar,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S2_P1DeadEnd:
							{
								if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
								{
									nPostUpdates = 0;
									AddControl( new L3_S2_MazeDeadEnd() );
								}
								else
									PreloadLoadLevelArea(eArea_L3_S2_P1DeadEnd,1);//todo_LoaderUpdates
								break;
							}
							break;

						case eArea_L3_S3_Pool:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S3_Pool() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S3_Pool,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S3_PZBlocks:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S3_PZBlock() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S3_PZBlocks,1);//todo_LoaderUpdates
							break;
						case eArea_L3_S3_PZUnderwater:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S3_PZUnderwater() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S3_PZUnderwater,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S4_FortFountain:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S4_FortFountain() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S4_FortFountain,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S4_FortFountainHO:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S4FortFoundationHO() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S4_FortFountainHO,1);//todo_LoaderUpdates
							break;

						case eArea_L3_S5_Cliff:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S5_Cliff() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S5_Cliff,1);//todo_LoaderUpdates
							break;  
					#endif // _LEVEL3_ENABLED

							//level 4
					#ifdef _LEVEL4_ENABLED
						case eArea_L4_S1_SwampEntrance:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S1_SwampEntrance() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S1_SwampEntrance,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S1_ShoeHO:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S1_ShoeHO() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S1_ShoeHO,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S2_RiverSide:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S2_RiverSide() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S2_RiverSide,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S2_PZAlligatorHead:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S2_AlligatorPuzzle() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S2_PZAlligatorHead,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S2_PZDoor:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;

								AddControl( new L4_S2_FanPuzzle() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S2_PZDoor,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S3_Cave:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S3_Cave() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S3_Cave,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S3_PZFireMaking:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S3_PZFireMaking() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S3_PZFireMaking,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S3_PZDoor:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S3_PZDoor() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S3_PZDoor,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S4_HillTop:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S4_HillTop() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S4_HillTop,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S4_BinocularHO:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S4_BinocularHO() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S4_BinocularHO,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S5_HillSide:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S5_HillSide() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S5_HillSide,1);//todo_LoaderUpdates
							break;
						case eArea_L4_S5_PZSwapTile:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L4_S5_PZSwapTiles() );
							}
							else
								PreloadLoadLevelArea(eArea_L4_S5_PZSwapTile,1);//todo_LoaderUpdates
							break;  
					#endif // _LEVEL4_ENABLED

							//Level 5
					#ifdef _LEVEL5_ENABLED
						case eArea_L5_S1_TigerDistant:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S1_TigerDistant() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S1_TigerDistant,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S2_TigerCloseup:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S2_TigerCloseup() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S2_TigerCloseup,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S2_PZBlocks:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S2_PZBlocks() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S2_PZBlocks,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S3_Pond:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S3_Pond() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S3_Pond,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S3_PZDungeonDoor:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S3_PZDungeonDoor() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S3_PZDungeonDoor,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S3_HOUnderwater:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S3_HOUnderwater() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S3_HOUnderwater,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S4_Dungeon:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S4_Dungeon() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S4_Dungeon,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S4_PZHandCuff:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S4_PZHandCuff() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S4_PZHandCuff,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S5_MountainBelow:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S5_MountainBelow() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S5_MountainBelow,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S5_HOGanapathi:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S5_HOGanapathi() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S5_HOGanapathi,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S5_PZSunClock:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S5_PZSunClock() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S5_PZSunClock,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S6_MountainTop:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S6_MountainTop() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S6_MountainTop,1);//todo_LoaderUpdates
							break;
						case eArea_L5_S6_HOTree:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L5_S6_HOTree() );
							}
							else
								PreloadLoadLevelArea(eArea_L5_S6_HOTree,1);//todo_LoaderUpdates
							break;  
					#endif // _LEVEL5_ENABLED

					#ifdef _LEVEL6_ENABLED
						case eArea_L6_S1_FloatingIsland1:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S1_FloatingIsland_1() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S1_FloatingIsland1,1);//todo_LoaderUpdates
							break;  
						case eArea_L6_S1_DescriptionHO:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S1_DescriptionHO() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S1_DescriptionHO,1);//todo_LoaderUpdates
							break;  
						case eArea_L6_S2_FloatingIsland2:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S2_FloatingIsland_2() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S2_FloatingIsland2,1);//todo_LoaderUpdates
							break; 		  
						case eArea_L6_S2_ShereHO:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S2_SphereHO() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S2_ShereHO,1);//todo_LoaderUpdates
							break; 
						case eArea_L6_S2_PZPineGum:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S2_PZPineGum() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S2_PZPineGum,1);//todo_LoaderUpdates
							break; 
						case eArea_L6_S3_FloatingIsland3:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S3_FloatingIsland_3() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S3_FloatingIsland3,1);//todo_LoaderUpdates
							break;
						case eArea_L6_S3_FakeJames:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S3_FakeJames() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S3_FakeJames,1);//todo_LoaderUpdates
							break;
						case eArea_L6_S3_GlowingHeartHO:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S3_GlowingHeartHO() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S3_GlowingHeartHO,1);//todo_LoaderUpdates
							break; 
						case eArea_L6_S3_PZPathway:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S3_PZPathway() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S3_PZPathway,1);//todo_LoaderUpdates
							break; 
						case eArea_L6_S4_FloatingIsland4:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S4_FloatingIsland_4() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S4_FloatingIsland4,1);//todo_LoaderUpdates
							break;  
						case eArea_L6_S4_PortalChest:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S4_PortalChest() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S4_PortalChest,1);//todo_LoaderUpdates
							break; 
						case eArea_L6_S4_PortalPuzzle:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S4_PortalPuzzle() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S4_PortalPuzzle,1);//todo_LoaderUpdates
							break; 
						case eArea_L6_S5_MarysDream:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6_S5_MarysDream() );
							}
							else
								PreloadLoadLevelArea(eArea_L6_S5_MarysDream,1);//todo_LoaderUpdates
							break; 
						case eArea_L6_S5_TabMiniGame:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L6S5TabMiniGame());
							}
							else
								PreloadLoadLevelArea(eArea_L6_S5_TabMiniGame,1);//todo_LoaderUpdates
							break;
					#endif // _LEVEL6_ENABLED

					#ifdef _LEVEL7_ENABLED
							//Level 7
						case eArea_L7_S1_Volcano:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S1_Volcano() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S1_Volcano,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S1_PZJigsawYellow:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S1_PZJigsawYellow() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S1_PZJigsawYellow,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S1_PZStarPuzzle:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S1_PZStarPuzzle() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S1_PZStarPuzzle,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S2_LasyaPrison:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S2_LasyaPrison() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S2_LasyaPrison,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S2_HORightHand:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S2_HORightHand() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S2_HORightHand,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S2_HOLeftHand:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S2_HOLeftHand() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S2_HOLeftHand,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S2_HOBrokenRightHand:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S2_HOBrokenRightHand() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S2_HOBrokenRightHand,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S2_HOBrokenLeftHand:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S2_HOBrokenLeftHand() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S2_HOBrokenLeftHand,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S2_PZEyePuzzle:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S2_PZEyePuzzle() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S2_PZEyePuzzle,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S2_PZLasyaStatue:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S2_PZLasyaStatue() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S2_PZLasyaStatue,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S3_Haven:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S3_Haven() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S3_Haven,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S3_PZDoorPuzzle:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S3_PZDoorPuzzle() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S3_PZDoorPuzzle,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S3_PZJigsawRed:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S3_PZJigsawRed() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S3_PZJigsawRed,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S4_Ameeha:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S4_Ameeha() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S4_Ameeha,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S4_PZJigsawBlue:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S4_PZJigsawBlue() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S4_PZJigsawBlue,1);//todo_LoaderUpdates
							break;
						case eArea_L7_S5_SophiaEnd:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L7_S5_SophiaEnd() );
							}
							else
								PreloadLoadLevelArea(eArea_L7_S5_SophiaEnd,1);//todo_LoaderUpdates
							break;
					#endif //

					#ifdef _LEVEL8_ENABLED
						case eArea_L8_S1_ShukrasPlace:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S1_ShukrasPlace());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S1_ShukrasPlace,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S1_PZDoorHandPuzzle:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S1_PZDoor());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S1_PZDoorHandPuzzle,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S1_DoorHO:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S1_DoorHO());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S1_DoorHO,1);//todo_LoaderUpdates
							break;
							break;
						case eArea_L8_S2_Garden:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S2_Garden());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S2_Garden,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S2_PZGears:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S2_GearPuzzle());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S2_PZGears,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S2_PZGate:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S2_PZGate());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S2_PZGate,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S2_PZSugarCane:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S2_PZSugarCane());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S2_PZSugarCane,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S3_BanyanTree:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S3_BanyanTree());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S3_BanyanTree,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S3_BanyanTreeHO:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S3_BanyanTreeHO());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S3_BanyanTreeHO,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S4_Pool:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S4_Pool());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S4_Pool,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S4_PZPoolGame:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S4_PZPoolGame());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S4_PZPoolGame,1);//todo_LoaderUpdates
							break;
						case eArea_L8_S5_VolcanoLand:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S5_VolcanoLand());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S5_VolcanoLand,1);//todo_LoaderUpdates
							break;

						case eArea_L8_S5_VirtualMiniGame:
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L8_S5_VirtualMiniGame());
							}
							else
								PreloadLoadLevelArea(eArea_L8_S5_VirtualMiniGame,1);//todo_LoaderUpdates
							break;
					#endif
				//Small scenes - Loader anim needed only if core not loaded -> 0 post updates
				//////////////////////////////
				default:
					std::string TmpStr = std::string("Er0x1601- Requested scene file does not exist = ") + LevelAreaNames[ProfileSystem::Instance->ActiveProfile.CurrentArea];
					#ifdef K_WIN32
						WARN( (const char*)(FileSystem::s2ws(TmpStr).c_str()) );//WIDESTR WARNINGS		
					#else
						WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
					#endif		
					// Something majorly wrong that there is no Level to load, go back to main menu gracefully
					Flow = fRootMenu;	
					#ifdef TOUCH_UI
						Cursor::SetInMenu(true);
					#endif		
					AddControl(new MenuSystem());
					/*
						#ifdef K_WIN32
							WARN((const char*)L"Requested scene file does not exist.");
						#else
							WARN((const char*)"Requested scene file does not exist.");
						#endif
					*/	
			}	
		}
		else
		{
			switch(ProfileSystem::Instance->ActiveProfile.CurrentArea)
			{
				//heavy loadcase!!
				//Large scenes			

				//Level 1 - demo
				#ifdef _LEVEL1_ENABLED
					case eArea_L1_S1_Beach:
						ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_1;
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S1_Beach() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S1_Beach,8); 
						break;
					case eArea_L1_S1_PZKidsPainting:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S1_PZKidsPainting() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S1_PZKidsPainting,6);
						break;
					case eArea_L1_S2_Frontyard:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S2_Frontyard() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S2_Frontyard,5);
						break;

					case eArea_L1_S3_Foyer:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S3_Foyer() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S3_Foyer,6);
						break;
					case eArea_L1_S2_PZFuse:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S2_PZFuse() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S2_PZFuse,2);
						break;

					case eArea_L1_S3_PZShelf:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S3_PZShelf());
						}
						else
							PreloadLoadLevelArea(eArea_L1_S3_PZShelf,1);
						break;

					case eArea_L1_S4_PZDoor:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S4_PZDoor() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S4_PZDoor,3);
						break;

					case eArea_L1_S4_Bedroom:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S4_Bedroom() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S4_Bedroom,3);
						break;

					case eArea_L1_S4_PZPainting:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S4_PZPainting() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S4_PZPainting,2);
						break;

					case eArea_L1_S3_DoorHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S3_DoorHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S3_DoorHO,3);
						break;

					case eArea_L1_S5_TreasureRoom:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S5_TreasureRoom());
						}
						else
							PreloadLoadLevelArea(eArea_L1_S5_TreasureRoom,3);
						break;

					case eArea_L1_S5_TunnelDoor:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S5_TunnelDoor());
						}
						else
							PreloadLoadLevelArea(eArea_L1_S5_TunnelDoor,3);
						break;
					case eArea_L1_S5_PZDynamiteMaking:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S5_PZDynamiteMaking() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S5_PZDynamiteMaking,3);
						break;

					case eArea_L1_S5_HOTreasureRoom:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S5_HOTreasureRoom() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S5_HOTreasureRoom,3);
						break;
					case eArea_L1_S6_HOBoatShed:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S6_HOBoatShed() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S6_HOBoatShed,3);
						break;
					case eArea_L1_S7_BoatDock:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S7_Boatdock() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S7_BoatDock,3);
						break;

					case eArea_L1_S6_BoatShed:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L1_S6_Boatshed() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_S6_BoatShed,3);
						break;


					case eArea_L1_EndOfDemo:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new EndOfDemoScene() );
						}
						else
							PreloadLoadLevelArea(eArea_L1_EndOfDemo,3);
						break;  
				#endif // _LEVEL1_ENABLED

				case eArea_KidsRoom:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new KidsRoom() );
					}
					else
						PreloadLoadLevelArea(eArea_KidsRoom,3);
					break;

				case eArea_ExtrasCutscenePlayer:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new ExtrasCutscenePlayer() );
					}
					else
						PreloadLoadLevelArea(eArea_ExtrasCutscenePlayer,3);
					break;

				case eArea_ExtrasRoom:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new ExtrasRoom() );
					}
					else
						PreloadLoadLevelArea(eArea_ExtrasRoom,3);
					break;

				case eArea_AwardsRoom:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new AwardsRoom() );
					}
					else
						PreloadLoadLevelArea(eArea_AwardsRoom,3);
					break;

				case eArea_Credits:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new Credits());
					}
					else
						PreloadLoadLevelArea(eArea_Credits,7);
					break;
				//Small scenes - Loader anim needed only if core not loaded -> 0 post updates
				//////////////////////////////
				default:
					std::string TmpStr = std::string("Er0x1601- Requested scene file does not exist in this Version, please buy the full game...");
					#ifdef K_WIN32
						WARN( (const char*)(FileSystem::s2ws(TmpStr).c_str()) );//WIDESTR WARNINGS		
					#else
						WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
					#endif		
					// Something majorly wrong that there is no Level to load, go back to main menu gracefully
					Flow = fRootMenu;
					#ifdef TOUCH_UI
						Cursor::SetInMenu(true);
					#endif		
					AddControl(new MenuSystem());
					/*
						#ifdef K_WIN32
							WARN((const char*)L"Requested scene file does not exist.");
						#else
							WARN((const char*)"Requested scene file does not exist.");
						#endif
					*/	
			}
		}
	#else
		switch(ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
			//heavy loadcase!!
			//Large scenes			

			//Level 1 - demo
			#ifdef _LEVEL1_ENABLED
				case eArea_L1_S1_Beach:
					ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_1;
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S1_Beach() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S1_Beach,8); 
					break;
				case eArea_L1_S1_PZKidsPainting:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S1_PZKidsPainting() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S1_PZKidsPainting,6);
					break;
				case eArea_L1_S2_Frontyard:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S2_Frontyard() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S2_Frontyard,5);
					break;

				case eArea_L1_S3_Foyer:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S3_Foyer() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S3_Foyer,6);
					break;
				case eArea_L1_S2_PZFuse:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S2_PZFuse() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S2_PZFuse,2);
					break;

				case eArea_L1_S3_PZShelf:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S3_PZShelf());
					}
					else
						PreloadLoadLevelArea(eArea_L1_S3_PZShelf,1);
					break;

				case eArea_L1_S4_PZDoor:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S4_PZDoor() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S4_PZDoor,3);
					break;

				case eArea_L1_S4_Bedroom:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S4_Bedroom() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S4_Bedroom,3);
					break;

				case eArea_L1_S4_PZPainting:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S4_PZPainting() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S4_PZPainting,2);
					break;

				case eArea_L1_S3_DoorHO:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S3_DoorHO() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S3_DoorHO,3);
					break;

				case eArea_L1_S5_TreasureRoom:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S5_TreasureRoom());
					}
					else
						PreloadLoadLevelArea(eArea_L1_S5_TreasureRoom,3);
					break;

				case eArea_L1_S5_TunnelDoor:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S5_TunnelDoor());
					}
					else
						PreloadLoadLevelArea(eArea_L1_S5_TunnelDoor,3);
					break;
				case eArea_L1_S5_PZDynamiteMaking:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S5_PZDynamiteMaking() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S5_PZDynamiteMaking,3);
					break;

				case eArea_L1_S5_HOTreasureRoom:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S5_HOTreasureRoom() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S5_HOTreasureRoom,3);
					break;
				case eArea_L1_S6_HOBoatShed:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S6_HOBoatShed() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S6_HOBoatShed,3);
					break;
				case eArea_L1_S7_BoatDock:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S7_Boatdock() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S7_BoatDock,3);
					break;

				case eArea_L1_S6_BoatShed:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new L1_S6_Boatshed() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_S6_BoatShed,3);
					break;


				case eArea_L1_EndOfDemo:
					if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
					{
						nPostUpdates = 0;
						AddControl( new EndOfDemoScene() );
					}
					else
						PreloadLoadLevelArea(eArea_L1_EndOfDemo,3);
					break;  
			#endif // _LEVEL1_ENABLED

			case eArea_KidsRoom:
				if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
				{
					nPostUpdates = 0;
					AddControl( new KidsRoom() );
				}
				else
					PreloadLoadLevelArea(eArea_KidsRoom,3);
				break;

			case eArea_ExtrasCutscenePlayer:
				if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
				{
					nPostUpdates = 0;
					AddControl( new ExtrasCutscenePlayer() );
				}
				else
					PreloadLoadLevelArea(eArea_ExtrasCutscenePlayer,3);
				break;

			case eArea_ExtrasRoom:
				if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
				{
					nPostUpdates = 0;
					AddControl( new ExtrasRoom() );
				}
				else
					PreloadLoadLevelArea(eArea_ExtrasRoom,3);
				break;

			case eArea_AwardsRoom:
				if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
				{
					nPostUpdates = 0;
					AddControl( new AwardsRoom() );
				}
				else
					PreloadLoadLevelArea(eArea_AwardsRoom,3);
				break;

			case eArea_Credits:
				if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
				{
					nPostUpdates = 0;
					AddControl( new Credits());
				}
				else
					PreloadLoadLevelArea(eArea_Credits,7);
				break;

			#ifndef _DEMOBUILD
					// Level 2
				#ifdef _LEVEL2_ENABLED
					case eArea_L2_S1_ForestEntrance:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S1_ForestEntrance() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S1_ForestEntrance,11);
						break;
					case eArea_L2_S1_UndergroundHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S1_UndergroundHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S1_UndergroundHO,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S2_NagaTemple:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S2_NagaTemple() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S2_NagaTemple,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S2_PZMango:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S2_PZMango() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S2_PZMango,1);//todo_LoaderUpdates
						break;
					case eArea_L2_S2_HONagaTemple:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S2_NagaTempleHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S2_HONagaTemple,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S3_CaveMouth:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S3_CaveMouth() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S3_CaveMouth,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S5_PZMedicine_Making:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S5_PZMedicineMaking());
						}
						else
							PreloadLoadLevelArea(eArea_L2_S5_PZMedicine_Making,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S3_PZStoneBeam:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S3_PZStoneBeam() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S3_PZStoneBeam,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S4_Cave:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S4_Cave() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S4_Cave,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S4_PZDeities:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S4_DeitiesPuzzle());
						}
						else
							PreloadLoadLevelArea(eArea_L2_S4_PZDeities,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S5_MoopansDen:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S5_MoopansDen() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S5_MoopansDen,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S5_PZEagleUnblock:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S5_EagleBlockPuzzle() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S5_PZEagleUnblock,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S6_PZTunnelClock:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S6_PZTunnelClock() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S6_PZTunnelClock,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S6_TunnelEnd:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S6_TunnelEnd() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S6_TunnelEnd,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S6_HOWaterfall:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2_S6_HOWaterfall() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S6_HOWaterfall,1);//todo_LoaderUpdates
						break;

					case eArea_L2_S7_PZSoldiers:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L2S7SoldiersPuzzle() );
						}
						else
							PreloadLoadLevelArea(eArea_L2_S7_PZSoldiers,1);//todo_LoaderUpdates
						break;  
				#endif // _LEVEL2_ENABLED

						//Level 3
				#ifdef _LEVEL3_ENABLED
					case eArea_L3_S1_FortEntrance:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S1_FortEntrance() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S1_FortEntrance,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S1_PZLionHead:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S1_PZLionHead());
						}
						else
							PreloadLoadLevelArea(eArea_L3_S1_PZLionHead,1);//todo_LoaderUpdates
						break;



					case eArea_L3_S1_HOLadder:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S1_HOLadder() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S1_HOLadder,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S2_FortInterior:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S2_FortInterior() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S2_FortInterior,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S2_P1Room1Start:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S2_P1Room1Start() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S2_P1Room1Start,1);//todo_LoaderUpdates
						break;	

					case eArea_L3_S2_P1Room2Wine:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S2_P1Room2Wine() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S2_P1Room2Wine,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S2_P1Room3Lock:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S2_P1Room3Lock() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S2_P1Room3Lock,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S2_P1Room4Match3:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S2_P1Room4Match3() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S2_P1Room4Match3,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S2_P1Room5Pipe:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S2_P1Room5Pipe() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S2_P1Room5Pipe,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S2_P1Room6Pillar:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S2_P1Room6Pillar() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S2_P1Room6Pillar,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S2_P1DeadEnd:
						{
							if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
							{
								nPostUpdates = 0;
								AddControl( new L3_S2_MazeDeadEnd() );
							}
							else
								PreloadLoadLevelArea(eArea_L3_S2_P1DeadEnd,1);//todo_LoaderUpdates
							break;
						}
						break;

					case eArea_L3_S3_Pool:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S3_Pool() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S3_Pool,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S3_PZBlocks:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S3_PZBlock() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S3_PZBlocks,1);//todo_LoaderUpdates
						break;
					case eArea_L3_S3_PZUnderwater:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S3_PZUnderwater() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S3_PZUnderwater,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S4_FortFountain:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S4_FortFountain() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S4_FortFountain,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S4_FortFountainHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S4FortFoundationHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S4_FortFountainHO,1);//todo_LoaderUpdates
						break;

					case eArea_L3_S5_Cliff:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L3_S5_Cliff() );
						}
						else
							PreloadLoadLevelArea(eArea_L3_S5_Cliff,1);//todo_LoaderUpdates
						break;  
				#endif // _LEVEL3_ENABLED

						//level 4
				#ifdef _LEVEL4_ENABLED
					case eArea_L4_S1_SwampEntrance:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S1_SwampEntrance() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S1_SwampEntrance,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S1_ShoeHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S1_ShoeHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S1_ShoeHO,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S2_RiverSide:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S2_RiverSide() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S2_RiverSide,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S2_PZAlligatorHead:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S2_AlligatorPuzzle() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S2_PZAlligatorHead,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S2_PZDoor:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;

							AddControl( new L4_S2_FanPuzzle() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S2_PZDoor,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S3_Cave:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S3_Cave() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S3_Cave,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S3_PZFireMaking:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S3_PZFireMaking() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S3_PZFireMaking,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S3_PZDoor:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S3_PZDoor() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S3_PZDoor,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S4_HillTop:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S4_HillTop() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S4_HillTop,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S4_BinocularHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S4_BinocularHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S4_BinocularHO,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S5_HillSide:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S5_HillSide() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S5_HillSide,1);//todo_LoaderUpdates
						break;
					case eArea_L4_S5_PZSwapTile:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L4_S5_PZSwapTiles() );
						}
						else
							PreloadLoadLevelArea(eArea_L4_S5_PZSwapTile,1);//todo_LoaderUpdates
						break;  
				#endif // _LEVEL4_ENABLED

						//Level 5
				#ifdef _LEVEL5_ENABLED
					case eArea_L5_S1_TigerDistant:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S1_TigerDistant() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S1_TigerDistant,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S2_TigerCloseup:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S2_TigerCloseup() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S2_TigerCloseup,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S2_PZBlocks:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S2_PZBlocks() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S2_PZBlocks,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S3_Pond:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S3_Pond() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S3_Pond,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S3_PZDungeonDoor:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S3_PZDungeonDoor() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S3_PZDungeonDoor,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S3_HOUnderwater:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S3_HOUnderwater() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S3_HOUnderwater,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S4_Dungeon:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S4_Dungeon() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S4_Dungeon,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S4_PZHandCuff:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S4_PZHandCuff() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S4_PZHandCuff,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S5_MountainBelow:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S5_MountainBelow() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S5_MountainBelow,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S5_HOGanapathi:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S5_HOGanapathi() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S5_HOGanapathi,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S5_PZSunClock:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S5_PZSunClock() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S5_PZSunClock,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S6_MountainTop:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S6_MountainTop() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S6_MountainTop,1);//todo_LoaderUpdates
						break;
					case eArea_L5_S6_HOTree:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L5_S6_HOTree() );
						}
						else
							PreloadLoadLevelArea(eArea_L5_S6_HOTree,1);//todo_LoaderUpdates
						break;  
				#endif // _LEVEL5_ENABLED

				#ifdef _LEVEL6_ENABLED
					case eArea_L6_S1_FloatingIsland1:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S1_FloatingIsland_1() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S1_FloatingIsland1,1);//todo_LoaderUpdates
						break;  
					case eArea_L6_S1_DescriptionHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S1_DescriptionHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S1_DescriptionHO,1);//todo_LoaderUpdates
						break;  
					case eArea_L6_S2_FloatingIsland2:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S2_FloatingIsland_2() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S2_FloatingIsland2,1);//todo_LoaderUpdates
						break; 		  
					case eArea_L6_S2_ShereHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S2_SphereHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S2_ShereHO,1);//todo_LoaderUpdates
						break; 
					case eArea_L6_S2_PZPineGum:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S2_PZPineGum() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S2_PZPineGum,1);//todo_LoaderUpdates
						break; 
					case eArea_L6_S3_FloatingIsland3:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S3_FloatingIsland_3() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S3_FloatingIsland3,1);//todo_LoaderUpdates
						break;
					case eArea_L6_S3_FakeJames:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S3_FakeJames() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S3_FakeJames,1);//todo_LoaderUpdates
						break;
					case eArea_L6_S3_GlowingHeartHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S3_GlowingHeartHO() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S3_GlowingHeartHO,1);//todo_LoaderUpdates
						break; 
					case eArea_L6_S3_PZPathway:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S3_PZPathway() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S3_PZPathway,1);//todo_LoaderUpdates
						break; 
					case eArea_L6_S4_FloatingIsland4:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S4_FloatingIsland_4() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S4_FloatingIsland4,1);//todo_LoaderUpdates
						break;  
					case eArea_L6_S4_PortalChest:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S4_PortalChest() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S4_PortalChest,1);//todo_LoaderUpdates
						break; 
					case eArea_L6_S4_PortalPuzzle:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S4_PortalPuzzle() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S4_PortalPuzzle,1);//todo_LoaderUpdates
						break; 
					case eArea_L6_S5_MarysDream:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6_S5_MarysDream() );
						}
						else
							PreloadLoadLevelArea(eArea_L6_S5_MarysDream,1);//todo_LoaderUpdates
						break; 
					case eArea_L6_S5_TabMiniGame:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L6S5TabMiniGame());
						}
						else
							PreloadLoadLevelArea(eArea_L6_S5_TabMiniGame,1);//todo_LoaderUpdates
						break;
				#endif // _LEVEL6_ENABLED

				#ifdef _LEVEL7_ENABLED
						//Level 7
					case eArea_L7_S1_Volcano:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S1_Volcano() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S1_Volcano,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S1_PZJigsawYellow:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S1_PZJigsawYellow() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S1_PZJigsawYellow,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S1_PZStarPuzzle:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S1_PZStarPuzzle() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S1_PZStarPuzzle,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S2_LasyaPrison:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S2_LasyaPrison() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S2_LasyaPrison,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S2_HORightHand:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S2_HORightHand() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S2_HORightHand,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S2_HOLeftHand:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S2_HOLeftHand() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S2_HOLeftHand,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S2_HOBrokenRightHand:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S2_HOBrokenRightHand() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S2_HOBrokenRightHand,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S2_HOBrokenLeftHand:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S2_HOBrokenLeftHand() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S2_HOBrokenLeftHand,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S2_PZEyePuzzle:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S2_PZEyePuzzle() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S2_PZEyePuzzle,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S2_PZLasyaStatue:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S2_PZLasyaStatue() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S2_PZLasyaStatue,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S3_Haven:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S3_Haven() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S3_Haven,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S3_PZDoorPuzzle:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S3_PZDoorPuzzle() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S3_PZDoorPuzzle,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S3_PZJigsawRed:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S3_PZJigsawRed() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S3_PZJigsawRed,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S4_Ameeha:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S4_Ameeha() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S4_Ameeha,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S4_PZJigsawBlue:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S4_PZJigsawBlue() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S4_PZJigsawBlue,1);//todo_LoaderUpdates
						break;
					case eArea_L7_S5_SophiaEnd:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L7_S5_SophiaEnd() );
						}
						else
							PreloadLoadLevelArea(eArea_L7_S5_SophiaEnd,1);//todo_LoaderUpdates
						break;
				#endif //

				#ifdef _LEVEL8_ENABLED
					case eArea_L8_S1_ShukrasPlace:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S1_ShukrasPlace());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S1_ShukrasPlace,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S1_PZDoorHandPuzzle:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S1_PZDoor());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S1_PZDoorHandPuzzle,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S1_DoorHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S1_DoorHO());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S1_DoorHO,1);//todo_LoaderUpdates
						break;
						break;
					case eArea_L8_S2_Garden:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S2_Garden());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S2_Garden,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S2_PZGears:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S2_GearPuzzle());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S2_PZGears,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S2_PZGate:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S2_PZGate());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S2_PZGate,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S2_PZSugarCane:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S2_PZSugarCane());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S2_PZSugarCane,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S3_BanyanTree:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S3_BanyanTree());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S3_BanyanTree,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S3_BanyanTreeHO:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S3_BanyanTreeHO());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S3_BanyanTreeHO,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S4_Pool:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S4_Pool());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S4_Pool,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S4_PZPoolGame:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S4_PZPoolGame());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S4_PZPoolGame,1);//todo_LoaderUpdates
						break;
					case eArea_L8_S5_VolcanoLand:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S5_VolcanoLand());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S5_VolcanoLand,1);//todo_LoaderUpdates
						break;

					case eArea_L8_S5_VirtualMiniGame:
						if( Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
						{
							nPostUpdates = 0;
							AddControl( new L8_S5_VirtualMiniGame());
						}
						else
							PreloadLoadLevelArea(eArea_L8_S5_VirtualMiniGame,1);//todo_LoaderUpdates
						break;
				#endif

			#endif // !_DEMOBUILD

			//Small scenes - Loader anim needed only if core not loaded -> 0 post updates
			//////////////////////////////
			default:
				std::string TmpStr = std::string("Er0x1601- Requested scene file does not exist = ") + LevelAreaNames[ProfileSystem::Instance->ActiveProfile.CurrentArea];
				#ifdef K_WIN32
					WARN( (const char*)(FileSystem::s2ws(TmpStr).c_str()) );//WIDESTR WARNINGS		
				#else
					WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
				#endif		
				// Something majorly wrong that there is no Level to load, go back to main menu gracefully
				Flow = fRootMenu;	
				#ifdef TOUCH_UI
					Cursor::SetInMenu(true);
				#endif		
				AddControl(new MenuSystem());
				/*
					#ifdef K_WIN32
						WARN((const char*)L"Requested scene file does not exist.");
					#else
						WARN((const char*)"Requested scene file does not exist.");
					#endif
				*/	
		}
	#endif	
}

const CPoint CamPos(0,0,0);
void GameFlow::PostUpdate(int Updates)
{
	if( nPostUpdates )
	{		
		if( GFApp->Window->isQuit() )
			GFApp->AppActive = false;//else user need to wait till full loader is done!!
		else 
		{
			LoadingFile += Updates;		
			//Update the glow ring	
			LoaderGlowBar->SetFrame((int)(MAX_GLOWRING_FRAMES * LoadingFile / (float)(nPostUpdates)));		

			//Render loaderscene
			GFApp->Window->setWorldView (GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, false);    		
			LoaderBase->Render(&CamPos);
			LoaderGlowBar->Render(&CamPos);
			CETitle->Render(&CamPos);
			GFApp->Window->flipBackBuffer();//no correction needed	
		}
	}
}

void GameFlow::ClearPrevLevelInventories()
{
	//Clear past inventory to avod crash for past gamers.
	switch (ProfileSystem::Instance->ActiveProfile.CurrentLevel)
	{
	case Level_2:	for(int i = 0; i <= eInv_L1S4_BookCode; i++)
						if( ProfileSystem::Instance->ActiveProfile.Inventory[i] )
						{
						  ProfileSystem::Instance->ActiveProfile.Inventory[i] = false;
						}
					break;
	case Level_3:	for(int i = 0; i <= eInv_L2S6_PaperPuzClue; i++)
						if( ProfileSystem::Instance->ActiveProfile.Inventory[i] )
						{
						  ProfileSystem::Instance->ActiveProfile.Inventory[i] = false;
						}
		break;
	case Level_4:
		 for(int i = 0; i <= eInv_L3S5_Rope; i++)
						if( ProfileSystem::Instance->ActiveProfile.Inventory[i] )
						{
						  ProfileSystem::Instance->ActiveProfile.Inventory[i] = false;
						}
		break;
	case Level_5: for(int i = 0; i <= eInv_L4_CB_BambooWithKnife; i++)
						if( ProfileSystem::Instance->ActiveProfile.Inventory[i] )
						{
						  ProfileSystem::Instance->ActiveProfile.Inventory[i] = false;
						}
		break;
	case Level_6:	for(int i = 0; i <= eInv_L5_S5_StoneEmblem; i++)
						if( ProfileSystem::Instance->ActiveProfile.Inventory[i] )
						{
						  ProfileSystem::Instance->ActiveProfile.Inventory[i] = false;
						}
		break;
	case Level_7:	for(int i = 0; i <= eInv_L6_CB_RopeWithMagnetisedHorseShoe; i++)
						if( ProfileSystem::Instance->ActiveProfile.Inventory[i] )
						{
						  ProfileSystem::Instance->ActiveProfile.Inventory[i] = false;
						}
		break;
	case Level_8:	 for(int i = 0; i <= eInv_L7_S4_WellGear; i++)
						if( ProfileSystem::Instance->ActiveProfile.Inventory[i] )
						{
						  ProfileSystem::Instance->ActiveProfile.Inventory[i] = false;
						}
		break;
	}
	

}

void GameFlow::PreloadLoadLevelArea( int LevelArea, int nUpdates )
{
	nPostUpdates = nUpdates;	

	LoadingFile = 0;
	LoadingScene->Show(true);
	LoaderGlowBar->ForcePlayAnimation(0);	
	Cursor::Show(false);
	Control::Audio->PlaySample(aSFXMagic1,false);

	ClearPrevLevelInventories();


	if( LevelArea != eArea_L1_EndOfDemo )//No need to load level assets for end of demo scene!!
		if( !Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] )
		{
			LvlAssetsLoader();
			Control::Audio->IsLevelAssetsLoaded[ProfileSystem::Instance->ActiveProfile.CurrentLevel] = true;
		}

		#if defined(K_ANDROID) && !defined(_DEMOBUILD)
			if(!GFApp->isUnlocked()) //If Free version
				{
					switch( LevelArea )
					{
						//Level 1
						#ifdef _LEVEL1_ENABLED
							case eArea_L1_S1_Beach:			AddControl( new L1_S1_Beach() );	break;
							case eArea_L1_S1_PZKidsPainting:AddControl( new L1_S1_PZKidsPainting() );break;
							case eArea_L1_S2_Frontyard:		AddControl( new L1_S2_Frontyard() );break;
							case eArea_L1_S2_PZFuse:		AddControl( new L1_S2_PZFuse() );	break;
							case eArea_L1_S3_Foyer:			AddControl( new L1_S3_Foyer() );	break;
							case eArea_L1_S3_PZShelf:		AddControl( new L1_S3_PZShelf());	break;
							case eArea_L1_S4_PZDoor:		AddControl( new L1_S4_PZDoor() );	break;
							case eArea_L1_S4_Bedroom:		AddControl( new L1_S4_Bedroom() );	break;
							case eArea_L1_S4_PZPainting:	AddControl( new L1_S4_PZPainting() );break;
							case eArea_L1_S3_DoorHO:		AddControl( new L1_S3_DoorHO() );	break;
							case eArea_L1_S5_TreasureRoom:	AddControl( new L1_S5_TreasureRoom() );	break;
							case eArea_L1_S5_PZDynamiteMaking:	AddControl( new L1_S5_PZDynamiteMaking() );	break;
							case eArea_L1_S5_HOTreasureRoom: AddControl(new L1_S5_HOTreasureRoom()); break;
							case eArea_L1_S5_TunnelDoor:	AddControl( new L1_S5_TunnelDoor() );	break;
							case eArea_L1_S6_HOBoatShed:	AddControl( new L1_S6_HOBoatShed() );	break;
							case eArea_L1_S7_BoatDock:		AddControl( new L1_S7_Boatdock() );	break;
							case eArea_L1_S6_BoatShed:		AddControl( new L1_S6_Boatshed() );	break;

							case eArea_L1_EndOfDemo:		AddControl( new EndOfDemoScene() );	break;  
						#endif // _LEVEL1_ENABLED
						case eArea_KidsRoom:			AddControl( new KidsRoom() );		break;
						case eArea_ExtrasCutscenePlayer:AddControl( new ExtrasCutscenePlayer() );	break;
						case eArea_ExtrasRoom:			AddControl( new ExtrasRoom() );		break;
						case eArea_AwardsRoom:			AddControl( new AwardsRoom() );		break;
						case eArea_Credits:				AddControl(new Credits()); break;
					}
				}
			else
				{
					switch( LevelArea )
					{
						//Level 1
						#ifdef _LEVEL1_ENABLED
							case eArea_L1_S1_Beach:			AddControl( new L1_S1_Beach() );	break;
							case eArea_L1_S1_PZKidsPainting:AddControl( new L1_S1_PZKidsPainting() );break;
							case eArea_L1_S2_Frontyard:		AddControl( new L1_S2_Frontyard() );break;
							case eArea_L1_S2_PZFuse:		AddControl( new L1_S2_PZFuse() );	break;
							case eArea_L1_S3_Foyer:			AddControl( new L1_S3_Foyer() );	break;
							case eArea_L1_S3_PZShelf:		AddControl( new L1_S3_PZShelf());	break;
							case eArea_L1_S4_PZDoor:		AddControl( new L1_S4_PZDoor() );	break;
							case eArea_L1_S4_Bedroom:		AddControl( new L1_S4_Bedroom() );	break;
							case eArea_L1_S4_PZPainting:	AddControl( new L1_S4_PZPainting() );break;
							case eArea_L1_S3_DoorHO:		AddControl( new L1_S3_DoorHO() );	break;
							case eArea_L1_S5_TreasureRoom:	AddControl( new L1_S5_TreasureRoom() );	break;
							case eArea_L1_S5_PZDynamiteMaking:	AddControl( new L1_S5_PZDynamiteMaking() );	break;
							case eArea_L1_S5_HOTreasureRoom: AddControl(new L1_S5_HOTreasureRoom()); break;
							case eArea_L1_S5_TunnelDoor:	AddControl( new L1_S5_TunnelDoor() );	break;
							case eArea_L1_S6_HOBoatShed:	AddControl( new L1_S6_HOBoatShed() );	break;
							case eArea_L1_S7_BoatDock:		AddControl( new L1_S7_Boatdock() );	break;
							case eArea_L1_S6_BoatShed:		AddControl( new L1_S6_Boatshed() );	break;

							case eArea_L1_EndOfDemo:		AddControl( new EndOfDemoScene() );	break;  
						#endif // _LEVEL1_ENABLED
						case eArea_KidsRoom:			AddControl( new KidsRoom() );		break;
						case eArea_ExtrasCutscenePlayer:AddControl( new ExtrasCutscenePlayer() );	break;
						case eArea_ExtrasRoom:			AddControl( new ExtrasRoom() );		break;
						case eArea_AwardsRoom:			AddControl( new AwardsRoom() );		break;
						case eArea_Credits:						AddControl(new Credits()); break;
						//Level 2
						#ifdef _LEVEL2_ENABLED
								case eArea_L2_S1_ForestEntrance:	AddControl( new L2_S1_ForestEntrance() );	break;
								case eArea_L2_S1_UndergroundHO:		AddControl( new L2_S1_UndergroundHO() );   break;
								case eArea_L2_S2_NagaTemple:		AddControl( new L2_S2_NagaTemple() );		break;
								case eArea_L2_S2_PZMango:			AddControl( new L2_S2_PZMango() );			break;
								case eArea_L2_S2_HONagaTemple:		AddControl( new L2_S2_NagaTempleHO() );		break;
								case eArea_L2_S3_CaveMouth:			AddControl( new L2_S3_CaveMouth() );		break;
								case eArea_L2_S3_PZStoneBeam:		AddControl( new L2_S3_PZStoneBeam() );		break;
								case eArea_L2_S4_Cave:				AddControl( new L2_S4_Cave());				break;
								case eArea_L2_S4_PZDeities:			AddControl( new L2_S4_DeitiesPuzzle());		break;
								case eArea_L2_S5_MoopansDen:		AddControl( new L2_S5_MoopansDen() );		break;
								case eArea_L2_S5_PZEagleUnblock:	AddControl( new L2_S5_EagleBlockPuzzle() ); break;
								case eArea_L2_S5_PZMedicine_Making: AddControl( new L2_S5_PZMedicineMaking());	break;
								case eArea_L2_S6_PZTunnelClock:		AddControl( new L2_S6_PZTunnelClock() );	break;
								case eArea_L2_S6_TunnelEnd:			AddControl( new L2_S6_TunnelEnd() );		break;
								case eArea_L2_S6_HOWaterfall:		AddControl( new L2_S6_HOWaterfall() );		break;
								case eArea_L2_S7_PZSoldiers:		AddControl( new L2S7SoldiersPuzzle() );		break;  
						#endif // _LEVEL2_ENABLED

						//Level 3
						#ifdef _LEVEL3_ENABLED
								case eArea_L3_S1_FortEntrance:		AddControl( new L3_S1_FortEntrance() );		break;
								case eArea_L3_S1_PZLionHead:		AddControl( new L3_S1_PZLionHead() );		break;	
								case eArea_L3_S1_HOLadder:			AddControl( new L3_S1_HOLadder() );			break;
								case eArea_L3_S2_FortInterior:		AddControl( new L3_S2_FortInterior() );		break;
								case eArea_L3_S2_P1Room1Start:		AddControl( new L3_S2_P1Room1Start() );		break;
								case eArea_L3_S2_P1Room2Wine:		AddControl( new L3_S2_P1Room2Wine() );		break;
								case eArea_L3_S2_P1Room3Lock:		AddControl( new L3_S2_P1Room3Lock() );		break;
								case eArea_L3_S2_P1Room4Match3:		AddControl( new L3_S2_P1Room4Match3() );	break;
								case eArea_L3_S2_P1Room5Pipe:		AddControl( new L3_S2_P1Room5Pipe() );		break;
								case eArea_L3_S2_P1Room6Pillar:		AddControl( new L3_S2_P1Room6Pillar() );	break;
								case eArea_L3_S2_P1DeadEnd:		AddControl( new L3_S2_MazeDeadEnd() );	break;
								case eArea_L3_S3_Pool:				AddControl( new L3_S3_Pool() );				break;
								case eArea_L3_S3_PZBlocks:          AddControl( new L3_S3_PZBlock());           break;
								case eArea_L3_S3_PZUnderwater:		AddControl( new L3_S3_PZUnderwater() );		break;
								case eArea_L3_S4_FortFountain:		AddControl( new L3_S4_FortFountain() );		break;
								case eArea_L3_S4_FortFountainHO:	AddControl( new L3_S4FortFoundationHO() );	break;
								case eArea_L3_S5_Cliff:				AddControl( new L3_S5_Cliff() );			break;  
						#endif // _LEVEL3_ENABLED

						//Level 4
						#ifdef _LEVEL4_ENABLED
								case eArea_L4_S1_SwampEntrance:		AddControl( new L4_S1_SwampEntrance() );	break;
								case eArea_L4_S1_ShoeHO:			AddControl( new L4_S1_ShoeHO() );			break;
								case eArea_L4_S2_RiverSide:			AddControl( new L4_S2_RiverSide() );		break;
								case eArea_L4_S2_PZAlligatorHead:	AddControl( new L4_S2_AlligatorPuzzle() );	break;

								case eArea_L4_S2_PZDoor:			AddControl( new L4_S2_FanPuzzle() );		break;
								case eArea_L4_S3_Cave:				AddControl( new L4_S3_Cave() );				break;
								case eArea_L4_S3_PZFireMaking:		AddControl(new L4_S3_PZFireMaking);			break;
								case eArea_L4_S3_PZDoor:			AddControl(new L4_S3_PZDoor());				break;
								case eArea_L4_S4_HillTop:			AddControl( new L4_S4_HillTop() );			break;
								case eArea_L4_S4_BinocularHO:		AddControl( new L4_S4_BinocularHO());		break;
								case eArea_L4_S5_HillSide:			AddControl( new L4_S5_HillSide() );			break;
								case eArea_L4_S5_PZSwapTile:		AddControl(new L4_S5_PZSwapTiles() );		break;  
						#endif // _LEVEL4_ENABLED

						//Level 5
						#ifdef _LEVEL5_ENABLED
								case eArea_L5_S1_TigerDistant:		AddControl(new L5_S1_TigerDistant() );	break;
								case eArea_L5_S2_TigerCloseup:		AddControl(new L5_S2_TigerCloseup() );	break;
								case eArea_L5_S2_PZBlocks:			AddControl(new L5_S2_PZBlocks() );		break;
								case eArea_L5_S3_Pond:				AddControl(new L5_S3_Pond() );			break;
								case eArea_L5_S3_PZDungeonDoor:		AddControl(new L5_S3_PZDungeonDoor() ); break;
								case eArea_L5_S3_HOUnderwater:		AddControl(new L5_S3_HOUnderwater() );	break;
								case eArea_L5_S4_Dungeon:			AddControl(new L5_S4_Dungeon() );		break;
								case eArea_L5_S4_PZHandCuff:		AddControl(new L5_S4_PZHandCuff() );	break;
								case eArea_L5_S5_MountainBelow:		AddControl(new L5_S5_MountainBelow() ); break;
								case eArea_L5_S5_HOGanapathi:		AddControl(new L5_S5_HOGanapathi() );	break;
								case eArea_L5_S5_PZSunClock:		AddControl(new L5_S5_PZSunClock() );	break;
								case eArea_L5_S6_MountainTop:		AddControl(new L5_S6_MountainTop() );	break;	
								case eArea_L5_S6_HOTree:			AddControl(new L5_S6_HOTree() );		break;  
						#endif // _LEVEL5_ENABLED

						//Level 6
						#ifdef _LEVEL6_ENABLED
								case eArea_L6_S1_FloatingIsland1:	AddControl(new L6_S1_FloatingIsland_1() );	break;  
								case eArea_L6_S1_DescriptionHO:	    AddControl(new L6_S1_DescriptionHO() );	    break;
								case eArea_L6_S2_FloatingIsland2:	AddControl(new L6_S2_FloatingIsland_2() );  break;
								case eArea_L6_S2_ShereHO:			AddControl(new L6_S2_SphereHO());			break;
								case eArea_L6_S2_PZPineGum:			AddControl(new L6_S2_PZPineGum());			break;
								case eArea_L6_S3_FloatingIsland3:	AddControl(new L6_S3_FloatingIsland_3() );  break;
								case eArea_L6_S3_FakeJames:			AddControl(new L6_S3_FakeJames() );			break;
								case eArea_L6_S3_PZPathway:			AddControl(new L6_S3_PZPathway() );			break;
								case eArea_L6_S3_GlowingHeartHO:	AddControl(new L6_S3_GlowingHeartHO() );	break;
								case eArea_L6_S4_FloatingIsland4:	AddControl(new L6_S4_FloatingIsland_4() );	break;
								case eArea_L6_S4_PortalChest:		AddControl(new L6_S4_PortalChest() );		break;
								case eArea_L6_S4_PortalPuzzle:		AddControl(new L6_S4_PortalPuzzle() );		break;
								case eArea_L6_S5_MarysDream:		AddControl(new L6_S5_MarysDream() );		break;
								case eArea_L6_S5_TabMiniGame:		AddControl(new L6S5TabMiniGame() );			break;
						#endif // _LEVEL6_ENABLED

						#ifdef _LEVEL7_ENABLED
									//Level 7
								case eArea_L7_S1_Volcano:				AddControl(new L7_S1_Volcano() ); break;
								case eArea_L7_S1_PZJigsawYellow:		AddControl(new L7_S1_PZJigsawYellow() ); break;
								case eArea_L7_S1_PZStarPuzzle:			AddControl(new L7_S1_PZStarPuzzle() ); break;
								case eArea_L7_S2_LasyaPrison:			AddControl(new L7_S2_LasyaPrison() ); break;
								case eArea_L7_S2_HORightHand:			AddControl(new L7_S2_HORightHand() ); break;
								case eArea_L7_S2_HOLeftHand:			AddControl(new L7_S2_HOLeftHand() ); break;
								case eArea_L7_S2_HOBrokenRightHand:		AddControl(new L7_S2_HOBrokenRightHand() ); break;
								case eArea_L7_S2_HOBrokenLeftHand:		AddControl(new L7_S2_HOBrokenLeftHand() ); break;
								case eArea_L7_S2_PZEyePuzzle:			AddControl(new L7_S2_PZEyePuzzle() ); break;
								case eArea_L7_S2_PZLasyaStatue:			AddControl(new L7_S2_PZLasyaStatue() ); break;
								case eArea_L7_S3_Haven:					AddControl(new L7_S3_Haven() ); break;
								case eArea_L7_S3_PZDoorPuzzle:			AddControl(new L7_S3_PZDoorPuzzle() ); break;
								case eArea_L7_S3_PZJigsawRed:			AddControl(new L7_S3_PZJigsawRed() ); break;
								case eArea_L7_S4_Ameeha:				AddControl(new L7_S4_Ameeha() ); break;
								case eArea_L7_S4_PZJigsawBlue:			AddControl(new L7_S4_PZJigsawBlue() ); break;
								case eArea_L7_S5_SophiaEnd:				AddControl(new L7_S5_SophiaEnd() ); break;
						#endif //_LEVEL7_ENABLED

						#ifdef _LEVEL8_ENABLED
								case eArea_L8_S1_ShukrasPlace:			AddControl(new L8_S1_ShukrasPlace() );		break;
								case eArea_L8_S1_PZDoorHandPuzzle:		AddControl(new L8_S1_PZDoor() );			break;
								case eArea_L8_S1_DoorHO:			    AddControl(new L8_S1_DoorHO());             break;
								case eArea_L8_S2_Garden:				AddControl(new L8_S2_Garden() );			break;
								case eArea_L8_S2_PZGears:				AddControl(new L8_S2_GearPuzzle() );		break;	
								case eArea_L8_S2_PZGate:				AddControl(new L8_S2_PZGate() );			break;
								case eArea_L8_S2_PZSugarCane:			AddControl(new L8_S2_PZSugarCane() );		break;
								case eArea_L8_S3_BanyanTree:			AddControl(new L8_S3_BanyanTree() );		break;
								case eArea_L8_S3_BanyanTreeHO:			AddControl(new L8_S3_BanyanTreeHO() );		break;
								case eArea_L8_S4_Pool:					AddControl(new L8_S4_Pool() );				break;
								case eArea_L8_S4_PZPoolGame:			AddControl(new L8_S4_PZPoolGame() );		break;
								case eArea_L8_S5_VolcanoLand:			AddControl(new L8_S5_VolcanoLand() );		break;
								case eArea_L8_S5_VirtualMiniGame:		AddControl(new L8_S5_VirtualMiniGame() );	break;
						#endif
					}
				}
		#else
			switch( LevelArea )
			{
				//Level 1
				#ifdef _LEVEL1_ENABLED
					case eArea_L1_S1_Beach:			AddControl( new L1_S1_Beach() );	break;
					case eArea_L1_S1_PZKidsPainting:AddControl( new L1_S1_PZKidsPainting() );break;
					case eArea_L1_S2_Frontyard:		AddControl( new L1_S2_Frontyard() );break;
					case eArea_L1_S2_PZFuse:		AddControl( new L1_S2_PZFuse() );	break;
					case eArea_L1_S3_Foyer:			AddControl( new L1_S3_Foyer() );	break;
					case eArea_L1_S3_PZShelf:		AddControl( new L1_S3_PZShelf());	break;
					case eArea_L1_S4_PZDoor:		AddControl( new L1_S4_PZDoor() );	break;
					case eArea_L1_S4_Bedroom:		AddControl( new L1_S4_Bedroom() );	break;
					case eArea_L1_S4_PZPainting:	AddControl( new L1_S4_PZPainting() );break;
					case eArea_L1_S3_DoorHO:		AddControl( new L1_S3_DoorHO() );	break;
					case eArea_L1_S5_TreasureRoom:	AddControl( new L1_S5_TreasureRoom() );	break;
					case eArea_L1_S5_PZDynamiteMaking:	AddControl( new L1_S5_PZDynamiteMaking() );	break;
					case eArea_L1_S5_HOTreasureRoom: AddControl(new L1_S5_HOTreasureRoom()); break;
					case eArea_L1_S5_TunnelDoor:	AddControl( new L1_S5_TunnelDoor() );	break;
					case eArea_L1_S6_HOBoatShed:	AddControl( new L1_S6_HOBoatShed() );	break;
					case eArea_L1_S7_BoatDock:		AddControl( new L1_S7_Boatdock() );	break;
					case eArea_L1_S6_BoatShed:		AddControl( new L1_S6_Boatshed() );	break;

					case eArea_L1_EndOfDemo:		AddControl( new EndOfDemoScene() );	break;  
				#endif // _LEVEL1_ENABLED

					case eArea_KidsRoom:			AddControl( new KidsRoom() );		break;
					case eArea_ExtrasCutscenePlayer:AddControl( new ExtrasCutscenePlayer() );	break;
					case eArea_ExtrasRoom:			AddControl( new ExtrasRoom() );		break;
					case eArea_AwardsRoom:			AddControl( new AwardsRoom() );		break;
					case eArea_Credits:						AddControl(new Credits()); break;

				#ifndef _DEMOBUILD
					//Level 2
					#ifdef _LEVEL2_ENABLED
							case eArea_L2_S1_ForestEntrance:	AddControl( new L2_S1_ForestEntrance() );	break;
							case eArea_L2_S1_UndergroundHO:		AddControl( new L2_S1_UndergroundHO() );   break;
							case eArea_L2_S2_NagaTemple:		AddControl( new L2_S2_NagaTemple() );		break;
							case eArea_L2_S2_PZMango:			AddControl( new L2_S2_PZMango() );			break;
							case eArea_L2_S2_HONagaTemple:		AddControl( new L2_S2_NagaTempleHO() );		break;
							case eArea_L2_S3_CaveMouth:			AddControl( new L2_S3_CaveMouth() );		break;
							case eArea_L2_S3_PZStoneBeam:		AddControl( new L2_S3_PZStoneBeam() );		break;
							case eArea_L2_S4_Cave:				AddControl( new L2_S4_Cave());				break;
							case eArea_L2_S4_PZDeities:			AddControl( new L2_S4_DeitiesPuzzle());		break;
							case eArea_L2_S5_MoopansDen:		AddControl( new L2_S5_MoopansDen() );		break;
							case eArea_L2_S5_PZEagleUnblock:	AddControl( new L2_S5_EagleBlockPuzzle() ); break;
							case eArea_L2_S5_PZMedicine_Making: AddControl( new L2_S5_PZMedicineMaking());	break;
							case eArea_L2_S6_PZTunnelClock:		AddControl( new L2_S6_PZTunnelClock() );	break;
							case eArea_L2_S6_TunnelEnd:			AddControl( new L2_S6_TunnelEnd() );		break;
							case eArea_L2_S6_HOWaterfall:		AddControl( new L2_S6_HOWaterfall() );		break;
							case eArea_L2_S7_PZSoldiers:		AddControl( new L2S7SoldiersPuzzle() );		break;  
					#endif // _LEVEL2_ENABLED

					//Level 3
					#ifdef _LEVEL3_ENABLED
							case eArea_L3_S1_FortEntrance:		AddControl( new L3_S1_FortEntrance() );		break;
							case eArea_L3_S1_PZLionHead:		AddControl( new L3_S1_PZLionHead() );		break;	
							case eArea_L3_S1_HOLadder:			AddControl( new L3_S1_HOLadder() );			break;
							case eArea_L3_S2_FortInterior:		AddControl( new L3_S2_FortInterior() );		break;
							case eArea_L3_S2_P1Room1Start:		AddControl( new L3_S2_P1Room1Start() );		break;
							case eArea_L3_S2_P1Room2Wine:		AddControl( new L3_S2_P1Room2Wine() );		break;
							case eArea_L3_S2_P1Room3Lock:		AddControl( new L3_S2_P1Room3Lock() );		break;
							case eArea_L3_S2_P1Room4Match3:		AddControl( new L3_S2_P1Room4Match3() );	break;
							case eArea_L3_S2_P1Room5Pipe:		AddControl( new L3_S2_P1Room5Pipe() );		break;
							case eArea_L3_S2_P1Room6Pillar:		AddControl( new L3_S2_P1Room6Pillar() );	break;
							case eArea_L3_S2_P1DeadEnd:		AddControl( new L3_S2_MazeDeadEnd() );	break;
							case eArea_L3_S3_Pool:				AddControl( new L3_S3_Pool() );				break;
							case eArea_L3_S3_PZBlocks:          AddControl( new L3_S3_PZBlock());           break;
							case eArea_L3_S3_PZUnderwater:		AddControl( new L3_S3_PZUnderwater() );		break;
							case eArea_L3_S4_FortFountain:		AddControl( new L3_S4_FortFountain() );		break;
							case eArea_L3_S4_FortFountainHO:	AddControl( new L3_S4FortFoundationHO() );	break;
							case eArea_L3_S5_Cliff:				AddControl( new L3_S5_Cliff() );			break;  
					#endif // _LEVEL3_ENABLED

					//Level 4
					#ifdef _LEVEL4_ENABLED
							case eArea_L4_S1_SwampEntrance:		AddControl( new L4_S1_SwampEntrance() );	break;
							case eArea_L4_S1_ShoeHO:			AddControl( new L4_S1_ShoeHO() );			break;
							case eArea_L4_S2_RiverSide:			AddControl( new L4_S2_RiverSide() );		break;
							case eArea_L4_S2_PZAlligatorHead:	AddControl( new L4_S2_AlligatorPuzzle() );	break;

							case eArea_L4_S2_PZDoor:			AddControl( new L4_S2_FanPuzzle() );		break;
							case eArea_L4_S3_Cave:				AddControl( new L4_S3_Cave() );				break;
							case eArea_L4_S3_PZFireMaking:		AddControl(new L4_S3_PZFireMaking);			break;
							case eArea_L4_S3_PZDoor:			AddControl(new L4_S3_PZDoor());				break;
							case eArea_L4_S4_HillTop:			AddControl( new L4_S4_HillTop() );			break;
							case eArea_L4_S4_BinocularHO:		AddControl( new L4_S4_BinocularHO());		break;
							case eArea_L4_S5_HillSide:			AddControl( new L4_S5_HillSide() );			break;
							case eArea_L4_S5_PZSwapTile:		AddControl(new L4_S5_PZSwapTiles() );		break;  
					#endif // _LEVEL4_ENABLED

					//Level 5
					#ifdef _LEVEL5_ENABLED
							case eArea_L5_S1_TigerDistant:		AddControl(new L5_S1_TigerDistant() );	break;
							case eArea_L5_S2_TigerCloseup:		AddControl(new L5_S2_TigerCloseup() );	break;
							case eArea_L5_S2_PZBlocks:			AddControl(new L5_S2_PZBlocks() );		break;
							case eArea_L5_S3_Pond:				AddControl(new L5_S3_Pond() );			break;
							case eArea_L5_S3_PZDungeonDoor:		AddControl(new L5_S3_PZDungeonDoor() ); break;
							case eArea_L5_S3_HOUnderwater:		AddControl(new L5_S3_HOUnderwater() );	break;
							case eArea_L5_S4_Dungeon:			AddControl(new L5_S4_Dungeon() );		break;
							case eArea_L5_S4_PZHandCuff:		AddControl(new L5_S4_PZHandCuff() );	break;
							case eArea_L5_S5_MountainBelow:		AddControl(new L5_S5_MountainBelow() ); break;
							case eArea_L5_S5_HOGanapathi:		AddControl(new L5_S5_HOGanapathi() );	break;
							case eArea_L5_S5_PZSunClock:		AddControl(new L5_S5_PZSunClock() );	break;
							case eArea_L5_S6_MountainTop:		AddControl(new L5_S6_MountainTop() );	break;	
							case eArea_L5_S6_HOTree:			AddControl(new L5_S6_HOTree() );		break;  
					#endif // _LEVEL5_ENABLED

					//Level 6
					#ifdef _LEVEL6_ENABLED
							case eArea_L6_S1_FloatingIsland1:	AddControl(new L6_S1_FloatingIsland_1() );	break;  
							case eArea_L6_S1_DescriptionHO:	    AddControl(new L6_S1_DescriptionHO() );	    break;
							case eArea_L6_S2_FloatingIsland2:	AddControl(new L6_S2_FloatingIsland_2() );  break;
							case eArea_L6_S2_ShereHO:			AddControl(new L6_S2_SphereHO());			break;
							case eArea_L6_S2_PZPineGum:			AddControl(new L6_S2_PZPineGum());			break;
							case eArea_L6_S3_FloatingIsland3:	AddControl(new L6_S3_FloatingIsland_3() );  break;
							case eArea_L6_S3_FakeJames:			AddControl(new L6_S3_FakeJames() );			break;
							case eArea_L6_S3_PZPathway:			AddControl(new L6_S3_PZPathway() );			break;
							case eArea_L6_S3_GlowingHeartHO:	AddControl(new L6_S3_GlowingHeartHO() );	break;
							case eArea_L6_S4_FloatingIsland4:	AddControl(new L6_S4_FloatingIsland_4() );	break;
							case eArea_L6_S4_PortalChest:		AddControl(new L6_S4_PortalChest() );		break;
							case eArea_L6_S4_PortalPuzzle:		AddControl(new L6_S4_PortalPuzzle() );		break;
							case eArea_L6_S5_MarysDream:		AddControl(new L6_S5_MarysDream() );		break;
							case eArea_L6_S5_TabMiniGame:		AddControl(new L6S5TabMiniGame() );			break;
					#endif // _LEVEL6_ENABLED

					#ifdef _LEVEL7_ENABLED
								//Level 7
							case eArea_L7_S1_Volcano:				AddControl(new L7_S1_Volcano() ); break;
							case eArea_L7_S1_PZJigsawYellow:		AddControl(new L7_S1_PZJigsawYellow() ); break;
							case eArea_L7_S1_PZStarPuzzle:			AddControl(new L7_S1_PZStarPuzzle() ); break;
							case eArea_L7_S2_LasyaPrison:			AddControl(new L7_S2_LasyaPrison() ); break;
							case eArea_L7_S2_HORightHand:			AddControl(new L7_S2_HORightHand() ); break;
							case eArea_L7_S2_HOLeftHand:			AddControl(new L7_S2_HOLeftHand() ); break;
							case eArea_L7_S2_HOBrokenRightHand:		AddControl(new L7_S2_HOBrokenRightHand() ); break;
							case eArea_L7_S2_HOBrokenLeftHand:		AddControl(new L7_S2_HOBrokenLeftHand() ); break;
							case eArea_L7_S2_PZEyePuzzle:			AddControl(new L7_S2_PZEyePuzzle() ); break;
							case eArea_L7_S2_PZLasyaStatue:			AddControl(new L7_S2_PZLasyaStatue() ); break;
							case eArea_L7_S3_Haven:					AddControl(new L7_S3_Haven() ); break;
							case eArea_L7_S3_PZDoorPuzzle:			AddControl(new L7_S3_PZDoorPuzzle() ); break;
							case eArea_L7_S3_PZJigsawRed:			AddControl(new L7_S3_PZJigsawRed() ); break;
							case eArea_L7_S4_Ameeha:				AddControl(new L7_S4_Ameeha() ); break;
							case eArea_L7_S4_PZJigsawBlue:			AddControl(new L7_S4_PZJigsawBlue() ); break;
							case eArea_L7_S5_SophiaEnd:				AddControl(new L7_S5_SophiaEnd() ); break;
					#endif //_LEVEL7_ENABLED

					#ifdef _LEVEL8_ENABLED
							case eArea_L8_S1_ShukrasPlace:			AddControl(new L8_S1_ShukrasPlace() );		break;
							case eArea_L8_S1_PZDoorHandPuzzle:		AddControl(new L8_S1_PZDoor() );			break;
							case eArea_L8_S1_DoorHO:			    AddControl(new L8_S1_DoorHO());             break;
							case eArea_L8_S2_Garden:				AddControl(new L8_S2_Garden() );			break;
							case eArea_L8_S2_PZGears:				AddControl(new L8_S2_GearPuzzle() );		break;	
							case eArea_L8_S2_PZGate:				AddControl(new L8_S2_PZGate() );			break;
							case eArea_L8_S2_PZSugarCane:			AddControl(new L8_S2_PZSugarCane() );		break;
							case eArea_L8_S3_BanyanTree:			AddControl(new L8_S3_BanyanTree() );		break;
							case eArea_L8_S3_BanyanTreeHO:			AddControl(new L8_S3_BanyanTreeHO() );		break;
							case eArea_L8_S4_Pool:					AddControl(new L8_S4_Pool() );				break;
							case eArea_L8_S4_PZPoolGame:			AddControl(new L8_S4_PZPoolGame() );		break;
							case eArea_L8_S5_VolcanoLand:			AddControl(new L8_S5_VolcanoLand() );		break;
							case eArea_L8_S5_VirtualMiniGame:		AddControl(new L8_S5_VirtualMiniGame() );	break;
					#endif

				

				#endif // !_DEMOBUILD

			}
		#endif
		LoadingScene->Show(false);
		LoaderGlowBar->PlayAnimation(1);				
		Cursor::Show(true);
}

void GameFlow::CreateThanks()
{
	PausedImage = NULL;
	PausedImage = new CObject();
	PausedImage->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\GAMEPAUSED.SPR", (GFApp->CurrentResolution.Width)*0.5f, (GFApp->CurrentResolution.Height)*0.5f, 3000);
	PausedImage->Show(false);
}

void GameFlow::CreateGreyscaleScene()
{
	//InitBlur
	GreyAlphaImage->SetAlpha(0.0f);
	GreyAlphaImage->Show(true);
}

void GameFlow::UpdateBlurScene( float Blur )
{
	GreyAlphaImage->SetAlpha(Blur);		
}

void GameFlow::TurnOffBlurScene()
{
	GreyAlphaImage->Show(false);
}

void GameFlow::WrongInvDropMsg( HudControl * MyHud )
{	
	Control::Input->ClearMouse();//Avoid unwanted click message handlers!!

	//if( MyHud->BannerState != eHMS_Idle )
	iWrongMsg = (iWrongMsg+1)%MAX_WRONGDROP_MSG;

	MyHud->ShowBannerText(WrongInvDropTexts[iWrongMsg],SFX_NIL);

	MyHud->CancelDrag();//just cancel so that user can choose another one if invbox is open!!	
}

void GameFlow::UnloadLevel2Assets()
{
	if( Control::Audio->IsLevelAssetsLoaded[Level_2] )
	{
		//unload samples with/without english build
		Control::Audio->UnloadSample(aTrackLevel2);
		Control::Audio->UnloadSample(aTrackL2theme);
		Control::Audio->UnloadSample(aTrackL2theme1);
		Control::Audio->UnloadSample(aTrackL2ho);
		Control::Audio->UnloadSample(aTrackL2puzzle);
		Control::Audio->UnloadSample(aTrackL2s7Puzzle);
		//Ambiance		




		//Sounds
		Control::Audio->UnloadSample(aSFXDanger0);						
		Control::Audio->UnloadSample(aSFXPain);		

		//unload any image directly if needed
		//CTexFactory::Instance->FreeImage("L2\\X.JPA");

		CTexFactory::Instance->FreeLevelImages(Level_2);
		Control::Audio->IsLevelAssetsLoaded[Level_2] = false;
	}
}

void GameFlow::UnloadLevel3Assets()
{
	if( Control::Audio->IsLevelAssetsLoaded[Level_3] )
	{
		//unload any image directly if needed
		//CTexFactory::Instance->FreeImage("L2\\x.JPA");


		Control::Audio->UnloadSample(aTrackL3theme);
		Control::Audio->UnloadSample(aTrackL3ho);
		Control::Audio->UnloadSample(aTrackL3puzzle);


		//Ambiance

		//Sounds
		Control::Audio->UnloadSample(aSFXDanger0);						
		Control::Audio->UnloadSample(aSFXPain);		

		//unload samples with/without english build
		CTexFactory::Instance->FreeLevelImages(Level_3);
		Control::Audio->IsLevelAssetsLoaded[Level_3] = false;
	}
}

void GameFlow::UnloadLevel4Assets()
{
	if( Control::Audio->IsLevelAssetsLoaded[Level_4] )
	{
		Control::Audio->UnloadSample(aTrackL4theme);
		Control::Audio->UnloadSample(aTrackL4ho);
		Control::Audio->UnloadSample(aTrackL4puzzle);
		//unload samples with/without english build
		CTexFactory::Instance->FreeLevelImages(Level_4);
		Control::Audio->IsLevelAssetsLoaded[Level_4] = false;
	}
}

void GameFlow::UnloadLevel5Assets()
{
	if( Control::Audio->IsLevelAssetsLoaded[Level_5] )
	{

		Control::Audio->UnloadSample(aTrackL5theme);
		Control::Audio->UnloadSample(aTrackL5ho);
		Control::Audio->UnloadSample(aTrackL5puzzle);
		//unload samples with/without english build
		CTexFactory::Instance->FreeLevelImages(Level_5);
		Control::Audio->IsLevelAssetsLoaded[Level_5] = false;
	}
}

void GameFlow::UnloadLevel6Assets()
{
	if( Control::Audio->IsLevelAssetsLoaded[Level_6] )
	{
		//unload samples with/without english build		
		Control::Audio->UnloadSample(aTrackL6theme);
		Control::Audio->UnloadSample(aTrackL6ho);
		Control::Audio->UnloadSample(aTrackL6puzzle);
		Control::Audio->UnloadSample(aTrackL6s3Puzzle);
		CTexFactory::Instance->FreeLevelImages(Level_6);
		Control::Audio->IsLevelAssetsLoaded[Level_6] = false;
	}
}

void GameFlow::UnloadLevel7Assets()
{
	if( Control::Audio->IsLevelAssetsLoaded[Level_7] )
	{

		Control::Audio->UnloadSample(aTrackL7theme);
		Control::Audio->UnloadSample(aTrackL7ho);
		Control::Audio->UnloadSample(aTrackL7puzzle);
		Control::Audio->UnloadSample(aTrackL7s5Theme);
		//unload samples with/without english build		
		CTexFactory::Instance->FreeLevelImages(Level_7);
		Control::Audio->IsLevelAssetsLoaded[Level_7] = false;
	}
}

void GameFlow::UnloadLevel8Assets()
{
	if( Control::Audio->IsLevelAssetsLoaded[Level_8] )
	{

		Control::Audio->UnloadSample(aTrackL8theme);
		Control::Audio->UnloadSample(aTrackL8ho);
		Control::Audio->UnloadSample(aTrackL8puzzle);
		//unload samples with/without english build		
		CTexFactory::Instance->FreeLevelImages(Level_8);
		Control::Audio->IsLevelAssetsLoaded[Level_8] = false;
	}
}

void GameFlow::UnloadAllAssets()
{
	UnloadBufferLvlInvHOObj();
	UnloadLevel1Assets();

	#ifndef _DEMOBUILD
		#ifdef K_ANDROID
			if(GFApp->isUnlocked()) // If full version 
			{
		#endif
			UnloadLevel2Assets();
			UnloadLevel3Assets();
			UnloadLevel4Assets();
			UnloadLevel5Assets();	
			UnloadLevel6Assets();
			UnloadLevel7Assets(); 
			UnloadLevel8Assets();
		#ifdef K_ANDROID
			}
		#endif
	#endif // !_DEMOBUILD

}

GameFlow::~GameFlow()
{
	GFInstance = NULL;	

	//no delete GFInfo control

	IsPakResLoadTerminate = false;


	delete PausedImage;

	for( int i = 0; i < 2; i++ )
		if( ThanksText[i] != NULL )
		{
			delete ThanksText[i];
			ThanksText[i] = NULL;
		}

		Cursor::Destroy();

	#ifdef _DEBUG
			SLOG::Destroy();  
	#endif // _DEBUG


		if( GreyAlphaImage != NULL )
		{
			delete GreyAlphaImage; GreyAlphaImage = NULL;
		}

		if( RTGraphic != NULL )
		{
			delete RTGraphic; RTGraphic = NULL;
		}

		if( CurrentSceneGraphic != NULL )
		{ delete CurrentSceneGraphic; CurrentSceneGraphic = NULL; }

		if( LoadingScene != NULL )
		{
			delete LoadingScene; LoadingScene = NULL;	 
		}	

		if( strikeGlowFX != NULL )
		{
			delete strikeGlowFX; strikeGlowFX = NULL;
		}

		SAFE_DELETE(RT_BgScn);


		UnloadStartCutscene();

		if(PrevSceneGraphic){delete PrevSceneGraphic;PrevSceneGraphic = NULL;}
		if(NewSceneGraphic){delete NewSceneGraphic;NewSceneGraphic = NULL;}


}

void GameFlow::WaitTillPakResLoadDone()
{
	if( IsPakResLoadInited  )
	{ //No thread. direct loading!!
		LoadingFile = 0;
		nPostUpdates = 50;
		LoadingScene->Show(true);
		//Control::Audio->PlaySample(aSFXMagic1,false);
		LoaderGlowBar->ForcePlayAnimation(0);

		//SLOG::LogLoadingStartTime();
		while( !IsPakResLoadTerminate )
		{
			LoadPakRes();//load one res at a time
			GFApp->Window->processEvents();//avoid irresponsive loop
		}
		InitGFInfoControl();

		//SLOG::LogLoadingEndTime("LoadPakRes");
		KMiscTools::pause(1500,true);
		LoadingScene->Show(false);
	}		
}

//to do - check mac compatibility	
void GameFlow::CheckAvailablePhyMemory()
{
	#ifdef K_WIN32
		{
			MEMORYSTATUS	memStatus;
			char LogStr[100];
			GlobalMemoryStatus(&memStatus);		
			availablePhyMem = (int)(memStatus.dwAvailPhys/1048576.0f);	
			sprintf(LogStr,"Physical mem available %d MB",availablePhyMem);

			K_LOG(LogStr);
			//availablePhyMem = MAX_16BIT_LOWEND_PEAKMEMORY;

			//Not enough mem to load truecolor(24 bit) graphics => force 16 bit!!
			IsForceTextures16Bit = ( availablePhyMem >= MAX_16BIT_LOWEND_PEAKMEMORY && availablePhyMem < MAX_LOWEND_PEAKMEMORY );
			if( availablePhyMem < MAX_16BIT_LOWEND_PEAKMEMORY )//Can't find enough mem to textures load!!
			{
				KMiscTools::messageBox((const char*)FileSystem::ws2s(LWMMRY_WStr).c_str(), (const char*)FileSystem::ws2s(GWRNG_0_WStr).c_str() );					
				/*	  
				#ifdef  _DEMOBUILD
				KMiscTools::messageBox((const char*)L"Low Memory Error!", (const char*)L"'Darkarta' trial needs atleast 500 MB memory! Please quit all other running applications or install additional memory!");
				#else
				KMiscTools::messageBox((const char*)L"Low Memory Error!", (const char*)L"'Darkarta' needs atleast 500 MB memory! Please quit all other running applications or install additional memory!");
				#endif
				*/

				exit(0);
			}
		}
	#else
		availablePhyMem = FULL_GAMECACHE_MEMORY;//Since macs are assumed to have good memory!!
	#endif	
	IsFullCacheMem = ( availablePhyMem >= FULL_GAMECACHE_MEMORY );
	IsPartialCacheMem = ( availablePhyMem >= PARTIAL_GAMECACHE_MEMORY );

}

#ifdef JAPANESE_BUILD
const char* GameFlow::JapInventoryDescrips( int rDescId )
{
	switch( rDescId )
	{
	default: return "";
		//Level 1 - 24
		//Change IDs as required
	case eInv_L1S1_Bag :return StringTable::getString("ID_0");break;
	}
}
#endif

void GameFlow::UnloadLevel1Assets()
{
	if( Control::Audio->IsLevelAssetsLoaded[Level_1] )
	{
		Control::Audio->UnloadSample(aTrackL1Broken);
		Control::Audio->UnloadSample(aTrackL1Mansion);
		Control::Audio->UnloadSample(aTrackL1Trapped);
		Control::Audio->UnloadSample(aTrackL1EscapePlan);
		Control::Audio->UnloadSample(aTrackL1Puzzle);

		CTexFactory::Instance->FreeImage("UI\\HUD\\HUD.PNG");
		CTexFactory::Instance->FreeImage("INV\\L1\\INV.PNG");
		CTexFactory::Instance->FreeImage("MAPDIA\\DIARYBG.jpa");
		CTexFactory::Instance->FreeImage("MNU\\IGM\\IGM.PNG");

		//Ambiance
		Control::Audio->UnloadSample(aAmbBeachLoop);

		//Sounds

		Control::Audio->UnloadSample(aSFXDanger0);						
		Control::Audio->UnloadSample(aSFXPain);


		Control::Audio->UnloadSample(aSFXL1S3P1PowerOff);
		Control::Audio->UnloadSample(aSFXL1S3P1PowerOn);

		Control::Audio->UnloadSample(aSFXL1ThunderV1);
		Control::Audio->UnloadSample(aSFXL1ThunderV2);


		//Unloaded in FreeLevelImage - avoid multiple search for deletion!!


		CTexFactory::Instance->FreeLevelImages(Level_1);
		Control::Audio->IsLevelAssetsLoaded[Level_1] = false;		
	}
}

void GameFlow::LvlAssetsLoader( )
{
	//Load ALL INV OBJECTS IN CURRENT LEVEL
	nPostUpdates++; UnloadBufferLvlInvHOObj();//unload previous
	LoadBufferLvlInvHOObj(ProfileSystem::Instance->ActiveProfile.CurrentLevel); PostUpdate(1);

	//Load the HUD


	//Load currentlevel and unload prevlevel sounds!!
	switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
	{
		case Level_1: if( IsPartialCacheMem )						  
						nPostUpdates += 32+0; //6 for level
					else
						nPostUpdates += 32;						  
			break; 
		case Level_2: if( IsPartialCacheMem )						  
						nPostUpdates += 18+0;
					else
						nPostUpdates += 18;break;
		case Level_3: if( IsPartialCacheMem )						  
						nPostUpdates += 18+0;
					else
						nPostUpdates += 18;break;			
		case Level_4: if( IsPartialCacheMem )						  
						nPostUpdates += 18+0;
					else
						nPostUpdates += 18;break;	
		case Level_5: if( IsPartialCacheMem )						  
						nPostUpdates += 18+0;
					else
						nPostUpdates += 18;break;
		case Level_6: if( IsPartialCacheMem )						  
						nPostUpdates += 18+0;
					else
						nPostUpdates += 18;break;	
		case Level_7: if( IsPartialCacheMem )						  
						nPostUpdates += 18+0;
					else
						nPostUpdates += 18;break;
		case Level_8: if( IsPartialCacheMem )						  
						nPostUpdates += 18+0;
					else
						nPostUpdates += 18;break;
	}		
	if( !IsLvlCommonTracksLoaded )
	{
		nPostUpdates += 1;//1 tracks 
		IsLvlCommonTracksLoaded = true;
	}

	InfoBanner::CreateLevel(ProfileSystem::Instance->ActiveProfile.CurrentLevel + 1);

	Control::Audio->LoadLevelSFXList(ProfileSystem::Instance->ActiveProfile.CurrentLevel); PostUpdate(1);
	//GameTextManager::GetInstance()->LoadBannerForLevel(ProfileSystem::Instance->ActiveProfile.CurrentLevel);
	#ifdef K_ANDROID
		if(!GFApp->isUnlocked()) // if free version
		{
			switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
				{
					case Level_1:/* Level 1 Load */ 
						//Music 
						//not looping

						//Composer tracks!! 
						Control::Audio->LoadSample(aTrackL1Broken, AudioTrack,true);PostUpdate(1);
						Control::Audio->LoadSample(aTrackL1Mansion, AudioTrack,true);PostUpdate(1);
						Control::Audio->LoadSample(aTrackL1Trapped, AudioTrack,true);PostUpdate(1);
						Control::Audio->LoadSample(aTrackL1EscapePlan, AudioTrack,true);PostUpdate(1);
						Control::Audio->LoadSample(aTrackL1Puzzle, AudioTrack,true);PostUpdate(1);
						Control::Audio->LoadSample(aTrackL1Hunted, AudioTrack,true);PostUpdate(1);

						//Common Images
						CTexFactory::Instance->LoadImg("UI\\HUD\\HUD.PNG",false,true,true);PostUpdate(1);
						PostUpdate(1);
						CTexFactory::Instance->LoadImg("MNU\\IGM\\IGM.PNG",false,true,true);PostUpdate(1);

						//Images
						CTexFactory::Instance->LoadImg("INV\\L1\\INV.PNG",false,true,true);PostUpdate(1);
						CTexFactory::Instance->LoadImg("MAPDIA\\DIARYBG.jpa",false,true,true);PostUpdate(1);

						//Ambiance 
						PostUpdate(1);

						Control::Audio->LoadSample(aAmbRain,AudioAmb,true);PostUpdate(1);										
						Control::Audio->LoadSample(aAmbBeachLoop,AudioAmb,true);PostUpdate(1);
						PostUpdate(1);
						PostUpdate(1);

						//Sounds			


						Control::Audio->LoadSample(aSFXDanger0,AudioSFX);PostUpdate(1); 			
						Control::Audio->LoadSample(aSFXPain,AudioSFX);PostUpdate(1); 	
						PostUpdate(1);

						PostUpdate(1);

						Control::Audio->LoadSample(aSFXL1S3P1PowerOff,AudioSFX);
						Control::Audio->LoadSample(aSFXL1S3P1PowerOn,AudioSFX);
						PostUpdate(1);
						PostUpdate(1);
						PostUpdate(1);		

						Control::Audio->LoadSample(aSFXL1ThunderV1,	AudioSFX);
						Control::Audio->LoadSample(aSFXL1ThunderV2,	AudioSFX);PostUpdate(1);

						PostUpdate(1);


						//24
						if( IsPartialCacheMem )//avoid inbetween loader +24
						{
							//CTexFactory::Instance->LoadImg("LV1\\FYD\\LITBG.PNG",false,true);PostUpdate(1);
						}
						break;

		
				}		
		}
		else
		{
			switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
			{
				case Level_1:/* Level 1 Load */ 
					//Music 
					//not looping

					//Composer tracks!! 
					Control::Audio->LoadSample(aTrackL1Broken, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL1Mansion, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL1Trapped, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL1EscapePlan, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL1Puzzle, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL1Hunted, AudioTrack,true);PostUpdate(1);

					//Common Images
					CTexFactory::Instance->LoadImg("UI\\HUD\\HUD.PNG",false,true,true);PostUpdate(1);
					PostUpdate(1);
					CTexFactory::Instance->LoadImg("MNU\\IGM\\IGM.PNG",false,true,true);PostUpdate(1);

					//Images
					CTexFactory::Instance->LoadImg("INV\\L1\\INV.PNG",false,true,true);PostUpdate(1);
					CTexFactory::Instance->LoadImg("MAPDIA\\DIARYBG.jpa",false,true,true);PostUpdate(1);

					//Ambiance 
					PostUpdate(1);

					Control::Audio->LoadSample(aAmbRain,AudioAmb,true);PostUpdate(1);										
					Control::Audio->LoadSample(aAmbBeachLoop,AudioAmb,true);PostUpdate(1);
					PostUpdate(1);
					PostUpdate(1);

					//Sounds			


					Control::Audio->LoadSample(aSFXDanger0,AudioSFX);PostUpdate(1); 			
					Control::Audio->LoadSample(aSFXPain,AudioSFX);PostUpdate(1); 	
					PostUpdate(1);

					PostUpdate(1);

					Control::Audio->LoadSample(aSFXL1S3P1PowerOff,AudioSFX);
					Control::Audio->LoadSample(aSFXL1S3P1PowerOn,AudioSFX);
					PostUpdate(1);
					PostUpdate(1);
					PostUpdate(1);		

					Control::Audio->LoadSample(aSFXL1ThunderV1,	AudioSFX);
					Control::Audio->LoadSample(aSFXL1ThunderV2,	AudioSFX);PostUpdate(1);

					PostUpdate(1);


					//24
					if( IsPartialCacheMem )//avoid inbetween loader +24
					{
						//CTexFactory::Instance->LoadImg("LV1\\FYD\\LITBG.PNG",false,true);PostUpdate(1);
					}
					break;
				case Level_2://unload prev level
					//load current level
					//CTexFactory::Instance->LoadImg("INV\\L1\\INV.PNG",false,true,true);
					//Composer tracks!! 
					Control::Audio->LoadSample(aTrackLevel2, AudioTrack,true);PostUpdate(1);	
					Control::Audio->LoadSample(aTrackL2theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL2theme1, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL2ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL2puzzle,AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL2s7Puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);		

					PostUpdate(1);
					//Sounds			
					Control::Audio->LoadSample(aSFXPain, AudioSFX);	PostUpdate(1);
					Control::Audio->LoadSample(aSFXDanger0,AudioSFX);PostUpdate(1);		
					//18
					if( IsPartialCacheMem )//avoid inbetween loader +18
					{
						//CTexFactory::Instance->LoadImg("LV1\\FYD\\LITBG.PNG",false,true);PostUpdate(1);
					}
					break;			
				case Level_3://unload prev level
					//load current level

					//CTexFactory::Instance->LoadImg("INV\\L3\\INV.PNG",false,true,true);PostUpdate(1);

					//Composer tracks!! 

					Control::Audio->LoadSample(aTrackL3theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL3ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL3puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);		

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds			
					Control::Audio->LoadSample(aSFXPain, AudioSFX);	PostUpdate(1);
					Control::Audio->LoadSample(aSFXDanger0,AudioSFX);PostUpdate(1);

					//18
					if( IsPartialCacheMem )//avoid inbetween loader +18
					{
						//CTexFactory::Instance->LoadImg("LV1\\FYD\\LITBG.PNG",false,true);PostUpdate(1);
					}
					break;
				case Level_4:

					Control::Audio->LoadSample(aTrackL4theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL4ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL4puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds
					break;  
				case Level_5://unload prev level
					//load current level

					//CTexFactory::Instance->LoadImg("INV\\L3\\INV.PNG",false,true,true);PostUpdate(1);

					//Composer tracks!! 

					Control::Audio->LoadSample(aTrackL5theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL5ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL5puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds			
					Control::Audio->LoadSample(aSFXPain, AudioSFX);	PostUpdate(1);
					Control::Audio->LoadSample(aSFXDanger0,AudioSFX);PostUpdate(1);

					//18
					if( IsPartialCacheMem )//avoid inbetween loader +18
					{
						//CTexFactory::Instance->LoadImg("LV1\\FYD\\LITBG.PNG",false,true);PostUpdate(1);
					}
					break;
				case Level_6:

					Control::Audio->LoadSample(aTrackL6theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL6ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL6puzzle,AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL6s3Puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds
					break;  
				case Level_7:

					Control::Audio->LoadSample(aTrackL7theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL7ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL7puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds
					break;  
				case Level_8:

					Control::Audio->LoadSample(aTrackL8theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL8ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL8puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds
					break;  
			}		
		}	
	#else
		switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
		{
			case Level_1:/* Level 1 Load */ 
				//Music 
				//not looping

				//Composer tracks!! 
				Control::Audio->LoadSample(aTrackL1Broken, AudioTrack,true);PostUpdate(1);
				Control::Audio->LoadSample(aTrackL1Mansion, AudioTrack,true);PostUpdate(1);
				Control::Audio->LoadSample(aTrackL1Trapped, AudioTrack,true);PostUpdate(1);
				Control::Audio->LoadSample(aTrackL1EscapePlan, AudioTrack,true);PostUpdate(1);
				Control::Audio->LoadSample(aTrackL1Puzzle, AudioTrack,true);PostUpdate(1);
				Control::Audio->LoadSample(aTrackL1Hunted, AudioTrack,true);PostUpdate(1);

				//Common Images
				CTexFactory::Instance->LoadImg("UI\\HUD\\HUD.PNG",false,true,true);PostUpdate(1);
				PostUpdate(1);
				CTexFactory::Instance->LoadImg("MNU\\IGM\\IGM.PNG",false,true,true);PostUpdate(1);

				//Images
				CTexFactory::Instance->LoadImg("INV\\L1\\INV.PNG",false,true,true);PostUpdate(1);
				CTexFactory::Instance->LoadImg("MAPDIA\\DIARYBG.jpa",false,true,true);PostUpdate(1);

				//Ambiance 
				PostUpdate(1);

				Control::Audio->LoadSample(aAmbRain,AudioAmb,true);PostUpdate(1);										
				Control::Audio->LoadSample(aAmbBeachLoop,AudioAmb,true);PostUpdate(1);
				PostUpdate(1);
				PostUpdate(1);

				//Sounds			


				Control::Audio->LoadSample(aSFXDanger0,AudioSFX);PostUpdate(1); 			
				Control::Audio->LoadSample(aSFXPain,AudioSFX);PostUpdate(1); 	
				PostUpdate(1);

				PostUpdate(1);

				Control::Audio->LoadSample(aSFXL1S3P1PowerOff,AudioSFX);
				Control::Audio->LoadSample(aSFXL1S3P1PowerOn,AudioSFX);
				PostUpdate(1);
				PostUpdate(1);
				PostUpdate(1);		

				Control::Audio->LoadSample(aSFXL1ThunderV1,	AudioSFX);
				Control::Audio->LoadSample(aSFXL1ThunderV2,	AudioSFX);PostUpdate(1);

				PostUpdate(1);


				//24
				if( IsPartialCacheMem )//avoid inbetween loader +24
				{
					//CTexFactory::Instance->LoadImg("LV1\\FYD\\LITBG.PNG",false,true);PostUpdate(1);
				}
				break;

			#ifndef _DEMOBUILD
				case Level_2://unload prev level
					//load current level
					//CTexFactory::Instance->LoadImg("INV\\L1\\INV.PNG",false,true,true);
					//Composer tracks!! 
					Control::Audio->LoadSample(aTrackLevel2, AudioTrack,true);PostUpdate(1);	
					Control::Audio->LoadSample(aTrackL2theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL2theme1, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL2ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL2puzzle,AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL2s7Puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);		

					PostUpdate(1);
					//Sounds			
					Control::Audio->LoadSample(aSFXPain, AudioSFX);	PostUpdate(1);
					Control::Audio->LoadSample(aSFXDanger0,AudioSFX);PostUpdate(1);		
					//18
					if( IsPartialCacheMem )//avoid inbetween loader +18
					{
						//CTexFactory::Instance->LoadImg("LV1\\FYD\\LITBG.PNG",false,true);PostUpdate(1);
					}
					break;			
				case Level_3://unload prev level
					//load current level

					//CTexFactory::Instance->LoadImg("INV\\L3\\INV.PNG",false,true,true);PostUpdate(1);

					//Composer tracks!! 

					Control::Audio->LoadSample(aTrackL3theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL3ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL3puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);		

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds			
					Control::Audio->LoadSample(aSFXPain, AudioSFX);	PostUpdate(1);
					Control::Audio->LoadSample(aSFXDanger0,AudioSFX);PostUpdate(1);

					//18
					if( IsPartialCacheMem )//avoid inbetween loader +18
					{
						//CTexFactory::Instance->LoadImg("LV1\\FYD\\LITBG.PNG",false,true);PostUpdate(1);
					}
					break;
				case Level_4:

					Control::Audio->LoadSample(aTrackL4theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL4ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL4puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds
					break;  
				case Level_5://unload prev level
					//load current level

					//CTexFactory::Instance->LoadImg("INV\\L3\\INV.PNG",false,true,true);PostUpdate(1);

					//Composer tracks!! 

					Control::Audio->LoadSample(aTrackL5theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL5ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL5puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds			
					Control::Audio->LoadSample(aSFXPain, AudioSFX);	PostUpdate(1);
					Control::Audio->LoadSample(aSFXDanger0,AudioSFX);PostUpdate(1);

					//18
					if( IsPartialCacheMem )//avoid inbetween loader +18
					{
						//CTexFactory::Instance->LoadImg("LV1\\FYD\\LITBG.PNG",false,true);PostUpdate(1);
					}
					break;
				case Level_6:

					Control::Audio->LoadSample(aTrackL6theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL6ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL6puzzle,AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL6s3Puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds
					break;  
				case Level_7:

					Control::Audio->LoadSample(aTrackL7theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL7ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL7puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds
					break;  
				case Level_8:

					Control::Audio->LoadSample(aTrackL8theme, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL8ho, AudioTrack,true);PostUpdate(1);
					Control::Audio->LoadSample(aTrackL8puzzle,AudioTrack,true);PostUpdate(1);
					PostUpdate(1);												
					//Ambiance 
					PostUpdate(1);

					PostUpdate(1);										
					PostUpdate(1);
					//Sounds
					break;  
			#endif // !_DEMOBUILD
		}	
	#endif	
}

void GameFlow::SetPopupTween( int StartX, int StartY, int EndX, int EndY )
{
	PStartX = StartX;
	PStartY = StartY;
	PEndX = EndX;
	PEndY = EndY;
}	

void GameFlow::ReturnToMenu()				
{	
	Control::Audio->TurnOffLvlMscTracks();
	UnloadAllAssets();//As we are leaving to menu
	Flow = fRootMenu;	
	ProfileSystem::Instance->ActiveProfile._IsFromGame = true;	
	IsInsideLevel = false;//cross transition and similar effects!!	
	#ifdef TOUCH_UI
		Cursor::SetInMenu(true);
	#endif
	AddControl(new MenuSystem());			
}

void GameFlow::InitScnLoadingSpr()
{
	for(int i = 0; i < MAX_LD_FRAMES; i++)
    {
        lpLoadingSpr[i] = KPTK::createKGraphic();
        lpLoadingSpr[i]->loadPicture(
                KMiscTools::makeFilePath(std::string("Assets\\UI\\SCNLOADING\\"+std::to_string(i)+".png").c_str()),
                false, true);
    }
    lpLoadingScnBG = KPTK::createKGraphic();
    lpLoadingScnBG->loadPicture(
            KMiscTools::makeFilePath(std::string("Assets\\UI\\SCNLOADING\\BG.jpg").c_str()),
            false, true);
}

#ifdef K_ANDROID
	unsigned int * GameFlow::overriddenGetBackBuffer(long *gnBWidth,long *gnBHeight, long nWidth, long nHeight, long nCropX, long nCropY, long nCropWidth, long nCropHeight)
	{	
		//unsigned char *lpBackBufferPixels, *lpCurBackBufferPixels, *lpEncodedImage = NULL;
		unsigned char *lpBackBufferPixels, *lpCurBackBufferPixels;
		unsigned int *lpImagePixels, *lpCurImagePixels;
		long i, j, nScreenX, nScreenY, nScreenWidth = _nActualWidth, nScreenHeight = _nActualHeight;

		if (!nWidth) nWidth = _nWindowWidth;
		if (!nHeight) nHeight = _nWindowHeight;
		
		nScreenX = nCropX * _nActualWidth / _nWindowWidth;
		nScreenY = nCropY * _nActualHeight / _nWindowHeight;
		if (!nCropWidth) nCropWidth = _nWindowWidth;
		if (!nCropHeight) nCropHeight = _nWindowHeight;
		nScreenWidth = nCropWidth * _nActualWidth / _nWindowWidth;
		nScreenHeight = nCropHeight * _nActualHeight / _nWindowHeight;
		
		if (!nWidth) nWidth = _nWindowWidth;
		if (!nHeight) nHeight = _nWindowHeight;

		/* Grab backbuffer */
			lpBackBufferPixels = new unsigned char [nScreenWidth * nScreenHeight * 4];
			glReadPixels (nScreenX, _nActualHeight - nScreenHeight - nScreenY, nScreenWidth, nScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, lpBackBufferPixels);
			*gnBWidth = nScreenWidth;
			*gnBHeight = nScreenHeight;

			/* Convert to an ABGR array and flip image back up the right way */
				lpImagePixels = new unsigned int [nScreenWidth * nScreenHeight];
				lpCurImagePixels = lpImagePixels;

				for (j = 0; j < nScreenHeight; j++) {
					lpCurBackBufferPixels = lpBackBufferPixels + (nScreenHeight - j - 1) * nScreenWidth * 4;
					for (i = 0; i < nScreenWidth; i++) {
						*lpCurImagePixels++ = lpCurBackBufferPixels[0] | (lpCurBackBufferPixels[1] << 8) | (lpCurBackBufferPixels[2] << 16) | 0xff000000;
						lpCurBackBufferPixels += 4;
					}
				}
				delete [] lpBackBufferPixels;
				return lpImagePixels;
	}
	bool GameFlow::overriddenSaveBackBuffer(const char *lpszFilename, long nImageFormat, long nWidth, long nHeight, long nCropX, long nCropY, long nCropWidth, long nCropHeight)				
	{	
		unsigned char *lpBackBufferPixels, *lpCurBackBufferPixels, *lpEncodedImage = NULL;
		unsigned int *lpImagePixels, *lpCurImagePixels;
		long i, j, nScreenX, nScreenY, nScreenWidth = _nActualWidth, nScreenHeight = _nActualHeight;
		size_t nEncodedImageSize;
		KImage bbImage;
		bool bSuccess;
		FILE *f;
		
		if (!nWidth) nWidth = _nWindowWidth;
		if (!nHeight) nHeight = _nWindowHeight;
		
		nScreenX = nCropX * _nActualWidth / _nWindowWidth;
		nScreenY = nCropY * _nActualHeight / _nWindowHeight;
		if (!nCropWidth) nCropWidth = _nWindowWidth;
		if (!nCropHeight) nCropHeight = _nWindowHeight;
		nScreenWidth = nCropWidth * _nActualWidth / _nWindowWidth;
		nScreenHeight = nCropHeight * _nActualHeight / _nWindowHeight;
		
		if (!nWidth) nWidth = _nWindowWidth;
		if (!nHeight) nHeight = _nWindowHeight;

		/* Grab backbuffer */
			lpBackBufferPixels = new unsigned char [nScreenWidth * nScreenHeight * 4];
			if (nImageFormat == K_IMAGE_BMP)
				glReadPixels (nScreenX, _nActualHeight - nScreenHeight - nScreenY, nScreenWidth, nScreenHeight, GL_RGB, GL_UNSIGNED_BYTE, lpBackBufferPixels);
			else
				glReadPixels (nScreenX, _nActualHeight - nScreenHeight - nScreenY, nScreenWidth, nScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, lpBackBufferPixels);
		/* Convert to an ABGR array and flip image back up the right way */
				lpImagePixels = new unsigned int [nScreenWidth * nScreenHeight];
				lpCurImagePixels = lpImagePixels;

				for (j = 0; j < nScreenHeight; j++) {
				lpCurBackBufferPixels = lpBackBufferPixels + (nScreenHeight - j - 1) * nScreenWidth * 4;
				for (i = 0; i < nScreenWidth; i++) {
					*lpCurImagePixels++ = lpCurBackBufferPixels[0] | (lpCurBackBufferPixels[1] << 8) | (lpCurBackBufferPixels[2] << 16) | 0xff000000;
					lpCurBackBufferPixels += 4;
				}
				}

				delete [] lpBackBufferPixels;

				bbImage.setPixels (nScreenWidth, nScreenHeight, false, lpImagePixels);
				bbImage.resampleImage (nWidth, nHeight);

				bSuccess = bbImage.encodeImage (nImageFormat, lpEncodedImage, nEncodedImageSize,75);

		if (!bSuccess || !lpEncodedImage) {
			/* Failed to encode */
			K_LOG ("OpenGL: failed to encode image in format %d for saveBackBuffer", nImageFormat);
			return false;
		}

				bbImage.freeImage ();

		/* Write to file */
		bSuccess = false;
				f = fopen (lpszFilename, "wb");

				if (f) {
				size_t nWritten;
				
				nWritten = fwrite (lpEncodedImage, 1, nEncodedImageSize, f);
				fclose (f);
				if (nWritten == nEncodedImageSize) bSuccess = true;
				}

		
		/* Success */
		delete [] lpEncodedImage;
		lpEncodedImage = NULL;
		return true;		
	}
#endif


