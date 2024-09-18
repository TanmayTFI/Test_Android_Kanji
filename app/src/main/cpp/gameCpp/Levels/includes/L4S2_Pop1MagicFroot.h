#ifndef DQFYLH_LEVEL4_SCENE2_POP1_MAGIC_FROOT_DEF
#define DQFYLH_LEVEL4_SCENE2_POP1_MAGIC_FROOT_DEF


#include "Popup.h"
#include "ClickablePolygon.h"

class L4S2_Pop1MagicFroot: public Popup
{
	ClickablePolygon * _bambooSpearDropPolygon;
	JSONAnimator * _jsonAnimFruitCut;
	
	
	bool _isMouseInteractionEnabled;
	public:

		L4S2_Pop1MagicFroot(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S2_Pop1MagicFroot();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void OnFruitCutAnimComplete(int id);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void OnFruitAreaClick(int id);

		float easing(float t,float b,float c,float d);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif