//====================================
//	DQFYLH
//	HO System
//	Level 7 - Scene 01 - POPUP 1: Flower
//====================================

#include "L7_S1_Volcano.h"
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

#include "L7S1_Pop1Flower.h"

const CRectangle  InvDestRect(539.0f,100.0f,331.0f,432.0f);
const CRectangle  StarDestRect(640.0f,130.0f,89.0f,101.0f);


L7S1_Pop1Flower::L7S1_Pop1Flower(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV7\\S1\\POP1\\P1.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	std::vector<SuperFX*> superfxArray;
	S7P1GliterprtFX = new SuperFX("PRT\\L7\\s1p1FlowerPrt.INI", PointSystem::CreateCPoint(683,384,0), 1100); 
	superfxArray.push_back(S7P1GliterprtFX);

	JAnim_Star = new JSONAnimator(0, CC_CALLBACK_1(L7S1_Pop1Flower::OnAnimationComplete, this));
	JAnim_Star->parseAndLoadJSON("ANIM\\L7\\S1P1CASEUNLOK.JSON", this);

	JAnim_Flower = new JSONAnimator(1, CC_CALLBACK_1(L7S1_Pop1Flower::OnAnimationComplete, this));
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

	if(ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedStarUsed)
	{
		GetObjectByID("redFlower")->SetEnable(true);
		GetObjectByID("caseDoorOpen")->SetEnable(true);
		GetObjectByID("caseDoor")->SetEnable(false);
	}

	

	Show(false);

}

void L7S1_Pop1Flower::OnAnimationComplete(int animID)
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
		ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedFlowerTaken = true;
		Hud->TakeInventoryItem(eInv_L7_S1_RedFlower);
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] = true;
		ClosePopup(1.0f);
	}
	
}

void L7S1_Pop1Flower::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}
	if (itemVal == eInv_L7_S1_RedStarEmblem &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedStarUsed)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedStarUsed= true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S1_RedStarEmblem] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S1_RedStarEmblem);
			JAnim_Star->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s1p1_staruse");
			isAnimationPlaying = true;
		}		
		return;
	}

	else if (itemVal == eInv_L7_S1_PotWithMagicWater &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_WaterUsedOnRedFlower && ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedStarUsed)
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L7_S1_WaterUsedOnRedFlower= true;
			if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueFlowerTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S1_PotWithMagicWater] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S1_PotWithMagicWater);
			}
			JAnim_Flower->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s1p1_waterused");
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

void L7S1_Pop1Flower::Required()
{

}

void L7S1_Pop1Flower::Update(float ds)
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

	if(!ProfileSystem::Instance->ActiveProfile.L7_S1_WaterUsedOnRedFlower && ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedStarUsed && InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L7flower");
		}
	}
	
	if( !ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedStarUsed && InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L7miss");
		}
	}
}

float L7S1_Pop1Flower::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L7S1_Pop1Flower::~L7S1_Pop1Flower()
{
	SAFE_DELETE(JAnim_Star);
	SAFE_DELETE(JAnim_Flower);
	SAFE_DELETE(S7P1GliterprtFX);
}

void L7S1_Pop1Flower::TakeInventoryItem(EHOInventory ItemID)
{
	/*std::vector<int> vecInvIds;
	switch (ItemID)
	{
	
	default:
		break;
	}*/
}

void L7S1_Pop1Flower::ReceiveMessage(int val)
{	

	
}

bool L7S1_Pop1Flower::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedStarUsed && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[1])
		isAnyTaskLeft = true;
	else if( !ProfileSystem::Instance->ActiveProfile.L7_S1_WaterUsedOnRedFlower && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L7S1_Pop1Flower::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if( !ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedStarUsed && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[1])
		Hud->ShowHintForInvItem(eInv_L7_S1_RedStarEmblem, &StarDestRect);
	else if( !ProfileSystem::Instance->ActiveProfile.L7_S1_WaterUsedOnRedFlower && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
		Hud->ShowHintForInvItem(eInv_L7_S1_PotWithMagicWater, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
