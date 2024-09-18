//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 02 - POPUP 2: Frankinscenes
//====================================

#include "L2S2_Pop2Frankinscence.h"
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

L2S2_Pop2Frankinscence::L2S2_Pop2Frankinscence(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV2\\S2\\POP2\\P2.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	

	GetObjectByID("P2_scrapper")->SetEnable(false);
	GetObjectByID("P2_frankinscenceCup")->SetEnable(false);
	GetObjectByID("P2_frankinscence")->SetEnable(false);

	GetObjectByID("P2_frankinscence1")->SetEnable(false);
	GetObjectByID("P2_frankinscence2")->SetEnable(false);
	GetObjectByID("P2_frankinscence3")->SetEnable(false);

	JAnim_Frank = new JSONAnimator(0, CC_CALLBACK_1(L2S2_Pop2Frankinscence::OnAnimationComplete, this));
	JAnim_Frank->parseAndLoadJSON("ANIM\\L2\\S2FRANKPICK.JSON", this);

	if(ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced )
	{
		GetObjectByID("P2_frankinscenceCup")->SetEnable(true);
	}

	Show(false);

}

void L2S2_Pop2Frankinscence::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		Hud->TakeInventoryItem(eInv_L2S2_Frankinscence,GetObjectByID("P2_frankinscenceCup")->Pos);
		GetObjectByID("P2_scrapper")->SetEnable(false);
		GetObjectByID("P2_frankinscenceCup")->SetEnable(false);
		GetObjectByID("P2_frankinscence")->SetEnable(false);
		GetObjectByID("P2_frankinscence1")->SetEnable(false);
		GetObjectByID("P2_frankinscence2")->SetEnable(false);
		GetObjectByID("P2_frankinscence3")->SetEnable(false);
		isAnimationPlaying = false;

		ClosePopup(1.5f);
		
	}
}
void L2S2_Pop2Frankinscence::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
		return;

	if (itemVal == eInv_L2S5_Ember &&  InvDestRect.Intersection(&dropPos))
	{
		GetObjectByID("P2_frankinscenceCup")->SetEnable(true);
		
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced = true;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S5_Ember] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S5_Ember);
		Control::Audio->QuickLoadAndPlaySFX("s2p2_emberplaced");
		return;
	}
	else if (itemVal == eInv_L2S1_Scrapper &&  InvDestRect.Intersection(&dropPos))
	{

		if(ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced )
		{
		
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_Scrapper] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S1_Scrapper);

			isAnimationPlaying = true;

			JAnim_Frank->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p2_franktake");
		}
		else
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			Hud->ShowBannerText("L2sort");
		}
		return;
	}
	else if (itemVal == eInv_L2S5_PotEmpty &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced )
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory();
			Hud->ShowBannerText("L2pot");
			return;
		}
		GFInstance->WrongInvDropMsg(Hud);
	}
	else
		GFInstance->WrongInvDropMsg(Hud);

}

void L2S2_Pop2Frankinscence::Required()
{

}

void L2S2_Pop2Frankinscence::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(isAnimationPlaying)
		return;
	
	if(ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced )
	{
		if (InvDestRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2find");
			} 
		}
		return;
	}
	else
	{
		if (InvDestRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2frankin");
			} 
		}
		return;
	}
}

float L2S2_Pop2Frankinscence::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S2_Pop2Frankinscence::~L2S2_Pop2Frankinscence()
{
	SAFE_DELETE(JAnim_Frank);
}

void L2S2_Pop2Frankinscence::TakeInventoryItem(EHOInventory ItemID)
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

void L2S2_Pop2Frankinscence::ReceiveMessage(int val)
{	

	
}
bool L2S2_Pop2Frankinscence::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken && ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken && ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced)
		isAnyTaskLeft = true;
	

	return isAnyTaskLeft;
}

void L2S2_Pop2Frankinscence::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S5_Ember, &InvDestRect);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken && ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken && ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced)
	{
		Hud->ShowHintForInvItem(eInv_L2S1_Scrapper, &InvDestRect);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
