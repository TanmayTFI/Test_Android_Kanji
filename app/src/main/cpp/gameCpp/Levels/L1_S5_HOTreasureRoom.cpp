//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 05 :  HO in Treasure Room
//====================================

#include "L1_S5_HOTreasureRoom.h"
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
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Fonts.h"
#include "AwardManager.h"

const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,98,0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  InvRectLocket(676.0f,328.0f,110.0f,92.0f);
int	 P2SwapTokenIds[5][4]	= {{0,1,2,3}, {0,5,6,1}, {1,6,7,2}, {3,2,7,4}, {0,3,4,5}};
int	 P2ShuffleInitPosContains[8] = {7,4,0,6,1,2,3,5};
const CRectangle  ActionRectCloth(589.0f,493.0f,249.0f,94.0f);
const CRectangle  InvRectBookPart1(721.0f,564.0f,99.0f,59.0f);

const CRectangle  BtnRectMatch3(536.0f,2.0f,286.0f,51.0f);

const CRectangle  BookLockPart1Rect(599.0f,417.0f,67.0f,63.0f);
const CRectangle  BookLockPart2Rect(342.0f,403.0f,74.0f,59.0f);

const CRectangle  p2SkipRect(573.0f,550.0f,188.0f,70.0f);


//P1 book HO
const CRectangle  P1BookLockPart2Rect(860.0f,372.0f,124.0f,102.0f);
const CRectangle  P1BookLockPart1Rect(400.0f,243.0f,113.0f,105.0f);


L1_S5_HOTreasureRoom::L1_S5_HOTreasureRoom()
{
	/*DebugText = new CBitmapText();
	ColorRGB yellow={(153/255.0f),(254/255.0f),(150/255.0f)};
	DebugText->SetTextProps(">Debug", 20, 30, 5030, yellow, eFont_Italic24, Align_Left);*/

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S5_HOTreasureRoom])
		AwardManager::GetInstance()->StartHO(eArea_L1_S5_HOTreasureRoom);

	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S5\\HO\\HO.SCN");

	SCN_bookClosed = HOScene->GetObjectByID("bookClosed");
	SCN_bookLockPart1 = HOScene->GetObjectByID("bookLockPart1");
	SCN_bookLockPart2 = HOScene->GetObjectByID("bookLockPart2");
	SCN_bookLockPart3 = HOScene->GetObjectByID("bookLockPart3");
	SCN_bookLockPartFixed1 = HOScene->GetObjectByID("bookLockPartFixed1");
	SCN_bookLockPartFixed2 = HOScene->GetObjectByID("bookLockPartFixed2");
	SCN_bookLockPartFixed3 = HOScene->GetObjectByID("bookLockPartFixed3");
	SCN_bookLockPartFixed4 = HOScene->GetObjectByID("bookLockPartFixed4");
	SCN_bookOpen = HOScene->GetObjectByID("bookOpen");
	SCN_chestBoxOpen = HOScene->GetObjectByID("chestBoxOpen");
	SCN_chestBoxSymbol = HOScene->GetObjectByID("chestBoxSymbol");
	SCN_clockOpen = HOScene->GetObjectByID("clockOpen");
	SCN_clothFolded = HOScene->GetObjectByID("clothFolded");
	SCN_clothOpen = HOScene->GetObjectByID("clothOpen");
	SCN_eagleEye = HOScene->GetObjectByID("eagleEye");
	SCN_eagleEyeInHand = HOScene->GetObjectByID("eagleEyeInHand");
	SCN_eagleMouthOpen = HOScene->GetObjectByID("eagleMouthOpen");
	SCN_grinderWithPestile = HOScene->GetObjectByID("grinderWithPestile");
	SCN_grinderOnly = HOScene->GetObjectByID("grinderonly");
	SCN_handClosed = HOScene->GetObjectByID("handClosed");
	SCN_handle = HOScene->GetObjectByID("handle");
	SCN_keyOnBox = HOScene->GetObjectByID("keyOnBox");
	SCN_locket = HOScene->GetObjectByID("locket");
	SCN_locketOnScene = HOScene->GetObjectByID("locketOnScene");
	SCN_musicboxOpen = HOScene->GetObjectByID("musicboxOpen");
	SCN_pestle = HOScene->GetObjectByID("pestle");
	SCN_ringBox = HOScene->GetObjectByID("ringBox");
	SCN_ringBoxCoin = HOScene->GetObjectByID("ringBoxCoin");
	SCN_ringBoxOpen = HOScene->GetObjectByID("ringBoxOpen");
	SCN_statueHandOpen = HOScene->GetObjectByID("statueHandOpen");

	for (int i = 0; i < eHOItem_MAX; i++)
	{
		SCN_ItemsOnHand[i] = HOScene->GetObjectByID(std::string("OnHandItem") + MKSTR(i));
		SCN_ItemsOnHand[i]->SetZ(eZOrder_INVBOXITEMS);
		SCN_ItemsOnHand[i]->SetScale(0.65f);
		SCN_ItemsOnHand[i]->SetEnable(false);
	}

	DestRect_HOItemArray[0] = new CRectangle(647.0f,192.0f,298.0f,283.0f);	//eHOItem1_NeckLocket
	DestRect_HOItemArray[1] = new CRectangle(536.0f,263.0f,248.0f,209.0f);	//eHOItem2_BookSymbolPartHorn
	DestRect_HOItemArray[2] = new CRectangle(579.0f,149.0f,323.0f,290.0f);	//eHOItem3_EagleEye
	DestRect_HOItemArray[3] = new CRectangle(478.0f,125.0f,449.0f,385.0f);	//eHOItem4_Clockneedle
	DestRect_HOItemArray[4] = new CRectangle(496.0f,247.0f,284.0f,229.0f);	//eHOItem5_DrawerHandle
	DestRect_HOItemArray[5] = new CRectangle(530.0f,305.0f,208.0f,166.0f);	//eHOItem6_KeyRingBox
	DestRect_HOItemArray[6] = new CRectangle(536.0f,263.0f,248.0f,209.0f);	//eHOItem7_BookSymbolPartHead
	DestRect_HOItemArray[7] = new CRectangle(537.0f,230.0f,249.0f,201.0f);	//eHOItem8_TokenBoxPuzzle
	DestRect_HOItemArray[8] = new CRectangle(1082.0f,66.0f,135.0f,189.0f);	//eHOItem9_Pestle

	ItemRect_HOItemArray[0] = new CRectangle(676.0f,328.0f,110.0f,92.0f);	//eHOItem1_NeckLocket
	ItemRect_HOItemArray[1] = new CRectangle(721.0f,564.0f,99.0f,59.0f);	//eHOItem2_BookSymbolPartHorn
	ItemRect_HOItemArray[2] = new CRectangle(469.0f,307.0f,132.0f,101.0f);	//eHOItem3_EagleEye
	ItemRect_HOItemArray[3] = new CRectangle(457.0f,330.0f,270.0f,128.0f);	//eHOItem4_Clockneedle
	ItemRect_HOItemArray[4] = new CRectangle(482.0f,197.0f,274.0f,336.0f);	//eHOItem5_DrawerHandle
	ItemRect_HOItemArray[5] = new CRectangle(503.0f,310.0f,112.0f,131.0f);	//eHOItem6_KeyRingBox
	ItemRect_HOItemArray[6] = new CRectangle(680.0f,324.0f,127.0f,93.0f);	//eHOItem7_BookSymbolPartHead
	ItemRect_HOItemArray[7] = new CRectangle(580.0f,341.0f,145.0f,94.0f);	//eHOItem8_TokenBoxPuzzle
	ItemRect_HOItemArray[8] = new CRectangle(566.0f,219.0f,171.0f,184.0f);	//eHOItem9_Pestle

	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);	
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

	vidTimer = 0;

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

	objectInHandId = -1;
	isObjectInHand = false;

	IsMortarPestleCollectWaiting = false;
	mortarCollectTimer = 0;

	InitHOItemsAndHUD();

	SceneObjectsUpdate();

	InitPopups();

	vfx_goldGlit = new SuperFX("PRT\\L1\\s5hogoldglit.INI", PointSystem::CreateCPoint(714,227,0), 100); 
	vfx_goldGlit->Start();
	vfx_goldGlit->AdvanceByTime(2.0f);
	vfx_flies = new SuperFX("PRT\\L1\\s5hoflies.INI", PointSystem::CreateCPoint(528,253,0), 100); 
	vfx_flies->Start();
	vfx_flies->AdvanceByTime(2.0f);
	vfx_glares = new SuperFX("PRT\\L1\\s5hoglares.INI", PointSystem::CreateCPoint(384,150,0), 100);
	vfx_glares->Start();
	vfx_glares->AdvanceByTime(2.0f);

	vfx_collectFX = new SuperFX("PRT\\L1\\s5hocollect.INI", PointSystem::CreateCPoint(384,150,0), 100);

	//Match 3
	Treasurematch3  = NULL;
	isMatch3Playing = false;
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
	BtnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 683, 30, 35);
	BtnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(BtnMatch3);

	BtnSwitchToHO = new CObject();
	BtnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 683, 30, 35);
	BtnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(BtnSwitchToHO);


	BtnMatch3->SetZ(100);
	BtnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);

	BtnSwitchToHO->Show(false);


	//if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
	{
		Hud->InvBoxCtrl->SetLockBase(HO_USEOBJECTS_FRAME);
		Hud->IsTakeAndUseHOScene = true;
	}

	IsVidSkippable = false;
	IsSkipMouseOver = false;
	IsSkipMouseClicked = false;
	SkipShowTimer = 0;

	skipObj = NULL;

	skipObj = new CObject();
	skipObj->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\SKIPBTN.SPR", (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y, eZOrder_CutsceneFull+10);
	skipObj->Show(false);

	//Video Init : Add Visible = true; to see anything on screen !!
	cv_LasyaAttack = NULL;
	isLasyaAttackVidPlaying = false;

	time = 0;
	subIndex = 0;
	subMax = 2;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("fireattack1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("fireattack2"));
	
	CRectangle RectFullVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	//CRectangle RectFullVid(171, 96, 1024, 576);
	if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttackVideoPlayed)
	{
		cv_LasyaAttack = new CVideo("OGV\\L1S5\\FIREATTACK.OGV", RectFullVid, false, eZOrder_CutsceneFull);
		cv_LasyaAttack->SetScale(GFApp->video_aspectRatio);
		//cv_LasyaAttack->SetScale(1.334f);
	}

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if( !Control::Audio->IsPlaying(aTrackL1Trapped) )
		Control::Audio->PlaySample(aTrackL1Trapped,true);
	//trapped if playing 
	Control::Audio->LoadSample(aAmbS4PuzShadowhand,AudioAmb);
	Control::Audio->PlaySample(aAmbS4PuzShadowhand,true);
	Control::Audio->LoadSample(aTrackCutscnS5MayaAttack,AudioTrack);
	
	//MUSIC_SFX
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S5_HOTreasureRoom]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S5_HOTreasureRoom])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S5_HOTreasureRoom] = true;

	

	
	

	Visible = true;	//To render anything with renderlistentity
}

void L1_S5_HOTreasureRoom::InitHOItemsAndHUD()
{
	//initialize HO Objects and states for the first time
	if( ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] == HOITEM_LOCKED )
	{
		ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] = HOITEM_UNLOCKED;
	}

	//HO Inv Box for UI Items
	const CRectangle  hoInventoryBoxRect(375.0f,645.0f,620.0f,139.0f);

	int numHOItemsInHUD = 5;

	HOInvBox = new HOInventoryBox(hoInventoryBoxRect, numHOItemsInHUD);
	HOInvBox->SetPosition(CPoint(HOInvBox->GetPosition().x, HOInvBox->GetPosition().y, eZOrder_HUD));
	for (int i = 0; i < eHOItem_MAX; i++)
	{
		std::string str;

		CObject * objItem = new CObject();
		str = std::string("LV1\\S5\\HO\\INV\\hoitem") + MKSTR(i) + std::string(".SPR");
		objItem->LoadSprite(str);
		objItem->SetZ(100);

		CObject * objItemShadow = new CObject();
		str = std::string("LV1\\S5\\HO\\INV\\hoitemshadow") + MKSTR(i) + std::string(".SPR");
		objItemShadow->LoadSprite(str);
		objItemShadow->SetZ(100);

		std::vector<int> collectSFXIds;
		collectSFXIds.push_back(aSFXInvCollectv1);
		collectSFXIds.push_back(aSFXInvCollectv2);
		collectSFXIds.push_back(aSFXInvCollectv3);
		InventoryItem * invItem = new InventoryItem(objItemShadow, objItem, i, CPoint(100,100, 100), this, "PRT\\L1\\HOTRAILFX.INI", collectSFXIds, aSFXInvWrongDrop, aSFXInvItemPick);
		HOInvBox->AddInventoryItem(invItem);
		//invItem->SetPosition(CPoint(100,100,100));
	}

	if (ProfileSystem::Instance->ActiveProfile.L1S5_HOHUDItemStates[0] == kInventoryItemState_NotLoaded)//checking first object is loaded
	{
		HOInvBox->LoadInventoryItemsToVisibleArea(0, numHOItemsInHUD);
	}
	else
	{
		HOInvBox->LoadInventoryItemStates(ProfileSystem::Instance->ActiveProfile.L1S5_HOHUDItemStates);
	}
}

void L1_S5_HOTreasureRoom::InitPopups()
{
	ActivePopupID = eHOPop_Inactive;

	Pop_CloseButton = NULL;

	IsPopupTransitionActive = false;

	for (int i = 0; i < eHOPop_Max; i++)
	{
		Pop_SCNArray[i] = NULL;
		Pop_ScnPolyArray[i] = NULL;
		Pop_States[i] = HOPOP_CLOSED;
	}

	//Check conditions here
	//To Disable popup use Pop_States[eHOPop1_BookLocketPop] = HOPOP_INACTIVE;
	isP1bookLockPartSelected = false;

	//eHOPop1_BookLocketPop
	p1IsKeyTurnAnimPlaying = false;
	p1KeyTurnTimer = 0;
	p1KeyTurnAnim = NULL;

	p1Vfx_BookOpen = NULL;
	p1IsWaitingForBookOpen = false;
	p1BookOpenWaitTimer = 0;

	PopupClosePostUpdate(eHOPop1_BookLocketPop);
	if(Pop_States[eHOPop1_BookLocketPop] != HOPOP_INACTIVE)
	{
		CHOScene *Pop1Scn = new CHOScene();
		Pop1Scn->Load("LV1\\S5\\HO\\POP1\\P1.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Pop1Scn->PushObject(popFrame); //in Normal Cases

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Pop1Scn->PushObject(popCloseBtn);

		Pop1Scn->Show(false);
		Pop1Scn->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop1Scn->SetZOff(eZOrder_Popup);
		
		
		P1SelectedLockPart = -1;


		P1bookLockPart1 = Pop1Scn->GetObjectByID("P1bookLockPart1");
		P1bookLockPart2 = Pop1Scn->GetObjectByID("P1bookLockPart2");
		

		P1bookLockPartFixed1 = Pop1Scn->GetObjectByID("bookLockPartFixed1");
		P1bookLockPartFixed2 = Pop1Scn->GetObjectByID("bookLockPartFixed2");
		P1bookLockPartFixed3 = Pop1Scn->GetObjectByID("bookLockPartFixed3");
		P1bookLockPartFixed4 = Pop1Scn->GetObjectByID("bookLockPartFixed4");

		P1bookLockPart1->InitPos = P1bookLockPart1->Pos;
		P1bookLockPart2->InitPos = P1bookLockPart2->Pos;

		Pop_SCNArray[eHOPop1_BookLocketPop] = Pop1Scn;
		int ArrayPts[] = {342,428,457,376,582,382,664,440,660,462,542,545,381,534};
		Pop_ScnPolyArray[eHOPop1_BookLocketPop] =  new CPolygon(7);
		Pop_ScnPolyArray[eHOPop1_BookLocketPop]->Init(ArrayPts);

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] >= HOITEM_USED)
		{
		
		}
		else
		{
			P1bookLockPartFixed4->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] >= HOITEM_USED)
		{
		
		}
		else
		{
			P1bookLockPartFixed3->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart2Fixed)
			P1bookLockPart1->SetEnable(false);
		else
			P1bookLockPartFixed1 ->SetEnable(false);

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart3Fixed)
			P1bookLockPart2 ->SetEnable(false);
		else
			P1bookLockPartFixed2 ->SetEnable(false);

		if( ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened )
		{
			P1bookLockPartFixed1->SetEnable(false);
			P1bookLockPartFixed2->SetEnable(false);
			P1bookLockPartFixed3->SetEnable(false);
			P1bookLockPartFixed4->SetEnable(false);
		}

		p1KeyTurnAnim = Pop1Scn->GetObjectByID("P1keyAnim");

		if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened)
		{
			Pop1Scn->GetObjectByID("P1bookOpen")->SetEnable(false);
			Pop1Scn->GetObjectByID("P1ringBox")->SetEnable(false);
			Pop1Scn->GetObjectByID("P1ringBoxOpen")->SetEnable(false);
			Pop1Scn->GetObjectByID("P1coin")->SetEnable(false);
			p1KeyTurnAnim->SetEnable(false);
		}
		else
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] >= HOITEM_USED)
			{
				p1KeyTurnAnim->SetEnable(false);
			}
			else
			{
				Pop1Scn->GetObjectByID("P1coin")->SetEnable(false);
				Pop1Scn->GetObjectByID("P1ringBoxOpen")->SetEnable(false);
				p1KeyTurnAnim->SetEnable(false);
			}	
		}

		p1Vfx_BookOpen = new SuperFX("PRT\\L1\\s5hop1bookopenfx.INI", PointSystem::CreateCPoint(650,386,0), eZOrder_Popup+10); 
	}

	//eHOPop2_ColorSlidePuzzleBox
	p2PuzzleEndTimer = 0;
	p2IsPuzzleEndPlaying = false;
	p2Vfx_PuzEndFX = NULL;

	p2HelpFrameBG = NULL;
	p2Helptext = NULL;

	PopupClosePostUpdate(eHOPop2_ColorSlidePuzzleBox);
	if(Pop_States[eHOPop2_ColorSlidePuzzleBox] != HOPOP_INACTIVE)
	{
		CHOScene *Pop2Scn = new CHOScene();
		Pop2Scn->Load("LV1\\S5\\HO\\POP2\\P2.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, 16);
		popFrame->SetUID("popFrame");
		Pop2Scn->InsertObject(popFrame, 16); //in Normal Cases

		CObject *_skipText = new CObject();
		_skipText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\SKIP.SPR", -20, 250, 50);
		_skipText->SetUID("btnskiptext");
		Pop2Scn->PushObject(_skipText);

		Pop2Scn->GetObjectByID("btnskiptext")->SetScale(1.5);

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Pop2Scn->PushObject(popCloseBtn);

		Pop2Scn->Show(false);
		Pop2Scn->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop2Scn->SetZOff(eZOrder_Popup);

		Pop_SCNArray[eHOPop2_ColorSlidePuzzleBox] = Pop2Scn;

		int ArrayPts[] = {1085,402,944,472,933,540,1039,646,1195,575,1207,502};
		Pop_ScnPolyArray[eHOPop2_ColorSlidePuzzleBox] =  new CPolygon(6);
		Pop_ScnPolyArray[eHOPop2_ColorSlidePuzzleBox]->Init(ArrayPts);

		//help text Frame
		p2HelpFrameBG = new CObject();
		p2HelpFrameBG->LoadInit("LV1\\S5\\HO\\POP2\\helpBG.SPR", CenterPoint.x, CenterPoint.y, eZOrder_Popup+100);
		p2HelpFrameBG->SetEnable(false);

		std::string p2PuzzleHelpText = "L1glyph";

		ColorRGB white = {0.90f, 0.90f, 0.80f};
		float _lineWidth = 600;
		int _nLines = 5;
		EFont _font = eFont_26;
		eTextAlignment _align = Align_Center;
		if(StringTable::IsKeyFound(p2PuzzleHelpText))
		{
			_lineWidth = StringTable::GetLineWidth(p2PuzzleHelpText);
			_nLines = StringTable::GetNoOfLines(p2PuzzleHelpText);
			_font = (EFont)StringTable::GetFont(p2PuzzleHelpText);
		}
		p2Helptext = new MultilineText(_nLines, PointSystem::CreateCPoint(CenterPoint.x, CenterPoint.y-50.0f, 0), eZOrder_Popup+102, white, 30, _lineWidth, _align, _font);
		p2Helptext->SetTextMultiline(StringTable::getStr(p2PuzzleHelpText));
		p2Helptext->Show(false);

		//Puzzle Vars
		for (int i = 0; i < 5; i++)
		{
			//Glow Objects
			p2BtnLocketArray[i] = Pop2Scn->GetObjectByID(std::string("P2TokenGlow")+MKSTR(i));
			p2BtnLocketArray[i]->SetEnable(false);
		}

		for (int i = 0; i < MAX_L1S5HO_P2COLORTOKENS; i++)
		{
			p2ColorTokenArray[i] = Pop2Scn->GetObjectByID(std::string("P2stone")+MKSTR((i+1)));
			p2PosContains[i] = i;
			p2TokenPosArray[i] = p2ColorTokenArray[i]->Pos;
		}

		

		p2SkipBtn = Pop2Scn->GetObjectByID("P2BtnSkip");
		/*
		SceneData skipTxt;
		skipTxt.objType = "Object";
		skipTxt.uid = "btnskiptext";
		skipTxt.spriteString = "LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\SKIP.SPR";
		skipTxt.spriteAnim = 0;
		skipTxt.x = p2SkipBtn->Pos.x;
		skipTxt.y = p2SkipBtn->Pos.y;
		skipTxt.ZOrder = p2SkipBtn->ZOrder + 1;
		

		skipText = Pop2Scn->CreateObject(skipTxt);
		*/

		

		p2HelpBtn = Pop2Scn->GetObjectByID("P2BtnHelp");
		skipText = Pop2Scn->GetObjectByID("btnskiptext");
		p2SkipBtnGlow = Pop2Scn->GetObjectByID("P2BtnSkipGlow");
		p2SkipBtnActive = Pop2Scn->GetObjectByID("P2BtnSkipActive");
		p2SkipBtnActive->Paused = true;
		p2SkipBtnGlow->SetEnable(false);

		p2SkipPuzzleTimer =0;
		p2SkipPuzzleThreshold = 30;//30 seconds default

		//Shuffle it now using predefined vars or saved vars in profile
		if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleShuffled)
		{
			ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleShuffled = true;
			for (int i = 0; i < MAX_L1S5HO_P2COLORTOKENS; i++)
			{
				p2PosContains[i] = P2ShuffleInitPosContains[i];
				int currentItemInI = p2PosContains[i];
				p2ColorTokenArray[currentItemInI]->Pos = p2TokenPosArray[i];

				ProfileSystem::Instance->ActiveProfile.L1S5_HOP2TokenPosContains[i] = p2PosContains[i];
			}
		}
		else
		{
			//read from profile
			for (int i = 0; i < MAX_L1S5HO_P2COLORTOKENS; i++)
			{
				p2PosContains[i] = ProfileSystem::Instance->ActiveProfile.L1S5_HOP2TokenPosContains[i];
				int currentItemInI = p2PosContains[i];
				p2ColorTokenArray[currentItemInI]->Pos = p2TokenPosArray[i];
			}
		}

		for (int j = 0; j < 4; j++)
		{
			p2SwappingTokenIds[j] = 0;
			p2SwapPosIds[j] = 0;
		}

		p2TotalSwapTime = 1.0f;
		p2TokenSwapTimer = 0;
		p2IsTokenSwapping = false;
		p2IsFirstSwapObjClicked = false;
		p2IsTokenRotating = false;
		p2SwapId1 = p2SwapId2 = 0;

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleSolved)
		{
			p2HelpBtn->SetEnable(false);
			p2SkipBtn->SetEnable(false);
			skipText->SetEnable(false);
			p2SkipBtnGlow->SetEnable(false);
			p2SkipBtnActive->SetEnable(false);

			Pop_SCNArray[eHOPop2_ColorSlidePuzzleBox]->GetObjectByID("P2coin")->SetEnable(false);
			Pop_SCNArray[eHOPop2_ColorSlidePuzzleBox]->GetObjectByID("P2PuzzleBg")->SetEnable(false);
			for (int i = 0; i < MAX_L1S5HO_P2COLORTOKENS; i++)
			{
				p2ColorTokenArray[i]->SetEnable(false);
			}
			for (int i = 0; i < 5; i++)
			{
				p2BtnLocketArray[i]->SetEnable(false);
			}
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] != HOITEM_USED)
		{
			Pop_SCNArray[eHOPop2_ColorSlidePuzzleBox]->GetObjectByID("P2coin")->SetEnable(false);

			p2HelpBtn->SetEnable(false);
			p2SkipBtn->SetEnable(false);
			skipText->SetEnable(false);
			p2SkipBtnGlow->SetEnable(false);
			p2SkipBtnActive->SetEnable(false);
		}

		p2Vfx_PuzEndFX = new SuperFX("PRT\\L1\\s5hop2puzendfx.INI", PointSystem::CreateCPoint(666,328,0), eZOrder_Popup+10);
	}

	//eHOPop3_Clock
	CAnim_ClockNeedleFix = NULL;
	p3IsClockNeedleFixPlaying = false;
	p3IsClockNeedlesRotating = false;
	p3NeedleRotateTimer = 0;

	PopupClosePostUpdate(eHOPop3_Clock);
	if(Pop_States[eHOPop3_Clock] != HOPOP_INACTIVE)
	{
		CHOScene *Pop3Scn = new CHOScene();
		Pop3Scn->Load("LV1\\S5\\HO\\POP3\\P3.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Pop3Scn->PushObject(popFrame); //in Normal Cases

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Pop3Scn->PushObject(popCloseBtn);

		Pop3Scn->Show(false);
		Pop3Scn->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop3Scn->SetZOff(eZOrder_Popup);

		Pop_SCNArray[eHOPop3_Clock] = Pop3Scn;

		int ArrayPts[] = {849,220,822,277,823,390,887,433,1007,377,995,251,914,211};
		Pop_ScnPolyArray[eHOPop3_Clock]=  new CPolygon(7);
		Pop_ScnPolyArray[eHOPop3_Clock]->Init(ArrayPts);

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] >= HOITEM_USED)
		{
			Pop3Scn->GetObjectByID("P3clockClosed")->SetEnable(false);
			Pop3Scn->GetObjectByID("P3handle")->SetEnable(false);
			Pop3Scn->GetObjectByID("P3needle")->SetEnable(false);
			Pop3Scn->GetObjectByID("P3HourNeedle")->SetEnable(false);
		}
		else
		{
			Pop3Scn->GetObjectByID("P3clockOpen")->SetEnable(false);
			Pop3Scn->GetObjectByID("P3handle")->SetEnable(false);
			Pop3Scn->GetObjectByID("P3needle")->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] >= HOITEM_TAKEN)
			Pop3Scn->GetObjectByID("P3needle")->SetEnable(false);
		
		//Pop3Scn->GetObjectByID("P3needle")->SetPivot(-102,-33); //only after animation
		Pop3Scn->GetObjectByID("P3HourNeedle")->SetPivot(-4,87);

		CAnim_ClockNeedleFix = new CustomAnimator("ANIM\\L1\\s5hop3needlefix.anim",Pop3Scn->GetObjectByID("P3needle"));
	}

	//eHOPop4_StatueHanuman
	p4Vfx_locketfix1 = NULL;
	p4Vfx_locketfix2 = NULL;
	CAnim_LocketFix = NULL;
	p4IsLocketFixAnimPlaying = false;
	p4IsHandOpenFadingIn = false;
	p4LocketFixFadeInTimer = 0;

	PopupClosePostUpdate(eHOPop4_StatueHanuman);
	if(Pop_States[eHOPop4_StatueHanuman] != HOPOP_INACTIVE)
	{
		CHOScene *Pop4Scn = new CHOScene();
		Pop4Scn->Load("LV1\\S5\\HO\\POP4\\P4.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Pop4Scn->PushObject(popFrame); //in Normal Cases

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Pop4Scn->PushObject(popCloseBtn);

		Pop4Scn->Show(false);
		Pop4Scn->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop4Scn->SetZOff(eZOrder_Popup);

		Pop_SCNArray[eHOPop4_StatueHanuman] = Pop4Scn;

		int ArrayPts[] = {1047,2,1069,72,1051,182,1009,209,913,165,880,63,928,7};
		Pop_ScnPolyArray[eHOPop4_StatueHanuman] =  new CPolygon(7);
		Pop_ScnPolyArray[eHOPop4_StatueHanuman]->Init(ArrayPts);

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] >= HOITEM_USED)
		{
			Pop4Scn->GetObjectByID("P4handClosed")->SetEnable(false);
		}
		else
		{
			Pop4Scn->GetObjectByID("P4handOpen")->SetEnable(false);
			Pop4Scn->GetObjectByID("P4neckLocket")->SetEnable(false);
			Pop4Scn->GetObjectByID("P4eagleEye")->SetEnable(false);
		}
		
		p4Vfx_locketfix1 = new SuperFX("PRT\\L1\\s5holockethandfx1.INI", PointSystem::CreateCPoint(571,410,0), eZOrder_Popup+10);
		p4Vfx_locketfix2 = new SuperFX("PRT\\L1\\s5holockethandfx2.INI", PointSystem::CreateCPoint(585,387,0), eZOrder_Popup+10);

		CAnim_LocketFix = new CustomAnimator("ANIM\\L1\\s5hoitem1necklocketfix.anim",Pop_SCNArray[eHOPop4_StatueHanuman]->GetObjectByID("P4neckLocket"));
	}

	//eHOPop5_Eagle
	PopupClosePostUpdate(eHOPop5_Eagle);
	if(Pop_States[eHOPop5_Eagle] != HOPOP_INACTIVE)
	{
		CHOScene *Pop5Scn = new CHOScene();
		Pop5Scn->Load("LV1\\S5\\HO\\POP5\\P5.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Pop5Scn->PushObject(popFrame); //in Normal Cases

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Pop5Scn->PushObject(popCloseBtn);

		Pop5Scn->Show(false);
		Pop5Scn->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop5Scn->SetZOff(eZOrder_Popup);

		Pop_SCNArray[eHOPop5_Eagle] = Pop5Scn;

		int ArrayPts[] = {700,99,637,116,593,157,609,263,687,296,751,287,775,260,808,127,747,88};
		Pop_ScnPolyArray[eHOPop5_Eagle] =  new CPolygon(9);
		Pop_ScnPolyArray[eHOPop5_Eagle]->Init(ArrayPts);

		Pop5Scn->GetObjectByID("P5eagleEyeToFix")->SetEnable(false);
		Pop5Scn->GetObjectByID("P5EyeGlow")->SetEnable(false);

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] == HOITEM_USED)
		{
			Pop5Scn->GetObjectByID("P5NeedleLocked")->SetEnable(false);
		}
		else
		{
			Pop5Scn->GetObjectByID("P5MouthOpen")->SetEnable(false);
			Pop5Scn->GetObjectByID("P5FixedEye")->SetEnable(false);
		}
	}

	//eHOPop6_ShelfSwapPuzzle
	CAnim_P6ShelfOpen = NULL;
	PopupClosePostUpdate(eHOPop6_ShelfSwapPuzzle);
	if(Pop_States[eHOPop6_ShelfSwapPuzzle] != HOPOP_INACTIVE)
	{
		CHOScene *Pop6Scn = new CHOScene();
		Pop6Scn->Load("LV1\\S5\\HO\\POP6\\P6.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Pop6Scn->PushObject(popFrame); //in Normal Cases

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Pop6Scn->PushObject(popCloseBtn);

		Pop6Scn->Show(false);
		Pop6Scn->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop6Scn->SetZOff(eZOrder_Popup);

		
		//ShelfPieceSwap HO
		const CRectangle  P6ShelfRectMask(359.0f,90.0f,649.0f,486.0f);

		isP6ShelfOpenAnimPlaying = false;
		P6_shelfOpenAnim = Pop6Scn->GetObjectByID("P6ShelfOpen");
		P6_ShelfClosed = Pop6Scn->GetObjectByID("P6ShelfClosed");
		P6_ShelfBorder = Pop6Scn->GetObjectByID("P6ShelfBorder");
		P6_shelfOpen  = Pop6Scn->GetObjectByID("P6boxOpened");
		P6_Key  = Pop6Scn->GetObjectByID("P6key");
		P6_handleFixed  = Pop6Scn->GetObjectByID("P6handleFixed");
		P6_bookLockPart = Pop6Scn->GetObjectByID("P6bookLockPart");
		for( int i = 0; i < 7; i++)
		{
			P6_SwapPiece[i] = Pop6Scn->GetObjectByID("P6swapPiece" + MKSTR(i+1));
			p6_positionArray[i] = CPoint(P6_SwapPiece[i]->Pos);
		}
		isP6SwapPieceSelected = false;
		isP6SwapHOPuzzleSolved =false;
		isP6SwappingInProgress = false;

		P6_SelectedPieceIndex = -1;

		P6_shelfOpenAnim->SetMaskRect( P6ShelfRectMask );
		for( int i = 0; i < 7; i++)
		{
			p6_randomIntegerArray[i] = i;
		}

		//shuffle shelf pieces
		for( int i = 0; i < 7; i++)
		{
			int randomVal = rand() % 7;
			int temp = p6_randomIntegerArray[i];
			p6_randomIntegerArray[i] = p6_randomIntegerArray[randomVal];
			p6_randomIntegerArray[randomVal] = temp;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved)
		{
			for( int i = 0; i < 7; i++)
			{
				P6_SwapPiece[i]->Pos = p6_positionArray[p6_randomIntegerArray[i]];
				p6_IndexArray[i] = p6_randomIntegerArray[i];
			} 
		}
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] == HOITEM_USED )
		{
			for( int i = 0; i < 7; i++)
			{
				P6_SwapPiece[i]->SetEnable(false);
			} 
			P6_shelfOpen->SetEnable(true);
			P6_ShelfClosed->SetEnable(false);
			P6_ShelfBorder->SetEnable(false);
			P6_shelfOpenAnim->SetEnable(false);

			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] == HOITEM_TAKEN )
			{
				P6_Key->SetEnable(false);
			}
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] == HOITEM_TAKEN)
			{
				P6_bookLockPart->SetEnable(false);
			}
		}
		else
		{
			P6_shelfOpen->SetEnable(false);
			P6_Key->SetEnable(false);
			P6_handleFixed->SetEnable(false);
			P6_bookLockPart->SetEnable(false);
			P6_ShelfClosed->SetEnable(false);
			P6_ShelfBorder->SetEnable(false);
			P6_shelfOpenAnim->SetEnable(false);

			CObject* objArr[] = {P6_shelfOpenAnim, P6_Key, P6_bookLockPart, P6_ShelfBorder};
			std::string animArr[] = {"ANIM\\L1\\S5ShelfOpen.anim", "ANIM\\L1\\S5Key.anim", "ANIM\\L1\\S5LockPart.anim",  "ANIM\\L1\\S5ShelfBorder.anim"};
			CAnim_P6ShelfOpen = new CustomAnimGroup(4, animArr, objArr);
		}

		

		Pop_SCNArray[eHOPop6_ShelfSwapPuzzle] = Pop6Scn;

		int ArrayPts[] = {196,66,408,23,473,44,484,224,280,289,203,217};
		Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle] =  new CPolygon(6);
		Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle]->Init(ArrayPts);
	}

	PopupAlpha = 0.0f;
	PopVal = 0.0f;

	popupCloseDelayTimer = 0;
}

void L1_S5_HOTreasureRoom::OnInventoryItemClick(int invItemType)
{
//	K_LOG("Item Clicked on %d", invItemType);
	if(isObjectInHand)
	{
		SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
		isObjectInHand = false;
		HOInvBox->ReturnItem(objectInHandId, Control::Input->Mpos());
	}
	else
	{
		Cursor::Show(false);
		objectInHandId = invItemType;
		isObjectInHand = true;
		//SCN_ItemsOnHand[objectInHandId]->SetPos(&Control::Input->Mpos());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CPoint mousePos = Control::Input->Mpos();
		SCN_ItemsOnHand[objectInHandId]->SetPos(&mousePos);
		SCN_ItemsOnHand[objectInHandId]->SetEnable(true);
		Control::Input->ClearMouse();
	}
}

void L1_S5_HOTreasureRoom::UpdateInventoryChangesToProfile(int itemId)
{
	HOInvBox->UpdateInventoryChangesToProfile(ProfileSystem::Instance->ActiveProfile.L1S5_HOHUDItemStates, itemId);
}

void L1_S5_HOTreasureRoom::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( IsPopupTransitionActive )
		UpdatePopupTransition();
}

bool L1_S5_HOTreasureRoom::checkForDestAreaClicks(CPoint pos)
{
	bool isDestAreaClicked = false;

	if(ActivePopupID == eHOPop_Inactive && DestRect_HOItemArray[eHOItem9_Pestle]->Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		isDestAreaClicked = true;
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L1pestle");
	}
	else if(ActivePopupID == eHOPop1_BookLocketPop)
	{
		if( (ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] != HOITEM_USED || ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] != HOITEM_USED) && DestRect_HOItemArray[eHOItem2_BookSymbolPartHorn]->Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			isDestAreaClicked = true;
			if( Control::Input->LBclicked() )
				Hud->ShowBannerText("L1slot");
		}
		else if(!p1IsWaitingForBookOpen && ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened && ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] != HOITEM_USED && DestRect_HOItemArray[eHOItem6_KeyRingBox]->Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			isDestAreaClicked = true;
			if( Control::Input->LBclicked() )
				Hud->ShowBannerText("L1tiny");
		}
	}
	else if(ActivePopupID == eHOPop2_ColorSlidePuzzleBox)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] != HOITEM_USED && DestRect_HOItemArray[eHOItem8_TokenBoxPuzzle]->Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			isDestAreaClicked = true;
			if( Control::Input->LBclicked() )
				Hud->ShowBannerText("L1diamond");
		}
	}
	else if(ActivePopupID == eHOPop3_Clock)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] != HOITEM_USED && DestRect_HOItemArray[eHOItem4_Clockneedle]->Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			isDestAreaClicked = true;
			if( Control::Input->LBclicked() )
				Hud->ShowBannerText("L1clock");
		}
	}
	else if(ActivePopupID == eHOPop4_StatueHanuman)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] != HOITEM_USED && DestRect_HOItemArray[eHOItem1_NeckLocket]->Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			isDestAreaClicked = true;
			if( Control::Input->LBclicked() )
				Hud->ShowBannerText("L1chain");
		}
	}
	else if(ActivePopupID == eHOPop5_Eagle)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] != HOITEM_USED && DestRect_HOItemArray[eHOItem3_EagleEye]->Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			isDestAreaClicked = true;
			if( Control::Input->LBclicked() )
				Hud->ShowBannerText("L1eye");
		}
	}
	else if(ActivePopupID == eHOPop6_ShelfSwapPuzzle)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] != HOITEM_USED && DestRect_HOItemArray[eHOItem5_DrawerHandle]->Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			isDestAreaClicked = true;
			if( Control::Input->LBclicked() )
				Hud->ShowBannerText("L1drawer");
		}
	}

	return isDestAreaClicked;
}

void L1_S5_HOTreasureRoom::OnItemOnHandClickHandler(CPoint pos)
{
	if(ActivePopupID == eHOPop_Inactive)
	{
		//check for scene clicks
		if( objectInHandId == eHOItem9_Pestle && DestRect_HOItemArray[eHOItem9_Pestle]->Intersection(&pos))
		{
			Cursor::Show(true);
			isObjectInHand = false;
			SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] = HOITEM_USED;

			HOInvBox->UnloadInventoryItemFromVisibleArea(eHOItem9_Pestle);

			//HO Completed
			ProfileSystem::Instance->ActiveProfile.L1S5_HOSolved = true;

			//award pestle here itself to avoid getting missed - if user closed inbetween video!!
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_MortarPestle] = true;
			ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken = true;

			Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5PestleFixinMortar);
			if(_handleExtrasGameComplete(false))
				return;

			IsMortarPestleCollectWaiting = true;

			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsTakeAndUseHOScene = false;

			SCN_grinderOnly->FadeOut(1.25f);
			SCN_grinderWithPestile->SetEnable(true);
			SCN_grinderWithPestile->FadeIn(1.45f);
			return;
		}

		//else check for active popup clicks
		bool isAnyPopupClicked = false;
		for (int i = 0; i < eHOPop_Max; i++)
		{
			if(Pop_States[i] != HOPOP_INACTIVE) //if popup is active
			{
				if(Pop_ScnPolyArray[i]!=NULL && Pop_ScnPolyArray[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_Look);
					if( Control::Input->LBclicked() )
					{
						isAnyPopupClicked = true;
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5Popup);
						ActivePopupID = i;
						Pop_States[i] = HOPOP_SCALEUP;

						Pop_CloseButton = Pop_SCNArray[i]->GetObjectByID("popClose");
						Pop_CloseButton->PlayAnimation(0);

						PopVal = 0;

						SetupPopupTransition(Pop_SCNArray[i], Pop_ScnPolyArray[i]->center.x, Pop_ScnPolyArray[i]->center.y);
					}
					break;
				}
			}
		}

		if(!isAnyPopupClicked)
		{
			//Return with error banner
			GFInstance->WrongInvDropMsg(Hud);
			Cursor::Show(true);
			isObjectInHand = false;
			SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
			HOInvBox->ReturnItem(objectInHandId, pos);
		}
	}
	else
	{
		//Popup Click and Item place handler
		switch (ActivePopupID)
		{
		case eHOPop1_BookLocketPop:
			//Click on Dest Rect
			if( objectInHandId == eHOItem7_BookSymbolPartHead && DestRect_HOItemArray[eHOItem7_BookSymbolPartHead]->Intersection(&pos))
			{
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] = HOITEM_USED;
				

				P1bookLockPartFixed3->SetEnable(true);
				P1bookLockPartFixed3->SetAlpha(0.0f);
				P1bookLockPartFixed3->FadeIn(1.5f);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5GarudaEmblemFix);

				SceneObjectsUpdate();
				HOInvBox->UnloadInventoryItemFromVisibleArea(eHOItem7_BookSymbolPartHead);
				HOInvBox->LoadNextInventoryItemToVisibleArea();
			}
			else if( objectInHandId == eHOItem2_BookSymbolPartHorn && DestRect_HOItemArray[eHOItem2_BookSymbolPartHorn]->Intersection(&pos))
			{
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] = HOITEM_USED;

				P1bookLockPartFixed4->SetEnable(true);
				P1bookLockPartFixed4->SetAlpha(0.0f);
				P1bookLockPartFixed4->FadeIn(1.5f);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5GarudaEmblemFix);
				
				SceneObjectsUpdate();

				HOInvBox->UnloadInventoryItemFromVisibleArea(eHOItem2_BookSymbolPartHorn);
				HOInvBox->LoadNextInventoryItemToVisibleArea();
			}
			else if( objectInHandId == eHOItem6_KeyRingBox && ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened && DestRect_HOItemArray[eHOItem6_KeyRingBox]->Intersection(&pos) )
			{
				Cursor::Show(true);
				isObjectInHand = false;
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5RingboxOpen);
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] = HOITEM_USED;
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] = HOITEM_UNLOCKED;

				HOInvBox->UnloadInventoryItemFromVisibleArea(eHOItem6_KeyRingBox);
				HOInvBox->LoadNextInventoryItemToVisibleArea();

				p1IsKeyTurnAnimPlaying = true;
				p1KeyTurnAnim->SetEnable(true);
				p1KeyTurnAnim->FadeIn(1.45f);
				p1KeyTurnAnim->PlayAnimation(1);
				
				SceneObjectsUpdate();
			}
			else
			{
				//Return with error banner
				GFInstance->WrongInvDropMsg(Hud);
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				HOInvBox->ReturnItem(objectInHandId, pos);

			}
			break;
		case eHOPop2_ColorSlidePuzzleBox:
			//Click on Dest Rect
			if( objectInHandId == eHOItem8_TokenBoxPuzzle && DestRect_HOItemArray[eHOItem8_TokenBoxPuzzle]->Intersection(&pos))
			{
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] = HOITEM_USED;

				SceneObjectsUpdate();

				HOInvBox->UnloadInventoryItemFromVisibleArea(eHOItem8_TokenBoxPuzzle);
				HOInvBox->LoadNextInventoryItemToVisibleArea();

				p2HelpBtn->SetEnable(true);
				p2SkipBtn->SetEnable(true);
				skipText->SetEnable(true);
				p2SkipBtnGlow->SetEnable(false);
				p2SkipBtnActive->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5CoinFixInbox);		
				Pop_SCNArray[eHOPop2_ColorSlidePuzzleBox]->GetObjectByID("P2coin")->SetEnable(true);
			}
			else
			{
				//Return with error banner
				GFInstance->WrongInvDropMsg(Hud);
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				HOInvBox->ReturnItem(objectInHandId, pos);
			}
			break;
		case eHOPop3_Clock:
			//Click on Dest Rect
			if( objectInHandId == eHOItem4_Clockneedle && DestRect_HOItemArray[eHOItem4_Clockneedle]->Intersection(&pos))
			{
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] = HOITEM_USED;
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] = HOITEM_UNLOCKED;

				SceneObjectsUpdate();

				HOInvBox->UnloadInventoryItemFromVisibleArea(eHOItem4_Clockneedle);
				HOInvBox->LoadNextInventoryItemToVisibleArea();

				CAnim_ClockNeedleFix->PlayAnim();
				p3IsClockNeedleFixPlaying = true;
			}
			else
			{
				//Return with error banner
				GFInstance->WrongInvDropMsg(Hud);
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				HOInvBox->ReturnItem(objectInHandId, pos);
			}
			break;
		case eHOPop4_StatueHanuman:
			//Click on Dest Rect
			if( objectInHandId == eHOItem1_NeckLocket && DestRect_HOItemArray[eHOItem1_NeckLocket]->Intersection(&pos))
			{
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] = HOITEM_USED;
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] = HOITEM_UNLOCKED;

				SceneObjectsUpdate();

				HOInvBox->UnloadInventoryItemFromVisibleArea(eHOItem1_NeckLocket);
				HOInvBox->LoadNextInventoryItemToVisibleArea();

				CAnim_LocketFix->PlayAnim();
				p4IsLocketFixAnimPlaying = true;
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5MonkeyHandOpen);
			}
			else
			{
				//Return with error banner
				GFInstance->WrongInvDropMsg(Hud);
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				HOInvBox->ReturnItem(objectInHandId, pos);
			}
			break;
		case eHOPop5_Eagle:
			//Click on Dest Rect
			if( objectInHandId == eHOItem3_EagleEye && DestRect_HOItemArray[eHOItem3_EagleEye]->Intersection(&pos))
			{
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] = HOITEM_USED;
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] = HOITEM_UNLOCKED;

				SceneObjectsUpdate();

				HOInvBox->UnloadInventoryItemFromVisibleArea(eHOItem3_EagleEye);
				HOInvBox->LoadNextInventoryItemToVisibleArea();

				Pop_SCNArray[eHOPop5_Eagle]->GetObjectByID("P5NeedleLocked")->FadeOut(1.5f);
				
				Pop_SCNArray[eHOPop5_Eagle]->GetObjectByID("P5MouthOpen")->SetEnable(true);
				Pop_SCNArray[eHOPop5_Eagle]->GetObjectByID("P5MouthOpen")->FadeIn(1.5f);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5EagleMouthOpen);
				Pop_SCNArray[eHOPop5_Eagle]->GetObjectByID("P5FixedEye")->SetEnable(true);
				Pop_SCNArray[eHOPop5_Eagle]->GetObjectByID("P5FixedEye")->FadeIn(1.5f);
			}
			else
			{
				//Return with error banner
				GFInstance->WrongInvDropMsg(Hud);
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				HOInvBox->ReturnItem(objectInHandId, pos);
			}
			break;
		case eHOPop6_ShelfSwapPuzzle:
			//Click on Dest Rect
			if( ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved && objectInHandId == eHOItem5_DrawerHandle && DestRect_HOItemArray[eHOItem5_DrawerHandle]->Intersection(&pos))
			{
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] = HOITEM_USED;
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] = HOITEM_UNLOCKED;
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] = HOITEM_UNLOCKED;
				SceneObjectsUpdate();

				HOInvBox->UnloadInventoryItemFromVisibleArea(eHOItem5_DrawerHandle);
				HOInvBox->LoadNextInventoryItemToVisibleArea();

				for( int i = 0; i < 7; i++)
				{
					P6_SwapPiece[i]->SetEnable(false);
				}
				
				isP6ShelfOpenAnimPlaying = true;
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5DrawerOpen);
				P6_ShelfClosed->SetEnable(false);
				CAnim_P6ShelfOpen->PlayAnim();
				
			}
			else
			{				
				//Return with error banner
				GFInstance->WrongInvDropMsg(Hud);
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				HOInvBox->ReturnItem(objectInHandId, pos);
			}
			break;
		}
	}
}

void L1_S5_HOTreasureRoom::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if(IsResetPop)
	{
		ResetHO();
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	CPoint pos = Control::Input->Mpos();
	if(!BtnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L1S5_HOSolved)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOSolved || IsMortarPestleCollectWaiting)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		BtnMatch3->Show(false);
		BtnSwitchToHO->Show(false);
	}

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOSolved && ProfileSystem::Instance->ActiveProfile.L1S5_HOCompletedWithMatch3)
	{
		//Set all items to end states

		SCN_grinderOnly->SetEnable(false);
		SCN_grinderWithPestile->SetEnable(false);

		//HO Completed
		ProfileSystem::Instance->ActiveProfile.L1S5_HOSolved = true;
		if(_handleExtrasGameComplete(false))
			return;


		//Collect Mortar Pestle
		Hud->InvBoxCtrl->ResetToInvBox();
		Hud->IsTakeAndUseHOScene = false;

		std::vector<int> vecInvIds;
		Hud->InvBoxCtrl->Activate(1);
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_MortarPestle] = true;
		ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken = true;			
		vecInvIds.push_back(eInv_L1S5_MortarPestle);
		Hud->ShowInventory(&vecInvIds);

		return;
	}

	if(IsMortarPestleCollectWaiting)
	{
		mortarCollectTimer += Control::LogicRate;
		if(mortarCollectTimer>2.0f)
		{
			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsTakeAndUseHOScene = false;

			SCN_grinderWithPestile->SetEnable(false);

			std::vector<int> vecInvIds;
			Hud->InvBoxCtrl->Activate(1);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_MortarPestle] = true;
			ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken = true;			
			vecInvIds.push_back(eInv_L1S5_MortarPestle);
			Hud->ShowInventory(&vecInvIds);

			IsMortarPestleCollectWaiting = false;
		}
		return;
	}

	if(isLasyaAttackVidPlaying)
	{
#ifndef FORCE_CLOSE_VID_END
		if( IsVidSkippable )
		{
			if(SkipShowTimer>2.0f)
			{
				if(!skipObj->Visible)
					skipObj->Show(true);

				CPoint mousePos = Control::Input->Mpos();
				IsSkipMouseOver = ( mousePos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) && 
					mousePos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
					mousePos.y >= SKIPIMAGE_Y &&
					mousePos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) + SKIPIMAGE_WIDTH) );

				if( IsSkipMouseOver && !IsSkipMouseClicked)  
				{
					Cursor::SetMode(CA_HandPointy);
					skipObj->PlayAnimation(1);
					IsSkipMouseClicked = Control::Input->LBclicked();					
					if( IsSkipMouseClicked )
					{
						skipObj->Show(false);
						Control::Audio->PlaySample(aSFXClick);
						Control::Input->ClearMouse();
						AwardManager::GetInstance()->SkipDialogueOrCutScene();
					}
				}
				else //Cursor not over skip region
				{
					skipObj->PlayAnimation(0);
				}
					
				if(cv_LasyaAttack->isEndReached() || IsSkipMouseClicked)
				{
					cv_LasyaAttack->StopAndFadeOutVideo(2.0f);
					isLasyaAttackVidPlaying = false;
					subtitleText->SetText("");
					Hud->AllowInput = true;
					Hud->ShowHud(false);

					if( !Control::Audio->IsPlaying(aTrackL1Trapped) )
						Control::Audio->PlaySample(aTrackL1Trapped, true);
					if( !Control::Audio->IsPlaying(aAmbS5TreasureRoom) )
						Control::Audio->PlaySample(aAmbS5TreasureRoom, true);
							
					//Control::Audio->StopSample(aTrackCutscnS5MayaAttack);

					AwardManager::GetInstance()->CompleteHO(eArea_L1_S5_HOTreasureRoom);					
					
					SendMessage(ControlExit, ID, HOSM_NewArea);
					return;
				}
			}
			else
				SkipShowTimer += 10*Control::LogicRate;
		}
#endif
		//Subtitles Code
		time = cv_LasyaAttack->GetTime();

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");

		if (time > subtitles[subIndex].end)
		{
			if (subMax - 1 > subIndex)
				subIndex++;
		}

		vidTimer += Control::LogicRate;
		if (vidTimer > 11)
		{
			cv_LasyaAttack->StopAndFadeOutVideo(2.0f);
			isLasyaAttackVidPlaying = false;
			subtitleText->SetText("");
			Hud->AllowInput = true;
			Hud->ShowHud(false);

			if (!Control::Audio->IsPlaying(aTrackL1Trapped))
				Control::Audio->PlaySample(aTrackL1Trapped, true);
			if (!Control::Audio->IsPlaying(aAmbS5TreasureRoom))
				Control::Audio->PlaySample(aAmbS5TreasureRoom, true);

			//Control::Audio->StopSample(aTrackCutscnS5MayaAttack);

			AwardManager::GetInstance()->CompleteHO(eArea_L1_S5_HOTreasureRoom);
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
		//GFApp->debugMsg = std::to_string(vidTimer);
		
		return;
	}


	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
			{
				Treasurematch3->clearBoard();
				BtnMatch3->Show(false);
				BtnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
				return;
			}
#endif

		if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
		{
			if( Control::Input->LBclicked() )
			{
				Treasurematch3->clearBoard();
				BtnMatch3->Show(false);
				BtnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
		if( IsMouseOverGoBackArea(&pos )
			#ifdef M3_BUTTON
			&& BtnMatch3->Visible
			#endif
		)
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S5_TreasureRoom);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S5_TreasureRoom);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S5_TreasureRoom, 2);
				}
#else
				Treasurematch3->clearBoard();
				BtnMatch3->Show(false);
				BtnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				return;
			}			
		}

		if(BtnRectMatch3.Intersection(&pos) && !Treasurematch3->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSwitchToHO->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				HOInvBox->SetisEnabled(true);

				Treasurematch3->clearBoard();
				BtnMatch3->Show(true);
				BtnSwitchToHO->Show(false);
				BtnSwitchToHO->PlayAnimation(0);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Hud->InvBoxCtrl->SetLockBase(HO_USEOBJECTS_FRAME);
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
		if(!isObjectInHand && !isP1bookLockPartSelected && checkForDestAreaClicks(pos))
			return;

		HOInvBox->HandleMouseInput(Control::Input);

		if(isObjectInHand)
		{
			SCN_ItemsOnHand[objectInHandId]->SetPos(&pos);

			if(Control::Input->RBclicked()) // problem for touch devices
			{
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[objectInHandId]->SetEnable(false);
				HOInvBox->CollectItem(objectInHandId, pos);
				return;
			}
			else if(Control::Input->LBclicked())
			{
				OnItemOnHandClickHandler(pos);
				return;
			}
		}

		if(ActivePopupID == eHOPop_Inactive)
		{
			//match 3 button active only when in scene
			if(BtnRectMatch3.Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandPointy); //sree
				BtnMatch3->PlayAnimation(1);
				if( Control::Input->LBclicked() )
				{
					HOInvBox->SetisEnabled(false);

					if(Treasurematch3 == NULL)
					{
						Treasurematch3 = new Match3(3);
						AddControl(Treasurematch3);
					}
					else
					{
						Treasurematch3->ShowBoard();
					}

					BtnMatch3->Show(false);
					BtnMatch3->PlayAnimation(0);
					BtnSwitchToHO->Show(true);
					isMatch3Playing = true;
					ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;

					Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
					return;
				}
			}
			else
			{
				BtnMatch3->PlayAnimation(0);
			}

			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] < HOITEM_TAKEN && InvRectLocket.Intersection(&pos))
			{
				if( Control::Input->LBclicked() )
				{
					vfx_collectFX->SetPos(&pos);
					vfx_collectFX->Start();
					HOInvBox->CollectItem(eHOItem1_NeckLocket, pos);
					ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] = HOITEM_TAKEN;
					SceneObjectsUpdate();
				}
			}
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] < HOITEM_TAKEN && ProfileSystem::Instance->ActiveProfile.L1S5_HOClothFolded && InvRectBookPart1.Intersection(&pos))
			{
				if( Control::Input->LBclicked() )
				{
					HOInvBox->CollectItem(eHOItem2_BookSymbolPartHorn, pos);
					ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] = HOITEM_TAKEN;
					SceneObjectsUpdate();
				}
			}
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] == HOITEM_TAKEN && !SCN_bookLockPartFixed1->IsActive)
            {
                InventoryItemState i = HOInvBox->GetInventoryItem(eHOItem2_BookSymbolPartHorn)->GetItemState();
			    if(i == kInventoryItemState_NotCollected)
                {
			        HOInvBox->CollectItem(eHOItem2_BookSymbolPartHorn, pos);
                    SceneObjectsUpdate();
                }
            }
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOClothFolded && ActionRectCloth.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked() )
				{
					SCN_clothOpen->SetEnable(false);
					SCN_clothFolded->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L1S5_HOClothFolded = true;
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5ClothMove);
				}
			}
		}
#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (ActivePopupID == eHOPop_Inactive && Control::Input->isKeyBackSensed())
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
			return;
		}
#endif
		if( ActivePopupID == eHOPop_Inactive && IsMouseOverGoBackArea(&pos ))
		{
			#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S5_TreasureRoom);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S5_TreasureRoom);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S5_TreasureRoom, 2);
				}
#else
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}			
			return;
		}

		if( !IsPopupTransitionActive )
			UpdatePopups(pos);
	}	

	//ResetHO();
}

void L1_S5_HOTreasureRoom::UpdatePopups(CPoint pos,bool ItemDropped)
{
	if(ActivePopupID == eHOPop_Inactive)
	{
		for (int i = 0; i < eHOPop_Max; i++)
		{
			if(Pop_States[i] != HOPOP_INACTIVE) //if popup is active
			{
				if(Pop_ScnPolyArray[i]!=NULL && Pop_ScnPolyArray[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_Look);
					if( Control::Input->LBclicked() )
					{
						ActivePopupID = i;
						Pop_States[i] = HOPOP_SCALEUP;

						Pop_CloseButton = Pop_SCNArray[i]->GetObjectByID("popClose");
						Pop_CloseButton->PlayAnimation(0);

						PopVal = 0;

						SetupPopupTransition(Pop_SCNArray[i], Pop_ScnPolyArray[i]->center.x, Pop_ScnPolyArray[i]->center.y);
					}
					break;
				}
			}
		}
	}
	else
	{
		//popup is opened
		switch (ActivePopupID)
		{
		case eHOPop1_BookLocketPop:
			{
				if(p1IsWaitingForBookOpen)
				{
					p1BookOpenWaitTimer += Control::LogicRate;
					if(p1BookOpenWaitTimer >2.25f)
					{
						Pop_SCNArray[eHOPop1_BookLocketPop]->GetObjectByID("P1bookOpen")->SetEnable(true);
						Pop_SCNArray[eHOPop1_BookLocketPop]->GetObjectByID("P1bookOpen")->FadeIn(1.25f);

						Pop_SCNArray[eHOPop1_BookLocketPop]->GetObjectByID("P1ringBox")->SetEnable(true);
						Pop_SCNArray[eHOPop1_BookLocketPop]->GetObjectByID("P1ringBox")->FadeIn(1.25f);

						P1bookLockPartFixed1->FadeOut(1.0f);
						P1bookLockPartFixed2->FadeOut(1.0f);
						P1bookLockPartFixed3->SetAlpha(1.0f);
						P1bookLockPartFixed3->FadeOut(1.0f);
						P1bookLockPartFixed4->SetAlpha(1.0f);
						P1bookLockPartFixed4->FadeOut(1.0f);

						p1IsWaitingForBookOpen = false;
		
						SceneObjectsUpdate();
					}
					return;
				}

				if(p1IsKeyTurnAnimPlaying)
				{
					if(p1KeyTurnAnim->IsAnimationFinished())
					{
						p1IsKeyTurnAnimPlaying = false;
						p1KeyTurnAnim->FadeOut(1.45f);

						Pop_SCNArray[eHOPop1_BookLocketPop]->GetObjectByID("P1ringBoxOpen")->SetEnable(true);
						Pop_SCNArray[eHOPop1_BookLocketPop]->GetObjectByID("P1ringBoxOpen")->FadeIn(1.5f);

						Pop_SCNArray[eHOPop1_BookLocketPop]->GetObjectByID("P1coin")->SetEnable(true);
						Pop_SCNArray[eHOPop1_BookLocketPop]->GetObjectByID("P1coin")->FadeIn(1.5f);
					}
					return;
				}

				Pop_CloseButton->PlayAnimation(0);
				if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
				{
					if(PopCloseButtonRect.Intersection(&pos))
						Cursor::SetMode(CA_HandPointy);
					Pop_CloseButton->PlayAnimation(1);
					if( Control::Input->LBclicked() )
					{
						CloseActivePopup();
					}
				}
				else
				{
					//Do Other Things here
					UpdateP1BookHO(pos);
				}
			}
			break;
		case eHOPop2_ColorSlidePuzzleBox:
			{
				if(p2IsPuzzleEndPlaying)
				{
					p2PuzzleEndTimer += Control::LogicRate;
					if(p2PuzzleEndTimer>2.65f)
					{
						SceneObjectsUpdate();
						Pop_SCNArray[eHOPop2_ColorSlidePuzzleBox]->GetObjectByID("P2coin")->FadeOut(1.5f);
						Pop_SCNArray[eHOPop2_ColorSlidePuzzleBox]->GetObjectByID("P2PuzzleBg")->FadeOut(1.5f);
						for (int i = 0; i < MAX_L1S5HO_P2COLORTOKENS; i++)
						{
							p2ColorTokenArray[i]->FadeOut(1.5f);
						}
						for (int i = 0; i < 5; i++)
						{
							p2BtnLocketArray[i]->FadeOut(1.5f);
						}

						p2IsPuzzleEndPlaying = false;
					}
					return;
				}

				if(p2HelpBtn->IsActive)
				{
					if(p2HelpFrameBG->IsActive)
					{
						if(p2HelpFrameBG->Collide(&pos))
						{
							Cursor::SetMode(CA_HandPointy);
						}
						if( Control::Input->LBclicked() )
						{
							p2Helptext->Show(false);
							p2HelpFrameBG->SetEnable(false);
						}
						return;
					}

					if(p2HelpBtn->Collide(&pos))
					{
						p2HelpBtn->PlayAnimation(1);
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked() )
						{
							p2Helptext->Show(true);
							p2HelpFrameBG->SetEnable(true);
							//Hud->ShowBannerText("L1open");
							return;
						}
					}
					else
					{
						p2HelpBtn->PlayAnimation(0);
					}
				}

				if(p2SkipBtn->IsActive)
				{
					if(p2SkipPuzzleTimer<p2SkipPuzzleThreshold)
					{
						//check for skip
						p2SkipPuzzleTimer += Control::LogicRate;
						int frame = int(p2SkipPuzzleTimer/p2SkipPuzzleThreshold*59.0f);
						p2SkipBtnActive->SetFrame(frame);

						if(p2SkipRect.Intersection(&pos))
						{
							Cursor::SetMode(CA_Help);
							if( Control::Input->LBclicked() )
							{
								Hud->ShowBannerText("L1srecharge");
							}
						}
					}
					else
					{
						if(p2SkipRect.Intersection(&pos))
						{
							p2SkipBtnGlow->SetEnable(true);
							Cursor::SetMode(CA_HandPointy);
							if( Control::Input->LBclicked() )
							{
								p2HelpBtn->SetEnable(false);
								p2SkipBtn->SetEnable(false);
								skipText->SetEnable(false);
								p2SkipBtnGlow->SetEnable(false);
								p2SkipBtnActive->SetEnable(false);

								//Move to original positions
								for (int i = 0; i < MAX_L1S5HO_P2COLORTOKENS; i++)
								{
									p2ColorTokenArray[i]->Pos = p2TokenPosArray[i];									
								}

								//Game Won through Skip
								ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleSolved = true;
								ProfileSystem::Instance->ActiveProfile.L1S5_HOChestBoxOpened = true;

								ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] = HOITEM_UNLOCKED;

								p2IsPuzzleEndPlaying = true;
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5BookOpenPzlSolved);
								p2Vfx_PuzEndFX->Start();

								for (int i = 0; i < 5; i++)
								{
									p2BtnLocketArray[i]->SetEnable(true);
									p2BtnLocketArray[i]->SetAlpha(0);
									p2BtnLocketArray[i]->FadeIn(1.0f);
								}
								return;
							}
						}
						else
						{
							p2SkipBtnGlow->SetEnable(false);
						}
					}
				}

				Pop_CloseButton->PlayAnimation(0);
				if( ( (!PopAreaRect.Intersection(&pos) && !(p2SkipBtn->IsActive && p2SkipRect.Intersection(&pos)) && !(p2HelpBtn->IsActive && p2HelpBtn->Collide(&pos)) ) || PopCloseButtonRect.Intersection(&pos)) )//clicked outside or close Btn
				{
					if(PopCloseButtonRect.Intersection(&pos))
						Cursor::SetMode(CA_HandPointy);
					Pop_CloseButton->PlayAnimation(1);
					if( Control::Input->LBclicked() )
					{
						CloseActivePopup();
					}
				}
				else
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleSolved)
					{
						if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] == HOITEM_UNLOCKED)
						{
							if(ItemRect_HOItemArray[eHOItem9_Pestle]->Intersection(&pos))
							{
								Cursor::SetMode(CA_HandTake);
								if( Control::Input->LBclicked() )
								{
									ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] = HOITEM_TAKEN;
									SceneObjectsUpdate();
									HOInvBox->CollectItem(eHOItem9_Pestle, pos);
									Pop_SCNArray[eHOPop2_ColorSlidePuzzleBox]->GetObjectByID("P2Pestel")->SetEnable(false);

									CloseActivePopup(true);
								}
							}
						}
						return;
					}
					else if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] != HOITEM_USED)
					{
						return;
					}
					//Do Other Things here
					if(!p2IsTokenRotating && !p2IsTokenSwapping)
					{
						//check for button clicks
						bool isAnyButtonOver = false;
						for (int i = 0; i < 5; i++)
						{
							if(p2BtnLocketArray[i]->Collide(&pos))
							{
								isAnyButtonOver = true;
								
								Cursor::SetMode(CA_HandPointy);
								p2BtnLocketArray[i]->SetEnable(true);
								if( Control::Input->LBclicked() )
								{
									//swap the surrounding tokens

									for (int j = 0; j < 4; j++)
									{
										p2SwappingTokenIds[j] = p2PosContains[P2SwapTokenIds[i][j]];
										p2SwapPosIds[j] = P2SwapTokenIds[i][j];
									}

									if(p2IsFirstSwapObjClicked)
									{
										p2IsFirstSwapObjClicked = false;

										p2ColorTokenArray[p2SwapId1]->PlayAnimation(0);
									}

									p2TokenSwapTimer = 0;
									p2IsTokenRotating = true;
									Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5Popup);
								}
							}
							else
								p2BtnLocketArray[i]->SetEnable(false);
						}

						//check for Swap
						if(!isAnyButtonOver)
						{
							for (int i = 0; i < MAX_L1S5HO_P2COLORTOKENS; i++)
							{
								if(p2IsFirstSwapObjClicked && i==p2SwapId1)
									continue;
								if(p2ColorTokenArray[i]->Collide(&pos))
								{
									Cursor::SetMode(CA_HandPointy);
									p2ColorTokenArray[i]->PlayAnimation(1);
									if( Control::Input->LBclicked() )
									{
										if(!p2IsFirstSwapObjClicked)
										{
											Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5Popup);
											p2IsFirstSwapObjClicked = true;											
											p2SwapId1 = i;
										}
										else
										{
											p2SwapId2 = i;
											for (int i = 0; i < MAX_L1S5HO_P2COLORTOKENS; i++)
											{
												if(p2PosContains[i] == p2SwapId1)
													p2Swap1PosId = i;
												if(p2PosContains[i] == p2SwapId2)
													p2Swap2PosId = i;
											}
											//check for adjascent swap buddy
											bool isAdjascent = false;
											int adjArray[8][3]={
												{1,3,5},
												{0,2,6},
												{1,3,7},
												{0,2,4},

												{3,5,7},
												{0,4,6},
												{1,5,7},
												{2,4,6}
											};
											for (int i = 0; i < 3; i++)
											{
												if(p2Swap2PosId == adjArray[p2Swap1PosId][i])
													isAdjascent = true;
											}
											if(isAdjascent)
											{
												p2TokenSwapTimer = 0;
												p2IsTokenSwapping = true;
												if( rand()%2 )
													Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5SwapMB1);
												else
													Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5SwapMB2);
											}
											else
											{
												Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5Popup);
												p2ColorTokenArray[p2SwapId1]->PlayAnimation(0);
												p2SwapId1 = p2SwapId2;
											}
										}
									}
								}
								else
									p2ColorTokenArray[i]->PlayAnimation(0);
							}
						}
					}
					else
					{
						//update Rotating here
						p2TokenSwapTimer += 1.65f*Control::LogicRate;

						if (p2IsTokenRotating)
						{
							for (int i = 0; i < 4; i++)
							{
								if(i==3)
								{
									p2ColorTokenArray[p2SwappingTokenIds[i]]->Pos.x = p2TokenPosArray[p2SwapPosIds[i]].x + (p2TokenPosArray[p2SwapPosIds[0]].x - p2TokenPosArray[p2SwapPosIds[i]].x)*easingQuadOut(p2TokenSwapTimer,0,1,p2TotalSwapTime);
									p2ColorTokenArray[p2SwappingTokenIds[i]]->Pos.y = p2TokenPosArray[p2SwapPosIds[i]].y + (p2TokenPosArray[p2SwapPosIds[0]].y - p2TokenPosArray[p2SwapPosIds[i]].y)*easingQuadOut(p2TokenSwapTimer,0,1,p2TotalSwapTime);
								}
								else
								{
									p2ColorTokenArray[p2SwappingTokenIds[i]]->Pos.x = p2TokenPosArray[p2SwapPosIds[i]].x + (p2TokenPosArray[p2SwapPosIds[i+1]].x - p2TokenPosArray[p2SwapPosIds[i]].x)*easingQuadOut(p2TokenSwapTimer,0,1,p2TotalSwapTime);
									p2ColorTokenArray[p2SwappingTokenIds[i]]->Pos.y = p2TokenPosArray[p2SwapPosIds[i]].y + (p2TokenPosArray[p2SwapPosIds[i+1]].y - p2TokenPosArray[p2SwapPosIds[i]].y)*easingQuadOut(p2TokenSwapTimer,0,1,p2TotalSwapTime);
								}
							} 
						}
						else if (p2IsTokenSwapping)
						{
							p2ColorTokenArray[p2SwapId1]->Pos.x = p2TokenPosArray[p2Swap1PosId].x + (p2TokenPosArray[p2Swap2PosId].x - p2TokenPosArray[p2Swap1PosId].x)*easingQuadOut(p2TokenSwapTimer,0,1,p2TotalSwapTime);
							p2ColorTokenArray[p2SwapId1]->Pos.y = p2TokenPosArray[p2Swap1PosId].y + (p2TokenPosArray[p2Swap2PosId].y - p2TokenPosArray[p2Swap1PosId].y)*easingQuadOut(p2TokenSwapTimer,0,1,p2TotalSwapTime);

							p2ColorTokenArray[p2SwapId2]->Pos.x = p2TokenPosArray[p2Swap2PosId].x + (p2TokenPosArray[p2Swap1PosId].x - p2TokenPosArray[p2Swap2PosId].x)*easingQuadOut(p2TokenSwapTimer,0,1,p2TotalSwapTime);
							p2ColorTokenArray[p2SwapId2]->Pos.y = p2TokenPosArray[p2Swap2PosId].y + (p2TokenPosArray[p2Swap1PosId].y - p2TokenPosArray[p2Swap2PosId].y)*easingQuadOut(p2TokenSwapTimer,0,1,p2TotalSwapTime);
						}

						if(p2TokenSwapTimer>=p2TotalSwapTime)
						{
							p2TokenSwapTimer = 0;

							if (p2IsTokenRotating)
							{
								//Adjust position
								for (int i = 0; i < 4; i++)
								{
									if(i==3)
									{
										p2ColorTokenArray[p2SwappingTokenIds[i]]->Pos = p2TokenPosArray[p2SwapPosIds[0]];
									}
									else
									{
										p2ColorTokenArray[p2SwappingTokenIds[i]]->Pos = p2TokenPosArray[p2SwapPosIds[i+1]];											
									}
								}

								//Change the Swapping Ids
								int temp = p2PosContains[p2SwapPosIds[0]];
								p2PosContains[p2SwapPosIds[0]] = p2PosContains[p2SwapPosIds[3]];
								p2PosContains[p2SwapPosIds[3]] = p2PosContains[p2SwapPosIds[2]];
								p2PosContains[p2SwapPosIds[2]] = p2PosContains[p2SwapPosIds[1]];
								p2PosContains[p2SwapPosIds[1]] = temp;

								p2IsTokenRotating = false;
							}
							else if (p2IsTokenSwapping)
							{
								int temp = p2PosContains[p2Swap2PosId];
								p2PosContains[p2Swap2PosId] = p2PosContains[p2Swap1PosId];
								p2PosContains[p2Swap1PosId] = temp;

								p2IsFirstSwapObjClicked = false;

								p2ColorTokenArray[p2SwapId1]->PlayAnimation(0);
								p2ColorTokenArray[p2SwapId2]->PlayAnimation(0);

								p2IsTokenSwapping = false;
							}

							//Save Pos Vars
							for (int j = 0; j < MAX_L1S5HO_P2COLORTOKENS; j++)
							{
								ProfileSystem::Instance->ActiveProfile.L1S5_HOP2TokenPosContains[j] = p2PosContains[j];
							}

							//Check if puzzle is solved
							bool IsGameWon = true;
							for (int i = 0; i < MAX_L1S5HO_P2COLORTOKENS; i++)	
							{
								if(p2PosContains[i] != i)
								{
									IsGameWon = false;
									break;
								}
							}

							if(IsGameWon)
							{
								//Turn off Skip Buttons
								p2HelpBtn->SetEnable(false);
								p2SkipBtn->SetEnable(false);
								skipText->SetEnable(false);
								p2SkipBtnGlow->SetEnable(false);
								p2SkipBtnActive->SetEnable(false);

								ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleSolved = true;
								ProfileSystem::Instance->ActiveProfile.L1S5_HOChestBoxOpened = true;

								ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] = HOITEM_UNLOCKED;

								p2IsPuzzleEndPlaying = true;
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5BookOpenPzlSolved);
								p2Vfx_PuzEndFX->Start();

								for (int i = 0; i < 5; i++)
								{
									p2BtnLocketArray[i]->SetEnable(true);
									p2BtnLocketArray[i]->SetAlpha(0);
									p2BtnLocketArray[i]->FadeIn(1.0f);
								}
							}
						}
					}
				}
			}
			break;
		case eHOPop3_Clock:
			{
				if(p3IsClockNeedleFixPlaying)
				{
					if(CAnim_ClockNeedleFix->IsPlaying())
					{
						if(CAnim_ClockNeedleFix->IsAnimFinished())
						{
							CAnim_ClockNeedleFix->Stop();

							Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3needle")->SetPivot(-102,-33);
							p3IsClockNeedlesRotating = true;
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5ClockHandleFix);
						}
					}
					else
					{
						p3NeedleRotateTimer += Control::LogicRate;
						Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3needle")->SetRelativeRotate(-120*Control::LogicRate);
						Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3HourNeedle")->SetRelativeRotate(-40*Control::LogicRate);

						if(p3NeedleRotateTimer>1.5f)
						{
							Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3clockOpen")->SetEnable(true);
							Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3clockOpen")->FadeIn(1.0f);
							Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3handle")->SetEnable(true);
							Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3handle")->FadeIn(1.0f);
							
							Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3needle")->FadeOut(1.0f);
							Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3HourNeedle")->FadeOut(1.0f);
							Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3clockClosed")->FadeOut(1.0f);

							p3IsClockNeedleFixPlaying = false;
						}
					}
					return;
				}

				Pop_CloseButton->PlayAnimation(0);
				if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
				{
					if(PopCloseButtonRect.Intersection(&pos))
						Cursor::SetMode(CA_HandPointy);
					Pop_CloseButton->PlayAnimation(1);
					if( Control::Input->LBclicked() )
					{
						CloseActivePopup();
					}
				}
				else
				{
					//Do Other Things here
					if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] == HOITEM_UNLOCKED)
					{
						if(ItemRect_HOItemArray[eHOItem5_DrawerHandle]->Intersection(&pos))
						{
							Cursor::SetMode(CA_HandTake);
							if( Control::Input->LBclicked() )
							{
								ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] = HOITEM_TAKEN;
								SceneObjectsUpdate();
								HOInvBox->CollectItem(eHOItem5_DrawerHandle, pos);
								Pop_SCNArray[eHOPop3_Clock]->GetObjectByID("P3handle")->SetEnable(false);

								CloseActivePopup(true);
							}
						}
					}
					
				}
			}
			break;
		case eHOPop4_StatueHanuman:
			{
				if(p4IsLocketFixAnimPlaying)
				{
					if(p4IsHandOpenFadingIn)
					{
						p4LocketFixFadeInTimer += Control::LogicRate;
						if(p4LocketFixFadeInTimer>0.75f)
						{
							p4IsHandOpenFadingIn = false;
							p4IsLocketFixAnimPlaying = false;
						}
					}
					else if(CAnim_LocketFix->IsAnimFinished())
					{
						CAnim_LocketFix->Stop();

						p4IsHandOpenFadingIn = true;
						
						Pop_SCNArray[eHOPop4_StatueHanuman]->GetObjectByID("P4eagleEye")->SetEnable(true);
						Pop_SCNArray[eHOPop4_StatueHanuman]->GetObjectByID("P4eagleEye")->FadeIn(1.5f);

						Pop_SCNArray[eHOPop4_StatueHanuman]->GetObjectByID("P4handClosed")->FadeOut(1.5f);

						Pop_SCNArray[eHOPop4_StatueHanuman]->GetObjectByID("P4handOpen")->SetEnable(true);
						Pop_SCNArray[eHOPop4_StatueHanuman]->GetObjectByID("P4handOpen")->FadeIn(1.5f);

						p4Vfx_locketfix1->Start();
						p4Vfx_locketfix2->Start();
					}
					return;
				}
				
				Pop_CloseButton->PlayAnimation(0);
				if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
				{
					if(PopCloseButtonRect.Intersection(&pos))
						Cursor::SetMode(CA_HandPointy);
					Pop_CloseButton->PlayAnimation(1);
					if( Control::Input->LBclicked() )
					{
						CloseActivePopup();
					}
				}
				else
				{
					//Do Other Things here					
					if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] == HOITEM_UNLOCKED)
					{
						if(ItemRect_HOItemArray[eHOItem3_EagleEye]->Intersection(&pos))
						{
							Cursor::SetMode(CA_HandTake);
							if( Control::Input->LBclicked() )
							{
								ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] = HOITEM_TAKEN;
								SceneObjectsUpdate();
								HOInvBox->CollectItem(eHOItem3_EagleEye, pos);
								Pop_SCNArray[eHOPop4_StatueHanuman]->GetObjectByID("P4eagleEye")->SetEnable(false);

								CloseActivePopup(true);
							}
						}
					}
				}
			}
			break;
		case eHOPop5_Eagle:
			{
				Pop_CloseButton->PlayAnimation(0);
				if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
				{
					if(PopCloseButtonRect.Intersection(&pos))
						Cursor::SetMode(CA_HandPointy);
					Pop_CloseButton->PlayAnimation(1);
					if( Control::Input->LBclicked() )
					{
						CloseActivePopup();
					}
				}
				else
				{
					//Do Other Things here
					if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] == HOITEM_UNLOCKED)
					{
						if(ItemRect_HOItemArray[eHOItem4_Clockneedle]->Intersection(&pos))
						{
							Cursor::SetMode(CA_HandTake);
							if( Control::Input->LBclicked() )
							{
								ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] = HOITEM_TAKEN;
								HOInvBox->CollectItem(eHOItem4_Clockneedle, pos);
								Pop_SCNArray[eHOPop5_Eagle]->GetObjectByID("P5MouthOpen")->PlayAnimation(1);
								CloseActivePopup(true);
							}
						}
					}
				}
			}
			break;
		case eHOPop6_ShelfSwapPuzzle:
			{
				if( isP6ShelfOpenAnimPlaying)
				{
					if(CAnim_P6ShelfOpen->IsAnimFinished())
					{
						P6_shelfOpen->SetEnable(true);
						P6_shelfOpenAnim->FadeOut(4.0f);
						//P6_shelfOpenAnim->SetEnable(false);
						isP6ShelfOpenAnimPlaying = false;
					}
					return;
				}

				Pop_CloseButton->PlayAnimation(0);
				if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
				{
					if(PopCloseButtonRect.Intersection(&pos))
						Cursor::SetMode(CA_HandPointy);
					Pop_CloseButton->PlayAnimation(1);
					if( Control::Input->LBclicked() )
					{
						CloseActivePopup();
					}
				}
				else
				{
					//Do Other Things here
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved)
						UpdateP6SwapPieceHO(pos);
					else
					{
						//Do stuff here
						if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] >= HOITEM_TAKEN)
						{
							P6_bookLockPart->SetEnable(false);
						}
						else if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] == HOITEM_UNLOCKED)
						{
							if(ItemRect_HOItemArray[eHOItem7_BookSymbolPartHead]->Intersection(&pos))
							{
								Cursor::SetMode(CA_HandTake);
								if( Control::Input->LBclicked() )
								{
									ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] = HOITEM_TAKEN;
									HOInvBox->CollectItem(eHOItem7_BookSymbolPartHead, pos);
									P6_bookLockPart->SetEnable(false);
									if( ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] == HOITEM_TAKEN)
									{
										CloseActivePopup();
									}
								}
							}
						}
						if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] >= HOITEM_TAKEN)
						{
							P6_Key->SetEnable(false);
						}
						else if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] == HOITEM_UNLOCKED)
						{
							if(ItemRect_HOItemArray[eHOItem6_KeyRingBox]->Intersection(&pos))
							{
								Cursor::SetMode(CA_HandTake);
								if( Control::Input->LBclicked() )
								{
									ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] = HOITEM_TAKEN;
									HOInvBox->CollectItem(eHOItem6_KeyRingBox, pos);
									P6_Key->SetEnable(false);
									if( ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] == HOITEM_TAKEN)
									{
										CloseActivePopup();
									}
								}
							}
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

void L1_S5_HOTreasureRoom::UpdateP1BookHO(CPoint pos)
{
	if( ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened )
	{
		//Ring Box Opened
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] == HOITEM_USED)
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] == HOITEM_UNLOCKED)
			{
				if(ItemRect_HOItemArray[eHOItem8_TokenBoxPuzzle]->Intersection(&pos))
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked() )
					{
						ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] = HOITEM_TAKEN;
						SceneObjectsUpdate();
						HOInvBox->CollectItem(eHOItem8_TokenBoxPuzzle, pos);
						Pop_SCNArray[eHOPop1_BookLocketPop]->GetObjectByID("P1coin")->SetEnable(false);

						CloseActivePopup(true);
					}
				}
			}
		}	
		return;
	}

	if( isP1bookLockPartSelected && P1SelectedLockPart == 1)
	{
		Cursor::SetMode(CA_HandPointy);
		P1bookLockPart1->Pos = pos;
		if( P1bookLockPartFixed1->Collide(&pos))
		{
			Cursor::SetMode(CA_Gear);
		}
		if( Control::Input->LBclicked() )
		{
			isP1bookLockPartSelected = false;
			P1SelectedLockPart = -1;
			if( P1bookLockPartFixed1->Collide(&pos))
			{
				P1bookLockPart1->FadeOut(2.0f);
				P1bookLockPartFixed1->SetEnable(true);
				P1bookLockPartFixed1->SetAlpha(0.0f);
				P1bookLockPartFixed1->FadeIn(2.0f);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5GarudaEmblemFix);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart2Fixed = true;
				SceneObjectsUpdate();
			}
			else
			{
				P1bookLockPart1->Pos = P1bookLockPart1->InitPos;
			}
		}
	}
	else if( isP1bookLockPartSelected && P1SelectedLockPart == 2)
	{
		Cursor::SetMode(CA_HandPointy);
		P1bookLockPart2->Pos = pos;
		if( P1bookLockPartFixed2->Collide(&pos))
		{
			Cursor::SetMode(CA_Gear);
		}
		if( Control::Input->LBclicked() )
		{
			isP1bookLockPartSelected = false;
			P1SelectedLockPart = -1;
			if( P1bookLockPartFixed2->Collide(&pos))
			{
				P1bookLockPart2->FadeOut(2.0f);
				P1bookLockPartFixed2->SetEnable(true);
				P1bookLockPartFixed2->SetAlpha(0.0f);
				P1bookLockPartFixed2->FadeIn(2.0f);
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5GarudaEmblemFix);
				ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart3Fixed = true;
				SceneObjectsUpdate();
			}
			else
			{
				P1bookLockPart2->Pos = P1bookLockPart2->InitPos;
			}
		}
	}
	else 
	{
		if( P1BookLockPart1Rect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart2Fixed )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5Popup);
				P1SelectedLockPart = 1;
				isP1bookLockPartSelected = true;
			}
		}
		if( P1BookLockPart2Rect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart3Fixed )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5Popup);
				P1SelectedLockPart = 2;
				isP1bookLockPartSelected = true;
			}
		}
	}
	if( ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart2Fixed && ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart3Fixed &&	 ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] == HOITEM_USED && ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] == HOITEM_USED)
	{
		//Book Opens
		p1IsWaitingForBookOpen = true;

		ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened = true;

		p1Vfx_BookOpen->Start();
		p1Vfx_BookOpen->AdvanceByTime(0.25f);		
		Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4BookOpened);
	}
}


void L1_S5_HOTreasureRoom::UpdateP6SwapPieceHO(CPoint pos )
{
	if( isP6SwappingInProgress )
	{
		if( !P6_SwapPiece [P6_SelectedPieceIndex]->IsTweening )
		{
			P6_SwapPiece[p6_prevSelectedPieceIndex]->SetZ( P6_SwapPiece[p6_prevSelectedPieceIndex]->ZOrder - 10 );
			P6_SwapPiece[P6_SelectedPieceIndex]->SetZ( P6_SwapPiece[P6_SelectedPieceIndex]->ZOrder - 10 );
			P6_SwapPiece[P6_SelectedPieceIndex]->PlayAnimation(0);
			P6_SwapPiece[p6_prevSelectedPieceIndex]->PlayAnimation(0);
			P6_SelectedPieceIndex = -1;
			if( CheckP6SwapPieceHoComplete() )
			{
				isP6SwapHOPuzzleSolved = true;
				ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved = true;
			}
			isP6SwappingInProgress = false;
		}
		return;
	}
	if(isP6SwapHOPuzzleSolved )
	{
		return;
	}
	//check if clicked on swap objects.
	for( int i = 0; i < 7; i++ )
	{
		if( P6_SwapPiece[i]->Collide(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			if( Control::Input->LBclicked() )
			{
				if( !isP6SwapPieceSelected )
				{
					isP6SwapPieceSelected = true;
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5Popup);
					P6_SelectedPieceIndex = i;
					P6_SwapPiece[P6_SelectedPieceIndex]->PlayAnimation(1);
					break;
				}
				else if( P6_SelectedPieceIndex != i && P6_SelectedPieceIndex != -1 )
				{
					isP6SwapPieceSelected = false;
					p6_prevSelectedPieceIndex = P6_SelectedPieceIndex;
					P6_SelectedPieceIndex = i;
					P6_SwapPiece[P6_SelectedPieceIndex]->PlayAnimation(1);
					SwapHOPiecesP6();
					break; //break from loop to avoid multiple selection
				}
			}
		}
	}
}

void L1_S5_HOTreasureRoom::SwapHOPiecesP6()
{
	
	int tempIndex = p6_IndexArray[p6_prevSelectedPieceIndex];
	p6_IndexArray[p6_prevSelectedPieceIndex] = p6_IndexArray[P6_SelectedPieceIndex];
	p6_IndexArray[P6_SelectedPieceIndex] = tempIndex;
					
	/*CPoint temp = P6_SwapPiece[second]->Pos;
	P6_SwapPiece[second]->Pos = P6_SwapPiece[first]->Pos;
	P6_SwapPiece[first]->Pos = temp;*/

	P6_SwapPiece[p6_prevSelectedPieceIndex]->SetZ( P6_SwapPiece[p6_prevSelectedPieceIndex]->ZOrder + 10 );
	P6_SwapPiece[P6_SelectedPieceIndex]->SetZ( P6_SwapPiece[P6_SelectedPieceIndex]->ZOrder + 10 );

	float tweenArray[] = {1.0f,P6_SwapPiece[P6_SelectedPieceIndex]->Pos.x, P6_SwapPiece[P6_SelectedPieceIndex]->Pos.y, 1,1};
	P6_SwapPiece[p6_prevSelectedPieceIndex]->TweenTo( tweenArray, 0.5f, TW_EASE, true );
	float tweenArray2[] =  {1.0f,P6_SwapPiece[p6_prevSelectedPieceIndex]->Pos.x, P6_SwapPiece[P6_SelectedPieceIndex]->Pos.y, 1,1};
	P6_SwapPiece[P6_SelectedPieceIndex]->TweenTo( tweenArray2, 0.5f, TW_EASE, true );
	isP6SwappingInProgress = true;
	Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4BooksMoved);
}


bool  L1_S5_HOTreasureRoom::CheckP6SwapPieceHoComplete()
{
	bool result = true;
	for( int i = 0; i < 7; i++ )
	{
		if( p6_IndexArray[i] != i )	
		{
			result = false;
		}
	}
	return(result);
}

void L1_S5_HOTreasureRoom::CloseActivePopup(bool isDelayed)
{
	if(isDelayed)
		popupCloseDelayTimer = 1.0f;

	Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5PopDown);
	//Control::Audio->PlaySample(aSFXpopdown);
	Pop_States[ActivePopupID] = HOPOP_SCALEDOWN;
	Pop_SCNArray[ActivePopupID]->Show(false);
	SetupPopupTransition(Pop_SCNArray[ActivePopupID], Pop_ScnPolyArray[ActivePopupID]->center.x, Pop_ScnPolyArray[ActivePopupID]->center.y);
}

void L1_S5_HOTreasureRoom::PopupClosePostUpdate(int popupId)
{
	//After popup Closed Disable popups if necessary
	switch (popupId)
	{
	case eHOPop1_BookLocketPop:
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] >= HOITEM_TAKEN)
		{			
			Pop_States[popupId] = HOPOP_INACTIVE;
		}
		break;
	case eHOPop2_ColorSlidePuzzleBox:
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] >= HOITEM_TAKEN)
		{
			Pop_States[popupId] = HOPOP_INACTIVE;
		}
		break;
	case eHOPop3_Clock:
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] >= HOITEM_TAKEN)
		{
			Pop_States[popupId] = HOPOP_INACTIVE;
		}
		break;
	case eHOPop4_StatueHanuman:
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] >= HOITEM_TAKEN)
		{
			Pop_States[popupId] = HOPOP_INACTIVE;
		}
		break;
	case eHOPop5_Eagle:
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] >= HOITEM_TAKEN)
		{
			Pop_States[popupId] = HOPOP_INACTIVE;
		}
		break;
	case eHOPop6_ShelfSwapPuzzle:
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] >= HOITEM_TAKEN && ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] >= HOITEM_TAKEN)
		{
			Pop_States[popupId] = HOPOP_INACTIVE;
		}
		break;
	}
}

void  L1_S5_HOTreasureRoom::SetupPopupTransition(CHOScene *Popup, float centerX, float centerY)
{

		HOScene->PauseObjects(true);

		/*int size = VFXArray.size();
		for (int i = 0; i < size; i++)		
			VFXArray[i]->Show(false);*/
		
		/*bgScaleOffsetX = popRect.GetCenterX();
		bgScaleOffsetY = popRect.GetCenterY();
		
		RT_BgScn->SetEnable(true);
		RT_BgScn->SetScale(1.0f);
		RT_BgScn->SetXY(0,0);*/

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
		GFInstance->SetPopupTween( (int)(centerX-683) ,(int)(centerY-384 ),171,0);
		
		if(Pop_States[ActivePopupID] == HOPOP_SCALEUP)
			GFInstance->CreateGreyscaleScene();
		
		SetZ(eZOrder_Popup+10);
}

void L1_S5_HOTreasureRoom::SceneObjectsUpdate()
{
	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] >= HOITEM_TAKEN)
		SCN_locketOnScene->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] >= HOITEM_USED)
	{
		SCN_handClosed->SetEnable(false);		
		SCN_statueHandOpen->SetEnable(true);
		SCN_locket->SetEnable(true);
		SCN_eagleEyeInHand->SetEnable(true);
	}
	else
	{
		SCN_statueHandOpen->SetEnable(false);
		SCN_locket->SetEnable(false);
		SCN_eagleEyeInHand->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] >= HOITEM_TAKEN)
		SCN_bookLockPart1->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] >= HOITEM_USED)
	{
		SCN_bookLockPartFixed1->SetEnable(true);
	}
	else
	{
		SCN_bookLockPartFixed1->SetEnable(false);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] >= HOITEM_TAKEN)
		SCN_eagleEyeInHand->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] >= HOITEM_USED)
	{
		SCN_eagleMouthOpen->SetEnable(true);
		SCN_eagleEye->SetEnable(true);
	}
	else
	{
		SCN_eagleMouthOpen->SetEnable(false);
		SCN_eagleEye->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] >= HOITEM_USED)
	{
		SCN_clockOpen->SetEnable(true);
		SCN_handle->SetEnable(true);
	}
	else
	{
		SCN_clockOpen->SetEnable(false);
		SCN_handle->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] >= HOITEM_TAKEN)
		SCN_handle->SetEnable(false);
	
	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] >= HOITEM_USED)
	{
		SCN_musicboxOpen->SetEnable(true);
	}
	else
	{
		SCN_musicboxOpen->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] >= HOITEM_USED)
	{
		SCN_ringBoxCoin->SetEnable(true);
		SCN_ringBoxOpen->SetEnable(true);
	}
	else
	{
		SCN_ringBoxCoin->SetEnable(false);
		SCN_ringBoxOpen->SetEnable(false);
		SCN_keyOnBox->SetEnable(false);
	}	

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] >= HOITEM_USED)
	{
		SCN_bookLockPartFixed4->SetEnable(true);
	}
	else
	{
		SCN_bookLockPartFixed4->SetEnable(false);
	}	

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] >= HOITEM_USED)
	{
		SCN_grinderWithPestile->SetEnable(true);
		SCN_grinderOnly->SetEnable(false);
	}
	else
	{
		SCN_grinderWithPestile->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] >= HOITEM_USED)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOChestBoxOpened)
			SCN_chestBoxSymbol->SetEnable(true);
	}
	else
	{
		SCN_chestBoxSymbol->SetEnable(false);
		
	}	

	
	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOClothFolded)
		SCN_clothOpen->SetEnable(false); 
	else
		SCN_clothFolded->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart2Fixed)
	{
		SCN_bookLockPart3->SetEnable(false);
		SCN_bookLockPartFixed3->SetEnable(true);
	}
	else
		SCN_bookLockPartFixed3->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart3Fixed)
	{
		SCN_bookLockPart2->SetEnable(false);
		SCN_bookLockPartFixed2->SetEnable(true);
	}
	else
		SCN_bookLockPartFixed2->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened)
	{
		SCN_bookOpen->SetEnable(false);
		SCN_ringBox->SetEnable(false);
		SCN_ringBoxCoin->SetEnable(false);
		SCN_ringBoxOpen->SetEnable(false);
		SCN_keyOnBox->SetEnable(false);
	}
	else
	{
		SCN_bookOpen->SetEnable(true);
		SCN_ringBox->SetEnable(true);
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] >= HOITEM_TAKEN)
			SCN_ringBoxCoin->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOChestBoxOpened)
	{
		SCN_chestBoxOpen->SetEnable(false);
		SCN_pestle->SetEnable(false);
	}
	else
	{
		SCN_chestBoxOpen->SetEnable(true);
		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] >= HOITEM_TAKEN)
			SCN_pestle->SetEnable(false);
		else
			SCN_pestle->SetEnable(true);

		
	}
}

float L1_S5_HOTreasureRoom::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void L1_S5_HOTreasureRoom::UpdatePopupTransition()
{
	//if(!GFInstance->IsScnBg1024Res)
	//{
	//	//1366 resolution
	//	RT_BgScn->SetScale(1.0f + PopupAlpha * 0.25f);
	//	RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));
	//}
	//else
	//{
	//	//1024 resolution
	//	RT_BgScn->SetScale(1.0f + PopupAlpha * 0.25f);
	//	RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) +171.0f ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));
	//}

	switch( Pop_States[ActivePopupID] )
	{
	case HOPOP_SCALEUP:
		PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
		PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

		if( PopVal >= 1.0f )
		{
			IsPopupTransitionActive = false;

			PopupAlpha = 1.0f;

			Pop_States[ActivePopupID] = HOPOP_OPEN;
			Pop_SCNArray[ActivePopupID]->Show(true);
		}
		GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);
		break;

	case HOPOP_SCALEDOWN:
		popupCloseDelayTimer -= 1.0f * Control::LogicRate;
		if( popupCloseDelayTimer > 0.0f ){
			return;	}
		else{
			popupCloseDelayTimer = 0.0;	}
			
		PopupAlpha -= POPUP_SCALEDOWN_FACTOR*Control::LogicRate;
		if( PopupAlpha <= 0.0f )
		{
			IsPopupTransitionActive = false;

			HOScene->PauseObjects(false);

			PopupAlpha = 0.0f;
			GFInstance->TurnOffBlurScene();

			Pop_States[ActivePopupID] = HOPOP_CLOSED;


			PopupClosePostUpdate(ActivePopupID);

			ActivePopupID = eHOPop_Inactive;
			//EndBgGraphicScaling();
			//SetZ(zOrderScnVideo);
		}
		else
			GFInstance->UpdateBlurScene(PopupAlpha);
		break;
	}
}

void L1_S5_HOTreasureRoom::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

if(ActivePopupID != eHOPop_Inactive)
	{
		if(IsPopupTransitionActive)
		{

			//Init again if render target lost due to fullscreen-window switches or similar
			if( GFInstance->RTGraphic != NULL )
			{
				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),				
				PopupAlpha,PopupAlpha>1.0f?1.0f:PopupAlpha);

				//Gradient alpha causes alpha on transition, leaking the bg scene through popups during transition
				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),
				PopupAlpha,PopupAlpha>1.0f?1.0f:PopupAlpha);
  
            }
		}
	}
}

L1_S5_HOTreasureRoom::~L1_S5_HOTreasureRoom()
{
	Control::Audio->Samples[aTrackL1Trapped]->SetVolume((float)Control::Audio->GlobalMusicVol);
	Hud->InvBoxCtrl->ResetToInvBox();
	Hud->IsTakeAndUseHOScene = false;

	Hud->AllowInput = true;
	Hud->ShowHud(false);

	for (int i = 0; i < eHOPop_Max; i++)
	{
		SAFE_DELETE(Pop_SCNArray[i]);
		SAFE_DELETE(Pop_ScnPolyArray[i]);
	}

	for (int i = 0; i < eHOItem_MAX; i++)
	{
		SAFE_DELETE(DestRect_HOItemArray[i]);
		SAFE_DELETE(ItemRect_HOItemArray[i]);
	}

	SAFE_DELETE(p1Vfx_BookOpen);

	SAFE_DELETE(p2Vfx_PuzEndFX);

	SAFE_DELETE(p2HelpFrameBG);
	SAFE_DELETE(p2Helptext);

	SAFE_DELETE(p4Vfx_locketfix1);
	SAFE_DELETE(p4Vfx_locketfix2);

	SAFE_DELETE(CAnim_LocketFix);

	SAFE_DELETE(CAnim_ClockNeedleFix);
	SAFE_DELETE(CAnim_P6ShelfOpen);

	SAFE_DELETE(vfx_goldGlit);
	SAFE_DELETE(vfx_flies);
	SAFE_DELETE(vfx_glares);

	SAFE_DELETE(vfx_collectFX);

	SAFE_DELETE(ResetDia);

	//SAFE_DELETE(DebugText);
	SAFE_DELETE(HOInvBox);
		
	SAFE_DELETE(skipObj);
	SAFE_DELETE(cv_LasyaAttack);

	SAFE_DELETE(subtitleText);

	//SAFE_DELETE(skipText);
	
	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(Treasurematch3 != NULL)
		{
			Treasurematch3->clearBoard();
			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			isMatch3Playing = false;
		}
	}

	//MUSIC_SFX
	Control::Audio->StopSample(aAmbS4PuzShadowhand);
	Control::Audio->UnloadSample(aAmbS4PuzShadowhand);
	
	//MUSIC_SFX
}

void L1_S5_HOTreasureRoom::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
		case eInv_L2S1_ColorPowder:
		break;
	
	default:
		break;
	}
}

void L1_S5_HOTreasureRoom::ReceiveMessage(int val)
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
		
		

		if(ProfileSystem::Instance->ActiveProfile.L1S5_HOSolved)
		{
			Hud->AllowInput = false;
			Hud->HideHud(false);

			isLasyaAttackVidPlaying = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
			cv_LasyaAttack->PlayFadeInVideo(1.5f);
		
			if( Control::Audio->IsPlaying(aTrackL1Trapped) )
				Control::Audio->StopSample(aTrackL1Trapped);
			if( Control::Audio->IsPlaying(aAmbS4PuzShadowhand) )
				Control::Audio->StopSample(aAmbS4PuzShadowhand);
			if( Control::Audio->IsPlaying(aAmbS5TreasureRoom) )
				Control::Audio->StopSample(aAmbS5TreasureRoom);
			//Control::Audio->PlaySample(aTrackCutscnS5MayaAttack,false);
			IsVidSkippable = true;
			SkipShowTimer = 0;
			
			//To save if player exits during cutscene
			ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttackVideoPlayed = true;
		}
	}
}

void L1_S5_HOTreasureRoom::ProcessNonHOHint()
{
	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		return;

	AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	int iNonHOHint = 0; 
	int MaxNonHOHints = 45;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates

		case 0: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] == HOITEM_TAKEN && Pop_States[eHOPop4_StatueHanuman] == HOPOP_OPEN)
				{
					InitHintTrailEmit(DestRect_HOItemArray[eHOItem1_NeckLocket]->GetCenterX() ,DestRect_HOItemArray[eHOItem1_NeckLocket]->GetCenterY());
					CPoint tempPoint (HOInvBox->GetInventoryItem(eHOItem1_NeckLocket)->GetX(),HOInvBox->GetInventoryItem(eHOItem1_NeckLocket)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 1: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] >= HOITEM_USED && Pop_States[eHOPop4_StatueHanuman] == HOPOP_OPEN)
				{
					InitHintTrailEmit(ItemRect_HOItemArray[eHOItem3_EagleEye]->GetCenterX() ,ItemRect_HOItemArray[eHOItem3_EagleEye]->GetCenterY());
					return;
				}
				break;
		case 2: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] == HOITEM_TAKEN && Pop_States[eHOPop5_Eagle] == HOPOP_OPEN)
				{
					InitHintTrailEmit(DestRect_HOItemArray[eHOItem3_EagleEye]->GetCenterX() ,DestRect_HOItemArray[eHOItem3_EagleEye]->GetCenterY());
					CPoint tempPoint (HOInvBox->GetInventoryItem(eHOItem3_EagleEye)->GetX(),HOInvBox->GetInventoryItem(eHOItem3_EagleEye)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 3: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] >= HOITEM_USED && Pop_States[eHOPop5_Eagle] == HOPOP_OPEN)
				{
					InitHintTrailEmit(ItemRect_HOItemArray[eHOItem4_Clockneedle]->GetCenterX() ,ItemRect_HOItemArray[eHOItem4_Clockneedle]->GetCenterY());
					return;
				}
				break;
		case 4: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] == HOITEM_TAKEN && Pop_States[eHOPop3_Clock] == HOPOP_OPEN)
				{
					InitHintTrailEmit(DestRect_HOItemArray[eHOItem4_Clockneedle]->GetCenterX() ,DestRect_HOItemArray[eHOItem4_Clockneedle]->GetCenterY());
					CPoint tempPoint (HOInvBox->GetInventoryItem(eHOItem4_Clockneedle)->GetX(),HOInvBox->GetInventoryItem(eHOItem4_Clockneedle)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 5: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] >= HOITEM_USED && Pop_States[eHOPop3_Clock] == HOPOP_OPEN)
				{
					InitHintTrailEmit(ItemRect_HOItemArray[eHOItem5_DrawerHandle]->GetCenterX() ,ItemRect_HOItemArray[eHOItem5_DrawerHandle]->GetCenterY());
					return;
				}
				break;
		case 6: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved && Pop_States[eHOPop6_ShelfSwapPuzzle] == HOPOP_OPEN)
				{
					Hud->ShowBannerText("L1tile");
					return;
				}
				break;
		case 7: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] == HOITEM_TAKEN && ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved && Pop_States[eHOPop6_ShelfSwapPuzzle] == HOPOP_OPEN)
				{
					InitHintTrailEmit(DestRect_HOItemArray[eHOItem5_DrawerHandle]->GetCenterX() ,DestRect_HOItemArray[eHOItem5_DrawerHandle]->GetCenterY());
					CPoint tempPoint (HOInvBox->GetInventoryItem(eHOItem5_DrawerHandle)->GetX(),HOInvBox->GetInventoryItem(eHOItem5_DrawerHandle)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 8: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] >= HOITEM_USED && Pop_States[eHOPop6_ShelfSwapPuzzle] == HOPOP_OPEN)
				{
					InitHintTrailEmit(ItemRect_HOItemArray[eHOItem6_KeyRingBox]->GetCenterX() ,ItemRect_HOItemArray[eHOItem6_KeyRingBox]->GetCenterY());
					return;
				}
				break;
		case 9: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] >= HOITEM_USED && Pop_States[eHOPop6_ShelfSwapPuzzle] == HOPOP_OPEN)
				{
					InitHintTrailEmit(ItemRect_HOItemArray[eHOItem7_BookSymbolPartHead]->GetCenterX() ,ItemRect_HOItemArray[eHOItem7_BookSymbolPartHead]->GetCenterY());
					return;
				}
				break;
		case 10: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart2Fixed && Pop_States[eHOPop1_BookLocketPop] == HOPOP_OPEN)
				{
					InitHintTrailEmit(P1bookLockPart1->Pos.x ,P1bookLockPart1->Pos.y);
					CPoint tempPoint (P1bookLockPartFixed1->Pos.x ,P1bookLockPartFixed1->Pos.y,0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 11: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart3Fixed && Pop_States[eHOPop1_BookLocketPop] == HOPOP_OPEN)
				{
					InitHintTrailEmit(P1bookLockPart2->Pos.x ,P1bookLockPart2->Pos.y);
					CPoint tempPoint (P1bookLockPartFixed2->Pos.x ,P1bookLockPartFixed2->Pos.y,0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 12: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] == HOITEM_TAKEN && Pop_States[eHOPop1_BookLocketPop] == HOPOP_OPEN)
				{
					InitHintTrailEmit(DestRect_HOItemArray[eHOItem2_BookSymbolPartHorn]->GetCenterX() ,DestRect_HOItemArray[eHOItem2_BookSymbolPartHorn]->GetCenterY());
					CPoint tempPoint (HOInvBox->GetInventoryItem(eHOItem2_BookSymbolPartHorn)->GetX(),HOInvBox->GetInventoryItem(eHOItem2_BookSymbolPartHorn)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 13: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] == HOITEM_TAKEN && Pop_States[eHOPop1_BookLocketPop] == HOPOP_OPEN)
				{
					InitHintTrailEmit(DestRect_HOItemArray[eHOItem7_BookSymbolPartHead]->GetCenterX() ,DestRect_HOItemArray[eHOItem7_BookSymbolPartHead]->GetCenterY());
					CPoint tempPoint (HOInvBox->GetInventoryItem(eHOItem7_BookSymbolPartHead)->GetX(),HOInvBox->GetInventoryItem(eHOItem7_BookSymbolPartHead)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 14: ++iNonHOHint;
			/*if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOHammerMoved && ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened && Pop_States[eHOPop1_BookLocketPop] == HOPOP_OPEN)
				{
					InitHintTrailEmit(ActionRectP1HammerHead.topleft.x + ActionRectP1HammerHead.w/2 ,ActionRectP1HammerHead.topleft.y + ActionRectP1HammerHead.h/2);
					return;
				}*/
				break;
		case 15: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened && ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] == HOITEM_TAKEN && Pop_States[eHOPop1_BookLocketPop] == HOPOP_OPEN)
				{
					InitHintTrailEmit(DestRect_HOItemArray[eHOItem6_KeyRingBox]->GetCenterX() ,DestRect_HOItemArray[eHOItem6_KeyRingBox]->GetCenterY());
					CPoint tempPoint (HOInvBox->GetInventoryItem(eHOItem6_KeyRingBox)->GetX(),HOInvBox->GetInventoryItem(eHOItem6_KeyRingBox)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 16: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] >= HOITEM_USED && Pop_States[eHOPop1_BookLocketPop] == HOPOP_OPEN)
				{
					InitHintTrailEmit(ItemRect_HOItemArray[eHOItem8_TokenBoxPuzzle]->GetCenterX() ,ItemRect_HOItemArray[eHOItem8_TokenBoxPuzzle]->GetCenterY());
					return;
				}
				break;
		case 17: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] == HOITEM_TAKEN && Pop_States[eHOPop2_ColorSlidePuzzleBox] == HOPOP_OPEN)
				{
					InitHintTrailEmit(DestRect_HOItemArray[eHOItem8_TokenBoxPuzzle]->GetCenterX() ,DestRect_HOItemArray[eHOItem8_TokenBoxPuzzle]->GetCenterY());
					CPoint tempPoint (HOInvBox->GetInventoryItem(eHOItem8_TokenBoxPuzzle)->GetX(),HOInvBox->GetInventoryItem(eHOItem8_TokenBoxPuzzle)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 18: ++iNonHOHint;
			if(! ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleSolved &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] >= HOITEM_USED&& Pop_States[eHOPop2_ColorSlidePuzzleBox] == HOPOP_OPEN)
				{
					Hud->ShowHintForSceneItem( &p2HelpBtn->Pos);
					Hud->ShowBannerText("L1open");
					return;
				}
				break;
		case 19: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleSolved && Pop_States[eHOPop2_ColorSlidePuzzleBox] == HOPOP_OPEN)
				{
					InitHintTrailEmit(ItemRect_HOItemArray[eHOItem9_Pestle]->GetCenterX() ,ItemRect_HOItemArray[eHOItem9_Pestle]->GetCenterY());
					return;
				}
				break;

		case 20: ++iNonHOHint;
			for (int i = 0; i < eHOPop_Max; i++)
			{
				if(Pop_States[i] == HOPOP_OPEN) //if popup is active
				{
					InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
					return;
				}

			}
			break;
		case 21: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] < HOITEM_TAKEN )
				{
					InitHintTrailEmit(InvRectLocket.topleft.x + InvRectLocket.w/2 ,InvRectLocket.topleft.y + InvRectLocket.h/2);
					return;
				}
				break;
		case 22: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] == HOITEM_TAKEN )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop4_StatueHanuman]->center.x ,Pop_ScnPolyArray[eHOPop4_StatueHanuman]->center.y);
					return;
				}
				break;
		case 23: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem1_NeckLocket] >= HOITEM_USED)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop4_StatueHanuman]->center.x ,Pop_ScnPolyArray[eHOPop4_StatueHanuman]->center.y);
					return;
				}
				break;
		case 24: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] == HOITEM_TAKEN )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop5_Eagle]->center.x ,Pop_ScnPolyArray[eHOPop5_Eagle]->center.y);
					return;
				}
				break;
		case 25: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem3_EagleEye] >= HOITEM_USED)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop5_Eagle]->center.x ,Pop_ScnPolyArray[eHOPop5_Eagle]->center.y);
					return;
				}
				break;
		case 26: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] == HOITEM_TAKEN )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop3_Clock]->center.x ,Pop_ScnPolyArray[eHOPop3_Clock]->center.y);
					return;
				}
				break;
		case 27: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem4_Clockneedle] >= HOITEM_USED)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop3_Clock]->center.x ,Pop_ScnPolyArray[eHOPop3_Clock]->center.y);
					return;
				}
				break;
		case 28: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle]->center.x ,Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle]->center.y);
					return;
				}
				break;
		case 29: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] == HOITEM_TAKEN && ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle]->center.x ,Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle]->center.y);
					return;
				}
				break;
		case 30: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] >= HOITEM_USED)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle]->center.x ,Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle]->center.y);
					return;
				}
				break;
		case 31: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem5_DrawerHandle] >= HOITEM_USED)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle]->center.x ,Pop_ScnPolyArray[eHOPop6_ShelfSwapPuzzle]->center.y);
					return;
				}
				break;
		case 32: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] < HOITEM_TAKEN )
				{
					InitHintTrailEmit(InvRectBookPart1.topleft.x + InvRectBookPart1.w/2 ,InvRectBookPart1.topleft.y + InvRectBookPart1.h/2);
					return;
				}
				break;
		case 33: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart2Fixed )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.x ,Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.y);
					return;
				}
				break;
		case 34: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart3Fixed )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.x ,Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.y);
					return;
				}
				break;
		case 35: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem2_BookSymbolPartHorn] == HOITEM_TAKEN )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.x ,Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.y);
					return;
				}
				break;
		case 36: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem7_BookSymbolPartHead] == HOITEM_TAKEN )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.x ,Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.y);
					return;
				}
				break;
		case 37: ++iNonHOHint;
			/*if(!ProfileSystem::Instance->ActiveProfile.L1S5_HOHammerMoved && ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.x ,Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.y);
					return;
				}*/
				break;
		case 38: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] == HOITEM_TAKEN && ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.x ,Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.y);
					return;
				}
				break;
		case 39: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem6_KeyRingBox] >= HOITEM_USED)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.x ,Pop_ScnPolyArray[eHOPop1_BookLocketPop]->center.y);
					return;
				}
				break;
		case 40: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] == HOITEM_TAKEN )
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop2_ColorSlidePuzzleBox]->center.x ,Pop_ScnPolyArray[eHOPop2_ColorSlidePuzzleBox]->center.y);
					return;
				}
				break;
		case 41: ++iNonHOHint;
			if(! ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleSolved &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem8_TokenBoxPuzzle] >= HOITEM_USED)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop2_ColorSlidePuzzleBox]->center.x ,Pop_ScnPolyArray[eHOPop2_ColorSlidePuzzleBox]->center.y);
					return;
				}
				break;
		case 42: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] < HOITEM_TAKEN &&  ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleSolved)
				{
					InitHintTrailEmit(Pop_ScnPolyArray[eHOPop2_ColorSlidePuzzleBox]->center.x ,Pop_ScnPolyArray[eHOPop2_ColorSlidePuzzleBox]->center.y);
					return;
				}
				break;
		case 43: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[eHOItem9_Pestle] == HOITEM_TAKEN )
				{
					InitHintTrailEmit(DestRect_HOItemArray[eHOItem9_Pestle]->GetCenterX() ,DestRect_HOItemArray[eHOItem9_Pestle]->GetCenterY());
					CPoint tempPoint (HOInvBox->GetInventoryItem(eHOItem9_Pestle)->GetX(),HOInvBox->GetInventoryItem(eHOItem9_Pestle)->GetY(),0);
					Hud->Hint2FX->ResetPos(&tempPoint);
					Hud->Hint2FX->Emit();
					return;
				}
				break;
		case 44: ++iNonHOHint;
				{
					InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
					return;
				}
				break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
	
}

void L1_S5_HOTreasureRoom::ResetHO()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if( ActivePopupID != eHOPop_Inactive )
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
			ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken = false;		
			ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttackVideoPlayed = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttckBanner = false;

			for (int i = 0; i < MAX_L1S5_HOITEMS; i++)
				ProfileSystem::Instance->ActiveProfile.L1S5_HOItemStates[i]=0;
			for (int i = 0; i < MAX_HOITEMS; i++)
				ProfileSystem::Instance->ActiveProfile.L1S5_HOHUDItemStates[i]=0;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleShuffled = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOP2PuzzleSolved = false;
			for (int i = 0; i < MAX_L1S5_HOP2TOKENS; i++)
				ProfileSystem::Instance->ActiveProfile.L1S5_HOP2TokenPosContains[i]=0;

			ProfileSystem::Instance->ActiveProfile.L1S5_HOClothFolded = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart2Fixed = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOBookPart3Fixed = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOBookOpened = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOHammerMoved = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOChestBoxOpened = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOSwapPuzzleSolved = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOSolved = false;
			ProfileSystem::Instance->ActiveProfile.L1S5_HOCompletedWithMatch3 = false;
		
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_HOTreasureRoom;
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
void L1_S5_HOTreasureRoom::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
				{
					if(Treasurematch3)
						Treasurematch3->clearBoard();
					BtnMatch3->Show(false);
					BtnSwitchToHO->Show(false);
					ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
					SendMessage(ControlExit, ID, HOSM_NewArea);
				} else {
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = GFHud->InvBoxCtrl->travelArea;
					SendMessage(ControlExit, ID, HOSM_NewArea);
				}
			}

			GFHud->InvBoxCtrl->NavigateText->Text = "";
			GFHud->InvBoxCtrl->travelArea = -1;
			GFHud->InvBoxCtrl->navTapped = false;
			GFHud->InvBoxCtrl->Navigate->Show(false);
			GFHud->InvBoxCtrl->NavArrow->Show(false);
		}
	}
}