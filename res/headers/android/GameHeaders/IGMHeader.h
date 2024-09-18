//====================================
// Framework
//====================================

#ifndef DQFULP_INGAMEMENU_HEADER_DEF
#define DQFULP_INGAMEMENU_HEADER_DEF

enum eIGMButtons
{
	IGMB_Resume,
	IGMB_Options,
	IGMB_Memories,
	IGMB_MainMenu,	
	IGMB_Restart,
	IGMB_Max
};

enum eHudButtonAnims
{
	HBA_Idle = 0,
	HBA_Full = 0,
	HBA_Closed = 0,
	HBA_Hover = 1,
	HBA_Opened = 1,
	HBA_Empty = 1,
	HBA_Click = 2,
	HBA_Hidden = 3
};


enum IGMPopup_State
{
	Popup_Inactive = -1,
	Popup_InGameMenu,
	Popup_Options,
	Popup_Restart,
	Popup_ModeSelect
};


enum IGMPopup_UI_Result
{
	Popup_NoInteraction,
	Popup_MouseOver,
	Popup_Click
};

#endif