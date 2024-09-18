//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 5 :  PUZZLE SWAP TILE
//====================================

#include "L4_S5_PZSwapTiles.h"
#include "Hud.h"
#include "GameFlow.h"
#include "Util.h"

const CRectangle  AligatorRect(543.0f,545.0f,155.0f,82.0f);
const CRectangle  KeyRect(710.0f,549.0f,133.0f,76.0f);


L4_S5_PZSwapTiles::L4_S5_PZSwapTiles()
{	
	
	IsPuzzleComplete = false;
	IsTilesSwapping = false; 
	ActiveTileIndex[0] = -1;
	ActiveTileIndex[1] = -1;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S5\\P1\\P1.SCN");

	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

	ResetDia = new CHOScene();
	ResetDia->Load("UI\\RESETDIA.SCN");

	CObject* resetText = new CObject();
	resetText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\RESETCNF.SPR", 670, 308, 2);
	resetText->SetUID("resetText");
	ResetDia->PushObject(resetText);

	CObject* mask = new CObject();
	mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
	mask->SetUID("mask");
	mask->SetScale(2);
	ResetDia->PushObject(mask);

	CObject* btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", 590, 420, 5);
	btnText->SetUID("yesBTNText");
	ResetDia->PushObject(btnText);

	btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", 780, 420, 5);
	btnText->SetUID("noBTNText");
	ResetDia->PushObject(btnText);

	ResetDia->SetZOff(eZOrder_Popup);
	ResetDia->Show(false);
	IsResetPop = false;
	
	InitSceneElements();
	InitAnimations();
	if( !ProfileSystem::Instance->ActiveProfile.L4_S5_SwapPuzzleCompleted )
		RandomizeTiles();
	else
		IsPuzzleComplete = true;

	if( !IsPuzzleComplete )
		Hud->InvBox_StartPuzzleMode();

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s5SwapPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s5SwapPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s5SwapPuzzleAmb,true);
	}

	IsTakingInvItem = false;
	//MUSIC_SFX
}

void L4_S5_PZSwapTiles::Required()
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

void L4_S5_PZSwapTiles::Update()
{
	#ifdef _PARTICLEEDITORENABLED
		if(_isParticleEditorActive)
			return;
	#endif // _PARTICLEEDITORENABLED

	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	UpdateTiles(pos);
	

	if( IsPuzzleComplete )
	{
		if(IsTakingInvItem )
			return;

		if(AligatorRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if( Input->LBclicked() )
			{
				ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken = true;
				Hud->TakeInventoryItem(eInv_L4_S5_AlligatorFace);
				IsTakingInvItem = true;
				AligatorHead->SetEnable(false);
			}
		}
		if( KeyRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if (Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken = true;
				Hud->TakeInventoryItem(eInv_L4_S5_CaveDoorKey);
				IsTakingInvItem = true;
				Key->SetEnable(false); 
			}
		}

	}

		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S5_HillSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
		#endif	

	if( IsMouseOverGoBackArea(&pos) )
	{
		
		#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L4_S5_HillSide);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L4_S5_HillSide);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S5_HillSide, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S5_HillSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
									
		}	
	}

	
}

void L4_S5_PZSwapTiles::InitSceneElements()
{
	for (int i = 0; i < 12; i++)
	{
		Tiles[i] = HOScene->GetObjectByID("Tile" + MKSTR(i+1) );
		PositionArray[i] = CPoint(Tiles[i]->Pos);
		RandomArray[i] = i;
		TilesIndex[i] = i;
	}
	AligatorHead = HOScene->GetObjectByID("AligatorHead");
	Key = HOScene->GetObjectByID("Key");
	PuzzleDoor = HOScene->GetObjectByID("Door");
	Hilight[0] = HOScene->GetObjectByID("HoverEffect03");
	Hilight[1] = HOScene->GetObjectByID("HoverEffect02");
	WrongHilight = HOScene->GetObjectByID("HoverEffect01");
	LightAnim  = HOScene->GetObjectByID("Light");
	WrongHilight->SetAlpha(0.0f);
	Hilight[0]->SetEnable(false);
	Hilight[1]->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L4_S5_SwapPuzzleCompleted )
	{
		PuzzleDoor->SetEnable(false);
		if( ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken )
			AligatorHead->SetEnable(false);
		if( ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken )
			Key->SetEnable(false);
	}

}

void L4_S5_PZSwapTiles::InitAnimations()
{
	S5PZ1_PuzzleCompleteFX  = new SuperFX("PRT\\L4\\S5PZ1_PuzzleComplete.INI", PointSystem::CreateCPoint(695,72,0), 1100); 
	S5PZ1_PuzzleComplete1FX = new SuperFX("PRT\\L4\\S5PZ1_PuzzleComplete1.INI", PointSystem::CreateCPoint(697,209,0), 1100); 
	S5PZ1_PuzzleComplete2FX = new SuperFX("PRT\\L4\\S5PZ1_PuzzleComplete2.INI", PointSystem::CreateCPoint(696,351,0), 1100); 
	S5PZ1_PuzzleComplete3FX = new SuperFX("PRT\\L4\\S5PZ1_PuzzleComplete3.INI", PointSystem::CreateCPoint(694,488,0), 1100); 
	S5PZ1_PuzzleComplete4FX = new SuperFX("PRT\\L4\\S5PZ1_PuzzleComplete4.INI", PointSystem::CreateCPoint(379,277,0), 1100); 
	S5PZ1_PuzzleComplete5FX = new SuperFX("PRT\\L4\\S5PZ1_PuzzleComplete5.INI", PointSystem::CreateCPoint(535,282,0), 1100); 
	S5PZ1_PuzzleComplete6FX = new SuperFX("PRT\\L4\\S5PZ1_PuzzleComplete6.INI", PointSystem::CreateCPoint(692,279,0), 1100); 
	S5PZ1_PuzzleComplete7FX = new SuperFX("PRT\\L4\\S5PZ1_PuzzleComplete7.INI", PointSystem::CreateCPoint(851,282,0), 1100); 
	S5PZ1_PuzzleComplete8FX = new SuperFX("PRT\\L4\\S5PZ1_PuzzleComplete8.INI", PointSystem::CreateCPoint(1005,281,0), 1100); 
	




	LightAnim->SetBlendMode(eBlend_AlphaAdditive);
	LightAnim->Animate(0, 0.8f, 9.0f);
}

void L4_S5_PZSwapTiles::RandomizeTiles()
{
	//shuffle shelf pieces
	for( int i = 0; i < 12; i++)
	{
		int randomVal = rand() % 12;
		int temp = RandomArray[i];
		RandomArray[i] = RandomArray[randomVal];
		RandomArray[randomVal] = temp;
	}
	for( int i = 0; i < 12; i++)
	{
		Tiles[i]->Pos = PositionArray[RandomArray[i]];
		TilesIndex[i] = RandomArray[i];
	} 
}

void L4_S5_PZSwapTiles::ProcessNonHOHint()
{
	if(IsTilesSwapping)
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 4;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken )	
				{
					Hud->InitHintTrailEmit(AligatorRect.GetCenterX(), AligatorRect.GetCenterY());
					return;
				}
			break;
		case 1: ++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken )
				{
					Hud->InitHintTrailEmit(KeyRect.GetCenterX(), KeyRect.GetCenterY()); 
					return;
				}
				break;
		case 2: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S5_HillSide] )
			{
				InitHintTrailEmit(HoveredBackBtnRectMax.GetCenterX(), HoveredBackBtnRectMax.GetCenterY(),true,CA_ExitDown); 
				return;
			}
			break;
		case 3: ++iNonHOHint;
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

void L4_S5_PZSwapTiles::UpdateTiles( CPoint pos )
{
	if( IsPuzzleComplete)
		return;

	if( IsTilesSwapping )
	{
		if( !Tiles [ActiveTileIndex[0]]->IsTweening )
		{
			Tiles[ActiveTileIndex[1]]->SetZ( Tiles[ActiveTileIndex[1]]->ZOrder - 10 );
			Tiles[ActiveTileIndex[0]]->SetZ( Tiles[ActiveTileIndex[0]]->ZOrder - 10 );
			Hilight[0]->SetZ( Hilight[0]->ZOrder - 10 );
			Hilight[1]->SetZ( Hilight[1]->ZOrder - 10 );
			Hilight[0]->SetEnable(false);
			Hilight[1]->SetEnable(false);
			ActiveTileIndex[0] = -1;
			ActiveTileIndex[1] = -1;
			if( CheckIfPuzzleSolved() )
			{
				OnPuzzleComplete();
				return;
			}
			IsTilesSwapping = false;
		}
		return;
	}

	for( int i = 0; i < 12; i++ )
	{
		if( Tiles[i]->Collide(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			if( Control::Input->LBclicked() )
			{
				if( ActiveTileIndex[0] == -1 )
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5Popup);
					ActiveTileIndex[0] = i;
					Hilight[0]->SetEnable(true);
					Hilight[0]->SetPos( &Tiles[i]->Pos);
					break;
				}
				else if( ActiveTileIndex[0] != -1 && ActiveTileIndex[0] != i )
				{
					ActiveTileIndex[1] = i;
					Hilight[1]->SetEnable(true);
					Hilight[1]->SetPos( &Tiles[i]->Pos);
					if( CheckIfValid(ActiveTileIndex[0], ActiveTileIndex[1] ))
						SwapTiles( ActiveTileIndex[0], ActiveTileIndex[1] );
					else
						ResetTiles();
					break; //break from loop to avoid multiple selection
				}
			}
		}
	}
}

void L4_S5_PZSwapTiles::ResetTiles()
{
	WrongHilight->SetAlpha(1.0f);
	WrongHilight->SetPos( &Tiles[ActiveTileIndex[1]]->Pos );
	WrongHilight->FadeOut(1.5f);
	ActiveTileIndex[0] = -1;
	ActiveTileIndex[1] = -1;
	Hilight[0]->SetEnable(false);
	Hilight[1]->SetEnable(false);
}


bool L4_S5_PZSwapTiles::CheckIfPuzzleSolved()
{
	bool result = true;
	for( int i = 0; i < 12; i++ )
	{
		if( TilesIndex[i] != i )	
		{
			result = false;
		}
	}
	return(result);
}

bool L4_S5_PZSwapTiles::CheckIfValid(int index1, int index2) 
{
	if( abs(Tiles[index1]->Pos.x - Tiles[index2]->Pos.x ) <= 165 && abs(Tiles[index1]->Pos.y - Tiles[index2]->Pos.y) <= 5)
		return (true);
	if( abs(Tiles[index1]->Pos.y - Tiles[index2]->Pos.y ) <= 150 && abs(Tiles[index1]->Pos.x - Tiles[index2]->Pos.x) <= 5 )
		return (true);
	return(false);
}


void L4_S5_PZSwapTiles::SwapTiles(int index1, int index2)
{
	int tempIndex = TilesIndex[index1];
	TilesIndex[index1] = TilesIndex[index2];
	TilesIndex[index2] = tempIndex;

	Tiles[index1]->SetZ( Tiles[index1]->ZOrder + 10 );
	Tiles[index2]->SetZ( Tiles[index2]->ZOrder + 10 );
	Hilight[0]->SetZ( Hilight[0]->ZOrder + 10 );
	Hilight[1]->SetZ( Hilight[1]->ZOrder + 10 );

	float tweenArray[] = {1.0f,Tiles[index2]->Pos.x, Tiles[index2]->Pos.y, 1,1};
	Tiles[index1]->TweenTo( tweenArray, 0.5f, TW_EASE, true );

	float tweenArray2[] =  {1.0f,Tiles[index1]->Pos.x, Tiles[index1]->Pos.y, 1,1};
	Tiles[index2]->TweenTo( tweenArray2, 0.5f, TW_EASE, true );
	Hilight[0]->TweenTo( tweenArray, 0.5f, TW_EASE, true );
	Hilight[1]->TweenTo( tweenArray2, 0.5f, TW_EASE, true );
	IsTilesSwapping = true;
	Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4BooksMoved);
}


void L4_S5_PZSwapTiles::OnPuzzleComplete()
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	S5PZ1_PuzzleCompleteFX->Start();
	S5PZ1_PuzzleComplete1FX->Start();
	S5PZ1_PuzzleComplete2FX->Start();
	S5PZ1_PuzzleComplete3FX->Start();
	S5PZ1_PuzzleComplete4FX->Start();
	S5PZ1_PuzzleComplete5FX->Start();
	S5PZ1_PuzzleComplete6FX->Start();
	S5PZ1_PuzzleComplete7FX->Start();
	S5PZ1_PuzzleComplete8FX->Start();
	
	//S5PZ1_PuzzleComplete2FX->AdvanceByTime(2.0f);
	//S5PZ1_PuzzleComplete3FX->AdvanceByTime(2.0f);
	//S5PZ1_PuzzleComplete4FX->AdvanceByTime(2.0f);
	//S5PZ1_PuzzleComplete5FX->AdvanceByTime(2.0f);
	//S5PZ1_PuzzleComplete6FX->AdvanceByTime(2.0f);
	//S5PZ1_PuzzleCompleteFX->AdvanceByTime(2.0f);
	//S5PZ1_PuzzleComplete1FX->AdvanceByTime(2.0f);
	//S5PZ1_PuzzleComplete8FX->AdvanceByTime(2.0f);
	//S5PZ1_PuzzleComplete7FX->AdvanceByTime(2.0f);

	IsTilesSwapping = false;
	IsPuzzleComplete = true;
	if(_handleExtrasGameComplete())
		return;

	ProfileSystem::Instance->ActiveProfile.L4_S5_SwapPuzzleCompleted = true;
	Hud->InvBox_StopPuzzleMode();
	float tweenArray[] = {1.0f, PuzzleDoor->Pos.x, PuzzleDoor->Pos.y - 75, 1,1 };
	PuzzleDoor->TweenTo(tweenArray, 0.5f, TW_EASE, true );
}

void L4_S5_PZSwapTiles::ProcessSkip()
{
	if(IsTilesSwapping)
		return;

	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	for (int i = 0; i < 12; i++)
	{
		Tiles[i]->SetPos( &PositionArray[i]); 
	}
	OnPuzzleComplete();
}


float L4_S5_PZSwapTiles::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L4_S5_PZSwapTiles::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

    
}


L4_S5_PZSwapTiles::~L4_S5_PZSwapTiles()
{	
	SAFE_DELETE(S5PZ1_PuzzleCompleteFX);
	SAFE_DELETE(S5PZ1_PuzzleComplete1FX);
	SAFE_DELETE(S5PZ1_PuzzleComplete2FX);
	SAFE_DELETE(S5PZ1_PuzzleComplete3FX);
	SAFE_DELETE(S5PZ1_PuzzleComplete4FX);
	SAFE_DELETE(S5PZ1_PuzzleComplete5FX);
	SAFE_DELETE(S5PZ1_PuzzleComplete6FX);
	SAFE_DELETE(S5PZ1_PuzzleComplete7FX);
	SAFE_DELETE(S5PZ1_PuzzleComplete8FX);
	SAFE_DELETE(ResetDia);

	Hud->InvBox_StopPuzzleMode();

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL4s5SwapPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL4s5SwapPuzzleAmb);
	//MUSIC_SFX
}

void L4_S5_PZSwapTiles::ReceiveMessage(int val)
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

		if( ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken &&  ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken )
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S5_HillSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);		
		}
		IsTakingInvItem = false;
	}	
}

void L4_S5_PZSwapTiles::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(ProfileSystem::Instance->ActiveProfile.L4_S5_SwapPuzzleCompleted)
	{
		ResetBtn->SetEnable(false);
		return;
	}
	if(timer > 0)
	{
		timer -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
		ResetBtn->SetEnable(false);
		if(!IsResetPop)
		{
			if(!isResetDiaFadeOut)
				ResetDia->FadeOut(2);
			isResetDiaFadeOut = true;
		}
		return;
	}
	else
	{
		ResetBtn->SetEnable(true);	
		if(!isFadeIn)
			ResetBtn->FadeIn(1);
		isFadeIn = true;
	}
	if(ResetBtn->IsIntersect(Control::Input->Mpos()))
	{
		ResetBtn->SetAnimation(1);
		if(!IsResetPop)
			Cursor::SetMode(CA_HandPointy);
		if(KInput::getLeftButtonState() == ISDOWN)
		{
			IsResetPop = true;
		}
	}
	else
	{
		ResetBtn->SetAnimation(2);
		Cursor::SetMode(CA_Normal);
	}

	if(IsResetPop)
	{
		if(!isResetDiaFadeIn)
		{
			ResetDia->FadeIn(2);
		}
		if(!isFadeOut)
			ResetBtn->FadeOut(1);
		isFadeOut = true;
		isResetDiaFadeIn = true;
		CObject *resetYes = ResetDia->GetObjectByID("BtnYes");
		CObject *resetNo = ResetDia->GetObjectByID("BtnNo");

		if(Hud->CheckPopupButton(resetYes) == Popup_Click)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S5_PZSwapTile;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			IsResetPop = false;
		}
		if(Hud->CheckPopupButton(resetNo) == Popup_Click)
		{
			IsResetPop = false;
			timer = 10.0f;
			isFadeIn = false;
			isFadeOut = false;
			isResetDiaFadeOut = false;
			isResetDiaFadeIn = false;
		}
	}
	else
	{
		ResetBtn->SetEnable(true);
	}
}
void L4_S5_PZSwapTiles::NavigateToScene()
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