//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 02 - POPUP 3: Coconut
//====================================

#include "L5_S4_Dungeon.h"
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

#include "L5S4_Pop3HandCuff.h"

const CRectangle  InvDestRect(567.0f,206.0f,253.0f,245.0f);
const CRectangle  InvRectBark(726.0f,328.0f,200.0f,137.0f);
const CRectangle  MemRectWatermelon(599.0f,176.0f,165.0f,120.0f);


L5S4_Pop3HandCuff::L5S4_Pop3HandCuff(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV5\\S4\\POP3\\P3.SCN"); 
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	

	Show(false);

}

void L5S4_Pop3HandCuff::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		
		
	}
	
}

void L5S4_Pop3HandCuff::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}
	
		GFInstance->WrongInvDropMsg(Hud);
}

void L5S4_Pop3HandCuff::Required()
{

}

void L5S4_Pop3HandCuff::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();

	if( InvDestRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Look);
			if( Control::Input->LBclicked())
			{	
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S4_PZHandCuff;
				Hud->SendNewAreaMessage();		
			}
		}
	
	
}

float L5S4_Pop3HandCuff::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L5S4_Pop3HandCuff::~L5S4_Pop3HandCuff()
{
	
}

void L5S4_Pop3HandCuff::TakeInventoryItem(EHOInventory ItemID)
{
	
}

void L5S4_Pop3HandCuff::ReceiveMessage(int val)
{	

	
}

bool L5S4_Pop3HandCuff::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	

	return isAnyTaskLeft;
}

void L5S4_Pop3HandCuff::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if( !ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete) 
		Hud->InitHintTrailEmit(InvDestRect.GetCenterX(), InvDestRect.GetCenterY());
	
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}