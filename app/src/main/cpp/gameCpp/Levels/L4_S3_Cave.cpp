//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL4 - SCENE3 : CAVE
//====================================

#include "L4_S3_Cave.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);

//PUZZLE RECTS
const CRectangle  PZFireSquareRect(658.0f,482.0f,114.0f,86.0f);
const CRectangle  PZS3DoorRect(887.0f,108.0f,97.0f,194.0f);

enum eL4S3HOPopEnums
{
	eL4S3Pop_Inactive = -1,
	eL4S3Pop_Pop1AntiquePot,
	eL4S3Pop_Max,
};

L4_S3_Cave::L4_S3_Cave()
{
	cv_EndCutscn	   = NULL;
	time = 0;
	subIndex = 0;
	subMax = 6;
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));


	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end6"));

	WaterFallFX		   = NULL;
	WaterFallBottomFX  = NULL;
	CaveGlowPlant1FX   = NULL;
	CaveGlowPlant2FX   = NULL;
	CaveGlowPlant3FX   = NULL;
	CaveGlowPlant4FX   = NULL;
	CaveGlowPlant5FX   = NULL;
	CaveGlowPlant6FX   = NULL;
	DoorOpenAnim	   = NULL;

	LoadHud(INVENTORY_V1);

	isVideoPlaying = false;
	IsDoorOpenAnimPlayed = false;

	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
		LoadScene("LV4\\S3\\S3DARK.SCN");
	else
		LoadScene("LV4\\S3\\S3LIGHT.SCN");

	InitSceneElements();
	initAnimations();

	if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved )
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);
		CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		cv_EndCutscn = new CVideo("OGV\\L4\\S3ENDCUTSCENE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//cv_EndCutscn->SetScale(1.334f);
		cv_EndCutscn->SetScale(GFApp->video_aspectRatio);
		cv_EndCutscn->SetSkipActive(1.0f);
		Control::Audio->QuickLoadAndPlaySFX("s3pz2_doorOpen");
		DoorOpenAnim->PlayAnim();

		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O2_FindWayOutOfForest] && 
			!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O2_FindWayOutOfForest] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O2_FindWayOutOfForest] = true;
			Hud->ShowObjectiveFX(eObj_L4O2_FindWayOutOfForest,true);
		}
	}

	InitPopups();


	//morph Object
#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S3Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("MorphTarget"), HOScene->GetObjectByID("MorphBase"));
		CRectangle  MorphRect(176.0f,61.0f,139.0f,113.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("MorphTarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("MorphTarget")->SetEnable(false);
#endif // _CEBUILD

	ActivePopupId = eL4S3Pop_Inactive;
	TaskPendingUpdate();

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s3CaveAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s3CaveAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s3CaveAmb,true);
	}

	//MUSIC_SFX
}

void L4_S3_Cave::InitPopups()
{
	int Pop1ArrayPts[] = {952,322,918,388,970,450,1078,438,1103,351,956,322};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL4S3Pop_Pop1AntiquePot, 6, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);
	pop1 = new L4S3_Pop1AntiquePot(eL4S3Pop_Pop1AntiquePot, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S3_Cave::OnPopupClosed, this));
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);
}

void L4_S3_Cave::OnPopupClosed(Popup * popup)
{

	WaterFallBottomFX->Start();
	CaveGlowPlant1FX->Start();
	CaveGlowPlant2FX->Start();
	CaveGlowPlant3FX->Start();
	CaveGlowPlant4FX->Start();
	CaveGlowPlant5FX->Start();
	CaveGlowPlant6FX->Start();

	TaskPendingUpdate();
	ActivePopupId = eL4S3Pop_Inactive;

	switch (popup->GetPopupId())
	{
	case eL4S3Pop_Pop1AntiquePot:
		break;
	default:
		break;
	}
}

L4_S3_Cave::~L4_S3_Cave()
{
	TaskPendingUpdate();

	for (int i = 0; i < eL4S3Pop_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopRectArray[i]);
	}
	SAFE_DELETE(cv_EndCutscn);
	SAFE_DELETE(subtitleText);
	SAFE_DELETE(WaterFallFX);
	SAFE_DELETE(WaterFallBottomFX);
	SAFE_DELETE(CaveGlowPlant1FX);
	SAFE_DELETE(CaveGlowPlant2FX);
	SAFE_DELETE(CaveGlowPlant3FX);
	SAFE_DELETE(CaveGlowPlant4FX);
	SAFE_DELETE(CaveGlowPlant5FX);
	SAFE_DELETE(CaveGlowPlant6FX);
	SAFE_DELETE(DoorOpenAnim);

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL4s3CaveAmb);
	Control::Audio->UnloadSample(aAmbL4s3CaveAmb);
	//MUSIC_SFX
}

void L4_S3_Cave::Required()
{
	HiddenObjectSystem::Required();	//forced update

//	if( Hud->PopupState != ePopInactive ) //Sac: Changing this to right enum value
	if( Hud->PopupState != Popup_Inactive )
		return;

	if(pop1->isVideoPlaying || isVideoPlaying || ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved)
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

void L4_S3_Cave::Update()
{
	NavigateToScene();
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif

	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if (ActivePopupId != eL4S3Pop_Inactive)
		return;


#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif // _CEBUILD

	if (isVideoPlaying )
	{
		//Subtitles Code
		time = cv_EndCutscn->GetTime();

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
		if(cv_EndCutscn->isEndReached() || vidTimer > 27)
		{
			subtitleText->SetText("");
			isVideoPlaying = false;
			cv_EndCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud(false);
			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L4P4_MaryL4Ending])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L4P4_MaryL4Ending] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L4P4_MaryL4Ending;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			}

			//Clearing all the Level 4 Objectives
			ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S1_TigerDistant;
			ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_5;
			//Backup Save for level restart
			ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);
			SendMessage(ControlExit, ID, HOSM_NewArea);
		} 
		return;
	}



	if( ActivePopupId == ePopInactive )
	{

		CPoint pos = Control::Input->Mpos();
		for (int i = 0; i < eL4S3Pop_Max; i++)
		{
			#ifdef _CEBUILD
				if( PopRectArray[i]->Intersection(&pos) && ( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S3Honey] ))
			#else
				if( PopRectArray[i]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken )
			#endif // _CEBUILD
			{
				Cursor::SetMode(CA_Look);
				if (Control::Input->LBclicked())
				{
					StopParticleEmission();
					Cursor::SetMode(CA_Normal);
					ActivePopupId = i;
					PopupArray[i]->OpenPopup();
				}
			} 
		}
		if( PZS3DoorRect.Intersection(&pos))
		{
			if (!ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved )
			{
				
#ifdef TOUCH_UI
		if(!GFHud->InvBoxCtrl->navTapped)
			{
				if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
					Cursor::SetMode(CA_Help);
			}
#else
		if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
					Cursor::SetMode(CA_Look);
				else 
					Cursor::SetMode(CA_Help);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				Cursor::SetMode(CA_Normal);
					if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
					{
						Hud->ShowBannerText("L4dark");
					}
					else
					{
						GFHud->InvBoxCtrl->GoToScene(eArea_L4_S3_PZDoor, 0, PZS3DoorRect);
					}
			}
#else			
			Cursor::SetMode(CA_Normal);
					if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
					{
						Hud->ShowBannerText("L4dark");
					}
					else
					{
						Control::Audio->PlayFootStepsFX();
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_PZDoor;
						SendMessage(ControlExit, ID, HOSM_NewArea);
					}
#endif
					
				} 
			}
			else if( IsDoorOpenAnimPlayed )
			{
				if( !GFInstance->IsReplayingGame )
				{
					Cursor::SetMode(CA_ExitUp);
					if( Control::Input->LBclicked() )
					{
						cv_EndCutscn->PlayVideo();
						Hud->EnableInput(false);
						Hud->HideHud(false);
						isVideoPlaying = true;
					}
				}
				else 
				{
					if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted) 
					{
						GFInstance->IsReplayingGame = false;
					}
					bool isPendingCollectableInLevel = false;
					if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S1Collected ||
						!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S3Collected ||
						!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S4Collected ||
						!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S5Collected )
					{
						isPendingCollectableInLevel = true;		
						if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle)//avoid looped banner calls!!
							Hud->ShowBannerText("L1mismo");
					}
					else 
					{
						for( int i = 0; i < 5; i++ )				
							if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[(ProfileSystem::Instance->ActiveProfile.CurrentLevel*5)+i] )
							{
								isPendingCollectableInLevel = true;
								if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle)//avoid looped banner calls!!
										Hud->ShowBannerText("L1mismc");
								break;
							}
					}

					//No pending collectables!!
					if( !isPendingCollectableInLevel )
					{
						Cursor::SetMode(CA_ExitUp);
						if( Control::Input->LBclicked() )
						{
							cv_EndCutscn->PlayVideo();
							Hud->EnableInput(false);
							Hud->HideHud(false);
							isVideoPlaying = true;
						}
					}
				}
			}
			return;		
		}
		else if( PZFireSquareRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S3_PZFireMaking, 0, PZFireSquareRect);
			}
#else			
			Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_PZFireMaking;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
									
			}			
			return;		
		}
		else if( IsMouseOverGoBackArea(&pos) )
		{
			
			#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L4_S2_RiverSide);
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
}


void L4_S3_Cave::InitSceneElements()
{
	MagicBluePlant01		 = HOScene->GetObjectByID("MagicBluePlant01Anim");
	MagicBluePlant02		 = HOScene->GetObjectByID("MagicBluePlant02Anim");
	MagicBluePlant03		 = HOScene->GetObjectByID("MagicBluePlant03Anim");
	PlantAnim				 = HOScene->GetObjectByID("PlantAnim");
	OpenDoor				 = HOScene->GetObjectByID("OpenDoor");
	DoorPuzzleDark			 = HOScene->GetObjectByID("DoorPuzzleDark");
	DoorPuzzleLight			 = HOScene->GetObjectByID("DoorPuzzleLight");
	AfterPuzzle				 = HOScene->GetObjectByID("AfterPuzzle");
	BeforePuzzle			 = HOScene->GetObjectByID("BeforePuzzle");
	WaterFall				 = HOScene->GetObjectByID("WaterFall");
	Plant01					 = HOScene->GetObjectByID("Plant01");
	Plant02					 = HOScene->GetObjectByID("Plant02");
	Plant03					 = HOScene->GetObjectByID("Plant03");
	Plant04					 = HOScene->GetObjectByID("Plant04");
	MagicGlitters02			 = HOScene->GetObjectByID("MagicGlitters02");
	MagicGlitters01			 = HOScene->GetObjectByID("MagicGlitters01");
	MagicPlant02			 = HOScene->GetObjectByID("MagicPlant02Anim");
	MagicPlant01			 = HOScene->GetObjectByID("MagicPlant01Anim");
	ScaryFace				 = HOScene->GetObjectByID("ScaryFace");


	if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
	{
		OpenDoor->SetEnable(false);
	}
}

void L4_S3_Cave::initAnimations()
{
	Plant01->SetPivot(0, 66);
	Plant01->Animate(0,0,0.5f,0,4.0f);
	Plant02->SetPivot(0, 39);
	Plant02->Animate(0,0,3,0,8.0f);
	Plant03->SetPivot(0, 63);
	Plant03->Animate(0,0,2.5f,0,6.0f);
	Plant04->SetPivot(0, 60);
	Plant04->Animate(0,0,2,0,5.0f);
	Plant04->SetPivot(0, 63);
	PlantAnim->Animate(0,0,1,0,9);

	ScaryFace->SetBlendMode(eBlend_AlphaAdditive);
	ScaryFace->Animate(0.45f, 0.85f, 6.0f);

	MagicBluePlant01->SetBlendMode(eBlend_AlphaAdditive);
	MagicBluePlant01->Animate(0, 0.8f, 9.0f);
	MagicBluePlant02->SetBlendMode(eBlend_AlphaAdditive);
	MagicBluePlant02->Animate(0, 0.9f, 8.0f);
	MagicBluePlant03->SetBlendMode(eBlend_AlphaAdditive);
	MagicBluePlant03->Animate(0, 0.7f, 6.0f);

	MagicPlant02->SetBlendMode(eBlend_AlphaAdditive);
	MagicPlant02->Animate(0, 0.8f, 9.0f);
	MagicPlant01->SetBlendMode(eBlend_AlphaAdditive);
	MagicPlant01->Animate(0, 0.7f, 5.0f);

	MagicGlitters01->SetEnable(false);
	MagicGlitters02->SetEnable(false);
	/*MagicGlitters01->SetBlendMode(eBlend_AlphaAdditive);
	MagicGlitters01->Animate(0.8f, 1.0f, 7.0f);
	MagicGlitters02->SetBlendMode(eBlend_AlphaAdditive);
	MagicGlitters02->Animate(0.8f, 1.0f, 6.0f);*/

	WaterFallFX = new SuperFX("PRT\\L4\\WaterFall.INI", PointSystem::CreateCPoint(684,16,0), 80); 
	WaterFallFX->Start();
	WaterFallFX->AdvanceByTime(2.0f);

	WaterFallBottomFX = new SuperFX("PRT\\L4\\WaterFallBottom.INI", PointSystem::CreateCPoint(670,292,0), 80); 
	WaterFallBottomFX->Start();
	WaterFallBottomFX->AdvanceByTime(2.0f);

	CaveGlowPlant1FX = new SuperFX("PRT\\L4\\CaveGlowPlant1.INI", PointSystem::CreateCPoint(345,629,0), 1100); 
	CaveGlowPlant1FX->Start();
	CaveGlowPlant1FX->AdvanceByTime(2.0f);

	CaveGlowPlant2FX = new SuperFX("PRT\\L4\\CaveGlowPlant2.INI", PointSystem::CreateCPoint(409,629,0), 1100); 
	CaveGlowPlant2FX->Start();
	CaveGlowPlant2FX->AdvanceByTime(2.0f);

	CaveGlowPlant3FX = new SuperFX("PRT\\L4\\CaveGlowPlant3.INI", PointSystem::CreateCPoint(1210,585,0), 1100); 
	CaveGlowPlant3FX->Start();
	CaveGlowPlant3FX->AdvanceByTime(2.0f);

	CaveGlowPlant4FX = new SuperFX("PRT\\L4\\CaveGlowPlant4.INI", PointSystem::CreateCPoint(226,325,0), 1100); 
	CaveGlowPlant4FX->Start();
	CaveGlowPlant4FX->AdvanceByTime(2.0f);

	CaveGlowPlant5FX = new SuperFX("PRT\\L4\\CaveGlowPlant5.INI", PointSystem::CreateCPoint(274,343,0), 1100); 
	CaveGlowPlant5FX->Start();
	CaveGlowPlant5FX->AdvanceByTime(2.0f);

	CaveGlowPlant6FX = new SuperFX("PRT\\L4\\CaveGlowPlant6.INI", PointSystem::CreateCPoint(179,239,0), 1100); 
	CaveGlowPlant6FX->Start();
	CaveGlowPlant6FX->AdvanceByTime(2.0f);

	if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
	{
		DoorOpenAnim = NULL;
		DoorOpenAnim = new JSONAnimator( 0, CC_CALLBACK_1(L4_S3_Cave::OnAnimationComplete, this ));
		DoorOpenAnim->parseAndLoadJSON("ANIM\\L4\\S3DOOROPENANIM.JSON", HOScene );
	}
	
}

void L4_S3_Cave::StopParticleEmission()
{
	WaterFallBottomFX			->StopImmediate();
	CaveGlowPlant1FX			->StopImmediate();
	CaveGlowPlant2FX			->StopImmediate();
	CaveGlowPlant3FX			->StopImmediate();
	CaveGlowPlant4FX			->StopImmediate();
	CaveGlowPlant5FX			->StopImmediate();
	CaveGlowPlant6FX			->StopImmediate();

}

void L4_S3_Cave::OnAnimationComplete(int index)
{	
	switch (index)
	{
	case 0:	IsDoorOpenAnimPlayed = true;
		break;
	}

}

void L4_S3_Cave::Render(const CPoint* pCam)
{

}


void L4_S3_Cave::TakeInventoryItem(EHOInventory ItemID)
{

}


void L4_S3_Cave::ReceiveMessage(int val)
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
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			//If no drop items in this scene !!
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}	
	TaskPendingUpdate();
}

float L4_S3_Cave::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L4_S3_Cave::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopupId != ePopInactive)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L4_S3_Cave::CheckForPendingTask()
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


	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit  )
	{
		sparklePointsVec.push_back(CPoint(PZS3DoorRect.GetCenterX(), PZS3DoorRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit  )
	{
		sparklePointsVec.push_back(CPoint(PZFireSquareRect.GetCenterX(), PZFireSquareRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved )
	{
		sparklePointsVec.push_back(CPoint(PZS3DoorRect.GetCenterX(), PZS3DoorRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	

}

void L4_S3_Cave::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S3Collected)
		{
			InitHintTrailEmit(220, 125);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S3Honey])
		{
			if (ActivePopupId != ePopInactive)
				InitHintTrailEmit(950, 320);
			else
				InitHintTrailEmit(1000, 370);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 10;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
			if( ActivePopupId != ePopInactive )
			{
				PopupArray[ActivePopupId]->ProcessHint();
				return;
			}
			break;
		case 1: ++iNonHOHint;
			for (int i = 0; i < eL4S3Pop_Max; i++)
			{
			//	if(Hud->PopupState != ePopupState_Inactive) //Sac: Changing this to right enum value
				if( Hud->PopupState != Popup_Inactive )
				{
					if(PopupArray[i]->isTaskLeft())
					{
						InitHintTrailEmit(&PopRectArray[i]->center,false);
						return;
					}
				}
			}
			break;
		case 2: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken )
			{
				InitHintTrailEmit(&PopRectArray[eL4S3Pop_Pop1AntiquePot]->center,false);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
			{
				InitHintTrailEmit(PZS3DoorRect.GetCenterX(),PZS3DoorRect.GetCenterY() ,false);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if(( ( (!ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced) && (ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken && 
				ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken &&  ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken) ) || 
				ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken && (!ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied) ) && 
				(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit) && (ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken &&
					ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken && ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken))
			{
				InitHintTrailEmit(PZFireSquareRect.GetCenterX(),PZFireSquareRect.GetCenterY() ,false);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved )
			{
				InitHintTrailEmit(PZS3DoorRect.GetCenterX(),PZS3DoorRect.GetCenterY() ,false);
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide])
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 7: ++iNonHOHint;
			if(Hud->ShowHintForComboInvItems())
				return;
			break;
		case 8: ++iNonHOHint;
			Hud->teleportSystem->ShowTeleport();
			return;
			break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L4_S3_Cave::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit )
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved )
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S3_Cave] = isAnyTaskLeft;

	if(ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide] = false;
	}


	if (!ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S5_HillSide] = true;

}
void L4_S3_Cave::NavigateToScene()
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