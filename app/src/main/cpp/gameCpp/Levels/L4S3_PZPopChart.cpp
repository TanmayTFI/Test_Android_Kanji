
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L4S3_PZPopChart.h"
#include "Hud.h"
#include "AssetsStringTable.h"

L4S3_PZPopChart::L4S3_PZPopChart(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV4\\S3\\P1\\CHART.SCN");

	ColorRGB color = { 0.1f, 0.1f, 0.1f };
	metalplate = new MultilineTextBox(AssetsStringTable::getString("metalplate"), 340, 250, zOrder + 10, color, eFont_Small, 20);
	woodenboard = new MultilineTextBox(AssetsStringTable::getString("woodenboard"), 500, 240, zOrder + 10, color, eFont_Small, 20);
	bow = new MultilineTextBox(AssetsStringTable::getString("bow"), 670, 250, zOrder + 10, color, eFont_Small, 20);
	coin = new MultilineTextBox(AssetsStringTable::getString("coin"), 900, 250, zOrder + 10, color, eFont_Small, 20);
	twigs = new MultilineTextBox(AssetsStringTable::getString("twigs"), 310, 350, zOrder + 10, color, eFont_Small, 20);
	oliveoil = new MultilineTextBox(AssetsStringTable::getString("oliveoil"), 440, 315, zOrder + 10, color, eFont_Small, 20);
	embers = new MultilineTextBox(AssetsStringTable::getString("embers"), 540, 350, zOrder + 10, color, eFont_Small, 20);
	embers2 = new MultilineTextBox(AssetsStringTable::getString("embers2"), 630, 480, zOrder + 10, color, eFont_Small, 20);

	//*woodenboard, *bow, *coin, *twigs, *oliveoil, *embers;

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	metalplate->Show(false);
	woodenboard->Show(false);
	bow->Show(false);
	coin->Show(false);
	twigs->Show(false);
	oliveoil->Show(false);
	embers->Show(false);
	embers2->Show(false);
	Show(false);
}

void L4S3_PZPopChart::Required()
{

}

void L4S3_PZPopChart::OnPopupCloseBtnClicked()
{
	//Control::Audio->QuickLoadAndPlaySFX("s5pz1_instructionpopupclose");
}

void L4S3_PZPopChart::Update(float ds)
{
	Popup::Update(ds);

	if (metalplate != NULL)	metalplate->Show(false);
	if (woodenboard != NULL)	woodenboard->Show(false);
	if (bow != NULL)	bow->Show(false);
	if (coin != NULL)	coin->Show(false);
	if (twigs != NULL)	twigs->Show(false);
	if (oliveoil != NULL)	oliveoil->Show(false);
	if (embers != NULL)	embers->Show(false);
	if (embers2 != NULL)	embers2->Show(false);


	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;

	if (metalplate != NULL)	metalplate->Show(true);
	if (woodenboard != NULL)	woodenboard->Show(true);
	if (bow != NULL)	bow->Show(true);
	if (coin != NULL)	coin->Show(true);
	if (twigs != NULL)	twigs->Show(true);
	if (oliveoil != NULL)	oliveoil->Show(true);
	if (embers != NULL)	embers->Show(true);
	if (embers2 != NULL)	embers2->Show(true);

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

void L4S3_PZPopChart::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L4S3_PZPopChart::ProcessHint()
{
	
}

bool L4S3_PZPopChart::isTaskLeft()
{
	bool isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L4S3_PZPopChart::~L4S3_PZPopChart()
{
	SAFE_DELETE(metalplate);
}
