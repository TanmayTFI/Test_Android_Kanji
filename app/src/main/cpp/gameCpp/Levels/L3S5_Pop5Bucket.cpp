//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 02 - POPUP 2: Skull
//====================================

#include "L3_S2_FortInterior.h"
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
#include "L3S5_Pop5Bucket.h"

const CRectangle  InvDestRect(368.0f,99.0f,626.0f,458.0f);
const CRectangle  PopupMaskRect(190.0f,88.0f,818.0f,490.0f);
const CRectangle  InvRectHammerHandle(581.0f,381.0f,224.0f,89.0f);

const CRectangle  PaptusFlyRect(386.0f,162.0f,64.0f,59.0f);

L3S5_Pop5Bucket::L3S5_Pop5Bucket(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S5\\POP5\\P5.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Control::Audio->LoadSample(aVOL3MaryRefS5_1,	AudioVO);
	
	dialogSystem = NULL;
	isDialogActive = false;
	isAnimationPlaying = false;

	VideoStartTimer = 1.5f;

	objAddDelayTimer = -1;
	

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced)
	{
		GetObjectByID("clip")->SetEnable(false);
		GetObjectByID("greenStone")->SetEnable(false);
		GetObjectByID("greenWater")->SetEnable(false);
		GetObjectByID("clipUnderWater")->SetEnable(false);

	}
	else //greenstone placed but clip yet to be picked!! (and hence we open popup to pickclip)
	{
		GetObjectByID("clip")->SetEnable(false);
		if( ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P4_GrandmaTalkEnds] )
			GetObjectByID("clipUnderWater")->SetEnable(true);
		else
			GetObjectByID("clipUnderWater")->SetEnable(false);
		GetObjectByID("greenStone")->SetEnable(false);
		GetObjectByID("blueWater")->SetEnable(false);
	}
	GetObjectByID("annMary")->SetEnable(false);

	GetObjectByID("paptus")->SetBlendMode(eBlend_AlphaAdditive);
	if(ProfileSystem::Instance->ActiveProfile.IsPaptusL3S5Collected)
	{
		GetObjectByID("paptus")->SetEnable(false);
	}

	vector<SuperFX*> fxVector;
	S5RisingFxFX = new SuperFX("PRT\\L3\\S5RisingFx.INI", PointSystem::CreateCPoint(687,350,0), 1100); 
	fxVector.push_back(S5RisingFxFX);



	JAnim_AnnMary = new JSONAnimator(0, CC_CALLBACK_1(L3S5_Pop5Bucket::OnAnimationComplete, this));
	JAnim_AnnMary->parseAndLoadJSON("ANIM\\L3\\S5P5ANNMARY.JSON", this, &fxVector);
	Show(false);
}

void L3S5_Pop5Bucket::OnAnimationComplete(int animID)
{
	if(animID == 0)//annmary anim complete
	{		
		dialogSystem->ShowDialog(eDialog_L3S5_AnnMary);
		isDialogActive = true;
		Hud->AllowInput = false;
		Hud->HideHud(false);
		isAnimationPlaying = true;
		Cursor::Show(true);
	}
}

void L3S5_Pop5Bucket::initDialogSystem(DialogSystem *l_dialogSystem)
{
	isDialogActive = false;
	dialogSystem = l_dialogSystem;
}


void L3S5_Pop5Bucket::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)	
		return;
	
	
	if (itemVal == eInv_L3S1_GreenCrystal &&  InvDestRect.Intersection(&dropPos))
	{
				
		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced)
		{
			JAnim_AnnMary->PlayAnim();
			isAnimationPlaying = true;
			Control::Audio->QuickLoadAndPlaySFX("s5p5_greenstoneused");		

			ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced = true;
			//since greencrystalstoneplaced, time to close inventory box & clear from greencrystal stone!!
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S1_GreenCrystal] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S1_GreenCrystal);
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L3S5_Pop5Bucket::OnTweenComplete(CObject *obj)
{
	
}

void L3S5_Pop5Bucket::onDialogComplete()
{	
	isDialogActive = false;
	Hud->AllowInput = true;
	Hud->ShowHud(false);
	Hud->ShowBannerText("L3ultimate");
	Control::Audio->PlaySample(aVOL3MaryRefS5_1);

	GetObjectByID("clipUnderWater")->SetEnable(true);
	GetObjectByID("greenStone")->SetEnable(false);
	GetObjectByID("blueWater")->SetEnable(false);
	GetObjectByID("greenWater")->SetEnable(true);
	GetObjectByID("clip")->SetEnable(false);
	GetObjectByID("annMary")->SetEnable(false);
	
	

	
	//Show Objective completed
	if(  ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O3_UnravelWellRiddle] && 
		!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L3O3_UnravelWellRiddle] )
	{
		objAddDelayTimer = 2.0f;

		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L3O3_UnravelWellRiddle] = true;
		Hud->ShowObjectiveFX(eObj_L3O3_UnravelWellRiddle,true);
	}
	
	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P4_GrandmaTalkEnds])
	{
		Hud->UnlockCharacter(eChar5_Grandma, 4.0f);
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P4_GrandmaTalkEnds] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L3P4_GrandmaTalkEnds;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
	}
	isAnimationPlaying = false;
}



void L3S5_Pop5Bucket::Required()
{

}

void L3S5_Pop5Bucket::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(isDialogActive)
	{
		dialogSystem->Update();
		return;
	}

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(!ProfileSystem::Instance->ActiveProfile.IsPaptusL3S5Collected && PaptusFlyRect.Intersection(&pos))
	{
		if(Control::Input->LBclicked())
		{
			Control::Audio->PlaySample(aSFXMagic2);
			GetObjectByID("paptus")->FadeOut(1.25f);
			Hud->CollectMysteryFlower(eArea_L3_S5_Cliff, GetObjectByID("paptus")->Pos);
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced )
		{
			if(InvDestRect.Intersection(&pos))
			{
				
					Cursor::SetMode(CA_Gear);
					if( Control::Input->LBclicked())
					{
						Hud->ShowBannerText("L3summoner");
					}
				
			}
			return;
		}

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}
	//froced start grandma animation once popup opens!!
	else if( ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced && !ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P4_GrandmaTalkEnds] )
	{
		JAnim_AnnMary->PlayAnim();
		isAnimationPlaying = true;
	}

	if(objAddDelayTimer>0)
	{
		objAddDelayTimer -= Control::LogicRate;
		if(objAddDelayTimer<0)
		{
			objAddDelayTimer = -1;
			//add new Objective

			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O4_FindWayFromBrokenCastle] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O4_FindWayFromBrokenCastle] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
						= eObj_L3O4_FindWayFromBrokenCastle;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L3O4_FindWayFromBrokenCastle);
			}
		}
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	//Objective Add
	if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O3_UnravelWellRiddle] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O3_UnravelWellRiddle] = true;
		ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
				= eObj_L3O3_UnravelWellRiddle;
		//Increment Objectives
		ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
		Hud->ShowObjectiveFX(eObj_L3O3_UnravelWellRiddle);
	}
	
	if( ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvClipTaken )
	{
		if(InvDestRect.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked())
			{
				GetObjectByID("clipUnderWater")->SetEnable(false);
				Hud->TakeInventoryItem(eInv_L3S5_MetalClip, GetObjectByID("clip")->Pos);
				ProfileSystem::Instance->ActiveProfile.L3_S5_InvClipTaken = true;
				ClosePopup(1.0f);				
			}
		}
	}
	
}

float L3S5_Pop5Bucket::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S5_Pop5Bucket::~L3S5_Pop5Bucket()
{
	Control::Audio->UnloadSample(aVOL3MaryRefS5_1);
	SAFE_DELETE(JAnim_AnnMary);
	SAFE_DELETE(S5RisingFxFX);
}

void L3S5_Pop5Bucket::ReceiveMessage(int val)
{	

	
}

bool L3S5_Pop5Bucket::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvClipTaken && ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced)
		isAnyTaskLeft = true;
	 if(!ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced && ProfileSystem::Instance->ActiveProfile.L3_S1_InvGreenCrystalTaken) 
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L3S5_Pop5Bucket::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvClipTaken && ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced)
		Hud->InitHintTrailEmit(InvDestRect.GetCenterX(), InvDestRect.GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced && ProfileSystem::Instance->ActiveProfile.L3_S1_InvGreenCrystalTaken) 
		Hud->ShowHintForInvItem(eInv_L3S1_GreenCrystal, &InvDestRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
