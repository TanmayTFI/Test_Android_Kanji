//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 01 - POPUP : LEAF CHEW
//====================================

#include "L4S1_PopLeafChewing.h"
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



L4S1_PopLeafChewing::L4S1_PopLeafChewing(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene,  zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV4\\S1\\POP\\POP.SCN");
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	Janim_chew = new JSONAnimator(2,CC_CALLBACK_1(L4S1_PopLeafChewing::OnChewComplete,this));
	Janim_chew->parseAndLoadJSON("ANIM\\L4\\L4_S1_POPLEAFCHEWING.JSON", this);
	isAnimPlaying = false;
}

void L4S1_PopLeafChewing::OnChewComplete(int animID)
{
	isAnimPlaying = false;
	ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken = true;
	Hud->TakeInventoryItem(eInv_L4_S1_Medicine);
	Hud->ShowBannerText("L4plantain");
	ClosePopup(1);
}

void L4S1_PopLeafChewing::Required()
{

}

void L4S1_PopLeafChewing::Update( float ds)
{
	Popup::Update(ds);

	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if (!isAnimPlaying)
	{
		Control::Audio->QuickLoadAndPlaySFX("s1p2_medicineChew");
		Janim_chew->PlayAnim();
		isAnimPlaying = true;
	}
}


float L4S1_PopLeafChewing::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S1_PopLeafChewing::~L4S1_PopLeafChewing()
{
	SAFE_DELETE(Janim_chew);
}

void L4S1_PopLeafChewing::TakeInventoryItem(EHOInventory ItemID)
{

}

void L4S1_PopLeafChewing::ReceiveMessage(int val)
{	

}

void L4S1_PopLeafChewing::ProcessNonHOHint()
{

}
