//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S4_Dungeon.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "MeshObject.h"
#include "L5S4_Pop2Pond.h"
#include "L5S4_Pop3HandCuff.h"
#include "L5S4_Pop1Bridge.h"

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);
const CRectangle  ActionRectAnsooyaTalk(722.0f,116.0f,203.0f,182.0f);
const CRectangle  InfoRectPond(152.0f,291.0f,230.0f,246.0f);
const CRectangle  InfoRectBridge(794.0f,352.0f,161.0f,179.0f);


L5_S4_Dungeon::L5_S4_Dungeon()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S4\\S4.SCN");

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);
	dialogSystem = NULL;
	//if condition to add
		dialogSystem = new DialogSystem();
		AddControl(dialogSystem);		

	isDialogActive = false;

	S4MoonPrtFX = new SuperFX("PRT\\L5\\S4MoonPrt.INI", PointSystem::CreateCPoint(930,101,0), 100); 
	S4MoonPrtFX->Start();
	S4MoonPrtFX->AdvanceByTime(2.0f);

	S4waterFallFX = new SuperFX("PRT\\L5\\S4waterFall.INI", PointSystem::CreateCPoint(145,41,0), 20); 
	S4waterFallFX->Start();
	S4waterFallFX->AdvanceByTime(2.0f);

	S4waterFallDownFX = new SuperFX("PRT\\L5\\S4waterFallDown.INI", PointSystem::CreateCPoint(225,442,0), 100); 
	S4waterFallDownFX->Start();

	S4waterFallDownFX->AdvanceByTime(2.0f);

	S4CrystalParticle1FX = new SuperFX("PRT\\L5\\S4CrystalParticle1.INI", PointSystem::CreateCPoint(620,477,0), 100); 
	S4CrystalParticle1FX->Start();
	S4CrystalParticle1FX->AdvanceByTime(2.0f);

	S4CrystalParticle2FX = new SuperFX("PRT\\L5\\S4CrystalParticle2.INI", PointSystem::CreateCPoint(401,337,0), 100); 
	S4CrystalParticle2FX->Start();
	S4CrystalParticle2FX->AdvanceByTime(2.0f);

	S4CrystalParticle3FX = new SuperFX("PRT\\L5\\S4CrystalParticle3.INI", PointSystem::CreateCPoint(991,639,0), 100); 
	S4CrystalParticle3FX->Start();
	S4CrystalParticle3FX->AdvanceByTime(2.0f);

	

	S4Lava1FX = new SuperFX("PRT\\L5\\S4Lava1.INI", PointSystem::CreateCPoint(825,467,0), 100); 
	

	S4Lava2FX = new SuperFX("PRT\\L5\\S4Lava2.INI", PointSystem::CreateCPoint(580,379,0), 100); 
	

	S4Lava3FX = new SuperFX("PRT\\L5\\S4Lava3.INI", PointSystem::CreateCPoint(1113,334,0), 100); 
	

	

	MeshObject* waterObj = new MeshObject(HOScene->GetObjectByID("pondWater"), 10, 10);
	waterObj->meshFX->SetSpeedAndDisplacement(3, 3, 2, 2);
	HOScene->InsertObject(waterObj, 1);

	

	HOScene->GetObjectByID("skullChain")->SetPivot(-15, -138);
	HOScene->GetObjectByID("skullChain")->Animate(0,0,4,0,3.0f);	

	HOScene->GetObjectByID("root1")->SetPivot(-14, -142);
	HOScene->GetObjectByID("root1")->Animate(0,0,4,0,8.0f);	

	HOScene->GetObjectByID("root2")->SetPivot(-30, -88);
	HOScene->GetObjectByID("root2")->Animate(0,0,3,0,4.0f);	

	HOScene->GetObjectByID("root3")->SetPivot(-14, -142);
	HOScene->GetObjectByID("root3")->Animate(0,0,2,0,5.0f);	

	HOScene->GetObjectByID("root4")->SetPivot(-14, -142);
	HOScene->GetObjectByID("root4")->Animate(0,0,2,0,6.0f);	

	HOScene->GetObjectByID("root5")->SetPivot(-14, -142);
	HOScene->GetObjectByID("root5")->Animate(0,0,3,0,7.0f);	

	HOScene->GetObjectByID("root6")->SetPivot(-14, -142);
	HOScene->GetObjectByID("root6")->Animate(0,0,4,0,8.0f);	

	HOScene->GetObjectByID("root7")->SetPivot(-67, -259);
	HOScene->GetObjectByID("root7")->Animate(0,0,3,0,8.0f);	

	HOScene->GetObjectByID("root8")->SetPivot(-14, -142);
	HOScene->GetObjectByID("root8")->Animate(0,0,3,0,7.0f);	

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken)
	{

			HOScene->GetObjectByID("anasooyaFree")->SetEnable(false);
	}

	cv_WaterCutscn = NULL;
	isWaterCutscnPlaying = false;
	vidTimer = 0;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeCutScenePlayed)
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);
		CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		if(ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted)
			cv_WaterCutscn = new CVideo("OGV\\L5\\S4WATERFIREPUTOUT.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		else
			cv_WaterCutscn = new CVideo("OGV\\L5\\S4WATERFIREPUTOUTNOBRIDGE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//cv_WaterCutscn->SetScale(1.334f);
		cv_WaterCutscn->SetScale(GFApp->video_aspectRatio);
		//cv_BlastCutscn->SetSkipActive(); 
	}

	JAnim_Bridge = new JSONAnimator(0, CC_CALLBACK_1(L5_S4_Dungeon::OnAnimationComplete, this));
	JAnim_Bridge->parseAndLoadJSON("ANIM\\L5\\S4BRIDGEOPEN.JSON", HOScene);

	JAnim_Anasuya = new JSONAnimator(1, CC_CALLBACK_1(L5_S4_Dungeon::OnAnimationComplete, this));
	JAnim_Anasuya->parseAndLoadJSON("ANIM\\L5\\S4ANASUYAFADEOUT.JSON", HOScene);

	

	if(IsVisitedForFirstTime)
		Hud->ShowBannerText("L5hand");
	#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S4Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		const CRectangle  MorphRect(267.0f,272.0f,122.0f,116.0f);

		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 
	InitPopups();

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete)
	{
		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O5_FindWayToFreeAnasuya] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O5_FindWayToFreeAnasuya] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O5_FindWayToFreeAnasuya] = true;
			Hud->ShowObjectiveFX(eObj_L5O5_FindWayToFreeAnasuya,true);
		}
	}

	SceneObjectsUpdate();

	TaskPendingUpdate();

		//MUSIC_SFX
	Control::Audio->LoadSample(aVOL5MaryRefS4_1,	AudioVO);
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s4DungeonAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s4DungeonAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s4DungeonAmb,true);
	}
	
	//MUSIC_SFX
	
	

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S4_Dungeon]  = true;
}

void L5_S4_Dungeon::OnAnimationComplete(int AnimID)
{
	if(AnimID == 0)
	{
		SceneObjectsUpdate();
	}
	else if(AnimID == 1)
	{
		HOScene->GetObjectByID("anasooyaFree")->SetEnable(false);
	}
}

void L5_S4_Dungeon::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL5S4_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	//Popup 1
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted)
	{
		int p1rect[] =  {968,385,1146,372,1170,494,1017,532};
		PopPolyArray[ePop1_Bridge] =  new CPolygon(4);
		PopPolyArray[ePop1_Bridge]->Init(p1rect);

		PopupArray[ePop1_Bridge] = new L5S4_Pop1Bridge(ePop1_Bridge,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop1_Bridge]->center, CC_CALLBACK_1(L5_S4_Dungeon::OnPopupClosed, this));
		PopupArray[ePop1_Bridge]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop1_Bridge] = ePopupState_Closed;
	}

	//Popup 2
	#ifdef _CEBUILD
		if (!ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S4Watermelon])
	#else
		if(!ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed)
	#endif 	
	{
		int p2rect[] ={338,368,492,335,560,382,588,455,452,488,378,456};
		PopPolyArray[ePop2_Pond] =  new CPolygon(6);
		PopPolyArray[ePop2_Pond]->Init(p2rect);

		PopupArray[ePop2_Pond] = new L5S4_Pop2Pond(ePop2_Pond,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop2_Pond]->center, CC_CALLBACK_1(L5_S4_Dungeon::OnPopupClosed, this));
		PopupArray[ePop2_Pond]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop2_Pond] = ePopupState_Closed;
	}

		//Popup 3
	if(!ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete)
	{
		int p1rect[] =  {741,125,914,120,908,300,725,292};
		PopPolyArray[ePop3_HandCuff] =  new CPolygon(4);
		PopPolyArray[ePop3_HandCuff]->Init(p1rect);

		PopupArray[ePop3_HandCuff] = new L5S4_Pop3HandCuff(ePop3_HandCuff,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop3_HandCuff]->center, CC_CALLBACK_1(L5_S4_Dungeon::OnPopupClosed, this));
		PopupArray[ePop3_HandCuff]->SetPopupBoundingBox(PopCircleRect);
		if(ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked)
			PopStates[ePop3_HandCuff] = ePopupState_Closed;
		else
		PopStates[ePop3_HandCuff] = ePopupState_Inactive;
		
	}
}

void L5_S4_Dungeon::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	isSparkleUpdatePending = true;
	switch (popup->GetPopupId())
	{
	case 0:
		if(ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted)
		{
			//disable popup
			Control::Audio->QuickLoadAndPlaySFX("s4_bridgeopen");
			JAnim_Bridge->PlayAnim();
			//CRectangle RectVidFull(171, 96, 1024, 576);
			CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
			SAFE_DELETE(cv_WaterCutscn);
			cv_WaterCutscn = new CVideo("OGV\\L5\\S4WATERFIREPUTOUT.OGV", RectVidFull, false, eZOrder_CutsceneFull);
			//cv_WaterCutscn->SetScale(1.334f);
			cv_WaterCutscn->SetScale(GFApp->video_aspectRatio);
			if(ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeCutScenePlayed)
			{
				ProfileSystem::Instance->ActiveProfile.L5_S4_BridgeOpened = true;

				//Show Objective completed
				if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O4_FindWayToAnasuya] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O4_FindWayToAnasuya] )
				{
					ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O4_FindWayToAnasuya] = true;
					Hud->ShowObjectiveFX(eObj_L5O4_FindWayToAnasuya,true);
				}
			}
				
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 1:
		if(ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed)
		{
			//disable popup
			if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted)
			{
				Hud->AllowInput = false;
				Hud->HideHud();			
				cv_WaterCutscn->PlayFadeInVideo(2.0f);
				isWaterCutscnPlaying = true;
			}
		}
		#ifdef _CEBUILD
		if (ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S4Watermelon])
		#else
		if(ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeUsed)
		#endif 	
		{
			
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	
	case 2:
		if(ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete)
		{
		PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	}
}

void L5_S4_Dungeon::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeCutScenePlayed)
	{
		HOScene->GetObjectByID("waterDungen")->SetEnable(true);
		HOScene->GetObjectByID("fireBg")->SetEnable(false);
		HOScene->GetObjectByID("pondCut")->SetEnable(true);

		MeshObject* waterObj1 = new MeshObject(HOScene->GetObjectByID("waterDungen"), 10, 10);
		waterObj1->meshFX->SetSpeedAndDisplacement(1, 1, 2, 2);
		HOScene->InsertObject(waterObj1, 1);
	}
	else 
	{
		HOScene->GetObjectByID("pondCut")->SetEnable(false);
		HOScene->GetObjectByID("waterDungen")->SetEnable(false);
		S4Lava1FX->Start();
		S4Lava1FX->AdvanceByTime(2.0f);
		S4Lava2FX->Start();
		S4Lava2FX->AdvanceByTime(2.0f);
		S4Lava3FX->Start();
		S4Lava3FX->AdvanceByTime(2.0f);

	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted)
		HOScene->GetObjectByID("bridge")->SetEnable(false);
	else
		HOScene->GetObjectByID("bridge")->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeHeadTaken)
		HOScene->GetObjectByID("pickAxeHead")->SetEnable(false);
	else
		HOScene->GetObjectByID("pickAxeHead")->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken)
		HOScene->GetObjectByID("bark")->SetEnable(false);
	else
		HOScene->GetObjectByID("bark")->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete)
	{
		HOScene->GetObjectByID("anasooyaChained")->SetEnable(false);
		if(!ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken)
			HOScene->GetObjectByID("anasooyaFree")->SetEnable(true);
		HOScene->GetObjectByID("bracelet")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("anasooyaChained")->SetEnable(true);
		HOScene->GetObjectByID("anasooyaFree")->SetEnable(false);
		HOScene->GetObjectByID("bracelet")->SetEnable(false);
	}

}

void L5_S4_Dungeon::Required()
{
	HiddenObjectSystem::Required();	//forced update
	if(isWaterCutscnPlaying)
		return;

	if( Control::Audio->IsPlaying(aTrackL5ho) )
		{
			Control::Audio->Samples[aTrackL5ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL5ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL5ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL5theme) )
					  Control::Audio->PlaySample(aTrackL5theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL5puzzle) )
		{
			Control::Audio->Samples[aTrackL5puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL5puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL5puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL5theme) )
					  Control::Audio->PlaySample(aTrackL5theme, true);
					  
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL5theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL5theme, true);
			}
		}
}

void L5_S4_Dungeon::Update()
{
	NavigateToScene();
	if(isDialogActive)
	{
		dialogSystem->Update();
		return;
	}

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	if (isWaterCutscnPlaying)
	{
		Cursor::SetMode(CA_Normal);
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_WaterCutscn->isEndReached() ||
#endif
			vidTimer > 12)
		{
			

			ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeCutScenePlayed= true;
			SceneObjectsUpdate();
			isWaterCutscnPlaying = false;
			cv_WaterCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();
			S4Lava1FX->StopImmediate();
			S4Lava2FX->StopImmediate();
			S4Lava3FX->StopImmediate();

			if(ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted)
			{
				//Show Objective completed
				if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O4_FindWayToAnasuya] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O4_FindWayToAnasuya] )
				{
					ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O4_FindWayToAnasuya] = true;
					Hud->ShowObjectiveFX(eObj_L5O4_FindWayToAnasuya,true);
				}

				ProfileSystem::Instance->ActiveProfile.L5_S4_BridgeOpened = true;
			}
			
		} 
		return;
	}

	CPoint pos = Control::Input->Mpos();
	

	if( ActivePopupID == ePopInactive )
	{
		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		

		//popup click checks
		for (int i = 0; i < ePopL5S4_Max; i++)
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
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif		

		if( IsMouseOverGoBackArea(&pos) )
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L5_S3_Pond);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L5_S3_Pond);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S3_Pond, 2);
			}
#else			
			Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
									
			}	
		}

		if( !ProfileSystem::Instance->ActiveProfile.L5_S4_BridgeOpened )	
		{
			if(ActionRectAnsooyaTalk.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					//Objective Add
					if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O4_FindWayToAnasuya] )
					{
						ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O4_FindWayToAnasuya] = true;
						ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
						= eObj_L5O4_FindWayToAnasuya;
						//Increment Objectives
						ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
						Hud->ShowObjectiveFX(eObj_L5O4_FindWayToAnasuya);

						Control::Audio->PlaySample(aSFXMagic1);
					}

					if( ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeCutScenePlayed)
						Hud->ShowBannerText("L5depress");
					else 
						Hud->ShowBannerText("L5hot");
				}
				return;
			}
			
			
		}
	else if(  !ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked &&  ProfileSystem::Instance->ActiveProfile.L5_S4_BridgeOpened  &&  ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeCutScenePlayed)	
		{
			if(ActionRectAnsooyaTalk.Intersection(&pos))
			{
				Cursor::SetMode(CA_Speech);
				if(Control::Input->LBclicked())
				{
					dialogSystem->ShowDialog(eDialog_L5S4Anasooya);
					isDialogActive = true;
					Hud->AllowInput = false;
					Hud->HideHud(true);
				}
				return;
			}
			
			
		}
	else if( ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked && !ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete)	
		{
			if(ActionRectAnsooyaTalk.Intersection(&pos))
			{
				Cursor::SetMode(CA_Look);
				if(Control::Input->LBclicked())
				{
					Cursor::SetMode(CA_Normal);
					ActivePopupID = ePop3_HandCuff;
					PopupArray[ePop3_HandCuff]->OpenPopup();
				}
				return;
			}
			
			
		}
	else if( ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete && !ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken)	
		{
			if(ActionRectAnsooyaTalk.Intersection(&pos))
			{
				Cursor::SetMode(CA_Speech);
				if(Control::Input->LBclicked())
				{
					dialogSystem->ShowDialog(eDialog_L5S4AnasooyaFree);
					isDialogActive = true;
					Hud->AllowInput = false;
					Hud->HideHud(true);
				}
				return;
			}
			
		}
	 if( !ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeCutScenePlayed )	
		{
			if(InfoRectPond.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L5stream");
				}
				return;
			}
			
			
		}

	 if( InfoRectBridge.Intersection(&pos))	
		{
			if( !ProfileSystem::Instance->ActiveProfile.L5_S4_BridgeOpened)
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					if(!ProfileSystem::Instance->ActiveProfile.L5_S4_PickAxeCutScenePlayed)
					{
						if(!ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted)
							Hud->ShowBannerText("L5bridge");
						else
							Hud->ShowBannerText("L5heat");
					}
					else
					{
						if(!ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted)							
							Hud->ShowBannerText("L5fire");
					}
				}
			}
			
			return;
		}
	}

}

void L5_S4_Dungeon::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL5S4_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if(  !ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked &&  ProfileSystem::Instance->ActiveProfile.L5_S4_BridgeOpened )
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked && !ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete && ProfileSystem::Instance->ActiveProfile.L5_S5_InvHandcuffKeyTaken)
		isAnyTaskLeft = true;
	if(ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete && !ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S4_Dungeon] = isAnyTaskLeft;

	if( ProfileSystem::Instance->ActiveProfile.L5_S4_InvBarkTaken && !ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken) 
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S2_TigerCloseup] = true;
}

void L5_S4_Dungeon::SparkleUpdate()
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

		//Check for Popup Tasks
		for (int i = 0; i < ePopL5S4_Max; i++)
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

		if(  !ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked &&  ProfileSystem::Instance->ActiveProfile.L5_S4_BridgeOpened )
		{
			sparklePoints[numSparkles].SetXY(ActionRectAnsooyaTalk.GetCenterX(), ActionRectAnsooyaTalk.GetCenterY());
			numSparkles++;
		}
		else if( ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked && !ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete && ProfileSystem::Instance->ActiveProfile.L5_S5_InvHandcuffKeyTaken)
		{
			sparklePoints[numSparkles].SetXY(ActionRectAnsooyaTalk.GetCenterX(), ActionRectAnsooyaTalk.GetCenterY());
			numSparkles++;
		}
		else if(ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete && !ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken)	
		{
			sparklePoints[numSparkles].SetXY(ActionRectAnsooyaTalk.GetCenterX(), ActionRectAnsooyaTalk.GetCenterY());
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

void L5_S4_Dungeon::ReceiveMessage(int val)
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
	if( val == HOSM_DialogClosed ) 
	{
		isDialogActive = false;
		Hud->AllowInput = true;
		Hud->ShowHud(true);
		if(ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken)
		{
			HOScene->GetObjectByID("bracelet")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L5_S4_PeacockChain, HOScene->GetObjectByID("bracelet")->Pos);
			Hud->ShowBannerText("L5close");
			JAnim_Anasuya->PlayAnim();

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P6_LathikaFreed])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P6_LathikaFreed] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L5P6_LathikaFreed;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			//SceneObjectsUpdate();
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L5_S6_MountainTop])
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S6_MountainTop] = true;
		}
		else if(ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked)
		{
			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O5_FindWayToFreeAnasuya] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O5_FindWayToFreeAnasuya] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
				= eObj_L5O5_FindWayToFreeAnasuya;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L5O5_FindWayToFreeAnasuya);

				Control::Audio->PlaySample(aSFXMagic1);
			}

			Hud->UnlockCharacter(eChar16_Anasooya, 3.0f);

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P4_MeetingLathika])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P4_MeetingLathika] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L5P4_MeetingLathika;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			Hud->ShowBannerText("L5poor");
			Control::Audio->PlaySample(aVOL5MaryRefS4_1);
			PopStates[ePop3_HandCuff] = ePopupState_Closed;
		}
		
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
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

void L5_S4_Dungeon::ProcessSkip()
{
	if (ActivePopupID == ePop1_Bridge)
	{
		((L5S4_Pop1Bridge*) PopupArray[ActivePopupID])->ProcessSkip();
	}
}

void L5_S4_Dungeon::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S4Collected)
		{
			InitHintTrailEmit(320, 330);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S4Watermelon])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(690, 250);
			else
				InitHintTrailEmit(480, 380);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}
	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
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
			for (int i = 0; i < ePopL5S4_Max; i++)
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
			if(  !ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked &&  ProfileSystem::Instance->ActiveProfile.L5_S4_BridgeOpened )
			{
			InitHintTrailEmit(&ActionRectAnsooyaTalk);
			return;
			}
			break;
		case 3: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked && !ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete && ProfileSystem::Instance->ActiveProfile.L5_S5_InvHandcuffKeyTaken)
			{
			InitHintTrailEmit(&ActionRectAnsooyaTalk);
			return;
			}
			break;
		case 4: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete && !ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken)
			{
			InitHintTrailEmit(&ActionRectAnsooyaTalk);
			return;
			}
			break;
		case 5: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S3_Pond])
			{
			InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
			return;
			}
			break;
		case 6: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
		case 7: ++iNonHOHint;
				{
					Hud->teleportSystem->ShowTeleport();
					return;
				}
				break;

		}
	}
	while( iNonHOHint < MaxNonHOHints );

	Hud->ShowNothingToDoMsgBanner();
}

L5_S4_Dungeon::~L5_S4_Dungeon()
{
	SAFE_DELETE(S4MoonPrtFX);
	SAFE_DELETE(S4waterFallFX);
	SAFE_DELETE(S4waterFallDownFX);
	SAFE_DELETE(S4CrystalParticle1FX);
	SAFE_DELETE(S4CrystalParticle2FX);
	SAFE_DELETE(S4CrystalParticle3FX);
	SAFE_DELETE(S4Lava1FX);
	SAFE_DELETE(S4Lava2FX);
	SAFE_DELETE(S4Lava3FX);

	SAFE_DELETE(JAnim_Bridge);
	SAFE_DELETE(JAnim_Anasuya);

	for (int i = 0; i < ePopL5S4_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}
	SAFE_DELETE(cv_WaterCutscn);
	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL5MaryRefS4_1);
	Control::Audio->StopSample(aAmbL5s4DungeonAmb);
	Control::Audio->UnloadSample(aAmbL5s4DungeonAmb);
	//MUSIC_SFX
}
void L5_S4_Dungeon::NavigateToScene()
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