//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 03 - POPUP 1: Saw
//====================================

#include "L3S3_Pop1Saw.h"
#include "Hud.h"

const CRectangle  ActionRectRockRightMove(742.0f,350.0f,97.0f,97.0f);
const CRectangle  ActionRectRockLeftMove(568.0f,276.0f,233.0f,156.0f);

const CRectangle  InvItemRectSaw(628.0f,292.0f,218.0f,225.0f);

L3S3_Pop1Saw::L3S3_Pop1Saw(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S3\\POP1\\P1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	isBannerShown = false;

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsRockMovedSaw1)
	{
		GetObjectByID("rockleft")->SetEnable(false);
	}
	if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsRockMovedSaw2)
	{
		GetObjectByID("rockright")->SetEnable(false);
	}
}

void L3S3_Pop1Saw::Required()
{

}

void L3S3_Pop1Saw::OnPopupOpened()
{
	if(!isBannerShown && !ProfileSystem::Instance->ActiveProfile.L3_S3_IsRockMovedSaw1)
	{
		isBannerShown = true;
		Hud->ShowBannerText("L3creepers");
	}
}

void L3S3_Pop1Saw::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L3S3_Pop1Saw::Update(float ds)
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

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsRockMovedSaw1)
	{
		if(ActionRectRockLeftMove.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.L3_S3_IsRockMovedSaw1 = true;

				Control::Audio->QuickLoadAndPlaySFX("s3p1_stonemove1");

				CObject* leftRock = GetObjectByID("rockleft");
				float valArr[5] = {0, leftRock->Pos.x-80, leftRock->Pos.y-20, 1.0, 15.0f};
				leftRock->TweenTo(valArr, 1.0f, 0, true);
				return;
			}
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsRockMovedSaw2)
	{
		if(ActionRectRockRightMove.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.L3_S3_IsRockMovedSaw2 = true;

				Control::Audio->QuickLoadAndPlaySFX("s3p1_stonemove2");

				CObject* rightRock = GetObjectByID("rockright");
				float valArr[5] = {0, rightRock->Pos.x+70, rightRock->Pos.y-20, 1.0, 15.0f};
				rightRock->TweenTo(valArr, 1.0f, 0, true);
				return;
			}
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken)
	{
		if(InvItemRectSaw.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("saw")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken = true;
				Hud->TakeInventoryItem(eInv_L3S3_Saw);
				ClosePopup(2.5f);
				return;
			}
		} 
	}
}

L3S3_Pop1Saw::~L3S3_Pop1Saw()
{
	
}

void L3S3_Pop1Saw::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsRockMovedSaw1)
	{
		Hud->InitHintTrailEmit(&ActionRectRockLeftMove);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsRockMovedSaw2)
	{
		Hud->InitHintTrailEmit(&ActionRectRockRightMove);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectSaw);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3S3_Pop1Saw::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}
