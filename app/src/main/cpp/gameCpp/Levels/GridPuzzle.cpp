//====================================
// Framework
// UpdateList Class
//====================================

#include "GridPuzzle.h"
#include "GameFlow.h"
namespace gridpuzzle
{
#pragma region Grid

	Grid::Grid(int gridIndex, bool isEmpty)
	{
		this->gridIndex = gridIndex;
		this->isEmpty = isEmpty;
	}

	Grid::~Grid()
	{
		//K_LOG("Grid destroctor");
	}

	void Grid::Change()
	{
		isEmpty = !isEmpty;
	}

#pragma endregion


#pragma region GridPuzzleObject


	GridPuzzleObject::GridPuzzleObject()
	{
		_isDragging = false;
	}

	GridPuzzleObject::GridPuzzleObject(int gridIndex, bool isMovable)
	{
		_currentGridIndex = gridIndex;
		_isMovable = isMovable;
	}

	GridPuzzleObject::~GridPuzzleObject()
	{

	}

	void GridPuzzleObject::setCurrentGridIndex(int index)
	{
		_currentGridIndex = index;
	}

	int GridPuzzleObject::getCurrentGridIndex()
	{
		return _currentGridIndex;
	}

	void GridPuzzleObject::setIsMovable(bool isMovable)
	{
		_isMovable = isMovable;
	}

	bool GridPuzzleObject::getISMovable()
	{
		return _isMovable;
	}

	bool GridPuzzleObject::getIsDragging()
	{
		return _isDragging;
	}

	void GridPuzzleObject::addSolution(Solution solution)
	{
		_arrayOfSolutions.push_back(solution);
	}

	void GridPuzzleObject::addSolution(int gridIndex, float rotation, bool isNeedToCheckRotation)
	{
		Solution solution = {gridIndex, rotation, isNeedToCheckRotation};
		_arrayOfSolutions.push_back(solution);
	}

	std::vector<Solution> GridPuzzleObject::getArrayOfSolutions()
	{
		return _arrayOfSolutions;
	}
#pragma endregion

#pragma region GridPuzzle


	GridPuzzle::GridPuzzle(int rows, int columns, CPoint gridOrigin, CPoint gridSize)
	{
		init(rows, columns, gridOrigin, gridSize);
	}

	void GridPuzzle::init(int rows, int columns, CPoint gridOrigin, CPoint gridSize)
	{
		_gridOrigin = gridOrigin;
		_gridSize = gridSize;
		_numOfColumns = columns;
		_numOfRows = rows;
		_isTweeningObject = false;

		for (int i = 0; i < rows * columns; i++)
		{
			Grid * grid = new Grid(i);
			_grids.push_back(grid);
		}
	}

	GridPuzzle::~GridPuzzle()
	{
		for (int i = 0; i < (int)(_grids.size()); i++)
		{
			delete _grids.at(i);
		}
		_grids.clear();
	}

	void GridPuzzle::initGridStates(int gridStates[], int totalGrids)
	{
		if (totalGrids !=(int)_grids.size())
		{
			//array size missmatch
			#ifdef K_WIN32
				EXCEPTION_ARRAY_BOUNDS_EXCEEDED;
			#else
				throw std::out_of_range ("Grid array out of bounds");
			#endif

			return;
		}

		for (int i = 0; i < totalGrids; i++)
		{
			_grids.at(i)->isEmpty = !gridStates[i];
		}
	}

	bool GridPuzzle::canMoveToPosition(CPoint position)
	{
		int index = getGridIndexForPosition(position);
		if(_grids.at(index)->isEmpty)
		{
			return true;
		}
		else 
		{
			return false;
		}

		return false;
	}

	CPoint GridPuzzle::getPositionOfGrid(int gridIndex)
	{
		CPoint point;
		int columnNumber = gridIndex % _numOfColumns;
		int rowNumber = gridIndex / _numOfColumns;
		point.x = _gridOrigin.x + (columnNumber * _gridSize.x) + (_gridSize.x / 2.0f);
		point.y = _gridOrigin.y + (rowNumber * _gridSize.y) + (_gridSize.y / 2.0f);
		return point;
	}

	int GridPuzzle::getGridIndexForPosition(CPoint position)
	{
		float dx = position.x - _gridOrigin.x;
		int columnNumber = (int)(dx / _gridSize.x);
		float dy = position.y - _gridOrigin.y;
		int rowNumber = (int)(dy / _gridSize.y); 
		int index = (rowNumber * _numOfColumns) + columnNumber;
		return index;
	}

	bool GridPuzzle::isSolved()
	{
		bool isSolved = true;
		int heroCount = 0;
		for (int i = 0; i < (int)(_objects.size()); i++)
		{
			GridPuzzleObject* object = _objects.at(i);
			std::vector<Solution> arrayOfSolutions = object->getArrayOfSolutions();
			bool isAnyCorrectSolution = false;
			if (arrayOfSolutions.size() > 0)
			{
				heroCount++;
			}
			for (int i = 0; i < (int)(arrayOfSolutions.size()); i++)
			{
				Solution solution = arrayOfSolutions.at(i);
				if (solution.gridIndexForSolution != -1 && solution.isNeedToCheckRotation)
				{
					if (solution.gridIndexForSolution == object->getCurrentGridIndex() && Elixir::NormalizeAngle(solution.rotationForSolution) == Elixir::NormalizeAngle(object->GetRotate()))
					{
						isAnyCorrectSolution = true;
						break;
					}
				}
				else if(solution.gridIndexForSolution != -1 && solution.gridIndexForSolution == object->getCurrentGridIndex())
				{
					isAnyCorrectSolution = true;
					break;
				}
				else if(solution.isNeedToCheckRotation && Elixir::NormalizeAngle(solution.rotationForSolution) == Elixir::NormalizeAngle(object->GetRotate()))
				{
					isAnyCorrectSolution = true;
					break;
				}
			}
			if(!isAnyCorrectSolution)
			{
				isSolved = false;
			}
			
		}
		if (heroCount > 0 && isSolved)
		{
			return true;
		}
		return false;
	}

	int GridPuzzle::getNumColumns()
	{
		return _numOfColumns;;
	}

	void GridPuzzle::addObject(GridPuzzleObject* object)
	{
		_objects.push_back(object);
	}

	GridPuzzleObject* GridPuzzle::getObject(int index)
	{
		return _objects.at(index);
	}

	void GridPuzzle::tweenObjectToGrid(GridPuzzleObject* object, int index)
	{
		CPoint pos = getPositionOfGrid(index);
		float valArr[5] = {-1, pos.x, pos.y, -1, -1};
		object->TweenTo(valArr, 0.4f, TW_EASEQUADOUT, true, CC_CALLBACK_1(GridPuzzle::onTweenComplete, this));	
		object->SetIsInteractionEnabled(false);
		_isTweeningObject = true;
	}

	void GridPuzzle::onTweenComplete(CObject* object)
	{
		GridPuzzleObject* puzzleObject = (GridPuzzleObject*)object;
		int index = getGridIndexForPosition(puzzleObject->Pos);
		puzzleObject->setCurrentGridIndex(index);

		_isTweeningObject = false;
		object->SetIsInteractionEnabled(true);
		if (isSolved())
		{
			onPuzzleSolved();
		}
	}

	void GridPuzzle::onPuzzleSolved()
	{
		_isSolved = true;
	}

	GridPuzzleObject* GridPuzzle::getObjectAtGridIndex(int index)
	{
		for (int i = 0; i < (int)(_objects.size()); i++)
		{
			GridPuzzleObject * object = _objects.at(i);
			if (object->getCurrentGridIndex() == index)
			{
				return object;
			}
		}

		return NULL;
	}

	GridPuzzleObject* GridPuzzle::getObjectAtPosition(CPoint position)
	{
		int index = getGridIndexForPosition(position);
		return getObjectAtGridIndex(index);
	}



#pragma endregion
}
