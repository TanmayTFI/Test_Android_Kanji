//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 05 : MOOPAANS DEN
//====================================

#include "L4_S2_FanPuzzle.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Polygon.h"
#include "Application.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"

static float angles[12] = {41.53f,33.61f,25.55f,17.38f,8.52f,0,-8.62f,-16.85f,-24.93f,-33.14f,-40.80f,-48.83f};
static float clickAreaAngles[13] = {-130, -121.72f, -113.5f, -105.7f, -96, -88, -80, -71, -64, -55.5f, -47.5f, -39.6f, -20.0f};
static int shuffledOrder[12] = {0, 6, 1, 10, 3, 7, 5, 8, 4, 2, 9, 11};
//static int shuffledOrder[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
L4_S2_FanPuzzle::L4_S2_FanPuzzle()
{

	_fanAreaPolygon			= NULL;
	_jsonAnimInsertKey		= NULL;
	_jsonAnimRotateKey		= NULL;
	_jsonAnimOpenDoor		= NULL;
	_jsonAnimStuckKey		= NULL;
	_keyDropArea			= NULL;

	_prevSelectedFan = NULL;
	_currentSelectedFan = NULL;
	_isMouseInteractionOnFanEnabled = false;
	_jsonAnimInsertKey = NULL;
	_jsonAnimRotateKey = NULL;
	_jsonAnimStuckKey = NULL;
	_jsonAnimOpenDoor = NULL;
	InitializeWithProfileData();

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s2FanPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s2FanPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s2FanPuzzleAmb,true);
	}

	//MUSIC_SFX
}

L4_S2_FanPuzzle::~L4_S2_FanPuzzle()
{
	Hud->InvBox_StopPuzzleMode();
	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan = _arrayOfFans.at(i);
		SAFE_DELETE(fan);
	}
	_arrayOfFans.clear();
	SAFE_DELETE(_fanAreaPolygon);
	SAFE_DELETE(_jsonAnimInsertKey);
	SAFE_DELETE(_jsonAnimRotateKey);
	SAFE_DELETE(_jsonAnimOpenDoor);
	SAFE_DELETE(_jsonAnimStuckKey);
	SAFE_DELETE(_keyDropArea);
	SAFE_DELETE(ResetDia);

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL4s2FanPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL4s2FanPuzzleAmb);
	//MUSIC_SFX
}

void L4_S2_FanPuzzle::InitializeWithProfileData()
{

	//LoadHud();

	LoadHud(INVENTORY_V1);
	Hud->InvBox_StopPuzzleMode();
	//Hud->InvBox_StartPuzzleMode();
	LoadScene("LV4\\S2\\FANPUZZLE\\L4S2FANPUZZLE.SCN");

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

	int ArrayPts[] = {583,576,718,579,740,490,664,386,563,460};
	_keyDropArea =  new CPolygon(5);
	_keyDropArea->Init(ArrayPts);


	_jsonAnimOpenDoor = new JSONAnimator(1, CC_CALLBACK_1(L4_S2_FanPuzzle::OnOpenDoorAnimComplete, this));
	_jsonAnimOpenDoor->parseAndLoadJSON("ANIM\\L4\\FANPUZZLE_DOOR_OPEN.JSON", HOScene);
	_jsonAnimOpenDoor->StopAnim();
	//_jsonAnimOpenDoor->SetEnableObjects(false);

	_jsonAnimInsertKey = new JSONAnimator(1, CC_CALLBACK_1(L4_S2_FanPuzzle::OnKeyInsertAnimComplete, this));
	_jsonAnimInsertKey->parseAndLoadJSON("ANIM\\L4\\FANPUZZLE_KEY.JSON", HOScene);
	_jsonAnimInsertKey->StopAnim();
	_jsonAnimInsertKey->SetEnableObjects(false);

	_jsonAnimRotateKey = new JSONAnimator(1, CC_CALLBACK_1(L4_S2_FanPuzzle::OnKeyRotateAnimComplete, this));
	_jsonAnimRotateKey->parseAndLoadJSON("ANIM\\L4\\FANPUZZLE_KEY_ROTAION.JSON", HOScene);
	_jsonAnimRotateKey->PlayAnim();
	_jsonAnimRotateKey->StopAnim();
	_jsonAnimRotateKey->SetAnimTime(0);
	_jsonAnimRotateKey->SetEnableObjects(false);

	_jsonAnimStuckKey = new JSONAnimator(1, CC_CALLBACK_1(L4_S2_FanPuzzle::OnKeyStuckAnimComplete, this));
	_jsonAnimStuckKey->parseAndLoadJSON("ANIM\\L4\\FANPUZZLE_KEY_STUCK.JSON", HOScene);
	_jsonAnimStuckKey->StopAnim();
	_jsonAnimStuckKey->SetEnableObjects(false);



	HOScene->GetObjectByID("FanFull")->Show(false);

	_doorLock = HOScene->GetObjectByID("DoorLock");
	_doorLock->SetMouseClickCallback(CC_CALLBACK_1(L4_S2_FanPuzzle::OnKeyClick, this));
	//_doorLock->SetIsInteractionEnabled(false);


	int fanClickAreaPts[] = {369,232,447,156,544,110,648,94,780,114,875,164,936,220,984,278,1019,338,936,456,937,480,903,489,811,577,777,587,778,568,723,540,653,486};
	_fanAreaPolygon=  new ClickablePolygon(0, 17, fanClickAreaPts, CC_CALLBACK_1(L4_S2_FanPuzzle::OnMouseClickOnFan, this));	

	for (int i = 0; i < 12; i++)
	{
		Fan * fan = new Fan(i);
		std::string fanId = std::string("Fan_") + MKSTR(i);
		fan->normal = HOScene->GetObjectByID(fanId);
		fan->normal->SetPivot(-fan->normal->GetWorldRect().w/2, fan->normal->GetWorldRect().h/2);


		fan->normal->SetRotate(angles[i]);
		fan->solvedRotation = angles[i];
		if (i == 0)
		{
			_firstFanZOrder = fan->normal->ZOrder;
		}

		fan->normal->SetXY(_doorLock->Pos.x + fan->normal->GetWorldRect().w/2, _doorLock->Pos.y - fan->normal->GetWorldRect().h/2);

		std::string greyImageId = std::string("Grey_") + MKSTR(i);
		fan->greyImage = HOScene->GetObjectByID(greyImageId);
		fan->greyImage->SetPivot(-fan->greyImage->GetWorldRect().w/2, fan->greyImage->GetWorldRect().h/2);


		fan->greyImage->SetRotate(angles[i]);
		fan->greyImage->SetXY(_doorLock->Pos.x + fan->greyImage->GetWorldRect().w/2, _doorLock->Pos.y - fan->greyImage->GetWorldRect().h/2);



		fan->selectedGlow = new CObject();
		fan->selectedGlow->LoadInit("LV4\\S2\\FANPUZZLE\\SelectedGlow.SPR", fan->normal->Pos.x, fan->normal->Pos.y, 6);
		fan->selectedGlow->SetXY(_doorLock->Pos.x + fan->selectedGlow->GetWorldRect().w/2, _doorLock->Pos.y - fan->selectedGlow->GetWorldRect().h/2 - 5);
		fan->selectedGlow->SetUID(std::string("selectedGlow") + MKSTR(i));
		fan->selectedGlow->SetPivot(-fan->selectedGlow->GetWorldRect().w/2, fan->selectedGlow->GetWorldRect().h/2);
		if (ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState == eFanPuzzleState_KeyInserted)
		{
			fan->selectedGlow->SetRotate(angles[0]);
			_doorLock->SetIsInteractionEnabled(true);
			_jsonAnimRotateKey->SetEnableObjects(true);
			_jsonAnimStuckKey->SetEnableObjects(true);
		}
		else
		{
			fan->selectedGlow->SetRotate(angles[i]);
		}

		_arrayOfFans.push_back(fan);
	}
	SortZOrders();
	UpdateFanStates();


	if(ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState == eFanPuzzleState_KeyInserted)
	{
		Shuffle(NULL);

		Hud->InvBox_StartPuzzleMode();
	}
	else if(ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState == eFanPuzzleState_Solved)
	{
		_doorLock->SetIsInteractionEnabled(true);
		_jsonAnimRotateKey->SetEnableObjects(true);
		//Hud->InvBox_StartPuzzleMode();
	}

}

void L4_S2_FanPuzzle::Update()
{
		
	NavigateToScene();
	if(IsResetPop)
	{
		ResetPuzzle();		
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif	
		CPoint mousePos = Control::Input->Mpos();
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
		//	else if( IsMouseOverGoBackArea(&Control::Input->Mpos()))
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
	else if( IsMouseOverGoBackArea(&mousePos))
	{
		
		#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown, eArea_L4_S2_RiverSide);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L4_S2_RiverSide);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S2_RiverSide, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
			return;
		}	
		return;
	}	

	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan = _arrayOfFans.at(i);
		if (fan->GetState() == eFanState_MouseOver)
		{
			fan->SetState(eFanState_Default);
		}
	}

	Cursor::SetMode(CA_Normal);
	Fan * fan = NULL;
	if (_isMouseInteractionOnFanEnabled) 
	{
		fan = GetFan(Control::Input->Mpos()); 
		if (fan != NULL && fan->GetState() == eFanState_Default)
		{
			fan->SetState(eFanState_MouseOver);
		}
		if (_fanAreaPolygon->GetCurrentMouseState() == eMouseState_Over)
		{
			if (fan != NULL)
			{
				if(fan->GetState() == eFanState_MouseOver)
				{
					Cursor::SetMode(CA_HandPointy);
				}
			}
		}
	}

	if (_doorLock->GetCurrentMouseState() == eMouseState_Over)
	{
		if (ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState == eFanPuzzleState_started)
		{
			Cursor::SetMode(CA_Gear);
		}
		else
		{
			Cursor::SetMode(CA_HandPointy);
		}
	}

	if (_fanAreaPolygon->GetCurrentMouseState() == eMouseState_Over)
	{
		if (ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState == eFanPuzzleState_started)
		{
			Cursor::SetMode(CA_Help);
		}

	}
	ResetPuzzle();
}

void L4_S2_FanPuzzle::Required()
{
	HiddenObjectSystem::Required();

	if( Control::Audio->IsPlaying(aTrackL4theme) )
	{
		Control::Audio->Samples[aTrackL4theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL4theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL4theme);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL4puzzle) )
				Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL4puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
	}
}

void L4_S2_FanPuzzle::OnPuzzleSolved(CObject *object)
{
	ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState = eFanPuzzleState_Solved;
	_fanAreaPolygon->SetIsInteractionEnabled(false);
	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan  = _arrayOfFans.at(i);
		fan->selectedGlow->SetAlpha(0);
		fan->selectedGlow->Show(true);
		fan->selectedGlow->FadeIn(2);
	}
	Control::Audio->QuickLoadAndPlaySFX("s2pz2_puzzleComplete");

	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	CallFunction * callFn = new CallFunction(0.65f, CC_CALLBACK_1(L4_S2_FanPuzzle::FadeOutFanGlow, this), (UpdateListEntity*)this);
	callFn->Run();

	Hud->InvBox_StopPuzzleMode();

}

void L4_S2_FanPuzzle::OnMouseClickOnFan(int polyId)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (Hud->IsDraggingItem())
	{
		return;
	}
	if(IsResetPop)
		return;

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState == eFanPuzzleState_started)
	{
		Hud->ShowBannerText("L4key");
	}
	if (!_isMouseInteractionOnFanEnabled)
	{
		return;
	}
	Fan * fan = GetFan(Control::Input->Mpos()); 
	if (fan == NULL)
	{
		return;
	}

	if (fan->GetState() == eFanState_Default || fan->GetState() == eFanState_MouseOver)
	{

		_currentSelectedFan = fan;
		_currentSelectedFan->SetState(eFanState_Selected);

		if (_prevSelectedFan == NULL)
		{
			_prevSelectedFan = fan;
		}
		else
		{
			float tweenVals1[5] = {-1, -1, -1, 1, _prevSelectedFan->normal->GetRotate()};


			float tweenVals2[5] = {-1, -1, -1, 1, _currentSelectedFan->normal->GetRotate()};
			_isMouseInteractionOnFanEnabled = false;

			float tweenVals3[5] = {0, -1, -1, -1, -1};

			_currentSelectedFan->normal->TweenTo(tweenVals1, 1, eEase_BACKOUT, true, CC_CALLBACK_1(L4_S2_FanPuzzle::OnTweenComplete, this));
			_prevSelectedFan->normal->TweenTo(tweenVals2, 1, eEase_BACKOUT, true);
			_currentSelectedFan->selectedGlow->TweenTo(tweenVals3, 1, 0, true);
			_prevSelectedFan->selectedGlow->TweenTo(tweenVals3, 1, 0, true);

			_currentFanIterator = std::find(_arrayOfFans.begin(), _arrayOfFans.end(), _currentSelectedFan);
			_prevFanIterator = std::find(_arrayOfFans.begin(), _arrayOfFans.end(), _prevSelectedFan);
			std::iter_swap(_currentFanIterator, _prevFanIterator);
			Control::Audio->QuickLoadAndPlaySFX("s2pz2_fansSliding");
			SortZOrders();
		}


	}
	else if (fan->GetState() == eFanState_Selected)
	{
		fan->SetState(eFanState_Default);
		_prevSelectedFan = NULL;
		_currentSelectedFan = NULL;
	}




}

void L4_S2_FanPuzzle::OnTweenComplete(CObject* object)
{
	UpdateFanStates();	
	_currentSelectedFan = NULL;
	_prevSelectedFan = NULL;
	if (IsPuzzleSolved())
	{
		OnPuzzleSolved(NULL);
	}
	else
	{
		_isMouseInteractionOnFanEnabled = true;
	}
}


Fan * L4_S2_FanPuzzle::GetFan(CPoint mousePos)
{
	float angle = Elixir::Angle(&_doorLock->Pos, &mousePos);
	int clickedFanIndex = -1;
	for (int i = 0; i < 12; i++)
	{
		if (clickAreaAngles[i] < angle && clickAreaAngles[i+1] > angle)
		{
			if (i != 0 && i != 11)
			{
				clickedFanIndex = i;
			}
		}
	}
	if (clickedFanIndex == -1)
	{
		return NULL;
	}

	return _arrayOfFans.at(clickedFanIndex);
}

void L4_S2_FanPuzzle::Shuffle(CObject * object)
{

	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan = NULL;
		int j = 0;
		for (;j < (int)(_arrayOfFans.size()); j++)
		{
			if (_arrayOfFans.at(j)->GetId() == shuffledOrder[i])
			{
				fan = _arrayOfFans.at(j);
				break;
			}
		}
		if (fan)
		{
			_arrayOfFans.erase(_arrayOfFans.begin() + j);
			_arrayOfFans.insert(_arrayOfFans.begin() + i, fan);
		}

	}

	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan* fan = _arrayOfFans.at(i);
		float tweenVals1[5] = {-1, -1, -1, 1, angles[i]};
		if (i == (int)_arrayOfFans.size()-1)
		{
			fan->normal->TweenTo(tweenVals1, 1, eEase_BACKOUT, true, CC_CALLBACK_1(L4_S2_FanPuzzle::OnShuffleCompleted, this));
		}
		else
		{
			fan->normal->TweenTo(tweenVals1, 1, eEase_BACKOUT, true);
		}
	}

	SortZOrders();
	UpdateFanStates();
}

void L4_S2_FanPuzzle::OnShuffleCompleted(CObject * obj)
{
	Hud->ShowBannerText("L4oh");
	_isMouseInteractionOnFanEnabled = true;
}

void L4_S2_FanPuzzle::SkipPuzzle()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);

	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan  = _arrayOfFans.at(i);
		float tweenVals1[5] = {-1, -1, -1, 1, angles[0]};
		if (i == (int) (_arrayOfFans.size() - 1))
		{
			fan->normal->TweenTo(tweenVals1, 0.5, 0, true, CC_CALLBACK_1(L4_S2_FanPuzzle::OpenFan, this));
		}
		else
		{
			fan->normal->TweenTo(tweenVals1, 0.5, 0, true);
		}
	}


}

void L4_S2_FanPuzzle::ProcessSkip()
{
	SkipPuzzle();
}

void L4_S2_FanPuzzle::SortZOrders()
{
	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan = _arrayOfFans.at(i);
		fan->normal->SetZ(_firstFanZOrder + (i * 2));
		fan->greyImage->SetZ(_firstFanZOrder + (i * 2));
		fan->selectedGlow->SetZ((_firstFanZOrder + 1) + (i * 2));
	}
}

void L4_S2_FanPuzzle::UpdateFanStates()
{
	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan = _arrayOfFans.at(i);
		if (fan->GetId() == i)
		{
			fan->SetState(eFanState_Locked);
		}
		else
		{
			fan->SetState(eFanState_Default);
		}
	}
}

void L4_S2_FanPuzzle::OpenFan(CObject * obj)
{
	_isMouseInteractionOnFanEnabled = false;

	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan = NULL;
		int j = 0;
		for (;j < (int)(_arrayOfFans.size()); j++)
		{
			if (_arrayOfFans.at(j)->GetId() == i)
			{
				fan = _arrayOfFans.at(j);
				break;
			}
		}
		if (fan)
		{
			_arrayOfFans.erase(_arrayOfFans.begin() + j);
			_arrayOfFans.insert(_arrayOfFans.begin() + i, fan);
		}

	}

	SortZOrders();
	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan  = _arrayOfFans.at(i);
		fan->SetState(eFanState_Locked);

		float tweenVals1[5] = {-1, -1, -1, -1, fan->solvedRotation};
		if (i == (int)(_arrayOfFans.size() - 1))
		{
			fan->normal->TweenTo(tweenVals1, 1, 0, true, CC_CALLBACK_1(L4_S2_FanPuzzle::OnPuzzleSolved, this));
		}
		else
		{
			fan->normal->TweenTo(tweenVals1, 1, 0, true);
		}
	}
	Control::Audio->QuickLoadAndPlaySFX("s2pz2_fansShuffle");
}

void L4_S2_FanPuzzle::CloseFan()
{
	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan  = _arrayOfFans.at(i);
		float tweenVals1[5] = {-1, -1, -1, 1, angles[0]};
		if (i == (int)(_arrayOfFans.size() - 1))
		{
			fan->normal->TweenTo(tweenVals1, 1, 0, true, CC_CALLBACK_1(L4_S2_FanPuzzle::Shuffle, this));
		}
		else
		{
			fan->normal->TweenTo(tweenVals1, 1, 0, true);
		}
	}
}

void L4_S2_FanPuzzle::FadeOutFanGlow(UpdateListEntity * obj)
{
	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan  = _arrayOfFans.at(i);
		fan->selectedGlow->FadeOut(0.5);
	}
	if (_handleExtrasGameComplete())
	{
		return;
	}
	Hud->ShowBannerText("L4bingo");
}

void L4_S2_FanPuzzle::OnFadeOutFanComplete(CObject * object)
{

}



void L4_S2_FanPuzzle::OnKeyClick(CObject * object)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive || Hud->IsDraggingItem())
	{
		return;
	}
	if(IsResetPop)
		return;
	if (ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState == eFanPuzzleState_started)
	{
		Hud->ShowBannerText("L4key");
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState == eFanPuzzleState_KeyInserted)
	{
		_doorLock->SetIsInteractionEnabled(false);
		_jsonAnimStuckKey->PlayAnim(false);
		Hud->ShowBannerText("L4work");
		Control::Audio->QuickLoadAndPlaySFX("s2pz2_keyStuck");
	}
	else if(ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState == eFanPuzzleState_Solved)
	{
		_doorLock->SetIsInteractionEnabled(false);
		_jsonAnimRotateKey->PlayAnim(false);
		Control::Audio->QuickLoadAndPlaySFX("s2pz2_keyTurn");
	}


}

void L4_S2_FanPuzzle::OnKeyInsertAnimComplete(int id)
{
	Hud->InvBox_StartPuzzleMode();
	CloseFan();

	_jsonAnimInsertKey->SetEnableObjects(false);
	_jsonAnimRotateKey->SetEnableObjects(true);
	_jsonAnimStuckKey->SetEnableObjects(true);
	_doorLock->SetIsInteractionEnabled(true);
}

void L4_S2_FanPuzzle::OnKeyRotateAnimComplete(int id)
{
	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan  = _arrayOfFans.at(i);
		fan->normal->Show(false);
		fan->greyImage->Show(false);
		fan->selectedGlow->Show(false);

	}
	HOScene->GetObjectByID("FanFull")->Show(true);
	_doorLock->Show(false);
	_jsonAnimRotateKey->SetEnableObjects(false);
	_jsonAnimOpenDoor->PlayAnim(false);
	Control::Audio->QuickLoadAndPlaySFX("s2pz2_doorOpen");

}

void L4_S2_FanPuzzle::OnOpenDoorAnimComplete(int id)
{

	//Objective Add
	if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O3_FindWayToMakeFire] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O3_FindWayToMakeFire] = true;
		ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
		= eObj_L4O3_FindWayToMakeFire;
		//Increment Objectives
		ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
		Hud->ShowObjectiveFX(eObj_L4O3_FindWayToMakeFire);

		Control::Audio->PlaySample(aSFXMagic1);
	}

	

	ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened = true;
	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_Cave;
	//ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_3;
	SendMessage(ControlExit, ID, HOSM_NewArea);
}

void L4_S2_FanPuzzle::OnKeyStuckAnimComplete(int id)
{
	_doorLock->SetIsInteractionEnabled(true);
}

void L4_S2_FanPuzzle::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
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

		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{

			if (itemVal == eInv_L4_S5_CaveDoorKey)
			{



				if (_keyDropArea->Intersection(&dropPos))
				{
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					//Clear Inventory. One Time Use Items.
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_CaveDoorKey] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S5_CaveDoorKey);
					ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState = eFanPuzzleState_KeyInserted;
					_jsonAnimInsertKey->SetEnableObjects(true);
					_jsonAnimInsertKey->PlayAnim(false);
					Control::Audio->QuickLoadAndPlaySFX("s2pz2_keyInsert");

				}
				else
				{
					GFInstance->WrongInvDropMsg(Hud);
				}
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}

		}
	}
}

void L4_S2_FanPuzzle::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 24;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState <= eFanPuzzleState_KeyInserted && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_CaveDoorKey])
			{
				Hud->ShowHintForInvItem( eInv_L4_S5_CaveDoorKey, PointSystem::CreateCPoint(_keyDropArea->center.x,_keyDropArea->center.y,0));
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if(IsPuzzleSolved()  && !ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened)
			{
				//InitHintTrailEmit(&HOScene->GetObjectByID("Key_4")->GetWorldRect());
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle hoSceneKey4 =HOScene->GetObjectByID("Key_4")->GetWorldRect();
				InitHintTrailEmit(&hoSceneKey4);
				return;
			}
			break;	
		case 2: ++iNonHOHint;
			if(  ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S1_SwampEntrance] )
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;		
		case 3: ++iNonHOHint;
			if(Hud->ShowHintForComboInvItems())
				return;
		case 4: ++iNonHOHint;
				{
					Hud->teleportSystem->ShowTeleport();
					return;
				}
				break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}


void L4_S2_FanPuzzle::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_CaveDoorKey] &&
		ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState <= eFanPuzzleState_KeyInserted )
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState >= eFanPuzzleState_KeyInserted && 
		!ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened ) 
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_PZDoor] = isAnyTaskLeft;

}

bool L4_S2_FanPuzzle::IsPuzzleSolved()
{
	bool isSolved = true;
	for (int i = 0; i < (int)(_arrayOfFans.size()); i++)
	{
		Fan * fan = _arrayOfFans.at(i);
		if (fan->GetId() != i)
		{
			isSolved = false;
			break;
		}
	}

	return isSolved;


}


Fan::Fan(int fanId)
{
	normal = NULL;
	selectedGlow = NULL;
	greyImage = NULL;
	_id = fanId;
	_state = eFanState_Default;

}

Fan::~Fan()
{
	SAFE_DELETE(selectedGlow);
}

FanState Fan::GetState()
{
	return _state;
}

void Fan::SetState(FanState fanState)
{
	normal->SetAlpha(0);
	greyImage->SetAlpha(0);
	selectedGlow->Show(false);

	selectedGlow->IsTweening = false;

	_state = fanState;
	switch (fanState)
	{
	case eFanState_Default:
		{
			normal->SetScale(1);
			greyImage->SetScale(1);
			selectedGlow->SetScale(1);
			selectedGlow->SetAlpha(1);
			greyImage->SetAlpha(1);

		}
		break;
	case eFanState_Selected:
		{
			normal->SetScale(1.02f);
			greyImage->SetScale(1.02f);
			greyImage->SetAlpha(1);
			selectedGlow->SetScale(1.02f);
			selectedGlow->Show(true);
			selectedGlow->SetAlpha(1);
		}
		break;
	case eFanState_MouseOver:
		{
			normal->SetScale(1.01f);
			greyImage->SetScale(1.01f);
			selectedGlow->SetScale(1.01f);
			selectedGlow->Show(true);
			selectedGlow->SetAlpha(0.5f);
			greyImage->SetAlpha(1);
		}
		break;
	case eFanState_Locked:
		{
			normal->SetAlpha(1);
			normal->SetScale(1.01f);
		}
		break;

	default:
		break;
	}
}

int Fan::GetId()
{
	return _id;
}

void Fan::Update(float ds)
{
	selectedGlow->SetScale(normal->GetScale());
	selectedGlow->SetRotate(normal->GetRotate());
	greyImage->SetRotate(normal->GetRotate());
}

void L4_S2_FanPuzzle::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState != eFanPuzzleState_KeyInserted)
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
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_PZDoor;
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
void L4_S2_FanPuzzle::NavigateToScene()
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