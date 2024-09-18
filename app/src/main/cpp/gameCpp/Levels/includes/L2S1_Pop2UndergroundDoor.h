#ifndef DQFYLH_LEVEL2_SCENE1_POP2_UNDERGROUNDDOOR_DEF
#define DQFYLH_LEVEL2_SCENE1_POP2_UNDERGROUNDDOOR_DEF

#include "Popup.h"
#include "SuperFX.h"

class L2S1_Pop2UndergroundDoor: public Popup
{
	public:
		
		HudControl *Hud;

		//objects
		CObject *doorClosed;
		CObject *doorHalfOpen;
		CObject *doorFullOpen;
		CObject *nagaSculpture1;
		CObject *nagaSculpture2;

		bool isToUndergroundAreaClicked;

		SuperFX *fireVFX;

		L2S1_Pop2UndergroundDoor(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S1_Pop2UndergroundDoor();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void Update(float ds);
		void Required();

		void OnPopupCloseBtnClicked();
		void OnPopupOpened();

		protected:
			
		void ProcessHint();
		bool isTaskLeft();
		void OnFirstTweenComplete(CObject *obj);
		void OnSecondTweenComplete(CObject *obj);
		void OnThirdTweenComplete(CObject *obj);
};

#endif