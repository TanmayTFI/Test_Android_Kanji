//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L4S5_Pop5Bridge.h"
#include "Hud.h"

const CRectangle  BridgeRect(345.0f,90.0f,665.0f,501.0f);

L4S5_Pop5Bridge::L4S5_Pop5Bridge(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV4\\S5\\POP5\\POP5.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	SetSparkleUpdate(false);
	Show(false);

	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced )
	{
		GetObjectByID("Door")->SetEnable(false);
	}

}

void L4S5_Pop5Bridge::Required()
{

}

void L4S5_Pop5Bridge::Update(float ds)
{

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}



	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
	if(Hud->IsDraggingItem())
		return;
	//handle interactions here

	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced && BridgeRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if(	Control::Input->LBclicked())
			Hud->ShowBannerText("L4doubt");
	}
}

void L4S5_Pop5Bridge::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

void L4S5_Pop5Bridge::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L4_S5_Door:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced)
		{
			ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced = true;
			ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L4_S1_SwampEntrance] = false;
			ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L4_S2_RiverSide] = false;
			ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L4_S4_HillTop] = false;
			ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L4_S3_Cave] = false;
			GetObjectByID("Door")->SetEnable(true);
			Control::Audio->QuickLoadAndPlaySFX(aSFXHintDischarge);
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_Door] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S5_Door); 
			ClosePopup(1.5f);
		}
		break;
	default:

		GFInstance->WrongInvDropMsg(Hud);
		break;
	}

}

void L4S5_Pop5Bridge::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_Door] && !ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced )
	{
		//	Hud->ShowHintForInvItem(eInv_L4_S5_Door, &GetObjectByID("Door")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle doorObjRect = GetObjectByID("Door")->GetWorldRect();
		Hud->ShowHintForInvItem(eInv_L4_S5_Door, &doorObjRect);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L4S5_Pop5Bridge::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	return isAnyTaskLeft;
}

L4S5_Pop5Bridge::~L4S5_Pop5Bridge()
{

}


