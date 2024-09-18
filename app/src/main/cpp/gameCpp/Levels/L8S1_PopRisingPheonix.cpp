//====================================
//	DQFYLH
//	HO System
//	Level 8 - Scene 01 - POPUP: 
//====================================

#include "L8S1_PopRisingPheonix.h"


L8S1_PopRisingPheonix::L8S1_PopRisingPheonix(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV8\\S1\\POP\\POP.SCN");
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	SetSparkleUpdate(false);
	Show(false);
}


void L8S1_PopRisingPheonix::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
	if (Control::Input->LBclicked())
		ClosePopup();
}

void L8S1_PopRisingPheonix::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/* switch (itemVal)
	{
	
	default:
	*/
		GFInstance->WrongInvDropMsg(Hud);
		/*break;
	}*/
}

void L8S1_PopRisingPheonix::OnAnimComplete(int animID)
{

}

float L8S1_PopRisingPheonix::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L8S1_PopRisingPheonix::~L8S1_PopRisingPheonix()
{
}

void L8S1_PopRisingPheonix::Required()
{

}

void L8S1_PopRisingPheonix::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L8S1_PopRisingPheonix::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	/*if (true)
	{
		isAnyTaskLeft = true;
	}*/

	return isAnyTaskLeft;
}

void L8S1_PopRisingPheonix::OnPopupOpened()
{
	/*CPoint endPos(220, 654, 0);
	SetZoomOrigin(endPos);*/
}

void L8S1_PopRisingPheonix::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	//if(true)
	//{
	//	//
	//}
	//else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}