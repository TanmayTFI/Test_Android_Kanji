//====================================
//	DQFYLH
//	HO System
//	Level 1 - EndOfDemoScene
//====================================

#include "EndOfDemoScene.h"
#include "Hud.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "GameFlow.h"
#include "Util.h"
#include "AwardManager.h"

const CRectangle  BtnRectMainMenu(887.0f,605.0f,158.0f,55.0f);

const CPoint	CenterPoint(683,384,0);

const CRectangle  fullAreaRect(3.0f,3.0f,1364.0f,780.0f);

const int ePopWallpaper = 10;

EndOfDemoScene::EndOfDemoScene()
{
	Visible = false;	

	LoadScene("LV1\\EOD\\EOD.SCN");

	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\MENUTITLE.SPR", 950, 180, 100);
	temp->SetScale(0.5);
	temp->SetUID("menuTitle");
	HOScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\EXIT.SPR", 965, 645, HOScene->GetObjectByID("btnexit")->ZOrder + 5);
	//temp->SetScale(0.8);
	temp->SetUID("exitBtnTXT");
	HOScene->PushObject(temp);

	ColorRGB textColor = { 1, 1, 1 };

	desc = new MultilineTextBox(AssetsStringTable::getString("EOD_desc"), 950, 270, 101, textColor, eFont_Diary, 25);
	desc->CenterText(); 
	desc->Show(true);

	thankyou = new MultilineTextBox(AssetsStringTable::getString("EOD_thankyou"), 950, 500, 101, textColor, eFont_Diary, 25);
	thankyou->CenterText();
	thankyou->Show(true);

	#ifdef K_ANDROID
	/*
		BtnMainMenu = HOScene->GetObjectByID("BtnMenu");
		BoughtImage = HOScene->GetObjectByID("bought");
		BoughtImage->Show(false);
	 */
		BtnUnlockNow = new CObject();
		BtnUnlockNow->LoadInit("UI\\HUD\\BTNMENU.SPR", 1200, 100, 25);
		BtnUnlockNow->SetUID("UNLOCK_BUTTON");
		BtnUnlockNow->SetScale(2);
		HOScene->PushObject(BtnUnlockNow);
		BtnMainMenu = HOScene->GetObjectByID("btnexit");
		BtnUnlockNow->SetEnable(false);
	#else		
		BtnExit = HOScene->GetObjectByID("btnexit");
	#endif
	

	for (int i = 0; i < MAX_NUM_WALLS; i++)
	{
		wallThumbArr[i] = NULL;
		wallThumbArr[i] = HOScene->GetObjectByID(std::string("wallthumb" + MKSTR((i+1))).c_str());
	}

	PopMaskBg = new CObject();
	PopMaskBg->LoadSprite("UI\\MASK.SPR");
	PopMaskBg->SetXY(683,384);
	PopMaskBg->SetScale(2.0);
	PopMaskBg->SetZ(30);
	PopMaskBg->Show(true);

	//Scene Changes
	GFInstance->PostUpdate(2);

	IsVidSkippable = true; //Just in this case only

	IsSkipMouseOver = false;
	IsSkipMouseClicked = false;
	SkipShowTimer = 0;

	SetZ(2);

	skipObj = NULL;

	skipObj = new CObject();
	skipObj->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\SKIPBTN.SPR", (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y, eZOrder_CutsceneFull+10);
	skipObj->Show(false);

	cv_OutroVideo = NULL;
	time = 0;
	subIndex = 0; 
	subMax = 10;

	vidTimer = 0;


	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("outro1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("outro2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("outro3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("outro4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("outro5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("outro6"));
	subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("outro7"));
	subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("outro8"));
	subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("outro9"));
	subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("outro10"));

	KSound *cvSnd_OutroVideo=NULL;
	#ifdef K_WIN32
	{
		if( !KSound::g_bErrSoundDeviceInit )
		{
			cvSnd_OutroVideo = new KSound;
		}
	}
	#else
		cvSnd_OutroVideo = new KSound;
	#endif
		
	Control::Audio->TurnOffLvlMscTracks();//turns off music-snd carryover....

	//CRectangle RectFullVid(171, 96, 1024, 576);
	CRectangle RectFullVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	if(!ProfileSystem::Instance->ActiveProfile.L1_IsDemoOutroVideoPlayed)
	{
		//#ifndef K_MACOS
			cv_OutroVideo = new CVideo("Data\\OUTRO.OGV", RectFullVid, false, eZOrder_CutsceneFull, NULL, cvSnd_OutroVideo, true);
		/*#else
			cv_OutroVideo = new CVideo("OUTRO.OGV", RectFullVid, false, eZOrder_CutsceneFull, NULL, cvSnd_OutroVideo, true);
		#endif
         */
	//	cv_OutroVideo->SetScale(1.334f);
		cv_OutroVideo->SetScale(GFApp->video_aspectRatio);

		if(cvSnd_OutroVideo != NULL)
			cvSnd_OutroVideo->setVolume((long)(ProfileSystem::Instance->ActiveProfile.VoiceVol*0.85f));
	}
	IsOutroVideoPlaying = false;

	PopupAlpha = 0;
	PopVal = 0;

	InitPopups();
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_EndOfDemo] = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_EndOfDemo])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_EndOfDemo] = true;

	if( !Control::Audio->IsPlaying(aTrackL1EscapePlan) )
		Control::Audio->PlaySample(aTrackL1EscapePlan,true);

	if(!ProfileSystem::Instance->ActiveProfile.L1_IsDemoOutroVideoPlayed)
	{
		Control::Audio->LoadSample(aTrackCutscnS7BoatOutro,AudioTrack);
	}

	Control::Audio->LoadSample(aAmbEndOfDemo,AudioAmb,true);

	if( !Control::Audio->IsPlaying(aAmbEndOfDemo) )		
	{
		Control::Audio->Samples[aAmbEndOfDemo]->SetVolume(70);
	}
	else //ensure 70
		Control::Audio->Samples[aAmbEndOfDemo]->SetVolume(70);

	

	
	

	if(!ProfileSystem::Instance->ActiveProfile.L1_IsDemoOutroVideoPlayed)
	{
		IsOutroVideoPlaying = true;
		cv_OutroVideo->PlayVideo();
		HOScene->Show(false);

		//Control::Audio->PlaySample(aTrackCutscnS7BoatOutro,false);
	}
	else
	{
	#ifndef _DEMOBUILD
		Control::Audio->PlaySample(aAmbEndOfDemo,true);
	#endif // _DEMOBUILD
	#ifdef K_ANDROID
		if (GFApp->isUnlocked()) //Full Build
			{
				Control::Audio->PlaySample(aAmbEndOfDemo,true);
			}
	#endif
	}

	//In case End of demo is played. For Full version. Move the Player to the Next Scene ie Forest Entrance
	#ifndef _DEMOBUILD
		#ifdef K_ANDROID
			if (GFApp->isUnlocked()) //If Full Build
			{
		#endif
				if(ProfileSystem::Instance->ActiveProfile.L1_IsDemoOutroVideoPlayed)
				{
					HOScene->Show(false);
					ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_2;
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
					SendMessage(ControlExit, ID, HOSM_NewArea);
				}
		#ifdef K_ANDROID
			}
		#endif
	#endif // _DEMOBUILD

	
	Visible = true;	//To render anything with renderlistentity especially Video
}

float EndOfDemoScene::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void EndOfDemoScene::Update()
{
	Cursor::SetMode(CA_Normal);

	
	

	if(IsOutroVideoPlaying)
	{
		if( IsVidSkippable )
		{
			if(SkipShowTimer>2.0f)
			{
				if(!skipObj->Visible)
					skipObj->Show(true);

				CPoint mousePos = Control::Input->Mpos();
				IsSkipMouseOver = ( mousePos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) && 
					mousePos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
					mousePos.y >= SKIPIMAGE_Y &&
					mousePos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) + SKIPIMAGE_WIDTH) );

				if( IsSkipMouseOver && !IsSkipMouseClicked)  
				{
					Cursor::SetMode(CA_HandPointy);
					skipObj->PlayAnimation(1);
					IsSkipMouseClicked = Control::Input->LBclicked();					
					if( IsSkipMouseClicked )
					{
						Control::Audio->PlaySample(aSFXClick);
						Control::Input->ClearMouse();
						AwardManager::GetInstance()->SkipDialogueOrCutScene();

					}
				}
				else //Cursor not over skip region
				{
					skipObj->PlayAnimation(0);
				}

				if(IsOutroVideoPlaying)
				{
					//Subtitles Code
					time = cv_OutroVideo->GetTime();

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
					vidTimer += Control::LogicRate;

					if(cv_OutroVideo->isEndReached() || IsSkipMouseClicked || vidTimer > 69)
					{
						subtitleText->SetText("");
						SkipShowTimer = 0;
						IsSkipMouseClicked = false;

						Control::Audio->PlaySample(aAmbEndOfDemo,true);//will be paused in iPad popups!!
						HOScene->Show(true);
						PopMaskBg->Show(false);
						cv_OutroVideo->StopAndFadeOutVideo(2.0f);					
						IsOutroVideoPlaying = false;

						//Control::Audio->StopSample(aTrackCutscnS7BoatOutro);

						skipObj->Show(false);
						ProfileSystem::Instance->ActiveProfile.L1_IsDemoOutroVideoPlayed = true;

					#ifndef _DEMOBUILD
						#ifdef K_ANDROID
							if (GFApp->isUnlocked()) //If Full Build
							{
						#endif
							ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_2;
							ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
							SendMessage(ControlExit, ID, HOSM_NewArea);
						#ifdef K_ANDROID
							}
						#endif
					#endif // _DEMOBUILD
					return;
					}
				}
			}
			else
				SkipShowTimer += 10*Control::LogicRate;
		}
		return;
	}

	CPoint pos = Control::Input->Mpos();

	#ifdef ANDROID
		if (Control::Input->isKeyBackSensed() && ActivePopupID == ePopInactive) 
		{
 			SAVEALLPROFILES();
			if(GFApp->IsSurveyBuild)
			{
				//GFApp->LaunchSurvey();
			}
			SendMessage(ControlExit, ID, HOSM_ReturnToMenu); //from here we dont exit, we go back to Main menu
			return;
		}
		if(BtnUnlockNow->Collide(&pos) && ActivePopupID == ePopInactive) // Is Upgrade now button pressed
			{
				Cursor::SetMode(CA_HandPointy);
				//BtnUnlockNow->PlayAnimation(1);
				if(Control::Input->LBclicked())
				{
					//BtnUnlockNow->PlayAnimation(2);
					//SAVEALLPROFILES();

					//GFApp->ShowUnlockStore(); //In-App unlock

					//KMiscTools::launchURL ("https://play.google.com/store/search?q=darkarta&c=apps&hl=en_US", true);

					return;
				}
			}
			else
			{
				//BtnUnlockNow->PlayAnimation(0);
			}
		if(BtnMainMenu->Collide(&pos) && ActivePopupID == ePopInactive) // Is Menu button pressed
			{
				Cursor::SetMode(CA_HandPointy);
				BtnMainMenu->PlayAnimation(1);
				if(Control::Input->LBclicked())
				{
					BtnMainMenu->PlayAnimation(2);
					SAVEALLPROFILES();
					SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
					return;
				}
			}
			else
			{
				BtnMainMenu->PlayAnimation(0);
			}
    #ifndef _DEMOBUILD
		if(!GFApp->isUnlocked()) // if the game is still free, do not show the "Congratz screen"
		{
			BtnUnlockNow->Show(true);
			//BoughtImage->Show(false);
		}	 	
		else
		{
			BtnUnlockNow->Show(false);
			//BoughtImage->Show(true);
		}
    #endif
	#else
		if(BtnExit->Collide(&pos) && ActivePopupID == ePopInactive)
		{
			Cursor::SetMode(CA_HandPointy);
			BtnExit->PlayAnimation(1);
			if(Control::Input->LBclicked())
			{
				BtnExit->PlayAnimation(2);
				SAVEALLPROFILES();
				

				if(GFApp->IsSurveyBuild)
				{
					//GFApp->LaunchSurvey();
				}
			
				//SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
				exit(0);
				return;
			}
		}
		else
		{
			BtnExit->PlayAnimation(0);
		}
	#endif

	if( !IsPopupTransitionActive ){
		UpdatePopups(pos);
	}
	else{
		UpdatePopupTransition();
	}

}

void EndOfDemoScene::InitPopups()
{	
	ActivePopupID = ePopInactive;

	IsPopupTransitionActive = false;
	Popup1_State = POPUP_CLOSED;

	SCNWallPop = NULL;
}



void EndOfDemoScene::UpdatePopups(CPoint pos,bool ItemDropped)
{
	if( (ActivePopupID == ePopInactive || ActivePopupID == ePopWallpaper) )
	{
		switch( Popup1_State )
		{
		case POPUP_CLOSED:
			for (int i = 0; i < MAX_NUM_WALLS; i++)
			{
				if(wallThumbArr[i]->Collide(&pos, -10))
				{
					wallThumbArr[i]->PlayAnimation(1);
					Cursor::SetMode(CA_Look);
					if(Control::Input->LBclicked())
					{
						wallThumbArr[i]->PlayAnimation(0);

						Control::Audio->PlaySample(aSFXpopup);
						Popup1_State = POPUP_SCALEUP;
						PopVal = 0;

						ActivePopupID = ePopWallpaper;

						if(SCNWallPop!=NULL)
							SAFE_DELETE(SCNWallPop);

						SCNWallPop = new CHOScene();
						SCNWallPop->Load("LV1\\EOD\\BLANK.SCN");

						CObject *popBGImage = new CObject();
						popBGImage->LoadInit(std::string("LV1\\EOD\\WALL" + MKSTR((i+1))+".SPR").c_str(), 0, 0, 0);
						SCNWallPop->InsertObject(popBGImage,0);

						SCNWallPop->Show(false);
						SCNWallPop->SetPosOff(CenterPoint.x,CenterPoint.y);
						SCNWallPop->SetZOff(eZOrder_Popup);

						SetupPopupTransition(SCNWallPop, wallThumbArr[i]->GetWorldRect());
					}
					break;
				}
				else
					wallThumbArr[i]->PlayAnimation(0);
			}
			break;
		case POPUP_OPEN:
			if(fullAreaRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if(Control::Input->LBclicked())
				{
					Control::Audio->PlaySample(aSFXpopdown);
					Popup1_State = POPUP_SCALEDOWN;					

					if(SCNWallPop!=NULL)
						SCNWallPop->Show(false);

					IsPopupTransitionActive = true;
				}				
			}
			break;
		}
	}
}

void  EndOfDemoScene::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{

		IsPopupTransitionActive = true;
 if( GFInstance->IsRTCreated )
    {
	
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS	
		GFInstance->IsRenderingToRT = true;
	}
		Popup->RenderToTarget(-173,0);

  if( GFInstance->IsRTCreated )
    {

		GFInstance->IsRenderingToRT = false;
		GFInstance->RTGraphic->endRenderToTarget();
	}
		GFInstance->SetPopupTween( (int)popRect.GetCenterX(-683) ,(int)popRect.GetCenterY(-384 ),173,0);

		PopMaskBg->SetAlpha(0);
		PopMaskBg->Show(true);

		SetZ(eZOrder_Popup);
	}

void EndOfDemoScene::UpdatePopupTransition()
{
	if( ActivePopupID == ePopWallpaper )
	{
		switch( Popup1_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;

				if(SCNWallPop!=NULL)
					SCNWallPop->Show(true);

				Popup1_State = POPUP_OPEN;
			}
			PopMaskBg->SetAlpha(PopupAlpha);
			break;
		case POPUP_SCALEDOWN:
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;
				Popup1_State = POPUP_CLOSED;

				PopMaskBg->Show(false);

				ActivePopupID = ePopInactive;
			}		
			else
				PopMaskBg->SetAlpha(PopupAlpha);
			break;
		}
	}
}

void EndOfDemoScene::Render(const CPoint* pCam)
{  
	static int FrameCnt = 0;
   	FrameCnt = (FrameCnt+1)%2;

	if(ActivePopupID != ePopInactive)
	{
		if(IsPopupTransitionActive)
		{

			//Init again if render target lost due to fullscreen-window switches or similar
			if( GFInstance->RTGraphic != NULL )
				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),
				//0.2f,0.6f); //test gap between frames
				PopupAlpha,PopupAlpha);
			return;
		}
   }
}

EndOfDemoScene::~EndOfDemoScene()
{
	Control::Audio->UnloadSample(aAmbEndOfDemo);

	Control::Audio->UnloadSample(aTrackCutscnS7BoatOutro);

	SAFE_DELETE(PopMaskBg);
	SAFE_DELETE(SCNWallPop);

	SAFE_DELETE(thankyou);
	SAFE_DELETE(desc);

	SAFE_DELETE(skipObj);

	SAFE_DELETE(cv_OutroVideo);
	SAFE_DELETE(subtitleText);
}


