//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 02 - POPUP 3: Coconut
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
#include "L3S2_Pop3Coconut.h"

const CRectangle  InvDestRect(574.0f,248.0f,191.0f,167.0f);



L3S2_Pop4Crow::L3S2_Pop4Crow(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S2\\POP4\\P4.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	isAnimationPlaying = false;

	GetObjectByID("DoorHandleMask")->SetEnable(false);
	GetObjectByID("spear")->SetEnable(false);
	GetObjectByID("p2CrowFlyingAnim1")->SetEnable(false);
	GetObjectByID("p2CrowFlyingAnim2")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_CrowFlewAway)
		GetObjectByID("p2CrowLoopAnim")->SetEnable(false);

	crowAnim1Playing = false;
	crowAnim2Playing = false;

	JAnim_Lock = new JSONAnimator(0, CC_CALLBACK_1(L3S2_Pop4Crow::OnAnimationComplete, this));
	JAnim_Lock->parseAndLoadJSON("ANIM\\L3\\S2POP4LOCKANIM.JSON", this);

	Show(false);

}

void L3S2_Pop4Crow::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		
		Cursor::Show(true);
		Hud->ShowBannerText("L3pool");
		//ClosePopup(1.0f);
	}
	
}

void L3S2_Pop4Crow::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L3S1_Spear &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened && ProfileSystem::Instance->ActiveProfile.L3_S2_CrowFlewAway)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened = true;
			JAnim_Lock->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p4_doorunlocked");
		}
		else 
		{
			Hud->ShowBannerText("L3crows");
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);

}

void L3S2_Pop4Crow::Required()
{

}

void L3S2_Pop4Crow::Update(float ds)
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

	if(crowAnim1Playing)
	{
		if(GetObjectByID("p2CrowFlyingAnim1")->IsAnimationFinished())
		{
			GetObjectByID("p2CrowFlyingAnim1")->SetEnable(false);
			GetObjectByID("p2CrowFlyingAnim2")->SetEnable(true);
			GetObjectByID("p2CrowFlyingAnim2")->PlayAnimation(0);
			crowAnim1Playing = false;
			crowAnim2Playing = true;
		}
	}

	if(crowAnim2Playing)
	{
		if(GetObjectByID("p2CrowFlyingAnim2")->IsAnimationFinished())
		{
			GetObjectByID("p2CrowFlyingAnim2")->SetEnable(true);
			crowAnim2Playing = false;
			ProfileSystem::Instance->ActiveProfile.L3_S2_CrowFlewAway = true;
		}
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_CrowFlewAway && InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("p2CrowFlyingAnim1")->SetEnable(true);
			GetObjectByID("p2CrowFlyingAnim1")->PlayAnimation(0);
			GetObjectByID("p2CrowLoopAnim")->SetEnable(false);
			crowAnim1Playing = true;
			Control::Audio->QuickLoadAndPlaySFX("s2p4_crowfly");
			Hud->ShowBannerText("L3crows");
		}
	}
	else if(InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			if(!ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened)
			Hud->ShowBannerText("L3lock");
			else
			Hud->ShowBannerText("L3pool");
		}
	}
}

float L3S2_Pop4Crow::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S2_Pop4Crow::~L3S2_Pop4Crow()
{
	SAFE_DELETE(JAnim_Lock);
}

void L3S2_Pop4Crow::ReceiveMessage(int val)
{	

	
}

bool L3S2_Pop4Crow::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_CrowFlewAway)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagStickTaken && !ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened && ProfileSystem::Instance->ActiveProfile.L3_S2_CrowFlewAway) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L3S2_Pop4Crow::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_CrowFlewAway)
		Hud->InitHintTrailEmit( &InvDestRect);
	else if(ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagStickTaken && !ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened && ProfileSystem::Instance->ActiveProfile.L3_S2_CrowFlewAway) 
		Hud->ShowHintForInvItem(eInv_L3S1_Spear, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
