

#ifndef DQFYLH_L7_S3_PUZZLE_DOOR_DEF
#define DQFYLH_L7_S3_PUZZLE_DOOR_DEF

//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "JSONAnimator.h"
#include "UIButton.h"
#include <stack>
#include "CVideo.h"
#include "AssetsStringTable.h"

namespace Level7_Scene3
{
	enum SymbolType
	{
		Symbol_Red = 0,
		Symbol_Blue,
		Symbol_Yellow,
		Symbol_Add
	};

	enum ColorValue
	{
		Color_Purple = 1,
		Color_Orange,
		Color_Green
	};

	class SceneObjectDelegate
	{
	public:

		virtual void onSymbolClicked(SymbolType type) = 0;
		virtual void onSymbolLockComplete() = 0;
	};

	class Hand : public CObject
	{
		bool _isShowing;
		C_SYNTHESIZE(CObject*, _handWithPower, HandWithPower);

	public:

		Hand(CObject* obj);
		~Hand();
	
		void ShowHand();
		void ShowPower();
		void FadeOut(float fadeOutSpeed);

	private:

		void Update(float ds);
	};

	class HandSymbol : public UIButton
	{
		bool _selected;
		int _disableDelay;
		bool _enabled;

		C_SYNTHESIZE(SymbolType, _type, HandSymbolType);
		C_SYNTHESIZE(Hand*, _hand, Hand);
		C_SYNTHESIZE(SuperFX*, _sfx, Sfx);
		C_SYNTHESIZE(SceneObjectDelegate*, _delegate, Delegate);

	public:

		HandSymbol(CObject* obj);
		~HandSymbol();

		void HideHand();
		void DisableSymbol();
		void EnableAfterDelay(float delay);
		void Enable();

	private:

		void Update(float ds);
		void Clicked();
		void OnMouseClick();
		void OnMouseUp();
	};

	class Lock
	{

		CObject* _lock1;
		CObject* _lock2;

		C_SYNTHESIZE_READONLY(bool, _isUnlocked, IsUnlocked);
		C_SYNTHESIZE(ColorValue, _color, Color);
		C_SYNTHESIZE(SceneObjectDelegate*, _delegate, Delegate);

	public:

		Lock(CObject* obj1, CObject* obj2);
		~Lock();

		bool Unlock(ColorValue color);
		void setUnlocked(bool isUnLocked);

	private:

		void openCompleted();

	};

	class PZController : public SceneObjectDelegate
	{
		
		SuperFX *s3RedHandEmitFX, *s3BlueHandEmitFX, *s3YellowHandEmitFX;
		SuperFX *s3PurpleEmitFX, *s3OrangeEmitFX, *s3GreenEmitFX;

		int _symbolClickDelay, _unLockDelay, _colorMatchDelay;

		bool _havePower;
		std::vector<HandSymbol*> _symbols;
		std::vector<Lock*> _locks;
		std::list<SymbolType> _selectedSymbols;
		std::vector<UIButton*> _buttonsList;

		std::map<ColorValue, CObject*> _doorCenterEffectObj;
		std::map<ColorValue, SuperFX*> _doorCenterParticle;

		UIButton *_addBtn, *_resetBtn;
		CObject *_resetHand;
		bool _isReset;

		C_SYNTHESIZE_READONLY(bool, _izPzCompleted, IsPzCompleted);

	public:

		PZController();
		~PZController();

		void InitPuzzle(CHOScene* scene);
		void ActivatePuzzle(bool activate);
		void DisableAllControlls();
		void Update();
		void SkipPuzzle();

	protected:

		 void onSymbolClicked(SymbolType type);
		 void onSymbolLockComplete();
		 void onAddButtonClick(CObject* btn);
		 void onResetButtonClick(CObject* btn);

	private:

		void ShowDoorUnlockEffect();
		void CheckCombination();
		void AnimateSymbol(CObject* obj);
	};
}

enum L7S3_PZState
{
	L7S3_PZState_WaitToCollectLever = 0,
	L7S3_PZState_Initialize,
	L7S3_PZState_CheckForPower,
	L7S3_PZState_WaitingForPower,
	L7S3_PZState_GotPower,
	L7S3_PZState_RemovingThreads,
	L7S3_PZState_TreadsRemoved,
	L7S3_PZState_RingOpenDelay,
	L7S3_PZState_RingOpened,
	L7S3_PZState_PlayingPuzzle,
	L7S3_PZState_Skipping,
	L7S3_PZState_AllLocksUnlocked,
	L7S3_PZState_WaitingToPickLetter = 60,
	L7S3_PZState_WaitingForLetterClose,
	L7S3_PZState_WaitingToOpenDoor,
	L7S3_PZState_OPeningDoor,
	L7S3_PZState_DoorOpened = 130,
	L7S3_PZState_WaitToEnterNextScene,
	L7S3_PZState_NightMareVdoPlaying,
	L7S3_PZState_NightMareVdoComplete,
	L7S3_PZState_GameForExtrasCompleted,
	L7S3_PZState_DoorClicked
};


class L7_S3_PZDoorPuzzle : public HiddenObjectSystem
{
	CVideo *_cvdo_ThreadCut;
	float vidTimer;
	float vidTimer2;
	JSONAnimator *_janim_TakeLetter, *_janim_LetterToMap;
	bool _isAnimPlaying;
	int _pzState;
	std::stack<int> _stateMacine;
	Level7_Scene3::PZController *pzController;
	CVideo* _cvdo_nightMare;
	float time;
	int subIndex;
	subText subtitles[5];
	CBitmapText* subtitleText; int subMax;

	CHOScene *threadScene;
	CHOScene *doorOpenedScene;

public:
	L7_S3_PZDoorPuzzle();
	~L7_S3_PZDoorPuzzle();
	float musicFadeOutTimer;
	CObject* havenText;
	void Update();
	void Required();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();

private: 

	void LoadSavedState();
	void onAnimComplete(int animId);
	void onRingOpenComple();
};

#endif
