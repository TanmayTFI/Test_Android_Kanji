//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S3_PZDoorPuzzle.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

const CRectangle  LeverRect(447.0f,305.0f,186.0f,270.0f);
const CRectangle  PZAreaRect(146.0f,7.0f,1053.0f,543.0f);
const CRectangle  NextAreaRect(577.0f,1.0f,213.0f,593.0f);
const CRectangle  LetterOnDoor(556.0f,30.0f,335.0f,274.0f);
const CRectangle  LetterBig(315.0f,98.0f,730.0f,502.0f);



const int ColorMatchDelay = 3*60;
const int UnlockDelay = 7*60;
// const int SymbolClickDelay = 3*60;  // Sac: Commenting Unused variable

namespace Level7_Scene3
{
	const int Min_No_Of_SymbolsToAdd = 3;

	Hand::Hand(CObject* obj)
	{
		LoadSprite(obj->Filename);
		SetPos(&obj->Pos);
		SetZ(obj->ZOrder);
		obj->SetEnable(false);
		
		_isShowing = false;
	}

	Hand::~Hand()
	{
		
	}

	void Hand::Update(float ds)
	{
		CObject::Update(ds);

		if (_isShowing && (_handWithPower->Status != SPR_FADEIN && _handWithPower->Status != SPR_ANIMATING))
		{
			_handWithPower->Animate(0.4f, 1.0f, 3);
		}
	}

	void Hand::ShowHand()
	{
		CObject::FadeIn(2);
	}

	void Hand::ShowPower()
	{
		_isShowing = true;
		FadeIn(2);
		_handWithPower->FadeIn(2);
		Control::Audio->QuickLoadAndPlaySFX("s3pz1_handposechange");
	}

	void Hand::FadeOut(float fadeOutSpeed)
	{
		CObject::FadeOut(fadeOutSpeed);
	
		if (_isShowing)
		{
			_handWithPower->FadeOut(fadeOutSpeed);
		}

		_isShowing = false;
	}





	HandSymbol::HandSymbol(CObject* obj):UIButton(obj)
	{	
		_disableDelay = 0;
		_selected = false;
		_enabled = true;
		_sfx = NULL;
	}

	HandSymbol::~HandSymbol()
	{
		_sfx = NULL;
	}

	void HandSymbol::Update(float ds)
	{
		UIButton::Update(ds);
		
		if (_disableDelay)
		{
			_disableDelay--;
			if (!_disableDelay && _enabled) 
			{
				Enable();
			}
		}
	}

	void HandSymbol::OnMouseClick()
	{
		UIButton::OnMouseClick();
		Clicked();
	}

	void HandSymbol::OnMouseUp()
	{
		UIButton::OnMouseUp();
		Clicked();
	}

	void HandSymbol::Clicked()
	{
		if (_selected || _disableDelay)
			return;

		_delegate->onSymbolClicked(_type);
		_hand->ShowPower();
		_sfx->ZOrder = ZOrder - 1;
		_sfx->Start();
		_selected = true;
		Enable();
		_isInteractionEnabled = false;
	}

	void HandSymbol::HideHand()
	{
		if (_selected)
		{
			_selected = false;
			_hand->FadeOut(2);
		}
	}

	void HandSymbol::DisableSymbol()
	{
		_enabled = false;
		Disable();
	}

	void HandSymbol::EnableAfterDelay(float delay)
	{
		if (!_selected)
		{
			_disableDelay = (int)delay;
		}
	}

	void HandSymbol::Enable()
	{
		_enabled = true;

		if (!_disableDelay)
		{
			UIButton::Enable();
		}
	}



	Lock::Lock(CObject* obj1, CObject* obj2)
	{
		_lock1 = obj1;
		_lock2 = obj2;
		_isUnlocked = false;
	}

	Lock::~Lock()
	{

	}

	bool Lock::Unlock(ColorValue color)
	{
		if (_isUnlocked)
			return true;

		if (_color == color)
		{
			//_lock1->PlayAnimation(1);
			//_lock2->PlayAnimation(1);

			float val[5] = {-1, -1, -1, -1, -180};
			_lock1->TweenTo(val, 1, TW_EASEBACKIN, true, CC_CALLBACK_0(Lock::openCompleted, this));
			_lock2->TweenTo(val, 1, TW_EASEBACKIN, true);
			return true;
		}

		return false;
	}

	void Lock::openCompleted()
	{
		_isUnlocked = true;
		_delegate->onSymbolLockComplete();
	}

	void Lock::setUnlocked(bool isUnlock)
	{
		_isUnlocked = isUnlock;

		if (isUnlock)
		{
			/*_lock1->PlayAnimation(1);
			_lock2->PlayAnimation(1);*/
			_lock1->SetRotate(-180);
			_lock2->SetRotate(-180);
		}
	}




	PZController::PZController()
	{
		_havePower = false;
		_izPzCompleted = false;
		_isReset = true;
		_symbolClickDelay = 0;
		_unLockDelay = 0;
	}

	PZController::~PZController()
	{
		for (int i = 0; i < (int)_locks.size(); i++)
		{
			SAFE_DELETE(_locks[i]);
		}
		_locks.clear();
		//_doorCenterParticle.clear();

		//SAFE_DELETE(_addBtn);
		//SAFE_DELETE(_resetBtn);

		SAFE_DELETE(s3PurpleEmitFX);
		SAFE_DELETE(s3OrangeEmitFX);
		SAFE_DELETE(s3GreenEmitFX);

		SAFE_DELETE(s3RedHandEmitFX); 
		SAFE_DELETE(s3BlueHandEmitFX); 
		SAFE_DELETE(s3YellowHandEmitFX);
	}

	void PZController::InitPuzzle(CHOScene* scene)
	{
		s3RedHandEmitFX = NULL;
		s3BlueHandEmitFX = NULL;
		s3YellowHandEmitFX = NULL;

		s3RedHandEmitFX = new SuperFX("PRT\\L7\\s3RedHandEmit.INI", PointSystem::CreateCPoint(399,197,0), 21); 
		s3BlueHandEmitFX = new SuperFX("PRT\\L7\\s3BlueHandEmit.INI", PointSystem::CreateCPoint(683,60,0), 24); 
		s3YellowHandEmitFX = new SuperFX("PRT\\L7\\s3YellowHandEmit.INI", PointSystem::CreateCPoint(943,201,0), 27); 
		
		SuperFX* sfxs[] = {s3RedHandEmitFX, s3BlueHandEmitFX, s3YellowHandEmitFX};

		for (int i = 0; i < 3; i++)
		{
			CObject* symbolObj = scene->GetObjectByID("Symbol_"+MKSTR(i));
			CObject* handObj = scene->GetObjectByID("Hand_"+MKSTR(i)+"_0");
			CObject* handPower = scene->GetObjectByID("Hand_"+MKSTR(i)+"_1");
			handPower->SetAlpha(0);

			Hand* hand = new Hand(handObj);
			hand->setHandWithPower(handPower);
			scene->PushObject(hand);
			hand->SetAlpha(0);

			HandSymbol* handSym = new HandSymbol(symbolObj);
			handSym->setHandSymbolType(SymbolType(i));
			handSym->setDelegate(this);
			handSym->setHand(hand);
			handSym->setSfx(sfxs[i]);
			handSym->SetIsInteractionEnabled(false);
			scene->PushObject(handSym);
			_symbols.push_back(handSym);
			handSym->SetScale(0);
			handSym->SetAlpha(0);
			_buttonsList.push_back(handSym);
		}

		_addBtn = NULL;
		_addBtn = new UIButton(scene->GetObjectByID("Symbol_Add"), CC_CALLBACK_1(PZController::onAddButtonClick, this));
		scene->PushObject(_addBtn);
		_addBtn->SetScale(0);
		_addBtn->Disable();
		_buttonsList.push_back(_addBtn);

		_resetBtn = NULL;
		_resetBtn = new UIButton(scene->GetObjectByID("Symbol_Reset"), CC_CALLBACK_1(PZController::onResetButtonClick, this));
		scene->PushObject(_resetBtn);
		_resetBtn->SetScale(0);
		_buttonsList.push_back(_resetBtn);

		float dpvitx[3] = {15, 15, 38};
		float dpvity[3] = {17, -17, 0};

		for (int i = 0; i < 3; i++)
		{
			CObject* lockObj1 = scene->GetObjectByID("Lock_L_"+MKSTR(i+1));
			lockObj1->SetPivot(-dpvitx[i], -dpvity[i]);
			CObject* lockObj2 = scene->GetObjectByID("Lock_R_"+MKSTR(i+1));
			lockObj2->SetPivot(dpvitx[i], dpvity[i]);

			ColorValue color = ColorValue((int)Color_Purple + i);

			Lock* lock = new Lock(lockObj1, lockObj2);
			lock->setColor(color);
			lock->setDelegate(this);
			lock->setUnlocked(ProfileSystem::Instance->ActiveProfile.L7_S3_LocksState[i]);
			_locks.push_back(lock);
		}
		
		s3PurpleEmitFX = NULL;
		s3OrangeEmitFX = NULL;
		s3GreenEmitFX = NULL;

		s3PurpleEmitFX = new SuperFX("PRT\\L7\\s3DoorLockColorPurple.INI", PointSystem::CreateCPoint(681,361,0), 2); 
		s3OrangeEmitFX = new SuperFX("PRT\\L7\\s3DoorLockColorOrange.INI", PointSystem::CreateCPoint(680,364,0), 2); 
		s3GreenEmitFX = new SuperFX("PRT\\L7\\s3DoorLockColorGreen.INI", PointSystem::CreateCPoint(680,364,0), 2); 

		SuperFX* sfxs1[] = {s3PurpleEmitFX, s3OrangeEmitFX, s3GreenEmitFX};

		for (int i = 0; i < 3; i++)
		{
			_doorCenterParticle[ColorValue(i+1)] = sfxs1[i];
			_doorCenterEffectObj[ColorValue(i+1)] = scene->GetObjectByID("DoorLockColorEffect_"+MKSTR(i+1));
			_doorCenterEffectObj[ColorValue(i+1)]->SetEnable(false);
		}

		_resetHand = scene->GetObjectByID("Reset_1");
	}

	void PZController::ActivatePuzzle(bool activate)
	{
		_havePower = activate;
		float valArr[5] = {1, -1, -1, 1.0f, -1};

		for (int i = 0; i < (int)_symbols.size(); i++)
		{
			_symbols[i]->TweenTo(valArr, i+0.5f, TW_EASEBACKOUT, true, CC_CALLBACK_1(PZController::AnimateSymbol, this));
			_symbols[i]->SetIsInteractionEnabled(activate);
		}

		_addBtn->TweenTo(valArr, 1, TW_EASEBACKOUT, true, CC_CALLBACK_1(PZController::AnimateSymbol, this));
		_resetBtn->TweenTo(valArr, 1, TW_EASEBACKOUT, true, CC_CALLBACK_1(PZController::AnimateSymbol, this));
	}

	void PZController::DisableAllControlls()
	{
		for (int i = 0; i < (int)_symbols.size(); i++)
		{
			_symbols[i]->FadeOut(2);
			_symbols[i]->SetIsInteractionEnabled(false);
		}

		_addBtn->FadeOut(2);
		_resetBtn->FadeOut(2);
	}

	void PZController::AnimateSymbol(CObject* obj)
	{
		obj->Animate(3, 4, 0, 0.03f, 4);
	}

	void PZController::Update()
	{
		if(_colorMatchDelay)
		{
			_colorMatchDelay--;

			if (!_colorMatchDelay)
				ShowDoorUnlockEffect();
		}

		if(_unLockDelay)
		{
			_unLockDelay--;

			if (!_unLockDelay)
				CheckCombination();
		}

		CPoint mPos = Control::Input->Mpos();
		for (int i = 0; i < (int)(_buttonsList.size()); i++)
		{
			if(_buttonsList[i]->GetIsInteractionEnabled() && _buttonsList[i]->GetWorldRect().Intersection(&mPos))
				Cursor::SetMode(CA_HandPointy);
		}
	}

	void PZController::onAddButtonClick(CObject *btn)
	{
		onSymbolClicked(Symbol_Add);
	}

	void PZController::onResetButtonClick(CObject *btn)
	{
		if (!_havePower)
			return;

		_selectedSymbols.clear();
		_resetHand->FadeIn(2);
		_isReset = true;
		
		for (int i = 0; i < (int)_symbols.size(); i++)
		{
			_symbols[i]->HideHand();
			_symbols[i]->Enable();
			_symbols[i]->getSfx()->StopEmit();
		}

		_addBtn->Disable();
		_resetBtn->Enable();
	}

	void PZController::onSymbolClicked(SymbolType type)
	{
		Control::Audio->QuickLoadAndPlaySFX("s3pz1_ringclicked");
		if (type != Symbol_Add)
		{
			for (int i = 0; i < (int)_symbols.size(); i++)
			{
				_symbols[i]->HideHand();
				_symbols[i]->DisableSymbol();
			}
		}
		else
		{
			for (int i = 0; i < (int)_symbols.size(); i++)
			{
				_symbols[i]->Enable();
			}
		}


		if (_selectedSymbols.size() == 0)
		{
			_addBtn->Enable();
		}
		else
		{
			_addBtn->Disable();
		}

		if (_isReset)
		{
			_resetHand->FadeOut(2);
			_isReset = false;
		}
		
		
		_selectedSymbols.push_back(type);

		if ((int)_selectedSymbols.size() >= Min_No_Of_SymbolsToAdd)
		{
			_colorMatchDelay = ColorMatchDelay;
			_unLockDelay = UnlockDelay;
			_resetBtn->Disable();
		}
	}

	void PZController::onSymbolLockComplete()
	{
		_izPzCompleted = true;

		for (int i = 0; i < (int)_locks.size(); i++)
		{
			if (!_locks[i]->getIsUnlocked())
			{
				_izPzCompleted = false;
				break;
			}
		}

		if (_izPzCompleted)
		{
			for (int i = 0; i < (int)_symbols.size(); i++)
			{
				_symbols[i]->HideHand();
			}

			if (_isReset)
				_resetHand->FadeOut(2);
		}
	}

	void PZController::ShowDoorUnlockEffect()
	{
		int sum = 0;
		list<SymbolType>::iterator it;
		for (it = _selectedSymbols.begin(); it != _selectedSymbols.end(); advance(it, 1))
		{
			if ((*it) != Symbol_Add)
				sum = sum + (int)(*it);
		}
        // Sac: the below line is trying to avoid the crash because if sum ends up with 0 as value, this could crash the game as _doorCenterEffectObj gets created with 4 items in it in which 0th element as NULL. and when sum is 0, it trys to call _doorCenterEffectObject[0] which is NULL and hence crash. However this happens only rarely, so to have a smooth flow, I'm adding this below simple hack,
        if(sum==0) sum = 1;
		ColorValue color = ColorValue(sum);
		_doorCenterEffectObj[color]->SetEnable(true);
		_doorCenterEffectObj[color]->Animate(0.65f, 1.0f, 1);
		_doorCenterParticle[color]->Start();
	}


	void PZController::CheckCombination()
	{
		int sum = 0;
		list<SymbolType>::iterator it;
		for (it = _selectedSymbols.begin(); it != _selectedSymbols.end(); advance(it, 1))
		{
			if ((*it) != Symbol_Add)
				sum = sum + (int)(*it);
		}

		_selectedSymbols.clear();
        
        // Sac: the below line is trying to avoid the crash because if sum ends up with 0 as value, this could crash the game as _doorCenterEffectObj gets created with 4 items in it in which 0th element as NULL. and when sum is 0, it trys to call _doorCenterEffectObject[0] which is NULL and hence crash. However this happens only rarely, so to have a smooth flow, I'm adding this below simple hack,
        if(sum==0) sum = 1;
        
		ColorValue color = ColorValue(sum);

		for (int i = 0; i < (int)_locks.size(); i++)
		{
			if (_locks[i]->Unlock(color))
			{
				Control::Audio->QuickLoadAndPlaySFX("s3pz1_lockpartunlock");
				ProfileSystem::Instance->ActiveProfile.L7_S3_LocksState[i] = true;
			}
		}

		_doorCenterEffectObj[color]->SetEnable(false);
		_doorCenterParticle[color]->StopEmit();
		onResetButtonClick(NULL);

		for (int i = 0; i < (int)_symbols.size(); i++)
		{
			_symbols[i]->Enable();
		}
	}

	void PZController::SkipPuzzle()
	{
		for (int i = 0; i < (int)_locks.size(); i++)
		{
			if (_locks[i]->Unlock(ColorValue(i+1)))
				ProfileSystem::Instance->ActiveProfile.L7_S3_LocksState[i] = true;
		}

		if (_isReset)
			_resetHand->FadeOut(2);
	}
}


L7_S3_PZDoorPuzzle::L7_S3_PZDoorPuzzle()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S3\\PZ2\\PZ2_1.SCN");

	havenText = new CObject();
	havenText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L7\\HAVENTEXT.SPR", 683, 290, eZOrder_Popup + 500);
	havenText->SetUID("HavenText");
	havenText->Show(false);
	HOScene->PushObject(havenText);

	threadScene = NULL;
	doorOpenedScene = NULL;
	_cvdo_ThreadCut = NULL;
	_cvdo_nightMare = NULL;

	time = 0;
	subIndex = 0;
	subMax = 4;
	vidTimer = 0;
	vidTimer2 = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));
	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("nightmare1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("nightmare2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("nightmare3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("nightmare4"));
	

	_isAnimPlaying = false;

	_janim_TakeLetter = new JSONAnimator(2, CC_CALLBACK_1(L7_S3_PZDoorPuzzle::onAnimComplete, this));
	_janim_TakeLetter ->parseAndLoadJSON("ANIM\\L7\\S3P1TAKELETTER.JSON",HOScene);

	_janim_LetterToMap = new JSONAnimator(3, CC_CALLBACK_1(L7_S3_PZDoorPuzzle::onAnimComplete, this));
	_janim_LetterToMap ->parseAndLoadJSON("ANIM\\L7\\S3P1LETTERTOMAP.JSON",HOScene);

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	_cvdo_ThreadCut = new CVideo("OGV\\L7\\S4DOORTHREADREMOVE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//_cvdo_ThreadCut->SetScale(1.334f);
	_cvdo_ThreadCut->SetScale(GFApp->video_aspectRatio);

	_pzState = ProfileSystem::Instance->ActiveProfile.L7_S3_PZDoorState;
	_stateMacine.push(_pzState);
	doorOpenedScene = new CHOScene();
	doorOpenedScene->Load("LV7\\S3\\PZ2\\PZ2_3.SCN");	
	doorOpenedScene->Show(false);

	LoadSavedState();
	pzController = new Level7_Scene3::PZController();
	pzController->InitPuzzle(HOScene);
	if(_pzState == L7S3_PZState_PlayingPuzzle)
		pzController->ActivatePuzzle(true);

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s3HavenAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s3HavenAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s3HavenAmb,true);
	}
	Control::Audio->LoadSample(aVOL7MaryRefS3_4,	AudioVO);
	//MUSIC_SFX
	
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S3_PZDoorPuzzle]  = true;
}

void L7_S3_PZDoorPuzzle::LoadSavedState()
{
	HOScene->GetObjectByID("HandCloth")->Show(false);
	HOScene->GetObjectByID("Hand_Add_0_0")->SetEnable(false);
	HOScene->GetObjectByID("Hand_Add_0_1")->SetEnable(false);
	HOScene->GetObjectByID("Reset_0")->Show(false);
	HOScene->GetObjectByID("Reset_1")->Show(false);
	HOScene->GetObjectByID("Letter")->SetEnable(false);
	HOScene->GetObjectByID("MagicRing")->SetScale(0);
	HOScene->GetObjectByID("MagicRing")->SetAlpha(0);

	switch (_pzState)
	{
	case L7S3_PZState_WaitToCollectLever:
		{
			threadScene = new CHOScene();
			threadScene->Load("LV7\\S3\\PZ2\\PZ2_2.SCN");
			threadScene->GetObjectByID("IronLever")->SetRotate(30);
		}
		break;
	case L7S3_PZState_Initialize:
	case L7S3_PZState_WaitingForPower:
		{
			if (!threadScene)
			{
				threadScene = new CHOScene();
				threadScene->Load("LV7\\S3\\PZ2\\PZ2_2.SCN");
			}

			if( ProfileSystem::Instance->ActiveProfile.L7_S3_InvLeverTaken )
			{
				threadScene->GetObjectByID("IronLever")->SetEnable(false);
			}
		}
		break;

	case L7S3_PZState_GotPower:
	case L7S3_PZState_TreadsRemoved:
	case L7S3_PZState_PlayingPuzzle:
		{
			HOScene->GetObjectByID("MagicRing")->SetScale(2.0f);
			HOScene->GetObjectByID("MagicRing")->SetAlpha(1);
			HOScene->GetObjectByID("HandCloth")->Show(true);
			HOScene->GetObjectByID("Reset_1")->Show(true);
			Hud->InvBox_StartPuzzleMode();
		}
		break;

	case L7S3_PZState_WaitingForLetterClose:
		{
			if( _janim_LetterToMap )
				_janim_LetterToMap ->SetAnimTime(0);
		}
		break;

	case L7S3_PZState_DoorOpened:
	case L7S3_PZState_WaitToEnterNextScene:
	case L7S3_PZState_NightMareVdoPlaying:
		{
			//CRectangle RectVidFull(171, 96, 1024, 576);
			CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
			_cvdo_nightMare = new CVideo("OGV\\L7\\S3NIGHTMARE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
			//_cvdo_nightMare->SetScale(1.334f);
			_cvdo_nightMare->SetScale(GFApp->video_aspectRatio);
			_pzState = L7S3_PZState_WaitToEnterNextScene;
		}

	case L7S3_PZState_WaitingToOpenDoor:
		{
			HOScene->GetObjectByID("LetterOnScene")->SetEnable(false);
		}
		break;

	case L7S3_PZState_NightMareVdoComplete:
		{
			doorOpenedScene->SetZOff(60);
			doorOpenedScene->Show(true);
		}
		break;
	}
	if (_pzState == L7S3_PZState_WaitingForPower)
		_pzState = L7S3_PZState_CheckForPower;

	if (_pzState == L7S3_PZState_RemovingThreads)
		_pzState = L7S3_PZState_TreadsRemoved;
}

void L7_S3_PZDoorPuzzle::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(_pzState == L7S3_PZState_RemovingThreads || _pzState == L7S3_PZState_NightMareVdoPlaying)
		return;

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

void L7_S3_PZDoorPuzzle::Update()
{
	NavigateToScene();
	if (_stateMacine.top() != _pzState)
	{
		ProfileSystem::Instance->ActiveProfile.L7_S3_PZDoorState = _pzState;
		_stateMacine.push(_pzState);
	}

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
    #endif

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	switch (_pzState)
	{
	case L7S3_PZState_WaitToCollectLever:
		{
			if (LeverRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					Hud->TakeInventoryItem(eInv_L7_S3_Lever);
					if( threadScene )
						threadScene->GetObjectByID("IronLever")->FadeOut();
					ProfileSystem::Instance->ActiveProfile.L7_S3_InvLeverTaken = true;
					ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S3_Haven] = true;
					_pzState = L7S3_PZState_Initialize;
				}
			}
			else if(PZAreaRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Help);
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L7_S3_ClickingOnWiredDoorWithoutPower);
				}
			}
		}
		break;

	case L7S3_PZState_Initialize:
		{
			_pzState = L7S3_PZState_CheckForPower;
		}
		break;

	case L7S3_PZState_CheckForPower:
		{
			if (ProfileSystem::Instance->ActiveProfile.L7_S4_HaveHandPower)
				_pzState = L7S3_PZState_GotPower;
			else
				_pzState = L7S3_PZState_WaitingForPower;
		}
		break;


	case L7S3_PZState_WaitingForPower:
		{
			if(PZAreaRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L7_S3_ClickingOnWiredDoorWithoutPower);
				}
			}
		}
		break;

	case L7S3_PZState_GotPower:
		{
			_cvdo_ThreadCut->PlayFadeInVideo(2);
			Hud->EnableInput(false);
			Hud->HideHud(false);
			_pzState = L7S3_PZState_RemovingThreads;
		}
		break;
	
	case L7S3_PZState_RemovingThreads:
		{
			if (_cvdo_ThreadCut )
			{
#ifdef FORCE_CLOSE_VIDEO
				vidTimer += Control::LogicRate;
#endif
				if(
#ifndef FORCE_CLOSE_VID_END
					_cvdo_ThreadCut->isEndReached() ||
#endif
					vidTimer > 6)
				{
					threadScene->Show(false);
					//SAFE_DELETE(threadScene); 
					_cvdo_ThreadCut->StopAndFadeOutVideo(2);
					Hud->AllowInput = true;
					Hud->ShowHud(false);	
					_pzState = L7S3_PZState_TreadsRemoved;
				}
			}
		}
		break;
	
	case L7S3_PZState_TreadsRemoved:
		{
			Hud->InvBox_StartPuzzleMode();
			HOScene->GetObjectByID("Reset_1")->FadeIn();
			HOScene->GetObjectByID("HandCloth")->FadeIn();
			float valArr[5] = {1, -1, -1, 2.0f, -1};
			HOScene->GetObjectByID("MagicRing")->TweenTo(valArr, 1, TW_EASEBACKOUT, true, CC_CALLBACK_0(L7_S3_PZDoorPuzzle::onRingOpenComple, this));
			_pzState = L7S3_PZState_RingOpenDelay;
		}
		break;
	
	case L7S3_PZState_RingOpenDelay:
		break;

	case L7S3_PZState_RingOpened:
		{
			pzController->ActivatePuzzle(true);
			_pzState = L7S3_PZState_PlayingPuzzle;
		}
		break;

	case L7S3_PZState_PlayingPuzzle:
		{
			pzController->Update();

			if (pzController->getIsPzCompleted())
			{
				Control::Audio->QuickLoadAndPlaySFX("s3pz1_puzzlecomplete");
				_pzState = L7S3_PZState_AllLocksUnlocked;
			}
		}
		break;
	
	case L7S3_PZState_Skipping:
		{
			pzController->SkipPuzzle();
			_pzState = L7S3_PZState_AllLocksUnlocked;
		}
		break;

	case L7S3_PZState_AllLocksUnlocked:
		{
			AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			if (_handleExtrasGameComplete())
			{
				_pzState =L7S3_PZState_GameForExtrasCompleted;
				return;
			}
			// Show Some particles
			Hud->InvBox_StopPuzzleMode();
			pzController->DisableAllControlls();
			HOScene->GetObjectByID("MagicRing")->FadeOut();
			HOScene->GetObjectByID("HandCloth")->FadeOut();
			_pzState++;
		}
		break;

	case L7S3_PZState_WaitingToPickLetter:
		{

		havenText->SetPos(HOScene->GetObjectByID("Letter")->GetPosPointer());
		havenText->SetRotate(HOScene->GetObjectByID("Letter")->GetRotate());
		havenText->SetScale(HOScene->GetObjectByID("Letter")->GetScale());
		havenText->SetAlpha(HOScene->GetObjectByID("Letter")->GetAlpha());
			if (_isAnimPlaying )
				return;
			
			if(LetterOnDoor.Intersection(&pos))
			{
				Cursor::SetMode(CA_Look);
				if (Control::Input->LBclicked())
				{
					_janim_TakeLetter->PlayAnim();
					havenText->Show(true);
					Control::Audio->QuickLoadAndPlaySFX("s3pz1_notezoomin");
					_isAnimPlaying = true;
				}
			}
		}
		break;
	
	case L7S3_PZState_WaitingForLetterClose:
		{
		havenText->SetPos(HOScene->GetObjectByID("Letter")->GetPosPointer());
		havenText->SetRotate(HOScene->GetObjectByID("Letter")->GetRotate());
		havenText->SetScale(HOScene->GetObjectByID("Letter")->GetScale());
		if (_isAnimPlaying)
		{
			return;
		}
			
			if(LetterBig.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("s3pz1_notezoomout");
					Control::Audio->PlaySample(aVOL7MaryRefS3_4);
					_janim_LetterToMap->PlayAnim();
					_isAnimPlaying = true;
					Hud->ShowBannerText(Banner_L7_S3_AfterReadNote);
				}
			}
		}
		break;

	case L7S3_PZState_WaitingToOpenDoor:
		{
			if (Control::Audio->IsPlaying(aVOL7MaryRefS3_4))
			{
				return;
			}

			_pzState = L7S3_PZState_WaitToEnterNextScene;
			ProfileSystem::Instance->ActiveProfile.L7_S3_PZDoorState = _pzState;
			Control::Audio->PlaySample(aSFXMagic2);
			ProfileSystem::Instance->ActiveProfile.L7_S3_PZDoorCompleted = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_Haven;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
			return;
		}
		break;

	case L7S3_PZState_OPeningDoor:
		{
		}
		break;
	
	case L7S3_PZState_DoorOpened:
		{
			
		}
		break;

	case L7S3_PZState_WaitToEnterNextScene:
		{
			if (NextAreaRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if (Control::Input->LBclicked())
				{
					if( _cvdo_nightMare )
						_cvdo_nightMare->PlayFadeInVideo(2);
					Hud->EnableInput(false);
					Hud->HideHud(false);
					_pzState = L7S3_PZState_NightMareVdoPlaying;
				}
			}
		}
		break;

	case L7S3_PZState_NightMareVdoPlaying:
		{
			if( _cvdo_nightMare )
				//Subtitles Code
				time = _cvdo_nightMare->GetTime();

				if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
				{
					subtitleText->SetText(subtitles[subIndex].text);
				}
				else
					subtitleText->SetText("");

				if (time > subtitles[subIndex].end)
				{
					if (subMax - 1 > subIndex)
						subIndex++;
				}
#ifdef FORCE_CLOSE_VIDEO
				vidTimer2 += Control::LogicRate;
#endif

				if (
#ifndef FORCE_CLOSE_VID_END
					_cvdo_nightMare->isEndReached() ||
#endif
					vidTimer2 > 23)
				{				
					subtitleText->SetText("");
					doorOpenedScene->SetZOff(60);
					doorOpenedScene->Show(true);
					_cvdo_nightMare->StopAndFadeOutVideo(1);
					Hud->AllowInput = true;
					Hud->ShowHud(false);
					Hud->ShowBannerText(Banner_L7_S3_AfterNightmareVdo);
					ProfileSystem::Instance->ActiveProfile.L7_S3_IsNightmareCutscnPlayed = true;

					//Journal Entry Add
					if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P9_Nightmare])
					{
						//Flag Set
						ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P9_Nightmare] = true;

						//Add to Note Array
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P9_Nightmare;

						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

						//Increment Notes
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
						Hud->ShowJrnEntryFX();
					}

					_pzState = L7S3_PZState_NightMareVdoComplete;
				}

			return;
		}
		break;
	
	case L7S3_PZState_NightMareVdoComplete:
		{
			if (NextAreaRect.Intersection(&pos))
			{
				
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitUp, eArea_L7_S5_SophiaEnd);
#else
		Cursor::SetMode(CA_ExitUp, eArea_L7_S5_SophiaEnd);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S5_SophiaEnd, 0, NextAreaRect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S5_SophiaEnd;
					SendMessage(ControlExit, ID, HOSM_NewArea);		
					_pzState = L7S3_PZState_DoorClicked;
#endif
					
				}
			}
		}
		break;

	case L7S3_PZState_GameForExtrasCompleted:
		break;
	case L7S3_PZState_DoorClicked:
		break;
	default:
		_pzState++;
		return;
	}

	//don't backtrack while we anim  going on....
	if( _pzState != L7S3_PZState_RingOpenDelay )
	{
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_Haven;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
		#endif	
	
		if( IsMouseOverGoBackArea(&pos) )
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L7_S3_Haven);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L7_S3_Haven);
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
}

void L7_S3_PZDoorPuzzle::ReceiveMessage(int val)
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

void L7_S3_PZDoorPuzzle::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_pzState = L7S3_PZState_Skipping;
}

void L7_S3_PZDoorPuzzle::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 3;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: 
			{
				++iNonHOHint;
				switch(_pzState)
				{
				case L7S3_PZState_WaitToCollectLever:
					Hud->InitHintTrailEmit(&LeverRect);
					return;

				case L7S3_PZState_WaitingForPower:
					Hud->InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
					return;

				case L7S3_PZState_WaitingToPickLetter:
					Hud->InitHintTrailEmit(&LetterOnDoor);
					return;

				case L7S3_PZState_WaitingForLetterClose:
					Hud->InitHintTrailEmit(&LetterBig);
					return;

				case L7S3_PZState_WaitingToOpenDoor:
					Hud->InitHintTrailEmit(&PZAreaRect);
					return;

				case L7S3_PZState_WaitToEnterNextScene:
					Hud->InitHintTrailEmit(&PZAreaRect, true, CA_ExitUp);
					return;

				case L7S3_PZState_NightMareVdoComplete:
					Hud->InitHintTrailEmit(&PZAreaRect, true, CA_ExitUp);
					return;
				}


			}
			break;
		
		case 1: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 2: ++iNonHOHint;
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

L7_S3_PZDoorPuzzle::~L7_S3_PZDoorPuzzle()
{
	Hud->InvBox_StopPuzzleMode();
	SAFE_DELETE(_cvdo_ThreadCut);
	SAFE_DELETE(_janim_LetterToMap);
	SAFE_DELETE(_janim_TakeLetter);
	SAFE_DELETE(threadScene);
	SAFE_DELETE(doorOpenedScene);


	SAFE_DELETE(subtitleText);
	SAFE_DELETE(pzController);
	SAFE_DELETE(_cvdo_nightMare);

	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL7MaryRefS3_4);
	Control::Audio->StopSample(aAmbL7s3HavenAmb);
	Control::Audio->UnloadSample(aAmbL7s3HavenAmb);
	//MUSIC_SFX
}


void L7_S3_PZDoorPuzzle::onAnimComplete(int animId)
{
	_isAnimPlaying = false;

	if (animId == 1)
		_pzState = L7S3_PZState_TreadsRemoved;
	else if (animId == 2)
	{
		_pzState = L7S3_PZState_WaitingForLetterClose;
		//havenText->Show(true);
	}
	else if(animId == 3)
	{
		_pzState = L7S3_PZState_WaitingToOpenDoor;
		havenText->Show(false);
		//Journal Entry Add
		if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P5_HavenOfResurrectionNotice])
		{
			//Flag Set
			ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P5_HavenOfResurrectionNotice] = true;

			//Add to Note Array
			ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P5_HavenOfResurrectionNotice;

			ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

			//Increment Notes
			ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		}

		Hud->ShowNewInfoTextBanner();
	}
}

void L7_S3_PZDoorPuzzle::onRingOpenComple()
{
	_pzState = L7S3_PZState_RingOpened;
}

void L7_S3_PZDoorPuzzle::NavigateToScene()
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