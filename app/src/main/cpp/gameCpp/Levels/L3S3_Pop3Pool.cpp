//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 03 - POPUP 3: Pool
//====================================

#include "L3S3_Pop3Pool.h"
#include "Hud.h"

const CRectangle  DestRectWater(375.0f,170.0f,601.0f,372.0f);
const CRectangle  InvItemRectLadderPiece(409.0f,312.0f,176.0f,78.0f);

L3S3_Pop3Pool::L3S3_Pop3Pool(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S3\\POP3\\P3.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	mesh_Water = NULL;
	mesh_Water = new MeshObject(GetObjectByID("meshwater"), 5, 5);
	mesh_Water->meshFX->SetSpeedAndDisplacement(4, 2, 4, 2);
	InsertObject(mesh_Water, 1);

	Show(false);

	OilDropsVFX = NULL;
	JAnim_CupWaterTaking = NULL;

	/*std::vector<SuperFX*> superfxArray;
	OilDropsVFX = new SuperFX("PRT\\L2\\s4oildrops.INI", PointSystem::CreateCPoint(488,106,0), zOrder+10); 
	superfxArray.push_back(OilDropsVFX);*/
	
	JAnim_CupWaterTaking = new JSONAnimator(0, CC_CALLBACK_1(L3S3_Pop3Pool::OnAnimationComplete, this));
	JAnim_CupWaterTaking->parseAndLoadJSON("ANIM\\L3\\S3CUPWATERTAKING.JSON", this);

	GetObjectByID("cupempty")->SetEnable(false);
	GetObjectByID("cupfilled")->SetEnable(false);

	GetObjectByID("watermask")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken)
		GetObjectByID("ladderpiece")->SetEnable(false);
	else
		GetObjectByID("ladderpiece")->Animate(1, 2, 1, 0, 4.0f);
}

void L3S3_Pop3Pool::Required()
{

}

void L3S3_Pop3Pool::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L3S3_CupEmpty && DestRectWater.Intersection(&dropPos))
	{
		Control::Audio->QuickLoadAndPlaySFX("s3p3_watertake");

		JAnim_CupWaterTaking->PlayAnim();
		isAnimationPlaying = true;

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken = true;

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_CupEmpty] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S3_CupEmpty);
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}
}

void L3S3_Pop3Pool::Update(float ds)
{
	Popup::Update(ds);
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

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken && InvItemRectLadderPiece.Intersection(&pos) && !Hud->IsDraggingItem())
	{
		Cursor::SetMode(CA_HandTake);
		if(Control::Input->LBclicked())
		{
			GetObjectByID("ladderpiece")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken = true;
			Hud->TakeInventoryItem(eInv_L3S3_LadderPiece2, GetObjectByID("ladderpiece")->Pos);

			if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken)
				ClosePopup(1.5f);
			return;
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken && DestRectWater.Intersection(&pos) && !Hud->IsDraggingItem())
	{
		Cursor::SetMode(CA_Gear);
		if(Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L3water");
		}
	}
}

void L3S3_Pop3Pool::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//Cup filled taken
		isAnimationPlaying = false;

		GetObjectByID("cupempty")->SetEnable(false);
		GetObjectByID("cupfilled")->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L3S3_CupWaterFilled);

		if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken)
			ClosePopup(1.5f);
	}
}

L3S3_Pop3Pool::~L3S3_Pop3Pool()
{
	SAFE_DELETE(OilDropsVFX);
	SAFE_DELETE(JAnim_CupWaterTaking);
}

void L3S3_Pop3Pool::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(isAnimationPlaying)
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken)
	{
		Hud->InitHintTrailEmit(&InvItemRectLadderPiece);
	}
	else if( ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupTaken && !ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken)
	{
		Hud->ShowHintForInvItem(eInv_L3S3_CupEmpty, &DestRectWater);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3S3_Pop3Pool::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken)
		isAnyTaskLeft = true;

	if( ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupTaken && !ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}
