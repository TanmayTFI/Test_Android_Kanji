//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 06 - POPUP 5: STONEFACE
//====================================

#include "L2S6_Pop5StoneFace.h"
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

const CRectangle  InvDestStoneBallRect(570.0f,246.0f,191.0f,310.0f);

L2S6_Pop5StoneFace::L2S6_Pop5StoneFace(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)

{
	//Popup load
	Load("LV2\\S6\\POP5\\P5.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	if (!ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced)
	{
		GetObjectByID("Stone-ball")->SetEnable(false);
	}
	/*else
	{
	GetObjectByID("Water")->SetEnable(false);
	}*/

	JAnim_stoneBall = new JSONAnimator(0, CC_CALLBACK_1(L2S6_Pop5StoneFace::OnAnimationComplete, this));
	JAnim_stoneBall->parseAndLoadJSON("ANIM\\L2\\S6P5STONEANIM.JSON", this);

}

void L2S6_Pop5StoneFace::Required()
{

}

void L2S6_Pop5StoneFace::OnPopupCloseBtnClicked()
{
	Control::Audio->StopSFX("s6p5_waterloop");
}

void L2S6_Pop5StoneFace::OnPopupOpened()
{
	Control::Audio->QuickLoadAndPlaySFX("s6p5_waterloop",true);
	Popup::OnPopupOpened();
}

void L2S6_Pop5StoneFace::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (InvDestStoneBallRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced && !Hud->IsDraggingItem())
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked() )
			Hud->ShowBannerText("L2face");
	}
}

void L2S6_Pop5StoneFace::OnAnimationComplete(int animID)
{
	ClosePopup(1.0f);
}

float L2S6_Pop5StoneFace::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S6_Pop5StoneFace::~L2S6_Pop5StoneFace()
{
	SAFE_DELETE(JAnim_stoneBall);
}

void L2S6_Pop5StoneFace::TakeInventoryItem(EHOInventory ItemID)
{
	/*
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	default:
	break;
	}
	*/
}

void L2S6_Pop5StoneFace::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (itemVal == eInv_L2S1_RoundStone && InvDestStoneBallRect.Intersection(&dropPos))
	{
		if (!ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced)
		{
			//GetObjectByID("Stone-ball")->SetEnable(true);
			JAnim_stoneBall->PlayAnim();
			//GetObjectByID("Water")->SetEnable(false);
			Control::Audio->StopSFX("s6p5_waterloop");
			Control::Audio->QuickLoadAndPlaySFX("s6p5_stonefix");

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_RoundStone] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S1_RoundStone);
		} 
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

bool L2S6_Pop5StoneFace::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S6_Pop5StoneFace::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S1_RoundStone, &InvDestStoneBallRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
