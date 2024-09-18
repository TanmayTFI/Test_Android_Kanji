//====================================
//	DQFYLH
//	HO System
//	AwardsRoom
//====================================

#include "AwardsRoom.h"
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
#include "AwardManager.h"
#include "GameTextManager.h"

const CPoint	CenterPoint(683,334,0);

const CRectangle  MemoryRect(233.0f,431.0f,195.0f,295.0f);
const CRectangle  AwardRect(943.0f,430.0f,202.0f,297.0f);
const CRectangle  MorphRect(480.0f,104.0f,416.0f,518.0f);

const CRectangle  BtnRectBackButton(166.0f,659.0f,136.0f,79.0f);

const CRectangle  BtnRectPrevPage(174.0f,232.0f,93.0f,114.0f);
const CRectangle  BtnRectNextPage(1094.0f,235.0f,109.0f,116.0f);

const CRectangle  SubIconRect3(378.0f,567.0f,101.0f,47.0f);
const CRectangle  SubIconRect5(632.0f,401.0f,96.0f,51.0f);
const CRectangle  SubIconRect13(954.0f,301.0f,81.0f,43.0f);

const std::string AwardTextArray[kAward_Max] = 
{
	"Exachieve",				//0 DRONA
	"Ex1hop", 	//1 NA SUCITA 1
	"Ex3hop", 		//32 NA SUCITA 2
	"Ex5hop",			//33 NA SUCITA 3
	"Ex10hop",		//34 NA SUCITA 4
	"Exbring",			//2 Arjuna
	"Exho150",			//3 Vayu
	"Exho90",			//3
	"Exho30",			//3
	"Exhopmatch",				//4 Sama kanta
	"Exf2",			//5 Agni
	"Exf4",			//5
	"Exf6",			//5
	"Exminute",			//6 GURU
	"Exjigsaw",	//7 NIRMOKTR
	"Exitem",			//8 PRASPHURANA
	"Exmini", //9 CHANAKYA
	"Exwithout",	//10 MA CARA
	"Exhop",		//11 HOP PATI
	"Exdialogues",	//12 TARTARITI
	"Ex10hr",			//13 KARMA
	"Ex4hr",	//13 
	"Exjourney",									//14 PRARAMBHA
	"Exlevel",			//15 DEVARUPIN
	"Excreek",	//16 AVARA CARA
	"Exfast",							//17 SIDDHI
	"Exgot",						//18 UDGHATIN
	"Exn33",										//19 UPALAMBHAKA
	"Ex1mem",			//20 SITA
	"Exall",			//21 MATR RAKSA
	"Ex1morph",			//22 DRASTTA
	"Exallmorph",		//23 GARUDA
	"Extutorial",		//24 ADHIMUKTI
	"Extablet",				//25 ZISAVA
	"Exunveil",				//26 PATHAKA
	"Exwreck",		//27 KAUSHIK
	"Exindus",				//28 PATHIKA
	"Exsave",					//29 RAKSHAK
	"Extame",		//30 DAMAKA
	"Exdecrypt",		//31 VIGRAHA
	"Exsoldier"		//35 Ajna Chakra
};

const int Page1_IDArray[NUM_PAGE1_AWARDS] = {0, 2, 3, 5, 6, 9, 13, 20, 23, 27, 28, 29, 30, 31, 35};
const int Page2_IDArray[NUM_PAGE2_AWARDS] = {1, 4, 7, 8, 10, 11, 12, 14, 15, 16, 17, 18, 19, 21, 22, 24, 25, 26, 32, 33, 34};

//0//0 DRONA
	//1//1 NA SUCITA 1
	//2//32 NA SUCITA 2
	//3//33 NA SUCITA 3
	//4//34 NA SUCITA 4
	//5//2 Arjuna
	//6//3 Vayu
	//7//3
	//8//3
	//9//4 Sama kanta
	//10//5 Agni
	//11//5
	//12//5
	//13//6 GURU
	//14//7 NIRMOKTR
	//15//8 PRASPHURANA
	//16//9 CHANAKYA
	//17//10 MA CARA
	//18//11 HOP PATI
	//19//12 TARTARITI
	//20//13 KARMA
	//21//13 
	//22//14 PRARAMBHA
	//23//15 DEVARUPIN
	//24//16 AVARA CARA
	//25//17 SIDDHI
	//26//18 UDGHATIN
	//27//19 UPALAMBHAKA
	//28//20 SITA
	//29//21 MATR RAKSA
	//30//22 DRASTTA
	//31//23 GARUDA
	//32//24 ADHIMUKTI
	//33//25 ZISAVA
	//34//26 PATHAKA
	//35//27 KAUSHIK
	//36//28 PATHIKA
	//37//29 RAKSHAK
	//38//30 DAMAKA
	//39//31 VIGRAHA
	//40//35 AJNA CHAKRA - To Add

int pageAwardToTextMapping[] ={
	0, 1, 5, 0, 9, 0,
	13, 14, 15, 16, 17, 18,
	19, 0, 22, 23, 24, 25,
	26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37,
	38, 39, 2, 3, 4, 40
};

AwardsRoom::AwardsRoom()
{
	MessageText = NULL;
	MessageElement = NULL;
	LoadScene("EXTRAS\\AWARDS\\AWD.SCN");

	//Localized
	TextTitle = new CObject();
	TextTitle->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\EXTRAS\\AWARDS\\TITLETEXT.SPR", 684, 650, 14);
	TextTitle->SetUID("TitleText");
	HOScene->PushObject(TextTitle);

	TextFrameBG = new CObject();
	TextFrameBG->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\EXTRAS\\AWARDS\\TXTFRAME.SPR", 685, 697, 13);
	TextFrameBG->SetUID("txtframe");
	HOScene->PushObject(TextFrameBG);
	

	HOScene->SetZOff(eZOrder_INVBOXITEMS+10);

	BackBtnGlow = HOScene->GetObjectByID("backbtnglow");
	BackBtnGlow->Show(false);

	BackBtnText = new CObject();
	BackBtnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", BackBtnGlow->Pos.x, BackBtnGlow->Pos.y-2, BackBtnGlow->ZOrder+1);
	BackBtnText->SetUID("BackBtnText");
	HOScene->PushObject(BackBtnText);
	BackBtnText->SetRotate(12);
	BackBtnText->SetScale(0.9);

	HOScene->GetObjectByID("RCrystalGlow")->Animate(0.45f, 1.0f, 2.5f, 1.0f);
	HOScene->GetObjectByID("LCrystalGlow")->Animate(0.35f, 1.0f, 3.0f);

	HOScene->GetObjectByID("lightglowleft")->Animate(0.35f, 1.0f, 4.0f);
	HOScene->GetObjectByID("lightglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightglowright")->Animate(0.35f, 0.90f, 4.0f, 2.15f);
	HOScene->GetObjectByID("lightglowright")->SetBlendMode(eBlend_AlphaAdditive);

	ColorRGB white = {0.90f, 0.90f, 0.80f};
	float _lineWidth = 500;
	int _nLines = 3;
	EFont _font = eFont_26;
	eTextAlignment _align = Align_Center;
	std::string temp = AwardTextArray[0];
	if(StringTable::IsKeyFound(temp))
	{
		_lineWidth = StringTable::GetLineWidth(temp);
		_nLines = StringTable::GetNoOfLines(temp);
		_font = (EFont)StringTable::GetFont(temp);
	}
	TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);
	TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));
	TextAwardDescription->SetScale(0.75f);
	//TextAwardDescription->Show(false);

	TextFrameBG->Show(false);
	TextTitle->Show(false);
	TextAwardDescription->Show(false);
	
	PopCloseBtn = NULL;

	PopMaskBg = new CObject();
	PopMaskBg->LoadSprite("UI\\MASK.SPR");
	PopMaskBg->SetXY(683,384);
	PopMaskBg->SetScale(2.0);
	PopMaskBg->SetZ(eZOrder_INVBOXITEMS+30);
	PopMaskBg->Show(false);

	InitMessageBanner();

	pageTab1 = HOScene->GetObjectByID("tabcircle1");
	pageTab2 = HOScene->GetObjectByID("tabcircle2");

	btnNextPageActiveIcon = HOScene->GetObjectByID("btnNextPageGlow");
	btnNextPageMouseOver = HOScene->GetObjectByID("btnNextPageMouseover");

	btnPrevPageActiveIcon = HOScene->GetObjectByID("btnPrevPageGlow");
	btnPrevPageMouseOver = HOScene->GetObjectByID("btnPrevPageMouseover");

	currentPage = 0;

	//awards subawards
	pg1Subicon5[0] = HOScene->GetObjectByID("subicon5bronze");
	pg1Subicon5[1] = HOScene->GetObjectByID("subicon5silver");
	pg1Subicon5[2] = HOScene->GetObjectByID("subicon5gold");

	pg1Subicon13[0] = HOScene->GetObjectByID("subicon13silver");
	pg1Subicon13[1] = HOScene->GetObjectByID("subicon13gold");

	pg1Subicon3[0] = HOScene->GetObjectByID("subicon3bronze");
	pg1Subicon3[1] = HOScene->GetObjectByID("subicon3silver");
	pg1Subicon3[2] = HOScene->GetObjectByID("subicon3gold");

	//awards subawards - zOrder
	pg1Subicon5[0]->SetZ(eZOrder_INVBOXITEMS+80);
	pg1Subicon5[1]->SetZ(eZOrder_INVBOXITEMS+80);
	pg1Subicon5[2]->SetZ(eZOrder_INVBOXITEMS+80);

	pg1Subicon13[0]->SetZ(eZOrder_INVBOXITEMS+80);
	pg1Subicon13[1]->SetZ(eZOrder_INVBOXITEMS+80);

	pg1Subicon3[0]->SetZ(eZOrder_INVBOXITEMS+80);
	pg1Subicon3[1]->SetZ(eZOrder_INVBOXITEMS+80);
	pg1Subicon3[2]->SetZ(eZOrder_INVBOXITEMS+80);

	subIconGlow = HOScene->GetObjectByID("subiconglow");
	subIconGlow->SetZ(eZOrder_INVBOXITEMS+90);
	subIconGlow->SetEnable(false);

	for (int i = 0; i < kAward_Max; i++)
	{
		isAwardCollected[i] = AwardManager::GetInstance()->IsAwardCompleted((AwardCategory)i);
	}

	//// Init Awards PAGE 1
	SCN_Page1 = new CHOScene();
	SCN_Page1->Load("EXTRAS\\AWARDS\\PG1\\AWDPG1.SCN");
	SCN_Page1->SetZOff(eZOrder_INVBOXITEMS+20);

	for (int i = 0; i < NUM_PAGE1_AWARDS; i++)
	{
		int awardID = Page1_IDArray[i];
	//	K_LOG("load page 1 item %d",awardID);

		pg1AwardGlows[i] = SCN_Page1->GetObjectByID(std::string("rndglow")+MKSTR(awardID));
		pg1AwardGlows[i]->SetBlendMode(eBlend_AlphaAdditive);
		pg1AwardGlows[i]->SetEnable(false);

		pg1AwardActiveGlows[i] = SCN_Page1->GetObjectByID(std::string("activeglow")+MKSTR(awardID));
		pg1AwardActiveGlows[i]->SetBlendMode(eBlend_AlphaAdditive);
		pg1AwardActiveGlows[i]->SetEnable(false);

		pg1AwardIcons[i] = SCN_Page1->GetObjectByID(std::string("awardicon")+MKSTR(awardID));

		
		if(awardID == 3) //vayu
		{
			//select from one of symbols
			if(isAwardCollected[6])
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon3[0]->PlayAnimation(1);
			}
			if(isAwardCollected[7])
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon3[1]->PlayAnimation(2);
			}
			if(isAwardCollected[8])
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon3[2]->PlayAnimation(3);
			}
		}
		else if(awardID == 5) //agni
		{
			//select from one of symbols
			if(isAwardCollected[10])
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon5[0]->PlayAnimation(1);
			}
			if(isAwardCollected[11])
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon5[1]->PlayAnimation(2);
			}
			if(isAwardCollected[12])
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon5[2]->PlayAnimation(3);
			}			
		}
		else if(awardID == 13) //karma
		{
			//select from one of symbols
			if(isAwardCollected[20])
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon13[0]->PlayAnimation(2);
			}
			if(isAwardCollected[21])
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon13[1]->PlayAnimation(3);
			}
		}
		else
		{
			//from the award Id obtained map to the award enum
			int awardMappedID = pageAwardToTextMapping[awardID];
			if(isAwardCollected[awardMappedID])
			{
				pg1AwardIcons[i]->PlayAnimation(1);
			}
		}
	}
	//// End of PAGE 1 Stuffs

	//// Init Awards PAGE 2
	SCN_Page2 = new CHOScene();
	SCN_Page2->Load("EXTRAS\\AWARDS\\PG2\\AWDPG2.SCN");
	SCN_Page2->SetZOff(eZOrder_INVBOXITEMS+20);

	for (int i = 0; i < NUM_PAGE2_AWARDS; i++)
	{
		int awardID = Page2_IDArray[i];
	//	K_LOG("load page 2 item %d",awardID);

		pg2AwardGlows[i] = SCN_Page2->GetObjectByID(std::string("rndglow")+MKSTR(awardID));
		pg2AwardGlows[i]->SetBlendMode(eBlend_AlphaAdditive);
		pg2AwardGlows[i]->SetEnable(false);

		pg2AwardActiveGlows[i] = SCN_Page2->GetObjectByID(std::string("activeglow")+MKSTR(awardID));
		pg2AwardActiveGlows[i]->SetBlendMode(eBlend_AlphaAdditive);
		pg2AwardActiveGlows[i]->SetEnable(false);

		pg2AwardIcons[i] = SCN_Page2->GetObjectByID(std::string("awardicon")+MKSTR(awardID));

		if(awardID == 8)
			pg2AwardIcons[i]->SetEnable(false); //Removing one award
		
		//from the award Id obtained map to the award enum
		int awardMappedID = pageAwardToTextMapping[awardID];
		if(isAwardCollected[awardMappedID])
		{
			pg2AwardIcons[i]->PlayAnimation(1);
		}
	}
	//// End of PAGE 2 Stuffs

	setupCurrentPage();

	Popup_State = POPUP_CLOSED;

	IsPopupActive = false;
	IsPopupTransitionActive = false;
	PopupAlpha = 0;
	PopVal = 0;

	WindFX = new SuperFX("PRT\\XTRA\\winddots.INI", PointSystem::CreateCPoint(643,324,0), eZOrder_INVBOXITEMS+100);
	GlareFX = new SuperFX("PRT\\XTRA\\glares.INI", PointSystem::CreateCPoint(688,131,0), eZOrder_INVBOXITEMS+100);
	LeftCrystalFX = new SuperFX("PRT\\XTRA\\leftcrystal.INI", PointSystem::CreateCPoint(288,668,0), eZOrder_INVBOXITEMS+100); 
	RightCrystalFX = new SuperFX("PRT\\XTRA\\rightcrystal.INI", PointSystem::CreateCPoint(962,642,0), eZOrder_INVBOXITEMS+100); 

	WindFX->Start();
	WindFX->AdvanceByTime(2.0f);

	LeftCrystalFX->Start();
	RightCrystalFX->Start();
	GlareFX->Start();

	leftPlantMesh = new MeshObject("EXTRAS\\AWARDS\\PLANTLEFT.JPA", 5, 5);
	leftPlantMesh->SetPosAndParams(119, 324, 1.5f, 1.25f, 6, 4);
	leftPlantMesh->SetZ(eZOrder_INVBOXITEMS+33);

	rightPlantMesh = new MeshObject("EXTRAS\\AWARDS\\PLANTRIGHT.JPA", 5, 5);
	rightPlantMesh->SetPosAndParams(1072, 363, 1.5f, 1.5f, 4, 6);
	rightPlantMesh->SetZ(eZOrder_INVBOXITEMS+33);


	SetZ(eZOrder_INVBOXITEMS+200);
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_AwardsRoom] = true;
	
	

	Control::Audio->TurnOffLvlMscTracks();//turns off music-snd carryover....

	Control::Audio->LoadSample(aTrackKidRoom, AudioTrack,true);
	Control::Audio->PlaySample(aTrackKidRoom,true);


	//Setting the Current Area, to prevent if user Force exit while here
	ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea;

	Visible = true;	//To render anything with renderlistentity especially Video
}

void AwardsRoom::setupCurrentPage()
{
	btnNextPageMouseOver->Show(false);
	btnPrevPageMouseOver->Show(false);
	if(currentPage==0)
	{
		btnNextPageActiveIcon->Show(true);

		btnPrevPageActiveIcon->Show(false);

		pageTab1->PlayAnimation(1);
		pageTab2->PlayAnimation(0);

		SCN_Page2->Show(false);
		SCN_Page1->Show(true);

		subIconGlow->SetEnable(false);

		//awards subawards
		pg1Subicon5[0]->SetEnable(true);
		pg1Subicon5[1]->SetEnable(true);
		pg1Subicon5[2]->SetEnable(true);

		pg1Subicon13[0]->SetEnable(true);
		pg1Subicon13[1]->SetEnable(true);

		pg1Subicon3[0]->SetEnable(true);
		pg1Subicon3[1]->SetEnable(true);
		pg1Subicon3[2]->SetEnable(true);
	}
	else if(currentPage==1)
	{
		btnNextPageActiveIcon->Show(false);

		btnPrevPageActiveIcon->Show(true);

		subIconGlow->SetEnable(false);

		pageTab1->PlayAnimation(0);
		pageTab2->PlayAnimation(1);

		SCN_Page1->Show(false);
		SCN_Page2->Show(true);

		//awards subawards - disable
		pg1Subicon5[0]->SetEnable(false);
		pg1Subicon5[1]->SetEnable(false);
		pg1Subicon5[2]->SetEnable(false);

		pg1Subicon13[0]->SetEnable(false);
		pg1Subicon13[1]->SetEnable(false);

		pg1Subicon3[0]->SetEnable(false);
		pg1Subicon3[1]->SetEnable(false);
		pg1Subicon3[2]->SetEnable(false);
	}
}

void AwardsRoom::Update()
{
	Cursor::SetMode(CA_Normal);	
	CPoint pos = Control::Input->Mpos();

	UpdateBanner();

	if(IsPopupTransitionActive)
		UpdatePopupTransition();

	if(currentPage==0)
	{
		if(BtnRectNextPage.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			btnNextPageMouseOver->Show(true);
			if(Control::Input->LBclicked())
			{
				btnNextPageMouseOver->Show(false);
				currentPage = 1;
				setupCurrentPage();
				return;
			}
		}
		else
			btnNextPageMouseOver->Show(false);

		for (int i = 0; i < NUM_PAGE1_AWARDS; i++)
		{
			int awardID = Page1_IDArray[i];
			if (pg1AwardIcons[i]->Collide(&pos, -20) || (awardID == 3 && SubIconRect3.Intersection(&pos)) || (awardID == 5 && SubIconRect5.Intersection(&pos)) || (awardID == 13 && SubIconRect13.Intersection(&pos)) )
			{

//#ifdef _CHEATS
//				if (Control::Input->LBclicked())
//				{
//					Award * award = NULL;
//					int id = pageAwardToTextMapping[awardID];
//					if (awardID == 3)
//					{
//						award = AwardManager::GetInstance()->GetAward(kAward_HO_Within_Time_150);
//						AwardManager::GetInstance()->UpdateEvents(kAward_HO_Within_Time_150, 1);
//
//						award = AwardManager::GetInstance()->GetAward(kAward_HO_Within_Time_90);
//						AwardManager::GetInstance()->UpdateEvents(kAward_HO_Within_Time_90, 1);
//
//						award = AwardManager::GetInstance()->GetAward(kAward_HO_Within_Time_30);
//						AwardManager::GetInstance()->UpdateEvents(kAward_HO_Within_Time_30, 1);
//					}
//					else if (awardID == 5)
//					{
//						award = AwardManager::GetInstance()->GetAward(kAward_Mini_Without_Skip_2);
//						AwardManager::GetInstance()->UpdateEvents(kAward_Mini_Without_Skip_2, 1);
//
//						award = AwardManager::GetInstance()->GetAward(kAward_Mini_Without_Skip_4);
//						AwardManager::GetInstance()->UpdateEvents(kAward_Mini_Without_Skip_4, 1);
//
//						award = AwardManager::GetInstance()->GetAward(kAward_Mini_Without_Skip_6);
//						AwardManager::GetInstance()->UpdateEvents(kAward_Mini_Without_Skip_6, 1);
//					}
//					else if (awardID == 13)
//					{
//						award = AwardManager::GetInstance()->GetAward(kAward_Game_Within_Time_10hr);
//						AwardManager::GetInstance()->UpdateEvents(kAward_Game_Within_Time_10hr, 1);
//
//						award = AwardManager::GetInstance()->GetAward(kAward_Game_Within_Time_4hr);
//						AwardManager::GetInstance()->UpdateEvents(kAward_Game_Within_Time_4hr, 1);
//					}
//					else
//					{
//						award = AwardManager::GetInstance()->GetAward((AwardCategory)id);
//						AwardManager::GetInstance()->UpdateEvents((AwardCategory)id, 1);
//						
//					}
//					UpdateIcons();
//				}  
//#endif // _CHEATS

				Cursor::SetMode(CA_HandPointy);

				bool isAwardActive = false;

				subIconGlow->SetEnable(false);

				if(awardID == 3) //vayu
				{
					//select from one of symbols
					//if(isAwardCollected[6] || isAwardCollected[7] || isAwardCollected[8])
					{

						//if(isAwardCollected[6] ) //&& pg1Subicon3[0]->Collide(&pos) first is default
						{
							ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[6];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);
	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));

							TextTitle->PlayAnimation(awardID);

							if(isAwardCollected[6] )
								isAwardActive = true;

							subIconGlow->SetPos(&pg1Subicon3[0]->Pos);
							subIconGlow->SetEnable(true);
						}
						if(pg1Subicon3[1]->Collide(&pos))
						{
							ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[7];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);
	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));
							TextTitle->PlayAnimation(awardID);

							if(isAwardCollected[7])
								isAwardActive = true;

							subIconGlow->SetPos(&pg1Subicon3[1]->Pos);
							subIconGlow->SetEnable(true);
						}
						if(pg1Subicon3[2]->Collide(&pos))
						{
							ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[8];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);
	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));
							TextTitle->PlayAnimation(awardID);

							if(isAwardCollected[8])
								isAwardActive = true;

							subIconGlow->SetPos(&pg1Subicon3[2]->Pos);
							subIconGlow->SetEnable(true);
						}
					}
				}
				else if(awardID == 5) //agni
				{
					//if(isAwardCollected[10] || isAwardCollected[11] || isAwardCollected[12])
					{
						//select from one of symbols
						//if(isAwardCollected[10]) //&& pg1Subicon5[0]->Collide(&pos) first is default
						{
							ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[10];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);
	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));
							TextTitle->PlayAnimation(awardID);

							if(isAwardCollected[10])
								isAwardActive = true;

							subIconGlow->SetPos(&pg1Subicon5[0]->Pos);
							subIconGlow->SetEnable(true);
						}
						if(pg1Subicon5[1]->Collide(&pos))
						{
							ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[11];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);
	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));
							TextTitle->PlayAnimation(awardID);

							if(isAwardCollected[11])
								isAwardActive = true;

							subIconGlow->SetPos(&pg1Subicon5[1]->Pos);
							subIconGlow->SetEnable(true);
						}
						if(pg1Subicon5[2]->Collide(&pos))
						{
							ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[12];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);
	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));
							TextTitle->PlayAnimation(awardID);

							if(isAwardCollected[12])
								isAwardActive = true;

							subIconGlow->SetPos(&pg1Subicon5[2]->Pos);
							subIconGlow->SetEnable(true);
						}	
					}
				}
				else if(awardID == 13) //karma
				{
					//select from one of symbols
					//if(isAwardCollected[20] || isAwardCollected[21])
					{

						//&& pg1Subicon13[0]->Collide(&pos) first is default
						{
							ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[20];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);
	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));
							TextTitle->PlayAnimation(awardID);

							if(isAwardCollected[20] )
								isAwardActive = true;

							subIconGlow->SetPos(&pg1Subicon13[0]->Pos);
							subIconGlow->SetEnable(true);
						}
						if(pg1Subicon13[1]->Collide(&pos))
						{
							ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[21];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);
	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));
							TextTitle->PlayAnimation(awardID);

							if(isAwardCollected[21])
								isAwardActive = true;

							subIconGlow->SetPos(&pg1Subicon13[1]->Pos);
							subIconGlow->SetEnable(true);
						}
					}
				}
				else
				{
					//from the award Id obtained map to the award enum
					int awardMappedID = pageAwardToTextMapping[awardID];
					//if(isAwardCollected[awardMappedID])
					{
						ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[awardMappedID];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));

						TextTitle->PlayAnimation(awardID);

						if(isAwardCollected[awardMappedID])
						{
							isAwardActive = true;
							pg1AwardIcons[i]->PlayAnimation(1);
						}
					}
				}

				if(!isAwardActive)
				{
					pg1AwardGlows[i]->SetEnable(true);

					TextTitle->Show(true);
					TextFrameBG->Show(true);
					TextAwardDescription->Show(true);
				}
				else
				{
					TextTitle->Show(true);
					TextFrameBG->Show(true);
					TextAwardDescription->Show(true);

					pg1AwardActiveGlows[i]->SetEnable(true);
				}

				if(!isAwardActive && Control::Input->LBclicked())
				{
					ShowBannerText("Exlock");
					//ShowMessageBanner(AwardTextArray[Page1_IDArray[i]]);
				}
				break;
			}
			else
			{
				pg1AwardGlows[i]->SetEnable(false);
				pg1AwardActiveGlows[i]->SetEnable(false);

				TextTitle->Show(false);
				TextFrameBG->Show(false);
				TextAwardDescription->Show(false);
			}
		}
	}
	else if(currentPage==1)
	{
		if(BtnRectPrevPage.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			btnPrevPageMouseOver->Show(true);
			if(Control::Input->LBclicked())
			{
				btnPrevPageMouseOver->Show(false);
				currentPage = 0;
				setupCurrentPage();
				return;
			}
		}
		else
		{
			btnPrevPageMouseOver->Show(false);
		}

		for (int i = 0; i < NUM_PAGE2_AWARDS; i++)
		{
			int awardID = Page2_IDArray[i];
			if (pg2AwardIcons[i]->Collide(&pos, -20) && awardID!=8)
			{

				Cursor::SetMode(CA_HandPointy);
#ifdef _CHEATS
				/*if (Control::Input->LBclicked())
				{
					int id = pageAwardToTextMapping[awardID];
					Award * award = AwardManager::GetInstance()->GetAward((AwardCategory)id);
					AwardManager::GetInstance()->UpdateEvents((AwardCategory)id, 1);
					UpdateIcons();
				}*/  
#endif // _CHEATS

				bool isAwardActive = false;

				//from the award Id obtained map to the award enum
				int awardMappedID = pageAwardToTextMapping[awardID];
				//if(isAwardCollected[awardMappedID])
				{
					ColorRGB white = {0.90f, 0.90f, 0.80f};
							float _lineWidth = 500;
							int _nLines = 3;
							EFont _font = eFont_26;
							eTextAlignment _align = Align_Center;
							std::string temp = AwardTextArray[awardMappedID];
							if(StringTable::IsKeyFound(temp))
							{
								_lineWidth = StringTable::GetLineWidth(temp);
								_nLines = StringTable::GetNoOfLines(temp);
								_font = (EFont)StringTable::GetFont(temp);
							}
							SAFE_DELETE(TextAwardDescription);
							TextAwardDescription = new MultilineText(_nLines, PointSystem::CreateCPoint(TextFrameBG->Pos.x, TextFrameBG->Pos.y-15.0f, 0), eZOrder_INVBOXITEMS+50, white, 30, _lineWidth, _align, _font);	
							TextAwardDescription->SetTextMultiline(StringTable::getStr(temp));
					TextTitle->PlayAnimation(awardID);

					if(isAwardCollected[awardMappedID])
					{
						isAwardActive = true;
						pg2AwardIcons[i]->PlayAnimation(1);
					}
				}
				
				if(!isAwardActive)
				{
					pg2AwardGlows[i]->SetEnable(true);

					TextTitle->Show(true);
					TextFrameBG->Show(true);
					TextAwardDescription->Show(true);
				}
				else
				{
					TextTitle->Show(true);
					TextFrameBG->Show(true);
					TextAwardDescription->Show(true);

					pg2AwardActiveGlows[i]->SetEnable(true);
				}

				if(!isAwardActive && Control::Input->LBclicked())
				{
					ShowBannerText("Exlock");
					//ShowMessageBanner(AwardTextArray[Page1_IDArray[i]]);
				}
				break;
			}
			else
			{
				pg2AwardGlows[i]->SetEnable(false);
				pg2AwardActiveGlows[i]->SetEnable(false);

				TextTitle->Show(false);
				TextFrameBG->Show(false);
				TextAwardDescription->Show(false);
			}
		}
	}
#ifdef ENABLE_ANDROID_BACK_BUTTON // Handle Back button gracefully
	if (Control::Input->isKeyBackSensed()) 
	{
			//Back to Scene or to Extras Scene
			if(ProfileSystem::Instance->ActiveProfile.IsAwardsRoomFromExtrasScene)
			{
				if(!(ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_KidsRoom && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_AwardsRoom))
					ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_ExtrasRoom;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
			else
			{
				//Back to Game Scene
				SendMessage(ControlExit, ID, HOSM_NewArea);
				ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;
			}				
			return;
	
	}
#endif

	if(BtnRectBackButton.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BackBtnGlow->Show(true);
		if(Control::Input->LBclicked())
		{
			BackBtnGlow->Show(false);
			//Back to Scene or to Extras Scene
			if(ProfileSystem::Instance->ActiveProfile.IsAwardsRoomFromExtrasScene)
			{
				if(!(ProfileSystem::Instance->ActiveProfile.CurrentArea >= eArea_KidsRoom && ProfileSystem::Instance->ActiveProfile.CurrentArea <= eArea_AwardsRoom))
					ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;				
				//ProfileSystem::Instance->ActiveProfile.MemoryRoomPrevArea = ProfileSystem::Instance->ActiveProfile.CurrentArea;
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_ExtrasRoom;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
			else
			{
				//Back to Game Scene
				SendMessage(ControlExit, ID, HOSM_NewArea);
				ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;
			}				
			return;
		}
	}
	else
	{
		BackBtnGlow->Show(false);
	}
}

void AwardsRoom::UpdateIcons()
{
	for (int i = 0; i < NUM_PAGE1_AWARDS; i++)
	{
		int awardID = Page1_IDArray[i];
	//	K_LOG("load page 1 item %d",awardID);

		pg1AwardGlows[i] = SCN_Page1->GetObjectByID(std::string("rndglow")+MKSTR(awardID));

		pg1AwardActiveGlows[i] = SCN_Page1->GetObjectByID(std::string("activeglow")+MKSTR(awardID));

		pg1AwardIcons[i] = SCN_Page1->GetObjectByID(std::string("awardicon")+MKSTR(awardID));

		int awardMappedID = pageAwardToTextMapping[awardID];
		
		if(awardID == 3) //vayu
		{
			//select from one of symbols
			if(AwardManager::GetInstance()->IsAwardCompleted(kAward_HO_Within_Time_150))
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon3[0]->PlayAnimation(1);
			}
			if(AwardManager::GetInstance()->IsAwardCompleted(kAward_HO_Within_Time_90))
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon3[1]->PlayAnimation(2);
			}
			if(AwardManager::GetInstance()->IsAwardCompleted(kAward_HO_Within_Time_30))
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon3[2]->PlayAnimation(3);
			}
		}
		else if(awardID == 5) //agni
		{
			//select from one of symbols
			if(AwardManager::GetInstance()->IsAwardCompleted(kAward_Mini_Without_Skip_2))
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon5[0]->PlayAnimation(1);
			}
			if(AwardManager::GetInstance()->IsAwardCompleted(kAward_Mini_Without_Skip_4))
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon5[1]->PlayAnimation(2);
			}
			if(AwardManager::GetInstance()->IsAwardCompleted(kAward_Mini_Without_Skip_6))
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon5[2]->PlayAnimation(3);
			}			
		}
		else if(awardID == 13) //karma
		{
			//select from one of symbols
			if(AwardManager::GetInstance()->IsAwardCompleted(kAward_Game_Within_Time_10hr))
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon13[0]->PlayAnimation(2);
			}
			if(AwardManager::GetInstance()->IsAwardCompleted(kAward_Game_Within_Time_4hr))
			{
				pg1AwardIcons[i]->PlayAnimation(1);
				pg1Subicon13[1]->PlayAnimation(3);
			}
		}
		else
		{
			//from the award Id obtained map to the award enum
			
			
			if(AwardManager::GetInstance()->IsAwardCompleted((AwardCategory)awardMappedID))
			{
				pg1AwardIcons[i]->PlayAnimation(1);
			}
		}
	}


	for (int i = 0; i < NUM_PAGE2_AWARDS; i++)
	{
		int awardID = Page2_IDArray[i];
	//	K_LOG("load page 2 item %d",awardID);

		pg2AwardGlows[i] = SCN_Page2->GetObjectByID(std::string("rndglow")+MKSTR(awardID));

		pg2AwardActiveGlows[i] = SCN_Page2->GetObjectByID(std::string("activeglow")+MKSTR(awardID));

		pg2AwardIcons[i] = SCN_Page2->GetObjectByID(std::string("awardicon")+MKSTR(awardID));
		
		//from the award Id obtained map to the award enum
		int awardMappedID = pageAwardToTextMapping[awardID];
		bool isAwardCompleted = AwardManager::GetInstance()->IsAwardCompleted((AwardCategory)awardMappedID);
		if(isAwardCompleted)
		{
			pg2AwardIcons[i]->PlayAnimation(1);
		}
	}
}

float AwardsRoom::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void AwardsRoom::UpdatePopupTransition()
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
				//MemPopScene->Show(true);
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

void  AwardsRoom::SetupPopupTransition(CHOScene *Popup, int popx, int popy)
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

void AwardsRoom::Render(const CPoint* pCam)
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
void AwardsRoom::InitMessageBanner()
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
//	MessageText->SetText("Message Text");
	MessageText->SetText(StringTable::getStr("ExMessage_Text"));

	const CPoint MessageBannerPos(0.0f, 0.0f, 0.0f);
	MessageElement = new CObject();
	MessageElement->LoadSprite("UI\\BNR.SPR");
	MessageElement->SetZ(eZOrder_Banner);//above back button
	MessageElement->SetPos(&MessageBannerPos);

	HideMessageBanner();
}

bool AwardsRoom::HideMessageBanner(void)
{
	bool WasActiveMessage = MessageElement->Visible;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;
	MessageElement->Show(false);
	MessageText->Show(false);

	return WasActiveMessage;
}


void AwardsRoom::TransOutBanner()
{
	if( BannerState == eHMS_TransIn )
	{
		BannerTimer = 0.0f;
		BannerState = eHMS_TransOut;
	}
	else
		TransOutFactor = 2.0f;
}


void AwardsRoom::ShowMessageBanner(const std::string& message, int SFXId)
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

void AwardsRoom::ShowBannerText(const std::string& keyString, int SFXId)
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

void AwardsRoom::UpdateBanner()
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

AwardsRoom::~AwardsRoom()
{
	SAFE_DELETE(PopMaskBg);

	SAFE_DELETE(leftPlantMesh);
	SAFE_DELETE(rightPlantMesh);

	//PAGE 1 Stuffs
	SAFE_DELETE(SCN_Page1);

	//PAGE 2 Stuffs
	SAFE_DELETE(SCN_Page2);

	SAFE_DELETE(TextAwardDescription);

	if( Control::Audio->IsPlaying(aTrackKidRoom))
		Control::Audio->StopSample(aTrackKidRoom);

	Control::Audio->UnloadSample(aTrackKidRoom);

	SAFE_DELETE(WindFX);
	SAFE_DELETE(GlareFX);
	SAFE_DELETE(LeftCrystalFX);
	SAFE_DELETE(RightCrystalFX);

	delete MessageElement;
	delete MessageText;
}
