//====================================
//  created by : Amal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S5_SophiaEnd.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "MeshObject.h"

//CRectangle L7_S5_RectVidFull(171, 96, 1024, 576);

L7_S5_SophiaEnd::L7_S5_SophiaEnd()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S5\\S5.SCN");

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);
	
	CRectangle L7_S5_RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	
	skyObj1 = NULL;
	skyObj1 = new MeshObject(HOScene->GetObjectByID("sky"), 5, 5);
	skyObj1->meshFX->SetSpeedAndDisplacement(4, 4, 2, 1);
	HOScene->InsertObject(skyObj1, 1);

	hairObj = NULL;
	if( GFInstance->IsReplayingGame  )//if ameeha cleared from tub!!
	{
		HOScene->GetObjectByID("sofiaSleep")->SetEnable(false);
		HOScene->GetObjectByID("ameehaHair")->SetEnable(false);
		HOScene->GetObjectByID("Ameeha")->SetEnable(false);
		HOScene->GetObjectByID("sofiaFloat")->SetEnable(false);
	}
	else
	{		
		hairObj = new MeshObject(HOScene->GetObjectByID("ameehaHair"), 5, 5);
		hairObj->meshFX->SetSpeedAndDisplacement(4, 4, 1, 1);
		HOScene->InsertObject(hairObj, 2);

		HOScene->GetObjectByID("Ameeha")->SetPivot(0,0);
		HOScene->GetObjectByID("Ameeha")->Animate(0,2,0,0,10.0f);

		HOScene->GetObjectByID("sofiaFloat")->SetPivot(0,0);
		HOScene->GetObjectByID("sofiaFloat")->Animate(0,5,0,0,10.0f);
	}
	s5SofiaCurseFX = NULL;
	s5SofiaCurseFX = new SuperFX("PRT\\L7\\s5SofiaCurse.INI", PointSystem::CreateCPoint(829,303,0), 100); 
	
	isEndCutscn1Playing = false;
	
	

	cv_EndCutscn1 = NULL;
	time = 0;
	subIndex = 0;
	subIndex2 = 0;
	subMax = 5;
	subMax2 = 8;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene5"));

	subtitles2[0] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_1"));
	subtitles2[1] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_2"));
	subtitles2[2] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_3"));
	subtitles2[3] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_4"));
	subtitles2[4] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_5"));
	subtitles2[5] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_6"));
	subtitles2[6] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_7"));
	subtitles2[7] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_8"));

	cv_EndCutscn1 = new CVideo("OGV\\L7\\S4ENDCUTSCENE1.OGV", L7_S5_RectVidFull, false, eZOrder_CutsceneFull);
	//cv_EndCutscn1->SetScale(1.334f);
	cv_EndCutscn1->SetScale(GFApp->video_aspectRatio);
	vidTimer = 0;
	vidTimer2 = 0;
	isEndCutscn2Playing = false;
	cv_EndCutscn2 = NULL;
	cv_EndCutscn2 = new CVideo("OGV\\L7\\S4ENDCUTSCENE2.OGV", L7_S5_RectVidFull, false, eZOrder_CutsceneFull);
	//cv_EndCutscn2->SetScale(1.334f);
	cv_EndCutscn2->SetScale(GFApp->video_aspectRatio);


	SceneObjectsUpdate();

	TaskPendingUpdate();

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s5SopiaAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s5SopiaAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s5SopiaAmb,true);
	}
	Control::Audio->LoadSample(aVOL7MaryRefS5_1,	AudioVO);
	//MUSIC_SFX
	
	//Show Objective completed
	if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L7O2_FindWayToSophia] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L7O2_FindWayToSophia] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L7O2_FindWayToSophia] = true;
		Hud->ShowObjectiveFX(eObj_L7O2_FindWayToSophia,true);
	}

	//Set Objective completed
	if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L7O3_FindWayToSaveAmeeha] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L7O3_FindWayToSaveAmeeha] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L7O3_FindWayToSaveAmeeha] = true;
	}
	

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S5_SophiaEnd]  = true;
}




void L7_S5_SophiaEnd::SceneObjectsUpdate()
{
	TaskPendingUpdate();
	if(ProfileSystem::Instance->ActiveProfile.L7_S5_End1CutScenePlayed && !ProfileSystem::Instance->ActiveProfile.L7_S5_End2CutScenePlayed )
	{
		
		HOScene->GetObjectByID("sofiaSleep")->SetEnable(false);
		
		
		HOScene->GetObjectByID("anandaBody")->SetEnable(true);
		HOScene->GetObjectByID("anandaCloth1")->SetEnable(true);
		HOScene->GetObjectByID("anandaHand")->SetEnable(true);
		HOScene->GetObjectByID("anandaHead")->SetEnable(true);
		HOScene->GetObjectByID("anandaLeg")->SetEnable(true);
		HOScene->GetObjectByID("anandaShadow")->SetEnable(true);

		HOScene->GetObjectByID("sofiaFloat")->SetEnable(true);
	
		
		/*s5SofiaCurseFX->Start();
		s5SofiaCurseFX->AdvanceByTime(2.0f);*/
		
		
	}
	else
	{
		
		HOScene->GetObjectByID("anandaBody")->SetEnable(false);
		HOScene->GetObjectByID("anandaCloth1")->SetEnable(false);
		HOScene->GetObjectByID("anandaHand")->SetEnable(false);
		HOScene->GetObjectByID("anandaHead")->SetEnable(false);
		HOScene->GetObjectByID("anandaLeg")->SetEnable(false);
		HOScene->GetObjectByID("anandaShadow")->SetEnable(false);

		HOScene->GetObjectByID("sofiaFloat")->SetEnable(false);
	
		
	}
}

void L7_S5_SophiaEnd::Required()
{
	HiddenObjectSystem::Required();	//forced update
	if(isEndCutscn1Playing || isEndCutscn2Playing)
		return;

	if( Control::Audio->IsPlaying(aTrackL7puzzle) )
		{
			Control::Audio->Samples[aTrackL7puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL7puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL7puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL7s5Theme) )
					  Control::Audio->PlaySample(aTrackL7s5Theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL7ho) )
		{
			Control::Audio->Samples[aTrackL7ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL7ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL7ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL7s5Theme) )
					  Control::Audio->PlaySample(aTrackL7s5Theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL7puzzle) )
		{
			Control::Audio->Samples[aTrackL7puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL7puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL7puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL7s5Theme) )
					  Control::Audio->PlaySample(aTrackL7s5Theme, true);
					  
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL7s5Theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL7s5Theme, true);
			}
		}
}

void L7_S5_SophiaEnd::Update()
{
	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	if( GFInstance->IsReplayingGame )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted) 
		{
			GFInstance->IsReplayingGame = false;
		}
		bool isPendingCollectableInLevel = false;
		if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S1Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S2Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S3Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S4Collected )
		{
			isPendingCollectableInLevel = true;					
			if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle  )//avoid looped banner calls!!
				Hud->ShowBannerText("L1mismo");
		}
		//No memory collectables in Level 7

		//No pending collectables during replaying!! force popup & exit!!
		if( !isPendingCollectableInLevel )
		{
			GFInstance->IsReplayingGame = false;//Force clear as we have collected all!!
			//Clearing all the Level 7 Objectives
			ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();						
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_Credits;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}

	CPoint pos = Control::Input->Mpos();

	{
		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		if (isEndCutscn1Playing)
		{
			//Subtitles Code
			time = cv_EndCutscn1->GetTime();

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
			Cursor::SetMode(CA_Normal);
#ifdef FORCE_CLOSE_VIDEO
			vidTimer += Control::LogicRate;
#endif
			if(
#ifndef FORCE_CLOSE_VID_END
				cv_EndCutscn1->isEndReached() ||
#endif
				vidTimer > 37)
			{
				subtitleText->SetText("");
				isEndCutscn1Playing = false;
				cv_EndCutscn1->StopAndFadeOutVideo(2.0f);
				Hud->AllowInput = true;
				Hud->ShowHud();

				//Journal Entry Add
				if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P10_DarkartaThrowsKid])
				{
					//Flag Set
					ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P10_DarkartaThrowsKid] = true;

					//Add to Note Array
					ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P10_DarkartaThrowsKid;

					ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

					//Increment Notes
					ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
					Hud->ShowJrnEntryFX();
				}

				ProfileSystem::Instance->ActiveProfile.L7_S5_End1CutScenePlayed = true;
				Control::Audio->PlaySample(aVOL7MaryRefS5_1);
				Hud->ShowBannerText("L7since");
				SceneObjectsUpdate();
			} 
			return;
		}

	if (isEndCutscn2Playing)
		{
			Cursor::SetMode(CA_Normal);
			//Subtitles Code
			time = cv_EndCutscn2->GetTime();

			if (time >= subtitles2[subIndex2].begin && time < subtitles2[subIndex2].end)
			{
				subtitleText->SetText(subtitles2[subIndex2].text);
			}
			else
				subtitleText->SetText("");

			if (time > subtitles2[subIndex2].end)
			{
				if(subMax2-1 > subIndex2)
					subIndex2++;
			}
#ifdef FORCE_CLOSE_VIDEO
			vidTimer2 += Control::LogicRate;
#endif
			if(
#ifndef FORCE_CLOSE_VID_END
				cv_EndCutscn2->isEndReached() ||
#endif
				vidTimer2 > 62)
			{
			
				isEndCutscn2Playing = false;
				cv_EndCutscn2->StopAndFadeOutVideo(2.0f);
				Hud->AllowInput = true;
				Hud->ShowHud();
				subtitleText->SetText("");
				//Clearing all the Level 7 Objectives
				ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();

				ProfileSystem::Instance->ActiveProfile.L7_S5_End2CutScenePlayed = true;
				ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted =  true;
				SceneObjectsUpdate();
				//Hud->BackToMainMenu();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_Credits;
				//Backup Save for level restart
				ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);
				SendMessage(ControlExit, ID, HOSM_NewArea);
			} 
			return;
		}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S5_End1CutScenePlayed)
	{
		if( Control::Input->LBclicked() )
		{
			isEndCutscn1Playing = true;
			Hud->AllowInput = false;
			Hud->HideHud();		
			cv_EndCutscn1->PlayFadeInVideo(2.0f);
			
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S5_End2CutScenePlayed)
	{
		if( Control::Input->LBclicked() )
		{
			isEndCutscn2Playing = true;
			Hud->AllowInput = false;
			Hud->HideHud();		
			cv_EndCutscn2->PlayFadeInVideo(2.0f);
			
		}
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		// Sac: no action for Back button is defined below, need to check.				
	}	
	#endif	

		if( IsMouseOverGoBackArea(&pos) )
		{
			Cursor::SetMode(CA_ExitDown,-1);
			if( Control::Input->LBclicked() )
			{
				//Control::Audio->PlayFootStepsFX();
				//ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_CaveMouth;
				//SendMessage(ControlExit, ID, HOSM_NewArea);						
			}	
		}
	}
}

void L7_S5_SophiaEnd::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S5_SophiaEnd] = isAnyTaskLeft;
}

void L7_S5_SophiaEnd::SparkleUpdate()
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

void L7_S5_SophiaEnd::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();		
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
			//GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L7_S5_SophiaEnd::ProcessSkip()
{
}

void L7_S5_SophiaEnd::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 4;
	do
	{
		switch (iNonHOHint)
		{
		default:
			//Exit coordinates
			break;
		case 0:
			if (!ProfileSystem::Instance->ActiveProfile.L7_S5_End2CutScenePlayed || !ProfileSystem::Instance->ActiveProfile.L7_S5_End1CutScenePlayed)
			{
				if(ProfileSystem::Instance->ActiveProfile.L7_S5_End1CutScenePlayed)
					InitHintTrailEmit(830, 270);
				else
					InitHintTrailEmit(700, 545);
				return;
			}
			break;
		case 1: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 2: ++iNonHOHint;
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

L7_S5_SophiaEnd::~L7_S5_SophiaEnd()
{	
	SAFE_DELETE(cv_EndCutscn1);
	SAFE_DELETE(cv_EndCutscn2);
	SAFE_DELETE(subtitleText);
	SAFE_DELETE(s5SofiaCurseFX);
		//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL7MaryRefS5_1);
	Control::Audio->StopSample(aAmbL7s5SopiaAmb);
	Control::Audio->UnloadSample(aAmbL7s5SopiaAmb);
	//MUSIC_SFX
}
