//====================================
// Framework
// UpdateList Class
//====================================

#include "UnblockPuzzle.h"


#pragma region Block

Block::Block()
{

}

Block::Block(BlockType blockType, int globalGridindex, int blockLength, bool isMovable, std::vector<Grid*> *  gridPath, bool isHero, int gridIndexForSolution)
{
	this->Init( blockType, globalGridindex, blockLength, isMovable, gridPath, isHero, gridIndexForSolution); 
}

void Block::Init(BlockType blockType, int globalGridindex, int blockLength, bool isMovable, std::vector<Grid*> *  gridPath, bool isHero, int gridIndexForSolution)
{
	this->_currentGlobalGridIndex = globalGridindex; 
	this->_currentLocalGridIndex = 0;
	this->blockType = blockType;
	this->blockLength = blockLength;
	this->isMovable = isMovable;
	this->isHero = isHero;
	this->gridIndexForSolution = gridIndexForSolution;
	this->gridPath = gridPath;
}


Block::~Block()
{
	gridPath->clear();
	delete gridPath;
	gridPath = NULL;
	
}

void Block::initGridProperties(int rows, int columns, CPoint gridOrigin, CPoint gridSize)
{
	_gridOrigin = gridOrigin;
	_gridSize = gridSize;
	_numOfColumns = columns;
	_numOfRows = rows;
	_numOfGrids = rows * columns;
}


bool Block::canMoveToPosition(CPoint position)
{
	//float distance;
	float blockHeadPosition;
	float blockTailPosition;
	int targetHeadIndex = -1;
	int targetTailIndex = -1;
	int currentTailIndex = _currentLocalGridIndex + (blockLength - 1);
	int currentHeadIndex = _currentLocalGridIndex;
	
	

	if (blockType == eBlockType_Horizontal)
	{
		blockHeadPosition = position.x - ((_gridSize.x * blockLength) / 2.0f);
		blockTailPosition = position.x + ((_gridSize.x * blockLength) / 2.0f);

		float leftBorder = _gridOrigin.x;
		float rightBorder = _gridOrigin.x + (_numOfColumns * _gridSize.x);

		if (blockHeadPosition < leftBorder || blockTailPosition > rightBorder)
		{
			return false;
		}
		targetHeadIndex = (int) ((blockHeadPosition - _gridOrigin.x) / _gridSize.x);
		targetTailIndex = (int) ((blockTailPosition - _gridOrigin.x) / _gridSize.x);
	}
	else if(blockType == eBlockType_Vertical)
	{
		
		blockHeadPosition = position.y - ((_gridSize.y * blockLength) / 2.0f);
		blockTailPosition = position.y + ((_gridSize.y * blockLength) / 2.0f);

		float topBorder = _gridOrigin.y;
		float bottomBorder = _gridOrigin.y + (_numOfRows * _gridSize.y);

		if (blockHeadPosition < topBorder || blockTailPosition > bottomBorder)
		{
			return false;
		}
		targetHeadIndex =  (int)((blockHeadPosition - _gridOrigin.y) / _gridSize.y);
		targetTailIndex =  (int)((blockTailPosition - _gridOrigin.y) / _gridSize.y);
	}

	

    int gridSize = (int) gridPath->size();  //Sac: converted Implicit to explicit

	if (targetHeadIndex < 0 || targetTailIndex >= gridSize)
	{
		return false;
	}

	bool isPathToTargetGridEmpty = true;

	if (targetHeadIndex >= _currentLocalGridIndex)// move forward
	{
		for (int i = (currentTailIndex + 1); i <= targetTailIndex; i++)
		{
			if (!gridPath->at(i)->isEmpty)
			{
				isPathToTargetGridEmpty = false;
			}
		}
	}
	else // move backward
	{
		for (int i = (currentHeadIndex - 1); i >= targetHeadIndex; i--)
		{
			if (!gridPath->at(i)->isEmpty)
			{
				isPathToTargetGridEmpty = false;
			}
		}
	}

	if (isPathToTargetGridEmpty)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

CPoint Block::GetPositionOfBlockOnGrid(int globalGridIndex)
{
	int row = globalGridIndex / _numOfColumns;
	int column = globalGridIndex % _numOfColumns;
	CPoint position;

	if (blockType == eBlockType_Horizontal)
	{
		position.x = _gridOrigin.x + (column * _gridSize.x) +  ((_gridSize.x * blockLength) / 2.0f);
		position.y = _gridOrigin.y + (row * _gridSize.y) + (_gridSize.y / 2);
	}
	else if(blockType == eBlockType_Vertical)
	{
		position.x = _gridOrigin.x + (column * _gridSize.x) + (_gridSize.x / 2);
		position.y = _gridOrigin.y + (row * _gridSize.y) +  ((_gridSize.y * blockLength) / 2.0f);
	}

	return position;
}

int Block::GetLocalGridIndexForPosition(CPoint position)
{
	float distance;
	float blockHeadPosition;
	int index = -1;
	if (blockType == eBlockType_Horizontal)
	{
		distance = position.x - _gridOrigin.x;
		blockHeadPosition = distance - ((_gridSize.x * blockLength) / 2.0f);
		index = (int) (blockHeadPosition / _gridSize.x);
	}
	else if(blockType == eBlockType_Vertical)
	{
		distance = position.y - _gridOrigin.y;
		blockHeadPosition = distance - ((_gridSize.y * blockLength) / 2.0f);
		index = (int) (blockHeadPosition / _gridSize.y);
	}

    int gridSize = (int) gridPath->size();  //Sac: converted Implicit to explicit

	if (index < 0 || index >= gridSize)
	{
		index = -1;
	}
	return index;
}

int Block::GetSnapLocalIndexForPosition(CPoint position)
{
	float distance;
	float blockHeadPosition;
	int index = -1;
	if (blockType == eBlockType_Horizontal)
	{
		distance = position.x - _gridOrigin.x;
		blockHeadPosition = distance - ((_gridSize.x * blockLength) / 2.0f);
		index = (int) (blockHeadPosition / _gridSize.x);
		CPoint gridPos = GetPositionOfBlockOnGrid(localToGlobal(index));
		if((position.x - gridPos.x) > _gridSize.x / 2.0f)
		{
			index += 1;
		}
	}
	else if(blockType == eBlockType_Vertical)
	{
		distance = position.y - _gridOrigin.y;
		blockHeadPosition = distance - ((_gridSize.y * blockLength) / 2.0f);
		index = (int) (blockHeadPosition / _gridSize.y);
		CPoint gridPos = GetPositionOfBlockOnGrid(localToGlobal(index));
		if((position.y - gridPos.y) > _gridSize.y / 2.0f)
		{
			index += 1;
		}
	}

    int gridSize = (int)gridPath->size();  //Sac: converted Implicit to explicit

	if (index < 0 || index >= gridSize)
	{
		index = -1;
	}
	return index;
}

void Block::setLocalGridIndex(int index)
{
	_currentLocalGridIndex = index;
	_currentGlobalGridIndex = localToGlobal(_currentLocalGridIndex);

	for (int i = _currentLocalGridIndex; i < _currentLocalGridIndex + blockLength; i++)
	{
		gridPath->at(i)->isEmpty = false;
	}	
}

int Block::getLocalGridIndex()
{
	return _currentLocalGridIndex;
}

void Block::setGlobalGridIndex(int index)
{
	_currentGlobalGridIndex = index;
	int localIndex = globalToLocal(index);
	setLocalGridIndex(localIndex);
}

int Block::getGlobalGridIndex()
{
	return _currentGlobalGridIndex;
}


int Block::globalToLocal(int globalIndex)
{
	int rowColValue=0;

	if (blockType == eBlockType_Horizontal)
	{
		//column
		rowColValue = globalIndex % _numOfColumns;
	}
	else if (blockType == eBlockType_Vertical)
	{
		//row
		rowColValue = globalIndex / _numOfColumns;
	}

	return rowColValue;
}

int Block::localToGlobal(int localIndex)
{
	return gridPath->at(localIndex)->gridIndex;
}

void Block::clearCurrentGrids()
{
	for (int i = _currentLocalGridIndex; i < _currentLocalGridIndex + blockLength; i++)
	{
		gridPath->at(i)->isEmpty = true;
	}
}

#pragma endregion

#pragma region UnblockPuzzle

UnblockPuzzle::UnblockPuzzle(int rows, int columns)
{
	_numColmns = columns;
	
	for (int i = 0; i < rows * columns; i++)
	{
		Grid * grid = new Grid(i);
		_grids.push_back(grid);
	}
}

UnblockPuzzle::~UnblockPuzzle()
{
    int gridSize = (int) _grids.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < gridSize; i++)
	{
		delete _grids.at(i);
	}
	_grids.clear();
}

void UnblockPuzzle::initGrids(int gridStates[], int totalGrids)
{
	if (totalGrids != (int)_grids.size())
	{
		//array size missmatch
		#ifdef K_WIN32
			EXCEPTION_ARRAY_BOUNDS_EXCEEDED;
		#else
			throw std::out_of_range ("Unblock Puzzle Grid array out of bounds");
		#endif

		return;
	}

	for (int i = 0; i < totalGrids; i++)
	{
		_grids.at(i)->isEmpty = !gridStates[i];
	}
}

bool UnblockPuzzle::isSolved()
{
	bool isSolved = true;
	int heroCount = 0;
    int blockSize = (int) _blocks.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < blockSize; i++)
	{
		Block* block = _blocks.at(i);
		if (block->isHero)
		{
			heroCount += 1;

			if(block->getGlobalGridIndex() != block->gridIndexForSolution)
			{
				isSolved = false;
			}
		}
	}
	if (heroCount > 0 && isSolved)
	{
		return true;
	}
	return false;
}

std::vector<Grid*>* UnblockPuzzle::getGridPath(int index, BlockType blockType)
{
	std::vector<Grid*> * gridPath = new std::vector<Grid*>();
	// int totalGrids = _grids.size();  //Sac: Unused variable, so commented it
	int row = index / _numColmns;
	int column = index % _numColmns;
	if (blockType == eBlockType_Horizontal)
	{
		for (int i = 0; i < _numColmns; i++)
		{
			int index1 = (row * _numColmns) + i;  //Sac: this was diclared as index while there is already index declared above, possible bad outcome
			gridPath->push_back(_grids.at(index1));
		}
	}
	else if (blockType == eBlockType_Vertical)
	{
        int totalRows = (int) _grids.size() / _numColmns; // Sac: There was an implicit int converstion so made it explicit
		for (int i = 0; i < totalRows; i++)
		{
			int index1 = column + (i * _numColmns);  //Sac: this was diclared as index while there is already index declared above, possible bad outcome
			gridPath->push_back(_grids.at(index1));
		}
	}

	return gridPath;
}

int UnblockPuzzle::getNumColumns()
{
	return _numColmns;
}

void UnblockPuzzle::addBlock(Block * block)
{
	_blocks.push_back(block);
}

Block* UnblockPuzzle::getBlock(int index)
{
	return _blocks.at(index);
}



#pragma endregion
