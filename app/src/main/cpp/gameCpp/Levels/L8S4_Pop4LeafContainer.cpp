
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8S4_Pop4LeafContainer.h"
#include "Hud.h"

const CRectangle  LeafRect(472.0f,191.0f,408.0f,311.0f);


L8S4_Pop4LeafContainer::L8S4_Pop4LeafContainer(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	isAnimationPlaying = false;
	//Popup load
	Load("LV8\\S4\\POP4\\POP4.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	GetObjectByID("Hand")->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer )
	{
		GetObjectByID("BoxClosed")->SetEnable(false);
		GetObjectByID("BoxGlow")->SetEnable(false);
		GetObjectByID("GreenLeaf")->SetEnable(false);
		GetObjectByID("YellowLeaf")->SetEnable(false);
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed )
	{
		GetObjectByID("BoxClosed")->SetEnable(false);
		GetObjectByID("BoxGlow")->SetEnable(false);
		GetObjectByID("GreenLeaf")->SetEnable(false);
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
	{
		GetObjectByID("BoxLidOpen")->SetEnable(false);
		GetObjectByID("GreenLeaf")->SetEnable(false);
		GetObjectByID("YellowLeaf")->SetEnable(false);
		GetObjectByID("BoxGlow")->SetEnable(false);
	}

	
	InitAnimations();
}

void L8S4_Pop4LeafContainer::Required()
{

}

void L8S4_Pop4LeafContainer::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if( Hud->IsDraggingItem() )
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

	if( LeafRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed &&
		ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer)
	{
		Cursor::SetMode(CA_HandPointy);
		if( Control::Input->LBclicked() )
		{
			ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed = true;
			ContainerClosing->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s4p4_closeLid");
			isAnimationPlaying = true;
		}
		return;
	}
	if( LeafRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S4_ContainerWithLeafOpened &&
		ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			//Show Objective completed
			if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L8O3_PurifyLeaf] && 
				!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L8O3_PurifyLeaf] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L8O3_PurifyLeaf] = true;
				Hud->ShowObjectiveFX(eObj_L8O3_PurifyLeaf,true);
			}

			Cursor::SetMode(CA_Normal);
			ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken = true;
			Hud->TakeInventoryItem(eInv_L8_S4_ContainerWithPurifiedLeaf);
			GetObjectByID("BoxOpen")->SetEnable(false);
			GetObjectByID("GreenLeaf")->SetEnable(false);
			GetObjectByID("BoxClosed")->SetEnable(false);
			GetObjectByID("BoxLidOpen")->SetEnable(false);
			GetObjectByID("YellowLeaf")->SetEnable(false);
			GetObjectByID("Hand")->SetEnable(false);
			GetObjectByID("BoxGlow")->SetEnable(false);
			UpdateBgGraphic();
			ClosePopup(1.0f);
		}
		return;
	}

	if(  LeafRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed && 
		!ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer)
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L8got");
	}


	/*if( LeafRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S4_ContainerWithLeafOpened &&
		ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved )
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			ProfileSystem::Instance->ActiveProfile.L8_S4_ContainerWithLeafOpened = true;
			GetObjectByID("GreenLeaf")->SetEnable(true);
			ContainerOpen->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s4p4_openLid");
			isAnimationPlaying = true;
		}
		return;
	}
	if( LeafRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken &&
		ProfileSystem::Instance->ActiveProfile.L8_S4_ContainerWithLeafOpened )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken = true;
			Hud->TakeInventoryItem(eInv_L8_S4_ContainerWithPurifiedLeaf);
			GetObjectByID("BoxOpen")->SetEnable(false);
			GetObjectByID("GreenLeaf")->SetEnable(false);
			GetObjectByID("BoxClosed")->SetEnable(false);
			GetObjectByID("BoxLidOpen")->SetEnable(false);
			GetObjectByID("YellowLeaf")->SetEnable(false);
			GetObjectByID("Hand")->SetEnable(false);
			GetObjectByID("BoxGlow")->SetEnable(false);
			UpdateBgGraphic();
		}
		return;
	}*/
	
}


void L8S4_Pop4LeafContainer::InitAnimations()
{
	ContainerClosing = NULL;
	ContainerClosing = new JSONAnimator( 0, CC_CALLBACK_1(L8S4_Pop4LeafContainer::OnAnimationComplete, this ));
	ContainerClosing->parseAndLoadJSON("ANIM\\L8\\S4POP4LIDCLOSE.JSON", this );

	ContainerOpen = NULL;
	ContainerOpen = new JSONAnimator( 1, CC_CALLBACK_1(L8S4_Pop4LeafContainer::OnAnimationComplete, this ));
	ContainerOpen->parseAndLoadJSON("ANIM\\L8\\S4POP4LIDOPEN.JSON", this );
}

void L8S4_Pop4LeafContainer::OnAnimationComplete(int index)
{
	isAnimationPlaying = false;
	switch (index)
	{
	case 0:	UpdateBgGraphic();
			ClosePopup(0.5f);
		break;
	case 1:	ContainerOpen->SetEnableObjects(false);
			GetObjectByID("BoxOpen")->SetEnable(true);
			GetObjectByID("GreenLeaf")->SetEnable(true);
			GetObjectByID("BoxClosed")->SetEnable(true);
			GetObjectByID("BoxLidOpen")->SetEnable(true);
		break;
	}
}


void L8S4_Pop4LeafContainer::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(LeafRect.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer )
	{
		if (itemVal == eInv_L8_S3_MaryLeaf )
		{
			GetObjectByID("YellowLeaf")->SetEnable(true);
			Control::Audio->QuickLoadAndPlaySFX("s4p4_placeYellowLeaf");
			ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer  = true;
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S3_MaryLeaf);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_MaryLeaf] = false;
			Hud->SendUpdateSceneMessage();
		}
		return;
	}
	GFInstance->WrongInvDropMsg(Hud);
}

void L8S4_Pop4LeafContainer::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if( !ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer && ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken )
		Hud->ShowHintForInvItem(eInv_L8_S3_MaryLeaf, &LeafRect);
	else if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed )
		Hud->InitHintTrailEmit( &LeafRect);
	else if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S4_ContainerWithLeafOpened )
		Hud->InitHintTrailEmit( &LeafRect);
	else if( ProfileSystem::Instance->ActiveProfile.L8_S4_ContainerWithLeafOpened && !ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
		Hud->InitHintTrailEmit( &LeafRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L8S4_Pop4LeafContainer::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced || !ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced || 
		!ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced )
	{
		isAnyTaskLeft = false;
		return isAnyTaskLeft;
	}
	if( !ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer && ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken )
		isAnyTaskLeft = true;
	else if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed )
		isAnyTaskLeft = true;
	else if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S4_ContainerWithLeafOpened )
		isAnyTaskLeft = true;
	else if( ProfileSystem::Instance->ActiveProfile.L8_S4_ContainerWithLeafOpened && !ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L8S4_Pop4LeafContainer::~L8S4_Pop4LeafContainer()
{
	SAFE_DELETE(ContainerOpen);
	SAFE_DELETE(ContainerClosing);
}
