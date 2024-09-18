
//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "L8_S5_VirtualMiniGame.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

const CRectangle  PZAreaRect(171.0f,5.0f,1020.0f,605.0f);
const float RotateOffset = 45;

namespace Level8_Scene5
{
	const char* itemNames[] = {"Bull_Head",
	"Axe",
	"Bird", 
	"Fish", 
	"Lotus", 
	"Deer", 
	"Crescent", 
	"Butterfly", 
	"Wheel"};


	HiddenObject::HiddenObject()
	{
		_isComplete = false;
		_name = "";
		_nameText = NULL;
		_toFindObj = NULL;
		_completeObj = NULL;
		_polygonArea = NULL;
		_isFound = false;

		HOTrailFX = NULL;
	}

	HiddenObject::~HiddenObject()
	{
		_name = "";
		SAFE_DELETE(_nameText);
		_toFindObj = NULL;
		_completeObj= NULL;
		SAFE_DELETE(_polygonArea);
		_isFound = false;
		SAFE_DELETE(HOTrailFX);
	}

	void HiddenObject::InitObject()
	{
		InitInvNameText();

		_completeObj->Show(false);
		_completeObj->Status = SPR_INVISIBLE;

		HOTrailFX = NULL;
		HOTrailFX = new SuperFX("PRT\\L8\\s5VirtualGameHO.INI", PointSystem::CreateCPoint(1065,122,0), 200);
	}

	void HiddenObject::InitInvNameText()
	{
		CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);
		float vertGap = InvAreaRect.h/3.0f, horizontalGap = InvAreaRect.w/3.0f;
		CPoint pos(0,0,0);

		pos.x = InvAreaRect.Left() + horizontalGap*0.5f + horizontalGap*(_index%3);
		pos.y = InvAreaRect.Top() + vertGap*0.5f + vertGap*(_index/3);

		ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
		_nameText = new CBitmapText();
		_nameText->SetTextProps(_name, pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_Small, Align_Center);
		_nameText->SetZ(eZOrder_INVBOXITEMS);
	}

	void HiddenObject::EnableHO(bool stratFromBegin)
	{
		if(stratFromBegin)
			_toFindObj->FadeIn();
		else
			_toFindObj->Show(true);

		InitObject();
	}

	void HiddenObject::HideName()
	{
		_nameText->Show(false);
	}

	bool HiddenObject::MouseClicked(CPoint pos)
	{
		if (_isFound)
		{
			return false;
		}

		if (_polygonArea->Intersection(&pos))
		{
			OnFound();
			return true;
		}

		return false;
	}

	void HiddenObject::OnFound()
	{
		HOTrailFX->SetPos(&_toFindObj->Pos);
		HOTrailFX->Start();
		_completeObj->FadeIn();
		_isFound = true;
		_nameText->SetStrikethrough(true);
	}

	void HiddenObject::AlreadyFound()
	{
		_isFound = true;
		_isComplete = true;
		_toFindObj->Show(false);
		_completeObj->Show(true);
		_nameText->SetAlpha(0.5f);
	}


	Shape::Shape()
	{
		_isRotation = false;
		_obj = NULL;
		_hudObj = NULL;
		_polyAreaToPlace = NULL;
		_isComplete = false;
	}

	Shape::~Shape()
	{
		SAFE_DELETE(_polyAreaToPlace);
	}

	void Shape::InitProp()
	{
		_finalPos = _obj->Pos;
		_hudPos = _hudObj->Pos;
	}

	void Shape::AlreadyFixed()
	{
		_hudObj->Show(false);
		_obj->SetRotate(0);
		_obj->Show(true);
		_isComplete = true;
	}

	void Shape::Skipping()
	{
		if(_isComplete)
			return;

		float speed = float(_index + 1);
		_hudObj->FadeOut(float(_index + 1));
		_obj->SetAlpha(0);
		_obj->SetRotate(0);
		_obj->Show(true);

		float valArray1[5] = {1, _finalPos.x, _finalPos.y, 1, -1};
		_obj->TweenTo(valArray1, speed*0.4f, TW_EASE, true);

		_isComplete = true;
	}

	CRectangle Shape::getHUDObjRect()
	{
		return _hudObj->GetWorldRect();
	}

	CRectangle Shape::getObjRect()
	{
		return _obj->GetWorldRect();
	}

	void Shape::OnSelect()
	{
		_hudObj->FadeOut(2);
		_obj->SetScale(1);
		_obj->FadeIn(4);
	}

	void Shape::Relese()
	{
		_hudObj->FadeIn(3);

		float valArray1[5] = {0, _hudPos.x, _hudPos.y, 0.2f, -1};
		_obj->TweenTo(valArray1, 0.3f, TW_EASE, true);
	}

	void Shape::RotateTo(float angleDiff)
	{
		if (_isRotation)
			return;

		float valArray1[5] = {-1, -1, -1, -1, _obj->GetRotate() + angleDiff};
		_obj->TweenTo(valArray1, 0.5f, TW_EASE, true, CC_CALLBACK_1(Shape::OnTweenEnd, this));

		_isRotation = true;
	}

	void Shape::OnTweenEnd(CObject* obj)
	{
		_isRotation = false;

		float currentAngle = (float)Elixir::NormalizeAngle(_obj->GetRotate(), 0, 360);
		_obj->SetRotate(currentAngle);
		_hudObj->SetRotate(currentAngle);
	}

	float Shape::GetRotate()
	{
		return _obj->GetRotate();	
	}

	bool Shape::PlaceAt(CPoint pos)
	{
		if (GetRotate() == 0.0f && _polyAreaToPlace->Intersection(&pos))
		{
			float valArray2[5] = {-1, _finalPos.x, _finalPos.y, -1, -1};
			_obj->TweenTo(valArray2, 0.1f, TW_EASE, true);

			_isComplete = true;
			return true;
		}
		return false;
	}

	void Shape::setPos(CPoint pos)
	{
		_obj->SetPos(&pos);
	}



	
	GameController::GameController()
	{
		_HOScene = NULL;
		_puzzleScene = NULL;
		_holdingShape = NULL;
		_gameCompleteCallback = nullptr;
	}

	GameController::~GameController()
	{
		for (int i = 0; i < (int)_shapes.size(); i++)
		{
			SAFE_DELETE(_shapes[i]);
		}

		for (int i = 0; i < (int)_hiddenObjs.size(); i++)
		{
			SAFE_DELETE(_hiddenObjs[i]);
		}

		SAFE_DELETE(s5VirtualGamePzCompleteFX);
		SAFE_DELETE(s5VirtualGameRoatationFX);
		SAFE_DELETE(_HOScene);

		_hiddenObjPolys.clear();
		_shapeDropPolys.clear();
		_hiddenObjs.clear();
		_shapes.clear();

		_hud->InvBoxCtrl->ResetToInvBox();
		_hud->InvBox_StopPuzzleMode();
	}

	void GameController::CreateGame(CHOScene* pzscene, CHOScene* HOGameScene, HudControl *hud, std::function<void ()> gameCompleteCallback)
	{
		_puzzleScene = pzscene;
		_HOScene = HOGameScene;
	 	_state = ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameState;
		_stateMachine.push(_state);
		_gameCompleteCallback = gameCompleteCallback;
		_hud = hud;

		InitPolyList();

		float Rotations[] = {45, 225, 135};

		for (int i = 0; i < 3; i++)
		{
			CObject* shapeObj = _puzzleScene->GetObjectByID("Blue_Shape_"+MKSTR(i));
			shapeObj->Show(false);
			shapeObj->SetZ(eZOrder_INVBOXITEMS);
			CObject* shapeObjHud = _puzzleScene->GetObjectByID("Blue_Shape_Hud_"+MKSTR(i));
			shapeObjHud->SetZ(eZOrder_INVBOXITEMS);

			shapeObj->SetRotate(Rotations[i]);
			shapeObjHud->SetRotate(Rotations[i]);

			Shape* shape = new Shape();
			shape->setIndex(i);
			shape->setObj(shapeObj);
			shape->setHudObj(shapeObjHud);
			shape->setPolyAreaToPlace(_shapeDropPolys[i]);
			_shapes.push_back(shape);
			shape->InitProp();

			if (ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameShapesComplete[i])
			{
				shape->AlreadyFixed();
			}
		}

		for (int i = 0; i >= 0; i++)
		{
			CObject* hiddenObj = _HOScene->GetObjectByID("HiddenObj_"+MKSTR(i));
			CObject* foundObj = _HOScene->GetObjectByID("FoundObj_"+MKSTR(i));

			if (!hiddenObj)
				break;

			hiddenObj->Show(false);
			foundObj->Show(false);

			HiddenObject* ho = new HiddenObject();
			ho->setIndex(i);
			ho->setToFindObject(hiddenObj);
			ho->setCompleteObject(foundObj);
			ho->setName(StringTable::getStr(itemNames[i]));
			ho->setPolygonArea(_hiddenObjPolys[i]);
			_hiddenObjs.push_back(ho);
		}

		_rotateBtn = _puzzleScene->GetObjectByID("RotatButton");

		_otherObjs.push_back(_HOScene->GetObjectByID("OtherObjects"));

		for (int i = 0; i < (int)_otherObjs.size(); i++)
		{
			_otherObjs[i]->Show(false);
		}
		s5VirtualGameRoatationFX = new SuperFX("PRT\\L8\\s5VirtualGameRoatation.INI", PointSystem::CreateCPoint(1111,503,0), 1100); 
		s5VirtualGameRoatationFX->SetPos(&_rotateBtn->Pos);

		s5VirtualGamePzCompleteFX = new SuperFX("PRT\\L8\\s5VirtualGamePzComplete.INI", PointSystem::CreateCPoint(650,260,0), 1100); 

		if (_state >= GameState_PuzzleComplete)
		{
			_puzzleScene->SetEnable(false);
			_HOScene->SetEnable(true);

			_hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);

			for (int i = 0; i < (int)_hiddenObjs.size(); i++)
			{
				_hiddenObjs[i]->EnableHO(false);

				if (ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameHOState[i])
				{
					_foundObjs.push_back(_hiddenObjs[i]);
					_hiddenObjs[i]->AlreadyFound();
				}
			}

			for (int i = 0; i < (int)_otherObjs.size(); i++)
			{
				_otherObjs[i]->Show(true);
			}
		}
		else
		{
			_state = GameState_Initializing;
		}
	}

	void GameController::UpdateController(CPoint mPos)
	{

		if (_state != _stateMachine.top())
		{
			_stateMachine.push(_state);
			ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameState = _state;
		}

		switch (_state)
		{
		case GameState_Initializing:
			{
				_hud->InvBox_StartPuzzleMode();
				_state = GameState_PlayingPuzzle;
			}
			break;
		
		case GameState_PlayingPuzzle:
			{
				for (int i = 0; i < (int)_shapes.size(); i++)
				{
					if (!_shapes[i]->getIsComplete() && _shapes[i]->getHUDObjRect().Intersection(&mPos))
					{
						Cursor::SetMode(CA_HandTake);

						if (Control::Input->LBclicked())
						{
							_holdingShape = _shapes[i]; 
							_holdingShape->OnSelect();
							Control::Audio->QuickLoadAndPlaySFX("s5pz1_TakePuzzlePiece");
							_state = GameState_HoldingShape; 
						}
					}
				}
			}
			break;
		
		case GameState_HoldingShape:
			{
				_holdingShape->setPos(mPos);

				if (_rotateBtn->GetWorldRect().Intersection(&mPos) && Control::Input->LBclicked())
				{
					float valArray1[5] = {-1, -1, -1, -1, _rotateBtn->GetRotate() + RotateOffset};
					_rotateBtn->TweenTo(valArray1, 0.5f, TW_EASE, true);
					_holdingShape->RotateTo(RotateOffset);
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_rotatePuzzlePiece");
					s5VirtualGameRoatationFX->Start();
					return;
				}

				if (Control::Input->LBclicked())
				{
					if (PZAreaRect.Intersection(&mPos))
					{

						if (_holdingShape->PlaceAt(mPos))
						{
							ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameShapesComplete[_holdingShape->getIndex()] = true;
							_holdingShape = NULL;
							Control::Audio->QuickLoadAndPlaySFX("s5pz1_placePuzzlePiece");
							_state = GameState_ShapePlaced;
						}
					}
					else
					{
						_holdingShape->Relese();
						_holdingShape = NULL;
						_state = GameState_PlayingPuzzle;
					}
				}
			}
			break;
		
		case GameState_ShapePlaced:
			{
				for (int i = 0; i < (int)_shapes.size(); i++)
				{
					if(!_shapes[i]->getIsComplete())
					{
						_state = GameState_PlayingPuzzle;
						return;
					}
				}

				_state = GameState_PuzzleComplete;
				AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			}
			break;

		case GameState_PuzzleComplete:
			{
				s5VirtualGamePzCompleteFX->Start();
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_puzzleComplete");
				_state++;
			}
			break;

		case GameState_StartingHO:
			{
				AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
				_puzzleScene->FadeOut();
				_HOScene->SetEnable(true);
				_HOScene->FadeIn();
				_hud->InvBox_StopPuzzleMode();
				_hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);

				for (int i = 0; i < (int)_hiddenObjs.size(); i++)
				{
					_hiddenObjs[i]->EnableHO();
				}

				for (int i = 0; i < (int)_otherObjs.size(); i++)
				{
					_otherObjs[i]->FadeIn();
				}

				_state = GameState_PlayingHO;
			}
			break;

		case GameState_PlayingHO:
			{
				if (PZAreaRect.Intersection(&mPos) && Control::Input->LBclicked())
				{
					_state = GameState_OnClicked;
				}
			}
			break;
		
		case GameState_OnClicked:
			{
				_state = GameState_PlayingHO;

				for (int i = 0; i < (int)_hiddenObjs.size(); i++)
				{
					if (_hiddenObjs[i]->MouseClicked(mPos))
					{
						_foundObjs.push_back(_hiddenObjs[i]);
						ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameHOState[i] = true;
						Control::Audio->QuickLoadAndPlaySFX("s5ho_hoItemFound");
						_state = GameState_OnFoundItem;
						break;
					}
				}
			}
			break;
		
		case GameState_OnFoundItem:
			{
				_state = GameState_PlayingHO;

				if (_hiddenObjs.size() == _foundObjs.size())
				{
					AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					Control::Audio->QuickLoadAndPlaySFX("s5ho_hoComplete");
					_state = GameState_FoundAllItems;
				}
			}
			break;
		
		case GameState_FoundAllItems:
			{
				_state++;
			}
			break;
		
		case GameState_FadeOutScene:
			{
				_HOScene->FadeOut(2);
				_state++;
			}
			break;

		case GameState_GameComplete:
			{
				for (int i = 0; i < (int)_hiddenObjs.size(); i++)
				{
					_hiddenObjs[i]->HideName();
				}

				_gameCompleteCallback();
				_state++;
			}
			break;
		
		default:
			{
				_state++;
			}
			break;
		}
	}

	void GameController::SkipPuzzle()
	{
		for (int i = 0; i < (int)_shapes.size(); i++)
		{
			_shapes[i]->Skipping();
			ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameShapesComplete[i] = true;
		}

		_state = GameState_PuzzleComplete;
	}

	void GameController::ShowHint()
	{
		for (int i = 0; i < (int)_hiddenObjs.size(); i++)
		{
			if (!_hiddenObjs[i]->getIsFound())
			{
				AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
				//_hud->InitHintTrailEmit(&_hiddenObjs[i]->getToFindObject()->GetWorldRect());
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle selectObjRect =_hiddenObjs[i]->getToFindObject()->GetWorldRect();
				_hud->InitHintTrailEmit(&selectObjRect);
				return;
			}
		}
	}

	bool GameController::canGoBack()
	{
		//if (_shapes[i]->getHudObj()->GetWorldRect().Intersection(&Control::Input->Mpos()))
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CPoint mPos =Control::Input->Mpos();
		for (int i = 0; i < (int)_shapes.size(); i++)
		{
			if (_shapes[i]->getHudObj()->GetWorldRect().Intersection(&mPos))
			{
				return false;
			}
		}

		return true;
	}

	void GameController::InitPolyList()
	{
		int ArrayPts1[] = {506,236,518,284,585,286,578,234};
		CPolygon* CPoly1=  new CPolygon(4);
		CPoly1->Init(ArrayPts1);
		_shapeDropPolys.push_back(CPoly1);

		int ArrayPts2[] = {834,205,829,253,879,272,895,214};
		CPolygon* CPoly2=  new CPolygon(4);
		CPoly2->Init(ArrayPts2);
		_shapeDropPolys.push_back(CPoly2);

		int ArrayPts3[] = {586,388,607,447,674,419,644,361};
		CPolygon* CPoly3=  new CPolygon(4);
		CPoly3->Init(ArrayPts3);
		_shapeDropPolys.push_back(CPoly3);

/*-----------------*/

		int HOArrayPts1[] = {541,512,549,494,524,500,523,519,517,524,533,531,530,548,545,566,565,555,565,531,579,534,584,507,557,491,566,512};
		CPolygon* HOCPoly1=  new CPolygon(14);
		HOCPoly1->Init(HOArrayPts1);
		_hiddenObjPolys.push_back(HOCPoly1);

		int HOArrayPts2[] = {1042,246,1040,273,1044,298,1059,291,1059,283,1068,277,1067,305,1082,307,1084,277,1096,277,1098,256,1084,256,1081,244,1069,246,1069,253,1056,251,1057,244};
		CPolygon* HOCPoly2=  new CPolygon(17);
		HOCPoly2->Init(HOArrayPts2);
		_hiddenObjPolys.push_back(HOCPoly2);

		int HOArrayPts3[] = {881,40,904,39,901,29,921,31,925,40,942,32,951,43,939,50,932,68,907,75,904,84,883,76,900,61,879,49};
		CPolygon* HOCPoly3=  new CPolygon(14);
		HOCPoly3->Init(HOArrayPts3);
		_hiddenObjPolys.push_back(HOCPoly3);

		int HOArrayPts4[] = {362,398,368,382,361,366,382,375,398,366,398,356,420,365,449,379,426,394,393,399,383,385};
		CPolygon* HOCPoly4=  new CPolygon(11);
		HOCPoly4->Init(HOArrayPts4);
		_hiddenObjPolys.push_back(HOCPoly4);

		int HOArrayPts5[] = {791,213,767,221,737,205,745,199,742,186,755,180,754,165,765,167,770,156,783,162,792,149,801,160,812,158,818,168,832,168,834,182,845,186,837,194,849,199,826,218};
		CPolygon* HOCPoly5=  new CPolygon(20);
		HOCPoly5->Init(HOArrayPts5);
		_hiddenObjPolys.push_back(HOCPoly5);

		int HOArrayPts6[] = {1014,134,1020,126,1047,131,1081,160,1089,157,1069,130,1066,116,1074,110,1064,103,1061,108,1034,102,1025,86,1033,79,1006,55,999,72,1012,78,1003,82,1004,97,1011,94,1010,110,994,109,997,129};
		CPolygon* HOCPoly6=  new CPolygon(22);
		HOCPoly6->Init(HOArrayPts6);
		_hiddenObjPolys.push_back(HOCPoly6);

		int HOArrayPts7[] = {817,413,834,432,847,433,868,416,869,438,852,455,828,455,811,433};
		CPolygon* HOCPoly7=  new CPolygon(8);
		HOCPoly7->Init(HOArrayPts7);
		_hiddenObjPolys.push_back(HOCPoly7);

		int HOArrayPts8[] = {620,517,631,499,651,505,658,474,644,472,651,462,641,441,622,451,617,470,579,467,593,497,601,494,599,502};
		CPolygon* HOCPoly8=  new CPolygon(13);
		HOCPoly8->Init(HOArrayPts8);
		_hiddenObjPolys.push_back(HOCPoly8);

		int HOArrayPts9[] = {551,223,556,188,582,170,622,183,636,218,622,243,593,255,564,248};
		CPolygon* HOCPoly9=  new CPolygon(8);
		HOCPoly9->Init(HOArrayPts9);
		_hiddenObjPolys.push_back(HOCPoly9);
	}


}

L8_S5_VirtualMiniGame::L8_S5_VirtualMiniGame()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	bgScene = new CHOScene();
	bgScene->Load("LV8\\S5\\S5.SCN");
	for (int i = 0; i < 6; i++)
	{
		std::string papusId = std::string("PapusAnim_") + MKSTR(i);
		CObject* papus = bgScene->GetObjectByID(papusId);
		papus->SetScale(2);
		papus->SetFrame(rand() % 48);
	}
	bgScene->GetObjectByID("Hand")->Animate(2, 3, 0, 0, 4);
	bgScene->GetObjectByID("OpeningGlow")->Animate(2, 3, 0, 0, 4);
	bgScene->GetObjectByID("Mary")->Animate(0, 0, 5, 0, 15);

	LoadScene("LV8\\S5\\HOP1\\PUZZLE.SCN");

	HOScene->SetZOff(20);

	CHOScene* hiddenObjScene = new	CHOScene();
	hiddenObjScene->Load("LV8\\S5\\HOP1\\HO.SCN");
	hiddenObjScene->SetEnable(false);
	hiddenObjScene->SetZOff(50);

	hiddenObjScene->GetObjectByID("BlackBg")->SetScale(10.60f); 
	HOScene->GetObjectByID("BlackBg")->SetScale(10.60f); 

	_isGameCompleted = false;
	_controller = new GameController();
	_controller->CreateGame(HOScene, hiddenObjScene, Hud, CC_CALLBACK_0(L8_S5_VirtualMiniGame::OnGameComplete, this));
	
		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL8s5VolcanoLandAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL8s5VolcanoLandAmb) )		
	{
		Control::Audio->PlaySample(aAmbL8s5VolcanoLandAmb,true);
	}
	
	//MUSIC_SFX
	
	HOScene->GetObjectByID("Ray_1")->Animate(0.3f, 0.76f, 2, 90);
	HOScene->GetObjectByID("Ray_2")->Animate(0.3f, 0.75f, 2, 180);
	HOScene->GetObjectByID("Ray_3")->Animate(0.3f, 0.75f, 2, 270);
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S5_VirtualMiniGame]  = true;


	

}

void L8_S5_VirtualMiniGame::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL8theme) )
	{
		Control::Audio->Samples[aTrackL8theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL8theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL8theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL8puzzle) )
				  Control::Audio->PlaySample(aTrackL8puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL8puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL8puzzle, true);
		}
	}
}

void L8_S5_VirtualMiniGame::Update()
{
	NavigateToScene();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	Cursor::SetMode(CA_Normal);
	CPoint pos = Control::Input->Mpos();
	if(!_isGameCompleted)
		_controller->UpdateController(pos);

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (_controller->canGoBack() && Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S5_VolcanoLand;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	

	if( _controller->canGoBack() &&  IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L8_S5_VolcanoLand);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L8_S5_VolcanoLand);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S5_VolcanoLand, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S5_VolcanoLand;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
									
		}	
	}
}

void L8_S5_VirtualMiniGame::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		
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
			
		}
	}
	else if( val == INV_CLOSE )
	{		
	}
}

void L8_S5_VirtualMiniGame::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_controller->SkipPuzzle();
}

void L8_S5_VirtualMiniGame::ProcessNonHOHint()
{

	if (!_isGameCompleted)
	{
		
		_controller->ShowHint();
		return;
	}
	
	Hud->ShowNothingToDoMsgBanner();
}

L8_S5_VirtualMiniGame::~L8_S5_VirtualMiniGame()
{
	SAFE_DELETE(_controller);
	SAFE_DELETE(bgScene);
		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL8s5VolcanoLandAmb);
	Control::Audio->UnloadSample(aAmbL8s5VolcanoLandAmb);
	//MUSIC_SFX
}

void L8_S5_VirtualMiniGame::OnGameComplete()
{
	_isGameCompleted = true;
	Hud->InvBoxCtrl->ResetToInvBox();
	ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameCompleted = true;


	//Show Objective completed
	if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L8O1_FindWayToResurrectMary] && 
		!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L8O1_FindWayToResurrectMary] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L8O1_FindWayToResurrectMary] = true;
		//Silent Completion of Objective...
		//Hud->ShowObjectiveFX(eObj_L8O1_FindWayToResurrectMary,true);
	}

	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S5_VolcanoLand;
	SendMessage(ControlExit, ID, HOSM_NewArea);		
}
void L8_S5_VirtualMiniGame::NavigateToScene()
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