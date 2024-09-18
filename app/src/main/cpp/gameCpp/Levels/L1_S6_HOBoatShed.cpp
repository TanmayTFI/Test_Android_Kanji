//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 02 : Frontyard
//====================================

#include "L1_S6_HOBoatShed.h"
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
#include "TextureFactory.h"
#include "AwardManager.h"

const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,98,0);

//const CPoint funnelFullPos(779, 100, 0);
//const CPoint funnelWithTapePos(791, 99, 0);
//const CPoint funnelWithTapeCutPos(799, 100, 0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  areaRectMatch3(536.0f,2.0f,286.0f,51.0f);


//Scene Rects
const CRectangle  SceneRectFrontdoor(666.0f,260.0f,179.0f,196.0f);

const CRectangle  SceneRectBoatdock(355.0f,320.0f,171.0f,285.0f);

//Popup Rects
const CRectangle  PopRectCarDickie(851.0f,407.0f,313.0f,287.0f);

const CPoint ClothPos(611, 361, 0);
const CPoint StickAnimPos1(715, 398, 0);
const CPoint StickAnimPos2(737, 177, 0);
const CPoint TapeRollingAnimPos1(250, 157, 0);
const CPoint TapeCuttingAnimPos1(264, 140, 0);
const CPoint TapeRollingAnimPos2(700, 27, 0);
const CPoint TapeCuttingAnimPos2(714, 10, 0);

const CPoint SquirrelRunAnim1Pos(761, 23, 0);
const CPoint SquirrelRunAnim2Pos(731, 5, 0);
const CPoint SquirrelRunAnim3Pos(875, 5, 0);

const CPoint MagnetAnimPos(984, 309, 0);
const CPoint PaperClipAnimPos(713, 412, 0);
const CPoint OilDrippingAnimPos(621, 88, 0);
const CPoint SmallBoxDoorOpenAnimPos(757, 1, 0);

L1_S6_HOBoatShed::L1_S6_HOBoatShed()
{
	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S6_HOBoatShed])
		AwardManager::GetInstance()->StartHO(eArea_L1_S6_HOBoatShed);
	isMouseAlreadyHandled = false;

	InitializeInventoryBox();

	musicFadeOutTimer = 0;
	isSquirrelAngryAnimPlaying = false;
	isClothAnimationPlaying = false;
	isStickAnimationPlaying = false;
	isTapeRollingAnimPlaying = false;
	isTapeCuttingAnimPlaying = false;
	isSquirrelRunAnimationPlaying = false;
	isMagnetAnimating = false;
	isPaperClipAnimPlaying = false;
	isOilDrippingAnimPlaying = false;
	isSmallBoxDoorOpenAnimPlaying = false;
	isLeftMiniBoxDoorOpenAnimPlaying = false;
	match3 = NULL;
	isMatch3Playing = false;

	squirrelTimer = 0.0f;
	squirrelThreshold = ( 2.0f + std::rand()%3 );

	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	Visible = false;

	isToolMoving = false;
	isToolMovingInLeftMiniBox = false;
	isObjectReturning = false;
	isObjectInHand = false;
	tweenTime = 0.0f;
	swapTime = 0.70f;

	//Main Scene
	LoadHud(INVENTORY_V1);

	LoadScene("LV1\\S6\\HO\\HO.SCN");

	squirrel = HOScene->GetObjectByID("squirrel");
	acon = HOScene->GetObjectByID("acon");
	beettle = HOScene->GetObjectByID("beettle");
	blade = HOScene->GetObjectByID("blade");
	clothOnHand = HOScene->GetObjectByID("clothOnHand");
	clothOnHand->Show(false);
	oilDipperOnHand = HOScene->GetObjectByID("oilDipperOnHand");
	oilDipperOnHand->Show(false);
	beetleAnimObject = HOScene->GetObjectByID("beetleAnim1");
	beetleAnimObject->Show(false);

	tapeCutAnimObjects[0] = HOScene->GetObjectByID("Blade_Anim");
	tapeCutAnimObjects[1] = HOScene->GetObjectByID("funnelFull");
	tapeCutAnimObjects[2] = HOScene->GetObjectByID("MaskCut_Anim");
	tapeCutAnimObjects[3] = HOScene->GetObjectByID("MaskUncut_Anim");
	tapeCutAnimObjects[4] = HOScene->GetObjectByID("tape_Anim");

	tapeRollAnimObj1 = HOScene->GetObjectByID("funnelFull");
	tapeRollAnimObj2 = HOScene->GetObjectByID("MaskUncut_Anim");
	tapeRollAnimObj3 = HOScene->GetObjectByID("tape_Anim");

	HOScene->GetObjectByID("funnel_Anim")->SetEnable(false);

	leftMiniBoxOpenAnimObject1 = HOScene->GetObjectByID("leftMiniBoxDoorWithHandle");
	leftMiniBoxOpenAnimObject1->Show(false);
	leftMiniBoxOpenAnimObject2 = HOScene->GetObjectByID("leftMiniBoxDoorOpen1");
	leftMiniBoxOpenAnimObject2->Show(false);
	leftMiniBoxOpenAnimObject3 = HOScene->GetObjectByID("leftMiniBoxDoorOpen2");
	leftMiniBoxOpenAnimObject3->Show(false);

	for (int i = 0; i < 5; i++)
	{
		tapeCutAnimObjects[i]->Show(false);
	}

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

	pointerInHand = new CObject;

	boxDoorOpen = HOScene->GetObjectByID("boxDoorOpen");

	closedBox = HOScene->GetObjectByID("closedBox");
	chestDoorVFX_0 = NULL;
	chestDoorVFX_1 = NULL;
	chestDoorVFX_2 = NULL;
	rayLeftVFX = new SuperFX("PRT\\L1\\lightprt.INI", PointSystem::CreateCPoint(294,285,0), 100);
	rayLeftVFX->Start();
	rayRightVFX = new SuperFX("PRT\\L1\\boatShedHORayRightVFX.INI", PointSystem::CreateCPoint(1146,273,0), 100); 
	rayRightVFX->Start();

	if (!ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened)
	{
		chestDoorVFX_0 = new SuperFX("PRT\\L1\\chestDoorShakeVFX_0.INI", PointSystem::CreateCPoint(249,349,0), closedBox->ZOrder - 1); 
		chestDoorVFX_1 = new SuperFX("PRT\\L1\\chestDoorShakeVFX_1.INI", PointSystem::CreateCPoint(657,352,0), closedBox->ZOrder - 1); 
		chestDoorVFX_2 = new SuperFX("PRT\\L1\\chestDoorShakeVFX_2.INI", PointSystem::CreateCPoint(900,31,0), closedBox->ZOrder - 1);  
	}

	paper = HOScene->GetObjectByID("paper");

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperCutScenePlayed)
	{
		//Tutorial show
		if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
		{
			//puzzle is activated only when fuses are fixed
			if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT21_Match3Tutorial])
				Hud->Tutor->ShowTutorial(TUT21_Match3Tutorial);
		}

		paper->SetEnable(false);
	}

	CObject* rayRight = HOScene->GetObjectByID("rayright");
	rayRight->SetPivot(257.0/2, 274.0/2);
	rayRight->SetScale(2);
	rayRight->Animate(0.7f, 0.9f, 5);

	CObject* rayLeft = HOScene->GetObjectByID("rayleft");
	rayLeft->SetPivot(-275.0/2, -347.0/2);
	rayLeft->SetScale(2);
	rayLeft->Animate(0.7f, 0.9f, 5);

	cloth = HOScene->GetObjectByID("cloth");
	funnelFull = HOScene->GetObjectByID("funnelFull");
	funnelFullWithTape = HOScene->GetObjectByID("funnelFullWithTape");
	funnelFullWithTapeCut = HOScene->GetObjectByID("funnelFullWithTapeCut");
	funnelHead = HOScene->GetObjectByID("funnelHead");
	funnelNose = HOScene->GetObjectByID("funnelNose");
	handle = HOScene->GetObjectByID("leftMiniBoxDoorHandleHO");
	handleOnMiniDoor = HOScene->GetObjectByID("leftMiniBoxDoorHandle");
	closedLeftMiniDoor = HOScene->GetObjectByID("leftMiniBoxDoor");
	openedLeftMiniDoor = HOScene->GetObjectByID("leftMiniBoxDoorOpen3");
	glove = HOScene->GetObjectByID("glove");
	magnet = HOScene->GetObjectByID("magnet");
	magnet2 = HOScene->GetObjectByID("magnetPlaced");
	oilDipper = HOScene->GetObjectByID("oilDipper");
	paperclip = HOScene->GetObjectByID("paperclip");
	paperclip2 = HOScene->GetObjectByID("paperClipOnMagnet");
	rust1 = HOScene->GetObjectByID("rust1");
	rustOnSmallBox = HOScene->GetObjectByID("rustOnSmallBox");
	smallBoxDoorClosed = HOScene->GetObjectByID("rightMiniBoxDoor");
	smallBoxDoorOpen = HOScene->GetObjectByID("rightMiniBoxDoorOpen");
	stick = HOScene->GetObjectByID("stick");
	tape = HOScene->GetObjectByID("tape");
	tool1 = HOScene->GetObjectByID("tool1");
	tool2 = HOScene->GetObjectByID("tool2");
	tool3 = HOScene->GetObjectByID("tool3");
	UseObjects = HOScene->GetObjectByID("UseObjects");
	oilDripAnim = HOScene->GetObjectByID("OilDripAnim");
	oilDripAnim->SetEnable(false);
	

	tapeRollAnimObj1->Show(false);
	tapeRollAnimObj2->Show(false);
	tapeRollAnimObj3->Show(false);

	vidTimer = 0;

	beettle = HOScene->GetObjectByID("beettle");

	/*
	txtMatch3 = HOScene->GetObjectByID("btnMatch3");
	txtMatch3MouseOver = HOScene->GetObjectByID("btnMatch3Over");
	txtPuzzle = HOScene->GetObjectByID("btnSwitchToHO");
	txtPuzzleMouseOver = HOScene->GetObjectByID("btnSwitchToHOOver");
	*/

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
	txtMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 683, 30, 35);
	txtMatch3->SetUID("btnMatch3");
	HOScene->PushObject(txtMatch3);

	txtPuzzle = new CObject();
	txtPuzzle->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 683, 30, 35);
	txtPuzzle->SetUID("btnSwitchToHO");
	HOScene->PushObject(txtPuzzle);

	txtMatch3MouseOver = new CObject();
	txtMatch3MouseOver->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 683, 30, 35);
	txtMatch3MouseOver->SetUID("btnMatch3");
	txtMatch3MouseOver->PlayAnimation(1);
	HOScene->PushObject(txtMatch3MouseOver);

	txtPuzzleMouseOver = new CObject();
	txtPuzzleMouseOver->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 683, 30, 35);
	txtPuzzleMouseOver->SetUID("btnSwitchToHO");
	txtPuzzleMouseOver->PlayAnimation(1);
	HOScene->PushObject(txtPuzzleMouseOver);

	txtMatch3->SetZ(eZOrder_Popup + 300);
	txtPuzzle->SetZ(eZOrder_PopupOverPopup + 300);

	txtMatch3MouseOver->SetZ(eZOrder_Popup + 301);
	txtPuzzleMouseOver->SetZ(eZOrder_PopupOverPopup + 302);
	txtPuzzle->Show(false);
	txtMatch3MouseOver->Show(false);
	txtPuzzleMouseOver->Show(false);

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
	{
		txtMatch3->Show(false);
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	moveableTools[0] = tool1;
	moveableTools[1] = tool2;
	moveableTools[2] = tool3;
	for (int i = 0; i < 3; i++)
	{
		leftMiniBoxMovableTools[i] = HOScene->GetObjectByID(std::string("miniBoxObj") + MKSTR(i + 1));
	}

	cAnimStick = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved)
	{
		CObject* objArr[] = {HOScene->GetObjectByID("stick"), HOScene->GetObjectByID("beetleAnim1")};
		std::string animArr[] = {"ANIM\\L1\\S6Stick_StickAnim.anim", "ANIM\\L1\\S6Beetle_StickAnim.anim"};
		cAnimStick = new CustomAnimGroup(2, animArr, objArr);
	}

	SmallBoxDoorOpenAnim = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened)
	{
		SmallBoxDoorOpenAnim = new CObject();
		SmallBoxDoorOpenAnim->LoadSprite("LV1\\S6\\HO\\SmallBoxDoorOpenAnim.SPR");
		SmallBoxDoorOpenAnim->Pos = SmallBoxDoorOpenAnimPos;
		SmallBoxDoorOpenAnim->SetZ(24);
		SmallBoxDoorOpenAnim->Show(false);
	}

	cAnimOilDipping = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved)
	{
		CObject* objArr[] = {HOScene->GetObjectByID("oilDipperOnHand")};
		std::string animArr[] = {"ANIM\\L1\\S6OilDipper.anim"};
		cAnimOilDipping = new CustomAnimGroup(1, animArr, objArr);
	}

	PaperClipAnim = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed)
	{
		PaperClipAnim = new CObject();
		PaperClipAnim->LoadSprite("LV1\\S6\\HO\\PaperClipAnim.SPR");
		PaperClipAnim->Pos = PaperClipAnimPos;
		PaperClipAnim->SetZ(24);
		PaperClipAnim->Show(false);
	}

	cAnimMagnet = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken)
	{
		CObject* objArr[] = {magnet2, paperclip};
		std::string animArr[] = {"ANIM\\L1\\S6Magnetplaced.anim", "ANIM\\L1\\S6Paperclip.anim"};
		cAnimMagnet = new CustomAnimGroup(2, animArr, objArr);
	}

	cAnimLeftMiniBoxOpen = NULL;
	//if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved)
	{

		CObject* objArr[] = {HOScene->GetObjectByID("leftMiniBoxDoorWithHandle"), HOScene->GetObjectByID("leftMiniBoxDoorOpen1"), HOScene->GetObjectByID("leftMiniBoxDoorOpen2"), HOScene->GetObjectByID("leftMiniBoxDoorOpen3")};
		std::string animArr[] = {"ANIM\\L1\\S6LeftMiniBoxDoorWithHandle.anim", "ANIM\\L1\\S6LeftMiniBoxDoorOpen1.anim", "ANIM\\L1\\S6LeftMiniBoxDoorOpen2.anim", "ANIM\\L1\\S6LeftMiniBoxDoorOpen3.anim"};
		cAnimLeftMiniBoxOpen = new CustomAnimGroup(4, animArr, objArr);
	}

	cAnimCloth = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOLeftMiniDoorOpened)
	{

		CObject* objArr[] = {HOScene->GetObjectByID("clothOnHand"), HOScene->GetObjectByID("rust1")};
		std::string animArr[] = {"ANIM\\L1\\S6ClothInHand_ClothAnim.anim", "ANIM\\L1\\S6Rust_ClothAnim.anim"};
		cAnimCloth = new CustomAnimGroup(2, animArr, objArr);
	}


	for (int i = 0; i < 3; i++)
	{
		SquirrelRunAnim[i] = NULL;
	}
	squirrelAngryAnim = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved)
	{
		
		squirrelAngryAnim = new CObject();
		squirrelAngryAnim->LoadSprite("LV1\\S6\\HO\\SquirrelAngry.SPR");
		squirrelAngryAnim->Pos = CPoint(759.0f, 21, 0.0f);
		squirrelAngryAnim->SetZ(60);
		squirrelAngryAnim->Show(false);

		squirrelRunAnimPointer = 0;
		SquirrelRunAnim[0] = new CObject();
		SquirrelRunAnim[0]->LoadSprite("LV1\\S6\\HO\\SquirrelRunAnim1.SPR");
		SquirrelRunAnim[0]->Pos = SquirrelRunAnim1Pos;
		SquirrelRunAnim[0]->SetZ(50);
		SquirrelRunAnim[0]->Show(false);

		SquirrelRunAnim[1] = new CObject();
		SquirrelRunAnim[1]->LoadSprite("LV1\\S6\\HO\\SquirrelRunAnim2.SPR");
		SquirrelRunAnim[1]->Pos = SquirrelRunAnim2Pos;
		SquirrelRunAnim[1]->SetZ(50);
		SquirrelRunAnim[1]->Show(false);

		SquirrelRunAnim[2] = new CObject();
		SquirrelRunAnim[2]->LoadSprite("LV1\\S6\\HO\\SquirrelRunAnim3.SPR");
		SquirrelRunAnim[2]->Pos = SquirrelRunAnim3Pos;
		SquirrelRunAnim[2]->SetZ(50);
		SquirrelRunAnim[2]->Show(false);
	}


	cAnimTapeRoll = NULL;

	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel)
	{
		CObject* objArr[] = { HOScene->GetObjectByID("MaskUncut_Anim"), HOScene->GetObjectByID("tape_Anim")};
		std::string animArr[] = { "ANIM\\L1\\S6Mask_TapeRoll.anim", "ANIM\\L1\\S6Tape_TapeRoll.anim"};
		cAnimTapeRoll = new CustomAnimGroup(2, animArr, objArr);
	}

	cAnimTapeCut = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeCutOnFunnel)
	{
		CObject* objArr[] = {tapeCutAnimObjects[0],  tapeCutAnimObjects[2], tapeCutAnimObjects[3], tapeCutAnimObjects[4]};
		std::string animArr[] = {"ANIM\\L1\\Blade_TapeCut.anim", "ANIM\\L1\\Mask_TapeCut.anim", "ANIM\\L1\\MaskFinal_TapeCut.anim", "ANIM\\L1\\Tape_TapeCut.anim"};
		cAnimTapeCut = new CustomAnimGroup(4, animArr, objArr);
	}

	toolPointer = 0;
	leftMiniBoxToolPointer = 0;
	for (int i = 0; i < 3; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolMoved[i])
		{
			toolPointer++;
			moveableTools[i]->Show(false);
		}

		if (ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolInLeftMiniBoxMoved[i])
		{
			leftMiniBoxToolPointer++;
			leftMiniBoxMovableTools[i]->Show(false);
		}
	}

	isAllToolsMoved = false;
	isAllToolsInLeftMiniBoxMoved = false;

	if(toolPointer == 3) isAllToolsMoved = true;
	if(leftMiniBoxToolPointer == 3) isAllToolsInLeftMiniBoxMoved = true;

	UseObjects = HOScene->GetObjectByID("UseObjects");
	UseObjects->SetZ(eZOrder_INVBOXBG+10);
	UseObjects->Show(false);

	if (ProfileSystem::Instance->ActiveProfile.L1S6_HOLeftMiniDoorOpened)
	{
		openedLeftMiniDoor->Show(true);
		closedLeftMiniDoor->Show(false);
		handleOnMiniDoor->Show(false);
	}
	else
	{
		openedLeftMiniDoor->Show(false);
		closedLeftMiniDoor->Show(true);
		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Handle) > kInventoryItemState_Collected)
		{
			handleOnMiniDoor->Show(true);
		}
		else
		{
			handleOnMiniDoor->Show(false);
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened)
	{
		boxDoorOpen->Show(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed)
		{
			HOScene->GetObjectByID("lache")->Show(false);
		}
		else
		{
			HOScene->GetObjectByID("lacheOpen")->Show(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved)
		{
			squirrel->Show(false);
		}
		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Acorn) >= kInventoryItemState_Collected)
		{
			acon->Show(false);
		}
		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved)
		{
			cloth->Show(false);
			rust1->Show(false);
		}
		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Stick) >= kInventoryItemState_Collected)
		{
			stick->Show(false);
		}
		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved)
		{
			beettle->Show(false);
			isBeetleAnimPlaying = false;
		}
		else
		{
			isBeetleAnimPlaying = true;
		}
		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Magnet) >= kInventoryItemState_Collected)
		{
			magnet->Show(false);
		}
		if (hoInventoryBox->GetInventoryItemStateOf(kInv_PaperClip) >= kInventoryItemState_Collected)
		{
			paperclip2->Show(false);
			magnet2->Show(false);
		}
		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Cloth) >= kInventoryItemState_Collected)
		{
			cloth->Show(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken)
		{
			magnet->Show(false);
			paperclip->Show(false);
			if(ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed)
			{
				magnet2->Show(false);
				paperclip2->Show(false);
			}
		}
		else
		{
			magnet2->Show(false);
			paperclip2->Show(false);
		}
	}
	else
	{
		HOScene->GetObjectByID("lache")->Show(false);
		HOScene->GetObjectByID("lacheOpen")->Show(false);
		closedBox->Show(false);
		magnet2->Show(false);
		paperclip->Show(false);
		paperclip2->Show(false);
		magnet->Show(false);
		cloth->Show(false);
		rust1->Show(false);
		stick->Show(false);
		beettle->Show(false);
		squirrel->Show(false);
		acon->Show(false);


		if (hoInventoryBox->GetInventoryItemStateOf(kInv_OilDripper) >= kInventoryItemState_Collected)
		{
			oilDipper->Show(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved)
		{
			oilDipper->Show(false);
		}

		if (hoInventoryBox->GetInventoryItemStateOf(kInv_FunnelNose) >= kInventoryItemState_Collected)
		{
			funnelNose->Show(false);
		}

		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Handle) >= kInventoryItemState_Collected)
		{
			handle->Show(false);
		}

		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Tape) >= kInventoryItemState_Collected)
		{
			tape->Show(false);
		}


		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Glove) >= kInventoryItemState_Collected)
		{
			glove->Show(false);
		}

		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Blade) >= kInventoryItemState_Collected)
		{
			blade->Show(false);
		}

		if (hoInventoryBox->GetInventoryItemStateOf(kInv_Handle) >= kInventoryItemState_Collected)
		{
			handle->Show(false);
		}



		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeCutOnFunnel)
		{
			funnelHead->Show(false);
			funnelNose->Show(false);
			funnelFull->Show(false);
			funnelFullWithTape->Show(false);
		}

		UpdateInsideBoxProperties();

	}


	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	

	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
	{
		Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
		Hud->IsTakeAndUseHOScene = true;
		//Hud->IsHOScene = true;
	}
	else
	{
		Hud->IsHOScene = false;
		//DisableText();
	}

	GFInstance->PostUpdate(2);	


	int ArrayPts1[] = {944,499,917,499,906,479,916,455,942,439,964,451,975,471};
	polyAconGet =  new CPolygon(7);
	polyAconGet->Init(ArrayPts1);
	getPoly[kInv_Acorn] = polyAconGet;
	getObject[kInv_Acorn] = acon;
	mousePointerString[kInv_Acorn] = "LV1\\S6\\HO\\acon.SPR";

	int ArrayPts2[] = {771,57,781,231,939,288,1004,149,967,36,872,9};
	polyAconSet=  new CPolygon(6);
	polyAconSet->Init(ArrayPts2);
	setPoly[kInv_Acorn] = polyAconSet;
	setObject[kInv_Acorn] = squirrel;


	int ArrayPts3[] = {983,503,987,535,1055,571,1101,532,1045,484};
	polyClothGet=  new CPolygon(5);
	polyClothGet->Init(ArrayPts3);
	getPoly[kInv_Cloth] = polyClothGet;
	getObject[kInv_Cloth] = cloth;
	mousePointerString[kInv_Cloth] = "LV1\\S6\\HO\\clothOnHand.SPR";

	int ArrayPts4[] = {639,512,623,431,789,362,800,448};
	polyClothSet =  new CPolygon(4);
	polyClothSet->Init(ArrayPts4);
	setPoly[kInv_Cloth] = polyClothSet;
	setObject[kInv_Cloth] = rust1;

	int ArrayPts5[] = {515,627,507,594,598,579,671,547,799,465,835,491,612,619,577,608};
	polyStickGet=  new CPolygon(8);
	polyStickGet->Init(ArrayPts5);
	getPoly[kInv_Stick] = polyStickGet;
	getObject[kInv_Stick] = stick;
	mousePointerString[kInv_Stick] = "LV1\\S6\\HO\\stick.SPR";

	int ArrayPts6[] = {813,499,805,690,1159,644,1183,405};
	polyStickSet=  new CPolygon(4);
	polyStickSet->Init(ArrayPts6);
	setPoly[kInv_Stick] = polyStickSet;
	setObject[kInv_Stick] = beettle;

	int ArrayPts7[] = {1141,308,1085,270,1102,201,1184,236};
	polyMagnetGet=  new CPolygon(4);
	polyMagnetGet->Init(ArrayPts7);
	getPoly[kInv_Magnet] = polyMagnetGet;
	getObject[kInv_Magnet] = magnet;
	mousePointerString[kInv_Magnet] = "LV1\\S6\\HO\\magnetOnHand.SPR";

	int ArrayPtsMagnet[] = {1055,484,1121,484,1158,416,1111,362,1036,380};
	polyMagnetSet=  new CPolygon(5);
	polyMagnetSet->Init(ArrayPtsMagnet);
	setPoly[kInv_Magnet] = polyMagnetSet;
	setObject[kInv_Magnet] = paperclip;

	int ArrayPts9[] = {917,529,883,583,908,597,946,547};
	polyPaperClipGet=  new CPolygon(4);
	polyPaperClipGet->Init(ArrayPts9);
	getPoly[kInv_PaperClip] = polyPaperClipGet;
	getObject[kInv_PaperClip] = paperclip2;
	mousePointerString[kInv_PaperClip] = "LV1\\S6\\HO\\paperclip.SPR";

	int ArrayPts10[] = {639,512,623,431,789,362,800,448};
	polyPaperClipSet =  new CPolygon(4);
	polyPaperClipSet->Init(ArrayPts10);
	setPoly[kInv_PaperClip] = polyPaperClipSet;
	setObject[kInv_PaperClip] = closedBox;

	int ArrayPts11[] = {857,169,764,170,728,120,771,66,837,92,872,142};
	polyFunnelGet =  new CPolygon(6);
	polyFunnelGet->Init(ArrayPts11);
	getPoly[kInv_FunnelNose] = polyFunnelGet;
	getObject[kInv_FunnelNose] = funnelNose;
	mousePointerString[kInv_FunnelNose] = "LV1\\S6\\HO\\funnelNose.SPR";

	int ArrayPts12[] = {343,280,263,299,248,276,279,192,322,142,362,134,379,171,384,237,390,279};
	polyFunnelSet =  new CPolygon(9);
	polyFunnelSet->Init(ArrayPts12);
	setPoly[kInv_FunnelNose] = polyFunnelSet;
	setObject[kInv_FunnelNose] = funnelHead;


	int ArrayPtsHandle[] = {299,291,331,391,369,378,338,278};
	polyHandleGet=  new CPolygon(4);
	polyHandleGet->Init(ArrayPtsHandle);
	getPoly[kInv_Handle] = polyHandleGet;
	getObject[kInv_Handle] = handle;
	mousePointerString[kInv_Handle] = "LV1\\S6\\HO\\leftMiniBoxDoorHandle.SPR";

	int ArrayPtsHandleSet[] = {290,424,338,538,530,453,468,350};
	polyHandleSet=  new CPolygon(4);
	polyHandleSet->Init(ArrayPtsHandleSet);
	setPoly[kInv_Handle] = polyHandleSet;
	setObject[kInv_Handle] = handleOnMiniDoor;


	int ArrayPts13[] = {600,415,529,418,502,365,599,325,676,323,723,364,642,416};
	polyOilDipperGet =  new CPolygon(7);
	polyOilDipperGet->Init(ArrayPts13);
	getPoly[kInv_OilDripper] = polyOilDipperGet;
	getObject[kInv_OilDripper] = oilDipper;
	mousePointerString[kInv_OilDripper] = "LV1\\S6\\HO\\oilDipperOnHand.SPR";

	int ArrayPts14[] = {803,412,666,212,948,49,1112,289};
	polyOilDipperSet=  new CPolygon(4);
	polyOilDipperSet->Init(ArrayPts14);
	setPoly[kInv_OilDripper] = polyOilDipperSet;
	setObject[kInv_OilDripper] = rustOnSmallBox;

	int ArrayPts15[] = {590,223,484,246,428,222,426,156,507,140,594,110,699,82,719,222};
	polyGloveGet=  new CPolygon(8);
	polyGloveGet->Init(ArrayPts15);
	getPoly[kInv_Glove] = polyGloveGet;
	getObject[kInv_Glove] = glove;
	mousePointerString[kInv_Glove] = "LV1\\S6\\HO\\gloveOnHand.SPR";


	int ArrayPts16[] = {763,215,850,318,1064,234,1004,101};
	polyGloveSet=  new CPolygon(4);
	polyGloveSet->Init(ArrayPts16);
	setPoly[kInv_Glove] = polyGloveSet;
	setObject[kInv_Glove] = smallBoxDoorClosed;

	int ArrayPts17[] = {882,295,828,267,902,217,976,253,895,304};
	polyTapeGet =  new CPolygon(5);
	polyTapeGet->Init(ArrayPts17);
	getPoly[kInv_Tape] = polyTapeGet;
	getObject[kInv_Tape] = tape;
	mousePointerString[kInv_Tape] = "LV1\\S6\\HO\\tapeOnHand.SPR";

	int ArrayPts18[] = {407,316,356,291,280,321,235,267,312,150,376,164,426,296};
	polyTapeSet =  new CPolygon(7);
	polyTapeSet->Init(ArrayPts18);
	setPoly[kInv_Tape] = polyTapeSet;
	setObject[kInv_Tape] = funnelFullWithTape;


	int ArrayPtsBladeGet[] = {387,464,433,415,473,446,420,498};
	polyBladeGet=  new CPolygon(4);
	polyBladeGet->Init(ArrayPtsBladeGet);
	getPoly[kInv_Blade] = polyBladeGet;
	getObject[kInv_Blade] = blade;
	mousePointerString[kInv_Blade] = "LV1\\S6\\HO\\bladeOnHand.SPR";

	setPoly[kInv_Blade] = polyTapeSet;
	setObject[kInv_Blade] = funnelFullWithTapeCut;

	/*int ArrayPts22[] = {295,562,150,254,759,8,923,17,1053,221};
	PolyBoxDoorArea =  new CPolygon(5);
	PolyBoxDoorArea->Init(ArrayPts22);*/

	int ArrayPts22[] = {133,186,295,556,1052,216,922,7,821,44,773,72,558,56};
	PolyBoxDoorArea=  new CPolygon(7);
	PolyBoxDoorArea->Init(ArrayPts22);


	int ArrayPts23[] = {391,296,344,282,280,308,258,299,277,227,322,164,356,152,380,197,375,238,404,270};
	polyInvFunnel1 =  new CPolygon(10);
	polyInvFunnel1->Init(ArrayPts23);

	int ArrayPts24[] = {844,164,796,149,728,174,709,165,721,103,772,33,791,19,820,41,822,96,817,112,855,146};
	polyInvFunnel2 =  new CPolygon(11);
	polyInvFunnel2->Init(ArrayPts24);

	int ArrayPts25[] = {395,331,495,162,557,136,827,226,730,467};
	polyPaper =  new CPolygon(5);
	polyPaper->Init(ArrayPts25);

	int Tool1Pts[] = {621,31,436,214,472,272,573,190,716,297,759,249,625,131,676,45};
	PolyToolArr[0]=  new CPolygon(8);
	PolyToolArr[0]->Init(Tool1Pts);
	int Tool2Pts[] = {453,152,429,209,682,296,650,359,713,382,772,321,779,236,731,204,688,241};
	PolyToolArr[1] =  new CPolygon(9);
	PolyToolArr[1]->Init(Tool2Pts);
	int Tool3Pts[] = {553,76,631,54,651,348,601,354};
	PolyToolArr[2]=  new CPolygon(4);
	PolyToolArr[2]->Init(Tool3Pts);


	for (int i = 0; i < 3; i++)
	{
		int ArrayPts[] = {351,426,328,477,469,479,488,432,434,414};
		leftMiniBoxPolyToolArr[i] =  new CPolygon(5);
		leftMiniBoxPolyToolArr[i]->Init(ArrayPts);
	}


	isPaperCutSceneVideoPlaying = false;

	isNewsToJournalPlaying = false;
	newsPaperTakeDelayTimer = 1.0f;
	NewsPaperFlying = HOScene->GetObjectByID("burnedPaper");
	NewsPaperFlying->SetEnable(false);

	//Add Visible = true; to see anything
	SetZ(eZOrder_Popup + 50);

	cv_paperCutscn = NULL;
	skipObj = NULL;


	time = 0;
	subIndex = 0;
	subMax = 3;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("papercut1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("papercut2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("papercut3"));

	paperCutscnDelayTimer = 1.0f;

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperCutScenePlayed)
	{
		cv_paperCutscn = new CVideo("OGV\\L1S6\\PAPERCUTSCENE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//cv_paperCutscn->SetScale(1.334f);
		cv_paperCutscn->SetScale(GFApp->video_aspectRatio);
	}

	skipObj = new CObject();
	skipObj->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\SKIPBTN.SPR", (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y, eZOrder_CutsceneFull+10);
	skipObj->Show(false);

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	//After Escape From Tunnel
	if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
	{
		if( Control::Audio->IsPlaying(aTrackL1EscapePlan) )
			ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;	
	}
	else if( ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed &&
		!ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed )
	{
		if( Control::Audio->IsPlaying(aTrackL1Hunted) )
			ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;	
	}
	else
	{
		if( Control::Audio->IsPlaying(aTrackL1Mansion) )
			ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;	
	}

	Control::Audio->LoadSample(aTrackCutscnS6Newspaper,AudioTrack);

	Control::Audio->LoadSample(aAmbS6ShedRoom,AudioAmb);
	Control::Audio->PlaySample(aAmbS6ShedRoom,true);
	Control::Audio->LoadSample(aVOL1MaryRefS6_1,AudioVO);
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S6_HOBoatShed]  = true;

	IsVidSkippable = false;
	IsSkipMouseOver = false;
	IsSkipMouseClicked = false;
	SkipShowTimer = 0;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S6_HOBoatShed])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S6_HOBoatShed] = true;

	

	
	

	Visible = true;
}

void L1_S6_HOBoatShed::InitializeInventoryBox()
{
	const CRectangle  hoInventoryBoxRect(375.0f,645.0f,620.0f,139.0f);

	hoInventoryBox = new HOInventoryBox(hoInventoryBoxRect, 4);
	hoInventoryBox->SetPosition(CPoint(hoInventoryBox->GetPosition().x, hoInventoryBox->GetPosition().y, eZOrder_HUD));
	for (int i = 0; i < MAX_HO_OBJECTS; i++)
	{
		std::string str;

		CObject * collected = new CObject();
		str = std::string("LV1\\S6\\HO\\Inventory\\InvIconCollected_") + MKSTR(i) + std::string(".SPR");
		collected->LoadSprite(str);
		collected->SetZ(100);

		CObject * notCollected = new CObject();
		str = std::string("LV1\\S6\\HO\\Inventory\\InvIconNotCollected_") + MKSTR(i) + std::string(".SPR");
		notCollected->LoadSprite(str);
		notCollected->SetZ(100);
		std::vector<int> collectSFXIds;
		collectSFXIds.push_back(aSFXInvCollectv1);
		collectSFXIds.push_back(aSFXInvCollectv2);
		collectSFXIds.push_back(aSFXInvCollectv3);
		InventoryItem * invItem = new InventoryItem(notCollected, collected, i, CPoint(100,100, 100), this, "PRT\\L1\\HOTrailFX.INI", collectSFXIds, aSFXInvWrongDrop, aSFXInvItemPick);
		hoInventoryBox->AddInventoryItem(invItem);
		//invItem->SetPosition(CPoint(100,100,100));
	}

	if (ProfileSystem::Instance->ActiveProfile.L1S6_HOInvItemStates[0] == kInventoryItemState_NotLoaded)//checking first object is loaded
	{
		hoInventoryBox->LoadInventoryItemsToVisibleArea(0, 4);
	}
	else
	{
		hoInventoryBox->LoadInventoryItemStates(ProfileSystem::Instance->ActiveProfile.L1S6_HOInvItemStates);
	}
}
L1_S6_HOBoatShed::~L1_S6_HOBoatShed()
{

	Control::Audio->Samples[aTrackL1Mansion]->SetVolume((float)Control::Audio->GlobalMusicVol);
	Control::Audio->Samples[aTrackL1Hunted]->SetVolume((float)Control::Audio->GlobalMusicVol);
	Control::Audio->Samples[aTrackL1EscapePlan]->SetVolume((float)Control::Audio->GlobalMusicVol);

	Hud->InvBox_StopPuzzleMode();
	Hud->IsHOScene = false;
	Hud->IsTakeAndUseHOScene = false;

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(match3 != NULL)
		{
			match3->clearBoard();
			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			isMatch3Playing = false;
		}
	}


	SAFE_DELETE(PaperClipAnim);
	SAFE_DELETE(chestDoorVFX_0);
	SAFE_DELETE(chestDoorVFX_1);
	SAFE_DELETE(chestDoorVFX_2);
	SAFE_DELETE(rayLeftVFX);
	SAFE_DELETE(rayRightVFX);
	SAFE_DELETE(hoInventoryBox);

	SAFE_DELETE(ResetDia);

	SAFE_DELETE(SmallBoxDoorOpenAnim);
	SAFE_DELETE(cAnimCloth);
	SAFE_DELETE(cAnimOilDipping);
	SAFE_DELETE(cAnimStick);
	SAFE_DELETE(cAnimTapeRoll);
	SAFE_DELETE(cAnimTapeCut);
	SAFE_DELETE(cAnimMagnet);


	SAFE_DELETE(PolyToolArr[0]);
	SAFE_DELETE(PolyToolArr[1]);
	SAFE_DELETE(PolyToolArr[2]);

	SAFE_DELETE(leftMiniBoxPolyToolArr[0]);
	SAFE_DELETE(leftMiniBoxPolyToolArr[1]);
	SAFE_DELETE(leftMiniBoxPolyToolArr[2]);

	SAFE_DELETE(pointerInHand);

	SAFE_DELETE(subtitleText);

	SAFE_DELETE(squirrelAngryAnim);
	for (int i = 0; i < 2; i++)
	{
		SAFE_DELETE(SquirrelRunAnim[i]);
	}

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	SAFE_DELETE(cv_paperCutscn);
	SAFE_DELETE(skipObj);

	if( Control::Audio->IsPlaying(aTrackL1Puzzle) )
		Control::Audio->StopSample(aTrackL1Puzzle);
	//MUSIC_SFX
	Control::Audio->UnloadSample(aTrackCutscnS6Newspaper);

	Control::Audio->StopSample(aAmbS6ShedRoom);
	Control::Audio->UnloadSample(aAmbS6ShedRoom);
	Control::Audio->UnloadSample(aVOL1MaryRefS6_1);
	//MUSIC_SFX
}
void L1_S6_HOBoatShed::ForceCompleteHO()
{
	ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOFunnelPiecesFixed = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel = true;
	ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeCutOnFunnel = true;
	for (int i = 0; i < 3; i++)
	{
		ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolMoved[i] = true;	
		ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolInLeftMiniBoxMoved[i] = true;	
	}

	toolPointer = 0;
	leftMiniBoxToolPointer = 0;
	for (int i = 0; i < 3; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolMoved[i])
		{
			moveableTools[i]->Show(false);
			toolPointer++;

		}

		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolInLeftMiniBoxMoved[i])
		{
			leftMiniBoxMovableTools[i]->Show(false);
			leftMiniBoxToolPointer++;
		}
	}

	closedBox->Show(false);
	magnet2->Show(false);
	paperclip->Show(false);
	paperclip2->Show(false);
	magnet->Show(false);
	cloth->Show(false);
	rust1->Show(false);
	stick->Show(false);
	beettle->Show(false);
	squirrel->Show(false);
	acon->Show(false);

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeCutOnFunnel)
	{
		funnelHead->Show(false);
		funnelNose->Show(false);
		funnelFull->Show(false);
		funnelFullWithTape->Show(false);
	}

	Hud->InvBox_StopPuzzleMode();

	UpdateInsideBoxProperties();
}

void L1_S6_HOBoatShed::UpdateInsideBoxProperties()
{
	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel)
	{
		funnelHead->Show(false);
		funnelNose->Show(false);
		funnelFull->Show(false);
		funnelFullWithTapeCut->Show(false);
		funnelFullWithTape->Show(true);
	}
	else
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOFunnelPiecesFixed)
		{
			funnelHead->Show(false);
			funnelNose->Show(false);
			funnelFull->Show(true);
			funnelFullWithTape->Show(false);
			funnelFullWithTapeCut->Show(false);
		}
		else
		{
			funnelFull->Show(false);
			funnelFullWithTape->Show(false);
			funnelFullWithTapeCut->Show(false);
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeCutOnFunnel)
	{
		funnelHead->Show(false);
		funnelNose->Show(false);
		funnelFull->Show(false);
		funnelFullWithTapeCut->Show(true);
		funnelFullWithTape->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened)
	{
		smallBoxDoorClosed->Show(false);
		glove->Show(false);
		rustOnSmallBox->Show(false);
		oilDripAnim->Show(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel)
		{
			tape->Show(false);
		}
	}
	else
	{
		smallBoxDoorOpen->Show(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved)
		{
			rustOnSmallBox->Show(false);
			oilDripAnim->Show(false);
		}
	}
}

void L1_S6_HOBoatShed::Required()
{

	HiddenObjectSystem::Required();	//forced update

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperCutScenePlayed)
	{

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
		else if( Control::Audio->IsPlaying(aTrackL1Hunted) )
		{
			Control::Audio->Samples[aTrackL1Hunted]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if(Control::Audio->GlobalMusicVol - musicFadeOutTimer<=0)
			{
				Control::Audio->Samples[aTrackL1Hunted]->SetVolume((float)Control::Audio->GlobalMusicVol);
				Control::Audio->StopSample(aTrackL1Hunted);
				musicFadeOutTimer = 0;
				if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
					Control::Audio->PlaySample(aTrackL1Puzzle, true);
			}
			musicFadeOutTimer += Control::LogicRate * 8;
		}
		else if( Control::Audio->IsPlaying(aTrackL1EscapePlan) )
		{
			Control::Audio->Samples[aTrackL1EscapePlan]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if(Control::Audio->GlobalMusicVol - musicFadeOutTimer<=0)
			{
				Control::Audio->Samples[aTrackL1EscapePlan]->SetVolume((float)Control::Audio->GlobalMusicVol);
				Control::Audio->StopSample(aTrackL1EscapePlan);
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
	}

	if( Hud->PopupState != Popup_Inactive )
		return;
}

float L1_S6_HOBoatShed::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L1_S6_HOBoatShed::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive || Hud->IsMapShowing)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();

	Cursor::SetMode(CA_Normal);

	if(!txtMatch3->Visible)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	} else{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		txtMatch3->Show(false);
		txtPuzzle->Show(false);
		txtMatch3MouseOver->Show(false);
		txtPuzzleMouseOver->Show(false);
	}

	//ResetHO();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
	{
		if(Hud->Tutor->State == TUT21_Match3Tutorial && Hud->Tutor->IsVisible)
		{
			return;
		}
	}

	if(isNewsToJournalPlaying)
	{
		newsPaperTakeDelayTimer -= Control::LogicRate;
		if(newsPaperTakeDelayTimer<0)
		{
			newsPaperTakeDelayTimer = 100.0f;
			float valArr[5] = {0.15f, 305, 632, 0.25f, 60.0f};
			NewsPaperFlying->TweenTo(valArr, 1.0f, TW_EASE, false);

			//Tutorial show
			if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
			{
				//puzzle is activated only when fuses are fixed
				if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT21_Match3Tutorial])
					Hud->Tutor->ShowTutorial(TUT21_Match3Tutorial);
			}
		}

		if(NewsPaperFlying->IsTweening && NewsPaperFlying->IsTweenFinished())
		{
			NewsPaperFlying->IsTweening = false;
			NewsPaperFlying->SetEnable(false);			

			//Journal Entry 16 Darkarta newspaper
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P16_DakartaNewspaper])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P16_DakartaNewspaper] = true;
				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
				= eJrn_L1P16_DakartaNewspaper;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}
			ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;
			isNewsToJournalPlaying = false;
		}
		return;
	}

	if(isPaperCutSceneVideoPlaying)
	{
		vidTimer += Control::LogicRate;

		if( IsVidSkippable )
		{
			if(SkipShowTimer>2.0f)
			{
				if(!skipObj->Visible)
					skipObj->Show(true);

				CPoint mousePos = Control::Input->Mpos();
				IsSkipMouseOver = ( mousePos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) && 
					mousePos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
					mousePos.y >= SKIPIMAGE_Y &&
					mousePos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) + SKIPIMAGE_WIDTH) );

				if( IsSkipMouseOver && !IsSkipMouseClicked)  
				{
					Cursor::SetMode(CA_HandPointy);
					skipObj->PlayAnimation(1);
					IsSkipMouseClicked = Control::Input->LBclicked();					
					if( IsSkipMouseClicked )
					{
						Control::Audio->PlaySample(aSFXClick);
						Control::Input->ClearMouse();
						AwardManager::GetInstance()->SkipDialogueOrCutScene();
					}
				}
				else //Cursor not over skip region
				{
					skipObj->PlayAnimation(0);
				}

				//Subtitles Code
				time = cv_paperCutscn->GetTime();

				if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
				{
					subtitleText->SetText(subtitles[subIndex].text);
				}
				else
					subtitleText->SetText("");

				if (time > subtitles[subIndex].end)
				{
					if (subMax - 1 > subIndex)
						subIndex++;
				}

				if(
#ifndef FORCE_CLOSE_VID_END
					cv_paperCutscn->isEndReached() ||
#endif
					IsSkipMouseClicked || vidTimer > 26)
				{
					subtitleText->SetText("");
					cv_paperCutscn->StopAndFadeOutVideo(2.0f);					
					isPaperCutSceneVideoPlaying = false;
					Hud->AllowInput = true;
					Hud->ShowHud(false);
					Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
					if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
						Control::Audio->PlaySample(aTrackL1Puzzle, true);

					if( !Control::Audio->IsPlaying(aAmbS6ShedRoom) )
						Control::Audio->PlaySample(aAmbS6ShedRoom,true);

					//Control::Audio->StopSample(aTrackCutscnS6Newspaper);

					skipObj->Show(false);

					ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperCutScenePlayed = true;
					txtMatch3->Show(true);
					paper->Show(false);

					isNewsToJournalPlaying = true;
					Control::Audio->QuickLoadAndPlaySFX(aSFXJrnlFlyer);
					NewsPaperFlying->SetEnable(true);

					Hud->ShowBannerText("L1nagin");
					Control::Audio->PlaySample(aVOL1MaryRefS6_1);
					return;
				}
			}
			else
				SkipShowTimer += 10*Control::LogicRate;
		}
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{

#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (!isObjectInHand && Control::Input->isKeyBackSensed())
		{
				match3->clearBoard();
				txtMatch3->Show(false);
				txtPuzzle->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_BoatShed;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
				return;		
		}
#endif

if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
{
    if( Control::Input->LBclicked() )
    {
        match3->clearBoard();
        txtMatch3->Show(false);
        txtPuzzle->Show(false);
        ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

        Control::Audio->PlayFootStepsFX();
        ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_BoatShed;
        SendMessage(ControlExit, ID, HOSM_NewArea);
    }
}

		if(!isObjectInHand && IsMouseOverGoBackArea(&pos )

#ifdef M3_BUTTON
		&& txtMatch3->Visible
#endif
		)
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S6_BoatShed);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S6_BoatShed);
#endif
			if( Control::Input->LBclicked() )
			{

#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S6_BoatShed, 2);
				}
#else
				match3->clearBoard();
				txtMatch3->Show(false);
				txtPuzzle->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_BoatShed;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				return;
			}			
		}

		if(areaRectMatch3.Intersection(&pos) && !match3->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			txtPuzzleMouseOver->Show(true);
			if( Control::Input->LBclicked() )
			{
				hoInventoryBox->SetisEnabled(true);

				match3->clearBoard();
				txtMatch3->Show(true);
				txtPuzzle->Show(false);
				txtPuzzleMouseOver->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				UseObjects->SetEnable(true);
				return;
			}
		}
		else
		{
			txtPuzzleMouseOver->Show(false);
		}
		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved && ProfileSystem::Instance->ActiveProfile.L1S6_HOCompletedWithMatch3)
	{
		ForceCompleteHO();
		UpdateInsideBoxProperties();

		txtMatch3->Show(false);
		txtMatch3MouseOver->Show(false);
		
		match3Back->Show(false);
		match3BackText->Show(false);

		ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved = true;

		if(_handleExtrasGameComplete(false))
			return;

		TakeInventoryItem(eInv_L1S6_Funnel);

		
		Hud->InvBox_StopPuzzleMode();
		Hud->IsHOScene = false;
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;

		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOInvFunnelTaken)
		{
			AwardManager::GetInstance()->CompleteHO(eArea_L1_S6_HOBoatShed);
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_BoatShed;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
		return;
	}

	Cursor::SetMode(CA_Normal);

	if(isBeetleAnimPlaying)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved)
		{
			if(beettle->IsAnimationFinished())
			{
				isBeetleAnimPlaying = false;
				beetleTimer = 0.0f;
				beetleTime = RandomRange(2,5);
			}
		}
		else
		{
			isBeetleAnimPlaying = false;
		}

	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved && !isBeetleAnimPlaying && !isStickAnimationPlaying)
	{
		beetleTimer += Control::LogicRate;
		if(beetleTimer >= beetleTime)
		{
			beettle->ForcePlayAnimation(0);
			isBeetleAnimPlaying = true;
		}
	}

	if (!ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved )
	{
		squirrelTimer += Control::LogicRate * 1.0f; 
		if( squirrel->pSprite->GetFrame() > 20 )
		{
			if( squirrelTimer >= squirrelThreshold )
			{
				squirrelThreshold = ( 2.0f + std::rand()%3 );
				squirrelTimer = 0.0f;
				squirrel->Paused = !squirrel->Paused;
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperCutScenePlayed && !isPaperCutSceneVideoPlaying)
	{
		if(isToolMoving)
		{		
			movingTimer += Control::LogicRate;
			CPoint temp;

			temp.x = toolStartPos.x + (toolEndPos.x - toolStartPos.x) * easing(movingTimer, 0, 1, moveTime);
			temp.y = toolStartPos.y + (toolEndPos.y - toolStartPos.y) * easing(movingTimer, 0, 1, moveTime);

			moveableTools[toolPointer]->Pos = temp;

			if(movingTimer >= moveTime)
			{
				moveableTools[toolPointer]->FadeOut(4.0f);
				isToolMoving = false;
				ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolMoved[toolPointer] = true;
				toolPointer++;
				if(toolPointer == 3)
				{
					isAllToolsMoved = true;
				}
			}

			return;
		}

		if(isToolMovingInLeftMiniBox)
		{	

			leftMiniBoxToolMovingTimer += Control::LogicRate;
			CPoint temp;

			temp.x = leftMiniBoxToolStartPos.x + (leftMiniBoxToolEndPos.x - leftMiniBoxToolStartPos.x) * easing(leftMiniBoxToolMovingTimer, 0, 1, leftMiniBoxToolMoveTime);
			temp.y = leftMiniBoxToolStartPos.y + (leftMiniBoxToolEndPos.y - leftMiniBoxToolStartPos.y) * easing(leftMiniBoxToolMovingTimer, 0, 1, leftMiniBoxToolMoveTime);

			leftMiniBoxMovableTools[leftMiniBoxToolPointer]->Pos = temp;

			if(leftMiniBoxToolMovingTimer >= leftMiniBoxToolMoveTime)
			{
				leftMiniBoxMovableTools[leftMiniBoxToolPointer]->FadeOut(4.0f);
				isToolMovingInLeftMiniBox = false;
				ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolInLeftMiniBoxMoved[leftMiniBoxToolPointer] = true;
				leftMiniBoxToolPointer++;
				if(leftMiniBoxToolPointer == 3)
				{
					isAllToolsInLeftMiniBoxMoved = true;
				}
			}

			return;
		}

		if(isOilDrippingAnimPlaying)
		{
			oilDripAnimTimer++;
			if (!oilDripAnim->IsActive && oilDripAnimTimer > 80)
			{
				rustOnSmallBox->Show(false);
				//oilDripAnim->Show(true);
				oilDripAnim->SetEnable(true);
				oilDripAnim->ForcePlayAnimation(0);
				oilDripAnim->ZOrder = rustOnSmallBox->ZOrder + 1;
			}
			if(cAnimOilDipping->IsAnimFinished())
			{
				isOilDrippingAnimPlaying = false;
				oilDripAnim->SetEnable(false);
				oilDipperOnHand->Show(false);
				//getObject[7]->FadeIn(4.0f);
			}
			return;
		}

		if(isPaperClipAnimPlaying)
		{
			if(PaperClipAnim->IsAnimationFinished())
			{
				isPaperClipAnimPlaying = false;
				PaperClipAnim->Show(false);
				HOScene->GetObjectByID("lache")->Show(false);
				HOScene->GetObjectByID("lacheOpen")->Show(true);
			}
			return;
		}

		if(isMagnetAnimating)
		{
			if(cAnimMagnet->IsAnimFinished())
			{
				isMagnetAnimating = false;
				magnet2->Show(true);
				paperclip->Show(false);
				paperclip2->Show(true);
				stick->ZOrder = 41;
			}
			return;
		}

		if(isClothAnimationPlaying)
		{
			if(cAnimCloth->IsAnimFinished())
			{
				isClothAnimationPlaying = false;
				clothOnHand->Show(false);
				rust1->Show(false);
			}
			return;
		}

		if(isSquirrelRunAnimationPlaying)
		{
			if(SquirrelRunAnim[squirrelRunAnimPointer]->IsAnimationFinished())
			{
				if(squirrelRunAnimPointer == 2)
				{
					isSquirrelRunAnimationPlaying = false;
					SquirrelRunAnim[squirrelRunAnimPointer]->Show(false);
				}
				else
				{
					squirrelRunAnimPointer++;
					SquirrelRunAnim[squirrelRunAnimPointer - 1]->Show(false);
					SquirrelRunAnim[squirrelRunAnimPointer]->Show(true);
					SquirrelRunAnim[squirrelRunAnimPointer]->ForcePlayAnimation(0);
				}
			}
			return;
		}

		if(isStickAnimationPlaying)
		{
			if(cAnimStick->IsAnimFinished())
			{
				stick->Show(false);
				beetleAnimObject->Show(false);
				isStickAnimationPlaying = false;
			}
			return;
		}

		if(isTapeRollingAnimPlaying)
		{
			if(cAnimTapeRoll->IsAnimFinished())
			{
				isTapeRollingAnimPlaying = false;
				tapeRollAnimObj1->Show(false);
				tapeRollAnimObj2->Show(false);
				tapeRollAnimObj3->Show(false);

				funnelFullWithTape->Show(true);
				ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel = true;
				UpdateInsideBoxProperties();
			}
			return;
		}

		if(isTapeCuttingAnimPlaying)
		{
			if(cAnimTapeCut->IsAnimFinished())
			{
				isTapeRollingAnimPlaying = false;
				for (int i = 0; i < 5; i++)
				{
					tapeCutAnimObjects[i]->Show(false);
				}
				funnelFullWithTapeCut->SetEnable(false);
				funnelFullWithTape->SetEnable(false);

				ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved = true;
				UpdateInsideBoxProperties();

				if(_handleExtrasGameComplete(false))
					return;

				TakeInventoryItem(eInv_L1S6_Funnel);

				Hud->InvBox_StopPuzzleMode();
				Hud->IsHOScene = false;
				//DisableText();
			}
			return;
		}

		if(isSmallBoxDoorOpenAnimPlaying)
		{
			if(SmallBoxDoorOpenAnim->IsAnimationFinished())
			{
				isSmallBoxDoorOpenAnimPlaying = false;
				SmallBoxDoorOpenAnim->Show(false);
				smallBoxDoorOpen->Show(true);
				ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened = true;
				//				txtGlove->SetStrikethrough(true);
				//				UpdateText();
			}
			return;
		}

		if(isLeftMiniBoxDoorOpenAnimPlaying)
		{
			if(cAnimLeftMiniBoxOpen->IsAnimFinished())
			{
				isLeftMiniBoxDoorOpenAnimPlaying = false;
				openedLeftMiniDoor->SetAlpha(1.0f);
				openedLeftMiniDoor->Show(true);
				closedLeftMiniDoor->Show(false);
				handleOnMiniDoor->Show(false);
				leftMiniBoxOpenAnimObject1->Show(false);
				leftMiniBoxOpenAnimObject2->Show(false);
				leftMiniBoxOpenAnimObject3->Show(false);

				ProfileSystem::Instance->ActiveProfile.L1S6_HOLeftMiniDoorOpened = true;
			}
			return;
		}

		if( !ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened && ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved)
		{
			if(PolyBoxDoorArea->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed )
					{
						closedBox->Shake(90.0f,2.0f,0,0.25f);
						HOScene->GetObjectByID("lache")->Shake(90.0f,2.0f,0,0.25f);
						Hud->ShowBannerText("L1lock",SFX_NIL);
					}
					else
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOOpenBox);

						UpdateInsideBoxProperties();
						ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened = true;
						closedBox->FadeOut(4.0f);
						HOScene->GetObjectByID("lacheOpen")->FadeOut(4.0f);
						boxDoorOpen->FadeIn(4.0f);
						
						
					}
					chestDoorVFX_0->Start();
					chestDoorVFX_1->Start();
					chestDoorVFX_2->Start();
					Control::Audio->PlaySample(aSFXWoodShake);
					return;
				}
			}
		}

		if (!isMouseAlreadyHandled)
		{
			if(isObjectInHand)
			{	
				HandleMouseForPlaceItem();
			}  

			else
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved)
				{
					if(polyAconSet->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked() && !isSquirrelAngryAnimPlaying )
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOSquirrelClicked);
							Hud->ShowBannerText("L1squirrel");
							//when clicked on squirrel
							isSquirrelAngryAnimPlaying = true;
							squirrel->Show(false);
							squirrelAngryAnim->Show(true);
							squirrelAngryAnim->ForcePlayAnimation(0);
						}
					}
				}

				if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved)
				{
					if(polyClothGet->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOBeetleClicked);
							Hud->ShowBannerText("L1rid");
						}
					}
				}

				if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken && !isStickAnimationPlaying)
				{
					if(setPoly[kInv_Magnet]->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1paperclip");
						}
					}
				}

				if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved)
				{
					if(polyClothSet->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1dirt");
						}
					}
				}

				if(ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved && !ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed)
				{
					if(polyPaperClipSet->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1safety");
						}
					}
				}

				if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved && ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed)
				{
					if(PolyBoxDoorArea->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1chest");
						}
					}
				}

				if(ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved)
				{
					if(polyGloveSet->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1hinges");
						}
					}
				}

				if(ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened && ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened)
				{
					if(polyGloveSet->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1prec");
						}
					}
				}

				if(ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened && ProfileSystem::Instance->ActiveProfile.L1S6_HOFunnelPiecesFixed && !ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel)
				{
					if(setPoly[kInv_Tape]->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1stick");
						}
					}

				}

				if(ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened && ProfileSystem::Instance->ActiveProfile.L1S6_HOFunnelPiecesFixed && ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel && !ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeCutOnFunnel)
				{
					if(setPoly[kInv_Tape]->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1tape");
						}
					}
				}

				if (ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened && hoInventoryBox->GetInventoryItemStateOf(kInv_Handle) <= kInventoryItemState_Collected && !ProfileSystem::Instance->ActiveProfile.L1S6_HOLeftMiniDoorOpened)
				{
					if(setPoly[kInv_Handle]->Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1box");
						}
					}
				}
				else
				{
					if (!isLeftMiniBoxDoorOpenAnimPlaying && !ProfileSystem::Instance->ActiveProfile.L1S6_HOLeftMiniDoorOpened && setPoly[kInv_Handle]->Intersection(&pos) && hoInventoryBox->GetInventoryItemStateOf(kInv_Handle) == kInventoryItemState_UnloadCompleted)
					{
						if( Control::Input->LBclicked())
						{
							closedLeftMiniDoor->Show(false);
							handleOnMiniDoor->Show(false);
							
							leftMiniBoxOpenAnimObject1->Show(true);
							leftMiniBoxOpenAnimObject2->Show(true);
							leftMiniBoxOpenAnimObject3->Show(true);
							openedLeftMiniDoor->Show(true);

							cAnimLeftMiniBoxOpen->PlayAnim();
							isLeftMiniBoxDoorOpenAnimPlaying = true;
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOGloveUsed); 
						}
						
					}
				}
				HandleMouseForCollectItem();


			} 


			if(ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened) //sree
			{

				if(!isAllToolsMoved)
				{
					if(PolyToolArr[toolPointer]->Intersection(&pos))
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOSlideObject);

							toolStartPos = moveableTools[toolPointer]->Pos;
							if(toolPointer == 2)
							{
								toolEndPos.x = moveableTools[toolPointer]->Pos.x + 100;
								toolEndPos.y = moveableTools[toolPointer]->Pos.y;
							}
							else
							{
								toolEndPos.x = moveableTools[toolPointer]->Pos.x - 100;
								toolEndPos.y = moveableTools[toolPointer]->Pos.y + 50;
							}

							isToolMoving = true;
							movingTimer = 0.0f;
							moveTime = 0.50f;
							return;
						}
					}
				}
				if(!isAllToolsInLeftMiniBoxMoved && ProfileSystem::Instance->ActiveProfile.L1S6_HOLeftMiniDoorOpened)
				{
					if(leftMiniBoxPolyToolArr[leftMiniBoxToolPointer]->Intersection(&pos))
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOSlideObject);

							leftMiniBoxToolStartPos = leftMiniBoxMovableTools[leftMiniBoxToolPointer]->Pos;
							if(leftMiniBoxToolPointer == 2)
							{
								leftMiniBoxToolEndPos.x = leftMiniBoxMovableTools[leftMiniBoxToolPointer]->Pos.x + 100;
								leftMiniBoxToolEndPos.y = leftMiniBoxMovableTools[leftMiniBoxToolPointer]->Pos.y - 50;
							}
							else
							{
								leftMiniBoxToolEndPos.x = leftMiniBoxMovableTools[leftMiniBoxToolPointer]->Pos.x;
								leftMiniBoxToolEndPos.y = leftMiniBoxMovableTools[leftMiniBoxToolPointer]->Pos.y - 50;
							}

							isToolMovingInLeftMiniBox = true;
							leftMiniBoxToolMovingTimer = 0.0f;
							leftMiniBoxToolMoveTime = 0.50f;
							return;
						}
					}
				}
			}
		}
		else
		{
			isMouseAlreadyHandled = false;
		}


	}
	else
	{
		//if(polyPaper->Intersection(&pos) && !isPaperCutSceneVideoPlaying) //raj
		//{
		//	Cursor::SetMode(CA_Look);
		//	if( Control::Input->LBclicked() )
		//	{
		//		Play cutScene videeo
		//		isPaperCutSceneVideoPlaying = true;
		//		txtMatch3->Show(false);
		//		return;
		//	}
		//}

		if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperCutScenePlayed)
		{
			if(paperCutscnDelayTimer<0)
			{
				isPaperCutSceneVideoPlaying = true;
				cv_paperCutscn->PlayFadeInVideo(2.0f);
				Hud->EnableInput(false);
				Hud->HideHud(false);

				//After Escape From Tunnel
				if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
				{
					if( Control::Audio->IsPlaying(aTrackL1EscapePlan) )
						Control::Audio->StopSample(aTrackL1EscapePlan);
				}
				else if( ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed &&
					!ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed )
				{
					if( Control::Audio->IsPlaying(aTrackL1Hunted) )
						Control::Audio->StopSample(aTrackL1Hunted);
				}
				else
				{
					if( Control::Audio->IsPlaying(aTrackL1Mansion) )
						Control::Audio->StopSample(aTrackL1Mansion);
				}

				if( Control::Audio->IsPlaying(aAmbS6ShedRoom) )
					Control::Audio->StopSample(aAmbS6ShedRoom);

				//Control::Audio->PlaySample(aTrackCutscnS6Newspaper,false);
				//MUSIC_SFX

				IsVidSkippable = true;
				txtMatch3->Show(false);
			}
			else
				paperCutscnDelayTimer -= Control::LogicRate;
			return;
		}
		/*if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperCutScenePlayed)
		{
		isPaperCutSceneVideoPlaying = true;
		cv_paperCutscn->PlayFadeInVideo(2.0f);

		IsVidSkippable = true;
		txtMatch3->Show(false);
		}*/
	}

#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (!isObjectInHand && Control::Input->isKeyBackSensed())
	{
		//No other animations playing!!
		if( !isSquirrelRunAnimationPlaying &&
			!isClothAnimationPlaying  &&
			!isStickAnimationPlaying &&
			!isMagnetAnimating  &&
			!isPaperClipAnimPlaying  &&
			!isOilDrippingAnimPlaying  &&
			!isSmallBoxDoorOpenAnimPlaying  &&
			!isTapeRollingAnimPlaying  &&
			!isTapeCuttingAnimPlaying )
		{
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_BoatShed;
				SendMessage(ControlExit, ID, HOSM_NewArea);
				return;		
		}
	}
#endif

	if(!isObjectInHand && IsMouseOverGoBackArea(&pos))
	{
		//No other animations playing!!
		if( !isSquirrelRunAnimationPlaying &&
			!isClothAnimationPlaying  &&
			!isStickAnimationPlaying &&
			!isMagnetAnimating  &&
			!isPaperClipAnimPlaying  &&
			!isOilDrippingAnimPlaying  &&
			!isSmallBoxDoorOpenAnimPlaying  &&
			!isTapeRollingAnimPlaying  &&
			!isTapeCuttingAnimPlaying )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S6_BoatShed);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S6_BoatShed);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S6_BoatShed, 2);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_BoatShed;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				return;
			}			
		}
	}

	if(areaRectMatch3.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved && !isPaperCutSceneVideoPlaying)
	{
		Cursor::SetMode(CA_HandPointy); //sree
		txtMatch3MouseOver->Show(true);
		if( Control::Input->LBclicked() )
		{
			hoInventoryBox->SetisEnabled(false);

			if(match3 == NULL)
			{
				match3 = new Match3(1);
				AddControl(match3);
			}
			else
			{
				match3->ShowBoard();
			}

			txtMatch3->Show(false);
			txtMatch3MouseOver->Show(false);
			txtPuzzle->Show(true);
			flagForText = true;
			isMatch3Playing = true;

			Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
			UseObjects->SetEnable(false);

			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;
			return;
		}
	}
	else
	{
		txtMatch3MouseOver->Show(false);
	}

#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (!isObjectInHand && Control::Input->isKeyBackSensed())
	{
		//No other animations playing!!
		if( !isSquirrelRunAnimationPlaying &&
			!isClothAnimationPlaying  &&
			!isStickAnimationPlaying &&
			!isMagnetAnimating  &&
			!isPaperClipAnimPlaying  &&
			!isOilDrippingAnimPlaying  &&
			!isSmallBoxDoorOpenAnimPlaying  &&
			!isTapeRollingAnimPlaying  &&
			!isTapeCuttingAnimPlaying )
		{
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_BoatShed;
				SendMessage(ControlExit, ID, HOSM_NewArea);
				return;		
		}
	}
#endif
	if(!isObjectInHand && IsMouseOverGoBackArea(&pos))
	{
		//No other animations playing!!
		if( !isSquirrelRunAnimationPlaying &&
			!isClothAnimationPlaying  &&
			!isStickAnimationPlaying &&
			!isMagnetAnimating  &&
			!isPaperClipAnimPlaying  &&
			!isOilDrippingAnimPlaying  &&
			!isSmallBoxDoorOpenAnimPlaying  &&
			!isTapeRollingAnimPlaying  &&
			!isTapeCuttingAnimPlaying )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S6_BoatShed);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S6_BoatShed);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S6_BoatShed, 2);
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

	if( isSquirrelAngryAnimPlaying )
	{
		if( squirrelAngryAnim->IsAnimationFinished() )
		{
			squirrel->Show(true);
			squirrelAngryAnim->Show(false);
			squirrel->ForcePlayAnimation(0);
			isSquirrelAngryAnimPlaying = false;
		}
	}

	hoInventoryBox->HandleMouseInput(Control::Input);

}

float L1_S6_HOBoatShed::RandomRange(int min, int max)
{
	int r = max * rand();
	return (float)(r % (max + 1 - min) + min);
}

void L1_S6_HOBoatShed::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{		
	case eInv_L1S6_OilDipper:
		Hud->InvBoxCtrl->Activate(1);			
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_OilDipper] = true;
		ProfileSystem::Instance->ActiveProfile.L1S6_HOInvOilDripperTaken = true;
		vecInvIds.push_back(eInv_L1S6_OilDipper);
		Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
		break;
	case eInv_L1S6_Funnel:
		Hud->InvBoxCtrl->Activate(1);			
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_Funnel] = true;
		ProfileSystem::Instance->ActiveProfile.L1S6_HOInvFunnelTaken = true;
		vecInvIds.push_back(eInv_L1S6_Funnel);
		Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
		break;
	default:
		break;
	}
}

void L1_S6_HOBoatShed::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

}

void L1_S6_HOBoatShed::OnInventoryItemClick(int invItemType)
{
	objectID = invItemType;
	isObjectInHand = true;
	isMouseAlreadyHandled = true;

	pointerInHand = new CObject();
	pointerInHand->LoadSprite(mousePointerString[invItemType]);
	getObject[invItemType]->Show(false);
	pointerInHand->Pos = Control::Input->Mpos();

	pointerInHand->Show(true);
	pointerInHand->SetZ(eZOrder_HUD + 1);



}

void L1_S6_HOBoatShed::ReceiveMessage(int val)
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
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			//No drop items in this scene and hence!!
			GFInstance->WrongInvDropMsg(Hud);
			return;
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L1_S6_HOBoatShed::ProcessNonHOHint()
{
	if(isNewsToJournalPlaying || isPaperCutSceneVideoPlaying)
		return;

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		return;

	AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);

	int iNonHOHint = 0; 
	int MaxNonHOHints = 1;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		
		
		case 0: ++iNonHOHint;
			for(int i = 0; i<= kInv_Blade; i++)
			{
				if(  hoInventoryBox->GetInventoryItemStateOf(i) == kInventoryItemState_Collected)
				{
					InitHintTrailEmit(setPoly[i]->center.x,setPoly[i]->center.y);
					CPoint tempPoint (hoInventoryBox->GetInventoryItem(i)->GetX(),hoInventoryBox->GetInventoryItem(i)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				else if( hoInventoryBox->GetInventoryItemStateOf(i) == kInventoryItemState_NotCollected)
				{
					InitHintTrailEmit(getPoly[i]->center.x,getPoly[i]->center.y);
					return;
				}

			}
			break;

		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L1_S6_HOBoatShed::UpdateInventoryChangesToProfile(int itemId)
{
	hoInventoryBox->UpdateInventoryChangesToProfile(ProfileSystem::Instance->ActiveProfile.L1S6_HOInvItemStates, itemId);
}

void L1_S6_HOBoatShed::HandleMouseForCollectItem()
{
	CPoint pos = Control::Input->Mpos();
	bool isRightClick = false;
	for (int i = 0; i < MAX_HO_OBJECTS; i++)
	{

		if ( hoInventoryBox->GetInventoryItem(i)->GetItemState() == kInventoryItemState_NotCollected)
		{
			switch (i)
			{
			case kInv_Acorn:
				if(getPoly[i]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved )
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}
				break;
			case kInv_Cloth:
				if(getPoly[i]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved && ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved)
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}
				break;
			case kInv_Stick:
				if(getPoly[i]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved)
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}
				break;
			case kInv_Magnet:
				if(getPoly[i]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken )
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}
				break;
			case kInv_PaperClip:
				if(getPoly[i]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened && ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken)
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						magnet2->FadeOut();
						break;
					}
				}
				break;
			case kInv_FunnelNose:
				if(getPoly[i]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOFunnelPiecesFixed && ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened)
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}
				break;
			case kInv_Handle:
				if(getPoly[i]->Intersection(&pos) && hoInventoryBox->GetInventoryItemStateOf(kInv_Handle) < kInventoryItemState_Collected && ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened)
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}
				break;
			case kInv_OilDripper:
				if(getPoly[i]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved)
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}
				break;
			case kInv_Glove:
				if(getPoly[i]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened && isAllToolsMoved)
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}
				break;
			case kInv_Tape:
				if(getPoly[i]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened && !ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel)
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}
				break;
			case kInv_Blade:
				if(getPoly[i]->Intersection(&pos) && isAllToolsInLeftMiniBoxMoved)
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						isRightClick = true;
						objectID = i;
						break;
					}
				}	
				break;

			default:
				break;
			}

			if(isRightClick)
			{
				//isObjectInHand = true;
				AwardManager::GetInstance()->StartMainStory();
				getObject[objectID]->Show(false);
				hoInventoryBox->CollectItem(HOInventoryItemIndex(i), getObject[objectID]->Pos);
				return;
			} 
		}

	}	  
}

void L1_S6_HOBoatShed::HandleMouseForPlaceItem()
{
	CPoint pos = Control::Input->Mpos();

	pointerInHand->SetPos(&pos);

	if(  (Control::Input->LBclicked() && hoInventoryBox->GetIsDraggingByMouseClickLock())   ||  (hoInventoryBox->GetIsDraggingByMouseDown() && !Control::Input->LBclicked() && !Control::Input->LBdown())  )
	{
#ifdef TOUCH_UI
		if(hoInventoryBox->GetIsDraggingByMouseClickLock())
		{
			pos = Control::Input->Mpos();
		}
#endif
		switch (objectID)
		{
		case kInv_Acorn:
			if(setPoly[kInv_Acorn]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved && !isSquirrelAngryAnimPlaying)
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_Acorn);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				setObject[kInv_Acorn]->Show(false);
				pointerInHand->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved = true;
				//txtAcorn->SetStrikethrough(true);
				SquirrelRunAnim[squirrelRunAnimPointer]->Show(true);
				SquirrelRunAnim[squirrelRunAnimPointer]->ForcePlayAnimation(0);
				isSquirrelRunAnimationPlaying = true;
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOSquirrelEat);
				
				
				return;
			}
			break;
		case kInv_Cloth:
			if(setPoly[kInv_Cloth]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved)
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_Cloth);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				rust1->Show(true);

				cAnimCloth->PlayAnim();
				clothOnHand->Show(true);
				pointerInHand->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved = true;
				isClothAnimationPlaying = true;
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOClothRub);
				
				
				return;
			}
			break;
		case kInv_Stick:
			if(setPoly[kInv_Stick]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved)
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_Stick);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				setObject[kInv_Stick]->Show(false);
				pointerInHand->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved = true;
				//txtStick->SetStrikethrough(true);
				stick->Show(true);
				beetleAnimObject->Show(true);
				cAnimStick->PlayAnim();
				isStickAnimationPlaying = true;
				isBeetleAnimPlaying = false;
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOBeetlePicked);
				
				
				return;
			}
			break;
		case kInv_Magnet:
			if(setPoly[kInv_Magnet]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken)
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_Magnet);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				setObject[kInv_Magnet]->Show(false);
				pointerInHand->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken = true;
				//txtMagnet->SetStrikethrough(true);//dd
				paperclip->Show(true);
				magnet2->Show(true);
				cAnimMagnet->PlayAnim();
				stick->ZOrder = 17;
				isMagnetAnimating = true;
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOClipClicked);
				
				
				return;
			}
			break;
		case kInv_PaperClip:
			if(setPoly[kInv_PaperClip]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed && ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken && ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved )
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_PaperClip);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				//setObject[4]->Show(false);
				//boxDoorOpen->Show(true);
				pointerInHand->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed = true;
				//UpdateInsideBoxProperties();
				PaperClipAnim->Show(true);
				PaperClipAnim->ForcePlayAnimation(0);
				isPaperClipAnimPlaying = true;
				//txtPaperClip->SetStrikethrough(true);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOOpenLockWithClip);
				
				
				return;
			}
			break;
		case kInv_FunnelNose:
			if(setPoly[kInv_FunnelNose]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOFunnelPiecesFixed && ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened)
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_FunnelNose);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				setObject[kInv_FunnelNose]->Show(false);
				funnelFull->Show(true);
				//funnelFull->Pos = funnelFullPos;
				pointerInHand->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S6_HOFunnelPiecesFixed = true;
				UpdateInsideBoxProperties();
				//txtFunnel->SetStrikethrough(true);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOFunnelPartJoin);
				
				
				return;
			}
			break;

		case kInv_Handle:
			if(setPoly[kInv_Handle]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened)
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_Handle);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				setObject[kInv_Handle]->Show(true);
				pointerInHand->Show(false);
				UpdateInsideBoxProperties();
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOGloveUsed);
				return;
			}
			break;
		case kInv_OilDripper:
			if(setPoly[kInv_OilDripper]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved)
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_OilDripper);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				pointerInHand->Show(false);
				//setObject[kInv_OilDripper]->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved = true;
				//txtOilDripper->SetStrikethrough(true);
				oilDipperOnHand->Show(true);
				cAnimOilDipping->PlayAnim();
				isOilDrippingAnimPlaying = true;
				oilDripAnimTimer = 0;
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOOilDipperUsed);
				
				
				return;
			}
			break;
		case kInv_Glove:
			if(setPoly[kInv_Glove]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened && isAllToolsMoved && ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved )
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_Glove);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				pointerInHand->Show(false);
				setObject[kInv_Glove]->Show(false);

				SmallBoxDoorOpenAnim->Show(true);
				SmallBoxDoorOpenAnim->ForcePlayAnimation(0);
				isSmallBoxDoorOpenAnimPlaying = true;

				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOGloveUsed);
				
				
				return;
			}
			break;
		case kInv_Tape:

			if(setPoly[kInv_Tape]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened && !ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel && ProfileSystem::Instance->ActiveProfile.L1S6_HOFunnelPiecesFixed)
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_Tape);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				pointerInHand->Show(false);
				setObject[kInv_Tape]->Show(false);//sr
				funnelFull->Show(false);
				cAnimTapeRoll->PlayAnim();
				tapeRollAnimObj1->Show(true);
				tapeRollAnimObj2->Show(true);
				tapeRollAnimObj3->Show(true);
				isTapeRollingAnimPlaying = true;
				//txtTape->SetStrikethrough(true);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOTapeUsed);
				
				
				return;
			}
			break;
		case kInv_Blade:

			if(setPoly[kInv_Blade]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel && !ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeCutOnFunnel )
			{
				hoInventoryBox->UnloadInventoryItemFromVisibleArea(kInv_Blade);
				hoInventoryBox->LoadNextInventoryItemToVisibleArea();
				isObjectInHand = false;
				pointerInHand->Show(false);
				setObject[kInv_Blade]->Show(false);
				funnelFullWithTape->Show(false);
				cAnimTapeCut->PlayAnim();
				for (int i = 0; i < 5; i++)
				{
					tapeCutAnimObjects[i]->Show(true);
				}
				isTapeCuttingAnimPlaying = true;
				//txtBlade->SetStrikethrough(true);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HOBladeUsed);
				
				
				return;
			}

			break;

		default:
			break;
		}

		isObjectInHand = false;
		//getObject[objectID]->Show(false);
		//return to inventory box
		hoInventoryBox->ReturnItem(objectID, pointerInHand->Pos);
		SAFE_DELETE(pointerInHand);
		/*isObjectReturning = true;
		returnObject = pointerInHand;
		returnTimer = 0.0f;
		startPos = pointerInHand->Pos;
		endPos  = getObject[objectID]->Pos;

		float distance = (float)sqrt( ((startPos.x - endPos.x)*(startPos.x - endPos.x)) + ((startPos.y - endPos.y) * (startPos.y - endPos.y)));
		returnEndTimer = distance / 1000;
		*/

	}
	return;

}

void L1_S6_HOBoatShed::ResetHO()
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
		
			ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOBeettleMoved = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxRustRemoved = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxRustRemoved = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperClipUsed = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOSmallBoxOpened = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOFunnelPiecesFixed = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeAppliedOnFunnel = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOTapeCutOnFunnel = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOInvOilDripperTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOInvFunnelTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOLeftMiniDoorOpened = false;

			for (int i = 0; i < 3; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolMoved[i] = false;
			}

			for (int i = 0; i < 3; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L1S6_HOIsToolInLeftMiniBoxMoved[i] = false;
			}
			ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved = false;
			ProfileSystem::Instance->ActiveProfile.L1S6_HOCompletedWithMatch3 = false;

			for (int i = 0; i < MAX_HOITEMS; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L1S6_HOInvItemStates[i] = 0;
			}

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_HOBoatShed;
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
void L1_S6_HOBoatShed::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);
				if(match3)
					match3->clearBoard();

				txtMatch3->Show(false);
				txtPuzzle->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

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