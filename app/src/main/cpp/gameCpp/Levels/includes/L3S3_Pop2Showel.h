#ifndef DQFYLH_L2_S3_POP2_SHOWEL_DEF
#define DQFYLH_L2_S3_POP2_SHOWEL_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L3S3_Pop2Showel: public Popup
{
	public:
		JSONAnimator *JAnim_SawAnim1, *JAnim_SawAnim2;

		L3S3_Pop2Showel(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S3_Pop2Showel();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnAnimationComplete(int animID);

		void Update(float ds );
		void Required();

		void ProcessHint();
		bool isTaskLeft();

	protected:
};

#endif