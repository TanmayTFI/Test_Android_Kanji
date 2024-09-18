//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 04 - POPUP 2: Fire
//====================================

#include "L2S4_Pop2FirePop.h"
#include "Hud.h"

const CRectangle  InvDestRectTorch(545.0f,197.0f,275.0f,277.0f);

L2S4_Pop2FirePop::L2S4_Pop2FirePop(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV2\\S4\\POP2\\P2.SCN");
	SetFrameAndCloseButton(true);

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);

	GetObjectByID("p2fireanim")->SetScale(3.0f);

	GetObjectByID("p2torchonfire")->SetEnable(false);
	GetObjectByID("p2oiltorch")->SetEnable(false);

	GetObjectByID("p2torchonfire")->SetZ(zOrder+eZOrder_PopFrame+10);
	GetObjectByID("p2oiltorch")->SetZ(zOrder+eZOrder_PopFrame+10);

	JAnim_FireTorch = new JSONAnimator(0, CC_CALLBACK_1(L2S4_Pop2FirePop::OnAnimationComplete, this));
	JAnim_FireTorch->parseAndLoadJSON("ANIM\\L2\\S4TORCHFIREANIM.JSON", this);

	isBannerShown = false;
}

void L2S4_Pop2FirePop::OnPopupOpened()
{
	if(!isBannerShown)
	{
		isBannerShown = true;
		Hud->ShowBannerText("L2burn");
	}

	Control::Audio->QuickLoadAndPlaySFX("s4p2fireloop", true);
}


void L2S4_Pop2FirePop::Required()
{
	
}

void L2S4_Pop2FirePop::OnPopupCloseBtnClicked()
{
	Control::Audio->StopSFX("s4p2fireloop");
}

void L2S4_Pop2FirePop::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L2S4_UnlitOilTorch && InvDestRectTorch.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchFlamed)
		{
			isAnimationPlaying = true;

			JAnim_FireTorch->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
		}
		else
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}
}

void L2S4_Pop2FirePop::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
	
	if( !Hud->IsDraggingItem() && !ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchFlamed )
	{
		if (InvDestRectTorch.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2burn");
			}
		}
	}
}

void L2S4_Pop2FirePop::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//Firetorch
		isAnimationPlaying = false;

		GetObjectByID("p2torchonfire")->SetEnable(false);

		ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchFlamed = true;

		Hud->ReleaseDrag();
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_UnlitOilTorch] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_UnlitOilTorch);

		ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchTaken = true;
		Hud->TakeInventoryItem(eInv_L2S4_LitTorch);

		ClosePopup(2.5f);
	}
}

L2S4_Pop2FirePop::~L2S4_Pop2FirePop()
{
	SAFE_DELETE(JAnim_FireTorch);
}

void L2S4_Pop2FirePop::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchStickOiledTaken && !ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S4_UnlitOilTorch, &InvDestRectTorch);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L2S4_Pop2FirePop::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchStickOiledTaken && !ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}
