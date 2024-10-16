//====================================
// Framework
// Main Menu Control
//====================================

#include "MainMenuControl.h"
#include "ProfileSystem.h"
#include "Object.h"
#include "Application.h"
#include "TextBox.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "GameFlow.h"
#include "Resources.h"
#include "InfoPanel.h"
#include "Util.h"
#include "KList.h"
#include "Fonts.h"
#include "Cursor.h"
#include "LangDefs.h"
#include "AwardManager.h"
#include "SteamManager.h"
#include "GameTextManager.h"
#include "AssetsStringTable.h"
#include <regex>
#include <android/log.h>
#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/input.h>
#define LOG_TAG "NativeInputSystem"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#ifdef _DEBUG
	#include "SuperLog.h"
#endif // _DEBUG
#define _CHEATS
#define MOMLULLABY_MUSIC_ENDTIME 28.06f
#define MOMLULLABY_MUSIC_KIDREAPPEARS 66.10f
#define MISSINGLILHONEY_MUSIC_ENDTIME 71.458f


#ifdef K_WIN32
	extern std::wstring AppTitle_Str;
	extern std::wstring GWRNG_4_WStr;
#else
	extern std::string AppTitle_Str;
	extern std::string GWRNG_4_WStr;
#endif

extern bool isTextBoxEnterKeyPressed;

const CRectangle  ComicBookUnlockedRect(544.0f,595.0f,258.0f,158.0f);

const std::string ErrorSameProfileName = "Exname";
const std::string ModeText_Casual = "Excasual";
const std::string ModeText_Advanced = "Exadvanced";
const std::string ModeText_Expert = "Exexpert";
const std::string ModeText_Custom = "";


const CRectangle  AdventureRectMainChapter(400.0f,207.0f,291.0f,391.0f);
const CRectangle  AdventureRectBonusChapter(697.0f,207.0f,300.0f,303.0f);

const std::string MMButtonID[MMB_Max] =
{												
	"btnplay", "btnoptions", "btnexit" , "btnextras", "btncredits"
	#ifdef K_ANDROID
		//, "UNLOCK_BUTTON"
	#endif 
};

enum CheckBoxState{CHECKBOX_ON, CHECKBOX_OFF};

const CPoint CopyrightPos(363.0f, 702.0f, 0.0f);
int CheatLevel;
int NativeCheatLevel = Level_Nil;
const CPoint MaskSprPos(0,0,0);
#define KNOB_STARTX 548.0f
#define KNOB_ENDX 	830.0f

#define MUSICKNOB_POSY	204
#define SOUNDKNOB_POSY	264
#define VOICEKNOB_POSY	326

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

const CRectangle  MusicSliderAreaRect(528.0f,182.0f,316.0f,40.0f);
const CRectangle  SFXSliderAreaRect(528.0f,245.0f,319.0f,39.0f);
const CRectangle  VoiceSliderAreaRect(528.0f,303.0f,319.0f,41.0f);

const CRectangle  WidescreenCheckRect(727.0f,515.0f,50.0f,42.0f);
const CRectangle  FullscreenCheckRect(723.0f,465.0f,56.0f,41.0f);
const CRectangle  CustomCursorCheckRect(722.0f,415.0f,54.0f,43.0f);
const CRectangle  ModeChangeTextRect(702.0f,354.0f,164.0f,42.0f);

#ifdef _DEMOBUILD
const CRectangle  PlayRect(760.0f,510.0f,100.0f,150.0f);
#else
const CRectangle  PlayRect(760.0f, 460.0f, 100.0f, 150.0f);
#endif

#ifdef TOUCH_UI
const CRectangle  ExitRect(764.0f,5770.0f,115.0f,52.0f);
const CRectangle  OptionsRect(1070.0f, 45.0f, 100.0f, 40.0f);//(732.0f,537.0f,164.0f,53.0f);
#else
const CRectangle  ExitRect(764.0f, 5770.0f, 115.0f, 52.0f);//(764.0f,577.0f,115.0f,52.0f);
const CRectangle  OptionsRect(1070.0f,45.0f,100.0f,40.0f);
#endif

const CRectangle  ExtrasRect(260.0f, 630.0f, 100.0f, 40.0f);//(940.0f,640.0f,146.0f,66.0f);
const CRectangle  CreditsRect(945.0f, 2000.0f, 145.0f, 51.0f);//(945.0f,715.0f,145.0f,51.0f);
const CRectangle  WelcomeRect(323.0f,470.0f,190.0f,180.0f);

#ifdef K_ANDROID
	const CRectangle  UnlockNowRect(1030.0f,194.0f,270.0f,80.0f);
#endif
const CRectangle  ChapterSelectRect(385.0f,142.0f,573.0f,483.0f);

const CPoint ModeBoxPos(212, 130.0f, 0.0f);
//static CBitmapText *TFBitmapText = NULL; 

const CPoint ModeTextsOff(460.0f, 266.0f, 0.0f);
const float  ModeTextsYSpacing = 45.0f;

const float HintTimesArray[10] = {5.0f,15.0f,30.0f,45.0f,60.0f,75.0f,90.0f,120.0f,135.0f,150.0f};
  
#ifdef K_iOS
/*
    bool lpCharValidationCallback( long c )
    {
        std::string TmpTxt = GFApp->lpTextField->getText();
        TmpTxt += c;
        TFBitmapText->SetText( TmpTxt ); 
        return (TFBitmapText->StringWidth() < 200 );//CHECK IF TEXTWIDTH EXCEEDED OUR BOX   
    }
	*/
const CRectangle MailRect(800,315,226,276);
const CRectangle UnlockRect(416,559,310,80);//Upsell
const CRectangle RateRect(839,241,185,67);//Awards
#endif

// Global variables

static JNIEnv* g_env = nullptr;
static jobject g_thiz = nullptr;
static jmethodID g_methodID = nullptr;

// JNI function
//extern "C"
//JNIEXPORT void JNICALL
//Java_com_example_test_1android_1kanji_MainActivity_nativeKeyPress(JNIEnv *env, jobject thiz,
//                                                                  jint key_code) {
//    // Store the JNIEnv and jobject for later use
//    g_env = env;
//    g_thiz = thiz;
//
//    // Get the method ID for nativeKeyPress if we haven't already
//    if (g_methodID == nullptr) {
//        jclass cls = env->GetObjectClass(thiz);
//        g_methodID = env->GetMethodID(cls, "nativeKeyPress", "(I)V");
//        env->DeleteLocalRef(cls);
//    }
//}
//class InputWrapper {
//public:
//    bool Kdown(int key_code) {
//        LOGI("Kdown called with key_code: %d", key_code);
//        if (g_env && g_thiz && g_methodID) {
//            g_env->CallVoidMethod(g_thiz, g_methodID, key_code);
//            LOGI("Kdown: Key pressed successfully");
//            return true;  // Assuming the key is always "down" when this is called
//        }
//        LOGI("Kdown: Failed to process key. JNI environment not properly set up.");
//        return false;
//    }
//
//    bool Kpressed(int key_code) {
//        LOGI("Kpressed called with key_code: %d", key_code);
//        // For simplicity, we're treating Kpressed the same as Kdown
//        // You might want to implement a different behavior if needed
//        bool result = Kdown(key_code);
//        LOGI("Kpressed: Returning %s", result ? "true" : "false");
//        return result;
//    }
//};


// Global instance to mimic Control::Input
//InputWrapper Input;

////// Function to handle key presses (to be called from your game logic)
//void handleKeyPress(int key_code) {
//    switch (key_code) {
//        case K_VK_1:
//            CheatLevel = Level_1;
//            LOGI("Set CheatLevel to Level_1");
//            break;
//        case K_VK_2:
//            CheatLevel = Level_2;
//            LOGI("Set CheatLevel to Level_2");
//            break;
//        case K_VK_3:
//            CheatLevel = Level_3;
//            LOGI("Set CheatLevel to Level_3");
//            break;
//        case K_VK_4:
//            CheatLevel = Level_4;
//            LOGI("Set CheatLevel to Level_4");
//            break;
//        case K_VK_5:
//            CheatLevel = Level_5;
//            LOGI("Set CheatLevel to Level_5");
//            break;
//        case K_VK_6:
//            CheatLevel = Level_6;
//            LOGI("Set CheatLevel to Level_6");
//            break;
//        case K_VK_7:
//            CheatLevel = Level_7;
//            LOGI("Set CheatLevel to Level_7");
//            break;
//        case K_VK_8:
//            CheatLevel = Level_8;
//            LOGI("Set CheatLevel to Level_8");
//            break;
//        default:
//            LOGI("Unhandled key code: %d", key_code);
//            break;
//    }
//}
MainMenuControl::MainMenuControl()
{
	GFMainMenuCtrl = this;
	//SLOG::LogLoadingStartTime();
	MessageText = NULL;
	MessageElement = NULL;

	isVidFireLoaded = false;
	//TFBitmapText = NULL;

	//If we come not through splash scene[for fast debug], we need to wait till loading done
	if( GFInstance->IsPakResLoadInited && !GFInstance->IsPakResLoadDone )
	{
		GFInstance->WaitTillPakResLoadDone();
		GFInstance->IsPakResLoadDone = true;
	}

	MenuScene = new CHOScene();
	MenuScene->Load("MNU\\MNU.SCN");

	scn_Bg = MenuScene->GetObjectByID("base");
	scn_MomGlow = MenuScene->GetObjectByID("glowmother");
	scn_MomRays = MenuScene->GetObjectByID("raysmother");

	scn_pillarLeft1 = MenuScene->GetObjectByID("pillarL1");
	scn_pillarLeft2 = MenuScene->GetObjectByID("pillarL2");
	scn_pillarRight1 = MenuScene->GetObjectByID("pillarR1");
	scn_pillarRight2 = MenuScene->GetObjectByID("pillarR2");
	scn_middlePart = MenuScene->GetObjectByID("midpart1");

	//scn_btnPlay = MenuScene->GetObjectByID("btnplay");
	//scn_btnOptions = MenuScene->GetObjectByID("btnoptions");
	//scn_btnExit = MenuScene->GetObjectByID("btnexit");
	//scn_btnProfile = MenuScene->GetObjectByID("btnprofilename");
	//scn_btnExtras = MenuScene->GetObjectByID("btncredits");
	//scn_btnCredits = MenuScene->GetObjectByID("btnextras");

	//Localized
	CObject *menu_title = new CObject();
	menu_title->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\MENUTITLE.SPR", 416, 147, 20);
	menu_title->SetUID("menutitle");
#ifndef _CEBUILD
	menu_title->PlayAnimation(1);
#endif // !_CEBUILD

	/*TODO: Delete after testing
	ProfileSystem::Instance->ActiveProfile.CurrentLevel = 7;
	ProfileSystem::Instance->ActiveProfile.CurrentArea = 113;
	ProfileSystem::Instance->ActiveProfile.Inventory[225] = true;
    */
	MenuScene->PushObject(menu_title);

#ifdef _DEMOBUILD
	scn_btnPlay = new CObject();
	scn_btnPlay->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MENUBTNS\\PLAY_FREE.SPR", 812, 580, 9);//807, 465, 9); 
	scn_btnPlay->SetUID("btnplay");
	scn_btnPlay->SetScale(0.8);
	MenuScene->PushObject(scn_btnPlay);
#else
	scn_btnPlay = new CObject();
	scn_btnPlay->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MENUBTNS\\PLAY.SPR", 812, 535, 9);//807, 465, 9); 
	scn_btnPlay->SetUID("btnplay");
	scn_btnPlay->SetScale(0.8);
	MenuScene->PushObject(scn_btnPlay);
#endif

	scn_btnCredits = new CObject();
	scn_btnCredits->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\BTNCREDITS.SPR", 1009, 736, 16);
	scn_btnCredits->SetUID("btncredits");
	scn_btnCredits->SetEnable(false);
	MenuScene->PushObject(scn_btnCredits);

	scn_btnExit = new CObject();
	scn_btnExit->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\BTNEXIT.SPR", 817, 600, 11);
	scn_btnExit->SetUID("btnexit");
	scn_btnExit->SetEnable(false);
	MenuScene->PushObject(scn_btnExit);

	scn_btnOptions = new CObject();
	scn_btnOptions->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MENUBTNS\\OPTIONS.SPR", 1130, 65, 10);//811, 538, 10);
	scn_btnOptions->SetUID("btnoptions");
	scn_btnOptions->SetScale(0.8);
	MenuScene->PushObject(scn_btnOptions);

	scn_btnProfile = new CObject();
	scn_btnProfile->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MENUBTNS\\PROFILE.SPR", 413, 580, 12);//408, 566, 12);
	scn_btnProfile->SetUID("btnprofilename");
	scn_btnProfile->SetScale(0.8);
	MenuScene->PushObject(scn_btnProfile);

	scn_btnExtras = new CObject();
	scn_btnExtras->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MENUBTNS\\EXTRAS.SPR", 300, 655, 17);//1016, 682, 17);
	scn_btnExtras->SetUID("btnextras");
	scn_btnExtras->SetScale(0.8);
	scn_btnExtras->SetRotate(10);
	MenuScene->PushObject(scn_btnExtras);

	SHARE_BTN = new CObject();
	SHARE_BTN->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MENUBTNS\\SHARE.SPR", 1016, 685, 17);
	SHARE_BTN->SetUID("btnshare");
	SHARE_BTN->SetScale(0.95f);
	SHARE_BTN->SetRotate(-5);
	MenuScene->PushObject(SHARE_BTN);

	LIKE_BTN = new CObject();
	LIKE_BTN->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MENUBTNS\\LIKE.SPR", 1010, 740, 17);
	LIKE_BTN->SetUID("btnlike");
	LIKE_BTN->SetScale(0.9);
	LIKE_BTN->SetRotate(-5);
	MenuScene->PushObject(LIKE_BTN);

	SIGN_UP_BTN = new CObject();
	SIGN_UP_BTN->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MENUBTNS\\SIGNUP.SPR", 1275, 642, 17);
	SIGN_UP_BTN->SetUID("btnlike");
	SIGN_UP_BTN->SetScale(0.8);
	//SIGN_UP_BTN->SetRotate(-5);
	MenuScene->PushObject(SIGN_UP_BTN);

#ifdef _DEMOBUILD
	scn_btnUnlocknow = new CObject();
	scn_btnUnlocknow->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MENUBTNS\\UNLOCK.SPR", 800, 465, 9);
	scn_btnUnlocknow->SetUID("UNLOCK_BUTTON");
	scn_btnUnlocknow->SetScale(0.7f);
	MenuScene->PushObject(scn_btnUnlocknow);
#endif

	scn_fgStoneLeft = MenuScene->GetObjectByID("fgstoneL");
	scn_fgStoneRight = MenuScene->GetObjectByID("fgstone");

	scn_extrasStone = MenuScene->GetObjectByID("extrastone");
	scn_groundStone = MenuScene->GetObjectByID("groundpillar");
	scn_crystalGlow = MenuScene->GetObjectByID("crystalglow");
	/*
	#ifdef K_ANDROID
	GFApp->FetchUserData();
	//scn_freeversion = MenuScene->GetObjectByID("FREE_VERSION");

	

	scn_hintStore = new CObject();
	scn_hintStore->LoadInit("UI\\HUD\\BTNMENU.SPR", 1200, 200, 25);
	scn_hintStore->SetUID("HINT_STORE");
	scn_hintStore->SetScale(2);
	MenuScene->PushObject(scn_hintStore);
    #ifdef UNLOCKED
	scn_hintStore->Show(false);
	#endif

	#endif
	*/

	#ifdef K_iOS
				//MenuScene->GetObjectByID("FREE_VERSION")->Show(false);
				//MenuScene->GetObjectByID("UNLOCK_BUTTON")->Show(false);
	#endif
	scn_gameTitle = MenuScene->GetObjectByID("menutitle");
	//credits - turn off CE
#ifndef _CEBUILD
	scn_gameTitle->PlayAnimation(1);
#endif

	for (int i = 0; i < NUM_FLYING_PAPS; i++)
	{
		pappus[i] = NULL;
	}

	isWaitingForMainChapterRevisit = false;
	MainChapterRevisitTicks = 0.0f;

#ifdef _ENABLE_CURL
		//User Data GameObjects 
		SceneData dataBtn;
		dataBtn.objType = "Object";
		dataBtn.uid = "dataSendBtn";
		dataBtn.spriteString = "UI\\HUD\\RDESC.SPR";
		dataBtn.spriteAnim = 2;
		dataBtn.x = 220;
		dataBtn.y = 700;
		dataBtn.ZOrder = eZOrder_OptionsScn + 23;

		dataButton = MenuScene->CreateObject(dataBtn);
		dataButton->Show(false);

		dataBtnTxt = new CBitmapText();
		dataBtnTxt->Text = StringTable::getString("dataBtnText");
		dataBtnTxt->SetZ(eZOrder_OptionsScn+26);
		dataBtnTxt->SetAlignment(Align_Center);
		dataBtnTxt->LoadFont(eFont_Small);
		dataBtnTxt->SetXY(165, 685);
		dataBtnTxt->Show(false);

		SceneData _dataBG;
		_dataBG.objType = "Object";
		_dataBG.uid = "dataDigBG";
		_dataBG.spriteString = "UI\\BUGREPORT.SPR";
		_dataBG.spriteAnim = 1;
		_dataBG.x = 680;
		_dataBG.y = 230;
		_dataBG.ZOrder = eZOrder_OptionsScn + 24;

		dataBG = MenuScene->CreateObject(_dataBG);
		dataBG->SetEnable(false);
		
		SceneData dataOKBtn;
		dataOKBtn.objType = "Object";
		dataOKBtn.uid = "dataOKBtn";
		dataOKBtn.spriteString = "MNU\\MODE\\BTNOK.SPR";
		dataOKBtn.spriteAnim = 0;
		dataOKBtn.x = 530;
		dataOKBtn.y = 480;
		dataOKBtn.ZOrder = eZOrder_OptionsScn + 25;

		dataOKButton = MenuScene->CreateObject(dataOKBtn);
		dataOKButton->SetEnable(false);

		SceneData dataNoBtn;
		dataNoBtn.objType = "Object";
		dataNoBtn.uid = "dataNoBtn";
		dataNoBtn.spriteString = "MNU\\MODE\\BTNCANCEL.SPR";
		dataNoBtn.spriteAnim = 0;
		dataNoBtn.x = 830;
		dataNoBtn.y = 480;
		dataNoBtn.ZOrder = eZOrder_OptionsScn + 25;

		dataCancelBtn = MenuScene->CreateObject(dataNoBtn);
		dataCancelBtn->SetEnable(false);

		SceneData dataCur;
		dataCur.objType = "Object";
		dataCur.uid = "dataCursor";
		dataCur.spriteString = "UI\\IB.SPR";
		dataCur.spriteAnim = 0;
		dataCur.x = 530;
		dataCur.y = 97;
		dataCur.ZOrder = eZOrder_OptionsScn + 25;

		dataCursor = MenuScene->CreateObject(dataCur);
		dataCursor->Show(false);

		emailAddress = new CBitmapText();
		emailAddress->SetXY(530,97);//1366x768
		emailAddress->SetZ(10010);	
		emailAddress->Alignment = Align_Left;
		emailAddress->LoadFont(eFont_Small);
		emailAddress->Show(true);
		emailAddress->Text = "";	

		emailMessage = new CBitmapText();
		emailMessage->SetXY(500,90);//1366x768
		emailMessage->SetZ(10010);	
		emailMessage->Alignment = Align_Left;
		emailMessage->LoadFont(eFont_Small);
		emailMessage->Show(false);
		emailMessage->Text = "";

		emailMsgLine1 = new CBitmapText();
		emailMsgLine1->SetXY(445,190);//1366x768
		emailMsgLine1->SetZ(10010);	
		emailMsgLine1->Alignment = Align_Left;
		emailMsgLine1->LoadFont(eFont_Small);
		emailMsgLine1->Show(true);
		emailMsgLine1->Text = "";	

		emailMsgLine2 = new CBitmapText();
		emailMsgLine2->SetXY(445,220);//1366x768
		emailMsgLine2->SetZ(10010);	
		emailMsgLine2->Alignment = Align_Left;
		emailMsgLine2->LoadFont(eFont_Small);
		emailMsgLine2->Show(true);
		emailMsgLine2->Text = "";

		emailMsgLine3 = new CBitmapText();
		emailMsgLine3->SetXY(445,250);//1366x768
		emailMsgLine3->SetZ(10010);	
		emailMsgLine3->Alignment = Align_Left;
		emailMsgLine3->LoadFont(eFont_Small);
		emailMsgLine3->Show(true);
		emailMsgLine3->Text = "";

		emailMsgLine4 = new CBitmapText();
		emailMsgLine4->SetXY(445,280);//1366x768
		emailMsgLine4->SetZ(10010);	
		emailMsgLine4->Alignment = Align_Left;
		emailMsgLine4->LoadFont(eFont_Small);
		emailMsgLine4->Show(true);
		emailMsgLine4->Text = "";

		emailMsgLine5 = new CBitmapText();
		emailMsgLine5->SetXY(445,310);//1366x768
		emailMsgLine5->SetZ(10010);	
		emailMsgLine5->Alignment = Align_Left;
		emailMsgLine5->LoadFont(eFont_Small);
		emailMsgLine5->Show(true);
		emailMsgLine5->Text = "";

		emailMsgLine6 = new CBitmapText();
		emailMsgLine6->SetXY(445,340);//1366x768
		emailMsgLine6->SetZ(10010);	
		emailMsgLine6->Alignment = Align_Left;
		emailMsgLine6->LoadFont(eFont_Small);
		emailMsgLine6->Show(true);
		emailMsgLine6->Text = "";

		emailMsgLine7 = new CBitmapText();
		emailMsgLine7->SetXY(445,370);//1366x768
		emailMsgLine7->SetZ(10010);	
		emailMsgLine7->Alignment = Align_Left;
		emailMsgLine7->LoadFont(eFont_Small);
		emailMsgLine7->Show(true);
		emailMsgLine7->Text = "";

		messageBoxWidth = 470;
		
		isCollectDia = false;
		emailFieldHasFocus = true;

		for(int i = 0; i < 42; i++)
		{
			keyPressed[i] = false;
		}
	#endif
	if(!GFInstance->IsSlowSystem)
	{
		//Papus flying only for high end systems
		for (int i = 0; i < NUM_FLYING_PAPS; i++)
		{
			pappus[i] = new CObject();
			pappus[i]->LoadInit(std::string("MNU\\PAPTUS" + MKSTR((i+1))+".SPR"), 0, 0, 7);

			//set properties
			pappus[i]->SetBlendMode(eBlend_AlphaAdditive);

			//in range -5 to -10
			papVelY[i] = -8.0f+float(rand()%4);
			papVelX[i] = -10.0f+float(rand()%20);
			papAccelY[i] = 0;

			pappus[i]->SetScale(0.65f+0.35f*(float(rand()%10)/10.0f));

			pappus[i]->SetAlpha(0.80f+0.20f*(float(rand()%10)/10.0f));		

			if(rand()%5>3)
				pappus[i]->SetZ(20);			

			//171 to 1195
			pappus[i]->Pos.x = 171.0f+float(rand()%256)+i*256.0f;
			pappus[i]->Pos.y = 200.0f+float(rand()%420);			
		}
	}

	MenuScene->GetObjectByID("fireglowL")->SetScale(2.0f);
	MenuScene->GetObjectByID("fireglowL")->SetBlendMode(eBlend_AlphaAdditive);
	MenuScene->GetObjectByID("fireglowL")->Animate(0.45f, 0.75f, 2.25f);

	MenuScene->GetObjectByID("fireglowR")->SetScale(2.0f);
	MenuScene->GetObjectByID("fireglowR")->SetBlendMode(eBlend_AlphaAdditive);
	MenuScene->GetObjectByID("fireglowR")->Animate(0.45f, 0.75f, 2.0f, 1.0f);

	if(GFInstance->IsSlowSystem)
	{
		//fire is disabled. So no need for glows
		MenuScene->GetObjectByID("fireglowL")->SetEnable(false);
		MenuScene->GetObjectByID("fireglowR")->SetEnable(false);
	}

	MenuScene->GetObjectByID("fireglowP")->SetBlendMode(eBlend_AlphaAdditive);
	MenuScene->GetObjectByID("fireglowP")->Animate(0.45f, 0.75f, 3.0f);

	scn_crystalGlow->SetBlendMode(eBlend_AlphaAdditive);
	scn_crystalGlow->Animate(0.25f, 1.0f, 7.0f);

	scn_MomGlow->SetBlendMode(eBlend_AlphaAdditive);
	scn_MomGlow->Animate(0.25f, 1.0f, 6.0f);
	scn_MomRays->SetBlendMode(eBlend_AlphaAdditive);
	scn_MomRays->Animate(0.25f, 1.0f, 5.0f);

	//init Pos setting
	scn_gameTitle->InitPos = scn_gameTitle->Pos;

	scn_Bg->InitPos = scn_Bg->Pos;
	scn_MomGlow->SetPos(scn_MomGlow->GetPosPointer());
	scn_MomRays->SetPos(scn_MomRays->GetPosPointer());

	scn_pillarLeft1->InitPos = scn_pillarLeft1->Pos;
	scn_pillarLeft2->InitPos = scn_pillarLeft2->Pos;
	scn_pillarRight1->InitPos = scn_pillarRight1->Pos;
	scn_pillarRight2->InitPos = scn_pillarRight2->Pos;

	scn_middlePart->SetPos(scn_middlePart->GetPosPointer());
	scn_btnPlay->SetPos(scn_btnPlay->GetPosPointer());
	scn_btnOptions->SetPos(scn_btnOptions->GetPosPointer());
	scn_btnExit->SetPos(scn_btnExit->GetPosPointer());
	scn_btnProfile->SetPos(scn_btnProfile->GetPosPointer());

	scn_fgStoneLeft->SetPos(scn_fgStoneLeft->GetPosPointer());
	scn_fgStoneRight->SetPos(scn_fgStoneRight->GetPosPointer());

	scn_extrasStone->SetPos(scn_extrasStone->GetPosPointer());
	scn_btnExtras->SetPos(scn_btnExtras->GetPosPointer());
	scn_btnCredits->SetPos(scn_btnCredits->GetPosPointer());
	scn_groundStone->SetPos(scn_groundStone->GetPosPointer());
	scn_crystalGlow->SetPos(scn_crystalGlow->GetPosPointer());
	#ifdef K_ANDROID
			//scn_freeversion->SetPos(scn_freeversion->GetPosPointer());
			//scn_btnUnlocknow->SetPos(scn_btnUnlocknow->GetPosPointer());
    #endif

    //Language Selection

	langBG = new CObject();
	langBG->LoadInit("MNU\\IGM\\PAUSEBG.SPR", 683, 386, eZOrder_OptionsScn + 100);
	langBG->SetUID("langBG");
	MenuScene->PushObject(langBG);

	int _X = 683; int _Y = 185; int _YOff = 55;
	enBtn = new CObject();
	enBtn->LoadInit("MNU\\OP\\BTNOK.SPR", _X, _Y, eZOrder_OptionsScn + 101);
	enBtn->SetUID("enBtn");
	MenuScene->PushObject(enBtn);

    enBtnTxt = new CObject();
    enBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\ENGLISH.spr", _X, _Y - 1, eZOrder_OptionsScn + 102);
    enBtnTxt->SetUID("enBtnTxt");
    MenuScene->PushObject(enBtnTxt);

    brBtn = new CObject();
    brBtn->LoadInit("MNU\\OP\\BTNOK.SPR", _X, _Y + (_YOff), eZOrder_OptionsScn + 101);
    brBtn->SetUID("brBtn");
    MenuScene->PushObject(brBtn);

    brBtnTxt = new CObject();
    brBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\PORTUGUESE.spr", _X, _Y - 1 + (_YOff), eZOrder_OptionsScn + 102);
    brBtnTxt->SetUID("brBtnTxt");
    MenuScene->PushObject(brBtnTxt);

    deBtn = new CObject();
    deBtn->LoadInit("MNU\\OP\\BTNOK.SPR", _X, _Y + (_YOff*2), eZOrder_OptionsScn + 101);
    deBtn->SetUID("deBtn");
    MenuScene->PushObject(deBtn);

    deBtnTxt = new CObject();
    deBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\GERMAN.spr", _X, _Y - 1 + (_YOff*2), eZOrder_OptionsScn + 102);
    deBtnTxt->SetUID("deBtnTxt");
    MenuScene->PushObject(deBtnTxt);

    frBtn = new CObject();
    frBtn->LoadInit("MNU\\OP\\BTNOK.SPR", _X, _Y + (_YOff*3), eZOrder_OptionsScn + 101);
    frBtn->SetUID("frBtn");
    MenuScene->PushObject(frBtn);

    frBtnTxt = new CObject();
    frBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\FRENCH.spr", _X, _Y - 1 + (_YOff*3), eZOrder_OptionsScn + 102);
    frBtnTxt->SetUID("frBtnTxt");
    MenuScene->PushObject(frBtnTxt);

	esBtn = new CObject();
	esBtn->LoadInit("MNU\\OP\\BTNOK.SPR", _X, _Y + (_YOff * 4), eZOrder_OptionsScn + 101);
	esBtn->SetUID("esBtn");
	MenuScene->PushObject(esBtn);

//	esBtnTxt = new CObject();
//	esBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\SPANISH.spr", _X, _Y - 1 + (_YOff * 4), eZOrder_OptionsScn + 102);
//	esBtnTxt->SetUID("esBtnTxt");
//	MenuScene->PushObject(esBtnTxt);

    itBtn = new CObject();
    itBtn->LoadInit("MNU\\OP\\BTNOK.SPR", _X, _Y + (_YOff*5), eZOrder_OptionsScn + 101);
    itBtn->SetUID("itBtn");
    MenuScene->PushObject(itBtn);

    itBtnTxt = new CObject();
    itBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\ITALIAN.spr", _X, _Y - 1 + (_YOff*5), eZOrder_OptionsScn + 102);
    itBtnTxt->SetUID("itBtnTxt");
    MenuScene->PushObject(itBtnTxt);

    ndBtn = new CObject();
    ndBtn->LoadInit("MNU\\OP\\BTNOK.SPR", _X, _Y + (_YOff*6), eZOrder_OptionsScn + 101);
    ndBtn->SetUID("ndBtn");
    MenuScene->PushObject(ndBtn);

    ndBtnTxt = new CObject();
    ndBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\DUTCH.spr", _X, _Y - 1 + (_YOff*6), eZOrder_OptionsScn + 102);
    ndBtnTxt->SetUID("ndBtnTxt");
    MenuScene->PushObject(ndBtnTxt);

    ruBtn = new CObject();
    ruBtn->LoadInit("MNU\\OP\\BTNOK.SPR", _X, _Y + (_YOff*7), eZOrder_OptionsScn + 101);
    ruBtn->SetUID("ruBtn");
    MenuScene->PushObject(ruBtn);

    ruBtnTxt = new CObject();
    ruBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\RUSSIAN.spr", _X, _Y - 1 + (_YOff*7), eZOrder_OptionsScn + 102);
    ruBtnTxt->SetUID("ruBtnTxt");
    MenuScene->PushObject(ruBtnTxt);

	langBackBtn = new CObject();
	langBackBtn->LoadInit("MNU\\OP\\BTNOK.SPR", 683, 680, eZOrder_OptionsScn + 101);
	langBackBtn->SetUID("langBackBtn");
	MenuScene->PushObject(langBackBtn);

	langBackBtnTxt = new CObject();
	langBackBtnTxt->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 683, 680, eZOrder_OptionsScn + 102);
	langBackBtnTxt->SetUID("langBackBtnTxt");
	MenuScene->PushObject(langBackBtnTxt);


	langRestartBG = new CObject();
	langRestartBG->LoadInit("UI\\W1.SPR", 683, 400, eZOrder_OptionsScn + 200);
	langRestartBG->SetUID("langRestartBG");
	langRestartBG->SetEnable(false);
	MenuScene->PushObject(langRestartBG);

	langRestartOKBtn = new CObject();
	langRestartOKBtn->LoadInit("MNU\\OP\\BTNOK.SPR", 683, 470, eZOrder_OptionsScn + 201);
	langRestartOKBtn->SetUID("langRestartOKBtn");
	langRestartOKBtn->SetEnable(false);
	MenuScene->PushObject(langRestartOKBtn);

	langRestartOKBtnTxt = new CObject();
	langRestartOKBtnTxt->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\OK.SPR", 683, 470, eZOrder_OptionsScn + 202);
	langRestartOKBtnTxt->SetUID("langRestartOKBtnTxt");
	langRestartOKBtnTxt->SetEnable(false);
	MenuScene->PushObject(langRestartOKBtnTxt);

	langRestartText = new CBitmapText();
	langRestartText->SetZ(eZOrder_OptionsScn + 210);
	langRestartText->SetPos(PointSystem::CreateCPoint(683, 350, 0));
	langRestartText->LoadFont(eFont_25);
	langRestartText->SetText(StringTable::getString("langRestartTxt"));
	langRestartText->SetAlignment(Align_Center);
	langRestartText->SetColor(1,1,1);
	langRestartText->Show(false);

	HideLanguageSelection();

	mouseDiff.x = 0;
	mouseDiff.y = 0;

	mouseTarget.x = 0;
	mouseTarget.y = 0;
	
	VFXCrystalFg = new SuperFX("PRT\\MNU\\crystalfg.INI", PointSystem::CreateCPoint(852,724,0), 50); //under midpart
	VFXCrystalFg->Start();
	VFXCrystalFg->InitPos = VFXCrystalFg->Pos;

	VFXCrystalBg = new SuperFX("PRT\\MNU\\crystalbg.INI", PointSystem::CreateCPoint(533,594,0), 5);
	VFXCrystalBg->Start();
	VFXCrystalBg->InitPos = VFXCrystalBg->Pos;

	VFXButton = new SuperFX("PRT\\MNU\\btnvfx.INI", PointSystem::CreateCPoint(807,457,0), 100); 
	//VFXButton->Start();

	VFXSnow = new SuperFX("PRT\\MNU\\snow.INI", PointSystem::CreateCPoint(595,161,0), 5);
	VFXSnow->Start();
	VFXSnow->AdvanceByTime(2.0f);
	
	ModeTextsLineWidth = 800;

	SetZ(100);

	fireLInitPos = fireLPos = 10;
	fireRInitPos = fireRPos = 1020;	
	
	lpVid_fire = NULL;

	if(!GFInstance->IsSlowSystem)
	{
		//Bg Loop - only for high end system
		lpVid_fire = new KVideo;
		bool bSuccess = lpVid_fire->openVideo (FileSystem::charPath("OGV\\MNU\\FIRE.OGV"),true,NULL,NULL,true);
		if (!bSuccess) 
		{
			KMiscTools::messageBox ("Video loading error!", "Couldn't open bgLoop.OGV");
			exit(0);
		}
		isVidFireLoaded = true;
	}

	MaskSpr = GFInstance->LoadBufferObject(MASK_OBJ);	
	//MaskSpr->SetPos(&MaskSprPos);	
	MaskSpr->SetZ(30);
	MaskSpr->Show(false);

	//Need to load it as this object has a different z depth!!
	MaskSpr2 = new CObject();
	MaskSpr2->LoadSprite("UI\\MASK.SPR");
	MaskSpr2->SetXY(683,384);
	MaskSpr2->SetScale(2.0);
	MaskSpr2->SetZ(1020);
	MaskSpr2->Show(false);

	//For Mode Popup over Options
	ModeMaskSpr = new CObject();
	ModeMaskSpr->LoadSprite("UI\\MASK.SPR");
	ModeMaskSpr->SetXY(683,384);
	ModeMaskSpr->SetScale(2.0);
	ModeMaskSpr->SetZ(eZOrder_ModesMenuMask);
	ModeMaskSpr->Show(false);

	isComicOpenedFromSelectAdvPopup = false;
	
	TextEntry = NULL;
	
	for(int i=0; i < MMB_Max; ++i)
	{
		Buttons[i] = MenuScene->GetObjectByID( MMButtonID[i] );
		//Buttons[i]->SetBlendMode(eBlend_AlphaAdditive);
	}
	ButtonRects[MMB_Play] = PlayRect;
	ButtonRects[MMB_Options] = OptionsRect;
	ButtonRects[MMB_Exit]= ExitRect;
	ButtonRects[MMB_Extras]= ExtrasRect;
	ButtonRects[MMB_Credits]= CreditsRect;

#ifdef  TOUCH_UI
	//ButtonRects[MMB_Unlocknow]= UnlockNowRect;
	Buttons[MMB_Exit]->SetEnable(false);
	//Buttons[MMB_Options]->SetY(Buttons[MMB_Options]->Pos.y + 25);
	//Buttons[MMB_Play]->SetScale(1.25f);
#endif

	//New UI Buttons
	/*
	Buttons[MMB_Play] = new CObject();
	Buttons[MMB_Play]->LoadInit("MNU\\MENUBTN\\PLAY.SPR", 817, 600, 11);
	Buttons[MMB_Play]->SetUID("PLAY_BTN");
	MenuScene->PushObject(PLAY_BTN);
	*/

	PlayBtnClicked = false;
	
	ProfileButton = MenuScene->GetObjectByID( "btnprofilename" );

	InitMessageBanner();	
		
	WelcomePlayerNameText = new CBitmapText();
	WelcomePlayerNameText->LoadFont(eFont_26);
	//WelcomePlayerNameText->SetPos(PointSystem::CreateCPoint(413.0f, 636.0f, 0.0f));	
	WelcomePlayerNameText->SetPos(PointSystem::CreateCPoint(413.0f, 516.0f, 0.0f));	

	profileTxtInitPos = WelcomePlayerNameText->Pos;
	profileTxtPos = profileTxtInitPos;

	WelcomePlayerNameText->SetAlignment(Align_Center);
	WelcomePlayerNameText->SetZ(8+4);
	WelcomePlayerNameText->SetRotate(10);
	WelcomePlayerNameText->SetColor((122/255.0f),(128/255.0f),(138/255.0f)); //contrast of base grey
	WelcomePlayerNameText->Show(false);

	IsMouseOverWelcomeText = false;

	////////////////////////////////////////////////////////////
	//Profile Popup Player Name Texts
	CPoint pos = CPoint(683.0f, 230.0f, 0.0f);	
	for(int i=0; i < MAX_PROFILES; ++i)
	{		
		ProfileText[i] = new CBitmapText();	
		ProfileText[i]->LoadFont(eFont_26);
		ProfileText[i]->SetPos(&pos);
		ProfileText[i]->SetAlignment(Align_Center);
		ProfileText[i]->SetZ(1010);


		if( i == ProfileSystem::Instance->ProfileSlot )
			ProfileText[i]->SetColor(1.0f, 1.0f, 1.0f); //yellow
		else
			ProfileText[i]->SetColor(0.75f, 0.75f, 0.75f); //orange
			
		ProfileText[i]->Show(false);
		if( ProfileSystem::Instance->IsProfileValid(i) )
			ProfileText[i]->SetText(ProfileSystem::Instance->GetProfileName(i));
		else
			ProfileText[i]->SetText("");
		pos.y += 60.0f;
	}

	iInactiveProfileSlot = -1;
	for( int i = 0; i < MAX_PROFILES; i++ )
	{
		IsSlotCreated[i] = false;
		IsSlotDeleted[i] = false;
	}
	
	SelectModePopupScene = GFInstance->LoadBufferScene(MODESELECT_SCN);	//LOAD BUFFERED
	if(SelectModePopupScene->GetObjectByID("modeBg")->ZOrder<=1)
		SelectModePopupScene->SetZOff(eZOrder_ModeSelectScn);
	SelectModePopupScene->Show(false);

	IsModeSelectionFromOptions = false;

	ModeButtons[0] = SelectModePopupScene->GetObjectByID("modCas");
	ModeButtons[1] = SelectModePopupScene->GetObjectByID("modAdv");
	ModeButtons[2] = SelectModePopupScene->GetObjectByID("modExp");
	ModeButtons[3] = SelectModePopupScene->GetObjectByID("modCus");

	SelectedMode = 0; //Mode already choosen

	HintTimeInt=0;
	SkipTimeInt=0;
	IsHintEnabled=true;
	IsSkipEnabled=true;
	IsTutorEnabled=true;
	IsSparklesEnabled=true;
	IsHOSparklesEnabled=false;
	IsTextTipsEnabled=false;
	IsAwardNotifyEnabled = false;

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

	#ifdef _DISABLE_EXTRA
		CustMode_ItemsBg->PlayAnimation(1);
	#else
		CustMode_ItemsBg->PlayAnimation(0);
	#endif

	CPoint textPos;
	for(int i=0; i < MODE_TEXT_LINES; ++i)
	{
		textPos = ModeBoxPos;		

		textPos.x += ModeTextsOff.x;//CENTRALIZE
		textPos.y += ModeTextsOff.y + ModeTextsYSpacing * i;

		ModeDescriptionTexts[i] = new CBitmapText();
		ModeDescriptionTexts[i]->SetZ(eZOrder_ModeSelectScn+10);
		ModeDescriptionTexts[i]->LoadFont(eFont_26);
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
	
	ExitGamePopupScene = new CHOScene();
	ExitGamePopupScene->Load("MNU\\EXIT\\EXIT.SCN");
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", ExitGamePopupScene->GetObjectByID("BtnOk")->Pos.x, ExitGamePopupScene->GetObjectByID("BtnOk")->Pos.y, ExitGamePopupScene->GetObjectByID("BtnOk")->ZOrder + 1);
	temp->SetUID("btnoktext");
	ExitGamePopupScene->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", ExitGamePopupScene->GetObjectByID("BtnCancel")->Pos.x, ExitGamePopupScene->GetObjectByID("BtnCancel")->Pos.y, ExitGamePopupScene->GetObjectByID("BtnCancel")->ZOrder + 1);
	temp->SetUID("btncanceltext");
	ExitGamePopupScene->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\QUITTEXT.SPR", 700, 260, ExitGamePopupScene->GetObjectByID("BtnOk")->ZOrder + 1);
	temp->SetUID("quittext");
	ExitGamePopupScene->PushObject(temp);
	#ifdef TOUCH_UI
		//This will be called gracefully by pressing BACK button anyting before the game / Main menu area
		ExitGamePopupScene->SetZOff(10050); // On top of thje Add player popup, as this should the Top most popup
	#else
		ExitGamePopupScene->SetZOff(1000);
	#endif
	ExitGamePopupScene->Show(false);

	ReviewGamePopupScene = new CHOScene();
	ReviewGamePopupScene->Load("MNU\\REVIEW\\REVIEW.SCN");
	ReviewGamePopupScene->SetZOff(1000);
	ReviewGamePopupScene->Show(false);

	int MainPts[] = {847,233,736,153,597,138,449,266,430,389,476,481};
	PolyMainChapter=  new CPolygon(6);
	PolyMainChapter->Init(MainPts);
	int BonusPts[] = {877,252,923,352,912,466,847,551,690,598,493,500};
	PolyBonusChapter=  new CPolygon(6);
	PolyBonusChapter->Init(BonusPts);

	SelectAdventurePopupScene = new CHOScene();
	SelectAdventurePopupScene->Load("MNU\\SELADV\\ADV.SCN");

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\MAINTXT.SPR", SelectAdventurePopupScene->GetObjectByID("mainchapbtn")->Pos.x, SelectAdventurePopupScene->GetObjectByID("mainchapbtn")->Pos.y, SelectAdventurePopupScene->GetObjectByID("mainchapbtn")->ZOrder + 5);
	temp->SetUID("MAINTXT");
	SelectAdventurePopupScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\BONUSTXT.SPR", SelectAdventurePopupScene->GetObjectByID("bonuschapbtn")->Pos.x, SelectAdventurePopupScene->GetObjectByID("bonuschapbtn")->Pos.y, SelectAdventurePopupScene->GetObjectByID("bonuschapbtn")->ZOrder + 5);
	temp->SetUID("BONUSTXT");
	SelectAdventurePopupScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\COMICBOOK_UNLOCKED.SPR", SelectAdventurePopupScene->GetObjectByID("ComicBook")->Pos.x, SelectAdventurePopupScene->GetObjectByID("ComicBook")->Pos.y, SelectAdventurePopupScene->GetObjectByID("ComicBook")->ZOrder + 5);
	temp->SetUID("COMICBOOK_UNLOCKED");
	SelectAdventurePopupScene->PushObject(temp);

	SelectAdventurePopupScene->SetZOff(1000);
	SelectAdventurePopupScene->Show(false);

	IsMainChapterSelected = false;
	
	OptionsPopupScene = GFInstance->LoadBufferScene(OPTIONS_PSCN);	//LOAD BUFFERED
	if(OptionsPopupScene->GetObjectByID("optBg")->ZOrder<=1)
		OptionsPopupScene->SetZOff(eZOrder_OptionsScn);
	OptionsPopupScene->Show(false);

	musicVolTxt = new CBitmapText();
	musicVolTxt->SetAlignment(Align_Center);
	musicVolTxt->SetZ(eZOrder_OptionsScn + 10);
	musicVolTxt->LoadFont(eFont_25);
	musicVolTxt->SetText(AssetsStringTable::getString("musicVolTxt"));
	musicVolTxt->SetPos(PointSystem::CreateCPoint(685, 176, 0));
	musicVolTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 		
	musicVolTxt->Show(false);

	if (OptionsPopupScene->GetObjectByID("langSelectBtn") == NULL)
	{
		langSelectBtn = new CObject();
#ifdef TOUCH_UI	
		langSelectBtn->LoadInit("MNU\\OP\\BTNGREEN.SPR", 785, 420, eZOrder_OptionsScn + 10);
#else	
		langSelectBtn->LoadInit("MNU\\OP\\BTNGREEN.SPR", 785, 545, eZOrder_OptionsScn + 10);
#endif // TOUCH_UI

		langSelectBtn->SetUID("langSelectBtn");
		langSelectBtn->Show(false);
		OptionsPopupScene->PushObject(langSelectBtn);
	}
	else
	{
		langSelectBtn = OptionsPopupScene->GetObjectByID("langSelectBtn");
		langSelectBtn->SetEnable(true);
	}

    std::string _langTXT = "";
    switch(StringTable::GetMyLangaugeID())
    {
        case LN_ENGLISH:_langTXT = "ENGLISH.spr"; break;
        case LN_BRAZILIAN_PORTUGUESE:_langTXT = "PORTUGUESE.spr"; break;
        case LN_FRENCH:_langTXT = "FRENCH.spr"; break;
        case LN_GERMAN:_langTXT = "GERMAN.spr"; break;
        case LN_ITALIAN:_langTXT = "ITALIAN.spr"; break;
        case LN_NEDERLANDS:_langTXT = "DUTCH.spr"; break;
        case LN_RUSSIAN:_langTXT = "RUSSIAN.spr"; break;
		//case LN_SPANISH:_langTXT = "SPANISH.spr"; break;
        default:_langTXT = "ENGLISH.spr"; break;
    }
	if (OptionsPopupScene->GetObjectByID("langSelectBtnTxt") == NULL)
	{
		langSelectBtnTxt = new CObject();
#ifdef TOUCH_UI
		langSelectBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\" + _langTXT, 785, 420, eZOrder_OptionsScn + 11);
#else
		langSelectBtnTxt->LoadInit("LOCALIZED\\LANGUAGE_BUTTONS\\" + _langTXT, 785, 542, eZOrder_OptionsScn + 11);
#endif
		langSelectBtnTxt->SetUID("langSelectBtnTxt");
		langSelectBtnTxt->Show(false);
		OptionsPopupScene->PushObject(langSelectBtnTxt);
	}
	else
	{
		langSelectBtnTxt = OptionsPopupScene->GetObjectByID("langSelectBtnTxt");
		langSelectBtnTxt->SetEnable(true);
	}

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

	languageTXT = new CBitmapText();
	languageTXT->SetAlignment(Align_Right);
	languageTXT->SetZ(eZOrder_OptionsScn + 10);
	languageTXT->LoadFont(eFont_25);
	languageTXT->SetText(AssetsStringTable::getString("languageTXT"));
#ifndef TOUCH_UI
	languageTXT->SetPos(PointSystem::CreateCPoint(685, 550, 0));
#else
	languageTXT->SetPos(PointSystem::CreateCPoint(685, 425, 0));
#endif // !TOUCH_UI
	languageTXT->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 		
	languageTXT->Show(false);

	modeBtnOver = OptionsPopupScene->GetObjectByID("btnover");
	modeBtnOver->SetEnable(false);
	modeBtnOver->SetBlendMode(eBlend_AlphaAdditive);
	
	ProfilesPopupScene = new CHOScene();
	ProfilesPopupScene->Load("MNU\\PROFSEL\\PSEL.SCN");

	//Localized
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\PROFILESELECTION.SPR", 683, 115, 10);
	temp->SetUID("profileSelectionTxt");
	ProfilesPopupScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\NewProfile.SPR", 683, 228, 11);
	temp->SetUID("NPTxt0");
	ProfilesPopupScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\NewProfile.SPR", 683, 290, 11);
	temp->SetUID("NPTxt1");
	ProfilesPopupScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\NewProfile.SPR", 683, 348, 11);
	temp->SetUID("NPTxt2");
	ProfilesPopupScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\NewProfile.SPR", 683, 408, 11);
	temp->SetUID("NPTxt3");
	ProfilesPopupScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\NewProfile.SPR", 683, 468, 11);
	temp->SetUID("NPTxt4");
	ProfilesPopupScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\OK.SPR", 590, 560, 50);
	temp->SetUID("okBtnTxt");
	ProfilesPopupScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\CANCEL.SPR", 780, 560, 50);
	temp->SetUID("okBtnTxt");
	ProfilesPopupScene->PushObject(temp);

	ProfilesPopupScene->SetZOff(1000);
	ProfilesPopupScene->Show(false);
	IsFromProfilesPopup = false;
    AwaitingEnterNamePopupSlot = -1;

	for (int i = 0; i < MAX_PROFILES; i++)
	{
		ProfNameGlow[i] = ProfilesPopupScene->GetObjectByID(std::string("profGlow")+MKSTR(i));
		ProfDeleteButtons[i] = ProfilesPopupScene->GetObjectByID(std::string("DelBtn")+MKSTR(i));
		ProfNewPlayerTexts[i] = ProfilesPopupScene->GetObjectByID(std::string("NPTxt")+MKSTR(i));
	}

	ProfNameRects[0] =new CRectangle(539.0f,204.0f,292.0f,50.0f);
	ProfNameRects[1] =new CRectangle(537.0f,264.0f,304.0f,51.0f);
	ProfNameRects[2] =new CRectangle(537.0f,326.0f,307.0f,50.0f);
	ProfNameRects[3] =new CRectangle(537.0f,386.0f,302.0f,48.0f);
	ProfNameRects[4] =new CRectangle(539.0f,444.0f,297.0f,52.0f);
	
	ProfDeleteButtonRects[0] = new CRectangle(787.0f,202.0f,54.0f,54.0f);
	ProfDeleteButtonRects[1] = new CRectangle(783.0f,267.0f,58.0f,50.0f);
	ProfDeleteButtonRects[2] = new CRectangle(785.0f,326.0f,58.0f,51.0f);
	ProfDeleteButtonRects[3] = new CRectangle(782.0f,386.0f,62.0f,50.0f);
	ProfDeleteButtonRects[4] = new CRectangle(783.0f,444.0f,61.0f,55.0f);

	EnterYourNamePopupScene = new CHOScene();
	EnterYourNamePopupScene->Load("MNU\\ADDPROF\\ADDPROF.SCN");

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\ACCEPT.SPR", EnterYourNamePopupScene->GetObjectByID("BtnOk")->Pos.x, EnterYourNamePopupScene->GetObjectByID("BtnOk")->Pos.y, EnterYourNamePopupScene->GetObjectByID("BtnOk")->ZOrder + 1);
	temp->SetUID("btnoktext");
	EnterYourNamePopupScene->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\CANCEL.SPR", EnterYourNamePopupScene->GetObjectByID("BtnCancel")->Pos.x, EnterYourNamePopupScene->GetObjectByID("BtnCancel")->Pos.y, EnterYourNamePopupScene->GetObjectByID("BtnCancel")->ZOrder + 1);
	temp->SetUID("btncanceltext");
	EnterYourNamePopupScene->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\ADDPLAYER.SPR", 680, 155, EnterYourNamePopupScene->GetObjectByID("BtnOk")->ZOrder + 1);
	temp->SetUID("addplayertext");
	EnterYourNamePopupScene->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\ENTERYOURNAME.SPR", 680, 250, EnterYourNamePopupScene->GetObjectByID("BtnCancel")->ZOrder + 1);
	temp->SetUID("msgtext");
	EnterYourNamePopupScene->PushObject(temp);

	EnterYourNamePopupScene->SetZOff(1030);
    EnterYourNamePopupScene->SetPosOff(50, 0);
    #ifdef K_ANDROID
        EnterYourNamePopupScene->SetPosOff(0, -100);
    #endif
	#ifdef K_iOS
        EnterYourNamePopupScene->SetPosOff(0, -125);
    #endif
	EnterYourNamePopupScene->Show(false);

	EraseProfilePopupScene = new CHOScene();
	EraseProfilePopupScene->Load("MNU\\DELPROF\\PRODEL.SCN"); 
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", EraseProfilePopupScene->GetObjectByID("BtnOk")->Pos.x, EraseProfilePopupScene->GetObjectByID("BtnOk")->Pos.y, EraseProfilePopupScene->GetObjectByID("BtnOk")->ZOrder + 1);
	temp->SetUID("btnoktext");
	EraseProfilePopupScene->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", EraseProfilePopupScene->GetObjectByID("BtnCancel")->Pos.x, EraseProfilePopupScene->GetObjectByID("BtnCancel")->Pos.y, EraseProfilePopupScene->GetObjectByID("BtnCancel")->ZOrder + 1);
	temp->SetUID("btncanceltext");
	EraseProfilePopupScene->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\DELETEPROFILE.SPR", 680, 250, EraseProfilePopupScene->GetObjectByID("BtnOk")->ZOrder + 1);
	temp->SetUID("deleteplayertext");
	EraseProfilePopupScene->PushObject(temp);
	EraseProfilePopupScene->SetZOff(1030);
	EraseProfilePopupScene->Show(false);

	EraseProfileNameText= new CBitmapText();
	EraseProfileNameText->LoadFont(eFont_26);
	EraseProfileNameText->SetPos(PointSystem::CreateCPoint(667.0f, 410.0f, 0.0f));
	EraseProfileNameText->SetAlignment(Align_Center);
	EraseProfileNameText->SetZ(1032);								
//	EraseProfileNameText->SetColor(1.0f, 1.0f, 1.0f); //white
	EraseProfileNameText->SetColor(0.75f, 0.75f, 0.75f); //Sac: Bijish wanted it to be Grey rather than white
	EraseProfileNameText->Show(false);
	EraseProfileNameText->SetText("");

	ProfilePopupAlpha = PopupAlpha = 1.0f;
	ProfilePopupCounter = PopupCounter = 0;
	PopupState = MMB_Popup_Inactive;
	if( ProfileSystem::Instance->ActiveProfile._IsSplashDone )
		WelcomeTextAlpha2 = 0.0f;
	else
		WelcomeTextAlpha2 = 1.0f;
	
	iActiveProfileSlot = ProfileSystem::Instance->ProfileSlot; 
	if( iActiveProfileSlot < 0 || 
		iActiveProfileSlot > 4 ||
		!ProfileSystem::Instance->IsProfileValid(iActiveProfileSlot) )
	{		
		ProfileSystem::Instance->SetProfileSlot(0);
		
		IsForcedNameEntry = true;
		ShowEnterYourNamePopup(0);
		
	}
	else
	{	
		IsForcedNameEntry = false;

		ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = ProfileSystem::Instance->ProfileSlot;
		
		//Welcome + 
		WelcomePlayerNameText->SetText(ProfileSystem::Instance->GetCurrentName());
		WelcomePlayerNameText->SetAlpha(WelcomeTextAlpha2);
		WelcomePlayerNameText->Show(true);
		
		if( ProfileSystem::Instance->ActiveProfile.IsMuted )
		{						
			Control::Audio->SetGlobalVolumes(0,0,0,true);
			Control::Audio->GlobalMusicVol = ProfileSystem::Instance->ActiveProfile.MusicVol;
			Control::Audio->GlobalSFXVol = ProfileSystem::Instance->ActiveProfile.SFXVol;
			Control::Audio->GlobalVoiceVol = ProfileSystem::Instance->ActiveProfile.VoiceVol;
		}
		else
		{
			//One time splashinit & that too only if vol < 100
			if( ProfileSystem::Instance->ActiveProfile._IsFromSplash )				 
			{
				if( ProfileSystem::Instance->ActiveProfile.SFXVol < 100)
						Control::Audio->SetGlobalSFXVolume(ProfileSystem::Instance->ActiveProfile.SFXVol);
				if( ProfileSystem::Instance->ActiveProfile.MusicVol < 100)
						Control::Audio->SetGlobalSFXVolume(ProfileSystem::Instance->ActiveProfile.MusicVol);
				if( ProfileSystem::Instance->ActiveProfile.VoiceVol < 100)
						Control::Audio->SetGlobalSFXVolume(ProfileSystem::Instance->ActiveProfile.VoiceVol);
						
					
			}
		}				
	}
	
	ProfileSystem::Instance->ActiveProfile._IsFromSplash = false;

	if( ProfileSystem::Instance->ActiveProfile._IsFromGame )
	{
		Control::Audio->ClearUnusedTracks();	
		ProfileSystem::Instance->ActiveProfile._IsFromGame = false;
	}
	
	if( ProfileSystem::Instance->ActiveProfile._IsSplashDone )
	{
		MenuAlpha = 0.5f;
		IsButtonsFadingIn = true;
	}
	else
	{
		MenuAlpha = 1.0f;
		IsButtonsFadingIn = false;
	}
	//ButtonsFading	
	for(int i=0; i < MMB_Max; ++i)
	{
		Buttons[i]->SetAlpha(MenuAlpha);
		Buttons[i]->Show(true);
	}	
	

	IsWelcomePlayerFadeIn = false;		
	
	Control::Audio->LoadSample(aTrackMenu,AudioTrack,true);
	Control::Audio->LoadSample(aVOMenuTest,	AudioVO);
    
	//CreateWatermark();	
	 
	
	if( !Control::Audio->IsPlaying(aTrackMenu) )
		Control::Audio->PlaySample(aTrackMenu,true);

#ifdef K_ANDROID
	/*
	if(GFApp->isUnlocked())
		{
			//scn_freeversion->Show(false);
			scn_btnUnlocknow->Show(false);
		}
		*/

#endif
	Control::Audio->SetGlobalMusicVolume(ProfileSystem::Instance->ActiveProfile.MusicVol);
	Control::Audio->SetGlobalSFXVolume(ProfileSystem::Instance->ActiveProfile.SFXVol);
	Control::Audio->SetGlobalVoiceVolume(ProfileSystem::Instance->ActiveProfile.VoiceVol);

	if(ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed)
		Control::Audio->QuickLoadAndPlaySFX(aSFXKidGiggle);
	Cursor::Show(true);

	extrasMenuControl = new ExtrasMenuControl(MaskSpr);
	extrasMenuControl->SetMemoriesRoomCallback(CC_CALLBACK_0(MainMenuControl::LoadKidsroom,this));
	extrasMenuControl->SetAwardroomCallback(CC_CALLBACK_0(MainMenuControl::LoadAwardsroom,this));
	extrasMenuControl->SetCutsceneVideoCallback(CC_CALLBACK_0(MainMenuControl::LoadCutsceneVideoPlayer,this));
	extrasMenuControl->SetLoadComicCallback(CC_CALLBACK_0(MainMenuControl::LoadComicBook,this));
	extrasMenuControl->SetHOPuzzleSceneLoadCallback(CC_CALLBACK_2(MainMenuControl::LoadPuzzleHOForExtras,this));
	extrasMenuControl->SetCreditsCallback(CC_CALLBACK_0(MainMenuControl::LoadCredits, this));

	extrasMenuControl->SetShowBannerCallback(CC_CALLBACK_2(MainMenuControl::ShowMessageBanner,this));

	AddControl(extrasMenuControl);

//	K_LOG("Comic control loading...");
	extrasComicControl = new ExtrasComicControl();
	extrasComicControl->SetCloseComicCallback(CC_CALLBACK_0(MainMenuControl::OnComicBookClosed,this));
	AddControl(extrasComicControl);
//	K_LOG("Comic control loaded!");
	//Show Extras Popup when coming from Memory Room
	if(ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromMainMenu || ProfileSystem::Instance->ActiveProfile.IsExtrasRoomFromMainMenu || GFInstance->IsCutsceneVideoPlayerActive || ProfileSystem::Instance->ActiveProfile.IsCreditsFromMainMenu)
	{
		ProfileSystem::Instance->ActiveProfile.IsExtrasRoomFromMainMenu = false;
		ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromMainMenu = false;
		GFInstance->IsCutsceneVideoPlayerActive = false;
		
		//TODO Show the previous menu in case of Cutscene Video Playing
		if(ProfileSystem::Instance->ActiveProfile.IsCreditsFromMainMenu) 
		{
			ProfileSystem::Instance->ActiveProfile.IsCreditsFromMainMenu = false;
			//If the Credits has been gone from Main Menu, then why should it open Extras Popup when it comes back ??
		}
		else
		{
			ProfileSystem::Instance->ActiveProfile.IsCreditsFromMainMenu = false;
			//extrasMenuControl->ShowExtrasPopup();
		}
			
	}

	testNodeMesh = NULL;
	//testNodeMesh = new NodeMeshObject(MenuScene->GetObjectByID("menutitle"), 4, 4);

	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		ProfileSystem::Instance->_isExtrasGamePlay = false;
		ProfileSystem::Instance->LoadProfile(iActiveProfileSlot);
	}

		

	if (ProfileSystem::Instance->ActiveProfile.IsBonusChapterUnlocked)
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsBonusChainRemoved)
		{
			ShowBannerText("Explay");
	//		K_LOG("AdventureGamePopup loading..");
			ShowSelectAdventureGamePopup();
	//		K_LOG("AdventureGamePopup loaded...");
		}
	}

	//Special Case : If player has finished game without skipping minigames, give him chanakya
	if (ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed)
	{
		//If player has completed the game
		bool isValidPuzzleSkipped = false;
		for (int i = 0; i < eArea_Max; i++)
		{
			if(ProfileSystem::Instance->ActiveProfile.isSkipUsedAfterMiniGameStart[i])
			{
				//if(i == eArea_L2_S7_PZSoldiers || i == eArea_L6_S5_TabMiniGame || i == eArea_L4_S3_PZDoor || eArea_L7_S2_PZLasyaStatue)
				if(i == eArea_L2_S7_PZSoldiers || i == eArea_L6_S5_TabMiniGame || i == eArea_L4_S3_PZDoor || i == eArea_L7_S2_PZLasyaStatue) //Sac: added i==
				{
					// nothing
				}
				else
				{
					isValidPuzzleSkipped = true;
					break;
				}
			}
		} 

		if(!isValidPuzzleSkipped)
		{
		//	K_LOG("DANGER::Going to award HandleGameCompleteWithoutSkip..."); 
			ProfileSystem::Instance->ActiveProfile.isMiniGameSkipedAfterGameStart = false;
			AwardManager::GetInstance()->HandleGameCompleteWithoutSkip();

		//	K_LOG("Award Chanakya got");
			//Linear way
			//SteamManager::GetInstance()->SetSteamAchievement("Award_Game_without_Skip");
			//kAward_Game_Without_Hint
			//SteamManager::GetInstance()->SetSteamAchievement("Award_Game_without_Skip"); //Chanakya award for players who lost it
		}
	}
	Visible = true;//CRenderListEntity
}

void MainMenuControl::LoadCutsceneVideoPlayer()
{
	//Load Memories Page
	//GFInstance->activeCutsceneID = 2;
	GFInstance->IsCutsceneVideoPlayerActive = true;
	if(!(ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_KidsRoom && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_AwardsRoom))
		ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_ExtrasCutscenePlayer;
	SendMessage(ActionProcessed, ID, xQuestMode);
}

void MainMenuControl::LoadComicBook()
{
	extrasComicControl->ShowComicScene();
}

void MainMenuControl::OnComicBookClosed()
{
	if(!isComicOpenedFromSelectAdvPopup)
	{
		extrasMenuControl->ShowExtrasPopup();
	}
	else
	{
		isComicOpenedFromSelectAdvPopup = false;
	}
}

void MainMenuControl::LoadCredits()
{
	ProfileSystem::Instance->ActiveProfile.IsCreditsFromMainMenu = true;
	ProfileSystem::Instance->ActiveProfile.CreditsPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_Credits;
	SendMessage(ActionProcessed, ID, xQuestMode);
}

void MainMenuControl::LoadKidsroom()
{
	//Load Memories Page
	ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromMainMenu = true;
	if(!(ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_KidsRoom && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_AwardsRoom))
		ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;	
	//ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_KidsRoom;
	SendMessage(ActionProcessed, ID, xQuestMode);
}


void MainMenuControl::LoadAwardsroom()
{
	//Load Extras From Main Menu
	ProfileSystem::Instance->ActiveProfile.IsExtrasRoomFromMainMenu = true;
	//ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
	if(!(ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_KidsRoom && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_AwardsRoom))
		ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_ExtrasRoom;
	SendMessage(ActionProcessed, ID, xQuestMode);
}

void MainMenuControl::LoadPuzzleHOForExtras(int levelID, int hoPuzzleAreaID)
{
	//Check Puzzle/HO Extra Gameplay
	ProfileSystem::Instance->setForExtrasGamePlay();

	ProfileSystem::Instance->ActiveProfile.CurrentLevel = levelID;
	ProfileSystem::Instance->ActiveProfile.CurrentArea = hoPuzzleAreaID;
	SendMessage(ActionProcessed, ID, xQuestMode);
}


void MainMenuControl::ShowExitGamePopup()
{
	MaskSpr->SetAlpha(0.0f);
	MaskSpr->Show(true);
    
	ExitGamePopupScene->Show(true);
	ExitGamePopupScene->SetSceneAlpha(0.0f);
	PopupAlpha = 0.0f;
	PopupState = MMB_Popup_ExitGame;
	PopupCounter = 0;	
}

void MainMenuControl::ShowSelectAdventureGamePopup()
{
	MaskSpr->SetAlpha(0.0f);
	MaskSpr->Show(true);

	SelectAdventurePopupScene->Show(true);
	//TODO: Show this when BOnus chapter is unvieled
	//if(!ProfileSystem::Instance->ActiveProfile.L1S1_BagOpened)

	comicBookUnlockBtn = SelectAdventurePopupScene->GetObjectByID("ComicBook");
	mainBtnGlow = SelectAdventurePopupScene->GetObjectByID("mainchapglow");
	bonusBtnGlow = SelectAdventurePopupScene->GetObjectByID("bonuschapglow");
	bonusBtnLock = SelectAdventurePopupScene->GetObjectByID("bonuslock");
	mainBtnGlow->Show(false);
	bonusBtnGlow->Show(false);

	SelectAdventurePopupScene->GetObjectByID("mainchapbtn")->Show(false);
	SelectAdventurePopupScene->GetObjectByID("bonuschapbtn")->Show(false);


	SelectAdventurePopupScene->GetObjectByID("BONUSTXT")->Show(false);
	SelectAdventurePopupScene->GetObjectByID("MAINTXT")->Show(false);

	bonusBtnLock->Show(false);

	IsMainChapterSelected = true;
	SelectAdventurePopupScene->SetSceneAlpha(0.0f);

	if(ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted)
	{
		comicBookUnlockBtn->SetEnable(true);
		comicBookUnlockBtn->FadeIn();

		SelectAdventurePopupScene->GetObjectByID("COMICBOOK_UNLOCKED")->SetEnable(true);
		SelectAdventurePopupScene->GetObjectByID("COMICBOOK_UNLOCKED")->FadeIn();
		ShowBannerText("Exrou");
	}
	else
	{
		comicBookUnlockBtn->SetEnable(false);
		SelectAdventurePopupScene->GetObjectByID("COMICBOOK_UNLOCKED")->SetEnable(false);
	}

	isChapterThumbsAnimating = false;
	chapThumbAnimTimer = 0;

	PopupAlpha = 0.0f;
	PopupState = MMB_Popup_SelectAdventure;
	PopupCounter = 0;
}

void MainMenuControl::ShowSelectModePopup()
{
	ModeMaskSpr->SetAlpha(0.0f);
	ModeMaskSpr->Show(true);
    
	SelectModePopupScene->Show(true);
	SelectModePopupScene->SetSceneAlpha(0.0f);

	modeNameText->Show(true);

	if(IsModeSelectionFromOptions)
	{
		//Mode chosen from Options. Set based on Active Profile
		int profileMode = ProfileSystem::Instance->ProfileMode[iActiveProfileSlot];
		for (int i = 0; i < MAX_GAMEMODES; i++)
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
			//	K_LOG("HintTimeInt %f",HintTimesArray[HintTimeInt]);
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
			//	K_LOG("SkipTimeInt %f",HintTimesArray[SkipTimeInt]);
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
#ifdef K_ANDROID
			IsAwardNotifyEnabled = true;
#else
			IsAwardNotifyEnabled = true;
#endif

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
	}
	else
	{
		//Set Casual btn to highlight, dim other ones	
		ModeButtons[0]->PlayAnimation(2);
	
		ModeButtons[1]->PlayAnimation(0);
		ModeButtons[2]->PlayAnimation(0);
		ModeButtons[3]->PlayAnimation(0);
	
		ModeTexts_Init(ModeText_Casual);//start with casual mode always!!
						  
		ModeTexts_Show(true);
		ModeTexts_SetAlpha(0.0f);

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
#ifdef K_ANDROID
		IsAwardNotifyEnabled = false;
#else
		IsAwardNotifyEnabled = true;
#endif

		//Reset knobs
		CPoint temp(CUSTOMKNOB_STARTX,HINTKNOB_POSY,0);
		CustMode_HintKnob->SetPos(&temp);
		temp.y = SKIPKNOB_POSY;
		CustMode_SkipKnob->SetPos(&temp);

		HintTimeInt = 0;
		SkipTimeInt = 0;

		CustMode_HintTime->PlayAnimation(0);
		CustMode_SkipTime->PlayAnimation(0);

		CustMode_TickTutor->PlayAnimation(1);
		CustMode_TickSparkles->PlayAnimation(1);
		CustMode_TickHOSparkles->PlayAnimation(1);
		CustMode_TickTextTips->PlayAnimation(1);
		CustMode_TickHints->PlayAnimation(1);
		CustMode_TickSkip->PlayAnimation(1);
		CustMode_TickAwardPopup->PlayAnimation(0);
	}

	PopupAlpha = 0.0f;
	PopupState = MMB_Popup_SelectMode;
	PopupCounter = 0;
}

#ifdef DISABLE_DIFF
void MainMenuControl::AndroidSetMode()
{
	ProfileSystem::Instance->SetProfileMode( 3 );

	ProfileSystem::Instance->ActiveProfile.Clear();
	ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;

	ProfileSystem::Instance->ActiveProfile.IsHintDisabled = false;
	ProfileSystem::Instance->ActiveProfile.IsSkipDisabled = false;
	ProfileSystem::Instance->ActiveProfile.IsExpertMode = false;

	AwardManager::GetInstance()->ExitExpertMode();

	ProfileSystem::Instance->ActiveProfile.CustomHintTimer = 1.0f;

	ProfileSystem::Instance->ActiveProfile.CustomSkipTimer = 1.0f;

	ProfileSystem::Instance->ActiveProfile.IsTutorialDisabled = false;

	ProfileSystem::Instance->ActiveProfile.IsHideSparkles = false;
	ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles = false;

	ProfileSystem::Instance->ActiveProfile.IsHidingTips = false;

	ProfileSystem::Instance->ActiveProfile.IsAwardPopupsEnabled = true;

	ProfileSystem::Instance->SetProfileSlot(iActiveProfileSlot);
	ProfileSystem::Instance->SaveProfile(iActiveProfileSlot);//Save current profile!!
	if( !ProfileSystem::Instance->LoadProfile(iActiveProfileSlot) )
	{
		HandleProfileCorruption();
		IsForcedNameEntry = true;
		ShowEnterYourNamePopup(iActiveProfileSlot);
	}
	else
	{
		ProfileSystem::Instance->WriteProfileSystemData();
	}
	AwardManager::DeleteInstance();

	//LoadProfile() clears the _iActiveProfileSlot
	ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
	//Profile Saved (for first time). Now set EmptyProfilenames to false
	IsProfileNamesEmpty = false;

	ModeMaskSpr->Show(false);
}
#endif

void MainMenuControl::UpdateSelectAdventurePopup()
{
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
			ShowMessageBanner("Back key is Sensed in UpdateSelectAdventurePopup");
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

			MaskSpr->SetAlpha(PopupAlpha);
			SelectAdventurePopupScene->SetSceneAlpha(PopupAlpha);

			isChapterThumbsAnimating = true;
			chapThumbAnimTimer = 0;

			CObject *obj;
			float targetX, targetY, twTime = 1.0f;

			obj = SelectAdventurePopupScene->GetObjectByID("mainchapbtn");
			targetX = obj->Pos.x;
			targetY = obj->Pos.y;
			obj->SetAlpha(0);
			obj->SetXY(938,169);

			float tweenVals[5] = {1.0f, targetX, targetY, 1.0f, 0};
			obj->TweenTo(tweenVals, twTime, 6, true);
			obj->Show(true);

			obj = SelectAdventurePopupScene->GetObjectByID("MAINTXT");
			targetX = obj->Pos.x;
			targetY = obj->Pos.y;
			obj->SetAlpha(0);
			obj->SetXY(938, 169);

			//float tweenVals[5] = { 1.0f, targetX, targetY, 1.0f, 0 };
			obj->TweenTo(tweenVals, twTime, 6, true);
			obj->Show(true);

			obj = SelectAdventurePopupScene->GetObjectByID("bonuschapbtn");
			targetX = obj->Pos.x;
			targetY = obj->Pos.y;
			obj->SetAlpha(0);
			obj->PlayAnimation(1);
			obj->SetXY(401,574);

			float tweenVals2[5] = {1.0f, targetX, targetY, 1.0f, 0};
			obj->TweenTo(tweenVals2, twTime, 6, true);
			obj->Show(true);

			obj = SelectAdventurePopupScene->GetObjectByID("BONUSTXT");
			targetX = obj->Pos.x;
			targetY = obj->Pos.y;
			obj->SetAlpha(0);
			obj->PlayAnimation(1);
			obj->SetXY(401, 574);

			//float tweenVals2[5] = { 1.0f, targetX, targetY, 1.0f, 0 };
			obj->TweenTo(tweenVals2, twTime, 6, true);
			obj->Show(true);

			if (!ProfileSystem::Instance->ActiveProfile.IsBonusChainRemoved)
			{
				obj = SelectAdventurePopupScene->GetObjectByID("bonuslock");
				obj->ForceShow();
				targetX = obj->Pos.x;
				targetY = obj->Pos.y;
				obj->SetAlpha(0);
				obj->SetXY(496,683);

				float tweenVals3[5] = {1.0f, targetX, targetY, 1.0f, 0};
				obj->TweenTo(tweenVals3, twTime, 6, true);
				obj->Show(true);
			}

			obj = NULL;

			Control::Audio->QuickLoadAndPlaySFX(aSFXMnuChapterSlide);
			return;
		}
		MaskSpr->SetAlpha(PopupAlpha);
		SelectAdventurePopupScene->SetSceneAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )
	{
		if(isChapterThumbsAnimating)
		{
			chapThumbAnimTimer += Control::LogicRate;
			if(chapThumbAnimTimer>1.0f)
			{
				isChapterThumbsAnimating = false;
				if (ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted && !ProfileSystem::Instance->ActiveProfile.IsBonusChainRemoved)
				{
					PlayBtnClicked = false;
					SelectAdventurePopupScene->GetObjectByID("bonuslock")->FadeOut();
					ProfileSystem::Instance->ActiveProfile.IsBonusChainRemoved = true;
				}
			}
			return;
		}

		CPoint mosPos = Control::Input->Mpos();
		bool clicked = Control::Input->LBclicked();

		Cursor::SetMode(CA_Normal);

		if(comicBookUnlockBtn->IsActive)
		{
			if (ComicBookUnlockedRect.Intersection(&mosPos))
			{
				Cursor::SetMode(CA_HandPointy);
				if( comicBookUnlockBtn->GetAnimation() != 1 )
				{
					ShowBannerText("Exread");
					comicBookUnlockBtn->PlayAnimation(1);
				}
				if(clicked )
				{					
					comicBookUnlockBtn->PlayAnimation(0);
					//Goto Comic Book
					isComicOpenedFromSelectAdvPopup = true;
					LoadComicBook();

					//Hide all Select adventure Popup and Stuff
					PopupAlpha = 0.0f;	
					PopupCounter=0;
					PopupState = MMB_Popup_Inactive;
					MaskSpr->Show(false);
					SelectAdventurePopupScene->Show(false);
					return;
				}
			}
			else
			{
				comicBookUnlockBtn->PlayAnimation(0);
			}
		}

		if(PolyMainChapter->Intersection(&mosPos))
		{
			Cursor::SetMode(CA_HandPointy);
			if( ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted && !mainBtnGlow->Visible )
			{
			#ifdef _CEBUILD
				ShowBannerText("Excollect");
			#else
				ShowBannerText("Exdlc");
			#endif 
				mainBtnGlow->Show(true);
			}
						
			if(clicked )
			{			
				if ( !ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted )				
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuChapterClick);
					if( !PlayBtnClicked )
					{
						isComicOpenedFromSelectAdvPopup = false;

						PlayBtnClicked = true;
						ClearMenuTrack();
						Control::Input->ClearMouse();
						Cursor::Show(false);//TURN OFF TO AVOID UNWANTED CLICKS TILL NXT SCENE!!
						ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;     
						AwardManager::GetInstance()->StartMainStory();
						SendMessage(ActionProcessed, ID, xQuestMode);
						return;
					}
					bonusBtnGlow->Show(false);
				}
				else if( !isWaitingForMainChapterRevisit )//we chose main chapter, but it's completed!!					
				{					
					//drive to pending level!!
					#ifdef _CEBUILD
						playPendingCollectableLevel();								
					#else
						ShowBannerText("Exdlc");
					#endif										
				}
				
			}
		}
		else
		{
			if(mainBtnGlow->Visible)
				mainBtnGlow->Show(false);
		}

		if(PolyBonusChapter->Intersection(&mosPos))
		{
			
			Cursor::SetMode(CA_HandPointy);
			if( ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted && !bonusBtnGlow->Visible )
			{
				#ifdef _CEBUILD		
					ShowBannerText("Exsee");
				#else //Buy DLC to play bonus chapter
					ShowBannerText("Exbuy");
				#endif	
				bonusBtnGlow->Show(true);
			}
			if(clicked )
			{
				//TODO check for bonus game unlock here
				if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted)
				{
					
			#ifdef _DEMOBUILD
					ShowBannerText("Exfull"); 
			#elif defined( _CEBUILD )
				#ifdef K_ANDROID
					/*
					if (!GFApp->isUnlocked()) //Free Build
						ShowBannerText("Exfull"); 
					else
						ShowBannerText("Exmain");
					 */
				#else
					ShowBannerText("Exmain"); 
				#endif
			#else //SE build
					ShowBannerText("Excollecter"); 
			#endif 

					bonusBtnLock->Shake(150, 5, 0, 0.25);
					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuModeLocked);
					return;
				}
				else
				{
				#ifdef _CEBUILD
					isComicOpenedFromSelectAdvPopup = false;

					PlayBtnClicked = true;
					ClearMenuTrack();
					Control::Input->ClearMouse();
					Cursor::Show(false);//TURN OFF TO AVOID UNWANTED CLICKS TILL NXT SCENE!!
					ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;     

					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
					ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_8; 
					SendMessage(ActionProcessed, ID, xQuestMode);
					return;
				#else //SE build
					ShowBannerText("Exbuy"); 
					bonusBtnLock->Shake(150, 5, 0, 0.25);
					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuModeLocked);
					return;
				#endif
				}

			}
		}
		else
		{
			if(bonusBtnGlow->Visible)
				bonusBtnGlow->Show(false);
		}

		if(!ChapterSelectRect.Intersection(&mosPos))
		{
			if(clicked)
			{
				//close this when clicking outside
				isComicOpenedFromSelectAdvPopup = false;
				PopupCounter = 2;
				return;
			}
		}
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 4.0f * Control::LogicRate;

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			PopupState = MMB_Popup_Inactive;
			MaskSpr->Show(false);

			SelectAdventurePopupScene->Show(false);
			/*SelectAdventurePopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
			SelectAdventurePopupScene->GetObjectByID("BtnCancel")->PlayAnimation(0);*/
		}
		else
		{
			MaskSpr->SetAlpha(PopupAlpha);
			SelectAdventurePopupScene->SetSceneAlpha(PopupAlpha);
		}
	}
}

void MainMenuControl::UpdateExitGamePopup()
{
	
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
		ShowMessageBanner("Back key is Sensed in UpdateExitGamePopup");
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
		ExitGamePopupScene->SetSceneAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )
	{
		CObject *Button = ExitGamePopupScene->GetObjectByID("BtnOk");	
		if( CheckPopupButton(Button) == MMB_Popup_Click )
		{
			SendMessage(ActionProcessed, ID, xExitGame);
			PopupCounter++;
			Cursor::Show(false);//EXITING- we don't need the cursor again

			return;	
		}
		else
		{
			CObject *Button = ExitGamePopupScene->GetObjectByID("BtnCancel");	
			if( CheckPopupButton(Button) == MMB_Popup_Click )
			{
				PopupCounter++;
			}
		}
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 4.0f * Control::LogicRate;
		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			PopupState = MMB_Popup_Inactive;
			MaskSpr->Show(false);
			ExitGamePopupScene->Show(false);
			ExitGamePopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
			ExitGamePopupScene->GetObjectByID("BtnCancel")->PlayAnimation(0);
		}
		else
		{
			MaskSpr->SetAlpha(PopupAlpha);
			ExitGamePopupScene->SetSceneAlpha(PopupAlpha);
		}
	}
}

void MainMenuControl::ShowReviewGamePopup()
{
	/*
	LoveItBtn = ReviewGamePopupScene->GetObjectByID("LoveIt");
	CouldbeBetterBtn = ReviewGamePopupScene->GetObjectByID("CouldbeBetter");	
	LeaveAReviewBtn = ReviewGamePopupScene->GetObjectByID("LeaveAReview");	
	NoThanksBtn = ReviewGamePopupScene->GetObjectByID("NoThanks");	
	SendFeedbackBtn = ReviewGamePopupScene->GetObjectByID("SendFeedback");	
	CommentsBtn = ReviewGamePopupScene->GetObjectByID("Comments");	
	
	EmailBtn = ReviewGamePopupScene->GetObjectByID("Email");	
	
	GPlusBtn = ReviewGamePopupScene->GetObjectByID("GPlus");	
	*/

	FacebookBtn = ReviewGamePopupScene->GetObjectByID("Facebook"); 
	TwitterBtn = ReviewGamePopupScene->GetObjectByID("Twitter");
	LinkedinBtn = ReviewGamePopupScene->GetObjectByID("Linkedin");
	PinterestBtn = ReviewGamePopupScene->GetObjectByID("Pinterest");
	IGBtn = ReviewGamePopupScene->GetObjectByID("Instagram");
	YTBtn = ReviewGamePopupScene->GetObjectByID("YouTube");

	CLOSEBtn = ReviewGamePopupScene->GetObjectByID("CLOSE");

	MaskSpr->SetAlpha(0.0f);
	MaskSpr->Show(true);
    
	ReviewGamePopupScene->Show(true);

	/*
	
	//PAGE 1 SHOWING
	CommentsBtn->PlayAnimation(0);//Page 1
	LoveItBtn->PlayAnimation(0);//Page 1 
	LoveItBtn->Show(true);
	CouldbeBetterBtn->PlayAnimation(0);//Page 1
	CouldbeBetterBtn->Show(true);

	//PAGE2 & PAGE3 Not showing!!
	LeaveAReviewBtn->PlayAnimation(0);//Page 1
	LeaveAReviewBtn->Show(false);
	NoThanksBtn->PlayAnimation(0);//Page 1
	NoThanksBtn->Show(false);
	SendFeedbackBtn->PlayAnimation(0);//Page 1
	SendFeedbackBtn->Show(false);
	*/

	ReviewGamePopupScene->SetSceneAlpha(0.0f);
	PopupAlpha = 0.0f;
	PopupState = MMB_Popup_ReviewGame;
	PopupCounter = 0;	
}

void MainMenuControl::CheckSocialButtons()
{	
	//Turn off share buttons
	if( CheckPopupButton(IGBtn, true) == MMB_Popup_Click )
	{
		/*
		SAVEALLPROFILES();      
   	    if (GFApp->Window) 
		   {

		#ifndef K_IPHONE
			  //iPhones can't have window handlers!!
			  //GFApp->Window->setCallBack (NULL);
		#endif
			  GFApp->Window->setPTKCallBack (NULL);
		   }

		//OPEN DEFAULT EMAIL COMPOSER
		static char szEntry[1024];
		char szMessage[1024];
		char szEnquotedErrMessage[1024];
		KStringFmt (szMessage, 1024, GFApp->getBuiltinString("BUILTIN_BODY_TELLFRIEND"),"");
		KStringFmt (szEntry, 256, GFApp->getBuiltinString("BUILTIN_REPORT_TELLFRIEND"), (const char*)AppTitle_Str.c_str());
		KStringFmt (szEnquotedErrMessage, 1024, "mailto:%s?subject=%s&body=%s",
			"friend@abc.com", szEntry, szMessage);
		GFApp->enquoteMessage (szEnquotedErrMessage, szMessage, 1024);
		
		long nResult = KMiscTools::alertBox ("Darkarta: A Broken Heart's Quest",GFApp->getBuiltinString("BUILTIN_REPORT_TELLFRIEND"), 
                                   "Tell a Friend", GFApp->getBuiltinString("BUILTIN_REPORT_QUIT"));
		if (nResult == 0) 
		{
			K_LOG("Mail:\n\n%s",szMessage);
			KMiscTools::launchURL (szMessage, true);				
		}
		*/


		KMiscTools::launchURL("https://instagram.com/tuttifrutti.in?igshid=mn2jov5rqmft", true);
		//exit(0);
	}
	else if( CheckPopupButton(FacebookBtn, true) == MMB_Popup_Click )
	{
		SAVEALLPROFILES();      
		//handle facebook share
#ifdef ANDROID
#ifdef _DEMOBULD
		KMiscTools::launchURL("https://www.facebook.com/sharer/sharer.php?u=https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq", true);
#else
		KMiscTools::launchURL("https://www.facebook.com/sharer/sharer.php?u=https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq.full", true);
#endif
#endif
#ifdef K_WIN32
		KMiscTools::launchURL("https://www.facebook.com/sharer/sharer.php?u=http%3A//store.steampowered.com/app/601530",true);
#endif
		//exit(0);		
	}
	else if( CheckPopupButton(TwitterBtn, true) == MMB_Popup_Click )
	{
		SAVEALLPROFILES();      
		//handle twitter share
#ifdef ANDROID
#ifdef _DEMOBULD
		KMiscTools::launchURL("https://twitter.com/home?status=https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq", true);
#else
		KMiscTools::launchURL("https://twitter.com/home?status=https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq.full", true);
#endif
#endif
#ifdef K_WIN32
		KMiscTools::launchURL("https://twitter.com/home?status=http%3A//store.steampowered.com/app/601530",true);
#endif
		//exit(0);
	}
	else if( CheckPopupButton(LinkedinBtn, true) == MMB_Popup_Click )
	{
		SAVEALLPROFILES();      
		//handle linkedin share
#ifdef ANDROID
#ifdef _DEMOBULD
		KMiscTools::launchURL("https://www.linkedin.com/shareArticle?mini=true&url=https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq&title=&summary=&source=", true);
#else
		KMiscTools::launchURL("https://www.linkedin.com/shareArticle?mini=true&url=https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq.full&title=&summary=&source=", true);
#endif
#endif
#ifdef K_WIN32
		KMiscTools::launchURL("https://www.linkedin.com/shareArticle?mini=true&url=http%3A//store.steampowered.com/app/601530&title=Loved%20Darkarta%20game,%20please%20check&summary=&source=",true);
#endif
		//exit(0);
	}
	else if( CheckPopupButton(PinterestBtn, true) == MMB_Popup_Click )
	{
		SAVEALLPROFILES();      
		//handle Pinterest share
		KMiscTools::launchURL("https://in.pinterest.com/tuttifruttigames/",true);
		//exit(0);
	}
	else if( CheckPopupButton(YTBtn, true) == MMB_Popup_Click )
	{
		
		SAVEALLPROFILES();      
		KMiscTools::launchURL("https://www.youtube.com/channel/UCOxG6zBows5VN-Xz2yvZNig",true);
		//exit(0);		
	}
	//https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq
}
void MainMenuControl::UpdateReviewGamePopup()
{
#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
		ShowMessageBanner("Back key is Sensed in UpdateReviewGamePopup");
		PopupCounter = 4;
	}
#endif
	if (PopupCounter == 0)
	{
		PopupAlpha += 4.0f * Control::LogicRate;

		if (PopupAlpha >= 1.0f)
		{
			PopupCounter = 1;
			PopupAlpha = 1.0f;
		}
		MaskSpr->SetAlpha(PopupAlpha);
		ReviewGamePopupScene->SetSceneAlpha(PopupAlpha);
	}
	else if (PopupCounter > 0 && PopupCounter < 4)
	{
		if (CheckPopupButton(CLOSEBtn) == MMB_Popup_Click)
		{
			PopupCounter = 4;//PAGE 4
			//QUIT
			//SendMessage(ActionProcessed, ID, xExitGame);
			//Cursor::Show(false);//EXITING- we don't need the cursor again
		}
		CheckSocialButtons();
	}
	else if (PopupCounter == 4)
	{
		PopupAlpha -= 4.0f * Control::LogicRate;
		if (PopupAlpha <= 0.0f)
		{
			PopupAlpha = 0.0f;
			PopupCounter++;
			PopupState = MMB_Popup_Inactive;
			MaskSpr->Show(false);
			ReviewGamePopupScene->Show(false);
		}
		else
		{
			MaskSpr->SetAlpha(PopupAlpha);
			ReviewGamePopupScene->SetSceneAlpha(PopupAlpha);
		}
	}
}

/*
void MainMenuControl::UpdateReviewGamePopup()
{
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
			ShowMessageBanner("Back key is Sensed in UpdateReviewGamePopup");
			PopupCounter = 4;
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
		ReviewGamePopupScene->SetSceneAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )//PAGE 1
	{		
		if( CheckPopupButton(LoveItBtn) == MMB_Popup_Click )
		{
			PopupCounter = 2;//PAGE 2
			CommentsBtn->PlayAnimation(1);//Page 2
			LoveItBtn->Show(false);
			CouldbeBetterBtn->Show(false);
			LeaveAReviewBtn->Show(true);
			NoThanksBtn->Show(true);
		}
		else if( CheckPopupButton(CouldbeBetterBtn) == MMB_Popup_Click )
		{
			PopupCounter = 3;//PAGE 3
			CommentsBtn->PlayAnimation(2);//Page 3
			LoveItBtn->Show(false);
			CouldbeBetterBtn->Show(false);
			SendFeedbackBtn->Show(true);
			NoThanksBtn->Show(true);
		}
		else if( CheckPopupButton(CLOSEBtn) == MMB_Popup_Click )
		{
			PopupCounter = 4;//PAGE 4
			//QUIT
			//SendMessage(ActionProcessed, ID, xExitGame);
			//Cursor::Show(false);//EXITING- we don't need the cursor again
		}
		else
			CheckSocialButtons();
	}
	else if( PopupCounter == 2 )//PAGE 2
	{		
		if( CheckPopupButton(LeaveAReviewBtn) == MMB_Popup_Click )
		{
			#ifdef K_ANDROID
		
				KMiscTools::launchURL("https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq",true);//when launches app will switch back-front buffers!!
		
			#else
			//Load from ini........
			KIniReader *iniReader;
			iniReader = new KIniReader ;
			iniReader->setIni(KMiscTools::makeFilePath("config.ini")) ;	
			char lpszURL[200]; 
			//OPEN REVIEW WEB PAGE 
			if( iniReader->getString("REVIEW","URL",lpszURL,200) )
			{
			//	K_LOG(lpszURL);
				KMiscTools::launchURL(lpszURL,true);//when launches app will switch back-front buffers!!		
			}
			delete iniReader;
			#endif			
			PopupCounter = 4;//PAGE 4
			//QUIT
			//SendMessage(ActionProcessed, ID, xExitGame);
			//Cursor::Show(false);//EXITING- we don't need the cursor again
		}
		else if( CheckPopupButton(NoThanksBtn) == MMB_Popup_Click )
		{
			PopupCounter = 4;//PAGE 4
			//QUIT
			//SendMessage(ActionProcessed, ID, xExitGame);
			//Cursor::Show(false);//EXITING- we don't need the cursor again
		}
		else if( CheckPopupButton(CLOSEBtn) == MMB_Popup_Click )
		{
			PopupCounter = 4;//PAGE 4
			//QUIT
			//SendMessage(ActionProcessed, ID, xExitGame);
			//Cursor::Show(false);//EXITING- we don't need the cursor again
		}
		else
			CheckSocialButtons();
	}
	else if( PopupCounter == 3 )//PAGE 3
	{		
			if( CheckPopupButton(SendFeedbackBtn) == MMB_Popup_Click )
			{
				SAVEALLPROFILES();      
   			if (GFApp->Window) 
			   {
			#ifndef K_IPHONE
				  //iPhones can't have window handlers!!
				  //GFApp->Window->setCallBack (NULL);
			#endif
				  GFApp->Window->setPTKCallBack (NULL);
			   }

			//OPEN DEFAULT EMAIL COMPOSER
			static char szEntry[1024];
			char szMessage[1024];
			char szEnquotedErrMessage[1024];
			KStringFmt (szMessage, 1024, GFApp->getBuiltinString("BUILTIN_BODY_IMPROVEGAME"),"");
			KStringFmt (szEntry, 256, GFApp->getBuiltinString("BUILTIN_REPORT_IMPROVEGAME"), (const char*)AppTitle_Str.c_str());
			KStringFmt (szEnquotedErrMessage, 1024, "mailto:%s?subject=%s&body=%s",
				"support@tuttifrutti.in", szEntry, szMessage);
			GFApp->enquoteMessage (szEnquotedErrMessage, szMessage, 1024);
			long nResult = KMiscTools::alertBox ("Darkarta: A Broken Heart's Quest",GFApp->getBuiltinString("BUILTIN_REPORT_IMPROVEGAME"), 
									   "Report to Developer", GFApp->getBuiltinString("BUILTIN_REPORT_QUIT"));
			if (nResult == 0) 
			{
				K_LOG("Mail:\n\n%s",szMessage);
				KMiscTools::launchURL (szMessage, true);					
			}
			//exit(0);
		}
		else if( CheckPopupButton(NoThanksBtn) == MMB_Popup_Click )
		{
			PopupCounter = 4;//PAGE 4
			//QUIT
			//SendMessage(ActionProcessed, ID, xExitGame);
			//Cursor::Show(false);//EXITING- we don't need the cursor again
		}
		else if( CheckPopupButton(CLOSEBtn) == MMB_Popup_Click )
		{
			PopupCounter = 4;//PAGE 4
			//QUIT
			//SendMessage(ActionProcessed, ID, xExitGame);
			//Cursor::Show(false);//EXITING- we don't need the cursor again
		}
		else
			CheckSocialButtons();
	}
	else if( PopupCounter == 4 )
	{
		PopupAlpha -= 4.0f * Control::LogicRate;
		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			PopupState = MMB_Popup_Inactive;
			MaskSpr->Show(false);
			ReviewGamePopupScene->Show(false);		
		}
		else
		{
			MaskSpr->SetAlpha(PopupAlpha);
			ReviewGamePopupScene->SetSceneAlpha(PopupAlpha);
		}
	}
}
*/

void MainMenuControl::UpdateSelectModePopup()
{
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
		ShowMessageBanner("Back key is Sensed in UpdateSelectModePopup");
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
		ModeMaskSpr->SetAlpha(PopupAlpha);
		SelectModePopupScene->SetSceneAlpha(PopupAlpha);
		ModeTexts_SetAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )
	{
		CPoint mosPos = Control::Input->Mpos();	
		bool clicked = Control::Input->LBclicked();
		CObject *Button = SelectModePopupScene->GetObjectByID("BtnOk");	
		if( CheckPopupButton(Button) == MMB_Popup_Click )
		{
			//Select Mode Confirm

			//for profile popup it will be setup later, from launch the index will be always 0 !! TODO: Naani kore????
			ProfileSystem::Instance->SetProfileMode( SelectedMode );	// setname & make it valid

			if(!IsModeSelectionFromOptions)
            {
				ProfileSystem::Instance->ActiveProfile.Clear();
				ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
			}

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

			if(!IsModeSelectionFromOptions)
			{
				//Expert, advanced modes also saved alongside Profile
				ProfileSystem::Instance->SetProfileSlot(iActiveProfileSlot);
				ProfileSystem::Instance->SaveProfile(iActiveProfileSlot);//Save current profile!!
				if( !ProfileSystem::Instance->LoadProfile(iActiveProfileSlot) )
				{
					HandleProfileCorruption();
					IsForcedNameEntry = true;
					ShowEnterYourNamePopup(iActiveProfileSlot);
				}
				else               
				{
					ProfileSystem::Instance->WriteProfileSystemData();					
				}
					AwardManager::DeleteInstance();

				//LoadProfile() clears the _iActiveProfileSlot
				ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
				//Profile Saved (for first time). Now set EmptyProfilenames to false
				IsProfileNamesEmpty = false;
			}
			else
			{
				//From Options Menu
				OptionsPopupScene->GetObjectByID("DiffModeText")->PlayAnimation(SelectedMode);
				ProfileSystem::Instance->SaveProfile(iActiveProfileSlot);//Save current profile!!
				ProfileSystem::Instance->WriteProfileSystemData();
				
			}
			
			PopupCounter++;
		}
		else
		{	
			bool modeSeletMouseOver = false;
			//Select Mode
			for (int i = 0; i < MAX_GAMEMODES; i++)
			{
				if( ModeRects[i]->Intersection(&mosPos) ) //sree
				{
					if(!modeSeletMouseOver)
					{
						modeSeletMouseOver = true;
						if( ModeButtons[i]->GetAnimation() != 1 && ModeButtons[i]->GetAnimation() != 2 )
							Control::Audio->PlaySample(aSFXRefill3);
					}
					if( SelectedMode == i || clicked )
					{
						ModeButtons[i]->PlayAnimation(2);//show clicked hightlight!!
					}
					else
					{
						ModeButtons[i]->PlayAnimation(1);//show mouseover highlight
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
				else
					modeSeletMouseOver = false;
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
									Control::Audio->PlaySample(aSFXRefill3);
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
										Control::Audio->PlaySample(aSFXRefill3);
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
									Control::Audio->PlaySample(aSFXRefill3);
								Cursor::SetMode(CA_HandPointy);
							}
							if( Control::Input->LBjustdown() )
							{
								IsSkipKnobDragStarted = true;
								Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderClick);
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
										Control::Audio->PlaySample(aSFXRefill3);
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
					#ifndef _DISABLE_EXTRA
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuTickClick);
						IsAwardNotifyEnabled = !IsAwardNotifyEnabled;
						CustMode_TickAwardPopup->PlayAnimation(!IsAwardNotifyEnabled?1:0);
					}
					#endif
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
		PopupAlpha -= 4.0f * Control::LogicRate;

		modeNameText->Show(false);

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			if(IsModeSelectionFromOptions)
			{
				PopupAlpha = 1.0f;
				PopupState = MMB_Popup_Options;
				IsModeSelectionFromOptions = false;
				PopupCounter = 1;
			}
			else
			{
				PopupState = MMB_Popup_Inactive;
				MaskSpr->Show(false);
				MaskSpr2->Show(false);
			}
			ModeMaskSpr->Show(false);
        
			SelectModePopupScene->Show(false);
			ModeTexts_Show(false);
			SelectModePopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
		}
		else
		{
			if(!IsModeSelectionFromOptions)
			{
				MaskSpr->SetAlpha(PopupAlpha);
				MaskSpr2->SetAlpha(PopupAlpha);
			}
            ModeMaskSpr->SetAlpha(PopupAlpha);
			ModeTexts_SetAlpha(PopupAlpha);
			SelectModePopupScene->SetSceneAlpha(PopupAlpha);
		}
	}
}



void MainMenuControl::UpdateOptionsPopup()
{
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
		ShowMessageBanner("Back key is Sensed in UpdateOptionsPopup");
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
		OptionsPopupScene->SetSceneAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )
	{

        UpdateLanguageSelection();
        if(langBG->Visible)
            return;

		CObject *OkButton = OptionsPopupScene->GetObjectByID("BtnOk");
		//CObject *CancelButton = OptionsPopupScene->GetObjectByID("BtnCancel");
		modeBtnOver->SetEnable(false);
		CPoint mousePos = Control::Input->Mpos();

		if( CheckPopupButton(OkButton) == MMB_Popup_Click)// || CheckPopupButton(CancelButton) == MMB_Popup_Click )
		{
			PopupCounter++;
			return;
		}


		//	else if( ModeChangeTextRect.Intersection(&Control::Input->Mpos()))
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
#ifndef DISABLE_DIFF
		else if( ModeChangeTextRect.Intersection(&mousePos))
		{
			modeBtnOver->SetEnable(true);
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				modeBtnOver->SetEnable(false);
				Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
				IsModeSelectionFromOptions = true;
				ShowSelectModePopup();
				return;
			}
		}
#endif // !K_ANDROID
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
							Control::Audio->PlaySample(aSFXRefill3);
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
				CPoint NewMusicKnobPos = Control::Input->Mpos();
				CObject *Knob = OptionsPopupScene->GetObjectByID("musicSlider");
				if(MusicSliderAreaRect.Intersection(&NewMusicKnobPos))
				{
					if( Cursor::GetMode() != CA_HandPointy )
					{
						if( PrevCursorMode != CA_HandPointy )
							Control::Audio->PlaySample(aSFXRefill3);
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
							Control::Audio->PlaySample(aSFXRefill3);
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
							Control::Audio->PlaySample(aSFXRefill3);
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
							Control::Audio->PlaySample(aSFXRefill3);
						Cursor::SetMode(CA_HandPointy);
					}
					if( Control::Input->LBjustdown() )
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderClick);
						Control::Audio->PlaySample(aVOMenuTest);
						IsVoiceKnobDragStarted = true;
					}
					return;
				}
				else
				{
					Knob->PlayAnimation(0);
				}
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
							Control::Audio->PlaySample(aSFXRefill3);
						Cursor::SetMode(CA_HandPointy);
					}
					if( Control::Input->LBdown() == false )
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuSliderMove);
						//Control::Audio->StopSample(aVOMenuTest);
						IsVoiceKnobDragStarted = false;
						Knob->PlayAnimation(0);
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
			//Mute Checkbox
				{
					//Fullscreen checkbox
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
							#ifndef TOUCH_UI //Custom Cursor
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
		PopupAlpha -= 4.0f * Control::LogicRate;

		musicVolTxt->Show(false);
		effectsVolTxt->Show(false);
		voiceVolTxt->Show(false);
		diffModeTxt->Show(false);
		customCurTxt->Show(false);
		fullscreenTxt->Show(false);
		languageTXT->Show(false);

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			PopupState = MMB_Popup_Inactive;
			OptionsPopupScene->Show(false);
			MaskSpr->Show(false);
			MaskSpr2->Show(false);
			OptionsPopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
			//OptionsPopupScene->GetObjectByID("BtnCancel")->PlayAnimation(0);
		}
		else
		{
			MaskSpr->SetAlpha(PopupAlpha);
			OptionsPopupScene->SetSceneAlpha(PopupAlpha);
		}
	}
}



void MainMenuControl::UpdateEnterYourNamePopup()
{
	
	if( PopupCounter == 0 )
	{
		PopupAlpha += 4.0f * Control::LogicRate;

		if( PopupAlpha >= 1.0f )
		{
			PopupCounter = 1;
			PopupAlpha = 1.0f;
	#ifdef K_iOS
            //To calculate width!
			/*
            TFBitmapText = new CBitmapText();
            TFBitmapText->LoadFont(eFont_26);
            TFBitmapText->SetXY(-1000,-1000);
            */
            /* Create iPhone textfield that can be edited with the virtual keyboard */
            GFApp->lpTextField = new KSysiPhoneTextField ();
            GFApp->lpTextField->setBackgroundColor (0.0f, 0.0f, 0.0f, 0.25f);
            GFApp->lpTextField->setTextColor ((233/255.0f),(204/255.0f),(131/255.0f), 0.0f);
            GFApp->lpTextField->setText ("");
            GFApp->lpTextField->setFontSize(25);
            GFApp->lpTextField->setMaxChars(24);//all iiiiiiii case
            GFApp->lpTextField->setTextAlignment(K_IPHONEALIGN_CENTER);
            GFApp->lpTextField->setKeyboardType(K_IPHONEKBD_DEFAULT);	
            //GFApp->lpTextField->setValidationCallback(lpCharValidationCallback);
            GFApp->lpTextField->setPosition (-1000, -500, 240, 50);//position after alignment!!
            GFApp->lpTextField->setFocus();                  
	#endif		
			TextEntry = new TextBox(this);
			#ifdef K_iOS
				TextEntry->SetXY(665,331);//1366x768
			#else
				TextEntry->SetXY(665,356);//1366x768
			#endif
			TextEntry->SetZ(10010);	
			
			//if( !GFApp->Window->hasFocus )
				//GFApp->Window->minimize();

			AddControl(TextEntry);
			TextEntry->Show(true);

	#ifdef K_ANDROID
        GFApp->lpUIContainer = new KUIElement (KUIElement::getRootElement());
        GFApp->lpUIContainer->setName ("ui_container");
        GFApp->lpUIContainer->setPosition (0,0);
        GFApp->lpUIContainer->setSize (GFApp->Instance->DesktopWidth, GFApp->Instance->DesktopWidth);

        GFApp->lpUIEditField = new CAndroidUIEditField(GFApp->lpUIContainer);
        GFApp->lpUIEditField->setPosition (0,-100);
        GFApp->lpUIEditField->setSize (400, 30);
        GFApp->lpUIEditField->setTextFont (GFApp->lpFont22);
        GFApp->lpUIEditField->setTextColor (0.91f, 0.8f, 0.51f, 1);
        GFApp->lpUIEditField->setTextOutlineColor (0, 0, 0, 0.25f);
        GFApp->lpUIEditField->setTextOutlineThickness (1);
        GFApp->lpUIEditField->setCursorColor (0, 0, 0, 0.5f);
        GFApp->lpUIEditField->setText ("");
        GFApp->lpUIEditField->setFocus();
		GFApp->lpUIEditField->setMaxChars(12); // Limiting the Characters name size to 12 letters
    #endif
			
		}
		
		
		if( IsForcedNameEntry )//else we have a 1.0f maskspr2 bkgnd set!!
            MaskSpr2->SetAlpha(PopupAlpha);
		EnterYourNamePopupScene->SetSceneAlpha(PopupAlpha);

		if(IsFromProfilesPopup)
		{
			for(int i=0; i < MAX_PROFILES; i++)
				ProfileText[i]->SetAlpha(1.0f-PopupAlpha);
			ProfilesPopupScene->SetSceneAlpha(1.0f-PopupAlpha);
		}
	}
	else if( PopupCounter == 1 )
	{
		CObject *CancelButton = EnterYourNamePopupScene->GetObjectByID("BtnCancel");
		#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed()) {
				ShowMessageBanner("Back key is Sensed in UpdateEnterYourNamePopup");
				if (TextEntry!= NULL)
					{
						TextEntry->Clear();
						TextEntry->Show(false); 
						#ifdef K_ANDROID
							KSysAndroid::setVirtualKeyboardStatus (false);
							delete GFApp->lpUIEditField;
							GFApp->lpUIEditField = NULL;
							delete GFApp->lpUIContainer;
							GFApp->lpUIContainer = NULL;
						#endif
					}
				if (WelcomePlayerNameText!=NULL)
				{
					WelcomePlayerNameText->SetText(ProfileSystem::Instance->GetCurrentName());		
					WelcomePlayerNameText->SetAlpha(1.0f);
					WelcomePlayerNameText->Show(true);
					PopupCounter = 4; 
				}
				return;
			}
		#endif
		if(!IsForcedNameEntry && CheckPopupButton(CancelButton) == MMB_Popup_Click)
		{
	           
			if (TextEntry!= NULL)
			{
				TextEntry->Clear();
				TextEntry->Show(false);
                #ifdef K_iOS
                        GFApp->IsTextFieldEntered = false;
                        delete GFApp->lpTextField;
                        GFApp->lpTextField = NULL;
                        //delete TFBitmapText; TFBitmapText = NULL;
                #endif
                
				#ifdef K_ANDROID
					KSysAndroid::setVirtualKeyboardStatus (false);
					delete GFApp->lpUIEditField;
					GFApp->lpUIEditField = NULL;
					delete GFApp->lpUIContainer;
					GFApp->lpUIContainer = NULL;
				#endif
			}

			if (WelcomePlayerNameText!=NULL)
			{
				WelcomePlayerNameText->SetText(ProfileSystem::Instance->GetCurrentName());		
				WelcomePlayerNameText->SetAlpha(1.0f);
				WelcomePlayerNameText->Show(true);
				PopupCounter = 4; 
			}
			return;
		}

		CObject *Button = EnterYourNamePopupScene->GetObjectByID("BtnOk");	
		
        std::string TextEntryStr;
        bool Is_iPad = false;
	#ifdef K_iOS
        Is_iPad = true;
        TextEntryStr = GFApp->lpTextField->getText();
        IsTextEnterKeyPressed = GFApp->IsTextFieldEntered;
	#else
		if (TextEntry!= NULL)
		{
			TextEntryStr = TextEntry->String;
    		#ifdef K_ANDROID
				TextEntryStr = GFApp->lpUIEditField->getText();
			#endif
		}
	#endif
        
		bool IsOkButtonClicked = (CheckPopupButton(Button) == MMB_Popup_Click && TextEntryStr.length());
		if( (Is_iPad && IsTextEnterKeyPressed) || (!isTextBoxEnterKeyPressed && IsOkButtonClicked) )
			for( int i = 0; i < MAX_PROFILES; i++ )
				if( i != iActiveProfileSlot )//no need to compare with ourself!!
					if( ProfileSystem::Instance->ProfileName[i] == TextEntryStr )//Name already present
					{
						ShowBannerText(ErrorSameProfileName);
						
						//KMiscTools::messageBox((const char*)L"Invalid profile name",(const char*)L"Name already being in use!");
						//KInput::hidePointer();//since user may see this before window creation - the system cursor will follow to game!!
						Control::Audio->PlaySample(aSFXNotifySoft);
						IsOkButtonClicked = false;//Not okay
                        isTextBoxEnterKeyPressed = false;
                   }

		if( isTextBoxEnterKeyPressed  || IsOkButtonClicked  )
		{
			Control::Input->ClearMouse();
#ifdef K_iOS
            TextEntryStr = GFApp->lpTextField->getText();
#endif
			NewProfileName = TextEntryStr;

			
			//Welcome +
			WelcomePlayerNameText->SetText( NewProfileName);				
			ProfileSystem::Instance->ProfileSlot = iActiveProfileSlot;
			ProfileSystem::Instance->SetProfileName( NewProfileName );	// setname & make it valid

            //Load application resolution settings
			switch( Application::Instance->CurrentResolution.Width )
            {
				case 1920:   // Full HD
                    	 if(Application::Instance->CurrentResolution.Height == 1080 )
							ProfileSystem::Instance->ActiveProfile.Resolution = R1920x1080;
						   else   // Full HD+
							ProfileSystem::Instance->ActiveProfile.Resolution = R1920x1200;
                        break;
				case 2560:   // Q HD
						ProfileSystem::Instance->ActiveProfile.Resolution = R2560x1440;
                        break;
				case 3840:   // 4K
						ProfileSystem::Instance->ActiveProfile.Resolution = R3840x2160;
                        break;
				case 7680:   // 8K
						ProfileSystem::Instance->ActiveProfile.Resolution = R7680x4320;
                        break;
				case 1344: ProfileSystem::Instance->ActiveProfile.Resolution = R1344x756;
                        break;
				case 1600: ProfileSystem::Instance->ActiveProfile.Resolution = R1600x900;
                        break;
                default:
                case 1366: ProfileSystem::Instance->ActiveProfile.Resolution = R1366x768;
                        break;
                case 1360: ProfileSystem::Instance->ActiveProfile.Resolution = R1360x768;
                        break;
                case 1280: if(Application::Instance->CurrentResolution.Height == 768 )
							   ProfileSystem::Instance->ActiveProfile.Resolution = R1280x768;
						   	else if(Application::Instance->CurrentResolution.Height == 800 )
							   ProfileSystem::Instance->ActiveProfile.Resolution = R1280x800;
							else
							   ProfileSystem::Instance->ActiveProfile.Resolution = R1280x720;
                        break;
				case 1152: ProfileSystem::Instance->ActiveProfile.Resolution = R1152x720;
                        break;
                case 1024:
						 if(Application::Instance->CurrentResolution.Height == 768 )
							   ProfileSystem::Instance->ActiveProfile.Resolution = R1024x768;
						   else
							   ProfileSystem::Instance->ActiveProfile.Resolution = R1024x640;
                        break;
                case 800: ProfileSystem::Instance->ActiveProfile.Resolution = R800x600;
                        break;
                case 640: ProfileSystem::Instance->ActiveProfile.Resolution = R640x480;
                        break;
            }
            ProfileSystem::Instance->ActiveProfile.BPP = Application::Instance->CurrentResolution.BPP;
            ProfileSystem::Instance->ActiveProfile.Windowed = Application::Instance->CurrentResolution.CanWindow;
            
            	//ProfileSystem::Instance->ActiveProfile.Clear(); ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true; //clear when user pressed okay!!
			IsSlotCreated[iActiveProfileSlot] = true;

            if (TextEntry!= NULL)
			{
				TextEntry->Clear();
				TextEntry->Show(false);
                
                #ifdef K_iOS
                    GFApp->IsTextFieldEntered = false;
                    delete GFApp->lpTextField;
                    GFApp->lpTextField = NULL;
                    //delete TFBitmapText; TFBitmapText = NULL;
                #endif
                
				#ifdef K_ANDROID
					KSysAndroid::setVirtualKeyboardStatus (false);
					delete GFApp->lpUIEditField;
					GFApp->lpUIEditField = NULL;
					delete GFApp->lpUIContainer;
					GFApp->lpUIContainer = NULL;
				#endif
			}
			
			
			PopupCounter++;

			Cursor::Show(true);//NEED TO ENTERNAME & CLICK ON "OK BUTTON"
			if( IsForcedNameEntry )
				IsForcedNameEntry = false;
				
			
            //Welcome +
            WelcomePlayerNameText->SetText( ProfileSystem::Instance->GetCurrentName());
            WelcomeTextAlpha = 0.0f;
			WelcomePlayerNameText->SetAlpha(WelcomeTextAlpha);
            WelcomePlayerNameText->Show(true);
            
            IsWelcomePlayerFadeIn = true;
			
		}
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 4.0f * Control::LogicRate;
		
		
		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupState = MMB_Popup_Inactive;
			EnterYourNamePopupScene->Show(false);
			EnterYourNamePopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
			EnterYourNamePopupScene->GetObjectByID("BtnCancel")->PlayAnimation(0);
			
#ifndef DISABLE_DIFF
			ShowSelectModePopup();
#else
			AndroidSetMode();
#endif // !K_ANDROID

		}
		else
		{				
			EnterYourNamePopupScene->SetSceneAlpha(PopupAlpha);
		}		
	}
	else if( PopupCounter == 4 )
	{
		//Cancelling the enter name popup
		PopupAlpha -= 4.0f * Control::LogicRate;
		
		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;
			EnterYourNamePopupScene->Show(false);
			EnterYourNamePopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
			EnterYourNamePopupScene->GetObjectByID("BtnCancel")->PlayAnimation(0);
			PopupAlpha = 1.0f;
			PopupState = MMB_Popup_Inactive;

			if(IsFromProfilesPopup)
			{
				for(int i=0; i < MAX_PROFILES; i++)
					ProfileText[i]->SetAlpha(PopupAlpha);
				ProfilesPopupScene->SetSceneAlpha(PopupAlpha);

				PopupState = MMB_Popup_Profiles;
			}
		}
		else
		{				
			MaskSpr2->SetAlpha(PopupAlpha);
			EnterYourNamePopupScene->SetSceneAlpha(PopupAlpha);

			if(IsFromProfilesPopup)
			{
				for(int i=0; i < MAX_PROFILES; i++)
					ProfileText[i]->SetAlpha(1.0f-PopupAlpha);
				ProfilesPopupScene->SetSceneAlpha(1.0f-PopupAlpha);
			}
		}
	}
}


void MainMenuControl::UpdateProfilesPopup()
{
	if( ProfilePopupCounter == 0 )
	{
		ProfilePopupAlpha += 4.0f * Control::LogicRate;

		if( ProfilePopupAlpha >= 1.0f )
		{
			ProfilePopupCounter = 1;
			ProfilePopupAlpha = 1.0f;	
		}

		for(int i=0; i < MAX_PROFILES; ++i)		
		{
			if( ProfileSystem::Instance->IsProfileValid(i) )
			{
				ProfileText[i]->SetAlpha(ProfilePopupAlpha);
				ProfileText[i]->Show(true);
			}
		}

		MaskSpr->SetAlpha(ProfilePopupAlpha);
		ProfilesPopupScene->SetSceneAlpha(ProfilePopupAlpha);
		WelcomePlayerNameText->SetAlpha(1.0f-ProfilePopupAlpha);//1..0
	}
	else if( ProfilePopupCounter == 1 )
	{
		CObject *Button;
		CPoint mosPos = Control::Input->Mpos();
		bool Clicked = Control::Input->LBclicked();
		
		for( int i = 0; i < MAX_PROFILES; i++ )	
		{
			if( ProfileSystem::Instance->IsProfileValid(i) )
			{
				if( i == ProfileSystem::Instance->ProfileSlot )
				{
					if( ProfDeleteButtonRects[i]->Intersection(&mosPos) )	//Delete Profile Button
					{
						ProfDeleteButtons[i]->SetAnimation(1);
						if( Clicked )
						{
							ProfDeleteButtons[i]->SetAnimation(2);
							Control::Audio->QuickLoadAndPlaySFX(aSFXMnuProfNameDelete);
							IsFromProfilesPopup = true;
							ShowEraseProfilePopup(i);
						}
						else if( PrevCursorMode != CA_HandPointy )
							Control::Audio->PlaySample(aSFXRefill3);
						Cursor::SetMode(CA_HandPointy);
					}
					else
						ProfDeleteButtons[i]->SetAnimation(0);
				}
				else if( ProfNameRects[i]->Intersection(&mosPos) )		//Active Profile Names Rectangles
				{
					if( Clicked )
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuProfNameClick);
						//remove active player glow
						ProfNameGlow[ProfileSystem::Instance->ProfileSlot]->Show(false);
						ProfDeleteButtons[ProfileSystem::Instance->ProfileSlot]->Show(false);
						//New player glow
						ProfNameGlow[i]->Show(true);
						ProfDeleteButtons[i]->Show(true);
						SetProfileSlotTextColors(i);										
					}
					else  if( PrevCursorMode != CA_HandPointy )
						Control::Audio->PlaySample(aSFXRefill3);
					Cursor::SetMode(CA_HandPointy);
				}
			}
			else //Click for newplayer
			{
				if( ProfNameRects[i]->Intersection(&mosPos) )
				{
					if( Clicked )
					{
						SelectedMode = 0; //defaults to casual Mode
						Control::Audio->QuickLoadAndPlaySFX(aSFXMnuProfNameClick);
						AwaitingEnterNamePopupSlot = i;ProfilePopupCounter++;
					}
					else  if( PrevCursorMode != CA_HandPointy )
						Control::Audio->PlaySample(aSFXRefill3);
					Cursor::SetMode(CA_HandPointy);
				}
			} 
		}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
			ShowMessageBanner("Back key is Sensed in UpdateProfilesPopup");
				ProfileSystem::Instance->RestoreProfileSystem();//overwrite create/delete
				//Clear off unwanted texts!!
				for( int i = 0; i < MAX_PROFILES; i++ )
					if( ProfileSystem::Instance->IsProfileValid(i) )
						ProfileText[i]->SetText(ProfileSystem::Instance->GetProfileName(i));
					else
						ProfileText[i]->SetText("");
					
				
				//Welcome +
				WelcomePlayerNameText->SetText( ProfileSystem::Instance->GetCurrentName() );	
				ProfileSystem::Instance->WriteProfileSystemData();//handle delete 
				
				ProfilePopupCounter++;
			}
		#endif
		Button = ProfilesPopupScene->GetObjectByID("BtnOk");	
		if( CheckPopupButton(Button) == MMB_Popup_Click )
		{
			for( int i = 0; i < MAX_PROFILES; i++ )
				if( IsSlotCreated[i] || IsSlotDeleted[i] )//It's our active slot!!
				{
					ProfileSystem::Instance->ActiveProfile.Clear();	ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
					ProfileSystem::Instance->SetProfileSlot(i);
					ProfileSystem::Instance->SaveProfile(i);//Save current profile!!
				}
			
			ProfileSystem::Instance->SetProfileSlot(iActiveProfileSlot);
			
			
			//Welcome +
			WelcomePlayerNameText->SetText(ProfileSystem::Instance->GetCurrentName() );	
			if( !ProfileSystem::Instance->LoadProfile(iActiveProfileSlot) )
			{
				HandleProfileCorruption();
                AwaitingEnterNamePopupSlot = iActiveProfileSlot;
                ProfilePopupCounter++;
			}
			else
			{
				AwardManager::DeleteInstance();
				ProfilePopupCounter++;
			}
			ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
		    ProfileSystem::Instance->WriteProfileSystemData();//handle create & delete
			
		}
		else
		{	
			Button = ProfilesPopupScene->GetObjectByID("BtnCancel");	
			if( CheckPopupButton(Button) == MMB_Popup_Click )
			{
				ProfileSystem::Instance->RestoreProfileSystem();//overwrite create/delete
				//Clear off unwanted texts!!
				for( int i = 0; i < MAX_PROFILES; i++ )
					if( ProfileSystem::Instance->IsProfileValid(i) )
						ProfileText[i]->SetText(ProfileSystem::Instance->GetProfileName(i));
					else
						ProfileText[i]->SetText("");
					
				
				//Welcome +
				WelcomePlayerNameText->SetText( ProfileSystem::Instance->GetCurrentName() );	
				ProfileSystem::Instance->WriteProfileSystemData();//handle delete 
				
				ProfilePopupCounter++;
			}
		}
	}
	else if( ProfilePopupCounter == 2 )
	{
		ProfilePopupAlpha -= 4.0f * Control::LogicRate;

		if( ProfilePopupAlpha <= 0.0f )
		{
			ProfileSystem::Instance->DeleteUnwantedSlotFiles();
			ProfilePopupAlpha = 0.0f;	
			ProfilePopupCounter++;
			PopupState = MMB_Popup_Inactive;
			ProfilesPopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
			ProfilesPopupScene->GetObjectByID("BtnCancel")->PlayAnimation(0);

			//Turn off
            if( AwaitingEnterNamePopupSlot == -1 )
                MaskSpr->Show(false);

            
			ProfilesPopupScene->Show(false);			
			for(int i=0; i < MAX_PROFILES; ++i)		
				if( ProfileSystem::Instance->IsProfileValid(i) )
					ProfileText[i]->Show(false);
		
            if( AwaitingEnterNamePopupSlot != -1 )
            {
                ShowEnterYourNamePopup(AwaitingEnterNamePopupSlot);
                AwaitingEnterNamePopupSlot = -1;
                //switch maskspr 1.0f=> maskspr2
                MaskSpr2->SetAlpha(1.0f);
                MaskSpr2->Show(true);
                MaskSpr->SetAlpha(0.0f);
                MaskSpr->Show(false);
            }
            else
            {
                WelcomePlayerNameText->SetAlpha(1.0f-ProfilePopupAlpha);//0..1
            }
		}
		else
		{
			for(int i=0; i < MAX_PROFILES; ++i)		
				if( ProfileSystem::Instance->IsProfileValid(i) )
				{
					ProfileText[i]->SetAlpha(ProfilePopupAlpha);
					ProfileText[i]->Show(true);
				}
			if( AwaitingEnterNamePopupSlot == -1 )
            {
                MaskSpr->SetAlpha(ProfilePopupAlpha);
                WelcomePlayerNameText->SetAlpha(1.0f-ProfilePopupAlpha);//0..1
            }
			ProfilesPopupScene->SetSceneAlpha(ProfilePopupAlpha);
		}
        
	}
}


void MainMenuControl::HandleProfileCorruption()
{
	switch (ProfileSystem::Instance->ActiveProfile.CurrentLevel)
	{
		case Level_1: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_Beach; break;
		case Level_2: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance; break;
		case Level_3: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance; break;
		case Level_4: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance; break;
		case Level_5: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S1_TigerDistant; break;
		case Level_6: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1; break;
		case Level_7: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano; break;
		case Level_8: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace; break;

		default:
		break;
	}

	/*

	GFMainMenuCtrl->ShowBannerText("Excorrupt");
    
    ProfileSystem::Instance->ProfileName[iActiveProfileSlot] = "invalid";
    ProfileSystem::Instance->ProfileValid[iActiveProfileSlot] = false;
    
	#ifdef K_WIN32
		_flushall();
	#endif
		std::string PrevSaveFile = ProfileSystem::Instance->UserPath + MKSTR(iActiveProfileSlot) + ".sav";
		if( KMiscTools::fileExists(PrevSaveFile.c_str()) )
			remove(PrevSaveFile.c_str());//check mac compatibility

	*/
}

void MainMenuControl::UpdateEraseProfilePopup()
{
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
		ShowMessageBanner("Back key is Sensed in UpdateEraseProfilePopup");
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
		MaskSpr2->SetAlpha(PopupAlpha);
		EraseProfilePopupScene->SetSceneAlpha(PopupAlpha);
		EraseProfileNameText->SetAlpha(PopupAlpha);
		if( IsFromProfilesPopup )
		{
			for(int i=0; i < MAX_PROFILES; i++)
				ProfileText[i]->SetAlpha(1.0f-PopupAlpha);
			ProfilesPopupScene->SetSceneAlpha(1.0f-PopupAlpha);
		}
	}
	else if( PopupCounter == 1 )
	{
		CObject *Button = EraseProfilePopupScene->GetObjectByID("BtnOk");	
		if( CheckPopupButton(Button) == MMB_Popup_Click )
		{
			//Delete Current Player
			ProfileSystem::Instance->DeleteSlot(iInactiveProfileSlot);
			ProfileText[iInactiveProfileSlot]->SetText("");
			ProfNameGlow[iInactiveProfileSlot]->Show(false);
			ProfDeleteButtons[iInactiveProfileSlot]->Show(false);
			ProfNewPlayerTexts[iInactiveProfileSlot]->Show(true);
			//Choose Next Player
			int numActivePlayers=0;
			for (int i = 0; i < MAX_PROFILES; i++)
			{	
				if( ProfileSystem::Instance->IsProfileValid(i) )
				{
					numActivePlayers++;
					
					//select next candidate
					SetProfileSlotTextColors(i);
					break;
				}
			}
			if(numActivePlayers==0)
			{
				//No More players left. Show Enter Name Popup
				IsProfileNamesEmpty = true;
				InitProfilesPage();
				IsSlotDeleted[iInactiveProfileSlot] = true;
				PopupCounter++;
			}
			else
			{
				InitProfilesPage();
				IsSlotDeleted[iInactiveProfileSlot] = true;
				PopupCounter++;
			}
		}
		else
		{
			Button = EraseProfilePopupScene->GetObjectByID("BtnCancel");	
			if( CheckPopupButton(Button) == MMB_Popup_Click )
			{
				PopupCounter++;
			}
		}
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 4.0f * Control::LogicRate;

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			PopupState = MMB_Popup_Inactive;
			EraseProfilePopupScene->GetObjectByID("BtnOk")->PlayAnimation(0);
			EraseProfilePopupScene->GetObjectByID("BtnCancel")->PlayAnimation(0);
			MaskSpr2->Show(false);
			EraseProfilePopupScene->Show(false);
			EraseProfileNameText->Show(false);

			if(IsProfileNamesEmpty)
			{
				ProfileSystem::Instance->SetProfileSlot(0);
				ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = ProfileSystem::Instance->ProfileSlot;
				IsForcedNameEntry = true;
				ShowEnterYourNamePopup(0);
			}
		}
		else
		{
			MaskSpr2->SetAlpha(PopupAlpha);
			EraseProfilePopupScene->SetSceneAlpha(PopupAlpha);
			EraseProfileNameText->SetAlpha(PopupAlpha);
		}
		if( IsFromProfilesPopup )
		{
			for(int i=0; i < MAX_PROFILES; i++)
				ProfileText[i]->SetAlpha(1.0f-PopupAlpha);
			ProfilesPopupScene->SetSceneAlpha(1.0f-PopupAlpha);
		}
	}
}

/*
const CPoint WatermarkTextPos(512.0f, 183.0f, 0.0f);
void MainMenuControl::CreateWatermark()
{
	WatermarkText = new CBitmapText();
	WatermarkText->SetZ(1000);
	WatermarkText->SetAlignment(Align_Center);
	WatermarkText->SetPos(&WatermarkTextPos);
	WatermarkText->LoadFont(eFont_26);//LoadFont(eFont_25);
	WatermarkText->SetText("Tuttifrutti Games");
	WatermarkText->SetScale(1.0f);
	WatermarkText->SetColor((233/255.0f),(204/255.0f),(131/255.0f));	
	WatermarkText->Show(false);
}
*/
void MainMenuControl::UpdateCreditsPopup()
{
	/*
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
		ShowMessageBanner("Back key is Sensed in UpdateCreditsPopup");
			PopupCounter = 3;
		}
	#endif
	*/
	if( PopupCounter == 0 )
	{
		PopupAlpha += 4.0f * Control::LogicRate;		
	
		if( PopupAlpha >= 1.0f )
		{
			PopupCounter = 1;
			PopupAlpha = 1.0f;			
		}
		//WatermarkText->SetAlpha(PopupAlpha);
		MaskSpr->SetAlpha(PopupAlpha);
	
		//Credits Popupscene
		//CreditsBase->SetAlpha(PopupAlpha);		
		if( PopupCounter == 1 )
			PopupAlpha = 0.0f;	
	}
	else if( PopupCounter == 1 )
	{
		PopupAlpha += 4.0f * Control::LogicRate;		
		if( PopupAlpha >= 1.0f )
			PopupAlpha = 1.0f;

		//CObject *Button = CreditsPopupScene->GetObjectByID("ok");	

		CPoint mosPos = Control::Input->Mpos();

		//WatermarkText->Show(true);
		/*if( CheckPopupButton(Button) == MMB_Popup_Click )
		{
			PopupCounter++;
		}*/
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 4.0f * Control::LogicRate;		
		if( PopupAlpha <= 0.0f )
		{
			PopupCounter++;
			PopupAlpha = 0.0f;
		}

		//CreditsBase->SetAlpha(0.75f+(0.25f*PopupAlpha));
		//MaskSpr->SetAlpha(0.75f+(0.25f*PopupAlpha));
		if( PopupCounter == 3 )
			PopupAlpha = 1.0f;
	}
	else if( PopupCounter == 3 )
	{
		PopupAlpha -= 4.0f * Control::LogicRate;

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			PopupState = MMB_Popup_Inactive;
			//CreditsPopupScene->GetObjectByID("ok")->PlayAnimation(0);
			MaskSpr->Show(false);
            
			//CreditsPopupScene->Show(false);		
			//WatermarkText->Show(false);		

			if( Control::Audio->IsPlaying(aTrackCredits))
				Control::Audio->StopSample(aTrackCredits);

			if( Control::Audio->IsPlaying(aTrackBonusCredit))
				Control::Audio->StopSample(aTrackBonusCredit);

			Control::Audio->PlaySample(aTrackMenu,true);			
		}
		else
		{
			MaskSpr->SetAlpha(PopupAlpha);
			
			//Credits Popupscene - down together!!
			//CreditsBase->SetAlpha(PopupAlpha);			
			//WatermarkText->SetAlpha(PopupAlpha/2);
		}		
	}
}

void MainMenuControl::ShowCreditsPopup()
{
	MaskSpr->SetAlpha(0.0f);
	MaskSpr->Show(true);

	CreditsPopupScene->Show(true);
	
	PopupAlpha = 0.0f;
	PopupState = MMB_Popup_Credits;
	PopupCounter = 0;	
	
	//Credits Popupscene
	CreditsBase->SetAlpha(0.0f);

	ClearMenuTrack();
	Control::Audio->LoadSample(aTrackCredits,AudioTrack,true);
	Control::Audio->PlaySample(aTrackCredits,true);

	//WatermarkText->SetAlpha(0.0f);
	//WatermarkText->Show(true);
}

void MainMenuControl::ShowEnterYourNamePopup(int Slot)
{
	iActiveProfileSlot = Slot;
	ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = Slot;
    if( IsForcedNameEntry )//else continue with profilepopup set maskspr2(alpha 1.0f)
    {
        MaskSpr2->SetAlpha(0.0f);
        MaskSpr2->Show(true);

		CObject *CancelButton = EnterYourNamePopupScene->GetObjectByID("BtnCancel");
		CancelButton->PlayAnimation(BA_Click);
    }
	else
	{
		CObject *CancelButton = EnterYourNamePopupScene->GetObjectByID("BtnCancel");
		CancelButton->PlayAnimation(BA_Idle);
	}
    
	EnterYourNamePopupScene->Show(true);
	EnterYourNamePopupScene->SetSceneAlpha(0.0f);
		
	PopupAlpha = 0.0f;
	PopupState = MMB_Popup_EnterYourName;
	NewProfileName = "";
	PopupCounter = 0;
	isTextBoxEnterKeyPressed = false;
}

void MainMenuControl::ShowOptionsPopup()
{
	// Sac: Please be sure that the Android art for Options menu is different from other platforms,
	// Android specific Options will not have Fullscreen and Widescreen check options in it.

	MaskSpr->SetAlpha(0.0f);
	MaskSpr->Show(true);
	
	#ifndef TOUCH_UI
		OptionsPopupScene->GetObjectByID("fscreenTick")->PlayAnimation((ProfileSystem::Instance->ActiveProfile.Windowed)?1:0);//fullscreen cb
		OptionsPopupScene->GetObjectByID("custCursorTick")->PlayAnimation(Cursor::IsSystemCursor());//customcursor cb
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

	musicVolTxt->Show(true);
	effectsVolTxt->Show(true);
	voiceVolTxt->Show(true);
	diffModeTxt->Show(true);

#ifndef DISABLE_LANG
	languageTXT->Show(true);
#else
	OptionsPopupScene->GetObjectByID("langSelectBtn")->Show(false);
	OptionsPopupScene->GetObjectByID("langSelectBtnTxt")->Show(false);	
#endif // !DISABLE_LANG

	#ifndef TOUCH_UI
	customCurTxt->Show(true);
	fullscreenTxt->Show(true);
	#endif

	OptionsPopupScene->SetSceneAlpha(0.0f);
	PopupAlpha = 0.0f;
	
	PopupState = MMB_Popup_Options;
	PopupCounter = 0;

#ifdef DISABLE_DIFF
	diffModeTxt->Show(false);
	OptionsPopupScene->GetObjectByID("DiffModeText")->Show(false);
	modeBtnOver->SetEnable(false);
#endif // K_ANDROID
}


void MainMenuControl::HighlightActiveProfile( int Slot , bool f)
{
	
}

void MainMenuControl::InitProfilesPage()
{
	SetProfileSlotTextColors(ProfileSystem::Instance->ProfileSlot);

	for( int i = 0; i < MAX_PROFILES; i++ )	
	{
		ProfNameGlow[i]->Show(false);
		ProfDeleteButtons[i]->Show(false);
		if( ProfileSystem::Instance->IsProfileValid(i) )
		{
			//Slot i is valid. Hide 'New Player' Text & show profile Text
			ProfNewPlayerTexts[i]->Show(false);

			ProfileText[i]->SetText(ProfileSystem::Instance->GetProfileName(i));
			ProfileText[i]->Show(true);	
		}
	}

	//Highlight active one
	ProfNameGlow[ProfileSystem::Instance->ProfileSlot]->Show(true);
	ProfDeleteButtons[ProfileSystem::Instance->ProfileSlot]->Show(true);
}

void MainMenuControl::SetProfileSlotTextColors( int slot )
{
	ProfileSystem::Instance->ProfileSlot = iActiveProfileSlot = slot;
	for(int i=0; i < MAX_PROFILES; ++i)
	{		
		if( i == slot )		
			// ProfileText[i]->SetColor(1.0f, 1.0f, 1.0f); //white old value
			ProfileText[i]->SetColor(0.75f, 0.75f, 0.75f);
		else		
			ProfileText[i]->SetColor(0.63f, 0.53f, 0.76f); //Purple as per what Bijish needed

	//			ProfileText[i]->SetColor(0.75f, 0.75f, 0.75f); //grey old value for unselected color

	}
}


void MainMenuControl::ShowProfilesPopup()
{
	MaskSpr->SetAlpha(0.0f);
	MaskSpr->Show(true);
	ProfilesPopupScene->SetSceneAlpha(0.0f);
	ProfilesPopupScene->Show(true);
	
	ProfilePopupAlpha = 0.0f;
	for(int i=0; i < MAX_PROFILES; ++i)		
		if( ProfileSystem::Instance->IsProfileValid(i) )
			ProfileText[i]->SetAlpha(0.0f);

	InitProfilesPage();

	iActiveProfileSlot = ProfileSystem::Instance->ProfileSlot;
	for( int i = 0; i < MAX_PROFILES; i++ )
	{
		IsSlotCreated[i] = false;
		IsSlotDeleted[i] = false;
	}
	iInactiveProfileSlot = -1;
	PopupState = MMB_Popup_Profiles;

	ProfilePopupCounter = 0;
	IsFromProfilesPopup = false;
	IsProfileNamesEmpty = false;
    AwaitingEnterNamePopupSlot = -1;
	
	ProfileSystem::Instance->SaveProfile(iActiveProfileSlot);//Save from game				
	ProfileSystem::Instance->WriteProfileSystemData();
	
	ProfileSystem::Instance->BackupProfileSystem();
}

void MainMenuControl::ShowEraseProfilePopup(int Slot)
{
	iInactiveProfileSlot = Slot;
	MaskSpr2->SetAlpha(0.0f);
	MaskSpr2->Show(true);
	
	EraseProfilePopupScene->Show(true);
	EraseProfilePopupScene->SetSceneAlpha(0.0f);
	EraseProfileNameText->SetText(ProfileSystem::Instance->GetProfileName(Slot));
	EraseProfileNameText->Show(true);
	EraseProfileNameText->SetAlpha(0.0f);

	PopupAlpha = 0.0f;
	PopupState = MMB_Popup_EraseProfile;
	PopupCounter = 0;	
}


void MainMenuControl::ClearMenuTrack()
{
	if( Control::Audio->IsPlaying(aTrackMenu) )
    {
	    Control::Audio->StopSample(aTrackMenu);
	    if( Control::Audio->PrevTrack1 == aTrackMenu )
		    Control::Audio->PrevTrack1 = Audio_Nil;												  
		else if( Control::Audio->PrevTrack2 == aTrackMenu )
		    Control::Audio->PrevTrack2 = Audio_Nil;
		else if( Control::Audio->PrevTrack3 == aTrackMenu )
		    Control::Audio->PrevTrack3 = Audio_Nil;
		else if( Control::Audio->PrevTrack4 == aTrackMenu )
		    Control::Audio->PrevTrack4 = Audio_Nil;
		else if( Control::Audio->PrevTrack5 == aTrackMenu )
		    Control::Audio->PrevTrack5 = Audio_Nil;
		else if( Control::Audio->PrevTrack6 == aTrackMenu )
		    Control::Audio->PrevTrack6 = Audio_Nil;
		else if( Control::Audio->PrevTrack7 == aTrackMenu )
		    Control::Audio->PrevTrack7 = Audio_Nil;
    }
}

///////////////////////////////////////////////////////////////////////////////
void MainMenuControl::Update()
{	
	// ShowMessageBanner("Main Menu loop update  "+MKSTR(Control::Input->Kdown(K_VK_PRIOR)));
	if( GFApp->SingleUpdate == false )//Menu needs single state update for music transition to be effective
		Application::SetSingleUpdate(true);
	
	UpdateBanner();

	if( GFInstance->GFInfoLive )
		return;

	if(extrasMenuControl->PopupState != Popup_Disabled)
		return;

	if(extrasComicControl->isActive)
		return;
	#if defined (_ENABLE_CURL) && !defined _DISABLE_BUG_REPORT
		if(isCollectDia)
		{
			CollectUserData();
			return;
		}
	#endif
	if( isWaitingForMainChapterRevisit )
	{
		MainChapterRevisitTicks -= Control::LogicRate;
		if( MainChapterRevisitTicks <= 0.0f)
		{
			MainChapterRevisitTicks = 0.0f;
			isWaitingForMainChapterRevisit = false;			
			SendMessage(ActionProcessed, ID, xQuestMode);
		}
		return;
	}

	PrevCursorMode = Cursor::GetMode();
	Cursor::SetMode(CA_Normal);

	if(!GFInstance->IsSlowSystem)
	{
		//only for high end system
		for (int i = 0; i < NUM_FLYING_PAPS; i++)
		{
			if(pappus[i]->pSprite->GetFrame()>30)
			{
				pappus[i]->Pos.y += (papVelY[i]-papAccelY[i])*Control::LogicRate;
				pappus[i]->Pos.x += papVelX[i]*Control::LogicRate;

				if(papAccelY[i]>0)
				{
					papAccelY[i] -= 10.0f*Control::LogicRate;
				}
			}
			if(pappus[i]->IsAnimationFinished())
			{
				papAccelY[i] = 20;
				pappus[i]->ForcePlayAnimation(0);
			}

			if(pappus[i]->Pos.y < -100.0f)
			{
				//in range -5 to -10
				papVelY[i] = -10.0f+float(rand()%5);
				papVelX[i] = -10.0f+float(rand()%20);
				papAccelY[i] = 0;

				if((rand()%5)>3)
					pappus[i]->SetZ(20);
				else
					pappus[i]->SetZ(7);

				pappus[i]->SetScale(0.65f+0.35f*(float(rand()%10)/10.0f));

				pappus[i]->SetAlpha(0.85f+0.15f*(float(rand()%10)/10.0f));		

				//171 to 1195
				pappus[i]->Pos.x = 171.0f+float(rand()%1024);
				pappus[i]->Pos.y = 680.0f+float(rand()%100);

				pappus[i]->SetAlpha(0);
				pappus[i]->FadeIn();
			}
		} 
	}
	

	CPoint mosPos = Control::Input->Mpos();
	// bool clicked = Control::Input->LBclicked(); //Sac: Unused variable, so commented it
#ifdef K_ANDROID
	/*
	if(scn_btnUnlocknow->Collide(&mosPos))
	{
		if(Control::Input->LBclicked())
		{
			GFApp->ShowUnlockStore();
			//GFApp->SignIN();
		}
	}
	if(scn_hintStore->Collide(&mosPos))
	{
		if(Control::Input->LBclicked())
		{
			GFApp->ShowHintStore();
			//GFApp->SignOUT();
		}
	}
	*/
#endif

	if( IsButtonsFadingIn )
	{
		WelcomeTextAlpha2 += 4.0f * Control::LogicRate;
		MenuAlpha += 4.0f*Control::LogicRate;

		if( MenuAlpha >= 1.0f )
			MenuAlpha = 1.0f;
		if(WelcomeTextAlpha2 >= 1.0f)
		{
			WelcomeTextAlpha2 = 1.0f;
			IsButtonsFadingIn = false;
		}

		for(int i=0; i < MMB_Max; ++i)
			Buttons[i]->SetAlpha(MenuAlpha);

		if( !IsWelcomePlayerFadeIn )
			WelcomePlayerNameText->SetAlpha(WelcomeTextAlpha2);
	}
	else if( IsWelcomePlayerFadeIn )
	{
		WelcomeTextAlpha += 4.0f*Control::LogicRate;
		if(WelcomeTextAlpha >= 1.0f)
		{
			WelcomeTextAlpha = 1.0f;
			IsWelcomePlayerFadeIn = false;
		}
		WelcomePlayerNameText->SetAlpha(WelcomeTextAlpha);
	}

	
	//Parallax Scrolls
	mouseTarget.x = (mosPos.x - 683.0f)*0.004f;
	mouseTarget.y = (mosPos.y - 384.0f)*0.002f;

	mouseDiff.x += (mouseTarget.x - mouseDiff.x)*0.06f;
	mouseDiff.y += (mouseTarget.y - mouseDiff.y)*0.04f;
	
	scn_Bg->SetXY(scn_Bg->InitPos.x-mouseDiff.x,0);
	scn_MomGlow->Pos.x = scn_MomGlow->InitPos.x-mouseDiff.x;
	scn_MomRays->Pos.x = scn_MomRays->InitPos.x-mouseDiff.x;
	
	float offset = 2.0f;

	scn_pillarLeft1->Pos.x = scn_pillarLeft1->InitPos.x-mouseDiff.x*offset;
	scn_pillarRight1->Pos.x = scn_pillarRight1->InitPos.x-mouseDiff.x*offset;

	offset = 3.0f;

	scn_pillarLeft2->Pos.x = scn_pillarLeft2->InitPos.x-mouseDiff.x*offset;
	scn_pillarRight2->Pos.x = scn_pillarRight2->InitPos.x-mouseDiff.x*offset;

	scn_gameTitle->Pos.x = scn_gameTitle->InitPos.x-mouseDiff.x*offset;

	offset = 4.5f;
	scn_middlePart->Pos.x = scn_middlePart->InitPos.x-mouseDiff.x*offset;
	scn_btnPlay->Pos.x = scn_btnPlay->InitPos.x-mouseDiff.x*offset;
	scn_btnOptions->Pos.x = scn_btnOptions->InitPos.x-mouseDiff.x*offset;
	scn_btnExit->Pos.x = scn_btnExit->InitPos.x-mouseDiff.x*offset;
	scn_btnProfile->Pos.x = scn_btnProfile->InitPos.x-mouseDiff.x*offset;
#ifdef _DEMOBUILD
	scn_btnUnlocknow->Pos.x = scn_btnUnlocknow->InitPos.x - mouseDiff.x * offset;
#endif

	profileTxtPos.x = profileTxtInitPos.x - mouseDiff.x*offset;
	WelcomePlayerNameText->SetPos(&profileTxtPos);

	offset = 6.0f;
	scn_fgStoneLeft->Pos.x = scn_fgStoneLeft->InitPos.x-mouseDiff.x*offset;
	scn_fgStoneRight->Pos.x = scn_fgStoneRight->InitPos.x-mouseDiff.x*offset;
	SIGN_UP_BTN->Pos.x = SIGN_UP_BTN->InitPos.x - mouseDiff.x * offset;

	fireLPos = fireLInitPos - mouseDiff.x*offset;
	fireRPos = fireRInitPos - mouseDiff.x*offset;

	offset = 6.0f;// 7.0f;
	scn_extrasStone->Pos.x = scn_extrasStone->InitPos.x-mouseDiff.x*offset;
	scn_btnExtras->Pos.x = scn_btnExtras->InitPos.x-mouseDiff.x*offset;
	scn_btnCredits->Pos.x = scn_btnCredits->InitPos.x-mouseDiff.x*offset;

	offset = 7.0f;
	scn_extrasStone->Pos.x = scn_extrasStone->InitPos.x - mouseDiff.x * offset;
	SHARE_BTN->Pos.x = SHARE_BTN->InitPos.x - mouseDiff.x * offset;
	LIKE_BTN->Pos.x = LIKE_BTN->InitPos.x - mouseDiff.x * offset;

	#ifdef K_ANDROID
	/*
		if(!GFApp->isUnlocked())
			{
				// if false, then it is full version, if not it is free
				scn_btnUnlocknow->Show(true);
				//scn_freeversion->Show(true);
				//scn_freeversion->Pos.x = scn_freeversion->InitPos.x-mouseDiff.x*offset;
				//scn_btnUnlocknow->Pos.x = scn_btnUnlocknow->InitPos.x-mouseDiff.x*offset;
				
			}
		else
			{
				scn_btnUnlocknow->Show(false);
				//scn_freeversion->Show(false);
			}
			*/
	#endif

	offset = 8.0f;
	scn_groundStone->Pos.x = scn_groundStone->InitPos.x-mouseDiff.x*offset;
	scn_crystalGlow->Pos.x = scn_crystalGlow->InitPos.x-mouseDiff.x*offset;
	VFXCrystalFg->Pos.x = VFXCrystalFg->InitPos.x-mouseDiff.x*offset;
	//end of parallax

		if( PopupState != MMB_Popup_Inactive )
		{
			//Turn off click higlightings in parent buttons after a frame!!
			for(int i=0; i < MMB_Max; ++i)
			{
				if( Buttons[i]->GetAnimation() != BA_Idle )
				{
					/*
                    #ifdef TOUCH_UI
				    if(i == MMB_Play)
				    {
                        Buttons[i]->PlayAnimation(BA_Hover);
                    }
				    else
                    #endif
					*/
				    {
                        Buttons[i]->PlayAnimation(BA_Idle);
                    }
				}
			}

			switch( PopupState )
			{
                default:break;
				case MMB_Popup_Credits: UpdateCreditsPopup();
					break;
				case MMB_Popup_ReviewGame: UpdateReviewGamePopup();
					break;
				case MMB_Popup_ExitGame: UpdateExitGamePopup();
					break;
				case MMB_Popup_SelectAdventure: UpdateSelectAdventurePopup();
					break;
				case MMB_Popup_SelectMode: UpdateSelectModePopup();
					break;
				case MMB_Popup_EraseProfile: UpdateEraseProfilePopup();
					if( IsFromProfilesPopup && PopupState == MMB_Popup_Inactive )
					{
						IsFromProfilesPopup = false;
						PopupState = MMB_Popup_Profiles;
					}
					break;
				case MMB_Popup_Profiles: UpdateProfilesPopup();
					break;
				case MMB_Popup_EnterYourName: UpdateEnterYourNamePopup();
					break;
				case MMB_Popup_Options: UpdateOptionsPopup();
					break;
			}		
		}
		else
		{
		
			#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed()) {
				ShowMessageBanner("Back key is Sensed in Update Main Menu loop");
				
					{
					#ifdef _STEAM_BUILD
						if( ProfileSystem::Instance->ActiveProfile.CurrentLevel >= Level_1 )
							ShowReviewGamePopup();										                                                  
						else
							ShowExitGamePopup();										                                                  
					#else
						ShowExitGamePopup();										                                                  
					#endif
					}
				}
			#endif
            
	
			bool isAnyButtonMouseover = false;
			for(int i=(MMB_Max-1); i >= 0; i--)
			{
                #ifdef  K_iOS
                    if( i == MMB_Exit )
                        continue;
                #endif
                
				if( Buttons[i]->GetAnimation() != BA_Click )
				{
					if( ButtonRects[i].Intersection(&mosPos) )
					{
						isAnyButtonMouseover = true;

						Cursor::SetMode(CA_HandPointy);
						VFXButton->SetPos(&Buttons[i]->Pos);
						if(!VFXButton->Active)
						{
							VFXButton->Start();
							VFXButton->AdvanceByTime(1.0f);
						}
						if( Control::Input->LBclicked())
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);

							switch( i )
							{
							#ifdef K_ANDROID
								/*
								case MMB_Unlocknow:  Buttons[i]->PlayAnimation( BA_Click );
									GFApp->initiateUnlockFToF(); // Initiates IAB purchase
									return;
								 */
							#endif
							case MMB_Play:
								Buttons[i]->PlayAnimation( BA_Click );

#ifdef K_ANDROID
								/*
									if(GFApp->noOfHints > ProfileSystem::Instance->NoOfHints )
									{
											GFApp->noOfHints = ProfileSystem::Instance->NoOfHints;
											GFApp->UpdateUserHint(to_string(ProfileSystem::Instance->NoOfHints));
									}
									if(GFApp->noOfHints == -777)
									{
										GFApp->noOfHints = ProfileSystem::Instance->NoOfHints;
									}
									*/
#endif
#ifdef _DEMOBUILD
								if(ProfileSystem::Instance->ActiveProfile.CurrentArea >= 22)
                                {
                                    ProfileSystem::Instance->ActiveProfile.CurrentArea = 17;
                                    ProfileSystem::Instance->ActiveProfile.CurrentLevel = 0;
                                }
#endif

								if (ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted)
								{
									//K_LOG("IsMainChapterCompleted");
									if (ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed)
									{		
										 if( !isWaitingForMainChapterRevisit )
										 {
											#ifdef _CEBUILD
												playPendingCollectableLevel();																						
											#else
												ShowMessageBanner("Buy DLC for 'Sophia Memory Collectables', 'Eagle Morphing Objects' and lot more...");
											#endif																						
										 }
									}
									else if(!ProfileSystem::Instance->ActiveProfile.L8_S1_IntroVidPlayed)
									{
										ShowSelectAdventureGamePopup();
									}
									else
									{
										ClearMenuTrack();
										Control::Input->ClearMouse();
										ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;
										Cursor::Show(false);//TURN OFF TO AVOID UNWANTED CLICKS TILL NXT SCENE!!
										if((ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_ExtrasCutscenePlayer) || 
											(ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_ExtrasRoom) || 
											(ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_AwardsRoom))
											{
												ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea;
											}
										else if(ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_KidsRoom)
											ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea;
										else if(ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_Credits)
											ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.CreditsPrevArea;
										SendMessage(ActionProcessed, ID, xQuestMode);
									}
								}
								else if(ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed)
								{
									//K_LOG("IsStartCutscenePlayed");
									if( !PlayBtnClicked )
									{
										PlayBtnClicked = true;
										ClearMenuTrack();
										Control::Input->ClearMouse();
										ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;
										Cursor::Show(false);//TURN OFF TO AVOID UNWANTED CLICKS TILL NXT SCENE!!
										if((ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_ExtrasCutscenePlayer) || 
											(ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_ExtrasRoom) || 
											(ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_AwardsRoom))
											{
												ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea;		
											}
										else if(ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_KidsRoom)
											ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea;
										else if(ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_Credits)
											ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.CreditsPrevArea;
										SendMessage(ActionProcessed, ID, xQuestMode);
									}
								}
								else
								{
									//K_LOG("ShowSelectAdventureGamePopup");
									//Not played yet. Show Adventure popup
									ShowSelectAdventureGamePopup();
								}
								return;
							case MMB_Options:Buttons[i]->PlayAnimation( BA_Click );
										ShowOptionsPopup();										  
										return;
							case MMB_Exit:  Buttons[i]->PlayAnimation( BA_Click );
								#ifdef _STEAM_BUILD
										if( ProfileSystem::Instance->ActiveProfile.CurrentLevel >= Level_1 )
											ShowReviewGamePopup();										                                                  
										else
											ShowExitGamePopup();										                                                  
								#else
									ShowExitGamePopup();										                                                  
								#endif
									return;
							case MMB_Extras:
									Buttons[i]->PlayAnimation( BA_Click );
									extrasMenuControl->ShowExtrasPopup();	
									return;
							case MMB_Credits:
								ProfileSystem::Instance->ActiveProfile.IsCreditsFromMainMenu = true;
								ProfileSystem::Instance->ActiveProfile.CreditsPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
								ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_Credits;
								SendMessage(ActionProcessed, ID, xQuestMode);
								return;

							}					
						}
						else
						{
							if( Buttons[i]->GetAnimation() != BA_Hover )
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXMnuMouseOver);
								//Control::Audio->PlaySample(aSFXRefill3);				
								Buttons[i]->PlayAnimation(BA_Hover);
							}
						}

					}
					else
					{
					/*
                        #ifdef TOUCH_UI
                        if(i == MMB_Play)
                        {
                            Buttons[i]->PlayAnimation(BA_Hover);
                        }
                        else
                        #endif
						*/
                        {
                            Buttons[i]->PlayAnimation(BA_Idle);
                        }
					}
				}
			}

			//New Share & Like buttons
			//Share script
			if (SHARE_BTN->IsIntersect(mosPos))
			{
				SHARE_BTN->PlayAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				if (Control::Input->LBclicked())
				{
					ShowReviewGamePopup();
				}
			}
			else
			{
				SHARE_BTN->PlayAnimation(0);
			}
			//Sign-up script
			if (SIGN_UP_BTN->IsIntersect(mosPos))
			{
				SIGN_UP_BTN->PlayAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				if (Control::Input->LBclicked())
				{
					KMiscTools::launchURL("mailto:info@tuttifrutti.in?subject=Suscribe to newsletter");
				}
			}
			else
			{
				SIGN_UP_BTN->PlayAnimation(0);
			}
			//Like script
			if (LIKE_BTN->IsIntersect(mosPos))
			{
				LIKE_BTN->PlayAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				if (Control::Input->LBclicked())
				{
#ifdef ANDROID
#ifdef _DEMOBUILD
					KMiscTools::launchURL("https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq", true);
#else
					KMiscTools::launchURL("https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq.full", true);
#endif // _DEMOBULD
#endif

#ifdef K_WIN32
					//Load from ini........
					KIniReader* iniReader;
					iniReader = new KIniReader;
					iniReader->setIni(KMiscTools::makeFilePath("config.ini"));
					char lpszURL[200];
					//OPEN REVIEW WEB PAGE 
					if (iniReader->getString("REVIEW", "URL", lpszURL, 200))
					{
						//	K_LOG(lpszURL);
						KMiscTools::launchURL(lpszURL, true);//when launches app will switch back-front buffers!!		
					}
					delete iniReader;
#endif
				}
			}
			else
			{
				LIKE_BTN->PlayAnimation(0);
			}
			//Unlock
#ifdef _DEMOBUILD
			if (scn_btnUnlocknow->IsIntersect(mosPos))
			{
				scn_btnUnlocknow->PlayAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				if (Control::Input->LBclicked())
				{
#ifdef ANDROID
					KMiscTools::launchURL("https://play.google.com/store/apps/details?id=in.tuttifrutti.darkarta.abhq.full", true);
#endif
				}
			}
			else
			{
				scn_btnUnlocknow->PlayAnimation(0);
			}
#endif // _DEMOBUILD
			if( WelcomeRect.Intersection(&mosPos) )
			{
				isAnyButtonMouseover = true;

				Cursor::SetMode(CA_HandPointy);
				ProfileButton->PlayAnimation(1);

				VFXButton->SetPos(&ProfileButton->Pos);
				if(!VFXButton->Active)
				{
					VFXButton->Start();
					VFXButton->AdvanceByTime(1.0f);
				}

				if( Control::Input->LBclicked() )
				{
					ProfileButton->PlayAnimation(0);
					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
					ShowProfilesPopup();
					return;
				}
				else if( !IsMouseOverWelcomeText )
				{
					IsMouseOverWelcomeText = true;
					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuMouseOver);
				}
			}
			else
			{
				ProfileButton->PlayAnimation(0);
				IsMouseOverWelcomeText = false;
			}

			if(!isAnyButtonMouseover && VFXButton->Active)
			{
				VFXButton->StopEmit();				
			}
		}
	#ifdef _CHEATS
	{
		if( Control::Input->Kdown(K_VK_L_SHIFT) && Control::Input->Kdown(K_VK_E))
		{
			//Check Puzzle/HO Extra Gameplay
			ProfileSystem::Instance->setForExtrasGamePlay();

			ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_2;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_PZMango;
			SendMessage(ActionProcessed, ID, xQuestMode);
			return;
		}
		else if( Control::Input->Kdown(K_VK_L_CONTROL) && Control::Input->Kdown(K_VK_E))
		{
			//Load and check Puzzle/HO Gameplay if any changes occur
			ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_1;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_PZShelf;

			//ProfileSystem::Instance->setForExtrasGamePlay();
			SendMessage(ActionProcessed, ID, xQuestMode);
			return;
		}
        LOGI("Cheats 1");
		#if ( !defined  K_iOS) || ( !defined  K_ANDROID)
        LOGI("Cheats 2");
			if( Control::Input->Kdown(K_VK_L_SHIFT) )
        #endif
            LOGI("Cheats 3");
		{
			 CheatLevel = Level_Nil;

		#if ( defined  K_iOS) || ( defined  K_ANDROID)
            LOGI("Cheats 4");

			#ifndef _RELEASEBUILD
            LOGI("Cheats 5");
				CheatLevel = Level_NIL;
            #endif
            LOGI("Cheats 6");
            LOGI("Enter Cheats");

            if(NativeCheatLevel!=Level_Nil)
            {
                CheatLevel = NativeCheatLevel;
                LOGI("Cheat Level set to native cheat level");
            }
//            {
//                if( Input->Kpressed(static_cast<EKeyboardLayout>(AKEYCODE_8)) )
//                {
//                    LOGI("Set CheatLevel to Level_8");
//                    CheatLevel = Level_8;
//                }
//                if( Input->Kpressed(static_cast<EKeyboardLayout>(AKEYCODE_7)) )
//                {
//                    LOGI("Set CheatLevel to Level_7");
//                    CheatLevel = Level_7;
//                }
//                else if( Input->Kpressed(static_cast<EKeyboardLayout>(AKEYCODE_6)) )
//                {
//                    LOGI("Set CheatLevel to Level_6");
//                    CheatLevel = Level_6;
//                }
//                else if( Input->Kpressed(static_cast<EKeyboardLayout>(AKEYCODE_5)) )
//                {
//                    LOGI("Set CheatLevel to Level_5");
//                    CheatLevel = Level_5;
//                }
//                else if( Input->Kpressed(static_cast<EKeyboardLayout>(AKEYCODE_4)) )
//                {
//                    LOGI("Set CheatLevel to Level_4");
//                    CheatLevel = Level_4;
//                }
//                else if( Input->Kpressed(static_cast<EKeyboardLayout>(AKEYCODE_3)) )
//                {
//                    LOGI("Set CheatLevel to Level_3");
//                    CheatLevel = Level_3;
//                }
//                else if( Input->Kpressed(static_cast<EKeyboardLayout>(AKEYCODE_2)) )
//                {
//                    LOGI("Set CheatLevel to Level_2");
//                    CheatLevel = Level_2;
//                }
//                else if( Input->Kpressed(static_cast<EKeyboardLayout>(AKEYCODE_1)) )
//                {
//                    LOGI("Set CheatLevel to Level_1");
//                    CheatLevel = Level_1;
//                }
//                else if(Input->Kdown(K_VK_8))
//                {
//                    Application::ToggleFullscreen();
//                }
//            }
        #else

			#ifdef K_WIN32
//				{
//					if( Control::Input->Kdown(K_VK_F8) )
//						CheatLevel = Level_8;
//					if( Control::Input->Kdown(K_VK_F7) )
//						CheatLevel = Level_7;
//					else if( Control::Input->Kdown(K_VK_F6) )
//						CheatLevel = Level_6;
//					else if( Control::Input->Kdown(K_VK_F5) )
//						CheatLevel = Level_5;
//					else if( Control::Input->Kdown(K_VK_F4) )
//						CheatLevel = Level_4;
//					else if( Control::Input->Kdown(K_VK_F3) )
//						CheatLevel = Level_3;
//					else if( Control::Input->Kdown(K_VK_F2) )
//						CheatLevel = Level_2;
//					else if( Control::Input->Kdown(K_VK_F1) )
//                        CheatLevel = Level_1;
//					else if(Control::Input->Kpressed(K_VK_F10))
//						Application::ToggleFullscreen();
//
//				}

            #else
            LOGI("Enter in Cheats");
//				{
//					if( Control::Input->Kdown(K_VK_8) )
//						CheatLevel = Level_8;
//					if( Control::Input->Kdown(K_VK_7) )
//						CheatLevel = Level_7;
//					else if( Control::Input->Kdown(K_VK_6) )
//						CheatLevel = Level_6;
//					else if( Control::Input->Kdown(K_VK_5) )
//						CheatLevel = Level_5;
//					else if( Control::Input->Kdown(K_VK_4) )
//						CheatLevel = Level_4;
//					else if( Control::Input->Kdown(K_VK_3) )
//						CheatLevel = Level_3;
//					else if( Control::Input->Kdown(K_VK_2) )
//						CheatLevel = Level_2;
//					else if( Control::Input->Kdown(K_VK_1) )
//						CheatLevel = Level_1;
//					else if(Control::Input->Kpressed(K_VK_8))
//						Application::ToggleFullscreen();
//				}
            }


			#endif
        #endif
			switch( CheatLevel )	
			{
        #define  _DEMOBUILD
		#ifndef _DEMOBUILD
				case Level_8 :
				{
						ProfileSystem::Instance->ActiveProfile.Clear();	
						ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
						ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
						ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
						ClearMenuTrack();					
						for( int i = PAGE_1; i < MAX_JRN_PAGES; i++ )
							ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[i] = i;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastRead = PAGE_1;
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected = 16+9+6+4+9+12+10+6;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = PAGE_16+9+6+4+9+12+10+6;
						ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead = PAGE_16+9+6+4+9+12+10+6;				
						ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;
						ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;

						ProfileSystem::Instance->ActiveProfile.TutorialState = -1; //disable tutorial
						ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked = true;			

						//ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameCompleted = true;
						ProfileSystem::Instance->ActiveProfile.CurrentArea =  eArea_L8_S1_ShukrasPlace;
						ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_8;
						SendMessage(ActionProcessed, ID, xQuestMode);
					}
					break;
				case Level_7 :
					{
						ProfileSystem::Instance->ActiveProfile.Clear();	
						ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
						ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
						ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
						ClearMenuTrack();					
						for( int i = PAGE_1; i < MAX_JRN_PAGES; i++ )
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[i] = i;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastRead = PAGE_1;
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected = 16+9+6+4+9+12+10;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = PAGE_16+9+6+4+9+12+10;
						ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead = PAGE_16+9+6+4+9+12+10;				
						ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;
						ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;

						ProfileSystem::Instance->ActiveProfile.TutorialState = -1; //disable tutorial

						ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked = true;

						ProfileSystem::Instance->ActiveProfile.CurrentArea =  eArea_L7_S1_Volcano;
						ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_7;
						SendMessage(ActionProcessed, ID, xQuestMode);
					}
					break;
				case Level_6:
					{
						ProfileSystem::Instance->ActiveProfile.Clear();	
						ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
						ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
						ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
						ClearMenuTrack();					
						for( int i = PAGE_1; i < MAX_JRN_PAGES; i++ )
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[i] = i;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastRead = PAGE_1;
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected = 16+9+6+4+9+12;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = PAGE_16+9+6+4+9+12;
						ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead = PAGE_16+9+6+4+9+12;				
						ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;
						ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;

						ProfileSystem::Instance->ActiveProfile.TutorialState = -1; //disable tutorial

						ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked = true;

						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1;					
						ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_6;
						SendMessage(ActionProcessed, ID, xQuestMode);
					}
					break;
				case Level_5:
					{
						ProfileSystem::Instance->ActiveProfile.Clear();	
						ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
						ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
						ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
						ClearMenuTrack();					
						for( int i = PAGE_1; i < MAX_JRN_PAGES; i++ )
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[i] = i;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastRead = PAGE_1;
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected = 16+9+6+4+9;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = PAGE_16+9+6+4+9;
						ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead = PAGE_16+9+6+4+9;				
						ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;
						ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;

						ProfileSystem::Instance->ActiveProfile.TutorialState = -1; //disable tutorial

						ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked = true;

						/*
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S1_BowArrowFixed] = true;
						ProfileSystem::Instance->ActiveProfile.InvObjects[0].InvId = eInv_L5_S1_BowArrowFixed;
						ProfileSystem::Instance->ActiveProfile.InvObjects[0].nPiecePicked = 1;
						ProfileSystem::Instance->ActiveProfile.InvObjects[0].MaxPieces = 1;					
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S6_Locket] = true;
						ProfileSystem::Instance->ActiveProfile.InvObjects[1].InvId = eInv_L5_S6_Locket;
						ProfileSystem::Instance->ActiveProfile.InvObjects[1].nPiecePicked = 1;
						ProfileSystem::Instance->ActiveProfile.InvObjects[1].MaxPieces = 1;
						*/

						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S1_TigerDistant;
						ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_5;
						SendMessage(ActionProcessed, ID, xQuestMode);
					}
					break;
				case Level_4:
					{
						ProfileSystem::Instance->ActiveProfile.Clear();	
						ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
						ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
						ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
						ClearMenuTrack();					
						for( int i = PAGE_1; i < MAX_JRN_PAGES; i++ )
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[i] = i;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastRead = PAGE_1;
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected = 16+9+6+4;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = PAGE_16+9+6+4;
						ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead = PAGE_16+9+6+4;				
						ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;
						ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;

						ProfileSystem::Instance->ActiveProfile.TutorialState = -1; //disable tutorial

						ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked = true;

						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance;
						ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_4;
						SendMessage(ActionProcessed, ID, xQuestMode);
					}
					break;
				case Level_3:
					{
						ProfileSystem::Instance->ActiveProfile.Clear();	
						ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
						ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
						ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
						ClearMenuTrack();					
						for( int i = PAGE_1; i < MAX_JRN_PAGES; i++ )
							ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[i] = i;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastRead = PAGE_1;
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected = 16+9+6;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = PAGE_16+9+6;
						ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead = PAGE_16+9+6;				
						ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;
						ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;

						ProfileSystem::Instance->ActiveProfile.TutorialState = -1; //disable tutorial

						ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked = true;
						
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
						ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_3;

						SendMessage(ActionProcessed, ID, xQuestMode);
					}
					break;
				case Level_2 :
					{
						////////////////////////////
						ProfileSystem::Instance->ActiveProfile.Clear();
						ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
						ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
						ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
						ClearMenuTrack();					
						for( int i = PAGE_1; i < MAX_JRN_PAGES; i++ )
							ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[i] = i;
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected = 16+9;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastRead = PAGE_1;
						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = PAGE_16+9;
						ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead = PAGE_16+9;
						ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;
						ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;

						ProfileSystem::Instance->ActiveProfile.TutorialState = -1; //disable tutorial

						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
						ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_2;

						ProfileSystem::Instance->ActiveProfile.L1S7_IsBoatStarted = true;

						SendMessage(ActionProcessed, ID, xQuestMode);
					}
					break;  
		#endif // !_DEMOBUILD
			case Level_1:
				{

					////////////////////////////
					ProfileSystem::Instance->ActiveProfile.Clear();
					ProfileSystem::Instance->ActiveProfile.IsHaveYouCutscenePlayed = true;
					ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot = iActiveProfileSlot;
					ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
					ClearMenuTrack();					
					for( int i = PAGE_1; i < MAX_JRN_PAGES; i++ )
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[i] = i;
					ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected = 16;
					ProfileSystem::Instance->ActiveProfile.JrnPageLastRead = PAGE_1;
					ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = PAGE_16;
					ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead = PAGE_16;
					ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;
					ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;


					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_Beach;
					ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_1;
					SendMessage(ActionProcessed, ID, xQuestMode);
				}	
				break;							 
			}//switch
		}//shift 
	}
	#endif
	#if defined (_ENABLE_CURL) && !defined _DISABLE_BUG_REPORT
		CollectUserData();	
	#endif
}

void MainMenuControl::ProcessMessage(sMessage message)
{
	/*if( message.Protocol == ControlExit )
		IsTextEnterKeyPressed = true;	*/
}


int MainMenuControl::CheckPopupButton(CObject *Button)
{
	if( Button->GetAnimation() == BA_Locked )
		return MMB_Popup_NoInteraction;

	CPoint mosPos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	if(Button->Collide(&mosPos))
	{
		Cursor::SetMode(CA_HandPointy);
		if(clicked )
		{
			Button->PlayAnimation(BA_Click);
			Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
			return MMB_Popup_Click;
		}
		else
		{
			if( Button->GetAnimation() != BA_Hover )
			{
				Control::Audio->PlaySample(aSFXRefill3);				
				Button->PlayAnimation(BA_Hover);
			}
			return MMB_Popup_MouseOver;
		}
	}
	else
		Button->PlayAnimation( BA_Idle );

	return MMB_Popup_NoInteraction;
}

int MainMenuControl::CheckPopupButton(CObject* Button, bool scale)
{
	if (Button->GetAnimation() == BA_Locked)
		return MMB_Popup_NoInteraction;

	CPoint mosPos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	if (Button->Collide(&mosPos))
	{
		Cursor::SetMode(CA_HandPointy);
		if (clicked)
		{
			Button->PlayAnimation(BA_Click);
			Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
			return MMB_Popup_Click;
}
		else
		{
			if (Button->GetAnimation() != BA_Hover)
			{
				Control::Audio->PlaySample(aSFXRefill3);
				Button->SetScale(1.1f);
				//Button->PlayAnimation(BA_Hover);
			}
			return MMB_Popup_MouseOver;
		}
	}
	else
	{
		Button->SetScale(1);
		//Button->PlayAnimation(BA_Idle);
	}

	return MMB_Popup_NoInteraction;
}

void MainMenuControl::Show(bool f)
{
	MenuScene->Show(f);
	#ifdef K_ANDROID
	/*
	if(GFApp->isUnlocked())
		{
			//scn_freeversion->Show(false);
			scn_btnUnlocknow->Show(false);
		}
		*/
	#endif
}

void MainMenuControl::Render(const CPoint* pCam)
{  
	static int FrameCnt = 0;
	FrameCnt = (FrameCnt+1)%2;

	if( isVidFireLoaded )
	{
		if( FrameCnt  || !lpVid_fire->isVideoAvailable() )
			lpVid_fire->readFrame();

		lpVid_fire->getKGraphic()->setAlphaMode(K_ALPHA_ADDITIVE);		
		//lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 356, 292, (short)fireLPos, 200, 0, 1.0f, 1.0f);
		lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 356, 292, (short)fireLPos, 200, 0, 1.0f, 1.0f);

		//lpVid_fire->getKGraphic()->blitAlphaRectFx (356, 0, 712, 292, (short)fireRPos, 200, 0, 1.0f, 1.0f);
		lpVid_fire->getKGraphic()->blitAlphaRectFx (356, 0, 712, 292, (short)fireRPos, 200, 0, 1.0f, 1.0f);
	}
}

/////////////////////////////////////////////////////
void MainMenuControl::InitMessageBanner()
{	
	IsBannerStandStill = false;
	TransOutFactor = 1.0f;
	BannerTimer = 0.0f;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;

	MessageText = new CBitmapText();
	MessageText->SetZ(eZOrder_Banner+1);
	MessageText->SetAlignment(Align_Center);
	MessageText->SetPos(&GFApp->MessageTextPos);
	MessageText->LoadFont(eFont_26);
	//MessageText->SetText("Message Text");
	MessageText->SetText(StringTable::getStr("ExMessage_Text"));
	
	const CPoint MessageBannerPos(0.0f, 0.0f, 0.0f);
	MessageElement = new CObject();
	MessageElement->LoadSprite("UI\\BNR.SPR");
	MessageElement->SetZ(eZOrder_Banner);//above back button
	MessageElement->SetPos(&MessageBannerPos);

	HideMessageBanner();
}

bool MainMenuControl::HideMessageBanner(void)
{
	bool WasActiveMessage = MessageElement->Visible;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;
	MessageElement->Show(false);
	MessageText->Show(false);

	return WasActiveMessage;
}


void MainMenuControl::TransOutBanner()
{
	if( BannerState == eHMS_TransIn )
	{
		BannerTimer = 0.0f;
		BannerState = eHMS_TransOut;
	}
	else
		TransOutFactor = 2.0f;
}


void MainMenuControl::ShowMessageBanner(const std::string& message, int SFXId)
{
	IsBannerStandStill = false;
    TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;	
	
	MessageText->Show(true);
	MessageText->SetText(message);
	MessageElement->Show(true);

	MessageElement->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}

void MainMenuControl::ShowBannerText(const std::string& keyString, int SFXId)
{
	IsBannerStandStill = false;
    TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;	
	
	MessageText->Show(true);
	MessageText->SetText(StringTable::getStr(keyString));
	MessageElement->Show(true);

	MessageElement->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}

void MainMenuControl::UpdateBanner()
{
	if( BannerState == eHMS_Idle )
		return;

	if( BannerState == eHMS_TransIn )
	{
		BannerAlpha += 4.0f * Control::LogicRate;

		if( BannerAlpha >= MAX_BANNER_ALPHA )
		{
			BannerAlpha = MAX_BANNER_ALPHA;
		}

		BannerTimer += Control::LogicRate;
		if( BannerTimer >= 3.0f  || (BannerAlpha == MAX_BANNER_ALPHA && Control::Input->LBclicked()) )
		{
			BannerTimer = 0.0f;
			BannerState = eHMS_TransOut;
		}
	}
	else if( !IsBannerStandStill )
	{
		BannerAlpha -= TransOutFactor * Control::LogicRate;

		if( BannerAlpha <= 0.0f )
		{
			BannerAlpha = 0.0f;
			BannerState = eHMS_Idle;
			MessageElement->Show(false);
			MessageText->Show(false);
			BannerCloseNotify();
		}
	}

	MessageElement->SetAlpha(BannerAlpha);
	float MTAlpha = ( BannerAlpha > MAX_BANNERTEXT_ALPHA )? MAX_BANNERTEXT_ALPHA: BannerAlpha;
	MessageText->SetAlpha(MTAlpha);
}

MainMenuControl::~MainMenuControl()
{	
	GFMainMenuCtrl = NULL;
	Application::SetSingleUpdate(false);

	//SAFE_DELETE(uiButtonTest);	
	//SAFE_DELETE(uiPanelTest);

	SAFE_DELETE(testNodeMesh);

	for (int i = 0; i < NUM_FLYING_PAPS; i++)
	{
		SAFE_DELETE(pappus[i]);
	}

	SAFE_DELETE(VFXCrystalFg);
	SAFE_DELETE(VFXCrystalBg);

	SAFE_DELETE(VFXButton);
	SAFE_DELETE(VFXSnow);

	for(int i=0; i < MODE_TEXT_LINES; ++i)	
		delete ModeDescriptionTexts[i];

	delete MessageElement;
	delete MessageText;
	

	Control::Audio->UnloadQuickLoadedSFX();

	Control::Audio->UnloadSample(aTrackMenu);
	Control::Audio->UnloadSample(aVOMenuTest);

	SAFE_DELETE(lpVid_fire);

	delete ReviewGamePopupScene;
	delete ExitGamePopupScene;
	delete SelectAdventurePopupScene;

	delete EnterYourNamePopupScene;
	delete ProfilesPopupScene;
	delete EraseProfilePopupScene;
	
	//Garbage collection in gameflow
	
	MaskSpr->Show(false);	
	MaskSpr = NULL;
	delete MaskSpr2;
	MaskSpr2 = NULL;

	delete ModeMaskSpr;
	ModeMaskSpr = NULL;
	

	//Garbage collection in gameflow
	OptionsPopupScene->Show(false);
	OptionsPopupScene = NULL;

	delete musicVolTxt;
	delete effectsVolTxt;
	delete voiceVolTxt;
	delete diffModeTxt;
	delete customCurTxt;
	delete fullscreenTxt;
	delete languageTXT;

	SelectModePopupScene->Show(false);
	modeNameText->Show(false);
	SelectModePopupScene = NULL;

	for (int i = 0; i < MAX_PROFILES; i++)
	{
		delete	ProfNameRects[i];			ProfNameRects[i] = NULL;
		delete	ProfDeleteButtonRects[i];	ProfDeleteButtonRects[i] = NULL;
	}

	delete MenuScene;

	delete WelcomePlayerNameText;
	delete	EraseProfileNameText;

	delete modeNameText;

	//delete CopyrightText;
	
	for(int i=0; i < MAX_PROFILES; ++i)
		delete ProfileText[i];

	if( !GFInstance->IsPartialCacheMem )
	{
		GFInstance->UnloadBufferObject(MASK_OBJ);
		GFInstance->UnloadBufferScene(OPTIONS_PSCN);
		GFInstance->UnloadBufferScene(MODESELECT_SCN);
	}

	Application::SetSingleUpdate(false);	
}

void MainMenuControl::ModeTexts_Init(std::string bigstring, int OffsetY, eTextAlignment AlignStyle, int OffsetX)
{
	if(!bigstring.empty()) bigstring = StringTable::getStr(bigstring);
	std::vector<std::string> Strings;
	std::string CurString = "";
	std::string CurWord = "";
	int CurWidth = 0;
	int WordLength = 0;
	unsigned int Char;
	CPoint textPos;
	for(int i=0; i < MODE_TEXT_LINES; ++i)
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
	if(StringTable::IsMyLangaugeLatin())
		CharWidth = 0;//We split words based on $ sign only!! :) 
		CharWidth = ModeDescriptionTexts[0]->charWidth(Char);
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

	if(Strings.size() >= MODE_TEXT_LINES)
	{
		ModeDescriptionTexts[0]->SetText("String is too big.");
	}
	else
	{
		int size = (int)Strings.size();
		for(int i=0; i < size; ++i)
		{
			if(size == 1)  // Sac: if we have only 1 or 2 lines, then push the line to center of the display box
				ModeDescriptionTexts[i+MODE_TEXT_LINES/2]->SetText(Strings[i]);
			else	
				ModeDescriptionTexts[i]->SetText(Strings[i]);			
		}
	}
}


void MainMenuControl::ModeTexts_Show( bool f )
{
	for(int i=0; i < MODE_TEXT_LINES; ++i)
	{
		ModeDescriptionTexts[i]->Show(f);
	}
}

void MainMenuControl::ModeTexts_SetAlpha( float f )
{
	for(int i=0; i < MODE_TEXT_LINES; ++i)
	{
		ModeDescriptionTexts[i]->SetAlpha(f);
	}
}

void MainMenuControl::playPendingCollectableLevel()
{
	int pendingMemCollectableInLevel = Level_Nil;
	//level1..7 - switch to smallest pending collectable level
	for( int i = 0; i < eMem_MAX; i++ )
		if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[i] )
		{
			pendingMemCollectableInLevel = Level_1+ (i/5);
			break;
		}

	int pendingMorphCollectableInLevel = Level_Nil; 
	if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S2Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S3Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S7Collected )
		pendingMorphCollectableInLevel = Level_1;
	else if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S1Collected || 
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S2Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S3Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S4Collected )
		pendingMorphCollectableInLevel = Level_2;	
	else if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S2Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S3Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S1Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S4Collected )
		pendingMorphCollectableInLevel = Level_3;	
	else if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S1Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S3Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S4Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S5Collected )
		pendingMorphCollectableInLevel = Level_4;	
	else if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S2Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S4Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S5Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S6Collected )
		pendingMorphCollectableInLevel = Level_5;	
	else if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected )
		pendingMorphCollectableInLevel = Level_6;	
	else if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S1Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S2Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S3Collected ||
		!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S4Collected )
		pendingMorphCollectableInLevel = Level_7;	

	int playLevel = Level_Nil;
	//if any of morph/mem nil chose other one!!
	if( pendingMemCollectableInLevel == Level_Nil )//no mem collectables pending!!
		playLevel = pendingMorphCollectableInLevel;
	else if( pendingMorphCollectableInLevel == Level_Nil )//no mem collectables pending!!
		playLevel = pendingMemCollectableInLevel;
	else //if both non-nil,  chose smallest
		playLevel = (pendingMemCollectableInLevel < pendingMorphCollectableInLevel)?pendingMemCollectableInLevel: pendingMorphCollectableInLevel;
						
	
	if( playLevel == Level_Nil )//Nothing pending in main chapter!!
	{
		#ifdef _CEBUILD
			if (ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed) 
				ShowBannerText("Excreate");		
			else
				ShowBannerText("Exchapter");
		#else
			//Buy DLC to play bonus chapter
			ShowBannerText("Exbuy");
		#endif		
		
	}
	else if( !isWaitingForMainChapterRevisit )
	{
		GFInstance->IsReplayingGame = true;
		for( int i = 0; i <= eArea_L7_S5_SophiaEnd; i++ )
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[i] = false;//Force clear all tasks - when revisiting we don't need any distractions!!
		ProfileSystem::Instance->ActiveProfile.CurrentLevel = playLevel;		
		switch( playLevel )
		{
			case Level_1: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_Beach;		   ShowBannerText("L1cmis");break;
			case Level_2: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance; ShowBannerText("L2narara");break;
			case Level_3: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;   ShowBannerText("L3broken");break;
			case Level_4: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance;  ShowBannerText("L4missed");break;
			case Level_5: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_TigerCloseup;   ShowBannerText("L5collect");break;
			case Level_6: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1;ShowBannerText("L6collect");break;
			case Level_7: ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;        ShowBannerText("L7level");break;
		}
		isWaitingForMainChapterRevisit = true;
		MainChapterRevisitTicks = 3.0f;		

		PlayBtnClicked = true;
		ClearMenuTrack();
		Control::Input->ClearMouse();
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;					
		Cursor::Show(false);//TURN OFF TO AVOID UNWANTED CLICKS TILL NXT SCENE!!					
	}
}


void MainMenuControl::ShowLanguageSelection()
{
	langBG->Show(true);

    enBtn->Show(true);
    brBtn->Show(true);
    deBtn->Show(true);
    frBtn->Show(true);
    itBtn->Show(true);
    ndBtn->Show(true);
    ruBtn->Show(true);
	esBtn->Show(true);
    enBtnTxt->Show(true);
    brBtnTxt->Show(true);
    deBtnTxt->Show(true);
    frBtnTxt->Show(true);
    itBtnTxt->Show(true);
    ndBtnTxt->Show(true);
    ruBtnTxt->Show(true);
	esBtnTxt->Show(true);

	langBackBtn->Show(true);
	langBackBtnTxt->Show(true);
}


void MainMenuControl::HideLanguageSelection()
{
	langBG->Show(false);

	enBtn->Show(false);
	brBtn->Show(false);
    deBtn->Show(false);
    frBtn->Show(false);
    itBtn->Show(false);
    ndBtn->Show(false);
    ruBtn->Show(false);
	esBtn->Show(false);
    enBtnTxt->Show(false);
    brBtnTxt->Show(false);
    deBtnTxt->Show(false);
    frBtnTxt->Show(false);
    itBtnTxt->Show(false);
    ndBtnTxt->Show(false);
    ruBtnTxt->Show(false);
    //esBtnTxt->Show(false);


	langBackBtn->Show(false);
	langBackBtnTxt->Show(false);
}

void MainMenuControl::UpdateLanguageSelection()
{
#ifdef DISABLE_LANG
	return;
#endif // !DISABLE_LANG

	CPoint pos = Control::Input->Mpos();
	if(!langBG->Visible)
	{
		if(langSelectBtn->IsIntersect(pos))
		{
			if(Control::Input->LBclicked())
			{
				ShowLanguageSelection();
				OptionsPopupScene->Show(false);
			}
		}

		return;
	}

	if(langRestartBG->IsActive)
	{
		if(langRestartOKBtn->IsIntersect(pos))
		{
			if(Control::Input->LBclicked())
			{
                //StringTable::SetMyLangauge(GFApp->GetAssetLanguage());
				/*
				StringTable::SetMyLangauge(GFApp->GetAssetLanguage());

				StringTable::readStringTable(); 
				StringTable::ReadBNRtextSizes();

				AssetsStringTable::readAssetsStringTable();
				AssetsStringTable::ReadBNRtextSizes();
				//HideLanguageSelection();
				//SendMessage(ActionProcessed, ID, HOSM_ReturnToMenu);
				*/
				exit(0);
			}
		}
		return;
	}

	if(langBackBtn->IsIntersect(pos))
	{
		if(Control::Input->LBclicked())
		{
			HideLanguageSelection();
			OptionsPopupScene->Show(true);
		}
	}

    if(enBtn->IsIntersect(pos))
    {
        if(Control::Input->LBclicked())
        {
            GFApp->SetAssetLanguage(LN_ENGLISH);
			langRestartBG->SetEnable(true);
			langRestartOKBtn->SetEnable(true);
			langRestartOKBtnTxt->SetEnable(true);
			langRestartText->Show(true);
			langBackBtn->Show(false);
			langBackBtnTxt->Show(false);
        }
    }

	if (esBtn->IsIntersect(pos))
	{
		if (Control::Input->LBclicked())
		{
			//GFApp->SetAssetLanguage(LN_SPANISH);
			//langRestartBG->SetEnable(true);
			//langRestartOKBtn->SetEnable(true);
			//langRestartOKBtnTxt->SetEnable(true);
			//langRestartText->Show(true);
			//langBackBtn->Show(false);
			//langBackBtnTxt->Show(false);
		}
	}

    if(brBtn->IsIntersect(pos))
    {
        if(Control::Input->LBclicked())
        {
            GFApp->SetAssetLanguage(LN_BRAZILIAN_PORTUGUESE);
			langRestartBG->SetEnable(true);
			langRestartOKBtn->SetEnable(true);
			langRestartOKBtnTxt->SetEnable(true);
			langRestartText->Show(true);
			langBackBtn->Show(false);
			langBackBtnTxt->Show(false);
        }
    }

    if(deBtn->IsIntersect(pos))
    {
        if(Control::Input->LBclicked())
        {
            GFApp->SetAssetLanguage(LN_GERMAN);
			langRestartBG->SetEnable(true);
			langRestartOKBtn->SetEnable(true);
			langRestartOKBtnTxt->SetEnable(true);
			langRestartText->Show(true);
			langBackBtn->Show(false);
			langBackBtnTxt->Show(false);
        }
    }

    if(frBtn->IsIntersect(pos))
    {
        if(Control::Input->LBclicked())
        {
            GFApp->SetAssetLanguage(LN_FRENCH);
			langRestartBG->SetEnable(true);
			langRestartOKBtn->SetEnable(true);
			langRestartOKBtnTxt->SetEnable(true);
			langRestartText->Show(true);
			langBackBtn->Show(false);
			langBackBtnTxt->Show(false);
        }
    }

    if(itBtn->IsIntersect(pos))
    {
        if(Control::Input->LBclicked())
        {
            GFApp->SetAssetLanguage(LN_ITALIAN);
			langRestartBG->SetEnable(true);
			langRestartOKBtn->SetEnable(true);
			langRestartOKBtnTxt->SetEnable(true);
			langRestartText->Show(true);
			langBackBtn->Show(false);
			langBackBtnTxt->Show(false);
        }
    }

    if(ndBtn->IsIntersect(pos))
    {
        if(Control::Input->LBclicked())
        {
            GFApp->SetAssetLanguage(LN_NEDERLANDS);
			langRestartBG->SetEnable(true);
			langRestartOKBtn->SetEnable(true);
			langRestartOKBtnTxt->SetEnable(true);
			langRestartText->Show(true);
			langBackBtn->Show(false);
			langBackBtnTxt->Show(false);
        }
    }

    if(ruBtn->IsIntersect(pos))
    {
        if(Control::Input->LBclicked())
        {
            GFApp->SetAssetLanguage(LN_RUSSIAN);
			langRestartBG->SetEnable(true);
			langRestartOKBtn->SetEnable(true);
			langRestartOKBtnTxt->SetEnable(true);
			langRestartText->Show(true);
			langBackBtn->Show(false);
			langBackBtnTxt->Show(false);
        }
    }

}
	#ifdef _ENABLE_CURL
	std::string _splitString(std::string text, int start, int end)
	{
		std::string result;
		end += start;
		for(int i = start; i < end; i++)
		{
			if(text[i] != '\0')
				result += text[i];
			else 
				break;
		}
		return result;
	}

	void MainMenuControl::CollectUserData()
	{
		if(PopupState == MMB_Popup_Options)
		{
			dataButton->Show(true);
			dataBtnTxt->Show(true);
		}
		else
		{
			dataButton->Show(false);
			dataBtnTxt->Show(false);
			return;
		}
		if(dataButton->IsIntersect(Control::Input->Mpos()))
		{
			Cursor::SetMode(CA_HandPointy);
			if(KInput::getLeftButtonState() == ISDOWN)
			{
				isCollectDia = true;
				emailMessage->Text = "";
				emailAddress->Text = "";
				dataBG->SetEnable(true);
				dataOKButton->SetEnable(true);
				dataCancelBtn->SetEnable(true);
				dataCursor->Show(true);
				#ifdef K_ANDROID
					GFApp->lpUIContainer = new KUIElement (KUIElement::getRootElement());
					GFApp->lpUIContainer->setName ("ui_container");
					GFApp->lpUIContainer->setPosition (0,0);
					GFApp->lpUIContainer->setSize (GFApp->Instance->DesktopWidth, GFApp->Instance->DesktopWidth);

					GFApp->lpUIEditField = new CAndroidUIEditField(GFApp->lpUIContainer);
					GFApp->lpUIEditField->setPosition (0,-100);
					GFApp->lpUIEditField->setSize (400, 30);
					GFApp->lpUIEditField->setTextFont (GFApp->lpFont22);
					GFApp->lpUIEditField->setTextColor (0.91f, 0.8f, 0.51f, 1);
					GFApp->lpUIEditField->setTextOutlineColor (0, 0, 0, 0.25f);
					GFApp->lpUIEditField->setTextOutlineThickness (1);
					GFApp->lpUIEditField->setCursorColor (0, 0, 0, 0.5f);
					GFApp->lpUIEditField->setText ("");
					GFApp->lpUIEditField->setFocus();				
					GFApp->lpUIEditField->setMaxChars(420);
				#endif
			}		
		}
		if(isCollectDia)
		{
			SetFieldFocus();	

			if(emailFieldHasFocus)
			{
				emailAddress->Text = CheckKeyPress(emailAddress->Text);
				dataCursor->SetPos(new CPoint(emailAddress->Pos.x+emailAddress->StringWidth()+5, emailAddress->Pos.y, 0));
				if(emailAddress->StringWidth() > 390)
					emailAddress->Text = _splitString(emailAddress->Text, 0, emailAddress->Text.length()-1);
			}
			else 
			{
				emailMessage->Text = CheckKeyPress(emailMessage->Text);
			}

			if(emailMessage->StringWidth() < messageBoxWidth)
			{
				emailMsgLine1->Text = emailMessage->Text;
				emailMsgLine2->Text = emailMsgLine3->Text = emailMsgLine4->Text = emailMsgLine5->Text = emailMsgLine6->Text = emailMsgLine7->Text = "";
				if(!emailFieldHasFocus) dataCursor->SetPos(new CPoint(emailMsgLine1->Pos.x+emailMsgLine1->StringWidth()+5, emailMsgLine1->Pos.y, 0));
			}
			else if(emailMessage->StringWidth() < messageBoxWidth*2)
			{
				emailMsgLine2->Text = _splitString(emailMessage->Text, emailMsgLine1->Text.length(), emailMessage->Text.length());
				emailMsgLine3->Text = emailMsgLine4->Text = emailMsgLine5->Text = emailMsgLine6->Text = emailMsgLine7->Text = "";
				if(!emailFieldHasFocus) dataCursor->SetPos(new CPoint(emailMsgLine2->Pos.x+emailMsgLine2->StringWidth()+5, emailMsgLine2->Pos.y, 0));
			}
			else if(emailMessage->StringWidth() < messageBoxWidth*3)
			{
				emailMsgLine3->Text = _splitString(emailMessage->Text, emailMsgLine1->Text.length()+emailMsgLine2->Text.length(), emailMessage->Text.length());
				emailMsgLine4->Text = emailMsgLine5->Text = emailMsgLine6->Text = emailMsgLine7->Text = "";
				if(!emailFieldHasFocus) dataCursor->SetPos(new CPoint(emailMsgLine3->Pos.x+emailMsgLine3->StringWidth()+5, emailMsgLine3->Pos.y, 0));
			}
			else if(emailMessage->StringWidth() < messageBoxWidth*4)
			{
				emailMsgLine4->Text = _splitString(emailMessage->Text, emailMsgLine1->Text.length()+emailMsgLine2->Text.length()+emailMsgLine3->Text.length(), emailMessage->Text.length());
				emailMsgLine5->Text = emailMsgLine6->Text = emailMsgLine7->Text = "";
				if(!emailFieldHasFocus) dataCursor->SetPos(new CPoint(emailMsgLine4->Pos.x+emailMsgLine4->StringWidth()+5, emailMsgLine4->Pos.y, 0));
			}
			else if(emailMessage->StringWidth() < messageBoxWidth*5)
			{
				emailMsgLine5->Text = _splitString(emailMessage->Text, emailMsgLine1->Text.length()+emailMsgLine2->Text.length()+emailMsgLine3->Text.length()+emailMsgLine4->Text.length(), emailMessage->Text.length());
				emailMsgLine6->Text = emailMsgLine7->Text = "";
				if(!emailFieldHasFocus) dataCursor->SetPos(new CPoint(emailMsgLine5->Pos.x+emailMsgLine5->StringWidth()+5, emailMsgLine5->Pos.y, 0));
			}
			else if(emailMessage->StringWidth() < messageBoxWidth*6)
			{
				emailMsgLine6->Text = _splitString(emailMessage->Text, emailMsgLine1->Text.length()+emailMsgLine2->Text.length()+emailMsgLine3->Text.length()+emailMsgLine4->Text.length()+emailMsgLine5->Text.length(), emailMessage->Text.length());
				emailMsgLine7->Text = "";
				if(!emailFieldHasFocus) dataCursor->SetPos(new CPoint(emailMsgLine6->Pos.x+emailMsgLine6->StringWidth()+5, emailMsgLine6->Pos.y, 0));
			}
			else if(emailMessage->StringWidth() < messageBoxWidth*7)
			{
				emailMsgLine7->Text = _splitString(emailMessage->Text, emailMsgLine1->Text.length()+emailMsgLine2->Text.length()+emailMsgLine3->Text.length()+emailMsgLine4->Text.length()+emailMsgLine5->Text.length()+emailMsgLine6->Text.length(), emailMessage->Text.length());
				if(!emailFieldHasFocus) dataCursor->SetPos(new CPoint(emailMsgLine7->Pos.x+emailMsgLine7->StringWidth()+5, emailMsgLine7->Pos.y, 0));
			}
			else
			{
				emailMessage->Text = _splitString(emailMessage->Text, 0, emailMsgLine1->Text.length()+emailMsgLine2->Text.length()+emailMsgLine3->Text.length()+emailMsgLine4->Text.length()+emailMsgLine5->Text.length()+emailMsgLine6->Text.length()+emailMsgLine7->Text.length());
				ShowBannerText("bugReportMsgLimit");
				#ifdef K_ANDROID
					GFApp->lpUIEditField->setText (emailMessage->Text.c_str());
				#endif
			}		

			OptionsPopupScene->Show(false);
			musicVolTxt->Show(false);
			effectsVolTxt->Show(false);
			voiceVolTxt->Show(false);
			diffModeTxt->Show(false);
			customCurTxt->Show(false);
			fullscreenTxt->Show(false);
			languageTXT->Show(false);

			Cursor::SetMode(CA_Normal);

			if(dataOKButton->IsIntersect(Control::Input->Mpos()))
			{
				Cursor::SetMode(CA_HandPointy);
				dataOKButton->PlayAnimation(1);
				if(KInput::getLeftButtonState() == ISDOWN)
				{
					const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");				
					if(std::regex_match(emailAddress->Text, pattern))
					{
						GFApp->fromUserEmail = "From : " + emailAddress->Text + "\n\n" + emailMessage->Text;
						GFApp->SendReportToDev();
						isCollectDia = false;
					}
					else
						ShowBannerText("plsEnterEmail");
				}
			}
			else
				dataOKButton->PlayAnimation(0);

			if(dataCancelBtn->IsIntersect(Control::Input->Mpos()))
			{
				Cursor::SetMode(CA_HandPointy);
				dataCancelBtn->PlayAnimation(1);
				if(KInput::getLeftButtonState() == ISDOWN)
				{
					isCollectDia = false;
				}
			}
			else
				dataCancelBtn->PlayAnimation(0);
		}
		else
		{
			dataBG->SetEnable(false);
			dataOKButton->SetEnable(false);
			dataCancelBtn->SetEnable(false);
			dataCursor->Show(false);
			
			emailAddress->Text = "";
			emailMessage->Text = "";
			emailMsgLine1->Text = "";
			emailMsgLine2->Text = "";
			emailMsgLine3->Text = "";
			emailMsgLine4->Text = "";
			emailMsgLine5->Text = "";
			emailMsgLine6->Text = "";
			emailMsgLine7->Text = "";

			OptionsPopupScene->Show(true);
			musicVolTxt->Show(true);
			effectsVolTxt->Show(true);
			voiceVolTxt->Show(true);
			diffModeTxt->Show(true);
			languageTXT->Show(true);
			#ifndef TOUCH_UI
			customCurTxt->Show(true);
			fullscreenTxt->Show(true);
			#endif
		}
	}


	std::string MainMenuControl::CheckKeyPress(std::string inputText)
	{
		#ifdef K_ANDROID
			inputText = GFApp->lpUIEditField->getText();
		#else
		//Text Input		
		for(int i = 0; i < 36; i++)
		{
			if(i < 26) // A-Z
			{
				if(KInput::isPressed((EKeyboardLayout)(25+i)))
					keyPressed[i] = true;
				else
				{
					if(keyPressed[i])
					{
						keyPressed[i] = false;
						if(KInput::isPressed(K_VK_L_SHIFT)||KInput::isPressed(K_VK_R_SHIFT))
							inputText += (char)(i+65);
						else
							inputText += (char)(i+97);
					}
				}
			}
			else if(i >= 26) // 0-9
			{
				if(KInput::isPressed((EKeyboardLayout)(51+(i-26)))  && (!KInput::isPressed(K_VK_L_SHIFT)||!KInput::isPressed(K_VK_R_SHIFT)))
					keyPressed[i] = true;
				else
				{
					if(keyPressed[i])
					{
						keyPressed[i] = false;
						inputText += (char)((i-26)+48);
					}
				}
			}
		}	

		//Special Chars
		if((KInput::isPressed(K_VK_2)||KInput::isPressed(K_VK_OEM_7)))
			keyPressed[36] = true;
		else
		{
			if(keyPressed[36])
			{
				keyPressed[36] = false;
				if((KInput::isPressed(K_VK_L_SHIFT)||KInput::isPressed(K_VK_R_SHIFT)))
					inputText += "@";
			}
		}

		if(KInput::isPressed(K_VK_OEM_PERIOD))
			keyPressed[37] = true;
		else
		{
			if(keyPressed[37])
			{
				keyPressed[37] = false;
				inputText += ".";
			}
		}

		if(KInput::isPressed(K_VK_BACK))
			keyPressed[38] = true;
		else
		{
			if(keyPressed[38])
			{
				keyPressed[38] = false;
				inputText = inputText.substr(0, inputText.length()-1);
			}
		}

		if(KInput::isPressed(K_VK_OEM_MINUS))
			keyPressed[39] = true;
		else
		{
			if(keyPressed[39])
			{
				keyPressed[39] = false;
				if(KInput::isPressed(K_VK_L_SHIFT)||KInput::isPressed(K_VK_R_SHIFT))
					inputText += "_";
				else
					inputText += "-";
			}
		}

		if(KInput::isPressed(K_VK_OEM_COMMA))
			keyPressed[40] = true;
		else
		{
			if(keyPressed[40])
			{
				keyPressed[40] = false;
				inputText += ",";
			}
		}
		
		if(KInput::isPressed(K_VK_SPACE))
			keyPressed[41] = true;
		else
		{
			if(keyPressed[41])
			{
				keyPressed[41] = false;
				inputText += " ";
			}
		}	
		#endif	

		return inputText;
	}

	void MainMenuControl::SetFieldFocus()
	{
		CPoint cPos = Control::Input->Mpos();

		if(cPos.x >= 520 && cPos.x <= 930 && cPos.y >= 80 && cPos.y <= 100)
		{
			if(Control::Input->LBdown())
			{
				emailFieldHasFocus = true;
				#ifdef K_ANDROID
					GFApp->lpUIEditField->setFocus();
					GFApp->lpUIEditField->setText (emailAddress->Text.c_str());
				#endif
			}		
		}
		if(cPos.x >= 430 && cPos.x <= 930 && cPos.y >= 170 && cPos.y <= 380)
		{
			if(Control::Input->LBdown())
			{
				emailFieldHasFocus = false;
				#ifdef K_ANDROID
					GFApp->lpUIEditField->setFocus();
				GFApp->lpUIEditField->setText (emailMessage->Text.c_str());
				#endif
			}		
		}
	}
	#endif


extern "C"
JNIEXPORT void JNICALL
Java_com_example_test_1android_1kanji_MainActivity_nativeKeyPress(JNIEnv *env, jobject thiz,
                                                                  jint key_code) {
    // Assuming CheatLevel and Level_X are defined elsewhere in your code
    switch (key_code) {
        case AKEYCODE_1:
            NativeCheatLevel = Level_1;
            LOGI("Set CheatLevel to Level_1");
            break;
        case AKEYCODE_2:
            NativeCheatLevel = Level_2;
            LOGI("Set CheatLevel to Level_2");
            break;
        case AKEYCODE_3:
            NativeCheatLevel = Level_3;
            LOGI("Set CheatLevel to Level_3");
            break;
        case AKEYCODE_4:
            NativeCheatLevel = Level_4;
            LOGI("Set CheatLevel to Level_4");
            break;
        case AKEYCODE_5:
            NativeCheatLevel = Level_5;
            LOGI("Set CheatLevel to Level_5");
            break;
        case AKEYCODE_6:
            NativeCheatLevel = Level_6;
            LOGI("Set CheatLevel to Level_6");
            break;
        case AKEYCODE_7:
            NativeCheatLevel = Level_7;
            LOGI("Set CheatLevel to Level_7");
            break;
        case AKEYCODE_8:
            NativeCheatLevel = Level_8;
            LOGI("Set CheatLevel to Level_8");
            break;
        default:
            LOGI("Unhandled key code: %d", key_code);
            break;
    }
}