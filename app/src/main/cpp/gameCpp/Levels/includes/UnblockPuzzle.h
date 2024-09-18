//====================================
// Framework
// UpdateList Class
//====================================

#ifndef UNBLOCK_PUZZLE_H
#define UNBLOCK_PUZZLE_H

#include "Framework.h"
#include "Object.h"
#include "GridPuzzle.h"
using namespace gridpuzzle;

enum BlockType
	{
		eBlockType_Horizontal = 0,
		eBlockType_Vertical,
	};

class Block 
{
	

	protected:

		int _currentLocalGridIndex; // startingGridIndex 
		int _currentGlobalGridIndex;
		CPoint  _gridOrigin;
		CPoint  _gridSize;
		int _numOfGrids;
		int _numOfRows;
		int _numOfColumns;

	public:


		BlockType blockType;
		std::vector<Grid*> * gridPath;
		int blockLength;
		bool isMovable;
		bool isHero;
		int gridIndexForSolution;

		Block();
		Block(BlockType blockType, int globalGridIndex, int blockLength, bool isMovable, std::vector<Grid*> * gridPath, bool isHero = false, int gridIndexForSolution = -1);
		void Init(BlockType blockType, int globalGridIndex, int blockLength, bool isMovable, std::vector<Grid*> * gridPath, bool isHero = false, int gridIndexForSolution = -1);
		~Block();
		void initGridProperties(int rows, int columns, CPoint gridOrigin, CPoint gridSize);
		
		bool canMoveToPosition(CPoint position);
		CPoint GetPositionOfBlockOnGrid(int globalGridIndex);
		int GetLocalGridIndexForPosition(CPoint position);
		int GetSnapLocalIndexForPosition(CPoint position);

		void setLocalGridIndex(int index);
		int getLocalGridIndex();

		void setGlobalGridIndex(int index);
		int getGlobalGridIndex();

		int globalToLocal(int globalIndex);
		int localToGlobal(int localIndex);

		void clearCurrentGrids();




};

class UnblockPuzzle
{
	protected:

	std::vector<Grid*> _grids;
	std::vector<Block*> _blocks;
	int _numColmns;

	public:

	UnblockPuzzle(int rows, int columns);
	~UnblockPuzzle();
	void initGrids(int gridStates[], int totalGrids);
	bool isSolved(); 
	std::vector<Grid*>* getGridPath(int globalIndex, BlockType blockType);
	int getNumColumns();
	void addBlock(Block* block);
	Block* getBlock(int index);

};
#endif