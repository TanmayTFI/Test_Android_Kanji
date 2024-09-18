//====================================
//DQFULP
// Hidden Object System
//====================================

#include "Application.h"
#include "Journal.h"
#include "HOScene.h"
#include "Resources.h"
#include "Util.h"
#include "Hud.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Fonts.h"
#include "GameFlow.h"
#include "InventoryBox.h"
#include "Tutorial.h"
#include "TextureFactory.h"
#include "LangDefs.h"
#include "AwardManager.h"
#include "AssetsStringTable.h"
#include "MultilineTextBox.h"

extern HudControl* GFHud;

const CRectangle  BtnRectJournalTab(199.0f,102.0f,55.0f,143.0f);
const CRectangle  BtnRectObjectivesTab(199.0f,254.0f,57.0f,141.0f);
const CRectangle  BtnRectMapTab(200.0f,400.0f,59.0f,141.0f);
const CRectangle  BtnRectCharactersTab(199.0f,547.0f,60.0f,140.0f);

const CRectangle  BtnRectClan1Btn(347.0f,12.0f,154.0f,38.0f);
const CRectangle  BtnRectClan2Btn(508.0f,12.0f,143.0f,39.0f);
const CRectangle  BtnRectClan3Btn(719.0f,14.0f,144.0f,37.0f);
const CRectangle  BtnRectClan4Btn(871.0f,12.0f,145.0f,40.0f);

const CRectangle  BtnRectLeftButton(253.0f,596.0f,74.0f,74.0f);
const CRectangle  BtnRectRightButton(1038.0f,594.0f,77.0f,74.0f);

const CRectangle  JournalAreaRect(181.0f,6.0f,960.0f,694.0f);

const CRectangle  BtnRectClose(1103.0f,22.0f,72.0f,67.0f);

int Objectives[MAX_CURRENT_OBJECTIVES] = {-1,-1,-1,-1,-1,-1};

Journal::Journal()
{

	//GFApp->lpFontDiary->setDropShadow(2, 5, 45, 0, 0, 0, 0.1f);

	JrnScene = GFInstance->LoadBufferScene(JRN_SCN);
	if (JrnScene->GetObjectByID("diarybg") != NULL && JrnScene->GetObjectByID("diarybg")->ZOrder <= 0)
		JrnScene->SetZOff(eZOrder_MapdiaryPopup);//Single time offsetting

	BtnTabJournal = JrnScene->GetObjectByID("btnjournal");
	BtnTabObjective = JrnScene->GetObjectByID("btnobjective");
	BtnTabMap = JrnScene->GetObjectByID("btnmap");
	BtnTabCharacter = JrnScene->GetObjectByID("btncharacter");

	BtnClose = JrnScene->GetObjectByID("closebtn");
	BtnPrevPage = JrnScene->GetObjectByID("BtnPrev");
	BtnNextPage = JrnScene->GetObjectByID("BtnNext");

	BtnClanCreeks = JrnScene->GetObjectByID("btnclan1");
	BtnClanSnake = JrnScene->GetObjectByID("btnclan2");
	BtnClanEagle = JrnScene->GetObjectByID("btnclan3");
	BtnClanOthers = JrnScene->GetObjectByID("btnclan4");

	ObjectiveTitle = JrnScene->GetObjectByID("objtitletext");
	//if(JrnScene->GetObjectByID("btnjournaltext") == NULL)
	{
		CObject* temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\JOURNAL.SPR", BtnTabJournal->Pos.x, BtnTabJournal->Pos.y, BtnTabJournal->ZOrder + 1);
		temp->SetUID("btnjournaltext");
		//temp->SetEnable(false);
		JrnScene->PushObject(temp);

		temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\MAP.SPR", BtnTabMap->Pos.x, BtnTabMap->Pos.y, BtnTabMap->ZOrder + 1);
		temp->SetUID("btnmaptext");
		//temp->SetEnable(false);
		JrnScene->PushObject(temp);

		temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\OBJECTIVE.SPR", BtnTabObjective->Pos.x, BtnTabObjective->Pos.y, BtnTabObjective->ZOrder + 1);
		temp->SetUID("btnobjectivetext");
		//temp->SetEnable(false);
		JrnScene->PushObject(temp);

		temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\CHARACTERS.SPR", BtnTabCharacter->Pos.x, BtnTabCharacter->Pos.y, BtnTabCharacter->ZOrder + 1);
		temp->SetUID("btncharactertext");
		//temp->SetEnable(false);
		JrnScene->PushObject(temp);

		temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\THECREEKS.SPR", BtnClanCreeks->Pos.x, BtnClanCreeks->Pos.y, BtnClanCreeks->ZOrder + 1);
		temp->SetUID("btnclan1text");
		//temp->SetEnable(false);
		JrnScene->PushObject(temp);

		temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\SNAKECLAN.SPR", BtnClanSnake->Pos.x, BtnClanSnake->Pos.y, BtnClanSnake->ZOrder + 1);
		temp->SetUID("btnclan2text");
		//temp->SetEnable(false);
		JrnScene->PushObject(temp);

		temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\EAGLECLAN.SPR", BtnClanEagle->Pos.x, BtnClanEagle->Pos.y, BtnClanEagle->ZOrder + 1);
		temp->SetUID("btnclan3text");
		//temp->SetEnable(false);
		JrnScene->PushObject(temp);

		temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\OTHERS.SPR", BtnClanOthers->Pos.x, BtnClanOthers->Pos.y, BtnClanOthers->ZOrder + 1);
		temp->SetUID("btnclan4text");
		//temp->SetEnable(false);
		JrnScene->PushObject(temp);

		temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\OBJECTIVES.SPR", ObjectiveTitle->Pos.x, ObjectiveTitle->Pos.y, ObjectiveTitle->ZOrder + 1);
		temp->SetUID("objtitletextLocalized");
		//temp->SetEnable(false);
		JrnScene->PushObject(temp);
	}

	JrnScene->Show(false);
		
	BgZOrder = JrnScene->GetObjectByID("diarybg")->ZOrder;

	GameTimeText = NULL;

	showDiaryText = false;
	textShowTimer = 0;

	isObjTabTutorActive = false;
	isTutorialActive = false;

	isCharacterPageActive = false;
	activeCharacterID = eChar1_Sophia;
	charPhoto = NULL;
	charText = NULL;

	//init elements
	float StartX = 330, StartY = 160;	
	float YSpacing = 60;
	for(int i=0; i < MAX_CURRENT_OBJECTIVES; i++)
	{
		ObjSymbols[i] = JrnScene->GetObjectByID(std::string("objicon")+MKSTR((i+1)));
		ObjSymbols[i]->SetPos(PointSystem::CreateCPoint(StartX-30,StartY + YSpacing*i,0));
		ObjSymbols[i]->SetEnable(false);

		ObjBmpTxt[i] = NULL;
	}

	maxObjectives=0;
	numCurrentObjectives = 0;
	
	CurJrnPage = NULL;
	PrevJrnPage = NULL;

	jrnTxtL = NULL;
	jrnTxtR = NULL;
	jrnTxtHeadingL = NULL;
	jrnTxtHeadingR = NULL;
	jrnImg = NULL;
	jrnImg2 = NULL;
	//Init before load!!

	journalState = JRN_IDLE;
	PopupAlpha = 0.0f;
	PopVal = 0;

	SetZ(eZOrder_MapdiaryPopup);
	
	Visible = false;
}

void Journal::UpdateJournalAndObjectives()
{
	maxObjectives = MAX_CURRENT_OBJECTIVES;
	numCurrentObjectives = 0;

	float StartX = 320, StartY = 160;	
	float YSpacing = 60;

	for(int i=0; i < maxObjectives; i++)
	{
		//Load Objectives Text
		if(ObjBmpTxt[i]==NULL)
		{
			ObjBmpTxt[i] = new CBitmapText();
			ObjBmpTxt[i]->SetZ(BgZOrder+10);
			ObjBmpTxt[i]->SetAlignment(Align_Left);
			ObjBmpTxt[i]->SetPos(PointSystem::CreateCPoint(StartX,StartY + YSpacing*i,0));
			ObjBmpTxt[i]->LoadFont(eFont_Drop);
			ObjBmpTxt[i]->SetColor(0.15f, 0.15f, 0.15f);
			ObjBmpTxt[i]->SetScale(1.0);
			ObjBmpTxt[i]->SetText("");
			ObjBmpTxt[i]->SetEnable(false);
		}

		if(i<ProfileSystem::Instance->ActiveProfile.NumObjectivesActive)
		{
			Objectives[i] = ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[i];

			if(Objectives[i] != -1)
			{
				numCurrentObjectives += 1;

				ObjBmpTxt[i]->SetEnable(true);
				ObjBmpTxt[i]->SetText(StringTable::getStr(LvLObjTexts[Objectives[i]]));			
				ObjSymbols[i]->SetEnable(true);

				bool IsObjCompleted = ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[Objectives[i]];
				if(IsObjCompleted)
				{
					ObjBmpTxt[i]->SetColor(0.5f, 0.5f, 0.5f);	//grey
					ObjSymbols[i]->PlayAnimation(1);
				}
			}
			else
			{			
				ObjSymbols[i]->SetEnable(false);
				ObjBmpTxt[i]->SetEnable(false);
			}
		}
	}

	if( ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked > ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead )//New page unlocked since last visit
	{
		ProfileSystem::Instance->ActiveProfile.JrnPageMaxRead = ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked;
		//then visit that page!
		ActiveJrnPage = ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked;
	}	//first visit or no flipping in last visit!!
	else if( ProfileSystem::Instance->ActiveProfile.JrnPageLastRead == -1 || ProfileSystem::Instance->ActiveProfile.JrnPageLastRead == ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked )
		ActiveJrnPage = ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked;
	else //flipping in last visit and no newpages unlocked!!
	{		
		//else goto last read page!!
		ActiveJrnPage = ProfileSystem::Instance->ActiveProfile.JrnPageLastRead;
	}

	if( ActiveJrnPage == PAGE_1 )
		BtnPrevPage->SetEnable(false);

	if( ActiveJrnPage == ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked )
		BtnNextPage->SetEnable(false);
}

Journal::~Journal()
{
	for(int i=0; i < MAX_CURRENT_OBJECTIVES; i++)
	{
		SAFE_DELETE(ObjBmpTxt[i]);		

		//delete ObjSymbols[i];
		ObjSymbols[i] = NULL;
	}
	/*
	JrnScene->GetObjectByID("btnjournaltext")->Show(false);
	JrnScene->GetObjectByID("btnmaptext")->Show(false);
	JrnScene->GetObjectByID("btnobjectivetext")->Show(false);
	JrnScene->GetObjectByID("btncharactertext")->Show(false);
	JrnScene->GetObjectByID("btnclan1text")->Show(false);
	JrnScene->GetObjectByID("btnclan2text")->Show(false);
	JrnScene->GetObjectByID("btnclan3text")->Show(false);
	JrnScene->GetObjectByID("objtitletextLocalized")->Show(false);
	*/

	JrnScene->Show(false);
	GFInstance->UnloadBufferScene(JRN_SCN);
	JrnScene = NULL;
	

	SAFE_DELETE(charPhoto);
	SAFE_DELETE(charText);

	SAFE_DELETE(GameTimeText);
	
	if( CurJrnPage!= NULL )
	{
		delete CurJrnPage;
		CurJrnPage = NULL;
	}
	
	if (jrnImg != NULL)
	{
		delete jrnImg;
		jrnImg = NULL;
	}

	if (jrnImg2 != NULL)
	{
		delete jrnImg2;
		jrnImg2 = NULL;
	}
	
	if( PrevJrnPage != NULL )
	{
		delete PrevJrnPage;
		PrevJrnPage = NULL;
	}

	if (jrnTxtL != NULL)
	{
		//jrnTxtL->Show(false);
		delete jrnTxtL;
		jrnTxtL = NULL;
	}

	if (jrnTxtR != NULL)
	{
		//jrnTxtR->Show(false);
		delete jrnTxtR;
		jrnTxtR = NULL;
	}

	if (jrnTxtHeadingL != NULL)
	{
		//jrnTxtHeadingL->Show(false);
		delete jrnTxtHeadingL;
		jrnTxtHeadingL = NULL;
	}
	if (jrnTxtHeadingR != NULL)
	{
		//jrnTxtHeadingR->Show(false);
		delete jrnTxtHeadingR;
		jrnTxtHeadingR = NULL;
	}

	if( GFInstance != NULL )   
	{
		if( !GFInstance->IsPartialCacheMem )
		{
			GFInstance->UnloadBufferScene(JRN_SCN);
		}
	}
}

void Journal::Required()
{
	if (showDiaryText)
	{
		if (textShowTimer < 0.5)
		{
			textShowTimer += Control::LogicRate;
		}
		else
		{
			showDiaryText = false;
			textShowTimer = 0;

			if (jrnTxtR)
				jrnTxtR->Show(true);
			if (jrnTxtL)
				jrnTxtL->Show(true);
			if (jrnTxtHeadingL)
				jrnTxtHeadingL->Show(true);
			if (jrnTxtHeadingR)
				jrnTxtHeadingR->Show(true);
			if (jrnImg)
				jrnImg->Show(true);
			if (jrnImg2)
				jrnImg2->Show(true);


			BtnTabCharacter->PlayAnimation(0);
			BtnTabObjective->PlayAnimation(0);
			BtnTabJournal->PlayAnimation(2);

			ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive = false;

			(ActiveJrnPage == PAGE_1) ? BtnPrevPage->SetEnable(false) : BtnPrevPage->SetEnable(true);
			(ActiveJrnPage == ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked) ? BtnNextPage->SetEnable(false) : BtnNextPage->SetEnable(true);
		}
	}
}



void Journal::Render(const CPoint* pCam)
{  
	if(!Visible)
		return;

	 //Draw render target object in Render as last object!!!
	if( journalState == JRN_POPUP || journalState == JRN_POPDOWN )
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

void Journal::Update()
{
	if( !GFHud->IsDiaryShowing || GFInstance->IsActivePopup )
		return;

#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
			if( (ProfileSystem::Instance->ActiveProfile.TutorialState == TUT_Inactive || !isObjTabTutorActive))
			{
				HideJournal();
			}
		}
#endif
	Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	if (BtnTabJournal->GetAnimation() == 0 || BtnTabJournal->GetAnimation() == 1)
		JrnScene->GetObjectByID("btnjournaltext")->PlayAnimation(0);
	else
		JrnScene->GetObjectByID("btnjournaltext")->PlayAnimation(1);

	if (BtnTabMap->GetAnimation() == 0 || BtnTabMap->GetAnimation() == 1)
		JrnScene->GetObjectByID("btnmaptext")->PlayAnimation(0);
	else
		JrnScene->GetObjectByID("btnmaptext")->PlayAnimation(1);

	if (BtnTabObjective->GetAnimation() == 0 || BtnTabObjective->GetAnimation() == 1)
		JrnScene->GetObjectByID("btnobjectivetext")->PlayAnimation(0);
	else
		JrnScene->GetObjectByID("btnobjectivetext")->PlayAnimation(1);

	if (BtnTabCharacter->GetAnimation() == 0 || BtnTabCharacter->GetAnimation() == 1)
		JrnScene->GetObjectByID("btncharactertext")->PlayAnimation(0);
	else
		JrnScene->GetObjectByID("btncharactertext")->PlayAnimation(1);

	if (BtnClanCreeks->GetAnimation() == 0 || BtnClanCreeks->GetAnimation() == 1)
		JrnScene->GetObjectByID("btnclan1text")->PlayAnimation(0);
	else
		JrnScene->GetObjectByID("btnclan1text")->PlayAnimation(1);
	if (BtnClanSnake->GetAnimation() == 0 || BtnClanSnake->GetAnimation() == 1)
		JrnScene->GetObjectByID("btnclan2text")->PlayAnimation(0);
	else
		JrnScene->GetObjectByID("btnclan2text")->PlayAnimation(1);
	if (BtnClanEagle->GetAnimation() == 0 || BtnClanEagle->GetAnimation() == 1)
		JrnScene->GetObjectByID("btnclan3text")->PlayAnimation(0);
	else
		JrnScene->GetObjectByID("btnclan3text")->PlayAnimation(1);
	if (BtnClanOthers->GetAnimation() == 0 || BtnClanOthers->GetAnimation() == 1)
		JrnScene->GetObjectByID("btnclan4text")->PlayAnimation(0);
	else
		JrnScene->GetObjectByID("btnclan4text")->PlayAnimation(1);
	
	JrnScene->GetObjectByID("btnclan1text")->SetEnable(BtnClanCreeks->IsActive);
	JrnScene->GetObjectByID("btnclan2text")->SetEnable(BtnClanSnake->IsActive);
	JrnScene->GetObjectByID("btnclan3text")->SetEnable(BtnClanEagle->IsActive);
	JrnScene->GetObjectByID("btnclan4text")->SetEnable(BtnClanOthers->IsActive);

	JrnScene->GetObjectByID("objtitletextLocalized")->SetEnable(ObjectiveTitle->IsActive);
	
	switch (journalState)
	{
		case JRN_POPUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);
			if( PopVal >= 1.0f )
			{
				PopupAlpha = 1.0f;

				GameTimeText->Show(true);

				JrnScene->Show(true);
				if( !ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive )			
				{
					CurJrnPage->Show(true);
					for(int i=0; i < numCurrentObjectives; i++)
					{					
						ObjSymbols[i]->SetEnable(false);
						ObjBmpTxt[i]->SetEnable(false);
					}
				}
				else
				{
					ObjectiveTitle->SetEnable(true);
					for(int i=0; i < numCurrentObjectives; i++)
					{
						ObjSymbols[i]->SetEnable(true);
						ObjBmpTxt[i]->SetEnable(true);
					}
				}

				journalState = JRN_POPPED;
				//TODO: Testing
				if (jrnTxtL != NULL)
				{
					jrnTxtL->Show(true);
				}
				if (jrnTxtR != NULL)
				{
					jrnTxtR->Show(true);
				}
				if (jrnTxtHeadingL != NULL)
				{
					jrnTxtHeadingL->Show(true);
				}
				if (jrnTxtHeadingR != NULL)
				{
					jrnTxtHeadingR->Show(true);
				}
				if (jrnImg != NULL)
				{
					jrnImg->Show(true);
				}
				if (jrnImg2 != NULL)
				{
					jrnImg2->Show(true);
				}

				/*
				if (!JrnScene->GetObjectByID("btnjournaltext")->IsActive)
					JrnScene->GetObjectByID("btnjournaltext")->SetEnable(true);
				if (!JrnScene->GetObjectByID("btnmaptext")->IsActive)
					JrnScene->GetObjectByID("btnmaptext")->SetEnable(true);
				if (!JrnScene->GetObjectByID("btnobjectivetext")->IsActive)
					JrnScene->GetObjectByID("btnobjectivetext")->SetEnable(true);
				if (!JrnScene->GetObjectByID("btncharactertext")->IsActive)
					JrnScene->GetObjectByID("btncharactertext")->SetEnable(true);			
					*/

				if(GFHud->Tutor != NULL && GFHud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT04_ObjectivesTab] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
				{
					isTutorialActive = true;
					isObjTabTutorActive = true;
					GFHud->Tutor->ShowTutorial(TUT04_ObjectivesTab);
				}
			}
			GFInstance->UpdateBlurScene(PopupAlpha);
			return;
		break;
		case JRN_POPDOWN:
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			GFInstance->UpdateBlurScene(PopupAlpha);
			if( PopupAlpha <= 0.113f )
			{
				PopupAlpha = 0.0f;
				GFInstance->TurnOffBlurScene();
				journalState = JRN_IDLE;
				GFHud->IsDiaryShowing = false;
			}
			return;
		break;

	default:
		break;
	}

	if( !isTutorialActive && BtnRectMapTab.Intersection(&pos) )
	{
		Cursor::SetMode(CA_HandPointy);
		BtnTabMap->PlayAnimation(1);
		if(clicked)
		{
			Control::Audio->QuickLoadAndPlaySFX(aSFXDiaTabClick);

			BtnTabMap->PlayAnimation(0);

			GFHud->ShowMapFromJournal();
			return;
		}
	}
	else
		BtnTabMap->PlayAnimation(0);

	if(!isCharacterPageActive)
	{
		//Character Page
		if( BtnRectCharactersTab.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnTabCharacter->PlayAnimation(1);
			if(clicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaTabClick);

				HideObjectivesPage();
				HideJournalPage();

				BtnTabJournal->PlayAnimation(0); 

				BtnTabObjective->PlayAnimation(0);			

				isCharacterPageActive = true;
				ShowCharacterPage();
			}
		}
		else
			BtnTabCharacter->PlayAnimation(0);
	}
	else
	{
		UpdateCharacterPage(pos);
	}


	//Objective Page
	if(isCharacterPageActive || (!isCharacterPageActive && ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive))
	{
		if( BtnRectJournalTab.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnTabJournal->PlayAnimation(1); 
			if(clicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaTabClick);

				ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive = false;

				HideCharacterPage();
				HideObjectivesPage();
				ShowJournalPage();
			} 
		}
		else
		{
			BtnTabJournal->PlayAnimation(0);
		}
	}

	if( isCharacterPageActive || (!isCharacterPageActive && !ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive) )
	{
		if(!isCharacterPageActive)
			UpdateJournalPage(pos);

		if( BtnRectObjectivesTab.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnTabObjective->PlayAnimation(1);
			if(clicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaTabClick);

				HideCharacterPage();
				ShowObjectivesPage();

				ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive = true;
				HideJournalPage();
			}
		}
		else
			BtnTabObjective->PlayAnimation(0);
	}

	if( (ProfileSystem::Instance->ActiveProfile.TutorialState == TUT_Inactive || !isObjTabTutorActive) && (BtnRectClose.Intersection(&pos) || !JournalAreaRect.Intersection(&pos)) )
	{
		Cursor::SetMode(CA_HandPointy);
		BtnClose->PlayAnimation(1);
		if(clicked)
		{
			BtnClose->PlayAnimation(2);
			//SendMessage(ControlExit, ID, 0);//go to next HO
			HideJournal();
		}
	}
	else
		BtnClose->PlayAnimation(0);
}

void Journal::UpdateCharacterPage(CPoint pos)
{
	//Character Page
	bool clicked = Control::Input->LBclicked();
	if(BtnPrevPage->IsActive && BtnRectLeftButton.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BtnPrevPage->PlayAnimation(1);
		if(clicked)
		{
			BtnPrevPage->PlayAnimation(2);
			Control::Audio->QuickLoadAndPlaySFX(aSFXDiaPageTurn);
			GetPreviousUnlockedCharacterID();
			SetupCharacterPage();
			loadCharacter(activeCharacterID);
		}
	}
	else
		BtnPrevPage->PlayAnimation(0);

	if(BtnNextPage->IsActive && BtnRectRightButton.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BtnNextPage->PlayAnimation(1);
		if(clicked)
		{
			BtnNextPage->PlayAnimation(2);
			Control::Audio->QuickLoadAndPlaySFX(aSFXDiaPageTurn);
			GetNextUnlockedCharacterID();
			SetupCharacterPage();
			loadCharacter(activeCharacterID);
		}
	}
	else
		BtnNextPage->PlayAnimation(0);

	
	if(BtnClanCreeks->GetAnimation()!=2)
		BtnClanCreeks->PlayAnimation(0);
	if(BtnClanSnake->GetAnimation()!=2)
		BtnClanSnake->PlayAnimation(0);
	if(BtnClanEagle->GetAnimation()!=2)
		BtnClanEagle->PlayAnimation(0);
	if(BtnClanOthers->GetAnimation()!=2)
		BtnClanOthers->PlayAnimation(0);
	//Check for Clan Click
	if(BtnClanCreeks->IsActive && BtnClanCreeks->GetAnimation()!=2 && BtnClanCreeks->Collide(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BtnClanCreeks->PlayAnimation(1);
		if(clicked)
		{
			GetUnlockedCharacterInClan(0);
			SetupCharacterPage();
			loadCharacter(activeCharacterID);
		}
	}
	else if(BtnClanSnake->IsActive && BtnClanSnake->GetAnimation()!=2 && BtnClanSnake->Collide(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BtnClanSnake->PlayAnimation(1);
		if(clicked)
		{
			GetUnlockedCharacterInClan(1);
			SetupCharacterPage();
			loadCharacter(activeCharacterID);
		}
	}
	else if(BtnClanEagle->IsActive && BtnClanEagle->GetAnimation()!=2 && BtnClanEagle->Collide(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BtnClanEagle->PlayAnimation(1);
		if(clicked)
		{
			GetUnlockedCharacterInClan(2);
			SetupCharacterPage();
			loadCharacter(activeCharacterID);
		}
	}
	else if(BtnClanOthers->IsActive && BtnClanOthers->GetAnimation()!=2 && BtnClanOthers->Collide(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BtnClanOthers->PlayAnimation(1);
		if(clicked)
		{
			GetUnlockedCharacterInClan(3);
			SetupCharacterPage();
			loadCharacter(activeCharacterID);
		}
	}

	//For debug version only
#ifdef DEBUG
	if (KInput::isPressed(K_VK_A))
	{
		AssetsStringTable::readAssetsStringTable();
		AssetsStringTable::ReadBNRtextSizes();
		SetupCharacterPage();
		loadCharacter(activeCharacterID);
	}
#endif
}


void  Journal::UpdateJournalPage(CPoint pos)
{
	//Journal Page
	bool clicked = Control::Input->LBclicked();
	if(BtnPrevPage->IsActive && 
		BtnRectLeftButton.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BtnPrevPage->PlayAnimation(1);
		if(clicked)
		{
			BtnPrevPage->PlayAnimation(2);
				
			if( ActiveJrnPage > 0 )
			{
				Control::Audio->PlaySample(aSFXClick);
				ActiveJrnPage--;

				if( ActiveJrnPage == PAGE_1 )
				{
					BtnPrevPage->SetEnable(false);
					BtnNextPage->SetEnable(true);
				}
				else
				{
					BtnNextPage->SetEnable(true);
				}

				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaPageTurn);
				LoadJrnPage();	//loads new page content!!
			}
		}
	}
	else
		BtnPrevPage->PlayAnimation(0);

	if(BtnNextPage->IsActive && 
		BtnRectRightButton.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BtnNextPage->PlayAnimation(1);
		if(clicked)
		{
			BtnNextPage->PlayAnimation(2);

			if( ActiveJrnPage < ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked && 
					ActiveJrnPage < (MAX_JRN_PAGES-1) )
			{
				Control::Audio->PlaySample(aSFXClick);
				ActiveJrnPage++;

				if( ActiveJrnPage == ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked )
				{
					BtnNextPage->SetEnable(false);
					BtnPrevPage->SetEnable(true);
				}
				else
				{
					BtnPrevPage->SetEnable(true);
				}
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaPageTurn);

				LoadJrnPage();	
			}
		}
	}
	else
		BtnNextPage->PlayAnimation(0);


	//For debug version only
#ifdef DEBUG
	if (KInput::isPressed(K_VK_A))
	{
		AssetsStringTable::readAssetsStringTable();
		AssetsStringTable::ReadBNRtextSizes();
		LoadJrnPage();
	}
#endif
}

float Journal::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void Journal::loadCharacter(int charID)
{
	SAFE_DELETE(charPhoto);
	SAFE_DELETE(charText);

	ProfileSystem::Instance->ActiveProfile.lastViewedCharacter = charID;
	
	charPhoto = new CObject();
	std::string filepath =  "MAPDIA\\CHAR\\CHAR"+MKSTR(charID)+".SPR";
	charPhoto->LoadInit(filepath, 480, 364, BgZOrder+2);

	ColorRGB color = { 0.1015f, 0.1718f, 0.2656f };
	charText = new MultilineTextBox(AssetsStringTable::getString(("CHAR"+ MKSTR(charID)).c_str()), 700, 100, BgZOrder + 3, color, eFont_Diary, 25);
	charText->LineTexts[0]->LoadFont(eFont_26);

	//charText = new CObject();
	filepath =  "MAPDIA\\CHAR\\CHARTEXT"+MKSTR(charID)+".SPR";
	//charText->LoadInit(filepath, 725, 140, BgZOrder+2);
	
	charPhoto->FadeIn(4.0f);
	//charText->FadeIn(4.0f);
}

void Journal::LoadJrnPage(bool IsInitialLoading)
{
	//Show CurJrnPage number
	if( (ActiveJrnPage+1) >= 10 )
	{
	}
	else
	{
	}
	
	if( PrevJrnPage != NULL )
	{
		PrevJrnPage->Show(false);
		delete PrevJrnPage; 
	}
	PrevJrnPage = CurJrnPage;

	if( PrevJrnPage != NULL )
		PrevJrnPage->SetZ(BgZOrder+3);//need to fadeout above current page
	
	CurJrnPage = new CObject();

	if (jrnImg != NULL)
	{
		//jrnImg->Show(false);
		delete jrnImg;
		jrnImg = NULL;
	}
	jrnImg = new CObject();
	if (jrnImg2 != NULL)
	{
		//jrnImg->Show(false);
		delete jrnImg2;
		jrnImg2 = NULL;
	}
	jrnImg2 = new CObject();
	int PageToShow = ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ActiveJrnPage];

	ColorRGB diarytextColor = { 0.1015f, 0.1718f, 0.2656f };
	int _fontSize = -1;
	int _lineSpacing = 25;

	if (jrnTxtL != NULL)
	{
		//jrnTxtL->Show(false);
		delete jrnTxtL;
		jrnTxtL = NULL;
	}

	if (jrnTxtR != NULL)
	{
		//jrnTxtR->Show(false);
		delete jrnTxtR;
		jrnTxtR = NULL;
	}

	if (jrnTxtHeadingL != NULL)
	{
		//jrnTxtHeadingL->Show(false);
		delete jrnTxtHeadingL;
		jrnTxtHeadingL = NULL;
	}
	if (jrnTxtHeadingR != NULL)
	{
		//jrnTxtHeadingR->Show(false);
		delete jrnTxtHeadingR;
		jrnTxtHeadingR = NULL;
	}
	
	
	switch( PageToShow )
	{
		default:
		//eJrn_L1P1_MomsThoughts
		case PAGE_1:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P1.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));

				   _fontSize = AssetsStringTable::GetFont("jrnP1");
				   if(_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP1");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP1"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if(journalState != JRN_POPPED)
						jrnTxtL->Show(false);

				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP1Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();

				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP1Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */

				   jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\DIARY\\5YEARS.SPR", 950, 225, BgZOrder + 20);
				   jrnImg->SetUID("jrnImg");
				   //jrnImg->SetRotate();
				   jrnImg->SetScale(0.7f);

				   if (journalState != JRN_POPPED)
				   {
					   jrnTxtHeadingL->Show(false);
					   jrnImg->Show(false);
				   }
				   break;

		//eJrn_L1P2_VacationBegins
		case PAGE_2:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P2.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));

				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP2Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP2Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP2Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP2Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\DIARY\\PHOTOS.SPR", 930, 410, BgZOrder + 20);
				   jrnImg->SetUID("jrnImg");
				   //jrnImg->SetRotate();
				   jrnImg->SetScale(0.7f);

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP2Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
				   {
					   jrnTxtL->Show(false);
					   jrnImg->Show(false);
				   }

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP2Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;

		//eJrn_L1P3_SophiaIsTakenAway
		case PAGE_3:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P3.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP3Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP3Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP3Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP3Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP3Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP3Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;

		//eJrn_L1P4_KidPainting
		case PAGE_4:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P4.SPR");
				  CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				  /*
				  jrnTxtHeadingL = new CBitmapText();
				  jrnTxtHeadingL->SetZ(BgZOrder + 10);
				  jrnTxtHeadingL->LoadFont(eFont_Italic24);
				  jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP4Heading"));
				  jrnTxtHeadingL->SetAlignment(Align_Center);
				  jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				  jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				  */
				  jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP4Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				  jrnTxtHeadingL->CenterText();
				  if (journalState != JRN_POPPED)
					  jrnTxtHeadingL->Show(false);

				  //Left Page
				  _fontSize = AssetsStringTable::GetFont("jrnP4Left");
				  if (_fontSize != -1)
					  GFApp->lpFontDiary->setHeightPix(_fontSize);
				  else
					  GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				  _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP4Left");
				  if (_lineSpacing == -1)
					  _lineSpacing = 25;

				  jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP4Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				  if (journalState != JRN_POPPED)
					  jrnTxtL->Show(false);

				  //Right Page
				  jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP4Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				  if (journalState != JRN_POPPED)
					  jrnTxtR->Show(false);
				   break;
		
	    //eJrn_L1P5_CursedMansion
		case PAGE_5:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P5.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));

				   _fontSize = AssetsStringTable::GetFont("jrnP5");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP5");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;
				   
				   jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S1\\CREEKMANSION_POP.SPR", 470, 165, BgZOrder + 20);
				   jrnImg->SetUID("jrnImg");
				   //jrnImg->SetRotate();
				   jrnImg->SetScale(0.6f);

				   jrnImg2->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S2\\LOVEBUILD_POP.SPR", 885, 495, BgZOrder + 20);
				   jrnImg2->SetUID("jrnImg2");
				   //jrnImg->SetRotate();
				   jrnImg2->SetScale(0.5f);

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP5"), 285, 280, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
				   {
					   jrnTxtL->Show(false);
					   jrnImg->Show(false);
					   jrnImg2->Show(false);
				   }

				   
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP5Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 270);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP5Heading"), 470, 270, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();

				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);
				   break;
		
		//eJrn_L1P6_ForgiveMeAnnMary
		case PAGE_6:
				   CurJrnPage->LoadSprite("MAPDIA\\PAG\\P6.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP6Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 100);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP6Heading"), 470, 100, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();

				   jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S3\\FORGIVE.SPR", 895, 450, BgZOrder + 20);
				   jrnImg->SetUID("jrnImg");
				   //jrnImg->SetRotate();
				   jrnImg->SetScale(0.4f);

				   if (journalState != JRN_POPPED)
				   {
					   jrnTxtHeadingL->Show(false);
					   jrnImg->Show(false);
				   }

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP6");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP6");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP6"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);
				   break;

		//eJrn_L1P7_MooppanFlyer
		case PAGE_7:
				   CurJrnPage->LoadSprite("MAPDIA\\PAG\\P7.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));

				   _fontSize = AssetsStringTable::GetFont("jrnP7");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP7");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP7"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);

				   jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S3\\P1SHELF\\MOOPANLETTER.SPR", 720, 115, BgZOrder + 20);
				   jrnImg->SetUID("jrnImg");
				   jrnImg->SetRotate(-5);
				   jrnImg->SetScale(0.7f);

				   if (journalState != JRN_POPPED)
				   {
					   jrnTxtL->Show(false);
					   jrnImg->Show(false);
				   }
				   					 /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP7Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP7Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();

				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);
				  break;

		//eJrn_L1P8_CatAttack
		case PAGE_8:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P8.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));

				   _fontSize = AssetsStringTable::GetFont("jrnP8");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP8");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP8"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP8Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP8Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();

				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);
				   break;

		//eJrn_L1P9_RajaFoyerJamesTaken
		case PAGE_9:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P9.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));

				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP9Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP9Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP9Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP9Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP9Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP9Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				  break;

		//eJrn_L1P10_CreekEscaped
		case PAGE_10:
					CurJrnPage->LoadSprite("MAPDIA\\PAG\\P10.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP10Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP10Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP10Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP10Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP10Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP10Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				  break;

		//eJrn_L1P11_CompanyWarningNotice
		case PAGE_11:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P11.SPR");
				  CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				  /*
				  jrnTxtHeadingL = new CBitmapText();
				  jrnTxtHeadingL->SetZ(BgZOrder + 10);
				  jrnTxtHeadingL->LoadFont(eFont_Italic24);
				  jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP11Heading"));
				  jrnTxtHeadingL->SetAlignment(Align_Center);
				  jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				  jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				  */
				  jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP11Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				  jrnTxtHeadingL->CenterText();
				  if (journalState != JRN_POPPED)
					  jrnTxtHeadingL->Show(false);

				  //Left Page
				  _fontSize = AssetsStringTable::GetFont("jrnP11");
				  if (_fontSize != -1)
					  GFApp->lpFontDiary->setHeightPix(_fontSize);
				  else
					  GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				  _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP11");
				  if (_lineSpacing == -1)
					  _lineSpacing = 25;

				  jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP11"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);

				  jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S4\\CREEKLETTER.SPR", 890, 360, BgZOrder + 20);
				  jrnImg->SetUID("jrnImg");
				  jrnImg->SetScale(0.6f);

				  if (journalState != JRN_POPPED)
				  {
					  jrnTxtL->Show(false);
					  jrnImg->Show(false);
				  }
				  break;

	    //eJrn_L1P12_CreekDiaryNotes
	    case PAGE_12:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P12.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP12Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 200);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP12Heading"), 470, 200, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP12");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP12");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP12"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S4\\DIARYTEXT.SPR", 900, 340, BgZOrder + 20);
				   jrnImg->SetUID("jrnImg");
				   jrnImg->SetRotate(-18);
				   jrnImg->SetScale(0.4f);

				   if (journalState != JRN_POPPED)
				   {
					   jrnTxtL->Show(false);
					   jrnImg->Show(false);
				   }
				   break;

	    //eJrn_L1P13_LasyaPush
	    case PAGE_13:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P13.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP13Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP13Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP13");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP13");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP13"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);
				   break;

	    //eJrn_L1P14_AfterLasyaAttack
	    case PAGE_14:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P14.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP14Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP14Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP14");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP14");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP14"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);
				   break;

	    //eJrn_L1P15_AfterTunnelblast
	    case PAGE_15:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P15.SPR");
				  CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				  /*
				  jrnTxtHeadingL = new CBitmapText();
				  jrnTxtHeadingL->SetZ(BgZOrder + 10);
				  jrnTxtHeadingL->LoadFont(eFont_Italic24);
				  jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP15Heading"));
				  jrnTxtHeadingL->SetAlignment(Align_Center);
				  jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				  jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				  */
				  jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP15Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				  jrnTxtHeadingL->CenterText();
				  if (journalState != JRN_POPPED)
					  jrnTxtHeadingL->Show(false);

				  //Left Page
				  _fontSize = AssetsStringTable::GetFont("jrnP15");
				  if (_fontSize != -1)
					  GFApp->lpFontDiary->setHeightPix(_fontSize);
				  else
					  GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				  _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP15");
				  if (_lineSpacing == -1)
					  _lineSpacing = 25;

				  jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP15"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				  if (journalState != JRN_POPPED)
					  jrnTxtL->Show(false);
				  break;

		//eJrn_L1P16_DakartaNewspaper
	    case PAGE_16:CurJrnPage->LoadSprite("MAPDIA\\PAG\\P16.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(718,361,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP16Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP16Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP16Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP16Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP16Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP16Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;

		/********************************  Level 2  *********************************/
		//eJrn_L2P1_BoatAttackReflection,
		case PAGE_17:CurJrnPage->LoadSprite("MAPDIA\\PAGL2\\P1.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP17HeadingL"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP17HeadingL"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);
				   /*
				   jrnTxtHeadingR = new CBitmapText();
				   jrnTxtHeadingR->SetZ(BgZOrder + 10);
				   jrnTxtHeadingR->LoadFont(eFont_Italic24);
				   jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP17HeadingR"));
				   jrnTxtHeadingR->SetAlignment(Align_Center);
				   jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP17HeadingR"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingR->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingR->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP17Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP17Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP17Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP17Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;
		//eJrn_L2P2_ReachedNarara,
		case PAGE_18:CurJrnPage->LoadSprite("MAPDIA\\PAGL2\\P2.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP18Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP18Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP18");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP18");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP18"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);
				   break;
		//eJrn_L2P3_SnakeTempleStorm,
		case PAGE_19:CurJrnPage->LoadSprite("MAPDIA\\PAGL2\\P3.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP19Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP19Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP19Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP19Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP19Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP19Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;
		//eJrn_L2P4_MooppanMeet,
		case PAGE_20:CurJrnPage->LoadSprite("MAPDIA\\PAGL2\\P4.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP20Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP20Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP20Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP20Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP20Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP20Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;
		//eJrn_L2P5_CrystalBallSophia,
		case PAGE_21:CurJrnPage->LoadSprite("MAPDIA\\PAGL2\\P5.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP21Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP21Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP21Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP21Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP21Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP21Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;
		//eJrn_L2P6_LasyaAttacksCave,
		case PAGE_22:CurJrnPage->LoadSprite("MAPDIA\\PAGL2\\P6.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP22Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP22Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP22Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP22Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP22Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP22Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;
		//eJrn_L2P7_MeetTheSoldiers,
		case PAGE_23:CurJrnPage->LoadSprite("MAPDIA\\PAGL2\\P7.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP23Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP23Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP23Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP23Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP23Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP23Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;
		//eJrn_L2P8_GrandpaMurdered,
		case PAGE_24:CurJrnPage->LoadSprite("MAPDIA\\PAGL2\\P8.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP24Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP24Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP24Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP24Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP24Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP24Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;
		//eJrn_L2P9_PastMemories
		case PAGE_25:CurJrnPage->LoadSprite("MAPDIA\\PAGL2\\P9.SPR");
				   CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				   /*
				   jrnTxtHeadingL = new CBitmapText();
				   jrnTxtHeadingL->SetZ(BgZOrder + 10);
				   jrnTxtHeadingL->LoadFont(eFont_Italic24);
				   jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP25Heading"));
				   jrnTxtHeadingL->SetAlignment(Align_Center);
				   jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				   jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 300);
				   */
				   jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP25Heading"), 470, 290, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				   jrnTxtHeadingL->CenterText();
				   if (journalState != JRN_POPPED)
					   jrnTxtHeadingL->Show(false);

				   //Left Page
				   _fontSize = AssetsStringTable::GetFont("jrnP25Left");
				   if (_fontSize != -1)
					   GFApp->lpFontDiary->setHeightPix(_fontSize);
				   else
					   GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				   _lineSpacing = AssetsStringTable::GetLineSpacing("jrnP25Left");
				   if (_lineSpacing == -1)
					   _lineSpacing = 25;

				   jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP25Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtL->Show(false);

				   //Right Page
				   jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP25Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				   if (journalState != JRN_POPPED)
					   jrnTxtR->Show(false);
				   break;

		/********************************  Level 3  *********************************/
		case PAGE_26:CurJrnPage->LoadSprite("MAPDIA\\PAGL3\\P1.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP26HeadingL"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP26HeadingL"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP26HeadingR"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 255);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP26HeadingR"), 900, 255, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP26Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP26Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP26Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP26Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_27:CurJrnPage->LoadSprite("MAPDIA\\PAGL3\\P2.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP27Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP27Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP27Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP27Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP27Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP27Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_28:CurJrnPage->LoadSprite("MAPDIA\\PAGL3\\P3.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP28Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP28Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP28Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP28Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP28Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP28Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_29:CurJrnPage->LoadSprite("MAPDIA\\PAGL3\\P4.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP29Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP29Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP29");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP29");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP29"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);
			break;
		case PAGE_30:CurJrnPage->LoadSprite("MAPDIA\\PAGL3\\P5.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP30Heading"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 225);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP30Heading"), 900, 225, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			_fontSize = AssetsStringTable::GetFont("jrnP30");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP30");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP30"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_31:CurJrnPage->LoadSprite("MAPDIA\\PAGL3\\P6.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP31Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP31Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP31Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP31Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP31Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP31Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;

		/********************************  Level 4  *********************************/
		case PAGE_32:CurJrnPage->LoadSprite("MAPDIA\\PAGL4\\P1.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP32Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP32Heading"), 470, 62, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP32Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP32Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP32Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP32Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_33:CurJrnPage->LoadSprite("MAPDIA\\PAGL4\\P2.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP33HeadingL"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 265);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP33HeadingL"), 470, 265, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP33HeadingR"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP33HeadingR"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP33Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP33Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP33Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP33Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_34:CurJrnPage->LoadSprite("MAPDIA\\PAGL4\\P3.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP34Heading"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 260);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP34Heading"), 900, 260, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP34");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP34");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP34"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_35:CurJrnPage->LoadSprite("MAPDIA\\PAGL4\\P4.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP35Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 300);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP35Heading"), 470, 300, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP35Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP35Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP35Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP35Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;

		/********************************  Level 5  *********************************/
		case PAGE_36:CurJrnPage->LoadSprite("MAPDIA\\PAGL5\\P1.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP36Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP36Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP36Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP36Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP36Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP36Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_37:CurJrnPage->LoadSprite("MAPDIA\\PAGL5\\P2.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP37Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP37Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP37");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP37");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP37"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);
			break;
		case PAGE_38:CurJrnPage->LoadSprite("MAPDIA\\PAGL5\\P3.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP38Heading"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP38Heading"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP38");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP38");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP38"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_39:CurJrnPage->LoadSprite("MAPDIA\\PAGL5\\P4.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP39Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP39Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP39Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP39Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP39Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP39Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_40:CurJrnPage->LoadSprite("MAPDIA\\PAGL5\\P5.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP40Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP40Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP40Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP40Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP40Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP40Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_41:CurJrnPage->LoadSprite("MAPDIA\\PAGL5\\P6.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP41Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP41Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP41");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP41");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP41"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);
			break;
		case PAGE_42:CurJrnPage->LoadSprite("MAPDIA\\PAGL5\\P7.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP42Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 370);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP42Heading"), 470, 370, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP42Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP42Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP42Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP42Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_43:CurJrnPage->LoadSprite("MAPDIA\\PAGL5\\P8.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP43Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 390);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP43Heading"), 470, 390, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP43");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP43");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP43"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);
			break;
		case PAGE_44:CurJrnPage->LoadSprite("MAPDIA\\PAGL5\\P9.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP44Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP44Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP44Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP44Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP44Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP44Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;

		/********************************  Level 6  *********************************/
		case PAGE_45:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P1.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP45Heading"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP45Heading"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP45");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP45");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP45"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);

			jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\AnanthaOurSavior.SPR", 490, 420, BgZOrder + 20);
			jrnImg->SetUID("jrnImg");
			jrnImg->SetRotate(10);
			jrnImg->SetScale(0.69f);

			if (journalState != JRN_POPPED)
			{
				jrnTxtR->Show(false);
				jrnImg->Show(false);
			}
				
			break;
		case PAGE_46:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P2.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP46Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 280);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP46Heading"), 470, 280, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP46Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP46Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP46Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP46Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_47:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P3.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP47Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP47Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP47Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP47Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP47Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP47Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_48:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P4.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP48Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP48Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP48");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP48");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP48"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\NAGINTEXT.SPR", 900, 350, BgZOrder + 20);
			jrnImg->SetUID("jrnImg");
			jrnImg->SetRotate(-8);
			jrnImg->SetScale(0.75f);

			if (journalState != JRN_POPPED)
			{
				jrnTxtL->Show(false);
				jrnImg->Show(false);
			}
			break;
		case PAGE_49:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P5.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP49Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 275);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP49Heading"), 470, 275, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP49Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP49Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP49Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP49Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_50:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P6.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP50Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP50Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP50");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP50");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP50"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\AMEEHATEXTZOOM.SPR", 875, 345, BgZOrder + 20);
			jrnImg->SetUID("jrnImg");
			jrnImg->SetRotate(-9);
			jrnImg->SetScale(0.75f);

			if (journalState != JRN_POPPED)
			{
				jrnTxtL->Show(false);
				jrnImg->Show(false);
			}
			break;
		case PAGE_51:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P7.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP51Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP51Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP51");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP51");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP51"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\ANANTHATEXT.SPR", 865, 440, BgZOrder + 20);
			jrnImg->SetUID("jrnImg");
			jrnImg->SetRotate(-15);
			jrnImg->SetScale(0.75f);

			if (journalState != JRN_POPPED)
			{
				jrnTxtL->Show(false);
				jrnImg->Show(false);
			}
			break;
		case PAGE_52:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P8.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP52HeadingL"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP52HeadingL"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP52HeadingR"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP52HeadingR"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP52Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP52Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP52Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP52Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_53:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P9.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP53Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP53Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP53");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP53");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP53"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\MAGICPOWDER.SPR", 890, 360, BgZOrder + 20);
			jrnImg->SetUID("jrnImg");
			jrnImg->SetRotate(-12);
			jrnImg->SetScale(0.5f);

			if (journalState != JRN_POPPED)
			{
				jrnTxtL->Show(false);
				jrnImg->Show(false);
			}
			break;
		case PAGE_54:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P10.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP54Heading"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP54Heading"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP54");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP54");
			if (_lineSpacing == -1)
				_lineSpacing = 25;
			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP54"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_55:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P11.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP55Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP55Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP55");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP55");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP55"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);
			break;
		case PAGE_56:CurJrnPage->LoadSprite("MAPDIA\\PAGL6\\P12.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP56Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP56Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP56Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP56Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP56Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP56Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;

		/********************************  Level 7  *********************************/
		case PAGE_57:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P1.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP57HeadingL"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP57HeadingL"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP57HeadingR"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP57HeadingR"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP57Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP57Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP57Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP57Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_58:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P2.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP58Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP58Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP58");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP58");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP58"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L7\\AMEEHALETTER.SPR", 885, 350, BgZOrder + 20);
			jrnImg->SetUID("jrnImg");
			jrnImg->SetRotate(-8);
			jrnImg->SetScale(0.75f);

			if (journalState != JRN_POPPED)
			{
				jrnTxtL->Show(false);
				jrnImg->Show(false);
			}
			break;
		case PAGE_59:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P3.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP59Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP59Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP59");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP59");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP59"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);
			break;
		case PAGE_60:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P4.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP60Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP60Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP60");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP60");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP60"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L7\\FLOWERCHART.SPR", 885, 425, BgZOrder + 20);
			jrnImg->SetUID("jrnImg");
			jrnImg->SetRotate(-12);
			jrnImg->SetScale(0.5f);

			if (journalState != JRN_POPPED)
			{
				jrnTxtL->Show(false);
				jrnImg->Show(false);
			}
			break;
		case PAGE_61:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P5.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP61Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP61Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP61");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP61");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP61"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L7\\HAVENTEXT.SPR", 900, 350, BgZOrder + 20);
			jrnImg->SetUID("jrnImg");
			jrnImg->SetRotate(-98);
			jrnImg->SetScale(0.75f);

			if (journalState != JRN_POPPED)
			{
				jrnTxtL->Show(false);
				jrnImg->Show(false);
			}
			break;
		case PAGE_62:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P6.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP62Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 310);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP62Heading"), 470, 310, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP62Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP62Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP62Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP62Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_63:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P7.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP63Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP63Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP63Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP63Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP63Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP63Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_64:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P8.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP64HeadingL"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP64HeadingL"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP64HeadingR"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP64HeadingR"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP64Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP64Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP64Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP64Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_65:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P9.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP65Heading"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP65Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP65Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP65Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP65Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP65Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;
		case PAGE_66:CurJrnPage->LoadSprite("MAPDIA\\PAGL7\\P10.SPR");
			CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
			/*
			jrnTxtHeadingL = new CBitmapText();
			jrnTxtHeadingL->SetZ(BgZOrder + 10);
			jrnTxtHeadingL->LoadFont(eFont_Italic24);
			jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP66HeadingL"));
			jrnTxtHeadingL->SetAlignment(Align_Center);
			jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 245);
			*/
			jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP66HeadingL"), 470, 245, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingL->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingL->Show(false);
			/*
			jrnTxtHeadingR = new CBitmapText();
			jrnTxtHeadingR->SetZ(BgZOrder + 10);
			jrnTxtHeadingR->LoadFont(eFont_Italic24);
			jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP66HeadingR"));
			jrnTxtHeadingR->SetAlignment(Align_Center);
			jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
			jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 420);
			*/
			jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP66HeadingR"), 900, 420, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
			jrnTxtHeadingR->CenterText();
			if (journalState != JRN_POPPED)
				jrnTxtHeadingR->Show(false);

			//Left Page
			_fontSize = AssetsStringTable::GetFont("jrnP66Left");
			if (_fontSize != -1)
				GFApp->lpFontDiary->setHeightPix(_fontSize);
			else
				GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

			_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP66Left");
			if (_lineSpacing == -1)
				_lineSpacing = 25;

			jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP66Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtL->Show(false);

			//Right Page
			jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP66Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
			if (journalState != JRN_POPPED)
				jrnTxtR->Show(false);
			break;

		/********************************  Level 8  *********************************/
		//eJrn_L8P1_ShukraVideoEnd,
		case PAGE_67:CurJrnPage->LoadSprite("MAPDIA\\PAGL8\\P1.SPR");
				CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				/*
				jrnTxtHeadingL = new CBitmapText();
				jrnTxtHeadingL->SetZ(BgZOrder + 10);
				jrnTxtHeadingL->LoadFont(eFont_Italic24);
				jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP67HeadingL"));
				jrnTxtHeadingL->SetAlignment(Align_Center);
				jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				*/
				jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP67HeadingL"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				jrnTxtHeadingL->CenterText();
				if (journalState != JRN_POPPED)
					jrnTxtHeadingL->Show(false);
				/*
				jrnTxtHeadingR = new CBitmapText();
				jrnTxtHeadingR->SetZ(BgZOrder + 10);
				jrnTxtHeadingR->LoadFont(eFont_Italic24);
				jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP67HeadingR"));
				jrnTxtHeadingR->SetAlignment(Align_Center);
				jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
				*/
				jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP67HeadingR"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				jrnTxtHeadingR->CenterText();
				if (journalState != JRN_POPPED)
					jrnTxtHeadingR->Show(false);

				//Left Page
				_fontSize = AssetsStringTable::GetFont("jrnP67Left");
				if (_fontSize != -1)
					GFApp->lpFontDiary->setHeightPix(_fontSize);
				else
					GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP67Left");
				if (_lineSpacing == -1)
					_lineSpacing = 25;

				jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP67Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				if (journalState != JRN_POPPED)
					jrnTxtL->Show(false);

				//Right Page
				jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP67Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				if (journalState != JRN_POPPED)
					jrnTxtR->Show(false);
				break;
				//eJrn_L8P2_PhoenixNoteEnd
		case PAGE_68:CurJrnPage->LoadSprite("MAPDIA\\PAGL8\\P2.SPR");
				CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				/*
				jrnTxtHeadingR = new CBitmapText();
				jrnTxtHeadingR->SetZ(BgZOrder + 10);
				jrnTxtHeadingR->LoadFont(eFont_Italic24);
				jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP68Heading"));
				jrnTxtHeadingR->SetAlignment(Align_Center);
				jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
				*/
				jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP68Heading"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				jrnTxtHeadingR->CenterText();
				if (journalState != JRN_POPPED)
					jrnTxtHeadingR->Show(false);

				//Left Page
				_fontSize = AssetsStringTable::GetFont("jrnP68");
				if (_fontSize != -1)
					GFApp->lpFontDiary->setHeightPix(_fontSize);
				else
					GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP68");
				if (_lineSpacing == -1)
					_lineSpacing = 25;

				//Right Page
				jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP68"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				jrnImg->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L8\\PHOENIXTEXT.SPR", 475, 345, BgZOrder + 20);
				jrnImg->SetUID("jrnImg");
				jrnImg->SetRotate(12);
				jrnImg->SetScale(0.75f);

				if (journalState != JRN_POPPED)
				{
					jrnTxtR->Show(false);
					jrnImg->Show(false);
				}
				break;
				//eJrn_L8P3_MammothMeet
		case PAGE_69:CurJrnPage->LoadSprite("MAPDIA\\PAGL8\\P3.SPR");
				CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				/*
				jrnTxtHeadingR = new CBitmapText();
				jrnTxtHeadingR->SetZ(BgZOrder + 10);
				jrnTxtHeadingR->LoadFont(eFont_Italic24);
				jrnTxtHeadingR->SetText(AssetsStringTable::getString("jrnP69Heading"));
				jrnTxtHeadingR->SetAlignment(Align_Center);
				jrnTxtHeadingR->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				jrnTxtHeadingR->SetXY(900 - (jrnTxtHeadingR->StringWidth() / 2), 80);
				*/
				jrnTxtHeadingR = new MultilineTextBox(AssetsStringTable::getString("jrnP69Heading"), 900, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				jrnTxtHeadingR->CenterText();
				if (journalState != JRN_POPPED)
					jrnTxtHeadingR->Show(false);

				//Left Page
				_fontSize = AssetsStringTable::GetFont("jrnP69");
				if (_fontSize != -1)
					GFApp->lpFontDiary->setHeightPix(_fontSize);
				else
					GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP69");
				if (_lineSpacing == -1)
					_lineSpacing = 25;

				//Right Page
				jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP69"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				if (journalState != JRN_POPPED)
					jrnTxtR->Show(false);
				break;
				//eJrn_L8P4_FlownToMohenjedaro
		case PAGE_70:CurJrnPage->LoadSprite("MAPDIA\\PAGL8\\P4.SPR");
				CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				/*
				jrnTxtHeadingL = new CBitmapText();
				jrnTxtHeadingL->SetZ(BgZOrder + 10);
				jrnTxtHeadingL->LoadFont(eFont_Italic24);
				jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP70Heading"));
				jrnTxtHeadingL->SetAlignment(Align_Center);
				jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				*/
				jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP70Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				jrnTxtHeadingL->CenterText();
				if (journalState != JRN_POPPED)
					jrnTxtHeadingL->Show(false);

				//Left Page
				_fontSize = AssetsStringTable::GetFont("jrnP70");
				if (_fontSize != -1)
					GFApp->lpFontDiary->setHeightPix(_fontSize);
				else
					GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP70");
				if (_lineSpacing == -1)
					_lineSpacing = 25;

				jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP70"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				if (journalState != JRN_POPPED)
					jrnTxtL->Show(false);
				break;
				//eJrn_L8P5_MaryResurrect
		case PAGE_71:CurJrnPage->LoadSprite("MAPDIA\\PAGL8\\P5.SPR");
				CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				/*
				jrnTxtHeadingL = new CBitmapText();
				jrnTxtHeadingL->SetZ(BgZOrder + 10);
				jrnTxtHeadingL->LoadFont(eFont_Italic24);
				jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP71Heading"));
				jrnTxtHeadingL->SetAlignment(Align_Center);
				jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				*/
				jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP71Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				jrnTxtHeadingL->CenterText();
				if (journalState != JRN_POPPED)
					jrnTxtHeadingL->Show(false);

				//Left Page
				_fontSize = AssetsStringTable::GetFont("jrnP71");
				if (_fontSize != -1)
					GFApp->lpFontDiary->setHeightPix(_fontSize);
				else
					GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP71");
				if (_lineSpacing == -1)
					_lineSpacing = 25;

				jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP71"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				if (journalState != JRN_POPPED)
					jrnTxtL->Show(false);
				break;
				//eJrn_L8P6_GrandReunion
		case PAGE_72:CurJrnPage->LoadSprite("MAPDIA\\PAGL8\\P6.SPR");
				CurJrnPage->SetPos(PointSystem::CreateCPoint(688,375,0));
				/*
				jrnTxtHeadingL = new CBitmapText();
				jrnTxtHeadingL->SetZ(BgZOrder + 10);
				jrnTxtHeadingL->LoadFont(eFont_Italic24);
				jrnTxtHeadingL->SetText(AssetsStringTable::getString("jrnP72Heading"));
				jrnTxtHeadingL->SetAlignment(Align_Center);
				jrnTxtHeadingL->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
				jrnTxtHeadingL->SetXY(470 - (jrnTxtHeadingL->StringWidth() / 2), 80);
				*/
				jrnTxtHeadingL = new MultilineTextBox(AssetsStringTable::getString("jrnP72Heading"), 470, 80, BgZOrder + 10, diarytextColor, eFont_Italic24, _lineSpacing);
				jrnTxtHeadingL->CenterText();
				if (journalState != JRN_POPPED)
					jrnTxtHeadingL->Show(false);

				//Left Page
				_fontSize = AssetsStringTable::GetFont("jrnP72Left");
				if (_fontSize != -1)
					GFApp->lpFontDiary->setHeightPix(_fontSize);
				else
					GFApp->lpFontDiary->setHeightPix(HEIGHTPIX_DIARY);

				_lineSpacing = AssetsStringTable::GetLineSpacing("jrnP72Left");
				if (_lineSpacing == -1)
					_lineSpacing = 25;

				jrnTxtL = new MultilineTextBox(AssetsStringTable::getString("jrnP72Left"), 285, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				if (journalState != JRN_POPPED)
					jrnTxtL->Show(false);

				//Right Page
				jrnTxtR = new MultilineTextBox(AssetsStringTable::getString("jrnP72Right"), 695, 90, BgZOrder + 10, diarytextColor, eFont_Diary, _lineSpacing);
				if (journalState != JRN_POPPED)
					jrnTxtR->Show(false);
				break;
	}

	CurJrnPage->SetZ(BgZOrder+2);//(2);		
	if( PrevJrnPage != NULL )
	{
		if(!IsInitialLoading)
		{
			CurJrnPage->FadeIn(2.0f);
			PrevJrnPage->FadeOut(2.0f);
		}
	}

	
	if( ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive )
	{
		CurJrnPage->Show(false);
		if (jrnTxtR)
			jrnTxtR->Show(false);
		if (jrnTxtL)
			jrnTxtL->Show(false);
		if (jrnTxtHeadingL)
			jrnTxtHeadingL->Show(false);
		if (jrnTxtHeadingR)
			jrnTxtHeadingR->Show(false);
		if (jrnImg)
			jrnImg->Show(false);
		if (jrnImg2)
			jrnImg2->Show(false);
	}
	else
	{
		CurJrnPage->Show(true);
		showDiaryText = true;
		textShowTimer = 0;
	}
}

void Journal::ShowJournalPage()//TODO: cleanup diary text
{
	BtnTabCharacter->PlayAnimation(0);
	BtnTabObjective->PlayAnimation(0);
	BtnTabJournal->PlayAnimation(2);

	ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive = false;

	( ActiveJrnPage == PAGE_1 )?BtnPrevPage->SetEnable(false):BtnPrevPage->SetEnable(true);
	( ActiveJrnPage == ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked )?BtnNextPage->SetEnable(false):BtnNextPage->SetEnable(true);

	LoadJrnPage();
}

void Journal::ShowObjectivesPage()//TODO: cleanup diary text
{
	BtnTabCharacter->PlayAnimation(0);
	BtnTabJournal->PlayAnimation(0);
	BtnTabObjective->PlayAnimation(2);

	ObjectiveTitle->SetEnable(true);

	for(int i=0; i < numCurrentObjectives; i++)
	{					
		ObjSymbols[i]->SetEnable(true);					
		ObjBmpTxt[i]->SetEnable(true);
	}

	if(GFHud->Tutor != NULL && GFHud->Tutor->IsActive)
	{
		ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT04_ObjectivesTab] = true;
		isObjTabTutorActive = false;
		GFHud->Tutor->HideTutorial(true);
	}
}

void Journal::ShowJournal(bool noTransition, bool showCharacterPage)
{
	isCharacterPageActive = showCharacterPage;

	UpdateJournalAndObjectives();

	if(GameTimeText==NULL)
	{
		GameTimeText = new CBitmapText();
		ColorRGB white={(225/255.0f),(235/255.0f),(235/255.0f)};

		//std::string elapsedTimeText = MKSTR(ProfileSystem::Instance->ActiveProfile.ElapsedGamePlayTime);
		
		GameTimeText->SetTextProps("", 683, 710, BgZOrder+20, white, eFont_Italic24, Align_Center);
	}

	int gamePlayInMinutes = ProfileSystem::Instance->ActiveProfile.ElapsedGamePlayTime/60;
	int gamePlayInHours = gamePlayInMinutes/60;
	gamePlayInMinutes = gamePlayInMinutes%60;
	std::string elapsedTimeText=StringTable::getStr("Explayed");
	if(gamePlayInHours>0) //0 hours
		elapsedTimeText += (MKSTR(gamePlayInHours) + StringTable::getStr("Exhrs"));

	elapsedTimeText += (MKSTR(gamePlayInMinutes) + StringTable::getStr("Exmins"));
	GameTimeText->SetText(elapsedTimeText);

	GameTimeText->Show(false);

	if(GFHud->Tutor != NULL && GFHud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT04_ObjectivesTab] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
	{
		//Hide the map button for this one
		BtnTabMap->SetEnable(false);
	}
	else
	{
		BtnTabMap->SetEnable(true);
	}

	BtnTabObjective->PlayAnimation(0);
	BtnTabJournal->PlayAnimation(0);
	BtnTabCharacter->PlayAnimation(0);

	if(isCharacterPageActive)
	{
		BtnTabCharacter->PlayAnimation(2);

		HideObjectivesPage();
		HideJournalPage();

		ShowCharacterPage();
	}
	else if( !ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive )
	{
		BtnTabJournal->PlayAnimation(2);

		HideCharacterPage();
		HideObjectivesPage();

		LoadJrnPage(true);
		if(CurJrnPage!=NULL)
			CurJrnPage->Show(false);

		//ShowJournalPage();
	}
	else
	{
		//Objectives Page Active
		BtnTabObjective->PlayAnimation(2);

		ObjectiveTitle->SetEnable(true);

		for(int i=0; i < numCurrentObjectives; i++)
		{					
			ObjSymbols[i]->SetEnable(true);					
			ObjBmpTxt[i]->SetEnable(true);
		}

		HideJournalPage();
		HideCharacterPage();
	}

	if(noTransition == false)
	{
		Visible = true;

		PopVal = 0;
		PopupAlpha = 0.0f;

		journalState = JRN_POPUP;
		InitRenderTarget(JrnScene);
		GFInstance->SetPopupTween( -480+15,360-27,171,0);
		GFInstance->CreateGreyscaleScene();

		if( ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive )
		{
			ObjectiveTitle->SetEnable(false);

			for(int i=0; i < numCurrentObjectives; i++)
			{					
				ObjSymbols[i]->SetEnable(false);					
				ObjBmpTxt[i]->SetEnable(false);
			}
		}

	}
	else
	{
		Visible = true;

		PopupAlpha = 1.0f;

		GameTimeText->Show(true);

		JrnScene->Show(true);
		journalState = JRN_POPPED;
				
		if( ActiveJrnPage == PAGE_1 )
			BtnPrevPage->SetEnable(false);

		if( ActiveJrnPage == ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked )
			BtnNextPage->SetEnable(false);

		if( ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive )
		{
			if (!isCharacterPageActive)
			{
				ObjectiveTitle->SetEnable(true);

				BtnPrevPage->SetEnable(false);
				BtnNextPage->SetEnable(false);

				for(int i=0; i < numCurrentObjectives; i++)
				{
					ObjSymbols[i]->SetEnable(true);
					ObjBmpTxt[i]->SetEnable(true);
				}  
			}
		}
		else
		{
			if (!isCharacterPageActive)
			{
				ObjectiveTitle->SetEnable(false);

				for(int i=0; i < numCurrentObjectives; i++)
				{						
					ObjSymbols[i]->SetEnable(false);
					ObjBmpTxt[i]->SetEnable(false);
				}

				if(CurJrnPage!=NULL)
					CurJrnPage->Show(true); 
			}
		}
	}
}

void Journal::ShowCharacterPage()//TODO: cleanup diary text
{	
	BtnTabCharacter->PlayAnimation(2);
	GetCharacterIDFromProfile();
	SetupCharacterPage();
	loadCharacter(activeCharacterID);
}

void Journal::GetUnlockedCharacterInClan(int clan)
{
	int charID = eChar1_Sophia;
	if(clan == 0) //creek
	{
		for (int i = eChar1_Sophia; i <= eChar5_Grandma; i++)
		{
			if(ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[i])
			{
				charID = i;
				break;
			}
		}
	}
	else if(clan == 1) //snake
	{
		for (int i = eChar6_Darkarta; i <= eChar9_Varuni; i++)
		{
			if(ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[i])
			{
				charID = i;
				break;
			}
		}
	}
	else if(clan == 2) //Eagle
	{
		for (int i = eChar10_Lasya; i <= eChar12_Devadeva; i++)
		{
			if(ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[i])
			{
				charID = i;
				break;
			}
		}
	}
	else if(clan == 3) //Others
	{
		for (int i = eChar13_Rajasthani; i <= eChar16_Anasooya; i++)
		{
			if(ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[i])
			{
				charID = i;
				break;
			}
		}
	}

	activeCharacterID = charID;
}

void Journal::SetupCharacterPage()
{
	BtnClanCreeks->SetEnable(false);
	BtnClanSnake->SetEnable(false);
	BtnClanEagle->SetEnable(false);
	BtnClanOthers->SetEnable(false);

	BtnClanCreeks->PlayAnimation(0);
	BtnClanSnake->PlayAnimation(0);
	BtnClanEagle->PlayAnimation(0);
	BtnClanOthers->PlayAnimation(0);

	if(activeCharacterID>eChar0_None && activeCharacterID<= eChar5_Grandma)
		BtnClanCreeks->PlayAnimation(2);
	else if(activeCharacterID>eChar5_Grandma && activeCharacterID <= eChar9_Varuni)
		BtnClanSnake->PlayAnimation(2);
	else if(activeCharacterID>eChar9_Varuni && activeCharacterID <= eChar12_Devadeva)
		BtnClanEagle->PlayAnimation(2);
	else
		BtnClanOthers->PlayAnimation(2);

	//Turning On or Off the Clan Buttons
	for (int i = eChar1_Sophia; i <= eChar16_Anasooya; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[i])
		{
			if(i>eChar0_None && i<= eChar5_Grandma)
				BtnClanCreeks->SetEnable(true);
			else if(i>eChar5_Grandma && i <= eChar9_Varuni)
				BtnClanSnake->SetEnable(true);
			else if(i>eChar9_Varuni && i <= eChar12_Devadeva)
				BtnClanEagle->SetEnable(true);
			else
				BtnClanOthers->SetEnable(true);
		}
	}

	BtnNextPage->SetEnable(true);
	BtnPrevPage->SetEnable(true);
	if(activeCharacterID<=eChar1_Sophia)
	{
		BtnPrevPage->SetEnable(false);	
	}
	else
	{
		//Check if any character is unlocked to the right
		int lastUnlockedCharacter = activeCharacterID;
		for (int i = activeCharacterID+1; i <= eChar16_Anasooya; i++)
		{
			if(ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[i])
				lastUnlockedCharacter = i;
		}
		if(lastUnlockedCharacter == activeCharacterID)
		{
			BtnNextPage->SetEnable(false);
		}
	}
}

void Journal::GetNextUnlockedCharacterID()
{
	//Check if any character is unlocked to the right
	for (int i = activeCharacterID+1; i <= eChar16_Anasooya; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[i])
		{
			activeCharacterID = i;
			break;
		}
	}
}

void Journal::GetPreviousUnlockedCharacterID()
{
	//Check if any character is unlocked to the left
	for (int i = activeCharacterID-1; i > eChar0_None; i--)
	{
		if(ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[i])
		{
			activeCharacterID = i;
			break;
		}
	}
}

void Journal::GetCharacterIDFromProfile()
{
	if(ProfileSystem::Instance->ActiveProfile.lastUnlockedCharacter != -1)
	{
		activeCharacterID = ProfileSystem::Instance->ActiveProfile.lastUnlockedCharacter;
		ProfileSystem::Instance->ActiveProfile.lastUnlockedCharacter = -1;
	}
	else
	{
		activeCharacterID = ProfileSystem::Instance->ActiveProfile.lastViewedCharacter;
	}
}

void  Journal::InitRenderTarget(CHOScene *Popup)
{
	if( GFInstance->IsRTCreated )
	{		
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS	
		GFInstance->IsRenderingToRT = true;
    }
		Popup->RenderToTarget(-171.0f);
		if(isCharacterPageActive)
		{
			if(charPhoto!=NULL)
				charPhoto->Render(PointSystem::CreateCPoint(-171.0f,0,0)); 
			//if(charText!=NULL)
				//charText->Render(PointSystem::CreateCPoint(-171.0f,0,0));
		}
		else
		{
			if( !ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive )
			{
				if(CurJrnPage!=NULL)
					CurJrnPage->Render(PointSystem::CreateCPoint(-171.0f,0,0)); 
			}
			else
			{
				for(int i=0; i < numCurrentObjectives; i++)
				{
					//Load Objectives Text
					if(ObjBmpTxt[i]!=NULL)
					{
						ObjBmpTxt[i]->Render(PointSystem::CreateCPoint(171.0f,0,0));
					}
				}
			}
		}

    if( GFInstance->IsRTCreated )
    {
		GFInstance->IsRenderingToRT = false;
		GFInstance->RTGraphic->endRenderToTarget();
	}
}

void  Journal::HideCharacterPage()
{
	isCharacterPageActive = false;

	BtnClanCreeks->SetEnable(false);
	BtnClanSnake->SetEnable(false);
	BtnClanEagle->SetEnable(false);
	BtnClanOthers->SetEnable(false);

	SAFE_DELETE(charPhoto);
	SAFE_DELETE(charText);
}

void  Journal::HideObjectivesPage()
{
	ObjectiveTitle->SetEnable(false);

	for(int i=0; i < numCurrentObjectives; i++)
	{
		ObjSymbols[i]->SetEnable(false);		
		ObjBmpTxt[i]->SetEnable(false);
	}
}

void  Journal::HideJournalPage()
{
	BtnNextPage->SetEnable(false);
	BtnPrevPage->SetEnable(false);

	/*
	if(jrnTxtL!=NULL)
		jrnTxtL->Show(false);
	if (jrnTxtR != NULL)
		jrnTxtR->Show(false);
	if (jrnTxtHeadingL != NULL)
		jrnTxtHeadingL->Show(false);
	if (jrnTxtHeadingR != NULL)
		jrnTxtHeadingR->Show(false);
	 */

	//TEST CODE
	if (jrnTxtL != NULL)
	{
		//jrnTxtL->Show(false);
		delete jrnTxtL;
		jrnTxtL = NULL;
	}

	if (jrnTxtR != NULL)
	{
		//jrnTxtR->Show(false);
		delete jrnTxtR;
		jrnTxtR = NULL;
	}

	if (jrnTxtHeadingL != NULL)
	{
		//jrnTxtHeadingL->Show(false);
		delete jrnTxtHeadingL;
		jrnTxtHeadingL = NULL;
	}
	if (jrnTxtHeadingR != NULL)
	{
		//jrnTxtHeadingR->Show(false);
		delete jrnTxtHeadingR;
		jrnTxtHeadingR = NULL;
	}

	if(CurJrnPage!=NULL)
		CurJrnPage->Show(false);

	if (jrnImg != NULL)
		jrnImg->Show(false);
	if (jrnImg2 != NULL)
		jrnImg2->Show(false);

	SAFE_DELETE(CurJrnPage);
	SAFE_DELETE(PrevJrnPage);
	SAFE_DELETE(jrnImg);
	SAFE_DELETE(jrnImg2);
	/*
	if (jrnTxtL != NULL)
		jrnTxtL->Show(false);
	SAFE_DELETE(jrnTxtL);
	*/
}

void Journal::HideJournal(bool noTransition)
{
	ProfileSystem::Instance->ActiveProfile.JrnPageLastRead = ActiveJrnPage;

	ObjectiveTitle->SetEnable(false);

	//check if journal page tab is active
	//if( !ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive )
	{
		for(int i=0; i < numCurrentObjectives; i++)
		{
			ObjSymbols[i]->SetEnable(false);
			ObjBmpTxt[i]->SetEnable(false);
		}
	}

	GameTimeText->Show(false);

	JrnScene->Show(false);

	if(noTransition)
	{
		journalState = JRN_IDLE;
		GFHud->IsDiaryShowing = false;
	}
	else
	{
		Control::Audio->QuickLoadAndPlaySFX(aSFXDiaClose);

		journalState = JRN_POPDOWN;
		InitRenderTarget(JrnScene);
	}

	if(isCharacterPageActive)
	{
		SAFE_DELETE(charPhoto);
		SAFE_DELETE(charText);
	}

	if(CurJrnPage!=NULL)
	{
		CurJrnPage->Show(false);
		SAFE_DELETE(CurJrnPage);
	}

	if (jrnTxtL != NULL)
	{
		delete jrnTxtL;
		jrnTxtL = NULL;
	}

	if (jrnTxtR != NULL)
	{
		delete jrnTxtR;
		jrnTxtR = NULL;
	}

	if (jrnTxtHeadingL != NULL)
	{
		delete jrnTxtHeadingL;
		jrnTxtHeadingL = NULL;
	}

	if (jrnTxtHeadingR != NULL)
	{
		delete jrnTxtHeadingR;
		jrnTxtHeadingR = NULL;
	}

	if (jrnImg != NULL)
	{
		delete jrnImg;
		jrnImg = NULL;
	}
	if (jrnImg2 != NULL)
	{
		delete jrnImg2;
		jrnImg2 = NULL;
	}

	SAFE_DELETE(PrevJrnPage);
}
