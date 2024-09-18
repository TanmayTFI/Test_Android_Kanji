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
#include "L3S5_Pop4Clip.h"

const CRectangle  InvDestRect(368.0f,99.0f,626.0f,458.0f);
const CRectangle  PopupMaskRect(190.0f,88.0f,818.0f,490.0f);
const CRectangle  InvRectHammerHandle(581.0f,381.0f,224.0f,89.0f);

const CRectangle  InfoRectCliff(377.0f,111.0f,581.0f,188.0f);


const CRectangle  MemoryRectEasterEgg(396.0f,354.0f,79.0f,86.0f);

L3S5_Pop4Clip::L3S5_Pop4Clip(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S5\\POP4\\P4.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	JAnim_Clip = new JSONAnimator(0, CC_CALLBACK_1(L3S5_Pop4Clip::OnAnimationComplete, this));
	JAnim_Clip->parseAndLoadJSON("ANIM\\L3\\S5P4NAILANIM.JSON", this);

	JAnim_Knot = new JSONAnimator(1, CC_CALLBACK_1(L3S5_Pop4Clip::OnAnimationComplete, this));
	JAnim_Knot->parseAndLoadJSON("ANIM\\L3\\S5P4KNOTANIM.JSON", this);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced)
	{
		GetObjectByID("clip")->SetEnable(false);
		GetObjectByID("clipShadow")->SetEnable(false);
		GetObjectByID("mask")->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered && ProfileSystem::Instance->ActiveProfile.L3_S5_NailPlaced) 
		GetObjectByID("nail")->SetEnable(true);
	else
		GetObjectByID("nail")->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered)
		GetObjectByID("nailHead")->SetEnable(false);
	
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied)
	{
		GetObjectByID("knote")->SetEnable(false);
		GetObjectByID("knot1")->SetEnable(false);
		GetObjectByID("rope")->SetEnable(false);
		GetObjectByID("ropeShadow")->SetEnable(false);
	}

	GetObjectByID("hammer")->SetEnable(false);
	GetObjectByID("hammer")->SetZ(GetObjectByID("hammer")->ZOrder+120);

	Show(false);


	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S5EasterEgg])
	{
		GetObjectByID("easteregg")->SetEnable(false);
	}
}

void L3S5_Pop4Clip::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		Hud->ShowBannerText("L3rope");
		isAnimationPlaying = false;
		Cursor::Show(true);
	}
	else if(animID == 1)
	{

		isAnimationPlaying = false;
		Cursor::Show(true);
		ClosePopup(1.0f);

		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O4_FindWayFromBrokenCastle] && 
			!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L3O4_FindWayFromBrokenCastle] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L3O4_FindWayFromBrokenCastle] = true;
			Hud->ShowObjectiveFX(eObj_L3O4_FindWayFromBrokenCastle,true);
		}
	}
	
}

void L3S5_Pop4Clip::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}
	if (itemVal == eInv_L3S5_MetalClip &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			GetObjectByID("clip")->SetEnable(true);
			GetObjectByID("clipShadow")->SetEnable(true);
			GetObjectByID("mask")->SetEnable(true);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S5_MetalClip] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S5_MetalClip);
			ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced = true;
			Control::Audio->QuickLoadAndPlaySFX("s5p4_clipused");
		}
		return;
	}
	else if (itemVal == eInv_L3S4_Nail &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_NailPlaced && ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			GetObjectByID("nail")->SetEnable(true);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_Nail] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S4_Nail);
			ProfileSystem::Instance->ActiveProfile.L3_S5_NailPlaced = true;
			Control::Audio->QuickLoadAndPlaySFX("s5p4_nailused");
		}
		else if(! ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced)
		{
			Hud->ShowBannerText("L3right");
			Hud->CancelDrag();
		}
		return;
	}
	else if (itemVal == eInv_L3S2_Hammer &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered && ProfileSystem::Instance->ActiveProfile.L3_S5_NailPlaced)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_Hammer] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_Hammer);
			ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered = true;
			JAnim_Clip->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p4_hammerused");
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		return;
	}
	else if (itemVal == eInv_L3S5_Rope &&  InvDestRect.Intersection(&dropPos))
	{

		if(ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered && !ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S5_Rope] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S5_Rope);
			ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied = true;
			JAnim_Knot->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p4_ropeused");
		}
		else
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory();
			Hud->ShowBannerText("L3tie");
			Hud->CancelDrag();
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);

}

void L3S5_Pop4Clip::Required()
{

}

void L3S5_Pop4Clip::Update(float ds)
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

	if(InfoRectCliff.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
				Hud->ShowBannerText("L3way");
		}
	}

	if(InvRectHammerHandle.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced && !ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered)
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
				Hud->ShowBannerText("L3metal");
		}
	}
	else if(InvRectHammerHandle.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied)
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
				Hud->ShowBannerText("L3far");
		}
	}
	
#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S5EasterEgg] && MemoryRectEasterEgg.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("easteregg")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L3S5EasterEgg, &GetObjectByID("easteregg")->Pos);
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S5EasterEgg] && MemoryRectEasterEgg.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("easteregg")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L3S5EasterEgg, &GetObjectByID("easteregg")->Pos);
		}
	}
#endif 
	
}

float L3S5_Pop4Clip::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S5_Pop4Clip::~L3S5_Pop4Clip()
{
	SAFE_DELETE(JAnim_Clip);
	SAFE_DELETE(JAnim_Knot);
}

void L3S5_Pop4Clip::ReceiveMessage(int val)
{	

	
}

bool L3S5_Pop4Clip::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	
		
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced && ProfileSystem::Instance->ActiveProfile.L3_S5_InvClipTaken) 
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_NailPlaced && ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced && ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted) 
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered && ProfileSystem::Instance->ActiveProfile.L3_S5_NailPlaced && ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerTaken) 
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied && ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered && ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L3S5_Pop4Clip::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced && ProfileSystem::Instance->ActiveProfile.L3_S5_InvClipTaken) 
		Hud->ShowHintForInvItem(eInv_L3S5_MetalClip, &InvDestRect);
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_NailPlaced && ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced && ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted) 
		Hud->ShowHintForInvItem(eInv_L3S4_Nail, &InvDestRect);
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered && ProfileSystem::Instance->ActiveProfile.L3_S5_NailPlaced && ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerTaken) 
		Hud->ShowHintForInvItem(eInv_L3S2_Hammer, &InvDestRect);
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied && ProfileSystem::Instance->ActiveProfile.L3_S5_NailHammered && ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken) 
		Hud->ShowHintForInvItem(eInv_L3S5_Rope, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
