#ifndef DQFYLH_LEVEL8_SCENE1_POP1_MAGIC_WAND_DEF
#define DQFYLH_LEVEL8_SCENE1_POP1_MAGIC_WAND_DEF

#include "Popup.h"
#include "L8S1_PopRisingPheonix.h"

class L8S1_Pop1MagicWand: public Popup
{
	bool isPopupActive;
	bool isCollectingInventory;
	L8S1_PopRisingPheonix *popUp;
	CPolygon *magicWandRect;
	public:

		L8S1_Pop1MagicWand(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L8S1_Pop1MagicWand();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		void OnPopupClosed(Popup * popup);
		void ReceiveMessage(int val);
	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif