//====================================
//DQFULP
// Combination Inventory Handling
//====================================

#include "ComboInventory.h"
#include "Hud.h"
#include "GameFlow.h"
#include "ProfileSystem.h"
#include "LangDefs.h"
#include "L3_S1_FortEntrance.h"

extern HudControl* GFHud;

const CRectangle  BtnRectClose(962.0f,47.0f,89.0f,84.0f);

const CRectangle  ActionRectZipOpen(625.0f,137.0f,51.0f,41.0f);
const CRectangle  InvItemRectKnife(508.0f,173.0f,148.0f,56.0f);
const CRectangle  InfoRectRuby(569.0f,237.0f,200.0f,165.0f);

const CRectangle  InvDestRectRubyParts(574.0f,229.0f,198.0f,178.0f);

const CRectangle  ActionRectBagLocketOpen(586.0f,208.0f,159.0f,108.0f);

const CRectangle  BigPopRectLocket(618.0f-100,221.0f-100,97.0f+200,76.0f+200);
const CRectangle  PopRectLocket(618.0f,221.0f,97.0f,76.0f);
const CRectangle  ActionRectKidDrawing(670.0f,327.0f,263.0f,197.0f);
const CRectangle  KidDrawingPuzzleRect(349.0f,123.0f,670.0f,495.0f);

const CRectangle  LeftArrowRect(216.0f,329.0f,156.0f,127.0f);
const CRectangle  RightArrowRect(1065.0f,205.0f,127.0f,137.0f);
const CRectangle  HelpRectPainting(417.0f,162.0f,583.0f,370.0f);

const CRectangle  ActionRectTearPaper(730.0f,92.0f,267.0f,202.0f);
const CRectangle  InvItemRectBeetlePart(795.0f,137.0f,153.0f,130.0f);
const CRectangle  InvItemRectBookCode(795.0f,137.0f,153.0f,130.0f);

const CRectangle  NextPageRect(964.0f,494.0f,81.0f,80.0f);
const CRectangle  PrevPageRect(298.0f,495.0f,87.0f,82.0f);
const CRectangle  MooppanInfoRect(370.0f,206.0f,245.0f,221.0f);
const CRectangle  SoldierInfoRect(718.0f,107.0f,258.0f,193.0f);

const CRectangle  DiaryItem1Rect(859.0f,302.0f,134.0f,144.0f);
const CRectangle  DiaryItem2Rect(822.0f,305.0f,106.0f,126.0f);
const CRectangle  DiaryItem3Rect(772.0f,363.0f,129.0f,125.0f);
const CRectangle  DiaryItem4Rect(772.0f,279.0f,263.0f,208.0f);
const CRectangle  DiaryItem5Rect(730.0f,326.0f,239.0f,173.0f);
const CRectangle  DiaryItemCreekNoticeRect(715.0f,219.0f,318.0f,355.0f);

const CRectangle  NoticeZoomRect(432.0f,18.0f,511.0f,677.0f);

const CRectangle  InvItemCoinRect(851.0f,370.0f,85.0f,79.0f);

const CRectangle  InvDestRectBeetleTail(435.0f,55.0f,470.0f,588.0f);
const CRectangle  InvDestInfoRectBeetle(435.0f,55.0f,349.0f,446.0f);

const CRectangle  InvItemRectCreekLetter(375.0f,6.0f,590.0f,618.0f);
const CRectangle  InvItemRect(635.0f,193.0f,230.0f,358.0f);

const CRectangle  InvDestRectHammerHead(428.0f,150.0f,523.0f,380.0f);
const CRectangle  InvDestRectCoconut(428.0f,150.0f,523.0f,380.0f);
const CRectangle  InvDestRectLadder(428.0f,150.0f,523.0f,380.0f);
const CRectangle  InvDestCoconutShell(407.0f,184.0f,539.0f,384.0f);
const CRectangle  InvItemRectBoxNagaKey(505.0f,289.0f,337.0f,258.0f);
const CRectangle  InvDestRectFlagCloth(619.0f,239.0f,204.0f,137.0f);

const CRectangle  InfoRectHammerHead(432.0f,103.0f,247.0f,387.0f);
const CRectangle  InvDestRectHammerHand(455.0f,114.0f,496.0f,456.0f);
const CRectangle  InvDestRectAxeSharpen(451.0f,60.0f,456.0f,462.0f);
const CRectangle  InvDestRectVinegarBottle(530.0f,230.0f,358.0f,263.0f);

const CRectangle  InvDestRectStick(580.0f,140.0f,201.0f,353.0f);
const CRectangle  InvDestRectBinocularLens(520.0f,144.0f,381.0f,283.0f);
const CRectangle  InvDestRectKnife(519.0f,133.0f,459.0f,369.0f);

const CRectangle InvDestRectBambooKnife(347.0f,68.0f,673.0f,579.0f);
const CRectangle InvDestRectBambooStrainer(347.0f,68.0f,673.0f,579.0f);
const CRectangle InvDestRectGrassRope(347.0f,68.0f,673.0f,579.0f);
const CRectangle InvDestRectHeartLocket(347.0f,68.0f,673.0f,579.0f);
const CRectangle InvDestRectRopeHorseShoe(347.0f,68.0f,673.0f,579.0f);

const CRectangle  InvDestRectL7Globe(330.0f,31.0f,709.0f,551.0f);
const CRectangle  InvDestRectL7PotMetalWire(297.0f,56.0f,729.0f,545.0f);
const CRectangle  ActionRectL7PotTryChain(278.0f,304.0f,367.0f,260.0f);

const CRectangle  InvDestRectL8MagicWandTool(359.0f,189.0f,550.0f,439.0f);
const CRectangle  InvDestRectL8ComboKey(359.0f,189.0f,550.0f,439.0f);

const CRectangle  ActionRectCorkOpen(365.0f,307.0f,90.0f,86.0f);
const CRectangle  ActionRectVinegarBottle(348.0f,350.0f,129.0f,211.0f);

//Level 5
const CRectangle  ActionRectBowStringFix(650.0f,348.0f,269.0f,133.0f);
const CRectangle  ActionRectBowArrowTake(312.0f,191.0f,288.0f,312.0f);
const CRectangle  InvDestRectBow(291.0f,96.0f,791.0f,442.0f);
const CRectangle  InvDestRectPickaxe(386.0f,60.0f,560.0f,483.0f);

const CRectangle  ActionRectLocket(450.0f,67.0f,492.0f,545.0f);


const CRectangle  BtnRectHint(1041.0f,617.0f,131.0f,108.0f); //handled bigger hint only, skip contained inside hint region!!  

const CPoint  PopCentre(683.0f,334.0f,0);

ComboInvSystem::ComboInvSystem()
{
	ComboScene = NULL;
	ComboPopupScene = NULL;
	ComboCloseBtn = NULL;

	ComboMaskBg = NULL;
	ComboPopMaskBg = NULL;

	PopCloseBtn = NULL;
	ComboBoundaryRect = NULL;

	clickDelayTimer = 0;

	_invId = -1;

	ComboPopState = JRN_IDLE;
	ComboState = JRN_IDLE;

	popOffsetX = popOffsetY = 0;
	IsPopTransitionActive = false;

	isAnyAnimActive = false;
	isMagicWandKeyInHand = false;
	Janim_MagicWand = NULL;
	
	isSkipTransition = false;

	PopupAlpha = 0.0f;
	SetZ(eZOrder_ComboPopup); // at same z order as popup

	Visible = false;
}

ComboInvSystem::~ComboInvSystem()
{
	HideComboInventoryPopup();
	Control::Audio->UnloadSample(aVOL1MaryRefS1_1);
	SAFE_DELETE(ComboScene);
	SAFE_DELETE(ComboPopupScene);
	SAFE_DELETE(ComboCloseBtn);
	SAFE_DELETE(ComboMaskBg);
	SAFE_DELETE(ComboPopMaskBg);
}

void ComboInvSystem::Required()
{
	if(!Visible)
		return;
	if(IsPopTransitionActive)
	{
		switch( ComboState )
		{
		case JRN_POPUP:
			PopupAlpha += POPUP_SCALE_FACTOR *  Control::LogicRate;
			if( PopupAlpha >= 1.0f )
			{
				IsPopTransitionActive = false;

				PopupAlpha = 1.0f;				
				ComboScene->Show(true);
				ComboState = JRN_POPPED;

				if( GFHud->Tutor != NULL && GFHud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT07_ZipperOpen] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
				{
					//User takes another key combination!!
					ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT06_BagZoomLook] = true;
					GFHud->Tutor->ShowTutorial(TUT07_ZipperOpen);
				}
				else if( GFHud->Tutor != NULL && GFHud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT19_ComboInvUseCombine])
				{
					//if beetle tail taken show tutorial
					if(_invId == eInv_L1S4_BeetleHead && ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken)
					{
						GFHud->Tutor->ShowTutorial(TUT19_ComboInvUseCombine);
					}
				}
			}
			ComboMaskBg->SetAlpha(PopupAlpha>1.0f?1.0f:PopupAlpha);
			ComboCloseBtn->SetAlpha(PopupAlpha>1.0f?1.0f:PopupAlpha);
			break;
		case JRN_POPDOWN:
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopTransitionActive = false;

				PopupAlpha = 0.0f;								
				ComboMaskBg->Show(false);
				ComboState = JRN_IDLE;

				SAFE_DELETE(ComboMaskBg);

				SendMessage(ActionProcessed, ID, HOSM_ComboInvClosed);

				Visible = false;
				return;
			}
			ComboMaskBg->SetAlpha(PopupAlpha>1.0f?1.0f:PopupAlpha);
			break;
		}
		switch( ComboPopState )
		{
		case JRN_POPUP:
			PopupAlpha += POPUP_SCALE_FACTOR *  Control::LogicRate;
			if( PopupAlpha >= 1.0f )
			{
				IsPopTransitionActive = false;

				PopupAlpha = 1.0f;				
				ComboPopupScene->Show(true);
				ComboPopState = JRN_POPPED;
			}
			ComboPopMaskBg->SetAlpha(PopupAlpha);
			break;
		case JRN_POPDOWN:
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopTransitionActive = false;

				ComboPopMaskBg->Show(false);

				PopupAlpha = 0.0f;								
				ComboPopState = JRN_IDLE;
			}
			ComboPopMaskBg->SetAlpha(PopupAlpha);
			break;
		}
	}
}

void ComboInvSystem::Render(const CPoint* pCam)
{  
	if(!Visible || !IsPopTransitionActive)
		return;

	//Draw render target object in Render as last object!!!
	if( ComboState == JRN_POPUP || ComboState == JRN_POPDOWN  || ComboPopState == JRN_POPUP || ComboPopState == JRN_POPDOWN)
	{
		//Init again if render target lost due to fullscreen-window switches or similar
        if( GFInstance->RTGraphic != NULL )
			GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
			short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
			short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
			-5*(1.0f-PopupAlpha),
			PopupAlpha,PopupAlpha);
    }
}

void ComboInvSystem::Update()
{
	if( !Visible || IsPopTransitionActive || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	if( GFHud->Tutor != NULL && GFHud->Tutor->IsActive)
	{
		if(GFHud->Tutor->IsVisible && GFHud->Tutor->State == TUT07_ZipperOpen)
		{
			if(_invId == eInv_L1S1_Bag)
			{
				Cursor::SetMode(CA_Normal);
				if( !ProfileSystem::Instance->ActiveProfile.L1S1_BagZipOpened )
				{
					if ( BagZipperPoly->Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandPointy);
						if( Control::Input->LBclicked())
						{
							ProfileSystem::Instance->ActiveProfile.L1S1_BagZipOpened = true;

							ComboScene->GetObjectByID("knifemask")->SetEnable(true);
							bag_ZipOpened->SetEnable(true);
							bag_Knife->SetEnable(true);

							ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT07_ZipperOpen] = true;
							GFHud->Tutor->ShowTutorial(TUT08_KnifeTakeFromBag);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BagZipSFX);
							CAnim_ZipperOpen->PlayAnim();
						} 
					}
				}
			}
			return;
		}
		else if(GFHud->Tutor->IsVisible && GFHud->Tutor->State == TUT08_KnifeTakeFromBag)
		{
			if(_invId == eInv_L1S1_Bag)
			{
				Cursor::SetMode(CA_Normal);
				if( !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken && InvItemRectKnife.Intersection(&pos) )
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked() )
					{
						ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken = true;

						ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT08_KnifeTakeFromBag] = true;
						GFHud->Tutor->HideTutorial(true);

						std::vector<int> vecInvIds;

						GFHud->InvBoxCtrl->Activate(1);			
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_SwissKnife] = true;
						ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken = true;
						vecInvIds.push_back(eInv_L1S1_SwissKnife);
						GFHud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());

						bag_Knife->SetEnable(false);

						GFHud->InvBoxCtrl->updateComboItemStatus(_invId);
					}
				}
			}
			return;
		}
	}

	Cursor::SetMode(CA_Normal);
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
			if(!GFHud->IsDraggingItem() && ComboPopState == JRN_IDLE && !isAnyAnimActive)
			{
				if ( _invId == eInv_L1S1_Bag && !ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingAddedToJournal && ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete)
				{
					//Force Do Not Close Until player take the Kids Painting
				}
				else
				{
					Cursor::SetMode(CA_HandPointy);
					HideComboInventoryPopup();
					return;
				}
			}
		}
	#endif
	if(!GFHud->InvBoxCtrl->IsHoverItem && !GFHud->IsDraggingItem() && ComboPopState == JRN_IDLE && BtnRectClose.Intersection(&pos) && !isAnyAnimActive)
	{
		if ( _invId == eInv_L1S1_Bag && !ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingAddedToJournal && ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete)
		{
			//Force Do Not Close Until player take the Kids Painting
		}
		else
		{
			Cursor::SetMode(CA_HandPointy);
			ComboCloseBtn->PlayAnimation(1);
			if(clicked)
			{
				HideComboInventoryPopup();
				return;
			}
		}
	}
	else
	{
		ComboCloseBtn->PlayAnimation(0);
		if ( _invId == eInv_L1S1_Bag && !ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingAddedToJournal && ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete)
		{
			//Force Do Not Close Until player take the Kids Painting
		}
		else if(!GFHud->InvBoxCtrl->IsHoverItem && !GFHud->IsDraggingItem() && !isAnyAnimActive && ComboPopState == JRN_IDLE && ComboBoundaryRect!=NULL && !ComboBoundaryRect->Intersection(&pos) && !BtnRectHint.Intersection(&pos))
		{
			ComboCloseBtn->PlayAnimation(1);
			if(clicked)
			{
				HideComboInventoryPopup();
				return;
			}
		}
	}

	switch (_invId)
	{
	case eInv_L1S1_Bag:
		if(ComboPopState == JRN_POPPED)
		{
			if(ruby_FullPart->IsTweening)
			{
				if(ruby_FullPart->IsTweenFinished())
				{
					PopupAlpha = 1.0f;

					ComboPopState = JRN_POPDOWN;
					InitRenderTarget(ComboPopupScene);
					GFInstance->SetPopupTween( 0, -50, 171, 0);
					IsPopTransitionActive = true;

					ComboPopupScene->Show(false);
				}
				return;
			}

			//close btn //TODO Inv Item click closes this popup. So a simple hack here. -faisal
			if( !GFHud->InvBoxCtrl->IsHoverItem && !GFHud->IsDraggingItem() && (Elixir::Distance(&pos,&PopCentre) > 240 || PopCloseBtn->Collide(&pos)) && (!CRectangle(1000,570,200,160).Intersection(&pos)))
			{
				PopCloseBtn->PlayAnimation(1);
				if(PopCloseBtn->Collide(&pos))
					Cursor::SetMode(CA_HandPointy);
				if(clicked)
				{
					PopupAlpha = 1.0f;

					ComboPopState = JRN_POPDOWN;
					InitRenderTarget(ComboPopupScene);
					GFInstance->SetPopupTween( 0, -50, 171, 0);
					IsPopTransitionActive = true;

					ComboPopupScene->Show(false);
				}
			}
			else
			{
				PopCloseBtn->PlayAnimation(0);

				if(!ProfileSystem::Instance->ActiveProfile.L1S1_LocketPartsPlaced)
				{
					//Show message banner causing problems with clear input
					if( !GFHud->IsDraggingItem() && InvDestRectRubyParts.Intersection(&pos) )
					{
						Cursor::SetMode(CA_Gear);
						if( clicked )
						{
							GFHud->ShowBannerText("L1broken");
						}
					}
				}
				else
				{
					if(activeRubyPart != -1)
					{
						Cursor::SetMode(CA_HandTake);

						ruby_Parts[activeRubyPart]->PlayAnimation(0);
						ruby_Parts[activeRubyPart]->Pos.x = pos.x;
						ruby_Parts[activeRubyPart]->Pos.y = pos.y;
						if( ruby_destRects[activeRubyPart]->Intersection(&pos) )
						{
							ruby_Parts[activeRubyPart]->PlayAnimation(1);
							if( clicked )
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BagLockPartFixed);

								ruby_Parts[activeRubyPart]->SetPos(ruby_FixPos[activeRubyPart]);
								ruby_IsPartFixed[activeRubyPart] = true;

								ruby_Parts[activeRubyPart]->SetZ(ruby_Parts[activeRubyPart]->ZOrder-4);


								//fix and save
								if(activeRubyPart == 0)		ProfileSystem::Instance->ActiveProfile.L1S1_LocketPart1Fixed = true;
								else if(activeRubyPart == 1)ProfileSystem::Instance->ActiveProfile.L1S1_LocketPart2Fixed = true;
								else if(activeRubyPart == 2)ProfileSystem::Instance->ActiveProfile.L1S1_LocketPart3Fixed = true;
								else if(activeRubyPart == 3)ProfileSystem::Instance->ActiveProfile.L1S1_LocketPart4Fixed = true;

								bool isAllPartsFixed = true;
								for (int i = 0; i < 4; i++)
								{
									if(!ruby_IsPartFixed[i])
										isAllPartsFixed = false;
								}

								if(isAllPartsFixed)
								{
									Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BagLatchOpen);

									bag_LocketFixed->SetEnable(true);
									ProfileSystem::Instance->ActiveProfile.L1S1_LocketAllPartsFixed = true;
									ruby_FullPart->SetAlpha(0);
									ruby_FullPart->SetEnable(true);
									float vals[5] = {1.0f,-1,-1,-1,-1};
									ruby_FullPart->TweenTo(vals, 0.65f, TW_EASE, false);
								}

								activeRubyPart = -1;
							}
						}
						else
						{
							if( clicked )
							{
								ruby_Parts[activeRubyPart]->SetPos(&ruby_Parts[activeRubyPart]->InitPos);
								activeRubyPart = -1;
							}
						}
					}
					else
					{
						for (int i = 0; i < 4; i++)
						{
							if ( !ruby_IsPartFixed[i] && ruby_takeRects[i]->Intersection(&pos) )
							{
								Cursor::SetMode(CA_HandTake);
								if(clicked)
								{
									Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BagLockPartPicked);
									activeRubyPart = i;
									ruby_Parts[activeRubyPart]->SetZ(ruby_Parts[activeRubyPart]->ZOrder+4);
								}
								break;
							}
						}
					} 
				}
			}
		}
		else
		{
			if (GFHud->InvBoxCtrl->InvBoxObj[GFHud->InvBoxCtrl->DragIndex].HOObj)
			{
				if (GFHud->InvBoxCtrl->InvBoxObj[GFHud->InvBoxCtrl->DragIndex].HOObj->PublicValue == eInv_L1S1_RubyPart1)
				{
					if (PopRectLocket.Intersection(&GFHud->InvBoxCtrl->InvBoxObj[GFHud->InvBoxCtrl->DragIndex].HOObj->Pos))
					{
						ComboPopMaskBg->SetAlpha(0);
						ComboPopMaskBg->Show(true);

						PopupAlpha = 0.2f;
						ComboPopState = JRN_POPUP;
						IsPopTransitionActive = true;
						InitRenderTarget(ComboPopupScene);
						GFInstance->SetPopupTween(0, -50, 171, 0);

						PopCloseBtn = ComboPopupScene->GetObjectByID("popClose");
					}
				}
			}
			if(IsKidDrawingAddingToJournal)
			{
				if(bag_KidsDrawing->IsTweening && bag_KidsDrawing->IsTweenFinished())
				{
					bag_KidsDrawing->IsTweening = false;
					bag_KidsDrawing->SetEnable(false);
					IsKidDrawingAddingToJournal = false;

					ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingAddedToJournal = true;

					//Journal Entry MooppanFlyer
					if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P4_KidPainting])
					{
						//Flag Set
						ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P4_KidPainting] = true;
						//Add to Note Array
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
						= eJrn_L1P4_KidPainting;

						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
						//Increment Notes
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
						GFHud->ShowJrnEntryFX();
					}
				}
				return;
			}
			if(IsKidDrawingZooming)
			{
				if(CAnim_kidDrawingSlide1->IsPlaying())
				{
					if(CAnim_kidDrawingSlide1->IsAnimFinished())
					{
						//set to top. Over the shadows and Lock Mask
						CAnim_kidDrawingSlide1->Stop();
						bag_KidsDrawing->SetZ(bag_CoverClosed->ZOrder+2);
						CAnim_kidDrawingSlide2->PlayAnim();
					}
				}
				else if(CAnim_kidDrawingSlide2->IsPlaying())
				{
					if(CAnim_kidDrawingSlide2->IsAnimFinished())
					{
						IsKidDrawingZooming = false;
						IsKidDrawingZoomed = true;

						CAnim_kidDrawingSlide2->Stop();

						HideComboInventoryPopup(true);

						ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingPuzzleActive = true;

						ProfileSystem::Instance->ActiveProfile.PrevComboActiveArea =  ProfileSystem::Instance->ActiveProfile.CurrentArea;
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_PZKidsPainting;

						GFHud->SendNewAreaMessage();
					}
				}
				return;
			}
			if( !ProfileSystem::Instance->ActiveProfile.L1S1_BagZipOpened )
			{
				if ( BagZipperPoly->Intersection(&pos) )
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked() )
					{
						ProfileSystem::Instance->ActiveProfile.L1S1_BagZipOpened = true;

						ComboScene->GetObjectByID("knifemask")->SetEnable(true);
						bag_ZipOpened->SetEnable(true);
						bag_Knife->SetEnable(true);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BagZipSFX);
						CAnim_ZipperOpen->PlayAnim();
					} 
				}
			}
			else if( !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken && InvItemRectKnife.Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked() )
				{
					ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken = true;

					std::vector<int> vecInvIds;

					GFHud->InvBoxCtrl->Activate(1);			
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_SwissKnife] = true;
					ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken = true;
					vecInvIds.push_back(eInv_L1S1_SwissKnife);
					GFHud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());

					bag_Knife->SetEnable(false);

					GFHud->InvBoxCtrl->updateComboItemStatus(_invId);
				}
			}

			if(isCreekletterZooming)
			{
				if(bag_CreekLetterZoomed->IsTweenFinished())
				{
					bag_CreekLetterZoomed->IsTweening = false;
					bag_CreekLetterZoomedText->IsTweening = false;
					isCreekletterZooming = false;
					isAnyAnimActive = false;

					isCreekletterZoomedIn = true;
				}
				return;
			}

			if( !ProfileSystem::Instance->ActiveProfile.L1S1_LocketAllPartsFixed )
			{
				if ( PopRectLocket.Intersection(&pos) )
				{
					Cursor::SetMode(CA_Look);
					if( clicked )
					{
						ComboPopMaskBg->SetAlpha(0);
						ComboPopMaskBg->Show(true);

						PopupAlpha = 0.2f;
						ComboPopState = JRN_POPUP;
						IsPopTransitionActive = true;
						InitRenderTarget(ComboPopupScene);
						GFInstance->SetPopupTween( 0, -50, 171, 0);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BagLockPopupSFX);

						PopCloseBtn = ComboPopupScene->GetObjectByID("popClose");
						return;
					} 
				}
			}
			else
			{
				//After Heart Locket Fixed
				if( !ProfileSystem::Instance->ActiveProfile.L1S1_BagOpened )
				{
					if( ActionRectBagLocketOpen.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						bag_LocketFixed->PlayAnimation(1);
						if(clicked)
						{
							ProfileSystem::Instance->ActiveProfile.L1S1_BagOpened = true;
							bag_LocketFixed->PlayAnimation(2);

							//bag_InvCreeksLetter->SetEnable(true);
							bag_CoverOpened->SetEnable(true);
							bag_CoverOpened->FadeIn(4.0f);
							bag_KidsDrawing->SetEnable(true);
							bag_KidsDrawing->FadeIn(4.0f);

							ComboScene->GetObjectByID("bagopenshadow")->SetEnable(true);
							ComboScene->GetObjectByID("bagopenshadow")->FadeIn(4.0f);

							bag_CoverClosed->FadeOut(2.0f);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BagOpen2);							

							IsbagOpeningPlaying = true;
						}
					}
					else
					{
						bag_LocketFixed->PlayAnimation(0);
					}
				}
				else
				{
					//Bag opened
					//Zoom in the drawing
					if (!ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete && !IsKidDrawingZoomed)
					{
						if(ActionRectKidDrawing.Intersection(&pos))
						{
							if(ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingPuzzleActive)
								Cursor::SetMode(CA_Look);
							else
								Cursor::SetMode(CA_HandTake);
							if(clicked)
							{
								if(ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingPuzzleActive)
								{
									HideComboInventoryPopup(true);

									ProfileSystem::Instance->ActiveProfile.PrevComboActiveArea =  ProfileSystem::Instance->ActiveProfile.CurrentArea;
									ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_PZKidsPainting;

									GFHud->SendNewAreaMessage();
								}
								else
								{
									Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1KidsPaintingClicked);

									//first time
									IsKidDrawingZooming = true;
									CAnim_kidDrawingSlide1->PlayAnim();
								}
							}
						} 
					}
					else
					{
						if ( !ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingAddedToJournal && ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete)
						{
							if(KidDrawingPuzzleRect.Intersection(&pos))
							{
								Cursor::SetMode(CA_HandTake);
								if(clicked)
								{
									//Add to Journal
									float valArr[5] = {0, 259, 700, 0.25f, 60.0f};
									bag_KidsDrawing->TweenTo(valArr, 1.0f, TW_EASE, false);

									IsKidDrawingAddingToJournal = true;
									Control::Audio->QuickLoadAndPlaySFX(aSFXJrnlFlyer);
								}
							}
						}
						else if( !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvLetterCreekTaken )
						{
							if(isCreekletterZoomedIn)
							{
								if(InvItemRectCreekLetter.Intersection(&pos))
								{
									Cursor::SetMode(CA_HandTake);
									if(clicked)
									{
										bag_CreekLetterZoomed->SetEnable(false);
										bag_CreekLetterZoomedText->SetEnable(false);

										//Bag is finished
										ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_Bag] = false;
										GFHud->ReleaseDrag();			
										GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S1_Bag);

										std::vector<int> vecInvIds;

										GFHud->InvBoxCtrl->Activate(1);
										ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_CreekLetter] = true;
										ProfileSystem::Instance->ActiveProfile.L1S1_IsInvLetterCreekTaken = true;
										vecInvIds.push_back(eInv_L1S1_CreekLetter);
										GFHud->ShowInventory(&vecInvIds);

										HideComboInventoryPopup(true);
									}
								}
							}
							else if(ActionRectKidDrawing.Intersection(&pos))
							{
								Cursor::SetMode(CA_Look);
								if(clicked)
								{									
									isCreekletterZooming = true;
									isAnyAnimActive = true;

									bag_InvCreeksLetter->SetEnable(false);

									bag_CreekLetterZoomed = new CObject();
									bag_CreekLetterZoomed->LoadInit("INV\\L1\\S1BAGPOP\\CREEKLETTERZOOM.SPR", 791, 438, eZOrder_ComboInvScene+30);

									bag_CreekLetterZoomed->SetScale(0.15f);
									bag_CreekLetterZoomed->SetRotate(45.0f);
									bag_CreekLetterZoomed->SetAlpha(0);

									float valArr[] = {1.0f, 677, 321, 1.0f, 0.0f};
									bag_CreekLetterZoomed->TweenTo(valArr, 0.75f, 0, false);


									bag_CreekLetterZoomedText = new CObject();
									bag_CreekLetterZoomedText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S1\\CREEKLETTERMARY.SPR", 791, 438, eZOrder_ComboInvScene + 31);

									bag_CreekLetterZoomedText->SetScale(0.15f);
									bag_CreekLetterZoomedText->SetRotate(45.0f);
									bag_CreekLetterZoomedText->SetAlpha(0);

									//float valArr[] = { 1.0f, 677, 321, 1.0f, 0.0f };
									bag_CreekLetterZoomedText->TweenTo(valArr, 0.75f, 0, false);
									


								}
							}
						}
					}
				}
			}
		}
		break;
	case eInv_L1S4_PhotoFrame:
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoTurned)
			{
				photo_leftArrow->PlayAnimation(0);
				photo_rightArrow->PlayAnimation(0);
				if( LeftArrowRect.Intersection(&pos) || RightArrowRect.Intersection(&pos))
				{
					if(LeftArrowRect.Intersection(&pos))
						photo_leftArrow->PlayAnimation(1);
					else
						photo_rightArrow->PlayAnimation(1);

					Cursor::SetMode(CA_HandTake);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4PotraitTurned);

						photo_leftArrow->SetEnable(false);
						photo_rightArrow->SetEnable(false);
						photo_bgfront->SetEnable(false);

						photo_paperOverlay->SetEnable(true);
						photo_bgback->SetEnable(true);
						photo_BeetlePart->SetEnable(true);
						photo_SecretCode->SetEnable(true);						

						ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoTurned = true;
					}
				}
			}
			else
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoPaperTeared)
				{
					if( ActionRectTearPaper.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4PortraitTear);

							photo_paperOverlay->SetEnable(false);	

							ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoPaperTeared = true;
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken)
				{
					if( InvItemRectBeetlePart.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							photo_BeetlePart->SetEnable(false);	

							std::vector<int> vecInvIds;

							GFHud->InvBoxCtrl->Activate(1);
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_BeetleTail] = true;
							ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken = true;
							vecInvIds.push_back(eInv_L1S4_BeetleTail);
							GFHud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
							return;
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvBookCodeTaken)
				{
					if( InvItemRectBookCode.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							photo_SecretCode->SetEnable(false);

							//Photoframe done
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_PhotoFrame] = false;
							GFHud->ReleaseDrag();			
							GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S4_PhotoFrame);

							std::vector<int> vecInvIds;

							GFHud->InvBoxCtrl->Activate(1);
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_BookCode] = true;
							ProfileSystem::Instance->ActiveProfile.L1S4_InvBookCodeTaken = true;
							vecInvIds.push_back(eInv_L1S4_BookCode);
							GFHud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());

							HideComboInventoryPopup(true);
						}
					}
				}

			}
		}
		break;
	case eInv_L1S4_Diary:
		{
			if(isCreekNoticeZooming)
			{
				if(diary_creekNotice->IsTweenFinished())
				{
					isCreekNoticeZooming = false;
					isCreekNoticeZoomedIn = true;
					GFHud->ShowBannerText("L1warn");
					diary_creekNotice->IsTweening = false;
					creekLetterText->IsTweening = false;
				}
				return;
			}
			else if(isCreekNoticeGoingToJournal)
			{
				if(diary_creekNotice->IsTweenFinished())
				{
					isCreekNoticeGoingToJournal = false;

					ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryCreekNoticeTaken = true;

					diary_creekNotice->IsTweening = false;
					diary_creekNotice->SetEnable(false);

					creekLetterText->IsTweening = false;
					creekLetterText->SetEnable(false);

					//Journal Entry 11 Creeks Letter
					if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P11_CompanyWarningNotice])
					{
						//Flag Set
						ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P11_CompanyWarningNotice] = true;
						//Add to Note Array
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
						= eJrn_L1P11_CompanyWarningNotice;

						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
						//Increment Notes
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;

						//Also Take the next one also - Journal Entry 12
						//Flag Set
						ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P12_CreekDiaryNotes] = true;
						//Add to Note Array
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
						= eJrn_L1P12_CreekDiaryNotes;

						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
						//Increment Notes
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;


						GFHud->ShowJrnEntryFX();
					}
				}
				return;
			}

			if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryPageTurned)
			{
				if( NextPageRect.Intersection(&pos) )
				{
					diary_nextPagebtn->SetEnable(true);
					Cursor::SetMode(CA_HandTake);
					if(clicked)
					{
						diary_prevPagebtn->Show(true);
						diary_nextPagebtn->FadeOut(2.0f);
						diary_page1->FadeOut(2.0f);

						diary_page1_text->FadeOut(2.0f);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4DiaryPageTurn);

						ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryPageTurned = true;
					}
				}
				else 
					diary_nextPagebtn->SetEnable(false);

				if( MooppanInfoRect.Intersection(&pos) )
				{					
					Cursor::SetMode(CA_Help);
					if(clicked)
					{
						GFHud->ShowBannerText("L1shaman");
					}
				}
				else if( SoldierInfoRect.Intersection(&pos) )
				{					
					Cursor::SetMode(CA_Help);
					if(clicked)
					{
						GFHud->ShowBannerText("L1soldier");
					}
				}
			}
			else
			{
				if( PrevPageRect.Intersection(&pos) )
				{
					diary_prevPagebtn->SetEnable(true);
					Cursor::SetMode(CA_HandTake);
					if(clicked)
					{
						diary_nextPagebtn->Show(true);
						diary_prevPagebtn->SetEnable(false);
						diary_page1->SetEnable(true);
						diary_page1->FadeIn(2.0f);

						diary_page1_text->SetEnable(true);
						diary_page1_text->FadeIn(2.0f);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4DiaryPageTurn);

						ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryPageTurned = false;
						return;
					}
				}
				else 
					diary_prevPagebtn->SetEnable(false);

				//page turned
				if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem1Removed)
				{
					if( DiaryItem1Rect.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4DiaryStampSlide);

							float valArr[] = {0, diary_item1->Pos.x+150.0f, diary_item1->Pos.y+150.0f, 1.1f, 25.0f};
							diary_item1->TweenTo(valArr, 0.65f, 0, true);

							ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem1Removed = true;
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem2Removed)
				{
					if( DiaryItem2Rect.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4DiaryStampSlide);

							float valArr[] = {0, diary_item2->Pos.x-150.0f, diary_item2->Pos.y+150.0f, 1.1f, -25.0f};
							diary_item2->TweenTo(valArr, 0.65f, 0, true);

							ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem2Removed = true;
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem3Removed)
				{
					if( DiaryItem3Rect.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4DiaryCurrencySlide);

							float valArr[] = {0, diary_item3->Pos.x+75.0f, diary_item3->Pos.y-100.0f, 1.1f, -25.0f};
							diary_item3->TweenTo(valArr, 0.65f, 0, true);

							ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem3Removed = true;
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem4Removed)
				{
					if( DiaryItem4Rect.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4DiaryCurrencySlide);

							float valArr[] = {0, diary_item4->Pos.x-75.0f, diary_item4->Pos.y-100.0f, 1.1f, 25.0f};
							diary_item4->TweenTo(valArr, 0.65f, 0, true);

							ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem4Removed = true;
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem5Removed)
				{
					if( DiaryItem5Rect.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4DiaryStampSlide);

							float valArr[] = {0, diary_item5->Pos.x+75.0f, diary_item5->Pos.y+100.0f, 1.1f, 25.0f};
							diary_item5->TweenTo(valArr, 0.65f, 0, true);

							ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem5Removed = true;
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryCreekNoticeTaken)
				{
					if(isCreekNoticeZoomedIn)
					{
						if( NoticeZoomRect.Intersection(&pos) )
						{
							Cursor::SetMode(CA_HandTake);
							if(clicked)
							{
								isCreekNoticeGoingToJournal = true;
								Control::Audio->QuickLoadAndPlaySFX(aSFXJrnlFlyer);

								float valArr[] = {0.1f, 330.0f, 620.0f, 0.21f, 25.0f};
								diary_creekNotice->TweenTo(valArr, 1.15f, 0, false);

								creekLetterText->TweenTo(valArr, 1.15f, 0, false);
							}
						}
					}
					else if(!isCreekNoticeZooming && DiaryItemCreekNoticeRect.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							isCreekNoticeZooming = true;

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4DiaryNotePicked);

							float valArr[] = {1, 683.0f, 360.0f, 1.0f, 0.0f};
							diary_creekNotice->TweenTo(valArr, 0.65f, 0, false);		

							creekLetterText->TweenTo(valArr, 0.65f, 0, false);
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvCoinTaken)
				{
					if( InvItemCoinRect.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if(clicked)
						{
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4CoinPicked);

							diary_coin->SetEnable(false);	

							//Diary done
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_Diary] = false;
							GFHud->ReleaseDrag();			
							GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S4_Diary);

							std::vector<int> vecInvIds;

							GFHud->InvBoxCtrl->Activate(1);
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_Coin] = true;
							ProfileSystem::Instance->ActiveProfile.L1S4_InvCoinTaken = true;
							vecInvIds.push_back(eInv_L1S4_Coin);
							GFHud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());

							HideComboInventoryPopup(true);
						}
					}
				}
			}
		}
		break;
	case eInv_L1S4_BeetleHead:
		if(isBeetleJoinPaying)
		{
			if(CAnim_BeetleJoin->IsAnimFinished())
			{
				isBeetleJoinPaying = false;
				isAnyAnimActive = false;

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_BeetleHead] = false;
				GFHud->ReleaseDrag();			
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S4_BeetleHead);

				std::vector<int> vecInvIds;

				GFHud->InvBoxCtrl->Activate(1);
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1CB_Beetle] = true;
				ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleTailFixed = true;
				vecInvIds.push_back(eInv_L1CB_Beetle);
				GFHud->ShowInventory(&vecInvIds);

				HideComboInventoryPopup(true);
			}
			return;
		}
		if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleTailFixed)
		{
			if( !GFHud->IsDraggingItem() && InvDestInfoRectBeetle.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L1missing");
					ComboScene->GetObjectByID("beetlehead")->Shake(225,10,0,0.25f);
				}
			}
		}
		break;
	case eInv_L1S5_HammerHead:
		if(isHammerJoinPaying)
		{
			if(CAnim_HammerJoin!=NULL && CAnim_HammerJoin->IsAnimFinished())
			{
				isHammerJoinPaying = false;
				isAnyAnimActive = false;

				//Hammer done
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_HammerHead] = false;
				GFHud->ReleaseDrag();
				GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S5_HammerHead);

				std::vector<int> vecInvIds;

				GFHud->InvBoxCtrl->Activate(1);
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1CB_Hammer] = true;
				ProfileSystem::Instance->ActiveProfile.L1S5_IsHammerHandFixed = true;
				vecInvIds.push_back(eInv_L1CB_Hammer);
				GFHud->ShowInventory(&vecInvIds);

				HideComboInventoryPopup();
			}
			return;
		}
		if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsHammerHandFixed && !GFHud->IsDraggingItem())
		{
			if( InfoRectHammerHead.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L1work");
					ComboScene->GetObjectByID("hammerhead")->Shake(225,10,0,0.25f);
				}
			}
		}
		break;
	case eInv_L2S4_AxeBlunt:
		if(isAxeSharpenAnimPlaying)
		{
			return;
		}

		if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeSharpenTaken)		
			HideComboInventoryPopup();

		if(!ProfileSystem::Instance->ActiveProfile.L2_S4_IsAxeSharpened)
		{
			if( !GFHud->IsDraggingItem() && InvDestRectAxeSharpen.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L2blunt");
				}
			}
		}
		break;
	case eInv_L2S6_KnifeBlunt:
		if(isAnyAnimActive)
		{
			return;
		}

		if(ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken)		
			HideComboInventoryPopup();

		if(!ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarPlaced)
		{
			if( !GFHud->IsDraggingItem() && InvDestRectVinegarBottle.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L2old");
				}
			}
		}
		else
		{
			if(!ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarCorkOpened)
			{				
				if( !GFHud->IsDraggingItem() && ActionRectCorkOpen.Intersection(&pos) )
				{
					Cursor::SetMode(CA_HandTake);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX("combo_knifeclean1");
						ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarCorkOpened = true;
						JAnim_CorkRemove->PlayAnim();
						isAnyAnimActive = true;
					}
				}
			}
			else
			{				
				if( !GFHud->IsDraggingItem() && ActionRectVinegarBottle.Intersection(&pos) )
				{
					Cursor::SetMode(CA_HandTake);
					if(clicked)
					{
						Control::Audio->QuickLoadAndPlaySFX("combo_knifeclean3");
						ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpened = true;
						JAnim_RustRemove->PlayAnim();
						isAnyAnimActive = true;
					}
				}
			}

		}
		break;

	case eInv_L3S1_FlagFull:
		{
			if(isAnyAnimActive)
				return;

			if (ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagStickTaken)
			{
				HideComboInventoryPopup();
				return;
			}

			if (ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagClothTaken)
			{
				ComboScene->GetObjectByID("Flag_stick")->FadeOut(0.8f);
				ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagStickTaken = true;
				GFHud->TakeInventoryItem(eInv_L3S1_Spear); 
			}

			if (InvItemRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if (Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("combo_flagclothtaken");
					JAnim_FlagComboRemove->PlayAnim();
					isAnyAnimActive = true;
				}
			}
		}
		break;

	case eInv_L3S1_LadderBroken:
		if(ladderClothFadingIn)
		{
			clothTieTimer += Control::LogicRate;
			if(clothTieTimer>1.25f)
			{
				ladderClothFadingIn = false;
				isAnyAnimActive = false;
				if (ProfileSystem::Instance->ActiveProfile.L3_S1_LadderPiecesFixed)
				{
					//Broken Ladder done
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S1_LadderBroken] = false;
					GFHud->ReleaseDrag();
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S1_LadderBroken);

					ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderFixedTaken = true;
					ComboScene->SetSceneAlpha(0);
					GFHud->TakeInventoryItem(eInv_L3S1_LadderFixed);
				}
			}
			return;
		}

		if(isAnyAnimActive)
		{
			return;
		}

		if(ProfileSystem::Instance->ActiveProfile.L3_S1_LadderBrokenStepFixed && ProfileSystem::Instance->ActiveProfile.L3_S1_LadderPiecesFixed)
		{
			HideComboInventoryPopup();
			return;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderBrokenStepFixed)
		{
			if( !GFHud->IsDraggingItem() && InvDestRectFlagCloth.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderPiecesFixed)
						GFHud->ShowBannerText("L3steps");
					else
						GFHud->ShowBannerText("L3mend");
				}
				return;
			}
		}

		if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderPiecesFixed)
		{
			if( !GFHud->IsDraggingItem() && InvDestRectLadder.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderBrokenStepFixed)
						GFHud->ShowBannerText("L3steps");
					else
						GFHud->ShowBannerText("L3until");
				}
			}
		}
		break;

	case eInv_L3S2_CoconutFull:
		{
			if(isAnyAnimActive)
				return;

			if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutShellTaken)
			{
				HideComboInventoryPopup();
				return;
			}

			if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved)
			{
				if( !GFHud->IsDraggingItem() && InvDestCoconutShell.Intersection(&pos) )
				{
					Cursor::SetMode(CA_Gear);
					if(clicked)
					{
						GFHud->ShowBannerText(Banner_L3_S3_BeforeCounetOuterRemove);
					}
				}
			}
			else if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved && 
				!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutShellTaken)
			{
				if( !GFHud->IsDraggingItem() && InvDestCoconutShell.Intersection(&pos) )
				{
					Cursor::SetMode(CA_Gear);
					if(clicked)
					{
						GFHud->ShowBannerText(Banner_L3_S3_AfterCocunetOuterRemoved);
					}
				}
			}

		}
		break;

	case eInv_L3S2_CocoShellPowderFilled:
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutWithTwineTaken)		
			HideComboInventoryPopup();

		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_CoconutComboTwineFixed)
		{
			if( !GFHud->IsDraggingItem() && InvDestCoconutShell.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L3twine");
				}
			}
		}
		break;
	case eInv_L3S2_HammerHead:
		{
			if(isAnyAnimActive)
				return;

			if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerTaken)
			{
				//GFHud->ShowBannerDirectText("DEBUG: Closing ComboInventory Popup here ...");
				HideComboInventoryPopup();
				return;
			}
		}
		break;

	case eInv_L3S3_Box:
		if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvSnakeDoorPartTaken)
			HideComboInventoryPopup();

		if(isAnyAnimActive)
		{
			clickDelayTimer -= Control::LogicRate;
			if(clickDelayTimer<0)
				isAnyAnimActive = false;
			return;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvSnakeDoorPartTaken)
		{
			if( !GFHud->IsDraggingItem() && InvItemRectBoxNagaKey.Intersection(&pos) )
			{
				if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsSecretBoxOpened)
				{
					Cursor::SetMode(CA_HandTake);
					if(clicked)
					{
						clickDelayTimer=1.0f;
						isAnyAnimActive = true;

						Control::Audio->QuickLoadAndPlaySFX("combo_chestopen");

						ProfileSystem::Instance->ActiveProfile.L3_S3_IsSecretBoxOpened = true;

						ComboScene->GetObjectByID("boxopened")->SetEnable(true);
						ComboScene->GetObjectByID("boxopened")->FadeIn(2.0f);
						ComboScene->GetObjectByID("boxclosed")->FadeOut(2.0f);
					}
				}
				else
				{
					Cursor::SetMode(CA_HandTake);
					if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvSnakeDoorPartTaken && clicked)
					{
						ProfileSystem::Instance->ActiveProfile.L3_S3_InvSnakeDoorPartTaken = true;

						//Box done
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_Box] = false;
						GFHud->ReleaseDrag();
						GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S3_Box);

						ComboScene->GetObjectByID("keypart")->SetEnable(false);
						GFHud->TakeInventoryItem(eInv_L3S3_SnakeDoorPart);

						TaskPendingUpdate();
					}
				}
			}
		}
		break;
	case eInv_L4_S1_Leaf:
		if(isAnyAnimActive)
		{
			return;
		}

		if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonCollected)		
			HideComboInventoryPopup();

		if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonCollected)
		{
			if( !GFHud->IsDraggingItem() && InvDestRectStick.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L4improvise");
				}
			}
		}
		break;
	case eInv_L4_CB_HandKerchief:
		if (CAnim_combo_knife->IsAnimFinished() && isKnifeAnimPlaying)
		{
			ComboScene->GetObjectByID("Broken_kerchief02")->SetEnable(false);
			isKnifeAnimPlaying = false;
			GFHud->TakeInventoryItem(eInv_L4_S1_KerchiefPiece1);
		}

		if (CAnim_combo_kerchief->IsAnimFinished() && isKerchiefAnimPlaying)
		{
			isKerchiefAnimPlaying = false;
			
			isAnyAnimActive = false;

			ComboScene->SetSceneAlpha(0);

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_HandKerchief] = false;
			GFHud->ReleaseDrag();
			GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_CB_HandKerchief);
			GFHud->TakeInventoryItem(eInv_L4_S1_KerchiefPiece2);


			ProfileSystem::Instance->ActiveProfile.L4_S1_HandKerchiefTored = true;
			TaskPendingUpdate();
		}
		if(isAnyAnimActive)
		{
			return;
		}

		if(ProfileSystem::Instance->ActiveProfile.L4_S1_HandKerchiefTored)		
			HideComboInventoryPopup();

		if(!ProfileSystem::Instance->ActiveProfile.L4_S1_HandKerchiefTored)
		{
			if( !GFHud->IsDraggingItem() && InvDestRectKnife.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L4big");
				}
			}
		}
		break;

	case eInv_L4_S2_Bamboo:
		{
			if (isAnyAnimActive)
			{
				return;
			}
			if(ProfileSystem::Instance->ActiveProfile.L4_S2_InvBambooSpearTaken)	
			{
				HideComboInventoryPopup();
			}
			if( !GFHud->IsDraggingItem() && InvDestRectBambooKnife.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					if (ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo)
					{
						GFHud->ShowBannerText("L4tie");
					}
					else
					{
						GFHud->ShowBannerText("L4thing");
					}
				}
			}
		}
		break;

	case eInv_L4_S2_GrassBunch:
		{
			if (isAnyAnimActive)
			{
				return;
			}
			if(ProfileSystem::Instance->ActiveProfile.L4_S2_InvRopeTaken)	
			{
				HideComboInventoryPopup();
			}
			if( !GFHud->IsDraggingItem() && InvDestRectGrassRope.Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandPointy);
				if(clicked)
				{

					if (ProfileSystem::Instance->ActiveProfile.L4_s2_GrassRopeFrameNumber < 9)
					{
						ProfileSystem::Instance->ActiveProfile.L4_s2_GrassRopeFrameNumber ++;
						rope->SetFrame(ProfileSystem::Instance->ActiveProfile.L4_s2_GrassRopeFrameNumber);
						GFHud->InvBoxCtrl->updateComboItemStatus(eInv_L4_S2_GrassBunch);
						Control::Audio->QuickLoadAndPlaySFX("combo_grassBraiding");
					}
					else if(ProfileSystem::Instance->ActiveProfile.L4_s2_GrassRopeFrameNumber == 9)
					{
						isAnyAnimActive = true;
						OnAnimationComplete(_Anim_L4S2_GrassRope);
					}

				}
			}
		}
		break;

	case eInv_L4_S4_BinocularWithoutLens:
		if(isAnyAnimActive)
		{
			return;
		}

		if(ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularWithLensTaken)		
			HideComboInventoryPopup();

		if(!ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularWithLensTaken)
		{
			if( !GFHud->IsDraggingItem() && InvDestRectBinocularLens.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L4lens");
				}
			}
		}
		break;
	case eInv_L4_S5_Net:
		{
			if (isAnyAnimActive)
			{
				return;
			}
			if(ProfileSystem::Instance->ActiveProfile.L4_S5_InvBambooStrainerTaken)	
			{
				HideComboInventoryPopup();
			}
			if( !GFHud->IsDraggingItem() && InvDestRectBambooStrainer.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					if (ProfileSystem::Instance->ActiveProfile.L4_S5_ComboBambooFrameAddedToNet)
					{
						GFHud->ShowBannerText("L4rope");
					}
					else
					{
						GFHud->ShowBannerText("L4tiny");
					}
				}
			}
		}
		break;

	case eInv_L5_S1_Bow:
		if(isAnyAnimActive)
		{
			return;
		}

		if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowFirstTaken && !ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
			HideComboInventoryPopup(); // For first time
		if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowSecondTaken && ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
			HideComboInventoryPopup(); // For first time

		if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowStringFixed)
		{
			if( !GFHud->IsDraggingItem() && ActionRectBowStringFix.Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandTake);
				if(clicked)
				{
					ProfileSystem::Instance->ActiveProfile.L5_S1_BowStringFixed = true;

					Control::Audio->QuickLoadAndPlaySFX("combo_bowropetie");

					JAnim_BowStringFix->PlayAnim();
					isAnyAnimActive = true;
					return;
				}
				return;
			}
		}
		if(ProfileSystem::Instance->ActiveProfile.L5_S1_BowArrowHolderPlaced)
		{
			if( !GFHud->IsDraggingItem() && ActionRectBowArrowTake.Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandTake);
				if(clicked)
				{
					if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowStringFixed)
					{
						GFHud->ShowBannerText("L5attach");
					}
					else
					{
						Control::Audio->QuickLoadAndPlaySFX("combo_arrowattch");

						JAnim_BowArrowFix->PlayAnim();
						isAnyAnimActive = true;
					}
				}
			}
		}
		else
		{
			if( !GFHud->IsDraggingItem() && InvDestRectBow.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
						GFHud->ShowBannerText("L5use");
					else
						GFHud->ShowBannerText("L5bow");
				}
			}
		}
		break;
	case eInv_L5_S4_PickaxeBlade:
		if(isAnyAnimActive)
		{
			return;
		}

		if(ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeTaken)
			HideComboInventoryPopup();

		if(!ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeTaken)
		{
			if( !GFHud->IsDraggingItem() && InvDestRectPickaxe.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L5handle");
				}
			}
		}
		break;
	case eInv_L5_S4_PeacockChain:
		{
			if(isAnyAnimActive)
				return;

			if (ProfileSystem::Instance->ActiveProfile.L5_S6_InvPeacockLocketTaken)
			{
				HideComboInventoryPopup();
				return;
			}


			if (InvItemRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if (Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("combo_locettake");

					JAnim_Locket->PlayAnim();
					isAnyAnimActive = true;
				}
			}
		}
		break;
	case eInv_L6_S4_Rope:
		{
			if (isAnyAnimActive)
			{
				return;
			}
			if(ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeHorseShoeComboTaken)	
			{
				HideComboInventoryPopup();
			}
			if( !GFHud->IsDraggingItem() && InvDestRectRopeHorseShoe.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L6rope");
				}
			}
		}
		break;

	case eInv_L7_S1_PotEmpty:
		if(ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotWithChainTaken)
			HideComboInventoryPopup();

		if(isAnyAnimActive)
		{
			return;
		}

		if( !GFHud->IsDraggingItem() && InvDestRectL7PotMetalWire.Intersection(&pos) )
		{
			Cursor::SetMode(CA_Gear);
			if(clicked)
			{
				GFHud->ShowBannerText("L7attach");
			}
		}

		if(ProfileSystem::Instance->ActiveProfile.L7_S1_ComboPotChainPlaced)
		{
			if( !GFHud->IsDraggingItem() && ActionRectL7PotTryChain.Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandTake);
				if(clicked)
				{
					GFHud->ShowBannerText("L7attach");
					JAnim_PotChainTry->PlayAnim();
					isAnyAnimActive = true;
				}
				return;
			}
		}
		break;

	case eInv_L7_S2_SphereHalfBase:
		if(ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereFullTaken)
		{
			HideComboInventoryPopup();
			return;
		}

		if(isAnyAnimActive)
		{
			return;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereFullTaken)
		{				
			if( !GFHud->IsDraggingItem() && InvDestRectL7Globe.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L7half");
				}
			}
		}
		break;
	case eInv_L8_S1_MagicWand:
		if (isMagicWandKeyInHand)
		{
			isMagicWandKeyInHand = false;
			Janim_MagicWand->PlayAnim();
			isAnyAnimActive = true;
			return;
		}
		if (ProfileSystem::Instance->ActiveProfile.L8_S1_MagicWandWithoutKeyTaken)
		{
			HideComboInventoryPopup();
			return;
		}

		if (isAnyAnimActive)
		{
			return;
		}

		if( !GFHud->IsDraggingItem() && InvDestRectL8MagicWandTool.Intersection(&pos) )
		{
			Cursor::SetMode(CA_Gear);
			if(clicked)
			{
				GFHud->ShowBannerText("L8safe");
			}
		}

		break;
	case eInv_L8_S2_HalfKey:
		{
			if (isAnyAnimActive)
			{
				return;
			}
			if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey])	
			{
				HideComboInventoryPopup();
			}
			if( !GFHud->IsDraggingItem() && InvDestRectL8ComboKey.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Gear);
				if(clicked)
				{
					GFHud->ShowBannerText("L8half");
				}
			}
		}
		break;

	default:
		break;
	}
}

void ComboInvSystem::ShowComboInventoryPopup(int invId, float offsetX, float offsetY, bool skipTransition)
{
	isSkipTransition = skipTransition;
	_invId = invId;

	clickDelayTimer = 0;

	popOffsetX = (int)(offsetX-683);
	popOffsetY = (int)(offsetY-384);

	if(invId == eInv_L1S1_Bag)
	{
		//Save it for further showing
		ProfileSystem::Instance->ActiveProfile.L1S1_ComboBagInitX = (int)offsetX;
		ProfileSystem::Instance->ActiveProfile.L1S1_ComboBagInitY = (int)offsetY;
	}

	ComboCloseBtn = new CObject();
	ComboCloseBtn->LoadInit("UI\\CLOSEBTN.SPR", 1004, 88, eZOrder_ComboInvScene+30);
	ComboCloseBtn->SetUID("closeBtn");

	ComboMaskBg = new CObject();
	ComboMaskBg->LoadSprite("UI\\MASK.SPR");
	ComboMaskBg->SetXY(683,384);
	ComboMaskBg->SetScale(2.0);
	ComboMaskBg->SetZ(eZOrder_ComboInvMask);
	ComboMaskBg->Show(false);

	ComboPopMaskBg = new CObject();
	ComboPopMaskBg->LoadSprite("UI\\MASK.SPR");
	ComboPopMaskBg->SetXY(683,384);
	ComboPopMaskBg->SetScale(2.0);
	ComboPopMaskBg->SetZ(eZOrder_ComboPopMask);
	ComboPopMaskBg->Show(false);

	//Level 1 Combo Items
#pragma region Level1ComboItems
	if(invId == eInv_L1S1_Bag)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L1\\S1BAGPOP\\BAG.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		Control::Audio->LoadSample(aVOL1MaryRefS1_1,AudioVO);

		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(302.0f,79.0f,784.0f,580.0f);

		bag_ZipOpened = ComboScene->GetObjectByID("zipopen");
		bag_Knife = ComboScene->GetObjectByID("bagknife");
		bag_Zipper = ComboScene->GetObjectByID("zipper");
		bag_CoverClosed = ComboScene->GetObjectByID("bagcoverclosed");
		bag_CoverOpened = ComboScene->GetObjectByID("bagcoveropen");
		bag_InvCreeksLetter = ComboScene->GetObjectByID("letter");
		bag_KidsDrawing = ComboScene->GetObjectByID("drawingBig");
		bag_LocketFixed = ComboScene->GetObjectByID("baglocketfixed");

		/*
		SceneData creekText;
		creekText.objType = "Object";
		creekText.uid = "creekText";
		creekText.spriteString = "LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S1\\CREEKLETTERMARY.SPR";
		creekText.spriteAnim = 0;
		creekText.x = bag_InvCreeksLetter->Pos.x;
		creekText.y = bag_InvCreeksLetter->Pos.y;
		creekText.ZOrder = bag_InvCreeksLetter->ZOrder + 1;

		ComboScene->CreateObject(creekText);
		*/

		CObject* objArr[] = {bag_ZipOpened, bag_Knife, bag_Zipper, ComboScene->GetObjectByID("knifemask")};
		std::string animArr[] = {"ANIM\\L1\\ZIPOPEN_ZIPOPENED.ANIM", "ANIM\\L1\\ZIPOPEN_KNIFE.ANIM", "ANIM\\L1\\ZIPOPEN_ZIPPER.ANIM", "ANIM\\L1\\ZIPOPEN_MASK.ANIM"};
		CAnim_ZipperOpen = new CustomAnimGroup(4, animArr, objArr);

		int pts1[] = {842,216,593,383,568,445,631,561,656,569,946,336,960,309,914,228};
		BagOpenPoly=  new CPolygon(8);
		BagOpenPoly->Init(pts1);

		int BagZipperPoints[] = {650,120,450,240,485,290,685,160,649,119};//old values {647,133,459,247,496,278,677,169,646,131};
		BagZipperPoly=  new CPolygon(5);
		BagZipperPoly->Init(BagZipperPoints);

		if(ProfileSystem::Instance->ActiveProfile.L1S1_BagZipOpened)
			bag_Zipper->SetXY(481,275);

		IsbagOpeningPlaying = false;

		IsKidDrawingAddingToJournal = false;

		IsKidDrawingZoomed = false;
		IsKidDrawingZooming = false;
		CAnim_kidDrawingSlide1 = new CustomAnimator("ANIM\\L1\\S1BAGDRAWINGSLIDE1.ANIM", bag_KidsDrawing);
		CAnim_kidDrawingSlide2 = new CustomAnimator("ANIM\\L1\\S1BAGDRAWINGSLIDE2.ANIM", bag_KidsDrawing);

		bag_CreekLetterZoomed = NULL;
		bag_CreekLetterZoomedText = NULL;
		isCreekletterZooming = false;
		isCreekletterZoomedIn = false;

		if(!ProfileSystem::Instance->ActiveProfile.L1S1_BagOpened)
		{
			bag_InvCreeksLetter->SetEnable(false);
			bag_CoverOpened->SetEnable(false);
			bag_KidsDrawing->SetScale(0.4f);
			bag_KidsDrawing->SetEnable(false);
			ComboScene->GetObjectByID("bagopenshadow")->SetEnable(false);
		}
		else
		{
			bag_LocketFixed->PlayAnimation(2);
			bag_CoverClosed->SetEnable(false);
			bag_KidsDrawing->SetScale(0.4f);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingAddedToJournal)
			{
				GFHud->ShowBannerText("L1milk");
				Control::Audio->PlaySample(aVOL1MaryRefS1_1);
			}

			bag_KidsDrawing->SetScale(1.0f);
			bag_KidsDrawing->SetRotate(13.5f);
			bag_KidsDrawing->SetZ(bag_CoverClosed->ZOrder+2);
			if(ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingAddedToJournal)
				bag_KidsDrawing->SetEnable(false);
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S1_BagZipOpened)
		{
			ComboScene->GetObjectByID("knifemask")->SetEnable(false);
			bag_Knife->SetEnable(false);
			bag_ZipOpened->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken)
		{
			bag_Knife->SetEnable(false);
		}

		for (int i = 0; i < 4; i++)
		{
			ruby_FixPos[i] = NULL;
			ruby_takeRects[i] = NULL;
			ruby_destRects[i] = NULL;
			ruby_IsPartFixed[i] = false;
		}

		if ( !ProfileSystem::Instance->ActiveProfile.L1S1_LocketAllPartsFixed )
		{
			bag_LocketFixed->SetEnable(false);

			ComboPopupScene = new CHOScene();
			ComboPopupScene->Load("INV\\L1\\S1RUBYPOP\\RUBY.SCN");

			//Add popup frame and close button
			CObject *popFrame = new CObject();
			popFrame->LoadInit("UI\\POPCIRCFRAME.SPR", 4, -9, 1);
			popFrame->SetUID("popFrame");
			//Place Ruby Pieces above Frame
			ComboPopupScene->InsertObject(popFrame,1);

			CObject *popCloseBtnObj = new CObject();
			popCloseBtnObj->LoadInit("UI\\CLOSEBTN.SPR", 145, -158, 7);
			popCloseBtnObj->SetUID("popClose");
			ComboPopupScene->PushObject(popCloseBtnObj);

			ComboPopupScene->SetZOff(eZOrder_ComboPopup);
			ComboPopupScene->SetPosOff(683, 334);

			activeRubyPart = -1;

			ruby_FixPos[0] = new CPoint(683-61, 334-41, 0);
			ruby_FixPos[1] = new CPoint(683-30, 334+7, 0);
			ruby_FixPos[2] = new CPoint(683+12, 334-12, 0);
			ruby_FixPos[3] = new CPoint(683+42, 334+4, 0);		

			ruby_takeRects[0] = new CRectangle(458.0f,224.0f,108.0f,100.0f);
			ruby_takeRects[1] = new CRectangle(509.0f,408.0f,121.0f,93.0f);
			ruby_takeRects[2] = new CRectangle(770.0f,398.0f,74.0f,81.0f);
			ruby_takeRects[3] = new CRectangle(827.0f,244.0f,69.0f,87.0f);

			ruby_destRects[0] = new CRectangle(572.0f,245.0f,110.0f,96.0f);
			ruby_destRects[1] = new CRectangle(597.0f,306.0f,105.0f,100.0f);
			ruby_destRects[2] = new CRectangle(653.0f,264.0f,81.0f,81.0f);
			ruby_destRects[3] = new CRectangle(684.0f,289.0f,80.0f,112.0f);

			ruby_FullPart = ComboPopupScene->GetObjectByID("rubyfullfinal");
			ruby_Parts[0] = ComboPopupScene->GetObjectByID("rubypart1");
			ruby_Parts[1] = ComboPopupScene->GetObjectByID("rubypart2");
			ruby_Parts[2] = ComboPopupScene->GetObjectByID("rubypart3");
			ruby_Parts[3] = ComboPopupScene->GetObjectByID("rubypart4");

			ruby_FullPart->SetEnable(false);

			if( ProfileSystem::Instance->ActiveProfile.L1S1_LocketPart1Fixed )	ruby_IsPartFixed[0] = true;
			if( ProfileSystem::Instance->ActiveProfile.L1S1_LocketPart2Fixed )	ruby_IsPartFixed[1] = true;
			if( ProfileSystem::Instance->ActiveProfile.L1S1_LocketPart3Fixed )	ruby_IsPartFixed[2] = true;
			if( ProfileSystem::Instance->ActiveProfile.L1S1_LocketPart4Fixed )	ruby_IsPartFixed[3] = true;

			for (int i = 0; i < 4; i++){
				if(ruby_IsPartFixed[i]){
					ruby_Parts[i]->SetPos(ruby_FixPos[i]);
				}

				if(!ProfileSystem::Instance->ActiveProfile.L1S1_LocketPartsPlaced)
					ruby_Parts[i]->SetEnable(false);
			}

			ComboPopupScene->Show(false);
		}
	}	
	else if(invId == eInv_L1S4_PhotoFrame)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L1\\S4PICPOP\\PAINTPOP.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(221.0f,36.0f,956.0f,606.0f);

		photo_leftArrow = ComboScene->GetObjectByID("arrowLeft");
		photo_rightArrow = ComboScene->GetObjectByID("arrowRight");		

		photo_paperOverlay = ComboScene->GetObjectByID("paperoverlay");
		photo_bgback = ComboScene->GetObjectByID("bgback");
		photo_bgfront = ComboScene->GetObjectByID("bgfront");
		photo_BeetlePart = ComboScene->GetObjectByID("beetlepart");
		photo_SecretCode = ComboScene->GetObjectByID("bookcode");

		if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoTurned)
		{
			photo_paperOverlay->SetEnable(false);
			photo_BeetlePart->SetEnable(false);
			photo_SecretCode->SetEnable(false);
			photo_bgback->SetEnable(false);
		}
		else
		{
			photo_leftArrow->SetEnable(false);
			photo_rightArrow->SetEnable(false);
			photo_bgfront->SetEnable(false);

			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoPaperTeared)
				photo_paperOverlay->SetEnable(false);
			if(ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken)
				photo_BeetlePart->SetEnable(false);
		}
	}
	else if(invId == eInv_L1S4_Diary)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L1\\S4DIARYPOP\\DIARY.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(218.0f,61.0f,900.0f,547.0f);

		diary_page1		= ComboScene->GetObjectByID("diarypage1");
		diary_nextPagebtn = ComboScene->GetObjectByID("diarysideglow");
		diary_prevPagebtn = ComboScene->GetObjectByID("diaryleftglow");
		diary_creekNotice = ComboScene->GetObjectByID("creekletter");
		diary_coin = ComboScene->GetObjectByID("coin");

		diary_item1 = ComboScene->GetObjectByID("item1");
		diary_item2 = ComboScene->GetObjectByID("item2");
		diary_item3 = ComboScene->GetObjectByID("item3");
		diary_item4 = ComboScene->GetObjectByID("item4");
		diary_item5 = ComboScene->GetObjectByID("item5");

		//Localized
		creekLetterText = new CObject();
		creekLetterText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S4\\CREEKLETTER.SPR", diary_creekNotice->Pos.x, diary_creekNotice->Pos.y, diary_creekNotice->ZOrder+1);
		creekLetterText->SetUID("creekLetterText");
		//creekLetterText->SetEnable(false);
		ComboScene->PushObject(creekLetterText);

		diary_page1_text = new CObject();
		diary_page1_text->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S4\\DIARYTEXT.SPR", diary_page1->Pos.x, diary_page1->Pos.y, diary_page1->ZOrder + 1);
		diary_page1_text->SetUID("diaryPage1Text");
		//diary_page1_text->SetEnable(false);
		ComboScene->PushObject(diary_page1_text);

		diary_prevPagebtn->SetEnable(false);
		diary_nextPagebtn->SetEnable(false);

		isCreekNoticeZoomedIn = false;
		isCreekNoticeZooming = false;
		isCreekNoticeGoingToJournal = false;

		diary_creekNotice->SetRotate(25.0f);
		diary_creekNotice->SetScale(0.7f);
		creekLetterText->SetRotate(25.0f);
		creekLetterText->SetScale(0.7f);

		if(ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryPageTurned)
		{
			diary_page1->SetEnable(false);
			diary_page1_text->SetEnable(false);

			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem1Removed)
				diary_item1->SetEnable(false);
			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem2Removed)
				diary_item2->SetEnable(false);
			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem3Removed)
				diary_item3->SetEnable(false);
			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem4Removed)
				diary_item4->SetEnable(false);
			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryItem5Removed)
				diary_item5->SetEnable(false);

			if (ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryCreekNoticeTaken)
			{
				diary_creekNotice->SetEnable(false);
				creekLetterText->SetEnable(false);
			}
		}
	}
	else if(invId == eInv_L1S4_BeetleHead)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L1\\S4BEETLE\\S4BEETLE.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(370.0f,34.0f,629.0f,611.0f);

		ComboScene->GetObjectByID("beetlehead")->SetRotate(30);
		ComboScene->GetObjectByID("beetletail")->SetRotate(30);
		ComboScene->GetObjectByID("headglow")->SetRotate(30);
		ComboScene->GetObjectByID("tailglow")->SetRotate(30);

		ComboScene->GetObjectByID("beetletail")->SetEnable(false);
		ComboScene->GetObjectByID("headglow")->SetEnable(false);
		ComboScene->GetObjectByID("tailglow")->SetEnable(false);

		CObject* objArr[] = {ComboScene->GetObjectByID("beetlehead"), ComboScene->GetObjectByID("beetletail"), ComboScene->GetObjectByID("headglow"), ComboScene->GetObjectByID("tailglow")};
		std::string animArr[] = {"ANIM\\L1\\S4BEETLEHEAD.ANIM", "ANIM\\L1\\S4BEETLETAIL.ANIM", "ANIM\\L1\\S4BEETLEHGLOW.ANIM", "ANIM\\L1\\S4BEETLETGLOW.ANIM"};
		CAnim_BeetleJoin = new CustomAnimGroup(4, animArr, objArr);

		isBeetleJoinPaying = false;
	}
	else if(invId == eInv_L1S5_HammerHead)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L1\\S5HAMMER\\HAM.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		ComboScene->Show(false);		

		ComboBoundaryRect = new CRectangle(349.0f,58.0f,649.0f,529.0f);

		ComboScene->GetObjectByID("headmask")->SetEnable(false);
		ComboScene->GetObjectByID("hammerhand")->SetEnable(false);

		CObject* objArr[] = {ComboScene->GetObjectByID("hammerhead"), ComboScene->GetObjectByID("hammerhand"), ComboScene->GetObjectByID("headmask")};
		std::string animArr[] = {"ANIM\\L1\\S5HAMHEAD.ANIM", "ANIM\\L1\\S5HAMHAND.ANIM", "ANIM\\L1\\S5HAMMASK.ANIM"};
		CAnim_HammerJoin = new CustomAnimGroup(3, animArr, objArr);
		isHammerJoinPaying = false;

	}  
#pragma endregion

	//Level 2
#pragma region Level2ComboItems
	if(invId == eInv_L2S4_AxeBlunt)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L2\\S4AXESHARPEN\\AXE.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(452.0f,67.0f,441.0f,494.0f);

		JAnim_AxeSharpen = NULL;
		isAxeSharpenAnimPlaying = false;

		superfxArray.clear();

		axeSparklesFX = new SuperFX("PRT\\L2\\s4whitesparkles.INI", PointSystem::CreateCPoint(1014,98,0), ComboScene->GetObjectByID("sharpedge")->ZOrder);
		superfxArray.push_back(axeSparklesFX);

		JAnim_AxeSharpen = new JSONAnimator(_Anim_L2S4_AxeSharpen, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_AxeSharpen->parseAndLoadJSON("ANIM\\L2\\S4AXESHARPEN.JSON", ComboScene, &superfxArray);

		ComboScene->GetObjectByID("glow")->SetEnable(false);
		ComboScene->GetObjectByID("sharpedge")->SetEnable(false);
		ComboScene->GetObjectByID("sharpstone")->SetEnable(false);
	}
	else if(invId == eInv_L2S6_KnifeBlunt)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L2\\S6KNIFE\\KNIFE.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(319.0f,111.0f,634.0f,461.0f);

		JAnim_CorkRemove = NULL;
		JAnim_RustRemove = NULL;
		vinegarDropsFX = NULL;

		std::vector<SuperFX*> superfxArray;
		vinegarDropsFX = new SuperFX("PRT\\L2\\cbvinegardrops.INI", PointSystem::CreateCPoint(488,106,0), eZOrder_ComboInvScene+15); 
		superfxArray.push_back(vinegarDropsFX);

		JAnim_CorkRemove = new JSONAnimator(_Anim_L2S5_CorkRemove, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_CorkRemove->parseAndLoadJSON("ANIM\\L2\\CBKNIFECORKTAKE.JSON", ComboScene);

		JAnim_RustRemove = new JSONAnimator(_Anim_L2S5_RustRemove, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_RustRemove->parseAndLoadJSON("ANIM\\L2\\CBKNIFERUSTREMOVE.JSON", ComboScene, &superfxArray);

		ComboScene->GetObjectByID("bottlepour")->SetEnable(false);

		if(!ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarPlaced)
		{
			ComboScene->GetObjectByID("corkmask")->SetEnable(false);
			ComboScene->GetObjectByID("corkfull")->SetEnable(false);
			ComboScene->GetObjectByID("bottle")->SetEnable(false);
		}
		else if(ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarCorkOpened)
		{
			ComboScene->GetObjectByID("corkmask")->SetEnable(false);
			ComboScene->GetObjectByID("corkfull")->SetEnable(false);
		}	
	} 
#pragma endregion

	//Level 3
	if(invId == eInv_L3S1_FlagFull)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L3\\S1FLAGCOMBO\\FLAGCOMBO.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		JAnim_FlagComboRemove = NULL;
		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		JAnim_FlagComboRemove = new JSONAnimator(_anim_L3S1_FlagComboRemove, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_FlagComboRemove->parseAndLoadJSON("ANIM\\L3\\S1FLAGREMOVEINV.JSON", ComboScene);

		ComboScene->GetObjectByID("Flag_unTied")->SetAlpha(0);
	}
	else if(invId == eInv_L3S1_LadderBroken)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L3\\S1LADDERFIX\\LADDER.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		ComboScene->Show(false);		

		ComboBoundaryRect = new CRectangle(309.0f,103.0f,785.0f,497.0f);

		JAnim_PiecesPlacing = NULL;

		ladderClothFadingIn = false;
		clothTieTimer = 0;

		JAnim_PiecesPlacing = new JSONAnimator(_Anim_L3S1_LadderPartsPlacing, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_PiecesPlacing->parseAndLoadJSON("ANIM\\L3\\S1LADDERSTEPSPLACE.JSON", ComboScene);

		ComboScene->GetObjectByID("fixpiecebottom")->SetEnable(false);
		ComboScene->GetObjectByID("fixpiecetop")->SetEnable(false);

		if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderPiecesFixed)
		{
			ComboScene->GetObjectByID("fixedpiecetop")->SetEnable(false);
			ComboScene->GetObjectByID("fixedpiecebottom")->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L3_S1_LadderBrokenStepFixed)
		{
			ComboScene->GetObjectByID("stepbroken")->SetEnable(false);
		}
		else
		{
			ComboScene->GetObjectByID("stepfixed")->SetEnable(false);
		}
	}
	else if(invId == eInv_L3S2_CoconutFull)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L3\\S2COCONUTSHELL\\COCOSHELL.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		JAnim_CoconutOuterRemove = NULL;
		JAnim_CoconutBreak = NULL;

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved)
		{
			ComboScene->GetObjectByID( "huskright")->SetAlpha(0);
			ComboScene->GetObjectByID( "huskback")->SetAlpha(0);
			ComboScene->GetObjectByID( "huskleft")->SetAlpha(0);
			ComboScene->GetObjectByID( "flowermask")->SetAlpha(0);
		}

		ComboScene->GetObjectByID( "sharprod1")->SetAlpha(0);
		ComboScene->GetObjectByID( "sharprod2")->SetAlpha(0);
		ComboScene->GetObjectByID( "sickle")->SetAlpha(0);
		ComboScene->GetObjectByID( "leftshell")->SetAlpha(0);
		ComboScene->GetObjectByID( "rightshell")->SetAlpha(0);

		JAnim_CoconutOuterRemove = new JSONAnimator(_Anim_L3S2_CoconutOuterShell, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_CoconutOuterRemove->parseAndLoadJSON("ANIM\\L3\\S2_COCONUT_SHELL_BREAKING.JSON", ComboScene);

		JAnim_CoconutBreak = new JSONAnimator(_Anim_L3S2_CoconutBreak, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_CoconutBreak->parseAndLoadJSON("ANIM\\L3\\S2_COCONUT_BREAKING.JSON", ComboScene);
	}
	else if(invId == eInv_L3S2_CocoShellPowderFilled)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L3\\S2CHIRATTA\\CHIRATTAPOWDER.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(370.0f,186.0f,586.0f,395.0f);

		JAnim_TwinePlacing = NULL;

		JAnim_TwinePlacing = new JSONAnimator(_Anim_L3S2_CoconutShellThreadPlace, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_TwinePlacing->parseAndLoadJSON("ANIM\\L3\\S2CHIRATTATWINE.JSON", ComboScene);

		ComboScene->GetObjectByID("threadfixed")->SetEnable(false);
		ComboScene->GetObjectByID("thread1")->SetEnable(false);
		ComboScene->GetObjectByID("thread2")->SetEnable(false);
		ComboScene->GetObjectByID("thread3")->SetEnable(false);
	}
	else if(invId == eInv_L3S3_Box)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L3\\S3BOX\\BOX.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);

		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsSecretBoxOpened)
		{
			ComboScene->GetObjectByID("boxclosed")->SetEnable(false);
		}
		else
		{
			ComboScene->GetObjectByID("boxopened")->SetEnable(false);
		}
	}
	else if(invId == eInv_L3S2_HammerHead)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L3\\S2HAMMER\\HAM.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		JAnim_HammerFix = NULL;

		JAnim_HammerFix = new JSONAnimator(_Anim_L3S2_HammerFix, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_HammerFix->parseAndLoadJSON("ANIM\\L3\\S2_HAMMER_HANDLE_REPLACE.JSON", ComboScene);

		ComboScene->GetObjectByID( "handle")->SetAlpha(0);

		GFHud->ShowBannerText(Banner_L3_S3_HammerHeadMultiInv);
	}


#pragma region Level 4
	else if(invId == eInv_L4_S1_Leaf)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L4\\LEAF\\CB_LEAF.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);


		ComboScene->GetObjectByID("Leaf_FirstFold_Leaf")->SetEnable(false);
		ComboScene->GetObjectByID("Leaf")->SetEnable(true);
		ComboScene->GetObjectByID("LeafPasteSpoon")->SetAlpha(0);
		ComboScene->GetObjectByID("Leaf_stick")->SetAlpha(0);

		JAnim_LeafSpoon = NULL;

		JAnim_LeafSpoon = new JSONAnimator(_Anim_L4S1_LeafSpoonMaking, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_LeafSpoon->parseAndLoadJSON("ANIM\\L4\\L4_S1_COMBO_LEAFSPOON.JSON", ComboScene);
	}
	else if (invId == eInv_L4_CB_HandKerchief)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L4\\KERCHIEF\\CB_KERCHIEF.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		ComboScene->GetObjectByID("Knife")->SetEnable(false);

		/*JAnim_Kerchief = NULL;

		JAnim_Kerchief = new JSONAnimator(_Anim_L4S1_KerchiefTore, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_Kerchief->parseAndLoadJSON("ANIM\\L4\\L4_S1_COMBO_KERCHIEF.JSON", ComboScene);*/

		CAnim_combo_knife = new CustomAnimator("ANIM\\L4\\L4_S1_KNIFE.ANIM", ComboScene->GetObjectByID("Knife"));
		isKnifeAnimPlaying = false;

		CAnim_combo_kerchief = new CustomAnimator("ANIM\\L4\\L4_S1_KERCHIEF.ANIM", ComboScene->GetObjectByID("Broken_kerchief02"));
		isKerchiefAnimPlaying = false;
	}
	else if(invId == eInv_L4_S2_Bamboo)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L4\\BAMBOOKNIFE\\BAMBOOKNIFECOMBO.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		JSONAnim_Combo_BambooKnife = new JSONAnimator(_Anim_L4S2_BambooKnife, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JSONAnim_Combo_BambooKnife->parseAndLoadJSON("ANIM\\L4\\L4_COMBOBAMBOOKNIFEANIM.JSON", ComboScene);
		JSONAnim_Combo_BambooKnife->PlayAnim();
		JSONAnim_Combo_BambooKnife->StopAnim();
		JSONAnim_Combo_BambooKnife->SetAnimTime(0);
		ComboScene->GetObjectByID("Kerchief")->SetEnable(false);
		ComboScene->GetObjectByID("KerchiefPiece")->SetEnable(false);

		if (!ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo)
		{
			ComboScene->GetObjectByID("Knife")->SetEnable(false);
		}
	}
	else if(invId == eInv_L4_S2_GrassBunch)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L4\\GRASSROPE\\GRASSROPECOMBO.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		rope = ComboScene->GetObjectByID("GrassRopeAnim");
		rope->Paused = true;
		rope->SetFrame(ProfileSystem::Instance->ActiveProfile.L4_s2_GrassRopeFrameNumber);
	}
	else if (invId == eInv_L4_S4_BinocularWithoutLens)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L4\\BINOCULAR\\CB_BINOCULAR.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		ComboScene->GetObjectByID("Lens")->SetEnable(false);
	}  
	else if(invId == eInv_L4_S5_Net)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L4\\BAMBOOSTRAINER\\L4_S3_BAMBOOSTRAINER.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		JSONAnim_Combo_BambooStrainer = new JSONAnimator(_Anim_L4S5_BambooStrainer, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JSONAnim_Combo_BambooStrainer->parseAndLoadJSON("ANIM\\L4\\L4_COMBOBAMBOOSTRAINERANIM.JSON", ComboScene);
		JSONAnim_Combo_BambooStrainer->PlayAnim();
		JSONAnim_Combo_BambooStrainer->StopAnim();
		JSONAnim_Combo_BambooStrainer->SetAnimTime(0);
		ComboScene->GetObjectByID("RopeFull")->SetEnable(false);
		ComboScene->GetObjectByID("Rope1")->SetEnable(false);
		ComboScene->GetObjectByID("Rope2")->SetEnable(false);
		ComboScene->GetObjectByID("Rope3")->SetEnable(false);
		ComboScene->GetObjectByID("Rope4")->SetEnable(false);

		if (!ProfileSystem::Instance->ActiveProfile.L4_S5_ComboBambooFrameAddedToNet)
		{
			ComboScene->GetObjectByID("Bamboo")->SetEnable(false);
		}
	}
#pragma endregion

	else if(invId == eInv_L5_S1_Bow)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L5\\S1BOW\\BOW.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(263.0f,150.0f,831.0f,428.0f);

		JAnim_BowStringFix = NULL;
		JAnim_BowArrowFix = NULL;

		JAnim_BowStringFix = new JSONAnimator(_Anim_L5S1_ArrowStringFix, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_BowStringFix->parseAndLoadJSON("ANIM\\L5\\S1BOWSTRINGFIX.JSON", ComboScene);

		JAnim_BowArrowFix = new JSONAnimator(_Anim_L5S1_ArrowBowFix, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_BowArrowFix->parseAndLoadJSON("ANIM\\L5\\S1ARROWFIX.JSON", ComboScene);

		if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowStringFixed)
		{
			ComboScene->GetObjectByID("thread2")->SetEnable(false);
			ComboScene->GetObjectByID("threadfinal")->SetEnable(false);
		}
		else
		{
			ComboScene->GetObjectByID("thread1")->SetEnable(false);
			ComboScene->GetObjectByID("thread2")->SetEnable(false);
		}

		if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowArrowHolderPlaced)
		{
			ComboScene->GetObjectByID("arrowholdermask")->SetEnable(false);
			ComboScene->GetObjectByID("arrow")->SetEnable(false);
			ComboScene->GetObjectByID("arrowholder")->SetEnable(false);
		}
	}
	else if(invId == eInv_L5_S4_PickaxeBlade)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L5\\S4PICKAXE\\PICKAXE.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(393.0f,81.0f,550.0f,486.0f);

		JAnim_PickaxeHandleFix = NULL;		

		JAnim_PickaxeHandleFix = new JSONAnimator(_Anim_L5S4_PickaxeFix, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_PickaxeHandleFix->parseAndLoadJSON("ANIM\\L5\\S1PICKAXEHANDLEFIX.JSON", ComboScene);

		ComboScene->GetObjectByID("handle")->SetEnable(false);
	}
	else if(invId == eInv_L5_S4_PeacockChain)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L5\\S6LOCKET\\LOCKET.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		JAnim_Locket = NULL;
		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		JAnim_Locket = new JSONAnimator(_Anim_L5S6_Locket, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_Locket->parseAndLoadJSON("ANIM\\L5\\S6LOCKET.JSON", ComboScene);

	}

#pragma region Level 6

	else if(invId == eInv_L6_S4_HeartContainer)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L6\\BIG\\HEARTLOCKETCOMBO\\HEARTLOCKETCOMBO.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		JSONAnim_Combo_HeartLocket = new JSONAnimator(_Anim_L4S5_BambooStrainer, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JSONAnim_Combo_HeartLocket->parseAndLoadJSON("ANIM\\L6\\HEARTLOCKETCOMBO.JSON", ComboScene);
		JSONAnim_Combo_HeartLocket->PlayAnim();
		JSONAnim_Combo_HeartLocket->StopAnim();
		JSONAnim_Combo_HeartLocket->SetAnimTime(0);
	}
	else if(invId == eInv_L6_S4_Rope)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L6\\BIG\\ROPEHORSESHOECOMBO\\ROPEHORSESHOE.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(343.0f,94.0f,695.0f,529.0f);

		JSONAnim_Combo_RopeHorseShoe = new JSONAnimator(_Anim_L6S4_RopeHorseShoe, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JSONAnim_Combo_RopeHorseShoe->parseAndLoadJSON("ANIM\\L6\\ROPEHORSESHOECOMBO.JSON", ComboScene);
		JSONAnim_Combo_RopeHorseShoe->PlayAnim();
		JSONAnim_Combo_RopeHorseShoe->StopAnim();
		JSONAnim_Combo_RopeHorseShoe->SetAnimTime(0);
	}
	
	


#pragma endregion

#pragma region Level 7
	else if(invId == eInv_L7_S1_PotEmpty)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L7\\S1POT\\POT.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(312.0f,71.0f,760.0f,546.0f);

		JAnim_PotChainTry = new JSONAnimator(_Anim_L7S1_PotChainTry, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_PotChainTry->parseAndLoadJSON("ANIM\\L7\\S1POTCHAINTRY.JSON", ComboScene);

		JAnim_PotWireFix = new JSONAnimator(_Anim_L7S1_PotWireFix, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_PotWireFix->parseAndLoadJSON("ANIM\\L7\\S2POTWIREFIX.JSON", ComboScene);

		ComboScene->GetObjectByID("chainhang")->SetEnable(false);
		ComboScene->GetObjectByID("metalwirefix")->SetEnable(false);
		ComboScene->GetObjectByID("handle")->SetEnable(false);
		ComboScene->GetObjectByID("metalwire")->SetEnable(false);
		if(!ProfileSystem::Instance->ActiveProfile.L7_S1_ComboPotChainPlaced)
		{
			ComboScene->GetObjectByID("chain")->SetEnable(false);
		}
	}
	else if(invId == eInv_L7_S2_SphereHalfBase)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L7\\S2SPHERE\\GLOBE.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);

		ComboBoundaryRect = new CRectangle(312.0f,71.0f,760.0f,546.0f);

		JAnim_GlobeFix = new JSONAnimator(_Anim_L7S2_GlobeFix, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		JAnim_GlobeFix->parseAndLoadJSON("ANIM\\L7\\S2SPHEREJOIN.JSON", ComboScene);

		ComboScene->GetObjectByID("glow")->SetEnable(false);
		ComboScene->GetObjectByID("globefixed")->SetEnable(false);
		ComboScene->GetObjectByID("globefix")->SetEnable(false);
	}
#pragma endregion

#pragma region Level 8
	else if(invId == eInv_L8_S1_MagicWand)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L8\\MAGICWAND\\MW.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);
		
		isMagicWandKeyInHand = false;

		ComboBoundaryRect = new CRectangle(312.0f,71.0f,760.0f,546.0f);

		Janim_MagicWandTool = new JSONAnimator(_Anim_L8S2_MagicWand1, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		Janim_MagicWandTool->parseAndLoadJSON("ANIM\\L8\\L8_CB_MAGICWANDKEY.JSON", ComboScene);

		Janim_MagicWand = new JSONAnimator(_Anim_L8S2_MagicWand2, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		Janim_MagicWand->parseAndLoadJSON("ANIM\\L8\\L8_CB_MAGICWAND.JSON", ComboScene);

		ComboScene->GetObjectByID("Shukra-key_Large")->SetEnable(false);
		ComboScene->GetObjectByID("Shukra-key01-")->SetEnable(false);
		ComboScene->GetObjectByID("Shukra-key02")->SetEnable(false);
	}

	else if(invId == eInv_L8_S2_HalfKey)
	{
		ComboScene = new CHOScene();
		ComboScene->Load("INV\\L8\\COMBOKEY\\COMBOKEY.SCN");
		ComboScene->SetZOff(eZOrder_ComboInvScene+10);
		ComboScene->Show(false);
		
		isMagicWandKeyInHand = false;

		ComboBoundaryRect = new CRectangle(312.0f,71.0f,760.0f,546.0f);

		Janim_KeyFix = new JSONAnimator(_Anim_L8S2_Key, CC_CALLBACK_1(ComboInvSystem::OnAnimationComplete, this));
		Janim_KeyFix->parseAndLoadJSON("ANIM\\L8\\COMBOKEY.JSON", ComboScene);
		Janim_KeyFix->SetAnimTime(0);


	}

#pragma endregion




	if(ComboBoundaryRect == NULL)
	{
		//just in case no popups are active
		ComboBoundaryRect = new CRectangle(233.0f,37.0f,891.0f,607.0f);
	}

#ifdef _CEBUILD
	GFHud->HideMorphObjects();
#endif //CE_BUILD

	if(isSkipTransition)
	{
		//directly show the combo
		PopupAlpha = 1.0f;

		Visible = true;
		ComboMaskBg->SetAlpha(PopupAlpha);
		ComboMaskBg->Show(true);

		ComboScene->Show(true);
		ComboState = JRN_POPPED;
	}
	else
	{
		PopupAlpha = 0.2f;

		Visible = true;
		ComboState = JRN_POPUP;
		IsPopTransitionActive = true;
		InitRenderTarget(ComboScene);

		ComboMaskBg->SetAlpha(0);
		ComboMaskBg->Show(true);

		ComboCloseBtn->SetAlpha(0);

		GFInstance->SetPopupTween( popOffsetX, popOffsetY, 171, 0);
	}
}

void ComboInvSystem::TaskPendingUpdate()
{
	if(_invId == eInv_L2S4_AxeBlunt)
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeSharpenTaken)
		{
			//if(!ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown)
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd] = true;
			//surely this one
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S4_Cave] = true;
		}
	}
	else if(_invId == eInv_L2S6_KnifeBlunt)
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken)
		{
			if(ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_ForestEntrance] = true;

			if(ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced || ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced || ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced)
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] = true;
		}
	}
	else if(_invId == eInv_L3S1_LadderBroken)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderFixedTaken)
		{
			if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S2_FortInterior])
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance] = true;
		}
	}
	else if(_invId == eInv_L3S2_Hammer)
	{
		//GFHud->ShowBannerDirectText("DEBUG: Into Hammer Task pending update ...");
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerTaken )
		{
			//GFHud->ShowBannerDirectText("DEBUG: Hammer Taken inside TaskPending Udpate...");
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = true;
			if(ProfileSystem::Instance->ActiveProfile.L3_S5_NailPlaced)
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S5_Cliff] = true;
		//GFHud->ShowBannerDirectText("DEBUG: Going out of Hammer TaskPending update here ...");
		}
	}
	else if(_invId == eInv_L3S2_CocoShellPowderFilled)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutWithTwineTaken)
		{
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_FortInterior] = true;
		}
	}
	else if(_invId == eInv_L3S2_CoconutFull)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutShellTaken)
		{
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_FortInterior] = true;
		}
	}
	else if(_invId == eInv_L3S3_Box)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvSnakeDoorPartTaken)
		{
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S4_FortFountain])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S4_FortFountain] = true;
			} 
		}
	}	
	else if(_invId == eInv_L4_S2_Bamboo)
	{
		if(ProfileSystem::Instance->ActiveProfile.L4_S2_InvBambooSpearTaken)
		{
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L4_S2_RiverSide])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide] = true;
			} 
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L4_S5_HillSide])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S5_HillSide] = true;
			}
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S1_SwampEntrance] = true;
		}
	}
	else if(_invId == eInv_L4_S4_BinocularWithoutLens)
	{
		if(ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularWithLensTaken)
		{
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L4_S4_HillTop])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;
			} 
		}
	}
	else if(_invId == eInv_L4_S2_GrassBunch)
	{
		if(ProfileSystem::Instance->ActiveProfile.L4_S2_InvRopeTaken)
		{
			/*if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L4_S2_RiverSide])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide] = true;
			} */
		}
	}	
	else if(_invId == eInv_L4_S5_Net)
	{
		if(ProfileSystem::Instance->ActiveProfile.L4_S5_InvBambooStrainerTaken)
		{
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L4_S4_HillTop])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;
			} 
		}
	}
	else if(_invId == eInv_L5_S4_PickaxeBlade)
	{
		if (ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeTaken)
		{
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S4_Dungeon] = true;
		}
	}
	else if(_invId == eInv_L6_S4_Rope)
	{
		if(ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeHorseShoeComboTaken)
		{
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S1_FloatingIsland1])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S1_FloatingIsland1] = true;
			} 
		}
	}
	else if(_invId == eInv_L6_S4_HeartContainer)//TODO:sujin
	{
		/*if(ProfileSystem::Instance->ActiveProfile.L4_S5_InvBambooStrainerTaken)
		{
		if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L4_S4_HillTop])
		{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;
		} 
		}*/
	}
	else if(_invId == eInv_L7_S1_PotEmpty)
	{
		if(ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotWithChainTaken)
		{
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S1_Volcano])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano] = true;
			} 
		}
	}
	else if(_invId == eInv_L7_S2_SphereHalfBase)
	{
		if(ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereFullTaken)
		{
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S2_LasyaPrison])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S2_LasyaPrison] = true;
			} 
		}
	}
	else if (_invId == eInv_L8_S1_MagicWand)
	{
		if (ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyTaken)
		{
			if (ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L8_S1_ShukrasPlace])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S1_ShukrasPlace] = true;
			}
		}
	}
	else if (_invId == eInv_L8_S2_HalfKey)
	{
		if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey])
		{
			if (ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L8_S2_Garden])
			{
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] = true;
			}
		}
	}
}

bool ComboInvSystem::ShowHint()
{
	bool isHintLeft = false;

	if( _invId == eInv_L1S1_Bag )
	{
		if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken)
		{
			GFHud->InitHintTrailEmit(BagZipperPoly->center.x, BagZipperPoly->center.y);
			isHintLeft = true;
		}
		else if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken &&  !ProfileSystem::Instance->ActiveProfile.L1S1_BagOpened)
		{
			if (ComboPopState != JRN_POPPED)
			{
				GFHud->InitHintTrailEmit(&PopRectLocket);
				isHintLeft = true;
			}
			else
			{
				for (int i = 0; i < MAX_INVBOX_OBJECTS; ++i)
				{
					if (GFHud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL)
					{
						if (GFHud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_RubyPart1)
						{
							if ((i - ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS)//greater than scene, make it rightmost element!!
								GFHud->InvBoxCtrl->ShiftInvBoxLeft(i - (MAX_INVBOX_DISPLAYED_OBJECTS - 1) - ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
							else if (i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj)//lesser than scene, make it leftmost element!!
								GFHud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj - i);

							GFHud->InitHintTrailEmit(GFHud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, GFHud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y, false);
							isHintLeft = true;
						}
					}
				}
			}
		}
		else if( ProfileSystem::Instance->ActiveProfile.L1S1_BagOpened &&  !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvLetterCreekTaken)
		{
			GFHud->InitHintTrailEmit(&ActionRectKidDrawing);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
		if(ComboPopState == JRN_POPPED)
		{
			if (!ProfileSystem::Instance->ActiveProfile.L1S1_LocketAllPartsFixed && ProfileSystem::Instance->ActiveProfile.L1S1_LocketPartsPlaced)
			{
				GFHud->InitHintTrailEmit(&PopRectLocket);
				isHintLeft = true;
			}
		}
	}
	else if(_invId == eInv_L1S4_BeetleHead)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L1S4_BeetleTail, &InvDestInfoRectBeetle);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L1S4_Diary)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDiaryPageTurned)
		{
			GFHud->InitHintTrailEmit(&NextPageRect);
			isHintLeft = true;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvCoinTaken)
		{
			GFHud->InitHintTrailEmit(&InvItemCoinRect);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L1S4_PhotoFrame)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoTurned)
		{
			GFHud->InitHintTrailEmit(&LeftArrowRect);
			isHintLeft = true;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsPhotoPaperTeared)
		{
			GFHud->InitHintTrailEmit(&ActionRectTearPaper);
			isHintLeft = true;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken)
		{
			GFHud->InitHintTrailEmit(&InvItemRectBeetlePart);
			isHintLeft = true;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvBookCodeTaken)
		{
			GFHud->InitHintTrailEmit(&InvItemRectBookCode);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L1S5_HammerHead)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L1S5_HammerHandle, &InvDestRectHammerHand);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L2S4_AxeBlunt)
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvStoneSharpnerTaken && !ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeSharpenTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L2S5_StoneSharpener, &InvDestRectAxeSharpen);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L2S6_KnifeBlunt)
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_InvVinegarTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarPlaced)
		{
			GFHud->ShowHintForInvItem(eInv_L2S5_VinegarBottle, &InvDestRectVinegarBottle);
			isHintLeft = true;
		}
		else if(ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarCorkOpened)
		{
			GFHud->InitHintTrailEmit(&ActionRectCorkOpen);
			isHintLeft = true;
		}
		else if(ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarCorkOpened && !ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpened)
		{
			GFHud->InitHintTrailEmit(&ActionRectVinegarBottle);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L3S1_FlagFull)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagClothTaken && ProfileSystem::Instance->ActiveProfile.L3_S1_Objectives[S1_Objctive_PickFlag])
		{
			GFHud->InitHintTrailEmit(&InvItemRect);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L3S1_LadderBroken)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderPiecesFixed && (ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken && ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken))
		{
			GFHud->ShowHintForInvItem(eInv_L3S1_LadderPiece1, &InvDestRectLadder);
			isHintLeft = true;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderBrokenStepFixed && ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagClothTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L3S1_FlagCloth, &InvDestRectLadder);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L3S2_CoconutFull)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved && ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L3S4_SharpTool, &InvDestRectCoconut);
			isHintLeft = true;
		}
		else if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved && !ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutShellTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L3S2_Sickle, &InvDestRectCoconut);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L3S2_CocoShellPowderFilled)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S2_CoconutComboTwineFixed && ProfileSystem::Instance->ActiveProfile.L3_S2_InvTwineTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L3S2_Twine, &InvDestCoconutShell);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L3S3_Box)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvSnakeDoorPartTaken)
		{
			GFHud->InitHintTrailEmit(&InvItemRectBoxNagaKey);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L3S2_HammerHead)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHandTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L3S2_HammerHandle, &InvDestRectHammerHead);
			isHintLeft = true;
		}
		else
		{
			GFHud->InitHintTrailEmit(&BtnRectClose, true);
			isHintLeft = true;
		}
	}
	else if (_invId == eInv_L4_S1_Leaf)
	{
		if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvStickTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L4_S1_Stick, &InvDestRectStick);
			isHintLeft = true;
		}

	}
	else if (_invId == eInv_L4_CB_HandKerchief)
	{
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvKnifeCollected)
		{
			GFHud->ShowHintForInvItem(eInv_L4_S1_Knife, &InvDestRectKnife);
			isHintLeft = true;
		}
	}
	else if (_invId == eInv_L4_S2_Bamboo)
	{
		if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife] &&!ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo)
		{
			GFHud->ShowHintForInvItem(eInv_L4_S1_Knife, &InvDestRectBambooKnife);
			isHintLeft = true;
		}
		else if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_KerchiefPiece2] && !ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] && ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo)
		{
			GFHud->ShowHintForInvItem(eInv_L4_S1_KerchiefPiece2, &InvDestRectBambooKnife);
			isHintLeft = true;
		}
	}
	else if (_invId == eInv_L4_S4_BinocularWithoutLens)
	{
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvBinocularLensTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L4_S1_BinocularLens, &InvDestRectBinocularLens);
			isHintLeft = true;
		}
	}
	else if (_invId == eInv_L4_S2_GrassBunch)
	{
		if (!ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_GrassRope])
		{
			GFHud->InitHintTrailEmit(&InvDestRectGrassRope);
			isHintLeft = true;
		}			
	}
	else if (_invId == eInv_L4_S5_Net)
	{
		if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_BambooStick1] &&!ProfileSystem::Instance->ActiveProfile.L4_S5_ComboBambooFrameAddedToNet)
		{
			GFHud->ShowHintForInvItem(eInv_L4_S2_BambooStick1, &InvDestRectBambooStrainer);
			isHintLeft = true;
		}
		else if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_GrassRope] &&!ProfileSystem::Instance->ActiveProfile.L4_S5_InvBambooStrainerTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L4_S2_GrassRope, &InvDestRectBambooStrainer);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L5_S1_Bow)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowStringFixed)
		{
			GFHud->InitHintTrailEmit(&ActionRectBowStringFix);
			isHintLeft = true;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowArrowHolderPlaced && ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L5_S1_ArrowHolder, &InvDestRectBow);
			isHintLeft = true;
		}
		else if(ProfileSystem::Instance->ActiveProfile.L5_S1_BowArrowHolderPlaced && (!ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowFirstTaken || !ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowSecondTaken))
		{
			GFHud->InitHintTrailEmit(&ActionRectBowArrowTake);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L5_S4_PickaxeBlade)
	{
		if( !ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeTaken && ProfileSystem::Instance->ActiveProfile.L5_S5_InvPickaxeHandleTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L5_S5_PickaxeHandle, &InvDestRectPickaxe);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L5_S4_PeacockChain)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L5_S6_InvPeacockLocketTaken && ProfileSystem::Instance->ActiveProfile.L5_S6_PeacockChainUsed)
		{
			GFHud->InitHintTrailEmit(&ActionRectLocket);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L6_S4_Rope)
	{
		if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_MagnetisedHorseShoe])
		{
			GFHud->ShowHintForInvItem(eInv_L6_S4_MagnetisedHorseShoe, &InvDestRectRopeHorseShoe);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L7_S1_PotEmpty)
	{
		if( ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken && !ProfileSystem::Instance->ActiveProfile.L7_S1_ComboPotChainPlaced )
		{
			GFHud->ShowHintForInvItem(eInv_L7_S3_MetalChain, &InvDestRectL7PotMetalWire);
			isHintLeft = true;
		}
		else if( ProfileSystem::Instance->ActiveProfile.L7_S1_ComboPotChainPlaced && ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken 
			&& !ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotWithChainTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L7_S3_MetalWire, &InvDestRectL7PotMetalWire);
			isHintLeft = true;
		}
	}
	else if(_invId == eInv_L7_S2_SphereHalfBase)
	{
		if(ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfLeftToFixTaken && !ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereFullTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L7_S2_SphereHalfToFix, &InvDestRectL7Globe);
			isHintLeft = true;
		}
	}
	else if (_invId == eInv_L8_S1_MagicWand)
	{
		if(ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken && !ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyTaken)
		{
			GFHud->ShowHintForInvItem(eInv_L8_S2_Tool, &InvDestRectL8MagicWandTool);
			isHintLeft = true;
		}
	}
	else if (_invId == eInv_L8_S2_HalfKey)
	{
		if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_HalfKey] && !ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey])
		{
			GFHud->ShowHintForInvItem(eInv_L8_S1_HalfKey, &InvDestRectL8ComboKey);
			isHintLeft = true;
		}
	}

	return isHintLeft;
}

void ComboInvSystem::OnAnimationComplete(int animID)
{
	if(animID == _Anim_L2S4_AxeSharpen)
	{
		isAnyAnimActive = false;
		isAxeSharpenAnimPlaying = false;

		//Hammer done
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_AxeBlunt] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_AxeBlunt);

		ComboScene->GetObjectByID("sharpedge")->SetEnable(false);
		ComboScene->GetObjectByID("bluntaxe")->SetEnable(false);

		ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeSharpenTaken = true;
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L2S4_AxeSharpen);
	}
	else if(animID == _Anim_L2S5_CorkRemove)
	{
		isAnyAnimActive = false;
		ComboScene->GetObjectByID("corkmask")->SetEnable(false);
	}
	else if(animID == _Anim_L2S5_RustRemove)
	{
		isAnyAnimActive = false;		

		//Hammer done
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_KnifeBlunt] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_KnifeBlunt);

		ComboScene->GetObjectByID("KnifeSharpen")->SetEnable(false);

		ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken = true;
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L2S6_KnifeSharpen);
	}
	else if(animID == _anim_L3S1_FlagComboRemove)
	{
		isAnyAnimActive = false;

		ComboScene->GetObjectByID("Flag_unTied")->FadeOut(0.8f);

		//Broken Ladder done
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S1_FlagFull] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S1_FlagFull);

		ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagClothTaken = true;
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L3S1_FlagCloth);

	}
	else if(animID == _Anim_L4S2_BambooKnife)
	{
		isAnyAnimActive = false;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_KerchiefPiece2] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_KerchiefPiece2);
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_Bamboo] = false;
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S2_Bamboo);
		ComboScene->GetObjectByID("Bamboo")->FadeOut(0.8f);
		ComboScene->GetObjectByID("Knife")->FadeOut(0.8f);
		ComboScene->GetObjectByID("Kerchief")->FadeOut(0.8f);

		ProfileSystem::Instance->ActiveProfile.L4_S2_InvBambooSpearTaken = true;
		GFHud->TakeInventoryItem(eInv_L4_CB_BambooWithKnife);
		TaskPendingUpdate();
	}
	else if(animID == _Anim_L4S2_GrassRope)
	{
		isAnyAnimActive = false;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_GrassBunch] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S2_GrassBunch);
		rope->Paused = false;
		rope->FadeOut(0.8f);//TODO:sujin

		ProfileSystem::Instance->ActiveProfile.L4_S2_InvRopeTaken = true;
		GFHud->TakeInventoryItem(eInv_L4_S2_GrassRope);
		TaskPendingUpdate();
	}
	else if(animID == _Anim_L4S5_BambooStrainer)
	{
		isAnyAnimActive = false;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_GrassRope] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S2_GrassRope);
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_Net] = false;
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S5_Net);
		ComboScene->GetObjectByID("Bamboo")->FadeOut(0.8f);
		ComboScene->GetObjectByID("Net")->FadeOut(0.8f);
		ComboScene->GetObjectByID("Rope1")->FadeOut(0.8f);
		ComboScene->GetObjectByID("Rope2")->FadeOut(0.8f);
		ComboScene->GetObjectByID("Rope3")->FadeOut(0.8f);
		ComboScene->GetObjectByID("Rope4")->FadeOut(0.8f);
		ProfileSystem::Instance->ActiveProfile.L4_S5_InvBambooStrainerTaken = true;
		GFHud->TakeInventoryItem(eInv_L4_S2_Strainer);
		TaskPendingUpdate();
	}
	else if(animID == _Anim_L3S1_LadderPartsPlacing)
	{
		isAnyAnimActive = false;

		GFHud->InvBoxCtrl->updateComboItemStatus(_invId);

		if (ProfileSystem::Instance->ActiveProfile.L3_S1_LadderBrokenStepFixed)
		{
			ComboScene->SetSceneAlpha(0);

			//Broken Ladder done
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S1_LadderBroken] = false;
			GFHud->ReleaseDrag();
			GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S1_LadderBroken);

			ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderFixedTaken = true;
			TaskPendingUpdate();
			GFHud->TakeInventoryItem(eInv_L3S1_LadderFixed);
		}
	}
	else if(animID == _Anim_L3S2_CoconutOuterShell)
	{
		isAnyAnimActive = false;

		ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved = true;

		GFHud->InvBoxCtrl->updateComboItemStatus(_invId);

	}

	else if(animID == _Anim_L3S2_CoconutBreak)
	{
		isAnyAnimActive = false;

		ComboScene->SetSceneAlpha(0);

		//Broken Ladder done
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_CoconutFull] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_CoconutFull);

		ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutShellTaken = true;
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L3S2_CoconutShell);
	}
	else if(animID == _Anim_L3S2_CoconutShellThreadPlace)
	{
		isAnyAnimActive = false;

		ComboScene->SetSceneAlpha(0);

		//Coconut done
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_CocoShellPowderFilled] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_CocoShellPowderFilled);

		ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutWithTwineTaken = true;
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L3S2_CoconutDynamite);
	}
	else if(animID == _Anim_L3S2_HammerFix)
	{
		//GFHud->ShowBannerDirectText("DEBUG: Hammer Head is fixed here ...");
		isAnyAnimActive = false;

		ComboScene->SetSceneAlpha(0);

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_HammerHead] = false;
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_HammerHead);
	// Sac: this is a check if the eInv_L3S2_HammerHandle is there or not already
		// as it is already getting removed once, why remove it again ?
		// removing this without this check will crash the game
		if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_HammerHandle])
		{
   			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_HammerHandle] = false;
        	GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_HammerHandle);
		}
		GFHud->ReleaseDrag();

		ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerTaken = true;
		//GFHud->ShowBannerDirectText("DEBUG: Hammer Head is fixed here - getting omtp TaskPendingUpdate ...");
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L3S2_Hammer);
	}
	else if (animID == _Anim_L4S1_LeafSpoonMaking)
	{
		isAnyAnimActive = false;

		ComboScene->SetSceneAlpha(0);

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Leaf] = false;
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_Leaf);


		ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonCollected = true;
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L4_S1_LeafSpoon);
	}
	else if (animID == _Anim_L5S1_ArrowStringFix)
	{
		isAnyAnimActive = false;
	}
	else if (animID == _Anim_L5S1_ArrowBowFix)
	{
		isAnyAnimActive = false;

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S1_Bow] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S1_Bow);

		ComboScene->GetObjectByID("bow")->SetEnable(false);
		ComboScene->GetObjectByID("threadfinal")->SetEnable(false);
		ComboScene->GetObjectByID("arrow")->SetEnable(false);

		if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowFirstTaken)
		{
			ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowFirstTaken = true;
			GFHud->SendUpdateSceneMessage();
		}
		else
		{
			ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowSecondTaken = true;
		}
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L5_S1_BowArrowFixed);
	}
	else if (animID == _Anim_L5S4_PickaxeFix)
	{
		isAnyAnimActive = false;

		ComboScene->SetSceneAlpha(0);

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S5_PickaxeHandle] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S5_PickaxeHandle);

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S4_PickaxeBlade] = false;
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S4_PickaxeBlade);

		ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeTaken = true;
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L5_S4_Pickaxe);
	}
	else if(animID == _Anim_L5S6_Locket)
	{
		isAnyAnimActive = false;


		//locket takeing
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S4_PeacockChain] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S4_PeacockChain);

		ProfileSystem::Instance->ActiveProfile.L5_S6_InvPeacockLocketTaken = true;
		TaskPendingUpdate();
		GFHud->TakeInventoryItem(eInv_L5_S6_Locket);

	}

	else if(animID == _Anim_L6S4_HeartLocket)
	{
		isAnyAnimActive = false;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_HeartContainer] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_HeartContainer);
		// Sac: this is a check if the eInv_L6_S3_GlowingHeart is there or not already
		// as it is already getting removed once, why remove it again ?
		// removing this without this check will crash the game
		if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_GlowingHeart])
		{
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_GlowingHeart] = false;
			GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S3_GlowingHeart);
		}
		ComboScene->GetObjectByID("Combo")->FadeOut(0.8f);


		//ProfileSystem::Instance->ActiveProfile.L6_s4_ = true;
		GFHud->TakeInventoryItem(eInv_L6_CB_HeartContainerWithCrystal);
		TaskPendingUpdate();
	}
	else if(animID == _Anim_L6S4_RopeHorseShoe)
	{
		isAnyAnimActive = false;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_Rope] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_Rope);
		// Sac: this is a check if the eInv_L6_S4_MagnetisedHorseShoe is there or not already
		// as it is already getting removed once, why remove it again ?
		// removing this without this check will crash the game
		if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_MagnetisedHorseShoe])
		{
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_MagnetisedHorseShoe] = false;
			// Sac Magenitisedhorseshoe is removed again here..
			GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_MagnetisedHorseShoe);
		}
		ComboScene->GetObjectByID("Combo")->FadeOut(0.8f);
		ComboScene->GetObjectByID("Rope")->FadeOut(0.8f);
		ComboScene->GetObjectByID("HorseShoe")->FadeOut(0.8f);


		ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeHorseShoeComboTaken = true;
		GFHud->TakeInventoryItem(eInv_L6_CB_RopeWithMagnetisedHorseShoe);
		TaskPendingUpdate();
	}
	else if(animID == _Anim_L7S1_PotChainTry)
	{
		isAnyAnimActive = false;
	}
	else if(animID == _Anim_L7S1_PotWireFix)
	{
		isAnyAnimActive = false;

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S3_MetalWire] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S3_MetalWire);

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S1_PotEmpty] = false;
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S1_PotEmpty);

		ComboScene->SetSceneAlpha(0);

		ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotWithChainTaken = true;
		GFHud->TakeInventoryItem(eInv_L7_S1_PotWithChain);	
		TaskPendingUpdate();
	}
	else if(animID == _Anim_L7S2_GlobeFix)
	{
		isAnyAnimActive = false;

		ComboScene->SetSceneAlpha(0);

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S2_SphereHalfToFix] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S2_SphereHalfToFix);

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S2_SphereHalfBase] = false;
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S2_SphereHalfBase);

		ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereFullTaken = true;
		GFHud->TakeInventoryItem(eInv_L7_S2_SphereFull);	

		GFHud->SendUpdateSceneMessage();
		TaskPendingUpdate();
	}
	else if(animID == _Anim_L8S2_MagicWand1)
	{
		isAnyAnimActive = false;
		isMagicWandKeyInHand = true;

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_Tool] = false;
		GFHud->ReleaseDrag();
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S2_Tool);

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_MagicWand] = false;
		GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S1_MagicWand);

		ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyTaken = true;
		GFHud->TakeInventoryItem(eInv_L8_S1_DoorKey);		

		TaskPendingUpdate();
	}
	else if(animID == _Anim_L8S2_MagicWand2)
	{
		isAnyAnimActive = false;

		ProfileSystem::Instance->ActiveProfile.L8_S1_MagicWandWithoutKeyTaken = true;
		GFHud->TakeInventoryItem(eInv_L8_CB_MagicWandWithoutKey,CPoint(ComboScene->GetObjectByID("Shukra-key_Large")->GetCenterX() - 100,ComboScene->GetObjectByID("Shukra-key_Large")->GetCenterY(),0));

		ComboScene->SetSceneAlpha(0);

		TaskPendingUpdate();
	}

	else if(animID == _Anim_L8S2_Key)
	{
		isAnyAnimActive = false;
		// Sac: experienced a crash at this place when the half keys are removed
		// so implementing a check before removing it.
		if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_HalfKey]){
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_HalfKey] = false;
			GFHud->ReleaseDrag();
			GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S1_HalfKey);
		}
		if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_HalfKey]){
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_HalfKey] = false;
			GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S2_HalfKey);
		}
		ComboScene->GetObjectByID("keyHalf1")->FadeOut(0.8f);
		ComboScene->GetObjectByID("keyHalf2")->FadeOut(0.8f);

		GFHud->TakeInventoryItem(eInv_L8_CB_FullKey);
		TaskPendingUpdate();
		return;
	}
}

void ComboInvSystem::HandleInventoryDrop()
{
	CPoint dropPos;
	int itemVal;
	if( GFHud->GetDropInfo(dropPos, itemVal))
	{
		bool isItemUsed = false;
		if(_invId == eInv_L1S1_Bag)
		{
			//only Ruby parts can be placed
			if(ComboPopState == JRN_POPPED)
			{
				if( itemVal == eInv_L1S1_RubyPart1 )
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken &&
						ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken)
					{
						if(InvDestRectRubyParts.Intersection(&dropPos))
						{
							isItemUsed = true;
							//All Ruby Taken
							ProfileSystem::Instance->ActiveProfile.L1S1_LocketPartsPlaced = true;

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BagLockPartPlaced);

							for (int i = 0; i < 4; i++){					
								ruby_Parts[i]->SetEnable(true);
							}

							GFHud->ReleaseDrag();
							GFHud->CloseInventory();

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_RubyPart1] = false;
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_RubyPart2] = false;
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_RubyPart3] = false;
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_RubyPart4] = false;

							GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S1_RubyPart1);
						}
					}
					else
					{
						isItemUsed = true;

						//At least some thing is taken
						GFHud->ShowBannerText("L1all");

						GFHud->ReleaseDrag();
						GFHud->CloseInventory(); 
					}
				}
				else if( itemVal == eInv_L1S1_SwissKnife )
				{
					isItemUsed = true;

					GFHud->ShowBannerText("L1bag");
					GFHud->ReleaseDrag();
					GFHud->CloseInventory(); 
				}
			}
			else if ( !ProfileSystem::Instance->ActiveProfile.L1S1_LocketAllPartsFixed && BigPopRectLocket.Intersection(&dropPos) )
			{
				isItemUsed = true;

				ComboPopMaskBg->SetAlpha(0);
				ComboPopMaskBg->Show(true);

				PopupAlpha = 0.2f;
				ComboPopState = JRN_POPUP;
				IsPopTransitionActive = true;
				InitRenderTarget(ComboPopupScene);
				GFInstance->SetPopupTween( 0, -50, 171, 0);

				PopCloseBtn = ComboPopupScene->GetObjectByID("popClose");
			}
		}
		else if(_invId == eInv_L1S4_BeetleHead)
		{			
			if( itemVal == eInv_L1S4_BeetleTail )
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleTailFixed)
				{
					if(InvDestRectBeetleTail.Intersection(&dropPos))
					{
						isItemUsed = true;

						ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleTailFixed = true;

						GFHud->ReleaseDrag();
						GFHud->CloseInventory();

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_BeetleTail] = false;
						GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S4_BeetleTail);

						isBeetleJoinPaying = true;
						CAnim_BeetleJoin->PlayAnim();

						isAnyAnimActive = true;

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4BeetleJoin);

						if( GFHud->Tutor != NULL && GFHud->Tutor->IsActive && GFHud->Tutor->State == TUT19_ComboInvUseCombine)
						{
							ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT18_ComboInvZoomLook] = true;
							ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT19_ComboInvUseCombine] = true;
							GFHud->Tutor->HideTutorial();
						}
					}
				}
			}
		}
		else if(_invId == eInv_L1S5_HammerHead)
		{
			if( itemVal == eInv_L1S5_HammerHandle )
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsHammerHandFixed)
				{
					if(InvDestRectHammerHand.Intersection(&dropPos))
					{
						ProfileSystem::Instance->ActiveProfile.L1S5_IsHammerHandFixed = true;

						GFHud->ReleaseDrag();
						GFHud->CloseInventory();

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_HammerHandle] = false;
						GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S5_HammerHandle);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5HammerJoin);

						isHammerJoinPaying = true;
						CAnim_HammerJoin->PlayAnim();
						isAnyAnimActive = true;

						isItemUsed = true;
					}
				}
			}
		}
		else if(_invId == eInv_L2S4_AxeBlunt)
		{
			if( itemVal == eInv_L2S5_StoneSharpener)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L2_S4_IsAxeSharpened)
				{
					if(InvDestRectAxeSharpen.Intersection(&dropPos))
					{
						isItemUsed = true;

						ProfileSystem::Instance->ActiveProfile.L2_S4_IsAxeSharpened = true;

						GFHud->ReleaseDrag();
						GFHud->CloseInventory();

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S5_StoneSharpener] = false;
						GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S5_StoneSharpener);

						//Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5HammerJoin);

						isItemUsed = true;

						isAxeSharpenAnimPlaying = true;
						JAnim_AxeSharpen->PlayAnim();
						isAnyAnimActive = true;
					}
				}
			}
		}
		else if(_invId == eInv_L2S6_KnifeBlunt)
		{
			if( itemVal == eInv_L2S5_VinegarBottle)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarPlaced)
				{
					if(InvDestRectVinegarBottle.Intersection(&dropPos))
					{
						ProfileSystem::Instance->ActiveProfile.L2_S5_VinegarPlaced = true;

						ComboScene->GetObjectByID("corkmask")->SetEnable(true);
						ComboScene->GetObjectByID("corkfull")->SetEnable(true);
						ComboScene->GetObjectByID("bottle")->SetEnable(true);

						GFHud->ReleaseDrag();
						GFHud->CloseInventory();

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S5_VinegarBottle] = false;
						GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S5_VinegarBottle);

						//Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5HammerJoin);

						isItemUsed = true;
					}
				}
			}
		}
		else if(_invId == eInv_L3S1_LadderBroken)
		{
			if(isAnyAnimActive)
				return;

			if( itemVal == eInv_L3S1_LadderPiece1)
			{			
				if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken || !ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken)
				{
					isItemUsed = true;
					GFHud->ShowBannerText("L3piece");

					GFHud->ReleaseDrag();
					GFHud->CloseInventory();
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderPiecesFixed)
				{
					if(InvDestRectLadder.Intersection(&dropPos))
					{
						ProfileSystem::Instance->ActiveProfile.L3_S1_LadderPiecesFixed = true;

						Control::Audio->QuickLoadAndPlaySFX("combo_ladderpiecesplaced");

						JAnim_PiecesPlacing->PlayAnim();
						isAnyAnimActive = true;

						GFHud->ReleaseDrag();
						GFHud->CloseInventory();

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S1_LadderPiece1] = false;
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S3_LadderPiece2] = false;
						GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S1_LadderPiece1);

						isItemUsed = true;
					}
				}
			}
			else if( itemVal == eInv_L3S1_FlagCloth)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L3_S1_LadderBrokenStepFixed)
				{
					if(InvDestRectLadder.Intersection(&dropPos))
					{
						ProfileSystem::Instance->ActiveProfile.L3_S1_LadderBrokenStepFixed = true;

						Control::Audio->QuickLoadAndPlaySFX("combo_clothtied");

						ladderClothFadingIn = true;
						isAnyAnimActive = true;

						ComboScene->GetObjectByID("stepbroken")->FadeOut(2.0f);
						ComboScene->GetObjectByID("stepfixed")->SetEnable(true);
						ComboScene->GetObjectByID("stepfixed")->FadeIn(3.0f);

						GFHud->ReleaseDrag();
						GFHud->CloseInventory();

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S1_FlagCloth] = false;
						GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S1_FlagCloth);

						isItemUsed = true;
					}
				}
			}
		}
		else if(_invId == eInv_L3S2_CoconutFull)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvCoconutOuterRemoved)
			{
				if( itemVal == eInv_L3S4_SharpTool)
				{
					if(InvDestRectCoconut.Intersection(&dropPos))
					{
						Control::Audio->QuickLoadAndPlaySFX("combo_coconutpeel");

						JAnim_CoconutOuterRemove->PlayAnim();
						isAnyAnimActive = true;

						GFHud->ReleaseDrag();
						GFHud->CloseInventory();

						ProfileSystem::Instance->ActiveProfile.Inventory[itemVal] = false;
						GFHud->InvBoxCtrl->ClearItemFromInvBox(itemVal);

						isItemUsed = true;
					}
				}
				else if(itemVal == eInv_L3S2_Sickle || itemVal == eInv_L3S3_Saw || itemVal == eInv_L3S1_Spear)
				{
					GFHud->ShowBannerText("L3pointy");
					Control::Input->ClearMouse();//Avoid unwanted click message handlers!!
					GFHud->CancelDrag();//just cancel so that user can choose another one if invbox is open!!
					isItemUsed = true;
				}
			}
			else
			{
				 if(itemVal == eInv_L3S2_Sickle)
				 {
					 if(InvDestRectLadder.Intersection(&dropPos))
					 {
						 Control::Audio->QuickLoadAndPlaySFX("combo_coconutbreak");

						 JAnim_CoconutBreak->PlayAnim();
						 isAnyAnimActive = true;

						 GFHud->ReleaseDrag();
						 GFHud->CloseInventory();

						 isItemUsed = true;
					 }
				 }
				 else if( itemVal == eInv_L3S3_Saw)
				 {
					 //if needed in future
				 }
			}
		}
		else if(_invId == eInv_L3S2_CocoShellPowderFilled)
		{
			if( itemVal == eInv_L3S2_Twine)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L3_S2_CoconutComboTwineFixed)
				{
					if(InvDestCoconutShell.Intersection(&dropPos))
					{
						ProfileSystem::Instance->ActiveProfile.L3_S2_CoconutComboTwineFixed = true;

						Control::Audio->QuickLoadAndPlaySFX("combo_bombthreadattach");

						JAnim_TwinePlacing->PlayAnim();
						isAnyAnimActive = true;

						GFHud->ReleaseDrag();
						GFHud->CloseInventory();

						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_Twine] = false;
						GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_Twine);

						isItemUsed = true;
					}
				}
			}
		}
		else if(_invId == eInv_L3S2_HammerHead)
		{
			if( itemVal == eInv_L3S2_HammerHandle)
			{
				if(InvDestRectHammerHead.Intersection(&dropPos))
				{
					//GFHud->ShowBannerDirectText("DEBUG: Hammer Head and Handle animation to start ...");
					Control::Audio->QuickLoadAndPlaySFX("combo_hammerfix");

					JAnim_HammerFix->PlayAnim();
					isAnyAnimActive = true;				
					GFHud->ReleaseDrag();
					GFHud->CloseInventory();

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_HammerHandle] = false;
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_HammerHandle);
					//GFHud->ShowBannerDirectText("DEBUG: Hammer Handle removed from Inventory ...");
					isItemUsed = true;
				}
			}
		}
		else if(_invId == eInv_L3S3_Box)
		{
		}
		else if (_invId == eInv_L4_S1_Leaf)
		{
			if (itemVal == eInv_L4_S1_Stick)
			{
				if(InvDestRectStick.Intersection(&dropPos))
				{
					ComboScene->GetObjectByID("Leaf_FirstFold_Leaf")->SetEnable(true);
					ComboScene->GetObjectByID("Leaf")->SetEnable(false);
					JAnim_LeafSpoon->PlayAnim();
					Control::Audio->QuickLoadAndPlaySFX("s1p1_thornRemove2");
					isAnyAnimActive = true;

					GFHud->ReleaseDrag();
					GFHud->CloseInventory();

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Stick] = false;
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_Stick);

					isItemUsed = true;
				}
			}
		}
		else if (_invId == eInv_L4_CB_HandKerchief)
		{
			if (itemVal == eInv_L4_S1_Knife)
			{
				if(InvDestRectKnife.Intersection(&dropPos))
				{
					ComboScene->GetObjectByID("Knife")->SetEnable(true);

					CAnim_combo_knife->PlayAnim();
					isKnifeAnimPlaying = true;
					CAnim_combo_kerchief->PlayAnim();
					isKerchiefAnimPlaying = true;
					Control::Audio->QuickLoadAndPlaySFX("combo_kercheifTear");
					isAnyAnimActive = true;

					GFHud->ReleaseDrag();
					GFHud->CloseInventory();

					isItemUsed = true;

				}
			}
		}
		else if (_invId == eInv_L4_S2_Bamboo)
		{
			if (itemVal == eInv_L4_S1_Knife)
			{
				if(InvDestRectBambooKnife.Intersection(&dropPos))
				{
					Control::Audio->QuickLoadAndPlaySFX("combo_lensOnbinocular");
					GFHud->ReleaseDrag();
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife] = false;
					ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo = true;
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_Knife);
					GFHud->InvBoxCtrl->Close();
					ComboScene->GetObjectByID("Knife")->SetEnable(true);
					GFHud->InvBoxCtrl->updateComboItemStatus(eInv_L4_S2_Bamboo);
					isItemUsed = true;
				}
			}

			if (itemVal == eInv_L4_S1_KerchiefPiece2)
			{
				if(InvDestRectBambooKnife.Intersection(&dropPos))
				{
					if (ProfileSystem::Instance->ActiveProfile.L4_S2_ComboKnifeAddedToBamboo)
					{
						Control::Audio->QuickLoadAndPlaySFX("combo_clothtied");

						ComboScene->GetObjectByID("KerchiefPiece")->SetEnable(true);
						GFHud->ReleaseDrag();
						JSONAnim_Combo_BambooKnife->PlayAnim();
						isItemUsed = true;
						isAnyAnimActive = true;
					}
					else
					{
						Control::Input->ClearMouse();//Avoid unwanted click message handlers!!
						GFHud->ShowBannerText("L4tying");
						GFHud->CancelDrag();//just cancel so that user can choose another one if invbox is open!!
						isItemUsed = true;
					}

				}
			}
		}
		else if (_invId == eInv_L4_S4_BinocularWithoutLens)
		{
			if (itemVal == eInv_L4_S1_BinocularLens)
			{
				if(InvDestRectBinocularLens.Intersection(&dropPos))
				{
					ComboScene->GetObjectByID("Lens")->SetEnable(true);

					GFHud->ReleaseDrag();
					GFHud->CloseInventory();

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_BinocularLens] = false;
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_BinocularLens);

					isItemUsed = true;

					ComboScene->SetSceneAlpha(0);

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S4_BinocularWithoutLens] = false;
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S4_BinocularWithoutLens);


					ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularWithLensTaken = true;
					TaskPendingUpdate();
					GFHud->TakeInventoryItem(eInv_L4_CB_Binocular);
				}
			}
		}
		else if (_invId == eInv_L4_S5_Net)
		{
			if (itemVal == eInv_L4_S2_BambooStick1)//TODO:sujin
			{
				if(InvDestRectBambooStrainer.Intersection(&dropPos))
				{
					GFHud->ReleaseDrag();
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_BambooStick1] = false;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_BambooStick2] = false;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_BambooStick3] = false;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_BambooStick4] = false;
					ProfileSystem::Instance->ActiveProfile.L4_S5_ComboBambooFrameAddedToNet = true;
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S2_BambooStick1);
					GFHud->InvBoxCtrl->Close();
					ComboScene->GetObjectByID("Bamboo")->SetEnable(true);
					GFHud->InvBoxCtrl->updateComboItemStatus(eInv_L4_S5_Net);
					isItemUsed = true;
					Control::Audio->QuickLoadAndPlaySFX("combo_stickOnNet");
				}
			}

			if (itemVal == eInv_L4_S2_GrassRope)
			{
				if(InvDestRectBambooStrainer.Intersection(&dropPos))
				{
					if (ProfileSystem::Instance->ActiveProfile.L4_S5_ComboBambooFrameAddedToNet)
					{
						ComboScene->GetObjectByID("RopeFull")->SetEnable(true);
						GFHud->ReleaseDrag();
						JSONAnim_Combo_BambooStrainer->PlayAnim();
						isItemUsed = true;
						isAnyAnimActive = true;
						Control::Audio->QuickLoadAndPlaySFX("combo_improvisedSpearRope");
					}
					else
					{
						Control::Input->ClearMouse();//Avoid unwanted click message handlers!!
						GFHud->ShowBannerText("L4its");
						GFHud->CancelDrag();//just cancel so that user can choose another one if invbox is open!!
						isItemUsed = true;
					}

				}
			}
		}
		else if (_invId == eInv_L5_S1_Bow)
		{
			if (itemVal == eInv_L5_S1_ArrowHolder)
			{
				if(InvDestRectBow.Intersection(&dropPos))
				{
					ProfileSystem::Instance->ActiveProfile.L5_S1_BowArrowHolderPlaced = true;

					ComboScene->GetObjectByID("arrowholdermask")->SetEnable(true);
					ComboScene->GetObjectByID("arrow")->SetEnable(true);
					ComboScene->GetObjectByID("arrowholder")->SetEnable(true);

					GFHud->ReleaseDrag();
					GFHud->CloseInventory();

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S1_ArrowHolder] = false;
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S1_ArrowHolder);

					isItemUsed = true;
				}
			}
		}
		else if (_invId == eInv_L5_S4_PickaxeBlade)
		{
			if (itemVal == eInv_L5_S5_PickaxeHandle)
			{
				if(InvDestRectPickaxe.Intersection(&dropPos))
				{
					Control::Audio->QuickLoadAndPlaySFX("combo_axefix");

					isAnyAnimActive = true;
					JAnim_PickaxeHandleFix->PlayAnim();

					GFHud->ReleaseDrag();
					GFHud->CloseInventory();

					isItemUsed = true;
				}
			}
		}
		else if (_invId == eInv_L6_S4_HeartContainer)
		{
			if (itemVal == eInv_L6_S3_GlowingHeart)
			{
				if(InvDestRectHeartLocket.Intersection(&dropPos))
				{
					GFHud->ReleaseDrag();
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_GlowingHeart] = false;
					// Sac eInv_L6_S3_GlowingHeart is been removed here once.. removeing it in another place will crash!
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S3_GlowingHeart);
					GFHud->InvBoxCtrl->Close();
					JSONAnim_Combo_HeartLocket->PlayAnim();
					isItemUsed = true;
					isAnyAnimActive = true;
				}
			}
		}
		else if (_invId == eInv_L6_S4_Rope)
		{
			if (itemVal == eInv_L6_S4_MagnetisedHorseShoe)
			{
				if(InvDestRectRopeHorseShoe.Intersection(&dropPos))
				{
					GFHud->ReleaseDrag();
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_MagnetisedHorseShoe] = false;
					// Sac Magenitisedhorseshoe is been removed here once.. removeing it in another place will crash!
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_MagnetisedHorseShoe);
					GFHud->InvBoxCtrl->Close();
					JSONAnim_Combo_RopeHorseShoe->PlayAnim();
					isItemUsed = true;
					isAnyAnimActive = true;
				}
			}
		}
		else if (_invId == eInv_L7_S1_PotEmpty)
		{
			if (itemVal == eInv_L7_S3_MetalChain)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L7_S1_ComboPotChainPlaced &&  InvDestRectL7PotMetalWire.Intersection(&dropPos))
				{
					ProfileSystem::Instance->ActiveProfile.L7_S1_ComboPotChainPlaced = true;

					Control::Audio->QuickLoadAndPlaySFX("combo_potchainplaced");

					GFHud->ReleaseDrag();

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S3_MetalChain] = false;
					GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S3_MetalChain);

					GFHud->InvBoxCtrl->Close();

					ComboScene->GetObjectByID("chain")->SetEnable(true);
					ComboScene->GetObjectByID("chain")->FadeIn(2.0f);

					isItemUsed = true;					
				}
			}
			else if (itemVal == eInv_L7_S3_MetalWire)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L7_S1_ComboPotChainPlaced)
				{
					GFHud->ReleaseDrag();
					GFHud->InvBoxCtrl->Close();

					GFHud->ShowBannerText("L7kind");
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L7_S1_InvPotWithChainTaken &&  InvDestRectL7PotMetalWire.Intersection(&dropPos))
				{
					GFHud->ReleaseDrag();

					GFHud->InvBoxCtrl->Close();

					Control::Audio->QuickLoadAndPlaySFX("combo_potchainconnected");

					JAnim_PotWireFix->PlayAnim();

					isAnyAnimActive = true;
					isItemUsed = true;					
				}
			}
		}
		else if (_invId == eInv_L7_S2_SphereHalfBase)
		{
			if (itemVal == eInv_L7_S2_SphereHalfToFix)
			{
				if(InvDestRectL7Globe.Intersection(&dropPos))
				{
					GFHud->ReleaseDrag();

					GFHud->InvBoxCtrl->Close();

					Control::Audio->QuickLoadAndPlaySFX("combo_spherefixed");

					JAnim_GlobeFix->PlayAnim();

					isItemUsed = true;
					isAnyAnimActive = true;
				}
			}
		}
		else if (_invId == eInv_L8_S1_MagicWand)
		{
			if (itemVal == eInv_L8_S2_Tool)
			{
				if (InvDestRectL8MagicWandTool.Intersection(&dropPos))
				{
					GFHud->ReleaseDrag();

					GFHud->InvBoxCtrl->Close();

					ComboScene->GetObjectByID("Shukra-key_Large")->SetEnable(true);
					ComboScene->GetObjectByID("Shukra-key01-")->SetEnable(true);
					ComboScene->GetObjectByID("Shukra-key02")->SetEnable(true);

					Janim_MagicWandTool->PlayAnim();
					Control::Audio->QuickLoadAndPlaySFX("combo_magicWand");

					isItemUsed = true;
					isAnyAnimActive = true;
				}
			}
		}
		else if (_invId == eInv_L8_S2_HalfKey)
		{
			if (itemVal == eInv_L8_S1_HalfKey)
			{
				if(InvDestRectBambooStrainer.Intersection(&dropPos))
				{
					GFHud->ReleaseDrag();
					Janim_KeyFix->PlayAnim();
					isItemUsed = true;
					isAnyAnimActive = true;
				}
			}

		}

		if(!isItemUsed)
			GFInstance->WrongInvDropMsg(GFHud);
	}
}

void  ComboInvSystem::InitRenderTarget(CHOScene *Popup)
{
	if( GFInstance->IsRTCreated )
	{		
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS	
		GFInstance->IsRenderingToRT = true;
    }
		Popup->RenderToTarget(-171.0f);
    if( GFInstance->IsRTCreated )
    {
		GFInstance->IsRenderingToRT = false;
		GFInstance->RTGraphic->endRenderToTarget();
	}
}

void ComboInvSystem::HideComboInventoryPopup(bool instantClose)
{	
	if(_invId == -1)
		return;

#ifdef _CEBUILD
	if(GFHud)
		GFHud->ShowMorphObjects();
#endif //CE_BUILD
	//if(!instantClose)
	{
		PopupAlpha = 1.0f;
		ComboState = JRN_POPDOWN;
		InitRenderTarget(ComboScene);
		if(!instantClose)
			GFInstance->SetPopupTween( popOffsetX, popOffsetY, 171, 0);
		else
			GFInstance->SetPopupTween( 0, 0, 171, 0);
		IsPopTransitionActive = true;
	}
	/*else
	{
	ComboState = JRN_IDLE;

	GFHud->IsComboInvPopupShowing = false;

	ComboMaskBg->Show(false);
	SAFE_DELETE(ComboMaskBg);
	Visible = false;
	}*/

	PopCloseBtn = NULL;

	Control::Audio->UnloadQuickLoadedSFX();

	//Dispose Loaded Stuffs
	SAFE_DELETE(ComboScene);
	SAFE_DELETE(ComboCloseBtn);

	SAFE_DELETE(ComboBoundaryRect);


	SAFE_DELETE(ComboPopMaskBg);

	if(_invId == eInv_L1S1_Bag)
	{
		for (int i = 0; i < 4; i++)
		{
			SAFE_DELETE(ruby_FixPos[i]);
			SAFE_DELETE(ruby_takeRects[i]);
			SAFE_DELETE(ruby_destRects[i]);
		}
		SAFE_DELETE(BagOpenPoly);
		SAFE_DELETE(CAnim_ZipperOpen);
		SAFE_DELETE(BagZipperPoly);
		SAFE_DELETE(CAnim_kidDrawingSlide1);
		SAFE_DELETE(CAnim_kidDrawingSlide2);
		SAFE_DELETE(bag_CreekLetterZoomed);
		SAFE_DELETE(bag_CreekLetterZoomedText);
	}
	else if(_invId == eInv_L1S4_BeetleHead)
	{
		SAFE_DELETE(CAnim_BeetleJoin);
	}
	else if(_invId == eInv_L1S5_HammerHead)
	{
		SAFE_DELETE(CAnim_HammerJoin);
	}
	else if(_invId == eInv_L2S4_AxeBlunt)
	{
		SAFE_DELETE(JAnim_AxeSharpen);
		SAFE_DELETE(axeSparklesFX);
	}
	else if(_invId == eInv_L2S6_KnifeBlunt)
	{
		SAFE_DELETE(JAnim_CorkRemove);
		SAFE_DELETE(JAnim_RustRemove);
		SAFE_DELETE(vinegarDropsFX);
	}
	else if(_invId == eInv_L3S1_FlagFull)
	{
		SAFE_DELETE(JAnim_FlagComboRemove);
	}
	else if(_invId == eInv_L3S1_LadderBroken)
	{
		SAFE_DELETE(JAnim_PiecesPlacing);
	}
	else if(_invId == eInv_L3S2_CoconutShell)
	{
		SAFE_DELETE(JAnim_CoconutOuterRemove);
		SAFE_DELETE(JAnim_CoconutBreak);
	}
	else if(_invId == eInv_L3S2_CocoShellPowderFilled)
	{
		SAFE_DELETE(JAnim_TwinePlacing);
	}
	else if (_invId == eInv_L3S2_HammerHead)
	{
		SAFE_DELETE(JAnim_HammerFix);
	}
	else if(_invId == eInv_L3S3_Box)
	{

	}
	else if (_invId == eInv_L4_S1_Leaf)
	{
		SAFE_DELETE(JAnim_LeafSpoon);
	}
	else if (_invId == eInv_L4_CB_HandKerchief)
	{
		SAFE_DELETE(CAnim_combo_knife);
		SAFE_DELETE(CAnim_combo_kerchief);
	}
	else if (_invId == eInv_L4_S2_Bamboo)
	{
		SAFE_DELETE(JSONAnim_Combo_BambooKnife);
	}
	else if (_invId == eInv_L4_S5_Net)
	{
		SAFE_DELETE(JSONAnim_Combo_BambooStrainer);
	}
	else if (_invId == eInv_L5_S1_Bow)
	{
		SAFE_DELETE(JAnim_BowArrowFix);
		SAFE_DELETE(JAnim_BowStringFix);
	}
	else if (_invId == eInv_L5_S4_PickaxeBlade)
	{
		SAFE_DELETE(JAnim_PickaxeHandleFix);
	}
	else if(_invId == eInv_L5_S4_PeacockChain)
	{
		SAFE_DELETE(JAnim_Locket);
	}
	else if (_invId == eInv_L6_S4_HeartContainer)
	{
		SAFE_DELETE(JSONAnim_Combo_HeartLocket);
	}
	else if (_invId == eInv_L6_S4_Rope)
	{
		SAFE_DELETE(JSONAnim_Combo_RopeHorseShoe);
	}
	else if (_invId == eInv_L7_S1_PotEmpty)
	{
		SAFE_DELETE(JAnim_PotChainTry);
		SAFE_DELETE(JAnim_PotWireFix);
	}
	else if (_invId == eInv_L7_S2_SphereHalfBase)
	{
		SAFE_DELETE(JAnim_GlobeFix);
	}
	else if (_invId == eInv_L8_S1_MagicWand)
	{
		SAFE_DELETE(Janim_MagicWand);
		SAFE_DELETE(Janim_MagicWandTool);
	}
	else if (_invId == eInv_L8_S2_HalfKey)
	{
		SAFE_DELETE(Janim_KeyFix);
	}

	_invId = -1;
}
