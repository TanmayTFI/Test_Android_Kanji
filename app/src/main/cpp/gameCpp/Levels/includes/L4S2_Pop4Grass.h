#ifndef DQFYLH_LEVEL4_SCENE2_POP4_GRASS_DEF
#define DQFYLH_LEVEL4_SCENE2_POP4_GRASS_DEF

#include "Popup.h"


class L4S2_Pop4Grass: public Popup
{

	CPolygon * _knifeDropPolygon;
	JSONAnimator * _jsonAnimGrassCut;
	CObject * _grass;
	bool _isMouseInteractionEnabled;
	public:

		L4S2_Pop4Grass(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S2_Pop4Grass();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();

		float easing(float t,float b,float c,float d);
		void OnGrassCutAnimComplete(int id);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif