//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room1Start
//====================================

#include "L3_S2_P1Room1Start.h"
#include "Application.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Rect.h"
#include <string>

const CRectangle  SceneCnter(620.0f,166.0f,427.0f,289.0f);
const CRectangle  PoolRect(509.0f,507.0f,301.0f,159.0f);
const CRectangle  InvDropDoor1(191.0f,109.0f,201.0f,220.0f);
const CRectangle  InvDropDoor2(591.0f,45.0f,269.0f,339.0f);
const CRectangle  InvDropDoor3(1060.0f,117.0f,176.0f,210.0f);

const CRectangle DoorRectsArray[] = {InvDropDoor1, InvDropDoor2, InvDropDoor3};
const eCursorAnim NextAreaPoint[] = {CA_ExitLeft, CA_ExitUp, CA_ExitRight};

static std::string VinePos[3] = {"Left", "Center", "Right"};

static int SolutionDoor = 2;

L3_S2_P1Room1Start::L3_S2_P1Room1Start()
{
	Visible = false;
	musicFadeOutTimer = 0;
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S2\\MAZE1\\MAZE1.SCN");

	_wrongScene = NULL;

	S2M1LightFX = new SuperFX("PRT\\L3\\S2M1LIGHT.INI", PointSystem::CreateCPoint(692,265,0), 30); 
	S2M1LightFX->Start();
	S2M1LightFX->AdvanceByTime(2.0f);


	JSONAnimator *janim_RemoveLeftVine = new JSONAnimator(0, CC_CALLBACK_1(L3_S2_P1Room1Start::OnAnimatonComplete, this));
	janim_RemoveLeftVine->parseAndLoadJSON("ANIM\\L3\\S2MAZE1_LEFTVINE.JSON",HOScene);

	JSONAnimator *janim_RemoveCenterVine = new JSONAnimator(1, CC_CALLBACK_1(L3_S2_P1Room1Start::OnAnimatonComplete, this));
	janim_RemoveCenterVine->parseAndLoadJSON("ANIM\\L3\\S2MAZE1_CENTERVINE.JSON",HOScene);
	
	JSONAnimator *janim_RemoveRightVine = new JSONAnimator(2, CC_CALLBACK_1(L3_S2_P1Room1Start::OnAnimatonComplete, this));
	janim_RemoveRightVine->parseAndLoadJSON("ANIM\\L3\\S2MAZE1_RIGHTVINE.JSON",HOScene);

	_janim_RemoveVine.push_back(janim_RemoveLeftVine);
	_janim_RemoveVine.push_back(janim_RemoveCenterVine);
	_janim_RemoveVine.push_back(janim_RemoveRightVine);

	HOScene->GetObjectByID("popfireanim_1")->SetScale(1.5);
	HOScene->GetObjectByID("popfireanim_2")->SetScale(1.5);
	HOScene->GetObjectByID("sickle")->SetAlpha(0);

	_pipeInWater = HOScene->GetObjectByID("pipe_underWater");

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken)
	{
		_pipeInWater->SetEnable(false);
	}

	_waterMask = HOScene->GetObjectByID("waterMask");
	_waterMask->Show(false);

	MeshObject* waterMesh = new MeshObject(HOScene->GetObjectByID("water"), 5, 5);
	waterMesh->meshFX->SetSpeedAndDisplacement(5, 3, 5, 3);
	HOScene->InsertObject(waterMesh, 1);
	
	for (int i = 0; i < 3; i++)
	{
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR1Status[i])
		{
			HOScene->GetObjectByID("Vine_"+ VinePos[i] +"1")->SetEnable(false);
			HOScene->GetObjectByID("Vine_"+ VinePos[i] +"2")->SetEnable(false);
		}
	}

	CObject *light_ray1 = HOScene->GetObjectByID("light_top");
	light_ray1->Animate(0.5f, 0.7f, 4);

	for (int i = 1; i <= 8; i++)
	{
		HOScene->GetObjectByID("vines_"+MKSTR(i))->Animate(0, (float)(4 + rand() % 5), 0, 0, (float)(3 + rand() % 3), (2.0f + 0.65f * i));
		
		if (i <= 2)
		{
			CObject *leaf = HOScene->GetObjectByID("l_"+MKSTR(i));
			leaf->SetPivot(0, -28);
			leaf->Animate(0, 2, 5, 0, 2);
		}
	}

	CObject *light_ray2 = HOScene->GetObjectByID("light_down");
	light_ray2->Animate(0.15f, 0.55f, 4);
	light_ray2->SetBlendMode(eBlend_AlphaAdditive);

	_activePopup = R1_NoPopup;

	CPoint zoomOrigin = CPoint(PoolRect.GetCenterX(), PoolRect.GetCenterY(), 0);
	_pipePopup = new L3_S2_M1PipePopUp(0, Hud, HOScene, eZOrder_Popup, zoomOrigin, CC_CALLBACK_1(L3_S2_P1Room1Start::OnClosePopup, this));

	Visible = true;	//To render anything with renderlistentity

	ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom = eArea_L3_S2_P1Room1Start;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
	{
		HOScene->Load("LV3\\S2\\MAZECOMMON\\MAP\\MAPHAND.SCN");
		for (int i = 0; i < 5; i++)
		{
			if(i!=0)
				HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->SetEnable(false);
			else
				HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->Animate(0.85f, 1.0f, 1.25f);
		}
	}
	else
		Hud->ShowBannerText(Banner_L3_S1_M1_WithoutMap);
	
	_jAnim_PlaceMap = NULL;

	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s2Maze,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s2Maze) )		
	{
		Control::Audio->PlaySample(aAmbL3s2Maze,true);
	}
	
	//MUSIC_SFX

	TaskPendingUpdate();
}

L3_S2_P1Room1Start::~L3_S2_P1Room1Start()
{
	SAFE_DELETE(_pipePopup);
	SAFE_DELETE(S2M1LightFX);
	SAFE_DELETE(_jAnim_PlaceMap);

	for (int i = 0; i < (int)(_janim_RemoveVine.size()); i++)
	{
		SAFE_DELETE(_janim_RemoveVine[i]);
	}

	_janim_RemoveVine.clear();

		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s2Maze);
	Control::Audio->UnloadSample(aAmbL3s2Maze);
	//MUSIC_SFX
}

void L3_S2_P1Room1Start::Required()
{
	HiddenObjectSystem::Required();	//forced update

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

void L3_S2_P1Room1Start::Update()
{
	NavigateToScene();
	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	if (_activePopup != R1_NoPopup)
		return;

	CPoint pos = Control::Input->Mpos();	
	Cursor::SetMode(CA_Normal);

	for (int i = 0; i < (int)(_janim_RemoveVine.size()); i++)
	{
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR1Status[i] &&
			DoorRectsArray[i].Intersection(&pos))
		{
			if (ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
			{


#ifdef TOUCH_UI
			//	if(!GFHud->InvBoxCtrl->navTapped)
			//		Cursor::SetMode(NextAreaPoint[i]);
#else
				Cursor::SetMode(NextAreaPoint[i]);
#endif

				if( Control::Input->LBclicked() )
				{
#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						if (i == SolutionDoor)
						{
							GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room2Wine, 0, DoorRectsArray[i]);
						}
						else
						{
							GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1DeadEnd, 0, DoorRectsArray[i]);
						}
					}
#else
					if (i == SolutionDoor)
					{
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room2Wine;
						SendMessage(ControlExit, ID, HOSM_NewArea);
					}
					else
					{
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1DeadEnd;
						SendMessage(ControlExit, ID, HOSM_NewArea);
					}
#endif

				}
			}
			else
			{
				Cursor::SetMode(CA_Gear);
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L3_S1_M1_WithoutMap);
				}
			}
			
		}
		else if (DoorRectsArray[i].Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);

			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText(Banner_L3_S2_M1_ClickCreeper);
			}
		}
	}
	
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken && PoolRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Look);
		if (Control::Input->LBclicked())
		{
			_waterMask->Show(true);
			_pipePopup->OpenPopup();
			_activePopup = R1_PipePickPopup;
			if(HOScene->GetObjectByID("handMazeMap"))
				HOScene->GetObjectByID("handMazeMap")->Show(false);
			if(HOScene->GetObjectByID("posindicator0"))
			HOScene->GetObjectByID("posindicator0")->Show(false);
		}

		return;
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_FortInterior;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif

	if( IsMouseOverGoBackArea(&pos) )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L3_S2_FortInterior);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L3_S2_FortInterior);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_FortInterior, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_FortInterior;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
}

void L3_S2_P1Room1Start::Render(const CPoint* pCam)
{  
	
}

void L3_S2_P1Room1Start::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
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
		if( Hud->GetDropInfo( dropPos, itemVal ) )
			HandleInventoryDrop(dropPos, itemVal);
	}
	else if( val == INV_CLOSE )
	{		
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken)
		{
			_pipeInWater->SetEnable(false);
			_pipePopup->ClosePopup();
		}
	}	
}

void L3_S2_P1Room1Start::TaskPendingUpdate()
{
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPuzzleComplete)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = true;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken && !ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = false;
}

float L3_S2_P1Room1Start::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L3_S2_P1Room1Start::ProcessNonHOHint()
{
	if (_activePopup == R1_PipePickPopup)
	{
		_pipePopup->ProcessHint();
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken)
	{
		Hud->InitHintTrailEmit(&PoolRect);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken &&
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_FortInterior])
	{
		Hud->InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken &&
		!ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
	{
		Hud->ShowHintForInvItem(eInv_L3S4_MetalSheetMapDrawn, &SceneCnter);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
	{
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR1Status[SolutionDoor])
		{
			Hud->InitHintTrailEmit(&DoorRectsArray[SolutionDoor],false,CA_ExitRight);
			return;
		}

		 Hud->ShowHintForInvItem(eInv_L3S2_Sickle, &DoorRectsArray[SolutionDoor]);
	}
	else
	{
		Hud->teleportSystem->ShowTeleport();
	}
}

void L3_S2_P1Room1Start::OnClosePopup(Popup *popup)
{
	_waterMask->Show(false);
	_activePopup = R1_NoPopup;
	if (HOScene->GetObjectByID("handMazeMap"))
		HOScene->GetObjectByID("handMazeMap")->Show(true);
	if (HOScene->GetObjectByID("posindicator0"))
		HOScene->GetObjectByID("posindicator0")->Show(true);
	TaskPendingUpdate();
}

void L3_S2_P1Room1Start::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (_activePopup == R1_PipePickPopup)
	{
		_pipePopup->HandleInventoryDrop(dropPos, itemVal);
	}
	else
	{
		if (itemVal == eInv_L3S4_MetalSheetMapDrawn)
		{
			ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed = true;
			HOScene->Load("LV3\\S2\\MAZECOMMON\\MAP\\MAPHAND.SCN");
			for (int i = 0; i < 5; i++)
			{
				if(i!=0)
					HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->SetEnable(false);
				else
					HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->Animate(0.85f, 1.0f, 1.25f);
			}

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_MetalSheetMapDrawn] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S4_MetalSheetMapDrawn);

			_jAnim_PlaceMap = new JSONAnimator(4, nullptr);
			_jAnim_PlaceMap->parseAndLoadJSON("ANIM\\L3\\S2MAPHANDPLACE.JSON",HOScene);
			_jAnim_PlaceMap->PlayAnim();

			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O2_UnravelMazeMystery] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O2_UnravelMazeMystery] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
						= eObj_L3O2_UnravelMazeMystery;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L3O2_UnravelMazeMystery);
			}
		}

		else if (itemVal == eInv_L3S2_Sickle)
		{
			for (int i = 0; i < (int)(_janim_RemoveVine.size()); i++)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR1Status[i] && 
					DoorRectsArray[i].Intersection(&dropPos))
				{
					if(i==0)
						Control::Audio->QuickLoadAndPlaySFX("s2m1_vinecut");
					else if(i==1)
						Control::Audio->QuickLoadAndPlaySFX("s2m1_vinecutC");
					else if(i==2)
						Control::Audio->QuickLoadAndPlaySFX("s2m1_vinecutR");

					_janim_RemoveVine[i]->PlayAnim(false);
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					return;
				}
			}

			GFInstance->WrongInvDropMsg(Hud);
		}
		else
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
}

void L3_S2_P1Room1Start::OnAnimatonComplete(int animID)
{
	ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR1Status[animID] = true;
}

void L3_S2_P1Room1Start::SparkleUpdate()
{
	isSparkleUpdatePending = false;

	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (_activePopup != R1_NoPopup)
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken)
	{
		sparklePointsVec.push_back(CPoint(PoolRect.GetCenterX(), PoolRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L3_S2_P1Room1Start::NavigateToScene()
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
