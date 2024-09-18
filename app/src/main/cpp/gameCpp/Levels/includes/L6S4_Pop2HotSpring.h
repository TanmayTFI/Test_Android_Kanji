#ifndef DQFYLH_LEVEL6_SCENE4_POP3_HOTSPRING_DEF
#define DQFYLH_LEVEL6_SCENE4_POP3_HOTSPRING_DEF

#include "Popup.h"
#include "MeshObject.h"
#include "JSONAnimator.h"
#include "SuperFX.h"

class L6S4_Pop2HotSpring: public Popup
{
	MeshObject *mo_water;
	SuperFX *s4_HotSpringPopupBubblesFX;
	JSONAnimator *Janim_ice_melt,*Janim_keyTake;
	SuperFX *SnowFX_L;
	bool isCollectingInventory;
	public:

		L6S4_Pop2HotSpring(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S4_Pop2HotSpring();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		void OnPopupOpened();

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif