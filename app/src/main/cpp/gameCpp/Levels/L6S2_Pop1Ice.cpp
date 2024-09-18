//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 02 - POPUP 1: 
//====================================

#include "L6S2_Pop1Ice.h"
const CRectangle  MemObjRect(884.0f,266.0f,85.0f,168.0f);

L6S2_Pop1Ice::L6S2_Pop1Ice(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S2\\P1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken)
		GetObjectByID("IceCoreWithKey")->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestleTaken)
		GetObjectByID("Pestle")->SetEnable(false);
	isCollectingInventory = false;

	int ArrayPts[] = {429,457,451,467,456,480,560,524,575,523,590,530,591,551,580,572,560,577,541,560,437,517,420,519,402,511,395,481,427,458};
	pestleRect =  new CPolygon(15);
	pestleRect->Init(ArrayPts);

	int ArrayPts1[] = {855,208,702,162,572,166,569,201,554,209,510,433,510,489,715,555,778,478,821,413,836,408,862,308,868,246,849,211};
	iceCubeRect =  new CPolygon(14);
	iceCubeRect->Init(ArrayPts1);



	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue])
	{
		GetObjectByID("MemObj")->SetEnable(false);
	}

	SetSparkleUpdate(false);
	Show(false);
}


void L6S2_Pop1Ice::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestleTaken && pestleRect->Intersection(&pos) && !isCollectingInventory)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("Pestle")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L6_S2_Pestle);
			isCollectingInventory = true;
			ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestleTaken = true;
			if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken)
			{
#ifdef _CEBUILD
				if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue])
				{			
					ClosePopup(1.5);
				}
#else
				ClosePopup(1.5);
#endif 
			}
		}
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken && iceCubeRect->Intersection(&pos) && !isCollectingInventory)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6ice");
			GetObjectByID("IceCoreWithKey")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L6_S2_StarShapedKeyInIce);
			isCollectingInventory = true;
			ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken = true;
			if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestleTaken)
			{
#ifdef _CEBUILD
				if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue])
				{			
					ClosePopup(1.5);
				}
#else
				ClosePopup(1.5);
#endif 
			}
		}
	}
#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue] && MemObjRect.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("MemObj")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L6S2MotherkidStatue, &GetObjectByID("MemObj")->Pos);
			if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestleTaken && ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken)
			{
				ClosePopup(1);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue] && MemObjRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("MemObj")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L6S2MotherkidStatue, &(GetObjectByID("MemObj")->Pos));
		}
	}
#endif 
}

void L6S2_Pop1Ice::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L6_S1_Club:
		if (GetObjectByID("IceCoreWithKey")->GetWorldRect().Intersection(&dropPos))
		{
			Hud->ShowBannerText("L6just");
			Hud->CancelDrag();
		}
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L6S2_Pop1Ice::ReceiveMessage(int val)
{	
	if( val == INV_CLOSE )
	{		
		isCollectingInventory = false;
	}	
}

void L6S2_Pop1Ice::OnAnimComplete(int animID)
{

}

float L6S2_Pop1Ice::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S2_Pop1Ice::~L6S2_Pop1Ice()
{
	SAFE_DELETE(pestleRect);
	SAFE_DELETE(iceCubeRect);
}

void L6S2_Pop1Ice::Required()
{

}

void L6S2_Pop1Ice::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L6S2_Pop1Ice::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken || 
		!ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestleTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L6S2_Pop1Ice::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("IceCoreWithKey")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle iceCoreObjRect =GetObjectByID("IceCoreWithKey")->GetWorldRect();
		Hud->InitHintTrailEmit(&iceCoreObjRect);
		
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestleTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("Pestle")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle pestleObjRect =GetObjectByID("Pestle")->GetWorldRect();
		Hud->InitHintTrailEmit(&pestleObjRect);
		
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
