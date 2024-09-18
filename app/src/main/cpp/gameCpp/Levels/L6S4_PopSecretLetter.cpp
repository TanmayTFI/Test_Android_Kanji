//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 04 - POPUP: 
//====================================

#include "L6S4_PopSecretLetter.h"


L6S4_PopSecretLetter::L6S4_PopSecretLetter(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S4\\S1\\P1\\P1.SCN");
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	SetSparkleUpdate(false);
	Show(false);
}


void L6S4_PopSecretLetter::Update(float ds)
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

void L6S4_PopSecretLetter::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/* switch (itemVal)
	{
	
	default:
	*/
		GFInstance->WrongInvDropMsg(Hud);
		/*break;
	}*/
}

void L6S4_PopSecretLetter::OnAnimComplete(int animID)
{

}

float L6S4_PopSecretLetter::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S4_PopSecretLetter::~L6S4_PopSecretLetter()
{
}

void L6S4_PopSecretLetter::Required()
{

}

void L6S4_PopSecretLetter::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L6S4_PopSecretLetter::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	/*if (true)
	{
		isAnyTaskLeft = true;
	}*/

	return isAnyTaskLeft;
}

void L6S4_PopSecretLetter::ProcessHint()
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