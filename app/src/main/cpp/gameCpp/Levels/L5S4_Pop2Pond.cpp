//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 02 - POPUP 3: Coconut
//====================================

#include "L5_S4_Dungeon.h"
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

#include "L5S4_Pop2Pond.h"

const CRectangle  InvDestRect(454.0f,321.0f,541.0f,212.0f);
const CRectangle  InvRectBark(726.0f,328.0f,200.0f,137.0f);
const CRectangle  MemRectWatermelon(599.0f,176.0f,165.0f,120.0f);


L5S4_Pop2Pond::L5S4_Pop2Pond(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV5\\S4\\POP2\\P2.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("crack1")->SetEnable(false);
	GetObjectByID("crack2")->SetEnable(false);
	GetObjectByID("crack3")->SetEnable(false);
	GetObjectByID("crack4")->SetEnable(false);
	GetObjectByID("ground1")->SetEnable(false);
	GetObjectByID("ground2")->SetEnable(false);
	GetObjectByID("ground3")->SetEnable(false);

	GetObjectByID("pickAxeDown")->SetEnable(false);
	GetObjectByID("pickAxeUp")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken )
		GetObjectByID("bark")->SetEnable(false);
	else
		GetObjectByID("bark")->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed)
	{
		GetObjectByID("crack1")->SetEnable(true);
		GetObjectByID("crack2")->SetEnable(true);
		GetObjectByID("crack3")->SetEnable(true);
		GetObjectByID("crack4")->SetEnable(true);
		GetObjectByID("pickAxeDown")->SetEnable(false);
		GetObjectByID("pickAxeUp")->SetEnable(false);
	}

	JAnim_PickAxe = new JSONAnimator(0, CC_CALLBACK_1(L5S4_Pop2Pond::OnAnimationComplete, this));
	JAnim_PickAxe->parseAndLoadJSON("ANIM\\L5\\S4P2PICKAXE.JSON", this);


	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S4Watermelon])
	{
		GetObjectByID("watermelon")->SetEnable(false);
	}

	Show(false);

}

void L5S4_Pop2Pond::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		isAnimationPlaying = false;
		GetObjectByID("pickAxeDown")->SetEnable(false);
		GetObjectByID("pickAxeUp")->SetEnable(false);
		ClosePopup(1.0f);
		
	}
	
}

void L5S4_Pop2Pond::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}
	if (itemVal == eInv_L5_S4_Pickaxe &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed && ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken )
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed= true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S4_Pickaxe] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S4_Pickaxe);
			JAnim_PickAxe->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s4p2_axeuse");
			isAnimationPlaying = true;
		}
		else 
		{
			Hud->ShowBannerText("L5bark");
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L5S4_Pop2Pond::Required()
{

}

void L5S4_Pop2Pond::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();

	if(!ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken && InvRectBark.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked())
				{
					GetObjectByID("bark")->SetEnable(false);
					ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken = true;
					Hud->TakeInventoryItem(eInv_L5_S4_Woodbark, Control::Input->Mpos());

				}
		}
		else if(ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken && InvDestRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L5stream");

				}
		}
	}
	#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S4Watermelon] && MemRectWatermelon.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("watermelon")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L5S4Watermelon, &GetObjectByID("watermelon")->Pos);

			if(ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed)
				ClosePopup(2.0f);
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S4Watermelon] && MemRectWatermelon.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("watermelon")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L5S4Watermelon, &(GetObjectByID("watermelon")->Pos));
		}
	}
#endif 
	
}

float L5S4_Pop2Pond::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L5S4_Pop2Pond::~L5S4_Pop2Pond()
{
	SAFE_DELETE(JAnim_PickAxe);
}

void L5S4_Pop2Pond::TakeInventoryItem(EHOInventory ItemID)
{
	/*std::vector<int> vecInvIds;
	switch (ItemID)
	{
	
	default:
		break;
	}*/
}

void L5S4_Pop2Pond::ReceiveMessage(int val)
{	

	
}

bool L5S4_Pop2Pond::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken) 
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed && ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L5S4_Pop2Pond::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if( !ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken) 
		Hud->InitHintTrailEmit(InvRectBark.GetCenterX(), InvRectBark.GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed && ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeTaken)
		Hud->ShowHintForInvItem(eInv_L5_S4_Pickaxe, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
