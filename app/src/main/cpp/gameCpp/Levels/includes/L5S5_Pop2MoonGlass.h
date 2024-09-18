

#ifndef DQFYLH_L5_S5_MOONGLASS_DEF
#define DQFYLH_L5_S5_MOONGLASS_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "JSONAnimator.h"

class L5S5_Pop2MoonGlass : public Popup
{
public:
	L5S5_Pop2MoonGlass(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S5_Pop2MoonGlass();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:
	JSONAnimator *JAnim_HammerHit;
	void OnAnimationComplete(int animID);
};

#endif
