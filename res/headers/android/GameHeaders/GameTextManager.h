//====================================
// Framework
// Game Text Manager
//====================================

#ifndef FRMWRK_GAMETEXT_DEF
#define FRMWRK_GAMETEXT_DEF

#include "LangDefs.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API

using namespace rapidjson;

class GameTextManager
{
	public:
		static void DeleteInstance();
		static GameTextManager* GetInstance();

		void SetMyLangauge(int lang);
		void LoadAllGameTexts();
		void LoadBannerForLevel();
		void ClearLevelBanners();
		std::string GetMyLangauge();
		int GetMyLangaugeID();
		std::string GetBannerText(const std::string keyString);

	private:
		Document _bannerJSONDoc;
		Document _commonJSONDoc;
		int myLang;
		
		GameTextManager();
		~GameTextManager();
};


#endif
