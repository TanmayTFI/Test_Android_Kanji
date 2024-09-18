#ifndef L3_S2_P1R2_Creeper_Puzzle_
#define L3_S2_P1R2_Creeper_Puzzle_

#include "Popup.h"
#include <vector>

enum MoveTo
{
	No_Move = 0,
	Move_Up,
	Move_Right,
	Move_Down,
	Move_Left
};

class TileInterface
{
public:

	virtual void onTweenComplete(CObject *obj = NULL) = 0;
};


class Tile : public CObject
{
	int _index;
	CRectangle _tileRect;
	
	CObject* _teethObj;
	CObject* _blockObj;
	CObject* _neutralObj;


	C_SYNTHESIZE(TileInterface*, _interface, Interface);
	C_SYNTHESIZE_READONLY(bool, _haveBlock, HaveBlock);
	C_SYNTHESIZE_READONLY(bool, _haveTeeth, HaveTeeth);
public:

	Tile(int index, CRectangle tileRect);
	~Tile();

	void setTeethFinal(CObject* obj, int zorder);
	void setBlockObj(CObject *Obj, int zOrder);
	void setNeutralObj(CObject* obj, int zOrder);
	void Move(MoveTo direction);
	void ShitTileWith(CPoint shiftOffset);
	void SetEnable(bool enable);

private:

	void Update(float ds);
	void TweenComplete(CObject* obj);
};

class PuzzleController : public TileInterface
{
	float _blockIndicateTimer;
	int _noOfCols;
	int _noOfRows;
	int _tilesToShow;

	std::vector<Tile*> _tilesList;
	std::vector<int> _currentTilesIndex;

	bool _pathEnd;
	bool _isTweening;

	int _playerIndex;
	int _blockIndex;
	C_SYNTHESIZE(CObject*, _redBlockIndicate, RedBlockIndicator);
	C_SYNTHESIZE(CObject*, _playerIndicate, PlayerIndicator);
	C_SYNTHESIZE(bool, _isGameOver, GameOver);

public:

	PuzzleController();
	~PuzzleController();

	void InitPuzzleWith(std::vector<Tile*> tiles, int noCols, int noRows, int tilesToShow);
	void Update();
	void Move(MoveTo direction);

	void Skip();
	void PuzzleStart();
	void PuzzlePaused();

private:

	void Reset();
	void ResetCurrentIndex();
	void onTweenComplete(CObject *obj = NULL);
	void PlayerTweenComplete(CObject *obj);
	void ShiftTiles();
	void ShiftTilesToSavedPos();
	void MovePlayer(float speed);
};


class L3_S2_P1R2CreeperPuzzle : public Popup
{
	bool _isgameOver;
	
	PuzzleController *_controller;
	
	std::vector<CObject*> _arrows;

	MoveTo _userInput;

public:

	L3_S2_P1R2CreeperPuzzle(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	virtual ~L3_S2_P1R2CreeperPuzzle(void);

	void ProcessSkip();
	void ProcessHint();

private:

	void Update(float ds);
	void OnPopupOpened();
	void OnPopupCloseBtnClicked();
};

#endif