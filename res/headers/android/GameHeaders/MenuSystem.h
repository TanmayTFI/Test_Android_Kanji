//====================================
//DQFULP
// Menu System Control
//====================================

#ifndef DQFULP_MENUSYSTEM_DEF
#define DQFULP_MENUSYSTEM_DEF

#include "Control.h"
#include "HOScene.h"


enum eMenuFlow
{
	Title,
	Main,
	Options
};

enum eExitCodes
{
	xMainMenu,
	xQuestMode,
	xExitGame,
};

enum eButtonAnims
{
	BA_Idle,
	BA_Hover,
	BA_Click,
	BA_Locked
};
class MenuSystem : public Control
{
	public:
		MenuSystem();
		~MenuSystem();

		//Inherited method
		void Update();

	private:
		eMenuFlow Flow;
		int		  ExitValue;

		Control	*ActiveControl;


		//Inherited method
		void ProcessMessage(sMessage message);

		//Menu System methods
		void ProcessControlChange();
};

#endif