//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7S3_Pop3Chain.h"
#include "Hud.h"

const CRectangle  chainMaskTopRect(492.0f,108.0f,408.0f,61.0f);
const CRectangle  chainMaskLeftRect(474.0f,135.0f,43.0f,445.0f);

const CRectangle  GearMaskRect(801.0f,88.0f,176.0f,134.0f);

const CRectangle  InvDestRectLeverchain(431.0f,88.0f,536.0f,491.0f);
const CRectangle  InvItemRectMetalWire(429.0f,86.0f,108.0f,150.0f);
const CRectangle  InvDestMinusRectChain(528.0f,171.0f,339.0f,413.0f);
const CRectangle  InvDestRectBullKey(570.0f,221.0f,255.0f,231.0f);

const CRectangle  BtnRectUpButton(1009.0f,269.0f,90.0f,110.0f);//up is right
const CRectangle  BtnRectDownButton(272.0f,273.0f,89.0f,117.0f);//down is left

const CRectangle  GearInfoRect(810.0f,65.0f,150.0f,142.0f);

L7S3_Pop3Chain::L7S3_Pop3Chain(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV7\\S3\\POP3\\P3.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	clickDelayTimer = -1;

	GetObjectByID("chaintop")->SetMaskRect(chainMaskTopRect);
	GetObjectByID("chainleft")->SetMaskRect(chainMaskLeftRect);

	GetObjectByID("gear")->SetZ(zOrder+110);
	GetObjectByID("gearnut")->SetZ(zOrder+111);
	GetObjectByID("lever")->SetZ(zOrder+112);
	//GetObjectByID("gear")->SetMaskRect(GearMaskRect);

	GetObjectByID("lever")->SetEnable(false);

	GetObjectByID("btntoup")->SetRotate(-90);
	GetObjectByID("btntodown")->SetRotate(-90);

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken)
	{
		GetObjectByID("gearnut")->SetEnable(false);
		GetObjectByID("chainright")->SetEnable(false);
		GetObjectByID("chaintop")->SetEnable(false);
		GetObjectByID("chainleft")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken)
		GetObjectByID("metalwire")->SetEnable(false);

	GetObjectByID("keypose1")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken)
	{
		GetObjectByID("keymask")->SetEnable(false);
		GetObjectByID("bullhead")->SetEnable(false);
		GetObjectByID("axes")->SetEnable(false);
	}

	JAnim_BullkeyFix = new JSONAnimator(0, CC_CALLBACK_1(L7S3_Pop3Chain::OnAnimationComplete, this));
	JAnim_BullkeyFix->parseAndLoadJSON("ANIM\\L7\\S3BULLKEYFIX.JSON", this);

	JAnim_LeverChainUnlock = new JSONAnimator(1, CC_CALLBACK_1(L7S3_Pop3Chain::OnAnimationComplete, this));
	JAnim_LeverChainUnlock->parseAndLoadJSON("ANIM\\L7\\S3LEVERCHAINUNLOCK.JSON", this);

	//UI sliding handling
	LoadNextPopup = nullptr;
	
	UIBtnDown = GetObjectByID("btntodown");
	UIBtnUp = GetObjectByID("btntoup");

	UIBtnDown->SetEnable(false);
	UIBtnUp->SetEnable(false);

	maxVelocity = 5;

	maxDisplacement = 400.0f;
	slideDisplacement = 0;
	slideTimer = 0;

	isSliding = false;
	isFadingIn = false;
}

void L7S3_Pop3Chain::OnAnimationComplete(int animID)
{
	//0 - Bullkeyfix
	//1 - Lever Chain Unlock
	isAnimationPlaying = false;

	if(animID == 0)
	{
		GetObjectByID("keypose1")->SetEnable(false);
		GetObjectByID("keymask")->SetEnable(false);
		GetObjectByID("bullhead")->SetEnable(false);
		GetObjectByID("axes")->SetEnable(false);

		Hud->TakeInventoryItem(eInv_L7_S3_Axes);
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano] = true;
	}
	else if(animID == 1)
	{
		GetObjectByID("gearnut")->SetEnable(false);
		GetObjectByID("lever")->SetEnable(false);
		GetObjectByID("chainright")->SetEnable(false);
		GetObjectByID("chaintop")->SetEnable(false);
		GetObjectByID("chainleft")->SetEnable(false);

		ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken = true;
		Hud->TakeInventoryItem(eInv_L7_S3_MetalChain);		

		if( ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken && 
			ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken )
			ClosePopup(1.5f);
	}
}

void L7S3_Pop3Chain::OpenPopup()
{
	SetSceneAlpha(1.0f);
	Popup::OpenPopup();
}

void L7S3_Pop3Chain::OnPopupOpened()
{
	SetupAndShowUI();
}

void L7S3_Pop3Chain::OnPopupCloseBtnClicked()
{
	HideUI();
}

//UI sliding handling
void L7S3_Pop3Chain::SetupAndShowUI()
{
	UIBtnDown->SetEnable(true);
	//UIBtnUp->SetEnable(true);
		
	UIBtnDown->FadeIn(4.0f);
	//UIBtnUp->FadeIn(4.0f);
}

void L7S3_Pop3Chain::HideUI()
{
	UIBtnDown->SetEnable(false);
	UIBtnUp->SetEnable(false);
}

void L7S3_Pop3Chain::StartSceneSlideIn(bool fromLeft)
{
	SetPopupState(ePopupState_Open);

	slideTimer = 0;

	SaveXY();
	isFadingIn = true;
	isSliding = true;
	SetSceneAlpha(0);

	maxDisplacement = abs(maxDisplacement); //positive
	if(fromLeft)
		maxDisplacement = -maxDisplacement;

	ShiftXY(maxDisplacement,0);
}

void L7S3_Pop3Chain::StartSceneSlideOut(bool toLeft)
{
	slideTimer = 0;

	HideUI();

	SaveXY();
	isFadingIn = false;
	isSliding = true;
	maxDisplacement = abs(maxDisplacement); //positive

	if(toLeft)
		maxDisplacement = -maxDisplacement; //shift scene to left

	//SetPopupState(ePopupState_Closed);
}

void L7S3_Pop3Chain::resetScene()
{
	RestoreXY();
	SetSceneAlpha(1.0f);
	if(isFadingIn)
	{
		_popupAlpha = 1.0f;
	}
	else
	{
		_popupTransitionTimer = 0;
		_popupAlpha = 0;
	}
}

//Popup ID and LoadPrevious if true, next if false
void L7S3_Pop3Chain::setLoadNextCallback(std::function<void (Popup*, bool)> _loadNextPopupCB)
{
	LoadNextPopup = _loadNextPopupCB;
}
//UI sliding handling

void L7S3_Pop3Chain::Required()
{

}

void L7S3_Pop3Chain::Update(float ds)
{
	Popup::Update(ds);
	if(GFInstance->IsActivePopup)
		return;

	if(isSliding)
	{
		slideTimer += Control::LogicRate*maxVelocity;
		if(!isFadingIn)
		{
			SetSceneAlpha(1.0f-slideTimer);
			ShiftXY(slideTimer*maxDisplacement,0);
		}
		else
		{
			SetSceneAlpha(slideTimer);
			ShiftXY(maxDisplacement - slideTimer*maxDisplacement,0);
		}

		if(slideTimer>1.0f)
		{
			//SLOG::Log("End of Pop wall 2 trans");
			//end of slide
			isSliding = false;

			if(isFadingIn)
				SetupAndShowUI();
			else
				SetPopupState(ePopupState_Closed);

			resetScene();
		}
		return;
	}

	if(GetPopupState()!= ePopupState_Open || Hud->IsDraggingItem())
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

	UIBtnUp->PlayAnimation(0);
	UIBtnDown->PlayAnimation(0);
	if(BtnRectDownButton.Intersection(&pos))
	{		
		UIBtnDown->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			UIBtnDown->PlayAnimation(0);
			StartSceneSlideOut(false);
			//Load Next One also
			if(LoadNextPopup!=nullptr)
				LoadNextPopup(this, true); //load Map Wall 1
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken)
	{
		if(InvDestRectBullKey.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("axes")->Shake(90, 3, 0, 0.35f);
				Hud->ShowBannerText("L7time");
			}
			return;
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken)
	{
		if(InvItemRectMetalWire.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("metalwire")->SetEnable(false);

				ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken = true;
				Hud->TakeInventoryItem(eInv_L7_S3_MetalWire);

				if( ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken && 
					ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken )
					ClosePopup(1.5f);
			}
			return;
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken)
	{
		if(InvDestRectLeverchain.Intersection(&pos))
		{
			if(!InvDestMinusRectChain.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					GetObjectByID("gear")->Shake(0,0, 15, 0.35f);
					GetObjectByID("chainright")->Shake(90,4, 0, 0.4f);
					GetObjectByID("chaintop")->Shake(180,4, 0, 0.4f);
					GetObjectByID("chainleft")->Shake(270,4, 0, 0.4f);
					Hud->ShowBannerText("L7chain");
				}
				return;
			}
		}
	}

	/*if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BStoneRemoved)
	{
		if(ActionRectGroundBrick.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("stone")->TweenPosTo(-50, -20, -1, 1.0f, 0, true);
				ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BStoneRemoved = true;
			}
		}
	}*/
	
}

void L7S3_Pop3Chain::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		Hud->ReleaseDrag();
		Hud->CloseInventory();
		return;
	}

	if (itemVal == eInv_L7_S3_AxeKey && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken && InvDestRectBullKey.Intersection(&dropPos))
	{
		ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken = true;

		isAnimationPlaying = true;
		Control::Audio->QuickLoadAndPlaySFX("s3p3_keyused");

		JAnim_BullkeyFix->PlayAnim();
		
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S3_AxeKey] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S3_AxeKey);
	}
	else if (itemVal == eInv_L7_S3_Lever && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && InvDestRectLeverchain.Intersection(&dropPos))
	{
		isAnimationPlaying = true;
		
		Control::Audio->QuickLoadAndPlaySFX("s3p3_leverused");

		JAnim_LeverChainUnlock->PlayAnim();
		
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S3_Lever] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S3_Lever);
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L7S3_Pop3Chain::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
	{
		Hud->ShowHintForInvItem(eInv_L7_S3_AxeKey,&InvDestRectBullKey);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvLeverTaken)
	{
		Hud->ShowHintForInvItem(eInv_L7_S3_Lever,&GearInfoRect);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectMetalWire);
	}
	else
	{
		bool isBottomPopupTaskLeft = false;

		//Bhasmam popup
		if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam && ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
			isBottomPopupTaskLeft = true;
		else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken)
			isBottomPopupTaskLeft = true;

		//Bull popup
		if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
			isBottomPopupTaskLeft = true;
		else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[0])
			isBottomPopupTaskLeft = true;
		else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
			isBottomPopupTaskLeft = true;
		else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
			isBottomPopupTaskLeft = true;

		if(isBottomPopupTaskLeft)
		{
			//Shows Down Button
			Hud->InitHintTrailEmit(&BtnRectDownButton, true);
			return;
		}

		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L7S3_Pop3Chain::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
	{
		isAnyTaskLeft = true;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvLeverTaken)
	{
		isAnyTaskLeft = true;
	}
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L7S3_Pop3Chain::~L7S3_Pop3Chain()
{
	SAFE_DELETE(JAnim_LeverChainUnlock);
	SAFE_DELETE(JAnim_BullkeyFix);
}
