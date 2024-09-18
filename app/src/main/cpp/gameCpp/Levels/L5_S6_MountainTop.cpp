//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S6_MountainTop.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "L5S6_Pop2Moon.h"
#include "L5S6_Pop1Peacock.h"

const float CLOUD_OVERFLOW_WIDTH = 225.0f;

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  HORect(637.0f,198.0f,635.0f,328.0f);


L5_S6_MountainTop::L5_S6_MountainTop()
{
	Replay_WaitingForCollectablePickTicks = 1.0f;
	IsReplay_WaitingForCollectablePick = true;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S6\\S6.SCN");

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	cloudBG1 = HOScene->GetObjectByID("cloud1");
	cloudBG2 = HOScene->GetObjectByID("cloud2");
	

	cloudBG1->SetBlendMode(eBlend_AlphaAdditive);
	cloudBG2->SetBlendMode(eBlend_AlphaAdditive);

	cloudbgSpeed1 = 10;
	cloudbgSpeed2 = 11;
	
	cloudFG1 = HOScene->GetObjectByID("cloud5");
	
	cloudfgSpeed1 = 14;

	cloudFG1->SetBlendMode(eBlend_AlphaAdditive);
	
	HOScene->GetObjectByID("plant1")->SetPivot(0, 180);
	HOScene->GetObjectByID("plant1")->Animate(0,0,4,0,8.0f);	

	HOScene->GetObjectByID("plant2")->SetPivot(0, 67);
	HOScene->GetObjectByID("plant2")->Animate(0,0,3,0,4.0f);	

	HOScene->GetObjectByID("plant3")->SetPivot(-62, 0);
	HOScene->GetObjectByID("plant3")->Animate(0,0,2,0,5.0f);	

	HOScene->GetObjectByID("plant4")->SetPivot(0, 20);
	HOScene->GetObjectByID("plant4")->Animate(0,0,2,0,6.0f);	

	HOScene->GetObjectByID("plant5")->SetPivot(0, 24);
	HOScene->GetObjectByID("plant5")->Animate(0,0,3,0,7.0f);	

	HOScene->GetObjectByID("plant6")->SetPivot(0,24);
	HOScene->GetObjectByID("plant6")->Animate(0,0,4,0,8.0f);	

	HOScene->GetObjectByID("plant7")->SetPivot(0,24);
	HOScene->GetObjectByID("plant7")->Animate(0,0,3,0,8.0f);	

	HOScene->GetObjectByID("plant8")->SetPivot(0, 50);
	HOScene->GetObjectByID("plant8")->Animate(0,0,3,0,7.0f);	

	HOScene->GetObjectByID("plant9")->SetPivot(0, 35);
	HOScene->GetObjectByID("plant9")->Animate(0,0,4,0,8.0f);	

	HOScene->GetObjectByID("plant10")->SetPivot(0, 60);
	HOScene->GetObjectByID("plant10")->Animate(0,0,3,0,8.0f);	

	HOScene->GetObjectByID("plant11")->SetPivot(0, 40);
	HOScene->GetObjectByID("plant11")->Animate(0,0,3,0,7.0f);	

	S6MoonprtFX = new SuperFX("PRT\\L5\\S6Moonprt.INI", PointSystem::CreateCPoint(906,239,0), 4); 
	S6MoonprtFX->Start();
	S6MoonprtFX->AdvanceByTime(2.0f);

	cvIntroCutscn = NULL;
	time = 0;
	subIndex = 0;
	subIndex2 = 0;
	subMax = 2;
	vidTimer = 0;
	vidTimer2 = 0;
	vidTimer3 = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("s6intro"));

	subtitles2[0] = AssetsStringTable::getSub(AssetsStringTable::getString("s6outro1"));
	subtitles2[1] = AssetsStringTable::getSub(AssetsStringTable::getString("s6outro2"));


	subtitles3[0] = AssetsStringTable::getSub(AssetsStringTable::getString("peacock1"));
	subtitles3[1] = AssetsStringTable::getSub(AssetsStringTable::getString("peacock2"));


	isIntroCutscnPlaying = false;
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_IntroCutScenePlayed)
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);	
		cvIntroCutscn = new CVideo("OGV\\L5\\S6INTRO.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//cvIntroCutscn->SetScale(1.334f);
		cvIntroCutscn->SetScale(GFApp->video_aspectRatio);
		cvIntroCutscn->SetSkipActive(); 

		Hud->AllowInput = false;
		Hud->HideHud();		
		cvIntroCutscn->PlayFadeInVideo(2.0f);
		isIntroCutscnPlaying = true;
	}

	cv_PeacockCutscn = NULL;
	isPeacockCutscnPlaying = false;
	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockArrivingCutScenePlayed)
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);
		cv_PeacockCutscn = new CVideo("OGV\\L5\\S6PEACOCK.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//cv_PeacockCutscn->SetScale(1.334f);
		cv_PeacockCutscn->SetScale(GFApp->video_aspectRatio);
		cv_PeacockCutscn->SetSkipActive(); 
	}

	cv_OutroCutscn = NULL;
	isOutroCutscnPlaying = false;
	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLeavingCutScenePlayed || ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed)
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);
		cv_OutroCutscn = new CVideo("OGV\\L5\\S6OUTRO.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//cv_OutroCutscn->SetScale(1.334f);
		cv_OutroCutscn->SetScale(GFApp->video_aspectRatio);
		cv_OutroCutscn->SetSkipActive(); 
	}

	//If not replaying game & if locket used, then outro video!!
	if( !GFInstance->IsReplayingGame && ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed  )
	{
		Hud->AllowInput = false;
		Hud->HideHud();		
		cv_OutroCutscn->PlayFadeInVideo(2.0f);
		isOutroCutscnPlaying = true;
	}

	
	Control::Audio->LoadSample(aVOL5MaryRefS6_1,	AudioVO);
	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_GotFreeBannerShown && ProfileSystem::Instance->ActiveProfile.L5_S6_IntroCutScenePlayed)
	{
			Hud->ShowBannerText("L5whew");
			Control::Audio->PlaySample(aVOL5MaryRefS6_1);
			ProfileSystem::Instance->ActiveProfile.L5_S6_GotFreeBannerShown = true;
	}

	#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S6Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		const CRectangle  MorphRect(996.0f,429.0f,154.0f,121.0f);

		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

	InitPopups();

	SceneObjectsUpdate();

	TaskPendingUpdate();

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s6MountainTopAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s6MountainTopAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s6MountainTopAmb,true);
	}
	
	//MUSIC_SFX
	
	if(ProfileSystem::Instance->ActiveProfile.L5_S6_HOCompleted)
	{
		//Journal Entry Add
		if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P7_TreeReleased])
		{
			//Flag Set
			ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P7_TreeReleased] = true;

			//Add to Note Array
			ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L5P7_TreeReleased;

			ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

			//Increment Notes
			ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			Hud->ShowJrnEntryFX();
		}
	}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S6_MountainTop]  = true;
}

void L5_S6_MountainTop::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL5S6_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}
	
	

	//Popup 1
	if(ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockArrivingCutScenePlayed)
	{
		int p1rect[] =  {270,253,468,236,537,364,531,474,414,559,190,556,204,418};
		PopPolyArray[ePop1_Peacock] =  new CPolygon(7);
		PopPolyArray[ePop1_Peacock]->Init(p1rect);

		PopupArray[ePop1_Peacock] = new L5S6_Pop1Peacock(ePop1_Peacock,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop1_Peacock]->center, CC_CALLBACK_1(L5_S6_MountainTop::OnPopupClosed, this));
		PopupArray[ePop1_Peacock]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop1_Peacock] = ePopupState_Closed;
	}

	//Popup 2
	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed)
	{
		int p2rect[] = {427,24,600,30,690,194,652,314,526,297,463,215,373,220,323,145};
		PopPolyArray[ePop2_Moon] =  new CPolygon(8);
		PopPolyArray[ePop2_Moon]->Init(p2rect);

		PopupArray[ePop2_Moon] = new L5S6_Pop2Moon(ePop2_Moon,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop2_Moon]->center, CC_CALLBACK_1(L5_S6_MountainTop::OnPopupClosed, this));
		PopupArray[ePop2_Moon]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop2_Moon] = ePopupState_Closed;
	}

}

void L5_S6_MountainTop::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	

	SceneObjectsUpdate();
	switch (popup->GetPopupId())
	{
	case 1://pop2_moon - locket shown to moon & peacock coming
	
		if(ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockChainUsed)		
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
			Hud->AllowInput = false;
			Hud->PrevCursorMode = -1;
			Hud->HideHud();		
			cv_PeacockCutscn->PlayFadeInVideo(2.0f);
			isPeacockCutscnPlaying = true;
			//Popup 1
			
				int p1rect[] =  {270,253,468,236,537,364,531,474,414,559,190,556,204,418};
				PopPolyArray[ePop1_Peacock] =  new CPolygon(7);
				PopPolyArray[ePop1_Peacock]->Init(p1rect);

				PopupArray[ePop1_Peacock] = new L5S6_Pop1Peacock(ePop1_Peacock,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop1_Peacock]->center, CC_CALLBACK_1(L5_S6_MountainTop::OnPopupClosed, this));
				PopupArray[ePop1_Peacock]->SetPopupBoundingBox(PopAreaRect);

				PopStates[ePop1_Peacock] = ePopupState_Closed;
			
		}
		break;
	case 0://pop1_peacock flying with us - outro!!
		//disable popup
		PopStates[popup->GetPopupId()] = ePopupState_Inactive;
			
		if( !GFInstance->IsReplayingGame && ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed )
		{
			Hud->AllowInput = false;
			Hud->HideHud();		
			cv_OutroCutscn->PlayFadeInVideo(2.0f);
			isOutroCutscnPlaying = true;
		}
		break;
	
	
	default:
		break;
	}
	
}

void L5_S6_MountainTop::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_HOCompleted)
			HOScene->GetObjectByID("branchAfterHO")->SetEnable(false);
	else
			HOScene->GetObjectByID("branchAfterHO")->SetEnable(true);

	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockArrivingCutScenePlayed)
	{
			HOScene->GetObjectByID("peacock")->SetEnable(false);
			HOScene->GetObjectByID("locket")->SetEnable(false);
	}
	else
	{
			HOScene->GetObjectByID("peacock")->SetEnable(true);
			if( ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed )
				HOScene->GetObjectByID("locket")->SetEnable(true);
	}
}

void L5_S6_MountainTop::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(isIntroCutscnPlaying || isPeacockCutscnPlaying || isOutroCutscnPlaying || ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLeavingCutScenePlayed)
		return;

	if( Control::Audio->IsPlaying(aTrackL5ho) )
		{
			Control::Audio->Samples[aTrackL5ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL5ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL5ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL5theme) )
					  Control::Audio->PlaySample(aTrackL5theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL5puzzle) )
		{
			Control::Audio->Samples[aTrackL5puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL5puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL5puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL5theme) )
					  Control::Audio->PlaySample(aTrackL5theme, true);
					  
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL5theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL5theme, true);
			}
		}
}

void L5_S6_MountainTop::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	if (isPeacockCutscnPlaying)
	{
		Cursor::SetMode(CA_Normal);
		time = cv_PeacockCutscn->GetTime();

		if (time >= subtitles3[subIndex2].begin && time < subtitles3[subIndex2].end)
		{
			subtitleText->SetText(subtitles3[subIndex2].text);
		}
		else
			subtitleText->SetText("");

		if (time > subtitles3[subIndex2].end)
		{
			if(subMax-1 > subIndex2)
				subIndex2++;
		}
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_PeacockCutscn->isEndReached() ||
#endif
			vidTimer > 13)
		{
			subtitleText->SetText("");
			AwardManager::GetInstance()->CollectAward(kAward_Tame_Magical_peacock);
			ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockArrivingCutScenePlayed= true;

			isPeacockCutscnPlaying = false;
			cv_PeacockCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();
			HOScene->GetObjectByID("peacock")->SetEnable(true);
			Hud->ShowBannerText("L5pet");

			//Show Objective completed
			if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O2_FindWayOutOfAtharwaSwamps] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O2_FindWayOutOfAtharwaSwamps] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O2_FindWayOutOfAtharwaSwamps] = true;
				Hud->ShowObjectiveFX(eObj_L5O2_FindWayOutOfAtharwaSwamps,true);
			}

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P8_MeetingPeacock])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P8_MeetingPeacock] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L5P8_MeetingPeacock;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}
		} 
		return;
	}

	if (isOutroCutscnPlaying)
	{

		Cursor::SetMode(CA_Normal);
		time = cv_OutroCutscn->GetTime();

		if (time >= subtitles2[subIndex].begin && time < subtitles2[subIndex].end)
		{
			subtitleText->SetText(subtitles2[subIndex].text);
		}
		else
			subtitleText->SetText("");

		if (time > subtitles2[subIndex].end)
		{
			if(subMax-1 > subIndex)
				subIndex++;
		}
#ifdef FORCE_CLOSE_VIDEO
		vidTimer2 += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_OutroCutscn->isEndReached() ||
#endif
			vidTimer2 > 22.5f)
		{
			subtitleText->SetText("");
			ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLeavingCutScenePlayed= true;

			isOutroCutscnPlaying = false;
			cv_OutroCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P9_PeacockRideEnds])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P9_PeacockRideEnds] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L5P9_PeacockRideEnds;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			}

			Hud->ShowHud();

			//Clearing all the Level 5 Objectives
			ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();

			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1;
			ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_6;
			//Backup Save for level restart
			ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);
			SendMessage(ControlExit, ID, HOSM_NewArea);	
			
		} 
		return;
	}

	if (isIntroCutscnPlaying)
	{

		Cursor::SetMode(CA_Normal);
		time = cvIntroCutscn->GetTime();

		if (time >= subtitles[0].begin && time < subtitles[0].end)
		{
			subtitleText->SetText(subtitles[0].text);
		}
		else
			subtitleText->SetText("");
#ifdef FORCE_CLOSE_VIDEO
		vidTimer3 += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cvIntroCutscn->isEndReached() ||
#endif
			vidTimer3 > 14)
		{
			subtitleText->SetText("");
			isIntroCutscnPlaying = false;
			cvIntroCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();
			ProfileSystem::Instance->ActiveProfile.L5_S6_IntroCutScenePlayed = true;
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S6_HOTree;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
			
		} 
		return;
	}

	
	if( GFInstance->IsReplayingGame )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted) 
		{
			GFInstance->IsReplayingGame = false;
		}
		bool isPendingCollectableInLevel = false;			
		
		if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S2Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S4Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S5Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S6Collected )
		{
			isPendingCollectableInLevel = true;		
			Replay_WaitingForCollectablePickTicks = 1.0f;//till morph objects flying to awardspopup we initialize this wait timer to 2 sec!!
			IsReplay_WaitingForCollectablePick = true;
			if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle  )//avoid looped banner calls!!
				Hud->ShowBannerText("L1mismo");
		}
		else 
		{
			for( int i = 0; i < 5; i++ )				
				if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[(ProfileSystem::Instance->ActiveProfile.CurrentLevel*5)+i] )
				{
					isPendingCollectableInLevel = true;
					Replay_WaitingForCollectablePickTicks = 1.0f;//till morph objects flying to awardspopup we initialize this wait timer to 2 sec!!
					IsReplay_WaitingForCollectablePick = true;
					if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle )//avoid looped banner calls!!
							Hud->ShowBannerText("L1mismc");
					break;
				}
		}
		

		//No pending collectables during replaying!!
		if( !isPendingCollectableInLevel && ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed && !isOutroCutscnPlaying )
		{		
			//wait.. 1 sec regardless of collectable present or not!!
			if( IsReplay_WaitingForCollectablePick ) 
			{
				if( Replay_WaitingForCollectablePickTicks >= 0.0f )
					Replay_WaitingForCollectablePickTicks -= Control::LogicRate;
				if( Replay_WaitingForCollectablePickTicks <= 0.0f )
				{
					Replay_WaitingForCollectablePickTicks = 0.0f;
					IsReplay_WaitingForCollectablePick = false;

					for( int i = 0; i < ePopL5S6_Max; i++ )
						PopStates[i] = ePopupState_Inactive;
					ActivePopupID = ePopInactive;

					Hud->AllowInput = false;
					Hud->HideHud();		
					cv_OutroCutscn->PlayFadeInVideo(2.0f);
					isOutroCutscnPlaying = true;					
				}
			}
			
		}
	}
	


	

	cloudBG1->Pos.x -= Control::LogicRate*cloudbgSpeed1;
	cloudBG2->Pos.x -= Control::LogicRate*cloudbgSpeed2;

	if(cloudBG1->Pos.x<(-CLOUD_OVERFLOW_WIDTH))
	{
		cloudBG1->Pos.x = 1366.0f + CLOUD_OVERFLOW_WIDTH + rand()%50;
		cloudbgSpeed1 = 10.0f+float(rand()%2);
	}

	if(cloudBG2->Pos.x<(-CLOUD_OVERFLOW_WIDTH))
	{
		cloudBG2->Pos.x = 1366.0f + CLOUD_OVERFLOW_WIDTH + rand()%50;
		cloudbgSpeed2 = 11.0f+rand()%2;
	}
	
	cloudFG1->Pos.x -= Control::LogicRate*cloudfgSpeed1;
	

	if(cloudFG1->Pos.x<(-CLOUD_OVERFLOW_WIDTH))
	{
		cloudFG1->Pos.x = 1366.0f + CLOUD_OVERFLOW_WIDTH + rand()%50;
		cloudfgSpeed1 = 12.0f+rand()%2;
	}

	

	CPoint pos = Control::Input->Mpos();

	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_HOCompleted)
	{
		if(HORect.Intersection(&pos))
		{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S6_HOTree, 0, HORect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S6_HOTree;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
					
			}
			
		}
		return;
	}

	if( ActivePopupID == ePopInactive )
	{
		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		//popup click checks
		for (int i = 0; i < ePopL5S6_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_Look);
					if( Control::Input->LBclicked() )
					{
						Cursor::SetMode(CA_Normal);
						ActivePopupID = i;
						PopupArray[i]->OpenPopup();
					}
					return;
				}
			}
		}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
		#endif	
			if( IsMouseOverGoBackArea(&pos) )
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,-1);
#else
		Cursor::SetMode(CA_ExitDown,-1);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S5_MountainBelow, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
									
			}	
		}
	}
}

void L5_S6_MountainTop::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL5S6_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_HOCompleted)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S6_MountainTop] = isAnyTaskLeft;
}

void L5_S6_MountainTop::SparkleUpdate()
{
	if(isSparkleUpdatePending)
	{
		isSparkleUpdatePending = false;
		if(SparkleFX==NULL)
		{
			SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);		
		}
		//Game Logic Goes here
		numSparkles = 0;
		currentSparkleID = 0;

		//Check for Popup Tasks
		for (int i = 0; i < ePopL5S6_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopupArray[i]->isTaskLeft())
				{
					sparklePoints[numSparkles].SetXY(PopPolyArray[i]->center.x, PopPolyArray[i]->center.y);
					numSparkles++;
					isSparkleUpdatePending = true;
				}
			}
		}
	}
	sparkleTimer += Control::LogicRate;
	if(sparkleTimer>sparkleThreshold)
	{
		sparkleTimer = 0;
		if(numSparkles>0)
		{			
			SparkleFX->SetPos(&sparklePoints[currentSparkleID]);
			SparkleFX->Start();
			currentSparkleID += 1;
			if(currentSparkleID>=numSparkles)
				currentSparkleID = 0;
		}
	}
}

void L5_S6_MountainTop::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();
		if(ActivePopupID != ePopInactive)
		{
			if(PopupArray[ActivePopupID]!=NULL)
				PopupArray[ActivePopupID]->UpdateBgGraphic();
		}
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				//UpdatePopups(dropPos,true);
				//popup click checks
				for (int i = 0; i < ePopL5S6_Max; i++)
				{
					if(PopStates[i] != ePopupState_Inactive) //if popup is active
					{
						if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&dropPos))
						{
				
							ActivePopupID = i;
							PopupArray[i]->OpenPopup();
							
							return;
						}
					}
				}
				if(ActivePopupID == ePopInactive)
				{
					//If No popups responded

					GFInstance->WrongInvDropMsg(Hud);
				}
				return;

			}
			else
			{

				if(PopupArray[ActivePopupID]!=NULL)
					PopupArray[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal);
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L5_S6_MountainTop::ProcessSkip()
{
}

void L5_S6_MountainTop::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S6Collected)
		{
			InitHintTrailEmit(1060, 500);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}
	int iNonHOHint = 0; 
	int MaxNonHOHints = 5;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: ++iNonHOHint;
			if( ActivePopupID != ePopInactive )
			{
				PopupArray[ActivePopupID]->ProcessHint();
				return;
			}
			break;
		case 1: ++iNonHOHint;
			for (int i = 0; i < ePopL5S6_Max; i++)
			{
				if(PopStates[i] != ePopupState_Inactive) //if popup is active
				{
					if(PopupArray[i]->isTaskLeft())
					{
						InitHintTrailEmit(&PopPolyArray[i]->center,false);
						return;
					}
				}
			}
			break;
		case 2: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L5_S6_HOCompleted)
			{
			InitHintTrailEmit(&HORect);
			return;
			}
			break;
		
		case 3: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
		case 4: ++iNonHOHint;
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

L5_S6_MountainTop::~L5_S6_MountainTop()
{
	SAFE_DELETE(S6MoonprtFX);
	SAFE_DELETE(cvIntroCutscn);
	SAFE_DELETE(cv_PeacockCutscn);
	SAFE_DELETE(cv_OutroCutscn);
	SAFE_DELETE(subtitleText);

	for (int i = 0; i < ePopL5S6_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL5MaryRefS6_1);
	Control::Audio->StopSample(aAmbL5s6MountainTopAmb);
	Control::Audio->UnloadSample(aAmbL5s6MountainTopAmb);
	//MUSIC_SFX
}
void L5_S6_MountainTop::NavigateToScene()
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