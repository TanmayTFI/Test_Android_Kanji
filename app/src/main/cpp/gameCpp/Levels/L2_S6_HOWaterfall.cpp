//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 06 : Waterfall HO
//====================================

#include "L2_S6_HOWaterfall.h"
#include "Hud.h"
#include "HOScene.h"
#include "Rect.h"
#include "Application.h"
#include "SDrop.h"
#include "Util.h"
#include "exMath.h"
#include "AwardManager.h"

const CRectangle  BtnRectMatch3HO(559.0f,1.0f,265.0f,39.0f);

const std::string HOItemNames[MAX_L2S6_HOOBJECTS] =
{
	"Goddess",
	"Spear_Head",
	"Lizard",
	"Star_Fish",
	"Worn_out_Sandal",
	"Lotus",
	"Frog",
	"Tribal_Mask",
	"Chess_Piece",
	"Crab",
	"Rope",
	"Beads",
	"Bone",
	"Flute",
	"Bracelet",
	"Indian_Doll",
	"Rag",
	"Gear",
	"Nails",
	"Fish",
	"Broom",
	"Glove"
};

L2_S6_HOWaterfall::L2_S6_HOWaterfall()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S6\\HO\\HO.SCN");
	musicFadeOutTimer = 0;
	//Scene animations
	HOScene->GetObjectByID("scnleafleft")->SetPivot(-50, 70);
	HOScene->GetObjectByID("scnleafleft")->Animate(0,0,3,0,8.0f);

	HOScene->GetObjectByID("scnleafright")->SetPivot(-54, 70);
	HOScene->GetObjectByID("scnleafright")->Animate(0,0,3,0,6.0f,1.25f);

	/*HOScene->GetObjectByID("rayleft1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("rayleft1")->Animate(0.15f, 1.0f, 5.0f);*/

	
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentLevel);

	FgPlantMesh = NULL;

	FgPlantMesh = new MeshObject("LV2\\S6\\HO\\leftmeshplant.PNG", 5, 5);
	FgPlantMesh->SetPosAndParams(-11,-17, 2, 2, 2, 5);
	FgPlantMesh->SetZ(4);

	waterHOFliesFX = new SuperFX("PRT\\L2\\waterHOFlies.INI", PointSystem::CreateCPoint(605,272,0), 1100); 
	waterHOFliesFX->Start();
	waterHOFliesFX->AdvanceByTime(2.0f);

	waterHODropsFX = new SuperFX("PRT\\L2\\waterHODrops.INI", PointSystem::CreateCPoint(887,1,0), 1100); 
	waterHODropsFX->Start();
	waterHODropsFX->AdvanceByTime(2.0f);

	HOInvBoxFX = new SuperFX("PRT\\L2\\HOInvBoxFX.INI", PointSystem::CreateCPoint(688,721,0), 1805); 

	foundItemsCount = 0;
	for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
	{
		HOPolyArray[i] = NULL;
		HOObjects[i] = NULL;
		if(ProfileSystem::Instance->ActiveProfile.L2S6_HOItemsFound[i])
		{
			foundItemsCount += 1;
		}
	}

	lastObjDelayTimer = -1;

	//Polygon Init
	int Pts0[] = {454,459,500,467,530,447,547,422,544,412,511,367,509,341,501,340,485,368,439,398,453,442};
	HOPolyArray[0]=  new CPolygon(11);
	HOPolyArray[0]->Init(Pts0);
	int Pts1[] = {483,201,493,178,491,171,524,109,536,107,537,122,519,183,510,185,502,209};
	HOPolyArray[1]=  new CPolygon(9);
	HOPolyArray[1]->Init(Pts1);
	int Pts2[] = {724,493,646,500,593,489,576,472,578,454,627,436,686,416,690,402,720,400,726,410,708,432,730,443,714,459,699,459,681,449,664,454,660,469,636,469,636,459,601,464,602,473,637,486};
	HOPolyArray[2]=  new CPolygon(22);
	HOPolyArray[2]->Init(Pts2);
	int Pts3[] = {741,490,742,502,771,512,807,543,827,540,809,497,859,490,858,473,791,457,783,438,766,446,773,456,766,477};
	HOPolyArray[3]=  new CPolygon(13);
	HOPolyArray[3]->Init(Pts3);
	int Pts4[] = {908,88,913,103,908,116,847,142,825,142,811,133,810,120,834,100,852,95,881,84};
	HOPolyArray[4]=  new CPolygon(10);
	HOPolyArray[4]->Init(Pts4);
	int Pts5[] = {354,536,367,528,387,505,373,501,380,484,379,470,359,478,355,455,344,455,340,479,346,511};
	HOPolyArray[5]=  new CPolygon(11);
	HOPolyArray[5]->Init(Pts5);
	int Pts6[] = {383,559,401,575,418,588,432,586,457,572,458,563,453,557,461,545,461,534,444,522,431,533,405,536,386,548};
	HOPolyArray[6]=  new CPolygon(13);
	HOPolyArray[6]->Init(Pts6);
	int Pts7[] = {597,256,627,257,659,228,660,196,664,173,634,140,597,133,567,143,541,170,534,195,538,235,566,261,591,264};
	HOPolyArray[7]=  new CPolygon(13);
	HOPolyArray[7]->Init(Pts7);
	int Pts8[] = {778,350,755,357,724,358,725,342,730,330,723,250,728,239,759,250,768,288,775,307,773,330};
	HOPolyArray[8]=  new CPolygon(11);
	HOPolyArray[8]->Init(Pts8);
	int Pts9[] = {516,534,533,524,532,534,556,538,565,523,589,528,603,541,606,551,617,575,614,593,591,608,556,602,541,602,544,578,528,549};
	HOPolyArray[9]=  new CPolygon(15);
	HOPolyArray[9]->Init(Pts9);
	int Pts10[] = {244,608,224,618,204,613,192,530,203,491,207,483,231,456,253,429,279,409,311,417,333,433,347,491,348,530,354,542,335,544,330,524,325,486,323,461,315,477,313,501,326,546,315,554,285,527,296,489,287,447,268,459,243,508,238,548};
	HOPolyArray[10]=  new CPolygon(28);
	HOPolyArray[10]->Init(Pts10);
	int Pts11[] = {969,490,969,501,981,506,994,509,1008,510,1011,494,1007,479,997,478,982,471};
	HOPolyArray[11]=  new CPolygon(9);
	HOPolyArray[11]->Init(Pts11);
	int Pts12[] = {812,56,816,65,803,78,770,113,766,123,755,130,731,122,724,107,736,102,755,105,788,70,789,57,803,52};
	HOPolyArray[12]=  new CPolygon(13);
	HOPolyArray[12]->Init(Pts12);
	int Pts13[] = {208,480,216,484,225,471,227,460,233,459,241,450,263,388,267,373,259,362,245,369,228,417,216,460};
	HOPolyArray[13]=  new CPolygon(12);
	HOPolyArray[13]->Init(Pts13);
	int Pts14[] = {1154,319,1138,340,1136,359,1148,381,1158,386,1173,377,1183,356,1183,338,1176,319,1166,317,1164,325,1169,333,1167,354,1157,355,1148,358,1149,346,1158,327};
	HOPolyArray[14]=  new CPolygon(17);
	HOPolyArray[14]->Init(Pts14);
	int Pts15[] = {905,312,919,307,926,322,934,331,948,360,940,375,956,406,947,418,913,425,905,412,898,362,889,353,889,337,897,333};
	HOPolyArray[15]=  new CPolygon(14);
	HOPolyArray[15]->Init(Pts15);
	int Pts16[] = {1117,448,1086,458,1086,469,1100,476,1087,489,1096,511,1117,503,1133,526,1172,504,1182,479,1178,469,1166,449,1167,441,1146,417,1131,428,1130,438,1160,450,1173,468};
	HOPolyArray[16]=  new CPolygon(18);
	HOPolyArray[16]->Init(Pts16);
	int Pts17[] = {880,172,895,166,898,153,914,150,926,163,925,176,941,183,948,161,943,143,925,126,907,127,889,132,879,144,876,160};
	HOPolyArray[17]=  new CPolygon(14);
	HOPolyArray[17]->Init(Pts17);
	int Pts18[] = {916,499,907,502,865,546,843,545,842,553,891,587,903,581,913,590,924,574,878,553,923,506};
	HOPolyArray[18]=  new CPolygon(11);
	HOPolyArray[18]->Init(Pts18);
	int Pts19[] = {404,256,410,269,425,281,458,288,481,283,492,277,504,286,509,279,505,267,507,251,499,248,493,261,463,255,451,240,442,253};
	HOPolyArray[19]=  new CPolygon(15);
	HOPolyArray[19]->Init(Pts19);
	int Pts20[] = {941,288,932,297,952,323,976,323,1045,288,1084,264,1163,253,1167,245,1157,218,1148,215,1142,222,1109,234,1087,260,1041,286,1016,294,971,294,949,288};
	HOPolyArray[20]=  new CPolygon(17);
	HOPolyArray[20]->Init(Pts20);
	int Pts21[] = {809,275,792,278,780,241,737,218,744,206,764,212,743,184,762,165,791,158,808,174,822,216,846,255,839,258,814,235,804,247};
	HOPolyArray[21]=  new CPolygon(15);
	HOPolyArray[21]->Init(Pts21);
	//Polygon Init End

	isHOCompleted =  false;

	isHOInvTextsInitialized = false;

	InitHOInventoryTexts();

	for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
	{
		HOObjects[i] = new L2S6HiddenObject();
		HOObjects[i]->setSceneObject(HOScene->GetObjectByID(std::string("hoscnitem" + MKSTR(i)).c_str()));
		HOObjects[i]->setFlyingObject(HOScene->GetObjectByID(std::string("hoflyitem" + MKSTR(i)).c_str()));
		HOObjects[i]->setNameText(HOInvboxTexts[i]);
		HOObjects[i]->setPolygonArea(HOPolyArray[i]);
		HOObjects[i]->InitObject();

		if(ProfileSystem::Instance->ActiveProfile.L2S6_HOItemsFound[i])
		{			
			HOObjects[i]->AlreadyFound();
		}
	}

	//Match 3
	match3Ctrl  = NULL;
	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	
	//BtnMatch3 = HOScene->GetObjectByID("btnMatch3");
	//BtnSwitchToHO = HOScene->GetObjectByID("btnSwitchToHO");

	match3Back = new CObject();
	match3Back->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
	match3Back->SetUID("match3Back");
	match3Back->Show(false);
	HOScene->PushObject(match3Back);

	match3BackText = new CObject();
	match3BackText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
	match3BackText->SetUID("match3BackText");
	match3BackText->Show(false);
	HOScene->PushObject(match3BackText);
#ifndef M3_BUTTON
	match3Back->SetEnable(false);
    match3BackText->SetEnable(false);
#endif
	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		match3Back->SetEnable(false);
		match3BackText->SetEnable(false);
	}
	BtnMatch3 = new CObject();
	BtnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 676, 25, 48);
	BtnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(BtnMatch3);

	BtnSwitchToHO = new CObject();
	BtnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 676, 25, 49);
	BtnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(BtnSwitchToHO);

	BtnMatch3->SetZ(100);
	BtnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);
	BtnSwitchToHO->Show(false);

	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;

	GFInstance->PostUpdate(2);

	Control::Audio->LoadSample(aAmbs6WaterHo,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs6WaterHo) )		
	{
		Control::Audio->PlaySample(aAmbs6WaterHo,true);
	}
}

void L2_S6_HOWaterfall::InitHOInventoryTexts(bool showChangeFX)
{
	if (!isHOInvTextsInitialized)
	{
		for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
		{
			ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
			HOInvboxTexts[i] = new CBitmapText();
			HOInvboxTexts[i]->SetTextProps(StringTable::getStr(HOItemNames[i]), 10, 10, eZOrder_INVBOXITEMS, gold, eFont_Small, Align_Center);
			HOInvboxTexts[i]->SetZ(eZOrder_INVBOXITEMS);
		}
		isHOInvTextsInitialized = true;
	}

	//Show/Hide first and second part
	if(foundItemsCount>11) //if first 12 objects are found
	{
		for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
		{
			if(i<12)
				HOInvboxTexts[i]->Show(false);
			else
				HOInvboxTexts[i]->Show(true);
		}
	}
	else
	{
		for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
		{
			if(i<12)
				HOInvboxTexts[i]->Show(true);
			else
				HOInvboxTexts[i]->Show(false);
		}
	}

	CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);

	//Set position of texts based on Inv Area Rect
	// into 4 x 3 first then 4 4 2

	int numCols=3, numRows=4;
	float vertGap = 26, horizontalGap = InvAreaRect.w/3.0f;
	CPoint pos(0,0,0);
	int k=0;
	if(foundItemsCount>11)
		k = 12; //arrange the second part of texts
	for (int i = 0; i < numCols; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			pos.x = InvAreaRect.Left() + horizontalGap*0.5f + horizontalGap*i;
			pos.y = InvAreaRect.Top() + vertGap*0.5f + vertGap*j;
			if(k<MAX_L2S6_HOOBJECTS)
				HOInvboxTexts[k]->SetPos(&pos);
			k+=1;
		}
	}

	if(showChangeFX)
	{
		Control::Audio->QuickLoadAndPlaySFX("S6WaterHOHalfListCompleted");
		HOInvBoxFX->Start();
	}
}

void L2_S6_HOWaterfall::Required()
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
			 if( !Control::Audio->IsPlaying(aTrackL2ho) )
				  Control::Audio->PlaySample(aTrackL2ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2ho, true);
		}
	}
}

void L2_S6_HOWaterfall::Update()
{
	NavigateToScene();
	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	if( Hud->PopupState != Popup_Inactive )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if(!BtnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompleted)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompleted)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		BtnMatch3->Show(false);
		BtnSwitchToHO->Show(false);
	}

#ifdef _CHEATS
	if(Control::Input->KpressedWithShift(K_VK_SPACE))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompleted)
		{
			HandleHOComplete();
			return;
		}
	}
#endif // _CHEATS

	if(lastObjDelayTimer>0)
	{
		lastObjDelayTimer -= Control::LogicRate;
		if(lastObjDelayTimer<0)
		{
			lastObjDelayTimer = -1;
			if(foundItemsCount==12)
			{
				InitHOInventoryTexts(true);
				/*Control::Audio->QuickLoadAndPlaySFX(aSFXL2S6HOHalfCompleted);
				HOInvBoxFX->Start();*/
				//Load next set of HO Objects
				for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
				{
					if(i<12)
						HOObjects[i]->SetActive(false);
					else
						HOObjects[i]->SetActive(true);
				}
			}
			else
			{
				AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
				HandleHOComplete();
			}
		}
		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompleted && ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompletedWithMatch3)
	{	
		HandleHOComplete();
		return;
	}

	if(isHOCompleted)
	{
		return;
	}

	//HO Click Checking
	if(!ProfileSystem::Instance->ActiveProfile._IsMatch3Playing) //active only when no match3
	{
		if( Control::Input->LBclicked() )
		{
			for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
			{
				if(HOObjects[i]->MouseClicked(pos))
				{
					int randId = rand()%3;
					switch (randId)
					{
					case 0:		Control::Audio->QuickLoadAndPlaySFX("S2NagaHOObjectFoundv1");		break;
					case 1:		Control::Audio->QuickLoadAndPlaySFX("S2NagaHOObjectFoundv2");		break;
					case 2:		Control::Audio->QuickLoadAndPlaySFX("S2NagaHOObjectFoundv3");		break;
					default:
						break;
					}

					ProfileSystem::Instance->ActiveProfile.L2S6_HOItemsFound[i] = true;
					foundItemsCount += 1;

					CheckHOComplete();
				}
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(BtnRectMatch3HO.Intersection(&pos) && !match3Ctrl->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSwitchToHO->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				//Show Inv Box Items
				//Show/Hide first and second part
				for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
				{
					HOObjects[i]->SwitchToHO();
				}

				match3Ctrl->clearBoard();
				BtnMatch3->Show(true);
				BtnSwitchToHO->Show(false);
				BtnSwitchToHO->PlayAnimation(0);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				return;
			}
		}
		else
		{
			BtnSwitchToHO->PlayAnimation(0);
		}
	}
	else
	{
		if(BtnRectMatch3HO.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnMatch3->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				if(match3Ctrl == NULL)
				{
					match3Ctrl = new Match3(5);
					AddControl(match3Ctrl);
				}
				else
				{
					match3Ctrl->ShowBoard();
				}

				//Hide Inv Box Items
				for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
				{
					HOObjects[i]->SwitchToMatch3();
				}

				BtnMatch3->Show(false);
				BtnMatch3->PlayAnimation(0);
				BtnSwitchToHO->Show(true);
				
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;

				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				return;
			}
		}
		else
		{
			BtnMatch3->PlayAnimation(0);
		}
	}
	

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
		{
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(match3Ctrl!=NULL)
					match3Ctrl->clearBoard();
				BtnMatch3->Show(false);
				BtnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_TunnelEnd;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
    #endif

	if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
	{
		if( Control::Input->LBclicked() )
		{
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(match3Ctrl!=NULL)
					match3Ctrl->clearBoard();
				BtnMatch3->Show(false);
				BtnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}

			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_TunnelEnd;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	if( IsMouseOverGoBackArea(&pos)
		#ifdef M3_BUTTON
		&& BtnMatch3->Visible
        #endif
        )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L2_S6_TunnelEnd);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L2_S6_TunnelEnd);
#endif

		if( Control::Input->LBclicked() )
		{

#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S6_TunnelEnd, 2);
			}
#else
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(match3Ctrl!=NULL)
					match3Ctrl->clearBoard();
				BtnMatch3->Show(false);
				BtnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}

			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_TunnelEnd;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif



			return;
		}
	}
}

void L2_S6_HOWaterfall::CheckHOComplete()
{
	if(foundItemsCount==12 || foundItemsCount>=MAX_L2S6_HOOBJECTS)
	{
		lastObjDelayTimer = 2.5f;
	}
}


void L2_S6_HOWaterfall::HandleHOComplete()
{
	//Control::Audio->QuickLoadAndPlaySFX(aSFXL2S2HOPartsCombineAnim);

	ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompleted = true;
	isHOCompleted = true;

	if(_handleExtrasGameComplete(false))
		return;

	Hud->HOState = HOInactive;
	Hud->InvBoxCtrl->SetLockBase(INVBOX_LR_FRAME);
	Hud->IsHOScene = false;

	Hud->InvBoxCtrl->ShowInvObj(true);



	for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
	{
		HOInvboxTexts[i]->Show(false);
	}

	ProfileSystem::Instance->ActiveProfile.L2S6_InvGlovesTaken = true;

	Hud->TakeInventoryItem(eInv_L2S6_Gloves);
}

L2_S6_HOWaterfall::~L2_S6_HOWaterfall()
{
	SAFE_DELETE(FgPlantMesh);


	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	Hud->InvBoxCtrl->ResetToInvBox();//Reset back to Inv Objects
	Hud->IsHOScene = false;

	for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
	{
		SAFE_DELETE(HOObjects[i]);
		SAFE_DELETE(HOPolyArray[i]);
		SAFE_DELETE(HOInvboxTexts[i]);
	}

	SAFE_DELETE(waterHOFliesFX);
	SAFE_DELETE(waterHODropsFX);
	SAFE_DELETE(HOInvBoxFX);

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbs6WaterHo);
	Control::Audio->UnloadSample(aAmbs6WaterHo);
	//MUSIC_SFX
	
}


void L2_S6_HOWaterfall::ReceiveMessage(int val)
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
	else if( val == MAP_HUD_CLICK ) 
	{
		//TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INV_CLOSE )
	{
		if(ProfileSystem::Instance->ActiveProfile.L2S6_InvGlovesTaken)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_TunnelEnd;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}	
}

void L2_S6_HOWaterfall::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 2;
    
	do
	{
        AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea); // Sac: what does this do here ??
		switch( iNonHOHint )
		{
			
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
					{
						return;
					}
					break;
			case 1: ++iNonHOHint;
					for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
					{
						if(!ProfileSystem::Instance->ActiveProfile.L2S6_HOItemsFound[i])
						{
							InitHintTrailEmit(HOObjects[i]->getSceneObject()->Pos.x, HOObjects[i]->getSceneObject()->Pos.y);
							return;
						}
					}
					break;

			
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

#pragma region L2S6HiddenObjectClass

L2S6HiddenObject::L2S6HiddenObject()
{
	_isActive = true;

	_isAnimComplete = false;
	_startPoint = NULL;
	_endPoint = NULL;
	_ctrlPoint = NULL;	

	_nameText = NULL;
	_scnObj = NULL;
	_flyObj = NULL;
	_polygonArea = NULL;
	_isFound = false;
	hoScaleTimer = hoFlyingTimer = 0;

	HOTrailFX = NULL;
	waterHOFX = NULL;
}

L2S6HiddenObject::~L2S6HiddenObject()
{
	_startPoint = NULL;
	_endPoint = NULL;
	SAFE_DELETE(_ctrlPoint);
	
	_scnObj = NULL;
	_flyObj = NULL;
	_polygonArea = NULL;	
	_isFound = false;
	SAFE_DELETE(HOTrailFX);
	SAFE_DELETE(waterHOFX);
}

void L2S6HiddenObject::InitObject()
{
	//set Active if the text is visible
	SetActive(_nameText->Visible);

	_startPoint = &_scnObj->Pos;

	CPoint offset(_nameText->StringWidth()*0.5f,0,0);
	_endPoint = &(_nameText->Pos);// + offset);

	_ctrlPoint = new CPoint((_startPoint->y + _endPoint->y)/2.0f, (_startPoint->y + _endPoint->y)/2.0f, 0);

	if(rand()%10>5)
		_ctrlPoint->x += (200+rand()%200);
	else
		_ctrlPoint->x -= (200+rand()%200);

	_flyObj->SetZ(eZOrder_INVBOXITEMS);
	_flyObj->SetEnable(false);

	HOTrailFX = NULL;
	HOTrailFX = new SuperFX("PRT\\L2\\HOTrailFX.INI", PointSystem::CreateCPoint(1065,122,0), 50);

	waterHOFX = new SuperFX("PRT\\L2\\waterHO.INI", PointSystem::CreateCPoint(564,368,0), 1100); 
	waterHOFX->SetPos(_startPoint);
}

bool L2S6HiddenObject::MouseClicked(CPoint pos)
{
	if (_isFound || !_isActive)
	{
		return false;
	}

	if (_polygonArea->Intersection(&pos))
	{
		OnFound();
		return true;
	}

	return false;
}

void L2S6HiddenObject::OnFound()
{
	_scnObj->FadeOut(3.0f);
	_flyObj->SetEnable(true);
	_flyObj->FadeIn(4.0f);
	_flyObj->SetZ(eZOrder_INVBOXITEMS);
	_isFound = true;
	waterHOFX->Start();
	_isObjectScaling = true;
}

void L2S6HiddenObject::Update(float ds)
{
	if (_isAnimComplete || !_isFound)
		return;

	if(_isObjectScaling)
	{
		//scaling anim
		hoScaleTimer += Control::LogicRate;
		_flyObj->SetScale(0.85f+Elixir::Sin(90+hoScaleTimer*360.0f)*0.15f);
		if(hoScaleTimer > 1.0f)
		{
			//start flying
			_isObjectScaling = false;
		}
	}
	else
	{
		if(!HOTrailFX->IsEmitting)
			HOTrailFX->Start();

		FXTimer += 1;
		if(FXTimer%3==0)
			HOTrailFX->Emit();

		//flying to HUD
		hoFlyingTimer += Control::LogicRate;

		CPoint temp = Elixir::GetBezierPoint(_startPoint, _ctrlPoint, _endPoint, hoFlyingTimer);
		_flyObj->SetPos(&temp);
		_flyObj->SetScale(1.0f-hoFlyingTimer*0.75f);
		HOTrailFX->SetPos(&temp);

		if(hoFlyingTimer>1.0f)
		{
			HOTrailFX->StopEmit();
			_nameText->SetStrikethrough(true);
			_flyObj->SetEnable(false);
			_isAnimComplete = true;
		}
	}
}

void L2S6HiddenObject::AlreadyFound()
{
	_isFound = true;
	_isAnimComplete = true;
	_scnObj->SetEnable(false);
	_flyObj->SetEnable(false);
	_nameText->SetStrikethrough(true);
	_nameText->SetAlpha(0.5f);
}

void L2S6HiddenObject::SwitchToMatch3()
{
	if(_isActive)
		_nameText->Show(false);
}

void L2S6HiddenObject::SwitchToHO()
{
	if(_isActive)
		_nameText->Show(true);
}

void L2_S6_HOWaterfall::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);

				if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
				{
					if(match3Ctrl!=NULL)
						match3Ctrl->clearBoard();
					BtnMatch3->Show(false);
					BtnSwitchToHO->Show(false);
					ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				}

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

#pragma endregion
