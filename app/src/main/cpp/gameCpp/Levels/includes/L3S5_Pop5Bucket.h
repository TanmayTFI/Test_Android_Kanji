#ifndef DQFYLH_LEVEL3_SCENE5_POP5_BUCKET_DEF
#define DQFYLH_LEVEL3_SCENE5_POP5_BUCKET_DEF

#include "Popup.h"
#include "CVideo.h"
#include "DialogSystem.h"

class L3S5_Pop5Bucket: public Popup
{
	SuperFX *S5RisingFxFX;

	public:
		L3S5_Pop5Bucket(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S5_Pop5Bucket();

		DialogSystem *dialogSystem;
		float VideoStartTimer;
		void initDialogSystem(DialogSystem *l_dialogSystem);
		void onDialogComplete();
		bool isDialogActive;
		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_Skull;
		void OnAnimationComplete(int animID);
		JSONAnimator  *JAnim_AnnMary;
		
		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();
		void OnTweenComplete(CObject *obj);
		float objAddDelayTimer;

	protected:

		void ReceiveMessage(int val);
		void ProcessHint();

};

#endif