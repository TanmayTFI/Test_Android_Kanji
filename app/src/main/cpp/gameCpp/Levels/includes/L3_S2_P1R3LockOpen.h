
#ifndef L3_S2_P1R3LockOpen_
#define L3_S2_P1R3LockOpen_

#include "Popup.h"

class L3_S2_P1R3LockOpen : public Popup
{
	JSONAnimator *_janim_LockBreak;

	C_SYNTHESIZE(int, _doorIndex, DoorIndex);

public:

	L3_S2_P1R3LockOpen(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	virtual ~L3_S2_P1R3LockOpen(void);

	void HandleInventoryDrop(CPoint dropPos, int itemVal);
	bool isTaskLeft();
	void ProcessHint();

private:

	void OnAnimationComplete(int animId);
	void Update(float ds);
};

#endif