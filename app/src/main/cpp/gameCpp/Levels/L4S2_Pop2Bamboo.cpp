//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 02 - POPUP 2: BAMBOO
//====================================

#include "L4S2_Pop2Bamboo.h"
#include "ProfileSystem.h"

const CRectangle  MemRectBanana(822.0f,424.0f,128.0f,95.0f);
const CRectangle  BambooRect(367.0f,351.0f,317.0f,110.0f);


L4S2_Pop2Bamboo::L4S2_Pop2Bamboo(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	_jsonAnimBambooCut = NULL;
	_smallStick0 = NULL;
	_smallStick1 = NULL;
	_smallStick2 = NULL;
	_smallStick3 = NULL;

	//Popup load
	Load("LV4\\S2\\POP2\\P2.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	SetSparkleUpdate(false);


	_smallStick0 = GetObjectByID("BambooStick0");
	_smallStick1 = GetObjectByID("BambooStick1");
	_smallStick2 = GetObjectByID("BambooStick2");
	_smallStick3 = GetObjectByID("BambooStick3");
	_bambooStick = GetObjectByID("Bamboo1");
	GetObjectByID("Knife")->SetEnable(false);

	int ArrayPts[] = {452,88,640,450,1007,381,1002,86};
	_knifeDropPolygon =  new CPolygon(4);
	_knifeDropPolygon->Init(ArrayPts);


	int ArrayPts0[] = {489,355,498,335,379,319,372,343};
	CPolygon * stickPoly0=  new CPolygon(4);
	stickPoly0->Init(ArrayPts0);
	_smallStick0->SetMouseClickProperties(stickPoly0, CC_CALLBACK_1(L4S2_Pop2Bamboo::OnBambooStickClick, this));
	_arrayOfObjects_Cursor_Take.push_back(_smallStick0);

	int ArrayPts1[] = {453,268,462,280,557,258,553,238};
	CPolygon * stickPoly1 =  new CPolygon(4);
	stickPoly1->Init(ArrayPts1);
	_smallStick1->SetMouseClickProperties(stickPoly1, CC_CALLBACK_1(L4S2_Pop2Bamboo::OnBambooStickClick, this));
	_arrayOfObjects_Cursor_Take.push_back(_smallStick1);

	int ArrayPts2[] = {524,309,528,326,632,319,631,296};
	CPolygon * stickPoly2 =  new CPolygon(4);
	stickPoly2->Init(ArrayPts2);
	_smallStick2->SetMouseClickProperties(stickPoly2, CC_CALLBACK_1(L4S2_Pop2Bamboo::OnBambooStickClick, this));
	_arrayOfObjects_Cursor_Take.push_back(_smallStick2);

	int ArrayPts3[] = {542,374,561,361,626,454,601,467};
	CPolygon * stickPoly3 =  new CPolygon(4);
	stickPoly3->Init(ArrayPts3);
	_smallStick3->SetMouseClickProperties(stickPoly3, CC_CALLBACK_1(L4S2_Pop2Bamboo::OnBambooStickClick, this));
	_arrayOfObjects_Cursor_Take.push_back(_smallStick3);


	int ArrayPts4[] = {413,370,412,428,825,435,834,360};
	CPolygon * stickPoly4 =  new CPolygon(4);
	stickPoly4->Init(ArrayPts4);
	_bambooStick->SetMouseClickProperties(stickPoly4, CC_CALLBACK_1(L4S2_Pop2Bamboo::OnBambooStickClick, this));
	_bambooStick->SetIsInteractionEnabled(false);
	_arrayOfObjects_Cursor_Take.push_back(_bambooStick);


	InitializeWithProfileData();
	Show(false);

	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana])
	{
		GetObjectByID("membanana")->SetEnable(false);
	}
}

void L4S2_Pop2Bamboo::InitializeWithProfileData()
{
	_bambooStick->SetIsInteractionEnabled(false);
	_arrayOfObjects_Cursor_Take.push_back(_bambooStick);

	_jsonAnimBambooCut = new JSONAnimator(1, CC_CALLBACK_1(L4S2_Pop2Bamboo::OnBambooCutAnimComplete, this));
	_jsonAnimBambooCut->parseAndLoadJSON("ANIM\\L4\\L4_S2_BAMBOOCUT.JSON", this);
	_jsonAnimBambooCut->PlayAnim();
	_jsonAnimBambooCut->StopAnim();
	_jsonAnimBambooCut->SetAnimTime(0);
	GetObjectByID("Knife")->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken)
	{
		_smallStick0->SetEnable(false);
		_smallStick0->SetIsInteractionEnabled(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken)
	{
		_smallStick1->SetEnable(false);
		_smallStick1->SetIsInteractionEnabled(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken)
	{
		_smallStick2->SetEnable(false);
		_smallStick2->SetIsInteractionEnabled(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken)
	{
		_smallStick3->SetEnable(false);
		_smallStick3->SetIsInteractionEnabled(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_IsBambooStickCut)
	{
		_jsonAnimBambooCut->SetAnimTime(1.0f);
		_bambooStick->SetIsInteractionEnabled(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S2_BambooStickTaken)
	{
		_bambooStick->SetEnable(false);
		_jsonAnimBambooCut->SetEnableObjects(false);
		_bambooStick->SetIsInteractionEnabled(false);
	}


	GetObjectByID("Knife")->SetEnable(false);

}

void L4S2_Pop2Bamboo::OnPopupOpened()
{

}

void L4S2_Pop2Bamboo::OnBambooStickClick(CObject * stick)
{
	if(GetPopupState()!= ePopupState_Open )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (Hud->IsDraggingItem())
	{
		return;
	}
	stick->SetIsInteractionEnabled(false);

	if(stick == _smallStick0)
	{
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken = true;
		_smallStick0->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L4_S2_BambooStick1, stick->Pos);
	}

	if(stick == _smallStick1)
	{
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken = true;
		_smallStick1->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L4_S2_BambooStick2, stick->Pos);
	}

	if(stick == _smallStick2)
	{
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken = true;
		_smallStick2->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L4_S2_BambooStick3, stick->Pos);
	}

	if(stick == _smallStick3)
	{
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken = true;
		_smallStick3->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L4_S2_BambooStick4, stick->Pos);
	}

	if (stick == _bambooStick)
	{
		ProfileSystem::Instance->ActiveProfile.L4_S2_BambooStickTaken = true;
		_bambooStick->SetEnable(false);
		Hud->TakeInventoryItem(eInv_L4_S2_Bamboo, stick->Pos);
	}

#ifdef _CEBUILD
	if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken 
		&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken
		&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken
		&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken
		&&ProfileSystem::Instance->ActiveProfile.L4_S2_BambooStickTaken
		&&ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana]	)
#else
	if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken 
		&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken
		&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken
		&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken
		&&ProfileSystem::Instance->ActiveProfile.L4_S2_BambooStickTaken	)
#endif 
	{
		ProfileSystem::Instance->ActiveProfile.L4_S2_Pop2BambooTaskCompleted = true;
		_clickAreaToOpenPopup->SetIsInteractionEnabled(false);
		ClosePopup(1.5);
	}
}


void L4S2_Pop2Bamboo::OnBambooCutAnimComplete(int id)
{
	GetObjectByID("Knife")->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L4_S2_IsBambooStickCut = true;
	_bambooStick->SetIsInteractionEnabled(true);
}


void L4S2_Pop2Bamboo::Required()
{

}

void L4S2_Pop2Bamboo::HandleInventoryDrop(CPoint dropPos, int itemVal)
{

	if (itemVal == eInv_L4_S1_Knife)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L4_S2_IsBambooStickCut && _knifeDropPolygon->Intersection(&dropPos))
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			
			_jsonAnimBambooCut->SetEnableObjects(true);
			_jsonAnimBambooCut->PlayAnim(false);
			Control::Audio->QuickLoadAndPlaySFX("s2p2_bambooCut");


		}
		else
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}

}

void L4S2_Pop2Bamboo::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

void L4S2_Pop2Bamboo::OpenPopup()
{
	InitializeWithProfileData();
	Popup::OpenPopup();
}


void L4S2_Pop2Bamboo::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}

	bool isCursorHandPick = false;
	for (int i = 0; i < (int)(_arrayOfObjects_Cursor_Take.size()); i++)
	{
		CObject * object = _arrayOfObjects_Cursor_Take.at(i);
		if (object->GetCurrentMouseState() == eMouseState_Over || object->GetCurrentMouseState() == eMouseState_Down)
		{
			isCursorHandPick = true;
			break;
		}
	}
	CPoint mousePos = Control::Input->Mpos();
	if (isCursorHandPick)
	{
		Cursor::SetMode(CA_HandTake);
	}
	//	else if(!Hud->IsDraggingItem()  && !ProfileSystem::Instance->ActiveProfile.L4_S2_IsBambooStickCut && _knifeDropPolygon->Intersection(&Control::Input->Mpos()))
	// Linux: gives error of taking address of temporary also potential crash factor
	// below is fix
	else if(!Hud->IsDraggingItem()  && !ProfileSystem::Instance->ActiveProfile.L4_S2_IsBambooStickCut && _knifeDropPolygon->Intersection(&mousePos))
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4bare");
		}
	}
	else if(!isMouseOverCloseButton)
	{
		Cursor::SetMode(CA_Normal);
	}

#ifdef _CEBUILD
//	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana] && MemRectBanana.Intersection(&Control::Input->Mpos()))
	// Linux: gives error of taking address of temporary also potential crash factor
	// below is fix
	CPoint mousePos1 = Control::Input->Mpos();
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana] && MemRectBanana.Intersection(&mousePos1))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("membanana")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L4S2Banana, &GetObjectByID("membanana")->Pos);
			if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken 
				&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken
				&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken
				&&ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken
				&&ProfileSystem::Instance->ActiveProfile.L4_S2_BambooStickTaken
				)
			{
				ProfileSystem::Instance->ActiveProfile.L4_S2_Pop2BambooTaskCompleted = true;
				_clickAreaToOpenPopup->SetIsInteractionEnabled(false);
				ClosePopup(1.5);
			}
		}
	}
#else
    CPoint mousePos1 = Control::Input->Mpos();
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana] && MemRectBanana.Intersection(&mousePos1))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("membanana")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L4S2Banana, &GetObjectByID("membanana")->Pos);
		}
	}
#endif 
}

L4S2_Pop2Bamboo::~L4S2_Pop2Bamboo()
{
	SAFE_DELETE(_jsonAnimBambooCut);
	_arrayOfObjects_Cursor_Take.clear();
}

bool L4S2_Pop2Bamboo::isTaskLeft()
{
	if (ProfileSystem::Instance->ActiveProfile.L4_S2_Pop2BambooTaskCompleted)
	{
		return false;
	}

	return true;
}

void L4S2_Pop2Bamboo::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S2_IsBambooStickCut && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife] )
		Hud->ShowHintForInvItem(eInv_L4_S1_Knife, PointSystem::CreateCPoint(_bambooStick->GetCenterX(),_bambooStick->GetCenterY(),0));
	else if( !ProfileSystem::Instance->ActiveProfile.L4_S2_BambooStickTaken && ProfileSystem::Instance->ActiveProfile.L4_S2_IsBambooStickCut)
		Hud->InitHintTrailEmit(BambooRect.GetCenterX(), BambooRect.GetCenterY());
	else if( !ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken )
		Hud->InitHintTrailEmit(_smallStick0->GetCenterX(), _smallStick0->GetCenterY());
	else if( !ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken)
		Hud->InitHintTrailEmit(_smallStick1->GetCenterX(), _smallStick1->GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken)
		Hud->InitHintTrailEmit(_smallStick2->GetCenterX(), _smallStick2->GetCenterY());
	else if( !ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken)
		Hud->InitHintTrailEmit(_smallStick3->GetCenterX(), _smallStick3->GetCenterY());
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
