#ifndef DQFYLH_LEVEL8_SCENE1_POP_NOTE_DEF
#define DQFYLH_LEVEL8_SCENE1_POP_NOTE_DEF

#include "Popup.h"

class L8S1_PopRisingPheonix: public Popup
{
	public:

		L8S1_PopRisingPheonix(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L8S1_PopRisingPheonix();

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