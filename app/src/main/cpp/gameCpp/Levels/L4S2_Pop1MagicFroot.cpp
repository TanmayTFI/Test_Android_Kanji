//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 02 - POPUP 1: MAGIC FROOT
//====================================

#include "L4S2_Pop1MagicFroot.h"


L4S2_Pop1MagicFroot::L4S2_Pop1MagicFroot(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	_jsonAnimFruitCut = NULL;
	_bambooSpearDropPolygon = NULL;

	//Popup load
	Load("LV4\\S2\\POP1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	_jsonAnimFruitCut = new JSONAnimator(1, CC_CALLBACK_1(L4S2_Pop1MagicFroot::OnFruitCutAnimComplete, this));
	_jsonAnimFruitCut->parseAndLoadJSON("ANIM\\L4\\L4_S2_POP1_FRUITANIM.JSON", this);
	_jsonAnimFruitCut->PlayAnim();
	_jsonAnimFruitCut->StopAnim();
	//_jsonAnimFruitCut->SetAnimTime(0);
	GetObjectByID("Dagger")->SetEnable(false);
	int ArrayPts[] = {367,95,373,568,1006,570,999,94};
	_bambooSpearDropPolygon =  new ClickablePolygon(0, 4, ArrayPts, CC_CALLBACK_1(L4S2_Pop1MagicFroot::OnFruitAreaClick, this));


	SetSparkleUpdate(false);
	Show(false);
	_isMouseInteractionEnabled = true;



}

void L4S2_Pop1MagicFroot::OnFruitAreaClick(int id)
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
	if (!Hud->IsDraggingItem())
	{
		Hud->ShowBannerText("L4exhausted");
	}
}

void L4S2_Pop1MagicFroot::Required()
{

}

void L4S2_Pop1MagicFroot::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}


void L4S2_Pop1MagicFroot::Update(float ds)
{

	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	CPoint pos = Control::Input->Mpos();
	if (_bambooSpearDropPolygon->Intersection(&pos) && _isMouseInteractionEnabled)
	{
		if (Control::Input->LBclicked() && !Hud->IsDraggingItem())
		{
			Hud->ShowBannerText("L4cant");
		}
		Cursor::SetMode(CA_Gear);
	}
	else if(!isMouseOverCloseButton)
	{
		Cursor::SetMode(CA_Normal);
	}


}

float L4S2_Pop1MagicFroot::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S2_Pop1MagicFroot::~L4S2_Pop1MagicFroot()
{
	SAFE_DELETE(_bambooSpearDropPolygon);
	SAFE_DELETE(_jsonAnimFruitCut);
}

bool L4S2_Pop1MagicFroot::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] )
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S2_Pop1MagicFroot::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if( !ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] )
	{
		Hud->ShowHintForInvItem(eInv_L4_CB_BambooWithKnife, &GetObjectByID("FruitWithRoot1")->Pos);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
void L4S2_Pop1MagicFroot::HandleInventoryDrop(CPoint dropPos, int itemVal)
{

	if (itemVal == eInv_L4_CB_BambooWithKnife)
	{
		if (_bambooSpearDropPolygon->Intersection(&dropPos))
		{
			GetObjectByID("Dagger")->SetEnable(true);
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			if (ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved && ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved && ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_CB_BambooWithKnife); 
			}

			_jsonAnimFruitCut->SetEnableObjects(true);
			_jsonAnimFruitCut->PlayAnim(false);
			_isMouseInteractionEnabled = false;
			Control::Audio->QuickLoadAndPlaySFX("s2p1_fruitPluck");


		}
		else
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if (itemVal == eInv_L4_S1_Knife)
	{
		Hud->ShowBannerText("L4something");
		Control::Input->ClearMouse();
		Hud->CancelDrag();	
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}

}

void L4S2_Pop1MagicFroot::OnFruitCutAnimComplete(int id)
{
	ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked = true;
	_scene->GetObjectByID("FruitUnderTree1")->SetEnable(true);
	_scene->GetObjectByID("FriutOnTree03")->SetEnable(false);
	_clickAreaToOpenPopup->SetIsInteractionEnabled(false);
	ClosePopup(1.5);
}
