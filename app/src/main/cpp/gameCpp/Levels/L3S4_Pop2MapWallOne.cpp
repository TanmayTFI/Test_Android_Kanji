//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L3S4_Pop2MapWallOne.h"
#include "Hud.h"

const CRectangle  BtnRectLeftButton(267.0f,263.0f,83.0f,108.0f);
const CRectangle  BtnRectRightButton(1008.0f,260.0f,95.0f,113.0f);

const CRectangle  InvDestRectBlankSheet(379.0f,96.0f,565.0f,459.0f);
const CRectangle  ActionRectStatue(619.0f,391.0f,221.0f,176.0f);
const CRectangle  ActionRectLockedDoor(371.0f,96.0f,609.0f,471.0f);
const CRectangle  ActionRectGroundBrick(621.0f,474.0f,118.0f,66.0f);

const CRectangle  PopupMaskRect(357.0f,86.0f,651.0f,494.0f);

L3S4_Pop2MapWallOne::L3S4_Pop2MapWallOne(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S4\\POP2A\\P2A.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	JAnim_DoorOpening = NULL;
	JAnim_MapTracing = NULL;

	JAnim_DoorOpening = new JSONAnimator(0, CC_CALLBACK_1(L3S4_Pop2MapWallOne::OnAnimationComplete, this));
	JAnim_DoorOpening->parseAndLoadJSON("ANIM\\L3\\S4P2DOOROPENING.JSON", this);

	JAnim_MapTracing = new JSONAnimator(1, CC_CALLBACK_1(L3S4_Pop2MapWallOne::OnAnimationComplete, this));
	JAnim_MapTracing->parseAndLoadJSON("ANIM\\L3\\S4P2MAPDRAWHAND.JSON", this);

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AStatueMoved)
		GetObjectByID("statue")->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced)
	{
		GetObjectByID("handL")->SetEnable(false);
	}

	GetObjectByID("handL")->SetMaskRect(PopupMaskRect);
	GetObjectByID("handR")->SetMaskRect(PopupMaskRect);
	
	GetObjectByID("handR")->SetEnable(false);
	GetObjectByID("mapline1")->SetEnable(false);
	GetObjectByID("mapline2")->SetEnable(false);
	GetObjectByID("mapline3")->SetEnable(false);
	GetObjectByID("mapline4")->SetEnable(false);

	GetObjectByID("dooropenL1")->SetEnable(false);
	GetObjectByID("dooropenR1")->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AMapDoorOpened)
	{
		GetObjectByID("dooropenR2")->SetEnable(false);
		GetObjectByID("dooropenL2")->SetEnable(false);
	}
	else
	{
		//Door Opened
		GetObjectByID("doorL")->SetEnable(false);
		GetObjectByID("doorR")->SetEnable(false);

		GetObjectByID("lockanim0")->SetEnable(false);
		GetObjectByID("lockanim1")->SetEnable(false);
		GetObjectByID("lockanim2")->SetEnable(false);
		GetObjectByID("lockanim3")->SetEnable(false);
	}

	GetObjectByID("groundglow")->Animate(0.45f, 0.85f, 2.0f);
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed)
		GetObjectByID("bricknormal")->SetEnable(false);
	else
		GetObjectByID("groundglow")->SetEnable(false);
		

	//UI sliding handling
	LoadNextPopup = nullptr;

	UINode0 = GetObjectByID("uinode0");
	UINode1 = GetObjectByID("uinode1");
	UINode2 = GetObjectByID("uinode2");
	UINode3 = GetObjectByID("uinode3");

	UINodeGlow = GetObjectByID("uinodeglow");
	
	UIBtnPrev = GetObjectByID("uileftbtn");
	UIBtnNext = GetObjectByID("uirightbtn");

	UINode0->SetEnable(false);
	UINode1->SetEnable(false);
	UINode2->SetEnable(false);
	UINode3->SetEnable(false);

	UIBtnPrev->SetEnable(false);
	UIBtnNext->SetEnable(false);

	UINodeGlow->SetEnable(false);

	maxVelocity = 5;

	maxDisplacement = 400.0f;
	slideDisplacement = 0;
	slideTimer = 0;

	isSliding = false;
	isFadingIn = false;
}

void L3S4_Pop2MapWallOne::OnPopupOpened()
{
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AMapDoorOpened)
		SetupAndShowUI();
}

void L3S4_Pop2MapWallOne::OnPopupCloseBtnClicked()
{
	HideUI();
}

//UI sliding handling
void L3S4_Pop2MapWallOne::SetupAndShowUI()
{
	UINode0->SetEnable(true);
	UINode1->SetEnable(true);
	UINode2->SetEnable(true);
	UINode3->SetEnable(true);

	UINodeGlow->SetEnable(true);
	
	UIBtnPrev->SetAlpha(0.25f);
	UIBtnPrev->SetEnable(true);
	UIBtnNext->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed)
		UINode0->PlayAnimation(1);
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BSnakeButtonPressed)
		UINode1->PlayAnimation(1);
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed)
		UINode2->PlayAnimation(1);
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2DSnakeButtonPressed)
		UINode3->PlayAnimation(1);

	UINode0->FadeIn(4.0f);
	UINode1->FadeIn(4.0f);
	UINode2->FadeIn(4.0f);
	UINode3->FadeIn(4.0f);

	UINodeGlow->FadeIn(4.0f);
		
	//UIBtnPrev->FadeIn(4.0f);
	UIBtnNext->FadeIn(4.0f);
}

void L3S4_Pop2MapWallOne::HideUI()
{
	UINode0->SetEnable(false);
	UINode1->SetEnable(false);
	UINode2->SetEnable(false);
	UINode3->SetEnable(false);

	UINodeGlow->SetEnable(false);

	UIBtnPrev->SetEnable(false);
	UIBtnNext->SetEnable(false);
}

void L3S4_Pop2MapWallOne::StartSceneSlideIn(bool fromLeft)
{
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AStatueMoved)
		GetObjectByID("statue")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed)
		GetObjectByID("bricknormal")->SetEnable(false);

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

void L3S4_Pop2MapWallOne::StartSceneSlideOut(bool toLeft)
{
	slideTimer = 0;

	HideUI();

	SaveXY();
	isFadingIn = false;
	isSliding = true;
	maxDisplacement = abs(maxDisplacement); //positive

	if(toLeft)
		maxDisplacement = -maxDisplacement; //shift scene to left
}

void L3S4_Pop2MapWallOne::resetScene()
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
void L3S4_Pop2MapWallOne::setLoadNextCallback(std::function<void (Popup*, bool)> _loadNextPopupCB)
{
	LoadNextPopup = _loadNextPopupCB;
}
//UI sliding handling

void L3S4_Pop2MapWallOne::Required()
{
	
}

void L3S4_Pop2MapWallOne::Update(float ds)
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
			//end of slide
			isSliding = false;

			resetScene();

			if(isFadingIn)
			{
				SetupAndShowUI();
			}
			else
				SetPopupState(ePopupState_Closed);
		}
		return;
	}

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
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AStatueMoved)
	{
		if(ActionRectStatue.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s4p2_map_rockmove");
				ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AStatueMoved = true;
				GetObjectByID("statue")->TweenPosTo(70, 20, -1, 1.0f, 0, true);
			}
			return;
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed)
	{
		if(ActionRectGroundBrick.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s4p2_map_tilepress");
				ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed = true;
				GetObjectByID("bricknormal")->FadeOut(2.0f);
				GetObjectByID("groundglow")->SetEnable(true);
				UINode0->PlayAnimation(1);
			}
			return;
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AMapDoorOpened)
	{
		UIBtnNext->PlayAnimation(0);
		//UIBtnPrev->PlayAnimation(0);
		/*if(BtnRectLeftButton.Intersection(&pos))
		{		
			UIBtnPrev->PlayAnimation(1);
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				StartSceneSlideIn();
			}
		}
		else*/ 
		if(BtnRectRightButton.Intersection(&pos))
		{
			UIBtnNext->PlayAnimation(1);
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				UIBtnNext->PlayAnimation(0);
				StartSceneSlideOut(true);
				//Load Next One also
				if(LoadNextPopup!=nullptr)
					LoadNextPopup(this, false); //load Map Wall 2
			}
		}

		if(ActionRectLockedDoor.Intersection(&pos))
		{
			if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed && ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BSnakeButtonPressed &&
				ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed && ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2DSnakeButtonPressed)
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					HideUI();					

					GetObjectByID("lockanim0")->PlayAnimation(1);
					GetObjectByID("lockanim1")->PlayAnimation(1);
					GetObjectByID("lockanim2")->PlayAnimation(1);
					GetObjectByID("lockanim3")->PlayAnimation(1);

					Control::Audio->QuickLoadAndPlaySFX("s4p2_map_dooropen");

					JAnim_DoorOpening->PlayAnim();
					isAnimationPlaying = true;
				}
			}
			else
			{
				Cursor::SetMode(CA_Help);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L3four");
				}
			}
		}
	}
	else
	{
		//Door Opened
		if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced)
		{
			if(ActionRectLockedDoor.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L3map");
				}
			}
		}
		else
		{
			if(InvDestRectBlankSheet.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L3sketch");
				}
			}
		}	
	}
}

void L3S4_Pop2MapWallOne::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//Door Opening
		isAnimationPlaying = false;
		ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AMapDoorOpened = true;

		Hud->ShowBannerText("L3advanced");

		Hud->SendUpdateSceneMessage();
		UpdateBgGraphic();
	}
	else if(animID == 1)
	{
		//Map Tracing
		isAnimationPlaying = false;

		ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken = true;
		Hud->TakeInventoryItem(eInv_L3S4_MetalSheetMapDrawn);

		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken)
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_FortInterior] = true;
		else
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = true;

		GetObjectByID("handL")->SetEnable(false);
		GetObjectByID("handR")->SetEnable(false);

		GetObjectByID("mapline1")->SetEnable(false);
		GetObjectByID("mapline2")->SetEnable(false);
		GetObjectByID("mapline3")->SetEnable(false);
		GetObjectByID("mapline4")->SetEnable(false);

		ClosePopup(1.5f);
	}
}

void L3S4_Pop2MapWallOne::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (itemVal == eInv_L3S4_MetalSheetBlank && ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AMapDoorOpened && ActionRectLockedDoor.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced)
		{
			ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced = true;

			Control::Audio->QuickLoadAndPlaySFX("s4p2_map_metalsheetplaced");

			GetObjectByID("handL")->SetEnable(true);
			GetObjectByID("handL")->FadeIn(2.0f);

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_MetalSheetBlank] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S4_MetalSheetBlank);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
	}
	else if (itemVal == eInv_L3S1_Charcoal && ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced && InvDestRectBlankSheet.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken)
		{
			ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken = true;
			
			Control::Audio->QuickLoadAndPlaySFX("s4p2_map_skechdraw");
			JAnim_MapTracing->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S1_Charcoal] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S1_Charcoal);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L3S4_Pop2MapWallOne::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AStatueMoved)
	{
		Hud->InitHintTrailEmit(&ActionRectStatue);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed)
	{
		Hud->InitHintTrailEmit(&ActionRectGroundBrick);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BSnakeButtonPressed
		|| !ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed || !ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2DSnakeButtonPressed)
	{
		Hud->InitHintTrailEmit(&BtnRectRightButton, true);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AMapDoorOpened)
	{
		Hud->InitHintTrailEmit(&ActionRectLockedDoor);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken && !ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced)
	{
		Hud->ShowHintForInvItem(eInv_L3S4_MetalSheetBlank, &ActionRectLockedDoor);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState >= 2 && ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced && !ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken)
	{
		Hud->ShowHintForInvItem(eInv_L3S1_Charcoal, &InvDestRectBlankSheet);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3S4_Pop2MapWallOne::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed || !ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BSnakeButtonPressed
		|| !ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed || !ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2DSnakeButtonPressed)
	{
		isAnyTaskLeft = true;
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed && ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BSnakeButtonPressed
		&& ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed && ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2DSnakeButtonPressed &&
		!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2AMapDoorOpened)
	{
		isAnyTaskLeft = true;
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken && !ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced)
	{
		isAnyTaskLeft = true;
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState >= 2 && ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced && !ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L3S4_Pop2MapWallOne::~L3S4_Pop2MapWallOne()
{
	SAFE_DELETE(JAnim_DoorOpening);
	SAFE_DELETE(JAnim_MapTracing);
}
