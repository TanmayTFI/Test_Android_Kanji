//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 1: Honey COMB
//====================================

#include "L2S5_Pop6Chart.h"
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



L2S5_Pop6Chart::L2S5_Pop6Chart(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene,  zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV2\\S5\\POP6\\POP6.SCN");

	ColorRGB color = { 0.1f, 0.1f, 0.1f };
	waxTxt = new MultilineTextBox(AssetsStringTable::getString("honeywax"), 315, 320, zOrder + 10, color, eFont_Small, 20);
	mangoTxt = new MultilineTextBox(AssetsStringTable::getString("mango"), 315, 225, zOrder + 10, color, eFont_Small, 20);
	grinderTxt = new MultilineTextBox(AssetsStringTable::getString("grinder"), 600, 270, zOrder + 10, color, eFont_Small, 20);
	liquidTxt = new MultilineTextBox(AssetsStringTable::getString("grindermix"), 700, 120, zOrder + 10, color, eFont_Small, 20);
	aleoveraTxt = new MultilineTextBox(AssetsStringTable::getString("aleovera"), 890, 200, zOrder + 10, color, eFont_Small, 20);
	aleopiecesTxt = new MultilineTextBox(AssetsStringTable::getString("aleoverapieces"), 890, 325, zOrder + 10, color, eFont_Small, 20);
	turmericTxt = new MultilineTextBox(AssetsStringTable::getString("turmeric"), 315, 470, zOrder + 10, color, eFont_Small, 20);
	minigrinderTxt = new MultilineTextBox(AssetsStringTable::getString("minigrinder"), 450, 470, zOrder + 10, color, eFont_Small, 20);
	turmericmixTxt = new MultilineTextBox(AssetsStringTable::getString("turmericmix"), 560, 470, zOrder + 10, color, eFont_Small, 20);
	boiledwaterTxt = new MultilineTextBox(AssetsStringTable::getString("boiledwater"), 680, 470, zOrder + 10, color, eFont_Small, 20);

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	mangoTxt->Show(false);
	waxTxt->Show(false);
	grinderTxt->Show(false);
	liquidTxt->Show(false);
	aleoveraTxt->Show(false);
	aleopiecesTxt->Show(false);
	turmericTxt->Show(false);
	minigrinderTxt->Show(false);
	turmericmixTxt->Show(false);
	boiledwaterTxt->Show(false);
	Show(false);

}

void L2S5_Pop6Chart::Required()
{

}

void L2S5_Pop6Chart::Update( float ds)
{
	Popup::Update(ds);

	if (mangoTxt != NULL)	mangoTxt->Show(false);
	if (waxTxt != NULL)	waxTxt->Show(false);
	if (grinderTxt != NULL)	grinderTxt->Show(false);
	if (liquidTxt != NULL)	liquidTxt->Show(false);
	if (aleoveraTxt != NULL)	aleoveraTxt->Show(false);
	if (aleopiecesTxt != NULL)	aleopiecesTxt->Show(false);
	if (turmericTxt != NULL)	turmericTxt->Show(false);
	if (minigrinderTxt != NULL)	minigrinderTxt->Show(false);
	if (turmericmixTxt != NULL)	turmericmixTxt->Show(false);
	if (boiledwaterTxt != NULL)	boiledwaterTxt->Show(false);

	if(GetPopupState()!= ePopupState_Open )
		return;

	if (mangoTxt != NULL)	mangoTxt->Show(true);
	if (waxTxt != NULL)	waxTxt->Show(true);
	if (grinderTxt != NULL)	grinderTxt->Show(true);
	if (liquidTxt != NULL)	liquidTxt->Show(true);
	if (aleoveraTxt != NULL)	aleoveraTxt->Show(true);
	if (aleopiecesTxt != NULL)	aleopiecesTxt->Show(true);
	if (turmericTxt != NULL)	turmericTxt->Show(true);
	if (minigrinderTxt != NULL)	minigrinderTxt->Show(true);
	if (turmericmixTxt != NULL)	turmericmixTxt->Show(true);
	if (boiledwaterTxt != NULL)	boiledwaterTxt->Show(true);

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();

	
}

void L2S5_Pop6Chart::OnPopupCloseBtnClicked()
{
	//Control::Audio->QuickLoadAndPlaySFX("s5pz1_instructionpopupclose");
}


float L2S5_Pop6Chart::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S5_Pop6Chart::~L2S5_Pop6Chart()
{
	SAFE_DELETE(mangoTxt);
	SAFE_DELETE(waxTxt);
	SAFE_DELETE(grinderTxt);
	SAFE_DELETE(liquidTxt);
	SAFE_DELETE(aleoveraTxt);
	SAFE_DELETE(aleopiecesTxt);
	SAFE_DELETE(turmericTxt);
	SAFE_DELETE(minigrinderTxt);
	SAFE_DELETE(turmericmixTxt);
	SAFE_DELETE(boiledwaterTxt);
}

void L2S5_Pop6Chart::TakeInventoryItem(EHOInventory ItemID)
{
	
}

void L2S5_Pop6Chart::ReceiveMessage(int val)
{	

}

void L2S5_Pop6Chart::ProcessNonHOHint()
{
	
}