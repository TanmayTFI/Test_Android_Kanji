//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 04 :  SECRETDOOR HO
//====================================

#include "L1_S3_DoorHO.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Fonts.h"
#include "AwardManager.h"

const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,98,0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  areaRectMatch3(994.0f,25.0f,171.0f,50.0f);

const CRectangle  hintImage0(377.0f,669.0f,73.0f,33.0f);
const CRectangle  hintImage1(478.0f,667.0f,69.0f,34.0f);
const CRectangle  hintImage2(575.0f,666.0f,116.0f,36.0f);
const CRectangle  hintImage3(742.0f,666.0f,129.0f,36.0f);
const CRectangle  hintImage4(376.0f,703.0f,74.0f,32.0f);
const CRectangle  hintImage5(475.0f,703.0f,73.0f,32.0f);
const CRectangle  hintImage6(576.0f,704.0f,146.0f,31.0f);
const CRectangle  hintImage7(739.0f,703.0f,116.0f,32.0f);
const CRectangle  hintImage8(376.0f,736.0f,77.0f,36.0f);
const CRectangle  hintImage9(473.0f,736.0f,93.0f,32.0f);
const CRectangle  hintImage10(575.0f,736.0f,129.0f,36.0f);
const CRectangle  hintImage11(739.0f,738.0f,92.0f,32.0f);

const CRectangle  lockAreaRect(601.0f,244.0f,216.0f,163.0f);

//Scene Rects

//Inventory Destination Rects
const CRectangle  InvDestRectHOList(411.0f,94.0f,593.0f,403.0f);
const CRectangle  HOBoundsRect(236.0f,28.0f,887.0f,500.0f);

const CRectangle  ActionRectHOListTake(600.0f,239.0f,216.0f,170.0f);

//Inventory Item Rects
const CPoint duckPoint(291.0f,503.0f,0.0f);
const CPoint payalPoint(941.0f,502.0f,0.0f);

//Video Rects
const CRectangle  VidRectFull(0,0,1366,768);

L1_S3_DoorHO::L1_S3_DoorHO()
{
	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S3_DoorHO])
		AwardManager::GetInstance()->StartHO(eArea_L1_S3_DoorHO);
	isSymbolText = false;
	musicFadeOutTimer = 0;
	match3 = NULL;
	isMatch3Playing = false;
	isHOSolved = false;

	hoTimer = 0.0f;
	
	swapTime = 0.70f;

	isObjectReturning = false;
	isObjectInHand = false;
	isLockSelected = false;
	Visible = false;
	currentLockPos = 3;
	isLockMoving = false;
	isLockPuzzleSolved = false;
	isShutterOpening = false;
	isLockMovedOnce = false;
	isStarDoorOpening = false;

	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	InitializeText();
	UpdateText();

	//first step to be done to avoid render deadlock

	keyValues[0].x = 1;
	keyValues[0].y = 2;

	keyValues[1].x = 4;
	keyValues[1].y = 5;

	keyValues[2].x = 0;
	keyValues[2].y = 6;

	for (int i = 0; i < 12; i++)
	{
		imageState[i] = false;
	}
	
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S3\\HO\\DOORHO.SCN");

	leaves[0] = HOScene->GetObjectByID("leaf1");
	leaves[1] = HOScene->GetObjectByID("leaf2");
	leaves[2] = HOScene->GetObjectByID("leaf3");

	for (int i = 0; i < 3; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[i])
		{
			leaves[i]->Show(false);
		}
	}
	
	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);	
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

	ResetDia = new CHOScene();
	ResetDia->Load("UI\\RESETDIA.SCN");

	CObject* resetText = new CObject();
	resetText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\RESETCNF.SPR", 670, 308, 2);
	resetText->SetUID("resetText");
	ResetDia->PushObject(resetText);

	CObject* _mask = new CObject();
	_mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
	_mask->SetUID("_mask");
	_mask->SetScale(2);
	ResetDia->PushObject(_mask);

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

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

	rose = HOScene->GetObjectByID("rose");
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HORoseFlowerFound)
	{
		rose->Show(false);
	}
	crescent = HOScene->GetObjectByID("moon");
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOCrescentFound)
	{
		crescent->Show(false);
	}
	snake = HOScene->GetObjectByID("snake");
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOSnakeFound)
	{
		snake->Show(false);
	}
	mask = HOScene->GetObjectByID("mask");
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOMaskFound)
	{
		mask->Show(false);
	}
	owl = HOScene->GetObjectByID("owl");
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOOwlFound)
	{
		owl->Show(false);
	}
	
	lionhead = HOScene->GetObjectByID("lionhead");
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLionHeadFound)
	{
		lionhead->Show(false);
	}

	Stars[0] = HOScene->GetObjectByID("star1");
	Stars[1] = HOScene->GetObjectByID("star2");

	for (int i = 0; i < 2; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[i])
		{
			Stars[i]->Show(false);
		}
	}
	
	tree = HOScene->GetObjectByID("tree");
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOTreeFound)
	{
		tree->Show(false);
	}
	goddes = HOScene->GetObjectByID("goddes");
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOGoddessFound)
	{
		goddes->Show(false);
	}
	turtle = HOScene->GetObjectByID("turtle");
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOTurtleFound)
	{
		turtle->Show(false);
	}
	
	fishhead = HOScene->GetObjectByID("fishhead");
	fishtail = HOScene->GetObjectByID("fishtale");
	fish = HOScene->GetObjectByID("fish");
	
	if(ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed && !ProfileSystem::Instance->ActiveProfile.L1S3_HOFishFound)
	{
		fishhead->Show(false);
		fishtail->Show(false);
		fish->Show(true);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOFishFound)
	{
		fish->Show(false);
		fishhead->Show(false);
		fishtail->Show(false);
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed)
	{
		fish->Show(false);
	}

	keyInside = HOScene->GetObjectByID("keyInside");
	btmLDoor = HOScene->GetObjectByID("btmLDoor");
	btmRDoor = HOScene->GetObjectByID("btmRDoor");
	stardoor = HOScene->GetObjectByID("stardoor");

	CRectangle  maskrect(606.0f,486.0f,201.0f,55.0f);

	btmLDoor->SetMaskRect(maskrect);
	btmRDoor->SetMaskRect(maskrect);

	gemGlow[0] = HOScene->GetObjectByID("gemglow1");
	gemGlow[1] = HOScene->GetObjectByID("gemglow2");
	gemGlow[2] = HOScene->GetObjectByID("gemglow3");
	gemGlow[3] = HOScene->GetObjectByID("gemglow4");

	for (int i = 0; i < 4; i++)
	{
		gemGlow[i]->Show(false);
	}
	isMemoryPuzzleReversing = false;
	isMemoryPuzzleSolved = false;

	glowLeft = HOScene->GetObjectByID("glowLeft");
	glowRight = HOScene->GetObjectByID("glowRight");
	glowTop = HOScene->GetObjectByID("glowTop");
	glowDown = HOScene->GetObjectByID("glowDown");
	glowCircle = HOScene->GetObjectByID("glowCircle");
	glowRect = HOScene->GetObjectByID("glowRect");
	topLDoor = HOScene->GetObjectByID("topLDoor");
	topRDoor = HOScene->GetObjectByID("topRDoor");
	doorgembg = HOScene->GetObjectByID("doorgemsbg");

	CRectangle  maskrect2(605.0f,168.0f,203.0f,57.0f);

	topLDoor->SetMaskRect(maskrect2);
	topRDoor->SetMaskRect(maskrect2);

	keyfixed = HOScene->GetObjectByID("keyfixed");

	if(!ProfileSystem::Instance->ActiveProfile.L1S3_KeyFixed)
	{
		keyfixed->Show(false);
	}
	else
	{
		keyInside->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOMemoryPuzzleSolved)
	{
		doorgembg->Show(false);
	}

	stardoorOpened = HOScene->GetObjectByID("stardoorOpened");
	stardoorAnim = HOScene->GetObjectByID("stardoorAnim");
	stardoorAnim->Show(false);
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapPuzzleSolved)
	{
		stardoor->Show(false);
		stardoorOpened->Show(true);
	}
	else
	{
		stardoor->Show(true);
		stardoorOpened->Show(false);
	}

	pointerInHand = NULL;

	movingObjects[0] = fishtail;
	movingObjects[1] = keyInside;

	swapLeverSel[0] = HOScene->GetObjectByID("swapLeverSel1");
	swapLeverSel[0]->Show(false);
	swapLeverSel[1] = HOScene->GetObjectByID("swapLeverSel2");
	swapLeverSel[1]->Show(false);
	swapLeverSel[2] = HOScene->GetObjectByID("swapLeverSel3");
	swapLeverSel[2]->Show(false);
	swapLeverSel[3] = HOScene->GetObjectByID("swapLeverSel4");
	swapLeverSel[3]->Show(false);

	//txtMatch3 = HOScene->GetObjectByID("txtMatch3");
	//txtMatch3MouseOver = HOScene->GetObjectByID("txtMatch3MouseOver");
	//txtPuzzle = HOScene->GetObjectByID("txtPuzzle");
	//txtPuzzleMouseOver = HOScene->GetObjectByID("txtPuzzleMouseOver");


	match3Back = new CObject();
	match3Back->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
	match3Back->SetUID("match3Back");
	match3Back->Show(false);
	HOScene->PushObject(match3Back);

	match3BackText = new CObject();
	match3BackText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
	match3BackText->SetUID("match3BackText");
	match3BackText->Show(false);
	HOScene->PushObject(match3BackText);
#ifndef M3_BUTTON
	match3Back->SetEnable(false);
    match3BackText->SetEnable(false);
#endif

if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		match3Back->SetEnable(false);
		match3BackText->SetEnable(false);
	}

	txtMatch3 = new CObject();
	txtMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 1079, 56, 54);
	txtMatch3->SetUID("btnMatch3");
	HOScene->PushObject(txtMatch3);

	txtPuzzle = new CObject();
	txtPuzzle->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 1000, 65, 55);
	txtPuzzle->SetUID("btnSwitchToHO");
	HOScene->PushObject(txtPuzzle);

	txtMatch3MouseOver = new CObject();
	txtMatch3MouseOver->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 1079, 56, 56);
	txtMatch3MouseOver->SetUID("btnMatch3");
	txtMatch3MouseOver->PlayAnimation(1);
	HOScene->PushObject(txtMatch3MouseOver);

	txtPuzzleMouseOver = new CObject();
	txtPuzzleMouseOver->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 1000, 65, 58);
	txtPuzzleMouseOver->SetUID("btnSwitchToHO");
	txtPuzzleMouseOver->PlayAnimation(1);
	HOScene->PushObject(txtPuzzleMouseOver);

	swapLeverMouseOver = HOScene->GetObjectByID("swapLeverMouseOver");
	swapLeverMouseOver->Show(false);

	txtMatch3->SetZ(eZOrder_Popup + 300);
	txtPuzzle->SetZ(eZOrder_PopupOverPopup + 300);
	txtMatch3MouseOver->SetZ(eZOrder_Popup + 301);
	txtPuzzleMouseOver->SetZ(eZOrder_PopupOverPopup + 302);
	txtPuzzle->Show(false);
	txtMatch3MouseOver->Show(false);
	txtPuzzleMouseOver->Show(false);

	glowLeft->Show(false);
	glowRight->Show(false);
	glowTop->Show(false);
	glowDown->Show(false);
	glowCircle->Show(false);
	glowRect->Show(false);

	lockGlow[0] = glowTop;
	lockGlow[1] = glowCircle;
	lockGlow[2] = glowRight;
	lockGlow[4] = glowLeft;
	lockGlow[5] = glowRect;
	lockGlow[6] = glowDown;
	selectedLockPointer = -1;

	for (int i = 0; i < 3; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[i])
		{
			switch (i)
			{
				case 0:
					glowCircle->Show(true);
					glowRight->Show(true);
					break;
				case 1:
					glowRect->Show(true);
					glowLeft->Show(true);
					break;
				case 2:
					glowTop->Show(true);
					glowDown->Show(true);
					break;
			}
		}
	}
	isLeverSwaping = false;
	for (int i = 0; i < 2; i++)
	{
		swapLeverIndex[i] = -1;
	}

	swapHand[0] = HOScene->GetObjectByID("swaphand1");
	swapHand[1] = HOScene->GetObjectByID("swaphand2");
	swapHand[2] = HOScene->GetObjectByID("swaphand3");
	swapHand[3] = HOScene->GetObjectByID("swaphand4");

	for (int i = 0; i < 7; i++)
	{
		targetSel[i] = HOScene->GetObjectByID(std::string("targetSel" + MKSTR(i)).c_str());
		targetSel[i]->Show(false);
	}

	LockSel = HOScene->GetObjectByID("LockSel");
	LockSel->Show(false);

	for (int i = 0; i < 7; i++)
	{
		if(i != 3)
		{
			button[i] = HOScene->GetObjectByID(std::string("button" + MKSTR(i)).c_str());
			button[i]->Show(false);
		}
	}

	//HOScene->GetObjectByID("swappartsmask")->Show(false);
	//K_LOG ("Checkpoint 1 ");

	swapHand[0]->Pos.y += 20;
	swapHand[1]->Pos.y += 20;
	swapHand[2]->Pos.y += 20;
	swapHand[3]->Pos.y += 20;

	swapHand[0]->Pos.x -= 10;
	swapHand[1]->Pos.x -= 10;
	swapHand[2]->Pos.x -= 10;
	swapHand[3]->Pos.x -= 10;

	swapCurrectAngles[0] = 150.0f;
	swapCurrectAngles[1] = 110.0f;
	swapCurrectAngles[2] = 70.0f;
	swapCurrectAngles[3] = 30.0f;

	memoryPuzzleCorrectOrder[0] = 1;
	memoryPuzzleCorrectOrder[1] = 3;
	memoryPuzzleCorrectOrder[2] = 2;
	memoryPuzzleCorrectOrder[3] = 0;

	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOStarDoorOpened)
	{
		swapHand[0]->SetRotate(30.0f);
		swapHand[1]->SetRotate(150.0f);
		swapHand[2]->SetRotate(70.0f);
		swapHand[3]->SetRotate(110.0f);

		for (int i = 0; i < 4; i++)
		{
			ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapLeverAngle[i] = swapHand[i]->GetRotate();
		}
		ProfileSystem::Instance->ActiveProfile.L1S3_HOStarDoorOpened = true;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			swapHand[i]->SetRotate(ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapLeverAngle[i]);
		}
	}

	for (int i = 0; i < 12; i++)
	{
		hintImages[i] = NULL;
	}

	hintBG = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened)
	{
		hintBG = new CObject();
		hintBG->LoadSprite("LV1\\S3\\HO\\hintBG.SPR");
		hintBG->Pos = CPoint(946.0f,716.0f,0.0f);
		hintBG->SetZ(3001);
		//hintBG->Show(false);

		if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced)
			hintBG->Show(false);

		for (int i = 0; i < 12; i++)
		{
			hintImages[i] = new CObject();
			hintImages[i]->LoadSprite("LV1\\S3\\HO\\hintImage" + MKSTR(i) + ".SPR");
			hintImages[i]->Pos = CPoint(946.0f,716.0f,0.0f);
			hintImages[i]->SetZ(3000 + (i+2));
			hintImages[i]->Show(false);
		}
	}
	//sree
	hintImageRects[0] = hintImage0;
	hintImageRects[1] = hintImage1;
	hintImageRects[2] = hintImage2;
	hintImageRects[3] = hintImage3;
	hintImageRects[4] = hintImage4;
	hintImageRects[5] = hintImage5;
	hintImageRects[6] = hintImage6;
	hintImageRects[7] = hintImage7;
	hintImageRects[8] = hintImage8;
	hintImageRects[9] = hintImage9;
	hintImageRects[10] = hintImage10;
	hintImageRects[11] = hintImage11;

	if(ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened)
	{
		topLDoor->Show(false);
		topRDoor->Show(false);
		btmLDoor->Show(false);
		btmRDoor->Show(false);
	//	K_LOG ("L1S3_KeyDoorOpened ---> true");

		setHOTextVisible();

		txtPuzzleInfo->Show(true);
		txtPuzzleInfo->SetText(StringTable::getStr("L1puzzle"));
	}
	else
	{
	//	K_LOG ("L1S3_KeyDoorOpened ---> false");
		txtPuzzleInfo->Show(false);
	}

	updateImageState();

	int ArrayPts1[] = {726,119,713,89,729,71,752,78,762,101,749,118};
	polyRoseflower =  new CPolygon(6);
	polyRoseflower->Init(ArrayPts1);

	int ArrayPts2[] = {630,72,633,101,645,117,670,120,682,116,698,97,682,102,664,103,648,93};
	polyCrescent =  new CPolygon(9);
	polyCrescent->Init(ArrayPts2);

	int ArrayPts3[] = {1038,223,1058,215,1077,256,1080,293,1064,299,1043,282,1036,223};
	polySnake =  new CPolygon(7);
	polySnake->Init(ArrayPts3);
	
	int ArrayPts4[] = {483,411,483,388,488,369,498,355,512,359,532,382,524,391,510,400};
	polyMask =  new CPolygon(8);
	polyMask->Init(ArrayPts4);

	int ArrayPts5[] = {914,282,920,266,923,251,930,258,945,256,946,243,956,258,965,269,965,276,981,291,995,317,1001,339,998,341,988,337,983,342,971,336,955,338,936,327,923,307,920,293};
	polyOwl =  new CPolygon(20);
	polyOwl->Init(ArrayPts5);

	int ArrayPts6[] = {854,155,841,143,840,128,852,114,861,106,866,107,865,116,870,127,870,140,864,150};
	polyLeaves[0] =  new CPolygon(10);
	polyLeaves[0]->Init(ArrayPts6);

	int ArrayPts7[] = {423,524,420,546,423,559,433,568,450,568,454,555,450,540,436,534};
	polyLeaves[1]=  new CPolygon(8);
	polyLeaves[1]->Init(ArrayPts7);

	int ArrayPts8[] = {968,237,969,223,982,213,1001,213,1016,221,1003,232,994,241,981,246};
	polyLeaves[2] =  new CPolygon(8);
	polyLeaves[2]->Init(ArrayPts8);

	int ArrayPts9[] = {268,282,277,284,280,277,272,264,276,239,289,226,301,231,307,237,313,231,314,242,310,251,320,258,327,265,328,272,324,275,317,284,305,289,296,297,285,297,271,289};
	polyLionhead =  new CPolygon(20);
	polyLionhead->Init(ArrayPts9);

	int ArrayPts10[] = {660,456,671,434,719,450,745,439,747,448,744,458,749,466,744,472,721,464,705,472,695,479,674,478,661,469};
	polyFishTail =  new CPolygon(13);
	polyFishTail->Init(ArrayPts10);

	int ArrayPts11[] = {443,272,436,261,462,253,476,250,492,241,509,240,505,249,528,256,553,263,569,255,577,258,573,267,577,277,570,281,553,274,528,283,510,287,476,285,454,280};
	polyFish =  new CPolygon(19);
	polyFish->Init(ArrayPts11);

	int ArrayPts12[] = {539,206,519,201,535,193,533,173,548,186,564,178,557,197,572,208,552,208,543,223};
	polyStars[0] =  new CPolygon(10);
	polyStars[0]->Init(ArrayPts12);

	int ArrayPts13[] = {577,60,590,51,589,32,603,45,620,38,613,54,625,65,608,68,599,80,592,65};
	polyStars[1] =  new CPolygon(10);
	polyStars[1]->Init(ArrayPts13);

	int ArrayPts14[] = {1025,501,1020,493,1021,483,1030,477,1048,465,1064,477,1071,488,1072,514,1068,525,1058,532,1045,534,1029,523};
	polyGodess =  new CPolygon(12);
	polyGodess->Init(ArrayPts14);

	int ArrayPts15[] = {897,463,892,479,876,488,875,499,891,499,898,509,912,511,921,522,931,517,929,502,939,484,959,476,960,466,941,469,933,453,941,443,936,434,927,438,923,447,906,453,885,449,879,455};
	polyTurtle =  new CPolygon(22);
	polyTurtle->Init(ArrayPts15);

	int ArrayPts16[] = {292,495,281,476,262,491,266,476,255,478,262,456,274,452,282,459,284,408,279,399,265,407,256,396,261,393,264,398,270,393,264,383,259,359,272,370,274,383,289,394,295,384,269,357,249,354,226,338,222,321,241,339,243,326,227,311,212,316,205,299,217,306,214,282,234,306,242,299,233,275,252,295,258,339,277,348,254,268,275,309,276,323,288,335,294,362,306,372,307,357,291,324,278,304,294,315,309,298,303,324,317,350,309,318,317,308,320,286,325,297,331,283,331,299,321,322,328,344,335,342,338,333,338,310,355,328,346,343,357,345,370,328,365,351,378,342,381,353,370,362,332,356,321,373,306,394,311,416,320,415,320,400,334,393,340,418,331,443,318,429,302,434,306,462,320,481,299,477};
	polyTree =  new CPolygon(84);
	polyTree->Init(ArrayPts16);

	//polySwapLever
	int ArrayPts17[] = {360,135,376,123,393,133,397,153,381,163,362,155};
	polySwapLever[0] =  new CPolygon(6);
	polySwapLever[0]->Init(ArrayPts17);

	int ArrayPts18[] = {331,177,339,158,360,159,370,178,355,196,335,191};
	polySwapLever[1] =  new CPolygon(6);
	polySwapLever[1]->Init(ArrayPts18);

	int ArrayPts19[] = {332,227,336,205,358,200,370,219,356,238};
	polySwapLever[2] =  new CPolygon(5);
	polySwapLever[2]->Init(ArrayPts19);

	int ArrayPts20[] = {358,253,369,231,395,239,394,267,368,269};
	polySwapLever[3] =  new CPolygon(5);
	polySwapLever[3]->Init(ArrayPts20);

	int ArrayPts21[] = {629,458,642,446,659,451,660,472,641,480,629,470};
	polyMemoryPuzzle[0] =  new CPolygon(6);
	polyMemoryPuzzle[0]->Init(ArrayPts21);

	int ArrayPts22[] = {666,470,669,453,686,444,699,456,697,475,681,480};
	polyMemoryPuzzle[1] =  new CPolygon(6);
	polyMemoryPuzzle[1]->Init(ArrayPts22);

	int ArrayPts23[] = {704,467,710,448,729,447,739,463,729,478,713,478};
	polyMemoryPuzzle[2] =  new CPolygon(6);
	polyMemoryPuzzle[2]->Init(ArrayPts23);

	int ArrayPts24[] = {742,464,750,446,768,446,778,460,770,476,753,478};
	polyMemoryPuzzle[3] =  new CPolygon(6);
	polyMemoryPuzzle[3]->Init(ArrayPts24);

	int ArrayPts25[] = {673,526,646,523,646,498,793,496,791,526};
	polyKey =  new CPolygon(5);
	polyKey->Init(ArrayPts25);

	int ArrayPts26[] = {631,268,783,272,779,387,637,385};
	polyKeyHole =  new CPolygon(4);
	polyKeyHole->Init(ArrayPts26);

	int ArrayPts37[] = {627,255,677,255,679,307,628,305};
	polyLockTargetNew[0] =  new CPolygon(4);
	polyLockTargetNew[0]->Init(ArrayPts37);

	int ArrayPts38[] = {685,303,727,302,728,255,681,258};
	polyLockTargetNew[1] =  new CPolygon(4);
	polyLockTargetNew[1]->Init(ArrayPts38);

	int ArrayPts39[] = {737,304,787,302,782,253,734,256};
	polyLockTargetNew[2] =  new CPolygon(4);
	polyLockTargetNew[2]->Init(ArrayPts39);

	int ArrayPts40[] = {693,340,690,312,727,312,729,343};
	polyLockTargetNew[3] =  new CPolygon(4);
	polyLockTargetNew[3]->Init(ArrayPts40);

	int ArrayPts41[] = {628,399,623,346,676,345,678,399};
	polyLockTargetNew[4] =  new CPolygon(4);
	polyLockTargetNew[4]->Init(ArrayPts41);

	int ArrayPts42[] = {683,396,680,345,733,346,737,398};
	polyLockTargetNew[5] =  new CPolygon(4);
	polyLockTargetNew[5]->Init(ArrayPts42);

	int ArrayPts43[] = {742,394,739,343,784,344,783,394};
	polyLockTargetNew[6] =  new CPolygon(4);
	polyLockTargetNew[6]->Init(ArrayPts43);

	memoryPuzzlePointer = -1;

	//Init HO
	LocketSCN = NULL;

	CAnim_LocketPlaceMove = NULL;
	CAnim_HoListPanelTake = NULL;

	isLocketPlaceAnimPlaying = false;
	isHOListPanelTakePlaying = false;

	if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOListPanelTaken)
	{
		LocketSCN = new CHOScene();
		LocketSCN->Load("LV1\\S3\\HO\\LOCKET.SCN");
		LocketSCN->SetZOff(68);

		hoListPanel = LocketSCN->GetObjectByID("holist");
		hoLocketFixHolder = LocketSCN->GetObjectByID("locketfixholder");
		hoLocket = LocketSCN->GetObjectByID("locket");
		hoLocketShadow = LocketSCN->GetObjectByID("locketshadow");
		hoLocketGlow = LocketSCN->GetObjectByID("locketglow");
		hoLocketMask = LocketSCN->GetObjectByID("locketmovemask");
		hoListPanelTake = LocketSCN->GetObjectByID("holisttake");
		hoListPanelTakeGlow = LocketSCN->GetObjectByID("holistglow");
		
		setHOTextVisible(false);
		txtMatch3->Show(false);
		hintBG->Show(false);
	
		match3Back->Show(false);
		match3BackText->Show(false);

		CObject* objArr[] = {hoListPanelTakeGlow, hoListPanel, hoLocketShadow, hoListPanelTake};
		std::string animArr[] = {"ANIM\\L1\\s3holistglow.anim", "ANIM\\L1\\s3holistlist.anim", "ANIM\\L1\\s3holistshadow.anim", "ANIM\\L1\\s3holisttake.anim"};
		CAnim_HoListPanelTake = new CustomAnimGroup(4, animArr, objArr);

		hoListPanelTakeGlow->SetEnable(false);
		hoListPanelTake->SetEnable(false);

		if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced)
		{
			CObject* objArr[] = {hoLocketGlow, hoLocketFixHolder, hoLocket, hoLocketShadow};
			std::string animArr[] = {"ANIM\\L1\\s3locketfixglow.anim", "ANIM\\L1\\s3locketfixholder.anim", "ANIM\\L1\\s3locketfixlocket.anim", "ANIM\\L1\\s3locketfixshadow.anim"};
			CAnim_LocketPlaceMove = new CustomAnimGroup(4, animArr, objArr);

			hoLocket->SetEnable(false);
			hoLocketShadow->SetEnable(false);
			hoLocketGlow->SetEnable(false);

			setHOTextVisible(false);
			txtMatch3->Show(false);
			hintBG->Show(false);
		}
		else
		{
			hoLocket->SetEnable(false);		
			hoLocketGlow->SetEnable(false);
			hoLocketMask->SetEnable(false);
			hoLocketFixHolder->SetEnable(false);
		}
	}
	else
	{
		//Activate HO
		if(!ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened)
		{
			Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
		}
		else
		{
			Hud->InvBoxCtrl->SetLockBase(PUZBOX_FRAME);
		}
		Hud->IsHOScene = true;
	}

	GFInstance->PostUpdate(2);	

	clickVFX = new SuperFX();
	clickVFX->LoadEffectIni("PRT\\L1\\DiffPuzFX.INI");
	clickVFX->SetZ(100);

	//Video Init : Add Visible = true; to see anything on screen !!
	SetZ(1);

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if( !Control::Audio->IsPlaying(aTrackL1Mansion) )
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;	

	Control::Audio->LoadSample(aAmbS4PuzShadowhand,AudioAmb);
	Control::Audio->PlaySample(aAmbS4PuzShadowhand,true);
	//MUSIC_SFX
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S3_DoorHO]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S3_DoorHO])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S3_DoorHO] = true;

	

	
	

	Visible = true;	//To render anything with renderlistentity
}

void L1_S3_DoorHO::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL1Mansion) )
	{
		Control::Audio->Samples[aTrackL1Mansion]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if(Control::Audio->GlobalMusicVol - musicFadeOutTimer<=0)
		{
			Control::Audio->Samples[aTrackL1Mansion]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL1Mansion);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
				Control::Audio->PlaySample(aTrackL1Puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 8;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
			Control::Audio->PlaySample(aTrackL1Puzzle, true);
	}

	if( Hud->PopupState != Popup_Inactive )
		return;
}


void L1_S3_DoorHO::checkAllHiddenObjectsFind()
{
	if (ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened) return;

	int count = 0;

	for (int i = 0; i < 3; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[i])
		{
			count++;
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[0] )
	{
		count++;
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[1] )
	{
		count++;
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOCrescentFound)
	{
		count++;
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HORoseFlowerFound)
	{
		count++;
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOSnakeFound)
	{
		count++;
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOMaskFound)
	{
		count++;
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOOwlFound)
	{
		count++;
	}
		
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLionHeadFound)
	{
		count++;
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOTreeFound)
	{
		count++;
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOGoddessFound)
	{
		count++;
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOTurtleFound)
	{
		count++;
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOFishFound)
	{
		count++;
	}

	if(count == 15)
	{
		Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOSlideTwoSidesOpen);

		ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened = true;
		topLDoorXPosStart = topLDoor->Pos.x;
		topRDoorXPosStart = topRDoor->Pos.x;
		btmLDoorXPosStart = btmLDoor->Pos.x;
		btmRDoorXPosStart  = btmRDoor->Pos.x;

		topLDoorXPosEnd = topLDoor->Pos.x - 300;
		topRDoorXPosEnd = topRDoor->Pos.x + 300;
		btmLDoorXPosEnd = btmLDoor->Pos.x - 300;
		btmRDoorXPosEnd  = btmRDoor->Pos.x + 300;
		shutterTimer = 0.0f;
		isShutterOpening = true;

		if(_handleExtrasGameComplete(false))
			return;

		hintBG->Show(false);
		Hud->InvBoxCtrl->SetLockBase(PUZBOX_FRAME);
		Hud->IsHOScene = true;
		setHOTextVisible();
		txtPuzzleInfo->Show(true);
		txtPuzzleInfo->SetText(StringTable::getStr("L1puzzle"));
	}
	updateImageState();
}

void L1_S3_DoorHO::updateImageState()
{
	imageState[0] = ProfileSystem::Instance->ActiveProfile.L1S3_HORoseFlowerFound;
	imageState[1] = ProfileSystem::Instance->ActiveProfile.L1S3_HOOwlFound;
	imageState[2] = ProfileSystem::Instance->ActiveProfile.L1S3_HOCrescentFound;
	int count = 0;
	for (int i = 0; i < 2; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[i])
		{
			count++;
		}
	}
	if(count == 2)
	{
		imageState[3] = true;
	}
	else
	{
		imageState[3] = false;
	}
	imageState[4] = ProfileSystem::Instance->ActiveProfile.L1S3_HOFishFound;
	imageState[5] = ProfileSystem::Instance->ActiveProfile.L1S3_HOTreeFound;
	count = 0;
	for (int i = 0; i < 3; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[i])
		{
			count++;
		}
	}
	if(count == 3)
	{
		imageState[6] = true;
	}
	else
	{
		imageState[6] = false;
	}
	
	imageState[7] = ProfileSystem::Instance->ActiveProfile.L1S3_HOGoddessFound;
	imageState[8] = ProfileSystem::Instance->ActiveProfile.L1S3_HOMaskFound;
	imageState[9] = ProfileSystem::Instance->ActiveProfile.L1S3_HOSnakeFound;
	imageState[10] = ProfileSystem::Instance->ActiveProfile.L1S3_HOLionHeadFound;
	imageState[11] = ProfileSystem::Instance->ActiveProfile.L1S3_HOTurtleFound;
}


void L1_S3_DoorHO::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		ResetHO();
		return;
	}
	else
		GFHud->AllowInput = true;
	CPoint pos = Control::Input->Mpos();

	if(!txtMatch3->Visible && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOListPanelTaken && !ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		txtMatch3->Show(false);
		txtPuzzle->Show(false);
		txtMatch3MouseOver->Show(false);
		txtPuzzleMouseOver->Show(false);
	}


	if(isHOSolved)
	{
		hoTimer += Control::LogicRate;

		if(hoTimer >= 2.5f)
		{
			AwardManager::GetInstance()->CompleteHO(eArea_L1_S3_DoorHO);
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
		return;
	}

	if(isLocketPlaceAnimPlaying)
	{
		Cursor::SetMode(CA_Normal);
		if(CAnim_LocketPlaceMove->IsAnimFinished())
		{
			isLocketPlaceAnimPlaying = false;
			Hud->AllowInput = true;
			CAnim_LocketPlaceMove->SetEnableObjects(false);

			hoLocketMask->SetEnable(false);
			hoLocketShadow->SetEnable(true);//except this
		}
		return;
	}

	if(isHOListPanelTakePlaying)
	{
		Cursor::SetMode(CA_Normal);
		if(CAnim_HoListPanelTake->IsAnimFinished())
		{
			
			isHOListPanelTakePlaying = false;
			Hud->AllowInput = true;
			CAnim_HoListPanelTake->SetEnableObjects(false);

			ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOListPanelTaken = true;			

			//Activate HO
			setHOTextVisible(true);
			txtMatch3->Show(true);
			hintBG->Show(true);

			Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
			Hud->IsHOScene = true;			
		}
		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOListPanelTaken)
	{
		Cursor::SetMode(CA_Normal);

		if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced)
		{
			if(InvDestRectHOList.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L1strange");
					hoLocketFixHolder->Shake(0,2,0,0.25f);
				}
			}
			else if(HOBoundsRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Help);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L1must");
				}
			}
		}
		else //locket is picked!!
		{
			if(ActionRectHOListTake.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOGemDoorSlideOpen2);
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOObjectSilClicked);
					CAnim_HoListPanelTake->PlayAnim();
					isHOListPanelTakePlaying = true;
					Hud->AllowInput = false;
				}
			}
			//holistpanel not taken - force them to pick it!!
			else if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L1check");				
			}
		}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
				SendMessage(ControlExit, ID, HOSM_NewArea);
				return;		
		}
        #endif

if( IsMouseOverGoBackArea(&pos ))
		{
#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S3_Foyer, 2);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
				SendMessage(ControlExit, ID, HOSM_NewArea);
				return;
#endif

			}			
		}
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
				match3->clearBoard();
				HOScene->GetObjectByID("txtMatch3")->Show(false);
				HOScene->GetObjectByID("txtPuzzle")->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				isMatch3Playing = false;
				return;		
		}
    #endif
		if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
		{
			if( Control::Input->LBclicked() )
			{
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
		if( IsMouseOverGoBackArea(&pos )
			#ifdef M3_BUTTON
			&& txtMatch3->Visible
            #endif
            )
		{
#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S3_Foyer, 2);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
				SendMessage(ControlExit, ID, HOSM_NewArea);
				match3->clearBoard();
				HOScene->GetObjectByID("txtMatch3")->Show(false);
				HOScene->GetObjectByID("txtPuzzle")->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				isMatch3Playing = false;
				return;
#endif

			}			
		}

		if(areaRectMatch3.Intersection(&pos) && !match3->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			txtPuzzleMouseOver->Show(true);
			if( Control::Input->LBclicked() )
			{
				match3->clearBoard();
				txtMatch3->Show(true);
				txtPuzzle->Show(false);
				txtPuzzleMouseOver->Show(false);
				isMatch3Playing = false;				

				if(!ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened)
				{
					hintBG->Show(true);
					setHOTextVisible(true);
					Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				}
				else
				{
					Hud->InvBoxCtrl->SetLockBase(PUZBOX_FRAME);
					txtPuzzleInfo->Show(true);
				}
				
				return;
			}
		}
		else
		{
			txtPuzzleMouseOver->Show(false);
		}

		return;
	}

	Cursor::SetMode(CA_Normal);

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOCompletedWithMatch3 && !ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved)
	{
		ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved = true;
		Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOWin);

		if(_handleExtrasGameComplete(false))
			return;


		//Open the Door Animation
		ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened = true;

		txtPuzzleInfo->Show(false);
		DisableText();
		txtMatch3->Show(false);
		isHOSolved = true;

		//Add Door Opening Frame
		CObject *doorOpened = new CObject();
		doorOpened->LoadInit("LV1\\S3\\HO\\dooropened.SPR", 183, 0, 80);
		HOScene->PushObject(doorOpened);

		doorOpened->Show(false);
		doorOpened->FadeIn(1.5f);

		/*Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
		SendMessage(ControlExit, ID, HOSM_NewArea);*/	
		return;
	}

	Cursor::SetMode(CA_Normal);

	if(!ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened)
	{
		for (int i = 0; i < 12; i++)
		{
			if(hintImageRects[i].Intersection(&pos) && imageState[i] == false)
			{
				hintImages[i]->Show(true);
			}
			else
			{
				hintImages[i]->Show(false);
			}
		}
	}

	if(isShutterOpening)
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;

		shutterTimer += Control::LogicRate;
		float endPos;
		endPos = topLDoorXPosStart + (topLDoorXPosEnd - topLDoorXPosStart) * easing(shutterTimer, 0, 1, 1.5f);
		topLDoor->Pos.x = endPos;
		endPos = topRDoorXPosStart + (topRDoorXPosEnd - topRDoorXPosStart) * easing(shutterTimer, 0, 1, 1.5f);
		topRDoor->Pos.x = endPos;

		endPos = btmLDoorXPosStart + (btmLDoorXPosEnd - btmLDoorXPosStart) * easing(shutterTimer, 0, 1, 1.5f);
		btmLDoor->Pos.x = endPos;

		endPos = btmRDoorXPosStart + (btmRDoorXPosEnd - btmRDoorXPosStart) * easing(shutterTimer, 0, 1, 1.5f);
		btmRDoor->Pos.x = endPos;

		if(shutterTimer > 1.5f)
		{
			isShutterOpening = false;
		}
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
			movingObjects[objectID]->Show(true);
			returnObject->Show(false);
			returnObject = NULL;
			isObjectReturning = false;
			returnTimer = 0.0f;
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
					if(polyFish->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed)
					{
						isObjectInHand = false;
						fish->Show(true);
						fishhead->Show(false);
						pointerInHand->Show(false);
						SAFE_DELETE(pointerInHand);
						ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed = true;
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemFixed);
						return;
					}
					break;
				case 1:
					if(polyKeyHole->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S3_KeyFixed)
					{
						isObjectInHand = false;
						keyfixed->Show(true);
						pointerInHand->Show(false);
						SAFE_DELETE(pointerInHand);
						ProfileSystem::Instance->ActiveProfile.L1S3_KeyFixed = true;
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOKeyFixed);
						Hud->ShowBannerText("L1hint");
						return;
					}
					break;
			}

			isObjectInHand = false;
			isObjectReturning = true;
			returnObject = pointerInHand;
			returnTimer = 0.0f;
			startPos = pointerInHand->Pos;
			endPos  = movingObjects[objectID]->Pos;

			float distance = (float)sqrt( ((startPos.x - endPos.x)*(startPos.x - endPos.x)) + ((startPos.y - endPos.y) * (startPos.y - endPos.y)));
			returnEndTimer = distance / 1000;
			
		}
		return;
	}
	else
	{
		if(polyFish->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed)
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L1tail");
			}
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S3_KeyFixed)
		{
			if(lockAreaRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L1place");
				}
			}
		}
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened && !ProfileSystem::Instance->ActiveProfile.L1S3_KeyFixed)
	{
		if(polyKey->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				objectID = 1;
				movingObjects[objectID]->Show(false);
				pointerInHand = new CObject();
				pointerInHand->LoadSprite("LV1\\S3\\HO\\keytaken.SPR");
				pointerInHand->Show(true);

				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOGemClicked);
				pointerInHand->Pos = movingObjects[objectID]->Pos;
				pointerInHand->SetZ(200);
				isObjectInHand = true;
				return;
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOMemoryPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed)
	{
		if(polyFishTail->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				K_LOG ("Jih...");
				objectID = 0;
				movingObjects[objectID]->Show(false);
				K_LOG ("Jih...2");
				pointerInHand = new CObject();
				pointerInHand->LoadSprite("LV1\\S3\\HO\\fishtale.SPR");
				pointerInHand->Show(true);

				pointerInHand->Pos = movingObjects[objectID]->Pos;
				pointerInHand->SetZ(200);
				isObjectInHand = true;
				K_LOG ("..huaaah");
				return;
			}
		}
	}

	if(isStarDoorOpening)
	{
		if(stardoorAnim->IsAnimationFinished())
		{
			isStarDoorOpening = false;
			stardoorAnim->Show(false);
			stardoorOpened->Show(true);
		}
		return;
	}

	if(isLeverSwaping)
	{
		swapTime  += Control::LogicRate;
		float easeTime = easing(swapTime, 0, 1, 0.5f);

		float angle = swapAngleStart[0] + (swapAngleStart[1] - swapAngleStart[0]) * easeTime;
		swapHand2[0]->SetRotate(angle);

		float angle2 = swapAngleStart[1] + (swapAngleStart[0] - swapAngleStart[1]) * easeTime;
		swapHand2[1]->SetRotate(angle2);

		if(swapTime >= 0.5f)
		{
			for (int i = 0; i < 4; i++)
			{
				swapLeverSel[i]->Show(false);
			}
			swapHand2[0]->SetRotate(swapAngleStart[1]);
			swapHand2[1]->SetRotate(swapAngleStart[0]);
			swapLeverIndex[0] = -1;
			swapLeverIndex[1] = -1;
			isLeverSwaping = false;
			swapHand2[0] = NULL;
			swapHand2[1] = NULL;

			bool isPuzzleSolved = true;
			for (int i = 0; i < 4; i++)
			{
				if(swapHand[i]->GetRotate() != swapCurrectAngles[i])
				{
					isPuzzleSolved = false;
					if(!isSymbolText)
					{
						Hud->ShowBannerText("L1looks");
						isSymbolText = true;
					}
					break;
				}
			}

			if(isPuzzleSolved)
			{
				ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapPuzzleSolved = true;
				stardoor->Show(false);

				stardoorAnim->Show(true);
				stardoorAnim->ForcePlayAnimation(0);
				isStarDoorOpening = true;

				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOCoverOpen);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOSlideTwoSidesOpen);
			}
		}

		return;
	}
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapPuzzleSolved)
	{
		swapLeverMouseOver->Show(false);
		for (int i = 0; i < 4; i++)
		{
			if(polySwapLever[i]->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				swapLeverMouseOver->Show(true);
				swapLeverMouseOver->Pos = swapLeverSel[i]->Pos;

				if(Control::Input->LBclicked())
				{
					if(swapLeverIndex[0] == -1)
					{
						swapLeverIndex[0] = i;
						swapLeverSel[i]->Show(true);
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOBlueBtnClick);
						return;
					}
					else
					{
						swapLeverSel[i]->Show(true);
						swapLeverIndex[1] = i;

						switch (swapLeverIndex[0])
						{
							case 0:
								swapAngleStart[0] = 30.0f;
								break;
							case 1:
								swapAngleStart[0] = 70.0f;
								break;
							case 2:
								swapAngleStart[0] = 110.0f;
								break;
							case 3:
								swapAngleStart[0] = 150.0f;
								break;
						}

						switch (swapLeverIndex[1])
						{
							case 0:
								swapAngleStart[1] = 30.0f;
								break;
							case 1:
								swapAngleStart[1] = 70.0f;
								break;
							case 2:
								swapAngleStart[1] = 110.0f;
								break;
							case 3:
								swapAngleStart[1] = 150.0f;
								break;
						}

						for (int i = 0; i < 4; i++)
						{
							if(swapHand[i]->GetRotate() == swapAngleStart[0])
							{
								swapHand2[0] = swapHand[i];
							}

							if(swapHand[i]->GetRotate() == swapAngleStart[1])
							{
								swapHand2[1] = swapHand[i];
							}
						}

						swapTime = 0.0f;

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOBlueBtnSwap);
					
						isLeverSwaping = true;
						swapLeverMouseOver->Show(false);
						return;
					}
					return;
				}
			}
		}

		if(Control::Input->LBclicked())
		{
			if(swapLeverIndex[0] != -1)
			{
				for (int i = 0; i < 4; i++)
				{
					swapLeverSel[i]->Show(false);
				}
				swapLeverIndex[0] = -1;
				swapLeverIndex[1] = -1;
				swapHand2[0] = NULL;
				swapHand2[1] = NULL;
			}
		}
	}

	if(isMemoryPuzzleReversing)
	{
		swapTime  += Control::LogicRate;

		if(swapTime >= 0.3f)
		{
			isMemoryPuzzleReversing = false;
			for (int i = 0; i < 4; i++)
			{
				gemGlow[i]->Show(false);
			}
		}

		return;
	}

	if(isMemoryPuzzleSolved)
	{
		swapTime  += Control::LogicRate;

		float yPos = startYPos + (endYPos - startYPos) * swapTime;
		doorgembg->Pos.y = yPos;

		//K_LOG ("Inside...isMemoryPuzzle solved...");

		if(swapTime >= 0.3f)
		{
			Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOGemDoorSlideOpen);

			isMemoryPuzzleSolved = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOMemoryPuzzleSolved = true;
			swapTime = 0.0f;
			doorgembg->FadeOut(2.0f);
		}

		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOMemoryPuzzleSolved)
	{
		for (int i = 0; i < 4; i++)
		{
			if(polyMemoryPuzzle[i]->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if(Control::Input->LBclicked())
				{
					gemGlow[i]->Show(true);

					memoryPuzzlePointer++;
					if(memoryPuzzleCorrectOrder[memoryPuzzlePointer] != i)
					{
						memoryPuzzlePointer = -1;
						isMemoryPuzzleReversing = true;
						swapTime = 0.0f;
						Hud->ShowBannerText("L1maybe");
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOGemWrongClick);
					}
					else
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOGemClicked);
					}
				}

				if(memoryPuzzlePointer == 3)
				{					
					isMemoryPuzzleSolved = true;
					startYPos = doorgembg->Pos.y;
					endYPos = startYPos + 200;
					swapTime = 0.0f;

					for (int j = 0; j < 4; j++)
					{
						gemGlow[j]->Show(false);
					}
				}

				return;
			}
		}
	}

	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[i])
		{
			if(polyLeaves[i]->Intersection(&pos))
			{
				if(Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

					clickVFX->SetPos(PointSystem::CreateCPoint(leaves[i]->Pos.x,leaves[i]->Pos.y,0));
					clickVFX->Start();
					leaves[i]->FadeOut(4.0f);
					ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[i] = true;
					UpdateLeafStarText();

					for (int j = 0; j < 3; j++)
					{
						if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[j])
						{
							count++;
						}
					}

					if(count == 3)
					{
						txtLeaves->SetStrikethrough(true);
						checkAllHiddenObjectsFind();
					}
				}
				break;
			}
		}
	}

	//START: Lock Puzzle //sree

	//sree
	if(isLockMoving)
	{
		moveTimer += Control::LogicRate * 4;
		CPoint temp;
		CPoint bt;
		float targetTime = 1.0f;

		if(fromPos == 3)
		{
			if(toPos == 0)
			{
				temp.x = (keyEndPos.x + keyStartPos.x)/2 - 30;
				temp.y = (keyEndPos.y + keyStartPos.y)/2 + 15;
				bt = Elixir::GetBezierPoint(&keyStartPos, &temp, &keyEndPos, moveTimer);
				keyfixed->Pos = bt;
			}
			else if(toPos == 2)
			{
				temp.x = (keyEndPos.x + keyStartPos.x)/2 + 30;
				temp.y = (keyEndPos.y + keyStartPos.y)/2 + 15;
				bt = Elixir::GetBezierPoint(&keyStartPos, &temp, &keyEndPos, moveTimer);
				keyfixed->Pos = bt;
			}
			else if(toPos == 4)
			{
				temp.x = (keyEndPos.x + keyStartPos.x)/2 - 30;
				temp.y = (keyEndPos.y + keyStartPos.y)/2 - 15;
				bt = Elixir::GetBezierPoint(&keyStartPos, &temp, &keyEndPos, moveTimer);
				keyfixed->Pos = bt;
			}
			else if(toPos == 6)
			{
				temp.x = (keyEndPos.x + keyStartPos.x)/2 + 30;
				temp.y = (keyEndPos.y + keyStartPos.y)/2 - 15;
				bt = Elixir::GetBezierPoint(&keyStartPos, &temp, &keyEndPos, moveTimer);
				keyfixed->Pos = bt;
			}
			else
			{
				targetTime = 0.6f;
				moveTimer += Control::LogicRate;
				temp.x = keyStartPos.x + (keyEndPos.x - keyStartPos.x) * easing(moveTimer,0,1,targetTime);
				temp.y = keyStartPos.y + (keyEndPos.y - keyStartPos.y) * easing(moveTimer,0,1,targetTime);
				keyfixed->Pos = temp;
				targetTime = 0.6f;
			}
			isLockMovedOnce = true;
		}
		else
		{
			if(fromPos == 0)
			{
				temp.x = (keyEndPos.x + keyStartPos.x)/2 - 30;
				temp.y = (keyEndPos.y + keyStartPos.y)/2 + 15;
				bt = Elixir::GetBezierPoint(&keyStartPos, &temp, &keyEndPos, moveTimer);
				keyfixed->Pos = bt;
			}
			else if(fromPos == 2)
			{
				temp.x = (keyEndPos.x + keyStartPos.x)/2 + 30;
				temp.y = (keyEndPos.y + keyStartPos.y)/2 + 15;
				bt = Elixir::GetBezierPoint(&keyStartPos, &temp, &keyEndPos, moveTimer);
				keyfixed->Pos = bt;
			}
			else if(fromPos == 4)
			{
				temp.x = (keyEndPos.x + keyStartPos.x)/2 - 30;
				temp.y = (keyEndPos.y + keyStartPos.y)/2 - 15;
				bt = Elixir::GetBezierPoint(&keyStartPos, &temp, &keyEndPos, moveTimer);
				keyfixed->Pos = bt;
			}
			else if(fromPos == 6)
			{
				temp.x = (keyEndPos.x + keyStartPos.x)/2 + 30;
				temp.y = (keyEndPos.y + keyStartPos.y)/2 - 15;
				bt = Elixir::GetBezierPoint(&keyStartPos, &temp, &keyEndPos, moveTimer);
				keyfixed->Pos = bt;
			}
			else
			{
				moveTimer += Control::LogicRate;
				targetTime = 0.6f;
				temp.x = keyStartPos.x + (keyEndPos.x - keyStartPos.x) * easing(moveTimer,0,1,targetTime);
				temp.y = keyStartPos.y + (keyEndPos.y - keyStartPos.y) * easing(moveTimer,0,1,targetTime);
				keyfixed->Pos = temp;
			}
		}
		if(moveTimer >= targetTime)
		{
			keyfixed->Pos = keyEndPos;

			if(fromPos != 3)//raj3
			{
				keyStartPos = keyfixed->Pos;
				keyEndPos = targetSel[nextEndPosIndex]->Pos;
				fromPos = 3;
				toPos = nextEndPosIndex;
				currentLockPos = nextEndPosIndex;
				isLockMoving = true;
				moveTimer = 0.0f;
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOKeyDrag);
				return;
			}
			
			isLockMoving = false;
			LockSel->Show(true);
			LockSel->Pos = keyfixed->Pos;
			lockGlow[toPos]->Show(true);

			if(fromPos == 3)
			{
				if(selectedLockPointer == -1)
				{
					selectedLockPointer = 0;
					selectedLockPoints[0] = toPos;
				}
				else
				{
					selectedLockPoints[1] = toPos;
					int selIndex = -1;

					for (int i = 0; i < 3; i++)
					{
						if((keyValues[i].x == selectedLockPoints[0] || keyValues[i].x == selectedLockPoints[1]) && (keyValues[i].y == selectedLockPoints[0] || keyValues[i].y == selectedLockPoints[1]))
						{
							selIndex = i;							
							ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[selIndex] = true;
							selectedLockPointer = -1;
							break;
						}
					}

					if(selIndex != -1)
					{
						K_LOG ("selIndex != -1");
						if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[selIndex] == true)
						{
							K_LOG ("ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[selIndex] == true");
							for (int i = 0; i < 7; i++)
							{
								if(i != 3)
								{
									lockGlow[i]->Show(false);
								}
							}

							for (int i = 0; i < 3; i++)
							{
								if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[i] == true)
								{
									lockGlow[(int)keyValues[i].x]->Show(true);
									lockGlow[(int)keyValues[i].y]->Show(true);
								}
							}
						}
					}
					else
					{
						selectedLockPoints[0] = toPos;
						for (int i = 0; i < 7; i++)
						{
							if(i != 3)
							{
								lockGlow[i]->Show(false);
							}
						}
						
						lockGlow[toPos]->Show(true);

						for (int i = 0; i < 3; i++)
						{
							if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[i] == true)
							{
								lockGlow[(int)keyValues[i].x]->Show(true);
								lockGlow[(int)keyValues[i].y]->Show(true);
							}
						}
					}

					int count = 0;
					for (int i = 0; i < 3; i++)
					{
						if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[i] == true)
						{
							count++;
						}
					}

					if(count == 3)
					{
						for (int i = 0; i < 3; i++)
						{
							if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[i] == true)
							{
								lockGlow[(int)keyValues[i].x]->Show(true);
								lockGlow[(int)keyValues[i].y]->Show(true);
							}
						}
						ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved = true;
						
						//Open the Door Animation
						ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened = true;

						txtPuzzleInfo->Show(false);
						DisableText();
						txtMatch3->Show(false);
						isHOSolved = true;
						
						//Add Door Opening Frame
						CObject *doorOpened = new CObject();
						doorOpened->LoadInit("LV1\\S3\\HO\\dooropened.SPR", 183, 0, 80);
						HOScene->PushObject(doorOpened);

						doorOpened->Show(false);
						doorOpened->FadeIn(1.5f);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOWin);
						return;
					}
				}
			}
		}
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved == false && ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened == true)
	{
		if( Elixir::Distance(&pos,&keyfixed->Pos) < 20 && isLockSelected == false)
		{
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				if(currentLockPos == 3)
				{
					LockSel->Show(true);
					LockSel->Pos = keyfixed->Pos;
					isLockSelected = true;
					for (int i = 0; i < 7; i++)
					{
						if(currentLockPos != i)
						{
							targetSel[i]->Show(true);
						}
					}
					return;
				}
				return;
			}
		}

		if(isLockSelected)
		{
			if(currentLockPos == 3)
			{
				for (int i = 0; i < 7; i++)
				{
					if(i != 3)
					{
						button[i]->Show(false);
					}
				}

				for (int i = 0; i < 7; i++)
				{
					if(polyLockTargetNew[i]->Intersection(&pos) && i != currentLockPos)
					{
						button[i]->Show(true);
						Cursor::SetMode(CA_HandPointy);
						if(Control::Input->LBclicked())
						{
							button[i]->Show(false);
							keyStartPos = keyfixed->Pos;
							keyEndPos = targetSel[i]->Pos;
							fromPos = 3;
							toPos = i;
							currentLockPos = i;
							isLockMoving = true;
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOKeyDrag);
							moveTimer = 0.0f;
							
							for (int j = 0; j < 7; j++)
							{
								targetSel[j]->Show(false);
								LockSel->Show(false);
							}
						}
					}
					else
					{
						if(i != 3)
						{
							button[i]->Show(false);
						}
					}
				}
			}//raj2
			else
			{
				for (int i = 0; i < 7; i++)
				{
					if(i != 3 )
					{
						button[i]->Show(false);
					}
				}

				for (int i = 0; i < 7; i++)
				{
					if(polyLockTargetNew[i]->Intersection(&pos) && i != currentLockPos && i != 3)
					{
						button[i]->Show(true);
						Cursor::SetMode(CA_HandPointy);
						if(Control::Input->LBclicked())
						{
							button[i]->Show(false);
							nextEndPosIndex = i;

							keyStartPos = keyfixed->Pos;
							keyEndPos = targetSel[3]->Pos;
							isLockMoving = true;
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOKeyDrag);
							moveTimer = 0.0f;
							fromPos = currentLockPos;
							toPos = 3;
							currentLockPos = 3;
							LockSel->Show(false);
							//return;
						}
					}
					else
					{
						if(i != 3)
						{
							button[i]->Show(false);
						}
					}
				}
			}
		}
	}
	//END: Lock Puzzle
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HORoseFlowerFound)
	{
		if(polyRoseflower->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(rose->Pos.x,rose->Pos.y,0));
				clickVFX->Start();
				rose->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HORoseFlowerFound = true;
				txtRoseFlower->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOCrescentFound)
	{
		if(polyCrescent->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(crescent->Pos.x,crescent->Pos.y,0));
				clickVFX->Start();
				crescent->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOCrescentFound = true;
				txtCrescent->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOSnakeFound)
	{
		if(polySnake->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(snake->Pos.x,snake->Pos.y,0));
				clickVFX->Start();
				snake->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOSnakeFound = true;
				txtSnake->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOMaskFound)
	{
		if(polyMask->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(mask->Pos.x,mask->Pos.y,0));
				clickVFX->Start();
				mask->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOMaskFound = true;
				txtMask->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOOwlFound)
	{
		if(polyOwl->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(owl->Pos.x,owl->Pos.y,0));
				clickVFX->Start();
				owl->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOOwlFound = true;
				txtOwl->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}
		
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOLionHeadFound)
	{
		if(polyLionhead->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(lionhead->Pos.x,lionhead->Pos.y,0));
				clickVFX->Start();
				lionhead->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOLionHeadFound = true;
				txtLionHead->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}
	
	count = 0;
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[0] && ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapPuzzleSolved)
	{
		if(polyStars[0]->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(Stars[0]->Pos.x,Stars[0]->Pos.y,0));
				clickVFX->Start();
				Stars[0]->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[0] = true;
				UpdateLeafStarText();
				count = 0;
				if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[0] )
				{
					count++;
				}

				if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[1] )
				{
					count++;
				}
				if(count == 2)
				{
					txtStars->SetStrikethrough(true);
					checkAllHiddenObjectsFind();
				}
			}
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[1] )
	{
		if(polyStars[1]->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(Stars[1]->Pos.x,Stars[1]->Pos.y,0));
				clickVFX->Start();
				Stars[1]->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[1] = true;
				UpdateLeafStarText();
				count = 0;
				if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[0] )
				{
					count++;
				}

				if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[1] )
				{
					count++;
				}
				if(count == 2)
				{
					txtStars->SetStrikethrough(true);
					checkAllHiddenObjectsFind();
				}
			}
		}
	}

	

	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOTreeFound)
	{
		if(polyTree->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(tree->Pos.x,tree->Pos.y,0));
				clickVFX->Start();
				tree->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOTreeFound = true;
				txtTree->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOGoddessFound)
	{
		if(polyGodess->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(goddes->Pos.x,goddes->Pos.y,0));
				clickVFX->Start();
				goddes->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOGoddessFound = true;
				txtGoddess->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOTurtleFound)
	{
		if(polyTurtle->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(turtle->Pos.x,turtle->Pos.y,0));
				clickVFX->Start();
				turtle->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOTurtleFound = true;
				txtTurtle->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed && !ProfileSystem::Instance->ActiveProfile.L1S3_HOFishFound)
	{
		if(polyFish->Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemClicked);

				clickVFX->SetPos(PointSystem::CreateCPoint(fish->Pos.x,fish->Pos.y,0));
				clickVFX->Start();
				fish->FadeOut(4.0f);
				ProfileSystem::Instance->ActiveProfile.L1S3_HOFishFound = true;
				txtFish->SetStrikethrough(true);
				checkAllHiddenObjectsFind();
			}
		}
	}

	if(areaRectMatch3.Intersection(&pos)  && !ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved)//sree
	{
		Cursor::SetMode(CA_HandPointy);
		txtMatch3MouseOver->Show(true);
		if( Control::Input->LBclicked() )
		{
			Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);

			if(!ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened)
				hintBG->Show(false);

			setHOTextVisible(false);
			if(match3 == NULL)
			{
				match3 = new Match3(2);
				AddControl(match3);
			}
			else
			{
				match3->ShowBoard();
			}
			txtPuzzleInfo->Show(false);
			txtMatch3->Show(false);
			txtMatch3MouseOver->Show(false);
			txtPuzzle->Show(true);
			isMatch3Playing = true;
			Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;
			return;
		}
	}
	else
	{
		txtMatch3MouseOver->Show(false);
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif

	if( IsMouseOverGoBackArea(&pos) )
	{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L1_S3_Foyer, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
		}	
	}

	ResetHO();
}


float L1_S3_DoorHO::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L1_S3_DoorHO::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

	static int FrameCnt = 0;
	FrameCnt = (FrameCnt+1)%2;
}

L1_S3_DoorHO::~L1_S3_DoorHO()
{

	Control::Audio->Samples[aTrackL1Mansion]->SetVolume((float)Control::Audio->GlobalMusicVol);
	Hud->InvBoxCtrl->ResetToInvBox();
	Hud->IsHOScene = false;

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(match3 != NULL)
		{
			match3->clearBoard();
			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			isMatch3Playing = false;
		}
	}

	SAFE_DELETE(clickVFX);

	SAFE_DELETE(LocketSCN);

	SAFE_DELETE(CAnim_LocketPlaceMove);
	SAFE_DELETE(CAnim_HoListPanelTake);

	SAFE_DELETE(pointerInHand);

	SAFE_DELETE(txtRoseFlower);
	SAFE_DELETE(txtCrescent);
	SAFE_DELETE(txtSnake);
	SAFE_DELETE(txtMask);
	SAFE_DELETE(txtOwl);
	SAFE_DELETE(txtLeaves);
	SAFE_DELETE(txtLionHead);
	SAFE_DELETE(txtFish);
	SAFE_DELETE(txtStars);
	SAFE_DELETE(txtTree);
	SAFE_DELETE(txtGoddess);
	SAFE_DELETE(txtTurtle);
	SAFE_DELETE(txtPuzzleInfo);

	SAFE_DELETE(polyRoseflower);
	SAFE_DELETE(polyCrescent);
	SAFE_DELETE(polySnake);
	SAFE_DELETE(polyMask);
	SAFE_DELETE(polyOwl);
	SAFE_DELETE(polyLionhead);
	SAFE_DELETE(polyFishTail);
	SAFE_DELETE(polyFish);
	SAFE_DELETE(polyTree);
	SAFE_DELETE(polyGodess);
	SAFE_DELETE(polyTurtle);

	SAFE_DELETE(polyKeyHole);
	SAFE_DELETE(polyKey);

	for (int i = 0; i < 2; i++)
	{
		SAFE_DELETE(polyStars[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		SAFE_DELETE(polyLeaves[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(polySwapLever[i]);
		SAFE_DELETE(polyMemoryPuzzle[i]);
	}

	for (int i = 0; i < 7; i++)
	{
		SAFE_DELETE(polyLockTargetNew[i]);
	}

	for (int i = 0; i < 12; i++)
	{
		SAFE_DELETE(hintImages[i]);
	}
	SAFE_DELETE(hintBG);

	SAFE_DELETE(ResetDia);
	
	//MUSIC_SFX
	Control::Audio->StopSample(aAmbS4PuzShadowhand);
	Control::Audio->UnloadSample(aAmbS4PuzShadowhand);
	//MUSIC_SFX
}

void L1_S3_DoorHO::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
		case eInv_L2S1_ColorPowder:
		break;
	
	default:
		break;
	}
}

void L1_S3_DoorHO::InitializeText()
{
	txtRoseFlower = NULL;
	txtCrescent = NULL;
	txtSnake = NULL;
	txtMask = NULL;
	txtOwl = NULL;
	txtLeaves = NULL;
	txtLionHead = NULL;
	txtFish = NULL;
	txtStars = NULL;
	txtTree = NULL;
	txtGoddess = NULL;
	txtTurtle = NULL;

	CPoint pos;
	txtRoseFlower = new CBitmapText();
	txtRoseFlower->LoadFont(eFont_Small);
	txtRoseFlower->SetAlignment(Align_Left);
    pos = CPoint(390,687,0);
	txtRoseFlower->SetPos(&pos);
	txtRoseFlower->SetScale(1.0f);
	txtRoseFlower->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtRoseFlower->SetZ(3000);
	txtRoseFlower->Show(true);
	txtRoseFlower->SetText(StringTable::getStr("Rose"));
	//txtStar->SetStrikethrough(true);

	txtOwl = new CBitmapText();
	txtOwl->LoadFont(eFont_Small);
	txtOwl->SetAlignment(Align_Left);
    pos = CPoint(490,687,0);
	txtOwl->SetPos(&pos);
	txtOwl->SetScale(1.0f);
	txtOwl->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtOwl->SetZ(3000);
	txtOwl->Show(true);
	txtOwl->SetText(StringTable::getStr("Owl"));
	
	txtCrescent = new CBitmapText();
	txtCrescent->LoadFont(eFont_Small);
	txtCrescent->SetAlignment(Align_Left);
    pos = CPoint(585,687,0);
	txtCrescent->SetPos(&pos);
	txtCrescent->SetScale(1.0f);
	txtCrescent->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtCrescent->SetZ(3000);
	txtCrescent->Show(true);
	txtCrescent->SetText(StringTable::getStr("Crescent"));

	txtStars = new CBitmapText();
	txtStars->LoadFont(eFont_Small);
	txtStars->SetAlignment(Align_Left);
    pos = CPoint(750,687,0);
	txtStars->SetPos(&pos);
	txtStars->SetScale(1.0f);
	txtStars->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtStars->SetZ(3000);
	txtStars->Show(true);
	txtStars->SetText(StringTable::getStr("Stars"));

	txtFish = new CBitmapText();
	txtFish->LoadFont(eFont_Small);
	txtFish->SetAlignment(Align_Left);
    pos = CPoint(390,720,0);
	txtFish->SetPos(&pos);
	txtFish->SetScale(1.0f);
	txtFish->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtFish->SetZ(3000);
	txtFish->Show(true);
	txtFish->SetText(StringTable::getStr("Fish"));

	txtTree = new CBitmapText();
	txtTree->LoadFont(eFont_Small);
	txtTree->SetAlignment(Align_Left);
    pos = CPoint(490,720,0);
	txtTree->SetPos(&pos);
	txtTree->SetScale(1.0f);
	txtTree->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtTree->SetZ(3000);
	txtTree->Show(true);
	txtTree->SetText(StringTable::getStr("Tree"));

	txtLeaves = new CBitmapText();
	txtLeaves->LoadFont(eFont_Small);
	txtLeaves->SetAlignment(Align_Left);
    pos = CPoint(585,720,0);
	txtLeaves->SetPos(&pos);
	txtLeaves->SetScale(1.0f);
	txtLeaves->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtLeaves->SetZ(3000);
	txtLeaves->Show(true);
	txtLeaves->SetText(StringTable::getStr("Leaves"));

	txtGoddess = new CBitmapText();
	txtGoddess->LoadFont(eFont_Small);
	txtGoddess->SetAlignment(Align_Left);
    pos = CPoint(750,720,0);
	txtGoddess->SetPos(&pos);
	txtGoddess->SetScale(1.0f);
	txtGoddess->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtGoddess->SetZ(3000);
	txtGoddess->Show(true);
	txtGoddess->SetText(StringTable::getStr("Goddess"));

	txtMask = new CBitmapText();
	txtMask->LoadFont(eFont_Small);
	txtMask->SetAlignment(Align_Left);
    pos = CPoint(390,754,0);
	txtMask->SetPos(&pos);
	txtMask->SetScale(1.0f);
	txtMask->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtMask->SetZ(3000);
	txtMask->Show(true);
	txtMask->SetText(StringTable::getStr("Mask"));

	txtSnake = new CBitmapText();
	txtSnake->LoadFont(eFont_Small);
	txtSnake->SetAlignment(Align_Left);
    pos = CPoint(490,754,0);
	txtSnake->SetPos(&pos);
	txtSnake->SetScale(1.0f);
	txtSnake->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtSnake->SetZ(3000);
	txtSnake->Show(true);
	txtSnake->SetText(StringTable::getStr("Snake"));

	txtLionHead = new CBitmapText();
	txtLionHead->LoadFont(eFont_Small);
	txtLionHead->SetAlignment(Align_Left);
    pos = CPoint(585,754,0);
	txtLionHead->SetPos(&pos);
	txtLionHead->SetScale(1.0f);
	txtLionHead->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtLionHead->SetZ(3000);
	txtLionHead->Show(true);
	txtLionHead->SetText(StringTable::getStr("LionHead"));
	
	txtTurtle = new CBitmapText();
	txtTurtle->LoadFont(eFont_Small);
	txtTurtle->SetAlignment(Align_Left);
    pos = CPoint(750,754,0);
	txtTurtle->SetPos(&pos);
	txtTurtle->SetScale(1.0f);
	txtTurtle->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtTurtle->SetZ(3000);
	txtTurtle->Show(true);
	txtTurtle->SetText(StringTable::getStr("Turtle"));

	txtPuzzleInfo = new CBitmapText();
	txtPuzzleInfo->LoadFont(eFont_Small);
	txtPuzzleInfo->SetAlignment(Align_Left);
    pos = CPoint(560.0f,715.0f,0);
	txtPuzzleInfo->SetPos(&pos);
	txtPuzzleInfo->SetScale(1.0f);
	txtPuzzleInfo->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtPuzzleInfo->SetZ(3000);
	txtPuzzleInfo->SetText(StringTable::getStr("L1solve"));
	txtPuzzleInfo->Show(false);
}

void L1_S3_DoorHO::setHOTextVisible(bool visible)
{
	txtRoseFlower->Show(visible);
	txtCrescent->Show(visible);
	txtSnake->Show(visible);
	txtMask->Show(visible);
	txtOwl->Show(visible);
	txtLeaves->Show(visible);
	txtLionHead->Show(visible);
	txtFish->Show(visible);
	txtStars->Show(visible);
	txtTree->Show(visible);
	txtGoddess->Show(visible);
	txtTurtle->Show(visible);
}

void L1_S3_DoorHO::UpdateLeafStarText()
{
	int count = 0;

	for (int i = 0; i < 3; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[i])
		{
			count++;
		}
	}
	
	txtLeaves->SetText(std::string(StringTable::getStr("Exleaves")) + MKSTR(count) + std::string("/3)"));

	count = 0;
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[0] )
	{
		count++;
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[1] )
	{
		count++;
	}
	txtStars->SetText(std::string(StringTable::getStr("Exstars")) + MKSTR(count) + std::string("/2)"));
}

void L1_S3_DoorHO::DisableText()
{
	txtRoseFlower->Show(false);
	txtCrescent->Show(false);
	txtSnake->Show(false);
	txtMask->Show(false);
	txtOwl->Show(false);
	txtLeaves->Show(false);
	txtLionHead->Show(false);
	txtFish->Show(false);
	txtStars->Show(false);
	txtTree->Show(false);
	txtGoddess->Show(false);
	txtTurtle->Show(false);

	if(hintBG != NULL)
	{
		hintBG->Show(false);
	}
}

void L1_S3_DoorHO::UpdateText()
{
	UpdateLeafStarText();

	int count = 0;

	for (int i = 0; i < 3; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[i])
		{
			count++;
		}
	}
	if(count == 3)
	{
		txtLeaves->SetAlpha(0.5f);
	}

	count = 0;
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[0] )
	{
		count++;
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[1] )
	{
		count++;
	}
	
	if(count == 2)
	{
		txtStars->SetAlpha(0.5f);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOCrescentFound)
	{
		txtCrescent->SetAlpha(0.5f);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S3_HORoseFlowerFound)
	{
		txtRoseFlower->SetAlpha(0.5f);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOSnakeFound)
	{
		txtSnake->SetAlpha(0.5f);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOMaskFound)
	{
		txtMask->SetAlpha(0.5f);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOOwlFound)
	{
		txtOwl->SetAlpha(0.5f);
	}
		
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOLionHeadFound)
	{
		txtLionHead->SetAlpha(0.5f);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOTreeFound)
	{
		txtTree->SetAlpha(0.5f);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOGoddessFound)
	{
		txtGoddess->SetAlpha(0.5f);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_HOTurtleFound)
	{
		txtTurtle->SetAlpha(0.5f);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed && ProfileSystem::Instance->ActiveProfile.L1S3_HOFishFound)
	{
		txtFish->SetAlpha(0.5f);
	}
}

void L1_S3_DoorHO::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
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
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if (itemVal == eInv_L1S3_HOLocket)
			{				
				if( !ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced)
				{
					if(InvDestRectHOList.Intersection(&dropPos))
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_HOLocket] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S3_HOLocket);
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOEmblemFixed);
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3HOObjectSilClicked);
						ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced = true;

						isLocketPlaceAnimPlaying = true;
						Hud->AllowInput = false;
						CAnim_LocketPlaceMove->PlayAnim();
					}
					else
					{
						GFInstance->WrongInvDropMsg(Hud);
					}
				}
				else
				{
					GFInstance->WrongInvDropMsg(Hud);
				} 
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}
}

void L1_S3_DoorHO::ProcessNonHOHint()
{
	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		return;

	AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	int iNonHOHint = 0; 
	int MaxNonHOHints = 24;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced && ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken )
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S3_HOLocket)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectHOList.GetCenterX(),InvDestRectHOList.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 1: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced && !ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOListPanelTaken )
					{
						InitHintTrailEmit(InvDestRectHOList.GetCenterX(),InvDestRectHOList.GetCenterY(),false);
						return;
					}
					break;		
			case 2: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HORoseFlowerFound && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(rose->Pos.x,rose->Pos.y,false);
						return;
					}
					break;
			
			case 3: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOCrescentFound && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(crescent->Pos.x,crescent->Pos.y,false);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOSnakeFound && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(snake->Pos.x,snake->Pos.y,false);
						return;
					}
					break;
					
			case 5: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOMaskFound && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(mask->Pos.x,mask->Pos.y,false);
						return;
					}
					break;
		   case 6: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOOwlFound && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(owl->Pos.x,owl->Pos.y,false);
						return;
					}
					break;
			case 7: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[0] && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(leaves[0]->Pos.x,leaves[0]->Pos.y,false);
						return;
					}
					break;
					
			case 8: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[1] && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(leaves[1]->Pos.x,leaves[1]->Pos.y,false);
						return;
					}
					break;
			
			case 9: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[2] && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(leaves[2]->Pos.x,leaves[2]->Pos.y,false);
						return;
					}
					break;
			case 10: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOLionHeadFound && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(lionhead->Pos.x,lionhead->Pos.y,false);
						return;
					}
					break;
			case 11: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOFishFound &&  ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(fish->Pos.x,fish->Pos.y,false);
						return;
					}
					break;		
			case 12: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[0] && ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(Stars[0]->Pos.x,Stars[0]->Pos.y,false);
						return;
					}
					break;
			case 13: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[1]  && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(Stars[1]->Pos.x,Stars[1]->Pos.y,false);
						return;
					}
					break;
			case 14: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOTreeFound  && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(tree->Pos.x,tree->Pos.y,false);
						return;
					}
					break;
			case 15: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOGoddessFound  && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(goddes->Pos.x,goddes->Pos.y,false);
						return;
					}
					break;
			case 16: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOTurtleFound  && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(turtle->Pos.x,turtle->Pos.y,false);
						return;
					}
					break;
			case 17: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOTurtleFound  && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(turtle->Pos.x,turtle->Pos.y,false);
						return;
					}
					break;
			case 18: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapPuzzleSolved  && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(swapHand[2]->Pos.x,swapHand[2]->Pos.y,false);
						return;
					}
					break;
			case 19: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOMemoryPuzzleSolved  && ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced )
					{
						InitHintTrailEmit(fishtail->Pos.x,fishtail->Pos.y,false);
						return;
					}
					break;
			case 20: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed  && ProfileSystem::Instance->ActiveProfile.L1S3_HOMemoryPuzzleSolved )
					{
						InitHintTrailEmit(fishtail->Pos.x,fishtail->Pos.y,false);
						CPoint tempPoint (fish->Pos.x,fish->Pos.y,0);
						Hud->Hint2FX->ResetPos(&tempPoint);
						Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 21: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_KeyFixed  && ProfileSystem::Instance->ActiveProfile.L1S3_HOMemoryPuzzleSolved )
					{
						InitHintTrailEmit(keyInside->Pos.x,keyInside->Pos.y,false);
						CPoint tempPoint (keyfixed->Pos.x,keyfixed->Pos.y,0);
						Hud->Hint2FX->ResetPos(&tempPoint);
						Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 22: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_HOSolved  && ProfileSystem::Instance->ActiveProfile.L1S3_KeyFixed )
					{
						for (int i = 0; i < 3; i++)
						{
							if(!ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[i])
							{
								//Lock Not Opened.
								ShowHintForLocketPuzzle(i);
								break;
							}
						}
						return;
					}
					break;
			case 23: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S3_Foyer])
					{
						InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
						return;
					}
					break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
	
}

void L1_S3_DoorHO::ShowHintForLocketPuzzle(int lockID)
{
	if(isLockMoving)
		return;

	if(!isLockSelected)
	{
		InitHintTrailEmit(keyfixed->Pos.x,keyfixed->Pos.y,true);
		return;
	}

	if(lockID == 0)
	{
		if(currentLockPos==1)
		{
			//Show to 2
			InitHintTrailEmit(&polyLockTargetNew[2]->center);
		}
		else
		{
			//Show to 1
			InitHintTrailEmit(&polyLockTargetNew[1]->center);
		}
	}
	else if(lockID == 1)
	{
		if(currentLockPos==4)
		{
			//Show to 2
			InitHintTrailEmit(&polyLockTargetNew[5]->center);
		}
		else
		{
			//Show to 1
			InitHintTrailEmit(&polyLockTargetNew[4]->center);
		}
	}
	else if(lockID == 2)
	{
		if(currentLockPos==0)
		{
			//Show to 2
			InitHintTrailEmit(&polyLockTargetNew[6]->center);
		}
		else
		{
			//Show to 1
			InitHintTrailEmit(&polyLockTargetNew[0]->center);
		}
	}
}

void L1_S3_DoorHO::ResetHO()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;

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
			if(ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced && !ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_HOLocket])
			{
				ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketPlaced = false;

				std::vector<int> vecInvIds;
				Hud->InvBoxCtrl->Activate(1);			
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_HOLocket] = true;
				ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken = true;
				vecInvIds.push_back(eInv_L1S3_HOLocket);
				Hud->ShowInventory(&vecInvIds);
			}


			ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOLocketDoorTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_IsDoorHOListPanelTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_KeyFixed = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_KeyDoorOpened = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOStarDoorOpened = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HORoseFlowerFound = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOCrescentFound = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOSnakeFound = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOMaskFound = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOOwlFound = false;
			for (int i = 0; i < 3; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L1S3_HOLeavesFound[i] = false;
			}

			ProfileSystem::Instance->ActiveProfile.L1S3_HOLionHeadFound = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOFishFound = false;
			ProfileSystem::Instance->ActiveProfile.L1S4_HOFishTailFixed = false;
			for (int i = 0; i < 2; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L1S3_HOStarsFound[i] = false;
			}
			ProfileSystem::Instance->ActiveProfile.L1S3_HOTreeFound = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOGoddessFound = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOTurtleFound = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapPuzzleSolved = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOMemoryPuzzleSolved = false;
			for (int i = 0; i < 3; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L1S3_HOLockOpened[i] = false;
			}

			for (int i = 0; i < 4; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L1S3_HOSwapLeverAngle[i] = 0.0f;
			}

			ProfileSystem::Instance->ActiveProfile.L1S3_HOSolved = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_HOCompletedWithMatch3 = false;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_DoorHO;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			IsResetPop = false;
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

void L1_S3_DoorHO::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);

				//GFHud->InvBoxCtrl->NavigateText->Show(false);
				if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
				{
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
					SendMessage(ControlExit, ID, HOSM_NewArea);
					if(match3)
						match3->clearBoard();
					HOScene->GetObjectByID("txtMatch3")->Show(false);
					HOScene->GetObjectByID("txtPuzzle")->Show(false);
					ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
					isMatch3Playing = false;
				} else {
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = GFHud->InvBoxCtrl->travelArea;
					SendMessage(ControlExit, ID, HOSM_NewArea);
				}
			}

			GFHud->InvBoxCtrl->NavigateText->Text = "";
			GFHud->InvBoxCtrl->travelArea = -1;
			GFHud->InvBoxCtrl->navTapped = false;
			GFHud->InvBoxCtrl->Navigate->Show(false);
			GFHud->InvBoxCtrl->NavArrow->Show(false);
		}
	}
}
