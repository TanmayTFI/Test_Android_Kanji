//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 06 - POPUP 4: HOLDER
//====================================

#include "L2S6_Pop4Holder.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "InteractiveObjectList.h"
#include "exMath.h"

#define RESTING_STATE 100
#define MOVING_STATE 200

#define STRIP1 0
#define STRIP2 1
#define STRIP3 2
#define NO_STRIP 4

const CRectangle  InvItemRectPuzClue(632.0f,317.0f,146.0f,117.0f);

L2S6_Pop4Holder::L2S6_Pop4Holder(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)

{

	//Popup load
	Load("LV2\\S6\\POP4\\P4.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	FirstDigit = ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[0];
	SecondDigit = ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[1];
	ThirdDigit = ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[2];

	const CPoint *headPos[3] = {GetObjectByID("0_f")->GetPosPointer(), GetObjectByID("0_s")->GetPosPointer(), GetObjectByID("0_t")->GetPosPointer()};
	for (int i = 0; i < 3; i++)
	{
		headPosX[i] = (int)(headPos[i]->x);
		headPosY[i] = (int)(headPos[i]->y);
		headPosZ[i] = (int)(headPos[i]->z);
	}

	CObject *Number[3];

	for (int i = 0; i < 10; i++)
	{
		std::string idStr[3] = {"_f", "_s", "_t"};
		for (int j = 0; j < 3; j++)
		{
			idStr[j] = std::to_string(i) + idStr[j];
			Number[j] = GetObjectByID(idStr[j]);
			CPoint *point1 = new CPoint((float)(headPosX[j] + 22), (float)(headPosY[j] + 22), (float)(headPosZ[j]));
			Number[j]->SetPos(point1);
			delete(point1);

			if (i == ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[j])
			{
				CPoint *point2 = new CPoint((float)(headPosX[j]), (float)(headPosY[j]), (float)(headPosZ[j]));
				Number[j]->SetPos(point2);
				delete(point2);
			}
		}
	}

	currentState = RESTING_STATE;
	tappedStrip = NO_STRIP;
	counter = 0;

	if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen)
	{
		GetObjectByID("HolderCap")->SetEnable(false);
		GetObjectByID("WhiteArea")->SetEnable(false);
		for (int i = 0; i < 10; i++)
		{
			std::string idStr[3] = {"_f", "_s", "_t"};
			for (int j = 0; j < 3; j++)
			{
				idStr[j] = std::to_string(i) + idStr[j];
				Number[j] = GetObjectByID(idStr[j]);
				Number[j]->SetEnable(false);
			}
		}
	}
	else
	{
		GetObjectByID("LidOpen")->SetEnable(false);
	}

	GetObjectByID("GreenGlow01")->SetEnable(false);
	GetObjectByID("GreeGlow02")->SetEnable(false);
	GetObjectByID("GreeGlow03")->SetEnable(false);
}

void L2S6_Pop4Holder::Required()
{

}

void L2S6_Pop4Holder::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (counter && counter < 100)
		counter++;

	if (counter == 10)
	{
		GetObjectByID("GreeGlow02")->SetEnable(true);
	}
	else if (counter == 20)
	{
		GetObjectByID("GreeGlow03")->SetEnable(true);
	}
	else  if (counter == 30)
	{
		CObject *cap = GetObjectByID("HolderCap");
		float valArray1[5] = {0, cap->Pos.x, cap->Pos.y, 1, 0};
		cap->TweenTo(valArray1, 1.0f, 0, false);
		CObject *WhiteArea = GetObjectByID("WhiteArea");
		float valArray2[5] = {0, WhiteArea->Pos.x, WhiteArea->Pos.y, 1, 0};
		WhiteArea->TweenTo(valArray2, 1.0f, 0, false);
		CObject *_f = GetObjectByID(std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[0])+ "_f");
		float valArray3[5] = {0, _f->Pos.x, _f->Pos.y, 1, 0};
		_f->TweenTo(valArray3, 1.0f, 0, false);
		CObject *_s = GetObjectByID(std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[1])+ "_s");
		float valArray4[5] = {0, _s->Pos.x, _s->Pos.y, 1, 0};
		_s->TweenTo(valArray4, 1.0f, 0, false);
		CObject *_t = GetObjectByID(std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[2])+ "_t");
		float valArray5[5] = {0, _t->Pos.x, _t->Pos.y, 1, 0};
		_t->TweenTo(valArray5, 1.0f, 0, false);
		CObject *GreeGlow01 = GetObjectByID("GreenGlow01");
		float valArray6[5] = {0, GreeGlow01->Pos.x, GreeGlow01->Pos.y, 1, 0};
		GreeGlow01->TweenTo(valArray6, 1.0f, 0, false);
		CObject *GreeGlow02 = GetObjectByID("GreeGlow02");
		float valArray7[5] = {0, GreeGlow02->Pos.x, GreeGlow02->Pos.y, 1, 0};
		GreeGlow02->TweenTo(valArray7, 1.0f, 0, false);
		CObject *GreeGlow03 = GetObjectByID("GreeGlow03");
		float valArray8[5] = {0, GreeGlow03->Pos.x, GreeGlow03->Pos.y, 1, 0};
		GreeGlow03->TweenTo(valArray8, 1.0f, 0, false);
		CObject *LidOpen = GetObjectByID("LidOpen");
		LidOpen->SetAlpha(0);
		LidOpen->SetEnable(true);
		float valArray9[5] = {1, LidOpen->Pos.x, LidOpen->Pos.y, 1, 0};
		LidOpen->TweenTo(valArray9, 1.0f, 0, false);
		CObject *Number[3];
		for (int i = 0; i < 10; i++)
		{
			std::string idStr[3] = {"_f", "_s", "_t"};
			for (int j = 0; j < 3; j++)
			{
				idStr[j] = std::to_string(i) + idStr[j];
				Number[j] = GetObjectByID(idStr[j]);
				Number[j]->SetEnable(false);
			}
		}

		ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen = true;

		counter = 0;
	}

	if (GetObjectByID("WhiteArea")->GetWorldRect().Intersection(&pos) && currentState == RESTING_STATE && !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen)
	{
		Cursor::SetMode(CA_HandPointy);

		if( Control::Input->LBclicked() )
		{
			std::string objID[3] = {std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[0])+ "_f",
				std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[1])+ "_s",
				std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[2])+ "_t"};

			for (int i = 0; i < 3; i++)
			{
				CObject *touchedNum = GetObjectByID(objID[i]);
				if (touchedNum->GetWorldRect().Intersection(&pos))
				{
					currentState = MOVING_STATE;
					tappedStrip = i;		
					Control::Audio->QuickLoadAndPlaySFX("s6p4_numberslide");
				}
			}
			if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
			{
				Hud->ShowBannerText("L2code");
			}
			else 
			{
				/*Hud->ShowBannerText("L2think");*/
			}
		}
		return;
	}

	if (currentState == MOVING_STATE)
	{
		int nextDigit;
		if (ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[tappedStrip] == 9)
			nextDigit = 0;
		else 
			nextDigit = ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[tappedStrip] + 1;

		std::string currObjID, nextObjID, str;

		switch (tappedStrip)
		{
		case 0:
			str = "_f";
			break;
		case 1:
			str = "_s";
			break;
		case 2:
			str = "_t";
			break;
		default:
			break;
		}

		currObjID = std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[tappedStrip])+ str;
		CObject *currNumber = GetObjectByID(currObjID);
		nextObjID = std::to_string(nextDigit)+ str;
		CObject *nextNum = GetObjectByID(nextObjID);

		float diff = 0.5f;
		CPoint *nextPos1 = new CPoint((currNumber->GetPosPointer()->x)- diff, (currNumber->GetPosPointer()->y)- diff, (currNumber->GetPosPointer()->z));
		CPoint *nextPos2 = new CPoint((nextNum->GetPosPointer()->x)- diff, (nextNum->GetPosPointer()->y)- diff, (nextNum->GetPosPointer()->z));
		currNumber->SetPos(nextPos1);
		nextNum->SetPos(nextPos2);
		delete(nextPos1);
		delete(nextPos2);

		if (nextNum->GetPosPointer()->x < headPosX[tappedStrip])
		{
			CPoint *currPos = new CPoint((float)(headPosX[tappedStrip] + 22), (float)(headPosY[tappedStrip] + 22), (float)(headPosZ[tappedStrip]));
			currNumber->SetPos(currPos);
			delete(currPos);
			ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[tappedStrip] = nextDigit;

			currentState = RESTING_STATE;
			tappedStrip = NO_STRIP;

			if (ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[0] == 1 && ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[1] == 3 && ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[2] == 7)
			{
				counter = 1;
				GetObjectByID("GreenGlow01")->SetEnable(true);
				Control::Audio->QuickLoadAndPlaySFX("s6p4_boxopen");
				return;
			}
		}
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken )
	{
		if( InvItemRectPuzClue.Intersection( &pos) )
		{
			if( ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen )
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					GetObjectByID("paper")->SetEnable(false);
					ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken = true;
					Hud->TakeInventoryItem(eInv_L2S6_PaperPuzClue);
					ClosePopup();
					return;
				}
			}
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen && Control::Input->LBclicked())
	{
		Hud->ShowBannerText("L2think");
	}
}

L2S6_Pop4Holder::~L2S6_Pop4Holder()
{

}

bool L2S6_Pop4Holder::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
		isAnyTaskLeft = true;
	else if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S6_Pop4Holder::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
	{
		Hud->InitHintTrailEmit(GetObjectByID("HolderCap")->GetCenterX(), GetObjectByID("HolderCap")->GetCenterY());
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen)
	{
		Hud->InitHintTrailEmit(GetObjectByID("paper")->GetCenterX(), GetObjectByID("paper")->GetCenterY());
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
