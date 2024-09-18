#ifndef DQFYLH_LEVEL8_SCENE1_POP3_DOOR_DEF
#define DQFYLH_LEVEL8_SCENE1_POP3_DOOR_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L8S1_Pop3DoorPopup: public Popup
{
	JSONAnimator *Janim_doorOpen;
	public:

		L8S1_Pop3DoorPopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L8S1_Pop3DoorPopup();


		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);
		void ProcessSkip();
		void OnAnimComplete(int animID);

	protected:
		void OnPopupOpened();
		void ProcessHint();
		bool isTaskLeft();
};

#endif