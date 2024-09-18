
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L6S2_PZPop1Chart.h"
#include "Hud.h"
#include "AssetsStringTable.h"

L6S2_PZPop1Chart::L6S2_PZPop1Chart(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV6\\S2\\PZ1\\CHART.SCN");
	//SetFrameAndCloseButton();

	// stonesTxt, * leavesTxt, * pineTxt, * stickTxt, * pestleTxt, * coalTxt;


	ColorRGB color = { 0.1f, 0.1f, 0.1f };
	stonesTxt = new MultilineTextBox(AssetsStringTable::getString("stonesTxt"), 390, 275, zOrder + 10, color, eFont_Small, 20);
	leavesTxt = new MultilineTextBox(AssetsStringTable::getString("leavesTxt"), 640, 210, zOrder + 10, color, eFont_Small, 20);
	pineTxt = new MultilineTextBox(AssetsStringTable::getString("pineTxt"), 865, 330, zOrder + 10, color, eFont_Small, 20);
	stickTxt = new MultilineTextBox(AssetsStringTable::getString("stickTxt"), 420, 475, zOrder + 10, color, eFont_Small, 20);
	pestleTxt = new MultilineTextBox(AssetsStringTable::getString("pestleTxt"), 850, 440, zOrder + 10, color, eFont_Small, 20);
	coalTxt = new MultilineTextBox(AssetsStringTable::getString("coalTxt"), 850, 550, zOrder + 10, color, eFont_Small, 20);

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	stonesTxt->Show(false);
	leavesTxt->Show(false);
	pineTxt->Show(false);
	stickTxt->Show(false);
	pestleTxt->Show(false);
	coalTxt->Show(false);
	Show(false);
}

void L6S2_PZPop1Chart::Required()
{

}

void L6S2_PZPop1Chart::Update(float ds)
{
	Popup::Update(ds);

	if (stonesTxt != NULL)	stonesTxt->Show(false);
	if (leavesTxt != NULL)	leavesTxt->Show(false);
	if (pineTxt != NULL)	pineTxt->Show(false);
	if (stickTxt != NULL)	stickTxt->Show(false);
	if (pestleTxt != NULL)	pestleTxt->Show(false);
	if (coalTxt != NULL)	coalTxt->Show(false);

	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;

	if (stonesTxt != NULL)	stonesTxt->Show(true);
	if (leavesTxt != NULL)	leavesTxt->Show(true);
	if (pineTxt != NULL)	pineTxt->Show(true);
	if (stickTxt != NULL)	stickTxt->Show(true);
	if (pestleTxt != NULL)	pestleTxt->Show(true);
	if (coalTxt != NULL)	coalTxt->Show(true);

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
}

void L6S2_PZPop1Chart::OnPopupCloseBtnClicked()
{
	//Control::Audio->QuickLoadAndPlaySFX("s5pz1_instructionpopupclose");
}

void L6S2_PZPop1Chart::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L6S2_PZPop1Chart::ProcessHint()
{
	
}


L6S2_PZPop1Chart::~L6S2_PZPop1Chart()
{
	Hud->InvBoxCtrl->ResetToInvBox();

	SAFE_DELETE(stonesTxt);
	SAFE_DELETE(leavesTxt);
	SAFE_DELETE(pineTxt);
	SAFE_DELETE(stickTxt);
	SAFE_DELETE(pestleTxt);
	SAFE_DELETE(coalTxt);
}
