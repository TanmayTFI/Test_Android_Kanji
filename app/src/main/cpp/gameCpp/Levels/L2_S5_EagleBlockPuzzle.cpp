//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 05 : MOOPAANS DEN
//====================================

#include "L2_S5_EagleBlockPuzzle.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "InteractiveObjectList.h"
#define NUM_ROWS 6
#define NUM_COLS 13

const CRectangle  MemoryRectHoneyPot(879.0f,469.0f,141.0f,140.0f);

#pragma region PUZZLE_BLOCK

PuzzleBlock::PuzzleBlock(CObject * outerGlow, BlockType blockType, int globalGridIndex, int blockLength, bool isMovable, std::vector<Grid*> * gridPath, bool isHero, int gridIndexForSolution)
{
	_block = new Block(blockType, globalGridIndex, blockLength, isMovable, gridPath, isHero, gridIndexForSolution);
	_outerGlow = outerGlow;
	_isDragging = false;
	_prevMousePoint.SetXY(-1000, -1000);
	CInteractiveObjectList::GetInstance()->Add(this);
	_indexToBeSnaped = -1;
	if (blockType == eBlockType_Horizontal)
	{
		_outerGlow->SetRotate(90);
	}

	_outerGlow->Show(false);
	_puzzleScreen = NULL;

}

void PuzzleBlock::Init(BlockType blockType, int globalGridIndex, int blockLength, bool isMovable, std::vector<Grid*> * gridPath, bool isHero, int gridIndexForSolution)
{
	_block->Init(blockType, globalGridIndex, blockLength, isMovable, gridPath, isHero, gridIndexForSolution);
	_isDragging = false;
	_prevMousePoint.SetXY(-1000, -1000);
	_indexToBeSnaped = -1;
	_outerGlow->Show(false);
}

PuzzleBlock::~PuzzleBlock()
{
	SAFE_DELETE(_block);
	SAFE_DELETE(_outerGlow);
	_puzzleScreen = NULL;
}


void PuzzleBlock::setGlobalGridIndex(int index)
{
	
	//SetPos(&_block->GetPositionOfBlockOnGrid(index));
	// Linux: gives error of taking address of temporary also potential crash factor
	// below is fix
	CPoint blockPos = _block->GetPositionOfBlockOnGrid(index);
	SetPos(&blockPos);
	_block->setGlobalGridIndex(index);
	if (_puzzleScreen)
	{
		_puzzleScreen->UpdateChangesToProfile();
	}
}

void PuzzleBlock::setPuzzleScreen(L2_S5_EagleBlockPuzzle * puzzleScreen)
{
	_puzzleScreen = puzzleScreen;
}


void PuzzleBlock::OnMouseDown()
{
	_isDragging = true;
	_prevMousePoint = Control::Input->Mpos();
	_block->clearCurrentGrids();
	ClearTween();
}

void PuzzleBlock::Update(float ds)
{
	CObject::Update(ds);
	if (_isDragging)
	{
		_outerGlow->Show(true);
	}
	else if (_currentMouseState == eMouseState_Out)
	{
		_outerGlow->Show(false);
	}
	_outerGlow->SetXY(Pos.x, Pos.y);
}

void PuzzleBlock::HandleMouseInput(CInput * mouseInput)
{
	if (!_block->isMovable)
	{
		return;
	}

	CObject::HandleMouseInput(mouseInput);
	if (_isDragging)
	{
		float dx = mouseInput->Mpos().x - _prevMousePoint.x;
		float dy = mouseInput->Mpos().y - _prevMousePoint.y;
		CPoint targetPos(Pos.x + dx, Pos.y + dy, 0);

		if (_block->blockType == eBlockType_Horizontal && fabs(dx) > 0)
		{
			if(_block->canMoveToPosition(targetPos))
			{
				SetX(targetPos.x);
			}
		}
		else if(_block->blockType == eBlockType_Vertical && fabs(dy) > 0)
		{
			if(_block->canMoveToPosition(targetPos))
			{
				SetY(targetPos.y);
			}
		}

		_prevMousePoint = mouseInput->Mpos();
	}

	if (_isDragging && !mouseInput->LBdown())
	{
		_isDragging = false;
		int index = _block->GetSnapLocalIndexForPosition(Pos);
		_block->setLocalGridIndex(index);
		_indexToBeSnaped = index;
		TweenToGrid(index);
		Control::Audio->QuickLoadAndPlaySFX("s5pz2_tiledrag");
	}

}

void PuzzleBlock::TweenToGrid(int index)
{
	CPoint pos = _block->GetPositionOfBlockOnGrid(_block->localToGlobal(index));
	float valArr[5] = {-1, pos.x, pos.y, -1, -1};
	TweenTo(valArr, 0.2f, TW_EASEBACKOUT, true, CC_CALLBACK_1(PuzzleBlock::OnTweenComplete, this));	
}

void PuzzleBlock::OnTweenComplete(CObject* obj)
{
	if (_indexToBeSnaped != -1)
	{
		setGlobalGridIndex(_block->localToGlobal(_indexToBeSnaped));
		_indexToBeSnaped = -1;
	}
}

Block* PuzzleBlock::getBlock()
{
	return _block;
}

void PuzzleBlock::SetisMovable(bool isMovable)
{
	_block->isMovable = isMovable;
	if (isMovable)
	{
		SetAlpha(1);
	}
	else
	{
		SetAlpha(0.8f);
		_outerGlow->Show(false);
	}
}

bool PuzzleBlock::GetIsDragging()
{
	return _isDragging;
}

CObject* PuzzleBlock::getOuterGlow()
{
	return _outerGlow;
}
#pragma endregion

#pragma region L2_S5_EAGLE_BLOCK_PUZZLE
const CRectangle  InvRect(288.0f,59.0f,819.0f,538.0f);

L2_S5_EagleBlockPuzzle::L2_S5_EagleBlockPuzzle()
{
	_isPuzzleSolved = false;
	musicFadeOutTimer = 0;
	_doorAnimTweenNumber = 0;
	_isDraggingAnyBlock = false;
	_isDoorOpenAnimPlaying = false;
	_indexOfCurrentDoorLock = 0;

	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S5\\EAGLEBLOCKPUZZLE\\EAGLEBLOCKPUZZLE.SCN");

	resetbtnText = new CObject();
	resetbtnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESET.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y + 5, HOScene->GetObjectByID("ResetBtn")->ZOrder + 1);
	resetbtnText->SetUID("resetText");
	HOScene->PushObject(resetbtnText);

	InitializeWithProfileData();

	Control::Audio->LoadSample(aAmbs2NagaHead,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs5MoopansDen) )		
	{
		Control::Audio->PlaySample(aAmbs5MoopansDen,true);
	}
}


L2_S5_EagleBlockPuzzle::~L2_S5_EagleBlockPuzzle()
{
	_doorLocks.clear();
	_arrayOfPuzzleBlocks.clear();
	SAFE_DELETE(_unblockPuzzle);
	SAFE_DELETE(_resetBtn);
	Hud->InvBox_StopPuzzleMode();

	//MUSIC_SFX


	Control::Audio->StopSample(aAmbs5MoopansDen);
	Control::Audio->UnloadSample(aAmbs5MoopansDen);
	//MUSIC_SFX
}

void L2_S5_EagleBlockPuzzle::InitializeWithProfileData()
{
	_nagaSculpture = HOScene->GetObjectByID("NagaKey");
	_nagaSculpture->SetMouseClickCallback(CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::OnMouseClick, this));
	_nagaSculpture->SetIsInteractionEnabled(false);
	_pujaScroll = HOScene->GetObjectByID("Scroll");

	_resetBtn = new UIButton(HOScene->GetObjectByID("ResetBtn"), CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::OnResetBtnClicked, this));


	int arrayPts[] = {258,464,1109,118,1184,244,386,588,317,598};
	CPolygon * poly=  new CPolygon(5);
	poly->Init(arrayPts);
	_pujaScroll->SetMouseClickProperties(poly, CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::OnMouseClick, this));

	_pujaScroll->SetIsInteractionEnabled(false);
	_puzzleGridBg = HOScene->GetObjectByID("puzzleGridBg");

	CObject * doorLock = HOScene->GetObjectByID("DoorLock_0");
	doorLock->Show(false);
	_doorLocks.push_back(doorLock);
	doorLock = HOScene->GetObjectByID("DoorLock_1");
	doorLock->Show(false);
	_doorLocks.push_back(doorLock);
	doorLock = HOScene->GetObjectByID("DoorLock_2");
	doorLock->Show(false);
	_doorLocks.push_back(doorLock);
	doorLock = HOScene->GetObjectByID("DoorLock_3");
	doorLock->Show(false);
	_doorLocks.push_back(doorLock);

	_endGridGlow = HOScene->GetObjectByID("EndGridGlow");
	_endGridGlow->Animate(0.3f, 1.0f, 1); 
	_endGridGlow->Show(false);

	_memoryHoneyPot = HOScene->GetObjectByID("MemHoneypot");
	_memoryHoneyPot->SetMouseClickCallback(CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::OnMouseClick, this));
	_memoryHoneyPot->SetIsInteractionEnabled(false);


	_memoryHoneyPot->Show(false);

	_halfOpendDoor = HOScene->GetObjectByID("EagleBlockOpenedDoor");
	_fullOpenedRightDoor = HOScene->GetObjectByID("EagleBlockRightDoor");
	_fullOpenedLeftDoor = HOScene->GetObjectByID("EagleBlockLeftDoor");

	_isFullOpenDoorFadingIn = false;
	_isHalfOpenDoorFadingIn = false;
	_doorOpenTweenTime = 0;
	_doorOpenDelayTime = 0;

	_halfOpendDoor->Show(false);
	_fullOpenedLeftDoor->Show(false);
	_fullOpenedRightDoor->Show(false);

	_numOfPuzzlePartSolved = 0;


	_unblockPuzzle = new UnblockPuzzle(NUM_ROWS, NUM_COLS);

	int gridStates[NUM_ROWS * NUM_COLS] = {

		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
	};

	_unblockPuzzle->initGrids(gridStates, (NUM_ROWS * NUM_COLS));
	if (ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockIndices[0] == -1)
	{
		int blockIndices[MAX_L2_S5_EAGLEBLOCKS] = {9, 14, 16, 17, 21, 22, 30, 37, 39, 41, 48, 51, 57, 59, 62, 65};
		for (int i = 0; i < MAX_L2_S5_EAGLEBLOCKS; i++)
		{
			ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockIndices[i] = blockIndices[i];
		}
	}


	BlockType blockTypes[MAX_L2_S5_EAGLEBLOCKS] = {eBlockType_Horizontal, eBlockType_Vertical, eBlockType_Vertical, eBlockType_Horizontal, eBlockType_Vertical, eBlockType_Horizontal, eBlockType_Vertical, eBlockType_Vertical, eBlockType_Horizontal, eBlockType_Vertical, eBlockType_Vertical, eBlockType_Vertical, eBlockType_Vertical, eBlockType_Vertical, eBlockType_Horizontal, eBlockType_Horizontal};
	int blockLengths[MAX_L2_S5_EAGLEBLOCKS] = {3, 2, 3, 2, 2, 3, 3, 2, 2, 3, 3, 3, 2, 2, 2, 2};

	int zOrder = HOScene->GetObjectByID("Mask")->ZOrder - 1;
	for (int i = 0; i < MAX_L2_S5_EAGLEBLOCKS; i++)
	{
		std::vector<Grid*> * path = _unblockPuzzle->getGridPath(ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockIndices[i], blockTypes[i]); 

		CObject * outerGlow = new CObject();
		if (blockLengths[i] == 3)
		{
			outerGlow->LoadInit("LV2\\S5\\EAGLEBLOCKPUZZLE\\ThreeGridEagleBlockGlow.SPR", 0, 0, zOrder);
			outerGlow->SetUID(std::string("outerGlow_") + MKSTR(i));
		}
		else
		{
			outerGlow->LoadInit("LV2\\S5\\EAGLEBLOCKPUZZLE\\TwoGridEagleBlockGlow.SPR", 0, 0, zOrder);
			outerGlow->SetUID(std::string("outerGlow_") + MKSTR(i));
		}



		PuzzleBlock * puzzleBlock = new PuzzleBlock(outerGlow, blockTypes[i], ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockIndices[i], blockLengths[i], true, path, false, -1);
		puzzleBlock->setPuzzleScreen(this);
		puzzleBlock->SetMouseOverCallback(CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::OnMouseOver, this));
		puzzleBlock->SetMouseOutCallback(CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::OnMouseOut, this));
		CPoint gridOrigin(229, 86, 0);
		CPoint gridSize(72, 72, 0);
		puzzleBlock->getBlock()->initGridProperties(NUM_ROWS, NUM_COLS, gridOrigin,  gridSize);
		_unblockPuzzle->addBlock(puzzleBlock->getBlock());
		std::string  str = std::string("LV2\\S5\\EAGLEBLOCKPUZZLE\\EagleBlock_") + MKSTR(i) + std::string(".SPR");
		puzzleBlock->LoadInit(str, 0, 0, zOrder - 1);
		puzzleBlock->SetUID(std::string("EagleBlock_") + MKSTR(i));
		puzzleBlock->setGlobalGridIndex(ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockIndices[i]);
		HOScene->PushObject(puzzleBlock);
		_arrayOfPuzzleBlocks.push_back(puzzleBlock);
		puzzleBlock->SetisMovable(false);
		puzzleBlock->Show(false);
	}

	_puzzleGridBg->Show(false);
	_fullOpenedLeftDoor->Show(false);
	_fullOpenedRightDoor->Show(false);
	_pujaScroll->Show(false);
	_nagaSculpture->Show(false);

	switch (ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState)
	{

	case eEagleBlockPuzzleState_started:
		{
			_endGridGlow->Show(true);
			_unblockPuzzle->getBlock(8)->isHero = true;
			_unblockPuzzle->getBlock(8)->gridIndexForSolution = 46;
			_puzzleGridBg->Show(true);
			for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
			{
				PuzzleBlock* puzzleBlock = _arrayOfPuzzleBlocks.at(i);
				puzzleBlock->Show(true);
				int blockIndex = puzzleBlock->getBlock()->getGlobalGridIndex();
				int column = blockIndex % NUM_COLS;
				if (column <= 6)
				{
					puzzleBlock->SetisMovable(true);
				}
			}

			for (int i = 0; i < (int)(_doorLocks.size()); i++)
			{
				CObject * doorLock = _doorLocks.at(i);
				doorLock->Show(true);
			}
			Hud->InvBox_StartPuzzleMode();
		}
		break;
	case eEagleBlockPuzzleState_HalfPartSolved:
		{
			_numOfPuzzlePartSolved = 1;
			_indexOfCurrentDoorLock = 2;
			_puzzleGridBg->Show(true);
			_endGridGlow->Show(true);
			for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
			{

				PuzzleBlock* puzzleBlock = _arrayOfPuzzleBlocks.at(i);
				puzzleBlock->Show(true);
				int blockIndex = puzzleBlock->getBlock()->getGlobalGridIndex();
				int column = blockIndex % NUM_COLS;
				if (column > 6)
				{
					puzzleBlock->SetisMovable(true);
				}
			}
			_unblockPuzzle->getBlock(8)->isHero = true;
			_unblockPuzzle->getBlock(8)->gridIndexForSolution = 50;

			for (int i = 2; i < (int)(_doorLocks.size()); i++)
			{
				CObject * doorLock = _doorLocks.at(i);
				doorLock->Show(true);
			}
			Hud->InvBox_StartPuzzleMode();
		}
		break;
	case eEagleBlockPuzzleState_Solved:
		{
			_resetBtn->Show(false);
			resetbtnText->Show(false);
			_resetBtn->SetIsInteractionEnabled(false);
			_pujaScroll->SetIsInteractionEnabled(true);
			_fullOpenedLeftDoor->Show(true);
			_fullOpenedRightDoor->Show(true);
			_pujaScroll->Show(true);
			_nagaSculpture->Show(true);
			if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle])
			{
				_memoryHoneyPot->Show(true);
				_memoryHoneyPot->SetIsInteractionEnabled(true);
			}
			_pujaScroll->SetIsInteractionEnabled(true);
		}
		break;
	case eL2S5EagleBlockPuzzleState_PujaChartTaken:
		{
			_resetBtn->Show(false);
			resetbtnText->Show(false);
			_resetBtn->SetIsInteractionEnabled(false);
			_fullOpenedLeftDoor->Show(true);
			_fullOpenedRightDoor->Show(true);
			_nagaSculpture->Show(true);
			if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle])
			{
				_memoryHoneyPot->Show(true);
				_memoryHoneyPot->SetIsInteractionEnabled(true);
			}
			_nagaSculpture->SetIsInteractionEnabled(true);
		}
		break;
	case eL2S5EagleBlockPuzzleState_NagaSculpture_Taken:
		{
			_resetBtn->Show(false);
			resetbtnText->Show(false);
			_resetBtn->SetIsInteractionEnabled(false);


			if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle])
			{
				_memoryHoneyPot->Show(true);
				_memoryHoneyPot->SetIsInteractionEnabled(true);
			}	
		}
	default:
		break;
	}

}

void L2_S5_EagleBlockPuzzle::OnResetBtnClicked(CObject * obj)
{
	float tweenVals[5] = {0, -1, -1, -1, -1};
	for (int i = 0; i < MAX_L2_S5_EAGLEBLOCKS; i++)
	{
		PuzzleBlock * puzzleBlock = _arrayOfPuzzleBlocks.at(i);
		puzzleBlock->SetisMovable(false);
		puzzleBlock->SetAlpha(1);
		if (i == MAX_L2_S5_EAGLEBLOCKS - 1)
		{
			puzzleBlock->TweenTo(tweenVals, 0.5, 0, true, CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::Reset, this));
		}
		else
		{
			puzzleBlock->TweenTo(tweenVals, 0.5, 0, true);
		}
	}
}


void L2_S5_EagleBlockPuzzle::Reset(CObject * obj)
{



	int gridStates[NUM_ROWS * NUM_COLS] = {

		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
	};

	_unblockPuzzle->initGrids(gridStates, (NUM_ROWS * NUM_COLS));


	int blockIndices[MAX_L2_S5_EAGLEBLOCKS] = {9, 14, 16, 17, 21, 22, 30, 37, 39, 41, 48, 51, 57, 59, 62, 65};
	if (ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState == eEagleBlockPuzzleState_HalfPartSolved)
	{
		blockIndices[8] = 46;
	}
	for (int i = 0; i < MAX_L2_S5_EAGLEBLOCKS; i++)
	{
		ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockIndices[i] = blockIndices[i];
	}





	for (int i = 0; i < MAX_L2_S5_EAGLEBLOCKS; i++)
	{

		PuzzleBlock * puzzleBlock = _arrayOfPuzzleBlocks.at(i);
		puzzleBlock->Init(puzzleBlock->getBlock()->blockType, ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockIndices[i], puzzleBlock->getBlock()->blockLength, true, puzzleBlock->getBlock()->gridPath, false, -1);
		puzzleBlock->setGlobalGridIndex(blockIndices[i]);
		puzzleBlock->SetisMovable(false);
		puzzleBlock->SetAlpha(0);
	}




	float tweenVals[5] = {1, -1, -1, -1, -1};
	for (int i = 0; i < MAX_L2_S5_EAGLEBLOCKS; i++)
	{

		PuzzleBlock * puzzleBlock = _arrayOfPuzzleBlocks.at(i);
		if (i == MAX_L2_S5_EAGLEBLOCKS - 1)
		{
			puzzleBlock->TweenTo(tweenVals, 0.5, 0, true, CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::OnResetFadeInComplete, this));
		}
		else
		{
			puzzleBlock->TweenTo(tweenVals, 0.5, 0, true);
		}

	}


}

void L2_S5_EagleBlockPuzzle::OnResetFadeInComplete(CObject * obj)
{
	switch (ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState)
	{
	case eEagleBlockPuzzleState_started:
		{
			_unblockPuzzle->getBlock(8)->isHero = true;
			_unblockPuzzle->getBlock(8)->gridIndexForSolution = 46;
			for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
			{
				PuzzleBlock* puzzleBlock = _arrayOfPuzzleBlocks.at(i);
				puzzleBlock->Show(true);
				int blockIndex = puzzleBlock->getBlock()->getGlobalGridIndex();
				int column = blockIndex % NUM_COLS;
				if (column <= 6)
				{
					puzzleBlock->SetisMovable(true);
				}
				else
				{
					puzzleBlock->SetisMovable(false);
				}
			}

			Hud->InvBox_StartPuzzleMode();
		}
		break;
	case eEagleBlockPuzzleState_HalfPartSolved:
		{
			_numOfPuzzlePartSolved = 1;
			for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
			{

				PuzzleBlock* puzzleBlock = _arrayOfPuzzleBlocks.at(i);
				puzzleBlock->Show(true);
				int blockIndex = puzzleBlock->getBlock()->getGlobalGridIndex();
				int column = blockIndex % NUM_COLS;
				if (column > 6)
				{
					puzzleBlock->SetisMovable(true);
				}
				else
				{
					puzzleBlock->SetisMovable(false);
				}
			}
			_unblockPuzzle->getBlock(8)->isHero = true;
			_unblockPuzzle->getBlock(8)->gridIndexForSolution = 50;
			Hud->InvBox_StartPuzzleMode();
		}
		break;
	}
}



void L2_S5_EagleBlockPuzzle::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL2theme) )
	{
		Control::Audio->Samples[aTrackL2theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL2theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL2theme);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL2puzzle) )
				Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2puzzle))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
	}

	if(_isDoorOpenAnimPlaying)
	{
		_doorOpenDelayTime += Control::LogicRate;
		if(_isHalfOpenDoorFadingIn && _doorOpenDelayTime<0.65f)
			return;
		if(_isHalfOpenDoorFadingIn)
		{
			_doorOpenTweenTime += Control::LogicRate;
			if(_doorOpenTweenTime>0.35f)

				_puzzleGridBg->SetAlpha((0.4f-(_doorOpenTweenTime-0.35f))/0.4f);
			_halfOpendDoor->SetAlpha((_doorOpenTweenTime*2.0f)>1.0f?1.0f:(_doorOpenTweenTime*2.0f));

			if(_doorOpenTweenTime > 1)
			{
				_doorOpenTweenTime = 0;
				_doorOpenDelayTime = 0;

				_isHalfOpenDoorFadingIn = false;
				_isFullOpenDoorFadingIn = true;

				_fullOpenedLeftDoor->SetAlpha(0);
				_fullOpenedRightDoor->SetAlpha(0);

				_fullOpenedLeftDoor->Show(true);
				_fullOpenedRightDoor->Show(true);

				_puzzleGridBg->SetAlpha(0);
			}
		}
		else if(_isFullOpenDoorFadingIn)
		{
			if(_doorOpenDelayTime > 0.5f)
			{
				_doorOpenTweenTime += Control::LogicRate;

				_halfOpendDoor->SetAlpha(1.0f - easing(_doorOpenTweenTime,0,1,1));

				_fullOpenedLeftDoor->SetAlpha(easing(_doorOpenTweenTime,0,1,1));
				_fullOpenedRightDoor->SetAlpha(easing(_doorOpenTweenTime,0,1,1));
			}

			if(_doorOpenTweenTime > 1)
			{
				_fullOpenedLeftDoor->SetAlpha(1.0f);
				_fullOpenedRightDoor->SetAlpha(1.0f);

				_halfOpendDoor->Show(false);

				_doorOpenTweenTime = 0;
				_isFullOpenDoorFadingIn = false;
				_isDoorOpenAnimPlaying = false;
				OnCompleteDoorOpenAnim();
			}
		}
	}
}

void L2_S5_EagleBlockPuzzle::Update()
{
	NavigateToScene();
	Cursor::SetMode(CA_Normal);
	_isDraggingAnyBlock = false;
	for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
	{
		PuzzleBlock* puzzleBlock = _arrayOfPuzzleBlocks.at(i);
		if (puzzleBlock->GetIsDragging())
		{
			_isDraggingAnyBlock = true;
		}
		if (_isDoorOpenAnimPlaying)
		{
			puzzleBlock->SetAlpha(_puzzleGridBg->pSprite->Alpha);
		}
	}

	if(_numOfPuzzlePartSolved <= 2 && _unblockPuzzle->isSolved())
	{
		OnPuzzleSolved();
	}

	if (_nagaSculpture->GetCurrentMouseState() == eMouseState_Over 
		|| _nagaSculpture->GetCurrentMouseState() == eMouseState_Down
		|| _pujaScroll->GetCurrentMouseState() == eMouseState_Over
		|| _nagaSculpture->GetCurrentMouseState() == eMouseState_Down
		)
	{
		Cursor::SetMode(CA_HandTake);
	}

	bool isHandPointy = false;
	if (ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState < eEagleBlockPuzzleState_Solved)
	{
		for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
		{
			PuzzleBlock * puzzleBlock = _arrayOfPuzzleBlocks.at(i);
			if (puzzleBlock->GetCurrentMouseState() == eMouseState_Over || puzzleBlock->GetCurrentMouseState() == eMouseState_Down)
			{
				isHandPointy = true;
				break;
			}
		}
		if (_isDraggingAnyBlock || _resetBtn->GetCurrentMouseState() == eMouseState_Over || _resetBtn->GetCurrentMouseState() == eMouseState_Down)
		{
			isHandPointy = true;
		}

		if (isHandPointy)
		{
			Cursor::SetMode(CA_HandPointy);
		}
	}


	//if( !_isDraggingAnyBlock && IsMouseOverGoBackArea(&Control::Input->Mpos()) )
	// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CPoint mousePos = Control::Input->Mpos();

	if( !_isDraggingAnyBlock && IsMouseOverGoBackArea(&mousePos) )
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L2_S5_MoopansDen);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L2_S5_MoopansDen);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S5_MoopansDen, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_MoopansDen;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed() && !_isDraggingAnyBlock )
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_MoopansDen;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}	
	#endif

}

void L2_S5_EagleBlockPuzzle::UpdateChangesToProfile()
{
	for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
	{
		PuzzleBlock * puzzleBlock = _arrayOfPuzzleBlocks.at(i);
		ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockIndices[i] = puzzleBlock->getBlock()->getGlobalGridIndex();
	}

}

float L2_S5_EagleBlockPuzzle::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L2_S5_EagleBlockPuzzle::OnPuzzleSolved()
{
	_resetBtn->Disable();
	_resetBtn->SetIsInteractionEnabled(false);
	if (_numOfPuzzlePartSolved > 2)
	{
		return;
	}
	_numOfPuzzlePartSolved++;
	if (_numOfPuzzlePartSolved == 1)
	{
		for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
		{

			PuzzleBlock* puzzleBlock = _arrayOfPuzzleBlocks.at(i);
			int blockIndex = puzzleBlock->getBlock()->getGlobalGridIndex();
			int column = blockIndex % NUM_COLS;
			if (column > 6)
			{
				puzzleBlock->SetisMovable(true);
			}
			else
			{
				puzzleBlock->SetisMovable(false);
			}
		}
		_unblockPuzzle->getBlock(8)->gridIndexForSolution = 50;
		_numOfLocksToMove = 2;
		StartDoorUnlockAnimSequence();
	}
	else if (_numOfPuzzlePartSolved == 2)
	{
		_resetBtn->Show(false);
		resetbtnText->Show(false);
		_numOfLocksToMove = 4;
		StartDoorUnlockAnimSequence();

		for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
		{
			PuzzleBlock* puzzleBlock = _arrayOfPuzzleBlocks.at(i);
			{
				puzzleBlock->getBlock()->isMovable = false;
			}
		}
		AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	}
}

void L2_S5_EagleBlockPuzzle::OnMouseClick(CObject * object)
{
	if(GFInstance->IsActivePopup)
	{
		return;
	}

	if (object->UID.compare("NagaKey") == 0)
	{
		Hud->TakeInventoryItem(eInv_L2S5_NagaSculpture);
		object->Show(false);
		ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken = true;
		_nagaSculpture->SetIsInteractionEnabled(false);
		ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState = eL2S5EagleBlockPuzzleState_NagaSculpture_Taken;

	}
	else if(object->UID.compare("Scroll") == 0)
	{
		_pujaScroll->SetIsInteractionEnabled(false);
		_nagaSculpture->SetIsInteractionEnabled(true);
		Hud->TakeInventoryItem(eInv_L2S5_SnakeChart);
		object->Show(false);
		ProfileSystem::Instance->ActiveProfile.L2_S5_InvPujaChartTaken = true;
		Control::Input->ClearMouse();
		ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState = eL2S5EagleBlockPuzzleState_PujaChartTaken;
		Control::Audio->QuickLoadAndPlaySFX("s5pz2_poojachartpicked");
	}
	else if(object->UID.compare("MemHoneypot") == 0)
	{
#ifdef _CEBUILD
		_memoryHoneyPot->FadeOut();
		Hud->ShowMemoryObjectCollected_CE(eMem_L2S5HoneyBottle, &HOScene->GetObjectByID("MemHoneypot")->Pos);
		_memoryHoneyPot->SetIsInteractionEnabled(false);

		if (ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState == eL2S5EagleBlockPuzzleState_NagaSculpture_Taken)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_MoopansDen;
			Hud->isSceneCloseAfterMemoryFade = true;
		}
#else
		_memoryHoneyPot->FadeOut();
		Hud->ShowMemoryObjectCollected_SE(eMem_L2S5HoneyBottle, &HOScene->GetObjectByID("MemHoneypot")->Pos);
#endif 

	}
}

void L2_S5_EagleBlockPuzzle::ProcessSkip()
{
	for (int i = 0; i < (int)(_doorLocks.size()); i++)
	{
		CObject * doorLock = _doorLocks.at(i);
		doorLock->Show(false);
	}
	for (int i = 0; i < (int)(_arrayOfPuzzleBlocks.size()); i++)
	{
		PuzzleBlock* puzzleBlock = _arrayOfPuzzleBlocks.at(i);
		{
			puzzleBlock->getBlock()->isMovable = false;
		}
	}

	_resetBtn->SetEnable(false);
	resetbtnText->Show(false);
	_resetBtn->Disable();
	_resetBtn->SetIsInteractionEnabled(false);

	Hud->InvBox_StopPuzzleMode();

	OnSnakeBlockTweenComplete(NULL);
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
}

void L2_S5_EagleBlockPuzzle::ReceiveMessage(int val)
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

	}
	else if( val == INV_CLOSE )
	{		


		if (ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken)
		{		
#ifdef _CEBUILD
			if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle])
#endif 
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_MoopansDen;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
	}	
}

void L2_S5_EagleBlockPuzzle::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle])
		{
			InitHintTrailEmit(950, 550);
		}
		return;
	}
	if(ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState >= eEagleBlockPuzzleState_Solved)
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState >= eL2S5EagleBlockPuzzleState_NagaSculpture_Taken)
		{
			InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
			return;
		}
		else 
		{
			InitHintTrailEmit(&InvRect);
			return;
		}
	}
}  

void L2_S5_EagleBlockPuzzle::OnMouseOver(CObject * obj)
{
	// PuzzleBlock * puzzleBlock = (PuzzleBlock*)obj;  //Sac: Unused variable, so commented it
	if (!_isDraggingAnyBlock)
	{
		((PuzzleBlock*)obj)->getOuterGlow()->Show(true);
	}
}

void L2_S5_EagleBlockPuzzle::OnMouseOut(CObject * obj)
{
	((PuzzleBlock*)obj)->getOuterGlow()->Show(false);
}

void L2_S5_EagleBlockPuzzle::OnSnakeBlockTweenComplete(CObject * obj)
{

	if(_handleExtrasGameComplete())
	{
		return;
	}
	if (_isDoorOpenAnimPlaying)
	{
		return;
	}
	_isHalfOpenDoorFadingIn = true;
	_isDoorOpenAnimPlaying = true;
	Control::Audio->QuickLoadAndPlaySFX("s5pz2_dooropen");
	_halfOpendDoor->Show(true);
	_halfOpendDoor->SetAlpha(0);
	_nagaSculpture->Show(true);
	_pujaScroll->Show(true);
	_endGridGlow->Show(false);
	Hud->ShowBannerText("L2giving");

#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle])
	{
		_memoryHoneyPot->Show(true);
		_memoryHoneyPot->SetIsInteractionEnabled(true);
	}	
#else
	_memoryHoneyPot->Show(true);
#endif 


}

void L2_S5_EagleBlockPuzzle::OnCompleteDoorOpenAnim()
{
	_isDoorOpenAnimPlaying = false;
	_pujaScroll->SetIsInteractionEnabled(true);
	ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState = eEagleBlockPuzzleState_Solved;
	ProfileSystem::Instance->ActiveProfile.L2_S5_ShelfPuzzleSolved = true;
	Hud->InvBox_StopPuzzleMode();
}


void L2_S5_EagleBlockPuzzle::StartDoorUnlockAnimSequence()
{

	CObject * doorLock = _doorLocks.at(_indexOfCurrentDoorLock);
	float tweenVals[5] = {-1, doorLock->Pos.x - 50, -1, -1, -1};
	doorLock->TweenTo(tweenVals, 0.5, 0, true, CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::OnCompleteDoorUnlockAnim, this));
	Control::Audio->QuickLoadAndPlaySFX("s5pz2_locksopen");

}

void L2_S5_EagleBlockPuzzle::OnCompleteDoorUnlockAnim(CObject * obj)
{
	_indexOfCurrentDoorLock++;
	if (_indexOfCurrentDoorLock < _numOfLocksToMove)
	{
		StartDoorUnlockAnimSequence();
	}
	else
	{
		if (ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState == eEagleBlockPuzzleState_started)
		{
			for (int i = 0; i < _numOfLocksToMove; i++)
			{
				CObject * doorLock = _doorLocks.at(i);
				doorLock->Show(false);
			}
			ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState = eEagleBlockPuzzleState_HalfPartSolved;
			_resetBtn->Enable();
			_resetBtn->SetIsInteractionEnabled(true);
		}
		else
		{
			for (int i = 0; i < _numOfLocksToMove; i++)
			{
				CObject * doorLock = _doorLocks.at(i);
				doorLock->Show(false);
			}
			PuzzleBlock* snakeBlock = _arrayOfPuzzleBlocks.at(8);
			float valArr[5] = {-1, snakeBlock->Pos.x + 250, snakeBlock->Pos.y, -1, -1};
			snakeBlock->TweenTo(valArr, 1.0f, TW_EASEBACKOUT, true, CC_CALLBACK_1(L2_S5_EagleBlockPuzzle::OnSnakeBlockTweenComplete, this));
			Control::Audio->QuickLoadAndPlaySFX("s5pz2_lasttileslide");
		}
	}

}

void L2_S5_EagleBlockPuzzle::NavigateToScene()
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

#pragma endregion


