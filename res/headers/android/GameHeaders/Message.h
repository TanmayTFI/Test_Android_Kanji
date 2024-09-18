//====================================
// Framework
// Message structure/protocol
//====================================

#ifndef FRMWRK_MESSAGE_DEF
#define FRMWRK_MESSAGE_DEF

enum eMessageProtocol
{
	UnknownProtocol = -1,
	ApplicationExit,
	ControlExit,
	ControlAdded,
	ActionProcessed,
	RemovalError,
	ModalLock,
};

struct sMessage
{
	eMessageProtocol	Protocol;
	int					CallerID;
	int					Value;

	sMessage()
	{
		Protocol = UnknownProtocol;
		CallerID = -1;
		Value	 = -1;
	}
};

#endif