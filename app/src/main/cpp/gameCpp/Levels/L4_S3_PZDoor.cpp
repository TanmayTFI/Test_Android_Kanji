//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 3 :  PUZZLE DOOR
//====================================

#include "L4_S3_PZDoor.h"
#include "Hud.h"
#include "GameFlow.h"
#include "Util.h"

const CRectangle  UpArrowRect(1114.0f,95.0f,77.0f,51.0f);
const CRectangle  DownArrowRect(1102.0f,465.0f,86.0f,49.0f);

L4_S3_PZDoor::L4_S3_PZDoor()
{	
	S3DoorDust1FX	= NULL;
	txtInfo			= NULL;
	BottomScene		= NULL;

	CurrentSceneState = SnakeTopScene;
	CurrentSelectedSymbol = -1;
	prevSelectedSymbol = -1;
	ScrollTimer = 0;
	ScrollTime = 1.5f;
	FadeTimer = 0;
	FadeTime = 1.5f;
	SelectedSymbolType = Dummy;
	IsInputLocked = false;
	SelectionCount = 0;
	IsShaking = false;
	ShakeTimer = 0.0f;
	numMatchesFound = 0;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S3\\P2\\P2TOP.SCN");

	BottomScene = new CHOScene();
	BottomScene->Load("LV4\\S3\\P2\\P2BOTTOM.SCN");
	BottomScene->SaveXY();
	//shift after setting mesh
	BottomScene->ShiftXY(0,768);

	Hud->InvBox_StartPuzzleMode();

	
	
	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	float _lineWidth = 700;
	int _nLines = 3;
	EFont _font = eFont_26;
	eTextAlignment _align = Align_Center;
	std::string temp = "Exscroll";
	if(StringTable::IsKeyFound(temp))
	{
		_lineWidth = StringTable::GetLineWidth(temp);
		_nLines = StringTable::GetNoOfLines(temp);
		_font = (EFont)StringTable::GetFont(temp);
	}
	txtInfo = new MultilineText(_nLines, PointSystem::CreateCPoint(680.0f, 700.0f,0), eZOrder_INVBOXITEMS, gold, 30, _lineWidth, _align, _font);
	//txtInfo->SetTextProps("0 out of 14", 560.0f, 716.0f, eZOrder_INVBOXITEMS, gold, eFont_26, Align_Center);
	txtInfo->SetTextMultiline(StringTable::getStr("Exscroll")+MKSTR(MAX_L4_S3_SYMBOLS-numMatchesFound)+")");
	//txtInfo->Show(false);

	UpArrow = BottomScene->GetObjectByID("ArrowUp");
	DownArrow = HOScene->GetObjectByID("ArrowDown");

	HilightA = HOScene->GetObjectByID("HilightA");
	HilightA->SetZ( HilightA->ZOrder + 50 );
	HilightA->SetEnable(false);

	HilightB = BottomScene->GetObjectByID("HilightB");
	HilightB->SetZ( HilightA->ZOrder + 50 );
	HilightB->SetEnable(false);


	DummyRect[0]	= CRectangle(569.0f,391.0f,48.0f,47.0f);
	DummyRect[1]	= CRectangle(565.0f,134.0f,51.0f,53.0f);
	DummyRect[2]	= CRectangle(556.0f,499.0f,51.0f,48.0f);
	DummyRect[3]	= CRectangle(658.0f,208.0f,50.0f,50.0f);
	DummyRect[4]	= CRectangle(427.0f,158.0f,49.0f,50.0f);
	DummyRect[5]	= CRectangle(885.0f,279.0f,59.0f,51.0f);
	DummyRect[6]	= CRectangle(677.0f,326.0f,52.0f,53.0f);
	DummyRect[7]	= CRectangle(346.0f,477.0f,56.0f,52.0f);
	DummyRect[8]	= CRectangle(910.0f,151.0f,50.0f,51.0f);
	DummyRect[9]	= CRectangle(908.0f,369.0f,50.0f,53.0f);

	SymbolRectA[0]	= CRectangle(542.0f,254.0f,59.0f,59.0f);
	SymbolRectA[1]	= CRectangle(816.0f,379.0f,71.0f,60.0f);
	SymbolRectA[2]	= CRectangle(879.0f,505.0f,58.0f,55.0f);
	SymbolRectA[3]	= CRectangle(878.0f,171.0f,47.0f,44.0f);
	SymbolRectA[4]	= CRectangle(664.0f,541.0f,64.0f,65.0f);
	SymbolRectA[5]	= CRectangle(411.0f,261.0f,59.0f,59.0f);
	SymbolRectA[6]	= CRectangle(497.0f,392.0f,48.0f,45.0f);
	SymbolRectA[7]	= CRectangle(482.0f,255.0f,58.0f,55.0f);

	SymbolRectB[0]	= CRectangle(848.0f,250.0f,54.0f,59.0f);
	SymbolRectB[1]	= CRectangle(433.0f,385.0f,68.0f,57.0f);
	SymbolRectB[2]	= CRectangle(426.0f,148.0f,59.0f,50.0f);
	SymbolRectB[3]	= CRectangle(420.0f,507.0f,54.0f,52.0f);
	SymbolRectB[4]	= CRectangle(798.0f,491.0f,61.0f,56.0f);
	SymbolRectB[5]	= CRectangle(793.0f,150.0f,49.0f,48.0f);
	SymbolRectB[6]	= CRectangle(790.0f,142.0f,55.0f,57.0f);
	SymbolRectB[7]	= CRectangle(577.0f,494.0f,56.0f,52.0f);
		

	for( int i = 0; i < MAX_L4_S3_SYMBOLS; i++ )
	{
		MatchedIndex[i] =  false;
		if( i == 7 )
		{
			SymbolsA[i]			= BottomScene->GetObjectByID("0" + MKSTR(i+1) + "SymbolIdleA");
			ActiveSymbolsA[i]	= BottomScene->GetObjectByID("0" + MKSTR(i+1) + "SymbolGlowA");
		}
		else
		{
			SymbolsA[i]			= HOScene->GetObjectByID("0" + MKSTR(i+1) + "SymbolIdleA");
			ActiveSymbolsA[i]	= HOScene->GetObjectByID("0" + MKSTR(i+1) + "SymbolGlowA");
		}

		if( i == 3 || i == 6 )
		{
			SymbolsB[i]			= HOScene->GetObjectByID("0" + MKSTR(i+1) + "SymbolIdleB");
			ActiveSymbolsB[i]	= HOScene->GetObjectByID("0" + MKSTR(i+1) + "SymbolGlowB");
		}
		else
		{
			SymbolsB[i]			= BottomScene->GetObjectByID("0" + MKSTR(i+1) + "SymbolIdleB");
			ActiveSymbolsB[i]	= BottomScene->GetObjectByID("0" + MKSTR(i+1) + "SymbolGlowB");
		}
			
		ActiveSymbolsA[i]->SetEnable(false);
		ActiveSymbolsB[i]->SetEnable(false);
	}

	for (int i = 0; i < MAX_L4_S3_DUMMYSYMBOLS; i++)
	{
		if( i > 1 && i < 6)
		{
			DummySymbols[i]		= HOScene->GetObjectByID("Dummy0" + MKSTR(i+1) ); 
		}
		else if( i != 9 )
		{
			DummySymbols[i]		= BottomScene->GetObjectByID("Dummy0" + MKSTR(i+1) ); 
		}
		else
		{
			DummySymbols[i]		= BottomScene->GetObjectByID("Dummy" + MKSTR(i+1) ); 
		}
	}

	S3DoorDust1FX = new SuperFX("PRT\\L4\\S3DoorDust1.INI", PointSystem::CreateCPoint(309,17,0), 1100); 
	
	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s3DoorPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s3DoorPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s3DoorPuzzleAmb,true);
	}
	
	//MUSIC_SFX

}

void L4_S3_PZDoor::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;	

	if( Control::Audio->IsPlaying(aTrackL4theme) )
	{
		Control::Audio->Samples[aTrackL4theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL4theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL4theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL4puzzle) )
				  Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL4puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
	}
}

void L4_S3_PZDoor::ProcessSkip()
{
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved )
	{
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		txtInfo->Show(false);
		Control::Audio->QuickLoadAndPlaySFX("s3pz2_puzzleComplete");
		Hud->InvBox_StopPuzzleMode();
		S3DoorDust1FX->Start();
		S3DoorDust1FX->AdvanceByTime(2.0f);
		ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved = true;
		HOScene->Shake(4,5,2.0f);
		BottomScene->Shake(4,5,2.0f);
		ShakeTimer = 0.0f;
		IsShaking = true;
	} 
}

void L4_S3_PZDoor::Update()
{
	#ifdef _PARTICLEEDITORENABLED
		if(_isParticleEditorActive)
			return;
	#endif // _PARTICLEEDITORENABLED

	NavigateToScene();
	if( IsShaking )
	{
		if( ShakeTimer >= 2.5f )
		{
			S3DoorDust1FX->StopImmediate();
			IsShaking = false;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
		ShakeTimer += Control::LogicRate;
		BottomScene->update(Control::LogicRate);
		HOScene->update(Control::LogicRate);
		return;
	}



	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if( IsInputLocked )
	{
		Cursor::SetMode(CA_Normal);
		FadeTimer += Control::LogicRate;
		if( FadeTimer > FadeTime )
		{
			IsInputLocked = false;
			FadeTimer = 0;
			SelectionCount = 0;
			HilightA->SetEnable(false);
			HilightB->SetEnable(false);
			prevSelectedSymbol = -1;
			CurrentSelectedSymbol = -1;
			HilightA->PlayAnimation(0);
			HilightB->PlayAnimation(0);
		}

		return ;
	}


	if( DownArrowRect.Intersection(&pos) && CurrentSceneState == SnakeTopScene )
	{
		DownArrow->PlayAnimation(1);
		if (Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s3pz2_sliding");
			CurrentSceneState = MovingDown;
			//HilightB->SetEnable(false);
		}
		return;
	}
	else if( UpArrowRect.Intersection(&pos) && CurrentSceneState == SnakeBottomScene )
	{
		UpArrow->PlayAnimation(1);
		if (Control::Input->LBclicked())
		{
			Control::Audio->QuickLoadAndPlaySFX("s3pz2_sliding");
			CurrentSceneState = MovingUp;
			//HilightA->SetEnable(false);
		}
		return;
	}

	if( CurrentSceneState == MovingDown || CurrentSceneState == MovingUp)
	{
		ScrollTimer += Control::LogicRate;
		
		//Hoscene to up
		if(CurrentSceneState == MovingDown)
		{
			float newPos = (0 + (-768.0f)*easing(ScrollTimer,0,1,ScrollTime));
			HOScene->ShiftXY(0,newPos);
			newPos = (768 + (-768)*easing(ScrollTimer,0,1,ScrollTime));
			BottomScene->ShiftXY(0,newPos);
		}
		else
		{
			float newPos = (-768 + (768)*easing(ScrollTimer,0,1,ScrollTime));
			HOScene->ShiftXY(0,newPos);
			newPos = (0 + (768)*easing(ScrollTimer,0,1,ScrollTime));
			BottomScene->ShiftXY(0,newPos);
		}
		

		if(ScrollTimer>ScrollTime)
		{
			if( CurrentSceneState == MovingUp )
			{
				CurrentSceneState = SnakeTopScene;
			}
			else
			{
				CurrentSceneState = SnakeBottomScene;
			}
			ScrollTimer = 0;
			
		}
		return;
	}

	for (int i = 0; i < MAX_L4_S3_SYMBOLS; i++)
	{
		if(CurrentSceneState == SnakeTopScene)
		{
			if( SymbolRectA[i].Intersection(&pos) && i != 7 )
			{
				Cursor::SetMode(CA_HandPointy);
				if(Input->LBclicked() )
				{
					if( CurrentSelectedSymbol == -1 )
					{
						if(MatchedIndex[i])
							return;
						SelectedSymbolType = SymbolA;
						SelectSymbol( true, &SymbolsA[i]->Pos );
						CurrentSelectedSymbol = i;
					}
					else
					{
						if( SelectedSymbolType == Dummy )
						{
							if(MatchedIndex[i])
								return;
							SelectSymbol( false, &SymbolsA[i]->Pos );
							ResetActiveSymbols(i);
						}
						else if( CurrentSelectedSymbol == i && SelectedSymbolType != SymbolA )
							ActivateSymbol(i);
						else if( !MatchedIndex[i] && CurrentSelectedSymbol != i )
							SelectSymbol(true, &SymbolsA[i]->Pos );
					}
					
				}
				return;
			}
			else if( SymbolRectB[i].Intersection(&pos) && ( i == 3 || i == 6 ) )
			{
				Cursor::SetMode(CA_HandPointy);
				if(Input->LBclicked() )
				{
					if (CurrentSelectedSymbol == -1 )
					{
						if(MatchedIndex[i])
							return;
						SelectedSymbolType = SymbolB;
						SelectSymbol( true, &SymbolsB[i]->Pos );
						CurrentSelectedSymbol = i;
					}
					else
					{
						if( SelectedSymbolType == Dummy )
						{
							if(MatchedIndex[i])
								return;
							SelectSymbol( false, &SymbolsB[i]->Pos );
							ResetActiveSymbols(i);
						}
						else if( CurrentSelectedSymbol == i && SelectedSymbolType != SymbolB)
							ActivateSymbol(i);
						else if( !MatchedIndex[i] && CurrentSelectedSymbol != i)
							SelectSymbol(true, &SymbolsB[i]->Pos );
					}
				}
				return;
			} 
		}
		else if(CurrentSceneState == SnakeBottomScene)
		{
			if( SymbolRectB[i].Intersection(&pos) && ( i != 3 || i != 6 ))
			{
				Cursor::SetMode(CA_HandPointy);
				if(Input->LBclicked() )
				{
					if (CurrentSelectedSymbol == -1 )
					{
						if(MatchedIndex[i])
							return;
						SelectedSymbolType = SymbolB;
						SelectSymbol( true, &SymbolsB[i]->Pos );
						CurrentSelectedSymbol = i;
					}
					else
					{
						if( SelectedSymbolType == Dummy )
						{
							if(MatchedIndex[i])
								return;
							SelectSymbol( false, &SymbolsB[i]->Pos );
							ResetActiveSymbols(i);
						}
						else if( CurrentSelectedSymbol == i && SelectedSymbolType != SymbolB)
							ActivateSymbol(i);
						else if( !MatchedIndex[i] && CurrentSelectedSymbol != i)
							SelectSymbol(true, &SymbolsB[i]->Pos );
					}
				}
				return;
			}
			else if( SymbolRectA[i].Intersection(&pos) && (i == 7 ))
			{
				Cursor::SetMode(CA_HandPointy);
				if(Input->LBclicked() )
				{
					if (CurrentSelectedSymbol == -1 )
					{
						if(MatchedIndex[i])
							return;
						SelectedSymbolType = SymbolA;
						SelectSymbol(true, &SymbolsA[i]->Pos );
						CurrentSelectedSymbol = i;
					}
					else
					{
						if( SelectedSymbolType == Dummy )
						{
							if(MatchedIndex[i])
								return;
							SelectSymbol( false, &SymbolsA[i]->Pos );
							ResetActiveSymbols(i);
						}
						else if( CurrentSelectedSymbol == i && SelectedSymbolType != SymbolA)
							ActivateSymbol(i);
						else if(!MatchedIndex[i] && CurrentSelectedSymbol != i)
							SelectSymbol(true, &SymbolsA[i]->Pos );
					}
				}
				return;
			} 
		}
		
	}

	for (int i = 0; i < MAX_L4_S3_DUMMYSYMBOLS; i++)
	{
		if( DummyRect[i].Intersection(&pos) )
		{

			if( i > 1 && i < 6 )
			{
				if(CurrentSceneState == SnakeTopScene)
					Cursor::SetMode(CA_HandPointy);
			}
			else if(CurrentSceneState == SnakeBottomScene)
				Cursor::SetMode(CA_HandPointy);

			if(Input->LBclicked() )
			{
				SelectedSymbolType = Dummy;
				if( CurrentSelectedSymbol != -1 )
				{
					SelectSymbol( false, &DummySymbols[i]->Pos);
					CurrentSelectedSymbol = -1;
					return;
				}
				SelectSymbol(false, &DummySymbols[i]->Pos);
				CurrentSelectedSymbol = -2;
			}
			return;
		} 
	}

	if (CurrentSceneState != MovingDown && CurrentSceneState != MovingUp )
	{
		if(CurrentSceneState == SnakeTopScene )
			DownArrow->PlayAnimation(0);
		else if(CurrentSceneState == SnakeBottomScene )
			UpArrow->PlayAnimation(0); 
	}

	if( IsMouseOverGoBackArea(&pos) )
	{
		
		#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L4_S3_Cave);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L4_S3_Cave);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S3_Cave, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif
	
}

void L4_S3_PZDoor::ActivateSymbol(int index)
{
	numMatchesFound++;
	txtInfo->SetTextMultiline(StringTable::getStr("Exscroll")+MKSTR(MAX_L4_S3_SYMBOLS-numMatchesFound)+")");
	Control::Audio->QuickLoadAndPlaySFX("s3pz2_Click");
	ActiveSymbolsA[index]->SetEnable(true);
	ActiveSymbolsB[index]->SetEnable(true);
	MatchedIndex[index] = true;
	CheckIfPuzzleSolved();
	CurrentSelectedSymbol = -1;
	prevSelectedSymbol = -1;
	HilightA->PlayAnimation(0);
	HilightB->PlayAnimation(0);
	HilightA->SetEnable(false);
	HilightB->SetEnable(false);
	SelectionCount = 0;
}


void L4_S3_PZDoor::ResetActiveSymbols(int index)
{
	Control::Audio->QuickLoadAndPlaySFX("s3pz2_incorrectInput");
	if( !MatchedIndex[index])
	{
		ActiveSymbolsA[index]->SetEnable(false);
		ActiveSymbolsB[index]->SetEnable(false);
	}
	CurrentSelectedSymbol = -1;
	prevSelectedSymbol = -1;
	SelectionCount = 1;
	IsInputLocked = true;
	HilightA->PlayAnimation(1);
	HilightB->PlayAnimation(1);
}

void L4_S3_PZDoor::SelectSymbol( bool isValid, CPoint *pos)
{
	if( SelectionCount > 0 )
	{
		IsInputLocked = true;
		HilightA->PlayAnimation(1);
		HilightB->PlayAnimation(1);
	}
	SelectionCount++;
	if( CurrentSceneState == SnakeTopScene )
	{
		if( CurrentSelectedSymbol == prevSelectedSymbol )
		{
			HilightA->SetPos(pos);
			HilightA->SetEnable(true);
		}
		else
		{
			HilightB->SetPos(pos);
			HilightB->SetEnable(true);
		}
	}
	else
	{
		if( CurrentSelectedSymbol == prevSelectedSymbol )
		{
			HilightB->SetPos(pos);
			HilightB->SetEnable(true);
		}
		else
		{
			HilightA->SetPos(pos);
			HilightA->SetEnable(true);
		}
	}
	Control::Audio->QuickLoadAndPlaySFX("s3pz2_Click");
	prevSelectedSymbol = CurrentSelectedSymbol;
}


void L4_S3_PZDoor::CheckIfPuzzleSolved()
{
	bool flag = true;
	for (int i = 0; i < MAX_L4_S3_SYMBOLS; i++)
		if( MatchedIndex[i] == false )
			flag = false;
	if(flag)
	{
		txtInfo->Show(false);
		Control::Audio->QuickLoadAndPlaySFX("s3pz2_puzzleComplete");
		Hud->InvBox_StopPuzzleMode();
		S3DoorDust1FX->Start();
		S3DoorDust1FX->AdvanceByTime(2.0f);
		ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved = true;
		HOScene->Shake(4,5,2.0f);
		BottomScene->Shake(4,5,2.0f);
		ShakeTimer = 0.0f;
		IsShaking = true;
		AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	}
}


float L4_S3_PZDoor::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L4_S3_PZDoor::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

    
}

L4_S3_PZDoor::~L4_S3_PZDoor()
{	
	Hud->InvBoxCtrl->ResetToInvBox();
	
	HilightA->SetEnable(false);
	HilightB->SetEnable(false);
	//txtInfo->Show(false);
	
	for (int i = 0; i < MAX_L4_S3_SYMBOLS; i++)
	{
		SymbolsA[i]->SetEnable(false);
		SymbolsB[i]->SetEnable(false);
		ActiveSymbolsA[i]->SetEnable(false);
		ActiveSymbolsB[i]->SetEnable(false);
	}
	for (int i = 0; i < MAX_L4_S3_DUMMYSYMBOLS; i++)
	{
		DummySymbols[i]->SetEnable(false);
	} 

	SAFE_DELETE(S3DoorDust1FX);
	SAFE_DELETE(txtInfo);
	SAFE_DELETE(BottomScene);

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL4s3DoorPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL4s3DoorPuzzleAmb);
	//MUSIC_SFX
}

void L4_S3_PZDoor::ReceiveMessage(int val)
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
		//CPoint dropPos;
		//int itemVal;
		
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L4_S3_PZDoor::ProcessNonHOHint()
{
	Hud->ShowNothingToDoMsgBanner();
}
void L4_S3_PZDoor::NavigateToScene()
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