//====================================
// Framework
// Splash Series Control
//====================================

#include "Splash.h"
#include "IOStream.h"
#include "Resources.h"
#include "ProfileSystem.h"
#include "Application.h"
#include "TextureFactory.h"
#include "GameFlow.h"

//Really 19 secs in core2duo
#define FULL_LOADER_DELAY 8
#define PARTIAL_LOADER_DELAY 6

SplashSeries::SplashSeries()
{

#ifdef TOUCH_UI
	GFApp->ProjZoom = 1;
#endif
	TimeElapsed		= 0.0f;
	CurrentSplash	= 0;	
	for( int i = 0; i < MAX_SPLASHES; i++ )
		SplashImage[i]   = NULL;	

	SplashFadeInEnd = 1.0f;//0.5f;

	Control::Audio->Samples[aSFXMagic1]->SetVolume(80);
	//Control::Audio->Samples[aSFXLogoSplash]->SetVolume(80);
			
	if( GFInstance->IsFullCacheMem )
	{
		#ifdef THIRDPARTY_SPLASH 
			SplashLen  = FULL_LOADER_DELAY/3; 
		#else 
			SplashLen  = FULL_LOADER_DELAY/2;//ogv from pak needs 
		#endif
	}
	else //Needs 5 sec - partial loading time - effective 2.5 sec splashloader
	{
		#ifdef THIRDPARTY_SPLASH
			SplashLen = FULL_LOADER_DELAY/3;
		#else 
			SplashLen = FULL_LOADER_DELAY/2;//ogv from pak needs
		#endif
	}

	GFInstance->IsLowFPSMachine = GFApp->Window->getAccelerationCap();//video card or software processing 	
	if( GFInstance->IsLowFPSMachine )//LowEndMachine - todo //low end core2duo machine takes more time to load 
		SplashLen += 2.0f;
#ifdef K_iOS
    SplashLen += 0.6f; //iPad low end machine needs more loading time!!
    //Control::Audio->PlaySample(aSFXLogoSplash); //loading snd-lag issue(cinematic loader in bkgnd)
#endif
    
	SplashFadeOutStart = SplashLen - 1.0f;//0.5f;
	ProfileSystem::Instance->ActiveProfile._IsFromSplash = true;
	int iSplash = 0;
	
    
	#ifdef THIRDPARTY_SPLASH
		SplashImage[iSplash] = KPTK::createKGraphic ();
		bool bSuccess = SplashImage[iSplash]->loadPicture (KMiscTools::makeFilePath ("Splash.JPG"), true, true,true);
		if( !bSuccess )//Avoid crash!!
		{
			KMiscTools::messageBox((const char*)StringTable::getStr("ExErrTitle").c_str(),(const char*)StringTable::getStr("ExErrSplashLoad").c_str() );	
		//	KMiscTools::messageBox( (const char*)L"Error", (const char*)L"Unable to load Splash.JPG");
			exit(0);
		}

		if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
			SplashImage[iSplash]->setTextureQuality(true);	
		iSplash++;
	#endif

#ifdef	_PUBLISHER_BUILD
    // update the game with the right Splash and credits screen for PUB.JPG and CREDITS_2_1.JPG 
	// from the right folders in Publishers_assets
    // Tehre are make commands for that, use that make command after the data updates, and before making pak files.


	SplashImage[iSplash]  = CTexFactory::Instance->GetImage("SPL\\PUB.JPG");
    if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
        SplashImage[iSplash]->setTextureQuality(true);
    iSplash++;
    /*
        SplashImage[iSplash]  = CTexFactory::Instance->GetImage("SPL\\BF.JPG");
    if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		SplashImage[iSplash]->setTextureQuality(true);
          iSplash++;
	*/

#endif

	SplashImage[iSplash]  = CTexFactory::Instance->GetImage("SPL\\TF.PNG");
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		SplashImage[iSplash]->setTextureQuality(true);
	iSplash++;
	
}

SplashSeries::~SplashSeries()
{	
    
	Control::Audio->Samples[aSFXMagic1]->SetVolume(100);//reset for future
	//Control::Audio->Samples[aSFXLogoSplash]->SetVolume(100);//reset for future
	
    
	#ifdef THIRDPARTY_SPLASH
		delete SplashImage[0];//first 3rdparty splash must be unloaded!!
	#endif

	/*if( Control::Audio->IsPlaying(aSFXLogoSplash))
		Control::Audio->StopSample(aSFXLogoSplash);
	Control::Audio->UnloadSample(aSFXLogoSplash);*/	

	//we dont' need splash screens further!!	
	CTexFactory::Instance->FreeImage("SPL\\TF.PNG");
	#ifdef _PUBLISHER_BUILD
		CTexFactory::Instance->FreeImage("SPL\\PUB.JPG");
	/*
		#ifdef K_ANDROID
			CTexFactory::Instance->FreeImage("SPL\\G5.JPG");
		#else
			#ifdef _BIGFISH_BUILD
				CTexFactory::Instance->FreeImage("SPL\\BF.JPG");
			#elif _GAMEHOUSE_BUILD
				CTexFactory::Instance->FreeImage("SPL\\GH.JPG");
			#else
				CTexFactory::Instance->FreeImage("SPL\\BF.JPG");
			#endif
		#endif
	*/
	#endif



}

void SplashSeries::DrawSplash()
{
    GFApp->Window->setWorldView(GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, true);//clear prev scene!!
    if( TimeElapsed < SplashFadeInEnd )//Alpha 0..1 - 0.5 fading in time
    {
        //if( CurrentSplash > 0 )
            SplashImage[CurrentSplash]->blitAlphaRectFx(0,0,1366,768,0,0,0,1.0f,TimeElapsed);
        //else
        //    SplashImage[CurrentSplash]->blitRect(0,0,1366,768,0,0);
    }
    else if( TimeElapsed < SplashFadeOutStart )//2.0f sec splash wait!!
		SplashImage[CurrentSplash]->blitRect(0,0,1366,768,0,0);
	else//Alpha 1..0
		//- 0.5 fading out time
        SplashImage[CurrentSplash]->blitAlphaRectFx(0,0,1366,768,0,0,0,1.0f,(SplashLen-TimeElapsed));	
}

void SplashSeries::Update()
{
	
	if( ProfileSystem::Instance->ActiveProfile._IsSplashDone || GFInstance->IsPakLoading )
		return;		

	
	DrawSplash();
	//No render updations needed during this time!!
	if( TimeElapsed >= SplashFadeInEnd && TimeElapsed < SplashFadeOutStart )
	{
		GFInstance->IsSplashFullAlpha = true;
		if( !GFInstance->IsPakResLoadInited )
			 GFInstance->initPakResLoad();
		if( GFInstance->IsPakResLoadTerminate )
		{
			//forced min wait for splash screen
			//KMiscTools::pause(1500,true);
			
			for( int i = 0; i < 30; i++)
			{
				if( KInput::getLeftButtonState() == ISDOWN )//thread terminated - so can skip splash
				{
					HandleSplashOver();
					return;
				}
				KMiscTools::pause(50,true);//poll - to handle alt-tab , alt-ctrl-delete keycombinations
			}
			
		}
		else 
		{
            // Sac: Commenting all below Unused variables
			/*
             int GapMS = (int)((SplashFadeOutStart-TimeElapsed)*1000); //1500..0
			 unsigned long MaxGap = (unsigned long)K_MAX(1,GapMS);
            //No Thread - direct loading!!
             unsigned long StartTime = KMiscTools::getMilliseconds();
                unsigned long EndTime = StartTime;
		*/
             /*while( !GFInstance->IsPakResLoadTerminate )
				{
					GFInstance->LoadPakRes();
					EndTime = KMiscTools::getMilliseconds();
					if( (EndTime - StartTime) >= MaxGap )
						break;
				}*/
				if( KInput::getLeftButtonState() == ISDOWN )//thread terminated - so can skip splash
				{
					CurrentSplash++;
					if( CurrentSplash >= MAX_SPLASHES )
					{		
						HandleSplashOver();				
						return;
					}
					//HandleSplashOver();
					return;
				}

				KMiscTools::pause(1500,true);
                //Forced splashwait for uniform splash
    //            if( (EndTime - StartTime) < MaxGap )//GFInstance->IsPakResLoadTerminate 
				//{
				//KMiscTools::pause(MaxGap-(EndTime - StartTime),true);
				//	int nLoops = (MaxGap-(EndTime - StartTime))/30;
				//	for( int i = 0; i < nLoops; i++)
				//	{
				//		if( GFInstance->IsPakResLoadTerminate )							
				//			if( KInput::getLeftButtonState() == ISDOWN )//thread terminated - so can skip splash
				//			{
				//				HandleSplashOver();
				//				return;
				//			}							
				//		KMiscTools::pause(30,true);//poll - to handle alt-tab , alt-ctrl-delete keycombinations
				//	}
				//}
		}
		GFInstance->IsSplashFullAlpha = false;


		TimeElapsed = SplashFadeOutStart;
	}
	else //fadein or fadeout!!
	{
		TimeElapsed += Control::LogicRate;
		//since thread runs in parallel, better gamer see our promo than too much loaders later!!
		if( TimeElapsed >= SplashLen )
		{
			TimeElapsed = 0.0f;	
			CurrentSplash++;
			if( CurrentSplash >= MAX_SPLASHES )
			{		
				HandleSplashOver();				
				return;
			}

			if( !ProfileSystem::Instance->ActiveProfile.IsMuted )
			{
					/*if( Control::Audio->IsPlaying(aSFXLogoSplash))
						Control::Audio->StopSample(aSFXLogoSplash);
					Control::Audio->PlaySample(aSFXLogoSplash);	*/				
			}
			
		}
	}	
}

void SplashSeries::HandleSplashOver()
{
	ProfileSystem::Instance->ActiveProfile._IsSplashDone = true;			
	if( GFInstance->IsPakResLoadInited && !GFInstance->IsPakResLoadDone )
	{
		GFInstance->WaitTillPakResLoadDone();
		GFInstance->IsPakResLoadDone = true;
		//Control::Audio->PlaySample(aSFXMagic1,false);
	}
	GFApp->Window->setWorldView(GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, true);//clear prev scene!!
#if ( defined  K_iOS)
	KSysiPhone::dismissSplashScreen(0.0f,true); //save memory!!	
#endif
	Exit();					
}
