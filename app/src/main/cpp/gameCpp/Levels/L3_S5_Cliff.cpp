//====================================
// DQFULP
// Hidden Object System
// Author : Amal
// LEVEL3 - SCENE3 : Cliff
//====================================

#include "L3_S5_Cliff.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Circle.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"

const CPoint		CenterPoint(683,334,0);

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CPoint		PopCloseRectBtnPoint(998,98,0);
const CRectangle  	PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);

const CRectangle  Pop1Vid1RectGrassCutting(351,97,650,488);

const CRectangle  Pop2Vid1RectDeerLoop(351,97,650,488);
const CRectangle  Pop2Vid2RectGrassToDeer(351,97,650,488);

const CRectangle  Pop3Vid1RectNailHammer(351,97,650,488);
const CRectangle  Pop3Vid2RectRopeFix(351,97,650,488);

L3_S5_Cliff::L3_S5_Cliff()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S5\\S5.SCN");
	musicFadeOutTimer = 0;
	//Scene Animations
	ExploreAreaUnlockTimer = -1;

	dialogSystem = NULL;
	//if condition to add
		dialogSystem = new DialogSystem();
		AddControl(dialogSystem);	
	
	HOScene->GetObjectByID("plant1")->SetPivot(47, 139);
	HOScene->GetObjectByID("plant1")->Animate(0,0,1,0,6.0f);	

	HOScene->GetObjectByID("plant2")->SetPivot(50, 100);
	HOScene->GetObjectByID("plant2")->Animate(0,0,1,0,5.0f);	

	HOScene->GetObjectByID("plant3")->SetPivot(38, 123);
	HOScene->GetObjectByID("plant3")->Animate(0,0,1,0,10.0f);	

	

	S5crystalprt1FX = new SuperFX("PRT\\L3\\S5crystalprt1.INI", PointSystem::CreateCPoint(323,337,0), 100); 
	S5crystalprt1FX->Start();
	S5crystalprt1FX->AdvanceByTime(2.0f);

	S5crystalprt2FX = new SuperFX("PRT\\L3\\S5crystalprt2.INI", PointSystem::CreateCPoint(325,538,0), 100); 
	S5crystalprt2FX->Start();
	S5crystalprt2FX->AdvanceByTime(2.0f);

	S5crystalprt3FX = new SuperFX("PRT\\L3\\S5crystalprt3.INI", PointSystem::CreateCPoint(142,91,0), 100); 
	S5crystalprt3FX->Start();
	S5crystalprt3FX->AdvanceByTime(2.0f);

	S5crystalprt4FX = new SuperFX("PRT\\L3\\S5crystalprt4.INI", PointSystem::CreateCPoint(1166,159,0), 100); 
	S5crystalprt4FX->Start();
	S5crystalprt4FX->AdvanceByTime(2.0f);

	S5crystalprt5FX = new SuperFX("PRT\\L3\\S5crystalprt5.INI", PointSystem::CreateCPoint(326,549,0), 100); 
	S5crystalprt5FX->Start();
	S5crystalprt5FX->AdvanceByTime(2.0f);

	S5crystalprt6FX = new SuperFX("PRT\\L3\\S5crystalprt6.INI", PointSystem::CreateCPoint(1298,425,0), 100); 
	S5crystalprt6FX->Start();
	S5crystalprt6FX->AdvanceByTime(2.0f);

	S5FogPrtFX = new SuperFX("PRT\\L3\\S5fogprt.INI", PointSystem::CreateCPoint(839,74,0), 100); 
	S5FogPrtFX->Start();
	S5FogPrtFX->AdvanceByTime(2.0f);

	meshCloth = NULL;
	meshCloth = new MeshObject(HOScene->GetObjectByID("flag"), 5, 5);
	meshCloth->meshFX->SetSpeedAndDisplacement(2, 1, 5, 2);
	meshCloth->meshFX->SetMeshBox(0,1, 4, 4);

	HOScene->PushObject(meshCloth);

	JAnim_DeerLoop = new JSONAnimator(0, CC_CALLBACK_1(L3_S5_Cliff::OnAnimationComplete, this));
	JAnim_DeerLoop->parseAndLoadJSON("ANIM\\L3\\S5DEERLOOPANIM.JSON", HOScene);

	
		JAnim_DeerFadeOut = new JSONAnimator(1, CC_CALLBACK_1(L3_S5_Cliff::OnAnimationComplete, this));
	JAnim_DeerFadeOut->parseAndLoadJSON("ANIM\\L3\\S5DEERFADEOUT.JSON", HOScene);

	isMaryFallCutscnPlaying = false;
	
	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	cv_MaryFallCutscn = new CVideo("OGV\\L3\\S5MARYFALL.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//cv_MaryFallCutscn->SetScale(1.334f);
	cv_MaryFallCutscn->SetScale(GFApp->video_aspectRatio);
	cv_MaryFallCutscn->SetSkipActive(); 

	vidTimer = 0;

	if( !GFInstance->IsReplayingGame )
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied)
		{
			//disable popup
			Hud->AllowInput = false;
			Hud->HideHud();		
			cv_MaryFallCutscn->PlayFadeInVideo(2.0f);
			isMaryFallCutscnPlaying = true;
		}
	}

	
	
	if(IsVisitedForFirstTime)
		Hud->ShowBannerText("L3deer");

	GFInstance->PostUpdate(1);

	InitPopups();
	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s5Cliff,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s5Cliff) )		
	{
		Control::Audio->PlaySample(aAmbL3s5Cliff,true);
	}
	
	//after stone placing gamer somehow missed grandma anim, then forcestart grandma animation once scene resumes!!
	if( ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced && !ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P4_GrandmaTalkEnds] )
	{
		ActivePopupID = ePop5_Bucket;
		PopupArray[ePop5_Bucket]->OpenPopup();
	}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L3_S5_Cliff]  = true;
}

void L3_S5_Cliff::InitPopups()
{	
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL3S5_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	
	//Popup 1
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvAllGrassTaken)
	{
		int p1rect[] = {796,446,980,426,993,603,929,646,781,661};
		PopPolyArray[ePop1_Grass] =  new CPolygon(5);
		PopPolyArray[ePop1_Grass]->Init(p1rect);

		PopupArray[ePop1_Grass] = new L3S5_Pop1Grass(ePop1_Grass,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop1_Grass]->center, CC_CALLBACK_1(L3_S5_Cliff::onPopupClosed, this));
		PopupArray[ePop1_Grass]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop1_Grass] = ePopupState_Closed;
	}

	//Popup 2
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone)
	{
		int p2rect[] = {646,292,810,296,800,422,769,444,670,438};
		PopPolyArray[ePop2_Deer] =  new CPolygon(5);
		PopPolyArray[ePop2_Deer]->Init(p2rect);

		PopupArray[ePop2_Deer] = new L3S5_Pop2Deer(ePop2_Deer,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop2_Deer]->center, CC_CALLBACK_1(L3_S5_Cliff::onPopupClosed, this));
		PopupArray[ePop2_Deer]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop2_Deer] = ePopupState_Closed;
	}

	////Popup 3
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken)
	{
		
		int p3rect[] = {301,146,393,146,410,259,320,272};
		PopPolyArray[ePop3_Rope] = new CPolygon(4);
		PopPolyArray[ePop3_Rope]->Init(p3rect);

		PopupArray[ePop3_Rope] = new L3S5_Pop3Rope(ePop3_Rope,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop3_Rope]->center, CC_CALLBACK_1(L3_S5_Cliff::onPopupClosed, this));
		PopupArray[ePop3_Rope]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop3_Rope] = ePopupState_Closed;
	}

	////Popup 4
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S5EasterEgg] )
	{

		int p4rect[] = {690,379,800,380,801,449,699,445};
		PopPolyArray[ePop4_Clip] = new CPolygon(4);
		PopPolyArray[ePop4_Clip]->Init(p4rect);

		PopupArray[ePop4_Clip] = new L3S5_Pop4Clip(ePop4_Clip,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop4_Clip]->center, CC_CALLBACK_1(L3_S5_Cliff::onPopupClosed, this));
		PopupArray[ePop4_Clip]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop4_Clip] = ePopupState_Closed;
	}
	

	////Popup 5
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvClipTaken)
	{

		int p5rect[] = {658,437,773,436,779,535,657,548};
		PopPolyArray[ePop5_Bucket] = new CPolygon(4);
		PopPolyArray[ePop5_Bucket]->Init(p5rect);

		PopupBucket = new L3S5_Pop5Bucket(ePop5_Bucket,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop5_Bucket]->center, CC_CALLBACK_1(L3_S5_Cliff::onPopupClosed, this));
		PopupArray[ePop5_Bucket] = PopupBucket;
		PopupArray[ePop5_Bucket]->SetPopupBoundingBox(PopAreaRect);
		PopupBucket->initDialogSystem(dialogSystem);

		PopStates[ePop5_Bucket] = ePopupState_Closed;
	}

	SceneObjectsUpdate();
}



void L3_S5_Cliff::onPopupClosed(Popup * popup)
{
	ActivePopupID = ePopInactive;
	isSparkleUpdatePending = true;
	switch (popup->GetPopupId())
	{
	case 0:
		SceneObjectsUpdate();
		if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvAllGrassTaken)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 1:
		TaskPendingUpdate();
		if(ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 2:
		SceneObjectsUpdate();
		if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 3:
		SceneObjectsUpdate();
		if(ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
			Hud->AllowInput = false;
			Hud->HideHud();		
			cv_MaryFallCutscn->PlayFadeInVideo(2.0f);
			isMaryFallCutscnPlaying = true;
		}
		break;
	case 4: //ePop5_Bucket
		SceneObjectsUpdate();
		if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvClipTaken)
		{
			//Show Objective completed
			if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O1_ExploreArea] && 
				!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L3O1_ExploreArea] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L3O1_ExploreArea] = true;
				ExploreAreaUnlockTimer = 6.0f;					
			}

			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}		
			

		break;
	default:
		break;
	}
}


L3_S5_Cliff::~L3_S5_Cliff()
{
	SAFE_DELETE(S5crystalprt1FX);
	SAFE_DELETE(S5crystalprt2FX);
	SAFE_DELETE(S5crystalprt3FX);
	SAFE_DELETE(S5crystalprt4FX);
	SAFE_DELETE(S5crystalprt5FX);
	SAFE_DELETE(S5crystalprt6FX);
	SAFE_DELETE(S5FogPrtFX);
	SAFE_DELETE(JAnim_DeerLoop);
	SAFE_DELETE(JAnim_DeerFadeOut);

	for (int i = 0; i < ePopL3S5_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	SAFE_DELETE(cv_MaryFallCutscn);
	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s5Cliff);
	Control::Audio->UnloadSample(aAmbL3s5Cliff);
	//MUSIC_SFX
}

void L3_S5_Cliff::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(isMaryFallCutscnPlaying || ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied)
		return;

	if(ExploreAreaUnlockTimer>0)
	{
		ExploreAreaUnlockTimer -= Control::LogicRate;
		if(ExploreAreaUnlockTimer<=0)
		{
			ExploreAreaUnlockTimer = 0.0f;
			Hud->ShowObjectiveFX(eObj_L3O1_ExploreArea,true);
		}
	}
	

	if( Control::Audio->IsPlaying(aTrackL3ho) )
		{
			Control::Audio->Samples[aTrackL3ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL3ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL3ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL3theme) )
					  Control::Audio->PlaySample(aTrackL3theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL3puzzle) )
		{
			Control::Audio->Samples[aTrackL3puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL3puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL3puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL3theme) )
					  Control::Audio->PlaySample(aTrackL3theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL3theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL3theme, true);
			}
		}
}

void L3_S5_Cliff::OnAnimationComplete(int AnimID)
{
	if(AnimID == 0)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone)
		{
			JAnim_DeerLoop->PlayAnim();
			HOScene->GetObjectByID("ropeClip")->SetEnable(false);
		}
	}
}

void L3_S5_Cliff::Update()
{
	
	NavigateToScene();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	

	if (isMaryFallCutscnPlaying)
	{
		Cursor::SetMode(CA_Normal);
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_MaryFallCutscn->isEndReached() ||
#endif
			vidTimer > 12)
		{
			
			isMaryFallCutscnPlaying = false;
			cv_MaryFallCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P6_MaryFallDown])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P6_MaryFallDown] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L3P6_MaryFallDown;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			}

			//Clearing all the Level 3 Objectives
			ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();

			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance;
			ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_4;

			//Backup Save for level restart
			ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);

			SendMessage(ControlExit, ID, HOSM_NewArea);		
			
		} 
		return;
	}
	else if( GFInstance->IsReplayingGame )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted) 
		{
			GFInstance->IsReplayingGame = false;
		}
		bool isPendingCollectableInLevel = false;
		 if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S2Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S3Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S1Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S4Collected )
		{
			isPendingCollectableInLevel = true;		
			if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle)//avoid looped banner calls!!
				Hud->ShowBannerText("L1mismo");
		}
		else 
		{
			for( int i = 0; i < 5; i++ )				
				if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[(ProfileSystem::Instance->ActiveProfile.CurrentLevel*5)+i] )
				{
					isPendingCollectableInLevel = true;
					if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle)//avoid looped banner calls!!
							Hud->ShowBannerText("L1mismc");
					break;
				}
		}

		//No pending collectables!!
		if( !isPendingCollectableInLevel )
			if(ProfileSystem::Instance->ActiveProfile.L3_S5_RopeTied)
			{
				//disable popup
				Hud->AllowInput = false;
				Hud->HideHud();		
				cv_MaryFallCutscn->PlayFadeInVideo(2.0f);
				isMaryFallCutscnPlaying = true;
				return;
			}
	}

	if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		SparkleUpdate();

	CPoint pos = Control::Input->Mpos();

	if( ActivePopupID == ePopInactive )
	{
		#ifdef _CEBUILD
		if(!Hud->isMouseOverExtraButton)
			Cursor::SetMode(CA_Normal);
	#else
		Cursor::SetMode(CA_Normal);
	#endif 

		for (int i = 0; i < ePopL3S5_Max; i++)
		{
			
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_Look);
					if( Control::Input->LBclicked() )
					{
						
							Cursor::SetMode(CA_Normal);
							ActivePopupID = i;
						
							PopupArray[i]->OpenPopup();
							
					}
					return;
				}
			}
			
		}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S4_FortFountain;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
		#endif	

		if( IsMouseOverGoBackArea(&pos))
		{

#ifdef TOUCH_UI
		//	if(!GFHud->InvBoxCtrl->navTapped)
		//		Cursor::SetMode(CA_ExitDown,eArea_L3_S4_FortFountain);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L3_S4_FortFountain);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L3_S4_FortFountain, 2);
				}
#else
				//Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S4_FortFountain;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				

				
				
			}	
		}
	}

	

}

void L3_S5_Cliff::SparkleUpdate()
{
	if(isSparkleUpdatePending)
	{
		isSparkleUpdatePending = false;
		if(SparkleFX==NULL)
		{
			SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);		
		}
		//Game Logic Goes here

		//Game Logic Goes here
		numSparkles = 0;
		currentSparkleID = 0;
		
		//Check for Popup Tasks
		for (int i = 0; i < ePopL3S5_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopupArray[i]->isTaskLeft())
				{
					sparklePoints[numSparkles].SetXY(PopPolyArray[i]->center.x, PopPolyArray[i]->center.y);
					numSparkles++;
				}
			}
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

void L3_S5_Cliff::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL3S5_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}


	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S5_Cliff] = isAnyTaskLeft;
	
	
}

void L3_S5_Cliff::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	HOScene->GetObjectByID("deerEat")->SetEnable(false);
	HOScene->GetObjectByID("deerEatShadow")->SetEnable(false);
	HOScene->GetObjectByID("deerGrass")->SetEnable(false);
	HOScene->GetObjectByID("deerBody")->SetEnable(false);
	HOScene->GetObjectByID("deerHead")->SetEnable(false);
	HOScene->GetObjectByID("deerShadow")->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvRopeTaken)
		HOScene->GetObjectByID("rope")->SetEnable(true);
	else
		HOScene->GetObjectByID("rope")->SetEnable(false);

	

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_GreenStonePlaced)
		HOScene->GetObjectByID("greenWater")->SetEnable(false);
	else
		HOScene->GetObjectByID("greenWater")->SetEnable(true);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_DeerGone)
	{
		JAnim_DeerLoop->PlayAnim();

	}
	else
	{
		HOScene->GetObjectByID("deerEat")->SetEnable(false);
		HOScene->GetObjectByID("deerEatShadow")->SetEnable(false);
		HOScene->GetObjectByID("deerGrass")->SetEnable(false);

	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_ClipPlaced)
		HOScene->GetObjectByID("ropeClip")->SetEnable(false);
	else
		HOScene->GetObjectByID("ropeClip")->SetEnable(true);
	

}

void L3_S5_Cliff::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
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
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == HOSM_DialogClosed ) 
	{
		PopupBucket->onDialogComplete();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				//UpdatePopups(dropPos,true);
				if(ActivePopupID == ePopInactive)
				{
					//If No popups responded
					GFInstance->WrongInvDropMsg(Hud);
				}
				return;
			}
			else
			{
				if(PopupArray[ActivePopupID]!=NULL)
					PopupArray[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal);
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

float L3_S5_Cliff::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L3_S5_Cliff::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S5EasterEgg])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(430, 390);
			else
				InitHintTrailEmit(730, 415);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}
	int iNonHOHint = 0; 
	int MaxNonHOHints = 5;
	do
	{
		switch( iNonHOHint )
		{
			default:			
				//Exit coordinates
				break;
			
			case 0: ++iNonHOHint;
					if( ActivePopupID != ePopInactive )
					{
						PopupArray[ActivePopupID]->ProcessHint();
						return;
					}
					break;
			case 1: ++iNonHOHint;
					for (int i = 0; i < ePopL3S5_Max; i++)
					{
						if(PopStates[i] != ePopupState_Inactive) //if popup is active
						{
							if(PopupArray[i]->isTaskLeft())
							{
								InitHintTrailEmit(&PopPolyArray[i]->center,false);

								return;
							}
						}
					}
					break;
			case 2: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S4_FortFountain] )
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
			case 4: ++iNonHOHint;
					{
						Hud->teleportSystem->ShowTeleport();
						return;
					}
					break;

		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!

	Hud->ShowNothingToDoMsgBanner();
}

void L3_S5_Cliff::NavigateToScene()
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

