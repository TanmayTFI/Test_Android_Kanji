//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 03 : STONEBEAMPUZZLE
//====================================

#include "L2_S3_PZStoneBeam.h"
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

const CRectangle  rectArrow0(237.0f,479.0f,30.0f,79.0f);
const CRectangle  rectArrow1(276.0f,339.0f,26.0f,59.0f);
const CRectangle  rectArrow2(362.0f,203.0f,27.0f,58.0f);
const CRectangle  rectArrow3(254.0f,105.0f,23.0f,63.0f);
const CRectangle  rectArrow4(523.0f,103.0f,23.0f,63.0f);
const CRectangle  rectArrow5(835.0f,102.0f,27.0f,65.0f);
const CRectangle  rectArrow6(1098.0f,104.0f,27.0f,67.0f);
const CRectangle  rectArrow7(987.0f,193.0f,29.0f,68.0f);
const CRectangle  rectArrow8(1069.0f,324.0f,30.0f,69.0f);
const CRectangle  rectArrow9(1092.0f,483.0f,29.0f,64.0f);

const CRectangle  centerWheelRect(566.0f,234.0f,245.0f,229.0f);




L2_S3_PZStoneBeam::L2_S3_PZStoneBeam()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S3\\P1\\P1.SCN");

	musicFadeOutTimer = 0;

	GFInstance->PostUpdate(2);

	HOScene->GetObjectByID("bottomglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("bottomglow")->Animate(0.45f, 0.95f, 4.0f);

	InitPuzzle();
	InitArrowArray();

	isCenterWheelActivated = false;

	isCenterWheelShaking = false;
	ShakeTimer = ShakeRotValue =0;

	s3smokeFX = new SuperFX("PRT\\L2\\s3smoke.INI", PointSystem::CreateCPoint(738,657,0), 1100); 
	s3smokeFX->Start();
	s3smokeFX->AdvanceByTime(2.0f);

	s3fliesFX = new SuperFX("PRT\\L2\\s3flies.INI", PointSystem::CreateCPoint(675,287,0), 1100); 
	s3fliesFX->Start();
	s3fliesFX->AdvanceByTime(2.0f);

	if(!ProfileSystem::Instance->ActiveProfile.L2_S3_isStoneBeamPuzzleDataSaved)
	{
		ShuffleCircles();
		fillSaveData();
		ProfileSystem::Instance->ActiveProfile.L2_S3_isStoneBeamPuzzleDataSaved =  true;
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			cirPieces[i]->originalPos		= ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleOriginalPos[i];
			cirPieces[i]->currentPosition	= ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleCurrentPos[i];
			cirPieces[i]->cir->Pos.x		= ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleXPos[i];
			cirPieces[i]->cir->Pos.y		= ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleYPos[i];
			cirPieces[i]->cir->SetRotate((float)ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleRotation[i]);
		}
	}

	GFInstance->PostUpdate(2);

	Control::Audio->LoadSample(aAmbs3CaveMouth,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs3CaveMouth) )		
	{
		Control::Audio->PlaySample(aAmbs3CaveMouth,true);
	}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L2_S3_PZStoneBeam]  = true;

	glow1 = HOScene->GetObjectByID("glow1");
	glow1->SetBlendMode(eBlend_AlphaAdditive);
	glow1->Show(false);

	glow2 = HOScene->GetObjectByID("glow2");
	glow2->SetBlendMode(eBlend_AlphaAdditive);
	glow2->Show(false);

	isRotating	= false;
	isSwaping	= false;

	startRotVal = 0;
	endRotVal = 0;
	tweenTime = 0;
	rotTime = 0.35f;
	swapTime = 0.50f;

	rotPieceId = -1;
	centerWheelRedLit = HOScene->GetObjectByID("centerwheel");

	isCenterWheelRotating = false;

	checkCirclePiecesInProperPlace();

	Hud->InvBox_StartPuzzleMode();

	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

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

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;
}

void L2_S3_PZStoneBeam::InitArrowArray()
{
	arrArrows[0] = rectArrow0;
	arrArrows[1] = rectArrow1;
	arrArrows[2] = rectArrow2;
	arrArrows[3] = rectArrow3;
	arrArrows[4] = rectArrow4;
	arrArrows[5] = rectArrow5;
	arrArrows[6] = rectArrow6;
	arrArrows[7] = rectArrow7;
	arrArrows[8] = rectArrow8;
	arrArrows[9] = rectArrow9;

	for (int i = 0; i < 10; i++)
	{

		//Creating glow arrows for mouse over effect. Add all objects to an array
		CObject *arrowGlow = HOScene->GetObjectByID(std::string("arrowGlow" + MKSTR(i)).c_str());
		arrowGlow->SetBlendMode(eBlend_AlphaAdditive);
		arrowGlowArray[i] = arrowGlow;
		arrowGlowArray[i]->Show(false);
	}
}
void L2_S3_PZStoneBeam::showGlowArrow(int index)
{
	arrowGlowArray[index]->Show(true);
}

void L2_S3_PZStoneBeam::hideGlowArrow()
{
	for (int i = 0; i < 10; i++)
	{
		arrowGlowArray[i]->Show(false);
	}
}

void L2_S3_PZStoneBeam::rotateBigCircle(float easeTime)
{
	float angle;
	for (int i = 0; i < 10; i++)
	{
		angle = cirPieces[i]->rotationStartVal + (cirPieces[i]->rotationEndVal - cirPieces[i]->rotationStartVal) * easeTime;
		cirPieces[i]->cir->SetRotate(angle);

		//K_LOG ("inside rotateBigCircle function rotation en value %d",cirPieces[i]->rotationEndVal);
	}

	angle = centerWheelStartPos + (centerWheelEndPos - centerWheelStartPos) * easeTime;
	centerWheelRedLit->SetRotate(angle);
}

void L2_S3_PZStoneBeam::checkRotationStatus()
{
	for (int i = 0; i < 10; i++)
	{
		if(cirPieces[i]->cir->GetRotate() != 0)
		{
			redGlow[cirPieces[i]->currentPosition]->Show(true);
		}
	}
}

void L2_S3_PZStoneBeam::clearRedGlow()
{
	for (int i = 0; i < 10; i++)
	{
		redGlow[i]->Show(false);
	}
}

void L2_S3_PZStoneBeam::checkCirclePiecesInProperPlace()
{
	bool isAllInProperPosition = true;
	for (int i = 0; i < 10; i++)
	{
		correctFixGlow[i]->Show(false);
		if(cirPieces[i]->currentPosition == cirPieces[i]->originalPos)
		{
			if(cirPieces[i]->cir->GetRotate()==0)
			{
				correctFixGlow[i]->Show(true);
				patternSuccessGlow[cirPieces[i]->currentPosition]->Show(true);
			}
		}
		else
		{
			patternSuccessGlow[cirPieces[i]->currentPosition]->Show(false);
			isAllInProperPosition = false;
		}
	}

	isPuzzleSolved();

	/*if(isAllInProperPosition)
	{
		centerWheelGreenLit->Show(false);
		centerWheelRedLit->Show(true);
		isCenterWheelEnabled = true;
	}
	else
	{
		centerWheelRedLit->Show(false);
		centerWheelGreenLit->Show(true);
		isCenterWheelEnabled = false;
	}*/
}

bool L2_S3_PZStoneBeam::isPuzzleSolved()
{

	for (int i = 0; i < 10; i++)
	{
		if (int(cirPieces[i]->cir->GetRotate()) != 0)
		{
			return false;
		}
		if(cirPieces[i]->currentPosition != cirPieces[i]->originalPos)
		{
			return false;
		}
	}

	centerWheelRedLit->FadeOut(2.0f);
	centerWheelGreenLit->FadeIn(4.0f);

	isCenterWheelActivated = true;
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Control::Audio->PlaySample(aSFXMagic2);

	if(_handleExtrasGameComplete())
		return true;

	Hud->ShowBannerText("L2finally");
	return true;
}

void L2_S3_PZStoneBeam::rotateReverseBigCircle(float easeTime)
{
	for (int i = 0; i < 10; i++)
	{
		float angle = cirPieces[i]->rotationStartVal + (cirPieces[i]->rotationEndVal - cirPieces[i]->rotationStartVal) * easeTime;
		cirPieces[i]->cir->SetRotate(angle);
	}
}

void L2_S3_PZStoneBeam::InitPuzzle()
{
	int arrValue[3] = { 60, 120, 150 };
	for (int i = 0; i < 10; i++)
	{
		CObject *cir = HOScene->GetObjectByID(std::string("P" + MKSTR(i)).c_str());

		SmallCircle *obj = new SmallCircle(cir);
		obj->currentPosition = i;
		obj->originalPos = i;
		cirPieces[i] = obj;
		int rndValue = rand() % 3;//Puzzle Difficulty
		if(rndValue <= 1) rndValue = 1;

		obj->cir->SetRotate(float(arrValue[rndValue]));

		ArrowPoly[i] = new CPolygon(4);

		//adding pattern success glow sprites to array
		CObject *temp = HOScene->GetObjectByID(std::string("patternSuccessGlow" + MKSTR(i)).c_str());
		patternSuccessGlow[i] = temp;
		temp->Show(false);

		correctFixGlow[i] = HOScene->GetObjectByID(std::string("FixGlow" + MKSTR(i)).c_str());
		correctFixGlow[i]->SetBlendMode(eBlend_AlphaAdditive);
		correctFixGlow[i]->Show(false);

		temp = HOScene->GetObjectByID(std::string("redGlow" + MKSTR(i)).c_str());
		redGlow[i] = temp;
		temp->Show(false);
		isPuzzleOver = false;
	}

	centerWheelGreenLit = HOScene->GetObjectByID("centerWheelGreenLit");
	centerWheelGreenLit->Show(false);

	int arr0[] = {255,479,301,436,325,458,279,500};
	ArrowPoly[0]->Init(arr0);
	int arr1[] = {290,328,333,289,355,309,311,346};
	ArrowPoly[1]->Init(arr1);
	int arr2[] = {368,193,410,147,433,176,389,212};
	ArrowPoly[2]->Init(arr2);
	int arr3[] = {303,59,323,74,282,122,253,102};
	ArrowPoly[3]->Init(arr3);
	int arr4[] = {520,99,558,54,586,76,548,128};
	ArrowPoly[4]->Init(arr4);
	int arr5[] = {795,79,826,58,866,108,832,130};
	ArrowPoly[5]->Init(arr5);
	int arr6[] = {1055,76,1082,55,1132,112,1097,129};
	ArrowPoly[6]->Init(arr6);
	int arr7[] = {971,150,1014,203,984,221,941,167};
	ArrowPoly[7]->Init(arr7);
	int arr8[] = {1027,304,1056,283,1103,347,1065,361};
	ArrowPoly[8]->Init(arr8);
	int arr9[] = {1053,460,1080,439,1120,494,1089,513};
	ArrowPoly[9]->Init(arr9);

}

void L2_S3_PZStoneBeam::ShuffleCircles()
{
	for (int i = 0; i < 10;)
	{
		int pos = int(rand()) % 9;
		if(pos == i) continue;
		if(cirPieces[pos]->originalPos == i) continue;
		SmallCircle *cir2 = cirPieces[pos];
		cirPieces[i]->swap2(cir2);
		i++;
	}
}

void L2_S3_PZStoneBeam::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(isSwaping)
	{
		tweenTime += Control::LogicRate;

		CPoint temp;
		CPoint temp2;
		temp.x = swapCircle1StartPos.x + (swapCircle1EndPos.x - swapCircle1StartPos.x) * easing(tweenTime,0,1,swapTime);
		temp.y = swapCircle1StartPos.y + (swapCircle1EndPos.y - swapCircle1StartPos.y) * easing(tweenTime,0,1,swapTime);

		cirPieces[swapPieceId]->cir->Pos = temp;

		temp2.x = swapCircle2StartPos.x + (swapCircle2EndPos.x - swapCircle2StartPos.x) * easing(tweenTime,0,1,swapTime);
		temp2.y = swapCircle2StartPos.y + (swapCircle2EndPos.y - swapCircle2StartPos.y) * easing(tweenTime,0,1,swapTime);

		swapingCircle->cir->Pos = temp2;

		if( tweenTime >= swapTime )
		{
			cirPieces[swapPieceId]->cir->Pos = swapCircle1EndPos;
			swapingCircle->cir->Pos =  swapCircle2EndPos;
			tweenTime = 0;
			isSwaping = false;
			clearIsSelecteFlag();

			checkCirclePiecesInProperPlace();
			fillSaveData();
		}
		return;
	}

	if( Control::Audio->IsPlaying(aTrackL2theme) )
	{
		Control::Audio->Samples[aTrackL2theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL2theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL2theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL2puzzle) )
				  Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL2theme1) )
	{
		Control::Audio->Samples[aTrackL2theme1]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL2theme1]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL2theme1);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL2puzzle) )
				  Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
	}

	if(isCenterWheelRotating)
	{
		tweenTime += Control::LogicRate;
		HOScene->update(Control::LogicRate);
		centerWheelGreenLit->SetRotate(-tweenTime*140.0f);
		if(tweenTime > 2.5f)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_CaveMouth;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
		return;
	}
}

void L2_S3_PZStoneBeam::clearIsSelecteFlag()
{
	for (int i = 0; i < 10; i++)
	{
		cirPieces[i]->isSelected = false;
	}
	glow1->Show(false);
	glow2->Show(false);
}

SmallCircle* L2_S3_PZStoneBeam::getSwapableCircle(SmallCircle *obj)
{
	for (int i = 0; i < 10; i++)
	{
		if(cirPieces[i] != obj && cirPieces[i]->isSelected == true)
		{
			return cirPieces[i];
		}
	}

	return nullptr;
}

void L2_S3_PZStoneBeam::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup )
		return;

	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

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


	if(isCenterWheelRotating)
	{
		return;
	}

	if(isCenterWheelActivated)
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;
		if(centerWheelRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Hud->InvBox_StopPuzzleMode();

				Control::Audio->QuickLoadAndPlaySFX("s3pz1_puzzlecomplete");

				ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated = true;

				HOScene->Shake(2, 2, 2.5f);

				isCenterWheelRotating = true;
				tweenTime = 0;
			}
		}
		return;
	}
	else
	{
		if(centerWheelRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Help);
			if(Control::Input->LBclicked())
			{
				if(!isCenterWheelShaking)
				{
					Control::Audio->QuickLoadAndPlaySFX("s1ho_mainboxdooropen");
					ShakeTimer = 0;
					isCenterWheelShaking = true;
					Hud->ShowBannerText("L2active");
				}
			} 
		}
	}
	
	if(isCenterWheelShaking)
	{
		ShakeTimer += Control::LogicRate;

		centerWheelRedLit->SetRotate(ShakeRotValue+Elixir::Sin(ShakeTimer*450)*8.0f);
		if(ShakeTimer>0.8f)
		{
			isCenterWheelShaking = false;
			centerWheelRedLit->SetRotate(ShakeRotValue);
		}
	}

	if(isSwaping)
	{
		return;
	}

	if(isRotating)
	{
		tweenTime += Control::LogicRate;

		float angle = startRotVal + (endRotVal - startRotVal)*easing(tweenTime,0,1,rotTime);
		cirPieces[rotPieceId]->cir->SetRotate(angle);

		if( tweenTime>=rotTime )
		{
			tweenTime = 0;
			isRotating = false;

			cirPieces[rotPieceId]->cir->SetRotate(endRotVal);

			if(cirPieces[rotPieceId]->cir->GetRotate() >= 360)
			{
				cirPieces[rotPieceId]->cir->SetRotate(cirPieces[rotPieceId]->cir->GetRotate() - 360);
			}

			correctFixGlow[rotPieceId]->Show(false);
			patternSuccessGlow[rotPieceId]->Show(false);
			if(cirPieces[rotPieceId]->currentPosition == cirPieces[rotPieceId]->originalPos && cirPieces[rotPieceId]->cir->GetRotate()==0)
			{
				correctFixGlow[rotPieceId]->Show(true);
				patternSuccessGlow[rotPieceId]->Show(true);

				Control::Audio->QuickLoadAndPlaySFX("s3pz1_stonecorrect");
			}

			isPuzzleSolved();

			fillSaveData();
		}
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		if( Elixir::Distance(&pos,&cirPieces[cirPieces[i]->currentPosition]->cirArea->center) < 44 )
		{
			//K_LOG ("mouse over '%d'", i);
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				swapingCircle = getSwapableCircle(cirPieces[i]);
				
				if( swapingCircle != nullptr)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3pz1_stoneswap");

					swapCircle1StartPos = cirPieces[i]->cir->Pos;
					swapCircle1EndPos = swapingCircle->cir->Pos;

					swapCircle2StartPos = swapingCircle->cir->Pos;
					swapCircle2EndPos = cirPieces[i]->cir->Pos;

					cirPieces[i]->swap(swapingCircle);

					glow2->Show(true);
					glow2->SetPos(cirPieces[i]->cir->GetPosPointer());

					isSwaping = true;
					swapPieceId = i;
					break;
				}
				else
				{
					if(cirPieces[i]->isSelected == true)
					{
						glow1->Show(false);
						cirPieces[i]->isSelected = false;
						//K_LOG ("isSelected --> false");
					}
					else
					{
						Control::Audio->QuickLoadAndPlaySFX("s3pz1_stoneselect");
						glow1->Show(true);
						glow1->SetPos(cirPieces[i]->cir->GetPosPointer());
						cirPieces[i]->isSelected = true;
						//K_LOG ("isSelected --> true");
					}
				}
			}
		}
	}

	//Checking arrows clicked or not, if clicked rotate
	for (int i = 0; i < 10; i++)
	{
		if(ArrowPoly[cirPieces[i]->currentPosition]->Intersection(&pos))
		{
			showGlowArrow(cirPieces[i]->currentPosition);

			Cursor::SetMode(CA_HandPointy);

			if(Control::Input->LBclicked())
			{
				clearIsSelecteFlag();
				isRotating = true;
				startRotVal = cirPieces[i]->cir->GetRotate();

				Control::Audio->QuickLoadAndPlaySFX("s3pz1_stonerotate");

				int rotationDegree;
				int mod = cirPieces[i]->currentPosition % 2;
				//K_LOG ("position %d modulo %d",cirPieces[i]->currentPosition,mod);
				if(mod)
				{
					rotationDegree = 150;
				}
				else
				{
					rotationDegree = 60;
				}

				endRotVal = cirPieces[i]->cir->GetRotate() + rotationDegree;
				rotPieceId = i;
			}
			break;
		}
		else
		{
			hideGlowArrow();
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_CaveMouth;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}
	#endif
	if( IsMouseOverGoBackArea(&pos) )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L2_S3_CaveMouth);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L2_S3_CaveMouth);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S3_CaveMouth, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_CaveMouth;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
}

void L2_S3_PZStoneBeam::fillSaveData()
{
	for (int i = 0; i < 10; i++)
	{
		ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleRotation[i] = (int)cirPieces[i]->cir->GetRotate();
		ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleXPos[i] = cirPieces[i]->cir->Pos.x;
		ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleYPos[i] = cirPieces[i]->cir->Pos.y;
		ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleCurrentPos[i] = cirPieces[i]->currentPosition;
		ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleOriginalPos[i] = cirPieces[i]->originalPos;
	}
}

void L2_S3_PZStoneBeam::ProcessSkip()
{
	if(isCenterWheelRotating)
		return;

	if(isSwaping)
	{
		cirPieces[swapPieceId]->cir->Pos = swapCircle1EndPos;
		swapingCircle->cir->Pos =  swapCircle2EndPos;
		isSwaping = false;
	}

	for (int i = 0; i < 10; i++)
	{
		//cirPieces[i] // sree
		SmallCircle *cir1 = cirPieces[ cirPieces[i]->originalPos];
		SmallCircle *cir2 = cirPieces[ cirPieces[i]->currentPosition];
		cir1->swap2(cir2);

		cir1->cir->SetRotate(0);
		cir2->cir->SetRotate(0);
		//i++;
	}

	clearIsSelecteFlag();
	checkCirclePiecesInProperPlace();

	for (int i = 0; i < 10; i++)
	{
		patternSuccessGlow[i]->Show(true); //force show all
	}

	centerWheelRedLit->Show(false);
	centerWheelGreenLit->Show(true);

	Control::Audio->QuickLoadAndPlaySFX("s3pz1_puzzlecomplete");

	isCenterWheelRotating = true;
	tweenTime = 0;
	isCenterWheelActivated = true;

	Hud->InvBox_StopPuzzleMode();
	
	ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated = true;
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
}


float L2_S3_PZStoneBeam::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2_S3_PZStoneBeam::~L2_S3_PZStoneBeam()
{
	Hud->InvBox_StopPuzzleMode();
	
	for (int i = 0; i < 10; i++)
	{
		SAFE_DELETE(ArrowPoly[i]);
		SAFE_DELETE(cirPieces[i]);
	}

	SAFE_DELETE(s3smokeFX);
	SAFE_DELETE(s3fliesFX);

	SAFE_DELETE(ResetDia);

	//MUSIC_SFX

	

	Control::Audio->StopSample(aAmbs3CaveMouth);
	Control::Audio->UnloadSample(aAmbs3CaveMouth);
	//MUSIC_SFX
}

void L2_S3_PZStoneBeam::ReceiveMessage(int val)
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
			
			
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L2_S3_PZStoneBeam::ProcessNonHOHint()
{
}

//START: SmallCircle class implementation
SmallCircle::SmallCircle()
{
	cirArea = NULL;
}

SmallCircle::SmallCircle(CObject *obj)
{
	cir = obj;
	cirArea = new CCircle(cir->Pos.x,cir->Pos.y,43);
}

SmallCircle::~SmallCircle()
{
	if(cirArea!=NULL)
		delete cirArea;
}

void SmallCircle::swap(SmallCircle *obj)
{
	//K_LOG ("Inside swaping function..");
	
	int tempPosition = currentPosition;
	currentPosition = obj->currentPosition;
	obj->currentPosition = tempPosition;
}

void SmallCircle::swap2(SmallCircle *obj) // using for initial swaping, without easing
{
	//K_LOG ("Inside swaping function..");
	//swaping the position and currentPosition variable value
	CPoint pos = cir->Pos;
	cir->Pos = obj->cir->Pos;
	obj->cir->Pos = pos;

	int tempPosition = currentPosition;
	currentPosition = obj->currentPosition;
	obj->currentPosition = tempPosition;
}

bool SmallCircle::isInPlace()
{
	if(currentPosition == originalPos)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}


//END: SmallCircle class implementation

void L2_S3_PZStoneBeam::ResetPuzzle()
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
			ProfileSystem::Instance->ActiveProfile.L2_S3_isStoneBeamPuzzleDataSaved = false;
			for (int i = 0; i < 10; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleRotation[i] = 0;
				ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleXPos[i] = 0.0f;
				ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleYPos[i] = 0.0f;
				ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleCurrentPos[i] = 0;
				ProfileSystem::Instance->ActiveProfile.L2_S3_PZStoneBeamCircleOriginalPos[i] = 0;
			}
			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_PZStoneBeam;
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

void L2_S3_PZStoneBeam::NavigateToScene()
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