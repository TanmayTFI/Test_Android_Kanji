//====================================
//	DQFYLH
//	HO System
//	Level 5 - Scene 06 - POPUP 1: Peacock
//====================================

#include "L5_S6_MountainTop.h"
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
#include "L5S6_Pop1Peacock.h"

const CRectangle  InvDestRect(619.0f,385.0f,310.0f,190.0f);


L5S6_Pop1Peacock::L5S6_Pop1Peacock(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV5\\S6\\POP1\\P1.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("locket")->SetEnable(false);
	
	
	std::vector<SuperFX*> superfxArray;
	S6P1LocketprtFX = new SuperFX("PRT\\L5\\S6P1Locketprt.INI", PointSystem::CreateCPoint(683,384,0), 1100); 
	superfxArray.push_back(S6P1LocketprtFX);

	

	JAnim_Locket = new JSONAnimator(0, CC_CALLBACK_1(L5S6_Pop1Peacock::OnAnimationComplete, this));
	JAnim_Locket->parseAndLoadJSON("ANIM\\L5\\S6P1PEACOCK.JSON", this, &superfxArray);

	Show(false);



}

void L5S6_Pop1Peacock::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		
		Cursor::Show(true);
		
		ClosePopup(1.0f);
		
	}
	
}

void L5S6_Pop1Peacock::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L5_S6_Locket &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed )
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S6_Locket] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S6_Locket);
			JAnim_Locket->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s6p1_locketused");
			isAnimationPlaying = true;
			
		}
		
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L5S6_Pop1Peacock::Required()
{

}

void L5S6_Pop1Peacock::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();

	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed && InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L5empty");

			}
	}
}

L5S6_Pop1Peacock::~L5S6_Pop1Peacock()
{
	SAFE_DELETE(JAnim_Locket);
	SAFE_DELETE(S6P1LocketprtFX);
}

void L5S6_Pop1Peacock::ReceiveMessage(int val)
{	

	
}

bool L5S6_Pop1Peacock::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed &&  ProfileSystem::Instance->ActiveProfile.L5_S6_InvPeacockLocketTaken) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L5S6_Pop1Peacock::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if( !ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockLocketUsed &&  ProfileSystem::Instance->ActiveProfile.L5_S6_InvPeacockLocketTaken) 
		Hud->ShowHintForInvItem(eInv_L5_S6_Locket, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
