#ifndef DQFYLH_LEVEL4_SCENE5_POP5_BRIDGE_DEF
#define DQFYLH_LEVEL4_SCENE5_POP5_BRIDGE_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L4S5_Pop5Bridge : public Popup
{
public:
	L4S5_Pop5Bridge(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L4S5_Pop5Bridge();

	void ClosePopup(float delayTime = 0);
	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

};

#endif