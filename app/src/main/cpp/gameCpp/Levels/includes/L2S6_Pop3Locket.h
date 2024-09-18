#ifndef DQFYLH_LEVEL2_SCENE6_POP3_LOCKET_DEF
#define DQFYLH_LEVEL2_SCENE6_POP3_LOCKET_DEF

#include "Popup.h"

class L2S6_Pop3Locket: public Popup
{
	CPolygon *grandma,*grandpa;
	public:

		//scene objects

		CObject *closedLocket;
		CObject *openedLocket;


		L2S6_Pop3Locket(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S6_Pop3Locket();

		void Update(float ds);
		void Required();
		//void HandleMouseInput(CInput * mouseInput);

		protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif