//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 01 - POPUP 4: CRUTCH
//====================================

#include "L4S1_Pop4Crutch.h"


L4S1_Pop4Crutch::L4S1_Pop4Crutch(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV4\\S1\\POP4\\P4.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	SetSparkleUpdate(false);
	Show(false);

	Crutch = GetObjectByID("Crutch");

}

void L4S1_Pop4Crutch::Required()
{

}

void L4S1_Pop4Crutch::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}


void L4S1_Pop4Crutch::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (Crutch->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvCrutchTaken)
	{
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
		{
			Cursor::SetMode(CA_HandTake);
			if (Control::Input->LBclicked())
			{
				Crutch->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L4_S1_InvCrutchTaken = true;
				Hud->TakeInventoryItem(eInv_L4_S1_Crutches);
				ClosePopup(2.0f);
			}
		}
		else
		{
			if (Control::Input->LBclicked())
				Hud->ShowBannerText("L4branch");
		}
	}
}

float L4S1_Pop4Crutch::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S1_Pop4Crutch::~L4S1_Pop4Crutch()
{
}

bool L4S1_Pop4Crutch::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L4_S1_InvCrutchTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S1_Pop4Crutch::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvCrutchTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
	{
		Hud->InitHintTrailEmit(Crutch->GetWorldRect().GetCenterX(), Crutch->GetWorldRect().GetCenterY());
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
