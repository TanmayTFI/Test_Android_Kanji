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



L3S2_Pop3Coconut::L3S2_Pop3Coconut(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S2\\POP3\\P3.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	isAnimationPlaying = false;

	GetObjectByID("coconutTaken")->SetEnable(false);
	GetObjectByID("Spear")->SetEnable(false);


	ProfileSystem::Instance->ActiveProfile.L3_S2_LeafMoved = true;
	ProfileSystem::Instance->ActiveProfile.L3_S2_Stick1Moved = true;
	ProfileSystem::Instance->ActiveProfile.L3_S2_Stick2Moved = true;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_LeafMoved)
		GetObjectByID("leaf")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_Stick1Moved)
		GetObjectByID("stick1")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_Stick2Moved)
		GetObjectByID("stick2")->SetEnable(false);

	JAnim_Coconut = new JSONAnimator(0, CC_CALLBACK_1(L3S2_Pop3Coconut::OnAnimationComplete, this));
	JAnim_Coconut->parseAndLoadJSON("ANIM\\L3\\S2POP3COCONUTANIM.JSON", this);

	Show(false);

}

void L3S2_Pop3Coconut::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		GetObjectByID("Spear")->SetEnable(false);
		GetObjectByID("coconut")->SetEnable(false);
		GetObjectByID("coconutMask")->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L3S2_CoconutFull, GetObjectByID("coconut")->Pos);
		Cursor::Show(true);
		ClosePopup(1.0f);
	}
	
}

void L3S2_Pop3Coconut::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L3S1_Spear &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconnutTaken && ProfileSystem::Instance->ActiveProfile.L3_S2_Stick2Moved)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconnutTaken = true;
			JAnim_Coconut->PlayAnim();
			isAnimationPlaying = true;
			Control::Audio->QuickLoadAndPlaySFX("s2p3_coconuttake");
		}
		else 
		{
			Hud->ShowBannerText("L3top");
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L3S2_Pop3Coconut::Required()
{

}

void L3S2_Pop3Coconut::Update(float ds)
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

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_LeafMoved &&  InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			float tweenArray[5] = {0,998,334,1,0};
			GetObjectByID("leaf")->TweenTo(tweenArray,1,0,false);
			ProfileSystem::Instance->ActiveProfile.L3_S2_LeafMoved = true;
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Stick1Moved &&  InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			float tweenArray[5] = {0,998,334,1,0};
			GetObjectByID("stick1")->TweenTo(tweenArray,1,0,false);
			ProfileSystem::Instance->ActiveProfile.L3_S2_Stick1Moved = true;
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Stick2Moved &&  InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			float tweenArray[5] = {0,998,334,1,0};
			GetObjectByID("stick2")->TweenTo(tweenArray,1,0,false);
			ProfileSystem::Instance->ActiveProfile.L3_S2_Stick2Moved = true;
		}
	}
	else  if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconnutTaken &&  InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L3coconut");
		}
	}
	
}

float L3S2_Pop3Coconut::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S2_Pop3Coconut::~L3S2_Pop3Coconut()
{
	SAFE_DELETE(JAnim_Coconut);
}

void L3S2_Pop3Coconut::ReceiveMessage(int val)
{	

	
}

bool L3S2_Pop3Coconut::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagStickTaken && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconnutTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L3S2_Pop3Coconut::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if(ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagStickTaken && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconnutTaken)
		Hud->ShowHintForInvItem(eInv_L3S1_Spear, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
