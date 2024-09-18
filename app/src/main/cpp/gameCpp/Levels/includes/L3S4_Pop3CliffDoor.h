

#ifndef DQFYLH_L3_S4_POP3_CLIFFDOOR_DEF
#define DQFYLH_L3_S4_POP3_CLIFFDOOR_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L3S4_Pop3CliffDoor : public Popup
{
public:
	L3S4_Pop3CliffDoor(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3S4_Pop3CliffDoor();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void OnAnimationComplete(int animID);
	JSONAnimator *JAnim_NagaLockPlace, *JAnim_DoorOpening;

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

};

#endif
