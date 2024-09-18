//====================================
//	DQFYLH
//	HO System
//	Level 8 - Scene 01 - POPUP 1: 
//====================================

#include "L8S1_Pop1MagicWand.h"
const CRectangle  PopAreaRect(479.0f,35.0f,413.0f,598.0f);


L8S1_Pop1MagicWand::L8S1_Pop1MagicWand(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV8\\S1\\P1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken)
	{
		GetObjectByID("MAgicWand")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvHalfKeyTaken)
	{
		GetObjectByID("Key_half")->SetEnable(false);
	}

	/*if (ProfileSystem::Instance->ActiveProfile.L8_S1_NoteTaken)
	{
		GetObjectByID("Letter")->SetEnable(false);
	}*/

	int ArrayPts[] = {751,416,642,479,662,516,690,520,708,566,732,564,838,525,831,514,813,502,799,501,750,411};
	CPolygon *CPolyObjName=  new CPolygon(11);
	CPolyObjName->Init(ArrayPts);
	popUp = new L8S1_PopRisingPheonix(1, Hud, NULL, zOrder + 200, CPolyObjName->center,CC_CALLBACK_1(L8S1_Pop1MagicWand::OnPopupClosed,this));

	CObject *temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L8\\PHOENIXTEXT.SPR", 685, 320, eZOrder_Popup + 500);
	temp->SetUID("phoenixText");
	temp->Show(false);
	popUp->PushObject(temp);

	delete(CPolyObjName);
	popUp->SetPopupBoundingBox(PopAreaRect);
	isPopupActive = false;
	isCollectingInventory = false;

	int ArrayPts1[] = {419,242,404,258,407,276,420,293,438,281,496,281,671,282,774,284,825,292,826,300,786,346,842,339,864,359,851,417,893,378,921,384,942,430,964,378,980,370,1005,393,999,143,980,160,958,160,933,105,914,158,904,165,850,123,862,185,854,196,787,180,836,230,817,239,810,235,776,243,738,253,434,251,417,240};
	magicWandRect=  new CPolygon(37);
	magicWandRect->Init(ArrayPts1);

	SetSparkleUpdate(false);
	Show(false);
}


void L8S1_Pop1MagicWand::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if(isPopupActive)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (GetObjectByID("Letter")->GetWorldRect().Intersection(&pos)/* && !ProfileSystem::Instance->ActiveProfile.L8_S1_NoteTaken*/)
	{
		Cursor::SetMode(CA_Look);
		if (Control::Input->LBclicked())
		{
			popUp->OpenPopup();
			//GetObjectByID("Letter")->SetEnable(false);
			isPopupActive = true;
		}
	}
	else if (GetObjectByID("Key_half")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S1_InvHalfKeyTaken && !isCollectingInventory)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("Key_half")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L8_S1_HalfKey);
			ProfileSystem::Instance->ActiveProfile.L8_S1_InvHalfKeyTaken = true;
			isCollectingInventory = true;
			if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken && ProfileSystem::Instance->ActiveProfile.L8_S1_NoteTaken)
			{
				ClosePopup(1.0f);
			}
		}
	}
	else if (magicWandRect->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken && !isCollectingInventory)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("MAgicWand")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L8_S1_MagicWand);
			ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken = true;
			isCollectingInventory = true;
			if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvHalfKeyTaken && ProfileSystem::Instance->ActiveProfile.L8_S1_NoteTaken)
			{
				ClosePopup(1.0f);
			}
		}
	}

}

void L8S1_Pop1MagicWand::OnPopupClosed(Popup * popup)
{
	isPopupActive = false;
	ProfileSystem::Instance->ActiveProfile.L8_S1_NoteTaken = true;
	//if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvHalfKeyTaken && ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken)
	//{
	//	ClosePopup(1.0f);
	//}

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P2_PhoenixNoteEnd])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P2_PhoenixNoteEnd] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L8P2_PhoenixNoteEnd;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}
}

void L8S1_Pop1MagicWand::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	//switch (itemVal)
	//{
	//	/*case eInv_L6_S1_Club:
	//	if (GetObjectByID("IceCoreWithKey")->GetWorldRect().Intersection(&dropPos))
	//	{
	//	Hud->ShowBannerText("L6just");
	//	Hud->CancelDrag();
	//	}
	//	break;*/
	//default:
	//	GFInstance->WrongInvDropMsg(Hud);
	//	break;
	//}
}

void L8S1_Pop1MagicWand::OnAnimComplete(int animID)
{

}

float L8S1_Pop1MagicWand::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L8S1_Pop1MagicWand::~L8S1_Pop1MagicWand()
{
	SAFE_DELETE(popUp);
	SAFE_DELETE(magicWandRect);
}

void L8S1_Pop1MagicWand::Required()
{

}

void L8S1_Pop1MagicWand::ClosePopup(float delayTime)
{
	if(isPopupActive)
		return;
	Popup::ClosePopup(delayTime);
}

void L8S1_Pop1MagicWand::ReceiveMessage(int val)
{	
	if( val == INV_CLOSE )
	{		
		isCollectingInventory = false;
	}	
}

bool L8S1_Pop1MagicWand::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L8_S1_InvHalfKeyTaken || 
		!ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken ||
		!ProfileSystem::Instance->ActiveProfile.L8_S1_NoteTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L8S1_Pop1MagicWand::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L8_S1_InvHalfKeyTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("Key_half")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("Key_half")->GetWorldRect();
		Hud->InitHintTrailEmit(&ObjRect);
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("MAgicWand")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("MAgicWand")->GetWorldRect();
		Hud->InitHintTrailEmit(&ObjRect);
		
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L8_S1_NoteTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("Letter")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("Letter")->GetWorldRect();
		Hud->InitHintTrailEmit(&ObjRect);
		
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
