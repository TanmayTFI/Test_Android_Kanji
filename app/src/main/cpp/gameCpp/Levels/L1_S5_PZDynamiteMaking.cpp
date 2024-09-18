//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 05 : Puzzle : Dynamite Making
//====================================

#include "L1_S5_PZDynamiteMaking.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Fonts.h"
#include "AssetsStringTable.h"

const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,98,0);

const CPoint funnelFullPos(779, 100, 0);
const CPoint funnelWithTapePos(791, 99, 0);
const CPoint funnelWithTapeCutPos(799, 100, 0);

const CPoint NailLoopAnimPos1(84, 385, 0);
const CPoint NailLoopAnimPos2(389, 277, 0);
const CPoint NailLoopAnimPos3(332, 134, 0);

const CPoint NailPluckingAnimPos1(77, 369, 0);
const CPoint NailPluckingAnimPos2(382, 261, 0);
const CPoint NailPluckingAnimPos3(325, 118, 0);

const CPoint hammerPos1(186, 482, 0);
const CPoint hammerPos2(491, 374, 0);
const CPoint hammerPos3(434, 231, 0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  areaRectMatch3(1027.0f,21.0f,248.0f,80.0f);
const CRectangle  PopRectNotice(153.0f,179.0f,198.0f,67.0f);

const CRectangle  areaRectNail1(39.0f,466.0f,309.0f,184.0f);
const CRectangle  areaRectNail2(344.0f,381.0f,314.0f,161.0f);
const CRectangle  areaRectNail3(302.0f,203.0f,268.0f,160.0f);

const CRectangle  DestRectMortar(570.0f,101.0f,802.0f,456.0f);

const CPoint hammerOffset1(-300,112,0);
const CPoint hammerOffset2(0,0,0);
const CPoint hammerOffset3(-52,-142,0);

const CRectangle  InvItemRectHammerHead(307.0f,397.0f,170.0f,87.0f);

L1_S5_PZDynamiteMaking::L1_S5_PZDynamiteMaking()
{

	Visible = false;

	nailArea[0] = areaRectNail1;
	nailArea[1] = areaRectNail2;
	nailArea[2] = areaRectNail3;

	NailLoopAnimPos[0] = NailLoopAnimPos1;
	NailLoopAnimPos[1] = NailLoopAnimPos2;
	NailLoopAnimPos[2] = NailLoopAnimPos3;

	NailPluckingAnimPos[0] = NailPluckingAnimPos1;
	NailPluckingAnimPos[1] = NailPluckingAnimPos2;
	NailPluckingAnimPos[2] = NailPluckingAnimPos3;

	hammerPos[0] = hammerPos1;
	hammerPos[1] = hammerPos2;
	hammerPos[2] = hammerPos3;

	hammerOffset[0] = hammerOffset1;
	hammerOffset[1] = hammerOffset2;
	hammerOffset[2] = hammerOffset3;

	isPuzzleOver = false;
	isMapMoving = false;
	isSpeedMeterRunning = false;
	isSpoonWithBluePowderInHand = false;
	isSpoonWithGreenPowderInHand = false;
	isObjectReturning = false;
	isObjectInHand = false;
	isCrystallAddedToGrinder = false;
	isNailPluckingAnimPlaying = false;
	isNailWrongPluckingAnimPlaying = false;
	tweenTime = 0.0f;
	swapTime = 0.70f;

	isCrystalTakeAnimPlaying = false;
	isAmoniMixAnimPlaying = false;
	isGreenPowderDropAnimPlaying = false;
	isGreenPowderTakeAnimPlaying = false;
	isGrinderAnimPlaying = false;
	isCapBottleAnimPlaying = false;
	isbluePowderTakeAnimPlaying = false;

	ActivePopupID = ePopInactive;

	//Main Scene
	LoadHud(INVENTORY_V1);

	hammer = NULL;
	NailPluckingAnim = NULL;
	
	GreenPowderTakeAnim = NULL;
	CrystalTakeAnim = NULL;
	CapBottleAnim = NULL;
	GreenPowderDropAnim = NULL;
	
	pointerInHand = new CObject();

	MoonLightVFX = NULL;
	LightVFX = NULL;

	LoadScene("LV1\\S5\\P1\\P1.SCN");

	amoniBottle = HOScene->GetObjectByID("amoniBottle");
	amoniBottleCork = HOScene->GetObjectByID("amoniBottleCork");
	bottle = HOScene->GetObjectByID("bottle");
	closedBoxDoor = HOScene->GetObjectByID("closedBoxDoor");
	crystalInGrinder1 = HOScene->GetObjectByID("grinderTopCoarseAnim");
	crystalInGrinder3 = HOScene->GetObjectByID("grinderTopFineAnim");
	greenPowderOnPlate = HOScene->GetObjectByID("greenPowderOnPlate");
	bluePowderOnPlate = HOScene->GetObjectByID("bluePowderOnPlate");
	grinder = HOScene->GetObjectByID("grinder");
	map1 = HOScene->GetObjectByID("map1");
	map2 = HOScene->GetObjectByID("map2");
	nails[0] = HOScene->GetObjectByID("nail1");
	nails[1] = HOScene->GetObjectByID("nail2");
	nails[2] = HOScene->GetObjectByID("nail3");
	openedBoxDoor = HOScene->GetObjectByID("openedBoxDoor");
	rode = HOScene->GetObjectByID("rode");
	speedBar = HOScene->GetObjectByID("speedBar");
	speedMeterBG = HOScene->GetObjectByID("speedMeterBG");
	spoon = HOScene->GetObjectByID("spoon");
	thread = HOScene->GetObjectByID("thread");
	threadOnBottle = HOScene->GetObjectByID("threadOnBottle");
	threadWaxOnBottle = HOScene->GetObjectByID("threadWaxOnBottle");
	waxCylinder = HOScene->GetObjectByID("waxCylinder");
	if( ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle )
		waxCylinder->Show(false);//wax cylinder on table!!

	grinderCoarse = HOScene->GetObjectByID("grinderTopCoarseAnim");
	grinderMedium = HOScene->GetObjectByID("grinderTopMediumAnim");
	grinderFine = HOScene->GetObjectByID("grinderTopFineAnim");
	rodInHand = HOScene->GetObjectByID("rodeInHand");
	grinderScatter = HOScene->GetObjectByID("grinderTopScatterAnim");
	
	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);	
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

	ResetDia = new CHOScene();
	ResetDia->Load("UI\\RESETDIA.SCN");

	CObject* resetText = new CObject();
	resetText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\RESETCNF.SPR", 670, 308, 2);
	resetText->SetUID("resetText");
	ResetDia->PushObject(resetText);

	CObject* mask = new CObject();
	mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
	mask->SetUID("mask");
	mask->SetScale(2);
	ResetDia->PushObject(mask);

	CObject* btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", 590, 420, 5);
	btnText->SetUID("yesBTNText");
	ResetDia->PushObject(btnText);

	btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", 780, 420, 5);
	btnText->SetUID("noBTNText");
	ResetDia->PushObject(btnText);

	ResetDia->SetZOff(eZOrder_Popup);
	ResetDia->Show(false);
	IsResetPop = false;

	//grinderCoarse->SetEnable(false);
	grinderMedium->SetEnable(false);
	//grinderFine->SetEnable(false);
	rodInHand->SetEnable(false);
	grinderScatter->SetEnable(false);

	Anim_HammerLoop =  HOScene->GetObjectByID("hammerWithoutNail");
	Anim_HammerMaskLoop  = HOScene->GetObjectByID("hammerMask");
	Anim_NailLoop = HOScene->GetObjectByID("nailAnimLoop");

	Anim_HammerLoop->SetEnable(false);
	Anim_HammerMaskLoop->SetEnable(false);
	Anim_NailLoop->SetEnable(false);

	spoonWithBluePowder = HOScene->GetObjectByID("spoonWithBluePowder");
	spoonWithBluePowder->Show(false);
	spoonWithGreenPowder = HOScene->GetObjectByID("spoonWithGreenPowder");
	spoonWithGreenPowder->Show(false);

	MoonLightVFX = new SuperFX();
	MoonLightVFX->LoadEffectIni("PRT\\L1\\s5moonprt.INI");
	MoonLightVFX->SetPos( PointSystem::CreateCPoint(1249,152,0));
	MoonLightVFX->SetZ(100);
	MoonLightVFX->Start();
	MoonLightVFX->AdvanceByTime(2.0f);
	VFXArray.push_back(MoonLightVFX);

	LightVFX = new SuperFX();
	LightVFX->LoadEffectIni("PRT\\L1\\s5lightprt.INI");
	LightVFX->SetPos(PointSystem::CreateCPoint(297,285,0));
	LightVFX->SetZ(100);
	LightVFX->Start();
	LightVFX->AdvanceByTime(2.0f);
	VFXArray.push_back(LightVFX);

	speedBar->Show(false);
	speedMeterBG->Show(false);
	
	NumberOfNailsPlucked = 0;

	HOScene->GetObjectByID("rayleft")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("rayleft")->Animate(0.1f, 0.90f, 10.0f);

	HOScene->GetObjectByID("rayright")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("rayright")->Animate(0.1f, 0.90f, 15.0f);

	CAnim_Grinder = NULL;
	CObject* objArr[] = {grinderCoarse, grinderMedium, grinderFine, rodInHand};
	std::string animArr[] = {"ANIM\\L1\\S5Coarse_GrindAnim.anim", "ANIM\\L1\\S5Medium_GrindAnim.anim", "ANIM\\L1\\S5Fine_GrindAnim.anim", "ANIM\\L1\\S5Rod_GrindAnim.anim"};
	CAnim_Grinder = new CustomAnimGroup(4, animArr, objArr);
	
	CAnim_HammerLoop = NULL;
	CObject* objArr2[] = {Anim_HammerLoop, Anim_NailLoop, Anim_HammerMaskLoop};
	std::string animArr2[] = {"ANIM\\L1\\S5Hammer_Loop.anim", "ANIM\\L1\\S5Nail_Loop.anim", "ANIM\\L1\\S5HammerMask_Loop.anim"};
	CAnim_HammerLoop = new CustomAnimGroup(3, animArr2, objArr2);

	Anim_GrinderMask =  HOScene->GetObjectByID("CrystalGrinderMask");
	Anim_GrinderMask->SetEnable(false);
	Anim_BluePowderMask = HOScene->GetObjectByID("bluePowderMask");
	Anim_BluePowderMask ->SetEnable(false);
	Anim_Spoon =  HOScene->GetObjectByID("Animspoon");
	Anim_Spoon ->SetEnable(false);
	Anim_SpoonWithPowder =  HOScene->GetObjectByID("AnimSpoonWithBluePowder");
	Anim_SpoonWithPowder->SetEnable(false);
	CAnim_BluePowderTake = NULL;

	CObject* objArr3[] = {Anim_GrinderMask, Anim_BluePowderMask, Anim_Spoon, Anim_SpoonWithPowder };
	std::string animArr3[] = {"ANIM\\L1\\S5CrystalGrinderMask_BluePowderTaking.anim", "ANIM\\L1\\S5PowderMask_BluePowderTaking.anim", "ANIM\\L1\\S5Spoon_BluePowderTaking.anim", "ANIM\\L1\\S5SpoonWithPowder_BluePowderTaking.anim"};
	CAnim_BluePowderTake = new CustomAnimGroup(4, animArr3, objArr3);

	for (int i = 0; i < 3; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[i] == true)
		{
			NumberOfNailsPlucked++;
			nails[i]->Show(false);
		}
	}
	
	if(NumberOfNailsPlucked < 3)
	{
		hammer = new CObject();
		hammer->LoadSprite("LV1\\S5\\P1\\hammer.SPR");
		hammer->SetZ(200);
		hammer->Pos = CPoint(931.0f,81.0f,0.0f);
		hammer->Show(false);
	}
	else
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened && ProfileSystem::Instance->ActiveProfile.L1S5_P1MortarPestlePlaced && !ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle)
			Hud->InvBox_StartPuzzleMode();
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerPlacedOnScene && NumberOfNailsPlucked < 3)
	{
		hammer->Show(true);
		clickedNailIndex = ProfileSystem::Instance->ActiveProfile.L1S5_SelectedNailIndex;
		hammer->Pos = hammerPos[clickedNailIndex];
		
	}

	
	if(NumberOfNailsPlucked < 3)
	{
		NailPluckingAnim = new CObject();
		NailPluckingAnim->LoadSprite("LV1\\S5\\P1\\NailPluckingAnim.SPR");
		NailPluckingAnim->SetZ(200);
		NailPluckingAnim->Pos = CPoint(931.0f,81.0f,0.0f);
		NailPluckingAnim->Show(false);
	}
	
	
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate)
	{
		AmoniMixAnim = new CObject();
		AmoniMixAnim->LoadSprite("LV1\\S5\\P1\\AmoniMixAnim.SPR");
		AmoniMixAnim->SetZ(200);
		AmoniMixAnim->Pos = CPoint(615.0f,182.0f,0.0f);
		AmoniMixAnim->Show(false);
	}

	
	GreenPowderTakeAnim = new CObject();
	GreenPowderTakeAnim->LoadSprite("LV1\\S5\\P1\\GreenPowderTakeAnim.SPR");
	GreenPowderTakeAnim->SetZ(200);
	GreenPowderTakeAnim->Pos = CPoint(639.0f,268.0f,0.0f);
	GreenPowderTakeAnim->Show(false);

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder)
	{
		CrystalTakeAnim = new CObject();
		CrystalTakeAnim->LoadSprite("LV1\\S5\\P1\\CrystalTakeAnim.SPR");
		CrystalTakeAnim->SetZ(200);
		CrystalTakeAnim->Pos = CPoint(110.0f,182.0f,0.0f);
		CrystalTakeAnim->Show(false);
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle)
	{
		CapBottleAnim = new CObject();
		CapBottleAnim->LoadSprite("LV1\\S5\\P1\\CapBottleAnim.SPR");
		CapBottleAnim->SetZ(12);
		CapBottleAnim->Pos = CPoint(1001.0f,72.0f,0.0f);
		CapBottleAnim->Show(false);

		GreenPowderDropAnim = new CObject();
		GreenPowderDropAnim->LoadSprite("LV1\\S5\\P1\\GreenPowderDropAnim.SPR");
		GreenPowderDropAnim->SetZ(12);
		GreenPowderDropAnim->Pos = CPoint(846.0f,28.0f,0.0f);
		GreenPowderDropAnim->Show(false);
	}

	
	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened)
	{
		closedBoxDoor->Show(false);
		openedBoxDoor->Show(true);
	}
	else
	{
		openedBoxDoor->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate)
	{
		map1->Show(false);
		map2->Show(true);
	}
	else
	{
		map1->Show(true);
		map2->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBottleCapRemoved)
	{
		amoniBottleCork->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate)
		{
			greenPowderOnPlate->Show(true);
			bluePowderOnPlate->Show(false);
			amoniBottle->Show(false);
			amoniBottleCork->Show(false);
		}
		else
		{
			greenPowderOnPlate->Show(false);
			bluePowderOnPlate->Show(true);
		}
	}
	else
	{
		greenPowderOnPlate->Show(false);
		bluePowderOnPlate->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated)
		{
			crystalInGrinder3->Show(true);
			crystalInGrinder1->Show(false);
			rode->Show(false);
		}
		else
		{
			crystalInGrinder3->Show(false);
			crystalInGrinder1->Show(true);
		}
	}
	else
	{
		crystalInGrinder1->Show(false);
		crystalInGrinder3->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle)
	{
		thread->Show(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle)
		{
			threadWaxOnBottle->Show(true);
			threadOnBottle->Show(false);
		}
		else
		{
			threadWaxOnBottle->Show(false);
			threadOnBottle->Show(true);
		}
	}
	else
	{
		threadWaxOnBottle->Show(false);
		threadOnBottle->Show(false);
	}

	HOScene->GetObjectByID("bottleFull")->Show(false);
	HOScene->GetObjectByID("bottleHalf")->Show(false);
	HOScene->GetObjectByID("bottleThreeforth")->Show(false);

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull)
	{
		HOScene->GetObjectByID("greenpowderspill")->Show(false);
		HOScene->GetObjectByID("greenpowderfill")->Show(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleTreeForth)
			HOScene->GetObjectByID("bottleThreeforth")->Show(true);
		else if(ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleHalf)
			HOScene->GetObjectByID("bottleHalf")->Show(true);
	}
	else
		HOScene->GetObjectByID("bottleFull")->Show(true);

	if(ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken)
		HOScene->GetObjectByID("hammerhead")->Show(false);
	


	GFInstance->PostUpdate(2);
	
	InitPopups();

	zOrderScnVideo = 3;
	zOrderPopVideo = eZOrder_Popup + 5;//sree

	int ArrayPts00[] = {949,251,961,182,1027,192,1016,269};
	polyAmoniBottleCap=  new CPolygon(4);
	polyAmoniBottleCap->Init(ArrayPts00);

	polyArray.push_back(polyAmoniBottleCap);

	int ArrayPts01[] = {643,588,596,475,748,420,817,522};
	polyMap1 =  new CPolygon(4);
	polyMap1->Init(ArrayPts01);

	polyArray.push_back(polyMap1);

	int ArrayPts02[] = {104,185,176,294,373,239,341,115};
	polyNotice =  new CPolygon(4);
	polyNotice->Init(ArrayPts02);

	polyArray.push_back(polyNotice);

	int ArrayPts03[] = {1016,292,1047,181,1032,172,1062,85,1101,91,1106,119,1128,129,1125,155,1137,184,1094,310,1057,319};
	polyInvBomb =  new CPolygon(11);
	polyInvBomb->Init(ArrayPts03);

	polyArray.push_back(polyInvBomb);


	int ArrayPts05[] = {113,603,140,367,455,281,531,461,116,606};
	polyBoxDoor=  new CPolygon(5);
	polyBoxDoor->Init(ArrayPts05);

	polyArray.push_back(polyBoxDoor);

	int ArrayPts[] = {617,462,652,384,747,356,849,383,891,476,836,543,722,560,645,524};
	polyPlate =  new CPolygon(8);
	polyPlate->Init(ArrayPts);

	polyArray.push_back(polyPlate);


	int ArrayPts0[] ={1028,94,1018,298,1195,306,1213,104};
	polyBottle =  new CPolygon(4);
	polyBottle->Init(ArrayPts0);

	polyArray.push_back(polyBottle);

	int ArrayPts1[] = {527,440,560,342,677,350,638,441};
	polyThreadGet=  new CPolygon(4);
	polyThreadGet->Init(ArrayPts1);
	getPoly[0] = polyThreadGet;
	getObject[0] = thread;
	mousePointerString[0] = "LV1\\S5\\P1\\thread.SPR";
	
	polyArray.push_back(polyThreadGet);

	int ArrayPts2[] = {1033,196,1059,70,1174,88,1149,203};
	polyThreadSet=  new CPolygon(4);
	polyThreadSet->Init(ArrayPts2);
	setPoly[0] = polyThreadSet;
	setObject[0] = threadOnBottle;

	polyArray.push_back(polyThreadSet);

	int ArrayPts3[] = {559,515,561,470,583,453,619,465,617,515,590,531};
	polyWaxCylinderGet =  new CPolygon(6);
	polyWaxCylinderGet->Init(ArrayPts3);
	getPoly[1] = polyWaxCylinderGet;
	getObject[1] = waxCylinder;
	mousePointerString[1] = "LV1\\S5\\P1\\waxCylinder.SPR";

	polyArray.push_back(polyWaxCylinderGet);

	setPoly[1] = polyThreadSet;
	setObject[1] = threadWaxOnBottle;

	int ArrayPts5[] = {1171,385,1104,484,1077,501,1084,524,1104,538,1133,539,1145,519,1140,503,1200,407};
	polyRodeGet=  new CPolygon(9);
	polyRodeGet->Init(ArrayPts5);
	getPoly[2] = polyRodeGet;
	getObject[2] = rode;
	mousePointerString[2] = "LV1\\S5\\P1\\rodeInHand.SPR";

	polyArray.push_back(polyRodeGet);

	int ArrayPts6[] = {984,404,985,283,1167,274,1171,447,991,459};
	polyRodeSet =  new CPolygon(5);
	polyRodeSet->Init(ArrayPts6);
	setPoly[2] = polyRodeSet;
	setObject[2] = crystalInGrinder3;

	polyArray.push_back(polyRodeSet);

	int ArrayPts7[] = {982,435,1005,303,1103,300,1166,355,1131,398,1102,461,1067,479,1016,470};
	polyGrinderGet=  new CPolygon(8);
	polyGrinderGet->Init(ArrayPts7);
	getPoly[3] = polyGrinderGet;
	getObject[3] = grinder;
	mousePointerString[3] = "LV1\\S5\\P1\\grinder.SPR";
	polyArray.push_back(polyGrinderGet);
	
	
	int ArrayPts8[] = {187,531,145,447,225,362,322,352,381,415,342,500,262,547};
	polyGrinderSet=  new CPolygon(7);
	polyGrinderSet->Init(ArrayPts8);
	setPoly[3] = polyGrinderSet;
	grinderWithCrystallInHand = new CObject();
	grinderWithCrystallInHand->LoadSprite("LV1\\S5\\P1\\grinderWithCrystallInHand.SPR");
	grinderWithCrystallInHand->SetZ(200);
	grinderWithCrystallInHand->Show(false);
	setObject[3] = grinderWithCrystallInHand;

	polyArray.push_back(polyGrinderSet);

	int ArrayPts9[] = {1082,608,1055,605,1021,565,996,532,953,503,930,506,892,471,894,435,930,438,967,463,971,483,970,489,1020,529,1074,566,1093,586};
	polySpoonGet=  new CPolygon(15);
	polySpoonGet->Init(ArrayPts9);
	getPoly[4] = polySpoonGet;
	getObject[4] = spoon;
	mousePointerString[4] = "LV1\\S5\\P1\\spoon.SPR";

	polyArray.push_back(polySpoonGet);


	int ArrayPts10[] = {985,386,985,298,1140,274,1204,391,1058,454};
	polySpoonSet=  new CPolygon(5);
	polySpoonSet->Init(ArrayPts10);
	setPoly[4] = polySpoonSet;
	spoonWithBluePowder = new CObject();
	spoonWithBluePowder->LoadSprite("LV1\\S5\\P1\\spoonWithBluePowder.SPR");
	spoonWithBluePowder->SetZ(200);
	spoonWithBluePowder->Show(false);
	setObject[4] = spoonWithBluePowder;

	polyArray.push_back(polySpoonSet);

	getPoly[5] = polySpoonGet;
	getObject[5] = spoon;
	mousePointerString[5] = "LV1\\S5\\P1\\spoon.SPR";

	setPoly[5] = polyPlate;
	spoonWithGreenPowder = new CObject();
	spoonWithGreenPowder->LoadSprite("LV1\\S5\\P1\\spoonWithGreenPowder.SPR");
	spoonWithGreenPowder->SetZ(200);
	spoonWithGreenPowder->Show(false);
	setObject[5] = spoonWithGreenPowder;

	int ArrayPts11[] = {950,358,920,336,923,302,956,254,961,218,970,181,997,185,1013,204,1009,218,1013,232,1001,249,1021,315,1005,359,975,364};
	polyAmoniBottleGet=  new CPolygon(14);
	polyAmoniBottleGet->Init(ArrayPts11);
	getPoly[6] = polyAmoniBottleGet;
	getObject[6] = amoniBottle;
	mousePointerString[6] = "LV1\\S5\\P1\\amoniBottleInHand.SPR";

	polyArray.push_back(polyAmoniBottleGet);

	setPoly[6] = polyPlate;
	setObject[6] = greenPowderOnPlate;

	RectHammer[0] = new CRectangle(95.0f,398.0f,190.0f,190.0f);
	RectHammer[1] = new CRectangle(363.0f,330.0f,172.0f,134.0f);
	RectHammer[2] = new CRectangle(342.0f,142.0f,206.0f,174.0f);

	int ArrayPts12[] = {264,393,286,416,279,429,245,457,179,510,175,524,197,538,218,545,208,566,155,556,118,529,115,509,105,499,88,486,109,456,125,477,141,489,156,489,161,492,252,393};
	polyHammer[0] =  new CPolygon(20);
	polyHammer[0]->Init(ArrayPts12);

	polyArray.push_back(polyHammer[0]);

	int ArrayPts13[] = {465,380,560,287,590,310,584,322,484,402,483,418,499,431,522,438,508,458,482,453,451,444,420,419,421,401,412,392,395,377,417,353,441,379};
	polyHammer[1] =  new CPolygon(17);
	polyHammer[1]->Init(ArrayPts13);

	polyArray.push_back(polyHammer[1]);

	int ArrayPts14[] = {408,238,502,146,514,146,531,165,529,175,431,256,425,266,432,282,464,292,453,315,400,303,364,276,361,253,337,232,357,209,377,227,390,239};
	polyHammer[2] =  new CPolygon(17);
	polyHammer[2]->Init(ArrayPts14);

	polyArray.push_back(polyHammer[2]);

	SetZ(zOrderScnVideo);

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1MortarPestlePlaced)
	{
		grinder->SetEnable(false);
		rode->SetEnable(false);
	}

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if( !Control::Audio->IsPlaying(aTrackL1Trapped) )
		Control::Audio->PlaySample(aTrackL1Trapped,true);

	Control::Audio->LoadSample(aAmbS5PuzDynamite,AudioAmb);
	Control::Audio->PlaySample(aAmbS5PuzDynamite,true);

	Control::Audio->LoadSample(aSFXL1S4CoinPicked,AudioSFX);
	Control::Audio->LoadSample(aSFXL1S4BooksMoved,AudioSFX);
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S5_PZDynamiteMaking]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S5_PZDynamiteMaking])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S5_PZDynamiteMaking] = true;

	

	
	

	Visible = true;	
}

void L1_S5_PZDynamiteMaking::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S5_TreasureRoom] = isAnyTaskLeft;
}

void L1_S5_PZDynamiteMaking::InitPopups()
{
	ActivePopupID = ePopInactive;

	IsPopupTransitionActive = false;

	Popup1_State = POPUP_CLOSED;
	Popup1Notice = NULL;

	Popup1Notice = new CHOScene();
	Popup1Notice->Load("LV1\\S5\\P1\\HELPSHEET.SCN");

	ColorRGB color = { 0.1f, 0.1f, 0.1f };
	NH4 = new MultilineTextBox(AssetsStringTable::getString("ammonia_crystals"), 315, 110, eZOrder_Popup + 10, color, eFont_Small, 20);
	NH4->Show(false);
	HNO3 = new MultilineTextBox(AssetsStringTable::getString("nitric_acid"), 380, 490, eZOrder_Popup + 10, color, eFont_Small, 20);
	HNO3->Show(false);
	NH4NO3 = new MultilineTextBox(AssetsStringTable::getString("ammonium_nitrate"), 670, 190, eZOrder_Popup + 10, color, eFont_Small, 20);
	NH4NO3->Show(false);
	minigrinderTxt = new MultilineTextBox(AssetsStringTable::getString("mini_grinder"), 420, 260, eZOrder_Popup + 10, color, eFont_Small, 20);
	minigrinderTxt->Show(false);
	threadTxt = new MultilineTextBox(AssetsStringTable::getString("thread"), 630, 510, eZOrder_Popup + 10, color, eFont_Small, 20);
	threadTxt->Show(false);
	waxTxt = new MultilineTextBox(AssetsStringTable::getString("wax"), 745, 580, eZOrder_Popup + 10, color, eFont_Small, 20);
	waxTxt->Show(false);

	Popup1Notice->Show(false);
	Popup1Notice->SetPosOff(CenterPoint.x,CenterPoint.y);
	Popup1Notice->SetZOff(eZOrder_Popup);
}

void L1_S5_PZDynamiteMaking::Required()
{
	
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( IsPopupTransitionActive )
		UpdatePopupTransition();
}

float L1_S5_PZDynamiteMaking::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L1_S5_PZDynamiteMaking::ProcessSkip()
{
	Hud->InvBox_StopPuzzleMode();

	//isPuzzleOver = true;
	ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate = true;
	ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder = true;
	ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated = true;
	ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate = true;
	ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBottleCapRemoved = true;
	ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate = true;
	ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull = true;
	ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle = true;
	ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle = true;

	
	threadWaxOnBottle->Show(true);
	thread->Show(false);
	threadOnBottle->Show(false);
	waxCylinder->Show(false);
	amoniBottleCork->Show(false);
	greenPowderOnPlate->Show(true);
	map1->Show(false);
	map2->Show(true);
	crystalInGrinder1->Show(false);
	crystalInGrinder3->Show(true);
	bluePowderOnPlate->Show(false);
	rode->Show(false);
	spoon->Show(false);
	HOScene->GetObjectByID("bottleFull")->Show(true);
	HOScene->GetObjectByID("bottleHalf")->Show(false);
	HOScene->GetObjectByID("bottleThreeforth")->Show(false);
	
	HOScene->GetObjectByID("greenpowderspill")->Show(true);
	HOScene->GetObjectByID("greenpowderfill")->Show(true);
}

void L1_S5_PZDynamiteMaking::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive || Hud->IsMapShowing)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	ResetPuzzle();

	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	if(isPuzzleOver)
	{
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
		SendMessage(ControlExit, ID, HOSM_NewArea);
		return;
	}

	if(isSpeedMeterRunning)
	{
		speedBar->Pos.x += barSpeed;
		if(speedBar->Pos.x >= speedBarEndPos.x || speedBar->Pos.x < speedBarStarPos.x)
		{
			barSpeed *= -1;
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken && InvItemRectHammerHead.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			HOScene->GetObjectByID("hammerhead")->Show(false);

			TakeInventoryItem(eInv_L1S5_HammerHead);
		}
		return;
	}

	if(ActivePopupID == ePopInactive)
	{

		if( !isNailPluckingAnimPlaying )
		{
			if(DestRectMortar.Intersection(&pos) || polySpoonGet->Intersection(&pos))
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened)
				{
					Cursor::SetMode(CA_Gear);
					if( Control::Input->LBclicked()) 
					{
							Hud->ShowBannerText("L1prepared");
					}

					return;
				}
				else if( !ProfileSystem::Instance->ActiveProfile.L1S5_P1MortarPestlePlaced )
				{
					Cursor::SetMode(CA_Gear);
					if( Control::Input->LBclicked())
					{
							Hud->ShowBannerText("L1mortar");
					}

					return;
				}	
			
			}
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle && !ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken)
		{
			if(polyInvBomb->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					TakeInventoryItem(eInv_L1S5_Dynamite);
					bottle->Show(false);
					threadWaxOnBottle->Show(false);
					HOScene->GetObjectByID("greenpowderfill")->Show(false);
					HOScene->GetObjectByID("bottleFull")->Show(false);
					HOScene->GetObjectByID("bottleHalf")->Show(false);
					HOScene->GetObjectByID("bottleThreeforth")->Show(false);
					isPuzzleOver = true;
					return;
				}
			}
		}

		

		if(isNailPluckingAnimPlaying)
		{
			if(NailPluckingAnim->IsAnimationFinished())
			{
				NailPluckingAnim->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[clickedNailIndex] = true;
				isNailPluckingAnimPlaying = false;
				speedBar->Show(false);
				speedMeterBG->Show(false);
				NumberOfNailsPlucked ++;
				if(NumberOfNailsPlucked < 3)
				{
					for (int i = 0; i < 3; i++)
					{
						if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[i] == false)
						{
							hammer->Show(true);
							hammer->Pos = hammerPos[i];
							clickedNailIndex = i;
							ProfileSystem::Instance->ActiveProfile.L1S5_SelectedNailIndex = clickedNailIndex;
						}
					}
				}
				else
				{
					//Hud->InvBox_StartPuzzleMode();
				}
			}
			return;
		}

		if(isNailWrongPluckingAnimPlaying)
		{
			if( CAnim_HammerLoop->IsAnimFinished())
			{
				CAnim_HammerLoop->SetEnableObjects(false);				
				hammer->Show(true);
				isNailWrongPluckingAnimPlaying = false;

				isSpeedMeterRunning = true;
			}
			//return;
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerPlacedOnScene && NumberOfNailsPlucked < 3)
		{
			if(RectHammer[clickedNailIndex]->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					if(isSpeedMeterRunning == false)
					{
						isSpeedMeterRunning = true;
						speedBar->Show(true);
						speedMeterBG->Show(true);
						const CRectangle  TmpRect(237.0f,366.0f,8.0f,8.0f);
						if(clickedNailIndex == 0)
						{
							speedMeterBG->Pos.x =  nails[clickedNailIndex]->Pos.x + 200;
						}
						else
						{
							speedMeterBG->Pos.x =  nails[clickedNailIndex]->Pos.x - 150;
						}
						speedMeterBG->Pos.y =  nails[clickedNailIndex]->Pos.y - 150;

						speedBarStarPos.x  = speedMeterBG->Pos.x - 100;
						speedBar->Pos.x = speedBarStarPos.x;
						speedBar->Pos.y = speedMeterBG->Pos.y;
						speedBarEndPos.x = speedMeterBG->Pos.x + 100;
						barSpeed = 2.0f + NumberOfNailsPlucked;
						nails[clickedNailIndex]->Show(false);
						CAnim_HammerLoop->SetPosOffset((int)hammerOffset[clickedNailIndex].x, (int)hammerOffset[clickedNailIndex].y);
					}
					else
					{
						float diff = abs(speedMeterBG->Pos.x - speedBar->Pos.x);
						if(diff < 40)
						{
							isSpeedMeterRunning = false;
							hammer->Show(false);
							NailPluckingAnim->Show(true);
							NailPluckingAnim->ForcePlayAnimation(0);
							NailPluckingAnim->Pos = NailPluckingAnimPos[clickedNailIndex];
							isNailPluckingAnimPlaying = true;
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5HammerUsed);

							if(isNailWrongPluckingAnimPlaying)
							{
								isNailWrongPluckingAnimPlaying = false;
								CAnim_HammerLoop->SetEnableObjects(false);
							}
							return;
						}
						else
						{
							
							hammer->Show(false);
							isSpeedMeterRunning = false;
							speedMeterBG->InitPos = speedMeterBG->Pos;
							speedMeterBG->Shake(0, 5.0f, 0, 0.35f);
							CAnim_HammerLoop->PlayAnim();
							isNailWrongPluckingAnimPlaying = true;
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5HammerUsed);
							return;
						}
					}
				}
			}
		}

		if(isMapMoving)
		{
			movingTimer += Control::LogicRate;
			float xPos = mapStartPosX + (map2->Pos.x - mapStartPosX) * easing(movingTimer, 0, 1, moveTime);
			float yPos = mapStartPosY + (map2->Pos.y - mapStartPosY ) * easing(movingTimer, 0, 1, moveTime);

			map1->Pos = CPoint(xPos,yPos,0);
			if(movingTimer >= moveTime)
			{
				map1->Show(false);
				map2->Show(true);
				isMapMoving = false;
			}

			return;
		}

		if(isObjectReturning)
		{
			returnTimer += Control::LogicRate;

			CPoint temp;
		
			temp.x = startPos.x + (endPos.x - startPos.x) * easing(returnTimer, 0, 1, returnEndTimer);
			temp.y = startPos.y + (endPos.y - startPos.y) * easing(returnTimer, 0, 1, returnEndTimer);

			returnObject->Pos = temp;
			if(returnTimer >= returnEndTimer)
			{
				getObject[objectID]->Show(true);

				returnObject->Show(false);
				returnObject = NULL;
				isObjectReturning = false;
				returnTimer = 0.0f;

				if(objectID == 3 && isCrystallAddedToGrinder)
				{
					grinder->Show(true);
					crystalInGrinder1->Show(true);
					isCrystallAddedToGrinder = false;
				}

				if(objectID == 4 && isSpoonWithBluePowderInHand)
				{
					spoon->Show(true);
					isSpoonWithBluePowderInHand = false;
				}
			
				if(objectID == 5 && isSpoonWithGreenPowderInHand)
				{
					spoon->Show(true);
					isSpoonWithGreenPowderInHand = false;
				}
			}
			return;
		}

		if(isSpoonWithBluePowderInHand )
		{
			if( Control::Input->LBclicked())
			{
				if(polyPlate->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate)
				{
					pointerInHand->Show(false);
					spoon->FadeIn(1.0f);
					bluePowderOnPlate->FadeIn(4.0f);
					isSpoonWithBluePowderInHand = false;
					isObjectInHand = false;
					ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate = true;
					return;
				}
				else
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate && polyPlate->Intersection(&pos))
					{
						Hud->ShowBannerText("L1first");
					}
					isObjectInHand = false;
					isObjectReturning = true;
					returnObject = pointerInHand;
					returnTimer = 0.0f;
					startPos = pointerInHand->Pos;
					endPos  = spoon->Pos;

					float distance = (float)sqrt( ((startPos.x - endPos.x)*(startPos.x - endPos.x)) + ((startPos.y - endPos.y) * (startPos.y - endPos.y)));
					returnEndTimer = distance / 1000;
					return;
				}
			}
		}

		if(isGreenPowderDropAnimPlaying)
		{
			if(GreenPowderDropAnim->IsAnimationFinished())
			{
				isGreenPowderDropAnimPlaying = false;
				GreenPowderDropAnim->Show(false);
				//spoon->FadeIn(1.0f);
				if( !ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull )
				{
					isObjectInHand = true;
					pointerInHand = new CObject();
					pointerInHand->LoadSprite(mousePointerString[5]);
					pointerInHand->Show(true);
					pointerInHand->PlayAnimation(1);
					pointerInHand->SetAlpha(0.0f);
					pointerInHand->FadeIn(4.0f);
					pointerInHand->Pos = getObject[5]->Pos;
					pointerInHand->SetZ(200);
				}
			}
			return;
		}

		if(isSpoonWithGreenPowderInHand )
		{
			if( Control::Input->LBclicked())
			{
				if(polyBottle->Intersection(&pos))
				{
					pointerInHand->Show(false);
					isSpoonWithGreenPowderInHand = false;
					isObjectInHand = false;
					
					isGreenPowderDropAnimPlaying = true;
					GreenPowderDropAnim->Show(true);
					GreenPowderDropAnim->ForcePlayAnimation(0);

					

					if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleHalf)
					{
						ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleHalf = true;
						HOScene->GetObjectByID("bottleHalf")->FadeIn(1.0f);
						
					}
					else if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleTreeForth)
					{
						ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleTreeForth = true;
						HOScene->GetObjectByID("bottleThreeforth")->FadeIn(1.0f);
					}
					else
					{
						HOScene->GetObjectByID("bottleFull")->FadeIn(1.0f);
						ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull = true;
						HOScene->GetObjectByID("greenpowderspill")->FadeIn(1.0f);
						HOScene->GetObjectByID("greenpowderfill")->FadeIn(1.0f);
					}
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5DynamiteCrystalInBottle);
					return;
				}
				else
				{
					isObjectInHand = false;
					isObjectReturning = true;
					returnObject = pointerInHand;
					returnTimer = 0.0f;
					startPos = pointerInHand->Pos;
					endPos  = spoon->Pos;

					float distance = (float)sqrt( ((startPos.x - endPos.x)*(startPos.x - endPos.x)) + ((startPos.y - endPos.y) * (startPos.y - endPos.y)));
					returnEndTimer = distance / 1000;
					return;
				}
			}
		}

		if(isCrystalTakeAnimPlaying)
		{
			if(CrystalTakeAnim->IsAnimationFinished())
			{
				isCrystalTakeAnimPlaying = false;

				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder = true;
				isCrystallAddedToGrinder = true;
				CrystalTakeAnim->Show(false);
				
				isObjectReturning = true;
				returnObject = setObject[3];
				returnTimer = 0.0f;
				startPos = CPoint(255.0f,273.0f,0.0f);
				endPos  = getObject[objectID]->Pos;
				returnObject->Pos = startPos;
				setObject[3]->Show(true);

				float distance = (float)sqrt( ((startPos.x - endPos.x)*(startPos.x - endPos.x)) + ((startPos.y - endPos.y) * (startPos.y - endPos.y)));
				returnEndTimer = distance / 1000;
			}
			return;
		}
		if(isGrinderAnimPlaying)
		{
			if(  CAnim_Grinder->IsAnimFinished() ) // GrinderAnim->IsAnimationFinished())
			{
				//CAnim_Grinder->SetEnableObjects(false);
				isGrinderAnimPlaying = false;
				setObject[2]->Show(true);
				crystalInGrinder1->Show(false);
				crystalInGrinder3->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated = true;
			}
			return;
		}
		if( ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated  && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate && !isObjectInHand )
		{
			if(crystalInGrinder1->Collide(&pos)  )
			{
				if( Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L1bare");
				}
			}
		}


	

		if(isbluePowderTakeAnimPlaying)
		{
			if( CAnim_BluePowderTake->IsAnimFinished())
			{
				CAnim_BluePowderTake->SetEnableObjects(false);
				isbluePowderTakeAnimPlaying = false;
				setObject[4]->Show(true);
				pointerInHand = setObject[4];
				isSpoonWithBluePowderInHand = true;
			}
			return;
		}

		if(isAmoniMixAnimPlaying)
		{
			if(AmoniMixAnim->IsAnimationFinished())
			{
				AmoniMixAnim->Show(false);
				isAmoniMixAnimPlaying = false;
				setObject[6]->Show(true);
				bluePowderOnPlate->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate = true;
			}
			return;
		}

		if(isGreenPowderTakeAnimPlaying)
		{
			if(GreenPowderTakeAnim->IsAnimationFinished())
			{
				GreenPowderTakeAnim->Show(false);
				isGreenPowderTakeAnimPlaying = false;
				isObjectInHand = true;
				setObject[5]->Show(true);
				pointerInHand = setObject[5];
				isSpoonWithGreenPowderInHand = true;
			}
			return;
		}
		if( ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate && !ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleHalf && !isObjectInHand )
		{
			if( setPoly[5]->Intersection(&pos) )
			{
				if( Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L1bare");
				}
			}

		}
		
		if(isCapBottleAnimPlaying)
		{
			if(CapBottleAnim->IsAnimationFinished())
			{
				CapBottleAnim->Show(false);
				isCapBottleAnimPlaying = false;
				setObject[1]->Show(true);
				threadOnBottle->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle = true;
				Hud->InvBox_StopPuzzleMode();
			}
			return;
		}
		
		if(isObjectInHand)
		{
			pointerInHand->SetPos(&pos);

			if( Control::Input->LBclicked())
			{
				switch (objectID)
				{
					case 0:
						if(setPoly[0]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle && ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull)
						{
							isObjectInHand = false;
							setObject[0]->Show(true);
							pointerInHand->Show(false);
							ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle = true;
							return;
						}
						else if( setPoly[0]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull )
						{
							Hud->ShowBannerText("L1container");
						}
						break;
					case 1:
						if(setPoly[1]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle && !ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle)
						{
							isObjectInHand = false;
							pointerInHand->Show(false);
							isCapBottleAnimPlaying = true;
							CapBottleAnim->Show(true);
							HOScene->GetObjectByID("greenpowderfill")->Show(false);
							setObject[0]->Show(false);
							CapBottleAnim->ForcePlayAnimation(0);
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5DynamiteBottleCapPlaced);
							return;
						}
						else if( setPoly[1]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle && ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull )
						{
							Hud->ShowBannerText("L1wick");
						}
						else if( setPoly[1]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull )
						{
							Hud->ShowBannerText("L1container");
						}
						break;
					case 2:
						if(setPoly[2]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated)
						{
							isObjectInHand = false;
							pointerInHand->Show(false);
							isGrinderAnimPlaying = true;
							
							grinderFine->Show(true);
							CAnim_Grinder->PlayAnim();

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5DynamiteCrystalCrush);
							return;
						}
						break;
					case 3:
						if(setPoly[3]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened)
						{
							isObjectInHand = false;
							pointerInHand->Show(false);
							isCrystalTakeAnimPlaying = true;
							CrystalTakeAnim->Show(true);
							CrystalTakeAnim->ForcePlayAnimation(0);
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5DynamiteCrystalsCollected);
							return;
						}
						break;
					case 4:
						if(setPoly[4]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate && isSpoonWithBluePowderInHand == false)
						{
							isObjectInHand = true;
							pointerInHand->Show(false);
							isbluePowderTakeAnimPlaying = true;
							CAnim_BluePowderTake->PlayAnim();
							return;
						}
						else if(setPoly[2]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated)
						{
							Hud->ShowBannerText("L1follow");
						}
						break;
					case 5:
						if(setPoly[5]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate && !ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull && !isSpoonWithGreenPowderInHand)
						{
							isGreenPowderTakeAnimPlaying = true;
							pointerInHand->PlayAnimation(0);
							pointerInHand->Show(false);
							GreenPowderTakeAnim->Show(true);
							GreenPowderTakeAnim->ForcePlayAnimation(0);
							return;
						}
						break;
					case 6:
						if(setPoly[6]->Intersection(&pos)  && !ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate)
						{
							isObjectInHand = false;
							pointerInHand->Show(false);
							pointerInHand->PlayAnimation(0);
							isAmoniMixAnimPlaying = true;
							AmoniMixAnim->Show(true);
							AmoniMixAnim->ForcePlayAnimation(0);
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5DynamiteAcidPour);
							return;
						}
						else
						{
							if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate)
							{
								Hud->ShowBannerText("L1first");
								//return;
							}
							else if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate)
							{
								Hud->ShowBannerText("L1follow");
								//return;
							}
						}
						break;
				}
			
				isObjectInHand = false;
				isObjectReturning = true;
				returnObject = pointerInHand;
				returnTimer = 0.0f;
				startPos = pointerInHand->Pos;
				endPos  = getObject[objectID]->Pos;

				float distance = (float)sqrt( ((startPos.x - endPos.x)*(startPos.x - endPos.x)) + ((startPos.y - endPos.y) * (startPos.y - endPos.y)));
				returnEndTimer = distance / 1000;
			
			}
			return;
		}
		else
		{
			if(polyBoxDoor->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened && NumberOfNailsPlucked < 3 && !ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerPlacedOnScene )
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					Control::Audio->PlaySample(aSFXWoodShake);
					closedBoxDoor->Shake(90.0f,2.0f,0,0.25f);
					HOScene->GetObjectByID("hammerhead")->Shake(90.0f,1.0f,0,0.35f);
					Hud->ShowBannerText("L1tool",SFX_NIL);
					return;
				}
			}

			if(setPoly[0]->Intersection(&pos) && (!ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull || !ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle || !ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle) )
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull && !ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle)
					{
						Hud->ShowBannerText("L1follow");
					}
					if(ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull && ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle && !ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle)
					{
						Hud->ShowBannerText("L1follow");
					}
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull)
					{
						Hud->ShowBannerText("L1follow");
					}
				}
				//Thread
				return;
			}

			if(setPoly[4]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate)
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L1bare");
				}
			}
			
			if(polyGrinderGet->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated)
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated)
					{
						Hud->ShowBannerText("L1follow");
						return;
					}
				}
			}

			if(setPoly[3]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened)
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L1better");
				}
			}
					
			if(polyPlate->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate)
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate )
					{
						Hud->ShowBannerText("L1follow");
						return;
					}
					if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate && !ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate)
					{
						Hud->ShowBannerText("L1chemicals");
						return;
					}
				}

				
				//add crystal powder to plate
				return;
			}
			
		}

		bool isRightClick = false;

		for (int i = 0; i < 7; i++)
		{
			switch (i)
			{
				case 0:
					if(getPoly[0]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle )
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked())
						{
							isRightClick = true;
							objectID = 0;
							break;
						}
					}
					break;
				case 1:
					if(getPoly[1]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle )
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked())
						{
							isRightClick = true;
							objectID = 1;
							break;
						}
					}
					break;
				case 2:
					if(getPoly[2]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated )
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked())
						{
							isRightClick = true;
							objectID = 2;
							break;
						}
					}
					break;
				case 3:
					if(getPoly[3]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder )
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked())
						{
							isRightClick = true;
							objectID = 3;
							break;
						}
					}
					break;
				case 4:
					if(getPoly[4]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate)
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked())
						{
							Control::Audio->PlaySample(aSFXL1S4CoinPicked,false);
							isRightClick = true;
							objectID = 4;
							break;
						}
					}
					break;
				case 5:
					if(getPoly[5]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate &&  !ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull)
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked())
						{
							isRightClick = true;
							objectID = 5;
							break;
						}
					}
					break;
				case 6:
					if(getPoly[6]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBottleCapRemoved)
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked())
						{
							isRightClick = true;
							objectID = 6;
							break;
						}
					}
					break;
			}

			if(isRightClick)
			{
				isObjectInHand = true;

				getObject[objectID]->Show(false);
				pointerInHand = new CObject();
				pointerInHand->LoadSprite(mousePointerString[objectID]);
				pointerInHand->Show(true);
				if( objectID == 4 || objectID == 5 || objectID == 1 )
				{
					pointerInHand->PlayAnimation(1);
				}

				pointerInHand->Pos = getObject[objectID]->Pos;
				pointerInHand->SetZ(200);
				return;
			}
			
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened && NumberOfNailsPlucked == 3 )
		{
			if(polyBoxDoor->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5DynamiteBoxOpen);

					closedBoxDoor->FadeOut(3.0f);
					openedBoxDoor->FadeIn(3.0f);
					ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened = true;
					ResetBtn->SetEnable(true);

					//Show the Notice first
					//Popup1_State = POPUP_SCALEUP;
				//	ActivePopupID = ePopL1S5_P3HelpManual;
					//Control::Audio->PlaySample(aSFXpopup);
					//PopupAlpha = 0.2f;
					//SetupPopupTransition(Popup1Notice, PopRectNotice);
					if( ProfileSystem::Instance->ActiveProfile.L1S5_P1MortarPestlePlaced)
						Hud->InvBox_StartPuzzleMode();
					return;
				}
			}
		}

		if(polyAmoniBottleCap->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBottleCapRemoved )
		{
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5DynamiteAcidCapOpen);

				amoniBottleCork->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBottleCapRemoved = true;
				return;
			}
		}
	
		if(polyMap1->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate )
		{
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5ClothMove);
				isMapMoving = true;
				mapStartPosX = map1->Pos.x;
				mapStartPosY = map1->Pos.y;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate = true;
				movingTimer = 0.0f;
				moveTime = 0.5f;
				return;
			}
		}
	}

	if( !IsPopupTransitionActive )
		UpdatePopups(pos);

#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (ActivePopupID == ePopInactive && Control::Input->isKeyBackSensed())
	{
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
		return;
	}
#endif

	if( ActivePopupID == ePopInactive && IsMouseOverGoBackArea(&pos ))
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitDown,eArea_L1_S5_TreasureRoom);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L1_S5_TreasureRoom);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L1_S5_TreasureRoom, 2);
			}
#else
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}			
		return;
	}
}

void L1_S5_PZDynamiteMaking::UpdatePopupTransition()
{
	if( ActivePopupID == ePopL1S5_P3HelpManual)
	{
		switch( Popup1_State )
		{
		case POPUP_SCALEUP:
			PopupAlpha += POPUP_SCALE_FACTOR *  Control::LogicRate;
			if( PopupAlpha >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;				
				Popup1Notice->Show(true);
				Popup1_State = POPUP_OPEN;
			}			
			if( PopupAlpha < 0.5f )
				GFInstance->UpdateBlurScene(2 * PopupAlpha);
			else
				GFInstance->UpdateBlurScene(1.0f);
			break;
		case POPUP_SCALEDOWN:
			
			PopupAlpha -= POPUP_SCALE_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;								
				GFInstance->TurnOffBlurScene();
				Popup1_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;
				Popup1Notice->Show(false);
				//EndBgGraphicScaling();
				
				SetZ(zOrderScnVideo);
			}		
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;

		}
	}
}

void L1_S5_PZDynamiteMaking::UpdatePopups(CPoint pos,bool ItemDropped)
{
	bool clicked = Control::Input->LBclicked();
	if( (ActivePopupID == ePopInactive || ActivePopupID == ePopL1S5_P3HelpManual))
	{
		switch( Popup1_State )
		{
		case POPUP_CLOSED:
			if( polyNotice->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened )
			{
				Cursor::SetMode(CA_Look);
				if( clicked || ItemDropped)
				{
					Popup1_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S5_P3HelpManual;
					Control::Audio->PlaySample(aSFXpopup);
					PopupAlpha = 0.2f;
					SetupPopupTransition(Popup1Notice, PopRectNotice);
				}
				return;
			}
			break;

		case POPUP_OPEN:
			Cursor::SetMode(CA_HandPointy);
			NH4->Show(true);
			HNO3->Show(true);
			NH4NO3->Show(true);
			minigrinderTxt->Show(true);
			threadTxt->Show(true);
			waxTxt->Show(true);
			if( clicked )
			{
				Popup1_State = POPUP_SCALEDOWN;
				Popup1Notice->Show(false);
				NH4->Show(false);
				HNO3->Show(false);
				NH4NO3->Show(false);
				minigrinderTxt->Show(false);
				threadTxt->Show(false);
				waxTxt->Show(false);
		
				SetupPopupTransition(Popup1Notice, PopRectNotice);
			}
			break;
		}
	}
}

void L1_S5_PZDynamiteMaking::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;

	switch (ItemID)
	{
	case eInv_L1S5_HammerHead:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_HammerHead] = true;
			ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken = true;			
			vecInvIds.push_back(eInv_L1S5_HammerHead);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
		break;
	case eInv_L1S5_Dynamite:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_Dynamite] = true;
			ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken = true;			
			vecInvIds.push_back(eInv_L1S5_Dynamite);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
		break;
	
	default:
		break;
	}	
}

void L1_S5_PZDynamiteMaking::Render(const CPoint* pCam)
{  
	static int FrameCnt = 0;
	FrameCnt = (FrameCnt+1)%2;
	
	if (ActivePopupID != ePopInactive)
	{
		//No Videos Active when Popup is active
		if(IsPopupTransitionActive)
		{
			//Init again if render target lost due to fullscreen-window switches or similar
			if( GFInstance->RTGraphic != NULL )
				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),
				//0.2f,0.6f); //test gap between frames
				PopupAlpha,PopupAlpha);
			return;
        }
		return; //No BG Video when popups are active
	}
}

L1_S5_PZDynamiteMaking::~L1_S5_PZDynamiteMaking()
{
	

	Hud->InvBox_StopPuzzleMode();

	int size =(int) VFXArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++){
		SAFE_DELETE(VFXArray[i]);
	}
	 size = (int)polyArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(polyArray[i]);
	}

	SAFE_DELETE(RectHammer[0]);
	SAFE_DELETE(RectHammer[1]);
	SAFE_DELETE(RectHammer[2]);

	SAFE_DELETE(hammer);
	SAFE_DELETE(NailPluckingAnim);
	SAFE_DELETE(GreenPowderTakeAnim);
	SAFE_DELETE(CrystalTakeAnim);
	SAFE_DELETE(CapBottleAnim);
	SAFE_DELETE(GreenPowderDropAnim);

	SAFE_DELETE(Popup1Notice);

	SAFE_DELETE(pointerInHand);
	SAFE_DELETE(ResetDia);

	SAFE_DELETE(NH4);
	SAFE_DELETE(HNO3);
	SAFE_DELETE(NH4NO3);
	SAFE_DELETE(minigrinderTxt);
	SAFE_DELETE(threadTxt);
	SAFE_DELETE(waxTxt);
	
	//MUSIC_SFX
	Control::Audio->UnloadSample(aSFXL1S4CoinPicked);
	Control::Audio->UnloadSample(aSFXL1S4BooksMoved);

	Control::Audio->StopSample(aAmbS5PuzDynamite);
	Control::Audio->UnloadSample(aAmbS5PuzDynamite);
	//MUSIC_SFX
}

void L1_S5_PZDynamiteMaking::ReceiveMessage(int val)
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
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				UpdatePopups(dropPos,true);
				if(ActivePopupID == ePopInactive)
				{
					//If No popups responded
					if( itemVal == eInv_L1CB_Hammer )
					{
						for (int i = 0; i < 3; i++)
						{
							if(nailArea[i].Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[i] == false && !ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerPlacedOnScene)
							{
								hammer->Show(true);
								hammer->Pos = hammerPos[i];
								ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerPlacedOnScene = true;
								clickedNailIndex = i;
								ProfileSystem::Instance->ActiveProfile.L1S5_SelectedNailIndex = clickedNailIndex;

								Hud->ReleaseDrag();
								Hud->CloseInventory(); 

								ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1CB_Hammer] = false;
								Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1CB_Hammer);
						
							}
						}
					}
					else if( itemVal == eInv_L1S5_MortarPestle )
					{
						
						if( !ProfileSystem::Instance->ActiveProfile.L1S5_P1MortarPestlePlaced)
						{
							grinder->SetEnable(true);
							rode->SetEnable(true);

							ProfileSystem::Instance->ActiveProfile.L1S5_P1MortarPestlePlaced = true;
							
							Control::Audio->PlaySample(aSFXL1S4BooksMoved);
							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_MortarPestle] = false;
							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S5_MortarPestle);
							if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened)
								Hud->InvBox_StartPuzzleMode();
						
						}
						
					}
					else
					{
						GFInstance->WrongInvDropMsg(Hud);
					}
					
				}
				return;
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L1_S5_PZDynamiteMaking::ClosePopup()
{
	
}

void L1_S5_PZDynamiteMaking::ProcessNonHOHint()
{
	if(isNailPluckingAnimPlaying || isNailWrongPluckingAnimPlaying)
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 9;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			
			case 0: ++iNonHOHint;
					if((!ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken || !ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken)  && ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
					{
						InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
						return;
					}
					break;
			case 1: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken)
					{
						InitHintTrailEmit(InvItemRectHammerHead.topleft.x + InvItemRectHammerHead.w/2, InvItemRectHammerHead.topleft.y + InvItemRectHammerHead.h/2,false);
						return;
					}
					break;
			case 2: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_IsHammerHandFixed && ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken && ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
						Hud->InvBoxCtrl->Activate();																								  
						
						for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
						{
							if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
							{	
								if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S5_HammerHead)
								{
									if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
											Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
									else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
											Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
									InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,true);
								}
							}
						}
											
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[0] && ProfileSystem::Instance->ActiveProfile.L1S5_IsHammerHandFixed)
					{
						InitHintTrailEmit(nailArea[0].GetCenterX(), nailArea[0].GetCenterY(),false);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[1] && ProfileSystem::Instance->ActiveProfile.L1S5_IsHammerHandFixed)
					{
						InitHintTrailEmit(nailArea[1].GetCenterX(), nailArea[1].GetCenterY(),false);
						return;
					}
					break;
			case 5: ++iNonHOHint;
				   if( !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[2] && ProfileSystem::Instance->ActiveProfile.L1S5_IsHammerHandFixed)
					{
						if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerPlacedOnScene)
						{
							Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1CB_Hammer)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (nailArea[2].GetCenterX(), nailArea[2].GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						}
						else
							InitHintTrailEmit(nailArea[2].GetCenterX(), nailArea[2].GetCenterY(),false);
						return;
					}
					break;
			case 6: ++iNonHOHint;
				   if( !ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[2] && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[1] && ProfileSystem::Instance->ActiveProfile.L1S5_P1IsNailPlucked[0])
					{
						InitHintTrailEmit(closedBoxDoor->Pos.x,closedBoxDoor->Pos.y,false);
						return;
					}
					break;
			case 7: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_P1MortarPestlePlaced && ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
						Hud->InvBoxCtrl->Activate();																								  
						
						for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
						{
							if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
							{	
								if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S5_MortarPestle)
								{
									if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
											Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
									else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
											Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
									InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,true);
								}
							}
						}
						CPoint tempPoint (grinder->Pos);
						Hud->Hint2FX->ResetPos(&tempPoint);
						Hud->Hint2FX->Emit();					
						return;
					}
					break;
			case 8: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle && !ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken)
					{
						InitHintTrailEmit(bottle->Pos.x,bottle->Pos.y,false);
						return;
					}
					break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void  L1_S5_PZDynamiteMaking::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{

		bgScaleOffsetX = popRect.GetCenterX();
		bgScaleOffsetY = popRect.GetCenterY();

		IsPopupTransitionActive = true;
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
		GFInstance->SetPopupTween( (int)popRect.GetCenterX(-683) ,(int)popRect.GetCenterY(-384),171,0);

		GFInstance->CreateGreyscaleScene();

		SetZ(zOrderPopVideo);
}

void L1_S5_PZDynamiteMaking::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened)
	{
		ResetBtn->SetEnable(false);
		return;
	}

	if(timer > 0)
	{
		timer -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
		ResetBtn->SetEnable(false);
		if(!IsResetPop)
		{
			if(!isResetDiaFadeOut)
				ResetDia->FadeOut(2);
			isResetDiaFadeOut = true;
		}
		return;
	}
	else
	{
		ResetBtn->SetEnable(true);	
		if(!isFadeIn)
			ResetBtn->FadeIn(1);
		isFadeIn = true;
	}
	if(ResetBtn->IsIntersect(Control::Input->Mpos()))
	{
		ResetBtn->SetAnimation(1);
		if(!IsResetPop)
			Cursor::SetMode(CA_HandPointy);
		if(KInput::getLeftButtonState() == ISDOWN)
		{
			IsResetPop = true;
		}
	}
	else
	{
		ResetBtn->SetAnimation(2);
		Cursor::SetMode(CA_Normal);
	}

	if(IsResetPop)
	{
		if(!isResetDiaFadeIn)
		{
			ResetDia->FadeIn(2);
		}
		if(!isFadeOut)
			ResetBtn->FadeOut(1);
		isFadeOut = true;
		isResetDiaFadeIn = true;
		CObject *resetYes = ResetDia->GetObjectByID("BtnYes");
		CObject *resetNo = ResetDia->GetObjectByID("BtnNo");

		if(Hud->CheckPopupButton(resetYes) == Popup_Click)
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened && ProfileSystem::Instance->ActiveProfile.L1S5_P1MortarPestlePlaced)
			{
				pointerInHand->Show(false);
				pointerInHand = new CObject();

				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBottleCapRemoved = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleHalf = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleTreeForth = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1GreenPowderAddedToBottleFull = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle = false;
				ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle = false;

				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_PZDynamiteMaking;
				SendMessage(ControlExit, ID, HOSM_NewArea);
				IsResetPop = false;
			}
		}
		if(Hud->CheckPopupButton(resetNo) == Popup_Click)
		{
			IsResetPop = false;
			timer = 10.0f;
			isFadeIn = false;
			isFadeOut = false;
			isResetDiaFadeOut = false;
			isResetDiaFadeIn = false;
		}
	}
	else
	{
		ResetBtn->SetEnable(true);
	}
}
void L1_S5_PZDynamiteMaking::NavigateToScene()
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