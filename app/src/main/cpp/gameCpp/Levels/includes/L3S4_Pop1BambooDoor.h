

#ifndef DQFYLH_L3_S4_POP1_BAMBOODOOR_DEF
#define DQFYLH_L3_S4_POP1_BAMBOODOOR_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L3S4_Pop1BambooDoor : public Popup
{
public:
	L3S4_Pop1BambooDoor(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3S4_Pop1BambooDoor();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void OnAnimationComplete(int animID);
	JSONAnimator *JAnim_BambooCut;

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

};

#endif
