#ifndef DQFYLH_LEVEL4_SCENE1_POP4_CRUTCH_DEF
#define DQFYLH_LEVEL4_SCENE1_POP4_CRUTCH_DEF

#include "Popup.h"


class L4S1_Pop4Crutch: public Popup
{
	public:

		CObject *Crutch;

		L4S1_Pop4Crutch(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S1_Pop4Crutch();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();

		float easing(float t,float b,float c,float d);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif