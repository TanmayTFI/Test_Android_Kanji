#ifndef DQFYLH_LEVEL4_SCENE5_POP1_DOOR_DEF
#define DQFYLH_LEVEL4_SCENE5_POP1_DOOR_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L4S5_Pop1Door: public Popup
{
	JSONAnimator *Janim_screwRemoval;
	CPolygon *TwigRect,*ClothRect;
public:

	L4S5_Pop1Door(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L4S5_Pop1Door();

	void ClosePopup(float delayTime = 0);
	void Update(float ds );
	void Required();

	float easing(float t,float b,float c,float d);
	void HandleInventoryDrop(CPoint dropPos, int itemVal);
//	void L4S5_Pop1Door::OnAnimComplete(int animID); // there is a extra qualification error in linux
	void OnAnimComplete(int animID);
protected:
	void ProcessHint();
	bool isTaskLeft();
};

#endif