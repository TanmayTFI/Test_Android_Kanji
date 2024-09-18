#ifndef DQFYLH_L2_S4_POP2_FIRE_DEF
#define DQFYLH_L2_S4_POP2_FIRE_DEF

#include "Popup.h"
#include "SuperLog.h"

#include "JSONAnimator.h"

class L2S4_Pop2FirePop: public Popup
{
	public:
		L2S4_Pop2FirePop(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S4_Pop2FirePop();	

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnAnimationComplete(int animID);

		void Update(float ds );
		void Required();

		void ProcessHint();
		bool isTaskLeft();

		void OnPopupCloseBtnClicked();
		void OnPopupOpened();

		bool isBannerShown;

	protected:
		JSONAnimator *JAnim_FireTorch;		
};

#endif