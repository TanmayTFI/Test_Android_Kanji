

#ifndef DQFYLH_L8_S4_PZPOOLMINIGAME_DEF
#define DQFYLH_L8_S4_PZPOOLMINIGAME_DEF

//====================================
//  created by : Sreeraj
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "CVideo.h"
#include "JSONAnimator.h"

class Flowers
{

public:
	CObject* obj;	
	int index;
	int flowerCount;
	bool isVisible;
	CPoint initialPosition;

	void Show(bool f){
			if(obj)
				obj->Show(f);
		};

	Flowers(void);
	~Flowers(void);
};

class L8_S4_PZPoolGame : public HiddenObjectSystem
{
public:
	L8_S4_PZPoolGame();
	~L8_S4_PZPoolGame();

	float musicFadeOutTimer;

	void Update();
	void Required();

	//CPoint FlowersInitPosition[27];
	Flowers *BigFlowers[24];
	Flowers *SmallFlowers[24];
	bool isFlowerOnHand;
	Flowers *flowerOnHand;
	float tweenTime;
	float swapTime;
	bool isFlowerReversing;
	CRectangle board[3][3];
	Flowers * flowersOnBoard[3][3][4];
	CPoint *smallBoardPositions[3][3][4];
	CObject *ColSelection;
	CPoint *ColSelectionPos[3][3];
	int totalFlowersInColumn[3][3];
	int numberOfDrops[3][3];
	int numberOfPlayerDrops[3][3];
	CObject *glowRect[3][3];
	CObject *glowRectGreen[3][3];
	CObject *PuzzleSolvedMsg;
	bool isPuzzleSolved;
	int initialFlowersOnBoard[3][3][4];
	int arrAddedIndex[16];
	CObject *leftText[3][2];
	CObject *topText[3][2];
	CObject *vertText1[2];
	CObject *vertText2[2];

	CObject *leftTextGreen[3][2];
	CObject *topTextGreen[3][2];
	CObject *vertText1Green[2];
	CObject *vertText2Green[2];

	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

	void NavigateToScene();

protected:
	bool _isParticlesStarted;
	float _particleTimer;
	float _partileTimerThreshold, _gameExitThreshold;

	CObject *SolvedState;
	SuperFX *S4_PoolPuzzleComplete1FX, *S4_PoolPuzzleComplete2FX, *S4_PoolPuzzleComplete3FX, *S4_PoolPuzzleComplete4FX;
	CObject *BgFlowers[6];
	CObject *BgLeaves[23];

	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	void ResetPuzzle();

	void InitSceneElements();
	void InitAnimations();
	void movePointerBackToOriginalPosition();
	float easing(float t,float b,float c,float d);
	void initBoardVlaues();

	void removeSmallFlowerOnBoard();
	bool CheckPuzzleSolved();
	void showSuccessLineGlow(int state,int index);
	void updatePuzzleStatusWithSavedValues();
	bool isMouseOnDisabledTile(CPoint pos);
	void updateBoardWithInitialFlowers();
	bool isIndexAdded(int);
	void hideAllText();
	void showGreenLight();

};

#endif
