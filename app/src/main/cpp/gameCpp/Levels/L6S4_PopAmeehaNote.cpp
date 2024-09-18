//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 04 - POPUP: 
//====================================

#include "L6S4_PopAmeehaNote.h"

const CRectangle  LetterCloseBtnRect(841.0f,15.0f,110.0f,106.0f);

L6S4_PopAmeehaNote::L6S4_PopAmeehaNote(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV6\\S4\\POP\\POP.SCN");
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	ameehaTalkAnim = GetObjectByID("ameehatalk");
	ameehaTalkAnim->SetEnable(false);

	GetObjectByID("letterwotext")->SetEnable(false);

	SetSparkleUpdate(false);
	Show(false);
}


void L6S4_PopAmeehaNote::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(ameehaTalkAnim->IsActive && !Control::Audio->IsPlaying(aVOL6NoteVOS4_1))
	{
		ameehaTalkAnim->SetEnable(false);
	}
/*  Need to check if this is needed, as Popup Update has it in it.
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
			ClosePopup();
		}
	#endif
*/
	GetObjectByID("closebtn")->PlayAnimation(0);
	if (LetterCloseBtnRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		GetObjectByID("closebtn")->PlayAnimation(1);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("closebtn")->PlayAnimation(0);
			ClosePopup();
		}
	}
}

void L6S4_PopAmeehaNote::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/* switch (itemVal)
	{
	
	default:
	*/
		GFInstance->WrongInvDropMsg(Hud);
		/*break;
	}*/
}

void L6S4_PopAmeehaNote::OnAnimComplete(int animID)
{

}

float L6S4_PopAmeehaNote::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S4_PopAmeehaNote::~L6S4_PopAmeehaNote()
{
}

void L6S4_PopAmeehaNote::Required()
{

}

void L6S4_PopAmeehaNote::OnPopupOpened()
{
	ameehaTalkAnim->SetEnable(true);
	ameehaTalkAnim->FadeIn(2.0f);

	GetObjectByID("letterwotext")->SetEnable(true);
	GetObjectByID("letterwotext")->FadeIn(2.0f);

	

	CPoint endPos(220, 654, 0);
	SetZoomOrigin(endPos);
}

void L6S4_PopAmeehaNote::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L6S4_PopAmeehaNote::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	/*if (true)
	{
		isAnyTaskLeft = true;
	}*/

	return isAnyTaskLeft;
}

void L6S4_PopAmeehaNote::ProcessHint()
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
