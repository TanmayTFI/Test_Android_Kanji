//====================================
//DQFULP
// Menu System Control
//====================================

#include "MenuSystem.h"
#include "Control.h"
#include "ProfileSystem.h"
#include "MainMenuControl.h"
#include "Application.h"


MenuSystem::MenuSystem()
{
	Flow = Main;
	ExitValue = 0;
	ActiveControl = new MainMenuControl();
	AddControl(ActiveControl);
}

MenuSystem::~MenuSystem()
{
	

}

void MenuSystem::Update()
{
}

void MenuSystem::ProcessMessage(sMessage message)
{
	if( message.Protocol == ActionProcessed )
	{
		ExitValue = message.Value;
		ProcessControlChange();
	}
	else
	{
		Control::ProcessMessage(message);
	}
}

void MenuSystem::ProcessControlChange()
{
	Cursor::Show(false);	

	if( RemoveControl(ActiveControl->ID, 0) == false)
	{
		//bad deal
	}

	switch(ExitValue)
	{
		case xMainMenu:
			Flow = Main;
			ActiveControl = new MainMenuControl();
			AddControl(ActiveControl);			
			Cursor::SetMode(CA_Normal);
			break;
		
		//EXTERNAL AFFAIRS
		case xQuestMode:	
			Cursor::Show(true);	
			SendMessage(ControlExit, ID, 0);
			break;
		case xExitGame:
			SendMessage(ApplicationExit, ID, 0);			
			break;
	}	
}