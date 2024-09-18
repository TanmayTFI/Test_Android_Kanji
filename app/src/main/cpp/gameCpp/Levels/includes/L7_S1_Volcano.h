

#ifndef DQFYLH_L7_S1_VOLCANO_DEF
#define DQFYLH_L7_S1_VOLCANO_DEF

//====================================
//  created by : Amal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

enum eL7S1PopEnums
{
	ePop1_Flower,
	ePop2_Well,
	ePopL7S1_Max
};

class L7_S1_Volcano : public HiddenObjectSystem
{
public:
	//Popups
	int ActivePopupID;
	float musicFadeOutTimer;

	SuperFX *s1lava1FX, *s1lava12FX, *s1lava13FX, *s1lava14FX;

	Popup*		PopupArray[ePopL7S1_Max];
	int			PopStates[ePopL7S1_Max];
	CPolygon*	PopPolyArray[ePopL7S1_Max];

	bool isAxeCutscnPlaying, isSofiaCutscnPlaying;
	CVideo *cv_AxeCutscn, *cv_SofiaCutscn;
	float vidTimer;
	float vidTimer2;
	float time;
	int subIndex;
	subText subtitles[3];
	CBitmapText* subtitleText; int subMax;

	void OnPopupClosed(Popup* popup);

	L7_S1_Volcano();
	~L7_S1_Volcano();

	void SparkleUpdate();
	void TaskPendingUpdate();

	void InitPopups();
	void Update();
	void Required();

	void SceneObjectsUpdate();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
