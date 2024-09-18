#include "L3_S2_P1R2CreeperPuzzle.h"
#include "Rect.h"
#include "Util.h"
#include "AwardManager.h"

const int NO_OF_COLS = 4;
const int NO_OF_ROWS = 6;
const int DELAY = 1;

const CRectangle  TileRect(358.0f,323.0f,328.0f,232.0f);
const CRectangle  PuzzlePopupRect(359.0f,87.0f,650.0f,490.0f);

const CRectangle  ArrowUp(836.0f,105.0f,69.0f,48.0f);
const CRectangle  ArrowRight(898.0f,151.0f,45.0f,55.0f);
const CRectangle  ArrowDown(846.0f,212.0f,56.0f,46.0f);
const CRectangle  ArrowLeft(807.0f,151.0f,37.0f,57.0f);

const CRectangle ArrowRects[] = {ArrowRight, ArrowUp, ArrowLeft, ArrowDown};
const MoveTo UserInputs[] = {Move_Right, Move_Up, Move_Left, Move_Down};

const CRectangle  TeethRect(361.0f,102.0f,325.0f,223.0f);

const int InitialTileIndex[4] = {0, 1, 4, 5};
const int SolutionInxes[4] = {16, 17, 20, 21};
const int TeethIndex = 20;
 
Tile::Tile(int index, CRectangle tileRect)
{
	_index = index;
	_tileRect = tileRect;
	_blockObj = NULL;
	_neutralObj = NULL;
	_haveBlock = false;
	_haveTeeth = false;
}

Tile::~Tile()
{
	_blockObj = NULL;
	_neutralObj = NULL;
}

void Tile::setBlockObj(CObject* obj, int zorder)
{
	_blockObj = obj;

	if (_blockObj)
	{
		_haveBlock = true;
		_blockObj->Pos = Pos;
		_blockObj->SetZ(zorder);
		_blockObj->SetMaskRect(PuzzlePopupRect);
	}
}

void Tile::setNeutralObj(CObject* obj, int zOrder)
{
	_neutralObj = obj;

	if (_neutralObj)
	{
		_neutralObj->Pos = Pos;
		_neutralObj->SetZ(zOrder);
		_neutralObj->SetMaskRect(PuzzlePopupRect);
	}
}

void Tile::setTeethFinal(CObject* obj, int zorder)
{
	_teethObj = obj;

	if (_teethObj)
	{
		_haveTeeth = true;
		_teethObj->Pos = Pos;
		_teethObj->SetZ(zorder);
		_teethObj->SetMaskRect(PuzzlePopupRect);
	}
}

void Tile::Update(float ds)
{
	CObject::Update(ds);

	if (_blockObj)
	{
		_blockObj->Pos = Pos;
	}

	if (_neutralObj)
	{
		_neutralObj->Pos = Pos;
	}

	if (_teethObj)
	{
		_teethObj->Pos = Pos;
	}
}


void Tile::Move(MoveTo direction)
{
	switch (direction)
	{
	case Move_Down: 
		{
			float valArr[5] = {-1, Pos.x, Pos.y - _tileRect.h, -1, -1};
			TweenTo(valArr, 1.0, 0, true, CC_CALLBACK_1(Tile::TweenComplete, this));
		}
		break;

	case Move_Left:
		{
			float valArr[5] = {-1, Pos.x + _tileRect.w, Pos.y, -1, -1};
			TweenTo(valArr, 1.0, 0, true, CC_CALLBACK_1(Tile::TweenComplete, this));
		}
		break;

	case Move_Up:
		{
			float valArr[5] = {-1, Pos.x, Pos.y + _tileRect.h, -1, -1};
			TweenTo(valArr, 1.0, 0, true, CC_CALLBACK_1(Tile::TweenComplete, this));
		}
		break;

	case Move_Right:
		{
			float valArr[5] = {-1, Pos.x - _tileRect.w, Pos.y, -1, -1};
			TweenTo(valArr, 1.0, 0, true, CC_CALLBACK_1(Tile::TweenComplete, this));
		}
		break;

	default:
		break;
	}
}

void Tile::TweenComplete(CObject* obj)
{
	if (_interface)
		_interface->onTweenComplete();
}

void Tile::ShitTileWith(CPoint shiftOffset)
{
	float valArr[5] = {-1, Pos.x - shiftOffset.x, Pos.y - shiftOffset.y, -1, -1};
	TweenTo(valArr, 0.3f, 0, true, CC_CALLBACK_1(Tile::TweenComplete, this));
}

void Tile::SetEnable(bool enable)
{
	CObject::SetEnable(enable);
	
	if (_blockObj)
		_blockObj->SetEnable(enable);

	if (_neutralObj)
		_neutralObj->SetEnable(enable);

	if (_teethObj)
		_teethObj->SetEnable(enable);
}




PuzzleController::PuzzleController()
{
	_noOfCols = 0;
	_noOfRows = 0;
	_tilesToShow = 0;
	_tilesList.clear();
	_blockIndicateTimer = 0;
	_playerIndicate = NULL;
	_redBlockIndicate = NULL;
}

PuzzleController::~PuzzleController()
{
	_tilesList.clear();
	_redBlockIndicate = NULL;
	_playerIndicate = NULL;
}

void PuzzleController::InitPuzzleWith(std::vector<Tile*> tiles, int noCols, int noRows, int tilesToShow)
{
	_noOfCols = noCols;
	_noOfRows = noRows;
	_tilesToShow = tilesToShow;
	_tilesList = tiles;
	_pathEnd = false;
	_isTweening = false;
	_blockIndex = -1;
	_isGameOver = false;
	_currentTilesIndex.clear();
	
	_playerIndex = ProfileSystem::Instance->ActiveProfile.L3_S2_P1PlayerTile;

	for (int i = 0; i < _tilesToShow; i++)
	{
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_P1CurrentTiles[i] != -1)
			_currentTilesIndex.push_back(ProfileSystem::Instance->ActiveProfile.L3_S2_P1CurrentTiles[i]);
		else
			_currentTilesIndex.push_back(InitialTileIndex[i]);

		_tilesList[InitialTileIndex[i]]->SetEnable(true);
	}

	ShiftTilesToSavedPos();
	_playerIndicate->SetAlpha(0);
	MovePlayer(0.5f);
}

void PuzzleController::Move(MoveTo direction)
{
	if (_isTweening || _blockIndicateTimer)
		return;

	int shiftIndex = 0;
	int PlayerShift = 0;

	switch (direction)
	{
	case No_Move:
		break;
	case Move_Up:
		{
			for (int i = _tilesToShow - 1; i >= 0; i--)
			{
				int index = _currentTilesIndex[i];
				index += _noOfCols;

				if (index < (_noOfCols * _noOfRows) && i >= _tilesToShow/2)
				{
					shiftIndex = _noOfCols;
				}
			}

			if (_playerIndex + _noOfCols < (_noOfCols * _noOfRows))
			{
				PlayerShift = _noOfCols;
			}
		}
		break;

	case Move_Right:
		{
			for (int i = _tilesToShow - 1; i >= 0; i--)
			{
				int index = _currentTilesIndex[i];
				index += 1;

				if ((index % _noOfCols) != 0 && i%2)
				{
					shiftIndex = 1;
				}
			}

			if (((_playerIndex + 1) % _noOfCols) != 0)
			{
				PlayerShift = 1;
			}
		}
		break;

	case Move_Down:
		{
			for (int i = 0; i < _tilesToShow; i++)
			{
				int index = _currentTilesIndex[i];
				index -= _noOfCols;

				if (index >= 0 && i < _tilesToShow/2)
				{
					shiftIndex = -_noOfCols;
				}
			}

			if (_playerIndex - _noOfCols >= 0)
			{
				PlayerShift = -_noOfCols;
			}
		}
		break;

	case Move_Left:
		{
			for (int i = 0; i < _tilesToShow; i++)
			{
				int index = _currentTilesIndex[i];

				if ((index - 1) >= (index / _noOfCols) * _noOfCols && !(i%2))
				{
					shiftIndex = -1;
				}
			}

			if (_playerIndex - 1 >= (_playerIndex / _noOfCols) * _noOfCols)
			{
				PlayerShift = -1;
			}
		}
		break;

	default:
		break;
	}

	if (PlayerShift == 0)
		return;

	_playerIndex += PlayerShift;
	ProfileSystem::Instance->ActiveProfile.L3_S2_P1PlayerTile = _playerIndex;

	for (int i = 0; i < _tilesToShow; i++)
	{
		if (_playerIndex == _currentTilesIndex[i])
		{
			MovePlayer(0.5f);
			shiftIndex = 0;
		}
	}

	if (_tilesList[_playerIndex]->getHaveBlock())
	{
		_blockIndex = _playerIndex;
		_pathEnd = true;
	}

	if (_tilesList[_playerIndex]->getHaveTeeth())	
	{
		_isGameOver = true;
	}

	if (shiftIndex == 0)
		return;

	MovePlayer(1);

	for (int i = 0; i < _tilesToShow; i++)
	{
		_currentTilesIndex[i] += shiftIndex; 
		ProfileSystem::Instance->ActiveProfile.L3_S2_P1CurrentTiles[i] = _currentTilesIndex[i];
	}

	_isTweening = true;

	for (int i = 0; i < (int)(_tilesList.size()); i++)
	{
		_tilesList[i]->Move(direction);
	}
}

void PuzzleController::MovePlayer(float speed)
{
	float valArr[5] = {0, -1, -1, -1, -1};
	_playerIndicate->TweenTo(valArr, speed, 0, true, CC_CALLBACK_1(PuzzleController::PlayerTweenComplete, this));
	_isTweening = true;

	Control::Audio->QuickLoadAndPlaySFX("s2m2pz1_scroll");
}

void PuzzleController::Update()
{
	if (_blockIndicateTimer)
	{
		_blockIndicateTimer -= Control::LogicRate;

		if (_blockIndicateTimer <= 0)
		{
			Reset();
			_blockIndicateTimer = 0;
		}
	}
}

void PuzzleController::Reset()
{
	ShiftTiles();
	MovePlayer(0.5f);
	_redBlockIndicate->StopAnim();
	_redBlockIndicate->SetAlpha(0);
	ResetCurrentIndex();
	ProfileSystem::Instance->ActiveProfile.L3_S2_P1PlayerTile = _playerIndex = 0;
}

void PuzzleController::ResetCurrentIndex()
{
	_currentTilesIndex.clear();
	for (int i = 0; i < _tilesToShow; i++)
	{
		_currentTilesIndex.push_back(InitialTileIndex[i]);
		ProfileSystem::Instance->ActiveProfile.L3_S2_P1CurrentTiles[i] = InitialTileIndex[i];
	}
}

void PuzzleController::ShiftTilesToSavedPos()
{
	_isTweening = true;

	int shiftPosX = ((int)(TileRect.w) * (_currentTilesIndex[0] % _noOfCols));
	int shiftPosY = -((int)(TileRect.h) * (_currentTilesIndex[0] / _noOfCols));

	for (int i = 0; i < (int)(_tilesList.size()); i++)
	{
		_tilesList[i]->ShitTileWith(CPoint((float)(shiftPosX), (float)(shiftPosY), 0));
	}

	if (_tilesList[_playerIndex]->getHaveTeeth())
	{
		_isGameOver = true;
	}

	for (int i = 0; i < _tilesToShow; i++)
	{
		_tilesList[_currentTilesIndex[i]]->SetEnable(true);
	}
}

void PuzzleController::ShiftTiles()
{
	_isTweening = true;

	int shiftPosX = -((int)(TileRect.w) * (_currentTilesIndex[0] % _noOfCols));
	int shiftPosY = ((int)(TileRect.h) * (_currentTilesIndex[0] / _noOfCols));

	for (int i = 0; i < (int)(_tilesList.size()); i++)
	{
		_tilesList[i]->ShitTileWith(CPoint((float)(shiftPosX), (float)(shiftPosY), 0));
	}
}

void PuzzleController::onTweenComplete(CObject *obj)
{
	_isTweening = false;

	if (_pathEnd)
	{
		_pathEnd = false;
		float valArr[5] = {0, -1, -1, -1, -1};
		_playerIndicate->TweenTo(valArr, 0.4f, 0, true, nullptr);
		_redBlockIndicate->Pos = _tilesList[_blockIndex]->Pos;
		_redBlockIndicate->Animate(0, 1, 0.5f);
		_redBlockIndicate->SetBlendMode(eBlend_AlphaAdditive);
		_blockIndicateTimer = DELAY;

		Control::Audio->QuickLoadAndPlaySFX("s2m2pz1_wrongtile");
	}
	else
	{
		Control::Audio->QuickLoadAndPlaySFX("s2m2pz1_righttile");
	}

	if (_isGameOver)
	{
		_playerIndicate->SetEnable(false);
		Control::Audio->QuickLoadAndPlaySFX("s2m2pz1_puzzlecomplete");
	}
}

void PuzzleController::PlayerTweenComplete(CObject *obj)
{
	_playerIndicate->SetPos(&_tilesList[_playerIndex]->Pos);
	CPoint pos = _playerIndicate->Pos;
	float valArr[5] = {1, pos.x, pos.y, -1, -1};
	_playerIndicate->TweenTo(valArr, 1, 0, true, CC_CALLBACK_1(PuzzleController::onTweenComplete, this));
}

void PuzzleController::Skip()
{
	_isTweening = true;

	int shiftPosX = -((int)(TileRect.w) * (_currentTilesIndex[0] % _noOfCols));
	int shiftPosY = ((int)(TileRect.h) * (_currentTilesIndex[0] / _noOfCols));
	shiftPosY -= (int)(TileRect.h) * _noOfCols;

	for (int i = 0; i < (int)(_tilesList.size()); i++)
	{
		_tilesList[i]->ShitTileWith(CPoint((float)(shiftPosX), (float)(shiftPosY), 0));
	}

	_playerIndex = ProfileSystem::Instance->ActiveProfile.L3_S2_P1PlayerTile = TeethIndex;

	for (int i = 0; i < _tilesToShow; i++)
	{
		_currentTilesIndex[i] = ProfileSystem::Instance->ActiveProfile.L3_S2_P1CurrentTiles[i] = SolutionInxes[i];
	}
	_redBlockIndicate->Show(false);
	_isGameOver = true;
}

void PuzzleController::PuzzleStart()
{
	for (int i = 0; i < (int)_tilesList.size(); i++)
	{
		_tilesList[i]->SetEnable(true);
	}
}

void PuzzleController::PuzzlePaused()
{
	for (int i = 0; i < (int)_tilesList.size(); i++)
	{
		_tilesList[i]->SetEnable(false);
	}

	for (int i = 0; i < _tilesToShow; i++)
	{
		_tilesList[_currentTilesIndex[i]]->SetEnable(true);
	}
}



L3_S2_P1R2CreeperPuzzle::L3_S2_P1R2CreeperPuzzle(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback)
						:Popup(popupId, hud, scene, zOrder, zoomOrigin, popupCloseCallback)
{

	Load("LV3\\S2\\MAZE2\\P1\\P1.SCN");
	SetFrameAndCloseButton();
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(PuzzlePopupRect);

	for (int i = 0; i < (int)(Objects.size()); i++)
	{
		if (Objects[i]->UID.find("Arrow_") != std::string::npos)
		{
			_arrows.push_back(Objects[i]);
		}
	}

	CObject* firstBox = GetObjectByID("box_0");
	CPoint initPos = firstBox->Pos;
	firstBox->SetEnable(false);

	std::vector<Tile*> tilesList;
	int totalTiles = NO_OF_COLS * NO_OF_ROWS;
	float tileX = initPos.x, xOffset = 0;
	float tileY = initPos.y, yOffset = 0;

	_controller = new PuzzleController();

	for (int i = 0; i < totalTiles; i++)
	{
		 xOffset = TileRect.w * (i%NO_OF_COLS); 
		 yOffset = TileRect.h * (i/NO_OF_COLS);

		Tile *tile = new Tile(i, TileRect);
		tile->LoadInit(std::string("LV3\\S2\\MAZE2\\P1\\box_")+MKSTR(i)+".SPR", tileX + xOffset, tileY - yOffset, zOrder+1);
		tile->setBlockObj(GetObjectByID("Block_"+MKSTR(i)), ZOrder+2);
		tile->setTeethFinal(GetObjectByID("teeth_"+MKSTR(i)), ZOrder+2);
		tile->setNeutralObj(GetObjectByID("Neutral_"+MKSTR(i)), ZOrder+2);
		tile->SetMaskRect(PuzzlePopupRect);
		tile->setInterface(_controller);
		tile->Show(false);
		tile->SetEnable(false);
		tilesList.push_back(tile);
		InsertObject(tile, 1);
	}

	CObject *redBlockIndicate = GetObjectByID("RedBlockIndicate");
	redBlockIndicate->SetScale(2);
	redBlockIndicate->SetAlpha(0);

	CObject *PlayerIndicate = GetObjectByID("PlayerPosIndicate");
	PlayerIndicate->SetScale(2);

	_controller->setRedBlockIndicator(redBlockIndicate);
	_controller->setPlayerIndicator(PlayerIndicate);
	_controller->InitPuzzleWith(tilesList, NO_OF_COLS, NO_OF_ROWS, 4);
	tilesList.clear();

	_isgameOver = false;
	_userInput = No_Move;
}


L3_S2_P1R2CreeperPuzzle::~L3_S2_P1R2CreeperPuzzle(void)
{
	SAFE_DELETE(_controller);
}

void L3_S2_P1R2CreeperPuzzle::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint mPos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (_isgameOver)
	{
		if (TeethRect.Intersection(&mPos))
		{
			Cursor::SetMode(CA_HandTake);
			if (Control::Input->LBclicked())
			{
				Hud->TakeInventoryItem(eInv_L3S2_SkullTeeth);
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_FortInterior] = true;
				GetObjectByID("teeth_20")->FadeOut(0.5f);
				ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken = true;
			}
		}
	}
	else
	{
		_controller->Update();
		_userInput = No_Move;

		for (int i = 0; i < (int)(_arrows.size()); i++)
		{
			_arrows[i]->SetAnimation(0);

			if (ArrowRects[i].Intersection(&mPos))
			{
				_arrows[i]->SetAnimation(1);
				Cursor::SetMode(CA_HandPointy);
				_userInput = UserInputs[i];
			}
		}

		if (_userInput != No_Move && Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s2m2pz1_arrowclick");

			_controller->Move(_userInput);
		}

		if (_controller->getGameOver())
		{
			AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			_isgameOver = true;
			Hud->InvBox_StopPuzzleMode();
			Hud->ShowBannerText(Banner_L3_S2_M2_CreeperPzComplete);

			for (int i = 0; i < (int)(_arrows.size()); i++)
			{
				_arrows[i]->Show(false);
			}
		}
	}
}

void L3_S2_P1R2CreeperPuzzle::OnPopupOpened()
{
	Popup::OnPopupOpened();
	Hud->InvBox_StartPuzzleMode();
	_controller->PuzzleStart();

	if (_controller->getGameOver())
	{
		_isgameOver = true;
		Hud->InvBox_StopPuzzleMode();

		for (int i = 0; i < (int)(_arrows.size()); i++)
		{
			_arrows[i]->Show(false);
		}
	}
}

void L3_S2_P1R2CreeperPuzzle::OnPopupCloseBtnClicked()
{
	Popup::OnPopupCloseBtnClicked();
	Hud->InvBox_StopPuzzleMode();
	_controller->PuzzlePaused();
}

void L3_S2_P1R2CreeperPuzzle::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_controller->Skip();
}

void L3_S2_P1R2CreeperPuzzle::ProcessHint()
{
	Hud->InitHintTrailEmit(&TeethRect);
}


