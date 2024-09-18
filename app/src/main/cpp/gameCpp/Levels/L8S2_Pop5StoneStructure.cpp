
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8S2_Pop5StoneStructure.h"
#include "Hud.h"

const CRectangle  DoorRect(573.0f,231.0f,255.0f,221.0f);


L8S2_Pop5StoneStructure::L8S2_Pop5StoneStructure(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	isAnimationPlaying = false;
	//Popup load
	Load("LV8\\S2\\POP5\\POP5.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	int ScrollArrayPts[] = {477,374,773,404,762,493,457,459};
	ScrollPoly =  new CPolygon(4);
	ScrollPoly->Init(ScrollArrayPts);
	int KeyArrayPts[] = {403,461,578,495,533,565,404,536};
	ToolPoly =  new CPolygon(4);
	ToolPoly->Init(KeyArrayPts);

	KeyInsertAnim = NULL;
	KeyInsertAnim = new JSONAnimator( 0, CC_CALLBACK_1(L8S2_Pop5StoneStructure::OnAnimationComplete, this ));
	KeyInsertAnim->parseAndLoadJSON("ANIM\\L8\\S2DOORKEYINSERT.JSON", this );

	KeyTurnAnim = NULL;
	KeyTurnAnim = new JSONAnimator( 1, CC_CALLBACK_1(L8S2_Pop5StoneStructure::OnAnimationComplete, this ));
	KeyTurnAnim->parseAndLoadJSON("ANIM\\L8\\S2DOOROPEN.JSON", this );

	if(ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened)
	{
		GetObjectByID("DoorClose")->SetEnable(false);
		GetObjectByID("KeyAnim1")->SetEnable(false);
		GetObjectByID("KeyAnim2")->SetEnable(false); 
		GetObjectByID("KeyMask")->SetEnable(false); 
		if(ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken)
			GetObjectByID("ShukraKey")->SetEnable(false); 
		if(ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken)
			GetObjectByID("PaperScroll")->SetEnable(false); 
	}
	else
	{
		GetObjectByID("DoorOpen")->SetEnable(false);
		GetObjectByID("KeyAnim2")->SetEnable(false); 
		if(!ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorKeyPlaced)
			GetObjectByID("KeyAnim1")->SetEnable(false);
	}


}

void L8S2_Pop5StoneStructure::OnAnimationComplete(int index)
{	
	isAnimationPlaying = false;
	switch (index)
	{
	case 0:	
		break;
	case 1:	KeyTurnAnim->SetEnableObjects(false);
		GetObjectByID("DoorOpen")->SetEnable(true);
		break;
	}
	
}

void L8S2_Pop5StoneStructure::Required()
{

}

void L8S2_Pop5StoneStructure::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(DoorRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened && ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorKeyPlaced )
	{
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			Cursor::SetMode(CA_Normal);
			KeyTurnAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p5_OpenDoor");
			ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened  = true;
			Hud->SendUpdateSceneMessage();
		}
		return;
	}
	if(ScrollPoly->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken )
	{
		Cursor::SetMode(CA_HandTake);
		if(Control::Input->LBclicked())
		{
			Cursor::SetMode(CA_Normal);
			GetObjectByID("PaperScroll")->SetEnable(false); 
			Hud->TakeInventoryItem(eInv_L8_S2_SymbolCombination, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken  = true;
			Hud->SendUpdateSceneMessage();
			if(ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken)
				ClosePopup(1.5f);
		}
		
		return;
	}
	if(ToolPoly->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken )
	{
		Cursor::SetMode(CA_HandTake);
		if(Control::Input->LBclicked())
		{
			Cursor::SetMode(CA_Normal);
			GetObjectByID("ShukraKey")->SetEnable(false); 
			Hud->TakeInventoryItem(eInv_L8_S2_Tool, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken  = true;
			Hud->SendUpdateSceneMessage();
			if(ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken )
				ClosePopup(1.5f);
		}
		return;
	}

	if( DoorRect.Intersection(&pos) && !Hud->IsDraggingItem() && !ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorKeyPlaced)
	{
		Cursor::SetMode(CA_Gear);
		if(Control::Input->LBclicked())
			Hud->ShowBannerText("L8ages");
	}



	//handle interactions here
}

void L8S2_Pop5StoneStructure::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(DoorRect.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorKeyPlaced )
	{
		if (itemVal == eInv_L8_CB_FullKey )
		{
			isAnimationPlaying = true;
			ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorKeyPlaced  = true;
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_CB_FullKey);
			KeyInsertAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p5_placeKeyOnDoor");
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		return;
	}
	GFInstance->WrongInvDropMsg(Hud);
}

void L8S2_Pop5StoneStructure::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;
	
	// hint code goes here
	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorKeyPlaced && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey])
		Hud->ShowHintForInvItem(eInv_L8_CB_FullKey, &DoorRect);
	else if( ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorKeyPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened )
		Hud->InitHintTrailEmit(&DoorRect, false);
	else if( ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken)
		Hud->InitHintTrailEmit(ScrollPoly->center.x, ScrollPoly->center.y);
	else if( ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened  && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken)
		Hud->InitHintTrailEmit(ToolPoly->center.x, ToolPoly->center.y);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L8S2_Pop5StoneStructure::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorKeyPlaced && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey])
		isAnyTaskLeft = true;
	else if( ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorKeyPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened )
		isAnyTaskLeft = true;
	else if( ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken)
		isAnyTaskLeft = true;
	else if( ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened  && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L8S2_Pop5StoneStructure::~L8S2_Pop5StoneStructure()
{
	SAFE_DELETE(KeyInsertAnim);
	SAFE_DELETE(KeyTurnAnim);
}
