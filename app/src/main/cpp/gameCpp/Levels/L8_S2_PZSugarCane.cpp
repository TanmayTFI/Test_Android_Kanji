
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8_S2_PZSugarCane.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

const int TotalLevels = 3;
const CRectangle  KeyRect(351.0f,291.0f,144.0f,78.0f);
const CRectangle  DropAreaDRect(270.0f,4.0f,848.0f,576.0f);
const CRectangle  SugerCaneRect(501.0f,-4.0f,472.0f,537.0f);



L8_S2_PZSugarCane::L8_S2_PZSugarCane()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S2\\PZ2\\SUGERCANE.SCN");

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	//InitPopups();

	_pzScene = new PZBubbleScene();
	_pzScene->Load("LV8\\S2\\PZ2\\PZ_2.SCN");
	_pzScene->setDelegate(this);
	
	_pzSceneHidedelay = 0.0f;
	_pzCurrentLevel = ProfileSystem::Instance->ActiveProfile.L8_S2_SugercanePZLevel;
	_state = (L8_S2_SceneState)ProfileSystem::Instance->ActiveProfile.L8_S2_SugercanePZState;

	if (_state == L8_S2_SceneState_MagicWandAnimation)
	{
		_state = L8_S2_SceneState_PlayPuzzle;
	}

	HOScene->GetObjectByID("Weapon")->SetEnable(false);
	HOScene->GetObjectByID("Sward")->SetEnable(false);

	std::vector<SuperFX*> sfx;
	s2WeaponUseFxFX = new SuperFX("PRT\\L8\\s2WeaponUseFx.INI", PointSystem::CreateCPoint(591,206,0), 19); 
	sfx.push_back(s2WeaponUseFxFX);

	_janim_WeaponUse = new JSONAnimator(0, CC_CALLBACK_1(L8_S2_PZSugarCane::OnAnimationComplete, this));
	_janim_WeaponUse->parseAndLoadJSON("ANIM\\L8\\S2_PZ2_WEAPONUSE.JSON", HOScene, &sfx);

	_janim_SwardUse = new JSONAnimator(1, CC_CALLBACK_1(L8_S2_PZSugarCane::OnAnimationComplete, this));
	_janim_SwardUse->parseAndLoadJSON("ANIM\\L8\\S2_PZ2_SWARDUSE.JSON", HOScene);

	S2CaneGrowFX = new SuperFX("PRT\\L8\\S2CaneGrow.INI", PointSystem::CreateCPoint(694,301,0), 1100); 

	for (int i = 0; i < 4; i++)
	{
		CObject* obj = HOScene->GetObjectByID("CaneLevel_"+MKSTR(i));
		_sugerCaneObjParts.push_back(obj);
		obj->SetAlpha(0);
	}

	LoadState();
	SceneObjectsUpdate();
	TaskPendingUpdate();

	Control::Audio->ClearUnusedTracks();
	if( !Control::Audio->IsPlaying(aTrackLevel5) )
		Control::Audio->PlaySample(aTrackLevel5,true);
	//if( !Control::Audio->IsPlaying(aLightRainAmb) )		
	//{
	//	Control::Audio->Samples[aLightRainAmb]->SetVolume(70);
	//	Control::Audio->PlaySample(aLightRainAmb,true);//will be paused in iPad popups!!
	//}
	//else //ensure 70
	//	Control::Audio->Samples[aLightRainAmb]->SetVolume(70);

	//if( !Control::Audio->IsPlaying(aAmbBeachLoop) )		
	//{
	//	Control::Audio->PlaySample(aAmbBeachLoop,true);//will be paused in iPad popups!!
	//}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S2_PZSugarCane]  = true;


	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	float _lineWidth = 700;
	int _nLines = 4;
	EFont _font = eFont_26;
	eTextAlignment _align = Align_Center;
	std::string temp = PZHelpTextArray[ePZHelp_L8_S2_SugerCanePuzzlePuzzle];
	if(StringTable::IsKeyFound(temp))
	{
		_lineWidth = StringTable::GetLineWidth(temp);
		_nLines = StringTable::GetNoOfLines(temp);
		_font = (EFont)StringTable::GetFont(temp);
	}

	pzHelpText = new MultilineText(_nLines, PointSystem::CreateCPoint(683, 688, 0), eZOrder_INVBOXITEMS, gold, 30, _lineWidth, _align, _font);
								  
	pzHelpText->SetTextMultiline(StringTable::getStr(temp));
	pzHelpText->Show(false);
}

void L8_S2_PZSugarCane::LoadState()
{
	if (ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken)
	{
		HOScene->GetObjectByID("Key")->SetEnable(false);
	}

	if(_pzCurrentLevel > 0)
		_sugerCaneObjParts[_pzCurrentLevel-1]->SetAlpha(1);
	else
		_sugerCaneObjParts[0]->SetAlpha(1);

	switch (_state)
	{
	
	case L8_S2_SceneState_PlayPuzzle:
	case L8_S2_SceneState_PuzzlePlaying:
		{
			_state = L8_S2_SceneState_PlayPuzzle;
		}
		break;
	
	case L8_S2_SceneState_PuzzleSceneHide:
	case L8_S2_SceneState_CaneGrowing:
		_state = L8_S2_SceneState_PuzzleComplete;
		break;
	
	case L8_S2_SceneState_WaitingForSword:
		break;
	case L8_S2_SceneState_SwardPlaying:
		break;
	case L8_S2_SceneState_SwardPlayingComplete:
		break;
	default:
		break;
	}
}


void L8_S2_PZSugarCane::SceneObjectsUpdate()
{
	TaskPendingUpdate();
}

void L8_S2_PZSugarCane::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL8theme) )
	{
		Control::Audio->Samples[aTrackL8theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL8theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL8theme);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL8puzzle) )
				Control::Audio->PlaySample(aTrackL8puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL8puzzle))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL8puzzle, true);
		}
	}
}

void L8_S2_PZSugarCane::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	
	if ((L8_S2_SceneState)ProfileSystem::Instance->ActiveProfile.L8_S2_SugercanePZState != _state)
	{
		ProfileSystem::Instance->ActiveProfile.L8_S2_SugercanePZState = (int)_state;
	}

	switch (_state)
	{
	case L8_S2_SceneState_WaitingForMagicWand:
		{
			if (!ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken && KeyRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					Hud->TakeInventoryItem(eInv_L8_S2_HalfKey);
					HOScene->GetObjectByID("Key")->FadeOut();
					ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken = true;
				}
			}

			if (SugerCaneRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L8_S2_ClickWitoutSugetcane);
				}
			}
		}
		break;
	
	case L8_S2_SceneState_MagicWandAnimation:
		{
			return;
		}
		break;
	
	case L8_S2_SceneState_PlayPuzzle:
		{
			Hud->InvBox_StartPuzzleMode();
			pzHelpText->Show(true);
			Control::Audio->QuickLoadAndPlaySFX("s2pz2_StartPuzzleLevel");
			_pzScene->LoadLevel(_pzCurrentLevel);
			_state = L8_S2_SceneState_PuzzlePlaying;
		}
		break;
	
	case L8_S2_SceneState_PuzzlePlaying:
		{
			_pzScene->update(Control::LogicRate);
		}
		break;
	
	case L8_S2_SceneState_PuzzleComplete:
		{
			_pzSceneHidedelay = 2.0f;
			S2CaneGrowFX->Start();
			Control::Audio->QuickLoadAndPlaySFX("s2p2_CaneGrowing");
			_state = L8_S2_SceneState_PuzzleSceneHide;
		}
		break;
	
	case L8_S2_SceneState_PuzzleSceneHide:
		{
			if (_pzSceneHidedelay > 0.0f)
			{
				_pzSceneHidedelay -= Control::LogicRate;
				if (_pzSceneHidedelay <= 0.0f)
				{
					float val[5] = {1, -1, -1, -1, -1};
					_sugerCaneObjParts[_pzCurrentLevel]->TweenTo(val, 1, 0, true);
					_state = L8_S2_SceneState_CaneGrowing;
				}
			}
		}
		break;

	case L8_S2_SceneState_CaneGrowing:
		{
			if(!_sugerCaneObjParts[_pzCurrentLevel]->IsTweening)
			{
				_sugerCaneObjParts[_pzCurrentLevel - 1]->SetAlpha(0);

				if (_pzCurrentLevel != TotalLevels)
				{
					Control::Audio->QuickLoadAndPlaySFX("s2pz2_WeaponUse");
					_janim_WeaponUse->PlayAnim();
					_state = L8_S2_SceneState_MagicWandAnimation;
				}
				else
				{
					_pzCurrentLevel++; // To confirm level is completed
					ProfileSystem::Instance->ActiveProfile.L8_S2_SugercanePZLevel = _pzCurrentLevel;
					ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown = true;
					AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					Hud->InvBox_StopPuzzleMode();
					pzHelpText->Show(false);
					_state = L8_S2_SceneState_WaitingForSword;
				}
			}
		}
		break;
	
	case L8_S2_SceneState_WaitingForSword:
		{
			if (SugerCaneRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L8_S2_ClickAfterSugetCaneGrown);	
				}
			}
		}
		break;

	case L8_S2_SceneState_SwardPlaying:
		{
			return;
		}
		break;

	case L8_S2_SceneState_SwardPlayingComplete:
		{
			
		}
		break;

	default:
		break;
	}


	if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		SparkleUpdate();
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
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
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
		return;
	}
}

void L8_S2_PZSugarCane::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_PZSugarCane] = isAnyTaskLeft;
}


void L8_S2_PZSugarCane::OnLevelComplete(int currentLevel)
{
	_state = L8_S2_SceneState_PuzzleComplete;
}


void L8_S2_PZSugarCane::SparkleUpdate()
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

void L8_S2_PZSugarCane::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();
	
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
			if ( itemVal == eInv_L8_CB_MagicWandWithoutKey)
			{
				Control::Audio->QuickLoadAndPlaySFX("s2pz2_WeaponUse");
				_janim_WeaponUse->PlayAnim();

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[itemVal] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(itemVal);

				_state = L8_S2_SceneState_MagicWandAnimation;
			}
			else if ( itemVal == eInv_L8_S3_Sword && _state == L8_S2_SceneState_WaitingForSword )
			{
				_janim_SwardUse->PlayAnim();
				Control::Audio->QuickLoadAndPlaySFX("s2pz2_CaneCutAnimation");

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[itemVal] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(itemVal);

				_state = L8_S2_SceneState_SwardPlaying;
			}
			else if( itemVal == eInv_L8_S3_Sword && _state == L8_S2_SceneState_WaitingForMagicWand )
			{
				Hud->CancelDrag();
				Hud->ShowBannerText(Banner_L8_S2_UsingSwardBeforePuzzle);
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		
		if(ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken)
		{
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
			SendMessage(ControlExit, ID, HOSM_NewArea);		
		}
	}
}

void L8_S2_PZSugarCane::ProcessSkip()
{
	_pzScene->FadeOut();

	float val0[5] = {0, -1, -1, -1, -1};
	for (int i = 0; i < TotalLevels-1; i++)
	{
		_sugerCaneObjParts[i]->TweenTo(val0, 1, 0, true);
	}

	S2CaneGrowFX->Start();

	float val1[5] = {1, -1, -1, -1, -1};
	_sugerCaneObjParts[TotalLevels]->TweenTo(val1, 1, 0, true);

	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Hud->InvBox_StopPuzzleMode();
	pzHelpText->Show(false);
	ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown = true;
	ProfileSystem::Instance->ActiveProfile.L8_S2_SugercanePZLevel = TotalLevels+1;
	_state = L8_S2_SceneState_WaitingForSword;
}

void L8_S2_PZSugarCane::ProcessNonHOHint()
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
				{
					if(!ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken)
					{
						Hud->InitHintTrailEmit(&KeyRect);
						return;
					}
				}

			break;
		case 1: ++iNonHOHint;
				{
					if (_state == L8_S2_SceneState_WaitingForMagicWand && ProfileSystem::Instance->ActiveProfile.L8_S1_MagicWandWithoutKeyTaken)
					{
						Hud->ShowHintForInvItem(eInv_L8_CB_MagicWandWithoutKey, &DropAreaDRect);
						return;
					}
				}
			break;
		case 2: ++iNonHOHint;
				{
					if (_state == L8_S2_SceneState_WaitingForSword && ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken)
					{
						Hud->ShowHintForInvItem(eInv_L8_S3_Sword, &DropAreaDRect);
						return;
					}
				}
			break;
		case 3: ++iNonHOHint;
			if (ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden])
			{
				Hud->InitHintTrailEmit(&HoveredBackBtnRectMax, true, CA_ExitDown);
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

void L8_S2_PZSugarCane::OnAnimationComplete(int animId)
{
	if (animId == 0)
	{
		_pzCurrentLevel++;
		ProfileSystem::Instance->ActiveProfile.L8_S2_SugercanePZLevel = _pzCurrentLevel;
		_state = L8_S2_SceneState_PlayPuzzle;
	}
	else if (animId == 1) // sward use
	{
		Hud->TakeInventoryItem(eInv_L8_S2_Sugarcane);
		ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken = true;
		_state = L8_S2_SceneState_SwardPlayingComplete;
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] = true;
	}
}

L8_S2_PZSugarCane::~L8_S2_PZSugarCane()
{
	SAFE_DELETE(_janim_WeaponUse)
	SAFE_DELETE(_janim_SwardUse);
	SAFE_DELETE(_pzScene);
	SAFE_DELETE(S2CaneGrowFX);
	SAFE_DELETE(s2WeaponUseFxFX);

	if(_pzCurrentLevel > 0 && _pzCurrentLevel <= TotalLevels)
	{
		Hud->InvBox_StopPuzzleMode();
		pzHelpText->Show(false);
	}

	SAFE_DELETE(pzHelpText);

	if( !Control::Audio->IsToLevelTrackRoom() )
		if( Control::Audio->IsPlaying(aTrackLevel5) )
			Control::Audio->StopSample(aTrackLevel5);
	/*if( ProfileSystem::Instance->ActiveProfile.CurrentArea != eArea_L1_S2_Frontyard )
	{
	Control::Audio->StopSample(aLightRainAmb);
	Control::Audio->Samples[aLightRainAmb]->SetVolume(100);			
	}*/
}
void L8_S2_PZSugarCane::NavigateToScene()
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