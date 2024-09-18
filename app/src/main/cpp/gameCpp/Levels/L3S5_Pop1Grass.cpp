//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 02 - POPUP 1: Thread
//====================================

#include "L3_S2_FortInterior.h"
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
#include "L3S5_Pop1Grass.h"

const CRectangle  InvDestRect(382.0f,112.0f,589.0f,444.0f);

const CRectangle  grassRect1(714.0f,381.0f,83.0f,190.0f);
const CRectangle  grassRect5(355.0f,397.0f,123.0f,180.0f);
const CRectangle  grassRect3(483.0f,405.0f,60.0f,165.0f);
const CRectangle  grassRect7(567.0f,387.0f,90.0f,185.0f);
const CRectangle  grassRect11(633.0f,110.0f,108.0f,182.0f);
const CRectangle  grassRect10(780.0f,196.0f,94.0f,162.0f);
const CRectangle  grassRect6(814.0f,374.0f,65.0f,187.0f);
const CRectangle  grassRect8(666.0f,385.0f,80.0f,184.0f);
const CRectangle  grassRect2(860.0f,351.0f,79.0f,166.0f);
const CRectangle  grassRect9(934.0f,251.0f,65.0f,120.0f);
const CRectangle  grassRect4(947.0f,408.0f,65.0f,157.0f);

const CRectangle  PopupMaskRect(357.0f,86.0f,651.0f,494.0f);


L3S5_Pop1Grass::L3S5_Pop1Grass(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S5\\POP1\\P1.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken1)
		GetObjectByID("drygrass01")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken2)
		GetObjectByID("drygrass02")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken3)
		GetObjectByID("drygrass03")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken4)
		GetObjectByID("drygrass04")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken5)
		GetObjectByID("drygrass05")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken6)
		GetObjectByID("drygrass06")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken7)
		GetObjectByID("drygrass07")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken8)
		GetObjectByID("drygrass08")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken9)
		GetObjectByID("drygrass09")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken10)
		GetObjectByID("drygrass10")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken11)
		GetObjectByID("drygrass11")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken1)
		GetObjectByID("grass01")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken2)
		GetObjectByID("grass02")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken3)
		GetObjectByID("grass03")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken4)
		GetObjectByID("grass04")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken5)
		GetObjectByID("grass05")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken6)
		GetObjectByID("grass06")->SetEnable(false);

	GetObjectByID("drygrass03")->SetMaskRect(PopupMaskRect);
	GetObjectByID("drygrass04")->SetMaskRect(PopupMaskRect);
	GetObjectByID("drygrass05")->SetMaskRect(PopupMaskRect);
	GetObjectByID("drygrass06")->SetMaskRect(PopupMaskRect);
	GetObjectByID("drygrass07")->SetMaskRect(PopupMaskRect);
	GetObjectByID("drygrass08")->SetMaskRect(PopupMaskRect);
	

	JAnim_grass1 = new JSONAnimator(1, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass1->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS1.JSON", this);

	JAnim_grass2 = new JSONAnimator(2, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass2->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS2.JSON", this);

	JAnim_grass3 = new JSONAnimator(3, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass3->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS3.JSON", this);

	JAnim_grass4 = new JSONAnimator(4, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass4->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS4.JSON", this);

	JAnim_grass5 = new JSONAnimator(5, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass5->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS5.JSON", this);

	JAnim_grass6 = new JSONAnimator(6, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass6->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS6.JSON", this);

	JAnim_grass7 = new JSONAnimator(7, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass7->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS7.JSON", this);
	
	JAnim_grass8 = new JSONAnimator(8, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass8->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS8.JSON", this);

	JAnim_grass9 = new JSONAnimator(9, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass9->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS9.JSON", this);

	JAnim_grass10 = new JSONAnimator(10, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass10->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS10.JSON", this);

	JAnim_grass11 = new JSONAnimator(11, CC_CALLBACK_1(L3S5_Pop1Grass::OnAnimationComplete, this));
	JAnim_grass11->parseAndLoadJSON("ANIM\\L3\\S5P1DRYGRASS11.JSON", this);


	Show(false);

}

void L3S5_Pop1Grass::OnAnimationComplete(int animID)
{

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken1 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken2 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken3 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken4 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken5 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken6)
		Hud->ShowBannerText("L3green");
	
	if(animID == 1)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken1 = false;
	}
	else if(animID == 2)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken2 = false;
	}
	else if(animID == 3)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken3 = false;
	}
	else if(animID == 4)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken4 = false;
	}
	else if(animID == 5)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken5 = false;
	}
	else if(animID == 6)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken6 = false;
	}
	else if(animID == 7)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken7 = false;
	}
	else if(animID == 8)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken8 = false;
	}
	else if(animID == 9)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken9 = false;
	}
	else if(animID == 10)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken10 = false;
	}
	else if(animID == 11)
	{
		//LampLite
		ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken11 = false;
	}
	
}

void L3S5_Pop1Grass::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

		GFInstance->WrongInvDropMsg(Hud);
}

void L3S5_Pop1Grass::Required()
{

}



void L3S5_Pop1Grass::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

		
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();

	if(grassRect11.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken11)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass11")->SetPivot(GetObjectByID("drygrass11")->Pos.x , GetObjectByID("drygrass11")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass11")->Pos.x,GetObjectByID("drygrass11")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass11")->TweenTo(tweenArray,1,0,false);*/

			JAnim_grass11->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken11 = true;
		}
	}
	else if(grassRect10.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken10)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass10")->SetPivot(GetObjectByID("drygrass10")->Pos.x , GetObjectByID("drygrass10")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass10")->Pos.x,GetObjectByID("drygrass10")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass10")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass10->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken10 = true;
		}
	}
	else if(grassRect9.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken9)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass09")->SetPivot(GetObjectByID("drygrass09")->Pos.x , GetObjectByID("drygrass09")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass09")->Pos.x,GetObjectByID("drygrass09")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass09")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass9->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken9 = true;
		}
	}
	else if(grassRect8.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken8)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass08")->SetPivot(GetObjectByID("drygrass08")->Pos.x , GetObjectByID("drygrass08")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass08")->Pos.x,GetObjectByID("drygrass08")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass08")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass8->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken8 = true;
		}
	}
	else if(grassRect7.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken7)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass07")->SetPivot(GetObjectByID("drygrass07")->Pos.x , GetObjectByID("drygrass07")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass07")->Pos.x,GetObjectByID("drygrass07")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass07")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass7->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken7 = true;
		}
	}
	else if(grassRect6.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken6)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass06")->SetPivot(GetObjectByID("drygrass06")->Pos.x , GetObjectByID("drygrass06")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass06")->Pos.x,GetObjectByID("drygrass06")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass06")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass6->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken6 = true;
		}
	}
	else if(grassRect5.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken5)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass05")->SetPivot(GetObjectByID("drygrass05")->Pos.x , GetObjectByID("drygrass05")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass05")->Pos.x,GetObjectByID("drygrass05")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass05")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass5->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken5 = true;
		}
	}
	else if(grassRect4.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken4)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass04")->SetPivot(GetObjectByID("drygrass04")->Pos.x , GetObjectByID("drygrass04")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass04")->Pos.x,GetObjectByID("drygrass04")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass04")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass4->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken4 = true;
		}
	}
	else if(grassRect3.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken3)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass03")->SetPivot(GetObjectByID("drygrass03")->Pos.x , GetObjectByID("drygrass03")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass03")->Pos.x,GetObjectByID("drygrass03")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass03")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass3->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken3 = true;
		}
	}
	else if(grassRect2.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken2)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass02")->SetPivot(GetObjectByID("drygrass02")->Pos.x , GetObjectByID("drygrass02")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass02")->Pos.x,GetObjectByID("drygrass02")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass02")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass2->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken2 = true;
		}
	}
	else if(grassRect1.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken1)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			/*GetObjectByID("drygrass01")->SetPivot(GetObjectByID("drygrass01")->Pos.x , GetObjectByID("drygrass01")->Pos.y );
			float tweenArray[5] = {0,GetObjectByID("drygrass01")->Pos.x,GetObjectByID("drygrass01")->Pos.y ,1,30.0f};
			GetObjectByID("drygrass01")->TweenTo(tweenArray,1,0,false);*/
			JAnim_grass1->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s5p1_drygrassmove");
			ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken1 = true;
		}
	}
	else if(grassRect1.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken1 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken1)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("grass01")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L3S5_Grass1, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken1 = true;
			CheckForAllGrassTaken();
		}
	}
	else if(grassRect2.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken2 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken2)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("grass02")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L3S5_Grass2, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken2 = true;
			CheckForAllGrassTaken();
		}
	}
	else if(grassRect7.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken7 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken3)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("grass03")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L3S5_Grass3, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken3 = true;
			CheckForAllGrassTaken();
		}
	}
	else if(grassRect5.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken5 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken4)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("grass04")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L3S5_Grass4, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken4 = true;
			CheckForAllGrassTaken();
		}
	}
	else if(grassRect11.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken11 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken5)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("grass05")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L3S5_Grass5, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken5 = true;
			CheckForAllGrassTaken();
		}
	}
	else if(grassRect10.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L3_S5_DryGrassTaken10 && !ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken6)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("grass06")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L3S5_Grass6, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken6 = true;
			CheckForAllGrassTaken();
		}
	}
}

void L3S5_Pop1Grass::CheckForAllGrassTaken()
{
	if(ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken1 && ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken2 && ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken3 && ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken4 && ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken5 && ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken6)
	{
		ProfileSystem::Instance->ActiveProfile.L3_S5_InvAllGrassTaken = true;
		ClosePopup(1.0f);
	}
}

float L3S5_Pop1Grass::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S5_Pop1Grass::~L3S5_Pop1Grass()
{
	SAFE_DELETE(JAnim_grass1);
	SAFE_DELETE(JAnim_grass2);
	SAFE_DELETE(JAnim_grass3);
	SAFE_DELETE(JAnim_grass4);
	SAFE_DELETE(JAnim_grass5);
	SAFE_DELETE(JAnim_grass6);
	SAFE_DELETE(JAnim_grass7);
	SAFE_DELETE(JAnim_grass8);
	SAFE_DELETE(JAnim_grass9);
	SAFE_DELETE(JAnim_grass10);
	SAFE_DELETE(JAnim_grass11);
}

void L3S5_Pop1Grass::ReceiveMessage(int val)
{	

	
}

bool L3S5_Pop1Grass::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvAllGrassTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L3S5_Pop1Grass::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken1)
		Hud->InitHintTrailEmit(grassRect1.GetCenterX(), grassRect1.GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken2)
		Hud->InitHintTrailEmit(grassRect2.GetCenterX(), grassRect2.GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken3)
		Hud->InitHintTrailEmit(grassRect7.GetCenterX(), grassRect7.GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken4)
		Hud->InitHintTrailEmit(grassRect5.GetCenterX(), grassRect5.GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken5)
		Hud->InitHintTrailEmit(grassRect11.GetCenterX(), grassRect11.GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S5_InvGrassTaken6)
		Hud->InitHintTrailEmit(grassRect10.GetCenterX(), grassRect10.GetCenterY());
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
