
//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "Credits.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"
#include "KPTK.h"

Credits::Credits()
{
	//Main Scene
	LoadScene("MNU\\CREDITS\\CREDITS.SCN");

	logo = new CObject();
	logo->LoadInit("MNU\\CREDITS\\LOGO.SPR", X, Y, 2001);
	logo->SetUID("logo");
	HOScene->PushObject(logo);

	TF_logo = new CObject();
	TF_logo->LoadInit("MNU\\CREDITS\\LOGO.SPR", 685, 1300, 2001);
	TF_logo->SetUID("TF_logo");
	TF_logo->PlayAnimation(0);
	HOScene->PushObject(TF_logo);

	CObject *Titlelogo = new CObject();
	Titlelogo->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\MENUTITLE.SPR", 683, 1000, 2005);
	Titlelogo->SetUID("Title"); 
#ifndef _CEBUILD
		//menu_title->PlayAnimation(1);
#endif // !_CEBUILD
	HOScene->PushObject(Titlelogo);

	scrollSpeed = -1.0f;

	_credictsStartDelay = 1.5f;


	Control::Audio->LoadSample(aTrackCredits,AudioTrack,true);

	if(!Control::Audio->IsPlaying(aTrackBonusCredit))
		Control::Audio->PlaySample(aTrackCredits,true);

	if (ProfileSystem::Instance->ActiveProfile.CreditsPrevArea != -1)
	{
		ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.CreditsPrevArea;
		ProfileSystem::Instance->ActiveProfile.CreditsPrevArea = -1;
	}
	GFInstance->PostUpdate(1);

	CreditsFliesFX = NULL;
	CreditsFliesFX = new SuperFX("PRT\\XTRA\\CreditsFlies.INI", PointSystem::CreateCPoint(695,661,0), 1105); 
	CreditsFliesFX->Start();

	CreditsSmokeFX = NULL;
	CreditsSmokeFX = new SuperFX("PRT\\XTRA\\CreditsSmoke.INI", PointSystem::CreateCPoint(667,456,0), 1100); 
	CreditsSmokeFX->Start();
	CreditsSmokeFX->AdvanceByTime(2.0f);

	pub = 0;

#ifndef K_ANDROID	 // not reading the Credits timer from INI file for android
	//Load from ini........
	KIniReader *iniReader;
	iniReader = new KIniReader ;
	iniReader->setIni(KMiscTools::makeFilePath("config.ini")) ;	
	pub = iniReader->getValue("CONFIG", "PUB", pub);
	delete iniReader; iniReader = NULL;

#else
		pub = 0;
#endif

	if (ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted)
	{
		ProfileSystem::Instance->ActiveProfile.IsBonusChapterUnlocked = true;
	}

	LoadTxt();

	_skipBtn = new UIButton("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\SKIPBTN.SPR", (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y, 20000, CC_CALLBACK_1(Credits::OnSkip, this));
	HOScene->PushObject(_skipBtn);

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_Credits]  = true;	

}

void Credits::Required()
{
	HiddenObjectSystem::Required();	//forced update
}

void Credits::Update()
{
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
		return;
#endif

	Cursor::SetMode(CA_Normal);
	CPoint mPos = Control::Input->Mpos();
	if (_skipBtn->GetWorldRect().Intersection(&mPos))
	{
		Cursor::SetMode(CA_HandPointy);
	}

	_credictsStartDelay -= Control::LogicRate;
	if (_credictsStartDelay > 0.0f)
		return;

	TranslateY();

	if (copyright->GetPos().y < -150) // Reached end
	{
		if (GFInstance->Flow == fRootMenu)
			SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
		else 
		{
			if (ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted)
			{
				AwardManager::GetInstance()->CompleteMainStory();
			}

			SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
		}

	}
}

void Credits::OnSkip(CObject* obj)
{
	scrollSpeed = -50;
}

void Credits::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		
	}
	else if( val == MAP_HUD_CLICK ) 
	{
	}
	else if( val == INVBOX_DROP ) 
	{		
		
	}
	else if( val == INV_CLOSE )
	{		


	}
}

Credits::~Credits()
{
	SAFE_DELETE(CreditsFliesFX);
	SAFE_DELETE(CreditsSmokeFX);

	if( Control::Audio->IsPlaying(aTrackCredits))
		Control::Audio->StopSample(aTrackCredits);

	 Control::Audio->UnloadSample(aTrackCredits);

	 if( Control::Audio->IsPlaying(aTrackBonusCredit))
	 {
		Control::Audio->StopSample(aTrackBonusCredit);
		Control::Audio->UnloadSample(aTrackBonusCredit);
	 }

	 SAFE_DELETE( gameProducers); SAFE_DELETE( creativeDirector); SAFE_DELETE( leadDesigner); SAFE_DELETE( artDirector); SAFE_DELETE( animationDirector); SAFE_DELETE( musicDirector); SAFE_DELETE( technicalLead); SAFE_DELETE( leadProgrammer); SAFE_DELETE( projectManagers);
	 SAFE_DELETE( story); SAFE_DELETE( emogamedes); SAFE_DELETE( script); SAFE_DELETE( srGamePro); SAFE_DELETE( srArtist); SAFE_DELETE( srAnim); SAFE_DELETE( leadVFX); SAFE_DELETE( trailer); SAFE_DELETE( sounddesign); SAFE_DELETE( music); SAFE_DELETE( voiceover); SAFE_DELETE( audioMix); SAFE_DELETE( QA); SAFE_DELETE( testLead);
	 SAFE_DELETE(testing); SAFE_DELETE(guide); SAFE_DELETE(_kanjiEng); SAFE_DELETE(addscript); SAFE_DELETE(addconsultant); SAFE_DELETE(addart); SAFE_DELETE(addvo); SAFE_DELETE(addmusic); SAFE_DELETE(addprogram); SAFE_DELETE(mobileport); SAFE_DELETE(linux_macport); SAFE_DELETE(localization_programming); SAFE_DELETE(language); SAFE_DELETE(digitalMarketing); SAFE_DELETE(growthHacking);
	 SAFE_DELETE( ksidc); SAFE_DELETE( ksum); SAFE_DELETE( infopark); SAFE_DELETE( gratitude); SAFE_DELETE( KFC); SAFE_DELETE( special); SAFE_DELETE( PUB); SAFE_DELETE( thankyou); SAFE_DELETE( copyright);
}

void Credits::LoadTxt()
{
	X = 685; Y = 1500;
	ColorRGB color = { 0.7668f, 0.8789f, 0.99f };
	ColorRGB headingColor = { 1.0f, 1.0f, 1.0f };

	float offset = 50;
	float lineSpacing = 30;
	int font = eFont_25;
	int headingFont = eFont_26;
	int zOrder = 2000;

	gameProducers = new MultilineTextBox(AssetsStringTable::getString("gameProducers"), X, Y, zOrder, color, font, lineSpacing);
	gameProducers->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	gameProducers->LineTexts[0]->LoadFont(headingFont);
	gameProducers->CenterText();
	Y += ((gameProducers->numTextLines + 1) * lineSpacing) + offset;

	creativeDirector = new MultilineTextBox(AssetsStringTable::getString("creativeDirector"), X, Y, zOrder, color, font, lineSpacing);
	creativeDirector->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	creativeDirector->LineTexts[0]->LoadFont(headingFont);
	creativeDirector->CenterText();
	Y += ((creativeDirector->numTextLines + 1) * lineSpacing) + offset;

	leadDesigner = new MultilineTextBox(AssetsStringTable::getString("leadDesigner"), X, Y, zOrder, color, font, lineSpacing);
	leadDesigner->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	leadDesigner->LineTexts[0]->LoadFont(headingFont);
	leadDesigner->CenterText();
	Y += ((leadDesigner->numTextLines + 1) * lineSpacing) + offset;

	artDirector = new MultilineTextBox(AssetsStringTable::getString("artDirector"), X, Y, zOrder, color, font, lineSpacing);
	artDirector->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	artDirector->LineTexts[0]->LoadFont(headingFont);
	artDirector->CenterText();
	Y += ((artDirector->numTextLines + 1) * lineSpacing) + offset;

	animationDirector = new MultilineTextBox(AssetsStringTable::getString("animationDirector"), X, Y, zOrder, color, font, lineSpacing);
	animationDirector->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	animationDirector->LineTexts[0]->LoadFont(headingFont);
	animationDirector->CenterText();
	Y += ((animationDirector->numTextLines + 1) * lineSpacing) + offset;

	musicDirector = new MultilineTextBox(AssetsStringTable::getString("musicDirector"), X, Y, zOrder, color, font, lineSpacing);
	musicDirector->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	musicDirector->LineTexts[0]->LoadFont(headingFont);
	musicDirector->CenterText();
	Y += ((musicDirector->numTextLines + 1) * lineSpacing) + offset;

	technicalLead = new MultilineTextBox(AssetsStringTable::getString("technicalLead"), X, Y, zOrder, color, font, lineSpacing);
	technicalLead->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	technicalLead->LineTexts[0]->LoadFont(headingFont);
	technicalLead->CenterText();
	Y += ((technicalLead->numTextLines + 1) * lineSpacing) + offset;

	leadProgrammer = new MultilineTextBox(AssetsStringTable::getString("leadProgrammer"), X, Y, zOrder, color, font, lineSpacing);
	leadProgrammer->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	leadProgrammer->LineTexts[0]->LoadFont(headingFont);
	leadProgrammer->CenterText();
	Y += ((leadProgrammer->numTextLines + 1) * lineSpacing) + offset;

	projectManagers = new MultilineTextBox(AssetsStringTable::getString("projectManagers"), X, Y, zOrder, color, font, lineSpacing);
	projectManagers->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	projectManagers->LineTexts[0]->LoadFont(headingFont);
	projectManagers->CenterText();
	Y += ((projectManagers->numTextLines + 1) * lineSpacing) + offset;

	story = new MultilineTextBox(AssetsStringTable::getString("story"), X, Y, zOrder, color, font, lineSpacing);
	story->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	story->LineTexts[0]->LoadFont(headingFont);
	story->CenterText();
	Y += ((story->numTextLines + 1) * lineSpacing) + offset;

	emogamedes = new MultilineTextBox(AssetsStringTable::getString("emogamedes"), X, Y, zOrder, color, font, lineSpacing);
	emogamedes->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	emogamedes->LineTexts[0]->LoadFont(headingFont);
	emogamedes->CenterText();
	Y += ((emogamedes->numTextLines + 1) * lineSpacing) + offset;

	script = new MultilineTextBox(AssetsStringTable::getString("script"), X, Y, zOrder, color, font, lineSpacing);
	script->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	script->LineTexts[0]->LoadFont(headingFont);
	script->CenterText();
	Y += ((script->numTextLines + 1) * lineSpacing) + offset;

	srGamePro = new MultilineTextBox(AssetsStringTable::getString("srGamePro"), X, Y, zOrder, color, font, lineSpacing);
	srGamePro->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	srGamePro->LineTexts[0]->LoadFont(headingFont);
	srGamePro->CenterText();
	Y += ((srGamePro->numTextLines + 1) * lineSpacing) + offset;

	srArtist = new MultilineTextBox(AssetsStringTable::getString("srArtist"), X, Y, zOrder, color, font, lineSpacing);
	srArtist->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	srArtist->LineTexts[0]->LoadFont(headingFont);
	srArtist->CenterText();
	Y += ((srArtist->numTextLines + 1) * lineSpacing) + offset;

	srAnim = new MultilineTextBox(AssetsStringTable::getString("srAnim"), X, Y, zOrder, color, font, lineSpacing);
	srAnim->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	srAnim->LineTexts[0]->LoadFont(headingFont);
	srAnim->CenterText();
	Y += ((srAnim->numTextLines + 1) * lineSpacing) + offset;

	leadVFX = new MultilineTextBox(AssetsStringTable::getString("leadVFX"), X, Y, zOrder, color, font, lineSpacing);
	leadVFX->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	leadVFX->LineTexts[0]->LoadFont(headingFont);
	leadVFX->CenterText();
	Y += ((leadVFX->numTextLines + 1) * lineSpacing) + offset;

	trailer = new MultilineTextBox(AssetsStringTable::getString("trailer"), X, Y, zOrder, color, font, lineSpacing);
	trailer->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	trailer->LineTexts[0]->LoadFont(headingFont);
	trailer->CenterText();
	Y += ((trailer->numTextLines + 1) * lineSpacing) + offset;

	sounddesign = new MultilineTextBox(AssetsStringTable::getString("sounddesign"), X, Y, zOrder, color, font, lineSpacing);
	sounddesign->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	sounddesign->LineTexts[0]->LoadFont(headingFont);
	sounddesign->CenterText();
	Y += ((sounddesign->numTextLines + 1) * lineSpacing) + offset;

	music = new MultilineTextBox(AssetsStringTable::getString("music_"), X, Y, zOrder, color, font, lineSpacing);
	music->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	music->LineTexts[0]->LoadFont(headingFont);
	music->CenterText();
	Y += ((music->numTextLines + 1) * lineSpacing) + offset;

	voiceover = new MultilineTextBox(AssetsStringTable::getString("voiceover"), X, Y, zOrder, color, font, lineSpacing);
	voiceover->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	voiceover->LineTexts[0]->LoadFont(headingFont);
	voiceover->CenterText();
	Y += ((voiceover->numTextLines + 1) * lineSpacing) + offset;

	audioMix = new MultilineTextBox(AssetsStringTable::getString("audioMix"), X, Y, zOrder, color, font, lineSpacing);
	audioMix->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	audioMix->LineTexts[0]->LoadFont(headingFont);
	audioMix->CenterText();
	Y += ((audioMix->numTextLines + 1) * lineSpacing) + offset;

	QA = new MultilineTextBox(AssetsStringTable::getString("QA"), X, Y, zOrder, color, font, lineSpacing);
	QA->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	QA->LineTexts[0]->LoadFont(headingFont);
	QA->CenterText();
	Y += ((QA->numTextLines + 1) * lineSpacing) + offset;

	testLead = new MultilineTextBox(AssetsStringTable::getString("testLead"), X, Y, zOrder, color, font, lineSpacing);
	testLead->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	testLead->LineTexts[0]->LoadFont(headingFont);
	testLead->CenterText();
	Y += ((testLead->numTextLines + 1) * lineSpacing) + offset;

	testing = new MultilineTextBox(AssetsStringTable::getString("testing"), X, Y, zOrder, color, font, lineSpacing);
	testing->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	testing->LineTexts[0]->LoadFont(headingFont);
	testing->CenterText();
	Y += ((testing->numTextLines + 1) * lineSpacing) + offset;

	guide = new MultilineTextBox(AssetsStringTable::getString("strategy_guide"), X, Y, zOrder, color, font, lineSpacing);
	guide->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	guide->LineTexts[0]->LoadFont(headingFont);
	guide->CenterText();
	Y += ((guide->numTextLines + 1) * lineSpacing) + offset;

	_kanjiEng = new MultilineTextBox(AssetsStringTable::getString("kanjiEng"), X, Y, zOrder, color, font, lineSpacing);
	_kanjiEng->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	_kanjiEng->LineTexts[0]->LoadFont(headingFont);
	_kanjiEng->CenterText();
	Y += ((_kanjiEng->numTextLines + 1) * lineSpacing) + offset;

	addprogram = new MultilineTextBox(AssetsStringTable::getString("addprogram"), X, Y, zOrder, color, font, lineSpacing);
	addprogram->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	addprogram->LineTexts[0]->LoadFont(headingFont);
	addprogram->CenterText();
	Y += ((addprogram->numTextLines + 1) * lineSpacing) + offset;
	
	addscript = new MultilineTextBox(AssetsStringTable::getString("addscript"), X, Y, zOrder, color, font, lineSpacing);
	addscript->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	addscript->LineTexts[0]->LoadFont(headingFont);
	addscript->CenterText();
	Y += ((addscript->numTextLines + 1) * lineSpacing) + offset;

	addconsultant = new MultilineTextBox(AssetsStringTable::getString("addconsultant"), X, Y, zOrder, color, font, lineSpacing);
	addconsultant->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	addconsultant->LineTexts[0]->LoadFont(headingFont);
	addconsultant->CenterText();
	Y += ((addconsultant->numTextLines + 1) * lineSpacing) + offset;

	addart = new MultilineTextBox(AssetsStringTable::getString("addart"), X, Y, zOrder, color, font, lineSpacing);
	addart->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	addart->LineTexts[0]->LoadFont(headingFont);
	addart->CenterText();
	Y += ((addart->numTextLines + 1) * lineSpacing) + offset;

	addvo = new MultilineTextBox(AssetsStringTable::getString("addvo"), X, Y, zOrder, color, font, lineSpacing);
	addvo->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	addvo->LineTexts[0]->LoadFont(headingFont);
	addvo->CenterText();
	Y += ((addvo->numTextLines + 1) * lineSpacing) + offset;

	addmusic = new MultilineTextBox(AssetsStringTable::getString("addmusic"), X, Y, zOrder, color, font, lineSpacing);
	addmusic->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	addmusic->LineTexts[0]->LoadFont(headingFont);
	addmusic->CenterText();
	Y += ((addmusic->numTextLines + 1) * lineSpacing) + offset;

	mobileport = new MultilineTextBox(AssetsStringTable::getString("mobileport"), X, Y, zOrder, color, font, lineSpacing);
	mobileport->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	mobileport->LineTexts[0]->LoadFont(headingFont);
	mobileport->CenterText();
	Y += ((mobileport->numTextLines + 1) * lineSpacing) + offset;

	linux_macport = new MultilineTextBox(AssetsStringTable::getString("linux_macport"), X, Y, zOrder, color, font, lineSpacing);
	linux_macport->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	linux_macport->LineTexts[0]->LoadFont(headingFont);
	linux_macport->CenterText();
	Y += ((linux_macport->numTextLines + 1) * lineSpacing) + offset;

	localization_programming = new MultilineTextBox(AssetsStringTable::getString("localization_programming"), X, Y, zOrder, color, font, lineSpacing);
	localization_programming->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	localization_programming->LineTexts[0]->LoadFont(headingFont);
	localization_programming->CenterText();
	Y += ((localization_programming->numTextLines + 1) * lineSpacing) + offset;

	language = new MultilineTextBox(AssetsStringTable::getString("language"), X, Y, zOrder, color, font, lineSpacing);
	language->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	language->LineTexts[0]->LoadFont(headingFont);
	language->CenterText();
	Y += ((language->numTextLines + 1) * lineSpacing) + offset;

	digitalMarketing = new MultilineTextBox(AssetsStringTable::getString("digitalMarketing"), X, Y, zOrder, color, font, lineSpacing);
	digitalMarketing->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	digitalMarketing->LineTexts[0]->LoadFont(headingFont);
	digitalMarketing->CenterText();
	Y += ((digitalMarketing->numTextLines + 1) * lineSpacing) + offset;

	growthHacking = new MultilineTextBox(AssetsStringTable::getString("growthHacking"), X, Y, zOrder, color, font, lineSpacing);
	growthHacking->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	growthHacking->LineTexts[0]->LoadFont(headingFont);
	growthHacking->CenterText();
	Y += ((growthHacking->numTextLines + 1) * lineSpacing) + offset;

	ksidc = new MultilineTextBox(AssetsStringTable::getString("ksidc"), X, Y, zOrder, color, font, lineSpacing);
	ksidc->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	ksidc->LineTexts[0]->LoadFont(headingFont);
	ksidc->CenterText();
	Y += ((ksidc->numTextLines + 1) * lineSpacing) + offset;

	ksum = new MultilineTextBox(AssetsStringTable::getString("ksum"), X, Y, zOrder, color, font, lineSpacing);
	ksum->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	ksum->LineTexts[0]->LoadFont(headingFont);
	ksum->CenterText();
	Y += ((ksum->numTextLines + 1) * lineSpacing) + offset;

	infopark = new MultilineTextBox(AssetsStringTable::getString("infopark"), X, Y, zOrder, color, font, lineSpacing);
	infopark->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	infopark->LineTexts[0]->LoadFont(headingFont);
	infopark->CenterText();
	Y += ((infopark->numTextLines + 1) * lineSpacing) + offset;

	gratitude = new MultilineTextBox(AssetsStringTable::getString("gratitude"), X, Y, zOrder, color, font, lineSpacing);
	gratitude->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	gratitude->LineTexts[0]->LoadFont(headingFont);
	gratitude->CenterText();
	Y += ((gratitude->numTextLines + 1) * lineSpacing) + offset;

	KFC = new MultilineTextBox(AssetsStringTable::getString("KFC"), X, Y, zOrder, color, font, lineSpacing);
	KFC->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	KFC->LineTexts[0]->LoadFont(headingFont);
	KFC->CenterText();
	Y += ((KFC->numTextLines + 1) * lineSpacing) + offset;

	special = new MultilineTextBox(AssetsStringTable::getString("special"), X, Y, zOrder, color, font, lineSpacing);
	special->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	special->LineTexts[0]->LoadFont(headingFont);
	special->CenterText();
	Y += ((special->numTextLines + 1) * lineSpacing) + offset + 125;

	logo->SetPos(PointSystem::CreateCPoint(X, Y, zOrder));
	Y += offset + 125;


	switch (pub)
	{
	case 0:
		pubString = "";
		logo->PlayAnimation(0);
		break;
	case 1:
		pubString = AssetsStringTable::getString("bigFish");
		logo->PlayAnimation(1);
		break;
	case 2:
		pubString = AssetsStringTable::getString("gameHouse");
		logo->PlayAnimation(2);
		break;
	case 3:
		pubString = AssetsStringTable::getString("steam");
		logo->PlayAnimation(3);
		break;
	case 4:
		pubString = AssetsStringTable::getString("humbleBundle");
		logo->PlayAnimation(4);
		break;
	default:
		pubString = "";
		logo->PlayAnimation(0);
		break;
	}


	PUB = new MultilineTextBox(pubString, X, Y, zOrder, color, font, lineSpacing);
	PUB->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	PUB->LineTexts[0]->LoadFont(headingFont);
	PUB->CenterText();
	Y += ((PUB->numTextLines + 1) * lineSpacing) + offset;

	thankyou = new MultilineTextBox(AssetsStringTable::getString("thankyou"), X, Y, zOrder, color, font, lineSpacing);
	thankyou->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	thankyou->LineTexts[0]->LoadFont(headingFont);
	thankyou->CenterText();
	Y += ((thankyou->numTextLines + 1) * lineSpacing) + offset;

	copyright = new MultilineTextBox(AssetsStringTable::getString("copyright"), X, Y, zOrder, color, font, lineSpacing);
	copyright->LineTexts[0]->SetColor(headingColor.r, headingColor.g, headingColor.b);
	copyright->LineTexts[0]->LoadFont(headingFont);
	copyright->CenterText();
	Y += ((copyright->numTextLines + 1) * lineSpacing) + offset;	
}

void Credits::TranslateY()
{

	HOScene->GetObjectByID("BG")->Pos.y -= (0.07 * -scrollSpeed);
	HOScene->GetObjectByID("Photo")->Pos.y += scrollSpeed;
	HOScene->GetObjectByID("Title")->Pos.y += scrollSpeed;
	TF_logo->Pos.y += scrollSpeed;

	gameProducers->TranslateY(scrollSpeed);
	creativeDirector->TranslateY(scrollSpeed);
	leadDesigner->TranslateY(scrollSpeed);
	artDirector->TranslateY(scrollSpeed);
	animationDirector->TranslateY(scrollSpeed);
	musicDirector->TranslateY(scrollSpeed);
	technicalLead->TranslateY(scrollSpeed);
	leadProgrammer->TranslateY(scrollSpeed);
	projectManagers->TranslateY(scrollSpeed);
	story->TranslateY(scrollSpeed);
	emogamedes->TranslateY(scrollSpeed);
	script->TranslateY(scrollSpeed);
	srGamePro->TranslateY(scrollSpeed);
	srArtist->TranslateY(scrollSpeed);
	srAnim->TranslateY(scrollSpeed);
	leadVFX->TranslateY(scrollSpeed);
	trailer->TranslateY(scrollSpeed);
	sounddesign->TranslateY(scrollSpeed);
	music->TranslateY(scrollSpeed);
	voiceover->TranslateY(scrollSpeed);
	audioMix->TranslateY(scrollSpeed);
	QA->TranslateY(scrollSpeed);
	testLead->TranslateY(scrollSpeed);
	testing->TranslateY(scrollSpeed); 
	guide->TranslateY(scrollSpeed);
	_kanjiEng->TranslateY(scrollSpeed);
	addscript->TranslateY(scrollSpeed);  
	addconsultant->TranslateY(scrollSpeed);  
	addart->TranslateY(scrollSpeed);  
	addvo->TranslateY(scrollSpeed); 
	addmusic->TranslateY(scrollSpeed);
	addprogram->TranslateY(scrollSpeed); 
	mobileport->TranslateY(scrollSpeed); 
	linux_macport->TranslateY(scrollSpeed);
	localization_programming->TranslateY(scrollSpeed); 
	language->TranslateY(scrollSpeed); 
	digitalMarketing->TranslateY(scrollSpeed);
	growthHacking->TranslateY(scrollSpeed);
	ksidc->TranslateY(scrollSpeed);
	ksum->TranslateY(scrollSpeed); 
	infopark->TranslateY(scrollSpeed);  
	gratitude->TranslateY(scrollSpeed); 
	KFC->TranslateY(scrollSpeed); 
	special->TranslateY(scrollSpeed);
	logo->Pos.y += scrollSpeed;
	PUB->TranslateY(scrollSpeed); 
	thankyou->TranslateY(scrollSpeed); 
	copyright->TranslateY(scrollSpeed);
}