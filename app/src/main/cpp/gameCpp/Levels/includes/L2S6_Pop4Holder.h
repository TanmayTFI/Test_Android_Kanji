#ifndef DQFYLH_LEVEL2_SCENE6_POP4_HOLDER_DEF
#define DQFYLH_LEVEL2_SCENE6_POP4_HOLDER_DEF

#include "Popup.h"

class L2S6_Pop4Holder: public Popup
{
	public:
		
		std::vector<int> vectorObject[3];
		std::vector<int>::iterator p;

		int FirstDigit;
		int SecondDigit;
		int ThirdDigit;
		int currentState;
		int tappedStrip;
		int counter;

		int headPosX[3], headPosY[3], headPosZ[3];

		//scene objects

		L2S6_Pop4Holder(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S6_Pop4Holder();

		void Update(float ds);
		void Required();
		void OnTweenComplete(CObject *obj);

		protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif