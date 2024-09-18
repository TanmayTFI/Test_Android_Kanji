

#ifndef L3_S1_Ember_Popup_
#define L3_S1_Ember_Popup_

#include "Popup.h"
#include "PopupInterface.h"
#include <map>
#include <stack>

using namespace std;

enum L3_EmberPopupProperties
{
	L3_P_Flame = 0,
	L3_P_FlameGlowMask,
	L3_P_LaddedStick,
	L3_P_CharcolPart1,
	L3_P_CharcolPart2,
	L3_P_CharcolPart3,
	L3_P_CharcolCollect,
	L3_P_CharcolShadow,
	L3_P_PuzzleDoorClose,
	L3_P_PuzzleDoorOpen,
	L3_P_Mug,
	L3_P_Shovel,
	L3_P_Mirror
};

enum L3_EmberState
{
	L3_Ember_NotStart = 0,
	L3_Ember_OffTheFlames,
	L3_Ember_CollectedCharcol,
	L3_Ember_RemovedCharcols,
	L3_Ember_PuzzleDoorOpened,
	L3_Ember_OnAnimation,
	L3_Ember_OnAnimationEnd,
	L3_Ember_Closed
};

struct L3_InvDropItem
{
public:
	bool haveItem;
	CPoint dropPos;
	int itemVal;
};

class L3_S1_EmberPopup : public Popup
{
	L3_EmberState _state;
	L3_EmberState _nextState;
	L3_InvDropItem _invDropItem;
	SuperFX *FireFogOnPopupFX;
	vector<SuperFX*> fxVector;

	JSONAnimator *_janim_OffTheFlame;
	JSONAnimator *_janim_RemoveCharcol;

	int _sceneObjective;

	stack<L3_EmberState> _stateMachine;
	map<L3_EmberPopupProperties, CObject*> _popupProperties;

	C_SYNTHESIZE(PopupInterface*, _interface, Interface);

public:

	L3_S1_EmberPopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3_S1_EmberPopup(void);
	
	void OpenPopup();
	void HandleInventoryDrop(CPoint dropPos, int itemVal);
	void OnAnimatonComplete(int animID);

protected:

	void Required();
	void Update(float ds);

	void OnPopupOpened();
	void OnPopupCloseBtnClicked();
	void OnScaleDownComplete();

private:

	bool isTaskLeft();
	void ProcessHint();
	float easing(float t,float b,float c,float d);
	void HandleProperties();

	void OnStateChange();

};

#endif