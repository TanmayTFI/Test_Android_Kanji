#ifndef DQFYLH_LEVEL4_SCENE1_POP5_TWIG_SQUARE_DEF
#define DQFYLH_LEVEL4_SCENE1_POP5_TWIG_SQUARE_DEF

#include "Popup.h"
#include "MeshObject.h"
#include "JSONAnimator.h"

class L4S1_Pop5TwigSquarePopup: public Popup
{
	public:
		MeshObject *mo_water;//, *mo_twigs, *mo_locket;
		JSONAnimator *Janim_twigMoving;

		L4S1_Pop5TwigSquarePopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S1_Pop5TwigSquarePopup();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void OnTwigMovingAnimComplete(int animID);

		float easing(float t,float b,float c,float d);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif