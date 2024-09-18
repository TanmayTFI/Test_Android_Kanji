//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 04 - POPUP 1: Woodcut
//====================================

#include "L2S4_Pop4StoneBrick.h"
#include "Hud.h"

const CRectangle  ActionRectMoveStone(465.0f,192.0f,441.0f,289.0f);

L2S4_Pop4StoneBrick::L2S4_Pop4StoneBrick(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV2\\S4\\POP4\\P4.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);

	JAnim_StoneMove1 = new JSONAnimator(0, CC_CALLBACK_1(L2S4_Pop4StoneBrick::OnAnimationComplete, this));
	JAnim_StoneMove1->parseAndLoadJSON("ANIM\\L2\\S4BRICKMOVE1.JSON", this);

	JAnim_StoneMove2 = new JSONAnimator(1, CC_CALLBACK_1(L2S4_Pop4StoneBrick::OnAnimationComplete, this));
	JAnim_StoneMove2->parseAndLoadJSON("ANIM\\L2\\S4BRICKMOVE2.JSON", this);
	
	if(ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabMoved1)
	{
		GetObjectByID("p4stone")->SetPos(PointSystem::CreateCPoint(681,356,0));
		GetObjectByID("p4stone")->SetScale(1.07f);
	}

	GetObjectByID("p4stonetaken")->SetEnable(false);
	
	if(ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved)
	{
		GetObjectByID("p4stone")->SetEnable(false);
	}
}

void L2S4_Pop4StoneBrick::Required()
{

}

void L2S4_Pop4StoneBrick::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	GFInstance->WrongInvDropMsg(Hud);
}

void L2S4_Pop4StoneBrick::Update(float ds)
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

	if(ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved)
	{
		if(ActionRectMoveStone.Intersection(&pos))
		{
			Cursor::SetMode(CA_Look);
			if(Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.L2_S4_BrickPuzzleAreaClicked = true;
				ClosePopup();
				/*ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_PZDeities;
				Hud->SendNewAreaMessage();*/
				return;
			}
		}
	}
	else
	{
		if(ActionRectMoveStone.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				if(!ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabMoved1)
				{
					JAnim_StoneMove1->PlayAnim();
					isAnimationPlaying = true;
				}
				else
				{
					GetObjectByID("p4stonetaken")->SetZ(eZOrder_Popup+eZOrder_PopFrame+10); //above frame
					JAnim_StoneMove2->PlayAnim();
					isAnimationPlaying = true;
				}
			}
		}
	}
}

void L2S4_Pop4StoneBrick::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//rockmove1
		isAnimationPlaying = false;
		ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabMoved1 = true;
		
		Hud->ShowBannerText("L2heavy");
	}
	else if(animID == 1)
	{
		//rockmove2
		isAnimationPlaying = false;
		ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved = true;

		GetObjectByID("p4stone")->SetEnable(false);
		GetObjectByID("p4stonetaken")->SetEnable(false);

		Hud->ShowBannerText("L2tough");
		
		Hud->SendUpdateSceneMessage();
	}
}

L2S4_Pop4StoneBrick::~L2S4_Pop4StoneBrick()
{
	SAFE_DELETE(JAnim_StoneMove1);
	SAFE_DELETE(JAnim_StoneMove2);
}

bool L2S4_Pop4StoneBrick::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S4_PZDeities])
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S4_Pop4StoneBrick::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved)
	{
		Hud->InitHintTrailEmit(&ActionRectMoveStone);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S4_PZDeities])
	{
		Hud->InitHintTrailEmit(&ActionRectMoveStone);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
