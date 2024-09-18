//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL4 - SCENE4 : HILL TOP
//====================================

#include "L4_S4_HillTop.h"
#include "MeshObject.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  SceneRectHillSide(551.0f,3.0f,453.0f,452.0f);
const CRectangle  CaveRect(362.0f,94.0f,197.0f,242.0f);

const CRectangle  HillSideRect(551.0f,3.0f,453.0f,452.0f);

enum eL4S4HOPopEnums
{
	eL4S4Pop_Inactive = -1,
	eL4S4Pop1_StoneMill,
	eL4S4Pop3_Skeleton,
	eL4S4Pop_Max
};

L4_S4_HillTop::L4_S4_HillTop()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S4\\S4.SCN");

	int Pop1ArrayPts[] = {255,390,331,361,407,392,435,481,281,461,254,392};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL4S4Pop1_StoneMill, 6, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L4S4_Pop1StoneMill *pop1 = new  L4S4_Pop1StoneMill(eL4S4Pop1_StoneMill, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S4_HillTop::OnPopupClosed, this));
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);
	popClickPoly = NULL;

	int Pop3ArrayPts[] = {1116,342,1228,383,1170,485,1028,443,1115,336};
	popClickPoly = new ClickablePolygon(eL4S4Pop3_Skeleton, 5, Pop3ArrayPts);
	PopRectArray.push_back(popClickPoly);
	pop3 = new  L4S4_Pop3Skeleton(eL4S4Pop3_Skeleton, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S4_HillTop::OnPopupClosed, this));
	pop3->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop3);
	popClickPoly = NULL;

	ActivePopUpId = eL4S4Pop_Inactive;
	mo_treeface = NULL;
	mo_treeface = new MeshObject(HOScene->GetObjectByID("TreeFace"), 5, 5);
	mo_treeface->meshFX->SetSpeedAndDisplacement(1, 2, 4, 6);
	HOScene->InsertObject(mo_treeface,2);

	HOScene->GetObjectByID("ScaryFaceGLow01")->Animate(0.5f,1.5f,2);
	HOScene->GetObjectByID("ScaryFaceGLow02")->Animate(0.5f,1.5f,2);
	HOScene->GetObjectByID("BlueGlowingPlants")->Animate(0.85f,1.2f,4);
	HOScene->GetObjectByID("Magic-plant_Glitters")->Animate(0.8f,1.5f,6);
	HOScene->GetObjectByID("Magic-plant_Glitters-02")->Animate(0.8f,1.5f,6);

	HOScene->GetObjectByID("TreeBranch_Anim")->SetPivot(-125,81);
	HOScene->GetObjectByID("TreeBranch_Anim")->Animate(0,0,1.0,0,7,0);
	HOScene->GetObjectByID("LeftPlant02_Leaves")->Animate(0.5,1,0,0,5,0);
	HOScene->GetObjectByID("LeftPlant02_Flower")->Animate(0.5,1,0,0,5,0);

	HOScene->GetObjectByID("YellowMushroom")->SetPivot(-65,133);
	HOScene->GetObjectByID("YellowMushroom")->Animate(0,0,2.5,0,5,0);

	isVideoPlaying = false;
	lpVidClimbUp = NULL;

	time = 0;
	subIndex = 0;
	subIndex2 = 0;
	subMax = 1;
	subMax2 = 1;
	vidTimer = 0;
	vidTimer2 = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("maryjump"));

	subtitles2[0] = AssetsStringTable::getSub(AssetsStringTable::getString("maryclimb"));

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	lpVidClimbUp = new CVideo("OGV\\L4\\S4MARYCLIMB.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpVidClimbUp->SetScale(1.334f);
	lpVidClimbUp->SetScale(GFApp->video_aspectRatio);

	lpVidJumpDown = NULL;
	lpVidJumpDown = new CVideo("OGV\\L4\\S4MARYRUNANDJUMB.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpVidJumpDown->SetScale(1.334f);
	lpVidJumpDown->SetScale(GFApp->video_aspectRatio);

	fallDone = false;
	bannerDelayTimer = -1;

	/*HOScene->GetObjectByID("LeftPlant02_Leaves")->SetPivot(103,79);
	HOScene->GetObjectByID("LeftPlant02_Leaves")->Animate(0,0,4,0,7,0);
	*/
#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S4Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(321.0f,200.0f,84.0f,128.0f);

		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif // _CEBUILD

	InitParticles();

	SceneObjectsUpdate();

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aVOL4MaryRefS4_3,	AudioVO);
	Control::Audio->LoadSample(aAmbL4s4HillTopAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s4HillTopAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s4HillTopAmb,true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularCompleted && !ProfileSystem::Instance->ActiveProfile.L4_S4_BannerTextAfterBinocularHOShown)
	{
		Hud->ShowBannerText("L4might");
		Control::Audio->PlaySample(aVOL4MaryRefS4_3);
		ProfileSystem::Instance->ActiveProfile.L4_S4_BannerTextAfterBinocularHOShown = true;
	}

	//MUSIC_SFX
}

bool L4_S4_HillTop::isPopupActive(int popId)
{
	switch (popId)
	{
	case eL4S4Pop1_StoneMill:	
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken || !ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken)
		{
			return true;
		}
#ifdef _CEBUILD
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf])
			return true;
#endif
		break;

	case eL4S4Pop3_Skeleton:	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularTaken)	return true;	break;
	default:
		break;
	}

	return false;
}


void L4_S4_HillTop::InitPopups()
{

}

void L4_S4_HillTop::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopUpId = eL4S4Pop_Inactive;

	switch (popup->GetPopupId())
	{
	case 0:
		break;
	case 1:
		break;
	}
}

void L4_S4_HillTop::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	HOScene->GetObjectByID("Olive")->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced && !ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated)
		HOScene->GetObjectByID("Olive")->SetEnable(true);
	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced)
	{
		HOScene->GetObjectByID("Bowl")->SetEnable(false);
		HOScene->GetObjectByID("BowlWithOlive")->SetEnable(false);
	}
	else if( ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated)
	{
		HOScene->GetObjectByID("Bowl")->SetEnable(false);
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken)
			HOScene->GetObjectByID("BowlWithOlive")->SetEnable(true);
		else
			HOScene->GetObjectByID("BowlWithOlive")->SetEnable(false);
	}
	else
		HOScene->GetObjectByID("Bowl")->SetEnable(true);
	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced)
		HOScene->GetObjectByID("Handle")->SetEnable(false);
	else
		HOScene->GetObjectByID("Handle")->SetEnable(true);

	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced )
	{
		/*HOScene->GetObjectByID("BridgeMask")->SetEnable(false);
		HOScene->GetObjectByID("DoorBridge")->SetEnable(false);*/
		HOScene->GetObjectByID("Bridge")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("Bridge")->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken)
	{
		HOScene->GetObjectByID("Door")->SetEnable(false);
	}

	HOScene->GetObjectByID("Mill_inclined")->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced)
	{
		HOScene->GetObjectByID("Mill_narmal")->SetEnable(true);
		//HOScene->GetObjectByID("Mill_inclined")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("Mill_narmal")->SetEnable(false);
		//HOScene->GetObjectByID("Mill_inclined")->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved)
	{
		HOScene->GetObjectByID("Sofia")->SetEnable(false);
	}
}

void L4_S4_HillTop::InitParticles()
{
	WaterFallFX_1 = NULL;
	WaterFallFX_2 = NULL;
	SnowFX_L = NULL;
	SnowFX_M = NULL;
	SnowFX_R = NULL;
	PlantGlowFX_L = NULL;
	PlantGlowFX_M = NULL;
	PlantGlowFX_R = NULL;

	WaterFallFX_1 = new SuperFX("PRT\\L4\\S4_WaterFall.INI", PointSystem::CreateCPoint(1145,132,0), 19); 
	WaterFallFX_1->Start();
	WaterFallFX_1->AdvanceByTime(2.0f);

	WaterFallFX_2 = new SuperFX("PRT\\L4\\S4_WaterFall_R.INI", PointSystem::CreateCPoint(1255,100,0), 19); 
	WaterFallFX_2->Start();
	WaterFallFX_2->AdvanceByTime(2.0f);

	SnowFX_L = new SuperFX("PRT\\L4\\S4_Snow_Left.INI", PointSystem::CreateCPoint(300,125,0), 19); 
	SnowFX_L->Start();
	SnowFX_L->AdvanceByTime(2.0f);

	SnowFX_M = new SuperFX("PRT\\L4\\S4_Snow_Middle.INI", PointSystem::CreateCPoint(750,450,0), 19); 
	SnowFX_M->Start();
	SnowFX_M->AdvanceByTime(2.0f);

	SnowFX_R = new SuperFX("PRT\\L4\\S4_Snow_Right.INI", PointSystem::CreateCPoint(1130,300,0), 19); 
	SnowFX_R->Start();
	SnowFX_R->AdvanceByTime(2.0f);

	PlantGlowFX_L = new SuperFX("PRT\\L4\\S4_PlantGlow_L.INI", PointSystem::CreateCPoint(50,350,0), 19); 
	PlantGlowFX_L->Start();
	PlantGlowFX_L->AdvanceByTime(2.0f);

	PlantGlowFX_M = new SuperFX("PRT\\L4\\S4_PlantGlow_M.INI", PointSystem::CreateCPoint(1150,500,0), 19); 
	PlantGlowFX_M->Start();
	PlantGlowFX_M->AdvanceByTime(2.0f);

	PlantGlowFX_R = new SuperFX("PRT\\L4\\S4_PlantGlow_R.INI", PointSystem::CreateCPoint(900,280,0), 19); 
	PlantGlowFX_R->Start();
	PlantGlowFX_R->AdvanceByTime(2.0f);
}

L4_S4_HillTop::~L4_S4_HillTop()
{
	for (int i = 0; i < (int)PopupArray.size(); i++)
	{
		SAFE_DELETE(PopupArray.at(i));
	}
	PopupArray.clear();

	SAFE_DELETE(WaterFallFX_1);
	SAFE_DELETE(WaterFallFX_2);
	SAFE_DELETE(SnowFX_L);
	SAFE_DELETE(SnowFX_M);
	SAFE_DELETE(SnowFX_R);
	SAFE_DELETE(PlantGlowFX_L);
	SAFE_DELETE(PlantGlowFX_M);
	SAFE_DELETE(PlantGlowFX_R);
	SAFE_DELETE(lpVidJumpDown);
	SAFE_DELETE(lpVidClimbUp);
	SAFE_DELETE(subtitleText);

	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL4MaryRefS4_3);
	Control::Audio->StopSample(aAmbL4s4HillTopAmb);
	Control::Audio->UnloadSample(aAmbL4s4HillTopAmb);
	//MUSIC_SFX
}

void L4_S4_HillTop::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(pop3->isVideoPlaying || isVideoPlaying)
		return;

	if( Control::Audio->IsPlaying(aTrackL4ho) )
	{
		Control::Audio->Samples[aTrackL4ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL4ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL4ho);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL4theme) )
				Control::Audio->PlaySample(aTrackL4theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL4puzzle) )
	{
		Control::Audio->Samples[aTrackL4puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL4puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL4puzzle);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL4theme) )
				Control::Audio->PlaySample(aTrackL4theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL4theme))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL4theme, true);
		}
	}
}

void L4_S4_HillTop::Update()
{
	//Cheats goes here
#ifdef _TABINGAMEUI
	if(_TabInGameMenuShowing)
		return;
#endif // _TABINGAMEUI
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif

	NavigateToScene();
	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	if (ActivePopUpId != eL4S4Pop_Inactive)
		return;

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif // _CEBUILD

	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_Visited)
		ProfileSystem::Instance->ActiveProfile.L4_S4_Visited = true;

	if (bannerDelayTimer)
	{
		bannerDelayTimer++;
		if (bannerDelayTimer == 360)
		{
			Hud->ShowBannerText("L4no");
			bannerDelayTimer = -1;
		}
	}

	if (isVideoPlaying )
	{
		//Subtitles Code
		if (!fallDone) 
		{
			time = lpVidJumpDown->GetTime();

			if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
			{
				subtitleText->SetText(subtitles[subIndex].text);
			}
			else
				subtitleText->SetText("");

#ifdef FORCE_CLOSE_VIDEO
			vidTimer += Control::LogicRate;
#endif
		}
		else
		{
			time = lpVidClimbUp->GetTime();

			if (time >= subtitles2[subIndex2].begin && time < subtitles2[subIndex2].end)
			{
				subtitleText->SetText(subtitles2[subIndex2].text);
			}
			else
				subtitleText->SetText("");	

#ifdef FORCE_CLOSE_VIDEO
			vidTimer += Control::LogicRate;
#endif
		}
		if((
#ifndef FORCE_CLOSE_VID_END
			lpVidJumpDown->isEndReached() ||
#endif
			vidTimer > 9) && !ProfileSystem::Instance->ActiveProfile.L4_S4_JumpCutscenePlayed)
		{
			subtitleText->SetText("");
			ProfileSystem::Instance->ActiveProfile.L4_S4_JumpCutscenePlayed = true;
			//isVideoPlaying = false;
			lpVidJumpDown->StopAndFadeOutVideo(2.0f);					
			/*	Hud->AllowInput = true;
			Hud->ShowHud();*/

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L4P2_MaryJumpGap])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L4P2_MaryJumpGap] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L4P2_MaryJumpGap;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			Hud->HideMessageBanner();
			Hud->SendUpdateSceneMessage();
			lpVidClimbUp->SetSkipActive();
			lpVidClimbUp->PlayVideo();
			fallDone = true;
		} 

		if((
#ifndef FORCE_CLOSE_VID_END
			lpVidClimbUp->isEndReached() ||
#endif
			vidTimer2 > 7) && !ProfileSystem::Instance->ActiveProfile.L4_S4_ClimbCutscenePlayed)
		{
			subtitleText->SetText("");
			ProfileSystem::Instance->ActiveProfile.L4_S4_ClimbCutscenePlayed = true;
			isVideoPlaying = false;
			lpVidClimbUp->StopAndFadeOutVideo(2.0f);					
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->SendUpdateSceneMessage();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S5_HillSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		} 
		return;
	}

	CPoint pos = Control::Input->Mpos();
	for (int i = 0; i < eL4S4Pop_Max; i++)
	{
		if( PopRectArray[i]->Intersection(&pos) && isPopupActive(i))
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				ActivePopUpId = i;
				PopupArray[i]->OpenPopup();
			}
		} 
	}

	if( HillSideRect.Intersection(&pos))
	{
		if (ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced)
		{
			
			#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitUp,eArea_L4_S5_HillSide);
#else
		Cursor::SetMode(CA_ExitUp,eArea_L4_S5_HillSide);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S5_HillSide, 0, HillSideRect);
			}
#else			
			Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S5_HillSide;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}
		}
		else if (ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularCompleted)
		{
			Cursor::SetMode(CA_ExitUp,eArea_L4_S5_HillSide);
			if( Control::Input->LBclicked() )
			{	
				bannerDelayTimer = 1;
				Hud->AllowInput = false;
				Hud->HideHud(false);
				lpVidJumpDown->SetSkipActive();
				isVideoPlaying = true;
				lpVidJumpDown->PlayVideo();
				Hud->ShowBannerText("L4can");
			}
		}
		else if (ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularWithLensTaken)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L4bino");			
			}
		}
		else if(Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4cross");
		}
	}
	else if(CaveRect.Intersection(&pos) && Control::Input->LBclicked() && !ProfileSystem::Instance->ActiveProfile.L4_S5_Visited)
	{
		Hud->ShowBannerText("L4believe");
	}

	else if( IsMouseOverGoBackArea(&pos) )
	{
		
		#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L4_S2_RiverSide);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L4_S2_RiverSide);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S2_RiverSide, 2);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif	

}

void L4_S4_HillTop::Render(const CPoint* pCam)
{

}


void L4_S4_HillTop::TakeInventoryItem(EHOInventory ItemID)
{

}


void L4_S4_HillTop::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		//TaskPendingUpdate();
		Hud->ShowMap();
	} 
	else if( val == MEMORY_OBJFX_REACHED ) 
	{
		pop3->startMemoryCutscene();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			TaskPendingUpdate();

			if (ActivePopUpId != ePopInactive)
			{
				PopupArray[ActivePopUpId]->HandleInventoryDrop(dropPos, itemVal);
				return;
			}
			else if (itemVal == eInv_L4_CB_Binocular)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularCompleted)
				{
					if (HillSideRect.Intersection(&dropPos) || CaveRect.Intersection(&dropPos))
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S4_BinocularHO;
						SendMessage(ControlExit, ID, HOSM_NewArea);	
					}
					else
						Hud->CancelDrag();
				}
				else
					Hud->CancelDrag();
			}
			else
				//If no drop items in this scene !!
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}	
}

float L4_S4_HillTop::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L4_S4_HillTop::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < (int)(PopupArray.size()); i++)
	{
		if(isPopupActive(i)) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularWithLensTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularCompleted)
		isAnyTaskLeft = true;

	if (ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced)
	{
		if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced)
			isAnyTaskLeft = true;
		if (ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && !ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced)
			isAnyTaskLeft = true;
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = isAnyTaskLeft;


	if( ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied && ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S3_Cave] = true;

	if( ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken && ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken && 
		ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced  && ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S3_Cave] = true;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S5_HillSide] = true;
}

void L4_S4_HillTop::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S4Collected)
		{
			InitHintTrailEmit(350, 280);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf])
		{
			if (ActivePopUpId != ePopInactive)
				InitHintTrailEmit(930, 190);
			else
				InitHintTrailEmit(350, 420);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 16;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
			if( ActivePopUpId != ePopInactive )
			{
				PopupArray[ActivePopUpId]->ProcessHint();
				return;
			}
			break;
		case 1: ++iNonHOHint;
			for (int i = 0; i < (int)(PopupArray.size()); i++)
			{
				if(isPopupActive(i)) //if popup is active
				{
					if(PopupArray[i]->isTaskLeft())
					{
						ClickablePolygon *rect = PopRectArray[i];
						InitHintTrailEmit(rect->center.x,rect->center.y,false);
						return;
					}
				}
			}
			break;
		case 2: ++iNonHOHint;
			if (ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularWithLensTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularCompleted)
			{
				Hud->ShowHintForInvItem(eInv_L4_CB_Binocular,&HillSideRect);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if (ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularCompleted && !ProfileSystem::Instance->ActiveProfile.L4_S5_Visited)
			{
				InitHintTrailEmit(&HillSideRect,true,CA_ExitUp);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide] )
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if((ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced && ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S5_HillSide]) || (ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularCompleted && !ProfileSystem::Instance->ActiveProfile.L4_S5_Visited))
			{
				InitHintTrailEmit(&SceneRectHillSide,true,CA_ExitUp);
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if(Hud->ShowHintForComboInvItems())
				return;
			break;

		case 7: ++iNonHOHint;
			Hud->teleportSystem->ShowTeleport();
			return;
			break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L4_S4_HillTop::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopUpId != ePopInactive)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L4_S4_HillTop::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < (int)PopupArray.size(); i++)
	{
		Popup * popup = PopupArray[i];
		if (popup)
		{
			if(popup->GetPopupState() == ePopupState_Inactive || popup->GetPopupState() == ePopupState_Closed) 
			{
				if(popup->isTaskLeft())
				{
					sparklePointsVec.push_back(popup->GetSparkleUpdatePoint());
					isSparkleUpdatePending = true;
				}
			}
		}

	}

}

void L4_S4_HillTop::NavigateToScene()
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