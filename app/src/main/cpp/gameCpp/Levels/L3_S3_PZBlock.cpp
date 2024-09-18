#include "L3_S3_PZBlock.h"
#include "Application.h"
#include "Util.h"
#include "Hud.h"
#include "ProfileSystem.h"
#include <array>

const CRectangle  TileRect(390.0f,49.0f,192.0f,193.0f);
const CRectangle  ShoeRect(551.0f,203.0f,236.0f,261.0f);
const CRectangle  MugRect(390.0f,49.0f,192.0f,193.0f);
const CRectangle  PuzzleRect(386.0f,47.0f,589.0f,583.0f);

const CPoint TileOrigin = CPoint(393, 49, 0);
const CPoint TileSize = CPoint(192, 192, 0);
const int NO_OF_COLS = 3;
const int NO_OF_ROWS = 3;
const float Puzzle_Skip_Time = 2;

bool IsPopup;

TileObject::TileObject(int gridIndex, bool isMovable):
					GridPuzzleObject(gridIndex, isMovable)
{
	_isSelected = false;
	_mouseOver = NULL;
	_selection = NULL;
	_animTimer = 0;

	_startPos = CPoint();
    _endpos = CPoint();
}

TileObject::~TileObject()
{
}

void TileObject::MoveTo(CPoint endPos)
{
	_selection->SetEnable(true);
	_mouseOver->SetEnable(false);

	_startPos = Pos;
	_endpos = endPos;

	Control::Audio->QuickLoadAndPlaySFX("s3pz1_tilemove");
}

void TileObject::CheckAndMove()
{
	int movableIndex[4] = {NO_OF_COLS, 1, -NO_OF_COLS, -1};
	
	for (int i = 0; i < 4; i++)
	{
		int nextIndex = _currentGridIndex + movableIndex[i];
			
		if (i == 1 && !(nextIndex%NO_OF_COLS))
			continue;

		if (i == 3 && !(_currentGridIndex%NO_OF_COLS))
			continue;

		if (nextIndex >= 0 && nextIndex < NO_OF_COLS * NO_OF_ROWS)
		{
			if (_delegate->isGridEmpty(nextIndex))
			{
				_isSelected = true;
				CPoint nextPos = _delegate->getPositionOfGrid(nextIndex);
				MoveTo(nextPos);
				_delegate->setGridEmpty(_currentGridIndex);
				setCurrentGridIndex(nextIndex);
				_delegate->onTileMove(_currentGridIndex);
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockTileIndex[_tileIndex] = _currentGridIndex;
				break;
			}
		}
	}
}

void TileObject::OnAnimationComplete()
{
	_isSelected = false;
	_selection->SetEnable(false);
	_mouseOver->Pos = _selection->Pos;
	_mouseOver->SetEnable(true);
	_delegate->onTileMoved();
}

void TileObject::SetIsInteractionEnabled(bool isInteractionEnabled)
{
	GridPuzzleObject::SetIsInteractionEnabled(isInteractionEnabled);
	_mouseOver->SetEnable(isInteractionEnabled);
}

void TileObject::setMouseOver(CObject* obj)
{
	_mouseOver = obj;
}

void TileObject::setMouseSelect(CObject *obj)
{
	_selection = obj;
	_selection->SetEnable(false);
}

void TileObject::Update(float ds)
{
	if(IsPopup)
		return;
	CObject::Update(ds);

	if (_isSelected)
		_selection->Pos = Pos;

	if (_endpos != _startPos)
	 {
		 _animTimer += Control::LogicRate * 2;

		 CPoint newPos;
		 newPos.x = _startPos.x + ((_endpos.x - _startPos.x) * easing(_animTimer, 0, 1, 1) );
		 newPos.y = _startPos.y + ((_endpos.y - _startPos.y) * easing(_animTimer, 0, 1, 1) );
		 SetPos(&newPos);

		 if (_animTimer >= 1)
		 {
			 _startPos = _endpos;
			 SetPos(&_endpos);
			 _animTimer = 0;
			 OnAnimationComplete();
		 }
	 }

}

void TileObject::OnMouseOver()
{
	if(IsPopup)
		return;
	CObject::OnMouseOver();
	_mouseOver->Pos = Pos;
}

void TileObject::OnMouseClick()
{
	if(IsPopup)
		return;

	CObject::OnMouseClick();

	if (_delegate->getTileMoving())
		 return;
	CheckAndMove();
}

void TileObject::OnMouseOut()
{
	CObject::OnMouseOut();
}

float TileObject::easing(float t,float b,float c,float d)
{
	t /= d;
	return (-c * t*(t-2.0f) + b);
}


S3_BlockPUzzle::S3_BlockPUzzle()
{
	_isPuzzleActive = false;
	_tileObjs.clear();
	_isTileMoving = false;
	_skipTimer = 0;
}

S3_BlockPUzzle::~S3_BlockPUzzle()
{
	_tileObjs.clear();
}

void S3_BlockPUzzle::InitPuzzle(vector<TileObject*> tiles)
{
	_gridOrigin = TileOrigin;
	_gridSize = TileSize;
	_numOfColumns = NO_OF_COLS;
	_numOfRows = NO_OF_ROWS;
	_isSolved = false;

	vector<int> puzzle(ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockTileIndex, 
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockTileIndex + sizeof ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockTileIndex / sizeof ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockTileIndex[0]);

	for (int i = 0; i < _numOfColumns * _numOfRows; i++)
	{
		Grid * grid = new Grid(i, true);
		_grids.push_back(grid);
	}

	_tileObjs = tiles;

	for (int i = 0; i < (int)(_tileObjs.size()); i++)
	{
		addObject(_tileObjs[i]);
		_tileObjs[i]->setCurrentGridIndex(puzzle[i]);
		_tileObjs[i]->Pos = getPositionOfGrid(puzzle[i]);
		_grids[puzzle[i]]->isEmpty = false;
	}

	_isTileMoving = false;
}

void S3_BlockPUzzle::ActivatePuzzle()
{
	_isPuzzleActive = true;

	for (int i = 0; i < (int)(_tileObjs.size()); i++)
	{
		_tileObjs[i]->SetIsInteractionEnabled(true);
	}
}

bool S3_BlockPUzzle::isGridEmpty(int gridIndex)
{
	return (_grids.at(gridIndex)->isEmpty);
}

bool S3_BlockPUzzle::canMoveToPosition(CPoint position)
{
	return GridPuzzle::canMoveToPosition(position);
}

CPoint S3_BlockPUzzle::getPositionOfGrid(int gridIndex)
{
	return GridPuzzle::getPositionOfGrid(gridIndex);
}

void S3_BlockPUzzle::setGridEmpty(int index)
{
	_grids[index]->isEmpty = true;
}

void S3_BlockPUzzle::onTileMove(int index)
{
	_grids[index]->isEmpty = false;
	_isTileMoving = true;
}

void S3_BlockPUzzle::onTileMoved()
{
	_isTileMoving = false;

	if (isSolved())
	{
		for (int i = 0; i < (int)(_tileObjs.size()); i++)
		{
			_tileObjs[i]->SetIsInteractionEnabled(false);
		}

		_isSolved = true;
	}
}

void S3_BlockPUzzle::Update(float ds)
{
	
	if(IsPopup)
		return;

	if (!_isPuzzleActive)
		return;

	if (_skipTimer > 0)
	{
		_skipTimer -= Control::LogicRate;
		if (_skipTimer <= 0)
		{
			SolvePuzzle();
		}
	}
}

void S3_BlockPUzzle::SkipPuzzle()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	for (int i = 0; i < (int)(_tileObjs.size()); i++)
	{
		_tileObjs[i]->FadeOut(0.8f);
	}

	_skipTimer = Puzzle_Skip_Time/2.0f;

	for (int i = 0; i < (int)(_tileObjs.size()); i++)
	{
		_tileObjs[i]->SetIsInteractionEnabled(false);
	}
}

void S3_BlockPUzzle::ForceSkipPuzzle()
{
	SolvePuzzle();
}

void S3_BlockPUzzle::SolvePuzzle()
{
	for (int i = 0; i < (int)(_tileObjs.size()); i++)
	{
		vector<Solution> solution = _tileObjs[i]->getArrayOfSolutions();
		int solutionIndex = solution[0].gridIndexForSolution;
		_tileObjs[i]->setCurrentGridIndex(solutionIndex);
		_tileObjs[i]->Pos = getPositionOfGrid(solutionIndex);
		_grids[solution[0].gridIndexForSolution]->isEmpty = false;
		_tileObjs[i]->SetIsInteractionEnabled(false);
		_tileObjs[i]->FadeIn(0.8f);
		ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockTileIndex[i] = solutionIndex; 
	}
}


L3_S3_PZBlock::L3_S3_PZBlock(void)
{

	LoadHud();
	LoadScene("LV3\\S3\\P2\\P2.SCN");
	musicFadeOutTimer = 0;
	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	_shoeMemoryVdo = new CVideo("OGV\\L3\\S3SHOEMEMORY.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	time = 0;
	subIndex = 0;
	subMax = 5; 
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory5"));

	//_shoeMemoryVdo->SetScale(1.334f);
	_shoeMemoryVdo->SetScale(GFApp->video_aspectRatio);
	_shoeMemoryVdo->SetSkipActive();

	s3BlockPZCompleteFX = new SuperFX("PRT\\L3\\S3BLOCKPZCOMPLETE.INI", PointSystem::CreateCPoint(681,338,0), 1100); 

	CObject* mouseOver = HOScene->GetObjectByID("mouseOver");
	CObject* tileSelection = HOScene->GetObjectByID("mouseSelect");
	_mugBoxDoor = HOScene->GetObjectByID("BackTile_open");
	_shoeObj = HOScene->GetObjectByID("shoe");

	_blockPuzzle = new S3_BlockPUzzle();

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

	vector<TileObject*> tiles;

	for (int i = 1; i <= 8; i++)
	{
		CObject *tileCObj = HOScene->GetObjectByID("Box_"+MKSTR(i));
		tileCObj->SetEnable(false);

		string spr = tileCObj->Filename;
		CPoint position = tileCObj->Pos;
		int zOrder = tileCObj->ZOrder;
		string uid = tileCObj->UID;

		TileObject* tile = new TileObject(i, true);
		tile->LoadInit(spr, position.x, position.y, zOrder);
		tile->setTileIndex(i - 1);
		tile->SetUID(uid);
		tile->addSolution(i);
		HOScene->PushObject(tile);
		tiles.push_back(tile);
		tile->setMouseOver(mouseOver);
		tile->setMouseSelect(tileSelection);
		tile->SetMouseInteractionArea(TileRect.w, TileRect.h);
		tile->SetIsInteractionEnabled(false);
		tile->setDelegate(_blockPuzzle);
	}

	_blockPuzzle->InitPuzzle(tiles);

	tiles.clear();
	_state = ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockState;
	_stateMachine.push(State_None);
	//MUSIC_SFX
	Control::Audio->LoadSample(aVOL3MaryRefS3_1,	AudioVO);
	Control::Audio->LoadSample(aAmbL3s3BlockPuzzle,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s3BlockPuzzle) )		
	{
		Control::Audio->PlaySample(aAmbL3s3BlockPuzzle,true);
	}
	
	//MUSIC_SFX
	LoadSavedState();
	_showHint = false;
}

void L3_S3_PZBlock::LoadSavedState()
{
	switch (_state)
	{
	case State_PlayingPuzzle:
		_shoeObj->SetEnable(false);
		_blockPuzzle->ActivatePuzzle();
		Hud->InvBox_StartPuzzleMode();
		break;

	case State_MugCollected:
		HOScene->GetObjectByID("glass_with_shadow")->SetEnable(false);
	case State_ReadyToCollectMug:
		_shoeObj->SetEnable(false);
		_mugBoxDoor->SetEnable(false);
		break;
	case State_PlayingCutScene:
		_shoeMemoryVdo->PlayVideo();
		Hud->EnableInput(false);
		Hud->HideHud(false);
		break;
	case State_WaitForShoe:
		Hud->ShowOrbGlowing();
		break;
	case State_CutSceneComplete:
	case State_PuzzleComplete:
	case State_None:
		break;
	case State_SkipPuzzle:
		_shoeObj->SetEnable(false);
		_blockPuzzle->ForceSkipPuzzle();
		break;
	default:
		break;
	}
}

L3_S3_PZBlock::~L3_S3_PZBlock(void)
{
	Hud->InvBox_StopPuzzleMode();
	SAFE_DELETE(s3BlockPZCompleteFX);
	SAFE_DELETE(_blockPuzzle);
	SAFE_DELETE(_shoeMemoryVdo);

	SAFE_DELETE(subtitleText);
	SAFE_DELETE(ResetDia);
	////MUSIC_SFX
	//Control::Audio->UnloadSample(aVOL3MaryRefS3_1);
	//Control::Audio->StopSample(aAmbL3s3BlockPuzzle);
	//Control::Audio->UnloadSample(aAmbL3s3BlockPuzzle);
	//MUSIC_SFX
}

void L3_S3_PZBlock::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MEMORY_OBJFX_REACHED ) 
	{
		_shoeMemoryVdo->PlayFadeInVideo(2);
		Hud->EnableInput(false);
		Hud->HideHud(false);
		_state = State_PlayingCutScene;
	}
	else if( val == INVBOX_DROP ) 
	{		
		
	}
	else if( val == INV_CLOSE )
	{		
		if (_state == State_MugCollected)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_Pool;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}
	}
}

void L3_S3_PZBlock::Render(const CPoint* pCam)
{
	
}

void L3_S3_PZBlock::Required()
{
	HiddenObjectSystem::Required();

	if(_state == State_PlayingCutScene)
		return;

	if( Control::Audio->IsPlaying(aTrackL3theme) )
	{
		Control::Audio->Samples[aTrackL3theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL3theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL3theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL3puzzle) )
				  Control::Audio->PlaySample(aTrackL3puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL3puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL3puzzle, true);
		}
	}
}

void L3_S3_PZBlock::Update()
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
	IsPopup = IsResetPop;
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	CPoint mPOs = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if (_state != _stateMachine.top())
	{
		_stateMachine.push(_state);
		OnStateChange();
	}

	switch (_state)
	{
	case State_None:
		_state = State_WaitForShoe;
		Hud->ShowBannerText(Banner_L3_S3_WhenSawSophiaShoe);
		Hud->ShowOrbGlowing();
		break;
	
	case State_WaitForShoe:
		{
			if (ShoeRect.Intersection(&mPOs))
			{
				Cursor::SetMode(CA_HandTake);

				if (Control::Input->LBclicked())
				{
					Cursor::SetMode(CA_Normal);
					Hud->HideOrbGlowing();

					Control::Audio->PlaySample(aSFXMagic3);
					Hud->SendOrbFXToObjectPos(*PointSystem::CreateCPoint(ShoeRect.GetCenterX(), ShoeRect.GetCenterY(),0));
				}
			}
		}
		break;

	case State_PlayingCutScene:
		{
			//Subtitles Code
			time = _shoeMemoryVdo->GetTime();

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
			vidTimer += Control::LogicRate;
#endif
			if (
#ifndef FORCE_CLOSE_VID_END
				_shoeMemoryVdo->isEndReached() ||
#endif
				vidTimer > 22)
			{
				subtitleText->SetText("");
				_shoeMemoryVdo->StopAndFadeOutVideo(2);
				_state = State_CutSceneComplete;
				Hud->AllowInput = true;
				Hud->ShowHud(false);
				//Journal Entry Add
				if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P5_CrystalVideoEnds])
				{
					//Flag Set
					ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P5_CrystalVideoEnds] = true;

					//Add to Note Array
					ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L3P5_CrystalVideoEnds;

					ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

					//Increment Notes
					ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
					Hud->ShowJrnEntryFX();
				}

				_shoeObj->FadeOut(1);
			}

			return;
		}
		break;

	case State_CutSceneComplete:
		{
			#ifdef _CEBUILD
				//collect memory object shoe
				if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S3Shoes])
					Hud->ShowMemoryObjectCollected_CE(eMem_L3S3Shoes, &_shoeObj->Pos);
			#else
				if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S3Shoes])
					Hud->ShowMemoryObjectCollected_SE(eMem_L3S3Shoes, &_shoeObj->Pos);
				//sophia nearby common banner felt more apt
				//Hud->ShowBannerText(eMem_L3S3Shoes);
			#endif 

			_blockPuzzle->ActivatePuzzle();
			Hud->InvBox_StartPuzzleMode();
			Hud->ShowBannerText(Banner_L3_S3_SophiaCutsceneComplete);
			Control::Audio->PlaySample(aVOL3MaryRefS3_1);
			_state = State_PlayingPuzzle;
		}
		break;

	case State_PlayingPuzzle:
		{
			if (_blockPuzzle->getIsSolved())
			{
				Control::Audio->QuickLoadAndPlaySFX("s3pz1_puzzlecomplete");
				_state = State_PuzzleComplete;
			}

			if (PuzzleRect.Intersection(&mPOs))
				return;
		}
		break;

	case State_SkipPuzzle:
		{
			_pzSkipTimer -= Control::LogicRate;
			_blockPuzzle->Update(1);
			if (_pzSkipTimer <= 0)
				_state = State_PuzzleComplete;
		}
		break;

	case State_PuzzleComplete:
		{
			AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			s3BlockPZCompleteFX->Start();

			if (_handleExtrasGameComplete())
			{
				_state = State_L3S3GameInExtrasComplete;
				return;
			}

			Hud->InvBox_StopPuzzleMode();
			_state++;
		}
		break;

	case State_OpenMugDoor:
		{
			Control::Audio->QuickLoadAndPlaySFX("s3p1_stonemove2");
			_mugBoxDoor->TweenPosTo(0, 193, 0, 2, TW_EASEBACKOUT, true);
			_state++;
		}
		break;

	case State_ReadyToCollectMug:
		{
			if (MugRect.Intersection(&mPOs))
			{
				Cursor::SetMode(CA_HandTake);

				if (Control::Input->LBclicked())
				{
					ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupTaken = true;
					HOScene->GetObjectByID("glass_with_shadow")->FadeOut(1);
					Hud->TakeInventoryItem(eInv_L3S3_CupEmpty);
					_state = State_MugCollected;
				}
			}
		}
		break;

	case State_MugCollected:
		break;
	
	case State_L3S3GameInExtrasComplete:
		break;

	default:
		_state++;
		break;
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_Pool;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif
	if( IsMouseOverGoBackArea(&mPOs) )
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L3_S3_Pool);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L3_S3_Pool);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L3_S3_Pool, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_Pool;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
}

void L3_S3_PZBlock::OnStateChange()
{
	ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockState = _state;
}

void L3_S3_PZBlock::ProcessNonHOHint()
{
	if (_state == State_WaitForShoe)
	{
		Hud->InitHintTrailEmit(&ShoeRect);
	}
	// else if(State_ReadyToCollectMug)
   else if(_state == State_ReadyToCollectMug) // Sac: Added _state == 
	{
		Hud->InitHintTrailEmit(&MugRect);
	}
}

void L3_S3_PZBlock::ProcessSkip()
{
	_blockPuzzle->SkipPuzzle();
	_pzSkipTimer = Puzzle_Skip_Time;
	_state = State_SkipPuzzle;
}


void L3_S3_PZBlock::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(_state < 3 || _state == State_PuzzleComplete)
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

			int puzzle[] =  {0, 7, 6, 8, 5, 2, 3, 1};
			for (int i = 0; i < 8; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockTileIndex[i] = puzzle[i];
			}
			ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockState = 4;
			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_PZBlocks;
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
void L3_S3_PZBlock::NavigateToScene()
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