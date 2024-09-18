//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 03 - POPUP 1: Honey COMB
//====================================

#include "L4S1_Pop1RightFoot.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "InteractiveObjectList.h"
#include "exMath.h"

const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);
const CRectangle  FeetRect(598.0f,208.0f,147.0f,223.0f);

L4S1_Pop1RightFoot::L4S1_Pop1RightFoot(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV4\\S1\\POP1\\POP1.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	SetSparkleUpdate(false);
	Show(false);

	MedPaste = GetObjectByID("PasteOnLeg");

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_SecondThornRemoved)
	{
		GetObjectByID("Mullu02")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_FirstThornRemoved)
	{
		GetObjectByID("Mullu01")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
	{
		GetObjectByID("Shoe")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied)
	{
		MedPaste->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved)
	{
		GetObjectByID("Blood")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied)
	{
		GetObjectByID("ClothPiece")->SetEnable(false);
	}

	GetObjectByID("Water")->SetEnable(false);
	GetObjectByID("Leaf_Spoon(Water)")->SetEnable(false);
	GetObjectByID("Leaf_Spoon(Water)")->SetZ(GetObjectByID("Leaf_Spoon(Water)")->ZOrder + 100);
	//GetObjectByID("Shoe")->SetZ(GetObjectByID("Shoe")->ZOrder + 100);
	GetObjectByID("LeafSpoonMerged")->SetEnable(false);

	Janim_thorn1Remove = new JSONAnimator(0,CC_CALLBACK_1(L4S1_Pop1RightFoot::OnThornRemoved, this));
	Janim_thorn1Remove->parseAndLoadJSON("ANIM\\L4\\LV4_S1_POP1_THORN REMOVE1.JSON", this);

	Janim_thorn2Remove = new JSONAnimator(1,CC_CALLBACK_1(L4S1_Pop1RightFoot::OnThornRemoved, this));
	Janim_thorn2Remove->parseAndLoadJSON("ANIM\\L4\\LV4_S1_POP1_THORN REMOVE2.JSON", this);

	Janim_removeShoe = new JSONAnimator(2,CC_CALLBACK_1(L4S1_Pop1RightFoot::OnShoeRemoved,this));
	Janim_removeShoe->parseAndLoadJSON("ANIM\\L4\\LV4_S1_POP1_SHOE REMOVE.JSON", this);

	Janim_cleanWithWater = new JSONAnimator(3);
	Janim_cleanWithWater->parseAndLoadJSON("ANIM\\L4\\LV4_S1_POP1_WATER.JSON", this);

	Janim_shoePutBack = new JSONAnimator(4,CC_CALLBACK_1(L4S1_Pop1RightFoot::OnShoePutBack,this));
	Janim_shoePutBack->parseAndLoadJSON("ANIM\\L4\\LV4_S1_POP1_SHOE_PUT_BACK_.JSON", this);

	isThornRemoving = false;
}

void L4S1_Pop1RightFoot::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (GetObjectByID("Mullu01")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_FirstThornRemoved && !isThornRemoving)
	{
		Cursor::SetMode(CA_HandTake);
		if(Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p1_thornRemove1");
			isThornRemoving = true;
			Janim_thorn1Remove->PlayAnim();
			UpdateBgGraphic();
			//GetObjectByID("Mullu01")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L4_S1_FirstThornRemoved = true;
		}
	}
	else if (GetObjectByID("Mullu02")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_SecondThornRemoved && !isThornRemoving)
	{
		Cursor::SetMode(CA_HandTake);
		if(Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p1_thornRemove2");
			isThornRemoving = true;
			Janim_thorn2Remove->PlayAnim();
			UpdateBgGraphic();
			//GetObjectByID("Mullu02")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L4_S1_SecondThornRemoved = true;
		}
	}
	else if (FeetRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved && ProfileSystem::Instance->ActiveProfile.L4_S1_FirstThornRemoved && ProfileSystem::Instance->ActiveProfile.L4_S1_SecondThornRemoved && !isThornRemoving)
	{
		Cursor::SetMode(CA_HandTake);
		if(Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p1_shoeRemove");
			Janim_removeShoe->PlayAnim();
			ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved = true;
		}

	}
	else if (FeetRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L4better"); 
	}
	else if (FeetRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L4apply");
	}
	else if (FeetRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied && !ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L4bandage");
	}
	else if ((!ProfileSystem::Instance->ActiveProfile.L4_S1_FirstThornRemoved || !ProfileSystem::Instance->ActiveProfile.L4_S1_SecondThornRemoved) && GetObjectByID("Mullu01")->GetWorldRect().Intersection(&pos) == false && GetObjectByID("Mullu02")->GetWorldRect().Intersection(&pos) == false)
	{
		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L4remove");
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack && FeetRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
	}
}

void L4S1_Pop1RightFoot::OnShoeRemoved(int animID)
{

	Hud->TakeInventoryItem(eInv_L4_S1_Shoe2);
	GetObjectByID("Shoe")->SetEnable(false);
	UpdateBgGraphic();
}

void L4S1_Pop1RightFoot::OnThornRemoved(int animID)
{
	isThornRemoving = false;
	UpdateBgGraphic();
}

void L4S1_Pop1RightFoot::OnShoePutBack(int animID)
{
	UpdateBgGraphic();
	ClosePopup(1.5f);
}


void L4S1_Pop1RightFoot::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L4_S1_LeafSpoonWithWater:
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && FeetRect.Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p1_waterPour");
			Janim_cleanWithWater->PlayAnim();
			//GetObjectByID("Blood")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_LeafSpoonWithWater] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_LeafSpoonWithWater);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L4_S1_LeafSpoon:
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && FeetRect.Intersection(&dropPos))
		{
			Hud->ShowBannerText("L4pour");
			Hud->CancelDrag();
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L4_S1_Medicine:
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied && FeetRect.Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p1_applyMedicine");
			CPoint *initPos = new CPoint(MedPaste->InitPos);
			MedPaste->SetPos(initPos);
			delete(initPos);
			MedPaste->SetAlpha(1);
			MedPaste->SetEnable(true);
			ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Medicine] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_Medicine);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L4_S1_KerchiefPiece1:
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied && !ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && FeetRect.Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p1_applyBandage");
			GetObjectByID("ClothPiece")->SetEnable(true);
			ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied = true;
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_KerchiefPiece1] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_KerchiefPiece1);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L4_S1_KerchiefPiece2:
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied && !ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && FeetRect.Intersection(&dropPos))
		{
			Hud->ShowBannerText("L4cloth");
			Hud->CancelDrag();
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L4_S1_Shoe2:
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack && FeetRect.Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p1_puttingShoe");
			GetObjectByID("Shoe")->SetEnable(true);
			Janim_shoePutBack->PlayAnim();
			ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack = true;
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Shoe2] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_Shoe2);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L4_S1_Shoe1:
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack && FeetRect.Intersection(&dropPos))
		{
			Hud->ShowBannerText("L4oops");
			Hud->CancelDrag();
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L4S1_Pop1RightFoot::Required()
{

}

float L4S1_Pop1RightFoot::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S1_Pop1RightFoot::~L4S1_Pop1RightFoot()
{
	SAFE_DELETE(Janim_shoePutBack);
	SAFE_DELETE(Janim_cleanWithWater);
	SAFE_DELETE(Janim_removeShoe);	
	SAFE_DELETE(Janim_thorn2Remove);
	SAFE_DELETE(Janim_thorn1Remove);
	
}


void L4S1_Pop1RightFoot::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L4S1_Pop1RightFoot::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved||
		(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved) ||
		(ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied && ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved) ||
		(ProfileSystem::Instance->ActiveProfile.L4_S1_HandKerchiefTored && !ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied) ||
		(ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S1_Pop1RightFoot::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved)
		Hud->InitHintTrailEmit(GetObjectByID("Shoe")->GetWorldRect().GetCenterX(), GetObjectByID("Shoe")->GetWorldRect().GetCenterY());

	else if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved)
		Hud->ShowHintForInvItem(eInv_L4_S1_LeafSpoonWithWater,&PopCircleRect);

	else if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied && ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved)
		Hud->ShowHintForInvItem(eInv_L4_S1_Medicine,&PopCircleRect);

	else if(ProfileSystem::Instance->ActiveProfile.L4_S1_HandKerchiefTored && !ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied)
		Hud->ShowHintForInvItem(eInv_L4_S1_KerchiefPiece1,&PopCircleRect);

	else if(ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
		Hud->ShowHintForInvItem(eInv_L4_S1_Shoe2,&PopCircleRect);

	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
