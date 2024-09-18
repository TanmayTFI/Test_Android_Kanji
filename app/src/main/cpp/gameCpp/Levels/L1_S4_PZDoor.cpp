//====================================
//	DQFYLH
//	Level 1 - Scene 03 :  DOOR PUZZLE
//====================================

#include "L1_S4_PZDoor.h"
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

const CRectangle  areaRectPayal(872.0f,459.0f,147.0f,107.0f);
const CRectangle  PopRect1Match3(1098.0f,9.0f,265.0f,95.0f);

//Scene Rects

const CRectangle  InfoRectKeyhole(611.0f,69.0f,141.0f,131.0f);

//Inventory Destination Rects
const CRectangle  InvDestRectKeyset(269.0f,38.0f,828.0f,554.0f);
const CRectangle  InvDestRectShadowHands(260.0f,41.0f,842.0f,550.0f);

//Inventory Item Rects
const CPoint duckPoint(291.0f,503.0f,0.0f);
const CPoint payalPoint(941.0f,502.0f,0.0f);

//Video Rects
const CRectangle  VidRectFull(0,0,1366,768);
const CPoint DoorOpenAnimPos(234.0f,308.0f,0.0f);

const CPoint CoinRotationPos1(313.0f,176.0f,0.0f);
const CPoint CoinRotationPos2(453.0f,273.0f,0.0f);
const CPoint CoinRotationPos3(629.0f,299.0f,0.0f);
const CPoint CoinRotationPos4(806.0f,273.0f,0.0f);
const CPoint CoinRotationPos5(946.0f,176.0f,0.0f);

L1_S4_PZDoor::L1_S4_PZDoor()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S4\\P2DOOR\\P1.SCN");
	musicFadeOutTimer = 0;
	isPuzzleSolved = false;


	if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsShadowPuzzleSolved)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsShadowPuzHandDoorOpened)
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced)
				Hud->InvBox_StartPuzzleMode();
			else
				Hud->InvBox_StopPuzzleMode();
		}
		else
			Hud->InvBox_StartPuzzleMode();
	}
	else
	{
		isPuzzleSolved = true;
		Hud->InvBox_StopPuzzleMode();
	}

	//InitPopups();
	GFInstance->PostUpdate(2);

	isObjectReturning = false;
	isObjectInHand = false;
	
	coinRotationPos[0] = CoinRotationPos1;
	coinRotationPos[1] = CoinRotationPos2;
	coinRotationPos[2] = CoinRotationPos3;
	coinRotationPos[3] = CoinRotationPos4;
	coinRotationPos[4] = CoinRotationPos5;

	isKeyAnimPlaying = false;
	isDoorOpenAnimPlaying = false;
	isCoinRotationAnimPlaying = false;
	isLockAnimationPlaying = false;
	isHandReversing = false;
	isHandSelected = false;
	isDoorOpened = true;
	isHandMoving = false;
	movingTime = 0.15f;

	isShutterOpening = false;
	timerForShutter = 0.0f;
	timeToOpenShutter = 0.5f;

	shapeRightIndices[0][0] = 9;
	shapeRightIndices[0][1] = 0;

	shapeRightIndices[1][0] = 1;
	shapeRightIndices[1][1] = 7;

	shapeRightIndices[2][0] = 2;
	shapeRightIndices[2][1] = 6;

	shapeRightIndices[3][0] = 8;
	shapeRightIndices[3][1] = 11;

	shapeRightIndices[4][0] = 3;
	shapeRightIndices[4][1] = 10;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			shapeCurrentIndices[i][j] = -1;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		blueShutters[i] = HOScene->GetObjectByID(std::string("shutterBlue" + MKSTR(i)).c_str());
		shutters[i] = HOScene->GetObjectByID(std::string("shutter" + MKSTR(i)).c_str());
		locks[i] = HOScene->GetObjectByID(std::string("lock" + MKSTR(i)).c_str());

		if(ProfileSystem::Instance->ActiveProfile.L1S3_IsShadowPuzzleSolved)
		{
			locks[i]->Show(false);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		success[i] = HOScene->GetObjectByID(std::string("success" + MKSTR(i + 1)).c_str());
	}

	for (int i = 0; i < 5; i++)
	{
		images[i] = HOScene->GetObjectByID(std::string("image" + MKSTR(i + 1)).c_str());
	}

	for (int i = 0; i < 5; i++)
	{
		circleSelection[i] = HOScene->GetObjectByID(std::string("circleSelection" + MKSTR(i)).c_str());
		circleSelection[i]->SetZ(199);
		circleSelection[i]->Show(false);
	}

	LockAnim[0] = HOScene->GetObjectByID("Lock0Anim");
	LockAnim[1] = HOScene->GetObjectByID("Lock1Anim");
	LockAnim[2] = HOScene->GetObjectByID("Lock2Anim");
	LockAnim[3] = HOScene->GetObjectByID("Lock3Anim");
	LockAnim[4] = HOScene->GetObjectByID("Lock4Anim");

	for (int i = 0; i < 5; i++)
	{
		LockAnim[i]->Show(false);
	}

	for (int i = 0; i < 12; i++)
	{
		polySelection[i] = HOScene->GetObjectByID(std::string("polySelection" + MKSTR(i)).c_str());
		polySelection[i]->Show(false);

		hands[i] = HOScene->GetObjectByID(std::string("hand" + MKSTR(i + 1)).c_str());
		hands[i]->SetZ(200);
		handsInitialPositions[i] = hands[i]->Pos;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced)
	{
		hands[6]->SetEnable(false);
		hands[9]->SetEnable(false);
	}

	door = HOScene->GetObjectByID("door");
	door->SetZ(201);

	for (int i = 0; i < 5; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsImagesFound[i] == false)
		{
			isDoorOpened = false;
			blueShutters[i]->Show(false);
		}
		else
		{
			images[i]->Show(false);
			shutters[i]->Show(false);
			blueShutters[i]->Show(true);
		}
	}
	solvedIndex = 0;

	for (int i = 0; i < 12; i++)
	{
		isHandSelectable[i] = true;
	}
	DoorOpenAnim = NULL;
	CoinRotationAnim = NULL;

	lockIndex = 0;
	if(isDoorOpened)
	{
		door->Show(false);
		for (int i = 0; i < 5; i++)
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsHandFixed[i] == true )
			{
				success[i]->Show(true);
				lockIndex++;
				isHandSelectable[shapeRightIndices[i][0]] = false;
				isHandSelectable[shapeRightIndices[i][1]] = false;
				for (int j = 0; j < 2; j++)
				{
					//shapes[(i * 2) + j]->Show(true);
					
					hands[shapeRightIndices[i][j]]->Show(false);
					isHandFixed[i][j] = true;
					shapeCurrentIndices[i][j] = 12;
					
				}
				solvedIndex ++;
			}
			else
			{
				success[i]->Show(false);
				
				for (int j = 0; j < 2; j++)
				{
					//shapes[(i * 2) + j]->Show(false);
					shapeCurrentIndices[i][j] = -1;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			//shapes[i]->Show(false);
			success[i]->Show(false);
		}

		DoorOpenAnim = new CObject();
		DoorOpenAnim->LoadSprite("LV1\\S4\\P2Door\\DoorOpenAnim.SPR");
		DoorOpenAnim->SetZ(200);
		DoorOpenAnim->Pos = DoorOpenAnimPos;
		DoorOpenAnim->Show(false);

		CoinRotationAnim = new CObject();
		CoinRotationAnim->LoadSprite("LV1\\S4\\P2Door\\CoinRotationAnim.SPR");
		CoinRotationAnim->SetZ(150);
		CoinRotationAnim->Show(false);
	}

	KeyAnim[0] = HOScene->GetObjectByID("Key1Anim");
	KeyAnim[1] = HOScene->GetObjectByID("Key2Anim");
	KeyAnim[2] = HOScene->GetObjectByID("Key3Anim");

	for (int i = 0; i < 3; i++)
	{
		KeyAnim[i]->Show(false);
	}

	int ArrayPts0[] = {1084,245,1142,256,1169,269,1176,293,1161,319,1100,319,1084,295,1071,281};
	polyImages[0] =  new CPolygon(8);
	polyImages[0]->Init(ArrayPts0);

	int ArrayPts1[] = {152,73,197,46,203,22,231,24,237,44,272,75,216,95,185,132,155,142};
	polyImages[1] =  new CPolygon(9);
	polyImages[1]->Init(ArrayPts1);

	int ArrayPts2[] =  {782,68,881,68,907,84,890,106,858,140,808,138};
	polyImages[2] =   new CPolygon(6);
	polyImages[2]->Init(ArrayPts2);

	int ArrayPts3[] = {587,72,586,105,599,121,614,148,623,172,649,154,626,125,652,107,626,65,605,73};
	polyImages[3]=  new CPolygon(10);
	polyImages[3]->Init(ArrayPts3);

	int ArrayPts4[] = {1114,148,1155,144,1183,167,1186,184,1178,194,1185,233,1150,247,1113,239,1109,244,1090,228,1098,166};
	polyImages[4] = new CPolygon(11);
	polyImages[4]->Init(ArrayPts4);

	//poly Hands
	
	int ArrayPts01[] = {259,489,302,426,339,490,301,557};
	polyHands[0] =  new CPolygon(4);
	polyHands[0]->Init(ArrayPts01);

	int ArrayPts02[] = {325,422,363,362,404,422,365,487};
	polyHands[1] =  new CPolygon(4);
	polyHands[1]->Init(ArrayPts02);

	int ArrayPts03[] = {388,502,429,434,469,499,429,569};
	polyHands[2] =  new CPolygon(4);
	polyHands[2]->Init(ArrayPts03);

	int ArrayPts04[] = {459,550,500,487,540,551,499,618};
	polyHands[3] =  new CPolygon(4);
	polyHands[3]->Init(ArrayPts04);

	int ArrayPts05[] = {527,501,570,438,609,503,569,569};
	polyHands[4] =  new CPolygon(4);
	polyHands[4]->Init(ArrayPts05);

	int ArrayPts06[] = {599,554,639,493,680,555,639,619};
	polyHands[5] =  new CPolygon(4);
	polyHands[5]->Init(ArrayPts06);

	int ArrayPts07[] = {675,507,719,444,758,505,719,573};
	polyHands[6] =  new CPolygon(4);
	polyHands[6]->Init(ArrayPts07);

	int ArrayPts08[] = {756,547,800,485,841,550,800,615};
	polyHands[7] =  new CPolygon(4);
	polyHands[7]->Init(ArrayPts08);

	int ArrayPts09[] = {821,486,866,425,908,489,867,556};
	polyHands[8] =  new CPolygon(4);
	polyHands[8]->Init(ArrayPts09);

	int ArrayPts010[] = {903,528,945,463,987,531,945,599};
	polyHands[9] =  new CPolygon(4);
	polyHands[9]->Init(ArrayPts010);

	int ArrayPts011[] = {966,437,1006,371,1050,434,1007,506};
	polyHands[10] =  new CPolygon(4);
	polyHands[10]->Init(ArrayPts011);

	int ArrayPts012[] = {1029,502,1071,440,1112,503,1072,573};
	polyHands[11] =  new CPolygon(4);
	polyHands[11]->Init(ArrayPts012);


	int ArrayPts20[] = {315,234,325,191,360,172,401,188,417,236,387,273,344,272};
	polyCircles[0] =  new CPolygon(7);
	polyCircles[0]->Init(ArrayPts20);

	int ArrayPts21[] = {453,331,465,288,505,270,545,291,555,337,520,372,478,365};
	polyCircles[1]=  new CPolygon(7);
	polyCircles[1]->Init(ArrayPts21);

	int ArrayPts22[] = {633,343,654,308,693,300,728,325,734,364,709,393,669,398,643,381};
	polyCircles[2] =  new CPolygon(8);
	polyCircles[2]->Init(ArrayPts22);

	int ArrayPts23[] = {809,315,832,279,876,275,908,304,907,345,876,370,839,367,813,345};
	polyCircles[3] =  new CPolygon(8);
	polyCircles[3]->Init(ArrayPts23);

	int ArrayPts24[] = {946,223,966,187,1001,176,1036,192,1048,226,1036,262,1006,277,969,269,951,247};
	polyCircles[4] =  new CPolygon(9);
	polyCircles[4]->Init(ArrayPts24);

	int ArrayPts25[] = {960,429,957,397,981,371,1017,376,1030,406,1018,438,998,448,994,478,987,512,989,554,979,561,954,554,956,515,972,517,979,455,977,444};
	polyKey[0] =  new CPolygon(16);
	polyKey[0]->Init(ArrayPts25);

	int ArrayPts26[] = {1028,395,1040,388,1075,431,1046,466,1045,576,1041,581,1013,581,1013,544,1025,541,1024,469,1006,445,1023,437,1032,408};
	polyKey[1] =  new CPolygon(13);
	polyKey[1]->Init(ArrayPts26);

	int ArrayPts27[] = {1104,501,1087,511,1097,536,1112,534,1134,527,1095,436,1112,420,1115,404,1107,389,1092,384,1078,377,1064,365,1059,384,1052,399,1075,428,1074,434,1082,441};
	polyKey[2] =  new CPolygon(17);
	polyKey[2]->Init(ArrayPts27);

	pointHands[0] = new CPoint(303.0f,297.0f,0.0f);
	pointHands[1] = new CPoint(359.0f,310.0f,0.0f);
	pointHands[2] = new CPoint(424.0f,380.0f,0.0f);
	pointHands[3] = new CPoint(497.0f,399.0f,0.0f);
	pointHands[4] = new CPoint(564.0f,413.0f,0.0f);
	pointHands[5] = new CPoint(632.0f,417.0f,0.0f);
	pointHands[6] = new CPoint(680.0f,414.0f,0.0f);
	pointHands[7] = new CPoint(713.0f,419.0f,0.0f);
	pointHands[8] = new CPoint(793.0f,415.0f,0.0f);
	pointHands[9] = new CPoint(861.0f,399.0f,0.0f);
	pointHands[10] = new CPoint(939.0f,375.0f,0.0f);
	pointHands[11] = new CPoint(1005.0f,301.0f,0.0f);
	pointHands[12] = new CPoint(1059.0f,305.0f,0.0f);

	keyOnScene = HOScene->GetObjectByID("keyOnScene");

	getPoly[0] = polyKey[0];
	getObject[0] = keyOnScene;
	mousePointerString[0] = "LV1\\S4\\P2Door\\keyPointer1.SPR";

	int ArrayPts00[] = {492,0,491,275,856,289,858,2};
	polyKeyHole =  new CPolygon(4);
	polyKeyHole->Init(ArrayPts00);
	setPoly[0] = polyKeyHole;
	setObject[0] = NULL;

	getPoly[1] = polyKey[1];
	getObject[1] = keyOnScene;
	mousePointerString[1] = "LV1\\S4\\P2Door\\keyPointer2.SPR";

	setPoly[1] = polyKeyHole;
	setObject[1] = NULL;

	getPoly[2] = polyKey[2];
	getObject[2] = keyOnScene;
	mousePointerString[2] = "LV1\\S4\\P2Door\\keyPointer3.SPR";

	setPoly[2] = polyKeyHole;
	setObject[2] = NULL;

	if(!ProfileSystem::Instance->ActiveProfile.L1S4_P2IsKeySetPlaced)
	{
		keyOnScene->Show(false);
	}

	for (int i = 0; i < 3; i++)
	{
		keySel[i] = HOScene->GetObjectByID(std::string("keySel" + MKSTR(i + 1)).c_str());
		keySel[i]->Show(false);
	}
	
	if(solvedIndex > 0)
	{
		locks[5 - solvedIndex]->Show(false);
	}

	zOrderScnVideo = 1;
	zOrderPopVideo = eZOrder_Popup + 10;

	IsZOrderChangeToScene = false;

	shutterVFX = NULL;
	shutterVFX = new SuperFX();
	shutterVFX->LoadEffectIni("PRT\\L1\\DiffPuzFX.INI");
	shutterVFX->SetZ(250);

	lockVFX = NULL;
	lockVFX = new SuperFX();
	lockVFX->LoadEffectIni("PRT\\L1\\DiffPuzFX.INI");
	lockVFX->SetZ(250);

	successVFX = NULL;
	successVFX = new SuperFX();
	successVFX->LoadEffectIni("PRT\\L1\\DiffPuzFX.INI");
	successVFX->SetZ(250);

	SetZ(1);

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if( Control::Audio->IsPlaying(aTrackL1Hunted) )
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;	


	Control::Audio->LoadSample(aAmbS4PuzShadowhand,AudioAmb);
	Control::Audio->PlaySample(aAmbS4PuzShadowhand,true);
	//MUSIC_SFX
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S4_PZDoor]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S4_PZDoor])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S4_PZDoor] = true;

	
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

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

	Visible = true;	//To render anything with renderlistentity
}

void L1_S4_PZDoor::ProcessSkip()
{
	//if puzzle Door Not opened Find the Animals Puzzle
	if(!ProfileSystem::Instance->ActiveProfile.L1S4_P2IsShadowPuzHandDoorOpened)
	{
		Hud->InvBox_StopPuzzleMode();
		ProfileSystem::Instance->ActiveProfile.SkipTimer[ProfileSystem::Instance->ActiveProfile.CurrentArea] = 0;

		for (int i = 0; i < 5; i++)
		{
			images[i]->Show(false);
			shutters[i]->Show(false);
			circleSelection[i]->Show(false);
			blueShutters[i]->Show(true);
			ProfileSystem::Instance->ActiveProfile.L1S4_P2IsImagesFound[i] = true;
		}
		isDoorOpened = true;
		door->Show(false);
		ProfileSystem::Instance->ActiveProfile.L1S4_P2IsShadowPuzHandDoorOpened = true;
	}
	else
	{
		//Hand Door Opened. Skip the Hand moving Puzzle here
		isPuzzleSolved = true;
		Hud->InvBox_StopPuzzleMode();

		for (int i = 0; i < 5; i++)
		{
			images[i]->Show(false);
			shutters[i]->Show(false);
			success[i]->Show(true);
			circleSelection[i]->Show(false);
			locks[i]->Show(false);
			blueShutters[i]->Show(true);
			ProfileSystem::Instance->ActiveProfile.L1S4_P2IsHandFixed[i] = true;
		}

		ProfileSystem::Instance->ActiveProfile.L1S3_IsShadowPuzzleSolved = true;
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		for (int i = 0; i < 12; i++)
		{
			if(!(i == 5 || i == 4))
			{
				hands[i]->SetEnable(false);
			}
			polySelection[i]->SetEnable(false);
		}
	}
}

void L1_S4_PZDoor::InitPopups()
{

}

void L1_S4_PZDoor::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL1Hunted) )
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
	else if( Control::Audio->IsPlaying(aTrackL1Mansion) )
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

	if( IsPopupTransitionActive )
		UpdatePopupTransition();
}

void L1_S4_PZDoor::InitPuzzle()
{
	
}

void L1_S4_PZDoor::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;	

	if(isChangeZOrder)
	{
		SetZ(1);
		isChangeZOrder = false;
	}

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

	if(isPuzzleSolved && ProfileSystem::Instance->_isExtrasGamePlay)
			return;

	if(isPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L1S4_P2IsKeySetPlaced && InfoRectKeyhole.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if(Control::Input->LBclicked())
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken)
				Hud->ShowBannerText("L1bunch");
			else
				Hud->ShowBannerText("L1almost");
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsShadowPuzHandDoorOpened && !ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced)
	{
		if(InvDestRectShadowHands.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				polySelection[6]->Show(true);
				polySelection[9]->Show(true);

				polySelection[6]->SetAlpha(1.0f);
				polySelection[9]->SetAlpha(1.0f);

				float valArr[5] = {0, -1, -1, -1, -1};
				polySelection[6]->TweenTo(valArr, 0.35f, 0, true);
				polySelection[9]->TweenTo(valArr, 0.35f, 0, true);

				Hud->ShowBannerText("L1hand");
			}
		}
	}
	else
	{
		if(isDoorOpened)
		{
			//START: HAND MOVING - FORWARD AND BACKWARD

			if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsKeySetPlaced)
			{
				for (int j = 0; j < 3; j++)
				{
					keySel[j]->Show(false);
				}
				for (int i = 0; i < 3; i++)
				{
					if(polyKey[i]->Intersection(&pos) && !isObjectInHand)
					{
						keySel[i]->Show(true);
						Cursor::SetMode(CA_HandPointy);
					}
				}

				if(isObjectReturning)
				{
					//K_LOG("Update: 3");
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
					}
					return;
				}

				if(isKeyAnimPlaying)
				{
					if(KeyAnim[objectID]->IsAnimationFinished())
					{
						isKeyAnimPlaying = false;

						if(objectID == 1)
						{
							ProfileSystem::Instance->ActiveProfile.L1S4_IsShadowPuzFinalKeyTurned = true;
							ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_Bedroom;
							SendMessage(ControlExit, ID, HOSM_NewArea);
							return;
						}
						else
						{
							KeyAnim[objectID]->Show(false);
							pointerInHand->Show(true);
							isObjectInHand = false;
							isObjectReturning = true;
							returnObject = pointerInHand;
							returnTimer = 0.0f;
							startPos = pointerInHand->Pos;
							endPos  = getObject[objectID]->Pos;

							float distance = (float)sqrt( ((startPos.x - endPos.x)*(startPos.x - endPos.x)) + ((startPos.y - endPos.y) * (startPos.y - endPos.y)));
							returnEndTimer = distance / 1000;
						}
					}
					return;
				}

				if(isObjectInHand)
				{
				//	K_LOG("Update: 4");
					pointerInHand->SetPos(&pos);
					pointerInHand->Pos.y += 100;
					if( Control::Input->LBclicked())
					{
						switch (objectID)
						{
						case 0:
							if(setPoly[0]->Intersection(&pos) )
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowWrongKey);

								isObjectInHand = false;
								pointerInHand->Show(false);
								KeyAnim[objectID]->Show(true);
								KeyAnim[objectID]->ForcePlayAnimation(0);
								isKeyAnimPlaying = true;
								Hud->ShowBannerText("Exrong");
								return;
							}
							break;
						case 1:
							if(setPoly[1]->Intersection(&pos) )
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowRightKey);

								isObjectInHand = false;
								pointerInHand->Show(false);
								KeyAnim[objectID]->Show(true);
								KeyAnim[objectID]->ForcePlayAnimation(0);
								isKeyAnimPlaying = true;
								return;
							}
							break;
						case 2:
							if(setPoly[2]->Intersection(&pos) )
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowWrongKey);

								isObjectInHand = false;
								pointerInHand->Show(false);
								KeyAnim[objectID]->Show(true);
								KeyAnim[objectID]->ForcePlayAnimation(0);
								isKeyAnimPlaying = true;
								Hud->ShowBannerText("Exrong");
								return;
							}
							break;

						default:
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

				bool isRightClick = false;

				for (int i = 0; i < 3; i++)
				{
					switch (i)
					{
					case 0:
						if(getPoly[0]->Intersection(&pos) )
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
						if(getPoly[1]->Intersection(&pos) )
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
						if(getPoly[2]->Intersection(&pos) )
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

					}

					if(isRightClick)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowKeySelected);

						isObjectInHand = true;
						getObject[objectID]->Show(false);

						pointerInHand = new CObject();
						pointerInHand->LoadSprite(mousePointerString[objectID]);
						pointerInHand->Show(true);
						pointerInHand->Pos = getObject[objectID]->Pos;
						pointerInHand->SetZ(200);
						return;
					}

				}
			}

			if(isLockAnimationPlaying)
			{
				if(LockAnim[lockIndex]->IsAnimationFinished())
				{
					LockAnim[lockIndex]->Show(false);
					isLockAnimationPlaying = false;
				}
			}

			if(isHandMoving)
			{
				moveTimer += Control::LogicRate;
				float xPos = startPos.x + (endPos.x - startPos.x ) * easing(moveTimer, 0, 1, movingTime);
				float yPos = startPos.y + (endPos.y - startPos.y ) * easing(moveTimer, 0, 1, movingTime);

				movingObject->Pos = CPoint(xPos, yPos, 0.0f);

				if(moveTimer >= movingTime)
				{
					movingObject->Pos = endPos;
					pathPointer ++;
					if(pathPointer > noOfPoints)
					{
						isHandMoving = false;

						if(isHandReversing == false)
						{
							int i = 0;
							if(shapeCurrentIndices[targetIndex][i] == -1 && shapeRightIndices[targetIndex][i] == selectedIndex)
							{
								shapeCurrentIndices[targetIndex][i] = selectedIndex;
							}
							else if(shapeCurrentIndices[targetIndex][i + 1] == -1 && shapeRightIndices[targetIndex][i + 1] == selectedIndex)
							{
								shapeCurrentIndices[targetIndex][i + 1] = selectedIndex;
							}
							else
							{
								if(shapeCurrentIndices[targetIndex][i] == -1)
								{
									shapeCurrentIndices[targetIndex][i] = selectedIndex;
								}
								else if(shapeCurrentIndices[targetIndex][i + 1] == -1)
								{
									shapeCurrentIndices[targetIndex][i + 1] = selectedIndex;
								}
							}

							//K_LOG ("shapeCurrentIndices[%d][%d] = %d  -- shapeRightIndices[%d][%d] = %d !!!!! shapeCurrentIndices[%d][%d] = %d  ------ shapeRightIndices[%d][%d] = %d", targetIndex, i,shapeCurrentIndices[targetIndex][i], targetIndex, i, shapeRightIndices[targetIndex][i],targetIndex, i,shapeCurrentIndices[targetIndex][i + 1], targetIndex, i, shapeRightIndices[targetIndex][i + 1]);

							if(shapeCurrentIndices[targetIndex][i] == shapeRightIndices[targetIndex][i] && shapeCurrentIndices[targetIndex][i + 1] == shapeRightIndices[targetIndex][i + 1])
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowPieceMatched);
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowLockReveled);

								success[targetIndex]->Show(true);
								successVFX->SetPos(PointSystem::CreateCPoint(hands[shapeCurrentIndices[targetIndex][0]]->Pos.x,hands[shapeCurrentIndices[targetIndex][0]]->Pos.y,0));
								successVFX->Start();
								hands[shapeCurrentIndices[targetIndex][0]]->Show(false);
								hands[shapeCurrentIndices[targetIndex][1]]->Show(false);

								ProfileSystem::Instance->ActiveProfile.L1S4_P2IsHandFixed[targetIndex] = true;

								shapeCurrentIndices[targetIndex][0] = 12;
								shapeCurrentIndices[targetIndex][1] = 12;
								solvedIndex ++;
								if(solvedIndex > 0)
								{
									lockVFX->SetPos(PointSystem::CreateCPoint(locks[lockIndex]->Pos.x ,locks[lockIndex]->Pos.y,0));
									lockVFX->Start();
									lockIndex = 5 - solvedIndex;
									locks[lockIndex]->Show(false);
									LockAnim[lockIndex]->Show(true);
									LockAnim[lockIndex]->ForcePlayAnimation(0);
									isLockAnimationPlaying = true;
								}

								if(solvedIndex == 5)
								{
									//Puzzle solved
									isPuzzleSolved = true;
									if(_handleExtrasGameComplete())
										return;
									ProfileSystem::Instance->ActiveProfile.L1S3_IsShadowPuzzleSolved = true;
									AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
									Hud->InvBox_StopPuzzleMode();
								}
							}

							isHandSelected = false;

							for (int i = 0; i < 12; i++)
							{
								polySelection[i]->Show(false);
								if(i < 5)
								{
									circleSelection[i]->Show(false);
								}
							}
						}
						else
						{
							isHandReversing = false;
						}
					}
					else
					{
						startPos = path[pathPointer - 1];
						endPos = path[pathPointer];
						moveTimer = 0.0f;
					}
				}
				return;
			}
			//END: HAND MOVING - FORWARD AND BACKWARD

			if(!isPuzzleSolved)
			{
				bool flag;
				for (int i = 0; i < 12; i++)
				{
					if(polyHands[i]->Intersection(&pos) && isHandSelectable[i] == true)
					{
						flag = true;
						for (int j = 0; j < 5; j++)
						{
							for (int k = 0; k < 2; k++)
							{
								//K_LOG ("shapeCurrentIndices[%d][%d] = %d  |  i = %d ",j,k,shapeCurrentIndices[j][k],i);
								if(shapeCurrentIndices[j][k] == i)
								{
									flag = false;
									break;
								}
							}
						}

						if(flag)
						{
							Cursor::SetMode(CA_HandPointy);
							if(Control::Input->LBclicked())
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowPieceSelect);

								for (int k = 0; k < 12; k++)
								{
									polySelection[k]->Show(false);
								}
								polySelection[i]->Show(true);
								isHandSelected = true;
								selectedIndex = i;
								for (int j = 0; j < 5; j++)
								{
									if(shapeCurrentIndices[j][0] == -1 || shapeCurrentIndices[j][1] == -1)
									{
										circleSelection[j]->Show(true);
										circleSelection[j]->SetZ(199);
									}
								}
								return;
							}
						}
					}
				}
			}

			if(isHandSelected)
			{
				for (int i = 0; i < 5; i++)
				{
					if(polyCircles[i]->Intersection(&pos) && (shapeCurrentIndices[i][0] == -1 || shapeCurrentIndices[i][1] == -1))
					{
						Cursor::SetMode(CA_HandPointy);
						if(Control::Input->LBclicked())
						{
							isHandSelectable[selectedIndex] = false;
							selectedIndex2 = selectedIndex;
							if(selectedIndex > 5) selectedIndex2 ++;
							CPoint pt(hands[selectedIndex]->Pos.x, hands[selectedIndex]->Pos.y, 0.0f);
							path[0] = pt;
							path[1] = *pointHands[selectedIndex2];
							int count = 1;
							float distance;
							if (abs(circleSelection[i]->Pos.x - pointHands[selectedIndex2]->x) > 10.0f)
							{
								if(circleSelection[i]->Pos.x < pointHands[selectedIndex2]->x) // left
								{
									for (int j = selectedIndex2; j >= 0 ; j--)
									{
										path[count] = *pointHands[j];
										count++;
										distance = abs(circleSelection[i]->Pos.x - pointHands[j]->x);

										if (distance <= 10.0f)
										{
											break;
										}
									}
								}
								else
								{
									for (int j = selectedIndex2; j < 13 ; j++)
									{
										path[count] = *pointHands[j];
										count++;
										distance = abs(circleSelection[i]->Pos.x - pointHands[j]->x);

										if (distance <= 10.0f)
										{
											break;
										}
									}
								}
							}

							path[count] = circleSelection[i]->Pos;
							noOfPoints = count;

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowPieceSlide);

							isHandMoving = true;
							moveTimer = 0.0f;

							pathPointer = 1;
							startPos = path[0];
							endPos = path[1];
							movingObject = hands[selectedIndex];
							targetIndex = i;
							return;
						}
					}
				}

				if(Control::Input->LBclicked())
				{
					for (int i = 0; i < 12; i++)
					{
						polySelection[i]->Show(false);
					}
					for (int i = 0; i < 5; i++)
					{
						circleSelection[i]->Show(false);
					}
					isHandSelected = false;
				}
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					//reverse
					if(polyCircles[i]->Intersection(&pos) && (shapeCurrentIndices[i][0] != -1 || shapeCurrentIndices[i][1] != -1) && shapeCurrentIndices[i][0] != 12 && shapeCurrentIndices[i][1] != 12)
					{
						//K_LOG ("shapeCurrentIndices[%d][0] = %d and shapeCurrentIndices[%d][1] = %d",i,shapeCurrentIndices[i][0],i,shapeCurrentIndices[i][1]);
						Cursor::SetMode(CA_HandPointy);
						if(Control::Input->LBclicked())
						{
							switch (i)
							{
							case 0:
								selectedIndex = 1;
								break;
							case 1:
								selectedIndex = 3;
								break;
							case 2:
								selectedIndex = 6;
								break;
							case 3:
								selectedIndex = 9;
								break;
							case 4:
								selectedIndex = 11;
								break;
							}

							if(shapeCurrentIndices[i][1] != -1)
							{
								handDestinationIndex = shapeCurrentIndices[i][1];
								shapeCurrentIndices[i][1] = -1;
							}
							else
							{
								handDestinationIndex = shapeCurrentIndices[i][0];
								shapeCurrentIndices[i][0] = -1;
							}

							isHandSelectable[handDestinationIndex] = true;

							CPoint pt(hands[handDestinationIndex]->Pos.x, hands[handDestinationIndex]->Pos.y, 0.0f);
							path[0] = pt;
							//path[1] = *pointHands[selectedIndex];
							int count = 1;
							float distance = abs(polySelection[handDestinationIndex]->Pos.x - pointHands[selectedIndex]->x);

							if (distance > 10.0f)
							{
								if(polySelection[handDestinationIndex]->Pos.x < pointHands[selectedIndex]->x) // left
								{
									for (int j = selectedIndex; j >= 0 ; j--)
									{
										path[count] = *pointHands[j];
										count++;
										distance = abs(polySelection[handDestinationIndex]->Pos.x - pointHands[j]->x);
										if (distance <= 10.0f)
										{
											break;
										}
									}
								}
								else
								{
									for (int j = selectedIndex; j < 13 ; j++)
									{
										path[count] = *pointHands[j];
										count++;
										distance = abs(polySelection[handDestinationIndex]->Pos.x - pointHands[j]->x);
										if (distance <= 10.0f)
										{
											break;
										}
									}
								}
							}

							path[count] = handsInitialPositions[handDestinationIndex];
							noOfPoints = count;

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowPieceSlide);

							isHandMoving = true;
							moveTimer = 0.0f;
							pathPointer = 1;
							startPos = path[0];
							endPos = path[1];
							movingObject = hands[handDestinationIndex];
							targetIndex = i;
							isHandReversing = true;

							return;
						}
					}
				}
			}
		}
		else
		{
			if(isCoinRotationAnimPlaying)
			{
				if(CoinRotationAnim->IsAnimationFinished())
				{
					CoinRotationAnim->Show(false);
					isCoinRotationAnimPlaying = false;
					blueShutters[shutterIndex]->Show(true);

					bool flag = true;
					for (int j = 0; j < 5; j++)
					{
						if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsImagesFound[j] == false)
						{
							flag = false;
							return;
						}
					}

					if(flag == true)
					{
						isShutterOpening = true;
						door->Show(false);
						DoorOpenAnim->Show(true);
						DoorOpenAnim->ForcePlayAnimation(0);
					}
				}
				return;
			}
			if(isShutterOpening)
			{
				if(DoorOpenAnim->IsAnimationFinished())
				{
					isShutterOpening = false;
					isDoorOpened = true;
					ProfileSystem::Instance->ActiveProfile.L1S4_P2IsShadowPuzHandDoorOpened = true;
					DoorOpenAnim->Show(false);

					Hud->InvBox_StopPuzzleMode();
					ProfileSystem::Instance->ActiveProfile.SkipTimer[ProfileSystem::Instance->ActiveProfile.CurrentArea] = 0;
				}
				return;
			}

			for (int i = 0; i < 5; i++)
			{
				if(polyImages[i]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S4_P2IsImagesFound[i] == false)
				{
					if(Control::Input->LBclicked())
					{
						ProfileSystem::Instance->ActiveProfile.L1S4_P2IsImagesFound[i] = true;
						images[i]->FadeOut(4.0f);
						//shutters[i]->FadeOut(4.0f);
						shutters[i]->Show(false);
						shutterIndex = i;

						shutterVFX->SetPos(PointSystem::CreateCPoint(coinRotationPos[i].x + 50,coinRotationPos[i].y + 50,0));
						shutterVFX->Start();

						CoinRotationAnim->Show(true);
						CoinRotationAnim->Pos = coinRotationPos[i];
						CoinRotationAnim->ForcePlayAnimation(0);
						isCoinRotationAnimPlaying = true;

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowFlipDisc);

						return;
					}
				}
			}
		}
	}


	//if( !IsPopupTransitionActive )
		//UpdatePopups(pos);
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			Hud->InvBox_StopPuzzleMode();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_Bedroom;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif

	if( IsMouseOverGoBackArea(&pos) )
	{
#ifdef TOUCH_UI
        //if(!GFHud->InvBoxCtrl->navTapped)
            //Cursor::SetMode(CA_ExitDown,eArea_L1_S4_Bedroom);
#else
        Cursor::SetMode(CA_ExitDown,eArea_L1_S4_Bedroom);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
            if(!GFHud->InvBoxCtrl->navTapped)
            {
                GFHud->InvBoxCtrl->GoToScene(eArea_L1_S4_Bedroom, 2);
            }
#else
            Hud->InvBox_StopPuzzleMode();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_Bedroom;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
}

void L1_S4_PZDoor::UpdatePopups(CPoint pos,bool ItemDropped)
{
	
}

void L1_S4_PZDoor::UpdatePopupTransition()
{
	
}

float L1_S4_PZDoor::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L1_S4_PZDoor::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

	static int FrameCnt = 0;
	FrameCnt = (FrameCnt+1)%2;
}

void  L1_S4_PZDoor::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{
	
}

L1_S4_PZDoor::~L1_S4_PZDoor()
{
	GFHud->InvBoxCtrl->ResetToInvBox();	
	
	Control::Audio->Samples[aTrackL1Hunted]->SetVolume((float)Control::Audio->GlobalMusicVol);
	SAFE_DELETE(shutterVFX);
	SAFE_DELETE(CoinRotationAnim);
	SAFE_DELETE(DoorOpenAnim);
	SAFE_DELETE(ResetDia);
	
	//MUSIC_SFX
	if( Control::Audio->IsPlaying(aTrackL1Puzzle) )
		Control::Audio->StopSample(aTrackL1Puzzle);

	Control::Audio->StopSample(aAmbS4PuzShadowhand);
	Control::Audio->UnloadSample(aAmbS4PuzShadowhand);
	//MUSIC_SFX
}

void L1_S4_PZDoor::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
		case eInv_L1S4_KeySet:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_KeySet] = true;		
			vecInvIds.push_back(eInv_L1S4_KeySet);
			Hud->ShowInventory(&vecInvIds);
		break;
		default:
			break;
	}
}

void L1_S4_PZDoor::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		Hud->InvBox_StopPuzzleMode();
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
			if( itemVal == eInv_L1S4_KeySet )
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S4_P2IsKeySetPlaced && isPuzzleSolved)
				{
					if(InvDestRectKeyset.Intersection(&dropPos))
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_KeySet] = false;					
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S4_KeySet);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowKetSetPlaced);
						keyOnScene->Show(true);

						ProfileSystem::Instance->ActiveProfile.L1S4_P2IsKeySetPlaced = true;
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S3_ShadowHand1)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced)
				{
					if(InvDestRectShadowHands.Intersection(&dropPos))
					{
						if(ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken && ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken)
						{
							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_ShadowHand1] = false;
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_ShadowHand2] = false;

							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S3_ShadowHand1);

							hands[6]->SetEnable(true);
							hands[9]->SetEnable(true);

							polySelection[6]->Show(false);
							polySelection[9]->Show(false);

							polySelection[6]->SetAlpha(1.0f);
							polySelection[9]->SetAlpha(1.0f);

							ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced = true;
							Hud->InvBox_StartPuzzleMode();
						}
						else
						{
							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							Hud->ShowBannerText("L1one");
						}
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L1_S4_PZDoor::ProcessNonHOHint()
{
	K_LOG("Hint!!!!!!!!!");
	int iNonHOHint = 0; 
	int MaxNonHOHints = 4;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			
			case 0: ++iNonHOHint;
					K_LOG("Hint!!!!!!!!!Case0");
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced   && ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken && ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S3_ShadowHand1)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectKeyset.GetCenterX(), InvDestRectKeyset.GetCenterY()-200,0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 1: ++iNonHOHint;
					K_LOG("Hint!!!!!!!!!Case0");
					if(  !ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened   && ProfileSystem::Instance->ActiveProfile.L1S4_P2IsKeySetPlaced)
					{
							InitHintTrailEmit(keyOnScene->Pos.x,keyOnScene->Pos.y,true);
							CPoint tempPoint (InvDestRectKeyset.GetCenterX(), InvDestRectKeyset.GetCenterY()-200,0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 2: ++iNonHOHint;
					K_LOG("Hint!!!!!!!!!Case0");
					if(  !ProfileSystem::Instance->ActiveProfile.L1S4_P2IsKeySetPlaced   && ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken && ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_KeySet)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectKeyset.GetCenterX(), InvDestRectKeyset.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 3: ++iNonHOHint;
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

void L1_S4_PZDoor::ResetPuzzle()
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
		
			if(!ProfileSystem::Instance->ActiveProfile.L1S4_P2IsShadowPuzHandDoorOpened)
			{
				for (int i = 0; i < 5; i++)
				{
					ProfileSystem::Instance->ActiveProfile.L1S4_P2IsImagesFound[i] = false;
				}
			}
			else
			{
				if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced && !ProfileSystem::Instance->ActiveProfile.L1S4_P2IsKeySetPlaced)
				{	
					std::vector<int> vecInvIds;
					Hud->InvBoxCtrl->Activate(1);			
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_ShadowHand1] = true;
					vecInvIds.push_back(eInv_L1S3_ShadowHand1);

					Hud->InvBoxCtrl->Activate(1);			
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_ShadowHand2] = true;
					vecInvIds.push_back(eInv_L1S4_ShadowHand2);
					Hud->ShowInventory(&vecInvIds);

					ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced = false;

					ProfileSystem::Instance->ActiveProfile.L1S3_IsShadowPuzzleSolved = false;

					for (int i = 0; i < 5; i++)
					{
						ProfileSystem::Instance->ActiveProfile.L1S4_P2IsHandFixed[i] = false;
					}					
				}				
				else if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsKeySetPlaced)
				{
					ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced = true;
					ProfileSystem::Instance->ActiveProfile.L1S3_IsShadowPuzzleSolved = true;
				}				
			}

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_PZDoor;
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

void L1_S4_PZDoor::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);

                Hud->InvBox_StopPuzzleMode();
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