//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 02 - POPUP 2: 
//====================================

#include "L6S2_Pop2Door.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

L6S2_Pop2Door::L6S2_Pop2Door(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S2\\P2\\P2.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("Door_04")->SetEnable(false);
	GetObjectByID("Door_03")->SetEnable(false);
	GetObjectByID("Door_02")->SetEnable(false);

	Janim_doorOpen = new JSONAnimator(0, CC_CALLBACK_1(L6S2_Pop2Door::OnAnimComplete,this));
	Janim_doorOpen->parseAndLoadJSON("ANIM\\L6\\S2_DOOROPEN.JSON", this);

	isAnimPlaying = false;

	SetSparkleUpdate(false);
	Show(false);
}


void L6S2_Pop2Door::Update(float ds)
{
	Popup::Update(ds);  
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	//if (ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated && !isAnimPlaying)
	//{
	//	isAnimPlaying = true;
	//	Janim_doorOpen->PlayAnim();
	//	ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened = true;
	//}

	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated && PopAreaRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s2p2_doorClick");
			Hud->ShowBannerText("L6place");
		}
	}

}

void L6S2_Pop2Door::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/*
	switch (itemVal)
	{
	
	default:
	*/
		GFInstance->WrongInvDropMsg(Hud);
		/*break;
	}
	*/
}

void L6S2_Pop2Door::OnAnimComplete(int animID)
{
	ClosePopup(1);
}

float L6S2_Pop2Door::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S2_Pop2Door::~L6S2_Pop2Door()
{
	SAFE_DELETE(Janim_doorOpen);
}

void L6S2_Pop2Door::Required()
{

}

void L6S2_Pop2Door::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L6S2_Pop2Door::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	/*if (ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated && !ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened)
	{
		isAnyTaskLeft = true;
	}*/

	return isAnyTaskLeft;
}

void L6S2_Pop2Door::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	/*if(ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated && !ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened)
	{
		Hud->InitHintTrailEmit(GetObjectByID("Door_01")->GetWorldRect().GetCenterX(),GetObjectByID("Door_01")->GetWorldRect().GetCenterY());
	}
	else*/
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
