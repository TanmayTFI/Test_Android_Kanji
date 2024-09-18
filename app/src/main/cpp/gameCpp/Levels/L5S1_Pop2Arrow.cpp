//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S1_Pop2Arrow.h"
#include "Hud.h"

const CRectangle  InvItemRectArrowHolder(534.0f,224.0f,282.0f,205.0f);

L5S1_Pop2Arrow::L5S1_Pop2Arrow(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S1\\POP2\\P2.SCN");
	SetFrameAndCloseButton(true);

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);
}

void L5S1_Pop2Arrow::Required()
{

}

void L5S1_Pop2Arrow::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
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
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
	{
		if(InvItemRectArrowHolder.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("arrowholder")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken = true;
				Hud->TakeInventoryItem(eInv_L5_S1_ArrowHolder);

				if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowTaken)
					Hud->ShowBannerText("L5arrow");

				ClosePopup(1.25f);
			}
		}
	}
}

void L5S1_Pop2Arrow::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L5S1_Pop2Arrow::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectArrowHolder);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L5S1_Pop2Arrow::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L5S1_Pop2Arrow::~L5S1_Pop2Arrow()
{
}
