//====================================
//DQFULP
// Hidden Object System
//====================================

#ifndef DQFULP_JOURNAL_DEF
#define DQFULP_JOURNAL_DEF

#include "Control.h"
#include "HOObject.h"
#include "BitmapText.h"
#include <string>
#include "MultilineTextBox.h"

class CObject;
class CHOScene;
class HudControl;
class Tutorial;
class CBitmapText;

#define MAX_CURRENT_OBJECTIVES 6

enum eCharacters{
	//creeks
	eChar0_None,
	eChar1_Sophia,
	eChar2_James,
	eChar3_Mary,
	eChar4_Creek,
	eChar5_Grandma,

	//Snake Clan
	eChar6_Darkarta,
	eChar7_NaginMaryPast,
	eChar8_AnanthaYoung,
	eChar9_Varuni,

	//Eagle Clan
	eChar10_Lasya,
	eChar11_Ameeha,
	eChar12_Devadeva,

	eChar13_Rajasthani,
	eChar14_Sukra,
	eChar15_Mooppan,
	eChar16_Anasooya
};

class Journal : public Control, CRenderListEntity
{
	public:

		//Character
		bool isCharacterPageActive;
		int activeCharacterID;
		CObject* charPhoto;
		MultilineTextBox *charText;

		void loadCharacter(int charID);

		int  ActiveJrnPage;
		int	BgZOrder;

		int journalState;

		CBitmapText *GameTimeText;
		
		//Scn Elements
		CObject *CurJrnPage, *PrevJrnPage;

		//Localized Journal Text
		MultilineTextBox *jrnTxtL, *jrnTxtR,* jrnTxtHeadingL, * jrnTxtHeadingR;
		float textShowTimer; bool showDiaryText;
		//CBitmapText* jrnTxtHeadingL, * jrnTxtHeadingR;
		CObject* jrnImg, *jrnImg2;

		CObject *BtnClose, *BtnPrevPage, *BtnNextPage; 
		CObject *BtnTabObjective, *BtnTabJournal, *BtnTabMap, *BtnTabCharacter;

		CObject *BtnClanCreeks, *BtnClanSnake, *BtnClanEagle, *BtnClanOthers;

		CObject *ObjectiveTitle;
		CBitmapText *ObjBmpTxt[MAX_CURRENT_OBJECTIVES];
		//MultilineTextBox* ObjBmpTxt[MAX_CURRENT_OBJECTIVES];
		CObject		*ObjSymbols[MAX_CURRENT_OBJECTIVES];
		int maxObjectives, numCurrentObjectives;

		bool isObjTabTutorActive, isTutorialActive;


		CHOScene *JrnScene;
		
		Journal();
		~Journal();

		void  LoadJrnPage(bool IsInitialLoading=false);		

		void  ShowJournal(bool noTransition=false, bool showCharacterPage=false);
		void  HideJournal(bool noTransition=false);

		void  HideCharacterPage();
		void  HideObjectivesPage();
		void  HideJournalPage();

		void  UpdateJournalPage(CPoint pos);

		void GetCharacterIDFromProfile();
		//Set Next or Previous Buttons
		void SetupCharacterPage();
		void UpdateCharacterPage(CPoint pos);
		void GetNextUnlockedCharacterID();
		void GetPreviousUnlockedCharacterID();
		void GetUnlockedCharacterInClan(int clan);

		void  ShowCharacterPage();
		void  ShowObjectivesPage();
		void  ShowJournalPage();

		float easingQuadOut(float t,float b,float c,float d);

		void  UpdateJournalAndObjectives();
		void  InitRenderTarget(CHOScene *Popup);
		float PopupAlpha, PopVal;

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);

		//Inherited method
		void Update();
		void Required();

	protected:
};

#endif