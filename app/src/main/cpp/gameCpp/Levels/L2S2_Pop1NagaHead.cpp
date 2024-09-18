//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 02 - POPUP 1: Naga Head
//====================================

#include "L2S2_Pop1NagaHead.h"
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

const CRectangle  InvDestRect(382.0f,112.0f,589.0f,444.0f);
L2S2_Pop1NagaHead::L2S2_Pop1NagaHead(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV2\\S2\\POP1\\P1.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	

	JAnim_LampLite = new JSONAnimator(0, CC_CALLBACK_1(L2S2_Pop1NagaHead::OnAnimationComplete, this));
	JAnim_LampLite->parseAndLoadJSON("ANIM\\L2\\S2LAMPLITE.JSON", this);

	GetObjectByID("Thiri1")->SetEnable(false);
	GetObjectByID("Thiri2")->SetEnable(false);
	GetObjectByID("Thiri")->SetEnable(false);
	GetObjectByID("lamp")->SetEnable(false);
	GetObjectByID("scupltureLite")->SetEnable(false);

	Show(false);

}

void L2S2_Pop1NagaHead::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		ClosePopup(1.5f);
		Cursor::Show(true);
	}
	
}

void L2S2_Pop1NagaHead::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}


	if (itemVal == eInv_L2S3_PoojaLamp &&  InvDestRect.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLite)
		{
			ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLite = true;

			JAnim_LampLite->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S3_PoojaLamp] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S3_PoojaLamp);
			Control::Audio->QuickLoadAndPlaySFX("s2p1_lampslite");
			isAnimationPlaying = true;
		}
	}
	else if (itemVal == eInv_L2S4_LitTorch &&  InvDestRect.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLite)
		{
			
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			Hud->ShowBannerText("L2lighting");
		}
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L2S2_Pop1NagaHead::Required()
{

}

void L2S2_Pop1NagaHead::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();
	
	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLite )
	{
		if (InvDestRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2lantern");
			} 
		}
		return;
	}
	
}

float L2S2_Pop1NagaHead::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S2_Pop1NagaHead::~L2S2_Pop1NagaHead()
{
	SAFE_DELETE(JAnim_LampLite);
}

void L2S2_Pop1NagaHead::TakeInventoryItem(EHOInventory ItemID)
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

void L2S2_Pop1NagaHead::ReceiveMessage(int val)
{	

	
}

bool L2S2_Pop1NagaHead::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLite && ProfileSystem::Instance->ActiveProfile.L2_S3_InvPoojaLampPicked)
		isAnyTaskLeft = true;
	

	return isAnyTaskLeft;
}

void L2S2_Pop1NagaHead::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLite && ProfileSystem::Instance->ActiveProfile.L2_S3_InvPoojaLampPicked)
	{
		Hud->ShowHintForInvItem(eInv_L2S3_PoojaLamp, &InvDestRect);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
