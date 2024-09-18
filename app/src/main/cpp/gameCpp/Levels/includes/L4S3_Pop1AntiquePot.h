#ifndef DQFYLH_LEVEL4_SCENE3_POP1_ANTIQUE_POT_DEF
#define DQFYLH_LEVEL4_SCENE3_POP1_ANTIQUE_POT_DEF

#include "Popup.h"
#include "JSONAnimator.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

class L4S3_Pop1AntiquePot: public Popup
{
	public:

		L4S3_Pop1AntiquePot(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S3_Pop1AntiquePot();

		CObject *PaperInHand;
		CObject *Pot;
		CObject *MetalPaper;
		CObject *Bowl;

		CVideo *cv_Cutscn;
		float time;
		float vidTimer;
		int subIndex;
		subText subtitles[20];
		CBitmapText* subtitleText; int subMax;
		bool isVideoPlaying;

		CPolygon *PotPoly, *PaperPoly, *InvBowlPoly;
		JSONAnimator *PotAnim, *PaperTakeAnim;

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		float easing(float t,float b,float c,float d);
		void OnDiaryNoteTweenComplete(CObject *obj);
	protected:
		void OnAnimationComplete(int id);
		void InitSceneElements();
		void ProcessHint();
		bool isTaskLeft();
};

#endif