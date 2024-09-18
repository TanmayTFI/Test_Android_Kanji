//====================================
// Framework
// Control class [abstract]
//====================================

#include "Control.h"
#include "ProfileSystem.h"
#include "iPadDefs.h"
#include "Application.h"

//Instantiation of class wide members
CInput*		Control::Input			= NULL;
Timer*		Control::MasterTmr	= NULL;
AudioSlave* Control::Audio			= NULL;
bool	Control::SystemInitialized	= false;
int		Control::CumulativeID	= 0;
float	Control::LogicRate = 0.0f;
bool    Control::IsSysVideoToStop = false;
bool	Control::Is_iPadCursorOverVideo = false;


//Class wide methods
void Control::InitializeSystem()
{
	if( Control::SystemInitialized )
	{
		//Logger::Log("Attempted duplicate initialization in Control.");
		return;
	}

    Control::IsSysVideoToStop = false;
	Control::Is_iPadCursorOverVideo = false;
    Control::Input = new CInput();
	Control::MasterTmr = new Timer();
	Control::Audio = new AudioSlave();
	Control::SystemInitialized = true;
}

void Control::DestroySystem()
{
	if( !Control::SystemInitialized )
	{
		//Logger::Log("Attempted duplicate destruction in Control.");
		return;
	}

	
	delete Control::Audio;
	Control::Audio = NULL;

	delete Control::MasterTmr;
	Control::MasterTmr = NULL;

	delete Control::Input;
	Control::Input = NULL;	

    Control::IsSysVideoToStop = false;
	Control::Is_iPadCursorOverVideo = false;

	Control::SystemInitialized = false;
}

int Control::RetrieveID()
{
	return Control::CumulativeID++;
}

void Control::SetLogicRate(int dms)
{
	Control::LogicRate = (float)dms / 1000.0f;
}

//Instance methods
Control::Control()
{
	ID = Control::RetrieveID();
	Modal = false;
	MsgAvailable = false;
}

Control::~Control()
{
	int size = (int)Controls.size();
	for(int i=0; i < size; ++i)
		delete Controls[i];
}

void Control::Process()
{
	int  control	= (int)Controls.size() - 1;
	bool execute	= true;

	//Process dependent controls from newest to oldest
	while( control >= 0 )
	{
		Controls[control]->Process();
		
		if( Controls[control]->MsgAvailable )
		{
			ProcessMessage( Controls[control]->GetMessage() );			
			execute = false;
		}
		else if( Controls[control]->Modal )
		{
			SendMessage(ModalLock, Controls[control]->ID, 0);
			execute = false;
		}
		
		--control;

		//Psuedo hack, to ensure flat level children are always allowed to run required.
		if( !execute )
		{
			while( control >= 0 )
			{
				Controls[control]->Required();
				--control;
			}
			break;
		}
	}

	//If all dependent controls have executed, we can update ourself
	//This is critical to maintain modal respect
	if( execute )
		Update();

	Required();//call main object required 
}

//Messages propogate through the entire control tree, although some may
//require action and send off the respective message.
void Control::ProcessMessage(sMessage message)
{
	switch( message.Protocol )
	{
		case ControlExit:
			{
                if( RemoveControl(message.CallerID, message.Value) )
					SendMessage(ActionProcessed, ID, 0);				
				else				
					SendMessage(RemovalError, ID, message.CallerID);	
			}
			break;
		default:
			SendMessage(message.Protocol, message.CallerID, message.Value);
	}
}

void Control::SendMessage(eMessageProtocol protocol, int cID, int value)
{
	if( protocol == ControlExit )
    {
        Cursor::Show(false);//avoid user clicking here and there afterwards!!
        //Reset zoom
#ifdef TOUCH_UI
        GFApp->ProjZoom = 1;
        GFApp->ProjX = 0;
        GFApp->ProjY = 0;
#endif
    }
	MsgAvailable = true;
	Message.Protocol = protocol;
	Message.CallerID = cID;
	Message.Value = value;
}



bool Control::RemoveControl(int id, int value)
{
	for(std::vector<Control*>::iterator it = Controls.begin(); 
		it != Controls.end();
		++it)
	{
		if( (*it)->ID == id )
		{
			delete (*it);
			(*it) = NULL;
			Controls.erase(it);
			return true;
		}
	}

	return false;
}

int Control::AddControl(Control* newControl)
{
	Controls.push_back(newControl);

	SendMessage(ControlAdded, ID, newControl->ID);
	
	
	return newControl->ID;
}

void Control::Exit()
{
	SendMessage(ControlExit, ID, 0); 
}

