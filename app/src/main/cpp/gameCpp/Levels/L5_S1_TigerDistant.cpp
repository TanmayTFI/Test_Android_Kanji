//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S1_TigerDistant.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);

const CRectangle  SceneRectCloseup(275.0f,99.0f,743.0f,309.0f);

const CRectangle  InfoSkeletonRect(404.0f,305.0f,154.0f,78.0f);

L5_S1_TigerDistant::L5_S1_TigerDistant()
{
	
	//Main Scene
	LoadHud(INVENTORY_V1);
	AwardManager::GetInstance()->CollectAward(kAward_Find_Who_Wreck_Indus);
	LoadScene("LV5\\S1\\S1.SCN");

	HOScene->GetObjectByID("blueglow1")->SetScale(2.00f);
	HOScene->GetObjectByID("blueglow2")->SetScale(2.50f);

	HOScene->GetObjectByID("blueglow1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow1")->Animate(0.15f,0.65f,7.0f);

	HOScene->GetObjectByID("blueglow2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow2")->Animate(0.15f,0.85f,4.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	isTigerBannerShown = false;
	tigerBannerTimer = -1.0f;

	meshLeaf = NULL;
	meshLeaf = new MeshObject(HOScene->GetObjectByID("meshleaf"), 5, 5);
	meshLeaf->meshFX->SetSpeedAndDisplacement(4, 3, 3, 3);

	HOScene->PushObject(meshLeaf);

	HOScene->GetObjectByID("tigerroar")->SetEnable(false);

	/*_JAnim_TigerLoop = new JSONAnimator(0);
	_JAnim_TigerLoop->parseAndLoadJSON("ANIM\\L5\\S1TIGERSCNLOOP.JSON", HOScene);
	_JAnim_TigerLoop->PlayAnim(true);*/

	InitPopups();

	SceneObjectsUpdate();

	TaskPendingUpdate();

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s1TigerDistantAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s1TigerDistantAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s1TigerDistantAmb,true);
	}
	
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S1_TigerDistant]  = true;
}

void L5_S1_TigerDistant::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL5S1_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	int popID=0;


	//Popup 1
	popID = ePop1_L5S1Pop1Bow;
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowTaken)
	{
		int pop1Pts[] = {871,437,1065,277,1194,284,1193,381,1029,574,861,533};
		CPolygon* pop1ClickPoly=  new CPolygon(6);
		pop1ClickPoly->Init(pop1Pts);

		Popup* Pop1Scene = new L5S1_Pop1Bow(popID, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L5_S1_TigerDistant::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop1ClickPoly;
		PopupArray[popID]	= Pop1Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2
	popID = ePop2_L5S1Pop2Arrow;
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
	{
		int pop2Pts[] = {557,430,574,408,728,394,786,446,651,498,561,475};
		CPolygon* pop2ClickPoly=  new CPolygon(6);
		pop2ClickPoly->Init(pop2Pts);

		Popup* Pop2Scene = new L5S1_Pop2Arrow(popID, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L5_S1_TigerDistant::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopCircleRect);

		PopPolyArray[popID] = pop2ClickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 3
	popID = ePop3_L5S1Pop3Tiger;
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
	{
		int pop3Pts[] = {565,222,725,219,797,290,743,382,559,383,513,294};
		CPolygon* pop3ClickPoly=  new CPolygon(6);
		pop3ClickPoly->Init(pop3Pts);

		Pop3Scene = new L5S1_Pop3Tiger(popID, Hud, HOScene, eZOrder_Popup, pop3ClickPoly->center, CC_CALLBACK_1(L5_S1_TigerDistant::OnPopupClosed, this));
		Pop3Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop3ClickPoly;
		PopupArray[popID]	= Pop3Scene;

		PopStates[popID] = ePopupState_Closed;
	}
}

void L5_S1_TigerDistant::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	int popID = popup->GetPopupId();
	
	if(popID == ePop1_L5S1Pop1Bow)
	{
		if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowTaken)			
		{
			isSparkleUpdatePending = true;
			PopStates[popID] = ePopupState_Inactive; //disable popup
		}
	}
	else if(popID == ePop2_L5S1Pop2Arrow)
	{
		if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)			
		{
			isSparkleUpdatePending = true;
			PopStates[popID] = ePopupState_Inactive; //disable popup
		}
	}
	else if(popID == ePop3_L5S1Pop3Tiger)
	{
		if(ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)			
		{
			isSparkleUpdatePending = true;

			tigerBannerTimer = 1.15f;

			PopStates[popID] = ePopupState_Inactive; //disable popup

			Hud->ReleaseDrag();
			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S1_BowArrowFixed] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S1_BowArrowFixed);

			Hud->TakeInventoryItem(eInv_L5_S1_Bow, *PointSystem::CreateCPoint(PopAreaRect.GetCenterX(), PopAreaRect.GetCenterY(), 0));

			//Show Objective completed
			if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O1_GetRidOfTiger] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O1_GetRidOfTiger] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L5O1_GetRidOfTiger] = true;
				Hud->ShowObjectiveFX(eObj_L5O1_GetRidOfTiger,true);
			}

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P1_TigerCutscene])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P1_TigerCutscene] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L5P1_TigerCutscene;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}
		}
	}

	TaskPendingUpdate();
}

void L5_S1_TigerDistant::SceneObjectsUpdate()
{
	if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowTaken)
		HOScene->GetObjectByID("bow")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
		HOScene->GetObjectByID("arrowholder")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
	{
		/*if(_JAnim_TigerLoop)
			_JAnim_TigerLoop->StopAnim();*/

		HOScene->GetObjectByID("tigerroar")->SetEnable(false);
		HOScene->GetObjectByID("tigerside")->SetEnable(false);
		HOScene->GetObjectByID("tigerbody")->SetEnable(false);
	}

	TaskPendingUpdate();
}

void L5_S1_TigerDistant::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
		if(Pop3Scene->isTigerShootCutscnPlaying)
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

void L5_S1_TigerDistant::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();

	if(!isTigerBannerShown)
	{
		if(tigerBannerTimer>0)
		{
			tigerBannerTimer -= Control::LogicRate;
			if(tigerBannerTimer<0)
			{
				Hud->ShowBannerText("L5leopard");
				isTigerBannerShown = true;
			}
		}
	}

	if( ActivePopupID == ePopInactive )
	{
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
		#endif
		Cursor::SetMode(CA_Normal);
		if(ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
		{
			if(SceneRectCloseup.Intersection(&pos))
			{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S2_TigerCloseup, 0, SceneRectCloseup);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_TigerCloseup;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
					
				}
			}
			return;
		}

		if(InfoSkeletonRect.Intersection(&pos))
		{
			//Cursor::SetMode(CA_ExitUp, -1);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L5give");
			}
			return;
		}

		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		//popup click checks
		for (int i = 0; i < ePopL5S1_Max; i++)
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
	}
}

void L5_S1_TigerDistant::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL5S1_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S1_TigerDistant] = isAnyTaskLeft;
}

void L5_S1_TigerDistant::SparkleUpdate()
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
		for (int i = 0; i < ePopL5S1_Max; i++)
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

void L5_S1_TigerDistant::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();
		//check if Bow and arrow is combined
		if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowFirstTaken)
		{
			isSparkleUpdatePending = true;
		}

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
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				//popup click checks
				for (int i = 0; i < ePopL5S1_Max; i++)
				{
					if(PopStates[i] != ePopupState_Inactive) //if popup is active
					{
						if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&dropPos))
						{
							ActivePopupID = i;
							PopupArray[i]->OpenPopup();
							break;
						}
					}
				}

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

void L5_S1_TigerDistant::ProcessSkip()
{
}

void L5_S1_TigerDistant::ProcessNonHOHint()
{
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
			for (int i = 0; i < ePopL5S1_Max; i++)
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
			if(ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
			{
				InitHintTrailEmit(&SceneRectCloseup,false);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			/*if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S5_MoopansDen])
			{
			InitHintTrailEmit(&ScnRectMooppansDen,true,CA_ExitLeft);
			return;
			}*/
			break;
		case 4: ++iNonHOHint;
			/*if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd])
			{
			InitHintTrailEmit(&ScnRectTunnelEnd,true,CA_ExitRight);
			return;
			}*/
			break;
		case 5: ++iNonHOHint;
			/*if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S3_CaveMouth])
			{
			InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
			return;
			}*/
			break;
		case 6: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
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

L5_S1_TigerDistant::~L5_S1_TigerDistant()
{
	//SAFE_DELETE(_JAnim_TigerLoop);

	for (int i = 0; i < ePopL5S1_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL5s1TigerDistantAmb);
	Control::Audio->UnloadSample(aAmbL5s1TigerDistantAmb);
	//MUSIC_SFX
}
void L5_S1_TigerDistant::NavigateToScene()
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