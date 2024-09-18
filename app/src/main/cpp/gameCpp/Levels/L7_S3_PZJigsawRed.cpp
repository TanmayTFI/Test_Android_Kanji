//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S3_PZJigsawRed.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

using namespace JigsawRed;

const CRectangle  InfoRectJigsaw(282.0f,17.0f,795.0f,416.0f);

L7_S3_PZJigsawRed::L7_S3_PZJigsawRed()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S3\\PZ1\\PZ1.SCN");

	HOScene->GetObjectByID("ray1")->SetScale(1.50f);
	HOScene->GetObjectByID("ray2")->SetScale(1.13f);
	HOScene->GetObjectByID("ray3")->SetScale(1.33f);
	HOScene->GetObjectByID("ray4")->SetScale(1.33f);

	HOScene->GetObjectByID("ray1")->Animate(0.55f,0.85f,4.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("ray2")->Animate(0.45f,0.95f,5.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("ray3")->Animate(0.35f,0.98f,3.8f,0,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("ray4")->Animate(0.45f,0.85f,5.0f,4.0f,eBlend_AlphaAdditive);	

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

	s3pzglareFX = new SuperFX("PRT\\L7\\s3pzglare.INI", PointSystem::CreateCPoint(746,100,0), 1100); 
	s3pzglareFX->Start();
	s3pzglareFX->AdvanceByTime(2.0f);

	s3pzmoonraysFX = new SuperFX("PRT\\L7\\s3pzmoonrays.INI", PointSystem::CreateCPoint(93,111,0), 1100); 
	s3pzmoonraysFX->Start();
	s3pzmoonraysFX->AdvanceByTime(2.0f);		

	//Init puzzle
	{
		isDragging = false;
		dragID = -1;

		//Init Poly
		CPolygon* scnPoly[MAX_L7S3_JIGSAWPIECES];
		int pts0[] = {688,553,692,582,821,617,835,617,880,590,881,573,834,549,755,541};
		scnPoly[0]=  new CPolygon(8);
		scnPoly[0]->Init(pts0);
		int pts1[] = {354,421,318,475,329,483,454,458,449,446};
		scnPoly[1]=  new CPolygon(5);
		scnPoly[1]->Init(pts1);
		int pts2[] = {380,487,385,517,404,533,474,543,504,572,549,524,542,471};
		scnPoly[2]=  new CPolygon(7);
		scnPoly[2]->Init(pts2);
		int pts3[] = {932,412,906,452,909,461,1033,458,1059,443,1015,410};
		scnPoly[3]=  new CPolygon(6);
		scnPoly[3]->Init(pts3);
		int pts4[] = {561,531,531,554,500,601,513,609,689,608,689,596,642,547};
		scnPoly[4]=  new CPolygon(7);
		scnPoly[4]->Init(pts4);
		int pts5[] = {883,461,851,478,885,555,896,551,947,501,1018,500,997,472};
		scnPoly[5]=  new CPolygon(7);
		scnPoly[5]->Init(pts5);
		int pts6[] = {684,467,640,456,609,464,572,490,566,515,658,540};
		scnPoly[6]=  new CPolygon(6);
		scnPoly[6]->Init(pts6);
		int pts7[] = {649,412,652,428,661,435,655,449,666,457,776,466,771,445,738,409,693,398};
		scnPoly[7]=  new CPolygon(9);
		scnPoly[7]->Init(pts7);
		int pts8[] = {753,470,726,479,723,510,742,521,852,553,858,526,845,480,812,468};
		scnPoly[8]=  new CPolygon(8);
		scnPoly[8]->Init(pts8);
		int pts9[] = {767,410,765,419,793,443,792,451,841,471,904,445,903,430,838,404,808,408};
		scnPoly[9]=  new CPolygon(9);
		scnPoly[9]->Init(pts9);
		int pts10[] = {520,391,458,421,464,445,507,460,558,465,648,437,628,414,574,391};
		scnPoly[10]=  new CPolygon(8);
		scnPoly[10]->Init(pts10);
		int pts11[] = {951,513,896,571,898,584,939,576,1062,588,1064,561,1050,543};
		scnPoly[11]=  new CPolygon(7);
		scnPoly[11]->Init(pts11);
		int pts12[] = {1045,513,1066,554,1106,572,1158,574,1208,558,1203,539,1134,517,1076,517};
		scnPoly[12]=  new CPolygon(8);
		scnPoly[12]->Init(pts12);
		int pts13[] = {369,548,384,589,418,638,456,588,465,547,401,546};
		scnPoly[13]=  new CPolygon(6);
		scnPoly[13]->Init(pts13);
		int pts14[] = {1006,465,1006,474,1028,504,1107,511,1154,505,1185,506,1184,495,1137,472,1075,465,1054,457};
		scnPoly[14]=  new CPolygon(10);
		scnPoly[14]->Init(pts14);
		int pts15[] = {255,482,295,496,344,491,374,497,383,535,342,555,301,542,202,546,203,530};
		scnPoly[15]=  new CPolygon(9);
		scnPoly[15]->Init(pts15);

		HOScene->GetObjectByID("fixitemshadow")->SetBlendMode(eBlend_AlphaSubtractive);

		for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
		{
			JigsawParts[i] = new JigsawPart();
			JigsawParts[i]->setIndex(i);
			JigsawParts[i]->setSceneObject(HOScene->GetObjectByID(std::string("scnpart" + MKSTR(i)).c_str()));
			JigsawParts[i]->setFixObject(HOScene->GetObjectByID(std::string("fixpart" + MKSTR(i)).c_str()));
			JigsawParts[i]->setShadow(HOScene->GetObjectByID("fixitemshadow"));
			JigsawParts[i]->setPolygonArea(scnPoly[i]);

			JigsawParts[i]->InitObject(CC_CALLBACK_2(L7_S3_PZJigsawRed::OnJigsawPlacedCallback, this));

			if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedPlaced[i])
				JigsawParts[i]->AlreadyFixed();
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted)
	{
		Hud->InvBox_StartPuzzleMode();
		HOScene->GetObjectByID("symbolfinal")->SetEnable(false);
	}
	else
	{
		for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
		{
			JigsawParts[i]->AlreadyFixed();
		}
	}


	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s3HavenAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s3HavenAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s3HavenAmb,true);
	}
	
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S3_PZJigsawRed]  = true;
}

void L7_S3_PZJigsawRed::Required()
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

void L7_S3_PZJigsawRed::Update()
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

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted)
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
			for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
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
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_Haven;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) && !isMouseOverAnyPart)
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown, eArea_L7_S3_Haven);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L7_S3_Haven);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S3_Haven, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_Haven;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	}
}

void L7_S3_PZJigsawRed::ReceiveMessage(int val)
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

void L7_S3_PZJigsawRed::OnJigsawPlacedCallback(int index, bool isFixed)
{
	isDragging = false;
	if(isFixed)
	{
		ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedPlaced[index] = true;
		//Profile Save and check for completion

		bool isWon = true;
		for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedPlaced[i])
			{
				isWon = false;
				break;
			}
		}

		if(isWon)
			HandlePuzzleComplete();
	}
}

void L7_S3_PZJigsawRed::HandlePuzzleComplete()
{
	
	ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted = true;
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);

	HOScene->GetObjectByID("symbolfinal")->SetEnable(true);
	HOScene->GetObjectByID("symbolfinal")->FadeIn(1.0f);

	Control::Audio->QuickLoadAndPlaySFX("jigsaw_puzzlesolved");

	if(_handleExtrasGameComplete())
		return;

	Hud->InvBox_StopPuzzleMode();

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano] = true;
}

void L7_S3_PZJigsawRed::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted)
	{
		if(isDragging)
			isDragging = false;

		for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
		{
			JigsawParts[i]->AlreadyFixed();
		}
		HandlePuzzleComplete();
	}
}

void L7_S3_PZJigsawRed::ProcessNonHOHint()
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
			if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted)
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

L7_S3_PZJigsawRed::~L7_S3_PZJigsawRed()
{
	Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(s3pzglareFX);
	SAFE_DELETE(s3pzmoonraysFX);

	for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
	{
		SAFE_DELETE(JigsawParts[i]);
	}
	SAFE_DELETE(ResetDia);
	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s3HavenAmb);
	Control::Audio->UnloadSample(aAmbL7s3HavenAmb);
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

void L7_S3_PZJigsawRed::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted)
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
			for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
				ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedPlaced[i] = false;
			ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted = false;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_PZJigsawRed;
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
void L7_S3_PZJigsawRed::NavigateToScene()
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