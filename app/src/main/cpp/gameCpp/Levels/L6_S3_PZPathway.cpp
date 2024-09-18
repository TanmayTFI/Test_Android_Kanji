
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L6_S3_PZPathway.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

static int NoOfRings = 5;
static int ANGLE_TO_ROTATE = 15;
static float RingRadious[] = {110, 160, 205, 250, 300};

using namespace std;

L6_S3_PZPathway::L6_S3_PZPathway()
{
	_isPuzzleSolved =  ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted;

	_ringToRoate = NULL;
	rotateTimer = 0;
	startAngle = endAngle = 0;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S3\\PZ1\\PZ1.SCN");

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

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s3PathwayPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s3PathwayPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL6s3PathwayPuzzleAmb,true);
	}
	
	//MUSIC_SFX
	
	

	InitRings();

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L6_S3_PZPathway]  = true;
}

void L6_S3_PZPathway::InitRings()
{
	_ringCenter = HOScene->GetObjectByID("Disk_1")->Pos;

	for (int i = 1; i <= NoOfRings; i++)
	{
		CObject* ringObj = HOScene->GetObjectByID(std::string("Disk_")+MKSTR(i));
		_rings.push_back(ringObj);
		//_ringRadious.push_back(ringObj->)
	}

	// handle saved data,
	if (_isPuzzleSolved)
	{
		for (int i = 0; i < (int)_rings.size(); i++)
		{
			_rings[i]->SetEnable(false);
		}
		_pzState = L6S3_KState_Complete;
	}
	else 
	{
		Hud->InvBox_StartPuzzleMode();

		if (ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayRingAngles[0] == -1)
		{
			ShuffleRings();
		}
		else
		{
			for (int i = 0; i < (int)_rings.size(); i++)
			{
				_rings[i]->SetRotate((float)ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayRingAngles[i]);

				if (_rings[i]->GetRotate() == 0)
					_rings[i]->SetTint(0.75f, 2, 0.75f);
			}
		}

		_pzState = L6S3_KState_WaitToStart;
	}
}

void L6_S3_PZPathway::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL6theme) )
	{
		Control::Audio->Samples[aTrackL6theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL6theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL6theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL6puzzle) )
				  Control::Audio->PlaySample(aTrackL6puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL6puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL6puzzle, true);
		}
	}
}

void L6_S3_PZPathway::ReceiveMessage(int val)
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
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

L6_S3_PZPathway::~L6_S3_PZPathway()
{
		//MUSIC_SFX
	Hud->InvBox_StopPuzzleMode();

	Control::Audio->StopSample(aAmbL6s3PathwayPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL6s3PathwayPuzzleAmb);

	SAFE_DELETE(ResetDia);
	//MUSIC_SFX
}

void L6_S3_PZPathway::ProcessNonHOHint()
{
	if (_pzState == L6S3_KState_Complete)
	{

	}
	else
	{
		Hud->ShowNothingToDoMsgBanner();
	}
}

void L6_S3_PZPathway::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted = true;
	_pzState = L6S3_KState_Skip;
}

float L6_S3_PZPathway::easing(float t,float b,float c,float d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
}

void L6_S3_PZPathway::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif

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
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FloatingIsland3;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	

	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L6_S3_FloatingIsland3);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L6_S3_FloatingIsland3);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S3_FloatingIsland3, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FloatingIsland3;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	} 

	switch (_pzState)
	{
	case L6S3_KState_WaitToStart:
		{
			// handle anything before start playing
			_pzState = L6S3_KState_Playing;
		}
		break;

	case L6S3_KState_Playing:
		{
			for (int i = 0; i < (int)_rings.size(); i++)
			{
				if(_rings[i]->GetRotate() != 0)
					_rings[i]->SetTint(1, 1, 1);
			}

			if (_ringToRoate != NULL)
			{
				if( Control::Input->LBdown() )
				{
					Cursor::SetMode(CA_HandPointy);

					float angle = atan2(_angleStartPos.y - pos.y, _angleStartPos.x - pos.x);
					angle = 180 - (angle * 180 / PIE);

					float angleDiff = angle - _startAngle;

					if(_ringToRoate->GetRotate() != (_ringAngle + angleDiff) && !Control::Audio->IsPlaying("s3pz2_tilerotate"))
						Control::Audio->QuickLoadAndPlaySFX("s3pz2_tilerotate");

					_ringToRoate->SetRotate(_ringAngle + angleDiff);
				}
				else
				{
					float angle = (float)Elixir::NormalizeAngle(_ringToRoate->GetRotate());
					_ringToRoate->SetRotate(angle);
					angle = angle < 5.0f ? 0 : angle;
					if (angle > 355.0f) {angle = (float)Elixir::NormalizeAngle(angle, -5, 5);_ringToRoate->SetRotate(angle);angle = 0;};
					float val[5] = {-1, -1, -1, -1, angle};
					_ringToRoate->TweenTo(val, 0.2f, 0, true, CC_CALLBACK_0(L6_S3_PZPathway::OnRotateComplete, this));
					_pzState = L6S3_KState_Snapping;
					
					if(angle == 0)
							_ringToRoate->SetTint(0.75f, 2.0f, 0.75f);

					_ringToRoate = NULL;
				}
			}
			else
			{
				for (int i = 0; i < (int)_rings.size(); i++)
				{
					float distance = Elixir::Distance(&_ringCenter, &pos);
					if (distance <= RingRadious[i])
					{
						if(_rings[i]->GetRotate() != 0)
							_rings[i]->SetTint(2, 1.75f, 0.5f);
						
						Cursor::SetMode(CA_HandPointy);

						if( Control::Input->LBdown() )
						{
							_angleStartPos = _ringCenter;
							float angle = atan2(_angleStartPos.y - pos.y, _angleStartPos.x - pos.x);
							angle = 180 - (angle * 180 / PIE);
							_startAngle = angle;
							RotateRing(i);
							_ringAngle = _ringToRoate->GetRotate();
						}

						break;
					}
				}
			} 	
		}
		break;

	case L6S3_KState_Snapping:
		break;

	case L6S3_KState_Skip:
		{
			StartAutoCompletePuzzle();
			_pzState = L6S3_KState_AutoComplete;
		}
		break;

	case L6S3_KState_AutoComplete:
		{
			if (_ringToRoate != NULL)
			{
				rotateTimer += Control::LogicRate * 2;

				float angle = startAngle + (endAngle - startAngle)*easing(rotateTimer, 0, 1, 1);
				_ringToRoate->SetRotate(angle);

				if (rotateTimer >= 1.0f)
				{
					angle = endAngle = 0.0f;
					_ringToRoate->SetRotate(angle);
					_ringToRoate = NULL;
					StartAutoCompletePuzzle();
				}
			}
		}
		break;

	case L6S3_KState_Won:
		{
			Control::Audio->QuickLoadAndPlaySFX("s3pz2_puzzlecomplete");

			if (_handleExtrasGameComplete())
			{
				_pzState = L6S3_KState_ExtrasComplete;
				return;
			}

			Hud->InvBox_StopPuzzleMode();
			_pzState++;
		}
		break;

	case L6S3_KState_Complete:
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FloatingIsland3;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
		break;

	case L6S3_KState_ExtrasComplete:
		break;

	default:
		_pzState++;
		break;
	}
}

void L6_S3_PZPathway::ShuffleRings()
{
	int angleArray[] = {30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
	int ranAngle = 0;

	for (int i = 0; i < (int)_rings.size(); i++)
	{
		ranAngle = rand()%11;
		_rings[i]->SetRotate((float)(angleArray[ranAngle]));
		ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayRingAngles[i] = angleArray[ranAngle];
	}
}

void L6_S3_PZPathway::RotateRing(int index)
{
	int direction = (int)(pow(-1, ((index % 2) + 1)));
	float angleToRotate = (float)(ANGLE_TO_ROTATE * direction);

	float currentAngle = _rings[index]->GetRotate();
	float newAngle = currentAngle + angleToRotate;

	rotateTimer = 0;
	_ringToRoate = _rings[index];
	startAngle = currentAngle;
	endAngle = newAngle;

	ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayRingAngles[index] = (int)newAngle;
}

void L6_S3_PZPathway::OnRotateComplete()
{
	for (int i = 0; i < (int)_rings.size(); i++)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayRingAngles[i] = (int)_rings[i]->GetRotate();
	}

	for (int i = 0; i < (int)_rings.size(); i++)
	{
		if (_rings[i]->GetRotate() != 0)
		{
			_pzState = L6S3_KState_Playing;
			return;
		}
	}

	OnPuzzleComplete();
}

void L6_S3_PZPathway::OnPuzzleComplete()
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	for (int i = 0; i < (int)_rings.size(); i++)
		_rings[i]->SetTint(1, 1, 1);

	ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted = true;

	if (_pzState != L6S3_KState_Skip)
		_pzState = L6S3_KState_Won;
	else
		_pzState = L6S3_KState_Complete;
}

void L6_S3_PZPathway::StartAutoCompletePuzzle()
{
	for (int i = 0; i < (int)_rings.size(); i++)
	{
		if (_rings[i]->GetRotate() != 0)
		{
			RotateRingToZero(i);
			return;
		}
	}

	Hud->InvBox_StopPuzzleMode();
	OnPuzzleComplete();
}

void L6_S3_PZPathway::RotateRingToZero(int index)
{
	float currentAngle = _rings[index]->GetRotate();

	int direction = (int)(pow(-1, ((index % 2) + 1)));
	float angleToRotate = 0;
	
	if (direction == 1)
	{
		angleToRotate = 360 - currentAngle;
	}
	else
	{
		angleToRotate = -currentAngle;
	}

	float newAngle = currentAngle + angleToRotate;

	rotateTimer = 0;
	_ringToRoate = _rings[index];
	startAngle = currentAngle;
	endAngle = newAngle;
}

void L6_S3_PZPathway::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;

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
			ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted = false;
			for (int i = 0; i < 5; i++)
				ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayRingAngles[i] = -1;

			_pzState = L6S3_KState_Playing;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_PZPathway;
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
void L6_S3_PZPathway::NavigateToScene()
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