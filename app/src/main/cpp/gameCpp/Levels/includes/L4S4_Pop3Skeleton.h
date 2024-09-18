#ifndef DQFYLH_LEVEL4_SCENE4_POP3_SKELETON_DEF
#define DQFYLH_LEVEL4_SCENE4_POP3_SKELETON_DEF

#include "Popup.h"
#include "JSONAnimator.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

class L4S4_Pop3Skeleton: public Popup
{
		bool isOrbGlowing, isOrbFXFlying;

	public:
		JSONAnimator *Janim_handOpen;
		CVideo *lpVidFlowerCutScn;
		float time;
		float vidTimer;
		int subIndex;
		subText subtitles[5];
		CBitmapText* subtitleText; int subMax;

		bool isVideoPlaying;

		L4S4_Pop3Skeleton(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S4_Pop3Skeleton();

		void startMemoryCutscene();

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