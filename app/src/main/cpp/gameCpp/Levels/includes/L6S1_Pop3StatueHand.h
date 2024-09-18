#ifndef DQFYLH_LEVEL6_SCENE1_POP3_STATUE_HAND_DEF
#define DQFYLH_LEVEL6_SCENE1_POP3_STATUE_HAND_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L6S1_Pop3StatueHand: public Popup
{
	JSONAnimator *Janim_magnify;
	CPolygon *ringRect;
	public:

		L6S1_Pop3StatueHand(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S1_Pop3StatueHand();

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