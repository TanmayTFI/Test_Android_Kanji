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
#include "L3S2_Pop5BackDoor.h"


const CRectangle  InvDestRect(498.0f,298.0f,460.0f,147.0f);


L3S2_Pop5BackDoor::L3S2_Pop5BackDoor(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S2\\POP5\\P5.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	isAnimationPlaying = false;

	GetObjectByID("doorBg")->SetEnable(false);
	GetObjectByID("doorOpen1")->SetEnable(false);
	GetObjectByID("doorOpen2")->SetEnable(false);
	GetObjectByID("doorOpen3")->SetEnable(false);
	

	JAnim_Lock = new JSONAnimator(0, CC_CALLBACK_1(L3S2_Pop5BackDoor::OnAnimationComplete, this));
	JAnim_Lock->parseAndLoadJSON("ANIM\\L3\\S2POP5DOORANIM.JSON", this);
	

	Show(false);

}

void L3S2_Pop5BackDoor::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		Cursor::Show(true);
		ClosePopup(1.0f);
	}
	
}

void L3S2_Pop5BackDoor::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}


		GFInstance->WrongInvDropMsg(Hud);

}

void L3S2_Pop5BackDoor::Required()
{

}

void L3S2_Pop5BackDoor::Update(float ds)
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

	if(InvDestRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S2_BackDoorOpened)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L3ladder");
			JAnim_Lock->PlayAnim();
			isAnimationPlaying = true;
			ProfileSystem::Instance->ActiveProfile.L3_S2_BackDoorOpened = true;
		}
	}
}

float L3S2_Pop5BackDoor::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S2_Pop5BackDoor::~L3S2_Pop5BackDoor()
{
	SAFE_DELETE(JAnim_Lock);
}

void L3S2_Pop5BackDoor::ReceiveMessage(int val)
{	

	
}

bool L3S2_Pop5BackDoor::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L3_S2_BackDoorOpened) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L3S2_Pop5BackDoor::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if( !ProfileSystem::Instance->ActiveProfile.L3_S2_BackDoorOpened) 
		Hud->InitHintTrailEmit( &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
