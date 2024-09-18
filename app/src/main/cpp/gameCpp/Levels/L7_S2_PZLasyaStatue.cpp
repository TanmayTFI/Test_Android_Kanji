//====================================
//  created by : Jithin (+ Faisal)
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S2_PZLasyaStatue.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Util.h"
#include "Hud.h"

const CRectangle  InvDestRectLasyaStatue(221.0f,29.0f,938.0f,548.0f);
const CRectangle  StatuePlaceRect(331.0f,257.0f,199.0f,215.0f);

const CRectangle  BtnRectUpArrow(1014.0f,2.0f,136.0f,84.0f);
const CRectangle  BtnRectDownArrow(1018.0f,478.0f,132.0f,72.0f);

const int OffsetDistance = 348;

const int StartPosYArray[3] = {0, OffsetDistance, OffsetDistance*2};

const int itemActiveID[MAX_L7S2_LASYAITEMS] = 
{
	eOffset_DOWN, //0
	eOffset_DOWN, //1
	eOffset_DOWN, //2
	
	eOffset_DOWNMID, //3
	eOffset_MIDDLE, //4
	eOffset_DOWNMID, //5

	eOffset_MIDTOP, //6
	eOffset_MIDTOP, //7
	eOffset_DOWNMID, //8

	eOffset_TOP, //9
	eOffset_TOP, //10
	eOffset_TOP, //11
	eOffset_MIDTOP, //12
	eOffset_TOP //13
};

L7_S2_PZLasyaStatue::L7_S2_PZLasyaStatue()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S2\\PZ2\\DOWN.SCN");

	TopScene = new CHOScene();
	TopScene->Load("LV7\\S2\\PZ2\\UP.SCN");
	TopScene->SaveXY();

	TopScene->GetObjectByID("Mask")->SetAlpha(0);

	s1StatuePzClickFX = new SuperFX("PRT\\L7\\s1StatuePzClick.INI", PointSystem::CreateCPoint(603,448,0), 1100); 

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	_vdo_teleport = new CVideo("OGV\\L7\\S2ESCAPEPRISON.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//_vdo_teleport->SetScale(1.334f);
	_vdo_teleport->SetScale(GFApp->video_aspectRatio);
	_vdo_teleport->SetSkipActive();
	vidTimer = 0;

	//shift after setting mesh
	TopScene->ShiftXY(0,-768);

	//Init Poly
	int Pts0[] = {741,418,753,414,761,459,751,473,641,506,614,498,603,470,610,455,622,473,665,449,673,464,715,455,718,480,739,475,731,450};
	itemPoly[0]=  new CPolygon(15);
	itemPoly[0]->Init(Pts0);
	int Pts1[] = {912,495,903,503,914,530,936,534,958,527,965,501,951,499,950,509,938,515,923,507};
	itemPoly[1]=  new CPolygon(10);
	itemPoly[1]->Init(Pts1);
	int Pts2[] = {726,398,734,409,730,418,707,425,700,408,704,401,719,396,712,362,700,364,689,358,691,344,717,335,722,343,717,361};
	itemPoly[2]=  new CPolygon(14);
	itemPoly[2]->Init(Pts2);
	
	//Mid Poly
	int Pts3[] = {983,590,995,594,995,613,974,638,953,635,906,587,867,480,847,339,845,318,859,322,859,340,864,365,876,432,893,493,920,566,954,604,968,613,979,600};
	itemPoly[3]=  new CPolygon(18);
	itemPoly[3]->Init(Pts3);
	int Pts4[] = {859,290,878,291,886,311,887,328,872,341,851,342,834,327,834,300};
	itemPoly[4]=  new CPolygon(8);
	itemPoly[4]->Init(Pts4);
	int Pts5[] = {614,262,637,257,645,269,641,287,647,317,655,325,653,342,658,378,665,385,664,403,641,409,635,393,637,387,657,381,649,343,637,346,626,344,623,330,627,326,644,322,637,283,619,287,611,275};
	itemPoly[5]=  new CPolygon(23);
	itemPoly[5]->Init(Pts5);
	int Pts6[] = {861,190,889,184,910,180,927,193,934,222,937,251,918,268,877,259,853,230};
	itemPoly[6]=  new CPolygon(9);
	itemPoly[6]->Init(Pts6);
	int Pts7[] = {989,66,973,66,972,51,988,20,1010,9,1030,9,1030,26,1026,33,1018,27,1001,35,986,52};
	itemPoly[7]=  new CPolygon(11);
	itemPoly[7]->Init(Pts7);
	int Pts8[] = {795,288,806,302,740,378,703,397,666,403,658,376,677,348,690,353,673,380,686,385,729,358};
	itemPoly[8]=  new CPolygon(11);
	itemPoly[8]->Init(Pts8);

	//TOP Poly
	int Pts9[] = {567,269,579,273,589,299,582,335,568,340,549,318,543,281,551,274};
	itemPoly[9]=  new CPolygon(8);
	itemPoly[9]->Init(Pts9);
	int Pts10[] = {802,275,811,273,811,295,821,310,835,312,854,290,860,270,868,273,867,296,851,325,839,332,836,344,820,341,817,329,800,310,800,291};
	itemPoly[10]=  new CPolygon(16);
	itemPoly[10]->Init(Pts10);
	int Pts11[] = {918,291,948,289,978,304,974,324,955,342,899,346,898,324,903,301};
	itemPoly[11]=  new CPolygon(8);
	itemPoly[11]->Init(Pts11);
	int Pts12[] = {547,384,547,361,520,300,526,264,522,247,539,208,537,153,562,203,593,221,597,238,607,259,620,293,610,327,617,367,608,368,592,340,596,295,590,265,561,253,541,270,537,301,551,332,560,350,578,345,593,348,582,349,565,359,553,385};
	itemPoly[12]=  new CPolygon(28);
	itemPoly[12]->Init(Pts12);
	int Pts13[] = {740,72,765,78,771,112,803,98,787,122,804,129,824,124,850,113,842,93,864,73,864,94,888,120,878,133,866,139,846,130,788,147,756,137,746,121,750,99,736,81};
	itemPoly[13]=  new CPolygon(20);
	itemPoly[13]->Init(Pts13);

	numDifferencesFound = 0;

	//get The Items Objects
	//0-3 from HOScene, 4-13 from TopScene
	for (int i = 0; i < MAX_L7S2_LASYAITEMS; i++)
	{
		if(i<4)
			DiffItemsArray[i] = HOScene->GetObjectByID(std::string("item" + MKSTR(i)).c_str());
		else
			DiffItemsArray[i] = TopScene->GetObjectByID(std::string("item" + MKSTR(i)).c_str());

		if(ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaDiffFound[i])
		{
			DiffItemsArray[i]->Show(false);
			numDifferencesFound += 1;
		}
	}

	offsetID = eOffset_DOWN;
	startPosY = 0;
	endPosY = 0;

	ScrollTimer = 0;
	ScrollTime = 1.5f;
	_teleportVdoPlaying = false;

	IsScreenScrolling = false;

	statueOnHand = new CObject();
	statueOnHand->LoadInit("LV7\\S2\\PZ2\\statueonhand.SPR", 326, 327, 9);
	statueOnHand->Show(false);

	s2difffoundFX = new SuperFX("PRT\\L7\\s2difffound.INI", PointSystem::CreateCPoint(314,480,0), 100); 

	arrowUp = new CObject();
	arrowUp->LoadInit("LV7\\S2\\PZ2\\arrowup.SPR", 1082, 35, 9);

	arrowDown = new CObject();
	arrowDown->LoadInit("LV7\\S2\\PZ2\\arrowdown.SPR", 1086, 512, 9);

	if(offsetID == eOffset_DOWN)
		arrowDown->Show(false);
	else if(offsetID == eOffset_TOP)
		arrowUp->Show(false);

	txtInfo = NULL;
	
	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	float _lineWidth = 700;
	int _nLines = 3;
	EFont _font = eFont_26;
	eTextAlignment _align = Align_Center;
	std::string temp = "Ex14diff";
	if(StringTable::IsKeyFound(temp))
	{
		_lineWidth = StringTable::GetLineWidth(temp);
		_nLines = StringTable::GetNoOfLines(temp);
		_font = (EFont)StringTable::GetFont(temp);
	}
	txtInfo = new MultilineText(_nLines, PointSystem::CreateCPoint(680.0f, 700.0f,0), eZOrder_INVBOXITEMS, gold, 30, _lineWidth, _align, _font);
	//txtInfo->SetTextProps("0 out of 14", 560.0f, 716.0f, eZOrder_INVBOXITEMS, gold, eFont_26, Align_Center);
	txtInfo->SetTextMultiline(StringTable::getStr("Ex14diff")+MKSTR(MAX_L7S2_LASYAITEMS-numDifferencesFound)+")");
	txtInfo->Show(false);

	if(ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePlaced)
	{
		//Activate HO
		ActivateHO();
	}

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s2LasyaPrisonAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s2LasyaPrisonAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s2LasyaPrisonAmb,true);
	}
	
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S2_PZLasyaStatue]  = true;
}

void L7_S2_PZLasyaStatue::ActivateHO()
{
	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;

	txtInfo->Show(true);

	statueOnHand->Show(true);
}

void L7_S2_PZLasyaStatue::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(IsScreenScrolling)
	{
		updateScrolling();
	}

	if(ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePuzzleCompleted)
	{
		if(!IsScreenScrolling)
			ScrollTimer += Control::LogicRate;

		//Effect here and return to haven scene
		if (_teleportVdoPlaying)
		{
#ifdef FORCE_CLOSE_VIDEO
			vidTimer += Control::LogicRate;
#endif
			if(
#ifndef FORCE_CLOSE_VID_END
				_vdo_teleport->isEndReached() ||
#endif
				vidTimer > 11)
			{
				Hud->AllowInput = true;
				Hud->ShowHud(false);
				//unlock Volcano
				ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L7_S1_Volcano] = false;
				//lock Lasya prison
				ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L7_S2_LasyaPrison] = true;

				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
				SendMessage(ControlExit, ID, HOSM_NewArea);
				_teleportVdoPlaying = false;
			}
			return;
		}
		
		if(ScrollTimer>2.0f)
		{
			_vdo_teleport->PlayFadeInVideo(2);
			Hud->EnableInput(false);
			Hud->HideHud(false);
			_teleportVdoPlaying = true;
		}
		return;
	}

	if(_teleportVdoPlaying)
		return;

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

void L7_S2_PZLasyaStatue::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();
	Cursor::SetMode(CA_Normal);

	if(IsScreenScrolling)
	{
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePuzzleCompleted)
	{
		return;
	}

	arrowUp->PlayAnimation(0);
	arrowDown->PlayAnimation(0);
	if(arrowUp->Visible && BtnRectUpArrow.Intersection(&pos))
	{
		arrowUp->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(clicked)
		{
			arrowUp->PlayAnimation(0);
			IsScreenScrolling = true;

			startPosY = StartPosYArray[offsetID];
			endPosY = startPosY+OffsetDistance;

			offsetID += 1;
			if(offsetID == eOffset_TOP)
				arrowUp->Show(false);

			arrowDown->Show(true);
		}
		return;
	}
	else if(arrowDown->Visible && BtnRectDownArrow.Intersection(&pos))
	{
		arrowDown->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(clicked)
		{
			arrowDown->PlayAnimation(0);

			IsScreenScrolling = true;

			startPosY = StartPosYArray[offsetID];
			endPosY = startPosY-OffsetDistance;

			offsetID -= 1;
			if(offsetID == eOffset_DOWN)
				arrowDown->Show(false);

			arrowUp->Show(true);
		}
		return;
	}

	bool isItemClicked = false;

	if(!ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePlaced)
	{
		if(InvDestRectLasyaStatue.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(clicked)
			{
				Hud->ShowBannerText("L7statue");
			}
			return;
		}
	}
	else if(!IsScreenScrolling)
	{
		//Handle Click Update
		if(clicked)
			isItemClicked = HandleItemClick(pos);
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON , need to check the game play and finalize
	if (!isItemClicked && Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif
	if( !isItemClicked && IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown, eArea_L7_S2_LasyaPrison);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L7_S2_LasyaPrison);
#endif
		if( clicked )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S2_LasyaPrison, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	}
}

bool L7_S2_PZLasyaStatue::HandleItemClick(CPoint pos)
{
	int startID, endID;
	//traverse through limited items only
	//Sac : but there are more possible enums used, and this means two of the left out enums can have
	// startID and endID as un defined.. so I'm initialising it anyway before this if
	startID = 0;
	endID = 0;
	//traverse through limited items only
	if(offsetID == eOffset_DOWN)
	{
		startID = 0;
		endID = 8;
	}
	else if(offsetID == eOffset_MIDDLE)
	{
		startID = 3;
		endID = 12;
	}
	else if(offsetID == eOffset_TOP)
	{
		startID = 6;
		endID = 13;
	}

	int offsetY = 0;
	for (int i = startID; i <= endID; i++)
	{
		offsetY = 0;
		if(offsetID == eOffset_DOWN)
		{
			if(itemActiveID[i] == eOffset_DOWNMID)
			{
				offsetY = -OffsetDistance;
				//SLOG::Log("item is offset DOWNMID Shift this:", i);
			}
		}
		else if(offsetID == eOffset_MIDDLE)
		{
			if(itemActiveID[i] == eOffset_MIDTOP && i!=6 && i!=7)
			{
				offsetY = -OffsetDistance;
				//SLOG::Log("item is offset MIDTOP Shift this:", i);
			}
		}
		else if(offsetID == eOffset_TOP)
		{
			if(itemActiveID[i] == eOffset_MIDTOP && i!=12)
			{
				offsetY = OffsetDistance;
				//SLOG::Log("item is offset MIDTOP Shift this:", i);
			}
		}

		if( !ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaDiffFound[i] && itemPoly[i]->Intersection(&pos, 0, offsetY) )
		{
			Control::Audio->QuickLoadAndPlaySFX("s2pz2_itemclicked");

			ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaDiffFound[i] = true;
			//SLOG::Log("clicked on id:", i);
			DiffItemsArray[i]->FadeOut(1.25f);
			numDifferencesFound += 1;
			//s1StatuePzClickFX->SetPos(&CPoint(pos.x, pos.y, 0));
			// Linux: gives error of taking address of temporary also potential crash factor
			// below is fix
			CPoint tempPos =CPoint(pos.x, pos.y, 0);
			s1StatuePzClickFX->SetPos(&tempPos);
			s1StatuePzClickFX->Start();

			txtInfo->SetTextMultiline(StringTable::getStr("Ex14diff")+MKSTR(MAX_L7S2_LASYAITEMS-numDifferencesFound)+")");

			if( numDifferencesFound >= MAX_L7S2_LASYAITEMS )
			{
				if (_handleExtrasGameComplete(false))
				{
					return false;
				}

				Control::Audio->QuickLoadAndPlaySFX("s2pz2_puzzlesolved");

				ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePuzzleCompleted = true;
				Hud->InvBoxCtrl->SetLockBase(INVBOX_LR_FRAME);
				txtInfo->Show(false);

				AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);

				statueOnHand->FadeOut(1.25f);
				arrowDown->Show(false);
				arrowUp->Show(false);
			
				if(offsetID != eOffset_TOP)
				{
					
					IsScreenScrolling = true;

					startPosY = StartPosYArray[offsetID];
					endPosY = 768;

					ScrollTime = 3.5f;
				}
			}
			return true;
		}
	}

	return false;
}

void L7_S2_PZLasyaStatue::updateScrolling()
{
	ScrollTimer += Control::LogicRate;

	float newPos = (startPosY + (endPosY-startPosY)*easing(ScrollTimer,0,1,ScrollTime));
	HOScene->ShiftXY(0, newPos);

	//Move Top scene also
	newPos -= 768;
	TopScene->ShiftXY(0,newPos);

	if(ScrollTimer>ScrollTime)
	{
		IsScreenScrolling = false;
		ScrollTimer = 0;
	}
}

float L7_S2_PZLasyaStatue::easing(float t,float b,float c,float d)
{
	static const float s=1.60158f;
	// return c*((t=t/d-1.0f)*t*((s+1.0f)*t + s) + 1.0f) + b;
	//Sac: this could be problematic, as we are assigning t and also using it for multiplication in same line
	// we better assign it first and then use it for multiplication
	//modifying as below...
	t=t/d-1.0f;
	return c*(t*t*((s+1.0f)*t + s) + 1.0f) + b;
}

void L7_S2_PZLasyaStatue::ReceiveMessage(int val)
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
			if(itemVal == eInv_L7_S2_LasyaStatue && !ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePlaced && InvDestRectLasyaStatue.Intersection(&dropPos))
			{
				ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePlaced = true;

				Control::Audio->QuickLoadAndPlaySFX("s2pz2_statueplaced");

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S2_LasyaStatue] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S2_LasyaStatue);

				ActivateHO();
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L7_S2_PZLasyaStatue::ProcessSkip()
{
}

void L7_S2_PZLasyaStatue::ShowHintForItems()
{
	int startID, endID;

	//traverse through limited items only
	//Sac : but there are more possible enums used, and this means two of the left out enums can have
	// startID and endID as un defined.. so I'm initialising it anyway before this if
	startID = 0;
	endID = 0;

	if(offsetID == eOffset_DOWN)
	{
		startID = 0;
		endID = 8;
	}
	else if(offsetID == eOffset_MIDDLE)
	{
		startID = 3;
		endID = 8;
	}
	else if(offsetID == eOffset_TOP)
	{
		startID = 6;
		endID = 13;
	}

	int offsetY = 0;
	for (int i = startID; i <= endID; i++)
	{
		offsetY = 0;
		if(offsetID == eOffset_DOWN)
		{
			if(itemActiveID[i] == eOffset_DOWNMID)
			{
				offsetY = -OffsetDistance;
				//SLOG::Log("item is offset DOWNMID Shift this:", i);
			}
		}
		else if(offsetID == eOffset_TOP)
		{
			if(itemActiveID[i] == eOffset_MIDTOP && i!=12)				
			{
				offsetY = OffsetDistance;
				//SLOG::Log("item is offset MIDTOP Shift this:", i);
			}
		}
		
		bool isItemVisibleOnCurrentScreen =	(offsetID == eOffset_DOWN && (itemActiveID[i] == eOffset_DOWNMID || itemActiveID[i] == eOffset_DOWN)) ||
								(offsetID == eOffset_MIDDLE && (itemActiveID[i] == eOffset_DOWNMID || itemActiveID[i] == eOffset_MIDDLE || itemActiveID[i] == eOffset_MIDTOP)) ||
								(offsetID == eOffset_TOP && (itemActiveID[i] == eOffset_TOP || itemActiveID[i] == eOffset_MIDTOP));

		if( isItemVisibleOnCurrentScreen && !ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaDiffFound[i] )
		{
			Hud->InitHintTrailEmit(itemPoly[i]->center.x, itemPoly[i]->center.y+offsetY);
			return;
		}
	}

	if(offsetID == eOffset_DOWN)
	{
		Hud->InitHintTrailEmit(&BtnRectUpArrow,true);
		AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	}
	else if(offsetID == eOffset_TOP)
	{
		Hud->InitHintTrailEmit(&BtnRectDownArrow,true);
		AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	}
	else if(offsetID == eOffset_MIDDLE)
	{
		//Check whether to show to UP or DOWN
		bool isTopAllTaken = true;
		bool isBottomAllTaken = true;

		startID = 0;
		endID = 3;

		for (int i = startID; i <= endID; i++)
		{
			if( !ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaDiffFound[i] )
			{
				isBottomAllTaken = false;
				break;
			}
		}

		if(!isBottomAllTaken)
		{
			Hud->InitHintTrailEmit(&BtnRectDownArrow,true);
			AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			return;
		}

		startID = 6;
		endID = 13;

		for (int i = startID; i <= endID; i++)
		{
			if( !ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaDiffFound[i] )
			{
				isTopAllTaken = false;
				break;
			}
		}

		if(!isTopAllTaken)
		{
			Hud->InitHintTrailEmit(&BtnRectUpArrow,true);
			AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			return;
		}
	}
}

void L7_S2_PZLasyaStatue::ProcessNonHOHint()
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

			break;
		case 1: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePlaced && ProfileSystem::Instance->ActiveProfile.L7_S2_InvStatueTaken)
			{
				Hud->ShowHintForInvItem(eInv_L7_S2_LasyaStatue, &StatuePlaceRect);
				return;
			}
			break;
		case 2: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePlaced && !ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePuzzleCompleted)
			{
				ShowHintForItems();
				return;
			}
			else
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
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

L7_S2_PZLasyaStatue::~L7_S2_PZLasyaStatue()
{
	if(Hud->IsHOScene)
	{
		Hud->InvBoxCtrl->SetLockBase(INVBOX_LR_FRAME);
		Hud->IsHOScene = false;
		Hud->HOState = HOInactive;
	}

	for (int i = 0; i < MAX_L7S2_LASYAITEMS; i++)
	{
		SAFE_DELETE(itemPoly[i]);
	}

	SAFE_DELETE(s2difffoundFX);

	SAFE_DELETE(s1StatuePzClickFX);
	SAFE_DELETE(txtInfo);

	SAFE_DELETE(statueOnHand);
	SAFE_DELETE(arrowUp);
	SAFE_DELETE(arrowDown);

	SAFE_DELETE(TopScene);
	SAFE_DELETE(_vdo_teleport);

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s2LasyaPrisonAmb);
	Control::Audio->UnloadSample(aAmbL7s2LasyaPrisonAmb);
	//MUSIC_SFX
}
void L7_S2_PZLasyaStatue::NavigateToScene()
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