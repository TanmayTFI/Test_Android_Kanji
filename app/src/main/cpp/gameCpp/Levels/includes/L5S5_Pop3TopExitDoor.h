

#ifndef DQFYLH_L5_S5_POP3TOPEXITDOOR_DEF
#define DQFYLH_L5_S5_POP3TOPEXITDOOR_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L5S5_Pop3TopExitDoor : public Popup
{
public:
	L5S5_Pop3TopExitDoor(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S5_Pop3TopExitDoor();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

	SuperFX* dustVFX;

	void OnAnimationComplete(int animID);

	bool isDoorOpenAnimPlaying;	

protected:
	JSONAnimator *_JAnim_DoorOpen;
};

#endif
