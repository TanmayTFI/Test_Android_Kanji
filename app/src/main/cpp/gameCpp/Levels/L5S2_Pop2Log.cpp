//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 02 - POPUP 3: Coconut
//====================================

#include "L3_S2_FortInterior.h"
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

#include "L5S2_Pop2Log.h"

const CRectangle  InvDestRect(399.0f,161.0f,193.0f,410.0f);
const CRectangle  InfoRectRiver(705.0f,387.0f,287.0f,179.0f);


L5S2_Pop2Log::L5S2_Pop2Log(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV5\\S2\\POP2\\P2.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("logFalling")->SetEnable(false);
	GetObjectByID("logInWater")->SetEnable(false);

	JAnim_Log = new JSONAnimator(0, CC_CALLBACK_1(L5S2_Pop2Log::OnAnimationComplete, this));
	JAnim_Log->parseAndLoadJSON("ANIM\\L5\\S2LOGFALLING.JSON", this);

	waterObj1 = NULL;
	waterObj1 = new MeshObject(GetObjectByID("PondWaterAnim"), 10, 10);
	waterObj1->meshFX->SetSpeedAndDisplacement(4, 4, 3, 3);
	InsertObject(waterObj1, 1);

	Show(false);

}

void L5S2_Pop2Log::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		GetObjectByID("logOnGround")->SetEnable(false);
		GetObjectByID("logOnGroundShadow")->SetEnable(false);
		GetObjectByID("logFalling")->SetEnable(false);
		ClosePopup(1.0f);
	}
	
}

void L5S2_Pop2Log::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}
	
		GFInstance->WrongInvDropMsg(Hud);
}

void L5S2_Pop2Log::Required()
{

}

void L5S2_Pop2Log::Update(float ds)
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

	if(!ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell && InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L5hollow");
				JAnim_Log->PlayAnim();
				Control::Audio->QuickLoadAndPlaySFX("s2p2_logfall");
				ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell = true;
				
			}
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell &&  InfoRectRiver.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			Hud->ShowBannerText("L5water");
		}
	}
	
}

float L5S2_Pop2Log::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L5S2_Pop2Log::~L5S2_Pop2Log()
{
	SAFE_DELETE(JAnim_Log);
}

void L5S2_Pop2Log::TakeInventoryItem(EHOInventory ItemID)
{
	/*std::vector<int> vecInvIds;
	switch (ItemID)
	{
	
	default:
		break;
	}*/
}

void L5S2_Pop2Log::ReceiveMessage(int val)
{	

	
}

bool L5S2_Pop2Log::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L5S2_Pop2Log::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if( !ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell) 
		Hud->InitHintTrailEmit(InvDestRect.GetCenterX(), InvDestRect.GetCenterY());
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
