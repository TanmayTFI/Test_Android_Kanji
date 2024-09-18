//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 01 - POPUP 5: TWIG SQUARE
//====================================

#include "L4S1_Pop5TwigSquarePopup.h"

const CRectangle  InvDestBambooWithKnifeRect(505.0f,170.0f,404.0f,234.0f);

L4S1_Pop5TwigSquarePopup::L4S1_Pop5TwigSquarePopup(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV4\\S1\\POP5\\P5.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("Hook2")->SetEnable(false);

	mo_water = NULL;
	mo_water = new MeshObject(GetObjectByID("waterMask"), 5, 5);
	mo_water->meshFX->SetSpeedAndDisplacement(4, 2, 3, 3);
	InsertObject(mo_water, 1);


	if (ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved)
	{
		GetObjectByID("algae")->SetPos(PointSystem::CreateCPoint(GetObjectByID("algae")->GetPosPointer()->x - 150, GetObjectByID("algae")->GetPosPointer()->y + 110, GetObjectByID("algae")->GetPosPointer()->z));
		GetObjectByID("Bracelet-locket_Mask")->SetPos(PointSystem::CreateCPoint(GetObjectByID("Bracelet-locket_Mask")->GetPosPointer()->x - 150, GetObjectByID("Bracelet-locket_Mask")->GetPosPointer()->y + 110, GetObjectByID("Bracelet-locket_Mask")->GetPosPointer()->z));
	}
	else
	{
		GetObjectByID("algae")->Animate(1.5,3,0,0,4);
		GetObjectByID("Bracelet-locket_Mask")->Animate(1.5,3,0,0,4);
	}

	/*mo_twigs = NULL;
	mo_twigs = new MeshObject(GetObjectByID("algae"), 5, 5);
	mo_twigs->meshFX->SetSpeedAndDisplacement(2, 2, 3, 3);
	PushObject(mo_twigs);

	mo_locket = NULL;
	mo_locket = new MeshObject(GetObjectByID("Bracelet-locket_Mask"), 2, 2);
	mo_locket->meshFX->SetSpeedAndDisplacement(1, 1, 3, 3);
	PushObject(mo_locket);
	*/
	Janim_twigMoving = new JSONAnimator(0,CC_CALLBACK_1(L4S1_Pop5TwigSquarePopup::OnTwigMovingAnimComplete, this));
	Janim_twigMoving->parseAndLoadJSON("ANIM\\L4\\L4_S1_POP5_TWIGMOVINGANIM.JSON", this);

	SetSparkleUpdate(false);
	Show(false);



}

void L4S1_Pop5TwigSquarePopup::OnTwigMovingAnimComplete(int animID)
{
	ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved = true;
	//GetObjectByID("algae")->StopAnim();
	//GetObjectByID("algae")->Animate(1.5,3,0,0,4);
	//GetObjectByID("Bracelet-locket_Mask")->Animate(1.5,3,0,0,4);
}

void L4S1_Pop5TwigSquarePopup::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L4_CB_BambooWithKnife:
		if (InvDestBambooWithKnifeRect.Intersection(&dropPos))
		{
			GetObjectByID("Hook2")->SetEnable(true);
			GetObjectByID("algae")->StopAnim();
			GetObjectByID("Bracelet-locket_Mask")->StopAnim();
			Janim_twigMoving->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s1p5_takeMedallion");

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			if (ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved && ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked && ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_CB_BambooWithKnife); 
			}
		}
		break;
	case eInv_L4_S2_Bamboo:
		if (InvDestBambooWithKnifeRect.Intersection(&dropPos))
		{
			Hud->ShowBannerText("L4nope");
			Hud->CancelDrag();
		}
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}


void L4S1_Pop5TwigSquarePopup::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
	if (GetObjectByID("Bracelet-locket_Mask")->GetWorldRect().Intersection(&pos))
	{
		if (!ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L4reach");
			}
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken)
		{
			Cursor::SetMode(CA_HandTake);
			if (Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken = true;
				Hud->TakeInventoryItem(eInv_L4_S1_Bracelet);
				GetObjectByID("Bracelet-locket_Mask")->SetEnable(false);
				ClosePopup(1.5f);
			}
		}

	}

}

float L4S1_Pop5TwigSquarePopup::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S1_Pop5TwigSquarePopup::~L4S1_Pop5TwigSquarePopup()
{
	SAFE_DELETE(Janim_twigMoving);
}

void L4S1_Pop5TwigSquarePopup::Required()
{

}

void L4S1_Pop5TwigSquarePopup::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}


bool L4S1_Pop5TwigSquarePopup::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if ((ProfileSystem::Instance->ActiveProfile.L4_S2_InvBambooSpearTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved) ||
		(ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S1_Pop5TwigSquarePopup::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(ProfileSystem::Instance->ActiveProfile.L4_S2_InvBambooSpearTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved)
	{
		Hud->ShowHintForInvItem(eInv_L4_CB_BambooWithKnife, &InvDestBambooWithKnifeRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken)
	{
		Hud->InitHintTrailEmit(GetObjectByID("Bracelet-locket_Mask")->GetWorldRect().GetCenterX(), GetObjectByID("Bracelet-locket_Mask")->GetWorldRect().GetCenterY());
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
