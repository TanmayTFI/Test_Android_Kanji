
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L6_S3_GlowingHeartHO.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"

const CRectangle  Match3btnRect(539.0f,2.0f,295.0f,43.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);
const CRectangle  VaseRect(204.0f,122.0f,153.0f,265.0f);



L6_S3_GlowingHeartHO::L6_S3_GlowingHeartHO ()
{
	HeartRect[0] = CRectangle(547.0f,181.0f,143.0f,128.0f);
	HeartRect[1] = CRectangle(473.0f,213.0f,157.0f,129.0f);
	HeartRect[2] = CRectangle(403.0f,260.0f,144.0f,111.0f);

	ObjectInHandID = -1;
	ActivePopupID = -1;
	IsLockOpening = -1;
	IsAnimationPlaying = false;
	moveDirection = 0;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S3\\HO\\HO.SCN");

	for (int i = 0; i < 20; i++)
	{
		CartObjectsPos[i] = CPoint(0,0,0); 
	}

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s3FloatingIsland3Amb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s3FloatingIsland3Amb) )		
	{
		Control::Audio->PlaySample(aAmbL6s3FloatingIsland3Amb,true);
	}
	
	//MUSIC_SFX
	
	//Match 3
	match3Ctrl  = NULL;
	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	match3Back = new CObject();
	match3Back->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
	match3Back->SetUID("match3Back");
	match3Back->Show(false);
	HOScene->PushObject(match3Back);

	match3BackText = new CObject();
	match3BackText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
	match3BackText->SetUID("match3BackText");
	match3BackText->Show(false);
	HOScene->PushObject(match3BackText);
#ifndef M3_BUTTON
	match3Back->SetEnable(false);
    match3BackText->SetEnable(false);
#endif
	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		match3Back->SetEnable(false);
		match3BackText->SetEnable(false);
	}
	BtnMatch3 = new CObject();
	BtnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 685, 20, 41);
	BtnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(BtnMatch3);

	BtnSwitchToHO = new CObject();
	BtnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 685, 20, 42);
	BtnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(BtnSwitchToHO);

	//BtnMatch3 = HOScene->GetObjectByID("btnMatch3");
	//BtnSwitchToHO = HOScene->GetObjectByID("btnSwitchToHO");

	BtnMatch3->SetZ(100);
	BtnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);
	BtnSwitchToHO->Show(false);

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L6_S3_GlowingHeartHO ]  = true;

	InitControlPolys();
	InitPopups();
	InitSceneElements();
	InitAnimations();
	TaskPendingUpdate();

	if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Visited )
	{
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Visited  = true;
		Hud->ShowBannerText("L6chest");
	}

}

void L6_S3_GlowingHeartHO::InitPopups()
{
	ActivePopupID = ePopInactive;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved)
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition = 0;

	int popID=0;
	//Popup 1
	popID = ePop1_L6S3Pop1Code;
	//if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved)
	{
		int PuzzleArray[] = {670,176,701,235,822,183,789,123};
		PuzzlePoly=  new CPolygon(4);
		PuzzlePoly->Init(PuzzleArray);

		Pop1Code = new L6S3_HOPop1Code(popID, Hud, HOScene, eZOrder_Popup, PuzzlePoly->center, CC_CALLBACK_1(L6_S3_GlowingHeartHO::OnPopupClosed, this));
		Pop1Code->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = PuzzlePoly;
		PopupArray[popID]	= Pop1Code;

		PopStates[popID] = ePopupState_Closed;
	}

	popID = ePop2_L6S3Pop2Vase;
	//if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] != e_L6S3_Used )
	{
		Pop2Vase = new L6S3_HOPop2Vase(popID, Hud, HOScene, eZOrder_Popup, DestPolyArray[e_Spanner]->center, CC_CALLBACK_1(L6_S3_GlowingHeartHO::OnPopupClosed, this));
		Pop2Vase->SetPopupBoundingBox(PopCircleRect);

		PopPolyArray[popID] = DestPolyArray[e_Spanner];
		PopupArray[popID]	= Pop2Vase;

		PopStates[popID] = ePopupState_Closed;
		if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed)
			Pop2Vase->IsSpannerActive = false;
	}
	
}

void L6_S3_GlowingHeartHO::OnPopupClosed(Popup* popup)
{
	Pop2Vase->IsSpannerActive = false;
	ActivePopupID = ePopInactive;
	int popID = popup->GetPopupId();
	SceneObjectsUpdate();
	S3_HO_GoldGlitter2FX->Start();
	S3_HO_GoldGlitterFX->Start();
	S3_HOLightEffectFX->Start();
	S3_HO_GoldGlitter3FX->Start();

	BtnMatch3->Show(true);
	BtnSwitchToHO->Show(false);
	BtnSwitchToHO->PlayAnimation(0);

	if(popID == ePop1_L6S3Pop1Code)
	{
		if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved)
		{
			JanimPuzzleDoorRetreat->PlayAnim();
			IsAnimationPlaying = true;
			Control::Audio->QuickLoadAndPlaySFX("s3ho_codeBlockSlide");
		}
	}
	else if(popID == ePop2_L6S3Pop2Vase)
	{
		if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved)
		{
			Vase_Door->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] = e_L6S3_Inactive;
		}
		if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed)
			Bolt1->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed)
			Bolt2->SetEnable(false);
		if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed )
		{
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] = e_L6S3_Used;
			HOScnObjects[e_Spanner]->SetEnable(false);
		}
		else 
		{
			
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] = e_L6S3_Inactive;
			HOScnObjects[e_Spanner]->SetEnable(true);
		}
		if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] == e_L6S3_Used )
			Spanner->SetEnable(false);
		else
			Spanner->SetEnable(true);
	}
}

void L6_S3_GlowingHeartHO ::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL6theme) )
	{
		Control::Audio->Samples[aTrackL6theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL6theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL6theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL6ho) )
				  Control::Audio->PlaySample(aTrackL6ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL6ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL6ho, true);
		}
	}
}

void L6_S3_GlowingHeartHO::CheckIfHOComplete()
{
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened1 && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened2 &&
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened3 && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle] == e_L6S3_Used)
	{
		Control::Audio->QuickLoadAndPlaySFX("s3ho_boxDoorOpen");
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainBoxOpened = true;
		TopHandle->FadeOut(1.5f);
		BoxPuzzleDoor->FadeOut(1.5f);
		GemGlow01->FadeOut(1.5f);
		GemGlow02->FadeOut(1.5f);
		GemGlow03->FadeOut(1.5f);
		Gems->FadeOut(1.5f);
		BoxClosed->FadeOut(1.5f);
		LaserLight->FadeOut(1.5f);
		MainLock01Open->FadeOut(1.5f);
		MainLock02Open->FadeOut(1.5f);
		MainLock03Open->FadeOut(1.5f);
	}
}


void L6_S3_GlowingHeartHO ::Update()
{

	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	NavigateToScene();

	if(ActivePopupID  != -1 )
		return;

	if( Hud->PopupState != Popup_Inactive )
		return;
	
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;


	if (!BtnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken && ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}
	if (ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		BtnMatch3->Show(false);
		BtnSwitchToHO->Show(false);
	}

	HiddenObjectSystem::HOUpdate();

	bool isMouseOverFlag = false;
	CPoint pos = Control::Input->Mpos();
	//Cursor::SetMode(CA_Normal);


	if(IsAnimationPlaying)
		return;

	if(!ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{

		if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainBoxOpened )
		{
			for (int i = 0; i < 3; i++)
			{
				if( HeartRect[i].Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition == i && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken) 
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked() )
					{
						ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken = true;
						Hud->TakeInventoryItem( eInv_L6_S3_GlowingHeart);
						HOScene->GetObjectByID("Heart")->SetEnable(false);
						Hud->ShowBannerText("L6warm");
					}
					return;
				}
			}
		}


		if( IsLockOpening != -1)
		{
			Cursor::SetMode(CA_Gear);
			switch (IsLockOpening)
			{
			case 1:	if( GemGlow01->GetAlpha() >= 0.9 )
					{
						Control::Audio->QuickLoadAndPlaySFX("s3ho_lockOpen");
						MainLock01->FadeOut(1.5f);
						MainLock01Open->SetEnable(true);
						MainLock01Open->SetAlpha(0.0f);
						MainLock01Open->FadeIn(1.5f);
						IsLockOpening = 4;
					}
				break;
			case 2:	if( GemGlow02->GetAlpha() >= 0.9 )
					{
						Control::Audio->QuickLoadAndPlaySFX("s3ho_lockOpen");
						MainLock02->FadeOut(1.5f);
						MainLock02Open->SetEnable(true);
						MainLock02Open->SetAlpha(0.0f);
						MainLock02Open->FadeIn(1.5f);
						IsLockOpening = 5;
					}
				break;
			case 3:	if( GemGlow03->GetAlpha() >= 0.9 )
					{
						Control::Audio->QuickLoadAndPlaySFX("s3ho_lockOpen");
						MainLock03->FadeOut(1.5f);
						MainLock03Open->SetEnable(true);
						MainLock03Open->SetAlpha(0.0f);
						MainLock03Open->FadeIn(1.5f);
						IsLockOpening = 6;
					}
				break;
			case 4:	if( MainLock01Open->GetAlpha() >= 0.9 )
					{
						CheckIfHOComplete();
						IsLockOpening = -1;
					}
				break;
			case 5:	if( MainLock02Open->GetAlpha() >= 0.9 )
					{
						CheckIfHOComplete();
						IsLockOpening = -1;
					}
				break;
			case 6:	if( MainLock03Open->GetAlpha() >= 0.9 )
					{
						CheckIfHOComplete();
						IsLockOpening = -1;
					}
				break;
			}
			return;
		}


		if( ActivePopupID == ePopInactive )
		{
			Cursor::SetMode(CA_Normal);
		
			/*if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
				SparkleUpdate();*/

			//popup click checks
			for (int i = 0; i < ePopL6S3_Max; i++)
			{
				if((PopStates[i] != ePopupState_Inactive ) && (ObjectInHandID == -1 )) //if popup is active
				{
					if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos))
					{
						if( CheckIfPopupActive(i) )
						{
							Cursor::SetMode(CA_Look);
							if( Control::Input->LBclicked() )
							{

								BtnMatch3->Show(true);
								BtnSwitchToHO->Show(false);
								BtnSwitchToHO->PlayAnimation(0);

								match3Back->Show(false);
								match3BackText->Show(false);

								Cursor::SetMode(CA_Normal);
								ActivePopupID = i;
								PopupArray[i]->OpenPopup();
								StopParticleEmission();								
								
								BtnMatch3->Show(true);
								BtnSwitchToHO->Show(false);
								BtnSwitchToHO->PlayAnimation(0);
							}
							return;
						}
					}
				}
			}
		}


		if (ObjectInHandID != -1 )
		{
			HOInHandObjects[ObjectInHandID]->SetPos( &pos );
			if( ObjectInHandID == e_Spanner && ActivePopupID == ePopInactive )
			{
				if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseToppled && DestPolyArray[ObjectInHandID]->Intersection(&pos) )
					Cursor::SetMode(CA_Look);
				else 
					Cursor::SetMode(CA_Normal);
			}

			if( Control::Input->LBclicked() )
			{
				HOInHandObjects[ObjectInHandID]->SetEnable(false);
				if(DestPolyArray[ObjectInHandID]->Intersection(&pos) )
				{
					if( ObjectInHandID != e_Spanner)
						ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[ObjectInHandID] = e_L6S3_Used;
					switch (ObjectInHandID)
					{
					case e_Rail:			HODestObjects[e_Rail]->SetEnable(true);
											Control::Audio->QuickLoadAndPlaySFX("s3ho_placeRail");
						//HODestObjects[e_Rail]->SetZ( Box->ZOrder - 1);
						break;
					case e_Wheel:			HODestObjects[e_Wheel]->SetEnable(true);
											Control::Audio->QuickLoadAndPlaySFX("s3ho_placeWheel");
						break;
					case e_Spanner:			if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseToppled )
											{
												if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed || !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed )
													Pop2Vase->IsSpannerActive = true;
												StopParticleEmission();
												Cursor::SetMode(CA_Normal);
												PopupArray[ePop2_L6S3Pop2Vase]->OpenPopup();
												ActivePopupID = ePop2_L6S3Pop2Vase;
											}
											else
											{
												ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[ObjectInHandID] = e_L6S3_Inactive;
												HOScnObjects[ObjectInHandID]->SetEnable(true);
											}
						break;
					case e_TopHandle:		HODestObjects[e_TopHandle]->SetEnable(true);
											SideLock->FadeOut(1.5f);
											SideLockOpen->SetEnable(true);
											SideLockOpen->SetAlpha(0.0f);
											SideLockOpen->FadeIn(1.5f);
											Control::Audio->QuickLoadAndPlaySFX("s3ho_boxheadfix");
						break;
					case e_LaserDevice:		HODestObjects[e_LaserDevice]->SetEnable(true);
											LaserLight->SetEnable(true);
											LaserLight->SetAlpha(0.0f);
											LaserLight->FadeIn(1.5f);
											Effects->SetEnable(false);
											UpdateGems();
											Control::Audio->QuickLoadAndPlaySFX("s3ho_placeLaserDevice");
						break;
					default:
						ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[ObjectInHandID] = e_L6S3_Inactive;
						HOScnObjects[ObjectInHandID]->SetEnable(true);
						break;
					}
				}
				else
				{
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[ObjectInHandID] = e_L6S3_Inactive;
					HOScnObjects[ObjectInHandID]->SetEnable(true);
				}
				ObjectInHandID = -1;
			}
			return;
		}

		if( ObjectInHandID == -1 )
		{
			for (int i = 0; i < e_L6S3HOMax; i++)
			{
				if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[i] == e_L6S3_Inactive && HOPolyArray[i]->Intersection(&pos) )
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked())
					{
						ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[i] = e_L6S3_Active;
						ObjectInHandID = i;
						HOInHandObjects[ObjectInHandID]->SetEnable(true);
						HOInHandObjects[ObjectInHandID]->SetZ( HOInHandObjects[ObjectInHandID]->ZOrder + 90 );
						HOInHandObjects[ObjectInHandID]->SetPos(&pos);
						HOScnObjects[ObjectInHandID]->SetEnable(false);
					}
					return;
				} 
			}
		}
		
		if( CodePoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeTaken )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked())
			{
				CodeInScene->SetEnable(false);
				Hud->TakeInventoryItem(eInv_L6_S3_PuzzleCode);
				Hud->ShowBannerText("L6dial");
				Control::Audio->QuickLoadAndPlaySFX("s3ho_takeCode");
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeTaken = true;
			}
			return;
		}
		if(VaseRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseToppled)
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s3ho_pushjar");
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseToppled = true;
				VaseBottom->SetEnable(true);
				Vase_Door->SetEnable(true);
				Vase->SetEnable(false);
				Bolt1->SetEnable(true);
				Bolt2->SetEnable(true);
				LesarDeviceInScene->SetEnable(true);
			}
			return;
		}

		if( LeverPoly[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->Intersection(&pos)  )
		{
			isMouseOverFlag = true;
			if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle] == e_L6S3_Used &&
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Wheel] == e_L6S3_Used &&
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Rail] == e_L6S3_Used &&
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved )
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked())
				{
					MoveCart(moveDirection);
				}
				
			}
			else 
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					Lever[0]->Shake(0,2,0,0.5f);
				}
			}
			UpdateBannerMessages(pos);
			return;
		}


		if( HoBoxPoly->Intersection(&pos) && !IsLockOpening )
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				for (int i = 0; i < 20; i++)
				{
					CartObjects[i]->Shake(0,2,0,0.5f);
				}
			}
		}
		
		
	}

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
		{
			if( Control::Input->LBclicked() )
			{
				Hud->InvBoxCtrl->ResetToInvBox();

				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FloatingIsland3;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
		if( match3Ctrl->isPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken )
		{
			match3Ctrl->clearBoard();
			BtnMatch3->Show(false); 
			BtnSwitchToHO->Show(false);
			
			

			Hud->HOState = HOInactive;
			Hud->InvBoxCtrl->ResetToInvBox();
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken = true;
			if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_PuzzleCode])
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_PuzzleCode] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S3_PuzzleCode);
			}
			Hud->TakeInventoryItem( eInv_L6_S3_GlowingHeart);
			Hud->ShowBannerText("L6warm");
			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			TaskPendingUpdate();
			return;
		}


		if(Match3btnRect.Intersection(&pos) && !match3Ctrl->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSwitchToHO->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);


				match3Ctrl->clearBoard();
				BtnMatch3->Show(true);
				BtnSwitchToHO->Show(false);
				BtnSwitchToHO->PlayAnimation(0);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Hud->InvBoxCtrl->ResetToInvBox();
				return;
			}
		}
		else
		{
			BtnSwitchToHO->PlayAnimation(0);
		}
		return;
	}
	else
	{
		if(Match3btnRect.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnMatch3->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				if(match3Ctrl == NULL)
				{
					match3Ctrl = new Match3(14);
					AddControl(match3Ctrl);
				}
				else
				{
					match3Ctrl->ShowBoard();
				}

				BtnMatch3->Show(false);
				BtnMatch3->PlayAnimation(0);
				BtnSwitchToHO->Show(true);
				
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;

				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				return;
			}
		}
		else
		{
			BtnMatch3->PlayAnimation(0);
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
//	if (Control::Input->isKeyBackSensed() && !isMouseOverFlag && IsLockOpening == -1 ) // Need to check if isMousOverFlag has to checked here for Back or not
	if (Control::Input->isKeyBackSensed() && IsLockOpening == -1 )

	{
		Hud->InvBoxCtrl->ResetToInvBox();

		Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FloatingIsland3;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif


	if( IsMouseOverGoBackArea(&pos) && !isMouseOverFlag && IsLockOpening == -1
														   #ifdef M3_BUTTON
														   && BtnMatch3->Visible
                                                           #endif
                                                           )
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S3_FloatingIsland3, 2);
			}
#else			
			Hud->InvBoxCtrl->ResetToInvBox();

			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FloatingIsland3;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}

	UpdateBannerMessages(pos);
}


void L6_S3_GlowingHeartHO::UpdateBannerMessages( CPoint pos)
{
	if( HoBoxPoly->Intersection(&pos) && ((ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Wheel]) != e_L6S3_Used)) //Sac: why converstion of enum to bool?
	{
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L6wheel");
	}
	else if( (HoBoxPoly->Intersection(&pos) || RailPoly1->Intersection(&pos) || RailPoly2->Intersection(&pos) )&& ((ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Rail]) != e_L6S3_Used) )
	{
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L6rail");
	}
	else if( PopPolyArray[ePop1_L6S3Pop1Code]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved)
	{
		if( Control::Input->LBclicked()  )
			Hud->ShowBannerText("L6hint");
	}
	else if( HoBoxPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved)
	{
		if( Control::Input->LBclicked()  )
			Hud->ShowBannerText("L6locks");
	}
	else if( DestPolyArray[e_LaserDevice]->Intersection(&pos) && ((ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice]) != e_L6S3_Used) )
	{
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L6magic");
	}
	else if(PopPolyArray[ePop1_L6S3Pop1Code]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved && 
		!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened1 && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened2 && 
		!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened3 )
	{
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L6green");
	}
	else if( DestPolyArray[e_TopHandle]->Intersection(&pos) && ((ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle]) != e_L6S3_Used))
	{
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L6top");
	}
	else if( LeverPoly[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->Intersection(&pos) && ( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened1 || !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened2 ||
		!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened3 ))
	{
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L6work");
	}
	else if( LeverPoly[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened1 && (!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened2 ||
		!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened3 ))
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L6lever");
}

void L6_S3_GlowingHeartHO::MoveCart(int direction)
{
	

	Lever[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->FadeOut(1.5f);

	Control::Audio->QuickLoadAndPlaySFX("s3ho_cartSlide");
	switch (direction)
	{
	case 0:	ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition++;
		break;
	case 1:	ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition--;
		break;
	default:
		break;
	}

	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition > 2 )
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition = 2;
	else if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition < 0 )
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition = 0;

	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition == 0 )
		moveDirection = 0;
	else if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition == 2 )
		moveDirection = 1;

	Lever[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->SetEnable(true);
	Lever[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->SetAlpha(0.0f);
	Lever[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->FadeIn(1.5f);

	CPoint RelativeOffset = CPoint(-72.0f * ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition, 
		35.0f * ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition,0.0f);
	for (int i = 0; i < 20; i++)
	{
		CPoint NewPosition = CPoint( CartObjectsPos[i].x + RelativeOffset.x, CartObjectsPos[i].y + RelativeOffset.y , 0 );
		CartObjects[i]->SetPos( &NewPosition ); 
	}

	UpdateGems();
}


void L6_S3_GlowingHeartHO::UpdateGems()
{
	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved )
	{
		if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened1 && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition == 0 &&
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] == e_L6S3_Used )
		{
			IsLockOpening = 1;
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened1 = true;
			GemGlow01->SetEnable(true);
			Control::Audio->QuickLoadAndPlaySFX("s3ho_gemGlow");
			GemGlow01->SetAlpha(0.0f);
			GemGlow01->FadeIn(1.5f);
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened2 && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition == 1 &&
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] == e_L6S3_Used )
		{
			IsLockOpening = 2;
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened2 = true;
			Control::Audio->QuickLoadAndPlaySFX("s3ho_gemGlow");
			GemGlow02->SetEnable(true);
			GemGlow02->SetAlpha(0.0f);
			GemGlow02->FadeIn(1.5f);
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened3 && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition == 2 &&
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] == e_L6S3_Used )
		{
			IsLockOpening = 3;
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened3 = true;
			Control::Audio->QuickLoadAndPlaySFX("s3ho_gemGlow");
			GemGlow03->SetEnable(true);
			GemGlow03->SetAlpha(0.0f);
			GemGlow03->FadeIn(1.5f);
		}
	}
}

bool L6_S3_GlowingHeartHO::CheckIfPopupActive(int popupId)
{
	bool flag = true;
	switch (popupId)
	{
	case ePop1_L6S3Pop1Code:	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved )
									flag = false;
		break;
	case ePop2_L6S3Pop2Vase:	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved || !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseToppled )
									flag = false;
		break;
	default:
		break;
	}
	return(flag);
}


void L6_S3_GlowingHeartHO::StopParticleEmission()
{
	S3_HO_GoldGlitter2FX->StopImmediate();
	S3_HO_GoldGlitterFX->StopImmediate();
	S3_HOLightEffectFX->StopImmediate();
	S3_HO_GoldGlitter3FX->StopImmediate();
}

void L6_S3_GlowingHeartHO ::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
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
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if( ActivePopupID == ePop1_L6S3Pop1Code )
			{
				PopupArray[ePop1_L6S3Pop1Code]->HandleInventoryDrop( dropPos, itemVal );
			}
			else
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		CPoint dropPos;
		if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken )
		{
			Hud->InvBoxCtrl->ResetToInvBox();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FloatingIsland3;
			SendMessage(ControlExit, ID, HOSM_NewArea);	 
		}
	}
}

void L6_S3_GlowingHeartHO::SceneObjectsUpdate()
{
	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed)
		Spanner->SetEnable(false);
	

	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed )
		Bolt1->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed )
		Bolt2->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved)
		Vase_Door->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved )
	{
		if( ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_PuzzleCode] )
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S3_PuzzleCode);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_PuzzleCode] = false;
		}
	}
}

void L6_S3_GlowingHeartHO ::InitSceneElements()
{
	Box								= HOScene->GetObjectByID("Box");
	BoxClosed						= HOScene->GetObjectByID("BoxClosed");
	BoxPuzzleDoor					= HOScene->GetObjectByID("BoxPuzzleDoor");
	Code							= HOScene->GetObjectByID("Code");
	CodeInScene						= HOScene->GetObjectByID("CodeInScene");
	Effects							= HOScene->GetObjectByID("Effects");
	GemGlow01						= HOScene->GetObjectByID("GemGlow01");
	GemGlow02						= HOScene->GetObjectByID("GemGlow02");
	GemGlow03						= HOScene->GetObjectByID("GemGlow03");
	Gems							= HOScene->GetObjectByID("Gems");
	LaserDevice						= HOScene->GetObjectByID("LaserDevice");
	LaserDeviceInHand				= HOScene->GetObjectByID("LaserDeviceInHand");
	LaserLight						= HOScene->GetObjectByID("LaserLight");
	LesarDeviceInScene				= HOScene->GetObjectByID("LaserDeviceInScene");
	LightRays						= HOScene->GetObjectByID("LightRays");
	MainLock01						= HOScene->GetObjectByID("MainLock01");
	MainLock01Open					= HOScene->GetObjectByID("MainLock01Open");
	MainLock02						= HOScene->GetObjectByID("MainLock02");
	MainLock02Open					= HOScene->GetObjectByID("MainLock02Open");
	MainLock03						= HOScene->GetObjectByID("MainLock03");
	MainLock03Open					= HOScene->GetObjectByID("MainLock03Open");
	Rail							= HOScene->GetObjectByID("Rail");
	RailInScene						= HOScene->GetObjectByID("RailInScene");
	RailOnClick						= HOScene->GetObjectByID("RailOnClick");
	SideLock						= HOScene->GetObjectByID("SideLock");
	SideLockOpen					= HOScene->GetObjectByID("SideLockOpen");
	Spanner							= HOScene->GetObjectByID("Spaner");
	SpannerOnClick					= HOScene->GetObjectByID("SpanerOnClick");
	TopHandle						= HOScene->GetObjectByID("TopHandle");
	TopHandleInScene				= HOScene->GetObjectByID("TopHandleInScene");
	TopHandleOnClick				= HOScene->GetObjectByID("Top_HandleOnClick");
	TyreInPlace						= HOScene->GetObjectByID("TyreInPlace");
	TyreInScene						= HOScene->GetObjectByID("TyreInScene");
	TyreOnclick						= HOScene->GetObjectByID("TyreOnclick");
	Vase							= HOScene->GetObjectByID("Vase");
	VaseBottom						= HOScene->GetObjectByID("VaseBottom");
	Vase_Door						= HOScene->GetObjectByID("Vase_Door");
	Bolt1							= HOScene->GetObjectByID("Bolt01");
	Bolt2							= HOScene->GetObjectByID("Bolt02");
	BoxMask							= HOScene->GetObjectByID("BoxMask");
	Heart							= HOScene->GetObjectByID("Heart");
	Lever[0]						= HOScene->GetObjectByID("Lever00");
	Lever[1]						= HOScene->GetObjectByID("Lever01");
	Lever[2]						= HOScene->GetObjectByID("Lever02");

	//Assigning the cart array elements
	CartObjects[0]					= Code;
	CartObjects[1]					= Box;
	CartObjects[2]					= BoxClosed;
	CartObjects[3]					= GemGlow01;
	CartObjects[4]					= GemGlow02;
	CartObjects[5]					= GemGlow03;
	CartObjects[6]					= Gems;
	CartObjects[7]					= MainLock01;
	CartObjects[8]					= MainLock02;
	CartObjects[9]					= MainLock03;
	CartObjects[10]					= MainLock01Open;
	CartObjects[11]					= MainLock02Open;
	CartObjects[12]					= MainLock03Open;
	CartObjects[13]					= SideLock;
	CartObjects[14]					= SideLockOpen;
	CartObjects[15]					= TopHandle;
	CartObjects[16]					= TyreInPlace;
	CartObjects[17]					= BoxPuzzleDoor;
	CartObjects[18]					= BoxMask;
	CartObjects[19]					= Heart;
	
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition > 2 )
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition = 2;
	else if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition < 0 )
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition = 0;

	GemGlow01	->SetEnable(false);
	GemGlow02	->SetEnable(false);
	GemGlow03	->SetEnable(false);

	//Vase->SetZ( Box->ZOrder - 1 );
	CPoint RelativeOffset = CPoint(-72.0f * ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition, 
		35.0f * ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition,0.0f);
	for (int i = 0; i < 20; i++)
	{
		CartObjectsPos[i] = CartObjects[i]->Pos;
		CPoint NewPosition = CPoint( CartObjectsPos[i].x + RelativeOffset.x, CartObjectsPos[i].y + RelativeOffset.y, 0 );
		CartObjects[i]->SetPos( &NewPosition ); 

	}

	for (int i = 0; i < 3; i++)
	{
		if ( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition != i )
		{
			Lever[i]->SetEnable(false); 
		} 
	}


	HOScnObjects[e_Rail]					= RailInScene;
	HOScnObjects[e_Wheel]					= TyreInScene;
	HOScnObjects[e_Spanner]					= Spanner;
	HOScnObjects[e_TopHandle]				= TopHandleInScene;
	HOScnObjects[e_LaserDevice]				= LesarDeviceInScene;

	HOInHandObjects[e_Rail]					= RailOnClick;
	HOInHandObjects[e_Wheel]				= TyreOnclick;
	HOInHandObjects[e_Spanner]				= SpannerOnClick;
	HOInHandObjects[e_TopHandle]			= TopHandleOnClick;
	HOInHandObjects[e_LaserDevice]			= LaserDeviceInHand;

	HODestObjects[e_Rail]					= Rail;
	HODestObjects[e_Wheel]					= TyreInPlace;
	HODestObjects[e_Spanner]				= Spanner;
	HODestObjects[e_TopHandle]				= TopHandle;
	HODestObjects[e_LaserDevice]			= LaserDevice;
	
	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[0] == -1 )
	{
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Rail]			 = e_L6S3_Inactive;
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Wheel]			 = e_L6S3_Inactive;
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner]			 = e_L6S3_Inactive;
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle]		 = e_L6S3_Inactive;
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice]		 = e_L6S3_Locked;
	}

	for (int i = 0; i < 6; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[i] == e_L6S3_Active )
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[i] = e_L6S3_Inactive;
	}

	for (int i = 0; i < e_L6S3HOMax; i++)
	{
		HOScnObjects[i]->SetEnable(false);
		HOInHandObjects[i]->SetEnable(false);
		HODestObjects[i]->SetEnable(false);

		if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[i] == e_L6S3_Used )
			HODestObjects[i]->SetEnable(true);
		else
			HOScnObjects[i]->SetEnable(true);
	}

	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved ) 
	{
		BoxPuzzleDoor->SetEnable(false);
		Code->SetEnable(false);
		CodeInScene->SetEnable(false);
		BoxMask->SetEnable(false);
	}
	
	if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeApplied )
		Code->SetEnable(false);	
	
	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeTaken )
		CodeInScene->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] != e_L6S3_Used)
		LaserLight->SetEnable(false);
	else
		Effects->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle] != e_L6S3_Used)
		SideLockOpen->SetEnable(false);
	else
		SideLock->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] == e_L6S3_Used)
		Spanner->SetEnable(false);

	if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened1 )
		MainLock01Open->SetEnable(false);
	else
	{
		MainLock01->SetEnable(false);
		GemGlow01->SetEnable(true);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened2 )
		MainLock02Open->SetEnable(false);
	else
	{
		MainLock02->SetEnable(false);
		GemGlow02->SetEnable(true);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened3 )
		MainLock03Open->SetEnable(false);
	else
	{
		MainLock03->SetEnable(false);
		GemGlow03->SetEnable(true);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseToppled )
	{
		VaseBottom->SetEnable(false);
		Vase_Door->SetEnable(false);
		LesarDeviceInScene->SetEnable(false);
		Bolt1->SetEnable(false);
		Bolt2->SetEnable(false);
	}
	else
	{
		Vase->SetEnable(false);
	}
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed)
		Bolt1->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed)
		Bolt2->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved)
		Vase_Door->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle] == e_L6S3_Used &&
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Wheel] == e_L6S3_Used &&
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Rail] == e_L6S3_Used &&
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved )
	{
		UpdateGems();
	}

	CheckIfHOComplete();


}

void L6_S3_GlowingHeartHO ::InitAnimations()
{
	Effects->SetBlendMode(eBlend_AlphaAdditive);
	Effects->Animate(0.45f, 1.0f,3.0f);

	LaserLight->SetZ(LaserDevice->ZOrder -1 );
	LaserLight->SetBlendMode(eBlend_AlphaAdditive);
	LaserLight->Animate(0.75f, 1.0f, 7.0f);

	

	S3_HO_GoldGlitter3FX = new SuperFX("PRT\\L6\\S3_HO_GoldGlitter3.INI", PointSystem::CreateCPoint(433,545,0), 1100); 
	S3_HO_GoldGlitter3FX->Start();
	S3_HO_GoldGlitter3FX->AdvanceByTime(2.0f);

	S3_HO_GoldGlitter2FX = new SuperFX("PRT\\L6\\S3_HO_GoldGlitter2.INI", PointSystem::CreateCPoint(93,154,0), 1100); 
	S3_HO_GoldGlitter2FX->Start();
	S3_HO_GoldGlitter2FX->AdvanceByTime(2.0f);

	S3_HO_GoldGlitterFX = new SuperFX("PRT\\L6\\S3_HO_GoldGlitter.INI", PointSystem::CreateCPoint(1172,181,0), 1100); 
	S3_HO_GoldGlitterFX->Start();
	S3_HO_GoldGlitterFX->AdvanceByTime(2.0f);

	S3_HOLightEffectFX = new SuperFX("PRT\\L6\\S3_HOLightEffect.INI", PointSystem::CreateCPoint(928,557,0), LaserDevice->ZOrder - 1); 
	S3_HOLightEffectFX->Start();
	S3_HOLightEffectFX->AdvanceByTime(2.0f);

	LightRays->SetBlendMode(eBlend_AlphaAdditive);
	LightRays->Animate(0.25f, 0.9f, 6.0f);

	JanimPuzzleDoorRetreat = NULL;
	JanimPuzzleDoorRetreat = new JSONAnimator( 0, CC_CALLBACK_1(L6_S3_GlowingHeartHO::OnAnimationComplete, this ));
	JanimPuzzleDoorRetreat->parseAndLoadJSON("ANIM\\L6\\S3_PUZZLEDOORRETREAT.JSON", HOScene );
}

void L6_S3_GlowingHeartHO ::OnAnimationComplete(int index)
{
	IsAnimationPlaying = false;
	switch (index)
	{
	case 0:	BoxMask->SetEnable(false);
		UpdateGems();
		JanimPuzzleDoorRetreat->SetEnableObjects(false);
		break;
	default:
		break;
	}
}

void L6_S3_GlowingHeartHO ::InitControlPolys()
{
	int ObjArray1[] = {190,87,277,41,329,98,286,108,228,117};
	HOPolyArray[e_Rail] =  new CPolygon(5);
	HOPolyArray[e_Rail]->Init(ObjArray1);
	int ObjArray2[] = {432,450,495,423,539,469,473,502};
	HOPolyArray[e_Wheel] =  new CPolygon(4);
	HOPolyArray[e_Wheel]->Init(ObjArray2);
	int ObjArray3[] = {948,127,998,112,1124,243,1027,283};
	HOPolyArray[e_Spanner] =  new CPolygon(4);
	HOPolyArray[e_Spanner]->Init(ObjArray3);
	int ObjArray4[] = {1014,384,1059,426,1154,318,1082,267};
	HOPolyArray[e_TopHandle] =  new CPolygon(4);
	HOPolyArray[e_TopHandle]->Init(ObjArray4);
	int ObjArray6[] = {300,234,363,258,394,327,345,393,262,400,205,332,227,264};
	HOPolyArray[e_LaserDevice] =  new CPolygon(7);
	HOPolyArray[e_LaserDevice]->Init(ObjArray6);

	int DestArray1[] = {457,580,537,533,591,592,515,641};
	DestPolyArray[e_Rail]=  new CPolygon(4);
	DestPolyArray[e_Rail]->Init(DestArray1);
	int DestArray2[] = {415,357,466,333,490,362,475,406,424,422,402,390};
	DestPolyArray[e_Wheel]=  new CPolygon(6);
	DestPolyArray[e_Wheel]->Init(DestArray2);
	int DestArray3[] = {358,454,268,441,217,379,207,288,254,165,366,144,407,183,397,289,377,390};
	DestPolyArray[e_Spanner]=  new CPolygon(9);
	DestPolyArray[e_Spanner]->Init(DestArray3);
	int DestArray4[] = {667,151,602,123,591,21,618,0,770,4,785,55,776,113};
	DestPolyArray[e_TopHandle]=  new CPolygon(7);
	DestPolyArray[e_TopHandle]->Init(DestArray4);
	int DestArray6[] = {823,653,817,558,833,468,926,383,1005,414,1050,526,1042,621,1009,696,897,725};
	DestPolyArray[e_LaserDevice]=  new CPolygon(9);
	DestPolyArray[e_LaserDevice]->Init(DestArray6);

	int CodeArray[] = {1049,135,1095,104,1192,191,1115,223};
	CodePoly=  new CPolygon(4);
	CodePoly->Init(CodeArray);

	int HoBoxArray[] = {378,370,551,597,1004,374,1010,150,880,9,393,165};
	HoBoxPoly=  new CPolygon(6);
	HoBoxPoly->Init(HoBoxArray);
	
	int RailArray2[] = {142,507,189,590,551,417,491,345};
	RailPoly2=  new CPolygon(4);
	RailPoly2->Init(RailArray2);
	
	int RailPolyArray[] = {952,324,1015,381,336,740,262,667};
	RailPoly1 =  new CPolygon(4);
	RailPoly1->Init(RailPolyArray);
	
	/*int LeverArray[] = {1028,528,1119,574,1143,554,1214,537,1228,513,1343,486,1327,428,1202,469,1132,469,1135,418,1109,402,1111,293,1068,292,1073,408,1061,420,1038,385,970,286,931,310,990,412,994,440};
	LeverPoly=  new CPolygon(20);
	LeverPoly->Init(LeverArray);*/

	int LeverArray1[] = {1035,536,1109,581,1138,577,1156,555,1201,545,1222,521,1342,497,1348,458,1311,431,1205,460,1164,448,1092,481,1045,488};
	LeverPoly[0] =  new CPolygon(13); 
	LeverPoly[0]->Init(LeverArray1);

	int LeverArray2[] = {1024,542,1123,580,1166,550,1160,496,1142,476,1149,433,1132,403,1119,294,1092,281,1065,300,1068,402,1053,423,1050,486,1017,506};
	LeverPoly[1] =  new CPolygon(14);
	LeverPoly[1]->Init(LeverArray2);

	int LeverArray3[] = {1031,542,1116,585,1152,559,1164,503,1140,465,1103,457,1068,409,1034,386,977,287,938,287,930,316,988,410,989,443,1031,512};
	LeverPoly[2] =  new CPolygon(14);
	LeverPoly[2]->Init(LeverArray3);

	
}

void L6_S3_GlowingHeartHO ::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopupID != ePopInactive)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L6_S3_GlowingHeartHO ::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S3_GlowingHeartHO] = isAnyTaskLeft;

}

void L6_S3_GlowingHeartHO::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < ePopL6S3_Max; i++)
	{
		Popup * popup = PopupArray[i];
		if (popup &&  CheckIfPopupActive(i) )
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

	if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeTaken )
	{
		sparklePointsVec.push_back(CPoint(CodePoly->center.x, CodePoly->center.y, 0));
		isSparkleUpdatePending = true;

	}
	else if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved )
	{
		sparklePointsVec.push_back(CPoint(PuzzlePoly->center.x, PuzzlePoly->center.y, 0));
		isSparkleUpdatePending = true;

	}
	else if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle] != e_L6S3_Used && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle] == e_L6S3_Inactive)
	{
		sparklePointsVec.push_back(CPoint(HOPolyArray[e_TopHandle]->center.x, HOPolyArray[e_TopHandle]->center.y, 0));
		isSparkleUpdatePending = true;

	}
	else if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Wheel] != e_L6S3_Used )
	{
		sparklePointsVec.push_back(CPoint(HOPolyArray[e_Wheel]->center.x, HOPolyArray[e_Wheel]->center.y, 0));
		isSparkleUpdatePending = true;

	}	
	else if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Rail] != e_L6S3_Used )
	{
		sparklePointsVec.push_back(CPoint(HOPolyArray[e_Rail]->center.x, HOPolyArray[e_Rail]->center.y, 0));
		isSparkleUpdatePending = true;

	}	
	else if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseToppled )
	{
		sparklePointsVec.push_back(CPoint(VaseRect.GetCenterX(), VaseRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}	
	else if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] != e_L6S3_Used && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] == e_L6S3_Inactive)
	{
		sparklePointsVec.push_back(CPoint(HOPolyArray[e_Spanner]->center.x, HOPolyArray[e_Spanner]->center.y, 0));
		isSparkleUpdatePending = true;

	}		
	else if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] == e_L6S3_Used && 
					!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved )
	{
		sparklePointsVec.push_back(CPoint(PopPolyArray[ePop2_L6S3Pop2Vase]->center.x, PopPolyArray[ePop2_L6S3Pop2Vase]->center.y, 0));
		isSparkleUpdatePending = true;

	}		
	else if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] != e_L6S3_Used && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] == e_L6S3_Inactive && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved )
	{
		sparklePointsVec.push_back(CPoint(HOPolyArray[e_LaserDevice]->center.x, HOPolyArray[e_LaserDevice]->center.y, 0));
		isSparkleUpdatePending = true;

	}	
	else if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainBoxOpened && 
					!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken )
	{
		sparklePointsVec.push_back(CPoint( HeartRect[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition].GetCenterX(), 
			HeartRect[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition].GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}	
	else if(	ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] == e_L6S3_Used && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved &&
				(  !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened1 || !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened2 ||
					!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened3 ))
	{
		sparklePointsVec.push_back(CPoint( LeverPoly[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->center.x, LeverPoly[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->center.y, 0));
		isSparkleUpdatePending = true;

	}	
}

void L6_S3_GlowingHeartHO ::ProcessSkip()
{
	if( ActivePopupID == ePop1_L6S3Pop1Code )
		Pop1Code->ProcessSkip();

}



void L6_S3_GlowingHeartHO ::ProcessNonHOHint()
{


	if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken ||  IsLockOpening != -1 || ProfileSystem::Instance->ActiveProfile._IsMatch3Playing )
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 15;
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
				if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeTaken )
				{
					InitHintTrailEmit( CodePoly->center.x, CodePoly->center.y );
					return;
				}
			break;
		case 2: ++iNonHOHint;
				if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved )
				{
					InitHintTrailEmit( PuzzlePoly->center.x, PuzzlePoly->center.y );
					return;
				}
			break;
		case 3: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle] != e_L6S3_Used && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_TopHandle] == e_L6S3_Inactive)
				{
					Hud->ShowHintForSceneItem( &DestPolyArray[e_TopHandle]->center );
					InitHintTrailEmit( HOPolyArray[e_TopHandle]->center.x, HOPolyArray[e_TopHandle]->center.y );
					return;
				}
			break;
		case 4: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Wheel] != e_L6S3_Used )
				{
					Hud->ShowHintForSceneItem( &DestPolyArray[e_Wheel]->center );
					InitHintTrailEmit( HOPolyArray[e_Wheel]->center.x, HOPolyArray[e_Wheel]->center.y );
					return;
				}
			break;
		case 5: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Rail] != e_L6S3_Used )
				{
					Hud->ShowHintForSceneItem( &DestPolyArray[e_Rail]->center );
					InitHintTrailEmit( HOPolyArray[e_Rail]->center.x, HOPolyArray[e_Rail]->center.y );
					return;
				}
			break;
		case 6: ++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseToppled  )
				{
					InitHintTrailEmit( VaseRect.GetCenterX(), VaseRect.GetCenterY() );
					return;
				}
			break;
		case 7: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] != e_L6S3_Used && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] == e_L6S3_Inactive)
				{
					Hud->ShowHintForSceneItem( &DestPolyArray[e_Spanner]->center );
					InitHintTrailEmit( HOPolyArray[e_Spanner]->center.x, HOPolyArray[e_Spanner]->center.y );
					return;
				}
			break;
		case 8: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_Spanner] == e_L6S3_Used && 
					!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved )
				{
					InitHintTrailEmit( PopPolyArray[ePop2_L6S3Pop2Vase]->center.x, PopPolyArray[ePop2_L6S3Pop2Vase]->center.y );
					return;
				}
			break;
		case 9: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] != e_L6S3_Used && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] == e_L6S3_Inactive && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved )
				{
					Hud->ShowHintForSceneItem( &DestPolyArray[e_LaserDevice]->center );
					InitHintTrailEmit( HOPolyArray[e_LaserDevice]->center.x, HOPolyArray[e_LaserDevice]->center.y );
					return;
				}
			break;
		case 10: ++iNonHOHint;
				if( ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainBoxOpened && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken )
				{
					InitHintTrailEmit( HeartRect[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition].GetCenterX(), 
						HeartRect[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition].GetCenterY() );
					return;
				}
			break;
		case 11: ++iNonHOHint;
				if(	ProfileSystem::Instance->ActiveProfile.L6_S3_HO_ObjectStates[e_LaserDevice] == e_L6S3_Used && 
					ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved &&
				(  !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened1 || !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened2 ||
					!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_MainLockOpened3 ))
				{
					InitHintTrailEmit( &LeverPoly[ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CartPosition]->center);
					return;
				}
			break;
		case 12: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 13: ++iNonHOHint;
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

L6_S3_GlowingHeartHO ::~L6_S3_GlowingHeartHO ()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL6s3FloatingIsland3Amb);
	Control::Audio->UnloadSample(aAmbL6s3FloatingIsland3Amb);
	//MUSIC_SFX

	for (int i = 0; i < e_L6S3HOMax; i++)
	{
		SAFE_DELETE( HOPolyArray[i] );
		SAFE_DELETE( DestPolyArray[i] );
		
	}

	for (int i = 0; i < 3; i++)
	{
		SAFE_DELETE( LeverPoly[i] ); 
	}

	SAFE_DELETE( CodePoly );
	SAFE_DELETE( HoBoxPoly );
	SAFE_DELETE( RailPoly2 );
	SAFE_DELETE( RailPoly1 );
	

	SAFE_DELETE(S3_HO_GoldGlitter2FX);
	SAFE_DELETE(S3_HO_GoldGlitterFX);
	SAFE_DELETE(S3_HOLightEffectFX);
	SAFE_DELETE(S3_HO_GoldGlitter3FX);
	SAFE_DELETE(JanimPuzzleDoorRetreat);
}


void L6_S3_GlowingHeartHO::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				//Cursor::SetMode(CA_ExitUp);
				Hud->InvBoxCtrl->ResetToInvBox();
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