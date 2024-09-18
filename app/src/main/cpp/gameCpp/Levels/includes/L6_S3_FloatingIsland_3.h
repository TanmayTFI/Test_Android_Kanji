//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE3 : FLOATING ISLAND_3
//====================================

#ifndef DQFYLH_LEVEL6_SCENE3_FLOATING_ISLAND_3_DEF
#define DQFYLH_LEVEL6_SCENE3_FLOATING_ISLAND_3_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"
#include "L6S3_Pop1Charcoal.h"
#include "L6S3_Pop2Fire.h"
#include "SuperFX.h"
#include "CVideo.h"
#include "JSONAnimator.h"

class L6_S3_FloatingIsland_3: public HiddenObjectSystem, CRenderListEntity
{
	
	SuperFX *S3GoldGlowFX;
	SuperFX *S3UnderFogFX;
	SuperFX *S3SmokeFX;

	public:
		float musicFadeOutTimer;
		//popups
		std::vector<ClickablePolygon*> PopRectArray;
		std::vector<Popup*> PopupArray;
		L6S3_Pop2Fire *pop2;
		int ActivePopUpId;
		SuperFX *PlantGlowFX_L, *PlantGlowFX_R, *SnowFX_L, *SnowFX_R;
		bool isVideoPlaying;
		CVideo *lpVidLasyaCutscene;
		JSONAnimator *Janim_pathDestroy, *Janim_pathBuild;

		//--------------------------- Functions -------------------------------//
		L6_S3_FloatingIsland_3();
		~L6_S3_FloatingIsland_3();

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
		void OnAnimComplete(int animID);
		void NavigateToScene();
	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void SparkleUpdate();
		void CheckForPendingTask();
};

#endif