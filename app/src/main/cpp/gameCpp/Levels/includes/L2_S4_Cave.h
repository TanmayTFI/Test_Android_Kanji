//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 04 : CAVE
//====================================

#ifndef DQFYLH_LEVEL2_SCENE4_CAVE_DEF
#define DQFYLH_LEVEL2_SCENE4_CAVE_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#include "L2S4_Pop1WoodCut.h"
#include "L2S4_Pop2FirePop.h"
#include "L2S4_Pop3OilStoneLamp.h"
#include "L2S4_Pop4StoneBrick.h"
#include "L2S4_Pop5Rake.h"
#include "AssetsStringTable.h"
#include "CVideo.h"
#include "MeshObject.h"

enum eL2S4PopEnums
{
	ePop1_WoodCut,
	ePop2_FirePopL,
	ePop2_FirePopR,
	ePop3_OilStoneLamp,
	ePop4_StoneBrick,
	ePop5_Rake,
	ePop_Max
};

class L2_S4_Cave: public HiddenObjectSystem
{
	public:
		bool isLasyaAttackVideoPlaying;
		CVideo *cv_LasyaAttack; 
		float vidTimer;
		float time;
		int subIndex;
		subText subtitles[5];
		CBitmapText* subtitleText; int subMax;
		//float lasyaAttackStartTimer;
		float musicFadeOutTimer;
		std::vector<SuperFX*>	VFXArray;

		MeshObject* meshWater;

		//Popups
		int ActivePopupID;

		Popup*		PopupArray[ePop_Max];
		int			PopStates[ePop_Max];
		CPolygon*	PopPolyArray[ePop_Max];


		bool isPuzzleDietiesActive;

		void OnPopupClosed(Popup* popup);
		
		L2_S4_Cave();
		~L2_S4_Cave();

		void SparkleUpdate();
		void TaskPendingUpdate();

		void InitPopups();
		void Update();
		void Required();

		void SceneObjectsUpdate();

		void NavigateToScene();
		
	protected:
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif