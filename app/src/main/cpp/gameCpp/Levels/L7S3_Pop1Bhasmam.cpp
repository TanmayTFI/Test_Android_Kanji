//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7S3_Pop1Bhasmam.h"
#include "Hud.h"

const CRectangle  BroomDropRect(593.0f,110.0f,249.0f,170.0f);

const CRectangle  InvDestRectBhasmam(425.0f,126.0f,529.0f,322.0f);
const CRectangle  InvItemRectBhasmam(437.0f,309.0f,279.0f,245.0f);

const CRectangle  BtnRectUpButton(1009.0f,269.0f,90.0f,110.0f);//up is right
const CRectangle  BtnRectDownButton(272.0f,273.0f,89.0f,117.0f);//down is left

L7S3_Pop1Bhasmam::L7S3_Pop1Bhasmam(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV7\\S3\\POP1\\P1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	clickDelayTimer = -1;

	std::vector<SuperFX*> superfxArray;
	dustFX = new SuperFX("PRT\\L7\\s3basmamdust.INI", PointSystem::CreateCPoint(488,106,0), zOrder+10); 
	superfxArray.push_back(dustFX);

	JAnim_BroomCollect = new JSONAnimator(0, CC_CALLBACK_1(L7S3_Pop1Bhasmam::OnAnimationComplete, this));
	JAnim_BroomCollect->parseAndLoadJSON("ANIM\\L7\\S3BHASMAMCOLLECT.JSON", this, &superfxArray);

	GetObjectByID("BrushLarge")->SetEnable(false);
	GetObjectByID("broomshadow")->SetEnable(false);

	GetObjectByID("btntoup")->SetRotate(-90);
	GetObjectByID("btntodown")->SetRotate(-90);
	
	if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken)
	{
		GetObjectByID("powder3")->SetEnable(false);
		GetObjectByID("powder2")->SetEnable(false);
		GetObjectByID("powder1")->SetEnable(false);

		GetObjectByID("diwalilamp")->SetEnable(false);
		GetObjectByID("bhasmam")->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam)
	{
		GetObjectByID("powder3")->SetEnable(false);
		GetObjectByID("powder2")->SetEnable(false);
		GetObjectByID("powder1")->SetEnable(false);		
	}
	else //broom used - bhasmam won't be in scene!!
		GetObjectByID("bhasmam")->SetEnable(false);
	

	//UI sliding handling
	LoadNextPopup = nullptr;
	
	UIBtnDown = GetObjectByID("btntodown");
	UIBtnUp = GetObjectByID("btntoup");

	UIBtnDown->SetEnable(false);
	UIBtnUp->SetEnable(false);

	maxVelocity = 5;

	maxDisplacement = 400.0f;
	slideDisplacement = 0;
	slideTimer = 0;

	isSliding = false;
	isFadingIn = false;
}

void L7S3_Pop1Bhasmam::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//Bhasmam Collect
		isAnimationPlaying = false;

		GetObjectByID("BrushLarge")->SetEnable(false);
		GetObjectByID("broomshadow")->SetEnable(false);
		GetObjectByID("bhasmam")->SetEnable(false);
	}
}

void L7S3_Pop1Bhasmam::OnPopupOpened()
{
	SetupAndShowUI();
}

void L7S3_Pop1Bhasmam::OnPopupCloseBtnClicked()
{
	HideUI();
}

//UI sliding handling
void L7S3_Pop1Bhasmam::SetupAndShowUI()
{
	//UIBtnDown->SetEnable(true);
	UIBtnUp->SetEnable(true);
		
	//UIBtnDown->FadeIn(4.0f);
	UIBtnUp->FadeIn(4.0f);
}

void L7S3_Pop1Bhasmam::HideUI()
{
	UIBtnDown->SetEnable(false);
	UIBtnUp->SetEnable(false);
}

void L7S3_Pop1Bhasmam::StartSceneSlideIn(bool fromLeft)
{
	SetPopupState(ePopupState_Open);

	slideTimer = 0;

	SaveXY();
	isFadingIn = true;
	isSliding = true;
	SetSceneAlpha(0);

	maxDisplacement = abs(maxDisplacement); //positive
	if(fromLeft)
		maxDisplacement = -maxDisplacement;

	ShiftXY(maxDisplacement,0);
}

void L7S3_Pop1Bhasmam::StartSceneSlideOut(bool toLeft)
{
	slideTimer = 0;

	HideUI();

	SaveXY();
	isFadingIn = false;
	isSliding = true;
	maxDisplacement = abs(maxDisplacement); //positive

	if(toLeft)
		maxDisplacement = -maxDisplacement; //shift scene to left

	//SetPopupState(ePopupState_Closed);
}

void L7S3_Pop1Bhasmam::resetScene()
{
	RestoreXY();
	SetSceneAlpha(1.0f);
	if(isFadingIn)
	{
		_popupAlpha = 1.0f;
	}
	else
	{
		_popupTransitionTimer = 0;
		_popupAlpha = 0;
	}
}

//Popup ID and LoadPrevious if true, next if false
void L7S3_Pop1Bhasmam::setLoadNextCallback(std::function<void (Popup*, bool)> _loadNextPopupCB)
{
	LoadNextPopup = _loadNextPopupCB;
}
//UI sliding handling

void L7S3_Pop1Bhasmam::Required()
{

}

void L7S3_Pop1Bhasmam::Update(float ds)
{
	Popup::Update(ds);
	if(GFInstance->IsActivePopup)
		return;

	if(isSliding)
	{
		slideTimer += Control::LogicRate*maxVelocity;
		if(!isFadingIn)
		{
			SetSceneAlpha(1.0f-slideTimer);
			ShiftXY(slideTimer*maxDisplacement,0);
		}
		else
		{
			SetSceneAlpha(slideTimer);
			ShiftXY(maxDisplacement - slideTimer*maxDisplacement,0);
		}

		if(slideTimer>1.0f)
		{
			//SLOG::Log("End of Pop wall 2 trans");
			//end of slide
			isSliding = false;

			if(isFadingIn)
				SetupAndShowUI();
			else
				SetPopupState(ePopupState_Closed);

			resetScene();
		}
		return;
	}

	if(GetPopupState()!= ePopupState_Open || Hud->IsDraggingItem())
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

	UIBtnUp->PlayAnimation(0);
	UIBtnDown->PlayAnimation(0);

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam)
	{
		if(InvDestRectBhasmam.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L7only");
			}
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken)
	{
		if(InvItemRectBhasmam.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("powder3")->SetEnable(false);
				GetObjectByID("powder2")->SetEnable(false);
				GetObjectByID("powder1")->SetEnable(false);

				GetObjectByID("diwalilamp")->SetEnable(false);

				ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken = true;
				Hud->TakeInventoryItem(eInv_L7_S3_Bhasmam);
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] = true;

				if( ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken )
					ClosePopup(1.5f);
			}
		}
	}

	if(BtnRectUpButton.Intersection(&pos))
	{
		UIBtnUp->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			StartSceneSlideOut(true);
			//Load Next One also
			if(LoadNextPopup!=nullptr)
				LoadNextPopup(this, false); //load Map Wall 3
		}
	}


	/*if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BStoneRemoved)
	{
		if(ActionRectGroundBrick.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("stone")->TweenPosTo(-50, -20, -1, 1.0f, 0, true);
				ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BStoneRemoved = true;
			}
		}
	}*/
	
}

void L7S3_Pop1Bhasmam::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	
	if (itemVal == eInv_L7_S3_Broom && !ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam && InvDestRectBhasmam.Intersection(&dropPos))
	{
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		{
			isAnimationPlaying = true;
			ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam = true;

			Control::Audio->QuickLoadAndPlaySFX("s3p1_broomused");

			JAnim_BroomCollect->PlayAnim();

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S3_Broom] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S3_Broom);
		}
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}	
}

void L7S3_Pop1Bhasmam::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam && ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
	{
		Hud->ShowHintForInvItem(eInv_L7_S3_Broom, &BroomDropRect);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectBhasmam);
	}
	else
	{
		bool isTopPopupTaskLeft = false;
		//Chain popup
		if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
			isTopPopupTaskLeft = true;
		else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvLeverTaken)
			isTopPopupTaskLeft = true;
		else if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken)
			isTopPopupTaskLeft = true;

		//Bull popup
		if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
			isTopPopupTaskLeft = true;
		else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[0])
			isTopPopupTaskLeft = true;
		else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
			isTopPopupTaskLeft = true;
		else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
			isTopPopupTaskLeft = true;

		if(isTopPopupTaskLeft)
		{
			//Shows Up Button
			Hud->InitHintTrailEmit(&BtnRectUpButton, true);
			return;
		}

		//Shows Close Button
		Popup::ProcessHint();
	}
	
}

bool L7S3_Pop1Bhasmam::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam && ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
	{
		isAnyTaskLeft = true;
	}
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L7S3_Pop1Bhasmam::~L7S3_Pop1Bhasmam()
{
	SAFE_DELETE(dustFX);
	SAFE_DELETE(JAnim_BroomCollect);
}
