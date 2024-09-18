//====================================
// DQFYLH
// Hidden Object System
// Cutscene Player
//====================================

#ifndef DQFYLH_EXTRASCUTSCENEPLAYER_DEF
#define DQFYLH_EXTRASCUTSCENEPLAYER_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "AssetsStringTable.h"

#include "CVideo.h"
#include "BitmapText.h"

class CHOScene;

class ExtrasCutscenePlayer: public HiddenObjectSystem
{
	public:

		//Scene Elements

		ExtrasCutscenePlayer();
		~ExtrasCutscenePlayer();

		void Update();
		void Required(){};

		CVideo *cv_Cutscn;
		//subText subtitles[20];

		float time;
		int subIndex;
		subText subtitles[20];
		CBitmapText* subtitleText; int subMax;
		CBitmapText* vid_time;

	protected:
		void ReceiveMessage(int val){};
};

#endif