#include "L3_S2_P1R3HammerPopup.h"

const CRectangle  PopupClickArea(380.0f,120.0f,611.0f,430.0f);


L3_S2_P1R3HammerPopup::L3_S2_P1R3HammerPopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) :
	 Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	Load("LV3\\S2\\MAZE3\\POP1\\HAMMERPOPUP.SCN");
	SetFrameAndCloseButton();
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(PopupClickArea);
}


L3_S2_P1R3HammerPopup::~L3_S2_P1R3HammerPopup(void)
{
}

void L3_S2_P1R3HammerPopup::Update(float ds)
{
	Popup::Update(ds);

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;
	
	if(GetPopupState()!= ePopupState_Open )
		return;

	CPoint mPos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (PopupClickArea.Intersection(&mPos) && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken )
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("Hammer_Head")->FadeOut(1.5f);
			Hud->TakeInventoryItem(eInv_L3S2_HammerHead);
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken = true;
		}
	}
}

bool L3_S2_P1R3HammerPopup::isTaskLeft()
{
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken)
	{
		return true;
	}

	return false;
}

void L3_S2_P1R3HammerPopup::ProcessHint()
{
	if (isTaskLeft())
		Hud->InitHintTrailEmit( &PopupClickArea);
}



