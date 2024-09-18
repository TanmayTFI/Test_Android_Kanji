//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 05 : Mooppan Shelf Puzzle
//====================================

#include "L1_S3_PZShelf.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "GameFlow.h"
#include "AwardManager.h"


//0 - X axis, 1 - Y axis, 2 - Rotation
const int ChangeMoveArray[14] = {2,15, 2,-20, 1,-10, 2,30, 0,20, 0,-10, 0,-15};

const CRectangle  FlyerZoomedRect(482.0f,91.0f,413.0f,534.0f);
const CRectangle  ShelfFlyerRect(491.0f,286.0f,430.0f,336.0f);

const CRectangle  InvItemRectHOLocket(607.0f,432.0f,224.0f,191.0f);
const CRectangle  MemoryRectDoll(518.0f,195.0f,344.0f,434.0f);

L1_S3_PZShelf::L1_S3_PZShelf()
{
	Visible = false;
	musicFadeOutTimer = 0;
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S3\\P1SHELF\\P1.SCN");

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

	GFInstance->PostUpdate(1);

	InitPuzzle();

	clickVFX = new SuperFX();
	clickVFX->LoadEffectIni("PRT\\L1\\DiffPuzFX.INI");
	clickVFX->SetZ(100);

	hintTrailTime = 0;
	isHintTrailActive = false;

	hintTrailVFX = new SuperFX("PRT\\L1\\hinttrail.INI", PointSystem::CreateCPoint(0.0f,0.0f,0), 100);
	hintCircleVFX = new SuperFX("PRT\\L1\\hintcirc.INI", PointSystem::CreateCPoint(0.0f,0.0f,0), 100);

	shelfBgVFX = new SuperFX("PRT\\L1\\s3shelffull.INI", PointSystem::CreateCPoint(678,339,0), 100); 
	shelfBgVFX->AdvanceByTime(2.0f);
	shelfBgVFX->Start();

	topGlareVFX = new SuperFX("PRT\\L1\\s3glares.INI", PointSystem::CreateCPoint(667,194,0), 100); 
	topGlareVFX->AdvanceByTime(2.0f);
	topGlareVFX->Start();

	if(!ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved)
		Hud->InvBox_StartPuzzleMode();
	
	GFInstance->PostUpdate(1);

	IsMooppanFlyerZooming = false;
	IsMooppanFlyerZoomedIn = false;
	IsFlyerFlyingToDiary = false;
	
	SetZ(30);
	
	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if( Control::Audio->IsPlaying(aTrackL1Mansion) )
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;	

	Control::Audio->LoadSample(aAmbS3PuzShelf,AudioAmb);
	Control::Audio->PlaySample(aAmbS3PuzShelf,true);

	Control::Audio->LoadSample(aVOL1MaryRefS3_2,AudioVO);
	//MUSIC_SFX
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S3_PZShelf]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S3_PZShelf])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S3_PZShelf] = true;


	isBackFromKidsRoom = false;

	//if came back from kids room
	if(ProfileSystem::Instance->ActiveProfile.L1S3_IsMemoryDollTaken)
	{
		HOScene->GetObjectByID("dollstatue")->SetEnable(false);
		isBackFromKidsRoom = true;
	}

#ifdef _CEBUILD
	//Tutorial show for memory collectible
	if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
	{		
		if(ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken && !ProfileSystem::Instance->ActiveProfile.L1S3_IsMemoryDollTaken && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT17_MemoryCollectible])
			Hud->Tutor->ShowTutorial(TUT17_MemoryCollectible);
	}
#endif 

	Visible = true;	//To render anything with renderlistentity
}

void L1_S3_PZShelf::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S5_TreasureRoom] = isAnyTaskLeft;
}

void L1_S3_PZShelf::InitPuzzle()
{
	isTweening = false;
	TotalTime = 1.0f;
	tweenTime = 0;

	isDoorOpening = false;

	numChangesFound = 0;

	delayTime = 0;

	startValue = 0;
	endValue = 0;

	isPuzzleSolved = ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved;

	int shelfId = 0;
	
	LockInitX = 0;
	for (int i = 0; i < NUMCHANGEELEMENTS; i++)
	{
		ChangeElements[i] = HOScene->GetObjectByID(std::string("Change")+MKSTR((i+1)));

		ShelfElements[shelfId++] = ChangeElements[i];

		Locks[i] = HOScene->GetObjectByID(std::string("Lock")+MKSTR((i+1)));
		//Lock x - 20 in open state

		ShelfElements[shelfId++] = Locks[i];

		IsChangeFound[i] = ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfDifferFound[i];

		if(IsChangeFound[i])
		{
			numChangesFound += 1;
			ChangeElements[i]->PlayAnimation(1);
			Locks[i]->Pos.x -= 20;
		}
		else
		{
			LockInitX = Locks[i]->Pos.x;
		}
	}

	CountGlow = HOScene->GetObjectByID("cntBtnGlow");
	CountText = HOScene->GetObjectByID("txtcount");
	CountFrameBg = HOScene->GetObjectByID("cntBtnBg");

	HintPuzBtn = HOScene->GetObjectByID("BtnHint");
	HintPuzBtnGlow = HOScene->GetObjectByID("BtnHintGlow");
	HintPuzBtnBar = HOScene->GetObjectByID("BtnHintChargeBar");

	HintPuzBtnGlow->SetEnable(false);

	HintPuzBtnBar->Paused = true;
	HintPuzTimer = 0;
	HintPuzThreshold = 15.0f;

	hintStartPt.x = HintPuzBtn->Pos.x;
	hintStartPt.y = HintPuzBtn->Pos.y;

	CountText->PlayAnimation(numChangesFound);

	flyerOnScene = HOScene->GetObjectByID("flyerscn");

	//Localized
	CObject* temp2 = new CObject();
	temp2->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S3\\P1SHELF\\MOOPANLETTER_ROLLED.SPR", flyerOnScene->Pos.x, flyerOnScene->Pos.y, flyerOnScene->ZOrder + 1);
	temp2->SetUID("flyerRolled");
	HOScene->PushObject(temp2);
	temp2->SetEnable(false);

	flyerZoomed = HOScene->GetObjectByID("flyerzoomed");
	//Localized
	temp2 = NULL;
	temp2 = new CObject();
	temp2->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S3\\P1SHELF\\MOOPANLETTER.SPR", flyerZoomed->Pos.x, flyerZoomed->Pos.y, flyerZoomed->ZOrder + 5);
	temp2->SetUID("flyerFull");
	HOScene->PushObject(temp2);
	temp2->SetEnable(false);

	flyerZoomed->SetEnable(false);

	CAnim_flyer = new CustomAnimator("ANIM\\L1\\s3flyerscn.anim", flyerOnScene);
	CAnim_flyerBig = new CustomAnimator("ANIM\\L1\\s3flyerzoom.anim", flyerZoomed);
		
	
	ShelfElements[shelfId++] = HOScene->GetObjectByID("LeafHillMask");
	ShelfElements[shelfId++] = HOScene->GetObjectByID("shelfBgLockMask1");
	ShelfElements[shelfId++] = HOScene->GetObjectByID("shelfBgLockMask2");
	ShelfElements[shelfId++] = HOScene->GetObjectByID("shelfBg");

	int ArrayPts1[] = {765,491,787,513,818,514,815,502,822,496,797,485};
	ChangePolyrects[0]=  new CPolygon(6);
	ChangePolyrects[0]->Init(ArrayPts1);
	int ArrayPts2[] = {844,284,864,288,872,305,898,314,931,308,899,329,856,319};
	ChangePolyrects[1]=  new CPolygon(7);
	ChangePolyrects[1]->Init(ArrayPts2);
	int ArrayPts3[] = {784,177,856,167,927,197,904,164,939,141,950,108,909,146,790,154};;
	ChangePolyrects[2]=  new CPolygon(8);
	ChangePolyrects[2]->Init(ArrayPts3);
	int ArrayPts4[] = {1059,127,1075,147,1073,160,1097,160,1107,181,1122,163,1135,158,1130,141,1144,129,1126,122,1122,110,1104,114,1095,99,1086,124};
	ChangePolyrects[3]=  new CPolygon(14);
	ChangePolyrects[3]->Init(ArrayPts4);
	int ArrayPts5[] = {1024,550,1046,536,1035,516,1005,506,995,540,1009,554};
	ChangePolyrects[4]=  new CPolygon(6);
	ChangePolyrects[4]->Init(ArrayPts5);
	int ArrayPts6[] = {1088,423,1103,405,1128,403,1138,416,1119,417,1112,443};
	ChangePolyrects[5]=  new CPolygon(6);
	ChangePolyrects[5]->Init(ArrayPts6);
	int ArrayPts7[] = {1029,314,1090,285,1124,303,1078,320,1061,325};
	ChangePolyrects[6]=  new CPolygon(5);
	ChangePolyrects[6]->Init(ArrayPts7);


	FullOpenLeftDoor = HOScene->GetObjectByID("FullLeftOpenDoor");
	FullOpenRightDoor = HOScene->GetObjectByID("FullRightOpenDoor");
	HalfOpenLeftDoor = HOScene->GetObjectByID("HalfLeftOpenDoor");
	HalfOpenLeftDoor->SetScale(2.0f);
	HalfOpenRightDoor = HOScene->GetObjectByID("HalfRightOpenDoor");
	HalfOpenRightDoor->SetScale(2.0f);

	FullOpenLeftDoor->Show(false);
	FullOpenRightDoor->Show(false);
	HalfOpenLeftDoor->Show(false);
	HalfOpenRightDoor->Show(false);

	//Localized
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S3\\P1SHELF\\FOUND.SPR", 720, 40, CountText->ZOrder + 5);
	temp->SetUID("found");
	HOScene->PushObject(temp);

	temp = NULL;
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S3\\P1SHELF\\HINT.SPR", 700, 100, HintPuzBtn->ZOrder + 5);
	temp->SetUID("hint");
	HOScene->PushObject(temp);

	if(isPuzzleSolved)
	{
		CountGlow->SetEnable(false);
		CountText->SetEnable(false);
		CountFrameBg->SetEnable(false);

		HintPuzBtn->SetEnable(false);
		HintPuzBtnBar->SetEnable(false);
		HintPuzBtnGlow->SetEnable(false);

		HOScene->GetObjectByID("found")->SetEnable(false);
		HOScene->GetObjectByID("hint")->SetEnable(false);

		SetShelfAlpha(0,false);
		
		FullOpenLeftDoor->Show(true);
		FullOpenRightDoor->Show(true);

		if(ProfileSystem::Instance->ActiveProfile.L1S3_PZMoopanFlyerTaken)
			flyerOnScene->Show(false);

		if(ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken)
			HOScene->GetObjectByID("holocket")->Show(false);
	}
}

void L1_S3_PZShelf::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL1Mansion) )
	{
		Control::Audio->Samples[aTrackL1Mansion]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if(Control::Audio->GlobalMusicVol - musicFadeOutTimer<=0)
		{
			Control::Audio->Samples[aTrackL1Mansion]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL1Mansion);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
				Control::Audio->PlaySample(aTrackL1Puzzle, true);
		}
			musicFadeOutTimer += Control::LogicRate * 8;
	}	
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
			Control::Audio->PlaySample(aTrackL1Puzzle, true);
	}

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(isPuzzleSolved)
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
		{
			return;
		}

		if(IsMooppanFlyerZooming)
		{
			if(CAnim_flyer->IsAnimFinished())
			{
				flyerOnScene->SetEnable(false);
				IsMooppanFlyerZooming = false;
				IsMooppanFlyerZoomedIn = true;
			}
			return;
		}

		if(isDoorOpening)
		{
			delayTime += Control::LogicRate;
			if(isHalfOpenDoorFadingIn && delayTime<0.65f)
				return;
			if(isHalfOpenDoorFadingIn)
			{
				tweenTime += Control::LogicRate;
				if(tweenTime>0.35f)
					SetShelfAlpha((0.4f-(tweenTime-0.35f))/0.4f);
				HalfOpenLeftDoor->SetAlpha((tweenTime*2.0f)>1.0f?1.0f:(tweenTime*2.0f));
				HalfOpenRightDoor->SetAlpha((tweenTime*2.0f)>1.0f?1.0f:(tweenTime*2.0f));

				if(tweenTime>TotalTime)
				{
					tweenTime = 0;
					delayTime = 0;

					isHalfOpenDoorFadingIn = false;
					isFullOpenDoorFadingIn = true;

					FullOpenLeftDoor->SetAlpha(0);
					FullOpenRightDoor->SetAlpha(0);

					FullOpenLeftDoor->Show(true);
					FullOpenRightDoor->Show(true);

					SetShelfAlpha(0,false);
				}
			}
			else if(isFullOpenDoorFadingIn)
			{
				if(delayTime>0.5f)
				{
					tweenTime += Control::LogicRate;

					HalfOpenLeftDoor->SetAlpha(1.0f - easing(tweenTime,0,1,TotalTime));
					HalfOpenRightDoor->SetAlpha(1.0f - easing(tweenTime,0,1,TotalTime));

					FullOpenLeftDoor->SetAlpha(easing(tweenTime,0,1,TotalTime));
					FullOpenRightDoor->SetAlpha(easing(tweenTime,0,1,TotalTime));
				}

				if(tweenTime>TotalTime)
				{
					FullOpenLeftDoor->SetAlpha(1.0f);
					FullOpenRightDoor->SetAlpha(1.0f);

					HalfOpenLeftDoor->Show(false);
					HalfOpenRightDoor->Show(false);

					tweenTime = 0;
					isFullOpenDoorFadingIn = false;
					isDoorOpening = false;
				}
			}
		}
	}
}

void L1_S3_PZShelf::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
		case eInv_L1S3_HOLocket:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_HOLocket] = true;
			ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken = true;
			vecInvIds.push_back(eInv_L1S3_HOLocket);
			Hud->ShowInventory(&vecInvIds);
		break;
	
	default:
		break;
	}
}

void L1_S3_PZShelf::SetShelfAlpha(float alpha, bool visible)
{
	for (int i = 0; i < NUMSHELFELEMENTS; i++)
	{
		if(!visible)
			ShelfElements[i]->Show(false);
		else
			ShelfElements[i]->SetAlpha(alpha);
	}
}

void L1_S3_PZShelf::Update()
{	
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	
	CPoint pos = Control::Input->Mpos();

	if (flyerOnScene->IsActive && ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L1S3_PZMoopanFlyerTaken)
	{
		HOScene->GetObjectByID("flyerRolled")->SetEnable(true);
		HOScene->GetObjectByID("flyerRolled")->Pos.x = flyerOnScene->Pos.x;
		HOScene->GetObjectByID("flyerRolled")->Pos.y = flyerOnScene->Pos.y;
		HOScene->GetObjectByID("flyerRolled")->SetRotate(flyerOnScene->GetRotate());
	}
	else
		HOScene->GetObjectByID("flyerRolled")->SetEnable(false);

	if (flyerZoomed->IsActive && ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L1S3_PZMoopanFlyerTaken)
	{
		if ((CAnim_flyerBig->animTime / CAnim_flyerBig->TotalAnimTime) >= 0.8f)
		{
			HOScene->GetObjectByID("flyerFull")->SetEnable(true);
			HOScene->GetObjectByID("flyerFull")->Pos.x = flyerZoomed->Pos.x;
			HOScene->GetObjectByID("flyerFull")->Pos.y = flyerZoomed->Pos.y;
			HOScene->GetObjectByID("flyerFull")->SetRotate(flyerZoomed->GetRotate());
		}
	}
	else
		HOScene->GetObjectByID("flyerFull")->SetEnable(false);

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
	
	if(isBackFromKidsRoom)
	{
		isBackFromKidsRoom = false;

		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
		SendMessage(ControlExit, ID, HOSM_NewArea);
		return;
	}
#else
	Cursor::SetMode(CA_Normal);
#endif 

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	if(isPuzzleSolved)
	{
		if(isDoorOpening)
		{
			return; //moved to required
		}
		else
		{
			if(IsMooppanFlyerZooming)
				return;

			if(IsFlyerFlyingToDiary)
			{
				if(flyerZoomed->IsTweening && flyerZoomed->IsTweenFinished())
				{
					flyerZoomed->IsTweening = false;
					flyerZoomed->SetEnable(false);
					ProfileSystem::Instance->ActiveProfile.L1S3_PZMoopanFlyerTaken = true;

					//Journal Entry 7 MooppanFlyer
					if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P7_MooppanFlyer])
					{
						//Flag Set
						ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P7_MooppanFlyer] = true;
						//Add to Note Array
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
							= eJrn_L1P7_MooppanFlyer;
						
						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
						//Increment Notes
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
						Hud->ShowJrnEntryFX();
						Hud->ShowBannerText("L1mopan");
						Control::Audio->PlaySample(aVOL1MaryRefS3_2);
					}

					IsFlyerFlyingToDiary = false;
				}
				return;
			}

			if(IsMooppanFlyerZoomedIn)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S3_PZMoopanFlyerTaken && FlyerZoomedRect.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked() )
					{
						IsMooppanFlyerZoomedIn = false;

						IsFlyerFlyingToDiary = true;
						Control::Audio->QuickLoadAndPlaySFX(aSFXJrnlFlyer);

						float valArr[5] = {0, 59, 537, 0.25f, 15.0f};
						flyerZoomed->TweenTo(valArr, 1.0f, TW_EASE, false);
						HOScene->GetObjectByID("flyerFull")->TweenTo(valArr, 1.0f, TW_EASE, false);
					}
				}
			}
			else
			{
				//Door is opened. Now Take the Items
				if(!ProfileSystem::Instance->ActiveProfile.L1S3_PZMoopanFlyerTaken)
				{
					if (ShelfFlyerRect.Intersection(&pos))
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked() )
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3ShelfNotePicked);

							CAnim_flyer->PlayAnim();
							CAnim_flyerBig->PlayAnim();
							flyerOnScene->SetEnable(true);
							flyerZoomed->SetEnable(true);

							IsMooppanFlyerZooming = true;
						} 
					}
				}
				else
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken)
					{
						if (InvItemRectHOLocket.Intersection(&pos))
						{
							Cursor::SetMode(CA_HandTake);
							if( Control::Input->LBclicked() )
							{
								HOScene->GetObjectByID("holocket")->Show(false);
								TakeInventoryItem(eInv_L1S3_HOLocket);

								#ifdef _CEBUILD
									if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
									{		
										if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsMemoryDollTaken && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT17_MemoryCollectible])
											Hud->Tutor->ShowTutorial(TUT17_MemoryCollectible);
									}
								#endif
							} 
						}
					}
					else
					{
					#ifdef _CEBUILD
						if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3MaryLamb] && MemoryRectDoll.Intersection(&pos))
						{
							if( Control::Input->LBclicked() )
							{
								if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
								{
									ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT17_MemoryCollectible] = true;
									Hud->Tutor->HideTutorial();
								}

								//Take Memory Doll								
								ProfileSystem::Instance->ActiveProfile.L1S3_IsMemoryDollTaken = true;

								HOScene->GetObjectByID("dollstatue")->FadeOut();
								Hud->ShowMemoryObjectCollected_CE(eMem_L1S3MaryLamb, &HOScene->GetObjectByID("dollstatue")->Pos);
								ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
								Hud->isSceneCloseAfterMemoryFade = true;
								//SendMessage(ControlExit, ID, HOSM_NewArea);
								return;
							}
						}
					#else
						if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3MaryLamb] && MemoryRectDoll.Intersection(&pos))
						{
							Cursor::SetMode(CA_Help);
							if( Control::Input->LBclicked() )
							{
								HOScene->GetObjectByID("dollstatue")->FadeOut();
								Hud->ShowMemoryObjectCollected_SE(eMem_L1S3MaryLamb, &HOScene->GetObjectByID("dollstatue")->Pos);
							}
						}
					#endif 
					}
				}
			}
		}
	}
	else
	{
		//Puzzle not solved yet
		if(Control::Input->LBclicked())
		{
			if(pos.x > 220 && pos.x < 660 && pos.y > 75 && pos.y < 570)
				Control::Audio->QuickLoadAndPlaySFX(aSFXGlassBtn);
			//if(pos.x ) 220, 75 660, 570
		}

		if(isTweening)
		{
			tweenTime += Control::LogicRate;
			float val = startValue + (endValue-startValue)*easing(tweenTime,0,1,TotalTime);
			
			if(tweenType==0)
			{
				ChangeElements[currentChangeId]->Pos.x = val;
			}
			else if(tweenType==1)
			{
				ChangeElements[currentChangeId]->Pos.y = val;
			}
			else if(tweenType==2)
			{
				ChangeElements[currentChangeId]->SetRotate(val);
			}

			Locks[currentChangeId]->Pos.x = LockInitX - 20.0f*easing(tweenTime,0,1,TotalTime);

			if(tweenTime>TotalTime)
			{
				tweenTime = 0.0f;
				isTweening = false;

				Locks[currentChangeId]->Pos.x = LockInitX - 20.0f;

				ChangeElements[currentChangeId]->PlayAnimation(1);
				IsChangeFound[currentChangeId] = true;

				ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfDifferFound[currentChangeId] = true;

				currentChangeId = -1;

				bool IsGameWon = true;
				for (int i = 0; i < NUMCHANGEELEMENTS; i++)
				{
					if(IsChangeFound[i]==false)
					{
						IsGameWon = false;
						break;
					}
				}

				if(IsGameWon)
				{
					startValue = 0;
					endValue = 1.0f;

					CountGlow->SetEnable(false);
					CountText->SetEnable(false);
					CountFrameBg->SetEnable(false);

					HintPuzBtn->SetEnable(false);
					HintPuzBtnBar->SetEnable(false);
					HintPuzBtnGlow->SetEnable(false);

					HOScene->GetObjectByID("found")->SetEnable(false);
					HOScene->GetObjectByID("hint")->SetEnable(false);

					isPuzzleSolved = true;
					if(_handleExtrasGameComplete())
						return;

					ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved = true;
					AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					delayTime = 0;

					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3SelfOpen);

					Hud->InvBox_StopPuzzleMode();

					TotalTime = 0.75f;
					isDoorOpening = true;
					isHalfOpenDoorFadingIn = true;

					HalfOpenLeftDoor->SetAlpha(0);
					HalfOpenRightDoor->SetAlpha(0);
					HalfOpenLeftDoor->Show(true);
					HalfOpenRightDoor->Show(true);
				}

			}
		}
		else
		{
			if(isHintTrailActive)
			{
				hintTrailTime += Control::LogicRate;
				CPoint bPos = Elixir::GetBezierPoint(&hintStartPt, &hintCtrlPt, &hintEndPt, hintTrailTime);
				hintTrailVFX->Pos = bPos;
				hintTrailVFX->Emit();

				if(hintTrailTime>1.0f)
				{
					isHintTrailActive = false;
					hintCircleVFX->Pos = bPos;
					hintCircleVFX->Start();
				}
				else
				{
					int frame = (int)((1.0f-hintTrailTime)*44.0f);
					HintPuzBtnBar->SetFrame(frame);
				}
				return;
			}
			if(HintPuzTimer>HintPuzThreshold)
			{
				if(HintPuzBtn->Collide(&pos))
				{
					HintPuzBtnGlow->SetEnable(true);
					HintPuzBtn->PlayAnimation(1);
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked() )
					{
						//Sent Hint to change Rect
						for (int i = 0; i < NUMCHANGEELEMENTS; i++)
						{
							if( !IsChangeFound[i] )
							{
								HintPuzBtnGlow->SetEnable(false);

								HintPuzTimer = 0;
								hintTrailTime = 0;
								isHintTrailActive = true;

								hintEndPt.x = ChangePolyrects[i]->center.x;
								hintEndPt.y = ChangePolyrects[i]->center.y;

								hintCtrlPt.x = ( hintEndPt.x + hintStartPt.x) * 0.5f;
								hintCtrlPt.y = ( hintEndPt.y + hintStartPt.y) * 0.5f;
								hintCtrlPt.y -= 200-rand()%400;

								break;
							}
						}
						return;
					}
				}
				else
					HintPuzBtnGlow->SetEnable(false);
			}
			else
			{
				if(HintPuzBtn->Collide(&pos,-20))
				{
					Cursor::SetMode(CA_Help);
					if( Control::Input->LBclicked() )
					{
						Hud->ShowBannerText("L1hcharge" );
					}
				}
				HintPuzTimer += Control::LogicRate;
				int frame = int(HintPuzTimer/HintPuzThreshold*45.0f);
				HintPuzBtnBar->SetFrame(frame);

				if(HintPuzTimer>HintPuzThreshold)
					HintPuzBtnBar->SetFrame(44);
			}

			for (int i = 0; i < NUMCHANGEELEMENTS; i++)
			{
				if( !IsChangeFound[i] && ChangePolyrects[i]->Intersection(&pos))
				{
					//Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked() )
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3ShelfFindDiff);
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3ShelfLockOpen);

						//Change Found
						isTweening = true;
						tweenTime = 0;

						clickVFX->SetPos(PointSystem::CreateCPoint(ChangeElements[i]->Pos.x,ChangeElements[i]->Pos.y,0));
						clickVFX->Start();

						currentChangeId = i;

						numChangesFound += 1;
						CountText->PlayAnimation(numChangesFound);
						CountGlow->ForcePlayAnimation(1);

						tweenType = ChangeMoveArray[i*2];

						if(tweenType==0)
						{
							startValue = ChangeElements[i]->Pos.x;
							endValue = ChangeElements[i]->Pos.x + ChangeMoveArray[i*2+1];
						}
						else if(tweenType==1)
						{
							startValue = ChangeElements[i]->Pos.y;
							endValue = ChangeElements[i]->Pos.y + ChangeMoveArray[i*2+1];
						}
						else if(tweenType==2)
						{
							startValue = ChangeElements[i]->GetRotate();
							endValue = ChangeElements[i]->GetRotate() + ChangeMoveArray[i*2+1];
						}
					}
				}
			}
		}
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
				Hud->InvBox_StopPuzzleMode();
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
				SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	#endif

	if( !isTweening && !isDoorOpening && IsMouseOverGoBackArea(&pos) )
	{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L1_S3_Foyer, 2);
			}
#else
			Hud->InvBox_StopPuzzleMode();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
}


void L1_S3_PZShelf::ProcessSkip()
{
	if(!isPuzzleSolved)
	{
		//Control::Audio->PlaySample(aSFXL2S5PuzWinDoorOpen);

		CountGlow->SetEnable(false);
		CountText->SetEnable(false);
		CountFrameBg->SetEnable(false);

		HintPuzBtn->SetEnable(false);
		HintPuzBtnBar->SetEnable(false);
		HintPuzBtnGlow->SetEnable(false);

		HOScene->GetObjectByID("found")->SetEnable(false);
		HOScene->GetObjectByID("hint")->SetEnable(false);

		startValue = 0;
		endValue = 1.0f;

		delayTime = 0;

		isPuzzleSolved = true;
		ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved = true;
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		Hud->InvBox_StopPuzzleMode();

		TotalTime = 0.75f;
		isDoorOpening = true;
		isHalfOpenDoorFadingIn = true;

		HalfOpenLeftDoor->SetAlpha(0);
		HalfOpenRightDoor->SetAlpha(0);
		HalfOpenLeftDoor->Show(true);
		HalfOpenRightDoor->Show(true);
	}
}


float L1_S3_PZShelf::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L1_S3_PZShelf::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;
}

L1_S3_PZShelf::~L1_S3_PZShelf()
{

	GFHud->InvBoxCtrl->ResetToInvBox();	
	
	SAFE_DELETE(CAnim_flyer) 
	SAFE_DELETE(CAnim_flyerBig)

	Control::Audio->Samples[aTrackL1Mansion]->SetVolume((float)Control::Audio->GlobalMusicVol);
	for (int i = 0; i < NUMCHANGEELEMENTS; i++)
	{
		delete ChangePolyrects[i];
		ChangePolyrects[i] = NULL;
	}

	for (int i = 0; i < NUMSHELFELEMENTS; i++)
	{
		ShelfElements[i] = NULL;
	}

	delete clickVFX;

	SAFE_DELETE(hintTrailVFX);
	SAFE_DELETE(hintCircleVFX);

	SAFE_DELETE(shelfBgVFX);
	SAFE_DELETE(topGlareVFX);
	SAFE_DELETE(ResetDia);

	//MUSIC_SFX
	if( Control::Audio->IsPlaying(aTrackL1Puzzle) )
		Control::Audio->StopSample(aTrackL1Puzzle);

	Control::Audio->StopSample(aAmbS3PuzShelf);
	Control::Audio->UnloadSample(aAmbS3PuzShelf);

	Control::Audio->UnloadSample(aVOL1MaryRefS3_2);
	//MUSIC_SFX
}

void L1_S3_PZShelf::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{
		Hud->InvBox_StopPuzzleMode();
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
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			
			
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		

		/*if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvUndergroundKeyTaken)		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}*/
	}	
}

void L1_S3_PZShelf::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3MaryLamb])
		{
			InitHintTrailEmit(700, 400);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}
	int iNonHOHint = 0; 
	int MaxNonHOHints = 1;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken)
					{
						InitHintTrailEmit(	InvItemRectHOLocket.GetCenterX(), InvItemRectHOLocket.GetCenterY(),false);
						return;
					}
					break;
			
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L1_S3_PZShelf::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved)
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
			ProfileSystem::Instance->ActiveProfile.L1S3_PZMoopanFlyerTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved = false;
			for (int i = 0; i < MAX_L1S3_SHELFPUZDIFFS; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfDifferFound[i] = false;
			}

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_PZShelf;
			SendMessage(ControlExit, ID, HOSM_NewArea);IsResetPop = false;
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
void L1_S3_PZShelf::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);

				Hud->InvBox_StopPuzzleMode();
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