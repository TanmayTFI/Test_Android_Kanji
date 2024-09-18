//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S4_PZJigsawBlue.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

using namespace JigsawBlue;

const CRectangle  InfoRectJigsaw(246.0f,9.0f,870.0f,410.0f);

L7_S4_PZJigsawBlue::L7_S4_PZJigsawBlue()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S4\\PZ1\\PZ1.SCN");

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

	//Init puzzle
	{
		isDragging = false;
		dragID = -1;

		//Init Poly
		CPolygon* scnPoly[MAX_L7S4_JIGSAWPIECES];
		int pts0[] = {688,395,686,419,647,449,531,454,522,425,558,404,623,404};
		scnPoly[0]=  new CPolygon(7);
		scnPoly[0]->Init(pts0);
		int pts1[] = {280,454,407,438,411,448,390,496,361,507,338,503,325,485,306,497,275,492};
		scnPoly[1]=  new CPolygon(9);
		scnPoly[1]->Init(pts1);
		int pts2[] = {589,567,614,547,762,509,775,526,772,576,767,595,713,573,650,587,624,607};
		scnPoly[2]=  new CPolygon(9);
		scnPoly[2]->Init(pts2);
		int pts3[] = {818,512,845,531,919,546,912,584,800,594,804,576,780,578};
		scnPoly[3]=  new CPolygon(7);
		scnPoly[3]->Init(pts3);
		int pts4[] = {424,509,276,541,278,555,371,595,432,587,452,537};
		scnPoly[4]=  new CPolygon(6);
		scnPoly[4]->Init(pts4);
		int pts5[] = {495,463,561,460,565,471,570,569,500,561,484,546,454,528,467,479};
		scnPoly[5]=  new CPolygon(8);
		scnPoly[5]->Init(pts5);
		int pts6[] = {979,475,1031,477,1097,503,1079,541,1072,586,979,568,909,534,902,508,913,494};
		scnPoly[6]=  new CPolygon(9);
		scnPoly[6]->Init(pts6);
		int pts7[] = {810,461,836,515,877,513,886,498};
		scnPoly[7]=  new CPolygon(4);
		scnPoly[7]->Init(pts7);
		int pts8[] = {1041,356,1023,363,1024,430,1041,442,1089,429,1100,384,1066,365};
		scnPoly[8]=  new CPolygon(7);
		scnPoly[8]->Init(pts8);
		int pts9[] = {1053,460,1092,492,1186,503,1192,480,1131,454,1087,447};
		scnPoly[9]=  new CPolygon(6);
		scnPoly[9]->Init(pts9);
		int pts10[] = {907,404,901,431,910,444,898,479,972,454,1032,461,1032,452,983,417};
		scnPoly[10]=  new CPolygon(8);
		scnPoly[10]->Init(pts10);
		int pts11[] = {819,386,866,386,889,395,893,422,856,471,835,446,837,417,816,402};
		scnPoly[11]=  new CPolygon(8);
		scnPoly[11]->Init(pts11);
		int pts12[] = {701,395,746,403,779,391,807,397,826,414,826,438,775,468,698,408};
		scnPoly[12]=  new CPolygon(8);
		scnPoly[12]->Init(pts12);
		int pts13[] = {374,372,364,405,355,417,365,424,517,421,518,404,500,379,467,365};
		scnPoly[13]=  new CPolygon(8);
		scnPoly[13]->Init(pts13);
		int pts14[] = {682,444,650,453,643,469,598,502,583,535,718,510,765,495,765,470,755,463,713,453};
		scnPoly[14]=  new CPolygon(10);
		scnPoly[14]->Init(pts14);


		HOScene->GetObjectByID("fixitemshadow")->SetBlendMode(eBlend_AlphaSubtractive);

		for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
		{
			JigsawParts[i] = new JigsawPart();
			JigsawParts[i]->setIndex(i);
			JigsawParts[i]->setSceneObject(HOScene->GetObjectByID(std::string("scnpart" + MKSTR(i)).c_str()));
			JigsawParts[i]->setFixObject(HOScene->GetObjectByID(std::string("fixpart" + MKSTR(i)).c_str()));
			JigsawParts[i]->setShadow(HOScene->GetObjectByID("fixitemshadow"));
			JigsawParts[i]->setPolygonArea(scnPoly[i]);

			JigsawParts[i]->InitObject(CC_CALLBACK_2(L7_S4_PZJigsawBlue::OnJigsawPlacedCallback, this));

			if(ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBluePlaced[i])
				JigsawParts[i]->AlreadyFixed();
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted)
	{
		Hud->InvBox_StartPuzzleMode();
		HOScene->GetObjectByID("symbolfinal")->SetEnable(false);
	}
	else
	{
		for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
		{
			JigsawParts[i]->AlreadyFixed();
		}
	}

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s4AmeehaAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s4AmeehaAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s4AmeehaAmb,true);
	}
	
	//MUSIC_SFX
	
	

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S4_PZJigsawBlue]  = true;
}

void L7_S4_PZJigsawBlue::Required()
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

void L7_S4_PZJigsawBlue::Update()
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

	bool isMouseOverAnyPart = false;

	if(ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted)
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
			for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
			{
				if(JigsawParts[i]->HandleMouseUpdate(pos, clicked))
				{
					isMouseOverAnyPart = true;
					if(JigsawParts[i]->getIsClicked())
					{
						dragID = i;
						isDragging = true;
					}
					break;
				}
			} 
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	//if (Control::Input->isKeyBackSensed() && !isMouseOverAnyPart)
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S4_Ameeha;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) && !isMouseOverAnyPart)
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L7_S4_Ameeha);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L7_S4_Ameeha);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S4_Ameeha, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S4_Ameeha;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	}
}

void L7_S4_PZJigsawBlue::OnJigsawPlacedCallback(int index, bool isFixed)
{
	isDragging = false;
	if(isFixed)
	{
		ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBluePlaced[index] = true;
		//Profile Save and check for completion

		bool isWon = true;
		for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBluePlaced[i])
			{
				isWon = false;
				break;
			}
		}

		if(isWon)
			HandlePuzzleComplete();
	}
}

void L7_S4_PZJigsawBlue::HandlePuzzleComplete()
{
	ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted = true;
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);

	HOScene->GetObjectByID("symbolfinal")->SetEnable(true);
	HOScene->GetObjectByID("symbolfinal")->FadeIn(1.0f);

	Control::Audio->QuickLoadAndPlaySFX("jigsaw_puzzlesolved");

	if(_handleExtrasGameComplete())
		return;

	Hud->InvBox_StopPuzzleMode();

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano] = true;
}

void L7_S4_PZJigsawBlue::ReceiveMessage(int val)
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

void L7_S4_PZJigsawBlue::ProcessSkip()
{
	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted)
	{
		if(isDragging)
			isDragging = false;

		for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
		{
			JigsawParts[i]->AlreadyFixed();
		}
		HandlePuzzleComplete();
	}
}

void L7_S4_PZJigsawBlue::ProcessNonHOHint()
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
			if(ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted)
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

L7_S4_PZJigsawBlue::~L7_S4_PZJigsawBlue()
{
	Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(s1pzglareFX);
	SAFE_DELETE(s1pzmoonraysFX);

	for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
	{
		SAFE_DELETE(JigsawParts[i]);
	}
	
	SAFE_DELETE(ResetDia);
		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s4AmeehaAmb);
	Control::Audio->UnloadSample(aAmbL7s4AmeehaAmb);
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

	_isClicked = false;
	_isFixed = false;
	_isActive = true;

	_isDragging = false;
}

JigsawPart::~JigsawPart()
{
	SAFE_DELETE(_polygonArea);
	SAFE_DELETE(_fixPos);
	SAFE_DELETE(FixFX);

	_scnObj = NULL;
	_shadowObj = NULL;
	_fixObj = NULL;
}

void JigsawPart::InitObject(std::function<void (int, bool)> jigsawPlaceCallback)
{
	_jigsawPlaceCallback = jigsawPlaceCallback;

	_shadowObj->Show(false);
	_fixObj->Show(false);

	FixFX = new SuperFX("PRT\\L7\\s3pzfix.INI", PointSystem::CreateCPoint(567,319,0), 5);
	FixFX->SetPos(&_fixObj->Pos);

	_fixPos = new CPoint(_fixObj->Pos);
}

bool JigsawPart::HandleMouseUpdate(CPoint pos, bool clicked)
{
	bool isMouseOverOrClicked=false;
	_isClicked = false;
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
				isMouseOverOrClicked = true;
				Cursor::SetMode(CA_HandPointy);
				_scnObj->PlayAnimation(1);
				if(clicked)
				{
					_isClicked = true;
					_fixObj->SetPos(&pos);
					_shadowObj->SetPos(&pos);
					CPoint offset(10,10,0);
					_shadowObj->SetRelativePos(&offset);
					_DragStart();
				}
			}
		}
	}
	return isMouseOverOrClicked;
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
		FixFX->Start();
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

void L7_S4_PZJigsawBlue::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted)
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
			for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
				ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBluePlaced[i] = false;
			ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted = false;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S4_PZJigsawBlue;
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
void L7_S4_PZJigsawBlue::NavigateToScene()
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