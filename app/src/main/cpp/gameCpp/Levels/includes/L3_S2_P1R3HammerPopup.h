
#ifndef L3_S2_P1R3HammerPopup_
#define L3_S2_P1R3HammerPopup_

#include "Popup.h"

class L3_S2_P1R3HammerPopup : public Popup
{
public:
	L3_S2_P1R3HammerPopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	virtual ~L3_S2_P1R3HammerPopup(void);

	bool isTaskLeft();
	void ProcessHint();

private:

	void Update(float ds);
};

#endif