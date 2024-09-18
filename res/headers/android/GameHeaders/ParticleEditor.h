	//====================================
// Framework
// Main Menu Control :)
//====================================

#ifndef DQFULH_PARTICLEDESIGNER_DEF
#define DQFULH_PARTICLEDESIGNER_DEF

#include "MenuSystem.h"
#include "Control.h"
#include "BitmapText.h"
#include "HOScene.h"

enum PEPopup_State
{
	PEPopup_Inactive = -1,

	PEPopup_EnterYourName,
	PEPopup_Options
};

class CBitmapText;
class SuperFX;
class TextBox;
class CObject;

#define NUMLEFTCOLUMNELEMENTS 22
#define NUMCENTERCOLUMNELEMENTS 17
#define NUMRIGHTCOLUMNELEMENTS 17

class ParticleEditorControl : public Control, CRenderListEntity
{
	public:

		CObject *CustomAnimObj;

		CObject *UI_CenterDot, *UI_EmitDir, *UI_EmitVarMin, *UI_EmitVarMax;
		bool IsCenterDragging;

		bool IsUIHiding;

		KGraphic *DebugGraphic;

		float srcWidth, srcHeight;
		int srcType;

		int activeId, activeColumn;
		
		//--------------------Particle Editor Stuffs------------------------//
		CRectangle	*LeftColumnRects[NUMLEFTCOLUMNELEMENTS];
		CObject		*LeftColumnKnobs[NUMLEFTCOLUMNELEMENTS];
		float		LeftColumnKnobYPos[NUMLEFTCOLUMNELEMENTS];
		CBitmapText	*LeftColumnValTexts[NUMLEFTCOLUMNELEMENTS];
		bool		IsLeftColKnobDragStarted[NUMLEFTCOLUMNELEMENTS];

		CRectangle	*CenterColumnRects[NUMCENTERCOLUMNELEMENTS];
		CObject		*CenterColumnKnobs[NUMCENTERCOLUMNELEMENTS];
		float		CenterColumnKnobYPos[NUMCENTERCOLUMNELEMENTS];
		CBitmapText	*CenterColumnValTexts[NUMCENTERCOLUMNELEMENTS];
		bool		IsCenterColKnobDragStarted[NUMCENTERCOLUMNELEMENTS];

		CRectangle	*RightColumnRects[NUMRIGHTCOLUMNELEMENTS];
		CObject		*RightColumnKnobs[NUMRIGHTCOLUMNELEMENTS];
		float		RightColumnKnobYPos[NUMRIGHTCOLUMNELEMENTS];
		CBitmapText	*RightColumnValTexts[NUMRIGHTCOLUMNELEMENTS];
		bool		IsRightColKnobDragStarted[NUMRIGHTCOLUMNELEMENTS];

		int			currentLeftKnobId;
		int			currentCenterKnobId;
		int			currentRightKnobId;

		int	resetDelayTimer;
		
		void UpdateParticleWithValue(int currentKnobId, float val, int column);
		void UpdateParticleWithDefault(int currentKnobId, int column);
		void UpdateEditorValues(int i, int column);
		//------------------------------------------------------------------//

		// Easing //
        
	    //Message Banner
		void UpdateBanner();
		void InitMessageBanner();		
		void TransOutBanner();
		void ShowMessageBanner(const std::string& message, int SFXId = SFX_SOFT_NOTIFY);
		bool HideMessageBanner(void);

		virtual void BannerCloseNotify(){};

		CBitmapText *MessageText;
		float BannerTimer;
		float BannerAlpha;
		float TransOutFactor;
		bool  IsBannerStandStill;
		eMessageBannerState BannerState;
		CObject	  *MessageElement;

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void   Render(const CPoint* pCam);

		KGraphic *StartColorImg, *EndColorImg;

		//Use file type 0 - VFX file, file type 1 - ImageFile
		void OpenFileAndAddToScene(int fileType);

		//TextBox  *TextEntry;
		std::string NewProfileName;

		ParticleEditorControl();
		~ParticleEditorControl();

		bool IsDisabled;

		int PrevCursorMode;

		void WriteParticleFileData();
		void HandleValueSliderUpdate(CPoint mosPos);
		void ShowUI(bool hide);
		void SetUIValuesWithActiveVFX();

		int skipEmitFrames;

		void LoadPreset(int id);
		void duplicateCurrentParticle();

		TextBox  *ParticleNameTextEntry;
		bool isParticleNameEntryActive;

		std::vector<CBitmapText*> _particleNameTextArray;
		std::vector<SuperFX*> _particlesArray;
		std::vector<CRectangle*> _outlineRects;
		int currentParticleId;

		void deleteCurrentParticle();

		int baseZOrder, defaultParticleZOrder;

		//Inherited methods
		void Update();
		void Show(bool f);

	protected:

		void ProcessMessage(sMessage message);

		SuperFX *ActiveVFX;
		CHOScene	*MainScene;
		CObject *EditorBg;

		CBitmapText	*ConsoleText;
		CBitmapText	*PositionText;
};

#endif