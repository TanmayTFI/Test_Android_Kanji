//====================================
//	DQFYLH
//	HO System
//	Level 8 - Scene 01 - POPUP 3: 
//====================================

#include "L8S1_Pop3DoorPopup.h"
#include "Util.h"
#include "HiddenObjectEnums.h"

L8S1_Pop3DoorPopup::L8S1_Pop3DoorPopup(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV8\\S1\\P3\\P3.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if (!ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced)
	{
		GetObjectByID("Shield_Inventory")->SetEnable(false);
	}

	/*if (!ProfileSystem::Instance->ActiveProfile.L8_S1_HandPlaced)
	{
	GetObjectByID("Hand01")->SetEnable(false);

	for (int i = 0; i < 3; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	string id = "Fingure" + MKSTR(i + 1) + "_Ring0" + MKSTR(j + 1);
	GetObjectByID(id)->SetEnable(false);
	}
	}
	}*/

	if (ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced)
	{
		GetObjectByID("Door_Left")->SetEnable(false);
		GetObjectByID("Door_Right")->SetEnable(false);
		GetObjectByID("Shield_Inventory")->SetEnable(false);
	}

	Janim_doorOpen = new JSONAnimator(0,CC_CALLBACK_1(L8S1_Pop3DoorPopup::OnAnimComplete,this));
	Janim_doorOpen->parseAndLoadJSON("ANIM\\L8\\L8_S1_P3_DOOROPEN.JSON",this);

	SetSparkleUpdate(false);
	Show(false);

}

void L8S1_Pop3DoorPopup::OnPopupOpened()
{

}

void L8S1_Pop3DoorPopup::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced && GetObjectByID("DoorMask")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L8even");
		}
	}
}

void L8S1_Pop3DoorPopup::ProcessSkip()
{
	Janim_doorOpen->PlayAnim();
	Hud->InvBox_StopPuzzleMode();

}

void L8S1_Pop3DoorPopup::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L8_S1_DoorKey:
		Control::Audio->QuickLoadAndPlaySFX("s1p3_placeKeyOnDoor");
		GetObjectByID("Shield_Inventory")->SetEnable(true);
		ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced = true;
		Janim_doorOpen->PlayAnim();
		Control::Audio->QuickLoadAndPlaySFX("s1p3_doorOpen");
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_DoorKey] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S1_DoorKey);
		//	Hud->InvBox_StartPuzzleMode();
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L8S1_Pop3DoorPopup::OnAnimComplete(int animID)
{
	//ProfileSystem::Instance->ActiveProfile.L8_S1_HandPlaced = true;
	ClosePopup();
}

float L8S1_Pop3DoorPopup::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L8S1_Pop3DoorPopup::~L8S1_Pop3DoorPopup()
{
	SAFE_DELETE(Janim_doorOpen);
}

void L8S1_Pop3DoorPopup::Required()
{

}

void L8S1_Pop3DoorPopup::ClosePopup(float delayTime)
{
	Hud->InvBox_StopPuzzleMode();
	Popup::ClosePopup(delayTime);
}

bool L8S1_Pop3DoorPopup::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced	&& ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L8S1_Pop3DoorPopup::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced && ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyTaken)
	{
		//Hud->ShowHintForInvItem(eInv_L8_S1_DoorKey, &GetObjectByID("Shield_Inventory")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("Shield_Inventory")->GetWorldRect();
		Hud->ShowHintForInvItem(eInv_L8_S1_DoorKey, &ObjRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
