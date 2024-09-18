//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S1_PZStarPuzzle.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

namespace Level7_Scene_1
{
	const CPoint DoorOpenOffset[] = {CPoint(-40, 28, 0),
									 CPoint(0, -40, 0),
									 CPoint(40, 28, 0)};

	SymbolForSelect::SymbolForSelect(CObject* obj)
	{
		LoadSprite(obj->Filename);
		SetPos(&obj->Pos);
		SetZ(obj->ZOrder);

		_selected = false;

		obj->SetEnable(false);
	}

	SymbolForSelect::~SymbolForSelect()
	{
		SAFE_DELETE(_boundArea);
	}

	void SymbolForSelect::OnCorrectSelection()
	{
		FadeOut(3.0f);
		_selected = true;
		Control::Audio->QuickLoadAndPlaySFX("s1pz1_tileclickedright");
	}

	void SymbolForSelect::OnWrongSelection()
	{
		Control::Audio->QuickLoadAndPlaySFX("s1pz1_tileclickedwrong");
		ForcePlayAnimation(1);
	}

	void SymbolForSelect::Reset()
	{
		if (_selected)
			FadeIn(5.0f);

		_selected = false;
	}

		

	
	StarColor::StarColor(CObject *obj, SymbolColor color)
	{
		LoadSprite(obj->Filename);
		SetPos(&obj->Pos);
		SetZ(obj->ZOrder);

		_color = color;
		_selectedCount = 0;
		_isDoorsOpened = false;
		_isSymbolComplete = false;
		_isCollected = false;
		_doorOpenIndex = 0;
		_isActive = false;

		obj->SetEnable(false);
	}

	StarColor::~StarColor()
	{

	}

	void StarColor::setDoorObject(CObject* obj)
	{
		_doors.push_back(obj);
	}

	void StarColor::AddSymbol(SymbolForSelect* symbol)
	{
		_symbolList.push_back(symbol);
	}

	void StarColor::Update()
	{
		if (!_isActive)
			return;

		CPoint mPos = Control::Input->Mpos();

		if (Control::Input->LBclicked())
		{
			OnClickRegistered(mPos);
		}
	}

	void StarColor::OnClickRegistered(CPoint mPos)
	{
		if (_isDoorsOpened)
		{
			if (GetLocalRect().Intersection(&mPos))
			{
				FadeOut(1);
				_isCollected = true;
			}
		}

		if (_isSymbolComplete)
			return;

		for (int i = 0; i < (int)_symbolList.size(); i++)
		{
			if (_symbolList[i]->getBoundArea()->Intersection(&mPos) )
			{
				if(_symbolList[i]->getIndex() == _selectedCount)
				{
					_symbolList[i]->OnCorrectSelection();
					_selectedCount++;
				}
				else
				{
					_symbolList[i]->OnWrongSelection();
					Reset();
				}
			}
		}

		if((int)_symbolList.size() == _selectedCount)
		{
			_isSymbolComplete = true;
			ProfileSystem::Instance->ActiveProfile.L7_S1_PZStarDoorOpened[_index] = true;
			OpenDoor();
			Control::Audio->QuickLoadAndPlaySFX("s1pz1_stardooropen");
		}

	}

	void StarColor::OpenDoor()
	{
		_doors[_doorOpenIndex]->TweenPosTo(DoorOpenOffset[_index].x, DoorOpenOffset[_index].y, 0, 1, 0, true, CC_CALLBACK_1(StarColor::OpenDoorTweenComplete, this));
	}

	void StarColor::OpenDoorTweenComplete(CObject* obj)
	{
		_doors[_doorOpenIndex]->Show(false);

		if (++_doorOpenIndex == (int)_doors.size())
		{
			_isDoorsOpened = true;
		}
		else
		{
			OpenDoor();
		}
	}

	void StarColor::Reset()
	{
		for (int i = 0; i < (int)_symbolList.size(); i++)
		{
			_symbolList[i]->Reset();
		}

		_selectedCount = 0;
	}

	void StarColor::ProcessHint(HudControl* Hud)
	{
		for (int i = 0; i < (int)_symbolList.size(); i++)
		{
			if (_symbolList[i]->getSelected())
				continue;

			Hud->InitHintTrailEmit(_symbolList[i]->getBoundArea()->center.x, _symbolList[i]->getBoundArea()->center.y);
			return;
		}
	}

}


L7_S1_PZStarPuzzle::L7_S1_PZStarPuzzle()
{

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S1\\PZ2\\PZ2.SCN");
	
	_collectedStarsCount = 0;
	_isAnyActivePZ = false;
	_isAnyPZCompleted = false;

	InitPuzzleObj();

	HOScene->GetObjectByID("starglowtopLeft")->Animate(0.7f, 1.0f, 2);
	HOScene->GetObjectByID("starglowtopRight")->Animate(0.7f, 1.0f, 2);
	HOScene->GetObjectByID("starglowbotleft")->Animate(0.7f, 1.0f, 2);
	HOScene->GetObjectByID("starglowbotRight")->Animate(0.7f, 1.0f, 2);
	HOScene->GetObjectByID("starglowbottom")->Animate(0.7f, 1.0f, 2);

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s1VolcanoAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s1VolcanoAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s1VolcanoAmb,true);
	}
	
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S1_PZStarPuzzle]  = true;
}

void L7_S1_PZStarPuzzle::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL7theme) )
	{
		Control::Audio->Samples[aTrackL7theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL7theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL7theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL7puzzle) )
				  Control::Audio->PlaySample(aTrackL7puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL7puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL7puzzle, true);
		}
	}
}

void L7_S1_PZStarPuzzle::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	
	for (int i = 0; i < (int)_stars.size(); i++)
	{
		if (_stars[i]->getIsDoorsOpened())
		{
			if (!_stars[i]->getIsCollected())
			{
				if (_stars[i]->GetWorldRect().Intersection(&pos))
				{
					Cursor::SetMode(CA_HandTake);
					if (Control::Input->LBclicked())
					{
						_stars[i]->FadeOut(1);
						_stars[i]->setIsCollected(true);
						ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[i] = true;
						_collectedStarsCount++;

						switch (i)
						{
						case 0:
							Hud->TakeInventoryItem(eInv_L7_S1_YellowStarEmblem);
							ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S3_Haven] = true;
							break;
						case 1:
							Hud->TakeInventoryItem(eInv_L7_S1_RedStarEmblem);
							ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano] = true;
							break;
						case 2:
							Hud->TakeInventoryItem(eInv_L7_S1_BlueStarEmblem);
							ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] = true;
							break;
						default:
							break;
						}
					}
				}
			}
		}
		else
		{
			_stars[i]->Update();
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L7_S1_Volcano);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L7_S1_Volcano);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S1_Volcano, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}

	if (Control::Input->LBclicked())
	{
		CheckAndUpdateStatus();
	}
}

void L7_S1_PZStarPuzzle::CheckAndUpdateStatus()
{
	_isAnyActivePZ = false;
	bool isAllCompleted = true;

	for (int i = 0; i < (int)_stars.size(); i++)
	{
		if (!_stars[i]->getIsDoorsOpened())
		{
			isAllCompleted = false;

			if(!_isAnyActivePZ && _stars[i]->getIsActive())
				_isAnyActivePZ = true;
		}
		else
		{
			_isAnyPZCompleted = true;
		}
	}

	if (isAllCompleted)
		return;

	if (!_isAnyPZCompleted && _isAnyActivePZ)
	{
		Hud->ShowBannerText(Banner_L7_S1_EnetrPuzzleWithSolutions);
	}

	if(!_isAnyPZCompleted && !_isAnyActivePZ)
	{
		Hud->ShowBannerText(Banner_L7_S1_ClickingOnPuzzleWithoutSolutions);
	}

	if (_isAnyPZCompleted && !_isAnyActivePZ)
	{
		Hud->ShowBannerText(Banner_L7_S1_ClickingOnPuzzleAfterCompletingOnePuzzle);
	}
}

void L7_S1_PZStarPuzzle::ReceiveMessage(int val)
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
		if (_collectedStarsCount == (int)_stars.size())
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
			SendMessage(ControlExit, ID, HOSM_NewArea);		
		}
		else
		{
			CheckAndUpdateStatus();
		}
	}
}

void L7_S1_PZStarPuzzle::ProcessSkip()
{
}

void L7_S1_PZStarPuzzle::ProcessNonHOHint()
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

		case 0: 
			{
				++iNonHOHint;
				for (int i = 0; i < (int)_stars.size(); i++)
				{
					if (_stars[i]->getIsDoorsOpened() &&
						!_stars[i]->getIsCollected())
					{
						//Hud->InitHintTrailEmit(&_stars[i]->GetWorldRect())
						// Linux: gives error of taking address of temporary also potential crash factor
						// below is fix
						CRectangle starsObjRect =_stars[i]->GetWorldRect();
						Hud->InitHintTrailEmit(&starsObjRect);
						return;
					}
				}	
			}
			break;

		case 1: 
			{
				++iNonHOHint;
				for (int i = 0; i < (int)_stars.size(); i++)
				{
					if (_stars[i]->getIsActive() &&
						!_stars[i]->getIsDoorsOpened())
					{
						_stars[i]->ProcessHint(Hud);
						return;
					}
				}

				Hud->InitHintTrailEmit(&HoveredBackBtnRectMax, true, CA_ExitDown);
				return;
			}
			break;

		case 2: ++iNonHOHint;

			break;
		case 3: ++iNonHOHint;

			break;
		case 4: ++iNonHOHint;

			break;
		case 5: ++iNonHOHint;

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

L7_S1_PZStarPuzzle::~L7_S1_PZStarPuzzle()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s1VolcanoAmb);
	Control::Audio->UnloadSample(aAmbL7s1VolcanoAmb);
	//MUSIC_SFX
}

void L7_S1_PZStarPuzzle::InitPuzzleObj()
{
	setPolyArray();
	
	bool activeArray[] = {ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted, 
						  ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted, 
						  ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted};

	for (int i = 0; i < 3; i++)
	{
		CObject* starObj = HOScene->GetObjectByID("Star_"+MKSTR(i));

		StarColor* star = new StarColor(starObj, SymbolColor(i));
		star->setIndex(i);
		star->setIsActive(activeArray[i]);
		_stars.push_back(star);
		HOScene->PushObject(star);

		if (ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[i])
		{
			_collectedStarsCount++;
			star->setIsCollected(true);
			star->Show(false);
		}

		bool doorOpened = false;
		if (ProfileSystem::Instance->ActiveProfile.L7_S1_PZStarDoorOpened[i])
		{
			star->setIsDoorsOpened(true);
			doorOpened = true;
		}

		for (int j = 0; j < 4; j++)
		{
			CObject* symObj = HOScene->GetObjectByID("Symbol_"+MKSTR(i)+"_"+MKSTR(j));

			SymbolForSelect* symbol = new SymbolForSelect(symObj);
			symbol->setIndex(j);
			symbol->setBoundArea(polyArray[i][j]);
			star->AddSymbol(symbol);
			HOScene->PushObject(symbol);

			if(doorOpened || !star->getIsActive())
				symbol->Show(false);
		}

		for (int j = 2; j >= 0; j--)
		{
			CObject*door = HOScene->GetObjectByID("Door_"+MKSTR(i)+"_"+MKSTR(j));
			star->setDoorObject(door);

			if (doorOpened)
				door->Show(false);
		}
	}

	CheckAndUpdateStatus();
}

void L7_S1_PZStarPuzzle::setPolyArray()
{
	int ArrayPts0[] = {379,354,368,363,361,384,392,393,426,388,424,361,428,353};
	CPolygon *CPoly0=  new CPolygon(7);
	CPoly0->Init(ArrayPts0);
	polyArray[0][0] = CPoly0; 

	int ArrayPts1[] = {430,354,429,385,444,397,477,399,484,357};
	CPolygon *CPoly1=  new CPolygon(5);
	CPoly1->Init(ArrayPts1);
	polyArray[0][1] = CPoly1;
	
	int ArrayPts2[] = {388,439,412,473,456,506,482,468,413,430};
	CPolygon *CPoly2=  new CPolygon(5);
	CPoly2->Init(ArrayPts2);
	polyArray[0][2] = CPoly2;
	
	
	int ArrayPts3[] = {522,387,510,450,535,461,571,354};
	CPolygon *CPoly3=  new CPolygon(4);
	CPoly3->Init(ArrayPts3);
	polyArray[0][3] = CPoly3;
	
	int ArrayPts4[] = {635,180,657,182,662,192,683,182,688,148,678,130,654,124};
	CPolygon *CPoly4=  new CPolygon(7);
	CPoly4->Init(ArrayPts4);
	polyArray[1][0] = CPoly4;
	
	int ArrayPts5[] = {596,299,623,282,636,304,674,288,671,256,643,232,617,231};
	CPolygon *CPoly5=  new CPolygon(7);
	CPoly5->Init(ArrayPts5);
	polyArray[1][1] = CPoly5;
	
	int ArrayPts6[] = {676,286,700,301,730,288,728,260,744,246,732,213,721,219,695,216,675,252};
	CPolygon *CPoly6=  new CPolygon(9);
	CPoly6->Init(ArrayPts6);
	polyArray[1][2] = CPoly6;
	
	int ArrayPts7[] = {642,230,663,190,683,183,694,213,671,251};
	CPolygon *CPoly7=  new CPolygon(5);
	CPoly7->Init(ArrayPts7);
	polyArray[1][3] = CPoly7;
	
	int ArrayPts8[] = {805,421,863,412,877,389,859,371,865,353,784,352};
	CPolygon *CPoly8=  new CPolygon(6);
	CPoly8->Init(ArrayPts8);
	polyArray[2][0] = CPoly8;
	
	
	int ArrayPts9[] = {866,413,878,388,938,375,948,394,934,417,889,433};
	CPolygon *CPoly9=  new CPolygon(6);
	CPoly9->Init(ArrayPts9);
	polyArray[2][1] = CPoly9;
	
	int ArrayPts10[] = {832,496,842,466,898,466,880,490,887,517,853,544};
	CPolygon *CPoly10=  new CPolygon(6);
	CPoly10->Init(ArrayPts10);
	polyArray[2][2] = CPoly10;
	
	int ArrayPts11[] = {914,428,932,420,960,424,967,437,985,444,928,486,932,455};
	CPolygon *CPoly11=  new CPolygon(7);
	CPoly11->Init(ArrayPts11);
	polyArray[2][3] = CPoly11;
}
void L7_S1_PZStarPuzzle::NavigateToScene()
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