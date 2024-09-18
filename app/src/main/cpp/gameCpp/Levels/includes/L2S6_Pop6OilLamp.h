#ifndef DQFYLH_LEVEL2_SCENE6_POP6_OILLAMP_DEF
#define DQFYLH_LEVEL2_SCENE6_POP6_OILLAMP_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L2S6_Pop6OilLamp: public Popup
{
	public:

		//scene objects
		JSONAnimator *JAnim_fireAnim;

		L2S6_Pop6OilLamp(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S6_Pop6OilLamp();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void Update(float ds);
		void Required();

		float easing(float t,float b,float c,float d);

		protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif