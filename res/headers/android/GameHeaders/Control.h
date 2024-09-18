//====================================
// Framework
// Control class [abstract]
//====================================

#ifndef FRMWRK_CONTROL_DEF
#define FRMWRK_CONTROL_DEF

#include "Message.h"
#include "Input.h"
#include "Timer.h"
#include "AudioSlave.h"
#include "Cursor.h"

enum _InventorySize{ INVENTORY_V1, INVENTORY_V2};

class Control
{
	public:
		int		ID;
		Control();
		virtual ~Control();

		//Instance method; not recommended to override, but can
		//be useful in exceptional cases. See class Application.
		virtual void Process();

		//Class wide methods
		static void		InitializeSystem();
		static void		DestroySystem();
		static int		RetrieveID();
		static void		SetLogicRate(int dms);
        static bool     IsSysVideoToStop;
		static bool		Is_iPadCursorOverVideo;

		

		//Inline instance methods
		inline void		SetModal(bool f=true)	{ Modal = f; }
		inline sMessage GetMessage()			{ MsgAvailable = false; return Message; }		
		

		virtual void DirectCallback(int value) { ; }
		static float    LogicRate;
		bool			MsgAvailable;
		bool			Modal;

		//Class wide members
		static CInput*		Input;
		static Timer*		MasterTmr;
		static AudioSlave*	Audio;

	protected:
		//Instance members		
		
		
		sMessage				Message;
		std::vector<Control *>	Controls;

		
		static bool	   SystemInitialized;
		static int	   CumulativeID;
		

		//Methods
		virtual void ProcessMessage(sMessage message);
		virtual void SendMessage(eMessageProtocol protocol, int cID, int value);		
		virtual bool RemoveControl(int id, int value);
		virtual int  AddControl(Control* newControl);
		virtual void Exit();
		
		

		//Pure virtual methods
		virtual void Update() = 0;

		//Super risky auto update -- use with caution
		//disobeys modalness
		virtual void Required(){};
};

#endif