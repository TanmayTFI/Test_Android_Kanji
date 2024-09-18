//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 03 - POPUP 1: Honey COMB
//====================================

#include "L2S5_Pop1Vinegar.h"
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


const CRectangle  InvStoneRect(448.0f,276.0f,239.0f,189.0f);

L2S5_Pop1Vinegar::L2S5_Pop1Vinegar(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	int VinegarArrayPts[] = {702,224,802,222,805 ,448,743,460,694,439};
	VinegarPoly =  new CPolygon(5);
	VinegarPoly->Init(VinegarArrayPts);

	int StethArrayPts[] = {676,458,764,470,858,456,848,419,975,406,987,475,688,544};
	StethPoly=  new CPolygon(7);
	StethPoly->Init(StethArrayPts);


	//Popup load
	Load("LV2\\S5\\POP1\\POP1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	
	InvStoneSharpner = GetObjectByID("Stone");
	InvVinegarBottle = GetObjectByID("Vinegar");
	MemStethescope = GetObjectByID("Stethescope");

	if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken )
	{
		InvStoneSharpner->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken )
	{
		InvVinegarBottle->SetEnable(false);
	}

	if( ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope] )
	{
		MemStethescope->SetEnable(false);
	}

	

	
	SetSparkleUpdate(false);
	Show(false);

	

	PopupDust = new SuperFX("PRT\\L2\\poupWindowDust.INI", PointSystem::CreateCPoint(688, 235,0), zOrder + 5 );
	//PopupDust->Start();

}

void L2S5_Pop1Vinegar::Required()
{

}

void L2S5_Pop1Vinegar::ClosePopup(float delayTime)
{
	PopupDust->StopImmediate();
	Popup::ClosePopup(delayTime);
}

void L2S5_Pop1Vinegar::OnPopupOpened()
{
	PopupDust->Start();
	PopupDust->AdvanceByTime(2.0f);
}



void L2S5_Pop1Vinegar::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if( GFInstance->IsActivePopup) 
	{
		return;
	}


	if(InvStoneRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			InvStoneSharpner->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken = true;
			Hud->TakeInventoryItem(eInv_L2S5_StoneSharpener);
			UpdateBgGraphic();
			SetSparkleUpdate(true);
		#ifdef _CEBUILD
			if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope] )
		#else
			if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken )
#endif 
			{
				ClosePopup(1.0f);
			}
			return;
		}
	}
	if(VinegarPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken )
	{
		Cursor::SetMode(CA_HandTake );
		if( Control::Input->LBclicked())
		{
			InvVinegarBottle->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken = true;
			Hud->TakeInventoryItem(eInv_L2S5_VinegarBottle, Control::Input->Mpos() );
			UpdateBgGraphic();
			SetSparkleUpdate(true);
		#ifdef _CEBUILD
			if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope]  )
		#else
			if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken )
#endif 
			{
				ClosePopup(1.0f);
			}
			return;
		}
	}
	#ifdef _CEBUILD
		if(StethPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope] )
		{
			if( Control::Input->LBclicked())
			{
				MemStethescope->FadeOut();
				Hud->ShowMemoryObjectCollected_CE(eMem_L2S5Stethoscope, &MemStethescope->Pos);
				ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope]= true;
				if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken && ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken )
				{
					ClosePopup(1.0f);
				}
				return;
			}
		}
	#else
		if(StethPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope] )
		{
			if( Control::Input->LBclicked())
			{
				MemStethescope->FadeOut();
				Hud->ShowMemoryObjectCollected_SE(eMem_L2S5Stethoscope, &MemStethescope->Pos);
			}
		}
	#endif 
}

float L2S5_Pop1Vinegar::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S5_Pop1Vinegar::~L2S5_Pop1Vinegar()
{
	SAFE_DELETE(PopupDust);
}

void L2S5_Pop1Vinegar::ReceiveMessage(int val)
{	

	
}

bool L2S5_Pop1Vinegar::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken || !ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken)
		isAnyTaskLeft = true;
	return isAnyTaskLeft;
}

void L2S5_Pop1Vinegar::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken  )
		Hud->InitHintTrailEmit(InvStoneSharpner->GetCenterX(), InvStoneSharpner->GetCenterY(), true);
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken  )
		Hud->InitHintTrailEmit(InvVinegarBottle->GetCenterX(), InvVinegarBottle->GetCenterY(),true);
	else
		Popup::ProcessHint();
}