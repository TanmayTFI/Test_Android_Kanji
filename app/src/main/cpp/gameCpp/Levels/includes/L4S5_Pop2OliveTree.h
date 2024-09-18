#ifndef DQFYLH_LEVEL4_SCENE5_POP2_OLIVETREE_DEF
#define DQFYLH_LEVEL4_SCENE5_POP2_OLIVETREE_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L4S5_Pop2OliveTree: public Popup
{
	JSONAnimator *Janim_OlivePlucking;
public:

	L4S5_Pop2OliveTree(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L4S5_Pop2OliveTree();

	void ClosePopup(float delayTime = 0);
	void Update(float ds );
	void Required();

	float easing(float t,float b,float c,float d);

	void HandleInventoryDrop(CPoint dropPos, int itemVal);
	void OnAnimComplete(int animID);
protected:
	void ProcessHint();
	bool isTaskLeft();
};

#endif