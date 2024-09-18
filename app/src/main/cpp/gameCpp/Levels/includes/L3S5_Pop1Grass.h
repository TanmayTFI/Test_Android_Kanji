#ifndef DQFYLH_LEVEL3_SCENE5_POP1_GRASS_DEF
#define DQFYLH_LEVEL3_SCENE5_POP1_GRASS_DEF

#include "Popup.h"

class L3S5_Pop1Grass: public Popup
{
	public:
		
		L3S5_Pop1Grass(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S5_Pop1Grass();

		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_grass1, *JAnim_grass2, *JAnim_grass3, *JAnim_grass4, *JAnim_grass5, *JAnim_grass6, *JAnim_grass7, *JAnim_grass8 , *JAnim_grass9, *JAnim_grass10, *JAnim_grass11;
		void OnAnimationComplete(int animID);

		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();
		void CheckForAllGrassTaken();
	protected:

		void ReceiveMessage(int val);
		void ProcessHint();

};

#endif