

#ifndef DQFYLH_L5_S2_PUZZLE_BLOCKS_DEF
#define DQFYLH_L5_S2_PUZZLE_BLOCKS_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#define NUM_BLOCKS 8
#define MAX_BLOCK_GRIDSIZE 9 //3x3 is the maximum block size now

class TetrisBlock{
protected:
	CObject *_obj, *_shadow;
	CPoint _shadowOff;
	int _rows, _colums, _numGridBlocks;
	std::vector<CRectangle*>	_rectArray;

	//default offset where mouse follows
	int _defColumnOffset, _defRowOffset;
public:
	//starting place on grid
	int startRow, startCol;
	int _columnOffset, _rowOffset;

	int values[MAX_BLOCK_GRIDSIZE];

	void setPosWithoutOffset(CPoint pos);
	void setPos(CPoint pos, int xOff=0, int yOff=0);
	void setEnable(bool f)	{_obj->SetEnable(f); _shadow->SetEnable(f);};

	void setEnableShadow(bool f) {_shadow->SetEnable(f);};

	CObject* getObj(){return _obj;};

	bool isPlacedOnGrid;

	bool IsActive() {return _obj->IsActive;};
	void SetTint(bool f) {
		if(f)
			_obj->SetTint(0.85f, 1.95f, 0.65f);
		else
			_obj->SetTint(1.0f, 1.0f, 1.0f);
	};

	void SetZ(int zorder){
		_obj->SetZ(zorder);
		_shadow->SetZ(zorder-1);
	};

	int getRows()		{return _rows;};
	int getColumns()	{return _colums;};
	int getNumBlocks()	{return _numGridBlocks;};

	void setDefaultOffsets(int rowOff, int colOff)	{ 
		_columnOffset = _defColumnOffset = colOff;
		_rowOffset = _defRowOffset=rowOff;
	};

	void restoreDefaultOffsets(int rowOff, int colOff)	{
		_columnOffset = _defColumnOffset;
		_rowOffset = _defRowOffset;
	};

	bool isCollided(CPoint pos);

	TetrisBlock(CObject *obj, CObject *shadow, int shadowOffsetX, int shadowOffsetY, int numRow, int numCol, int val[]);
	~TetrisBlock(){};
};

class L5_S2_PZBlocks: public HiddenObjectSystem
{
	public:
		float musicFadeOutTimer;
		std::vector<CObject*>		blocks_thumbs;
		std::vector<TetrisBlock*>	blocks_big;
		std::vector<CObject*>		grid_glowArray;
		std::vector<float>			glow_TargetAlpha;

		std::vector<int>			indexArray;
		std::vector<int>			gridValArray;
		std::vector<CRectangle*>	gridRectArray;
		std::vector<CPolygon*>		scnStonePolyArray;
		
		bool wasMouseInsideGridRect;

		SuperFX *s2topdustFX, *s2bottomdustFX;
		SuperFX *s2fliesFX, *s2glaresFX;

		bool isDragging;
		int dragBlockID;
		
		L5_S2_PZBlocks();
		~L5_S2_PZBlocks();
		
		void SavePuzzleState();
		void RestorePuzzleState();
		void InitPuzzle();
		void InitGrid();
		void CheckAndPlaceBlockOnGrid(CPoint pos);
		bool checkForEmptySpace(TetrisBlock *block, int clickRow, int clickColumn);
		std::vector<int> getValueArrayFromGrid(int clickRow, int clickColumn, int numRows, int numCols);
		void eraseBlockGridData(TetrisBlock* block);

		void updateGlowAroundMouse(CPoint pos);
		void clearGridGlows();

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		//End Stuff
		CObject *objGridComplete;
		float objGridStartx;
		float easing(float t,float b,float c,float d);

		bool isSkipAnimPlaying, isScalingDown, isSlidingLeft, isSlideDoorOpened;
		float endTimer, skipTimer;
		bool isPuzzleSolved;
		bool checkForGameWin();
		void SolvePuzzle(bool isSkipped=false);

		void Update();
		void Required();

		void ResetPuzzle();
		void NavigateToScene();

	protected:
		void ReceiveMessage(int val);
		void ProcessSkip();
		void ProcessNonHOHint();
};

#endif
