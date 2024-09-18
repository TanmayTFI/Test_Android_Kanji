//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 02 - POPUP 3: TREE BOTTOM
//====================================

#include "L4S2_Pop3TreeBottom.h"


L4S2_Pop3TreeBottom::L4S2_Pop3TreeBottom(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV4\\S2\\POP3\\P3.SCN");
	_twig = GetObjectByID("WoodenBranch");
	_fruit = GetObjectByID("Fruit");
	_twig->SetMouseClickCallback(CC_CALLBACK_1(L4S2_Pop3TreeBottom::OnObjectClick, this));
	_fruit->SetMouseClickCallback(CC_CALLBACK_1(L4S2_Pop3TreeBottom::OnObjectClick, this));
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);


	SetSparkleUpdate(false);
	Show(false);

	_jsonAnimFruitEat = new JSONAnimator(1, CC_CALLBACK_1(L4S2_Pop3TreeBottom::OnFruitEatAnimComplete, this));
	_jsonAnimFruitEat->parseAndLoadJSON("ANIM\\L4\\L4_S2_POP3_FRUITEAT.JSON", this);
	_jsonAnimFruitEat->StopAnim();
	_fruitEnergyFX = new SuperFX("PRT\\L4\\S2FruitEnergy.INI", PointSystem::CreateCPoint(666,480,0), 1100); 
	_fruitEnergyFX->AdvanceByTime(2.0f);

	InitializeWithProfileData();



}

void L4S2_Pop3TreeBottom::Required()
{

}

void L4S2_Pop3TreeBottom::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

void L4S2_Pop3TreeBottom::OpenPopup()
{
	InitializeWithProfileData();
	Popup::OpenPopup();
}


void L4S2_Pop3TreeBottom::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}

	bool isCursorHandPick = false;

	if (_twig->GetCurrentMouseState() == eMouseState_Over || _twig->GetCurrentMouseState() == eMouseState_Down)
	{
		isCursorHandPick = true;
	}
	if (_fruit->GetCurrentMouseState() == eMouseState_Over || _fruit->GetCurrentMouseState() == eMouseState_Down)
	{
		isCursorHandPick = true;
	}


	if (isCursorHandPick)
	{
		Cursor::SetMode(CA_HandTake);
	}
	else if(!isMouseOverCloseButton)
	{
		Cursor::SetMode(CA_Normal);
	}
}

float L4S2_Pop3TreeBottom::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S2_Pop3TreeBottom::~L4S2_Pop3TreeBottom()
{
	SAFE_DELETE(_fruitEnergyFX);
}

bool L4S2_Pop3TreeBottom::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken)
	{
		isAnyTaskLeft = true;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken && ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S2_Pop3TreeBottom::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken)
	{
		Hud->InitHintTrailEmit(_twig->GetCenterX(), _twig->GetCenterY());
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken && ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked)
	{
		Hud->InitHintTrailEmit(_fruit->GetCenterX(), _fruit->GetCenterY());
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}

void L4S2_Pop3TreeBottom::InitializeWithProfileData()
{
	_twig->SetEnable(true);
	GetObjectByID("BranchShadow")->SetEnable(true);
	_twig->SetIsInteractionEnabled(true);

	_fruit->SetEnable(true);
	_fruit->SetIsInteractionEnabled(true);

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken)
	{
		_twig->SetEnable(false);
		GetObjectByID("BranchShadow")->SetEnable(false);
		_twig->SetIsInteractionEnabled(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken || !ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked)
	{
		_fruit->SetEnable(false);
		_fruit->SetIsInteractionEnabled(false);
	}
}

void L4S2_Pop3TreeBottom::OnObjectClick(CObject * object)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (Hud->IsDraggingItem())
	{
		return;
	}
	if(GetPopupState()!= ePopupState_Open )
		return;
	object->SetIsInteractionEnabled(false);

	if (object == _twig)
	{
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken = true;
		_twig->SetEnable(false);
		GetObjectByID("BranchShadow")->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L4_S2_Twig1, object->Pos);

		if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken 
			&&ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken
			)
		{
			_clickAreaToOpenPopup->SetIsInteractionEnabled(false);
			ClosePopup(1.5);
		}
	}
	if (object == _fruit)
	{
		ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken = true;
		_jsonAnimFruitEat->PlayAnim();
		
		Control::Audio->QuickLoadAndPlaySFX("s2p3_fruitEating");
	}


}


void L4S2_Pop3TreeBottom::OnFruitEatAnimComplete(int id)
{
	Hud->ShowBannerText("L4strength");
	_scene->GetObjectByID("FruitUnderTree1")->SetEnable(false);
	_clickAreaToOpenPopup->SetIsInteractionEnabled(false);
	_fruitEnergyFX->Start();
	if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken 
			&&ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken)
	{
		ClosePopup(4.5);
	}
	
}
