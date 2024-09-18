//====================================
//	DQFYLH
//	HO System
//	Level 8 - Scene 03 - POPUP 2: 
//====================================

#include "L8S3_Pop2HiddenCompartment.h"


L8S3_Pop2HiddenCompartment::L8S3_Pop2HiddenCompartment(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV8\\S3\\P2\\P2.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	janim_Floor = new JSONAnimator(0, CC_CALLBACK_1(L8S3_Pop2HiddenCompartment::OnAnimComplete,this));
	janim_Floor->parseAndLoadJSON("ANIM\\L8\\L8_S3_P2_FLOORCRACKING.JSON",this);

	if (ProfileSystem::Instance->ActiveProfile.L8_S3_FloorCracked)
	{
		GetObjectByID("floor_1")->SetEnable(false);
		GetObjectByID("floor_02")->SetEnable(false);
		GetObjectByID("floor_03")->SetEnable(false);
	}

	SetSparkleUpdate(false);
	Show(false);
}


void L8S3_Pop2HiddenCompartment::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_FloorCracked)
	{
		Control::Audio->QuickLoadAndPlaySFX("s3p2_groundBreaking");
		janim_Floor->PlayAnim();
		//ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[2] = true;
		ProfileSystem::Instance->ActiveProfile.L8_S3_FloorCracked = true;
	}
	else if (GetObjectByID("goddess-statue")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("goddess-statue")->SetEnable(false);
			GetObjectByID("glow")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L8_S3_IdolBronze);
			ProfileSystem::Instance->ActiveProfile.L8_S3_InvIdolTaken = true;
			ClosePopup(1.5f);
		}
	}
}

void L8S3_Pop2HiddenCompartment::OnPopupOpened()
{
	ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[2] = true;

	bool allSymbolsFound = true;
	for (int i = 0; i < MAX_L8_MAGIC_SYMBOLS; i++)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[i])
		{
			allSymbolsFound = false;
			return;
		}
	}

	if (allSymbolsFound)
	{
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_MagicGlass] = false;
		Hud->ReleaseDrag();
		Hud->CloseInventory();
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S1_MagicGlass);
	}
}

void L8S3_Pop2HiddenCompartment::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	//switch (itemVal)
	//{
	//	//case eInv_L6_S1_Club:
	//	//	if (GetObjectByID("IceCoreWithKey")->GetWorldRect().Intersection(&dropPos))
	//	//	{
	//	//		Hud->ShowBannerText("L6just");
	//	//		Hud->CancelDrag();
	//	//	}
	//	//	break;
	//default:
	//	GFInstance->WrongInvDropMsg(Hud);
	//	break;
	//}
}

void L8S3_Pop2HiddenCompartment::OnAnimComplete(int animID)
{

}

float L8S3_Pop2HiddenCompartment::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L8S3_Pop2HiddenCompartment::~L8S3_Pop2HiddenCompartment()
{
	SAFE_DELETE(janim_Floor);
}

void L8S3_Pop2HiddenCompartment::Required()
{

}

void L8S3_Pop2HiddenCompartment::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L8S3_Pop2HiddenCompartment::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_InvIdolTaken )
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L8S3_Pop2HiddenCompartment::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L8_S3_InvIdolTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("goddess-statue")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("goddess-statue")->GetWorldRect();
		Hud->InitHintTrailEmit(&ObjRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
