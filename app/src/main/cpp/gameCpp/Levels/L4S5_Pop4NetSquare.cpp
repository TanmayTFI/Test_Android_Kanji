//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 05 - POPUP 4: BRIDGE
//====================================

#include "L4S5_Pop4NetSquare.h"

const CRectangle  NetRect(403.0f,92.0f,583.0f,398.0f);

L4S5_Pop4NetSquare::L4S5_Pop4NetSquare(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV4\\S5\\POP4\\P4.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("KnifeWithBamboo")->SetEnable(false);
	GetObjectByID("Net2")->SetAlpha(0);
	GetObjectByID("Net3")->SetAlpha(0);


	Janim_cutNet = new JSONAnimator(0,CC_CALLBACK_1(L4S5_Pop4NetSquare::OnAnimComplete, this));
	Janim_cutNet->parseAndLoadJSON("ANIM\\L4\\S5_NET_CUT.JSON", this);

	SetSparkleUpdate(false);
	Show(false);
}

void L4S5_Pop4NetSquare::Required()
{

}

void L4S5_Pop4NetSquare::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}


void L4S5_Pop4NetSquare::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && NetRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4net");
		}
	}
}

void L4S5_Pop4NetSquare::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L4_CB_BambooWithKnife:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken)
		{
			Janim_cutNet->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p2_netCut");

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			if (ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved && ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved && ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked && ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_CB_BambooWithKnife); 
			}
		}
		break;
	default:

		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L4S5_Pop4NetSquare::OnAnimComplete(int animID)
{
	GetObjectByID("Net3")->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken = true;
	Hud->TakeInventoryItem(eInv_L4_S5_Net);
	ClosePopup(1);
}

float L4S5_Pop4NetSquare::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S5_Pop4NetSquare::~L4S5_Pop4NetSquare()
{
	SAFE_DELETE(Janim_cutNet);
}

bool L4S5_Pop4NetSquare::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] )
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S5_Pop4NetSquare::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] )
	{
		Hud->ShowHintForInvItem(eInv_L4_CB_BambooWithKnife, &NetRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
