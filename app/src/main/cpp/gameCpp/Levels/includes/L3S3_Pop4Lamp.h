#ifndef DQFYLH_L3_S3_POP4_LAMP_DEF
#define DQFYLH_L3_S3_POP4_LAMP_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L3S3_Pop4Lamp: public Popup
{
	public:
		JSONAnimator *JAnim_GlassBreaking;
		SuperFX *OilDropsVFX;

		L3S3_Pop4Lamp(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S3_Pop4Lamp();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnAnimationComplete(int animID);

		void Update(float ds );
		void Required();

		void ProcessHint();
		bool isTaskLeft();

	protected:
};

#endif