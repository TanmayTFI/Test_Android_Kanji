
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8S2_Pop1Gate.h"
#include "Hud.h"

const CRectangle  LockRect(529.0f,377.0f,209.0f,179.0f);


L8S2_Pop1Gate::L8S2_Pop1Gate(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV8\\S2\\POP1\\POP1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	KeyInsertAnim = NULL;
	KeyInsertAnim = new JSONAnimator( 0, CC_CALLBACK_1(L8S2_Pop1Gate::OnAnimationComplete, this ));
	KeyInsertAnim->parseAndLoadJSON("ANIM\\L8\\S2GATEKEYINSERT.JSON", this );

	KeyTurnAnim = NULL;
	KeyTurnAnim = new JSONAnimator( 1, CC_CALLBACK_1(L8S2_Pop1Gate::OnAnimationComplete, this ));
	KeyTurnAnim->parseAndLoadJSON("ANIM\\L8\\S2GATEOPEN.JSON", this );

	GetObjectByID("KeyPose02")->SetEnable(false);
	GetObjectByID("LockOpen")->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced )
		GetObjectByID("KeyPose01")->SetEnable(false);

}

void L8S2_Pop1Gate::Required()
{

}


void L8S2_Pop1Gate::OnAnimationComplete(int index)
{	
	isAnimationPlaying = false;
	switch (index)
	{
	case 0:	
		break;
	case 1:ClosePopup(1.5f);
		break;
	}

}

void L8S2_Pop1Gate::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();


	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(LockRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened && ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced )
	{
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			KeyTurnAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p1_lockOpen");
			ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened  = true;
			Hud->SendUpdateSceneMessage();
		}
		return;
	}


	//handle interactions here


	if(LockRect.Intersection(&pos) && !Hud->IsDraggingItem() && !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened )
	{
		Cursor::SetMode(CA_Gear);
		if(Control::Input->LBclicked())
			Hud->ShowBannerText("L8surprise");
	}
}

void L8S2_Pop1Gate::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(LockRect.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced )
	{
		if (itemVal == eInv_L8_S3_CrystalKey )
		{
			isAnimationPlaying = true;
			ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced  = true;
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			Control::Audio->QuickLoadAndPlaySFX("s2p1_placeKeyOnGate");
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S3_CrystalKey);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_CrystalKey] = false;
			KeyInsertAnim->PlayAnim();
		}
		return;
	}
	GFInstance->WrongInvDropMsg(Hud);
}

void L8S2_Pop1Gate::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;



	// hint code goes here
	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_CrystalKey])
		Hud->ShowHintForInvItem(eInv_L8_S3_CrystalKey, &LockRect);
	else if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened && ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced )
		Hud->InitHintTrailEmit( &LockRect, false );
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L8S2_Pop1Gate::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_CrystalKey])
		isAnyTaskLeft = true;
	else if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened && ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced )
		isAnyTaskLeft = true;


	return isAnyTaskLeft;
}

L8S2_Pop1Gate::~L8S2_Pop1Gate()
{
	SAFE_DELETE(KeyInsertAnim);
	SAFE_DELETE(KeyTurnAnim);
}
