#ifndef DQFYLH_LEVEL6_SCENE1_POP4_SANDALS_DEF
#define DQFYLH_LEVEL6_SCENE1_POP4_SANDALS_DEF

#include "Popup.h"
#include "L6S1_PopAnanthaNote.h"
#include "CVideo.h"

class L6S1_Pop4StatueSandals: public Popup
{
	
	CPolygon *notePoly;
	/*CVideo *lpVidVaruniCutScn;

	bool isVideoPlaying;*/

	bool isAmeehaNotePopupActive;
	public:

		L6S1_Pop4StatueSandals(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S1_Pop4StatueSandals();
		L6S1_PopAnanthaNote *popUp;
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