//====================================
// DQFULP
// Dialog Inventory box
//====================================

#ifndef DQFULP_DIALOGSYSTEM_DEF
#define DQFULP_DIALOGSYSTEM_DEF

#include "Control.h"
#include "HOObject.h"
#include "HOScene.h"
#include "CVideo.h"
#include "MultilineText.h"
#include "MeshObject.h"
#include <string>

class CObject;
class CHOScene;
class CBitmapText;

enum DialogID{
	eDialog_S1RajaBeach,
	eDialog_S3RajaFoyer,
	eDialog_S3RajaFoyerAskForLetter,
	eDialog_S3RajaFoyerDark,
	eDialog_S4Creek,
	eDialog_S3RajaFoyerShowLetter,
	eDialog_L2S5SMoopanTired,
	eDialog_L2S5SMoopanDrinking,
	eDialog_L3S5_AnnMary,
	eDialog_L5S4Anasooya,
	eDialog_L5S4AnasooyaFree,
	eDialog_L7S4Ameha,
	eDialog_L5S6TreeGuardian,
	eDialog_L5S6TreeGuardianAfterBadgePlaced,
	eDialog_L6S3FakeJames,
	eDialog_L6S4RockMan,
	eDialog_L6S5Sophia,
	eDialog_L6S5SophiaPlayingWithTablet,
	eDialog_L7S3Anantha
};

class DialogSystem : public Control
{
	public:

		//Scn Elements
		CObject *BtnSkip, *btnSkipText;
		CObject *TxtContinue, *btnContText;

		CHOScene *DialogScene;
		CHOScene *CharScene;

		//eDialog_S1RajaBeach
		CObject *S1RajaHead;

		//eDialog_S4Creek
		CObject *S4CreekHead;

		//eDialog_L2S5SMoopanTired
		CObject *S5MoopanHeadTired;
		CObject *MoopanHead;

		bool IsMoopanTalking;

		//eDialog_L5S6TreeGuardian
		CObject *S6TreeMouth;

		//eDialog_L6S3FakeJames
		CObject *L6S3JamesFace;

		//eDialog_L6S4RockMan
		CObject *L6S4Mouth;
		float rockTimer;
		bool startRockTimer;

		//eDialog_L6S5Sophia
		CObject* L6S5Mouth;
		int		SophiaPosition;
		bool	IsSophiaTalking;

		//eDialog_L7S3Anantha
		CObject *L7AnanthaHead, *L7AnanthaEyeBlink;
		float L7BlinkTimer;

		MeshObject* hairObj;

		CVideo *cv_CreekPortalShowFX, *cv_CreekPortalLoopFX;
		
		bool isCreekPortalShowFXPlaying;

		bool isMaskEnabled;
		
		DialogSystem();
		~DialogSystem();

		void  ShowDialog(int dialogId, bool quickShow=false);
		void  HideDialog();
		void  CloseDialogSystem();
		int	  GetDialogId(){return _dialogId;};

		MultilineText *DialogText;

		float clickWaitTimer;

		void Update();		
		void Required();

	protected:
		bool _isActive;

		bool _isFadingIn, _isFadingOut;
		float _fadeAlpha;

		bool _isTextFadingOut, _isTextFadingIn;
		int _nextDialogId;

		int _dialogId;

		int _activeDialogIndex;
		int _numDialogLines;
		bool	_maryTalksArray[10];
		std::vector<std::string>	_dialogArray;
		std::vector<int>	_dialogVOArray;
};

#endif