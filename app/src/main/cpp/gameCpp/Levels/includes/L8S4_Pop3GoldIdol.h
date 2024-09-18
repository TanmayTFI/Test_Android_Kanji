

#ifndef DQFYLH_LEVEL8_SCENE4_POP3_GOLD_IDOL_DEF
#define DQFYLH_LEVEL8_SCENE4_POP3_GOLD_IDOL_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L8S4_Pop3GoldIdol: public Popup
{
public:
	L8S4_Pop3GoldIdol(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L8S4_Pop3GoldIdol();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:
		SuperFX *S4_IdolPlaceFX;
};

#endif
