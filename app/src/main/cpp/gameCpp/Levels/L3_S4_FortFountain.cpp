//====================================
// DQFULP
// Hidden Object System
// Author : Faisal
// LEVEL3 - SCENE3 : FortFountain
//====================================

#include "L3_S4_FortFountain.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "SDrop.h"
#include "Util.h"

#include "L3S4_Pop1BambooDoor.h"
#include "L3S4_Pop2MapWallOne.h"
#include "L3S4_Pop2MapWallTwo.h"
#include "L3S4_Pop2MapWallThree.h"
#include "L3S4_Pop2MapWallFour.h"
#include "L3S4_Pop3CliffDoor.h"
#include "L3S4_Pop4Metalsheet.h"

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);

const CRectangle  popAreaExtraRect(273.0f,79.0f,825.0f,529.0f);

//Scene Rects
const CRectangle  SceneRectCliffSide(345.0f,29.0f,220.0f,250.0f);
const CRectangle  SceneRectHOFountain(657.0f,399.0f,195.0f,157.0f);

const CRectangle  FlagRect1(247.0f,1.0f,96.0f,280.0f);
const CRectangle  FlagRect2(613.0f,0.0f,80.0f,297.0f);
const CRectangle  FlagRect3(1053.0f,0.0f,85.0f,406.0f);

L3_S4_FortFountain::L3_S4_FortFountain()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S4\\S4.SCN");
	musicFadeOutTimer = 0;
	HOScene->GetObjectByID("blueglow1")->SetScale(1.60f);
	HOScene->GetObjectByID("blueglow3")->SetScale(2.10f);
	HOScene->GetObjectByID("fireanim1")->SetRotate(-15.0f);
	HOScene->GetObjectByID("fireanim3")->SetRotate(-15.0f);
	HOScene->GetObjectByID("fireanim2")->SetScale(0.60f);

	HOScene->GetObjectByID("blueglow1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow1")->Animate(0.15f,0.85f, 4.0f);

	HOScene->GetObjectByID("blueglow2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow2")->Animate(0.15f,0.95f, 3.0f);

	HOScene->GetObjectByID("blueglow2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow2")->Animate(0.15f,0.95f, 4.0f, 1.85f);

	HOScene->GetObjectByID("yellowglow1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("yellowglow1")->Animate(0.15f,0.75f, 2.5f);

	HOScene->GetObjectByID("fireglow1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("fireglow1")->Animate(0.15f,0.75f, 2.5f);

	HOScene->GetObjectByID("yellowglow2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("yellowglow2")->Animate(0.15f,0.75f, 3.0f);

	HOScene->GetObjectByID("fireglow2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("fireglow2")->Animate(0.15f,0.75f, 3.0f);

	HOScene->GetObjectByID("yellowglow3")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("yellowglow3")->Animate(0.15f,0.75f, 2.6f);

	HOScene->GetObjectByID("fireglow3")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("fireglow3")->Animate(0.15f,0.75f, 2.6f);

	HOScene->GetObjectByID("moonglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("moonglow")->Animate(0.15f,0.45f, 3.0f);

	SuperFX *s4smokefgFX, *s4smokebgFX, *s4fireprtleftFX, *s4fireprtrightFX, *s4crystal1FX, *s4crystal2FX;

	s4smokefgFX = new SuperFX("PRT\\L3\\s4smokefg.INI", PointSystem::CreateCPoint(711,647,0), 100); 
	s4smokefgFX->Start();
	s4smokefgFX->AdvanceByTime(2.0f);

	s4smokebgFX = new SuperFX("PRT\\L3\\s4smokebg.INI", PointSystem::CreateCPoint(960,192,0), 100); 
	s4smokebgFX->Start();
	s4smokebgFX->AdvanceByTime(2.0f);

	s4fireprtleftFX = new SuperFX("PRT\\L3\\s4fireprtleft.INI", PointSystem::CreateCPoint(211,249,0), 100); 
	s4fireprtleftFX->Start();
	s4fireprtleftFX->AdvanceByTime(2.0f);

	s4fireprtrightFX = new SuperFX("PRT\\L3\\s4fireprtright.INI", PointSystem::CreateCPoint(1168,281,0), 100); 
	s4fireprtrightFX->Start();
	s4fireprtrightFX->AdvanceByTime(2.0f);

	s4crystal1FX = new SuperFX("PRT\\L3\\s4crystal1.INI", PointSystem::CreateCPoint(420,548,0), 100); 
	s4crystal1FX->Start();
	s4crystal1FX->AdvanceByTime(2.0f);

	s4crystal2FX = new SuperFX("PRT\\L3\\s4crystal2.INI", PointSystem::CreateCPoint(1114,543,0), 100); 
	s4crystal2FX->Start();
	s4crystal2FX->AdvanceByTime(2.0f);
	
	VFXArray.push_back(s4smokefgFX);
	VFXArray.push_back(s4smokebgFX);
	VFXArray.push_back(s4fireprtleftFX);
	VFXArray.push_back(s4fireprtrightFX);
	VFXArray.push_back(s4crystal1FX);
	VFXArray.push_back(s4crystal2FX);

	hodotsFX = NULL;
	holinesFX = NULL;

	if( !ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted )
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			//HO Particles
			hodotsFX = new SuperFX("PRT\\L3\\hodots.INI", PointSystem::CreateCPoint(765,514,0), 100); 
			hodotsFX->Start();
			hodotsFX->AdvanceByTime(2.0f);

			holinesFX = new SuperFX("PRT\\L3\\holines.INI", PointSystem::CreateCPoint(777,506,0), 100); 
			holinesFX->Start();
			holinesFX->AdvanceByTime(2.0f);
		}
	}

	meshCloth1 = NULL;
	meshCloth1 = new MeshObject(HOScene->GetObjectByID("cloth1"), 5, 5);
	meshCloth1->meshFX->SetSpeedAndDisplacement(2, 1, 5, 2);
	meshCloth1->meshFX->SetMeshBox(0,1, 4, 4);

	meshCloth2 = NULL;
	meshCloth2 = new MeshObject(HOScene->GetObjectByID("cloth2"), 5, 5);
	meshCloth2->meshFX->SetSpeedAndDisplacement(1, 1, 5, 2);
	meshCloth2->meshFX->SetMeshBox(0,1, 4, 4);

	meshCloth3 = NULL;
	meshCloth3 = new MeshObject(HOScene->GetObjectByID("cloth3"), 5, 5);
	meshCloth3->meshFX->SetSpeedAndDisplacement(2, 1, 5, 2);
	meshCloth3->meshFX->SetMeshBox(0,1, 4, 4);

	HOScene->PushObject(meshCloth1);
	HOScene->PushObject(meshCloth2);
	HOScene->PushObject(meshCloth3);

	InitPopups();

	SceneObjectsUpdate();

	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s4Fountain,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s4Fountain) )		
	{
		Control::Audio->PlaySample(aAmbL3s4Fountain,true);
	}
	
	//MUSIC_SFX

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsVisitedOnce)
	{
		ProfileSystem::Instance->ActiveProfile.L3_S4_IsVisitedOnce = true;
		Hud->ShowBannerText("L3destructio");
	}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L3_S4_FortFountain]  = true;

	//Morph Object
#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S4Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(798.0f,130.0f,64.0f,42.0f);		
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 
}

void L3_S4_FortFountain::InitPopups()
{	
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL3S4_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	popMapWall1 = NULL;
	popMapWall2 = NULL;
	popMapWall3 = NULL;
	popMapWall4 = NULL;

	int popID=0;

	//Popup 1
	popID = ePop1_BambooDoor;
#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken || !ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue])
#else
	if(!(ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken && ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken))
#endif 
	{
		int pop1Pts[] = {730,200,772,194,793,216,798,281,731,298};
		CPolygon* pop1ClickPoly=  new CPolygon(5);
		pop1ClickPoly->Init(pop1Pts);

		Popup* Pop1Scene = new L3S4_Pop1BambooDoor(popID, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L3_S4_FortFountain::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop1ClickPoly;
		PopupArray[popID]	= Pop1Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2 wall 1
	popID = ePop2_1MapWall;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken)
	{
		int pop2Pts[] = {855,367,883,325,994,307,1049,341,1050,458,1024,485,862,483};
		CPolygon* pop2ClickPoly=  new CPolygon(7);
		pop2ClickPoly->Init(pop2Pts);

		L3S4_Pop2MapWallOne* Pop2Scene = new L3S4_Pop2MapWallOne(popID, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L3_S4_FortFountain::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(popAreaExtraRect);

		popMapWall1 = Pop2Scene;
		popMapWall1->setLoadNextCallback(CC_CALLBACK_2(L3_S4_FortFountain::LoadNextPopup, this));

		PopPolyArray[popID] = pop2ClickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2 wall 2
	popID = ePop2_2MapWall;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken)
	{
		int pop2Pts[] = {855,367,883,325,994,307,1049,341,1050,458,1024,485,862,483};
		CPolygon* pop2ClickPoly=  new CPolygon(7);
		pop2ClickPoly->Init(pop2Pts);

		L3S4_Pop2MapWallTwo* Pop2Scene = new L3S4_Pop2MapWallTwo(popID, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L3_S4_FortFountain::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(popAreaExtraRect);

		popMapWall2 = Pop2Scene;
		popMapWall2->setLoadNextCallback(CC_CALLBACK_2(L3_S4_FortFountain::LoadNextPopup, this));

		PopPolyArray[popID] = pop2ClickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2 wall 3
	popID = ePop2_3MapWall;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken)
	{
		int pop2Pts[] = {855,367,883,325,994,307,1049,341,1050,458,1024,485,862,483};
		CPolygon* pop2ClickPoly=  new CPolygon(7);
		pop2ClickPoly->Init(pop2Pts);

		L3S4_Pop2MapWallThree* Pop2Scene = new L3S4_Pop2MapWallThree(popID, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L3_S4_FortFountain::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(popAreaExtraRect);

		popMapWall3 = Pop2Scene;
		popMapWall3->setLoadNextCallback(CC_CALLBACK_2(L3_S4_FortFountain::LoadNextPopup, this));

		PopPolyArray[popID] = pop2ClickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2 wall 4
	popID = ePop2_4MapWall;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken)
	{
		int pop2Pts[] = {855,367,883,325,994,307,1049,341,1050,458,1024,485,862,483};
		CPolygon* pop2ClickPoly=  new CPolygon(7);
		pop2ClickPoly->Init(pop2Pts);

		L3S4_Pop2MapWallFour* Pop2Scene = new L3S4_Pop2MapWallFour(popID, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L3_S4_FortFountain::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(popAreaExtraRect);

		popMapWall4 = Pop2Scene;
		popMapWall4->setLoadNextCallback(CC_CALLBACK_2(L3_S4_FortFountain::LoadNextPopup, this));

		PopPolyArray[popID] = pop2ClickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 3
	popID = ePop3_CliffDoor;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened)
	{
		int pop3Pts[] = {405,68,449,75,475,127,478,234,370,214,373,83};
		CPolygon* pop3ClickPoly=  new CPolygon(6);
		pop3ClickPoly->Init(pop3Pts);

		Popup* Pop3Scene = new L3S4_Pop3CliffDoor(popID, Hud, HOScene, eZOrder_Popup, pop3ClickPoly->center, CC_CALLBACK_1(L3_S4_FortFountain::OnPopupClosed, this));
		Pop3Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop3ClickPoly;
		PopupArray[popID]	= Pop3Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 4
	popID = ePop4_MetalSheet;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken)
	{
		int pop4Pts[] = {436,420,513,420,555,466,506,523,416,523,384,458};
		CPolygon* pop4ClickPoly=  new CPolygon(6);
		pop4ClickPoly->Init(pop4Pts);

		Pop4Scene = new L3S4_Pop4Metalsheet(popID, Hud, HOScene, eZOrder_Popup, pop4ClickPoly->center, CC_CALLBACK_1(L3_S4_FortFountain::OnPopupClosed, this));
		Pop4Scene->SetPopupBoundingBox(PopCircleRect);

		PopPolyArray[popID] = pop4ClickPoly;
		PopupArray[popID]	= Pop4Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	GFInstance->PostUpdate(1);
}

L3_S4_FortFountain::~L3_S4_FortFountain()
{
	/*
	already added to scene
	SAFE_DELETE(meshCloth1);
	SAFE_DELETE(meshCloth2);
	SAFE_DELETE(meshCloth3);*/

	SAFE_DELETE(hodotsFX);
	SAFE_DELETE(holinesFX);

	int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(VFXArray[i]);
	}
	VFXArray.clear();

	for (int i = 0; i < ePopL3S4_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}
	
	//MUSIC_SFX
	

	Control::Audio->StopSample(aAmbL3s4Fountain);
	Control::Audio->UnloadSample(aAmbL3s4Fountain);
	//MUSIC_SFX
}

void L3_S4_FortFountain::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken)
		if(Pop4Scene->isAmeehaCutscnPlaying)
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

void L3_S4_FortFountain::SparkleUpdate()
{
	if(isSparkleUpdatePending)
	{
		isSparkleUpdatePending = false;
		if(SparkleFX==NULL)
		{
			SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);		
		}
		//Game Logic Goes here
		numSparkles = 0;
		currentSparkleID = 0;
		
		//Check for Popup Tasks
		for (int i = 0; i < ePopL3S4_Max; i++)
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

		if(!ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted)
		{
			sparklePoints[numSparkles].SetXY(SceneRectHOFountain.GetCenterX(), SceneRectHOFountain.GetCenterY());
			numSparkles++;
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

void L3_S4_FortFountain::SceneObjectsUpdate()
{
	if( ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken )
	{
		HOScene->GetObjectByID("bamboostick")->SetEnable(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L3_S4_IsNagaLockPlaced )
	{
		HOScene->GetObjectByID("cliffdoorlockfull")->SetEnable(false);
	}
	else
	{
		if( !ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened )
			HOScene->GetObjectByID("cliffdoorlockfull")->SetEnable(true);
		else
			HOScene->GetObjectByID("cliffdoorlockfull")->SetEnable(false);
	}

	if( ProfileSystem::Instance->ActiveProfile.L3_S4_IsAmeehaCutscenePlayed )
	{
		HOScene->GetObjectByID("diarynote")->SetEnable(false);
	}

	if( ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken )
	{
		HOScene->GetObjectByID("metalsheet")->SetEnable(false);
	}

	if( ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened )
	{
		HOScene->GetObjectByID("cliffdoorclosed")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("cliffdoorclosed")->SetEnable(true);
	}

	if( ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AMapDoorOpened )
	{
		HOScene->GetObjectByID("mapdoorclosed")->SetEnable(false);
		HOScene->GetObjectByID("mapdooropen")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("mapdoorclosed")->SetEnable(true);
		HOScene->GetObjectByID("mapdooropen")->SetEnable(false);
	}

	TaskPendingUpdate();
}

void L3_S4_FortFountain::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL3S4_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S4_FortFountain] = isAnyTaskLeft;

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S5_Cliff]
		&& !ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S5_Cliff] = true;

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S2_P1Room1Start]
		&& !ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = true;
}

void L3_S4_FortFountain::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;

	isSparkleUpdatePending = true;

	int popID = popup->GetPopupId();
	if(popID == ePop1_BambooDoor)
	{
	#ifdef _CEBUILD
		if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken && ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue])
	#else
		if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken && ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken)
	#endif 
		{
			//disable popup
			PopStates[popID] = ePopupState_Inactive;
		}
	}
	else if(popID == ePop2_1MapWall)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken)
		{
			//disable popup
			PopStates[popID] = ePopupState_Inactive;

			PopStates[ePop2_2MapWall] = ePopupState_Inactive;
			PopStates[ePop2_3MapWall] = ePopupState_Inactive;
			PopStates[ePop2_4MapWall] = ePopupState_Inactive;
		}
	}
	else if(popID == ePop3_CliffDoor)
	{	
		if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened)
		{
			//disable popups
			PopStates[popID] = ePopupState_Inactive;
		}
	}
	else if(popID == ePop4_MetalSheet)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken)
		{
			//disable popups
			PopStates[popID] = ePopupState_Inactive;
		}
	}

	SceneObjectsUpdate();
}

void L3_S4_FortFountain::LoadNextPopup(Popup* popup, bool loadPrevious)
{
	if(popup->GetPopupId() == ePop2_1MapWall && !loadPrevious)
	{
		popMapWall2->StartSceneSlideIn(false);
		ActivePopupID = ePop2_2MapWall;
	}
	else if(popup->GetPopupId() == ePop2_2MapWall)
	{
		if(loadPrevious)
		{
			popMapWall1->StartSceneSlideIn(true);
			ActivePopupID = ePop2_1MapWall;
		}
		else
		{
			popMapWall3->StartSceneSlideIn(false);
			ActivePopupID = ePop2_3MapWall;
		}
	}
	else if(popup->GetPopupId() == ePop2_3MapWall)
	{
		if(loadPrevious)
		{
			ActivePopupID = ePop2_2MapWall;
			popMapWall2->StartSceneSlideIn(true);
		}
		else
		{
			ActivePopupID = ePop2_4MapWall;
			popMapWall4->StartSceneSlideIn(false);
		}
	}
	else if(popup->GetPopupId() == ePop2_4MapWall && loadPrevious)
	{
		ActivePopupID = ePop2_3MapWall;
		popMapWall3->StartSceneSlideIn(true);
	}
}

void L3_S4_FortFountain::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	CPoint pos = Control::Input->Mpos();

	if( ActivePopupID == ePopInactive)
	{
		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		for (int i = 0; i < ePopL3S4_Max; i++)
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
					}
					return;
				}
			}
		}

		

		if( FlagRect1.Intersection(&pos) || FlagRect2.Intersection(&pos) || FlagRect3.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L3snake");
				return;
			}
		}

		if( !ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted && SceneRectHOFountain.Intersection(&pos) )
		{

#ifdef TOUCH_UI
		//	if(!GFHud->InvBoxCtrl->navTapped)
		//		Cursor::SetMode(CA_Look,eArea_L3_S4_FortFountainHO);
#else
			Cursor::SetMode(CA_Look,eArea_L3_S4_FortFountainHO);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L3_S4_FortFountainHO, 0, SceneRectHOFountain);
				}
#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S4_FortFountainHO;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}

		if( ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened && SceneRectCliffSide.Intersection(&pos) )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitUp,eArea_L3_S5_Cliff);
#else
			Cursor::SetMode(CA_ExitUp,eArea_L3_S5_Cliff);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L3_S5_Cliff, 0, SceneRectCliffSide);
				}
#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S5_Cliff;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_Pool;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
		#endif	
		if( IsMouseOverGoBackArea(&pos) )
		{

#ifdef TOUCH_UI
		//	if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitDown,eArea_L3_S3_Pool);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L3_S3_Pool);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L3_S3_Pool, 2);
				}
#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_Pool;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}	
		}
	}
}

void L3_S4_FortFountain::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
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
			//If no drop items in this scene !!
			//GFInstance->WrongInvDropMsg(Hud);
			//else use below
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

void L3_S4_FortFountain::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S4Collected)
		{
			InitHintTrailEmit(835, 150);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(950, 520);
			else
				InitHintTrailEmit(765, 240);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 7;
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
					for (int i = 0; i < ePopL3S4_Max; i++)
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
					if( !ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted )
					{
						InitHintTrailEmit(&SceneRectHOFountain);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S5_Cliff])
					{
						InitHintTrailEmit(&SceneRectCliffSide,true,CA_ExitUp);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S3_Pool])
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;
			case 5: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
					break;
			case 6: ++iNonHOHint;
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
void L3_S4_FortFountain::NavigateToScene()
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