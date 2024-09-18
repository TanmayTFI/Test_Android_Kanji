//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 07 : Boatdock
//====================================

#include "L1_S7_Boatdock.h"
#include "Hud.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "AwardManager.h"
#include "GameFlow.h"

// pop up close button, pos
const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,98,0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);


//Scene Rects
const CRectangle  SceneRectBoatShed(239.0f,180.0f,121.0f,178.0f);

//Popup Rects
const CRectangle  Pop1RectDieselTank(770.0f,359.0f,132.0f,103.0f);
const CRectangle  Pop2RectKeyStart(625.0f,273.0f,130.0f,86.0f);


//Inventory Item Rects
const CRectangle  InvDestRectRopeCut(734.0f,381.0f,272.0f,188.0f);

const CRectangle  InvDestRectPlanks(226.0f,261.0f,323.0f,321.0f);

const CRectangle  InvItemRectPlankScn(829.0f,464.0f,124.0f,162.0f);

const CRectangle  pop1RectDieselTankCap(602.0f,217.0f,128.0f,97.0f);
const CRectangle  pop1RectFunnelandCanArea(553.0f,132.0f,251.0f,237.0f);

const CRectangle  pop2RectkeyFixArea(785.0f,347.0f,141.0f,143.0f);

//Action Rects

//Info Rects
const CRectangle  InfoRectOwl(1010.0f,66.0f,62.0f,86.0f);
const CRectangle  InfoRectMountains(435.0f,2.0f,624.0f,158.0f);
const CRectangle  InfoRectTunnelDoor(1057.0f,405.0f,115.0f,135.0f);



const CRectangle  ActionRectRopeClick(428.0f,247.0f,380.0f,250.0f);

const CRectangle VidRectBg(0,0,1366,768);
const CPoint seaPos(808,321,0);

const CRectangle PopVidRectBg(349,99,652,484);

const CPoint p1DieselFillAnimPos(808,321,0);
const CPoint p1TankCloseAnimPos(808,321,0);
const CPoint p1TankOpenAnimPos(808,321,0);


L1_S7_Boatdock::L1_S7_Boatdock()
{
	Visible = false;

	RT_BgScn = NULL;

	LightVFX = NULL;

	Replay_WaitingForCollectablePickTicks = 1.0f;
	IsReplay_WaitingForCollectablePick = true;

	bgScaleOffsetX = 0.0f;
	bgScaleOffsetY = 0.0f;
	
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S7\\S7.SCN");
	popupCloseDelayTimer = 0.0f;

	flyingInsects = new FlyingInsects();
	flyingInsects->InitFlies(850,158,956,237);
	
	InitPopups();

	RainBgVFX = new SuperFX("PRT\\BG\\rainfuse.INI", PointSystem::CreateCPoint(580.0f,-50.0f,0), 100);
	RainBgVFX->Start();
	RainBgVFX->AdvanceByTime(2.0f);

	RainDropsVFX = new SuperFX("PRT\\L1\\raindropsFY.INI", PointSystem::CreateCPoint(759.0f,581.0f,0), 100);
	RainDropsVFX->Start();
	RainDropsVFX->AdvanceByTime(2.0f);

	RainFallLeftVFX = new SuperFX("PRT\\L1\\s7leftdrops.INI", PointSystem::CreateCPoint(194,109,0), 100); 
	RainFallLeftVFX->Start();

	RainFallLampVFX = new SuperFX("PRT\\L1\\s7lampdrops.INI", PointSystem::CreateCPoint(902,233,0), 100); 
	RainFallLampVFX->Start();

	MapleLeafVFX = new SuperFX("PRT\\L1\\leaf.INI", PointSystem::CreateCPoint(1163,-45,0), 100); 
	MapleLeafVFX->Start();

	L1S7_P1RainVFX = new SuperFX("PRT\\L1\\poprain.INI", PointSystem::CreateCPoint(722,225,0), eZOrder_Popup+60);
	L1S7_P1RaindropsVFX = new SuperFX("PRT\\L1\\pops1drops.INI", PointSystem::CreateCPoint(683,430,0), eZOrder_Popup+50); 

	Control::Audio->LoadSample(aVOL1MaryRefS7_1,AudioVO);

	//Scene Elements	
	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}
	
	boat = HOScene->GetObjectByID("boat");
	rope = HOScene->GetObjectByID("ropeanim");

	boatShadow = HOScene->GetObjectByID("boatshadow");
	boatShadow->InitPos = boatShadow->Pos;
	boatWaveSide = HOScene->GetObjectByID("boatwaveside");	
	boatWaveback = HOScene->GetObjectByID("boatwaveback");

	HOScene->GetObjectByID("lampglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lampglow")->Animate(0.15f,0.75f,6.0f);
		
	tree1 = HOScene->GetObjectByID("tree1");
	tree2 = HOScene->GetObjectByID("tree2");
	tree3 = HOScene->GetObjectByID("tree3");
	plant1 = HOScene->GetObjectByID("plant1");
	plant2 = HOScene->GetObjectByID("plant2");
	plant3 = HOScene->GetObjectByID("plant3");

	CAnim_PlankPlace = NULL;
	isPlankPlaceAnimPlaying = false;

	if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsStepsPlaced)
	{
		CObject* objArr[] = {HOScene->GetObjectByID("stepbottom"), HOScene->GetObjectByID("steptop"), HOScene->GetObjectByID("stepfirst")};
		std::string animArr[] = {"ANIM\\L1\\s7stepbottom.anim", "ANIM\\L1\\s7steptop.anim", "ANIM\\L1\\s7stepfirst.anim"};
		CAnim_PlankPlace = new CustomAnimGroup(3, animArr, objArr);

		HOScene->GetObjectByID("stepbottom")->SetEnable(false);
		HOScene->GetObjectByID("steptop")->SetEnable(false);
		HOScene->GetObjectByID("stepfirst")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken)
		HOScene->GetObjectByID("plankscn")->SetEnable(false);

	//boat->SetZ(50);

	if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite )
	{
		HOScene->GetObjectByID("lightRoom")->SetEnable(false);
	}	

	if(ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut )
		rope->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite )
	{
		HOScene->GetObjectByID("DoorOpen")->SetEnable(false);
		HOScene->GetObjectByID("light_tunnel")->SetEnable(false);
	}
	else
	{
		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O4_EscapeMansion] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O4_EscapeMansion] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O4_EscapeMansion] = true;
			Hud->ShowObjectiveFX(eObj_L1O4_EscapeMansion,true);
		}

		HOScene->GetObjectByID("light_tunnel")->SetBlendMode(eBlend_AlphaAdditive);
		HOScene->GetObjectByID("light_tunnel")->Animate(0.75f, 1.0f, 10.0f);
		
		LightVFX = new SuperFX();
		LightVFX->LoadEffectIni("PRT\\L1\\s7lightprt.INI");
		LightVFX->SetPos(PointSystem::CreateCPoint(1090,487,0));
		LightVFX->SetZ(100);
		LightVFX->Start();
		LightVFX->AdvanceByTime(2.0f);
		VFXArray.push_back(LightVFX);
		if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsEscapedFromTunnelBanner)
		{
			Hud->ShowBannerText("L1good");
			Control::Audio->PlaySample(aVOL1MaryRefS7_1);
			ProfileSystem::Instance->ActiveProfile.L1S5_IsEscapedFromTunnelBanner = true;
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed)
		{
			//check if funnel placed
			if(ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && !ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelRemoved)
				boat->PlayAnimation(2);
			else
				boat->PlayAnimation(1);
		}
	}	

	seaMesh = NULL;

	// adjust sea animation later..
	seaMesh = new MeshObject("LV1\\S7\\sea.jpa", 16, 16);
	seaMesh->SetPosAndParams(462, 148, 2, 1, 3, 4);
	seaMesh->meshFX->SetOffset(462,148);
	seaMesh->meshFX->isTaperedDown = false;
	seaMesh->SetZ(1);
	
	HOScene->InsertObject(seaMesh,1);

	tree1->SetPivot(-176,-46);
	tree1->Animate(0,0,3,0,8);

	tree2->SetPivot(-61,-80);
	tree2->Animate(0,0,3,0,4);

	tree3->SetPivot(62,56);
	tree3->Animate(0,0,3,0,8);

	plant1->SetPivot(25,80);
	plant1->Animate(0,0,3,0,8);

	plant2->SetPivot(-36,88);
	plant2->Animate(0,0,3,0,8);

	plant3->SetPivot(20,36);
	plant3->Animate(0,0,3,0,12);

	boat->InitPos = boat->Pos;
	boatAngle = 0;

	boatWaveSide->SetScale(1.0f+0.25f*Elixir::Sin(boatAngle));
	boatWaveSide->SetAlpha(1.0f*Elixir::Sin(boatAngle));

	boatWaveback->SetScale(1.0f+0.25f*Elixir::Sin(boatAngle));
	boatWaveback->SetAlpha(1.0f*Elixir::Sin(boatAngle));

	boat->Pos.y = boat->InitPos.y + (Elixir::Sin(boatAngle))*2;
	boat->SetRotate((Elixir::Sin(boatAngle))*1);
	boatShadow->Pos.y = boatShadow->InitPos.y + (Elixir::Sin(boatAngle))*1;

	GFInstance->PostUpdate(2);	

	zOrderScnVideo = 1;
	zOrderPopVideo = eZOrder_Popup+1;
	SetZ(zOrderScnVideo);

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	//After Escape From Tunnel
	if(ProfileSystem::Instance->ActiveProfile.IsStartMusic)
	{
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = false;
		if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			if( Control::Audio->IsPlaying(aTrackL1Trapped) )
				Control::Audio->StopSample(aTrackL1Trapped);

			if( !Control::Audio->IsPlaying(aTrackL1EscapePlan) )
				Control::Audio->PlaySample(aTrackL1EscapePlan,false);
		}
		else if( ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed &&
			!ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed )
		{
			if( !Control::Audio->IsPlaying(aTrackL1Hunted) )
				Control::Audio->PlaySample(aTrackL1Hunted,false);
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL1Mansion) )
				Control::Audio->PlaySample(aTrackL1Mansion,false);
		}
	}

	Control::Audio->LoadSample(aAmbS7Boatdock,AudioAmb);
	Control::Audio->PlaySample(aAmbS7Boatdock,true);
	//MUSIC_SFX
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S7_BoatDock]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S7_BoatDock])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S7_BoatDock] = true;
	
	

	
	

	UpdateBgGraphic();

#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S7Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("crow"), HOScene->GetObjectByID("snake"));
		CRectangle  MorphRect(384.0f,135.0f,49.0f,46.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("crow")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("crow")->SetEnable(false);
#endif 

	TaskPendingUpdate();

	if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
	{
		//Journal Entry 15 Escaped the Tunnel
		if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P15_AfterTunnelblast])
		{
			//Flag Set
			ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P15_AfterTunnelblast] = true;
			//Add to Note Array
			ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
			= eJrn_L1P15_AfterTunnelblast;

			ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
			//Increment Notes
			ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			Hud->ShowJrnEntryFX();
		} 
	}

	Visible = true;	//To render anything with renderlistentity
}

void L1_S7_Boatdock::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut && ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked && ProfileSystem::Instance->ActiveProfile.L1S6_HOInvFunnelTaken)
		 isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced && ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced && ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken && ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken && !ProfileSystem::Instance->ActiveProfile.L1S7_IsStepsPlaced)
		isAnyTaskLeft = true;			
	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S7_BoatDock] = isAnyTaskLeft;

	if(ProfileSystem::Instance->ActiveProfile.L1S7_IsStepsPlaced && !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S6_BoatShed]) 
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S6_BoatShed] = true;
}

void L1_S7_Boatdock::UpdateBoatKeyPopupItems()
{
	if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced)
	{
		p2KeyTurned->SetEnable(false);
		p2KeyFixed->SetEnable(false);
	}
	else
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted)
		{
			p2KeyTurned->SetEnable(true);
			p2KeyFixed->SetEnable(false);
		}
	}
}

void L1_S7_Boatdock::UpdateDieselTankPopupItems()
{
	if(ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelRemoved)
		{
			p1FunnelFixed->SetEnable(false);

			if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed)
			{
				p1TankCap->SetEnable(false);
			}
			else
			{
				p1TankCapOpend->SetEnable(false);
			}
		}
		else
		{
			p1TankCap->SetEnable(false);
		}
	}
	else
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced)
		{
			p1TankCap->SetEnable(false);
		}
		else
		{
			p1FunnelFixed->SetEnable(false);
			if(ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen)
			{
				p1TankCap->SetEnable(false);
			}
			else
			{
				p1TankCapOpend->SetEnable(false);
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut)
	{
		p1Rope->SetEnable(false);
	}
	else
	{
		p1CuttedRope->SetEnable(false);
	}
}

void L1_S7_Boatdock::InitPopups()
{
	ActivePopupID = ePopInactive;

	Pop_CloseButton = NULL;

	PopVal = 0.0f;
	PopupAlpha = 0;

	IsPopupTransitionActive = false;

	isDieselFillAnimPlaying = false;
	isTankOpenAnimPlaying = false;
	isTankCloseAnimPlaying = false;
	isKeyTurnAnimPlaying = false;

	p2KeyTurnAnimTimer = 0;

	isRopeKnifeCutAnimPlaying = false;

	Popup1_State = POPUP_CLOSED;

	Popup1DieselTank = NULL;

	CAnim_RopeKnifeCut = NULL;

	//for popup3 swing rain particle
	L1S7_P1RainVFX = NULL;
	L1S7_P1RaindropsVFX = NULL;

	if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled || !ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut)
	{
		Popup1DieselTank = new CHOScene();
		Popup1DieselTank->Load("LV1\\S7\\P1.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Popup1DieselTank->PushObject(popFrame); //in Normal Cases

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Popup1DieselTank->PushObject(popCloseBtn);

		p1SwissKnife = Popup1DieselTank->GetObjectByID("p1Knife");
		p1SwissKnife->SetZ(eZOrder_PopFrame+2);
		p1SwissKnife->SetEnable(false);

		Popup1DieselTank->SetPosOff(CenterPoint.x,CenterPoint.y);
		Popup1DieselTank->SetZOff(eZOrder_Popup);

		p1Water = new MeshObject("LV1\\S7\\p1Water.jpa", 8, 8);
		p1Water->SetPosAndParams(541,250, 4, 2, 6, 4);
		//p1Water->meshFX->SetOffset(541,250);
		p1Water->SetZ(eZOrder_Popup + 1);
	
		Popup1DieselTank->InsertObject(p1Water,1);

		Popup1DieselTank->Show(false);

		p1CuttedRope = Popup1DieselTank->GetObjectByID("p1CuttedRope");
		//p1DieselCan = Popup1DieselTank->GetObjectByID("p1DieselCan");
		p1FunnelFixed = Popup1DieselTank->GetObjectByID("p1FunnelFixed");
		p1Rope = Popup1DieselTank->GetObjectByID("p1Rope");
		p1TankCap = Popup1DieselTank->GetObjectByID("p1TankCap");
		p1TankCapOpend = Popup1DieselTank->GetObjectByID("p1TankCapOpend");

		p1TankOpenAnim = Popup1DieselTank->GetObjectByID("p1TankOpenAnim");
		p1TankOpenAnim->SetEnable(false);
		p1TankCloseAnim = Popup1DieselTank->GetObjectByID("p1TankCloseAnim");
		p1TankCloseAnim->SetEnable(false);
		p1DieselFillAnim = Popup1DieselTank->GetObjectByID("p1DieselFillAnim");
		p1DieselFillAnim->SetEnable(false);

		CObject* objArr[] = {p1CuttedRope, p1Rope, p1SwissKnife};
		std::string animArr[] = {"ANIM\\L1\\s7ropecut.anim", "ANIM\\L1\\s7rope.anim", "ANIM\\L1\\s7knife.anim"};
		CAnim_RopeKnifeCut = new CustomAnimGroup(3, animArr, objArr);

		//sree
		//p1DieselCan->SetEnable(false);

		UpdateDieselTankPopupItems();

		
	}

	Popup2_State = POPUP_CLOSED;

	Popup2BoatKeyStart = NULL;

	Popup2BoatKeyStart = new CHOScene();
	Popup2BoatKeyStart->Load("LV1\\S7\\P2.SCN");

	//Add popup frame and close button
	CObject *popFrame = new CObject();
	popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
	popFrame->SetUID("popFrame");
	Popup2BoatKeyStart->PushObject(popFrame); //in Normal Cases

	CObject *popCloseBtn = new CObject();
	popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
	popCloseBtn->SetUID("popClose");
	Popup2BoatKeyStart->PushObject(popCloseBtn);

	p2KeyFixed = Popup2BoatKeyStart->GetObjectByID("p2KeyFixed");
	p2KeyTurned = Popup2BoatKeyStart->GetObjectByID("p2KeyTurned");

	if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled)
		Popup2BoatKeyStart->GetObjectByID("p2fuelfillneedle")->SetEnable(false);
	
	p2FuelRedGlow =	Popup2BoatKeyStart->GetObjectByID("p2fuelredglow");
	p2FuelRedGlow->SetEnable(false);

	p2KeyTurnAnim = Popup2BoatKeyStart->GetObjectByID("p2KeyTurnAnim");
	p2KeyTurnAnim->SetEnable(false);
	p2NeedleAnim = Popup2BoatKeyStart->GetObjectByID("p2NeedleAnim");
	p2NeedleAnim->SetEnable(false);

	UpdateBoatKeyPopupItems();

	Popup2BoatKeyStart->SetPosOff(CenterPoint.x,CenterPoint.y);
	Popup2BoatKeyStart->SetZOff(eZOrder_Popup);

	Popup2BoatKeyStart->Show(false);

	GFInstance->PostUpdate(1);
}

void L1_S7_Boatdock::Required()
{	
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( ActivePopupID == ePopInactive )
	{
		boatAngle += Control::LogicRate*80;
		boat->Pos.y = boat->InitPos.y + (Elixir::Sin(boatAngle))*2;
		boatShadow->Pos.y = boatShadow->InitPos.y + (Elixir::Sin(boatAngle))*1;

		boatWaveSide->SetScale(1.0f+0.25f*Elixir::Sin(boatAngle));
		boatWaveSide->SetAlpha(1.0f*Elixir::Sin(boatAngle));

		boatWaveback->SetScale(1.0f+0.25f*Elixir::Sin(boatAngle));
		boatWaveback->SetAlpha(1.0f*Elixir::Sin(boatAngle));

		boat->SetRotate((Elixir::Sin(boatAngle))*1);
	}

	if( IsPopupTransitionActive )
		UpdatePopupTransition();
}

void L1_S7_Boatdock::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	flyingInsects->Update();

	if(GFInstance->IsSceneGraphicReloaded)
	{
		UpdateBgGraphic();
		GFInstance->IsSceneGraphicReloaded = false;
	}

	CPoint pos = Control::Input->Mpos();
#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	if(isPlankPlaceAnimPlaying)
	{
		if(CAnim_PlankPlace->IsAnimFinished())
		{
			isPlankPlaceAnimPlaying = false;
		}
		return;
	}

	
	if( GFInstance->IsReplayingGame )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted) 
		{
			GFInstance->IsReplayingGame = false;
			return;
		}

		bool isPendingCollectableInLevel = false;
		
		if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S2Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S3Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S7Collected )
		{
			isPendingCollectableInLevel = true;		
			Replay_WaitingForCollectablePickTicks = 1.0f;//till morph objects in scene we initialize this wait timer to 2 sec!!
			IsReplay_WaitingForCollectablePick = true;
			if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle)//avoid looped banner calls!!
				Hud->ShowBannerText("L1mismo");
		}
		else 
		{
			for( int i = 0; i < 5; i++ )				
				if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[(ProfileSystem::Instance->ActiveProfile.CurrentLevel*5)+i] )
				{
					isPendingCollectableInLevel = true;
					Replay_WaitingForCollectablePickTicks = 1.0f;//till mem collectables in scene we initialize this wait timer to 2 sec!!
					IsReplay_WaitingForCollectablePick = true;
					if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle)//avoid looped banner calls!!
							Hud->ShowBannerText("L1mismc");
					break;
				}
		}		

		//No pending collectables!!
		if( !isPendingCollectableInLevel && ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted)
		{
			//wait.. 1 sec regardless of collectable present or not!! 
			if( IsReplay_WaitingForCollectablePick ) 
			{
				if( Replay_WaitingForCollectablePickTicks >= 0.0f )
					Replay_WaitingForCollectablePickTicks -= Control::LogicRate;
				if( Replay_WaitingForCollectablePickTicks <= 0.0f )
				{
					Replay_WaitingForCollectablePickTicks = 0.0f;
					IsReplay_WaitingForCollectablePick = false;
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_EndOfDemo;
					SendMessage(ControlExit, ID, HOSM_NewArea);
					return;
				}
			}
			else 
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_EndOfDemo;
				SendMessage(ControlExit, ID, HOSM_NewArea);
				return;
			}			
			
		}
	}
	else if( ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted )
	{
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_EndOfDemo;
		SendMessage(ControlExit, ID, HOSM_NewArea);
		return;
	}
		

	if( ActivePopupID == ePopInactive )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken && InvItemRectPlankScn.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				HOScene->GetObjectByID("plankscn")->SetEnable(false);

				std::vector<int> vecInvIds;
				Hud->InvBoxCtrl->Activate(1);			
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S7_Plank2] = true;		
				ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken = true;
				vecInvIds.push_back(eInv_L1S7_Plank2);
				Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
				TaskPendingUpdate();
			}
			return;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsStepsPlaced)
		{
			if(InvDestRectPlanks.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L1jump");
				}
			}
		}
		else
		{
			if( SceneRectBoatShed.Intersection(&pos) )
			{

#ifdef TOUCH_UI
				//if(!GFHud->InvBoxCtrl->navTapped)
				//	Cursor::SetMode(CA_ExitLeft,eArea_L1_S6_BoatShed);
#else
				Cursor::SetMode(CA_ExitLeft,eArea_L1_S6_BoatShed);
#endif

				if( Control::Input->LBclicked() )
				{
#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						GFHud->InvBoxCtrl->GoToScene(eArea_L1_S6_BoatShed, 3, SceneRectBoatShed);
					}
#else
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_BoatShed;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				}
				return;
			}
		}

#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
		{
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
				SendMessage(ControlExit, ID, HOSM_NewArea);								
			return;
		}
#endif
		if(IsMouseOverGoBackArea(&pos))
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S2_Frontyard);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S2_Frontyard);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S2_Frontyard, 2);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}			
			return;
		}

		if( InfoRectOwl.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1she");
				if( !Control::Audio->IsPlaying(aSFXL1S2MonkeyClicked) )
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2MonkeyClicked);
			}
		}
		else if( InfoRectMountains.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1narara");
				if( !Control::Audio->IsPlaying(aSFXJrnlFlyer) )				
					Control::Audio->QuickLoadAndPlaySFX(aSFXJrnlFlyer);
			}
		}

		if( InfoRectTunnelDoor.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			Cursor::SetMode(CA_Help);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1way");
			}
		}
	}

	if( !IsPopupTransitionActive )
		UpdatePopups(pos);
}

float L1_S7_Boatdock::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void L1_S7_Boatdock::EndBgGraphicScaling()
{
	RT_BgScn->SetScale(1.0f);
	RT_BgScn->SetEnable(false);

	HOScene->Show(true);
	HOScene->PauseObjects(false);
}

void L1_S7_Boatdock::UpdateBgGraphic()
{
	if(RT_BgScn == NULL)
	{
		RT_BgScn = new CObject();
		RT_BgScn->LoadSprite("UI\\ZOOMBG.SPR");
		RT_BgScn->SetZ(20);
		RT_BgScn->SetEnable(false);
	}
	
    if(GFInstance->IsScnRTCreated)
    {
        GFInstance->CurrentSceneGraphic->clearRenderTarget();
        GFInstance->CurrentSceneGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS
        GFInstance->IsRenderingToRT = true;
    }
        if(GFInstance->IsScnBg1024Res)
            HOScene->RenderToTarget(-171.0f);
        else
            HOScene->RenderToTarget();
    if(GFInstance->IsScnRTCreated)
    {
        GFInstance->IsRenderingToRT = false;
        GFInstance->CurrentSceneGraphic->endRenderToTarget();
    }
	RT_BgScn->pSprite->Image = GFInstance->CurrentSceneGraphic;
}

void L1_S7_Boatdock::UpdatePopupTransition()
{
	if(!GFInstance->IsScnBg1024Res)
	{
		//1366 resolution
		RT_BgScn->SetScale(1.0f + PopupAlpha * 0.25f);
		RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));
	}
	else
	{
		//1024 resolution
		RT_BgScn->SetScale(1.0f + PopupAlpha * 0.25f);
		RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) +171.0f ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));
	}

	if ( ActivePopupID == ePopL1S7_P1DieselTank)
	{
		switch( Popup1_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				//for pop transition purposes
				//p1Water->meshFX->SetOffset(541,250);

				PopupAlpha = 1.0f;				
				Popup1DieselTank->Show(true);
				Popup1_State = POPUP_OPEN;

				
			}			
			GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		case POPUP_SCALEDOWN:
			popupCloseDelayTimer -= 1.0f * Control::LogicRate;
			if( popupCloseDelayTimer > 0.0f )
			{
				return;
			}
			else
			{
				popupCloseDelayTimer = 0.0;
			}
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;								
				GFInstance->TurnOffBlurScene();
				
				EndBgGraphicScaling();

				Popup1_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;

				SetZ(zOrderScnVideo);
			}		
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;

		} 
	}
	else if( ActivePopupID==ePopL1S7_P2BoatKeyStart)
	{
		switch( Popup2_State )
		{
			case POPUP_SCALEUP:
				PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
				PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

				if( PopVal >= 1.0f )
				{
					IsPopupTransitionActive = false;

					PopupAlpha = 1.0f;				
					Popup2BoatKeyStart->Show(true);
					Popup2_State = POPUP_OPEN;
					L1S7_P1RainVFX->Start();
					L1S7_P1RainVFX->AdvanceByTime(2.0f);
					L1S7_P1RaindropsVFX->Start();
					L1S7_P1RaindropsVFX->AdvanceByTime(2.0f);
				}
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;
			case POPUP_SCALEDOWN:

				PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
				if( PopupAlpha <= 0.0f )
				{
					IsPopupTransitionActive = false;
					
					PopupAlpha = 0.0f;								
					GFInstance->TurnOffBlurScene();

					EndBgGraphicScaling();

					Popup2_State = POPUP_CLOSED;
					ActivePopupID = ePopInactive;

					SetZ(zOrderScnVideo);
				}		
				else
					GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		}
	}
}

void L1_S7_Boatdock::UpdatePopups(CPoint pos,bool ItemDropped)
{
	if ((ActivePopupID == ePopInactive || ActivePopupID == ePopL1S7_P1DieselTank) )
	{
		switch( Popup1_State )
		{
		case POPUP_CLOSED:
			if( Pop1RectDieselTank.Intersection(&pos) && (!ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled || !ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed || !ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut))
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped)
				{
					ActivePopupID = ePopL1S7_P1DieselTank;
					Popup1_State = POPUP_SCALEUP;

					UpdateBgGraphic();

					Pop_CloseButton = Popup1DieselTank->GetObjectByID("popClose");
					Pop_CloseButton->PlayAnimation(0);

					//Control::Audio->PlaySample(aSFXpopup);
					//Popup1DieselTank->Show(true);

					//for pop transition purposes
					//p1Water->meshFX->SetOffset(541-171,250);

					PopVal = 0;
					PopupAlpha = 0.0f;
					SetupPopupTransition(Popup1DieselTank, Pop1RectDieselTank);
				}
				return;
			}
			break;

		case POPUP_OPEN:
			if(ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut && !isRopeKnifeCutAnimPlaying && !isTankCloseAnimPlaying)
			{
				Popup1_State = POPUP_SCALEDOWN;
				Control::Audio->PlaySample(aSFXpopdown);
				Popup1DieselTank->Show(false);

				//for pop transition purposes
				//p1Water->meshFX->SetOffset(541-171,250);

				SetupPopupTransition(Popup1DieselTank, Pop1RectDieselTank);
				return;
			}
			if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
			{				
				Pop_CloseButton->PlayAnimation(1);
				if(PopCloseButtonRect.Intersection(&pos))
					Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked() )
				{
					Popup1_State = POPUP_SCALEDOWN;
					Control::Audio->PlaySample(aSFXpopdown);

					Popup1DieselTank->Show(false);

					//for pop transition purposes
					//p1Water->meshFX->SetOffset(541-171,250);

					SetupPopupTransition(Popup1DieselTank, Pop1RectDieselTank);
				}
				return;
			}
			else
			{
				Pop_CloseButton->PlayAnimation(0);

				if(isRopeKnifeCutAnimPlaying)
				{
					if(CAnim_RopeKnifeCut->IsAnimFinished())
					{
						isRopeKnifeCutAnimPlaying = false;
					}
					return;
				}

				if(isDieselFillAnimPlaying)
				{
					if(p1DieselFillAnim->IsAnimationFinished())
					{
						isDieselFillAnimPlaying = false;
						p1DieselFillAnim->SetEnable(false);
						ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled = true;

						Popup2BoatKeyStart->GetObjectByID("p2fuelfillneedle")->SetEnable(true);
					}
					return;
				}

				if(isTankOpenAnimPlaying)
				{
					if(p1TankOpenAnim->IsAnimationFinished())
					{
						isTankOpenAnimPlaying = false;
						p1TankOpenAnim->SetEnable(false);
						p1TankCapOpend->SetEnable(true);

						boat->PlayAnimation(1);
					}
					return;
				}

				if(isTankCloseAnimPlaying)
				{
					if(p1TankCloseAnim->IsAnimationFinished())
					{
						isTankCloseAnimPlaying = false;
						p1TankCloseAnim->SetEnable(false);
						p1TankCap->SetEnable(true);
					}
					return;
				}

				if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut)
				{
					if(InvDestRectRopeCut.Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked() )
						{
							if(!ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken || !ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed)
								Hud->ShowBannerText("L1until");
							else
								Hud->ShowBannerText("L1fueled");
						}
					}
				}

				if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled)
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen)
					{
						if( pop1RectDieselTankCap.Intersection(&pos) )
						{
							Cursor::SetMode(CA_HandTake);
							if( Control::Input->LBclicked() )
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S7DieselCapOpen);

								p1TankCap->SetEnable(false);
								//p1TankCapOpend->SetEnable(true);
								isTankOpenAnimPlaying = true;
								p1TankOpenAnim->SetEnable(true);
								p1TankOpenAnim->ForcePlayAnimation(0);								
								ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen = true;
								return;
							}
						}
					}
					else
					{
						if(pop1RectFunnelandCanArea.Intersection(&pos))
						{
							Cursor::SetMode(CA_Gear);
							if( Control::Input->LBclicked() )
							{
								Hud->ShowBannerText("L1tank");

								//Objective Add
								if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O5_FindBoatFuel] )
								{
									ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O5_FindBoatFuel] = true;
									ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
									= eObj_L1O5_FindBoatFuel;
									//Increment Objectives
									ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
									Hud->ShowObjectiveFX(eObj_L1O5_FindBoatFuel);

									Control::Audio->PlaySample(aSFXMagic1);
								}
							}
						}
					}
				}
				else
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelRemoved)
					{
						if( pop1RectFunnelandCanArea.Intersection(&pos) )
						{
							Cursor::SetMode(CA_HandPointy);
							if( Control::Input->LBclicked() )
							{
								boat->PlayAnimation(1);

								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S7DieselFunnelRemoved);

								p1FunnelFixed->FadeOut(4.0f);
								ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelRemoved = true;
							}
						}
					}
					else
					{
						if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed)
						{
							if( pop1RectFunnelandCanArea.Intersection(&pos) )
							{
								Cursor::SetMode(CA_HandPointy);
								if( Control::Input->LBclicked() )
								{
									boat->PlayAnimation(0);

									Control::Audio->QuickLoadAndPlaySFX(aSFXL1S7DieselCapClose);

									//p1TankCap->SetEnable(true);
									p1TankCloseAnim->SetEnable(true);
									p1TankCloseAnim->ForcePlayAnimation(0);
									p1TankCapOpend->SetEnable(false);
									isTankCloseAnimPlaying = true;
									ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed = true;
									isSparkleUpdatePending = true;
									return;
								}
							}
						}
					}
				}				
			}
		} 
	}

	if ((ActivePopupID == ePopInactive || ActivePopupID == ePopL1S7_P2BoatKeyStart) && !ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted)
	{
		switch( Popup2_State )
		{
		case POPUP_CLOSED:
			if( Pop2RectKeyStart.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped)
				{
					ActivePopupID = ePopL1S7_P2BoatKeyStart;
					Popup2_State = POPUP_SCALEUP;

					UpdateBgGraphic();

					Pop_CloseButton = Popup2BoatKeyStart->GetObjectByID("popClose");
					Pop_CloseButton->PlayAnimation(0);

					Control::Audio->PlaySample(aSFXpopup);
					//Popup1DieselTank->Show(true);

					PopVal = 0;
					PopupAlpha = 0.0f;
					SetupPopupTransition(Popup2BoatKeyStart, Pop2RectKeyStart);

				}
				return;
			}
			break;

		case POPUP_OPEN:
			if(isKeyTurnAnimPlaying)
			{
				p2KeyTurnAnimTimer += Control::LogicRate;
				if(p2KeyTurnAnimTimer > 3.5f ) //p2NeedleAnim->IsAnimationFinished()
				{
					isKeyTurnAnimPlaying = false;
					//Clearing all the Level 1 Objectives
					ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();

					//Play Cut Scene....
					ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted = true;

					int temp1 = ProfileSystem::Instance->ActiveProfile.CurrentArea;
					int temp2 = ProfileSystem::Instance->ActiveProfile.CurrentLevel;

					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
					ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_2;
					//Backup Save for level restart
					ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);

					ProfileSystem::Instance->ActiveProfile.CurrentArea = temp1;
					ProfileSystem::Instance->ActiveProfile.CurrentLevel = temp2;

				}
				return;
			}

			if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
			{
				
				if(PopCloseButtonRect.Intersection(&pos))
					Cursor::SetMode(CA_HandPointy);
				Pop_CloseButton->PlayAnimation(1);
				if( Control::Input->LBclicked() )
				{
					L1S7_P1RainVFX->StopImmediate();
					L1S7_P1RaindropsVFX->StopImmediate();
					Popup2_State = POPUP_SCALEDOWN;
					Control::Audio->PlaySample(aSFXpopdown);

					Popup2BoatKeyStart->Show(false);
					SetupPopupTransition(Popup2BoatKeyStart, Pop2RectKeyStart);
				}
				return;
			}
			else
			{
				Pop_CloseButton->PlayAnimation(0);

				if(ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced)
				{
					if( pop2RectkeyFixArea.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked() )
						{
							if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled)
							{
								//play key turn error anim
								p2FuelRedGlow->ForcePlayAnimation(0);
								p2FuelRedGlow->SetEnable(true);
								p2KeyTurnAnim->SetEnable(true);
								p2KeyTurnAnim->ForcePlayAnimation(0);
								p2NeedleAnim->SetEnable(true);
								p2NeedleAnim->ForcePlayAnimation(1);
								Hud->ShowBannerText("L1silly");
							}
							else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut)
							{								
								Hud->ShowBannerText("L1funnel");
							}
							else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelRemoved)
							{
								Hud->ShowBannerText("L1cap");
							}
							else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed)
							{								
								Hud->ShowBannerText("L1sea");
							}
							else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted)
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S7BoatEngineStart);

								
								p2KeyFixed->SetEnable(false);
								//p2KeyTurned->SetEnable(true);
								isKeyTurnAnimPlaying = true;
								p2KeyTurnAnim->SetEnable(true);
								p2KeyTurnAnim->ForcePlayAnimation(2);
								p2NeedleAnim->SetEnable(true);
								p2NeedleAnim->ForcePlayAnimation(1);
								AwardManager::GetInstance()->CollectAward(kAward_Start_Journey_Narara);
							}
						}
					}
				}
				else
				{
					if( pop2RectkeyFixArea.Intersection(&pos) )
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked() )
						{
							Hud->ShowBannerText("L1yes");
						}
					}
				}
			}
		} 
	}
}

void L1_S7_Boatdock::Render(const CPoint* pCam)
{  
   static int FrameCnt = 0;
   FrameCnt = (FrameCnt+1)%2;
   
   if(IsPopupTransitionActive)
	{
		//Init again if render target lost due to fullscreen-window switches or similar
		if( GFInstance->RTGraphic != NULL )
			GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
			short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
			short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
			-5*(1.0f-PopupAlpha),			
			PopupAlpha,PopupAlpha);
        
    }
}

L1_S7_Boatdock::~L1_S7_Boatdock()
{
	TaskPendingUpdate();

	int size = (int)VFXArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++){
		SAFE_DELETE(VFXArray[i]);
	}

	RT_BgScn->pSprite->Image = NULL;
	SAFE_DELETE(RT_BgScn);

	SAFE_DELETE(flyingInsects);

	SAFE_DELETE(Popup1DieselTank);
	SAFE_DELETE(Popup2BoatKeyStart);

	SAFE_DELETE(CAnim_RopeKnifeCut);
	SAFE_DELETE(CAnim_PlankPlace);

	SAFE_DELETE(RainBgVFX);
	SAFE_DELETE(RainDropsVFX);
	SAFE_DELETE(RainFallLeftVFX);
	SAFE_DELETE(RainFallLampVFX);
	SAFE_DELETE(MapleLeafVFX);
	SAFE_DELETE(L1S7_P1RainVFX);
	SAFE_DELETE(L1S7_P1RaindropsVFX);

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	//MUSIC_SFX
	Control::Audio->StopSample(aAmbS7Boatdock);
	Control::Audio->UnloadSample(aAmbS7Boatdock);
	Control::Audio->UnloadSample(aVOL1MaryRefS7_1);
	//MUSIC_SFX
}

void L1_S7_Boatdock::ReceiveMessage(int val)
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
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if( itemVal == eInv_L1S2_Plank1 )
			{
				if( ActivePopupID == ePopInactive && InvDestRectPlanks.Intersection(&dropPos) )
				{
					if( (ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken) )
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Plank1] = false;
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S7_Plank2] = false;
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_Plank3] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S2_Plank1);

						ProfileSystem::Instance->ActiveProfile.L1S7_IsStepsPlaced = true;

						CAnim_PlankPlace->PlayAnim();
						isPlankPlaceAnimPlaying = true;
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S7PlankFixed);
						TaskPendingUpdate();
						isSparkleUpdatePending = true;
					}
					else
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 

						int PlankPickCnt = 0;
						if( ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken )
							PlankPickCnt++;
						if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken )
							PlankPickCnt++;
						if( ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken )
							PlankPickCnt++;

						if( PlankPickCnt == 2 )
							Hud->ShowBannerText("L1coll");						
						else
							Hud->ShowBannerText("L1more");						
					}
					return;
				}
				else
				{
					GFInstance->WrongInvDropMsg(Hud);
					return;
				}
			}
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				UpdatePopups(dropPos,true);
				if(ActivePopupID == ePopInactive)
				{
					//If No popups responded
					GFInstance->WrongInvDropMsg(Hud);
				}
				return;
			}
			if( itemVal == eInv_L1S6_Funnel )
			{
				if( Popup1_State == POPUP_OPEN && pop1RectFunnelandCanArea.Intersection(&dropPos) )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen && !ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && !ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled)
					{
						Hud->ShowBannerText("L1hmm");
						Hud->CancelDrag();
					}
					else if( ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen && !ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && !ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled )
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_Funnel] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S6_Funnel);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S7DieselFunnelPlaced);

						ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced = true;
						p1FunnelFixed->SetEnable(true);

						boat->PlayAnimation(2);

						TaskPendingUpdate();
						isSparkleUpdatePending = true;
					}
					return;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S6_CanFull )
			{
				if( Popup1_State == POPUP_OPEN && pop1RectFunnelandCanArea.Intersection(&dropPos) )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen && !ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && !ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled)
					{
						Hud->ShowBannerText("L1hmm");
						Hud->CancelDrag();
					}
					else if( ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen && !ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && !ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled )
					{
						Hud->ShowBannerText("L1pour");
						Hud->CancelDrag();
					}
					else if( ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen && ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && !ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled )
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_CanFull] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S6_CanFull);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S7DieselPoured);

						p1DieselFillAnim->SetEnable(true);
						p1DieselFillAnim->ForcePlayAnimation(0);
						isDieselFillAnimPlaying = true;

						TaskPendingUpdate();
					}
					return;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S2_EmptyCan )
			{
				if( Popup1_State == POPUP_OPEN && pop1RectFunnelandCanArea.Intersection(&dropPos) )
				{
					Hud->ShowBannerText("L1can");
					Hud->CancelDrag();
					return;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S1_SwissKnife )
			{
				if( Popup1_State == POPUP_OPEN && InvDestRectRopeCut.Intersection(&dropPos) )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut )
					{
						if( !ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken )
						{
							Hud->ShowBannerText("L1prep");
							Hud->CancelDrag();
						}
						//we have the key!!
						else if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed )//cap not closed!!
						{
							if( ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled )//everything done - just need to close the cap!!
								Hud->ShowBannerText("L1sea");
							else //diesel not filled!!
								Hud->ShowBannerText("L1yet");
							Hud->CancelDrag();
						}
						else
						{
							Hud->ReleaseDrag();
							Hud->CloseInventory(); 
						
							if (ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut && ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut)
							{
								//if frontyard swing and beach rose cut, no need for knife
								ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_SwissKnife] = false;
								Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S1_SwissKnife); 
							}

							ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut = true;
							rope->SetEnable(false);
							UpdateBgGraphic();

							CAnim_RopeKnifeCut->PlayAnim();
							isRopeKnifeCutAnimPlaying = true;
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S7RopeCut);
							TaskPendingUpdate();
							isSparkleUpdatePending = true;
							/*p1Rope->SetEnable(false);
							p1CuttedRope->SetEnable(true);*/
						}
					}
					return;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S5_BoatKey )
			{
				if( Popup2_State == POPUP_OPEN && pop2RectkeyFixArea.Intersection(&dropPos) )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced )
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_BoatKey] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S5_BoatKey);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S7BoatKeyFixed);

						ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced = true;
						p2KeyFixed->SetEnable(true);
						TaskPendingUpdate();
						isSparkleUpdatePending = true;
					}
					return;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}			
			else 
				GFInstance->WrongInvDropMsg(Hud);

		}
	}
	else if( val == INV_CLOSE )
	{
		
		
	}	
}

void L1_S7_Boatdock::SparkleUpdate()
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

		if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken)
		{
			sparklePoints[numSparkles].SetXY(InvItemRectPlankScn.GetCenterX(), InvItemRectPlankScn.GetCenterY());
			numSparkles++;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsStepsPlaced && ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken)
		{
			sparklePoints[numSparkles].SetXY(InvDestRectPlanks.GetCenterX(), InvDestRectPlanks.GetCenterY());
			numSparkles++;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut )
		{
			sparklePoints[numSparkles].SetXY(Pop1RectDieselTank.GetCenterX(), Pop1RectDieselTank.GetCenterY());
			numSparkles++;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_HOInvFunnelTaken)
		{
			sparklePoints[numSparkles].SetXY(Pop1RectDieselTank.GetCenterX(), Pop1RectDieselTank.GetCenterY());
			numSparkles++;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked)
		{
			sparklePoints[numSparkles].SetXY(Pop1RectDieselTank.GetCenterX(), Pop1RectDieselTank.GetCenterY());
			numSparkles++;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced && ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken)
		{
			sparklePoints[numSparkles].SetXY(Pop2RectKeyStart.GetCenterX(), Pop2RectKeyStart.GetCenterY());
			numSparkles++;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted && ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed)
		{
			sparklePoints[numSparkles].SetXY(Pop2RectKeyStart.GetCenterX(), Pop2RectKeyStart.GetCenterY());
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


void L1_S7_Boatdock::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S7Collected)
		{
			InitHintTrailEmit(400, 150);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 21;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced && ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken && Popup2_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S5_BoatKey)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (pop2RectkeyFixArea.GetCenterX(), pop2RectkeyFixArea.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						
						return;
					}
					break;
					
			case 1: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted && ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced && Popup2_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	pop2RectkeyFixArea.GetCenterX(), pop2RectkeyFixArea.GetCenterY(),false);
						return;
					}
					break;	
			case 2: ++iNonHOHint;
					 if( Popup2_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
						return;
					}
					break;	
			
			case 3: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken && ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && Popup1_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_SwissKnife)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectRopeCut.GetCenterX(), InvDestRectRopeCut.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
		
						return;
					}
					break;
			case 4: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen && Popup1_State == POPUP_OPEN)
					{

						InitHintTrailEmit(	pop1RectDieselTankCap.GetCenterX(), pop1RectDieselTankCap.GetCenterY(),false);
						return;
					}
					break;
					
			case 5: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen && ProfileSystem::Instance->ActiveProfile.L1S6_HOInvFunnelTaken && Popup1_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S6_Funnel)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (pop1RectDieselTankCap.GetCenterX(), pop1RectDieselTankCap.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
				
						return;
					}
					break;	
		   case 6: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled && ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked && Popup1_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S6_CanFull)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (pop1RectDieselTankCap.GetCenterX(), pop1RectDieselTankCap.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						
						return;
					}
					break;
			case 7: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	pop1RectDieselTankCap.GetCenterX(), pop1RectDieselTankCap.GetCenterY(),false);
						return;
					}
					break;
			case 8: ++iNonHOHint;
					 if( Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
						return;
					}
					break;	
			case 9: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced && ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken )
					{
						InitHintTrailEmit(	Pop2RectKeyStart.GetCenterX(), Pop2RectKeyStart.GetCenterY(),true);
						return;
					}
					break;
					
			case 10: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted && ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatKeyPlaced )
					{
						InitHintTrailEmit(	Pop2RectKeyStart.GetCenterX(), Pop2RectKeyStart.GetCenterY(),true);
						return;
					}
					break;	
			
			case 11: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut && ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken )
					{
						InitHintTrailEmit(Pop1RectDieselTank.GetCenterX(), Pop1RectDieselTank.GetCenterY(),true);
						return;
					}
					break;
			case 12: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen )
					{
						InitHintTrailEmit(Pop1RectDieselTank.GetCenterX(), Pop1RectDieselTank.GetCenterY(),true);
						return;
					}
					break;
					
			case 13: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && ProfileSystem::Instance->ActiveProfile.L1S7_IsCapOpen && ProfileSystem::Instance->ActiveProfile.L1S6_HOInvFunnelTaken )
					{
						InitHintTrailEmit(Pop1RectDieselTank.GetCenterX(), Pop1RectDieselTank.GetCenterY(),true);
						return;
					}
					break;	
		   case 14: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled && ProfileSystem::Instance->ActiveProfile.L1S7_IsFunnelPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked )
					{
						InitHintTrailEmit(Pop1RectDieselTank.GetCenterX(), Pop1RectDieselTank.GetCenterY(),true);
						return;
					}
					break;
			case 15: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsCapClosed && ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled )
					{
						InitHintTrailEmit(Pop1RectDieselTank.GetCenterX(), Pop1RectDieselTank.GetCenterY(),true);
						return;
					}
					break;
			case 16: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken && ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken && !ProfileSystem::Instance->ActiveProfile.L1S7_IsStepsPlaced)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S2_Plank1)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectPlanks.GetCenterX(), InvDestRectPlanks.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
		
						return;
					}
					break;
			case 17: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken  )
					{
						InitHintTrailEmit(InvItemRectPlankScn.GetCenterX(), InvItemRectPlankScn.GetCenterY(),true);
						return;
					}
					break;
			case 18: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S6_BoatShed])
					{
						InitHintTrailEmit(SceneRectBoatShed.GetCenterX(), SceneRectBoatShed.GetCenterY(),true);
						return;
					}
					break;
			case 19: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S2_Frontyard])
					{
						InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
						return;
					}
					break;
			case 20: ++iNonHOHint;
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

void  L1_S7_Boatdock::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{

		IsPopupTransitionActive = true;

		bgScaleOffsetX = popRect.GetCenterX();
		bgScaleOffsetY = popRect.GetCenterY();
		
		RT_BgScn->SetEnable(true);
		RT_BgScn->SetScale(1.0f);
		RT_BgScn->SetXY(0,0);

		//HOScene->Show(false);
		HOScene->PauseObjects(true);
    if( GFInstance->IsRTCreated )
    {
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS
		GFInstance->IsRenderingToRT = true;
    }
		Popup->RenderToTarget(-171,0);
    if( GFInstance->IsRTCreated )
    {
		GFInstance->IsRenderingToRT = false;
		GFInstance->RTGraphic->endRenderToTarget();
    }
		GFInstance->SetPopupTween( (int)popRect.GetCenterX(-683) ,(int)popRect.GetCenterY(-384 ), 171, 0);

		GFInstance->CreateGreyscaleScene();

		SetZ(zOrderPopVideo);
}

void L1_S7_Boatdock::NavigateToScene()
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