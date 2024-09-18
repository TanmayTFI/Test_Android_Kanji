#ifndef DQFYLH_LEVEL8_SCENE1_POP_PUZZLE_DEF
#define DQFYLH_LEVEL8_SCENE1_POP_PUZZLE_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L8S3_Pop1CrystalKey: public Popup
{
	JSONAnimator *Janim_DoorOpen;
	public:

		L8S3_Pop1CrystalKey(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L8S3_Pop1CrystalKey();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		bool isTaskLeft();
		void ProcessHint();
};

#endif