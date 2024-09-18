//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 03 - POPUP 1: Honey COMB
//====================================

#include "L4S1_Pop2HealingObjects.h"
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

const CRectangle  PopAreaRect(0.0f,0.0f,1366.0f,768.0f);
const CRectangle  InvDestLeafSpoonRect(710.0f,179.0f,292.0f,366.0f);

const CRectangle  MemRectPumpkin(414.0f,441.0f,87.0f,89.0f);
const CRectangle  MedallionRect(692.0f,146.0f,107.0f,91.0f);


int ArrayPts[] = {679,223,682,189,698,156,762,167,810,160,857,186,1003,241,1003,560,966,549,950,562,806,534,664,569,666,524,707,504,726,484,816,411,802,325,771,322,764,290,739,279,704,219,676,222};
int ArrayPts1[] = {500,431,601,403,690,425,634,518,559,493,509,464,499,433};
int ArrayPts2[] = {638,343,633,353,559,359,495,401,475,454,436,476,423,564,392,555,402,473,421,402,451,337,472,378,476,331,528,323,540,345,635,341};
int ArrayPts3[] = {573,417,658,394,654,336,667,385,685,384,689,339,700,384,724,396,765,400,756,440,715,447,703,459,718,483,707,503,680,494,655,476,633,477,605,484,580,450,573,419};
	
L4S1_Pop2HealingObjects::L4S1_Pop2HealingObjects(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	mo_water = NULL;
	//Popup load
	Load("LV4\\S1\\POP2\\POP2.SCN");
	SetFrameAndCloseButton(false);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("ThumbiWings03")->SetAnimSpeed(5.0f);
	skimmerBody = GetObjectByID("ThumbiBody");
	skimmerWings = GetObjectByID("ThumbiWings03");
	spoonWithWater = GetObjectByID("LeefSpoonwatter");
	spoonWithWater->SetZ(spoonWithWater->ZOrder + 200);
	spoonWithOutWater = GetObjectByID("LeafSpoon(Water)");
	GetObjectByID("Medallion")->Animate(1,2,0,0,3,2);

	mo_water = new MeshObject(GetObjectByID("PondWaterAnim"), 5, 5);
	mo_water->meshFX->SetSpeedAndDisplacement(2, 4, 2, 3);
	//PushObject(mo_water);
	InsertObject(mo_water,2);

	SetSparkleUpdate(false);
	Show(false);


	Janim_skimmerFly = new JSONAnimator(0,CC_CALLBACK_1(L4S1_Pop2HealingObjects::OnAnimComplete,this));
	Janim_skimmerFly->parseAndLoadJSON("ANIM\\L4\\THUMBI.JSON", this);

	Janim_takeWaterWithSpoon = new JSONAnimator(1,CC_CALLBACK_1(L4S1_Pop2HealingObjects::OnAnimComplete,this));
	Janim_takeWaterWithSpoon->parseAndLoadJSON("ANIM\\L4\\LEEFSPOONWATTER.JSON", this);

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvStickTaken)
	{
		GetObjectByID("stick_plant")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafTaken)
	{
		GetObjectByID("JackFruitTreeLeef")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken)
	{
		GetObjectByID("Plant")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_SkimmerFlyed)
	{
		skimmerBody->SetEnable(false);
		skimmerWings->SetEnable(false);
	}

	GetObjectByID("water_mask")->SetAlpha(0);

	for (int i = 1; i < 4; i++)
	{
		std::string str = "Bigplantanimation0";
		str+= std::to_string(i+1);
		switch (i)
		{
		case 0:
			GetObjectByID(str)->SetPivot(0,149);
			GetObjectByID(str)->Animate(0,0,-2,0,5,3);
			break;
		case 1:
			GetObjectByID(str)->Animate(3,0,0,0,7);
			break;
		case 2:
			GetObjectByID(str)->SetPivot(0,147);
			GetObjectByID(str)->Animate(0,0,1.5f,0,7,2);
			break;
		case 3:
			GetObjectByID(str)->SetPivot(0,165);
			GetObjectByID(str)->Animate(0,0,-1,0,4,4);
			break;
		case 4:
			GetObjectByID(str)->SetPivot(0,114);
			GetObjectByID(str)->Animate(0,0,1.2f,0,6);
			break;
		default:
			break;
		}
	}

	spoonWithWater->SetEnable(false);
	spoonWithOutWater->SetEnable(false);

	WaterRect =  new CPolygon(22);
	WaterRect->Init(ArrayPts);

	CPolygon *CPolyObjName=  new CPolygon(7);
	CPolyObjName->Init(ArrayPts1);
	popLeafChew = new L4S1_PopLeafChewing(0, Hud, NULL, zOrder + 200, CPolyObjName->center, CC_CALLBACK_1(L4S1_Pop2HealingObjects::OnPopupClosed,this));
	popLeafChew->SetPopupBoundingBox(PopAreaRect);
	CPolyObjName = NULL;

	popUpActive = false;
	isAnimPlaying = false;

	StickPoly=  new CPolygon(16);
	StickPoly->Init(ArrayPts2);

	PlantPoly =  new CPolygon(20);
	PlantPoly->Init(ArrayPts3);



	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin])
	{
		GetObjectByID("mempumpkin")->SetEnable(false);
	}
}

void L4S1_Pop2HealingObjects::OnPopupClosed(Popup * popup)
{
	popUpActive = false;

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken)
	{
#ifdef _CEBUILD
		if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin])
		{			
			ClosePopup(0);
		}
#else
		ClosePopup(0);
#endif 
	}
}

void L4S1_Pop2HealingObjects::OnAnimComplete(int animID)
{
	isAnimPlaying = false;

	switch (animID)
	{
	case 0:
		ProfileSystem::Instance->ActiveProfile.L4_S1_SkimmerFlyed = true;
		break;
	case 1:
		Hud->TakeInventoryItem(eInv_L4_S1_LeafSpoonWithWater);
		ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken = true;
		spoonWithWater->SetEnable(false);
		spoonWithOutWater->SetEnable(false);

		if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken)
		{
#ifdef _CEBUILD
			if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin])
			{			
				ClosePopup(1);
			}
#else
			ClosePopup(1);
#endif 
		}
		break;
	default:
		break;
	}
}

void L4S1_Pop2HealingObjects::ClosePopup(float delayTime)
{
	if (popUpActive)
		return;
	Popup::ClosePopup(delayTime);
}

void L4S1_Pop2HealingObjects::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L4_S1_LeafSpoon:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken && InvDestLeafSpoonRect.Intersection(&dropPos) && !isAnimPlaying)
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p2_fetchWaterWithSpoon");
			Janim_takeWaterWithSpoon->PlayAnim();
			isAnimPlaying = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_LeafSpoon] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_LeafSpoon);
		}
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L4S1_Pop2HealingObjects::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || popUpActive)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);


	if (((skimmerBody->GetWorldRect().Intersection (&pos)) || (skimmerWings->GetWorldRect().Intersection (&pos))) && !ProfileSystem::Instance->ActiveProfile.L4_S1_SkimmerFlyed && !isAnimPlaying)
	{
		Cursor::SetMode(CA_HandPointy);
		if (Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p2_dragonfly");
			Janim_skimmerFly->PlayAnim();
			isAnimPlaying = true;
		}
	}
	else if (StickPoly->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L4_S1_SkimmerFlyed && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvStickTaken)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("stick_plant")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L4_S1_InvStickTaken = true;
			Hud->TakeInventoryItem(eInv_L4_S1_Stick);
			Hud->ShowBannerText("L4attach");
		}
	}
	else if (GetObjectByID("JackFruitTreeLeef")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafTaken)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("JackFruitTreeLeef")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafTaken = true;
			Hud->TakeInventoryItem(eInv_L4_S1_Leaf);
			Hud->ShowBannerText("L4improvise");
		}
	}
	else if (MedallionRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4reach");
		}
	}
	else if (WaterRect->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L4spoon");
	}
	else if (PlantPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken && !popUpActive)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			popUpActive = true;
			GetObjectByID("Plant")->SetEnable(false);
			popLeafChew->OpenPopup();
		}
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin] && MemRectPumpkin.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("mempumpkin")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L4S1Pumpkin, &GetObjectByID("mempumpkin")->Pos);
			if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken)
			{
				ClosePopup(1);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin] && MemRectPumpkin.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("mempumpkin")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L4S1Pumpkin, &GetObjectByID("mempumpkin")->Pos);
		}
	}
#endif 
}

void L4S1_Pop2HealingObjects::Required()
{

}

float L4S1_Pop2HealingObjects::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S1_Pop2HealingObjects::~L4S1_Pop2HealingObjects()
{
	SAFE_DELETE(Janim_skimmerFly);
	SAFE_DELETE(Janim_takeWaterWithSpoon);
	SAFE_DELETE(WaterRect);
	SAFE_DELETE(popLeafChew);
	SAFE_DELETE(StickPoly);
	SAFE_DELETE(PlantPoly);
}

void L4S1_Pop2HealingObjects::ReceiveMessage(int val)
{	


}

bool L4S1_Pop2HealingObjects::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafTaken ||
		!ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken||
		(!ProfileSystem::Instance->ActiveProfile.L4_S1_SkimmerFlyed  && !isAnimPlaying) ||
		(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvStickTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_SkimmerFlyed)||
		(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonCollected))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S1_Pop2HealingObjects::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafTaken)
		Hud->InitHintTrailEmit(GetObjectByID("JackFruitTreeLeef")->GetWorldRect().GetCenterX(), GetObjectByID("JackFruitTreeLeef")->GetWorldRect().GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken)
		Hud->InitHintTrailEmit(PlantPoly->center.x, PlantPoly->center.y);
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S1_SkimmerFlyed  && !isAnimPlaying)
		Hud->InitHintTrailEmit(skimmerBody->GetCenterX(), skimmerBody->GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvStickTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_SkimmerFlyed)
		Hud->InitHintTrailEmit(StickPoly->center.x - 50, StickPoly->center.y);
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonCollected)
		Hud->ShowHintForInvItem(eInv_L4_S1_LeafSpoon,&InvDestLeafSpoonRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
