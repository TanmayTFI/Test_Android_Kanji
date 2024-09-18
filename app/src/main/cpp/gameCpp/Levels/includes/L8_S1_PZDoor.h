

#ifndef DQFYLH_L8_S1_DOORPUZZLE_DEF
#define DQFYLH_L8_S1_DOORPUZZLE_DEF

//====================================
//  created by : Sreeraj
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "JSONAnimator.h"

class RingPart
{
	public:
		CObject *obj;
		int col;
		int row;
		int index;
		bool isVisible;
		std::string name;
		int currentRow;
		CPoint ringPartPos;
		CPolygon *ringPositionPoly;
		bool isWinPart;

		RingPart(void);
		~RingPart(void);
};

class L8_S1_PZDoor : public HiddenObjectSystem
{
	CPolygon *handRect;
	CPolygon *magicGlassRect;
	JSONAnimator *Janim_doorOpen;

	int _delayTimer;
public:
	L8_S1_PZDoor();
	~L8_S1_PZDoor();

	void Update();
	void Required();
	void NavigateToScene();

	RingPart *ringParts[3][12];
	CObject *handWithGlow,*hand;
	CObject *symbolGlow[3];

	CObject *MouseOver[3];
	CObject *Selection[3];
	CObject *PuzzleSolvedMsg;

	int ringIndices[12];
	int ringPieceCount;
	int rowContInColumn[4];

	bool isHandMoveAnimationRunning;
	float handStartY, handEndY;

	CPolygon *positionPoly[3][4];
	CPoint ringPositions[3][4];

	RingPart *visibleRings[3][4];

	RingPart *swapPart1New , * swapPart2New;
	RingPart *swapPart1 , * swapPart2;
	bool isSwaping;
	float tweenTime;
	float swapTime;
	std::string path;
	bool isSwapingSameCol;
	bool puzzleSoved;

	float musicFadeOutTimer;

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	void InitSceneElements();
	void UpdateMousePointer(int,int);

	float easing(float t,float b,float c,float d);
	void GenerateRingPieces();
	void setPositionForRingPieces();
	void swapRingPieces(RingPart*, RingPart*);
	std::string getSpriteName(RingPart *);

	void SetWinParts();
	bool isPuzzleSolved();
	void SaveData();
	void SetRingsPartWithSaveData();
	void OnPuzzleComplete();
	void OnAnimComplete(int animID);
};

#endif
