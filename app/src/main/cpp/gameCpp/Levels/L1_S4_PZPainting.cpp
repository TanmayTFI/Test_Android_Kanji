//====================================
//DQFYLH
// Hidden Object System
//====================================

#include "L1_S4_PZPainting.h"
#include "Hud.h"
#include "Banners.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "TextureFactory.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "InfoPanel.h"
#include "AwardManager.h"

int	CurrentPos[MAX_PUZ_PIECES] = { 4,8,5,7,2,3,0,6,1 };
//Test
//int	CurrentPos[MAX_PUZ_PIECES] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
int	AdjPosArray[MAX_PUZ_PIECES][6] = {
		{1,2,-1,-1,-1,-1},		//0
		{0,2,3,-1,-1,-1},		//1
		{0,1,3,4,7,-1},			//2
		{1,2,4,5,-1,-1},		//3
		{2,3,5,6, 7, 8},		//4
		{3,4,6,-1,-1,-1},		//5
		{4,5,8,-1,-1,-1},		//6
		{2,4,8,-1,-1,-1},		//7
		{4,6,7,-1,-1,-1}		//8	
	};

const int BLANKPUZPIECE = 1;

const CRectangle  InvItemRectpainting(153.0f,18.0f,1043.0f,601.0f);
const CRectangle  SceneRectDoorHO(214.0f,57.0f,914.0f,560.0f);

//16 control points for bezier
float ControlPts[32] = { 333,195, 544,172, 426,258, 411,353, 552,237, 693,118, 757,230, 718,423, 548,457, 899,299, 1006,278, 874,317, 797,499, 793,569, 972,463, 899,428 };

L1_S4_PZPainting::L1_S4_PZPainting()
{
	Visible = false;
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S4\\P1\\MAIN.SCN");
	musicFadeOutTimer = 0;
	LeftLockOpened = HOScene->GetObjectByID("p1LeftLockOpen");
	RightLockOpened = HOScene->GetObjectByID("p1RightLockOpen");

	IsPaintingPuzzleSolved = ProfileSystem::Instance->ActiveProfile.L1S4_PaintingPuzzleSolved;

	PaintPuzzleScn = new CHOScene();
	PaintPuzzleScn->Load("LV1\\S4\\P1\\P1.SCN");
	PaintPuzzleScn->SetZOff(3);
	
	LeftLock = PaintPuzzleScn->GetObjectByID("p1LeftLock");
	RightLock = PaintPuzzleScn->GetObjectByID("p1RightLock");

	LeftLockGlow = PaintPuzzleScn->GetObjectByID("p1lockglowleft");
	LeftLockGlow->SetBlendMode(eBlend_AlphaAdditive);

	RightLockGlow = PaintPuzzleScn->GetObjectByID("p1lockglowright");
	RightLockGlow->SetBlendMode(eBlend_AlphaAdditive);
	
	gearSparksVFX = new SuperFX("PRT\\L1\\gearsparks.INI", PointSystem::CreateCPoint(608,211,0), 5);	
	endGameVFX = new SuperFX("PRT\\L1\\paintendfx.INI", PointSystem::CreateCPoint(676,349,0), 100);

	ambVFXFlies = new SuperFX("PRT\\L1\\s4pzflies.INI", PointSystem::CreateCPoint(508,175,0), 100);	
	ambVFXFlies->Start();
	ambVFXFlies->AdvanceByTime(1.0f);
	ambVFXGlares = new SuperFX("PRT\\L1\\s4pzglares.INI", PointSystem::CreateCPoint(1095,175,0), 100); 
	ambVFXGlares->Start();
	ambVFXGlares->AdvanceByTime(1.0f);

	ResetBtn = PaintPuzzleScn->GetObjectByID("ResetBtn");
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

	LeftLockGlow->SetEnable(false);
	RightLockGlow->SetEnable(false);

	if(IsPaintingPuzzleSolved)
	{
		LeftLock->SetEnable(false);
		RightLock->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S4_InvPaintingFrameTaken)
			PaintPuzzleScn->Show(false);
	}
	else
	{
		LeftLockOpened->SetEnable(false);
		RightLockOpened->SetEnable(false);

		Hud->InvBox_StartPuzzleMode();
	}

	InitPuzzle();

	tweenTime = 0;

	GFInstance->PostUpdate(1);
	
	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if( Control::Audio->IsPlaying(aTrackL1Hunted) )
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;	

	Control::Audio->LoadSample(aAmbS4PuzPainting,AudioAmb);
	Control::Audio->PlaySample(aAmbS4PuzPainting,true);

	//MUSIC_SFX
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S4_PZPainting]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S4_PZPainting])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S4_PZPainting] = true;
	
	 Visible = true;//CRenderListEntity
}

void L1_S4_PZPainting::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S4_Bedroom] = isAnyTaskLeft;
}

void L1_S4_PZPainting::InitPuzzle()
{
	//Painting puzzle stuffs
	IsEndAnimPlaying = false;
	endAnimTimer = 0;

	isLockLeftOpened = false;
	isLockRightOpened = false;

	IsPuzSliding = false;
	SlidePuzId = -1;

	Gear1 = PaintPuzzleScn->GetObjectByID("gear0");
	Gear2 = PaintPuzzleScn->GetObjectByID("gear1");

	PuzMouseOverCircle = PaintPuzzleScn->GetObjectByID("p1glow");
	PuzMouseOverCircle->SetBlendMode(eBlend_AlphaAdditive);
	PuzMouseOverCircle->SetAlpha(0.75f);
	PuzMouseOverCircle->SetEnable(false);

	CObject *obj = PaintPuzzleScn->GetObjectByID("leftglow");
	obj->SetScale(2.0f);
	obj->SetBlendMode(eBlend_AlphaAdditive);
	obj->Animate(0.25f,0.5f,6.0f);

	obj = PaintPuzzleScn->GetObjectByID("ray1");
	obj->SetScale(2.0f);
	obj->SetRotate(45);
	obj->SetBlendMode(eBlend_AlphaAdditive);
	obj->Animate(0.15f,0.65f,6.0f, 1.25f);

	obj = PaintPuzzleScn->GetObjectByID("ray2");
	obj->SetScale(2.0f);
	obj->SetRotate(45);
	obj->SetBlendMode(eBlend_AlphaAdditive);
	obj->Animate(0.15f,0.75f,8.0f);

	obj = PaintPuzzleScn->GetObjectByID("ray3");
	obj->SetScale(2.0f);
	obj->SetRotate(45);
	obj->SetBlendMode(eBlend_AlphaAdditive);
	obj->Animate(0.15f,0.65f,10.0f, 1.25f);

	obj = NULL;

	for (int i = 0; i < MAX_PUZ_PIECES; i++)
	{
		PuzPieces[i]	= PaintPuzzleScn->GetObjectByID(std::string("p1pz" + MKSTR(i)).c_str());
		PuzPosArray[i] = new CPoint(PuzPieces[i]->Pos);

		//PuzPieces[i]->SetPos( PointSystem::CreateCPoint(PuzPosArray[CurrentPos[i]]->x,PuzPosArray[CurrentPos[i]]->y,0) );
	}	
	PuzPieces[BLANKPUZPIECE]->Show(false);

	if(!IsPaintingPuzzleSolved)
	{
		for (int i = 0; i < MAX_PUZ_PIECES; i++)
		{
			PuzPieces[i]->SetPos( PointSystem::CreateCPoint(PuzPosArray[CurrentPos[i]]->x,PuzPosArray[CurrentPos[i]]->y,0) );
		}
	}

	Gear1->SetXY(PuzPosArray[CurrentPos[BLANKPUZPIECE]]->x,PuzPosArray[CurrentPos[BLANKPUZPIECE]]->y);
	Gear1->ForcePlayAnimation(1);
	Gear1->SetFrame(22);

	bzControlPt = new CPoint();
	bzStartPos = new CPoint();
	bzEndPos = new CPoint();
}

L1_S4_PZPainting::~L1_S4_PZPainting()
{
	GFHud->InvBoxCtrl->ResetToInvBox();	

	Control::Audio->Samples[aTrackL1Hunted]->SetVolume((float)Control::Audio->GlobalMusicVol);
	if(!IsPaintingPuzzleSolved)
		Hud->InvBox_StopPuzzleMode();

	//MUSIC_SFX
	if( Control::Audio->IsPlaying(aTrackL1Puzzle) )
		if(ProfileSystem::Instance->ActiveProfile.CurrentArea != eArea_L1_S4_PZDoor)
			Control::Audio->StopSample(aTrackL1Puzzle);

	Control::Audio->StopSample(aAmbS4PuzPainting);
	Control::Audio->UnloadSample(aAmbS4PuzPainting);

	//MUSIC_SFX

	//garbage collection in gameflow
	delete PaintPuzzleScn;	PaintPuzzleScn = NULL;

	SAFE_DELETE(gearSparksVFX);
	SAFE_DELETE(endGameVFX);

	SAFE_DELETE(ambVFXFlies);
	SAFE_DELETE(ambVFXGlares);
	SAFE_DELETE(ResetDia);

	delete bzControlPt;
	delete bzStartPos;
	delete bzEndPos;

	for (int i = 0; i < MAX_PUZ_PIECES; i++)
	{
		delete PuzPosArray[i];
		PuzPosArray[i] = NULL;
	}	
}

void L1_S4_PZPainting::ReceiveMessage(int val)
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
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{
		
		
	}	
}

void L1_S4_PZPainting::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	Cursor::SetMode(CA_Normal);

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	CPoint pos		= Control::Input->Mpos();

	bool clicked	= Control::Input->LBclicked();
	// bool Rclicked	= Control::Input->RBclicked(); //Sac: Unused variable, so commented it

	if( IsEndAnimPlaying )
		return;

	if(IsPaintingPuzzleSolved )
	{
		if( !ProfileSystem::Instance->ActiveProfile.L1S4_InvPaintingFrameTaken && InvItemRectpainting.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(clicked)
			{
				std::vector<int> vecInvIds;

				Hud->InvBoxCtrl->Activate(1);			
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_PhotoFrame] = true;
				ProfileSystem::Instance->ActiveProfile.L1S4_InvPaintingFrameTaken = true;
				
				//EmeraldTablet->Show(false);
				PaintPuzzleScn->FadeOut();

				vecInvIds.push_back(eInv_L1S4_PhotoFrame);
				Hud->ShowInventory(&vecInvIds);
				return;
			}
		}
		else if( SceneRectDoorHO.Intersection(&pos))
		{
#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_Look);
#else
			Cursor::SetMode(CA_Look);
#endif

			if(clicked)
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S4_PZDoor, 2, SceneRectDoorHO);
				}
#else
				Hud->InvBox_StopPuzzleMode();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_PZDoor;
				SendMessage(ControlExit, ID, HOSM_NewArea);
				return;
#endif

			}
		}
	}		//Paint Puzzle stuffs
	else
	{
		if(IsPuzSliding)
		{
			PuzMouseOverCircle->SetEnable(false);
			tweenTime += 1.5f*Control::LogicRate;

			CPoint bt = GetBezierPoint(bzStartPos,bzControlPt,bzEndPos,tweenTime);

			PuzPieces[SlidePuzId]->SetPos(&bt);

			/*PuzPieces[SlidePuzId]->Pos.x = bt.x;
			PuzPieces[SlidePuzId]->Pos.y = bt.y;*/

			PuzPieces[SlidePuzId]->SetRotate( tweenTime*360.0f);

			if(tweenTime>=1.0f)
			{
				tweenTime = 0;

				PuzPieces[SlidePuzId]->Pos.x = endX;
				PuzPieces[SlidePuzId]->Pos.y = endY;

				PuzPieces[SlidePuzId]->SetRotate(0);

				SlidePuzId = -1;

				IsPuzSliding = false;

				//Check for Game Won
				bool gameWon = true;
				for (int i = 0; i < MAX_PUZ_PIECES; i++)
				{
					if( CurrentPos[i] != i )
					{
						gameWon = false;
						break;
					}

				}

				if(gameWon)
				{	
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4PaintingPuzzleSolved);

					IsEndAnimPlaying = true;
					endGameVFX->Start();
					if(_handleExtrasGameComplete())
						return;

					if( Hud->IsPuzzleScene )
						Hud->InvBox_StopPuzzleMode();

					ProfileSystem::Instance->ActiveProfile.L1S4_PaintingPuzzleSolved = true;

					AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
					LeftLockGlow->SetAlpha(0);
					LeftLockGlow->SetEnable(true);
					RightLockGlow->SetAlpha(0);
					RightLockGlow->SetEnable(true);
				}
			}
		}
		else
		{
			for (int i = 0; i < MAX_PUZ_PIECES; i++)
			{
				if( PuzPieces[i]->Collide(&pos) )
				{
					Cursor::SetMode( CA_HandPointy );
					
					int currentTilePos = CurrentPos[i];
					int flag = 0;
					for (int k = 0; k < 6; k++)
					{
						if(AdjPosArray[currentTilePos][k] != -1 && AdjPosArray[currentTilePos][k] == CurrentPos[BLANKPUZPIECE])
						{
							flag = 1;
							break;
						}
					}

					if(flag == 1)
					{
						PuzMouseOverCircle->SetEnable(true);
						PuzMouseOverCircle->SetXY(PuzPieces[i]->Pos.x,PuzPieces[i]->Pos.y);
					}
					else
					{
						if(clicked)
						{
							PuzPieces[i]->Shake((rand()%4>2)?45.0f:135.0f,2.0f,(rand()%4>2)?20.0f:-20.0f,0.35f);
							Hud->ShowBannerText("L1space");
							return;
						}
					}

					if(clicked)
					{
						//Clicked on id i
						//get current pos of clicked id
						 currentTilePos = CurrentPos[i];

						//Check if any adjascent area is free
						for (int j = 0; j < 6; j++)
						{
							if ( AdjPosArray[currentTilePos][j] != -1 && AdjPosArray[currentTilePos][j] == CurrentPos[BLANKPUZPIECE] )
							{
								//If blank space is near
								int tempId = CurrentPos[i];
								CurrentPos[i] = CurrentPos[BLANKPUZPIECE];
								CurrentPos[BLANKPUZPIECE] = tempId;

								CalcControlPoint(tempId, CurrentPos[i]);

								bzEndPos->x = endX = PuzPieces[BLANKPUZPIECE]->Pos.x;
								bzEndPos->y = endY = PuzPieces[BLANKPUZPIECE]->Pos.y;
					
								bzStartPos->x = startX = PuzPieces[BLANKPUZPIECE]->Pos.x = PuzPieces[i]->Pos.x;
								bzStartPos->y = startY = PuzPieces[BLANKPUZPIECE]->Pos.y = PuzPieces[i]->Pos.y;

								SlidePuzId = i;

								Gear1->SetXY(startX,startY);
								Gear2->SetXY(endX,endY);

								gearSparksVFX->SetXY(startX,startY);
								gearSparksVFX->Start();

								Gear1->ForcePlayAnimation(1);
								Gear2->ForcePlayAnimation(1);
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4PaintingSlide);
								IsPuzSliding = true;
								break;
							}
						}
					}
					break;
				}
				else
				{
					PuzMouseOverCircle->SetEnable(false);
				}
			}
		}
	}

#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			Hud->InvBox_StopPuzzleMode();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_Bedroom;
			SendMessage(ControlExit, ID, HOSM_NewArea);				
			return;			
		}
#endif

	if( IsMouseOverGoBackArea(&pos) )
	{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitDown,eArea_L1_S4_Bedroom);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L1_S4_Bedroom);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L1_S4_Bedroom, 2);
			}
#else
			Hud->InvBox_StopPuzzleMode();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_Bedroom;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
		return;		
	}

}

void L1_S4_PZPainting::CalcControlPoint(int startId, int endId)
{
	int controlId = 0;
	switch (startId)
	{
	case 0:	 
		if(endId==1)
			 controlId = 0;
		else if(endId==2)
			 controlId = 3;
	break;

	case 1:	 
		if(endId==0)
			 controlId = 0;
		else if(endId==2)
			 controlId = 2;
		else if(endId==3)
			 controlId = 1;
	break;

	case 2:	 
		if(endId==0)
			 controlId = 3;
		else if(endId==1)
			 controlId = 2;
		else if(endId==3)
			 controlId = 4;
		else if(endId==4)
			 controlId = 7;
		else if(endId==7)
			 controlId = 8;
	break;

	case 3:	 
		if(endId==1)
			 controlId = 1;
		else if(endId==2)
			 controlId = 4;
		else if(endId==4)
			 controlId = 6;
		else if(endId==5)
			 controlId = 5;
	break;

	case 4:	 
		if(endId==3)
			 controlId = 6;
		else if(endId==5)
			 controlId = 9;
		else if(endId==6)
			 controlId = 11;
		else if(endId==8)
			 controlId = 15;
		else if(endId==7)
			 controlId = 12;
		else if(endId==2)
			 controlId = 7;
	break;

	case 5:	 
		if(endId==3)
			 controlId = 5;
		else if(endId==4)
			 controlId = 9;
		else if(endId==6)
			 controlId = 10;
	break;

	case 6:	 
		if(endId==5)
			 controlId = 10;
		else if(endId==4)
			 controlId = 11;
		else if(endId==8)
			 controlId = 14;
	break;

	case 7:	 
		if(endId==2)
			 controlId = 8;
		else if(endId==4)
			 controlId = 12;
		else if(endId==8)
			 controlId = 13;
	break;

	case 8:	 
		if(endId==4)
			 controlId = 15;
		else if(endId==6)
			 controlId = 14;
		else if(endId==7)
			 controlId = 13;
	break;
	default:
		break;
	}

	bzControlPt->SetXY(ControlPts[controlId*2], ControlPts[controlId*2+1]);
}

CPoint L1_S4_PZPainting::GetBezierPoint(CPoint* p1,CPoint* c1,CPoint* p2,float t)
{
	CPoint	bt;

	bt.x = (1 - t) * (1 - t) * p1->x + 2 * t * (1 - t) * c1->x + t * t * p2->x;
	bt.y = (1 - t) * (1 - t) * p1->y + 2 * t * (1 - t) * c1->y + t * t * p2->y;

	return bt;
}

float L1_S4_PZPainting::easing(float t,float b,float c,float d)
{
	//t is the current time (or position) of the tween. This can be seconds or frames, steps, seconds, ms, whatever as long as the unit is the same as is used for the total time [3].
	//b is the beginning value of the property.
	//c is the change between the beginning and destination value of the property.
	//d is the total time of the tween.
	return (c * t / d + b);
}

void L1_S4_PZPainting::ProcessSkip()
{
	if( !IsEndAnimPlaying && !ProfileSystem::Instance->ActiveProfile.L1S4_PaintingPuzzleSolved )
	{
		//Set all to correct positions
		for (int i = 0; i < MAX_PUZ_PIECES; i++)
		{
			PuzPieces[i]->SetPos( PointSystem::CreateCPoint(PuzPosArray[i]->x,PuzPosArray[i]->y,0) );
		}
		
		Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4PaintingPuzzleSolved);

		if( Hud->IsPuzzleScene )
			Hud->InvBox_StopPuzzleMode();

		ProfileSystem::Instance->ActiveProfile.L1S4_PaintingPuzzleSolved = true;
		IsEndAnimPlaying = true;
		endGameVFX->Start();
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		LeftLockGlow->SetAlpha(0);
		LeftLockGlow->SetEnable(true);
		RightLockGlow->SetAlpha(0);
		RightLockGlow->SetEnable(true);

	}
	//Control::Audio->PlaySample(aSFXClick);
}


void L1_S4_PZPainting::Render(const CPoint* pCam)
{
	if( !Visible ) 
		return;
}

void L1_S4_PZPainting::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL1Hunted) )
	{
		Control::Audio->Samples[aTrackL1Hunted]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if(Control::Audio->GlobalMusicVol - musicFadeOutTimer<=0)
		{
			Control::Audio->Samples[aTrackL1Hunted]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL1Hunted);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
				Control::Audio->PlaySample(aTrackL1Puzzle, true);
		}
			musicFadeOutTimer += Control::LogicRate * 8;
	}
	else if( Control::Audio->IsPlaying(aTrackL1Mansion) )
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

	if( IsEndAnimPlaying )
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;

		endAnimTimer += Control::LogicRate;
		if(endAnimTimer<=2)
		{
			float val = Elixir::Sin((endAnimTimer/2.0f)*180);
			LeftLockGlow->SetAlpha(val);
			RightLockGlow->SetAlpha(val);
		}
		else
		{
			if(!isLockLeftOpened && endAnimTimer>2.5f)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4PaintingLocksOpen);

				isLockLeftOpened = true;
				LeftLock->FadeOut(4.0f);
				LeftLockOpened->FadeIn(4.0f);
				LeftLockOpened->SetEnable(true);
			}
			else if(!isLockRightOpened && endAnimTimer>3.0f)
			{
				isLockRightOpened = true;
				IsEndAnimPlaying = false;

				IsPaintingPuzzleSolved = true;

				RightLock->FadeOut(4.0f);
				RightLockOpened->FadeIn(4.0f);
				RightLockOpened->SetEnable(true);
			}
		}
	}

	if( Hud->PopupState != Popup_Inactive )
		return;
}

void L1_S4_PZPainting::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 1;
	do
	{
		switch( iNonHOHint )
		{
			default:
			
			case 0:	++iNonHOHint;
						//Always active exit coordinate!! - hintover never true
						InitHintTrailEmit(InvItemRectpainting.GetCenterX(), InvItemRectpainting.GetCenterY(),false);
						return;
					break;
		}
	}
	while(  iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L1_S4_PZPainting::ResetPuzzle()
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
			CurrentPos[0] = 4; CurrentPos[1] = 8; CurrentPos[2] = 5; CurrentPos[3] = 7;
			CurrentPos[4] = 2; CurrentPos[5] = 3; CurrentPos[6] = 0; CurrentPos[7] = 6;
			CurrentPos[8] = 1;

			ProfileSystem::Instance->ActiveProfile.L1S4_PaintingPuzzleSolved = false;
			ProfileSystem::Instance->ActiveProfile.L1S4_InvPaintingFrameTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoTurned = false;
			ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoPaperTeared = false;
			ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S4_InvBookCodeTaken = false;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_PZPainting;
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

void L1_S4_PZPainting::NavigateToScene()
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