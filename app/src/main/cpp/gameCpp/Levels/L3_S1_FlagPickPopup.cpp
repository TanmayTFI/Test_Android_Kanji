#include "L3_S1_FlagPickPopup.h"
#include "L3_S1_FortEntrance.h"

const CRectangle  PopRect(399.0f,109.0f,476.0f,461.0f);

L3_S1_FlagPickPopup::L3_S1_FlagPickPopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback)
										:Popup(popupId, hud, scene, zOrder, zoomOrigin, popupCloseCallback)
{
	_interface = NULL;
	Load("LV3\\S1\\POP\\POP3\\S1FLAGPICKPOPUP.SCN");
	SetFrameAndCloseButton();
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(GetObjectByID("PopupBg"));

	_janim_PickFlag = new JSONAnimator(1, CC_CALLBACK_1(L3_S1_FlagPickPopup::OnAnimationComplete, this));
	_janim_PickFlag->parseAndLoadJSON("ANIM\\L3\\S1POP3_FLAGCUTANIM.JSON",this);

	CObject* Saw = GetObjectByID("Saw");
	Saw->SetAlpha(0);

	flagObj = GetObjectByID("FlagPopup");

	_flagMesh = new MeshObject(GetObjectByID("Flag_mesh"), 5, 5);
	_flagMesh->meshFX->SetSpeedAndDisplacement(14, 12, 14, 2);
	_flagMesh->meshFX->SetMeshBox(1, 1, 5, 5);
	_flagMesh->Show(false);
}


L3_S1_FlagPickPopup::~L3_S1_FlagPickPopup(void)
{
	_interface = NULL;
	SAFE_DELETE(_flagMesh);
	SAFE_DELETE(_janim_PickFlag);
}

void L3_S1_FlagPickPopup::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (itemVal == eInv_L3S3_Saw && _popupBoundingBox.Intersection(&dropPos))
	{
		Control::Audio->QuickLoadAndPlaySFX("s1p2_flagpolecut");

		_flagMesh->Show(false);
		flagObj->Show(true);
		_janim_PickFlag->PlayAnim(false);
		isAnimationPlaying = true;
		_interface->UpdateHOSystem(this, S1_Objctive_PickFlag);

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 
		//Clear Inventory. One Time Use Items.
		if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootLeftSawed)
		{
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_Saw] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S3_Saw);
		}
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}
}

void L3_S1_FlagPickPopup::OnAnimationComplete(int animId)
{
	isAnimationPlaying = false;
	GetObjectByID("SpearTop")->FadeOut(1.5f);
	flagObj->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagFullTaken = true;
	Hud->TakeInventoryItem(eInv_L3S1_FlagFull);
}

void L3_S1_FlagPickPopup::ProcessHint()
{
	if (ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken)
	{
		Hud->ShowHintForInvItem(eInv_L3S3_Saw, &_popupBoundingBox);
		return;
	}

	Popup::ProcessHint();
}

bool L3_S1_FlagPickPopup::isTaskLeft()
{
	if (ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken &&
		!ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagFullTaken)
	{
		return true;
	}

	return false;
}

void L3_S1_FlagPickPopup::Update(float ds)
{
	Popup::Update(ds);

	if(GetPopupState()!= ePopupState_Open)
		return;

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())	
		return;

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint mPos = Control::Input->Mpos();

	if (PopRect.Intersection(&mPos))
	{
		Cursor::SetMode(CA_Gear);

		if (Control::Input->LBclicked() && !isAnimationPlaying)
		{
			Hud->ShowBannerText(Banner_L3_S1_ClickOnSpearHook);
		}
	}
}

void L3_S1_FlagPickPopup::OnScaleUpComplete()
{
	Popup::OnScaleUpComplete();
	flagObj->Show(false);
	_flagMesh->Show(true);
}

void L3_S1_FlagPickPopup::OnPopupCloseBtnClicked()
{
	Popup::OnScaleUpComplete();
	
	if(!ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagFullTaken)
		flagObj->Show(true);
	
	if(_flagMesh)
		_flagMesh->Show(false);
}




