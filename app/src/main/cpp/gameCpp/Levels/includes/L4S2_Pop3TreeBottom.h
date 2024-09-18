#ifndef DQFYLH_LEVEL4_SCENE2_POP3_TREE_BOTTOM_DEF
#define DQFYLH_LEVEL4_SCENE2_POP3_TREE_BOTTOM_DEF

#include "Popup.h"


class L4S2_Pop3TreeBottom: public Popup
{
	CObject * _twig;
	CObject * _fruit;
	JSONAnimator * _jsonAnimFruitEat;
	SuperFX * _fruitEnergyFX;
	public:

		L4S2_Pop3TreeBottom(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S2_Pop3TreeBottom();

		void ClosePopup(float delayTime = 0);
		void OpenPopup();
		void Update(float ds );
		void Required();

		float easing(float t,float b,float c,float d);

		void InitializeWithProfileData();
		void OnObjectClick(CObject * stick);
		void OnFruitEatAnimComplete(int id);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif