//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE3 : FLOATING ISLAND_3
//====================================

#include "L6_S3_FloatingIsland_3.h"
#include "Util.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);
const CRectangle  HORect(685.0f,125.0f,284.0f,169.0f);
const CRectangle  PuzRect(373.0f,237.0f,116.0f,89.0f);


enum eL6S1HOPopEnums
{
	eL6S3Pop_Inactive = -1,
	eL6S3Pop1_Charcoal,
	eL6S3Pop2_Fire
};

L6_S3_FloatingIsland_3::L6_S3_FloatingIsland_3()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S3\\S3.SCN");
	HOScene->SetZOff(10);
	HOScene->GetObjectByID("Bg")->SetZ(0);

	int Pop1ArrayPts[] = {955,376,892,448,967,543,1095,513,1122,428,996,385,953,373};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL6S3Pop1_Charcoal, 7, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L6S3_Pop1Charcoal *pop1 = new  L6S3_Pop1Charcoal(eL6S3Pop1_Charcoal, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S3_FloatingIsland_3::OnPopupClosed, this));
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);
	popClickPoly = NULL;

	int Pop2ArrayPts[] = {198,172,127,257,169,399,240,431,346,394,361,328,302,189,198,174};
	popClickPoly = new ClickablePolygon(eL6S3Pop2_Fire, 8, Pop2ArrayPts);
	PopRectArray.push_back(popClickPoly);
	pop2 = new  L6S3_Pop2Fire(eL6S3Pop2_Fire, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S3_FloatingIsland_3::OnPopupClosed, this));
	pop2->SetPopupBoundingBox(PopCircleRect);
	PopupArray.push_back(pop2);
	popClickPoly = NULL;

	/*isVideoPlaying = false;
	lpVidLasyaCutscene = NULL;

	CRectangle RectVidFull(171, 96, 1024, 576);
	lpVidLasyaCutscene = new CVideo("OGV\\L2\\S6WOODCUTSCN.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	lpVidLasyaCutscene->SetScale(1.334f);*/

	Janim_pathDestroy = NULL;
	Janim_pathDestroy = new JSONAnimator(0);
	Janim_pathDestroy->parseAndLoadJSON("ANIM\\L6\\S3_PATH_DESTROY.JSON",HOScene);

	Janim_pathBuild = NULL;
	Janim_pathBuild = new JSONAnimator(1,CC_CALLBACK_1(L6_S3_FloatingIsland_3::OnAnimComplete,this));
	Janim_pathBuild->parseAndLoadJSON("ANIM\\L6\\S3_PATH_BUILD.JSON",HOScene);

	HOScene->GetObjectByID("Mask")->SetZ(2);

	S3UnderFogFX = new SuperFX("PRT\\L6\\S3UnderFog.INI", PointSystem::CreateCPoint(524,403,0), 1);
	S3UnderFogFX->Start();
	S3UnderFogFX->AdvanceByTime(2.0f);

	S3GoldGlowFX = new SuperFX("PRT\\L6\\S3GoldGlow.INI", PointSystem::CreateCPoint(902,178,0), 12); 
	S3GoldGlowFX->Start();
	S3GoldGlowFX->AdvanceByTime(2.0f);

	S3SmokeFX = new SuperFX("PRT\\L6\\S3_CharcoalSmoke.INI", PointSystem::CreateCPoint(1010,420,0), HOScene->GetObjectByID("coal")->ZOrder + 1); 

	ActivePopUpId = eL6S3Pop_Inactive;

#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(431.0f,77.0f,114.0f,92.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

	InitParticles();

	SceneObjectsUpdate();


	if( ProfileSystem::Instance->ActiveProfile.L6_S3_JamesDialogsShown  )
	{
		ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened = true;
		HOScene->GetObjectByID("James")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S3_LasyaCutscenePlayed && !ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayDestructed)
	{
		HOScene->GetObjectByID("CenterPieceBridge")->SetEnable(false);
		Hud->ShowBannerText("L6pest");
		Control::Audio->PlaySample(aVOL6MaryRefS3_1);
		ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayDestructed = true;
	}

	//MUSIC_SFX
	Control::Audio->LoadSample(aVOL6MaryRefS3_1,	AudioVO);
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s3FloatingIsland3Amb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s3FloatingIsland3Amb) )		
	{
		Control::Audio->PlaySample(aAmbL6s3FloatingIsland3Amb,true);
	}

	//MUSIC_SFX
	TaskPendingUpdate();
}

bool L6_S3_FloatingIsland_3::isPopupActive(int popId)
{
	switch (popId)
	{
	case eL6S3Pop2_Fire:if (!ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened)	return true;	break;
	case eL6S3Pop1_Charcoal:if ((!ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S3PaintBrush]) && ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived) 	return true;	break;
	default:
		break;
	}

	return false;
}


void L6_S3_FloatingIsland_3::InitPopups()
{

}

void L6_S3_FloatingIsland_3::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopUpId = eL6S3Pop_Inactive;

	switch (popup->GetPopupId())
	{
	case 0:
		break;
	case 1:
		break;
	}
}

void L6_S3_FloatingIsland_3::OnAnimComplete(int animID)
{
	Hud->ShowBannerText("L6yes");
}


void L6_S3_FloatingIsland_3::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened)
	{
		HOScene->GetObjectByID("DarkBg")->SetEnable(false);
		/*for (int i = 1; i < 4; i++)
		{
		string _id = "Fire" + MKSTR(i);
		HOScene->GetObjectByID(_id)->SetEnable(true);
		}*/
		for (int i = 1; i < 4; i++)
		{
			string _id = "lava0" + MKSTR(i);
			HOScene->GetObjectByID(_id)->SetEnable(true);
		}
		HOScene->GetObjectByID("Glow")->SetEnable(true);
	}
	else
	{
		/*for (int i = 1; i < 4; i++)
		{
		string _id = "Fire" + MKSTR(i);
		HOScene->GetObjectByID(_id)->SetEnable(false);
		}*/
		for (int i = 1; i < 4; i++)
		{
			string _id = "lava0" + MKSTR(i);
			HOScene->GetObjectByID(_id)->SetEnable(false);
		}
		HOScene->GetObjectByID("Glow")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken)
	{
		HOScene->GetObjectByID("coal")->SetEnable(false);
		if (S3SmokeFX->Active)
		{
			S3SmokeFX->StopImmediate();
		}
	}
	else
	{
		S3SmokeFX->Start();
		S3SmokeFX->AdvanceByTime(2.0f);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayDestructed && !ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived)
	{
		HOScene->GetObjectByID("CenterPieceBridge")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived)
	{
		HOScene->GetObjectByID("CenterPieceBridge")->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayDestructed)
	{
		HOScene->GetObjectByID("James")->SetEnable(false);
	}
}

void L6_S3_FloatingIsland_3::InitParticles()
{
	HOScene->GetObjectByID("ForgroundRoot01")->Animate(3,1,0,0,3,2);
	HOScene->GetObjectByID("ForgroundRoot02")->Animate(3,1,0,0,3,2);

	HOScene->GetObjectByID("HangingChain01")->SetPivot(0,-79);
	HOScene->GetObjectByID("HangingChain01")->Animate(0,0,6,0,5,3);
	HOScene->GetObjectByID("HangingChain02")->SetPivot(0,-53);
	HOScene->GetObjectByID("HangingChain02")->Animate(0,0,4,0,4,3);
	HOScene->GetObjectByID("HangingChain03")->SetPivot(0,-70);
	HOScene->GetObjectByID("HangingChain03")->Animate(0,0,3,0,4,3);

	//HOScene->GetObjectByID("lava01")->Animate(0.9f,1,3,2);
	for (int i = 1; i < 4; i++)
	{
		string _id = "lava0" + MKSTR(i);
		HOScene->GetObjectByID(_id)->Animate(0.85f,1,3,2);
	}
}

L6_S3_FloatingIsland_3::~L6_S3_FloatingIsland_3()
{
	TaskPendingUpdate();
	SAFE_DELETE(S3GoldGlowFX);
	SAFE_DELETE(S3UnderFogFX);
	SAFE_DELETE(S3SmokeFX);
	SAFE_DELETE(Janim_pathBuild);
	SAFE_DELETE(Janim_pathDestroy);
	for (int i = 0; i < (int)PopupArray.size(); i++)
	{
		SAFE_DELETE(PopupArray.at(i));
	}
	PopupArray.clear();
	for (int i = 0; i < (int)PopRectArray.size(); i++)
	{
		SAFE_DELETE(PopRectArray.at(i));
	}
	PopRectArray.clear();

	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL6MaryRefS3_1);
	Control::Audio->StopSample(aAmbL6s3FloatingIsland3Amb);
	Control::Audio->UnloadSample(aAmbL6s3FloatingIsland3Amb);
	//MUSIC_SFX
}

void L6_S3_FloatingIsland_3::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(pop2->isVideoPlaying)
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

void L6_S3_FloatingIsland_3::Update()
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
	if( Hud->PopupState != Popup_Inactive )
		return;
#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	if (ActivePopUpId != eL6S3Pop_Inactive)
		return;

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	HiddenObjectSystem::HOUpdate();

	CPoint pos = Control::Input->Mpos();

	for (int i = 0; i < (int)(PopupArray.size()); i++)
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
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
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
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	}

	
	if (!ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened && Control::Input->LBclicked())
	{
		Hud->ShowBannerText("L6little");
	}
	//Till James dialog completes we need to show him again!!!
	else if (ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened && !ProfileSystem::Instance->ActiveProfile.L6_S3_JamesDialogsShown && Control::Input->LBclicked())
	{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S3_FakeJames, 0, CRectangle(875, 150, 100, 100));
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FakeJames;

		SendMessage(ControlExit, ID, HOSM_NewArea);
		//Hud->ShowBannerText("James? How did you reach here?"); //Sac: where is the external text for this ?
#endif
		
		
	}	
	else if (!(ProfileSystem::Instance->ActiveProfile.L6_S3_LasyaCutscenePlayed && !ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayDestructed))
	{
		if (ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayDestructed && !ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted && PuzRect.Intersection(&pos))
		{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S3_PZPathway, 0, PuzRect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_PZPathway;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}
		}
		else if (ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted && !ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived)
		{
			//Show Objective completed
			if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O2_FindWayOutToConnectBridge] && 
				!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L6O2_FindWayOutToConnectBridge] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L6O2_FindWayOutToConnectBridge] = true;
				Hud->ShowObjectiveFX(eObj_L6O2_FindWayOutToConnectBridge,true);
			}

			Janim_pathBuild->PlayAnim();
			ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived = true;
		}
		else if (ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived && HORect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken)
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S3_GlowingHeartHO, 0, HORect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_GlowingHeartHO;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}
		}
	}
}

void L6_S3_FloatingIsland_3::Render(const CPoint* pCam)
{

}


void L6_S3_FloatingIsland_3::TakeInventoryItem(EHOInventory ItemID)
{

}


void L6_S3_FloatingIsland_3::ReceiveMessage(int val)
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
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			if (ActivePopUpId != ePopInactive)
			{
				PopupArray[ActivePopUpId]->HandleInventoryDrop(dropPos, itemVal);
				return;
			}
			else if (itemVal == eInv_L6_S4_ColouredPowder)
			{
				Hud->ShowBannerText("L6guess");
				Hud->CancelDrag();
			}
			else if (itemVal == eInv_L6_S1_Steel)
			{
				if (PopRectArray[eL6S3Pop2_Fire]->Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced)
				{
					PopupArray[eL6S3Pop2_Fire]->OpenPopup();
					ActivePopUpId = eL6S3Pop2_Fire;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if (itemVal == eInv_L6_S4_FlintStones)
			{
				if (PopRectArray[eL6S3Pop2_Fire]->Intersection(&dropPos) && ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced && !ProfileSystem::Instance->ActiveProfile.L6_S3_FlintStoneUsed)
				{
					PopupArray[eL6S3Pop2_Fire]->OpenPopup();
					ActivePopUpId = eL6S3Pop2_Fire;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else
			{
				//If no drop items in this scene !!
				GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		


	}	
	TaskPendingUpdate();
}

float L6_S3_FloatingIsland_3::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L6_S3_FloatingIsland_3::TaskPendingUpdate()
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

	if (!ProfileSystem::Instance->ActiveProfile.L6_S3_JamesDialogsShown && ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened)
		isAnyTaskLeft = true;
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayDestructed && !ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted)
		isAnyTaskLeft = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived  && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S3_FloatingIsland3] = isAnyTaskLeft;

	if(ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvMortarWithCharcoalPlaced )
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_PZPineGum] = true;
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2] = true;
	}

	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced && ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_PortalChest] = true;
	}
}

void L6_S3_FloatingIsland_3::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected)
		{
			InitHintTrailEmit(500, 150);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S3PaintBrush])
		{
			if (ActivePopUpId != ePopInactive)
				InitHintTrailEmit(515, 140);
			else
				InitHintTrailEmit(1000, 450);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
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
			if(!ProfileSystem::Instance->ActiveProfile.L6_S3_JamesDialogsShown && ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened)
			{
				//InitHintTrailEmit(&HOScene->GetObjectByID("James")->GetWorldRect(),false);
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle jamesObjRect =HOScene->GetObjectByID("James")->GetWorldRect();
				InitHintTrailEmit(&jamesObjRect,false);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayDestructed && !ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted)
			{
				InitHintTrailEmit(&PuzRect,false);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived  && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken)
			{
				InitHintTrailEmit(&HORect,false);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2] )
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
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

void L6_S3_FloatingIsland_3::SparkleUpdate()
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

void L6_S3_FloatingIsland_3::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < (int)PopupArray.size(); i++)
	{
		Popup * popup = PopupArray[i];
		if (popup)
		{
			if(isPopupActive(i)) 
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

	if(!ProfileSystem::Instance->ActiveProfile.L6_S3_JamesDialogsShown && ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened)
	{
		sparklePointsVec.push_back(CPoint(HOScene->GetObjectByID("James")->GetWorldRect().GetCenterX(), HOScene->GetObjectByID("James")->GetWorldRect().GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}

	if(ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayDestructed && !ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted)
	{
		sparklePointsVec.push_back(CPoint(PuzRect.GetCenterX(), PuzRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if(ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived  && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_InvHeartTaken)
	{
		sparklePointsVec.push_back(CPoint(HORect.GetCenterX(), HORect.GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}
}

void L6_S3_FloatingIsland_3::NavigateToScene()
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