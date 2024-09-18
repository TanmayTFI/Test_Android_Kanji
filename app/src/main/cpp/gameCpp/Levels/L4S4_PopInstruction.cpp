//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 04 - POPUP : CHART
//====================================

#include "L4S4_PopInstruction.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h" 
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "InteractiveObjectList.h"
#include "exMath.h"
#include "SuperLog.h"
#include "AssetsStringTable.h"



L4S4_PopInstruction::L4S4_PopInstruction(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene,  zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV4\\S4\\POP4\\POP.SCN");
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	
	ColorRGB color = { 0.1f, 0.1f, 0.1f };
	olives = new MultilineTextBox(AssetsStringTable::getString("olives"), 550, 160, zOrder + 10, color, eFont_Small, 20);
	strainer = new MultilineTextBox(AssetsStringTable::getString("strainer"), 720, 225, zOrder + 10, color, eFont_Small, 20);
	cup = new MultilineTextBox(AssetsStringTable::getString("cup"), 950, 160, zOrder + 10, color, eFont_Small, 20);
	useforfire = new MultilineTextBox(AssetsStringTable::getString("useforfire"), 760, 450, zOrder + 10, color, eFont_26, 20);
	grinder = new MultilineTextBox(AssetsStringTable::getString("grinder"), 350, 370, zOrder + 10, color, eFont_Small, 20);

	olives->Show(false);
	strainer->Show(false);
	cup->Show(false);
	useforfire->Show(false);
	grinder->Show(false);

	Show(false);
}

void L4S4_PopInstruction::Required()
{

}

void L4S4_PopInstruction::Update( float ds)
{
	Popup::Update(ds);

	if (olives != NULL)	olives->Show(false);
	if (strainer != NULL)	strainer->Show(false);
	if (cup != NULL)	cup->Show(false);
	if (useforfire != NULL)	useforfire->Show(false);
	if (grinder != NULL)	grinder->Show(false);

	if(GetPopupState()!= ePopupState_Open )
		return;

	if (olives != NULL)	olives->Show(true);
	if (strainer != NULL)	strainer->Show(true);
	if (cup != NULL)	cup->Show(true);
	if (useforfire != NULL)	useforfire->Show(true);
	if (grinder != NULL)	grinder->Show(true);

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
	if (Control::Input->LBclicked())
		ClosePopup();
}


float L4S4_PopInstruction::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S4_PopInstruction::~L4S4_PopInstruction()
{
	SAFE_DELETE(olives);
	SAFE_DELETE(strainer);
	SAFE_DELETE(cup);
	SAFE_DELETE(useforfire);
	SAFE_DELETE(grinder);
}

void L4S4_PopInstruction::TakeInventoryItem(EHOInventory ItemID)
{
	
}

void L4S4_PopInstruction::ReceiveMessage(int val)
{	

}

void L4S4_PopInstruction::ProcessNonHOHint()
{
	
}