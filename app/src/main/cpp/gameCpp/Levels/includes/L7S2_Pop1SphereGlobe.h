

#ifndef DQFYLH_L7S2_POP1_SPHEREGLOBE_DEF
#define DQFYLH_L7S2_POP1_SPHEREGLOBE_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "JSONAnimator.h"

class L7S2_Pop1SphereGlobe : public Popup
{
public:

	SuperFX *s2spherepopFX;

	L7S2_Pop1SphereGlobe(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L7S2_Pop1SphereGlobe();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	JSONAnimator *JAnim_GlobePlace;
	void OnAnimationComplete(int id);

	void ProcessHint();
	bool isTaskLeft();

protected:

};

#endif
