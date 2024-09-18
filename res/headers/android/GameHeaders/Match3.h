//====================================
// DQFYLH
// Hidden Object System
// MATCH 3
//====================================

#ifndef DQFYLH_MATCH3_DEF
#define DQFYLH_MATCH3_DEF

#include "Control.h"
#include "GameFlow.h"

class CHOScene;

#define MAX_MATCH3_ROWS 7
#define MAX_MATCH3_COLS 10
#define MAX_MATCH3_BREAKS 140 //2*MAX_MATCH3_ROWS*MAX_MATCH3_COLS
#define MAX_MATCH3_VARIETY 5 
class Match3: public Control
{

	C_SYNTHESIZE_READONLY(bool, _isCompleted, IsCompleted);

	public:

		Match3();
		Match3(int levelId);
		~Match3();

		int LevelId;

		CBitmapText *txtInfo;

		//Match 3
		CHOScene *Match3Scene;

		//Popup Elements
		CObject *m3Image1, *m3MouseOver,*m3Mask;
		CObject *board[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
		CObject *gems[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
		CObject *gemSel[2];
		CObject *m3GemOverlay;
		bool removableGems[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
		bool isItDropDisabled[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
		void ShowGemBreakAnim();
		int countGemsToRemove;
		int selX;
		int selY;
		bool isTileSwaping;
		bool isReverseSwaping;
		int fromX,toX,fromY,toY;
		float tweenTime, swapTime, dropTime;
		bool isRemovablesMarked;
		bool isPuzzleSolved;
		CObject *breakAnim[MAX_MATCH3_BREAKS];
		int indexBreakAnim;
		bool isBreakAnimPlaying;
		
		CObject *dropingGems[MAX_MATCH3_BREAKS];
		CPoint dropingGemsStartPos[MAX_MATCH3_BREAKS];
		CPoint dropingGemsEndPos[MAX_MATCH3_BREAKS];
		CObject *iceGems[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
		void fillDropingGemsArray();
	
		bool isSelected[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
		int selIndex;
		int gemType[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
		std::string strGems[MAX_MATCH3_VARIETY];
		void insertNewGem(int x,int y, int Type);
		bool isMatch3Found();
		void markRemovableGems();
		void deleteMarkedGems();
		bool isGemsDroping;
		void InitPuzzle();
		

		std::string path;
		void clearBoard();
		void ShowBoard();

		bool isGemAnimPlaying();

		void PlaySuccessSFX();
		int PrevRandomId;

		void Update();
		void Required();

		float easing(float t,float b,float c,float d);

		//AJI
		int DeadLockTicks;
		void HandleDeadLock();
		bool IsTileIdPresent( int iRow, int iCol, int TileId );
		void DeleteMatch3Board();
		void RemoveMatches();

	protected:
		void ReceiveMessage(int val);
		void DisposeAssets();
};

#endif