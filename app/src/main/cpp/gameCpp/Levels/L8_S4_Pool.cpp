
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8_S4_Pool.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"


const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);
const CRectangle  PoolPuzzleRect(572.0f,287.0f,243.0f,105.0f);


L8_S4_Pool::L8_S4_Pool()
{
	IsPoolMiniGameActive = false;
	SwanAnimThreshold = 1.0f;
	SwanAnimTimer = 0.0f;
	IsAnimPlaying = false;
	AnimTimer = 0.0f;
	AnimThreshold = 1.5f;
	SwanDirection = 1;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S4\\S4.SCN");

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL8s4PoolAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL8s4PoolAmb) )		
	{
		Control::Audio->PlaySample(aAmbL8s4PoolAmb,true);
	}

	//MUSIC_SFX

	if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsMinigameAnimPlayed )
	{
		IsAnimPlaying = true;
		AnimTimer += Control::LogicRate;
		ProfileSystem::Instance->ActiveProfile.L8_S4_IsMinigameAnimPlayed = true;
		/*MiniGameGoingDown->PlayAnim();
		Control::Audio->QuickLoadAndPlaySFX("s4_MiniGameGoingDown");*/
	}

	



	InitPopups();
	InitSceneElements();
	InitAnimations();

	int ArrayPts[] = {590,294,798,293,832,410,571,404};
	PoolMiniGamePoly =  new CPolygon(4);
	PoolMiniGamePoly->Init(ArrayPts);

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S4_Pool]  = true;


	if (!ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced )
		BronzeIdol->SetEnable(false);
	if (!ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced)
		SilverIdol->SetEnable(false);
	if (!ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced)
		GoldIdol->SetEnable(false);

	SwanPath = NULL;
	SwanPath = new Bezier2D(983, 445, 941, 539, 727, 521);
}

void L8_S4_Pool::InitAnimations()
{
	LeafPillarComingUp = NULL;
	LeafPillarComingUp = new JSONAnimator( 0, CC_CALLBACK_1(L8_S4_Pool::OnAnimationComplete, this ));
	LeafPillarComingUp->parseAndLoadJSON("ANIM\\L8\\S4SMALLPILLARRISING.JSON", HOScene );

	LeafPillarGoingDown = NULL;
	LeafPillarGoingDown = new JSONAnimator( 1, CC_CALLBACK_1(L8_S4_Pool::OnAnimationComplete, this ));
	LeafPillarGoingDown->parseAndLoadJSON("ANIM\\L8\\S4MINIGAMEBASERISING.JSON", HOScene );

	MiniGameGoingDown = NULL;
	MiniGameGoingDown = new JSONAnimator( 2, CC_CALLBACK_1(L8_S4_Pool::OnAnimationComplete, this ));
	MiniGameGoingDown->parseAndLoadJSON("ANIM\\L8\\S4MINIGAMEBASEGOINGDOWN.JSON", HOScene );
	
	/*S4SwanSwimming = NULL;
	S4SwanSwimming = new JSONAnimator( 3, CC_CALLBACK_1(L8_S4_Pool::OnAnimationComplete, this ));
	S4SwanSwimming->parseAndLoadJSON("ANIM\\L8\\S4SWANSWIMMING.JSON", HOScene );
	S4SwanSwimming->PlayAnim();*/

	S4LeafFallFX = new SuperFX("PRT\\L8\\S4LeafFall.INI", PointSystem::CreateCPoint(1153,24,0), 1100); 
	S4LeafFallFX->Start();
	S4LeafFallFX->AdvanceByTime(2.0f);

	HOScene->GetObjectByID("caustics")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("caustics")->SetAlpha(0.5f);

	HOScene->GetObjectByID("causticsScaled1")->SetScale(0.35f);
	HOScene->GetObjectByID("causticsScaled1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("causticsScaled1")->SetAlpha(0.5f);

	HOScene->GetObjectByID("causticsScaled2")->SetScale(0.35f);
	HOScene->GetObjectByID("causticsScaled2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("causticsScaled2")->SetAlpha(0.5f);
	
	HOScene->GetObjectByID("SwanAnim00")->SetEnable(false);
	//HOScene->GetObjectByID("caustics")->Animate(0.05f,0.45f,8.0f, eBlend_AlphaAdditive);

	
}

void L8_S4_Pool::OnAnimationComplete(int index)
{
	IsAnimPlaying = false;
	switch (index)
	{
	case 0:		
		break;
	case 1:		
		break;
	case 2:		
		break;
	case 3:		

		break;
	}
}

void L8_S4_Pool::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL8S4_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	int popID=0;

	int pop1Pts[] = {388,283,507,278,504,54,373,56};
	PillarBronzePoly=  new CPolygon(4);
	PillarBronzePoly->Init(pop1Pts);
	int pop2Pts[] = {850,270,1000,275,1010,44,864,40};
	PillarSilverPoly=  new CPolygon(4);
	PillarSilverPoly->Init(pop2Pts);
	int pop3Pts[] = {635,195,740,197,744,36,632,38};
	PillarGoldPoly=  new CPolygon(4);
	PillarGoldPoly->Init(pop3Pts);

	//Popup 1
	popID = ePop1_L8S4BronzeIdol;
	if(!ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced)
	{
		int ArrayPts1[] = {422,318,491,313,494,258,516,199,449,178,377,197,394,267};
		CPolygon* pop1ClickPoly=  new CPolygon(7);
		pop1ClickPoly->Init(ArrayPts1);

		Popup* Pop1Scene = new L8S4_Pop1BronzeIdol(popID, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L8_S4_Pool::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopCircleRect);

		PopPolyArray[popID] = pop1ClickPoly;
		PopupArray[popID]	= Pop1Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2
	popID = ePop2_L8S4SilverIdol;
	if(!ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced)
	{

		int ArrayPts2[] = {881,312,938,315,958,274,988,228,981,193,921,183,858,204,870,252};

		CPolygon* pop2ClickPoly=  new CPolygon(8);
		pop2ClickPoly->Init(ArrayPts2);

		Popup* Pop2Scene = new L8S4_Pop2SilverIdol(popID, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L8_S4_Pool::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopCircleRect);

		PopPolyArray[popID] = pop2ClickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 3
	popID = ePop3_L8S4GoldIdol;
	if(!ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced)
	{

		int ArrayPts3[] = {658,209,707,213,718,179,731,142,685,132,636,147,647,180};
		CPolygon* pop3ClickPoly=  new CPolygon(7);
		pop3ClickPoly->Init(ArrayPts3);

		Popup* Pop3Scene = new L8S4_Pop3GoldIdol(popID, Hud, HOScene, eZOrder_Popup, pop3ClickPoly->center, CC_CALLBACK_1(L8_S4_Pool::OnPopupClosed, this));
		Pop3Scene->SetPopupBoundingBox(PopCircleRect);

		PopPolyArray[popID] = pop3ClickPoly;
		PopupArray[popID]	= Pop3Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 4
	popID = ePop4_L8S4LeafContainer;
	if(!ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken)
	{
		int pop4Pts[] = {653,266,747,262,747,309,654,307};
		CPolygon* pop4ClickPoly=  new CPolygon(4);
		pop4ClickPoly->Init(pop4Pts);

		Popup* Pop4Scene = new L8S4_Pop4LeafContainer(popID, Hud, HOScene, eZOrder_Popup, pop4ClickPoly->center, CC_CALLBACK_1(L8_S4_Pool::OnPopupClosed, this));
		Pop4Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop4ClickPoly;
		PopupArray[popID]	= Pop4Scene;

		PopStates[popID] = ePopupState_Inactive;
	}


}

void L8_S4_Pool::CheckIfBasePillarActive()
{
	if( ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced && ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced && 
		ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced)
	{
		IsAnimPlaying = true;
		LeafPillarComingUp->PlayAnim();
		Control::Audio->QuickLoadAndPlaySFX("s4_mainPillarRising");
		PopStates[ePop4_L8S4LeafContainer] = ePopupState_Closed;
	}

}

void L8_S4_Pool::ToggleParticleEffects()
{
	if( ActivePopupID != ePopInactive )
	{
		//S4SwanSwimming->StopAnim();
		S4LeafFallFX->StopImmediate();
	}
	else
	{
		//S4SwanSwimming->PlayAnim();
		S4LeafFallFX->Start();
		S4LeafFallFX->AdvanceByTime(2.0);
	}
}


void L8_S4_Pool::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	int popID = popup->GetPopupId();
	ToggleParticleEffects();

	if(popID == ePop1_L8S4BronzeIdol)
	{
		if(ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced)			
		{
			CheckIfBasePillarActive();
			isSparkleUpdatePending = true;
			PopStates[popID] = ePopupState_Inactive; //disable popup
		}
	}
	else if(popID == ePop2_L8S4SilverIdol)
	{
		if(ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced)			
		{
			CheckIfBasePillarActive();
			isSparkleUpdatePending = true;
			PopStates[popID] = ePopupState_Inactive; //disable popup
		}
	}
	else if(popID == ePop3_L8S4GoldIdol)
	{
		if(ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced)			
		{
			CheckIfBasePillarActive();
			isSparkleUpdatePending = true;
			PopStates[popID] = ePopupState_Inactive; //disable popup
		}
	}
	else if(popID == ePop4_L8S4LeafContainer)
	{
		if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved)
		{
			IsAnimPlaying = true;
			LeafBoxOpen->SetEnable(false);
			LeafPillarGoingDown->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s4_MiniGameRising");
		}
		else if(ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken)			
		{
			LeafBoxClosed->SetEnable(false);
			LeafBoxOpen->SetEnable(false);
			isSparkleUpdatePending = true;
			PopStates[popID] = ePopupState_Inactive; //disable popup
		}
	}
	TaskPendingUpdate();
}

bool L8_S4_Pool::isPopupActive(int popId)
{
	switch (popId)
	{
	case ePop1_L8S4BronzeIdol: if ( !ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced)		return true;	break;
	case ePop2_L8S4SilverIdol: if ( !ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced)		return true;	break;
	case ePop3_L8S4GoldIdol: if ( !ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced)		return true;	break;
	case ePop4_L8S4LeafContainer: if (( ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced && ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced && 
									  ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed ) ||
									  (ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken))		return true;	break;
	default:
		break;
	}

	return false;
}

void L8_S4_Pool::Required()
{
	HiddenObjectSystem::Required();	//forced update
	if( Control::Audio->IsPlaying(aTrackL8ho) )
	{
		Control::Audio->Samples[aTrackL8ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL8ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL8ho);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL8theme) )
				Control::Audio->PlaySample(aTrackL8theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL8puzzle) )
	{
		Control::Audio->Samples[aTrackL8puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL8puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL8puzzle);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL8theme) )
				Control::Audio->PlaySample(aTrackL8theme, true);

		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL8theme))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL8theme, true);
		}
	}
}

void L8_S4_Pool::Update()
{
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif

	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if( ActivePopupID != ePopInactive || Hud->IsDraggingItem() )
		return;

	HiddenObjectSystem::HOUpdate();
	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if( SwanDirection == 1 )
	{
		if(SwanAnimTimer >= SwanAnimThreshold )
		{
			SwanAnim01->SetEnable(false);
			SwanAnim01flip->SetPos( &SwanAnim01->Pos );
			SwanAnim01flip->SetEnable(true);
			SwanDirection = -1;
		}
		else
		{
			//SwanAnim01->SetPos( &SwanPath->GetPointAt( SwanAnimTimer ) );
			// Linux: gives error of taking address of temporary also potential crash factor
			// below is fix
			CPoint SwanpathPos =SwanPath->GetPointAt( SwanAnimTimer );
			SwanAnim01->SetPos( &SwanpathPos);
			SwanAnimTimer += Control::LogicRate * 0.1f;
		}
	}
	else 
	{
		if(SwanAnimTimer < 0 )
		{
			SwanAnim01flip->SetEnable(false);
			SwanAnim01->SetPos( &SwanAnim01flip->Pos );
			SwanAnim01->SetEnable(true);
			SwanDirection = 1;
		}
		else 
		{
			//SwanAnim01flip->SetPos( &SwanPath->GetPointAt( SwanAnimTimer ) );
			// Linux: gives error of taking address of temporary also potential crash factor
			// below is fix
			CPoint SwanpathPos =SwanPath->GetPointAt( SwanAnimTimer );
			SwanAnim01flip->SetPos( &SwanpathPos );
			SwanAnimTimer -= Control::LogicRate * 0.1f;
		}
	}

	
		//SwanAnimTimer = 0.0f;
		

		/*S4SwanSwimming->PlayAnim();
		SwanAnimTimer = 0.0f;
		SwanAnimThreshold = rand()%7 + 6;*/

	if(IsAnimPlaying )
	{
		if(AnimTimer > 0.0f )
		{
			if( AnimTimer >= AnimThreshold )
			{
				AnimTimer = 0.0f;
				MiniGameGoingDown->PlayAnim();
				Control::Audio->QuickLoadAndPlaySFX("s4_MiniGameGoingDown");
				return;
			}
			AnimTimer += Control::LogicRate;
		}
		return;
	}

	if( ActivePopupID == ePopInactive )
	{
		//if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		//	SparkleUpdate();

		//popup click checks
		for (int i = 0; i < ePopL8S4_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos) && isPopupActive(i) )
				{
					Cursor::SetMode(CA_Look);
					if( Control::Input->LBclicked() )
					{
						Cursor::SetMode(CA_Normal);
						ActivePopupID = i;
						PopupArray[i]->OpenPopup();
						ToggleParticleEffects();
					}
					return;
				}
			}
		}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
				Cursor::SetMode(CA_Normal);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
				SendMessage(ControlExit, ID, HOSM_NewArea);										
	}	
	#endif	
		if( IsMouseOverGoBackArea(&pos) )
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L8_S2_Garden);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L8_S2_Garden);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S2_Garden, 2);
			}
#else			
			Cursor::SetMode(CA_Normal);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
									
			}	
		}
	}

	if( PoolPuzzleRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed  && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved)
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S4_PZPoolGame, 0, PoolPuzzleRect);
			}
#else			
			Cursor::SetMode(CA_Normal);
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S4_PZPoolGame;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
			
			return;
		}		
	}

	if( Control::Input->LBclicked() )
	{
		Hud->ShowBannerText("L8shangri");
	}

}

void L8_S4_Pool::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
	}
	else if(val == UPDATE_SCENE_OBJECTS)
	{
		SceneObjectsUpdate();
		if(ActivePopupID != ePopInactive)
		{
			if(PopupArray[ActivePopupID]!=NULL)
				PopupArray[ActivePopupID]->UpdateBgGraphic();
		}
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;

		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(ActivePopupID != ePopInactive)
			{
				if(PopupArray[ActivePopupID]!=NULL)
					PopupArray[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal );
			}
			else if(itemVal == eInv_L8_S1_IdolGold && PillarGoldPoly->Intersection(&dropPos))
			{
				GoldIdol->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced  = true;
				Hud->ReleaseDrag();
				Hud->CloseInventory();
				Control::Audio->QuickLoadAndPlaySFX("s4p1_placeIdol");
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S1_IdolGold);
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_IdolGold] = false;
				CheckIfBasePillarActive();
			}
			else if( itemVal == eInv_L8_S2_IdolSilver && PillarSilverPoly->Intersection(&dropPos) )
			{
				SilverIdol->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced = true;
				Hud->ReleaseDrag();
				Hud->CloseInventory();
				Control::Audio->QuickLoadAndPlaySFX("s4p1_placeIdol");
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S2_IdolSilver);
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_IdolSilver] = false;
				CheckIfBasePillarActive();
			}
			else if( itemVal  == eInv_L8_S3_IdolBronze && PillarBronzePoly->Intersection(&dropPos) )
			{
				BronzeIdol->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced = true;
				Hud->ReleaseDrag();
				Hud->CloseInventory();
				Control::Audio->QuickLoadAndPlaySFX("s4p1_placeIdol");
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S3_IdolBronze);
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_IdolBronze] = false;
				CheckIfBasePillarActive();
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		

	}
}

void L8_S4_Pool::SceneObjectsUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced )
		BronzeIdol->SetEnable(true);
	if (ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced)
		SilverIdol->SetEnable(true);
	if (ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced)
		GoldIdol->SetEnable(true);
}


void L8_S4_Pool::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL8S4_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S4_Pool] = isAnyTaskLeft;

	if( ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] = true;
}

void L8_S4_Pool::ProcessSkip()
{
}

void L8_S4_Pool::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 6;
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
			for (int i = 0; i < ePopL8S4_Max; i++)
			{
				if( PopupArray[i] != NULL && PopStates[i] != ePopupState_Inactive) //if popup is active
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
			if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed  && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved )
			{
				Hud->InitHintTrailEmit(&PoolPuzzleRect);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] )
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 4: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 5: ++iNonHOHint;
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

void L8_S4_Pool::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopupID != ePopInactive)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L8_S4_Pool::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < ePopL8S4_Max; i++)
	{
		Popup * popup = PopupArray[i];
		if (popup)
		{
			if(popup->GetPopupState() == ePopupState_Inactive || popup->GetPopupState() == ePopupState_Closed) 
			{
				if(popup->isTaskLeft())
				{
					sparklePointsVec.push_back(popup->GetSparkleUpdatePoint());
					isSparkleUpdatePending = true;
				}
			}
		}

	}

	if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed  && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved )
	{
		sparklePointsVec.push_back(CPoint(PoolPuzzleRect.GetCenterX(), PoolPuzzleRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}

}


void L8_S4_Pool::InitSceneElements()
{
	BasePillar				= HOScene->GetObjectByID("BasePillar");
	BronzeIdol				= HOScene->GetObjectByID("BronzeIdol");
	GlowLeft				= HOScene->GetObjectByID("GlowLeft");
	GlowRight				= HOScene->GetObjectByID("GlowRight");
	GoldIdol				= HOScene->GetObjectByID("GoldIdol");
	LeafBoxClosed			= HOScene->GetObjectByID("LeafBoxClosed");
	LeafBoxOpen				= HOScene->GetObjectByID("LeafBoxOpen");
	LeavesAnim00			= HOScene->GetObjectByID("LeavesAnim00");
	LeavesAnim01 			= HOScene->GetObjectByID("LeavesAnim01");
	LeavesAnim02			= HOScene->GetObjectByID("LeavesAnim02");
	LightRays				= HOScene->GetObjectByID("LightRays");
	LotusAfter				= HOScene->GetObjectByID("LotusAfter");
	LotusBefore				= HOScene->GetObjectByID("LotusBefore");
	MinigameBase			= HOScene->GetObjectByID("MinigameBase");
	PillarBack				= HOScene->GetObjectByID("PillarBack");
	PillarLeft				= HOScene->GetObjectByID("PillarLeft");
	PillarRight				= HOScene->GetObjectByID("PillarRight");
	PinkFlowerAnim00		= HOScene->GetObjectByID("PinkFlowerAnim00");
	PinkFlowerAnim01		= HOScene->GetObjectByID("PinkFlowerAnim01");
	PinkFlowerAnim02		= HOScene->GetObjectByID("PinkFlowerAnim02");
	PinkFlowerAnim03		= HOScene->GetObjectByID("PinkFlowerAnim03");
	Pool					= HOScene->GetObjectByID("Pool");
	PuzzleCompleteState		= HOScene->GetObjectByID("PuzzleCompleteState");
	RabitAnim00				= HOScene->GetObjectByID("RabitAnim00");
	RabitAnim01				= HOScene->GetObjectByID("RabitAnim01");
	SilverIdol				= HOScene->GetObjectByID("SilverIdol");
	SwanAnim00				= HOScene->GetObjectByID("SwanAnim00");
	SwanAnim01				= HOScene->GetObjectByID("SwanAnim01");
	YellowAnim00			= HOScene->GetObjectByID("YellowAnim00");
	YellowAnim01			= HOScene->GetObjectByID("YellowAnim01");
	YellowAnim02			= HOScene->GetObjectByID("YellowAnim02");
	SwanAnim01flip			= HOScene->GetObjectByID("SwanAnim01flip");

	SwanAnim01flip->SetEnable(false);

	PuzzleCompleteState ->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced || !ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced || 
		!ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced)
	{
		BasePillar		->SetEnable(false);
		LeafBoxClosed	->SetEnable(false);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced && ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced && 
		ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced)
		PopStates[ePop4_L8S4LeafContainer] = ePopupState_Closed;

	if( !ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer )
	{
		LeafBoxOpen		->SetEnable(false);
		MinigameBase	->SetEnable(false);
		LotusAfter		->SetEnable(false);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed  )
	{
		LeafBoxOpen		->SetEnable(false);
		MinigameBase	->SetEnable(false);
		LotusAfter		->SetEnable(false);
	}
	else if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsContainerClosed && !ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved)
	{
		LeafBoxClosed	->SetEnable(false);
		LeafBoxOpen		->SetEnable(false);
		BasePillar		->SetEnable(false);
		LotusBefore		->SetEnable(false);
	}
	else if( ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken)
	{
		if( IsAnimPlaying && AnimTimer > 0 )
		{
			PuzzleCompleteState ->SetEnable(true);
			LeafBoxClosed	->SetEnable(false);
			LeafBoxOpen		->SetEnable(false);
			BasePillar		->SetEnable(false);
			LotusBefore		->SetEnable(false);
		}
		else
		{
			LeafBoxOpen		->SetEnable(false);
			MinigameBase	->SetEnable(false);
			LotusAfter		->SetEnable(false);
		}
	}
	else if( ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
	{
		LeafBoxClosed	->SetEnable(false);
		LeafBoxOpen		->SetEnable(false);
		MinigameBase	->SetEnable(false);
		LotusAfter		->SetEnable(false);
	}
}


L8_S4_Pool::~L8_S4_Pool()
{
	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL8s4PoolAmb);
	Control::Audio->UnloadSample(aAmbL8s4PoolAmb);
	//MUSIC_SFX

	for (int i = 0; i < ePopL8S4_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
	}
	//SAFE_DELETE(S4SwanSwimming);
	SAFE_DELETE(S4LeafFallFX);
	SAFE_DELETE(LeafPillarGoingDown);
	SAFE_DELETE(MiniGameGoingDown);
	SAFE_DELETE(LeafPillarComingUp);

}
void L8_S4_Pool::NavigateToScene()
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