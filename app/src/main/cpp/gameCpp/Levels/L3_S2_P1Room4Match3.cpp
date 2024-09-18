//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj / Faisal
// LEVEL3 - SCENE3 : P1Room4Match3
//====================================

#include "L3_S2_P1Room4Match3.h"
#include "Banners.h"
#include "Hud.h"
#include "Application.h"
#include "SDrop.h"
#include "Util.h"

#include "L3_S2_M4Pop1Match3.h"
#include "L3_S2_M4Pop2Pipe.h"

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  SceneRectExitLeft(188.0f,130.0f,113.0f,234.0f);
const CRectangle  SceneRectExitCenter(574.0f,250.0f,110.0f,150.0f);
const CRectangle  SceneRectExitRight(1067.0f,249.0f,107.0f,155.0f);


L3_S2_P1Room4Match3::L3_S2_P1Room4Match3()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S2\\MAZE4\\MAZE4.SCN");

	HOScene->GetObjectByID("blueglow")->SetScale(1.20f);

	HOScene->GetObjectByID("blueglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow")->Animate(0.25f,0.85f, 3.5f);

	HOScene->GetObjectByID("fireglow1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("fireglow1")->Animate(0.25f,0.85f, 3.0f, 3.34f);

	HOScene->GetObjectByID("fireglow2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("fireglow2")->Animate(0.25f,0.85f, 3.5f, 1.34f);

	HOScene->GetObjectByID("fireglow3")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("fireglow3")->Animate(0.25f,0.95f, 2.5f, 1.34f);

	HOScene->GetObjectByID("lighttop")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lighttop")->Animate(0.65f,1.0f, 4.0f, 1.34f);

	HOScene->GetObjectByID("doorglow1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("doorglow1")->Animate(0.2f,0.7f, 4.0f, 1.34f);

	HOScene->GetObjectByID("doorglow2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("doorglow2")->Animate(0.15f,0.7f, 3.0f, 1.34f);

	HOScene->GetObjectByID("doorglow3")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("doorglow3")->Animate(0.25f,0.65f, 5.0f, 1.34f);

	HOScene->GetObjectByID("floorglow")->SetScale(2.0f);
	HOScene->GetObjectByID("floorglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("floorglow")->Animate(0.65f,0.8f, 4.0f, 4.34f);

	HOScene->GetObjectByID("vine3")->Animate(0,4,0,0,6.0f, 3.5f);
	HOScene->GetObjectByID("vine2")->Animate(0,5,0,0,7.0f);
	HOScene->GetObjectByID("vine1")->Animate(0,4,0,0,4.0f);
	HOScene->GetObjectByID("vine4")->Animate(0,2,0,0,6.0f);
	HOScene->GetObjectByID("vine5")->Animate(0,3,0,0,5.0f);

	SuperFX *s2m4bluetopFX, *s2m4blueleftFX, *s2m4smokefgFX;
	SuperFX *s2m4fire1FX, *s2m4fire2FX, *s2m4fire3FX;

	s2m4fire1FX = new SuperFX("PRT\\L3\\s2m4fire1.INI", PointSystem::CreateCPoint(504,387,0), 100); 
	s2m4fire1FX->Start();
	s2m4fire1FX->AdvanceByTime(2.0f);

	s2m4fire2FX = new SuperFX("PRT\\L3\\s2m4fire2.INI", PointSystem::CreateCPoint(868,395,0), 100); 
	s2m4fire2FX->Start();
	s2m4fire2FX->AdvanceByTime(2.0f);

	s2m4fire3FX = new SuperFX("PRT\\L3\\s2m4fire3.INI", PointSystem::CreateCPoint(1236,391,0), 100); 
	s2m4fire3FX->Start();
	s2m4fire3FX->AdvanceByTime(2.0f);

	s2m4bluetopFX = new SuperFX("PRT\\L3\\s2m4bluetop.INI", PointSystem::CreateCPoint(708,115,0), 100); 
	s2m4bluetopFX->Start();
	s2m4bluetopFX->AdvanceByTime(2.0f);

	s2m4blueleftFX = new SuperFX("PRT\\L3\\s2m4blueleft.INI", PointSystem::CreateCPoint(168,93,0), 100); 
	s2m4blueleftFX->Start();
	s2m4blueleftFX->AdvanceByTime(2.0f);

	s2m4smokefgFX = new SuperFX("PRT\\L3\\s2m4smokefg.INI", PointSystem::CreateCPoint(712,657,0), 100); 
	s2m4smokefgFX->Start();		
	s2m4smokefgFX->AdvanceByTime(2.0f);

	VFXArray.push_back(s2m4bluetopFX);
	VFXArray.push_back(s2m4blueleftFX);
	VFXArray.push_back(s2m4smokefgFX);
	VFXArray.push_back(s2m4fire1FX);
	VFXArray.push_back(s2m4fire2FX);
	VFXArray.push_back(s2m4fire3FX);
	
	InitPopups();

	isDoorOpenAnimPlaying = false;

	JAnim_DoorsOpening = NULL;
	
	JAnim_DoorsOpening = new JSONAnimator(0, CC_CALLBACK_1(L3_S2_P1Room4Match3::OnAnimationComplete, this));
	JAnim_DoorsOpening->parseAndLoadJSON("ANIM\\L3\\S2M4DOORSOPEN.JSON", HOScene);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened)
	{
		
		HOScene->GetObjectByID("doorglow1")->SetEnable(false);
		HOScene->GetObjectByID("doorglow2")->SetEnable(false);
		HOScene->GetObjectByID("doorglow3")->SetEnable(false);
	}

	GFInstance->PostUpdate(2);

	SceneObjectsUpdate();

	Control::Audio->ClearUnusedTracks();
	
	if( !Control::Audio->IsPlaying(aTrackLevel3) )
		Control::Audio->PlaySample(aTrackLevel3,true);
	/* if( !Control::Audio->IsPlaying(aAmbRain) )		
	{
		Control::Audio->Samples[aAmbRain]->SetVolume(70);
		Control::Audio->PlaySample(aAmbRain,true);//will be paused in iPad popups!!
	}
	else //ensure 70
		Control::Audio->Samples[aAmbRain]->SetVolume(70);

	if( !Control::Audio->IsPlaying(aAmbBeachLoop) )		
	{
		Control::Audio->PlaySample(aAmbBeachLoop,true);//will be paused in iPad popups!!
	} */

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L3_S2_P1Room4Match3]  = true;

	
	ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom = eArea_L3_S2_P1Room4Match3;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
	{
		HOScene->Load("LV3\\S2\\MAZECOMMON\\MAP\\MAPHAND.SCN");
		for (int i = 0; i < 5; i++)
		{
			if(i!=3)
				HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->SetEnable(false);
			else
				HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->Animate(0.85f, 1.0f, 1.25f);
		}
	}

	TaskPendingUpdate();
}

void L3_S2_P1Room4Match3::InitPopups()
{	
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL3S2M4_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	int popID=0;

	//Popup 1
	popID = ePop1_Match3;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened)
	{
		int pop1Pts[] = {560,121,560,223,719,231,706,119};
		CPolygon* pop1ClickPoly=  new CPolygon(4);
		pop1ClickPoly->Init(pop1Pts);

		Popup* Pop1Scene = new L3_S2_M4Pop1Match3(popID, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L3_S2_P1Room4Match3::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop1ClickPoly;
		PopupArray[popID]	= Pop1Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2
	popID = ePop2_Pipe;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken)
	{
		int pop2Pts[] = {259,341,308,354,303,407,278,469,209,441};
		CPolygon* pop2ClickPoly=  new CPolygon(5);
		pop2ClickPoly->Init(pop2Pts);

		Popup* Pop2Scene = new L3_S2_M4Pop2Pipe(popID, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L3_S2_P1Room4Match3::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop2ClickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	GFInstance->PostUpdate(1);
}

L3_S2_P1Room4Match3::~L3_S2_P1Room4Match3()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(VFXArray[i]);
	}
	//VFXArray.clear();
	for (int i = 0; i < ePopL3S2M4_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	//JAnim_DoorsOpening->StopAnim();
	SAFE_DELETE(JAnim_DoorsOpening);
	
	if( !Control::Audio->IsToLevelTrackRoom() )
		if( Control::Audio->IsPlaying(aTrackLevel3) )
			Control::Audio->StopSample(aTrackLevel3);
			
	/*
	{
		Control::Audio->StopSample(aAmbRain);
		Control::Audio->Samples[aAmbRain]->SetVolume(100);			
	}
	Control::Audio->StopSample(aAmbBeachLoop); */
}


void L3_S2_P1Room4Match3::OnAnimationComplete(int id)
{
	isDoorOpenAnimPlaying = false;
}

void L3_S2_P1Room4Match3::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;	
}

void L3_S2_P1Room4Match3::SparkleUpdate()
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
		for (int i = 0; i < ePopL3S2M4_Max; i++)
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

void L3_S2_P1Room4Match3::SceneObjectsUpdate()
{
	if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken)
	{
		HOScene->GetObjectByID("pipe")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened)
	{
		HOScene->GetObjectByID("doorcenterL")->SetEnable(false);
		HOScene->GetObjectByID("doorcenterR")->SetEnable(false);
		HOScene->GetObjectByID("doorleftL")->SetEnable(false);
		HOScene->GetObjectByID("doorleftR")->SetEnable(false);
		HOScene->GetObjectByID("doorrightL")->SetEnable(false);
		HOScene->GetObjectByID("doorrightR")->SetEnable(false);
	}

	TaskPendingUpdate();
}

void L3_S2_P1Room4Match3::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL3S2M4_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = isAnyTaskLeft;
}


void L3_S2_P1Room4Match3::ProcessSkip()
{
	if(ActivePopupID == ePop1_Match3)
	{
		if(PopStates[ePop1_Match3] != ePopInactive && PopupArray[ePop1_Match3]!=NULL)
			PopupArray[ePop1_Match3]->ProcessHint();
	}
}

void L3_S2_P1Room4Match3::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	int popID = popup->GetPopupId();
	if(popID == ePop1_Match3)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened)
		{
			HOScene->GetObjectByID("doorglow1")->SetEnable(true);
			HOScene->GetObjectByID("doorglow2")->SetEnable(true);
			HOScene->GetObjectByID("doorglow3")->SetEnable(true);

			//Door Open Animation Playing
			isDoorOpenAnimPlaying = true;
			JAnim_DoorsOpening->PlayAnim();
			HOScene->Shake(2, 2, 2.5f);

			//disable popup
			PopStates[popID] = ePopupState_Inactive;

			isSparkleUpdatePending = true;
		}
	}
	else if(popID == ePop2_Pipe)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken)
		{
			if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken)
			{
				HOScene->GetObjectByID("pipe")->SetEnable(false);
			}

			//disable popup
			PopStates[popID] = ePopupState_Inactive;

			isSparkleUpdatePending = true;
		}
	}
	HOScene->GetObjectByID("handMazeMap")->Show(true);
	HOScene->GetObjectByID("posindicator3")->Show(true);
}

void L3_S2_P1Room4Match3::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	CPoint pos = Control::Input->Mpos();

	if(isDoorOpenAnimPlaying)
	{
		//HOScene->update(Control::LogicRate);
		Cursor::SetMode(CA_Normal);
		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		SparkleUpdate();

	if( ActivePopupID == ePopInactive)
	{
		Cursor::SetMode(CA_Normal);
		for (int i = 0; i < ePopL3S2M4_Max; i++)
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
						HOScene->GetObjectByID("handMazeMap")->Show(false);
						HOScene->GetObjectByID("posindicator3")->Show(false);
					}
					return;
				}
			}
		}
		
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened)
		{
			if( SceneRectExitLeft.Intersection(&pos) || SceneRectExitRight.Intersection(&pos) )
			{

#ifdef TOUCH_UI
			//	if(!GFHud->InvBoxCtrl->navTapped)
			//		Cursor::SetMode(CA_ExitUp,-1);
#else
				Cursor::SetMode(CA_ExitUp,-1);
#endif

				if( Control::Input->LBclicked() )
				{
#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						if(SceneRectExitLeft.Intersection(&pos))
							GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1DeadEnd, 0, SceneRectExitLeft);
							
						if(SceneRectExitRight.Intersection(&pos))
							GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1DeadEnd, 0, SceneRectExitRight);
					}
#else
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1DeadEnd;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				}	
				return;
			}
			else if( SceneRectExitCenter.Intersection(&pos) )
			{

#ifdef TOUCH_UI
				//if(!GFHud->InvBoxCtrl->navTapped)
				//	Cursor::SetMode(CA_ExitUp,-1);
#else
				Cursor::SetMode(CA_ExitUp,-1);
#endif

				if( Control::Input->LBclicked() )
				{
#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room5Pipe, 0, SceneRectExitCenter);
					}
#else
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room5Pipe;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				}	
				return;
			}
		}
		else
		{
			if( SceneRectExitLeft.Intersection(&pos) || SceneRectExitRight.Intersection(&pos) || SceneRectExitCenter.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Help);
				if( Control::Input->LBclicked() )
				{
					Hud->ShowBannerText(Banner_L3_S2_M4_ClickOnLockedDoor);
				}	
				return;
			}
		}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room3Lock;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif
		if( IsMouseOverGoBackArea(&pos) )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitDown,eArea_L3_S2_P1Room3Lock);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L3_S2_P1Room3Lock);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room3Lock, 2);
				}
#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room3Lock;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}	
		}
	}
}

void L3_S2_P1Room4Match3::ReceiveMessage(int val)
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
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			//If no drop items in this scene !!
			//GFInstance->WrongInvDropMsg(Hud);
			//Hud->ReleaseDrag();
			//Hud->CloseInventory(); 
			
			//else use below
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

void L3_S2_P1Room4Match3::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 7;
	do
	{
		switch( iNonHOHint )
		{
			default:			
					++iNonHOHint;
				break;

			case 0: ++iNonHOHint;
					if( ActivePopupID != ePopInactive )
					{
						PopupArray[ActivePopupID]->ProcessHint();
						return;
					}
					break;

			case 1: ++iNonHOHint;
					for (int i = 0; i < ePopL3S2M4_Max; i++)
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
					if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken 
						|| !ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken 
						|| !ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken)
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened)
					{
						InitHintTrailEmit(&SceneRectExitCenter,false,CA_ExitUp);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					break;
			case 5: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
					break;
			case 6: ++iNonHOHint;
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

void L3_S2_P1Room4Match3::NavigateToScene()
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