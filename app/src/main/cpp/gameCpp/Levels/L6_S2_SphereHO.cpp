//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 02 : Sphere HO
//====================================

#include "L6_S2_SphereHO.h"
#include "Hud.h"
#include "HOScene.h"
#include "Rect.h"
#include "Application.h"
#include "SDrop.h"
#include "Util.h"
#include "exMath.h"
#include "GameFlow.h"


const CRectangle  Match3btnRect(529.0f,2.0f,305.0f,46.0f);

L6_S2_SphereHO::L6_S2_SphereHO()
{
	//Main Scene
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S2\\HO\\HO.SCN");

	//Match 3
	match3Ctrl  = NULL;
	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	//BtnMatch3 = HOScene->GetObjectByID("btnMatch3");
	//BtnSwitchToHO = HOScene->GetObjectByID("btnSwitchToHO");
	match3Back = new CObject();
	match3Back->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
	match3Back->SetUID("match3Back");
	match3Back->Show(false);
	HOScene->PushObject(match3Back);

	match3BackText = new CObject();
	match3BackText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
	match3BackText->SetUID("match3BackText");
	match3BackText->Show(false);
	HOScene->PushObject(match3BackText);
#ifndef M3_BUTTON
	match3Back->SetEnable(false);
    match3BackText->SetEnable(false);
#endif
	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		match3Back->SetEnable(false);
		match3BackText->SetEnable(false);
	}
	BtnMatch3 = new CObject();
	BtnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 682, 26, 48);
	BtnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(BtnMatch3);

	BtnSwitchToHO = new CObject();
	BtnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 682, 26, 49);
	BtnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(BtnSwitchToHO);

	BtnMatch3->SetZ(100);
	BtnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);
	BtnSwitchToHO->Show(false);

	//Scene animations

	for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
	{
		HOPolyArray[i] = NULL;
	}

	//Polygon Init
	int pts0[] = {382,82,292,136,326,180,345,152,369,149,407,175,406,160,382,129,391,87,382,78};
	HOPolyArray[0]=  new CPolygon(10);
	HOPolyArray[0]->Init(pts0);
	int pts1[] = {830,300,796,302,792,330,815,320,821,329,815,346,837,367,854,363,839,310,829,297};
	HOPolyArray[1]=  new CPolygon(10);
	HOPolyArray[1]->Init(pts1);
	int pts2[] = {726,267,716,268,719,284,708,296,714,313,721,310,750,394,756,385,788,330,723,265};
	HOPolyArray[2]=  new CPolygon(10);
	HOPolyArray[2]->Init(pts2);
	int pts3[] = {744,179,737,184,744,198,737,210,787,248,800,253,817,231,743,175};
	HOPolyArray[3]=  new CPolygon(8);
	HOPolyArray[3]->Init(pts3);
	int pts4[] = {595,181,609,193,617,218,619,231,642,218,666,232,665,204,632,166,595,162,594,176,594,179};
	HOPolyArray[4]=  new CPolygon(11);
	HOPolyArray[4]->Init(pts4);
	int pts5[] = {526,318,549,373,567,373,585,328,523,317};
	HOPolyArray[5]=  new CPolygon(5);
	HOPolyArray[5]->Init(pts5);
	int pts6[] = {859,539,831,563,886,595,907,561,861,536};
	HOPolyArray[6]=  new CPolygon(5);
	HOPolyArray[6]->Init(pts6);
	//Polygon Init End

	foundItemsCount = 0;
	isHOCompleted =  false;

	for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
	{
		HOScnObjects[i] = HOScene->GetObjectByID(std::string("ShapeOnSphere" + MKSTR(i)).c_str());
		HOInvboxObjects[i] = HOScene->GetObjectByID(std::string("silhouette" + MKSTR(i)).c_str());

		HOInvboxObjects[i]->SetZ(eZOrder_INVBOXITEMS);

		if(ProfileSystem::Instance->ActiveProfile.L6_S2_HOItemsFound[i])
		{
			foundItemsCount += 1;

			//Set collected object
			HOInvboxObjects[i]->PlayAnimation(1);

			HOScnObjects[i]->Show(false);
		}
	}


	//BzStartPt = new CPoint(470,600,0);
	//BzEndPt = new CPoint(470,600,0);
	//BzCtrlPt = new CPoint(470,600,0);

	isHOObjectFlying = isHOObjectScaling = false;
	hoScaleTimer = hoFlyingTimer =0;

	flyingHOItemId = -1;

	//End Combining Animation

	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;

	GFInstance->PostUpdate(2);
		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s2SphereHOamb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s2SphereHOamb) )		
	{
		Control::Audio->PlaySample(aAmbL6s2SphereHOamb,true);
	}
	
	//MUSIC_SFX
	
	
	//HOScene->GetObjectByID("BackGroundGlow")->Animate(0.6,1,4);
}

void L6_S2_SphereHO::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL6theme) )
	{
		Control::Audio->Samples[aTrackL6theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL6theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL6theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL6ho) )
				  Control::Audio->PlaySample(aTrackL6ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL6ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL6ho, true);
		}
	}
}

CPoint L6_S2_SphereHO::GetBezierPoint(CPoint* p1,CPoint* c1,CPoint* p2,float t)
{
	CPoint	bt;

	bt.x = (1 - t) * (1 - t) * p1->x + 2 * t * (1 - t) * c1->x + t * t * p2->x;
	bt.y = (1 - t) * (1 - t) * p1->y + 2 * t * (1 - t) * c1->y + t * t * p2->y;

	return bt;
}

float L6_S2_SphereHO::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L6_S2_SphereHO::Update()
{
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif
	NavigateToScene();
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if( Hud->PopupState != Popup_Inactive )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	if(!BtnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}
	if(ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		BtnMatch3->Show(false);
		BtnSwitchToHO->Show(false);
	}
	HOScene->GetObjectByID("BackGroundGlow")->SetRotate(HOScene->GetObjectByID("BackGroundGlow")->GetRotate() + 0.1f);

	if(!ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{

	#ifdef _CHEATS
		if(Control::Input->KpressedWithShift(K_VK_SPACE))
		{
			if(!ProfileSystem::Instance->ActiveProfile.L6_S2_HOComplete)
			{
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);

				//Set all items to end states
				for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
				{
					HOScnObjects[i]->Show(false);
					HOInvboxObjects[i]->Show(true);
					HOInvboxObjects[i]->PlayAnimation(1);
				}
				HandleHOComplete();
				return;
			}
		}
	#endif // _CHEATS


		if(isHOCompleted && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected)
		{
			if(_handleExtrasGameComplete(false))
					return;
			Hud->TakeInventoryItem(eInv_L6_S2_PuzzlePices);
			ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected = true;
			ProfileSystem::Instance->ActiveProfile.L6_S2_HOComplete = true;
		}

		//HO Click Checking
		//if(!ProfileSystem::Instance->ActiveProfile._IsMatch3Playing) //active only when no match3
		{
			for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L6_S2_HOItemsFound[i] && HOPolyArray[i]->Intersection(&pos))
				{
					if( Control::Input->LBclicked() )
					{
						//SFX
						int randId = rand()%3;
						switch (randId)
						{
						case 0:		Control::Audio->QuickLoadAndPlaySFX("s2ho_clickHOItem");		break;
						case 1:		Control::Audio->QuickLoadAndPlaySFX("s2ho_clickHOItem");		break;
						case 2:		Control::Audio->QuickLoadAndPlaySFX("s2ho_clickHOItem");		break;
						default:
							break;
						}


						ProfileSystem::Instance->ActiveProfile.L6_S2_HOItemsFound[i] = true;
						foundItemsCount += 1;

						//hide show items
						HOScnObjects[i]->SetScale(1.2f);
						HOScnObjects[i]->FadeOut();
						HOInvboxObjects[i]->PlayAnimation(1);

						flyingHOItemId = i;

						if(foundItemsCount>=MAX_L6S2_HOOBJECTS)
						{
							Control::Audio->QuickLoadAndPlaySFX("s2ho_hoComplete");
							HandleHOComplete();
						}

						return;
					}
				}
			}

		}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			Hud->InvBoxCtrl->ResetToInvBox();
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;			
		}	
		#endif

		if( IsMouseOverGoBackArea(&pos)
			#ifdef M3_BUTTON
			&& BtnMatch3->Visible
            #endif
            )
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L6_S2_FloatingIsland2);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L6_S2_FloatingIsland2);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S2_FloatingIsland2, 2);
			}
#else			
			Hud->InvBoxCtrl->ResetToInvBox();
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
				return;
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
		{
			if( Control::Input->LBclicked() )
			{
				Hud->InvBoxCtrl->ResetToInvBox();
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
		if( match3Ctrl->isPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected )
		{
			for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
				HOInvboxObjects[i]->SetEnable(false);

			match3Ctrl->clearBoard();
			BtnMatch3->Show(false); 
			BtnSwitchToHO->Show(false);
			Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
			Hud->HOState = HOInactive;
			Hud->InvBoxCtrl->ResetToInvBox();
			ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected = true;
			Hud->TakeInventoryItem(eInv_L6_S2_PuzzlePices);
			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			HandleHOComplete();
			return;
		}


		if(Match3btnRect.Intersection(&pos) && !match3Ctrl->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSwitchToHO->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				match3Ctrl->clearBoard();
				BtnMatch3->Show(true);
				BtnSwitchToHO->Show(false);
				BtnSwitchToHO->PlayAnimation(0);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
					HOInvboxObjects[i]->SetEnable(true);

				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				return;
			}
		}
		else
		{
			BtnSwitchToHO->PlayAnimation(0);
		}
		return;
	}
	else
	{
		if(Match3btnRect.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnMatch3->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				if(match3Ctrl == NULL)
				{
					match3Ctrl = new Match3(13);
					AddControl(match3Ctrl);
				}
				else
				{
					match3Ctrl->ShowBoard();
				}
				for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
					HOInvboxObjects[i]->SetEnable(false);

				BtnMatch3->Show(false);
				BtnMatch3->PlayAnimation(0);
				BtnSwitchToHO->Show(true);
				
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;

				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				return;
			}
		}
		else
		{
			BtnMatch3->PlayAnimation(0);
		}
	}

}

void L6_S2_SphereHO::HandleHOComplete()
{

	if(_handleExtrasGameComplete(false))
		return;

	AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Control::Audio->QuickLoadAndPlaySFX(aSFXL2S2HOPartsCombineAnim);

	isHOCompleted = true;
	IsAnimPartsFadingIn = true;
	ProfileSystem::Instance->ActiveProfile.L6_S2_HOComplete = true;

	for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
	{
		HOInvboxObjects[i]->FadeOut(2.0f);
	}
}

L6_S2_SphereHO::~L6_S2_SphereHO()
{

	Hud->InvBoxCtrl->ResetToInvBox();//Reset back to Inv Objects
	Hud->IsHOScene = false;

	for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
	{
		SAFE_DELETE(HOPolyArray[i]);
	}

		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL6s2SphereHOamb);
	Control::Audio->UnloadSample(aAmbL6s2SphereHOamb);
	//MUSIC_SFX

}


void L6_S2_SphereHO::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		//TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		Hud->InvBoxCtrl->ResetToInvBox();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
		SendMessage(ControlExit, ID, HOSM_NewArea);
	}	
}

void L6_S2_SphereHO::ProcessNonHOHint()
{
	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		return;

	for (int i = 0; i < MAX_L6S2_HOOBJECTS; i++)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L6_S2_HOItemsFound[i])
		{
			InitHintTrailEmit(HOScnObjects[i]->Pos.x, HOScnObjects[i]->Pos.y);
			AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			return;
		}
	}

}
void L6_S2_SphereHO::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				//Cursor::SetMode(CA_ExitUp);
				Hud->InvBoxCtrl->ResetToInvBox();
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