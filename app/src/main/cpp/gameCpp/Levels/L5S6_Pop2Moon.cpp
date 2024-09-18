//====================================
//	DQFYLH
//	HO System
//	Level 5 - Scene 06 - POPUP 2: Moon
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
#include "L5S6_Pop2Moon.h"

const CRectangle  InvDestRect(465.0f,159.0f,434.0f,392.0f);

L5S6_Pop2Moon::L5S6_Pop2Moon(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV5\\S6\\POP2\\P2.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("hand")->SetEnable(false);
	GetObjectByID("locket")->SetEnable(false);
	
	
	std::vector<SuperFX*> superfxArray;
	S6P2LocketFX = new SuperFX("PRT\\L5\\S6P2Locket.INI", PointSystem::CreateCPoint(668,471,0), 1100); 
	superfxArray.push_back(S6P2LocketFX);



	JAnim_Locket = new JSONAnimator(0, CC_CALLBACK_1(L5S6_Pop2Moon::OnAnimationComplete, this));
	JAnim_Locket->parseAndLoadJSON("ANIM\\L5\\S6P2LOCKET.JSON", this, &superfxArray);

	Show(false);

}

void L5S6_Pop2Moon::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		
		Cursor::Show(true);
		
		ClosePopup(1.0f);
		
	}
	
}

void L5S6_Pop2Moon::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L5_S4_PeacockChain &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockChainUsed )
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockChainUsed = true;
			Hud->AllowInput = false;
			JAnim_Locket->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s6p2_necklaceused");
			isAnimationPlaying = true;
		}
		
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L5S6_Pop2Moon::Required()
{

}

void L5S6_Pop2Moon::Update(float ds)
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

	if(!ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockChainUsed && InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
			{
				if(ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken)
					Hud->ShowBannerText("L5lathika");
				else
					Hud->ShowBannerText("L5big");

			}
	}

}

L5S6_Pop2Moon::~L5S6_Pop2Moon()
{
	SAFE_DELETE(JAnim_Locket);
	SAFE_DELETE(S6P2LocketFX);
}

void L5S6_Pop2Moon::ReceiveMessage(int val)
{	

	
}

bool L5S6_Pop2Moon::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if( !ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockChainUsed && ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L5S6_Pop2Moon::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if( !ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockChainUsed && ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken) 
		Hud->ShowHintForInvItem(eInv_L5_S4_PeacockChain, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
