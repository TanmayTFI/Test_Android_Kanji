

#ifndef L3_S2_P1R3PipePopup_
#define L3_S2_P1R3PipePopup_

#include "Popup.h"

class L3_S2_P1R3PipePopup : public Popup
{
	JSONAnimator *_janim_StonesRemove;

public:
	L3_S2_P1R3PipePopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	virtual ~L3_S2_P1R3PipePopup(void);

	void HandleInventoryDrop(CPoint dropPos, int itemVal);
private:

	void OnAnimationComplete(int animId);
	void Update(float ds);
	bool isTaskLeft();
	void ProcessHint();
	void OnPopupCloseBtnClicked();
};

#endif