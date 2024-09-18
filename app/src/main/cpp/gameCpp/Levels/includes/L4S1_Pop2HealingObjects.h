#ifndef DQFYLH_LEVEL4_SCENE1_POP2_LEAF_DEF
#define DQFYLH_LEVEL4_SCENE1_POP2_LEAF_DEF

#include "Popup.h"
#include "MeshObject.h"
#include "JSONAnimator.h"
#include "L4S1_PopLeafChewing.h"

class L4S1_Pop2HealingObjects: public Popup
{
	public:
		bool popUpActive, isAnimPlaying;

		MeshObject *mo_water;
		JSONAnimator *Janim_skimmerFly, *Janim_takeWaterWithSpoon;
		L4S1_PopLeafChewing *popLeafChew;

		CPolygon *WaterRect, *StickPoly, *PlantPoly;

		CObject *skimmerBody, *skimmerWings;
		CObject *spoonWithWater, *spoonWithOutWater;

		L4S1_Pop2HealingObjects(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S1_Pop2HealingObjects();

		void Update(float ds );
		void Required();
		void ClosePopup(float delayTime);
		void OnAnimComplete(int animID);

		float easing(float t,float b,float c,float d);

		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void OnPopupClosed(Popup * popup);

	protected:

		void ReceiveMessage(int val);
		
		void ProcessHint();
		bool isTaskLeft();
};

#endif