//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S5_Pop3TopExitDoor.h"
#include "Hud.h"

const CRectangle  InvDestRectTopDoor(386.0f,112.0f,484.0f,405.0f);

L5S5_Pop3TopExitDoor::L5S5_Pop3TopExitDoor(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S5\\POP3\\P3.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	GetObjectByID("doorglow")->SetEnable(false);

	if( !ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted )
	{
		GetObjectByID("greenlightR1")->SetEnable(false);
		GetObjectByID("greenlightR2")->SetEnable(false);
		GetObjectByID("greenlightR3")->SetEnable(false);

		GetObjectByID("greenlightL1")->SetEnable(false);
		GetObjectByID("greenlightL2")->SetEnable(false);
		GetObjectByID("greenlightL3")->SetEnable(false);
	}

	GetObjectByID("badgeglow")->SetEnable(false);
	GetObjectByID("badge")->SetEnable(false);

	GetObjectByID("greenlightL2")->SetMaskRect(InvDestRectTopDoor);
	GetObjectByID("leftdoor")->SetMaskRect(InvDestRectTopDoor);

	std::vector<SuperFX*> superfxArray;
	dustVFX = new SuperFX("PRT\\L5\\s5dooropendust.INI", PointSystem::CreateCPoint(100,100,0), zOrder+10);
	superfxArray.push_back(dustVFX);

	_JAnim_DoorOpen = new JSONAnimator(0, CC_CALLBACK_1(L5S5_Pop3TopExitDoor::OnAnimationComplete, this));
	_JAnim_DoorOpen->parseAndLoadJSON("ANIM\\L5\\S5TOPDOOROPEN.JSON", this, &superfxArray);	
}

void L5S5_Pop3TopExitDoor::Required()
{

}

void L5S5_Pop3TopExitDoor::OnAnimationComplete(int animID)
{
	isAnimationPlaying = false;

	Hud->SendUpdateSceneMessage();

	GetObjectByID("doorglow")->Animate(0.85f,1.0f,4.0f, 1.57f);
}

void L5S5_Pop3TopExitDoor::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
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
	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened)
	{
		if(InvDestRectTopDoor.Intersection(&pos))
		{
			Cursor::SetMode(CA_ExitUp, -1);
			if(Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S6_MountainTop;
				Hud->SendNewAreaMessage();
				return;
			}
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsSoldierBadgeUsedOnDoor)
	{
		if(InvDestRectTopDoor.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("leftdoor")->Shake(180, 1, 0, 0.35f);
				GetObjectByID("rightdoor")->Shake(0, 1, 0, 0.35f);
				Hud->ShowBannerText("L3its");
			}
		}
	}
}

void L5S5_Pop3TopExitDoor::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		GFInstance->WrongInvDropMsg(Hud);
		return;
	}

	if (itemVal == eInv_L5_S2_SoldierBadge && InvDestRectTopDoor.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			Hud->ShowBannerText("L5fit");
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsSoldierBadgeUsedOnDoor)
		{
			isAnimationPlaying = true;
			ProfileSystem::Instance->ActiveProfile.L5_S5_IsSoldierBadgeUsedOnDoor = true;
			ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened = true;

			Control::Audio->QuickLoadAndPlaySFX("s5p3_badgeused");

			_JAnim_DoorOpen->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
		}
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L5S5_Pop3TopExitDoor::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted && ProfileSystem::Instance->ActiveProfile.L5_S2_InvSoldierBadgeTaken && !ProfileSystem::Instance->ActiveProfile.L5_S5_IsSoldierBadgeUsedOnDoor)
	{
		Hud->ShowHintForInvItem(eInv_L5_S2_SoldierBadge, &InvDestRectTopDoor);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened)
	{
		Hud->InitHintTrailEmit(&InvDestRectTopDoor, true, CA_ExitUp);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L5S5_Pop3TopExitDoor::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted && ProfileSystem::Instance->ActiveProfile.L5_S5_IsBambooLadderPlacedOnScene &&
		ProfileSystem::Instance->ActiveProfile.L5_S2_InvSoldierBadgeTaken && !ProfileSystem::Instance->ActiveProfile.L5_S5_IsSoldierBadgeUsedOnDoor)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L5S5_Pop3TopExitDoor::~L5S5_Pop3TopExitDoor()
{
	SAFE_DELETE(dustVFX);
	SAFE_DELETE(_JAnim_DoorOpen);
}
