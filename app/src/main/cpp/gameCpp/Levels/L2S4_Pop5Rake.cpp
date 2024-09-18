//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 04 - POPUP 1: Woodcut
//====================================

#include "L2S4_Pop5Rake.h"
#include "Hud.h"

const CRectangle  InvItemRectRake(590.0f,207.0f,167.0f,222.0f);

L2S4_Pop5Rake::L2S4_Pop5Rake(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV2\\S4\\POP5\\P5.SCN");
	SetFrameAndCloseButton(true);

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
}

void L2S4_Pop5Rake::Required()
{

}

void L2S4_Pop5Rake::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	
}

void L2S4_Pop5Rake::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	//UICloseButton->PlayAnimation(0);
	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken && InvItemRectRake.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("p5rake")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken = true;
			Hud->TakeInventoryItem(eInv_L2S4_Rake, Control::Input->Mpos());
			UpdateBgGraphic();

			ClosePopup(1.5f);
		}
	}
}

void L2S4_Pop5Rake::OnAnimationComplete(int animID)
{
}

L2S4_Pop5Rake::~L2S4_Pop5Rake()
{
}

void L2S4_Pop5Rake::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectRake);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L2S4_Pop5Rake::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
	{
		isAnyTaskLeft = true;
	}
	return isAnyTaskLeft;
}