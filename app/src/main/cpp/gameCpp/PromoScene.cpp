//====================================
//	DQFYLH
//	HO System
//	Level 1 - PromoScene
//====================================

#include "PromoScene.h"
#include "Hud.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "TextureFactory.h"
#include "MenuSystem.h"


PromoScene::PromoScene()
{
	Visible = false;
	IsVideo1Loaded = false; //first step to be done to avoid render deadlock
	IsPromoVideoPlaying = false;
	
	//Scene Changes
	GFInstance->PostUpdate(2);

	
	SkipImage   = NULL;
	SkipImage   = CTexFactory::Instance->GetImage("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\CORE.PNG");
	SkipImage->setTextureQuality(true);

	IsSkipMouseOver = false;
	SkipShowTimer = 0;

	//Video 1 - Outro Video
	lpPromoCutsceneVideo = NULL;

	lpPromoCutsceneSndStream = NULL;

	#ifdef K_WIN32
	{
		if( !KSound::g_bErrSoundDeviceInit )
			lpPromoCutsceneSndStream = new KSound;
	}
	#else
		lpPromoCutsceneSndStream = new KSound;
	#endif

		
	Control::Audio->TurnOffLvlMscTracks();//turns off music-snd carryover....

	lpPromoCutsceneVideo = new KVideo;
	//#ifndef K_MACOS
		bool bSuccess = lpPromoCutsceneVideo->openVideo(KMiscTools::makeFilePath("Data\\PROMO.OGV"),true,NULL,lpPromoCutsceneSndStream,false,true);
	/*#else
		bool bSuccess = lpPromoCutsceneVideo->openVideo(KMiscTools::makeFilePath("PROMO.OGV"),true,NULL,lpPromoCutsceneSndStream,false,true);
	#endif
     */
	if (!bSuccess) 
	{
		#ifdef K_WIN32
				KMiscTools::messageBox ("Video loading error!", "Couldn't open Promo Video");	
		#else
				KMiscTools::messageBox ((const char*)"Video loading error!", (const char*)"Couldn't open Promo Video");	
		#endif
	  exit(0);
	}
	IsVideo1Loaded = true;
	if( lpPromoCutsceneSndStream != NULL )
		lpPromoCutsceneSndStream->setVolume(ProfileSystem::Instance->ActiveProfile.VoiceVol);
	SetZ(11000);

	Control::Audio->ClearUnusedTracks();
	
	GFInstance->PostUpdate(1);	
	

	IsPromoVideoPlaying = true;
	Visible = true;	//To render anything with renderlistentity especially Video
}


void PromoScene::Update()
{
	
	Cursor::SetMode(CA_Normal);

	if(IsPromoVideoPlaying)
	{
		//Handle Skip Video
		if(SkipShowTimer>4.0f)
		{
			CPoint mousePos = Control::Input->Mpos();
			
				IsSkipMouseOver = ( mousePos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) && 
					mousePos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
					mousePos.y >= SKIPIMAGE_Y &&
					mousePos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) + SKIPIMAGE_WIDTH) );

			if( IsSkipMouseOver )  
			{
				if( Control::Input->LBclicked() )
				{
					Control::Audio->PlaySample(aSFXClick);												

					ExitGame();	
				}
				else 
					Cursor::Show(false);			
			}
			else //Cursor not over skip region!!
				Cursor::Show(true);
		}
		else
			SkipShowTimer += 10*Control::LogicRate;
		return;
	}
	
}

void PromoScene::ExitGame()
{
	IsPromoVideoPlaying = false;
	if( lpPromoCutsceneSndStream != NULL )
		lpPromoCutsceneSndStream->stopStream();
	
	//char buffer[10]; 
	//K_LOG(_itoa(ProfileSystem::Instance->ActiveProfile.CurrentArea,buffer,10));
	//K_LOG(_itoa(ProfileSystem::Instance->ActiveProfile._PrevArea,buffer,10));
	ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile._PrevArea;//restore to old profile!!	
	SendMessage(ApplicationExit, ID, 0);	
}

void PromoScene::Render(const CPoint* pCam)
{  
   static int FrameCnt = 0;
   FrameCnt = (FrameCnt+1)%2;
	  
   if( IsVideo1Loaded && IsPromoVideoPlaying)
   {
	    if( FrameCnt  || !lpPromoCutsceneVideo->isVideoAvailable() )
		   lpPromoCutsceneVideo->readFrame();
		
       lpPromoCutsceneVideo->getKGraphic()->blitRect (0, 0, 1366, 768, 0, 0);

	   //Handle Skip Video
		if( SkipShowTimer> 4.0f)
		{
			SkipImage->blitAlphaRectF( 64,32,128,64,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);
			if( IsSkipMouseOver )  
			{
				SkipImage->blitAlphaRect( 192,0,256,64,(short)Control::Input->Mpos().x,(short)Control::Input->Mpos().y);//Cursor
			}
			else //Cursor not over skip region!!
			{
				SkipImage->blitAlphaRectF( 0,32,64,64,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);				
			}
		}

	   if(lpPromoCutsceneVideo->isEndReached() || (lpPromoCutsceneVideo->getTime() >= lpPromoCutsceneVideo->getTotalTime()-0.1f))
	   {
		   ProfileSystem::Instance->ActiveProfile.IsPromoCutscenePlayed  = true; 
		   Control::Audio->PlaySample(aSFXClick);
		   ExitGame();				   
	   }
   }
}

PromoScene::~PromoScene()
{
	IsVideo1Loaded = false;	

	if( SkipImage )
	{
		CTexFactory::Instance->FreeImage(&SkipImage);
		SkipImage = NULL;
	}
	delete lpPromoCutsceneVideo; lpPromoCutsceneVideo = NULL;

	if(lpPromoCutsceneSndStream!=NULL)
		delete lpPromoCutsceneSndStream;
}
