//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 02 - POPUP 4: GRASS
//====================================

#include "L4S2_Pop4Grass.h"


L4S2_Pop4Grass::L4S2_Pop4Grass(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	_jsonAnimGrassCut = NULL;
	_grass = NULL;
	_isMouseInteractionEnabled = true;
	//Popup load
	Load("LV4\\S2\\POP4\\P4.SCN");
	_grass = GetObjectByID("GrassCutTop");

	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	_jsonAnimGrassCut = new JSONAnimator(1, CC_CALLBACK_1(L4S2_Pop4Grass::OnGrassCutAnimComplete, this));
	_jsonAnimGrassCut->parseAndLoadJSON("ANIM\\L4\\L4_S2_POP4_GRASSCUT.JSON", this);
	_jsonAnimGrassCut->PlayAnim();
	_jsonAnimGrassCut->StopAnim();
	_jsonAnimGrassCut->SetAnimTime(0);
	GetObjectByID("Knife")->SetEnable(false);

	int ArrayPts[] = {364,194,473,333,575,188,772,111,993,158,1001,565,364,570};
	_knifeDropPolygon =  new CPolygon(7);
	_knifeDropPolygon->Init(ArrayPts);

	SetSparkleUpdate(false);
	Show(false);
}

void L4S2_Pop4Grass::Required()
{

}

void L4S2_Pop4Grass::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}


void L4S2_Pop4Grass::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!Hud->IsDraggingItem()  && _knifeDropPolygon->Intersection(&pos) && _isMouseInteractionEnabled)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4grass");
		}
	}
	else if(!isMouseOverCloseButton)
	{
		Cursor::SetMode(CA_Normal);
	}


}

float L4S2_Pop4Grass::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S2_Pop4Grass::~L4S2_Pop4Grass()
{
	SAFE_DELETE(_knifeDropPolygon);
	SAFE_DELETE(_jsonAnimGrassCut);
}

bool L4S2_Pop4Grass::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken )
	{
		if( ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife] || ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife])
		{
			isAnyTaskLeft = true;
		}
	}
	return isAnyTaskLeft;
}

void L4S2_Pop4Grass::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if( !ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken )
	{
		if( ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife] )
			Hud->ShowHintForInvItem(eInv_L4_S1_Knife, &_knifeDropPolygon->center);
		else if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] )
			Hud->ShowHintForInvItem(eInv_L4_CB_BambooWithKnife, &_knifeDropPolygon->center);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}

void L4S2_Pop4Grass::HandleInventoryDrop(CPoint dropPos, int itemVal)
{

	if (itemVal == eInv_L4_S1_Knife || itemVal == eInv_L4_CB_BambooWithKnife)
	{
		if (_knifeDropPolygon->Intersection(&dropPos))
		{
			GetObjectByID("Knife")->SetEnable(true);
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			_jsonAnimGrassCut->SetEnableObjects(true);
			_jsonAnimGrassCut->PlayAnim(false);
			_isMouseInteractionEnabled = false;
			Control::Audio->QuickLoadAndPlaySFX("s2p4_grassCut");

			if (itemVal == eInv_L4_CB_BambooWithKnife)
			{
				if (ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved && ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked && ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved)
				{
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_CB_BambooWithKnife); 
				}
			}
		}
		else
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}

}

void L4S2_Pop4Grass::OnGrassCutAnimComplete(int id)
{
	ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken = true;
	_grass->SetEnable(false);
	Hud->TakeInventoryItem(eInv_L4_S2_GrassBunch, _grass->Pos);
	_clickAreaToOpenPopup->SetIsInteractionEnabled(false);
	ClosePopup(1.5);
}
