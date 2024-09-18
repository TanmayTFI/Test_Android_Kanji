

#ifndef DQFYLH_L5_S6_MOUTAIN_TOP_DEF
#define DQFYLH_L5_S6_MOUTAIN_TOP_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

enum eL5S6PopEnums
{
	ePop1_Peacock,
	ePop2_Moon,
	ePopL5S6_Max
};

class L5_S6_MountainTop : public HiddenObjectSystem
{
public:
	float 	Replay_WaitingForCollectablePickTicks;
	bool    IsReplay_WaitingForCollectablePick;

	//Popups
	int ActivePopupID;
	CObject		*cloudBG1, *cloudBG2;
	float cloudbgSpeed1, cloudbgSpeed2;
	CObject		*cloudFG1;
	float cloudfgSpeed1;
	SuperFX *S6MoonprtFX;
	float musicFadeOutTimer;
	bool isIntroCutscnPlaying;
	CVideo *cvIntroCutscn;
	float time;
	float vidTimer;
	float vidTimer2;
	float vidTimer3;
	int subIndex, subIndex2;
	subText subtitles[3], subtitles2[3], subtitles3[3];
	CBitmapText* subtitleText; int subMax;

	bool isPeacockCutscnPlaying;
	CVideo *cv_PeacockCutscn;

	bool isOutroCutscnPlaying;
	CVideo *cv_OutroCutscn;

	Popup*		PopupArray[ePopL5S6_Max];
	int			PopStates[ePopL5S6_Max];
	CPolygon*	PopPolyArray[ePopL5S6_Max];

	void OnPopupClosed(Popup* popup);

	L5_S6_MountainTop();
	~L5_S6_MountainTop();

	void SparkleUpdate();
	void TaskPendingUpdate();

	void InitPopups();
	void Update();
	void Required();

	void NavigateToScene();
	void SceneObjectsUpdate();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
