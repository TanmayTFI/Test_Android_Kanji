

#ifndef DQFYLH_L7_S5_SOPHIAEND_DEF
#define DQFYLH_L7_S5_SOPHIAEND_DEF

//====================================
//  created by : Amal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "CVideo.h"
#include "MeshObject.h"
#include "AssetsStringTable.h"

class L7_S5_SophiaEnd : public HiddenObjectSystem
{
public:
	float musicFadeOutTimer;

	SuperFX *s5SofiaCurseFX;

	bool isEndCutscn1Playing, isEndCutscn2Playing;
	CVideo *cv_EndCutscn1, *cv_EndCutscn2;
	float vidTimer;
	float vidTimer2;
	float time;
	int subIndex, subIndex2;
	subText subtitles[10], subtitles2[10];
	CBitmapText* subtitleText;
	int subMax, subMax2;
	MeshObject* skyObj1;
	MeshObject* hairObj;
	L7_S5_SophiaEnd();
	~L7_S5_SophiaEnd();

	void SparkleUpdate();
	void TaskPendingUpdate();

	void Update();
	void Required();

	void SceneObjectsUpdate();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
