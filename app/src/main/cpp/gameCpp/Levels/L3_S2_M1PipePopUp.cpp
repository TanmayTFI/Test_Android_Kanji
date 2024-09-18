#include "L3_S2_M1PipePopUp.h"

const CRectangle  PopupBounding(355.0f,89.0f,652.0f,495.0f);

L3_S2_M1PipePopUp::L3_S2_M1PipePopUp(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback)
	: Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	Load("LV3\\S2\\MAZE1\\POP1\\POP1.SCN");
	SetFrameAndCloseButton();
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(PopupBounding);

	GetObjectByID("Spear")->SetAlpha(0);

	_pipeObj = GetObjectByID("pipe");
	_waterMask = GetObjectByID("waterMask");

	MeshObject* popupWaterMesh = new MeshObject(GetObjectByID("waterMesh"), 5, 5);
	popupWaterMesh->meshFX->SetSpeedAndDisplacement(5, 3, 5, 3);
	InsertObject(popupWaterMesh, 1);
	popupWaterMesh->Show(false);

	MeshObject* waterMeshMask = new MeshObject(GetObjectByID("waterMeshMask"), 5, 5);
	waterMeshMask->meshFX->SetSpeedAndDisplacement(5, 3, 5, 3);
	PushObject(waterMeshMask);
	waterMeshMask->Show(false);

	_janim_PipePick = new JSONAnimator(1, CC_CALLBACK_1(L3_S2_M1PipePopUp::OnAnimationComplete, this));
	_janim_PipePick->parseAndLoadJSON("ANIM\\L3\\S2MAZE1_PIPEPICKANIM.JSON",this);
}

L3_S2_M1PipePopUp::~L3_S2_M1PipePopUp(void)
{
	SAFE_DELETE(_janim_PipePick);
}

void L3_S2_M1PipePopUp::Update(float ds)
{
	Popup::Update(ds);

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(GetPopupState()!= ePopupState_Open )
		return;

	CPoint mPos = Control::Input->Mpos();
	if(isMouseOverCloseButton)
		return;

	if (_popupBoundingBox.Intersection(&mPos))
	{
		Cursor::SetMode(CA_Gear);

		if (!Hud->IsDraggingItem() && Control::Input->LBclicked())
		{
			Hud->ShowBannerText(Banner_L3_S2_M1_ClickOnWell);
		}
	}
	else
		Cursor::SetMode(CA_Normal);
}

void L3_S2_M1PipePopUp::OnScaleUpComplete()
{
	Popup::OnScaleUpComplete();
	_waterMask->Show(false);
}

void L3_S2_M1PipePopUp::OnPopupCloseBtnClicked()
{
	_waterMask->Show(true);
	Popup::OnPopupCloseBtnClicked();
}

void L3_S2_M1PipePopUp::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (itemVal == eInv_L3S1_Spear)
	{
		Control::Audio->QuickLoadAndPlaySFX("s2m1p1_pipetake");

		_janim_PipePick->PlayAnim(false);
		isAnimationPlaying = true;

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 
		//Clear Inventory. One Time Use Items.
		ProfileSystem::Instance->ActiveProfile.Inventory[itemVal] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(itemVal);
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}
}

void L3_S2_M1PipePopUp::OnAnimationComplete(int animID)
{
	isAnimationPlaying = false;
	_pipeObj->FadeOut(1.5f);
	Hud->TakeInventoryItem(eInv_L3S2_PipePiece1Room1, _pipeObj->Pos);
	ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken = true;
}

void L3_S2_M1PipePopUp::ProcessHint()
{
	Hud->ShowHintForInvItem(eInv_L3S1_Spear, &_popupBoundingBox);
}
