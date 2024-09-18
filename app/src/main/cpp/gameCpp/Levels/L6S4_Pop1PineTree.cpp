//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 04 - POPUP 1: 
//====================================

#include "L6S4_Pop1PineTree.h"
#include "Util.h"
const CRectangle  PinetreeRect(694.0f,91.0f,310.0f,352.0f);

const CRectangle  InvDestMortarRect(484.0f,129.0f,331.0f,380.0f);

L6S4_Pop1PineTree::L6S4_Pop1PineTree(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S4\\P1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeTaken)
	{
		GetObjectByID("Rope")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvStickTaken)
	{
		GetObjectByID("Stick")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced)
	{
		GetObjectByID("Pan")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected)
	{
		GetObjectByID("Pan")->SetEnable(false);
		for (int i = 1; i < 4; i++)
		{
			string _id = "FilledPan0" + MKSTR(i);
			GetObjectByID(_id)->SetEnable(false);
			_id = "PineSap0" + MKSTR(i);
			GetObjectByID(_id)->SetEnable(false);
		}
	}
	GetObjectByID("Stick_vert")->SetEnable(false);
	GetObjectByID("Pan")->SetZ(GetObjectByID("Pan")->ZOrder + 200);
	GetObjectByID("Stick_vert")->SetZ(GetObjectByID("Stick_vert")->ZOrder + 200);
	GetObjectByID("Stick")->SetZ(GetObjectByID("Stick")->ZOrder + 200);

	for (int i = 1; i < 4; i++)
	{
		string _id = "FilledPan0" + MKSTR(i);
		GetObjectByID(_id)->SetEnable(false);
		GetObjectByID(_id)->SetZ(GetObjectByID(_id)->ZOrder + 200);
		_id = "PineSap0" + MKSTR(i);
		GetObjectByID(_id)->SetZ(GetObjectByID(_id)->ZOrder + 200);
	}

	Janim_stickCut = new JSONAnimator(0, CC_CALLBACK_1(L6S4_Pop1PineTree::OnAnimComplete,this));
	Janim_stickCut->parseAndLoadJSON("ANIM\\L6\\S4_CUT_STICK.JSON",this);

	Janim_removeGum = new JSONAnimator(1, CC_CALLBACK_1(L6S4_Pop1PineTree::OnAnimComplete,this));
	Janim_removeGum->parseAndLoadJSON("ANIM\\L6\\S4_COLLECT_GUM.JSON",this);

	int ArrayPts[] = {534,360,593,335,651,266,668,277,630,324,647,316,662,335,586,371,560,379,526,415,485,447,420,488,410,490,403,468,447,441,500,405,531,361};
	stickPoly =  new CPolygon(17);
	stickPoly->Init(ArrayPts);

	isCollectingInventory = false;

	SetSparkleUpdate(false);
	Show(false);
}


void L6S4_Pop1PineTree::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(isAnimationPlaying)
		return;

	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvStickTaken && stickPoly->Intersection(&pos) && !isCollectingInventory)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			Janim_stickCut->PlayAnim();
			isAnimationPlaying = true;
			isCollectingInventory = true;
			ProfileSystem::Instance->ActiveProfile.L6_S4_InvStickTaken = true;
		}
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeTaken && GetObjectByID("Rope")->GetWorldRect().Intersection(&pos) && !isCollectingInventory)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("Rope")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeTaken = true;
			Hud->TakeInventoryItem(eInv_L6_S4_Rope);
			isCollectingInventory = true;

			if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected)
			{
				ClosePopup(1);
			}
		}
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected && InvDestMortarRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced)
			{
				Hud->ShowBannerText("L6pan"); 
			}
			else
			{
				Hud->ShowBannerText("L6small");
			}
		}
	}
	else if (PinetreeRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6hard");
		}
	}
}

void L6S4_Pop1PineTree::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L6_S1_MetalContainer:
		if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p1_placePan");
			GetObjectByID("Pan")->SetEnable(true);
			ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_MetalContainer] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S1_MetalContainer);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L6_S1_Mortar:
		if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced)
		{
			Hud->ShowBannerText("L6bigger");
			Hud->CancelDrag();
		}
		break;
	case eInv_L6_S4_Stick:
		if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected && ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p1_collectPinePitch");
			GetObjectByID("Stick_vert")->SetEnable(true);
			for (int i = 1; i < 4; i++)
			{
				string _id = "FilledPan0" + MKSTR(i);
				GetObjectByID(_id)->SetEnable(true);
			}
			Janim_removeGum->PlayAnim();
			isAnimationPlaying = true;
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_Stick] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_Stick);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced)
		{
			Hud->ShowBannerText("L6sort");
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

void L6S4_Pop1PineTree::OnAnimComplete(int animID)
{
	isAnimationPlaying = false;
	switch (animID)
	{
	case 0:
		GetObjectByID("Stick")->SetEnable(false);

		Hud->TakeInventoryItem(eInv_L6_S4_Stick);
		break;
	case 1:
		GetObjectByID("Stick_vert")->SetEnable(false);
		GetObjectByID("Pan")->SetEnable(false);
		for (int i = 1; i < 4; i++)
		{
			string _id = "FilledPan0" + MKSTR(i);
			GetObjectByID(_id)->SetEnable(false);
		}
		ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected = true;
		Hud->TakeInventoryItem(eInv_L6_S4_PinePitch);
		if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeTaken)
		{
			ClosePopup(1);
		}
		break;
	default:
		break;
	}
}

float L6S4_Pop1PineTree::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S4_Pop1PineTree::~L6S4_Pop1PineTree()
{
	SAFE_DELETE(Janim_removeGum);
	SAFE_DELETE(Janim_stickCut);
	SAFE_DELETE(stickPoly);
}

void L6S4_Pop1PineTree::Required()
{

}

void L6S4_Pop1PineTree::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

void L6S4_Pop1PineTree::ReceiveMessage(int val)
{	

	if( val == INV_CLOSE )
	{		
		/*if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected)
		{
		ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected = true;
		Hud->TakeInventoryItem(eInv_L6_S4_Stick);
		}*/
		isCollectingInventory = false;
	}	
}

bool L6S4_Pop1PineTree::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeTaken ||
		!ProfileSystem::Instance->ActiveProfile.L6_S4_InvStickTaken||
		(ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced) ||
		(ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced && ProfileSystem::Instance->ActiveProfile.L6_S4_InvStickTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L6S4_Pop1PineTree::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("Rope")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ropeObjRect =GetObjectByID("Rope")->GetWorldRect();
		Hud->InitHintTrailEmit(&ropeObjRect);
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvStickTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("DryBranch")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle dryBranchObjRect =GetObjectByID("DryBranch")->GetWorldRect();
		Hud->InitHintTrailEmit(&dryBranchObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced)
	{
		Hud->ShowHintForInvItem(eInv_L6_S1_MetalContainer,&InvDestMortarRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced && ProfileSystem::Instance->ActiveProfile.L6_S4_InvStickTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumCollected)
	{
		Hud->ShowHintForInvItem(eInv_L6_S4_Stick,&InvDestMortarRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
