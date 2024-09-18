#ifndef DQFYLH_LEVEL2_SCENE5_POP3_EMBER_DEF
#define DQFYLH_LEVEL2_SCENE5_POP3_EMBER_DEF

#include "Popup.h"

class L2_S5_MoopansDen;

class L2S5_Pop3Ember: public Popup
{
	public:
		L2S5_Pop3Ember(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);

		~L2S5_Pop3Ember();
		
		void Update(float ds);
		void Required();
		void ProcessHint();

		float easing(float t,float b,float c,float d);
		
		

	protected:

		L2_S5_MoopansDen *parentScene;
		void ReceiveMessage(int val);
		bool isTaskLeft();
		
};

#endif