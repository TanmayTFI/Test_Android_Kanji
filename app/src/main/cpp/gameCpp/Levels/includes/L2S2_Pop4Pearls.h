#ifndef DQFYLH_LEVEL2_SCENE2_POP4_PEARLS_DEF
#define DQFYLH_LEVEL2_SCENE2_POP4_PEARLS_DEF

#include "Popup.h"

class L2S2_Pop4Pearls: public Popup
{
	public:
		L2S2_Pop4Pearls(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S2_Pop4Pearls();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();

		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();

	protected:

		void ReceiveMessage(int val);
		void ProcessHint();
};

#endif