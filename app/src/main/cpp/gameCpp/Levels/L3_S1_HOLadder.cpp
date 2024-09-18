//====================================
// DQFULP
// Hidden Object System
// Author : Faisal
// LEVEL3 - SCENE1 : HOLadder
//====================================

#include "L3_S1_HOLadder.h"
#include "Hud.h"
#include "Util.h"
#include "Application.h"
#include "exMath.h"

const CRectangle  Match3btnRect(531.0f,1.0f,278.0f,48.0f);

int hudSymbolOrder[MAX_L3S1_HOOBJECTS] =  {0,  1, 2,  3,  4,  5,  6,  7,  8,  9, 10,  11};
int StoneOverlapID[MAX_L3S1_HOOBJECTS] = { 3, 8, 5, -1, -1, -1,  7, -1, -1, 11, -1,  -1};

L3_S1_HOLadder::L3_S1_HOLadder()
{
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentLevel);
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S1\\HO\\HO.SCN");
	musicFadeOutTimer = 0;
	//Match 3
	match3Ctrl  = NULL;
	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

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
	BtnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 670, 25, 45);
	BtnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(BtnMatch3);

	BtnSwitchToHO = new CObject();
	BtnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 670, 25, 46);
	BtnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(BtnSwitchToHO);

	BtnMatch3->SetZ(100);
	BtnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);
	BtnSwitchToHO->Show(false);
	
	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;

	isHOComplete = ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompleted;
	hoEndDelayTimer = -1;

	if(isHOComplete)
		hoEndDelayTimer = 1.5f;

	InitHO();

	if(IsVisitedForFirstTime)
	{
		Hud->ShowBannerText("L3rubble");
	}

	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s1LadderHO,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s1LadderHO) )		
	{
		Control::Audio->PlaySample(aAmbL3s1LadderHO,true);
	}
	
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L3_S1_HOLadder]  = true;
}

void L3_S1_HOLadder::InitHO()
{
	nextSymbolID = -1;	

	for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
	{
		HObjects[i] = NULL;
		HOPolyArray[i] = NULL;
	}

#pragma region PolygonInit
	int Pts0[] = {720,306,686,315,685,332,721,351,768,347,775,330};
	HOPolyArray[0]=  new CPolygon(6);
	HOPolyArray[0]->Init(Pts0);
	int Pts1[] = {506,266,515,275,501,301,451,345,424,332,447,293};
	HOPolyArray[1]=  new CPolygon(6);
	HOPolyArray[1]->Init(Pts1);
	int Pts2[] = {400,461,401,469,371,496,334,507,333,471,374,459};
	HOPolyArray[2]=  new CPolygon(6);
	HOPolyArray[2]->Init(Pts2);
	int Pts3[] = {734,321,744,309,784,317,785,344,779,366,741,361};
	HOPolyArray[3]=  new CPolygon(6);
	HOPolyArray[3]->Init(Pts3);
	int Pts4[] = {250,292,239,298,249,352,269,358,283,392,325,368,301,344,303,319};
	HOPolyArray[4]=  new CPolygon(8);
	HOPolyArray[4]->Init(Pts4);
	int Pts5[] = {309,465,291,471,283,513,318,518,348,542,363,526,375,490,337,481};
	HOPolyArray[5]=  new CPolygon(8);
	HOPolyArray[5]->Init(Pts5);
	int Pts6[] = {540,396,570,428,536,472,495,434};
	HOPolyArray[6]=  new CPolygon(4);
	HOPolyArray[6]->Init(Pts6);
	int Pts7[] = {437,417,491,450,486,493,435,463};
	HOPolyArray[7]=  new CPolygon(4);
	HOPolyArray[7]->Init(Pts7);
	int Pts8[] = {434,250,431,284,452,306,462,301,478,318,493,303,481,287,490,268,468,248};
	HOPolyArray[8]=  new CPolygon(9);
	HOPolyArray[8]->Init(Pts8);
	int Pts9[] = {656,173,671,173,691,202,688,237,674,266,648,268,637,233,639,191};
	HOPolyArray[9]=  new CPolygon(8);
	HOPolyArray[9]->Init(Pts9);
	int Pts10[] = {868,93,872,105,906,118,943,122,955,113,946,98,901,83};
	HOPolyArray[10]=  new CPolygon(7);
	HOPolyArray[10]->Init(Pts10);
	int Pts11[] = {452,140,509,153,508,223,451,212};
	HOPolyArray[11]=  new CPolygon(4);
	HOPolyArray[11]->Init(Pts11);  
#pragma endregion

	for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
	{
		HObjects[i] = new L3S1HiddenObject();
		HObjects[i]->setPolygonArea(HOPolyArray[i]);
		HObjects[i]->setHudSymbol(HOScene->GetObjectByID(std::string("symbol" + MKSTR(i)).c_str()));
		HObjects[i]->setSymbolGlow(HOScene->GetObjectByID(std::string("symglow" + MKSTR(i)).c_str()));
		HObjects[i]->getSymbolGlow()->SetBlendMode(eBlend_AlphaAdditive);
		HObjects[i]->setSceneObject(HOScene->GetObjectByID(std::string("rock" + MKSTR(i)).c_str()));

		HObjects[i]->setObjectID(i);

		HObjects[i]->InitObject(CC_CALLBACK_1(L3_S1_HOLadder::OnObjectReachedHUD, this));		

		if(ProfileSystem::Instance->ActiveProfile.L3_S1_HOObjFound[i])
		{
			HObjects[i]->AlreadyFound();
		}		
	}

	for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
	{
		int symbolID = hudSymbolOrder[i];
		HudSymbolPoints[i] = NULL;
		HudSymbolPoints[i] = new CPoint(HObjects[symbolID]->getHudSymbol()->Pos);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudSymbolsInitialized)
	{
		//initialize for first time
		ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudSymbolsInitialized = true;

		ProfileSystem::Instance->ActiveProfile.L3_S1_HONextHudSymbolID = MAX_HUDL3S1_SYMBOLS;

		for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
		{
			int symbolID = hudSymbolOrder[i];
			
			HObjects[symbolID]->getHudSymbol()->Show(true);			

			ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i] = symbolID;
			//SLOG::LogInt("symbol ID:",symbolID);
		}
	}
	else
	{
		for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
		{
			int symbolID = ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i];
			
			if( symbolID != -1 )
			{
				HObjects[symbolID]->getHudSymbol()->SetPos(HudSymbolPoints[i]);
				HObjects[symbolID]->getHudSymbol()->Show(true);
			}
		}
	}
}

L3_S1_HOLadder::~L3_S1_HOLadder()
{
	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
	{
		SAFE_DELETE(HudSymbolPoints[i]);
	}

	for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
	{
		SAFE_DELETE(HObjects[i]);
		SAFE_DELETE(HOPolyArray[i]);		
	}

	Hud->InvBoxCtrl->ResetToInvBox();

		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s1LadderHO);
	Control::Audio->UnloadSample(aAmbL3s1LadderHO);
	//MUSIC_SFX
}

void L3_S1_HOLadder::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL3theme) )
	{
		Control::Audio->Samples[aTrackL3theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL3theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL3theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL3ho) )
				  Control::Audio->PlaySample(aTrackL3ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL3ho))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL3ho, true);
		}
	}
}

void L3_S1_HOLadder::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();	
	Cursor::SetMode(CA_Normal);
	if(!BtnMatch3->Visible && !isHOComplete)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(isHOComplete)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		BtnMatch3->Show(false);
		BtnSwitchToHO->Show(false);
	}

	if(isHOComplete)
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;

		if(hoEndDelayTimer>0)
		{
			hoEndDelayTimer -= Control::LogicRate;
			if(hoEndDelayTimer<0)
			{
				hoEndDelayTimer = -1;

				Hud->HOState = HOInactive;
				Hud->InvBoxCtrl->ResetToInvBox();
				
				ProfileSystem::Instance->ActiveProfile.L3_S1_InvBrokenLadderTaken = true;
				Hud->TakeInventoryItem(eInv_L3S1_LadderBroken);
			}
		}
		return;
	}

#ifdef _CHEATS
	if(Control::Input->KpressedWithShift(K_VK_SPACE))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompleted)
		{
			hoEndDelayTimer = 1.0f;

			HandleHOComplete();
			return;
		}
	}
#endif // _CHEATS

	if(!ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompleted && ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompletedWithMatch3)
	{	
		hoEndDelayTimer = 1.0f;

		HandleHOComplete();
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
		{
			if( Control::Input->LBclicked() )
			{
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
		if(Match3btnRect.Intersection(&pos) && !match3Ctrl->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSwitchToHO->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				//Show Inv Box Items
				for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
				{
					int objID = ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i];
					if(objID!=-1)
					{
						HObjects[objID]->SwitchToHO();
					}
				}

				match3Ctrl->clearBoard();
				BtnMatch3->Show(true);
				BtnSwitchToHO->Show(false);
				BtnSwitchToHO->PlayAnimation(0);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

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
					match3Ctrl = new Match3(6);
					AddControl(match3Ctrl);
				}
				else
				{
					match3Ctrl->ShowBoard();
				}

				//Hide Inv Box Items				
				for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
				{
					int objID = ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i];
					if(objID!=-1)
					{
						HObjects[objID]->SwitchToMatch3();
					}
				}

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
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);
	}
    #endif

	if( IsMouseOverGoBackArea(&pos)
		#ifdef M3_BUTTON
		&& BtnMatch3->Visible
        #endif
        )
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L3_S1_FortEntrance);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L3_S1_FortEntrance);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L3_S1_FortEntrance, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
	else if( Control::Input->LBclicked() )
	{
		for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
		{
			//if(!ProfileSystem::Instance->ActiveProfile.L3_S1_HOObjFound[i])
			{
				bool isSymbolHidden = true;
				if(StoneOverlapID[i] > -1)
				{
					//check if overlapped symbol is taken
					if(ProfileSystem::Instance->ActiveProfile.L3_S1_HOObjFound[StoneOverlapID[i]])
						isSymbolHidden = false;
				}
				else
					isSymbolHidden = false;
			
				if(!isSymbolHidden && CheckIfSymbolShownInHUD(i))
				{
					if(HObjects[i]->MouseClicked(pos))
					{
						//Take the HO item
						TakenHOItem(i);
						break;
						//Cursor::SetMode(CA_HandPointy); for testing only
						//if( Control::Input->LBclicked() )
						{
							
						}
						return;
					}
				}
			}
		}
	}
}

bool L3_S1_HOLadder::CheckHOComplete()
{
	bool isAllHOFound = true;
	for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S1_HOObjFound[i])
		{
			isAllHOFound = false;
			break;
		}
	}

	return isAllHOFound;
}

void L3_S1_HOLadder::HandleHOComplete()
{
	isHOComplete = true;
	ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompleted = true;

	HOScene->GetObjectByID("ladderbgright")->SetEnable(false);
	HOScene->GetObjectByID("ladderbgcenter")->SetEnable(false);
	HOScene->GetObjectByID("ladderbgleft")->SetEnable(false);

	for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
	{
		HObjects[i]->AlreadyFound();
	}
	AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	if(_handleExtrasGameComplete(false))
		return;
}

bool L3_S1_HOLadder::CheckIfSymbolShownInHUD(int objID)
{
	bool isVisibleInHUD = false;
	for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
	{
		if(objID == ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i])
		{
			isVisibleInHUD = true;
			break;
		}

	}
	return isVisibleInHUD;
}

void L3_S1_HOLadder::OnObjectReachedHUD(int objectID)
{
	if( ProfileSystem::Instance->ActiveProfile.L3_S1_HONextHudSymbolID < 12 )
	{
		//Add another symbol from array to hud
		int posToAdd = 0;
		for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
		{
			if(objectID == ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i])
			{
				posToAdd = i;
				break;
			}
		}

		nextSymbolID = hudSymbolOrder[ProfileSystem::Instance->ActiveProfile.L3_S1_HONextHudSymbolID];
		ProfileSystem::Instance->ActiveProfile.L3_S1_HONextHudSymbolID += 1; //increment it

		//SLOG::LogInt("nextSymbol ID:", nextSymbolID);
		//SLOG::LogInt("Add Next symbol to Pos:", posToAdd);

		ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[posToAdd] = nextSymbolID;

		HObjects[nextSymbolID]->getHudSymbol()->SetPos(HudSymbolPoints[posToAdd]);
	}
	else
	{
		nextSymbolID = -1;
		for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
		{
			if(objectID == ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i])
			{
				ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i] = -1; //disable
				break;
			}
		}
	}

	float valArr[5] = {0, -1, -1, 0, -1};
	HObjects[objectID]->getHudSymbol()->TweenTo(valArr, 0.75f, 0, true);

	//Show next symbol
	if(nextSymbolID != -1)
	{
		HObjects[nextSymbolID]->getHudSymbol()->Show(true);
		HObjects[nextSymbolID]->getHudSymbol()->SetScale(0.01f);

		float valArr[5] = {-1, -1, -1, 1.0f, -1};
		HObjects[nextSymbolID]->getHudSymbol()->TweenTo(valArr, 0.75f, 0, true);		

		nextSymbolID = -1;
	}

	//isHOFxFlying = false;
	if(CheckHOComplete())
	{
		hoEndDelayTimer = 1.5f;
		HandleHOComplete();
	}
}

void L3_S1_HOLadder::TakenHOItem(int objID)
{
	//SLOG::LogInt("Take Obj:", objID);
	ProfileSystem::Instance->ActiveProfile.L3_S1_HOObjFound[objID] = true;

	return;
    // Sac: what are these code below going to do when you have used a return already without any condition ???
	if( ProfileSystem::Instance->ActiveProfile.L3_S1_HONextHudSymbolID < 12 )
	{
		//Add another symbol from array to hud
		int posToAdd = 0;
		for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
		{
			if(objID == ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i])
			{
				posToAdd = i;
				break;
			}
		}

		nextSymbolID = hudSymbolOrder[ProfileSystem::Instance->ActiveProfile.L3_S1_HONextHudSymbolID];
		ProfileSystem::Instance->ActiveProfile.L3_S1_HONextHudSymbolID += 1; //increment it

		//SLOG::LogInt("nextSymbol ID:", nextSymbolID);
		//SLOG::LogInt("Add Next symbol to Pos:", posToAdd);

		ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[posToAdd] = nextSymbolID;

		HObjects[nextSymbolID]->getHudSymbol()->SetPos(HudSymbolPoints[posToAdd]);
	}
	else
	{
		for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
		{
			if(objID == ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i])
			{
				ProfileSystem::Instance->ActiveProfile.L3_S1_HOHudObjID[i] = -1; //disable
				break;
			}
		}
	}
}

void L3_S1_HOLadder::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		if(ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompleted)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
}

void L3_S1_HOLadder::ProcessNonHOHint()
{
	if(!ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompleted && !ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L3_S1_HOObjFound[i])
			{
				bool isSymbolHidden = true;
				if(StoneOverlapID[i] > -1)
				{
					//check if overlapped symbol is taken
					if(ProfileSystem::Instance->ActiveProfile.L3_S1_HOObjFound[StoneOverlapID[i]])
						isSymbolHidden = false;
				}
				else
					isSymbolHidden = false;
			
				if(!isSymbolHidden && CheckIfSymbolShownInHUD(i))
				{
					Hud->InitHintTrailEmit(HOPolyArray[i]->center.x, HOPolyArray[i]->center.y);
					AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					break;
				}
			}
		}
		return;
	}

	Hud->ShowNothingToDoMsgBanner();
}

void L3_S1_HOLadder::NavigateToScene()
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

#pragma region L3S1HiddenObjectClass

L3S1HiddenObject::L3S1HiddenObject()
{
	_isActive = true;
	_isFound = false;
	_isAnimComplete = false;

	_startPoint = NULL;
	_endPoint = NULL;
	_ctrlPoint = NULL;	

	_symbolGlow = NULL;
	_hudSymbol = NULL;
	_scnObj = NULL;

	_flownToHUDCallback = nullptr;

	_polygonArea = NULL;
	
	hoFlyingTimer = 0;

	symbolShowFX = NULL;
	HOTrailFX = NULL;
	hoClickFX = NULL;
}

L3S1HiddenObject::~L3S1HiddenObject()
{
	_startPoint = NULL;
	_endPoint = NULL;
	SAFE_DELETE(_ctrlPoint);
	
	_scnObj = NULL;	
	_polygonArea = NULL;	
	_isFound = false;
	SAFE_DELETE(HOTrailFX);
	SAFE_DELETE(hoClickFX);
	SAFE_DELETE(symbolShowFX);
}

void L3S1HiddenObject::InitObject(std::function<void (int)> flownToHUDCallback)
{
	_flownToHUDCallback = flownToHUDCallback;

	_startPoint = &_polygonArea->center;
	_endPoint = &_hudSymbol->Pos;
	_ctrlPoint = new CPoint((_startPoint->y + _endPoint->y)/2.0f, (_startPoint->y + _endPoint->y)/2.0f, 0);

	_symbolGlow->Show(false);
	_hudSymbol->SetZ(eZOrder_INVBOXITEMS);
	_hudSymbol->Show(false);

	if(rand()%10>5)
		_ctrlPoint->x += (200+rand()%200);
	else
		_ctrlPoint->x -= (200+rand()%200);

	symbolShowFX = new SuperFX("PRT\\L3\\s1hosymbolfx.INI", PointSystem::CreateCPoint(1065,122,0), 1845);
	HOTrailFX = new SuperFX("PRT\\L3\\s1hotrailfx.INI", PointSystem::CreateCPoint(1065,122,0), 1845);

	hoClickFX = new SuperFX("PRT\\L3\\s1hoclickfx.INI", PointSystem::CreateCPoint(564,368,0), 100); 
	hoClickFX->SetPos(_startPoint);
}

bool L3S1HiddenObject::MouseClicked(CPoint pos)
{
	if (_isFound || !_isActive)
	{
		return false;
	}

	if (_polygonArea->Intersection(&pos))
	{
		OnFound();
		return true;
	}

	return false;
}

void L3S1HiddenObject::OnFound()
{
	Control::Audio->QuickLoadAndPlaySFX("s1ho_objectsclicked");

	_scnObj->FadeOut(1.5f);
	_symbolGlow->Show(true);
	_symbolGlow->FadeOut(0.65f);
	
	_isFound = true;

	_isObjFlying = true;
	HOTrailFX->SetPos(_startPoint);
}

void L3S1HiddenObject::Update(float ds)
{
	if (_isAnimComplete || !_isFound)
		return;

	if(_isObjFlying)
	{
		if(!HOTrailFX->IsEmitting)
			HOTrailFX->Start();

		FXTimer += 1;
		if(FXTimer%3==0)
			HOTrailFX->Emit();

		//flying to HUD
		hoFlyingTimer += Control::LogicRate;

		CPoint temp = Elixir::GetBezierPoint(_startPoint, _ctrlPoint, _endPoint, hoFlyingTimer);
		HOTrailFX->SetPos(&temp);

		if(hoFlyingTimer>1.0f)
		{
			HOTrailFX->StopEmit();
			symbolShowFX->SetPos(&_hudSymbol->Pos);
			symbolShowFX->Start();
			_isAnimComplete = true;

			if(_flownToHUDCallback!=nullptr)
				_flownToHUDCallback(_objID);
		}
	}
}

void L3S1HiddenObject::AlreadyFound()
{
	_isFound = true;
	_isAnimComplete = true;
	_scnObj->SetEnable(false);	
	_hudSymbol->Show(false);

	if(HOTrailFX->IsActive)
		HOTrailFX->StopEmit();
}

void L3S1HiddenObject::SwitchToMatch3()
{
	if(_isActive)
		_hudSymbol->Show(false);
}

void L3S1HiddenObject::SwitchToHO()
{
	if(_isActive)
		_hudSymbol->Show(true);
}

#pragma endregion

