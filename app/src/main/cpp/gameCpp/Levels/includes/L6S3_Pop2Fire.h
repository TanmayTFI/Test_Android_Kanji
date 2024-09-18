#ifndef DQFYLH_LEVEL6_SCENE3_POP2_FIRE_DEF
#define DQFYLH_LEVEL6_SCENE3_POP2_FIRE_DEF

#include "Popup.h"
#include "JSONAnimator.h"
#include "CVideo.h"

class L6S3_Pop2Fire: public Popup
{
	JSONAnimator *Janim_RubForFire;
	CVideo *lpVidroomlighteningCutScn;
	float vidTimer;
	
	public:

		L6S3_Pop2Fire(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S3_Pop2Fire();
		bool isVideoPlaying;
		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif