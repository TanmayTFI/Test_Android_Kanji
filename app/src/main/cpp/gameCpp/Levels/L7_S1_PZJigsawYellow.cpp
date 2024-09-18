//====================================
//  created by : faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S1_PZJigsawYellow.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

using namespace JigsawYellow;

const CRectangle  InfoRectJigsaw(229.0f,22.0f,911.0f,420.0f);

L7_S1_PZJigsawYellow::L7_S1_PZJigsawYellow()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S1\\PZ1\\PZ1.SCN");

	HOScene->GetObjectByID("ray1")->SetScale(2.00f);
	HOScene->GetObjectByID("ray2")->SetScale(2.00f);
	HOScene->GetObjectByID("ray3")->SetScale(2.10f);


	HOScene->GetObjectByID("ray1")->Animate(0.55f,0.85f,4.0f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("ray2")->Animate(0.45f,0.65f,5.0f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("ray3")->Animate(0.35f,0.75f,6.0f,0,eBlend_AlphaAdditive);

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

	s1pzmoonraysFX = new SuperFX("PRT\\L7\\s1pzmoonrays.INI", PointSystem::CreateCPoint(1246,79,0), 1100); 
	s1pzmoonraysFX->Start();
	s1pzmoonraysFX->AdvanceByTime(2.0f);

	s1pzglareFX = new SuperFX("PRT\\L7\\s1pzglare.INI", PointSystem::CreateCPoint(932,52,0), 1100); 
	s1pzglareFX->Start();
	s1pzglareFX->AdvanceByTime(2.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	//Init puzzle
	{
		isDragging = false;
		dragID = -1;

		//Init Poly
		CPolygon* scnPoly[MAX_L7S1_JIGSAWPIECES];
		int pts0[] = {1010,390,965,432,975,450,1098,428};
		scnPoly[0]=  new CPolygon(4);
		scnPoly[0]->Init(pts0);
		int pts1[] = {887,449,817,440,780,446,690,490,713,518,832,502,888,457};
		scnPoly[1]=  new CPolygon(7);
		scnPoly[1]->Init(pts1);
		int pts2[] = {1093,459,1011,451,989,461,1006,502,1030,512,1115,515};
		scnPoly[2]=  new CPolygon(6);
		scnPoly[2]->Init(pts2);
		int pts3[] = {904,370,842,382,819,381,786,428,898,448,908,422,904,388};
		scnPoly[3]=  new CPolygon(7);
		scnPoly[3]->Init(pts3);
		int pts4[] = {890,450,964,461,971,481,926,529,863,500,838,466,863,447};
		scnPoly[4]=  new CPolygon(7);
		scnPoly[4]->Init(pts4);
		int pts5[] = {609,519,584,542,662,618,744,573,811,582,750,519};
		scnPoly[5]=  new CPolygon(6);
		scnPoly[5]->Init(pts5);
		int pts6[] = {780,507,743,537,746,549,864,557,871,546,863,532};
		scnPoly[6]=  new CPolygon(6);
		scnPoly[6]->Init(pts6);
		int pts7[] = {888,543,835,589,900,620,979,566,934,548};
		scnPoly[7]=  new CPolygon(5);
		scnPoly[7]->Init(pts7);
		int pts8[] = {322,518,301,533,229,551,323,617,359,589,371,526};
		scnPoly[8]=  new CPolygon(6);
		scnPoly[8]->Init(pts8);
		int pts9[] = {1008,508,961,525,973,557,1083,555};
		scnPoly[9]=  new CPolygon(4);
		scnPoly[9]->Init(pts9);
		int pts10[] = {662,403,635,390,521,387,483,416,597,480,605,436,662,415};
		scnPoly[10]=  new CPolygon(7);
		scnPoly[10]->Init(pts10);
		int pts11[] = {659,457,612,466,588,500,696,507};
		scnPoly[11]=  new CPolygon(4);
		scnPoly[11]->Init(pts11);
		int pts12[] = {453,545,523,568,506,593,423,616,347,620,379,578};
		scnPoly[12]=  new CPolygon(6);
		scnPoly[12]->Init(pts12);
		int pts13[] = {913,414,913,424,985,405,945,383};
		scnPoly[13]=  new CPolygon(4);
		scnPoly[13]->Init(pts13);
		int pts14[] = {669,415,667,455,761,439,781,419,724,402};
		scnPoly[14]=  new CPolygon(5);
		scnPoly[14]->Init(pts14);
		int pts15[] = {580,483,479,529,479,541,562,587,583,521};
		scnPoly[15]=  new CPolygon(5);
		scnPoly[15]->Init(pts15);
		int pts16[] = {378,496,502,438,531,454,543,473,519,504,403,525};
		scnPoly[16]=  new CPolygon(6);
		scnPoly[16]->Init(pts16);
		int pts17[] = {313,413,405,416,440,400,464,422,370,495,342,488,266,484};
		scnPoly[17]=  new CPolygon(7);
		scnPoly[17]->Init(pts17);

		HOScene->GetObjectByID("fixitemshadow")->SetBlendMode(eBlend_AlphaSubtractive);

		for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
		{
			JigsawParts[i] = new JigsawPart();
			JigsawParts[i]->setIndex(i);
			JigsawParts[i]->setSceneObject(HOScene->GetObjectByID(std::string("scnpart" + MKSTR(i)).c_str()));
			JigsawParts[i]->setFixObject(HOScene->GetObjectByID(std::string("fixpart" + MKSTR(i)).c_str()));
			JigsawParts[i]->setShadow(HOScene->GetObjectByID("fixitemshadow"));
			JigsawParts[i]->setPolygonArea(scnPoly[i]);

			JigsawParts[i]->InitObject(CC_CALLBACK_2(L7_S1_PZJigsawYellow::OnJigsawPlacedCallback, this));

			if(ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowPlaced[i])
				JigsawParts[i]->AlreadyFixed();
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted)
	{
		Hud->InvBox_StartPuzzleMode();
		HOScene->GetObjectByID("symbolfinal")->SetEnable(false);
	}
	else
	{
		for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
		{
			JigsawParts[i]->AlreadyFixed();
		}
	}

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s1VolcanoAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s1VolcanoAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s1VolcanoAmb,true);
	}
	
	//MUSIC_SFX
	
	

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S1_PZJigsawYellow]  = true;
}

void L7_S1_PZJigsawYellow::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL7theme) )
	{
		Control::Audio->Samples[aTrackL7theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL7theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL7theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL7puzzle) )
				  Control::Audio->PlaySample(aTrackL7puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL7puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL7puzzle, true);
		}
	}
}

void L7_S1_PZJigsawYellow::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
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
	bool clicked = Control::Input->LBclicked();
	Cursor::SetMode(CA_Normal);

	if(ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted)
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;

		if(InfoRectJigsaw.Intersection(&pos))
		{
			Cursor::SetMode(CA_Help);
			if(clicked)
				Hud->ShowBannerText("L7better");
		}
	}
	else
	{
		if(isDragging)
		{
			if(dragID!=-1)
				JigsawParts[dragID]->HandleMouseUpdate(pos, clicked);
		}
		else
		{
			for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
			{
				if(JigsawParts[i]->HandleMouseUpdate(pos, clicked))
				{
					dragID = i;
					isDragging = true;
					break;
				}
			} 
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	

	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L7_S1_Volcano);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L7_S1_Volcano);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S1_Volcano, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	}
}

void L7_S1_PZJigsawYellow::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L7_S1_PZJigsawYellow::OnJigsawPlacedCallback(int index, bool isFixed)
{
	isDragging = false;
	if(isFixed)
	{
		ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowPlaced[index] = true;
		//Profile Save and check for completion

		bool isWon = true;
		for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowPlaced[i])
			{
				isWon = false;
				break;
			}
		}

		if(isWon)
			HandlePuzzleComplete();
	}
}

void L7_S1_PZJigsawYellow::HandlePuzzleComplete()
{
	
	ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted = true;
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);

	HOScene->GetObjectByID("symbolfinal")->SetEnable(true);
	HOScene->GetObjectByID("symbolfinal")->FadeIn(1.0f);

	Control::Audio->QuickLoadAndPlaySFX("jigsaw_puzzlesolved");

	if(_handleExtrasGameComplete())
		return;

	Hud->InvBox_StopPuzzleMode();

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano] = true;
}


void L7_S1_PZJigsawYellow::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	if(!ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted)
	{
		if(isDragging)
			isDragging = false;

		for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
		{
			JigsawParts[i]->AlreadyFixed();
		}
		HandlePuzzleComplete();
	}
}

void L7_S1_PZJigsawYellow::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: ++iNonHOHint;

			break;
		case 1: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted)
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 2: ++iNonHOHint;

			break;
		case 3: ++iNonHOHint;

			break;
		case 4: ++iNonHOHint;

			break;
		case 5: ++iNonHOHint;

			break;
		case 6: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 7: ++iNonHOHint;
				{
					Hud->teleportSystem->ShowTeleport();
					return;
				}
				break;

		}
	}
	while( iNonHOHint < MaxNonHOHints );

	Hud->ShowNothingToDoMsgBanner();
}

L7_S1_PZJigsawYellow::~L7_S1_PZJigsawYellow()
{
	Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(s1pzmoonraysFX);
	SAFE_DELETE(s1pzglareFX);

	for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
	{
		SAFE_DELETE(JigsawParts[i]);
	} 
	
	SAFE_DELETE(ResetDia);
		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s1VolcanoAmb);
	Control::Audio->UnloadSample(aAmbL7s1VolcanoAmb);
	//MUSIC_SFX
}


JigsawPart::JigsawPart()
{
	_FIX_GAP_THRESHOLD = 100.0f;
	_fixPos = NULL;

	FixFX = NULL;

	_index = 0;
	_scnObj = NULL;
	_shadowObj = NULL;
	_fixObj = NULL;
	_polygonArea = NULL;

	_isFixed = false;
	_isActive = true;

	_isDragging = false;
}

JigsawPart::~JigsawPart()
{
	SAFE_DELETE(_polygonArea);
	SAFE_DELETE(_fixPos);
	_scnObj = NULL;
	_shadowObj = NULL;
	_fixObj = NULL;
}

void JigsawPart::InitObject(std::function<void (int, bool)> jigsawPlaceCallback)
{
	_jigsawPlaceCallback = jigsawPlaceCallback;

	_shadowObj->Show(false);
	_fixObj->Show(false);

	_fixPos = new CPoint(_fixObj->Pos);
}

bool JigsawPart::HandleMouseUpdate(CPoint pos, bool clicked)
{
	if(!_isFixed)
	{
		if(_isDragging)
		{
			Cursor::SetMode(CA_HandPointy);
			_fixObj->SetPos(&pos);
			_shadowObj->SetPos(&pos);
			CPoint offset(10,10,0);
			_shadowObj->SetRelativePos(&offset);
			if(clicked)
			{
				_CheckAndPlaceJigsaw();
			}
		}
		else
		{
			_scnObj->PlayAnimation(0);
			if(_polygonArea->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				_scnObj->PlayAnimation(1);
				if(clicked)
				{
					_fixObj->SetPos(&pos);
					_shadowObj->SetPos(&pos);
					CPoint offset(10,10,0);
					_shadowObj->SetRelativePos(&offset);
					_DragStart();
					return true;
				}
			}
		}
	}
	return false;
}

void JigsawPart::AlreadyFixed(){
	_isFixed = true;

	_fixObj->SetPos(_fixPos);
	_fixObj->Show(true);
	_fixObj->SetZ(10);
	_scnObj->Show(false);
}

void JigsawPart::_DragStart(){
	_isDragging = true;

	Control::Audio->QuickLoadAndPlaySFX("jigsaw_piececlicked");

	_shadowObj->Show(true);

	_shadowObj->SetZ(40);
	_fixObj->SetZ(41);
	_fixObj->Show(true);
	_scnObj->Show(false);
}

void JigsawPart::_CheckAndPlaceJigsaw(){
	_isDragging = false;
	bool isWithinFixArea = (abs(_fixObj->Pos.x - _fixPos->x) < _FIX_GAP_THRESHOLD) && (abs(_fixObj->Pos.y - _fixPos->y) < _FIX_GAP_THRESHOLD);
	if(isWithinFixArea)
	{
		//place it
		_fixObj->SetPos(_fixPos);
		_isFixed = true;
		_shadowObj->Show(false);
		_fixObj->SetZ(10);

		Control::Audio->QuickLoadAndPlaySFX("jigsaw_placedright");

		//TODO callback to scene
		if(_jigsawPlaceCallback!=nullptr)
			_jigsawPlaceCallback(_index, true);
	}
	else
	{
		Control::Audio->QuickLoadAndPlaySFX("jigsaw_placedwrong");

		//reset it
		_fixObj->Show(false);
		_scnObj->PlayAnimation(0);
		_scnObj->Show(true);
		_shadowObj->Show(false);

		if(_jigsawPlaceCallback!=nullptr)
			_jigsawPlaceCallback(_index, false);
	}
}

void L7_S1_PZJigsawYellow::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted)
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
			for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
				ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowPlaced[i] = false;
			ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted = false;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_PZJigsawYellow;
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
void L7_S1_PZJigsawYellow::NavigateToScene()
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