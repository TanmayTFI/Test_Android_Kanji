#ifndef DQFYLH_LEVEL4_SCENE4_POP_CHART_DEF
#define DQFYLH_LEVEL4_SCENE4_POP_CHART_DEF

#include "Popup.h"
#include "MultilineTextBox.h"

class L4S4_PopInstruction: public Popup
{
	public:
		L4S4_PopInstruction(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);

		~L4S4_PopInstruction();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();

		float easing(float t,float b,float c,float d);
		MultilineTextBox* olives, *strainer, *cup, *useforfire, *grinder;

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif