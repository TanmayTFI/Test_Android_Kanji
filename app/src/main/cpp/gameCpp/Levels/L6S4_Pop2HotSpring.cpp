//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 04 - POPUP 3: 
//====================================

#include "L6S4_Pop2HotSpring.h"

const CRectangle  WaterRect(606.0f,95.0f,396.0f,247.0f);
const CRectangle  MemObjRect(396.0f,131.0f,43.0f,133.0f);

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

L6S4_Pop2HotSpring::L6S4_Pop2HotSpring(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S4\\P2\\P2.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	mo_water = new MeshObject(GetObjectByID("WaterAnimation"), 5, 5);
	mo_water->meshFX->SetSpeedAndDisplacement(4, 3, 2, 3);
	PushObject(mo_water);

	GetObjectByID("WaterPopup__0017_IceWithKey")->SetEnable(false);
	GetObjectByID("Key-UnderWater")->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken)
	{
		GetObjectByID("Key-UnderWater")->SetEnable(true);
	}
	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken)
	{
		GetObjectByID("glass")->SetEnable(false);
	}

	GetObjectByID("Cloth")->SetMaskRect(PopAreaRect);

	GetObjectByID("Cloth")->SetEnable(false);
	//GetObjectByID("Cloth")->SetZ(eZOrder_Popup - 1);

	Janim_ice_melt = new JSONAnimator(0);
	Janim_ice_melt->parseAndLoadJSON("ANIM\\L6\\S4_ICE_MELTING.JSON", this);

	Janim_keyTake = new JSONAnimator(1,CC_CALLBACK_1(L6S4_Pop2HotSpring::OnAnimComplete,this));
	Janim_keyTake->parseAndLoadJSON("ANIM\\L6\\S4_TAKE_KEY.JSON", this);

	SnowFX_L =  new SuperFX("PRT\\L6\\s4_pop_smoke.INI", PointSystem::CreateCPoint(838,305,0), eZOrder_PopupCloseButton); 



	s4_HotSpringPopupBubblesFX = new SuperFX("PRT\\L6\\s4_HotSpringPopupBubbles.INI", PointSystem::CreateCPoint(825,279,0), eZOrder_PopupCloseButton); 

	isCollectingInventory = false;


	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue])
	{
		GetObjectByID("MemObj")->SetEnable(false);
	}

	SetSparkleUpdate(false);
	Show(false);
}


void L6S4_Pop2HotSpring::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken && GetObjectByID("glass")->GetWorldRect().Intersection(&pos) && !isCollectingInventory)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("glass")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L6_S4_Lens);
			ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken = true;

			if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken)
			{
#ifdef _CEBUILD
				if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue])
				{			
					ClosePopup(1.5);
				}
#else
				ClosePopup(1.5);
#endif 
			}
		}
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced && WaterRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6hot");
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken && GetObjectByID("Key-UnderWater")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6bare");
		}
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue] && MemObjRect.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("MemObj")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L6S4MotherStatue, &GetObjectByID("MemObj")->Pos);
			if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken && ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken)
			{
				ClosePopup(1);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue] && MemObjRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("MemObj")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L6S4MotherStatue, &(GetObjectByID("MemObj")->Pos));
		}
	}
#endif 
}

void L6S4_Pop2HotSpring::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L6_S2_StarShapedKeyInIce:
		if (!ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p2_iceMelt");
			GetObjectByID("WaterPopup__0017_IceWithKey")->SetEnable(true);

			Janim_ice_melt->PlayAnim();

			ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S2_StarShapedKeyInIce] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S2_StarShapedKeyInIce);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L6_S1_RagCloth:
		if (ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p2_takingStarKey");
			isCollectingInventory = true;
			Janim_keyTake->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_RagCloth] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S1_RagCloth);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L6S4_Pop2HotSpring::OnAnimComplete(int animID)
{
	isCollectingInventory = false;
	GetObjectByID("Key-UnderWater")->SetEnable(false);
	Hud->TakeInventoryItem(eInv_L6_S4_StarShapedKey);
	ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken = true;

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken)
	{
#ifdef _CEBUILD
		if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue])
		{			
			ClosePopup(1.5);
		}
#else
		ClosePopup(1.5);
#endif 
	}
}

float L6S4_Pop2HotSpring::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S4_Pop2HotSpring::~L6S4_Pop2HotSpring()
{
	SAFE_DELETE(SnowFX_L);
	SAFE_DELETE(Janim_ice_melt);
	SAFE_DELETE(Janim_keyTake);
	SAFE_DELETE(s4_HotSpringPopupBubblesFX);
}

void L6S4_Pop2HotSpring::Required()
{

}

void L6S4_Pop2HotSpring::OnPopupOpened()
{
	SnowFX_L->Start();
	SnowFX_L->AdvanceByTime(2);

	s4_HotSpringPopupBubblesFX->Start();
	s4_HotSpringPopupBubblesFX->AdvanceByTime(2.0f);



	Popup::OnPopupOpened();
}

void L6S4_Pop2HotSpring::ClosePopup(float delayTime)
{
	SnowFX_L->StopImmediate();
	s4_HotSpringPopupBubblesFX->StopImmediate();
	Popup::ClosePopup(delayTime);
}

bool L6S4_Pop2HotSpring::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken ||
		(ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced) ||
		(ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced && ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L6S4_Pop2HotSpring::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnifyingGlassTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("glass")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle glassObjRect =GetObjectByID("glass")->GetWorldRect();
		Hud->InitHintTrailEmit(&glassObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced)
	{
		Hud->ShowHintForInvItem(eInv_L6_S2_StarShapedKeyInIce,&WaterRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced && ProfileSystem::Instance->ActiveProfile.L6_S1_InvClothTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken)
	{
		//Hud->ShowHintForInvItem(eInv_L6_S1_RagCloth,&GetObjectByID("Key-UnderWater")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle keyUnderwaterObjRect =GetObjectByID("Key-UnderWater")->GetWorldRect();
		Hud->ShowHintForInvItem(eInv_L6_S1_RagCloth,&keyUnderwaterObjRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
