

#ifndef DQFYLH_L7_S2_HOBROKENRIGHTHAND_DEF
#define DQFYLH_L7_S2_HOBROKENRIGHTHAND_DEF

//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "L7_S2_HandHOBasic.h"
#include <map>
#include <vector>
#include <list>
#include <stack>

using namespace Level7_Scene_2;

class L7_S2_HOBrokenRightHand : public HiddenObjectSystem, public HandHOControllerDelegate
{
	int _sceneState;
	std::stack<int> _stateMachine;
	Level7_Scene_2::HOController *_hoCtrl;

	CObject *_openDoor, *_closedDoor;
	CObject *_globGlow;
	SuperFX *s2GlobHalfGlowFX;

public:
	float musicFadeOutTimer;

	L7_S2_HOBrokenRightHand();
	~L7_S2_HOBrokenRightHand();

	void Update();
	void Required();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	std::vector<CPolygon*> getPolyList();

	void onSymbolComplete(int index);
	bool getIsSymbolCompleted(int index);
};

#endif
