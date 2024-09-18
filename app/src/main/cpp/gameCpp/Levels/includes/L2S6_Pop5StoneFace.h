#ifndef DQFYLH_LEVEL2_SCENE6_POP5_STONEFACE_DEF
#define DQFYLH_LEVEL2_SCENE6_POP5_STONEFACE_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L2S6_Pop5StoneFace: public Popup
{
	public:

		JSONAnimator *JAnim_stoneBall;

		L2S6_Pop5StoneFace(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S6_Pop5StoneFace();

		void TakeInventoryItem(EHOInventory ItemID);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void Update(float ds);
		void Required();
		void OnPopupOpened();
		void OnPopupCloseBtnClicked();

		float easing(float t,float b,float c,float d);
		void OnAnimationComplete(int animID);

		protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif