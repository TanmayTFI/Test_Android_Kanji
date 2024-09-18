//====================================
//  created by : Amal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S1_Volcano.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "L7S1_Pop2Well.h"
#include "L7S1_Pop1Flower.h"

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PuzzleRect(210.0f,270.0f,186.0f,118.0f);
const CRectangle  sofiaRect(469.0f,453.0f,245.0f,142.0f);
const CRectangle  SceneRectHeaven(1050.0f,100.0f,170.0f,151.0f);
const CRectangle  InfoRectMoon(422.0f,10.0f,182.0f,146.0f);



L7_S1_Volcano::L7_S1_Volcano()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S1\\S1.SCN");

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	HOScene->GetObjectByID("chain1")->SetPivot(240,-57);
	HOScene->GetObjectByID("chain1")->Animate(0,0,2,0,10.0f);

	HOScene->GetObjectByID("chain2")->SetPivot(-246,110);
	HOScene->GetObjectByID("chain2")->Animate(0,0,2,0,9.0f);

	HOScene->GetObjectByID("chain3")->SetPivot(-313,-140);
	HOScene->GetObjectByID("chain3")->Animate(0,0,2,0,10.0f);

	HOScene->GetObjectByID("chain4")->SetPivot(-251,-63);
	HOScene->GetObjectByID("chain4")->Animate(0,0,2,0,12.0f);


	if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S2_LasyaPrison] )
	{
		HOScene->GetObjectByID("sofiaHand")->SetEnable(false);
		HOScene->GetObjectByID("sofiaBody")->SetEnable(false);
		HOScene->GetObjectByID("sofiaFace")->SetEnable(false);
		HOScene->GetObjectByID("sofiaHair")->SetEnable(false);
	}

	isAxeCutscnPlaying = false;
	
	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	cv_AxeCutscn = new CVideo("OGV\\L7\\S1AXECLIMBING.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//cv_AxeCutscn->SetScale(1.334f);
	cv_AxeCutscn->SetScale(GFApp->video_aspectRatio);
	cv_AxeCutscn->SetSkipActive();

	isSofiaCutscnPlaying = false;
	vidTimer = 0;
	vidTimer2 = 0;
	cv_SofiaCutscn = new CVideo("OGV\\L7\\S1LASYAILLUTION.OGV", RectVidFull, false, eZOrder_CutsceneFull);

	time = 0;
	subIndex = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("lasyaillusion"));

	//cv_SofiaCutscn->SetScale(1.334f);
	cv_SofiaCutscn->SetScale(GFApp->video_aspectRatio);
	cv_SofiaCutscn->SetSkipActive();

	

	s1lava1FX = new SuperFX("PRT\\L7\\s1lava1.INI", PointSystem::CreateCPoint(1278,376,0), 100); 
	s1lava1FX->Start();
	s1lava1FX->AdvanceByTime(2.0f);

	s1lava12FX = new SuperFX("PRT\\L7\\s1lava12.INI", PointSystem::CreateCPoint(951,598,0), 100); 
	s1lava12FX->Start();
	s1lava12FX->AdvanceByTime(2.0f);

	s1lava13FX = new SuperFX("PRT\\L7\\s1lava13.INI", PointSystem::CreateCPoint(756,370,0), 100); 
	s1lava13FX->Start();
	s1lava13FX->AdvanceByTime(2.0f);

	s1lava14FX = new SuperFX("PRT\\L7\\s1lava14.INI", PointSystem::CreateCPoint(724,119,0), 100); 
	s1lava14FX->Start();
	s1lava14FX->AdvanceByTime(2.0f);

	#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S1Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		const CRectangle  MorphRect(393.0f,208.0f,122.0f,116.0f);

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
	Control::Audio->LoadSample(aAmbL7s1VolcanoAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s1VolcanoAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s1VolcanoAmb,true);
	}
	
	//MUSIC_SFX

	if (ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePuzzleCompleted)
	{
		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L7O1_FindWayOutOfLasyaPrison] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L7O1_FindWayOutOfLasyaPrison] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L7O1_FindWayOutOfLasyaPrison] = true;
			Hud->ShowObjectiveFX(eObj_L7O1_FindWayOutOfLasyaPrison,true);
		} 
	}

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P12_SophiaMemoriesEnd])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P12_SophiaMemoriesEnd] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P12_SophiaMemoriesEnd;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;	

		Hud->ShowJrnEntryFX();
	}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S1_Volcano]  = true;
}

void L7_S1_Volcano::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL7S1_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}



	//Popup 1
	if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedFlowerTaken)
	{
		int p1rect[] =  {836,98,1023,88,1017,285,837,300};
		PopPolyArray[ePop1_Flower] =  new CPolygon(4);
		PopPolyArray[ePop1_Flower]->Init(p1rect);

		PopupArray[ePop1_Flower] = new L7S1_Pop1Flower(ePop1_Flower,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop1_Flower]->center, CC_CALLBACK_1(L7_S1_Volcano::OnPopupClosed, this));
		PopupArray[ePop1_Flower]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop1_Flower] = ePopupState_Closed;
	}

	//Popup 2
	if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
	{
		int p2rect[] ={188,36,276,10,359,79,303,175,213,182};
		PopPolyArray[ePop2_Well] =  new CPolygon(5);
		PopPolyArray[ePop2_Well]->Init(p2rect);

		PopupArray[ePop2_Well] = new L7S1_Pop2Well(ePop2_Well,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop2_Well]->center, CC_CALLBACK_1(L7_S1_Volcano::OnPopupClosed, this));
		PopupArray[ePop2_Well]->SetPopupBoundingBox(PopAreaRect);
		if(ProfileSystem::Instance->ActiveProfile.L7_S1_InvAxeUsed)
			PopStates[ePop2_Well] = ePopupState_Closed;
		else
			PopStates[ePop2_Well] = ePopupState_Inactive;
	}
}

void L7_S1_Volcano::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	SceneObjectsUpdate();
	switch (popup->GetPopupId())
	{
	case 0:
		if(ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedFlowerTaken)
		{
			//disable popup
			
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 1:
		if(ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
		{
			//disable popup
			
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		else if(ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed && !ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened)
		{
			Hud->InvBox_StopPuzzleMode();
		}
		break;
	}
}

void L7_S1_Volcano::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if( !ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted )	
	{
		HOScene->GetObjectByID("afterPuzzle")->SetEnable(false);
		HOScene->GetObjectByID("beforePuzzle")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("afterPuzzle")->SetEnable(true);
		HOScene->GetObjectByID("beforePuzzle")->SetEnable(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened)
	{
		HOScene->GetObjectByID("wheelDoorOpen")->SetEnable(false);
		HOScene->GetObjectByID("water")->SetEnable(false);
		HOScene->GetObjectByID("wheelDoorClosed")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("wheelDoorOpen")->SetEnable(true);
		HOScene->GetObjectByID("water")->SetEnable(true);
		HOScene->GetObjectByID("wheelDoorClosed")->SetEnable(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedStarUsed )	
	{
		HOScene->GetObjectByID("doorOpen")->SetEnable(false);
		HOScene->GetObjectByID("doorClosed")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("doorOpen")->SetEnable(true);
		HOScene->GetObjectByID("doorClosed")->SetEnable(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L7_S1_WaterUsedOnRedFlower )	
	{
		HOScene->GetObjectByID("flowerOpen")->SetEnable(false);
		HOScene->GetObjectByID("flowerClose")->SetEnable(true);
	}
	else
	{
		if( !ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedFlowerTaken )	
			HOScene->GetObjectByID("flowerOpen")->SetEnable(true);
		else
			HOScene->GetObjectByID("flowerOpen")->SetEnable(false);

		HOScene->GetObjectByID("flowerClose")->SetEnable(false);
	}

}

void L7_S1_Volcano::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(isAxeCutscnPlaying || isSofiaCutscnPlaying)
		return;

	if( Control::Audio->IsPlaying(aTrackL7ho) )
		{
			Control::Audio->Samples[aTrackL7ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL7ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL7ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL7theme) )
					  Control::Audio->PlaySample(aTrackL7theme, true);
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
				 if( !Control::Audio->IsPlaying(aTrackL7theme) )
					  Control::Audio->PlaySample(aTrackL7theme, true);
					  
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL7theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL7theme, true);
			}
		}
}

void L7_S1_Volcano::Update()
{
	NavigateToScene();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	if (isAxeCutscnPlaying)
	{
		Cursor::SetMode(CA_Normal);
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_AxeCutscn->isEndReached() ||
#endif
			vidTimer > 9)
		{
			
			isAxeCutscnPlaying = false;
			cv_AxeCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->ShowBannerText("L7top");
			PopStates[ePop2_Well] = ePopupState_Closed;
			ActivePopupID = ePop2_Well;
			PopupArray[ePop2_Well]->OpenPopup();
		} 
		return;
	}

	if (isSofiaCutscnPlaying)
	{
		Cursor::SetMode(CA_Normal);
		//Subtitles Code
		time = cv_SofiaCutscn->GetTime();

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");
#ifdef FORCE_CLOSE_VIDEO
		vidTimer2 += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_SofiaCutscn->isEndReached() ||
#endif
			vidTimer2 > 16)
		{
			subtitleText->SetText("");
			isAxeCutscnPlaying = false;
			cv_SofiaCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
			SendMessage(ControlExit, ID, HOSM_NewArea);		
		} 
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if( ActivePopupID == ePopInactive )
	{
		
		#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed()) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
		#endif
		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S2_LasyaPrison] )
		{
			if(sofiaRect.Intersection(&pos))
				Cursor::SetMode(CA_HandPointy);
			if( Control::Input->LBclicked() )
			{
				Hud->AllowInput = false;
				Hud->HideHud();		
				cv_SofiaCutscn->PlayFadeInVideo(2.0f);
				isSofiaCutscnPlaying = true;
			}
			return;
		}

		//popup click checks
		for (int i = 0; i < ePopL7S1_Max; i++)
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
						if(i== ePop2_Well && ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed && !ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened)
						{
							Hud->InvBox_StartPuzzleMode();
						}
					}
					
					return;
				}
			}
		}
		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvAxeUsed && (PopPolyArray[ePop2_Well]!=NULL && PopPolyArray[ePop2_Well]->Intersection(&pos)) )
		{
			Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L7reach");
				}
				return;
		}
		if(InfoRectMoon.Intersection(&pos))
		{
			
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L7creep");
				}
				return;
		}
		if( !ProfileSystem::Instance->ActiveProfile.L7_S1_PZStarDoorOpened[0] )	
		{
			if(PuzzleRect.Intersection(&pos))
			{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S1_PZJigsawYellow, 0, PuzzleRect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_PZJigsawYellow;
					SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
						
				}
				return;
			}
			
			
		}
		if( !(ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[0] && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[1] && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[2])  )	
		{
			if(sofiaRect.Intersection(&pos))
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S1_PZStarPuzzle, 0, sofiaRect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_PZStarPuzzle;
					SendMessage(ControlExit, ID, HOSM_NewArea);		
#endif
					
				}
				return;
			}
			
			
		}
		if(SceneRectHeaven.Intersection(&pos))
			{
				
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitUp, eArea_L7_S3_Haven);
#else
		Cursor::SetMode(CA_ExitUp, eArea_L7_S3_Haven);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S3_Haven, 0, SceneRectHeaven);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_Haven;
					SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
						
				}
				return;
			}
		
	}
}

void L7_S1_Volcano::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL7S1_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if( (!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[0] && ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted ) || (! ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[1] && ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted) || (!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[2] && ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted))	
			isAnyTaskLeft = true;	
	if(!ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted)
			isAnyTaskLeft = true;	

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano] = isAnyTaskLeft;

	if( !ProfileSystem::Instance->ActiveProfile.L7_S4_InvWaterUsedOnBlueFlower && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] = true;

	if( !ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S3_Haven] = true;
	if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S5_SophiaEnd] && (!ProfileSystem::Instance->ActiveProfile.L7_S5_End2CutScenePlayed || !ProfileSystem::Instance->ActiveProfile.L7_S5_End1CutScenePlayed))
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S5_SophiaEnd] = true;
}

void L7_S1_Volcano::SparkleUpdate()
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
		for (int i = 0; i < ePopL7S1_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopupArray[i]->isTaskLeft())
				{
					sparklePoints[numSparkles].SetXY(PopPolyArray[i]->center.x, PopPolyArray[i]->center.y);
					numSparkles++;
				}
			}
		}

		if( (!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[0] && ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted ) || (! ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[1] && ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted) || (!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[2] && ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted))	
		{
			sparklePoints[numSparkles].SetXY(sofiaRect.GetCenterX(), sofiaRect.GetCenterY());
			numSparkles++;
		}

		if (ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken && !ProfileSystem::Instance->ActiveProfile.L7_S1_InvAxeUsed)
		{
			sparklePoints[numSparkles].SetXY(PopPolyArray[ePop2_Well]->center.x, PopPolyArray[ePop2_Well]->center.y);
			numSparkles++;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted)
		{
			sparklePoints[numSparkles].SetXY(PuzzleRect.GetCenterX(), PuzzleRect.GetCenterY());
			numSparkles++;
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

void L7_S1_Volcano::ReceiveMessage(int val)
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
				if (itemVal == eInv_L7_S3_Axes && PopPolyArray[ePop2_Well]->Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L7_S1_InvAxeUsed)
				{
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					ProfileSystem::Instance->ActiveProfile.L7_S1_InvAxeUsed= true;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S3_Axes] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S3_Axes);
					Hud->AllowInput = false;
					Hud->HideHud();		
					cv_AxeCutscn->PlayFadeInVideo(2.0f);
					isAxeCutscnPlaying = true;
				}
				else if(ActivePopupID == ePopInactive)
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

void L7_S1_Volcano::ProcessSkip()
{
	if (ActivePopupID == ePop2_Well)
	{
		((L7S1_Pop2Well*)PopupArray[ActivePopupID])->ProcessSkip();
	}
}

void L7_S1_Volcano::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S1Collected)
		{
			InitHintTrailEmit(450, 260);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 7;
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
		case 3: ++iNonHOHint;
			for (int i = 0; i < ePopL7S1_Max; i++)
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
			if( (!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[0] && ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted ) || (! ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[1] && ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted) || (!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[2] && ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted))	
				{
				InitHintTrailEmit(&sofiaRect);
				return;
				}

			if (ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken && !ProfileSystem::Instance->ActiveProfile.L7_S1_InvAxeUsed)
			{
				Hud->ShowHintForInvItem(eInv_L7_S3_Axes, &PopPolyArray[ePop2_Well]->center);
				return;
			}

				break;
		case 1: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S2_LasyaPrison])
			{
				InitHintTrailEmit(&sofiaRect);
				return;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted)
				{
				InitHintTrailEmit(&PuzzleRect);
				return;
				}
				break;
		case 4: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S3_Haven] || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S3_Haven])
			{
			InitHintTrailEmit(&SceneRectHeaven,true,CA_ExitUp);
			return;
			}
			break;
		case 5: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 6: ++iNonHOHint;
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

L7_S1_Volcano::~L7_S1_Volcano()
{
	SAFE_DELETE(cv_AxeCutscn);
	SAFE_DELETE(cv_SofiaCutscn);

	SAFE_DELETE(subtitleText);
	SAFE_DELETE(s1lava1FX);
	SAFE_DELETE(s1lava12FX);
	SAFE_DELETE(s1lava13FX);
	SAFE_DELETE(s1lava14FX);

	for (int i = 0; i < ePopL7S1_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s1VolcanoAmb);
	Control::Audio->UnloadSample(aAmbL7s1VolcanoAmb);
	//MUSIC_SFX
}
void L7_S1_Volcano::NavigateToScene()
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