//====================================
//  created by : Amal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S4_Ameeha.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "MeshObject.h"
#include "L7S4_Pop1Pooja.h"
#include "L7S4_Pop2Flower.h"


const CRectangle  	PopAreaRect(345.0f,70.0f,665.0f,521.0f);
const CRectangle  PuzzleRect(948.0f,500.0f,91.0f,84.0f);
const CRectangle  ActionRectAmeha(539.0f,74.0f,323.0f,328.0f);


L7_S4_Ameeha::L7_S4_Ameeha()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S4\\S4.SCN");
	dialogSystem = NULL;
	//if condition to add
		dialogSystem = new DialogSystem();
		AddControl(dialogSystem);		
		PoojaPopup = NULL;
	isDialogActive = false;
	HOScene->GetObjectByID("glow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("glow")->Animate(0,1.0f,7.0f);
	waterObj1 = NULL;
	waterObj2 = NULL;
	if( GFInstance->IsReplayingGame  )//if ameeha cleared from tub!!
	{
		HOScene->GetObjectByID("waterAnim")->SetEnable(false);
		HOScene->GetObjectByID("hairCloths")->SetEnable(false);
	}
	else
	{
		waterObj1 = new MeshObject(HOScene->GetObjectByID("waterAnim"), 10, 10);
		waterObj1->meshFX->SetSpeedAndDisplacement(4, 4, 3, 3);
		HOScene->InsertObject(waterObj1, 1);

		waterObj2 = new MeshObject(HOScene->GetObjectByID("hairCloths"), 10, 10);
		waterObj2->meshFX->SetSpeedAndDisplacement(4, 4, 3, 3);
		HOScene->InsertObject(waterObj2, 1);
	}
	skyObj1 = NULL;
		skyObj1 = new MeshObject(HOScene->GetObjectByID("SkyAnim1"), 5, 5);
		skyObj1->meshFX->SetSpeedAndDisplacement(4, 4, 2, 1);
		HOScene->InsertObject(skyObj1, 1);

		skyObj2 = NULL;
		skyObj2 = new MeshObject(HOScene->GetObjectByID("SkyAnim2"), 5, 5);
		skyObj2->meshFX->SetSpeedAndDisplacement(4, 4, 2, 1);
		HOScene->InsertObject(skyObj2, 1);

		if( GFInstance->IsReplayingGame  )//ameeha cleared from tub!!
		{
			HOScene->GetObjectByID("ameeha")->SetEnable(false);
		}
		else
		{
			HOScene->GetObjectByID("ameeha")->SetPivot(0,0);
			HOScene->GetObjectByID("ameeha")->Animate(0,5,0,0,10.0f);
		}

		s4BubblesFX = new SuperFX("PRT\\L7\\s4Bubbles.INI", PointSystem::CreateCPoint(706,389,0), 100); 
		s4BubblesFX->Start();
		s4BubblesFX->AdvanceByTime(2.0f);



	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	InitPopups();

	SceneObjectsUpdate();

	TaskPendingUpdate();

	#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S4Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		const CRectangle  MorphRect(1042.0f,452.0f,135.0f,104.0f);

		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s4AmeehaAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s4AmeehaAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s4AmeehaAmb,true);
	}
	
	//MUSIC_SFX
	
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S4_Ameeha]  = true;
}

void L7_S4_Ameeha::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL7S4_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	//Popup 1
	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_HaveHandPower)
	{
		int p1rect[] = {585,485,810,482,825,606,584,611}; 
		PopPolyArray[ePopL7S4_Pop1Table] =  new CPolygon(4);
		PopPolyArray[ePopL7S4_Pop1Table]->Init(p1rect);

		PoojaPopup = new L7S4_Pop1Pooja(ePopL7S4_Pop1Table,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePopL7S4_Pop1Table]->center, CC_CALLBACK_1(L7_S4_Ameeha::OnPopupClosed, this));	

		PopupArray[ePopL7S4_Pop1Table] = PoojaPopup;
		PopupArray[ePopL7S4_Pop1Table]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePopL7S4_Pop1Table] = ePopupState_Closed;
	}

	//Popup 2
	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueFlowerTaken)
	{
		
		int p2rect[] ={304,335,463,331,501,466,334,484};
		PopPolyArray[ePopL7S4_Pop2Flower] =  new CPolygon(4);
		PopPolyArray[ePopL7S4_Pop2Flower]->Init(p2rect);

		PopupArray[ePopL7S4_Pop2Flower] = new L7S4_Pop2Flower(ePopL7S4_Pop2Flower,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePopL7S4_Pop2Flower]->center, CC_CALLBACK_1(L7_S4_Ameeha::OnPopupClosed, this));
		PopupArray[ePopL7S4_Pop2Flower]->SetPopupBoundingBox(PopAreaRect);
		
		PopStates[ePopL7S4_Pop2Flower] = ePopupState_Closed;
	}
}

void L7_S4_Ameeha::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	isSparkleUpdatePending = true;
	SceneObjectsUpdate();
	switch (popup->GetPopupId())
	{
	case 0:
		if(ProfileSystem::Instance->ActiveProfile.L7_S4_HaveHandPower)
		{
			//disable popup
			Hud->ShowBannerText("L7warm");
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
			Hud->AllowInput = true;
		}
		break;
	case 1:
		if(ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueFlowerTaken)
		{
			//disable popup
			
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	}
}

void L7_S4_Ameeha::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if( !ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted )	
	{
		HOScene->GetObjectByID("afterPuzzle")->SetEnable(false);
		HOScene->GetObjectByID("beforePuzzle")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("afterPuzzle")->SetEnable(true);
		HOScene->GetObjectByID("beforePuzzle")->SetEnable(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueStarUsed )	
	{
		HOScene->GetObjectByID("doorOpen")->SetEnable(false);
		HOScene->GetObjectByID("doorClosed")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("doorOpen")->SetEnable(true);
		HOScene->GetObjectByID("doorClosed")->SetEnable(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L7_S4_InvWaterUsedOnBlueFlower )	
	{
		HOScene->GetObjectByID("blueFlowerOpen")->SetEnable(false);
		HOScene->GetObjectByID("blueFlowerClosed")->SetEnable(true);
	}
	else
	{
		if( !ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueFlowerTaken)	
			HOScene->GetObjectByID("blueFlowerOpen")->SetEnable(true);
		else
			HOScene->GetObjectByID("blueFlowerOpen")->SetEnable(false);

		HOScene->GetObjectByID("blueFlowerClosed")->SetEnable(false);
	}

}

void L7_S4_Ameeha::Required()
{
	HiddenObjectSystem::Required();	//forced update
	if(PoojaPopup != NULL)
		if(PoojaPopup->_isVdoStartPlaying)
			return;

	if( Control::Audio->IsPlaying(aTrackL7ho) )
		{
			Control::Audio->Samples[aTrackL7ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL7ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL7ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL7theme) )
					  Control::Audio->PlaySample(aTrackL7theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL7puzzle) )
		{
			Control::Audio->Samples[aTrackL7puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL7puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL7puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL7theme) )
					  Control::Audio->PlaySample(aTrackL7theme, true);
					  
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL7theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL7theme, true);
			}
		}
}

void L7_S4_Ameeha::Update()
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

	CPoint pos = Control::Input->Mpos();

	if( ActivePopupID == ePopInactive )
	{
		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		if(  !ProfileSystem::Instance->ActiveProfile.L7_S4_AmehaTalked)	
		{
			if(ActionRectAmeha.Intersection(&pos))
			{
				Cursor::SetMode(CA_Speech);
			}
			if(Control::Input->LBclicked())
			{
				dialogSystem->ShowDialog(eDialog_L7S4Ameha);
				isDialogActive = true;
				Hud->AllowInput = false;
				Hud->HideHud(true);
			}
				
			return;
			
		}
		else 
		{
			if( !GFInstance->IsReplayingGame )			
			//ameeha won't be present after she freed!!			
				if(ActionRectAmeha.Intersection(&pos))//only when ameeha  
				{
					Cursor::SetMode(CA_Speech);
					if(Control::Input->LBclicked())
					{
						Hud->ShowBannerText("L7haste");
					}
					return;
				}
		}

		//popup click checks
		for (int i = 0; i < ePopL7S4_Max; i++)
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

		if( !ProfileSystem::Instance->ActiveProfile.L7_S1_PZStarDoorOpened[2] )	
		{
			if(PuzzleRect.Intersection(&pos))
			{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S4_PZJigsawBlue, 0, PuzzleRect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S4_PZJigsawBlue;
					SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
						
				}
				return;
			}
		}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_Haven;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	
		if( IsMouseOverGoBackArea(&pos) )
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L7_S3_Haven);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L7_S3_Haven);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S3_Haven, 2);
			}
#else			
			Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_Haven;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
									
			}	
		}
	}
}

void L7_S4_Ameeha::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL7S4_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if( !ProfileSystem::Instance->ActiveProfile.L7_S4_AmehaTalked )
			isAnyTaskLeft = true;
	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted )
			isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] = isAnyTaskLeft;
}

void L7_S4_Ameeha::SparkleUpdate()
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
		for (int i = 0; i < ePopL7S4_Max; i++)
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

		if(!ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted)
		{
			sparklePoints[numSparkles].SetXY(PuzzleRect.GetCenterX(), PuzzleRect.GetCenterY());
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

void L7_S4_Ameeha::ReceiveMessage(int val)
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
	else if( val == HOSM_DialogClosed ) 
	{
		isDialogActive = false;
		Hud->AllowInput = true;
		Hud->ShowHud(true);

		//Journal Entry Add
		if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P3_AmeehaDialogEnd])
		{
			//Flag Set
			ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P3_AmeehaDialogEnd] = true;

			//Add to Note Array
			ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P3_AmeehaDialogEnd;

			ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

			//Increment Notes
			ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			Hud->ShowJrnEntryFX();
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
				//popup click checks
				for (int i = 0; i < ePopL7S4_Max; i++)
				{
					if(PopStates[i] != ePopupState_Inactive) //if popup is active
					{
						if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&dropPos))
						{
				
							ActivePopupID = i;
							PopupArray[i]->OpenPopup();
							
							return;
						}
					}
				}
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

void L7_S4_Ameeha::ProcessSkip()
{
	if (ActivePopupID == ePopL7S4_Pop1Table)
	{
		((L7S4_Pop1Pooja*)PopupArray[ActivePopupID])->ProcessSkip();
	}
}

void L7_S4_Ameeha::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S4Collected)
		{
			InitHintTrailEmit(1110, 510);
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

		case 1: ++iNonHOHint;
			if( ActivePopupID != ePopInactive )
			{
				PopupArray[ActivePopupID]->ProcessHint();
				return;
			}
			break;
		case 2: ++iNonHOHint;
			for (int i = 0; i < ePopL7S4_Max; i++)
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
		case 0: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L7_S4_AmehaTalked )
			{
				InitHintTrailEmit(&ActionRectAmeha);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted )
			{
			InitHintTrailEmit(&PuzzleRect);
			return;
			}
			break;
		case 4: ++iNonHOHint;
			/*if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd])
			{
			InitHintTrailEmit(&ScnRectTunnelEnd,true,CA_ExitRight);
			return;
			}*/
			break;
		case 5: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S3_Haven])
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

L7_S4_Ameeha::~L7_S4_Ameeha()
{

	SAFE_DELETE(s4BubblesFX);
	for (int i = 0; i < ePopL7S4_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s4AmeehaAmb);
	Control::Audio->UnloadSample(aAmbL7s4AmeehaAmb);
	//MUSIC_SFX
}
void L7_S4_Ameeha::NavigateToScene()
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