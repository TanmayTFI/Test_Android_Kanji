//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL4 - SCENE2 : RIVER SIDE
//====================================

#ifndef DQFYLH_LEVEL4_SCENE5_HILLSIDE_DEF
#define DQFYLH_LEVEL4_SCENE5_HILLSIDE_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"
#include "L4S5_Pop1Door.h"
#include "L4S5_Pop2OliveTree.h"
#include "L4S5_Pop3Skeleton.h"
#include "L4S5_Pop4NetSquare.h"
#include "L4S5_Pop5Bridge.h"

class L4_S5_HillSide: public HiddenObjectSystem, CRenderListEntity
{
	public:
		float musicFadeOutTimer;
		//popups
		std::vector<ClickablePolygon*> PopRectArray;
		std::vector<Popup*> PopupArray;

		int ActivePopUpId;
		
		
		
		CObject *Door, *Cloud01, *clouds02, *clouds03, *FlintStone, *MagicMushroom, *MagicPlant01, *MagicPlant02, *Morph01, *Morph02, *NetCut, *Owl, *Plant01, *Plant02, *Plant03, *Plant04;
		CObject *PlantAnim, *PlantYellow01, *PlantYellow02, *ScaryFace1, *ScaryFace2, *ScaryFace3, *ScaryFace4, *Scroll, *SolvedSate, *TreeBranch, *TreeGlow, *VioletGlow;
		
		SuperFX *S5GlowPlant01FX;
		SuperFX *S5GlowPlant02FX;
		SuperFX *S5GlowPlant03FX;
		SuperFX *S5GlowPlant04FX;
		SuperFX *S5GlowPlant05FX;
		SuperFX *S5GlowPlant06FX;

		//--------------------------- Functions -------------------------------//
		L4_S5_HillSide();
		~L4_S5_HillSide();

		void InitPopups();
		void OnPopupClosed(Popup * popup);
		
		float easing(float t,float b,float c,float d);
		
		void TakeInventoryItem(EHOInventory ItemID);
		//Inherit this CRenderListEntity function to render anything to scene!!
		void Render(const CPoint* pCam);
		void Update();
		void Required();
		void NavigateToScene();


	protected:
		void StopParticleEmission();
		bool CheckIfPopupActive(int id);
		void InitSceneElements();
		void InitAnimations();
		void ReceiveMessage(int val);
		void SceneObjectsUpdate();
		void ProcessNonHOHint();
		void SparkleUpdate();
		void CheckForPendingTask();
		void TaskPendingUpdate();
};

#endif