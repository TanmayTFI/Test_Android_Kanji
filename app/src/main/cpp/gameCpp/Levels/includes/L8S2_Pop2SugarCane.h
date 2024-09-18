

#ifndef DQFYLH_LEVEL8_SCENE2_POP2_SUGARCANE_DEF
#define DQFYLH_LEVEL8_SCENE2_POP2_SUGARCANE_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L8S2_Pop2SugarCane: public Popup
{
public:
	L8S2_Pop2SugarCane(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L8S2_Pop2SugarCane();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

	JSONAnimator *CaneCutAnim, *CaneGrowAnim;

protected:
	std::vector<SuperFX*> superfxArray;
	SuperFX *S2_SugarCaneGrowFX;

	void OnAnimationComplete(int index);
	void InitSceneElements();
};

#endif
