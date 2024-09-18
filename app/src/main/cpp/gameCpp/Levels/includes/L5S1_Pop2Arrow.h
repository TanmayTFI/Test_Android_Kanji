

#ifndef DQFYLH_L5_S1_POP2ARROW_DEF
#define DQFYLH_L5_S1_POP2ARROW_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L5S1_Pop2Arrow : public Popup
{
public:
	L5S1_Pop2Arrow(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S1_Pop2Arrow();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

};

#endif
