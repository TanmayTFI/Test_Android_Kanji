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
#include "L3S2_Pop7MazeDoor.h"


const CRectangle  InvDestRect(461.0f,284.0f,407.0f,279.0f);
const CRectangle  ActionRectThread(589.0f,400.0f,85.0f,173.0f);
const CRectangle  PopupMaskRect(171.0f,82.0f,1030.0f,493.0f);

L3S2_Pop7MazeDoor::L3S2_Pop7MazeDoor(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S2\\POP7\\P7.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	isAnimationPlaying = false;

	GetObjectByID("brick2")->SetEnable(false);
	GetObjectByID("brick3")->SetEnable(false);
	GetObjectByID("coconutShell")->SetEnable(false);
	GetObjectByID("hand")->SetEnable(false);
	GetObjectByID("popfireanim")->SetEnable(false);
	GetObjectByID("popfireanim1")->SetScale(3.0f);
	GetObjectByID("thread")->SetEnable(false);
	GetObjectByID("thread1")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_BrickTaken)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_BrickFixed)
		{
			GetObjectByID("brick2")->SetEnable(false);
			GetObjectByID("brick1")->SetEnable(false);
			GetObjectByID("brick3")->SetEnable(true);
			GetObjectByID("hand")->SetEnable(true);
		}
		
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_CocoShellPowderFilledFixed)
	{
		GetObjectByID("coconutShell")->SetEnable(true);
		GetObjectByID("thread")->SetEnable(true);
	}

	GetObjectByID("hand")->SetMaskRect(PopupMaskRect);

	JAnim_BrickTake = new JSONAnimator(0, CC_CALLBACK_1(L3S2_Pop7MazeDoor::OnAnimationComplete, this));
	JAnim_BrickTake->parseAndLoadJSON("ANIM\\L3\\S2POP7BRICKTAKEANIM.JSON", this);

	JAnim_BrickPlace = new JSONAnimator(1, CC_CALLBACK_1(L3S2_Pop7MazeDoor::OnAnimationComplete, this));
	JAnim_BrickPlace->parseAndLoadJSON("ANIM\\L3\\S2POP7BRICKPLACEANIM.JSON", this);
	
	JAnim_BombLite = new JSONAnimator(2, CC_CALLBACK_1(L3S2_Pop7MazeDoor::OnAnimationComplete, this));
	JAnim_BombLite->parseAndLoadJSON("ANIM\\L3\\S2POP7BOMBLITE.JSON", this);

	Show(false);

}

void L3S2_Pop7MazeDoor::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		isAnimationPlaying = false;
		GetObjectByID("brick2")->SetEnable(false);
		GetObjectByID("brick1")->SetEnable(false);
		GetObjectByID("brick3")->SetEnable(true);
		GetObjectByID("hand")->SetEnable(true);
		ProfileSystem::Instance->ActiveProfile.L3_S2_BrickTaken = true;
		
		Cursor::Show(true);
	}
	else if(animID == 1)
	{
		isAnimationPlaying = false;
		GetObjectByID("brick2")->SetEnable(false);
		GetObjectByID("brick1")->SetEnable(true);
		GetObjectByID("brick3")->SetEnable(false);
		GetObjectByID("hand")->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L3_S2_BrickFixed = true;
		
	}
	else if(animID == 2)
	{
		isAnimationPlaying = false;
		ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken = true;
		
		Cursor::Show(true);
		ClosePopup(1.0f);
	}
	
}

void L3S2_Pop7MazeDoor::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L3S2_CoconutDynamite &&  InvDestRect.Intersection(&dropPos) && ProfileSystem::Instance->ActiveProfile.L3_S2_BrickTaken)
	{
		GetObjectByID("coconutShell")->SetEnable(true);
		GetObjectByID("thread")->SetEnable(true);
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 
		ProfileSystem::Instance->ActiveProfile.L3_S2_CocoShellPowderFilledFixed = true;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_CoconutDynamite] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_CoconutDynamite);
		Control::Audio->QuickLoadAndPlaySFX("s2p7_bombplaced");
		
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L3S2_Pop7MazeDoor::Required()
{

}

void L3S2_Pop7MazeDoor::Update(float ds)
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

	if(InvDestRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S2_BrickTaken)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			JAnim_BrickTake->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p7_bricktake");
			isAnimationPlaying = true;
			
		}
	}
	else if(InvDestRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L3_S2_CocoShellPowderFilledFixed && !ProfileSystem::Instance->ActiveProfile.L3_S2_BrickFixed)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			JAnim_BrickPlace->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p7_brickfixed");
			isAnimationPlaying = true;
			
		}
	}
	else if(ActionRectThread.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken && ProfileSystem::Instance->ActiveProfile.L3_S2_BrickFixed)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			JAnim_BombLite->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p7_bomblite");
			isAnimationPlaying = true;
			
		}
	}
	else if(ActionRectThread.Intersection(&pos) || InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L3blow");
		}
	}
}

float L3S2_Pop7MazeDoor::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S2_Pop7MazeDoor::~L3S2_Pop7MazeDoor()
{
	SAFE_DELETE(JAnim_BombLite);
	SAFE_DELETE(JAnim_BrickPlace);
	SAFE_DELETE(JAnim_BrickTake);
}

void L3S2_Pop7MazeDoor::ReceiveMessage(int val)
{	

	
}

bool L3S2_Pop7MazeDoor::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutWithTwineTaken && !ProfileSystem::Instance->ActiveProfile.L3_S2_CocoShellPowderFilledFixed) 
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken && ProfileSystem::Instance->ActiveProfile.L3_S2_CocoShellPowderFilledFixed)
		isAnyTaskLeft = true;
		

	return isAnyTaskLeft;
}

void L3S2_Pop7MazeDoor::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	 if(!ProfileSystem::Instance->ActiveProfile.L3_S2_BrickTaken )
		 Hud->InitHintTrailEmit( &InvDestRect);
	else if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutWithTwineTaken && !ProfileSystem::Instance->ActiveProfile.L3_S2_CocoShellPowderFilledFixed) 
		Hud->ShowHintForInvItem(eInv_L3S2_CoconutDynamite, &InvDestRect);
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_BrickFixed && ProfileSystem::Instance->ActiveProfile.L3_S2_CocoShellPowderFilledFixed)
		Hud->InitHintTrailEmit( &InvDestRect);
	else if(ProfileSystem::Instance->ActiveProfile.L3_S2_BrickFixed && !ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken)
		Hud->InitHintTrailEmit( &ActionRectThread);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
