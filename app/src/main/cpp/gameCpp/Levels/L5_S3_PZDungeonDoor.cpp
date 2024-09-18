//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S3_PZDungeonDoor.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

const CRectangle  MainRotateButton(719.0f,292.0f,110.0f,116.0f);

const CRectangle  SwapLockets1(681.0f,171.0f,185.0f,62.0f);
const CRectangle  SwapLockets2(593.0f,263.0f,70.0f,170.0f);
const CRectangle  SwapLockets3(893.0f,257.0f,56.0f,184.0f);
const CRectangle  SwapLockets4(679.0f,470.0f,185.0f,60.0f);

const CRectangle SwapLockets[] = {SwapLockets1,SwapLockets2,SwapLockets3,SwapLockets4};

const CRectangle  HookDropRect(195.0f,215.0f,293.0f,280.0f);

const float AngleOffsetForRings[] = {0, 90, 270, -180};

const float TweenSpeed = 1;

namespace Level5_Scene3			  
{								  
	Locket::Locket()
	{
		_animTimer = 1;
		_startAngle = _endAngle = 0; 
		_currentIndex = 0;
		_color = PZ_Color_None;
		_colorOffsetVal = 0;
	}

	Locket::~Locket()
	{
	}

	void Locket::Update(float ds)
	{
		CObject::Update(ds);
	
		if (_startAngle != _endAngle)
		{
			_animTimer += Control::LogicRate;
			if(_animTimer >= 1)
			{
				_startAngle = _endAngle;
				_animTimer = 1;
			}

			float newAngle = _startAngle + (_endAngle - _startAngle) * easing(_animTimer, 0, 1, 1);
			CPoint newPos;
			newPos.x = _basePos.x + _radious * Elixir::Cos((float)(Elixir::NormalizeAngle(newAngle)));
			newPos.y = _basePos.y + _radious * Elixir::Sin((float)(Elixir::NormalizeAngle(newAngle)));
			SetPos(&newPos);
			SetRotate(newAngle);
		}
	}

	void Locket::setRotationProperty(CPoint bPos, CPoint tNextPos, float angleOffset)
	{
		_basePos = bPos;
		_radious = abs(Elixir::Distance(&bPos, &tNextPos));

		_startAngle = _endAngle = (90 * _currentIndex) + angleOffset;
	}

	void Locket::MoveTo(CPoint lastPos)
	{
		float valArray[5] = {1, lastPos.x, lastPos.y, 1, GetRotate()};
		TweenTo(valArray, TweenSpeed, 0, true);
	}

	void Locket::Rotate()
	{
		_endAngle += 90;
		_animTimer = 0;
	}




	LocketsHolder::LocketsHolder()
	{
		_index = 0;
		_angleOffset = 0;
		_color = PZ_Color_None;
	}

	LocketsHolder::~LocketsHolder()
	{
	}

	void LocketsHolder::AddLocket(Locket* locket)
	{
        locket->SetPos(&_locketPoints[(int)_lockets.size()]);  //Sac: converted Implicit to explicit

		locket->setRotationProperty(Pos, locket->Pos, _angleOffset);
		_lockets.push_back(locket);
	}

	void LocketsHolder::AddPoint(int index, CPoint point)
	{
		_locketPoints[index] = point;
	}

	void LocketsHolder::Rotate()
	{
		if (IsTweening)
			return;

		float valArray[5] = {1, Pos.x, Pos.y, 1, GetRotate() - 90};
		TweenTo(valArray, TweenSpeed, 0, true, CC_CALLBACK_0(LocketsHolder::OnRotationComplete, this));

		int nextIndex = 0;

		for (int i = 0; i < (int)(_locketPoints.size()); i++)
		{
			if (++nextIndex == (int)_locketPoints.size())
			{
				nextIndex = 0;
			}

			_lockets[i]->Rotate();
			_lockets[i]->setIndex(nextIndex);
		}

		std::rotate(_lockets.rbegin(), _lockets.rbegin() + 1, _lockets.rend());
	}

	void LocketsHolder::OnRotationComplete()
	{
		int stId = _index*PZ_Color_TotalColors;
		for (int i = 0; i < (int)(_lockets.size()); i++)
		{
			ProfileSystem::Instance->ActiveProfile.L5_S3_PZLocketIndexes[i + stId] = (_lockets[i]->getColor() * PZ_Color_TotalColors) + _lockets[i]->getColorOffsetVal();
		}
	}

	Locket* LocketsHolder::GetLocket(int index)
	{
		return _lockets[index];
	}

	void LocketsHolder::Exchange(Locket* locket, int index)
	{
		CPoint pos = _locketPoints[index];
		locket->setIndex(index);
		locket->setRotationProperty(Pos, pos, _angleOffset);
		locket->MoveTo(pos);
		_lockets[index] = locket;
	}

	bool LocketsHolder::IsSolved()
	{
		for (int i = 0; i < (int)(_lockets.size()); i++)
		{
			if (_color != _lockets[i]->getColor())
			{
				return false;
			}
		}

		return true;
	}

	
	PuzzleController::PuzzleController()
	{
		_isPuzzleActive = false;
		_isSolved = false;
		_puzzleButton = NULL;
	}

	PuzzleController::~PuzzleController()
	{
		cout<<"Dist";
		SAFE_DELETE(_puzzleButton);

		for (int i = 0; i < (int)_swapBtns.size(); i++)
		{
			SAFE_DELETE(_swapBtns[i]);
		}

		_swapBtns.clear();
	}

	void PuzzleController::InitController(vector<CObject*> holders, vector<CObject*> lockets, CHOScene *scene)
	{

		for (int i = 0; i < (int)(holders.size()); i++)
		{
			string spr = holders[i]->Filename;
			CPoint position = holders[i]->Pos;
			int zOrder = holders[i]->ZOrder;
			string uid = holders[i]->UID;
			ColorType color = GetColorTypeFrom(uid);

			holders[i]->SetEnable(false);

			LocketsHolder *obj = new LocketsHolder();
			obj->LoadInit(spr, position.x, position.y, zOrder);
			obj->SetUID(uid);
			obj->setHolderIndex(i);
			obj->setColor(color);
			obj->setAngleOffset(AngleOffsetForRings[i]);
			scene->PushObject(obj);
			_baseHolders[color] = obj;
		}

		for (int i = 0; i < (int)(lockets.size()); i++)
		{
			ColorType color = GetColorTypeFrom(lockets[i]->UID);
			_baseHolders[color]->AddPoint(i%PZ_Color_TotalColors, lockets[i]->Pos);
		}

		vector<CObject*> shuffledList;
		for (int i = 0; i < (int)(lockets.size()); i++)
		{
			int index = ProfileSystem::Instance->ActiveProfile.L5_S3_PZLocketIndexes[i];
			shuffledList.push_back(lockets[index]);
		}

		std::map<ColorType, int> forSave;
		for (int i = 0; i < (int)(shuffledList.size()); i++)
		{
			string spr = shuffledList[i]->Filename;
			CPoint position = shuffledList[i]->Pos;
			int zOrder = shuffledList[i]->ZOrder;
			string uid = shuffledList[i]->UID;
			ColorType color = GetColorTypeFrom(uid);

			int colorOffset = forSave[color];

			shuffledList[i]->SetEnable(false);
			
			Locket *obj = new Locket();
			obj->LoadInit(spr, position.x, position.y, zOrder);
			obj->SetUID(uid);
			obj->setIndex(i%int(PZ_Color_TotalColors));
			obj->setColor(color);
			obj->setColorOffsetVal(colorOffset);
			scene->PushObject(obj);
			_locketsForSkip.insert(std::make_pair(obj, obj->Pos)); // for handle skip
			_baseHolders[ColorType(i/int(PZ_Color_TotalColors))]->AddLocket(obj);

			++colorOffset;
			forSave[color] = colorOffset;
		}

		forSave.clear();

		for (int i = 1; i <= 4; i++)
		{
			UIButton* btn = new UIButton(scene->GetObjectByID("SwapBtn_"+MKSTR(i)));
			btn->SetRotate(scene->GetObjectByID("SwapBtn_"+MKSTR(i))->GetRotate());
			btn->Disable();
			_swapBtns.push_back(btn);
		}
	}

	void PuzzleController::ActivatePuzzle()
	{
		_isPuzzleActive = true;

		_puzzleButton->Enable();
		for (int i = 0; i < 4; i++)
		{
			_swapBtns[i]->Enable();
		}
	}

	void PuzzleController::Update()
	{
		if (!_isPuzzleActive)
			return;

		CPoint pos = Control::Input->Mpos();
				
		if (MainRotateButton.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			if (Control::Input->LBclicked())
			{
				if(!_pzBtnRing->IsTweening)
				{
					float valArray[5] = {-1, -1, -1, -1, _pzBtnRing->GetRotate() - 90};
					_pzBtnRing->TweenTo(valArray, TweenSpeed, 0, true);
				}

				Control::Audio->QuickLoadAndPlaySFX("s3pz1_tilerotate");
				std::map<ColorType, LocketsHolder*>::iterator itr;
				for (itr = _baseHolders.begin(); itr != _baseHolders.end(); ++itr)
				{
					itr->second->Rotate();
				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (_swapBtns[i]->GetWorldRect().Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
			
				if (Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("s3pz1_tileswap");

					std::vector<SwapObject> swapObjs;
					std::map<ColorType, LocketsHolder*>::iterator itr;
					for (itr = _baseHolders.begin(); itr != _baseHolders.end(); ++itr)
					{
						for (int j = 0; j < 4; j++)
						{
							Locket* locket = itr->second->GetLocket(j);
						
							if (SwapLockets[i].Intersection(locket->GetPosPointer()))
							{
								SwapObject obj = {locket->getIndex(), locket, itr->second};
								swapObjs.push_back(obj);
							}
						}
					}
					
					for (int i = 0; i < (int)(swapObjs.size()); i++)
					{
						swapObjs[0]._locketHolder->Exchange(swapObjs[1]._locket, swapObjs[0]._locketIndex);
						std::rotate(swapObjs.rbegin(), swapObjs.rbegin() + 1, swapObjs.rend());
					}

					_isSolved = true;

					for (itr = _baseHolders.begin(); itr != _baseHolders.end(); ++itr)
					{
						if(!itr->second->IsSolved())
							_isSolved = false;
					}
				}
			}
		}
	}

	ColorType PuzzleController::GetColorTypeFrom(string uid)
	{
		ColorType type = PZ_Color_None;

		if (uid.find("Orange") != string::npos)
			type = PZ_Color_Orange;
		else if (uid.find("Purple") != string::npos)
			type = PZ_Color_Purple;
		else if (uid.find("Blue") != string::npos)
			type = PZ_Color_Blue;
		else if (uid.find("Green") != string::npos)
			type = PZ_Color_Green;

		return type;
	}

	void PuzzleController::Skip()
	{
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		std::map<Locket*, CPoint>::iterator itr;
		for (itr = _locketsForSkip.begin(); itr != _locketsForSkip.end(); ++itr)
		{
			itr->first->SetPos(&itr->second);
		}

		_isSolved = true;	
	}



	PuzzleScene::PuzzleScene()
	{

	}

	PuzzleScene::~PuzzleScene()
	{

	}

	void PuzzleScene::LoadPuzzle(const std::string& scnFile)
	{
		Load(scnFile);

		GetObjectByID("SwapBtn_1")->SetRelativeRotate(90);
		GetObjectByID("SwapBtn_4")->SetRelativeRotate(90);
	}

	PuzzleController* PuzzleScene::CreateController()
	{
		PuzzleController* controler = new PuzzleController();
		vector<CObject*> lockets;
		vector<CObject*> holders;

		for (int i = 0; i < (int)(Objects.size()); i++)
		{
			if (Objects[i]->UID.find("MainDisc") != string::npos)
			{
				holders.push_back(Objects[i]);
			}
			else if(Objects[i]->UID.find("Locket") != string::npos)
			{
				lockets.push_back(Objects[i]);
			}
		}

		controler->InitController(holders, lockets, this);
		controler->setPuzzleButton(new UIButton(GetObjectByID("CenterBtn")));
		controler->setPZBtnRing(GetObjectByID("CenterBtnRing"));
		controler->getPuzzleButton()->Disable();
		return controler;
	}
}

using namespace Level5_Scene3;

L5_S3_PZDungeonDoor::L5_S3_PZDungeonDoor()
{
	//Main Scene
	LoadHud(INVENTORY_V1);

	PuzzleScene *scene = new PuzzleScene();
	scene->LoadPuzzle("LV5\\S3\\PZ1\\PZ1.SCN");
	_puzzleCtrl = scene->CreateController();
	
	if (HOScene)
	{
		delete HOScene;
		HOScene = NULL;
	}

	HOScene = scene;	

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

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s3DungeonDoorPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s3DungeonDoorPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s3DungeonDoorPuzzleAmb,true);
	}
	
	//MUSIC_SFX
	
	pzHelpText = NULL;

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	float _lineWidth = 700;
	int _nLines = 3;
	EFont _font = eFont_Small;
	eTextAlignment _align = Align_Center;
	std::string temp = PZHelpTextArray[ePZHelp_L5_S3_DoorPuzzle];
	if(StringTable::IsKeyFound(temp))
	{
		_lineWidth = StringTable::GetLineWidth(temp);
		_nLines = StringTable::GetNoOfLines(temp);
		_font = (EFont)StringTable::GetFont(temp);
	}
	pzHelpText = new MultilineText(_nLines, PointSystem::CreateCPoint(683, 688, 0), eZOrder_INVBOXITEMS, gold, 30, _lineWidth, _align, _font);

	pzHelpText->SetTextMultiline(StringTable::getStr(temp));
	pzHelpText->Show(false);
	

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S3_PZDungeonDoor]  = true;

	_JAnim_SceneAnimation = new JSONAnimator(1, CC_CALLBACK_1(L5_S3_PZDungeonDoor::OnAnimatonComplete, this));
	_JAnim_SceneAnimation->parseAndLoadJSON("ANIM\\L5\\S3_HOOKMISSINGANIM.JSON",HOScene);

	_stateMachine.push(L5S3PZ_ReachFirstTime);
	_state = ProfileSystem::Instance->ActiveProfile.L5_S3_PZState;

	HOScene->GetObjectByID("GearAnim")->Paused = true;
	InitWithState();
}

void L5_S3_PZDungeonDoor::InitWithState()
{
	switch (_state)
	{
	case L5S3PZ_ReachFirstTime:
		{
			HOScene->GetObjectByID("ChainHangingLongLength")->SetEnable(false);
			HOScene->GetObjectByID("ChainHanging")->SetEnable(false);
		}
		break;

	case L5S3PZ_WaitingForHook:
		{
			HOScene->GetObjectByID("Hook")->SetEnable(false);
			HOScene->GetObjectByID("Hook_Clip01")->SetEnable(false);
			HOScene->GetObjectByID("Hook_Clip02")->SetEnable(false);
			HOScene->GetObjectByID("Chain02")->SetEnable(false);
			HOScene->GetObjectByID("ChainHanging")->SetEnable(false);
		}
		break;

	case L5S3PZ_HookPlaced:
	case L5S3PZ_PlayingGame:
		{
			_puzzleCtrl->ActivatePuzzle();
			Hud->InvBox_StartPuzzleMode();
			pzHelpText->Show(true);
			HOScene->GetObjectByID("ChainHangingLongLength")->SetEnable(false);
			HOScene->GetObjectByID("ChainHanging")->SetEnable(false);
		}
		break;

	default:
		break;
	}
}

void L5_S3_PZDungeonDoor::Required()
{
	HiddenObjectSystem::Required();	//forced update
	if( Control::Audio->IsPlaying(aTrackL5theme) )
	{
		Control::Audio->Samples[aTrackL5theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL5theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL5theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL5puzzle) )
				  Control::Audio->PlaySample(aTrackL5puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL5puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL5puzzle, true);
		}
	}
}

void L5_S3_PZDungeonDoor::Update()
{
	NavigateToScene();

	if( Hud->PopupState != Popup_Inactive )
		return;

#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif

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
	Cursor::SetMode(CA_Normal);
	if (_stateMachine.top() != _state)
	{
		ProfileSystem::Instance->ActiveProfile.L5_S3_PZState = _state;
		_stateMachine.push(_state);
	}

	switch (_state)
	{
	case L5S3PZ_ReachFirstTime:
		if(MainRotateButton.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			if (Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s3pz1_chainbreak");

				_JAnim_SceneAnimation->PlayAnim();
				ProfileSystem::Instance->ActiveProfile.L5_S3_PZHaveHook = false;
				ProfileSystem::Instance->ActiveProfile.L5_S3_isLostHook = true;
				_state = L5S3PZ_HookFallAnim;
			}
		}
		break;

	case L5S3PZ_HookFallAnim:
		if(ProfileSystem::Instance->ActiveProfile.L5_S3_isLostHook)
		{
			if(_JAnim_SceneAnimation != NULL)
			{
				if(_JAnim_SceneAnimation->Filename == "ANIM\\L5\\S3_HOOKMISSINGANIM.JSON")
				{
					if(!_JAnim_SceneAnimation->IsAnimPlaying())
						_state++;
				}
			}
		}
		return;

	case L5S3PZ_HookLost:
		{
			_state = L5S3PZ_WaitingForHook;

			if(ProfileSystem::Instance->ActiveProfile.L5_S5_InvWaterproofBagTaken)
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S3_Pond] = true;

			ProfileSystem::Instance->ActiveProfile.L5_S3_PZState = _state;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
		}
		break;

	case L5S3PZ_WaitingForHook:
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked() && !IsMouseOverGoBackArea(&pos))
			{
				Hud->ShowBannerText(Banner_L5_S3_ClickAfterHookLost);
			}
		}
		break;

	case L5S3PZ_PlacingHook:
		if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S3_Hook])
			{
				if(_JAnim_SceneAnimation->Filename == "ANIM\\L5\\S3_HOOKMISSINGANIM.JSON")
					{
						_state = L5S3PZ_HookPlaced;
						HOScene->GetObjectByID("ChainHangingLongLength")->SetEnable(false);
						HOScene->GetObjectByID("ChainHanging")->SetEnable(false);
					}
			}
		return;

	case L5S3PZ_HookPlaced:
		{
			_puzzleCtrl->ActivatePuzzle();
			Hud->InvBox_StartPuzzleMode();
			pzHelpText->Show(true);
			_state = L5S3PZ_PlayingGame;
		}
		break;

	case L5S3PZ_PlayingGame:
		{
			_puzzleCtrl->Update();

			if (_puzzleCtrl->getIsSolved())
			{
				if (_handleExtrasGameComplete())
				{
					_state = L5S3PZ_GameForExtrasComplete;
					return;
				}

				_state++;
			}
		}
		break;

	case L5S3PZ_PuzzleSolved:
		{
			AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			Hud->InvBox_StopPuzzleMode();
			pzHelpText->Show(false);
			SAFE_DELETE(_JAnim_SceneAnimation);
			_JAnim_SceneAnimation = new JSONAnimator(3, CC_CALLBACK_1(L5_S3_PZDungeonDoor::OnAnimatonComplete, this));
			_JAnim_SceneAnimation->parseAndLoadJSON("ANIM\\L5\\S3_CHAINDOOROPEN.JSON",HOScene);
			_JAnim_SceneAnimation->PlayAnim();
			HOScene->GetObjectByID("GearAnim")->Paused = false;

			Control::Audio->QuickLoadAndPlaySFX("s3pz1_puzzlecomplete");
			Control::Audio->QuickLoadAndPlaySFX("s4p1_discrotate");

			_state = L5S3PZ_OnComplete;
		}
		break;

	case L5S3PZ_OnComplete:
		if(!_JAnim_SceneAnimation->IsAnimPlaying())
		{
			if(!ProfileSystem::Instance->ActiveProfile.L5_S3_PZCompleted)
				_state = L5S3PZ_PlayingGame;
		}
		
		return;

	case L5S3PZ_ExitFromScene:
		{
			ProfileSystem::Instance->ActiveProfile.L5_S3_PZCompleted = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
			_state++;
		}
		break;

	case L5S3PZ_GameForExtrasComplete:
		break;

	default:
		_state++;
		return;
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif	

	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L5_S3_Pond);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L5_S3_Pond);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S3_Pond, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
			return;
		}	
	}
}

void L5_S3_PZDungeonDoor::ReceiveMessage(int val)
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
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			if (_state == L5S3PZ_WaitingForHook && itemVal == eInv_L5_S3_Hook)
			{
				Control::Audio->QuickLoadAndPlaySFX("s3pz1_hookfix");

				SAFE_DELETE(_JAnim_SceneAnimation);
				_JAnim_SceneAnimation = new JSONAnimator(2, CC_CALLBACK_1(L5_S3_PZDungeonDoor::OnAnimatonComplete, this));
				_JAnim_SceneAnimation->parseAndLoadJSON("ANIM\\L5\\S3_HOOKPLACINGANIM.JSON",HOScene);
				_JAnim_SceneAnimation->PlayAnim();
				_state = L5S3PZ_PlacingHook;

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S3_Hook] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S3_Hook);
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L5_S3_PZDungeonDoor::ProcessSkip()
{
	_puzzleCtrl->Skip();
}

void L5_S3_PZDungeonDoor::ProcessNonHOHint()
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
				{
					switch (_state)
					{
					case L5S3PZ_ReachFirstTime:
						{
							Hud->InitHintTrailEmit(&MainRotateButton);
							return;
						}
						break;

					case L5S3PZ_WaitingForHook:
						{
							if (ProfileSystem::Instance->ActiveProfile.L5_S3_PZHaveHook)
								Hud->ShowHintForInvItem(eInv_L5_S3_Hook, &HookDropRect);
							else
								Hud->InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
							return;
						}
						break;
					default: break;
					}
				}
			break;
		case 1: ++iNonHOHint;
			/*if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S3_CaveMouth])
			{
			InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
			return;
			}*/
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

L5_S3_PZDungeonDoor::~L5_S3_PZDungeonDoor()
{
	SAFE_DELETE(_JAnim_SceneAnimation);

	if(!_puzzleCtrl->getIsSolved())
		Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(_puzzleCtrl);
	SAFE_DELETE(pzHelpText);
	SAFE_DELETE(ResetDia);

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL5s3DungeonDoorPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL5s3DungeonDoorPuzzleAmb);
	//MUSIC_SFX
}

void L5_S3_PZDungeonDoor::OnAnimatonComplete(int animID)
{
	if (animID == 1)
	{
		Hud->ShowBannerText(Banner_L5_S3_HookFallsIntoTheWater);
		_state++;	
	}
	else if(animID == 2)
	{
		_state = L5S3PZ_HookPlaced;
	}
	else if(animID == 3)
	{
		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O3_OpenTheDungeon] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O3_OpenTheDungeon] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O3_OpenTheDungeon] = true;
			Hud->ShowObjectiveFX(eObj_L5O3_OpenTheDungeon,true);
		}

		_state = L5S3PZ_ExitFromScene;
	}
}
void L5_S3_PZDungeonDoor::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(_state != L5S3PZ_PlayingGame)
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
			int tmpInitialState[16] = {0,12,4,8,13,5,1,2,10,11,7,15,14,3,9,6};
			for (int i = 0; i < 16; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L5_S3_PZLocketIndexes[i] = tmpInitialState[i];
			}

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_PZDungeonDoor;
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
void L5_S3_PZDungeonDoor::NavigateToScene()
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