//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 1: Honey COMB
//====================================

#include "L2S5_Pop7Moopan.h"
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



const CRectangle  MoopanRect(415.0f,97.0f,505.0f,467.0f);


L2S5_Pop7Moopan::L2S5_Pop7Moopan(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	isVideoPlaying = false;
	isAnimationPlaying		= false;

	Load("LV2\\S5\\POP7\\POP7.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	
	
	Show(false);

	
	InitSceneElements();
	int MoopanArray[] = {780,89,867,143,886,535,949,572,376,570,384,492,447,494,463,453,420,397,536,315,619,357,680,239,655,161};
	Pop7MoopanPoly =  new CPolygon(13);
	Pop7MoopanPoly->Init(MoopanArray);


	cv_Cutscn = NULL;

	time = 0;
	subIndex = 0;
	subMax = 4;
	vidTimer = 0;


	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("moopantheyyam1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("moopantheyyam2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("moopantheyyam3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("moopantheyyam4"));

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	cv_Cutscn = new CVideo("OGV\\L2\\S5MOOPPANTHEYYAM.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//cv_Cutscn->SetScale(1.334f);
	cv_Cutscn->SetScale(GFApp->video_aspectRatio);
	cv_Cutscn->SetSkipActive();

	SetSparkleUpdate(false);


	JAnim_MoopanTired = NULL;
	JAnim_MoopanTired = new JSONAnimator( 0, CC_CALLBACK_1(L2S5_Pop7Moopan::OnAnimationComplete, this ));
	JAnim_MoopanTired->parseAndLoadJSON("ANIM\\L2\\S5MOOPANTIRED.JSON", this );
	JAnim_MoopanTired->PlayAnim();

	MoopanTalk->PlayAnimation(0);
	MoopanTalk->SetEnable(false);
	JAnim_MoopanTalk = NULL;
	JAnim_MoopanTalk = new JSONAnimator( 1, CC_CALLBACK_1(L2S5_Pop7Moopan::OnAnimationComplete, this ));
	JAnim_MoopanTalk->parseAndLoadJSON("ANIM\\L2\\S5MOOPANTALKTIRED.JSON", this );
	JAnim_MoopanTalk->PlayAnim(true);

	JAnim_MoopanDrinking = NULL;
	JAnim_MoopanDrinking = new JSONAnimator( 2, CC_CALLBACK_1(L2S5_Pop7Moopan::OnAnimationComplete, this ));
	JAnim_MoopanDrinking->parseAndLoadJSON("ANIM\\L2\\S5MOOPANDRINKMEDICINE.JSON", this );
	//JAnim_MoopanDrinking->PlayAnim(true);
	JAnim_MoopanDrinking->SetEnableObjects(false);
}

void L2S5_Pop7Moopan::SetDialogSystem(DialogSystem *l_dialogSystem)
{
	isDialogActive = false;
	dialogSystem = l_dialogSystem;
}


void L2S5_Pop7Moopan::InitSceneElements()
{
	isSceneUpdated = false;
	isTalkingDialogComplete = false;

	MoopanTalk			 = GetObjectByID("MooppanTalk1");
	MoopanTiredArray[0]  = GetObjectByID("Body");
	MoopanTiredArray[1]  = GetObjectByID("Head");
	MoopanTiredArray[2]  = GetObjectByID("Leg");
	MoopanTiredArray[3]  = GetObjectByID("LowerBody");
	MoopanTiredArray[4]  = GetObjectByID("Mouth");
	MoopanTiredArray[5]  = GetObjectByID("PalmLeft");
	MoopanTiredArray[6]  = GetObjectByID("ForeArmLeft");
	MoopanTiredArray[7]  = GetObjectByID("UpperArmLeft");
	

	MoopanDrinkingArray[0]   = GetObjectByID("DrinkLeg");
	MoopanDrinkingArray[1]   = GetObjectByID("DrinkLowerBody");
	MoopanDrinkingArray[2]   = GetObjectByID("DrinkBody");
	MoopanDrinkingArray[3]   = GetObjectByID("DrinkHead1");
	MoopanDrinkingArray[4]   = GetObjectByID("DrinkHead2");
	MoopanDrinkingArray[5]   = GetObjectByID("DrinkUpperArmR");
	MoopanDrinkingArray[6]   = GetObjectByID("DrinkForeArmR");
	MoopanDrinkingArray[7]   = GetObjectByID("DrinkEyes");
	MoopanDrinkingArray[8]   = GetObjectByID("DrinkUpperArmL");
	MoopanDrinkingArray[9]   = GetObjectByID("DrinkForeArmL");
	MoopanDrinkingArray[10]  = GetObjectByID("DrinkPalmL");

	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
		for (int i = 0; i < 11; i++)
			MoopanDrinkingArray[i]->SetEnable(false); 
	else
		for (int i = 0; i < 8; i++)
			MoopanTiredArray[i]->SetEnable(false); 

}

void L2S5_Pop7Moopan::UpdateSceneElements()
{
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
	{
		if( !isSceneUpdated )
		{
			for (int i = 0; i < 11; i++)
			{
				MoopanDrinkingArray[i]->FadeIn(1.5f);
			}
			for (int i = 0; i < 8; i++)
			{
				MoopanTiredArray[i]->FadeOut(1.5f);
			}
			isSceneUpdated = true;
		}
	}

}

void L2S5_Pop7Moopan::OnDialogComplete(int index )
{


	switch (index)
	{
	case eDialog_L2S5SMoopanTired:		
				isAnimationPlaying = false;
				isDialogActive = false;
				Hud->EnableInput(true);
				Hud->ShowHud(true);
				isTalkingDialogComplete = true;
				MoopanTalk->SetEnable(false);
				MoopanTalk->PlayAnimation(0);

				//Objective Add
				if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O2_PrepareMedicine] )
				{
					ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O2_PrepareMedicine] = true;
					ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
					= eObj_L2O2_PrepareMedicine;
					//Increment Objectives
					ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
					Hud->ShowObjectiveFX(eObj_L2O2_PrepareMedicine);

					Control::Audio->PlaySample(aSFXMagic1);
				}

				Hud->UnlockCharacter(eChar15_Mooppan, 2.95f);
		break;
	case eDialog_L2S5SMoopanDrinking:	
				isAnimationPlaying = false;
				isDialogActive = false;
				Hud->AllowInput = false;
				Hud->HideHud();
				isVideoPlaying = true;
				isAnimationPlaying = true;
				cv_Cutscn->PlayVideo();
				Cursor::SetMode(CA_Normal);
				if( Control::Audio->IsPlaying(aTrackL2theme) )
						Control::Audio->StopSample(aTrackL2theme);
				if( Control::Audio->IsPlaying(aAmbs5MoopansDen) )		
					Control::Audio->StopSample(aAmbs5MoopansDen);

				Hud->UnlockCharacter(eChar15_Mooppan, 2.95f);
				
		break;
	default:
		break;
	}
}


void L2S5_Pop7Moopan::OnAnimationComplete(int id)
{
	switch (id)
	{
	case 0:
		JAnim_MoopanTired->PlayAnim();
		break;
	case 1:	
		break;
	case 2: 
		JAnim_MoopanDrinking->SetEnableObjects(false);
		JAnim_MoopanTired->PlayAnim(true);
		JAnim_MoopanTalk->PlayAnim(true);
		MoopanTalk->SetEnable(true);
		isDialogActive = true;
		dialogSystem->ShowDialog(eDialog_L2S5SMoopanDrinking);
		Hud->EnableInput(false);
		Hud->HideHud(true);
		break;
	}
}

void L2S5_Pop7Moopan::Required()
{

}

void L2S5_Pop7Moopan::Update( float ds )
{
	

	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(isDialogActive)
	{
		Cursor::SetMode(CA_Normal);
		if( dialogSystem->IsMoopanTalking )
		{
			if(MoopanTalk->GetAnimation()==0)
				MoopanTalk->PlayAnimation(1);
		}
		else
		{
			if(MoopanTalk->GetAnimation()==1)
				MoopanTalk->PlayAnimation(0);
		}
		dialogSystem->Update();
		return;
	}

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup ||  Hud->IsDraggingItem())
		return;


	if (isVideoPlaying )
	{
		Cursor::SetMode(CA_Normal);
		//Subtitles Code
		time = cv_Cutscn->GetTime();

#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");

		if (time > subtitles[subIndex].end)
		{
			if (subMax - 1 > subIndex)
				subIndex++;
		}

		if(
#ifndef FORCE_CLOSE_VID_END
			cv_Cutscn->isEndReached() ||
#endif
			vidTimer > 53)
		{
			subtitleText->SetText("");
			isVideoPlaying = false;
			cv_Cutscn->StopAndFadeOutVideo(2.0f);					
			Hud->EnableInput(true);
			Hud->ShowHud(true);
			Hud->SendUpdateSceneMessage();
			ClosePopup();
			
			if( !Control::Audio->IsPlaying(aAmbs5MoopansDen) )		
				Control::Audio->PlaySample(aAmbs5MoopansDen,true);
		} 
		return;
	}

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	
	
	if(Pop7MoopanPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan && !Hud->IsDraggingItem() )
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked())
		{
			if( ProfileSystem::Instance->ActiveProfile.L2_S5_ClickedOnMooppan )
			{
				Hud->ShowBannerText("L2med");
				return;
			}
			if( !isTalkingDialogComplete )
			{
				isAnimationPlaying = true;
				isDialogActive = true;
				MoopanTalk->SetEnable(true);
				dialogSystem->ShowDialog(eDialog_L2S5SMoopanTired);
				ProfileSystem::Instance->ActiveProfile.L2_S5_ClickedOnMooppan = true;
				Hud->EnableInput(false);
				Hud->HideHud(true);
			}
		}
		return;
	}
}

void L2S5_Pop7Moopan::ReceiveMessage(int val)
{	

	
}

float L2S5_Pop7Moopan::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S5_Pop7Moopan::~L2S5_Pop7Moopan()
{
	SAFE_DELETE(cv_Cutscn);
	SAFE_DELETE(JAnim_MoopanTired);
	SAFE_DELETE(JAnim_MoopanTalk);
	SAFE_DELETE(subtitleText);
}


void L2S5_Pop7Moopan::HandleInventoryDrop(CPoint dropPos,int itemVal )
{
	if( itemVal == eInv_L2S5_MedicineJar )
	{
		if(  !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
		{
			ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan = true;
			if( !isSceneUpdated )
			{
				isAnimationPlaying = true;
				UpdateSceneElements();
				MoopanTalk->SetEnable(false);
				JAnim_MoopanTired->StopAnim();
				JAnim_MoopanTalk->StopAnim();
				JAnim_MoopanTalk->SetEnableObjects(false);
				JAnim_MoopanDrinking->PlayAnim();
				Control::Audio->QuickLoadAndPlaySFX("s5p7_moopanDrinking");
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
				SetSparkleUpdate(true);
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S5_MedicineJar] = false;

				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S5_MedicineJar);
				Hud->EnableInput(false);
				Hud->HideHud(true);
				return;
			}
		}
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

bool L2S5_Pop7Moopan::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan && ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken )
		isAnyTaskLeft = true;
	return isAnyTaskLeft;
}

void L2S5_Pop7Moopan::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan && ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken )
		Hud->ShowHintForInvItem(eInv_L2S5_MedicineJar, &MoopanRect);
	else
		Popup::ProcessHint();
}
