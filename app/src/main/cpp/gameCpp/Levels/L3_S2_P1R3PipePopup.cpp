

#include "L3_S2_P1R3PipePopup.h"
#include "Util.h"

const CRectangle  PopupClickArea(380.0f,120.0f,611.0f,430.0f);
const CRectangle  HammerRect(755.0f,113.0f,139.0f,93.0f);

L3_S2_P1R3PipePopup::L3_S2_P1R3PipePopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) :
	 Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	Load("LV3\\S2\\MAZE3\\POP2\\PIPEPICKPOPUP.SCN");
	SetFrameAndCloseButton();
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(PopupClickArea);

	CObject *shovel = GetObjectByID("shovel");
	shovel->SetAlpha(0);

	_janim_StonesRemove = NULL;

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved)
	{
		for (int i = 1; i <= 6; i++)
		{
			GetObjectByID("stone_0"+MKSTR(i))->SetEnable(false);
		}
	}
	else
	{
		_janim_StonesRemove = new JSONAnimator(1, CC_CALLBACK_1(L3_S2_P1R3PipePopup::OnAnimationComplete, this));
		_janim_StonesRemove->parseAndLoadJSON("ANIM\\L3\\S2_P1R3STONEREMOVE1.JSON",this);
	}

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken )
	{
		GetObjectByID("Pipe")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken )
	{
		GetObjectByID("hammerhead")->SetEnable(false);
	}

}

L3_S2_P1R3PipePopup::~L3_S2_P1R3PipePopup(void)
{
	SAFE_DELETE(_janim_StonesRemove);
}

void L3_S2_P1R3PipePopup::Update(float ds)
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

	if (HammerRect.Intersection(&mPos) && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken )
	{
		Cursor::SetMode(CA_HandTake);

		if (Control::Input->LBclicked())
		{
			Hud->TakeInventoryItem(eInv_L3S2_HammerHead);
			Hud->ShowBannerText(Banner_L3_S2_M3_AfterHammerHeadCollect);
			GetObjectByID("hammerhead")->FadeOut(1.5f);
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken = true;
		}
	}
	else if (_popupBoundingBox.Intersection(&mPos))
	{
		if (!ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved)
		{
			Cursor::SetMode(CA_Gear);

			if (!Hud->IsDraggingItem() && Control::Input->LBclicked())
			{
				Hud->ShowBannerText(Banner_L3_S2_M3_ClickOnSand);
			}
		}
		else 
		{
			if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken )
			{
				Cursor::SetMode(CA_HandTake);

				if (Control::Input->LBclicked())
				{
					Hud->TakeInventoryItem(eInv_L3S2_PipePiece3Room3, GetObjectByID("Pipe")->Pos);
					GetObjectByID("Pipe")->FadeOut(1.5f);
					ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken = true;
				}
			}
		}
	}
}

void L3_S2_P1R3PipePopup::OnAnimationComplete(int animId)
{
	isAnimationPlaying = false;
	ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved = true;
}

void L3_S2_P1R3PipePopup::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (_janim_StonesRemove && 
		PopupClickArea.Intersection(&dropPos) && 
		itemVal ==  eInv_L3S3_Showel)
	{
		Control::Audio->QuickLoadAndPlaySFX("s2m3p1_shoveluse");

		isAnimationPlaying = true;
		_janim_StonesRemove->PlayAnim();

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		if (ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState >= 3)// charcol removed
		{
			ProfileSystem::Instance->ActiveProfile.Inventory[itemVal] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(itemVal);
		}
	}
	else
	{
		Popup::HandleInventoryDrop(dropPos, itemVal);
	}
}

bool L3_S2_P1R3PipePopup::isTaskLeft()
{
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken ||
		( ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken &&
		!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken))
	{
		return true;
	}

	return false;
}

void L3_S2_P1R3PipePopup::ProcessHint()
{
	if (isTaskLeft())
	{
		if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken)
		{
			Hud->InitHintTrailEmit(&HammerRect);
			return;
		}

		if (ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved)
		{
			Hud->InitHintTrailEmit(&PopupClickArea);
			return;
		}

		Hud->ShowHintForInvItem(eInv_L3S3_Showel, &PopupClickArea);
	}
	else
	{
		Popup::ProcessHint();
	}
}

void L3_S2_P1R3PipePopup::OnPopupCloseBtnClicked()
{
	Popup::OnPopupCloseBtnClicked();
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken &&
		ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken)
		Hud->ShowBannerText(Banner_L3_S2_M3_ClosingPopupWithoutTakeHammerHead);
}

