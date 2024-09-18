//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 04 : CAVE
//====================================

#include "L3_S1_PZLionHead.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Circle.h"

static int ANGLE_TO_ROTATE = 30;
static float RingRadious[4] = {60, 110, 160, 205};
const CRectangle invItemRectGem(578.0f, 278.0f, 233.0f, 131.0f);

L3_S1_PZLionHead::L3_S1_PZLionHead()
{
	_isPuzzleSolved =  ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadPuzzleCompleted;
	musicFadeOutTimer = 0;
	Visible = false;
	_ringToRoate = NULL;
	rotateTimer = 0;
	startAngle = endAngle = 0;
	//first step to be done to avoid render deadlock

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S1\\P1\\P1.SCN");

	GFInstance->PostUpdate(2);

	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s1LionHead,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s1LionHead) )		
	{
		Control::Audio->PlaySample(aAmbL3s1LionHead,true);
	}
	
	//MUSIC_SFX
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L3_S1_PZLionHead] = true;

	P1LeafsFX = new SuperFX("PRT\\L3\\P1Leafs.INI", PointSystem::CreateCPoint(683,-10,0), 1100); 
	P1LeafsFX->Start();
	P1LeafsFX->AdvanceByTime(2.0f);

	GemParticlesFX = new SuperFX("PRT\\L3\\GemParticles.INI", PointSystem::CreateCPoint(696,341,0), 1100); 

	s1RoundPZCompleteFX = new SuperFX("PRT\\L3\\s1RoundPZComplete.INI", PointSystem::CreateCPoint(684,346,0), 1100); 

	InitRings();

	Visible = true;	//To render anything with renderlistentity
}

void L3_S1_PZLionHead::InitRings()
{
	_ringOpen = HOScene->GetObjectByID("ring_open");
	_ringOpen->SetEnable(false);
	_ringCenter = HOScene->GetObjectByID("ring_1")->Pos;

	_gem = HOScene->GetObjectByID("greengem");
	_gem->SetEnable(false);
	_gemGlow = HOScene->GetObjectByID("greengemglow");
	_gemGlow->SetEnable(false);

	for (int i = 1; i <= 4; i++)
	{
		CObject* ringObj = HOScene->GetObjectByID(std::string("ring_")+MKSTR(i));
		_rings.push_back(ringObj);
		//_ringRadious.push_back(ringObj->)
	}

	// handle saved data,
	if (_isPuzzleSolved)
	{
		for (int i = 0; i < 4; i++)
		{
			_rings[i]->SetEnable(false);
			_ringOpen->SetEnable(true);
			_gem->SetEnable(true);
		}
		_pzState = KState_Complete;
	}
	else 
	{
		Hud->InvBox_StartPuzzleMode();

		if (ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadRingAngles[0] == -1)
		{
			ShuffleRings();
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				_rings[i]->SetRotate(ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadRingAngles[i]);

				if (_rings[i]->GetRotate() == 0)
					_rings[i]->SetTint(0.75f, 2, 0.75f);
			}
		}

		_pzState = KState_Initialize;
	}
}


L3_S1_PZLionHead::~L3_S1_PZLionHead()
{
	if (!_isPuzzleSolved)
		Hud->InvBox_StopPuzzleMode();
	
	_rings.clear();

	SAFE_DELETE(GemParticlesFX);
	SAFE_DELETE(P1LeafsFX);
	SAFE_DELETE(s1RoundPZCompleteFX);
	
	if( !Control::Audio->IsToLevelTrackRoom() )
		if( Control::Audio->IsPlaying(aTrackLevel2) )
			Control::Audio->StopSample(aTrackLevel2);
		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s1LionHead);
	Control::Audio->UnloadSample(aAmbL3s1LionHead);
	//MUSIC_SFX
}

void L3_S1_PZLionHead::Required()
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
			 if( !Control::Audio->IsPlaying(aTrackL3puzzle) )
				  Control::Audio->PlaySample(aTrackL3puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL3puzzle))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL3puzzle, true);
		}
	}

}

float L3_S1_PZLionHead::easing(float t,float b,float c,float d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
}

void L3_S1_PZLionHead::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;
}

void L3_S1_PZLionHead::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
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
		// back from puzzle scene
		if (_isPuzzleSolved)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}	
	}	
}

void L3_S1_PZLionHead::ProcessNonHOHint()
{
	if (_pzState == KState_Complete)
	{
		Hud->InitHintTrailEmit(invItemRectGem.GetCenterX(), invItemRectGem.GetCenterY());
	}
	else
	{
		Hud->ShowNothingToDoMsgBanner();
	}
}

void L3_S1_PZLionHead::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_pzState = KState_Skip;
}

void L3_S1_PZLionHead::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
	} 
	#endif
	if( IsMouseOverGoBackArea(&pos) )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L3_S1_FortEntrance);
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

	switch (_pzState)
	{
	case KState_Initialize:
		{
			_pzState = KState_WaitToStart;
		}
		break;
	
	case KState_WaitToStart:
		{
			// handle anything before start playing
			_pzState = KState_Playing;
		}
		break;

	case KState_Playing:
		{
			for (int i = 0; i < 4; i++)
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

					if(_ringToRoate->GetRotate() != (_ringAngle + angleDiff) && !Control::Audio->IsPlaying("s1pz1_tilerotate"))
						Control::Audio->QuickLoadAndPlaySFX("s1pz1_tilerotate");

					_ringToRoate->SetRotate(_ringAngle + angleDiff);
				}
				else
				{
					float angle = (float)(Elixir::NormalizeAngle(_ringToRoate->GetRotate()));
					_ringToRoate->SetRotate(angle);
					angle = (angle < 5.0f) ? 0 : angle;
					if (angle > 355.0f) {angle = (float)(Elixir::NormalizeAngle((double)(angle), -5, 5));_ringToRoate->SetRotate(angle);angle = 0;};
					float val[5] = {-1, -1, -1, -1, angle};
					_ringToRoate->TweenTo(val, 0.2f, 0, true, CC_CALLBACK_0(L3_S1_PZLionHead::OnRotateComplete, this));
					_pzState = KState_Snapping;
					
					if(angle == 0)
							_ringToRoate->SetTint(0.75f, 2.0f, 0.75f);

					_ringToRoate = NULL;
				}
			}
			else
			{
				for (int i = 0; i < 4; i++)
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

	case KState_Snapping:
		break;

	case KState_Skip:
		{
			StartAutoCompletePuzzle();
			ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadPuzzleCompleted = true;
			_pzState = KState_AutoComplete;
		}
		break;

	case KState_AutoComplete:
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

	case KState_Won:
		{
			Control::Audio->QuickLoadAndPlaySFX("s1pz1_puzzlecomplete");

			s1RoundPZCompleteFX->Start();

			if (_handleExtrasGameComplete())
			{
				_pzState = KState_L3S1CompleteInExtras;
				return;
			}
			_pzState++;
		}
		break;

	case KState_OpenDoor:
		{
			ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadPuzzleCompleted = true;
			_isPuzzleSolved = true;

			for (int i = 0; i < 4; i++)
			{
				_rings[i]->FadeOut(1.0f);
				ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadRingAngles[i] = 0;
			}

			Hud->InvBox_StopPuzzleMode();

			_gem->SetEnable(true);
			_gem->FadeIn();

			_ringOpen->SetEnable(true);
			_ringOpen->FadeIn(1.0f);

			_pzState = KState_Complete;
		}
		break;

	case KState_Complete:
		{
			if (!GemParticlesFX->Active)
			{
				_gemGlow->SetEnable(true);
				_gemGlow->SetBlendMode(eBlend_AlphaAdditive);
				_gemGlow->Animate(0.2f, 0.7f, 4);

				GemParticlesFX->Start();
				GemParticlesFX->AdvanceByTime(2.0f); 
			}

			if (invItemRectGem.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);

				if( Control::Input->LBclicked() )
				{
					_gem->FadeOut(1);
					_gemGlow->FadeOut(1);
					GemParticlesFX->StopEmit();
					ProfileSystem::Instance->ActiveProfile.L3_S1_invGreenEyeCollected = true;
					Hud->TakeInventoryItem(eInv_L3S1_GreenEye);
					_pzState = KState_GemCollected;
				}
			}
		}
		break;

	case KState_GemCollected:
		{
			Cursor::SetMode(CA_Normal);
			// back to previous scene
		}
		break;

	case KState_L3S1CompleteInExtras:
		break;

	default:
		_pzState++;
		break;
	}
}

void L3_S1_PZLionHead::ShuffleRings()
{
	int angleArray[] = {30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
	int ranAngle = 0;

	for (int i = 0; i < 4; i++)
	{
		ranAngle = rand()%11;
		_rings[i]->SetRotate((float)(angleArray[ranAngle]));
		ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadRingAngles[i] = (float)(angleArray[ranAngle]);
	}
}

void L3_S1_PZLionHead::RotateRing(int index)
{
	int direction = (int)(pow(-1, ((index % 2) + 1)));
	float angleToRotate = (float)(ANGLE_TO_ROTATE * direction);

	float currentAngle = _rings[index]->GetRotate();
	float newAngle = currentAngle + angleToRotate;

	rotateTimer = 0;
	_ringToRoate = _rings[index];
	startAngle = currentAngle;
	endAngle = newAngle;

	ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadRingAngles[index] = newAngle;
}

void L3_S1_PZLionHead::OnRotateComplete()
{
	for (int i = 0; i < 4; i++)
	{
		ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadRingAngles[i] = _rings[i]->GetRotate();
	}

	for (int i = 0; i < 4; i++)
	{
		if (_rings[i]->GetRotate() != 0)
		{
			_pzState = KState_Playing;
			return;
		}
	}

	OnPuzzleComplete();
}

void L3_S1_PZLionHead::OnPuzzleComplete()
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	for (int i = 0; i < (int)_rings.size(); i++)
		_rings[i]->SetTint(1, 1, 1);

	if (_pzState != KState_Skip)
		_pzState = KState_Won;
	else
		_pzState = KState_OpenDoor;
}

void L3_S1_PZLionHead::StartAutoCompletePuzzle()
{
	for (int i = 0; i < 4; i++)
	{
		if (_rings[i]->GetRotate() != 0)
		{
			RotateRingToZero(i);
			return;
		}
	}

	OnPuzzleComplete();
}

void L3_S1_PZLionHead::RotateRingToZero(int index)
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
void L3_S1_PZLionHead::NavigateToScene()
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