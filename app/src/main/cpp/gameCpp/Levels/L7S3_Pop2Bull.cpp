//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7S3_Pop2Bull.h"
#include "Hud.h"

const CRectangle  InvDestRectYellowEmblem(677.0f,129.0f,320.0f,232.0f);
const CRectangle  InvItemRectBroom(786.0f,442.0f,190.0f,90.0f);
const CRectangle  InvItemRectAxeKey(537.0f,439.0f,79.0f,74.0f);
const CRectangle  ActionRectLetter(496.0f,361.0f,163.0f,184.0f);
const CRectangle  InvDestRectFlower(486.0f,218.0f,266.0f,314.0f);

const CRectangle  ActionRectLetterZoomed(448.0f,3.0f,478.0f,630.0f);

const CRectangle  BtnRectUpButton(1009.0f,269.0f,90.0f,110.0f);//up is right
const CRectangle  BtnRectDownButton(272.0f,273.0f,89.0f,117.0f);//down is left

const CRectangle  MaskRectPopup(361.0f,91.0f,645.0f,491.0f);

const CRectangle  CloseBtnRect(838.0f,1.0f,84.0f,74.0f);

const CRectangle  PaptusFlyRect(551.0f,420.0f,61.0f,68.0f);

L7S3_Pop2Bull::L7S3_Pop2Bull(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV7\\S3\\POP2\\P2.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	ameehaLetterText = new CObject();
	ameehaLetterText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L7\\AMEEHALETTER.SPR", 690, 290, eZOrder_Popup + 500);
	ameehaLetterText->SetUID("ameehaLetterText");
	ameehaLetterText->Show(false);

	clickDelayTimer = -1;

	Control::Audio->LoadSample(aVOL7NoteVOS3_1,	AudioVO);

	s3flowerfxFX = new SuperFX("PRT\\L7\\s3flowerfx.INI", PointSystem::CreateCPoint(633,324,0), 1028); 

	isLetterZoomedIn = false;

	isLetterSecondBannerShown = false;
	letterBannerTimer = -1;

	GetObjectByID("paptus")->SetBlendMode(eBlend_AlphaAdditive);
	if(ProfileSystem::Instance->ActiveProfile.IsPaptusL7S3Collected)
	{
		GetObjectByID("paptus")->SetEnable(false);
	}

	GetObjectByID("btntoup")->SetRotate(-90);
	GetObjectByID("btntodown")->SetRotate(-90);

	//Ammeha Letter
	GetObjectByID("letterzoomed")->SetEnable(false);
	GetObjectByID("letterzoomed")->SetZ(zOrder+120);	

	GetObjectByID("letterwotext")->SetEnable(false);
	GetObjectByID("letterwotext")->SetZ(zOrder+121);

	GetObjectByID("ameehatalk")->SetEnable(false);
	GetObjectByID("ameehatalk")->SetZ(zOrder+122);

	GetObjectByID("lettertext")->SetEnable(false);
	GetObjectByID("lettertext")->SetZ(zOrder+123);

	GetObjectByID("closebtn")->SetEnable(false);
	GetObjectByID("closebtn")->SetZ(zOrder+125);

	//For Ameeha Notice Popup
	GetObjectByID("Mask")->SetEnable(false);
	GetObjectByID("Mask")->SetZ(zOrder+100);
	GetObjectByID("Mask")->SetScale(2.5f);

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
		GetObjectByID("broom")->SetEnable(false);

	GetObjectByID("yellowemblem")->SetEnable(false);

	GetObjectByID("waterpot")->SetEnable(false);
	GetObjectByID("potwater")->SetEnable(false);

	GetObjectByID("flowerfx")->SetEnable(false);
	GetObjectByID("flowerrays")->SetEnable(false);
	GetObjectByID("floweropen")->SetEnable(false);

	GetObjectByID("flowerfx")->SetZ(zOrder+10);
	GetObjectByID("flowerrays")->SetZ(zOrder+10);
	GetObjectByID("floweropen")->SetZ(zOrder+10);

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved)
	{
		GetObjectByID("flowerclosed")->SetEnable(false);
		GetObjectByID("flowerstem")->SetEnable(false);
		GetObjectByID("bullslideend")->SetEnable(false);
	}
	else
	{
		//Bull Moved
		GetObjectByID("bullslideend")->SetEnable(true);
		GetObjectByID("bullstatue")->SetEnable(false);
		//GetObjectByID("bullstatue")->SetPos(PointSystem::CreateCPoint(941,266,0));
		
		GetObjectByID("letter")->SetPos(PointSystem::CreateCPoint(571,467,0));
		GetObjectByID("letter")->SetRotate(17.8f);
		GetObjectByID("letter")->SetScale(1.03f);
		if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsLetterAmeehaTaken)
			GetObjectByID("letter")->SetEnable(false);

		if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken)
		{
			GetObjectByID("flowerclosed")->SetEnable(false);
			GetObjectByID("flowerstem")->SetEnable(false);
			GetObjectByID("floweropen")->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
			GetObjectByID("axekey")->SetEnable(false);
	}

	std::vector<SuperFX*> superfxArray;
	bullSparkFX = new SuperFX("PRT\\L7\\s3bullsparks.INI", PointSystem::CreateCPoint(488,106,0), zOrder+10); 
	superfxArray.push_back(bullSparkFX);

	/*
	0 - JAnim_BullSlide
	1 - JAnim_LetterZoomin
	2 - JAnim_LetterZoomout
	3 - JAnim_FlowerOpen
	*/

	JAnim_BullSlide = new JSONAnimator(0, CC_CALLBACK_1(L7S3_Pop2Bull::OnAnimationComplete, this));
	JAnim_BullSlide->parseAndLoadJSON("ANIM\\L7\\S3BULLSLIDEFLOWEROPEN.JSON", this, &superfxArray);

	JAnim_LetterZoomin = new JSONAnimator(1, CC_CALLBACK_1(L7S3_Pop2Bull::OnAnimationComplete, this));
	JAnim_LetterZoomin->parseAndLoadJSON("ANIM\\L7\\S3LETTERZOOMIN.JSON", this);

	JAnim_LetterZoomout = new JSONAnimator(2, CC_CALLBACK_1(L7S3_Pop2Bull::OnAnimationComplete, this));
	JAnim_LetterZoomout->parseAndLoadJSON("ANIM\\L7\\S3LETTERZOOMOUT.JSON", this);

	JAnim_FlowerOpen = new JSONAnimator(3, CC_CALLBACK_1(L7S3_Pop2Bull::OnAnimationComplete, this));
	JAnim_FlowerOpen->parseAndLoadJSON("ANIM\\L7\\S3FLOWEROPEN.JSON", this);

	GetObjectByID("bullstatue")->SetMaskRect(MaskRectPopup);
	GetObjectByID("yellowemblem")->SetMaskRect(MaskRectPopup);

	//UI sliding handling
	LoadNextPopup = nullptr;
	
	UIBtnDown = GetObjectByID("btntodown");
	UIBtnUp = GetObjectByID("btntoup");

	UIBtnDown->SetEnable(false);
	UIBtnUp->SetEnable(false);

	maxVelocity = 5;

	maxDisplacement = 400.0f;
	slideDisplacement = 0;
	slideTimer = 0;

	isSliding = false;
	isFadingIn = false;
}

void L7S3_Pop2Bull::OnAnimationComplete(int animID)
{
	isAnimationPlaying = false;
	/*
	0 - JAnim_BullSlide
	1 - JAnim_LetterZoomin
	2 - JAnim_LetterZoomout
	3 - JAnim_FlowerOpen
	*/
	if(animID==0)
	{
		GetObjectByID("yellowemblem")->SetEnable(false);
		GetObjectByID("bullslideend")->SetEnable(true);
		GetObjectByID("bullstatue")->SetEnable(false);

		s3flowerfxFX->Start();
		//s3flowerfxFX->AdvanceByTime(2.0f);
	}
	else if(animID==1)
	{
		GetObjectByID("closebtn")->SetEnable(true);
		isLetterZoomedIn = true;
		isAnimationPlaying = true;

		//Ameeha Talking
		//GetObjectByID("letterzoomed")->SetEnable(false);

		GetObjectByID("letterwotext")->SetEnable(true);
		GetObjectByID("ameehatalk")->SetEnable(true);
		GetObjectByID("lettertext")->SetEnable(true);

		GetObjectByID("letterwotext")->FadeIn(1.35f);
		GetObjectByID("ameehatalk")->FadeIn(1.35f);
		GetObjectByID("lettertext")->FadeIn(1.35f);
		ameehaLetterText->FadeIn(1.35f);

		Control::Audio->PlaySample(aVOL7NoteVOS3_1,false);
	}
	else if(animID==2)
	{
		GetObjectByID("Mask")->SetEnable(false);
		isLetterZoomedIn = false;
		ProfileSystem::Instance->ActiveProfile.L7_S3_IsLetterAmeehaTaken = true;
		GetObjectByID("letter")->SetEnable(false);
		GetObjectByID("letterzoomed")->SetEnable(false);
		Control::Audio->StopSample(aVOL7NoteVOS3_1);
		//Add a diary note Entry here

		//Journal Entry Add
		if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P2_AmeehaNotice])
		{
			//Flag Set
			ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P2_AmeehaNotice] = true;

			//Add to Note Array
			ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P2_AmeehaNotice;

			ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

			//Increment Notes
			ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		}

		Hud->ShowNewInfoTextBanner();
	}
	else if(animID==3)
	{
		//flower opened
		GetObjectByID("floweropen")->SetEnable(false);
		GetObjectByID("flowerclosed")->SetEnable(false);
		GetObjectByID("flowerstem")->SetEnable(false);
		GetObjectByID("waterpot")->SetEnable(false);
		GetObjectByID("flowerfx")->SetEnable(false);
		GetObjectByID("flowerrays")->SetEnable(false);
		GetObjectByID("potwater")->SetEnable(false);

		ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken = true;
		Hud->TakeInventoryItem(eInv_L7_S3_YellowFlowe);
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] = true;

		if( ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken 
			&& ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
			ClosePopup(1.5f);

		if(s3flowerfxFX->Active)
			s3flowerfxFX->StopImmediate();
	}
}

void L7S3_Pop2Bull::OpenPopup()
{
	SetSceneAlpha(1.0f);
	Popup::OpenPopup();
}

void L7S3_Pop2Bull::OnPopupOpened()
{
	SetupAndShowUI();
}

void L7S3_Pop2Bull::OnPopupCloseBtnClicked()
{
	HideUI();
}

//UI sliding handling
void L7S3_Pop2Bull::SetupAndShowUI()
{
	UIBtnDown->SetEnable(true);
	UIBtnUp->SetEnable(true);
		
	UIBtnDown->FadeIn(4.0f);
	UIBtnUp->FadeIn(4.0f);

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken)
	{
		s3flowerfxFX->Start();
	}
}

void L7S3_Pop2Bull::HideUI()
{
	UIBtnDown->SetEnable(false);
	UIBtnUp->SetEnable(false);

	if(s3flowerfxFX->Active)
		s3flowerfxFX->StopImmediate();
}

void L7S3_Pop2Bull::StartSceneSlideIn(bool fromLeft)
{
	SetPopupState(ePopupState_Open);

	slideTimer = 0;

	SaveXY();
	isFadingIn = true;
	isSliding = true;
	SetSceneAlpha(0);

	maxDisplacement = abs(maxDisplacement); //positive
	if(fromLeft)
		maxDisplacement = -maxDisplacement;

	ShiftXY(maxDisplacement,0);
}

void L7S3_Pop2Bull::StartSceneSlideOut(bool toLeft)
{
	slideTimer = 0;

	HideUI();

	SaveXY();
	isFadingIn = false;
	isSliding = true;
	maxDisplacement = abs(maxDisplacement); //positive

	if(toLeft)
		maxDisplacement = -maxDisplacement; //shift scene to left

	//SetPopupState(ePopupState_Closed);
}

void L7S3_Pop2Bull::resetScene()
{
	RestoreXY();
	SetSceneAlpha(1.0f);
	if(isFadingIn)
	{
		_popupAlpha = 1.0f;
	}
	else
	{
		_popupTransitionTimer = 0;
		_popupAlpha = 0;
	}
}

//Popup ID and LoadPrevious if true, next if false
void L7S3_Pop2Bull::setLoadNextCallback(std::function<void (Popup*, bool)> _loadNextPopupCB)
{
	LoadNextPopup = _loadNextPopupCB;
}
//UI sliding handling

void L7S3_Pop2Bull::Required()
{

}

void L7S3_Pop2Bull::Update(float ds)
{
	Popup::Update(ds);
	if(GFInstance->IsActivePopup)
		return;

	if(isSliding)
	{
		slideTimer += Control::LogicRate*maxVelocity;
		if(!isFadingIn)
		{
			SetSceneAlpha(1.0f-slideTimer);
			ShiftXY(slideTimer*maxDisplacement,0);
		}
		else
		{
			SetSceneAlpha(slideTimer);
			ShiftXY(maxDisplacement - slideTimer*maxDisplacement,0);
		}

		if(slideTimer>1.0f)
		{
			//SLOG::Log("End of Pop wall 2 trans");
			//end of slide
			isSliding = false;

			if(isFadingIn)
				SetupAndShowUI();
			else
				SetPopupState(ePopupState_Closed);

			resetScene();
		}
		return;
	}

	if(GetPopupState()!= ePopupState_Open || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		if (isLetterZoomedIn)
		{
			GetObjectByID("closebtn")->PlayAnimation(0);

			
			if(!Control::Audio->IsPlaying(aVOL7NoteVOS3_1) && GetObjectByID("ameehatalk")->IsActive)
			{
				GetObjectByID("ameehatalk")->SetEnable(false);
			}

			bool isCloseBtnMouseOver = CloseBtnRect.Intersection(&pos);
			if(isCloseBtnMouseOver)
			{
				Cursor::SetMode(CA_HandPointy);
				GetObjectByID("closebtn")->PlayAnimation(1);
			}
			#ifdef ENABLE_ANDROID_BACK_BUTTON
					if (Control::Input->isKeyBackSensed()) {
						Control::Audio->QuickLoadAndPlaySFX("s3p2_notezoomout");

						GetObjectByID("letterzoomed")->SetEnable(true);

						GetObjectByID("letterwotext")->SetEnable(false);
						GetObjectByID("ameehatalk")->SetEnable(false);
						GetObjectByID("lettertext")->SetEnable(false);

						GetObjectByID("Mask")->FadeOut(2.0f);

						Hud->ShowBannerText("L7marry");

						letterBannerTimer = 3.5f;

						JAnim_LetterZoomout->PlayAnim();
						isAnimationPlaying = true;
						isLetterZoomedIn = false;
					}
			#endif

			if(Control::Input->LBclicked())
			{
				if( isCloseBtnMouseOver || !ActionRectLetterZoomed.Intersection(&pos) )
				{
					GetObjectByID("closebtn")->SetEnable(false);
					Control::Audio->QuickLoadAndPlaySFX("s3p2_notezoomout");

					GetObjectByID("letterzoomed")->SetEnable(true);

					GetObjectByID("letterwotext")->SetEnable(false);
					GetObjectByID("ameehatalk")->SetEnable(false);
					GetObjectByID("lettertext")->SetEnable(false);

					ameehaLetterText->SetEnable(false);

					GetObjectByID("Mask")->FadeOut(2.0f);

					Hud->ShowBannerText("L7marry");

					letterBannerTimer = 3.5f;

					JAnim_LetterZoomout->PlayAnim();
					isAnimationPlaying = true;
					isLetterZoomedIn = false;
				}
			}
		}
		return;
	}

	if(!isLetterSecondBannerShown)
	{
		if(letterBannerTimer>0)
		{
			letterBannerTimer -= Control::LogicRate;
			if(letterBannerTimer<0)
			{
				letterBannerTimer = -1;
				isLetterSecondBannerShown = true;

				Hud->ShowBannerText("L7chose");
			}
		}
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsLetterAmeehaTaken && !ProfileSystem::Instance->ActiveProfile.IsPaptusL7S3Collected && PaptusFlyRect.Intersection(&pos))
	{
		if(Control::Input->LBclicked())
		{
			Control::Audio->PlaySample(aSFXMagic2);
			GetObjectByID("paptus")->FadeOut(1.25f);
			Hud->CollectMysteryFlower(eArea_L7_S3_Haven, GetObjectByID("paptus")->Pos);
			return;
		}
	}

	//handle interactions here

	UIBtnUp->PlayAnimation(0);
	UIBtnDown->PlayAnimation(0);
	if(BtnRectDownButton.Intersection(&pos))
	{		
		UIBtnDown->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			UIBtnDown->PlayAnimation(0);
			StartSceneSlideOut(false);
			//Load Next One also
			if(LoadNextPopup!=nullptr)
				LoadNextPopup(this, true); //load Map Wall 1
		}
	}
	else if(BtnRectUpButton.Intersection(&pos))
	{
		UIBtnUp->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			StartSceneSlideOut(true);
			//Load Next One also
			if(LoadNextPopup!=nullptr)
				LoadNextPopup(this, false); //load Map Wall 3
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
	{
		if(InvItemRectBroom.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("broom")->SetEnable(false);

				ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken = true;
				Hud->TakeInventoryItem(eInv_L7_S3_Broom);
			}
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved)
	{
		if(InvDestRectYellowEmblem.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L7bull");
			}
		}
	}
	else
	{
		if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsLetterAmeehaTaken)
		{
			if (!isLetterZoomedIn)
			{
				if(ActionRectLetter.Intersection(&pos))
				{
					Cursor::SetMode(CA_Look);
					if(Control::Input->LBclicked())
					{
						Control::Audio->QuickLoadAndPlaySFX("s3p2_notezoomin");
						JAnim_LetterZoomin->PlayAnim();
						GetObjectByID("Mask")->FadeIn(1.5f);
						GetObjectByID("Mask")->SetEnable(true);
						isAnimationPlaying = true;
					}
				} 
			}
			else
			{
				
			}
		}
		else
		{
			if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
			{
				if(InvItemRectAxeKey.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandTake);
					if(Control::Input->LBclicked())
					{
						GetObjectByID("axekey")->SetEnable(false);

						ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken = true;
						Hud->TakeInventoryItem(eInv_L7_S3_AxeKey);
					}

					return;
				} 
			}

			if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken)
			{
				if(InvDestRectFlower.Intersection(&pos))
				{
					Cursor::SetMode(CA_Gear);
					if(Control::Input->LBclicked())
					{
						Hud->ShowBannerText("L7fresh");
					}
				}
			}
		}
	}
}

void L7S3_Pop2Bull::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		Hud->CancelDrag();
		return;
	}

	if (itemVal == eInv_L7_S1_YellowStarEmblem && !ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && InvDestRectYellowEmblem.Intersection(&dropPos))
	{
		ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved = true;

		isAnimationPlaying = true;
		Control::Audio->QuickLoadAndPlaySFX("s3p2_staruse");

		JAnim_BullSlide->PlayAnim();
		
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S1_YellowStarEmblem] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S1_YellowStarEmblem);
	}
	else if (itemVal == eInv_L7_S1_PotWithMagicWater && ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && 
		!ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && InvDestRectFlower.Intersection(&dropPos))
	{
		isAnimationPlaying = true;
		JAnim_FlowerOpen->PlayAnim();

		Control::Audio->QuickLoadAndPlaySFX("s3p2_waterused");
		
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		if (ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueFlowerTaken)
		{
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S1_PotWithMagicWater] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S1_PotWithMagicWater); 
		}
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L7S3_Pop2Bull::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if (isLetterZoomedIn)
	{
		Hud->InitHintTrailEmit(&CloseBtnRect, true);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectBroom);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[0])
	{
		Hud->ShowHintForInvItem(eInv_L7_S1_YellowStarEmblem, &InvDestRectYellowEmblem);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && !ProfileSystem::Instance->ActiveProfile.L7_S3_IsLetterAmeehaTaken)
	{
		if(isLetterZoomedIn)
			Hud->InitHintTrailEmit(&ActionRectLetterZoomed);
		else
			Hud->InitHintTrailEmit(&ActionRectLetter);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsLetterAmeehaTaken && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectAxeKey);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsLetterAmeehaTaken && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
	{
		Hud->ShowHintForInvItem(eInv_L7_S1_PotWithMagicWater, &InvDestRectFlower);
	}
	else
	{
		bool isTopPopupTaskLeft = false;
		if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
			isTopPopupTaskLeft = true;
		else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvLeverTaken)
			isTopPopupTaskLeft = true;
		else if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken)
			isTopPopupTaskLeft = true;

		if(isTopPopupTaskLeft)
		{
			//Shows Up Button
			Hud->InitHintTrailEmit(&BtnRectUpButton, true);
			return;
		}

		bool isBottomPopupTaskLeft = false;

		if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam && ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
			isBottomPopupTaskLeft = true;
		else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBroomUsedOnBhasmam && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken)
			isBottomPopupTaskLeft = true;

		if(isBottomPopupTaskLeft)
		{
			//Shows Down Button
			Hud->InitHintTrailEmit(&BtnRectDownButton, true);
			return;
		}

		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L7S3_Pop2Bull::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvBroomTaken)
	{
		isAnyTaskLeft = true;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[0])
	{
		isAnyTaskLeft = true;
	}//letter ameeha taken neglected
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
	{
		isAnyTaskLeft = true;
	}
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsBullMoved && !ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S1_InvMagicWaterTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L7S3_Pop2Bull::~L7S3_Pop2Bull()
{
	SAFE_DELETE(s3flowerfxFX);
	Control::Audio->UnloadSample(aVOL7NoteVOS3_1);
	SAFE_DELETE(bullSparkFX);
	SAFE_DELETE(JAnim_BullSlide);
	SAFE_DELETE(JAnim_LetterZoomin);
	SAFE_DELETE(JAnim_LetterZoomout);
	SAFE_DELETE(JAnim_FlowerOpen);
}
