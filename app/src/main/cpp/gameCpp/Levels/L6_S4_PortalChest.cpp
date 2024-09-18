//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE4 : FLOATING ISLAND4
//====================================

#include "L6_S4_PortalChest.h"
#include "Util.h"
#include "GameFlow.h"
#include "Hud.h"

//const CRectangle  PuzzleRect(499.0f,591.0f,368.0f,138.0f);
const CRectangle  PuzzleRect(475.0f,518.0f,423.0f,91.0f);
const CRectangle  SceneRectS4(187.0f,691.0f,809.0f,90.0f);
const CRectangle  RockManRect(416.0f,2.0f,527.0f,500.0f);
const CRectangle  PopAreaRect(360.0f,126.0f,584.0f,415.0f);
const CRectangle  HeartRect(719.0f,247.0f,166.0f,164.0f);

L6_S4_PortalChest::L6_S4_PortalChest()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S4\\S1\\S1.SCN");

	InitParticles();

	dialogSystem = NULL;
	//if condition to add
	dialogSystem = new DialogSystem();
	AddControl(dialogSystem);	
	isDialogActive = false;

	int ArrayPts[] = {549,314,538,321,585,374,603,369,550,310};
	CPolygon *CPolyObjName=  new CPolygon(5);
	CPolyObjName->Init(ArrayPts);
	popUp = new L6S4_PopSecretLetter(1, Hud, NULL, eZOrder_Popup, CPolyObjName->center,CC_CALLBACK_1(L6_S4_PortalChest::OnPopupClosed,this));
	
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\MAGICPOWDER.SPR", 660, 330, eZOrder_Popup + 2);
	temp->SetUID("MAGICPOWDERTEXT");
	temp->Show(false);
	popUp->PushObject(temp);

	popUp->SetPopupBoundingBox(PopAreaRect);
	delete(CPolyObjName);

	Janim_Glue = new JSONAnimator(0);
	Janim_Glue->parseAndLoadJSON("ANIM\\L6\\S4_S1_GLUEAPPLYING.JSON",HOScene);

	isVideoPlaying = false;
	lpVidMaryWalking = NULL;
	vidTimer = 0;
	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	lpVidMaryWalking = new CVideo("OGV\\L6\\S4PORTALCUTSCENE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpVidMaryWalking->SetScale(1.334f);
	lpVidMaryWalking->SetScale(GFApp->video_aspectRatio);

	SceneObjectsUpdate();
	//MUSIC_SFX
	Control::Audio->LoadSample(aVOL6MaryRefS6_1,	AudioVO);
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s4PortalAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s4PortalAmb) )		
	{
		Control::Audio->PlaySample(aAmbL6s4PortalAmb,true);
	}
	activePopupId = ePopInactive;
	//MUSIC_SFX

	if( GFInstance->IsReplayingGame )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted) 
		{
			GFInstance->IsReplayingGame = false;
		}
		bool isPendingCollectableInLevel = false;
		if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected )
		{
			isPendingCollectableInLevel = true;					
			if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle  )//avoid looped banner calls!!
				Hud->ShowBannerText("L1mismo");
		}
		else 
		{
			for( int i = 0; i < 4; i++ )//picked mem collectable till 4th floating world!!
				if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[(ProfileSystem::Instance->ActiveProfile.CurrentLevel*5)+i] )
				{
					isPendingCollectableInLevel = true;
					if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle )//avoid looped banner calls!!
						 Hud->ShowBannerText("L1mismc");
					break;
				}
		}
		//No pending collectables during replaying!!
		if( !isPendingCollectableInLevel && ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete )
		{
			ProfileSystem::Instance->ActiveProfile.L6_S4_MaryWalkingCutscenePlayed = true;
			Hud->AllowInput = false;
			Hud->HideHud();
			lpVidMaryWalking->SetSkipActive();
			isVideoPlaying = true;
			lpVidMaryWalking->PlayVideo();
			return;
		}
	}
	else if (  ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete )
	{
		ProfileSystem::Instance->ActiveProfile.L6_S4_MaryWalkingCutscenePlayed = true;
		Hud->AllowInput = false;
		Hud->HideHud();
		lpVidMaryWalking->SetSkipActive();
		isVideoPlaying = true;
		lpVidMaryWalking->PlayVideo();
	}
}

void L6_S4_PortalChest::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced)
	{
		HOScene->GetObjectByID("HeartCrystal")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("HeartCrystal")->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_ChestOpened)
	{
		HOScene->GetObjectByID("Door01")->SetEnable(false);
		HOScene->GetObjectByID("Door02")->SetEnable(false);
	}

	/*if (ProfileSystem::Instance->ActiveProfile.L6_S4_NoteOpened)
	{
		HOScene->GetObjectByID("Scroll")->SetEnable(false);
	}
*/
	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken)
	{
		HOScene->GetObjectByID("Powder-Bottle")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied)
	{
		HOScene->GetObjectByID("GlueOnSlot")->SetEnable(false);
	}

	HOScene->GetObjectByID("GlueStick")->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
	{
		HOScene->GetObjectByID("Letter")->SetEnable(false);
	}
}

void L6_S4_PortalChest::InitParticles()
{

}

void L6_S4_PortalChest::OnPopupClosed(Popup * popup)
{
	activePopupId = ePopInactive;

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P9_MagicPowderFlier])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P9_MagicPowderFlier] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P9_MagicPowderFlier;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}
}


L6_S4_PortalChest::~L6_S4_PortalChest()
{
	SAFE_DELETE(lpVidMaryWalking);
	SAFE_DELETE(Janim_Glue);
	SAFE_DELETE(popUp);
	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL6MaryRefS6_1);
	Control::Audio->StopSample(aAmbL6s4PortalAmb);
	Control::Audio->UnloadSample(aAmbL6s4PortalAmb);
	//MUSIC_SFX
}

void L6_S4_PortalChest::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if (isVideoPlaying )
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

void L6_S4_PortalChest::Update()
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
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if(isDialogActive)
	{
		dialogSystem->Update();
		return;
	}

	if (activePopupId != ePopInactive)
	{
		return;
	}
#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif // _CEBUILD

	
	if (ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete && !ProfileSystem::Instance->ActiveProfile.L6_S4_MaryWalkingCutscenePlayed)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S4_MaryWalkingCutscenePlayed = true;
		Hud->AllowInput = false;
		Hud->HideHud();
		lpVidMaryWalking->SetSkipActive();
		isVideoPlaying = true;
		lpVidMaryWalking->PlayVideo();
	}
	else if (isVideoPlaying )
	{
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			lpVidMaryWalking->isEndReached() ||
#endif
			vidTimer > 20)
		{
			isVideoPlaying = false;
			lpVidMaryWalking->StopAndFadeOutVideo(2.0f);					
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->SendUpdateSceneMessage();

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S5_MarysDream;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		} 
		return;
	}

	CPoint pos = Control::Input->Mpos();
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_FloatingIsland4;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	

	if( RockManRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S4_IsRockManDialogOver )
	{
		Cursor::SetMode(CA_Speech);
		if (Control::Input->LBclicked())
		{
			dialogSystem->ShowDialog(eDialog_L6S4RockMan);

			isDialogActive = true;
			Hud->AllowInput = false;
			Hud->HideHud(true);	
			ProfileSystem::Instance->ActiveProfile.L6_S4_IsRockManDialogOver = true;
		}
	}
	else if (PuzzleRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete && ProfileSystem::Instance->ActiveProfile.L6_S4_IsRockManDialogOver)
	{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S4_PortalPuzzle, 0, PuzzleRect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_PortalPuzzle;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}
	}
	else if( IsMouseOverGoBackArea(&pos))
	{
		
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L6_S4_FloatingIsland4);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L6_S4_FloatingIsland4);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S4_FloatingIsland4, 2);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_FloatingIsland4;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	} 
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced && HOScene->GetObjectByID("Heart")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6stuck");
		}
	}
	/*else if (ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced && HOScene->GetObjectByID("Heart")->GetWorldRect().Intersection(&pos))
	{
	Cursor::SetMode(CA_HandTake);
	if (Control::Input->LBclicked())
	{
	HOScene->GetObjectByID("HeartCrystal")->SetEnable(false);
	HOScene->GetObjectByID("Heart")->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied = true;
	Hud->TakeInventoryItem(eInv_L6_CB_HeartContainerWithCrystal);
	}
	}*/
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced && !ProfileSystem::Instance->ActiveProfile.L6_S4_ChestOpened)
	{
		/*Cursor::SetMode(CA_HandPointy);
		if (Control::Input->LBclicked())*/
		{
			Control::Audio->QuickLoadAndPlaySFX("s4_doorSlideOpen");
			HOScene->GetObjectByID("Door01")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L6_S4_ChestOpened = true;
			HOScene->GetObjectByID("Door02")->TweenPosTo(100,0,0,1,0,true);


			

			//Show Objective completed
			if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O3_FindMissingRockManHeart] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L6O3_FindMissingRockManHeart] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L6O3_FindMissingRockManHeart] = true;
				Hud->ShowObjectiveFX(eObj_L6O3_FindMissingRockManHeart,true);
			}
		}
	}
	else if (HOScene->GetObjectByID("Door01")->GetWorldRect().Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken /*&& !ProfileSystem::Instance->ActiveProfile.L6_S4_NoteOpened*/ /*&& ProfileSystem::Instance->ActiveProfile.L6_S4_ChestOpened*/)
	{
		Cursor::SetMode(CA_HandPointy);
		if (Control::Input->LBclicked())
		{
			popUp->OpenPopup();
			activePopupId = 1;
			/*HOScene->GetObjectByID("Scroll")->SetEnable(false);*/
			ProfileSystem::Instance->ActiveProfile.L6_S4_NoteOpened = true;
		}
	}
	else if (HOScene->GetObjectByID("Door01")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken /*&& ProfileSystem::Instance->ActiveProfile.L6_S4_NoteOpened*/ && ProfileSystem::Instance->ActiveProfile.L6_S4_ChestOpened)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			HOScene->GetObjectByID("Powder-Bottle")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L6_S4_ColouredPowder);
			ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken = true;

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P8_RockmanPowderGot])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P8_RockmanPowderGot] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P8_RockmanPowderGot;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}
		}
	}


}

void L6_S4_PortalChest::Render(const CPoint* pCam)
{

}

void L6_S4_PortalChest::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	if( val == HOSM_DialogClosed ) 
	{
		isDialogActive = false;
		//Hud->AllowInput = true;
		Hud->EnableInput(true);
		Hud->ShowHud(true);
		Hud->ShowBannerText("L6bull");

		Control::Audio->PlaySample(aVOL6MaryRefS6_1);		//Objective Add
		if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O3_FindMissingRockManHeart] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O3_FindMissingRockManHeart] = true;
			ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
			= eObj_L6O3_FindMissingRockManHeart;
			//Increment Objectives
			ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
			Hud->ShowObjectiveFX(eObj_L6O3_FindMissingRockManHeart);

			Control::Audio->PlaySample(aSFXMagic1);
		}	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			//If no drop items in this scene !!

			if (itemVal == eInv_L6_S3_GlowingHeart)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced && HOScene->GetObjectByID("Heart")->GetWorldRect().Intersection(&dropPos) && ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied)
				{
					Control::Audio->QuickLoadAndPlaySFX("s4_fixHeart");
					HOScene->GetObjectByID("HeartCrystal")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced = true;

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_GlowingHeart] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S3_GlowingHeart);
				}
				else if (HOScene->GetObjectByID("Heart")->GetWorldRect().Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied)
				{
					Hud->ShowBannerText("L6stick");
					Hud->CancelDrag();
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if (itemVal == eInv_L6_S2_PinegumStick)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied && ProfileSystem::Instance->ActiveProfile.L6_S4_IsRockManDialogOver)
				{
					Control::Audio->QuickLoadAndPlaySFX("s4_applyGlue");
					Janim_Glue->PlayAnim();
					ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied = true;

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S2_PinegumStick] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S2_PinegumStick);
				}
				else
					GFInstance->WrongInvDropMsg(Hud);

			}
			else if (itemVal == eInv_L6_S4_ColouredPowder)
			{
				Hud->ShowBannerText("L6guess");
				Hud->CancelDrag();
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}	
	TaskPendingUpdate();
}

float L6_S4_PortalChest::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L6_S4_PortalChest::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected)
		isAnyTaskLeft = true;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied && ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected)
		isAnyTaskLeft = true;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken && ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied)
		isAnyTaskLeft = true;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_NoteOpened && ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken)
		isAnyTaskLeft = true;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken && ProfileSystem::Instance->ActiveProfile.L6_S4_ChestOpened)
		isAnyTaskLeft = true;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_PortalChest] = isAnyTaskLeft;

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken && !ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2]  = true;
}

void L6_S4_PortalChest::ProcessNonHOHint()
{

	

	int iNonHOHint = 0; 
	int MaxNonHOHints = 10;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;
		case 0: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L6_S4_IsRockManDialogOver )
			{
				InitHintTrailEmit(&RockManRect,false);
				return;
			}
			break; 
		case 1: ++iNonHOHint;
			if (!ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied && ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected)
			{
				Hud->ShowHintForInvItem(eInv_L6_S2_PinegumStick,&HeartRect);
				return;
			}
			break;
		case 2: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced && ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied)
			{
				//Hud->ShowHintForInvItem(eInv_L6_S3_GlowingHeart,&HOScene->GetObjectByID("Heart")->GetWorldRect());
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle heartObjRect =HOScene->GetObjectByID("Heart")->GetWorldRect();
				Hud->ShowHintForInvItem(eInv_L6_S3_GlowingHeart,&heartObjRect);
				return;
			}
			break;

		case 3:++iNonHOHint;
			if (!ProfileSystem::Instance->ActiveProfile.L6_S4_NoteOpened && ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken)
			{
				//InitHintTrailEmit(&HOScene->GetObjectByID("Door01")->GetWorldRect(),false);
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle doorObjRect =HOScene->GetObjectByID("Door01")->GetWorldRect();
				InitHintTrailEmit(&doorObjRect,false);
				return;
			}
			break;

		case 4:++iNonHOHint;
			if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken && ProfileSystem::Instance->ActiveProfile.L6_S4_ChestOpened)
			{
				//InitHintTrailEmit(&HOScene->GetObjectByID("Door01")->GetWorldRect(),false);
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle doorObjRect =HOScene->GetObjectByID("Door01")->GetWorldRect();
				InitHintTrailEmit(&doorObjRect,false);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
			{
				InitHintTrailEmit(&PuzzleRect,false);
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected )
			{
				InitHintTrailEmit(&PuzzleRect,false);
				return;
			}
			break;
		case 7: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4])
			{
				InitHintTrailEmit(&SceneRectS4,true,CA_ExitDown);
				return;
			}
			break;
		case 8: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 9: ++iNonHOHint;
				{
					Hud->teleportSystem->ShowTeleport();
					return;
				}
				break;

		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!

	Hud->ShowNothingToDoMsgBanner();
}

void L6_S4_PortalChest::NavigateToScene()
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