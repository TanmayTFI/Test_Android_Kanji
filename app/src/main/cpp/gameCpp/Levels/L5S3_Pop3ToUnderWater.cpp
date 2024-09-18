//====================================
//  created by : JIthin M
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S3_Pop3ToUnderWater.h"
#include "Hud.h"
#include "MeshObject.h"

L5S3_Pop3ToUnderWater::L5S3_Pop3ToUnderWater(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S3\\POP3\\POP3.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	SetPopupBoundingBox(GetObjectByID("bg"));

	GetObjectByID("bag")->SetEnable(false);
	water = NULL;
	water = new MeshObject(GetObjectByID("water"), 15, 15);
	water->meshFX->SetSpeedAndDisplacement(3, 1, 4, 2);
	PushObject(water);
	waterEffect = NULL;
	waterEffect = new MeshObject(GetObjectByID("water_effect"), 15, 15);
	waterEffect->meshFX->SetSpeedAndDisplacement(3, 1, 4, 2);
	PushObject(waterEffect);

	JAnim_BagPlacing = new JSONAnimator(1, CC_CALLBACK_1(L5S3_Pop3ToUnderWater::OnAnimationComplete, this));
	JAnim_BagPlacing->parseAndLoadJSON("ANIM\\L5\\S3_UNDERWATERDIVEBAG.JSON", this);

	Show(false);
}

void L5S3_Pop3ToUnderWater::Required()
{

}

void L5S3_Pop3ToUnderWater::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
		return;

	if (isMouseOverCloseButton)
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (_popupBoundingBox.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);

		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText(Banner_L5_S3_NoDivingBell);
		}
	}
	//handle interactions here
}

void L5S3_Pop3ToUnderWater::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (itemVal == eInv_L5_S5_DivingBag)
	{
		Control::Audio->QuickLoadAndPlaySFX("s3p3_sackuse");

		Hud->ShowBannerText(Banner_L5_S3_UsingDivingBell);
		JAnim_BagPlacing->PlayAnim();
		isAnimationPlaying = true;

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S5_DivingBag] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S5_DivingBag);
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}
}

void L5S3_Pop3ToUnderWater::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if (ProfileSystem::Instance->ActiveProfile.L5_S5_InvWaterproofBagTaken)
	{
		Hud->ShowHintForInvItem(eInv_L5_S5_DivingBag, &_popupBoundingBox);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L5S3_Pop3ToUnderWater::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if (ProfileSystem::Instance->ActiveProfile.L5_S3_isLostHook && 
		ProfileSystem::Instance->ActiveProfile.L5_S5_InvWaterproofBagTaken &&
		!ProfileSystem::Instance->ActiveProfile.L5_S3_isHoActive)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L5S3_Pop3ToUnderWater::~L5S3_Pop3ToUnderWater()
{
	SAFE_DELETE(JAnim_BagPlacing);
}

void L5S3_Pop3ToUnderWater::OnAnimationComplete(int animID)
{
	isAnimationPlaying = false;
	ProfileSystem::Instance->ActiveProfile.L5_S3_isHoActive = true;
	_interface->UpdateHOSystem(this);
	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_HOUnderwater;
	_interface->NewArea();
}
