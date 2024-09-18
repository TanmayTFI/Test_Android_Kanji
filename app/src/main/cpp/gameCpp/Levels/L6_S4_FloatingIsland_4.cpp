//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE4 : FLOATING ISLAND_4
//====================================

#include "L6_S4_FloatingIsland_4.h"
#include "Util.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);

const CRectangle  SceneRectPortalChest(821.0f,259.0f,245.0f,219.0f);
const CRectangle  SkeletonHeadRect1(262.0f,210.0f,43.0f,45.0f);
const CRectangle  SkeletonHeadRect2(292.0f,369.0f,30.0f,21.0f);
const CRectangle  SkeletonHeadRect3(352.0f,422.0f,58.0f,62.0f);
const CRectangle  SkeletonHeadRect4(804.0f,399.0f,50.0f,44.0f);

const CRectangle  FloatingRect1(18.0f,25.0f,188.0f,288.0f);
const CRectangle  FloatingRect2(467.0f,15.0f,149.0f,148.0f);
const CRectangle  FloatingRect3(644.0f,228.0f,101.0f,130.0f);
const CRectangle  FloatingRect4(920.0f,39.0f,90.0f,197.0f);
const CRectangle  FloatingRect5(1192.0f,13.0f,167.0f,364.0f);

enum eL6S1HOPopEnums
{
	eL6S4Pop_Inactive = -1,
	eL6S4Pop1_PineTree,
	eL6S4Pop2_HotSpring,
	eL6S4Pop3_MagnetizingRocks
};

L6_S4_FloatingIsland_4::L6_S4_FloatingIsland_4()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S4\\S4.SCN");

	int Pop1ArrayPts[] = {237,254,337,239,356,325,248,359,235,254};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL6S4Pop1_PineTree, 5, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);
	pop1 = new  L6S4_Pop1PineTree(eL6S4Pop1_PineTree, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S4_FloatingIsland_4::OnPopupClosed, this));
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);
	popClickPoly = NULL;

	int Po3ArrayPts[] = {636,522,588,516,542,539,535,576,581,582,636,579,662,543,635,519};
	popClickPoly = new ClickablePolygon(eL6S4Pop2_HotSpring, 8, Po3ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L6S4_Pop2HotSpring *pop3 = new  L6S4_Pop2HotSpring(eL6S4Pop2_HotSpring, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S4_FloatingIsland_4::OnPopupClosed, this));
	pop3->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop3);
	popClickPoly = NULL;

	int Pop4ArrayPts[] = {589,496,683,499,680,462,653,412,600,387,538,450,584,477,586,496};
	popClickPoly = new ClickablePolygon(eL6S4Pop3_MagnetizingRocks, 8, Pop4ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L6S4_Pop3MagnetizingRocks *pop4 = new  L6S4_Pop3MagnetizingRocks(eL6S4Pop3_MagnetizingRocks, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S4_FloatingIsland_4::OnPopupClosed, this));

	CObject *naginLetter = new CObject();
	naginLetter->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\NAGINLETTER.SPR", 685, 345, eZOrder_Popup + 200);
	naginLetter->SetUID("lettertext");
	naginLetter->Show(false);
	pop4->PushObject(naginLetter);

	pop4->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop4);
	popClickPoly = NULL;

	ActivePopUpId = eL6S4Pop_Inactive;

#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(1005.0f,35.0f,99.0f,75.0f);		
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s4FloatingIsland4Amb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s4FloatingIsland4Amb) )		
	{
		Control::Audio->PlaySample(aAmbL6s4FloatingIsland4Amb,true);
	}

	//MUSIC_SFX



	InitParticles();

	SceneObjectsUpdate();
	TaskPendingUpdate();

	if(ProfileSystem::Instance->ActiveProfile.L6_S4_IsVisited )
		ProfileSystem::Instance->ActiveProfile.L6_S4_IsVisited  = true;
}

bool L6_S4_FloatingIsland_4::isPopupActive(int popId)
{
	switch (popId)
	{
	case eL6S4Pop1_PineTree:	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeTaken || !ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected)	return true;	break;
	case eL6S4Pop2_HotSpring:	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken || !ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue])	return true;	break;
	case eL6S4Pop3_MagnetizingRocks:	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnetisedHorseShoeTaken || !ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected)	return true;	break;
	default:
		break;
	}

	return false;
}


void L6_S4_FloatingIsland_4::InitPopups()
{

}

void L6_S4_FloatingIsland_4::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopUpId = eL6S4Pop_Inactive;

	/*
	switch (popup->GetPopupId())
	{
	default:
	break;
	}
	*/
}

void L6_S4_FloatingIsland_4::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_HeartCrystalPlaced)
	{
		HOScene->GetObjectByID("GlowingHeartSlot")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_ChestOpened)
	{
		HOScene->GetObjectByID("RoundChestSlot")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("RoundChestSlot")->SetEnable(true);
	}
}

void L6_S4_FloatingIsland_4::InitParticles()
{


	

	s4_PondSmokeFX = new SuperFX("PRT\\L6\\s4_PondSmoke.INI", PointSystem::CreateCPoint(602,553,0), 19); 
	s4_PondSmokeFX->Start();
	s4_PondSmokeFX->AdvanceByTime(2.0f);

	

	s4_bubblesFX = new SuperFX("PRT\\L6\\s4_bubbles.INI", PointSystem::CreateCPoint(609,547,0), 19); 
	s4_bubblesFX->Start();
	s4_bubblesFX->AdvanceByTime(2.0f);




	for (int i = 0; i < 3; i++)
	{
		string _id = "FloatingLands0" + MKSTR(i);
		HOScene->GetObjectByID(_id)->Animate(0, 3.0f + (float)(i), 0, 0, 5, (float)i);
	}
	HOScene->GetObjectByID("PlantTipAnimation01")->SetPivot(46,41);
	HOScene->GetObjectByID("PlantTipAnimation01")->Animate(0,0,2,0,3,2);
	HOScene->GetObjectByID("PlantTipAnimation02")->SetPivot(-48,0);
	HOScene->GetObjectByID("PlantTipAnimation02")->Animate(0,0,1.5,0,4,2);
	HOScene->GetObjectByID("PlantTipAnimation03")->SetPivot(0,24);
	HOScene->GetObjectByID("PlantTipAnimation03")->Animate(0,0,-1,0,3,2);
	HOScene->GetObjectByID("PlantTipAnimation04")->SetPivot(0,35);
	HOScene->GetObjectByID("PlantTipAnimation04")->Animate(0,0,-2,0,3,3);
	HOScene->GetObjectByID("PlantTipAnimation05")->SetPivot(0,25);
	HOScene->GetObjectByID("PlantTipAnimation05")->Animate(0,0,-1,0,3,4);
	HOScene->GetObjectByID("PlantTipAnimation06")->SetPivot(0,24);
	HOScene->GetObjectByID("PlantTipAnimation06")->Animate(0,0,-2,0,3,5);

	SnowFX_L = new SuperFX("PRT\\L6\\s4_smoke.INI", PointSystem::CreateCPoint(900,480,0), 19); 
	SnowFX_L->Start();
	SnowFX_L->AdvanceByTime(2.0f);

	S4_StarsFX = new SuperFX("PRT\\L6\\S4_Stars.INI", PointSystem::CreateCPoint(744,195,0), 100); 
	S4_StarsFX->Start();
	S4_StarsFX->AdvanceByTime(2.0f);
}

L6_S4_FloatingIsland_4::~L6_S4_FloatingIsland_4()
{
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
	SAFE_DELETE(SnowFX_L);
	SAFE_DELETE(S4_StarsFX);
	SAFE_DELETE(s4_bubblesFX);
	SAFE_DELETE(s4_PondSmokeFX);
	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL6s4FloatingIsland4Amb);
	Control::Audio->UnloadSample(aAmbL6s4FloatingIsland4Amb);
	//MUSIC_SFX
}

void L6_S4_FloatingIsland_4::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
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

void L6_S4_FloatingIsland_4::Update()
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
	if (ActivePopUpId != eL6S4Pop_Inactive)
		return;
	HiddenObjectSystem::HOUpdate();

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	CPoint pos = Control::Input->Mpos();

	for (int i = 0; i < (int)PopupArray.size(); i++)
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
	else if (SceneRectPortalChest.Intersection(&pos))
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S4_PortalChest, 0, SceneRectPortalChest);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_PortalChest;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}
	}
	else if (SkeletonHeadRect1.Intersection(&pos) || SkeletonHeadRect2.Intersection(&pos) || SkeletonHeadRect3.Intersection(&pos) || SkeletonHeadRect4.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6skulls");
		}
	}
	else if (FloatingRect1.Intersection(&pos) || FloatingRect2.Intersection(&pos) || FloatingRect3.Intersection(&pos) || FloatingRect4.Intersection(&pos) || FloatingRect5.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6live");
		}
	}
}

void L6_S4_FloatingIsland_4::Render(const CPoint* pCam)
{

}


void L6_S4_FloatingIsland_4::TakeInventoryItem(EHOInventory ItemID)
{

}


void L6_S4_FloatingIsland_4::ReceiveMessage(int val)
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
		if (ActivePopUpId == eL6S4Pop1_PineTree)
		{
			pop1->ReceiveMessage(val);
		}

	}
	TaskPendingUpdate();
}

float L6_S4_FloatingIsland_4::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L6_S4_FloatingIsland_4::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i =  0; i < (int)(PopupArray.size()); i++)
	{
		if(isPopupActive(i)) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = isAnyTaskLeft;

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected && ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced && !ProfileSystem::Instance->ActiveProfile.L6_S3_FlintStoneUsed)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S3_FloatingIsland3] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S1_FloatingIsland1] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S1_FloatingIsland1] = true;
	if(ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanWithPinePitchPlaced)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_PZPineGum] = true;
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2] = true;
	}
}

void L6_S4_FloatingIsland_4::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected)
		{
			InitHintTrailEmit(1060, 80);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue])
		{
			if (ActivePopUpId != ePopInactive)
				InitHintTrailEmit(410, 200);
			else
				InitHintTrailEmit(610, 550);
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
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_PortalChest] )
			{
				InitHintTrailEmit(&SceneRectPortalChest,true);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2] )
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if(Hud->ShowHintForComboInvItems())
				return;
			break;

		case 5: ++iNonHOHint;
			Hud->teleportSystem->ShowTeleport();
			return;
			break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L6_S4_FloatingIsland_4::SparkleUpdate()
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

void L6_S4_FloatingIsland_4::CheckForPendingTask()
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
	if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_PortalChest] )
	{
		sparklePointsVec.push_back(CPoint(SceneRectPortalChest.GetCenterX(), SceneRectPortalChest.GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}


}

void L6_S4_FloatingIsland_4::NavigateToScene()
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