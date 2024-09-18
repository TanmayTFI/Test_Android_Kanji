//====================================
// DQFULP
// Hidden Object System
// Author : Faisal
// LEVEL3 - SCENE3 : PZUnderwater
//====================================

#include "L3_S3_PZUnderwater.h"
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

const CPoint		CenterPoint(683,334,0);

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CPoint		PopCloseRectBtnPoint(998,98,0);
const CRectangle  	PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);

const CRectangle  ButtonLeftPinkRect(201.0f,479.0f,247.0f,105.0f);
const CRectangle  ButtonCenterRedRect(597.0f,465.0f,232.0f,114.0f);
const CRectangle  ButtonRightBlueRect(938.0f,467.0f,244.0f,115.0f);

const CRectangle  InfoRectWater(200.0f,584.0f,1023.0f,137.0f);

const CRectangle  LeftBlockRect(227.0f,74.0f,276.0f,306.0f);
const CRectangle  RightBlockRect(882.0f,97.0f,278.0f,251.0f);
const CRectangle  DestRectPuzzleBlocks(185.0f,11.0f,1007.0f,562.0f);

const CRectangle  PuzBlockRect1(250.0f,119.0f,224.0f,208.0f);
const CRectangle  PuzBlockRect2(610.0f,114.0f,201.0f,203.0f);
const CRectangle  PuzBlockRect3(920.0f,126.0f,199.0f,190.0f);

const CRectangle  SceneRiseUpRect(3.0f,0.0f,1361.0f,87.0f);

//Info Rects - Just for information showing
const CRectangle  InfoRectOxygenBar(579.0f,573.0f,221.0f,68.0f);


//Video Rects
const CRectangle  VidRectFull(0,0,1366,768);

const CRectangle  Vid2BgRectUWLoop(0,0,1366,768);

// const int FIRSTFRAME = 0;  //Sac: Unused variable, so commented it
const int FWDANIM	 = 1;
const int REVANIM	 = 2;
const int ANIM_LASTFRAME = 3;

const int COLOR_PINK = 0;
const int COLOR_RED = 1;
const int COLOR_BLUE = 2;

const bool CorrectPinkOuter[8] = {true,false,false,false,false,true,false,true};
const bool CorrectPinkDiag [8] = {false,false,false,true,true,false,false,false};
const bool CorrectPinkInner[4] = {false,false,false,false};

const bool CorrectRedOuter[8] = {false,true,false,false,false,false,false,true};
const bool CorrectRedDiag [8] = {false,false,false,false,false,true,false,false};
const bool CorrectRedInner[4] = {false,false,false,true};

const bool CorrectBlueOuter[8] = {true,false,false,false,false,false,false,false};
const bool CorrectBlueDiag [8] = {false,false,false,false,false,false,false,true};
const bool CorrectBlueInner[4] = {false,true,true,false};

const float MaxOxyTimer = 40.f;

L3_S3_PZUnderwater::L3_S3_PZUnderwater()
{
	IsGameWon = false;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S3\\P1\\P1.SCN");
	musicFadeOutTimer = 0;
	meshTopWater = NULL;
	meshTopWater = new MeshObject(HOScene->GetObjectByID("watermesh"), 10, 5);
	meshTopWater->meshFX->SetSpeedAndDisplacement(4, 2, 4, 2);

	SymbolGlowBlue = HOScene->GetObjectByID("symbGlowBlue");
	SymbolGlowPink = HOScene->GetObjectByID("symbGlowPink");
	SymbolGlowRed = HOScene->GetObjectByID("symbGlowRed");

	SymbolGlowBlue->SetBlendMode(eBlend_AlphaAdditive);
	SymbolGlowPink->SetBlendMode(eBlend_AlphaAdditive); 
	SymbolGlowRed->SetBlendMode(eBlend_AlphaAdditive);

	SymbolGlowBlue->Show(false);
	SymbolGlowPink->Show(false);
	SymbolGlowRed->Show(false);

	PinkGlowObjs.push_back(HOScene->GetObjectByID("topbtnpinkglow"));
	//pipeglows
	PinkGlowObjs.push_back(HOScene->GetObjectByID("toppinkpipeglow1"));
	PinkGlowObjs.push_back(HOScene->GetObjectByID("toppinkpipeglow2"));

	BlueGlowObjs.push_back(HOScene->GetObjectByID("topbtnblueglow"));
	//pipeglows
	BlueGlowObjs.push_back(HOScene->GetObjectByID("topbluepipeglow1"));
	BlueGlowObjs.push_back(HOScene->GetObjectByID("topbluepipeglow2"));
	BlueGlowObjs.push_back(HOScene->GetObjectByID("topbluepipeglow3"));
	BlueGlowObjs.push_back(HOScene->GetObjectByID("topbluepipeglow4"));

	RedGlowObjs.push_back(HOScene->GetObjectByID("topbtnredglow"));
	//pipeglows
	RedGlowObjs.push_back(HOScene->GetObjectByID("topredpipeglow1"));
	RedGlowObjs.push_back(HOScene->GetObjectByID("topredpipeglow2"));
	RedGlowObjs.push_back(HOScene->GetObjectByID("topredpipeglow3"));

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

    int size = (int) PinkGlowObjs.size(); //Sac: Implicit to explicit 
	for (int i = 0; i < size; i++)
	{
		PinkGlowObjs.at(i)->SetBlendMode(eBlend_AlphaAdditive);
	}
	size = (int) BlueGlowObjs.size();  //Sac: Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		BlueGlowObjs.at(i)->SetBlendMode(eBlend_AlphaAdditive);
	}
	size = (int) RedGlowObjs.size();  //Sac: Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		RedGlowObjs.at(i)->SetBlendMode(eBlend_AlphaAdditive);
	}

	SetButtonGlow(COLOR_RED, false);
	SetButtonGlow(COLOR_PINK, false);
	SetButtonGlow(COLOR_BLUE, false);

	s3bubbleRFX = new SuperFX("PRT\\L3\\s3bubbleR.INI", PointSystem::CreateCPoint(1108,666,0), 100); 
	s3bubbleRFX->Start();
	s3bubbleRFX->AdvanceByTime(2.0f);
	s3bubbleRFX->Show(false);

	s3bubbleLFX = new SuperFX("PRT\\L3\\s3bubbleR.INI", PointSystem::CreateCPoint(150,666,0), 100); 
	s3bubbleLFX->Start();
	s3bubbleLFX->AdvanceByTime(2.0f);
	s3bubbleLFX->Show(false);

	//Init Key states in HO SCENE
	activeColorId = COLOR_PINK;
	SaveKeyStatesUpdateTop(false);
	activeColorId = COLOR_BLUE;
	SaveKeyStatesUpdateTop(false);
	activeColorId = COLOR_RED;
	SaveKeyStatesUpdateTop(false);

	IsScreenScrolling = false;
	IsDivingToWater = false;
	IsRisingToSurface = false;

	IsUnderwater = false;

	isSkippedUnderwater = false;

	activeColorId = COLOR_PINK;

	UWScene = new CHOScene();
	UWScene->Load("LV3\\S3\\P1\\UW.SCN");
	UWScene->SaveXY();

	meshUGWater = NULL;
	meshUGWater = new MeshObject(UWScene->GetObjectByID("underwatermesh"), 8, 8);
	meshUGWater->meshFX->SetSpeedAndDisplacement(3, 2, 2, 2);
	meshUGWater->SetEnable(false);

	//shift after setting mesh
	UWScene->ShiftXY(0,768);

	UWOxyBar = UWScene->GetObjectByID("uwoxybar");
	UWOxyRedLight = UWScene->GetObjectByID("uwoxyredglow");
	UWOxyBar->Paused = true;
	UWOxyRedLight->Show(false);

	OxyTimer = MaxOxyTimer;

	//init Underwater scene
	UWScene->GetObjectByID("PinkKeyDiag1")->SetRotate(90);
	UWScene->GetObjectByID("PinkKeyDiag3")->SetRotate(-90);
	UWScene->GetObjectByID("PinkKeyDiag4")->SetRotate(-90);
	UWScene->GetObjectByID("PinkKeyDiag6")->SetRotate(90);
	UWScene->GetObjectByID("PinkKeyStraight2")->SetRotate(90);
	UWScene->GetObjectByID("PinkKeyStraight3")->SetRotate(90);
	UWScene->GetObjectByID("PinkKeyStraight6")->SetRotate(-90);
	UWScene->GetObjectByID("PinkKeyStraight7")->SetRotate(-90);
	UWScene->GetObjectByID("PinkKeyInside0")->SetRotate(90);
	UWScene->GetObjectByID("PinkKeyInside2")->SetRotate(-90);

	UWScene->GetObjectByID("BlueKeyDiag1")->SetRotate(90);
	UWScene->GetObjectByID("BlueKeyDiag3")->SetRotate(-90);
	UWScene->GetObjectByID("BlueKeyDiag4")->SetRotate(-90);
	UWScene->GetObjectByID("BlueKeyDiag6")->SetRotate(90);
	UWScene->GetObjectByID("BlueKeyStraight2")->SetRotate(90);
	UWScene->GetObjectByID("BlueKeyStraight3")->SetRotate(90);
	UWScene->GetObjectByID("BlueKeyStraight6")->SetRotate(-90);
	UWScene->GetObjectByID("BlueKeyStraight7")->SetRotate(-90);
	UWScene->GetObjectByID("BlueKeyInside0")->SetRotate(90);
	UWScene->GetObjectByID("BlueKeyInside2")->SetRotate(-90);

	UWScene->GetObjectByID("RedKeyDiag1")->SetRotate(90);
	UWScene->GetObjectByID("RedKeyDiag3")->SetRotate(-90);
	UWScene->GetObjectByID("RedKeyDiag4")->SetRotate(-90);
	UWScene->GetObjectByID("RedKeyDiag6")->SetRotate(90);
	UWScene->GetObjectByID("RedKeyStraight2")->SetRotate(90);
	UWScene->GetObjectByID("RedKeyStraight3")->SetRotate(90);
	UWScene->GetObjectByID("RedKeyStraight6")->SetRotate(-90);
	UWScene->GetObjectByID("RedKeyStraight7")->SetRotate(-90);
	UWScene->GetObjectByID("RedKeyInside0")->SetRotate(90);
	UWScene->GetObjectByID("RedKeyInside2")->SetRotate(-90);

	for (int i = 0; i < 8; i++)
	{
		UWScene->GetObjectByID(std::string("PinkKeyStraight")+MKSTR(i))->Show(false);		
		UWScene->GetObjectByID(std::string("BlueKeyStraight")+MKSTR(i))->Show(false);
		UWScene->GetObjectByID(std::string("RedKeyStraight")+MKSTR(i))->Show(false);
		OuterKeyState[i] = false;
	}
	
	for (int i = 0; i < 8; i++)
	{
		UWScene->GetObjectByID(std::string("PinkKeyDiag")+MKSTR(i))->Show(false);
		UWScene->GetObjectByID(std::string("BlueKeyDiag")+MKSTR(i))->Show(false);
		UWScene->GetObjectByID(std::string("RedKeyDiag")+MKSTR(i))->Show(false);
		DiagonalKeyState[i] = false;
	}
	
	for (int i = 0; i < 4; i++)
	{
		UWScene->GetObjectByID(std::string("PinkKeyInside")+MKSTR(i))->Show(false);
		UWScene->GetObjectByID(std::string("BlueKeyInside")+MKSTR(i))->Show(false);
		UWScene->GetObjectByID(std::string("RedKeyInside")+MKSTR(i))->Show(false);
		InnerKeyState[i] = false;
	}

	UWScene->GetObjectByID("uwpinkgems1")->Show(false);
	UWScene->GetObjectByID("uwpinkgems2")->Show(false);
	UWScene->GetObjectByID("uwpinkgems3")->Show(false);

	UWScene->GetObjectByID("uwredgems1")->Show(false);
	UWScene->GetObjectByID("uwredgems2")->Show(false);
	UWScene->GetObjectByID("uwredgems3")->Show(false);

	OuterRects[0] = new CRectangle(489.0f,112.0f,59.0f,39.0f);
	OuterRects[1] = new CRectangle(825.0f,118.0f,46.0f,31.0f);
	OuterRects[2] = new CRectangle(882.0f,157.0f,30.0f,48.0f);
	OuterRects[3] = new CRectangle(882.0f,467.0f,35.0f,48.0f);
	OuterRects[4] = new CRectangle(827.0f,524.0f,51.0f,31.0f);
	OuterRects[5] = new CRectangle(493.0f,523.0f,52.0f,32.0f);
	OuterRects[6] = new CRectangle(452.0f,466.0f,32.0f,50.0f);
	OuterRects[7] = new CRectangle(454.0f,158.0f,33.0f,45.0f);

	OuterBackRects[0] = new CRectangle(614.0f,121.0f,42.0f,28.0f);
	OuterBackRects[1] = new CRectangle(718.0f,121.0f,43.0f,26.0f);
	OuterBackRects[2] = new CRectangle(879.0f,264.0f,36.0f,42.0f);
	OuterBackRects[3] = new CRectangle(884.0f,360.0f,33.0f,46.0f);
	OuterBackRects[4] = new CRectangle(720.0f,525.0f,42.0f,28.0f);
	OuterBackRects[5] = new CRectangle(611.0f,528.0f,46.0f,27.0f);
	OuterBackRects[6] = new CRectangle(454.0f,367.0f,33.0f,41.0f);
	OuterBackRects[7] = new CRectangle(450.0f,267.0f,37.0f,40.0f);

	DiagRects[0] = new CRectangle(491.0f,154.0f,40.0f,36.0f);
	DiagRects[1] = new CRectangle(491.0f,283.0f,40.0f,40.0f);
	DiagRects[2] = new CRectangle(843.0f,152.0f,34.0f,32.0f);
	DiagRects[3] = new CRectangle(840.0f,285.0f,37.0f,38.0f);
	DiagRects[4] = new CRectangle(845.0f,484.0f,34.0f,37.0f);
	DiagRects[5] = new CRectangle(841.0f,348.0f,40.0f,37.0f);
	DiagRects[6] = new CRectangle(488.0f,484.0f,38.0f,36.0f);
	DiagRects[7] = new CRectangle(491.0f,353.0f,37.0f,33.0f);

	DiagBackRects[0] = new CRectangle(625.0f,275.0f,40.0f,40.0f);
	DiagBackRects[1] = new CRectangle(619.0f,153.0f,46.0f,42.0f);
	DiagBackRects[2] = new CRectangle(706.0f,275.0f,38.0f,41.0f);
	DiagBackRects[3] = new CRectangle(703.0f,152.0f,41.0f,39.0f);
	DiagBackRects[4] = new CRectangle(706.0f,355.0f,42.0f,44.0f);
	DiagBackRects[5] = new CRectangle(705.0f,482.0f,41.0f,41.0f);
	DiagBackRects[6] = new CRectangle(627.0f,354.0f,40.0f,41.0f);
	DiagBackRects[7] = new CRectangle(630.0f,486.0f,38.0f,37.0f);

	InnerRects[0] = new CRectangle(669.0f,266.0f,33.0f,38.0f);
	InnerRects[1] = new CRectangle(830.0f,324.0f,41.0f,24.0f);
	InnerRects[2] = new CRectangle(670.0f,365.0f,32.0f,39.0f);
	InnerRects[3] = new CRectangle(498.0f,326.0f,41.0f,25.0f);

	InnerBackRects[0] = new CRectangle(667.0f,159.0f,34.0f,53.0f);
	InnerBackRects[1] = new CRectangle(709.0f,318.0f,49.0f,35.0f);
	InnerBackRects[2] = new CRectangle(670.0f,464.0f,31.0f,50.0f);
	InnerBackRects[3] = new CRectangle(613.0f,317.0f,48.0f,33.0f);

	prevPosTop = 0;
	prevPosUW = 768;

	ScrollTimer = 0;
	ScrollTime = 2.5f;

	WinParticleFX = new SuperFX();
	WinParticleFX->LoadEffectIni("PRT\\L3\\RisingFx.INI");
	WinParticleFX->SetPos(PointSystem::CreateCPoint(704,630,0));
	WinParticleFX->SetZ(100);
	//WinParticleFX->Start();

	IsWinningFXActive = false;
	winFXTimer = 0;
	
	if(ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced)
	{
		HOScene->GetObjectByID("leftEmpty")->Show(false);
		HOScene->GetObjectByID("rightEmpty")->Show(false);

		Hud->InvBox_StartPuzzleMode();
	}

	GFInstance->PostUpdate(2);

	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s2Maze,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s2Maze) )		
	{
		Control::Audio->PlaySample(aAmbL3s2Maze,true);
	}
	
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L3_S3_PZUnderwater]  = true;
}

//_PUZZLE
void L3_S3_PZUnderwater::InitPuzzle()
{
	
}

L3_S3_PZUnderwater::~L3_S3_PZUnderwater()
{
	if(IsUnderwater)
	{
		SaveKeyStatesUpdateTop(true);
	}

	SAFE_DELETE(meshTopWater);
	SAFE_DELETE(meshUGWater);

	SAFE_DELETE(s3bubbleRFX);
	SAFE_DELETE(s3bubbleLFX);
	SAFE_DELETE(WinParticleFX);
	
	SAFE_DELETE(ResetDia);

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced)
		Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(UWScene);

	for (int i = 0; i < 8; i++)
	{

		SAFE_DELETE(DiagBackRects[i]);
	}
	for (int i = 0; i < 8; i++)
	{

		SAFE_DELETE(DiagRects[i]);
	}
	for (int i = 0; i < 8; i++)
	{

		SAFE_DELETE(OuterBackRects[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		SAFE_DELETE(OuterRects[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(InnerBackRects[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(InnerRects[i]);
	}

	

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s3UnderwaterPuzzle);
	Control::Audio->UnloadSample(aAmbL3s3UnderwaterPuzzle);
	//MUSIC_SFX
}

void L3_S3_PZUnderwater::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL3theme) )
	{
		Control::Audio->Samples[aTrackL3theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL3theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL3theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL3puzzle) )
				  Control::Audio->PlaySample(aTrackL3puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL3puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL3puzzle, true);
		}
	}
}

void L3_S3_PZUnderwater::Update()
{
	
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	CPoint pos = Control::Input->Mpos();	
	Cursor::SetMode(CA_Normal);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced)
	{
		if(DestRectPuzzleBlocks.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked() && !Hud->IsDraggingItem())
			{
				Hud->ShowBannerText("L3square");
			}
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
		return;
	}

	if(IsWinningFXActive)
	{
		winFXTimer += Control::LogicRate;
		if(winFXTimer>2.5f)
		{
			ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_Pool;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			winFXTimer = 0;
		}
		return;
	}

	if(IsScreenScrolling)
	{
		ScrollTimer += Control::LogicRate;
		if(IsDivingToWater)
		{
			//Hoscene to up
			float newPos = (0 + (-768.0f)*easing(ScrollTimer,0,1,ScrollTime));
			//++float _newPos = (-768.0f) * (ScrollTimer / ScrollTime);
			HOScene->ShiftXY(0,newPos);
			//_newPos = 768 + ((-768.0f) * (ScrollTimer / ScrollTime));
			newPos = (768 + (-768)*easing(ScrollTimer,0,1,ScrollTime));
			UWScene->ShiftXY(0,newPos);

			if(ScrollTimer>ScrollTime)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZGoingUnderwaterFirstTime)
				{
					ProfileSystem::Instance->ActiveProfile.L3_S3_PZGoingUnderwaterFirstTime = true;
					Hud->ShowBannerText("L3breath");
				}

				IsDivingToWater = false;
				IsScreenScrolling = false;

				meshUGWater->SetEnable(true);
				s3bubbleRFX->Show(true);
				s3bubbleLFX->Show(true);

				IsUnderwater = true;

				ScrollTimer = 0;
			}
		}
		else if(IsRisingToSurface)
		{
			//Hoscene to down
			float newPos = (-768 + (768)*easing(ScrollTimer,0,1,ScrollTime));
			//float _newPos = -768 + ((768.0f) * (ScrollTimer / ScrollTime));
			HOScene->ShiftXY(0,newPos);
			newPos = (0 + (768)*easing(ScrollTimer,0,1,ScrollTime));
			//_newPos = (768.0f) * (ScrollTimer / ScrollTime);
			UWScene->ShiftXY(0,newPos);

			if(ScrollTimer>ScrollTime)
			{
				HideKeys();

				IsDivingToWater = false;
				IsScreenScrolling = false;

				IsUnderwater = false;

				meshTopWater->SetEnable(true);

				bool isGameWon = CheckIfGameWon();
				if(isGameWon || isSkippedUnderwater)
				{
					if(isGameWon)
						Hud->ShowBannerText("L3hard");

					SymbolGlowBlue->Show(true);
					SymbolGlowPink->Show(true);
					SymbolGlowRed->Show(true);
					AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					if(_handleExtrasGameComplete())
						return;

					Control::Audio->QuickLoadAndPlaySFX("s3pz2_puzzlesolved");
					WinParticleFX->Start();
					IsWinningFXActive = true;
				}

				ScrollTimer = 0;
			}
		}
		return;
	}

	if(IsUnderwater)
	{
		OxyTimer -= Control::LogicRate;
		if(OxyTimer<=0)
		{
			Hud->ShowBannerText("L3surface");
			//Rise Up
			RiseupFromUnderwater();
		}
		else
		{
			int frame = (int)floorf((OxyTimer/MaxOxyTimer)*48.0f);
			UWOxyBar->SetFrame(frame);

			if(frame<18)
				UWOxyRedLight->Show(true);
		}
		//Underwater Code Goes Here
		if( SceneRiseUpRect.Intersection(&pos) )
		{
			Cursor::SetMode(CA_ExitUp,-1);
			if( Control::Input->LBclicked() )
			{
				RiseupFromUnderwater();
			}	
		}

		if( InfoRectOxygenBar.Intersection(&pos) )
		{
			Cursor::SetMode(CA_Help);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L3underwater");				
			}
			return;
		}

		for (int i = 0; i < 8; i++)
		{
			if(!OuterKeyState[i])
			{
				//Not activated
				if(OuterRects[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked() )
					{
						Control::Audio->QuickLoadAndPlaySFX("s3pz2_arrowclicked");
						OuterKeys[i]->PlayAnimation(FWDANIM);
						OuterKeyState[i] = true;
					}
					break;
				}
			}
			else
			{
				//Activated Click To go back
				if(OuterBackRects[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked() )
					{			
						Control::Audio->QuickLoadAndPlaySFX("s3pz2_arrowclicked");
						OuterKeys[i]->PlayAnimation(REVANIM);
						OuterKeyState[i] = false;
					}
					break;
				}
			}

			if(!DiagonalKeyState[i])
			{
				//Not activated
				if(DiagRects[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked() )
					{
						Control::Audio->QuickLoadAndPlaySFX("s3pz2_arrowclicked");
						DiagonalKeys[i]->PlayAnimation(FWDANIM);
						DiagonalKeyState[i] = true;
					}
					break;
				}
			}
			else
			{
				//Activated Click To go back
				if(DiagBackRects[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked() )
					{		
						Control::Audio->QuickLoadAndPlaySFX("s3pz2_arrowclicked");
						DiagonalKeys[i]->PlayAnimation(REVANIM);
						DiagonalKeyState[i] = false;
					}
					break;
				}
			}

			if (i<4)
			{
				if(!InnerKeyState[i])
				{
					//Not activated
					if(InnerRects[i]->Intersection(&pos))
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked() )
						{
							Control::Audio->QuickLoadAndPlaySFX("s3pz2_arrowclicked");
							InnerKeys[i]->PlayAnimation(FWDANIM);
							InnerKeyState[i] = true;
						}
						break;
					}
				}
				else
				{
					//Activated Click To go back
					if(InnerBackRects[i]->Intersection(&pos))
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked() )
						{			
							Control::Audio->QuickLoadAndPlaySFX("s3pz2_arrowclicked");
							InnerKeys[i]->PlayAnimation(REVANIM);
							InnerKeyState[i] = false;
						}
						break;
					}
				} 
			}
		}
	}
	else
	{
		if( PuzBlockRect1.Intersection(&pos) || PuzBlockRect2.Intersection(&pos) || PuzBlockRect3.Intersection(&pos))
		{
			Cursor::SetMode(CA_Help);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L3pattern");
			}
		}

		//Surface code goes here
		if( ButtonCenterRedRect.Intersection(&pos) )
		{
			SetButtonGlow(COLOR_RED, true);			
			Cursor::SetMode(CA_HandPointy);
			if( Control::Input->LBclicked() )
			{
				StartDivingUnderwater(COLOR_RED);
			}
		}
		else
			SetButtonGlow(COLOR_RED, false);

		if( ButtonLeftPinkRect.Intersection(&pos) )
		{
			SetButtonGlow(COLOR_PINK, true);
			Cursor::SetMode(CA_HandPointy);
			if( Control::Input->LBclicked() )
			{
				StartDivingUnderwater(COLOR_PINK);
			}
		}
		else
			SetButtonGlow(COLOR_PINK, false);

		if( ButtonRightBlueRect.Intersection(&pos) )
		{
			SetButtonGlow(COLOR_BLUE, true);
			Cursor::SetMode(CA_HandPointy);
			if( Control::Input->LBclicked() )
			{
				StartDivingUnderwater(COLOR_BLUE);
			}
		}
		else
			SetButtonGlow(COLOR_BLUE, false);
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
	else
	{
		/*
		if( !IsUnderwater && InfoRectWater.Intersection(&pos) )
			Hud->ShowBannerText("L3under");
			*/
	}
}

void L3_S3_PZUnderwater::SetButtonGlow(int colorID, bool enable)
{
	int size = 0;
	if(colorID == COLOR_RED)
	{
		size = (int) RedGlowObjs.size();  //Sac: Implicit to explicit
		for (int i = 0; i < size; i++)
		{
			RedGlowObjs.at(i)->SetEnable(enable);
		}
	}
	else if(colorID == COLOR_PINK)
	{
		size = (int) PinkGlowObjs.size();  //Sac: Implicit to explicit
		for (int i = 0; i < size; i++)
		{
			PinkGlowObjs.at(i)->SetEnable(enable);
		}
	}
	else if(colorID == COLOR_BLUE)
	{
		size = (int) BlueGlowObjs.size();  //Sac: Implicit to explicit
		for (int i = 0; i < size; i++)
		{
			BlueGlowObjs.at(i)->SetEnable(enable);
		}
	}
	
}

void L3_S3_PZUnderwater::RiseupFromUnderwater()
{
	IsScreenScrolling = true;

	s3bubbleRFX->Show(false);
	s3bubbleLFX->Show(false);

	SaveKeyStatesUpdateTop(true);

	meshUGWater->SetEnable(false);

	Control::Audio->QuickLoadAndPlaySFX("s3pz2_moveoverwater");
		
	IsRisingToSurface = true;
}

void L3_S3_PZUnderwater::StartDivingUnderwater(int colorID)
{
	meshTopWater->SetEnable(false);

	IsScreenScrolling = true;
	HOScene->SaveXY();

	OxyTimer = MaxOxyTimer;
	UWOxyBar->SetFrame(47);
	UWOxyRedLight->Show(false);

	Control::Audio->QuickLoadAndPlaySFX("s3pz2_moveunderwater");

	IsDivingToWater = true;

	activeColorId = colorID;
	SetupUnderWaterScene();
}

void L3_S3_PZUnderwater::SaveKeyStatesUpdateTop(bool saveState)
{
	//Save States
	//BLUE No Rotation
	if(activeColorId==COLOR_BLUE)
	{
		if (saveState)
		{
			for (int i = 0; i < 8; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueOuterKeyState[i] = OuterKeyState[i];
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueDiagonalKeyState[i] = DiagonalKeyState[i];
				if(i<4)
					ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueInnerKeyState[i] = InnerKeyState[i]; 
			}
		}
		for (int i = 0; i < 8; i++)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueOuterKeyState[i])
				HOScene->GetObjectByID(std::string("blueOuterGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("blueOuterGlow")+MKSTR(i))->Show(true);

			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueDiagonalKeyState[i])
				HOScene->GetObjectByID(std::string("blueDiagGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("blueDiagGlow")+MKSTR(i))->Show(true);

			if(i<4)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueInnerKeyState[i])
					HOScene->GetObjectByID(std::string("blueInnerGlow")+MKSTR(i))->Show(false);
				else
					HOScene->GetObjectByID(std::string("blueInnerGlow")+MKSTR(i))->Show(true); 
			}
		}	
	}
	else if(activeColorId==COLOR_PINK)
	{
		//Rotation Discarded
		if (saveState)
		{
			for (int i = 0; i < 8; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkOuterKeyState[i] = OuterKeyState[i];
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkDiagonalKeyState[i] = DiagonalKeyState[i];
				if(i<4)
					ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkInnerKeyState[i] = InnerKeyState[i]; 
			}
		}
		for (int i = 0; i < 8; i++)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkOuterKeyState[i])
				HOScene->GetObjectByID(std::string("pinkOuterGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("pinkOuterGlow")+MKSTR(i))->Show(true);

			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkDiagonalKeyState[i])
				HOScene->GetObjectByID(std::string("pinkDiagGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("pinkDiagGlow")+MKSTR(i))->Show(true);

			if(i<4)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkInnerKeyState[i])
					HOScene->GetObjectByID(std::string("pinkInnerGlow")+MKSTR(i))->Show(false);
				else
					HOScene->GetObjectByID(std::string("pinkInnerGlow")+MKSTR(i))->Show(true); 
			}
		}


		/*
		//Rotate Left By 90 degree - Now discarded rotID = i
		for (int i = 0; i < 8; i++)
		{
			int rotId = i;
			int rotId = i+2;
			if(rotId>7)
				rotId = rotId-8;
			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkOuterKeyState[rotId])
				HOScene->GetObjectByID(std::string("pinkOuterGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("pinkOuterGlow")+MKSTR(i))->Show(true);

			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkDiagonalKeyState[rotId])
				HOScene->GetObjectByID(std::string("pinkDiagGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("pinkDiagGlow")+MKSTR(i))->Show(true);

			rotId = i+1;
			if(rotId>3)
				rotId = rotId-4;
			if(i<4)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkInnerKeyState[rotId])
					HOScene->GetObjectByID(std::string("pinkInnerGlow")+MKSTR(i))->Show(false);
				else
					HOScene->GetObjectByID(std::string("pinkInnerGlow")+MKSTR(i))->Show(true);
			}
		}*/
	}

	else if(activeColorId==COLOR_RED)
	{
		if (saveState)
		{
			for (int i = 0; i < 8; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedOuterKeyState[i] = OuterKeyState[i];
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedDiagonalKeyState[i] = DiagonalKeyState[i];
				if(i<4)
					ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedInnerKeyState[i] = InnerKeyState[i]; 
			}
		}
		//Rotation Discarded
		for (int i = 0; i < 8; i++)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedOuterKeyState[i])
				HOScene->GetObjectByID(std::string("redOuterGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("redOuterGlow")+MKSTR(i))->Show(true);

			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedDiagonalKeyState[i])
				HOScene->GetObjectByID(std::string("redDiagGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("redDiagGlow")+MKSTR(i))->Show(true);

			if(i<4)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedInnerKeyState[i])
					HOScene->GetObjectByID(std::string("redInnerGlow")+MKSTR(i))->Show(false);
				else
					HOScene->GetObjectByID(std::string("redInnerGlow")+MKSTR(i))->Show(true); 
			}
		}

		/*
		//Rotate Right By 90 degree
		for (int i = 0; i < 8; i++)
		{
			int rotId = i-2;
			if(rotId<0)
				rotId = 8+rotId;
			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedOuterKeyState[rotId])
				HOScene->GetObjectByID(std::string("redOuterGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("redOuterGlow")+MKSTR(i))->Show(true);

			if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedDiagonalKeyState[rotId])
				HOScene->GetObjectByID(std::string("redDiagGlow")+MKSTR(i))->Show(false);
			else
				HOScene->GetObjectByID(std::string("redDiagGlow")+MKSTR(i))->Show(true);

			rotId = i-1;
			if(rotId<0)
				rotId = 4+rotId;
			if(i<4)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedInnerKeyState[rotId])
					HOScene->GetObjectByID(std::string("redInnerGlow")+MKSTR(i))->Show(false);
				else
					HOScene->GetObjectByID(std::string("redInnerGlow")+MKSTR(i))->Show(true);
			}
		}*/
	}
}

bool L3_S3_PZUnderwater::CheckIfGameWon()
{
	bool gameWon = true;
	for (int i = 0; i < 8; i++)
	{
		int rotId = i; //Rotation Cancelled
		/*int rotId = i+2;
		if(rotId>7)
			rotId = rotId-8;*/
		if( ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkOuterKeyState[rotId] != CorrectPinkOuter[i] ||
			 ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkDiagonalKeyState[rotId] != CorrectPinkDiag[i])
		{
			gameWon = false;
			break;
		}

		rotId = i; //Rotation Cancelled
		/*rotId = i-2;
		if(rotId<0)
			rotId = 8+rotId;*/
		if( ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedOuterKeyState[rotId] != CorrectRedOuter[i] ||
			 ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedDiagonalKeyState[rotId] != CorrectRedDiag[i])
		{
			gameWon = false;
			break;
		}

		if( ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueOuterKeyState[i] != CorrectBlueOuter[i] ||
			 ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueDiagonalKeyState[i] != CorrectBlueDiag[i])
		{
			gameWon = false;
			break;
		}

		if(i<4)
		{
			/*rotId = i+1;
			if(rotId>3)
				rotId = rotId-4;*/
			if( ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkInnerKeyState[rotId] != CorrectPinkInner[i] )
			{
				gameWon = false;
				break;
			}

			/*rotId = i-1;
			if(rotId<0)
				rotId = 4+rotId;*/
			if( ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedInnerKeyState[rotId] != CorrectRedInner[i] )
			{
				gameWon = false;
				break;
			}

			if( ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueInnerKeyState[i] != CorrectBlueInner[i])
			{
				gameWon = false;
				break;
			}
		}
	}
	return gameWon;
}

void L3_S3_PZUnderwater::HideKeys()
{
	for (int i = 0; i < 8; i++)
	{
		OuterKeys[i]->Show(false);
		DiagonalKeys[i]->Show(false);
		if(i<4)
		{
			InnerKeys[i]->Show(false);
		}
	}
}

void L3_S3_PZUnderwater::SetupUnderWaterScene()
{
	//Setup UW Scene
	for (int i = 0; i < 8; i++)
	{
		if(activeColorId==COLOR_PINK)//pink
		{
			OuterKeys[i] = UWScene->GetObjectByID(std::string("PinkKeyStraight")+MKSTR(i));
			OuterKeyState[i] = ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkOuterKeyState[i];
		}
		else if(activeColorId==COLOR_RED)//Red
		{
			OuterKeys[i] = UWScene->GetObjectByID(std::string("RedKeyStraight")+MKSTR(i));
			OuterKeyState[i] = ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedOuterKeyState[i];
		}
		else if(activeColorId==COLOR_BLUE)//Blue
		{
			OuterKeys[i] = UWScene->GetObjectByID(std::string("BlueKeyStraight")+MKSTR(i));
			OuterKeyState[i] = ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueOuterKeyState[i];
		}
		OuterKeys[i]->Show(true);
		if(OuterKeyState[i])
			OuterKeys[i]->PlayAnimation(ANIM_LASTFRAME);
	}
	
	for (int i = 0; i < 8; i++)
	{
		if(activeColorId==COLOR_PINK)//pink
		{
			DiagonalKeys[i] = UWScene->GetObjectByID(std::string("PinkKeyDiag")+MKSTR(i));
			DiagonalKeyState[i] = ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkDiagonalKeyState[i];
		}
		else if(activeColorId==COLOR_RED)//Red
		{
			DiagonalKeys[i] = UWScene->GetObjectByID(std::string("RedKeyDiag")+MKSTR(i));
			DiagonalKeyState[i] = ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedDiagonalKeyState[i];
		}
		else if(activeColorId==COLOR_BLUE)//Blue
		{
			DiagonalKeys[i] = UWScene->GetObjectByID(std::string("BlueKeyDiag")+MKSTR(i));
			DiagonalKeyState[i] = ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueDiagonalKeyState[i];
		}

		DiagonalKeys[i]->Show(true);
		if(DiagonalKeyState[i])
			DiagonalKeys[i]->PlayAnimation(ANIM_LASTFRAME);
	}
	
	for (int i = 0; i < 4; i++)
	{
		if(activeColorId==COLOR_PINK)//pink
		{
			InnerKeys[i] = UWScene->GetObjectByID(std::string("PinkKeyInside")+MKSTR(i));
			InnerKeyState[i] = ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkInnerKeyState[i];
		}
		else if(activeColorId==COLOR_RED)//Red
		{
			InnerKeys[i] = UWScene->GetObjectByID(std::string("RedKeyInside")+MKSTR(i));
			InnerKeyState[i] = ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedInnerKeyState[i];
		}
		else if(activeColorId==COLOR_BLUE)//Blue
		{
			InnerKeys[i] = UWScene->GetObjectByID(std::string("BlueKeyInside")+MKSTR(i));
			InnerKeyState[i] = ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueInnerKeyState[i];
		}

		InnerKeys[i]->Show(true);
		if(InnerKeyState[i])
			InnerKeys[i]->PlayAnimation(ANIM_LASTFRAME);
	}

	UWScene->GetObjectByID("uwsymbolPink")->Show(false);
	UWScene->GetObjectByID("uwsymbolRed")->Show(false);
	UWScene->GetObjectByID("uwsymbolBlue")->Show(false);

	if(activeColorId==COLOR_PINK)
		UWScene->GetObjectByID("uwsymbolPink")->Show(true);
	else if(activeColorId==COLOR_RED)
		UWScene->GetObjectByID("uwsymbolRed")->Show(true);
	else if(activeColorId==COLOR_BLUE)
		UWScene->GetObjectByID("uwsymbolBlue")->Show(true);

	UWScene->GetObjectByID("uwredgems1")->Show(false);
	UWScene->GetObjectByID("uwredgems2")->Show(false);
	UWScene->GetObjectByID("uwredgems3")->Show(false);

	UWScene->GetObjectByID("uwpinkgems1")->Show(false);
	UWScene->GetObjectByID("uwpinkgems2")->Show(false);
	UWScene->GetObjectByID("uwpinkgems3")->Show(false); 

	if(activeColorId==COLOR_RED)
	{
		UWScene->GetObjectByID("uwredgems1")->Show(true);
		UWScene->GetObjectByID("uwredgems2")->Show(true);
		UWScene->GetObjectByID("uwredgems3")->Show(true);
	}
	else if (activeColorId==COLOR_PINK)
	{
		UWScene->GetObjectByID("uwpinkgems1")->Show(true);
		UWScene->GetObjectByID("uwpinkgems2")->Show(true);
		UWScene->GetObjectByID("uwpinkgems3")->Show(true);
	}
}

void L3_S3_PZUnderwater::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			if( itemVal == eInv_L3S2_PuzzleBlock1Left)
			{
				if( DestRectPuzzleBlocks.Intersection(&dropPos) )
				{
					//if both blocks are collected
					if( ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[0] > -1 && ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[1] > -1)
					{
						Control::Audio->QuickLoadAndPlaySFX("s3pz2_tilesfix");

						ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced = true;

						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
						
						//Clear Inventory. One Time Use Items.
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_PuzzleBlock1Left] = false;
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_PuzzleBlock2Right] = false;

						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_PuzzleBlock1Left);

						HOScene->GetObjectByID("leftEmpty")->Show(false);
						HOScene->GetObjectByID("rightEmpty")->Show(false);

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
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L3_S3_PZUnderwater::ProcessSkip()
{
	if(!IsWinningFXActive)
	{
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);

		if(IsUnderwater)
		{
			RiseupFromUnderwater();
			isSkippedUnderwater = true;
		}
		else
		{
			SymbolGlowBlue->Show(true);
			SymbolGlowPink->Show(true);
			SymbolGlowRed->Show(true);

			Control::Audio->QuickLoadAndPlaySFX("s3pz2_puzzlesolved");

			WinParticleFX->Start();
			IsWinningFXActive = true;
		}
	}
}

float L3_S3_PZUnderwater::easing(float t,float b,float c,float d)
{
	float s = 1.70158f;
	// Sac: this is a problematic function, should the s across be s*1.525f or only in the place where it is multipled ??
	if ((t/=d/2) < 1)
    {
        s*=(1.525f);
        return c/2*(t*t*(((s)+1)*t - s)) + b;
    }
    float postFix = t-=2;
    s*=(1.525f);
	return c/2*((postFix)*t*(((s)+1)*t + s) + 2) + b;
}

void L3_S3_PZUnderwater::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 2;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced )
					{
						if(ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[0] > -1 && ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[1] > -1)
						{
							Hud->ShowHintForInvItem(eInv_L3S2_PuzzleBlock1Left, &DestRectPuzzleBlocks);
						}
						else
						{
							InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						}
						return;
					}
					break;
			case 1: ++iNonHOHint;
					return;
					break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L3_S3_PZUnderwater::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced)
	{
		ResetBtn->SetEnable(false);
		return;
	}
	ResetBtn->SetEnable(true);
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
			ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved= false;

			for (int i = 0; i < 8; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkOuterKeyState[i] = false;
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedOuterKeyState[i] = false;
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueOuterKeyState[i] = false;
			}
			for (int i = 0; i < 8; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkDiagonalKeyState[i] = false;
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedDiagonalKeyState[i] = false;
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueDiagonalKeyState[i] = false;
			}
			for (int i = 0; i < 4; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZPinkInnerKeyState[i] = false;
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZRedInnerKeyState[i] = false;
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlueInnerKeyState[i] = false;
			}

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_PZUnderwater;
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
void L3_S3_PZUnderwater::NavigateToScene()
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