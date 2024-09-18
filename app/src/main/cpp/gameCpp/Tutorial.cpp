//====================================
//DQFULP
// Tutorial
//====================================

#include "Tutorial.h"
#include "Object.h"
#include "Rect.h"
#include "Control.h"
#include "exMath.h"
#include "Fonts.h"
#include "GameFlow.h"//defines LANGDEF and Stringtable
#include "DataEnums.h"
#include "exMath.h"
#include "Hud.h"
#include "Application.h"
#include "AwardManager.h"
#include "GameTextManager.h"


#if defined( ENGLISH_BUILD )

std::string TutorialTexts[TUT_MAX] =
{
	"Exempty",
	"Extalk",
	"Exinteract",
	"Exwhenever",
	"Exclick",
	"Expick",
	"Exexamine",
	"Exsome",
	"Exup",
	"Exarea",
	"Exuse",
	"Exmorph",
	"Exnavigate",
	"Exhint",
	"Exmap",
	"Exand",
	"Exskip",
	"Exmemory",
	"Excombine",
	"Expart",
	"Exglow",
	"Exyou",
	"Exnon"
};

#endif

// const float AlphaSpeed = 2.0f;  // Sac: Commenting Unused variable

enum cursorId
{
	eTutCursor_Talk,
	eTutCursor_HandTake,
	eTutCursor_ZoomLens,
	eTutCursor_UpArrow,
	eTutCursor_Knife,
	eTutCursor_HandPointy
};

Tutorial::Tutorial()
{	
	State = TUT00_Prompt;
	TutTransitionState = TUT_HIDDEN;
	
	if(ProfileSystem::Instance->ActiveProfile.TutorialState == TUT_Inactive || ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted)
		IsActive = false;
	else
		IsActive = true;

	IsVisible = false;

	PrevCursorMode = -1;

	BtnScaleAngle = 0;	

	IsTutFadingOut = false;
	IsNextTutQueued = false;

	isMouseOverTutorUI = false;

	//Load the tutorial scene
	TutorialScene = GFInstance->LoadBufferScene(TUTORIAL_SCN);

	//Set up initial state
	TutorialScene->Show(false);
	if(TutorialScene->GetObjectByID("tutframe")->ZOrder<2)
		TutorialScene->SetZOff(eZOrder_Banner + 900);

	//Set Init Positions
	TutorialScene->SaveXY();

	FrameBg = TutorialScene->GetObjectByID("tutframe");
	BtnSkip = TutorialScene->GetObjectByID("btntutskip");
	BtnContinue = TutorialScene->GetObjectByID("btntxtcontinue");
	IconBg = TutorialScene->GetObjectByID("cursorframe");
	CursorIcon = TutorialScene->GetObjectByID("cursoricon");

	Arrow1 = TutorialScene->GetObjectByID("tutarrow1");
	Arrow2 = TutorialScene->GetObjectByID("tutarrow2");
	CircleAnim1 = TutorialScene->GetObjectByID("tutcircleanim1");
	CircleAnim2 = TutorialScene->GetObjectByID("tutcircleanim2");

	PromptWindowScn = NULL;
	isPromptWindowShowing = false;
	isSkipPromptWindowActive = false;

	BtnFullTut = BtnSpecialFeatures = BtnNoTutorial = NULL;

	TutorSkipMask = NULL;

	textOffsetY = -32; //Sac: this was -28, I'm increasing it little more to make the text get closer to the frame

	MultiTextTutorial = NULL;

	ColorRGB white = {0.90f, 0.90f, 0.80f};
	float _lineWidth = 480;
	int  _nLines = 4;
	EFont _font = eFont_26;
	eTextAlignment _align = Align_Center;
	if(StringTable::IsKeyFound(TutorialTexts[0]))
	{
		_lineWidth = StringTable::GetLineWidth(TutorialTexts[0]);
		_nLines = StringTable::GetNoOfLines(TutorialTexts[0]);
		_font = (EFont)StringTable::GetFont(TutorialTexts[0]);
	}
	MultiTextTutorial = new MultilineText(_nLines, PointSystem::CreateCPoint(FrameBg->Pos.x, FrameBg->Pos.y+textOffsetY, 0), eZOrder_Banner + 1000, white, 30, _lineWidth, _align, _font); // Sac: line with is 500, chainging it to 480
	MultiTextTutorial->SetTextMultiline(StringTable::getStr(TutorialTexts[0]));
	MultiTextTutorial->Show(false);
}

Tutorial::~Tutorial()
{
	SAFE_DELETE(PromptWindowScn);
	SAFE_DELETE(TutorSkipMask);
	isPromptWindowShowing = false;

	SAFE_DELETE(MultiTextTutorial);

	if(IsActive) //else it will be unloaded
	{
		TutorialScene->RestoreXY();

		//Garbage collection in gameflow
		TutorialScene->Show(false);
		TutorialScene = NULL;
	}

	if( !GFInstance->IsPartialCacheMem )
	{
		GFInstance->UnloadBufferScene(TUTORIAL_SCN);
	}
}

void Tutorial::ShowSkipTutorialWindow()
{
	if(PromptWindowScn==NULL)
	{
		PromptWindowScn = new CHOScene();
		PromptWindowScn->Load("TUT\\TUTPROMPT.SCN");

		tutHeading = new CObject();
		tutHeading->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\TUTORIAL.SPR", 683, 162, 10);
		tutHeading->SetUID("TUTORIAL_HEAD");
		PromptWindowScn->PushObject(tutHeading);

		tutPromptMsg = new CObject();
		tutPromptMsg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\MSGTEXT.SPR", PromptWindowScn->GetObjectByID("tutprompttext")->Pos.x, PromptWindowScn->GetObjectByID("tutprompttext")->Pos.y, PromptWindowScn->GetObjectByID("tutprompttext")->ZOrder + 10);
		tutPromptMsg->SetUID("tutPromptMsgTXT");
		PromptWindowScn->PushObject(tutPromptMsg);

		BtnFullTutTXT = new CObject();
		BtnFullTutTXT->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\FULL_TUT.SPR", PromptWindowScn->GetObjectByID("BtnTutFull")->Pos.x, PromptWindowScn->GetObjectByID("BtnTutFull")->Pos.y, PromptWindowScn->GetObjectByID("BtnTutFull")->ZOrder + 10);
		BtnFullTutTXT->SetUID("BtnTutFullTXT");
		PromptWindowScn->PushObject(BtnFullTutTXT);

		BtnSpecialFeatureTXT = new CObject();
		BtnSpecialFeatureTXT->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\SPL_TUT.SPR", PromptWindowScn->GetObjectByID("BtnTutSpecial")->Pos.x, PromptWindowScn->GetObjectByID("BtnTutSpecial")->Pos.y, PromptWindowScn->GetObjectByID("BtnTutSpecial")->ZOrder + 10);
		BtnSpecialFeatureTXT->SetUID("tBtnTutSpecialTXT");
		PromptWindowScn->PushObject(BtnSpecialFeatureTXT);

		BtnNoTutorialTXT = new CObject();
		BtnNoTutorialTXT->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\NO_TUT.SPR", PromptWindowScn->GetObjectByID("BtnTutNo")->Pos.x, PromptWindowScn->GetObjectByID("BtnTutNo")->Pos.y, PromptWindowScn->GetObjectByID("BtnTutNo")->ZOrder + 10);
		BtnNoTutorialTXT->SetUID("BtnTutNoTXT");
		PromptWindowScn->PushObject(BtnNoTutorialTXT);

		btnTXTYes = new CObject();
		btnTXTYes->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", PromptWindowScn->GetObjectByID("BtnYes")->Pos.x, PromptWindowScn->GetObjectByID("BtnYes")->Pos.y, PromptWindowScn->GetObjectByID("BtnYes")->ZOrder + 10);
		btnTXTYes->SetUID("BtnYesTXT");
		PromptWindowScn->PushObject(btnTXTYes);

		btnTXTNo = new CObject();
		btnTXTNo->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", PromptWindowScn->GetObjectByID("BtnNo")->Pos.x, PromptWindowScn->GetObjectByID("BtnNo")->Pos.y, PromptWindowScn->GetObjectByID("BtnNo")->ZOrder + 10);
		btnTXTNo->SetUID("BtnNoTXT");
		PromptWindowScn->PushObject(btnTXTNo);

		mask = new CObject();
		mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
		mask->SetUID("mask");
		mask->SetScale(2);
		PromptWindowScn->PushObject(mask);

		PromptWindowScn->SetZOff(eZOrder_Banner + 1100);
	}

	PromptWindowScn->GetObjectByID("BtnTutFull")->SetEnable(false);
	PromptWindowScn->GetObjectByID("BtnTutSpecial")->SetEnable(false);
	PromptWindowScn->GetObjectByID("BtnTutNo")->SetEnable(false);

	PromptWindowScn->GetObjectByID("BtnNo")->SetEnable(true);
	PromptWindowScn->GetObjectByID("BtnYes")->SetEnable(true);


	PromptWindowScn->GetObjectByID("BtnNoTXT")->SetEnable(true);
	PromptWindowScn->GetObjectByID("BtnYesTXT")->SetEnable(true);

	BtnFullTutTXT->SetEnable(false); BtnSpecialFeatureTXT->SetEnable(false); BtnNoTutorialTXT->SetEnable(false); mask->SetEnable(false);
	tutPromptMsg->PlayAnimation(1);

	
	if(TutorSkipMask==NULL)
	{
		TutorSkipMask = new CObject();
		TutorSkipMask->LoadSprite("UI\\MASK.SPR");
		TutorSkipMask->SetXY(683,384);
		TutorSkipMask->SetScale(2.0);
		TutorSkipMask->SetZ(eZOrder_HUD+10);
	}
	
	TutorSkipMask->FadeIn(2.0f);
	

	PromptWindowScn->GetObjectByID("tutprompttext")->PlayAnimation(1);

	PromptWindowScn->Show(true);
	PromptWindowScn->FadeIn(2.0f);


	isPromptWindowShowing = true;
	isSkipPromptWindowActive = true;
}

void Tutorial::ShowTutorial(TutorialState TutId)
{
	State = TutId;
	if( IsTutFadingOut )
	{
		IsNextTutQueued = true;
		return;
	}

	TutorialScene->Show(false);

	Arrow1->SetEnable(false);
	Arrow2->SetEnable(false);

	CircleAnim1->SetEnable(false);
	CircleAnim2->SetEnable(false);

	IconBg->SetEnable(false);
	CursorIcon->SetEnable(false);
	BtnContinue->SetEnable(false);

	switch (TutId)
	{
	case TUT00_Prompt:

		PromptWindowScn = new CHOScene();
		PromptWindowScn->Load("TUT\\TUTPROMPT.SCN");

		tutHeading = new CObject();
		tutHeading->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\TUTORIAL.SPR", 683, 162, 10);
		tutHeading->SetUID("TUTORIAL_HEAD");
		PromptWindowScn->PushObject(tutHeading);

		tutPromptMsg = new CObject();
		tutPromptMsg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\MSGTEXT.SPR", PromptWindowScn->GetObjectByID("tutprompttext")->Pos.x, PromptWindowScn->GetObjectByID("tutprompttext")->Pos.y, PromptWindowScn->GetObjectByID("tutprompttext")->ZOrder + 10);
		tutPromptMsg->SetUID("tutPromptMsgTXT");
		PromptWindowScn->PushObject(tutPromptMsg);

		BtnFullTutTXT = new CObject();
		BtnFullTutTXT->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\FULL_TUT.SPR", PromptWindowScn->GetObjectByID("BtnTutFull")->Pos.x, PromptWindowScn->GetObjectByID("BtnTutFull")->Pos.y, PromptWindowScn->GetObjectByID("BtnTutFull")->ZOrder + 10);
		BtnFullTutTXT->SetUID("BtnTutFullTXT");
		PromptWindowScn->PushObject(BtnFullTutTXT);

		BtnSpecialFeatureTXT = new CObject();
		BtnSpecialFeatureTXT->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\SPL_TUT.SPR", PromptWindowScn->GetObjectByID("BtnTutSpecial")->Pos.x, PromptWindowScn->GetObjectByID("BtnTutSpecial")->Pos.y, PromptWindowScn->GetObjectByID("BtnTutSpecial")->ZOrder + 10);
		BtnSpecialFeatureTXT->SetUID("tBtnTutSpecialTXT");
		PromptWindowScn->PushObject(BtnSpecialFeatureTXT);

		BtnNoTutorialTXT = new CObject();
		BtnNoTutorialTXT->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\TUT\\NO_TUT.SPR", PromptWindowScn->GetObjectByID("BtnTutNo")->Pos.x, PromptWindowScn->GetObjectByID("BtnTutNo")->Pos.y, PromptWindowScn->GetObjectByID("BtnTutNo")->ZOrder + 10);
		BtnNoTutorialTXT->SetUID("BtnTutNoTXT");
		PromptWindowScn->PushObject(BtnNoTutorialTXT);

		btnTXTYes = new CObject();
		btnTXTYes->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", PromptWindowScn->GetObjectByID("BtnYes")->Pos.x, PromptWindowScn->GetObjectByID("BtnYes")->Pos.y, PromptWindowScn->GetObjectByID("BtnYes")->ZOrder + 10);
		btnTXTYes->SetUID("BtnYesTXT");
		PromptWindowScn->PushObject(btnTXTYes);

		btnTXTNo = new CObject();
		btnTXTNo->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", PromptWindowScn->GetObjectByID("BtnNo")->Pos.x, PromptWindowScn->GetObjectByID("BtnNo")->Pos.y, PromptWindowScn->GetObjectByID("BtnNo")->ZOrder + 10);
		btnTXTNo->SetUID("BtnNoTXT");
		PromptWindowScn->PushObject(btnTXTNo);

		mask = new CObject();
		mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
		mask->SetUID("mask");
		mask->SetScale(2);
		PromptWindowScn->PushObject(mask);
		
		PromptWindowScn->SetZOff(eZOrder_Banner + 1100);

		BtnFullTut = PromptWindowScn->GetObjectByID("BtnTutFull");
		BtnSpecialFeatures = PromptWindowScn->GetObjectByID("BtnTutSpecial");
		BtnNoTutorial = PromptWindowScn->GetObjectByID("BtnTutNo");

		PromptWindowScn->GetObjectByID("BtnNo")->SetEnable(false);
		PromptWindowScn->GetObjectByID("BtnYes")->SetEnable(false);

		
		PromptWindowScn->GetObjectByID("BtnNoTXT")->SetEnable(false);
		PromptWindowScn->GetObjectByID("BtnYesTXT")->SetEnable(false);

		isPromptWindowShowing = true;

		GFInstance->CreateGreyscaleScene();
		GFInstance->UpdateBlurScene(1.0f);

		break;

	case TUT01_TalkWithRaja:
		TutorialScene->ShiftXY(900, 220);

		Arrow1->SetPos( PointSystem::CreateCPoint(668,341,0) );
		Arrow1->SetRotate(-135.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(533,404,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		IconBg->SetEnable(true);
		CursorIcon->PlayAnimation(eTutCursor_Talk);
		CursorIcon->SetEnable(true);
		break;

	case TUT02_TakeJames:
		TutorialScene->ShiftXY(936, 335);

		Arrow1->SetPos( PointSystem::CreateCPoint(811,460,0) );
		Arrow1->SetRotate(-120.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(708,564,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		IconBg->SetEnable(true);
		CursorIcon->PlayAnimation(eTutCursor_HandTake);
		CursorIcon->SetEnable(true);
		break;
	
	case TUT03_Journal:
		TutorialScene->ShiftXY(511, 420);

		Arrow1->SetPos( PointSystem::CreateCPoint(313,543,0) );
		Arrow1->SetRotate(-120.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(217,654,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);
		break;

	case TUT04_ObjectivesTab:
		TutorialScene->ShiftXY(528, 165);

		Arrow1->SetPos( PointSystem::CreateCPoint(387,267,0) );
		Arrow1->SetRotate(-135.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		Arrow1->SetZ(eZOrder_Banner + 1000);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(238,323,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		CircleAnim1->SetZ(eZOrder_Banner + 1000);
		break;

	case TUT05_BagInvTake:
		TutorialScene->ShiftXY(839, 350);

		Arrow1->SetPos( PointSystem::CreateCPoint(792,472,0) );
		Arrow1->SetRotate(-120.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(683,571,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);
		break;
	
	case TUT06_BagZoomLook:
		TutorialScene->ShiftXY(577, 469);
		{
		//Temporary Solution to activate the inventory box when mouseover. Mouse moved to inv box
		int CurX = GFApp->CurrentResolution.Width/2;
		int CurY = GFApp->CurrentResolution.Height-15;
		Cursor::SetXY((float)CurX,(float)(CurY));
		KInput::mousePointerTo((long)(CurX),(long)(CurY));//avoid invbox autoopen at init from menu - or prevscene_exit_clk!!
		}
		GFHud->InvBoxCtrl->Activate();
		GFHud->InvBoxCtrl->IsLocked = true;
		

		Arrow1->SetPos( PointSystem::CreateCPoint(509,589,0) );
		Arrow1->SetRotate(-105.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(432,712,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		IconBg->SetEnable(true);
		CursorIcon->PlayAnimation(eTutCursor_ZoomLens);
		CursorIcon->SetEnable(true);
		break;

	case TUT07_ZipperOpen:
		TutorialScene->ShiftXY(892, 361);

		Arrow1->SetPos( PointSystem::CreateCPoint(738,253,0) );
		Arrow1->SetRotate(135.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(647,164,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		IconBg->SetEnable(true);
		CursorIcon->PlayAnimation(eTutCursor_HandPointy);
		CursorIcon->SetEnable(true);
		break;

	case TUT08_KnifeTakeFromBag:
		TutorialScene->ShiftXY(558, 421);

		Arrow1->SetPos( PointSystem::CreateCPoint(464,291,0) );
		Arrow1->SetRotate(45.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(582,206,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		IconBg->SetEnable(true);
		CursorIcon->PlayAnimation(eTutCursor_HandTake);
		CursorIcon->SetEnable(true);
		break;

	case TUT09_RosePopupZoom:
		TutorialScene->ShiftXY(702, 228);

		Arrow1->SetPos( PointSystem::CreateCPoint(826,357,0) );
		Arrow1->SetRotate(-60.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(904,454,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		IconBg->SetEnable(true);
		CursorIcon->PlayAnimation(eTutCursor_ZoomLens);
		CursorIcon->SetEnable(true);
		break;

	case TUT10_UseKnifeOnRose:
		TutorialScene->ShiftXY(910, 164);
		{
		//Temporary Solution to activate the inventory box when mouseover. Mouse moved to inv box
		int CurX = GFApp->CurrentResolution.Width/2;
		int CurY = GFApp->CurrentResolution.Height-15;
		Cursor::SetXY((float)CurX,(float)(CurY));
		KInput::mousePointerTo((long)(CurX),(long)(CurY));//avoid invbox autoopen at init from menu - or prevscene_exit_clk!!
		}

		GFHud->InvBoxCtrl->IsLocked = true;																							 
		GFHud->InvBoxCtrl->Activate();

		Arrow1->SetPos( PointSystem::CreateCPoint(684,274,0) );
		Arrow1->SetRotate(-135.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		Arrow2->SetPos( PointSystem::CreateCPoint(457,605,0) );
		Arrow2->SetRotate(-60.0f);
		Arrow2->SetScale(0.50f);
		Arrow2->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(558,311,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		CircleAnim2->SetPos( PointSystem::CreateCPoint(548,697,0) );
		CircleAnim2->SetScale(0.65f);
		CircleAnim2->ForcePlayAnimation(0);
		CircleAnim2->SetEnable(true);

		IconBg->SetEnable(true);
		CursorIcon->PlayAnimation(eTutCursor_Knife);
		CursorIcon->SetEnable(true);
		break;
	
	case TUT11_MorphObject:
		TutorialScene->ShiftXY(618, 280);

		Arrow1->SetPos( PointSystem::CreateCPoint(812,164,0) );
		Arrow1->SetRotate(60.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(940,102,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);
		break;

	case TUT12_NavigationUp:
		TutorialScene->ShiftXY(808, 536);

		Arrow1->SetPos( PointSystem::CreateCPoint(581,420,0) );
		Arrow1->SetRotate(135.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(533,308,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);
		break;

	case TUT13_HintTutorial:
		TutorialScene->ShiftXY(876, 432);

		Arrow1->SetPos( PointSystem::CreateCPoint(1010,554,0) );
		Arrow1->SetRotate(-60.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(1105,641,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		BtnContinue->SetEnable(true);
		break;

	case TUT14_MapTutorial:
		TutorialScene->ShiftXY(553, 436);

		Arrow1->SetPos( PointSystem::CreateCPoint(390,552,0) );
		Arrow1->SetRotate(-135.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(287,644,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);
		break;

	case TUT15_MapDetailsTutorial:
		TutorialScene->ShiftXY(456, 239);

		BtnContinue->SetEnable(true);
		break;

	case TUT16_SkipPuzzle:
		TutorialScene->ShiftXY(876, 432);

		Arrow1->SetPos( PointSystem::CreateCPoint(1010,554,0) );
		Arrow1->SetRotate(-60.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(1105,641,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		BtnContinue->SetEnable(true);
		break;

	case TUT17_MemoryCollectible:
		TutorialScene->ShiftXY(946, 147);

		Arrow1->SetPos( PointSystem::CreateCPoint(810,271,0) );
		Arrow1->SetRotate(-135.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(711,370,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);
		break;

	case TUT18_ComboInvZoomLook:
		{
		TutorialScene->ShiftXY(683, 432);

		//Get the Beetle Position
		float beetlePosX=0;

		{
		//Temporary Solution to activate the inventory box when mouseover. Mouse moved to inv box
		int CurX = GFApp->CurrentResolution.Width/2;
		int CurY = GFApp->CurrentResolution.Height-15;
		Cursor::SetXY((float)CurX,(float)(CurY));
		KInput::mousePointerTo((long)(CurX),(long)(CurY));//avoid invbox autoopen at init from menu - or prevscene_exit_clk!!
		}

		GFHud->InvBoxCtrl->IsLocked = true;																							 
		GFHud->InvBoxCtrl->Activate();
						
		for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
		{
			if( GFHud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
			{	
				if(  GFHud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_BeetleHead)
				{
					if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
							GFHud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
					else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
							GFHud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
					beetlePosX = GFHud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x;
					break;
				}
			}
		}

		Arrow1->SetPos( PointSystem::CreateCPoint(beetlePosX,605,0) );
		Arrow1->SetRotate(-80.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(beetlePosX,697,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);
		}
		break;

	case TUT19_ComboInvUseCombine:
		{
		TutorialScene->ShiftXY(510, 432);

		//Get the Beetle Position
		float beetlePosX=0;

		{
		//Temporary Solution to activate the inventory box when mouseover. Mouse moved to inv box
		int CurX = GFApp->CurrentResolution.Width/2;
		int CurY = GFApp->CurrentResolution.Height-15;
		Cursor::SetXY((float)CurX,(float)(CurY));
		KInput::mousePointerTo((long)(CurX),(long)(CurY));//avoid invbox autoopen at init from menu - or prevscene_exit_clk!!
		}

		GFHud->InvBoxCtrl->IsLocked = true;																							 
		GFHud->InvBoxCtrl->Activate();
						
		for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
		{
			if( GFHud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
			{	
				if(  GFHud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_BeetleTail)
				{
					if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
							GFHud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
					else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
							GFHud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
					beetlePosX = GFHud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x;
					break;
				}
			}
		}

		Arrow1->SetPos( PointSystem::CreateCPoint(beetlePosX,605,0) );
		Arrow1->SetRotate(-80.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos( PointSystem::CreateCPoint(beetlePosX,697,0) );
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);
		}
		break;

	case TUT21_Match3Tutorial:
		TutorialScene->ShiftXY(453, 285);

		Arrow1->SetPos(PointSystem::CreateCPoint(576,141,0));
		Arrow1->SetRotate(50.0f);
		Arrow1->SetScale(0.50f);
		Arrow1->SetEnable(true);

		CircleAnim1->SetPos(PointSystem::CreateCPoint(683,45,0));
		CircleAnim1->SetScale(0.65f);
		CircleAnim1->ForcePlayAnimation(0);
		CircleAnim1->SetEnable(true);

		BtnContinue->SetEnable(true);
		break;

		
		
	case TUT_MAX:
		break;
	default:
		break;
	}
	if(isPromptWindowShowing)
	{
		PromptWindowScn->FadeIn(2.0f);
	}

	if(TutId != TUT00_Prompt)
	{
		TutorialScene->Show(true);
		MultiTextTutorial->Show(true);
	}
	else
	{
		MultiTextTutorial->Show(false);
	}

	
	ColorRGB white = {0.90f, 0.90f, 0.80f};	
	float _lineWidth = 480;
	int  _nLines = 4;
	EFont _font = eFont_26;
	eTextAlignment _align = Align_Center;
	if(StringTable::IsKeyFound(TutorialTexts[TutId]))
	{
		_lineWidth = StringTable::GetLineWidth(TutorialTexts[TutId]);
		_nLines = StringTable::GetNoOfLines(TutorialTexts[TutId]);
		_font = (EFont)StringTable::GetFont(TutorialTexts[TutId]);
	}
	//MultiTextTutorial = NULL;
	SAFE_DELETE(MultiTextTutorial);
	MultiTextTutorial = new MultilineText(_nLines, PointSystem::CreateCPoint(FrameBg->Pos.x, FrameBg->Pos.y+textOffsetY, 0), eZOrder_Banner + 1000, white, 30, _lineWidth, _align, _font); // Sac: line with is 500, chainging it to 480
	MultiTextTutorial->SetPosXY(FrameBg->Pos.x, FrameBg->Pos.y+textOffsetY);
	MultiTextTutorial->SetTextMultiline(StringTable::getStr(TutorialTexts[TutId]));

	TutTransitionState = TUT_FADEIN;

	TutTransGapTimer = 0.35f; //default is 1 second
	TutTransAlpha = 0;
	SetTutorialAlpha(TutTransAlpha);
	IsActive = true;
	IsVisible = true;
}

void Tutorial::Required()
{

}

void Tutorial::SetTutorialAlpha(float alpha)
{
	TutorialScene->SetSceneAlpha(alpha);
	MultiTextTutorial->setAlpha(alpha);
}

void Tutorial::Update()
{
	if(!IsActive)
	{
		return;
	}

	CPoint pos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	isMouseOverTutorUI = false;

	if(isPromptWindowShowing)
	{
		if (isSkipPromptWindowActive)
		{
			//yes no buttons only in skip tutorial window
			CObject *btnCancel = PromptWindowScn->GetObjectByID("BtnNo");
			CObject *btnOk = PromptWindowScn->GetObjectByID("BtnYes");

			if(btnOk->Collide(&pos))
			{
				btnOk->PlayAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				isMouseOverTutorUI = true;
				if(clicked)
				{
					isMouseOverTutorUI = false;
					isMouseOverTutorUI = false;
					isPromptWindowShowing = false;
					btnOk->PlayAnimation(2);

					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);

					GFInstance->TurnOffBlurScene();

					PromptWindowScn->FadeOut(2.0f);
					
					TutorSkipMask->FadeOut(2.0f);
					Deactivate();
					
					Control::Input->ClearMouse();
					return;
				}
			}
			else
				btnOk->PlayAnimation(0);

			if(btnCancel->Collide(&pos))
			{
				btnCancel->PlayAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				isMouseOverTutorUI = true;
				if(clicked)
				{
					isMouseOverTutorUI = false;

					isPromptWindowShowing = false;
					btnCancel->PlayAnimation(2);

					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);

					PromptWindowScn->FadeOut(2.0f);

					TutorSkipMask->FadeOut(2.0f);
					isSkipPromptWindowActive = false;
					
					Control::Input->ClearMouse();
					return;
				}
			}
			else
				btnCancel->PlayAnimation(0);
		}
		else
		{
			//Prompt for Full Tutorial, Special Features or No Tutorial
			if(BtnFullTut->Collide(&pos, -8))
			{
				BtnFullTut->PlayAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				isMouseOverTutorUI = true;
				if(clicked)
				{
					isMouseOverTutorUI = false;

					isPromptWindowShowing = false;
					BtnFullTut->PlayAnimation(2);

					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);

					GFInstance->TurnOffBlurScene();

					PromptWindowScn->FadeOut(2.0f);

					ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT00_Prompt] = true;
					ShowTutorial(TUT01_TalkWithRaja);

					Control::Input->ClearMouse();
					return;
				}
			}
			else
				BtnFullTut->PlayAnimation(0);

			if(BtnSpecialFeatures->Collide(&pos, -8))
			{
				BtnSpecialFeatures->PlayAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				isMouseOverTutorUI = true;
				if(clicked)
				{
					isMouseOverTutorUI = false;

					isPromptWindowShowing = false;
					BtnSpecialFeatures->PlayAnimation(2);

					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);

					GFInstance->TurnOffBlurScene();

					PromptWindowScn->FadeOut(2.0f);

					ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT00_Prompt] = true;
					ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly = true;
					//ShowTutorial(TUT01_TalkWithRaja);
					HideTutorial(true);

					if( !ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames)
					{
						//force disable morphing objects until raja took james
#ifdef _CEBUILD
						GFHud->isMorphObjectsActive = false;
#endif
					}

					Control::Input->ClearMouse();
					return;
				}
			}
			else
				BtnSpecialFeatures->PlayAnimation(0);

			if(BtnNoTutorial->Collide(&pos, -8))
			{
				BtnNoTutorial->PlayAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				isMouseOverTutorUI = true;
				if(clicked)
				{
					isMouseOverTutorUI = false;

					isPromptWindowShowing = false;
					BtnNoTutorial->PlayAnimation(2);

					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);

					PromptWindowScn->FadeOut(2.0f);
					
					GFInstance->TurnOffBlurScene();
					AwardManager::GetInstance()->SkipTutorial();
					Deactivate();

					Control::Input->ClearMouse();
					return;
				}
			}
			else
				BtnNoTutorial->PlayAnimation(0);
		}

		return;
	}

	//Tutorial transition
	if( TutTransGapTimer <= 0)
	{
		TutTransGapTimer = 0;
		if( TutTransitionState == TUT_FADEIN )
		{
			TutTransAlpha += 2.0f*Control::LogicRate;
			if(TutTransAlpha >= 1.0f)
			{
				TutTransAlpha = 1.0f;
				TutTransitionState = TUT_IDLE;
			}
			SetTutorialAlpha(TutTransAlpha);
			return;
		}
		else if( TutTransitionState == TUT_FADEOUT )
		{
			TutTransAlpha -= 2.0f*Control::LogicRate;
			if(TutTransAlpha <= 0)
			{
				TutTransAlpha = 0;
				TutTransitionState = TUT_HIDDEN;
				IsTutFadingOut = false;

				IsVisible = false;

				MultiTextTutorial->Show(false);
				TutorialScene->Show(false);

				PrevCursorMode = -1;

				if( IsNextTutQueued )
				{
					IsNextTutQueued = false;
					ShowTutorial(State);	//Show next state
				}
			}
			SetTutorialAlpha(TutTransAlpha);
			return;
		}
	}
	else
	{
		TutTransGapTimer -= Control::LogicRate;
		return;
	}


	BtnScaleAngle += 100*Control::LogicRate;
	if(Arrow1->Visible)
	{
		Arrow1->SetScale(Elixir::Sin(BtnScaleAngle)*0.05f+0.50f);
	}
	if(Arrow2->Visible)
	{
		Arrow2->SetScale(Elixir::Sin(BtnScaleAngle)*0.05f+0.50f);
	}

	//Btn Interactions
	if(BtnSkip->Visible)
	{
		if(BtnSkip->Collide(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSkip->PlayAnimation(1);
			isMouseOverTutorUI = true;
			if(clicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
				BtnSkip->PlayAnimation(2);

				ShowSkipTutorialWindow();

				Control::Input->ClearMouse();
				return;
			}
		}
		else
		{
			BtnSkip->PlayAnimation(0);
		}
	}
	if(BtnContinue->IsActive)
	{
		if(BtnContinue->Collide(&pos) || !FrameBg->Collide(&pos))
		{
			if(BtnContinue->Collide(&pos))
				Cursor::SetMode(CA_HandPointy);
			BtnContinue->PlayAnimation(1);
			if(clicked)
			{
				BtnContinue->PlayAnimation(2);

				HideTutorial();
				ProfileSystem::Instance->ActiveProfile.IsTutorialShown[State] = true;

				Control::Input->ClearMouse();
			}
		}
		else
		{
			BtnContinue->PlayAnimation(0);
		}
	}
}

void Tutorial::Deactivate()
{ 
	IsActive = false;
	IsVisible = false;

	MultiTextTutorial->Show(false);
	TutorialScene->FadeOut(2.0f); 

	State = TUT_Inactive;
	ProfileSystem::Instance->ActiveProfile.TutorialState = TUT_Inactive;

	GFInstance->UnloadBufferScene(TUTORIAL_SCN);

	PrevCursorMode = -1;
}

void Tutorial::HideTutorial(bool isQuick)
{
	if(!ProfileSystem::Instance->ActiveProfile.IsInvLocked)
		GFHud->InvBoxCtrl->IsLocked = false;

	if(isQuick)
	{
		TutTransAlpha = 0;
		TutTransitionState = TUT_HIDDEN;
		IsTutFadingOut = false;

		IsVisible = false;

		MultiTextTutorial->Show(false);
		TutorialScene->Show(false);

		PrevCursorMode = -1;
	}
	else
	{
		//No gap for hiding
		TutTransGapTimer = 0;
		TutTransitionState = TUT_FADEOUT;
		IsTutFadingOut = true;
	}
}
