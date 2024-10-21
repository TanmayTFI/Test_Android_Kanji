//====================================
//DQFULP
// Extras Menu
// Hidden Object System
//====================================
#ifdef K_WIN32
	#include <shlobj.h>
	#pragma comment(lib, "shell32.lib")
#endif
#include <string.h>

#include "Application.h"
#include "ExtrasMenuControl.h"
#include "HOScene.h"

#include "GameFlow.h"
#include "MenuSystem.h"
#include "Resources.h"

#include "Fonts.h"
#include "Util.h"

#include "AssetsStringTable.h"


const CRectangle  ComicBookRect(600,550,550, 150);//(205.0f,544.0f,956.0f,151.0f);335, 150
const CRectangle  CreditsRect(205, 544, 335, 150);
const CRectangle  ZoomedRect(171.0f,77.0f,1016.0f,571.0f);

std::string wallpaperPath ="MNU\\EXTRAS\\WALLZOOM\\";
std::string conceptsPath ="MNU\\EXTRAS\\CONCEPTS\\";

const CRectangle  MusicBtn_PlayTrack(566.0f,514.0f,104.0f,107.0f);
const CRectangle  MusicBtn_StopTrack(677.0f,528.0f,85.0f,82.0f);
const CRectangle  MusicBtn_NextTrack(771.0f,529.0f,83.0f,80.0f);
const CRectangle  MusicBtn_PrevTrack(474.0f,527.0f,84.0f,85.0f);

const int MusicTrackIDs[MAX_MUSIC_TRACKS] ={
	aTrackL1Mansion,
	aTrackL1Broken,
	aTrackL2theme1,
	aTrackL3theme,
	aTrackL4theme,
	aTrackL5theme,

	aTrackL6theme,
	aTrackL7theme,
	aTrackL8theme,
	aTrackMenu,
	aTrackKidRoom,
	aTrackL1KidsPuzzleBGM
};

const char* MusicTracks[MAX_MUSIC_TRACKS] ={
	"HOME_THAT_LOVE_BUILT.OGG",
	"THE_BROKEN_HEART.OGG",
	"FORBIDDEN_FORGOTTEN.OGG",
	"NAGA_ASURA_GATEWAY.OGG",
	"ATHARWA_SWAMPS.OGG",
	"SUNKEN_TEMPLE_OF_DEATH.OGG",
	"NETHERWORLD.OGG",
	"MOHENJO_DARO.OGG",
	"RISING_OF_THE_PHOENIX.OGG",
	"THE_LOST_LULLABY.OGG",
	"EVER_MOMMA'S_LIL_PRINCESS.OGG",
	"WILL_YOU_SAVE_ME_MAMMA.OGG"
};

ExtrasMenuControl::ExtrasMenuControl(CObject* maskSpr)
{	
	isBackButtonSelected =false;
	IsExtrasItemSelected = false;
	ExtrasPageSize = 0;
	LastSelectedExtrasBtn = -1;
	pageIndex	= 0;
	PopupAlpha = 0;
	PopupCounter = 0;

	isComicBookClicked = false;

	_maskSpr = maskSpr;

	_kidroomAreaCallback = nullptr;
	_AwardsAreaCallback = nullptr;
	_CreditsCallback = nullptr;

	_HOPuzzleLoadCallback = nullptr;
	_showMessageBannerCallback = nullptr;

	PopupState = Popup_Disabled;

	ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId = -1;

	//__EXTRASCENEPOPUPS
	ExtrasPopupScene = new CHOScene();
	ExtrasPopupScene->Load("MNU\\EXTRAS\\EXTRASMENU.SCN");
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\EXTRAS.SPR", ExtrasPopupScene->GetObjectByID("Title")->Pos.x, ExtrasPopupScene->GetObjectByID("Title")->Pos.y, ExtrasPopupScene->GetObjectByID("Title")->ZOrder + 1);
	temp->SetUID("titletext"); 
	ExtrasPopupScene->PushObject(temp);

	comicBtn = new CObject();
//	comicBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\COMIC.SPR", 80, 230, ExtrasPopupScene->GetObjectByID("Title")->ZOrder + 10);
//	comicBtn->SetUID("COMICBOOKTXT");
//	comicBtn->SetScale(1);
//	ExtrasPopupScene->PushObject(comicBtn);

//	creditsBtn = new CObject();
//	creditsBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\CREDITS.SPR", -350, 235, ExtrasPopupScene->GetObjectByID("Title")->ZOrder + 10);
//	creditsBtn->SetUID("creditsBtn");
//	creditsBtn->SetScale(1);
//	ExtrasPopupScene->PushObject(creditsBtn);

	/*
	temp = new CObject();
	temp->LoadInit("MNU\\EXTRAS\\CREDITS.SPR", 100, 100, ExtrasPopupScene->GetObjectByID("Title")->ZOrder + 10);
	temp->SetUID("creditsBtn");
	temp->SetScale(0.5);
	ExtrasPopupScene->PushObject(temp);
	*/

	ExtrasPopupScene->SetPosOff(683,384);
	ExtrasPopupScene->SetZOff(1000);
	ExtrasPopupScene->Show(false);

	//PopupState = Popup_Extras;

	achievementsTxt = new CBitmapText();
	achievementsTxt->SetZ(1100);
	achievementsTxt->SetAlignment(Align_Center);
	achievementsTxt->SetPos(PointSystem::CreateCPoint(945, 525, 0.0f));
	achievementsTxt->LoadFont(eFont_25);
	achievementsTxt->SetText(AssetsStringTable::getString("achievements"));
	achievementsTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 	
	achievementsTxt->Show(false);

	minigamesTxt = new CBitmapText();
	minigamesTxt->SetZ(1100);
	minigamesTxt->SetAlignment(Align_Center);
	minigamesTxt->SetPos(PointSystem::CreateCPoint(775, 340, 0.0f));
	minigamesTxt->LoadFont(eFont_25);
	minigamesTxt->SetText(AssetsStringTable::getString("minigames"));
	minigamesTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 	
	minigamesTxt->Show(false);

	memoriesTxt = new CBitmapText();
	memoriesTxt->SetZ(1100);
	memoriesTxt->SetAlignment(Align_Center);
	memoriesTxt->SetPos(PointSystem::CreateCPoint(945, 340, 0.0f));
	memoriesTxt->LoadFont(eFont_25);
	memoriesTxt->SetText(AssetsStringTable::getString("memories"));
	memoriesTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 	
	memoriesTxt->Show(false);

	conceptsTxt = new CBitmapText();
	conceptsTxt->SetZ(1100);
	conceptsTxt->SetAlignment(Align_Center);
	conceptsTxt->SetPos(PointSystem::CreateCPoint(385, 340, 0.0f));
	conceptsTxt->LoadFont(eFont_25);
	conceptsTxt->SetText(AssetsStringTable::getString("concepts"));
	conceptsTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 	
	conceptsTxt->Show(false);

	cutscenesTxt = new CBitmapText();
	cutscenesTxt->SetZ(1100);
	cutscenesTxt->SetAlignment(Align_Center);
	cutscenesTxt->SetPos(PointSystem::CreateCPoint(575, 340, 0.0f));
	cutscenesTxt->LoadFont(eFont_25);
	cutscenesTxt->SetText(AssetsStringTable::getString("cutscenes"));
	cutscenesTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 	
	cutscenesTxt->Show(false);

	musicTxt = new CBitmapText();
	musicTxt->SetZ(1100);
	musicTxt->SetAlignment(Align_Center);
	musicTxt->SetPos(PointSystem::CreateCPoint(775, 525, 0.0f));
	musicTxt->LoadFont(eFont_25);
	musicTxt->SetText(AssetsStringTable::getString("music"));
	musicTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 	
	musicTxt->Show(false);

	hopuzzlesTxt = new CBitmapText();
	hopuzzlesTxt->SetZ(1100);
	hopuzzlesTxt->SetAlignment(Align_Center);
	hopuzzlesTxt->SetPos(PointSystem::CreateCPoint(385, 525, 0.0f));
	hopuzzlesTxt->LoadFont(eFont_25);
	hopuzzlesTxt->SetText(AssetsStringTable::getString("hopuzzles"));
	hopuzzlesTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 	
	hopuzzlesTxt->Show(false);

#ifndef TOUCH_UI
	wallpapersTxt = new CBitmapText();
	wallpapersTxt->SetZ(1100);
	wallpapersTxt->SetAlignment(Align_Center);
	wallpapersTxt->SetPos(PointSystem::CreateCPoint(575, 525, 0.0f));
	wallpapersTxt->LoadFont(eFont_25);
	wallpapersTxt->SetText(AssetsStringTable::getString("wallpapers"));
	wallpapersTxt->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f)); //gold 	
	wallpapersTxt->Show(false);
#endif

	SCN_CutScenes = new CHOScene();
	SCN_CutScenes->Load("MNU\\EXTRAS\\CUTSCENES.SCN");
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\CUTSCENES.SPR", SCN_CutScenes->GetObjectByID("Titles")->Pos.x, SCN_CutScenes->GetObjectByID("Titles")->Pos.y, SCN_CutScenes->GetObjectByID("Titles")->ZOrder + 1);
	temp->SetUID("titletext");
	SCN_CutScenes->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", SCN_CutScenes->GetObjectByID("BackButton")->Pos.x-15, SCN_CutScenes->GetObjectByID("BackButton")->Pos.y-2, SCN_CutScenes->GetObjectByID("BackButton")->ZOrder + 1);
	temp->SetUID("BackButtonText");
	SCN_CutScenes->PushObject(temp);
	SCN_CutScenes->SetPosOff(683,350);
	SCN_CutScenes->SetZOff(1005);
	SCN_CutScenes->Show(false);
	
	
	SCN_Concepts = new CHOScene();
	SCN_Concepts->Load("MNU\\EXTRAS\\CONCEPTART.SCN"); 
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\CONCEPTART.SPR", SCN_Concepts->GetObjectByID("Titles")->Pos.x, SCN_Concepts->GetObjectByID("Titles")->Pos.y, SCN_Concepts->GetObjectByID("Titles")->ZOrder + 1);
	temp->SetUID("titletext");
	SCN_Concepts->PushObject(temp); 
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", SCN_Concepts->GetObjectByID("BackButton")->Pos.x - 15, SCN_Concepts->GetObjectByID("BackButton")->Pos.y - 2, SCN_Concepts->GetObjectByID("BackButton")->ZOrder + 1);
	temp->SetUID("BackButtonText");
	SCN_Concepts->PushObject(temp);
	SCN_Concepts->SetPosOff(683,350);
	SCN_Concepts->SetZOff(1005);
	SCN_Concepts->Show(false);

	SCN_HOScenes = new CHOScene();
	SCN_HOScenes->Load("MNU\\EXTRAS\\HOSCENES.SCN");
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\HO-PUZZLE.SPR", SCN_HOScenes->GetObjectByID("Titles")->Pos.x, SCN_HOScenes->GetObjectByID("Titles")->Pos.y, SCN_HOScenes->GetObjectByID("Titles")->ZOrder + 1);
	temp->SetUID("titletext");
	SCN_HOScenes->PushObject(temp); 
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", SCN_HOScenes->GetObjectByID("BackButton")->Pos.x - 15, SCN_HOScenes->GetObjectByID("BackButton")->Pos.y - 2, SCN_HOScenes->GetObjectByID("BackButton")->ZOrder + 1);
	temp->SetUID("BackButtonText");
	SCN_HOScenes->PushObject(temp);
	SCN_HOScenes->SetPosOff(683,350);
	SCN_HOScenes->SetZOff(1005);
	SCN_HOScenes->Show(false);

	SCN_MiniGames = new CHOScene();
	SCN_MiniGames->Load("MNU\\EXTRAS\\MINIGAMES.SCN");
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\MINI-GAMES.SPR", SCN_MiniGames->GetObjectByID("Titles")->Pos.x, SCN_MiniGames->GetObjectByID("Titles")->Pos.y, SCN_MiniGames->GetObjectByID("Titles")->ZOrder + 1);
	temp->SetUID("titletext");
	SCN_MiniGames->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", SCN_MiniGames->GetObjectByID("BackButton")->Pos.x - 15, SCN_MiniGames->GetObjectByID("BackButton")->Pos.y - 2, SCN_MiniGames->GetObjectByID("BackButton")->ZOrder + 1);
	temp->SetUID("BackButtonText");
	SCN_MiniGames->PushObject(temp);
	SCN_MiniGames->SetPosOff(683,350);
	SCN_MiniGames->SetZOff(1005);
	SCN_MiniGames->Show(false);

	SCN_Wallpapaers = new CHOScene();
	SCN_Wallpapaers ->Load("MNU\\EXTRAS\\WALLPAPERLAYOUT.SCN");
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\WALLPAPERS.SPR", SCN_Wallpapaers->GetObjectByID("Titles")->Pos.x, SCN_Wallpapaers->GetObjectByID("Titles")->Pos.y, SCN_Wallpapaers->GetObjectByID("Titles")->ZOrder + 1);
	temp->SetUID("titletext");
	SCN_Wallpapaers->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", SCN_Wallpapaers->GetObjectByID("BackButton")->Pos.x - 15, SCN_Wallpapaers->GetObjectByID("BackButton")->Pos.y - 2, SCN_Wallpapaers->GetObjectByID("BackButton")->ZOrder + 1);
	temp->SetUID("BackButtonText");
	SCN_Wallpapaers->PushObject(temp);
	SCN_Wallpapaers ->SetPosOff(683,350);
	SCN_Wallpapaers ->SetZOff(1005);
	SCN_Wallpapaers ->Show(false);

	SCN_Music = new CHOScene();
	SCN_Music->Load("MNU\\EXTRAS\\MUSICLAYOUT.SCN");
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\TRACKLIST.SPR", SCN_Music->GetObjectByID("SoundsList")->Pos.x, SCN_Music->GetObjectByID("SoundsList")->Pos.y, SCN_Music->GetObjectByID("SoundsList")->ZOrder+1);
	temp->SetUID("tracklist");
	SCN_Music->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MNU\\EXTRAS\\SOUNDTRACKS.SPR", SCN_Music->GetObjectByID("Titles")->Pos.x, SCN_Music->GetObjectByID("Titles")->Pos.y, SCN_Music->GetObjectByID("Titles")->ZOrder + 1);
	temp->SetUID("titletext");
	SCN_Music->PushObject(temp);
	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", SCN_Music->GetObjectByID("BackButton")->Pos.x - 15, SCN_Music->GetObjectByID("BackButton")->Pos.y - 2, SCN_Music->GetObjectByID("BackButton")->ZOrder + 1);
	temp->SetUID("BackButtonText");
	SCN_Music->PushObject(temp);
	SCN_Music->SetPosOff(683,350);
	SCN_Music->SetZOff(1005);
	SCN_Music->Show(false);

	isExtrasMusicPlaying = false;
	currentMusicPlayingID = -1;
	selectedMusicID = -1;
	for (int i = 0; i < MAX_MUSIC_TRACKS; i++)
	{
		MusicTrackGlows[i] = SCN_Music->GetObjectByID(std::string("msctrackglow")+MKSTR(i));
		MusicTrackGlows[i]->SetBlendMode(eBlend_AlphaAdditive);
		MusicTrackGlows[i]->SetEnable(false);
	}

	float yOffset = 5.0f;
	MusicTrackRects[0] = new CRectangle(326.0f,160.0f+yOffset,200.0f,25.0f);
	MusicTrackRects[1] = new CRectangle(322.0f,195.0f+yOffset,234.0f,27.0f);
	MusicTrackRects[2] = new CRectangle(320.0f,229.0f+yOffset,250.0f,32.0f);
	MusicTrackRects[3] = new CRectangle(319.0f,268.0f+yOffset,257.0f,32.0f);
	MusicTrackRects[4] = new CRectangle(319.0f,305.0f+yOffset,256.0f,28.0f);
	MusicTrackRects[5] = new CRectangle(323.0f,344.0f+yOffset,261.0f,36.0f);
	MusicTrackRects[6] = new CRectangle(841.0f,161.0f+yOffset,184.0f,28.0f);
	MusicTrackRects[7] = new CRectangle(840.0f,196.0f+yOffset,184.0f,29.0f);
	MusicTrackRects[8] = new CRectangle(779.0f,231.0f+yOffset,249.0f,29.0f);
	MusicTrackRects[9] = new CRectangle(811.0f,268.0f+yOffset,213.0f,28.0f);
	MusicTrackRects[10]= new CRectangle(731.0f,304.0f+yOffset,293.0f,33.0f);
	MusicTrackRects[11]= new CRectangle(739.0f,343.0f+yOffset,290.0f,32.0f);
	
	CurrentPageText = new CBitmapText();
	CurrentPageText->SetZ(1100);
	CurrentPageText->SetAlignment(Align_Center);
	CurrentPageText->SetPos( PointSystem::CreateCPoint(683.0f, 580.0f, 0.0f));
	CurrentPageText->LoadFont(eFont_26);
	CurrentPageText->SetText("1/1");
	CurrentPageText->Show(false);

	ExtraItemsRect[0] = new CRectangle(331.0f,175.0f,146.0f,173.0f);
	ExtraItemsRect[1] = new CRectangle(520.0f,173.0f,146.0f,173.0f);
	ExtraItemsRect[2] = new CRectangle(708.0f,174.0f,161.0f,175.0f);
	ExtraItemsRect[3] = new CRectangle(895.0f,171.0f,149.0f,181.0f);
#ifdef TOUCH_UI
	ExtraItemsRect[4] = new CRectangle(329.0f + 83,390.0f,153.0f,179.0f);
	ExtraItemsRect[5] = new CRectangle(518.0f,385.0f,161.0f,195.0f);
	ExtraItemsRect[6] = new CRectangle(718.0f - 83,384.0f,137.0f,195.0f);
	ExtraItemsRect[7] = new CRectangle(873.0f - 83,384.0f,162.0f,188.0f);
#else
	ExtraItemsRect[4] = new CRectangle(329.0f,390.0f,153.0f,179.0f);
	ExtraItemsRect[5] = new CRectangle(518.0f,385.0f,161.0f,195.0f);
	ExtraItemsRect[6] = new CRectangle(718.0f,384.0f,137.0f,195.0f);
	ExtraItemsRect[7] = new CRectangle(873.0f,384.0f,162.0f,188.0f);
#endif

	PageItemRect[0] = new CRectangle(261.0f,146.0f,254.0f,146.0f);
	PageItemRect[1] = new CRectangle(555.0f,144.0f,252.0f,149.0f);
	PageItemRect[2] = new CRectangle(843.0f,141.0f,254.0f,149.0f);
	PageItemRect[3] = new CRectangle(261.0f,326.0f,252.0f,154.0f);
	PageItemRect[4] = new CRectangle(554.0f,330.0f,253.0f,149.0f);
	PageItemRect[5] = new CRectangle(844.0f,328.0f,253.0f,150.0f);

	#if (!defined K_ANDROID || !defined K_iOS)	// for now we dont use this for Andoid
		WallPapaerRect1920[0] = new CRectangle(382.0f,313.0f,150.0f,44.0f);
		WallPapaerRect1920[1] = new CRectangle(670.0f,312.0f,150.0f,45.0f);
		WallPapaerRect1920[2] = new CRectangle(964.0f,314.0f,150.0f,42.0f);
		WallPapaerRect1366[0] = new CRectangle(383.0f,367.0f,148.0f,41.0f);
		WallPapaerRect1366[1] = new CRectangle(671.0f,365.0f,151.0f,42.0f);
		WallPapaerRect1366[2] = new CRectangle(964.0f,368.0f,150.0f,41.0f);
		WallPapaerRect1024[0] = new CRectangle(381.0f,414.0f,150.0f,43.0f);
		WallPapaerRect1024[1] = new CRectangle(671.0f,414.0f,150.0f,45.0f);
		WallPapaerRect1024[2] = new CRectangle(963.0f,414.0f,153.0f,43.0f);
	#endif

	zoomImageSpr = NULL;

	zoomDarkBG = new CObject();
	zoomDarkBG->LoadSprite("MNU\\EXTRAS\\MASK.SPR");
	zoomDarkBG->SetXY(683,384);
	zoomDarkBG->SetScale(2.0);
	zoomDarkBG->SetZ(1200);
	zoomDarkBG->Show(false);

	zoomState = eZoomInactive;

	for (int i = 0; i < 8; i++)
	{
		ExtrasOver[i] = NULL;
		ExtrasOver[i] = ExtrasPopupScene->GetObjectByID(std::string("ExtrasItem_")+MKSTR(i));
		ExtrasOver[i]->Show(false);
	}

#ifdef TOUCH_UI
	ExtrasOver[4]->SetX(483);
	ExtrasOver[6]->SetX(685);
	ExtrasOver[7]->SetX(870);

	hopuzzlesTxt->SetX(hopuzzlesTxt->Pos.x + 95);
	musicTxt->SetX(musicTxt->Pos.x - 93);
	achievementsTxt->SetX(achievementsTxt->Pos.x - 80);
#endif

	#ifdef K_ANDROID
		if (GFApp->isUnlocked())UnlockExtrasforFullVersion(); // If the game is Full version unlock all Extras already.
	#endif
}

ExtrasMenuControl::~ExtrasMenuControl()
{
	_maskSpr = NULL;

	for (int i = 0; i < MAX_MUSIC_TRACKS; i++)
	{
		SAFE_DELETE(MusicTrackRects[i]);
	}

	_CutsceneVideoCallback = nullptr;
	_kidroomAreaCallback = nullptr;
	_AwardsAreaCallback = nullptr;
	_CreditsCallback = nullptr;

	_HOPuzzleLoadCallback= nullptr;

	for (int i = 0; i < 8; i++)
	{
		ExtrasOver[i] = NULL;
		delete ExtraItemsRect[i];
		ExtraItemsRect[i] = NULL;
	}

	for(int i = 0; i < 6; i++ )
	{
		delete PageItemRect[i];
		PageItemRect[i] = NULL;
	}

	SAFE_DELETE(zoomImageSpr);
	SAFE_DELETE(zoomDarkBG);

	delete CurrentPageText;
	delete ExtrasPopupScene;
	delete SCN_Concepts;
	delete SCN_CutScenes;
	delete SCN_HOScenes;
	delete SCN_MiniGames;
	delete SCN_Wallpapaers;
	delete SCN_Music;

	delete achievementsTxt;
	delete minigamesTxt;
	delete memoriesTxt;
	delete conceptsTxt;
	delete cutscenesTxt;
	delete musicTxt;
	delete hopuzzlesTxt;
#ifndef TOUCH_UI
	delete wallpapersTxt;
#endif
}

void ExtrasMenuControl::Required()
{
	
}

void ExtrasMenuControl::UpdateUnlockStatus()
{
	for (int i = 0; i < 6; i++)
	{
		switch(PopupState)
		{
		case Popup_Extras_CutScenes:	if( CheckUnlockStatus(i))
											SCN_CutScenes->GetObjectByID("Lock" + MKSTR(i))->SetEnable(false);
										else
											SCN_CutScenes->GetObjectByID("Lock" + MKSTR(i))->SetEnable(true);
			break;
		case Popup_Extras_HOScenes:		if( CheckUnlockStatus(i))
											SCN_HOScenes->GetObjectByID("Lock" + MKSTR(i))->SetEnable(false);
										else
											SCN_HOScenes->GetObjectByID("Lock"+ MKSTR(i))->SetEnable(true);
			break;
		case Popup_Extras_MiniGames:	if( CheckUnlockStatus(i))
											SCN_MiniGames->GetObjectByID("Lock" + MKSTR(i))->SetEnable(false);
										else
											SCN_MiniGames->GetObjectByID("Lock"+ MKSTR(i))->SetEnable(true);
			break;
		default: break;
		}
		
	}
}

void ExtrasMenuControl::UnlockExtrasforFullVersion()
{
	for (int i = 0; i < 6; i++)
		{
			UnlockStatus[i] = true;
		}	
}

bool ExtrasMenuControl::CheckUnlockStatus(int ItemId)
{
	#ifdef K_ANDROID  // if it is android and if the build is a Full version, then skip unlock check and unlock everything
		if (GFApp->isUnlocked()) 
			{
				UnlockExtrasforFullVersion();
				return true;
			}
	#endif
	bool _unlockFlag = false;
	int _id = ( pageIndex * 6 ) + ItemId;
	if( PopupState == Popup_Extras_CutScenes )
	{
		switch (_id)
		{
		case eCutscene_BackStory:	_unlockFlag = true;
			break;
		case eCutscene_L1Intro:
			if(ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L1SophiaPainting:
			if(ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete)
				_unlockFlag = true;
			break;
		case eCutscene_L1CreekNarration:
			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L1NewsPaper:
			if(ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperCutScenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L1EndCutScene:
			if(ProfileSystem::Instance->ActiveProfile.L1_IsDemoOutroVideoPlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L2Intro:
			if(ProfileSystem::Instance->ActiveProfile.L2_S1_IsIntroSwimVideoPlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L2MoopanTheyyam:
			if(ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan)
				_unlockFlag = true;
			break;
		case eCutscene_L2EndCutScene:
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S1_FortEntrance])
				_unlockFlag = true;
			break;
		case eCutscene_L3ShoeMemory:
			if(ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockState>=3)
				_unlockFlag = true;
			break;
		case eCutscene_L3DiaryNote:
			if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsAmeehaCutscenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L4FlowerMemory:
			if (ProfileSystem::Instance->ActiveProfile.L4_S4_FlowerCutscenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L4DiaryNote:
			if(ProfileSystem::Instance->ActiveProfile.L4_S3_IsMetalPaperTaken)
				_unlockFlag = true;
			break;
		case eCutscene_L4EndCutScene:
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L5_S1_TigerDistant])
				_unlockFlag = true;
			break;
		case eCutscene_L5GrandmaCutscene:
			if(ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L5EndPeacockCutScene:
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S1_FloatingIsland1])
				_unlockFlag = true;
			break;
		case eCutscene_L6FakeJames:
			if(ProfileSystem::Instance->ActiveProfile.L6_S3_LasyaCutscenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L6VaruniCutScene:
			if(ProfileSystem::Instance->ActiveProfile.L6_S1_VaruniCutscenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L6EndPortalCutscene:
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S1_Volcano])
				_unlockFlag = true;
			break;
		case eCutscene_L7LasyaIllusion:
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S2_LasyaPrison])
				_unlockFlag = true;
			break;
		case eCutscene_L7MarysNightmare:
			if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsNightmareCutscnPlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L7LasyaAttack:
			if(ProfileSystem::Instance->ActiveProfile.L6_S3_LasyaCutscenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L7EndCutScene1:
			if(ProfileSystem::Instance->ActiveProfile.L7_S5_End1CutScenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L7EndCutScene2:
			if(ProfileSystem::Instance->ActiveProfile.L7_S5_End2CutScenePlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L8Intro:
			if(ProfileSystem::Instance->ActiveProfile.L8_S1_IntroVidPlayed)
				_unlockFlag = true;
			break;
		case eCutscene_L8FlyingMammoth:
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L8_S5_VolcanoLand])
				_unlockFlag = true;
			break;
		case eCutscene_L8EndCutScene:
			if(ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed)
				_unlockFlag = true;
			break;
		default:
			break;
		}
	}
	else if( PopupState == Popup_Extras_HOScenes )
	{
		switch (_id)
		{
		case 0:			if( ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved )
							_unlockFlag = true;
			break;
		case 1:			if( ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
							_unlockFlag = true;
			break;
		case 2:			if( ProfileSystem::Instance->ActiveProfile.L1S5_HOSolved)
							_unlockFlag = true;
			break;
		case 3:			if( ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken )
							_unlockFlag = true;
			break;
		case 4:			if( ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted )
							_unlockFlag = true;
			break;
		case 5:			if( ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompleted )
							_unlockFlag = true;
			break;
		case 6:			if( ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompleted )
							_unlockFlag = true;
			break;
		case 7:			if( ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted )
							_unlockFlag = true;
			break;
		case 8:			if( ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken )
							_unlockFlag = true;
			break;
		case 9:
            //This was originally 6 , and size of the array is 6, so the last value is 5
                        if( ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularFound[5] )
                            _unlockFlag = true;
			break;
		case 10:		if( ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterCompleted )
							_unlockFlag = true;
			break;
		case 11:		if( ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiCompleted )
							_unlockFlag = true;
			break;
		case 12:		if( ProfileSystem::Instance->ActiveProfile.L5_S6_HOCompleted )
							_unlockFlag = true;
			break;
		case 13:		if( ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken )
							_unlockFlag = true;
			break;
		case 14:		if( ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected )
							_unlockFlag = true;
			break;
		case 15:		if( ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted )
							_unlockFlag = true;
			break;
		case 16:		if( ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrokenLeftHandCompleted )
							_unlockFlag = true;
			break;
		default:
			break;
		}
	}
	else if( PopupState == Popup_Extras_MiniGames )
	{
		switch (_id)
		{
		case 0:		if(ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete)
						_unlockFlag = true;
			break;
		case 1:		if(ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved)
						_unlockFlag = true;
			break;
		case 2:		if(ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved)
						_unlockFlag = true;
			break;
		case 3:		if(ProfileSystem::Instance->ActiveProfile.L1S4_PaintingPuzzleSolved)
						_unlockFlag = true;
			break;
		case 4:		if(ProfileSystem::Instance->ActiveProfile.L1S3_IsShadowPuzzleSolved)
						_unlockFlag = true;
			break;
		case 5:		if(ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken)
						_unlockFlag = true;
			break;
		case 6:		if(ProfileSystem::Instance->ActiveProfile.L2_S3_StonePathActivated)
						_unlockFlag = true;
			break;
		case 7:		if(ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState >= 1)
						_unlockFlag = true;
			break;
		case 8:		if(ProfileSystem::Instance->ActiveProfile.L2_S5_ShelfPuzzleSolved)
						_unlockFlag = true;
			break;
		case 9:		if(ProfileSystem::Instance->ActiveProfile.L2_S6_ClockPuzzleSolved)
						_unlockFlag = true;
			break;
		case 10:	if(ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState >= 10 )
						_unlockFlag = true;
			break;
		case 11:	if(ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadPuzzleCompleted)
						_unlockFlag = true;
			break;
		case 12:	if(ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlockState >= 5)
						_unlockFlag = true;
			break;
		case 13:	if(ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved)
						_unlockFlag = true;
			break;
		case 14:	if(ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved)
						_unlockFlag = true;
			break;
		case 15:	if(ProfileSystem::Instance->ActiveProfile.L4_S2_FanPuzzleState >= 2 )
						_unlockFlag = true;
			break;
		case 16:	if(ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved)
						_unlockFlag = true;
			break;
		case 17:	if(ProfileSystem::Instance->ActiveProfile.L4_S5_SwapPuzzleCompleted)
						_unlockFlag = true;
			break;
		case 18:	if(ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlocksPuzzleSolved)
						_unlockFlag = true;
			break;
		case 19:	if(ProfileSystem::Instance->ActiveProfile.L5_S3_PZState >= 161 )
						_unlockFlag = true;
			break;
		case 20:	if(ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffState >= 123 )
						_unlockFlag = true;
			break;
		case 21:	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted)
						_unlockFlag = true;
			break;
		case 22:	if(ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete)
						_unlockFlag = true;
			break;
		case 23:	if(ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved )
						_unlockFlag = true;
			break;
		case 24:	if(ProfileSystem::Instance->ActiveProfile.L6_S3_PZPathwayPuzzleCompleted )
						_unlockFlag = true;
			break;
		case 25:	if(ProfileSystem::Instance->ActiveProfile.L7_S1_PZJigsawYellowCompleted)
						_unlockFlag = true;
			break;
		case 26:	if(ProfileSystem::Instance->ActiveProfile.L7_S4_PZJigsawBlueCompleted)
						_unlockFlag = true;
			break;
		case 27:	if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted)
						_unlockFlag = true;
			break;
		case 28:	if(ProfileSystem::Instance->ActiveProfile.L7_S2_PZEyePuzzleCompleted)
						_unlockFlag = true;
			break;
		case 29:	if(ProfileSystem::Instance->ActiveProfile.L7_S2_PZLasyaStatuePuzzleCompleted)
						_unlockFlag = true;
			break;
		case 30:	if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZDoorCompleted)
						_unlockFlag = true;
			break;
		case 31:	if(ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved)
						_unlockFlag = true;
			break;
		default:
			break;
		}
	}
	return(_unlockFlag);
}

void ExtrasMenuControl::Update()
{
	if(PopupState == Popup_Disabled)
		return;
	PrevCursorMode = Cursor::GetMode();
	Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	if(zoomState != eZoomInactive)
	{
		if(zoomState == eZoomIdle)
		{
			if(ZoomedRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if (clicked)
				{
					if (zoomImageSpr!=NULL)
					{
						float valArr[5] = {0.0f, zoomOrigin.x, zoomOrigin.y, 0.15f, 15.0f};
						zoomImageSpr->TweenTo(valArr, 0.45f, TW_EASEQUADOUT, true, CC_CALLBACK_0(ExtrasMenuControl::onZoomImageTweenCompleted,this));

						zoomDarkBG->FadeOut(4.0f);
						zoomState = eZoomingOut; 
					}
				}
			}
		}
		return;
	}
	switch(PopupState)
	{
		case Popup_Extras:				UpdateExtrasPopup();
			break;
		case Popup_Extras_ConceptArt:	UpdateExtrasItemPagePopup(ExtraPage_Concept_Art);
			break;
		case Popup_Extras_CutScenes:	UpdateExtrasItemPagePopup(ExtraPage_CutScenes);
			break;
		case Popup_Extras_HOScenes:		UpdateExtrasItemPagePopup(ExtraPage_HOScenes);
			break;
		case Popup_Extras_MiniGames:	UpdateExtrasItemPagePopup(ExtraPage_MiniGames);
			break;
		case Popup_Extras_Wallpapers:	UpdateExtrasItemPagePopup(ExtraPage_Wallpapaers);
			break;
		case Popup_Extras_Music:		UpdateExtrasItemPagePopup(ExtraPage_Music);
			break; 
		default: break;
	}

	int pageItemSize = 6;
	if(PopupState == Popup_Extras_Wallpapers )
		pageItemSize = 3;
	

	if(PopupState != Popup_Extras_Music )
	{
		for (int i = 0; i < pageItemSize; i++)
		{
			if( PageItemRect[i]->Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandPointy);

				if (Control::Input->LBclicked() )
				{
					Cursor::SetMode(CA_Normal);
					HandleItemClicks(i);
				}
			} 
		} 
	}

	if(PopupState == Popup_Extras_Wallpapers)
	{
		#if (!defined K_ANDROID || !defined K_iOS) // for now we skip the saving part in Android
			int size = eSaveInvalid;
			for (int i = 0; i < 3; i++)
			{
				if( WallPapaerRect1024[i]->Intersection(&pos) )
					size = eSave1024;
				if( WallPapaerRect1366[i]->Intersection(&pos)) 
					size = eSave1366;
				if( WallPapaerRect1920[i]->Intersection(&pos) )
					size = eSave1920;
				
				if ( size != eSaveInvalid )
				{
					Cursor::SetMode(CA_HandPointy);
					if (Control::Input->LBclicked() )
					{
						Cursor::SetMode(CA_Normal);
						OnSaveWallpaperEvent( i, size );
						return;
					}
				}
			}
		#endif
	}
}


void ExtrasMenuControl::OnSaveWallpaperEvent(int itemId, int itemSize)
{

	int ItemId = -1;
	ItemId =  (pageIndex * 3) + itemId;
	
	char *filePathChar = KMiscTools::makeFilePath("Assets\\MNU\\EXTRAS\\FULLSIZE\\");
	std::string srcFilePath(filePathChar);	

	std::string filename = "WALLPAPER" + MKSTR(ItemId) + ".JPG";
	#ifdef K_WIN32
		std::string destFilename = "\\WALLPAPER" + MKSTR(ItemId);
	#else
		std::string destFilename = "/WALLPAPER-" + MKSTR(ItemId);
	#endif
	switch (itemSize)
	{
	case eSave1024: 
				#ifdef K_WIN32
					srcFilePath += ("1024X768\\"+filename);
				#else
					srcFilePath += ("1024X768/"+filename);
				#endif
					destFilename += "_1024.JPG";
		break;
	case eSave1366:	
				#ifdef K_WIN32
					srcFilePath += ("1366X768\\"+filename);
				#else
					srcFilePath += ("1366X768/"+filename);
				#endif
					destFilename += "_1366.JPG";
		break;
	case eSave1920:	
				#ifdef K_WIN32
					srcFilePath += ("1920X1080\\"+filename);
				#else
					srcFilePath += ("1920X1080/"+filename);
				#endif
					destFilename += "_1920.JPG";
		break;
	default:
		break;
	}


	#ifdef K_WIN32
		//get Documents Folder
		CHAR myDocumentsPath[MAX_PATH];
		HRESULT result = 0; 
		
		//////////////////////////

	
		  OSVERSIONINFOEXA vi;
		  /* Check Windows version */
		  ZeroMemory (&vi, sizeof (vi));
		  vi.dwOSVersionInfoSize = sizeof (vi);
		  GetVersionExA ((OSVERSIONINFOA *) &vi);		  
		  switch (vi.dwMajorVersion) 
		  {
			  default:
			   /* 4 - DOS based Windows: 95, 98 or ME */				  
			   /* 5 - NT kernel based Windows 5.x: Win2000,Windows Server 2003 or XP */
				//C:\Documents and Settings\USERNAME\Application Data\STUDIONAME\GAMENAME
				result = SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocumentsPath);
				break;
			  case 6: //NTDDI_VISTA
				    /* Vista,Windows Server 2008,Windows 7,Windows Server 2008 RC2 */
					//C:\Users\ USERNAME\AppData\Roaming\STUDIONAME\GAMENAME
					//if (vi.dwMinorVersion == 2 ) //windows 10 machine detected!!
					{
						K_LOG("Windows Vista onwards machine detected");
						LPWSTR roamingPath;
						result = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &roamingPath);												
						strcpy(myDocumentsPath,(FileSystem::ws2s(roamingPath)).c_str());						
						if (SUCCEEDED(result))
							K_LOG(myDocumentsPath);
						
					}
				    
				 break;  
		  }
	#endif

	//////////////////////////
	#if defined K_WIN32
		std::string SaveFolder = myDocumentsPath + std::string("\\Darkarta - A Broken Heart's Quest");
	#elif (defined K_ANDROID)
		bool storage_permissions = true;
		std::string SaveFolder ="";
		if(	!KSysAndroid::checkPermissions ("android.permission.WRITE_EXTERNAL_STORAGE"))
		{
			if(!KSysAndroid::requestPermissions("android.permission.WRITE_EXTERNAL_STORAGE","We require your permission to store the wallpaper to your system, Please allow us to do so for accessing these wonderful wallpapers in your device, Thanks!"))
			{
				storage_permissions = false;
			}
		}
		else
		{
			if(	!KSysAndroid::checkPermissions ("android.permission.CAMERA"))
			{
				if(!KSysAndroid::requestPermissions("android.permission.CAMERA","We require your permission to store the wallpaper to your system, Please allow us to do so for accessing these wonderful wallpapers in your device, Thanks!"))
				{
					storage_permissions = false;
				}
			}
			else
				SaveFolder =   GFApp->pathToExternalDirectory + std::string("/Wallpapers/Darkarta-ABHQ");
		}
	#elif (defined K_iOS)
		std::string SaveFolder =  KMiscTools::getUserFolder() + std::string("/Pictures/Darkarta - A Broken Heart's Quest");
	#else
		std::string userFolder = KMiscTools::getUserFolder();
		// now we need to strip the hidden folder of .kanjidata from the folder to get actual user's home folder
		std::string toRemove = "/.kanjidata";
		std::string::size_type i = userFolder.find(toRemove);
		if (i != std::string::npos)
   			userFolder.erase(i, toRemove.length());
		// Final Save folder where our game can save the Wallpaper
		std::string SaveFolder = userFolder +std::string("/Pictures/Darkarta - A Broken Heart's Quest");
	#endif
	
	bool dirExists=KMiscTools::fileExists(SaveFolder.c_str());
	if( !dirExists )
	{
		K_LOG("Folder doesnt exist %s, So trying to create it",SaveFolder.c_str());
		dirExists=KMiscTools::createDirectory(SaveFolder.c_str());// Trying to create Game Wallpaper folder in User's Pictures directory
	}

	#ifndef K_WIN32
		// We will try this only in non Win systems, as the code to handle this for Win system is handled seperately below
		// if User Folder cant be used or permission is denined, then try creating Wallpapers folder inside Game Folder
		if( !dirExists )
		{
				std::string SaveFolder1 =  KMiscTools::getGameFolder() + std::string("/Wallpapers");
				K_LOG("Folder Can not be created in  %s, So trying to create it in Game Folder as %s",SaveFolder.c_str(), SaveFolder1.c_str());
				dirExists=KMiscTools::fileExists(SaveFolder1.c_str());
				if( !dirExists )
				{
					K_LOG("Folder doesnt exist %s, So trying to create it",SaveFolder1.c_str());
					dirExists=KMiscTools::createDirectory(SaveFolder1.c_str());//Game store directory
				}
				if (dirExists) SaveFolder = SaveFolder1;
		}
		
		
	#endif

	//////////////////////////
	std::string destinationPath =  SaveFolder+destFilename;

	#ifdef K_WIN32
	/*
		LPCSTR lpSrcFilePtr = srcFilePath.c_str();
		LPCSTR lpDestFilePtr = destinationPath.c_str();

		BOOL CopyStats = CopyFileA(lpSrcFilePtr, lpDestFilePtr, true);
		K_LOG("COPYSTATS %s",(CopyStats == TRUE)?"true":"false");
		if(GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			K_LOG("Error File not found");
		}
		else
		{
			K_LOG("sourcefile %s", srcFilePath.c_str());
			K_LOG("destFile %s", destinationPath.c_str());
		}
	
		if( CopyStats == FALSE &&
			(GetLastError() == ERROR_PATH_NOT_FOUND || GetLastError() == ERROR_ACCESS_DENIED) )//as fallback option trying to save in game folder!!!
		{
			std::string gameFolderPath = KMiscTools::getGameFolder();		 
			std::string wallPaperPath = gameFolderPath+destFilename;
			LPCSTR lpDestFilePtr2 = wallPaperPath.c_str();
			K_LOG("%s",wallPaperPath.c_str());
			std::ifstream srce( lpSrcFilePtr, std::ios::binary ) ;
			std::ofstream dest( lpDestFilePtr2, std::ios::binary ) ;
			dest << srce.rdbuf() ;
		}


		_flushall();//write all open output streams
	
	
		std::string saveMsg = StringTable::getStr("Exwall")+ SaveFolder;
		if(_showMessageBannerCallback!=nullptr)
			_showMessageBannerCallback(saveMsg,0);
			*/
			K_LOG("Saving wallpaper to %s",destinationPath.c_str());
				std::string saveMsg = StringTable::getStr("Exwalln1")+ SaveFolder;
			
				KGraphic *lpWallpaperGraphic = KPTK::createKGraphic ();
				bool bWSuccess = false;
				bWSuccess = lpWallpaperGraphic->loadPicture (srcFilePath.c_str(), true, true);
				if (!bWSuccess) {
					std::string TmpStr = std::string("Er0x12201-");
					std::string TmpStr1 =  std::string("Er0x12201- Wallpaper Source file couldnt be opened, Saving Wallpaper Failed!");
					TmpStr = TmpStr + srcFilePath;
					TmpStr = TmpStr + std::string(" failed to open") ;	
					K_LOG(" %s  from the resource archive", TmpStr.c_str());
						WARN( (const char*)(TmpStr1.c_str()) );
					return;
				}
				lpWallpaperGraphic->setTextureQuality (true);
				lpWallpaperGraphic->savePicture(destinationPath.c_str(), K_IMAGE_JPG);
				delete lpWallpaperGraphic;
				//check if the file was saved in the filesystem or not
				bool fileExists=KMiscTools::fileExists(destinationPath.c_str());
				if( !fileExists )
				{
					K_LOG("Source file was found, read and tryed to save to Destination file %s, But is not found created!!",destinationPath.c_str());
					saveMsg = std::string("Couldn't save the Wallpaper to the ")+ SaveFolder + std::string(".") ;
				}
				else
				{
					K_LOG("Wallpaper saved successfully to %s!!",destinationPath.c_str());
					saveMsg = StringTable::getStr("Exwall")+ SaveFolder;
				}			
				
				if(_showMessageBannerCallback!=NULL)
				_showMessageBannerCallback(saveMsg,0);
	
	#else 
		if(dirExists){
			#ifdef K_ANDROID
                std::string saveMsg ="";
				if (storage_permissions)
					{
						
						K_LOG("Saving wallpaper to %s",destinationPath.c_str());
						saveMsg = StringTable::getStr("Exwalln1")+ SaveFolder;
					
						//KGraphic *lpWallpaperGraphic = KPTK::createKGraphic ();
						bool bWSuccess = false;
						std::ifstream srce( srcFilePath.c_str(), std::ios::binary ) ;
						std::ofstream dest( destinationPath.c_str(), std::ios::binary ) ;
						dest << srce.rdbuf() ;
						/*
						bWSuccess = lpWallpaperGraphic->loadPicture (srcFilePath.c_str(), true, true);
						if (!bWSuccess) {
							std::string TmpStr = std::string("Er0x12201-");
							std::string TmpStr1 =  std::string("Er0x12201- Wallpaper Source file couldnt be opened, Saving Wallpaper Failed!");
							TmpStr = TmpStr + srcFilePath;
							TmpStr = TmpStr + std::string(" failed to open") ;	
							K_LOG(" %s  from the resource archive", TmpStr.c_str());
								WARN( (const char*)(TmpStr1.c_str()) );
							return;
						}*/
						/*
						lpWallpaperGraphic->setTextureQuality (true);
						lpWallpaperGraphic->savePicture(destinationPath.c_str(), K_IMAGE_JPG);
						delete lpWallpaperGraphic;
						*/
						//check if the file was saved in the filesystem or not
						bool fileExists=KMiscTools::fileExists(destinationPath.c_str());
						if( !fileExists )
						{
							K_LOG("Source file was found, read and tryed to save to Destination file %s, But is not found created!!",destinationPath.c_str());
							saveMsg = std::string("Couldn't save the Wallpaper to the ")+ SaveFolder + std::string(".") ;
						}
						else
						{
							K_LOG("Wallpaper saved successfully to %s!!",destinationPath.c_str());
							saveMsg = StringTable::getStr("Exwall")+ SaveFolder;
						}
					}	
				else
				{
					saveMsg = std::string("Sorry you didn't provide the permission to save the Wallpapers!");
				}	
					KSysAndroid::showToast(saveMsg.c_str(),true);
					if(_showMessageBannerCallback!=NULL)
					_showMessageBannerCallback(saveMsg,0);
			#else
            
#ifndef K_iOS
				K_LOG("Saving wallpaper to %s",destinationPath.c_str());
				std::string saveMsg = StringTable::getStr("Exwalln1")+ SaveFolder;
			
				KGraphic *lpWallpaperGraphic = KPTK::createKGraphic ();
				bool bWSuccess = false;
				bWSuccess = lpWallpaperGraphic->loadPicture (srcFilePath.c_str(), true, true);
				if (!bWSuccess) {
					std::string TmpStr = std::string("Er0x12201-");
					std::string TmpStr1 =  std::string("Er0x12201- Wallpaper Source file couldnt be opened, Saving Wallpaper Failed!");
					TmpStr = TmpStr + srcFilePath;
					TmpStr = TmpStr + std::string(" failed to open") ;	
					K_LOG(" %s  from the resource archive", TmpStr.c_str());
						WARN( (const char*)(TmpStr1.c_str()) );
					return;
				}
				lpWallpaperGraphic->setTextureQuality (true);
				lpWallpaperGraphic->savePicture(destinationPath.c_str(), K_IMAGE_JPG);
				delete lpWallpaperGraphic;
				//check if the file was saved in the filesystem or not
				bool fileExists=KMiscTools::fileExists(destinationPath.c_str());
				if( !fileExists )
				{
					K_LOG("Source file was found, read and tryed to save to Destination file %s, But is not found created!!",destinationPath.c_str());
					saveMsg = std::string("Couldn't save the Wallpaper to the ")+ SaveFolder + std::string(".") ;
				}
				else
				{
					K_LOG("Wallpaper saved successfully to %s!!",destinationPath.c_str());
					saveMsg = StringTable::getStr("Exwall")+ SaveFolder;
				}			
				
				if(_showMessageBannerCallback!=NULL)
				_showMessageBannerCallback(saveMsg,0);
#endif
			#endif
	//			saveMsg = StringTable::getStr("Exwall")+ SaveFolder;
		}
		else
		{
			K_LOG("Not able to Save wallpaper to %s",destinationPath.c_str());
			std::string saveMsg = StringTable::getStr("Exwalln1")+ SaveFolder;
			#ifdef K_ANDROID
				KSysAndroid::showToast(saveMsg.c_str(),true);
			#endif
				if(_showMessageBannerCallback!=NULL)
				_showMessageBannerCallback(saveMsg,0);
			
		}
	#endif	
}


void ExtrasMenuControl::HandleItemClicks(int index )
{
	int ItemId = -1;
	switch(PopupState)
	{
	case Popup_Extras_CutScenes:	
		{
			if( CheckUnlockStatus(index) )
			{
				ItemId =  (pageIndex * 6) + index;
				GFInstance->activeCutsceneID = ItemId;
				if( _CutsceneVideoCallback != nullptr )
				{
					_CutsceneVideoCallback();
				}
			}
			else
			{
				std::string lockMsg = StringTable::getString("lockMsgCut");
				if(_showMessageBannerCallback!=nullptr)
					_showMessageBannerCallback(lockMsg,0);
			}
		}							
		break;
	case Popup_Extras_ConceptArt:
		{
			ItemId =  (pageIndex * 6) + index;
			SetupZoomImageAndShow(ItemId, index);
		}		
		break;
	case Popup_Extras_HOScenes:
		{
			if( CheckUnlockStatus(index) )
			{
				ItemId =  (pageIndex * 6) + index;
				LoadHiddenObjectScene(ItemId);
			}
			else
			{
				std::string lockMsg = StringTable::getString("lockMsgHO");
				if(_showMessageBannerCallback!=nullptr)
					_showMessageBannerCallback(lockMsg,0);
			}
		}	
		
		break;
	case Popup_Extras_MiniGames:
		{
			if( CheckUnlockStatus(index) )
			{
				ItemId =  (pageIndex * 6) + index;
				LoadPuzzleScene(ItemId);
			}
			else
			{
				std::string lockMsg = StringTable::getString("lockMsgMini");
				if(_showMessageBannerCallback!=nullptr)
					_showMessageBannerCallback(lockMsg,0);
			}
		}	
		break;						
	case Popup_Extras_Wallpapers:	ItemId =  (pageIndex * 3) + index;
		SetupZoomImageAndShow(ItemId, index);
		break;						
	case Popup_Extras_Music:		ItemId =  index;
		break;
	default: break;	
	} 	
}

void ExtrasMenuControl::LoadHiddenObjectScene(int ItemId)
{
	int extrasHOLevelIDs[] = {
		Level_1, //eArea_L1_S3_DoorHO,
		Level_1, //eArea_L1_S6_HOBoatShed,
		Level_1, //eArea_L1_S5_HOTreasureRoom,
		Level_2, //eArea_L2_S1_UndergroundHO,
		Level_2, //eArea_L2_S2_HONagaTemple,
		Level_2, //eArea_L2_S6_HOWaterfall,
		Level_3, //eArea_L3_S1_HOLadder,
		Level_3, //eArea_L3_S4_FortFountainHO,
		Level_4, //eArea_L4_S1_ShoeHO,
		Level_4, //eArea_L4_S4_BinocularHO,
		Level_5, //eArea_L5_S3_HOUnderwater,
		Level_5, //eArea_L5_S5_HOGanapathi,
		Level_5, //eArea_L5_S6_HOTree,
		Level_6, //eArea_L6_S1_DescriptionHO,
		Level_6, //eArea_L6_S2_ShereHO,
		Level_7, //eArea_L7_S2_HORightHand,
		Level_7 //eArea_L7_S2_HOBrokenRightHand
	};
	int extrasHOSceneIDs[] = {
		eArea_L1_S3_DoorHO,
		eArea_L1_S6_HOBoatShed,
		eArea_L1_S5_HOTreasureRoom,
		eArea_L2_S1_UndergroundHO,
		eArea_L2_S2_HONagaTemple,
		eArea_L2_S6_HOWaterfall,
		eArea_L3_S1_HOLadder,
		eArea_L3_S4_FortFountainHO,
		eArea_L4_S1_ShoeHO,
		eArea_L4_S4_BinocularHO,
		eArea_L5_S3_HOUnderwater,
		eArea_L5_S5_HOGanapathi,
		eArea_L5_S6_HOTree,
		eArea_L6_S1_DescriptionHO,
		eArea_L6_S2_ShereHO,
		eArea_L7_S2_HORightHand,
		eArea_L7_S2_HOBrokenRightHand
	};

	if(_HOPuzzleLoadCallback!=nullptr)
		_HOPuzzleLoadCallback(extrasHOLevelIDs[ItemId], extrasHOSceneIDs[ItemId]);
}

void ExtrasMenuControl::LoadPuzzleScene(int ItemId)
{
	int extrasPuzzleLevelIDs[] = {
		Level_1, //eArea_L1_S1_PZKidsPainting,
		Level_1, //eArea_L1_S2_PZFuse,
		Level_1, //eArea_L1_S3_PZShelf,
		Level_1, //eArea_L1_S4_PZPainting,
		Level_1, //eArea_L1_S4_PZDoor,
		Level_2, //eArea_L2_S2_PZMango,
		Level_2, //eArea_L2_S3_PZStoneBeam,
		Level_2, //eArea_L2_S4_PZDeities,
		Level_2, //eArea_L2_S5_PZEagleUnblock,
		Level_2, //eArea_L2_S6_PZTunnelClock,
		Level_2, //eArea_L2_S7_PZSoldiers,
		Level_3, //eArea_L3_S1_PZLionHead,
		Level_3, //eArea_L3_S3_PZBlocks,
		Level_3, //eArea_L3_S3_PZUnderwater,
		Level_4, //eArea_L4_S2_PZAlligatorHead,
		Level_4, //eArea_L4_S2_PZDoor,
		Level_4, //eArea_L4_S3_PZDoor,
		Level_4, //eArea_L4_S5_PZSwapTile,
		Level_5, //eArea_L5_S2_PZBlocks,
		Level_5, //eArea_L5_S3_PZDungeonDoor,
		Level_5, //eArea_L5_S4_PZHandCuff,
		Level_5, //eArea_L5_S5_PZSunClock,
		Level_6, //eArea_L6_S4_PortalPuzzle,
		Level_6, //eArea_L6_S5_TabMiniGame,
		Level_6, //eArea_L6_S3_PZPathway,
		Level_7, //eArea_L7_S1_PZJigsawYellow,
		Level_7, //eArea_L7_S4_PZJigsawBlue,
		Level_7, //eArea_L7_S3_PZJigsawRed,
		Level_7, //eArea_L7_S2_PZEyePuzzle,
		Level_7, //eArea_L7_S2_PZLasyaStatue,
		Level_7, //eArea_L7_S3_PZDoorPuzzle,
		Level_8 //eArea_L8_S4_PZPoolGame
	};
	int extrasPuzzleSceneIDs[] = {
		eArea_L1_S1_PZKidsPainting,
		eArea_L1_S2_PZFuse,
		eArea_L1_S3_PZShelf,
		eArea_L1_S4_PZPainting,
		eArea_L1_S4_PZDoor,
		eArea_L2_S2_PZMango,
		eArea_L2_S3_PZStoneBeam,
		eArea_L2_S4_PZDeities,
		eArea_L2_S5_PZEagleUnblock,
		eArea_L2_S6_PZTunnelClock,
		eArea_L2_S7_PZSoldiers,
		eArea_L3_S1_PZLionHead,
		eArea_L3_S3_PZBlocks,
		eArea_L3_S3_PZUnderwater,
		eArea_L4_S2_PZAlligatorHead,
		eArea_L4_S2_PZDoor,
		eArea_L4_S3_PZDoor,
		eArea_L4_S5_PZSwapTile,
		eArea_L5_S2_PZBlocks,
		eArea_L5_S3_PZDungeonDoor,
		eArea_L5_S4_PZHandCuff,
		eArea_L5_S5_PZSunClock,
		eArea_L6_S4_PortalPuzzle,
		eArea_L6_S5_TabMiniGame,
		eArea_L6_S3_PZPathway,
		eArea_L7_S1_PZJigsawYellow,
		eArea_L7_S4_PZJigsawBlue,
		eArea_L7_S3_PZJigsawRed,
		eArea_L7_S2_PZEyePuzzle,
		eArea_L7_S2_PZLasyaStatue,
		eArea_L7_S3_PZDoorPuzzle,
		eArea_L8_S4_PZPoolGame
	};

	if(_HOPuzzleLoadCallback!=nullptr)
		_HOPuzzleLoadCallback(extrasPuzzleLevelIDs[ItemId], extrasPuzzleSceneIDs[ItemId]);
}

void ExtrasMenuControl::SetupZoomImageAndShow(int itemID, int posIndex)
{
	if(zoomImageSpr!=NULL)
		SAFE_DELETE(zoomImageSpr);

	std::string filepath = "";

	switch(PopupState)
	{
	case Popup_Extras_ConceptArt:
		filepath = conceptsPath + "concept"+MKSTR(itemID)+".SPR";	
		break;
	case Popup_Extras_HOScenes:
		break;
	case Popup_Extras_MiniGames:
		break;						
	case Popup_Extras_Wallpapers:		
		//Preview the Page
		filepath = wallpaperPath + "Wallpaper"+MKSTR(itemID)+".SPR";	
		break;						
	case Popup_Extras_Music:
		break;
        default: //Sac: added this because some of the Enumurated values are not used here
        break;
	}

	if(filepath!="")
	{
		zoomOrigin.x = PageItemRect[posIndex]->GetCenterX();
		zoomOrigin.y = PageItemRect[posIndex]->GetCenterY();

		zoomImageSpr = new CObject();
		zoomImageSpr->LoadInit(filepath, zoomOrigin.x, zoomOrigin.y, 1300);

		zoomImageSpr->SetAlpha(0);
		zoomImageSpr->SetScale(0.15f);
		zoomImageSpr->SetRotate(15.0f);

		float valArr[5] = {1.0f, 683, 384, 1.0, 0.0f};
		zoomImageSpr->TweenTo(valArr, 0.55f, TW_EASEQUADOUT, true, CC_CALLBACK_0(ExtrasMenuControl::onZoomImageTweenCompleted,this));

		zoomDarkBG->FadeIn(4.0f);

		zoomState = eZoomingIn;
	}
}

void ExtrasMenuControl::onZoomImageTweenCompleted()
{
	switch (zoomState)
	{
	case eZoomingIn:
		zoomState = eZoomIdle;
		break;
	case eZoomingOut:
		zoomState = eZoomInactive;

		SAFE_DELETE(zoomImageSpr);
		break;
	default:break;	
	}
}

void ExtrasMenuControl::UpdateExtrasPopup()
{
	if( PopupCounter == 0 )
	{
		PopupAlpha += 4.0f * Control::LogicRate;

		if( PopupAlpha >= 1.0f )
		{
			PopupCounter = 1;
			PopupAlpha = 1.0f;			
		}
		_maskSpr->SetAlpha(PopupAlpha);
		ExtrasPopupScene->SetSceneAlpha(PopupAlpha);
	}
	else if( PopupCounter == 1 )
	{
		
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				PopupCounter = 2;
			}
		#endif
		CObject *Button = ExtrasPopupScene->GetObjectByID("UICloseButton");	
		int buttonStatus = CheckPopupButton(Button);
		if( buttonStatus == ePopup_MouseOver )
		{
			Button->ForcePlayAnimation(1);
		}
		else
		{
			Button->ForcePlayAnimation(0);
		}
		if( buttonStatus == ePopup_Click )
		{
			PopupCounter++;
			return;
		}
		//else
		{
			CPoint mosPos = Control::Input->Mpos();
			for (int i = 0; i < 8; i++)
			{
				if( ExtraItemsRect[i]->Intersection(&mosPos)  
				#ifdef TOUCH_UI
				&& i!=5
                #endif
				)
				{

					if( (PrevCursorMode != CA_HandPointy && Cursor::GetMode() != CA_HandPointy) || LastSelectedExtrasBtn != i )										
					{
						Control::Audio->PlaySample(aSFXRefill2);//don't play again!!						
						LastSelectedExtrasBtn = i;
					}

					Cursor::SetMode(CA_HandPointy);

					if(ExtrasOver[i]->GetAnimation() == 0)
					{
						ExtrasOver[i]->ForcePlayAnimation(1);
						//ExtrasOver[i]->FadeIn(4.0f);
					}
					// 0 - Concept Art
					// 1 - Cut Scenes
					// 2 - Minigames
					// 3 - Memories
					// 4 - HO Puzzles
					// 5 - Wallpapaers
					// 6 - Music
					// 7 - Achievements
					if(i != 3 && i != 7 )
					{
						if( Control::Input->LBclicked())
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);

							#ifdef _DEMOBUILD //_DEMOBUILD
								_showMessageBannerCallback(StringTable::getStr("Exbonus"),0);
								return;
							#endif
							
							#ifdef K_ANDROID
								if (!GFApp->isUnlocked()) //Free Build
									{
									_showMessageBannerCallback(StringTable::getStr("Exbonus"),0);
									return;
									}
							#endif
							#ifndef _CEBUILD //SE BUILD
								_showMessageBannerCallback(StringTable::getStr("Exbonus"),0);
								return;
							#endif 

							switch (i)
							{
							case 0:	SelectedExtraPageScene	= ExtraPage_Concept_Art;
								break;
							case 1:	SelectedExtraPageScene	= ExtraPage_CutScenes;
								break;
							case 2:	SelectedExtraPageScene	= ExtraPage_MiniGames;
								break;
							case 4:	SelectedExtraPageScene	= ExtraPage_HOScenes;
								break;
							case 5:	SelectedExtraPageScene	= ExtraPage_Wallpapaers;
								break;
							case 6: SelectedExtraPageScene = ExtraPage_Music;
								break;
							default:
								break;
							}
							IsExtrasItemSelected = true;
							PopupCounter++;

							return;
						}
					}
					//Memories
					else if(i==3)
					{
						if(Control::Input->LBclicked())
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
							if(_kidroomAreaCallback!=nullptr)
								_kidroomAreaCallback();
							break;
						}
					}
					else if(i==7)
					{
						if(Control::Input->LBclicked())
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
							
							#ifdef _DEMOBUILD //_DEMOBUILD
								_showMessageBannerCallback(StringTable::getStr("Exbonus"),0);
								return;
							#endif
							#ifdef K_ANDROID
								if (!GFApp->isUnlocked()) //Free Build
									{
									_showMessageBannerCallback(StringTable::getStr("Exbonus"),0);
									return;
									}
							#endif
							#ifndef _CEBUILD //SE BUILD
								_showMessageBannerCallback(StringTable::getStr("Exbonus"),0);
								return;
							#endif 

							if(_AwardsAreaCallback!=nullptr)
								_AwardsAreaCallback();
							break;
						}
					}					
				}
				else
				{
					ExtrasOver[i]->ForcePlayAnimation(0);
				}
			}

			if( comicBtn->Collide(&mosPos) )
			{
				Cursor::SetMode(CA_HandPointy);
				comicBtn->PlayAnimation(0);
				if(Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXCrystal);
					//if(ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted)
					{
						isComicBookClicked = true;
						PopupCounter += 1;
					}
					/*else
					{
						_showMessageBannerCallback("Complete the main chapter to unlock the Comic Book.",0);
					}*/
				}
			}
			else
			{
				comicBtn->PlayAnimation(1);
			}

			if (creditsBtn->Collide(&mosPos))
			{
				Cursor::SetMode(CA_HandPointy);
				creditsBtn->PlayAnimation(0);
				if (Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXCrystal);
					if (_CreditsCallback != nullptr)
						_CreditsCallback();
				}
			}
			else
			{
				creditsBtn->PlayAnimation(1);
			}
		}
	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 4.0f * Control::LogicRate;

		achievementsTxt->Show(false);
		minigamesTxt->Show(false);
		memoriesTxt->Show(false);
		conceptsTxt->Show(false);
		cutscenesTxt->Show(false);
		musicTxt->Show(false);
		hopuzzlesTxt->Show(false);
#ifndef TOUCH_UI
		wallpapersTxt->Show(false);
#endif

		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;	
			PopupCounter++;
			PopupState = Popup_Disabled;
			_maskSpr->Show(false);
			LastSelectedExtrasBtn = -1;

			ExtrasPopupScene->Show(false);
			if( IsExtrasItemSelected )
			{
				PopulateExtraPopupItems(SelectedExtraPageScene);
				IsExtrasItemSelected = false;
			}
			else
			{
				ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId = -1;
			}

			if(isComicBookClicked)
			{
				isComicBookClicked = false;
				if(_LoadComicCallback!=nullptr)
					_LoadComicCallback();
			}
		}
		else
		{
			_maskSpr->SetAlpha(PopupAlpha);
			ExtrasPopupScene->SetSceneAlpha(PopupAlpha);
		}
	} 
}

int ExtrasMenuControl::CheckPopupButton(CObject *Button)
{
	if( Button->GetAnimation() == eBtn_Locked )
		return ePopup_NoInteraction;

	CPoint mosPos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	if(Button->Collide(&mosPos))
	{
		Cursor::SetMode(CA_HandPointy);
		if(clicked )
		{
			Button->PlayAnimation(eBtn_Click);
			Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
			return ePopup_Click;
		}
		else
		{
			if( Button->GetAnimation() != eBtn_Hover )
			{
				Control::Audio->PlaySample(aSFXRefill3);				
				Button->PlayAnimation(eBtn_Hover);
			}
			return ePopup_MouseOver;
		}
	}
	else
		Button->PlayAnimation( eBtn_Idle );

	return ePopup_NoInteraction;
}

void ExtrasMenuControl::PopulateExtraPopupItems(eExtrasPages item)
{
	ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId = item;
	switch (item)
	{
	case ExtraPage_Concept_Art:
		PopupState = Popup_Extras_ConceptArt;
		ExtrasPageSize = 2;
		SCN_Concepts->Show(true);
		SCN_Concepts->SetSceneAlpha(0.0f);
		SCN_Concepts->GetObjectByID("Titles")->ForcePlayAnimation(0);
		SCN_Concepts->GetObjectByID("PrevButton")->SetEnable(false);
		SCN_Concepts->GetObjectByID("NextButton")->SetEnable(true);
		//SCN_Concepts->GetObjectByID("NextButton")->SetEnable(false);
		CurrentPageText->SetText("1/2");
		for(int i = 0; i < 6; i++)
		{
			SCN_Concepts->GetObjectByID(std::string("ConceptArt" + MKSTR(i)))->ForcePlayAnimation(i);	
		}
		break;
	case ExtraPage_CutScenes:	
		PopupState = Popup_Extras_CutScenes;
		ExtrasPageSize = 5;
		SCN_CutScenes->Show(true);
		SCN_CutScenes->SetSceneAlpha(0.0f);
		SCN_CutScenes->GetObjectByID("Titles")->ForcePlayAnimation(1);
		SCN_CutScenes->GetObjectByID("PrevButton")->SetEnable(false);
		SCN_CutScenes->GetObjectByID("NextButton")->SetEnable(true);
		CurrentPageText->SetText("1/5");
		for(int i = 0; i < 6; i++)  
		{
			SCN_CutScenes->GetObjectByID(std::string("CutScenes" + MKSTR(i)))->ForcePlayAnimation(i);	
		}
		break;
	case ExtraPage_HOScenes:	
		PopupState = Popup_Extras_HOScenes;
		ExtrasPageSize = 3;
		SCN_HOScenes->Show(true);
		SCN_HOScenes->SetSceneAlpha(0.0f);
		SCN_HOScenes->GetObjectByID("Titles")->ForcePlayAnimation(3);
		SCN_HOScenes->GetObjectByID("PrevButton")->SetEnable(false);
		SCN_HOScenes->GetObjectByID("NextButton")->SetEnable(true);
		CurrentPageText->SetText("1/3");
		for(int i = 0; i < 6; i++)
		{
			SCN_HOScenes->GetObjectByID(std::string("HOScenes" + MKSTR(i)))->ForcePlayAnimation(i);	
		}
		break;
	case ExtraPage_Wallpapaers:	
		ExtrasPageSize = 11;
		SCN_Wallpapaers->Show(true);
		SCN_Wallpapaers->SetSceneAlpha(0.0f);
		PopupState = Popup_Extras_Wallpapers;
		SCN_Wallpapaers->GetObjectByID("Titles")->ForcePlayAnimation(4);
		SCN_Wallpapaers->GetObjectByID("PrevButton")->SetEnable(false);
		SCN_Wallpapaers->GetObjectByID("NextButton")->SetEnable(true);

		// Sac: Hide all the Wallpaper saving buttons until we figure out a way to save wallpapers in Android
		#if (defined K_ANDROID || defined K_iOS)
			SCN_Wallpapaers->GetObjectByID("WSaveButton01")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WSaveButton02")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WSaveButton03")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WSaveButton11")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WSaveButton12")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WSaveButton13")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WSaveButton21")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WSaveButton22")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WSaveButton23")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WallpaperSizeText1")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WallpaperSizeText2")->SetEnable(false);
			SCN_Wallpapaers->GetObjectByID("WallpaperSizeText3")->SetEnable(false);
		#endif
		CurrentPageText->SetText("1/11");
		for(int i = 0; i < 3; i++)
		{
			SCN_Wallpapaers->GetObjectByID(std::string("Wallpapers" + MKSTR(i)))->ForcePlayAnimation(i);	
		}
		break;
	case ExtraPage_Music:
		ExtrasPageSize = 1;
		SCN_Music->Show(true);
		SCN_Music->SetSceneAlpha(0.0f);
		SCN_Music->GetObjectByID("Titles")->ForcePlayAnimation(5);
		PopupState = Popup_Extras_Music;
		CurrentPageText->Show(false);
		break;
	case ExtraPage_MiniGames:
		PopupState = Popup_Extras_MiniGames;
		ExtrasPageSize = 6;
		SCN_MiniGames->Show(true);
		SCN_MiniGames->SetSceneAlpha(0.0f);
		SCN_MiniGames->GetObjectByID("Titles")->ForcePlayAnimation(2);
		SCN_MiniGames->GetObjectByID("PrevButton")->SetEnable(false);
		SCN_MiniGames->GetObjectByID("NextButton")->SetEnable(true);
		CurrentPageText->SetText("1/6");
		for(int i = 0; i < 6; i++)
		{
			SCN_MiniGames->GetObjectByID(std::string("MiniGames" + MKSTR(i)))->ForcePlayAnimation(i);	
		}
		break;
	default:
		break;
	}
	if( item != ExtraPage_Music)
	{
		CurrentPageText->Show(true);
		CurrentPageText->SetAlpha(0.0f);
		CurrentPageText->SetColor(1.0f,1.0f,1.0f);
	}
	
	PopupCounter = 0;
	PopupAlpha = 0.0f;
	pageIndex = 0;
	UpdateUnlockStatus();
}

void ExtrasMenuControl::UpdateExtrasItemPagePopup(eExtrasPages currentPage )
{
	if( PopupCounter == 0 )
	{
		PopupAlpha += 4.0f * Control::LogicRate;

		if( PopupAlpha >= 1.0f )
		{
			PopupCounter = 1;
			PopupAlpha = 1.0f;
		}
		_maskSpr->SetAlpha(PopupAlpha);
		CurrentPageText->SetAlpha(PopupAlpha);
		switch (currentPage )
		{
		case ExtraPage_Concept_Art:
			SCN_Concepts->SetSceneAlpha(PopupAlpha);
			break;
		case ExtraPage_CutScenes:	
			SCN_CutScenes->SetSceneAlpha(PopupAlpha);
			break;
		case ExtraPage_HOScenes:	
			SCN_HOScenes->SetSceneAlpha(PopupAlpha);
			break;
		case ExtraPage_MiniGames:	
			SCN_MiniGames->SetSceneAlpha(PopupAlpha);
			break;
		case ExtraPage_Wallpapaers:	
			SCN_Wallpapaers->SetSceneAlpha(PopupAlpha);
			break;
		case ExtraPage_Music:	
			SCN_Music->SetSceneAlpha(PopupAlpha);
			break;
		default:
		break;
		}
		
	}
	else if( PopupCounter == 1 )
	{
		CObject *PrevButton;
		CObject *NextButton;
		CObject *Button;
		CObject *BackButton;

		
		switch (currentPage )
		{
		case ExtraPage_Concept_Art:
				PrevButton = SCN_Concepts->GetObjectByID("PrevButton");	
				NextButton = SCN_Concepts->GetObjectByID("NextButton");	
				Button	   = SCN_Concepts->GetObjectByID("UICloseButton");	
				BackButton = SCN_Concepts->GetObjectByID("BackButton");
			break;
		case ExtraPage_CutScenes:	
				PrevButton = SCN_CutScenes->GetObjectByID("PrevButton");	
				NextButton = SCN_CutScenes->GetObjectByID("NextButton");	
				Button	   = SCN_CutScenes->GetObjectByID("UICloseButton");	
				BackButton = SCN_CutScenes->GetObjectByID("BackButton");
			break;
		case ExtraPage_HOScenes:	
				PrevButton = SCN_HOScenes->GetObjectByID("PrevButton");	
				NextButton = SCN_HOScenes->GetObjectByID("NextButton");	
				Button	   = SCN_HOScenes->GetObjectByID("UICloseButton");	
				BackButton = SCN_HOScenes->GetObjectByID("BackButton");
			break;
		case ExtraPage_MiniGames:	
				PrevButton = SCN_MiniGames->GetObjectByID("PrevButton");	
				NextButton = SCN_MiniGames->GetObjectByID("NextButton");	
				Button	   = SCN_MiniGames->GetObjectByID("UICloseButton");	
				BackButton = SCN_MiniGames->GetObjectByID("BackButton");
			break;
		case ExtraPage_Wallpapaers:	
				PrevButton = SCN_Wallpapaers->GetObjectByID("PrevButton");	
				NextButton = SCN_Wallpapaers->GetObjectByID("NextButton");	
				Button	   = SCN_Wallpapaers->GetObjectByID("UICloseButton");	
				BackButton = SCN_Wallpapaers->GetObjectByID("BackButton");
			break;
		case ExtraPage_Music:	
				Button	   = SCN_Music->GetObjectByID("UICloseButton");	
				BackButton = SCN_Music->GetObjectByID("BackButton");
			break;
		default:
		break;
		}
		int buttonStatus;
		//prev button input check
		if (currentPage != ExtraPage_Music && pageIndex > 0 )
		{
			buttonStatus = CheckPopupButton(PrevButton);
			if( buttonStatus == ePopup_Click )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
				pageIndex--;
				UpdateUnlockStatus();
				CurrentPageText->SetText(MKSTR(pageIndex + 1) + "/" + MKSTR( ExtrasPageSize) );
				NextButton->SetEnable(true);
				switch (currentPage )
				{
				case ExtraPage_Concept_Art:	
					for(int i = 0; i < 6; i++)
					{
						SCN_Concepts->GetObjectByID(std::string("ConceptArt" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 6 ));	
					}
					break;
				case ExtraPage_CutScenes:	
					for(int i = 0; i < 6; i++)
					{
						SCN_CutScenes->GetObjectByID(std::string("CutScenes" + MKSTR(i)))->SetEnable(true);
						SCN_CutScenes->GetObjectByID(std::string("CutScenes" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 6 ));	
					}
					break;
				case ExtraPage_HOScenes:	
					for(int i = 0; i < 6; i++)
					{
						SCN_HOScenes->GetObjectByID(std::string("HOScenes" + MKSTR(i)))->SetEnable(true);
						SCN_HOScenes->GetObjectByID(std::string("HOScenes" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 6 ));	
					}
					break;
				case ExtraPage_MiniGames:	
					for(int i = 0; i < 6; i++)
					{
						SCN_MiniGames->GetObjectByID(std::string("MiniGames" + MKSTR(i)))->SetEnable(true);
						SCN_MiniGames->GetObjectByID(std::string("MiniGames" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 6 ));	
					}
					break;
				case ExtraPage_Wallpapaers:	
					for(int i = 0; i < 3; i++)
					{
						SCN_Wallpapaers->GetObjectByID(std::string("Wallpapers" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 3 ));	
					}
					break;
				default:
					break;
				}
				if( pageIndex <= 0 )
				{
					PrevButton->SetEnable(false);
				} 
			} 
		}
		//Next button Input check
		if (currentPage != ExtraPage_Music && pageIndex < ExtrasPageSize - 1)
		{
			buttonStatus = CheckPopupButton(NextButton);
			if( buttonStatus == ePopup_Click )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
				pageIndex++;
				UpdateUnlockStatus();
				PrevButton->SetEnable(true);
				CurrentPageText->SetText(MKSTR(pageIndex + 1) + "/" + MKSTR( ExtrasPageSize));
				switch (currentPage )
				{
				case ExtraPage_Concept_Art:	
					for(int i = 0; i < 6; i++)
					{
						SCN_Concepts->GetObjectByID(std::string("ConceptArt" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 6 ));	
					}
					break;
				case ExtraPage_CutScenes:	
					for(int i = 0; i < 6; i++)
					{
						if(pageIndex >= 4 && i >= 3 )
						{
							SCN_CutScenes->GetObjectByID(std::string("CutScenes" + MKSTR(i)))->SetEnable(false);
							SCN_CutScenes->GetObjectByID(std::string("Lock" + MKSTR(i)))->SetEnable(false);
						}
						else 
						SCN_CutScenes->GetObjectByID(std::string("CutScenes" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 6 ));	
					}
					break;
				case ExtraPage_HOScenes:	
					for(int i = 0; i < 6; i++)
					{
						if(pageIndex >= 2 && i == 5 )
						{
							SCN_HOScenes->GetObjectByID(std::string("HOScenes" + MKSTR(i)))->SetEnable(false);
							SCN_HOScenes->GetObjectByID(std::string("Lock" + MKSTR(i)))->SetEnable(false);
						}
						else 
							SCN_HOScenes->GetObjectByID(std::string("HOScenes" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 6 ));	
					}
					break;
				case ExtraPage_MiniGames:	
					for(int i = 0; i < 6; i++)
					{
						if(pageIndex >= 5 && i > 1 )
						{
							SCN_MiniGames->GetObjectByID(std::string("MiniGames" + MKSTR(i)))->SetEnable(false);
							SCN_MiniGames->GetObjectByID(std::string("Lock" + MKSTR(i)))->SetEnable(false);
						}
						else
							SCN_MiniGames->GetObjectByID(std::string("MiniGames" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 6 ));	
					}
					break;
				case ExtraPage_Wallpapaers:	
					for(int i = 0; i < 3; i++)
					{
						SCN_Wallpapaers->GetObjectByID(std::string("Wallpapers" + MKSTR(i)))->ForcePlayAnimation(i + (pageIndex * 3 ));	
					}
					break;
				default:
					break;
				}
				if( pageIndex >= ExtrasPageSize - 1)
				{
					NextButton->SetEnable(false);
				} 
			} 
		}
		
		if(currentPage == ExtraPage_Music)
		{
			CPoint pos = Control::Input->Mpos();
			bool clicked = Control::Input->LBclicked();

			CObject *obj_PlayBtn = SCN_Music->GetObjectByID("SoundPlayButton"); 
			CObject *obj_StopBtn = SCN_Music->GetObjectByID("SoundStopButton"); 
			CObject *obj_NextTrackBtn = SCN_Music->GetObjectByID("SoundForwardButton"); 
			CObject *obj_PrevTrackBtn = SCN_Music->GetObjectByID("SoundPrevButton"); 

			obj_PlayBtn->PlayAnimation(0);
			obj_StopBtn->PlayAnimation(0);
			obj_NextTrackBtn->PlayAnimation(0);
			obj_PrevTrackBtn->PlayAnimation(0);

				
			if( selectedMusicID != -1 )
				MusicTrackGlows[selectedMusicID]->SetEnable(true);
			if(MusicBtn_PlayTrack.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				obj_PlayBtn->PlayAnimation(1);
				if(clicked)
				{
					Control::Audio->PlaySample(aSFXClick);
					//Play Music
					if(selectedMusicID==-1) //No music selected. Play First One
					{
						selectedMusicID = 0;
						MusicTrackGlows[selectedMusicID]->SetEnable(true);
						SaveMusicTrack(selectedMusicID);						
					}
										

					//Stop Previous Music
					if(currentMusicPlayingID!=-1)
					{
						MusicTrackGlows[currentMusicPlayingID]->SetEnable(false);
						Control::Audio->StopSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));
						Control::Audio->UnloadSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));
					}

					//Play Music i
					isExtrasMusicPlaying = true;
					currentMusicPlayingID = selectedMusicID;

					if( Control::Audio->IsPlaying(aTrackMenu) )
						Control::Audio->StopSample(aTrackMenu);

					Control::Audio->LoadSample(eAudio(MusicTrackIDs[currentMusicPlayingID]), AudioTrack,true);
					Control::Audio->PlaySample(eAudio(MusicTrackIDs[currentMusicPlayingID]), true);
				}
				return;
			}

			else if(MusicBtn_StopTrack.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				obj_StopBtn->PlayAnimation(1);
				if(clicked)
				{
					Control::Audio->PlaySample(aSFXClick);
					//Stop Playing Music
					if(currentMusicPlayingID!=-1)
					{
						//MusicTrackGlows[currentMusicPlayingID]->SetEnable(false);
						Control::Audio->StopSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));
						Control::Audio->UnloadSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));

						currentMusicPlayingID = -1;
					}
				}
				return;
			}

			else if(MusicBtn_NextTrack.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				obj_NextTrackBtn->PlayAnimation(1);
				if(clicked)
				{
					Control::Audio->PlaySample(aSFXClick);
					
					if(selectedMusicID==-1) //No music selected. Play First One
					{
						selectedMusicID = 0;
						
					}
					else
					{
						MusicTrackGlows[selectedMusicID]->SetEnable(false);

						selectedMusicID += 1;
						if(selectedMusicID > (MAX_MUSIC_TRACKS-1))
							selectedMusicID = 0;
						
					}
					SaveMusicTrack(selectedMusicID);
					MusicTrackGlows[selectedMusicID]->SetEnable(true);
					//Stop Previous Music
					if(currentMusicPlayingID!=-1)
					{
						MusicTrackGlows[currentMusicPlayingID]->SetEnable(false);
						Control::Audio->StopSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));
						Control::Audio->UnloadSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));
					}


					//Play Music i
					isExtrasMusicPlaying = true;
					currentMusicPlayingID = selectedMusicID;

					if( Control::Audio->IsPlaying(aTrackMenu) )
						Control::Audio->StopSample(aTrackMenu);

					Control::Audio->LoadSample(eAudio(MusicTrackIDs[currentMusicPlayingID]), AudioTrack,true);
					Control::Audio->PlaySample(eAudio(MusicTrackIDs[currentMusicPlayingID]), true);

				}
				return;
			}

			else if(MusicBtn_PrevTrack.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				obj_PrevTrackBtn->PlayAnimation(1);
				if(clicked)
				{
					Control::Audio->PlaySample(aSFXClick);
					
					if(selectedMusicID==-1) //No music selected. Play First One
					{
						selectedMusicID = 0;
						
					}
					else
					{
						MusicTrackGlows[selectedMusicID]->SetEnable(false);

						selectedMusicID -= 1;
						if(selectedMusicID < 0)
							selectedMusicID = (MAX_MUSIC_TRACKS-1);
					}
					SaveMusicTrack(selectedMusicID);
					MusicTrackGlows[selectedMusicID]->SetEnable(true);

					//Stop Previous Music
					if(currentMusicPlayingID!=-1)
					{
						MusicTrackGlows[currentMusicPlayingID]->SetEnable(false);
						Control::Audio->StopSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));
						Control::Audio->UnloadSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));
					}

					//Play Music i
					isExtrasMusicPlaying = true;
					currentMusicPlayingID = selectedMusicID;

					if( Control::Audio->IsPlaying(aTrackMenu) )
						Control::Audio->StopSample(aTrackMenu);

					Control::Audio->LoadSample(eAudio(MusicTrackIDs[currentMusicPlayingID]), AudioTrack,true);
					Control::Audio->PlaySample(eAudio(MusicTrackIDs[currentMusicPlayingID]), true);
				}
				return;
			}

//const CRectangle  MusicBtn_PlayTrack(566.0f,514.0f,104.0f,107.0f);
//const CRectangle  MusicBtn_StopTrack(677.0f,528.0f,85.0f,82.0f);
//const CRectangle  MusicBtn_NextTrack(771.0f,529.0f,83.0f,80.0f);
//const CRectangle  MusicBtn_PrevTrack(474.0f,527.0f,84.0f,85.0f);
			//Update Music Page
			bool isMouseOverTitle = false;
			for (int i = 0; i < MAX_MUSIC_TRACKS; i++)
			{
				if(selectedMusicID!=i)
				{
					MusicTrackGlows[i]->SetEnable(false);
					//if(MusicTrackRects[i]->Intersection(&Control::Input->Mpos()))
					// Linux: gives error of taking address of temporary also potential crash factor
					// below is fix
					CPoint mousePos = Control::Input->Mpos();
					if(MusicTrackRects[i]->Intersection(&mousePos))
					{
						isMouseOverTitle = true;
						Cursor::SetMode(CA_HandPointy);
						MusicTrackGlows[i]->SetEnable(true);
						if(Control::Input->LBclicked())
						{
							//Select Music i							
							selectedMusicID = i;
							SaveMusicTrack(selectedMusicID);
						}
					}
				}
				else
					MusicTrackGlows[i]->SetEnable(true);
			}

			if(isMouseOverTitle)
				return;
		}

		//close button input check
		buttonStatus = CheckPopupButton(Button);


		if( buttonStatus == ePopup_MouseOver )
		{
			Button->ForcePlayAnimation(1);
		}
		else
		{
			Button->ForcePlayAnimation(0);
		}
		if( buttonStatus == ePopup_Click )
		{
			ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId = -1;
			Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
			PopupCounter++;

			if(currentPage == ExtraPage_Music)
			{
				if(currentMusicPlayingID != -1 && isExtrasMusicPlaying)
				{
					//Stop Playing
					if( Control::Audio->IsPlaying(eAudio(MusicTrackIDs[currentMusicPlayingID])) )
						Control::Audio->StopSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));

					isExtrasMusicPlaying = false;
					currentMusicPlayingID = -1;

					Control::Audio->LoadSample(aTrackMenu, AudioTrack,true);

					if( !Control::Audio->IsPlaying(aTrackMenu) )
						Control::Audio->PlaySample(aTrackMenu, true);
				}
				
			}
		}

		//Popup is active
		//Actual popup updation goes here.
		//check for back button

		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId = -1;
				Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
				if (currentPage !=Popup_Extras) // if it is Main Extras Popup, then it is not a Back button, it is Exit Button 
					isBackButtonSelected = true;
				PopupCounter++;

				if(currentPage == ExtraPage_Music)
				{
					if(currentMusicPlayingID != -1 && isExtrasMusicPlaying)
					{
						//Stop Playing
						if( Control::Audio->IsPlaying(eAudio(MusicTrackIDs[currentMusicPlayingID])) )
							Control::Audio->StopSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));

						isExtrasMusicPlaying = false;
						currentMusicPlayingID = -1;

						Control::Audio->LoadSample(aTrackMenu, AudioTrack,true);

						if( !Control::Audio->IsPlaying(aTrackMenu) )
							Control::Audio->PlaySample(aTrackMenu, true);
					}
					
				}
			}
		#endif

		buttonStatus = CheckPopupButton(BackButton);
		if( buttonStatus == ePopup_MouseOver )
		{
			BackButton->ForcePlayAnimation(1);
		}
		else
		{
			BackButton->ForcePlayAnimation(0);
		}
		if( buttonStatus == ePopup_Click )
		{
			ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId = -1;
			Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
			isBackButtonSelected = true;
			PopupCounter++;

			if(currentPage == ExtraPage_Music)
			{
				if(currentMusicPlayingID != -1 && isExtrasMusicPlaying)
				{
					//Stop Playing
					if( Control::Audio->IsPlaying(eAudio(MusicTrackIDs[currentMusicPlayingID])) )
						Control::Audio->StopSample(eAudio(MusicTrackIDs[currentMusicPlayingID]));

					isExtrasMusicPlaying = false;
					currentMusicPlayingID = -1;

					Control::Audio->LoadSample(aTrackMenu, AudioTrack,true);

					if( !Control::Audio->IsPlaying(aTrackMenu) )
						Control::Audio->PlaySample(aTrackMenu, true);
				}
				
			}
		}

	}
	else if( PopupCounter == 2 )
	{
		PopupAlpha -= 4.0f * Control::LogicRate;		

		if( PopupAlpha <= 0.0f )
		{
			PopupCounter++;
			_maskSpr->Show(false);
			LastSelectedExtrasBtn = -1;
			switch (currentPage )
			{
			case ExtraPage_Concept_Art:
				SCN_Concepts->Show(false);
				break;
			case ExtraPage_CutScenes:	
				SCN_CutScenes->Show(false);
				break;
			case ExtraPage_HOScenes:	
				SCN_HOScenes->Show(false);
				break;
			case ExtraPage_MiniGames:	
				SCN_MiniGames->Show(false);
				break;
			case ExtraPage_Wallpapaers:	
				SCN_Wallpapaers->Show(false);
				break;
			case ExtraPage_Music:	
				SCN_Music->Show(false);
				break;
			default:
			break;
			}
			
			if( isBackButtonSelected)
			{
				ShowExtrasPopup();
			}
			else
			{
				PopupState = Popup_Disabled;
			}
		}
		else
		{
			if( !isBackButtonSelected)
			{
				_maskSpr->SetAlpha(PopupAlpha);
			}
			CurrentPageText->SetAlpha(PopupAlpha);
			switch (currentPage )
			{
			case ExtraPage_Concept_Art:
				SCN_Concepts->SetSceneAlpha(PopupAlpha);
				break;
			case ExtraPage_CutScenes:	
				SCN_CutScenes->SetSceneAlpha(PopupAlpha);
				break;
			case ExtraPage_HOScenes:	
				SCN_HOScenes->SetSceneAlpha(PopupAlpha);
				break;
			case ExtraPage_MiniGames:	
				SCN_MiniGames->SetSceneAlpha(PopupAlpha);
				break;
			case ExtraPage_Wallpapaers:	
				SCN_Wallpapaers->SetSceneAlpha(PopupAlpha);
				break;
			case ExtraPage_Music:	
				SCN_Music->SetSceneAlpha(PopupAlpha);
				break;
			default:
			break;
			}
		}
	}
}



void ExtrasMenuControl::SaveMusicTrack(int itemId)
{

	#if(defined K_WIN32 && !defined _STEAM_BUILD)
		char *filePathChar = KMiscTools::makeFilePath("Assets\\");
		std::string srcFilePath(filePathChar);	

		srcFilePath += MKSTR(AudioFiles[MusicTrackIDs[itemId]]);
		std::string destFilename = "\\TRACK" + MKSTR(itemId)+"_"+MKSTR(MusicTracks[itemId]);
		
		
		

		//get Documents Folder
		CHAR myDocumentsPath[MAX_PATH];
		HRESULT result = 0; 
		
		//////////////////////////

		
			OSVERSIONINFOEXA vi;
			/* Check Windows version */
			ZeroMemory (&vi, sizeof (vi));
			vi.dwOSVersionInfoSize = sizeof (vi);
			GetVersionExA ((OSVERSIONINFOA *) &vi);		  
			switch (vi.dwMajorVersion) 
			{
				default:
				/* 4 - DOS based Windows: 95, 98 or ME */				  
				/* 5 - NT kernel based Windows 5.x: Win2000,Windows Server 2003 or XP */
					//C:\Documents and Settings\USERNAME\Application Data\STUDIONAME\GAMENAME
					result = SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocumentsPath);
					break;
				case 6:
						/* Vista,Windows Server 2008,Windows 7,Windows Server 2008 RC2 */
						//C:\Users\ USERNAME\AppData\Roaming\STUDIONAME\GAMENAME
						//if (vi.dwMinorVersion == 2 ) //windows 10 machine detected!!
						{
							K_LOG("Windows Vista onwards machine detected");
							LPWSTR roamingPath;
							result = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &roamingPath);						
							strcpy(myDocumentsPath,(FileSystem::ws2s(roamingPath)).c_str());						
							if (SUCCEEDED(result))
								K_LOG(myDocumentsPath);
							
						}
						
					break;  
			}
		
		//	#else
		//		strcpy(myDocumentsPath, KMiscTools::getUserFolder());		
		//	#endif
		
		//////////////////////////
		//	#ifdef K_WIN32
			std::string SaveFolder = myDocumentsPath + std::string("\\Darkarta - A Broken Heart's Quest");
		//	#else
		//		std::string SaveFolder = myDocumentsPath + std::string("//Darkarta - A Broken Heart's Quest");
		//	#endif
		if( !KMiscTools::fileExists(SaveFolder.c_str()) )
			KMiscTools::createDirectory(SaveFolder.c_str());//Game store directory
		//////////////////////////

		std::string destinationPath =  SaveFolder+destFilename;
		
		LPCSTR lpSrcFilePtr = srcFilePath.c_str();
		LPCSTR lpDestFilePtr = destinationPath.c_str();

		BOOL CopyStats = CopyFileA(lpSrcFilePtr, lpDestFilePtr, true);
		K_LOG("COPYSTATS %s",(CopyStats == TRUE)?"true":"false");
		if(GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			K_LOG("Error File not found");		
		}
		else
		{
			K_LOG("sourcefile %s", srcFilePath.c_str());
			K_LOG("destFile %s", destinationPath.c_str());
		}

		if( CopyStats == FALSE &&
			(GetLastError() == ERROR_PATH_NOT_FOUND || GetLastError() == ERROR_ACCESS_DENIED ) )//as fallback option trying to save in game folder!!!
		{
			std::string gameFolderPath = KMiscTools::getGameFolder();		 
			std::string MusicTrackPath = gameFolderPath+destFilename;
			LPCSTR lpDestFilePtr2 = MusicTrackPath.c_str();
			K_LOG("%s",MusicTrackPath.c_str());
			std::ifstream srce( lpSrcFilePtr, std::ios::binary ) ;
			std::ofstream dest( lpDestFilePtr2, std::ios::binary ) ;
			dest << srce.rdbuf() ;		
		}
		
		std::string saveMsg = StringTable::getStr("Exmusic")+ SaveFolder;
		if(_showMessageBannerCallback!=nullptr)
			_showMessageBannerCallback(saveMsg,0);

		//	#ifdef K_WIN32
			_flushall();//write all open output streams
	#endif
	
}

void ExtrasMenuControl::ShowExtrasPopup()
{

	PopupState = Popup_Extras;

	_maskSpr->SetAlpha(0.0f);
	_maskSpr->Show(true);

	achievementsTxt->Show(true);
	achievementsTxt->FadeInText(3);

	minigamesTxt->Show(true);
	minigamesTxt->FadeInText(3);

	memoriesTxt->Show(true);
	memoriesTxt->FadeInText(3);

	conceptsTxt->Show(true);
	conceptsTxt->FadeInText(3);

	cutscenesTxt->Show(true);
	cutscenesTxt->FadeInText(3);

	musicTxt->Show(true);
	musicTxt->FadeInText(3);

	hopuzzlesTxt->Show(true);
	hopuzzlesTxt->FadeInText(3);
#ifndef TOUCH_UI
	wallpapersTxt->Show(true);
	wallpapersTxt->FadeInText(3);
#endif

	ExtrasPopupScene->Show(true);
	for (int i = 0; i < 8; i++)
	{
		ExtrasOver[i]->ForcePlayAnimation(0);
	}

#ifdef TOUCH_UI
	ExtrasOver[5]->SetEnable(false);
#endif

	ExtrasPopupScene->SetSceneAlpha(0.0f);
	PopupAlpha = 0.0f;
	PopupCounter = 0;
	LastSelectedExtrasBtn = -1;

	/*
	if(ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId != -1 )
	{
		if(ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId != 3 && ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId != 7 )
		{
			Control::Audio->QuickLoadAndPlaySFX(aSFXMnuBTNClick);
			switch (ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId)
			{
			case 0:	SelectedExtraPageScene	= ExtraPage_Concept_Art;
				break;
			case 1:	SelectedExtraPageScene	= ExtraPage_CutScenes;
				break;
			case 2:	SelectedExtraPageScene	= ExtraPage_MiniGames;
				break;
			case 4:	SelectedExtraPageScene	= ExtraPage_HOScenes;
				break;
			case 5:	SelectedExtraPageScene	= ExtraPage_Wallpapaers;
				break;
			case 6: SelectedExtraPageScene = ExtraPage_Music;
				break;
			default:
				break;
			}
			PopulateExtraPopupItems(SelectedExtraPageScene);
			pageIndex = ProfileSystem::Instance->ActiveProfile.ExtrasCurrentPageId -1;
		}
	}
	*/

}

void ExtrasMenuControl::Show(bool noTransition)
{
	
}

void ExtrasMenuControl::Hide(bool noTransition)
{
	
}
