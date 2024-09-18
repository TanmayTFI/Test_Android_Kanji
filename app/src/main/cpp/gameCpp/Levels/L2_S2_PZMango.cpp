 //====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 02 : MANGOPUZZLE
//====================================

#include "L2_S2_PZMango.h"
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


const CRectangle  mango2Rect(719.0f,82.0f,117.0f,126.0f);
const CRectangle  mango3Rect(926.0f,288.0f,128.0f,133.0f);
const CRectangle  mango1Rect(243.0f,281.0f,169.0f,166.0f);

const CRectangle  Vid1BgRectLoop(0,0,1366,768);

L2_S2_PZMango::L2_S2_PZMango()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S2\\P1\\P1.SCN");

	musicFadeOutTimer = 0;

	CAnim_StoneThrow = NULL;
	CAnim_StoneThrowComplete = NULL;
	LeafFallAnim = NULL;
	mangoEFX = NULL;

	if(ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced)
		Hud->InvBox_StartPuzzleMode();
	isMangoHitAnimPlaying = false;
	isLeafFallAnimating = false;
	isHandAnimation = false;
	GFInstance->PostUpdate(2);

	InitPuzzle();
	
	GFInstance->PostUpdate(2);

	Control::Audio->LoadSample(aAmbs2MangoPuzzle,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs2MangoPuzzle) )		
	{
		Control::Audio->PlaySample(aAmbs2MangoPuzzle,true);
	}
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L2_S2_PZMango]  = true;


	isRotating = false;
	tweenTime = 0;
}

void L2_S2_PZMango::AnimateLines(CObject *mango)
{
	
	hLine->Pos.x = mango->Pos.x - 150;
	hLine->Pos.y =  mango->Pos.y;

	vLine->Pos.x = mango->Pos.x;
	vLine->Pos.y = mango->Pos.y - 150;

	hLine->Show(true);
	vLine->Show(true);
}

void L2_S2_PZMango::showStoneThrowAnim(int num)
{
	if(num == 1)
	{
		if(CAnim_StoneThrow->IsAnimFinished())
		{
			CAnim_StoneThrowComplete->PlayAnim();
			isHandAnimation = true;
			HOScene->GetObjectByID("hand")->SetEnable(true);
			HOScene->GetObjectByID("handwithstone")->SetEnable(false);
			if(isStoneStrew == false)
			{
				ThrowMango(mango1,isHit);
				isStoneStrew = true;
			}

			throwStoneAnimIndex ++;
			
			if (throwStoneAnimIndex > 2)
			{
				isStoneStrew = false;
				isThrowStoneHandAnimation = false;
				throwStoneAnimIndex = 0;
				return;
			}
			/*TM1_Anim[throwStoneAnimIndex]->Show(true);
			TM1_Anim[throwStoneAnimIndex]->ForcePlayAnimation(0);*/
		}
		return;
	}
	else if(num == 2)
	{
		if(CAnim_StoneThrow->IsAnimFinished())
		{
			CAnim_StoneThrowComplete->PlayAnim();
			isHandAnimation = true;
			HOScene->GetObjectByID("hand")->SetEnable(true);
			HOScene->GetObjectByID("handwithstone")->SetEnable(false);
			if(isStoneStrew == false)
			{
				ThrowMango(mango2,isHit);
				isStoneStrew = true;
			}
			
			throwStoneAnimIndex ++;
			
			if (throwStoneAnimIndex > 2)
			{
				isStoneStrew = false;
				isThrowStoneHandAnimation = false;
				throwStoneAnimIndex = 0;
				return;
			}
			
		}
		return;
	}
	else if(num == 3)
	{
		if(CAnim_StoneThrow->IsAnimFinished())
		{
			CAnim_StoneThrowComplete->PlayAnim();
			isHandAnimation = true;
			HOScene->GetObjectByID("hand")->SetEnable(true);
			HOScene->GetObjectByID("handwithstone")->SetEnable(false);
			if(isStoneStrew == false)
			{
				ThrowMango(mango3,isHit);
				isStoneStrew = true;
			}
			
			throwStoneAnimIndex ++;
			
			if (throwStoneAnimIndex > 2)
			{
				isStoneStrew = false;
				isThrowStoneHandAnimation = false;
				throwStoneAnimIndex = 0;
				return;
			}
			
		}
		return;
	}
}

void L2_S2_PZMango::InitPuzzle()
{
	isHandReadyTimerOn = false;
	isPuzzleSolved = false;

	mango1 = HOScene->GetObjectByID("mango1");
	mango2 = HOScene->GetObjectByID("mango2");
	mango3 = HOScene->GetObjectByID("mango3");

	stone = HOScene->GetObjectByID("stone");
	//handWithoutStone = HOScene->GetObjectByID(std::string("hand"));
	//handReady = HOScene->GetObjectByID("HandReadyAnim");

	CObject* objArr[] = { HOScene->GetObjectByID("handwithstone")};
	std::string animArr[] = {"ANIM\\L2\\S2P1HandStone.anim"};
	CAnim_StoneThrow = new CustomAnimGroup(1, animArr, objArr);

	CObject* objArr1[] = {  HOScene->GetObjectByID("hand") };
	std::string animArr1[] = { "ANIM\\L2\\S2P1Hand.anim"};
	CAnim_StoneThrowComplete = new CustomAnimGroup(1, animArr1, objArr1);
	
	 HOScene->GetObjectByID("hand")->SetEnable(false);


	 HOScene->GetObjectByID("Leaf1")->SetPivot(110, -136);
	 HOScene->GetObjectByID("Leaf1")->Animate(0,0,1,0,7.0f);
	 HOScene->GetObjectByID("Leaf2")->SetPivot(110, -136);
	 HOScene->GetObjectByID("Leaf2")->Animate(0,0,1,0,7.0f);
	 HOScene->GetObjectByID("Leaf3")->SetPivot(110, -136);
	 HOScene->GetObjectByID("Leaf3")->Animate(0,0,1,0,7.0f);
	 HOScene->GetObjectByID("leaf4")->SetPivot(110, -136);
	 HOScene->GetObjectByID("leaf4")->Animate(0,0,1,0,7.0f);
	 HOScene->GetObjectByID("leaf5")->SetPivot(110, -136);
	 HOScene->GetObjectByID("leaf5")->Animate(0,0,1,0,7.0f);
	 HOScene->GetObjectByID("leaf6")->SetPivot(110, -136);
	 HOScene->GetObjectByID("leaf6")->Animate(0,0,1,0,7.0f);
	 HOScene->GetObjectByID("leaf7")->SetPivot(110, -136);
	 HOScene->GetObjectByID("leaf7")->Animate(0,0,1,0,7.0f);

	 HOScene->GetObjectByID("branch1")->SetPivot(110, -136);
	 HOScene->GetObjectByID("branch1")->Animate(0,0,1,0,7.0f);
	 HOScene->GetObjectByID("branch2")->SetPivot(110, -136);
	 HOScene->GetObjectByID("branch2")->Animate(0,0,1,0,7.0f);
	 HOScene->GetObjectByID("branch3")->SetPivot(110, -136);
	 HOScene->GetObjectByID("branch3")->Animate(0,0,1,0,7.0f);
	 HOScene->GetObjectByID("branch4")->SetPivot(110, -136);
	 HOScene->GetObjectByID("branch4")->Animate(0,0,1,0,7.0f);
	

	CollectableMango[0] = HOScene->GetObjectByID("CollectableMango1");
	CollectableMango[1] = HOScene->GetObjectByID("CollectableMango2");
	CollectableMango[2] = HOScene->GetObjectByID("CollectableMango3");

	CollectableMangoBG[0] = HOScene->GetObjectByID("CollectableMangoBG1");
	CollectableMangoBG[1] = HOScene->GetObjectByID("CollectableMangoBG2");
	CollectableMangoBG[2] = HOScene->GetObjectByID("CollectableMangoBG3");

	collectedMangoCount = 3;
	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PZMango1Taken)
	{
		collectedMangoCount--;
	}
	else
	{
		mango1->Show(false);
		mango1 = NULL;
	}
	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PZMango2Taken)
	{
		collectedMangoCount--;
	}
	else
	{
		mango2->Show(false);
		mango2 = NULL;
	}
	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PZMango3Taken)
	{
		collectedMangoCount--;
	}
	else
	{
		mango3->Show(false);
		mango3 = NULL;
	}
	//K_LOG ("--------------------collectedMangoCount -------- %d ",collectedMangoCount);

	for (int i = 2 ; i >= collectedMangoCount; i--)
	{
		CollectableMango[i]->Show(false);
	}

	hLine = HOScene->GetObjectByID("HorizontalLine");
	vLine = HOScene->GetObjectByID("VerticalLine");
	hLine->Show(false);
	vLine->Show(false);
	stone->Show(false);
	//mango1->Show(false);
	//handWithoutStone->Show(false);

	isLineAnimating = false;
	isStoneThrowing = false;
	isMangoDroping = false;

	isStoneStrew = false;

	yValue = 2;
	xValue = 2;

	swapTime = 0.60f;
	rotTime = 0.20f;
	dropTime =  0.60f;

	stoneOriginalX = 1050.0f;
	stoneOriginalY = 551.0f;

	//physics
	vX = 0;
	vY = 0;
	force = 35.0f;
	gravity = 0.5f;
	throwStoneAnimIndex = 0;
	isThrowStoneHandAnimation = false;

	for (int i = 0; i < 3; i++)
	{
		CollectableMango[i]->SetScale(0.75f);
		CollectableMangoBG[i]->SetScale(0.75f);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced)
	{
		 HOScene->GetObjectByID("handwithstone")->SetEnable(false);
		 for (int i = 0; i < 3; i++)
		 {
			CollectableMango[i]->Show(false);
			CollectableMangoBG[i]->Show(false);
		 }
	}
}

void L2_S2_PZMango::ThrowMango(CObject *mango,bool isHit)
{
	stone->Show(true);
	stone->Pos.x = stoneOriginalX;
	stone->Pos.y = stoneOriginalY;
	rotationValue = 0;
	isStoneThrowing = true;
	
	if(isHit)
	{
		isLineAnimating = false;
		hLine->Show(false);
		vLine->Show(false);
		dX = mango->Pos.x - stoneOriginalX;
		if(mango == mango1)
		{
			dY = mango->Pos.y - stoneOriginalY - 130;
		}
		else
		{
			dY = mango->Pos.y - stoneOriginalY - 50;
		}
	}
	else
	{
		dX = mango->Pos.x - stoneOriginalX;
		dY = mango->Pos.y - stoneOriginalY + 100;
	}

	scaleFactor = 1;
	vX = 0;
	vY = 0;
	angle = atan2(dY,dX);

	aX = cos(angle) * force;
	aY = sin(angle) * force;
	vX += aX;
	vY += aY;

	stoneRotationStartValue = stone->GetRotate();
	stoneRotationEndValue = stone->GetRotate() + 360;
}

void L2_S2_PZMango::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

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
}

void L2_S2_PZMango::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	case eInv_L2S2_Mangoes:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S2_Mangoes] = true;
			ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken = true;			
			vecInvIds.push_back(eInv_L2S2_Mangoes);
			Hud->ShowInventory(&vecInvIds);
		
		break;
	
	default:
		break;
	}
}

void L2_S2_PZMango::startHandReadyTimer()
{
	handReady->Show(false);
	isHandReadyTimerOn = true;
	handReadyTimer = 0.0f;
}

void L2_S2_PZMango::Update()
{
	
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);


	if(isHandAnimation)
	{
		if(CAnim_StoneThrowComplete->IsAnimFinished())
		{
			HOScene->GetObjectByID("hand")->SetEnable(false);
			HOScene->GetObjectByID("handwithstone")->SetEnable(true);
			isHandAnimation = false;
		}
	}

	if(isMangoHitAnimPlaying)
	{
		
	}

	if(isLeafFallAnimating)
	{
		if(LeafFallAnim->IsAnimationFinished())
		{
			LeafFallAnim->Show(false);
			isLeafFallAnimating = false;
		}
	}

	if(isPuzzleSolved)
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;
		timerGameOver += Control::LogicRate;
		if(timerGameOver >= 0.5f)
		{
			for (int i = 0; i < 3; i++)
			{
				CollectableMango[i]->Show(false);
				CollectableMangoBG[i]->Show(false);
			}
		
			Hud->InvBox_StopPuzzleMode();
			TakeInventoryItem(eInv_L2S2_Mangoes);

			
		}
		return;
	}

	float distance;
	float  dx;
	float  dy;

	

	if(isMangoDroping)
	{
		tweenTime += Control::LogicRate;
		float tempY = mangoStartY + (mangoEndY - mangoStartY) * easing(tweenTime,0,1.5,dropTime);
		activeMango->Pos.y = tempY;
		if( tweenTime >= dropTime )
		{
			activeMango->Show(false);
			if(activeMango == mango1)
			{
				mango1 = NULL;
			}
			else if(activeMango == mango2)
			{
				mango2 = NULL;
			}
			else if(activeMango == mango3)
			{
				mango3 = NULL;
			}
			tweenTime = 0.0f;
			isMangoDroping = false;
			isStoneThrowing = false;

			if(TMIndex == 1)
			{
				ProfileSystem::Instance->ActiveProfile.L2_S2_PZMango1Taken = true;
			}
			else if(TMIndex == 2)
			{
				ProfileSystem::Instance->ActiveProfile.L2_S2_PZMango2Taken = true;
			}
			else if(TMIndex == 3)
			{
				ProfileSystem::Instance->ActiveProfile.L2_S2_PZMango3Taken = true;
				//TM3_Anim[throwStoneAnimIndex]->SetAlpha(0.0f);
			}

			collectedMangoCount ++;//sreeee
			for (int i = 0 ; i < collectedMangoCount; i++)
			{
				CollectableMango[i]->Show(true);
			}

			if(collectedMangoCount == 3)
			{
				isPuzzleSolved = true;
				AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
				if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PZIsMiss)
					AwardManager::GetInstance()->CollectAward(kAward_MangoMiniGame_Without_Misthrow);
				timerGameOver = 0.0f;
				Control::Audio->QuickLoadAndPlaySFX("s2pz1_puzzlesoved");
				if(_handleExtrasGameComplete())
					return;
				return;
			}
		}
	}

	if(isThrowStoneHandAnimation)
	{
		showStoneThrowAnim(TMIndex);
	}

	if(isStoneThrowing)
	{
		vY += gravity;

		stone->Pos.x += vX;
		stone->Pos.y += vY;
		scaleFactor -= 0.02f;
		stone->SetScale(scaleFactor);
		rotationValue -= 20;
		stone->SetRotate(rotationValue);

		if(activeMango == mango1)
		{
			if(isHit)
			{
				dx = stone->Pos.x - mango1->Pos.x;
				dy = stone->Pos.y - mango1->Pos.y;
				distance = sqrt(dx * dx + dy * dy);
				if( distance < 120.0f && isMangoDroping == false)
				{
					leafAnimIndex = 0;
					isMangoHitAnimPlaying = true;
					
					SAFE_DELETE(LeafFallAnim);	
					SAFE_DELETE(mangoEFX);
					LeafFallAnim = new CObject();
					LeafFallAnim->LoadSprite("LV2\\S2\\P1\\LeafFallAnim.SPR");
					LeafFallAnim->SetZ(29);
					LeafFallAnim->Pos.x = mango1->Pos.x - 50;
					LeafFallAnim->Pos.y = mango1->Pos.y - 50;
					LeafFallAnim->Show(true);

					mangoEFX = new CObject();
					mangoEFX->LoadSprite("LV2\\S2\\P1\\Efx.SPR");
					mangoEFX->SetZ(30);
					mangoEFX->Pos.x = mango1->Pos.x - 50;
					mangoEFX->Pos.y = mango1->Pos.y - 50;
					mangoEFX->Show(true);
					isMangoDroping = true;
					mangoStartY = activeMango->Pos.y;
					mangoEndY = 750;
					//isStoneThrowing = false;
					isLineAnimating = false;
					hLine->Show(false);
					vLine->Show(false);
				}
			}
			else
			{
				if( stone->Pos.x <= -10)
				{
					isStoneThrowing = false;
				}
			}
		}
		else if(activeMango == mango2)
		{
			if(isHit)
			{
				dx = stone->Pos.x - mango2->Pos.x;
				dy = stone->Pos.y - mango2->Pos.y;
				distance = sqrt(dx * dx + dy * dy);
				//K_LOG ("distance ------------------------------> %d ",(int)distance);
				if( distance < 60.0f && isMangoDroping == false)
				{
					leafAnimIndex = 1;
					isMangoHitAnimPlaying = true;
					SAFE_DELETE(LeafFallAnim);	
					SAFE_DELETE(mangoEFX);					

					LeafFallAnim = new CObject();
					LeafFallAnim->LoadSprite("LV2\\S2\\P1\\LeafFallAnim.SPR");
					LeafFallAnim->SetZ(29);
					LeafFallAnim->Pos.x = mango2->Pos.x - 50;
					LeafFallAnim->Pos.y = mango2->Pos.y - 50;
					LeafFallAnim->Show(true);

					mangoEFX = new CObject();
					mangoEFX->LoadSprite("LV2\\S2\\P1\\Efx.SPR");
					mangoEFX->SetZ(30);
					mangoEFX->Pos.x = mango2->Pos.x - 50;
					mangoEFX->Pos.y = mango2->Pos.y - 50;
					mangoEFX->Show(true);
					isMangoDroping = true;
					mangoStartY = activeMango->Pos.y;
					mangoEndY = 750;
					//isStoneThrowing = false;
					isLineAnimating = false;
					hLine->Show(false);
					vLine->Show(false);
				}
			}
			else
			{
				if( stone->Pos.y <= -10)
				{
					isStoneThrowing = false;
				}
			}
		}
		else if(activeMango == mango3)
		{
			if(isHit)
			{
				dx = stone->Pos.x - mango3->Pos.x;
				dy = stone->Pos.y - mango3->Pos.y;
				distance = sqrt(dx * dx + dy * dy);
				if( distance < 80.0f && isMangoDroping == false)
				{
					leafAnimIndex = 2;
					isMangoHitAnimPlaying = true;
					SAFE_DELETE(LeafFallAnim);	
					SAFE_DELETE(mangoEFX);					

					LeafFallAnim = new CObject();
					LeafFallAnim->LoadSprite("LV2\\S2\\P1\\LeafFallAnim.SPR");
					LeafFallAnim->SetZ(29);
					LeafFallAnim->Pos.x = mango3->Pos.x - 50;
					LeafFallAnim->Pos.y = mango3->Pos.y - 50;
					LeafFallAnim->Show(true);

					mangoEFX = new CObject();
					mangoEFX->LoadSprite("LV2\\S2\\P1\\Efx.SPR");
					mangoEFX->SetZ(30);
					mangoEFX->Pos.x = mango3->Pos.x - 50;
					mangoEFX->Pos.y = mango3->Pos.y - 50;
					mangoEFX->Show(true);
					isMangoDroping = true;
					mangoStartY = activeMango->Pos.y;
					mangoEndY = 750;
					//isStoneThrowing = false;
					isLineAnimating = false;
					hLine->Show(false);
					vLine->Show(false);
				}
			}
			else
			{
				if( stone->Pos.y > 1386)
				{
					isStoneThrowing = false;
				}
			}
		}
	}

	if(isThrowStoneHandAnimation || isStoneThrowing) return;

	if(isLineAnimating)
	{
		dx = vLine->Pos.x - hLine->Pos.x;
		dy = vLine->Pos.y - hLine->Pos.y;
		distance = sqrt(dx * dx + dy * dy);			

		if( mango1Rect.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				if(activeMango == mango1)
				{
					if(distance < 60)
					{
						isHit = true;
						 Control::Audio->QuickLoadAndPlaySFX("s2pz1_mangohit");
					}
					else
					{
						isHit = false;
						ProfileSystem::Instance->ActiveProfile.L2_S2_PZIsMiss = true;
						Control::Audio->QuickLoadAndPlaySFX("s2pz1_mangomiss");
					}
					TMIndex = 1;
					//startHandReadyTimer();
					throwStoneAnimIndex = 0;
					/*TM1_Anim[throwStoneAnimIndex]->Show(true);
					TM1_Anim[throwStoneAnimIndex]->ForcePlayAnimation(0);*/
					CAnim_StoneThrow->PlayAnim();
					isThrowStoneHandAnimation = true;
					return;
				}
			}				
		}
		else if( mango2Rect.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				if(activeMango == mango2)
				{
					if(distance < 60)
					{
						isHit = true;
						Control::Audio->QuickLoadAndPlaySFX("s2pz1_mangohit");
					}
					else
					{
						isHit = false;
						ProfileSystem::Instance->ActiveProfile.L2_S2_PZIsMiss = true;
						Control::Audio->QuickLoadAndPlaySFX("s2pz1_mangomiss");
					}
					TMIndex = 2;
					//startHandReadyTimer();
					throwStoneAnimIndex = 0;
					/*TM2_Anim[throwStoneAnimIndex]->Show(true);
					TM2_Anim[throwStoneAnimIndex]->ForcePlayAnimation(0);*/
					CAnim_StoneThrow->PlayAnim();
					isThrowStoneHandAnimation = true;
					return;
				}
			}				
		}
		else if( mango3Rect.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				if(activeMango == mango3)
				{
					if(distance < 60)
					{
						isHit = true;
						Control::Audio->QuickLoadAndPlaySFX("s2pz1_mangohit");
					}
					else
					{
						isHit = false;
						ProfileSystem::Instance->ActiveProfile.L2_S2_PZIsMiss = true;
						Control::Audio->QuickLoadAndPlaySFX("s2pz1_mangomiss");
					}
					TMIndex = 3;
					//startHandReadyTimer();
					throwStoneAnimIndex = 0;
					/*TM3_Anim[throwStoneAnimIndex]->Show(true);
					TM3_Anim[throwStoneAnimIndex]->ForcePlayAnimation(0);*/
					CAnim_StoneThrow->PlayAnim();
					isThrowStoneHandAnimation = true;
					return;
				}
			}				
		}
		else
		{
			//Click outside anywhere
			if( Control::Input->LBclicked() )
			{
				if(distance < 60)
				{
					isHit = true;
					Control::Audio->QuickLoadAndPlaySFX("s2pz1_mangohit");
				}
				else
				{
					isHit = false;
					ProfileSystem::Instance->ActiveProfile.L2_S2_PZIsMiss = true;
					Control::Audio->QuickLoadAndPlaySFX("s2pz1_mangomiss");
				}
				//startHandReadyTimer();
				throwStoneAnimIndex = 0;

				if(activeMango == mango1)
				{
					TMIndex = 1;
					/*TM1_Anim[throwStoneAnimIndex]->Show(true);
					TM1_Anim[throwStoneAnimIndex]->ForcePlayAnimation(0);*/
					CAnim_StoneThrow->PlayAnim();
					isThrowStoneHandAnimation = true;
				}
				else if(activeMango == mango2)
				{
					TMIndex = 2;
					/*TM2_Anim[throwStoneAnimIndex]->Show(true);
					TM2_Anim[throwStoneAnimIndex]->ForcePlayAnimation(0);*/
					CAnim_StoneThrow->PlayAnim();
					isThrowStoneHandAnimation = true;
				}
				else if(activeMango == mango3)
				{
					TMIndex = 3;
					/*TM3_Anim[throwStoneAnimIndex]->Show(true);
					TM3_Anim[throwStoneAnimIndex]->ForcePlayAnimation(0);*/
					CAnim_StoneThrow->PlayAnim();
					isThrowStoneHandAnimation = true;
				}
			
				return;
			}	
		}
		
		hLine->Pos.x += xValue;
		vLine->Pos.y += yValue;

		if(xValue == 2)
		{
			if(hLine->Pos.x >= xTarget)
			{
				xTarget = hLine->Pos.x - 300;
				xValue = -2;
			}
		}
		else
		{
			if(hLine->Pos.x <= xTarget)
			{
				xTarget = hLine->Pos.x + 300;
				xValue = 2;
			}
		}

		if(yValue == 2)
		{
			if(vLine->Pos.y >= yTarget)
			{
				yTarget = vLine->Pos.y - 300;
				yValue = -2;
			}
		}
		else
		{
			if(vLine->Pos.y <= yTarget)
			{
				yTarget = vLine->Pos.y + 300;
				yValue = 2;
			}
		}
	}

	if( mango1Rect.Intersection(&pos) && mango1 != NULL&& ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced)
	{
		Cursor::SetMode(CA_HandPointy);
		if( Control::Input->LBclicked())
		{
			isLineAnimating = true;
			activeMango = mango1;
			AnimateLines(mango1);	
			xTarget = hLine->Pos.x + 300;
			yTarget = vLine->Pos.y + 300;
		}			
		return;		
	}

	if( mango2Rect.Intersection(&pos) && mango2 != NULL && ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced)
	{
		Cursor::SetMode(CA_HandPointy);
		if( Control::Input->LBclicked() )
		{
			isLineAnimating = true;
			activeMango = mango2;
			AnimateLines(mango2);	
			xTarget = hLine->Pos.x + 300;
			yTarget = vLine->Pos.y + 300;
		}			
		return;		
	}
	if( mango3Rect.Intersection(&pos) && mango3 != NULL && ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced)
	{
		Cursor::SetMode(CA_HandPointy);
		if( Control::Input->LBclicked() )
		{
			isLineAnimating = true;
			activeMango = mango3;
			AnimateLines(mango3);	
			xTarget = hLine->Pos.x + 300;
			yTarget = vLine->Pos.y + 300;
		}
		return;		
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_NagaTemple;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}
	#endif
	if( IsMouseOverGoBackArea(&pos))
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L2_S2_NagaTemple);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L2_S2_NagaTemple);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S2_NagaTemple, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_NagaTemple;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced)
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			Hud->ShowBannerText("L2mango");
		}
	}

}


void L2_S2_PZMango::ProcessSkip()
{
	if(!isPuzzleSolved)
	{
		isPuzzleSolved = true;
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		ProfileSystem::Instance->ActiveProfile.L2_S2_PZIsMiss = true;
		for (int i = 0; i < 3; i++)
		{
			CollectableMango[i]->Show(false);
			CollectableMangoBG[i]->Show(false);
		}

		Hud->InvBox_StopPuzzleMode();
		TakeInventoryItem(eInv_L2S2_Mangoes);

		
	}
}


float L2_S2_PZMango::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2_S2_PZMango::~L2_S2_PZMango()
{
	if(!isPuzzleSolved)
		Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(CAnim_StoneThrow);
	SAFE_DELETE(CAnim_StoneThrowComplete);
	SAFE_DELETE(LeafFallAnim);
	SAFE_DELETE(mangoEFX);
	//MUSIC_SFX

	

	Control::Audio->StopSample(aAmbs2MangoPuzzle);
	Control::Audio->UnloadSample(aAmbs2MangoPuzzle);
	//MUSIC_SFX
}

void L2_S2_PZMango::ReceiveMessage(int val)
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
			if( itemVal == eInv_L2S6_SmallStone1 && !ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced)
			{
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					Hud->HideMessageBanner();

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_SmallStone1] = false;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_SmallStone2] = false;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_SmallStone3] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_SmallStone1);
					ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced = true;
					Hud->InvBox_StartPuzzleMode();
					 Control::Audio->QuickLoadAndPlaySFX("s2pz1_stoneplaced");
					 HOScene->GetObjectByID("handwithstone")->SetEnable(true);
					 for (int i = 0; i < 3; i++)
					 {
						CollectableMangoBG[i]->Show(true);
					 }
					return;
			}
			else if (itemVal == eInv_L2S1_RoundStone && !ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced)
			{
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					Hud->ShowBannerText("L2this");
				
			}
			else
					GFInstance->WrongInvDropMsg(Hud);
			
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		

		if(isPuzzleSolved  && ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_NagaTemple;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
		}
	}	
}

void L2_S2_PZMango::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 2;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced && ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken)
					{
						Hud->ShowHintForInvItem(eInv_L2S6_SmallStone1, &Vid1BgRectLoop);
						return;
					}
					break;
			case 1: ++iNonHOHint;
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;
			
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();	
}
 void L2_S2_PZMango::NavigateToScene()
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