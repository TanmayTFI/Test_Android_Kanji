//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S5_PZSunClock.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"
#include "MeshObject.h"

const CRectangle  OrbMaskRect(449.0f,20.0f,516.0f,354.0f);
const CRectangle  InvDestRectSolution(310.0f,63.0f,778.0f,487.0f);
const CRectangle  solutionPaperRect(279.0f,443.0f,318.0f,218.0f);

const CRectangle  HelpRectCenter(567.0f,208.0f,244.0f,121.0f);
const int solvedState[MAX_L5S5_PZBLOCKS] = {2, 3, 0, 1, 2, 2, 2, 3};

L5_S5_PZSunClock::L5_S5_PZSunClock()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S5\\P1\\P1.SCN");

	HOScene->GetObjectByID("baseglow1")->SetScale(0.90f);
	HOScene->GetObjectByID("baseglow2")->SetScale(0.80f);
	HOScene->GetObjectByID("baseglow3")->SetScale(0.63f);
	HOScene->GetObjectByID("baseglow4")->SetScale(0.60f);
	HOScene->GetObjectByID("baseglow5")->SetScale(0.63f);
	HOScene->GetObjectByID("baseglow6")->SetScale(0.70f);
	HOScene->GetObjectByID("baseglow7")->SetScale(0.93f);

	HOScene->GetObjectByID("ray1")->SetScale(1.50f);
	HOScene->GetObjectByID("ray1")->Animate(0.15f,0.65f,4.5f,0,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("ray2")->SetScale(2.00f);
	HOScene->GetObjectByID("ray2")->Animate(0.15f,0.75f,5.5f,2.5f,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("ray3")->SetScale(1.60f);
	HOScene->GetObjectByID("ray3")->Animate(0.15f,0.65f,4.0f,0,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("ray4")->SetScale(1.70f);
	HOScene->GetObjectByID("ray4")->Animate(0.15f,0.75f,5.0f,0,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("ray5")->Animate(0.25f,0.75f,4.0f,2.5f,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("ray5")->Animate(0.25f,0.75f,4.0f,2.5f,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("orbrays1")->Animate(0.25f,0.75f,4.5f,1.5f,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("orbrays2")->Animate(0.15f,0.95f,4.0f,2.5f,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("orbrays3")->Animate(0.25f,0.85f,4.4f,2.5f,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("orbrays1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("orbrays2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("orbrays3")->SetBlendMode(eBlend_AlphaAdditive);

	HOScene->GetObjectByID("orbrays1")->SetEnable(false);
	HOScene->GetObjectByID("orbrays2")->SetEnable(false);
	HOScene->GetObjectByID("orbrays3")->SetEnable(false);

	HOScene->GetObjectByID("orbsideglow")->Animate(0.85f,1.0f,2.5f,1.25f);

	HOScene->GetObjectByID("orb")->Animate(0.85f,1.0f,2.5f,1.5f);
	HOScene->GetObjectByID("orb")->Pos.y += 140.0f;
	HOScene->GetObjectByID("orb")->SetMaskRect(OrbMaskRect);

	MeshObject* waterObj = new MeshObject(HOScene->GetObjectByID("meshwater2"), 20, 20);
	waterObj->meshFX->SetSpeedAndDisplacement(3, 4, 5, 2);
	HOScene->InsertObject(waterObj, 1);

	isPuzzleActive = false;
	isPuzzleSolved = false;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_PZSolutionPaperPlaced)
		HOScene->GetObjectByID("solvepaper")->SetEnable(false);
	else
	{
		Hud->InvBox_StartPuzzleMode();
		isPuzzleActive = true;
	}

	s5orbrisingFX = new SuperFX("PRT\\L5\\s5orbrising.INI", PointSystem::CreateCPoint(694,294,0), 12); 

	s5pzfliesFX = new SuperFX("PRT\\L3\\s5pzflies.INI", PointSystem::CreateCPoint(776,66,0), 1100); 
	s5pzfliesFX->Start();
	s5pzfliesFX->AdvanceByTime(2.0f);

	isOrbRisingAnimCompleted = false;

	_JAnim_OrbRising = new JSONAnimator(0, CC_CALLBACK_1(L5_S5_PZSunClock::OnAnimationComplete, this));
	_JAnim_OrbRising->parseAndLoadJSON("ANIM\\L5\\S5ORBRISING.JSON", HOScene);

	InitPuzzle();

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s5SunclockPuzzleAmd,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s5SunclockPuzzleAmd) )		
	{
		Control::Audio->PlaySample(aAmbL5s5SunclockPuzzleAmd,true);
	}
	
	//MUSIC_SFX
	
	

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S5_PZSunClock]  = true;
}

void L5_S5_PZSunClock::InitPuzzle()
{
	clickTimer = -1;

	_tokenRects[0] = new CRectangle(618.0f,336.0f,158.0f,141.0f);
	_tokenRects[1] = new CRectangle(855.0f,315.0f,141.0f,129.0f);
	_tokenRects[2] = new CRectangle(876.0f,190.0f,117.0f,111.0f);
	_tokenRects[3] = new CRectangle(829.0f,86.0f,96.0f,93.0f);
	_tokenRects[4] = new CRectangle(648.0f,66.0f,95.0f,95.0f);
	_tokenRects[5] = new CRectangle(461.0f,88.0f,98.0f,87.0f);
	_tokenRects[6] = new CRectangle(397.0f,193.0f,115.0f,99.0f);
	_tokenRects[7] = new CRectangle(387.0f,311.0f,148.0f,124.0f);

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_PZStatesInitialized)
	{
		ProfileSystem::Instance->ActiveProfile.L5_S5_PZStatesInitialized = true;
		
		int InitTokenStates[MAX_L5S5_PZBLOCKS] = {3,1,1,2, 0,1,1,0};

		for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
		{
			_tokenStates[i] = InitTokenStates[i];
		}

		SaveTokenStates();
	}
	else
	{
		for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
		{
			_tokenStates[i] = ProfileSystem::Instance->ActiveProfile.L5_S5_PZTokenState[i];
		}
	}

	for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
	{
		_baseGlows[i] = HOScene->GetObjectByID(std::string("baseglow" + MKSTR(i)).c_str());
		_baseGlows[i]->SetBlendMode(eBlend_AlphaAdditive);
		_baseGlows[i]->SetEnable(false);

		for (int k = 0; k < 4; k++)
		{
			CObject *token = HOScene->GetObjectByID(std::string("token" + MKSTR(i)+ MKSTR(k)).c_str());
			_tokenArray.push_back(token);
		}
	}

	UpdateBlockStates();
}

void L5_S5_PZSunClock::SaveTokenStates()
{
	for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
	{
		ProfileSystem::Instance->ActiveProfile.L5_S5_PZTokenState[i] = _tokenStates[i];
	}
}

void L5_S5_PZSunClock::UpdateBlockStates()
{
    int size = (int)_tokenArray.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)
	{
		_tokenArray[i]->Show(false);
	}

	for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
	{
		// 0 1 2 3 - Block 1 states
		int istate = _tokenStates[i];
		_tokenArray[i*4+istate]->Show(true);
	}
}


void L5_S5_PZSunClock::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL5theme) )
	{
		Control::Audio->Samples[aTrackL5theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL5theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL5theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL5puzzle) )
				  Control::Audio->PlaySample(aTrackL5puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL5puzzle))
		{
			Control::Audio->PlaySample(aTrackL5puzzle, true);
		}
	}
}

void L5_S5_PZSunClock::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if(clickTimer>0)
	{
		clickTimer -= Control::LogicRate;
		if(clickTimer<0)
		{
			//check puzzle complete
			bool isGameWon = true;
			for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
			{
				if(solvedState[i] != _tokenStates[i])
				{
					isGameWon = false;
					break;
				}
			}

			if(isGameWon)
			{
				HandlePuzzleComplete();
				return;
			}
		}
		return;
	}

	if(isPuzzleSolved)
	{
		if(isOrbRisingAnimCompleted)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
		return;
	}

	if(!isPuzzleActive)
	{
		if(InvDestRectSolution.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L5guide");
			}
		}
	}
	else
	{
		if(HelpRectCenter.Intersection(&pos))
		{
			Cursor::SetMode(CA_Help);
			if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L5hole");
			}
			return;
		}

		//Puzzle Check
		for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
		{
			if(_tokenRects[i]->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if(Control::Input->LBclicked())
				{
					clickTimer = 0.5f;
					changeTokenState(i);

					SaveTokenStates();
				}
			}
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown, eArea_L5_S5_MountainBelow);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L5_S5_MountainBelow);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S5_MountainBelow, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}
}

void L5_S5_PZSunClock::changeTokenState(int id)
{
	Control::Audio->QuickLoadAndPlaySFX("s5pz1_discturned");

	_tokenStates[id] += 1;
	if(_tokenStates[id]>3)
		_tokenStates[id] = 0;
	
	for (int i = 0; i < 4; i++)
	{
		if(_tokenArray[id*4+i]->Visible)
		{
			//fadeout the visible one
			_tokenArray[id*4+i]->FadeOut(2.0f);
		}
		else if(_tokenStates[id] == i)
		{
			//fade in this token
			_tokenArray[id*4+i]->FadeIn(2.0f);
		}
	}

	//SLOG::Log("_tokenArray:", _tokenStates, MAX_L5S5_PZBLOCKS);
}

void L5_S5_PZSunClock::OnAnimationComplete(int animID)
{
	isOrbRisingAnimCompleted = true;
}


void L5_S5_PZSunClock::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(!ProfileSystem::Instance->ActiveProfile.L5_S5_PZSolutionPaperPlaced)
			{
				if (itemVal == eInv_L5_S3_MoonSolution && InvDestRectSolution.Intersection(&dropPos))
				{
					ProfileSystem::Instance->ActiveProfile.L5_S5_PZSolutionPaperPlaced = true;

					Control::Audio->QuickLoadAndPlaySFX("s5pz1_clueused");

					HOScene->GetObjectByID("solvepaper")->SetEnable(true);
					HOScene->GetObjectByID("solvepaper")->FadeIn(2.0f);

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S3_MoonSolution] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S3_MoonSolution);

					Hud->InvBox_StartPuzzleMode();
					isPuzzleActive = true;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L5_S5_PZSunClock::ProcessSkip()
{

	if(!isPuzzleSolved)
	{
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
		{
			_tokenStates[i] = solvedState[i];
		}
		UpdateBlockStates();

		HandlePuzzleComplete();
	}
}

void L5_S5_PZSunClock::HandlePuzzleComplete()
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Control::Audio->QuickLoadAndPlaySFX("s5pz1_puzzlecomplete");

	HOScene->GetObjectByID("solvepaper")->FadeOut(3.0f);

	ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted = true;

	isPuzzleSolved = true;

	if(_handleExtrasGameComplete())
		return;

	s5orbrisingFX->Start();
	_JAnim_OrbRising->PlayAnim();
}

void L5_S5_PZSunClock::ProcessNonHOHint()
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

			break;
		case 1: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L5_S5_PZSolutionPaperPlaced)
			{
				if(ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken)
				{
					Hud->ShowHintForInvItem(eInv_L5_S3_MoonSolution, &InvDestRectSolution);
				}
				else
				{
					InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				}
				return;
			}
			break;
		case 2: ++iNonHOHint;

			break;
		case 3: ++iNonHOHint;

			break;
		case 4: ++iNonHOHint;

			break;
		case 5: ++iNonHOHint;

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

L5_S5_PZSunClock::~L5_S5_PZSunClock()
{
	Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(s5orbrisingFX);
	SAFE_DELETE(s5pzfliesFX);
	SAFE_DELETE(_JAnim_OrbRising);

	for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
	{
		SAFE_DELETE(_tokenRects[i]);
	}

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL5s5SunclockPuzzleAmd);
	Control::Audio->UnloadSample(aAmbL5s5SunclockPuzzleAmd);
	//MUSIC_SFX
}
void L5_S5_PZSunClock::NavigateToScene()
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