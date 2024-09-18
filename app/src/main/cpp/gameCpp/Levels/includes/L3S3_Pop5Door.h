#ifndef DQFYLH_L3_S3_POP5_DOOR_DEF
#define DQFYLH_L3_S3_POP5_DOOR_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L3S3_Pop5Door: public Popup
{
	public:
		JSONAnimator *JAnim_DoorOpening;

		L3S3_Pop5Door(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S3_Pop5Door();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnAnimationComplete(int animID);

		void Update(float ds );
		void Required();

		void ProcessHint();
		bool isTaskLeft();

	protected:
};

#endif