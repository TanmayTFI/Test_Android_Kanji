
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L6_S3_FakeJames.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"

const CRectangle  JamesRect(315.0f,21.0f,691.0f,737.0f);


L6_S3_FakeJames::L6_S3_FakeJames()
{
	isVideoPlaying = false;
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S3\\FAKEJAMES\\FAKEJAMES.SCN");

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	

	dialogSystem = NULL;
	//if condition to add
	dialogSystem = new DialogSystem();
	AddControl(dialogSystem);	
	isDialogActive = false;


	cv_Cutscn = NULL;

	time = 0;
	subIndex = 0;
	subMax = 2;
	vidTimer = 0;
	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("fakejames1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("fakejames2"));
	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	cv_Cutscn = new CVideo("OGV\\L6\\S3FAKEJAMES.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//cv_Cutscn->SetScale(1.334f);
	cv_Cutscn->SetScale(GFApp->video_aspectRatio);
	cv_Cutscn->SetSkipActive();

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s3FloatingIsland3Amb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s3FloatingIsland3Amb) )		
	{
		Control::Audio->PlaySample(aAmbL6s3FloatingIsland3Amb,true);
	}
	
	//MUSIC_SFX
	
	

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L6_S3_FakeJames]  = true;
}

void L6_S3_FakeJames::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(isVideoPlaying)
		return;

	if( Control::Audio->IsPlaying(aTrackL6ho) )
		{
			Control::Audio->Samples[aTrackL6ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL6ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL6ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL6theme) )
					  Control::Audio->PlaySample(aTrackL6theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL6puzzle) )
		{
			Control::Audio->Samples[aTrackL6puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL6puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL6puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL6theme) )
					  Control::Audio->PlaySample(aTrackL6theme, true);
					  
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL6theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL6theme, true);
			}
		}
}

void L6_S3_FakeJames::Update()
{
	NavigateToScene();
	if(isDialogActive)
	{
		dialogSystem->Update();
		return;
	}
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if (isVideoPlaying )
	{
		//Subtitles Code
		time = cv_Cutscn->GetTime();

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
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_Cutscn->isEndReached() ||
#endif
			vidTimer > 23)
		{
			isVideoPlaying = false;
			cv_Cutscn->StopAndFadeOutVideo(2.0f);					
			Hud->EnableInput(true);
			Hud->ShowHud(false);
			subtitleText->SetText("");
			ProfileSystem::Instance->ActiveProfile.L6_S3_LasyaCutscenePlayed = true;

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P5_LasyaBrokeBridge])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P5_LasyaBrokeBridge] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P5_LasyaBrokeBridge;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;

				Hud->ShowJrnEntryFX();
			}
					
			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O2_FindWayOutToConnectBridge] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O2_FindWayOutToConnectBridge] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
				= eObj_L6O2_FindWayOutToConnectBridge;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L6O2_FindWayOutToConnectBridge);

				Control::Audio->PlaySample(aSFXMagic1);
			}

			

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FloatingIsland3;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		} 
		return;
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed() && !isVideoPlaying ) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
	#endif

	if( JamesRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Speech);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			dialogSystem->ShowDialog(eDialog_L6S3FakeJames);
			isDialogActive = true;
			Hud->AllowInput = false;
			Hud->HideHud(false);				
		}	
	}
}

void L6_S3_FakeJames::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
	}
	if( val == HOSM_DialogClosed ) 
	{
		Cursor::SetMode(CA_Normal); 
		isDialogActive = false;
		Hud->AllowInput = false;
		Hud->HideHud();
		ProfileSystem::Instance->ActiveProfile.L6_S3_JamesDialogsShown = true;
		isVideoPlaying = true;
		cv_Cutscn->PlayVideo();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L6_S3_FakeJames::ProcessSkip()
{
}

void L6_S3_FakeJames::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: ++iNonHOHint;

			break;
		case 1: ++iNonHOHint;
			if( !isDialogActive )
			{
				InitHintTrailEmit( &JamesRect);
				return;
			}
			break;
		case 2: ++iNonHOHint;

			break;
		case 3: ++iNonHOHint;

			break;
		case 4: ++iNonHOHint;

			break;
		case 5: ++iNonHOHint;

			break;
		case 6: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 7: ++iNonHOHint;
				{
					Hud->teleportSystem->ShowTeleport();
					return;
				}
				break;

		}
	}
	while( iNonHOHint < MaxNonHOHints );

	Hud->ShowNothingToDoMsgBanner();
}

L6_S3_FakeJames::~L6_S3_FakeJames()
{
	SAFE_DELETE(cv_Cutscn);
	SAFE_DELETE(subtitleText);
	Hud->AllowInput = true;
	Hud->ShowHud(false);
		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL6s3FloatingIsland3Amb);
	Control::Audio->UnloadSample(aAmbL6s3FloatingIsland3Amb);
	//MUSIC_SFX
}
void L6_S3_FakeJames::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				//Cursor::SetMode(CA_ExitUp);
				
				//GFHud->InvBoxCtrl->NavigateText->Show(false);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = GFHud->InvBoxCtrl->travelArea;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
				
			GFHud->InvBoxCtrl->NavigateText->Text = "";
			GFHud->InvBoxCtrl->travelArea = -1;
			GFHud->InvBoxCtrl->navTapped = false;
			GFHud->InvBoxCtrl->Navigate->Show(false);					
			GFHud->InvBoxCtrl->NavArrow->Show(false);
		}
	}
}