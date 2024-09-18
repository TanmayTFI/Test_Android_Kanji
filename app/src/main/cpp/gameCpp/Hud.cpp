//====================================
// Framework
// Hud Control
//====================================

#include "Hud.h"
#include "HOScene.h"
#include "BitmapText.h"
#include "Util.h"
#include "Banners.h"
#include "Fonts.h"
#include "FoundInvPopup.h"
#include "InventoryBox.h"
#include "Point.h"
#include "MenuSystem.h"
#include "ProfileSystem.h"
#include "ParticleEffect.h"
#include "Application.h"
#include "IGMConstants.h"
#include "GameFlow.h"
#include "GameTextManager.h"
#include "exRandom.h"
#include "Point.h"
#include "Award.h"
#include "AwardManager.h"
#include "AssetsStringTable.h"
#define MAX_HINT_FRAMES 32.0f

const CRectangle  MusicSliderAreaRect(528.0f,182.0f,316.0f,40.0f);
const CRectangle  SFXSliderAreaRect(528.0f,245.0f,319.0f,39.0f);
const CRectangle  VoiceSliderAreaRect(528.0f,303.0f,319.0f,41.0f);

const CRectangle  WidescreenCheckRect(727.0f,515.0f,50.0f,42.0f);
const CRectangle  FullscreenCheckRect(723.0f,465.0f,56.0f,41.0f);
const CRectangle  CustomCursorCheckRect(722.0f,415.0f,54.0f,43.0f);
const CRectangle  ModeChangeTextRect(702.0f,354.0f,164.0f,42.0f);

const CRectangle  BtnRectMenu(183.0f,717.0f,123.0f,49.0f);
const CRectangle  BtnRectGuide(1040.0f,732.0f,93.0f,33.0f);
const CRectangle  BtnRectHint(1041.0f,617.0f,131.0f,108.0f);
const CRectangle  BtnRectSkip(1041.0f,617.0f+30,131.0f,108.0f-30);
const CRectangle  InvBoxLockRect(551,635,38,38);
const CRectangle IBLeftArrRect(355,695,41,35);
const CRectangle IBRightArrRect(973,695,40,35);
const CRectangle InvBoxRect(366,630,632,108+30);
const CPoint InvBoxDownPos(683.0f, 846.0f+25, 0.0f);

#define MAX_HUD_PANEL_POINTS 10
int HudX[MAX_HUD_PANEL_POINTS] = {134,173,295,335,547,  586,771,798,1173,1231};
int HudY[MAX_HUD_PANEL_POINTS] = {768,625,623,657,660,  630,631,657,650,768};

#ifdef _CEBUILD
	#define COLLECTIBLE_VERT_GAP 176.0f
#endif
	

const std::string HudElementIDs[HE_Max] = 
{
	"btnmenu",	//HE_Menu
	"btnmap",	//HE_Map
	"btnhint",	//HE_Hint
	"btnskip",	//HE_Skip
	"btndiary", //HE_Journal
	"btnguide",	//HE_Guide
	"BNR"		//HE_MessageBanner
};

	#define CUSTOMKNOB_STARTX 778.0f
	#define CUSTOMKNOB_ENDX   960.0f

	#define HINTKNOB_POSY	471.0f
	#define SKIPKNOB_POSY	577.0f

	const CRectangle  CustHintSliderRect(755.0f,452.0f,227.0f,35.0f);
	const CRectangle  CustSkipSliderRect(763.0f,559.0f,214.0f,36.0f);

	const CRectangle  CustRectTutor(635.0f,382.0f,39.0f,36.0f);
	const CRectangle  CustRectSparkles(634.0f,425.0f,40.0f,32.0f);
	const CRectangle  CustRectHOSparkles(634.0f,461.0f,40.0f,35.0f);
	const CRectangle  CustRectTextTips(632.0f,502.0f,44.0f,36.0f);
	const CRectangle  CustRectAwardNotify(631.0f,542.0f,45.0f,36.0f);

	const CRectangle  CustRectHintTick(846.0f,384.0f,42.0f,35.0f);
	const CRectangle  CustRectSkipTick(845.0f,487.0f,44.0f,38.0f);

	const CRectangle  TeleAreaRect(537.0f,213.0f,332.0f,390.0f);
	const CRectangle  TeleCloseBtnRect(840.0f,161.0f,85.0f,69.0f);

	const float HintTimesArray[10] = {5.0f,15.0f,30.0f,45.0f,60.0f,75.0f,90.0f,120.0f,135.0f,150.0f};

	//Mode Select Stuffs
	const CPoint ModeBoxPos(212, 130.0f, 0.0f);
	const CPoint ModeTextsOff(460.0f, 266.0f, 0.0f);
	const float  ModeTextsYSpacing = 45.0f;

	const std::string ErrorSameProfileName = "Exname";

	const std::string ModeText_Casual = "Excasual";
	const std::string ModeText_Advanced = "Exadvanced";
	const std::string ModeText_Expert = "Exexpert";
	const std::string ModeText_Custom = "";


const std::string AwardTextArray[kAward_Max] = 
{
	"Exachieveme",				//0 DRONA
	"Exhop1", 	//1 NA SUCITA 1
	"Exhop3", 		//32 NA SUCITA 2
	"Exhop5",			//33 NA SUCITA 3
	"Exhop10",		//34 NA SUCITA 4
	"Exdown",			//2 Arjuna
	"Ex150ho",			//3 Vayu
	"Ex90ho",			//3
	"Ex30ho",			//3
	"Exmatchho",				//4 Sama kanta
	"Ex2p",			//5 Agni
	"Ex4p",			//5
	"Ex6p",			//5
	"Exless",			//6 GURU
	"Exjigsaw",	//7 NIRMOKTR
	"Exsecond",			//8 PRASPHURANA
	"Exmini", //9 CHANAKYA
	"Exusing",	//10 MA CARA
	"Exngame",		//11 HOP PATI
	"Exnskip",	//12 TARTARITI
	"Ex10hr",			//13 KARMA
	"Exn4",	//13 
	"Exjourney",									//14 PRARAMBHA
	"Exnexp",			//15 DEVARUPIN
	"Exncreek",	//16 AVARA CARA
	"Exfast",							//17 SIDDHI
	"Exgot",						//18 UDGHATIN
	"Exn33",										//19 UPALAMBHAKA
	"Exn1",			//20 SITA
	"Exnall",			//21 MATR RAKSA
	"Ex1morph",			//22 DRASTTA
	"Exallmorph",		//23 GARUDA
	"Extutorial",		//24 ADHIMUKTI
	"Exntab",				//25 ZISAVA
	"Exunveil",				//26 PATHAKA
	"Exwreck",		//27 KAUSHIK
	"Exindus",				//28 PATHIKA
	"Exsave",					//29 RAKSHAK
	"Extame",		//30 DAMAKA
	"Exndec",		//31 VIGRAHA
	"Exsoldier"		//35 Ajna Chakra
};

void HudControl::InvBox_StartPuzzleMode()
{
	AwardManager::GetInstance()->StartMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	//Save prev Hint time
	ProfileSystem::Instance->ActiveProfile.HintSavedTime = (int)(HintVert*10000);

	IsHintFullyCharged = false; //if hint is fully charged. Do not carry it to skip.

	//Get the previous Skip timer
	HintVert = ProfileSystem::Instance->ActiveProfile.SkipTimer[ProfileSystem::Instance->ActiveProfile.CurrentArea]/10000.0f;

	pCockHeadIdle->SetEnable(true);
	pCockHeadWakeIdle->SetEnable(false);

	//Gradual Hint Increase
	float finalHintVert = HintVert;
	float hintInc = HintVert/10;

	HintVert = hintInc;
	//Hint Update works in 30, 60, 90 degrees. So Instant change will cause peacock anim update not working properly
	while (HintVert<finalHintVert)
	{
		HintVert += hintInc;
		UpdateHint();
	}

	HintVert = finalHintVert;
	UpdateHint();

	IsPuzzleScene = true;
	InvBoxCtrl->SetLockBase(PUZBOX_FRAME);//lock in HO anim	
	
	Elements[HE_Hint]->SetEnable(false);
	hintText->SetEnable(false);
	Elements[HE_Skip]->SetEnable(true);
	skipText->SetEnable(true);

	Elements[HE_Hint]->Show(false);
	Elements[HE_Skip]->Show(true);
	skipText->Show(true);
	hintText->Show(false);

	HintRechargeWarnText->SetText(StringTable::getStr(TextSkipRechargeWarning));
}

void HudControl::InvBox_StopPuzzleMode()
{
	//save the current scene skip time
	ProfileSystem::Instance->ActiveProfile.SkipTimer[ProfileSystem::Instance->ActiveProfile.CurrentArea] = (int)(HintVert*10000);
	
	//When a puzzle is solved. Change the Hud.
	IsPuzzleScene = false;

	InvBoxCtrl->SetLockBase(INVBOX_LR_FRAME);
	
	InvBoxCtrl->HidePuzzleTexts();
	InvBoxCtrl->InitInvBoxUpIdle();

	Elements[HE_Hint]->SetEnable(true);
	hintText->SetEnable(true);
	Elements[HE_Skip]->SetEnable(false);
	skipText->SetEnable(false);

	Elements[HE_Hint]->Show(true);
	Elements[HE_Skip]->Show(false);
	skipText->Show(false);
	hintText->Show(true);

	//restore hint time
	RestartHintFX();

	HintRechargeWarnText->SetText(StringTable::getStr(TextHintRechargeWarning));
}

void HudControl::HideHud(bool fading)
{
	isHUDVisible = false;

	HintRechargeTextBg->Show(false);
	HintRechargeWarnText->Show(false);
	if(!fading)
	{
		InvBoxCtrl->HideInvBox(false);
		HudScene->Show(false);

		//Handle localized text
		skipText->SetEnable(false);
		hintText->SetEnable(false);
		menuText->SetEnable(false);
		guideText->SetEnable(false);
		mapText->SetEnable(false);

		//Handle black base during bannerdisplay!!
		if( MessageText->Visible )
			Elements[HE_MessageBanner]->Show(true);
	}
	else
	{
		InvBoxCtrl->HideInvBox(true);
		HudScene->FadeOut(8.0);
		//Handle localized text
		skipText->SetEnable(false);
		hintText->SetEnable(false);
		menuText->SetEnable(false);
		guideText->SetEnable(false);
		mapText->SetEnable(false);
	}

#ifdef _CEBUILD

	if(extrasBtnText)
        extrasBtnText->Show(false);

    if(achBtnText)
        achBtnText->Show(false);

#endif // _CEBUILD
}

void HudControl::ShowHud(bool fading)
{
	isHUDVisible = true;
	

	if(fading)
	{
		HudScene->FadeIn(8.0);
		InvBoxCtrl->ShowInvBox(true);

		//Handle localized text
		skipText->SetEnable(true);
		hintText->SetEnable(true);
		menuText->SetEnable(true);
		guideText->SetEnable(true);
		mapText->SetEnable(true);		
	}
	else
	{
		HudScene->Show(true);

#if defined (K_ANDROID) && !defined(UNLOCKED)
		showADorBuyDialog(false, adType);
		#endif // K_ANDROID

		//Handle localized text
		skipText->SetEnable(true);
		hintText->SetEnable(true);
		menuText->SetEnable(true);
		guideText->SetEnable(true);
		mapText->SetEnable(true);

#ifndef _CEBUILD //SE BUILD
		HudScene->GetObjectByID("btnguide")->Show(false);
		HudScene->GetObjectByID("btnguide")->SetEnable(false);
#endif
		InvBoxCtrl->ShowInvBox(false);
	}
	Elements[HE_MessageBanner]->Show(false);
	MessageText->Show(false);

	Elements[HE_Hint]->SetEnable(!IsPuzzleScene);
	Elements[HE_Skip]->SetEnable(IsPuzzleScene);
	skipText->SetEnable(IsPuzzleScene);
	hintText->SetEnable(!IsPuzzleScene);

	Elements[HE_Hint]->Show(!IsPuzzleScene);
	Elements[HE_Skip]->Show(IsPuzzleScene);
	skipText->Show(IsPuzzleScene);
	hintText->Show(!IsPuzzleScene);
}

HudControl::HudControl(int InventorySize)
{
	GFHud = this;

	HOState = HOInactive;

	IsHintRechargeInstant = false;
	IsPuzzleScene = false;
	GFInstance->IsInGameMenuPopup = false;

	IsHOScene = false;
	IsTakeAndUseHOScene = false;
	isHUDVisible = true;
	isMultipleItemsFlag = false;
	InvBoxCtrl = NULL;

	HintVert = ProfileSystem::Instance->ActiveProfile.HintSavedTime/10000.0f;
	PrevCursorMode = -1;

	TransOutFactor = 1.0f;	

	AllowInput = true;

	//HUD Scene Elements
	HudScene = GFInstance->LoadBufferScene(HO_HUD_SCN);
	if(HudScene->GetObjectByID("Leftbg")->ZOrder<2)
	{
		HudScene->SetZOff(eZOrder_HUD);		
	}
	HudScene->GetObjectByID("BNR")->SetZ(eZOrder_Banner);
	
	//Localized
	/*
	CObject* tempTxt = new CObject();
	tempTxt->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\GUIDE.SPR", HudScene->GetObjectByID("btnguide")->Pos.x, HudScene->GetObjectByID("btnguide")->Pos.y, HudScene->GetObjectByID("btnguide")->ZOrder+1);
	tempTxt->SetUID("btnguidetext");
	HudScene->PushObject(tempTxt);

	tempTxt = new CObject();
	tempTxt->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\HINT.SPR", HudScene->GetObjectByID("btnhint")->Pos.x, HudScene->GetObjectByID("btnhint")->Pos.y, HudScene->GetObjectByID("btnhint")->ZOrder + 1);
	tempTxt->SetUID("btnhinttext");
	HudScene->PushObject(tempTxt);

	tempTxt = new CObject();
	tempTxt->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\MAP.SPR", HudScene->GetObjectByID("btnmap")->Pos.x, HudScene->GetObjectByID("btnmap")->Pos.y, HudScene->GetObjectByID("btnmap")->ZOrder + 1);
	tempTxt->SetUID("btnmaptext");
	HudScene->PushObject(tempTxt);

	tempTxt = new CObject();
	tempTxt->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\MENU.SPR", HudScene->GetObjectByID("btnmenu")->Pos.x, HudScene->GetObjectByID("btnmenu")->Pos.y, HudScene->GetObjectByID("btnmenu")->ZOrder + 1);
	tempTxt->SetUID("btnmenutext");
	HudScene->PushObject(tempTxt);

	tempTxt = new CObject();
	tempTxt->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\SKIP.SPR", HudScene->GetObjectByID("btnskip")->Pos.x, HudScene->GetObjectByID("btnskip")->Pos.y, HudScene->GetObjectByID("btnskip")->ZOrder + 1);
	tempTxt->SetUID("btnskiptext");
	HudScene->PushObject(tempTxt);
	*/
	
	SceneData menuTxt;
	menuTxt.objType = "Object";
	menuTxt.uid = "btnmenutext";
	menuTxt.spriteString = "LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\MENU.SPR";
	menuTxt.spriteAnim = 0;
	menuTxt.x = HudScene->GetObjectByID("btnmenu")->Pos.x;
	menuTxt.y = HudScene->GetObjectByID("btnmenu")->Pos.y;
	menuTxt.ZOrder = HudScene->GetObjectByID("btnmenu")->ZOrder + 1;

	menuText = HudScene->CreateObject(menuTxt);

	SceneData guideTxt;
	guideTxt.objType = "Object";
	guideTxt.uid = "btnguidetext";
	guideTxt.spriteString = "LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\GUIDE.SPR";
	guideTxt.spriteAnim = 0;
	guideTxt.x = HudScene->GetObjectByID("btnguide")->Pos.x;
	guideTxt.y = HudScene->GetObjectByID("btnguide")->Pos.y;
	guideTxt.ZOrder = HudScene->GetObjectByID("btnguide")->ZOrder + 1;

	guideText = HudScene->CreateObject(guideTxt);

	SceneData hintTxt;
	hintTxt.objType = "Object";
	hintTxt.uid = "btnhinttext";
	hintTxt.spriteString = "LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\HINT.SPR";
	hintTxt.spriteAnim = 0;
	hintTxt.x = HudScene->GetObjectByID("btnhint")->Pos.x;
	hintTxt.y = HudScene->GetObjectByID("btnhint")->Pos.y;
	hintTxt.ZOrder = HudScene->GetObjectByID("btnhint")->ZOrder + 1;

	hintText = HudScene->CreateObject(hintTxt);

	SceneData skipTxt;
	skipTxt.objType = "Object";
	skipTxt.uid = "btnskiptext";
	skipTxt.spriteString = "LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\SKIP.SPR";
	skipTxt.spriteAnim = 0;
	skipTxt.x = HudScene->GetObjectByID("btnskip")->Pos.x;
	skipTxt.y = HudScene->GetObjectByID("btnskip")->Pos.y;
	skipTxt.ZOrder = HudScene->GetObjectByID("btnskip")->ZOrder + 1;

	skipText = HudScene->CreateObject(skipTxt);

	SceneData mapTxt;
	mapTxt.objType = "Object";
	mapTxt.uid = "btnmaptext";
	mapTxt.spriteString = "LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\MAP.SPR";
	mapTxt.spriteAnim = 0;
	mapTxt.x = HudScene->GetObjectByID("btnmap")->Pos.x;
	mapTxt.y = HudScene->GetObjectByID("btnmap")->Pos.y;
	mapTxt.ZOrder = HudScene->GetObjectByID("btnmap")->ZOrder + 1;

	mapText = HudScene->CreateObject(mapTxt);

    HudScene->Show(true);//since turning off in garbagecollector!!

#ifdef  K_ANDROID

    adViewDialogScene = new CHOScene();

	noOfHintsText = new CBitmapText();
    noOfHintsText->SetPos(PointSystem::CreateCPoint(1320, 30, 0));
    noOfHintsText->SetZ(eZOrder_HUD + 100);
    noOfHintsText->SetAlignment(Align_Center);
    noOfHintsText->LoadFont(eFont_25);
    noOfHintsText->SetText(to_string(GFApp->noOfHints));

#ifdef  UNLOCKED
    noOfHintsText->Show(false);
#endif

    adViewDialog = new CObject();
	adViewDialog->LoadInit("UI\\W1.SPR", 685, 300, eZOrder_HUD + 50);
	adViewDialog->SetUID("adViewDialog");
    adViewDialogScene->PushObject(adViewDialog);
    adViewDialog->Show(false);

	viewADButton = new CObject();
	viewADButton->LoadInit("MNU\\ADDPROF\\BTNOK.SPR", 550, 370, eZOrder_HUD + 51);
	viewADButton->SetUID("viewADButton");
    adViewDialogScene->PushObject(viewADButton);
    viewADButton->Show(false);

	buyHintsButton = new CObject();
	buyHintsButton->LoadInit("MNU\\ADDPROF\\BTNOK.SPR", 820, 370, eZOrder_HUD + 52);
	buyHintsButton->SetUID("buyHintsButton");
    adViewDialogScene->PushObject(buyHintsButton);
    buyHintsButton->Show(false);

    adViewDialogText = new MultilineTextBox("Buy or view AD", 685, 250, eZOrder_HUD + 100, ColorRGB{1,1,1}, eFont_25, 25);
    adViewDialogText->CenterText();

    adBtnText = new CBitmapText();
    adBtnText->SetPos(PointSystem::CreateCPoint(550, 370, 0));
    adBtnText->SetZ(eZOrder_HUD + 100);
    adBtnText->SetAlignment(Align_Center);
    adBtnText->LoadFont(eFont_25);
    adBtnText->SetText("AD");

    buyBtnText = new CBitmapText();
    buyBtnText->SetPos(PointSystem::CreateCPoint(820, 370, 0));
    buyBtnText->SetZ(eZOrder_HUD + 100);
    buyBtnText->SetAlignment(Align_Center);
    buyBtnText->LoadFont(eFont_25);
    buyBtnText->SetText("Buy");

    showADorBuyDialog(false, 0);

#endif

#ifndef _CEBUILD //SE BUILD
	HudScene->GetObjectByID("btnguide")->Show(false);
	HudScene->GetObjectByID("btnguide")->SetEnable(false);
#endif

	HintChargeVFX = GFInstance->LoadBufferSuperFX(HINTCHARGED_SUPERFX);
	HintChargeVFX->StopImmediate();


	//Hint stuffs
	HintTrailTime = 0.0f;
	IsHintTrailRunning = false;
	IsDirectionalHint = false;
	IsHintTrailCircling = false;

	MemoryTrailTime = 0;
	IsMemoryFXTrailing = false;
	memoryStartPt.SetXY(258,650);

	hintStartPt.SetXY(1090,660);
	
	HintTrailCirclingTime =0;

	paptusFX = NULL;
	paptusglowFX = NULL;

	MemoryTrailSuperFX = NULL;
	MemoryTrailSoftSuperFX = NULL;
	MemoryExplodeSuperFX = NULL;

	HintTrailSuperFX = GFInstance->LoadBufferSuperFX(HINTTRAIL_SUPERFX);
	HintTrailSoftSuperFX = GFInstance->LoadBufferSuperFX(HINTTRAILSOFT_SUPERFX);

	HintCircleSuperFX = GFInstance->LoadBufferSuperFX(HINTCIRCLE_SUPERFX);
	HintExplodeSuperFX = GFInstance->LoadBufferSuperFX(HINTEXPLODE_SUPERFX);

	HintGreenCircleFX = GFInstance->LoadBufferSuperFX(HINTGREENCIRCLE_SUPERFX);

	HintDirCursor = GFInstance->LoadBufferObject(HINT_NAVIG_CURSOR_OBJ);
	HintDirCursor->Show(false);

	Hint2FX = HintGreenCircleFX;

	//Hint peacock
	pCockFeathers[0] = HudScene->GetObjectByID("pcockfeatherC");
	pCockFeathers[1] = HudScene->GetObjectByID("pcockfeatherL0");
	pCockFeathers[2] = HudScene->GetObjectByID("pcockfeatherL1");
	pCockFeathers[3] = HudScene->GetObjectByID("pcockfeatherL2");
	pCockFeathers[4] = HudScene->GetObjectByID("pcockfeatherR0");
	pCockFeathers[5] = HudScene->GetObjectByID("pcockfeatherR1");
	pCockFeathers[6] = HudScene->GetObjectByID("pcockfeatherR2");

	for (int i = 0; i < NUM_PCOCK_FEATHERS; i++)
		pCockFeathers[i]->SetPivot(0, 50);
	
	pCockHeadIdle = HudScene->GetObjectByID("pcockidle");
	pCockHeadWakeup = HudScene->GetObjectByID("pcockwakeup");
	pCockHeadWakeIdle = HudScene->GetObjectByID("pcockwakeidle");

	isPCockWakeupPlaying = false;
	isPCockWakeDownPlaying = false;

	pCockHeadIdle->SetEnable(true); //just in case it appears missing
	pCockHeadWakeIdle->SetEnable(false);
	pCockHeadWakeup->SetEnable(false);

	hintMaxAngle = 90.0f;
	hintAngle = HintVert*hintMaxAngle;

	orbFX = NULL;

	if(!ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked)
	{
		HudScene->GetObjectByID("orb")->SetEnable(false);
	}
	else
	{
		HudScene->GetObjectByID("orb")->SetEnable(true);
	}

	HudScene->GetObjectByID("orb")->ForcePlayAnimation(0);

	//Cache Items
	for(int i=0; i < HE_Max; ++i)	
		Elements[i] = HudScene->GetObjectByID( HudElementIDs[i] );

	Elements[HE_Hint]->PlayAnimation(2);//disabled state
	Elements[HE_Skip]->PlayAnimation(2);//disabled state

	Elements[HE_Hint]->SetEnable(true);
	Elements[HE_Skip]->SetEnable(false);
	skipText->SetEnable(false);
	hintText->SetEnable(true);

	Elements[HE_Hint]->Show(true);
	Elements[HE_Skip]->Show(false);
	skipText->Show(false);
	hintText->Show(true);

	//Hint is recharging message box
	HintRechargeTextBg = GFInstance->LoadBufferObject(HINT_RECHARGE_FRAME_BG);
	HintRechargeTextBg->SetEnable(false);

	HintRechargeWarnText = new CBitmapText();	
	HintRechargeWarnText->SetZ(Elements[HE_Hint]->ZOrder+22);
	HintRechargeWarnText->SetAlignment(Align_Center);
	HintRechargeWarnText->LoadFont(eFont_25);
	HintRechargeWarnText->SetText(StringTable::getStr(TextSkipRechargeWarning));
	int temp = HintRechargeWarnText->StringWidth();
	HintRechargeWarnText->SetText(StringTable::getStr(TextHintRechargeWarning));
	HintRechargeWarnText->SetColor(239.0f/255.0f,228.0f/255.0f,176.0f/255.0f);
	HintRechargeWarnText->SetScale(0.9f);
	// Sac: Added this below code to adjust the Hint Recharge BG dynamically based on text width	
	int hintRechargeWarnTxtWidth = max(temp, HintRechargeWarnText->StringWidth());	
						   if( hintRechargeWarnTxtWidth <= 85 )
						   {
							   HintRechargeTextBg->PlayAnimation(0);//-51
						   }
						   else if( hintRechargeWarnTxtWidth <= 100 )//9=>+3
						   {
							   HintRechargeTextBg->PlayAnimation(1);//-60

						   }
						   else if( hintRechargeWarnTxtWidth <= 125 )//14=>+4
						   {
							   HintRechargeTextBg->PlayAnimation(2);//-74
						   }
						   else if( hintRechargeWarnTxtWidth <= 146 )//9=>+3
						   {
							   HintRechargeTextBg->PlayAnimation(3);//-83
						   }
						   else if( hintRechargeWarnTxtWidth <= 178 )//14=>+4
						   {
							   HintRechargeTextBg->PlayAnimation(4);//-97
						   }
						   else if( hintRechargeWarnTxtWidth <= 190 )//8=>+3
						   {
							   HintRechargeTextBg->PlayAnimation(5);//-105
						   }
						   else if( hintRechargeWarnTxtWidth <= 211 )//10=>+3
						   {
							   HintRechargeTextBg->PlayAnimation(6);//-115
						   }
						   else if( hintRechargeWarnTxtWidth <= 220 )//10=>+3
						   {
							   HintRechargeTextBg->PlayAnimation(7);//-115
						   }
						   else //~220 and all++ - //4=>+1
						   {
							   HintRechargeTextBg->PlayAnimation(8);//-119
						   }


	HintRechargeWarnText->Show(false);
	
	IsRechargeWarningShowing = false;

	int mapPts[] = {260,619,311,624,331,647,306,696,267,688,251,646};
	BtnPolyMap=  new CPolygon(6);
	BtnPolyMap->Init(mapPts);

	int diaryPts[] = {160,626,232,613,269,692,269,704,193,719,181,711};
	BtnPolyDiary=  new CPolygon(6);
	BtnPolyDiary->Init(diaryPts);

	int orbPts[] = {300,630,305,621,275,598,253,595,222,599,200,623,202,629,257,656};
	crystalOrbPoly=  new CPolygon(8);
	crystalOrbPoly->Init(orbPts);


	DiaryAddVFX = GFInstance->LoadBufferSuperFX(DIARYFX_SUPERFX);

	//HUD Boundary Polygons for Two States
	int hudLockedPts[] = {112,768,164,621,255,605,332,641,341,657,569,654,601,631,775,634,805,659,1008,656,1051,599,1159,592,1261,768};
	hudLockedPoly=  new CPolygon(13);
	hudLockedPoly->Init(hudLockedPts);

	int hudUnlockedPts[] = {112,768,162,614,308,613,359,686,355,758,550,755,569,730,777,730,814,760,1012,758,1008,649,1060,592,1175,593,1226,672,1241,768};
	hudMinimizedPoly =  new CPolygon(15);
	hudMinimizedPoly->Init(hudUnlockedPts);

	// Map
	IsMapShowing = false;
	MapControl = new Map();
	AddControl(MapControl);

	//Message Banner
	InitMessageBanner();
	HideMessageBanner();
	
	//Found Inv Popup
	foundInvPopupCtrl = new FoundInvPopup(InventorySize);//Same pos always!! 
	AddControl(foundInvPopupCtrl);

	//Inventory Box
	InvBoxCtrl = new InventoryBox();
	AddControl(InvBoxCtrl);

	if( InvBoxCtrl )
		InvBoxCtrl->InvBox->Show(true);

	// Teleport
	teleportSystem = new TeleportSystem();
	AddControl(teleportSystem);
	//teleportSystem->ShowTeleport(); TODO testing done here
	
	//Journal
	JournalCtrl = new Journal();
	AddControl(JournalCtrl);
	IsDiaryShowing = false;


	//Journal
	GuideCtrl = new StrategyGuide();
	AddControl(GuideCtrl);
	IsGuideShowing = false;

	
	IsHintPaused = false;
	IsHintDischarging = false;
	IsHintFullyCharged = false;

	InitHintAnim();

	//Tutorial init
	Tutor = NULL;
	if( ProfileSystem::Instance->ActiveProfile.TutorialState != TUT_Inactive && !ProfileSystem::Instance->ActiveProfile.IsTutorialDisabled )
	{
		//Tutorial Enabled
		Tutor = new Tutorial();
		//Show any pending tutorial here
	}
	
	MaskSpr = GFInstance->LoadBufferObject(MASK_OBJ);
	MaskSpr->SetPos(&MaskSprPos);
	MaskSpr->SetXY(683,384);
	MaskSpr->SetScale(2.0);
	MaskSpr->SetZ(eZOrder_InGameMenuMask);
	MaskSpr->Show(false);

	ModeMaskSpr = GFInstance->LoadBufferObject(MODEMASK_OBJ);
	ModeMaskSpr->SetPos(&MaskSprPos);
	ModeMaskSpr->SetZ(eZOrder_ModesMenuMask);
	ModeMaskSpr->Show(false);

#ifdef _CEBUILD
	//Morph_Objects
	morphCollectObj = NULL;
	morphDummyObj = NULL;

	isMorphObjectsActive = isMorphObjectVisible = isMorphTutorialActive = false;
	morphTimer = morphFadeTimer = 0;
	isMorphFadingIn = false;
	isMorphFadingOut = false;
	if( GFInstance->IsReplayingGame )
		morphTimerThreshold = (float)Elixir::Rand(REPLAY_MORPHOBJECT_MINTIME, REPLAY_MORPHOBJECT_MAXTIME);
	else 
		morphTimerThreshold = (float)Elixir::Rand(MORPHOBJECT_MINTIME, MORPHOBJECT_MAXTIME);

	collectMorphFrameBG = NULL;
	morphCollectAnimTime = 0;
	isMorphObjectCollected = isMorphCollectibleFlying = isMorphCollectibleUIShowing = false;
	isMouseOverMorphButton = false;
	morphText = NULL;

	//COMMON
	isMouseOverExtraButton = false;

	//Memory_Items
	memoryObjFadeTimer = 0;
	isMemoryObjectCollected = false;
	isMemoryObjectFading = false;

	
	memoryCollectFrameBG = NULL;
	memoryCollectTextBG = NULL;
	isMemoryFlying = false;
	isMemoryUIShowing = false;

	memoryCollectAnimTime = 0;
	MemoryText = NULL;

	isSceneCloseAfterMemoryFade = false;
	isMouseOverMemoryButton = false;

	memoryCollectObj = NULL;
	memoryPreviewObj = NULL;
	memoryCollectItemGlow = NULL;

	//Awards_Items
	isAwardRemovedFromQueue = false;
	awardIcon = NULL;
	isAwardUIShowing = false;
	isAwardUIPaused = false;
	isAwardFadingOut = false;
	isMouseOverAwardButton = false;
	awardCollectAnimTime = 0;
	AwardText = NULL;
	awardCollectFrameBG = NULL;	

	btnGotoMorphButton = NULL;
	btnGotoMemoryButton = NULL;
	btnGotoAwardsButton = NULL;

	achBtnText = NULL;
	extrasBtnText = NULL;
	memoriesBtnText = NULL;
	textBG_Mask = NULL; 

	awardsHideText = NULL;
	awardsHideTick = NULL;

	morphVFX = NULL;
	awardsVFX = NULL;

	currentAwardId = AwardManager::GetInstance()->GetCompletedAwardFromQueue();
	if(currentAwardId != -1)
	{
		ShowAwardAchieved(currentAwardId);
	}
#endif 

	IsComboInvPopupShowing = false;

	comboInvSystemCtrl = new ComboInvSystem();
	AddControl(comboInvSystemCtrl);

	if(ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingPzBagActive)
	{
		IsComboInvPopupShowing = true;
		ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingPzBagActive = false;
		comboInvSystemCtrl->ShowComboInventoryPopup(eInv_L1S1_Bag,(float)ProfileSystem::Instance->ActiveProfile.L1S1_ComboBagInitX,(float)ProfileSystem::Instance->ActiveProfile.L1S1_ComboBagInitY,true);
	}

	AwardManager::GetInstance()->SetAwardInterface(this);

	characterUnlockPopup = NULL;
	characterUnlockPopup = new UnlockPopup(DiaryAddVFX);


	//Mode Select Popup
	iActiveProfileSlot = ProfileSystem::Instance->ProfileSlot;

	SelectModePopupScene = GFInstance->LoadBufferScene(MODESELECT_SCN);	//LOAD BUFFERED
	if(SelectModePopupScene->GetObjectByID("modeBg")->ZOrder<=1)
		SelectModePopupScene->SetZOff(eZOrder_ModeSelectScn);
	SelectModePopupScene->Show(false);

	ModeButtons[0] = SelectModePopupScene->GetObjectByID("modCas");
	ModeButtons[1] = SelectModePopupScene->GetObjectByID("modAdv");
	ModeButtons[2] = SelectModePopupScene->GetObjectByID("modExp");
	ModeButtons[3] = SelectModePopupScene->GetObjectByID("modCus");

	SelectedMode = 0; //Mode already choosen

	HintTimeInt=0;
	SkipTimeInt=0;

	//Custom Mode
	IsHintEnabled=true;
	IsSkipEnabled=true;
	IsTutorEnabled=true;
	IsSparklesEnabled=true;
	IsHOSparklesEnabled=false;
	IsTextTipsEnabled=false;
	IsAwardNotifyEnabled = true;

	IsHintKnobDragStarted = false;
	IsSkipKnobDragStarted = false;

	//Custom Mode Items
	CustMode_ItemsBg = SelectModePopupScene->GetObjectByID("CustModeItems");
	CustMode_HintKnob = SelectModePopupScene->GetObjectByID("HintKnob");
	CustMode_SkipKnob = SelectModePopupScene->GetObjectByID("SkipKnob");
	CustMode_HintTime = SelectModePopupScene->GetObjectByID("HintTime");
	CustMode_SkipTime = SelectModePopupScene->GetObjectByID("SkipTime");

	CustMode_TickTutor = SelectModePopupScene->GetObjectByID("TutorialTick");
	CustMode_TickSparkles = SelectModePopupScene->GetObjectByID("SparklesTick");
	CustMode_TickHOSparkles = SelectModePopupScene->GetObjectByID("HOSparkesTick");
	CustMode_TickTextTips = SelectModePopupScene->GetObjectByID("TextTipsTick");
	CustMode_TickHints = SelectModePopupScene->GetObjectByID("HintTick");
	CustMode_TickSkip = SelectModePopupScene->GetObjectByID("SkipTick");
	CustMode_TickAwardPopup = SelectModePopupScene->GetObjectByID("DisableAwardPopupTick");

	ModeTextsLineWidth = 800;
	CPoint textPos;
	for(int i=0; i < MODE2_TEXT_LINES; ++i)
	{
		textPos = ModeBoxPos;		

		textPos.x += ModeTextsOff.x;//CENTRALIZE
		textPos.y += ModeTextsOff.y + ModeTextsYSpacing * i;

		ModeDescriptionTexts[i] = new CBitmapText();
		ModeDescriptionTexts[i]->SetZ(eZOrder_ModeSelectScn+10);
		ModeDescriptionTexts[i]->LoadFont(eFont_25);		
		ModeDescriptionTexts[i]->SetText("ModeDescriptionTexts Placeholder");
		//ModeDescriptionTexts[i]->SetAlignment(Align_Left);		
		ModeDescriptionTexts[i]->SetPos(&textPos);		
		//ModeDescriptionTexts[i]->SetScale(1.0f);
		ModeDescriptionTexts[i]->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 		
		ModeDescriptionTexts[i]->Show(false);
	}

	modeNameText = new CBitmapText();
	modeNameText->SetAlignment(Align_Left);
	modeNameText->SetZ(eZOrder_ModeSelectScn + 10);
	modeNameText->LoadFont(eFont_26);
	modeNameText->SetText(AssetsStringTable::getString("diffmodeText"));
	modeNameText->SetPos(PointSystem::CreateCPoint(370, 330, 0));
	modeNameText->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 		
	modeNameText->Show(false);

	ModeRects[0] = new CRectangle(341.0f,154.0f,161.0f,193.0f);
	ModeRects[1] = new CRectangle(522.0f,156.0f,153.0f,192.0f);
	ModeRects[2] = new CRectangle(695.0f,156.0f,156.0f,194.0f);
	ModeRects[3] = new CRectangle(872.0f,157.0f,152.0f,196.0f);
	

	IsObjectiveTextShowing = false;
	ObjectiveTextsAlpha = 0.0f;
	ObjectiveCounter = 0;

	IsNewInfoTextShowing = false;
	newInfoTextTimer = 0;

	NewInfoTextBgSpr = NULL;
	NewInfoText = NULL;

	ObjectiveTextBgSpr = GFInstance->LoadBufferObject(HUD_OBJBG_OBJ);

	ObjectiveTextBgSpr->SetPos(PointSystem::CreateCPoint(280.0f+80,570.0f,0));
	ObjectiveTextBgSpr->SetZ(5029);
	ObjectiveTextBgSpr->Show(false);

	ObjectiveTitle = new CBitmapText();
	ColorRGB yellow={(233/255.0f),(204/255.0f),(131/255.0f)};
	ObjectiveTitle->SetTextProps(StringTable::getStr("Exnew"), 293+80, 554, 5030, yellow, eFont_Italic24, Align_Center);
	ObjectiveTitle->Show(false);

	ObjectiveTextDescrip = new CBitmapText();
	ObjectiveTextDescrip->SetTextProps(StringTable::getStr("Exhelp"), 286+80, 584, 5030, yellow, eFont_25, Align_Center);
	ObjectiveTextDescrip->Show(false);

	InGameMenuPopupScene = GFInstance->LoadBufferScene(INGAMEMENU_PSCN);
	if(InGameMenuPopupScene->GetObjectByID("igmbg")->ZOrder<=1)
		InGameMenuPopupScene->SetZOff(eZOrder_InGameMenu);
	InGameMenuPopupScene->Show(false);

	RestartPopup = GFInstance->LoadBufferScene(RESTART_DIA);
	if(RestartPopup->GetObjectByID("restartbg")->ZOrder<=1)
		RestartPopup->SetZOff(eZOrder_InGameMenu+1);
	RestartPopup->Show(false);
	

	for(int i=0; i < IGMB_Max; ++i)
	{
		Buttons[i] = InGameMenuPopupScene->GetObjectByID( IGMButtonID[i] );
		Buttons[i]->PlayAnimation(0);

		if(ProfileSystem::Instance->_isExtrasGamePlay)
		{
			if(i == IGMB_Restart || i == IGMB_Memories)
			{
				Buttons[i]->Show(false);
			}
		}	
	}

	MainMenuPos.x = 687;
	MainMenuPos.y = 519;
	
	ButtonRects[IGMB_Resume] = ResumeRect;
	ButtonRects[IGMB_Options] = OptionsRect;
	ButtonRects[IGMB_Memories] = MemoriesRect;
	ButtonRects[IGMB_MainMenu] = MainMenuRect;
	ButtonRects[IGMB_Restart] = RestartRect;


	OptionsPopupScene = GFInstance->LoadBufferScene(OPTIONS_PSCN);	
	if(OptionsPopupScene->GetObjectByID("optBg")->ZOrder<=1)
		OptionsPopupScene->SetZOff(eZOrder_OptionsScn);
	OptionsPopupScene->Show(false);

	if(OptionsPopupScene->GetObjectByID("langSelectBtn"))
		OptionsPopupScene->GetObjectByID("langSelectBtn")->SetEnable(false);
	if(OptionsPopupScene->GetObjectByID("langSelectBtnTxt"))
		OptionsPopupScene->GetObjectByID("langSelectBtnTxt")->SetEnable(false);


	musicVolTxt = new CBitmapText();
	musicVolTxt->SetAlignment(Align_Center);
	musicVolTxt->SetZ(eZOrder_OptionsScn + 10);
	musicVolTxt->LoadFont(eFont_25);
	musicVolTxt->SetText(AssetsStringTable::getString("musicVolTxt"));
	musicVolTxt->SetPos(PointSystem::CreateCPoint(685, 176, 0));
	musicVolTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 		
	musicVolTxt->Show(false);

	effectsVolTxt = new CBitmapText();
	effectsVolTxt->SetAlignment(Align_Center);
	effectsVolTxt->SetZ(eZOrder_OptionsScn + 10);
	effectsVolTxt->LoadFont(eFont_25);
	effectsVolTxt->SetText(AssetsStringTable::getString("effectsVolTxt"));
	effectsVolTxt->SetPos(PointSystem::CreateCPoint(685, 240, 0));
	effectsVolTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 		
	effectsVolTxt->Show(false);

	voiceVolTxt = new CBitmapText();
	voiceVolTxt->SetAlignment(Align_Center);
	voiceVolTxt->SetZ(eZOrder_OptionsScn + 10);
	voiceVolTxt->LoadFont(eFont_25);
	voiceVolTxt->SetText(AssetsStringTable::getString("voiceVolTxt"));
	voiceVolTxt->SetPos(PointSystem::CreateCPoint(685, 300, 0));
	voiceVolTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 		
	voiceVolTxt->Show(false);

	diffModeTxt = new CBitmapText();
	diffModeTxt->SetAlignment(Align_Right);
	diffModeTxt->SetZ(eZOrder_OptionsScn + 10);
	diffModeTxt->LoadFont(eFont_25);
	diffModeTxt->SetText(AssetsStringTable::getString("diffModeTxt"));
	diffModeTxt->SetPos(PointSystem::CreateCPoint(685, 380, 0));
	diffModeTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 		
	diffModeTxt->Show(false);

	customCurTxt = new CBitmapText();
	customCurTxt->SetAlignment(Align_Right);
	customCurTxt->SetZ(eZOrder_OptionsScn + 10);
	customCurTxt->LoadFont(eFont_25);
	customCurTxt->SetText(AssetsStringTable::getString("customCurTxt"));
	customCurTxt->SetPos(PointSystem::CreateCPoint(685, 440, 0));
	customCurTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 		
	customCurTxt->Show(false);

	fullscreenTxt = new CBitmapText();
	fullscreenTxt->SetAlignment(Align_Right);
	fullscreenTxt->SetZ(eZOrder_OptionsScn + 10);
	fullscreenTxt->LoadFont(eFont_25);
	fullscreenTxt->SetText(AssetsStringTable::getString("fullscreenTxt"));
	fullscreenTxt->SetPos(PointSystem::CreateCPoint(685, 490, 0));
	fullscreenTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 		
	fullscreenTxt->Show(false);

	modeBtnOver = OptionsPopupScene->GetObjectByID("btnover");
	modeBtnOver->SetEnable(false);
	modeBtnOver->SetBlendMode(eBlend_AlphaAdditive);

	PopupAlpha = 1.0f;
	PopupCounter = 0;
	PopupState = Popup_Inactive;
}

void HudControl::FadeInOrb()
{
	HudScene->GetObjectByID("orb")->SetEnable(true);
	HudScene->GetObjectByID("orb")->FadeIn(4.0f);
}

void HudControl::SendOrbFXToObjectPos(CPoint memoryObjectPos)
{
	IsMemoryFXTrailing = true;
	GFInstance->IsActivePopup = true;

	MemoryTrailTime = 0;

	memoryEndPt = memoryObjectPos;

	float MiddleX = (memoryStartPt.x + memoryEndPt.x)/2;
	float MiddleY = (memoryStartPt.y + memoryEndPt.y)/2;

	float angle = 0;
	//Control point will be perpendicular to line. around 200-400 range
	float randValue = rand()%300 + 200.0f;

	MemoryExplodeSuperFX->Pos.x = memoryEndPt.x;
	MemoryExplodeSuperFX->Pos.y = memoryEndPt.y;

	memoryCtrlPt.x = MiddleX + randValue*Elixir::Cos(angle);
	memoryCtrlPt.y = MiddleY - randValue*Elixir::Sin(angle);
}

void HudControl::ShowOrbGlowing()
{
	if(orbFX==NULL)
	{
		orbFX = new SuperFX("PRT\\UI\\orb.INI", PointSystem::CreateCPoint(252,652,0), 1542); 
	}
	
	MemoryTrailSuperFX = new SuperFX("PRT\\UI\\memtrail.INI", PointSystem::CreateCPoint(1110.0f,690.0f,0), eZOrder_HUD+25);
	MemoryTrailSoftSuperFX = new SuperFX("PRT\\UI\\memsofttrail.INI", PointSystem::CreateCPoint(1110.0f,690.0f,0), eZOrder_HUD+25);
	MemoryExplodeSuperFX = new SuperFX("PRT\\UI\\memexplo.INI", PointSystem::CreateCPoint(1110.0f,690.0f,0), eZOrder_HUD+25);

	orbFX->Start();
	HudScene->GetObjectByID("orb")->ForcePlayAnimation(1);
}

void HudControl::HideOrbGlowing()
{
	if(orbFX!=NULL)
		orbFX->StopEmit();

	HudScene->GetObjectByID("orb")->ForcePlayAnimation(0);
}

HudControl::~HudControl()
{
	InvBoxCtrl->AddPendingInvToInvBox(0); //Add pending (flying) inv items to Inventory
	GFHud = NULL;
	ProfileSystem::Instance->ActiveProfile.HintSavedTime = (int)(HintVert*10000);
    if( GFInstance != NULL )
        GFInstance->IsInGameMenuPopup = false;

	SAFE_DELETE(orbFX);

	SAFE_DELETE(MemoryTrailSuperFX);
	SAFE_DELETE(MemoryTrailSoftSuperFX);
	SAFE_DELETE(MemoryExplodeSuperFX);

	SAFE_DELETE(paptusFX);
	SAFE_DELETE(paptusglowFX);

#ifdef K_ANDROID
	SAFE_DELETE(noOfHintsText);
    SAFE_DELETE(adViewDialogText);
    SAFE_DELETE(adBtnText);
    SAFE_DELETE(buyBtnText);
    SAFE_DELETE(adViewDialogScene);
#endif

	//Control deletes Inventory
	//Control deletes Valuable
	//Control deletes InvBoxCtrl
	if( InvBoxCtrl )
		InvBoxCtrl->InvBox->Show(false);

	Control::Audio->UnloadQuickLoadedSFX();

#ifdef _CEBUILD
	//Collectibles		

	//Morph Objects
	morphCollectObj = NULL;
	morphDummyObj = NULL;

	SAFE_DELETE(collectMorphFrameBG);
	SAFE_DELETE(morphText);

	
	SAFE_DELETE(memoryCollectFrameBG);
	SAFE_DELETE(memoryCollectTextBG);
	SAFE_DELETE(memoryCollectObj);
	SAFE_DELETE(memoryPreviewObj);
	SAFE_DELETE(memoryCollectItemGlow);
	SAFE_DELETE(MemoryText);


	SAFE_DELETE(awardIcon);
	SAFE_DELETE(awardCollectFrameBG);
	SAFE_DELETE(AwardText);

	SAFE_DELETE(btnGotoMemoryButton);
	SAFE_DELETE(btnGotoMorphButton);
	SAFE_DELETE(btnGotoAwardsButton);

	SAFE_DELETE(achBtnText);
	SAFE_DELETE(textBG_Mask);
	SAFE_DELETE(extrasBtnText);
	SAFE_DELETE(memoriesBtnText);

	SAFE_DELETE(awardsHideText);
	SAFE_DELETE(awardsHideTick);

	SAFE_DELETE(morphVFX);
	SAFE_DELETE(awardsVFX);
#endif 

	SAFE_DELETE(characterUnlockPopup);

	//Garbage collection in gameflow
	InGameMenuPopupScene->Show(false);
	InGameMenuPopupScene = NULL;

	//Garbage collection in gameflow
	OptionsPopupScene->Show(false);
	OptionsPopupScene = NULL;

	delete musicVolTxt;
	delete effectsVolTxt;
	delete voiceVolTxt;
	delete diffModeTxt;
	delete customCurTxt;
	delete fullscreenTxt;

	SelectModePopupScene->Show(false);
	SelectModePopupScene = NULL;

	for(int i=0; i < MODE2_TEXT_LINES; ++i)	
		delete ModeDescriptionTexts[i];

	HintRechargeTextBg->SetEnable(false);
	HintRechargeTextBg = NULL;
	SAFE_DELETE(HintRechargeWarnText);

	HintChargeVFX->StopImmediate();
	HintChargeVFX = NULL;

	//Garbage collection in gameflow
	MaskSpr->Show(false);
	MaskSpr = NULL;

	//Hint Stuffs
	HintDirCursor->Show(false);
	HintDirCursor = NULL;

	//Garbage collection in gameflow
	Hint2FX = NULL;

	SAFE_DELETE(BtnPolyMap);
	SAFE_DELETE(BtnPolyDiary);
	SAFE_DELETE(crystalOrbPoly);

	SAFE_DELETE(Tutor);

	DiaryAddVFX->StopImmediate();
	DiaryAddVFX = NULL;

	HintTrailSuperFX->StopImmediate();
	HintTrailSuperFX = NULL;

	HintTrailSoftSuperFX->StopImmediate();
	HintTrailSoftSuperFX = NULL;

	HintCircleSuperFX->StopImmediate();
	HintCircleSuperFX = NULL;

	HintExplodeSuperFX->StopImmediate();
	HintExplodeSuperFX = NULL;

	HintGreenCircleFX->StopImmediate();
	HintGreenCircleFX = NULL;

	//delete ObjectiveTextBgSpr;
	ObjectiveTextBgSpr->Show(false);
	ObjectiveTextBgSpr = NULL;

	SAFE_DELETE(NewInfoTextBgSpr);
	SAFE_DELETE(NewInfoText);

	delete	ObjectiveTitle;
	ObjectiveTitle = NULL;

	delete	ObjectiveTextDescrip;
	ObjectiveTextDescrip = NULL;

	//Garbage collection in gameflow
	delete MessageText;

    //Garbage collection in gameflow
	HudScene->Show(false);
	HudScene = NULL;

	SAFE_DELETE(skipText);
	SAFE_DELETE(hintText);
	SAFE_DELETE(menuText);
	SAFE_DELETE(guideText);
	SAFE_DELETE(mapText);

	if( GFInstance != NULL && !GFInstance->IsPartialCacheMem )
	{
		GFInstance->UnloadBufferSuperFX(HINTCHARGED_SUPERFX);
		GFInstance->UnloadBufferSuperFX(HINTTRAIL_SUPERFX);
		GFInstance->UnloadBufferSuperFX(HINTTRAILSOFT_SUPERFX);
		GFInstance->UnloadBufferSuperFX(HINTCIRCLE_SUPERFX);
		GFInstance->UnloadBufferSuperFX(HINTEXPLODE_SUPERFX);
		GFInstance->UnloadBufferSuperFX(HINTGREENCIRCLE_SUPERFX);
		GFInstance->UnloadBufferSuperFX(DIARYFX_SUPERFX);

		GFInstance->UnloadBufferScene(HO_HUD_SCN);

		GFInstance->UnloadBufferObject(MASK_OBJ);
		GFInstance->UnloadBufferObject(MODEMASK_OBJ);
		GFInstance->UnloadBufferScene(INGAMEMENU_PSCN);
		GFInstance->UnloadBufferScene(OPTIONS_PSCN);
		GFInstance->UnloadBufferScene(MODESELECT_SCN);		
		GFInstance->UnloadBufferObject(HUD_OBJBG_OBJ);
		GFInstance->UnloadBufferObject(HINT_NAVIG_CURSOR_OBJ);
		GFInstance->UnloadBufferObject(HINT_RECHARGE_FRAME_BG);
	}
}

bool HudControl::IsMouseOverHUD(const CPoint* pos)
{
	if(InvBoxCtrl->IsLocked)
	{
		return hudLockedPoly->Intersection(pos);
	}
	else if(InvBoxCtrl->State == InvBox_Inactive)
	{
		return hudMinimizedPoly->Intersection(pos);
	}
	else
	{
		return hudLockedPoly->Intersection(pos);
	}
}

void HudControl::EnableInput(bool f)
{
	AllowInput = f;
}

void HudControl::ShowJrnEntryFX()
{	
	DiaryAddVFX->Start();
	Control::Audio->QuickLoadAndPlaySFX(aSFXDiaJournalAdd);
}

void HudControl::OnAwardCompleted(int awardId)
{
	#ifdef _CEBUILD
		 ShowAwardAchieved(awardId);		
	#endif 
}

#ifdef _CEBUILD

//Memory Objects
void HudControl::ShowMemoryObjectCollected_CE(int memId, CPoint* memPos)
{	
	ShowBannerText(MemoryBannerTexts[memId]);
	Control::Audio->PlaySample(aSFXMagic2);

	/*
	int randId = rand()%3;
	switch (randId)
	{
	case 0:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv1);		break;
	case 1:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv2);		break;
	case 2:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv3);		break;
	default:
		break;
	}
	*/
	
	ProfileSystem::Instance->ActiveProfile.NumMemoriesCollected += 1;
	ProfileSystem::Instance->ActiveProfile.LastMemoryCollected = memId;
	ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[memId] = true;
	AwardManager::GetInstance()->CollectMemory();
	isMemoryObjectCollected = true;


	
	//Show Some FX at pos
	paptusFX = new SuperFX("PRT\\UI\\paptus.INI", memPos, 1100); 
	paptusFX->Start();

	paptusglowFX = new SuperFX("PRT\\UI\\paptusglow.INI", memPos, 1100); 
	paptusglowFX->Start();


	//memoryCollectFrameBG
	if(memoryCollectFrameBG!=NULL)
		SAFE_DELETE(memoryCollectFrameBG);

	memoryCollectFrameBG = new CObject();
	memoryCollectFrameBG->LoadInit("UI\\COLLECT\\MEMORYCOLLECTBG.SPR", 13, 20, 5000);//("UI\\COLLECT\\COLLECTEDBG.SPR", 13, 20, 5000);

	if(ProfileSystem::Instance->ActiveProfile.Resolution <= R1024x768)
		memoryCollectFrameBG->SetXY(413.0f, 141.0f);
	//else if(ProfileSystem::Instance->ActiveProfile.Resolution <= R1280x720)
	else if(ProfileSystem::Instance->ActiveProfile.Resolution <= R1280x800)
		memoryCollectFrameBG->SetXY(285, 141.0f);
	#ifdef K_ANDROID
	else if(ProfileSystem::Instance->ActiveProfile.Resolution >= R1920x1200)
		memoryCollectFrameBG->SetXY(350, 141.0f);
	#endif
	else
		memoryCollectFrameBG->SetXY(242.0f, 141.0f);

	memoryCollectFrameBG->SetAlpha(0);

	//shift for multiple collectables
	if(isMorphCollectibleUIShowing)
		memoryCollectFrameBG->Pos.y += COLLECTIBLE_VERT_GAP;
	if(isAwardUIShowing)
		memoryCollectFrameBG->Pos.y += COLLECTIBLE_VERT_GAP;

	//memoryCollectTextBG
	if(memoryCollectTextBG!=NULL)
		SAFE_DELETE(memoryCollectTextBG);

	
	memoryCollectTextBG = new CObject();
	memoryCollectTextBG->LoadInit("UI\\COLLECT\\MEMTEXTBG.SPR", memoryCollectFrameBG->Pos.x+154, memoryCollectFrameBG->Pos.y, 5010, false);
	memoryCollectTextBG->SetAlpha(0);

	float textOffsetX=-70.0f, textOffsetY=-16.0f+11; // Sac: Moving OffsetX, Y to further left from 70.0f, -10.0f
	
	
	std::string temp = std::string(StringTable::getStr("Exnobj"))+MKSTR(" ")+MKSTR(ProfileSystem::Instance->ActiveProfile.NumMemoriesCollected)+MKSTR("")+std::string(StringTable::getStr("Ex/30"));
	if(MemoryText == NULL)
	{
		ColorRGB white = {1.0f, 1.0f, 1.0f};
		ColorRGB lightBlue = {0.51f, 0.77f, 0.96f};
		float _lineWidth = 400;
		int _nLines = 3;
		EFont _font = eFont_Small;
		eTextAlignment _align = Align_Left;
		/*
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		*/
		MemoryText = new MultilineText(_nLines, PointSystem::CreateCPoint(memoryCollectFrameBG->Pos.x+textOffsetX,memoryCollectFrameBG->Pos.y+textOffsetY,0), 5012, white, 30, _lineWidth, _align, _font);
	}	
	MemoryText->SetPosXY(memoryCollectFrameBG->Pos.x+textOffsetX,memoryCollectFrameBG->Pos.y+textOffsetY + 7);
	MemoryText->SetTextMultiline(temp);
	MemoryText->setAlpha(0);
	MemoryText->Show(true);

	textBG_Mask = new CObject();
	textBG_Mask->LoadInit("UI\\COLLECT\\TXTBG.SPR", memoryCollectFrameBG->Pos.x + textOffsetX , memoryCollectFrameBG->Pos.y + textOffsetY, 5011);
	textBG_Mask->SetAlpha(0);
	textBG_Mask->Show(false);


	if(btnGotoMemoryButton!=NULL)
	{
		SAFE_DELETE(btnGotoMemoryButton);
	}
			
	btnGotoMemoryButton = new CObject();
	btnGotoMemoryButton->LoadInit("UI\\COLLECT\\BTNKIDROOM.SPR", memoryCollectFrameBG->Pos.x+82, memoryCollectFrameBG->Pos.y+67, 5000+6);
	btnGotoMemoryButton->SetAlpha(0);

	if (memoriesBtnText != NULL)
	{
		SAFE_DELETE(memoriesBtnText);
	}

	memoriesBtnText = new CObject();
	memoriesBtnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\MEMORIES.SPR", memoryCollectFrameBG->Pos.x + 82, memoryCollectFrameBG->Pos.y + 67, 5000 + 7);
	memoriesBtnText->SetAlpha(0);


	if(memoryCollectItemGlow!=NULL)
		SAFE_DELETE(memoryCollectItemGlow);
	if(memoryCollectObj!=NULL)
		SAFE_DELETE(memoryCollectObj);
	if(memoryPreviewObj!=NULL)
		SAFE_DELETE(memoryPreviewObj);

	memoryCollectItemGlow = new CObject();	
	memoryCollectObj = new CObject();
	memoryPreviewObj = new CObject();

	int startId = 0;

	switch (ProfileSystem::Instance->ActiveProfile.CurrentLevel)//Each Level has separate spritesheets for quick loading
	{
	case Level_1:
		{
			memoryCollectItemGlow->LoadInit("UI\\COLLECT\\COLLECTGLOW.SPR", memoryCollectFrameBG->Pos.x-151, memoryCollectFrameBG->Pos.y-6, 5000+2);
			memoryCollectObj->LoadInit("UI\\COLLECT\\MEM1COLLECTITEM.SPR", memPos->x, memPos->y, 5000+10);
			memoryPreviewObj->LoadInit("UI\\COLLECT\\MEM1BGPREVIEW.SPR", memoryCollectFrameBG->Pos.x+20, memoryCollectFrameBG->Pos.y+1, 5000+4);
		}
		break;
	case Level_2:
		{
			memoryCollectItemGlow->LoadInit("UI\\COLLECT\\L2COLLECTGLOW.SPR", memoryCollectFrameBG->Pos.x-151, memoryCollectFrameBG->Pos.y-6, 5000+2);
			memoryCollectObj->LoadInit("UI\\COLLECT\\MEM2COLLECTITEM.SPR", memPos->x, memPos->y, 5000+10);
			memoryPreviewObj->LoadInit("UI\\COLLECT\\MEM2BGPREVIEW.SPR", memoryCollectFrameBG->Pos.x+20, memoryCollectFrameBG->Pos.y+1, 5000+4);

			startId = eMem_L2S5Statue;
		}
		break;
	case Level_3:
		{
			memoryCollectItemGlow->LoadInit("UI\\COLLECT\\L3COLLECTGLOW.SPR", memoryCollectFrameBG->Pos.x-151, memoryCollectFrameBG->Pos.y-6, 5000+2);
			memoryCollectObj->LoadInit("UI\\COLLECT\\MEM3COLLECTITEM.SPR", memPos->x, memPos->y, 5000+10);
			memoryPreviewObj->LoadInit("UI\\COLLECT\\MEM3BGPREVIEW.SPR", memoryCollectFrameBG->Pos.x+20, memoryCollectFrameBG->Pos.y+1, 5000+4);

			startId = eMem_L3S1MemMirror;
		}
		break;
	case Level_4:
		{
			memoryCollectItemGlow->LoadInit("UI\\COLLECT\\L4COLLECTGLOW.SPR", memoryCollectFrameBG->Pos.x-151, memoryCollectFrameBG->Pos.y-6, 5000+2);
			memoryCollectObj->LoadInit("UI\\COLLECT\\MEM4COLLECTITEM.SPR", memPos->x, memPos->y, 5000+10);
			memoryPreviewObj->LoadInit("UI\\COLLECT\\MEM4BGPREVIEW.SPR", memoryCollectFrameBG->Pos.x+20, memoryCollectFrameBG->Pos.y+1, 5000+4);

			startId = eMem_L4S1Pumpkin;
		}
		break;
	case Level_5:
		{
			memoryCollectItemGlow->LoadInit("UI\\COLLECT\\L5COLLECTGLOW.SPR", memoryCollectFrameBG->Pos.x-151, memoryCollectFrameBG->Pos.y-6, 5000+2);
			memoryCollectObj->LoadInit("UI\\COLLECT\\MEM5COLLECTITEM.SPR", memPos->x, memPos->y, 5000+10);
			memoryPreviewObj->LoadInit("UI\\COLLECT\\MEM5BGPREVIEW.SPR", memoryCollectFrameBG->Pos.x+20, memoryCollectFrameBG->Pos.y+1, 5000+4);

			startId = eMem_L5S2DogStatue;
		}
		break;
	case Level_6:
		{
			memoryCollectItemGlow->LoadInit("UI\\COLLECT\\L6COLLECTGLOW.SPR", memoryCollectFrameBG->Pos.x-151, memoryCollectFrameBG->Pos.y-6, 5000+2);
			memoryCollectObj->LoadInit("UI\\COLLECT\\MEM6COLLECTITEM.SPR", memPos->x, memPos->y, 5000+10);
			memoryPreviewObj->LoadInit("UI\\COLLECT\\MEM6BGPREVIEW.SPR", memoryCollectFrameBG->Pos.x+20, memoryCollectFrameBG->Pos.y+1, 5000+4);

			startId = eMem_L6S1LocketChain;
		}
		break;
	default:
		break;
	}

	memoryCollectObj->PlayAnimation((int)memId-startId);
	memoryPreviewObj->PlayAnimation((int)memId-startId);

	memoryCollectObj->SetAlpha(0);
	memoryCollectObj->FadeIn();

	memoryPreviewObj->SetScale(0.7f);
	memoryPreviewObj->SetXY(memoryPreviewObj->Pos.x - 30, memoryPreviewObj->Pos.y - 25);

	memoryPreviewObj->SetAlpha(0);
	memoryCollectItemGlow->SetAlpha(0);

	memoryObjFadeTimer = 0;

	isMemoryFlying = false;
	isMemoryUIShowing = true;
	isMemoryObjectFading = true;

	//Init Bezier
	memoryObjStartPt.x = memPos->x;
	memoryObjStartPt.y = memPos->y;

	memoryObjEndPt.x = memoryCollectFrameBG->Pos.x-151.0f;
	memoryObjEndPt.y = memoryCollectFrameBG->Pos.y-1.0f;

	float randValue = rand()%600 - 300.0f;
	memoryObjCtrlPt.SetXY( (memoryObjStartPt.x+memoryObjEndPt.x)/2, (memoryObjStartPt.y+memoryObjEndPt.y)/2 + randValue );
}

//Collectables - Morph Objects, Memory Objects, Achievements
void HudControl::ShowMorphObjectCollected()
{
	int randId = rand()%3;
	switch (randId)
	{
	case 0:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv1);		break;
	case 1:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv2);		break;
	case 2:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv3);		break;
	default:
		break;
	}

	ProfileSystem::Instance->ActiveProfile.NumMorphObjectsCollected += 1;	
	AwardManager::GetInstance()->CollectMorphing();
	switch( ProfileSystem::Instance->ActiveProfile.CurrentArea )
	{	
		//Level 1
		case eArea_L1_S1_Beach:				ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected = true;	break;
		case eArea_L1_S2_Frontyard:			ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S2Collected = true;	break;
		case eArea_L1_S3_Foyer:				ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S3Collected = true;	break;
		case eArea_L1_S7_BoatDock:			ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S7Collected = true;	break;

		case eArea_L2_S1_ForestEntrance:	ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S1Collected = true;	break;
		case eArea_L2_S2_NagaTemple:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S2Collected = true;	break;
		case eArea_L2_S3_CaveMouth:			ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S3Collected = true;  break;
		case eArea_L2_S4_Cave:				ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S4Collected = true;	break;
		case eArea_L2_S5_MoopansDen:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S5Collected = true;	break;
		case eArea_L2_S6_TunnelEnd:			ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S6Collected = true;	break;

			
		case eArea_L3_S1_FortEntrance:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S1Collected = true;	break;
		case eArea_L3_S2_FortInterior:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S2Collected = true;	break;
		case eArea_L3_S3_Pool:				ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S3Collected = true;	break;
		case eArea_L3_S4_FortFountain:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S4Collected = true;	break;	

		case eArea_L4_S1_SwampEntrance:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S1Collected = true;	break;
		case eArea_L4_S3_Cave:				ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S3Collected = true;	break;	
		case eArea_L4_S4_HillTop:			ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S4Collected = true;	break;
		case eArea_L4_S5_HillSide:			ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S5Collected = true;	break;

		case eArea_L5_S2_TigerCloseup:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S2Collected = true;	break;
		case eArea_L5_S4_Dungeon:			ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S4Collected = true;	break;	
		case eArea_L5_S5_MountainBelow:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S5Collected = true;	break;
		case eArea_L5_S6_MountainTop:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S6Collected = true;	break;

		case eArea_L6_S1_FloatingIsland1:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected = true;	break;
		case eArea_L6_S2_FloatingIsland2:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected = true;	break;	
		case eArea_L6_S3_FloatingIsland3:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected = true;	break;
		case eArea_L6_S4_FloatingIsland4:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected = true;	break;

		case eArea_L7_S1_Volcano:			ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S1Collected = true;	break;
		case eArea_L7_S2_LasyaPrison:		ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S2Collected = true;	break;	
		case eArea_L7_S3_Haven:				ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S3Collected = true;	break;
		case eArea_L7_S4_Ameeha:			ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S4Collected = true;	break;

		default:
			/*
			#ifdef K_WIN32
				 WARN(StringTable::getString("ERR_NoMorp")); 
			#else
				 WARN(StringTable::getString("ERR_NoMorp")); 
			#endif	
			*/
		 break;
		}

	isMorphObjectCollected = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAwardPopupsEnabled)
	{
		if(morphVFX==NULL)
		{
			morphVFX = new SuperFX();
			morphVFX = new SuperFX("PRT\\UI\\morphcollect.INI", PointSystem::CreateCPoint(0,0,0), 500);
		}
		morphVFX->SetPos(&morphCollectObj->Pos);
		morphVFX->Start();

		morphDummyObj->FadeIn(2.0f);

		morphCollectObj->SetAlpha(1.0f);
		morphCollectObj->FadeOut(0.75f); //Fadeout only
		morphCollectObj->SetZ(eZOrder_Popup+210);

		//if award popup disabled and we clicked morph skip to next tutorial soon!!
		if(isMorphTutorialActive)
		{
			isMorphTutorialActive = false;
			ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject] = true;

			if(Tutor != NULL && Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
			{
				if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] )
					Tutor->ShowTutorial(TUT12_NavigationUp);
			}
			else
			{
				Tutor->HideTutorial();
			}
		}
		return;
	}

	if(collectMorphFrameBG==NULL)
	{
		collectMorphFrameBG = new CObject();
		collectMorphFrameBG->LoadInit("UI\\COLLECT\\COLLECTEDBG.SPR", 13, 20, eZOrder_Popup+204);
	}

	if(ProfileSystem::Instance->ActiveProfile.Resolution <= R1024x768)
		collectMorphFrameBG->SetXY(413.0f, 141.0f);
	//else if(ProfileSystem::Instance->ActiveProfile.Resolution <= R1280x720)
	else if(ProfileSystem::Instance->ActiveProfile.Resolution <= R1280x800)
		collectMorphFrameBG->SetXY(285, 141.0f);
	#ifdef K_ANDROID
	else if(ProfileSystem::Instance->ActiveProfile.Resolution >= R1920x1200)
		collectMorphFrameBG->SetXY(350, 141.0f);
	#endif
	else
		collectMorphFrameBG->SetXY(242.0f, 141.0f);

	//shift for multiple collectables
	if(isMemoryUIShowing)
		collectMorphFrameBG->Pos.y += COLLECTIBLE_VERT_GAP;
	if(isAwardUIShowing)
		collectMorphFrameBG->Pos.y += COLLECTIBLE_VERT_GAP;

	collectMorphFrameBG->SetAlpha(0);
	collectMorphFrameBG->FadeIn();

	float iconOffsetX = 148.0f;

	morphObjStartPt = morphCollectObj->Pos;
			
	btnGotoMorphButton = new CObject();
	btnGotoMorphButton->LoadInit("UI\\COLLECT\\BTNEXTRAS.SPR", collectMorphFrameBG->Pos.x+82, collectMorphFrameBG->Pos.y+67, eZOrder_Popup+206);
	btnGotoMorphButton->SetAlpha(0);

	extrasBtnText = new CObject();
	extrasBtnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\EXTRAS.SPR", collectMorphFrameBG->Pos.x + 82, collectMorphFrameBG->Pos.y + 67, eZOrder_Popup + 207);
	extrasBtnText->SetAlpha(0);

	if(morphVFX==NULL)
	{
		morphVFX = new SuperFX();
		morphVFX = new SuperFX("PRT\\UI\\morphcollect.INI", PointSystem::CreateCPoint(0,0,0), 500);
	}
	morphVFX->SetPos(&morphObjStartPt);
	morphVFX->Start();

	//morphCollectObj->SetXY(collectMorphFrameBG->Pos.x-iconOffsetX,collectMorphFrameBG->Pos.y);
	morphCollectObj->FadeIn(4.0f); //fast fade in, if fading in progress
	morphCollectObj->SetZ(eZOrder_Popup+210);

	morphDummyObj->FadeIn(4.0f);

	float textOffsetX=-70.0f, textOffsetY=-16.0f; // Sac: Moving OffsetX,Y to further left from -44.0f and -10.0f
	std::string temp = std::string(StringTable::getStr("Exnmorph"))+ " " +MKSTR(ProfileSystem::Instance->ActiveProfile.NumMorphObjectsCollected)+std::string(StringTable::getStr("Ex/28"));
	if(morphText == NULL)
	{
		ColorRGB white = {1.0f, 1.0f, 1.0f};
		float _lineWidth = 400;
		int _nLines = 3;
		EFont _font = eFont_26;
		eTextAlignment _align = Align_Left;
		/*
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		*/
		morphText = new MultilineText(_nLines, PointSystem::CreateCPoint(collectMorphFrameBG->Pos.x+textOffsetX,collectMorphFrameBG->Pos.y+textOffsetY,0), eZOrder_Popup+210, white, 30, _lineWidth, _align, _font);
	}
	morphText->SetPosXY(collectMorphFrameBG->Pos.x+textOffsetX,collectMorphFrameBG->Pos.y+textOffsetY);
	morphText->SetTextMultiline(temp);
	morphText->setAlpha(0);
	morphText->Show(true);

	morphCollectAnimTime = 0;

	morphObjEndPt.x = collectMorphFrameBG->Pos.x-iconOffsetX;
	morphObjEndPt.y = collectMorphFrameBG->Pos.y;

	float randValue = rand()%600 - 300.0f;

	morphObjCtrlPt.SetXY( (morphObjStartPt.x+morphObjEndPt.x)/2, (morphObjStartPt.y+morphObjEndPt.y)/2 + randValue);

	isMorphCollectibleFlying = true;
	isMorphCollectibleUIShowing = true;
}

void HudControl::PauseAwardUI(bool pause)
{
	isAwardUIPaused = pause;
	if(awardCollectFrameBG!=NULL)
		awardCollectFrameBG->Paused = pause;
	if(btnGotoAwardsButton!=NULL)
		btnGotoAwardsButton->Paused = pause;
	if(awardIcon!=NULL)
		awardIcon->Paused			= pause;
}

void HudControl::HideAwardUI()
{
	if(isAwardUIShowing)
	{
		//quickly start fading
		awardCollectFrameBG->FadeOut(4.0f);
		awardIcon->FadeOut(4.0f);
		btnGotoAwardsButton->FadeOut(4.0f);
		achBtnText->FadeInOut(4.0f);
		awardsHideText->FadeOut(4.0f);
		awardsHideTick->FadeOut(4.0f);
		AwardText->Show(false);

		isMouseOverAwardButton = false;
		isAwardUIShowing = false;
	}
}

void HudControl::ShowAwardAchieved(int awardId)
{

	if(!ProfileSystem::Instance->ActiveProfile.IsAwardPopupsEnabled )
	{
		isAwardRemovedFromQueue = true;
		AwardManager::GetInstance()->RemoveCompletedAwardFromQueue(currentAwardId);
		return;
	}
/*
	if(awardId == 6 || awardId == 7 || awardId == 20)
	{
		isAwardRemovedFromQueue = true;
		AwardManager::GetInstance()->RemoveCompletedAwardFromQueue(currentAwardId);
		return;
	}
*/
	int randId = rand()%3;
	switch (randId)
	{
	case 0:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv1);		break;
	case 1:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv2);		break;
	case 2:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv3);		break;
	default:
		break;
	}

	int awardIDMapArray[kAward_Max] = {
		0, 1, 32, 33, 34, 2,
		3, 3, 3, 4, 5, 5, 5,
		6, 7, 8, 9, 10,
		11, 12, 13, 13, 14,
		15, 16, 17, 18, 19,
		20, 21, 22, 23, 24,
		25, 26, 27, 28, 29, 30, 31, 35
	};

	isAwardRemovedFromQueue = false;
	currentAwardId = awardId;

	//0//0 DRONA
	//1//1 NA SUCITA 1
	//2//32 NA SUCITA 2
	//3//33 NA SUCITA 3
	//4//34 NA SUCITA 4
	//5//2 Arjuna
	//6//3 Vayu
	//7//3
	//8//3
	//9//4 Sama kanta
	//10//5 Agni
	//11//5
	//12//5
	//13//6 GURU
	//14//7 NIRMOKTR
	//15//8 PRASPHURANA
	//16//9 CHANAKYA
	//17//10 MA CARA
	//18//11 HOP PATI
	//19//12 TARTARITI
	//20//13 KARMA
	//21//13 
	//22//14 PRARAMBHA
	//23//15 DEVARUPIN
	//24//16 AVARA CARA
	//25//17 SIDDHI
	//26//18 UDGHATIN
	//27//19 UPALAMBHAKA
	//28//20 SITA
	//29//21 MATR RAKSA
	//30//22 DRASTTA
	//31//23 GARUDA
	//32//24 ADHIMUKTI
	//33//25 ZISAVA
	//34//26 PATHAKA
	//35//27 KAUSHIK
	//36//28 PATHIKA
	//37//29 RAKSHAK
	//38//30 DAMAKA
	//39//31 VIGRAHA
	//40//35 AJNA CHAKRA - To Add

	int awardIdToShow=awardIDMapArray[awardId];

	if(awardCollectFrameBG==NULL)
	{
		awardCollectFrameBG = new CObject();
		awardCollectFrameBG->LoadInit("UI\\COLLECT\\COLLECTEDBG.SPR", 13, 20, 5000);
	}

	if(ProfileSystem::Instance->ActiveProfile.Resolution <= R1024x768)
		awardCollectFrameBG->SetXY(413.0f, 141.0f);
	//else if(ProfileSystem::Instance->ActiveProfile.Resolution <= R1280x720)
	else if(ProfileSystem::Instance->ActiveProfile.Resolution <= R1280x800)
		awardCollectFrameBG->SetXY(285, 141.0f);
	#ifdef K_ANDROID
	else if(ProfileSystem::Instance->ActiveProfile.Resolution >= R1920x1200) 
		awardCollectFrameBG->SetXY(350, 141.0f);
	#endif
	else
		awardCollectFrameBG->SetXY(242.0f, 141.0f);
	//shift for multiple collectables
	if(isMemoryUIShowing)
		awardCollectFrameBG->Pos.y += COLLECTIBLE_VERT_GAP;
	if(isMorphCollectibleUIShowing)
		awardCollectFrameBG->Pos.y += COLLECTIBLE_VERT_GAP;

	if(btnGotoAwardsButton==NULL)
	{
		btnGotoAwardsButton = new CObject();
		btnGotoAwardsButton->LoadInit("UI\\COLLECT\\BTNEXTRAS.SPR", awardCollectFrameBG->Pos.x+162, awardCollectFrameBG->Pos.y+59, 7000+6);
	}

	if (achBtnText == NULL)
	{
		achBtnText = new CObject();
		achBtnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\EXTRAS.SPR", awardCollectFrameBG->Pos.x + 162, awardCollectFrameBG->Pos.y + 59, 7000 + 7);
	}

	if(awardsHideText==NULL)
	{
		awardsHideText = new CObject();
		awardsHideText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\HIDEAWARDSTXT.SPR", awardCollectFrameBG->Pos.x-24, awardCollectFrameBG->Pos.y+51, 5000+6);

		awardsHideTick = new CObject();
		awardsHideTick->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\HIDETICK.SPR", awardCollectFrameBG->Pos.x+46, awardCollectFrameBG->Pos.y+51, 5000+6);
	}

	btnGotoAwardsButton->SetXY(awardCollectFrameBG->Pos.x+162, awardCollectFrameBG->Pos.y+59);
	achBtnText->SetXY(awardCollectFrameBG->Pos.x + 162, awardCollectFrameBG->Pos.y + 59);
	

	if(awardIcon!=NULL)
		SAFE_DELETE(awardIcon);

	awardIcon = new CObject();
	awardIcon->LoadInit(std::string("UI\\COLLECT\\AWICONS\\AWARDICON" + MKSTR(awardIdToShow) + "G.SPR").c_str(), awardCollectFrameBG->Pos.x-150, awardCollectFrameBG->Pos.y, 5000+6);
	awardIcon->SetScale(1.35f);

	if(awardsVFX==NULL)
	{
		awardsVFX = new SuperFX();
		awardsVFX = new SuperFX("PRT\\UI\\awardfx.INI", PointSystem::CreateCPoint(0,0,0), 500);
	}
	awardsVFX->StopImmediate();
	awardsVFX->SetPos(&awardCollectFrameBG->Pos);
	awardsVFX->Start();

	awardCollectFrameBG->SetAlpha(0);
	awardCollectFrameBG->FadeIn();

	btnGotoAwardsButton->SetAlpha(0);
	btnGotoAwardsButton->FadeIn();

	achBtnText->SetAlpha(0);
	achBtnText->FadeIn();


	awardsHideText->SetAlpha(0);
	awardsHideText->FadeIn();

	awardsHideTick->SetAlpha(0);
	awardsHideTick->FadeIn();

	awardIcon->SetAlpha(0);
	awardIcon->FadeIn();

	float textOffsetX=-64.0f, textOffsetY=-38.0f; //Sac: changing ofsetX, Y to make it go further inside from -65.0f, -40.0f
	
	std::string temp = StringTable::getStr("Exunlock")+StringTable::getStr(AwardTextArray[awardId]);
	if(AwardText == NULL)
	{
		ColorRGB white = {1.0f, 1.0f, 1.0f};

		float _lineWidth = 400;
		int _nLines = 4;
		EFont _font = eFont_26;
		eTextAlignment _align = Align_Left;
		
		if(StringTable::IsKeyFound(AwardTextArray[awardId]))
		{
			_lineWidth = StringTable::GetLineWidth(AwardTextArray[awardId]);
			_nLines = StringTable::GetNoOfLines(AwardTextArray[awardId]);
			_font = (EFont)StringTable::GetFont(AwardTextArray[awardId]);
		}
		
		AwardText = new MultilineText(_nLines, PointSystem::CreateCPoint(awardCollectFrameBG->Pos.x+textOffsetX,awardCollectFrameBG->Pos.y+textOffsetY,0), 5010, white, 30, _lineWidth, _align, _font);
		/*
		if(StringTable::IsLanguageMultiByte())
			AwardText = new MultilineText(4, PointSystem::CreateCPoint(awardCollectFrameBG->Pos.x+textOffsetX,awardCollectFrameBG->Pos.y+textOffsetY,0), 5010, white, 30, 700, Align_Left);
		else
			AwardText = new MultilineText(4, PointSystem::CreateCPoint(awardCollectFrameBG->Pos.x+textOffsetX,awardCollectFrameBG->Pos.y+textOffsetY,0), 5010, white, 30, 400, Align_Left);
			*/
	}
	AwardText->SetPosXY(awardCollectFrameBG->Pos.x+textOffsetX,awardCollectFrameBG->Pos.y+textOffsetY);
	AwardText->SetTextMultiline(temp);
	AwardText->setAlpha(0);
	AwardText->Show(true);

	awardCollectAnimTime = 0;

	isAwardFadingOut = false;
	isAwardUIShowing = true;
}

void HudControl::setMorphObjects(CObject* collectObj, CObject* dummyObj)
{
	morphCollectObj = collectObj;
	morphDummyObj = dummyObj;

	morphCollectObj->SetEnable(false);

	isMorphObjectVisible = true;
	isMorphObjectsActive = true;
}

void HudControl::setMorphObjRect(CRectangle cRect)
{
	morphRect = cRect;
}

void HudControl::HideMorphObjects()
{
	if(isMorphObjectsActive)
	{
		isMorphObjectVisible = false;

		if(isMorphFadingIn)
		{
			if( morphDummyObj )
				morphDummyObj->SetAlpha(0);
			if( morphCollectObj )
				morphCollectObj->SetAlpha(1.0f);
		}

		if(isMorphFadingOut)
		{
			if( morphDummyObj )
				morphDummyObj->SetAlpha(1.0f);
			if( morphCollectObj )
				morphCollectObj->SetAlpha(0);
		}
	}
}

void HudControl::HideMorphCollectedObject()
{
	if(isMorphCollectibleUIShowing)
	{
		if( morphCollectObj )
		{
			morphCollectObj->SetEnable(false);
			morphCollectObj->SetAlpha(0);
		}
	}
}

void HudControl::ShowMorphCollectedObject()
{
	if(isMorphCollectibleUIShowing)
	{
		if( morphCollectObj )
		{
			morphCollectObj->SetEnable(true);
			morphCollectObj->Paused = false;
			morphCollectObj->Show(true);
			morphCollectObj->SetAlpha(1.0f);
		}
	}
}

void HudControl::ShowMorphObjects()
{
	isMorphObjectVisible = true;
}

#endif 





void HudControl::ShowNewInfoTextBanner()
{
	DiaryAddVFX->Start();
	Control::Audio->QuickLoadAndPlaySFX(aSFXDiaJournalAdd);

	if(NewInfoTextBgSpr==NULL)
	{
		NewInfoTextBgSpr = new CObject();
		NewInfoTextBgSpr->LoadInit("UI\\HUD\\newinfobg.SPR", 274.0f, 596, 5032);
	}
	NewInfoTextBgSpr->FadeIn(1.5f);

	if(NewInfoText==NULL)
	{
		NewInfoText = new CBitmapText();
		ColorRGB yellow={(233/255.0f),(204/255.0f),(131/255.0f)};
		NewInfoText->SetTextProps(StringTable::getStr("Exinfo"), 266, 592, 5034, yellow, eFont_Italic24, Align_Center);
	}
	NewInfoText->Show(true);
	NewInfoText->FadeInText(1.0f);

	IsNewInfoTextShowing = true;
	newInfoTextTimer = 0;
}

void HudControl::ShowObjectiveFX(int ObjId, bool IsCompleted)
{
	DiaryAddVFX->Start();
	
	IsObjectiveTextShowing = true;
	ObjectiveTextsAlpha = 0.0f;
	ObjectiveCounter = 0;

	ObjectiveTextBgSpr->SetAlpha(ObjectiveTextsAlpha);
	ObjectiveTextBgSpr->Show(true);

	if(IsCompleted)
	{
		ObjectiveTitle->SetText(StringTable::getStr("Exobj"));
		Control::Audio->QuickLoadAndPlaySFX(aSFXDiaJournalAdd);
	}
	else
	{
		ObjectiveTitle->SetText(StringTable::getStr("Exnew"));
		Control::Audio->QuickLoadAndPlaySFX(aSFXDiaObjAdd);
	}

	ObjectiveTitle->SetAlpha(ObjectiveTextsAlpha);
	ObjectiveTitle->Show(true);

	ObjectiveTextDescrip->SetText(StringTable::getStr(LvLObjTexts[ObjId]));
	ObjectiveTextDescrip->SetAlpha(ObjectiveTextsAlpha);
	ObjectiveTextDescrip->Show(true);
}

void HudControl::Required()
{	
	UpdateBanner();

#ifdef _CEBUILD
	CPoint mosPos = Control::Input->Mpos();
	bool click = Control::Input->LBclicked();

	if(isMorphCollectibleUIShowing)
	{
		morphCollectAnimTime += Control::LogicRate;
		if(isMorphCollectibleFlying)
		{
			if( morphCollectAnimTime >= 1.0f )
			{
				morphCollectAnimTime = 1.0f;
				isMorphCollectibleFlying = false;

				if(isMorphTutorialActive)
				{
					isMorphTutorialActive = false;
					ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject] = true;

					if(Tutor != NULL && Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
					{
						if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] )
							Tutor->ShowTutorial(TUT12_NavigationUp);
					}
					else
					{
						Tutor->HideTutorial();
					}
				}
			}

			//update moving morph object
			CPoint bPos = Elixir::GetBezierPoint(&morphObjStartPt, &morphObjCtrlPt, &morphObjEndPt, morphCollectAnimTime);

			morphText->setAlpha(morphCollectAnimTime);
			btnGotoMorphButton->SetAlpha(morphCollectAnimTime);
			extrasBtnText->SetAlpha(morphCollectAnimTime);
			morphCollectObj->Pos = bPos;

			HintTrailSuperFX->Pos = bPos;
			HintTrailSuperFX->Emit();
		}
		else
		{
			//morph reached the panel
			if(morphCollectAnimTime>4.0f)
			{
				collectMorphFrameBG->FadeOut(2.0f);
				morphText->Show(false);
				btnGotoMorphButton->FadeOut(2.0f);
				extrasBtnText->Show(false);//FadeOut(2.0f);

				morphCollectObj->FadeOut(2.0f);

				morphTimer = 0;
				morphFadeTimer = 0;

				morphCollectAnimTime = 0;

				isMouseOverMorphButton = false;
				isMorphCollectibleUIShowing = false;
			}
			else
			{
				if(btnGotoMorphButton->Collide(&mosPos))
				{
					isMouseOverMorphButton = true;
					Cursor::SetMode(CA_HandPointy);
					if(click)
					{
						//Goto Extras Room
						ProfileSystem::Instance->ActiveProfile.IsExtrasRoomFromMainMenu = false;
						if(!(ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_KidsRoom && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_AwardsRoom))
						{
							ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
							ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
						}
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_ExtrasRoom;
						SendMessage(ActionProcessed, ID, MAP_NEW_AREA);//Handle map transition
						return;
					}
				}
				else
					isMouseOverMorphButton = false;	
			}
		}
	}

	if(isMemoryUIShowing)
	{
		//wait for object to fade
		if(isMemoryObjectFading)
		{
			memoryObjFadeTimer += Control::LogicRate;
			if(memoryObjFadeTimer>0.85f)
			{
				isMemoryObjectFading = false;
				isMemoryFlying = true;
			}
			return;
		}

		memoryCollectAnimTime += Control::LogicRate;
		if(isMemoryFlying)
		{
			if( memoryCollectAnimTime >= 1.0f )
			{
				memoryCollectAnimTime = 1.0f;
				isMemoryFlying = false;
			}

			CPoint bPos = Elixir::GetBezierPoint(&memoryObjStartPt, &memoryObjCtrlPt, &memoryObjEndPt, memoryCollectAnimTime);

			MemoryText->setAlpha(memoryCollectAnimTime);
			textBG_Mask->SetAlpha(memoryCollectAnimTime);
			if(isMemoryObjectCollected)
			{
				memoryCollectFrameBG->SetAlpha(memoryCollectAnimTime);
				memoryCollectItemGlow->SetAlpha(memoryCollectAnimTime);
				btnGotoMemoryButton->SetAlpha(memoryCollectAnimTime);
				memoriesBtnText->SetAlpha(memoryCollectAnimTime);
				memoryCollectTextBG->SetAlpha(memoryCollectAnimTime);
				memoryPreviewObj->SetAlpha(memoryCollectAnimTime);
				memoryCollectObj->Pos = bPos;
			}

			HintTrailSuperFX->Pos = bPos;
			HintTrailSuperFX->Emit();
		}
		else
		{
			if(memoryCollectAnimTime>4.0f)
			{
				memoryCollectFrameBG->FadeOut(2.0f);
				MemoryText->Show(false);
				textBG_Mask->Show(false);
				btnGotoMemoryButton->FadeOut(2.0f);
				memoriesBtnText->FadeOut(2.0f);
				memoryCollectTextBG->FadeOut(2.0f);
				if(isMemoryObjectCollected)
				{
					memoryPreviewObj->FadeOut(2.0f);
					memoryCollectObj->FadeOut(2.0f);
					memoryCollectItemGlow->FadeOut(2.0f);

					isMemoryObjectCollected = false;

					if(isSceneCloseAfterMemoryFade)
					{
						//Move to Current Area
						SendMessage(ActionProcessed, ID, MAP_NEW_AREA);
					}
				}

				memoryCollectAnimTime = 0;

				//Memory Items	
				memoryObjFadeTimer = 0;

				isMouseOverMemoryButton = false;
				isMemoryUIShowing = false;
			}
			else
			{
				if(btnGotoMemoryButton->Collide(&mosPos))
				{
					isMouseOverMemoryButton = true;
					Cursor::SetMode(CA_HandPointy);
					if(click)
					{
						if(isMemoryObjectCollected)
						{
							//Goto Kids Room
							ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromMainMenu = false;
							if(!(ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_KidsRoom && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_AwardsRoom))
								ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
							ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_KidsRoom;
							SendMessage(ActionProcessed, ID, MAP_NEW_AREA);//Handle map transition
						}
						return;
					}
				}
				else
					isMouseOverMemoryButton = false;	
			}
		}
	}

	if(isAwardUIShowing && !isAwardUIPaused)
	{
		awardCollectAnimTime += Control::LogicRate;

		if( awardCollectAnimTime <= 1.0f )
		{
			AwardText->setAlpha(awardCollectAnimTime);
		}

		if(!isAwardRemovedFromQueue)
		{
			if(awardCollectAnimTime>0.5f)
			{
				isAwardRemovedFromQueue = true;
				AwardManager::GetInstance()->RemoveCompletedAwardFromQueue(currentAwardId);
			}
		}
		
		if(awardCollectAnimTime>4.0f)
		{
			if(!isAwardFadingOut)
			{
				isAwardFadingOut = true;
				awardCollectFrameBG->FadeOut(1.5f);
				awardIcon->FadeOut(1.5f);
				btnGotoAwardsButton->FadeOut(1.5f);
				achBtnText->FadeOut(1.5f);
				awardsHideText->FadeOut(1.5f);
				awardsHideTick->FadeOut(1.5f);

				isMouseOverAwardButton = false;
			}
			else
			{
				AwardText->setAlpha(5.0f-awardCollectAnimTime);
				if(awardCollectAnimTime>4.75f)
				{
					isAwardUIShowing = false;
					//show next queued award

					currentAwardId = AwardManager::GetInstance()->GetCompletedAwardFromQueue();
					if(currentAwardId != -1)
					{
						//show award from queue
						ShowAwardAchieved(currentAwardId);
					}
					else
					{
						AwardText->Show(false);
					}
					
				}
			}
		}
		else
		{
			if(btnGotoAwardsButton->Collide(&mosPos))
			{
				isMouseOverAwardButton = true;
				Cursor::SetMode(CA_HandPointy);
				if(click)
				{
					//Goto Kids Room
					if(!(ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_KidsRoom && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_AwardsRoom))
					{
						ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
						ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
					}
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_AwardsRoom;
					SendMessage(ActionProcessed, ID, MAP_NEW_AREA);//Handle map transition
					return;
				}
			}
			else if(awardsHideTick->Collide(&mosPos) || awardsHideText->Collide(&mosPos))
			{
				isMouseOverAwardButton = true;
				Cursor::SetMode(CA_HandPointy);
				if(click)
				{
					if(awardsHideTick->GetAnimation()==0)
					{
						ProfileSystem::Instance->ActiveProfile.IsAwardPopupsEnabled = false;
						awardsHideTick->PlayAnimation(1);
					}
					else
					{
						ProfileSystem::Instance->ActiveProfile.IsAwardPopupsEnabled = true;
						awardsHideTick->PlayAnimation(0);
					}
					return;
				}
			}
			else
				isMouseOverAwardButton = false;	
		}
	}

	if(isMouseOverMorphButton || isMouseOverMemoryButton || isMouseOverAwardButton)
	{
		isMouseOverExtraButton = true;
	}
	else
	{
		isMouseOverExtraButton = false;
	}

	if( isMorphObjectsActive && !isMorphObjectCollected && isMorphObjectVisible)
	{
		if(isMorphFadingIn || isMorphFadingOut)
		{
			//user must click now to collect it
			if(morphRect.Intersection(&mosPos))
			{
				if(click)
				{
					ShowMorphObjectCollected();
					isMorphFadingIn = isMorphFadingOut = false;
					isMorphObjectsActive = false;
					return;
				}
			}

			morphFadeTimer += Control::LogicRate;
			if(isMorphFadingIn)
			{
				//Visible Time for morph collect
				if(morphFadeTimer>2.25f)
				{
					morphFadeTimer = 0;

					morphCollectObj->FadeOut();
					morphDummyObj->FadeIn();

					isMorphFadingIn = false;
					isMorphFadingOut = true;
				}
			}
			else if(isMorphFadingOut)
			{
				if(morphFadeTimer>2.0f)
				{					
					morphCollectObj->SetEnable(false);
					isMorphFadingOut = false;
				}
			}
		}
		else
		{
			morphTimer += Control::LogicRate;
			if(morphTimer>morphTimerThreshold)
			{
				morphTimer = 0;
				if(isMorphTutorialActive)
					morphTimerThreshold = 1.5f;
				else
				{
					if( GFInstance->IsReplayingGame )	
						morphTimerThreshold = (float)Elixir::Rand(REPLAY_MORPHOBJECT_MINTIME, REPLAY_MORPHOBJECT_MAXTIME);
					else
						morphTimerThreshold = (float)Elixir::Rand(MORPHOBJECT_MINTIME, MORPHOBJECT_MAXTIME);
				}

				morphCollectObj->SetEnable(true);

				morphCollectObj->FadeIn();
				morphDummyObj->FadeOut();

				morphFadeTimer = 0;
				isMorphFadingIn = true;
			}
		}
	}  
#endif 

	if( IsObjectiveTextShowing )
	{
		switch (ObjectiveCounter)
		{
		case 0:
			//FADE IN
			ObjectiveTextsAlpha += Control::LogicRate;

			if(ObjectiveTextsAlpha>1.0f)
			{
				ObjectiveTextsAlpha = 1.0f;

				ObjectiveTextBgSpr->SetAlpha(ObjectiveTextsAlpha);
				ObjectiveTitle->SetAlpha(ObjectiveTextsAlpha);
				ObjectiveTextDescrip->SetAlpha(ObjectiveTextsAlpha);

				ObjectiveTextsAlpha = 0.0f;

				ObjectiveCounter++;
			}
			else
			{
				ObjectiveTextBgSpr->SetAlpha(ObjectiveTextsAlpha);
				ObjectiveTitle->SetAlpha(ObjectiveTextsAlpha);
				ObjectiveTextDescrip->SetAlpha(ObjectiveTextsAlpha);
			}
			break;
		case 1:
			//STATIC
			ObjectiveTextsAlpha += Control::LogicRate;

			if(ObjectiveTextsAlpha>2.0f)
			{
				ObjectiveTextsAlpha = 1.0f;
				ObjectiveCounter++;
			}
			break;
		case 2:
			//FADE OUT
			ObjectiveTextsAlpha -= Control::LogicRate;

			if(ObjectiveTextsAlpha<0.0f)
			{
				ObjectiveTextsAlpha = 0.0f;
				ObjectiveCounter=0;

				IsObjectiveTextShowing = false;

				ObjectiveTextBgSpr->Show(false);
				ObjectiveTitle->Show(false);
				ObjectiveTextDescrip->Show(false);
			}
			else
			{
				ObjectiveTextBgSpr->SetAlpha(ObjectiveTextsAlpha);
				ObjectiveTitle->SetAlpha(ObjectiveTextsAlpha);
				ObjectiveTextDescrip->SetAlpha(ObjectiveTextsAlpha);
			}
			break;
		}
	}

	if(!isHUDVisible)
		return;

	characterUnlockPopup->Update(Control::LogicRate);

	if(IsNewInfoTextShowing)
	{
		newInfoTextTimer += Control::LogicRate;
		if(newInfoTextTimer>3.5f)
		{
			IsNewInfoTextShowing = false;
			NewInfoTextBgSpr->FadeOut(1.5f);
			NewInfoText->FadeOutText(1.5f);
		}
	}

	if( IsMemoryFXTrailing )
	{
		MemoryTrailTime += 0.8f*Control::LogicRate;
		if( MemoryTrailTime >= 1.0f )
		{
			Control::Audio->PlaySample(aSFXMagic2);
			
			MemoryTrailTime = 0.0f;
			IsMemoryFXTrailing = false;

			GFInstance->IsActivePopup = false;

			if(MemoryExplodeSuperFX)
				MemoryExplodeSuperFX->Start();

			SendMessage(ActionProcessed, ID, MEMORY_OBJFX_REACHED);
		}
		else
		{
			//update bezier
			CPoint bPos = Elixir::GetBezierPoint(&memoryStartPt, &memoryCtrlPt ,&memoryEndPt,MemoryTrailTime);

			if(MemoryTrailSuperFX)
				MemoryTrailSuperFX->Pos = bPos;
			if(MemoryTrailSoftSuperFX)
				MemoryTrailSoftSuperFX->Pos = bPos;

			if(MemoryTrailSuperFX)
				MemoryTrailSuperFX->Emit();
			if(MemoryTrailSoftSuperFX)
				MemoryTrailSoftSuperFX->Emit();
		}
	}


	if(IsHintTrailCircling)
	{
		HintTrailCirclingTime += Control::LogicRate;
		
		if( HintTrailCirclingTime >= 1.0f )
		{
			if(!ProfileSystem::Instance->ActiveProfile.IsInvLocked && InvBoxCtrl->IsLocked && !IsPuzzleScene && ProfileSystem::Instance->ActiveProfile.CurrentArea != eArea_L2_S2_HONagaTemple && ProfileSystem::Instance->ActiveProfile.CurrentArea != eArea_L2_S1_UndergroundHO)
			{
				InvBoxCtrl->IsLocked = false;
				InvBoxCtrl->TurnOffArrowsLock();
				InvBoxCtrl->SlidingState = Sliding_Down;
				/*
				Hud->InvBoxCtrl->NoAutoClose = false;
				Hud->InvBoxCtrl->Deactivate();*/
			}
			if(IsDirectionalHint)
			{
				HintDirCursor->FadeOut(1.0f);
			}

			HintTrailSuperFX->StopEmit();
			HintTrailSoftSuperFX->StopEmit();

			HintCircleSuperFX->StopEmit();
			HintExplodeSuperFX->StopEmit();

			HintTrailCirclingTime = 0.0f;
			IsHintTrailCircling = false;
		}
	}		
	else if( IsHintTrailRunning )
	{
		HintTrailTime += Control::LogicRate;
		if( HintTrailTime >= 1.0f )//if( HintTrailTime >= 1.0f )
		{
			Control::Audio->PlaySample(aSFXMagic2);
			
			HintTrailTime = 0.0f;
			IsHintTrailRunning = false;			

			HintTrailCirclingTime = 0.0f;
			IsHintTrailCircling = true;

			HintCircleSuperFX->Start();
			HintExplodeSuperFX->Start();

			if(IsDirectionalHint)
			{				
				HintDirCursor->FadeIn(2.0f);
			}
		}
		else
		{
			//update bezier
			CPoint bPos = Elixir::GetBezierPoint(&hintStartPt, &hintCtrlPt ,&hintEndPt,HintTrailTime);

			HintTrailSuperFX->Pos = bPos;
			HintTrailSoftSuperFX->Pos = bPos;

			HintTrailSuperFX->Emit();
			HintTrailSoftSuperFX->Emit();
		}
	}

	if(isPCockWakeupPlaying)
	{
		if(pCockHeadWakeup->IsAnimationFinished())
		{
			isPCockWakeupPlaying = false;
			pCockHeadWakeIdle->SetEnable(true);
			pCockHeadWakeup->SetEnable(false);
		}
	}

	if(isPCockWakeDownPlaying)
	{
		if(pCockHeadWakeup->IsAnimationFinished())
		{
			isPCockWakeDownPlaying = false;
			pCockHeadIdle->SetEnable(true);
			pCockHeadWakeup->SetEnable(false);
		}
	}

	if(IsHintPaused)
		return;

	if(IsHintDischarging)
	{
		HintVert -= 1.0f * Control::LogicRate;
		if(HintVert<0.1f)
			IsHintDischarging = false;
		else
			UpdateHint();
		return;
	}

	//Hints not filled - update it!!
	if( HintVert < 1.0f && (Elements[HE_Hint]->Visible || Elements[HE_Skip]->Visible) )
	{
		if (IsPuzzleScene) //HintSavedTime USED AS SKIP HERE!!
		{
			switch( ProfileSystem::Instance->ProfileMode[ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot] )
			{
				default:
				case EMODE_CASUAL:HintVert += 0.0333f * Control::LogicRate;//30 sec
									break;
				case EMODE_ADVANCED:HintVert += 0.0111f * Control::LogicRate;//90 sec
									break;
				case EMODE_EXPERT:HintVert += 0.00833f * Control::LogicRate;//120 sec
									break;
				case EMODE_CUSTOM:
							if(ProfileSystem::Instance->ActiveProfile.IsSkipDisabled)
								HintVert += 0 * Control::LogicRate;//skip won't recharge
							else
							{
								#ifdef INSTANT_RECHARGE
									HintVert += Control::LogicRate;//CustomHintTimer sec	
								#else
									HintVert += ProfileSystem::Instance->ActiveProfile.CustomSkipTimer * Control::LogicRate;//CustomHintTimer sec	
								#endif
							}							
							break;
			} 
		}				
		else
		{
			if(IsHintRechargeInstant)
			{
				HintVert += Control::LogicRate;
				if(HintVert >=1.0)
				{
					IsHintRechargeInstant = false;
				}
			}
			else
			{
				switch( ProfileSystem::Instance->ProfileMode[ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot] )
				{
					default:
					case EMODE_CASUAL:HintVert += 0.0333f * Control::LogicRate;//30 sec
										break;
					case EMODE_ADVANCED:HintVert += 0.01665f * Control::LogicRate;//60 sec
										break;
					case EMODE_EXPERT:HintVert += 0.0111f * Control::LogicRate;//90 sec
										break;
					case EMODE_CUSTOM:
#ifdef INSTANT_RECHARGE
						//HintVert += Control::LogicRate;//CustomHintTimer sec
						//break;
#endif
						if(ProfileSystem::Instance->ActiveProfile.IsHintDisabled)
							HintVert += 0 * Control::LogicRate;//hint won't recharge
						else
							{
								HintVert += ProfileSystem::Instance->ActiveProfile.CustomHintTimer * Control::LogicRate;//CustomHintTimer sec
							}	
						break;
							
				} 
			}
		}

		//Hint full!!
		#ifdef _CHEATS
		{
			if( Control::Input->Kpressed(K_VK_H) && !IsHintFullyCharged)
			{
				HintVert = 1.0f;
				//Hint Update works in 30, 60, 90 degrees. So Instant change will cause peacock anim update not working properly
				for (int i = 1; i <= 10; i++)
				{
					HintVert = ((float)(i)/10.0f);
					UpdateHint();
				}
			}
		}
		#endif

		UpdateHint();
	}
}

void HudControl::ShowHintForInvItem(EHOInventory inventoryItemId, bool isInstantRecharge, eCursorAnim cursorId)
{
	InvBoxCtrl->IsLocked = true;																							 
	InvBoxCtrl->Activate();
						
	for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
	{
		if( InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
		{	
			if(  InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == inventoryItemId)
			{
				if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
						InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj, true);
				else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
						InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i, true);
									
				InitHintTrailEmit(InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y, isInstantRecharge, cursorId);
				break;
			}
		}
	}
}

void HudControl::ShowHintForInvItem(EHOInventory inventoryItemId, const CRectangle* destRect, bool isInstantRecharge, eCursorAnim cursorId)
{
	ShowHintForInvItem(inventoryItemId, (destRect==NULL)?NULL:PointSystem::CreateCPoint(destRect->GetCenterX(), destRect->GetCenterY(), 0.0f), isInstantRecharge, cursorId);
}

void HudControl::ShowHintForInvItem(EHOInventory inventoryItemId, CPolygon* destPoly, bool isInstantRecharge, eCursorAnim cursorId)
{
	ShowHintForInvItem(inventoryItemId, (destPoly==NULL)?NULL:PointSystem::CreateCPoint(destPoly->center.x, destPoly->center.y, 0.0f), isInstantRecharge, cursorId);
}

void HudControl::ShowHintForInvItem(EHOInventory inventoryItemId, CPoint* destPos, bool isInstantRecharge, eCursorAnim cursorId)
{
	ShowHintForInvItem(inventoryItemId, isInstantRecharge, cursorId);

	if(destPos!=NULL)
	{
		HintGreenCircleFX->SetPos(destPos);
		HintGreenCircleFX->Start();
	}
}

void HudControl::ShowHintForSceneItem(CPoint* destPos)
{
	HintGreenCircleFX->SetPos(destPos);
	HintGreenCircleFX->Start();
}

void  HudControl::InitHintTrailEmit(const CRectangle *destRect, bool isInstantRecharge, eCursorAnim cursorId)
{
	InitHintTrailEmit(destRect->GetCenterX(), destRect->GetCenterY(), isInstantRecharge, cursorId);
}

void HudControl::InitHintTrailEmit(float x, float y, bool isInstantRecharge, eCursorAnim cursorId)
{
	if(IsHintTrailRunning)
	{
		return;
	}

	AwardManager::GetInstance()->UseHint();
	Control::Audio->QuickLoadAndPlaySFX(aSFXHintDischarge);

	if(!isInstantRecharge)
	{
		IsHintDischarging = true;

		HintChargeVFX->Start();
		for (int i = 0; i < NUM_PCOCK_FEATHERS; i++)
		{
			pCockFeathers[i]->ForcePlayAnimation(2);
		}

		isPCockWakeupPlaying = false; //if the rise up playing is active
		pCockHeadWakeIdle->SetEnable(false);

		pCockHeadWakeup->SetEnable(true);
		pCockHeadWakeup->ForcePlayAnimation(1);

		isPCockWakeDownPlaying = true;

		IsHintFullyCharged = false;
	}

	UseCurrentHint();

	HintTrailTime = 0.0f;

	IsHintTrailCircling = false;
	IsHintTrailRunning = true;	

	if( cursorId == CA_ExitDown || cursorId == CA_ExitUp || cursorId == CA_ExitLeft || cursorId == CA_ExitRight || cursorId == CA_ExitLeftDiag || cursorId == CA_ExitRightDiag)
	{
		IsDirectionalHint = true;
		if(  ProfileSystem::Instance->ActiveProfile.IsInvLocked && cursorId == CA_ExitDown)
		{
			HintDirCursor->SetXY(x-2, y-110);
			y-=100;
		}
		else
			HintDirCursor->SetXY(x-7, y-2);

		switch( cursorId )
		{
				case CA_ExitLeft  :HintDirCursor->SetRotate(90); break;
				case CA_ExitRight :HintDirCursor->SetRotate(-90); break;		
				case CA_ExitUp    :HintDirCursor->SetRotate(0);break;
				case CA_ExitDown  :HintDirCursor->SetRotate(180);break;
				default:break;
		}		
	}

	float MiddleX = (hintStartPt.x + x)/2;
	float MiddleY = (hintStartPt.y + y)/2;

	float angle = 0;
	//Control point will be perpendicular to line. around 200-400 range
	//in either +90 or -90
	float randValue = rand()%300 + 200.0f;
	if( cursorId != CA_ExitDown )
	{
		if(randValue>300)
			angle += 90;
		else
			angle -= 90; 
	}
	else
		angle += 90;	//To avoid going under the Screen during backward hint showing

	HintExplodeSuperFX->Pos.x = HintCircleSuperFX->Pos.x = hintEndPt.x = x;
	HintExplodeSuperFX->Pos.y = HintCircleSuperFX->Pos.y = hintEndPt.y = y;


	hintCtrlPt.x = MiddleX + randValue*Elixir::Cos(angle);
	hintCtrlPt.y = MiddleY - randValue*Elixir::Sin(angle);
}

bool  HudControl::ShowHintForComboInvItems()
{
	bool isAnyComboItemTaskLeft = false;
	 
	int hintItemId = InvBoxCtrl->GetActiveComboItemId();

	if(hintItemId != -1)
	{
		isAnyComboItemTaskLeft = true;
		ShowHintForInvItem((EHOInventory)hintItemId);
	}

	return isAnyComboItemTaskLeft;
}

void HudControl::RestartHintFX()
{
	IsHintFullyCharged = false;
	HintVert = ProfileSystem::Instance->ActiveProfile.HintSavedTime/10000.0f;
	//Elements[HE_Hint]->SetVertPerc(HintVert);
	if( HintVert == 1.0f )
	{
		Control::Audio->PlaySample(aSFXRefill2);

		//TODO Hint fx starts here
	}
	else if( HintVert < 1.0f && Elements[HE_Hint]->Visible )
	{
		//TODO Hint fx stops here
	}

	//Gradual Hint Increase
	float finalHintVert = HintVert;
	float hintInc = HintVert/10;

	HintVert = hintInc;
	//Hint Update works in 30, 60, 90 degrees. So Instant change will cause peacock anim update not working properly
	while (HintVert<finalHintVert)
	{
		HintVert += hintInc;
		UpdateHint();
	}

	HintVert = finalHintVert;

	UpdateHint();
}

void HudControl::InitHintAnim()
{	
	if(HintVert>=0.99f)
	{
		HintVert = 1.0f;

		pCockHeadIdle->SetEnable(false);
		pCockHeadWakeIdle->SetEnable(true);

		IsHintFullyCharged = true;		

		if(IsPuzzleScene)
			Elements[HE_Skip]->PlayAnimation(0);
		else
			Elements[HE_Hint]->PlayAnimation(0);

		IsRechargeWarningShowing = false;
		HintRechargeTextBg->SetEnable(false);
		HintRechargeWarnText->Show(false);		
	}

	hintAngle = HintVert*hintMaxAngle;

	for (int i = 0; i < NUM_PCOCK_FEATHERS; i++)
	{
		//Right is Minus. Left is Plus. Rotation Values
		//const float pCockEndRot[NUM_PCOCK_FEATHERS]	= {0, 30, 54, 86, -30, -54, -86};
		/*
			pCockFeathers[1] = HudScene->GetObjectByID("pcockfeatherL0");
			pCockFeathers[2] = HudScene->GetObjectByID("pcockfeatherL1");
			pCockFeathers[3] = HudScene->GetObjectByID("pcockfeatherL2");

			pCockFeathers[4] = HudScene->GetObjectByID("pcockfeatherR0");
			pCockFeathers[5] = HudScene->GetObjectByID("pcockfeatherR1");
			pCockFeathers[6] = HudScene->GetObjectByID("pcockfeatherR2");
		*/
		if(i==0)
			continue;

		if(hintAngle<=30.0f)
		{
			if(i<4)
				pCockFeathers[i]->SetRotate(hintAngle);
			else
				pCockFeathers[i]->SetRotate(-hintAngle);
		}
		else if(hintAngle<=60.0f)
		{			
			//except first two
			if(i!=1 && i!=4)
			{
				if(i<4)
					pCockFeathers[i]->SetRotate(hintAngle);
				else
					pCockFeathers[i]->SetRotate(-hintAngle);
			}
			else
			{
				pCockFeathers[1]->SetRotate(30);
				pCockFeathers[4]->SetRotate(-30);
			}
		}
		else
		{
			pCockFeathers[1]->SetRotate(30);
			pCockFeathers[4]->SetRotate(-30);

			pCockFeathers[2]->SetRotate(60);
			pCockFeathers[5]->SetRotate(-60);

			//only last two
			if(i==3 || i==6)
			{				
				if(i<4)
					pCockFeathers[i]->SetRotate(hintAngle);
				else
					pCockFeathers[i]->SetRotate(-hintAngle);
			}
		}
	}	
}

void HudControl::UpdateHint()
{
	if(!IsHintDischarging && !IsHintFullyCharged && HintVert>=0.99f)
	{
		//TODO Hint fx stops here
		HintVert = 1.0f;

		//Show Hint discharge fx here
		pCockHeadIdle->SetEnable(false);
		pCockHeadWakeIdle->SetEnable(false);

		IsHintFullyCharged = true;

		for (int i = 0; i < NUM_PCOCK_FEATHERS; i++)
		{
			pCockFeathers[i]->ForcePlayAnimation(1);
		}

		pCockHeadWakeup->SetEnable(true);
		pCockHeadWakeup->ForcePlayAnimation(0);

		HintChargeVFX->Start();
		isPCockWakeupPlaying = true;

		if(IsPuzzleScene)
			Elements[HE_Skip]->PlayAnimation(0);
		else
			Elements[HE_Hint]->PlayAnimation(0);

		IsRechargeWarningShowing = false;
		HintRechargeTextBg->SetEnable(false);
		HintRechargeWarnText->Show(false);

		Control::Audio->QuickLoadAndPlaySFX(aSFXHintFullCharge);
	}

#ifdef INSTANT_RECHARGE
	if(!IsHintDischarging) {
		if (HintVert >= 0.99f)
			HintVert = 1;
		else {
			HintVert += 0.5 * Control::LogicRate;
		}
	}
#endif

	hintAngle = HintVert*hintMaxAngle;

	for (int i = 0; i < NUM_PCOCK_FEATHERS; i++)
	{
		//Right is Minus. Left is Plus. Rotation Values
		//const float pCockEndRot[NUM_PCOCK_FEATHERS]	= {0, 30, 54, 86, -30, -54, -86};
		/*
			pCockFeathers[1] = HudScene->GetObjectByID("pcockfeatherL0");
			pCockFeathers[2] = HudScene->GetObjectByID("pcockfeatherL1");
			pCockFeathers[3] = HudScene->GetObjectByID("pcockfeatherL2");

			pCockFeathers[4] = HudScene->GetObjectByID("pcockfeatherR0");
			pCockFeathers[5] = HudScene->GetObjectByID("pcockfeatherR1");
			pCockFeathers[6] = HudScene->GetObjectByID("pcockfeatherR2");
		*/
		if(i==0)
			continue;

		if(hintAngle<=30.0f)
		{
			if(i<4)
				pCockFeathers[i]->SetRotate(hintAngle);
			else
				pCockFeathers[i]->SetRotate(-hintAngle);
		}
		else if(hintAngle<=60.0f)
		{			
			//except first two
			if(i!=1 && i!=4)
			{
				if(i<4)
					pCockFeathers[i]->SetRotate(hintAngle);
				else
					pCockFeathers[i]->SetRotate(-hintAngle);
			}
		}
		else
		{
			//only last two
			if(i==3 || i==6)
			{				
				if(i<4)
					pCockFeathers[i]->SetRotate(hintAngle);
				else
					pCockFeathers[i]->SetRotate(-hintAngle);
			}
		}
	}	
}

void HudControl::UnlockCharacter(int id, float delay)
{
	if(!ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[id])
	{
		ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[id] = true;
		ProfileSystem::Instance->ActiveProfile.lastUnlockedCharacter = id;

		//Hud->UnlockCharacter(eChar7_NaginMaryPast);

		//show character unlock Banner
		characterUnlockPopup->ShowPopup(delay);
		switch (id)
		{
		case eChar4_Creek:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Sir_Creek"));
			break;
		case eChar5_Grandma:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Ann_Mary"));
			break;
		case eChar6_Darkarta:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Darkarta"));
			break;
		case eChar7_NaginMaryPast:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Nagin"));
			break;
		case eChar8_AnanthaYoung:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Prince_Anantha"));
			break;
		case eChar9_Varuni:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Varuni"));
			break;
		case eChar10_Lasya:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Lasya"));
			break;
		case eChar11_Ameeha:
			characterUnlockPopup->SetText(StringTable::getStr("Exchars"), StringTable::getStr("Ameeha_&_Prince_Anantha"));
			break;
		case eChar12_Devadeva:
			characterUnlockPopup->SetText(StringTable::getStr("Exchars"), StringTable::getStr("Devadeva_&_Shukra"));
			break;
		case eChar13_Rajasthani:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Rajasthani"));
			break;
		case eChar14_Sukra:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Shukra"));
			break;
		case eChar15_Mooppan:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Mooppan"));
			break;
		case eChar16_Anasooya:
			characterUnlockPopup->SetText(StringTable::getStr("Exchar"), StringTable::getStr("Lathika"));
			break;
		default:
			break;
		}
	}
}

void HudControl::ShowMapFromJournal()
{
	JournalCtrl->HideJournal(true);
	IsDiaryShowing = false;

	MapControl->SetUpAndShowMap(true);
	IsMapShowing = true;
}

void HudControl::ShowJournalFromMap(bool showCharacterPage)
{
	MapControl->HideMap();
	IsMapShowing = false;

	JournalCtrl->ShowJournal(true, showCharacterPage);
	IsDiaryShowing = true;
}

void HudControl::Update()
{
	if( GFInstance->GFInfoLive )
		return;

	if( teleportSystem->isActive )
	{
		return;
	}

	/*
	if(GFInstance->IsActivePopup)
	{
		GFHud->AllowInput = false;
	}
	else
	{
		GFHud->AllowInput = true;
	}
	*/

	/*
	if (Elements[HE_Guide]->IsActive)
		HudScene->GetObjectByID("btnguidetext")->SetEnable(true);
	else
		HudScene->GetObjectByID("btnguidetext")->SetEnable(false);

	if (Elements[HE_Skip]->IsActive)
		HudScene->GetObjectByID("btnskiptext")->SetEnable(true);
	else
		HudScene->GetObjectByID("btnskiptext")->SetEnable(false);

	if (Elements[HE_Map]->IsActive)
		HudScene->GetObjectByID("btnmaptext")->SetEnable(true);
	else
		HudScene->GetObjectByID("btnmaptext")->SetEnable(false);

	if (Elements[HE_Menu]->IsActive)
		HudScene->GetObjectByID("btnmenutext")->SetEnable(true);
	else
		HudScene->GetObjectByID("btnmenutext")->SetEnable(false);

	if (Elements[HE_Hint]->IsActive)
		HudScene->GetObjectByID("btnhinttext")->SetEnable(true);
	else
		HudScene->GetObjectByID("btnhinttext")->SetEnable(false);
		*/

	CPoint mosPos = Control::Input->Mpos();
	bool click = Control::Input->LBclicked();

#if defined (K_ANDROID) && !defined(UNLOCKED)

	noOfHintsText->SetText(to_string(GFApp->noOfHints));

    if(adORBuyHudActive)
    {
        if(viewADButton->IsIntersect(mosPos))
        {
            if(click)
            {
                if(adType == 0)
                {
                    showADorBuyDialog(false, 0);
                    GFApp->ShowInterstitialAD();
                } else if(adType == 1)
                {
                    showADorBuyDialog(false, 1);
                    GFApp->ShowRewardedAD();
                }
            }
        }

        if(buyHintsButton->IsIntersect(mosPos))
        {
            if(click)
            {
                showADorBuyDialog(false, adType);
                GFApp->ShowHintStore();
            }
        }

        if(!adViewDialog->IsIntersect(mosPos))
        {
            if(click)
            {
                showADorBuyDialog(false, adType);
            }
        }
    }
#endif

	if(IsRechargeWarningShowing)
	{
		CPoint mPos = mosPos;
		mPos.y += 80.0f;
		HintRechargeTextBg->SetPos(&mPos);								
		mPos.x -= 2.0f;
#ifdef JAPANESE_BUILD
		mPos.y -= 16.0f;
#else
		mPos.y -= 15.0f;
#endif
		HintRechargeWarnText->SetPos(&mPos);
	}

	if(Tutor != NULL && Tutor->IsActive && Tutor->IsVisible)
	{
		Tutor->Update();

		if(Tutor->State == TUT03_Journal)
		{
			if( ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked != -1 )
			{
				if( Elements[HE_Journal]->Visible && BtnPolyDiary->Intersection(&mosPos))
				{
					PrevCursorMode = Cursor::GetMode();
					Cursor::SetMode(CA_HandPointy);
					if( click )
					{		
						Elements[HE_Journal]->PlayAnimation(HBA_Click);	
						Control::Audio->PlaySample(aSFXClick);
						Control::Audio->QuickLoadAndPlaySFX(aSFXDiaOpen);

						IsDiaryShowing = true;
						JournalCtrl->ShowJournal();

						ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT03_Journal] = true;
						Tutor->HideTutorial();
					}
					else if( Elements[HE_Journal]->GetAnimation() != HBA_Hover )
					{
						Control::Audio->PlaySample(aSFXMouseover);
						Elements[HE_Journal]->PlayAnimation(HBA_Hover);
					}
				}
				else
				{
					Elements[HE_Journal]->PlayAnimation(HBA_Idle);
					Cursor::SetMode(CA_Normal);
				}
			}
		}
		else if(Tutor->State == TUT13_HintTutorial)
		{
			bool IsHintActive = ( Elements[HE_Hint]->Visible && !GFInstance->IsActivePopup );

			if( IsHintActive && BtnRectHint.Intersection(&mosPos) )
			{
				PrevCursorMode = Cursor::GetMode();
				Cursor::SetMode(CA_HandPointy);
				if( click )
				{		
                    Elements[HE_Hint]->PlayAnimation(HBA_Click);	
					
					//clicking neighbourhint must close old!!
					if( InvBoxCtrl->IsOpen() )//mouse over "neighbour hint" then turn over invbox!!
					{
						InvBoxCtrl->NoAutoClose = false;
  						InvBoxCtrl->Close();
					}
					if( HintVert == 1.0f )				
					{
						SendMessage(ActionProcessed, ID, HOSM_Hint);

						ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT13_HintTutorial] = true;
						Tutor->HideTutorial();
					}
				}
				else if( Elements[HE_Hint]->GetAnimation() != HBA_Hover )
				{
					Control::Audio->PlaySample(aSFXMouseover);
                    Elements[HE_Hint]->PlayAnimation(HBA_Hover);
				}
			}
			else
			{
				Elements[HE_Hint]->PlayAnimation(HBA_Idle);
				Cursor::SetMode(CA_Normal);
			}
		}
		else if(Tutor->State == TUT14_MapTutorial)
		{
			//HE_Map    
			if( BtnPolyMap->Intersection(&mosPos) )
			{
				Cursor::SetMode(CA_HandPointy);
				if( click )
				{		
                    Elements[HE_Map]->PlayAnimation(HBA_Click);	
					
					Control::Audio->PlaySample(aSFXClick);
					Control::Audio->QuickLoadAndPlaySFX(aSFXDiaOpen);

					MapControl->SetUpAndShowMap();
					IsMapShowing = true;

					ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT14_MapTutorial] = true;
					Tutor->HideTutorial();
				}
				else if( Elements[HE_Map]->GetAnimation() != HBA_Hover )
				{
					Control::Audio->PlaySample(aSFXMouseover);
                    Elements[HE_Map]->PlayAnimation(HBA_Hover);
				}
			}
			else
			{
				Elements[HE_Map]->PlayAnimation(HBA_Idle);
				Cursor::SetMode(CA_Normal);
			}
		}
		else if(Tutor->State == TUT16_SkipPuzzle)
		{
			bool IsSkipActive = ( Elements[HE_Skip]->Visible && !GFInstance->IsActivePopup );

			if( IsSkipActive && BtnRectSkip.Intersection(&mosPos) )
			{
				PrevCursorMode = Cursor::GetMode();
				Cursor::SetMode(CA_HandPointy);
				if( click )
				{		
                    Elements[HE_Skip]->PlayAnimation(HBA_Click);	
					
					//clicking neighbourhint must close old!!
					if( InvBoxCtrl->IsOpen() )//mouse over "neighbour hint" then turn over invbox!!
					{
						InvBoxCtrl->NoAutoClose = false;
  						InvBoxCtrl->Close();																  
					}
					if( HintVert == 1.0f )				
					{
						SendMessage(ActionProcessed, ID, HOSM_Skip);

						ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT16_SkipPuzzle] = true;
						Tutor->HideTutorial();
					}
				}
				else if( Elements[HE_Skip]->GetAnimation() != HBA_Hover )
				{
					Control::Audio->PlaySample(aSFXMouseover);
                    Elements[HE_Skip]->PlayAnimation(HBA_Hover);
				}
			}
			else
			{
				bool IsIdle = true;
				if( IsIdle )
				{
					Elements[HE_Skip]->PlayAnimation(HBA_Idle);		
				}

				Cursor::SetMode(CA_Normal);
			}
		}
		else if(Tutor->State == TUT15_MapDetailsTutorial)
		{
		}
		return;
	}

	if(Tutor != NULL && Tutor->IsActive)
	{
		if(Tutor->State == TUT02_TakeJames)
		{
			//To Avoid clicking on Map or Journal or Hint while Taking james anim
			return;
		}
		else if(Tutor->State == TUT08_KnifeTakeFromBag && IsComboInvPopupShowing)
		{
			//To Avoid clicking on Map or Journal or Hint while in combo inventory mode
			return;
		}
	}

	if (IsDiaryShowing || IsGuideShowing || IsMemoryFXTrailing)
	{
		return;
	}

	if(IsMapShowing)
	{
		if( MapControl->IsMapClosed )
		{
			if( MapControl->IsOtherSceneClicked )
			{
				SendMessage(ActionProcessed, ID, MAP_NEW_AREA);
			}
			IsMapShowing = false;
		}
		return;
	}

	if( PopupState != Popup_Inactive )
	{
		PrevCursorMode = Cursor::GetMode();		
		if( !InvBoxCtrl->IsHoverItem )
			Cursor::SetMode(CA_Normal);
		switch( (int)PopupState )
		{
			case Popup_InGameMenu:UpdateInGameMenuPopup();
				return;
			case Popup_Options   :UpdateOptionsPopup();
				return;
			case Popup_ModeSelect:UpdateSelectModePopup();
				return;
			case Popup_Restart:UpdateRestartPopup();
				return;
		}		
	}

	bool IsAnyElementCollided = false;
	if( Cursor::Visible )
	{
		if(AllowInput )
		{
			
			for( int i=0; i < HE_Max; ++i )
			{
				if( i == HE_MessageBanner )//skip Non-interactive hud elements!! 
					continue;
				
				if( Elements[i] != NULL )
				{ 
					bool IsCurElementCollided = false;
					switch( i )
					{
						case HE_Map    : IsCurElementCollided = BtnPolyMap->Intersection(&mosPos);
										break;
						case HE_Hint   :{
											bool IsHintActive = ( Elements[HE_Hint]->Visible && !GFInstance->IsActivePopup );
										
											if( IsHintActive )
											{
												IsCurElementCollided = BtnRectHint.Intersection(&mosPos);
												if(IsCurElementCollided)
												{
													if( HintVert < 1.0f )
													{
														IsCurElementCollided = false;
														if(!IsRechargeWarningShowing)
														{
															CPoint temp = mosPos; 
															temp.y += 80.0f;
															HintRechargeTextBg->SetPos(&temp);
															temp.x -= 2.0f;
															#ifdef JAPANESE_BUILD
																	temp.y -= 16.0f;
															#else
																	temp.y -= 15.0f;
															#endif
															HintRechargeWarnText->SetPos(&temp);

															HintRechargeTextBg->SetEnable(true);
															#ifndef TOUCH_UI
															HintRechargeTextBg->Show(true);
															HintRechargeWarnText->Show(true);
															#else															
															HintRechargeTextBg->Show(false);
															HintRechargeWarnText->Show(false);
															#endif
															IsRechargeWarningShowing = true;
														}
													}
												}
												else
												{
													if(IsRechargeWarningShowing)
													{
														IsRechargeWarningShowing = false;
														HintRechargeTextBg->SetEnable(false);
														HintRechargeTextBg->Show(false);
														HintRechargeWarnText->Show(false);
													}
												}
											}
										}
										break;
						case HE_Skip   :{										
											if( Elements[HE_Skip]->Visible )
											{
												IsCurElementCollided = BtnRectSkip.Intersection(&mosPos);												
												if(IsCurElementCollided)
												{
													if( HintVert < 1.0f )
													{
														IsCurElementCollided = false;
														if(!IsRechargeWarningShowing)
														{
															HintRechargeTextBg->SetPos(&mosPos);
															HintRechargeTextBg->SetEnable(true);
															HintRechargeWarnText->SetPos(&mosPos);
															#ifndef TOUCH_UI
															HintRechargeTextBg->Show(true);
															HintRechargeWarnText->Show(true);
															#else															
															HintRechargeTextBg->Show(false);
															HintRechargeWarnText->Show(false);
															#endif
															IsRechargeWarningShowing = true;
														}
													}
												}
												else
												{
													if(IsRechargeWarningShowing)
													{
														IsRechargeWarningShowing = false;
														HintRechargeTextBg->SetEnable(false);
														HintRechargeTextBg->Show(false);
														HintRechargeWarnText->Show(false);
													}
												}
											}
										}
										break;
						case HE_Journal:if( ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked != -1 )
											if( Elements[HE_Journal]->Visible )
											{
												IsCurElementCollided = BtnPolyDiary->Intersection(&mosPos); 
											}
										 break;
						case HE_Guide   : if( Elements[HE_Guide]->Visible )
										  {
#ifdef _CEBUILD
											 IsCurElementCollided = BtnRectGuide.Intersection(&mosPos);
#endif
										  }
										  break;
						case HE_Menu   : if( Elements[HE_Menu]->Visible )
										 {
											 IsCurElementCollided = BtnRectMenu.Intersection(&mosPos); 											
										 }
										 break;
						default: IsCurElementCollided = Elements[i]->Collide(&mosPos); break;
					}

					if( IsCurElementCollided )
					{
						IsAnyElementCollided = true;
						PrevCursorMode = Cursor::GetMode();
						Cursor::SetMode(CA_HandPointy);
						if( click )
						{		
                            Elements[i]->PlayAnimation(HBA_Click);
                            ProcessInteraction(i);//sounds played inside!!							
						}
						else if( Elements[i]->GetAnimation() != HBA_Hover )
						{
							Control::Audio->PlaySample(aSFXRefill2);
                            Elements[i]->PlayAnimation(HBA_Hover);
						}
					}
					else
					{
						Elements[i]->PlayAnimation(HBA_Idle);
						if( HintVert < 1.0f && (i==HE_Hint || i==HE_Skip) )
							Elements[i]->PlayAnimation(2);
					}
				}
			}

			//No collision now and some cursor still in prev mode reset!!
			if( !IsAnyElementCollided )
			{
				//if Crystal ball collected and clicked
				if(ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked && crystalOrbPoly->Intersection(&mosPos))
				{
					if (click && HudScene->GetObjectByID("orb")->GetAnimation()==0)
					{
						ShowBannerText("Excrystal");
					}
				}
				if( PrevCursorMode != -1 )
				{
					Cursor::SetMode(PrevCursorMode);
					PrevCursorMode = -1;
				}
			}
		}
	}

	if( !IsTakeAndUseHOScene && !IsPuzzleScene && !IsHOScene && isHUDVisible && Elixir::PointInsidePoly(MAX_HUD_PANEL_POINTS,HudX,HudY,(int)mosPos.x,(int)mosPos.y) )
	{
		if( !IsAnyElementCollided )//Not collided with any hud elements - Diary,Map,Menu,Hint,Guide
			if( !InvBoxCtrl->IsHoverItem )
				if( !InvBoxLockRect.Intersection(&mosPos) && !IBLeftArrRect.Intersection(&mosPos) && !IBRightArrRect.Intersection(&mosPos) )
					if( !InvBoxCtrl->IsDragging )
					{
						if( ProfileSystem::Instance->ActiveProfile.IsInvLocked )
							Control::Input->ClearMouse();
						else if( InvBoxRect.Intersection(&mosPos) )
						{
							if( InvBoxCtrl->InvBox->Pos.y < InvBoxDownPos.y  )//Inevntory activated!!
								Control::Input->ClearMouse();
						}
						else
							Control::Input->ClearMouse();
					}
	}
}

void HudControl::ShowMap()
{
	MapControl->SetUpAndShowMap();
	IsMapShowing = true;
}

void HudControl::CallPauseMenu()
{
	ProcessInteraction(HE_Menu);

}

void HudControl::ProcessInteraction(int i)
{
	switch(i)
	{
		case HE_Guide:
			if(!IsComboInvPopupShowing){
				Control::Audio->PlaySample(aSFXClick);
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaOpen);
				IsGuideShowing = true;
				GFInstance->IsActivePopup = true;
				GuideCtrl->ShowStrategyGuide();
			}
			break;
		case HE_Journal:
			if(!IsComboInvPopupShowing){
				Control::Audio->PlaySample(aSFXClick);
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaOpen);
				IsDiaryShowing = true;
				JournalCtrl->ShowJournal();
			}
			break;
		case HE_Menu:
			if(!IsComboInvPopupShowing){
				Control::Audio->PlaySample(aSFXClick);
				ShowInGameMenuPopup();
				Elements[HE_Menu]->PlayAnimation(HBA_Idle);
			}
			break;
		case HE_Map:
			if(!IsComboInvPopupShowing){
				Control::Audio->PlaySample(aSFXClick);
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaOpen);
				SendMessage(ActionProcessed, ID, MAP_HUD_CLICK);
				/*MapControl->SetUpAndShowMap();
				IsMapShowing = true;*/
			}
			break;
		case HE_Hint:
			K_LOG("Sac: Entering Hint.....");
			if(!IsComboInvPopupShowing){
				K_LOG("Sac: Entering Hint IsComboInvPopupShowing.....");
				if( HintVert == 1.0f )
				{					
					K_LOG("Sac: Sending Actionpressed Hint.....");
                    SendMessage(ActionProcessed, ID, HOSM_Hint);
				}
			}
			else
			{
				K_LOG("Sac: IsComboInvPopupShowing not happening.....");
				//is combo Inv showing Show the hint.
				if(!comboInvSystemCtrl->ShowHint())
				{
					K_LOG("Sac: Is Combo Inv Showing Hint ??.....");
					//Some sort of error if this shows up
					ShowNothingToDoMsgBanner();
				}
			}
			break;
		case HE_Skip:
			//puzzle scene has skip button
#if defined (K_ANDROID) && !defined(UNLOCKED)
			if(GFApp->noOfHints >= 5)
			{
			    if(!IsHintDischarging)
			    {
                    GFApp->noOfHints -= 5;
                    GFApp->UpdateUserHint(to_string(GFApp->noOfHints));
                    ProfileSystem::Instance->WriteProfileSystemData();

                    IsHintDischarging = true;
                    Control::Audio->QuickLoadAndPlaySFX(aSFXHintDischarge);
                    SendMessage(ActionProcessed, ID, HOSM_Skip);
                }
			}
			else
			{
                GFHud->showADorBuyDialog(true, 1);
				//GFApp->ShowRewardedAD();
			}
            break;
#else
			IsHintDischarging = true;
			Control::Audio->QuickLoadAndPlaySFX(aSFXHintDischarge);
			SendMessage(ActionProcessed, ID, HOSM_Skip);
			break;
#endif

	}
	
}

void HudControl::TransOutBanner()
{
	if( BannerState == eHMS_TransIn )
	{
		BannerTimer = 0.0f;
		BannerState = eHMS_TransOut;
	}
	else
		TransOutFactor = 2.0f;
}

void HudControl::UpdateBanner()
{
	if( BannerState == eHMS_Idle )
		return;

	if( BannerState == eHMS_TransIn )
	{
		BannerAlpha += 2.0f * Control::LogicRate;

		if( BannerAlpha >= MAX_BANNER_ALPHA )
		{
			BannerAlpha = MAX_BANNER_ALPHA;
		}

		BannerTimer += Control::LogicRate;
		if( BannerTimer >= 6.0f  /*|| (BannerAlpha == MAX_BANNER_ALPHA && Control::Input->LBclicked())*/ )
		{
			BannerTimer = 0.0f;
			BannerState = eHMS_TransOut;
		}
	}
	else
	{
		BannerAlpha -= TransOutFactor * Control::LogicRate;

		if( BannerAlpha <= 0.0f )
		{
			BannerAlpha = 0.0f;
			BannerState = eHMS_Idle;
			Elements[HE_MessageBanner]->Show(false);
			MessageText->Show(false);

			//Banner CloseNotify - if needed
			//SendMessage(ActionProcessed, ID, HOSM_BannerClose );
		}
	}

	Elements[HE_MessageBanner]->SetAlpha(BannerAlpha);
	float MTAlpha = ( BannerAlpha > MAX_BANNERTEXT_ALPHA )? MAX_BANNERTEXT_ALPHA: BannerAlpha;
	MessageText->SetAlpha(MTAlpha);
}

void HudControl::ProcessMessage(sMessage message)
{	
	if( message.Value == INVBOX_CLOSE  )
	{
		//if( !ProfileSystem::Instance->ActiveProfile.IsInvLocked )
		   // Control::Audio->PlaySample(aSFXInvItemPick);
	}
	if( message.Value == HOSM_TeleportClosed  )
	{
		ProfileSystem::Instance->ActiveProfile.CurrentArea = teleportSystem->targetAreaID;
		SendNewAreaMessage();
	}
	if( message.Value == HOSM_ComboInvClosed  )
	{
		if(Tutor != NULL && Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT09_RosePopupZoom] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
		{
			Tutor->ShowTutorial(TUT09_RosePopupZoom);
		}

		if(Tutor != NULL && Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT18_ComboInvZoomLook])
		{
			//if beetle tail taken from painting
			if(ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken && ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken)
			{
				Tutor->ShowTutorial(TUT18_ComboInvZoomLook);
			}
		}
			
		IsComboInvPopupShowing = false;
		SendMessage(ActionProcessed, ID, UPDATE_AREA_TASK);
	}
	else if( message.Value == FOUNDINVPOPUP_CLOSE )
	{
		foundInvPopupCtrl->Close();
	}	
	else if( message.Value == INV_CLOSE )
	{
		BannerState = eHMS_TransOut;
		SendMessage(ActionProcessed, ID, INV_CLOSE );
	}
	else if( message.Value == VAL_FLYING )
	{
		SendMessage(ActionProcessed, ID, VAL_FLYING );
	}
	else if( message.Value == VAL_CLOSE )
	{
		BannerState = eHMS_TransOut;

		//bool IsInvPoppingUp = false;
		//if( InventoryQueued )
		//{
		//	//InventoryQueued = false;
		//	ShowInventory(&InventoryIDs);
		//	InventoryIDs.clear();
		//}

		SendMessage(ActionProcessed, ID, VAL_CLOSE);		
	}
	else if( message.Value == INVBOX_OPEN )
	{
	}
	else if ( message.Value == INVBOX_DROP )
	{
		if(IsComboInvPopupShowing)
			comboInvSystemCtrl->HandleInventoryDrop();
		else
			SendMessage(ActionProcessed, ID, INVBOX_DROP);//Handle invbox drop
		Control::Input->ClearMouse();
	}
	else if( message.Value == INVBOX_MOUSEOVER )
	{
		/*
		int i = InvBoxCtrl->GetMouseOverItem();
		if( i >= 0 && i < eInv_Max )
		{
			#ifdef JAPANESE_BUILD
				ShowMessageBanner( GFInstance->JapInventoryDescrips(i) );
			#else
				ShowMessageBanner( InventoryDescrips[i] );
			#endif
		}
		*/
	}	
	else if ( message.Value == INVBOX_STARTDRAG )
	{
		SendMessage(ActionProcessed, ID, INVBOX_STARTDRAG);
	}
	else if ( message.Value == INVBOX_CANCELDRAG )
	{
		SendMessage(ActionProcessed, ID, INVBOX_CANCELDRAG);
	}
	else if ( message.Value == INVBOX_ENDDRAG )
	{
		SendMessage(ActionProcessed, ID, INVBOX_ENDDRAG);
	}

}

void HudControl::ShowNothingToDoMsgBanner()
{
	K_LOG("Sac: Show nothing to do message from Hint.....");
	ShowBannerText("Exhere");

}

void HudControl::ShowBannerText(const std::string& keyString, int SFXId)
{
	TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;
	
	MessageText->Show(true);
	if(StringTable::IsKeyFound(keyString))
	{
		MessageText->LoadFont(StringTable::GetFont(keyString));
	}
	MessageText->SetText(StringTable::getStr(keyString));
	Elements[HE_MessageBanner]->Show(true);
	

	Elements[HE_MessageBanner]->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		//SFX_NIL - not handled!!			
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;			
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}
void HudControl::ShowBannerDirectText(const std::string& textString, int SFXId)
{
	TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;
	
	MessageText->Show(true);
	MessageText->SetText(textString);
	Elements[HE_MessageBanner]->Show(true);
	

	Elements[HE_MessageBanner]->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		//SFX_NIL - not handled!!			
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;			
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}

void HudControl::ShowBannerText( int InfoTxtId, int SFXId )
{	
	if(ProfileSystem::Instance->ActiveProfile.IsHidingTips)
		return;
	TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;
	
	MessageText->Show(true);
	if(StringTable::IsKeyFound(InfoBanner::getBanner(EInfoTexts(InfoTxtId))))
	{
		MessageText->LoadFont(StringTable::GetFont(InfoBanner::getBanner(EInfoTexts(InfoTxtId))));
	}
	MessageText->SetText(StringTable::getStr(InfoBanner::getBanner(EInfoTexts(InfoTxtId))));	
	Elements[HE_MessageBanner]->Show(true);

	Elements[HE_MessageBanner]->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		//SFX_NIL - not handled!!			
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;			
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}

void HudControl::ShowMemoryObjectCollected_SE(int memID, CPoint *pos)
{	
	ShowBannerText(MemoryBannerTexts[memID]);
	Control::Audio->PlaySample(aSFXMagic2);
	ProfileSystem::Instance->ActiveProfile.NumMemoriesCollected += 1;
	ProfileSystem::Instance->ActiveProfile.LastMemoryCollected = memID;
	ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[memID] = true;
	//Show Some FX at pos
	paptusFX = new SuperFX("PRT\\UI\\paptus.INI", pos, 1100); 
	paptusFX->Start();

	paptusglowFX = new SuperFX("PRT\\UI\\paptusglow.INI", pos, 1100); 
	paptusglowFX->Start();
}

void HudControl::CollectMysteryFlower(int areaID, CPoint pos)
{
	if(!ProfileSystem::Instance->ActiveProfile.IsPaptusL1S2Collected && !ProfileSystem::Instance->ActiveProfile.IsPaptusL1S2Collected)
		ShowBannerText("L1flower");

	switch (areaID)
	{
	case eArea_L1_S2_Frontyard:
		ProfileSystem::Instance->ActiveProfile.IsPaptusL1S2Collected = true;
		break;
	case eArea_L1_S3_Foyer:
		ProfileSystem::Instance->ActiveProfile.IsPaptusL1S3Collected = true;
		break;
	case eArea_L2_S6_TunnelEnd:
		ProfileSystem::Instance->ActiveProfile.IsPaptusL2S6Collected = true;
		break;
	case eArea_L3_S5_Cliff:
		ProfileSystem::Instance->ActiveProfile.IsPaptusL3S5Collected = true;
		break;
	case eArea_L4_S3_Cave:
		ProfileSystem::Instance->ActiveProfile.IsPaptusL4S3Collected = true;
		break;
	case eArea_L5_S5_MountainBelow:
		ProfileSystem::Instance->ActiveProfile.IsPaptusL5S5Collected = true;
		break;
	case eArea_L6_S1_DescriptionHO:
		ProfileSystem::Instance->ActiveProfile.IsPaptusL6S1Collected = true;
		break;
	case eArea_L6_S1_FloatingIsland1:
		ProfileSystem::Instance->ActiveProfile.IsPaptusL6S1popCollected = true;
		break;
	case eArea_L7_S3_Haven:
		ProfileSystem::Instance->ActiveProfile.IsPaptusL7S3Collected = true;
		break;

	default:
		break;
	}

	//Show Some FX at pos
	paptusFX = new SuperFX("PRT\\UI\\paptus.INI", &pos, 1100); 
	paptusFX->Start();

	paptusglowFX = new SuperFX("PRT\\UI\\paptusglow.INI", &pos, 1100); 
	paptusglowFX->Start();
}

void HudControl::ShowMessageBanner( const std::string& message,int SFXId)
{	
	TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;
	
	MessageText->Show(true);
	MessageText->SetText(message);	
	Elements[HE_MessageBanner]->Show(true);

	Elements[HE_MessageBanner]->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		//SFX_NIL - not handled!!			
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;			
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}

void HudControl::ShowMessageBanner( int InfoTxtId, int SFXId )
{	
	if(ProfileSystem::Instance->ActiveProfile.IsHidingTips)
		return;
	TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;
	
	MessageText->Show(true);
	MessageText->SetText(InfoBanner::getBanner(EInfoTexts(InfoTxtId)));	
	Elements[HE_MessageBanner]->Show(true);

	Elements[HE_MessageBanner]->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		//SFX_NIL - not handled!!			
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;			
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}


bool HudControl::HideMessageBanner(void)
{
	bool WasActiveMessage = Elements[HE_MessageBanner]->Visible;

	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;
	Elements[HE_MessageBanner]->Show(false);
	MessageText->Show(false);

	return WasActiveMessage;
}

bool HudControl::IsInvBoxOpen()
{
	return InvBoxCtrl->IsOpen();
}

bool HudControl::IsDraggingItem()
{
	return InvBoxCtrl->IsDragging;
}

bool HudControl::Collide(const CPoint* collidepos)
{
	bool Collided = false;

	for(int i=0; i < HE_Max; ++i)
	{
		if(Elements[i] == NULL)
			continue;

		if(Elements[i]->Collide(collidepos))
		{
			Collided = true;
			break;
		}
	}


	return Collided;
}

void HudControl::SendUpdateSceneMessage()
{
	SendMessage(ActionProcessed, ID, UPDATE_SCENE_OBJECTS);
}

void HudControl::TakeInventoryItem(int invItemId, CPoint pos )
{
	SendMessage(ActionProcessed, ID, UPDATE_SCENE_OBJECTS);
	InvBoxCtrl->Activate(1);
	std::vector<int> vecInvIds;
	vecInvIds.push_back(invItemId);
	ProfileSystem::Instance->ActiveProfile.Inventory[invItemId] = true;
	ShowInventoryDirect(&vecInvIds, pos );
	InvBoxCtrl->UpdateAllComboInvItems();
}


void HudControl::TakeInventoryItem(int invItemId)
{
	SendMessage(ActionProcessed, ID, UPDATE_SCENE_OBJECTS);

	InvBoxCtrl->Activate(1);
	std::vector<int> vecInvIds;
	vecInvIds.push_back(invItemId);
	ProfileSystem::Instance->ActiveProfile.Inventory[invItemId] = true;
	ShowInventory(&vecInvIds);
}

void HudControl::TakeInventoryItem(int invItemId, bool isMultipleItems)
{
	SendMessage(ActionProcessed, ID, UPDATE_SCENE_OBJECTS);

	InvBoxCtrl->Activate(1);
	std::vector<int> vecInvIds;
	vecInvIds.push_back(invItemId);
	ProfileSystem::Instance->ActiveProfile.Inventory[invItemId] = true;
	ShowInventory(&vecInvIds, true);
}

void HudControl::ShowInventory(std::vector<int>* vecInvIds)
{
	AwardManager::GetInstance()->CollectInventoryItem();
	Control::Audio->PlayCollectFX();
	std::string foundMsg;

	if(isMultipleItemsFlag)
		foundMsg = StringTable::getStr("Exyous");
	else
		foundMsg = StringTable::getStr("Exyouf");

	isMultipleItemsFlag = false;

	if( vecInvIds->size() > 0 )
	{
		if(vecInvIds->at(0)<eInv_Max)
			foundMsg += StringTable::getStr(InventoryDescrips[vecInvIds->at(0)]);
	}

		if( vecInvIds->size() > 1 )
			ShowBannerText( "Exfound");
		else
			ShowMessageBanner(foundMsg);
	
	foundInvPopupCtrl->Activate(vecInvIds);
}

void HudControl::ShowInventory(std::vector<int>* vecInvIds, bool isMultipleItems)
{
	isMultipleItemsFlag = true;
	ShowInventory(vecInvIds);
}

 void HudControl::ShowInventoryDirect(std::vector<int>* vecInvIds, CPoint pos)
{
	AwardManager::GetInstance()->CollectInventoryItem();
	Control::Input->ClearMouse(); // To avoid clicks over overlapping objects
	foundInvPopupCtrl->ActivateDirect(vecInvIds, pos);
}

bool HudControl::GetDropInfo(CPoint& hotspot, int& itemVal)
{
	return InvBoxCtrl->GetDropInfo(hotspot, itemVal);
}

void HudControl::CloseInventory()
{
	 //Control::Audio->PlaySample(aSFXClick);
	InvBoxCtrl->ExternalClose();	
}

void HudControl::ReleaseDrag()
{
	InvBoxCtrl->ReleaseDrag();
}

void HudControl::GlowDragObject(bool glow)
{
	//InvBoxCtrl->GlowDragInvObject(glow);
}

void HudControl::CancelDrag()
{
	
	 Control::Audio->PlaySample(aSFXInvWrongDrop);//Wrong Inv Drop Sound
	InvBoxCtrl->CancelDrag();
}



void HudControl::UpdateOptionsPopup()
{
	if( PopupCounter == 0 )
	{
		PopupAlpha += 2.0f * Control::LogicRate;

		if( PopupAlpha >= 1.0f )
		{
			PopupCounter = 1;
			PopupAlpha = 1.0f;							
		}
		InGameMenuPopupScene->SetSceneAlpha(1.0f-PopupAlpha/2.0f);
		OptionsPopupScene->SetSceneAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )
	{
		CObject *Button = OptionsPopupScene->GetObjectByID("BtnOk");
		//CObject *CancelButton = OptionsPopupScene->GetObjectByID("BtnCancel");
		modeBtnOver->SetEnable(false);
		CPoint mousePos = Control::Input->Mpos();
		if( CheckPopupButton(Button) == Popup_Click) // || CheckPopupButton(CancelButton) == Popup_Click )
		{
			PopupCounter++;
			return;
		}
	//	else if( ModeChangeTextRect.Intersection(&Control::Input->Mpos()))
	// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
#ifndef K_ANDROID
		else if( ModeChangeTextRect.Intersection(&mousePos))
		{
			modeBtnOver->SetEnable(true);
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				modeBtnOver->SetEnable(false);
				Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
				ShowSelectModePopup();
				return;
			}
		}
#endif
		else
		{							
			CPoint mosPos = Control::Input->Mpos();	
			bool clicked = Control::Input->LBclicked();
	
			//Music knob
			if( !IsMusicKnobDragStarted )
			{
				CObject *Knob = OptionsPopupScene->GetObjectByID("musicSlider");
				MusicDragOrigin = Control::Input->Mpos();
				if( Knob->Collide(&MusicDragOrigin) )
				{
					Knob->PlayAnimation(1);
					if( Cursor::GetMode() != CA_HandPointy )
					{
						if( PrevCursorMode != CA_HandPointy )
							Control::Audio->PlaySample(aSFXRefill2);
						Cursor::SetMode(CA_HandPointy);
					}
					if( Control::Input->LBjustdown() )
					{				
						IsMusicKnobDragStarted = true;
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderClick);
					}
					return;
				}
				else
					Knob->PlayAnimation(0);
			}
			else
			{
				CObject *Knob = OptionsPopupScene->GetObjectByID("musicSlider");
				CPoint NewMusicKnobPos = Control::Input->Mpos();														
				if(MusicSliderAreaRect.Intersection(&NewMusicKnobPos))
				{
					if( Cursor::GetMode() != CA_HandPointy )
					{
						if( PrevCursorMode != CA_HandPointy )
							Control::Audio->PlaySample(aSFXRefill2);
						Cursor::SetMode(CA_HandPointy);
					}
					if( Control::Input->LBdown() == false )
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderMove);
						IsMusicKnobDragStarted = false;
						Knob->PlayAnimation(0);
					}
					else
					{
						Control::Audio->PlaySample(aSFXCrystal);
						if( NewMusicKnobPos.x < KNOB_STARTX )
							NewMusicKnobPos.x = KNOB_STARTX;
						else if( NewMusicKnobPos.x > KNOB_ENDX )
							NewMusicKnobPos.x = KNOB_ENDX;
						NewMusicKnobPos.y = MUSICKNOB_POSY;							
						OptionsPopupScene->GetObjectByID("musicSlider")->SetPos(&NewMusicKnobPos);
						ProfileSystem::Instance->ActiveProfile.MusicVol = int(((NewMusicKnobPos.x-KNOB_STARTX)*100.0f)/(KNOB_ENDX-KNOB_STARTX));
						if( ProfileSystem::Instance->ActiveProfile.IsMuted )
							Control::Audio->GlobalMusicVol = ProfileSystem::Instance->ActiveProfile.MusicVol;
						else
						{
							Control::Audio->SetGlobalMusicVolume(ProfileSystem::Instance->ActiveProfile.MusicVol);
						}
					}
					return;
				}
			}

			//Sound knob
			if( !IsSoundKnobDragStarted )
			{
				CObject *Knob = OptionsPopupScene->GetObjectByID("sfxSlider");					
				SoundDragOrigin = Control::Input->Mpos();	
				if(Knob->Collide(&SoundDragOrigin))
				{
					Knob->PlayAnimation(1);
					if( Cursor::GetMode() != CA_HandPointy )
					{
						if( PrevCursorMode != CA_HandPointy )
							Control::Audio->PlaySample(aSFXRefill2);
						Cursor::SetMode(CA_HandPointy);
					}
					if( Control::Input->LBjustdown() )
					{				
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderClick);
						IsSoundKnobDragStarted = true;
					}
					return;
				}
				else
					Knob->PlayAnimation(0);
			}
			else
			{
				CObject *Knob = OptionsPopupScene->GetObjectByID("sfxSlider");
				CPoint NewSoundKnobPos = Control::Input->Mpos();
				if(SFXSliderAreaRect.Intersection(&NewSoundKnobPos))
				{
					if( Cursor::GetMode() != CA_HandPointy )
					{
						if( PrevCursorMode != CA_HandPointy )
							Control::Audio->PlaySample(aSFXRefill2);
						Cursor::SetMode(CA_HandPointy);
					}
					if( Control::Input->LBdown() == false )
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderMove);
						IsSoundKnobDragStarted = false;
						Knob->PlayAnimation(0);
					}
					else
					{						
						Control::Audio->PlaySample(aSFXCrystal);
						if( NewSoundKnobPos.x < KNOB_STARTX )
							NewSoundKnobPos.x = KNOB_STARTX;
						else if( NewSoundKnobPos.x > KNOB_ENDX )
							NewSoundKnobPos.x = KNOB_ENDX;
						NewSoundKnobPos.y = SOUNDKNOB_POSY;							
						OptionsPopupScene->GetObjectByID("sfxSlider")->SetPos(&NewSoundKnobPos);
						ProfileSystem::Instance->ActiveProfile.SFXVol = int(((NewSoundKnobPos.x-KNOB_STARTX)*100.0f)/(KNOB_ENDX-KNOB_STARTX));
						if( ProfileSystem::Instance->ActiveProfile.IsMuted )
							Control::Audio->GlobalSFXVol = ProfileSystem::Instance->ActiveProfile.SFXVol;
						else
							Control::Audio->SetGlobalSFXVolume(ProfileSystem::Instance->ActiveProfile.SFXVol);
					}
					return;
				}
			}

			//Voice Knob
			if( !IsVoiceKnobDragStarted )
			{
				CObject *Knob = OptionsPopupScene->GetObjectByID("voiceSlider");
				VoiceDragOrigin = Control::Input->Mpos();	
				if(Knob->Collide(&VoiceDragOrigin))
				{
					Knob->PlayAnimation(1);
					if( Cursor::GetMode() != CA_HandPointy )
					{
						if( PrevCursorMode != CA_HandPointy )
							Control::Audio->PlaySample(aSFXRefill2);
						Cursor::SetMode(CA_HandPointy);
					}
					if( Control::Input->LBjustdown() )
					{				
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderClick);
						Control::Audio->LoadSample(aVOMenuTest,	AudioVO);
						Control::Audio->PlaySample(aVOMenuTest);
						IsVoiceKnobDragStarted = true;
					}
					return;
				}
				else
					Knob->PlayAnimation(0);
			}
			else
			{
				CObject *Knob = OptionsPopupScene->GetObjectByID("voiceSlider");
				CPoint NewKnobPos = Control::Input->Mpos();
				if(VoiceSliderAreaRect.Intersection(&NewKnobPos))
				{
					if( Cursor::GetMode() != CA_HandPointy )
					{
						if( PrevCursorMode != CA_HandPointy )
							Control::Audio->PlaySample(aSFXRefill2);
						Cursor::SetMode(CA_HandPointy);
					}
					if( Control::Input->LBdown() == false )
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderMove);
						Knob->PlayAnimation(0);
						IsVoiceKnobDragStarted = false;
					}
					else
					{
						if(!Control::Audio->IsPlaying(aVOMenuTest))
						{
							Control::Audio->PlaySample(aVOMenuTest);
						}

						Control::Audio->PlaySample(aSFXCrystal);
						if( NewKnobPos.x < KNOB_STARTX )
							NewKnobPos.x = KNOB_STARTX;
						else if( NewKnobPos.x > KNOB_ENDX )
							NewKnobPos.x = KNOB_ENDX;
						NewKnobPos.y = VOICEKNOB_POSY;							
						OptionsPopupScene->GetObjectByID("voiceSlider")->SetPos(&NewKnobPos);
						ProfileSystem::Instance->ActiveProfile.VoiceVol = int(((NewKnobPos.x-KNOB_STARTX)*100.0f)/(KNOB_ENDX-KNOB_STARTX));
						if( ProfileSystem::Instance->ActiveProfile.IsMuted )
							Control::Audio->GlobalVoiceVol = ProfileSystem::Instance->ActiveProfile.VoiceVol;
						else
							Control::Audio->SetGlobalVoiceVolume(ProfileSystem::Instance->ActiveProfile.VoiceVol);
					}
					return;
				}
			}
			#ifndef TOUCH_UI
			{
				//Fullscreen
				CObject *Checkbox = OptionsPopupScene->GetObjectByID("fscreenTick");					
				if(FullscreenCheckRect.Intersection(&mosPos))	
				{
					Cursor::SetMode(CA_HandPointy);
					if( clicked )				
					{
						if( Application::ToggleFullscreen() )
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
							if( !GFInstance->GFInfoLive )//User may cancel during popup!!
							{
								if( Checkbox->GetAnimation() == CHECKBOX_OFF )
									Checkbox->PlayAnimation(CHECKBOX_ON);
								else
									Checkbox->PlayAnimation(CHECKBOX_OFF);
							}
						}
						else
						{
							ShowBannerText("Exvideo");

							Control::Audio->PlaySample(aSFXFolly);
						}
					}
					return;		
				}
				else if(WidescreenCheckRect.Intersection(&mosPos))	
				{
					#ifndef _DISABLE_WIDESCREEN
						Checkbox = OptionsPopupScene->GetObjectByID("wideChkTick");
						Cursor::SetMode(CA_HandPointy);
						if( ProfileSystem::Instance->ActiveProfile.IsWideScreen )
							Checkbox->PlayAnimation(CHECKBOX_ON);
						else
							Checkbox->PlayAnimation(CHECKBOX_OFF);

						if( clicked )				
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
							ProfileSystem::Instance->ActiveProfile.IsWideScreen = !ProfileSystem::Instance->ActiveProfile.IsWideScreen;						

							if( GFInstance->GFInfoControl != NULL )
							{
								GFInstance->IsWideScreenChangeInfoPopup = true;
								GFInstance->ActivateGFInfo("Ok","Cancel");					 
							}						
						}
						return;		
					#endif
				}
				else   
				{
					{
						#ifndef TOUCH_UI//Custom Cursor
						CObject *Checkbox = OptionsPopupScene->GetObjectByID("custCursorTick");					
						if(CustomCursorCheckRect.Intersection(&mosPos))	
						{
							Cursor::SetMode(CA_HandPointy);
							if( clicked )				
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
								if( Checkbox->GetAnimation() == CHECKBOX_OFF )
								{
									Checkbox->PlayAnimation(CHECKBOX_ON);
									Cursor::HideSystemCursor();//hide windows cursor - show custom sprite cursor										
								}
								else
								{
									Checkbox->PlayAnimation(CHECKBOX_OFF);																		
									Cursor::UseSystemCursor();//show windows cursor - hide custom sprite cursor
								}									
							}
							return;
						}
						#endif 						

					}	
				}	
			}	
			#endif			
		}
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 2.0f * Control::LogicRate;

		musicVolTxt->Show(false);
		effectsVolTxt->Show(false);
		voiceVolTxt->Show(false);
		diffModeTxt->Show(false);
		customCurTxt->Show(false);
		fullscreenTxt->Show(false);

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter = 1;
			PopupState = Popup_InGameMenu;

			Control::Audio->UnloadSample(aVOMenuTest);
			
			OptionsPopupScene->Show(false);
			InGameMenuPopupScene->Show(true);
			OptionsPopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
			//OptionsPopupScene->GetObjectByID("BtnCancel")->PlayAnimation(0);
		}
		else
		{			
			OptionsPopupScene->SetSceneAlpha(PopupAlpha);
		}
		InGameMenuPopupScene->SetSceneAlpha(1.0f-PopupAlpha/2.0f);
	}
}

void HudControl::UpdateSelectModePopup()
{
	if( PopupCounter == 0 )
	{
		PopupAlpha += 2.0f * Control::LogicRate;

		if( PopupAlpha >= 1.0f )
		{
			PopupCounter = 1;
			PopupAlpha = 1.0f;			
		}
		ModeMaskSpr->SetAlpha(PopupAlpha);
		SelectModePopupScene->SetSceneAlpha(PopupAlpha);
		ModeTexts_SetAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )
	{
		CPoint mosPos = Control::Input->Mpos();	
		bool clicked = Control::Input->LBclicked();
		CObject *Button = SelectModePopupScene->GetObjectByID("BtnOk");	
		if( CheckPopupButton(Button) == Popup_Click )
		{
			//Select Mode Confirm

			//for profile popup it will be setup later, from launch the index will be always 0 !! TODO: Naani kore????
			ProfileSystem::Instance->SetProfileMode( SelectedMode );	// setname & make it valid

			ProfileSystem::Instance->ActiveProfile.IsHintDisabled = false;
			ProfileSystem::Instance->ActiveProfile.IsSkipDisabled = false;
			ProfileSystem::Instance->ActiveProfile.IsExpertMode = false;
			if( SelectedMode == EMODE_EXPERT )
			{
				ProfileSystem::Instance->ActiveProfile.IsExpertMode = true;
				AwardManager::GetInstance()->StartExpertMode();
				ProfileSystem::Instance->ActiveProfile.IsHideSparkles = true;
				ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles = true;
				ProfileSystem::Instance->ActiveProfile.IsHidingTips = false;
			}
			else if( SelectedMode == EMODE_CASUAL )
			{
				AwardManager::GetInstance()->ExitExpertMode();
				ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles = false;
				ProfileSystem::Instance->ActiveProfile.IsHideSparkles = false;
				ProfileSystem::Instance->ActiveProfile.IsHidingTips = false;
			}
			else if( SelectedMode == EMODE_ADVANCED )
			{
				AwardManager::GetInstance()->ExitExpertMode();
				ProfileSystem::Instance->ActiveProfile.IsHideSparkles = false;
				ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles = false;
				ProfileSystem::Instance->ActiveProfile.IsHidingTips = false;
			}
			else if( SelectedMode == EMODE_CUSTOM )
			{
				AwardManager::GetInstance()->ExitExpertMode();
				ProfileSystem::Instance->ActiveProfile.IsHintDisabled = !IsHintEnabled;
				ProfileSystem::Instance->ActiveProfile.CustomHintTimer = 1.0f/HintTimesArray[HintTimeInt];

				ProfileSystem::Instance->ActiveProfile.IsSkipDisabled = !IsSkipEnabled;
				ProfileSystem::Instance->ActiveProfile.CustomSkipTimer = 1.0f/HintTimesArray[SkipTimeInt];

				ProfileSystem::Instance->ActiveProfile.IsTutorialDisabled = !IsTutorEnabled;

				ProfileSystem::Instance->ActiveProfile.IsHideSparkles = !IsSparklesEnabled;
				ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles = !IsHOSparklesEnabled;

				ProfileSystem::Instance->ActiveProfile.IsHidingTips = !IsTextTipsEnabled;

				ProfileSystem::Instance->ActiveProfile.IsAwardPopupsEnabled = IsAwardNotifyEnabled;
			}
            
			ProfileSystem::Instance->SaveProfile(iActiveProfileSlot);//Save current profile!!
			ProfileSystem::Instance->WriteProfileSystemData();
			
			PopupCounter++;
		}
		else
		{				
			//Select Mode
			for (int i = 0; i < 4; i++)
			{
				if( ModeRects[i]->Intersection(&mosPos) ) //sree
				{
					if( SelectedMode == i || clicked )
						ModeButtons[i]->PlayAnimation(2);//show clicked hightlight!!
					else
					{
						ModeButtons[i]->PlayAnimation(1);//show mouseover highlight
						Control::Audio->PlaySample(aSFXRefill2);
					}
					ModeButtons[i]->Show(true);
					Cursor::SetMode(CA_HandPointy);

					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuModeClick);
						switch( i ) 
						{
							case EMODE_CASUAL:	ModeTexts_Init(ModeText_Casual);	break;
							case EMODE_ADVANCED:ModeTexts_Init(ModeText_Advanced);	break;
							case EMODE_EXPERT:	ModeTexts_Init(ModeText_Expert);	break;
							case EMODE_CUSTOM:	ModeTexts_Init(ModeText_Custom);	break;
						}

						SelectedMode = i;

						if( SelectedMode == EMODE_CUSTOM )
						{
							//Show Custom Mode Stuffs
							CustMode_ItemsBg->Show(true);
							SelectModePopupScene->GetObjectByID("custommodetext")->Show(true);
							CustMode_HintKnob->Show(true);
							CustMode_SkipKnob->Show(true);
							CustMode_HintTime->Show(true);
							CustMode_SkipTime->Show(true);

							CustMode_TickTutor->Show(true);
							CustMode_TickSparkles->Show(true);
							CustMode_TickHOSparkles->Show(true);
							CustMode_TickTextTips->Show(true);
							CustMode_TickAwardPopup->Show(true);
							CustMode_TickHints->Show(true);
							CustMode_TickSkip->Show(true);
						}
						else
						{
							//Hide Custom Mode Stuffs
							CustMode_ItemsBg->Show(false);
							SelectModePopupScene->GetObjectByID("custommodetext")->Show(false);
							CustMode_HintKnob->Show(false);
							CustMode_SkipKnob->Show(false);
							CustMode_HintTime->Show(false);
							CustMode_SkipTime->Show(false);

							CustMode_TickTutor->Show(false);
							CustMode_TickSparkles->Show(false);
							CustMode_TickHOSparkles->Show(false);
							CustMode_TickTextTips->Show(false);
							CustMode_TickAwardPopup->Show(false);
							CustMode_TickHints->Show(false);
							CustMode_TickSkip->Show(false);
						}
					}
				}
				else if( SelectedMode != i )//Never turn off prev set modes!! 				
					ModeButtons[i]->PlayAnimation(0);
			}

			if( SelectedMode == EMODE_CUSTOM )
			{
				//Custom Mode Stuffs here
				
				//Hint knob
				if (IsHintEnabled)
				{
					if( !IsHintKnobDragStarted)
					{
						if( CustMode_HintKnob->Collide(&mosPos) )
						{
							CustMode_HintKnob->PlayAnimation(1);
							if( Cursor::GetMode() != CA_HandPointy )
							{
								if( PrevCursorMode != CA_HandPointy )
									Control::Audio->PlaySample(aSFXRefill2);
								Cursor::SetMode(CA_HandPointy);
							}
							if( Control::Input->LBjustdown() )
							{
								IsHintKnobDragStarted = true;
								Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderClick);
							}
						}
						else
							CustMode_HintKnob->PlayAnimation(0);
					}
					else
					{
						if(Control::Input->LBdown())
						{
							if(CustHintSliderRect.Intersection(&mosPos))
							{
								if( Cursor::GetMode() != CA_HandPointy )
								{
									if( PrevCursorMode != CA_HandPointy )
										Control::Audio->PlaySample(aSFXRefill2);
									Cursor::SetMode(CA_HandPointy);
								}

								Control::Audio->PlaySample(aSFXCrystal);
								if( mosPos.x < CUSTOMKNOB_STARTX )
									mosPos.x = CUSTOMKNOB_STARTX;
								else if( mosPos.x > CUSTOMKNOB_ENDX )
									mosPos.x = CUSTOMKNOB_ENDX;
								mosPos.y = HINTKNOB_POSY;							
								CustMode_HintKnob->SetPos(&mosPos);
								HintTimeInt = (int)floor((mosPos.x-CUSTOMKNOB_STARTX)/(CUSTOMKNOB_ENDX-CUSTOMKNOB_STARTX)*9.5f);//10 values
								CustMode_HintTime->PlayAnimation(HintTimeInt);
								ProfileSystem::Instance->ActiveProfile.CustomHintTimer = 1.0f/HintTimesArray[HintTimeInt];
							}						
						}
						else
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderMove);
							IsHintKnobDragStarted = false;
							CustMode_HintKnob->PlayAnimation(0);
						}
						return;
					} 
				}

				//Skip knob
				if (IsSkipEnabled)
				{
					if( !IsSkipKnobDragStarted )
					{
						if( CustMode_SkipKnob->Collide(&mosPos) )
						{
							CustMode_SkipKnob->PlayAnimation(1);
							if( Cursor::GetMode() != CA_HandPointy )
							{
								if( PrevCursorMode != CA_HandPointy )
									Control::Audio->PlaySample(aSFXRefill2);
								Cursor::SetMode(CA_HandPointy);
							}
							if( Control::Input->LBjustdown() )
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderClick);
								IsSkipKnobDragStarted = true;
							}
						}
						else
							CustMode_SkipKnob->PlayAnimation(0);
					}
					else
					{
						if(Control::Input->LBdown())
						{
							if(CustSkipSliderRect.Intersection(&mosPos))
							{
								if( Cursor::GetMode() != CA_HandPointy )
								{
									if( PrevCursorMode != CA_HandPointy )
										Control::Audio->PlaySample(aSFXRefill2);
									Cursor::SetMode(CA_HandPointy);
								}

								Control::Audio->PlaySample(aSFXCrystal);
								if( mosPos.x < CUSTOMKNOB_STARTX )
									mosPos.x = CUSTOMKNOB_STARTX;
								else if( mosPos.x > CUSTOMKNOB_ENDX )
									mosPos.x = CUSTOMKNOB_ENDX;
								mosPos.y = SKIPKNOB_POSY;							
								CustMode_SkipKnob->SetPos(&mosPos);
								SkipTimeInt = (int)floor((mosPos.x-CUSTOMKNOB_STARTX)/(CUSTOMKNOB_ENDX-CUSTOMKNOB_STARTX)*9.5f);//10 values
								CustMode_SkipTime->PlayAnimation(SkipTimeInt);
								ProfileSystem::Instance->ActiveProfile.CustomSkipTimer = 1.0f/HintTimesArray[SkipTimeInt];
							}						
						}
						else
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderMove);
							IsSkipKnobDragStarted = false;
							CustMode_SkipKnob->PlayAnimation(0);
						}
						return;
					} 
				}

				//Tick Checks
				if(CustRectTutor.Intersection(&mosPos))	
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
						IsTutorEnabled = !IsTutorEnabled;
						CustMode_TickTutor->PlayAnimation(IsTutorEnabled?1:0);
					}
				}
				else if(CustRectSparkles.Intersection(&mosPos))	
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
						IsSparklesEnabled = !IsSparklesEnabled;
						CustMode_TickSparkles->PlayAnimation(IsSparklesEnabled?1:0);
					}
				}
				else if(CustRectHOSparkles.Intersection(&mosPos))	
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
						IsHOSparklesEnabled = !IsHOSparklesEnabled;
						CustMode_TickHOSparkles->PlayAnimation(IsHOSparklesEnabled?1:0);
					}
				}
				else if(CustRectTextTips.Intersection(&mosPos))	
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
						IsTextTipsEnabled = !IsTextTipsEnabled;
						CustMode_TickTextTips->PlayAnimation(IsTextTipsEnabled?1:0);
					}
				}
				else if(CustRectAwardNotify.Intersection(&mosPos))	
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
						IsAwardNotifyEnabled = !IsAwardNotifyEnabled;
						CustMode_TickAwardPopup->PlayAnimation(!IsAwardNotifyEnabled?1:0);
					}
				}
				else if(CustRectHintTick.Intersection(&mosPos))	
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
						IsHintEnabled = !IsHintEnabled;
						CustMode_TickHints->PlayAnimation(IsHintEnabled?1:0);
						CustMode_HintKnob->PlayAnimation(IsHintEnabled?0:2);
						CustMode_HintTime->SetAlpha(IsHintEnabled?1.0f:0.65f);
					}
				}
				else if(CustRectSkipTick.Intersection(&mosPos))	
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
						IsSkipEnabled = !IsSkipEnabled;
						CustMode_TickSkip->PlayAnimation(IsSkipEnabled?1:0);
						CustMode_SkipKnob->PlayAnimation(IsSkipEnabled?0:2);
						CustMode_SkipTime->SetAlpha(IsSkipEnabled?1.0f:0.65f);
					}
				}
			}
		}
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 2.0f * Control::LogicRate;
		modeNameText->Show(false);

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			PopupAlpha = 1.0f;
			PopupState = Popup_Options;
			PopupCounter = 1;
			ModeMaskSpr->Show(false);

			int profileMode = ProfileSystem::Instance->ProfileMode[iActiveProfileSlot];
			OptionsPopupScene->GetObjectByID("DiffModeText")->PlayAnimation(profileMode);
            
			SelectModePopupScene->Show(false);
			ModeTexts_Show(false);
			SelectModePopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
		}
		else
		{
            ModeMaskSpr->SetAlpha(PopupAlpha);
			ModeTexts_SetAlpha(PopupAlpha);
			SelectModePopupScene->SetSceneAlpha(PopupAlpha);
		}
	}
}


int HudControl::CheckPopupButton(CObject *Button)
{
	CPoint mosPos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	if(Button->Collide(&mosPos)) 
	{
		if(clicked )
		{
			Button->PlayAnimation(BA_Click);
			Control::Audio->PlaySample(aSFXClick);
			return Popup_Click;
		}
		else
		{
			Cursor::SetMode(CA_HandPointy);
			if( Button->GetAnimation() != BA_Hover )
			{
				Control::Audio->PlaySample(aSFXRefill2);			
				Button->PlayAnimation(BA_Hover);
			}
			return Popup_MouseOver;
		}
	}
	else
	{		
		Button->PlayAnimation( BA_Idle );
	}

	return Popup_NoInteraction;
}


void HudControl::ShowSelectModePopup()
{
	ModeMaskSpr->SetAlpha(0.0f);
	ModeMaskSpr->Show(true);
    
	SelectModePopupScene->Show(true);
	modeNameText->Show(true);
	SelectModePopupScene->SetSceneAlpha(0.0f);

	//Mode chosen from Options. Set based on Active Profile
	int profileMode = ProfileSystem::Instance->ProfileMode[iActiveProfileSlot];
	for (int i = 0; i < 4; i++)
	{
		if(i==profileMode)
		{
			ModeButtons[i]->PlayAnimation(2); //highlight active one
			SelectedMode = i;
			switch (i)
			{
			case 0:
				ModeTexts_Init(ModeText_Casual); break;
			case 1:
				ModeTexts_Init(ModeText_Advanced); break;
			case 2:
				ModeTexts_Init(ModeText_Expert); break;
			case 3:
				ModeTexts_Init(ModeText_Custom); break;
			default:
				break;
			}
		}
		else
			ModeButtons[i]->PlayAnimation(0);
	}
						  
	ModeTexts_Show(true);
	ModeTexts_SetAlpha(0.0f);

	if(profileMode==3)//custom mode
	{
		IsHintEnabled = !ProfileSystem::Instance->ActiveProfile.IsHintDisabled;

		//reset Times
		CustMode_HintTime->PlayAnimation(0);
		CustMode_SkipTime->PlayAnimation(0);

		if(IsHintEnabled)
		{
			CustMode_TickHints->PlayAnimation(1);

			int NumTimes = 10;
			for (int i = 0; i < NumTimes; i++)
			{
				if( Elixir::Equal(ProfileSystem::Instance->ActiveProfile.CustomHintTimer,1.0f/HintTimesArray[i]) )
				{
					HintTimeInt = i;
					break;
				}
			}
			K_LOG("HintTimeInt %f",HintTimesArray[HintTimeInt]);
			CustMode_HintKnob->PlayAnimation(0);
			//Reset knobs
			CPoint temp(CUSTOMKNOB_STARTX+(CUSTOMKNOB_ENDX-CUSTOMKNOB_STARTX)/10.0f*HintTimeInt,HINTKNOB_POSY,0);
			CustMode_HintKnob->SetPos(&temp);

			CustMode_HintTime->PlayAnimation(HintTimeInt);
		}
		else
		{
			CustMode_HintKnob->PlayAnimation(2);
			CustMode_HintTime->SetAlpha(0.75f);
		}

		IsSkipEnabled = !ProfileSystem::Instance->ActiveProfile.IsSkipDisabled;

		if(IsSkipEnabled)
		{
			CustMode_TickSkip->PlayAnimation(1);

			int NumTimes = 10;
			for (int i = 0; i < NumTimes; i++)
			{
				if( Elixir::Equal(ProfileSystem::Instance->ActiveProfile.CustomSkipTimer,1.0f/HintTimesArray[i]) )
				{
					SkipTimeInt = i;
					break;
				}
			}
			K_LOG("SkipTimeInt %f",HintTimesArray[SkipTimeInt]);
			CustMode_SkipKnob->PlayAnimation(0);
			//Reset knobs
			CPoint temp(CUSTOMKNOB_STARTX+(CUSTOMKNOB_ENDX-CUSTOMKNOB_STARTX)/10.0f*SkipTimeInt,SKIPKNOB_POSY,0);
			CustMode_SkipKnob->SetPos(&temp);

			CustMode_SkipTime->PlayAnimation(SkipTimeInt);
		}
		else
		{
			CustMode_SkipKnob->PlayAnimation(2);
			CustMode_SkipTime->SetAlpha(0.75f);
		}
			

		IsTutorEnabled = !ProfileSystem::Instance->ActiveProfile.IsTutorialDisabled;
		CustMode_TickTutor->PlayAnimation(IsTutorEnabled?1:0);

		IsSparklesEnabled = !ProfileSystem::Instance->ActiveProfile.IsHideSparkles;
		CustMode_TickSparkles->PlayAnimation(IsSparklesEnabled?1:0);

		IsHOSparklesEnabled = !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles;
		CustMode_TickHOSparkles->PlayAnimation(IsHOSparklesEnabled?1:0);

		IsTextTipsEnabled = !ProfileSystem::Instance->ActiveProfile.IsHidingTips;
		CustMode_TickTextTips->PlayAnimation(IsTextTipsEnabled?1:0);

		IsAwardNotifyEnabled = ProfileSystem::Instance->ActiveProfile.IsAwardPopupsEnabled;
		CustMode_TickAwardPopup->PlayAnimation(!IsAwardNotifyEnabled?1:0);
	}
	else
	{
		//Hide Custom Mode Stuffs
		CustMode_ItemsBg->Show(false);
		SelectModePopupScene->GetObjectByID("custommodetext")->Show(false);
		CustMode_HintKnob->Show(false);
		CustMode_SkipKnob->Show(false);
		CustMode_HintTime->Show(false);
		CustMode_SkipTime->Show(false);

		CustMode_TickTutor->Show(false);
		CustMode_TickSparkles->Show(false);
		CustMode_TickHOSparkles->Show(false);
		CustMode_TickTextTips->Show(false);
		CustMode_TickAwardPopup->Show(false);
		CustMode_TickHints->Show(false);
		CustMode_TickSkip->Show(false);

		CustMode_HintKnob->PlayAnimation(0);
		CustMode_SkipKnob->PlayAnimation(0);

		HintTimeInt=0;
		SkipTimeInt=0;
		IsHintEnabled=true;
		IsSkipEnabled=true;
		IsTutorEnabled=true;
		IsSparklesEnabled=true;
		IsHOSparklesEnabled=true;
		IsTextTipsEnabled=true;
		IsAwardNotifyEnabled=true;

		//Reset knobs
		CPoint temp(CUSTOMKNOB_STARTX,HINTKNOB_POSY,0);
		CustMode_HintKnob->SetPos(&temp);
		temp.y = SKIPKNOB_POSY;
		CustMode_SkipKnob->SetPos(&temp);

		HintTimeInt = 0;
		SkipTimeInt = 0;

		CustMode_HintTime->PlayAnimation(0);
		CustMode_SkipTime->PlayAnimation(0);

		CustMode_HintTime->SetAlpha(1.0f);
		CustMode_SkipTime->SetAlpha(1.0f);

		CustMode_TickTutor->PlayAnimation(1);
		CustMode_TickSparkles->PlayAnimation(1);
		CustMode_TickHOSparkles->PlayAnimation(1);
		CustMode_TickTextTips->PlayAnimation(1);
		CustMode_TickAwardPopup->PlayAnimation(0);
		CustMode_TickHints->PlayAnimation(1);
		CustMode_TickSkip->PlayAnimation(1);
	}

	PopupAlpha = 0.0f;
	PopupState = Popup_ModeSelect;
	PopupCounter = 0;
}


void HudControl::ShowOptionsPopup()
{
	#ifndef TOUCH_UI
		OptionsPopupScene->GetObjectByID("fscreenTick")->PlayAnimation(ProfileSystem::Instance->ActiveProfile.Windowed);
		OptionsPopupScene->GetObjectByID("custCursorTick")->PlayAnimation(Cursor::IsSystemCursor());
		#ifndef _DISABLE_WIDESCREEN
			OptionsPopupScene->GetObjectByID("wideChkTick")->PlayAnimation(!ProfileSystem::Instance->ActiveProfile.IsWideScreen);//default setfixedaspectratio widescreen = false CHECKBOX_OFF
		#endif
	#endif	

	
	//Set to init knob init values
	CPoint NewMusicKnobPos, NewSoundKnobPos;
	CPoint NewVoiceKnobPos;
	NewMusicKnobPos.y = MUSICKNOB_POSY;
	NewSoundKnobPos.y = SOUNDKNOB_POSY;
	NewVoiceKnobPos.y = VOICEKNOB_POSY;

	IsMusicKnobDragStarted = IsSoundKnobDragStarted = false;
	IsEnvirKnobDragStarted= IsVoiceKnobDragStarted = false;

	int profileMode = ProfileSystem::Instance->ProfileMode[iActiveProfileSlot];
	OptionsPopupScene->GetObjectByID("DiffModeText")->PlayAnimation(profileMode);
	
	NewMusicKnobPos.x = KNOB_STARTX + ((KNOB_ENDX-KNOB_STARTX)*ProfileSystem::Instance->ActiveProfile.MusicVol/100.0f);	
	OptionsPopupScene->GetObjectByID("musicSlider")->SetPos(&NewMusicKnobPos);

	NewSoundKnobPos.x = KNOB_STARTX + ((KNOB_ENDX-KNOB_STARTX)*ProfileSystem::Instance->ActiveProfile.SFXVol/100.0f);	
	OptionsPopupScene->GetObjectByID("sfxSlider")->SetPos(&NewSoundKnobPos);

	NewVoiceKnobPos.x = KNOB_STARTX + ((KNOB_ENDX-KNOB_STARTX)*ProfileSystem::Instance->ActiveProfile.VoiceVol/100.0f);	
	OptionsPopupScene->GetObjectByID("voiceSlider")->SetPos(&NewVoiceKnobPos);

	OptionsPopupScene->Show(true);	
	OptionsPopupScene->SetSceneAlpha(0.0f);
	
	InGameMenuPopupScene->Show(false);

	musicVolTxt->Show(true);
	effectsVolTxt->Show(true);
	voiceVolTxt->Show(true);
	diffModeTxt->Show(true);
	#ifndef TOUCH_UI
	customCurTxt->Show(true);
	fullscreenTxt->Show(true);
	#endif

#ifdef K_ANDROID
	diffModeTxt->Show(false);
	OptionsPopupScene->GetObjectByID("DiffModeText")->Show(false);
	modeBtnOver->Show(false);
#endif // K_ANDROID

/*
	These buttons no more exist in the OP.SCN, so no need to hide something that is not there
 	#ifdef K_ANDROID
		//	Dont show Full screen and Wide screen checks in this version
		OptionsPopupScene->GetObjectByID("custCursorTick")->Show(false);//fullscreen 
		OptionsPopupScene->GetObjectByID("fscreenTick")->Show(false);//fullscreen 
		OptionsPopupScene->GetObjectByID("wideChkTick")->Show(false);//widescreen 
	#endif
*/
	PopupAlpha = 0.0f;
	PopupState = Popup_Options;
	PopupCounter = 0;
}

void HudControl::ShowInGameMenuPopup()
{
	MaskSpr->SetAlpha(0.0f);
	MaskSpr->Show(true);

	InGameMenuPopupScene->SetSceneAlpha(0.0f);
	InGameMenuPopupScene->Show(true);
	PopupAlpha = 0.0f;
	PopupState = Popup_InGameMenu;
	PopupCounter = 0;
	GFInstance->IsActivePopup = true;
	GFInstance->IsInGameMenuPopup = true;
}


void HudControl::UpdateInGameMenuPopup()
{
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				PopupCounter = 2;
			}
	#endif
	if( PopupCounter == 0 )
	{
		PopupAlpha += 4.0f * Control::LogicRate;

		if( PopupAlpha >= 1.0f )
		{
			PopupCounter = 1;
			PopupAlpha = 1.0f;			
		}
		MaskSpr->SetAlpha(PopupAlpha);
		InGameMenuPopupScene->SetSceneAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )
	{
		
		CPoint pos = Control::Input->Mpos();
		Cursor::SetMode(CA_Normal);

		//Disable Memories & Restart Level if from extras mini game
		if(ProfileSystem::Instance->_isExtrasGamePlay)
		{
			Buttons[IGMB_Restart]->Show(false);
			Buttons[IGMB_Restart]->SetEnable(false);

			Buttons[IGMB_Memories]->Show(false);
			Buttons[IGMB_Memories]->SetEnable(false);

			ButtonRects[IGMB_MainMenu] = MemoriesRect;
			Buttons[IGMB_MainMenu]->SetXY(Buttons[IGMB_Memories]->Pos.x, Buttons[IGMB_Memories]->Pos.y);
		}
		else
		{
			Buttons[IGMB_Restart]->Show(true);
			Buttons[IGMB_Restart]->SetEnable(true);

			Buttons[IGMB_Memories]->Show(true);
			Buttons[IGMB_Memories]->SetEnable(true);

			ButtonRects[IGMB_MainMenu] = MainMenuRect;
			Buttons[IGMB_MainMenu]->SetXY(MainMenuPos.x, MainMenuPos.y);
		}

		for(int i=0; i < IGMB_Max; ++i)
		{

			if(ProfileSystem::Instance->_isExtrasGamePlay)
			{
				if(i == IGMB_Restart || i == IGMB_Memories)
					continue;
			}

			if( ButtonRects[i].Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandPointy);
				if(Buttons[i]->GetAnimation()==0)
				{					
					Buttons[i]->PlayAnimation(1);
				}

				if( Control::Input->LBclicked() )
				{
					Control::Audio->PlaySample(aSFXClick);
					Control::Input->ClearMouse();
					switch( i )
					{
						case IGMB_Resume:PopupCounter++;
										return;
						case IGMB_Restart:
										if(ProfileSystem::Instance->_isExtrasGamePlay)
										{
											GFInstance->IsActivePopup = false;
											GFInstance->IsInGameMenuPopup = false;
											SendMessage(ActionProcessed, ID, HOSM_ReturnToMenu);
											return;	
										}
										else
											ShowRestartPopup();
										return;
						case IGMB_Options:
										ShowOptionsPopup();										  
										return;
						case IGMB_Memories:										
										ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromMainMenu = false;
										if(!(ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_KidsRoom && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_AwardsRoom))
											ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
										ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_KidsRoom;
										SendMessage(ActionProcessed, ID, MAP_NEW_AREA);//Handle map transition
										return;
						case IGMB_MainMenu:
										//Musics will be turned off in HiddenObjectSystem::ProcessMessage
										GFInstance->IsActivePopup = false;
										GFInstance->IsInGameMenuPopup = false;
										SendMessage(ActionProcessed, ID, HOSM_ReturnToMenu);
										PopupCounter++;
										return;
					}					
				}
			}
			else
			{				
				Buttons[i]->PlayAnimation(0);
			}
		}		
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 2.0f * Control::LogicRate;

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			PopupState = Popup_Inactive;			
			MaskSpr->Show(false);
			InGameMenuPopupScene->Show(false);
			InGameMenuPopupScene->GetObjectByID("resume")->PlayAnimation(0);
			InGameMenuPopupScene->GetObjectByID("options")->PlayAnimation(0);
			InGameMenuPopupScene->GetObjectByID("mainmenu")->PlayAnimation(0);			

			GFInstance->IsActivePopup = false;
			GFInstance->IsInGameMenuPopup = false;
		}
		else
		{
			MaskSpr->SetAlpha(PopupAlpha);
			InGameMenuPopupScene->SetSceneAlpha(PopupAlpha);
		}
	}

	//if(RestartNo
}

void HudControl::ShowRestartPopup()
{
	RestartPopup->Show(true);	
	RestartPopup->SetSceneAlpha(0.0f);

	PopupAlpha = 0.0f;
	PopupState = Popup_Restart;
	PopupCounter = 0;
}

void HudControl::UpdateRestartPopup()
{
	if( PopupCounter == 0 )
	{
		PopupAlpha += 2.0f * Control::LogicRate;

		if( PopupAlpha >= 1.0f )
		{
			PopupCounter = 1;
			PopupAlpha = 1.0f;				
			InGameMenuPopupScene->SetSceneAlpha(0);
		}
		else
			InGameMenuPopupScene->SetSceneAlpha(1.0f-PopupAlpha/2.0f);
		RestartPopup->SetSceneAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )
	{
		CObject *RestartYes = RestartPopup->GetObjectByID("BtnYes");
		CObject *RestartNo = RestartPopup->GetObjectByID("BtnNo");

		if(CheckPopupButton(RestartNo) == Popup_Click )
		{
			PopupCounter++;
			return;
		}
		if(CheckPopupButton(RestartYes) == Popup_Click)
		{
			InvBoxCtrl->IsLocked = false;
			InvBoxCtrl->IsDragging = false;
			InvBoxCtrl->ExternalClose();

			for( int j= 0 ; j < MAX_INVBOX_OBJECTS; j++)
			{
				ProfileSystem::Instance->ActiveProfile.InvObjects[j].InvId = -1;
				ProfileSystem::Instance->ActiveProfile.InvObjects[j].nPiecePicked = 0;
				ProfileSystem::Instance->ActiveProfile.InvObjects[j].MaxPieces = 0;
			}
			ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;
			ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;	
			
			ProfileSystem::Instance->LoadProfileBackup(ProfileSystem::Instance->ProfileSlot);
			SendMessage(ActionProcessed, ID, MAP_NEW_AREA);
		}
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 2.0f * Control::LogicRate;

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter = 1;
			PopupState = Popup_InGameMenu;

			Control::Audio->UnloadSample(aVOMenuTest);
			
			RestartPopup->Show(false);
			RestartPopup->GetObjectByID("BtnYes")->PlayAnimation(0);
			RestartPopup->GetObjectByID("BtnNo")->PlayAnimation(0);
		}
		else
		{			
			RestartPopup->SetSceneAlpha(PopupAlpha);
		}
		InGameMenuPopupScene->SetSceneAlpha(1.0f-PopupAlpha/2.0f);
	}
}

void HudControl::BackToMainMenu()
{
	GFInstance->IsActivePopup = false;
	GFInstance->IsInGameMenuPopup = false;
	SendMessage(ActionProcessed, ID, HOSM_ReturnToMenu);
}


void HudControl::UseCurrentHint()
{
	Control::Audio->PlayHintFX();

	if(IsPuzzleScene)
		Elements[HE_Skip]->PlayAnimation(2);
	else
		Elements[HE_Hint]->PlayAnimation(2);	
}

void HudControl::SendNewAreaMessage()
{
	SendMessage(ActionProcessed, ID, MAP_NEW_AREA);//Handle map transition
}

void HudControl::InitMessageBanner()
{
	BannerTimer = 0.0f;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;
	MessageText = new CBitmapText();
	MessageText->SetZ(eZOrder_Banner+1);
	MessageText->SetAlignment(Align_Center);
	MessageText->SetPos(PointSystem::CreateCPoint(float(MAX_DISPLAY_WIDTH/2.0f), 30.0f, 0.0f));
	if(StringTable::IsLanguageMultiByte())
		MessageText->LoadFont(eFont_Small);
	else
		MessageText->LoadFont(eFont_26);
//	MessageText->SetText("Message Text");	
	MessageText->SetText(StringTable::getStr("ExMessage_Text"));	
	MessageText->SetScale(1.0f);
	MessageText->SetColor(1.0f,1.0f,1.0f);//195/255.0f, 238/255.0f, 250/255.0f);	
}

void HudControl::ModeTexts_Init(std::string bigstring, int OffsetY, eTextAlignment AlignStyle, int OffsetX)
{
	if(!bigstring.empty()) bigstring = StringTable::getStr(bigstring);
	std::vector<std::string> Strings;
	std::string CurString = "";
	std::string CurWord = "";
	int CurWidth = 0;
	int WordLength = 0;
//#ifdef JAPANESE_BUILD 
	unsigned int Char;
//#else
//	unsigned char Char;
//#endif
	//Hide all current strings
	CPoint textPos;
	for(int i=0; i < MODE2_TEXT_LINES; ++i)
	{
		ModeDescriptionTexts[i]->SetText(" ");

		textPos = ModeBoxPos;
		
		textPos.x += OffsetX + ModeTextsOff.x;

		if( AlignStyle == Align_Left )
			textPos.x -= 260;

		textPos.y += ModeTextsOff.y + OffsetY + ModeTextsYSpacing * i;
		
		ModeDescriptionTexts[i]->SetPos(&textPos);		
		ModeDescriptionTexts[i]->SetAlignment(AlignStyle);
	}

	//Split String
	int length = (int)bigstring.length();
	int CharWidth = 0;
	for(int i=0; i < length; ++i)
	{
		Char = bigstring.at(i);//japanese will have multibyte characters!!
//#ifdef JAPANESE_BUILD
	if(StringTable::IsMyLangaugeLatin())
		CharWidth = 0;//We split words based on $ sign only!! :) 
//#else
		CharWidth = ModeDescriptionTexts[0]->charWidth(Char);
//#endif
		if( Char == '\\'  || Char == '$')//for japanese $
		{
			CurWidth = 0;
			Strings.push_back(CurString);

			CurString = "";
		}
		//If current word cannot fit on the line
		//add line, clear line, and add char to current word
		else if( (CharWidth + CurWidth + WordLength) > ModeTextsLineWidth )
		{
			CurWidth = 0;
			Strings.push_back(CurString);

			CurString = "";

			CurWord += Char;
			WordLength += CharWidth;			
		}
		else
		{
			WordLength += CharWidth;
			CurWord += Char;

			//Hit a space or end of string, add a word
			if( Char == 32 || i == (length - 1))
			{
				CurWidth += WordLength;
				WordLength = 0;

				CurString += CurWord;
				CurWord = "";

				//Add last string
				if( i == (length - 1) )
				{
					Strings.push_back(CurString);
				}
			}
		}
	}

	if(Strings.size() >= MODE2_TEXT_LINES)
	{
		ModeDescriptionTexts[0]->SetText("String is too big.");
	}
	else
	{
		int size = (int)Strings.size();
		for(int i=0; i < size; ++i)
		{
			if(size == 1)  // Sac: if we have only 1 or 2 lines, then push the line to center of the display box
				ModeDescriptionTexts[i+MODE2_TEXT_LINES/2]->SetText(Strings[i]);
			else	
				ModeDescriptionTexts[i]->SetText(Strings[i]);			
		}
	}
}


void HudControl::ModeTexts_Show( bool f )
{
	for(int i=0; i < MODE2_TEXT_LINES; ++i)
	{
		ModeDescriptionTexts[i]->Show(f);
	}
}

void HudControl::ModeTexts_SetAlpha( float f )
{
	for(int i=0; i < MODE2_TEXT_LINES; ++i)
	{
		ModeDescriptionTexts[i]->SetAlpha(f);
	}
}
	

//Unlock Popup Class
UnlockPopup::UnlockPopup(SuperFX *addFx)
{
	_addFX = addFx;
	_textBgSpr = NULL;
	_textTitle = NULL;
	_textDescrip = NULL;

	_isShowing = false;
	_alpha = 0;
	_delayTimer = 0;

	//default 1.5seconds
	_textStaticTime = 1.5f;

	_state = 0;
}

void UnlockPopup::SetText(std::string titleText, std::string descriptionText)
{
	if(_textTitle!=NULL)
		_textTitle->SetText(titleText);

	if(_textDescrip!=NULL)
		_textDescrip->SetText(descriptionText);
}

void UnlockPopup::ShowPopup(float delay)
{
	if(_addFX!=NULL)
		_addFX->Start();

	_delayTimer = delay;

	ColorRGB yellow={(233/255.0f),(204/255.0f),(131/255.0f)};
	if(_textTitle==NULL)
	{
		_textTitle = new CBitmapText();
		_textTitle->SetTextProps(StringTable::getStr("Exnew"), 293+30, 564, 5030, yellow, eFont_Italic24, Align_Center);
	}

	if(_textDescrip==NULL)
	{
		_textDescrip = new CBitmapText();
		_textDescrip->SetTextProps(StringTable::getStr("Exhelp"), 286+30, 594, 5030, yellow, eFont_25, Align_Center);
	}

	if(_textBgSpr==NULL)
	{
		_textBgSpr = new CObject();
		_textBgSpr->LoadInit("UI\\HUD\\OBJBG.SPR", 340.0f, 570.0f, 5029);
	}

	_alpha = 0;
	UpdateAlpha();
	_state = 1;
	_isShowing = true;
}

void UnlockPopup::UpdateAlpha()
{
	if(_textTitle!=NULL)
		_textTitle->SetAlpha(_alpha);

	if(_textDescrip!=NULL)
		_textDescrip->SetAlpha(_alpha);

	if(_textBgSpr!=NULL)
		_textBgSpr->SetAlpha(_alpha);
}

void UnlockPopup::Update(float ds)
{
	if( _isShowing )
	{
		switch (_state)
		{
		case 1:
			//FADE IN
			if(_delayTimer>0)
			{
				_delayTimer -= ds;
				if(_delayTimer<0)
					_delayTimer = 0;
				return;
			}

			_alpha += ds;

			if(_alpha>1.0f)
			{
				_alpha = 1.0f;
				UpdateAlpha();

				_alpha = 0.0f;
				_state++;
			}
			else
			{
				UpdateAlpha();
			}
			break;
		case 2:
			//STATIC
			_alpha += Control::LogicRate;

			if(_alpha>_textStaticTime)
			{
				_alpha = 1.0f;
				_state++;
			}
			break;
		case 3:
			//FADE OUT
			_alpha -= Control::LogicRate;

			if(_alpha<0.0f)
			{
				_alpha = 0.0f;
				UpdateAlpha();
				_state=0;

				_isShowing = false;
			}
			else
			{
				UpdateAlpha();
			}
			break;
		}
	}
}

UnlockPopup::~UnlockPopup()
{
	SAFE_DELETE(_textBgSpr);
	SAFE_DELETE(_textTitle);
	SAFE_DELETE(_textDescrip);

	_addFX = NULL;
}

#ifdef K_ANDROID
void HudControl::showADorBuyDialog(bool f, int type)
{
    adViewDialog->Show(f);
	viewADButton->Show(f);
	buyHintsButton->Show(f);
	adBtnText->Show(f);
	buyBtnText->Show(f);
	adViewDialogText->Show(f);
	adORBuyHudActive = f;
	adType = type;
}
#endif // K_ANDROID