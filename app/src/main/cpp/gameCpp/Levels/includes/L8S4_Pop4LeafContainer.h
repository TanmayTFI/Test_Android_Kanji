

#ifndef DQFYLH_LEVEL8_SCENE4_POP4_LEAF_CONTAINER_DEF
#define DQFYLH_LEVEL8_SCENE4_POP4_LEAF_CONTAINER_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "JSONAnimator.h"

class L8S4_Pop4LeafContainer: public Popup
{
public:
	L8S4_Pop4LeafContainer(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L8S4_Pop4LeafContainer();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();
protected:

	
	JSONAnimator *ContainerClosing;
	JSONAnimator *ContainerOpen;
	
	
	void InitAnimations();
	void OnAnimationComplete(int index);
};

#endif
