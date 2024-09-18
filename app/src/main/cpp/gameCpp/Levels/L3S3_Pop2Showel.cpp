//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 03 - POPUP 2: Showel
//====================================

#include "L3S3_Pop2Showel.h"
#include "Hud.h"

const CRectangle  InvItemRectShowel(591.0f,110.0f,320.0f,393.0f);

const CRectangle  DestRectSawRootCut(388.0f,104.0f,584.0f,440.0f);

L3S3_Pop2Showel::L3S3_Pop2Showel(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S3\\POP2\\P2.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	JAnim_SawAnim1 = NULL;
	JAnim_SawAnim2 = NULL;
	
	JAnim_SawAnim1 = new JSONAnimator(0, CC_CALLBACK_1(L3S3_Pop2Showel::OnAnimationComplete, this));
	JAnim_SawAnim1->parseAndLoadJSON("ANIM\\L3\\S3ROOTCUT1.JSON", this);

	JAnim_SawAnim2 = new JSONAnimator(1, CC_CALLBACK_1(L3S3_Pop2Showel::OnAnimationComplete, this));
	JAnim_SawAnim2->parseAndLoadJSON("ANIM\\L3\\S3ROOTCUT2.JSON", this);

	GetObjectByID("sawleft")->SetEnable(false);
	GetObjectByID("sawright")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootRightSawed)
		GetObjectByID("treerootright")->SetEnable(false);
	
	if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootLeftSawed)
		GetObjectByID("treerootleft")->SetEnable(false);
}

void L3S3_Pop2Showel::Required()
{

}

void L3S3_Pop2Showel::HandleInventoryDrop(CPoint dropPos, int itemVal)
{	
	if(isAnimationPlaying)
		return;

	if (itemVal == eInv_L3S3_Saw && DestRectSawRootCut.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootRightSawed)
		{
			isAnimationPlaying = true;

			Control::Audio->QuickLoadAndPlaySFX("s3p2_rootcut1");

			JAnim_SawAnim1->PlayAnim();

			Hud->InvBoxCtrl->ShowDragInv(false);
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootLeftSawed)
		{
			isAnimationPlaying = true;

			Control::Audio->QuickLoadAndPlaySFX("s3p2_rootcut2");

			JAnim_SawAnim2->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			if(ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagFullTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_Saw] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S3_Saw);
			}
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}
}

void L3S3_Pop2Showel::Update(float ds)
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

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootRightSawed && ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootLeftSawed)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken)
		{
			if(InvItemRectShowel.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken = true;
					GetObjectByID("showel")->SetEnable(false);
					Hud->TakeInventoryItem(eInv_L3S3_Showel);
					ClosePopup(2.5f);
				}
			}
		}
	}
	else
	{
		if(DestRectSawRootCut.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s3shovelshake");
				GetObjectByID("showel")->Shake(100, 4, 2, 0.45f);
				Hud->ShowBannerText("L3roots");
			}
		}
	}
}

void L3S3_Pop2Showel::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		isAnimationPlaying = false;

		//root anim 1
		ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootRightSawed = true;
		GetObjectByID("sawleft")->SetEnable(false);
		GetObjectByID("treerootright")->SetEnable(false);

		Hud->InvBoxCtrl->ShowDragInv(true);
	}
	else if(animID == 1)
	{
		isAnimationPlaying = false;

		//root anim 2
		ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootLeftSawed = true;
		GetObjectByID("sawright")->SetEnable(false);
		GetObjectByID("treerootleft")->SetEnable(false);
	}
}

L3S3_Pop2Showel::~L3S3_Pop2Showel()
{
	SAFE_DELETE(JAnim_SawAnim1);
	SAFE_DELETE(JAnim_SawAnim2);
}

void L3S3_Pop2Showel::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootRightSawed || !ProfileSystem::Instance->ActiveProfile.L3_S3_IsRootLeftSawed)
		{
			Hud->ShowHintForInvItem(eInv_L3S3_Saw, &DestRectSawRootCut);
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken)
		{
			Hud->InitHintTrailEmit(&InvItemRectShowel);
		}
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3S3_Pop2Showel::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken)
		{
			isAnyTaskLeft = true;
		}
	}

	return isAnyTaskLeft;
}
