//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 5: SCRAPTER
//====================================

#include "L2S1_Pop3AloeveraPlant.h"
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

const CRectangle  InvItemRectStoneBall(457.0f,382.0f,183.0f,180.0f);
const CRectangle  InvDestRectKnife(511.0f,143.0f,366.0f,278.0f);


L2S1_Pop3AloeveraPlant::L2S1_Pop3AloeveraPlant(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)

{

	//Popup load
	Load("LV2\\S1\\POP3\\P3.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	//pondWater = NULL;
	//pondWater = new MeshObject("LV2\\S1\\POP3\\PondWaterAnim.PNG", 5, 5);
	//pondWater->SetPosAndParams(683-244,334-210, 2, 4, 2, 3);
	//pondWater->SetZ(1);
	////InsertObject(pondWater, 1);
	//pondWater->SetEnable(false);

	pondWater = NULL;
	pondWater = new MeshObject(GetObjectByID("PondWaterAnim"), 5, 5);
	pondWater->meshFX->SetSpeedAndDisplacement(2, 4, 2, 3);
	InsertObject(pondWater, 1);
	pondWater->SetEnable(false);

	knife = GetObjectByID("Knife01");
	stoneBall = GetObjectByID("StoneBall");
	leafSet = GetObjectByID("Leaves1");
	bigLeaf = GetObjectByID("Leaves02");
	aloeveraLeafTop = GetObjectByID("AloeVera_Leaf01");
	aloeveraLeafBotttom = GetObjectByID("AloeVera_Leaf02");
	gloveWithLeaf = GetObjectByID("GlovesWitLeaf");
	gloveWithoutLeaf = GetObjectByID("GlovesWithoutLeaf");

	knife->SetEnable(false);
	leafSet->SetPivot(102,64);
	leafSet->Animate(0,0,2,0,5,2);
	bigLeaf->SetPivot(84,75);
	bigLeaf->Animate(0,0,0.9f,0,5,2);

	if (ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
	{
		stoneBall->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S1_IsAloveraSliced)
	{
		aloeveraLeafTop->SetEnable(false);
		aloeveraLeafBotttom->SetEnable(false);
	}

	gloveWithLeaf->SetEnable(false);
	gloveWithoutLeaf->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && !ProfileSystem::Instance->ActiveProfile.L2_S1_IsAloveraSliced)
	{
		gloveWithLeaf->SetEnable(true);
	}

	_isTweening = false;
}

void L2S1_Pop3AloeveraPlant::OnScaleUpComplete()
{
	pondWater->SetEnable(true);

	Popup::OnScaleUpComplete();
}

void L2S1_Pop3AloeveraPlant::ClosePopup(float delayTime)
{
	if (_isTweening)
		return;
	pondWater->SetEnable(false);

	Popup::ClosePopup();
}


void L2S1_Pop3AloeveraPlant::Required()
{

}

void L2S1_Pop3AloeveraPlant::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
	{
		if( InvItemRectStoneBall.Intersection( &pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				stoneBall->SetEnable(false);
				TakeInventoryItem(eInv_L2S1_RoundStone);
				Hud->ShowBannerText("L2stone");
				return;
			}		
		}
	}

	if (InvDestRectKnife.Intersection(&pos) )
	{
		if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
		{
			Cursor::SetMode(CA_Gear);
			if (!ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed)
			{
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L2thorns");
					return;
				}
			}
			else
			{
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L2ease");
					return;
				}
			}
		}
	}
}

float L2S1_Pop3AloeveraPlant::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S1_Pop3AloeveraPlant::~L2S1_Pop3AloeveraPlant()
{

}

void L2S1_Pop3AloeveraPlant::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{

	case eInv_L2S1_RoundStone:
		Hud->InvBoxCtrl->Activate(1);			
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_RoundStone] = true;
		ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken = true;			
		vecInvIds.push_back(eInv_L2S1_RoundStone);
		Hud->ShowInventory(&vecInvIds);
		if (ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
		{
			ClosePopup();
		}
		break;

	case eInv_L2S1_Aloevera:
		Hud->InvBoxCtrl->Activate(1);			
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_Aloevera] = true;
		ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken = true;
		vecInvIds.push_back(eInv_L2S1_Aloevera);
		Hud->ShowInventory(&vecInvIds);
		gloveWithLeaf->SetEnable(false);
		knife->SetEnable(false);
		if (ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
		{
			ClosePopup(); 
		}
		break;

	default:
		break;
	}
}

void L2S1_Pop3AloeveraPlant::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (InvDestRectKnife.Intersection(&dropPos))
	{
		if (itemVal == eInv_L2S6_Gloves)
		{
			if (!ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed)
			{
				gloveWithoutLeaf->SetEnable(true);

				float valArray1[5] = {0, gloveWithoutLeaf->Pos.x, gloveWithoutLeaf->Pos.y, 1, 0};
				gloveWithoutLeaf->TweenTo(valArray1, 1.0f, 0, false);

				gloveWithLeaf->SetEnable(true);
				gloveWithLeaf->SetAlpha(0);

				float valArray2[5] = {1, gloveWithLeaf->Pos.x, gloveWithLeaf->Pos.y, 1, 0};
				gloveWithLeaf->TweenTo(valArray2, 1.0f, 0, false);

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed = true;
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_Gloves] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_Gloves);
				Control::Audio->QuickLoadAndPlaySFX("s1p3_glove");
			} 
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
		else if (itemVal == eInv_L2S6_KnifeSharpen)
		{
			if (!ProfileSystem::Instance->ActiveProfile.L2_S1_IsAloveraSliced)
			{
				if (ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed)
				{

					knife->SetEnable(true);
					aloeveraLeafBotttom->SetEnable(false);
					aloeveraLeafTop->SetEnable(false);
					float valArray1[5] = {1, knife->Pos.x, knife->Pos.y + 60, 1, 0};
					knife->TweenTo(valArray1, 1.00f, 0, false);
					gloveWithLeaf->SetPivot(23.5, 75.5);
					float valArray2[5] = {1, gloveWithLeaf->Pos.x, gloveWithLeaf->Pos.y, 1, 30};
					gloveWithLeaf->TweenTo(valArray2, 1.50f, 0, true,CC_CALLBACK_1(L2S1_Pop3AloeveraPlant::OnTweenComplete,this));
					_isTweening = true;

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					if (ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced)
					{
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_KnifeSharpen] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_KnifeSharpen);
					}

					ProfileSystem::Instance->ActiveProfile.L2_S1_IsAloveraSliced = true; 
					Control::Audio->QuickLoadAndPlaySFX("s1p3_aloeveracut");
				}
				else
				{
					Hud->ShowBannerText("L2support");
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
				}
			} 
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L2S1_Pop3AloeveraPlant::OnTweenComplete(CObject *obj)
{
	_isTweening = false;
	TakeInventoryItem(eInv_L2S1_Aloevera);
	/*aloeveraLeafBotttom->SetPivot(52, 36);
	float valArray3[5] = {1, aloeveraLeafBotttom->Pos.x, aloeveraLeafBotttom->Pos.y, 1, 30};
	aloeveraLeafBotttom->TweenTo(valArray3, 1.50f, 0, true,CC_CALLBACK_1(L2S1_Pop3AloeveraPlant::OnSecondTweenComplete,this));*/
}

void L2S1_Pop3AloeveraPlant::OnSecondTweenComplete(CObject *obj)
{
	/*knife->SetEnable(false);
	aloeveraLeafBotttom->SetEnable(false);
	aloeveraLeafTop->SetEnable(false);
	TakeInventoryItem(eInv_L2S1_Aloevera);*/
}

bool L2S1_Pop3AloeveraPlant::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken && ProfileSystem::Instance->ActiveProfile.L2S6_InvGlovesTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken && ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S1_Pop3AloeveraPlant::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
	{
		Hud->InitHintTrailEmit(InvItemRectStoneBall.GetCenterX(), InvItemRectStoneBall.GetCenterY());
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken && ProfileSystem::Instance->ActiveProfile.L2S6_InvGlovesTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed)
	{
		Hud->ShowHintForInvItem(eInv_L2S6_Gloves, &InvDestRectKnife);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken && ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S6_KnifeSharpen, &InvDestRectKnife);		
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
