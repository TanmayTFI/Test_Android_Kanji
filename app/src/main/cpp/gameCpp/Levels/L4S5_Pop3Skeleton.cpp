//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 05 - POPUP 3: SKELETON
//====================================

#include "L4S5_Pop3Skeleton.h"
const CRectangle  PopAreaRect(270.0f,74.0f,825.0f,539.0f);
const CRectangle  StarMemRect(537.0f,236.0f,57.0f,55.0f);

L4S5_Pop3Skeleton::L4S5_Pop3Skeleton(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV4\\S5\\POP3\\P3.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	int ArrayPts[] = {710,239,679,328,686,347,709,344,742,268,729,238,708,236};
	CPolygon *CPolyObjName=  new CPolygon(7);
	CPolyObjName->Init(ArrayPts);
	popChart = new L4S4_PopInstruction(1, Hud, NULL, zOrder + 200,CPolyObjName->center,CC_CALLBACK_1(L4S5_Pop3Skeleton::OnPopupClosed,this));
	popChart->SetPopupBoundingBox(PopAreaRect);
	SAFE_DELETE(CPolyObjName);

	SetSparkleUpdate(false);
	
	if( ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected )
		GetObjectByID("Machine_base")->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S5StarLocket])
	{
		GetObjectByID("Star")->SetEnable(false);
	}

	Show(false);

	activePopUPID = ePopInactive;

}

void L4S5_Pop3Skeleton::OnPopupClosed(Popup * popup)
{
	activePopUPID = ePopInactive;

	/*Hud->TakeInventoryItem(eInv_L4_S5_GrinderTop);
	ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected = true;*/
	UpdateBgGraphic();
	SetSparkleUpdate(true);
	ClosePopup(1.5f);
}

void L4S5_Pop3Skeleton::Required()
{

}

void L4S5_Pop3Skeleton::ClosePopup(float delayTime)
{
	if (activePopUPID != ePopInactive)
		return;
	Popup::ClosePopup(delayTime);
}


void L4S5_Pop3Skeleton::Update(float ds)
{
	Popup::Update(ds);

	if(GetPopupState()!= ePopupState_Open || activePopUPID != ePopInactive)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
		
#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S5StarLocket] && StarMemRect.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )		
		{
			GetObjectByID("Star")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L4S5StarLocket, &GetObjectByID("Star")->Pos);

			if (ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected)
				ClosePopup(1.5f);
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S5StarLocket] && StarMemRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("Star")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L4S5StarLocket, &(GetObjectByID("Star")->Pos));
		}
	}
#endif 

	if (!ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected && GetObjectByID("Machine_base")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{			
			GetObjectByID("Machine_base")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L4_S5_GrinderTop);
			ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected = true;
			UpdateBgGraphic();
			SetSparkleUpdate(true);

#ifdef _CEBUILD
			if(ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S5StarLocket])
				ClosePopup(1.5f);
#else
			ClosePopup(1.5f);
#endif

			/*activePopUPID = 1;
			popChart->OpenPopup();*/

			/*Hud->TakeInventoryItem(eInv_L4_S5_OilMakingChart);
			ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected = true;
			UpdateBgGraphic();
			SetSparkleUpdate(true);
			ClosePopup(1.5f);*/
		}
	}
}

float L4S5_Pop3Skeleton::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S5_Pop3Skeleton::~L4S5_Pop3Skeleton()
{
	SAFE_DELETE(popChart);
}

bool L4S5_Pop3Skeleton::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected )
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S5_Pop3Skeleton::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected )
	{
		Hud->InitHintTrailEmit( GetObjectByID("Machine_base")->GetCenterX(),  GetObjectByID("Machine_base")->GetCenterY());
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}