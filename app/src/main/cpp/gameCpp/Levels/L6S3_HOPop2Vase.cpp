
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L6S3_HOPop2Vase.h"
#include "Hud.h"

const CRectangle  Bolt1Rect(591.0f,287.0f,79.0f,74.0f);
const CRectangle  Bolt2Rect(700.0f,304.0f,80.0f,86.0f);
const CRectangle  DoorRect(561.0f,242.0f,258.0f,199.0f);


L6S3_HOPop2Vase::L6S3_HOPop2Vase(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV6\\S3\\HO_P2\\HO_P2.SCN");
	SetFrameAndCloseButton(true);

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);
	Spanner = GetObjectByID("Spaner");

	//if( !IsSpannerActive)
	Spanner->SetEnable(false);
	IsSpannerActive = false;
	isAnimationPlaying = false;
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed)
		GetObjectByID("Bolt02")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed)
		GetObjectByID("Bolt01")->SetEnable(false);

	JAnim_Bolt1Remove = NULL;
	JAnim_Bolt1Remove = new JSONAnimator( 0, CC_CALLBACK_1(L6S3_HOPop2Vase::OnAnimationComplete, this ));
	JAnim_Bolt1Remove->parseAndLoadJSON("ANIM\\L6\\S3_HO_BOLT1REMOVE.JSON", this );

	JAnim_Bolt2Remove = NULL;
	JAnim_Bolt2Remove = new JSONAnimator( 1, CC_CALLBACK_1(L6S3_HOPop2Vase::OnAnimationComplete, this ));
	JAnim_Bolt2Remove->parseAndLoadJSON("ANIM\\L6\\S3_HO_BOLT2REMOVE.JSON", this );

	JAnim_DoorRemove = NULL;
	JAnim_DoorRemove = new JSONAnimator( 2, CC_CALLBACK_1(L6S3_HOPop2Vase::OnAnimationComplete, this ));
	JAnim_DoorRemove->parseAndLoadJSON("ANIM\\L6\\S3_HO_DOORREMOVE.JSON", this );

	SpannerOffset = CPoint(120, -40, 0);
}

void L6S3_HOPop2Vase::OnPopupOpened()
{
	if( !IsSpannerActive)
		Spanner->SetEnable(false);
	else
		Spanner->SetEnable(true);
}

void L6S3_HOPop2Vase::OnAnimationComplete(int index)
{
	isAnimationPlaying = false;
	switch (index)
	{
	case 0:	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed)
			{
				Spanner->SetEnable(false);
				IsSpannerActive =false;
			}
		break;
	case 1:	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed)
			{
				Spanner->SetEnable(false);
				IsSpannerActive = false;
			}
		break;
	case 2:		
		IsSpannerActive = false;
		JAnim_DoorRemove->SetEnableObjects(false);
		Spanner->SetEnable(false);
		ClosePopup();
		break;
	default:
		break;
	}
	Hud->SendUpdateSceneMessage();
}

void L6S3_HOPop2Vase::Required()
{

}

void L6S3_HOPop2Vase::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	

	CPoint SpannerPos = CPoint( pos.x + SpannerOffset.x,pos.y + SpannerOffset.y , eZOrder_PopupCloseButton + 10 );

	if( IsSpannerActive )
	{
		Spanner->SetPos( &SpannerPos );
		Spanner->SetZ((int)(SpannerPos.z));
		if( Bolt1Rect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed)
		{
			if( Control::Input->LBclicked() )
			{
				JAnim_Bolt1Remove->PlayAnim();
				Control::Audio->QuickLoadAndPlaySFX("s3hop2_boltRemove");
				isAnimationPlaying = true;
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed = true;
			}
		}
		else if( Bolt2Rect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed)
		{
			if( Control::Input->LBclicked() )
			{
				JAnim_Bolt2Remove->PlayAnim();
				Control::Audio->QuickLoadAndPlaySFX("s3hop2_boltRemove");
				isAnimationPlaying = true;
				ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed = true;
			}
		}
		return;
	}
	
	if( DoorRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed &&
			!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			JAnim_DoorRemove->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s3hop2_lidRemove");
			isAnimationPlaying =true;
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved = true;
			//ClosePopup(1.5f);
		}
		return;
	}

	if( !IsSpannerActive && DoorRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed && 
		!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed )
	{
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L6tool");
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
	
}

void L6S3_HOPop2Vase::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L6S3_HOPop2Vase::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;
	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L6_S3_HO_VaseDoorRemoved && ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt1Removed &&
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_Bolt2Removed)
		Hud->InitHintTrailEmit(DoorRect.GetCenterX(), DoorRect.GetCenterY(), false);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L6S3_HOPop2Vase::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	return isAnyTaskLeft;
}

L6S3_HOPop2Vase::~L6S3_HOPop2Vase()
{
	SAFE_DELETE(JAnim_Bolt1Remove);
	SAFE_DELETE(JAnim_Bolt2Remove);
	SAFE_DELETE(JAnim_DoorRemove);
}

