//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 06 : Cave End
//====================================

#include "L2_S6_TunnelEnd.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "TextureFactory.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "AwardManager.h"

const CPoint		CenterPoint(683,334,0);
const CPoint		PopCloseRectBtnPoint(998,98,0);
const CRectangle	PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle	PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle	PopCircleAreaRect(502.0f,153.0f,364.0f,360.0f);

//Inventory Destination Rects
const CRectangle  InvDestRectAxeOnWood(227.0f,3.0f,893.0f,600.0f);
const CRectangle  InvDestRectWood(225.0f,7.0f,974.0f,583.0f);

//Scene Rects
const CRectangle  pzClockRect(621.0f,176.0f,250.0f,225.0f);
const CRectangle  pop1RectCreekSkeleton(443.0f,527.0f,280.0f,135.0f);


const CRectangle  PopRect2Locket(619.0f,338.0f,116.0f,107.0f);
const CRectangle  PopRect3Holder(596.0f,120.0f,180.0f,158.0f);

const CRectangle  ActionRectOpenLocket(653.0f,219.0f,144.0f,142.0f);

const CRectangle  SceneRectWaterfallHO(926.0f,321.0f,64.0f,135.0f);

const CRectangle  InvDestRectFireLeft(136.0f,142.0f,238.0f,277.0f);
const CRectangle  InvDestRectFireRight(1110.0f,58.0f,254.0f,295.0f);


L2_S6_TunnelEnd::L2_S6_TunnelEnd()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S6\\S6.SCN");
	musicFadeOutTimer = 0;

	rootsToCut = HOScene->GetObjectByID("CutActivity_Root1");


	IsWoodCutDown = ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown;

	GFInstance->PostUpdate(1);

	InitPopups();


	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L2_S1_ForestEntrance]  = true;

	// popups
	int p1rect[] = {493,313,468,359,505,410,641,390,619,339,553,301};
	pop1ClickPoly = new ClickablePolygon(eHOPop1_StonesNKnife, 6, p1rect, CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaClicked,this));
	pop1ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaMouseOver,this));
	pop1ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaMouseOut,this));
	PopPolyArray[eHOPop1_StonesNKnife] = pop1ClickPoly;

	pop1Stones = new L2S6_Pop1StonesNKnife(eHOPop1_StonesNKnife, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L2_S6_TunnelEnd::onPopupClosed, this));
	pop1Stones->SetPopupBoundingBox(PopAreaRect);
	PopUpArray[eHOPop1_StonesNKnife] = pop1Stones;

	//int p2Rect[] = {572,512, 438,556, 442,629, 573,693, 705,629, 707,543, 570,509};
	int p2Rect[] = {420,510, 550,510, 550, 590, 420,590, 419,509};
	pop2ClickPoly=  new ClickablePolygon(eHOPop2_CreeksSkeleton, 5, p2Rect, CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaClicked,this));
	pop2ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaMouseOver,this));
	pop2ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaMouseOut,this));
	PopPolyArray[eHOPop2_CreeksSkeleton] = pop2ClickPoly;

	pop2Skeleton = new L2S6_Pop2CreeksSkeleton(eHOPop2_CreeksSkeleton, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L2_S6_TunnelEnd::onPopupClosed, this));
	pop2Skeleton->SetPopupBoundingBox(PopAreaRect);
	PopUpArray[eHOPop2_CreeksSkeleton] = pop2Skeleton;

	int p5Rect[] = {924,334,924,404,961,430,990,403,1014,345,982,295,921,331};
	pop5ClickPoly=  new ClickablePolygon(eHOPop5_StoneFace, 7, p5Rect, CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaClicked,this));
	pop5ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaMouseOver,this));
	pop5ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaMouseOut,this));
	PopPolyArray[eHOPop5_StoneFace] = pop5ClickPoly;

	pop5StoneFace = new L2S6_Pop5StoneFace(eHOPop5_StoneFace, Hud, HOScene, eZOrder_Popup, pop5ClickPoly->center, CC_CALLBACK_1(L2_S6_TunnelEnd::onPopupClosed, this));
	pop5StoneFace->SetPopupBoundingBox(PopAreaRect);
	PopUpArray[eHOPop5_StoneFace] = pop5StoneFace;

	int p6Rect[] = {857,212,874,310,769,347,677,282,740,211,854,209};
	pop6ClickPoly=  new ClickablePolygon(eHOPop6_OilLamp, 6, p6Rect, CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaClicked,this));
	pop6ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaMouseOver,this));
	pop6ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2_S6_TunnelEnd::onPopAreaMouseOut,this));
	PopPolyArray[eHOPop6_OilLamp] = pop6ClickPoly;

	pop6OilLamp = new L2S6_Pop6OilLamp(eHOPop6_OilLamp, Hud, HOScene, eZOrder_Popup, pop6ClickPoly->center, CC_CALLBACK_1(L2_S6_TunnelEnd::onPopupClosed, this));
	pop6OilLamp->SetPopupBoundingBox(PopCircleAreaRect);
	PopUpArray[eHOPop6_OilLamp] = pop6OilLamp;

	isMouseOverPopup = false;

	fogVFXL = new SuperFX("PRT\\L2\\S6_snowL.INI", PointSystem::CreateCPoint(400,600,0), 6);
	fogVFXL->Start();

	fogVFXR = new SuperFX("PRT\\L2\\S6_snowMt.INI", PointSystem::CreateCPoint(760,265,0), 6);
	fogVFXR->Start();

	lpVidWoodCutScn = NULL;
	vidTimer = 0;

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	lpVidWoodCutScn = new CVideo("OGV\\L2\\S6WOODCUTSCN.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpVidWoodCutScn->SetScale(1.334f);
	lpVidWoodCutScn->SetScale(GFApp->video_aspectRatio);

	isVideoPlaying = false;

	holinesFX = new SuperFX("PRT\\L2\\holines.INI", PointSystem::CreateCPoint(952,420,0), 100); 

	hodotsFX = new SuperFX("PRT\\L2\\hodots.INI", PointSystem::CreateCPoint(946,437,0), 100); 

	if( ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced && !ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompleted)
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			holinesFX->Start();
			holinesFX->AdvanceByTime(2.0f);

			hodotsFX->Start();
			hodotsFX->AdvanceByTime(2.0f);
		}
	}

	SceneObjectsUpdate();

	//#ifdef _CEBUILD
	//	//Morph Object Add
	//	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S6Collected)
	//	{
	//		Hud->setMorphObjects(HOScene->GetObjectByID("morphing_Eagle"), HOScene->GetObjectByID("morphing_Snake"));
	//		const CRectangle  MorphRect(1082.0f,333.0f,49.0f,61.0f);
	//		Hud->setMorphObjRect(MorphRect);
	//	}
	//	else
	//	{
	//		HOScene->GetObjectByID("morphing_Eagle")->SetEnable(false);
	//	}
	//#else
	//	HOScene->GetObjectByID("morphing_Eagle")->SetEnable(false);
	//#endif 

	TaskPendingUpdate();


	Control::Audio->LoadSample(aVOL2MaryRefS6_1,	AudioVO);
	Control::Audio->LoadSample(aAmbs6TunnelEnd,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs6TunnelEnd) )		
	{
		Control::Audio->PlaySample(aAmbs6TunnelEnd,true);
	}
}

void L2_S6_TunnelEnd::onPopAreaMouseOut(int popId)
{
	isMouseOverPopup = false;
}

void L2_S6_TunnelEnd::onPopAreaMouseOver(int popId)
{
	if (ActivePopupID != eHOS6Pop_Inactive || isVideoPlaying)
		return;

	switch (popId)
	{
	case eHOPop1_StonesNKnife: if((!ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken || !ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken) )//&& ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
								   isMouseOverPopup = true;
		break;
	case eHOPop2_CreeksSkeleton: if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles])
									 isMouseOverPopup = true;
		break;

	case eHOPop5_StoneFace:	if (!ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced)// && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
								isMouseOverPopup = true;
		break;
	case eHOPop6_OilLamp:	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
								isMouseOverPopup = true;
		break;
	default:
		break;
	}

}

void L2_S6_TunnelEnd::onPopAreaClicked(int popId)
{
	if (ActivePopupID != eHOS6Pop_Inactive || isVideoPlaying)
		return;
	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown)
		return;

	switch (popId)
	{
	case eHOPop1_StonesNKnife:

		if( (!ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken || !ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken))// && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
		{
			ActivePopupID = eHOPop1_StonesNKnife;
			pop1Stones->OpenPopup();
		}
		break;
	case eHOPop2_CreeksSkeleton:
#ifdef _CEBUILD
		if(!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles])
#else
		if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken)
#endif
		{
			ActivePopupID = eHOPop2_CreeksSkeleton;
			pop2Skeleton->OpenPopup();
		}
		break;
	case eHOPop5_StoneFace:

		if (!ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced)// && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened )
		{
			ActivePopupID = eHOPop5_StoneFace;
			pop5StoneFace->OpenPopup();
		}		
		break;
	case eHOPop6_OilLamp:
		if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
		{
			ActivePopupID = eHOPop6_OilLamp;
			pop6OilLamp->OpenPopup();
		}
		break;
	default:
		break;
	}
}

void L2_S6_TunnelEnd::SparkleUpdate()
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

void L2_S6_TunnelEnd::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < eHOL2S6Pop_Max; i++)
	{
		if (PopStates[i] != ePopupState_Inactive)
		{
			//if(popup->GetPopupState() == ePopupState_Inactive || popup->GetPopupState() == ePopupState_Closed) 
			{
				if(PopUpArray[i]->isTaskLeft())
				{
					sparklePointsVec.push_back(PopUpArray[i]->GetSparkleUpdatePoint());
					isSparkleUpdatePending = true;
				}
			}
		}

	}


	if (ProfileSystem::Instance->ActiveProfile.L2_S6_ClockPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken && ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected)
	{
		sparklePointsVec.push_back(CPoint(pzClockRect.GetCenterX(), pzClockRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
}

void L2_S6_TunnelEnd::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < eHOL2S6Pop_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopUpArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}
	if( !ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown && ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeSharpenTaken)
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompleted && ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced)
		isAnyTaskLeft = true;		
	if( !ProfileSystem::Instance->ActiveProfile.L2_S6_ClockPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken && ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd] = isAnyTaskLeft;

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && ProfileSystem::Instance->ActiveProfile.L2S6_InvGlovesTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_ForestEntrance] = true;
	if( !ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S2_NagaTemple] = true;
}

void L2_S6_TunnelEnd::SceneObjectsUpdate()
{
	if(IsWoodCutDown)
	{
		rootsToCut->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
	{
		HOScene->GetObjectByID("Fire2")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("Fire2")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced)
	{
		HOScene->GetObjectByID("WaterBG")->SetEnable(false);
		HOScene->GetObjectByID("PopupArea_Stone")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("PopupArea_Stone")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
	{
		HOScene->GetObjectByID("Creek_locket_closed")->SetEnable(false);
		HOScene->GetObjectByID("Creek_locket_opned")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("Creek_locket_opned")->SetEnable(false);
	}

	HOScene->GetObjectByID("Creek_Bottle_paper")->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen)
	{
		HOScene->GetObjectByID("Creek_Bottle_inside")->SetEnable(true);
		HOScene->GetObjectByID("Creek_Bottle_open")->SetEnable(true);

		if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken)
		{
			HOScene->GetObjectByID("Creek_Bottle_paper")->SetEnable(true);
		}
	}
	else
	{
		HOScene->GetObjectByID("Creek_Bottle_inside")->SetEnable(false);
		HOScene->GetObjectByID("Creek_Bottle_open")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
	{
		HOScene->GetObjectByID("Dark")->SetEnable(false);
	}
}

void L2_S6_TunnelEnd::InitPopups()
{	
	ActivePopupID = ePopInactive;

	for (int i = 0; i < eHOL2S6Pop_Max; i++)
	{
		PopStates[i] = ePopupState_Inactive;
	}

	//PopUp1
	if((!ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken || !ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken))// && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
	{
		PopStates[eHOPop1_StonesNKnife] = ePopupState_Closed;
	}
	//PopUp2
#ifdef _CEBUILD
	if( !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles])
#else
	if( !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken)
#endif
	{
		PopStates[eHOPop2_CreeksSkeleton] = ePopupState_Closed;
	}
	//PopUp3
	if (!ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced)// && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
	{
		PopStates[eHOPop5_StoneFace] = ePopupState_Closed;
	}		
	//PopUp4
	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
	{
		PopStates[eHOPop6_OilLamp] = ePopupState_Closed;
	}
}

void L2_S6_TunnelEnd::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if (isVideoPlaying )
		return;

	if( Control::Audio->IsPlaying(aTrackL2theme1))
		return;

	if( Control::Audio->IsPlaying(aTrackL2ho) )
	{
		Control::Audio->Samples[aTrackL2ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL2ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL2ho);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL2theme) )
				Control::Audio->PlaySample(aTrackL2theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL2puzzle) )
	{
		Control::Audio->Samples[aTrackL2puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL2puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL2puzzle);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL2theme) )
				Control::Audio->PlaySample(aTrackL2theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2theme))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2theme, true);
		}
	}
}

void L2_S6_TunnelEnd::Update()
{
	NavigateToScene();
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

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if (ActivePopupID != eHOS6Pop_Inactive)
		return;

	HiddenObjectSystem::HOUpdate();

	CPoint pos = Control::Input->Mpos();

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	if (isVideoPlaying )
	{
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			lpVidWoodCutScn->isEndReached() ||
#endif
			vidTimer > 22)
		{
			isVideoPlaying = false;
			rootsToCut->SetEnable(false);
			lpVidWoodCutScn->StopAndFadeOutVideo(2.0f);					
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->SendUpdateSceneMessage();
			Hud->ShowBannerText("L2could");
			Control::Audio->PlaySample(aVOL2MaryRefS6_1);
			if( !Control::Audio->IsPlaying(aAmbs6TunnelEnd) )		
				Control::Audio->PlaySample(aAmbs6TunnelEnd,true);
		} 
		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown)
	{
		if( InvDestRectWood.Intersection(&pos) )
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked() )
			{				
				Hud->ShowBannerText("L2cut");
			}
			return;
		}
	}

	if (isMouseOverPopup)
	{
		Cursor::SetMode(CA_Look);
		return;
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
			return;
		}
	#endif

	if( IsMouseOverGoBackArea(&pos) )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L2_S4_Cave);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L2_S4_Cave);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S4_Cave, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown)
	{
		if( pzClockRect.Intersection(&pos))
		{
			if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
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
						GFHud->InvBoxCtrl->GoToScene(eArea_L2_S6_PZTunnelClock, 0, pzClockRect);
					}
#else
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_PZTunnelClock;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				}	
			}
			else if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2spot");	
			}
		}

		/*	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened && Control::Input->LBclicked())
		{
		Hud->ShowBannerText("It's pitch dark");	//Sac: where is the external text for this ?
		}*/
	}

	if( ActivePopupID == ePopInactive && SceneRectWaterfallHO.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced && !ProfileSystem::Instance->ActiveProfile.L2S6_InvGlovesTaken)
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S6_HOWaterfall, 0, SceneRectWaterfallHO);
			}
#else
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_HOWaterfall;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}			
		return;
	}

	/*if (pos.y < 100 && Control::Input->LBclicked())
	{
	Hud->ShowBannerText("It's pitch dark");//Sac: where is the external text for this ?

	}*/
}

void L2_S6_TunnelEnd::onPopupClosed(Popup * popup)
{
	//K_LOG("popup closed %d", popup->GetPopupId());

	SceneObjectsUpdate();

	ActivePopupID = eHOS6Pop_Inactive;

	bool isInactive = false;

	switch (popup->GetPopupId())
	{
	case eHOPop1_StonesNKnife: if(ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken && ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken)
								   isInactive = true;
		break;
	case eHOPop5_StoneFace:	if (ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced)
							{
								isInactive = true;
								//Start HO Sparkles
								if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
								{
									holinesFX->Start();
									holinesFX->AdvanceByTime(2.0f);

									hodotsFX->Start();
									hodotsFX->AdvanceByTime(2.0f);
								}
							}
							break;
	case eHOPop6_OilLamp:	if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
								isInactive = true;
		break;

	default:
		break;
	}

	if (isInactive)
		PopStates[popup->GetPopupId()] = ePopupState_Inactive;
}

float L2_S6_TunnelEnd::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

L2_S6_TunnelEnd::~L2_S6_TunnelEnd()
{
	TaskPendingUpdate();

	
	for (int i = 0; i < eHOL2S6Pop_Max; i++)
	{
		SAFE_DELETE(PopUpArray[i]);		
	}
	SAFE_DELETE(pop1ClickPoly);
	SAFE_DELETE(pop2ClickPoly);
	SAFE_DELETE(pop5ClickPoly);
	SAFE_DELETE(pop6ClickPoly);

	SAFE_DELETE(fogVFXL);
	SAFE_DELETE(fogVFXR);

	SAFE_DELETE(lpVidWoodCutScn);

	SAFE_DELETE(holinesFX);
	SAFE_DELETE(hodotsFX);

	//MUSIC_SFX
	//Quick SFX unloaded in HUD
	Control::Audio->UnloadSample(aVOL2MaryRefS6_1);
	Control::Audio->StopSample(aAmbs6TunnelEnd);
	Control::Audio->UnloadSample(aAmbs6TunnelEnd);
	//MUSIC_SFX
}

void L2_S6_TunnelEnd::ReceiveMessage(int val)
{
	if( val == HOSM_DialogClosed ) 
	{
		//isDialogActive = false;
		Hud->AllowInput = true;
		Hud->ShowHud(true);

	//	isSparkleUpdatePending = true;
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			if(ActivePopupID == ePopInactive)
			{
				if( itemVal == eInv_L2S4_AxeSharpen && !IsWoodCutDown && InvDestRectAxeOnWood.Intersection(&dropPos))
				{
					if( !IsWoodCutDown && InvDestRectAxeOnWood.Intersection(&dropPos) )
					{
						//if( !ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown )
						{
							ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown = true;
							IsWoodCutDown = true;

							Hud->ReleaseDrag();
							Hud->CloseInventory(); 						
							if(ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2)
							{
								ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_AxeSharpen] = false;
								Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_AxeSharpen);
							}

							Hud->AllowInput = false;
							Hud->HideHud();

							Hud->HideMessageBanner();
							lpVidWoodCutScn->SetSkipActive();
							isVideoPlaying = true;
							lpVidWoodCutScn->PlayFadeInVideo(2.0f);

							if( Control::Audio->IsPlaying(aTrackL2theme) )
								Control::Audio->StopSample(aTrackL2theme);
							if( Control::Audio->IsPlaying(aAmbs6TunnelEnd) )		
								Control::Audio->StopSample(aAmbs6TunnelEnd);
						}
						return;
					}
				}
				else if ( itemVal == eInv_L2S4_UnlitOilTorch && IsWoodCutDown && (InvDestRectFireLeft.Intersection(&dropPos) || InvDestRectFireRight.Intersection(&dropPos)) )
				{
					Hud->ShowBannerText("L2high");

					Hud->CancelDrag();
				}
				/*else if (itemVal == eInv_L2S6_KnifeBlunt && !IsWoodCutDown && InvDestRectAxeOnWood.Intersection(&dropPos))
				{
				Hud->ShowBannerText("It's rusted. I wonder if there's any kind of rust remover around here?");//Sac: where is the external text for this ?
				}
				else if (itemVal == eInv_L2S6_KnifeSharpen &&!IsWoodCutDown &&  InvDestRectAxeOnWood.Intersection(&dropPos))
				{
				Hud->ShowBannerText("Nope! I need something bigger. Worth the try though.");//Sac: where is the external text for this ?
				}*/
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			if (ActivePopupID != eHOS6Pop_Inactive)
				PopUpArray[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal);
		}

	}
}

void L2_S6_TunnelEnd::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(545, 460);
			else
				InitHintTrailEmit(550, 570);
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
			break;

		case 0: ++iNonHOHint;
			if( ActivePopupID != ePopInactive )
			{
				PopUpArray[ActivePopupID]->ProcessHint();
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown)
			{
				for (int i = 0; i < eHOL2S6Pop_Max; i++)
				{
					if(PopStates[i] != ePopupState_Inactive) //if popup is active
					{
						if(PopUpArray[i]->isTaskLeft())
						{
							InitHintTrailEmit(&PopPolyArray[i]->center,false);
							return;
						}
					}
				}
			}
			break;
		case 2: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown && ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeSharpenTaken)
			{
				Hud->ShowHintForInvItem(eInv_L2S4_AxeSharpen,&InvDestRectAxeOnWood);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompleted && ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced)
			{
				InitHintTrailEmit(&SceneRectWaterfallHO,false);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L2_S6_ClockPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken && ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected)
			{
				InitHintTrailEmit(&pzClockRect,false);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L2_S6_ClockPuzzleSolved)
			{
				InitHintTrailEmit(&pzClockRect,true,CA_ExitUp);
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S4_Cave])
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 7: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 8: ++iNonHOHint;
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


void L2_S6_TunnelEnd::NavigateToScene()
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