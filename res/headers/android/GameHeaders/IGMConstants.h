//====================================
// Framework
//====================================

#ifndef DQFULP_INGAMEMENU_CONSTANTS_DEF
#define DQFULP_INGAMEMENU_CONSTANTS_DEF

#define KNOB_STARTX 548.0f
#define KNOB_ENDX 	830.0f

#define MUSICKNOB_POSY	204
#define SOUNDKNOB_POSY	264
#define VOICEKNOB_POSY	326

enum CheckBoxState{CHECKBOX_ON, CHECKBOX_OFF};

const CRectangle  ResumeRect(584.0f,173.0f,218.0f,56.0f);
const CRectangle  OptionsRect(596.0f,258.0f,189.0f,50.0f);
const CRectangle  MainMenuRect(575.0f,491.0f,221.0f,58.0f);
const CRectangle  MemoriesRect(582.0f,333.0f,213.0f,61.0f);
const CRectangle  RestartRect(575.0f,412.0f,221.0f,58.0f);


const std::string IGMButtonID[IGMB_Max] =
{												
	"resume", "options", "memories", "mainmenu", "restart" 
};
const CPoint MaskSprPos(0,0,0);

#endif