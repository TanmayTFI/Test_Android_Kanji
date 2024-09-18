//====================================
// DQFYLH
// Hidden Object System
// Level 4 - Scene 02 : PUZZLE DOOR
//====================================

#ifndef DQFYLH_LEVEL4_SCENE3_PZDOOR_DEF
#define DQFYLH_LEVEL4_SCENE3_PZDOOR_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MultilineText.h"

enum L4S3_SceneState
{
	SnakeTopScene,
	SnakeBottomScene,
	MovingDown,
	MovingUp
};

enum L4S3_SymbolsType
{
	SymbolA,
	SymbolB,
	Dummy
};

class L4_S3_PZDoor: public HiddenObjectSystem, CRenderListEntity
{
	public:
		float musicFadeOutTimer;
		//Scene Elements
		

		//Popups
		
		L4_S3_PZDoor();
		~L4_S3_PZDoor();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);
		void Update();
		void Required();

		CHOScene *BottomScene;

		float ScrollTimer, FadeTimer;
		float ScrollTime, FadeTime;
		int CurrentSceneState;
		int SelectionCount;
		int CurrentSelectedSymbol, prevSelectedSymbol;
		bool IsInputLocked;
		bool	MatchedIndex[MAX_L4_S3_SYMBOLS];
		bool IsShaking;

		MultilineText *txtInfo;
		int numMatchesFound;

		float ShakeTimer;
		L4S3_SymbolsType SelectedSymbolType;
		SuperFX *S3DoorDust1FX;

		CRectangle	SymbolRectA[MAX_L4_S3_SYMBOLS];
		CRectangle	SymbolRectB[MAX_L4_S3_SYMBOLS];
		CRectangle	DummyRect[MAX_L4_S3_DUMMYSYMBOLS];

		CObject *HilightA, *HilightB;
		CObject *SymbolsA[MAX_L4_S3_SYMBOLS];
		CObject *SymbolsB[MAX_L4_S3_SYMBOLS];
		CObject *ActiveSymbolsA[MAX_L4_S3_SYMBOLS];
		CObject *ActiveSymbolsB[MAX_L4_S3_SYMBOLS];
		CObject *DummySymbols[MAX_L4_S3_DUMMYSYMBOLS];

		void NavigateToScene();
		
		float easing(float t,float b,float c,float d);

	protected:
		CObject *UpArrow, *DownArrow;

		void ProcessSkip();
		void OnTweenComplete();
		void SelectSymbol(bool isValid, CPoint *pos);
		void ActivateSymbol(int index);
		void ResetActiveSymbols(int index);
		void CheckIfPuzzleSolved();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif