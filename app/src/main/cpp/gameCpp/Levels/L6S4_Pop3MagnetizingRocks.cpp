//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 04 - POPUP 3: 
//====================================

#include "L6S4_Pop3MagnetizingRocks.h"

const CRectangle  InvDestClubRect(387.0f,140.0f,599.0f,412.0f);
const CRectangle  InvDestHorseShoeRect(508.0f,322.0f,214.0f,198.0f);

const CRectangle  PopAreaRect(479.0f,35.0f,413.0f,598.0f);

const CRectangle  AmeehaLetterRect(425.0f,10.0f,520.0f,666.0f);



L6S4_Pop3MagnetizingRocks::L6S4_Pop3MagnetizingRocks(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S4\\P3\\P3.SCN");
	//Localized
	/*
	naginLetter = new CObject();
	naginLetter->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\NAGINLETTER.SPR", 685, 345, zOrder + 200);
	naginLetter->SetUID("lettertext2");
	naginLetter->Show(false);
	scene->PushObject(naginLetter);
	
	/*
	SceneData textNagin;
	textNagin.objType = "Object";
	textNagin.uid = "lettertext";
	textNagin.spriteString = "LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\NAGINLETTER.SPR";
	textNagin.spriteAnim = 0;
	textNagin.x = 685;
	textNagin.y = 345;
	textNagin.ZOrder = zOrder + 200;

	naginLetter = CreateObject(textNagin);
	//naginLetter->Show(false);
	*/

	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Control::Audio->LoadSample(aVOL6NoteVOS4_2,	AudioVO);
	GetObjectByID("club")->SetEnable(false);
	GetObjectByID("M_HorseShoe")->SetEnable(false);
	GetObjectByID("HorseShoe")->SetEnable(false);

	//For Ameeha Notice Popup
	GetObjectByID("Mask")->SetEnable(false);
	GetObjectByID("Mask")->SetZ(zOrder+120);
	GetObjectByID("Mask")->SetScale(2.5f);

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened)
	{
		GetObjectByID("MagneticRock")->SetEnable(false);
		GetObjectByID("StonePiece1")->SetEnable(false);
		GetObjectByID("StonePiece2")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected)
	{
		GetObjectByID("FlintStoneOnPopup")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnetisedHorseShoeTaken)
	{
		GetObjectByID("M_HorseShoe")->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_AmeehaNoteTaken)
	{
		GetObjectByID("Letter")->SetEnable(false);
	}

	Janim_rockBreaking = new JSONAnimator(0, CC_CALLBACK_1(L6S4_Pop3MagnetizingRocks::OnAnimComplete, this));
	Janim_rockBreaking->parseAndLoadJSON("ANIM\\L6\\S4_BREAK_ROCK.JSON",this);

	Janim_magnetiseHorseshoe = new JSONAnimator(1, CC_CALLBACK_1(L6S4_Pop3MagnetizingRocks::OnAnimComplete, this));
	Janim_magnetiseHorseshoe->parseAndLoadJSON("ANIM\\L6\\S4_MAGNETIZE_HORSESHOE.JSON",this);

	int ArrayPts[] = {552,269,664,501,823,426,708,193,553,265};
	CPolygon *CPolyObjName=  new CPolygon(5);
	CPolyObjName->Init(ArrayPts);
	popAmeehaNote = new L6S1_PopNaginNote(1, Hud, NULL, zOrder + 200, CPolyObjName->center,CC_CALLBACK_1(L6S4_Pop3MagnetizingRocks::OnPopupClosed,this));

	//Localized
	CObject *temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\NAGINTEXT.SPR", 680, 330, eZOrder_Popup + 210);
	temp->SetUID("text");
	temp->Show(false);
	popAmeehaNote->PushObject(temp);

	popAmeehaNote->SetPopupBoundingBox(AmeehaLetterRect);
	delete(CPolyObjName);

	int ArrayPts1[] = {553,270,711,193,827,417,664,501,550,277,548,270};
	letterPoly =  new CPolygon(6);
	letterPoly->Init(ArrayPts1);

	int ArrayPts2[] = {645,298,624,357,631,422,677,466,683,469,706,430,706,351,669,308,645,294};
	stonePoly =  new CPolygon(9);
	stonePoly->Init(ArrayPts2);

	activePopupId = ePopInactive;
	Control::Audio->LoadSample(aVOL6MaryRefS6_2,	AudioVO);
	SetSparkleUpdate(false);
	Show(false);
}


void L6S4_Pop3MagnetizingRocks::Update(float ds)
{
	Popup::Update(ds);
	//naginLetter->Show(false);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if (activePopupId != ePopInactive)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if (popAmeehaNote->GetPopupState() == ePopupState_Open)
	{
		return;
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_AmeehaNoteTaken)
	{
		if(GetObjectByID("lettertext") != NULL)
			GetObjectByID("lettertext")->Show(false);
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
	

	if (letterPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S4_AmeehaNoteTaken)
	{
		Cursor::SetMode(CA_Look);
		if (Control::Input->LBclicked())
		{
			isAnimationPlaying = true; //To avoid popup closing
			Control::Audio->PlaySample(aVOL6NoteVOS4_2);
			GetObjectByID("Mask")->SetEnable(true);
			GetObjectByID("Mask")->FadeIn(3.0f);
			popAmeehaNote->OpenPopup();
			GetObjectByID("Letter")->SetEnable(false);
			if (GetObjectByID("lettertext") != NULL)
				GetObjectByID("lettertext")->Show(false);
			//GetObjectByID("lettertext")->SetEnable(false);
			activePopupId = 1;
		}
	}
	else if (InvDestClubRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && ProfileSystem::Instance->ActiveProfile.L6_S4_AmeehaNoteTaken)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6rock");
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected && stonePoly->Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("FlintStoneOnPopup")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L6_S4_FlintStones);
			ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected = true;

			if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnetisedHorseShoeTaken)
			{
				ClosePopup(1);
			}
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && !ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized && InvDestHorseShoeRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);

		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L6magnet");
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnetisedHorseShoeTaken && GetObjectByID("M_HorseShoe")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("M_HorseShoe")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L6_S4_MagnetisedHorseShoe);
			ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnetisedHorseShoeTaken = true;

			if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected)
			{
				ClosePopup(1);
			}
		}
	}
}

void L6S4_Pop3MagnetizingRocks::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L6_S1_Club:
		if (!ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && InvDestClubRect.Intersection(&dropPos) && ProfileSystem::Instance->ActiveProfile.L6_S4_AmeehaNoteTaken)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p3_breakRock");
			Janim_rockBreaking->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			if (ProfileSystem::Instance->ActiveProfile.L6_S3_ClubsUsed)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Club] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S1_Club);
			}
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L6_S3_HorseShoe:
		if (ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && InvDestHorseShoeRect.Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p3_magnetizeHorseShoe");
			Janim_magnetiseHorseshoe->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_HorseShoe] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S3_HorseShoe);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L6S4_Pop3MagnetizingRocks::OnAnimComplete(int animID)
{
	switch (animID)
	{
	case 0:
		GetObjectByID("club")->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened = true;
		break;
	case 1:
		ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized = true;
	default:
		break;
	}
}

void L6S4_Pop3MagnetizingRocks::OnPopupClosed(Popup * popup)
{
	activePopupId = ePopInactive;

	isAnimationPlaying = false;

	GetObjectByID("Mask")->SetEnable(false);

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P4_NaginNotice])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P4_NaginNotice] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P4_NaginNotice;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
	}

	Hud->ShowNewInfoTextBanner();

	ProfileSystem::Instance->ActiveProfile.L6_S4_AmeehaNoteTaken = true;
	/*Control::Audio->StopSample(aVOL6NoteVOS4_2);
	Control::Audio->PlaySample(aVOL6MaryRefS4_1);*/	
	Control::Audio->StopSample(aVOL6NoteVOS4_2);
	Control::Audio->PlaySample(aVOL6MaryRefS6_2);
	Hud->ShowBannerText("L6nagin");
}

float L6S4_Pop3MagnetizingRocks::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S4_Pop3MagnetizingRocks::~L6S4_Pop3MagnetizingRocks()
{
	SAFE_DELETE(popAmeehaNote);
	SAFE_DELETE(Janim_magnetiseHorseshoe);
	SAFE_DELETE(Janim_rockBreaking);
	SAFE_DELETE(letterPoly);
	SAFE_DELETE(stonePoly);
	/*Control::Audio->UnloadSample(aVOL6MaryRefS4_1);
	Control::Audio->UnloadSample(aVOL6MaryRefS5_1);*/
	Control::Audio->UnloadSample(aVOL6NoteVOS4_2);
	Control::Audio->UnloadSample(aVOL6MaryRefS6_2);
}

void L6S4_Pop3MagnetizingRocks::Required()
{

}

void L6S4_Pop3MagnetizingRocks::ClosePopup(float delayTime)
{
	if (activePopupId != ePopInactive)
		return;
	
	Popup::ClosePopup(delayTime);
}

bool L6S4_Pop3MagnetizingRocks::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_AmeehaNoteTaken ||
		(ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened) ||
		(ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected) ||
		(ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized) ||
		(ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnetisedHorseShoeTaken))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L6S4_Pop3MagnetizingRocks::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S4_AmeehaNoteTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("Letter")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle letterObjRect =GetObjectByID("Letter")->GetWorldRect();
		Hud->InitHintTrailEmit(&letterObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened)
	{
		Hud->ShowHintForInvItem(eInv_L6_S1_Club,&InvDestClubRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("FlintStoneOnPopup")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle flintstoneObjRect =GetObjectByID("FlintStoneOnPopup")->GetWorldRect();
		Hud->InitHintTrailEmit(&flintstoneObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened && ProfileSystem::Instance->ActiveProfile.L6_S1_InvHorseShoeTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized)
	{
		Hud->ShowHintForInvItem(eInv_L6_S3_HorseShoe,&InvDestHorseShoeRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_IsHorseShoeMagnetized && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvMagnetisedHorseShoeTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("M_HorseShoe")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle horseshoeObjRect =GetObjectByID("M_HorseShoe")->GetWorldRect();
		Hud->InitHintTrailEmit(&horseshoeObjRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
