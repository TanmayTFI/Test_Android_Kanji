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
#include "L3S2_Pop6GunPowder.h"


const CRectangle  InvDestRect(779.0f,221.0f,222.0f,174.0f);
const CRectangle  InvItemRectSickle(413.0f,226.0f,301.0f,336.0f);

const CRectangle  memoryRectSun(572.0f,210.0f,77.0f,53.0f);


L3S2_Pop6GunPowder::L3S2_Pop6GunPowder(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S2\\POP6\\P6.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	isAnimationPlaying = false;

	GetObjectByID("emptyShell")->SetEnable(false);
	GetObjectByID("shellWithGunPowder")->SetEnable(false);
	GetObjectByID("shellTurned")->SetEnable(false);
	GetObjectByID("stoneTurn2")->SetEnable(false);
	GetObjectByID("stoneTurn3")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_RockMoved)
	{
		GetObjectByID("stoneTurn3")->SetEnable(true);
		GetObjectByID("stoneTurn1")->SetEnable(false);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken)
		GetObjectByID("sickle")->SetEnable(false);

	JAnim_Stone = new JSONAnimator(0, CC_CALLBACK_1(L3S2_Pop6GunPowder::OnAnimationComplete, this));
	JAnim_Stone->parseAndLoadJSON("ANIM\\L3\\S2POP6STONEANIM.JSON", this);

	JAnim_Powder = new JSONAnimator(1, CC_CALLBACK_1(L3S2_Pop6GunPowder::OnAnimationComplete, this));
	JAnim_Powder->parseAndLoadJSON("ANIM\\L3\\S2POP6GUNPOWDERANIM.JSON", this);
	

	Show(false);


	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing])
	{
		GetObjectByID("sunmemory")->SetEnable(false);
	}

}

void L3S2_Pop6GunPowder::OnAnimationComplete(int animID)
{
	if(animID == 1)
	{
		isAnimationPlaying = false;
		GetObjectByID("emptyShell")->SetEnable(false);
		GetObjectByID("shellWithGunPowder")->SetEnable(false);
		GetObjectByID("shellTurned")->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L3S2_CocoShellPowderFilled, GetObjectByID("shellWithGunPowder")->Pos);
		ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken = true;
		Cursor::Show(true);
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken)
		{
			#ifdef _CEBUILD
				if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing])
				{			
					ClosePopup(1.0f);
				}
			#else
					ClosePopup(1.0f);
			#endif 
		}
	}
	else if(animID == 0)
	{
		isAnimationPlaying = false;
		ProfileSystem::Instance->ActiveProfile.L3_S2_RockMoved = true;
	}
	
}

void L3S2_Pop6GunPowder::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L3S2_CoconutShell &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_CoconutShell] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_CoconutShell);
			JAnim_Powder->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p6_gunpowdertake");
			isAnimationPlaying = true;
		}
		else 
		{
			Hud->ShowBannerText("L3powder");
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);

}

void L3S2_Pop6GunPowder::Required()
{

}

void L3S2_Pop6GunPowder::Update(float ds)
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

	if(InvItemRectSickle.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S2_RockMoved)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			JAnim_Stone->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p6_brickmoved");
			isAnimationPlaying = true;
		}
	}
	else if(InvItemRectSickle.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L3_S2_RockMoved && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("sickle")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L3S2_Sickle, GetObjectByID("sickle")->Pos);
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken = true;
			if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken)
			{
				#ifdef _CEBUILD
					if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing])
					{			
						ClosePopup(1.0f);
					}
				#else
						ClosePopup(1.0f);
				#endif 
			}
		}
	}
	else if(InvDestRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken)
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L3gun");
		}
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing] && memoryRectSun.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("sunmemory")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L3S2SunDrawing, &GetObjectByID("sunmemory")->Pos);
			if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken && ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken)
			{
				ClosePopup(1.5f);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing] && memoryRectSun.Intersection(&pos))
	{	
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("sunmemory")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L3S2SunDrawing, &GetObjectByID("sunmemory")->Pos);
		}
	}
#endif 
	
}

float L3S2_Pop6GunPowder::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S2_Pop6GunPowder::~L3S2_Pop6GunPowder()
{
	SAFE_DELETE(JAnim_Stone);
	SAFE_DELETE(JAnim_Powder);
}

void L3S2_Pop6GunPowder::ReceiveMessage(int val)
{	

	
}

bool L3S2_Pop6GunPowder::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutShellTaken && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken) 
		isAnyTaskLeft = true;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L3S2_Pop6GunPowder::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutShellTaken && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken) 
		Hud->ShowHintForInvItem(eInv_L3S2_CoconutShell, &InvDestRect);
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken)
		Hud->InitHintTrailEmit( &InvItemRectSickle);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
