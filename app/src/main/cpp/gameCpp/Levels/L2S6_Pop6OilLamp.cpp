//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 06 - POPUP 6: OIL LAMP
//====================================

#include "L2S6_Pop6OilLamp.h"
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

const CRectangle  InvDestTorchRect(595.0f,307.0f,206.0f,127.0f);

L2S6_Pop6OilLamp::L2S6_Pop6OilLamp(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)

{
	//Popup load
	Load("LV2\\S6\\POP6\\P6.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;


	JAnim_fireAnim = NULL;
	JAnim_fireAnim = new JSONAnimator(0);
	JAnim_fireAnim->parseAndLoadJSON("ANIM\\L2\\S6P6FIREANIM.JSON", this);


	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
	{
		GetObjectByID("p2torchonfire")->SetEnable(false);
		GetObjectByID("Mask")->SetEnable(false);
	}
	else
	{
		JAnim_fireAnim->PlayAnim();
		GetObjectByID("bgblue")->SetEnable(false);
	}


}

void L2S6_Pop6OilLamp::Required()
{

}

void L2S6_Pop6OilLamp::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (InvDestTorchRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened && !Hud->IsDraggingItem())
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L2torch");
		}
	}
}

float L2S6_Pop6OilLamp::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S6_Pop6OilLamp::~L2S6_Pop6OilLamp()
{
	SAFE_DELETE(JAnim_fireAnim);
}

void L2S6_Pop6OilLamp::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (InvDestTorchRect.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
	{
		if (itemVal == eInv_L2S4_LitTorch)
		{
			GetObjectByID("p2torchonfire")->SetEnable(true);
			GetObjectByID("torch")->SetEnable(false);
			JAnim_fireAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s6p6_torchplaced");

			GetObjectByID("Mask")->SetEnable(true);
			GetObjectByID("bgblue")->SetEnable(false);

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened = true;

			ClosePopup(0.5);

			if (ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_LitTorch] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_LitTorch); 
			} 
		}
		else if (itemVal == eInv_L2S3_PoojaLamp)
		{
			Hud->ShowBannerText("L2weird");
			Hud->CancelDrag();
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		//ClosePopup(2.5f);
	}
}

bool L2S6_Pop6OilLamp::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened && ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S6_Pop6OilLamp::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened && ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S4_LitTorch, &InvDestTorchRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
