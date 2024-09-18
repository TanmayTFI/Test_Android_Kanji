//====================================
//	DQFYLH
//	HO System
//	Level 1 - HaveYouCutScene
//====================================

#include "HaveYouCutScene.h"
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
#include "AwardManager.h"


HaveYouCutScene::HaveYouCutScene()
{
	Visible = false;
	IsVideo1Loaded = false; //first step to be done to avoid render deadlock
	IsHaveYouVideoPlaying = false;

	//Scene Changes
	GFInstance->PostUpdate(2);

	CoreImage   = NULL;
	CoreImage   = CTexFactory::Instance->GetImage("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\CORE.PNG");
	CoreImage->setTextureQuality(true);

	templeOdoomTxt = NULL;
	templeOdoomTxt = CTexFactory::Instance->GetImage("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\VIDEO\\TEMPLEODOOM.PNG");
	templeOdoomTxt->setTextureQuality(true);

	SCNSndExp = new CHOScene();
	SCNSndExp->Load("UI\\SNDEXP\\SNDEXP.SCN");

	SndExpTxt = new CObject();
	SndExpTxt->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\SNDEXP.SPR", 725, 420, 9);
	SndExpTxt->SetUID("SndExpTxt");
	SCNSndExp->PushObject(SndExpTxt);

	SCNSndExp->SetZOff(2);

	vfxRain = new SuperFX("PRT\\SND\\Rain.INI", PointSystem::CreateCPoint(665,45,0), 32); 
	vfxRain->Start();
	vfxRain->AdvanceByTime(2.0f);
	vfxRainDrops = new SuperFX("PRT\\SND\\RainDrops.INI", PointSystem::CreateCPoint(737,623,0), 31); 
	vfxRainDrops->Start();
	vfxRainDrops->AdvanceByTime(2.0f);
	vfxLeaves = new SuperFX("PRT\\SND\\leaves.INI", PointSystem::CreateCPoint(1019,159,0), 30);
	vfxLeaves->Start();
	vfxLeaves->AdvanceByTime(2.0f);

	FgFeatherMesh = NULL;

	FgFeatherMesh = new MeshObject("UI\\SNDEXP\\PEACOCKFEATHERS.jpa", 5, 5);
	FgFeatherMesh->SetPosAndParams(0, 242, 0, 3, 0, 3);
	FgFeatherMesh->SetZ(6);

	SCNSndExp->GetObjectByID("cloudsmall")->SetBlendMode(eBlend_AlphaAdditive);
	SCNSndExp->GetObjectByID("cloudbig")->SetBlendMode(eBlend_AlphaAdditive);

	SCNSndExp->GetObjectByID("cloudbig")->SetAlpha(0.45f);
	SCNSndExp->GetObjectByID("cloudsmall")->SetAlpha(0.45f);

	MaskBg = SCNSndExp->GetObjectByID("Mask");
	MaskBg->SetXY(683,384);
	MaskBg->SetScale(2.0);
	MaskBg->SetZ(50);
	
	SCNSndExp->PushObject(FgFeatherMesh);
	
	sndSceneFadeInTime = 0;
	sndSceneFadeOutTime = 0;

	sndSceneIdleTime = 0;

	isSndSceneFadingOut = false;
	isSndSceneFadingIn = true;

	//Video 1 - Outro Video
	lpHaveYouCutsceneVideo = NULL;
	lpHaveYouCutsceneSndStream = NULL;

	#ifdef K_WIN32
	{
		if( !KSound::g_bErrSoundDeviceInit )
			lpHaveYouCutsceneSndStream = new KSound;
	}
	#else
		lpHaveYouCutsceneSndStream = new KSound;
	#endif

	lpHaveYouCutsceneVideo = new KVideo;
    //	#ifndef K_MACOS
		bool bSuccess = lpHaveYouCutsceneVideo->openVideo(KMiscTools::makeFilePath("Data\\HAVEYOU.OGV"),true,NULL,lpHaveYouCutsceneSndStream,false,true);
    /*	#else
		bool bSuccess = lpHaveYouCutsceneVideo->openVideo(KMiscTools::makeFilePath("HAVEYOU.OGV"),true,NULL,lpHaveYouCutsceneSndStream,false,true);
	#endif
    */
    if (!bSuccess)
	{
		#ifdef K_WIN32
			KMiscTools::messageBox ("Video loading error!", "Couldn't open HAVEYOU Video");	
		#else
			KMiscTools::messageBox ((const char*)"Video loading error!", (const char*)"Couldn't open HAVEYOU Video");	
		#endif
	  exit(0);
	}
	IsVideo1Loaded = true;
	if( lpHaveYouCutsceneSndStream != NULL )
		lpHaveYouCutsceneSndStream->setVolume(ProfileSystem::Instance->ActiveProfile.VoiceVol);
	SetZ(11000);

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	
	Control::Audio->LoadSample(aSFXSoundExperience,AudioTrack);


	//MUSIC_SFX
	
	
	GFInstance->PostUpdate(1);

	Control::Audio->PlaySample(aSFXSoundExperience,false);

	//Cursor not needed for this scene!!
	isMouseHover = false;
	isSkipped = false;

	isSkipShowing = false;

	Cursor::Show(true);

	isFPSCheckingActive = false;
	//if(!ProfileSystem::Instance->ActiveProfile.IsCheckedForLowSystem)
	{
		isFPSCheckingActive = true;
		Control::MasterTmr->IsCalculateFPS = true;
	}

	Totalfps = 0;
	fpsCount = 0;

	//IsHaveYouVideoPlaying = true;
	Visible = true;	//To render anything with renderlistentity especially Video

	time = 0;
	subIndex = 0;

	subMax = 6;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(30000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou6"));
}

void HaveYouCutScene::Update()
{
	mosPos = Control::Input->Mpos();

	isMouseHover = ( (mosPos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0))) && 
				mosPos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
				mosPos.y >= SKIPIMAGE_Y &&
				mosPos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) + SKIPIMAGE_WIDTH) );

	Cursor::SetMode(CA_Normal);
	if(IsHaveYouVideoPlaying)
	{
		if(isMouseHover && isSkipShowing)
		{
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked() && !isSkipped)
			{
				isSkipped = true;
				Control::Audio->PlaySample(aSFXClick);
				return;
			}
		}
	}
	else
	{

		if(isFPSCheckingActive)
		{
			Totalfps += Control::MasterTmr->FramesPerSecond;
			fpsCount += 1;
		}

		sndSceneIdleTime += Control::LogicRate;
		if(isSndSceneFadingIn)
		{
			sndSceneFadeInTime += Control::LogicRate;
			MaskBg->SetAlpha(1.0f-sndSceneFadeInTime*0.65f);
			if(sndSceneFadeInTime>1.5f)
			{
				MaskBg->SetAlpha(0);
				MaskBg->Show(false);				
				//SCNSndExp->GetObjectByID("PeacockFeathers")->SetEnable(false);

				isSndSceneFadingIn = false;
				return;
			}
		}

		SCNSndExp->GetObjectByID("cloudbig")->Pos.x -= Control::LogicRate*7.5f;
		SCNSndExp->GetObjectByID("cloudsmall")->Pos.x -= Control::LogicRate*5.0f;

		if(!isSndSceneFadingOut && sndSceneIdleTime > 7.0f)
		{
			isSndSceneFadingOut = true;

			MaskBg->SetAlpha(0);
			MaskBg->Show(true);
		}

		if(isSndSceneFadingOut)
		{
			sndSceneFadeOutTime += Control::LogicRate;
			MaskBg->SetAlpha(sndSceneFadeOutTime*0.65f);
			Control::Audio->Samples[aSFXSoundExperience]->SetVolume(Control::Audio->GlobalMusicVol - sndSceneFadeOutTime*0.65f*Control::Audio->GlobalMusicVol);
			//Control::Audio->Samples[aSFXSoundExperience]->SetVolume(1.0f-sndSceneFadeOutTime*0.65f*100.0f);
			if(sndSceneFadeOutTime>1.5f)
			{
				Control::Audio->StopSample(aSFXSoundExperience);

				isSndSceneFadingOut = false;

				vfxRain->StopImmediate();
				vfxRainDrops->StopImmediate();
				vfxLeaves->StopImmediate();

				MaskBg->SetAlpha(1.0f);
				SCNSndExp->Show(false);

				if(isFPSCheckingActive)
				{
					isFPSCheckingActive = false;
					int averageFPS = Totalfps/fpsCount;
					//K_LOG("Average FPS is %d",averageFPS);
					if(averageFPS<40)
					{
						K_LOG("Warning : System Diagnosed as Low System");
						GFInstance->IsSlowSystem = true;
					}
					//GFInstance->IsSlowSystem = true; //To Simulate Low Machine state
					Control::MasterTmr->IsCalculateFPS = false;
				}
				
				IsHaveYouVideoPlaying = true;
			}
		}

		if( !isSndSceneFadingIn && !isSndSceneFadingOut && Control::Input->LBclicked())
		{
			isSndSceneFadingOut = true;

			MaskBg->SetAlpha(0);
			MaskBg->Show(true);
		}
	}
}

void HaveYouCutScene::Render(const CPoint* pCam)
{  
   static int FrameCnt = 0;
   FrameCnt = (FrameCnt+1)%2;
	  
   if( IsVideo1Loaded && IsHaveYouVideoPlaying)
   {
	    if( FrameCnt  || !lpHaveYouCutsceneVideo->isVideoAvailable() )
		   lpHaveYouCutsceneVideo->readFrame();
				//lpHaveYouCutsceneVideo->getKGraphic()->blitAlphaRectFx(0,0,1024,576,0,0,0,1.334f,eBlend_AlphaNormal,false,false,-512,-288);//draw 1024x576 scaled cine
		//lpHaveYouCutsceneVideo->getKGraphic()->blitAlphaRectFx(0,0,GFApp->video_width,GFApp->video_height,0,0,0,GFApp->video_aspectRatio,eBlend_AlphaNormal,false,false,GFApp->video_negativeHalfWidth,GFApp->video_negativeHalfHeight);//draw scaled cine
       lpHaveYouCutsceneVideo->getKGraphic()->stretchAlphaRect(0,0,GFApp->video_width,GFApp->video_height,0,0,MAX_DISPLAY_WIDTH,MAX_DISPLAY_HEIGHT,eBlend_AlphaNormal,false,false,0,0);

	   if (lpHaveYouCutsceneVideo->getFrame()>48)
	   {
		   isSkipShowing = true;
		   if( isMouseHover )  
		   {
			   Control::Input->LBtnUpdate();
			   CoreImage->blitAlphaRectF( 64,0,128,32,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);
			   if( isSkipped )
			   {
				   CoreImage->blitAlphaRectF( 64,0,128,32,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);
				   Control::Input->ClearMouse();
			   }
			   else
			   {
				   CoreImage->blitAlphaRectF( 64,0,128,32,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);
			   }
		   }
		   else //Cursor not over skip region!!
		   {
			   CoreImage->blitAlphaRectF( 0,0,64,32,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);
		   } 
	   }
	   
	   time = lpHaveYouCutsceneVideo->getTime();

	   if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
	   {
		   subtitleText->SetText(subtitles[subIndex].text);
	   }
	   else
		   subtitleText->SetText("");

	   if (time > subtitles[subIndex].end)
	   {
		   if (subMax - 1 > subIndex)
			   subIndex++;
	   }

	  if (time >= 0 && time < 5)
	  {
	    	templeOdoomTxt->blitAlphaRectFx(0, 0, 504, 110, 440, 325, 0, 1.5, 1);
	  }
	   
	   if( lpHaveYouCutsceneVideo->isEndReached() || isSkipped || (lpHaveYouCutsceneVideo->getTime() >= lpHaveYouCutsceneVideo->getTotalTime()-0.1f))
	   {
		   subtitleText->SetText("");
		   ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
		   IsHaveYouVideoPlaying = false;
		   if( lpHaveYouCutsceneSndStream != NULL )
				lpHaveYouCutsceneSndStream->stopStream();


		   Exit();
	   }
   }
}

HaveYouCutScene::~HaveYouCutScene()
{
	IsVideo1Loaded = false;	
	SAFE_DELETE(SCNSndExp);

	if( CoreImage )
	{
		CTexFactory::Instance->FreeImage(&CoreImage);
		CoreImage = NULL;	
	}

	if (templeOdoomTxt)
	{
		CTexFactory::Instance->FreeImage(&templeOdoomTxt);
		templeOdoomTxt = NULL;
	}

	SAFE_DELETE(vfxRain);
	SAFE_DELETE(vfxRainDrops);
	SAFE_DELETE(vfxLeaves);

	
	Control::Audio->StopSample(aSFXSoundExperience);
	Control::Audio->UnloadSample(aSFXSoundExperience);

	
	delete lpHaveYouCutsceneVideo; 
	lpHaveYouCutsceneVideo = NULL;

	if(lpHaveYouCutsceneSndStream!=NULL)
	{
		delete lpHaveYouCutsceneSndStream;
		lpHaveYouCutsceneSndStream = NULL;
	}

	SAFE_DELETE(subtitleText);
}