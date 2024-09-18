#ifndef DQFYLH_LEVEL8_SCENE3_POP2_HIDDEN_COMPARTMENT_DEF
#define DQFYLH_LEVEL8_SCENE3_POP2_HIDDEN_COMPARTMENT_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L8S3_Pop2HiddenCompartment: public Popup
{
	JSONAnimator *janim_Floor;
	public:

		L8S3_Pop2HiddenCompartment(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L8S3_Pop2HiddenCompartment();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		void OnPopupOpened();
	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif