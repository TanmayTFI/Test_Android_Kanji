
//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L3S4_Pop2MapWallThree.h"
#include "Hud.h"

const CRectangle  ActionRectStoneLeaves(611.0f,286.0f,174.0f,141.0f);
const CRectangle  ActionRectGroundBrick(555.0f,229.0f,321.0f,229.0f);

const CRectangle  BtnRectLeftButton(267.0f,263.0f,83.0f,108.0f);
const CRectangle  BtnRectRightButton(1008.0f,260.0f,95.0f,113.0f);

L3S4_Pop2MapWallThree::L3S4_Pop2MapWallThree(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S4\\POP2C\\P2C.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CLeavesRemoved)
	{
		GetObjectByID("leaf1")->SetEnable(false);
		GetObjectByID("leaf2")->SetEnable(false);
		GetObjectByID("leaf4")->SetEnable(false);
		GetObjectByID("leaf5")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CStoneRemoved)
	{
		GetObjectByID("stone")->SetEnable(false);
	}

	GetObjectByID("groundglow")->Animate(0.45f, 0.85f, 2.0f);
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed)
	{
		GetObjectByID("bricknormal")->SetEnable(false);
	}
	else
	{
		GetObjectByID("groundglow")->SetEnable(false);
	}

	//UI sliding handling
	LoadNextPopup = nullptr;

	UINode0 = GetObjectByID("uinode0");
	UINode1 = GetObjectByID("uinode1");
	UINode2 = GetObjectByID("uinode2");
	UINode3 = GetObjectByID("uinode3");

	UINodeGlow = GetObjectByID("uinodeglow");
	
	UIBtnPrev = GetObjectByID("uileftbtn");
	UIBtnNext = GetObjectByID("uirightbtn");

	UINode0->SetEnable(false);
	UINode1->SetEnable(false);
	UINode2->SetEnable(false);
	UINode3->SetEnable(false);

	UIBtnPrev->SetEnable(false);
	UIBtnNext->SetEnable(false);

	UINodeGlow->SetEnable(false);

	maxVelocity = 5;

	maxDisplacement = 400.0f;
	slideDisplacement = 0;
	slideTimer = 0;

	isSliding = false;
	isFadingIn = false;
}

void L3S4_Pop2MapWallThree::OnPopupOpened()
{
	SetupAndShowUI();
}

void L3S4_Pop2MapWallThree::OnPopupCloseBtnClicked()
{
	HideUI();
}

//UI sliding handling
void L3S4_Pop2MapWallThree::SetupAndShowUI()
{
	UINode0->SetEnable(true);
	UINode1->SetEnable(true);
	UINode2->SetEnable(true);
	UINode3->SetEnable(true);

	UINodeGlow->SetEnable(true);

	UIBtnPrev->SetEnable(true);
	UIBtnNext->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ASnakeButtonPressed)
		UINode0->PlayAnimation(1);
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2BSnakeButtonPressed)
		UINode1->PlayAnimation(1);
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed)
		UINode2->PlayAnimation(1);
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2DSnakeButtonPressed)
		UINode3->PlayAnimation(1);

	UINode0->FadeIn(4.0f);
	UINode1->FadeIn(4.0f);
	UINode2->FadeIn(4.0f);
	UINode3->FadeIn(4.0f);

	UINodeGlow->FadeIn(4.0f);
		
	UIBtnPrev->FadeIn(4.0f);
	UIBtnNext->FadeIn(4.0f);
}

void L3S4_Pop2MapWallThree::HideUI()
{
	UINode0->SetEnable(false);
	UINode1->SetEnable(false);
	UINode2->SetEnable(false);
	UINode3->SetEnable(false);

	UINodeGlow->SetEnable(false);

	UIBtnPrev->SetEnable(false);
	UIBtnNext->SetEnable(false);
}

void L3S4_Pop2MapWallThree::StartSceneSlideIn(bool fromLeft)
{
	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CLeavesRemoved)
	{
		GetObjectByID("leaf1")->SetEnable(false);
		GetObjectByID("leaf2")->SetEnable(false);
		GetObjectByID("leaf4")->SetEnable(false);
		GetObjectByID("leaf5")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CStoneRemoved)
	{
		GetObjectByID("stone")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed)
	{
		GetObjectByID("bricknormal")->SetEnable(false);
	}

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

void L3S4_Pop2MapWallThree::StartSceneSlideOut(bool toLeft)
{
	slideTimer = 0;

	HideUI();

	SaveXY();
	isFadingIn = false;
	isSliding = true;
	maxDisplacement = abs(maxDisplacement); //positive

	if(toLeft)
		maxDisplacement = -maxDisplacement; //shift scene to left

	SetPopupState(ePopupState_Closed);
}

void L3S4_Pop2MapWallThree::resetScene()
{
	RestoreXY();
	_popupAlpha = 1.0f;
}

//Popup ID and LoadPrevious if true, next if false
void L3S4_Pop2MapWallThree::setLoadNextCallback(std::function<void (Popup*, bool)> _loadNextPopupCB)
{
	LoadNextPopup = _loadNextPopupCB;
}
//UI sliding handling

void L3S4_Pop2MapWallThree::Required()
{

}

void L3S4_Pop2MapWallThree::Update(float ds)
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

	//handle interactions here

	UIBtnNext->PlayAnimation(0);
	UIBtnPrev->PlayAnimation(0);
	if(BtnRectLeftButton.Intersection(&pos))
	{		
		UIBtnPrev->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			UIBtnPrev->PlayAnimation(0);
			StartSceneSlideOut(false);
			//Load Next One also
			if(LoadNextPopup!=nullptr)
				LoadNextPopup(this, true); //load Map Wall 2
		}
	}
	else if(BtnRectRightButton.Intersection(&pos))
	{
		UIBtnNext->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			StartSceneSlideOut(true);
			//Load Next One also
			if(LoadNextPopup!=nullptr)
				LoadNextPopup(this, false); //load Map Wall 4
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CStoneRemoved)
	{
		if(ActionRectGroundBrick.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s4p2_map_rockmove");
				GetObjectByID("stone")->TweenPosTo(80, -30, -1, 1.0f, 0, true);
				ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CStoneRemoved = true;
			}
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CLeavesRemoved)
	{
		if(ActionRectGroundBrick.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s4p2_map_leafmove");

				GetObjectByID("leaf1")->TweenPosTo(-70, -20, -1, 1.0f, 0, true);
				GetObjectByID("leaf2")->TweenPosTo(70, -20, -1, 1.0f, 0, true);
				GetObjectByID("leaf4")->TweenPosTo(70, 20, -1, 1.0f, 0, true);
				GetObjectByID("leaf5")->TweenPosTo(-70, 40, -1, 1.0f, 0, true);

				ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CLeavesRemoved = true;
			}
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed)
	{
		if(ActionRectGroundBrick.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s4p2_map_tilepress");

				ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed = true;
				GetObjectByID("bricknormal")->FadeOut(2.0f);
				GetObjectByID("groundglow")->SetEnable(true);
				UINode2->PlayAnimation(1);
			}
			return;
		}
	}
}

void L3S4_Pop2MapWallThree::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L3S4_Pop2MapWallThree::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CLeavesRemoved)
	{
		Hud->InitHintTrailEmit(&ActionRectStoneLeaves);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CStoneRemoved)
	{
		Hud->InitHintTrailEmit(&ActionRectStoneLeaves);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2CSnakeButtonPressed)
	{
		Hud->InitHintTrailEmit(&ActionRectGroundBrick);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2DSnakeButtonPressed)
	{
		Hud->InitHintTrailEmit(&BtnRectRightButton, true);
	}
	else
	{
		//Shows Previous Button
		Hud->InitHintTrailEmit(&BtnRectLeftButton, true);
	}
}

bool L3S4_Pop2MapWallThree::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	return isAnyTaskLeft;
}

L3S4_Pop2MapWallThree::~L3S4_Pop2MapWallThree()
{
}
