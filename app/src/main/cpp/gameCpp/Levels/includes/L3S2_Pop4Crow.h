#ifndef DQFYLH_LEVEL3_SCENE2_POP4_CROW_DEF
#define DQFYLH_LEVEL3_SCENE2_POP4_CROW_DEF

#include "Popup.h"

class L3S2_Pop4Crow: public Popup
{
	public:
		L3S2_Pop4Crow(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S2_Pop4Crow();

		bool crowAnim1Playing;
		bool crowAnim2Playing;

		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_Lock;
		void OnAnimationComplete(int animID);

		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();

	protected:

		void ReceiveMessage(int val);
		void ProcessHint();

};

#endif