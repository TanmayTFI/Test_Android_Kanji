

#ifndef DQFYLH_LEVEL6_SCENE3_FAKE_JAMES_DEF
#define DQFYLH_LEVEL6_SCENE3_FAKE_JAMES_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "DialogSystem.h"
#include "AssetsStringTable.h"

class L6_S3_FakeJames : public HiddenObjectSystem
{
public:
	L6_S3_FakeJames();
	~L6_S3_FakeJames();
	float musicFadeOutTimer;
	CVideo *cv_Cutscn;
	float time;
	float vidTimer;
	int subIndex;
	int subMax;
	subText subtitles[3];
	CBitmapText* subtitleText;
	void Update();
	void Required();
	void NavigateToScene();

	DialogSystem *dialogSystem;
	bool isDialogActive;
	bool isVideoPlaying;
protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
