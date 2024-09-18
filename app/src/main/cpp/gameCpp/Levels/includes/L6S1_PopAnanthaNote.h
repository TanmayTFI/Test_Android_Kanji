#ifndef DQFYLH_LEVEL6_SCENE1_POP_ANANTHA_LETTER_DEF
#define DQFYLH_LEVEL6_SCENE1_POP_ANANTHA_LETTER_DEF

#include "Popup.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

class L6S1_PopAnanthaNote: public Popup
{
	CVideo *lpVidVaruniCutScn;
	float time;
	float vidTimer;

	int subIndex;
	subText subtitles[10];
	CBitmapText* subtitleText; int subMax;

	
	public:
		bool isVideoPlaying;
		L6S1_PopAnanthaNote(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S1_PopAnanthaNote();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		void OnPopupOpened();
	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif