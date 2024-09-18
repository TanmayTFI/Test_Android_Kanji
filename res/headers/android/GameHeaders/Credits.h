

#ifndef DQFYLH_Credits_DEF
#define DQFYLH_Credits_DEF

//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "UpdateListEntity.h"
#include <vector>
#include <list>
#include "UIButton.h"
#include "MultilineTextBox.h"
#include "AssetsStringTable.h"

class Credits : public HiddenObjectSystem
{
	float _credictsStartDelay;

	UIButton* _skipBtn;
	
	SuperFX *CreditsFliesFX;
	SuperFX *CreditsSmokeFX;
	float _creditsPlayTime, _CreditsMaxPlayTime;

public:

	Credits();
	~Credits();

	void Update();
	void Required();
	void LoadTxt();
	void TranslateY();
	float scrollSpeed;
	std::string pubString;
	int pub;

	CObject* logo, *TF_logo;

	MultilineTextBox* gameProducers, * creativeDirector, * leadDesigner, * artDirector, * animationDirector,* musicDirector,* technicalLead,* leadProgrammer,* projectManagers;
	MultilineTextBox* story, * emogamedes, * script, * srGamePro, * srArtist, * srAnim, * leadVFX, * trailer, * sounddesign, * music, * voiceover, * audioMix, * QA, * testLead;
	MultilineTextBox* testing, * guide, * _kanjiEng, * addscript, * addconsultant, * addart, * addvo, * addmusic, * addprogram, * mobileport, * linux_macport, * localization_programming, * language, * digitalMarketing, *growthHacking;
	MultilineTextBox* ksidc, * ksum, * infopark, * gratitude, * KFC, * special, *PUB, *thankyou, * copyright;
	float X, Y;

private:

	void OnSkip(CObject* obj);

protected:

	void ReceiveMessage(int val);
};

#endif
