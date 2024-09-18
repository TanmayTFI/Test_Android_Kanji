#ifndef L3_S3_PZBlock_
#define L3_S3_PZBlock_

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MeshObject.h"
#include "GridPuzzle.h"
#include "CVideo.h"
#include <stack>
#include "AssetsStringTable.h"

using namespace std;
using namespace gridpuzzle;

class CHOScene;

enum S3_PZ2SceneState
{
	State_None = 0,
	State_WaitForShoe,
	State_PlayingCutScene,
	State_CutSceneComplete,
	State_PlayingPuzzle,
	State_SkipPuzzle,
	State_PuzzleComplete,
	State_OpenMugDoor = 100,
	State_ReadyToCollectMug = 150,
	State_MugCollected,
	State_L3S3GameInExtrasComplete

};

class TileObjProtocol
{
	C_SYNTHESIZE(bool, _isTileMoving, TileMoving);

public:

	virtual bool isGridEmpty(int gridIndex) = 0;
	virtual	bool canMoveToPosition(CPoint position) = 0;
	virtual CPoint getPositionOfGrid(int gridIndex) = 0;
	virtual void setGridEmpty(int index) = 0;
	virtual void onTileMove(int index) = 0;
	virtual void onTileMoved() = 0;
};


class TileObject : public GridPuzzleObject
{
	bool _isSelected;

	//tweening
	CPoint _startPos;
	CPoint _endpos;
	float _animTimer;

	CObject* _selection;

	C_SYNTHESIZE(TileObjProtocol*, _delegate, Delegate);
	C_SYNTHESIZE(int, _tileIndex, TileIndex);
public:

	CObject* _mouseOver;

	TileObject(int gridIndex, bool isMovable);
	~TileObject();

	void setMouseOver(CObject* obj);
	void setMouseSelect(CObject *obj);
	void SetIsInteractionEnabled(bool isInteractionEnabled);

private:
	
	float easing(float t,float b,float c,float d);
	void MoveTo(CPoint endPos);
	void CheckAndMove();
	void OnAnimationComplete();
	void Update(float ds);
	void OnMouseOver();
	void OnMouseClick();
	void OnMouseOut();
};

class S3_BlockPUzzle : public GridPuzzle, public TileObjProtocol
{
	vector<TileObject*> _tileObjs;
	
	bool _isPuzzleActive;
	float _skipTimer;
	
	C_SYNTHESIZE_READONLY(bool, _isSolved, IsSolved);

public:

	S3_BlockPUzzle();
	~S3_BlockPUzzle();

	void InitPuzzle(vector<TileObject*> tiles);
	void ActivatePuzzle();
	void Update(float ds);
	void SkipPuzzle();
	void ForceSkipPuzzle();

private:

	bool isGridEmpty(int gridIndex);
	bool canMoveToPosition(CPoint position);
	CPoint getPositionOfGrid(int gridIndex);
	void setGridEmpty(int index);
	void onTileMove(int index);
	void onTileMoved();
	void SolvePuzzle();
};

class L3_S3_PZBlock : public HiddenObjectSystem, CRenderListEntity
{
	stack<int> _stateMachine;
	int _state;
	S3_BlockPUzzle *_blockPuzzle;
	float musicFadeOutTimer;
	CVideo* _shoeMemoryVdo;
	float time;
	float vidTimer;
	int subIndex;
	subText subtitles[7];
	CBitmapText* subtitleText; int subMax;
	CObject* _mugBoxDoor;
	CObject* _shoeObj;
	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

	SuperFX *s3BlockPZCompleteFX;

	float _pzSkipTimer;
	bool _showHint;
	void NavigateToScene();

public:
	L3_S3_PZBlock(void);
	~L3_S3_PZBlock(void);

private:

	void LoadSavedState();
	void OnStateChange();
	void ReceiveMessage(int val);
	void Render(const CPoint* pCam);
	void Required();
	void Update();
	void ProcessSkip();
	void ResetPuzzle();
	void ProcessNonHOHint();
};

#endif

