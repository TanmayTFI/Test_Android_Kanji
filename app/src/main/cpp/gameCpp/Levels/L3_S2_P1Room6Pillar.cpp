//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room6Pillar
//====================================

#include "L3_S2_P1Room6Pillar.h"
#include "HOScene.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include <math.h>
#include "Hud.h"

#define WheelRotateTo 180

static CPoint WheelSize = CPoint(172, 174, 0);
static CPoint DustPartclePos[] = {CPoint(465, 531, 0), CPoint(699, 516, 0), CPoint(965, 519, 0)};
static PillerState InitialStates[] = {Piller_Top, Piller_Bottom, Piller_Top};

const CRectangle  LeftBoxRect(296.0f,135.0f,66.0f,64.0f);
const CRectangle  RightBoxRect(1023.0f,139.0f,75.0f,60.0f);
const CRectangle  BoxRects[] = {LeftBoxRect, RightBoxRect};

const CRectangle  LeftBoxScene(667.0f,190.0f,209.0f,207.0f);
const CRectangle  RightBoxScene(586.0f,201.0f,152.0f,133.0f);

const CRectangle  SceneArea(166.0f,35.0f,1133.0f,435.0f);
const CRectangle  HammerDropRect(233.0f,438.0f,956.0f,271.0f);


/*------------PillarWheel-----------*/

PillarWheel::PillarWheel()
{
	_isWheelRotating = 0;
	_pillerState = Piller_Top;
	_animTimer = 0.0f;
	_currentAngle = 0.0f;

	_cursorOnWheel = false;
	_pillerDustFX = NULL;
	_isWheelDisabled = false;
}

PillarWheel::~PillarWheel()
{
	if( _pillerDustFX )
		_pillerDustFX->StopEmit();
	SAFE_DELETE(_pillerDustFX);
}

void PillarWheel::SetPillerState(PillerState state)
{
	_pillerState = _pillerNextState = state;
	_piller->Pos.y = (float)(_pillerState);
	Init();
}

void PillarWheel::Init()
{
	int i = 0;

	while (i < (int)(_pillerStates.size()))
	{
		int posY = _pillerStates.front();

		_pillerStates.pop();
		_pillerStates.push(PillerState(posY));

		if (posY == _piller->Pos.y)
			break;

		i++;
	}
}

void PillarWheel::Update(float ds)
{
	CObject::Update(ds);

	if (_pillerState != _pillerNextState)
	{
		_animTimer += Control::LogicRate * 0.5f;

		_piller->Pos.y = _pillerState + ((_pillerNextState - _pillerState) * easing(_animTimer, 0, 1, 1) );
		_piller->Pos.x += (0.5f * sin((360 * _animTimer) / 2));

		float angle = _currentAngle - (WheelRotateTo*easing(_animTimer, 0, 1, 1));
		SetRotate(angle);

		if (_animTimer >= 1)
		{
			_pillerState = _pillerNextState;
			_piller->Pos.y = (float)(_pillerState);
			_currentAngle = GetRotate() >= 360 ? 360 - GetRotate() : GetRotate();
			SetRotate(_currentAngle);
			_animTimer = 0;
			_interface->OnWheelEndRotate(this);
			_pillerDustFX->StopEmit();
		}
	}
}

void PillarWheel::OnMouseOver()
{
	CObject::OnMouseOver();
	_cursorOnWheel = true;
}

void PillarWheel::OnMouseClick()
{
	CObject::OnMouseClick();

	if (_animTimer != 0 || _isWheelDisabled)
		return;

	_pillerNextState = _pillerStates.front();
	_pillerStates.pop();
	_pillerStates.push(_pillerNextState);

	_interface->OnWheelStartRotate(this);
	_pillerDustFX->Start();
}

void PillarWheel::OnMouseOut()
{
	CObject::OnMouseOut();
	_cursorOnWheel = false;
}


/*-------------L3_S2_P1Room6Pillar-------------*/

L3_S2_P1Room6Pillar::L3_S2_P1Room6Pillar()
{
	Visible = false;
	musicFadeOutTimer = 0;
	boxPickScene = NULL;

	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S2\\MAZE6\\MAZE6.SCN");
	
	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;
	vidTimer = 0;
	vidTimer2 = 0;

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

	_puzzleState = ProfileSystem::Instance->ActiveProfile.L3_S2_M6_SceneState;
	_stateMachine.push(_puzzleState);

	_collectedBoxes.clear();
	_enabledBox = NO_BOX;
	InitSceneProperties();
	InitPuzzle();

	ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom = eArea_Nil;

	if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_Sickle] &&
		ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken)
	{
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_Sickle] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_Sickle);
	}

	//MUSIC_SFX
	Control::Audio->LoadSample(aVOL3MaryRefS2_1,	AudioVO);
	Control::Audio->LoadSample(aAmbL3s2Maze,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s2Maze) )		
	{
		Control::Audio->PlaySample(aAmbL3s2Maze,true);
	}
	
	//MUSIC_SFX
	

	TaskPendingUpdate();
}


L3_S2_P1Room6Pillar::~L3_S2_P1Room6Pillar()
{
	
	Hud->InvBox_StopPuzzleMode();
	
	M6LighitParticleFX->StopEmit();
	SAFE_DELETE(M6LighitParticleFX);
	SAFE_DELETE(_vdo_LasiaAttack);
	SAFE_DELETE(subtitleText);
	SAFE_DELETE(_vdo_CrystalBreak);
	SAFE_DELETE(ResetDia);
		
	for (int i = 0; i < (int)(_moveToBoxVdos.size()); i++)
	{
		_moveToBoxVdos[i]->StopVideo();
		SAFE_DELETE(_moveToBoxVdos[i]);
	}
	_moveToBoxVdos.clear();

	SAFE_DELETE(boxPickScene);
	//MUSIC_SFX
	Control::Audio->StopSample(aVOL3MaryRefS2_1);
	Control::Audio->UnloadSample(aVOL3MaryRefS2_1);
	Control::Audio->StopSample(aAmbL3s2Maze);
	Control::Audio->UnloadSample(aAmbL3s2Maze);
	//K_LOG("audio samples cleared");
	
	//MUSIC_SFX
}

void L3_S2_P1Room6Pillar::InitSceneProperties()
{
	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVid(0.0f,0.0f,1366.0f,767.0f);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	_vdo_LasiaAttack = new CVideo("OGV\\L3\\MAZE6LASYAATTACK.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	time = 0;
	subIndex = 0;
	subMax = 1;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));
	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("lasyamazeattack"));
	//_vdo_LasiaAttack->SetScale(1.334f);
	_vdo_LasiaAttack->SetScale(GFApp->video_aspectRatio);
	_vdo_LasiaAttack->SetSkipActive(1.0f);

	_vdo_CrystalBreak = new CVideo("OGV\\L3\\MAZE6CRYSTALBREAKING.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//_vdo_CrystalBreak->SetScale(1.334f);
	_vdo_CrystalBreak->SetScale(GFApp->video_aspectRatio);

	 _puzzleState = (_puzzleState == M6_Scene_LasiaAttack) ? M6_Scene_AfterLasiaAttack : _puzzleState;
	 _puzzleState = (_puzzleState == M6_Scene_BrakingCrystal) ? M6_Scene_BreakingCrystalComplete : _puzzleState;

	if(_puzzleState != M6_Scene_AfterLasiaAttack)
	{
		HOScene->GetObjectByID("Crystal_Left")->SetEnable(false);
		HOScene->GetObjectByID("Crystal_Right")->SetEnable(false);
	}

	for (int i = 1; i <= 2; i++)
	{
		CVideo *vdo = new CVideo("OGV\\L3\\MAZE6CUTSCENE_" + MKSTR(i) +".OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//vdo->SetScale(1.334f);
		vdo->SetScale(GFApp->video_aspectRatio);
		_moveToBoxVdos.push_back(vdo);

		CObject *box = HOScene->GetObjectByID("box_" + MKSTR(i));
		_boxObjects.push_back(box);

		CObject *boxglow = HOScene->GetObjectByID("boxGlow_" + MKSTR(i));
		boxglow->SetEnable(false);
		_boxGlowObjects.push_back(boxglow);

		_boxPickSceneFile.push_back("LV3\\S2\\MAZE6\\BOXPICK_" + MKSTR(i) + ".SCN");
	}

	_overlap = HOScene->GetObjectByID("overlap");
	_overlap->SetZ(eZOrder_CutsceneFull + 1);
	_overlap->SetEnable(false);

	CObject *vine_lt_1 = HOScene->GetObjectByID("vine_lt_1");
	CObject *vine_lt_2 = HOScene->GetObjectByID("vine_lt_2");
	CObject *vine_lt_3 = HOScene->GetObjectByID("vine_lt_3");
	CObject *vine_lt_4 = HOScene->GetObjectByID("vine_lt_4");
	CObject *vine_rt_2 = HOScene->GetObjectByID("vine_rt_2");
	CObject *vine_rt_1 = HOScene->GetObjectByID("vine_rt_1");

	CObject *light_ray = HOScene->GetObjectByID("top_light");

	light_ray->Animate(0.65f, 0.8f, 4);
	light_ray->SetBlendMode(eBlend_AlphaAdditive);

	vine_lt_2->Animate(0, 7, 0, 0, 8);

	vine_lt_1->Animate(0, 0, 6, 0, 7);
	vine_lt_1->SetPivot(0, 0 - 250);

	vine_lt_3->Animate(0, 5, 4, 0, 6);
	vine_lt_3->SetPivot(0, 0 - 150);

	vine_lt_4->Animate(0, 0, 5, 0, 4);
	vine_lt_4->SetPivot(0, 0 - 150);

	vine_rt_2->Animate(0, 5, 0, 0, 5);

	vine_rt_1->Animate(0, 0, 5, 0, 4);
	vine_rt_1->SetPivot(0, 0 - 150);

	M6LighitParticleFX = new SuperFX("PRT\\L3\\M6LighitParticle.INI", PointSystem::CreateCPoint(735,246,0), 1100); 
	M6LighitParticleFX->SetZ(60);

	M6LighitParticleFX->Start();
	M6LighitParticleFX->AdvanceByTime(2.0f);
}

//_PUZZLE
void L3_S2_P1Room6Pillar::InitPuzzle()
{
	queue<PillerState> possibleStates;
	possibleStates.push(Piller_Top);
	possibleStates.push(Piller_Middle);
	possibleStates.push(Piller_Bottom);
	possibleStates.push(Piller_Middle);

	_stateOnVdoEnd[0] = M6_Puzzle_Scene_Left;
	_stateOnVdoEnd[1] = M6_Puzzle_Scene_Right;

	for (int i = 0; i < 2; i++)
	{
		int boxindex = ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[i];
		
		if (boxindex != -1)
		{
			_collectedBoxes.push_back(BoxToPick(boxindex)); 
			_boxObjects[boxindex]->SetEnable(false);	
		}
	}

	PillerState pillerPos[3];
	vector<PillerState> pillerPosforSolution;

	for (int i = 0; i < 3; i++)
	{
		int pos = ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPositions[i];

		if (pos != 0)
		{
			pillerPos[i] = PillerState(pos);
		}
		else
		{
			pillerPos[i] = InitialStates[i];
			ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPositions[i] = int(InitialStates[i]);
		}

		pillerPosforSolution.push_back(pillerPos[i]);
	}

	_enabledBox = PuzzleSolution::getSolution(pillerPosforSolution);

	for (int i = 0; i < (int)(_collectedBoxes.size()); i++)
	{
		if (_enabledBox == _collectedBoxes[i])
			_enabledBox = NO_BOX;
	}

	if (_enabledBox != NO_BOX)
	{
		Hud->ShowBannerText(Banner_L3_S2_M6_PillerSolved);
		_boxGlowObjects[_enabledBox]->SetEnable(true);
		_boxGlowObjects[_enabledBox]->Animate(0.4f, 1.0f, 1.5f);
	}


	for (int i = 0; i <= 2; i++)
	{
		CObject *piller = HOScene->GetObjectByID("piller_"+MKSTR(i+1));
		CObject *Wheel = HOScene->GetObjectByID("Wheel_"+MKSTR(i+1));
		Wheel->SetEnable(false);
		
		pillarWheels[i] = NULL;
		pillarWheels[i] = new PillarWheel();
		pillarWheels[i]->LoadInit(Wheel->Filename, Wheel->Pos.x, Wheel->Pos.y, 15, true);
		pillarWheels[i]->SetUID(Wheel->UID);
		pillarWheels[i]->setInterface(this);
		pillarWheels[i]->setPiller(piller);
		pillarWheels[i]->setPillerPossibleStates(possibleStates);
		pillarWheels[i]->setPillerDustFX(new SuperFX("PRT\\L3\\S2PillerDust.INI", &DustPartclePos[i], 1100));
		HOScene->PushObject(pillarWheels[i]);
		pillarWheels[i]->SetPillerState(pillerPos[i]);
		pillarWheels[i]->SetMouseInteractionArea(WheelSize.x, WheelSize.y);
		
		if (_puzzleState < M6_Puzzle_Start)
		{
			pillarWheels[i]->setWheelDisabled(true);
		}
		_PillarWheels.push_back(pillarWheels[i]);
		HOScene->RemoveObject(Wheel);
	}

	switch (_puzzleState)
	{
	case M6_Puzzle_Scene_Left:
	case M6_Puzzle_Scene_Right:
		{
			boxPickScene = new CHOScene();
			boxPickScene->Load(_boxPickSceneFile[_enabledBox]);
			boxPickScene->SetZOff(100);
			boxPickScene->FadeIn(2);
			boxPickScene->GetObjectByID("popfireanim")->SetScale(2.5f);
			boxPickScene->GetObjectByID("vine_Scene")->Animate(0, 5, 0, 0, 5);
			_boxOnScene = boxPickScene->GetObjectByID("BoxToPick");

			for (int i = 0; i < (int)(_PillarWheels.size()); i++)
			{
				_PillarWheels[i]->setWheelDisabled(true);
			}
		}
		break;

	default: break;
	}
}


void L3_S2_P1Room6Pillar::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(_puzzleState == M6_Scene_LasiaAttack || _puzzleState == M6_Scene_BrakingCrystal || _puzzleState ==  M6_Puzzle_Onvdo)
		return;

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

void L3_S2_P1Room6Pillar::Update()
{
	NavigateToScene();
	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	CPoint pos = Control::Input->Mpos();

	if (_stateMachine.top() != _puzzleState)
	{
		ProfileSystem::Instance->ActiveProfile.L3_S2_M6_SceneState = _puzzleState;
		_stateMachine.push(_puzzleState);
	}

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	switch (_puzzleState)
	{
	case M6_Scene_Enter:
		{
			_vdo_LasiaAttack->PlayFadeInVideo(1);
			Hud->EnableInput(false);
			Hud->HideHud(false);
			_puzzleState = M6_Scene_LasiaAttack;
			Control::Audio->StopSample(aAmbL3s2Maze);
		}
		break;

	case M6_Scene_LasiaAttack:
		{
			Cursor::SetMode(CA_Normal);
			//Subtitles Code
			time = _vdo_LasiaAttack->GetTime();

			if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
			{
				subtitleText->SetText(subtitles[subIndex].text);
			}
			else
				subtitleText->SetText("");

			if (time > subtitles[subIndex].end)
			{
				if (subMax - 1 > subIndex)
					subIndex++;
			}
#ifdef FORCE_CLOSE_VIDEO
			vidTimer += Control::LogicRate;
#endif
			if (
#ifndef FORCE_CLOSE_VID_END
				_vdo_LasiaAttack->isEndReached() ||
#endif
				vidTimer > 14)
			{
				subtitleText->SetText("");
				_vdo_LasiaAttack->StopAndFadeOutVideo(1);
				Hud->AllowInput = true;
				Hud->ShowHud(false);
				HOScene->GetObjectByID("Crystal_Left")->SetEnable(true);
				HOScene->GetObjectByID("Crystal_Right")->SetEnable(true);
				Hud->ShowBannerText(Banner_L3_S2_LasiayAttackComplete);
				Control::Audio->PlaySample(aVOL3MaryRefS2_1);
				Control::Audio->PlaySample(aAmbL3s2Maze,true);
				_puzzleState = M6_Scene_AfterLasiaAttack;

				//Journal Entry Add
				if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P2_LasyaAttack])
				{
					//Flag Set
					ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P2_LasyaAttack] = true;

					//Add to Note Array
					ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L3P2_LasyaAttack;

					ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

					//Increment Notes
					ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
					Hud->ShowJrnEntryFX();
				}
			}

			return;
		}
		break;

	case M6_Scene_AfterLasiaAttack:
		{
			Cursor::SetMode(CA_Normal);

			if (SceneArea.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);

				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L3_S2_MazeOnClickCrystal);
				}
			}
		}
		break;

	case M6_Scene_BrakingCrystal:
		{
			Cursor::SetMode(CA_Normal);
#ifdef FORCE_CLOSE_VIDEO
			vidTimer2 += Control::LogicRate;
#endif
			if (
#ifndef FORCE_CLOSE_VID_END
				_vdo_CrystalBreak->isEndReached() ||
#endif
				vidTimer2 > 11)
			{
				Hud->ShowBannerText("L3slab");
				_vdo_CrystalBreak->StopAndFadeOutVideo(1);
				Hud->AllowInput = true;
				Hud->ShowHud(false);
				_puzzleState = M6_Scene_BreakingCrystalComplete;
				Control::Audio->PlaySample(aAmbL3s2Maze,true);
			}

			return;
		}
		break;

	case M6_Scene_BreakingCrystalComplete:
		{
			HOScene->GetObjectByID("Crystal_Left")->SetEnable(false);
			HOScene->GetObjectByID("Crystal_Right")->SetEnable(false);
			_puzzleState = M6_Puzzle_Start;
		}
		break;

	case M6_Puzzle_Start:
		{
			for (int i = 0; i < (int)(_PillarWheels.size()); i++)
			{
				_PillarWheels[i]->setWheelDisabled(false);
			}

			_puzzleState = M6_Puzzle_Playing;
		}
		break;

	case M6_Puzzle_Playing:
		{
			switch (_enabledBox)
			{
			case NO_BOX:
				{
					CheckCursorMode();

					if (LeftBoxRect.Intersection(&pos) || RightBoxRect.Intersection(&pos))
					{
						if (Control::Input->LBclicked())
						{
							Hud->ShowBannerText(Banner_L3_S2_M6_ClickOnSqueBox);
						}
					}
				}
				break;

			case LEFT_BOX:
			case RIGHT_BOX:
				{
					if (BoxRects[_enabledBox].Intersection(&pos))
					{
						Cursor::SetMode(CA_Look);

						if (Control::Input->LBclicked())
						{
							CollectBox();
						}
					}
					else
						CheckCursorMode();
				}
				break;

				default:
					break;
			}
		}
		break;

	case M6_Puzzle_Onvdo:
		{
			Cursor::SetMode(CA_Normal);
			if(_enabledBox !=NO_BOX) // put this check because sometimes NO_BOX is passed in as a value and that will crash the game.
				if (_moveToBoxVdos[_enabledBox]->isEndReached())
				{
					OnVideoEnd();
					_puzzleState = M6_Puzzle_Onvdo_End;
					Control::Audio->PlaySample(aAmbL3s2Maze,true);
				}

			return;
		}
		break;

	case M6_Puzzle_Onvdo_End:
		{
			if(_enabledBox !=NO_BOX) // put this check because sometimes NO_BOX is passed in as a value and that will crash the game.
				_puzzleState = _stateOnVdoEnd[_enabledBox];
		}
		break;

	case M6_Puzzle_Scene_Left:
		{
			if (LeftBoxScene.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);

				if (Control::Input->LBclicked())
				{
					OnBoxCollect();
				}
			}
			else
			{
				Cursor::SetMode(CA_Normal);
			}

			return;
		}
		break;

	case M6_Puzzle_Scene_Right:
		{
			if (RightBoxScene.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);

				if (Control::Input->LBclicked())
				{
					OnBoxCollect();
				}
			}
			else
			{
				Cursor::SetMode(CA_Normal);
			}

			return;
		}
		break;

	case M6_Puzzle_WaitForCollectInv:
		Cursor::SetMode(CA_Normal);
		break;

	case M6_Puzzle_OnBoxCollected:
		{
			SAFE_DELETE(boxPickScene);

			if (_collectedBoxes.size() == 2)
			{
				_puzzleState = M6_Puzzle_Complete;
				ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPuzzleComplete = true;
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = false;
				ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L3_S2_P1Room1Start] = true;
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S3_Pool] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_FortInterior;
				SendMessage(ControlExit, ID, HOSM_NewArea);		

				for (int i = 0; i < (int)(_PillarWheels.size()); i++)
				{
					_PillarWheels[i]->setWheelDisabled(true);
				}
			}
			else
				_puzzleState = M6_Puzzle_Playing;
		}
		break;

	case M6_Puzzle_Complete:
		{
			Cursor::SetMode(CA_Normal);
		}
		break;

	default:
		_puzzleState++;
		Cursor::SetMode(CA_Normal);
		break;
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
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L3_S2_FortInterior);
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

		return;
	} 
}

void L3_S2_P1Room6Pillar::CheckCursorMode()
{
	for (int i = 0; i < (int)(_PillarWheels.size()); i++)
	{
		if (!_PillarWheels[i]->getCursorOnWheel())
			Cursor::SetMode(CA_Normal);
		else
		{
			Cursor::SetMode(CA_HandPointy);
			break;
		}
	}
}

//void L3_S2_P1Room6Pillar::ProcessMessage(sMessage message)
//{
//	
//}

void L3_S2_P1Room6Pillar::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

}

void L3_S2_P1Room6Pillar::ReceiveMessage(int val)
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
		{
			if (_puzzleState == M6_Scene_AfterLasiaAttack &&
				itemVal == eInv_L3S2_Hammer)
			{
				_vdo_CrystalBreak->PlayFadeInVideo(1);
				Hud->EnableInput(false);
				Hud->HideHud(false);
				_puzzleState = M6_Scene_BrakingCrystal;
				Control::Audio->StopSample(aAmbL3s2Maze);
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{	
		if( boxPickScene )
			boxPickScene->FadeOut(2);
		_puzzleState += 2;
	}	
}

void L3_S2_P1Room6Pillar::TaskPendingUpdate()
{
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPuzzleComplete)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = true;
	else
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = false;
}

//_PUZZLE
void L3_S2_P1Room6Pillar::ProcessSkip()
{

}

float L3_S2_P1Room6Pillar::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L3_S2_P1Room6Pillar::ProcessNonHOHint()
{
	if (_puzzleState >= M6_Puzzle_Start)
	{
		if (PuzzleState(_puzzleState) == M6_Puzzle_Scene_Left 
			|| PuzzleState(_puzzleState) == M6_Puzzle_Scene_Right)
		{
			InitHintTrailEmit(_boxOnScene->Pos.x, _boxOnScene->Pos.y);
			return;
		}

		if (_enabledBox != NO_BOX)
		{
			InitHintTrailEmit(_boxObjects[_enabledBox]->Pos.x, _boxObjects[_enabledBox]->Pos.y);
			return;
		}

		vector<PillerState> pillerPos;

		for (int i = 0; i < 3; i++)
		{
			pillerPos.push_back(_PillarWheels[i]->getCurrentPillerPos());
		}

		if (_puzzleState != M6_Puzzle_Complete)
		{
			PillarWheel *hintWheel = _PillarWheels[PuzzleSolution::hint_GetWheelIndex(pillerPos, _collectedBoxes)];
			InitHintTrailEmit(hintWheel->Pos.x, hintWheel->Pos.y);
		} 
	}
	else
	{
		Hud->ShowHintForInvItem(eInv_L3S2_Hammer, &HammerDropRect);
	}
}

void L3_S2_P1Room6Pillar::SparkleUpdate()
{
	isSparkleUpdatePending = false;

	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if(_puzzleState == M6_Puzzle_Scene_Left)
	{
		sparklePointsVec.push_back(CPoint(LeftBoxScene.GetCenterX(), LeftBoxScene.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
	else if(_puzzleState == M6_Puzzle_Scene_Right)
	{
		sparklePointsVec.push_back(CPoint(RightBoxScene.GetCenterX(), RightBoxScene.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
	else if (_enabledBox != NO_BOX)
	{
		sparklePointsVec.push_back(_boxObjects[_enabledBox]->Pos);
		isSparkleUpdatePending = true;
	}

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}


void L3_S2_P1Room6Pillar::OnWheelStartRotate(CObject *obj)
{
	if (_enabledBox != NO_BOX)
	{
		_boxGlowObjects[_enabledBox]->SetEnable(false);
	}

	Control::Audio->QuickLoadAndPlaySFX("s2m6_pillarmove");
	_enabledBox = NO_BOX;
}

void L3_S2_P1Room6Pillar::OnWheelEndRotate(CObject *obj)
{
	vector<PillerState> pillerPos;

	for (int i = 0; i < 3; i++)
	{
		pillerPos.push_back(_PillarWheels[i]->getCurrentPillerPos());
		ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPositions[i] = (int)_PillarWheels[i]->getCurrentPillerPos();
	}

	_enabledBox = PuzzleSolution::getSolution(pillerPos);

	for (int i = 0; i < (int)(_collectedBoxes.size()); i++)
	{
		if (_enabledBox == _collectedBoxes[i])
			_enabledBox = NO_BOX;
	}

	if (_enabledBox != NO_BOX)
	{
		Hud->ShowBannerText(Banner_L3_S2_M6_PillerSolved);
		Control::Audio->QuickLoadAndPlaySFX("s2m6_pillarsolution");
		_boxGlowObjects[_enabledBox]->SetEnable(true);
		_boxGlowObjects[_enabledBox]->Animate(0.4f, 1.0f, 1.5f);
	}
}

void L3_S2_P1Room6Pillar::CollectBox()
{
	if (_collectedBoxes.size() == 0)
	{
		_overlap->SetEnable(true);
		_moveToBoxVdos[_enabledBox]->PlayFadeInVideo(2.0f);
		Hud->EnableInput(false);
		Hud->HideHud(false);
		_puzzleState = M6_Puzzle_Onvdo;
		Control::Audio->StopSample(aAmbL3s2Maze);
	}
	else
	{
		boxPickScene = new CHOScene();
		boxPickScene->Load(_boxPickSceneFile[_enabledBox]);
		boxPickScene->SetZOff(100);
		boxPickScene->FadeIn(2);
		boxPickScene->GetObjectByID("popfireanim")->SetScale(2.5f);
		boxPickScene->GetObjectByID("vine_Scene")->Animate(0, 5, 0, 0, 5);
		_boxOnScene = boxPickScene->GetObjectByID("BoxToPick");
		_puzzleState = _stateOnVdoEnd[_enabledBox];
	}

	for (int i = 0; i < (int)(_PillarWheels.size()); i++)
	{
		_PillarWheels[i]->setWheelDisabled(true);
	}
}

void L3_S2_P1Room6Pillar::OnVideoEnd()
{
	_overlap->SetEnable(false);
	_moveToBoxVdos[_enabledBox]->StopVideo();
	Hud->AllowInput = true;
	Hud->ShowHud(false);
	boxPickScene = new CHOScene();
	boxPickScene->Load(_boxPickSceneFile[_enabledBox]);
	boxPickScene->SetZOff(100);
	boxPickScene->GetObjectByID("popfireanim")->SetScale(2.5f);
	boxPickScene->GetObjectByID("vine_Scene")->Animate(0, 5, 0, 0, 5);
	_boxOnScene = boxPickScene->GetObjectByID("BoxToPick");
}

void L3_S2_P1Room6Pillar::OnBoxCollect()
{
	_boxObjects[_enabledBox]->SetEnable(false);
	_boxGlowObjects[_enabledBox]->SetEnable(false);
	_boxOnScene->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[_collectedBoxes.size()] = int(_enabledBox);
	Hud->TakeInventoryItem(eInv_L3S2_PuzzleBlock1Left + int(_enabledBox), _boxOnScene->Pos);
	_collectedBoxes.push_back(_enabledBox);
	_enabledBox = NO_BOX;
	_puzzleState = M6_Puzzle_WaitForCollectInv;

	if(_collectedBoxes.size()==2)
	{
		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O2_UnravelMazeMystery] && 
			!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L3O2_UnravelMazeMystery] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L3O2_UnravelMazeMystery] = true;
			Hud->ShowObjectiveFX(eObj_L3O2_UnravelMazeMystery,true);
		}
	}

	for (int i = 0; i < (int)(_PillarWheels.size()); i++)
	{
		_PillarWheels[i]->setWheelDisabled(false);
	}
}

void L3_S2_P1Room6Pillar::ResetPuzzle()
{
	if(_puzzleState < 4)
	{
		ResetBtn->SetEnable(false);
		return;
	}
	ResetBtn->SetEnable(true);
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
			ProfileSystem::Instance->ActiveProfile.L3_S2_M6_SceneState = 5;
			ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPuzzleComplete = false;
			for (int i = 0; i < 3; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPositions[i] = 0;
			}
			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room6Pillar;
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
void L3_S2_P1Room6Pillar::NavigateToScene()
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