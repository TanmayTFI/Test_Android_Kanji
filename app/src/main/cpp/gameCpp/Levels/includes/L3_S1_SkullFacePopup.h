


#ifndef L3_S1_SkullFace_Popup_
#define L3_S1_SkullFace_Popup_

#include "Popup.h"
#include "PopupInterface.h"
#include <map>
#include <stack>

using namespace std;

enum SkullPopup_States
{
	L3_Skull_Initial = 0,
	L3_Skull_EyeFixed,
	L3_Skull_CrystalCollected,
	L3_Skull_OnAnimate,
	L3_Skull_AnimComplete
};

enum L3_SkullPopupProperties
{
	S1_P_GemDrop = 0,
	S1_P_MouthOpen,
	S1_P_EyeFixed,
	S1_P_CrystalInMouth
};

class L3_S1_SkullFacePopup : public Popup
{
	map<L3_SkullPopupProperties, CObject*> _popupProperties;
	stack<SkullPopup_States> _stateMachine;

	JSONAnimator *_janim_PlaceEye;

	SkullPopup_States _state;
	SkullPopup_States _nextState;

	C_SYNTHESIZE(PopupInterface*, _interface, Interface);

public:
	L3_S1_SkullFacePopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3_S1_SkullFacePopup(void);

	void SavedState(bool State);
	void OpenPopup();
	void HandleInventoryDrop(CPoint dropPos, int itemVal);

protected:

	void Update(float ds);
	void OnPopupCloseBtnClicked();

private:

	void HandleProperties();
	void OnAnimationComplete(int animID);
	void ProcessHint();
	bool isTaskLeft();
};

#endif