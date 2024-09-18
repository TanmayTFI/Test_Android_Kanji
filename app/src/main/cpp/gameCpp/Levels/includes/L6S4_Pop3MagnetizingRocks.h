#ifndef DQFYLH_LEVEL6_SCENE4_POP3_ROCKS_DEF
#define DQFYLH_LEVEL6_SCENE4_POP3_ROCKS_DEF

#include "Popup.h"
#include "JSONAnimator.h"
//#include "L6S4_PopAmeehaNote.h"
#include "L6S1_PopNaginNote.h"

class L6S4_Pop3MagnetizingRocks: public Popup
{
	JSONAnimator *Janim_rockBreaking, *Janim_magnetiseHorseshoe;
	int activePopupId;
	L6S1_PopNaginNote *popAmeehaNote;
	CPolygon *letterPoly,*stonePoly;
	public:

		L6S4_Pop3MagnetizingRocks(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S4_Pop3MagnetizingRocks();

		CObject* naginLetter;

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		void OnPopupClosed(Popup * popup);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif