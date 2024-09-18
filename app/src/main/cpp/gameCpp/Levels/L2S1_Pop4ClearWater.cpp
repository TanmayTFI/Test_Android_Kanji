//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 5: SCRAPTER
//====================================

#include "L2S1_Pop4ClearWater.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "InteractiveObjectList.h"
#include "exMath.h"

#define ANGLE_TO_ROTATE 120

const CRectangle  InvDestRectEmptyPot(500.0f,346.0f,408.0f,189.0f);


L2S1_Pop4ClearWater::L2S1_Pop4ClearWater(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)

{

	//Popup load
	Load("LV2\\S1\\POP4\\P4.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	leavesSet1 = GetObjectByID("Leaves01");
	leavesSet2 = GetObjectByID("Leaves02");
	leavesSet3 = GetObjectByID("Foreground_Palnt01");
	emptyPot = GetObjectByID("PotAnim_01");
	filledPot = GetObjectByID("FullPot");

	leavesSet1->SetPivot(-64,-56);
	leavesSet1->Animate(0,2,0,0,5);
	leavesSet2->SetPivot(-98,-64);
	leavesSet2->Animate(0,2,0,0,6,2);
	leavesSet3->SetPivot(119,104);
	leavesSet3->Animate(2,1,0,0,7,3);
	emptyPot->SetEnable(false);
	filledPot->SetEnable(false);

	pondWater = NULL;
	pondWater = new MeshObject("LV2\\S1\\POP4\\Water_Animation.PNG", 5, 5);
	pondWater->SetPosAndParams(683 - 321,334 - 29, 3, 2, 4, 5);
	pondWater->SetZ(zOrder + 1);
	InsertObject(pondWater,1);
	pondWater->SetEnable(false);

	CAnim_EmptyPot = NULL;
	CAnim_EmptyPot = new CustomAnimator("ANIM\\L2\\pot.anim", emptyPot);

	CAnim_FilledPot = NULL;
	CAnim_FilledPot = new CustomAnimator("ANIM\\L2\\potfull.anim", filledPot);

	CAnim_WaterMask = NULL;
	CAnim_WaterMask = new CustomAnimator("ANIM\\L2\\waterMask.anim", GetObjectByID("Water_Mask"));

	IsCustomAnimPlaying = false;
}

L2S1_Pop4ClearWater::~L2S1_Pop4ClearWater()
{
	SAFE_DELETE(CAnim_EmptyPot);
	SAFE_DELETE(CAnim_FilledPot);
	SAFE_DELETE(CAnim_WaterMask);
}

void L2S1_Pop4ClearWater::OpenPopup()
{
	//pondWater->meshFX->SetOffset(191, 304);

	Popup::OpenPopup();
}


void L2S1_Pop4ClearWater::OnScaleUpComplete()
{
	//pondWater->meshFX->SetOffset(362, 304);
	pondWater->SetEnable(true);

	Popup::OnScaleUpComplete();
}

void L2S1_Pop4ClearWater::ClosePopup(float delayTime)
{
	if (IsCustomAnimPlaying)
	{
		return;
	}
	//pondWater->meshFX->SetOffset(191, 304);
	pondWater->SetEnable(false);

	if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken)
		Popup::ClosePopup();
	else
		Popup::ClosePopup(2.5f);
}

void L2S1_Pop4ClearWater::Required()
{

}

void L2S1_Pop4ClearWater::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(CAnim_FilledPot->IsAnimFinished() && IsCustomAnimPlaying)
	{
		IsCustomAnimPlaying = false;
		TakeInventoryItem(eInv_L2S1_PotWithWater);
		filledPot->SetEnable(false);
	}

	if (InvDestRectEmptyPot.Intersection(&pos) && !Hud->IsDraggingItem())
	{
		if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2water");
			}
		}
	}
}

void L2S1_Pop4ClearWater::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	case eInv_L2S1_PotWithWater:
		Hud->InvBoxCtrl->Activate(1);		
		filledPot->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_PotWithWater] = true;
		ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken = true;			
		vecInvIds.push_back(eInv_L2S1_PotWithWater);
		Hud->ShowInventory(&vecInvIds);
		ClosePopup(2.5f);
		break;
	default:
		break;
	}
}

void L2S1_Pop4ClearWater::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (InvDestRectEmptyPot.Intersection(&dropPos))
	{
		if (itemVal == eInv_L2S5_PotEmpty)
		{
			if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken )
			{
				CAnim_EmptyPot->PlayAnim();
				CAnim_FilledPot->PlayAnim();
				CAnim_WaterMask->PlayAnim();
				IsCustomAnimPlaying = true;
				Control::Audio->QuickLoadAndPlaySFX("s1p4_watercollect");

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S5_PotEmpty] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S5_PotEmpty);
			} 
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		return;
	}
}

bool L2S1_Pop4ClearWater::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken && ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S1_Pop4ClearWater::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken && ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S5_PotEmpty, &InvDestRectEmptyPot);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
