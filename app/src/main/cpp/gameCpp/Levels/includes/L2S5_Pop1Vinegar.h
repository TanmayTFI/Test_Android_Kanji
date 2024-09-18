#ifndef DQFYLH_LEVEL2_SCENE5_POP1_VINEGAR_DEF
#define DQFYLH_LEVEL2_SCENE5_POP1_VINEGAR_DEF

#include "Popup.h"
#include "SuperLog.h"

class L2_S5_MoopansDen;

class L2S5_Pop1Vinegar: public Popup
{
	public:

		L2S5_Pop1Vinegar(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S5_Pop1Vinegar();

		CObject *MemStethescope;
		CObject *InvVinegarBottle;
		CObject *InvStoneSharpner;


		SuperFX *PopupDust;
		CPolygon *VinegarPoly;
		CPolygon *StethPoly;

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();

		void OnPopupOpened();
		void ProcessHint();

		

		float easing(float t,float b,float c,float d);

	protected:
		bool isTaskLeft();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif