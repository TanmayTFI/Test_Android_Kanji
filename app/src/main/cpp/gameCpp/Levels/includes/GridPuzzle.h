//====================================
// Framework
// UpdateList Class
//====================================

#ifndef GRID_PUZZLE_H
#define GRID_PUZZLE_H

#include "Framework.h"
#include "Object.h"

namespace gridpuzzle
{
class Grid
{
	public:	
		Grid(int gridIndex, bool isEmpty = true);
		~Grid();
		bool isEmpty;
		int gridIndex;

		void Change();
};

struct Solution
{
	int gridIndexForSolution;
	float rotationForSolution;
	bool isNeedToCheckRotation;	
};

class GridPuzzleObject : public CObject
{
	

	protected:

		int _currentGridIndex; 
		bool _isMovable;
		bool _isDragging;
		std::vector<Solution> _arrayOfSolutions;

	public:


		GridPuzzleObject();
		GridPuzzleObject(int gridIndex, bool isMovable);
		~GridPuzzleObject();


		virtual void setCurrentGridIndex(int index);
		int getCurrentGridIndex();
		
		virtual void setIsMovable(bool isMovable);
		bool getISMovable();
		bool getIsDragging();
		void addSolution(Solution solution);
		void addSolution(int gridIndex, float rotation = 0.0f, bool isNeedToCheckRotation = false);
		std::vector<Solution> getArrayOfSolutions();
};

class GridPuzzle
{
	protected:

	std::vector<Grid*> _grids;
	std::vector<GridPuzzleObject*> _objects;
	CPoint  _gridOrigin;
	CPoint  _gridSize;
	int _numOfGrids;
	int _numOfRows;
	int _numOfColumns;
	bool _isTweeningObject;
	bool _isSolved;

	public:

	GridPuzzle()
	{

	}
	GridPuzzle(int rows, int columns, CPoint gridOrigin, CPoint gridSize);
	virtual ~GridPuzzle();

	void init(int rows, int columns, CPoint gridOrigin, CPoint gridSize);
	void initGridStates(int gridStates[], int totalGrids);
	bool canMoveToPosition(CPoint position);
	CPoint getPositionOfGrid(int gridIndex);
	int getGridIndexForPosition(CPoint position);

	bool isSolved(); 
	int getNumColumns();
	void addObject(GridPuzzleObject* object);
	GridPuzzleObject* getObject(int index);

	virtual void tweenObjectToGrid(GridPuzzleObject* object, int index);
	virtual void onTweenComplete(CObject* object);
	virtual void onPuzzleSolved();
	GridPuzzleObject* getObjectAtGridIndex(int index);
	GridPuzzleObject* getObjectAtPosition(CPoint point);

};
}
#endif