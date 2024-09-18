

#ifndef DQFYLH_L3_S4_POP4_METALSHEET_DEF
#define DQFYLH_L3_S4_POP4_METALSHEET_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

class L3S4_Pop4Metalsheet : public Popup
{
public:
	L3S4_Pop4Metalsheet(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3S4_Pop4Metalsheet();

	bool isAmeehaCutscnPlaying;
	CVideo *cv_AmeehaCutscn;
	float time;
	float vidTimer;
	int subIndex;
	subText subtitles[15];
	CBitmapText* subtitleText; int subMax;

	float clickDelayTimer;

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void OnDiaryNoteTweenComplete(CObject *obj);

	void Update(float ds);
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

};

#endif
