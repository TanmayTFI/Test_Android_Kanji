//====================================
// Framework
//====================================

#include "ParticleEditor.h"
#include "SuperFX.h"
#include "Object.h"
#include "Application.h"
#include "TextBox.h"
#include "AnimationStream.h"
#include "TextureFactory.h"
#include "GameFlow.h"
#include "Resources.h"
#include "InfoPanel.h"
#include "Util.h"
#include "KList.h"
#include "Fonts.h"
#include "LangDefs.h"
#include "IOStream.h"

enum CheckBoxState{CHECKBOX_ON, CHECKBOX_OFF};

#define LEFTSLIDER_STARTX	764.0f
#define LEFTSLIDER_ENDX		900.0f

#define CENTERSLIDER_STARTX	974.0f
#define CENTERSLIDER_ENDX	1114.0f

#define RIGHTSLIDER_STARTX	1164.0f
#define RIGHTSLIDER_ENDX	1304.0f

//Colums
#define LEFTCOL		0
#define CENTERCOL	1
#define RIGHTCOL	2

const std::string PresetPaths[10] =
{
	"PRT\\PRESETS\\1BlackSmoke.INI",
	"PRT\\PRESETS\\2GlowingFire.INI",
	"PRT\\PRESETS\\3GlowingRing1.INI",
	"",
	"",
	"",
	""
};

const float LeftSliderMaxValues[NUMLEFTCOLUMNELEMENTS] ={
	5,	//Blend Mode [0-Additive : 1-Alpha]
	6,	//Source Type [0-Point : 1-Circle : 2-Horz : 3-Vert]
	1024,//Source Width (circle radius)
	768,//Source Height
	512,//Size of Particle Pool
	2,	//Emit Timer (time interval for emissions)
	100,//Emit Particles
	1,		//Uniform emission [0-no : 1-yes]
	360,	//Angle
	360,	//Angle Var
	
	0,	//Empty
	
	20,//Life
	20,//LifeVar

	2000,//Speed
	1000,//SpeedVar

	1600,	//X acceleration
	1600,	//Y acceleration

	1.0f,	//Start Alpha
	1.0f,	//Start Alpha Var

	2.0f,	//End Alpha
	1.0f,	//End Alpha Var
	
	3		//Alpha Mode
};

const float CenterSliderMaxValues[NUMCENTERCOLUMNELEMENTS] ={
	360,	//Init Rotation
	360,	//Init Rotation variance
	360,	//End Rotation
	360,	//End Rotation variance
	3,		//Rotation Mode [ 0-Linear : 2-Wave]

	4.0f,	//Init Scale
	4.0f,	//Init Scale variance
	4.0f,	//End Scale
	4.0f,	//End Scale variance
	3,		//Scale Mode [ 0-Linear : 2-Wave]

	0,	//Empty

	1.0f,	//Init Red
	1.0f,	//Init Green
	1.0f,	//Init Blue
	1.0f,	//Init Red variance
	1.0f,	//Init Green variance
	1.0f	//Init Blue variance
};

enum EnumParticleCenterConsts
{
	ePfxInitRot,
	ePfxInitRotVar,
	ePfxEndRot,
	ePfxEndRotVar,
	ePfxRotMode,

	ePfxInitScale,
	ePfxInitScaleVar,
	ePfxEndScale,
	ePfxEndScaleVar,
	ePfxScaleMode,

	ePfxInitCEmpty,

	ePfxInitR,
	ePfxInitG,
	ePfxInitB,

	ePfxInitRVar,
	ePfxInitGVar,
	ePfxInitBVar
};

const float RightSliderMaxValues[NUMRIGHTCOLUMNELEMENTS] ={
	20,	//Duration of Emit

	1.0f,	//Drag
	180.0f,	//Emitter Angle
	eZOrder_INVBOXITEMS, //Zorder
	0,	//Empty
	0,	//Empty
	0,	//Empty
	0,	//Empty
	0,	//Empty
	0,	//Empty
	0,	//Empty

	1.0f,	//End Red
	1.0f,	//End Green
	1.0f,	//End Blue
	1.0f,	//End Red variance
	1.0f,	//End Green variance
	1.0f	//End Blue variance
};

enum EnumParticleRightConsts
{
	ePfxDuration,

	ePfxRDrag,
	ePfxREmitterAngle,
	ePfxRZorder,
	ePfxREmpty3,
	ePfxREmpty4,
	ePfxREmpty5,
	ePfxREmpty6,
	ePfxREmpty7,
	ePfxREmpty8,
	ePfxREmpty9,

	ePfxEndColorR,
	ePfxEndColorG,
	ePfxEndColorB,
	ePfxEndColorRVar,
	ePfxEndColorGVar,
	ePfxEndColorBVar
};

std::string ParticleEditorTipTexts[NUMLEFTCOLUMNELEMENTS] =
{
	"Blend Mode [0-Additive : 1-Alpha]",
	"Source [0-Rect : 1-Circle : 2-Radial]",
	"Source Width",
	"Source Height",
	"Pool Count",
	"Emit Time",
	"Emit Parts Per Emit",
	"Uniform",
	"Emit Angle",
	"Emit Angle Variance",

	"Empty",

	"LifeSpan",
	"LifeSpan Variance",
	"Speed of Particle",
	"Speed Variance",
	"X Acceleration",
	"Y Acceleration",
	"Start Alpha",
	"Start Alpha Variance",
	"End Alpha",
	"End Alpha Variance",
	"Alpha Mode [0:Linear, 1:Wave Mode]",
};

std::string ParticleEffectModeTexts[4] =
{
	"Linear",
	"Wave",
	"Empty",
	"Empty"
};

std::string SrcTypeTexts[6] =
{
	"Rect",
	"Circle",
	"Radial",
	"RadialSquare",
	"RadialOut",
	"RadialIN"
};

std::string BlendTypeTexts[5] =
{
	"AlphaAddit",
	"AlphaNormal",//Normal
	"AlphaSatSub",
	"AlphaSub",
	"AlphaMultiply"
};

enum EnumParticleLeftConsts
{
	ePfxBlendMode,
	ePfxSourceType,
	ePfxSourceWidth,
	ePfxSourceHeight,
	ePfxPoolSize,
	ePfxEmitTimer,
	ePfxEmitPartPer,
	ePfxUniformEmit,
	ePfxEmitAngle,
	ePfxEmitAngleVar,

	ePfxEmpty1,

	ePfxLife,
	ePfxLifeVar,
	ePfxSpeed,
	ePfxSpeedVar,
	ePfxAccelX,
	ePfxAccelY,
	ePfxStartAlpha,
	ePfxStartAlphaVar,
	ePfxEndAlpha,
	ePfxEndAlphaVar,
	ePfxAlphaMode
};

const float TEXT_OFFSET_X=10;
const float TEXT_OFFSET_Y=10;

ParticleEditorControl::ParticleEditorControl()
{		
	MessageText = NULL;
	MessageElement = NULL;
	//If we come not through splash scene[for fast debug], we need to wait till loading done
	if( GFInstance->IsPakResLoadInited && !GFInstance->IsPakResLoadDone )
	{
		GFInstance->WaitTillPakResLoadDone();
		GFInstance->IsPakResLoadDone = true;
	}

	IsDisabled = false;

	baseZOrder = eZOrder_INVBOXITEMS+100;
	defaultParticleZOrder = eZOrder_Popup+100;
	
	//Load Scene
	MainScene = new CHOScene();
	MainScene->Load("NONGAME\\PRTED\\PRTED.SCN");
	MainScene->SetZOff(baseZOrder);

	currentParticleId = -1;

	srcWidth = 10;
	srcHeight = 10;

	activeId = 0;
	activeColumn = 0;

	resetDelayTimer = 0;

	srcType = 0;

	//Particle Editor Stuffs starts here
	EditorBg = MainScene->GetObjectByID("EditorBg");

	//UI Stuffs
	UI_CenterDot = MainScene->GetObjectByID("originPos");
	UI_EmitDir = MainScene->GetObjectByID("emitDirection");
	UI_EmitVarMin = MainScene->GetObjectByID("emitvarmin");
	UI_EmitVarMax = MainScene->GetObjectByID("emitvarmax");

	UI_EmitDir->SetPos(UI_CenterDot->GetPosPointer());
	UI_EmitVarMax->SetPos(UI_CenterDot->GetPosPointer());
	UI_EmitVarMin->SetPos(UI_CenterDot->GetPosPointer());

	IsCenterDragging = false;

	IsUIHiding = false;

	DebugGraphic = KPTK::createKGraphic();
	DebugGraphic->setTextureQuality(true);

	skipEmitFrames = 0;

	ActiveVFX = NULL;

	/*ActiveVFX = new SuperFX();
	ActiveVFX->LoadEffectIni("PRT\\PV2.INI");
	ActiveVFX->SetPos(UI_CenterDot->GetPosPointer());
	ActiveVFX->SetZ(5000);
	ActiveVFX->Start();*/

	currentLeftKnobId = -1;
	currentCenterKnobId = -1;
	currentRightKnobId = -1;

	//Left Knobs
	float startY = 80;
	float rowWidth = 30;
	for (int i = 0; i < NUMLEFTCOLUMNELEMENTS; i++)
	{
		//Rect Areas
		LeftColumnRects[i] = new CRectangle(LEFTSLIDER_STARTX,startY+(rowWidth*i)-10,(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX),30);

		//Knobs
		LeftColumnKnobs[i] = new CObject();
		LeftColumnKnobs[i]->LoadSprite("NONGAME\\PRTED\\KNOB.SPR");
		LeftColumnKnobs[i]->SetPos(PointSystem::CreateCPoint(LEFTSLIDER_STARTX,startY+(rowWidth*i),0));
		LeftColumnKnobs[i]->SetZ(baseZOrder+10);
		
		LeftColumnKnobYPos[i] = startY+(rowWidth*i);

		LeftColumnValTexts[i] = new CBitmapText();
		LeftColumnValTexts[i]->LoadFont(eFont_Italic24);
		LeftColumnValTexts[i]->SetPos(PointSystem::CreateCPoint(LEFTSLIDER_ENDX-25, startY+(rowWidth*i)-15, 0.0f));
		LeftColumnValTexts[i]->SetAlignment(Align_Left);
		LeftColumnValTexts[i]->SetZ(baseZOrder+15);
		LeftColumnValTexts[i]->SetColor((120/255.0f),(240/255.0f),(120/255.0f)); //green

		//flag
		IsLeftColKnobDragStarted[i] = false;

		//UPDATE Editor values
		//UpdateEditorValues(i,LEFTCOL);
	}

	for (int i = 0; i < NUMCENTERCOLUMNELEMENTS; i++)
	{
		//Center Knobs
		//Rect Areas
		CenterColumnRects[i] = new CRectangle(CENTERSLIDER_STARTX,startY+(rowWidth*i)-10,(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX),30);

		//Knobs
		CenterColumnKnobs[i] = new CObject();
		CenterColumnKnobs[i]->LoadSprite("NONGAME\\PRTED\\KNOB.SPR");
		CenterColumnKnobs[i]->SetPos(PointSystem::CreateCPoint(CENTERSLIDER_STARTX,startY+(rowWidth*i),0));
		CenterColumnKnobs[i]->SetZ(baseZOrder+10);
		//CenterColumnKnobs[i]->Show(false);
		
		CenterColumnKnobYPos[i] = startY+(rowWidth*i);

		CenterColumnValTexts[i] = new CBitmapText();
		CenterColumnValTexts[i]->LoadFont(eFont_Italic24);
		CenterColumnValTexts[i]->SetPos(PointSystem::CreateCPoint(CENTERSLIDER_ENDX-15, startY+(rowWidth*i)-15, 0.0f));
		CenterColumnValTexts[i]->SetAlignment(Align_Left);
		CenterColumnValTexts[i]->SetZ(baseZOrder+15);
		CenterColumnValTexts[i]->SetColor((100/255.0f),(189/255.0f),(100/255.0f)); //green		

		//flag
		IsCenterColKnobDragStarted[i] = false;

		//UPDATE Editor values
		//UpdateEditorValues(i,CENTERCOL);
	}

	for (int i = 0; i < NUMRIGHTCOLUMNELEMENTS; i++)
	{
		//Right Knobs
		RightColumnRects[i] = new CRectangle(RIGHTSLIDER_STARTX,startY+(rowWidth*i)-10,(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX),30);

		//Knobs
		RightColumnKnobs[i] = new CObject();
		RightColumnKnobs[i]->LoadSprite("NONGAME\\PRTED\\KNOB.SPR");
		RightColumnKnobs[i]->SetPos(PointSystem::CreateCPoint(RIGHTSLIDER_STARTX,startY+(rowWidth*i),0));
		RightColumnKnobs[i]->SetZ(baseZOrder+10);
		//RightColumnKnobs[i]->Show(false);
		
		RightColumnKnobYPos[i] = startY+(rowWidth*i);

		RightColumnValTexts[i] = new CBitmapText();
		RightColumnValTexts[i]->LoadFont(eFont_Italic24);
		RightColumnValTexts[i]->SetPos(PointSystem::CreateCPoint(RIGHTSLIDER_ENDX-15, startY+(rowWidth*i)-15, 0.0f));
		RightColumnValTexts[i]->SetAlignment(Align_Left);
		RightColumnValTexts[i]->SetZ(baseZOrder+15);
		RightColumnValTexts[i]->SetColor((100/255.0f),(189/255.0f),(100/255.0f)); //green		

		//flag
		IsRightColKnobDragStarted[i] = false;

		//UPDATE Editor values
		//UpdateEditorValues(i,RIGHTCOL);
	}

	InitMessageBanner();

	StartColorImg   = CTexFactory::Instance->GetImage("NONGAME\\PRTED\\COLORBOX.PNG");
	EndColorImg		= CTexFactory::Instance->GetImage("NONGAME\\PRTED\\COLORBOX.PNG");
		
	ConsoleText = new CBitmapText();
	ConsoleText->LoadFont(eFont_Italic24);
	ConsoleText->SetPos(PointSystem::CreateCPoint(10.0f, 35.0f, 0.0f));
	ConsoleText->SetAlignment(Align_Left);
	ConsoleText->SetZ(baseZOrder+15);
	ConsoleText->SetColor((120/255.0f),(240/255.0f),(120/255.0f)); //green
	ConsoleText->SetText("Particle Editor v0.120");
	ConsoleText->Show(false);

	ParticleNameTextEntry = NULL;

	ParticleNameTextEntry = new TextBox();
	ParticleNameTextEntry->SetXY(180,50);//1366x768
	ParticleNameTextEntry->SetZ(baseZOrder+100);		

	AddControl(ParticleNameTextEntry);
	ParticleNameTextEntry->Show(false);

	isParticleNameEntryActive = false;

	PositionText = new CBitmapText();
	PositionText->LoadFont(eFont_Italic24);
	PositionText->SetPos(PointSystem::CreateCPoint(10.0f, 55.0f, 0.0f));//578 r
	PositionText->SetAlignment(Align_Left);
	PositionText->SetZ(baseZOrder+15);
	PositionText->SetColor((120/255.0f),(240/255.0f),(120/255.0f)); //green
	PositionText->SetText("");
	//PositionText->SetText(std::string("Pos:" + MKSTR(ActiveVFX->Pos.x)+","+MKSTR(ActiveVFX->Pos.y)));
	
	SetZ(baseZOrder+20);
	Application::SetSingleUpdate(true);
	Visible = true;//CRenderListEntity
}

void ParticleEditorControl::Update()
{		
	UpdateBanner();

	PrevCursorMode = Cursor::GetMode();
	Cursor::SetMode(CA_Normal); 
	CPoint mosPos = Control::Input->Mpos();

	if(isParticleNameEntryActive)
	{
		if(ParticleNameTextEntry->CheckForEnter())
		{
			isParticleNameEntryActive = false;

			ActiveVFX->SetUID(ParticleNameTextEntry->String);
			_particleNameTextArray[currentParticleId]->SetText(ParticleNameTextEntry->String);
			ParticleNameTextEntry->Show(false);
			ParticleNameTextEntry->Clear();
		}
		else if( Control::Input->Kpressed(K_VK_ESCAPE) )
		{
			isParticleNameEntryActive = false;

			//cancel
			ParticleNameTextEntry->Show(false);
			ParticleNameTextEntry->Clear();
		}		
		return;
	}

	if(ActiveVFX!=NULL)
		HandleValueSliderUpdate(mosPos);

    int size = (int)_outlineRects.size();  //Sac: converted Implicit to explicit

	if(IsCenterDragging)
	{
		Cursor::SetMode(CA_HandPointy);
		UI_CenterDot->SetScale(1.12f);
		UI_CenterDot->SetPos(&mosPos);
		
		_outlineRects[currentParticleId]->topleft.x = mosPos.x-5;
		_outlineRects[currentParticleId]->topleft.y = mosPos.y-5;

		if(ActiveVFX!=NULL)
		{
			ActiveVFX->SetPos(UI_CenterDot->GetPosPointer());
			_particleNameTextArray[currentParticleId]->SetPos(PointSystem::CreateCPoint(ActiveVFX->Pos.x + TEXT_OFFSET_X, ActiveVFX->Pos.y + TEXT_OFFSET_Y, 0));
		}

		UI_EmitDir->SetPos(UI_CenterDot->GetPosPointer());
		UI_EmitVarMax->SetPos(UI_CenterDot->GetPosPointer());
		UI_EmitVarMin->SetPos(UI_CenterDot->GetPosPointer());
		if( Control::Input->LBdown() == false )
		{
			//PositionText->SetText(std::string("Pos:" + MKSTR(ActiveVFX->Pos.x)+","+MKSTR(ActiveVFX->Pos.y)));
			IsCenterDragging = false;
			UI_CenterDot->SetScale(1.0f);
		}
		return;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			if(_outlineRects[i]->Intersection(&mosPos))
			{
				Cursor::SetMode(CA_HandPointy);
				if(Control::Input->LBjustdown())
				{
					//Select that VFX
					currentParticleId = i;
					ActiveVFX = _particlesArray[i];

					SetUIValuesWithActiveVFX();

					UI_CenterDot->SetPos(&mosPos);

					_outlineRects[currentParticleId]->topleft.x = mosPos.x-5;
					_outlineRects[currentParticleId]->topleft.y = mosPos.y-5;

					if(ActiveVFX!=NULL)	ActiveVFX->SetPos(UI_CenterDot->GetPosPointer());

					UI_EmitDir->SetPos(UI_CenterDot->GetPosPointer());
					UI_EmitVarMax->SetPos(UI_CenterDot->GetPosPointer());
					UI_EmitVarMin->SetPos(UI_CenterDot->GetPosPointer());
					//Update UI

					IsCenterDragging = true;
				}
				break;
			}
		} 
	}

	if( Control::Input->Kdown(K_VK_L_CONTROL) )
	{
		//open VFX file
		if( Control::Input->Kpressed(K_VK_O) )
		{
			OpenFileAndAddToScene(0);
		}//replace image file
		else if( Control::Input->Kpressed(K_VK_I) )
		{
			OpenFileAndAddToScene(1);
		}
		else if( Control::Input->Kpressed(K_VK_S) )
		{
			WriteParticleFileData();
		}
		/*else if( Control::Input->Kpressed(K_VK_D) )
		{
			IsDisabled = !IsDisabled;

			if(IsDisabled)
				ShowUI(false);
			else
				ShowUI(true);
		}*/
	}
	else if( !Control::Input->LBdown() && Control::Input->Kpressed(K_VK_R) )
	{
		if(ActiveVFX!=NULL)
			ActiveVFX->Start();
	}
	else if( Control::Input->Kpressed(K_VK_H) )
	{
		IsUIHiding = !IsUIHiding;
		
		ShowUI(!IsUIHiding);
	}
	else if( Control::Input->Kpressed(K_VK_DEL) )
	{
		deleteCurrentParticle();
	}
	else if( Control::Input->Kpressed(K_VK_D) )
	{
		duplicateCurrentParticle();
	}
	else if( Control::Input->Kpressed(K_VK_F2) )
	{
		ParticleNameTextEntry->Show(true);
		ParticleNameTextEntry->Clear();
		isParticleNameEntryActive = true;

		ConsoleText->SetText("Enter Particle Name");
	}
	
}

void ParticleEditorControl::duplicateCurrentParticle()
{
	if(ActiveVFX!=NULL)
	{
		SuperFX *vfx = new SuperFX();
		vfx->LoadEffectIni(ActiveVFX->Filename);
		vfx->SetPos(PointSystem::CreateCPoint(683,384,0));
		vfx->SetZ(defaultParticleZOrder);
		vfx->Start();

		vfx->SetUID(ActiveVFX->UID);

		ColorRGB blue = {(255/255.0f),(255/255.0f),(255/255.0f)};
		CBitmapText* _fxText = new CBitmapText();
		_fxText->SetTextProps(vfx->UID, vfx->Pos.x+TEXT_OFFSET_X, vfx->Pos.y+TEXT_OFFSET_Y, defaultParticleZOrder+10, blue, eFont_Italic24);
		_fxText->SetAlpha(0.75f);
		_fxText->SetScale(0.75f);

		_particleNameTextArray.push_back(_fxText);

		_particlesArray.push_back(vfx);
        currentParticleId = (int)_particlesArray.size()-1;  //Sac: converted Implicit to explicit


		ActiveVFX = _particlesArray[currentParticleId];

		SetUIValuesWithActiveVFX();

		CRectangle *rect = new CRectangle(ActiveVFX->Pos.x-5, ActiveVFX->Pos.y-5, 10, 10);
		_outlineRects.push_back(rect);

		UI_CenterDot->SetPos(&ActiveVFX->Pos);

		UI_EmitDir->SetPos(ActiveVFX->GetPosPointer());
		UI_EmitVarMax->SetPos(ActiveVFX->GetPosPointer());
		UI_EmitVarMin->SetPos(ActiveVFX->GetPosPointer());
		//Update UI

		ConsoleText->SetText(std::string("FX duplicated " + ActiveVFX->Filename).c_str());
	}
}

void ParticleEditorControl::deleteCurrentParticle()
{
	if(currentParticleId != -1)
	{
		ActiveVFX = NULL;

		//SuperFX *fxTemp = _particlesArray.at(currentParticleId);
		SAFE_DELETE(_particlesArray.at(currentParticleId));
		SAFE_DELETE(_outlineRects.at(currentParticleId));
		SAFE_DELETE(_particleNameTextArray.at(currentParticleId));

		//Delete the FX in this pos
		_particlesArray.erase(_particlesArray.begin()+currentParticleId);
		_outlineRects.erase(_outlineRects.begin()+currentParticleId);
		_particleNameTextArray.erase(_particleNameTextArray.begin()+currentParticleId);

        int size =(int)_particlesArray.size();  //Sac: converted Implicit to explicit

		if(size>0)
		{
			//Select the last one 
			ActiveVFX = _particlesArray.at(0);
			currentParticleId = 0;
			SetUIValuesWithActiveVFX();

			UI_CenterDot->SetPos(&ActiveVFX->Pos);

			UI_EmitDir->SetPos(ActiveVFX->GetPosPointer());
			UI_EmitVarMax->SetPos(ActiveVFX->GetPosPointer());
			UI_EmitVarMin->SetPos(ActiveVFX->GetPosPointer());
		}
	}
}

void ParticleEditorControl::ShowUI(bool hide)
{
	EditorBg->Show(hide);
	UI_CenterDot->Show(hide);
	UI_EmitDir->Show(hide);
	UI_EmitVarMin->Show(hide);
	UI_EmitVarMax->Show(hide);

    int size = (int)_particleNameTextArray.size(); //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)
	{
		_particleNameTextArray[i]->Show(hide);
	}

	for (int i = 0; i < NUMLEFTCOLUMNELEMENTS; i++){
		LeftColumnKnobs[i]->Show(hide);
		LeftColumnValTexts[i]->Show(hide);
	}

	for (int i = 0; i < NUMCENTERCOLUMNELEMENTS; i++){
		CenterColumnKnobs[i]->Show(hide);
		CenterColumnValTexts[i]->Show(hide);
	}

	for (int i = 0; i < NUMRIGHTCOLUMNELEMENTS; i++){
		RightColumnKnobs[i]->Show(hide);
		RightColumnValTexts[i]->Show(hide);
	}
}

void ParticleEditorControl::SetUIValuesWithActiveVFX()
{
	if(ActiveVFX == NULL)
		return;

	for (int i = 0; i < NUMLEFTCOLUMNELEMENTS; i++)
	{
		//UPDATE Editor values
		UpdateEditorValues(i,LEFTCOL);
	}

	for (int i = 0; i < NUMCENTERCOLUMNELEMENTS; i++)
	{
		//UPDATE Editor values
		UpdateEditorValues(i,CENTERCOL);
	}

	for (int i = 0; i < NUMRIGHTCOLUMNELEMENTS; i++)
	{
		//UPDATE Editor values
		UpdateEditorValues(i,RIGHTCOL);
	}
}

void ParticleEditorControl::HandleValueSliderUpdate(CPoint mosPos)
{
	if(resetDelayTimer>0)
	{
		resetDelayTimer--;
		return;
	}

	if( currentLeftKnobId != -1 || currentCenterKnobId != -1 || currentRightKnobId != -1 )
	{
		if ( currentLeftKnobId != -1 )
		{
			if(LeftColumnRects[currentLeftKnobId]->Intersection(&mosPos))
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBdown() == false )
				{
					LeftColumnKnobs[currentLeftKnobId]->PlayAnimation(0);
					IsLeftColKnobDragStarted[currentLeftKnobId] = false; 
					currentLeftKnobId = -1;
				}
				else
				{
					if( mosPos.x < LEFTSLIDER_STARTX )
						mosPos.x = LEFTSLIDER_STARTX;
					else if( mosPos.x > LEFTSLIDER_ENDX )
						mosPos.x = LEFTSLIDER_ENDX;
					mosPos.y = LeftColumnKnobYPos[currentLeftKnobId];

					float val = ((mosPos.x-LEFTSLIDER_STARTX)*LeftSliderMaxValues[currentLeftKnobId])/(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX);

					UpdateParticleWithValue(currentLeftKnobId, val, LEFTCOL);
					LeftColumnKnobs[currentLeftKnobId]->SetPos(&mosPos);

					//Reset Values to default
					if( Control::Input->Kpressed(K_VK_R) )
					{
						UpdateParticleWithDefault(currentLeftKnobId, LEFTCOL);

						//cancel dragging
						LeftColumnKnobs[currentLeftKnobId]->PlayAnimation(0);
						IsLeftColKnobDragStarted[currentLeftKnobId] = false; 
						currentLeftKnobId = -1;

						resetDelayTimer = 10;

						Control::Input->ClearMouse();
					}
				}
			}
			else if( Control::Input->LBdown() == false )
			{
				LeftColumnKnobs[currentLeftKnobId]->PlayAnimation(0);
				IsLeftColKnobDragStarted[currentLeftKnobId] = false; 
				currentLeftKnobId = -1;
			} 
		}
		else if ( currentCenterKnobId != -1 )
		{
			if(CenterColumnRects[currentCenterKnobId]->Intersection(&mosPos))
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBdown() == false )
				{
					CenterColumnKnobs[currentCenterKnobId]->PlayAnimation(0);
					IsCenterColKnobDragStarted[currentCenterKnobId] = false; 
					currentCenterKnobId = -1;
				}
				else
				{
					if( mosPos.x < CENTERSLIDER_STARTX )
						mosPos.x = CENTERSLIDER_STARTX;
					else if( mosPos.x > CENTERSLIDER_ENDX )
						mosPos.x = CENTERSLIDER_ENDX;
					mosPos.y = CenterColumnKnobYPos[currentCenterKnobId];

					float val = ((mosPos.x-CENTERSLIDER_STARTX)*CenterSliderMaxValues[currentCenterKnobId])/(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX);

					UpdateParticleWithValue(currentCenterKnobId, val, CENTERCOL);
					CenterColumnKnobs[currentCenterKnobId]->SetPos(&mosPos);

					//Reset Values to default
					if( Control::Input->Kpressed(K_VK_R) )
					{
						UpdateParticleWithDefault(currentCenterKnobId, CENTERCOL);

						//cancel dragging
						CenterColumnKnobs[currentCenterKnobId]->PlayAnimation(0);
						IsCenterColKnobDragStarted[currentCenterKnobId] = false; 
						currentCenterKnobId = -1;

						resetDelayTimer = 10;

						Control::Input->ClearMouse();
					}
				}
			}
			else if( Control::Input->LBdown() == false )
			{
				CenterColumnKnobs[currentCenterKnobId]->PlayAnimation(0);
				IsCenterColKnobDragStarted[currentCenterKnobId] = false; 
				currentCenterKnobId = -1;
			} 
		}
		else if ( currentRightKnobId != -1 )
		{
			if(RightColumnRects[currentRightKnobId]->Intersection(&mosPos))
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBdown() == false )
				{
					RightColumnKnobs[currentRightKnobId]->PlayAnimation(0);
					IsRightColKnobDragStarted[currentRightKnobId] = false; 
					currentRightKnobId = -1;
				}
				else
				{
					if( mosPos.x < RIGHTSLIDER_STARTX )
						mosPos.x = RIGHTSLIDER_STARTX;
					else if( mosPos.x > RIGHTSLIDER_ENDX )
						mosPos.x = RIGHTSLIDER_ENDX;
					mosPos.y = RightColumnKnobYPos[currentRightKnobId];

					float val = ((mosPos.x-RIGHTSLIDER_STARTX)*RightSliderMaxValues[currentRightKnobId])/(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX);

					UpdateParticleWithValue(currentRightKnobId, val, RIGHTCOL);
					RightColumnKnobs[currentRightKnobId]->SetPos(&mosPos);

					//Reset Values to default
					if( Control::Input->Kpressed(K_VK_R) )
					{
						UpdateParticleWithDefault(currentRightKnobId, RIGHTCOL);

						//cancel dragging
						RightColumnKnobs[currentRightKnobId]->PlayAnimation(0);
						IsRightColKnobDragStarted[currentRightKnobId] = false; 
						currentRightKnobId = -1;

						resetDelayTimer = 10;

						Control::Input->ClearMouse();
					}
				}
			}
			else if( Control::Input->LBdown() == false )
			{
				RightColumnKnobs[currentRightKnobId]->PlayAnimation(0);
				IsRightColKnobDragStarted[currentRightKnobId] = false; 
				currentRightKnobId = -1;
			} 
		}
	}
	else
	{
		//Knob Checks
		for (int i = 0; i < NUMLEFTCOLUMNELEMENTS; i++)
		{
			if( !IsLeftColKnobDragStarted[i] )
			{
				CObject *Knob = LeftColumnKnobs[i];			
				if(Knob->Collide(&mosPos))
				{
					Cursor::SetMode(CA_HandPointy);
					ConsoleText->SetText(ParticleEditorTipTexts[i]);
					if( Control::Input->LBjustdown() )
					{
						activeId = i;
						activeColumn = LEFTCOL;
						Knob->PlayAnimation(1);
						IsLeftColKnobDragStarted[i] = true;
						currentLeftKnobId = i;
					}
					break;
				}
			}
		}

		if (currentLeftKnobId == -1)
		{
			for (int i = 0; i < NUMCENTERCOLUMNELEMENTS; i++)
			{
				if( !IsCenterColKnobDragStarted[i] )
				{
					CObject *Knob = CenterColumnKnobs[i];			
					if(Knob->Collide(&mosPos))
					{
						Cursor::SetMode(CA_HandPointy);
						//ConsoleText->SetText(ParticleEditorTipTexts[i]);
						if( Control::Input->LBjustdown() )
						{
							activeId = i;
							activeColumn = CENTERCOL;

							Knob->PlayAnimation(1);
							IsCenterColKnobDragStarted[i] = true;
							currentCenterKnobId = i;
						}
						break;
					}
				}
			} 
		}

		if ( currentLeftKnobId == -1 && currentCenterKnobId == -1 )
		{
			for (int i = 0; i < NUMRIGHTCOLUMNELEMENTS; i++)
			{
				if( !IsRightColKnobDragStarted[i] )
				{
					CObject *Knob = RightColumnKnobs[i];			
					if(Knob->Collide(&mosPos))
					{
						Cursor::SetMode(CA_HandPointy);
						//ConsoleText->SetText(ParticleEditorTipTexts[i]);
						if( Control::Input->LBjustdown() )
						{
							activeId = i;
							activeColumn = RIGHTCOL;

							Knob->PlayAnimation(1);
							IsRightColKnobDragStarted[i] = true;
							currentRightKnobId = i;
						}
						break;
					}
				}
			} 
		}
	}
}

void ParticleEditorControl::OpenFileAndAddToScene(int fileType)
{
#ifdef K_WIN32
	if( !ProfileSystem::Instance->ActiveProfile.Windowed )
		GFApp->ToggleFullscreen();

	//REFERENCE : http://www.cplusplus.com/forum/windows/82432/

	//OPENING FILE FROM EXPLORER
	char buffer[MAX_PATH] = "";
	OPENFILENAMEA ofn = {0};  // note:  OPENFILENAMEA, not OPENFILENAME
	// the 'A' at the end specifies we want the 'char' version and not the 'TCHAR' version
	// if you want the 'wchar_t' version, you want to use OPENFILENAMEW instead

	ofn.lStructSize = sizeof(ofn);
	//ofn.hwndOwner = your_hwnd; // not entirely necessary if you don't have a window
	if( fileType == 0 )// VFX file
		ofn.lpstrFilter = "PRT_iniFiles\0*.ini\0All Files\0*.*\0";
	else if( fileType == 1 )
		ofn.lpstrFilter = "PNGJPG Files\0*.png\0*.jpg\0All Files\0*.*\0";

	ofn.nFilterIndex = 1; // for some reason this is 1-based not zero-based. grrrrr

	ofn.Flags = OFN_FILEMUSTEXIST;  // only allow the user to open files that actually exist

	// the most important bits:
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = MAX_PATH;  // size of our 'buffer' buffer

	// Now that we've prepped the struct, actually open the dialog:
	// the below function call actually opens the "File Open" dialog box, and returns
	// once the user exits out of it
	if( !GetOpenFileNameA( &ofn ) ) // <- again note the 'A' at the end, signifying the 'char' version
	{
		// code reaches here if the user hit 'Cancel'
		K_LOG("Cancelled");
	}
	else
	{
		// code reaches here if the user hit 'OK'. The full path of the file
		// they selected is now stored in 'buffer'
		K_LOG("File Opened");
		std::string filename(buffer);
		std::size_t pos = filename.find("Assets")+7;//Assets + /
		filename = filename.substr(pos);
		
		K_LOG(filename.c_str());

		if( fileType == 0 )
		{
			SuperFX *vfx = new SuperFX();
			vfx->LoadEffectIni(filename);
			vfx->SetPos(PointSystem::CreateCPoint(683,384,0));
			vfx->SetZ(defaultParticleZOrder);
			vfx->Start();

			ColorRGB blue = {(255/255.0f),(255/255.0f),(255/255.0f)};
			CBitmapText* _fxText = new CBitmapText();
			_fxText->SetTextProps(vfx->UID, vfx->Pos.x+TEXT_OFFSET_X, vfx->Pos.y+TEXT_OFFSET_Y, defaultParticleZOrder+10, blue, eFont_Italic24);
			_fxText->SetAlpha(0.75f);
			_fxText->SetScale(0.75f);

			_particleNameTextArray.push_back(_fxText);

			_particlesArray.push_back(vfx);
			currentParticleId = _particlesArray.size()-1;

			ActiveVFX = _particlesArray[currentParticleId];

			SetUIValuesWithActiveVFX();

			CRectangle *rect = new CRectangle(ActiveVFX->Pos.x-5, ActiveVFX->Pos.y-5, 10, 10);
			_outlineRects.push_back(rect);

			UI_CenterDot->SetPos(&ActiveVFX->Pos);

			UI_EmitDir->SetPos(ActiveVFX->GetPosPointer());
			UI_EmitVarMax->SetPos(ActiveVFX->GetPosPointer());
			UI_EmitVarMin->SetPos(ActiveVFX->GetPosPointer());
			//Update UI

			ConsoleText->SetText(std::string("file Loaded " + filename).c_str());
		}
		else if( fileType == 1 )
		{
			//Load Image texture for current particle
			if(currentParticleId!=-1)
			{
				CTexFactory::Instance->FreeImage(&_particlesArray[currentParticleId]->Texture);

				_particlesArray[currentParticleId]->Texture = CTexFactory::Instance->GetImage(filename,false,true);
				_particlesArray[currentParticleId]->Tw = _particlesArray[currentParticleId]->Texture->getWidth();
				_particlesArray[currentParticleId]->Th = _particlesArray[currentParticleId]->Texture->getHeight();
				_particlesArray[currentParticleId]->THalfw = 0.5f * _particlesArray[currentParticleId]->Tw;
				_particlesArray[currentParticleId]->THalfh = 0.5f * _particlesArray[currentParticleId]->Th;
			}
			else
				ShowMessageBanner("No active Particle FX");
		}		
	}
#else
	ConsoleText->SetText("This functionality is only availabe in Windows system");
#endif
}

void ParticleEditorControl::LoadPreset(int id)
{
	//SAFE_DELETE(ActiveVFX);

	/*ActiveVFX = new SuperFX();
	ActiveVFX->LoadEffectIni(PresetPaths[id]);
	ActiveVFX->SetPos(PointSystem::CreateCPoint(683,384,0));
	ActiveVFX->SetZ(5000);*/

	ConsoleText->SetText(std::string("Preset Loaded " + MKSTR(id)).c_str());
}

void ParticleEditorControl::UpdateParticleWithDefault(int currentKnobId, int column)
{
	float val=0;
	int intVal = (int)val;
	//round to 2 decimal places
	float rndVal = (float)((int)(val * 100)/100.0f);
	CPoint knobPos(0,0,0);

	if (column == LEFTCOL)
	{
		knobPos.y = LeftColumnKnobYPos[currentKnobId];
		switch (currentKnobId)
		{
		case ePfxBlendMode:
			intVal = 0;
			if(intVal<5)
			{
				ActiveVFX->BlendMode = (EBlend)intVal;
				LeftColumnValTexts[currentKnobId]->SetText( BlendTypeTexts[intVal] );
				knobPos.x = LEFTSLIDER_STARTX + intVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/5.0f;
			}
			break;

		case ePfxSourceType:
			intVal = 0;
			if(intVal<6)
			{				
				ActiveVFX->SourceType = EffectSourceV2(intVal);
				LeftColumnValTexts[currentKnobId]->SetText( SrcTypeTexts[intVal] );
				srcType = intVal;

				knobPos.x = LEFTSLIDER_STARTX + intVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/6.0f;
			}
			break;

		case ePfxSourceWidth:
			rndVal = 20;
			ActiveVFX->EmitSrcWidth = rndVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			srcWidth = rndVal;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxSourceHeight:
			rndVal = 20;
			ActiveVFX->EmitSrcHeight = rndVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			srcHeight = rndVal;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxPoolSize:
			intVal = 64;
			#ifdef _PARTICLEEDITORENABLED
				ActiveVFX->newPoolSize = intVal;
				ActiveVFX->isPoolChanged = true;
			#endif
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxEmitTimer:
			rndVal = 0.2f;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			ActiveVFX->EmitTimer = rndVal;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxEmitPartPer:
			rndVal = 5.0f;
			ActiveVFX->EmitPartsPer = rndVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxUniformEmit:
			val = 0;
			ActiveVFX->UniformEmission = (val>0.5f)?1:0;
			LeftColumnValTexts[currentKnobId]->SetText( (val>0.5f)?"On":"Off" );
			knobPos.x = LEFTSLIDER_STARTX + val*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxEmitAngle:
			intVal = 0;
			ActiveVFX->EmitAngle = (float)intVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );

			UI_EmitDir->SetRotate((float)intVal);
			UI_EmitVarMin->SetRotate((ActiveVFX->EmitAngle) - (float)(intVal));
			UI_EmitVarMax->SetRotate(ActiveVFX->EmitAngle + (float)(intVal));

			knobPos.x = LEFTSLIDER_STARTX + intVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxEmitAngleVar:
			intVal = 0;
			ActiveVFX->EmitAngleVar = (float)intVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );
			UI_EmitVarMin->SetRotate((ActiveVFX->EmitAngle- (float)(intVal)));
			UI_EmitVarMax->SetRotate(ActiveVFX->EmitAngle + (float)(intVal));

			knobPos.x = LEFTSLIDER_STARTX + intVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxLife:
			rndVal = 1.0f;
			ActiveVFX->LifeSpan = rndVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );

			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxLifeVar:
			rndVal = 0.0f;
			ActiveVFX->LifeSpanVar = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );

			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxSpeed:
			rndVal = 2.0f;
			ActiveVFX->Speed = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxSpeedVar:
			rndVal = 0.0f;
			ActiveVFX->SpeedVar = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxAccelX:
			ActiveVFX->AccelX = 0;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->AccelX)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId]+ (0.5f*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX));
			break;

		case ePfxAccelY:
			ActiveVFX->AccelY = 0;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->AccelY)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId]+ (0.5f*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX));
			break;

		case ePfxStartAlpha:
			rndVal = 1.0f;
			ActiveVFX->StartAlpha = rndVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->StartAlpha)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxStartAlphaVar:
			rndVal = 0.0f;
			ActiveVFX->StartAlphaVar = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->StartAlphaVar)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxEndAlpha:
			rndVal = 0.0f;
			ActiveVFX->EndAlpha = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->EndAlpha)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxEndAlphaVar:
			rndVal = 0.0f;
			ActiveVFX->EndAlphaVar = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->EndAlphaVar)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		case ePfxAlphaMode:
			intVal = 0;
			if(intVal<4)
			{
				ActiveVFX->AlphaMode = (ParticleEffectModeV2)intVal;
				LeftColumnValTexts[currentKnobId]->SetText( ParticleEffectModeTexts[intVal] );
			}
			knobPos.x = LEFTSLIDER_STARTX + intVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[currentKnobId];
			break;

		default:
			break;
		}
		LeftColumnKnobs[currentKnobId]->SetPos(&knobPos);
	}
	else if (column == CENTERCOL)
	{
		knobPos.y = CenterColumnKnobYPos[currentKnobId];
		switch (currentKnobId)
		{
		case ePfxInitRot:
			rndVal = 0.0f;
			ActiveVFX->StartRot = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxInitRotVar:
			rndVal = 0.0f;
			ActiveVFX->StartRotVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxEndRot:
			rndVal = 0.0f;
			ActiveVFX->EndRot = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxEndRotVar:
			rndVal = 0.0f;
			ActiveVFX->EndRotVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxRotMode:
			intVal = 0;
			if(intVal<4)
			{
				ActiveVFX->RotMode = (ParticleEffectModeV2)intVal;
				CenterColumnValTexts[currentKnobId]->SetText( ParticleEffectModeTexts[intVal] );
			}
			knobPos.x = CENTERSLIDER_STARTX + intVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxInitScale:
			rndVal = 1.0f;
			ActiveVFX->StartSize = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxInitScaleVar:
			rndVal = 0.0f;
			ActiveVFX->StartSizeVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxEndScale:
			rndVal = 0.0f;
			ActiveVFX->EndSize = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxEndScaleVar:
			rndVal = 0.0f;
			ActiveVFX->EndSizeVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxScaleMode:
			intVal = 0;
			if(intVal<4)
			{
				ActiveVFX->ScaleMode = (ParticleEffectModeV2)intVal;
				CenterColumnValTexts[currentKnobId]->SetText( ParticleEffectModeTexts[intVal] );
			}
			knobPos.x = CENTERSLIDER_STARTX + intVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;

		case ePfxInitR:
			rndVal = 0.0f;
			ActiveVFX->R = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxInitG:
			rndVal = 0.0f;
			ActiveVFX->G = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxInitB:
			rndVal = 0.0f;
			ActiveVFX->B = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;

		case ePfxInitRVar:
			rndVal = 0.0f;
			ActiveVFX->RVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxInitGVar:
			rndVal = 0.0f;
			ActiveVFX->GVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		case ePfxInitBVar:
			rndVal = 0.0f;
			ActiveVFX->BVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[currentKnobId];
			break;
		default:
			break;
		}
		CenterColumnKnobs[currentKnobId]->SetPos(&knobPos);
	}
	else if (column == RIGHTCOL)
	{
		knobPos.y = RightColumnKnobYPos[currentKnobId];
		switch (currentKnobId)
		{
		case ePfxDuration:
			rndVal = 0.0f;
			ActiveVFX->Duration = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;
		case ePfxRDrag:
			rndVal = 1.0f;
			ActiveVFX->Drag = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;

		case ePfxREmitterAngle:
			rndVal = 0.0f;

			ActiveVFX->EmitSrcAngle = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->EmitSrcAngle)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;

		case ePfxRZorder:
			intVal = 100;
			ActiveVFX->SetZ(intVal);
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + intVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;

		case ePfxEndColorR:
			rndVal = 0;
			ActiveVFX->TR = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;
		case ePfxEndColorG:
			rndVal = 0;
			ActiveVFX->TG = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;
		case ePfxEndColorB:
			rndVal = 0;
			ActiveVFX->TB = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;
		case ePfxEndColorRVar:
			rndVal = 0;
			ActiveVFX->TRVar = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;
		case ePfxEndColorGVar:
			rndVal = 0;
			ActiveVFX->TGVar = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;
		case ePfxEndColorBVar:
			rndVal = 0;
			ActiveVFX->TBVar = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[currentKnobId];
			break;

		default:
			break;
		} 
		RightColumnKnobs[currentKnobId]->SetPos(&knobPos);
	}

	ActiveVFX->StopImmediate();
	ActiveVFX->Start();
}


void ParticleEditorControl::UpdateEditorValues(int i, int column)
{
	int intVal = 0;
	float rndVal = 0;

	CPoint knobPos(0,0,0);

	if (column == LEFTCOL)
	{
		knobPos.y = LeftColumnKnobYPos[i];
		switch (i)
		{
		case ePfxBlendMode:		LeftColumnValTexts[i]->SetText( BlendTypeTexts[ActiveVFX->BlendMode] );
			intVal = (int)ActiveVFX->BlendMode;
			knobPos.x = LEFTSLIDER_STARTX + intVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/5.0f;
			break;

		case ePfxSourceType:	LeftColumnValTexts[i]->SetText( SrcTypeTexts[ActiveVFX->SourceType] );
			intVal = (int)ActiveVFX->SourceType;
			knobPos.x = LEFTSLIDER_STARTX + intVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/6.0f;
			srcType = intVal;
			break;

		case ePfxSourceWidth:	LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EmitSrcWidth)).c_str() );
			rndVal = ActiveVFX->EmitSrcWidth;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			srcWidth = rndVal;
			break;

		case ePfxSourceHeight:	LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EmitSrcHeight)).c_str() );
			rndVal = ActiveVFX->EmitSrcHeight;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			srcHeight = rndVal;
			break;

		case ePfxPoolSize:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->PartPool)).c_str() );
			intVal = ActiveVFX->PartPool;
			knobPos.x = LEFTSLIDER_STARTX + intVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxEmitTimer:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EmitTimer)).c_str() );
			rndVal = ActiveVFX->EmitTimer;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxEmitPartPer:	LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EmitPartsPer)).c_str() );
			rndVal = ActiveVFX->EmitPartsPer;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxUniformEmit:	LeftColumnValTexts[i]->SetText( (ActiveVFX->UniformEmission>0.5f)?"On":"Off" );
			rndVal = (ActiveVFX->UniformEmission>0.5f)?1.0f:0;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX);
			break;

		case ePfxEmitAngle:	LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EmitAngle)).c_str() );
			rndVal = ActiveVFX->EmitAngle;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			UI_EmitDir->SetRotate(rndVal);
			break;

		case ePfxEmitAngleVar:	LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EmitAngleVar)).c_str() );
			rndVal = ActiveVFX->EmitAngleVar;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			UI_EmitVarMin->SetRotate(ActiveVFX->EmitAngle - rndVal);
			UI_EmitVarMax->SetRotate(ActiveVFX->EmitAngle + rndVal);
			break;

		case ePfxLife:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->LifeSpan)).c_str() );
			rndVal = ActiveVFX->LifeSpan;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxLifeVar:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->LifeSpanVar)).c_str() );
			rndVal = ActiveVFX->LifeSpanVar;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxSpeed:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->Speed)).c_str() );
			rndVal = ActiveVFX->Speed;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxSpeedVar:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->SpeedVar)).c_str() );
			rndVal = ActiveVFX->SpeedVar;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxAccelX:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->AccelX)).c_str() );
			rndVal = ActiveVFX->AccelX;
			//Negative values
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i]+ (0.5f*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX));
			break;

		case ePfxAccelY:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->AccelY)).c_str() );
			rndVal = ActiveVFX->AccelY;
			//Negative values
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i]+ (0.5f*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX));
			break;
		
		case ePfxStartAlpha:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->StartAlpha)).c_str() );
			rndVal = ActiveVFX->StartAlpha;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxStartAlphaVar:		LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->StartAlphaVar)).c_str() );
			rndVal = ActiveVFX->StartAlphaVar;
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxEndAlpha:		rndVal = ActiveVFX->EndAlpha;
			LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxEndAlphaVar:	rndVal = ActiveVFX->EndAlphaVar;
			LeftColumnValTexts[i]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			knobPos.x = LEFTSLIDER_STARTX + rndVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		case ePfxAlphaMode:	intVal = (int)ActiveVFX->AlphaMode;
			LeftColumnValTexts[i]->SetText( ParticleEffectModeTexts[intVal] );
			knobPos.x = LEFTSLIDER_STARTX + intVal*(LEFTSLIDER_ENDX-LEFTSLIDER_STARTX)/LeftSliderMaxValues[i];
			break;

		default:
			knobPos.x = LEFTSLIDER_STARTX;
			break;
		}

		LeftColumnKnobs[i]->SetPos(&knobPos);
	}
	else if (column == CENTERCOL)
	{
		knobPos.y = CenterColumnKnobYPos[i];
		switch (i)
		{
		case ePfxInitRot:			 
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->StartRot)).c_str() );
			rndVal = ActiveVFX->StartRot;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxInitRotVar:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->StartRotVar)).c_str() );
			rndVal = ActiveVFX->StartRotVar;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxEndRot:			 
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EndRot)).c_str() );
			rndVal = ActiveVFX->EndRot;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxEndRotVar:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EndRotVar)).c_str() );
			rndVal = ActiveVFX->EndRotVar;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxRotMode:
			CenterColumnValTexts[i]->SetText( ParticleEffectModeTexts[ActiveVFX->RotMode] );
			intVal = (int)ActiveVFX->RotMode;
			knobPos.x = CENTERSLIDER_STARTX + intVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;

		case ePfxInitScale:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->StartSize)).c_str() );
			rndVal = ActiveVFX->StartSize;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxInitScaleVar:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->StartSizeVar)).c_str() );
			rndVal = ActiveVFX->StartSizeVar;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxEndScale:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EndSize)).c_str() );
			rndVal = ActiveVFX->EndSize;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxEndScaleVar:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EndSizeVar)).c_str() );
			rndVal = ActiveVFX->EndSizeVar;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxScaleMode:
			CenterColumnValTexts[i]->SetText( ParticleEffectModeTexts[ActiveVFX->ScaleMode] );
			intVal = (int)ActiveVFX->ScaleMode;
			knobPos.x = CENTERSLIDER_STARTX + intVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
	
		case ePfxInitR:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->R)).c_str() );
			rndVal = ActiveVFX->R;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxInitG:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->G)).c_str() );
			rndVal = ActiveVFX->G;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxInitB:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->B)).c_str() );
			rndVal = ActiveVFX->B;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxInitRVar:			 
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->TR)).c_str() );
			rndVal = ActiveVFX->TR;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxInitGVar:
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->TG)).c_str() );
			rndVal = ActiveVFX->TG;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		case ePfxInitBVar:			 
			CenterColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->TB)).c_str() );
			rndVal = ActiveVFX->TB;
			knobPos.x = CENTERSLIDER_STARTX + rndVal*(CENTERSLIDER_ENDX-CENTERSLIDER_STARTX)/CenterSliderMaxValues[i];
			break;
		default:
			knobPos.x = CENTERSLIDER_STARTX;
			break;
		}
		CenterColumnKnobs[i]->SetPos(&knobPos);
	}
	else if (column == RIGHTCOL)
	{
		knobPos.y = RightColumnKnobYPos[i];
		switch (i)
		{
		case ePfxDuration:
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->Duration)).c_str() );
			rndVal = ActiveVFX->Duration;
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;

		case ePfxRDrag:
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->Drag)).c_str() );
			rndVal = ActiveVFX->Drag;
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;

		case 	ePfxREmitterAngle:
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->EmitSrcAngle)).c_str() );
			rndVal = ActiveVFX->EmitSrcAngle;
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;

		case ePfxRZorder:
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->ZOrder)).c_str() );
			intVal = ActiveVFX->ZOrder;
			knobPos.x = RIGHTSLIDER_STARTX + intVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;

		case ePfxEndColorR:
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->TR)).c_str() );
			rndVal = ActiveVFX->TR;
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;
		case ePfxEndColorG:
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->TG)).c_str() );
			rndVal = ActiveVFX->TG;
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;
		case ePfxEndColorB:
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->TB)).c_str() );
			rndVal = ActiveVFX->TB;
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;
		case ePfxEndColorRVar:			 
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->TRVar)).c_str() );
			rndVal = ActiveVFX->TRVar;
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;
		case ePfxEndColorGVar:
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->TGVar)).c_str() );
			rndVal = ActiveVFX->TGVar;
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;
		case ePfxEndColorBVar:
			RightColumnValTexts[i]->SetText( std::string("" + MKSTR(ActiveVFX->TBVar)).c_str() );
			rndVal = ActiveVFX->TBVar;
			knobPos.x = RIGHTSLIDER_STARTX + rndVal*(RIGHTSLIDER_ENDX-RIGHTSLIDER_STARTX)/RightSliderMaxValues[i];
			break;
		default:
			knobPos.x = RIGHTSLIDER_STARTX;
			break;
		}
		RightColumnKnobs[i]->SetPos(&knobPos);
	}
}

void ParticleEditorControl::UpdateParticleWithValue(int currentKnobId, float val, int column)
{	
	int intVal = (int)val;
	//round to 2 decimal places
	float rndVal = (float)((int)(val * 100)/100.0f);

	if (column == LEFTCOL)
	{
		switch (currentKnobId)
		{
		case ePfxBlendMode:
			if(intVal<5)
			{
				ActiveVFX->BlendMode = (EBlend)intVal;
				LeftColumnValTexts[currentKnobId]->SetText( BlendTypeTexts[intVal] );
			}
			break;

		case ePfxSourceType:
			if(intVal<6)
			{
				ActiveVFX->SourceType = EffectSourceV2(intVal);
				LeftColumnValTexts[currentKnobId]->SetText( SrcTypeTexts[intVal] );
				srcType = intVal;
			}
			break;

		case ePfxSourceWidth:
			ActiveVFX->EmitSrcWidth = rndVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			srcWidth = rndVal;
			break;

		case ePfxSourceHeight:
			ActiveVFX->EmitSrcHeight = rndVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			srcHeight = rndVal;
			break;

		case ePfxPoolSize:
			#ifdef _PARTICLEEDITORENABLED
				ActiveVFX->newPoolSize = intVal;
				ActiveVFX->isPoolChanged = true;
			#endif
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );
			break;

		case ePfxEmitTimer:
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			ActiveVFX->EmitTimer = rndVal;
			break;

		case ePfxEmitPartPer:
			ActiveVFX->EmitPartsPer = rndVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );
			break;

		case ePfxUniformEmit:
			ActiveVFX->UniformEmission = (val>0.5f)?1:0;

			LeftColumnValTexts[currentKnobId]->SetText( (val>0.5f)?"On":"Off" );
			break;

		case ePfxEmitAngle:
			ActiveVFX->EmitAngle = (float)intVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );

			UI_EmitDir->SetRotate((float)intVal);
			UI_EmitVarMin->SetRotate((ActiveVFX->EmitAngle) - (float)(intVal));
			UI_EmitVarMax->SetRotate(ActiveVFX->EmitAngle + (float)(intVal));
			break;

		case ePfxEmitAngleVar:
			ActiveVFX->EmitAngleVar = (float)intVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );
			UI_EmitVarMin->SetRotate((ActiveVFX->EmitAngle- (float)(intVal)));
			UI_EmitVarMax->SetRotate(ActiveVFX->EmitAngle + (float)(intVal));
			break;

		case ePfxLife:
			ActiveVFX->LifeSpan = rndVal;
			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;

		case ePfxLifeVar:
			ActiveVFX->LifeSpanVar = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;

		case ePfxSpeed:
			ActiveVFX->Speed = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;

		case ePfxSpeedVar:
			ActiveVFX->SpeedVar = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;

		case ePfxAccelX:
			ActiveVFX->AccelX = rndVal - LeftSliderMaxValues[currentKnobId]*0.5f;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->AccelX)).c_str() );
			break;

		case ePfxAccelY:
			ActiveVFX->AccelY = rndVal - LeftSliderMaxValues[currentKnobId]*0.5f;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->AccelY)).c_str() );
			break;

		case ePfxStartAlpha:
			ActiveVFX->StartAlpha = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->StartAlpha)).c_str() );
			break;

		case ePfxStartAlphaVar:
			ActiveVFX->StartAlphaVar = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->StartAlphaVar)).c_str() );
			break;

		case ePfxEndAlpha:
			ActiveVFX->EndAlpha = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->EndAlpha)).c_str() );
			break;

		case ePfxEndAlphaVar:
			ActiveVFX->EndAlphaVar = rndVal;

			LeftColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(ActiveVFX->EndAlphaVar)).c_str() );
			break;

		case ePfxAlphaMode:
			if(intVal<4)
			{
				ActiveVFX->AlphaMode = (ParticleEffectModeV2)intVal;
				LeftColumnValTexts[currentKnobId]->SetText( ParticleEffectModeTexts[intVal] );
			}
			break;

		default:
			break;
		}
	}
	else if (column == CENTERCOL)
	{
		switch (currentKnobId)
		{
		case ePfxInitRot:
			ActiveVFX->StartRot = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxInitRotVar:
			ActiveVFX->StartRotVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxEndRot:
			ActiveVFX->EndRot = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxEndRotVar:
			ActiveVFX->EndRotVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxRotMode:
			if(intVal<4)
			{
				ActiveVFX->RotMode = (ParticleEffectModeV2)intVal;
				CenterColumnValTexts[currentKnobId]->SetText( ParticleEffectModeTexts[intVal] );
			}
			break;
		case ePfxInitScale:
			ActiveVFX->StartSize = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxInitScaleVar:
			ActiveVFX->StartSizeVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxEndScale:
			ActiveVFX->EndSize = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxEndScaleVar:
			ActiveVFX->EndSizeVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxScaleMode:
			if(intVal<4)
			{
				ActiveVFX->ScaleMode = (ParticleEffectModeV2)intVal;
				CenterColumnValTexts[currentKnobId]->SetText( ParticleEffectModeTexts[intVal] );
			}
			break;

		case ePfxInitR:
			ActiveVFX->R = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxInitG:
			ActiveVFX->G = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxInitB:
			ActiveVFX->B = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;

			case ePfxInitRVar:
			ActiveVFX->RVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxInitGVar:
			ActiveVFX->GVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxInitBVar:
			ActiveVFX->BVar = rndVal;
			CenterColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		default:
			break;
		} 
	}
	else if (column == RIGHTCOL)
	{
		switch (currentKnobId)
		{
		case ePfxDuration:
			ActiveVFX->Duration = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;

		case ePfxRDrag:
			rndVal = 0.5f + 0.5f*rndVal;
			ActiveVFX->Drag = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;

		case ePfxREmitterAngle:
			ActiveVFX->EmitSrcAngle = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;

		case ePfxRZorder:
			ActiveVFX->SetZ(intVal);
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(intVal)).c_str() );
			break;

		case ePfxEndColorR:
			ActiveVFX->TR = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxEndColorG:
			ActiveVFX->TG = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxEndColorB:
			ActiveVFX->TB = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxEndColorRVar:
			ActiveVFX->TRVar = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxEndColorGVar:
			ActiveVFX->TGVar = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;
		case ePfxEndColorBVar:
			ActiveVFX->TBVar = rndVal;
			RightColumnValTexts[currentKnobId]->SetText( std::string("" + MKSTR(rndVal)).c_str() );
			break;

		default:
			break;
		} 
	}

	ActiveVFX->StopImmediate();
	ActiveVFX->Start();
}

void ParticleEditorControl::ProcessMessage(sMessage message)
{
	/*if( message.Protocol == ControlExit )
		IsTextEnterKeyPressed = true;*/
}


void ParticleEditorControl::Show(bool f)
{
	MainScene->Show(f);
	ShowUI(f);
	ConsoleText->Show(f);
}


void ParticleEditorControl::Render(const CPoint* pCam)
{  
	if(!ConsoleText->Visible)
		return;
	
	if(IsDisabled)
		return;

	if( !IsUIHiding && ActiveVFX!=NULL)
	{
		StartColorImg->setBlitColor(ActiveVFX->R,ActiveVFX->G,ActiveVFX->B,1.0f);
		StartColorImg->blitRect(0,0,25,21,1080,360);

		EndColorImg->setBlitColor(ActiveVFX->TR,ActiveVFX->TG,ActiveVFX->TB,1.0f);
		EndColorImg->blitRect(0,0,25,21,1270,360);

        int size = (int)_outlineRects.size();  //Sac: converted Implicit to explicit

		for (int i = 0; i < size; i++)
			DebugGraphic->drawRect(_outlineRects[i]->Left(), _outlineRects[i]->Top(), _outlineRects[i]->Right(), _outlineRects[i]->Bottom(), 1.0f, 0.25f, 0.25f, 0.75f);
	}

	CPoint cPos = UI_CenterDot->Pos;
	//Draw Rectangle if type 0 or Ellipse
	if(srcType==EffectV2_RECT || srcType==EffectV2_RADIALSQR)
	{
		//Top right down left
		CPoint topLeft;
		topLeft.x = cPos.x - srcWidth*0.5f;
		topLeft.y = cPos.y - srcHeight*0.5f;

		CPoint topRight;
		topRight.x = cPos.x + srcWidth*0.5f;
		topRight.y = cPos.y - srcHeight*0.5f;

		CPoint bottomLeft;
		bottomLeft.x = cPos.x - srcWidth*0.5f;
		bottomLeft.y = cPos.y + srcHeight*0.5f;

		CPoint bottomRight;
		bottomRight.x = cPos.x + srcWidth*0.5f;
		bottomRight.y = cPos.y + srcHeight*0.5f;

		if(ActiveVFX && ActiveVFX->EmitSrcAngle > 0)
		{
			topLeft = Elixir::RotatePointByAngle(cPos, topLeft, ActiveVFX->EmitSrcAngle);
			topRight = Elixir::RotatePointByAngle(cPos, topRight, ActiveVFX->EmitSrcAngle);
			bottomLeft = Elixir::RotatePointByAngle(cPos, bottomLeft, ActiveVFX->EmitSrcAngle);
			bottomRight = Elixir::RotatePointByAngle(cPos, bottomRight, ActiveVFX->EmitSrcAngle);

		}
 
		DebugGraphic->drawLine(topLeft.x, topLeft.y, topRight.x, topRight.y, (255/255.0f),(100/255.0f),(100/255.0f),0.8f);
		DebugGraphic->drawLine(topRight.x, topRight.y, bottomRight.x, bottomRight.y, (255/255.0f),(100/255.0f),(100/255.0f),0.8f);
		DebugGraphic->drawLine(bottomRight.x, bottomRight.y, bottomLeft.x, bottomLeft.y, (255/255.0f),(100/255.0f),(100/255.0f),0.8f);
		DebugGraphic->drawLine(bottomLeft.x, bottomLeft.y, topLeft.x, topLeft.y, (255/255.0f),(100/255.0f),(100/255.0f),0.8f);
	}
	else if(srcType==EffectV2_CIRCLE || srcType==EffectV2_RADIAL || srcType==EffectV2_RADIALOUT || srcType == EffectV2_RADIALIN )
	{
		for (int i = 0; i < 360; i+=2)
		{
			DebugGraphic->drawLine(cPos.x + srcWidth*Elixir::Cos((float)i), cPos.y + srcHeight*Elixir::Sin((float)i), cPos.x + srcWidth*Elixir::Cos((float)(i+2)), cPos.y + srcHeight*Elixir::Sin((float)(i+2)), (255/255.0f),(100/255.0f),(100/255.0f),0.8f);
		}
	}
}



/////////////////////////////////////////////////////
void ParticleEditorControl::InitMessageBanner()
{	
	IsBannerStandStill = false;
	TransOutFactor = 1.0f;
	BannerTimer = 0.0f;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;

	MessageText = new CBitmapText();
	MessageText->SetZ(eZOrder_Banner+1);
	MessageText->SetAlignment(Align_Center);
	MessageText->SetPos(&GFApp->MessageTextPos);
	MessageText->LoadFont(eFont_26);
	// MessageText->SetText("Message Text");
	MessageText->SetText(StringTable::getStr("ExMessage_Text"));
	
	const CPoint MessageBannerPos(0.0f, 0.0f, 0.0f);
	MessageElement = new CObject();
	MessageElement->LoadSprite("UI\\BNR.SPR");
	MessageElement->SetZ(eZOrder_Banner);//above back button
	MessageElement->SetPos(&MessageBannerPos);

	HideMessageBanner();
}

bool ParticleEditorControl::HideMessageBanner(void)
{
	bool WasActiveMessage = MessageElement->Visible;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;
	MessageElement->Show(false);
	MessageText->Show(false);

	return WasActiveMessage;
}


void ParticleEditorControl::TransOutBanner()
{
	if( BannerState == eHMS_TransIn )
	{
		BannerTimer = 0.0f;
		BannerState = eHMS_TransOut;
	}
	else
		TransOutFactor = 2.0f;
}


void ParticleEditorControl::ShowMessageBanner(const std::string& message, int SFXId)
{
	IsBannerStandStill = false;
    TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;	
	
	MessageText->Show(true);
	MessageText->SetText(message);
	MessageElement->Show(true);

	MessageElement->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	/*switch( SFXId )
	{
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aNotifySoftSFX);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aNotifyHardSFX);break;
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aGlassBtnSFX);break;
	}*/
}

void ParticleEditorControl::UpdateBanner()
{
	if( BannerState == eHMS_Idle )
		return;

	if( BannerState == eHMS_TransIn )
	{
		BannerAlpha += 2.0f * Control::LogicRate;

		if( BannerAlpha >= MAX_BANNER_ALPHA )
		{
			BannerAlpha = MAX_BANNER_ALPHA;
		}

		BannerTimer += Control::LogicRate;
		if( BannerTimer >= 3.0f  || (BannerAlpha == MAX_BANNER_ALPHA && Control::Input->LBclicked()) )
		{
			BannerTimer = 0.0f;
			BannerState = eHMS_TransOut;
		}
	}
	else if( !IsBannerStandStill )
	{
		BannerAlpha -= TransOutFactor * Control::LogicRate;

		if( BannerAlpha <= 0.0f )
		{
			BannerAlpha = 0.0f;
			BannerState = eHMS_Idle;
			MessageElement->Show(false);
			MessageText->Show(false);
			BannerCloseNotify();
		}
	}

	MessageElement->SetAlpha(BannerAlpha);
	float MTAlpha = ( BannerAlpha > MAX_BANNERTEXT_ALPHA )? MAX_BANNERTEXT_ALPHA: BannerAlpha;
	MessageText->SetAlpha(MTAlpha);
}


//SAVING DATA to PMOD.ini
void ParticleEditorControl::WriteParticleFileData()
{
#ifdef K_WIN32

	int size = _particlesArray.size();
	if(size<=0)
	{
		ConsoleText->SetText("No Particle added. Ctrl-O to add");
		return;
	}

	if( !ProfileSystem::Instance->ActiveProfile.Windowed )
		GFApp->ToggleFullscreen();

	std::string declareCodeData="SuperFX ";
	std::string codeInitData="";
	std::string deleteCodeData="";
	
	//For storing code data
	std::string codeStringData="";
	
	OPENFILENAMEA ofn;

	//strcpy(szFileName, codeStringData.c_str());

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
    //ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "ini files (*.ini)\0*.ini\0All Files (*.*)\0*.*\0";
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "ini";

	for (int i = 0; i < size; i++)
	{
		ActiveVFX = _particlesArray[i];
		ofn.lpstrTitle = ActiveVFX->Filename.c_str();

		char szFileName[MAX_PATH] = "";
		ofn.lpstrFile = szFileName;
		strcpy(szFileName, ActiveVFX->UID.c_str());

		if(GetSaveFileNameA(&ofn))
		{
			CIOStream stream;
			
			stream.Open(szFileName, File_Write);
			
			//delimit szFilename using \\ and check for Assets relative path for

			//Copy szFilename to another array
			char tempFilename[MAX_PATH];
			size_t destination_size = sizeof (szFileName);
			strncpy(tempFilename, szFileName, destination_size);
			tempFilename[destination_size - 1] = '\0';

			std::string pathArray[10];
			int numPaths=0, assetPath=0;
			char * pch;
			pch = strtok (tempFilename," .\\");
			while (pch != NULL)
			{
				pathArray[numPaths] = pch;
				if(pathArray[numPaths] == "Assets")
					assetPath = numPaths+1;

				numPaths+=1;
				pch = strtok (NULL, " .\\");				
			}

			std::string iniFileName = pathArray[numPaths-2];
			std::string filepath="";
			for (int k = assetPath; k < numPaths-1; k++)
			{
				filepath += pathArray[k];
				if(k==(numPaths-2))
					filepath += ".INI";
				else
					filepath += "\\\\";
			}

			//Declare Code : eg SuperFX *vfx1, *vfx2;
			declareCodeData += ("*"+iniFileName+"FX");
			if(i==(size-1))
			{
				declareCodeData += ";\n\n";
			}
			else
			{
				declareCodeData += ", ";
			}

			//Code Body Data
			codeInitData += iniFileName + "FX = new SuperFX(\""+MKSTR(filepath)+"\", PointSystem::CreateCPoint("+MKSTR(ActiveVFX->Pos.x)+","+MKSTR(ActiveVFX->Pos.y)+",0), "+MKSTR(ActiveVFX->ZOrder)+"); \n";
			codeInitData += iniFileName + "FX->Start();\n";
			codeInitData += iniFileName + "FX->AdvanceByTime(2.0f);\n\n";

			//codeStringData += "SuperFX *" + iniFileName +"FX;\n";
			
			deleteCodeData += "SAFE_DELETE("+iniFileName+"FX);\n";

			//=====================
			// Write Emitter
			//=====================

			//Emitter Properties
			stream.WriteString("[EMT_PROP]");
			stream.WriteString(std::string("ver=" + MKSTR(ActiveVFX->Version)).c_str());

			stream.WriteString(std::string("tex=" + MKSTR(ActiveVFX->texName)).c_str());

			stream.WriteString(std::string("tx=" + MKSTR(ActiveVFX->Tx)).c_str());
			stream.WriteString(std::string("ty=" + MKSTR(ActiveVFX->Ty)).c_str());
			stream.WriteString(std::string("pw=" + MKSTR(ActiveVFX->Tw)).c_str());
			stream.WriteString(std::string("ph=" + MKSTR(ActiveVFX->Th)).c_str());

			stream.WriteString(std::string("var=" + MKSTR(ActiveVFX->Variants)).c_str());
			stream.WriteString(std::string("cols=" + MKSTR(ActiveVFX->VariantCols)).c_str());

			stream.WriteString(std::string("partpool=" + MKSTR(ActiveVFX->PartPool)).c_str());

			stream.WriteString(std::string("blend=" + MKSTR((int)ActiveVFX->BlendMode)).c_str());
	
			stream.WriteString(std::string("srctype=" + MKSTR((int)ActiveVFX->SourceType)).c_str());
			stream.WriteString(std::string("srcWidth=" + MKSTR(ActiveVFX->EmitSrcWidth)).c_str());
			stream.WriteString(std::string("srcHeight=" + MKSTR(ActiveVFX->EmitSrcHeight)).c_str());
	
			stream.WriteString(std::string("emittime=" + MKSTR(ActiveVFX->EmitTimer)).c_str());
			stream.WriteString(std::string("emitparts=" + MKSTR(ActiveVFX->EmitPartsPer)).c_str());

			stream.WriteString(std::string("uniform=" + MKSTR(ActiveVFX->UniformEmission)).c_str());

			stream.WriteString(std::string("emitAngle=" + MKSTR(ActiveVFX->EmitAngle)).c_str());
			stream.WriteString(std::string("emitAngleVar=" + MKSTR(ActiveVFX->EmitAngleVar)).c_str());

			stream.WriteString("[PRT_PROP]");
			stream.WriteString(std::string("life=" + MKSTR(ActiveVFX->LifeSpan)).c_str());
			stream.WriteString(std::string("lifevar=" + MKSTR(ActiveVFX->LifeSpanVar)).c_str());

			stream.WriteString(std::string("Speed=" + MKSTR(ActiveVFX->Speed)).c_str());
			stream.WriteString(std::string("SpeedVar=" + MKSTR(ActiveVFX->SpeedVar)).c_str());

			stream.WriteString(std::string("AccelX=" + MKSTR(ActiveVFX->AccelX)).c_str());
			stream.WriteString(std::string("AccelY=" + MKSTR(ActiveVFX->AccelY)).c_str());

			stream.WriteString(std::string("StartAlpha=" + MKSTR(ActiveVFX->StartAlpha)).c_str());
			stream.WriteString(std::string("StartAlphaVar=" + MKSTR(ActiveVFX->StartAlphaVar)).c_str());
			stream.WriteString(std::string("EndAlpha=" + MKSTR(ActiveVFX->EndAlpha)).c_str());
			stream.WriteString(std::string("EndAlphaVar=" + MKSTR(ActiveVFX->EndAlphaVar)).c_str());
			stream.WriteString(std::string("AlphaMode=" + MKSTR((int)ActiveVFX->AlphaMode)).c_str());

			stream.WriteString(std::string("StartRot=" + MKSTR(ActiveVFX->StartRot)).c_str());
			stream.WriteString(std::string("StartRotVar=" + MKSTR(ActiveVFX->StartRotVar)).c_str());
			stream.WriteString(std::string("EndRot=" + MKSTR(ActiveVFX->EndRot)).c_str());
			stream.WriteString(std::string("EndRotVar=" + MKSTR(ActiveVFX->EndRotVar)).c_str());
			stream.WriteString(std::string("RotMode=" + MKSTR((int)ActiveVFX->RotMode)).c_str());

			stream.WriteString(std::string("StartSize=" + MKSTR(ActiveVFX->StartSize)).c_str());
			stream.WriteString(std::string("StartSizeVar=" + MKSTR(ActiveVFX->StartSizeVar)).c_str());
			stream.WriteString(std::string("EndSize=" + MKSTR(ActiveVFX->EndSize)).c_str());
			stream.WriteString(std::string("EndSizeVar=" + MKSTR(ActiveVFX->EndSizeVar)).c_str());
			stream.WriteString(std::string("ScaleMode=" + MKSTR((int)ActiveVFX->ScaleMode)).c_str());

			stream.WriteString(std::string("R=" + MKSTR(ActiveVFX->R)).c_str());
			stream.WriteString(std::string("G=" + MKSTR(ActiveVFX->G)).c_str());
			stream.WriteString(std::string("B=" + MKSTR(ActiveVFX->B)).c_str());

			stream.WriteString(std::string("Rvar=" + MKSTR(ActiveVFX->RVar)).c_str());
			stream.WriteString(std::string("Gvar=" + MKSTR(ActiveVFX->GVar)).c_str());
			stream.WriteString(std::string("Bvar=" + MKSTR(ActiveVFX->BVar)).c_str());

			stream.WriteString(std::string("TR=" + MKSTR(ActiveVFX->TR)).c_str());
			stream.WriteString(std::string("TG=" + MKSTR(ActiveVFX->TG)).c_str());
			stream.WriteString(std::string("TB=" + MKSTR(ActiveVFX->TB)).c_str());

			stream.WriteString(std::string("TRvar=" + MKSTR(ActiveVFX->TRVar)).c_str());
			stream.WriteString(std::string("TGvar=" + MKSTR(ActiveVFX->TGVar)).c_str());
			stream.WriteString(std::string("TBvar=" + MKSTR(ActiveVFX->TBVar)).c_str());

			stream.WriteString( (ActiveVFX->HiDef)?"HiDef=true":"HiDef=false" );
			stream.WriteString(std::string("Drn=" + MKSTR(ActiveVFX->Duration)).c_str());
			stream.WriteString(std::string("Drag=" + MKSTR(ActiveVFX->Drag)).c_str());
			stream.WriteString(std::string("srcAngle=" + MKSTR(ActiveVFX->EmitSrcAngle)).c_str());

			stream.Close();
		}
	}

	//Also Copy Code to Clipboard	
	{
		codeStringData = declareCodeData + codeInitData + deleteCodeData;

		OpenClipboard(0);
		EmptyClipboard();	
		HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,codeStringData.size());
		if (!hg){
			CloseClipboard();
			return;
		}
		memcpy(GlobalLock(hg),codeStringData.c_str(),codeStringData.size());
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT,hg);
		CloseClipboard();
		GlobalFree(hg);
	}
	ConsoleText->SetText("Files Saved");

#else
	ConsoleText->SetText("This functionality is only availabe in Windows system");
#endif
}

ParticleEditorControl::~ParticleEditorControl()
{	
	//Application::SetSingleUpdate(false);	
	K_LOG("ParticleEditorControl Destruct");
	delete DebugGraphic;

	delete MessageElement;
	delete MessageText;

	K_LOG("_particlesArray Destruct");
	int size = (int)_particlesArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		delete _particlesArray[i];
		_particlesArray[i] = NULL;

		delete _outlineRects[i];
		_outlineRects[i] = NULL;

		delete _particleNameTextArray[i];
		_particleNameTextArray[i] = NULL;
	}

	K_LOG("NUMLEFTCOLUMNELEMENTS Destruct");
	for (int i = 0; i < NUMLEFTCOLUMNELEMENTS; i++)
	{
		delete LeftColumnRects[i];
		delete LeftColumnKnobs[i];
		delete LeftColumnValTexts[i];
	}

	for (int i = 0; i < NUMCENTERCOLUMNELEMENTS; i++)
	{
		delete CenterColumnRects[i];
		delete CenterColumnKnobs[i];
		delete CenterColumnValTexts[i];
	}

	for (int i = 0; i < NUMRIGHTCOLUMNELEMENTS; i++)
	{
		delete RightColumnRects[i];
		delete RightColumnKnobs[i];
		delete RightColumnValTexts[i];
	}

	delete MainScene;

	//SAFE_DELETE(ActiveVFX);

	delete ConsoleText;
	delete PositionText;	

	CTexFactory::Instance->FreeImage(&StartColorImg);
	CTexFactory::Instance->FreeImage(&EndColorImg);
	StartColorImg = NULL;
	EndColorImg = NULL;

	//Application::SetSingleUpdate(false);
}
