//====================================
//	DQFYLH
//	HO System
//	KidsRoom
//====================================

#include "KidsRoom.h"
#include "Hud.h"
#include "Fonts.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "TextureFactory.h"
#include "GameTextManager.h"

const CPoint	CenterPoint(683,334,0);

const CRectangle  WindBellRect(504.0f,1.0f,112.0f,272.0f);
const CRectangle	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  TableLampRect(379.0f,413.0f,58.0f,78.0f);



KidsRoom::KidsRoom()
{
	MessageText = NULL;
	MessageElement = NULL;
	LoadScene("EXTRAS\\KIDROOM\\ROOM.SCN");
	
	HOScene->SetZOff(eZOrder_INVBOXITEMS+10);

	BackBtn = HOScene->GetObjectByID("BTNback");

	HOScene->GetObjectByID("lampGlow")->SetEnable(false);
	HOScene->GetObjectByID("lampGlow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lampGlow")->Animate(0.75f, 0.9f, 4.0f);

	CObject *temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\EXTRAS\\KIDROOM\\BG_TEXT.SPR", 1010, 245, eZOrder_INVBOXITEMS + 11);
	temp->SetUID("BGtext");
	HOScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", BackBtn->Pos.x, BackBtn->Pos.y-2, BackBtn->ZOrder+1);
	temp->SetUID("backText");
	HOScene->PushObject(temp);

	HOScene->GetObjectByID("LightRay1")->Animate(0.25f, 0.75f, 6.0f);
	HOScene->GetObjectByID("LightRay2")->Animate(0.25f, 0.75f, 6.0f, 1.25f);

	HOScene->GetObjectByID("LightRay1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("LightRay2")->SetBlendMode(eBlend_AlphaAdditive);
	
	HOScene->GetObjectByID("lightglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightglow")->Animate(0.45f, 0.75f, 8.0f);

	HOScene->GetObjectByID("bedglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("bedglow")->Animate(0.45f, 0.75f, 8.0f, 1.0f);
	
	HangBellBody = HOScene->GetObjectByID("windBellTop");
	maxDeg = 5.0f;

	IsMaxDegRising = false;

	BellTopAngle = (float) (rand()%360);
	HangBellBody->SetRotate(Elixir::Sin(BellTopAngle)*maxDeg*0.5f);
	for (int i = 0; i < 5; i++)
	{
		HangBellParts[i] = HOScene->GetObjectByID(std::string("WindBell")+MKSTR((i+1)));

		HangBellAngles[i] = (float) (rand()%360);
		HangBellParts[i]->SetRotate(Elixir::Sin(HangBellAngles[i])*maxDeg);
	}

	txtMemoryCollectedCount = new CBitmapText();
	ColorRGB blueColor={(90/255.0f),(168/255.0f),(238/255.0f)}; //Sac: Color changed as per Bijish's values
	//ColorRGB blueColor={(209/255.0f),(254/255.0f),(1.0f)}; 
	txtMemoryCollectedCount->SetTextProps(StringTable::getStr("Exnew"), 382, 100, HangBellBody->ZOrder-1, blueColor, eFont_Italic24, Align_Center);
	txtMemoryCollectedCount->SetRotate(-12.0f);
	txtMemoryCollectedCount->SetAlpha(0.75f);

	MemPopScene = NULL;
	PopCloseBtn = NULL;

	PopMaskBg = new CObject();
	PopMaskBg->LoadSprite("UI\\MASK.SPR");
	PopMaskBg->SetXY(683,384);
	PopMaskBg->SetScale(2.0);
	PopMaskBg->SetZ(eZOrder_INVBOXITEMS+30);
	PopMaskBg->Show(false);

	lastObjFX = new SuperFX("PRT\\MNU\\crystalfg.INI", PointSystem::CreateCPoint(725,512,0), 51+eZOrder_INVBOXITEMS);

	//Init Polygons for Mem Objects
	NumMemObjActive = (int)eMem_L6S5KidSandal+1; //used

	int numMemObjectsCollected = 0;

	for (int i = 0; i < eMem_MAX; i++)
	{
		MemPolyArray[i] = NULL;		

		MemObjects[i] = NULL;
		if(i<NumMemObjActive)
			MemObjects[i] = HOScene->GetObjectByID(std::string("L1MemObj" + MKSTR(i)).c_str());

		isMemCollected[i] = ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[i];

		if(isMemCollected[i])
			numMemObjectsCollected += 1;

		if(i<NumMemObjActive && MemObjects[i]!=NULL)
		{
			if(isMemCollected[i])
				MemObjects[i]->PlayAnimation(1);
			else
				MemObjects[i]->SetAlpha(0.5f);
		}

		if( ProfileSystem::Instance->ActiveProfile.LastMemoryCollected == i)
		{
			lastObjFX->SetPos(&MemObjects[i]->Pos);
			lastObjFX->Start();
		}

	}

	//Set Collected Text
	txtMemoryCollectedCount->SetText(MKSTR(numMemObjectsCollected)+" "+StringTable::getStr("Exout")+MKSTR((int)eMem_MAX)+" "+StringTable::getStr("Exmemories"));

	//Level 1 Objects
	int MemPts0[] = {687,475,715,450,769,466,766,509,743,566,691,560};
	MemPolyArray[eMem_L1S1Teddy]=  new CPolygon(6);
	MemPolyArray[eMem_L1S1Teddy]->Init(MemPts0);

	int MemPts1[] = {801,469,795,518,832,524,880,504,891,474};
	MemPolyArray[eMem_L1S3MaryLamb]=  new CPolygon(5);
	MemPolyArray[eMem_L1S3MaryLamb]->Init(MemPts1);

	int MemPts2[] = {573,170,576,248,684,238,667,174};
	MemPolyArray[eMem_L1S3RibbonNeck]=  new CPolygon(4);
	MemPolyArray[eMem_L1S3RibbonNeck]->Init(MemPts2);

	int MemPts3[] = {621,641,584,658,582,682,608,705,663,714,687,692,682,666};
	MemPolyArray[eMem_L1S4BoxingGloves]=  new CPolygon(7);
	MemPolyArray[eMem_L1S4BoxingGloves]->Init(MemPts3);

	int MemPts4[] = {923,448,911,470,945,497,1048,549,1080,546,1087,524,944,446};
	MemPolyArray[eMem_L1S6ChappathiRoll]=  new CPolygon(7);
	MemPolyArray[eMem_L1S6ChappathiRoll]->Init(MemPts4);

	//Level 2
	int Pts5[] = {160,453,257,446,283,582,183,614,146,460};
	MemPolyArray[5]=  new CPolygon(5);
	MemPolyArray[5]->Init(Pts5);
	int Pts6[] = {968,390,934,422,997,475,1034,440};
	MemPolyArray[6]=  new CPolygon(4);
	MemPolyArray[6]->Init(Pts6);
	int Pts7[] = {766,370,808,370,810,447,765,443};
	MemPolyArray[7]=  new CPolygon(4);
	MemPolyArray[7]->Init(Pts7);
	int Pts8[] = {666,529,687,531,689,550,667,563,682,576,676,606,588,601,605,555};
	MemPolyArray[8]=  new CPolygon(8);
	MemPolyArray[8]->Init(Pts8);
	int Pts9[] = {675,221,677,237,775,242,808,255,820,225,785,215,766,190,718,189,711,213};
	MemPolyArray[9]=  new CPolygon(9);
	MemPolyArray[9]->Init(Pts9);
	
	//Level 3
	int Pts10[] = {687,578,683,586,683,610,694,610,758,599,753,580};
	MemPolyArray[10]=  new CPolygon(6);
	MemPolyArray[10]->Init(Pts10);
	int Pts11[] = {710,611,755,617,740,705,684,689};
	MemPolyArray[11]=  new CPolygon(4);
	MemPolyArray[11]->Init(Pts11);
	int Pts12[] = {573,682,533,671,481,684,478,705,493,725,582,720};
	MemPolyArray[12]=  new CPolygon(6);
	MemPolyArray[12]->Init(Pts12);
	int Pts13[] = {749,359,692,351,684,361,689,444,760,450,764,438};
	MemPolyArray[13]=  new CPolygon(6);
	MemPolyArray[13]->Init(Pts13);
	int Pts14[] = {174,242,248,252,237,320,166,315};
	MemPolyArray[14]=  new CPolygon(4);
	MemPolyArray[14]->Init(Pts14);

	//Level 4
	int Pts15[] = {624,363,601,363,567,386,569,430,599,454,637,450,663,411,650,377};
	MemPolyArray[15]=  new CPolygon(8);
	MemPolyArray[15]->Init(Pts15);
	int Pts16[] = {806,562,786,575,796,613,844,629,879,623,884,595,845,587};
	MemPolyArray[16]=  new CPolygon(7);
	MemPolyArray[16]->Init(Pts16);
	int Pts17[] = {905,303,902,376,1018,373,1009,307};
	MemPolyArray[17]=  new CPolygon(4);
	MemPolyArray[17]->Init(Pts17);
	int Pts18[] = {546,264,513,261,482,303,486,339,573,343,574,311};
	MemPolyArray[18]=  new CPolygon(6);
	MemPolyArray[18]->Init(Pts18);
	int Pts19[] = {1124,259,1098,258,1082,281,1100,309,1096,366,1143,369,1143,352,1152,343,1147,313,1153,299};
	MemPolyArray[19]=  new CPolygon(10);
	MemPolyArray[19]->Init(Pts19);

	//Level 5
	int Pts20[] = {617,255,596,277,606,293,597,326,603,342,664,340,655,301,664,302,664,280,646,251};
	MemPolyArray[20]=  new CPolygon(10);
	MemPolyArray[20]->Init(Pts20);
	int Pts21[] = {672,246,747,246,751,336,667,335};
	MemPolyArray[21]=  new CPolygon(4);
	MemPolyArray[21]->Init(Pts21);
	int Pts22[] = {293,583,283,607,311,632,377,622,385,577,357,550,368,482,305,448,277,498,287,566};
	MemPolyArray[22]=  new CPolygon(10);
	MemPolyArray[22]->Init(Pts22);
	int Pts23[] = {751,337,772,249,793,249,820,348,758,343};
	MemPolyArray[23]=  new CPolygon(5);
	MemPolyArray[23]->Init(Pts23);
	int Pts24[] = {919,504,834,524,860,567,964,543};
	MemPolyArray[24]=  new CPolygon(4);
	MemPolyArray[24]->Init(Pts24);

	//Level 6
	int Pts25[] = {208,334,273,323,285,403,216,418};	
	MemPolyArray[25]=  new CPolygon(4);
	MemPolyArray[25]->Init(Pts25);
	int Pts26[] = {1033,264,1060,265,1063,286,1079,298,1066,323,1077,355,1059,373,1021,371,1021,319,1003,288};
	MemPolyArray[26]=  new CPolygon(10);
	MemPolyArray[26]->Init(Pts26);
	int Pts27[] = {1084,405,1053,478,1164,512,1188,422};
	MemPolyArray[27]=  new CPolygon(4);
	MemPolyArray[27]->Init(Pts27);
	int Pts28[] = {924,191,835,204,840,309,934,299};
	MemPolyArray[28]=  new CPolygon(4);
	MemPolyArray[28]->Init(Pts28);
	int Pts29[] = {500,459,530,460,535,483,532,513,497,514};
	MemPolyArray[29]=  new CPolygon(5);
	MemPolyArray[29]->Init(Pts29);

	//End of Polygon init

	InitMessageBanner();

	Popup_State = POPUP_CLOSED;

	IsPopupActive = false;
	IsPopupTransitionActive = false;
	PopupAlpha = 0;
	PopVal = 0;

	PrevRandomId = 0;

	windowFX = new SuperFX();
	windowFX->LoadEffectIni("PRT\\KID\\WindowFX.INI");
	windowFX->SetZ(eZOrder_INVBOXITEMS+50);
	windowFX->SetPos(PointSystem::CreateCPoint(346,284,0));
	windowFX->Start();

	LampFX = new SuperFX();
	LampFX->LoadEffectIni("PRT\\KID\\LampFX2.INI");
	LampFX->SetZ(50+eZOrder_INVBOXITEMS);
	LampFX->SetPos(PointSystem::CreateCPoint(1253,155,0));
	LampFX->Start();

	ObjMouseOverFX = new SuperFX("PRT\\KID\\objglows.INI", PointSystem::CreateCPoint(725,512,0), 50+eZOrder_INVBOXITEMS);

	Control::Audio->TurnOffLvlMscTracks();//turns off music-snd carryover....

	SetZ(eZOrder_INVBOXITEMS+200);
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_KidsRoom] = true;

	
	

	Control::Audio->LoadSample(aTrackKidRoom, AudioTrack,true);
	Control::Audio->PlaySample(aTrackKidRoom,true);

	Control::Audio->LoadSample(aSFXKidRoomBell1,	AudioSFX);
	Control::Audio->LoadSample(aSFXKidRoomBell2,	AudioSFX);
	Control::Audio->LoadSample(aSFXKidRoomBell3,	AudioSFX);
	Control::Audio->LoadSample(aSFXKidRoomBell4,	AudioSFX);


	if(!ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromExtras)
	{
		//Setting the Current Area, to prevent if user Force exit while here
		ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea;
	}
	else
	{
		//Setting the Current Area, to prevent if user Force exit while here
		ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea;
	}

	Visible = true;	//To render anything with renderlistentity especially Video

	PrevRandomId = rand()%4;
	PlayRandomBellSFX();
}

void KidsRoom::PlayRandomBellSFX()
{
	switch (PrevRandomId)
	{
	case 0:		Control::Audio->PlaySample(aSFXKidRoomBell1,false);		break;
	case 1:		Control::Audio->PlaySample(aSFXKidRoomBell2,false);		break;
	case 2:		Control::Audio->PlaySample(aSFXKidRoomBell3,false);		break;
	case 3:		Control::Audio->PlaySample(aSFXKidRoomBell4,false);		break;
	default:
		break;
	}

	int randId = rand()%4;	

	if(randId == PrevRandomId)
		randId+=1;
	
	if(randId>3)
		randId = 0;

	PrevRandomId = randId;
}

void KidsRoom::Update()
{
	Cursor::SetMode(CA_Normal);	
	CPoint pos = Control::Input->Mpos();


#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) 
		{
			//Back to Scene or to Main Menu
				if(ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromExtras)//inside game
				{
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_ExtrasRoom;
					SendMessage(ControlExit, ID, HOSM_NewArea);
				}
				else if(ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromMainMenu)//main menu
					SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
				else
				{
					if( ProfileSystem::Instance->ActiveProfile.CurrentArea == -1 )//cleared 8 levels and jumped into memories from credits page
						SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
					else
					{
						SendMessage(ControlExit, ID, HOSM_NewArea);
						ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;
					}
				}
				return;
		}
#endif

	UpdateBanner();

	if(IsPopupTransitionActive)
	{
		UpdatePopupTransition();
		return;
	}

	if(IsPopupActive && !IsPopupTransitionActive)
	{
		if(PopCloseBtn!=NULL && (PopCloseBtn->Collide(&pos) || !PopAreaRect.Intersection(&pos)))
		{
			PopCloseBtn->PlayAnimation(1);
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				PopCloseBtn->PlayAnimation(2);
				IsPopupTransitionActive = true;

				MemPopScene->Show(false);

				PopCloseBtn = NULL;
				Popup_State = POPUP_SCALEDOWN;
				return;
			}
		}
		else
		{
			if(PopCloseBtn!=NULL)
				PopCloseBtn->PlayAnimation(0);
		}
		return;
	}

	if(TableLampRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			if(HOScene->GetObjectByID("lampGlow")->IsActive)
				HOScene->GetObjectByID("lampGlow")->SetEnable(false);
			else
				HOScene->GetObjectByID("lampGlow")->SetEnable(true);
		}
	}
	

	if(IsMaxDegRising)
	{
		maxDeg += 100.0f*Control::LogicRate;
		if(maxDeg>5.0f)
			IsMaxDegRising = false;
	}

	if(maxDeg>0)
	{
		BellTopAngle += 150.0f*Control::LogicRate;
		HangBellBody->SetRotate(Elixir::Sin(BellTopAngle)*maxDeg*0.25f);
		for (int i = 0; i < 5; i++)
		{
			HangBellAngles[i] += 200.0f*Control::LogicRate;
			HangBellParts[i]->SetRotate(Elixir::Sin(HangBellAngles[i])*maxDeg);
		}

		maxDeg -= Control::LogicRate;
	}
	else
	{
		if(WindBellRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				IsMaxDegRising = true;
				PlayRandomBellSFX();

				BellTopAngle = (float) (rand()%360);
				for (int i = 0; i < 5; i++)
					HangBellAngles[i] = (float) (rand()%360);
			}
		}
	}

	bool isAnyObjectMouseOver = false;

	for (int i = 0; i < NumMemObjActive; i++)
	{
		if(MemPolyArray[i]!=NULL && MemPolyArray[i]->Intersection(&pos))
		{
			if (!isAnyObjectMouseOver)
			{
				if(isMemCollected[i])
				{
					isAnyObjectMouseOver = true;

					Cursor::SetMode(CA_Look);
					MemObjects[i]->PlayAnimation(2);

					if(!ObjMouseOverFX->Active)
					{
						ObjMouseOverFX->SetPos(&MemObjects[i]->Pos);
						ObjMouseOverFX->Start();
					}
				}
				else
				{
					MemObjects[i]->SetAlpha(0.75f);
				} 
			}
			
			if(Control::Input->LBclicked())
			{
				if(isMemCollected[i])
				{
					Cursor::SetMode(CA_Normal);
					IsPopupActive = true;

					if(MemPopScene!=NULL)
						SAFE_DELETE(MemPopScene);

					ShowBannerText(MemoryBannerTexts[i]);

					MemPopScene = new CHOScene();
					MemPopScene->Load("EXTRAS\\KIDROOM\\POP\\MEMPOP.SCN");
					
					CObject *popBGImage = new CObject();
					if( i == 0 || i == 1 || i == 5 || i == 6 || i == 8 || i == 9 || i == 10 || i == 11 || i == 12  || i == 13 || i == 15 || i == 18 || i == 20 || i == 23)
						popBGImage->LoadInit(std::string("EXTRAS\\KIDROOM\\POP\\MEMPOP" + MKSTR(i) + ".SPR").c_str(), 0, 0, 0);
					else
						popBGImage->LoadInit(std::string("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\EXTRAS\\KIDROOM\\MEMPOP" + MKSTR(i)+".SPR").c_str(), 0, 0, 0);
					MemPopScene->InsertObject(popBGImage,0);

					PopCloseBtn = MemPopScene->GetObjectByID("closebtn");

					MemPopScene->SetPosOff(CenterPoint.x, CenterPoint.y);
					MemPopScene->SetZOff(eZOrder_INVBOXITEMS+100);

					Popup_State = POPUP_SCALEUP;

					PopVal = 0;
					PopupAlpha = 0;

					SetupPopupTransition(MemPopScene, (int)MemPolyArray[i]->center.x - 683, (int)MemPolyArray[i]->center.y -334);
					MemPopScene->Show(false);
					return;
				}
				else
				{
					ShowBannerText("Excoll");
				}
			}
			break;
		}
		else
		{
			if(isMemCollected[i])
			{
				MemObjects[i]->PlayAnimation(1);
			}
			else
			{
				MemObjects[i]->SetAlpha(0.5f);
			}
		}
	}

	if(!isAnyObjectMouseOver)
	{
		if(ObjMouseOverFX->Active)
			ObjMouseOverFX->StopEmit();
	}

	if(BackBtn->Collide(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BackBtn->PlayAnimation(1);
		if(Control::Input->LBclicked())
		{
			BackBtn->PlayAnimation(2);
			//Back to Scene or to Main Menu
			if(ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromExtras)//inside game
			{
				ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea = ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea;
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_ExtrasRoom;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
			else if(ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromMainMenu)//main menu
				SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
			else
			{
				if( ProfileSystem::Instance->ActiveProfile.CurrentArea == -1 )//cleared 8 levels and jumped into memories from credits page
					SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
				else
				{
					SendMessage(ControlExit, ID, HOSM_NewArea);
					ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;
				}
			}
			return;
		}
	}
	else
	{
		BackBtn->PlayAnimation(0);
	}
}

float KidsRoom::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void KidsRoom::UpdatePopupTransition()
{
	if( IsPopupActive )
	{
		switch( Popup_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;
				MemPopScene->Show(true);
				Popup_State = POPUP_OPEN;
			}
			PopMaskBg->SetAlpha(PopupAlpha);
			break;
		case POPUP_SCALEDOWN:
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;
				PopMaskBg->Show(false);
				Popup_State = POPUP_CLOSED;
				
				IsPopupActive = false;
			}		
			else
				PopMaskBg->SetAlpha(PopupAlpha);
			break;
		}
	}
}

void  KidsRoom::SetupPopupTransition(CHOScene *Popup, int popx, int popy)
{
	
		IsPopupTransitionActive = true;
    if( GFInstance->IsRTCreated )
    {
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS	
		GFInstance->IsRenderingToRT = true;
    }
		Popup->RenderToTarget(-171,0);
    if( GFInstance->IsRTCreated )
    {
		GFInstance->IsRenderingToRT = false;
		GFInstance->RTGraphic->endRenderToTarget();
    }
		GFInstance->SetPopupTween( (popx) ,(popy),171,0);

		PopMaskBg->SetAlpha(0);
		PopMaskBg->Show(true);
}

void KidsRoom::Render(const CPoint* pCam)
{  
   static int FrameCnt = 0;
   FrameCnt = (FrameCnt+1)%2;

   if(IsPopupActive && IsPopupTransitionActive)
	{
            //Init again if render target lost due to fullscreen-window switches or similar
            if( GFInstance->RTGraphic != NULL )
			GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
			short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
			short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
			-5*(1.0f-PopupAlpha),
			//0.2f,0.6f); //test gap between frames
			PopupAlpha,PopupAlpha);
    }
}

/////////////////////////////////////////////////////
void KidsRoom::InitMessageBanner()
{	
	IsBannerStandStill = false;
	TransOutFactor = 1.0f;
	BannerTimer = 0.0f;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;

	MessageText = new CBitmapText();
	MessageText->SetZ(eZOrder_Banner+1);
	MessageText->SetAlignment(Align_Center);
	MessageText->SetPos(&GFApp->MessageTextPos);
	MessageText->LoadFont(eFont_26);
	//MessageText->SetText("Message Text");
	MessageText->SetText(StringTable::getStr("ExMessage_Text"));

	const CPoint MessageBannerPos(0.0f, 0.0f, 0.0f);
	MessageElement = new CObject();
	MessageElement->LoadSprite("UI\\BNR.SPR");
	MessageElement->SetZ(eZOrder_Banner);//above back button
	MessageElement->SetPos(&MessageBannerPos);

	HideMessageBanner();
}

bool KidsRoom::HideMessageBanner(void)
{
	bool WasActiveMessage = MessageElement->Visible;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;
	MessageElement->Show(false);
	MessageText->Show(false);

	return WasActiveMessage;
}


void KidsRoom::TransOutBanner()
{
	if( BannerState == eHMS_TransIn )
	{
		BannerTimer = 0.0f;
		BannerState = eHMS_TransOut;
	}
	else
		TransOutFactor = 2.0f;
}


void KidsRoom::ShowMessageBanner(const std::string& message, int SFXId)
{
	IsBannerStandStill = false;
    TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;	
	
	MessageText->Show(true);
	MessageText->SetText(message);
	MessageElement->Show(true);

	MessageElement->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}

void KidsRoom::ShowBannerText(const std::string& keyString, int SFXId)
{
	IsBannerStandStill = false;
    TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;	
	
	MessageText->Show(true);
	MessageText->SetText(StringTable::getStr(keyString));
	MessageElement->Show(true);

	MessageElement->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}

void KidsRoom::UpdateBanner()
{
	if( BannerState == eHMS_Idle )
		return;

	if( BannerState == eHMS_TransIn )
	{
		BannerAlpha += 2.0f * Control::LogicRate;

		if( BannerAlpha >= MAX_BANNER_ALPHA )
		{
			BannerAlpha = MAX_BANNER_ALPHA;
		}

		BannerTimer += Control::LogicRate;
		if( BannerTimer >= 3.0f  || (BannerAlpha == MAX_BANNER_ALPHA && Control::Input->LBclicked()) )
		{
			BannerTimer = 0.0f;
			BannerState = eHMS_TransOut;
		}
	}
	else if( !IsBannerStandStill )
	{
		BannerAlpha -= TransOutFactor * Control::LogicRate;

		if( BannerAlpha <= 0.0f )
		{
			BannerAlpha = 0.0f;
			BannerState = eHMS_Idle;
			MessageElement->Show(false);
			MessageText->Show(false);
			BannerCloseNotify();
		}
	}

	MessageElement->SetAlpha(BannerAlpha);
	float MTAlpha = ( BannerAlpha > MAX_BANNERTEXT_ALPHA )? MAX_BANNERTEXT_ALPHA: BannerAlpha;
	MessageText->SetAlpha(MTAlpha);
}

KidsRoom::~KidsRoom()
{
	for (int i = 0; i < eMem_MAX; i++)
	{
		SAFE_DELETE(MemPolyArray[i]);
	}

	SAFE_DELETE(MemPopScene);
	SAFE_DELETE(PopMaskBg);

	SAFE_DELETE(txtMemoryCollectedCount);

	if( Control::Audio->IsPlaying(aTrackKidRoom))
		Control::Audio->StopSample(aTrackKidRoom);

	Control::Audio->UnloadSample(aTrackKidRoom);

	Control::Audio->UnloadSample(aSFXKidRoomBell1);
	Control::Audio->UnloadSample(aSFXKidRoomBell2);
	Control::Audio->UnloadSample(aSFXKidRoomBell3);
	Control::Audio->UnloadSample(aSFXKidRoomBell4);

	SAFE_DELETE(windowFX);
	SAFE_DELETE(LampFX);
	SAFE_DELETE(ObjMouseOverFX);
	SAFE_DELETE(lastObjFX);

	delete MessageElement;
	delete MessageText;
}
