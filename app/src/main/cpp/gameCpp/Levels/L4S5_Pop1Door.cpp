//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 05 - POPUP 1: DOOR
//====================================

#include "L4S5_Pop1Door.h"

const CRectangle  ScrewRect(628.0f,419.0f,132.0f,66.0f);

L4S5_Pop1Door::L4S5_Pop1Door(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV4\\S5\\POP1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if (ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved)
	{
		GetObjectByID("ScrewL")->SetEnable(false);
		GetObjectByID("ScrewR")->SetEnable(false);
		GetObjectByID("DoorMetal")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken)
	{
		GetObjectByID("Door")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved)
	{
		GetObjectByID("Cloth")->SetEnable(false);
	}
	GetObjectByID("Knife")->SetEnable(false);

	GetObjectByID("Cloth")->SetMaskRect(GetObjectByID("BG")->GetWorldRect());

	Janim_screwRemoval = new JSONAnimator(0,CC_CALLBACK_1(L4S5_Pop1Door::OnAnimComplete,this));
	Janim_screwRemoval->parseAndLoadJSON("ANIM\\L4\\S5_SCREWREMOVAL.JSON", this);

	int ArrayPts[] = {444,281,437,292,448,319,583,346,585,386,602,393,658,369,630,304,606,314,447,280};
	TwigRect =  new CPolygon(10);
	TwigRect->Init(ArrayPts);

	int ArrayPts1[] = {434,304,415,442,504,449,622,421,638,382,603,333,555,319,530,297,459,296,433,302};
	ClothRect=  new CPolygon(10);
	ClothRect->Init(ArrayPts1);

	SetSparkleUpdate(false);
	Show(false);

}

void L4S5_Pop1Door::Required()
{

}

void L4S5_Pop1Door::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}


void L4S5_Pop1Door::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (ScrewRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4bolt");
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken && GetObjectByID("Door")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("Door")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L4_S5_Door);
			ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken = true;
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken && !ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved && ClothRect->Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4piece");
			GetObjectByID("Cloth")->TweenPosTo(-150,0,-1,1,1,false);
			ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved = true;
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken && TwigRect->Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("WoodenPice")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L4_S5_Twig3);
			ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken = true;
			ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken = true;

			ClosePopup(1);
		}
	}
}

void L4S5_Pop1Door::OnAnimComplete(int animID)
{
	ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved = true;
}

void L4S5_Pop1Door::HandleInventoryDrop(CPoint dropPos, int itemVal)
{

	switch (itemVal)
	{
	case eInv_L4_CB_BambooWithKnife:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved)
		{
			Janim_screwRemoval->PlayAnim();
			
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			if (ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved && ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked && ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_CB_BambooWithKnife); 
			}
		}
		break;
	default:

		GFInstance->WrongInvDropMsg(Hud);
		break;
	}

}


float L4S5_Pop1Door::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S5_Pop1Door::~L4S5_Pop1Door()
{
	SAFE_DELETE(Janim_screwRemoval);
	SAFE_DELETE(TwigRect);
	SAFE_DELETE(ClothRect);
}

bool L4S5_Pop1Door::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved && ProfileSystem::Instance->ActiveProfile.L4_S2_InvBambooSpearTaken)
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken && ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved)
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken && !ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved)
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken && ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L4S5_Pop1Door::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved && ProfileSystem::Instance->ActiveProfile.L4_S2_InvBambooSpearTaken)
	{
		Hud->ShowHintForInvItem(eInv_L4_CB_BambooWithKnife, &ScrewRect);
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken && ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved)
	{
		//	Hud->InitHintTrailEmit(&GetObjectByID("Door")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle doorObjRect = GetObjectByID("Door")->GetWorldRect();
		Hud->InitHintTrailEmit(&doorObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorTaken && !ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved)
	{
		//	Hud->InitHintTrailEmit(&GetObjectByID("Cloth")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle clothObjRect = GetObjectByID("Cloth")->GetWorldRect();
		Hud->InitHintTrailEmit(&clothObjRect);
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken && ProfileSystem::Instance->ActiveProfile.L4_S5_ClothRemoved)
	{
		
		//	Hud->InitHintTrailEmit(&GetObjectByID("WoodenPice")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle woodObjRect = GetObjectByID("WoodenPice")->GetWorldRect();
		Hud->InitHintTrailEmit(&woodObjRect);

	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
