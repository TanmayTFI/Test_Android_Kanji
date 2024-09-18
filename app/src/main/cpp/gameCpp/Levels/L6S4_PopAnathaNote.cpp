//====================================
//	DQFYLH
//	HO System
//	Level 8 - Scene 01 - POPUP: 
//====================================

#include "L6S4_PopAnathaNote.h"


L6S4_PopAnathaNote::L6S4_PopAnathaNote(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV6\\S4\\PZ1\\POP\\POP.SCN");	

	SetZOff(zOrder);
	//SetPosOff(683.0f, 334.0f);
	Control::Audio->LoadSample(aVOL6NoteVOS4_3,	AudioVO);
	SetSparkleUpdate(false);
	Show(false);
}


void L6S4_PopAnathaNote::Update(float ds)
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

void L6S4_PopAnathaNote::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/* switch (itemVal)
	{
	
	default:
	*/
		GFInstance->WrongInvDropMsg(Hud);
		/*break;
	}*/
}

void L6S4_PopAnathaNote::OnAnimComplete(int animID)
{

}

float L6S4_PopAnathaNote::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S4_PopAnathaNote::~L6S4_PopAnathaNote()
{
	Control::Audio->UnloadSample(aVOL6NoteVOS4_3);
}

void L6S4_PopAnathaNote::Required()
{

}

void L6S4_PopAnathaNote::ClosePopup(float delayTime)
{
	Control::Audio->StopSample(aVOL6NoteVOS4_3);
	Popup::ClosePopup(delayTime);
}

bool L6S4_PopAnathaNote::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	/*if (true)
	{
		isAnyTaskLeft = true;
	}*/

	return isAnyTaskLeft;
}

void L6S4_PopAnathaNote::OnPopupOpened()
{
	Control::Audio->PlaySample(aVOL6NoteVOS4_3);
	CPoint endPos(220, 654, 0);
	SetZoomOrigin(endPos);
}

void L6S4_PopAnathaNote::ProcessHint()
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