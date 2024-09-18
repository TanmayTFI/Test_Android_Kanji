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
#include "L5S2_Pop1Skelton.h"

const CRectangle  InvRectBadge(587.0f,290.0f,230.0f,222.0f);
const CRectangle  InfoRectSkelton(376.0f,147.0f,627.0f,173.0f);
const CRectangle  InfoRectSkelton1(376.0f,319.0f,228.0f,238.0f);


const CRectangle  MemoryRectLocket(842.0f,321.0f,76.0f,115.0f);L5S2_Pop1Skelton::L5S2_Pop1Skelton(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV5\\S2\\POP1\\P1.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);


	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone])
		GetObjectByID("memlocket")->SetEnable(false);
	
	if( ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken )
		GetObjectByID("amulet")->SetEnable(false);

	Show(false);

}

void L5S2_Pop1Skelton::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		Cursor::Show(true);
	}
	
}

void L5S2_Pop1Skelton::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}


		GFInstance->WrongInvDropMsg(Hud);

}

void L5S2_Pop1Skelton::Required()
{

}

void L5S2_Pop1Skelton::Update(float ds)
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
	
	
	if(!ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken && InvRectBadge.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
			{
				GetObjectByID("amulet")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken = true;
				ProfileSystem::Instance->ActiveProfile.L5_S2_InvSoldierBadgeTaken = true;
				Hud->TakeInventoryItem(eInv_L5_S2_SoldierBadge, Control::Input->Mpos());

				#ifdef _CEBUILD
					if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone])
						ClosePopup(1.0f);
				#else
					ClosePopup(1.0f);
				#endif 
			}
	}
	else if(InfoRectSkelton.Intersection(&pos) || InfoRectSkelton1.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			Hud->ShowBannerText("L5died");
		}
	}
#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone] && MemoryRectLocket.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memlocket")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L5S2FootStone, &GetObjectByID("memlocket")->Pos);
			
			if (ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken)
				ClosePopup(1.0f);
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone] && MemoryRectLocket.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memlocket")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L5S2FootStone, &(GetObjectByID("memlocket")->Pos));
		}
	}
#endif 
}

float L5S2_Pop1Skelton::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L5S2_Pop1Skelton::~L5S2_Pop1Skelton()
{
	
}

void L5S2_Pop1Skelton::TakeInventoryItem(EHOInventory ItemID)
{
	/*std::vector<int> vecInvIds;
	switch (ItemID)
	{
	
	default:
		break;
	}*/
}

void L5S2_Pop1Skelton::ReceiveMessage(int val)
{	

	
}

bool L5S2_Pop1Skelton::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken )
		isAnyTaskLeft = true;
	

	return isAnyTaskLeft;
}

void L5S2_Pop1Skelton::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken )
		Hud->InitHintTrailEmit(InvRectBadge.GetCenterX(), InvRectBadge.GetCenterY());
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
