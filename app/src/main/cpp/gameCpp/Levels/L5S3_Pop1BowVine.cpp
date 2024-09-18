
//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S3_Pop1BowVine.h"
#include "Hud.h"
#include "MeshObject.h"

const CRectangle  PopupClickArea(522.0f-196,171.0f-189,438.0f,385.0f);
const CRectangle  MemoryRectStatue(919.0f,151.0f,80.0f,92.0f);
const CRectangle  NextSceenRect(503.0f,97.0f,180.0f,145.0f);

L5S3_Pop1BowVine::L5S3_Pop1BowVine(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S3\\POP1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	SetPopupBoundingBox(GetObjectByID("bg"));
	waterObj = NULL;
	waterObj = new MeshObject(GetObjectByID("WaterMesh"), 10, 10);
	waterObj->meshFX->SetSpeedAndDisplacement(4, 9, 5, 2);
	InsertObject(waterObj, 1);

	GetObjectByID("arrow")->SetEnable(false);
	GetObjectByID("hand_bow")->SetEnable(false);
	GetObjectByID("roop")->SetEnable(false);
	GetObjectByID("hand_2")->SetEnable(false);
	GetObjectByID("vine_2")->SetEnable(false);
	CObject* finalVine = GetObjectByID("vine_3");
	finalVine->SetEnable(false);

	PoolWaterFallFX = new SuperFX("PRT\\L5\\S3PoolWaterFall.INI", PointSystem::CreateCPoint(481,127,0), 1004); 
	PoolWaterFallBottomFX = new SuperFX("PRT\\L5\\S3PoolWaterFallBottom.INI", PointSystem::CreateCPoint(428,459,0), 1004); 
	
	CObject* _tree2 = GetObjectByID("tree_lt_down_2");
	_tree2->SetPivot(-59, 14);
	_tree2->Animate(0, 0, 4, 0, 4, 1.57f);

	CObject* _tree1 = GetObjectByID("tree_lt_down_1");
	_tree1->SetPivot(-28, 24);
	_tree1->Animate(0, 0, 2, 0, 4.5f);

	_isArrowDroped = false;
	_janim_ShootArrow = NULL;

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed)
	{
		_janim_ShootArrow = new JSONAnimator(1, CC_CALLBACK_1(L5S3_Pop1BowVine::OnAnimationComplete, this));
		_janim_ShootArrow->parseAndLoadJSON("ANIM\\L5\\S3VINECUTBOW.JSON", this);
	}
	else
	{
		GetObjectByID("vine_1")->SetEnable(false);
		finalVine->SetEnable(true);
		finalVine->SetPivot(9, -159);
		finalVine->Animate(0, 0, 2, 0, 4);

	}


	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S3AngelStatue])
	{
		GetObjectByID("memstatue")->SetEnable(false);
	}



	Show(false);
}

void L5S3_Pop1BowVine::OpenPopup(bool withArrow)
{
	Popup::OpenPopup();
	_isArrowDroped = withArrow;
}

void L5S3_Pop1BowVine::Required()
{

}

void L5S3_Pop1BowVine::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->IsDraggingItem())
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (NextSceenRect.Intersection(&pos))
	{
		if (ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed)
		{
			Cursor::SetMode(CA_ExitUp, eArea_L5_S5_MountainBelow);
			if(Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
				_interface->NewArea();
			}
		}
		else
		{
			Cursor::SetMode(CA_Gear);
			if(!Hud->IsDraggingItem() &&
				Control::Input->LBclicked())
			{
				Hud->ShowBannerText(Banner_L5_S3_NoOutCreeper);
			}
		}
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S3AngelStatue] && MemoryRectStatue.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memstatue")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L5S3AngelStatue, &GetObjectByID("memstatue")->Pos);
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S3AngelStatue] && MemoryRectStatue.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memstatue")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L5S3AngelStatue, &(GetObjectByID("memstatue")->Pos));
		}
	}
#endif 
}

void L5S3_Pop1BowVine::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (_janim_ShootArrow && 
		PopupClickArea.Intersection(&dropPos) && 
		itemVal ==  eInv_L5_S1_BowArrowFixed)
	{
		Control::Audio->QuickLoadAndPlaySFX("s3p1_arrowshooting");
		_janim_ShootArrow->PlayAnim();
		isAnimationPlaying = true;

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S1_BowArrowFixed] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S1_BowArrowFixed);
	}
	else
	{
		Popup::HandleInventoryDrop(dropPos, itemVal);
	}
}

void L5S3_Pop1BowVine::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if((!ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed) ||
		(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowSecondTaken))
	{
		Hud->ShowHintForInvItem(eInv_L5_S1_BowArrowFixed, &PopupClickArea);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed)
	{
		Hud->InitHintTrailEmit(&NextSceenRect, true, CA_ExitUp);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L5S3_Pop1BowVine::isTaskLeft()
{
	bool isAnyTaskLeft = ((!ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed) || (!ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowSecondTaken)) ?  true : false;
	return isAnyTaskLeft;
}

bool L5S3_Pop1BowVine::isTaskLeft2()
{
	bool isAnyTaskLeft = isTaskLeft();

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S3AngelStatue])
		isAnyTaskLeft = true;
#endif

	return isAnyTaskLeft;
}

L5S3_Pop1BowVine::~L5S3_Pop1BowVine()
{
	SAFE_DELETE(PoolWaterFallFX);
	SAFE_DELETE(PoolWaterFallBottomFX);
	SAFE_DELETE(_janim_ShootArrow);
}

void L5S3_Pop1BowVine::OnAnimationComplete(int animID)
{
	isAnimationPlaying = false;
	ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed = true;
	CObject* finalVine = GetObjectByID("vine_3");
	finalVine->SetPivot(9, -159);
	finalVine->Animate(0, 0, 2, 0, 4);
	_interface->UpdateHOSystem(this);
}

void L5S3_Pop1BowVine::OnPopupOpened()
{
	PoolWaterFallBottomFX->Start();
	PoolWaterFallBottomFX->AdvanceByTime(2.0f);
	PoolWaterFallFX->Start();
	PoolWaterFallFX->AdvanceByTime(2.0f);

	if (_isArrowDroped)
	{
		Control::Audio->QuickLoadAndPlaySFX("s3p1_arrowshooting");
		_janim_ShootArrow->PlayAnim();
		isAnimationPlaying = true;

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S1_BowArrowFixed] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S1_BowArrowFixed);
	}
}

void L5S3_Pop1BowVine::OnPopupCloseBtnClicked()
{
	PoolWaterFallBottomFX->StopImmediate();
	PoolWaterFallFX->StopImmediate();
}

