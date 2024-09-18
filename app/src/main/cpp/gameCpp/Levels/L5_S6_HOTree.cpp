//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S6_HOTree.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

const CRectangle  InvDestRectSoldierBadge(241.0f,391.0f,354.0f,204.0f);
bool isHOCollected[MAX_L5S6_HOOBJECTS] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false};

const float CLOUD_OVERFLOW_WIDTH = 225.0f;

const CRectangle  ActionRectChamaleon(962.0f,50.0f,58.0f,54.0f);

L5_S6_HOTree::L5_S6_HOTree()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S6\\HO\\HO.SCN");

	treeEyeTimer = 0;
	treeEyeTimerThreshold = (float)(10+rand()%20);

	HOScene->GetObjectByID("bluefgglow")->Animate(0.15f,0.85f,4.0f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("orangeglow")->Animate(0.15f,0.85f,4.25f, 2.25f, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("redglow")->Animate(0.15f,0.85f,3.75f, 0.25f, eBlend_AlphaAdditive);

	s6hofgblueplantFX = new SuperFX("PRT\\L5\\s6hofgblueplant.INI", PointSystem::CreateCPoint(684,564,0), 100); 
	s6hofgblueplantFX->Start();
	s6hofgblueplantFX->AdvanceByTime(2.0f);

	s6horedplantFX = new SuperFX("PRT\\L5\\s6horedplant.INI", PointSystem::CreateCPoint(867,187,0), 100); 
	s6horedplantFX->Start();
	s6horedplantFX->AdvanceByTime(2.0f);

	s6hosnowFX = new SuperFX("PRT\\L3\\s6hosnow.INI", PointSystem::CreateCPoint(851,15,0), 1100); 
	s6hosnowFX->Start();
	s6hosnowFX->AdvanceByTime(2.0f);

	s6hofogFX = new SuperFX("PRT\\L3\\s6hofog.INI", PointSystem::CreateCPoint(854,579,0), 1100); 
	s6hofogFX->Start();
	s6hofogFX->AdvanceByTime(2.0f);

	s6hoeyefxFX = new SuperFX("PRT\\L5\\s6hoeyefx.INI", PointSystem::CreateCPoint(714,241,0), 100); 
	s6hoeyefx2 = new SuperFX("PRT\\L5\\s6hoeyefx.INI", PointSystem::CreateCPoint(797,248,0), 100); 

	s6holeafFX = new SuperFX("PRT\\L5\\s6holeaf.INI", PointSystem::CreateCPoint(883,-256,0), 100); 
	s6holeafFX->Start();

	s6homanjaadifxFX = new SuperFX("PRT\\L5\\s6homanjaadifx.INI", PointSystem::CreateCPoint(935,488,0), 100);

	cloudBG1 = HOScene->GetObjectByID("cloudbg1");
	cloudBG2 = HOScene->GetObjectByID("cloudbg2");
	cloudBG2->SetScale(1.15f);

	cloudBG1->SetBlendMode(eBlend_AlphaAdditive);
	cloudBG2->SetBlendMode(eBlend_AlphaAdditive);

	cloudbgSpeed1 = 10;
	cloudbgSpeed2 = 11;
	
	cloudFG1 = HOScene->GetObjectByID("fgclouds1");
	cloudFG2 = HOScene->GetObjectByID("fgclouds2");
	cloudfgSpeed1 = 12;
	cloudfgSpeed2 = 14;

	cloudFG1->SetBlendMode(eBlend_AlphaAdditive);
	cloudFG2->SetBlendMode(eBlend_AlphaAdditive);
	
	HOScene->GetObjectByID("branch1")->SetPivot(74, -64);
	HOScene->GetObjectByID("branch1")->Animate(0,0,3,0,6.0f);

	HOScene->GetObjectByID("branch2")->SetPivot(-115, 4);
	HOScene->GetObjectByID("branch2")->Animate(0,0,2,0,5.0f);

	HOScene->GetObjectByID("branch4")->SetPivot(22, 107);
	HOScene->GetObjectByID("branch4")->Animate(0,0,3,0,6.0f);

	HOScene->GetObjectByID("fgsnaileyeL")->SetPivot(-6, 8);
	HOScene->GetObjectByID("fgsnaileyeL")->Animate(0,0,4,0,4.0f);

	HOScene->GetObjectByID("fgsnaileyeR")->SetPivot(-10, -7);
	HOScene->GetObjectByID("fgsnaileyeR")->Animate(0,0,5.0f,0,4.0f,2.57f);

	HOScene->GetObjectByID("leaf2")->SetPivot(-10, 10);
	HOScene->GetObjectByID("leaf2")->Animate(0,0,1,0,4.0f);

	HOScene->GetObjectByID("leaf3")->SetPivot(39, 18);
	HOScene->GetObjectByID("leaf3")->Animate(0,0,3,0,5.0f);

	HOScene->GetObjectByID("leaf3")->SetPivot(39, 18);
	HOScene->GetObjectByID("leaf3")->Animate(0,0,3,0,5.0f);

	HOScene->GetObjectByID("leaf4")->SetPivot(15, 16);
	HOScene->GetObjectByID("leaf4")->Animate(0,0,3,0,5.0f);

	HOScene->GetObjectByID("leaf5")->SetPivot(-14, -27);
	HOScene->GetObjectByID("leaf5")->Animate(0,0,3.25f,0,5.0f, 3.25f);

	//animals
	HOScene->GetObjectByID("raccoonhead")->Animate(0,2,2.0f,0,4.0f);

	HOScene->GetObjectByID("squirreltail")->SetPivot(26, -5);
	HOScene->GetObjectByID("squirreltail")->Animate(0,0,2.0f,0,4.0f);

	HOScene->GetObjectByID("squirrelhead")->SetPivot(-6, 14);
	HOScene->GetObjectByID("squirrelhead")->Animate(0,1,2.0f,0,3.2f);

	HOScene->GetObjectByID("lizardtail")->SetPivot(-7, -10);
	HOScene->GetObjectByID("lizardtail")->Animate(0,0,2.0f,0,3.2f);
	
	_faceMesh = new MeshObject(	HOScene->GetObjectByID("meshface"), 5, 5);
	_faceMesh->meshFX->SetSpeedAndDisplacement(2, 4, 1, 4);
	_faceMesh->meshFX->SetMeshBox(1, 1, 5, 5);

	isTreeReleaseCutscnPlaying = false;
	cv_TreeReleaseCutscn = NULL;
	vidTimer = 0;

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	cv_TreeReleaseCutscn = new CVideo("OGV\\L5\\S6TREERELEASE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//cv_TreeReleaseCutscn->SetScale(1.334f);	
	cv_TreeReleaseCutscn->SetScale(GFApp->video_aspectRatio);	

		//MUSIC_SFX
	Control::Audio->LoadSample(aVOL5TreeRefS6_1,	AudioVO);
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s6TreeHOAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s6TreeHOAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s6TreeHOAmb,true);
	}
	
	//MUSIC_SFX

	if(IsVisitedForFirstTime)
	{
		SetLockLevels(true);
	}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S6_HOTree]  = true;

	for (int i = 0; i < MAX_L5S6_HOOBJECTS; i++)
	{
		HOPolyArray[i] = NULL;
		HOScnObjects[i] = NULL;
		HOGlowObjects[i] = NULL;
		HOManjadiObjects[i] = NULL;
	}

	InitHO();

	if(ProfileSystem::Instance->ActiveProfile.L5_S6_HOCompleted)
	{
		isHOCompleted = true;
		hoCompleteTimer = 2.0f;
	}
}

void L5_S6_HOTree::SetLockLevels(bool lockLevel)
{
	//Lock all levels
	int levelsToLock[]={
		eArea_L5_S1_TigerDistant,
		eArea_L5_S2_TigerCloseup,
		eArea_L5_S3_Pond,
		eArea_L5_S4_Dungeon,
		eArea_L5_S5_MountainBelow,
		eArea_L5_S6_MountainTop
	};

	for (int i = 0; i < 6; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[levelsToLock[i]])
			ProfileSystem::Instance->ActiveProfile.IsAreaLocked[levelsToLock[i]] = lockLevel;
	}
}

void L5_S6_HOTree::InitHO()
{
	JAnim_SoldierBadgePlace = NULL;
	boxOpenVFX = NULL;
	isSoldierBadgePlacePlaying = false;

	isHOCompleted = false;
	hoCompleteTimer = 0;

	hoHelpText = NULL;

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	float _lineWidth = 700;
	int _nLines = 4;
	EFont _font = eFont_26;
	eTextAlignment _align = Align_Center;
	std::string temp = "Exboard";
	if(StringTable::IsKeyFound(temp))
	{
		_lineWidth = StringTable::GetLineWidth(temp);
		_nLines = StringTable::GetNoOfLines(temp);
		_font = (EFont)StringTable::GetFont(temp);
	}

	hoHelpText = new MultilineText(_nLines, PointSystem::CreateCPoint(683, 696, 0), eZOrder_INVBOXITEMS, gold, 30, _lineWidth, _align, _font);
	
	hoHelpText->SetTextMultiline(StringTable::getStr(temp));
	hoHelpText->Show(false);

	HOFoundFX = NULL;
	HOFoundFX = new SuperFX("PRT\\L5\\s6HOFX.INI", PointSystem::CreateCPoint(1065,122,0), 100);

	boxGlow1Butterfly	= HOScene->GetObjectByID("boxbutterflyglow");
	boxGlow2Nest		= HOScene->GetObjectByID("boxnestglow");
	boxGlow3Bird		= HOScene->GetObjectByID("boxbirdglow");
	boxGlow4Snake		= HOScene->GetObjectByID("boxsnakeglow");
	boxGlow5Nuts		= HOScene->GetObjectByID("boxnutglow");

	boxGlow1Butterfly->SetBlendMode(eBlend_AlphaAdditive);	
	boxGlow2Nest->SetBlendMode(eBlend_AlphaAdditive);	
	boxGlow3Bird->SetBlendMode(eBlend_AlphaAdditive);
	boxGlow4Snake->SetBlendMode(eBlend_AlphaAdditive);
	boxGlow5Nuts->SetBlendMode(eBlend_AlphaAdditive);

	boxGlow1Butterfly->Show(false);
	boxGlow2Nest->Show(false);
	boxGlow3Bird->Show(false);
	boxGlow4Snake->Show(false);
	boxGlow5Nuts->Show(false);

	manjaadiRiseup = HOScene->GetObjectByID("manjaadiriseup");
	manjaadiRiseup->SetScale(2.0f);
	manjaadiRiseup->Show(false);

	ManjadiFull = HOScene->GetObjectByID("manjaadifull1");
	ManjadiHalf = HOScene->GetObjectByID("manjaadifull2");
	ManjadiQuarter = HOScene->GetObjectByID("manjaadifull3");

	numObjCollected = 0;

	isDialogActive = false;
	dialogSystem = NULL;

	dialogDelayTimer = -1;

	if(ProfileSystem::Instance->ActiveProfile.L5_S6_HOSoldierBadgePlaced)
	{
		HOScene->GetObjectByID("soldierbadge")->SetEnable(false);
		HOScene->GetObjectByID("boxglow")->SetEnable(false);
		HOScene->GetObjectByID("boxcover")->SetEnable(false);

		hoHelpText->Show(true);

		Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
		Hud->IsHOScene = true;
	}
	else
	{
		std::vector<SuperFX*> superfxArray;
		boxOpenVFX = new SuperFX("PRT\\L5\\s6hoboxopen.INI", &HOScene->GetObjectByID("soldierbadge")->Pos, 100); 
		superfxArray.push_back(boxOpenVFX);

		JAnim_SoldierBadgePlace = new JSONAnimator(0, CC_CALLBACK_1(L5_S6_HOTree::onAnimationComplete, this));
		JAnim_SoldierBadgePlace->parseAndLoadJSON("ANIM\\L5\\S6HOBADGEPLACE.JSON", HOScene, &superfxArray);

		HOScene->GetObjectByID("soldierbadge")->SetEnable(false);
		HOScene->GetObjectByID("boxglow")->SetEnable(false);

		//if condition to add
		dialogSystem = new DialogSystem();
		AddControl(dialogSystem);

		if(!ProfileSystem::Instance->ActiveProfile.L5_S6_HOGuardianIntroTalkCompleted)
		{
			dialogDelayTimer = 1.5f;
			isDialogActive = true;
			if(Hud->IsDraggingItem())
				Hud->ReleaseDrag(); //Clear any inventory in hand
			Hud->AllowInput = false;
			Hud->HideHud(true);
		}
	}

#pragma region Polygon_Init
	int Pts0[] = {638,26,632,39,655,62,683,62,703,29,690,22,668,35};
	HOPolyArray[0]=  new CPolygon(7);
	HOPolyArray[0]->Init(Pts0);
	int Pts1[] = {643,532,618,567,626,580,638,582,635,592,641,594,689,578,689,568,660,563,655,537};
	HOPolyArray[1]=  new CPolygon(10);
	HOPolyArray[1]->Init(Pts1);
	int Pts2[] = {811,504,786,500,783,514,818,539,836,536,849,518,836,480,823,484};
	HOPolyArray[2]=  new CPolygon(8);
	HOPolyArray[2]->Init(Pts2);
	int Pts3[] = {392,147,383,171,363,192,359,217,381,240,384,253,399,253,403,238,413,209,400,173,398,150};
	HOPolyArray[3]=  new CPolygon(11);
	HOPolyArray[3]->Init(Pts3);
	int Pts4[] = {1135,180,1122,191,1120,214,1135,231,1151,216,1153,199,1146,196,1138,203,1135,189};
	HOPolyArray[4]=  new CPolygon(9);
	HOPolyArray[4]->Init(Pts4);
	int Pts5[] = {463,137,486,138,523,122,525,127,527,148,510,162,514,170,484,171,484,158,464,146};
	HOPolyArray[5]=  new CPolygon(10);
	HOPolyArray[5]->Init(Pts5);
	int Pts6[] = {1006,289,985,271,971,253,953,246,944,255,942,274,955,291,980,292,1002,297};
	HOPolyArray[6]=  new CPolygon(9);
	HOPolyArray[6]->Init(Pts6);
	int Pts7[] = {1128,507,1113,519,1091,543,1084,558,1092,564,1106,546,1121,546,1129,547,1145,519,1142,507};
	HOPolyArray[7]=  new CPolygon(10);
	HOPolyArray[7]->Init(Pts7);
	int Pts8[] = {620,437,647,437,655,452,652,471,631,479,629,464,617,456};
	HOPolyArray[8]=  new CPolygon(7);
	HOPolyArray[8]->Init(Pts8);

	int Pts9[] = {291,81,321,97,320,110,313,126,299,124,298,142,305,179,302,205,292,212,283,201,291,171,278,138,287,110,299,110,282,96};
	HOPolyArray[9]=  new CPolygon(15);
	HOPolyArray[9]->Init(Pts9);

	int Pts10[] = {931,126,981,126,985,134,995,177,960,207,954,199,966,188,961,167,934,164,940,185,930,190,902,162};
	HOPolyArray[10]=  new CPolygon(12);
	HOPolyArray[10]->Init(Pts10);
	int Pts11[] = {804,348,810,348,814,361,807,379,811,398,795,426,789,448,770,475,752,480,749,520,743,523,733,486,739,466,758,461,774,422,788,402};
	HOPolyArray[11]=  new CPolygon(16);
	HOPolyArray[11]->Init(Pts11);
	int Pts12[] = {1068,295,1058,306,1059,331,1068,336,1084,332,1085,309};
	HOPolyArray[12]=  new CPolygon(6);
	HOPolyArray[12]->Init(Pts12);
	int Pts13[] = {361,116,366,126,359,143,343,151,333,134,340,121,350,121};
	HOPolyArray[13]=  new CPolygon(7);
	HOPolyArray[13]->Init(Pts13); 
#pragma endregion

	//Objects Init
	for (int i = 0; i < MAX_L5S6_HOOBJECTS; i++)
	{
		HOScnObjects[i] = HOScene->GetObjectByID(std::string("hoobj" + MKSTR(i)).c_str());
		HOGlowObjects[i] = HOScene->GetObjectByID(std::string("hoobjglow" + MKSTR(i)).c_str());
		HOManjadiObjects[i] = HOScene->GetObjectByID(std::string("manjaadi" + MKSTR(i)).c_str());

		isHOCollected[i] = ProfileSystem::Instance->ActiveProfile.L5_S6_HOObjectFound[i];
		if(!isHOCollected[i])
		{
			HOGlowObjects[i]->Show(false);
			HOManjadiObjects[i]->Show(false);
		}
		else
		{
			numObjCollected += 1;
		}
	}

	ManjadiHalf->Show(false);
	ManjadiQuarter->Show(false);

	updateBoxSymbolGlow();
}

void L5_S6_HOTree::onAnimationComplete(int id)
{
	isSoldierBadgePlacePlaying = false;

	dialogSystem->ShowDialog(eDialog_L5S6TreeGuardianAfterBadgePlaced);
	isDialogActive = true;	
}


void L5_S6_HOTree::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(isTreeReleaseCutscnPlaying)
		return;

	if( Control::Audio->IsPlaying(aTrackL5theme) )
	{
		Control::Audio->Samples[aTrackL5theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL5theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL5theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL5ho) )
				  Control::Audio->PlaySample(aTrackL5ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL5ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL5ho, true);
		}
	}
}

void L5_S6_HOTree::Update()
{
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	if(isTreeReleaseCutscnPlaying)
	{
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_TreeReleaseCutscn->isEndReached() ||
#endif
			vidTimer > 3)
		{
			Hud->AllowInput = true;
			Hud->ShowHud(false);
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S6_MountainTop;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
		return;
	}

	if(isDialogActive)
	{
		if(dialogDelayTimer>0)
		{
			dialogDelayTimer -= Control::LogicRate;
			if(dialogDelayTimer<0)
			{
				dialogDelayTimer = -1;

				dialogSystem->ShowDialog(eDialog_L5S6TreeGuardian);
				isDialogActive = true;				
			}
		}
		else
			dialogSystem->Update();
		return;
	}

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if( ActionRectChamaleon.Intersection(&pos) )
	{
		if (Control::Input->LBclicked())
		{
			float g = 0.5f + 1.25f*((rand()%10)/10.0f);
			float r = 0.5f + 1.25f*((rand()%10)/10.0f);
			float b = 0.5f + 1.25f*((rand()%10)/10.0f);
			HOScene->GetObjectByID("lizardtail")->SetTint(r, g, b);
			HOScene->GetObjectByID("scnanimal1")->SetTint(r, g, b);
			return;
		}
	}


	if(isHOCompleted)
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;

		hoCompleteTimer += Control::LogicRate;
		if(hoCompleteTimer>2.5f)
		{
			isTreeReleaseCutscnPlaying = true;
			cv_TreeReleaseCutscn->PlayFadeInVideo(3.0f);
			Hud->EnableInput(false);
			Hud->HideHud(false);
		}
		return;
	}

	treeEyeTimer += Control::LogicRate;
	if(treeEyeTimer>treeEyeTimerThreshold)
	{
		s6hoeyefxFX->Start();
		s6hoeyefx2->Start();

		treeEyeTimer = 0;
		treeEyeTimerThreshold = (float)(10+rand()%20);
	}


	cloudBG1->Pos.x -= Control::LogicRate*cloudbgSpeed1;
	cloudBG2->Pos.x -= Control::LogicRate*cloudbgSpeed2;

	if(cloudBG1->Pos.x<(-CLOUD_OVERFLOW_WIDTH))
	{
		cloudBG1->Pos.x = 1366.0f + CLOUD_OVERFLOW_WIDTH + rand()%50;
		cloudbgSpeed1 = 10.0f+float(rand()%2);
	}

	if(cloudBG2->Pos.x<(-CLOUD_OVERFLOW_WIDTH))
	{
		cloudBG2->Pos.x = 1366.0f + CLOUD_OVERFLOW_WIDTH + rand()%50;
		cloudbgSpeed2 = 11.0f+rand()%2;
	}
	
	cloudFG1->Pos.x -= Control::LogicRate*cloudfgSpeed1;
	cloudFG2->Pos.x -= Control::LogicRate*cloudfgSpeed2;

	if(cloudFG1->Pos.x<(-CLOUD_OVERFLOW_WIDTH))
	{
		cloudFG1->Pos.x = 1366.0f + CLOUD_OVERFLOW_WIDTH + rand()%50;
		cloudfgSpeed1 = 12.0f+rand()%2;
	}

	if(cloudFG2->Pos.x<(-CLOUD_OVERFLOW_WIDTH))
	{
		cloudFG2->Pos.x = 1366.0f + CLOUD_OVERFLOW_WIDTH + rand()%50;
		cloudfgSpeed2 = 13.0f+rand()%2;
	}

	if(isSoldierBadgePlacePlaying)
		return;

	if( !ProfileSystem::Instance->ActiveProfile.L5_S6_HOSoldierBadgePlaced)
	{
		if(InvDestRectSoldierBadge.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked() )
			{
				HOScene->GetObjectByID("boxcover")->Shake(100, 2, 0, 0.35f);
				Hud->ShowBannerText("L5circle");
			}
		}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			// Need to check the Back button flow
			Hud->ShowBannerText("L5grip");					
		}	
		#endif	
		if( IsMouseOverGoBackArea(&pos) )
		{
			Cursor::SetMode(CA_ExitDown,-1);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L5grip");
			}	
		}
		return;
	}

	bool clicked = Control::Input->LBclicked();
	//HO Hanndling
	if(clicked)
	{
		for (int i = 0; i < MAX_L5S6_HOOBJECTS; i++)
		{
			if(!isHOCollected[i])
			{
				if(HOPolyArray[i]->Intersection(&pos))
				{
					Control::Audio->QuickLoadAndPlaySFX("s6ho_itemclicked");

					HOFoundFX->SetPos(&HOGlowObjects[i]->Pos);
					HOFoundFX->Start();
					HOGlowObjects[i]->FadeIn(2.0f);

					s6homanjaadifxFX->Start();

					//Rise up manjaadi
					manjaadiRiseup->Show(true);
					manjaadiRiseup->Pos = manjaadiRiseup->InitPos;					
					manjaadiRiseup->TweenPosTo(0, -100, -1, 0.75f, 0, true);

					HOManjadiObjects[i]->Pos.y -= 150;
					HOManjadiObjects[i]->SetRotate(float(rand()%30));
					HOManjadiObjects[i]->TweenPosTo(0, 150, 0, 1.25f, TW_EASEQUADIN, true);
					HOManjadiObjects[i]->Show(true);

					ProfileSystem::Instance->ActiveProfile.L5_S6_HOObjectFound[i] = isHOCollected[i] = true;
					numObjCollected += 1;

					updateBoxSymbolGlow();

					if(numObjCollected>=MAX_L5S6_HOOBJECTS)
					{
						HandleHOComplete();				
					}
					break;
				}
			}
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		// Need to check the Back button flow
		Hud->ShowBannerText("L5grip");					
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) )
	{
		Cursor::SetMode(CA_ExitDown,-1);
		if( clicked )
		{
			Hud->ShowBannerText("L5grip");
		}	
	}
}

void L5_S6_HOTree::HandleHOComplete()
{
	AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	isHOCompleted = true;
	hoCompleteTimer = 0;

	ProfileSystem::Instance->ActiveProfile.L5_S6_HOCompleted = true;

	if(_handleExtrasGameComplete(false))
		return;

	SetLockLevels(false);

	Hud->ShowBannerText("L5free");
	Control::Audio->PlaySample(aVOL5TreeRefS6_1);
}

void L5_S6_HOTree::updateBoxSymbolGlow()
{
	if(!boxGlow1Butterfly->Visible && isHOCollected[0] && isHOCollected[1] && isHOCollected[2])
		boxGlow1Butterfly->FadeIn(1.0f);

	if(!boxGlow2Nest->Visible && isHOCollected[3] && isHOCollected[4])
		boxGlow2Nest->FadeIn(1.0f);

	if(!boxGlow3Bird->Visible && isHOCollected[5] && isHOCollected[6] && isHOCollected[7] && isHOCollected[8])
		boxGlow3Bird->FadeIn(1.0f);
	
	if(!boxGlow4Snake->Visible && isHOCollected[9] && isHOCollected[10] && isHOCollected[11])
		boxGlow4Snake->FadeIn(1.0f);

	if(!boxGlow5Nuts->Visible && isHOCollected[12] && isHOCollected[13])
		boxGlow5Nuts->FadeIn(1.0f);

	if(ManjadiFull->Visible && numObjCollected>5)
	{
		ManjadiFull->Show(false);
		ManjadiHalf->Show(true);
	}

	if(ManjadiHalf->Visible && numObjCollected>10)
	{
		ManjadiHalf->Show(false);
		ManjadiQuarter->Show(true);
	}
}

void L5_S6_HOTree::ReceiveMessage(int val)
{
	if( val == HOSM_DialogClosed ) 
	{
		isDialogActive = false;
		Hud->AllowInput = true;
		Hud->ShowHud(true);

		if(dialogSystem->GetDialogId() == eDialog_L5S6TreeGuardianAfterBadgePlaced)
		{
			// Start HO
			hoHelpText->Show(true);

			Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
			Hud->IsHOScene = true;
			AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);

		}
		else if(dialogSystem->GetDialogId() == eDialog_L5S6TreeGuardian)
		{
			ProfileSystem::Instance->ActiveProfile.L5_S6_HOGuardianIntroTalkCompleted = true;
		}
	}
	else if( val == MAP_NEW_AREA ) 
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

			
		if( !ProfileSystem::Instance->ActiveProfile.L5_S6_HOGuardianIntroTalkCompleted && isDialogActive)
		{
			Hud->ReleaseDrag();
			return;
		}

			if( itemVal == eInv_L5_S2_SoldierBadge )
			{
				if( !ProfileSystem::Instance->ActiveProfile.L5_S6_HOSoldierBadgePlaced && InvDestRectSoldierBadge.Intersection(&dropPos) )
				{
					ProfileSystem::Instance->ActiveProfile.L5_S6_HOSoldierBadgePlaced = true;

					Control::Audio->QuickLoadAndPlaySFX("s6ho_badgeused");

					isSoldierBadgePlacePlaying = true;
					JAnim_SoldierBadgePlace->PlayAnim();



					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
						
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S2_SoldierBadge] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S2_SoldierBadge);

					//No further user inputs - as further going to dialogmode!!
					Hud->AllowInput = false;
					Hud->IsHOScene = true;
					Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim						
					Hud->HideHud(true);
					return;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L5_S6_HOTree::ProcessSkip()
{
}

void L5_S6_HOTree::ProcessNonHOHint()
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
			if(!ProfileSystem::Instance->ActiveProfile.L5_S6_HOSoldierBadgePlaced)
			{
				Hud->ShowHintForInvItem(eInv_L5_S2_SoldierBadge, &InvDestRectSoldierBadge);
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if(!isHOCompleted)
			{
				for (int i = 0; i < MAX_L5S6_HOOBJECTS; i++)
				{
					if(!isHOCollected[i])
					{
						Hud->InitHintTrailEmit(HOPolyArray[i]->center.x, HOPolyArray[i]->center.y);
						AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
						return;
					}
				}
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

L5_S6_HOTree::~L5_S6_HOTree()
{
	Hud->InvBoxCtrl->SetLockBase(INVBOX_LR_FRAME);//lock in HO anim
	Hud->IsHOScene = false;

	SAFE_DELETE(cv_TreeReleaseCutscn);

	SAFE_DELETE(JAnim_SoldierBadgePlace);
	SAFE_DELETE(boxOpenVFX);
	SAFE_DELETE(hoHelpText);
	SAFE_DELETE(HOFoundFX);

	SAFE_DELETE(s6hofgblueplantFX);
	SAFE_DELETE(s6horedplantFX);
	SAFE_DELETE(s6hoeyefxFX);
	SAFE_DELETE(s6hoeyefx2);
	SAFE_DELETE(s6holeafFX);
	SAFE_DELETE(s6homanjaadifxFX);
	SAFE_DELETE(s6hosnowFX);
	SAFE_DELETE(s6hofogFX);

	SAFE_DELETE(_faceMesh);

	for (int i = 0; i < MAX_L5S6_HOOBJECTS; i++)
	{
		SAFE_DELETE(HOPolyArray[i]);
		HOScnObjects[i] = NULL;
		HOGlowObjects[i] = NULL;
		HOManjadiObjects[i] = NULL;
	}

	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL5TreeRefS6_1);
	Control::Audio->StopSample(aAmbL5s6TreeHOAmb);
	Control::Audio->UnloadSample(aAmbL5s6TreeHOAmb);
	//MUSIC_SFX
}
