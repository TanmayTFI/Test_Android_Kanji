//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 02 - POPUP 2: Skull
//====================================

#include "L3_S2_FortInterior.h"
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
#include "L3S2_Pop2Skull.h"

const CRectangle  InvDestRect(407.0f,364.0f,252.0f,195.0f);
const CRectangle  PopupMaskRect(190.0f,88.0f,818.0f,490.0f);
const CRectangle  InvRectHammerHandle(581.0f,381.0f,224.0f,89.0f);


L3S2_Pop2Skull::L3S2_Pop2Skull(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S2\\POP2\\P2.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed)
	{
		GetObjectByID("tooth")->SetEnable(false);
		GetObjectByID("hammerHand")->SetEnable(false);
	}
	else
	{
		GetObjectByID("skull")->SetMaskRect(PopupMaskRect);
		GetObjectByID("tooth")->SetY(407);
		GetObjectByID("skull")->SetY(245);
		GetObjectByID("tooth")->SetEnable(true);
		GetObjectByID("hammerHand")->SetEnable(true);

	}

	JAnim_Skull = new JSONAnimator(0, CC_CALLBACK_1(L3S2_Pop2Skull::OnAnimationComplete, this));
	JAnim_Skull->parseAndLoadJSON("ANIM\\L3\\S2POP2SKULLANIM.JSON", this);

	Show(false);

}

void L3S2_Pop2Skull::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		Cursor::Show(true);
	}
	
}

void L3S2_Pop2Skull::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L3S2_SkullTeeth &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed)
		{
			
		
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			GetObjectByID("skull")->SetMaskRect(PopupMaskRect);
			ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_SkullTeeth] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_SkullTeeth);
			JAnim_Skull->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p2_skullteethuse");
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L3S2_Pop2Skull::Required()
{

}

void L3S2_Pop2Skull::Update(float ds)
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
	
	if( InvRectHammerHandle.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHandTaken && ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("hammerHand")->SetEnable(false);
			GetObjectByID("hammerHandDark")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L3S2_HammerHandle, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHandTaken = true;
			
			ClosePopup(1.0f);
			
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed && (InvDestRect.Intersection(&pos) || InvRectHammerHandle.Intersection(&pos)))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			if( InvDestRect.Intersection(&pos))
				Hud->ShowBannerText("L3teeth");
			else if(InvRectHammerHandle.Intersection(&pos))
				Hud->ShowBannerText("L3teeth");
		}
	}
	
}

float L3S2_Pop2Skull::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S2_Pop2Skull::~L3S2_Pop2Skull()
{
	SAFE_DELETE(JAnim_Skull);
}

void L3S2_Pop2Skull::ReceiveMessage(int val)
{	

	
}

bool L3S2_Pop2Skull::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHandTaken && ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed)
		isAnyTaskLeft = true;
	if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken && !ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L3S2_Pop2Skull::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHandTaken && ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed)
		Hud->InitHintTrailEmit(InvRectHammerHandle.GetCenterX(), InvRectHammerHandle.GetCenterY());
	else if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken && !ProfileSystem::Instance->ActiveProfile.L3_S2_ToothFixed) 
		Hud->ShowHintForInvItem(eInv_L3S2_SkullTeeth, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
