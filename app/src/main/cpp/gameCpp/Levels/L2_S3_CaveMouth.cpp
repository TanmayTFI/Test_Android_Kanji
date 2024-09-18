//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 : Forest Entrance
//====================================

#include "L2_S3_CaveMouth.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "TextureFactory.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "AwardManager.h"

const CPoint		CenterPoint(683,334,0);
const CRectangle	PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle	PopCircleAreaRect(502.0f,153.0f,364.0f,360.0f);

//Info Rects
const CRectangle  InfoRectCaveEntranceBlocked(502.0f,244.0f,274.0f,238.0f);
const CRectangle  InfoRectWater(488.0f,533.0f,457.0f,80.0f);

const CRectangle  InfoRectFloatingRocks(526.0f,534.0f,241.0f,75.0f);

//Scene Rects
const CRectangle  SceneRectCave1(500.0f,232.0f,259.0f,294.0f);
const CRectangle  PZRectStoneBeam(314.0f,305.0f,174.0f,258.0f);

const CRectangle  PZRectStoneBeamCenter(382.0f,395.0f,83.0f,83.0f);

L2_S3_CaveMouth::L2_S3_CaveMouth()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S3\\S3.SCN");
	musicFadeOutTimer = 0;
	SCN_turmericPlants = HOScene->GetObjectByID("turmeric-cut-after");
	Scn_PoojaLamp = HOScene->GetObjectByID("lamp");
	Scn_PoojaLamp->SetEnable(false);

	SCN_caveRoots = HOScene->GetObjectByID("roots_Cave");
	for (int i = 0; i < 5; i++)
	{
		std::string str = "vines0";
		str += std::to_string(i + 1);
		SCN_vines[i] = HOScene->GetObjectByID(str);
		switch (i)
		{
		case 0:
			SCN_vines[i]->SetPivot(-74, -47.5);
			SCN_vines[i]->Animate(3, 0, 2, 0, 5, 2);
			break;
		case 1:
			SCN_vines[i]->SetPivot(-18, -174);
			SCN_vines[i]->Animate(0, 0, 7, 0, 7);
			break;
		case 2:
		case 4:
			SCN_vines[i]->SetPivot(-23, -22.5);
			SCN_vines[i]->Animate(0, 0, 3, 0, 5, 1);
			break;
		case 3:
			SCN_vines[i]->SetPivot(-21.5, -13.5);
			SCN_vines[i]->Animate(0, 0, 4, 0, 6, 3);
			break;
		default:
			break;
		}
	}
	HOScene->GetObjectByID("BranchAnimation")->Animate(2, 10, 0, 0, 7, 2);
	HOScene->GetObjectByID("mushroom")->Animate(3, 0, 0, 0, 3, 2);
	HOScene->GetObjectByID("plant1")->SetPivot(0, 50);
	HOScene->GetObjectByID("plant1")->Animate(0, 0, 2, 0, 4);
	HOScene->GetObjectByID("water")->SetEnable(false);

	for (int i = 0; i < 4; i++)
	{
		std::string str = "Stone";
		str += std::to_string(i + 1);
		SCN_stone[i] = HOScene->GetObjectByID(str);
		SCN_stone[i]->SetScale(1 - i*0.15f);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S3_IsStonePathActivatedMessageShown)
	{
		for(int i = 0; i < 4; i++)
			SCN_stone[i]->PlayAnimation(2);
	}

	SceneObjectsUpdate();

	GFInstance->PostUpdate(1);

	InitPopups();
	
	

	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L2_S1_ForestEntrance]  = true;

	// popups

	int p1rect[] = {660,537,740,542,791,505,796,446,755,398,665,408,630,466,637,517};
	pop1ClickPoly = new ClickablePolygon(0, 8, p1rect, CC_CALLBACK_1(L2_S3_CaveMouth::onPopAreaClicked,this));
	
	pop1Turmeric = new L2S3_Pop1Turmeric(eHOPop1_Turmeric, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L2_S3_CaveMouth::onPopupClosed, this));
	pop1Turmeric->SetPopupBoundingBox(PopAreaRect);
	PopUpArray[eHOPop1_Turmeric] = pop1Turmeric;



	isMouseOverPopup = false;

	fogVFXL = new SuperFX("PRT\\L2\\S3_snowL.INI", PointSystem::CreateCPoint(300,300,0), 2);
	fogVFXL->Start();

	fogVFXR = new SuperFX("PRT\\L2\\S3_snowR.INI", PointSystem::CreateCPoint(800,300,0), 2);
	fogVFXR->Start();

	puzCompleteGlow = new SuperFX("PRT\\L2\\S3Particle.INI", PointSystem::CreateCPoint(PZRectStoneBeamCenter.GetCenterX(),PZRectStoneBeamCenter.GetCenterY(),0), 2);

	HOScene->GetObjectByID("Thiri")->SetScale(0.2f);
	CPoint *thiriPos = new CPoint(10,95,0);
	HOScene->GetObjectByID("Thiri")->SetRelativePos(thiriPos);
	delete(thiriPos);

	#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S3Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(895.0f,472.0f,65.0f,96.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

	TaskPendingUpdate();

	

			Control::Audio->LoadSample(aAmbs3CaveMouth,AudioAmb);
			if( !Control::Audio->IsPlaying(aAmbs3CaveMouth) )		
			{
				Control::Audio->PlaySample(aAmbs3CaveMouth,true);
			}
			if(ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated)
				Hud->PrevCursorMode = -1;
}

void L2_S3_CaveMouth::onPopAreaMouseOut(int popId)
{
	isMouseOverPopup = false;
}

void L2_S3_CaveMouth::onPopAreaMouseOver(int popId)
{
	if (ActivePopupID != eHOS3Pop_Inactive)
		return;

	switch (popId)
	{
	case eHOPop1_Turmeric: if( ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated && (!ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked))
										isMouseOverPopup = true;
										break;
	
	default:
		break;
	}

}

void L2_S3_CaveMouth::onPopAreaClicked(int popId)
{
	if (ActivePopupID != eHOS3Pop_Inactive)
		return;

	switch (popId)
	{
	case eHOPop1_Turmeric:
		if( ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated && (!ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked))
		{
			ActivePopupID = eHOPop1_Turmeric;
			pop1Turmeric->OpenPopup();
		}
		break;
	
	default:
		break;
	}
}

void L2_S3_CaveMouth::SparkleUpdate()
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

		if( !ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked  && ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
		{
			sparklePoints[numSparkles].SetXY(pop1ClickPoly->center.x, pop1ClickPoly->center.y );
			numSparkles++;
		}
		if( !ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FrogJumpPlayed && ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated)
		{
			sparklePoints[numSparkles].SetXY(pop1ClickPoly->center.x, pop1ClickPoly->center.y );
			numSparkles++;
		}

		if( !ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated  )
		{
			sparklePoints[numSparkles].SetXY(PZRectStoneBeam.GetCenterX(), PZRectStoneBeam.GetCenterY());
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

void L2_S3_CaveMouth::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	if( !ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked  && ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
			isAnyTaskLeft = true;		
	if( !ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S4_Cave] )
			isAnyTaskLeft = true;		


	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S3_CaveMouth] = isAnyTaskLeft;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced &&	ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] = true;
}

void L2_S3_CaveMouth::SceneObjectsUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked)
	{
		SCN_turmericPlants->SetEnable(false);
	}

	
	
	if  (ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown )
	{
		SCN_caveRoots->SetEnable(false);
	}

	TaskPendingUpdate();
}

void L2_S3_CaveMouth::InitPopups()
{	
	ActivePopupID = ePopInactive;	
}

void L2_S3_CaveMouth::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL2theme1))
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
				Control::Audio->ClearUnusedTracks();
				Control::Audio->PlaySample(aTrackL2theme, true);
			}
		}
}

void L2_S3_CaveMouth::Update()
{
	NavigateToScene();

	//Cheats goes here
	#ifdef _TABINGAMEUI
		if(_TabInGameMenuShowing)
			return;
	#endif // _TABINGAMEUI

	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif
	
	if( Hud->PopupState != Popup_Inactive )
		return;

#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if (isMouseOverPopup)
	{
		Cursor::SetMode(CA_Look);
		return;
	}

	CPoint pos = Control::Input->Mpos();


	if( ActivePopupID == ePopInactive )
	{	
	#ifdef _CEBUILD
		if(!Hud->isMouseOverExtraButton)
			Cursor::SetMode(CA_Normal);
	#else
		Cursor::SetMode(CA_Normal);
	#endif 

		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		if( ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated )
		{
			if(!ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked)
			{
				if(pop1ClickPoly->Intersection(&pos))
				{
					Cursor::SetMode(CA_Look);
				
					return;
				}
			}

			if (!ProfileSystem::Instance->ActiveProfile.L2_S3_IsStonePathActivatedMessageShown)
			{
				Hud->ShowBannerText("L2cross");
				ProfileSystem::Instance->ActiveProfile.L2_S3_IsStonePathActivatedMessageShown = true;
				puzCompleteGlow->Start();
				for(int i = 0; i < 4; i++)
						SCN_stone[i]->PlayAnimation(1);
				Control::Audio->QuickLoadAndPlaySFX("s3_stonerise");
			}

			if( SceneRectCave1.Intersection(&pos))
			{

#ifdef TOUCH_UI
				//if(!GFHud->InvBoxCtrl->navTapped)
				//	Cursor::SetMode(CA_ExitUp,eArea_L2_S4_Cave);
#else
				Cursor::SetMode(CA_ExitUp,eArea_L2_S4_Cave);
#endif

				if( Control::Input->LBclicked() )
				{
#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						GFHud->InvBoxCtrl->GoToScene(eArea_L2_S4_Cave, 0, SceneRectCave1);
					}
#else
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_Cave;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				}
				return;
			}
		}
		else
		{
			if ( PZRectStoneBeam.Intersection(&pos) )
			{

#ifdef TOUCH_UI
				//if(!GFHud->InvBoxCtrl->navTapped)
					//Cursor::SetMode(CA_Look);
#else
				Cursor::SetMode(CA_Look);
#endif

				if( Control::Input->LBclicked() )
				{
#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						GFHud->InvBoxCtrl->GoToScene(eArea_L2_S3_PZStoneBeam, 0, PZRectStoneBeam);
					}
#else
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_PZStoneBeam;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				} 
				return;
			}
			else if ( InfoRectCaveEntranceBlocked.Intersection(&pos) || InfoRectWater.Intersection( &pos))
			{
				Cursor::SetMode(CA_Help);
				if( Control::Input->LBclicked() )
				{
					Hud->ShowBannerText("L2across");
				}
				return;
			}	
		}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
	#endif
		if( IsMouseOverGoBackArea(&pos) )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitDown,eArea_L2_S1_ForestEntrance);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L2_S1_ForestEntrance);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S1_ForestEntrance, 2);
				}
#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}	
			return;
		}

		if ( ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated && InfoRectFloatingRocks.Intersection(&pos) )
		{
			Cursor::SetMode(CA_Help);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L2float");
			}
			return;
		}
		else if (pos.y > 550 || pos.y < 100 || pos.x > 1200 || pos.x < 100)
		{
			Cursor::SetMode(CA_Help);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2cafe");
			}
		}
	}
}

void L2_S3_CaveMouth::onPopupClosed(Popup * popup)
{
	//K_LOG("popup closed %d", popup->GetPopupId());

	SceneObjectsUpdate();

	if(popup->GetPopupId() == eHOPop1_Turmeric)
	{
		if( ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked )
			isSparkleUpdatePending = true;
	}

	ActivePopupID = ePopInactive;
}

L2_S3_CaveMouth::~L2_S3_CaveMouth()
{
	TaskPendingUpdate();

	SAFE_DELETE(pop1Turmeric);
	//SAFE_DELETE(pop2Lighter);
	SAFE_DELETE(pop1ClickPoly);
	//SAFE_DELETE(pop2ClickPoly);

	SAFE_DELETE(fogVFXL);
	SAFE_DELETE(fogVFXR);
	if (puzCompleteGlow)
		SAFE_DELETE(puzCompleteGlow);

	//MUSIC_SFX
	//Quick SFX unloaded in HUD
	Control::Audio->StopSample(aAmbs3CaveMouth);
	Control::Audio->UnloadSample(aAmbs3CaveMouth);
	//MUSIC_SFX
}

void L2_S3_CaveMouth::ReceiveMessage(int val)
{
	if( val == HOSM_DialogClosed ) 
	{
		//isDialogActive = false;
		Hud->AllowInput = true;
		Hud->ShowHud(true);

		isSparkleUpdatePending = true;
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
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
					if (itemVal == eInv_L2S5_PotEmpty)
					{
						Hud->ShowBannerText("L2clean");
						Hud->CancelDrag();
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				return;
			}
		
			switch (ActivePopupID)
			{
			case eHOPop1_Turmeric:
				pop1Turmeric->HandleInventoryDrop(dropPos, itemVal);
				break;
			/*case eHOPop2_Lighter:
				pop2Lighter->HandleInventoryDrop(dropPos, itemVal);
				break;*/
			default:
				break;
			}
		}
			
	}
}

void L2_S3_CaveMouth::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S3Collected)
		{
			InitHintTrailEmit(920, 500);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	if (ActivePopupID != ePopInactive)
	{
		
		PopUpArray[ActivePopupID]->ProcessHint();
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 6;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked  && ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
					{
						InitHintTrailEmit(pop1ClickPoly->center.x, pop1ClickPoly->center.y,false);
						return;
					}
					break;
			case 1: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated  )
					{
						InitHintTrailEmit(PZRectStoneBeam.GetCenterX(), PZRectStoneBeam.GetCenterY(),false);
						return;
					}
					break;
			case 2: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_ForestEntrance])
					{
						InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S4_Cave] || (!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S4_Cave] && ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated))
					{
						InitHintTrailEmit(SceneRectCave1.GetCenterX(), SceneRectCave1.GetCenterY(),true,CA_ExitUp);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
			case 5: ++iNonHOHint;
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


void L2_S3_CaveMouth::NavigateToScene()
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