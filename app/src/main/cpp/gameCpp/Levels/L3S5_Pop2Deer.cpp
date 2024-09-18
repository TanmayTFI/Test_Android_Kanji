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
#include "L3S5_Pop2Deer.h"

const CRectangle  InvDestRect(368.0f,99.0f,626.0f,458.0f);
const CRectangle  InvRectHammerHandle(581.0f,381.0f,224.0f,89.0f);
const CRectangle  PopupMaskRect(362.0f,87.0f,648.0f,490.0f);



L3S5_Pop2Deer::L3S5_Pop2Deer(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S5\\POP2\\P2.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("handSleev")->SetEnable(false);
	GetObjectByID("hand")->SetEnable(false);
	GetObjectByID("grassInMouth2")->SetEnable(false);

	GetObjectByID("grassInMouth1")->SetEnable(false);
	GetObjectByID("grassInHand")->SetEnable(false);
	GetObjectByID("grass")->SetEnable(false);
	GetObjectByID("deerMouthOpen")->SetEnable(false);

	GetObjectByID("deerTail")->SetEnable(false);
	GetObjectByID("deerRightEar")->SetEnable(false);
	GetObjectByID("deerNeck")->SetEnable(false);

	GetObjectByID("deerLeftEar")->SetEnable(false);
	GetObjectByID("deerHead")->SetEnable(false);
	GetObjectByID("deerFrontRightLegTop")->SetEnable(false);

	GetObjectByID("deerFrontRightLegBottom")->SetEnable(false);
	GetObjectByID("deerFrontLeftLegTop")->SetEnable(false);
	GetObjectByID("deerFrontLeftLegBottom")->SetEnable(false);

	GetObjectByID("deerBody")->SetEnable(false);
	GetObjectByID("deerBlibnk")->SetEnable(false);
	GetObjectByID("deerBackRightLegTop")->SetEnable(false);

	GetObjectByID("deerBackRightLegBottom")->SetEnable(false);
	GetObjectByID("deerBackLeftLegTop")->SetEnable(false);
	GetObjectByID("deerBackLeftLegBottom")->SetEnable(false);

	JAnim_DeerLoop = new JSONAnimator(0, CC_CALLBACK_1(L3S5_Pop2Deer::OnAnimationComplete, this));
	JAnim_DeerLoop->parseAndLoadJSON("ANIM\\L3\\S5P2DEERLOOPANIM.JSON", this);

	JAnim_DeerRun = new JSONAnimator(1, CC_CALLBACK_1(L3S5_Pop2Deer::OnAnimationComplete, this));
	JAnim_DeerRun->parseAndLoadJSON("ANIM\\L3\\S5P2DEERWALKINGANIM.JSON", this);

	JAnim_DeerAngry = new JSONAnimator(2, CC_CALLBACK_1(L3S5_Pop2Deer::OnAnimationComplete, this));
	JAnim_DeerAngry->parseAndLoadJSON("ANIM\\L3\\S5P2DEERANGRY.JSON", this);


	GetObjectByID("deerFrontRightLegTop")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerFrontRightLegBottom")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerFrontLeftLegTop")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerFrontLeftLegBottom")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerBackRightLegTop")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerBackRightLegBottom")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerBackLeftLegTop")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerBackLeftLegBottom")->SetMaskRect(PopupMaskRect);

	GetObjectByID("handSleev")->SetMaskRect(PopupMaskRect);
	GetObjectByID("hand")->SetMaskRect(PopupMaskRect);
	GetObjectByID("grassInMouth2")->SetMaskRect(PopupMaskRect);

	GetObjectByID("grassInMouth1")->SetMaskRect(PopupMaskRect);
	GetObjectByID("grassInHand")->SetMaskRect(PopupMaskRect);
	GetObjectByID("grass")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerMouthOpen")->SetMaskRect(PopupMaskRect);

	GetObjectByID("deerTail")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerRightEar")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerNeck")->SetMaskRect(PopupMaskRect);

	GetObjectByID("deerLeftEar")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerHead")->SetMaskRect(PopupMaskRect);

	GetObjectByID("deerBody")->SetMaskRect(PopupMaskRect);
	GetObjectByID("deerBlibnk")->SetMaskRect(PopupMaskRect);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone)
	{
		JAnim_DeerLoop->PlayAnim();
		GetObjectByID("grassInMouth2")->SetEnable(false);
		GetObjectByID("grassInMouth1")->SetEnable(false);
		GetObjectByID("deerMouthOpen")->SetEnable(false);	
	}
	
	Show(false);

}

void L3S5_Pop2Deer::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone)
		{
			JAnim_DeerLoop->PlayAnim();
			GetObjectByID("grassInMouth2")->SetEnable(false);
			GetObjectByID("grassInMouth1")->SetEnable(false);
			GetObjectByID("deerMouthOpen")->SetEnable(false);
		}
	}
	else if(animID == 1)
	{
			Hud->SendUpdateSceneMessage();
			ClosePopup(1.0f);
		
	}
	
}

void L3S5_Pop2Deer::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/*if(isAnimationPlaying)
	{
		return;
	}*/

	if (itemVal == eInv_L3S5_Grass1 &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone && ProfileSystem::Instance->ActiveProfile.L3_S5_InvAllGrassTaken)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S5_Grass1] = false;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S5_Grass2] = false;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S5_Grass3] = false;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S5_Grass4] = false;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S5_Grass5] = false;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S5_Grass6] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S5_Grass1);
			JAnim_DeerRun->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p2_deerfed");
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
	

}

void L3S5_Pop2Deer::Required()
{

}

void L3S5_Pop2Deer::Update(float ds)
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
	
	if(InvDestRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone)
	{
		Cursor::SetMode(CA_Gear);

		if( Control::Input->LBclicked() )
		{
			Hud->ShowBannerText("L3move");
			JAnim_DeerAngry->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p2_deerclicked");
			GetObjectByID("grassInMouth2")->SetEnable(false);
			GetObjectByID("grassInMouth1")->SetEnable(false);
			GetObjectByID("deerMouthOpen")->SetEnable(false);
		}
	}
	
}

float L3S5_Pop2Deer::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S5_Pop2Deer::~L3S5_Pop2Deer()
{
	SAFE_DELETE(JAnim_DeerLoop);
	SAFE_DELETE(JAnim_DeerRun);
	SAFE_DELETE(JAnim_DeerAngry);
	
}

void L3S5_Pop2Deer::ReceiveMessage(int val)
{	

	
}

bool L3S5_Pop2Deer::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	 if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvAllGrassTaken && !ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L3S5_Pop2Deer::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	 if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvAllGrassTaken && !ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone) 
		Hud->ShowHintForInvItem(eInv_L3S5_Grass1, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
