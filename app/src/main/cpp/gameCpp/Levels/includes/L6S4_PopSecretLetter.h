#ifndef DQFYLH_LEVEL6_SCENE4_POP_SECRET_LETTER_DEF
#define DQFYLH_LEVEL6_SCENE4_POP_SECRET_LETTER_DEF

#include "Popup.h"

class L6S4_PopSecretLetter: public Popup
{
	public:

		L6S4_PopSecretLetter(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S4_PopSecretLetter();

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