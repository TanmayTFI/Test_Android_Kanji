//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 03 - POPUP 5: Door
//====================================

#include "L3S3_Pop5Door.h"
#include "Hud.h"

const CRectangle  ActionRectDoor(489.0f,120.0f,395.0f,452.0f);

L3S3_Pop5Door::L3S3_Pop5Door(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S3\\POP5\\P5.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	
	JAnim_DoorOpening = NULL;

	JAnim_DoorOpening = new JSONAnimator(0, CC_CALLBACK_1(L3S3_Pop5Door::OnAnimationComplete, this));
	JAnim_DoorOpening->parseAndLoadJSON("ANIM\\L3\\S3DOOROPEN.JSON", this);

	GetObjectByID("leftfireglow")->SetBlendMode(eBlend_AlphaAdditive);
	GetObjectByID("leftfireglow")->Animate(0.35f, 0.75f, 3.0f);

	GetObjectByID("rightfireglow")->SetBlendMode(eBlend_AlphaAdditive);
	GetObjectByID("rightfireglow")->Animate(0.25f, 0.85f, 3.0f, 1.25f);
}

void L3S3_Pop5Door::Required()
{

}

void L3S3_Pop5Door::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L3S3_Pop5Door::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
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

	if(ActionRectDoor.Intersection(&pos))
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened)
		{
			Cursor::SetMode(CA_ExitUp, eArea_L3_S4_FortFountain);
			if(Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S4_FortFountain;
				Hud->SendNewAreaMessage();
				return;
			}
		}
		else
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				if(ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p5_dooropen");

					JAnim_DoorOpening->PlayAnim();
					isAnimationPlaying = true;
				}
				else
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p5doorlocked");
					GetObjectByID("doorleft")->Shake(180, 1, 0, 0.35f);
					GetObjectByID("doorright")->Shake(0, 1, 0, 0.35f);
					Hud->ShowBannerText("L3locked");
				}
			}
		}
	}
}

void L3S3_Pop5Door::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//Door Opened
		isAnimationPlaying = false;

		GetObjectByID("doorhalfopen")->SetEnable(false);
		GetObjectByID("doorleft")->SetEnable(false);
		GetObjectByID("doorright")->SetEnable(false);

		Hud->SendUpdateSceneMessage();

		ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened = true;
	}
}

L3S3_Pop5Door::~L3S3_Pop5Door()
{
	SAFE_DELETE(JAnim_DoorOpening);
}

void L3S3_Pop5Door::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened && ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened)
	{
		Hud->InitHintTrailEmit(&ActionRectDoor);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened)
	{
		Hud->InitHintTrailEmit(&ActionRectDoor);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3S3_Pop5Door::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened && ProfileSystem::Instance->ActiveProfile.L3_S2_PoolDoorOpened)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}
