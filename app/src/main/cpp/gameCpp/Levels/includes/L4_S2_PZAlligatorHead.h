//====================================
// DQFYLH
// Hidden Object System
// Level 4 - Scene 02 : PUZZLE ALLIGATOR HEAD
//====================================

#ifndef DQFYLH_LEVEL4_SCENE2_PZALLIGATORHEAD_DEF
#define DQFYLH_LEVEL4_SCENE2_PZALLIGATORHEAD_DEF

#include "HiddenObjectSystem.h"

class L4_S2_PZAlligatorHead: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements
		

		//Popups
		
		L4_S2_PZAlligatorHead();
		~L4_S2_PZAlligatorHead();

	

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);
		void Update();
		void Required();

		float easing(float t,float b,float c,float d);
		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif
