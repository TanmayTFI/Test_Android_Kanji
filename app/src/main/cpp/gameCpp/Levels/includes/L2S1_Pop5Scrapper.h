#ifndef DQFYLH_LEVEL2_SCENE1_POP5_SRAPTER_DEF
#define DQFYLH_LEVEL2_SCENE1_POP5_SRAPTER_DEF

#include "Popup.h"

class L2S1_Pop5Scrapper: public Popup
{
	public:

		//scene objects

		CObject *Scrapper;

		L2S1_Pop5Scrapper(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);

		void TakeInventoryItem(EHOInventory ItemID);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void Update(float ds);
		void Required();

		protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif