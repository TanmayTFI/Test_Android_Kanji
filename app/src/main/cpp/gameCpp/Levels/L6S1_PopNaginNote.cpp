//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 01 - POPUP: 
//====================================

#include "L6S1_PopNaginNote.h"

const CRectangle  LetterCloseBtnRect(841.0f,15.0f,110.0f,106.0f);

L6S1_PopNaginNote::L6S1_PopNaginNote(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV6\\S1\\HO\\POP\\POP.SCN");
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	naginTalkAnim = GetObjectByID("NaginDialogue");
	naginTalkAnim->SetEnable(false);

	GetObjectByID("letterwotext")->SetEnable(false);

	SetSparkleUpdate(false);
	Show(false);
}


void L6S1_PopNaginNote::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(naginTalkAnim->IsActive && !Control::Audio->IsPlaying(aVOL6NoteVOS4_2))
	{
		naginTalkAnim->SetEnable(false);
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
			ClosePopup();
		}
	#endif
	//GetObjectByID("closebtn")->PlayAnimation(0);
	//if (LetterCloseBtnRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		//GetObjectByID("closebtn")->PlayAnimation(1);
		if (Control::Input->LBclicked())
		{
			//GetObjectByID("closebtn")->PlayAnimation(0);
			ClosePopup();
		}
	}
}

void L6S1_PopNaginNote::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/* switch (itemVal)
	{
	
	default:
	*/
		GFInstance->WrongInvDropMsg(Hud);
		/*break;
	}*/
}

void L6S1_PopNaginNote::OnAnimComplete(int animID)
{

}

float L6S1_PopNaginNote::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S1_PopNaginNote::~L6S1_PopNaginNote()
{
}

void L6S1_PopNaginNote::Required()
{

}

void L6S1_PopNaginNote::OnPopupOpened()
{
	Popup::OnPopupOpened();

	naginTalkAnim->SetEnable(true);
	naginTalkAnim->FadeIn(2.0f);

	GetObjectByID("letterwotext")->SetEnable(true);
	GetObjectByID("letterwotext")->FadeIn(2.0f);

	CPoint endPos(220, 654, 0);
	SetZoomOrigin(endPos);
}

void L6S1_PopNaginNote::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L6S1_PopNaginNote::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	/*if (true)
	{
		isAnyTaskLeft = true;
	}*/

	return isAnyTaskLeft;
}

void L6S1_PopNaginNote::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	//if(true)
	//{
	//	//
	//}
	//else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
