//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 03 - POPUP 1: Honey COMB
//====================================

#include "L4S1_Pop3Pocket.h"
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


L4S1_Pop3Pocket::L4S1_Pop3Pocket(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	
	//Popup load
	Load("LV4\\S1\\POP3\\POP3.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	
	SetSparkleUpdate(false);
	Show(false);

	Kerchief = GetObjectByID("Kerchief_Mask");
	Open_state = GetObjectByID("Pocket_Open");
	Closed_state = GetObjectByID("Pocket_Closed");

	if (!ProfileSystem::Instance->ActiveProfile.L4_S1_PocketOpend)
	{
		Kerchief->SetEnable(false); 
		Open_state->SetEnable(false);
	}
	else
	{
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvHandKerchiefTaken)
			Kerchief->SetEnable(false); 
		Closed_state->SetEnable(false);
	}

	GetObjectByID("Pocket_HalfOpen")->SetEnable(false);

	Janim_pocketopen = new JSONAnimator();
	Janim_pocketopen->parseAndLoadJSON("ANIM\\L4\\L4_S1_POP3_PKT_HO.JSON", this);

}

void L4S1_Pop3Pocket::Required()
{

}

void L4S1_Pop3Pocket::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}


void L4S1_Pop3Pocket::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (Closed_state->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_PocketOpend/* && !isAnimPlaying*/)
	{
		Cursor::SetMode(CA_HandPointy);
		if (Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p3_pocketOpen");
			Janim_pocketopen->PlayAnim();
			ProfileSystem::Instance->ActiveProfile.L4_S1_PocketOpend = true;
		}
	}
	else if (Kerchief->GetWorldRect().Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L4_S1_PocketOpend && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvHandKerchiefTaken)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			Kerchief->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L4_S1_InvHandKerchiefTaken = true;
			Hud->TakeInventoryItem(eInv_L4_CB_HandKerchief);
			ClosePopup(2.0f);
		}
	}

}

float L4S1_Pop3Pocket::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S1_Pop3Pocket::~L4S1_Pop3Pocket()
{
	SAFE_DELETE(Janim_pocketopen);
}

void L4S1_Pop3Pocket::ReceiveMessage(int val)
{	

	
}

bool L4S1_Pop3Pocket::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L4_S1_InvHandKerchiefTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S1_Pop3Pocket::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L4_S1_PocketOpend)
		Hud->InitHintTrailEmit(Closed_state->GetWorldRect().GetCenterX(), Closed_state->GetWorldRect().GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvHandKerchiefTaken)
		Hud->InitHintTrailEmit(Kerchief->GetWorldRect().GetCenterX(), Kerchief->GetWorldRect().GetCenterY());
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}