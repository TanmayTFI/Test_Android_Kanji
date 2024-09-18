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
#include "L3S2_Pop1Thread.h"

const CRectangle  InvDestRect(382.0f,112.0f,589.0f,444.0f);

L3S2_Pop1Thread::L3S2_Pop1Thread(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV3\\S2\\POP1\\P1.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	isAnimationPlaying = false;

	int ArrayPts5[] = {631,365,647,355,666,399,649,424,585,437,553,447,521,474,460,494,425,471,399,434,399,386,440,348,496,343,530,363,566,363,591,318,632,288,704,298,734,342,728,386,706,421,655,452,625,482,648,504,697,516,744,494,763,458,753,419,748,377,761,331,772,306,755,262,713,240,684,263,670,299,649,316,616,316,585,274,591,228,636,213,677,207,711,176,726,159,777,146,821,160,854,190,872,225,874,259,875,284,853,325,839,349,846,371,868,378,890,377,911,350,917,321,901,293,883,278,883,257,919,273,943,312,936,360,908,395,850,403,820,384,810,337,850,291,854,254,830,199,797,178,745,174,722,201,695,223,656,237,623,243,608,260,615,283,652,278,669,250,695,225,717,219,770,248,787,281,794,308,786,338,767,371,772,399,786,433,787,474,771,505,740,528,703,540,653,532,624,515,609,492,608,459,620,445,657,428,676,421,699,396,707,375,711,338,688,311,664,312,628,321,610,334,605,356,564,386,518,384,486,368,445,374,427,393,421,426,436,446,451,455,474,464,505,452,523,435,543,424,569,418,592,417,611,414,627,412,637,399};
	Thread5Poly=  new CPolygon(124);
	Thread5Poly->Init(ArrayPts5);

	int ArrayPts4[] = {659,119,679,141,642,175,571,178,544,194,556,237,545,296,549,329,583,327,614,342,626,375,618,413,609,454,641,492,686,500,718,481,753,472,812,469,840,428,845,388,797,375,747,362,735,327,758,273,758,227,723,190,690,190,665,213,664,253,681,282,713,301,720,336,711,368,690,408,678,436,694,474,679,480,655,443,666,398,687,358,696,337,671,305,641,270,640,228,650,187,679,170,718,160,753,180,782,222,785,258,778,297,762,324,765,347,812,353,846,358,870,381,871,422,861,457,831,490,769,496,735,501,708,515,652,520,609,502,589,460,593,409,609,379,586,355,548,355,519,325,521,288,531,235,519,193,535,156,577,149,620,150};
	Thread4Poly=  new CPolygon(76);
	Thread4Poly->Init(ArrayPts4);

	int ArrayPts3[] = {839,467,867,455,850,439,854,415,885,401,910,410,928,441,919,480,901,498,869,500,839,489,816,436,789,398,752,382,690,383,665,407,625,428,572,405,542,357,547,313,579,274,608,221,603,174,575,130,520,124,483,156,456,163,446,124,422,129,431,173,447,195,488,186,524,157,557,158,566,183,574,207,571,239,545,267,516,297,515,344,529,397,547,438,581,443,632,454,683,442,690,417,712,411,748,412,773,426,790,443,805,495,839,520,882,528,938,508,952,454,939,406,910,379,861,376,833,400};
	Thread3Poly=  new CPolygon(59);
	Thread3Poly->Init(ArrayPts3);

	int ArrayPts2[] = {790,322,779,307,829,275,884,293,914,338,908,388,869,417,810,415,765,387,730,389,702,432,658,457,587,434,568,377,587,329,620,302,653,291,671,272,650,246,594,231,548,255,532,284,532,313,505,319,511,274,530,234,578,207,650,219,691,253,698,287,678,308,635,328,608,347,595,381,612,415,651,426,690,406,706,374,738,359,771,367,797,378,831,390,863,390,889,354,870,319,839,306};
	Thread2Poly=  new CPolygon(46);
	Thread2Poly->Init(ArrayPts2);

	int ArrayPts1[] = {536,504,558,515,594,469,661,459,700,423,711,374,707,333,668,316,610,322,591,302,602,271,635,247,701,257,735,313,786,354,853,364,898,340,911,290,895,284,876,327,843,343,795,336,759,304,718,245,657,225,612,231,584,250,572,288,573,330,589,340,650,339,684,342,687,371,677,413,652,427,598,441};
	Thread1Poly=  new CPolygon(36);
	Thread1Poly->Init(ArrayPts1);

	tempObj = NULL;
	IsObjFading = false;

	GetObjectByID("thread1Green")->SetEnable(false);
	GetObjectByID("thread2Green")->SetEnable(false);
	GetObjectByID("thread3Green")->SetEnable(false);
	GetObjectByID("thread4Green")->SetEnable(false);
	GetObjectByID("thread5Green")->SetEnable(false);

	GetObjectByID("thread1Red")->SetEnable(false);
	GetObjectByID("thread2Red")->SetEnable(false);
	GetObjectByID("thread3Red")->SetEnable(false);
	GetObjectByID("thread4Red")->SetEnable(false);
	GetObjectByID("thread5Red")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_Thread1Taken)
		GetObjectByID("thread1")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S2_Thread2Taken)
		GetObjectByID("thread2")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S2_Thread3Taken)
		GetObjectByID("thread3")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L3_S2_Thread4Taken)
		GetObjectByID("thread4")->SetEnable(false);

	Show(false);

}

void L3S2_Pop1Thread::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		isAnimationPlaying = false;
		ClosePopup(1.5f);
		Cursor::Show(true);
	}
	
}

void L3S2_Pop1Thread::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}
	GFInstance->WrongInvDropMsg(Hud);

}

void L3S2_Pop1Thread::Required()
{

}

void L3S2_Pop1Thread::puzzleSkip()
{
	Hud->InvBox_StopPuzzleMode();
	GetObjectByID("thread5")->SetEnable(false);
	Hud->TakeInventoryItem(eInv_L3S2_Twine, GetObjectByID("thread5")->Pos);
	ProfileSystem::Instance->ActiveProfile.L3_S2_InvTwineTaken = true;
	ClosePopup(1.0f);
}


void L3S2_Pop1Thread::ResetItems()
{
	Hud->ShowBannerText("L3remove");

	GetObjectByID("thread1")->SetAlpha(1.0f);
	GetObjectByID("thread2")->SetAlpha(1.0f);
	GetObjectByID("thread3")->SetAlpha(1.0f);
	GetObjectByID("thread4")->SetAlpha(1.0f);
	GetObjectByID("thread5")->SetAlpha(1.0f);

	GetObjectByID("thread1")->SetEnable(true);
	GetObjectByID("thread2")->SetEnable(true);
	GetObjectByID("thread3")->SetEnable(true);
	GetObjectByID("thread4")->SetEnable(true);
	GetObjectByID("thread5")->SetEnable(true);

	GetObjectByID("thread1Green")->SetAlpha(1.0f);
	GetObjectByID("thread2Green")->SetAlpha(1.0f);
	GetObjectByID("thread3Green")->SetAlpha(1.0f);
	GetObjectByID("thread4Green")->SetAlpha(1.0f);
	GetObjectByID("thread5Green")->SetAlpha(1.0f);

	GetObjectByID("thread1Green")->SetEnable(false);
	GetObjectByID("thread2Green")->SetEnable(false);
	GetObjectByID("thread3Green")->SetEnable(false);
	GetObjectByID("thread4Green")->SetEnable(false);
	GetObjectByID("thread5Green")->SetEnable(false);

	GetObjectByID("thread1Red")->SetAlpha(1.0f);
	GetObjectByID("thread2Red")->SetAlpha(1.0f);
	GetObjectByID("thread3Red")->SetAlpha(1.0f);
	GetObjectByID("thread4Red")->SetAlpha(1.0f);
	GetObjectByID("thread5Red")->SetAlpha(1.0f);

	GetObjectByID("thread1Red")->SetEnable(false);
	GetObjectByID("thread2Red")->SetEnable(false);
	GetObjectByID("thread3Red")->SetEnable(false);
	GetObjectByID("thread4Red")->SetEnable(false);
	GetObjectByID("thread5Red")->SetEnable(false);

	ProfileSystem::Instance->ActiveProfile.L3_S2_Thread1Taken = false;
	ProfileSystem::Instance->ActiveProfile.L3_S2_Thread2Taken = false;
	ProfileSystem::Instance->ActiveProfile.L3_S2_Thread3Taken = false;
	ProfileSystem::Instance->ActiveProfile.L3_S2_Thread4Taken = false;
}

void L3S2_Pop1Thread::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

		
	if( Hud->IsDraggingItem()  )
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	if(IsObjFading)
	{
		if(tempObj != NULL)
		{
			if(tempObj->pSprite->Alpha > 0)
			{
				tempObj->SetAlpha(tempObj->pSprite->Alpha - 0.01f);
				if(tempObj == GetObjectByID("thread1Green"))
					GetObjectByID("thread1")->SetAlpha(tempObj->pSprite->Alpha - 0.01f);
				if(tempObj == GetObjectByID("thread2Green"))
					GetObjectByID("thread2")->SetAlpha(tempObj->pSprite->Alpha - 0.01f);
				if(tempObj == GetObjectByID("thread3Green"))
					GetObjectByID("thread3")->SetAlpha(tempObj->pSprite->Alpha - 0.01f);
				if(tempObj == GetObjectByID("thread4Green"))
					GetObjectByID("thread4")->SetAlpha(tempObj->pSprite->Alpha - 0.01f);
			}
			else
			{
				IsObjFading = false;
				if(tempObj == GetObjectByID("thread1Red")|| tempObj == GetObjectByID("thread2Red")|| tempObj == GetObjectByID("thread3Red")|| tempObj == GetObjectByID("thread4Red") || tempObj == GetObjectByID("thread5Red"))
					ResetItems();
			}

		}
		return;
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();
	
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Thread1Taken &&  Thread1Poly->Intersection(&pos))
	{
		if( Control::Input->LBclicked())
		{
			GetObjectByID("thread1Green")->SetEnable(true);
			tempObj = GetObjectByID("thread1Green");
			IsObjFading = true;
			ProfileSystem::Instance->ActiveProfile.L3_S2_Thread1Taken = true;
			Control::Audio->QuickLoadAndPlaySFX("s2p1_rightthreadclicked");
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Thread2Taken && ProfileSystem::Instance->ActiveProfile.L3_S2_Thread1Taken && Thread2Poly->Intersection(&pos))
	{
		if( Control::Input->LBclicked())
		{
			GetObjectByID("thread2Green")->SetEnable(true);
			tempObj = GetObjectByID("thread2Green");
			IsObjFading = true;
			ProfileSystem::Instance->ActiveProfile.L3_S2_Thread2Taken = true;
			Control::Audio->QuickLoadAndPlaySFX("s2p1_rightthreadclicked");
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Thread3Taken && ProfileSystem::Instance->ActiveProfile.L3_S2_Thread2Taken && Thread3Poly->Intersection(&pos))
	{
		if( Control::Input->LBclicked())
		{
			GetObjectByID("thread3Green")->SetEnable(true);
			tempObj = GetObjectByID("thread3Green");
			IsObjFading = true;
			ProfileSystem::Instance->ActiveProfile.L3_S2_Thread3Taken = true;
			Control::Audio->QuickLoadAndPlaySFX("s2p1_rightthreadclicked");
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Thread4Taken && ProfileSystem::Instance->ActiveProfile.L3_S2_Thread3Taken && Thread4Poly->Intersection(&pos))
	{
		if( Control::Input->LBclicked())
		{
			GetObjectByID("thread4Green")->SetEnable(true);
			tempObj = GetObjectByID("thread4Green");
			IsObjFading = true;
			ProfileSystem::Instance->ActiveProfile.L3_S2_Thread4Taken = true;
			Control::Audio->QuickLoadAndPlaySFX("s2p1_rightthreadclicked");
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Thread2Taken && Thread2Poly->Intersection(&pos))
	{
		if( Control::Input->LBclicked())
			{
				GetObjectByID("thread2Red")->SetEnable(true);
				tempObj = GetObjectByID("thread2Red");
				IsObjFading = true;
				Control::Audio->QuickLoadAndPlaySFX("s2p1_wrongthreadclicked");
			}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Thread3Taken && Thread3Poly->Intersection(&pos))
	{
		if( Control::Input->LBclicked())
			{
				GetObjectByID("thread3Red")->SetEnable(true);
				tempObj = GetObjectByID("thread3Red");
				IsObjFading = true;
				Control::Audio->QuickLoadAndPlaySFX("s2p1_wrongthreadclicked");
			}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Thread4Taken && Thread4Poly->Intersection(&pos))
	{
		if( Control::Input->LBclicked())
			{
				GetObjectByID("thread4Red")->SetEnable(true);
				tempObj = GetObjectByID("thread4Red");
				IsObjFading = true;
				Control::Audio->QuickLoadAndPlaySFX("s2p1_wrongthreadclicked");
			}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_Thread4Taken && Thread5Poly->Intersection(&pos))
	{
		if( Control::Input->LBclicked())
			{
				GetObjectByID("thread5Red")->SetEnable(true);
				tempObj = GetObjectByID("thread5Red");
				IsObjFading = true;
				Control::Audio->QuickLoadAndPlaySFX("s2p1_wrongthreadclicked");
			}
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S2_Thread4Taken && InvDestRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
			{
				puzzleSkip();
			}
	}
}

float L3S2_Pop1Thread::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L3S2_Pop1Thread::~L3S2_Pop1Thread()
{
	SAFE_DELETE(Thread5Poly);
	SAFE_DELETE(Thread4Poly);
	SAFE_DELETE(Thread3Poly);
	SAFE_DELETE(Thread2Poly);
	SAFE_DELETE(Thread1Poly);
}

void L3S2_Pop1Thread::ReceiveMessage(int val)
{	

	
}

bool L3S2_Pop1Thread::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvTwineTaken)
		isAnyTaskLeft = true;
	

	return isAnyTaskLeft;
}

void L3S2_Pop1Thread::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	puzzleSkip();
}
