
#include "L3_S2_P1_MazePipePopup.h"
#include "AwardManager.h"

static int ANGLE_TO_ROTATE = 90;
static int Number_Of_Cols = 5;
static int Number_Of_rows = 7;
static CPoint GridOrigin = CPoint(582, 88, 0);
static CPoint GridSize = CPoint(67, 69, 0);
const CRectangle  PuzzleRect(581.0f,95.0f,345.0f,471.0f);
const CRectangle  PopupRect(399.0f,122.0f,576.0f,422.0f);
#define MAX_OUTERGRIDS 5
static int EmptyOuterGrids[MAX_OUTERGRIDS] = {3, 4, 5, 9, 30};


/*---------------------MazePipeObjet--------------------*/

float Find_Pos_Distance(CPoint p1, CPoint p2)
{
	float tmpX = p2.x - p1.x;
	tmpX *= tmpX;
	float tmpY = p2.y - p1.y;
	tmpY *= tmpY;
	return sqrt(tmpX + tmpY);
}

MazePipeObjet::MazePipeObjet()
{
	_delegate = NULL;
	_currentPos = CPoint();
	_nextPos = CPoint();
	_endpos = CPoint();
	_startPos = CPoint();
	_animTimer = 0;
	_rotateTimer = 0;
	_startAngle = _endAngle = 0;
	_mouseState = Mouse_Default;
	_pipeIndex = 0;
	_pipeMotionAnimTime = 1;
}

MazePipeObjet::~MazePipeObjet()
{
	_delegate = NULL;
}

void MazePipeObjet::LoadInit(const std::string& sprPath, float x, float y, int zOrder, bool enable)
{
	CObject::LoadInit(sprPath, x, y, zOrder, enable);
	_currentPos = CPoint(x, y, 0);
	_originPos = _currentPos;
	_originalzOrder = ZOrder;
}


void MazePipeObjet::setCurrentGridIndex(int index)
{
	GridPuzzleObject::setCurrentGridIndex(index);
	if( _pipeIndex >= 0 && _pipeIndex < MAX_PIPE_INDEX )
		ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeGridIndex[_pipeIndex] = index;

	if (index == -1)
		return;

	_currentPos = _delegate->getPosition(index);
	_nextPos = _currentPos;
	MoveTo(_currentPos);
}

 void MazePipeObjet::Update(float ds)
 {
	 CObject::Update(ds);

	 _mousePos = Control::Input->Mpos();

	 switch (_mouseState)
	 {
	 case Mouse_Default:
		 break;

	 case Mouse_Down:
		 {
			 if (Find_Pos_Distance(_mouseDownPos, _mousePos) > 10)
			 {
				 SetZ(_zOrderTop);
				 _mouseState = Mouse_Dragging;
			 }
		 }
		 break;

	 case Mouse_Dragging:
		 {
			 _nextPos = _mousePos;
			 
			 if (!PopupRect.Intersection(&_mousePos))
			 {
				 if(_mousePos.x < PopupRect.topleft.x)
					 _nextPos.x = PopupRect.topleft.x;

				 if(_mousePos.y < PopupRect.topleft.y)
					 _nextPos.y = PopupRect.topleft.y;

				 if(_mousePos.x > (PopupRect.topleft.x + PopupRect.w))
					 _nextPos.x = PopupRect.topleft.x +  + PopupRect.w;

				 if (_mousePos.y > (PopupRect.topleft.y + PopupRect.h))
					_nextPos.y = PopupRect.topleft.y +  + PopupRect.h;
			 }

			 SetPos(&_nextPos);

			 if (!Control::Input->LBdown())
			 {
				 _mouseState = Mouse_Up;
			 }
		 }
		 break;

	 case Mouse_Up:
		 {
			 _nextPos = _mousePos;
			 SetZ(_originalzOrder);
			 OnMouseUp();
			 _mouseState = Mouse_Default;
		 }
		 break;

	 case Mouse_Click:
		 {
			 int angle = (int)(GetRotate()) + ANGLE_TO_ROTATE;
			 RotateTo(angle);
			 _mouseState = Mouse_Default;
		 }
		 break;

	 default:
		 break;
	 }

	 if (_endpos != _startPos)
	 {
		 _animTimer += Control::LogicRate;

		 CPoint newPos;
		 newPos.x = _startPos.x + ((_endpos.x - _startPos.x) * easing(_animTimer, 0, 1, _pipeMotionAnimTime) );
		 newPos.y = _startPos.y + ((_endpos.y - _startPos.y) * easing(_animTimer, 0, 1, _pipeMotionAnimTime) );
		 SetPos(&newPos);

		 if (_animTimer >= _pipeMotionAnimTime)
		 {
			 _startPos = _endpos;
			 SetPos(&_endpos);
			 _animTimer = 0;
			 OnAnimationComplete();
			 _pipeMotionAnimTime = 1;
		 }
	 }

	 if (_startAngle != _endAngle)
	 {
		 _rotateTimer += Control::LogicRate * 2;

		 float angle = _startAngle + (_endAngle - _startAngle)*easing(_rotateTimer, 0, 1, 1);
		 SetRotate(angle);

		 if (_rotateTimer >= 1)
		 {
			 _endAngle = (_endAngle >= 360 ? 0 : _endAngle);
			 if( _pipeIndex >= 0 && _pipeIndex < MAX_PIPE_INDEX )
				ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeAngles[_pipeIndex] = _endAngle;
			 _startAngle = _endAngle;
			 SetRotate((float)(_endAngle));
			 _rotateTimer = 0;
			 OnAnimationComplete();
		 }
	 }
 }

 void MazePipeObjet::OnMouseOver()
 {
	 CObject::OnMouseOver();
	 Cursor::SetMode(CA_HandPointy);
 }

 void MazePipeObjet::OnMouseClick()
 {
	 CObject::OnMouseClick();
	 SetScale(1.0f);

	 if (_mouseState == Mouse_Dragging)
	 {
		_mouseState = Mouse_Up;
		return;
	 }

	 _mouseState = Mouse_Click;
 }

 void MazePipeObjet::OnMouseDown()
 {
	 CObject::OnMouseDown();
	 _mouseState = Mouse_Down;
	 _mouseDownPos = _mousePos;
	 SetScale(1.05f);

	 if (!PopupRect.Intersection(&_mousePos))
		return;

	 if (getCurrentGridIndex() != -1)
	 {
		_delegate->onPlayerMoveStart(getCurrentGridIndex());		
	 }
 }

 void MazePipeObjet::OnMouseUp()
 {
	 CObject::OnMouseUp();
	 _mouseState = Mouse_Up;
	 SetScale(1.0f);

	 if (!PuzzleRect.Intersection(&_mousePos))
	 {
		 MoveTo(_originPos);
		 _currentPos = _originPos;

		 if (getCurrentGridIndex() != -1)
			_delegate->setGridToEmpty(getCurrentGridIndex());

		 setCurrentGridIndex(-1);
		 return;
	 }
	
	 if (_delegate->canMoveToPoint(_nextPos))
	 {
		 if (getCurrentGridIndex() != -1)
			_delegate->setGridToEmpty(getCurrentGridIndex());

		 setCurrentGridIndex(_delegate->getGridIndex(_nextPos));	
	 }
	 else
	 {
		 _nextPos = _currentPos;
		 MoveTo(_currentPos);
	 }
 }

 void MazePipeObjet::OnMouseOut()
 {
	 CObject::OnMouseOut();
	 Cursor::SetMode(CA_Normal);
 }

 void MazePipeObjet::MoveTo(CPoint endPos)
 {
	 if (_animTimer != 0)
		 return;

	 _startPos = Pos;
	 _endpos = endPos;

	 float distance = Elixir::Distance(&_startPos, &_endpos);
	 distance = distance > 800.0f ? 800 : distance;
	 _pipeMotionAnimTime = Elixir::Lerp(0.15f, 0.45f, distance/800.0f);

	 Control::Audio->QuickLoadAndPlaySFX("s2m5pz1_pipesplaced");
 }

 void MazePipeObjet::RotateTo(int angle)
 {
	 if (_rotateTimer != 0)
		 return;

	 _startAngle = (int)(GetRotate());
	 _endAngle = angle;

	Control::Audio->QuickLoadAndPlaySFX("s2m5pz1_piperotate");
 }

 void MazePipeObjet::OnAnimationComplete()
 {
	 if (getCurrentGridIndex() != -1)
		_delegate->onPlayerMovEnd(getCurrentGridIndex());
 }

/*---------------------MazePipePuzzle--------------------*/

MazePipePuzzle::MazePipePuzzle()
{
	 _initialPos[6] = CPoint(598, 533, 0);
     _initialPos[7] = CPoint(476-47, 230, 0);
     _initialPos[10] = CPoint(911+40, 215+20, 0); 
     _initialPos[13] = CPoint(504, 498, 0);
     _initialPos[16] = CPoint(505, 157, 0);
     _initialPos[22] = CPoint(513, 288, 0);
     _initialPos[25] = CPoint(880, 152, 0);
     _initialPos[33] = CPoint(558-144, 218+121, 0);
	 
	 InitSolution();
	 _isSolved = false;
}

void MazePipePuzzle::InitSolution()
{
	{
		PipeSolution pipe;
		pipe.Add(6, 0);
		pipe.Add(6, 180);
		pipe.Add(25, 90);
		pipe.Add(25, 270);
		pipe.Add(33, 90);
		pipe.Add(33, 270);
		Solution.push_back(pipe);
	}
	{
		PipeSolution pipe;
		pipe.Add(7, 0);
		pipe.Add(10, 270);
		Solution.push_back(pipe);
	}
	{
		PipeSolution pipe;
		pipe.Add(10, 0);
		pipe.Add(7, 90);
		Solution.push_back(pipe);
	}
	{
		PipeSolution pipe;
		pipe.Add(13, 0);
		pipe.Add(22, 0);
		Solution.push_back(pipe);
	}
	{
		PipeSolution pipe;
		pipe.Add(16, 0);
		pipe.Add(16, 90);
		pipe.Add(16, 180);
		pipe.Add(16, 270);
		Solution.push_back(pipe);
	}
	{
		PipeSolution pipe;
		pipe.Add(22, 0);
		pipe.Add(13, 0);
		Solution.push_back(pipe);
	}
	{
		PipeSolution pipe;
		pipe.Add(25, 0);
		pipe.Add(25, 180);
		pipe.Add(33, 0);
		pipe.Add(33, 180);
		pipe.Add(6, 90);
		pipe.Add(6, 270);
		Solution.push_back(pipe);
	}
	{
		PipeSolution pipe;
		pipe.Add(33, 0);
		pipe.Add(25, 0);
		pipe.Add(6, 90);
		pipe.Add(33, 180);
		pipe.Add(25, 180);
		pipe.Add(6, 270);
		Solution.push_back(pipe);
	}

	{
		PipeSolution pipe;
		pipe.Add(6, 0);
		SkipSolution.push_back(pipe);
	}

	{
		PipeSolution pipe;
		pipe.Add(7, 0);
		SkipSolution.push_back(pipe);
	}

	{
		PipeSolution pipe;
		pipe.Add(10, 0);
		SkipSolution.push_back(pipe);
	}

	{
		PipeSolution pipe;
		pipe.Add(13, 0);
		SkipSolution.push_back(pipe);
	}

	{
		PipeSolution pipe;
		pipe.Add(16, 0);
		SkipSolution.push_back(pipe);
	}

	{
		PipeSolution pipe;
		pipe.Add(22, 0);
		SkipSolution.push_back(pipe);
	}

	{
		PipeSolution pipe;
		pipe.Add(25, 0);
		SkipSolution.push_back(pipe);
	}

	{
		PipeSolution pipe;
		pipe.Add(33, 0);
		SkipSolution.push_back(pipe);
	}
}

MazePipePuzzle::~MazePipePuzzle()
{
	_mazePipeObjets.clear();
	_pipes.clear();
	_greenPipes.clear();
}

void MazePipePuzzle::InitPuzzleWith(vector<CObject*> pipes, CHOScene *scene)
{
	_gridOrigin = GridOrigin;
	_gridSize = GridSize;
	_numOfColumns = Number_Of_Cols;
	_numOfRows = Number_Of_rows;
	_pipes = pipes;

	for (int i = 0; i < (_numOfRows * _numOfColumns); i++)
	{
		Grid * grid = new Grid(i, false);
		_grids.push_back(grid);
		if( i < MAX_OUTERGRIDS)
		{
			if (i == EmptyOuterGrids[i])
			{
				grid->isEmpty = true;
			}
		}
	}

	for (int i = 0; i < (int)(_pipes.size()); i++)
	{
		string uidStr = _pipes[i]->UID;
		std::size_t pos = uidStr.find("_I_");
		uidStr = uidStr.substr (pos);  
		uidStr.replace(0,3,"");
		int gridIndex = atoi(uidStr.c_str());

		InitPosition(_pipes[i], gridIndex);
		_grids[gridIndex]->isEmpty = true;
	}

	int zorderTop = (*_pipes.rbegin())->ZOrder;
	zorderTop += 1;

	vector<int> pipeIndex(ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeGridIndex, ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeGridIndex + (sizeof ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeGridIndex / sizeof ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeGridIndex[0]));
	vector<int> pipeAngles(ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeAngles, ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeAngles + (sizeof ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeAngles / sizeof ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeAngles[0])); 
	
	for (int i = 0; i < (int)(_pipes.size()); i++)
	{
		string spr = _pipes[i]->Filename;
		CPoint position = _pipes[i]->Pos;
		int zOrder = _pipes[i]->ZOrder;
		string uid = _pipes[i]->UID;

		PipeSolution pipe = Solution[i];

		MazePipeObjet *obj = new MazePipeObjet();
		obj->LoadInit(spr, position.x, position.y, zOrder);
		obj->SetUID(uid);
		obj->Show(false);
		obj->setPipeIndex(i);
		obj->setDelegate(this);
		obj->setZOrderTop(zorderTop);
		obj->setCurrentGridIndex(pipeIndex[i]);
		obj->SetRotate((float)(pipeAngles[i]));
		obj->setIsMovable(true);
		pipe.SetSolutions(obj);
		addObject(obj);
		scene->PushObject(obj);
		_mazePipeObjets.push_back(obj);

		if (uid.find("green") != string::npos)
		{
			_greenPipes.push_back(obj);
			obj->SetEnable(false);
		}
	}
}

void MazePipePuzzle::InitPosition(CObject *pipe, int gridIndex)
{
	_solutionPos[gridIndex] = pipe->Pos;
	pipe->SetPos(&_initialPos[gridIndex]);
}

void MazePipePuzzle::ActivatePuzzle()
{
	for (int i = 0; i < (int)(_greenPipes.size()); i++)
	{
		_greenPipes[i]->SetEnable(true);
	}

	for (int i = 0; i < (int)(_mazePipeObjets.size()); i++)
	{
		_mazePipeObjets[i]->SetMouseInteractionArea(GridSize.x, GridSize.y);
	}
}

void MazePipePuzzle::Update(float ds)
{

}

void MazePipePuzzle::SkipPuzzle()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	for (int i = 0; i < (int)(_mazePipeObjets.size()); i++)
	{
		PipeSolution pipe = SkipSolution[i];
		
		map<int, vector<int> > solutionIndexAngle = pipe._gridIndexAndAngle;
		map<int, vector<int> >::iterator mapit = solutionIndexAngle.begin();
		CPoint position = getPosition(mapit->first);

		_mazePipeObjets[i]->setCurrentGridIndex(mapit->first);
		_mazePipeObjets[i]->MoveTo(position);
		_mazePipeObjets[i]->RotateTo(mapit->second[0]);
	}
}

bool MazePipePuzzle::canMoveToPoint(CPoint pos)
{
	return canMoveToPosition(pos);
}

void MazePipePuzzle::setGridToEmpty(int index)
{
	_grids[index]->isEmpty = true;
}

CPoint MazePipePuzzle::getPosition(int index)
{
	return _solutionPos[index];
//	return getPositionOfGrid(index);
}

int MazePipePuzzle::getGridIndex(CPoint pos)
{
	return getGridIndexForPosition(pos);
}

void MazePipePuzzle::onPlayerMoveStart(int index)
{
	
}

void MazePipePuzzle::onPlayerMovEnd(int index)
{
	if (isSolved())
	{
		OnPuzzleSolved();     
	}
	else
	{
		if( _grids[index] != NULL )
			_grids[index]->isEmpty = false;
	}
}

void MazePipePuzzle::OnPuzzleSolved()
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	for (int i = 0; i < (int)(_mazePipeObjets.size()); i++)
	{
		_mazePipeObjets[i]->SetIsInteractionEnabled(false);
	}

	ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipePuzzleCompleted = true;
	_isSolved = true;
}


/*---------------------L3_S2_P1_MazePipePopup--------------------*/

L3_S2_P1_MazePipePopup::L3_S2_P1_MazePipePopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) 
	: Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	_isPuzzleActive = false;
	_isPuzzleSolved = false;
	_puzzleState = Puzzle_Inactive;

	_pipePuzzle = NULL;
	Load("LV3\\S2\\MAZE5\\MAZE5PIPE\\MZ5PIPEPUZZLE.SCN");
	SetFrameAndCloseButton();
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(GetObjectByID("base"));

	vector<CObject*> pipesList;
	pipesList.clear();

	for (int i = 0; i < (int)(Objects.size()); i++)
	{
		if (Objects[i]->UID.find("pipe") != string::npos)
		{
			Objects[i]->SetEnable(false);		
			pipesList.push_back(Objects[i]);	
		}
	}

	_pipePuzzle = new MazePipePuzzle();
	_pipePuzzle->InitPuzzleWith(pipesList, this);
	pipesList.clear();

	SetReadyForPlay(ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipePuzzleActivated);
}

L3_S2_P1_MazePipePopup::~L3_S2_P1_MazePipePopup()
{
	SAFE_DELETE(_pipePuzzle);
	if(!_isPuzzleSolved)
		Hud->InvBox_StopPuzzleMode();
}

void L3_S2_P1_MazePipePopup::SetReadyForPlay(bool ready)
{
	ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipePuzzleActivated = ready;
	_isPuzzleActive = ready;

	if (ready)
	{
		_pipePuzzle->ActivatePuzzle();
		_puzzleState = Puzzle_Active;
	}
}

void L3_S2_P1_MazePipePopup::Required()
{

}

float L3_S2_P1_MazePipePopup::easing(float t,float b,float c,float d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
}

void L3_S2_P1_MazePipePopup::OnPopupOpened()
{
	Popup::OnPopupOpened();

	if (_isPuzzleActive)
		Hud->InvBox_StartPuzzleMode();
}

void L3_S2_P1_MazePipePopup::OnPopupCloseBtnClicked()
{
	Popup::OnPopupCloseBtnClicked();
	
	if (_isPuzzleActive)
		Hud->InvBox_StopPuzzleMode();
}

void L3_S2_P1_MazePipePopup::ProcessHint()
{
	Hud->ShowHintForInvItem(eInv_L3S2_PipePiece1Room1, &PuzzleRect);
}

void L3_S2_P1_MazePipePopup::ProcessSkip()
{
	_pipePuzzle->SkipPuzzle();
}

void L3_S2_P1_MazePipePopup::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (itemVal == eInv_L3S2_PipePiece1Room1)
	{
		if (PopupRect.Intersection(&dropPos))
		{
			if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken &&
				ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken &&
				ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken &&
				ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken )
			{
				SetReadyForPlay(true);

				Hud->ReleaseDrag();
				Hud->CloseInventory();

				//Clear Inventory. One Time Use Items.
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_PipePiece1Room1] = false;
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_PipePiece2Room2] = false;
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_PipePiece3Room3] = false;
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_PipePiece4Room4] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_PipePiece1Room1);
				
				Hud->InvBox_StartPuzzleMode();
				return;
			}
			else
			{
				Hud->ShowBannerText(Banner_L3_S2_M5_DropPartualPipes);
				Hud->CancelDrag();
				return;
			}
		}
	}

	Popup::HandleInventoryDrop(dropPos, itemVal);
}

void L3_S2_P1_MazePipePopup::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState() != ePopupState_Open )
		return;

	if( Hud->PopupState != Popup_Inactive )
		return;

#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;			

	_pipePuzzle->Update(ds);

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint mousePos = Control::Input->Mpos();

	switch (_puzzleState)
	{
	case Puzzle_Inactive:
		{
			if (PopupRect.Intersection(&mousePos))
			{
				Cursor::SetMode(CA_Gear);
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L3_S2_M5_ClickOnPopupWithoutPipe);
				}
			}
		}
		break;

	case Puzzle_Active:
		{
			_puzzleState = Puzzle_Playing;
		}
		break;

	case Puzzle_Playing:
		{
			if (_pipePuzzle->getIsSolved())
			{
				_puzzleState++;
			}

			if (Control::Input->LBclicked() && !Hud->IsDraggingItem() && mousePos.y < 600)
			{
				Hud->ShowBannerText(Banner_L3_S2_M5_ClickOnPopupWithPipe);
			}
		}
		break;

	case Puzzle_Solved:
		{
			Control::Audio->QuickLoadAndPlaySFX("s2m5pz1_puzzlecomplete");
			_isPuzzleSolved = true;
			ClosePopup();
			_puzzleState = Puzzle_Complete;
		}
		break;

	case Puzzle_Complete:
		{

		}
		break;

	default:
		_puzzleState++;
		break;
	}
}


