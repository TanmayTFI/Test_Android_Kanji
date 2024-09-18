//====================================
//  created by : Faisal Rasak
//  copyright  : Tuttifrutti Games
//====================================

#include "L3_S2_M4Pop2Pipe.h"
#include "Hud.h"

const CRectangle  InvItemRectPipe(632.0f,215.0f,162.0f,176.0f);

L3_S2_M4Pop2Pipe::L3_S2_M4Pop2Pipe(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S2\\MAZE4\\POP2\\P2.SCN");
	SetFrameAndCloseButton(true);

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);
}

void L3_S2_M4Pop2Pipe::Required()
{

}

void L3_S2_M4Pop2Pipe::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
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
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken)
	{
		if(InvItemRectPipe.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("pipe")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken = true;
				Hud->TakeInventoryItem(eInv_L3S2_PipePiece4Room4, GetObjectByID("pipe")->Pos);
				ClosePopup(2.5f);
				return;
			}
		} 
	}
}

void L3_S2_M4Pop2Pipe::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L3_S2_M4Pop2Pipe::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken)
	{
		Hud->InitHintTrailEmit(&InvItemRectPipe);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3_S2_M4Pop2Pipe::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L3_S2_M4Pop2Pipe::~L3_S2_M4Pop2Pipe()
{
}