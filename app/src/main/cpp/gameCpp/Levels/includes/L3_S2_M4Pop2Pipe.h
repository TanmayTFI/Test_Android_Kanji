
#ifndef DQFYLH_L3_S2_MAZE4_POP2_DEF
#define DQFYLH_L3_S2_MAZE4_POP2_DEF

//====================================
//  created by : Faisal Rasak
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L3_S2_M4Pop2Pipe : public Popup
{
public:
	L3_S2_M4Pop2Pipe(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3_S2_M4Pop2Pipe();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

};

#endif
