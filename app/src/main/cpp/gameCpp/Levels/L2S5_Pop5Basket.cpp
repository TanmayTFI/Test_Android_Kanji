//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 1: Honey COMB
//====================================

#include "L2S5_Pop5Basket.h"
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


const CRectangle  PopBasketRect(588.0f,179.0f,332.0f,284.0f);
const CRectangle  MemoryRectStatue(840.0f,106.0f,78.0f,110.0f);


L2S5_Pop5Basket::L2S5_Pop5Basket(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	isBasketMoveAnimPlaying = false;
	//Popup load
	Load("LV2\\S5\\POP5\\POP5.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	
	Show(false);

	if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken )
	{
		GetObjectByID("Pot")->Show(false); 
		GetObjectByID("Pot")->SetEnable(false);
	}
	
	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue])
	{		
		GetObjectByID("memstatue")->Show(false);
	}

	SetSparkleUpdate(false);

	Basket = GetObjectByID("Basket");
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_StrawBasketRemoved )
	{
		Basket->SetEnable(false);
		return;
	}
	else
	{	
		CObject* objArr[] = {Basket, GetObjectByID("Pot") };
		std::string animArr[] = {"ANIM\\L2\\S5Pop5Basket.anim", "ANIM\\L2\\S5Pop5Pot.anim"};
		CAnim_BasketMove = new CustomAnimGroup(2, animArr, objArr);
	}

	

}

void L2S5_Pop5Basket::Required()
{

}

void L2S5_Pop5Basket::Update( float ds )
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;


	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if(PopBasketRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_StrawBasketRemoved )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s5p5_basketopen");
			CAnim_BasketMove->PlayAnim();
			ProfileSystem::Instance->ActiveProfile.L2_S5_StrawBasketRemoved = true;
			isBasketMoveAnimPlaying = true;
			Hud->SendUpdateSceneMessage();
			UpdateBgGraphic();
			return;
		}
	}

	if(PopBasketRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken && ProfileSystem::Instance->ActiveProfile.L2_S5_StrawBasketRemoved )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s5pz1_potplaced");
			TakeInventoryItem( eInv_L2S5_PotEmpty ); 
			UpdateBgGraphic();

#ifdef _CEBUILD
			if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue])
			{			
				ClosePopup(1.5f);
			}
#else
			ClosePopup(1.5f);
#endif 
			
			return;
		}
	}


	if (isBasketMoveAnimPlaying )
	{
		if( CAnim_BasketMove->IsAnimFinished() )
		{
			isBasketMoveAnimPlaying = false;
			Basket->SetEnable(false);
		}
		return;
	}

	
#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue] && MemoryRectStatue.Intersection(&pos))
	{			
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memstatue")->FadeOut(2.0f);
			Hud->ShowMemoryObjectCollected_CE(eMem_L2S5Statue, &GetObjectByID("memstatue")->Pos);
			if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken)
			{
				ClosePopup(1.5f);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue] && MemoryRectStatue.Intersection(&pos))
	{	
		//Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memstatue")->FadeOut(2.0f);
			Hud->ShowMemoryObjectCollected_SE(eMem_L2S5Statue, &GetObjectByID("memstatue")->Pos);
		}
	}
#endif 


}



float L2S5_Pop5Basket::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S5_Pop5Basket::~L2S5_Pop5Basket()
{
	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_StrawBasketRemoved )
		SAFE_DELETE(CAnim_BasketMove);
}

void L2S5_Pop5Basket::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	case eInv_L2S5_PotEmpty:

			GetObjectByID("Pot")->SetEnable(false);
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S5_PotEmpty] = true;
			SetSparkleUpdate(true);
			Hud->TakeInventoryItem(eInv_L2S5_PotEmpty);
		break;
	default:
		break;
	}
}

void L2S5_Pop5Basket::ReceiveMessage(int val)
{

}


bool L2S5_Pop5Basket::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken || !ProfileSystem::Instance->ActiveProfile.L2_S5_StrawBasketRemoved )
		isAnyTaskLeft = true;
	return isAnyTaskLeft;
}


void L2S5_Pop5Basket::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_StrawBasketRemoved  )
		Hud->InitHintTrailEmit(GetObjectByID("Basket")->GetCenterX(-85), GetObjectByID("Basket")->GetCenterY(),true);
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken)
		Hud->InitHintTrailEmit(GetObjectByID("Pot")->GetCenterX(-55), GetObjectByID("Pot")->GetCenterY(),true);
	else if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue])
		Hud->InitHintTrailEmit(870, 160, true);
	else
		Popup::ProcessHint();
}