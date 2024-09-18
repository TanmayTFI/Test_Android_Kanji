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
#include "L3S5_Pop3Rope.h"

const CRectangle  InvDestRect(368.0f,99.0f,626.0f,458.0f);
const CRectangle  PopupMaskRect(190.0f,88.0f,818.0f,490.0f);
const CRectangle  InvRectHammerHandle(581.0f,381.0f,224.0f,89.0f);


L3S5_Pop3Rope::L3S5_Pop3Rope(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S5\\POP3\\P3.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	JAnim_Rope = new JSONAnimator(0, CC_CALLBACK_1(L3S5_Pop3Rope::OnAnimationComplete, this));
	JAnim_Rope->parseAndLoadJSON("ANIM\\L3\\S5P3ROPETAKEANIM.JSON", this);

	GetObjectByID("bambooStick")->SetEnable(false);
	GetObjectByID("bambooStick")->SetMaskRect(PopupMaskRect);
	Show(false);

}

void L3S5_Pop3Rope::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		Cursor::Show(true);
		GetObjectByID("rope")->SetEnable(false);
		GetObjectByID("ropeShadow")->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L3S5_Rope, GetObjectByID("rope")->Pos);
		ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken = true;
		ClosePopup(1.0f);
	}
	
}

void L3S5_Pop3Rope::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}
	if (itemVal == eInv_L3S4_BambooStick &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_BambooStick] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S4_BambooStick);
			JAnim_Rope->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p3_ropetaken");
		}
		return;
	}
	else
		if (itemVal == eInv_L3S1_Spear &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken)
		{
			Hud->ShowBannerText("L3hook");
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);

}

void L3S5_Pop3Rope::Required()
{

}

void L3S5_Pop3Rope::Update(float ds)
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
	
	if(InvDestRect.Intersection(&pos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken )
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L3looks");
			}
		}
	}

	
}

float L3S5_Pop3Rope::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S5_Pop3Rope::~L3S5_Pop3Rope()
{
	SAFE_DELETE(JAnim_Rope);
}

void L3S5_Pop3Rope::ReceiveMessage(int val)
{	

	
}

bool L3S5_Pop3Rope::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	 if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeTaken) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L3S5_Pop3Rope::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	 if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeTaken) 
		Hud->ShowHintForInvItem(eInv_L3S4_BambooStick, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
