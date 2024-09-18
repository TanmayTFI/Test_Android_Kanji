//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 02 - POPUP 2: Skull
//====================================

#include "L5_S4_Dungeon.h"
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
#include "L7S1_Pop2Well.h"
#include "MeshObject.h"


const CRectangle  InvItemRect(385.0f,95.0f,160.0f,114.0f);
const CRectangle  InvRect(507.0f,191.0f,441.0f,346.0f);

const CRectangle  MemoryRectLocket(842.0f,321.0f,76.0f,115.0f);

const CRectangle  ActionRectSymbol1(661.0f,192.0f,95.0f,62.0f);
const CRectangle  ActionRectSymbol2(776.0f,207.0f,94.0f,80.0f);
const CRectangle  ActionRectSymbol3(833.0f,312.0f,85.0f,67.0f);
const CRectangle  ActionRectSymbol4(765.0f,411.0f,116.0f,69.0f);
const CRectangle  ActionRectSymbol5(660.0f,430.0f,94.0f,91.0f);
const CRectangle  ActionRectSymbol6(530.0f,391.0f,105.0f,94.0f);
const CRectangle  ActionRectSymbol7(501.0f,301.0f,100.0f,78.0f);
const CRectangle  ActionRectSymbol8(539.0f,218.0f,113.0f,74.0f);


L7S1_Pop2Well::L7S1_Pop2Well(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV7\\S1\\POP2\\P2.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	isAnimationPlaying = false;

	currentPattern = 0;
	intialPatternPlaying = false;
	patternTimer = 900;
	intialPatternPlayed = false;
	waterObj1 = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened)
	{
		GetObjectByID("doorOpen")->SetEnable(false);
		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed)
			GetObjectByID("wheel")->SetEnable(false);
	}
	else
	{
		GetObjectByID("wheel")->SetEnable(false);
		GetObjectByID("doorClosed")->SetEnable(false);
		GetObjectByID("symbols_1")->SetEnable(false);
		GetObjectByID("symbols_2")->SetEnable(false);
		GetObjectByID("symbols_3")->SetEnable(false);
		GetObjectByID("symbols_4")->SetEnable(false);
		GetObjectByID("symbols_5")->SetEnable(false);
		GetObjectByID("symbols_6")->SetEnable(false);
		GetObjectByID("symbols_7")->SetEnable(false);
		GetObjectByID("symbols_8")->SetEnable(false);

		waterObj1 = new MeshObject(GetObjectByID("wateranim"), 10, 10);
		waterObj1->meshFX->SetSpeedAndDisplacement(4, 4, 3, 3);
		InsertObject(waterObj1, 1);

		
	}

	if(ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotEmptyTaken)
		GetObjectByID("pot")->SetEnable(false);

	GetObjectByID("potWithChain")->SetEnable(false);
	GetObjectByID("potWater")->SetEnable(false);

	GetObjectByID("symbols_1_blue")->SetEnable(false);
	GetObjectByID("symbols_2_blue")->SetEnable(false);
	GetObjectByID("symbols_3_blue")->SetEnable(false);
	GetObjectByID("symbols_4_blue")->SetEnable(false);
	GetObjectByID("symbols_5_blue")->SetEnable(false);
	GetObjectByID("symbols_6_blue")->SetEnable(false);
	GetObjectByID("symbols_7_blue")->SetEnable(false);
	GetObjectByID("symbols_8_blue")->SetEnable(false);

	GetObjectByID("symbols_1_red")->SetEnable(false);
	GetObjectByID("symbols_2_red")->SetEnable(false);
	GetObjectByID("symbols_3_red")->SetEnable(false);
	GetObjectByID("symbols_4_red")->SetEnable(false);
	GetObjectByID("symbols_5_red")->SetEnable(false);
	GetObjectByID("symbols_6_red")->SetEnable(false);
	GetObjectByID("symbols_7_red")->SetEnable(false);
	GetObjectByID("symbols_8_red")->SetEnable(false);

	
	JAnim_Wheel = new JSONAnimator(0, CC_CALLBACK_1(L7S1_Pop2Well::OnAnimationComplete, this));
	JAnim_Wheel->parseAndLoadJSON("ANIM\\L7\\S1P2WHEEL.JSON", this);
	
	JAnim_Puzzle = new JSONAnimator(1, CC_CALLBACK_1(L7S1_Pop2Well::OnAnimationComplete, this));
	JAnim_Puzzle->parseAndLoadJSON("ANIM\\L7\\S1P2DOOROPEN.JSON", this);

	JAnim_Pot = new JSONAnimator(2, CC_CALLBACK_1(L7S1_Pop2Well::OnAnimationComplete, this));
	JAnim_Pot->parseAndLoadJSON("ANIM\\L7\\S1P2POT.JSON", this);

	Show(false);

}

void L7S1_Pop2Well::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		GetObjectByID("wheel")->SetEnable(true);
		isAnimationPlaying = false;
		Cursor::Show(true);
	}
	else if(animID == 1)
	{
		isAnimationPlaying = false;
		ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened = true;
		Hud->InvBox_StopPuzzleMode();
		Cursor::Show(true);
	}
	else if(animID == 2)
	{
		isAnimationPlaying = false;
		GetObjectByID("potWithChain")->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken = true;
		Hud->TakeInventoryItem(eInv_L7_S1_PotWithMagicWater, GetObjectByID("potWithChain")->Pos);
		ClosePopup(1.0f);
	}
}

void L7S1_Pop2Well::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L7_S4_WellGear &&  InvRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed )
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S4_WellGear] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S4_WellGear);
			JAnim_Wheel->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s1p2_gearused");
			isAnimationPlaying = true;
		}		
		return;
	}
	else if (itemVal == eInv_L7_S1_PotWithChain &&  InvRect.Intersection(&dropPos))
	{

		if(ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened && !ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken )
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S1_PotWithChain] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S1_PotWithChain);
			JAnim_Pot->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s1p2_watertaken");
			isAnimationPlaying = true;
			return;
		}
		
		
	}
	else if (itemVal == eInv_L7_S1_PotEmpty &&  InvRect.Intersection(&dropPos))
	{

		if(ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened && !ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken )
		{
			Hud->ShowBannerText("L7pot");
			return;
		}
		
		
	}
	else
					GFInstance->WrongInvDropMsg(Hud);
}

void L7S1_Pop2Well::Required()
{

}

void L7S1_Pop2Well::Update(float ds)
{
		Popup::Update(ds);

		if(GetPopupState()!= ePopupState_Open || Hud->IsDraggingItem() || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
			return;

		if(intialPatternPlaying)
		{
			//patternTimer -= Control::LogicRate;

			patternTimer--;
			if(patternTimer %100 == 0 )
			{
				GetObjectByID("symbols_1_blue")->SetEnable(false);
				GetObjectByID("symbols_2_blue")->SetEnable(false);
				GetObjectByID("symbols_3_blue")->SetEnable(false);
				GetObjectByID("symbols_4_blue")->SetEnable(false);
				GetObjectByID("symbols_5_blue")->SetEnable(false);
				GetObjectByID("symbols_6_blue")->SetEnable(false);
				GetObjectByID("symbols_7_blue")->SetEnable(false);
				GetObjectByID("symbols_8_blue")->SetEnable(false);

				GetObjectByID("symbols_1_red")->SetEnable(false);
				GetObjectByID("symbols_2_red")->SetEnable(false);
				GetObjectByID("symbols_3_red")->SetEnable(false);
				GetObjectByID("symbols_4_red")->SetEnable(false);
				GetObjectByID("symbols_5_red")->SetEnable(false);
				GetObjectByID("symbols_6_red")->SetEnable(false);
				GetObjectByID("symbols_7_red")->SetEnable(false);
				GetObjectByID("symbols_8_red")->SetEnable(false);
			}

			if(patternTimer == 800)
			{
				GetObjectByID("symbols_6_blue")->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
			}
			else if(patternTimer == 700)
			{
				GetObjectByID("symbols_7_blue")->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
			}
			else if(patternTimer == 600)
			{
				GetObjectByID("symbols_4_blue")->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
			}
			else if(patternTimer == 500)
			{
				GetObjectByID("symbols_2_blue")->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
			}
			else if(patternTimer == 400)
			{
				GetObjectByID("symbols_8_blue")->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
			}
			else if(patternTimer == 300)
			{
				GetObjectByID("symbols_3_blue")->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
			}
			else if(patternTimer == 200)
			{
				GetObjectByID("symbols_5_blue")->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
			}
			else if(patternTimer == 100)
			{
				GetObjectByID("symbols_1_blue")->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
				
			}
			else if(patternTimer == 0)
			{
				intialPatternPlaying = false;
				isAnimationPlaying = false;
				intialPatternPlayed = true;
				patternTimer = 900;
			}
			
		}
		if(isAnimationPlaying)
		{
			Cursor::SetMode(CA_Normal);
			return;
		}

		if(!isMouseOverCloseButton)
			Cursor::SetMode(CA_Normal);

		CPoint pos = Control::Input->Mpos();

		if(intialPatternPlayed)
		{
			if(ActionRectSymbol6.Intersection(&pos) && currentPattern < 2)
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked())
				{
					if(currentPattern == 1)
					{
						GetObjectByID("symbols_6_blue")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
						currentPattern ++;
					}
					else
					{
						GetObjectByID("symbols_6_red")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedwrong");
						currentPattern = 1;
						intialPatternPlaying = true;
						isAnimationPlaying = true;
						intialPatternPlayed = false;
					}
				}
			}
			if(ActionRectSymbol7.Intersection(&pos) && currentPattern < 3)
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked())
				{
					if(currentPattern == 2)
					{
						GetObjectByID("symbols_7_blue")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
						currentPattern ++;
					}
					else
					{
						GetObjectByID("symbols_7_red")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedwrong");
						currentPattern = 1;
						intialPatternPlaying = true;
						isAnimationPlaying = true;
						intialPatternPlayed = false;
					}
				}
			}
			if(ActionRectSymbol4.Intersection(&pos) && currentPattern < 4)
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked())
				{
					if(currentPattern == 3)
					{
						GetObjectByID("symbols_4_blue")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
						currentPattern ++;
					}
					else
					{
						GetObjectByID("symbols_4_red")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedwrong");
						currentPattern = 1;
						intialPatternPlaying = true;
						isAnimationPlaying = true;
						intialPatternPlayed = false;
					}
				}
			}
			if(ActionRectSymbol2.Intersection(&pos) && currentPattern < 5)
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked())
				{
					if(currentPattern == 4)
					{
						GetObjectByID("symbols_2_blue")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
						currentPattern ++;
					}
					else
					{
						GetObjectByID("symbols_2_red")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedwrong");
						currentPattern = 1;
						intialPatternPlaying = true;
						isAnimationPlaying = true;
						intialPatternPlayed = false;
					}
				}
			}
			if(ActionRectSymbol8.Intersection(&pos) && currentPattern < 6)
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked())
				{
					if(currentPattern == 5)
					{
						GetObjectByID("symbols_8_blue")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
						currentPattern ++;
					}
					else
					{
						GetObjectByID("symbols_8_red")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedwrong");
						currentPattern = 1;
						intialPatternPlaying = true;
						isAnimationPlaying = true;
						intialPatternPlayed = false;
					}
				}
			}
			if(ActionRectSymbol3.Intersection(&pos) && currentPattern < 7)
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked())
				{
					if(currentPattern == 6)
					{
						GetObjectByID("symbols_3_blue")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
						currentPattern ++;
					}
					else
					{
						GetObjectByID("symbols_3_red")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedwrong");
						currentPattern = 1;
						intialPatternPlaying = true;
						isAnimationPlaying = true;
						intialPatternPlayed = false;
					}
				}
			}
			if(ActionRectSymbol5.Intersection(&pos) && currentPattern < 8)
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked())
				{
					if(currentPattern == 7)
					{
						GetObjectByID("symbols_5_blue")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
						currentPattern ++;
					}
					else
					{
						GetObjectByID("symbols_5_red")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedwrong");
						currentPattern = 1;
						intialPatternPlaying = true;
						isAnimationPlaying = true;
						intialPatternPlayed = false;
					}
				}
			}
			if(ActionRectSymbol1.Intersection(&pos) && currentPattern < 9)
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked())
				{
					if(currentPattern == 8)
					{
						GetObjectByID("symbols_1_blue")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedright");
						currentPattern ++;
						JAnim_Puzzle->PlayAnim();
						Control::Audio->QuickLoadAndPlaySFX("s1p2_wellopen");
					}
					else
					{
						GetObjectByID("symbols_1_red")->SetEnable(true);
						Control::Audio->QuickLoadAndPlaySFX("s1p2_symbolclickedwrong");
						currentPattern = 1;
						intialPatternPlaying = true;
						isAnimationPlaying = true;
						intialPatternPlayed = false;
					}
				}
			}
		}
		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed && InvRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L7some");
			}
		}
		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken && ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened && InvRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L7deep");
			}
		}
		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotEmptyTaken && InvItemRect.Intersection(&pos))
		{
			if(!(ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed && !ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					GetObjectByID("pot")->SetEnable(false);
					ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotEmptyTaken= true;
					Hud->TakeInventoryItem(eInv_L7_S1_PotEmpty, GetObjectByID("pot")->Pos);

				}
			}
		}
		else if(ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed && !ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened)	
		{
			/*JAnim_Puzzle->PlayAnim();
			isAnimationPlaying = true;*/

			if(currentPattern == 0)
			{
				currentPattern ++;
				intialPatternPlaying = true;
				isAnimationPlaying = true;
				Hud->InvBox_StartPuzzleMode();
			}
		}
				
}

float L7S1_Pop2Well::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L7S1_Pop2Well::~L7S1_Pop2Well()
{
	SAFE_DELETE(JAnim_Wheel);
	SAFE_DELETE(JAnim_Puzzle); 
	SAFE_DELETE(JAnim_Pot);
}

void L7S1_Pop2Well::TakeInventoryItem(EHOInventory ItemID)
{
	/*std::vector<int> vecInvIds;
	switch (ItemID)
	{
	
	default:
		break;
	}*/
}

void L7S1_Pop2Well::ReceiveMessage(int val)
{	

	
}

bool L7S1_Pop2Well::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken &&  ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotWithChainTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed &&  ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotEmptyTaken  )
		isAnyTaskLeft = true;
		
	return isAnyTaskLeft;
}

void L7S1_Pop2Well::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken &&  ProfileSystem::Instance->ActiveProfile.L7_S1_WellLidOpened && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotWithChainTaken)
		Hud->ShowHintForInvItem(eInv_L7_S1_PotWithChain, &InvRect);
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvWheelUsed &&  ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken )
		Hud->ShowHintForInvItem(eInv_L7_S4_WellGear, &InvRect);
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotEmptyTaken  )
		Hud->InitHintTrailEmit(InvItemRect.GetCenterX(), InvItemRect.GetCenterY());
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

void L7S1_Pop2Well::ProcessSkip()
{
	JAnim_Puzzle->PlayAnim();
}



