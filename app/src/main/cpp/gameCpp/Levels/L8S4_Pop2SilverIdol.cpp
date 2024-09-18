
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8S4_Pop2SilverIdol.h"
#include "Hud.h"

const CRectangle  IdolRect(586.0f,190.0f,208.0f,266.0f);

L8S4_Pop2SilverIdol::L8S4_Pop2SilverIdol(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	isAnimationPlaying = false;
	//Popup load
	Load("LV8\\S4\\POP2\\POP2.SCN");
	SetFrameAndCloseButton(true);

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	if(!ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced)
		GetObjectByID("SilverIdol")->SetEnable(false);

	S4_IdolPlaceFX = new SuperFX("PRT\\L8\\S4_IdolPlace.INI", PointSystem::CreateCPoint(685,337,0), GetObjectByID("SilverIdol")->ZOrder + 4 ); 
}

void L8S4_Pop2SilverIdol::Required()
{

}

void L8S4_Pop2SilverIdol::Update(float ds)
{

	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
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
	if(IdolRect.Intersection(&pos) && !Hud->IsDraggingItem() )
	{
		Cursor::SetMode(CA_Gear);
		if(  Control::Input->LBclicked())
			Hud->ShowBannerText("L8special");
	}
	//Hud->InvBoxCtrl->GlowDragInvObject(eInv_L8_S2_IdolSilver, IdolRect);
}

void L8S4_Pop2SilverIdol::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(IdolRect.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced )
	{
		if (itemVal == eInv_L8_S2_IdolSilver )
		{
			S4_IdolPlaceFX->Start();
			S4_IdolPlaceFX->AdvanceByTime(2.0f);
			GetObjectByID("SilverIdol")->SetEnable(true);
			ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced  = true;
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			Control::Audio->QuickLoadAndPlaySFX("s4p1_placeIdol");
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S2_IdolSilver);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_IdolSilver] = false;
			Hud->SendUpdateSceneMessage();
			return;
		}
	}
	GFInstance->WrongInvDropMsg(Hud);
}

void L8S4_Pop2SilverIdol::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if( !ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced && ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken )
		Hud->ShowHintForInvItem(eInv_L8_S2_IdolSilver, &IdolRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L8S4_Pop2SilverIdol::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced && ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken )
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L8S4_Pop2SilverIdol::~L8S4_Pop2SilverIdol()
{
	SAFE_DELETE(S4_IdolPlaceFX);
}
