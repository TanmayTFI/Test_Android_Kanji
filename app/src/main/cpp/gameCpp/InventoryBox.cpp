//====================================
// Framework
// InvBox Control
//====================================

#include "InventoryBox.h"
#include "Object.h"
#include "Point.h"
#include "Resources.h"
#include "ProfileSystem.h"
#include "BitmapText.h"
#include "Fonts.h"
#include "GameFlow.h"
#include "FoundInvPopup.h"
#include "Hud.h"
#include "AwardManager.h"
#include "Application.h"

extern HudControl* GFHud;
#define INVBOX_TILE_GAP 45


#define IBSLDANIM_MOVEX_PERFRAME   4
#define IBSLDANIM_NUMFRAMES		  20

#define INVITEM_DRAG_SCALE 1.0f
#define INVITEM_IDLE_SCALE 0.75f
#define INVITEM_HOVER_SCALE 0.95f

const CPoint InvBoxUpPos(683.0f, 768.0f, 0.0f);
const CPoint InvBoxDownPos(683.0f, 846.0f+25, 0.0f);

const CRectangle InvBoxRect(339,630,680,108+30);
const CRectangle IBLeftArrRect(355,695,41,35);
const CRectangle IBRightArrRect(973,695,40,35);

const CRectangle InvBoxLockRect(551,635,38,38);
const CRectangle InvBoxActivateRect(339,715,679,53);
const CPoint CasePos(683.0f, 324.0f, 0.0f);

const float InvBoxItemStartX = 385.0f;
const float InvBoxItem_Gap = 10.0f;
const float InvBoxItemStartY = 668.0f;
const float InvBoxItemTxtOffsetX = 24.0f;
const float InvBoxItemTxtOffsetY = -24.0f-19;

#ifdef TOUCH_UI
// the Cursor Icon should appear little above the place where the user places their finger, only then the cursor is visible.
#define CURSOR_TOP_DISPLACEMENT 20.0f
#define CURSOR_LOOK_TOP_DISPLACEMENT 30.0f
#define CURSOR_HANDPOINTY_TOP_DISPLACEMENT 30.0f
#define CURSOR_GEAR_TOP_DISPLACEMENT 30.0f
#define CURSOR_HELP_TOP_DISPLACEMENT 40.0f
#define CURSOR_SPEECH_TOP_DISPLACEMENT 40.0f
#define CURSOR_HANDTAKE_TOP_DISPLACEMENT 30.0f
#else
#define CURSOR_TOP_DISPLACEMENT 0.0f
	#define CURSOR_LOOK_TOP_DISPLACEMENT 0.0f
	#define CURSOR_HANDPOINTY_TOP_DISPLACEMENT 0.0f
	#define CURSOR_GEAR_TOP_DISPLACEMENT 0.0f
	#define CURSOR_HELP_TOP_DISPLACEMENT 0.0f
	#define CURSOR_SPEECH_TOP_DISPLACEMENT 0.0f
	#define CURSOR_HANDTAKE_TOP_DISPLACEMENT 0.0f
#endif

InventoryBox::InventoryBox()
{	
	State = InvBox_Inactive;
	DragIndex = -1;


	InvBoxLeftAnimatingTicks = 0;
	InvBoxRightAnimatingTicks = 0;

	IsLocked = ProfileSystem::Instance->ActiveProfile.IsInvLocked;
	IsDragging = false;
	dragTicks = 0;
	IsHoverItem = false;
	IsPuzzleModeActive = false;
	SlidingState = Sliding_Nil;
	NoAutoClose = false;
	ItemFlyStatus = Inv_StandStill;

	isInvBoxVisible = true;

	_timer = 0.0f;

	MouseOverItem = -1;
	HoverItemId = -1;

	CPoint textPos;
	//avoid dyanmic load &delete!!
	for(int i=0; i < HELP_TEXT_LINES; ++i)
	{
		textPos.x = 683.0f;
		textPos.y = 696.0f + 30 * i;
		HelpTexts[i] = NULL;
		HelpTexts[i] = new CBitmapText();
		HelpTexts[i]->SetZ(eZOrder_INVBOXBG+10);			
		HelpTexts[i]->SetPos(&textPos);		
		HelpTexts[i]->LoadFont(eFont_26);
		HelpTexts[i]->SetAlignment(Align_Center);
		HelpTexts[i]->SetText("");			
		HelpTexts[i]->SetScale(1.0f);
		HelpTexts[i]->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 		
		HelpTexts[i]->Show(false);
	}

	for(int i=0; i < MAX_INVBOX_OBJECTS; ++i)
	{
		InvBoxObj[i].HOObj = NULL;
		InvBoxObj[i].Text = NULL;
		InvBoxObj[i].nPiecePicked = 0;
		InvBoxObj[i].MaxPieces = 0;
		//rectangle not initialized here!!
	}

	MouseHoverTime = 0.0f;	
	ToolTip  = GFInstance->LoadBufferObject(INVBOX_TOOLTXT_OBJ);
	ToolTip->SetZ(eZOrder_INVBOXITEMS+10);
	ToolTip->Show(false);

	ToolText = new CBitmapText();	
	ToolText->SetZ(eZOrder_INVBOXITEMS+12);
	ToolText->SetAlignment(Align_Center);
	ToolText->LoadFont(eFont_25);
#ifdef JAPANESE_BUILD
	ToolText->SetScale(0.94f);//avoid overlapping text
#else
	ToolText->SetScale(0.8f);
#endif
	ToolText->SetText(StringTable::getStr("Extool"));

	ToolText->SetColor(239.0f/255.0f,228.0f/255.0f,176.0f/255.0f);
	ToolText->Show(false);

	NavigateText = new CBitmapText();	
	NavigateText->SetZ(eZOrder_INVBOXITEMS+16);
	NavigateText->SetAlignment(Align_Center);
	NavigateText->LoadFont(eFont_25);
	NavigateText->SetScale(0.85f);
	NavigateText->SetColor((233/255.0f),(204/255.0f),(131/255.0f));//gold
	NavigateText->SetText("");
	NavigateText->SetXY(683, 600);
	NavigateText->Show(true);
	
	navTapped = false;

	Navigate = new CObject();
	Navigate->LoadSprite("UI\\HUD\\RDESC.SPR");	
	Navigate->SetZ(eZOrder_INVBOXITEMS+15);
	Navigate->SetXY(683, 600);
	Navigate->Show(false);

	NavArrow = new CObject();
	NavArrow->LoadSprite("UI\\CUR\\CUR_NAV.SPR"); //("UI\\FOOT.SPR");
	NavArrow->SetZ(eZOrder_INVBOXITEMS+15);
	NavArrow->SetXY(683, 600);
	NavArrow->Show(false);


	InvBox = GFInstance->LoadBufferObject(INVBOX_BASE_OBJ);
	IBLock = GFInstance->LoadBufferObject(INVBOX_LOCK_OBJ);
	IBLock->SetPos(&InvBoxUpPos);	
	IBLArr = GFInstance->LoadBufferObject(INVBOX_LARR_OBJ);
	IBLArr->SetPos(&InvBoxUpPos);	
	IBRArr = GFInstance->LoadBufferObject(INVBOX_RARR_OBJ);
	IBRArr->SetPos(&InvBoxUpPos);

	activeComboItemIndex = -1;

	if( ProfileSystem::Instance->ActiveProfile.IsInvLocked )
	{
		IBSetPos(&InvBoxUpPos); 
		CreateInvBoxItems();
		State = InvBox_Idle;
	}
	else
	{
		ShowInvObj(false); 
		IBSetPos(&InvBoxDownPos);  
	}
	InitArrowsLock();
/*
#ifdef TOUCH_UI
	InitTouchX = KInput::getMouseX();
	InitTouchY = KInput::getMouseY();
#endif
 */
}

void InventoryBox::ShowInvBox(bool fading)
{
	if(fading)
	{
		FadeInInvObj(8.0);
		//ShowInvObj(true);

		InvBox->FadeIn(8.0); 
		IBLock->FadeIn(8.0);
		IBLArr->FadeIn(8.0);
		IBRArr->FadeIn(8.0);
	}
	else
	{
		ShowInvObj(true);

		InvBox->Show(true);
		IBLock->Show(true);
		IBLArr->Show(true);
		IBRArr->Show(true);
	}

	isInvBoxVisible = true;
}

void InventoryBox::HideInvBox(bool fading)
{
	isInvBoxVisible = false;

	if(fading)
	{
		//FadeOutInvObj(8.0);
		ShowInvObj(false);

		InvBox->FadeOut(8.0);
		IBLock->FadeOut(8.0);
		IBLArr->FadeOut(8.0);
		IBRArr->FadeOut(8.0);
	}
	else
	{
		ShowInvObj(false);

		InvBox->Show(false);
		IBLock->Show(false);
		IBLArr->Show(false);
		IBRArr->Show(false);
	}
}



void InventoryBox::InitInvBoxUpIdle()
{
	IBSetPos(&InvBoxUpPos); 
	CreateInvBoxItems();
	//ShowInvObj(true);
	State = InvBox_Idle;			
}

void InventoryBox::HidePuzzleTexts( )
{	
	for(int i=0; i < HELP_TEXT_LINES; ++i)
		if( HelpTexts[i] != NULL )
			HelpTexts[i]->Show(false);
}

void InventoryBox::InitArrowsLock()
{
	if( InvBox->Pos.y == InvBoxUpPos.y )
	{			
		IBLArr->ForcePlayAnimation(2);//turn to fade state at init!!
		IBRArr->ForcePlayAnimation(2);//turn to fade state at init!!

		if( ProfileSystem::Instance->ActiveProfile.IsInvLocked )//for others will be turned on later!!	
		{
			IBLock->ForcePlayAnimation(0);	
			IBLock->Show(true);
		}
		else
		{
			IBLock->ForcePlayAnimation(1);
			IBLock->Show(true);
		}


		if( ProfileSystem::Instance->ActiveProfile.InvObjectCnt > MAX_INVBOX_DISPLAYED_OBJECTS)
		{
			if( ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj > 0)
				IBLArr->ForcePlayAnimation(1);//turn off - left shift possible!!
			if( ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj < (ProfileSystem::Instance->ActiveProfile.InvObjectCnt - MAX_INVBOX_DISPLAYED_OBJECTS))
				IBRArr->ForcePlayAnimation(1);//turn off
		}
		IBLArr->Show(true);
		IBRArr->Show(true);
	}
	else
		TurnOffArrowsLock();
}

void InventoryBox::TurnOffArrowsLock()
{
	IBLArr->ForcePlayAnimation(3);//turn off
	IBRArr->ForcePlayAnimation(3);//turn off
	IBLock->ForcePlayAnimation(1);//turn off
}

void InventoryBox::ResetToInvBox( )
{	
	AwardManager::GetInstance()->setIsHO(false);
	

	InvBox->PlayAnimation(INVBOX_LR_FRAME);
	InvBoxFrame = INVBOX_LR_FRAME;	

	////////////////////////
	if( ProfileSystem::Instance->ActiveProfile.IsInvLocked )
	{
		IBSetPos(&InvBoxUpPos); 
		CreateInvBoxItems();
		State = InvBox_Idle;
	}
	else
	{
		ShowInvObj(false); 
		IBSetPos(&InvBoxDownPos);  
		State = InvBox_Inactive;
	}
	InitArrowsLock();
}

void InventoryBox::SetLockBase( int Frame )
{	
	if( Frame == PUZBOX_FRAME )
	{
		for(int i=0; i < HELP_TEXT_LINES; ++i)
			if( HelpTexts[i] != NULL )
				HelpTexts[i]->Show(true);//unhide!!

		switch ( ProfileSystem::Instance->ActiveProfile.CurrentArea )
		{
		default:  break;
		case eArea_L1_S1_PZKidsPainting: SetMultilineText(PZHelpTextArray[ePZHelp_L1_S1_KidsPainting],50); break;
		case eArea_L1_S2_PZFuse: SetMultilineText(PZHelpTextArray[ePZHelp_L1_S2_Fuse],50); break;
		case eArea_L1_S3_PZShelf: SetMultilineText(PZHelpTextArray[ePZHelp_L2_S3_ShelfPuzzle],50); break;
		case eArea_L1_S4_PZDoor:
			if(ProfileSystem::Instance->ActiveProfile.L1S4_P2IsShadowPuzHandDoorOpened)
				SetMultilineText(PZHelpTextArray[ePZHelp_L2_S3_ShadowPuzzleDoorOpened],50);
			else
				SetMultilineText(PZHelpTextArray[ePZHelp_L2_S3_ShadowPuzzle],50);

			break;

		case eArea_L1_S4_PZPainting:
			HelpTexts[0]->SetAlignment(Align_Center);
			HelpTexts[0]->SetText(std::string(StringTable::getStr(PZHelpTextArray[ePZHelp_L1_S4_PaintingPuzzle]) + "").c_str());

			HelpTexts[1]->SetText(" ");
			HelpTexts[2]->SetText(" ");
			//SetMultilineText(PZHelpTextArray[ePZHelp_L1_S4_PaintingPuzzle],50); - //TODO turned off another mutliline on same file as creates crash on release build occassionally!!
			break;
		case eArea_L1_S5_PZDynamiteMaking: SetMultilineText(PZHelpTextArray[ePZHelp_L1_S5_DynamitePuzzle],50); break;

			//Level 2
		case eArea_L2_S2_PZMango:			SetMultilineText(PZHelpTextArray[ePZHelp_L2_S2_MangoPuzzle],50);		break;
		case eArea_L2_S3_PZStoneBeam:		SetMultilineText(PZHelpTextArray[ePZHelp_L2_S3_StoneBeamPuzzle],50);	break;
		case eArea_L2_S4_PZDeities:			SetMultilineText(PZHelpTextArray[ePZHelp_L2_S4_DietiesPuzzle], 50);		break;
		case eArea_L2_S5_PZEagleUnblock:	SetMultilineText(PZHelpTextArray[ePZHelp_L2_S5_UnblockPuzzle],50, Align_Center, -50);		break;
		case eArea_L2_S5_PZMedicine_Making: SetMultilineText(PZHelpTextArray[ePZHelp_L2_S5_MedicineMaking],50);		break;
		case eArea_L2_S6_PZTunnelClock:		SetMultilineText(PZHelpTextArray[ePZHelp_L2_S6_TunnelPuzzle],50);		break;
		case eArea_L2_S7_PZSoldiers:	
			{
				std::string helpText;
				int currentPuzzleNumber = ProfileSystem::Instance->ActiveProfile.L2_S7_CurrentPuzzle;
				switch (currentPuzzleNumber)
				{

				case 2:
					helpText = "L2puzzle";
						SetMultilineText(helpText,50);
					break;

				case 3:
					helpText = "L2sequence";
						SetMultilineText(helpText,50, Align_Center,-50);
					break;

				case 4:
					helpText = "L2wrong";
						SetMultilineText(helpText,50, Align_Center,-50);
					break;

				case 5:
					helpText = "L2match";
						SetMultilineText(helpText,50, Align_Center,-50);
					break;

				case 6:
					helpText = "L2dial";
						SetMultilineText(helpText,50, Align_Center,-50);
					break;

				default:
					break;
				}
			}
			break;
			//Level 3
		case eArea_L3_S3_PZUnderwater:		SetMultilineText(PZHelpTextArray[ePZHelp_L3_S3_UnderwaterPuzzle],50); break;
		case eArea_L3_S1_PZLionHead:  SetMultilineText(PZHelpTextArray[ePZHelp_L3_S1_LionHeadPuzzle],50); break;
		case eArea_L3_S3_PZBlocks:  SetMultilineText(PZHelpTextArray[ePZHelp_L3_S3_BlockPuzzle],50); break;
		case eArea_L3_S2_P1Room2Wine:SetMultilineText(PZHelpTextArray[ePZHelp_L3_S2_CreeperPuzzle],50); break;
		case eArea_L3_S2_P1Room4Match3:SetMultilineText(PZHelpTextArray[ePZHelp_L3_S2_Match3Puzzle],50); break;
		case eArea_L3_S2_P1Room5Pipe:SetMultilineText(PZHelpTextArray[ePZHelp_L3_S2_PipePuzzle],50); break;
		case eArea_L3_S2_FortInterior:SetMultilineText(PZHelpTextArray[ePZHelp_L3_S2_ThreadPuzzle],50); break;
			//Level 4
		case eArea_L4_S2_PZAlligatorHead:SetMultilineText(PZHelpTextArray[ePZHelp_L4_S2_AlligatorPuzzle], 50); break;
		case eArea_L4_S2_PZDoor:SetMultilineText(PZHelpTextArray[ePZHelp_L4_S2_FanPuzzle], 50); break;
		case eArea_L4_S3_PZDoor:SetMultilineText(PZHelpTextArray[ePZHelp_L4_S3_DoorPuzzle],50); break;
		case eArea_L4_S5_PZSwapTile:SetMultilineText(PZHelpTextArray[ePZHelp_L4_S5_SwapPuzzle], 50); break;

		case eArea_L5_S2_PZBlocks:	SetMultilineText(PZHelpTextArray[ePZHelp_L5_S2_BlocksPuzzle],50); break;
		case eArea_L5_S5_PZSunClock:	SetMultilineText(PZHelpTextArray[ePZHelp_L5_S5_MoonPuzzle],50); break;
		case eArea_L5_S4_Dungeon:	SetMultilineText(PZHelpTextArray[ePZHelp_L5_S4_Bridge],50); break;
		case eArea_L5_S4_PZHandCuff:
			if(!ProfileSystem::Instance->ActiveProfile.L5_S4_Level1Completed)
				SetMultilineText(PZHelpTextArray[ePZHelp_L5_S4_HandCuffPuzzle1],50); 
			else
				SetMultilineText(PZHelpTextArray[ePZHelp_L5_S4_HandCuffPuzzle2],50); 
			break;

			//Level 6
		case eArea_L6_S1_FloatingIsland1: SetMultilineText(PZHelpTextArray[ePZHelp_L6_S1_StatueChestPuzzle], 50); break;	
		case eArea_L6_S2_PZPineGum:SetMultilineText(PZHelpTextArray[ePZHelp_L6_S2_PineGluePuzzle], 50); break;		
		case eArea_L6_S3_PZPathway:SetMultilineText(PZHelpTextArray[ePZHelp_L6_S3_PathwayPuzzle], 50); break;	
		case eArea_L6_S3_GlowingHeartHO : SetMultilineText(PZHelpTextArray[ePZHelp_L6_S3_HO_DialPadPuzzle], 50); break;
		case eArea_L6_S4_PortalPuzzle: SetMultilineText(PZHelpTextArray[ePZHelp_L6_S4_PortalPuzzle], 50); break;
		case eArea_L6_S5_TabMiniGame : SetMultilineText(PZHelpTextArray[ePZHelp_L6_S5_TabPuzzle], 50); break;

			//Level 7
		case eArea_L7_S1_PZJigsawYellow:	SetMultilineText(PZHelpTextArray[ePZHelp_L7_S1_JigsawPuzzle],50); break;
		case eArea_L7_S1_Volcano:	SetMultilineText(PZHelpTextArray[ePZHelp_L7_S1_WellMemoryPuzzle],50); break;
		case eArea_L7_S2_PZEyePuzzle:	SetMultilineText(PZHelpTextArray[ePZHelp_L7_S2_EyePuzzle],50); break;
		case eArea_L7_S2_HOBrokenLeftHand : SetMultilineText(PZHelpTextArray[ePZHelp_L7_S2_LeftHandHOPZ],50); break;
		case eArea_L7_S2_HOBrokenRightHand : SetMultilineText(PZHelpTextArray[ePZHelp_L7_S2_RightHandHOPZ],50); break;
		case eArea_L7_S3_PZJigsawRed:	SetMultilineText(PZHelpTextArray[ePZHelp_L7_S3_JigsawPuzzle],50); break;

		case eArea_L7_S3_PZDoorPuzzle:	SetMultilineText(PZHelpTextArray[ePZHelp_L7_S3_DoorPuzzle],50); break;
		case eArea_L7_S4_Ameeha:
			{
				if (ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopPoojaPuzzleMode == 0)
				{
					SetMultilineText(PZHelpTextArray[ePZHelp_L7_S4_BhasmamPuzzle1],50, Align_Center,-50);
				}
				else if(ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopPoojaPuzzleMode == 2)
				{
					SetMultilineText(PZHelpTextArray[ePZHelp_L7_S4_BhasmamPuzzle2],50, Align_Center,-50);
				}
			}
			break;

		case eArea_L7_S4_PZJigsawBlue:	SetMultilineText(PZHelpTextArray[ePZHelp_L7_S4_JigsawPuzzle],50); break;		


			//level 8
		case eArea_L8_S1_PZDoorHandPuzzle:	SetMultilineText(PZHelpTextArray[ePZHelp_L8_S1_HandDoorPuzzle],50); break;	
		case eArea_L8_S2_PZGate:			SetMultilineText(PZHelpTextArray[ePZHelp_L8_S2_GatePuzzle],50); break;	
		case eArea_L8_S2_PZGears:			SetMultilineText(PZHelpTextArray[ePZHelp_L8_S2_GearPuzzle],50); break;	
		case eArea_L8_S4_PZPoolGame:		SetMultilineText(PZHelpTextArray[ePZHelp_L8_S4_PoolPuzzle],50); break;		
		case eArea_L8_S5_VirtualMiniGame:	SetMultilineText(PZHelpTextArray[ePZHelp_L8_S5_VirtualPuzzle],50); break;		
		}	
	}



	InvBox->ForceShow();//handle fade out case!!
	//Inventory panel is now free to act!!
	InvBox->SetPos(&InvBoxUpPos);
	InvBox->PlayAnimation(Frame);//TODO: SHOULD STOP THE DRAG & DROP 
	InvBox->Status = SPR_VISIBLE;


	ShowInvObj(false);

	switch( Frame )
	{
	case HOBOX_FRAME:
		AwardManager::GetInstance()->setIsHO(true);
		TurnOffArrowsLock();
		if(ProfileSystem::Instance->ActiveProfile.CurrentArea == eArea_L4_S1_ShoeHO)
		{
			for(int i=0; i < HELP_TEXT_LINES; ++i)
				if( HelpTexts[i] != NULL )
					HelpTexts[i]->Show(true);//unhide!!

			std::string helpText;
			int currentHOItem = 0;

			for (int i = 0; i < 12; i++)
			{
				if (ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeFound[i])
				{
					currentHOItem = i + 1;
				}
			}

			switch (currentHOItem)
			{
			case 0:
				helpText = "L4scales";
				break;

			case 1:
				helpText = "L4fits";
				break;

			case 2:
				helpText = "L4found";
				break;

			case 3:
				helpText = "L4bunny";
				break;

			case 4:
				helpText = "L4rainbow";
				break;

			case 5:
				helpText = "L4seen";
				break;

			case 6:
				helpText = "L4tail";
				break;

			case 7:
				helpText = "L4step";
				break;

			case 8:
				helpText = "L4blade";
				break;

			case 9:
				helpText = "L4skin";
				break;

			case 10:
				helpText = "L4fabric";
				break;

			case 11:
				helpText = "L4pair";
				break;

			default:
				helpText = "";
				break;
			}
			SetMultilineText(helpText,50, Align_Center,-50);
		}
		break;

	case HO_USEOBJECTS_FRAME:
	case HO_PLACEOBJECTS_FRAME:
	case HO_FINDPAIRS_FRAME:
		AwardManager::GetInstance()->setIsHO(true);
	case MATCH3_FRAME:
	case PUZBOX_FRAME:  
		IsLocked = true; 
		TurnOffArrowsLock();

		if( State == InvBox_TransIn )
			State = InvBox_Idle; //To Avoid inv box items overlapping with puzzle frame sometimes
		break;

	case INVBOX_LR_FRAME: IsLocked = ProfileSystem::Instance->ActiveProfile.IsInvLocked; 
		InitArrowsLock();

		break;
	default: IsLocked = false; break;
	}
	InvBoxFrame = Frame;				
	InvBox->Visible = true;

}


void InventoryBox::Required()
{
	if(!isInvBoxVisible)
		return;

	//Handle specific cases
	if( InvBoxFrame == INVBOX_LR_FRAME )
		if( ProfileSystem::Instance->ActiveProfile.IsInvLocked )
			if( IBLArr->GetAnimation() == 3 && IBRArr->GetAnimation() == 3 )// - turned off!!
				InitArrowsLock();

	if ( InvBoxLeftAnimatingTicks > 0 )
	{
		InvBoxLeftAnimatingTicks--;
		CPoint tempPos;
		for( int i = 0; i < ProfileSystem::Instance->ActiveProfile.InvObjectCnt; i++ )
			if( InvBoxObj[i].HOObj != NULL )
			{
				int DisplayIndex = i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj;				
				tempPos.x = InvBox->Pos.x + InvBoxItemXOff + (InvBoxItemSpaceX * DisplayIndex) + InvBoxLeftAnimatingTicks*IBSLDANIM_MOVEX_PERFRAME;
				tempPos.y = InvBox->Pos.y + InvBoxItemYOff;
				InvBoxObj[i].HOObj->SetPos(&tempPos);
				if( InvBoxObj[i].Text != NULL )
				{
					tempPos.x += InvBoxItemTxtOffsetX;
					tempPos.y += InvBoxItemTxtOffsetY;
					InvBoxObj[i].Text->SetPos(&tempPos);						
				}
			}
			return;
	}
	else if( InvBoxRightAnimatingTicks > 0 )
	{
		InvBoxRightAnimatingTicks--; 
		CPoint tempPos;
		for( int i = 0; i < ProfileSystem::Instance->ActiveProfile.InvObjectCnt; i++ )
			if( InvBoxObj[i].HOObj != NULL )
			{
				int DisplayIndex = i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj;				
				tempPos.x = InvBox->Pos.x + InvBoxItemXOff + (InvBoxItemSpaceX * DisplayIndex) - InvBoxRightAnimatingTicks*IBSLDANIM_MOVEX_PERFRAME;
				tempPos.y = InvBox->Pos.y + InvBoxItemYOff;
				InvBoxObj[i].HOObj->SetPos(&tempPos);
				if( InvBoxObj[i].Text != NULL )
				{
					tempPos.x += InvBoxItemTxtOffsetX;
					tempPos.y += InvBoxItemTxtOffsetY;
					InvBoxObj[i].Text->SetPos(&tempPos);						
				}
			}
			return;
	}
	if( State == InvBox_TransIn )
		UpdateTransIn();
	else if( State == InvBox_TransOut )
		UpdateTransOut();	
}


void InventoryBox::ShowInvObj(bool f)
{
	for(int i=0; i < MAX_INVBOX_OBJECTS; ++i)
	{
		if( InvBoxObj[i].HOObj != NULL )		
			InvBoxObj[i].HOObj->Show(f);			
		if( InvBoxObj[i].Text != NULL )
			InvBoxObj[i].Text->Show(f);
	}
}

void InventoryBox::FadeOutInvObj(float speed)
{
	for(int i=0; i < MAX_INVBOX_OBJECTS; ++i)
	{
		if( InvBoxObj[i].HOObj != NULL )		
			InvBoxObj[i].HOObj->FadeOut(speed);			
		if( InvBoxObj[i].Text != NULL )
			InvBoxObj[i].Text->FadeOut(speed);
	}
}

void InventoryBox::FadeInInvObj(float speed)
{
	for(int i=0; i < MAX_INVBOX_OBJECTS; ++i)
	{
		if( InvBoxObj[i].HOObj != NULL )		
			InvBoxObj[i].HOObj->FadeIn(speed);			
		if( InvBoxObj[i].Text != NULL )
			InvBoxObj[i].Text->FadeIn(speed);
	}
}

void InventoryBox::CancelDrag()
{
	if(DragIndex != -1)
	{		
		if( InvBox->Pos.y < InvBoxDownPos.y && SlidingState != Sliding_Down )//Open/Opening state!!
		{
			float x = InvBoxObj[DragIndex].HOObj->Pos.x-Origin.x;
			float y = InvBoxObj[DragIndex].HOObj->Pos.y-Origin.y;
			float Gap = sqrt((x*x)+(y*y));

			//we have hit invbox
			if( InvBox->Pos.y == InvBoxDownPos.y && InvBoxActivateRect.Intersection(&InvBoxObj[DragIndex].HOObj->Pos) )				
			{
				EndDrag();			
				ItemFlyStatus = Inv_StandStill;


				//turn off since drag over				
				Close();
				//Control::Audio->PlaySample(aSFXClick);
				Deactivate();				
			}
			//LOCKED!!
			else if( InvBox->Pos.y < InvBoxDownPos.y && Gap <= INVBOX_TILE_GAP && IsOpen() )
			{
				EndDrag();
				ItemFlyStatus = Inv_StandStill;
			}
			else
			{
				ItemFlyStatus = Inv_Flying;
				InvBoxObj[DragIndex].HOObj->SetZ(eZOrder_INVBOXITEMS+10);

				CPoint offsetOrigin = Origin; //If Inv Taken and dropped in scene when Inv Box is down. Move Inv to below the screen.
				if(!ProfileSystem::Instance->ActiveProfile.IsInvLocked && SlidingState==Sliding_Nil)
					offsetOrigin.y += 60;
				InvBoxObj[DragIndex].HOObj->Remove(&offsetOrigin,1.0f,true,MAX_INVBOXOBJ_SIZE,false,false);	//always move to original position!!	
			}
		}
		else
		{
			float x = InvBoxObj[DragIndex].HOObj->Pos.x-Origin.x;
			float y = InvBoxObj[DragIndex].HOObj->Pos.y-Origin.y;
			float Gap = sqrt((x*x)+(y*y));
			//we have hit invbox
			if( InvBox->Pos.y == InvBoxDownPos.y && InvBoxActivateRect.Intersection(&InvBoxObj[DragIndex].HOObj->Pos) )				
			{
				EndDrag();			
				ItemFlyStatus = Inv_StandStill;

				//turn off since drag over					
				Close();
				//Control::Audio->PlaySample(aSFXClick);
				Deactivate();				
			}
			//LOCKED
			else if( InvBox->Pos.y < InvBoxDownPos.y && Gap <= INVBOX_TILE_GAP && IsOpen() ) 
			{
				EndDrag();
				ItemFlyStatus = Inv_StandStill;
			}			
			else
			{
				ItemFlyStatus = Inv_Flying;
				CPoint tempPos = InvBoxTargetHitPos;
				tempPos.x = Origin.x;

				CPoint offsetOrigin = tempPos; //If Inv Taken and dropped in scene when Inv Box is down. Move Inv to below the screen.
				if(!ProfileSystem::Instance->ActiveProfile.IsInvLocked && SlidingState==Sliding_Nil)
					offsetOrigin.y += 60;
				InvBoxObj[DragIndex].HOObj->SetZ(eZOrder_INVBOXITEMS+10);
				InvBoxObj[DragIndex].HOObj->Remove(&offsetOrigin,1.0f,false,MAX_INVBOXOBJ_SIZE,false,false);		
			}
		}
	}
}



void InventoryBox::EndDrag()
{
	ResetDragging();	
	InvBoxObj[DragIndex].HOObj->Status = eHO_Hidden;
	if( InvBoxObj[DragIndex].HOObj->PublicValue == eInv_L1S1_SwissKnife )
		InvBoxObj[DragIndex].HOObj->PlayAnimation(0);
	Cursor::Show(true);
	DragIndex = -1;
	IsDragging = false;		
	dragTicks = 0;
}


void InventoryBox::ResetDragging()
{
	if(DragIndex == -1)
	{
		#ifdef K_WIN32
			WARN(StringTable::getString("ERR_InvDrag"));////Removed Wide string conversion for Windows	
		#else
			WARN(StringTable::getString("ERR_InvDrag"));
		#endif
		return;
	}

	if( InvBoxObj[DragIndex].HOObj->ZOrder != eZOrder_INVBOXITEMS )
	{
		InvBoxObj[DragIndex].HOObj->SetZ(eZOrder_INVBOXITEMS);
		InvBoxObj[DragIndex].HOObj->SetPos(&Origin);//restore

		if( InvBoxObj[DragIndex].HOObj->PublicValue == eInv_L1S1_SwissKnife )
			InvBoxObj[DragIndex].HOObj->PlayAnimation(0);

		InvBoxObj[DragIndex].HOObj->SetScale(INVITEM_IDLE_SCALE);	
		InvBoxObj[DragIndex].HOObj->Effect->StopEmit();
		if( InvBoxObj[DragIndex].Text != NULL )
			InvBoxObj[DragIndex].Text->Show(true);
		if( ProfileSystem::Instance->ActiveProfile.Inventory[InvBoxObj[DragIndex].HOObj->PublicValue] )			
		{
			InvBoxObj[DragIndex].HOObj->ForceShow();//rescale to 1.0
			switch( SlidingState )
			{
			case Sliding_Up  ://always show as will reach fullexposedstate soon
				break;
			case Sliding_Nil :if( InvBox->Pos.y >= InvBoxDownPos.y )
							  {
								  InvBoxObj[DragIndex].HOObj->Show(false);//No need to show them in closed invboxpanel!!
								  if( InvBoxObj[DragIndex].Text != NULL )
									  InvBoxObj[DragIndex].Text->Show(false);
							  }
							  break;
			case Sliding_Down:if( InvBox->Pos.y > InvBoxUpPos.y )//If invbox is sliding "down" very soon 
							  {
								  InvBoxObj[DragIndex].HOObj->Show(false);
								  if( InvBoxObj[DragIndex].Text != NULL )
									  InvBoxObj[DragIndex].Text->Show(false);
							  }
							  break;
			}		
		}
		else
		{
			//Check for multiInventory Item. Since only the first ID is accessed, if player haven't collected the first of the Several Items, it will turn invisible. Hence.
			if(!IsMultiPiece_InvBoxObj(InvBoxObj[DragIndex].HOObj->PublicValue))
			{
				InvBoxObj[DragIndex].HOObj->Show(false);
				if( InvBoxObj[DragIndex].Text != NULL )
					InvBoxObj[DragIndex].Text->Show(false);
			}
		}
	}
}

void InventoryBox::ExternalClose()//closing invbox from HUD
{
	if( DragIndex != -1 )
	{		
		InvBoxObj[DragIndex].HOObj->Status = eHO_Hidden;
		if( InvBoxObj[DragIndex].HOObj->PublicValue == eInv_L1S1_SwissKnife )
			InvBoxObj[DragIndex].HOObj->PlayAnimation(0);
		DragIndex = -1;
		IsDragging = false;
		dragTicks = 0;
	}
	NoAutoClose = false;
	State = InvBox_Idle;
	Cursor::Show(true);

	if( SlidingState != Sliding_Nil )
	{
		//If Sliding up or down. Slide it down.
		CPoint pos = InvBox->Pos;
		pos.y = InvBoxDownPos.y;
		IBSetPos(&pos);
	}

	SlidingState = Sliding_Nil;
	if( ItemFlyStatus == Inv_StandStill && DragIndex == -1 )
		Deactivate();
}

void InventoryBox::Close()
{
	NoAutoClose = false;
	SlidingState = Sliding_Nil;
	if( IsLocked )
	{
		//forced close to avoid unwanted inbetween states!!
		if( !IsDragging )
			ExternalClose();
		else if( ItemFlyStatus == Inv_StandStill && DragIndex == -1 )
			Deactivate();
	}
	else
	{
		TurnOffArrowsLock();
		State = InvBox_TransOut;
	}
}


void InventoryBox::ReleaseDrag()
{
	if(DragIndex == -1)
	{
		PRINT((const char*)L"Bad Drag Index in InvBox Release Drag!");
		Cursor::Show(true);
		IsDragging = false;		
		dragTicks = 0;
		return;
	}

	InvBoxObj[DragIndex].HOObj->SetTint(1.0f,1.0f,1.0f);
	InvBoxObj[DragIndex].HOObj->SetScale(1.0f);

	InvBoxObj[DragIndex].HOObj->Show(false);	
	EndDrag();	/* Instant appear in invbox */	
}

void InventoryBox::GlowDragInvObject(int ID, CRectangle rect)
{
	if (DragIndex == -1)
		return;

	if (InvBoxObj)
	{
		if (InvBoxObj[DragIndex].HOObj)
		{
			if (InvBoxObj[DragIndex].HOObj->PublicValue != ID)
				return;
		}
		else
			return;
	}
	else
		return;

	CPoint _pos = Control::Input->Mpos();
	if(!rect.Intersection(&_pos))
	{
		InvBoxObj[DragIndex].HOObj->SetTint(1.0f, 1.0f, 1.0f);
		InvBoxObj[DragIndex].HOObj->SetScale(1.5f);
		return;
	}		
	
	InvBoxObj[DragIndex].HOObj->SetTint(1.95f, 1.45f, 0.28f);
	InvBoxObj[DragIndex].HOObj->SetScale(1.75f);
}


void InventoryBox::ShowDragInv( bool status)
{
	if( DragIndex >= 0 && DragIndex < MAX_INVBOX_OBJECTS )//DragIndex != -1
		if( InvBoxObj[DragIndex].HOObj != NULL )
			InvBoxObj[DragIndex].HOObj->Show(status);
}

bool InventoryBox::GetDropInfo(CPoint& hotspot, int& itemVal)
{
	if(DragIndex == -1)
		return false;

	hotspot = InvBoxObj[DragIndex].HOObj->Pos;
	itemVal = InvBoxObj[DragIndex].HOObj->PublicValue;	

	return true;
}

int InventoryBox::GetMouseOverItem()
{
	return MouseOverItem;
}

void InventoryBox::UpdateTransIn()
{
	CPoint pos = InvBox->Pos;
	pos.y -= 400.0f * Control::LogicRate;
	if( pos.y < InvBoxUpPos.y )
	{
		pos.y = InvBoxUpPos.y;
		State = InvBox_Idle;
		IBSetPos(&pos);
		InitArrowsLock();
	}
	else
		IBSetPos(&pos);
}

void InventoryBox::FadeIn( float fade )
{
	InvBox->FadeIn( fade );
	for(int i=0; i < MAX_INVBOX_OBJECTS; ++i)
		if( InvBoxObj[i].HOObj )
			InvBoxObj[i].HOObj->FadeIn( fade );
}

void InventoryBox::Deactivate()
{
	if( !IsLocked )
	{
		//Turn off all else!!
		for(int i=0; i < MAX_INVBOX_OBJECTS; ++i)
		{
			if( InvBoxObj[i].HOObj != NULL )
			{
				InvBoxObj[i].HOObj->Show(false);
				//IF BUFFERED, Never DELETE OR CREATE - JUST ASSIGN FROM BUFFER
				if( GFInstance->iBufferLvl_InvHOObjLoaded != ProfileSystem::Instance->ActiveProfile.CurrentLevel )
					delete InvBoxObj[i].HOObj;
				InvBoxObj[i].HOObj = NULL;
			}
			if( InvBoxObj[i].Text )
				InvBoxObj[i].Text->Show(false);
		}
	}
	SetModal(false);
	State = InvBox_Inactive;
	SendMessage(ActionProcessed, ID, INVBOX_CLOSE);	
}

void InventoryBox::UpdateTransOut()
{
	if( SlideDown() )	
	{
		if( ItemFlyStatus == Inv_StandStill && DragIndex == -1 )
			Deactivate();	
	}
}

bool InventoryBox::SlideUp()
{	
	CPoint pos = InvBox->Pos;
	pos.y -= 400.0f * Control::LogicRate;	
	if( pos.y <= InvBoxUpPos.y )
	{
		pos.y = InvBoxUpPos.y;
		IBSetPos(&pos);
		SlidingState = Sliding_Nil;
		InitArrowsLock();
		return true;
	}
	else
		IBSetPos(&pos);
	return false;	
}

bool InventoryBox::SlideDown()
{
	if( IsLocked )
	{
		SlidingState = Sliding_Nil;
		//forced close to avoid unwanted inbetween states!!
		if( !IsDragging )
			ExternalClose();
		return true;
	}
	else
	{
		CPoint pos = InvBox->Pos;
		pos.y += 400.0f * Control::LogicRate;
		if( pos.y >= InvBoxDownPos.y )
		{
			//out of focus - no need for inv_objects to be in memory
			pos.y = InvBoxDownPos.y;
			IBSetPos(&pos);

			SlidingState = Sliding_Nil;
			//forced close to avoid unwanted inbetween states!!
			if( !IsDragging )
				ExternalClose();
			return true;
		}
		IBSetPos(&pos);
		return false;
	}
}

void InventoryBox::HandleFlyDrop( int i )
{
	//if( InvBoxObj[i].HOObj->PublicValue == eInv_L1_GateKey )
	//	SendMessage(ActionProcessed, ID, INVBOX_CANCELDRAG);	
	//Control::Audio->PlaySample(aSFXInvItemPick);
	float x = InvBoxObj[i].HOObj->TargetPos.x-InvBoxObj[i].HOObj->Pos.x;
	float y = InvBoxObj[i].HOObj->TargetPos.y-InvBoxObj[i].HOObj->Pos.y;
	float Gap = sqrt((x*x)+(y*y));
	if( IsOpen() && Gap >= INVBOX_TILE_GAP )//More than a tile gap!!	
		CancelDrag(); /* Instant appear in invbox */			
	else		
		EndDrag(); /* Instant appear in invbox */						


	Control::Input->ClearMouse();	
}

void InventoryBox::HandleDirectDrop( int i )
{
	/*
	if( InvBoxObj[i].HOObj->PublicValue == eInv_L1_GateKey )
	SendMessage(ActionProcessed, ID, INVBOX_CANCELDRAG);	
	*/
	//Control::Audio->PlaySample(aSFXInvItemPick);
	EndDrag(); /* Instant appear in invbox */							

	Control::Input->ClearMouse();

	//If we have dropped it into closed invbox then no need of invbox further!!
	if( InvBox->Pos.y == InvBoxDownPos.y )
		ExternalClose();
}


InventoryBox::~InventoryBox()
{

	// Bug fix : When the item placed on scene and returns (wrong inv), and moved to next scene (if invbox is unlocked)
	// the flying item will be invisible (until user clicks and release the item).
	if(DragIndex != -1)
		ReleaseDrag();

	//if (DragIndex != -1)
	//{
	//	ResetDragging();					
	//	if( InvBoxObj[DragIndex].HOObj->Effect->IsParticlesActive()  )
	//	{
	//		//if( InvBox->Pos.y == InvBoxDownPos.y )//Hide it under along with other neighbours!! 
	//		{						
	//			CPoint tempPos = InvBox->Pos;
	//			tempPos.x += InvBoxItemXOff + InvBoxItemSpaceX * (DragIndex-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);//drag
	//			tempPos.y += InvBoxItemYOff;
	//			InvBoxObj[DragIndex].HOObj->SetPos(&tempPos);
	//			InvBoxObj[DragIndex].HOObj->ForceShow();

	//			if( InvBoxObj[DragIndex].Text != NULL )
	//			{
	//				tempPos.x += InvBoxItemTxtOffsetX; 
	//				tempPos.y += InvBoxItemTxtOffsetY;
	//				InvBoxObj[DragIndex].Text->SetPos(&tempPos);	
	//				InvBoxObj[DragIndex].Text->Show(true);

	//			}

	//			InvBoxObj[DragIndex].HOObj->Status = eHO_Hidden;
	//		}
	//		InvBoxObj[DragIndex].HOObj->Effect->DA = 2;//wait till fadeout quickly!!
	//		ItemFlyStatus = Inv_WaitForVFX;
	//	}
	//	else
	//	{						
	//		ItemFlyStatus = Inv_StopFlying;
	//	} 
	//}


	//Garbage collection in gameflow
	InvBox->Show(false);InvBox = NULL;	
	IBLArr->Show(false);IBLArr = NULL;
	IBRArr->Show(false);IBRArr = NULL;
	IBLock->Show(false);IBLock = NULL;

	ToolTip->Show(false);ToolTip = NULL;	

	//unbuffered objects killed!!
	for(int i=0; i < HELP_TEXT_LINES; i++)
		if( HelpTexts[i] != NULL )
		{
			delete HelpTexts[i];
			HelpTexts[i] = NULL;
		}


		//DELETE STRUCTURE
		for(int i=0; i < MAX_INVBOX_OBJECTS; ++i)
		{
			if( InvBoxObj[i].HOObj != NULL )
			{
				//IF BUFFERED, Never DELETE OR CREATE - JUST ASSIGN FROM BUFFER
				if( GFInstance->iBufferLvl_InvHOObjLoaded != ProfileSystem::Instance->ActiveProfile.CurrentLevel )
					delete InvBoxObj[i].HOObj;
				else 
				{
					//if coming area puzzle/ho we have to turn off  
					if( GFInstance->IsHOPuzzleScene(ProfileSystem::Instance->ActiveProfile.CurrentArea)  || 
						!ProfileSystem::Instance->ActiveProfile.IsInvLocked )//or if no invlock we will turn off!!
						InvBoxObj[i].HOObj->Show(false);
				}
				InvBoxObj[i].HOObj = NULL;
			}
			if( InvBoxObj[i].Text != NULL )
			{
				delete InvBoxObj[i].Text;
				InvBoxObj[i].Text = NULL;
			}
		}


		delete ToolText;
		delete Navigate;
		delete NavigateText;
		delete NavArrow;

		if( GFInstance != NULL )   
			if( !GFInstance->IsPartialCacheMem )
			{
				GFInstance->UnloadBufferObject(INVBOX_TOOLTXT_OBJ);
				GFInstance->UnloadBufferObject(INVBOX_BASE_OBJ);
				GFInstance->UnloadBufferObject(INVBOX_LARR_OBJ);
				GFInstance->UnloadBufferObject(INVBOX_RARR_OBJ);
				GFInstance->UnloadBufferObject(INVBOX_LOCK_OBJ);
			}
}

void InventoryBox::Activate( int nFlyingObjects )
{
    /*
#if ( defined  K_iOS) || ( defined  K_ANDROID)
	if( InitTouchX != -1 )
	{
		if( InitTouchX != KInput::getMouseX() || InitTouchY != KInput::getMouseY() )
		{
			//first touch sensed!!
			InitTouchX = -1;
			InitTouchY = -1;
		}
		else //wait till first invbox touch -> discard last scene exit touch!!
			return;
	}
#endif
*/
	if( !Cursor::Visible )
		return;

	if( IsDragging )
	{	
		if( SlidingState != Sliding_Up )
		{
			if( IsLocked )
				SlidingState = Sliding_Nil;
			else
			{
				if(ItemFlyStatus != Inv_Flying) //Do not slide up when inv item is flying. Cause inv item to disappear sometimes.
					SlidingState = Sliding_Up;	//Locked state don't need such sound
			}
		}
		return;
	}
	else if( IsOpen() )//State == InvBox_Idle
	{
		//Not fully exposed and newbees will fit in current exposure itself!!
		if( (ProfileSystem::Instance->ActiveProfile.InvObjectCnt+nFlyingObjects) < MAX_INVBOX_OBJECTS )
			return;
		else
		{			
			//TODO: - Sliding need to be handled very very soon!!!
				KMiscTools::messageBox((const char*)StringTable::getStr("ExErrSlidingTitle").c_str(),(const char*)StringTable::getStr("ExErrInvCountMore25").c_str() );			
			exit(0);
		}
	}
	else if( SlidingState == Sliding_Up || State == InvBox_TransIn )
		return;

	///////////////////////////////
	CreateInvBoxItems();					

	//ShowInvObj(true);
	if( IsLocked )
	{
		CPoint pos = InvBox->Pos;
		pos.y = InvBoxUpPos.y;
		State = InvBox_Idle;
		IBSetPos(&pos);
	}
	else
	{
		State = InvBox_TransIn;	
		IBSetPos(&InvBoxDownPos);
	}

	//SetModal(true);
	MouseHoverTime = 0.0f;
}

void InventoryBox::ClearItemFromInvBox( int InvId )
{
	//////////////////////////////////////////////////////////////////////////////
	//////FIND OBJECT IN  INVOBJECTS ARRAY - CLEAR - SHIFT LEFTWARDS TO FILL EMPTY
	//////////////////////////////////////////////////////////////////////////////
	int RemapInvId = GetRemapInvId( InvId );
	int iInvObj = 0;	
	for(;iInvObj < MAX_INVBOX_OBJECTS; iInvObj++ ) // First check if ibject exists in inv
	{
		if( ProfileSystem::Instance->ActiveProfile.InvObjects[iInvObj].InvId == RemapInvId )
			break;
	}	
	if (iInvObj == MAX_INVBOX_OBJECTS) return; // if it doesnt dont do anything, to avoid crashes ! :)

	if( ProfileSystem::Instance->ActiveProfile.InvObjects[iInvObj].InvId == RemapInvId )
		{			
			//CLEAR OBJ!!
			ProfileSystem::Instance->ActiveProfile.InvObjects[iInvObj].InvId = -1;
			ProfileSystem::Instance->ActiveProfile.InvObjects[iInvObj].nPiecePicked = 0;
			ProfileSystem::Instance->ActiveProfile.InvObjects[iInvObj].MaxPieces = 0;
		}

		//CLEAR OBJECT IN PROFILESYSTEM INVBOX ARRAY!!!
		//SLIDE LEFTWARDS ONCE!!
		int k = iInvObj;
		for(; k < (MAX_INVBOX_OBJECTS-1); k++ ) //LEFTWARDS COPY, SKIP LAST AS WILL BE INITED BELOW!! 
		{
			//K_LOG("Shifting zero value inside inv...");
			ProfileSystem::Instance->ActiveProfile.InvObjects[k].InvId = ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].InvId;
			ProfileSystem::Instance->ActiveProfile.InvObjects[k].nPiecePicked = ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].nPiecePicked;
			ProfileSystem::Instance->ActiveProfile.InvObjects[k].MaxPieces = ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].MaxPieces;

			if( ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].InvId == -1 )//NO MORE OBJECTS LEFT!!
			{
				ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].nPiecePicked = 0;
				ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].MaxPieces = 0;	
				break; //EMPTY WILL BE DONE BELOW!!!				
			}
		}
		//Clear last one, as nothing to copy from!!
		if( k == (MAX_INVBOX_OBJECTS-1))
		{
			ProfileSystem::Instance->ActiveProfile.InvObjects[k].InvId = -1;
			ProfileSystem::Instance->ActiveProfile.InvObjects[k].nPiecePicked = 0;
			ProfileSystem::Instance->ActiveProfile.InvObjects[k].MaxPieces = 0;				
		}

		//////////////////////////////////////////////////////////////////////////////
		//CLEAR OBJECT IN INVBOXOBJECTS ARRAY, SHIFT LEFTWARDS TO FILL EMPTY!!!!
		//////////////////////////////////////////////////////////////////////////////
		//turn off first few objects to be overwritten!!	
		if( InvBoxObj[iInvObj].HOObj != NULL )
		{
			InvBoxObj[iInvObj].HOObj->Show(false);
			if( InvBoxObj[iInvObj].Text != NULL )
				InvBoxObj[iInvObj].Text->Show(false);
		}

		int i = iInvObj;
		CPoint tempPos;			
		//Shift objects leftwards  - 0..2nd last (6-7 operations needed)
		for( ;i < (MAX_INVBOX_OBJECTS-1) ; i++ )
		{		
			InvBoxObj[i].nPiecePicked = InvBoxObj[i+1].nPiecePicked;
			InvBoxObj[i].MaxPieces = InvBoxObj[i+1].MaxPieces;
			InvBoxObj[i].HOObj = InvBoxObj[i+1].HOObj;
			InvBoxObj[i].Text  = InvBoxObj[i+1].Text;
			//rectangle is the same, just 4 other vars change = objects/text/npieces/maxpieces!!

			//OBJECTS JUST COPY
			if( InvBoxObj[i].HOObj != NULL )
			{
				int DisplayIndex = i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj;
				tempPos.x = InvBox->Pos.x + InvBoxItemXOff + (InvBoxItemSpaceX * DisplayIndex);//SHIFTINVBOX visible frustrum only!!!!!
				tempPos.y = InvBox->Pos.y + InvBoxItemYOff;
				InvBoxObj[i].HOObj->SetPos(&tempPos);//effectively 1..last set pos of 0...2nd last
				if(i >= ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj && i < (ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+MAX_INVBOX_DISPLAYED_OBJECTS))
					InvBoxObj[i].HOObj->Show(true);
				else
					InvBoxObj[i].HOObj->Show(false);
			}

			if( InvBoxObj[i].Text != NULL )// i <- (i+1) ~       from (0<-1)...(2nd last<-last) 
			{
				if( InvBoxObj[i].HOObj != NULL )
					if( !IsMultiPiece_InvBoxObj(InvBoxObj[i].HOObj->eInv_Id)) //if not multipiece then empty text mus be displayed!!
						InvBoxObj[i].Text->SetText("");
				tempPos.x += InvBoxItemTxtOffsetX;
				tempPos.y += InvBoxItemTxtOffsetY;
				InvBoxObj[i].Text->SetPos(&tempPos);	
				if(i >= ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj && i < (ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+MAX_INVBOX_DISPLAYED_OBJECTS))
				{
					InvBoxObj[i].Text->Show(true);
				}
				else
					InvBoxObj[i].Text->Show(false);
			}


			//NULLIFY
			if( i == (MAX_INVBOX_OBJECTS-2) || InvBoxObj[i+1].HOObj == NULL )
			{
				InvBoxObj[i+1].nPiecePicked = 0;
				InvBoxObj[i+1].MaxPieces = 0;

				if( InvBoxObj[i+1].HOObj != NULL )
				{
					InvBoxObj[i+1].HOObj->Show(false);
					InvBoxObj[i+1].HOObj = NULL;				
				}
				if( InvBoxObj[i+1].Text != NULL )
				{
					InvBoxObj[i+1].Text->Show(false);
					InvBoxObj[i+1].Text = NULL;
				}
				break;
			}
		}		

		ProfileSystem::Instance->ActiveProfile.InvObjectCnt--;
		if( ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj > 0  )						
			if( (ProfileSystem::Instance->ActiveProfile.InvObjectCnt-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) < MAX_INVBOX_DISPLAYED_OBJECTS )
				ShiftInvBoxRight(1);
}

void InventoryBox::CreateInvBoxItems()
{
	///INV OBJ CREATION!!
	/*char buffer[128];
	sprintf(buffer,"STARTING iInvBxStrtObj = %d InvCnt = %d",ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj, ProfileSystem::Instance->ActiveProfile.InvObjectCnt);
	K_LOG(buffer);*/

	/////////////////////////////////////////////
	/////// COPY INVBOX TO PROFILEBOX -> //////// (As we may have dropped some inv objects to huds inbetween )
	/////////////////////////////////////////////
	//We just need that order - will build up PiecesPicked from latest "inventory"!!!
	for(int i=0; i < MAX_INVBOX_OBJECTS; i++)
		ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked = 0;

	//READ INVENTORYBOX "LATEST" PIECE CONTENTS TO PROFILE_INVBOX!! (User may have dropped some objects to scene & hence!!)
	for(int i=0; i < eInv_Max; i++)
		if( ProfileSystem::Instance->ActiveProfile.Inventory[i]  )
		{
			int RemapInvId = GetRemapInvId( i );
			//Find multipiece slot & update!!
			for( int j = 0;j < MAX_INVBOX_OBJECTS; j++ )	
				if( ProfileSystem::Instance->ActiveProfile.InvObjects[j].InvId == RemapInvId )
				{
					ProfileSystem::Instance->ActiveProfile.InvObjects[j].nPiecePicked++;
					break;
				}
		}

		//REMOVE EMPTY SLOTS INBETWEEN DUE TO PLAYER DROPPINGS - SLIDE LEFTWARDS FOR A "SLOT DELETE"!!
		for( int i = 0;i < MAX_INVBOX_OBJECTS; i++ )	
		{
			if( ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId == -1 )//We have reached end!!
				break;
			else if( ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked == 0 )//empty inbetween slot found!!
			{
				////////////////////////
				//SLIDE LEFTWARDS ONCE!!
				////////////////////////
				int k = i;
				for(; k < (MAX_INVBOX_OBJECTS-1); k++ ) //LEFTWARDS COPY, SKIP LAST AS WILL BE INITED BELOW!! 
				{
					//K_LOG("Shifting zero value inside inv...");
					ProfileSystem::Instance->ActiveProfile.InvObjects[k].InvId = ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].InvId;
					ProfileSystem::Instance->ActiveProfile.InvObjects[k].nPiecePicked = ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].nPiecePicked;
					ProfileSystem::Instance->ActiveProfile.InvObjects[k].MaxPieces = ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].MaxPieces;				
					if( ProfileSystem::Instance->ActiveProfile.InvObjects[k+1].InvId == -1 )//NO MORE OBJECTS LEFT!!
						break; //EMPTY WILL BE DONE BELOW!!!				
				}
				//Clear last one!!
				if( k == (MAX_INVBOX_OBJECTS-1))
				{
					ProfileSystem::Instance->ActiveProfile.InvObjects[k].InvId = -1;
					ProfileSystem::Instance->ActiveProfile.InvObjects[k].nPiecePicked = 0;
					ProfileSystem::Instance->ActiveProfile.InvObjects[k].MaxPieces = 0;				
				}			
			}
		}

		int ItemsCnt = 0;//ITEMS CNT WE INCREMENT ONLY MULTIITEMS & NORMAL OBJECTS
		for( int i = 0;i < MAX_INVBOX_OBJECTS; i++ )//Count non-empty boxes	
			if( ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked >= 1 )  
				ItemsCnt++;

		ProfileSystem::Instance->ActiveProfile.InvObjectCnt = ItemsCnt;
		//ensure user inbetween invitem drops!!
		if( ProfileSystem::Instance->ActiveProfile.InvObjectCnt <= MAX_INVBOX_DISPLAYED_OBJECTS )		
			ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0; //Reinit to zero again, as less than 7 objects in scene!!
		else if((ProfileSystem::Instance->ActiveProfile.InvObjectCnt-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) < MAX_INVBOX_DISPLAYED_OBJECTS )//Always ensure we are seeing full list when itemcnt greater than 7
		{
			ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = ProfileSystem::Instance->ActiveProfile.InvObjectCnt-MAX_INVBOX_DISPLAYED_OBJECTS;//Reinit to proper value!!
		//	K_LOG("activate reset::");
		}

		/*char buffer2[128];
		sprintf(buffer2,"END:: iInvBxStrtObj = %d InvCnt = %d",ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj, ProfileSystem::Instance->ActiveProfile.InvObjectCnt);
		K_LOG(buffer2);*/

		///////////////////////////////////////////
		//////////// SHOW PROFILE_INVBOX //////////
		///////////////////////////////////////////
		for(int i=0; i < ProfileSystem::Instance->ActiveProfile.InvObjectCnt; i++)
		{		
			if( ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked == 0 )
				break;
			else
			{			
				//IF BUFFERED, Never DELETE OR CREATE - JUST ASSIGN FROM BUFFER
				if( GFInstance->iBufferLvl_InvHOObjLoaded == ProfileSystem::Instance->ActiveProfile.CurrentLevel )
				{
					InvBoxObj[i].HOObj = GFInstance->BufferLvlInvHOObj[ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId];
					InvBoxObj[i].HOObj->Show(true);
				}
				else
				{
					if( InvBoxObj[i].HOObj != NULL )
					{
						delete InvBoxObj[i].HOObj;
						InvBoxObj[i].HOObj = NULL;
					}
					InvBoxObj[i].HOObj = new CHOObject(VFX_REDSPARKLE);
					InvBoxObj[i].HOObj->LoadSprite(InventorySprNames[ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId]);
				}
				InvBoxObj[i].HOObj->SetZ(eZOrder_INVBOXITEMS);
				InvBoxObj[i].HOObj->PublicValue = ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId;//InvId set
				if(CheckIfItemIsComboInv(InvBoxObj[i].HOObj->PublicValue))
				{
					IsComboItemActive(InvBoxObj[i].HOObj->PublicValue, i);
				}
				InvBoxObj[i].HOObj->SetScale(INVITEM_IDLE_SCALE);
				InvBoxObj[i].MaxPieces = ProfileSystem::Instance->ActiveProfile.InvObjects[i].MaxPieces;//Maxpieces set
				InvBoxObj[i].nPiecePicked = ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked;//nPiecespicked set

				//Ensure invbox inside display, turn off all others!!
				int DisplayIndex = (i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
				CPoint tempPos;
				tempPos.x = InvBox->Pos.x + InvBoxItemXOff + (InvBoxItemSpaceX * DisplayIndex);//entire objects ACTIVATE!!!!!
				tempPos.y = InvBox->Pos.y + InvBoxItemYOff;
				InvBoxObj[i].HOObj->SetPos(&tempPos);					

				if( InvBoxObj[i].MaxPieces > 1 ) //Show text, and play "multiitem" animation
				{
					if( InvBoxObj[i].Text == NULL )//No need of creating again!!
						createMultiPiece_InvBoxObjTxt( i );
					//Just set text!!				
					InvBoxObj[i].Text->SetText("("+std::to_string(InvBoxObj[i].nPiecePicked)+"/"+std::to_string(InvBoxObj[i].MaxPieces)+")");
					//changed here.....for multiInventory Item
					//InvBoxObj[i].HOObj->PlayAnimation(1);
					if(InvBoxObj[i].HOObj->PublicValue == eInv_L1S3_ShadowHand1 || InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_ShadowHand2 )
					{
						if(InvBoxObj[i].nPiecePicked > 1 )
						{
							InvBoxObj[i].HOObj->PlayAnimation(2);
						}
						else
						{
							switch (InvBoxObj[i].HOObj->PublicValue)
							{
							case eInv_L1S3_ShadowHand1: if( !ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken)
															InvBoxObj[i].HOObj->PlayAnimation(0);
								break;
							case eInv_L1S4_ShadowHand2:	if( !ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken)  
															InvBoxObj[i].HOObj->PlayAnimation(1);
								break;
							}
						}

					}
					else
						InvBoxObj[i].HOObj->PlayAnimation(InvBoxObj[i].nPiecePicked - 1);
				}

				//Show those within 0..6 slots, all others turned off			
				if( DisplayIndex >= 0 && DisplayIndex < MAX_INVBOX_DISPLAYED_OBJECTS )//0..6 will be displayed!!
				{
					InvBoxObj[i].HOObj->ForceShow();
					if( InvBoxObj[i].Text != NULL )
						InvBoxObj[i].Text->Show(true);				
				}
				else
				{
					InvBoxObj[i].HOObj->Show(false);	
					if( InvBoxObj[i].Text != NULL )
						InvBoxObj[i].Text->Show(false);				
				} 

				//set rectangles!!
				InvBoxObj[i].Rectangle.topleft.x = InvBoxItemStartX + (InvBoxItemSpaceX*DisplayIndex) + InvBoxItem_Gap;
				InvBoxObj[i].Rectangle.topleft.y = InvBoxItemStartY + InvBoxItem_Gap;
				InvBoxObj[i].Rectangle.w = InvBoxItemSpaceX-2*InvBoxItem_Gap;
				InvBoxObj[i].Rectangle.h = InvBoxItemSpaceY-InvBoxItem_Gap;
			}
		}
}

void InventoryBox::Update()
{
	if( !isInvBoxVisible || !InvBox->Visible || (GFInstance->IsActivePopup)  )
		return;

	if(GFHud->IsMapShowing || GFHud->IsDiaryShowing || GFHud->IsGuideShowing || GFHud->IsMemoryFXTrailing)
		return;
	if( InvBoxFrame == HOBOX_FRAME || InvBoxFrame == HO_USEOBJECTS_FRAME || InvBoxFrame == HO_PLACEOBJECTS_FRAME || InvBoxFrame == HO_FINDPAIRS_FRAME || InvBoxFrame == PUZBOX_FRAME || InvBoxFrame == MATCH3_FRAME )//No need to do any further processing here!! 
	{
		if(isAreaHOorPuzzle(ProfileSystem::Instance->ActiveProfile.CurrentLevel, ProfileSystem::Instance->ActiveProfile.CurrentArea))
			return;
		else
			InvBoxFrame = INVBOX_LR_FRAME;
	}
	if ( InvBoxLeftAnimatingTicks != 0 || InvBoxRightAnimatingTicks != 0 )
		return;

	_timer += (float(Control::MasterTmr->RealDMS) / 1000.0f);
	if( _timer > 10.0f )
	{
		_timer = 0.0f;
		CrossCheckInventoryItems();
	}

	if(Control::Input->LBclicked())
	{
		if(Control::Input->Mpos().y > 640)
		{
			GFHud->InvBoxCtrl->NavigateText->Text = "";
			GFHud->InvBoxCtrl->travelArea = -1;
			GFHud->InvBoxCtrl->navTapped = false;
			GFHud->InvBoxCtrl->Navigate->Show(false);
			GFHud->InvBoxCtrl->NavArrow->Show(false);
		}
	}

	if(GFHud->Tutor != NULL && GFHud->Tutor->IsActive && GFHud->Tutor->IsVisible)
	{
		if(GFHud->Tutor->State == TUT19_ComboInvUseCombine)
		{
			//Since HUD->Tutorial->Update not Caling in this tutorial state
			GFHud->Tutor->Update();
		}
	}

	if(GFApp->RefreshInvItems)
	{
		CrossCheckInventoryItems();
		GFApp->RefreshInvItems = false;
	}
	if(IsHoverItem && HoverItemId != -1)
	{
		if(InvBoxObj[HoverItemId].HOObj->GetScale()<INVITEM_HOVER_SCALE)
		{
			float scale = InvBoxObj[HoverItemId].HOObj->GetScale() + 4.0f*Control::LogicRate;
			if(scale>INVITEM_HOVER_SCALE)
				scale=INVITEM_HOVER_SCALE;
			InvBoxObj[HoverItemId].HOObj->SetScale(scale);
		}
		HoverItemId = -1;
	}
	else
	{
		ToolTip->Show(false);
		ToolText->Show(false);
	}

	IsHoverItem = false;
	if(!IsToolTipShowing)
	{
		ToolTip->Show(false);
		ToolText->Show(false);
	}

	//FOR LOCKED NO NEED TO SLIDE UP/DOWN
	switch( SlidingState )//Handle exposed invbox case!!
	{
	case Sliding_Down: SlideDown();
		break;
	case Sliding_Up:   SlideUp();
		break;
	}

	if(GFHud->IsComboInvPopupShowing && GFHud->comboInvSystemCtrl->isAnyAnimActive)
		return;

	if( IsDragging )
		dragTicks++;

	CPoint mosPos = Control::Input->Mpos();

	if( Cursor::Visible || IsDragging )//avoid invbox opening while flying!!
	{
		if( IsDragging )//During dragging mode this window will be setmodal - only active "update"!! 
		{
			if( mosPos.x >= 214 && mosPos.x <= 811 )
				if( InvBox->Pos.y != InvBoxUpPos.y  )
				{	
					if( IsOpen() )//suit idle(open state)
					{
						if( SlidingState != Sliding_Up )
							if( InvBoxRect.Intersection(&mosPos) )//if close or closing - revive
								Activate();
					}		
					//suit trans in/out!!
					else 
					{
						if( InvBoxRect.Intersection(&mosPos) )
							Activate();
					}
				}
		}
		//Dragging over - just dropped dragelements!!
		else 
		{

			if( IsLocked )
			{
				if( SlidingState == Sliding_Nil  )
				{
					if( InvBoxRect.Intersection(&mosPos) )
						Activate();
				}
			}
			else 
			{
				if( SlidingState == Sliding_Nil && InvBox->Pos.y == InvBoxDownPos.y )
				{
					if( InvBoxActivateRect.Intersection(&mosPos))
						Activate();
				}
			}
		}
	}



	if( ItemFlyStatus != Inv_StandStill )//some flying elements
	{
		//reached target!!
		if( DragIndex != -1 )
		{
			if( ItemFlyStatus == Inv_WaitForVFX )
			{
				if( InvBox->Pos.y < InvBoxDownPos.y  )//Update dragging show status
				{						
					CPoint tempPos = InvBox->Pos;
					tempPos.x += InvBoxItemXOff + InvBoxItemSpaceX * (DragIndex-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);//drag
					tempPos.y += InvBoxItemYOff;
					InvBoxObj[DragIndex].HOObj->SetPos(&tempPos);
					InvBoxObj[DragIndex].HOObj->ForceShow();
					if( InvBoxObj[DragIndex].Text != NULL )
					{
						tempPos.x += InvBoxItemTxtOffsetX; 
						tempPos.y += InvBoxItemTxtOffsetY;
						InvBoxObj[DragIndex].Text->SetPos(&tempPos);	
						InvBoxObj[DragIndex].Text->Show(true);						
					}
					InvBoxObj[DragIndex].HOObj->Status = eHO_Hidden;
				}				
				if( !InvBoxObj[DragIndex].HOObj->Effect->IsParticlesActive()  )
				{
					ItemFlyStatus = Inv_StopFlying;								
				}				
			}
			else
			{
				bool IsTargetReached = false;
				if( InvBoxObj[DragIndex].HOObj->Status == eHO_Finished || InvBoxObj[DragIndex].HOObj->Status ==  eHO_StandStill )
					IsTargetReached = true;
				//We slide open the closedinvbox inbetween & object gone past it's original pos!!
				else if( InvBox->Pos.y == InvBoxUpPos.y  && 
					SlidingState != Sliding_Down && InvBoxObj[DragIndex].HOObj->Pos.y > Origin.y  )
				{
					IsTargetReached = true;
					InvBoxObj[DragIndex].HOObj->Pos.x = Origin.x;
					InvBoxObj[DragIndex].HOObj->Pos.y = Origin.y;						
					InvBoxObj[DragIndex].HOObj->ForceShow();
					InvBoxObj[DragIndex].HOObj->Status = eHO_Hidden;
				}

				if( IsTargetReached )
				{					
					ResetDragging();					
					if( InvBoxObj[DragIndex].HOObj->Effect->IsParticlesActive()  )
					{
						if( InvBox->Pos.y == InvBoxDownPos.y )//Hide it under along with other neighbours!! 
						{						
							CPoint tempPos = InvBox->Pos;
							tempPos.x += InvBoxItemXOff + InvBoxItemSpaceX * (DragIndex-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);//drag
							tempPos.y += InvBoxItemYOff;
							InvBoxObj[DragIndex].HOObj->SetPos(&tempPos);
							InvBoxObj[DragIndex].HOObj->ForceShow();

							if( InvBoxObj[DragIndex].Text != NULL )
							{
								tempPos.x += InvBoxItemTxtOffsetX; 
								tempPos.y += InvBoxItemTxtOffsetY;
								InvBoxObj[DragIndex].Text->SetPos(&tempPos);	
								InvBoxObj[DragIndex].Text->Show(true);
							}

							InvBoxObj[DragIndex].HOObj->Status = eHO_Hidden;
						}
						InvBoxObj[DragIndex].HOObj->Effect->DA = 2;//wait till fadeout quickly!!
						ItemFlyStatus = Inv_WaitForVFX;
					}
					else
					{						
						ItemFlyStatus = Inv_StopFlying;
					}
				}
			}

			if( ItemFlyStatus == Inv_StopFlying )
			{
				if( SlidingState == Sliding_Up || SlidingState == Sliding_Down )//During Transition - never change those invbox states!!
				{
					EndDrag();
					ItemFlyStatus = Inv_StandStill;
				}
				else
				{
					SlidingState = Sliding_Nil;
					State = InvBox_Idle;
					EndDrag();					

					if( InvBox->Pos.y > InvBoxUpPos.y || IsLocked )//invbox not open!!
					{					
						Close();		
						//Control::Audio->PlaySample(aSFXClick);
						Deactivate();										
					}
					ItemFlyStatus = Inv_StandStill;				
				}
			}			
		}
		return;		
	}


	if( State != InvBox_Idle )//transin, out and inactive 
		return;

	//Handle InvBox_idle - open invbox 	
#if ( defined  K_iOS) || ( defined  K_ANDROID)
	bool click;
	if( IsDragging )
		click = !Control::Input->LBdown();
	else
		click = Control::Input->LBjustdown();    
#else
	bool click = Control::Input->LBclicked();
#endif



	if( IsDragging )
	{
		InvBoxObj[DragIndex].HOObj->SetPos(&mosPos);
#ifdef TOUCH_UI
		if(InvBoxObj[DragIndex].HOObj->PublicValue != eInv_L4_S1_Bracelet) //Some exceptions
		{
			InvBoxObj[DragIndex].HOObj->SetY(InvBoxObj[DragIndex].HOObj->Pos.y - 75);
		}
		InvBoxObj[DragIndex].HOObj->SetScale(1.5f);
#endif
		if( Control::Input->RBclicked() )//cancel drag!!
		{
			HandleFlyDrop(DragIndex);			
			return;
		}

		if(click)//handle canceldrag or droptoscene!
			{ // Sac: added this extra Braces to avoide warning suggeston of ambiguous else
				if( dragTicks > 20 )
				{

					if( InvBoxRect.Intersection(&mosPos) )			
						HandleDirectDrop(DragIndex);													
					else
					{
						SendMessage(ActionProcessed, ID, INVBOX_DROP);	
					}
					return;
				}
				else
				{
					Control::Input->ClearMouse();
				}
			}
	}	


	//oustide invbox rectangle
	if(  !InvBoxRect.Intersection(&mosPos) )//FOR LOCKED NO CLOSE!!
	{

		if( GFHud )
			if( GFHud->foundInvPopupCtrl->State != FoundInvPopup_Inactive )//Inventory popup is active
				return;		
		if( State != InvBox_Inactive && InvBox->Pos.y < InvBoxDownPos.y )
		{
			//outside invbox to close it
			if( IsDragging )
			{
				if( !NoAutoClose )
				{
					if( !ProfileSystem::Instance->ActiveProfile.IsInvLocked )
						TurnOffArrowsLock();
					SlidingState = Sliding_Down;									
				}
			}
			else
			{				
				NoAutoClose  = false; //No linked Items

				//calculate autoclose state!!
				if( !NoAutoClose )
				{
					//Control::Audio->PlaySample(aSFXClick);
					Control::Input->ClearMouse();
					State = InvBox_Idle;
					Close();
				}
				//Even NoAutoClose can be closed by clicking outside!!
				else if( click )
				{

					NoAutoClose = false;
					State = InvBox_Idle;
					//Control::Audio->PlaySample(aSFXClick);
					Control::Input->ClearMouse();
					Close();
				}
			}
		}

	}		
	else if( !IsDragging )//invbox collided - check if coll with any entity!!
	{	
		int PrevCursorMode = Cursor::GetMode();
		Cursor::SetMode(CA_Normal);

		for(int i=ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj; i < (ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+MAX_INVBOX_DISPLAYED_OBJECTS) && i < MAX_INVBOX_OBJECTS; ++i)
		{
			if( InvBoxObj[i].HOObj != NULL  )
			{		
				if( InvBoxObj[i].HOObj->Visible && InvBoxObj[i].Rectangle.Intersection(&mosPos) )
				{
					IsHoverItem = true;

					HoverItemId = i;
					//InvBoxObj[i].HOObj->SetScale(1.0f);

					if(GFHud->Tutor != NULL && GFHud->Tutor->IsActive && GFHud->Tutor->IsVisible)
					{
						if(GFHud->Tutor->State == TUT09_RosePopupZoom || GFHud->Tutor->State == TUT11_MorphObject || GFHud->Tutor->State == TUT12_NavigationUp || GFHud->Tutor->State == TUT13_HintTutorial || GFHud->Tutor->State == TUT14_MapTutorial)
						{
							//Return for disabling combo items during tutorials
							return;
						}
					}

					bool IsAComboItem = CheckIfItemIsComboInv(InvBoxObj[i].HOObj->PublicValue);

					if(!GFHud->IsComboInvPopupShowing)
						Cursor::SetMode(IsAComboItem?CA_Look:CA_HandPointy);
					else
						Cursor::SetMode(CA_HandPointy);

					if( Control::Input->prevLBjustdown && Control::Input->getLBD() > 1 )
					{
						click = true;
						Control::Input->ClearMouse();
					}
					if( click )
					{

						// combination inv
						if(!GFHud->IsComboInvPopupShowing)
						{
							if( IsAComboItem )
							{
								if(GFHud->Tutor != NULL && GFHud->Tutor->IsActive && GFHud->Tutor->IsVisible)
								{
									if(GFHud->Tutor->State == TUT10_UseKnifeOnRose)
									{
										//Return for disabling combo items during tutorials
										return;
									}
								}

								if( GFHud->Tutor != NULL && GFHud->Tutor->IsActive && (GFHud->Tutor->State == TUT06_BagZoomLook))
								{
									//User takes another key combination!!
									ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT06_BagZoomLook] = true;
									GFHud->Tutor->HideTutorial(true);
								}
								else if( GFHud->Tutor != NULL && GFHud->Tutor->IsActive && InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_BeetleHead && (GFHud->Tutor->State == TUT18_ComboInvZoomLook))
								{
									//Beetle Combo Tutorial
									GFHud->Tutor->HideTutorial(true);
								}

								activeComboItemIndex = i;

								//show combo inventory popup
								GFHud->comboInvSystemCtrl->ShowComboInventoryPopup(InvBoxObj[i].HOObj->PublicValue, InvBoxObj[i].HOObj->Pos.x, InvBoxObj[i].HOObj->Pos.y);
								GFHud->IsComboInvPopupShowing = true;
								return;
							}
						}
						else
						{
							if(IsAComboItem)
								return;
						}



						NoAutoClose = false;							


						Cursor::Show(false);						

						Origin = InvBoxObj[i].HOObj->Pos;
						InvBoxObj[i].HOObj->SetZ(eZOrder_INVBOXITEMS+10);

						if( InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_SwissKnife )
							InvBoxObj[i].HOObj->PlayAnimation(1);
						//if( InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_Bag ) //Why only for shear this size
						//	InvBoxObj[i].HOObj->SetScale(1.2f);
						//else
						InvBoxObj[i].HOObj->SetScale(INVITEM_DRAG_SCALE);//slightly increased size than in inventory!!
						//InvBoxObj[i].HOObj->SetScale(2.0f);
						//InvBoxObj[i].HOObj->SetPos(&(CPoint(650.0f,350.0f,0.0f)));
						InvBoxObj[i].HOObj->SetPos(&Origin);
						DragIndex = i;
						IsDragging = true;
						dragTicks = 0;
						if( InvBoxObj[DragIndex].Text != NULL )
							InvBoxObj[i].Text->Show(false);

						Control::Audio->PlaySample(aSFXInvItemPick);					
						Control::Input->ClearMouse();						
					}
					else //During dragging state no need for mouseover
					{
						if( MouseOverItem == InvBoxObj[i].HOObj->PublicValue )
						{
							MouseHoverTime += Control::LogicRate;

							if( MouseHoverTime > 0.2f)
							{
								if (!IsToolTipShowing)
								{

#ifdef JAPANESE_BUILD
									ToolText->SetText(GFInstance->JapInventoryDescrips(MouseOverItem));
#else
									ToolText->SetText(StringTable::getStr(InventoryDescrips[MouseOverItem]));
#endif
								/*
									int ToolTipWidth = ToolText->StringWidth();									

									if( ToolTipWidth <= 40 )		ToolTip->PlayAnimation(0);
									else if( ToolTipWidth <= 74 )	ToolTip->PlayAnimation(1);
									else if( ToolTipWidth <= 105 )	ToolTip->PlayAnimation(2);
									else if( ToolTipWidth <= 135 )	ToolTip->PlayAnimation(3);
									else if( ToolTipWidth <= 160 )	ToolTip->PlayAnimation(4);
									else							ToolTip->PlayAnimation(5);
*/

 int ToolTipWidth = ToolText->StringWidth();
 int InvToolTipTotalWidths = 	atoi(StringTable::getStr("InvToolTipTotalWidths").c_str());
	for(int itr=0; itr < InvToolTipTotalWidths ; ++itr)
		{

			int InvToolTipTotalWidth= atoi(StringTable::getStr("InvToolTipWidth"+std::to_string(itr)).c_str());
					if(i<InvToolTipTotalWidths)
					{
						if( ToolTipWidth <= InvToolTipTotalWidth )
				  		{
						  ToolTip->PlayAnimation(itr);
					  		break;
					 	 }
					}
					else
					{
					  ToolTip->PlayAnimation(8);
					  break;
					}
				  
		}

		/*				   if( ToolTipWidth <= 40 )   //Sac: was 81 before, but that is not enough for more than 50, so changing
						   {
							   ToolTip->PlayAnimation(0);//-51
						   }
						   else if( ToolTipWidth <= 74 )//9=>+3 //Sac: wqas 95, changing to 74
						   {
							   ToolTip->PlayAnimation(1);//-60
						   }
						   else if( ToolTipWidth <= 105 )//14=>+4 //was 119
						   {
							   ToolTip->PlayAnimation(2);//-74
						   }
						   else if( ToolTipWidth <= 135 )//9=>+3 //was 139
						   {
							   ToolTip->PlayAnimation(3);//-83
						   }
						   else if( ToolTipWidth <= 160 )//14=>+4 //was 169
						   {
							   ToolTip->PlayAnimation(4);//-97
						   }
						   else if( ToolTipWidth <= 180 )//8=>+3
						   {
							   ToolTip->PlayAnimation(5);//-105
						   }
						   else if( ToolTipWidth <= 200 )//10=>+3
						   {
							   ToolTip->PlayAnimation(6);//-115
						   }
						   else //~220 and all++ - //4=>+1
						   {
							   ToolTip->PlayAnimation(7);//-119
						   }
*/
									IsToolTipShowing = true;
									ToolTip->FadeIn(8.0f);

									if(CheckIfItemIsComboInv(InvBoxObj[i].HOObj->PublicValue))
										mosPos.y -= 34; 
									else
										mosPos.y -= 24;
									ToolTip->SetPos(&mosPos);								
									mosPos.x -= 2.0f;
#ifdef JAPANESE_BUILD
									mosPos.y -= 16.0f;
#else
									mosPos.y -= 0.0f;
#endif
									ToolText->SetPos(&mosPos);
									ToolText->Show(true);
									ToolTip->Show(true);
									ToolText->FadeInText(8.0f); 
								}
								else
								{
									if(CheckIfItemIsComboInv(InvBoxObj[i].HOObj->PublicValue))
										mosPos.y -= 34; 
									else
										mosPos.y -= 24;
									ToolTip->SetPos(&mosPos);								
									mosPos.x -= 2.0f;
#ifdef JAPANESE_BUILD
									mosPos.y -= 16.0f;
#else
									mosPos.y -= 0.0f;
#endif
									ToolText->SetPos(&mosPos);
								}
							}
						}
						else
							MouseHoverTime = 0.0f;

						if( MouseOverItem != InvBoxObj[i].HOObj->PublicValue )
						{
							Control::Audio->PlaySample(aSFXMouseover);
							MouseOverItem = InvBoxObj[i].HOObj->PublicValue;
							IsToolTipShowing = false;
						}

						//SendMessage(ActionProcessed, ID, INVBOX_MOUSEOVER);
					}
					return;
				}
				else if ( MouseOverItem == InvBoxObj[i].HOObj->PublicValue )
				{
					InvBoxObj[i].HOObj->SetScale(INVITEM_IDLE_SCALE);
					IsToolTipShowing = false;
					MouseHoverTime = 0.0f;
				}
				else
				{
					InvBoxObj[i].HOObj->SetScale(INVITEM_IDLE_SCALE);
				}
				//InvItem Count correction
				if(InvBoxObj[i].MaxPieces == 1)
				{
					if(InvBoxObj[i].Text != NULL)
						InvBoxObj[i].Text->Show(false);
				}
			}			
		}

		if( InvBox->Pos.y == InvBoxUpPos.y )//IBLEFTARR, IBRGHTARR & LOCK BUTTON never clicked during sliding up/down!! 
		{
			InitArrowsLock();

			if( InvBoxLockRect.Intersection(&mosPos) )
			{
				IsHoverItem = true;
				Cursor::SetMode(CA_HandPointy);			
				if( click )
				{		
					ProfileSystem::Instance->ActiveProfile.IsInvLocked = !ProfileSystem::Instance->ActiveProfile.IsInvLocked;										
					IsLocked = ProfileSystem::Instance->ActiveProfile.IsInvLocked;
					if( IsLocked )
						IBLock->ForcePlayAnimation(0);					
					else
						IBLock->ForcePlayAnimation(1);					
					Control::Input->ClearMouse();
				}
			}
			//If IBLeftArrowRect
			else if( IBLeftArrRect.Intersection(&mosPos) )
			{			
				if( ProfileSystem::Instance->ActiveProfile.InvObjectCnt > MAX_INVBOX_DISPLAYED_OBJECTS )
				{
					if( ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj > 0 )//Mouse over only for this!!						
					{
						IsHoverItem = true;
						Cursor::SetMode(CA_HandPointy);
						IBLArr->ForcePlayAnimation(0);//LEFT ARROW NORMAL			
						if( click )
						{				
							Control::Audio->PlaySample(aSFXInvArrowClick);

							if( ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj >= MAX_INVBOX_DISPLAYED_OBJECTS  )
								ShiftInvBoxRight(MAX_INVBOX_DISPLAYED_OBJECTS);//full scene shift!!
							else
								ShiftInvBoxRight( ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj );
						}
					}
				}

			}
			else if( IBRightArrRect.Intersection(&mosPos) )
			{
				if( ProfileSystem::Instance->ActiveProfile.InvObjectCnt > MAX_INVBOX_DISPLAYED_OBJECTS )
				{
					if( ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj < (ProfileSystem::Instance->ActiveProfile.InvObjectCnt-MAX_INVBOX_DISPLAYED_OBJECTS))//Reinit to proper value!!
					{
						IsHoverItem = true;
						Cursor::SetMode(CA_HandPointy);					
						IBRArr->ForcePlayAnimation(0);//RIGHT ARROW HIGHLIGHTED					
						if( click )
						{
							Control::Audio->PlaySample(aSFXInvArrowClick);
							if( (ProfileSystem::Instance->ActiveProfile.InvObjectCnt - ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= (2*MAX_INVBOX_DISPLAYED_OBJECTS) )
								ShiftInvBoxLeft(MAX_INVBOX_DISPLAYED_OBJECTS);//full scene shift!!
							else
								ShiftInvBoxLeft( (ProfileSystem::Instance->ActiveProfile.InvObjectCnt - ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) - MAX_INVBOX_DISPLAYED_OBJECTS );
						}
					}
				}
			}
			else
			{
				if( InvBoxRect.Intersection(&mosPos) )//we collided with box, buth not with any invitems!!
					Cursor::SetMode(CA_HandPointy);//to avoid cursor switching glitch when we slide over inv items!!
				else
					Cursor::SetMode(PrevCursorMode);//restore!!
			}	
		}
		else
		{
			if( InvBoxRect.Intersection(&mosPos) )//we collided with box, buth not with any invitems!!
				Cursor::SetMode(CA_HandPointy);//to avoid cursor switching glitch when we slide over inv items!!
			else
				Cursor::SetMode(PrevCursorMode);//restore!!
		}
	}
}

bool InventoryBox::CheckIfItemIsComboInv(int invId)
{
	bool isCombo = false;
	switch (invId)
	{
	case eInv_L1S1_Bag:
	case eInv_L1S4_BeetleHead:
	case eInv_L1S4_Diary:
	case eInv_L1S4_PhotoFrame:
	case eInv_L1S5_HammerHead:
	case eInv_L2S4_AxeBlunt:
	case eInv_L2S6_KnifeBlunt:
	case eInv_L3S1_FlagFull:
	case eInv_L3S1_LadderBroken:
	case eInv_L3S2_CoconutFull:
	case eInv_L3S2_CocoShellPowderFilled:
	case eInv_L3S2_HammerHead:
	case eInv_L3S3_Box:
	case eInv_L4_S1_Leaf:
	case eInv_L4_CB_HandKerchief:
	case eInv_L4_S2_GrassBunch:
	case eInv_L4_S2_Bamboo:
	case eInv_L4_S5_Net:
	case eInv_L4_S4_BinocularWithoutLens:
	case eInv_L5_S1_Bow:
	case eInv_L5_S4_PickaxeBlade:
		//case eInv_L6_S4_HeartContainer:
	case eInv_L6_S4_Rope:
	case eInv_L7_S1_PotEmpty:
	case eInv_L7_S2_SphereHalfBase:
	case eInv_L8_S2_HalfKey:

		isCombo = true;
		break;
	case eInv_L5_S4_PeacockChain:
		if(!ProfileSystem::Instance->ActiveProfile.L5_S6_InvPeacockLocketTaken && ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockChainUsed)
		{
			isCombo = true;
		}
		break;
	case eInv_L8_S1_MagicWand:
		isCombo = true;
		break;
	default:
		break;
	}
	return isCombo;
}

void InventoryBox::updateComboItemStatus(int invId)
{
	//Update from outside
	for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
	{
		if( InvBoxObj[i].HOObj != NULL  )
		{	
			int linvId = InvBoxObj[i].HOObj->PublicValue;
			if(linvId == invId)
			{
				IsComboItemActive(invId, i);
				break;
			}
		}
	}
}

int InventoryBox::GetActiveComboItemId()
{
	int itemID = -1;
	for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
	{
		int invId = ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId;
		if( invId != -1 )
		{
			if(CheckIfItemIsComboInv(invId) && IsComboItemActive(invId, i))
			{
				IsLocked = true;																							 
				Activate();

				itemID = invId;
				break;
			}
		}
	}

	return itemID;
}

void InventoryBox::UpdateAllComboInvItems()
{
	//int itemID = -1;  //Sac: Unused variable, so commented it
	for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
	{
		if( InvBoxObj[i].HOObj != NULL  )
		{	
			int invId = InvBoxObj[i].HOObj->PublicValue;
			if(CheckIfItemIsComboInv(invId))
			{
				IsComboItemActive(invId, i);
			}
		}
	}
}

bool InventoryBox::IsComboItemActive(int invId, int invPosID)
{
	bool isActive = false;
	switch (invId)
	{
	case eInv_L1S1_Bag:
		if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken)
			isActive = true;

		if(!ProfileSystem::Instance->ActiveProfile.L1S1_BagOpened)
		{
			//And has the gems in hand, Active till taking the creek letter
			if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken &&
				ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken)
				isActive = true;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsInvLetterCreekTaken)
		{
			isActive = true;
		}
		break;
	case eInv_L1S4_BeetleHead:
		if(ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken)
		{
			isActive = true;
		}
		break;
	case eInv_L1S4_Diary:
		if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvCoinTaken)
		{
			isActive = true;
		}
		break;
	case eInv_L1S4_PhotoFrame:
		if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvBookCodeTaken)
		{
			isActive = true;
		}
		break;
	case eInv_L1S5_HammerHead:
		if(ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken)
		{
			isActive = true;
		}		
		break;
		//Level 2
	case eInv_L2S4_AxeBlunt:
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken)
		{
			isActive = true;
		}
		break;
	case eInv_L2S6_KnifeBlunt:
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken)
		{
			isActive = true;
		}
		break;

		//Level 3
	case eInv_L3S1_FlagFull:
		isActive = true;
		break;

	case eInv_L3S1_LadderBroken:
		if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderPiecesFixed)
		{
			if(ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken && ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken)
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(2);
				isActive = true;
			}
			else
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(1);
			}
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderBrokenStepFixed)
		{
			if(ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagClothTaken)
			{
				isActive = true;
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(4);
			}
			else
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(3);
			}
		}
		return isActive;

		break;

	case eInv_L3S2_CoconutFull:
		{
			if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved)
			{
				if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken)
				{
					if (InvBoxObj[invPosID].HOObj)
						InvBoxObj[invPosID].HOObj->PlayAnimation(2);
					isActive = true;
				}
				else
				{
					if (InvBoxObj[invPosID].HOObj)
						InvBoxObj[invPosID].HOObj->PlayAnimation(1);
				}
			}
			else
			{
				if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken)
				{
					if (InvBoxObj[invPosID].HOObj)
						InvBoxObj[invPosID].HOObj->PlayAnimation(5);
					isActive = true;
				}
				else
				{
					if (InvBoxObj[invPosID].HOObj)
						InvBoxObj[invPosID].HOObj->PlayAnimation(4);
				}
			}

			return isActive;
		}
		break;

	case eInv_L3S2_CocoShellPowderFilled:
		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutWithTwineTaken && ProfileSystem::Instance->ActiveProfile.L3_S2_InvTwineTaken )
		{
			isActive = true;
		}
		break;

	case eInv_L3S2_HammerHead:
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHandTaken)
		{
			isActive = true;
		}
		break;

	case eInv_L3S3_Box:
		isActive = true;
		break;

		//Level 4
	case eInv_L4_S1_Leaf:
		{
			if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonCollected)
			{
				if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvStickTaken)
				{
					if (InvBoxObj[invPosID].HOObj)
						InvBoxObj[invPosID].HOObj->PlayAnimation(2);
					isActive = true;
					return isActive;
				}
				else
				{
					if (InvBoxObj[invPosID].HOObj)
						InvBoxObj[invPosID].HOObj->PlayAnimation(1);
					isActive = false;
					return isActive;
				}

			}
		}
		break;
	case eInv_L4_CB_HandKerchief:
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvHandKerchiefTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvKnifeCollected && !ProfileSystem::Instance->ActiveProfile.L4_S1_HandKerchiefTored)
		{
			isActive = true;
		}
		break;
	case eInv_L4_S2_Bamboo:
		{
			if (!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife])
			{
				if (!ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo)
				{
					if (InvBoxObj[invPosID].HOObj)
						InvBoxObj[invPosID].HOObj->PlayAnimation(1);
				}
			}

			if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife] && !ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo)
			{
				isActive = true;
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(2);
			}

			if (ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo && !ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_KerchiefPiece2])
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(3);
			}

			if (ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_KerchiefPiece2])
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(4);
				isActive = true;
			}

			return isActive;
		}
		break;
	case eInv_L4_S2_GrassBunch:
		{
			if (ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken && !ProfileSystem::Instance->ActiveProfile.L4_S2_InvRopeTaken)//TODO:sujin
			{
				isActive = true;
				if (InvBoxObj[invPosID].HOObj)
				{
					InvBoxObj[invPosID].HOObj->Paused = true;
					InvBoxObj[invPosID].HOObj->SetFrame(ProfileSystem::Instance->ActiveProfile.L4_s2_GrassRopeFrameNumber);
				}
			}
		}
		break;
	case eInv_L4_S4_BinocularWithoutLens:
		if (ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvBinocularLensTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularWithLensTaken)
		{
			isActive = true;
		}
		break;

	case eInv_L4_S5_Net:
		{
			bool isAllBambooSticksTaken = false;

			if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken
				&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken
				&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken
				&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken)
			{
				isAllBambooSticksTaken = true;
			}

			if (!isAllBambooSticksTaken)
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(1);
				return isActive;
			}

			if (isAllBambooSticksTaken && !ProfileSystem::Instance->ActiveProfile.L4_S5_ComboBambooFrameAddedToNet)
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(2);
				isActive = true;
				return isActive;
			}

			if (ProfileSystem::Instance->ActiveProfile.L4_S5_ComboBambooFrameAddedToNet && !ProfileSystem::Instance->ActiveProfile.L4_S2_InvRopeTaken)
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(3);
				return isActive;
			}

			if (ProfileSystem::Instance->ActiveProfile.L4_S5_ComboBambooFrameAddedToNet && ProfileSystem::Instance->ActiveProfile.L4_S2_InvRopeTaken)
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(4);
				isActive = true;
				return isActive;
			}
		}
		break;

		//Level 5
	case eInv_L5_S1_Bow:
		if (!ProfileSystem::Instance->ActiveProfile.L5_S1_BowStringFixed)
		{
			if (ProfileSystem::Instance->ActiveProfile.L5_S1_BowArrowHolderPlaced)
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(5);
			}
			else
			{
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(2);
			}

			isActive = true;
			return isActive;
		}
		else
		{
			if (!ProfileSystem::Instance->ActiveProfile.L5_S1_BowArrowHolderPlaced)
			{
				//Bow holder is not placed
				if (!ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
				{
					if (InvBoxObj[invPosID].HOObj)
						InvBoxObj[invPosID].HOObj->PlayAnimation(3);

					isActive = false;
					return isActive;
				}
				else
				{
					//player has bow holder
					if (InvBoxObj[invPosID].HOObj)
						InvBoxObj[invPosID].HOObj->PlayAnimation(4);

					isActive = true;
					return isActive;
				}
			}
			else
			{
				//Bow holder placed. String is fixed
				if (InvBoxObj[invPosID].HOObj)
					InvBoxObj[invPosID].HOObj->PlayAnimation(6);

				if (ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken && !ProfileSystem::Instance->ActiveProfile.L5_S1_BowArrowFirstLoaded)
					isActive = true;
				else if (ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken && !ProfileSystem::Instance->ActiveProfile.L5_S1_BowArrowSecondLoaded)
					isActive = true;
				else
					isActive = true;

				return isActive;
			}
		}
		break;
	case eInv_L5_S4_PickaxeBlade:
		if (ProfileSystem::Instance->ActiveProfile.L5_S5_InvPickaxeHandleTaken)
		{
			isActive = true;
		}
		break;
	case eInv_L5_S4_PeacockChain:
		if (!ProfileSystem::Instance->ActiveProfile.L5_S6_InvPeacockLocketTaken && ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockChainUsed)
		{
			isActive = true;
		}
		break;
	case eInv_L6_S4_HeartContainer:
		{
			//if (ProfileSystem::Instance->ActiveProfile.L6_s4)//TODO:sujin
			{
				isActive = true;
			}
		}
		break;
	case eInv_L6_S4_Rope:
		{
			if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnetisedHorseShoeTaken)//TODO:sujin
			{
				isActive = true;
			}
		}
		break;

	case eInv_L7_S1_PotEmpty:
		{
			if (!ProfileSystem::Instance->ActiveProfile.L7_S1_ComboPotChainPlaced && ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken)
			{
				isActive = true;
			}
			else if (ProfileSystem::Instance->ActiveProfile.L7_S1_ComboPotChainPlaced && ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken)
			{
				isActive = true;
			}
		}
		break;

	case eInv_L7_S2_SphereHalfBase:
		{
			if (ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfLeftToFixTaken)
			{
				isActive = true;
			}
		}
		break;
	case eInv_L8_S1_MagicWand:
		{
			if (ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken)
			{
				isActive = true;
			}
		}
		break;
	case eInv_L8_S2_HalfKey:
		{
			if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_HalfKey] && !ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey])
			{
				isActive = true;
				//if (InvBoxObj[invPosID].HOObj)
				//InvBoxObj[invPosID].HOObj->PlayAnimation(2);
			}
		}

	default:
		break;
	}

	if(isActive)
	{
		if(InvBoxObj[invPosID].HOObj!=NULL)
			InvBoxObj[invPosID].HOObj->PlayAnimation(2);
	}
	else
	{
		if(InvBoxObj[invPosID].HOObj!=NULL)
			InvBoxObj[invPosID].HOObj->PlayAnimation(1);
	}

	return isActive;
}

void InventoryBox::IBSetPos(const CPoint* pos)
{
	CPoint tempPos = (*pos);

	InvBox->SetPos(pos);


	for(int i=0; i < MAX_INVBOX_OBJECTS; ++i)
	{
		if( InvBoxObj[i].HOObj == NULL || i == DragIndex )
			continue;

		int DisplayIndex = (i - ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
		tempPos = (*pos);
		tempPos.x += InvBoxItemXOff + (InvBoxItemSpaceX * DisplayIndex);//entire objects SET POS!!!!! 1RST CREATION TIME!!
		tempPos.y += InvBoxItemYOff;
		InvBoxObj[i].HOObj->SetPos(&tempPos);

		if( InvBoxObj[i].Text != NULL )
		{
			tempPos.x += InvBoxItemTxtOffsetX; 
			tempPos.y += InvBoxItemTxtOffsetY;
			InvBoxObj[i].Text->SetPos(&tempPos);	
		}

		if( DisplayIndex >= 0 && DisplayIndex < MAX_INVBOX_DISPLAYED_OBJECTS )//0..6 will be displayed!!
		{
			InvBoxObj[i].HOObj->Show(true);
			if( InvBoxObj[i].Text )
				InvBoxObj[i].Text->Show(true);
		}
		else
		{
			InvBoxObj[i].HOObj->Show(false);
			if( InvBoxObj[i].Text )
				InvBoxObj[i].Text->Show(false);
		}

		//Init all invbox item rectangles
		InvBoxObj[i].Rectangle.topleft.x = InvBoxItemStartX + (InvBoxItemSpaceX*DisplayIndex) + InvBoxItem_Gap;
		InvBoxObj[i].Rectangle.topleft.y = InvBoxItemStartY + InvBoxItem_Gap;
		InvBoxObj[i].Rectangle.w = InvBoxItemSpaceX-2*InvBoxItem_Gap;
		InvBoxObj[i].Rectangle.h = InvBoxItemSpaceY-InvBoxItem_Gap;

	}
}

void InventoryBox::InitMultiPiece_InvBoxObj(int iItem, int eInvId, int nPiecesPicked, int MaxPieces, int nonRemappedInvId )
{
	int animValue = 0;

	InvBoxObj[iItem].HOObj->PublicValue = eInvId;
	InvBoxObj[iItem].MaxPieces = MaxPieces;
	if( MaxPieces == 1 )
		InvBoxObj[iItem].nPiecePicked = nPiecesPicked; //Piece count won't increase further!!
	else
	{

		//Multipiece object text, and anim!!
		if( InvBoxObj[iItem].Text == NULL )//No need of creating again!!
			createMultiPiece_InvBoxObjTxt( iItem );
		//Just set text!!	
		switch (nonRemappedInvId)
		{
		case eInv_L1S3_ShadowHand1: if( !ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken)
										animValue = 0;
									else
										animValue = 2;
			break;
		case eInv_L1S4_ShadowHand2:	if( !ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken)  
										animValue = 1;
									else
										animValue = 2;
			break;
		default:	animValue =InvBoxObj[iItem].nPiecePicked -1 ;
			break;
		}
		InvBoxObj[iItem].Text->SetText("("+std::to_string(InvBoxObj[iItem].nPiecePicked)+"/"+std::to_string(InvBoxObj[iItem].MaxPieces)+")");
		InvBoxObj[iItem].Text->Show(true);	
		InvBoxObj[iItem].HOObj->PlayAnimation(animValue);
	}
}


void InventoryBox::InitMultiPiece_InvBoxObj( int iItem, int eInvId, int nPiecesPicked, int MaxPieces )
{
	InvBoxObj[iItem].HOObj->PublicValue = eInvId;
	InvBoxObj[iItem].MaxPieces = MaxPieces;
	if( MaxPieces == 1 )
		InvBoxObj[iItem].nPiecePicked = nPiecesPicked; //Piece count won't increase further!!
	else
	{
		/*K_LOG("Adding multipiece object");

		char buffer[128];
		sprintf(buffer,"iItem = %d eInvId = %d",iItem, eInvId);
		K_LOG(buffer);
		sprintf(buffer,"Before: InvBoxObj[iItem].nPiecePicked = %d + Add = %d",InvBoxObj[iItem].nPiecePicked, nPiecesPicked);
		K_LOG(buffer);
		InvBoxObj[iItem].nPiecePicked += nPiecesPicked; 

		sprintf(buffer,"= After: InvBoxObj[iItem].nPiecePicked = %d ",InvBoxObj[iItem].nPiecePicked);
		K_LOG(buffer);*/

		//Multipiece object text, and anim!!
		if( InvBoxObj[iItem].Text == NULL )//No need of creating again!!
			createMultiPiece_InvBoxObjTxt( iItem );
		//Just set text!!	

		InvBoxObj[iItem].Text->SetText("("+std::to_string(InvBoxObj[iItem].nPiecePicked)+"/"+std::to_string(InvBoxObj[iItem].MaxPieces)+")");
		InvBoxObj[iItem].Text->Show(true);	
		InvBoxObj[iItem].HOObj->PlayAnimation(InvBoxObj[iItem].nPiecePicked -1);
	}
}

void InventoryBox::createMultiPiece_InvBoxObjTxt( int iItem )
{
	InvBoxObj[iItem].Text = new CBitmapText();
	InvBoxObj[iItem].Text->SetZ(InvBoxObj[iItem].HOObj->ZOrder+1);
	InvBoxObj[iItem].Text->SetAlignment(Align_Center);

	InvBoxObj[iItem].Text->LoadFont(eFont_25);		
	InvBoxObj[iItem].Text->SetScale(0.72f);
	CPoint tempPos = InvBox->Pos;
	tempPos.x += InvBoxItemXOff + (InvBoxItemSpaceX * (iItem-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj)) + InvBoxItemTxtOffsetX;//for text obj
	tempPos.y += InvBoxItemYOff+ InvBoxItemTxtOffsetY;
	InvBoxObj[iItem].Text->SetPos(&tempPos);	


	InvBoxObj[iItem].Text->SetColor((233/255.0f),(204/255.0f),(131/255.0f));
}



void InventoryBox::ShiftInvBoxLeft( int ShiftCnt, bool instant ) // <<
{	
//	K_LOG("InvBoxShift left");
	if ( InvBoxLeftAnimatingTicks != 0 || InvBoxRightAnimatingTicks != 0 )
		return;
//	K_LOG("InvBoxShift left starts...");
	if(!instant)
		InvBoxLeftAnimatingTicks = IBSLDANIM_NUMFRAMES;
	else
		InvBoxLeftAnimatingTicks = 0;
	ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj += ShiftCnt; 		
	//K_LOG("shifting left");
	for( int i = 0; i < ProfileSystem::Instance->ActiveProfile.InvObjectCnt; i++ )
		if( InvBoxObj[i].HOObj != NULL )
		{
			int DisplayIndex = i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj;
			CPoint tempPos;
			tempPos.x = InvBox->Pos.x + InvBoxItemXOff + (InvBoxItemSpaceX * DisplayIndex) + InvBoxLeftAnimatingTicks*IBSLDANIM_MOVEX_PERFRAME;
			tempPos.y = InvBox->Pos.y + InvBoxItemYOff;
			InvBoxObj[i].HOObj->SetPos(&tempPos);
			if( DisplayIndex >= 0 && DisplayIndex < MAX_INVBOX_DISPLAYED_OBJECTS  )
				InvBoxObj[i].HOObj->Show(true);
			else
				InvBoxObj[i].HOObj->Show(false);

			if( InvBoxObj[i].Text != NULL )
			{
				tempPos.x += InvBoxItemTxtOffsetX;
				tempPos.y += InvBoxItemTxtOffsetY;
				InvBoxObj[i].Text->SetPos(&tempPos);	
				if( DisplayIndex >= 0 && DisplayIndex < MAX_INVBOX_DISPLAYED_OBJECTS  )
					InvBoxObj[i].Text->Show(true);
				else
					InvBoxObj[i].Text->Show(false);
			}	

			//set rectangles!!		
			InvBoxObj[i].Rectangle.topleft.x = InvBoxItemStartX + (InvBoxItemSpaceX*DisplayIndex) + InvBoxItem_Gap;
			InvBoxObj[i].Rectangle.topleft.y = InvBoxItemStartY + InvBoxItem_Gap;
			InvBoxObj[i].Rectangle.w = InvBoxItemSpaceX-2*InvBoxItem_Gap;
			InvBoxObj[i].Rectangle.h = InvBoxItemSpaceY-InvBoxItem_Gap;
		}

		//K_LOG("shift completed");	
		//New objects will fly over to the empty rightmost gaps!!!


}


void InventoryBox::ShiftInvBoxRight( int ShiftCnt , bool instant) // >> - for multipiece objects only this needed!!
{
	//K_LOG("InvBoxShift right");
	if ( InvBoxLeftAnimatingTicks != 0 || InvBoxRightAnimatingTicks != 0 )
		return;
//	K_LOG("InvBoxShift right starts...");
	if(!instant)
		InvBoxRightAnimatingTicks = IBSLDANIM_NUMFRAMES;
	else
		InvBoxRightAnimatingTicks = 0;
	//K_LOG("shifting right");
	ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj -= ShiftCnt; 		
	for( int i = 0; i < ProfileSystem::Instance->ActiveProfile.InvObjectCnt; i++ )
		if( InvBoxObj[i].HOObj != NULL )
		{
			int DisplayIndex = i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj;
			CPoint tempPos;
			tempPos.x = InvBox->Pos.x + InvBoxItemXOff + (InvBoxItemSpaceX * DisplayIndex) - InvBoxRightAnimatingTicks*IBSLDANIM_MOVEX_PERFRAME;
			tempPos.y = InvBox->Pos.y + InvBoxItemYOff;
			InvBoxObj[i].HOObj->SetPos(&tempPos);
			if( DisplayIndex >= 0 && DisplayIndex < MAX_INVBOX_DISPLAYED_OBJECTS  )
				InvBoxObj[i].HOObj->Show(true);
			else
				InvBoxObj[i].HOObj->Show(false);

			if( InvBoxObj[i].Text != NULL )
			{
				tempPos.x += InvBoxItemTxtOffsetX;
				tempPos.y += InvBoxItemTxtOffsetY;
				InvBoxObj[i].Text->SetPos(&tempPos);	
				if( DisplayIndex >= 0 && DisplayIndex < MAX_INVBOX_DISPLAYED_OBJECTS  )
					InvBoxObj[i].Text->Show(true);
				else
					InvBoxObj[i].Text->Show(false);
			}	

			//set rectangles!!		
			InvBoxObj[i].Rectangle.topleft.x = InvBoxItemStartX + (InvBoxItemSpaceX*DisplayIndex) + InvBoxItem_Gap;
			InvBoxObj[i].Rectangle.topleft.y = InvBoxItemStartY + InvBoxItem_Gap;
			InvBoxObj[i].Rectangle.w = InvBoxItemSpaceX-2*InvBoxItem_Gap;
			InvBoxObj[i].Rectangle.h = InvBoxItemSpaceY-InvBoxItem_Gap;
		}				
		//K_LOG("shift completed");		
}

void InventoryBox::AddPendingInvToInvBox(int eInvId)
{
	if(GFHud!=NULL)
	{
		if(GFHud->foundInvPopupCtrl)
		{
			std::vector<CHOObject *> FoundObjects = GFHud->foundInvPopupCtrl->getFoundObjects();
			int Objsize = (int)FoundObjects.size();
			for(int i=0; i < Objsize; ++i)
			{
				GFHud->InvBoxCtrl->AddToInvBox(FoundObjects[i]->eInv_Id,&FoundObjects[i]->Pos);
			}

			GFHud->foundInvPopupCtrl->disposeObjects();
		}
	}
}

void InventoryBox::PostUpdateAddingToInvBox(int eInvId)
{
	//K_LOG("PostUpdateAddingToInvBox iNVsTARToBJ");
	int RemapInvId = GetRemapInvId( eInvId );
	int Slot = GetMultiPiece_InvBoxObjSlot(RemapInvId);
	if( Slot != -1 ) //If multiitem and slot present, just update that - else create new from scratch!!
	{
		//K_LOG("Multipiece invbox entry");
		HandleMultiPiece_InvObjUpdate(Slot, eInvId);		
		return;
		//No need of updating InvObjectCnt;
	}
	//////////////////////////////////////
	int ItemsCnt = 0; 
	for(; ItemsCnt < MAX_INVBOX_OBJECTS; ++ItemsCnt)
	{
		if( InvBoxObj[ItemsCnt].HOObj == NULL )
			break;
		else if( InvBoxObj[ItemsCnt].HOObj->PublicValue == RemapInvId )
			return;
	}

	InvBoxObj[ItemsCnt].HOObj->Show(true);

	//InvBoxObj[ItemsCnt].HOObj->FadeIn(4.0f);
	if( InvBoxObj[ItemsCnt].Text != NULL )
	{		
		////If new object is a multiinv object - then update piececount!!		
		InvBoxObj[ItemsCnt].Text->Show(true);			
	}
}

//working properly!!
void InventoryBox::AddToInvBox( int eInvId, CPoint *Pos )
{
	int RemapInvId = GetRemapInvId( eInvId );
	int Slot = GetMultiPiece_InvBoxObjSlot(RemapInvId);
	if( Slot != -1 ) //If multiitem and slot present, just update that - else create new from scratch!!
	{
		//K_LOG("Multipiece invbox entry");
		HandleMultiPiece_InvObjUpdate(Slot, eInvId);		
		return;
		//No need of updating InvObjectCnt;
	}
	//////////////////////////////////////
	int ItemsCnt = 0; 
	for(; ItemsCnt < MAX_INVBOX_OBJECTS; ++ItemsCnt)
	{
		if( InvBoxObj[ItemsCnt].HOObj == NULL )
			break;
		else if( InvBoxObj[ItemsCnt].HOObj->PublicValue == RemapInvId )
			return;
	}	
	if( ItemsCnt == MAX_INVBOX_OBJECTS )
	{
		KMiscTools::messageBox((const char*)StringTable::getStr("ExErrTitle").c_str(),(const char*)StringTable::getStr("ExErrInvFull").c_str() );	
		//KMiscTools::messageBox( (const char*)L"Error", (const char*)L"InvBox full!!!!!!!!");
		return;	
	}

	//IF BUFFERED, Never DELETE OR CREATE - JUST ASSIGN FROM BUFFER
	if( GFInstance->iBufferLvl_InvHOObjLoaded == ProfileSystem::Instance->ActiveProfile.CurrentLevel )
	{
		InvBoxObj[ItemsCnt].HOObj = GFInstance->BufferLvlInvHOObj[RemapInvId];
		InvBoxObj[ItemsCnt].HOObj->Show(true);
	}
	else
	{
		if( InvBoxObj[ItemsCnt].HOObj != NULL )
		{
			delete InvBoxObj[ItemsCnt].HOObj;
			InvBoxObj[ItemsCnt].HOObj = NULL;
		}
		InvBoxObj[ItemsCnt].HOObj = new CHOObject(VFX_REDSPARKLE);
		InvBoxObj[ItemsCnt].HOObj->LoadSprite(InventorySprNames[RemapInvId]);
	}
	InvBoxObj[ItemsCnt].HOObj->SetZ(eZOrder_INVBOXITEMS);
	InvBoxObj[ItemsCnt].HOObj->PublicValue = RemapInvId;
	InvBoxObj[ItemsCnt].HOObj->SetScale(INVITEM_IDLE_SCALE);

	//Combo Inventory handling adding Plus Sign
	if(CheckIfItemIsComboInv(InvBoxObj[ItemsCnt].HOObj->PublicValue))
		IsComboItemActive(InvBoxObj[ItemsCnt].HOObj->PublicValue, ItemsCnt);

	UpdateAllComboInvItems();
	
		

	if( Pos != NULL )
		InvBoxObj[ItemsCnt].HOObj->SetPos(Pos);
	InvBoxObj[ItemsCnt].HOObj->Show(true);	

	InvBoxObj[ItemsCnt].nPiecePicked = 1;
	HandleMultiPiece_InvObjUpdate(ItemsCnt, eInvId);//Last slot!!

	//InvBoxObj[ItemsCnt].HOObj->FadeIn(4.0f);
	if( InvBoxObj[ItemsCnt].Text != NULL )
	{		
		if( !IsMultiPiece_InvBoxObj(eInvId)) //if not multipiece then empty text must be displayed!!
			InvBoxObj[ItemsCnt].Text->SetText("");
		////If new object is a multiinv object - then update piececount!!		
		InvBoxObj[ItemsCnt].Text->Show(true);
	}


	int DisplayIndex = (ItemsCnt-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);

	/*
	CPoint tempPos;
	tempPos.x = InvBox->Pos.x + InvBoxItemXOff + (InvBoxItemSpaceX * DisplayIndex);//ADDTOINVBOX!!!!!!!
	tempPos.y = InvBox->Pos.y + InvBoxItemYOff;
	InvBoxObj[ItemsCnt].HOObj->SetPos(&tempPos);	
	*/

	//set rectangles!!		
	InvBoxObj[ItemsCnt].Rectangle.topleft.x = InvBoxItemStartX + (InvBoxItemSpaceX*DisplayIndex) + InvBoxItem_Gap;
	InvBoxObj[ItemsCnt].Rectangle.topleft.y = InvBoxItemStartY + InvBoxItem_Gap;
	InvBoxObj[ItemsCnt].Rectangle.w = InvBoxItemSpaceX-2*InvBoxItem_Gap;
	InvBoxObj[ItemsCnt].Rectangle.h = InvBoxItemSpaceY-InvBoxItem_Gap;

	//K_LOG("New invbox entry");
	ProfileSystem::Instance->ActiveProfile.InvObjectCnt = ItemsCnt+1;
	//K_LOG("ADDTOINVBOX iNVsTARToBJ");
	/*char buffer[128];
	sprintf(buffer,"iInvBxStrtObj = %d InvCnt = %d",ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj, ProfileSystem::Instance->ActiveProfile.InvObjectCnt);
	K_LOG(buffer);*/
}

//////////////////////////////////////////////////////////////////////////////////////
///  MULTIPIECE INVENTORY OBJECTS 'Handling' 4 functions STARTS!!! 
//////////////////////////////////////////////////////////////////////////////////////
// Level1 - 4 type multipiece objects - 4 glyphcoins, 2 books, 2 jigsaw pieces, 2 clock handles, 
//////////////////////////////////////////////////////////////////////////////////////
int InventoryBox::GetRemapInvId( int eInvId )
{
	//ReMap all multi item objects to 1rst entity!!
	switch( eInvId )
	{
		//Level 1
	case eInv_L1S1_RubyPart1:
	case eInv_L1S1_RubyPart2:
	case eInv_L1S1_RubyPart3:
	case eInv_L1S1_RubyPart4: return eInv_L1S1_RubyPart1;

	case eInv_L1S2_Plank1:
	case eInv_L1S1_Plank3:
	case eInv_L1S7_Plank2: return eInv_L1S2_Plank1;

	case eInv_L1S2_Cloth1Swing:
	case eInv_L1S2_Cloth2LovePop:
	case eInv_L1S2_Cloth3CarDicky: return eInv_L1S2_Cloth1Swing;

	case eInv_L1S3_ShadowHand1:
	case eInv_L1S4_ShadowHand2: return eInv_L1S3_ShadowHand1;

		//Level 2
	case eInv_L2S2_Pearl1:
	case eInv_L2S2_Pearl3:
	case eInv_L2S2_Pearl4: return eInv_L2S2_Pearl1;

	case eInv_L2S6_SmallStone1:
	case eInv_L2S6_SmallStone2:
	case eInv_L2S6_SmallStone3: return eInv_L2S6_SmallStone1;



		//Level 3
	case eInv_L3S2_PuzzleBlock1Left: 
	case eInv_L3S2_PuzzleBlock2Right:return eInv_L3S2_PuzzleBlock1Left;

	case eInv_L3S1_LadderPiece1:
	case eInv_L3S3_LadderPiece2:	return eInv_L3S1_LadderPiece1;


	case eInv_L3S2_PipePiece1Room1: 
	case eInv_L3S2_PipePiece2Room2:
	case eInv_L3S2_PipePiece3Room3:
	case eInv_L3S2_PipePiece4Room4: return eInv_L3S2_PipePiece1Room1;

	case eInv_L3S5_Grass1: 
	case eInv_L3S5_Grass2:
	case eInv_L3S5_Grass3:
	case eInv_L3S5_Grass4: 
	case eInv_L3S5_Grass5:
	case eInv_L3S5_Grass6:	return eInv_L3S5_Grass1;

		//Level 4
	case eInv_L4_S2_BambooStick1:
	case eInv_L4_S2_BambooStick2:
	case eInv_L4_S2_BambooStick3:
	case eInv_L4_S2_BambooStick4:return eInv_L4_S2_BambooStick1;

	case eInv_L4_S2_Twig1:
	case eInv_L4_S4_Twig2:
	case eInv_L4_S5_Twig3: return eInv_L4_S2_Twig1;


		//Level 5
		//Level 6
		//Level 7
	default: return eInvId;
	}
}

bool InventoryBox::IsMultiPiece_InvBoxObj( int eInvId )
{
	switch( eInvId )
	{
	case eInv_L1S1_RubyPart1:
	case eInv_L1S1_RubyPart2:
	case eInv_L1S1_RubyPart3:
	case eInv_L1S1_RubyPart4:

	case eInv_L1S2_Plank1:
	case eInv_L1S1_Plank3:
	case eInv_L1S7_Plank2:

	case eInv_L1S2_Cloth1Swing:
	case eInv_L1S2_Cloth2LovePop:
	case eInv_L1S2_Cloth3CarDicky:

	case eInv_L1S3_ShadowHand1:
	case eInv_L1S4_ShadowHand2:

	case eInv_L2S2_Pearl1: 
	case eInv_L2S2_Pearl3:
	case eInv_L2S2_Pearl4:

	case eInv_L2S6_SmallStone1:
	case eInv_L2S6_SmallStone2:
	case eInv_L2S6_SmallStone3:

	case eInv_L3S2_PuzzleBlock1Left:
	case eInv_L3S2_PuzzleBlock2Right: 

	case eInv_L3S1_LadderPiece1:
	case eInv_L3S3_LadderPiece2:	return true; //IT'S A MULTIPIECE OBJ!!

	case eInv_L3S2_PipePiece1Room1: 
	case eInv_L3S2_PipePiece2Room2:
	case eInv_L3S2_PipePiece3Room3:
	case eInv_L3S2_PipePiece4Room4: 
	case eInv_L3S5_Grass1: 
	case eInv_L3S5_Grass2:
	case eInv_L3S5_Grass3:
	case eInv_L3S5_Grass4: 
	case eInv_L3S5_Grass5:
	case eInv_L3S5_Grass6:	return true;

	case eInv_L4_S2_BambooStick1:
	case eInv_L4_S2_BambooStick2:
	case eInv_L4_S2_BambooStick3:
	case eInv_L4_S2_BambooStick4:

	case eInv_L4_S2_Twig1:
	case eInv_L4_S4_Twig2:
	case eInv_L4_S5_Twig3:	return true;

	default: return false;
	}	
}

int InventoryBox::GetMultiPiece_InvBoxObjSlot( int RemapInvId )
{
	if( IsMultiPiece_InvBoxObj(RemapInvId) )
	{
		//Find multipiece slot!!
		for( int i = 0; i < MAX_INVBOX_OBJECTS; i++ )		
			if( InvBoxObj[i].HOObj != NULL )
				if( InvBoxObj[i].HOObj->PublicValue == RemapInvId )
					return i;// GO BACK WITH 1RST slot!!

		return -1; //Can't find such an object in InvBox
	}
	else
		return -1;
}


/////////////////////////////////////////////////////
void InventoryBox::HandleMultiPiece_InvObjUpdate( int Slot, int eInvId )//Updates in multiple stages too....!!!
{
	int RemapInvId = GetRemapInvId(eInvId);
	switch( eInvId )
	{
	case eInv_L1S1_RubyPart1:
		if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4 );
		break;
	case eInv_L1S1_RubyPart2:
		if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4 );
		break;
	case eInv_L1S1_RubyPart3:
		if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4 );
		break;
	case eInv_L1S1_RubyPart4:
		if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4 );
		break;

	case eInv_L1S2_Plank1:
		if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3 );
		break;

	case eInv_L1S7_Plank2:
		if( ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3 );
		break;

	case eInv_L1S1_Plank3:
		if( ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3 );
		break;

	case eInv_L1S2_Cloth1Swing:
		if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3 );
		break;
	case eInv_L1S2_Cloth2LovePop:
		if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3 );
		break;
	case eInv_L1S2_Cloth3CarDicky:
		if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3 );
		break;

	case eInv_L1S3_ShadowHand1:
		if( ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 2 , eInvId);
		break;
	case eInv_L1S4_ShadowHand2:
		if( ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 2 , eInvId);
		break;

	case eInv_L2S2_Pearl1:
		if( ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3, eInvId);
		break;
	case eInv_L2S2_Pearl3:
		if( ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3, eInvId);
		break;
	case eInv_L2S2_Pearl4:
		if( ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3, eInvId);
		break;

	case eInv_L2S6_SmallStone1:
		if( ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3, eInvId);
		break;
	case eInv_L2S6_SmallStone2:
		if( ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3, eInvId);
		break;
	case eInv_L2S6_SmallStone3:
		if( ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3, eInvId);
		break;

	case eInv_L3S2_PuzzleBlock1Left:
		if( ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[0] == 0 || ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[1] == 0) //0-LeftBox
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 2, eInvId);
		break;

	case eInv_L3S2_PuzzleBlock2Right:
		if( ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[0] == 1 || ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[1] == 1) //1-RightBox
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 2, eInvId);
		break;

	case eInv_L3S1_LadderPiece1:
		if( ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken )
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 2, eInvId);
		break;

	case eInv_L3S3_LadderPiece2:
		if( ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken )
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 2, eInvId);
		break;

	case eInv_L3S2_PipePiece1Room1:
		if( ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken )
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4, eInvId);
		break; 

	case eInv_L3S2_PipePiece2Room2:
		if( ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4, eInvId);
		break;

	case eInv_L3S2_PipePiece3Room3:
		if( ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4, eInvId);
		break;

	case eInv_L3S2_PipePiece4Room4:
		if( ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4, eInvId);
		break;

	case eInv_L4_S2_BambooStick1:
		if( ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4 );
		break;

	case eInv_L4_S2_BambooStick2:
		if( ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4 );
		break;

	case eInv_L4_S2_BambooStick3:
		if( ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4 );
		break;

	case eInv_L4_S2_BambooStick4:
		if( ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 4 );
		break;

	case eInv_L4_S2_Twig1:
		if( ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3 );
		break;

	case eInv_L4_S4_Twig2:
		if( ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3 );
		break;

	case eInv_L4_S5_Twig3:
		if( ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken) 
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 3 );
		break;

	case eInv_L3S5_Grass1:
		if( ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken1)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 6, eInvId);
		break;

	case eInv_L3S5_Grass2:
		if( ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken2)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 6, eInvId);
		break;

	case eInv_L3S5_Grass3:
		if( ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken3)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 6, eInvId);
		break;

	case eInv_L3S5_Grass4:
		if( ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken4)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 6, eInvId);
		break;

	case eInv_L3S5_Grass5:
		if( ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken5)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 6, eInvId);
		break;

	case eInv_L3S5_Grass6:
		if( ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken6)
			InitMultiPiece_InvBoxObj( Slot, RemapInvId, 1, 6, eInvId);
		break;		

	default: InitMultiPiece_InvBoxObj( Slot,RemapInvId, 1, 1 );
		break;
	}

	//SAVE THIS NEW INVENTORY INFO TO PROFILE IB SLOT too (DURING NEXT ACTIVATION WE LOAD INFO FROM THAT, for a crash too this might help!! ) 
	ProfileSystem::Instance->ActiveProfile.InvObjects[Slot].InvId = RemapInvId;
	ProfileSystem::Instance->ActiveProfile.InvObjects[Slot].nPiecePicked = InvBoxObj[Slot].nPiecePicked;
	ProfileSystem::Instance->ActiveProfile.InvObjects[Slot].MaxPieces = InvBoxObj[Slot].MaxPieces;	
}
//////////////////////////////////////////////////////////////////////////////////////
///  MULTIPIECE INVENTORY OBJECTS 'Handling' ENDS!!! 
//////////////////////////////////////////////////////////////////////////////////////

#define PUZZLE_HELPBOX_WIDTH 608

void InventoryBox::SetMultilineText(std::string bigstring, int OffsetY, eTextAlignment AlignStyle, int OffsetX)
{
	std::string temp = bigstring;
	if(!bigstring.empty()) bigstring = StringTable::getStr(bigstring);
	std::vector<std::string> Strings;
	std::string CurString = "";
	std::string CurWord = "";
	int CurWidth = 0;
	int WordLength = 0;
#ifdef JAPANESE_BUILD 
	unsigned int Char;
#else
	unsigned char Char;
#endif
	int _puzzle_width = PUZZLE_HELPBOX_WIDTH;
	int _font = eFont_25;
	if(StringTable::IsKeyFound(temp))
	{
		//_puzzle_width = StringTable::GetLineWidth(temp);
		_font = StringTable::GetFont(temp);
	}
	
	//Hide all current strings
	CPoint textPos;
	for(int i=0; i < HELP_TEXT_LINES; ++i)	
		if( HelpTexts[i] != NULL )
		{
			HelpTexts[i]->SetText("");

			HelpTexts[i]->LoadFont(_font);
			textPos.x = 683;//730;//681.0f;
			//if( AlignStyle == Align_Left )
				//textPos.x -= 260;
			textPos.y = 688.0f + 30 * i;	
			HelpTexts[i]->SetPos(&textPos);		
			HelpTexts[i]->SetAlignment(AlignStyle);
		}


		//Split String
		int length = (int)bigstring.length();
		int CharWidth = 0;
		for(int i=0; i < length; ++i)
		{
			Char = bigstring.at(i);//japanese will have multibyte characters!!
			if(StringTable::IsMyLangaugeLatin())
				CharWidth = 0;//We split words based on $ sign only!! :) 
			CharWidth = HelpTexts[0]->charWidth(Char);

			if( Char == '\\'  || Char == '$')//for japanese $
			{
				CurWidth = 0;
				Strings.push_back(CurString);

				CurString = "";
			}
			//If current word cannot fit on the line
			//add line, clear line, and add char to current word
			else if( (CharWidth + CurWidth + WordLength) > _puzzle_width )
			{
				CurWidth = 0;
				Strings.push_back(CurString);

				CurString = "";

				CurWord += Char;
				WordLength += CharWidth;			
			}
			else
			{
				WordLength += CharWidth;
				CurWord += Char;

				//Hit a space or end of string, add a word
				if( Char == 32 || i == (length - 1))
				{
					CurWidth += WordLength;
					WordLength = 0;

					CurString += CurWord;
					CurWord = "";

					//Add last string
					if( i == (length - 1) )
					{
						Strings.push_back(CurString);
					}
				}
			}
		}

		
		if(Strings.size() >= HELP_TEXT_LINES)
		{
			HelpTexts[1]->SetText("String is too big.");
		}
		else
		{
			int size = (int)Strings.size();
			if( size == 1 )
			{
				HelpTexts[1]->SetText(Strings[0]);
				HelpTexts[1]->LoadFont(_font);
				//HelpTexts[1]->SetX(HelpTexts[1]->Pos.x + OffsetX);
				HelpTexts[1]->SetAlignment(AlignStyle);
			}
			else
			{
				for(int i=0; i < size; ++i)
				{
					if( size == 2 )
					{
						textPos.y = 688.0f + (30 * i) + (30/2);	
						HelpTexts[i]->SetPos(&textPos);	
					}
					HelpTexts[i]->SetText(Strings[i]);
					HelpTexts[i]->LoadFont(_font);
					//HelpTexts[i]->SetX(HelpTexts[i]->Pos.x + OffsetX);
					HelpTexts[i]->SetAlignment(AlignStyle);
				}
			}
		}
}

void InventoryBox::CrossCheckInventoryItems()
{
	switch(ProfileSystem::Instance->ActiveProfile.CurrentLevel)
	{
		case Level_1:
		CheckAndSetItem(eInv_L1S1_Bag);
		CheckAndSetItem(eInv_L1S1_RubyPart1, eInv_L1S1_RubyPart2, eInv_L1S1_RubyPart3, eInv_L1S1_RubyPart4);
		CheckAndSetItem(eInv_L1S1_SwissKnife);
		CheckAndSetItem(eInv_L1S2_Plank1, eInv_L1S7_Plank2, eInv_L1S1_Plank3);
		CheckAndSetItem(eInv_L1S2_EmptyCan);
		CheckAndSetItem(eInv_L1S2_Hose);
		CheckAndSetItem(eInv_L1S2_TreeBranch);
		CheckAndSetItem(eInv_L1S2_Cloth1Swing, eInv_L1S2_Cloth2LovePop, eInv_L1S2_Cloth3CarDicky);
		CheckAndSetItem(eInv_L1S3_Banana);
		CheckAndSetItem(eInv_L1S3_MatchBox);
		CheckAndSetItem(eInv_L1S3_NagaKey);
		CheckAndSetItem(eInv_L1S3_HOLocket);
		CheckAndSetItem(eInv_L1S3_ShadowHand1, eInv_L1S4_ShadowHand2);
		CheckAndSetItem(eInv_L1S4_KeySet);
		CheckAndSetItem(eInv_L1S4_BeetleHead);
		CheckAndSetItem(eInv_L1S4_BeetleTail);
		CheckAndSetItem(eInv_L1S4_Coin);
		CheckAndSetItem(eInv_L1S4_Diary);
		CheckAndSetItem(eInv_L1S4_PhotoFrame);
		CheckAndSetItem(eInv_L1S4_Coin);
		CheckAndSetItem(eInv_L1S5_BoatKey);
		CheckAndSetItem(eInv_L1S5_Dynamite);
		CheckAndSetItem(eInv_L1S5_HammerHandle);
		CheckAndSetItem(eInv_L1S5_HammerHead);
		CheckAndSetItem(eInv_L1S5_MortarPestle);
		CheckAndSetItem(eInv_L1S6_CanFull);
		CheckAndSetItem(eInv_L1S6_Crowbar);
		CheckAndSetItem(eInv_L1S6_Funnel);
		CheckAndSetItem(eInv_L1S6_OilDipper);
		CheckAndSetItem(eInv_L1CB_Beetle);
		CheckAndSetItem(eInv_L1CB_Hammer);
		CheckAndSetItem(eInv_L1S1_CreekLetter);
		CheckAndSetItem(eInv_L1S4_BookCode);
		break;
		
		case Level_2:
		CheckAndSetItem(eInv_L2S1_Aloevera);
		CheckAndSetItem(eInv_L2S1_HoneyComb);
		CheckAndSetItem(eInv_L2S1_PotWithWater);
		CheckAndSetItem(eInv_L2S1_ColorPowder);
		CheckAndSetItem(eInv_L2S1_RoundStone);
		CheckAndSetItem(eInv_L2S1_Scrapper);
		CheckAndSetItem(eInv_L2S2_Frankinscence);
		CheckAndSetItem(eInv_L2S2_Mangoes);
		CheckAndSetItem(eInv_L2S2_Pearl1, eInv_L2S2_Pearl2, eInv_L2S2_Pearl3, eInv_L2S2_Pearl4);
		CheckAndSetItem(eInv_L2S3_PoojaLamp);
		CheckAndSetItem(eInv_L2S3_Turmeric);
		CheckAndSetItem(eInv_L2S4_AxeBlunt);
		CheckAndSetItem(eInv_L2S4_AxeSharpen);
		CheckAndSetItem(eInv_L2S4_LitTorch);
		CheckAndSetItem(eInv_L2S4_UnlitTorch);
		CheckAndSetItem(eInv_L2S4_UnlitOilTorch);
		CheckAndSetItem(eInv_L2S4_Firewoods);
		CheckAndSetItem(eInv_L2S4_Rake);
		CheckAndSetItem(eInv_L2S4_Lever);
		CheckAndSetItem(eInv_L2S5_SnakeChart);
		CheckAndSetItem(eInv_L2S5_MedicineJar);
		CheckAndSetItem(eInv_L2S5_NagaSculpture);
		CheckAndSetItem(eInv_L2S5_PotEmpty);
		CheckAndSetItem(eInv_L2S5_Ember);
		CheckAndSetItem(eInv_L2S5_StoneSharpener);
		CheckAndSetItem(eInv_L2S5_VinegarBottle);
		CheckAndSetItem(eInv_L2S6_Gloves);
		CheckAndSetItem(eInv_L2S6_SmallStone1, eInv_L2S6_SmallStone2, eInv_L2S6_SmallStone3);
		CheckAndSetItem(eInv_L2S6_KnifeBlunt);
		CheckAndSetItem(eInv_L2S6_KnifeSharpen);
		CheckAndSetItem(eInv_L2S6_PaperPuzClue);
		break;

		case Level_3:
		CheckAndSetItem(eInv_L3S1_Charcoal);
		CheckAndSetItem(eInv_L3S1_FlagFull);
		CheckAndSetItem(eInv_L3S1_Spear);
		CheckAndSetItem(eInv_L3S1_FlagCloth);
		CheckAndSetItem(eInv_L3S1_GreenEye);
		CheckAndSetItem(eInv_L3S1_GreenCrystal);
		CheckAndSetItem(eInv_L3S1_LadderBroken);
		CheckAndSetItem(eInv_L3S1_LadderPiece1, eInv_L3S3_LadderPiece2);
		CheckAndSetItem(eInv_L3S1_LadderFixed);
		CheckAndSetItem(eInv_L3S2_CoconutFull);
		CheckAndSetItem(eInv_L3S2_CoconutShell);
		CheckAndSetItem(eInv_L3S2_CocoShellPowderFilled);
		CheckAndSetItem(eInv_L3S2_CoconutDynamite);
		CheckAndSetItem(eInv_L3S2_Twine);
		CheckAndSetItem(eInv_L3S2_Sickle);
		CheckAndSetItem(eInv_L3S2_SkullTeeth);
		CheckAndSetItem(eInv_L3S2_Hammer);
		CheckAndSetItem(eInv_L3S2_HammerHandle);
		CheckAndSetItem(eInv_L3S2_HammerHead);
		CheckAndSetItem(eInv_L3S2_PipePiece1Room1, eInv_L3S2_PipePiece2Room2, eInv_L3S2_PipePiece3Room3, eInv_L3S2_PipePiece4Room4);
		CheckAndSetItem(eInv_L3S2_PuzzleBlock1Left, eInv_L3S2_PuzzleBlock2Right);
		CheckAndSetItem(eInv_L3S3_Box);
		CheckAndSetItem(eInv_L3S3_SnakeDoorPart);
		CheckAndSetItem(eInv_L3S3_CupEmpty);
		CheckAndSetItem(eInv_L3S3_CupWaterFilled);
		CheckAndSetItem(eInv_L3S3_Saw);
		CheckAndSetItem(eInv_L3S3_Showel);
		CheckAndSetItem(eInv_L3S4_Nail);
		CheckAndSetItem(eInv_L3S4_MetalSheetBlank);
		CheckAndSetItem(eInv_L3S4_MetalSheetMapDrawn);
		CheckAndSetItem(eInv_L3S4_BambooStick);
		CheckAndSetItem(eInv_L3S4_SharpTool);
		CheckAndSetItem(eInv_L3S5_Grass1, eInv_L3S5_Grass2, eInv_L3S5_Grass3, eInv_L3S5_Grass4, eInv_L3S5_Grass5, eInv_L3S5_Grass6);
		CheckAndSetItem(eInv_L3S5_MetalClip);
		CheckAndSetItem(eInv_L3S5_Rope);
		break;

		case Level_4:
		CheckAndSetItem(eInv_L4_S1_Knife);
		CheckAndSetItem(eInv_L4_S1_Bracelet);
		CheckAndSetItem(eInv_L4_S1_KerchiefPiece1);
		CheckAndSetItem(eInv_L4_S1_KerchiefPiece2);
		CheckAndSetItem(eInv_L4_S1_Shoe1);
		CheckAndSetItem(eInv_L4_S1_Shoe2);
		CheckAndSetItem(eInv_L4_S1_BinocularLens);
		CheckAndSetItem(eInv_L4_S1_Leaf);
		CheckAndSetItem(eInv_L4_S1_Stick);
		CheckAndSetItem(eInv_L4_S1_LeafSpoon);
		CheckAndSetItem(eInv_L4_S1_LeafSpoonWithWater);
		CheckAndSetItem(eInv_L4_S1_Medicine);
		CheckAndSetItem(eInv_L4_S1_Crutches);
		CheckAndSetItem(eInv_L4_S2_Bamboo);
		CheckAndSetItem(eInv_L4_S2_BambooStick1, eInv_L4_S2_BambooStick2, eInv_L4_S2_BambooStick3, eInv_L4_S2_BambooStick4);
		CheckAndSetItem(eInv_L4_S2_Twig1, eInv_L4_S4_Twig2, eInv_L4_S5_Twig3);
		CheckAndSetItem(eInv_L4_S2_GrinderHandle);
		CheckAndSetItem(eInv_L4_S2_GrassRope);
		CheckAndSetItem(eInv_L4_S2_GrassBunch);
		CheckAndSetItem(eInv_L4_S2_Strainer);
		CheckAndSetItem(eInv_L4_S3_Pot);
		CheckAndSetItem(eInv_L4_S3_PotWithOlive);
		CheckAndSetItem(eInv_L4_S4_BinocularWithoutLens);
		CheckAndSetItem(eInv_L4_S5_Door);
		CheckAndSetItem(eInv_L4_S5_Olives);
		CheckAndSetItem(eInv_L4_S5_CaveDoorKey);
		CheckAndSetItem(eInv_L4_S5_AlligatorFace);
		CheckAndSetItem(eInv_L4_S5_Net);
		CheckAndSetItem(eInv_L4_S5_GrinderTop);
		CheckAndSetItem(eInv_L4_CB_HandKerchief);
		CheckAndSetItem(eInv_L4_CB_Binocular);
		CheckAndSetItem(eInv_L4_CB_BambooWithKnife);
		break;

		case Level_5:
		CheckAndSetItem(eInv_L5_S1_ArrowHolder);
		CheckAndSetItem(eInv_L5_S1_BowArrowFixed);
		CheckAndSetItem(eInv_L5_S1_Bow);
		CheckAndSetItem(eInv_L5_S2_GreasySapBark);
		CheckAndSetItem(eInv_L5_S2_Hammer);
		CheckAndSetItem(eInv_L5_S2_SoldierBadge);
		CheckAndSetItem(eInv_L5_S3_Hook);
		CheckAndSetItem(eInv_L5_S3_MoonSolution);
		CheckAndSetItem(eInv_L5_S4_PeacockChain);
		CheckAndSetItem(eInv_L5_S4_Pickaxe);
		CheckAndSetItem(eInv_L5_S4_PickaxeBlade);
		CheckAndSetItem(eInv_L5_S4_Woodbark);
		CheckAndSetItem(eInv_L5_S5_BambooLadder);
		CheckAndSetItem(eInv_L5_S5_DivingBag);
		CheckAndSetItem(eInv_L5_S5_Keyhandcuff);
		CheckAndSetItem(eInv_L5_S5_PickaxeHandle);
		CheckAndSetItem(eInv_L5_S6_Locket);
		CheckAndSetItem(eInv_L5_S5_StoneEmblem);
		break;

		case Level_6:
		CheckAndSetItem(eInv_L6_S1_Mortar);
		CheckAndSetItem(eInv_L6_S1_Steel);
		CheckAndSetItem(eInv_L6_S1_MetalContainer);
		CheckAndSetItem(eInv_L6_S1_Emblem);
		CheckAndSetItem(eInv_L6_S1_RagCloth);
		CheckAndSetItem(eInv_L6_S1_Tongs);
		CheckAndSetItem(eInv_L6_S1_Club);
		CheckAndSetItem(eInv_L6_S2_PuzzlePices);
		CheckAndSetItem(eInv_L6_S2_PinegumStick);
		CheckAndSetItem(eInv_L6_S2_StarShapedKeyInIce);
		CheckAndSetItem(eInv_L6_S2_Pestle);
		CheckAndSetItem(eInv_L6_S3_PuzzleCode);
		CheckAndSetItem(eInv_L6_S3_GlowingHeart);
		CheckAndSetItem(eInv_L6_S3_Charcoal);
		CheckAndSetItem(eInv_L6_S3_HorseShoe);
		CheckAndSetItem(eInv_L6_S4_Stick);
		CheckAndSetItem(eInv_L6_S4_PinePitch);
		CheckAndSetItem(eInv_L6_S4_Rope);
		CheckAndSetItem(eInv_L6_S4_FlintStones);
		CheckAndSetItem(eInv_L6_S4_MagnetisedHorseShoe);
		CheckAndSetItem(eInv_L6_S4_StarShapedKey);
		CheckAndSetItem(eInv_L6_S4_Lens);
		CheckAndSetItem(eInv_L6_S4_HeartContainer);
		CheckAndSetItem(eInv_L6_S4_ColouredPowder);
		CheckAndSetItem(eInv_L6_CB_HeartContainerWithCrystal);
		CheckAndSetItem(eInv_L6_CB_RopeWithMagnetisedHorseShoe);
		break;

		case Level_7:
		CheckAndSetItem(eInv_L7_S1_BlueStarEmblem);
		CheckAndSetItem(eInv_L7_S1_PotEmpty);
		CheckAndSetItem(eInv_L7_S1_PotWithMagicWater);
		CheckAndSetItem(eInv_L7_S1_PotWithChain);
		CheckAndSetItem(eInv_L7_S1_RedFlower);
		CheckAndSetItem(eInv_L7_S1_RedStarEmblem);
		CheckAndSetItem(eInv_L7_S1_YellowStarEmblem);
		CheckAndSetItem(eInv_L7_S2_LasyaStatue);
		CheckAndSetItem(eInv_L7_S2_SphereFull);
		CheckAndSetItem(eInv_L7_S2_SphereHalfBase);
		CheckAndSetItem(eInv_L7_S2_SphereHalfToFix);
		CheckAndSetItem(eInv_L7_S2_SymbolDisk);
		CheckAndSetItem(eInv_L7_S3_AxeKey);
		CheckAndSetItem(eInv_L7_S3_Axes);
		CheckAndSetItem(eInv_L7_S3_Bhasmam);
		CheckAndSetItem(eInv_L7_S3_Broom);
		CheckAndSetItem(eInv_L7_S3_Lever);
		CheckAndSetItem(eInv_L7_S3_MetalChain);
		CheckAndSetItem(eInv_L7_S3_MetalWire);
		CheckAndSetItem(eInv_L7_S3_YellowFlowe);
		CheckAndSetItem(eInv_L7_S4_BlueFlower);
		CheckAndSetItem(eInv_L7_S4_DiwaliLamp);
		CheckAndSetItem(eInv_L7_S4_WellGear);
		break;

		case Level_8:
		CheckAndSetItem(eInv_L8_S1_MagicGlass);
		CheckAndSetItem(eInv_L8_S1_MagicWand);
		CheckAndSetItem(eInv_L8_S1_DoorKey);
		CheckAndSetItem(eInv_L8_S1_HalfKey);
		CheckAndSetItem(eInv_L8_S1_IdolGold);
		CheckAndSetItem(eInv_L8_S2_Sugarcane);
		CheckAndSetItem(eInv_L8_S2_HalfKey);
		CheckAndSetItem(eInv_L8_S2_Tool);
		CheckAndSetItem(eInv_L8_S2_SymbolCombination);
		CheckAndSetItem(eInv_L8_S2_SwordHandle);
		CheckAndSetItem(eInv_L8_S2_IdolSilver);
		CheckAndSetItem(eInv_L8_S3_Sword);
		CheckAndSetItem(eInv_L8_S3_CrystalKey);
		CheckAndSetItem(eInv_L8_S3_MaryLeaf);
		CheckAndSetItem(eInv_L8_S3_IdolBronze);
		CheckAndSetItem(eInv_L8_S4_ContainerWithPurifiedLeaf);
		CheckAndSetItem(eInv_L8_CB_FullKey);
		CheckAndSetItem(eInv_L8_CB_MagicWandWithoutKey);
		break;
	}

	//CheckForMissingItemDeadlocks();
	CreateInvBoxItems();

}

int InventoryBox::SearchInventoryItem(int ID)
{
	for(int i = 0; i < MAX_INVBOX_OBJECTS; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId == ID)
			return i;
		if(InvBoxObj[i].HOObj != NULL)
		if(InvBoxObj[i].HOObj->PublicValue == ID)
			return i;
	}
	return -1;
}

int InventoryBox::GetInvBoxLastIndex()
{
	for(int i = 0; i < MAX_INVBOX_OBJECTS; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId == -1)
			return i;
	}
	return 0;
}

inline void InventoryBox::CheckAndSetItem(int ID)
{
	if(ProfileSystem::Instance->ActiveProfile.Inventory[ID])
		{
			if(SearchInventoryItem(ID) == -1)
			{
				int i = GetInvBoxLastIndex();

				ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId = ID;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked = 1;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].MaxPieces = 1;
			}
		}
}

inline void InventoryBox::CheckAndSetItem(int ID, int ID2)
{
	if(ProfileSystem::Instance->ActiveProfile.Inventory[ID] || 
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID2])
	   {
			if(SearchInventoryItem(ID) == -1)
			{
				int i = GetInvBoxLastIndex();

				ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId = ID;
				int ctr = 0;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID2]) ctr++;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked = ctr;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].MaxPieces = 2;
			}
	   }
}

inline void InventoryBox::CheckAndSetItem(int ID, int ID2, int ID3)
{
	if(ProfileSystem::Instance->ActiveProfile.Inventory[ID] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID2] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID3])
	   {
			if(SearchInventoryItem(ID) == -1)
			{
				int i = GetInvBoxLastIndex();

				ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId = ID;
				int ctr = 0;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID2]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID3]) ctr++;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked = ctr;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].MaxPieces = 3;
			}
	   }
}

inline void InventoryBox::CheckAndSetItem(int ID, int ID2, int ID3, int ID4)
{
	if(ProfileSystem::Instance->ActiveProfile.Inventory[ID] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID2] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID3] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID4])
	   {
			if(SearchInventoryItem(ID) == -1)
			{
				int i = GetInvBoxLastIndex();

				ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId = ID;
				int ctr = 0;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID2]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID3]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID4]) ctr++;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked = ctr;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].MaxPieces = 4;
			}
	   }
}

inline void InventoryBox::CheckAndSetItem(int ID, int ID2, int ID3, int ID4, int ID5)
{
	if(ProfileSystem::Instance->ActiveProfile.Inventory[ID] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID2] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID3] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID4] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID5])
	   {
			if(SearchInventoryItem(ID) == -1)
			{
				int i = GetInvBoxLastIndex();

				ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId = ID;
				int ctr = 0;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID2]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID3]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID4]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID5]) ctr++;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked = ctr;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].MaxPieces = 5;
			}
	   }
}

inline void InventoryBox::CheckAndSetItem(int ID, int ID2, int ID3, int ID4, int ID5, int ID6)
{
	if(ProfileSystem::Instance->ActiveProfile.Inventory[ID] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID2] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID3] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID4] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID5] ||
	   ProfileSystem::Instance->ActiveProfile.Inventory[ID6])
	   {
			if(SearchInventoryItem(ID) == -1)
			{
				int i = GetInvBoxLastIndex();

				ProfileSystem::Instance->ActiveProfile.InvObjects[i].InvId = ID;
				int ctr = 0;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID2]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID3]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID4]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID5]) ctr++;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[ID6]) ctr++;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].nPiecePicked = ctr;
				ProfileSystem::Instance->ActiveProfile.InvObjects[i].MaxPieces = 6;
			}
	   }
}

void InventoryBox::CheckForMissingItemDeadlocks()
{
	switch(ProfileSystem::Instance->ActiveProfile.CurrentLevel)
	{
		case Level_1:
			//Beetle after combining
			if(ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken && 
				ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken && 
				ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleTailFixed && 
				!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1CB_Beetle] && 
				!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook)
				{
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1CB_Beetle] = true;
					if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_BeetleHead])
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_BeetleHead] = false;

					if(SearchInventoryItem(eInv_L1S4_BeetleHead) != -1)
						ClearItemFromInvBox(eInv_L1S4_BeetleHead);
					
					if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_BeetleTail])
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_BeetleTail] = false;

					if(SearchInventoryItem(eInv_L1S4_BeetleTail) != -1)
						ClearItemFromInvBox(eInv_L1S4_BeetleTail);
				}

			//Missing matchbox
			if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed && 
				!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_MatchBox] && !ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded)
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
						{
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_MatchBox] = true;							
							ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded = false;
							ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite = false;
						}
				}
			if(ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_MatchBox])
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_MatchBox] = false;
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S3_MatchBox);
			}


			//Missing Fuel Can
			if(ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked && 
				!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_CanFull] && 
				!ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled)
				{
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_CanFull] = true;
				}
			if(ProfileSystem::Instance->ActiveProfile.L1S7_IsDieselFilled && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_CanFull])
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_CanFull] = false;
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S6_CanFull);
			}


			//Missing Coin
			if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_Coin])
			{
				if(ProfileSystem::Instance->ActiveProfile.L1S4_InvCoinTaken && !ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout)
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSecretDoorOpened)
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_Coin] = true;
				}
			}
			if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_Coin] && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_Coin] = false;
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S4_Coin);
			}


			//Missing Hammer
			if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1CB_Hammer])
			{
				if(ProfileSystem::Instance->ActiveProfile.L1S5_IsHammerHandFixed && !ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerPlacedOnScene)
				{
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1CB_Hammer] = true;
					if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_HammerHandle])
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_HammerHandle] = false;
					if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_HammerHead])
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_HammerHead] = false;
				}
			}

		break;

		case Level_2:

			//Missing Lever
			if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_Lever] && !ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed && ProfileSystem::Instance->ActiveProfile.L2_S4_InvMetalLeverTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_Lever] = true;
			}

			if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_Lever] && ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_Lever] = false;
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_Lever);
			}

		break;

		case Level_3:

			//Mug with water
			if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken)
			{
				if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_CupWaterFilled])
				{
					if(ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState == 0)
					{
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_CupWaterFilled] = true;
					}
				}
			}
			if(ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState == 1 && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S1_Charcoal])
				ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState = 2;
			if(ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState >= 1 && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_CupWaterFilled])
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_CupWaterFilled] = false;
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S3_CupWaterFilled);
			}

			//Map sheet
			if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvMapDrawnSheetTaken && 
				!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_MetalSheetMapDrawn] && 
				!ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
				{
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_MetalSheetMapDrawn] = true;
				}

			//Sharp Tool
			if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved && !ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_SharpTool] && ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_SharpTool] = true;
			}
			if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_SharpTool])
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_SharpTool] = false;
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S4_SharpTool);
			}

			//Coconut shell
			if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCocoShellPowderFilledTaken && 
				!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_CoconutShell] && 
				!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_CocoShellPowderFilled] && 
				ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutShellTaken)
				{
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_CoconutShell] = true;
				}
			if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutWithTwineTaken && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_CoconutShell])
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_CoconutShell] = false;
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_CoconutShell);
			}

			//Coconut bomb
			if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutWithTwineTaken && ProfileSystem::Instance->ActiveProfile.L3_S2_CoconutComboTwineFixed)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_Twine] = true;
				ProfileSystem::Instance->ActiveProfile.L3_S2_CoconutComboTwineFixed = false;
			}
			if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutWithTwineTaken && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_Twine])
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_Twine] = false;
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_Twine);
			}

			//Shovel
			if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken && !ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_Showel] && !ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_Showel] = true;
			}
			if(ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_Showel])
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_Showel] = false;
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S3_Showel);
			}

		break;

		case Level_4:

			//Missing Knife & Shoe
			if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken || ProfileSystem::Instance->ActiveProfile.L4_S1_InvKnifeCollected)
			{
				if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife])
				{
					if(!ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo)
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife] = true;
				}
				if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Shoe1])
				{
					if(!ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe)
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Shoe1] = true;
				}
				ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeCompleted = true;
			}

			//Leaf Spoon
			if(!ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved)
			{
				if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonCollected && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken)
				{
					if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_LeafSpoon])
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_LeafSpoon] = true;
				}

				if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken)
				{
					if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_LeafSpoonWithWater])
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_LeafSpoonWithWater] = true;
				}
			}

			//Spear
			if((!ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected || !ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved || !ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken) && !ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] && ProfileSystem::Instance->ActiveProfile.L4_S2_InvBambooSpearTaken)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] = true;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_Olives])
					ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected = true;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_Door] || ProfileSystem::Instance->ActiveProfile.L4_S5_DoorPlaced)
					ProfileSystem::Instance->ActiveProfile.L4_S5_ScrewRemoved = true;
				if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_Net])
					ProfileSystem::Instance->ActiveProfile.L4_S5_InvNetTaken = true;
			}


		break;

		case Level_5:

		break;

		case Level_6:

			//Missing mortar
			if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Mortar])
			{
				if(!ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken && ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken)
				{
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Mortar] = true;
				}
			}

			//Missing horseshoe
			if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_HorseShoe] && ProfileSystem::Instance->ActiveProfile.L6_S3_InvHorseShoeTaken)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized)
				{
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_HorseShoe] = true;
				}
			}

		break;

		case Level_7:

		break;

		case Level_8:

			//Magic wand without key
			if(!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_MagicWandWithoutKey] && (ProfileSystem::Instance->ActiveProfile.L8_S1_MagicWandWithoutKeyTaken || ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyTaken))
			{
				if(ProfileSystem::Instance->ActiveProfile.L8_S2_SugercanePZState <= 0)
				{
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_MagicWandWithoutKey] = true;
				}
			}

		break;
	}
}

bool InventoryBox::isAreaHOorPuzzle(int level, int area)
{
	switch (level)
	{
		case Level_1 :
		switch (area)
		{
			case eArea_L1_S1_PZKidsPainting : return true; break;
			case eArea_L1_S2_PZFuse : return true; break;
			case eArea_L1_S3_PZShelf : return true; break;
			case eArea_L1_S4_PZDoor : return true; break;
			case eArea_L1_S4_PZPainting : return true; break;
			case eArea_L1_S3_DoorHO : return true; break;
			case eArea_L1_S5_PZDynamiteMaking : return true; break;
			case eArea_L1_S5_HOTreasureRoom : return true; break;
			case eArea_L1_S6_HOBoatShed : return true; break;
			default : return false; break;
		}
		break;

		case Level_2 :
		switch (area)
		{
			case eArea_L2_S1_UndergroundHO : return true; break;
			case eArea_L2_S2_PZMango : return true; break;
			case eArea_L2_S2_HONagaTemple : return true; break;
			case eArea_L2_S3_PZStoneBeam : return true; break;
			case eArea_L2_S4_PZDeities : return true; break;
			case eArea_L2_S5_PZEagleUnblock : return true; break;
			case eArea_L2_S5_PZMedicine_Making : return true; break;
			case eArea_L2_S6_HOWaterfall : return true; break;
			case eArea_L2_S6_PZTunnelClock : return true; break;
			case eArea_L2_S7_PZSoldiers : return true; break;
			default : return false; break;
		}
		break;

		case Level_3 :
		switch (area)
		{
			case eArea_L3_S1_HOLadder : return true; break;
			case eArea_L3_S1_PZLionHead : return true; break;
			case eArea_L3_S3_PZBlocks : return true; break;
			case eArea_L3_S3_PZUnderwater : return true; break;
			case eArea_L3_S4_FortFountainHO : return true; break;
			case eArea_L3_S2_FortInterior : return true; break;				
			case eArea_L3_S2_P1Room2Wine : return true; break;
			case eArea_L3_S2_P1Room4Match3 : return true; break;
			case eArea_L3_S2_P1Room5Pipe : return true; break;
			default : return false; break;
		}
		break;

		case Level_4 :
		switch (area)
		{
			case eArea_L4_S1_ShoeHO : return true; break;
			case eArea_L4_S2_PZAlligatorHead : return true; break;
			case eArea_L4_S2_PZDoor : return true; break;
			case eArea_L4_S3_PZFireMaking : return true; break;
			case eArea_L4_S3_PZDoor : return true; break;
			case eArea_L4_S4_BinocularHO : return true; break;
			case eArea_L4_S5_PZSwapTile : return true; break;
			case eArea_L4_S1_SwampEntrance : return true; break;
			default : return false; break;
		}
		break;

		case Level_5 :
		switch (area)
		{
			case eArea_L5_S2_PZBlocks : return true; break;
			case eArea_L5_S3_PZDungeonDoor : return true; break;
			case eArea_L5_S3_HOUnderwater : return true; break;
			case eArea_L5_S4_PZHandCuff : return true; break;
			case eArea_L5_S5_HOGanapathi : return true; break;
			case eArea_L5_S5_PZSunClock : return true; break;
			case eArea_L5_S6_HOTree : return true; break;
			case eArea_L5_S4_Dungeon : return true; break;
			default : return false; break;
		}
		break;

		case Level_6 :
		switch (area)
		{
			case eArea_L6_S1_DescriptionHO : return true; break;
			case eArea_L6_S1_FloatingIsland1 : return true; break;
			case eArea_L6_S2_ShereHO : return true; break;
			case eArea_L6_S2_PZPineGum : return true; break; //Check
			case eArea_L6_S3_GlowingHeartHO : return true; break;
			case eArea_L6_S3_PZPathway : return true; break;
			case eArea_L6_S4_PortalPuzzle : return true; break;
			case eArea_L6_S5_MarysDream : return true; break;
			case eArea_L6_S5_TabMiniGame : return true; break;
			default : return false; break;
		}
		break;

		case Level_7 :
		switch (area)
		{
			case eArea_L7_S1_PZJigsawYellow : return true; break;
			case eArea_L7_S1_PZStarPuzzle : return true; break;
			case eArea_L7_S2_HORightHand : return true; break;
			case eArea_L7_S2_HOLeftHand : return true; break;
			case eArea_L7_S2_HOBrokenRightHand : return true; break;
			case eArea_L7_S2_HOBrokenLeftHand : return true; break;
			case eArea_L7_S2_PZEyePuzzle : return true; break;
			case eArea_L7_S2_PZLasyaStatue : return true; break;
			case eArea_L7_S3_PZDoorPuzzle : return true; break;
			case eArea_L7_S3_PZJigsawRed : return true; break;
			case eArea_L7_S4_PZJigsawBlue : return true; break;
			case eArea_L7_S4_Ameeha : return true; break;
			case eArea_L7_S1_Volcano : return true; break;
			default : return false; break;
		}
		break;

		case Level_8 :
		switch (area)
		{
			case eArea_L8_S1_PZDoorHandPuzzle : return true; break;
			case eArea_L8_S1_DoorHO : return true; break;
			case eArea_L8_S2_PZGears : return true; break;
			case eArea_L8_S2_PZGate : return true; break;
			case eArea_L8_S2_PZSugarCane : return true; break;
			case eArea_L8_S3_BanyanTreeHO : return true; break;
			case eArea_L8_S4_PZPoolGame : return true; break;
			case eArea_L8_S5_VirtualMiniGame : return true; break;
			case eArea_L8_S1_ShukrasPlace : return true; break;
			case eArea_L8_S2_Garden : return true; break;
			case eArea_L8_S3_BanyanTree : return true; break;
			default : return false; break;
		}		
		break;

		default : return false; break;
	}
}

std::string _SceneNames[eArea_Max+1] =
{ 
	//Level 1 - demo
	"Beach",				//eArea_L1_S1_Beach
	"Kid_Painting",			//eArea_L1_S1_PZKidsPainting
	"Mansion_Frontyard",	//eArea_L1_S2_Frontyard
	"Fuse_Puzzle",			//eArea_L1_S2_PZFuse

	"Foyer",				//eArea_L1_S3_Foyer
	"Shelf_Puzzle",			//eArea_L1_S3_PZShelf
	"ShadowPuzzle_Door",	//eArea_L1_S4_PZDoor
	"Bedroom",				//eArea_L1_S4_Bedroom
	"Painting_Puzzle",		//eArea_L1_S4_PZPainting
	"Metal_Wall_HO",		//eArea_L1_S3_DoorHO

	"Treasure_Room",		//eArea_L1_S5_TreasureRoom
	"Tunnel_Door",			//eArea_L1_S5_TunnelDoor
	"Dynamite_Puzzle",		//eArea_L1_S5_PZDynamiteMaking
	"TreasureRoom_HO",		//eArea_L1_S5_HOTreasureRoom
	"Boat_Shed",			//eArea_L1_S6_BoatShed
	"ToolBox_HO",			//eArea_L1_S6_HOBoatShed
	"Boat_Dock",			//eArea_L1_S7_BoatDock

	"End_Of_Demo",			//eArea_L1_EndOfDemo
	"Kids_Room",			//eArea_KidsRoom
	"Extras_Cutscene_Player",//eArea_ExtrasCutscenePlayer
	"Extras_Room",			//eArea_ExtrasRoom
	"Awards_Room",			//eArea_AwardsRoom

	// Level 2 - updated 28-10-16 -from Ryan
	"Narara_Entrance",		//eArea_L2_S1_ForestEntrance,
	"Underground",			//eArea_L2_S1_UndergroundHO,
	"Mango_Puzzle",			//eArea_L2_S2_PZMango,
	"Naga_Temple",          //eArea_L2_S2_NagaTemple,
	"HO_Naga_Temple",		//eArea_L2_S2_HONagaTemple
	"Cave_Entrance",		//eArea_L2_S3_CaveMouth
	"Stone_Beam_Puzzle",	//eArea_L2_S3_PZStoneBeam,
	"Cave_Intersection",	//eArea_L2_S4_Cave,
	"Deities_Puzzle",		//eArea_L2_S4_PZDeities
	"Moopans_Cave",			//eArea_L2_S5_MoopansDen,
	"Eagle_Unblock_Puzzle",	//eArea_L2_S5_EagleUnblock,
	"Medicine_Making_Puzzle",//eArea_L2_S5_PZ_Medicine_Making
	"The_Vault",			//eArea_L2_S6_TunnelEnd,
	"Waterfall_HO",			//eArea_L2_S6_HOWaterfall
	"Tunnel_Clock_Puzzle",	//eArea_L2_S6_PZTunnelClock,
	"Soldiers",				//eArea_L2_S7_PZSoldiers

	//Level 3 - updated 16-11-16 -from Ryan
	"Ruined_Castle",		//eArea_L3_S1_FortEntrance,
	"Ancient_Ruins",		//eArea_L3_S1_HOLadder,
	"Lion_Head_Puzzle",		//eArea_L3_S1_PZLionHead,
	"Castle_Interior",		//eArea_L3_S2_FortInterior,
	"Maze",					//eArea_L3_S2_P1Room1Start,
	"Maze",					//eArea_L3_S2_P1Room2Wine,
	"Maze",					//eArea_L3_S2_P1Room3Lock,
	"Maze",					//eArea_L3_S2_P1Room4Match3,
	"Maze",					//eArea_L3_S2_P1Room5Pipe,
	"Maze",					//eArea_L3_S2_P1Room6Pillar,
	"DeadEnd",				//eArea_L3_S2_P1DeadEnd
	"Pool",					//eArea_L3_S3_Pool,
	"Block_Puzzle",          //eArea_L3_S3_PZBlocks
	"Underwater_Puzzle",	//eArea_L3_S3_PZUnderwater,
	"Fountain",				//eArea_L3_S4_FortFountain,
	"Fort_Fountain_HO",		//eArea_L3_S4_FortFountainHO,
	"Cliff",				//eArea_L3_S5_Cliff

	//Level 4
	"Swamp",		//eArea_L4_S1_SwampEntrance,
	"Shoe_HO",				//eArea_L4_S1_ShoeHO,
	"River_Side",			//eArea_L4_S2_RiverSide,
	"Alligator_Head_Puzzle",//eArea_L4_S2_PZAlligatorHead,
	"Door_Puzzle",	//eArea_L4_S2_PZDoor,
	"Cave",					//eArea_L4_S3_Cave,	
	"Fire_Making_Puzzle",	//eArea_L4_S3_PZFireMaking
	"Cave_Door_Puzzle",	//eArea_L4_S3_PZDoor
	"Hill",				//eArea_L4_S4_HillTop,
	"Binocular_HO",				//eArea_L4_S4_BinocularHO
	"Hill_Side",				//eArea_L4_S5_HillSide		
	"Swap_Tile_Puzzle",	//			eArea_L4_S5_PZSwapTile

	"Forest_Entrance",		//eArea_L5_S1_TigerDistant,
	"Forest_Entrance",		//eArea_L5_S2_TigerCloseup,
	"Blocks_Puzzle",		//eArea_L5_S2_PZBlocks,
	"Pond",		//eArea_L5_S3_Pond,
	"DungeonDoor_Puzzle",		//eArea_L5_S3_PZDungeonDoor,
	"Underwater_HO",		//eArea_L5_S3_HOUnderwater,
	"Dungeon",		//eArea_L5_S4_Dungeon,
	"Handcuff_Puzzle",		//eArea_L5_S4_PZHandCuff,
	"Moon_Light_Cave",		//eArea_L5_S5_MountainBelow,
	"Ganapathi_HO",		//eArea_L5_S5_HOGanapathi,
	"Sun_Clock_Puzzle",	//eArea_L5_S5_PZSunClock,
	"The_Guardian",		//eArea_L5_S6_MountainTop,	
	"Tree_HO",		//eArea_L5_S6_HOTree,	

	"Statue",// eArea_L6_S1_FloatingIsland1:
	"Description_HO",
	"Nether_World",// 	eArea_L6_S2_FloatingIsland2:
	"SphereHO",//eArea_L6_S2_ShereHO
	"Pine_Gum_Puzzle",	//eArea_L6_S2_PZPineGum
	"Secret_Cave",// 	eArea_L6_S3_FloatingIsland3:
	"Fake_James",	//eArea_L6_S3_FakeJames
	"Glowing_Heart_HO",			//eArea_L6_S3_GlowingHeartHO
	"Pathway_Puzzle",					//eArea_L6_S3_PZPathway
	"Portal",// 	eArea_L6_S4_FloatingIsland4:
	"PortalChest",//eArea_L6_S4_PortalChest
	"Portal_Puzzle",	//"eArea_L6_S4_PortalPuzzle
	"MarysDream",// 	eArea_L6_S5_MarysDream:
	"Tab_MiniGame", //eArea_L6_S5_TabMiniGame

	"Volcano",		//eArea_L7_S1_Volcano
	"PZJigsawYellow",		//eArea_L7_S1_PZJigsawYellow
	"PZStarPuzzle",		//eArea_L7_S1_PZStarPuzzle
	"Lasyas_Prison",		//eArea_L7_S2_LasyaPrison
	"HORightHand",		//eArea_L7_S2_HORightHand
	"HOLeftHand",		//eArea_L7_S2_HOLeftHand
	"HOBrokenRightHand",		//eArea_L7_S2_HOBrokenRightHand
	"HOBrokenLeftHand",		//eArea_L7_S2_HOBrokenLeftHand
	"PZEyePuzzle",		//eArea_L7_S2_PZEyePuzzle
	"PZLasyaStatue",		//eArea_L7_S2_PZLasyaStatue
	"Temple",		//eArea_L7_S3_Haven
	"PZDoorPuzzle",		//eArea_L7_S3_PZDoorPuzzle
	"PZJigsawRed",		//eArea_L7_S3_PZJigsawRed
	"Ameeha",		//eArea_L7_S4_Ameeha
	"PZJigsawBlue",		//eArea_L7_S4_PZJigsawBlue
	"Abyss",		//eArea_L7_S5_SophiaEnd

	"Shukras_Palace",//	eArea_L8_S1_ShukrasPlace,
	"Hand_Door_Puzzle",//	eArea_L8_S1_PZ1HandDoorPuzzle,
	"Door_HO", //eArea_L8_S1_DoorHO
	"Garden",//eArea_L8_S2_Garden
	"Gears_Puzzle",		//eArea_L8_S2_PZGears
	"Gate_Puzzle", //eArea_L8_S2_PZGate
	"Sugar_Cane_Puzzle", //eArea_L8_S2_PZSugarCane
	"Tree_of_Life",//eArea_L8_S3_BanyanTree
	"Banyan_Tree_HO", //eArea_L8_S3_BanyanTreeHO
	"Pool",//eArea_L8_S4_Pool
	"Pool_Puzzle", //eArea_L8_S4_PZPoolGame
	"Mohenjedaro",//eArea_L8_S5_VolcanoLand
	"Virtual_Minigame"//eArea_L8_S5_VirtualMiniGame
};

void InventoryBox::GoToScene(int area, int mode, CRectangle rectArea)
{
	NavigateText->Text = StringTable::getStr(_SceneNames[area]);
	//NavigateText->Show(true);
	travelArea = area;
	navTapped = true;

	//mode = 0;

	if(rectArea.w == 0 && rectArea.h == 0)
	{
		tapPosition = *PointSystem::CreateCPoint(683, 595, 0);//Control::Input->Mpos();
	}
	else
	{
		tapPosition = *PointSystem::CreateCPoint(rectArea.GetCenterX(), rectArea.GetCenterY(), 0);
	}

	navRectX1 = tapPosition.x + 100;
	navRectY1 = tapPosition.y + 100;
	navRectX2 = tapPosition.x - 100;
	navRectY2 = tapPosition.y - 100;


	Navigate->SetXY(tapPosition.x, tapPosition.y);
	NavigateText->SetXY(tapPosition.x, tapPosition.y);
	NavArrow->SetXY(tapPosition.x, tapPosition.y);
	Navigate->Show(true);
	NavArrow->Show(true);

    switch( mode )
    {
        case 3  :NavArrow->SetRotate(90); break;
        case 1  :NavArrow->SetRotate(-90); break;
        case 0  :NavArrow->SetRotate(0);break;
        case 2  :NavArrow->SetRotate(180);break;
    }


	int rDescWidth = NavigateText->StringWidth();

						   int HalfTipWidth = 0;		
						   if( rDescWidth <= 85 )
						   {
							   Navigate->PlayAnimation(0);//-51
							   HalfTipWidth = 51;
						   }
						   else if( rDescWidth <= 100 )//9=>+3
						   {
							   Navigate->PlayAnimation(1);//-60
							   HalfTipWidth = 60;
						   }
						   else if( rDescWidth <= 125 )//14=>+4
						   {
							   Navigate->PlayAnimation(2);//-74
							   HalfTipWidth = 74;
						   }
						   else if( rDescWidth <= 146 )//9=>+3
						   {
							   Navigate->PlayAnimation(3);//-83
							   HalfTipWidth = 83;
						   }
						   else if( rDescWidth <= 178 )//14=>+4
						   {
							   Navigate->PlayAnimation(4);//-97
							   HalfTipWidth = 97;
						   }
						   else if( rDescWidth <= 190 )//8=>+3
						   {
							   Navigate->PlayAnimation(5);//-105
							   HalfTipWidth = 105;
						   }
						   else if( rDescWidth <= 211 )//10=>+3
						   {
							   Navigate->PlayAnimation(6);//-115
							   HalfTipWidth = 115;
						   }
						   else if( rDescWidth <= 220 )//10=>+3
						   {
							   Navigate->PlayAnimation(7);//-115
							   HalfTipWidth = 119;
						   }
						   else //~220 and all++ - //4=>+1
						   {
							   Navigate->PlayAnimation(8);//-119
							   HalfTipWidth = 133;
						   }



						   CPoint Pos = tapPosition;

						   /*
							switch( mode )
							{
							#ifndef TOUCH_UI
									case 3  :Pos.x += (-36+HalfTipWidth);Pos.y += -38; break;
									case 1 :Pos.x += (-70+HalfTipWidth);Pos.y += -38; break;
									case 0    :Pos.x += 2;Pos.y += 58; break;
									case 2  :Pos.x -= 4; Pos.y -= 22; break;
							#else
									case 3  :Pos.x += (-36+HalfTipWidth);Pos.y += -3.25*CURSOR_TOP_DISPLACEMENT; break;
									case 1 :Pos.x += (-70+HalfTipWidth);Pos.y += -3.25*CURSOR_TOP_DISPLACEMENT; break;
									case 0    :Pos.x += 2;Pos.y += -3.25*CURSOR_TOP_DISPLACEMENT; break;
									case 2  :Pos.x -= 4; Pos.y += -3.25*CURSOR_TOP_DISPLACEMENT; break;
							#endif
							}
*/
						NavArrow->SetPos(&Pos);
							NavArrow->SetY(Pos.y - 25);
						NavArrow->Show(true);
						   Navigate->SetPos(&Pos);
						   Navigate->SetY(Pos.y + 35);
						   Navigate->Show(true);
						   NavigateText->SetPos(&Pos);
						   NavigateText->SetY(Pos.y + 20);
						   NavigateText->Show(true);
}
