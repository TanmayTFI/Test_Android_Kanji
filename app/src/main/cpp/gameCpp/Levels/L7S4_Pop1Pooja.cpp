
//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "L7S4_Pop1Pooja.h"
#include "Hud.h"
#include "Util.h"

const CRectangle  PotArea(622.0f,88.0f,145.0f,55.0f);
const CRectangle FlowersRect[] = {CRectangle(631.0f,209.0f,108.0f,94.0f), CRectangle(711.0f,340.0f,107.0f,94.0f),CRectangle(561.0f,326.0f,98.0f,110.0f)};
const CRectangle  	PopAreaRect(345.0f,70.0f,665.0f,521.0f);
const CRectangle  InvItemRect(647.0f,162.0f,255.0f,234.0f);
const CRectangle  InvDestRect(464.0f,130.0f,481.0f,417.0f);

const float VDOStartDelay = 1.5f;

Flower::Flower(int index)
{
	_placed = false;
	_Obj = NULL;
	_isCompleted = false;
	_currentArea = NULL;
	_index = index;
}

Flower::~Flower()
{

}

FlowerAreas* Flower::getCurrentArea()
{
	return _currentArea;
}

void Flower::PlacedAt(FlowerAreas *area)
{
	if (_currentArea)
	{
		float arr[5] = {-1, area->_pos.x, area->_pos.y, -1, -1};
		_Obj->TweenTo(arr, 0.3f, 0, true);
	}
	else
	{
		_Obj->SetPos(&area->_pos);
		_placed = true;	
	}

	_currentArea = area;
	_Obj->SetEnable(true);	
	_currentArea->_haveFlower = true;

	if (_currentArea->_pos == _finalArea->_pos)
	{
		_isCompleted = true;
	}
}

bool Flower::isInArea(FlowerAreas *area)
{
	if (_currentArea && _currentArea->_pos == area->_pos)
	{
		return true;
	}

	return false;
}




L7S4_Pop1Pooja::L7S4_Pop1Pooja(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) 
	:Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV7\\S4\\POP1\\POP1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	
	_pzMode = L7S4_Puzzle_None;
	_fixedFlowerCount = 0;
	_placedAllFlowers = false;
	_isDrawing = false;
	_isTakeBusmomToHand = false;
	_hashDrawComplete = false;
	_flowerPuzzleComplete = false;
	_isVdoStartPlaying = false;
	_selectedFlower = NULL;
	_vdoStartTimer = 0.0f;
	InitPolys();
	_isInPuzzleMod = false;
	noteAnimating = false;

	GetObjectByID("Line_glow")->SetEnable(false);
	GetObjectByID("BasmamPot_glow")->SetEnable(false);
	s4tableonfireFX = new SuperFX("PRT\\L7\\s4tableonfire.INI", PointSystem::CreateCPoint(683,331,0), 1144); 

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	_cvdo_handPowerGain = new CVideo("OGV\\L7\\S4HANDPOWERGAIN.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//_cvdo_handPowerGain->SetScale(1.334f);
	_cvdo_handPowerGain->SetScale(GFApp->video_aspectRatio);
	vidTimer = 0;
	GetObjectByID("Script_Large")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteClicked)
		GetObjectByID("Script_Level")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken)
		GetObjectByID("wheel")->SetEnable(false);

	s4BhasmaPowderFX = new SuperFX("PRT\\L7\\s4BhasmaPowder.INI", PointSystem::CreateCPoint(557,250,0), 1100);
	s4BhasmaPartiFX = new SuperFX("PRT\\L7\\s4BhasmaParti.INI", PointSystem::CreateCPoint(565,241,0), 1100); 

	JAnim_NoteOpen = new JSONAnimator(0, CC_CALLBACK_1(L7S4_Pop1Pooja::OnAnimationComplete, this));
	JAnim_NoteOpen->parseAndLoadJSON("ANIM\\L7\\S4P1FLYERZOOMIN.JSON", this);

	flowerChart = new CObject();
	flowerChart->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L7\\FLOWERCHART.SPR", 683, 375, eZOrder_Popup + 500);
	flowerChart->SetUID("flowerChart");
	flowerChart->Show(false);
	
	JAnim_NoteClose = new JSONAnimator(1, CC_CALLBACK_1(L7S4_Pop1Pooja::OnAnimationComplete, this));
	JAnim_NoteClose->parseAndLoadJSON("ANIM\\L7\\S4P1FLYERZOOMOUT.JSON", this);
	

	GetObjectByID("Line_1")->SetEnable(false);
	_hashBox_0 = GetObjectByID("BasmamPot_0");
	_hashBox_0->SetZ(_hashBox_0->ZOrder + 100);

	_hashBoxUsed = ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamUsed;
	_hashBox_0->SetEnable(_hashBoxUsed);

	_invTakenFlowers.clear();
	_invTakenFlowers.push_back(ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedFlowerTaken);
	_invTakenFlowers.push_back(ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueFlowerTaken);
	_invTakenFlowers.push_back(ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken);

	for (int i = 0; i < (int)Objects.size(); i++)
	{
		if (Objects[i]->UID.find("Part_") != string::npos)
		{
			Part part;
			part.Init(Objects[i]);
			_parts.push_back(part);
			Objects[i]->SetEnable(false);
		}

		if (Objects[i]->UID.find("Flower_") != string::npos)
		{		
			FlowerAreas *area = new FlowerAreas();
			area->_pos = Objects[i]->Pos;
			area->_haveFlower = false;
			area->_index = (int)_flowerPos.size();
			_flowerPos.push_back(area);

			Flower *flower = new Flower(int(_flowers.size()));
			flower->setObj(Objects[i]);
			flower->setFinalArea(area);
			_flowers.push_back(flower);
			Objects[i]->SetEnable(false);
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteTaken && ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteClicked)
	{
		GetObjectByID("Script_Large")->SetEnable(true);
		GetObjectByID("Script_Level")->SetEnable(false);
	}


	Show(false);

	if (ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamComplete)
	{
		GetObjectByID("Line_1")->SetEnable(true);
		_hashDrawComplete = true;
		_hashBox_0->SetEnable(false); 
	}

	for (int i = 0; i < (int)(_flowerPos.size()); i++)
	{
		if (ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopFlowerPos[i] != -1)
		{
			int p = ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopFlowerPos[i];
			_flowers[i]->PlacedAt(_flowerPos[p]);
			_fixedFlowerCount++;
		}
	}

	if (_fixedFlowerCount == (int)_flowerPos.size())
	{
		_placedAllFlowers = true;
	}

	CheckForFlowerCompleted();
}

void L7S4_Pop1Pooja::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		
		isAnimationPlaying = false;
		GetObjectByID("Script_Large")->SetEnable(true);
		GetObjectByID("Script_Level")->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteClicked  = true;
		Cursor::Show(true);

	}
	else if(animID == 1)
	{
		isAnimationPlaying = false;
		GetObjectByID("Script_Large")->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteTaken  = true;
		Cursor::Show(true);

		//Journal Entry Add
		if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P4_ThreeFlowerNotice])
		{
			//Flag Set
			ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P4_ThreeFlowerNotice] = true;

			//Add to Note Array
			ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P4_ThreeFlowerNotice;

			ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

			//Increment Notes
			ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			Hud->ShowJrnEntryFX();
		}
	}
	
}

void L7S4_Pop1Pooja::Required()
{

}

void L7S4_Pop1Pooja::Update(float ds)
{
	Popup::Update(ds);
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup ||  Hud->IsDraggingItem())
		return;

	if (GetPopupState() != ePopupState_Open || GFInstance->IsActivePopup)
	{
		flowerChart->Show(false);
		return;
	}
	if (_isVdoStartPlaying)
	{
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			_cvdo_handPowerGain->isEndReached() ||
#endif
			vidTimer > 6)
		{
			isAnimationPlaying = false;
			_isVdoStartPlaying = false;
			Hud->EnableInput(false);
			Hud->ShowHud(false);
			_cvdo_handPowerGain->StopAndFadeOutVideo(2);
			ClosePopup();
		}
		return;
	}

	if (!ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteTaken && GetObjectByID("Script_Large")->IsActive)
	{
		flowerChart->Show(true);
		flowerChart->SetPos(GetObjectByID("Script_Large")->GetPosPointer());
		flowerChart->SetRotate(GetObjectByID("Script_Large")->GetRotate());
		flowerChart->SetScale(GetObjectByID("Script_Large")->GetScale());
		flowerChart->SetAlpha(GetObjectByID("Script_Large")->GetAlpha());
	}
	else
	{
		flowerChart->Show(false);
	}

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	if (ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopPoojaPuzzleMode != (int)_pzMode)
	{
		ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopPoojaPuzzleMode = (int)_pzMode;	

		if(!_isInPuzzleMod && (_pzMode == L7S4_Puzzle_BusmomPlaced || _pzMode == L7S4_Puzzle_Flower))
		{
			Hud->InvBox_StartPuzzleMode();
			_isInPuzzleMod = true;
			#ifdef TOUCH_UI
			GFApp->pinchDisabled = true;
			#endif
		}
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);	

	switch (_pzMode)
	{
	case L7S4_Puzzle_None:
		{
			if(ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteTaken && !ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken && InvItemRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					GetObjectByID("wheel")->SetEnable(false);
					ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken = true;
					Hud->TakeInventoryItem(eInv_L7_S4_WellGear, GetObjectByID("wheel")->Pos);

				}
				return;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteTaken && ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteClicked && PopAreaRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("s4p1_notezoomout");
					JAnim_NoteClose->PlayAnim();
					isAnimationPlaying = true;

				}
				return;
			}
			else if( !ProfileSystem::Instance->ActiveProfile.L7_S4_P1NoteClicked && InvItemRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("s4p1_notezoomin");
					noteAnimating = true;
					JAnim_NoteOpen->PlayAnim();
					isAnimationPlaying = true;
				}
				return;
			}

			else if(ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken && _hashDrawComplete && !_placedAllFlowers  && InvDestRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{

					Hud->ShowBannerText("L7still");

				}
				return;
			}
			else if(ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken && !ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamUsed  && _flowerPuzzleComplete  && InvDestRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L7design");
				}
				return;
			}
			else if(ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken  && !ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamUsed && InvDestRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L7mark");
				}
				return;
			}

			if (_hashDrawComplete && _flowerPuzzleComplete)
			{
				_vdoStartTimer = VDOStartDelay;
				_pzMode = L7S4_Puzzle_Completed;
			}
		}
		break;

	case L7S4_Puzzle_Flower:
		{
			for (int j = 0; j < (int)(_flowerPos.size()); j++)
			{
				if(FlowersRect[j].Intersection(&pos))
				{
					if (_selectedFlower)
					{
						Cursor::SetMode(CA_HandPointy);
						if (Control::Input->LBclicked())
						{
							for (int i = 0; i < (int)_flowers.size(); i++)
							{
								if (_flowers[i]->isInArea(_flowerPos[j]))
								{
									_flowers[i]->PlacedAt(_selectedFlower->getCurrentArea());
									ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopFlowerPos[i] = _selectedFlower->getCurrentArea()->_index;
									_selectedFlower->PlacedAt(_flowerPos[j]);	
									ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopFlowerPos[_selectedFlower->_index] = j;
									_selectedFlower = NULL;
									CheckForFlowerCompleted();
									break;
								}
							}

						}
					}
					else
					{
						if (_flowerPos[j]->_haveFlower)
						{
							Cursor::SetMode(CA_HandPointy);
							if (Control::Input->LBclicked())
							{
								for (int i = 0; i < (int)_flowers.size(); i++)
								{
									if (_flowers[i]->isInArea(_flowerPos[j]))
									{
										_selectedFlower = _flowers[i];
									}
								}
							}
						}
					}
				}
			}

			if (_flowerPuzzleComplete)
			{
				_pzMode = L7S4_Puzzle_None;
				Hud->InvBox_StopPuzzleMode();
				_isInPuzzleMod = false;
#ifdef TOUCH_UI
				GFApp->pinchDisabled = false;
#endif
			}
		}
		break;

	case L7S4_Puzzle_BusmomPlaced:
		{
			if (_isTakeBusmomToHand && !_isDrawing)
			{
				if (_popupBoundingBox.Intersection(&pos))
				{
					Cursor::SetMode(CA_Normal);
					s4BhasmaPowderFX->Active = true;
					s4BhasmaPartiFX->Active = true;
					s4tableonfireFX->Active = true;
					s4tableonfireFX->Visible = true; 
					s4BhasmaPowderFX->Pos = pos;
					s4BhasmaPartiFX->Pos = pos;

					if (Control::Input->LBclicked() || Control::Input->LBdown())
					{
						_isDrawing = true;
						GetObjectByID("Line_glow")->SetEnable(false);
						_pzMode = L7S4_Puzzle_Busmom;
					}

					for (int i = 0; i < (int)_boundries.size(); i++)
					{
						if (_boundries[i]->Intersection(&pos))
						{
							Cursor::SetMode(CA_Normal);
							return;
						}
					}

					Cursor::SetMode(CA_HandPointy);
				}
				else
				{
					s4BhasmaPowderFX->Active = false;
					s4BhasmaPartiFX->Active = false;
				}
			}

			if (!_hashDrawComplete)
			{
				if (_hashBoxUsed && !_isTakeBusmomToHand && PotArea.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandTake);
					_hashBox_0->PlayAnimation(1);

					if (!Hud->IsDraggingItem() && Control::Input->LBclicked())
					{
						Control::Audio->QuickLoadAndPlaySFX("s4p1_bhasmamclicked");
						_isTakeBusmomToHand = true;
						GetObjectByID("Line_glow")->SetEnable(true);
						GetObjectByID("Line_glow")->Animate(0.4f, 0.8f, 1.4f, 0, eBlend_AlphaAdditive);
						s4BhasmaPowderFX->Start();
						s4BhasmaPartiFX->Start();
						s4tableonfireFX->Start();
					}
				}
				else
				{
					_hashBox_0->PlayAnimation(0);
				}
			}
			break;

	case L7S4_Puzzle_Busmom:
		{
				if (_isDrawing && _popupBoundingBox.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					s4BhasmaPowderFX->Pos = pos;
					s4BhasmaPartiFX->Pos = pos;

					for (int i = 0; i < (int)_boundries.size(); i++)
					{
#ifndef TOUCH_UI
						if (_boundries[i]->Intersection(&pos))
						{
							ResetLineDraw();
							_isDrawing = false;
							GetObjectByID("Line_glow")->SetEnable(true);
							_pzMode = L7S4_Puzzle_BusmomPlaced;
							return;
						}
						else
#endif
						{
							DrawAt(pos);
						}
					}

					for (int i = 0; i < (int)_parts.size(); i++)
					{
						if (!_parts[i]._isDraw)
							return;
					}

					_hashDrawComplete = true;
					Hud->InvBox_StopPuzzleMode();
					_isInPuzzleMod = false; 
#ifdef TOUCH_UI
						GFApp->pinchDisabled = false;
#endif
					_hashBox_0->FadeOut();
					s4BhasmaPowderFX->StopEmit();
					s4BhasmaPartiFX->StopEmit();
					s4tableonfireFX->StopEmit();
					GetObjectByID("BasmamPot_glow")->SetEnable(false);
					ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamComplete = true;
					Control::Audio->QuickLoadAndPlaySFX("s4p1_bhasmamcomplete");
					_pzMode = L7S4_Puzzle_None;
				}
			}
		}
		break;

	case L7S4_Puzzle_Completed:
		{
			_vdoStartTimer -= ds;
			if (_vdoStartTimer <= 0.0f)
			{
				ProfileSystem::Instance->ActiveProfile.L7_S4_HaveHandPower = true;
				ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopPoojaPopComplete = true;
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S3_Haven] = true;
				_cvdo_handPowerGain->PlayFadeInVideo(2);
				Hud->AllowInput = true;
				Hud->ShowHud(false);
				_isVdoStartPlaying = true;
				isAnimationPlaying = true;
				return;
			}
		}
		break;

	default:
		break;
	}
	
}

void L7S4_Pop1Pooja::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken)
	{
		GFInstance->WrongInvDropMsg(Hud);
		return;
	}

	int i = -1;
	if (itemVal == eInv_L7_S1_RedFlower)
	{
		i = 0;
	}
	else if(itemVal == eInv_L7_S4_BlueFlower)
	{
		i = 1;
	}
	else if(itemVal == eInv_L7_S3_YellowFlowe)
	{
		i = 2;
	}
	else if(itemVal == eInv_L7_S3_Bhasmam  && _popupBoundingBox.Intersection(&dropPos))
	{
		Control::Audio->QuickLoadAndPlaySFX("s4p1_bhasmamplaced");
		ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamUsed = true;
		_hashBoxUsed = true;
		_hashBox_0->SetEnable(true);

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.Inventory[itemVal] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(itemVal);

		_pzMode = L7S4_Puzzle_BusmomPlaced;
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
		return;
	}
	
	if (i != -1)
	{
		for (int j = 0; j < (int)(_flowerPos.size()); j++)
		{
			if(FlowersRect[j].Intersection(&dropPos))
			{
				if(_flowerPos[j]->_haveFlower)
				{
					GFInstance->WrongInvDropMsg(Hud);
					break;
				}

				_flowers[i]->PlacedAt(_flowerPos[j]);

				ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopFlowerPos[i] = j;

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[itemVal] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(itemVal);

				Control::Audio->QuickLoadAndPlaySFX("s4p1_flowerplaced");
				_fixedFlowerCount++;
			}
		}

		CheckForFlowerCompleted();
		
		if (_fixedFlowerCount == (int)_flowerPos.size())
		{
			_placedAllFlowers = true;
		}
	}

	if (_placedAllFlowers && !_flowerPuzzleComplete)
	{
		_pzMode = L7S4_Puzzle_Flower;
	}
}

void L7S4_Pop1Pooja::CheckForFlowerCompleted()
{
	for (int k = 0; k < (int)_flowers.size(); k++)
	{
		_flowerPuzzleComplete = true;

		if (!_flowers[k]->getIsCompleted())
		{
			_flowerPuzzleComplete = false;
			break;
		}
	}
}


void L7S4_Pop1Pooja::ProcessSkip()
{
	switch (_pzMode)
	{
	case L7S4_Puzzle_None:
		break;
	case L7S4_Puzzle_Flower:
		{
			for (int i = 0; i < (int)_flowers.size(); i++)
			{
				_flowers[i]->PlacedAt(_flowers[i]->getFinalArea());
			}

			_flowerPuzzleComplete = true;
		}
		break;

	case L7S4_Puzzle_BusmomPlaced:
	case L7S4_Puzzle_Busmom:
		{
			_hashDrawComplete = true;	
			ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamComplete = true;

			_hashBox_0->SetEnable(false);

			for (int i = 0; i < (int)_parts.size(); i++)
			{
				_parts[i].Reset();
			}

			GetObjectByID("Line_1")->SetEnable(true);
			GetObjectByID("Line_glow")->SetEnable(false);
			s4tableonfireFX->StopEmit();
		}
		break;

	default:
		break;
	}

	Hud->InvBox_StopPuzzleMode();
	_isInPuzzleMod = false;
#ifdef TOUCH_UI
	GFApp->pinchDisabled = false;
#endif
	_pzMode = L7S4_Puzzle_None;
}

void L7S4_Pop1Pooja::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRect);
		return;
	}


	EHOInventory invItemVal[] = {eInv_L7_S1_RedFlower, eInv_L7_S4_BlueFlower, eInv_L7_S3_YellowFlowe};

	for (int i = 0; i < (int)_invTakenFlowers.size(); i++)
	{
		if (_invTakenFlowers[i] && !_flowers[i]->getPlaced())
		{
			for (int j = 0; j < (int)_flowerPos.size(); j++)
			{
				if (!_flowerPos[j]->_haveFlower)
				{
					Hud->ShowHintForInvItem(invItemVal[i], &_flowerPos[j]->_pos);
					return;
				}
			}
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamUsed)
		{
			Hud->ShowHintForInvItem(eInv_L7_S3_Bhasmam, &PotArea);
			return;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamComplete)
		{
			Hud->InitHintTrailEmit(&PotArea);
			return;
		}
	}



	// hint code goes here
	//else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L7S4_Pop1Pooja::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L7_S4_InvWellWheelTaken)
	{
		isAnyTaskLeft = true;
	}

	_invTakenFlowers.clear();
	_invTakenFlowers.push_back(ProfileSystem::Instance->ActiveProfile.L7_S1_InvRedFlowerTaken);
	_invTakenFlowers.push_back(ProfileSystem::Instance->ActiveProfile.L7_S4_InvBlueFlowerTaken);
	_invTakenFlowers.push_back(ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken);

	// EHOInventory invItemVal[] = {eInv_L7_S1_RedFlower, eInv_L7_S4_BlueFlower, eInv_L7_S3_YellowFlowe};  // Sac: Commenting Unused variable

	for (int i = 0; i < (int)_invTakenFlowers.size(); i++)
	{
		if (_invTakenFlowers[i] && !_flowers[i]->getPlaced())
		{
			isAnyTaskLeft = true;
		}
	}

	if (_placedAllFlowers && !_flowerPuzzleComplete)
	{
		isAnyTaskLeft = true;
	}

	if (ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken && !ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamComplete)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L7S4_Pop1Pooja::~L7S4_Pop1Pooja()
{
	Hud->InvBox_StopPuzzleMode();
	_isInPuzzleMod = false;
#ifdef TOUCH_UI
	GFApp->pinchDisabled = false;
#endif
	for (int i = 0; i < (int)(_flowers.size()); i++)
	{
		SAFE_DELETE(_flowers[i]);
	}

	for (int i = 0; i < (int)(_flowerPos.size()); i++)
	{
		SAFE_DELETE(_flowerPos[i]);
	}

	for (int i = 0; i < (int)(_boundries.size()); i++)
	{
		SAFE_DELETE(_boundries[i]);
	}

	_parts.clear();
	_flowers.clear();
	_boundries.clear();

	SAFE_DELETE(s4BhasmaPowderFX);
	SAFE_DELETE(s4BhasmaPartiFX);
	SAFE_DELETE(_cvdo_handPowerGain);
	SAFE_DELETE(JAnim_NoteClose);
	SAFE_DELETE(JAnim_NoteOpen);
	SAFE_DELETE(s4tableonfireFX);
	SAFE_DELETE(flowerChart);
}

void L7S4_Pop1Pooja::InitPolys()
{

	int ArrayPts[] = {553,175,483,400,681,553,884,404,812,174,558,171,551,88,959,87,1005,132,1008,572,364,578,359,92,545,86};
	CPolygon* OuterPoly=  new CPolygon(13);
	OuterPoly->Init(ArrayPts);
	_boundries.push_back(OuterPoly);

	int ArrayPts1[] = {553,322,535,392,658,478,658,374};
	CPolygon* InnerPoly1=  new CPolygon(4);
	InnerPoly1->Init(ArrayPts1);
	_boundries.push_back(InnerPoly1);
	
	int ArrayPts2[] = {575,276,591,210,784,207,838,378,710,484,706,337};
	CPolygon* InnerPoly2=  new CPolygon(6);
	InnerPoly2->Init(ArrayPts2);
	_boundries.push_back(InnerPoly2);

	//int ArrayPts[] = {554,170,489,397,685,552,884,400,809,170,559,167,525,141,827,136,925,412,685,577,446,411,522,148};
	//CPolygon* OuterPoly=  new CPolygon(12);
	//OuterPoly->Init(ArrayPts);
	//_boundries.push_back(OuterPoly);

	//int ArrayPts1[] = {568,275,590,203,785,200,808,272,687,335};
	//CPolygon* InnerPoly1=  new CPolygon(5);
	//InnerPoly1->Init(ArrayPts1);
	//_boundries.push_back(InnerPoly1);

	//int ArrayPts2[] = {706,367,814,313,844,381,706,484};
	//CPolygon* InnerPoly2=  new CPolygon(4);
	//InnerPoly2->Init(ArrayPts2);
	//_boundries.push_back(InnerPoly2);

	//int ArrayPts3[] = {528,391,666,488,669,369,553,312};
	//CPolygon* InnerPoly3=  new CPolygon(4);
	//InnerPoly3->Init(ArrayPts3);
	//_boundries.push_back(InnerPoly3);
}

void L7S4_Pop1Pooja::DrawAt(CPoint mPos)
{
	for (int i = 0; i < (int)_parts.size(); i++)
	{
		if (_parts[i].getObjRect().Intersection(&mPos))
		{
			_parts[i].Draw();
		}
	}
}

void L7S4_Pop1Pooja::ResetLineDraw()
{
	Control::Audio->QuickLoadAndPlaySFX("s4p1_bhasmamrest");

	for (int i = 0; i < (int)_parts.size(); i++)
	{
		_parts[i].Reset();
	}
}

void L7S4_Pop1Pooja::OnPopupOpened()
{
	Popup::OnPopupOpened();
	ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopPoojaPuzzleMode = -1;

	if (_hashBoxUsed && !_hashDrawComplete)
	{
		_pzMode = L7S4_Puzzle_BusmomPlaced;
		return;
	}

	if (_placedAllFlowers && !_flowerPuzzleComplete)
	{
		_pzMode = L7S4_Puzzle_Flower;
		return;
	}
}

 void L7S4_Pop1Pooja::OnPopupCloseBtnClicked()
 {
	 Popup::OnPopupCloseBtnClicked();
	if (_hashBoxUsed && !_hashDrawComplete)
	{
		Hud->InvBox_StopPuzzleMode();
		s4BhasmaPowderFX->StopEmit();
		s4BhasmaPartiFX->StopEmit();
		s4tableonfireFX->StopImmediate();
		_isDrawing = false;
	}

	if (_placedAllFlowers && !_flowerPuzzleComplete)
	{
		Hud->InvBox_StopPuzzleMode();
	}

	_isInPuzzleMod = false;
	Hud->AllowInput = true;
#ifdef TOUCH_UI
	GFApp->pinchDisabled = false;
#endif
 }


