

#ifndef DQFYLH_L5_S5_PUZZLE_SUNCLOCK_DEF
#define DQFYLH_L5_S5_PUZZLE_SUNCLOCK_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "JSONAnimator.h"

#define MAX_L5S5_PZBLOCKS 8

class L5_S5_PZSunClock : public HiddenObjectSystem
{
public:
	L5_S5_PZSunClock();
	~L5_S5_PZSunClock();
	float musicFadeOutTimer;
	void Update();
	void Required();

	void InitPuzzle();
	void UpdateBlockStates();

	void SaveTokenStates();
	void changeTokenState(int id);
	void NavigateToScene();

protected:
	std::vector<CObject*> _tokenArray;
	int _tokenStates[MAX_L5S5_PZBLOCKS];
	CRectangle* _tokenRects[MAX_L5S5_PZBLOCKS];
	CObject*	_baseGlows[MAX_L5S5_PZBLOCKS];

	float clickTimer;

	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();

	bool isPuzzleActive, isPuzzleSolved;

	void OnAnimationComplete(int animID);
	bool isOrbRisingAnimCompleted;
	JSONAnimator* _JAnim_OrbRising;

	SuperFX *s5orbrisingFX, *s5pzfliesFX;

	void HandlePuzzleComplete();
};

#endif
