#ifndef DQFYLH_LEVEL2_SCENE6_POP2_CREEKSSKELETON_DEF
#define DQFYLH_LEVEL2_SCENE6_POP2_CREEKSSKELETON_DEF

#include "Popup.h"
#include "ClickablePolygon.h"
#include "L2S6_Pop3Locket.h"
#include "L2S6_Pop4Holder.h"

using namespace std;

class L2S6_Pop2CreeksSkeleton: public Popup
{
	public:

		int ActivePopUpID;
		bool isMouseOverPopup;

		//scene objects
		CObject *MemSpectacles;
		ClickablePolygon *pop3ClickPoly;
		ClickablePolygon *pop4ClickPoly;

		L2S6_Pop3Locket *pop3Locket;
		L2S6_Pop4Holder *pop4Holder;

		Popup *PopUpArray[2];

		L2S6_Pop2CreeksSkeleton(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S6_Pop2CreeksSkeleton();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void Update(float ds);
		void Required();
		void onPopAreaClicked(int popId);
		void onPopupClosed(Popup * popup);
		void onPopAreaMouseOver(int popId);
		void onPopAreaMouseOut(int popId);

		float easing(float t,float b,float c,float d);

		protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif