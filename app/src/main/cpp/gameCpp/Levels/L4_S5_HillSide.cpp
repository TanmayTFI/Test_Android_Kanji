//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL4 - SCENE2 : RIVER SIDE
//====================================

#include "L4_S5_HillSide.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);

const CRectangle  PZSwapTileRect(815.0f,211.0f,81.0f,51.0f);

enum eL4S5HOPopEnums
{
	eL4S5Pop_Inactive = -1,
	eL4S5Pop1_Door,
	eL4S5Pop2_OliveTree,
	eL4S5Pop3_Skeleton,
	eL4S5Pop4_NetSquare,
	eL4S5Pop5_Bridge,
	eL4S5Pop_Max
};

L4_S5_HillSide::L4_S5_HillSide()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S5\\S5.SCN");

	int Pop1ArrayPts[] = {331,228,349,206,382,220,394,258,332,265,329,228};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL4S5Pop1_Door, 6, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L4S5_Pop1Door *pop1 = new  L4S5_Pop1Door(eL4S5Pop1_Door, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S5_HillSide::OnPopupClosed, this));
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);
	popClickPoly = NULL;

	int Pop2ArrayPts[] = {676,150,635,171,633,208,675,216,700,203,718,191,791,210,818,184,803,142,752,151,674,149};
	popClickPoly = new ClickablePolygon(eL4S5Pop2_OliveTree, 11, Pop2ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L4S5_Pop2OliveTree *pop2 = new  L4S5_Pop2OliveTree(eL4S5Pop2_OliveTree, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S5_HillSide::OnPopupClosed, this));
	pop2->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop2);
	popClickPoly = NULL;

	int Pop3ArrayPts[] = {881,427,897,389,951,375,982,434,942,479,898,467,876,427};
	popClickPoly = new ClickablePolygon(eL4S5Pop3_Skeleton, 7, Pop3ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L4S5_Pop3Skeleton *pop3 = new  L4S5_Pop3Skeleton(eL4S5Pop3_Skeleton, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S5_HillSide::OnPopupClosed, this));
	pop3->SetPopupBoundingBox(PopCircleRect);
	PopupArray.push_back(pop3);
	popClickPoly = NULL;

	int Pop4ArrayPts[] = {419,176,430,258,448,270,494,249,476,165,416,175};
	popClickPoly = new ClickablePolygon(eL4S5Pop4_NetSquare, 6, Pop4ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L4S5_Pop4NetSquare *pop4 = new  L4S5_Pop4NetSquare(eL4S5Pop4_NetSquare, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S5_HillSide::OnPopupClosed, this));
	pop4->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop4);
	popClickPoly = NULL;

	int Pop5ArrayPts[] = {394,742,458,547,686,495,919,553,922,741};
	popClickPoly = new ClickablePolygon(eL4S5Pop5_Bridge, 5, Pop5ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L4S5_Pop5Bridge *pop5 = new  L4S5_Pop5Bridge(eL4S5Pop5_Bridge, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S5_HillSide::OnPopupClosed, this));
	pop5->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop5);
	popClickPoly = NULL;
	SAFE_DELETE(popClickPoly);

	ActivePopUpId = eL4S5Pop_Inactive;

	InitSceneElements();
	InitAnimations();

	//morph Object
#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S5Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("MorphTarget"), HOScene->GetObjectByID("MorphBase"));
		CRectangle  MorphRect(413.0f,116.0f,61.0f,51.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("MorphTarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("MorphTarget")->SetEnable(false);
#endif 

	SceneObjectsUpdate();

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s5HillSideAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s5HillSideAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s5HillSideAmb,true);
	}

	//MUSIC_SFX
}

void L4_S5_HillSide::InitSceneElements()
{
	Door			    = HOScene->GetObjectByID("Door");
	Cloud01			    = HOScene->GetObjectByID("Cloud01");
	clouds02		    = HOScene->GetObjectByID("clouds02");
	clouds03		    = HOScene->GetObjectByID("clouds03");
	FlintStone		    = HOScene->GetObjectByID("FlintStone");
	MagicMushroom	    = HOScene->GetObjectByID("MagicMushroom");
	MagicPlant01	    = HOScene->GetObjectByID("MagicPlant01");
	MagicPlant02	    = HOScene->GetObjectByID("MagicPlant02");
	Morph01			    = HOScene->GetObjectByID("Morph01");
	Morph02			    = HOScene->GetObjectByID("Morph02");
	NetCut			    = HOScene->GetObjectByID("NetCut");
	Owl				    = HOScene->GetObjectByID("Owl");
	Plant01			    = HOScene->GetObjectByID("Plant01");
	Plant02			    = HOScene->GetObjectByID("Plant02");
	Plant03			    = HOScene->GetObjectByID("Plant03");
	Plant04			    = HOScene->GetObjectByID("Plant04");
	PlantAnim		    = HOScene->GetObjectByID("PlantAnim");
	PlantYellow01	    = HOScene->GetObjectByID("PlantYellow01");
	PlantYellow02	    = HOScene->GetObjectByID("PlantYellow02");
	ScaryFace1		    = HOScene->GetObjectByID("ScaryFace1");
	ScaryFace2		    = HOScene->GetObjectByID("ScaryFace2");
	ScaryFace3			= HOScene->GetObjectByID("ScaryFace3");
	ScaryFace4			= HOScene->GetObjectByID("ScaryFace4");
	Scroll				= HOScene->GetObjectByID("Scroll");
	SolvedSate			= HOScene->GetObjectByID("SolvedSate");
	TreeBranch			= HOScene->GetObjectByID("TreeBranch");
	TreeGlow			= HOScene->GetObjectByID("TreeGlow");
	VioletGlow			= HOScene->GetObjectByID("VioletGlow");
}	

void L4_S5_HillSide::InitAnimations()
{
	PlantYellow01->SetBlendMode(eBlend_AlphaAdditive);
	PlantYellow01->Animate(0, 0.8f, 9.0f);
	PlantYellow02->SetBlendMode(eBlend_AlphaAdditive);
	PlantYellow02->Animate(0, 0.9f, 8.0f);
	MagicPlant01->SetBlendMode(eBlend_AlphaAdditive);
	MagicPlant01->Animate(0, 0.7f, 6.0f);
	MagicPlant02->SetBlendMode(eBlend_AlphaAdditive);
	MagicPlant02->Animate(0, 0.7f, 6.0f);

	Plant01->SetBlendMode(eBlend_AlphaAdditive);
	Plant02->SetBlendMode(eBlend_AlphaAdditive);
	Plant03->SetBlendMode(eBlend_AlphaAdditive);
	Plant04->SetBlendMode(eBlend_AlphaAdditive);

	Plant01->Animate(0, 0.8f, 9.0f);
	Plant02->Animate(0, 0.8f, 9.0f);
	Plant03->Animate(0, 0.8f, 9.0f);
	Plant04->Animate(0, 0.8f, 9.0f);

	ScaryFace1->SetBlendMode(eBlend_AlphaAdditive);
	ScaryFace2->SetBlendMode(eBlend_AlphaAdditive);
	ScaryFace3->SetBlendMode(eBlend_AlphaAdditive);
	ScaryFace4->SetBlendMode(eBlend_AlphaAdditive);

	ScaryFace1->Animate(0, 0.8f, 9.0f);
	ScaryFace2->Animate(0, 0.8f, 9.0f);
	ScaryFace3->Animate(0, 0.8f, 9.0f);
	ScaryFace4->Animate(0, 0.8f, 9.0f);



	S5GlowPlant01FX = new SuperFX("PRT\\L4\\S5GlowPlant01.INI", PointSystem::CreateCPoint(443,484,0), 1100); 
	S5GlowPlant01FX->Start();
	S5GlowPlant01FX->AdvanceByTime(2.0f);

	S5GlowPlant02FX = new SuperFX("PRT\\L4\\S5GlowPlant02.INI", PointSystem::CreateCPoint(498,480,0), 1100); 
	S5GlowPlant02FX->Start();
	S5GlowPlant02FX->AdvanceByTime(2.0f);

	S5GlowPlant03FX = new SuperFX("PRT\\L4\\S5GlowPlant03.INI", PointSystem::CreateCPoint(659,362,0), 1100); 
	S5GlowPlant03FX->Start();
	S5GlowPlant03FX->AdvanceByTime(2.0f);

	S5GlowPlant04FX = new SuperFX("PRT\\L4\\S5GlowPlant04.INI", PointSystem::CreateCPoint(363,281,0), 1100); 
	S5GlowPlant04FX->Start();
	S5GlowPlant04FX->AdvanceByTime(2.0f);

	S5GlowPlant05FX = new SuperFX("PRT\\L4\\S5GlowPlant05.INI", PointSystem::CreateCPoint(983,290,0), 1100); 
	S5GlowPlant05FX->Start();
	S5GlowPlant05FX->AdvanceByTime(2.0f);

	S5GlowPlant06FX = new SuperFX("PRT\\L4\\S5GlowPlant06.INI", PointSystem::CreateCPoint(143,551,0), 1100); 
	S5GlowPlant06FX->Start();
	S5GlowPlant06FX->AdvanceByTime(2.0f);

}

void L4_S5_HillSide::InitPopups()
{

}

void L4_S5_HillSide::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopUpId = eL4S5Pop_Inactive;

	S5GlowPlant01FX->Start();
	S5GlowPlant02FX->Start();
	S5GlowPlant03FX->Start();
	S5GlowPlant04FX->Start();
	S5GlowPlant05FX->Start();
	S5GlowPlant06FX->Start();

	switch (popup->GetPopupId())
	{
	case 0:
		break;
	case 1:
		break;
	}
}

L4_S5_HillSide::~L4_S5_HillSide()
{
	for (int i = 0; i < eL4S5Pop_Max; i++)
	{
		SAFE_DELETE(PopupArray.at(i));
	}
	PopupArray.clear();

	SAFE_DELETE(S5GlowPlant01FX);
	SAFE_DELETE(S5GlowPlant02FX);
	SAFE_DELETE(S5GlowPlant03FX);
	SAFE_DELETE(S5GlowPlant04FX);
	SAFE_DELETE(S5GlowPlant05FX);
	SAFE_DELETE(S5GlowPlant06FX);

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL4s5HillSideAmb);
	Control::Audio->UnloadSample(aAmbL4s5HillSideAmb);
	//MUSIC_SFX
}

void L4_S5_HillSide::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
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

void L4_S5_HillSide::Update()
{
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif

	NavigateToScene();
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if (ActivePopUpId != eL4S5Pop_Inactive)
		return;


#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	CPoint pos = Control::Input->Mpos();

	if (!ProfileSystem::Instance->ActiveProfile.L4_S5_Visited)
	{
		/*Hud->ShowBannerText("That was a close shave.");*/ //Sac: where is the external text for this ?
		ProfileSystem::Instance->ActiveProfile.L4_S5_Visited = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L4_S1_SwampEntrance] = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L4_S2_RiverSide] = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L4_S4_HillTop] = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L4_S3_Cave] = true;
	}

	for (int i = 0; i < eL4S5Pop_Max-1; i++)
	{
		if( PopRectArray[i]->Intersection(&pos))
		{
			if( CheckIfPopupActive(i))
			{
				if( i != eL4S5Pop5_Bridge )
					Cursor::SetMode(CA_Look);
				else 
					Cursor::SetMode(CA_ExitDown);
				#ifdef ENABLE_ANDROID_BACK_BUTTON
					if (Control::Input->isKeyBackSensed())
					{
						Cursor::SetMode(CA_Normal);
						StopParticleEmission();
						ActivePopUpId = i;
						PopupArray[i]->OpenPopup();
						return;
					}	
				#endif	
				if (Control::Input->LBclicked())
				{
					Cursor::SetMode(CA_Normal);
					StopParticleEmission();
					ActivePopUpId = i;
					PopupArray[i]->OpenPopup();
					return;
				}
			}
		} 
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			if( !ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced )
			{
					Cursor::SetMode(CA_Normal);
					StopParticleEmission();
					ActivePopUpId = eL4S5Pop5_Bridge;
					PopupArray[eL4S5Pop5_Bridge]->OpenPopup();
				return; 
			}
			Cursor::SetMode(CA_Normal);
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S4_HillTop;
			SendMessage(ControlExit, ID, HOSM_NewArea);			
		}	
	#endif	

	if( IsMouseOverGoBackArea(&pos) )
	{
		if( !ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced )
		{
			Cursor::SetMode(CA_ExitDown,eArea_L4_S4_HillTop);
			if (Control::Input->LBclicked())
			{
				Cursor::SetMode(CA_Normal);
				StopParticleEmission();
				ActivePopUpId = eL4S5Pop5_Bridge;
				PopupArray[eL4S5Pop5_Bridge]->OpenPopup();
			}
			return; 
		}

		
		#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L4_S4_HillTop);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L4_S4_HillTop);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S4_HillTop, 2);
			}
#else			
			Cursor::SetMode(CA_Normal);
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S4_HillTop;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	}
	else if( PZSwapTileRect.Intersection(&pos) && 
		(!ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken ||  !ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken))
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
				TaskPendingUpdate();
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S5_PZSwapTile, 0, PZSwapTileRect);
			}
#else			
			Cursor::SetMode(CA_Normal);
			TaskPendingUpdate();
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S5_PZSwapTile;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}			
	}
}

void L4_S5_HillSide::StopParticleEmission()
{
	S5GlowPlant01FX->StopImmediate();
	S5GlowPlant02FX->StopImmediate();
	S5GlowPlant03FX->StopImmediate();
	S5GlowPlant04FX->StopImmediate();
	S5GlowPlant05FX->StopImmediate();
	S5GlowPlant06FX->StopImmediate();
}

void L4_S5_HillSide::SparkleUpdate()
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

void L4_S5_HillSide::CheckForPendingTask()
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

	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_SwapPuzzleCompleted )	
	{
		sparklePointsVec.push_back(CPoint(PZSwapTileRect.GetCenterX(), PZSwapTileRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken || !ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken )
	{
		sparklePointsVec.push_back(CPoint(PZSwapTileRect.GetCenterX(), PZSwapTileRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
}

bool L4_S5_HillSide::CheckIfPopupActive(int id)
{
	bool flag =  true;
	switch (id)
	{
	case eL4S5Pop1_Door:		if( ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken && ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken)
									flag = false;
		break;
	case eL4S5Pop2_OliveTree:	if( ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected )
									flag = false;
		break;
	case eL4S5Pop3_Skeleton:	if( ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected)
								{
									flag = false;
#ifdef _CEBUILD
									if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S5StarLocket])
										flag = true;
#endif
								}
								break;
	case eL4S5Pop4_NetSquare:	if( ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken)
									flag = false;
		break;
	case eL4S5Pop5_Bridge:		if( ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced )
									flag = false;
		break;
	default:
		break;
	}
	return (flag);
}


void L4_S5_HillSide::Render(const CPoint* pCam)
{

}


void L4_S5_HillSide::TakeInventoryItem(EHOInventory ItemID)
{

}

void L4_S5_HillSide::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected)
		Scroll->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken)
		NetCut->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken)
		Door->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved)
		HOScene->GetObjectByID("Sofia")->SetEnable(false);
}


void L4_S5_HillSide::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{

		if( !ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced )
			Control::Audio->PlaySample(aSFXInvWrongDrop);
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
			if(ActivePopUpId != ePopupState_Inactive)
			{
				PopupArray[ActivePopUpId]->HandleInventoryDrop(dropPos, itemVal);
			}
			else
				//If no drop items in this scene !!
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();
		if(ActivePopUpId != eL4S5Pop_Inactive)
		{
			if(PopupArray[ActivePopUpId]!=NULL)
				PopupArray[ActivePopUpId]->UpdateBgGraphic();
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
	TaskPendingUpdate();
}

float L4_S5_HillSide::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L4_S5_HillSide::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S5Collected)
		{
			InitHintTrailEmit(440, 145);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S5StarLocket])
		{
			if (ActivePopUpId != ePopInactive)
				InitHintTrailEmit(560, 260);
			else
				InitHintTrailEmit(920, 430);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	if( ActivePopUpId != ePopupState_Inactive )
	{
		switch (ActivePopUpId)
		{
		case eL4S5Pop1_Door:PopupArray[eL4S5Pop1_Door]->ProcessHint();
			break;
		case eL4S5Pop2_OliveTree:PopupArray[eL4S5Pop2_OliveTree]->ProcessHint();
			break;
		case eL4S5Pop3_Skeleton:PopupArray[eL4S5Pop3_Skeleton]->ProcessHint();
			break;
		case eL4S5Pop4_NetSquare:PopupArray[eL4S5Pop4_NetSquare]->ProcessHint();
			break;
		case eL4S5Pop5_Bridge:PopupArray[eL4S5Pop5_Bridge]->ProcessHint();
			break;
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S5_SwapPuzzleCompleted )	
			{
				Hud->InitHintTrailEmit(PZSwapTileRect.GetCenterX(), PZSwapTileRect.GetCenterY());
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken || !ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken )
			{
				Hud->InitHintTrailEmit(PZSwapTileRect.GetCenterX(), PZSwapTileRect.GetCenterY()); 
				return;
			}
			break;
		case 2: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected )
			{
				Hud->InitHintTrailEmit(PopRectArray[eL4S5Pop3_Skeleton]->center.x, PopRectArray[eL4S5Pop3_Skeleton]->center.y); 
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] )
			{
				Hud->InitHintTrailEmit(PopRectArray[eL4S5Pop4_NetSquare]->center.x, PopRectArray[eL4S5Pop4_NetSquare]->center.y); 
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if( (!ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife]) || !ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken)
			{
				Hud->InitHintTrailEmit(PopRectArray[eL4S5Pop1_Door]->center.x, PopRectArray[eL4S5Pop1_Door]->center.y); 
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] )
			{
				Hud->InitHintTrailEmit(PopRectArray[eL4S5Pop2_OliveTree]->center.x, PopRectArray[eL4S5Pop2_OliveTree]->center.y); 
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] && ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken )
			{
				InitHintTrailEmit(HoveredBackBtnRectMax.GetCenterX(), HoveredBackBtnRectMax.GetCenterY(),true,CA_ExitDown); 
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
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L4_S5_HillSide::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_SwapPuzzleCompleted )
		isAnyTaskLeft = true;
	if(!ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken)
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife])
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife])
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife])
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected )
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S5_HillSide] = isAnyTaskLeft;


	if( ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced && !ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;

	if( ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected && !ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;

	if( ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken && ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken && 
		ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced  && ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S3_Cave] = true;

	if(ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken && !ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide] = true;

	if(ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken && !ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide] = true;
}
void L4_S5_HillSide::NavigateToScene()
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