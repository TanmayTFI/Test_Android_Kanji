

#ifndef DQFYLH_L5_S1_POP3TIGER_DEF
#define DQFYLH_L5_S1_POP3TIGER_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "JSONAnimator.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

class L5S1_Pop3Tiger : public Popup
{
public:
	L5S1_Pop3Tiger(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S1_Pop3Tiger();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

	void OnAnimationComplete(int animID);

	bool isTigerShootCutscnPlaying;
	CVideo *cv_TigerShootCutscn;
	float time;
	float vidTimer;
	int subIndex;
	subText subtitles[3];
	CBitmapText* subtitleText; int subMax;

	void OnPopupOpened();

protected:

	JSONAnimator *_JAnim_TigerLoop, *_JAnim_BowAim;

};

#endif
