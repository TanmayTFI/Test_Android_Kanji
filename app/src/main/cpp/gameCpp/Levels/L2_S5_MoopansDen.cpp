//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 05 : MOOPAANS DEN
//====================================


#include "L2_S5_MoopansDen.h"
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
#include "exMath.h"


const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,99,0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);
const CRectangle  PopRect1MooppanTired(909.0f,224.0f,175.0f,191.0f);

const CRectangle  InfoRectMooppanMeditate(762.0f,79.0f,134.0f,190.0f);

//Puzzle Rects
const CRectangle  pzMedicineMakingRect(234.0f,331.0f,248.0f,128.0f);
const CRectangle  PZMooppanShelfRect(981.0f,166.0f,90.0f,130.0f);
const CRectangle  InvPotRect(999.0f,324.0f,110.0f,88.0f);
const CRectangle  PopVinegarRect(302.0f,157.0f,91.0f,69.0f);





L2_S5_MoopansDen::L2_S5_MoopansDen()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S5\\L2_S5.SCN");
	musicFadeOutTimer = 0;
	InvAmber = HOScene->GetObjectByID("Amber");
	InvPot	= HOScene->GetObjectByID("Pot");
	StrawBasket	= HOScene->GetObjectByID("Basket");

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_IsVisited)
	{
		Hud->ShowBannerText("L2too");
		ProfileSystem::Instance->ActiveProfile.L2_S5_IsVisited = true;
	}
	
	SceneObjectsUpdate();

	dialogIndex = 0;
	dialogSystem = NULL;
	//if condition to add
	dialogSystem = new DialogSystem();
	AddControl(dialogSystem);	

	Scn_OrbTaking = HOScene->GetObjectByID("orbtaking");
	Scn_OrbTaking->SetZ(5032);
	Scn_OrbTaking->SetEnable(false);

	IsOrbFadingIn = false;
	IsOrbTaken = false;
	IsOrbAnimatingToHUD = false;

	orbControlPos.SetXY(400,206);
	orbInitPos = Scn_OrbTaking->Pos;
	orbEndPos.SetXY(260, 655);
	orbInitScale = 1.0f;
	orbFinalScale = 0.95f;
	orbTweenTimer = 0;
	orbTweenMaxTime = 1.25f;

	InitPopups();
	InitSceneElements();
	//Animations

	HOScene->GetObjectByID("AnimationRoot1")->SetPivot(0, 0);
	HOScene->GetObjectByID("AnimationRoot1")->Animate(1,4,0,0,9.0f);

	HOScene->GetObjectByID("AnimationRoot2")->SetPivot(0, 0);
	HOScene->GetObjectByID("AnimationRoot2")->Animate(1,4,0,0,8.0f);	

	HOScene->GetObjectByID("AnimationRoot3")->SetPivot(0, 0);
	HOScene->GetObjectByID("AnimationRoot3")->Animate(1,4,0,0,7.0f);	

	HOScene->GetObjectByID("Plant00")->SetPivot(-39, 50);
	HOScene->GetObjectByID("Plant00")->Animate(0,0,4,0,9.0f);	

	HOScene->GetObjectByID("Plant01")->SetPivot(-26, 80);
	HOScene->GetObjectByID("Plant01")->Animate(0,0,4,0,7.0f);	

	HOScene->GetObjectByID("Flower1")->SetPivot(-58, 143);
	HOScene->GetObjectByID("Flower1")->Animate(0,0,2,0,5.0f);	

	HOScene->GetObjectByID("Flower2")->SetPivot(-66, 109);
	HOScene->GetObjectByID("Flower2")->Animate(0,0,3,0,6.0f);	
	
	HOScene->GetObjectByID("WindowLightLeft")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("WindowLightLeft")->Animate(0.15f, 0.65f, 6.0f);

	HOScene->GetObjectByID("WindowLightTop")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("WindowLightTop")->Animate(0.35f, 0.85f, 5.0f);

	//Particles

	float zorder = (float)(HOScene->GetObjectByID("mask6")->ZOrder - 1);
	
	DustWindowLeft = new SuperFX("PRT\\L2\\dustWindowLeft.INI", PointSystem::CreateCPoint(320, 200,0), 100);
	DustWindowLeft->Start();

	DustWindowRight = new SuperFX("PRT\\L2\\dustWindowRight.INI", PointSystem::CreateCPoint(770, 0,0), (int)(zorder));
	DustWindowRight->Start();

	//morph Object
	HOScene->GetObjectByID("MorphTarget")->SetEnable(false); //morph object removed
	HOScene->GetObjectByID("MorphBase")->SetEnable(false);
//#ifdef _CEBUILD
//	//Morph Object Add
//	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S5Collected)
//	{
//		Hud->setMorphObjects(HOScene->GetObjectByID("MorphTarget"), HOScene->GetObjectByID("MorphBase"));
//		CRectangle  MorphRect(571.0f,328.0f,110.0f,121.0f);
//		Hud->setMorphObjRect(MorphRect);
//	}
//	else
//	{
//		HOScene->GetObjectByID("MorphTarget")->SetEnable(false);
//	}
//#else
//	HOScene->GetObjectByID("MorphTarget")->SetEnable(false);
//#endif 

	//Show Objective completed
	if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O1_MeetMooppan] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O1_MeetMooppan] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O1_MeetMooppan] = true;
		Hud->ShowObjectiveFX(eObj_L2O1_MeetMooppan,true);
	}

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P4_MooppanMeet])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P4_MooppanMeet] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L2P4_MooppanMeet;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}

	TaskPendingUpdate();

	Control::Audio->LoadSample(aVOL2MaryRefS5_1,	AudioVO);
	Control::Audio->LoadSample(aAmbs5MoopansDen,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs5MoopansDen) )		
	{
		Control::Audio->PlaySample(aAmbs5MoopansDen,true);
	}
}

void L2_S5_MoopansDen::InitSceneElements()
{																			  
	Alovera					= HOScene->GetObjectByID("Alovera");
	AloveraCut				= HOScene->GetObjectByID("AloveraCut");
	Chart					= HOScene->GetObjectByID("Chart");
	Fire					= HOScene->GetObjectByID("Fire");
	FireFx					= HOScene->GetObjectByID("FireFx");
	Fog						= HOScene->GetObjectByID("Fog");
	HoneyPot				= HOScene->GetObjectByID("HoneyPot");
	Kindi					= HOScene->GetObjectByID("Kindi");
	Mango					= HOScene->GetObjectByID("Mango");
	MangoCut				= HOScene->GetObjectByID("MangoCut");
	MiniGrinderHadle		= HOScene->GetObjectByID("MiniGrinderHadle");
	MortarWithPestle		= HOScene->GetObjectByID("MortarWithPestle");
	PestleOnGround			= HOScene->GetObjectByID("PestleOnGround");
	Pot						= HOScene->GetObjectByID("WaterPot");
	TurmericCrusher			= HOScene->GetObjectByID("TurmericCrusher");
	TurmericFull			= HOScene->GetObjectByID("TurmericFull");
	TurmericPices			= HOScene->GetObjectByID("TurmericPices");
	TurmericPot				= HOScene->GetObjectByID("TurmericPot");
	Wood					= HOScene->GetObjectByID("Wood");



	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced  )
	{
		Alovera->SetEnable(false);
		AloveraCut->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced)
		Alovera->SetEnable(false);
	else
		AloveraCut->SetEnable(false);
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied)
		AloveraCut->SetEnable(false);

	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced  )
	{
		TurmericFull->SetEnable(false);
		TurmericPices->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced)
		TurmericFull->SetEnable(false);
	else
		TurmericPices->SetEnable(false);
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericAddedToPot)
		TurmericPices->SetEnable(false);

	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced  )
	{
		Mango->SetEnable(false);
		MangoCut->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced)
		Mango->SetEnable(false);
	else
		MangoCut->SetEnable(false);
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot)
		MangoCut->SetEnable(false);

	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced )
		Wood->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired )
	{
		FireFx->SetEnable(false);
		Fire->SetEnable(false);
		Fog->SetEnable(false);
	}
	
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken )
		Kindi->SetEnable(false);
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied )
		HoneyPot->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated )
		MiniGrinderHadle->SetEnable(false);
	else
		PestleOnGround->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied )
	{
		TurmericCrusher->SetEnable(false);
		MortarWithPestle->SetEnable(false);
		TurmericPot->SetEnable(false);
	}
	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated )
		MortarWithPestle->SetEnable(false);
	else
	{
		TurmericPot->SetEnable(false);
		TurmericCrusher->SetEnable(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced)
		Pot->SetEnable(false);
}


void L2_S5_MoopansDen::OnPopupClosed(Popup * popup)
{
	switch (popup->GetPopupId())
	{
	case ePop1_Vinegar:
		//SLOG::LogInt("popup closed %d", popup->GetPopupId());
		break;
	case ePop2_MedicineMaking:
		break;
	case ePop3_Amber:
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken)
		{
			InvAmber->SetEnable(false);
			isSparkleUpdatePending = true;
		}
		break;
	case ePop4_EagleBlock:
		break;
	case ePop5_BambooBasket:
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_StrawBasketRemoved)
		{
			if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken )
			{
				InvPot->SetEnable(false);
				isSparkleUpdatePending = true;
			}
			StrawBasket->SetEnable(false);
		}
		break;
	case ePop6_MedicineRecipe:
		break;
	case ePop7_Moopan:
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
		{
			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O4_FindWayOutOfNetherworld] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O4_FindWayOutOfNetherworld] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
				= eObj_L2O4_FindWayOutOfNetherworld;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L2O4_FindWayOutOfNetherworld);

				Control::Audio->PlaySample(aSFXMagic1);
			}

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P5_CrystalBallSophia])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P5_CrystalBallSophia] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L2P5_CrystalBallSophia;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			IsOrbAnimatingToHUD = true;
			ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked = true;
			Scn_OrbTaking->SetEnable(true);

			isSparkleUpdatePending = true;
		}
		break;
	default:
		break;
	}
	ActivePopupID = ePopInactive;
	TaskPendingUpdate();
}

void L2_S5_MoopansDen::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopS5_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	ActivePopupID = ePopInactive;
	
	//Popup 1
#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken || !ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken
		|| !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope] )
#else
	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken || !ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken)
#endif 
	{
		int popPts[] = {329,156,314,184,308,215,318,225,386,225,386,196,373,174,348,156,328,153};
		CPolygon* pop1ClickPoly=  new CPolygon(9);
		pop1ClickPoly->Init(popPts);

		Popup* Pop1Scene = new L2S5_Pop1Vinegar(ePop1_Vinegar, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L2_S5_MoopansDen::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[ePop1_Vinegar] = pop1ClickPoly;
		PopupArray[ePop1_Vinegar]	= Pop1Scene;

		PopStates[ePop1_Vinegar] = ePopupState_Closed;
	}
	
	//Popup 2
	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken )
	{
		//int popPts[] = {307,534,394,533,432,562,389,600,309,595,274,565,304,532};
		//CPolygon* pop2ClickPoly=  new CPolygon(7);

		int popPts[] = {270,530, 430,530, 430,597, 270,597, 269,529};
		CPolygon* pop2ClickPoly=  new CPolygon(5);
		pop2ClickPoly->Init(popPts);

		Popup* Pop2Scene = new L2S5_Pop3Ember(ePop3_Amber, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L2_S5_MoopansDen::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopCircleRect);

		PopPolyArray[ePop3_Amber] = pop2ClickPoly;
		PopupArray[ePop3_Amber]	= Pop2Scene;

		PopStates[ePop3_Amber] = ePopupState_Closed;
	}

	//Popup 3
	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue]) 
	{
		int popPts[] = {1001,365,1006,331,1050,317,1091,339,1094,383,1041,401,998,369};
		CPolygon* pop3ClickPoly=  new CPolygon(7);
		pop3ClickPoly->Init(popPts);

		Popup* Pop3Scene = new L2S5_Pop5Basket(ePop5_BambooBasket, Hud, HOScene, eZOrder_Popup, pop3ClickPoly->center, CC_CALLBACK_1(L2_S5_MoopansDen::OnPopupClosed, this));
		Pop3Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[ePop5_BambooBasket] = pop3ClickPoly;
		PopupArray[ePop5_BambooBasket]	= Pop3Scene;

		PopStates[ePop5_BambooBasket] = ePopupState_Closed;
	}
	MoopanPopup = NULL;
	//Popup 4
	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
	{
		int popPts[] = {1006,160,1041,186,1033,231,1050,295,996,306,885,307,926,227,975,233,971,165};
		CPolygon* pop4ClickPoly=  new CPolygon(9);
		pop4ClickPoly->Init(popPts);

		
		MoopanPopup = new L2S5_Pop7Moopan(ePop7_Moopan, Hud, HOScene, eZOrder_Popup, pop4ClickPoly->center, CC_CALLBACK_1(L2_S5_MoopansDen::OnPopupClosed, this));
		MoopanPopup->SetPopupBoundingBox(PopAreaRect);
		MoopanPopup->SetDialogSystem(dialogSystem);

		PopPolyArray[ePop7_Moopan] = pop4ClickPoly;
		PopupArray[ePop7_Moopan]	= MoopanPopup;

		PopStates[ePop7_Moopan] = ePopupState_Closed;

		
	}
	
	
}

void L2_S5_MoopansDen::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL2theme1))
		return;
	
	if ( MoopanPopup != NULL && MoopanPopup->isVideoPlaying )
		return;

	if( Control::Audio->IsPlaying(aTrackL2ho) )
		{
			Control::Audio->Samples[aTrackL2ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL2ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL2ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL2theme) )
					  Control::Audio->PlaySample(aTrackL2theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL2puzzle) )
		{
			Control::Audio->Samples[aTrackL2puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL2puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL2puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL2theme) )
					  Control::Audio->PlaySample(aTrackL2theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL2theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL2theme, true);
			}
		}
}

void L2_S5_MoopansDen::SceneObjectsUpdate()
{
	if (!ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan)
	{
		HOScene->GetObjectByID("MooppanMeditation")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("MooppanSitting-")->SetEnable(false);
		HOScene->GetObjectByID("MooppanMeditation")->SetEnable(true);
		HOScene->GetObjectByID("MooppanMeditation")->Animate(0,3,0,0,4.0f);	
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken )
	{
		InvAmber->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_ShelfPuzzleSolved )
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvPujaChartTaken  && ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken  )
		{
			HOScene->GetObjectByID("ShelfOpen")->PlayAnimation(1);
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvPujaChartTaken  && !ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken  )
		{
			HOScene->GetObjectByID("ShelfOpen")->PlayAnimation(0);
		}
		else
			HOScene->GetObjectByID("ShelfOpen")->PlayAnimation(2);
	}
	else
	{
		HOScene->GetObjectByID("ShelfOpen")->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_StrawBasketRemoved )
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken )
		{
			InvPot->SetEnable(false);
		}
		StrawBasket->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken )
	{
		HOScene->GetObjectByID("Stone")->SetEnable(false);
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken )
	{
		HOScene->GetObjectByID("Vinegar")->SetEnable(false);
	}
	TaskPendingUpdate();
}

bool L2_S5_MoopansDen::CheckIfPopupActive(int polyIndex)
{
	bool isPopupActive = true;
	switch (polyIndex)
	{
	case ePop1_Vinegar:		if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken && ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken && 
								ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope])
		isPopupActive = false;
		break;
	case ePop2_MedicineMaking: if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken )
		isPopupActive = false;
		break;
	case ePop3_Amber:			if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken )
		isPopupActive = false;
		break;
	case ePop4_EagleBlock:		if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken && ProfileSystem::Instance->ActiveProfile.L2_S5_InvPujaChartTaken )
		isPopupActive = false;
		break;
	case ePop5_BambooBasket:	
			if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue] && ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken)
			{			
				isPopupActive = false;
			}
		break;
	case ePop7_Moopan:		if(ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
		isPopupActive = false;
	default:
		break;
	}
	return(isPopupActive);
}


void L2_S5_MoopansDen::Update()
{
	NavigateToScene();
	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive )
		return;

	if( IsOrbAnimatingToHUD )
	{
		orbTweenTimer += Control::LogicRate;
		CPoint bt = Elixir::GetBezierPoint(&orbInitPos,&orbControlPos,&orbEndPos,orbTweenTimer/orbTweenMaxTime);

		float scale = orbInitScale + (orbFinalScale-orbInitScale)*Elixir::Easing(orbTweenTimer,0,1,orbTweenMaxTime);

		Scn_OrbTaking->SetPos(&bt);
		Scn_OrbTaking->SetScale(scale);
		Scn_OrbTaking->SetAlpha(2.0f - orbTweenTimer);

		if(orbTweenTimer>orbTweenMaxTime)
		{
			orbTweenTimer = 0.0f;
			IsOrbAnimatingToHUD = false;

			Scn_OrbTaking->SetEnable(false);
			Hud->FadeInOrb();
			//Control::Audio->PlaySample(aSFXL1S2MapFadeIn);
		}

		return;
	}	

	CPoint pos = Control::Input->Mpos();


	if( ActivePopupID == ePopInactive )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		Cursor::SetMode(CA_Normal);
		for (int i = 0; i < ePopS5_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos))
				{
					if(CheckIfPopupActive(i))
					{
						Cursor::SetMode(CA_Look);
						if( Control::Input->LBclicked() )
						{
							Cursor::SetMode(CA_Normal);
							ActivePopupID = i;
							PopupArray[i]->OpenPopup();
							if(ActivePopupID == ePop3_Amber)
							{
								Hud->ShowBannerText("L2coal");
							}
						}
						return;
					}

				}
			}
		}

	}
	
	
	if( ActivePopupID == ePopInactive )
	{

		if( pzMedicineMakingRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken)
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_Look);
#else
			Cursor::SetMode(CA_Look);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S5_PZMedicine_Making, 0, pzMedicineMakingRect);
				}
#else
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_PZMedicine_Making;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
		if( PZMooppanShelfRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan && (!ProfileSystem::Instance->ActiveProfile.L2_S5_InvPujaChartTaken || 
			!ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle]))
		{

#ifdef TOUCH_UI
		//	if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_Look);
#else
			Cursor::SetMode(CA_Look);
#endif

			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L2hope");
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S5_PZEagleUnblock, 2, PZMooppanShelfRect);
				}
#else

				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_PZEagleUnblock;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
		else if( InfoRectMooppanMeditate.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2disturb");
				Control::Audio->PlaySample(aVOL2MaryRefS5_1);
			}

		}
		if( IsMouseOverGoBackArea(&pos) )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitDown,eArea_L2_S4_Cave);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L2_S4_Cave);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S4_Cave, 2);
				}
#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_Cave;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}	
		}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
			{
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_Cave;
				SendMessage(ControlExit, ID, HOSM_NewArea);						
			}	
		#endif
	}
}

void L2_S5_MoopansDen::SparkleUpdate()
{
	if(isSparkleUpdatePending)
	{
		isSparkleUpdatePending = false;
		if(SparkleFX==NULL)
		{
			SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);		
		}
		//Game Logic Goes here
		numSparkles = 0;
		currentSparkleID = 0;

		if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken)
		{
			sparklePoints[numSparkles].SetXY(PopPolyArray[ePop3_Amber]->center.x, PopPolyArray[ePop3_Amber]->center.y );
			numSparkles++;
		}
		if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken || !ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken )
		{
			sparklePoints[numSparkles].SetXY(PopPolyArray[ePop1_Vinegar]->center.x, PopPolyArray[ePop1_Vinegar]->center.y);
			numSparkles++;
		}
		if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue])
		{
			sparklePoints[numSparkles].SetXY(PopPolyArray[ePop5_BambooBasket]->center.x, PopPolyArray[ePop5_BambooBasket]->center.y);
			numSparkles++;
		}
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
		{
			if((!ProfileSystem::Instance->ActiveProfile.L2_S5_InvPujaChartTaken || !ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken || 
				!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle]) && ProfileSystem::Instance->ActiveProfile.L2_S5_ShelfPuzzleSolved )
			{
				sparklePoints[numSparkles].SetXY(PZMooppanShelfRect.GetCenterX(), PZMooppanShelfRect.GetCenterY());
				numSparkles++;
			}
		}
		else if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
		{
			sparklePoints[numSparkles].SetXY(PopPolyArray[ePop7_Moopan]->center.x, PopPolyArray[ePop7_Moopan]->center.y);
			numSparkles++;
		}
	}
	sparkleTimer += Control::LogicRate;
	if(sparkleTimer>sparkleThreshold)
	{
		sparkleTimer = 0;
		if(numSparkles>0)
		{			
			SparkleFX->SetPos(&sparklePoints[currentSparkleID]);
			SparkleFX->Start();
			currentSparkleID += 1;
			if(currentSparkleID>=numSparkles)
				currentSparkleID = 0;
		}
	}
}

L2_S5_MoopansDen::~L2_S5_MoopansDen()
{
	MoopanPopup = NULL;
	//SAFE_DELETE(MoopanPopup);

	if( !Control::Audio->IsToLevelTrackRoom() )
		if( Control::Audio->IsPlaying(aTrackLevel2) )
			Control::Audio->StopSample(aTrackLevel2);
		
	TaskPendingUpdate();

	for (int i = 0; i < ePopS5_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}
	SAFE_DELETE( DustWindowRight );
	SAFE_DELETE( DustWindowLeft );

	//MUSIC_SFX
	//Quick SFX unloaded in HUD
	Control::Audio->UnloadSample(aVOL2MaryRefS5_1);
	Control::Audio->StopSample(aAmbs5MoopansDen);
	Control::Audio->UnloadSample(aAmbs5MoopansDen);
	//MUSIC_SFX
}

void L2_S5_MoopansDen::ReceiveMessage(int val)
{	
	if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if( ActivePopupID == ePop7_Moopan )
			{
				PopupArray[ePop7_Moopan]->HandleInventoryDrop( dropPos, itemVal );
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();
		if(ActivePopupID != ePopInactive)
		{
			if(PopupArray[ActivePopupID]!=NULL)
				PopupArray[ActivePopupID]->UpdateBgGraphic();
		}
	}
	else if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
	else if( val == HOSM_DialogClosed )
	{
		if (MoopanPopup  != NULL )
		{
			int _dialogId = dialogSystem->GetDialogId();
			MoopanPopup->OnDialogComplete(_dialogId);
			dialogIndex++; 
		}
	}
	TaskPendingUpdate();
}

void L2_S5_MoopansDen::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
			
	if (!ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue])
		isAnyTaskLeft = true;
	else if (!ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken || !ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken)
		isAnyTaskLeft = true;
	else if (!ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken)
		isAnyTaskLeft = true;
	else if ((!ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
		|| (!ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced && ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked)
		|| (!ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced && ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken)
		|| (!ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked)
		|| (!ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken)
		|| (!ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced && ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken)
		|| (ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced && ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchFlamed && !ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
		|| (ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_IsAloveraSliced)
		|| (ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced)
		|| (ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced)
		|| (ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced))
		isAnyTaskLeft = true;
	/*else if( ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired &&	ProfileSystem::Instance->ActiveProfile.L2_S1_IsAloveraSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced
		  && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced )
		isAnyTaskLeft = true;*/
	else if (ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan)
		isAnyTaskLeft = true;
	else if (ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan && !ProfileSystem::Instance->ActiveProfile.L2_S5_ShelfPuzzleSolved)
		isAnyTaskLeft = true;
	else if (!ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] = isAnyTaskLeft;

	if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_ForestEntrance] = true;
	if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken && !ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S2_NagaTemple] = true;

	if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_ForestEntrance] = true;

	if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvPujaChartTaken && !ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S2_NagaTemple] = true;
}

void L2_S5_MoopansDen::ProcessNonHOHint()
{

	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(880, 160);
			else
				InitHintTrailEmit(1060, 360);
		}
		else if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle])
		{
			InitHintTrailEmit(1020, 200);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(850, 470);
			else
				InitHintTrailEmit(350, 195);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	if( ActivePopupID != ePopInactive )
	{
		switch (ActivePopupID)
		{
		case ePop1_Vinegar:PopupArray[ePop1_Vinegar]->ProcessHint();
			break;
		case ePop2_MedicineMaking:
			break;
		case ePop3_Amber:PopupArray[ePop3_Amber]->ProcessHint();
			break;
		case ePop4_EagleBlock:
			break;
		case ePop5_BambooBasket:PopupArray[ePop5_BambooBasket]->ProcessHint();
			break;
		case ePop6_MedicineRecipe:PopupArray[ePop6_MedicineRecipe]->ProcessHint();
			break;
		case ePop7_Moopan:PopupArray[ePop7_Moopan]->ProcessHint();
			break;
		default:
			break;
		}
		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken|| !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue])
	{
		InitHintTrailEmit(PopPolyArray[ePop5_BambooBasket]->center.x, PopPolyArray[ePop5_BambooBasket]->center.y,true);
		return;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken || !ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken )
	{
		InitHintTrailEmit(PopPolyArray[ePop1_Vinegar]->center.x, PopPolyArray[ePop1_Vinegar]->center.y,true);
		return;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_InvAmberTaken )
	{
		InitHintTrailEmit(PopPolyArray[ePop3_Amber]->center.x, PopPolyArray[ePop3_Amber]->center.y,true);
		return;
	}
	else if(  (!ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced &&		ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
			||(!ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced &&		ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked)
			||(!ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced &&			ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken 		)
			||(!ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced &&		ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked )
			||(!ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced &&	ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken)
			||(!ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced &&			ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken)
			||( ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced &&			ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchFlamed &&	
				!ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
			||( ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced &&		ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken &&		
				!ProfileSystem::Instance->ActiveProfile.L2_S1_IsAloveraSliced)
			||( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced &&		ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken &&		
				!ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced)
			||( ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced &&			ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken &&		
				!ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced)
			||( ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced &&		ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken &&		
				!ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced) )
	{
		InitHintTrailEmit(pzMedicineMakingRect.GetCenterX(), pzMedicineMakingRect.GetCenterY(),true);
		return;
	}
	else if( ProfileSystem::Instance->ActiveProfile.L2_S5_PuzzleModeActive && ! ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken )
	{
		InitHintTrailEmit(pzMedicineMakingRect.GetCenterX(), pzMedicineMakingRect.GetCenterY(),true);
		return;
	}
	else if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan )
	{
		InitHintTrailEmit(PopRect1MooppanTired.GetCenterX(), PopRect1MooppanTired.GetCenterY(),true);
		return;
	}
	else if( ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan && !ProfileSystem::Instance->ActiveProfile.L2_S5_ShelfPuzzleSolved )
	{
		InitHintTrailEmit(PZMooppanShelfRect.GetCenterX(), PZMooppanShelfRect.GetCenterY(),true);
		return;
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken && ProfileSystem::Instance->ActiveProfile.L2_S5_ShelfPuzzleSolved)
	{
		InitHintTrailEmit(PZMooppanShelfRect.GetCenterX(), PZMooppanShelfRect.GetCenterY(), true);
		return;
	}
	/*
	else if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd] )
	{
		InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
		return;
	}
	*/
	else if(Hud->ShowHintForComboInvItems() )
		return;
	else 
	{
		Hud->teleportSystem->ShowTeleport();
		return;
	}
    //Sac: What is the use of this command ? why software will execute a line after return ???
	Hud->ShowNothingToDoMsgBanner();
}

void L2_S5_MoopansDen::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);

				//GFHud->InvBoxCtrl->NavigateText->Show(false);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = GFHud->InvBoxCtrl->travelArea;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}

			GFHud->InvBoxCtrl->NavigateText->Text = "";
			GFHud->InvBoxCtrl->travelArea = -1;
			GFHud->InvBoxCtrl->navTapped = false;
			GFHud->InvBoxCtrl->Navigate->Show(false);
			GFHud->InvBoxCtrl->NavArrow->Show(false);
		}
	}
}