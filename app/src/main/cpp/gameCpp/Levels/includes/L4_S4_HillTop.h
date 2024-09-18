//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL4 - SCENE2 : RIVER SIDE
//====================================

#ifndef DQFYLH_LEVEL4_SCENE4_HILLTOP_DEF
#define DQFYLH_LEVEL4_SCENE4_HILLTOP_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"
#include "L4S4_Pop1StoneMill.h"
#include "L4S4_Pop3Skeleton.h"
#include "MeshObject.h"
#include "AssetsStringTable.h"

class L4_S4_HillTop: public HiddenObjectSystem, CRenderListEntity
{
	CVideo *lpVidJumpDown, *lpVidClimbUp;
	float time;
	float vidTimer;
	float vidTimer2;
	int subIndex, subIndex2;
	subText subtitles[3], subtitles2[3];
	CBitmapText* subtitleText; int subMax, subMax2;
	bool fallDone;
	bool isVideoPlaying;
	public:
		float musicFadeOutTimer;
		//popups
		std::vector<ClickablePolygon*> PopRectArray;
		std::vector<Popup*> PopupArray;
		L4S4_Pop3Skeleton *pop3;
		SuperFX *WaterFallFX_1, *WaterFallFX_2;
		SuperFX *SnowFX_L, *SnowFX_M, *SnowFX_R;
		SuperFX *PlantGlowFX_L, *PlantGlowFX_M, *PlantGlowFX_R;

		int ActivePopUpId;
		int bannerDelayTimer;
		MeshObject *mo_treeface;
		//--------------------------- Functions -------------------------------//
		L4_S4_HillTop();
		~L4_S4_HillTop();

		void InitPopups();
		void OnPopupClosed(Popup * popup);

		bool isPopupActive(int popId);
		
		float easing(float t,float b,float c,float d);
		
		void TakeInventoryItem(EHOInventory ItemID);
		//Inherit this CRenderListEntity function to render anything to scene!!
		void Render(const CPoint* pCam);
		void Update();
		void Required();
		void SceneObjectsUpdate();
		void TaskPendingUpdate();
		void InitParticles();
		void NavigateToScene();
	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void SparkleUpdate();
		void CheckForPendingTask();
};

#endif