

#ifndef DQFYLH_L5_S4_PUZZLE_HANDCUFF_DEF
#define DQFYLH_L5_S4_PUZZLE_HANDCUFF_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "UIButton.h"
#include <queue>
#include <stack>
#include <utility>

namespace Level5_Scene4_PZ1
{
	enum KeyToothState
	{
		Tooth_Up = 0,
		Tooth_Middle,
		Tooth_Bottom
	};

	enum KeyRotateDirection
	{
		Key_Left = -1,
		Key_None = 0,
		Key_Right = 1
	};

	struct LockSolutionPattern
	{
	public:

		KeyToothState _tooth1;
		KeyToothState _tooth2;
		KeyToothState _tooth3;
		KeyToothState _tooth4;

		bool operator==(LockSolutionPattern rhs)
		{
			if (_tooth1 == rhs._tooth1 &&
				_tooth2 == rhs._tooth2 &&
				_tooth3 == rhs._tooth3 &&
				_tooth4 == rhs._tooth4)
				return true;

			return false;
		}
	};

	class KeyNob : public CObject
	{
		std::queue<KeyToothState> __toothPosibleStates;
		std::vector<CObject*> __tooths;
		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE_READONLY(KeyToothState, _toothState, ToothState);

	public:

		KeyNob(CObject* animObj);
		~KeyNob();

		void CreateTooth(CHOScene* scene);
		void Rotate();
		void SetAlpha(float alpha);
		void Show(bool show);

	private:

		void OnMouseClick();
		void OnMouseOver();
		void OnMouseOut();
		void UpdateToothState();
	};


	class Key : public CObject
	{
		bool _active;
		std::vector<KeyNob*> __nobs;
		C_SYNTHESIZE(CObject*, _keyToPut, KeyToPut);
		C_SYNTHESIZE(CObject*, _disableMask, DisableMask);
		C_SYNTHESIZE_READONLY(bool, _isInHole, IsInHole);

	public:

		Key(CObject* scnObj);
		~Key();

		void CreateKey(CHOScene* scene);
		void SetAlpha(float alpha);
		void Show(bool show);
		LockSolutionPattern getUnLockPattern();
		bool isMouseOnNob();
		void setIsInHole(bool isinhole);
		void SetEnableKey(bool enable);

	private:

	};

	class PZ_Interface
	{
	public:

		virtual void OnUnlock() = 0;
		virtual void OnKeyHoleSelected() = 0;
		virtual void OnMatchFound(int index) = 0;
		virtual void OnWrongSelection() = 0;
	};


	class Lock
	{
		LockSolutionPattern _solution;

		std::queue<std::pair<KeyRotateDirection, int> > _solutionRotation;
		std::queue<std::pair<KeyRotateDirection, int> > _handleRotation;
		std::pair<KeyRotateDirection, int> _currentRotation;
		int _numOfRotation;
		int _currentRotationIndex;

		std::vector<CObject*> _wrongLight;
		std::vector<CObject*> _correctLight;
		UIButton *_rotateLeft, *_rotateRight;
		CObject *_keyOnHole;
		CObject *_lockLiver;

		C_SYNTHESIZE(CObject*, _holeGlow, HoleGlow);
		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(CRectangle, _lockRect, LockRect);
		C_SYNTHESIZE_READONLY(Key*, _currentKey, CurrentKey);
		C_SYNTHESIZE(bool, _isUnLocked, Unlocked);
		C_SYNTHESIZE(PZ_Interface*, _interface, Interface);

	public:

		Lock();
		~Lock();

		void AddKeySolution(LockSolutionPattern Pattern);
		void AddToSolutionKeyRotation(int rotateTo, int times);
		void CreateLock(CHOScene* scene);
		
		bool PutKey(Key *key);
		void SkipInitiated();

	private:

		void Reset();
		void OnKeyMatched();
		void onLeftRotate(CObject* btn);
		void onRightRotate(CObject* btn);
		void onKeyFadeInComplete(CObject* obj);
		void onKeyRotateComplete(CObject* obj);
		void onLiverPullComplete(CObject* obj);

	protected:

		void Unlock();
	};

	class ToothShape
	{
		bool _selected;
		bool _readyToCheck;
		float _wrongIndicateDisableTimer;

		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(bool, _isCompleted, IsCompleted);
		C_SYNTHESIZE(CPolygon*, _toothPoly, ToothPoly);
		C_SYNTHESIZE(CPolygon*, _keyHolePoly, KeyHolePoly);
		C_SYNTHESIZE(CObject*, _keyHoleHighLight, KeyHoleHighlight);
		C_SYNTHESIZE(CObject*, _keyHoleSelect, KeyHoleSelect);
		C_SYNTHESIZE(CObject*, _toothHihgLight, ToothHighlight);
		C_SYNTHESIZE(CObject*, _pathHihgLight, PathHighlight);
		C_SYNTHESIZE(CObject*, _toothCorrect, ToothCorrect);
		C_SYNTHESIZE(CObject*, _toothWrong, ToothWrong);
		C_SYNTHESIZE(CObject*, _keyHoleDoor, KeyHoleDoor);
		C_SYNTHESIZE(PZ_Interface*, _interface, Interface);

	public:

		ToothShape();
		~ToothShape();

		void Init();
		void Update();
		void EnableTooth(bool enable);
		void OnCompleted();
		void EnableKeyHole(bool enable);
		
	};


	class PZ_Controller : public PZ_Interface
	{
		std::vector<ToothShape*> _keyTooths;
		Key* _key;
		std::vector<Lock*> _locks;
		
		C_SYNTHESIZE_READONLY(bool, _isPZL1Solved, IsL1Solved);
		C_SYNTHESIZE_READONLY(bool, _isPZL2Solved, IsL2Solved);

	public:

		PZ_Controller();
		~PZ_Controller();

		void InitPuzzle(CHOScene* scene);
		void ShowKey(bool show);
		void ActivateL1();
		void ActivateL2();
		void L1Update();
		void L2Update();
		void Skip();

	private:

		void InitL1Data(CHOScene *scene);
		void OnKeyHoleSelected();
		void OnMatchFound(int index);
		void OnWrongSelection();
		void OnUnlock();
	};
}

enum L5_S4_PZState
{
	L5_S4_PZ_Inactive = 0,
	L5_S4_PZ_Active,
	L5_S4_PZ_Level1Playing = 120,
	L5_S4_PZ_Level1Complete,
	L5_S4_PZ_Level2Playing,
	L5_S4_PZ_Skip,
	L5_S4_PZ_Solved,
	L5_S4_PZ_Completed = 200,
	L5_S4_PZ_GameForExtrasComplete
};

class L5_S4_PZHandCuff : public HiddenObjectSystem
{
	float musicFadeOutTimer;
	int _pzState;
	std::stack<L5_S4_PZState> _stateMachine;
	Level5_Scene4_PZ1::PZ_Controller *_pzController;

public:
	L5_S4_PZHandCuff();
	~L5_S4_PZHandCuff();

	void Update();
	void Required();
	void ResetPuzzle();
	void NavigateToScene();

	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

protected:

	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();

private:

	void InitPuzzle();
};

#endif
