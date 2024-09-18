//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE1 : FLOATING ISLAND_1
//====================================

#include "L6_S1_FloatingIsland_1.h"
#include "MeshObject.h"


const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  SceneRectFloatindIsland_2(987.0f,247.0f,162.0f,113.0f);
const CRectangle  DescriptionHORect(905.0f,405.0f,177.0f,162.0f);

const CRectangle  FloatingRect1(4.0f,2.0f,138.0f,116.0f);
const CRectangle  FloatingRect2(423.0f,64.0f,55.0f,79.0f);
const CRectangle  FloatingRect3(532.0f,5.0f,43.0f,46.0f);
const CRectangle  FloatingRect4(493.0f,198.0f,53.0f,77.0f);
const CRectangle  FloatingRect5(818.0f,202.0f,80.0f,100.0f);
const CRectangle  FloatingRect6(887.0f,47.0f,287.0f,99.0f);
const CRectangle  FloatingRect7(960.0f,159.0f,226.0f,125.0f);

enum eL6S1HOPopEnums
{
	eL6S1Pop_Inactive = -1,
	eL6S1Pop1_Water,
	eL6S1Pop2_Statue_Chest,
	eL6S1Pop3_Statue_Hand,
	eL6S1Pop4_Statue_Sandal
};

L6_S1_FloatingIsland_1::L6_S1_FloatingIsland_1()
{
	
	LoadHud(INVENTORY_V1);
	AwardManager::GetInstance()->CollectAward(kAward_Visit_4_World);
	LoadScene("LV6\\S1\\S1.SCN");


	

	int Pop1ArrayPts[] = {47,482,83,518,71,547,9,557,16,593,185,573,252,567,327,517,196,497,49,480};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL6S1Pop1_Water, 10, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);
	pop1 = new  L6S1_Pop1Water(eL6S1Pop1_Water, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S1_FloatingIsland_1::OnPopupClosed, this));
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);
	popClickPoly = NULL;

	int Pop2ArrayPts[] = {595,114,606,177,693,177,705,120,647,101};
	popClickPoly = new ClickablePolygon(eL6S1Pop2_Statue_Chest, 5, Pop2ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L6S1_Pop2StatueChest *pop2 = new  L6S1_Pop2StatueChest(eL6S1Pop2_Statue_Chest, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S1_FloatingIsland_1::OnPopupClosed, this));
	pop2->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop2);
	popClickPoly = NULL;

	int Pop3ArrayPts[] = {749,178,748,202,826,228,854,172,806,126,748,175};
	popClickPoly = new ClickablePolygon(eL6S1Pop3_Statue_Hand, 6, Pop3ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L6S1_Pop3StatueHand *pop3 = new  L6S1_Pop3StatueHand(eL6S1Pop3_Statue_Hand, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S1_FloatingIsland_1::OnPopupClosed, this));
	pop3->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop3);
	popClickPoly = NULL;

	int Pop4ArrayPts[] = {569,483,647,470,669,528,589,547,566,485};
	popClickPoly = new ClickablePolygon(eL6S1Pop4_Statue_Sandal, 5, Pop4ArrayPts);
	PopRectArray.push_back(popClickPoly);
	pop4 = new  L6S1_Pop4StatueSandals(eL6S1Pop4_Statue_Sandal, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S1_FloatingIsland_1::OnPopupClosed, this));
	pop4->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop4);
	popClickPoly = NULL;

	ActivePopUpId = eL6S1Pop_Inactive;

	HOScene->GetObjectByID("plant_Anim01")->SetPivot(49,91);
	HOScene->GetObjectByID("plant_Anim01")->Animate(0,0,5,0,5,3);
	HOScene->GetObjectByID("plant_Anim02")->SetPivot(48,68);
	HOScene->GetObjectByID("plant_Anim02")->Animate(0,0,-4,0,6,3);
	HOScene->GetObjectByID("plant_Anim03")->SetPivot(0,113);
	HOScene->GetObjectByID("plant_Anim03")->Animate(0,0,1,0,7,4);
	HOScene->GetObjectByID("plant_Anim04")->SetPivot(0,80);
	HOScene->GetObjectByID("plant_Anim04")->Animate(0,0,1.5,0,4,2);

	mo_water = NULL;
	mo_water = new MeshObject(HOScene->GetObjectByID("water"), 5, 5);
	mo_water->meshFX->SetSpeedAndDisplacement(4, 3, 2, 3);
	HOScene->InsertObject(mo_water,1);

	int ArrayPts[] = {569,517,489,518,436,518,432,530,489,542,572,540,568,515};
	ClubPolygon =  new CPolygon(7);
	ClubPolygon->Init(ArrayPts);

	StatueRayFX = new SuperFX("PRT\\L6\\S1_StatueRay.INI", PointSystem::CreateCPoint(825,174,0), 11); 

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_ChestPuzzleSolved)
	{
		StatueRayFX->Start();
	}

	Control::Audio->LoadSample(aVOL6MaryRefS1_1,	AudioVO);
	if(IsVisitedForFirstTime)
	{
		Hud->ShowBannerText("L6ride");
		Control::Audio->PlaySample(aVOL6MaryRefS1_1);
	}

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s1FloatingIsland1Amb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s1FloatingIsland1Amb) )		
	{
		Control::Audio->PlaySample(aAmbL6s1FloatingIsland1Amb,true);
	}

#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(911.0f-660.0f,340.0f+80.0f,108.0f,68.0f);
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

	if( !ProfileSystem::Instance->ActiveProfile.L6_S1_Visited)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S1_Visited = true;
		//Objective Add
		if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O1_FindWayOutOfNetherworld] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O1_FindWayOutOfNetherworld] = true;
			ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
			= eObj_L6O1_FindWayOutOfNetherworld;
			//Increment Objectives
			ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
			Hud->ShowObjectiveFX(eObj_L6O1_FindWayOutOfNetherworld);

			Control::Audio->PlaySample(aSFXMagic1);
		}

		
	}

}

bool L6_S1_FloatingIsland_1::isPopupActive(int popId)
{
	switch (popId)
	{
	case eL6S1Pop1_Water:	if(!ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken || !ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain])	return true;	break;
	case eL6S1Pop2_Statue_Chest:if(!ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated)	return true;	break;
	case eL6S1Pop3_Statue_Hand:	if(/*!ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken || !ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound*/!ProfileSystem::Instance->ActiveProfile.L6_S1_ChestPuzzleSolved)	return true;	break;
	case eL6S1Pop4_Statue_Sandal:	if(!ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken/* || !ProfileSystem::Instance->ActiveProfile.L6_S1_InvMetalContainerTaken*/)	return true;	break;
	default:
		break;
	}

	return false;
}


void L6_S1_FloatingIsland_1::InitPopups()
{

}

void L6_S1_FloatingIsland_1::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopUpId = eL6S1Pop_Inactive;

	switch (popup->GetPopupId())
	{
	case eL6S1Pop1_Water:
		break;
	case eL6S1Pop2_Statue_Chest:
		{
			Hud->InvBox_StopPuzzleMode();
			if (ProfileSystem::Instance->ActiveProfile.L6_S1_ChestPuzzleSolved)
			{
				StatueRayFX->Start();
				Control::Audio->QuickLoadAndPlaySFX("s1_lightRayFromHand");
				Hud->ShowBannerText("L6light");
			}
		}
		break;
	}
}

void L6_S1_FloatingIsland_1::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_ScrewUnsealed)
	{
		HOScene->GetObjectByID("Chappal")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken)
	{
		HOScene->GetObjectByID("cloth")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken)
	{
		HOScene->GetObjectByID("club")->SetEnable(false);
	}

	/*if (!ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced)
	{
	HOScene->GetObjectByID("chest-on-statue")->SetEnable(false);
	}
	*/
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced)
	{
		HOScene->GetObjectByID("dor_open")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_LetterOpened)
	{
		HOScene->GetObjectByID("letter")->SetEnable(false);
	}
}

void L6_S1_FloatingIsland_1::InitParticles()
{
	S1FireFlyFX = new SuperFX("PRT\\L6\\S1FireFly.INI", PointSystem::CreateCPoint(696,550,0), 100); 
	S1FireFlyFX->Start();
	S1FireFlyFX->AdvanceByTime(2.0f);

	S1UnderFogFX = new SuperFX("PRT\\L6\\S1UnderFog.INI", PointSystem::CreateCPoint(748,635,0), 100); 
	S1UnderFogFX->Start();
	S1UnderFogFX->AdvanceByTime(2.0f);

	//S1SkyStarsFX = new SuperFX("PRT\\L6\\S1SkyStars.INI", PointSystem::CreateCPoint(643,131,0), 1100); 
	//S1SkyStarsFX->Start();
	//S1SkyStarsFX->AdvanceByTime(2.0f);

	HOScene->GetObjectByID("FloatingLand01")->Animate(0,-5,0,0,4,2);
	HOScene->GetObjectByID("FloatingLand02")->Animate(0,6,0,0,6,2);
	HOScene->GetObjectByID("FloatingLand03")->Animate(0,-5,0,0,4,2);
	HOScene->GetObjectByID("FloatingLand04")->Animate(0,6,0,0,6,2);
	HOScene->GetObjectByID("FloatingLand05")->Animate(0,-5,0,0,4,2);
}

L6_S1_FloatingIsland_1::~L6_S1_FloatingIsland_1()
{
	SAFE_DELETE(ClubPolygon);
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
	
	SAFE_DELETE(StatueRayFX);
	SAFE_DELETE(S1FireFlyFX);
	SAFE_DELETE(S1UnderFogFX);
	//SAFE_DELETE(S1SkyStarsFX);

	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL6MaryRefS1_1);
	Control::Audio->StopSample(aAmbL6s1FloatingIsland1Amb);
	Control::Audio->UnloadSample(aAmbL6s1FloatingIsland1Amb);
	//MUSIC_SFX


	//SteamAPI_Shutdown();
}

void L6_S1_FloatingIsland_1::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(pop1->isVideoPlaying || pop4->popUp->isVideoPlaying)
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

void L6_S1_FloatingIsland_1::Update()
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
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	if (ActivePopUpId != eL6S1Pop_Inactive)
		return;

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
	#endif

	HiddenObjectSystem::HOUpdate();

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	CPoint pos = Control::Input->Mpos();	

	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken && ClubPolygon->Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			Hud->TakeInventoryItem(eInv_L6_S1_Club);
			HOScene->GetObjectByID("club")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken = true;
			return;
		}
	}

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

	if( SceneRectFloatindIsland_2.Intersection(&pos))
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitUp,eArea_L6_S2_FloatingIsland2);
#else
		Cursor::SetMode(CA_ExitUp,eArea_L6_S2_FloatingIsland2);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S2_FloatingIsland2, 0, SceneRectFloatindIsland_2);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
						
		}
	}
	else if (DescriptionHORect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved)
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S1_DescriptionHO, 0, DescriptionHORect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_DescriptionHO;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}
	}
	else if (FloatingRect1.Intersection(&pos) || FloatingRect2.Intersection(&pos) || FloatingRect3.Intersection(&pos) || FloatingRect4.Intersection(&pos) || FloatingRect5.Intersection(&pos)/* || FloatingRect6.Intersection(&pos) || FloatingRect7.Intersection(&pos)*/)
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6live");
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved && !ProfileSystem::Instance->ActiveProfile.L6_S1_TextForHOShown)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S1_TextForHOShown = true;
		Hud->ShowBannerText("L6love");
	}
}

void L6_S1_FloatingIsland_1::Render(const CPoint* pCam)
{

}


void L6_S1_FloatingIsland_1::TakeInventoryItem(EHOInventory ItemID)
{

}


void L6_S1_FloatingIsland_1::ReceiveMessage(int val)
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
	else if( val == MEMORY_OBJFX_REACHED ) 
	{
		pop1->StartMemoryCutscene();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			if (itemVal == eInv_L6_S4_ColouredPowder)
			{
				Hud->ShowBannerText("L6guess");
				Hud->CancelDrag();
			}
			else
			{
				if (ActivePopUpId != ePopInactive)
				{
					PopupArray[ActivePopUpId]->HandleInventoryDrop(dropPos, itemVal);
					return;
				}
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

float L6_S1_FloatingIsland_1::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L6_S1_FloatingIsland_1::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken)
		isAnyTaskLeft = true;

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

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S1_FloatingIsland1] = isAnyTaskLeft;

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken && ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived && !ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S3_FloatingIsland3])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S3_FloatingIsland3] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken && ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened && !ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S3_FloatingIsland3])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S3_FloatingIsland3] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken && ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S4_FloatingIsland4])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken && ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && !ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S4_FloatingIsland4])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S4_FloatingIsland4])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken && ProfileSystem::Instance->ActiveProfile.L6_S3_PathwayActived && !ProfileSystem::Instance->ActiveProfile.L6_S3_ClubsUsed && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S3_FloatingIsland3])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S3_FloatingIsland3] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated && !ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S2_FloatingIsland2])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected && !ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_PortalChest] = true;
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
	}
}

void L6_S1_FloatingIsland_1::ProcessSkip()
{
	if( ActivePopUpId == eL6S1Pop2_Statue_Chest )
	{
		((L6S1_Pop2StatueChest *)(PopupArray[eL6S1Pop2_Statue_Chest]))->ProcessSkip();
		return;
	}
}

void L6_S1_FloatingIsland_1::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected)
		{
			InitHintTrailEmit(300, 450);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain])
		{
			if (ActivePopUpId != ePopInactive)
				InitHintTrailEmit(950, 390);
			else
				InitHintTrailEmit(220, 540);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 9;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates

		case 1: ++iNonHOHint;
			if (!ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken)
			{
				InitHintTrailEmit(HOScene->GetObjectByID("club")->GetWorldRect().GetCenterX(),HOScene->GetObjectByID("club")->GetWorldRect().GetCenterY(),false);
				return;
			}
			break;
		case 0: ++iNonHOHint;
			if( ActivePopUpId != ePopInactive )
			{
				PopupArray[ActivePopUpId]->ProcessHint();
				return;
			}
			break;
		case 2: ++iNonHOHint;
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
		case 3: ++iNonHOHint;
			if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved)
			{
				InitHintTrailEmit(DescriptionHORect.GetCenterX(),DescriptionHORect.GetCenterY(),false);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2] || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S2_FloatingIsland2])
			{
				InitHintTrailEmit(&SceneRectFloatindIsland_2,true,CA_ExitUp);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if(Hud->ShowHintForComboInvItems())
				return;
			break;

		case 6: ++iNonHOHint;
			Hud->teleportSystem->ShowTeleport();
			return;
			break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L6_S1_FloatingIsland_1::SparkleUpdate()
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

void L6_S1_FloatingIsland_1::CheckForPendingTask()
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


	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken)
	{
		sparklePointsVec.push_back(CPoint(HOScene->GetObjectByID("club")->GetWorldRect().GetCenterX(), HOScene->GetObjectByID("club")->GetWorldRect().GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved)
	{
		sparklePointsVec.push_back(CPoint(DescriptionHORect.GetCenterX(), DescriptionHORect.GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}
}

void L6_S1_FloatingIsland_1::NavigateToScene()
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