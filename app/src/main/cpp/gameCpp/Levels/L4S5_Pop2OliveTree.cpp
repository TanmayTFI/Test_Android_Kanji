//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 05 - POPUP 2: OLIVE TREE
//====================================

#include "L4S5_Pop2OliveTree.h"


L4S5_Pop2OliveTree::L4S5_Pop2OliveTree(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV4\\S5\\POP2\\P2.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("KnifeWithBamboo")->SetEnable(false);

	Janim_OlivePlucking = new JSONAnimator(0,CC_CALLBACK_1(L4S5_Pop2OliveTree::OnAnimComplete,this));
	Janim_OlivePlucking->parseAndLoadJSON("ANIM\\L4\\S5_OLIVEPLUCKING.JSON", this);

	GetObjectByID("Olive")->SetPivot(0,-145);
	GetObjectByID("Olive")->Animate(0,0,1,0,4,2);
	GetObjectByID("LeafiNTop")->SetPivot(38,-121);
	GetObjectByID("LeafiNTop")->Animate(0,0,-1.5,0,8,2);

	SetSparkleUpdate(false);
	Show(false);

}

void L4S5_Pop2OliveTree::Required()
{

}

void L4S5_Pop2OliveTree::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}


void L4S5_Pop2OliveTree::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && GetObjectByID("Olive")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L2high");
		}
	}

}

void L4S5_Pop2OliveTree::HandleInventoryDrop(CPoint dropPos, int itemVal)
{

	switch (itemVal)
	{
	case eInv_L4_CB_BambooWithKnife:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected)
		{
			Janim_OlivePlucking->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p2_oliveTake");
			
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			if (ProfileSystem::Instance->ActiveProfile.L4_S1_TwigsMoved && ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken && ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved && ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked && ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_CB_BambooWithKnife); 
			}
		}
		break;
	default:

		GFInstance->WrongInvDropMsg(Hud);
		break;
	}

}

void L4S5_Pop2OliveTree::OnAnimComplete(int animID)
{
	GetObjectByID("Olive")->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected = true;
	Hud->TakeInventoryItem(eInv_L4_S5_Olives);
	ClosePopup(1);
}

float L4S5_Pop2OliveTree::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S5_Pop2OliveTree::~L4S5_Pop2OliveTree()
{
	SAFE_DELETE(Janim_OlivePlucking);
}

bool L4S5_Pop2OliveTree::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] && !ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected )
	{
		isAnyTaskLeft = true;
	}
	return isAnyTaskLeft;
}

void L4S5_Pop2OliveTree::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] && !ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected )
	{
		//Hud->ShowHintForInvItem(eInv_L4_CB_BambooWithKnife, &GetObjectByID("Olive")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle oliveObjRect = GetObjectByID("Olive")->GetWorldRect();
		Hud->ShowHintForInvItem(eInv_L4_CB_BambooWithKnife, &oliveObjRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
