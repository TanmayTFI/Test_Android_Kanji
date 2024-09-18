
#ifndef EAGLE_BLOCK_PUZZLE_H
#define EAGLE_BLOCK_PUZZLE_H

#include "HiddenObjectSystem.h"
#include "UnblockPuzzle.h"
#include "GameFlow.h"
#include "UIButton.h"

#include "SuperFX.h"

class ParticleEffect;
class CHOScene;
class L2_S5_EagleBlockPuzzle;

enum L2S5EagleBlockPuzzleState
{
	eEagleBlockPuzzleState_started = 0,
	eEagleBlockPuzzleState_HalfPartSolved,
	eEagleBlockPuzzleState_Solved,
	eL2S5EagleBlockPuzzleState_PujaChartTaken,
	eL2S5EagleBlockPuzzleState_NagaSculpture_Taken
};

class PuzzleBlock: public CObject
{
private:

	bool _isDragging;
	Block * _block;
	CObject * _outerGlow;
	CPoint  _prevMousePoint;
	int _indexToBeSnaped;
	L2_S5_EagleBlockPuzzle* _puzzleScreen;
	


public:
	
	PuzzleBlock(CObject * outerGlow, BlockType blockType, int globalGridIndex, int blockLength, bool isMovable, std::vector<Grid*> * gridPath, bool isHero = false, int gridIndexForSolution = -1);
	void Init(BlockType blockType, int globalGridIndex, int blockLength, bool isMovable, std::vector<Grid*> * gridPath, bool isHero = false, int gridIndexForSolution = -1);
	~PuzzleBlock();

	void setGlobalGridIndex(int index);
	void setPuzzleScreen(L2_S5_EagleBlockPuzzle* puzzleScreen);

	void OnMouseDown();

	virtual void Update(float ds);
	void HandleMouseInput(CInput * mouseInput);
	void TweenToGrid(int index);
	void OnTweenComplete(CObject* obj);
	Block* getBlock();
	void SetisMovable(bool isMovable);
	bool GetIsDragging();
	CObject* getOuterGlow();
	
};

class L2_S5_EagleBlockPuzzle: public HiddenObjectSystem
{
private:
	float musicFadeOutTimer;
	UnblockPuzzle * _unblockPuzzle;
	std::vector<PuzzleBlock*> _arrayOfPuzzleBlocks;
	int _numOfPuzzlePartSolved;
	bool _isDraggingAnyBlock;
	int _doorAnimTweenNumber;
	bool _isDoorOpenAnimPlaying;
	CObject * _puzzleGridBg;
	std::vector<CObject*> _doorLocks;
	int _numOfLocksToMove;
	CObject * _endGridGlow;
	int _indexOfCurrentDoorLock;
	CObject * _nagaSculpture;
	CObject * _pujaScroll;
	CObject * _halfOpendDoor;
	CObject * _fullOpenedRightDoor;
	CObject * _fullOpenedLeftDoor;
	CObject * _memoryHoneyPot;

	bool _isHalfOpenDoorFadingIn;
	bool _isFullOpenDoorFadingIn;
	float _doorOpenDelayTime;
	float _doorOpenTweenTime;

	bool _isPuzzleSolved;

	UIButton * _resetBtn;



public:

	L2_S5_EagleBlockPuzzle();
	~L2_S5_EagleBlockPuzzle();

	void InitializeWithProfileData();
	void Reset(CObject * obj);
	void OnResetBtnClicked(CObject * obj);
	void OnResetFadeInComplete(CObject * obj);
	void OnMouseClick(CObject * obj);
	void Update();
	void UpdateChangesToProfile();
	void ProcessSkip();
	void Required();
	float easing(float t,float b,float c,float d);
	void OnPuzzleSolved();
	void OnMouseOver(CObject * object);
	void OnMouseOut(CObject * object);
	void OnSnakeBlockTweenComplete(CObject * obj);
	void OnCompleteDoorOpenAnim();
	void StartDoorUnlockAnimSequence();
	void OnCompleteDoorUnlockAnim(CObject * obj);

	void NavigateToScene();
	CObject* resetbtnText;
	
protected:

	void ReceiveMessage(int val);
	void ProcessNonHOHint();
};

#endif