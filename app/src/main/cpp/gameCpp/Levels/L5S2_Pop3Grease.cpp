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
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "InteractiveObjectList.h"
#include "exMath.h"
#include "L5S2_Pop3Grease.h"

const CRectangle  InvDestRect(479.0f,295.0f,340.0f,206.0f);
const CRectangle  MemRectDog(823.0f,319.0f,106.0f,135.0f);

L5S2_Pop3Grease::L5S2_Pop3Grease(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV5\\S2\\POP3\\P3.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("treeBarkWithGrease")->SetEnable(false);
	GetObjectByID("treeBark")->SetEnable(false);
	

	JAnim_Grease = new JSONAnimator(0, CC_CALLBACK_1(L5S2_Pop3Grease::OnAnimationComplete, this));
	JAnim_Grease->parseAndLoadJSON("ANIM\\L5\\S2GREASETAKKING.JSON", this);

	Show(false);


	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue])
	{
		GetObjectByID("memdog")->SetEnable(false);
	}


}

void L5S2_Pop3Grease::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		
		Cursor::Show(true);
		GetObjectByID("treeBarkWithGrease")->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L5_S2_GreasySapBark,GetObjectByID("treeBarkWithGrease")->Pos);
		#ifdef _CEBUILD
			if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue])
			{
				ClosePopup(1.0f);
			}
		#else
				ClosePopup(1.0f);
		#endif 
	}
	
}

void L5S2_Pop3Grease::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L5_S4_Woodbark &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken )
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S4_Woodbark] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S4_Woodbark);
			JAnim_Grease->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p3_greasetake");
			isAnimationPlaying = true;
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

void L5S2_Pop3Grease::Required()
{

}

void L5S2_Pop3Grease::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
		return;

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();

	if( !ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken &&  InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			Hud->ShowBannerText("L5greece");
		}
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue] && MemRectDog.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memdog")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L5S2DogStatue, &GetObjectByID("memdog")->Pos);

			if(ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken)
				ClosePopup(1.0f);
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue] && MemRectDog.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memdog")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L5S2DogStatue, &(GetObjectByID("memdog")->Pos));
		}
	}
#endif 
}

L5S2_Pop3Grease::~L5S2_Pop3Grease()
{
	SAFE_DELETE(JAnim_Grease);
}

void L5S2_Pop3Grease::ReceiveMessage(int val)
{	

	
}

bool L5S2_Pop3Grease::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken && ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L5S2_Pop3Grease::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if( !ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken && ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken) 
		Hud->ShowHintForInvItem(eInv_L5_S4_Woodbark, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
