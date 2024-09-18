//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL4 - SCENE2 : RIVER SIDE
//====================================

#ifndef DQFYLH_LEVEL4_SCENE2_RIVER_SIDE_DEF
#define DQFYLH_LEVEL4_SCENE2_RIVER_SIDE_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"

#include "L4S2_Pop1MagicFroot.h"
#include "L4S2_Pop2Bamboo.h"
#include "L4S2_Pop3TreeBottom.h"
#include "L4S2_Pop4Grass.h"
#include "MeshObject.h"
enum eL4S2HOPopEnums
{
	eL4S2Pop_Inactive = -1,
	eL4S2Pop1_MagicFruit,
	eL4S2Pop2_Bamboo,
	eL4S2Pop3_TreeBottom,
	eL4S2Pop4_Grass,
	eL4S2Pop_Max
};


class L4_S2_RiverSide: public HiddenObjectSystem, CRenderListEntity
{
	public:
		//popups
		//int ActivePopUpId;
		//std::vector<CPolygon*> PopRectArray;
		//std::vector<Popup*> PopupArray;
		float musicFadeOutTimer;
		int			ActivePopupID;
		Popup *		PopupArray[eL4S2Pop_Max];
		std::vector<SuperFX *>_arrayOfVFX;
		
		MeshObject *waterMesh; 
		MeshObject *leafMesh;
		//--------------------------- Functions -------------------------------//
		L4_S2_RiverSide();
		~L4_S2_RiverSide();

		CObject *blurBg;
		CObject *BambooPlant;
		CObject *BambooSingle;
		CObject *BgLight;
		CObject *CaveDoor01;
		CObject *CaveDoor02;
		CObject *FriutOnTree01;
		CObject *FriutOnTree03;
		CObject *FriutOnTree04;
		CObject *FriutOnTree05;
		CObject *FriutOnTree06;
		CObject *FriutOnTree07;
		CObject *FruitOnTree02;
		CObject *FruitUnderTree1;
		CObject *GlowDots01;
		CObject *GlowDots02;
		CObject *MagicPlant;
		CObject *MagicPlantGlow;
		CObject *PinkFruitGlow;
		CObject *ScaryGlow01;
		CObject *ScaryGlow02;
		CObject *ScaryGlow03;
		CObject *ScaryGlow04;


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
		bool CheckIfPopupActive(int Id);
		void UpdateBannerMessages();
		void SceneObjectsUpdate();
		void SparkleUpdate();
		void TaskPendingUpdate();
		void InitSceneElements();
		void initAnimations();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void OnPopupClicked(int popupId);
		void CheckForPendingTask();
};

#endif