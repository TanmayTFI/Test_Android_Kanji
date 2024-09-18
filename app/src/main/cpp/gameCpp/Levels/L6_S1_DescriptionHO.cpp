#include "L6_S1_DescriptionHO.h"
#include "GameFlow.h"
#include "Hud.h"
#include "Util.h"

const CRectangle  Match3btnRect(529.0f,0.0f,309.0f,47.0f);
const CRectangle  InvDestRectEmblem(549.0f,206.0f,267.0f,242.0f);
const CRectangle  PopAreaRect(479.0f,35.0f,413.0f,598.0f);


const string descriptionText[] = {"L6empty",
	"Exeagle",
	"Exprince",
	"Excast",
	"Exwhile",
	"Exescape"};

const int numberOfHiddenObjects[] = {4,2,3,4,2};

#define PAUSE_DURATION	90

const CRectangle  PaptusFlyRect(270.0f,482.0f,62.0f,66.0f);

L6_S1_DescriptionHO::L6_S1_DescriptionHO()
{
	popNote =NULL;

	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S1\\HO\\HO.SCN");

	//Localized
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\INDUSSNAKE.SPR", 375, 200,  98);
	temp->SetUID("text1");
	if(ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced)
		temp->Show(false);
	HOScene->PushObject(temp);

	//Localized
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\SCRIPTURES.SPR", 980, 200, 98);
	temp->SetUID("text2"); 
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced)
		temp->Show(false);
	HOScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\AMEEHATEXTZOOM.SPR", 980, 200, 98);
	temp->SetUID("lettertext");
	temp->Show(false);
	HOScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\AMEEHATEXTZOOM.SPR", 720, 300, 200);
	temp->SetScale(0.7);
	temp->SetRotate(10);
	temp->SetUID("lettertext2");
	HOScene->PushObject(temp);

	Control::Audio->LoadSample(aVOL6DescS1_1,	AudioVO);
	Control::Audio->LoadSample(aVOL6DescS1_2,	AudioVO);
	Control::Audio->LoadSample(aVOL6DescS1_3,	AudioVO);
	Control::Audio->LoadSample(aVOL6DescS1_4,	AudioVO);
	Control::Audio->LoadSample(aVOL6DescS1_5,	AudioVO);
	Control::Audio->LoadSample(aVOL6NoteVOS4_1,	AudioVO);
	Control::Audio->LoadSample(aVOL6MaryRefS4_1,	AudioVO);
	Control::Audio->LoadSample(aVOL6MaryRefS5_1,	AudioVO);

	//Match 3
	match3Ctrl  = NULL;
	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	
	//BtnMatch3 = HOScene->GetObjectByID("btnMatch3");
	//BtnSwitchToHO = HOScene->GetObjectByID("btnSwitchToHO");
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
	BtnMatch3 = new CObject();
	BtnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 682, 26, 48);
	BtnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(BtnMatch3);

	BtnSwitchToHO = new CObject();
	BtnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 682, 26, 49);
	BtnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(BtnSwitchToHO);

	BtnMatch3->SetZ(eZOrder_PopupOverPopup + 299);
	BtnMatch3->Show(false);
	BtnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);
	BtnSwitchToHO->Show(false);

	//Add reset

	SceneData resetbtn;
	resetbtn.objType = "Object";
	resetbtn.uid = "ResetBtn";
	resetbtn.spriteString = "UI\\RESETBTN.SPR";
	resetbtn.spriteAnim = 0;
	resetbtn.x = 140;
	resetbtn.y = 350;
	resetbtn.ZOrder = eZOrder_PopupOverPopup + 299;

	ResetBtn = HOScene->CreateObject(resetbtn);
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
	
	activeHO = 0;
	for (int i = 0; i < MAX_L6S1_HOSCENES; i++)
	{
		if (ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[i])
		{
			activeHO++;
		}
	}

	for (int i = 1; i < MAX_L6S1_HOSCENES; i++)
	{
		int _index = MAX_L6S1_HOSCENES - i;
		int _j = i - 1;
		Ho_Scene[_j] = new CHOScene();
		string folderName = "LV6\\S1\\HO\\SLIDE_" + MKSTR(_index);
		Ho_Scene[_j]->Load(folderName + std::string("\\HO_SLIDE_") + MKSTR(_index) + std::string(".SCN"));
		Ho_Scene[_j]->SetZOff(4);
		if (_j == activeHO - 1 || (_j == 0 && activeHO == 0))
		{
			Ho_Scene[_j]->SetPosOff(260 + ((0) * 1036), 88);
		}
		else
		{
			Ho_Scene[_j]->SetPosOff(260 + ((1) * 1036), 88 );
		}

	}

	/*switch (activeHO)
	{
	case 0:*/
	for (int i = 0; i < 4; i++)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S1[i])
		{
			string _id = "slide-5_Large0" + MKSTR(i + 1);
			Ho_Scene[0]->GetObjectByID(_id)->SetAlpha(0);
		}
		else
		{
			string _id = "slide-5_Small0" + MKSTR(i + 1);
			Ho_Scene[0]->GetObjectByID(_id)->SetEnable(false);
		}
	}
	for (int i = 0; i < 2; i++)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S2[i])
		{
			string _id = "slide-4_Large0" + MKSTR(i + 1);
			Ho_Scene[1]->GetObjectByID(_id)->SetAlpha(0);
		}
		else
		{
			string _id = "slide-4_Small0" + MKSTR(i + 1);
			Ho_Scene[1]->GetObjectByID(_id)->SetEnable(false);
		}
	}
	/*break;
	case 1:*/
	for (int i = 0; i < 3; i++)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S3[i])
		{
			string _id = "slide-3_Large0" + MKSTR(i + 1);
			Ho_Scene[2]->GetObjectByID(_id)->SetAlpha(0);
		}
		else
		{
			string _id = "slide-3_Small0" + MKSTR(i + 1);
			Ho_Scene[2]->GetObjectByID(_id)->SetEnable(false);
		}
	}
	/*break;
	case 2:*/
	for (int i = 0; i < 4; i++)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S4[i])
		{
			string _id = "slide-2_Large0" + MKSTR(i + 1);
			Ho_Scene[3]->GetObjectByID(_id)->SetAlpha(0);
		}
		else
		{
			string _id = "slide-2_Small0" + MKSTR(i + 1);
			Ho_Scene[3]->GetObjectByID(_id)->SetEnable(false);
		}
	}
	/*break;
	case 3:*/
	for (int i = 0; i < 2; i++)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S5[i])
		{
			string _id = "slide-1_Large0" + MKSTR(i + 1);
			Ho_Scene[4]->GetObjectByID(_id)->SetAlpha(0);
		}
		else
		{
			string _id = "slide-1_Small0" + MKSTR(i + 1);
			Ho_Scene[4]->GetObjectByID(_id)->SetEnable(false);
		}
	}
	/*break;
	default:
	break;
	}*/

	//ShowHintText(activeHO + 1);
	if (activeHO < MAX_L6S1_HOSCENES)
	{
		Hud->ShowBannerText(descriptionText[activeHO]);
		if(activeHO == 1)
			Control::Audio->PlaySample(aVOL6DescS1_1);
		else if(activeHO == 2)
			Control::Audio->PlaySample(aVOL6DescS1_2);
		else if(activeHO == 3)
			Control::Audio->PlaySample(aVOL6DescS1_3);
		else if(activeHO == 4)
			Control::Audio->PlaySample(aVOL6DescS1_4);
		else if(activeHO == 5)
			Control::Audio->PlaySample(aVOL6DescS1_5);
	}

	/*Back_Btn = new UIButton("LV6\\S1\\HO\\ButtonBack.SPR", HOScene->GetObjectByID("ButtonBack")->GetPosPointer()->x, HOScene->GetObjectByID("ButtonBack")->GetPosPointer()->y, 100, CC_CALLBACK_1(L6_S1_DescriptionHO::OnBackArrowClick, this));
	Next_Btn = new UIButton("LV6\\S1\\HO\\NextButton.SPR", HOScene->GetObjectByID("NextButton")->GetPosPointer()->x, HOScene->GetObjectByID("NextButton")->GetPosPointer()->y, 100, CC_CALLBACK_1(L6_S1_DescriptionHO::OnNextArrowClick, this));
	*/
	HOScene->GetObjectByID("Bg")->SetZ(99);
	HOScene->GetObjectByID("FrontCover")->SetZ(96);
	HOScene->GetObjectByID("Effect")->SetZ(97);
	HOScene->GetObjectByID("LOcket")->SetZ(98);

	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced)
	{
		HOScene->GetObjectByID("Effect")->SetEnable(false);
		HOScene->GetObjectByID("LOcket")->SetEnable(false);
		Hud->ShowBannerText("L6empty");
	}
	//emblem fixed and HO not finished!!
	else if ( !ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P3_DescriptionHOSolved] )
	{
		BtnMatch3->Show(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[0])
	{
		HOScene->GetObjectByID("FrontCover")->SetEnable(false);
		HOScene->GetObjectByID("Effect")->SetEnable(false);
		HOScene->GetObjectByID("LOcket")->SetEnable(false);
	}

	HOScene->GetObjectByID("tongs")->SetEnable(false);
	HOScene->GetObjectByID("letter")->SetEnable(false);
	HOScene->GetObjectByID("lettertext2")->SetEnable(false);
	HOScene->GetObjectByID("shadow")->SetEnable(false);
	HOScene->GetObjectByID("Slide_Depth")->SetEnable(false);

	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken && ProfileSystem::Instance->ActiveProfile.L6_S1_LetterTaken)
	{
		HOScene->GetObjectByID("tongs")->SetEnable(true);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_LetterTaken && ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[MAX_L6S1_HOSCENES - 1])
	{
		HOScene->GetObjectByID("letter")->SetEnable(true);
		HOScene->GetObjectByID("lettertext2")->SetEnable(true);
		HOScene->GetObjectByID("tongs")->SetEnable(true);
		//HOScene->GetObjectByID("shadow")->SetEnable(true);
	}

	HOScene->GetObjectByID("paptus")->SetBlendMode(eBlend_AlphaAdditive);
	if(ProfileSystem::Instance->ActiveProfile.IsPaptusL6S1Collected)
	{
		HOScene->GetObjectByID("paptus")->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[MAX_L6S1_HOSCENES - 1])
		HOScene->GetObjectByID("paptus")->SetEnable(false);

	_objectInHand = 0;
	_hiddenObj = NULL;

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s1DescriptionHOAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s1DescriptionHOAmb) )		
	{
		Control::Audio->PlaySample(aAmbL6s1DescriptionHOAmb,true);
	}

	//MUSIC_SFX

	int ArrayPts[] = {569,173,625,446,813,402,760,137,567,174};
	CPolygon *CPolyObjName=  new CPolygon(5);
	CPolyObjName->Init(ArrayPts);
	popNote = new L6S4_PopAmeehaNote(1, Hud, NULL, eZOrder_Popup, CPolyObjName->center,CC_CALLBACK_1(L6_S1_DescriptionHO::OnPopupClosed,this));

	//Localized
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\AMEEHATEXTZOOM.SPR", 680, 330, eZOrder_Popup + 20);
	temp->SetUID("text");
	temp->Show(false);
	popNote->PushObject(temp);

	popNote->SetPopupBoundingBox(PopAreaRect);
	delete(CPolyObjName);

	activePopupID = -1;

	_delay = -1;

	if(ProfileSystem::Instance->_isExtrasGamePlay)
	{
		HOScene->GetObjectByID("text1")->SetEnable(false);
		HOScene->GetObjectByID("text2")->SetEnable(false);
	}
}

void L6_S1_DescriptionHO::ReceiveMessage(int val)
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
			//If no drop items in this scene !!
			if (itemVal == eInv_L6_S1_Emblem && !ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced && InvDestRectEmblem.Intersection(&dropPos))
			{
				Control::Audio->QuickLoadAndPlaySFX("s1ho_placingEmblem");
				HOScene->GetObjectByID("LOcket")->SetEnable(true);
				HOScene->GetObjectByID("Effect")->SetEnable(true);
				HOScene->GetObjectByID("Effect")->SetAlpha(0);
				BtnMatch3->Show(true);				
				float valArray[] = {1,HOScene->GetObjectByID("Effect")->GetPosPointer()->x,HOScene->GetObjectByID("Effect")->GetPosPointer()->y,1,0};
				HOScene->GetObjectByID("Effect")->TweenTo(valArray,1,0,true,CC_CALLBACK_1(L6_S1_DescriptionHO::OnFadeInComplete,this));
				ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced = true;
				AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Emblem] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S1_Emblem);
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{	
		if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved && ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken)
		{
			Hud->InvBoxCtrl->ResetToInvBox();
			AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1;
			SendMessage(ControlExit, ID, HOSM_NewArea);	 
		}
	}	
}

void L6_S1_DescriptionHO::OnFadeInComplete(CObject *obj)
{
	activeHO++;
	Control::Audio->QuickLoadAndPlaySFX("s1ho_tileSliding");
	HOScene->GetObjectByID("FrontCover")->TweenPosTo(-1036,0,0,1,false,true,CC_CALLBACK_1(L6_S1_DescriptionHO::OnLeftMoveTweenComplete,this));
	HOScene->GetObjectByID("Effect")->TweenPosTo(-1036,0,0,1,false,true);
	HOScene->GetObjectByID("LOcket")->TweenPosTo(-1036,0,0,1,false,true);
	HOScene->GetObjectByID("Slide_Depth")->SetEnable(true);
	HOScene->GetObjectByID("Slide_Depth")->TweenPosTo(-1036,0,0,1,false,true,CC_CALLBACK_1(L6_S1_DescriptionHO::OnDepthEffectTweenComplete,this));
	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[0])
	{
		HOScene->GetObjectByID("text1")->SetEnable(true);
		HOScene->GetObjectByID("text1")->TweenPosTo(-1036, 0, 0, 1, false, true, CC_CALLBACK_1(L6_S1_DescriptionHO::OnDepthEffectTweenComplete, this));
		HOScene->GetObjectByID("text2")->SetEnable(true);
		HOScene->GetObjectByID("text2")->TweenPosTo(-1036, 0, 0, 1, false, true, CC_CALLBACK_1(L6_S1_DescriptionHO::OnDepthEffectTweenComplete, this));
	}

	ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[0] = true;
}

void L6_S1_DescriptionHO::OnLeftMoveTweenComplete(CObject *obj)
{
	Hud->ShowBannerText(descriptionText[activeHO]);
	if(activeHO == 1)
		Control::Audio->PlaySample(aVOL6DescS1_1);
	else if(activeHO == 2)
		Control::Audio->PlaySample(aVOL6DescS1_2);
	else if(activeHO == 3)
		Control::Audio->PlaySample(aVOL6DescS1_3);
	else if(activeHO == 4)
		Control::Audio->PlaySample(aVOL6DescS1_4);
	else if(activeHO == 5)
		Control::Audio->PlaySample(aVOL6DescS1_5);
}


void L6_S1_DescriptionHO::OnDepthEffectTweenComplete(CObject *obj)
{
	HOScene->GetObjectByID("Slide_Depth")->SetEnable(false);
	HOScene->GetObjectByID("Slide_Depth")->SetPos(&(HOScene->GetObjectByID("Slide_Depth")->InitPos));
	HOScene->GetObjectByID("text1")->SetEnable(false);
	HOScene->GetObjectByID("text1")->SetPos(&(HOScene->GetObjectByID("text1")->InitPos)); 
	HOScene->GetObjectByID("text2")->SetEnable(false);
	HOScene->GetObjectByID("text2")->SetPos(&(HOScene->GetObjectByID("text2")->InitPos));
}

void L6_S1_DescriptionHO::OnPopupClosed(Popup * popup)
{
	activePopupID = -1;
	ProfileSystem::Instance->ActiveProfile.L6_S1_LetterTaken = true;

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P6_AmeehaNote])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P6_AmeehaNote] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P6_AmeehaNote;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}

	Control::Audio->StopSample(aVOL6NoteVOS4_1);
	Control::Audio->PlaySample(aVOL6MaryRefS4_1);
	Hud->ShowBannerText("L6me");
}

void L6_S1_DescriptionHO::Update()
{
#ifdef _TABINGAMEUI
	if(_TabInGameMenuShowing)
		return;
#endif // _TABINGAMEUI
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif
	NavigateToScene();

	if (activePopupID != -1)
		return;
	if( Hud->PopupState != Popup_Inactive )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	if (_delay > -1)
	{
		if (_delay < PAUSE_DURATION)
		{
			_delay++;
			return;
		}
		_delay = -1;
		InitTweens(MAX_L6S1_HOSCENES - activeHO,numberOfHiddenObjects[activeHO - 1],numberOfHiddenObjects[activeHO]);
		activeHO++;
	}

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	CPoint pos = Control::Input->Mpos();
	if(!BtnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved && ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}
	if(ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		BtnMatch3->Show(false);
		BtnSwitchToHO->Show(false);
	}
	ResetHO();

	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	if(ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[MAX_L6S1_HOSCENES - 1] && !ProfileSystem::Instance->ActiveProfile.IsPaptusL6S1Collected && PaptusFlyRect.Intersection(&pos))
	{
		if(Control::Input->LBclicked())
		{
			Control::Audio->PlaySample(aSFXMagic2);
			HOScene->GetObjectByID("paptus")->FadeOut(1.25f);
			Hud->CollectMysteryFlower(eArea_L6_S1_DescriptionHO, HOScene->GetObjectByID("paptus")->Pos);
			return;
		}
	}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			Hud->InvBoxCtrl->ResetToInvBox();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1;
			SendMessage(ControlExit, ID, HOSM_NewArea);										
		}	
        #endif
	if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
	{
		if( Control::Input->LBclicked() )
		{
			Hud->InvBoxCtrl->ResetToInvBox();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	if( IsMouseOverGoBackArea(&pos)
		#ifdef M3_BUTTON
		&& !ProfileSystem::Instance->ActiveProfile._IsMatch3Playing
        #endif
        )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L6_S1_FloatingIsland1);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L6_S1_FloatingIsland1);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S1_FloatingIsland1, 2);
			}
#else			
			Hud->InvBoxCtrl->ResetToInvBox();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if( match3Ctrl->isPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken )
		{
			match3Ctrl->clearBoard();
			BtnMatch3->Show(false); 
			BtnSwitchToHO->Show(false);
			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->HOState = HOInactive;
			
			ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken = true;
			Hud->TakeInventoryItem(eInv_L6_S3_HorseShoe);
			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			return;
		}


		if(Match3btnRect.Intersection(&pos) && !match3Ctrl->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSwitchToHO->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				match3Ctrl->clearBoard();
				BtnMatch3->Show(true);
				BtnSwitchToHO->Show(false);
				BtnSwitchToHO->PlayAnimation(0);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Hud->InvBoxCtrl->ResetToInvBox();
				return;
			}
		}
		else
		{
			BtnSwitchToHO->PlayAnimation(0);
		}
		return;
	}
	else if(ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced)
	{
		if(Match3btnRect.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnMatch3->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				if(match3Ctrl == NULL)
				{
					match3Ctrl = new Match3(12);
					AddControl(match3Ctrl);
				}
				else
				{
					match3Ctrl->ShowBoard();
				}

				BtnMatch3->Show(false);
				BtnMatch3->PlayAnimation(0);
				BtnSwitchToHO->Show(true);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;

				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				return;
			}
		}
		else
		{
			BtnMatch3->PlayAnimation(0);
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{

		if (HOScene->GetObjectByID("letter")->GetWorldRect().Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[MAX_L6S1_HOSCENES - 1] && !ProfileSystem::Instance->ActiveProfile.L6_S1_LetterTaken)
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				activePopupID = 1;
				popNote->OpenPopup();
				Control::Audio->PlaySample(aVOL6NoteVOS4_1);
				Control::Audio->StopSample(aVOL6DescS1_5);
				HOScene->GetObjectByID("letter")->SetEnable(false);
				HOScene->GetObjectByID("lettertext2")->SetEnable(false);
				//HOScene->GetObjectByID("shadow")->SetEnable(false);
			}
		}
		else if (ProfileSystem::Instance->ActiveProfile.L6_S1_LetterTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_MaryTextShown)
		{
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L6this");
				Control::Audio->StopSample(aVOL6MaryRefS4_1);
				Control::Audio->PlaySample(aVOL6MaryRefS5_1);
				ProfileSystem::Instance->ActiveProfile.L6_S4_MaryTextShown = true;
			}
		}
		else if (ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[MAX_L6S1_HOSCENES - 1] && HOScene->GetObjectByID("tongs")->GetWorldRect().Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S4_MaryTextShown && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken)
		{
			Cursor::SetMode(CA_HandTake);
			if (Control::Input->LBclicked())
			{
				HOScene->GetObjectByID("tongs")->SetEnable(false);
				//Hud->TakeInventoryItem(eInv_L6_S1_Tongs);
				Hud->TakeInventoryItem(eInv_L6_S3_HorseShoe);
				ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken = true;

			}
			return;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[MAX_L6S1_HOSCENES - 1])
		{
			if (activeHO > 0 && !_objectInHand)
			{
				for (int i = 0; i < numberOfHiddenObjects[activeHO - 1]; i++)
				{
					string _hiddenObjName = "slide-" + MKSTR(MAX_L6S1_HOSCENES - (activeHO)) + "_Small0" + MKSTR(i + 1);
					_hiddenObj = Ho_Scene[activeHO - 1]->GetObjectByID(_hiddenObjName);
					if (_hiddenObj->GetWorldRect().Intersection(&pos) && Control::Input->LBclicked() && _hiddenObj->IsActive)
					{
						//SLOG::Log(_hiddenObjName);
						_objectInHand = i + 1;
						_hiddenObj->SetZ(_hiddenObj->ZOrder + 200);
						return;
					}
				}

				if (Control::Input->LBclicked() && !ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
				{
					Hud->ShowBannerText(descriptionText[activeHO]);
				}
			}

			else if (_objectInHand)
			{
				_hiddenObj->SetPos(&pos);

				string _slotName = "slide-" + MKSTR(MAX_L6S1_HOSCENES - (activeHO)) + "_Large0" + MKSTR(_objectInHand);
				CObject	*_slotObj = Ho_Scene[activeHO - 1]->GetObjectByID(_slotName);
				if (_slotObj->GetWorldRect().Intersection(&pos, 10) && Control::Input->LBclicked())
				{
					_hiddenObj->SetZ(_hiddenObj->ZOrder - 200);
					Control::Audio->QuickLoadAndPlaySFX("s1ho_pieceFixing");
					bool _isComplete = true;
					//SLOG::Log(_slotName);
					switch (activeHO)
					{
					case 1:
						ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S1[_objectInHand - 1] = true;
						for (int i = 0; i < 4; i++)
						{
							if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S1[i])
							{
								_isComplete = false;
							}
						}

						if (_isComplete)
						{
							_delay++;
							/*InitTweens(MAX_L6S1_HOSCENES - activeHO,numberOfHiddenObjects[activeHO - 1],numberOfHiddenObjects[activeHO]);*/
							ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[activeHO] = true;
							//activeHO++;
						}
						break;
					case 2:
						ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S2[_objectInHand - 1] = true;
						for (int i = 0; i < 2; i++)
						{
							if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S2[i])
							{
								_isComplete = false;
							}
						}

						if (_isComplete)
						{
							_delay++;
							//InitTweens(MAX_L6S1_HOSCENES - activeHO,numberOfHiddenObjects[activeHO - 1],numberOfHiddenObjects[activeHO]);
							ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[activeHO] = true;
							//activeHO++;
						}

						break;
					case 3:
						ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S3[_objectInHand - 1] = true;
						for (int i = 0; i < 3; i++)
						{
							if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S3[i])
							{
								_isComplete = false;
							}
						}

						if (_isComplete)
						{
							_delay++;
							//InitTweens(MAX_L6S1_HOSCENES - activeHO,numberOfHiddenObjects[activeHO - 1],numberOfHiddenObjects[activeHO]);
							ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[activeHO] = true;
							//activeHO++;
						}
						break;
					case 4:
						ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S4[_objectInHand - 1] = true;
						for (int i = 0; i < 4; i++)
						{
							if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S4[i])
							{
								_isComplete = false;
							}
						}

						if (_isComplete)
						{
							_delay++;
							//InitTweens(MAX_L6S1_HOSCENES - activeHO,numberOfHiddenObjects[activeHO - 1],numberOfHiddenObjects[activeHO]);
							ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[activeHO] = true;
							//activeHO++;
						}
						break;
					case 5:
						ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S5[_objectInHand - 1] = true;
						for (int i = 0; i < 2; i++)
						{
							if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S5[i])
							{
								_isComplete = false;
							}
						}

						if (_isComplete)
						{
							if(_handleExtrasGameComplete(false))
							{
								_objectInHand = 0;
								_slotObj->SetAlpha(1);
								_hiddenObj->SetEnable(false);
								SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
								return;
							}
							AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
							ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[activeHO] = true;

							//Journal Entry Add
							if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P3_DescriptionHOSolved])
							{
								//Flag Set
								ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P3_DescriptionHOSolved] = true;
								BtnMatch3->Show(false);
								BtnSwitchToHO->Show(false);

								//Add to Note Array
								ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P3_DescriptionHOSolved;

								ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

								//Increment Notes
								ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
								Hud->ShowJrnEntryFX();
							}

							HOScene->GetObjectByID("paptus")->SetEnable(true);
							HOScene->GetObjectByID("paptus")->FadeIn();

							Control::Audio->QuickLoadAndPlaySFX("s1ho_hoComplete");
							//SLOG::Log("Puzzle complete");
							HOScene->GetObjectByID("letter")->SetEnable(true);
							HOScene->GetObjectByID("lettertext2")->SetEnable(true);
							//HOScene->GetObjectByID("shadow")->SetEnable(true);
							HOScene->GetObjectByID("tongs")->SetEnable(true);
							Ho_Scene[MAX_L6S1_HOSCENES - 2]->FadeOut();
						}
						break;
					default:
						break;
					}
					_objectInHand = 0;
					_slotObj->SetAlpha(1);
					_hiddenObj->SetEnable(false);

				}
			}


		}

		

		if (!ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L6empty");
			}		
		}
	}



}

void L6_S1_DescriptionHO::InitTweens(int index, int numObj, int nextSceneNumObjects)
{
	Control::Audio->QuickLoadAndPlaySFX("s1ho_tileSliding");
	string _id;
	for (int i = 0; i < numObj; i++)
	{
		_id = "slide-" + MKSTR(index) + "_Large0" + MKSTR(i + 1);
		Ho_Scene[activeHO - 1]->GetObjectByID(_id)->TweenPosTo(-1036,0,0,1,0,true);
		_id = "slide-" + MKSTR(index) + "_Small0" + MKSTR(i + 1);
		Ho_Scene[activeHO - 1]->GetObjectByID(_id)->TweenPosTo(-1036,0,0,1,0,true);
	}
	_id = "slide-" + MKSTR(index) + "_Bg";
	Ho_Scene[activeHO - 1]->GetObjectByID(_id)->TweenPosTo(-1036,0,0,1,0,true,CC_CALLBACK_1(L6_S1_DescriptionHO::OnLeftMoveTweenComplete,this));
	HOScene->GetObjectByID("Slide_Depth")->SetEnable(true);
	HOScene->GetObjectByID("Slide_Depth")->TweenPosTo(-1036,0,0,1,false,true,CC_CALLBACK_1(L6_S1_DescriptionHO::OnDepthEffectTweenComplete,this));
	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[0])
	{
		HOScene->GetObjectByID("text1")->SetEnable(true);
		HOScene->GetObjectByID("text1")->TweenPosTo(-1036, 0, 0, 1, false, true, CC_CALLBACK_1(L6_S1_DescriptionHO::OnDepthEffectTweenComplete, this));
		HOScene->GetObjectByID("text2")->SetEnable(true);
		HOScene->GetObjectByID("text2")->TweenPosTo(-1036, 0, 0, 1, false, true, CC_CALLBACK_1(L6_S1_DescriptionHO::OnDepthEffectTweenComplete, this));
	}
	for (int i = 0; i < nextSceneNumObjects; i++)

	{
		_id = "slide-" + MKSTR(index - 1) + "_Large0" + MKSTR(i + 1);
		Ho_Scene[activeHO]->GetObjectByID(_id)->TweenPosTo(-1036,0,0,1,0,true);
		_id = "slide-" + MKSTR(index - 1) + "_Small0" + MKSTR(i + 1);
		Ho_Scene[activeHO]->GetObjectByID(_id)->TweenPosTo(-1036,0,0,1,0,true);
	}
	_id = "slide-" + MKSTR(index - 1) + "_Bg";
	Ho_Scene[activeHO]->GetObjectByID(_id)->TweenPosTo(-1036,0,0,1,0,true);
}


void L6_S1_DescriptionHO::ShowHintText(int currHO)
{
	CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);
	float vertGap = 26, horizontalGap = InvAreaRect.w/4.0f;
	CPoint pos(0,0,0);

	pos.x = InvAreaRect.Left() + horizontalGap*2;
	pos.y = InvAreaRect.Top() + vertGap*2;

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	CBitmapText *_hintText = new CBitmapText();
	_hintText->SetTextProps(StringTable::getStr(descriptionText[currHO]), pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_25, Align_Center);
	_hintText->SetZ(eZOrder_INVBOXITEMS);
	_hintText->SetScale(0.8f);
}

void L6_S1_DescriptionHO::ProcessNonHOHint()
{
	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		return;
	if(activePopupID != -1)
		return;

	bool haveHint = false;

	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced && ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken)
	{
		haveHint = true;
		Hud->ShowHintForInvItem(eInv_L6_S1_Emblem,&InvDestRectEmblem);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[MAX_L6S1_HOSCENES - 1] && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken)
	{
		haveHint = true;
		InitHintTrailEmit(HOScene->GetObjectByID("tongs")->GetPosPointer()->x, HOScene->GetObjectByID("tongs")->GetPosPointer()->y);
		AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	}
	else
	{
		switch (activeHO)
		{
		case 1:
			for (int i = 0; i < 4; i++)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S1[i])
				{
					haveHint = true;
					string _hiddenObjName = "slide-5_Small0" + MKSTR(i + 1);
					_hiddenObj = Ho_Scene[activeHO - 1]->GetObjectByID(_hiddenObjName);
					InitHintTrailEmit(_hiddenObj->GetPosPointer()->x, _hiddenObj->GetPosPointer()->y);
					string _sceneObjName = "slide-5_Large0" + MKSTR(i + 1);
					CObject *_sceneObj = Ho_Scene[activeHO - 1]->GetObjectByID(_sceneObjName);
					Hud->ShowHintForSceneItem(PointSystem::CreateCPoint(_sceneObj->GetPosPointer()->x,_sceneObj->GetPosPointer()->y,_sceneObj->GetPosPointer()->z));
					AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					break;
				}
			}
			break;
		case 2:
			for (int i = 0; i < 2; i++)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S2[i])
				{
					haveHint = true;
					string _hiddenObjName = "slide-4_Small0" + MKSTR(i + 1);
					_hiddenObj = Ho_Scene[activeHO - 1]->GetObjectByID(_hiddenObjName);
					InitHintTrailEmit(_hiddenObj->GetPosPointer()->x, _hiddenObj->GetPosPointer()->y);
					string _sceneObjName = "slide-4_Large0" + MKSTR(i + 1);
					CObject *_sceneObj = Ho_Scene[activeHO - 1]->GetObjectByID(_sceneObjName);
					Hud->ShowHintForSceneItem(PointSystem::CreateCPoint(_sceneObj->GetPosPointer()->x,_sceneObj->GetPosPointer()->y,_sceneObj->GetPosPointer()->z));
					AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					break;
				}
			}

			break;
		case 3:
			for (int i = 0; i < 3; i++)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S3[i])
				{
					haveHint = true;
					string _hiddenObjName = "slide-3_Small0" + MKSTR(i + 1);
					_hiddenObj = Ho_Scene[activeHO - 1]->GetObjectByID(_hiddenObjName);
					InitHintTrailEmit(_hiddenObj->GetPosPointer()->x, _hiddenObj->GetPosPointer()->y);
					string _sceneObjName = "slide-3_Large0" + MKSTR(i + 1);
					CObject *_sceneObj = Ho_Scene[activeHO - 1]->GetObjectByID(_sceneObjName);
					Hud->ShowHintForSceneItem(PointSystem::CreateCPoint(_sceneObj->GetPosPointer()->x,_sceneObj->GetPosPointer()->y,_sceneObj->GetPosPointer()->z));
					AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					break;
				}
			}
			break;
		case 4:
			for (int i = 0; i < 4; i++)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S4[i])
				{
					haveHint = true;
					string _hiddenObjName = "slide-2_Small0" + MKSTR(i + 1);
					_hiddenObj = Ho_Scene[activeHO - 1]->GetObjectByID(_hiddenObjName);
					InitHintTrailEmit(_hiddenObj->GetPosPointer()->x, _hiddenObj->GetPosPointer()->y);
					string _sceneObjName = "slide-2_Large0" + MKSTR(i + 1);
					CObject *_sceneObj = Ho_Scene[activeHO - 1]->GetObjectByID(_sceneObjName);
					Hud->ShowHintForSceneItem(PointSystem::CreateCPoint(_sceneObj->GetPosPointer()->x,_sceneObj->GetPosPointer()->y,_sceneObj->GetPosPointer()->z));
					AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					break;
				}
			}
			break;
		case 5:
			for (int i = 0; i < 2; i++)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S5[i])
				{
					haveHint = true;
					string _hiddenObjName = "slide-1_Small0" + MKSTR(i + 1);
					_hiddenObj = Ho_Scene[activeHO - 1]->GetObjectByID(_hiddenObjName);
					InitHintTrailEmit(_hiddenObj->GetPosPointer()->x, _hiddenObj->GetPosPointer()->y);
					string _sceneObjName = "slide-1_Large0" + MKSTR(i + 1);
					CObject *_sceneObj = Ho_Scene[activeHO - 1]->GetObjectByID(_sceneObjName);
					Hud->ShowHintForSceneItem(PointSystem::CreateCPoint(_sceneObj->GetPosPointer()->x,_sceneObj->GetPosPointer()->y,_sceneObj->GetPosPointer()->z));
					AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					break;
				}
			}
			break;
		default:
			break;
		}

	}

	if (!haveHint)
		Hud->ShowNothingToDoMsgBanner();
}


void L6_S1_DescriptionHO::Render(const CPoint* pCam)
{

}

void L6_S1_DescriptionHO::Required()
{
	if( Control::Audio->IsPlaying(aTrackL6theme) )
	{
		Control::Audio->Samples[aTrackL6theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL6theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL6theme);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL6ho) )
				Control::Audio->PlaySample(aTrackL6ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL6ho))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL6ho, true);
		}
	}
}

L6_S1_DescriptionHO::~L6_S1_DescriptionHO()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	//SAFE_DELETE(Back_Btn);
	//SAFE_DELETE(Next_Btn);
	SAFE_DELETE(popNote);

	for (int i = 1; i < MAX_L6S1_HOSCENES; i++)
	{
		SAFE_DELETE(Ho_Scene[i - 1]);
	}
	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL6DescS1_1);
	Control::Audio->UnloadSample(aVOL6DescS1_2);
	Control::Audio->UnloadSample(aVOL6DescS1_3);
	Control::Audio->UnloadSample(aVOL6DescS1_4);
	Control::Audio->UnloadSample(aVOL6DescS1_5);
	Control::Audio->StopSample(aAmbL6s1DescriptionHOAmb);
	Control::Audio->UnloadSample(aAmbL6s1DescriptionHOAmb);
	Control::Audio->StopSample(aVOL6NoteVOS4_1);
	Control::Audio->UnloadSample(aVOL6MaryRefS4_1);
	Control::Audio->UnloadSample(aVOL6MaryRefS5_1);
	//MUSIC_SFX

	SAFE_DELETE(ResetDia);
	SAFE_DELETE(ResetBtn);
}




//void L6_S1_DescriptionHO::OnBackArrowClick(CObject * obj)
//{
//	SLOG::Log("back btn click");
//}
//
//void L6_S1_DescriptionHO::OnNextArrowClick(CObject * obj)
//{
//	SLOG::Log("next btn click");
//}


void L6_S1_DescriptionHO::ResetHO()
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

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Emblem] = true;
			ProfileSystem::Instance->ActiveProfile.L6_S1_EmblemPlaced = false;
			ProfileSystem::Instance->ActiveProfile.L6_S1_TextForHOShown = false;

			for (int i = 0; i < MAX_L6S1_HOSCENES; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L6_S1_SceneHOSolved[i] = false;
			}
			ProfileSystem::Instance->ActiveProfile.L6_S1_DescriptionHOSolved = false;

			for (int i = 0; i < 4; i++)
				ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S1[i] = false;
			for (int i = 0; i < 2; i++)
				ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S2[i] = false;
			for (int i = 0; i < 3; i++)
				ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S3[i] = false;
			for (int i = 0; i < 4; i++)
				ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S4[i] = false;
			for (int i = 0; i < 2; i++)
				ProfileSystem::Instance->ActiveProfile.L6_S1_DescHOFound_S5[i] = false;
			ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken = false;
			ProfileSystem::Instance->ActiveProfile.L6_S1_LetterTaken = false;
			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_DescriptionHO;
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
void L6_S1_DescriptionHO::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				//Cursor::SetMode(CA_ExitUp);
				Hud->InvBoxCtrl->ResetToInvBox();
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