//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 01 - POPUP 3: 
//====================================

#include "L6S1_Pop3StatueHand.h"
#include "AwardManager.h"

const CRectangle  HandRect(365.0f,174.0f,402.0f,322.0f);
const CRectangle  SymbolsRect(366.0f,260.0f,133.0f,123.0f);


L6S1_Pop3StatueHand::L6S1_Pop3StatueHand(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S1\\P3\\P3.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound)
	{
		GetObjectByID("magnifying-glass_Transperent")->SetEnable(false);
		GetObjectByID("Zoom_BG")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken)
	{
		GetObjectByID("Cloth")->SetEnable(false);
	}

	Janim_magnify = new JSONAnimator(0,CC_CALLBACK_1(L6S1_Pop3StatueHand::OnAnimComplete,this));
	Janim_magnify->parseAndLoadJSON("ANIM\\L6\\S1_MAGNIFYINGLENS.JSON",this);

	int ArrayPts[] = {749,262,757,295,741,370,766,376,799,305,819,280,795,244,746,261};
	ringRect =  new CPolygon(8);
	ringRect->Init(ArrayPts);

	SetSparkleUpdate(false);
	Show(false);
}


void L6S1_Pop3StatueHand::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (GetObjectByID("Cloth")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("Cloth")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L6_S1_RagCloth);
			ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken = true;
		}
	}
	else if (ringRect->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated)
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6ring");
		}
	}
	else if (HandRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound && ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6but");
		}
	}
}

void L6S1_Pop3StatueHand::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L6_S4_Lens:
		if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken)
		{
			GetObjectByID("magnifying-glass_Transperent")->SetEnable(true);
			GetObjectByID("Zoom_BG")->SetEnable(true);
			Janim_magnify->PlayAnim(true);
			ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound = true;
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_Lens] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_Lens);

			AwardManager::GetInstance()->CollectAward(kAward_Decrypt_Indus_Script);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L6S1_Pop3StatueHand::OnAnimComplete(int animID)
{
	//ClosePopup();
}


float L6S1_Pop3StatueHand::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S1_Pop3StatueHand::~L6S1_Pop3StatueHand()
{
	SAFE_DELETE(Janim_magnify);
	SAFE_DELETE(ringRect);
}

void L6S1_Pop3StatueHand::Required()
{

}

void L6S1_Pop3StatueHand::OnPopupOpened()
{
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound)
	{
		Janim_magnify->PlayAnim(true);
	}

	Popup::OnPopupOpened();
}


void L6S1_Pop3StatueHand::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L6S1_Pop3StatueHand::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken || 
		(ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L6S1_Pop3StatueHand::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("Cloth")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle clothObjRect =GetObjectByID("Cloth")->GetWorldRect();
		Hud->InitHintTrailEmit(&clothObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound)
	{
		Hud->ShowHintForInvItem(eInv_L6_S4_Lens,&SymbolsRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
