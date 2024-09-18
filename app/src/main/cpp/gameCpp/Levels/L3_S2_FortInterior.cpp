//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE2 : FortInterior
//====================================

#include "L3_S2_FortInterior.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Circle.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"

const CPoint		CenterPoint(683,334,0);
const CPoint		PopCloseRectBtnPoint(998,98,0);
const CRectangle	PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  SceneRectMaze(920.0f,126.0f,118.0f,241.0f);



//Inventory Item Rects
//Info Rects


L3_S2_FortInterior::L3_S2_FortInterior()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S2\\S2.SCN");
	musicFadeOutTimer = 0;
	moonprtFX = NULL;
	S2crystalprt1FX = NULL;
	S2crystalprt2FX = NULL;
	S2crystalprt3FX = NULL;

	for (int i = 0; i < ePopL3S2_Max; i++)
	{
		PopupArray[i] = NULL;
		PopPolyArray[i]= NULL;
	}
	//Scene Animations
	HOScene->GetObjectByID("blueLight")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueLight")->Animate(0.75f,1.0f,10.0f);

	HOScene->GetObjectByID("yellowLight")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("yellowLight")->Animate(0.75f,1.0f,2.0f);

	HOScene->GetObjectByID("leaf1")->SetPivot(-15, -102);
	HOScene->GetObjectByID("leaf1")->Animate(0,0,1,0,2.0f);	

	HOScene->GetObjectByID("leaf2")->SetPivot(-14, -142);
	HOScene->GetObjectByID("leaf2")->Animate(0,0,1,0,2.0f);	

	HOScene->GetObjectByID("leaf3")->SetPivot(-14, -142);
	HOScene->GetObjectByID("leaf3")->Animate(0,0,1,0,2.0f);	

	HOScene->GetObjectByID("leaf4")->SetPivot(-14, -142);
	HOScene->GetObjectByID("leaf4")->Animate(0,0,1,0,2.0f);	

	HOScene->GetObjectByID("chain1")->SetPivot(-100, 93);
	HOScene->GetObjectByID("chain1")->Animate(0,0,1,0,2.0f);	

	HOScene->GetObjectByID("chain2")->SetPivot(230, -40);
	HOScene->GetObjectByID("chain2")->Animate(0,0,0.5f,0,5.0f);	

	HOScene->GetObjectByID("chain3")->SetPivot(235, -156);
	HOScene->GetObjectByID("chain3")->Animate(0,0,0.5f,0,5.0f);	

	HOScene->GetObjectByID("chain4")->SetPivot(-55, 108);
	HOScene->GetObjectByID("chain4")->Animate(0,0,0.3f,0,2.0f);	

	HOScene->GetObjectByID("popfireanim")->SetScale(0.65f);
	
	moonprtFX = new SuperFX("PRT\\L3\\S2moonprt.INI",PointSystem::CreateCPoint(406,206,0), 100); 
	moonprtFX->Start();
	moonprtFX->AdvanceByTime(2.0f);

	

	S2crystalprt1FX = new SuperFX("PRT\\L3\\S2crystalprt1.INI", PointSystem::CreateCPoint(119,565,0), 100); 
	S2crystalprt1FX->Start();
	S2crystalprt1FX->AdvanceByTime(2.0f);

	S2crystalprt2FX = new SuperFX("PRT\\L3\\S2crystalprt2.INI", PointSystem::CreateCPoint(484,481,0), 100); 
	S2crystalprt2FX->Start();
	S2crystalprt2FX->AdvanceByTime(2.0f);

	S2crystalprt3FX = new SuperFX("PRT\\L3\\S2crystalprt3.INI", PointSystem::CreateCPoint(1296,474,0), 100); 
	S2crystalprt3FX->Start();
	S2crystalprt3FX->AdvanceByTime(2.0f);

	isBlastCutscnPlaying = false;
	cv_BlastCutscn = NULL;
	vidTimer = 0;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken)
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);
		CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		//CRectangle RectVidFull(0, 0, 1366, 768);
		cv_BlastCutscn = new CVideo("OGV\\L3\\S2WALLBLAST.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//cv_BlastCutscn->SetScale(1.334f);
		cv_BlastCutscn->SetScale(GFApp->video_aspectRatio);
		//cv_BlastCutscn->SetSkipActive(); 
	}

	GFInstance->PostUpdate(1);

	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s2FortInterior,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s2FortInterior) )		
	{
		Control::Audio->PlaySample(aAmbL3s2FortInterior,true);
	}
	
	//MUSIC_SFX

	InitPopups();

#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S2Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		const CRectangle  MorphRect(754.0f,277.0f,91.0f,74.0f);
		
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
	Control::Audio->ClearUnusedTracks();
	
	//MUSIC_SFX

	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L3_S2_FortInterior]  = true;

	
}

void L3_S2_FortInterior::SparkleUpdate()
{
	if(isSparkleUpdatePending)
	{
		isSparkleUpdatePending = false;
		if(SparkleFX==NULL)
		{
			SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);		
		}
		//Game Logic Goes here

		//Game Logic Goes here
		numSparkles = 0;
		currentSparkleID = 0;
		
		//Check for Popup Tasks
		for (int i = 0; i < ePopL3S2_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopupArray[i]->isTaskLeft())
				{
					sparklePoints[numSparkles].SetXY(PopPolyArray[i]->center.x, PopPolyArray[i]->center.y);
					numSparkles++;
				}
			}
		}
	}

	sparkleTimer += Control::LogicRate;
	if(sparkleTimer>sparkleThreshold)
	{
		sparkleTimer = 0;
		if(numSparkles>0)
		{			
			SparkleFX->SetPos(&sparklePoints[currentSparkleID]);
			SparkleFX->Start();
			currentSparkleID += 1;
			if(currentSparkleID>=numSparkles)
				currentSparkleID = 0;
		}
	}
}

void L3_S2_FortInterior::SceneObjectsUpdate()
{
	TaskPendingUpdate();
	HOScene->GetObjectByID("crow")->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_BrickTaken || ProfileSystem::Instance->ActiveProfile.L3_S2_BrickFixed)
		HOScene->GetObjectByID("brick")->SetEnable(false);
	else
		HOScene->GetObjectByID("brick")->SetEnable(true);
	
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_CocoShellPowderFilledFixed || ProfileSystem::Instance->ActiveProfile.L3_S2_BrickFixed)
		HOScene->GetObjectByID("shell")->SetEnable(false);
	else
		HOScene->GetObjectByID("shell")->SetEnable(true);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_CocoShellPowderFilledFixed ||  ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken)
		HOScene->GetObjectByID("thread")->SetEnable(false);
	else
		HOScene->GetObjectByID("thread")->SetEnable(true);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed)
		HOScene->GetObjectByID("tooth")->SetEnable(false);
	else
		HOScene->GetObjectByID("tooth")->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_CrowFlewAway)
		HOScene->GetObjectByID("CrowLoopAnim")->SetEnable(false);
	else
		HOScene->GetObjectByID("CrowLoopAnim")->SetEnable(true);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken)
		HOScene->GetObjectByID("doorBlasted")->SetEnable(false);
	else
		HOScene->GetObjectByID("doorBlasted")->SetEnable(true);
	
	


}

void L3_S2_FortInterior::TaskPendingUpdate()
{
	
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL3S2_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken && !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S2_P1Room1Start])
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_FortInterior] = isAnyTaskLeft;
	
	if(ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened && !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S4_FortFountain])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S3_Pool] = true;
}

void L3_S2_FortInterior::InitPopups()
{	
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL3S2_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	
	//Popup 1
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvTwineTaken)
	{
		int p1rect[] = {493,356,600,353,628,446,494,448};
		
		PopPolyArray[ePop1_Thread] = new CPolygon(4);
		PopPolyArray[ePop1_Thread]->Init(p1rect);


		PopupArray[ePop1_Thread] = new L3S2_Pop1Thread(ePop1_Thread,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop1_Thread]->center, CC_CALLBACK_1(L3_S2_FortInterior::onPopupClosed, this));
		PopupArray[ePop1_Thread]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop1_Thread] = ePopupState_Closed;
	}

	//Popup 2
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHandTaken)
	{
		int p2rect[] = {604,202,710,197,758,133,733,89,649,69,579,87};
		PopPolyArray[ePop2_Skull] = new CPolygon(6);
		PopPolyArray[ePop2_Skull]->Init(p2rect);

		PopupArray[ePop2_Skull] = new L3S2_Pop2Skull(ePop2_Skull,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop2_Skull]->center, CC_CALLBACK_1(L3_S2_FortInterior::onPopupClosed, this));
		PopupArray[ePop2_Skull]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop2_Skull] = ePopupState_Closed;
	}
	
	//Popup 3
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconnutTaken)
	{
		
		int p3rect[] = {601,425,652,457,706,456,763,428,785,400,730,384,664,387};
		PopPolyArray[ePop3_Coconut] = new CPolygon(7);
		PopPolyArray[ePop3_Coconut]->Init(p3rect);

		PopupArray[ePop3_Coconut] = new L3S2_Pop3Coconut(ePop3_Coconut,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop3_Coconut]->center, CC_CALLBACK_1(L3_S2_FortInterior::onPopupClosed, this));
		PopupArray[ePop3_Coconut]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop3_Coconut] = ePopupState_Closed;
	}

	//Popup 4
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened)
	{

		int p3rect[] = {577,233,582,201,668,193,740,216,741,261,692,285,628,286};
		PopPolyArray[ePop4Crow] = new CPolygon(7);
		PopPolyArray[ePop4Crow]->Init(p3rect);

		PopupArray[ePop4Crow] = new L3S2_Pop4Crow(ePop4Crow,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop4Crow]->center, CC_CALLBACK_1(L3_S2_FortInterior::onPopupClosed, this));
		PopupArray[ePop4Crow]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop4Crow] = ePopupState_Closed;
	}

	//Popup 5
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_BackDoorOpened)
	{

		int p5rect[] = {304,564,1147,570,1148,666,310,663};
		PopPolyArray[ePop5_BackDoor] = new CPolygon(4);
		PopPolyArray[ePop5_BackDoor]->Init(p5rect);

		PopupArray[ePop5_BackDoor] = new L3S2_Pop5BackDoor(ePop5_BackDoor,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop5_BackDoor]->center, CC_CALLBACK_1(L3_S2_FortInterior::onPopupClosed, this));
		PopupArray[ePop5_BackDoor]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop5_BackDoor] = ePopupState_Inactive;
	}

	//Popup 6
#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken || !ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing])
#else
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken || !ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken)
#endif 
	{
		int p6rect[] = {364,417,493,430,537,501,527,529,407,535,338,490};
		PopPolyArray[ePop6_GunPowder] = new CPolygon(6);
		PopPolyArray[ePop6_GunPowder]->Init(p6rect);

		PopupArray[ePop6_GunPowder] = new L3S2_Pop6GunPowder(ePop6_GunPowder,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop6_GunPowder]->center, CC_CALLBACK_1(L3_S2_FortInterior::onPopupClosed, this));
		PopupArray[ePop6_GunPowder]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop6_GunPowder] = ePopupState_Closed;
	}

	//Popup 7
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken)
	{

		int p5rect[] = {917,128,1041,119,1020,357,912,355};
		PopPolyArray[ePop7_MazeDoor] = new CPolygon(4);
		PopPolyArray[ePop7_MazeDoor]->Init(p5rect);

		PopupArray[ePop7_MazeDoor] = new L3S2_Pop7MazeDoor(ePop7_MazeDoor,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop7_MazeDoor]->center, CC_CALLBACK_1(L3_S2_FortInterior::onPopupClosed, this));
		PopupArray[ePop7_MazeDoor]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop7_MazeDoor] = ePopupState_Closed;
	}

	SceneObjectsUpdate();
}

void L3_S2_FortInterior::onPopupClosed(Popup * popup)
{
	ActivePopupID = ePopInactive;
	isSparkleUpdatePending = true;
	switch (popup->GetPopupId())
	{
	case 0:
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvTwineTaken)
		{
			//disable popup
			SceneObjectsUpdate();
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		else
		{
			Hud->InvBox_StopPuzzleMode();
			SceneObjectsUpdate();
		}
		break;
	case 1:
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHandTaken)
		{
			//disable popup
			SceneObjectsUpdate();
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		else
			SceneObjectsUpdate();
		break;
	case 2:
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconnutTaken)
		{
			//disable popup
			SceneObjectsUpdate();
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		else
			SceneObjectsUpdate();
		break;
	case 3:
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened)
		{
			//disable popup
			SceneObjectsUpdate();
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		else
			SceneObjectsUpdate();
		break;
	case 4:
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_BackDoorOpened)
		{
			//disable popup
			SceneObjectsUpdate();
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		else
			SceneObjectsUpdate();
		break;
	case 5:
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken && ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken)
		{
			//disable popup
			SceneObjectsUpdate();
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		else
			SceneObjectsUpdate();
		break;
	case 6:
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
			Hud->AllowInput = false;
			Hud->HideHud();		
			cv_BlastCutscn->PlayFadeInVideo(2.0f);
			isBlastCutscnPlaying = true;
		}
		else
			SceneObjectsUpdate();
		break;
	default:
		break;
	}
}

void L3_S2_FortInterior::ProcessSkip()
{
	PopupArray[ePop1_Thread]->ProcessHint();
}

void L3_S2_FortInterior::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(isBlastCutscnPlaying)
		return;

	if( Control::Audio->IsPlaying(aTrackL3ho) )
		{
			Control::Audio->Samples[aTrackL3ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL3ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL3ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL3theme) )
					  Control::Audio->PlaySample(aTrackL3theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL3puzzle) )
		{
			Control::Audio->Samples[aTrackL3puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL3puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL3puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL3theme) )
					  Control::Audio->PlaySample(aTrackL3theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL3theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL3theme, true);
			}
		}
}

void L3_S2_FortInterior::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if (isBlastCutscnPlaying)
	{
		Cursor::SetMode(CA_Normal);
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_BlastCutscn->isEndReached() ||
#endif
			vidTimer > 3)
		{
			SceneObjectsUpdate();
			isBlastCutscnPlaying = false;
			cv_BlastCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->ShowBannerText("L3blast");

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P1_WallExplode])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P1_WallExplode] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L3P1_WallExplode;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}
		} 
		return;
	}

	CPoint pos = Control::Input->Mpos();

	if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		SparkleUpdate();

	if( ActivePopupID == ePopInactive )
	{
		#ifdef _CEBUILD
		if(!Hud->isMouseOverExtraButton)
			Cursor::SetMode(CA_Normal);
		#else
			Cursor::SetMode(CA_Normal);
		#endif 
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
		}
		#endif
		for (int i = 0; i < ePopL3S2_Max; i++)
		{
			if(i!=4)
			{
				if(PopStates[i] != ePopupState_Inactive) //if popup is active
				{
					if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos))
					{
						Cursor::SetMode(CA_Look);
						if( Control::Input->LBclicked() )
						{
						
								Cursor::SetMode(CA_Normal);
								ActivePopupID = i;
						
								PopupArray[i]->OpenPopup();
								if(i == ePop1_Thread)
								{
									Hud->InvBox_StartPuzzleMode();
								}
								else if(i == ePop7_MazeDoor && !ProfileSystem::Instance->ActiveProfile.L3_S2_BrickTaken)
									Hud->ShowBannerText("L3blow");
							
						}
						return;
					}
				}
			}
		}


		 if( ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken && SceneRectMaze.Intersection(&pos) )
		{	
			if( !ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPuzzleComplete)
			{

#ifdef TOUCH_UI
			//	if(!GFHud->InvBoxCtrl->navTapped)
			//		Cursor::SetMode(CA_ExitRight,eArea_L3_S2_P1Room1Start);
#else
				Cursor::SetMode(CA_ExitRight,eArea_L3_S2_P1Room1Start);
#endif

				if( Control::Input->LBclicked() )
				{
#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						if (ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom == eArea_Nil) // reached last room
						{
							GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room6Pillar, 1, SceneRectMaze);
						}
						else
						{
							GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room1Start, 1, SceneRectMaze);
						}

					}
#else
					if (ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom == eArea_Nil) // reached last room
					{
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room6Pillar;
					}
					else
					{
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room1Start;
					}

					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				}
			}
			else
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked() )
				{
					Hud->ShowBannerText("L3think");
				}
			}
		}
		 
		else if( IsMouseOverGoBackArea(&pos))
		{

#ifdef TOUCH_UI
		//	 if(!GFHud->InvBoxCtrl->navTapped)
		//		 Cursor::SetMode(CA_ExitDown,eArea_L3_S1_FortEntrance);
#else
			 Cursor::SetMode(CA_ExitDown,eArea_L3_S1_FortEntrance);
#endif

			 if( Control::Input->LBclicked() )
			 {
#ifdef TOUCH_UI
				 if(!GFHud->InvBoxCtrl->navTapped)
				 {
					 GFHud->InvBoxCtrl->GoToScene(eArea_L3_S1_FortEntrance, 2);
				 }
#else
				 //Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				

				
			}	
		}
	}
	
}

L3_S2_FortInterior::~L3_S2_FortInterior()
{
	TaskPendingUpdate();

	SAFE_DELETE(moonprtFX);
	SAFE_DELETE(S2crystalprt1FX);
	SAFE_DELETE(S2crystalprt2FX);
	SAFE_DELETE(S2crystalprt3FX);

	for (int i = 0; i < ePopL3S2_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	SAFE_DELETE(cv_BlastCutscn);
	
		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s1FortEntrance);
	Control::Audio->UnloadSample(aAmbL3s1FortEntrance);
	//MUSIC_SFX
}

void L3_S2_FortInterior::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();
		
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
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				//UpdatePopups(dropPos,true);
				if(ActivePopupID == ePopInactive)
				{
					//If No popups responded
					GFInstance->WrongInvDropMsg(Hud);
				}
				return;
			}
			else
			{
				if(PopupArray[ActivePopupID]!=NULL)
					PopupArray[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal);
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L3_S2_FortInterior::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S2Collected)
		{
			InitHintTrailEmit(800, 310);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(615, 235);
			else
				InitHintTrailEmit(430, 500);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 6;
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
					for (int i = 0; i < ePopL3S2_Max; i++)
					{
						if(PopStates[i] != ePopupState_Inactive) //if popup is active
						{
							if(PopupArray[i]->isTaskLeft())
							{
								InitHintTrailEmit(&PopPolyArray[i]->center,false);

								return;
							}
						}
					}
					break;
			case 2: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance] && ProfileSystem::Instance->ActiveProfile.L3_S2_BackDoorOpened)
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if((ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S2_P1Room1Start]) && ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken)
					{
						InitHintTrailEmit(&SceneRectMaze,true,CA_ExitRight);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
			case 5: ++iNonHOHint;
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
void L3_S2_FortInterior::NavigateToScene()
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

