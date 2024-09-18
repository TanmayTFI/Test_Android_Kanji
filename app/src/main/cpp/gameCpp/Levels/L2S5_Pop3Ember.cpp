//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 1: Honey COMB
//====================================

#include "L2S5_Pop3Ember.h"
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

const CRectangle  InvEmberRect(596.0f,293.0f,184.0f,155.0f);


L2S5_Pop3Ember::L2S5_Pop3Ember(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene,  zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV2\\S5\\POP3\\POP3.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	SetSparkleUpdate(false);
	Show(false);

}

void L2S5_Pop3Ember::Required()
{

}

void L2S5_Pop3Ember::Update( float ds)
{
	Popup::Update(ds);

	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	
	
	if(InvEmberRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("Ember")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken = true;
			Hud->TakeInventoryItem(eInv_L2S5_Ember);
			UpdateBgGraphic();
			SetSparkleUpdate(true);
			ClosePopup(1.0f);
		}
	}

}

float L2S5_Pop3Ember::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S5_Pop3Ember::~L2S5_Pop3Ember()
{

}

void L2S5_Pop3Ember::ReceiveMessage(int val)
{	

}

bool L2S5_Pop3Ember::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken)
		isAnyTaskLeft = true;
	return isAnyTaskLeft;
}

void L2S5_Pop3Ember::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken  )
			Hud->InitHintTrailEmit(GetObjectByID("Ember")->GetCenterX(), GetObjectByID("Ember")->GetCenterY(),true);
	else
		Popup::ProcessHint();
	
}