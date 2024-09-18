//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L3S4_Pop3CliffDoor.h"
#include "Hud.h"

const CRectangle  InvDestRectNagaPart(516.0f,166.0f,356.0f,355.0f);

const CRectangle  ActionRectDoor(451.0f,98.0f,457.0f,473.0f);

const CRectangle  MaskRect(361.0f,86.0f,650.0f,490.0f);

L3S4_Pop3CliffDoor::L3S4_Pop3CliffDoor(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	
	isAnimationPlaying = false; //ALWAYS INIT TO ZERO TO AVOID AMBIGUITY!!

	//Popup load
	Load("LV3\\S4\\POP3\\P3.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	JAnim_NagaLockPlace = new JSONAnimator(0, CC_CALLBACK_1(L3S4_Pop3CliffDoor::OnAnimationComplete, this));
	JAnim_NagaLockPlace->parseAndLoadJSON("ANIM\\L3\\S4DOORLOCKPLACE.JSON", this);

	JAnim_DoorOpening = new JSONAnimator(1, CC_CALLBACK_1(L3S4_Pop3CliffDoor::OnAnimationComplete, this));
	JAnim_DoorOpening->parseAndLoadJSON("ANIM\\L3\\S4DOOROPEN.JSON", this);

	GetObjectByID("LockPlace")->SetEnable(false);
	
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsNagaLockPlaced)
	{
		GetObjectByID("CenterLockR")->PlayAnimation(1);
	}

	GetObjectByID("doorL")->SetMaskRect(MaskRect);
	GetObjectByID("doorR")->SetMaskRect(MaskRect);
}

void L3S4_Pop3CliffDoor::Required()
{

}

void L3S4_Pop3CliffDoor::Update(float ds)
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

	//handle interactions here
	if(ActionRectDoor.Intersection(&pos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened)
		{
			if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsNagaLockPlaced)
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened = true;//to AVOID cliff hanger inbetween forced quit cases!!
					Control::Audio->QuickLoadAndPlaySFX("s4p3_dooropen");
					JAnim_DoorOpening->PlayAnim();
					return;
				}
			}
			else
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					GetObjectByID("doorR")->Shake(0, 1, 0, 0.35f);
					GetObjectByID("doorL")->Shake(180, 1, 0, 0.35f);

					GetObjectByID("CenterLockR")->Shake(0, 1, 0, 0.35f);
					GetObjectByID("CenterLockL")->Shake(180, 1, 0, 0.35f);

					Control::Audio->QuickLoadAndPlaySFX("s4p3doorlocked");

					Hud->ShowBannerText("L3empty");
				}
			}
			
		}
		else
		{
			Cursor::SetMode(CA_ExitUp, -1);
			if(Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S5_Cliff;
				Hud->SendNewAreaMessage();
				return;
			}
		}
	}
}

void L3S4_Pop3CliffDoor::OnAnimationComplete(int animID)
{
	if(animID ==0)
	{
		//Naga Lock Place Anim
		isAnimationPlaying = false;

		GetObjectByID("LockPlace")->SetEnable(false);
		GetObjectByID("CenterLockR")->PlayAnimation(1);

		Hud->SendUpdateSceneMessage();
	}
	else if(animID ==1)
	{
		//Door Opening Anim
		isAnimationPlaying = false;

		

		Hud->SendUpdateSceneMessage();
	}
}


void L3S4_Pop3CliffDoor::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(itemVal == eInv_L3S3_SnakeDoorPart)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsNagaLockPlaced && ActionRectDoor.Intersection(&dropPos))
		{
			isAnimationPlaying = true;

			Control::Audio->QuickLoadAndPlaySFX("s4p3_symbolplaced");

			JAnim_NagaLockPlace->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L3_S4_IsNagaLockPlaced = true;

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_SnakeDoorPart] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S3_SnakeDoorPart);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L3S4_Pop3CliffDoor::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if( ProfileSystem::Instance->ActiveProfile.L3_S3_InvSnakeDoorPartTaken && !ProfileSystem::Instance->ActiveProfile.L3_S4_IsNagaLockPlaced)
	{
		Hud->ShowHintForInvItem(eInv_L3S3_SnakeDoorPart, &InvDestRectNagaPart);
	}
	else if( ProfileSystem::Instance->ActiveProfile.L3_S4_IsNagaLockPlaced && !ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened)
	{
		Hud->InitHintTrailEmit(&ActionRectDoor);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened)
	{
		Hud->InitHintTrailEmit(&ActionRectDoor, false, CA_ExitUp);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3S4_Pop3CliffDoor::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( ProfileSystem::Instance->ActiveProfile.L3_S3_InvSnakeDoorPartTaken && !ProfileSystem::Instance->ActiveProfile.L3_S4_IsNagaLockPlaced)
	{
		isAnyTaskLeft = true;
	}
	else if( ProfileSystem::Instance->ActiveProfile.L3_S4_IsNagaLockPlaced && !ProfileSystem::Instance->ActiveProfile.L3_S4_IsCliffDoorOpened)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L3S4_Pop3CliffDoor::~L3S4_Pop3CliffDoor()
{
	SAFE_DELETE(JAnim_NagaLockPlace);
	SAFE_DELETE(JAnim_DoorOpening);
}
