

#ifndef DQFYLH_L5_S5_POP4SACK_DEF
#define DQFYLH_L5_S5_POP4SACK_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "JSONAnimator.h"

class L5S5_Pop4Sack : public Popup
{
public:
	L5S5_Pop4Sack(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S5_Pop4Sack();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

	void OnAnimationComplete(int animID);

protected:
	SuperFX *s5sacksmokeFX, *s5sackdustFX;
	JSONAnimator* _JAnim_SackFall;
};

#endif
