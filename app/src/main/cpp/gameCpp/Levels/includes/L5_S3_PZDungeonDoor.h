

#ifndef DQFYLH_L5_S3_PUZZLEDUNGEON_DEF
#define DQFYLH_L5_S3_PUZZLEDUNGEON_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "JSONAnimator.h"
#include "UIButton.h"
#include <stack>
#include "MultilineText.h"

namespace Level5_Scene3
{
	enum ColorType
	{
		PZ_Color_None = -1,
		PZ_Color_Orange,
		PZ_Color_Purple,
		PZ_Color_Blue,
		PZ_Color_Green,
		PZ_Color_TotalColors
	};

	class Locket : public CObject
	{

		float _startAngle, _endAngle; 
		float _radious;
		CPoint _basePos;
		float _animTimer;

		C_SYNTHESIZE(ColorType, _color, Color);
		C_SYNTHESIZE(int, _currentIndex, Index);
		C_SYNTHESIZE(int, _colorOffsetVal, ColorOffsetVal);

	public:

		Locket();
		~Locket();

		void setRotationProperty(CPoint bPos, CPoint tNextPos, float angleOffset);
		void MoveTo(CPoint pos);
		void Rotate();

	private:

		void Update(float ds);
	};

	class LocketsHolder : public CObject
	{

		std::vector<Locket*> _lockets;
		std::map<int, CPoint> _locketPoints;

		C_SYNTHESIZE(ColorType, _color, Color);
		C_SYNTHESIZE(int, _index, HolderIndex);
		C_SYNTHESIZE(float, _angleOffset, AngleOffset);

	public:

		LocketsHolder();
		~LocketsHolder();

		void AddLocket(Locket* locket);
		void AddPoint(int index, CPoint point);

		void Rotate();
		Locket* GetLocket(int index);
		void Exchange(Locket* locket, int index);

		bool IsSolved();

	private:

		void OnRotationComplete();

	};

	struct SwapObject
	{
	public:

		int _locketIndex;
		Locket* _locket;
		LocketsHolder *_locketHolder;
	};

	class PuzzleController
	{

		bool _isPuzzleActive;
		std::vector<UIButton*> _swapBtns;

		std::map<ColorType, LocketsHolder*> _baseHolders;
		std::map<Locket*, CPoint> _locketsForSkip;
		C_SYNTHESIZE(UIButton*, _puzzleButton, PuzzleButton);
		C_SYNTHESIZE(CObject*, _pzBtnRing, PZBtnRing);
		C_SYNTHESIZE(bool, _isSolved, IsSolved);

	public:

		PuzzleController();
		~PuzzleController();

		void InitController(std::vector<CObject*> holders, std::vector<CObject*> lockets, CHOScene *scene);
		void ActivatePuzzle();
		void Update();
		void Skip();

	private:

		ColorType GetColorTypeFrom(std::string uid);

	};

	class PuzzleScene : public CHOScene
	{

	public:

		PuzzleScene();
		~PuzzleScene();

		void LoadPuzzle(const std::string& Name);
		PuzzleController* CreateController();

	private:


	};
}

enum L5S3PZ_State
{
	L5S3PZ_ReachFirstTime = 0,
	L5S3PZ_HookFallAnim,
	L5S3PZ_HookLost = 60,
	L5S3PZ_WaitingForHook,
	L5S3PZ_PlacingHook,
	L5S3PZ_HookPlaced,
	L5S3PZ_PlayingGame,
	L5S3PZ_PuzzleSolved = 160,
	L5S3PZ_OnComplete,
	L5S3PZ_ExitFromScene = 200,
	L5S3PZ_GameForExtrasComplete
};

class L5_S3_PZDungeonDoor : public HiddenObjectSystem
{
	float musicFadeOutTimer;
	Level5_Scene3::PuzzleController *_puzzleCtrl;
	int _state;
	std::stack<int> _stateMachine;
	JSONAnimator *_JAnim_SceneAnimation;

	MultilineText* pzHelpText;

public:
	L5_S3_PZDungeonDoor();
	~L5_S3_PZDungeonDoor();

	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

	void Update();
	void Required();	
	void ResetPuzzle();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	void OnAnimatonComplete(int animID);

private:

	void InitWithState();
};

#endif
