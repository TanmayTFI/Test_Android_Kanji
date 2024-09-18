#ifndef DQFYLH_LEVEL6_SCENE2_POP1_ICE_DEF
#define DQFYLH_LEVEL6_SCENE2_POP1_ICE_DEF

#include "Popup.h"

class L6S2_Pop1Ice: public Popup
{
	bool isCollectingInventory;
	CPolygon *pestleRect, *iceCubeRect;
	public:

		L6S2_Pop1Ice(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S2_Pop1Ice();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		void ReceiveMessage(int val);
	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif