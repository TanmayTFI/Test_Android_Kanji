#include "L3_S2_M2PipePopUp.h"
#include "Util.h"

const CRectangle  PipeArea(586.0f,214.0f,248.0f,212.0f);

L3_S2_M2PipePopUp::L3_S2_M2PipePopUp(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback):
	Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	Load("LV3\\S2\\MAZE2\\POP1\\POP1.SCN");
	SetFrameAndCloseButton();
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(GetObjectByID("popupbg"));

	_pipe = GetObjectByID("Pipe");

	for (int i = 1; i <= 4; i++)
	{
		CObject *mud = GetObjectByID("MudOnPipe_0"+MKSTR(i));
		_mudOnPipe.push_back(mud);
	}

	_noOfClicks = 0;
	for (int i = 0; i < 2; i++)
	{
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_R2RemovedMudOnPipe[i])
		{
			_noOfClicks++;
			_mudOnPipe[i + i%2]->SetEnable(false);
			_mudOnPipe[i + i%2 + 1]->SetEnable(false);
		}
	}
}


L3_S2_M2PipePopUp::~L3_S2_M2PipePopUp(void)
{

}

void L3_S2_M2PipePopUp::Update(float ds)
{
	Popup::Update(ds);

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(GetPopupState()!= ePopupState_Open )
		return;

	CPoint mPos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken &&
		PipeArea.Intersection(&mPos))
	{
		if (_noOfClicks < 2)
		{
			Cursor::SetMode(CA_HandPointy);

			if (Control::Input->LBclicked())
			{
				_noOfClicks++;
				ProfileSystem::Instance->ActiveProfile.L3_S2_R2RemovedMudOnPipe[_noOfClicks-1] = true;

				if (_noOfClicks%2)
				{
					float valArrLeft[5] = {0, _mudOnPipe[0]->Pos.x - 40, _mudOnPipe[0]->Pos.y, -1, -1};
					float valArrRight[5] = {0, _mudOnPipe[1]->Pos.x + 40, _mudOnPipe[1]->Pos.y, -1, -1};

					_mudOnPipe[0]->TweenTo(valArrLeft, 1.0f, 0, true);
					_mudOnPipe[1]->TweenTo(valArrRight, 1.0f, 0, true);

					Control::Audio->QuickLoadAndPlaySFX("s2m2p1_soilmoved1");
				}
				else
				{
					float valArrUp[5] = {0, _mudOnPipe[2]->Pos.x, _mudOnPipe[2]->Pos.y - 40, -1, -1};
					float valArrDown[5] = {0, _mudOnPipe[3]->Pos.x, _mudOnPipe[3]->Pos.y - 40, -1, -1};

					_mudOnPipe[2]->TweenTo(valArrUp, 1.0f, 0, true);
					_mudOnPipe[3]->TweenTo(valArrDown, 1.0f, 0, true);

					Control::Audio->QuickLoadAndPlaySFX("s2m2p1_soilmoved2");
				}
			}
		}
		else
		{
			Cursor::SetMode(CA_HandTake);

			if (Control::Input->LBclicked())
			{
				_pipe->FadeOut(1.5f);
				Hud->TakeInventoryItem(eInv_L3S2_PipePiece2Room2, _pipe->Pos);
				ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken = true;
			}
		}
	}
}

void L3_S2_M2PipePopUp::OnPopupOpened()
{
	Popup::OnPopupOpened();
	Hud->ShowBannerText(Banner_L3_S2_M2_ClickOnRubble);
}

void L3_S2_M2PipePopUp::ProcessHint()
{
	Hud->InitHintTrailEmit(&PipeArea);
}


