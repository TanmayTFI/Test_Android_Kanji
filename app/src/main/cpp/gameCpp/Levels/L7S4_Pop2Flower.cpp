//====================================
//	DQFYLH
//	HO System
//	Level 7 - Scene 04 - POPUP 2: Flower
//====================================

#include "L7_S4_Ameeha.h"
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

#include "L7S4_Pop2Flower.h"

const CRectangle  InvDestRect(539.0f,100.0f,331.0f,432.0f);
const CRectangle  StarDestRect(640.0f,130.0f,89.0f,101.0f);

L7S4_Pop2Flower::L7S4_Pop2Flower(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV7\\S4\\POP2\\P1.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	std::vector<SuperFX*> superfxArray;
	S7P1GliterprtFX = new SuperFX("PRT\\L7\\s1p1FlowerPrt.INI", PointSystem::CreateCPoint(683,384,0), 1100); 
	superfxArray.push_back(S7P1GliterprtFX);

	JAnim_Star = new JSONAnimator(0, CC_CALLBACK_1(L7S4_Pop2Flower::OnAnimationComplete, this));
	JAnim_Star->parseAndLoadJSON("ANIM\\L7\\S1P1CASEUNLOK.JSON", this);

	JAnim_Flower = new JSONAnimator(1, CC_CALLBACK_1(L7S4_Pop2Flower::OnAnimationComplete, this));
	JAnim_Flower->parseAndLoadJSON("ANIM\\L7\\S1P1FLOWEROPEN.JSON", this, &superfxArray);

	GetObjectByID("redFlower")->SetEnable(false);
	GetObjectByID("caseDoorOpen")->SetEnable(false);
	GetObjectByID("flowerOpen")->SetEnable(false);
	GetObjectByID("flowerOpenEffect")->SetEnable(false);
	GetObjectByID("flowerOpenSparkles")->SetEnable(false);
	GetObjectByID("pot")->SetEnable(false);
	GetObjectByID("redFlower")->SetEnable(false);
	GetObjectByID("redFlowerHighlight")->SetEnable(false);
	GetObjectByID("water")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueStarUsed)
	{
		GetObjectByID("redFlower")->SetEnable(true);
		GetObjectByID("caseDoorOpen")->SetEnable(true);
		GetObjectByID("caseDoor")->SetEnable(false);
	}

	

	Show(false);

}

void L7S4_Pop2Flower::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		isAnimationPlaying = false;
		GetObjectByID("redFlower")->SetEnable(true);
		GetObjectByID("caseDoorOpen")->SetEnable(true);
		GetObjectByID("caseDoor")->SetEnable(false);
		
	}
	else if(animID == 1)
	{
		isAnimationPlaying = false;
		GetObjectByID("flowerOpen")->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueFlowerTaken = true;
		Hud->TakeInventoryItem(eInv_L7_S4_BlueFlower);
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] = true;
		ClosePopup(1.0f);
	}
	
}

void L7S4_Pop2Flower::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}
	if (itemVal == eInv_L7_S1_BlueStarEmblem &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueStarUsed)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueStarUsed= true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S1_BlueStarEmblem] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S1_BlueStarEmblem);
			Control::Audio->QuickLoadAndPlaySFX("s4p2_starused");
			JAnim_Star->PlayAnim();
			isAnimationPlaying = true;
		}		
		return;
	}
	else if (itemVal == eInv_L7_S1_PotWithMagicWater &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L7_S4_InvWaterUsedOnBlueFlower && ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueStarUsed)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L7_S4_InvWaterUsedOnBlueFlower= true;
			if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedFlowerTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S1_PotWithMagicWater] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S1_PotWithMagicWater);
			}
			Control::Audio->QuickLoadAndPlaySFX("s4p2_waterused");
			JAnim_Flower->PlayAnim();
			isAnimationPlaying = true;
		}
		else 
		{
			Hud->ShowBannerText("L7must");
		}
		return;
	}
	else
					GFInstance->WrongInvDropMsg(Hud);
}

void L7S4_Pop2Flower::Required()
{

}

void L7S4_Pop2Flower::Update(float ds)
{
		Popup::Update(ds);
		if(GetPopupState()!= ePopupState_Open )
		return;

		
		if( Hud->IsDraggingItem()  )
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();

	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_InvWaterUsedOnBlueFlower && ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueStarUsed && InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L7flower");
		}
	}
	
	if( !ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueStarUsed && InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L7miss");
		}
	}
	
}

float L7S4_Pop2Flower::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L7S4_Pop2Flower::~L7S4_Pop2Flower()
{
	SAFE_DELETE(JAnim_Star);
	SAFE_DELETE(JAnim_Flower);
	SAFE_DELETE(S7P1GliterprtFX);
}

void L7S4_Pop2Flower::TakeInventoryItem(EHOInventory ItemID)
{
	/*std::vector<int> vecInvIds;
	switch (ItemID)
	{
	
	default:
		break;
	}*/
}

void L7S4_Pop2Flower::ReceiveMessage(int val)
{	

	
}

bool L7S4_Pop2Flower::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueStarUsed && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[2])
		isAnyTaskLeft = true;
	else if( !ProfileSystem::Instance->ActiveProfile.L7_S4_InvWaterUsedOnBlueFlower && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
		isAnyTaskLeft = true;
		

	return isAnyTaskLeft;
}

void L7S4_Pop2Flower::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if( !ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueStarUsed && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[2])
		Hud->ShowHintForInvItem(eInv_L7_S1_BlueStarEmblem, &StarDestRect);
	else if( !ProfileSystem::Instance->ActiveProfile.L7_S4_InvWaterUsedOnBlueFlower && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
		Hud->ShowHintForInvItem(eInv_L7_S1_PotWithMagicWater, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
