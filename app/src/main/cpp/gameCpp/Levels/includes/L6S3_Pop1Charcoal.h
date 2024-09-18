#ifndef DQFYLH_LEVEL6_SCENE3_POP1_CHARCOAL_DEF
#define DQFYLH_LEVEL6_SCENE3_POP1_CHARCOAL_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L6S3_Pop1Charcoal: public Popup
{
	JSONAnimator *Janim_charcoalHit, *Janim_charcoalTake;
	SuperFX *S3SmokeFX;

	public:

		L6S3_Pop1Charcoal(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S3_Pop1Charcoal();

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