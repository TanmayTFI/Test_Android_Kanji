

#ifndef DQFYLH_L5_S5_POP1_BAMBOOLADDER_DEF
#define DQFYLH_L5_S5_POP1_BAMBOOLADDER_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "MeshObject.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

class L5S5_Pop1BambooLadder : public Popup
{
public:
	L5S5_Pop1BambooLadder(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S5_Pop1BambooLadder();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	CPolygon *stone1poly, *stone2poly, *pillarpoly, *bamboopoly;

	float clickTimer;

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

	bool isAmeehaCutscnPlaying;
	CVideo *cv_AmeehaCutscn;
	float time;
	float vidTimer;
	int subIndex;
	subText subtitles[20];
	CBitmapText* subtitleText; int subMax;

	void OnDiaryNoteTweenComplete(CObject* obj);

protected:
	MeshObject *_waterMesh;

};

#endif
