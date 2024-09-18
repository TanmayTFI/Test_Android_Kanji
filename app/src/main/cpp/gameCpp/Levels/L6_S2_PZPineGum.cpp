
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L6_S2_PZPineGum.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"

enum ObjectStates
{
	Obj_Locked = 0,
	Obj_Inactive,
	Obj_Active,
	Obj_Used
};

enum L6S2PZ_Objects
{
	e_L6S2_Stone1 = 0,
	e_L6S2_Stone2,
	e_L6S2_Leaves1,
	e_L6S2_Leaves2,
	e_L6S2_Leaves3,
	e_L6S2_PinePitchPan,
	e_L6S2_CharcoalMortar,
	e_L6S2_Pestle,
	e_L6S2_Max
};

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  SceneRect(185.0f,39.0f,1084.0f,672.0f);
const CRectangle  ChartRect(625.0f,158.0f,228.0f,121.0f);

L6_S2_PZPineGum::L6_S2_PZPineGum()
{
	 ObjectInHandID = -1;
	 IsFadingIn				= false;
	 IsFadingOut	= false;
	 IsChartActive	= false;
	 FadeAlpha		= 0.0f;
	 IsAnimPlaying = false;
	 IsStickInHand  = false;
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S2\\PZ1\\PZ1.SCN");

	IsPuzzleModeActive = true;
	InitSceneElements();
	InitControlPolys();
	InitAnimations();

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s2PineGumPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s2PineGumPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL6s2PineGumPuzzleAmb,true);
	}
	

	int ChartArray[] = {672,184,841,206,820,289,639,250};
	ChartPoly = new CPolygon(4);
	ChartPoly->Init(ChartArray);
	InstructionChart = new L6S2_PZPop1Chart(0, Hud, HOScene, eZOrder_Popup, ChartPoly->center, CC_CALLBACK_1(L6_S2_PZPineGum::OnPopupClosed, this));
	InstructionChart->SetPopupBoundingBox(PopAreaRect);

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L6_S2_PZPineGum]  = true;

	TaskPendingUpdate();
}

void L6_S2_PZPineGum::OnPopupClosed(Popup * popup)
{
	IsChartActive = false;
}


void L6_S2_PZPineGum::Required()
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
			 if( !Control::Audio->IsPlaying(aTrackL6puzzle) )
				  Control::Audio->PlaySample(aTrackL6puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL6puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL6puzzle, true);
		}
	}
}

void L6_S2_PZPineGum::Update()
{
	#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif
	NavigateToScene();


	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	HiddenObjectSystem::HOUpdate();
	if(IsAnimPlaying)
	{
		Hud->AllowInput = false;
		return;
	}
	
	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	if(IsChartActive)
		return;

	if( IsStickInHand )
	{
		StickInHand->SetPos( &pos );
		if( pos.y >= ( HoveredBackBtnRectMax.topleft.y + 60 ) )
		{
			StickInScene->SetEnable(true);
			StickInHand->SetEnable(false);
			IsStickInHand = false;
			return;
		}
		if( Control::Input->LBclicked() )
		{
			StickInScene->SetEnable(true);
			StickInHand->SetEnable(false);
			IsStickInHand = false;
			if(	DestPolyArray[e_L6S2_PinePitchPan]->Intersection(&pos) && 
				!ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] == 1 &&
				PZObjStates[e_L6S2_PinePitchPan] == Obj_Used && PZObjStates[e_L6S2_CharcoalMortar] == Obj_Used && 
				PZObjStates[e_L6S2_Stone1] == Obj_Used && PZObjStates[e_L6S2_Stone2] == Obj_Used && 
				PZObjStates[e_L6S2_Leaves1] == Obj_Used && PZObjStates[e_L6S2_Leaves2] == Obj_Used && PZObjStates[e_L6S2_Leaves3] == Obj_Used)
			{
				GlueMixAnim->PlayAnim();
				Control::Audio->QuickLoadAndPlaySFX("s2pz1_pineGumMix");
				IsAnimPlaying = true;
				StickInScene->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected = true;
			}
			else
			{
				StickInScene->SetEnable(true);
				StickInHand->SetEnable(false);
				IsStickInHand = false;
				return;
			}

		}
		return;
	}

	


	if (ObjectInHandID == -1 )
	{
		if( !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken && PanPoly->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
				Cursor::SetMode(CA_Normal);
				Hud->TakeInventoryItem(eInv_L6_S1_MetalContainer);
				PanInScene->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken = true;
				TaskPendingUpdate();
			}
			return;
		}
		if( StickPoly->Intersection(&pos)  )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
				Cursor::SetMode(CA_Normal);
				IsStickInHand = true;
				StickInHand->SetPos( &pos );
				StickInScene->SetEnable(false);
				StickInHand->SetEnable(true);
				
			}
			return;
		}
		if(ChartRect.Intersection(&pos) )
		{
			Cursor::SetMode(CA_Look);
			if( Control::Input->LBclicked())
			{
				InstructionChart->OpenPopup(); 
				IsChartActive = true;
			}
			return;
		} 
	}

	UpdatePuzzleMode(pos);
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L6_S2_FloatingIsland2);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L6_S2_FloatingIsland2);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S2_FloatingIsland2, 2);
			}
#else			
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}

	UpdateBannerMessages(pos);
}



void L6_S2_PZPineGum::UpdatePuzzleMode(CPoint pos)
{
	if (ObjectInHandID != -1 )
	{
		PZInHandObjects[ObjectInHandID]->SetPos( &pos );
		if( pos.y >= ( HoveredBackBtnRectMax.topleft.y + 60 ) )
		{
			ResetDragItem();	
			PZInHandObjects[ObjectInHandID]->SetZ( PZInHandObjects[ObjectInHandID]->ZOrder - eZOrder_INVBOXITEMS );
			PZInHandObjects[ObjectInHandID]->SetEnable(false);
			ObjectInHandID = -1;
			return;
		}
		if( Control::Input->LBclicked() )
		{
			if( ObjectInHandID != e_L6S2_PinePitchPan )
				PZInHandObjects[ObjectInHandID]->SetEnable(false);
			if (DestPolyArray[ObjectInHandID]->Intersection(&pos))
			{
				PZScnObjects[ObjectInHandID]->SetEnable(false);
				PZObjStates[ObjectInHandID] = Obj_Used;
				switch (ObjectInHandID)
				{
				case e_L6S2_Stone1:		Rock01->SetEnable(true);
										ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] = 1;
										Control::Audio->QuickLoadAndPlaySFX("s2pz1_placeRock");
					break;
				case e_L6S2_Stone2:		Rock02->SetEnable(true);
										ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] = 1;
										Control::Audio->QuickLoadAndPlaySFX("s2pz1_placeRock");
					break;
				case e_L6S2_Leaves1:	Leaves->SetEnable(true);
										ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves1] = 1;
										Control::Audio->QuickLoadAndPlaySFX("s2pz1_placeLeaves");
					break;
				case e_L6S2_Leaves2:	Leaves->SetEnable(true);
										ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves2] = 1;
										Control::Audio->QuickLoadAndPlaySFX("s2pz1_placeLeaves");
					break;
				case e_L6S2_Leaves3:	Leaves->SetEnable(true);
										ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves3] = 1;
										Control::Audio->QuickLoadAndPlaySFX("s2pz1_placeLeaves");
					break;
				case e_L6S2_PinePitchPan:if ( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] && ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] &&
											  !ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] )
										{
											Pan->SetEnable(true);
											ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] = 1;
											PinePitch->SetEnable(true);
											Control::Audio->QuickLoadAndPlaySFX("s2pz1_placePinePitchPan"); 
										}
										else
											ResetDragItem();
										break;
										//dropped charcoal but not yet crushed
				case e_L6S2_Pestle:		if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced  && !ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] )
										{
											PZInHandObjects[e_L6S2_CharcoalMortar]->PlayAnimation(1);
											CharcoalCrushAnim->PlayAnim();
											Control::Audio->QuickLoadAndPlaySFX("s2pz1_charcoalGrind");
											IsAnimPlaying = true;
											ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] = 1; 
										}
										else
											ResetDragItem();
										break;
										//pitch pan placed, charcoal placed & crushed - then time to show crushed charcoal to pan (actually need to add over leaves to help create stronger fire ) 
				case e_L6S2_CharcoalMortar:		if ( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] && 
													 ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] &&
													 !ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar])
										{
											Control::Audio->QuickLoadAndPlaySFX("s2pz1_addCharcoalToPan");
											CharcoalAddAnim->PlayAnim();
											IsAnimPlaying = true;
											ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] = 1; 
										}
										else
										{
											ResetDragItem();
											if(PZObjStates[e_L6S2_Pestle] != Obj_Used )
												Hud->ShowBannerText("L6too");
										}
										break;
				
				}
				UpdatePuzzleStates();
			}
			else
				ResetDragItem();	
			TaskPendingUpdate();
			PZInHandObjects[ObjectInHandID]->SetZ( PZInHandObjects[ObjectInHandID]->ZOrder - eZOrder_INVBOXITEMS );
			ObjectInHandID = -1;
			return; 
		}
	}
	else
	{
		for (int i = 0; i < e_L6S2_Max; i++)
		{
			if( PZPolyArray[i]->Intersection(&pos) && PZObjStates[i] ==  Obj_Inactive)
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					//SFX
					int randId = rand()%3;
					switch (randId)
					{
					case 0:		Control::Audio->QuickLoadAndPlaySFX(aSFXL2S2HOObjectClickv1);		break;
					case 1:		Control::Audio->QuickLoadAndPlaySFX(aSFXL2S2HOObjectClickv2);		break;
					case 2:		Control::Audio->QuickLoadAndPlaySFX(aSFXL2S2HOObjectClickv3);		break;
					default:
						break;
					}
					PZObjStates[i] = Obj_Active;
					ObjectInHandID = i;
					PZInHandObjects[ObjectInHandID]->SetZ( PZInHandObjects[ObjectInHandID]->ZOrder + eZOrder_INVBOXITEMS );
					PZInHandObjects[i]->SetPos(&pos);
					PZScnObjects[i]->SetEnable(false);
					PZInHandObjects[i]->SetEnable(true);
				}
				return;
			} 
		}
	}
}

void L6_S2_PZPineGum::UpdatePuzzleStates()
{
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced && PZObjStates[e_L6S2_PinePitchPan] != Obj_Used )
			PZObjStates[e_L6S2_PinePitchPan] = Obj_Inactive;

	if( ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced && PZObjStates[e_L6S2_CharcoalMortar] != Obj_Used )
		PZObjStates[e_L6S2_CharcoalMortar] = Obj_Inactive;

	if( PZObjStates[e_L6S2_PinePitchPan] == Obj_Used && PZObjStates[e_L6S2_CharcoalMortar] == Obj_Used && PZObjStates[e_L6S2_Stone1] == Obj_Used && PZObjStates[e_L6S2_Stone2] == Obj_Used 
		&& PZObjStates[e_L6S2_Leaves1] == Obj_Used && PZObjStates[e_L6S2_Leaves2] == Obj_Used && PZObjStates[e_L6S2_Leaves3] == Obj_Used )
	{
		//IsPuzzleModeActive = false;
		FireMakingAnim->PlayAnim();
		Control::Audio->QuickLoadAndPlaySFX("s2pz1_createFire");
		IsAnimPlaying =true;
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 
		//clear inevntory now itself - else user may backtrack and steel & flintstone will carry on to next level!!!
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S1_Steel);
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_FlintStones); 
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Steel] = false;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_FlintStones] = false;
	}

}

void L6_S2_PZPineGum::ResetDragItem()
{

	switch (ObjectInHandID)
	{
	case e_L6S2_Stone1:		Hud->ShowBannerText("L6put");
		break;
	case e_L6S2_Stone2:		Hud->ShowBannerText("L6put");
		break;
	case e_L6S2_Leaves1:	Hud->ShowBannerText("L6pile");
		break;
	case e_L6S2_Leaves2:	Hud->ShowBannerText("L6pile");
		break;
	case e_L6S2_Leaves3:	Hud->ShowBannerText("L6pile");
		break;
	case e_L6S2_PinePitchPan:		Hud->ShowBannerText("L6order");	
		break;
	case e_L6S2_CharcoalMortar:		GFInstance->WrongInvDropMsg(Hud);
		break;
	case e_L6S2_Pestle:		if( !ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken )
								Hud->ShowBannerText("L6better");											
							else if( !ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced )
								Hud->ShowBannerText("L6mortar");
							else 
								GFInstance->WrongInvDropMsg(Hud);
		break;
	default:
		break;
	}
										

	PZObjStates[ObjectInHandID] = Obj_Inactive;

	if( ObjectInHandID == e_L6S2_PinePitchPan) 
		PZScnObjects[e_L6S2_PinePitchPan]->SetPos( &PZPolyArray[e_L6S2_PinePitchPan]->center);
	
	PZScnObjects[ObjectInHandID]->SetEnable(true);
}

void L6_S2_PZPineGum::UpdateBannerMessages( CPoint pos)
{
	if( Control::Input->LBclicked())
	{
		if( DestPolyArray[e_L6S2_PinePitchPan]->Intersection(&pos) && PZObjStates[e_L6S2_Stone1] != Obj_Used && PZObjStates[e_L6S2_Stone2] != Obj_Used)
			Hud->ShowBannerText("L6area");
		else if( (PZPolyArray[e_L6S2_Stone1]->Intersection(&pos) || PZPolyArray[e_L6S2_Stone2]->Intersection(&pos) || DestPolyArray[e_L6S2_Stone1]->Intersection(&pos) ) && 
			( PZObjStates[e_L6S2_Stone1] != Obj_Used && PZObjStates[e_L6S2_Stone2] != Obj_Used))
			Hud->ShowBannerText("L6stone");
		else if( (PZPolyArray[e_L6S2_Leaves1]->Intersection(&pos) || PZPolyArray[e_L6S2_Leaves2]->Intersection(&pos) || PZPolyArray[e_L6S2_Leaves3]->Intersection(&pos) ) && 
			( PZObjStates[e_L6S2_Leaves1] != Obj_Used && PZObjStates[e_L6S2_Leaves2] != Obj_Used && PZObjStates[e_L6S2_Leaves3] != Obj_Used))
			Hud->ShowBannerText("L6dry");
		else if( PZPolyArray[e_L6S2_PinePitchPan]->Intersection(&pos) && PZObjStates[e_L6S2_PinePitchPan] != Obj_Used && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced )
			Hud->ShowBannerText("L6perfect");
		else if( StickPoly->Intersection(&pos) )
			Hud->ShowBannerText("L6around");
	}
}


void L6_S2_PZPineGum::OnAnimationComplete(int index)
{
	IsAnimPlaying = false;
	switch (index)
	{
	case 0:		FireMakingAnim->SetEnableObjects(false);
				Fire->SetEnable(true);
				PineGumMeltAnim->PlayAnim();
				IsAnimPlaying =true;				
		break;
	case 1:		PestleInHand->SetEnable(false);
				PestleInScene->SetEnable(false);
				PZScnObjects[e_L6S2_CharcoalMortar]->PlayAnimation(1);
				PZInHandObjects[e_L6S2_CharcoalMortar]->PlayAnimation(1);
		break;
	case 2:		PinePitch->SetEnable(false);
		break;
	case 3:		StickInHand->SetEnable(false);
				GlueStick_Scene->SetEnable(false);
				Hud->TakeInventoryItem(eInv_L6_S2_PinegumStick);
		break;
	case 4:		CharcoalAddAnim->SetEnableObjects(false);
		CoalPowder->SetEnable(true);
		break;
	}
	Hud->AllowInput = true;
	TaskPendingUpdate();
}

void L6_S2_PZPineGum::InitAnimations()
{
	FireSparks = new SuperFX("PRT\\L6\\S2_PZ1_firesparks.INI", PointSystem::CreateCPoint(1000,473,0), 1100); 
	superfxArray1.push_back( FireSparks );
	FireMakingAnim = NULL;
	FireMakingAnim = new JSONAnimator( 0, CC_CALLBACK_1(L6_S2_PZPineGum::OnAnimationComplete, this ));
	FireMakingAnim->parseAndLoadJSON("ANIM\\L6\\S2_FLINTONSTEEL.JSON", HOScene, &superfxArray1 );
	
	CharcoalCrushAnim = NULL;
	CharcoalCrushAnim = new JSONAnimator( 1, CC_CALLBACK_1(L6_S2_PZPineGum::OnAnimationComplete, this ));
	CharcoalCrushAnim->parseAndLoadJSON("ANIM\\L6\\S2_CHARCOALGRINDING.JSON", HOScene );

	VapourFX = new SuperFX("PRT\\L6\\S2_PZ1_Vapour.INI", PointSystem::CreateCPoint(994,330,0), 1100); 
	superfxArray2.push_back( VapourFX );
	PineGumMeltAnim = NULL;
	PineGumMeltAnim = new JSONAnimator( 2, CC_CALLBACK_1(L6_S2_PZPineGum::OnAnimationComplete, this ));
	PineGumMeltAnim->parseAndLoadJSON("ANIM\\L6\\S2_PINEGUMMELTING.JSON", HOScene, &superfxArray2 );
	
	GlueMixAnim = NULL;
	GlueMixAnim = new JSONAnimator( 3, CC_CALLBACK_1(L6_S2_PZPineGum::OnAnimationComplete, this ));
	GlueMixAnim->parseAndLoadJSON("ANIM\\L6\\S2_GLUEMIX.JSON", HOScene );

	CharcoalAddAnim = NULL;
	CharcoalAddAnim = new JSONAnimator( 4, CC_CALLBACK_1(L6_S2_PZPineGum::OnAnimationComplete, this ));
	CharcoalAddAnim->parseAndLoadJSON("ANIM\\L6\\S2_CHARCOALPOWDERADDTOPAN.JSON", HOScene );
}


void L6_S2_PZPineGum::InitSceneElements()
{

	ChartInScene		= HOScene->GetObjectByID("ChartInScene");
	CoalPowder			= HOScene->GetObjectByID("CoalPowder");
	FlintInHand			= HOScene->GetObjectByID("FlintInHand");
	GlueStick_Scene		= HOScene->GetObjectByID("GlueStick_Scene");
	Leaves				= HOScene->GetObjectByID("Leaves");
	Leaves01InScene		= HOScene->GetObjectByID("Leaves01InScene");
	Leaves02InScene		= HOScene->GetObjectByID("Leaves02InScene");
	Leaves03InScene		= HOScene->GetObjectByID("Leaves03InScene");
	LeavesInHand		= HOScene->GetObjectByID("LeavesInHand");
	MortarInHand		= HOScene->GetObjectByID("MortarInHand");
	MortarInScene		= HOScene->GetObjectByID("MortarInScene");
	Pan					= HOScene->GetObjectByID("Pan");
	PanInHand			= HOScene->GetObjectByID("PanInHand");
	PanInScene			= HOScene->GetObjectByID("PanInScene");
	PestleInHand		= HOScene->GetObjectByID("PestleInHand");
	PestleInScene		= HOScene->GetObjectByID("PestleInScene");
	PineGlue			= HOScene->GetObjectByID("PineGlue");
	PinePitch			= HOScene->GetObjectByID("PinePitch");
	PinePitchMelted		= HOScene->GetObjectByID("PinePitchMelted");
	PlantMask			= HOScene->GetObjectByID("PlantMask");
	Rock01				= HOScene->GetObjectByID("Rock01");
	Rock01InHand		= HOScene->GetObjectByID("Rock01InHand");
	Rock01InScene		= HOScene->GetObjectByID("Rock01InScene");
	Rock02				= HOScene->GetObjectByID("Rock02");
	Rock02InHand		= HOScene->GetObjectByID("Rock02InHand");
	Rock02InScene		= HOScene->GetObjectByID("Rock02InScene");
	Rock03 				= HOScene->GetObjectByID("Rock03");
	Rock03InHand		= HOScene->GetObjectByID("Rock03InHand");
	SteelInHand			= HOScene->GetObjectByID("SteelInHand");
	StickInHand			= HOScene->GetObjectByID("StickInHand");
	StickInScene		= HOScene->GetObjectByID("StickInScene");
	Fire				= HOScene->GetObjectByID("FireLoop");

	FlintInHand			    ->SetEnable(false);
	//StickInScene			->SetEnable(false);
	SteelInHand				->SetEnable(false);
	CoalPowder				->SetEnable(false);
	Leaves					->SetEnable(false);
	Pan						->SetEnable(false);
	PineGlue				->SetEnable(false);
	PinePitch				->SetEnable(false);
	PinePitchMelted			->SetEnable(false);
	//PlantMask				->SetEnable(false);
	Rock01					->SetEnable(false);
	Rock02					->SetEnable(false);
	GlueStick_Scene			->SetEnable(false);
	StickInHand				->SetEnable(false);
	Fire					->SetEnable(false);
	//StickInScene			->SetEnable(false);

	PZScnObjects[e_L6S2_Stone1]		= Rock01InScene;
	PZScnObjects[e_L6S2_Stone2]		= Rock02InScene;
	PZScnObjects[e_L6S2_Leaves1]	= Leaves01InScene;
	PZScnObjects[e_L6S2_Leaves2]	= Leaves02InScene;
	PZScnObjects[e_L6S2_Leaves3]	= Leaves03InScene;
	PZScnObjects[e_L6S2_PinePitchPan]		= PanInHand;
	PZScnObjects[e_L6S2_CharcoalMortar]		= MortarInScene;
	PZScnObjects[e_L6S2_Pestle]		= PestleInScene;

	PZInHandObjects[e_L6S2_Stone1]	= Rock01InHand;
	PZInHandObjects[e_L6S2_Stone2]	= Rock02InHand;
	PZInHandObjects[e_L6S2_Leaves1]	= LeavesInHand;
	PZInHandObjects[e_L6S2_Leaves2]	= LeavesInHand;
	PZInHandObjects[e_L6S2_Leaves3]	= LeavesInHand;
	PZInHandObjects[e_L6S2_PinePitchPan]		= PanInHand;
	PZInHandObjects[e_L6S2_CharcoalMortar]	= MortarInHand;
	PZInHandObjects[e_L6S2_Pestle]	= PestleInHand;

	for (int i = 0; i < e_L6S2_Max; i++)
	{
		if(i != e_L6S2_PinePitchPan )
			PZInHandObjects[i]->SetEnable(false); 
	}

	PZObjStates[e_L6S2_Stone1]		=  Obj_Inactive;
	PZObjStates[e_L6S2_Stone2]		=  Obj_Inactive;
	PZObjStates[e_L6S2_Leaves1]		=  Obj_Inactive;
	PZObjStates[e_L6S2_Leaves2]		=  Obj_Inactive;
	PZObjStates[e_L6S2_Leaves3]		=  Obj_Inactive;
	PZObjStates[e_L6S2_PinePitchPan]			=  Obj_Locked;
	PZObjStates[e_L6S2_CharcoalMortar]		=  Obj_Locked;
	PZObjStates[e_L6S2_Pestle]		=  Obj_Locked;
	// Sac: Modified the ! based checks to have a () surrounding the checks, or else it is not sounding right
	// unless that is what intended... need explaination
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced && !(ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] == 1) )
		PZObjStates[e_L6S2_Pestle]		=  Obj_Inactive;

	if( !(ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] == 1) && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced )
		PZObjStates[e_L6S2_PinePitchPan] =  Obj_Inactive;

	if(ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced && !(ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] == 1) ) //Sac: changed the loginga not to have a checking expression
		PZObjStates[e_L6S2_CharcoalMortar] =  Obj_Inactive;

	for( int i = 0; i < e_L6S2_Max; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[i] == 1 )
		{
			PZScnObjects[i]->SetEnable(false);
			PZObjStates[i] = Obj_Used;
		}
	}

	if( ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken )
		PanInScene->SetEnable(false);
	
	if( !ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced )
		PZScnObjects[e_L6S2_CharcoalMortar]->SetEnable(false);
	else if(ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] == 1)
	{
		PZScnObjects[e_L6S2_CharcoalMortar]->PlayAnimation(1);
		PZInHandObjects[e_L6S2_CharcoalMortar]->PlayAnimation(1);
	}
	if( !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced )
		PZScnObjects[e_L6S2_PinePitchPan]->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced )
		PZScnObjects[e_L6S2_Pestle]->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] == 1 )
		Rock01->SetEnable(true);
	if(ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] == 1 )
		Rock02->SetEnable(true);
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves1] == 1 ||
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves2] == 1 ||
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves3] == 1)
		Leaves->SetEnable(true);
	if(ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] == 1 )
	{
		Pan->SetEnable(true);
		PinePitch->SetEnable(true);
	}
	if(ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] == 1)
	{
		MortarInScene->SetEnable(false);
		MortarInHand->SetEnable(true);
		MortarInHand->SetPos(&MortarInScene->Pos);
	}
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] == 1)
	{
		Fire->SetEnable(true);
		PinePitchMelted->SetEnable(true);
		CoalPowder->SetEnable(false);
		PinePitch->SetEnable(false);
		MortarInScene->SetEnable(false);
		MortarInHand->SetEnable(false);
		//Hud->InvBox_StopPuzzleMode();
		//IsPuzzleModeActive = false;
	}
	

}

void L6_S2_PZPineGum::InitControlPolys()
{
	for (int i = 0; i < MAX_L6S2_PZOBJECTS; i++)
	{
		PZPolyArray[i]		= NULL;
		DestPolyArray[i]	= NULL;
	}

	int ArrayPts0[] = {513,584,500,488,599,418,682,445,703,567,613,597};
	PZPolyArray[e_L6S2_Stone1] =  new CPolygon(6);
	PZPolyArray[e_L6S2_Stone1]->Init(ArrayPts0);
	int ArrayPts1[] = {216,212,290,225,357,190,318,91,259,83,212,139};
	PZPolyArray[e_L6S2_Stone2] =  new CPolygon(6);
	PZPolyArray[e_L6S2_Stone2]->Init(ArrayPts1);
	int ArrayPts2[] = {219,387,187,449,235,504,323,502,353,415,280,384};
	PZPolyArray[e_L6S2_Leaves1]	=  new CPolygon(6);
	PZPolyArray[e_L6S2_Leaves1]->Init(ArrayPts2);
	int ArrayPts3[] = {383,182,345,224,371,266,430,267,477,246,488,195};
	PZPolyArray[e_L6S2_Leaves2] =  new CPolygon(6);
	PZPolyArray[e_L6S2_Leaves2]->Init(ArrayPts3);
	int ArrayPts4[] = {435,511,372,521,338,555,363,615,437,631,472,592,476,550};
	PZPolyArray[e_L6S2_Leaves3] =  new CPolygon(7);
	PZPolyArray[e_L6S2_Leaves3]->Init(ArrayPts4);
	int ArrayPts6[] = {201,206,271,190,348,235,321,300,223,302,182,266};
	PZPolyArray[e_L6S2_PinePitchPan] =  new CPolygon(6);
	PZPolyArray[e_L6S2_PinePitchPan]->Init(ArrayPts6);
	int ArrayPts7[] = {456,353,484,330,539,322,582,342,589,381,523,416,469,407};
	PZPolyArray[e_L6S2_CharcoalMortar] =  new CPolygon(7);
	PZPolyArray[e_L6S2_CharcoalMortar]->Init(ArrayPts7);
	int ArrayPts8[] = {296,358,300,385,334,406,392,385,433,357,445,323,418,289,357,312};
	PZPolyArray[e_L6S2_Pestle] =  new CPolygon(8);
	PZPolyArray[e_L6S2_Pestle]->Init(ArrayPts8);


	int DestArrayPts0[] = {769,537,775,483,810,402,836,360,911,316,987,297,1038,307,1117,340,1163,386,1175,432,1175,474,1140,527,1077,547,1013,570,927,577,835,565};
	DestPolyArray[e_L6S2_Stone1]=  new CPolygon(16);
	DestPolyArray[e_L6S2_Stone1]->Init(DestArrayPts0);
	DestPolyArray[e_L6S2_Stone2]=  new CPolygon(16);
	DestPolyArray[e_L6S2_Stone2]->Init(DestArrayPts0);
	DestPolyArray[e_L6S2_Leaves1]=  new CPolygon(16);
	DestPolyArray[e_L6S2_Leaves1]->Init(DestArrayPts0);
	DestPolyArray[e_L6S2_Leaves2]=  new CPolygon(16);
	DestPolyArray[e_L6S2_Leaves2]->Init(DestArrayPts0);
	DestPolyArray[e_L6S2_Leaves3]=  new CPolygon(16);
	DestPolyArray[e_L6S2_Leaves3]->Init(DestArrayPts0);
	DestPolyArray[e_L6S2_PinePitchPan]=  new CPolygon(16);
	DestPolyArray[e_L6S2_PinePitchPan]->Init(DestArrayPts0);
	DestPolyArray[e_L6S2_CharcoalMortar]=  new CPolygon(16);
	DestPolyArray[e_L6S2_CharcoalMortar]->Init(DestArrayPts0);

	int StickArrayPts[] = {499,345,383,286,564,156,689,212};
	StickPoly=  new CPolygon(4);
	StickPoly->Init(StickArrayPts);

	int DestArrayPts1[] = {443,427,411,318,497,246,613,256,641,370,605,456};
	DestPolyArray[e_L6S2_Pestle]=  new CPolygon(6);
	DestPolyArray[e_L6S2_Pestle]->Init(DestArrayPts1);
	ObjectInHandID = -1;

	int PanArray[] = {367,169,425,176,496,159,509,112,450,69,382,69,349,102};
	PanPoly =  new CPolygon(7);
	PanPoly->Init(PanArray);


}


void L6_S2_PZPineGum::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken )
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_Charcoal] && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S2_Pestle] && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_PinePitch] && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced  )
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] == 0)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] == 0)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves1] == 0)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves2] == 0)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves3] == 0)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] == 0 && 
		ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] == 1 && 
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] == 1)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] == 0 && 
		ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced )
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] == 0 && 
		ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] == 1)
		isAnyTaskLeft = true;
	if(!ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected && 
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves1] &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves2] &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves3] &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] &&
		ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] )
		isAnyTaskLeft = true;
	
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_PZPineGum] = isAnyTaskLeft;

	if( ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected && ProfileSystem::Instance->ActiveProfile.L6_S4_IsVisited )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected && !ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied && ProfileSystem::Instance->ActiveProfile.L6_S4_IsVisited )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
}

void L6_S2_PZPineGum::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			
			if( SceneRect.Intersection(&dropPos) )
			{
				switch (itemVal )
				{
					case eInv_L6_S4_PinePitch:
						ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced  = true;
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
						Control::Audio->QuickLoadAndPlaySFX("s2pz1_placePinePitchPan");
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_PinePitch] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_PinePitch);
						PZScnObjects[e_L6S2_PinePitchPan]->SetEnable(true);
						UpdatePuzzleStates();
						return;
					case eInv_L6_S3_Charcoal:
						ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced  = true;
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
						Control::Audio->QuickLoadAndPlaySFX("s2pz1_placeCharcoal");
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_Charcoal] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S3_Charcoal);
						PZScnObjects[e_L6S2_CharcoalMortar]->SetEnable(true);
						UpdatePuzzleStates();
						return;
					case eInv_L6_S2_Pestle:
						ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced  = true;
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
						PZObjStates[e_L6S2_Pestle]		=  Obj_Inactive;
						Control::Audio->QuickLoadAndPlaySFX("s2pz1_placePestle");
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S2_Pestle] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S2_Pestle);
						PZScnObjects[e_L6S2_Pestle]->SetEnable(true);
						UpdatePuzzleStates();
						return;
					case eInv_L6_S1_Mortar: 
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
						Hud->ShowBannerText("L6yet");
						return;
					case eInv_L6_S1_MetalContainer:
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
						Hud->ShowBannerText("L6gut");
						return;
				}
			}
			
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		if(ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected)
		{
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
		}
	}
	TaskPendingUpdate();
}

void L6_S2_PZPineGum::ProcessSkip()
{
	//Hud->InvBox_StopPuzzleMode();
	Hud->ReleaseDrag();			
	Hud->TakeInventoryItem(eInv_L6_S2_PinegumStick);
	Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S1_Steel);
	Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_FlintStones); 
	ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Steel] = false;
	ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_FlintStones] = false;
	ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected = true;
}


void L6_S2_PZPineGum::ProcessNonHOHint()
{
	if( IsAnimPlaying )
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 16;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken )
			{
				InitHintTrailEmit(&PanPoly->center);
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_Charcoal] && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced)
			{
				Hud->ShowHintForInvItem(eInv_L6_S3_Charcoal, &SceneRect );
				return;
			}
			break;
		case 2: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S2_Pestle] && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced)
			{
				Hud->ShowHintForInvItem(eInv_L6_S2_Pestle, &SceneRect );
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_PinePitch] && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced  )
			{
				Hud->ShowHintForInvItem(eInv_L6_S4_PinePitch, &SceneRect );
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] == 0)
			{
				Hud->ShowHintForSceneItem(&DestPolyArray[e_L6S2_Stone1]->center);
				InitHintTrailEmit( PointSystem::CreateCPoint(PZScnObjects[e_L6S2_Stone1]->GetCenterX(),PZScnObjects[e_L6S2_Stone1]->GetCenterY(),0));
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] == 0)
			{
				Hud->ShowHintForSceneItem(&DestPolyArray[e_L6S2_Stone2]->center);
				InitHintTrailEmit( PointSystem::CreateCPoint(PZScnObjects[e_L6S2_Stone2]->GetCenterX(),PZScnObjects[e_L6S2_Stone2]->GetCenterY(),0));
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves1] == 0)
			{
				Hud->ShowHintForSceneItem(&DestPolyArray[e_L6S2_Leaves1]->center);
				InitHintTrailEmit( PointSystem::CreateCPoint(PZScnObjects[e_L6S2_Leaves1]->GetCenterX(),PZScnObjects[e_L6S2_Leaves1]->GetCenterY(),0));
				return;
			}
			break;
		case 7: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves2] == 0)
			{
				Hud->ShowHintForSceneItem(&DestPolyArray[e_L6S2_Leaves2]->center);
				InitHintTrailEmit( PointSystem::CreateCPoint(PZScnObjects[e_L6S2_Leaves2]->GetCenterX(),PZScnObjects[e_L6S2_Leaves2]->GetCenterY(),0));
				return;
			}
			break;
		case 8: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves3] == 0)
			{
				Hud->ShowHintForSceneItem(&DestPolyArray[e_L6S2_Leaves3]->center);
				InitHintTrailEmit( PointSystem::CreateCPoint(PZScnObjects[e_L6S2_Leaves3]->GetCenterX(),PZScnObjects[e_L6S2_Leaves3]->GetCenterY(),0));
				return;
			}
			break;
		case 9: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] == 0 && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] == 1 && 
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] == 1)
			{
				Hud->ShowHintForSceneItem(&DestPolyArray[e_L6S2_PinePitchPan]->center);
				InitHintTrailEmit( PointSystem::CreateCPoint(PZScnObjects[e_L6S2_PinePitchPan]->GetCenterX(),PZScnObjects[e_L6S2_PinePitchPan]->GetCenterY(),0));
				return;
			}
			break;
		case 10: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] == 0 && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced )
			{
				Hud->ShowHintForSceneItem(&DestPolyArray[e_L6S2_Pestle]->center);
				InitHintTrailEmit( PointSystem::CreateCPoint(PZScnObjects[e_L6S2_Pestle]->GetCenterX(),PZScnObjects[e_L6S2_Pestle]->GetCenterY(),0));
				return;
			}
			break;
		case 11: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] == 0 && ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] == 1 && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced )
			{
				Hud->ShowHintForSceneItem(&DestPolyArray[e_L6S2_CharcoalMortar]->center);
				InitHintTrailEmit( PointSystem::CreateCPoint(PZScnObjects[e_L6S2_CharcoalMortar]->GetCenterX(),PZScnObjects[e_L6S2_CharcoalMortar]->GetCenterY(),0));
				return;
			}
			break;
		case 12: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected && 
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves1] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves2] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves3] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] )
			{
				Hud->ShowHintForSceneItem(&DestPolyArray[e_L6S2_Stone1]->center);
				InitHintTrailEmit( &StickPoly->center);
				return;
			}
			break;
		case 13: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2] )
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 14: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 15: ++iNonHOHint;
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

void L6_S2_PZPineGum::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L6_S2_PZPineGum::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
			if( !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken )
			{
				sparklePointsVec.push_back(PanPoly->center);
				isSparkleUpdatePending = true;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] == 0)
			{
				sparklePointsVec.push_back(CPoint(PZScnObjects[e_L6S2_Stone1]->GetCenterX(), PZScnObjects[e_L6S2_Stone1]->GetCenterY(), 0));
				isSparkleUpdatePending = true;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] == 0)
			{
				sparklePointsVec.push_back(CPoint(PZScnObjects[e_L6S2_Stone2]->GetCenterX(), PZScnObjects[e_L6S2_Stone2]->GetCenterY(), 0));
				isSparkleUpdatePending = true;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves1] == 0)
			{
				sparklePointsVec.push_back(CPoint(PZScnObjects[e_L6S2_Leaves1]->GetCenterX(), PZScnObjects[e_L6S2_Leaves1]->GetCenterY(), 0));
				isSparkleUpdatePending = true;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves2] == 0)
			{
				sparklePointsVec.push_back(CPoint(PZScnObjects[e_L6S2_Leaves2]->GetCenterX(), PZScnObjects[e_L6S2_Leaves2]->GetCenterY(), 0));
				isSparkleUpdatePending = true;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves3] == 0)
			{
				sparklePointsVec.push_back(CPoint(PZScnObjects[e_L6S2_Leaves3]->GetCenterX(), PZScnObjects[e_L6S2_Leaves3]->GetCenterY(), 0));
				isSparkleUpdatePending = true;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] == 0 && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] == 1 && 
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] == 1)
			{
				sparklePointsVec.push_back(CPoint(PZScnObjects[e_L6S2_PinePitchPan]->GetCenterX(), PZScnObjects[e_L6S2_PinePitchPan]->GetCenterY(), 0));
				isSparkleUpdatePending = true;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] == 0 && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced )
			{
				sparklePointsVec.push_back(CPoint(PZScnObjects[e_L6S2_Pestle]->GetCenterX(), PZScnObjects[e_L6S2_Pestle]->GetCenterY(), 0));
				isSparkleUpdatePending = true;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] == 0 && ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] == 1 && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced )
			{
				sparklePointsVec.push_back(CPoint(PZScnObjects[e_L6S2_CharcoalMortar]->GetCenterX(), PZScnObjects[e_L6S2_CharcoalMortar]->GetCenterY(), 0));
				isSparkleUpdatePending = true;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected && 
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone1] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Stone2] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves1] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves2] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Leaves3] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_PinePitchPan] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_Pestle] &&
				ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[e_L6S2_CharcoalMortar] )
			{
				sparklePointsVec.push_back(CPoint(StickPoly->center.x, StickPoly->center.y, 0));
				isSparkleUpdatePending = true;
			}

}


L6_S2_PZPineGum::~L6_S2_PZPineGum()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	TaskPendingUpdate();
	//Hud->InvBox_StopPuzzleMode();

	for (int i = 0; i < MAX_L6S2_PZOBJECTS; i++)
	{
		SAFE_DELETE(PZPolyArray[i]); 
		SAFE_DELETE(DestPolyArray[i]);
	}

	SAFE_DELETE(ChartPoly);
	SAFE_DELETE(InstructionChart);
	SAFE_DELETE(FireMakingAnim);
	SAFE_DELETE(CharcoalCrushAnim);
	SAFE_DELETE(CharcoalAddAnim);
	SAFE_DELETE(PineGumMeltAnim);
	SAFE_DELETE(GlueMixAnim);
	SAFE_DELETE(FireSparks);
	SAFE_DELETE(VapourFX);
		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL6s2PineGumPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL6s2PineGumPuzzleAmb);
	//MUSIC_SFX
}
void L6_S2_PZPineGum::NavigateToScene()
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