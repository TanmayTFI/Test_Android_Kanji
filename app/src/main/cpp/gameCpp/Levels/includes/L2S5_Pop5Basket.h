#ifndef DQFYLH_LEVEL2_SCENE5_POP5_BASKET_DEF
#define DQFYLH_LEVEL2_SCENE5_POP5_BASKET_DEF

#include "Popup.h"

class L2S5_Pop5Basket: public Popup
{
	public:
		L2S5_Pop5Basket(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S5_Pop5Basket();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds );
		void Required();

		bool isBasketMoveAnimPlaying;


		CustomAnimGroup *CAnim_BasketMove;
		CObject *Basket;

		float easing(float t,float b,float c,float d);
		void ProcessHint();

	protected:
		bool isTaskLeft();
		void ReceiveMessage(int val);
		
};

#endif