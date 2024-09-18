#ifndef DQFYLH_LEVEL6_SCENE2_POP2_DOOR_DEF
#define DQFYLH_LEVEL6_SCENE2_POP2_DOOR_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L6S2_Pop2Door: public Popup
{
	JSONAnimator *Janim_doorOpen;
	bool isAnimPlaying;
	public:
		L6S2_Pop2Door(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S2_Pop2Door();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif