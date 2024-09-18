//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 04 - POPUP 1: Woodcut
//====================================

#include "L2S4_Pop1WoodCut.h"
#include "Hud.h"

const CRectangle  InvStoneRect(438.0f,296.0f,232.0f,190.0f);
const CRectangle  InvVinegarRect(678.0f,256.0f,99.0f,229.0f);

const CRectangle  InvItemRectAxe(588.0f,229.0f,187.0f,254.0f);
const CRectangle  InvDestRectAxe(443.0f,147.0f,455.0f,330.0f);

L2S4_Pop1WoodCut::L2S4_Pop1WoodCut(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV2\\S4\\POP1\\P1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	JAnim_WoodCut1 = new JSONAnimator(0, CC_CALLBACK_1(L2S4_Pop1WoodCut::OnAnimationComplete, this));
	JAnim_WoodCut1->parseAndLoadJSON("ANIM\\L2\\S4WOODCUT1.JSON", this);
	//JAnim_WoodCut1->PlayAnim(true);

	JAnim_WoodCut2 = new JSONAnimator(1, CC_CALLBACK_1(L2S4_Pop1WoodCut::OnAnimationComplete, this));
	JAnim_WoodCut2->parseAndLoadJSON("ANIM\\L2\\S4WOODCUT2.JSON", this);

	if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeBluntTaken)
		GetObjectByID("p1sceneaxe")->SetEnable(false);

	GetObjectByID("p1cuttingaxe")->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut1)
	{
		GetObjectByID("p1woodcut1")->SetEnable(false);
		GetObjectByID("p1woodcut2pc1")->SetEnable(false);
		GetObjectByID("p1woodcut2pc2")->SetEnable(false);
	}
	else
	{
		GetObjectByID("p1wooduncut")->SetEnable(false);
		GetObjectByID("p1woodcut1")->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2)
	{
		GetObjectByID("p1woodpc1")->SetEnable(false);
		GetObjectByID("p1woodpc2")->SetEnable(false);
		GetObjectByID("p1woodpc3")->SetEnable(false);
		GetObjectByID("p1woodpc4")->SetEnable(false);
	}
}

void L2S4_Pop1WoodCut::Required()
{

}

void L2S4_Pop1WoodCut::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L2S4_AxeSharpen && InvDestRectAxe.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut1)
		{
			ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut1 = true;

			JAnim_WoodCut1->PlayAnim();

			Hud->InvBoxCtrl->ShowDragInv(false);

			isAnimationPlaying = true;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2)
		{
			ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2 = true;

			JAnim_WoodCut2->PlayAnim();

			isAnimationPlaying = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			if(ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_AxeSharpen] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_AxeSharpen);
			}
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

void L2S4_Pop1WoodCut::Update(float ds)
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

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeBluntTaken)
	{
		if (InvItemRectAxe.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked())
			{
				GetObjectByID("p1sceneaxe")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeBluntTaken = true;
				Hud->TakeInventoryItem(eInv_L2S4_AxeBlunt, Control::Input->Mpos());
			}
			return;
		}
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut1 || !ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2)
	{
		if (!Hud->IsDraggingItem() && InvDestRectAxe.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeBluntTaken)
					Hud->ShowBannerText("L2log");
				else
					Hud->ShowBannerText("L2fire");
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2 && !ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken && InvItemRectAxe.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("p1woodpc1")->SetEnable(false);
			GetObjectByID("p1woodpc2")->SetEnable(false);
			GetObjectByID("p1woodpc3")->SetEnable(false);
			GetObjectByID("p1woodpc4")->SetEnable(false);
			
			ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken = true;
			Hud->TakeInventoryItem(eInv_L2S4_Firewoods, Control::Input->Mpos());

			ClosePopup(1.5f);
		}
	}
}

void L2S4_Pop1WoodCut::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//woodcut1
		isAnimationPlaying = false;
		Hud->InvBoxCtrl->ShowDragInv(true);

		Hud->SendUpdateSceneMessage();
	}
	else if(animID == 1)
	{
		//woodcut2
		isAnimationPlaying = false;

		Hud->SendUpdateSceneMessage();
	}
}

bool L2S4_Pop1WoodCut::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeBluntTaken)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S4_IsAxeSharpened && !ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2 && !ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S4_Pop1WoodCut::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeBluntTaken)
	{
		Hud->InitHintTrailEmit(InvItemRectAxe.GetCenterX(), InvItemRectAxe.GetCenterY());
	}
	else if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeSharpenTaken && !ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2)
	{
		Hud->ShowHintForInvItem(eInv_L2S4_AxeSharpen, &InvDestRectAxe);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2 && !ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken)
	{
		Hud->InitHintTrailEmit(InvItemRectAxe.GetCenterX(), InvItemRectAxe.GetCenterY());
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

L2S4_Pop1WoodCut::~L2S4_Pop1WoodCut()
{
	SAFE_DELETE(JAnim_WoodCut1);
	SAFE_DELETE(JAnim_WoodCut2);
}
