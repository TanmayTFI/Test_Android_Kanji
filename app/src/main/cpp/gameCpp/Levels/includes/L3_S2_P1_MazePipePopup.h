//====================================
// DQFULP
// Hidden Object System

// LEVEL3 - SCENE2 : Maze Pipe
//====================================

#ifndef DQFYLH_LEVEL3_SCENE2_P1_MAZEPIPE
#define DQFYLH_LEVEL3_SCENE2_P1_MAZEPIPE

#include "Popup.h"
#include "GridPuzzle.h"
#include <vector>
#include "GameFlow.h"

using namespace gridpuzzle;
using namespace std;

enum MouseStateInObject
{
	Mouse_Default =0,
	Mouse_Down,
	Mouse_Dragging,
	Mouse_Up,
	Mouse_Click
};

class PuzzleObjectInterface
{
public:

	virtual bool canMoveToPoint(CPoint pos) = 0;
	virtual void setGridToEmpty(int index) = 0; 
	virtual CPoint getPosition(int index) = 0;
	virtual int getGridIndex(CPoint pos) = 0;
	virtual void onPlayerMoveStart(int index) = 0;
	virtual void onPlayerMovEnd(int index) = 0;
};


class MazePipeObjet : public GridPuzzleObject
{
	MouseStateInObject _mouseState;

	CPoint _originPos;
	CPoint _currentPos;
	CPoint _nextPos;

	CPoint _mousePos;
	CPoint _mouseDownPos;

	//tweening
	CPoint _startPos;
	CPoint _endpos;
	int _startAngle; 
	int _endAngle;
	float _animTimer;
	float _rotateTimer;
	float _pipeMotionAnimTime;

	int _originalzOrder;

	C_SYNTHESIZE(int, _pipeIndex, PipeIndex);
	C_SYNTHESIZE(PuzzleObjectInterface*, _delegate, Delegate);
	C_SYNTHESIZE(int, _zOrderTop, ZOrderTop);

public:
	MazePipeObjet();
	~MazePipeObjet();

	void LoadInit(const std::string& sprPath, float x, float y, int zOrder, bool enable=true);
	void setCurrentGridIndex(int index);
	void MoveTo(CPoint endPos);
	void RotateTo(int angle);

private:

	void OnAnimationComplete();

	void Update(float ds);
	void OnMouseOver();
	void OnMouseClick();
	void OnMouseDown();
	void OnMouseUp();
	void OnMouseOut();
};

class PipeSolution
{
public:

	map<int, vector<int> > _gridIndexAndAngle;

	PipeSolution()
	{

	}

	void Add(int gridIndex, int angles)
	{
		vector<int> angs = _gridIndexAndAngle[gridIndex];
		angs.push_back(angles);
		_gridIndexAndAngle[gridIndex] = angs;
	}

	void SetSolutions(MazePipeObjet *obj)
	{
		map<int, vector<int> >::iterator it;
		for (it = _gridIndexAndAngle.begin(); it != _gridIndexAndAngle.end(); it++)
		{
			for (int i = 0; i < (int)(it->second.size()); i++)
			{
				obj->addSolution(it->first, (float)(it->second[i]), true);
			}
		}
	}
};

class MazePipePuzzle : public GridPuzzle, public PuzzleObjectInterface
{	
	vector<CObject*> _pipes;
	vector<MazePipeObjet*> _mazePipeObjets;
	vector<MazePipeObjet*> _greenPipes;

	// position map
	map<int, CPoint> _solutionPos;
	map<int, CPoint> _initialPos;

	// Solution map
	vector<PipeSolution> Solution;
	vector<PipeSolution> SkipSolution;
	void InitSolution();

	C_SYNTHESIZE(bool, _isSolved, IsSolved);

public:
	MazePipePuzzle();
	~MazePipePuzzle();

	void InitPosition(CObject *pipe, int gridIndex);
	void InitPuzzleWith(vector<CObject*> pipes, CHOScene *scene);
	void ActivatePuzzle();
	void Update(float ds);
	void SkipPuzzle();

private:

	void OnPuzzleSolved();

	//delegate functions
	bool canMoveToPoint(CPoint pos);
	void setGridToEmpty(int index);
	CPoint getPosition(int index);
	int getGridIndex(CPoint pos);
	void onPlayerMoveStart(int index);
	void onPlayerMovEnd(int index);
};

enum L3_S2_P1_PuzzleState
{
	Puzzle_Inactive = 0,
	Puzzle_Active,
	Puzzle_Playing,
	Puzzle_Solved = 120,
	Puzzle_Complete

};

class L3_S2_P1_MazePipePopup: public Popup
{
	MazePipePuzzle *_pipePuzzle;
	bool _isPuzzleActive;

	int _puzzleState;

	C_SYNTHESIZE_READONLY(bool, _isPuzzleSolved, IsPuzzleSolved);
public:

	L3_S2_P1_MazePipePopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3_S2_P1_MazePipePopup();

	void SetReadyForPlay(bool ready);
	void Required();
	void Update(float ds);

	void HandleInventoryDrop(CPoint dropPos, int itemVal);
	void ProcessSkip();

	float easing(float t,float b,float c,float d);
	void ProcessHint();

protected:

	void OnPopupOpened();
	void OnPopupCloseBtnClicked();
};

#endif