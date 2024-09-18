#include "L3_S2_P1R3LockOpen.h"

const CRectangle  PopupClickArea(570.0f,211.0f,235.0f,273.0f);
const int SolutionDoor = 0;

L3_S2_P1R3LockOpen::L3_S2_P1R3LockOpen(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) :
	 Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	Load("LV3\\S2\\MAZE3\\POP3\\LOCKOPEN.SCN");
	SetFrameAndCloseButton(true);
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(GetObjectByID("BG"));

	CObject *hammer = GetObjectByID("hammer");
	hammer->SetAlpha(0);
	hammer->SetZ(zOrder + eZOrder_PopCloseBtn + 1);

	_janim_LockBreak = new JSONAnimator(1, CC_CALLBACK_1(L3_S2_P1R3LockOpen::OnAnimationComplete, this));
	_janim_LockBreak->parseAndLoadJSON("ANIM\\L3\\S2_R3LOCKBREAK.JSON",this);
}

L3_S2_P1R3LockOpen::~L3_S2_P1R3LockOpen(void)
{
	SAFE_DELETE(_janim_LockBreak);
}

void L3_S2_P1R3LockOpen::Update(float ds)
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

	if (PopupClickArea.Intersection(&mPos))
	{
		Cursor::SetMode(CA_Gear);

		if (!Hud->IsDraggingItem() && Control::Input->LBclicked())
		{
			Hud->ShowBannerText(Banner_L3_S2_M3_ClickOnLockedDoor);
		}
	}
}

void L3_S2_P1R3LockOpen::OnAnimationComplete(int animId)
{
	isAnimationPlaying = false;
	ProfileSystem::Instance->ActiveProfile.L3_S2_R3DoorOpen[_doorIndex] = true;
	ClosePopup();
}

void L3_S2_P1R3LockOpen::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (PopupClickArea.Intersection(&dropPos) && 
		itemVal ==  eInv_L3S2_Hammer)
	{
		Control::Audio->QuickLoadAndPlaySFX("s2m3p1_lockbreak");

		_janim_LockBreak->PlayAnim();
		isAnimationPlaying = true;

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 
	}
	else
	{
		Popup::HandleInventoryDrop(dropPos, itemVal);
	}
}

bool L3_S2_P1R3LockOpen::isTaskLeft()
{
	if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerTaken && !ProfileSystem::Instance->ActiveProfile.L3_S2_R3DoorOpen[SolutionDoor])
	{
		return true;
	}

	return false;
}

void L3_S2_P1R3LockOpen::ProcessHint()
{
	if (isTaskLeft())
		Hud->ShowHintForInvItem(eInv_L3S2_Hammer, &_popupBoundingBox);
}

