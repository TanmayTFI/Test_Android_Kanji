//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 01 - PUZZLE: MUSHROOM
//====================================

#include "L4_S1_PZMushroom.h"
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
#include "AwardManager.h"

#define START_NODE	15

L4_S1_PZMushroom::L4_S1_PZMushroom(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV4\\S1\\P1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);
	float vertGap = 26, horizontalGap = InvAreaRect.w/4.0f;
	CPoint pos(0,0,0);

	pos.x = InvAreaRect.Left() + horizontalGap*2;
	pos.y = InvAreaRect.Top() + vertGap;

	_hintText1	= NULL;
	_hintText2	= NULL;

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	_hintText1 = new CBitmapText();
	_hintText1->SetTextProps(StringTable::getStr("Exdrag"), pos.x , pos.y, eZOrder_INVBOXITEMS, gold, eFont_25, Align_Center);
	_hintText1->SetZ(eZOrder_INVBOXITEMS);
	_hintText1->SetScale(0.9f);
	_hintText1->SetEnable(false);
	_hintText2 = new CBitmapText();
	_hintText2->SetTextProps(StringTable::getStr("Exmushroom"), pos.x, pos.y + vertGap, eZOrder_INVBOXITEMS, gold, eFont_25, Align_Center);
	_hintText2->SetZ(eZOrder_INVBOXITEMS);
	_hintText2->SetScale(0.9f);
	_hintText2->SetEnable(false);

	SetSparkleUpdate(false);
	Show(false);

	for (int i = 0; i < NUM_MUSHROOMS; i++)
	{
		std::string str = "M";
		str += std::to_string(i + 1);
		Mushroom_white[i] = GetObjectByID(str);
		Mushroom_white[i]->SetZ(zOrder + (i + 1) * 3 - 2);
#ifndef TOUCH_UI
		Mushroom_white[i]->SetMouseDownCallback(CC_CALLBACK_1(L4_S1_PZMushroom::OnMouseClickOnMushroom, this));
#endif
		Mushroom_white[i]->SetMouseOverProperties(25, CC_CALLBACK_1(L4_S1_PZMushroom::OnMouseOverMushroom, this));

		str = "MO";
		str += std::to_string(i + 1);
		Mushroom_mouseOver[i] = GetObjectByID(str);
		Mushroom_mouseOver[i]->SetZ(zOrder + (i + 1) * 3 - 1);
		Mushroom_mouseOver[i]->SetPos(Mushroom_white[i]->GetPosPointer());

		str = "MN";
		str += std::to_string(i + 1);
		Mushroom[i] = GetObjectByID(str);
		Mushroom[i]->SetZ(zOrder + (i + 1) * 3 - 0);
		Mushroom[i]->SetPos(Mushroom_white[i]->GetPosPointer());

		Mushroom_c *temp = new Mushroom_c();
		temp->id = i;

		switch (i)
		{
		case 0:
			temp->neighbourList[0] = 2;
			temp->neighbourList[1] = 3;
			temp->neighbourList[2] = -1;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 1:
			temp->neighbourList[0] = 2;
			temp->neighbourList[1] = 5;
			temp->neighbourList[2] = 6;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 2:
			temp->neighbourList[0] = 1;
			temp->neighbourList[1] = 5;
			temp->neighbourList[2] = 6;
			temp->neighbourList[3] = 7;
			temp->neighbourList[4] = 3;
			temp->neighbourList[5] = 0;
			break;
		case 3:
			temp->neighbourList[0] = 0;
			temp->neighbourList[1] = 2;
			temp->neighbourList[2] = 7;
			temp->neighbourList[3] = 4;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 4:
			temp->neighbourList[0] = 3;
			temp->neighbourList[1] = -1;
			temp->neighbourList[2] = -1;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 5:
			temp->neighbourList[0] = 1;
			temp->neighbourList[1] = 6;
			temp->neighbourList[2] = 8;
			temp->neighbourList[3] = 9;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 6:
			temp->neighbourList[0] = 1;
			temp->neighbourList[1] = 2;
			temp->neighbourList[2] = 5;
			temp->neighbourList[3] = 7;
			temp->neighbourList[4] = 9;
			temp->neighbourList[5] = 10;
			break;
		case 7:
			temp->neighbourList[0] = 2;
			temp->neighbourList[1] = 3;
			temp->neighbourList[2] = 6;
			temp->neighbourList[3] = 10;
			temp->neighbourList[4] = 11;
			temp->neighbourList[5] = -1;
			break;
		case 8:
			temp->neighbourList[0] = 5;
			temp->neighbourList[1] = 9;
			temp->neighbourList[2] = 12;
			temp->neighbourList[3] = 13;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 9:
			temp->neighbourList[0] = 5;
			temp->neighbourList[1] = 6;
			temp->neighbourList[2] = 8;
			temp->neighbourList[3] = 10;
			temp->neighbourList[4] = 13;
			temp->neighbourList[5] = 14;
			break;

		case 10:
			temp->neighbourList[0] = 6;
			temp->neighbourList[1] = 7;
			temp->neighbourList[2] = 9;
			temp->neighbourList[3] = 11;
			temp->neighbourList[4] = 14;
			temp->neighbourList[5] = 15;
			break;

		case 11:
			temp->neighbourList[0] = 7;
			temp->neighbourList[1] = 10;
			temp->neighbourList[2] = 15;
			temp->neighbourList[3] = 16;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 12:
			temp->neighbourList[0] = 8;
			temp->neighbourList[1] = 13;
			temp->neighbourList[2] = 17;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 13:
			temp->neighbourList[0] = 8;
			temp->neighbourList[1] = 9;
			temp->neighbourList[2] = 12;
			temp->neighbourList[3] = 14;
			temp->neighbourList[4] = 17;
			temp->neighbourList[5] = 18;
			break;
		case 14:
			temp->neighbourList[0] = 9;
			temp->neighbourList[1] = 10;
			temp->neighbourList[2] = 15;
			temp->neighbourList[3] = 13;
			temp->neighbourList[4] = 18;
			temp->neighbourList[5] = -1;
			break;
		case START_NODE:
			temp->neighbourList[0] = 10;
			temp->neighbourList[1] = 11;
			temp->neighbourList[2] = 14;
			temp->neighbourList[3] = 16;
			temp->neighbourList[4] = 19;
			temp->neighbourList[5] = -1;
			break;

		case 16:
			temp->neighbourList[0] = 11;
			temp->neighbourList[1] = 15;
			temp->neighbourList[2] = 19;
			temp->neighbourList[3] = 20;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 17:
			temp->neighbourList[0] = 12;
			temp->neighbourList[1] = 13;
			temp->neighbourList[2] = 18;
			temp->neighbourList[3] = 21;
			temp->neighbourList[4] = 22;
			temp->neighbourList[5] = -1;
			break;
		case 18:
			temp->neighbourList[0] = 13;
			temp->neighbourList[1] = 14;
			temp->neighbourList[2] = 17;
			temp->neighbourList[3] = 22;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 19:
			temp->neighbourList[0] = 15;
			temp->neighbourList[1] = 16;
			temp->neighbourList[2] = 20;
			temp->neighbourList[3] = 23;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 20:
			temp->neighbourList[0] = 16;
			temp->neighbourList[1] = 19;
			temp->neighbourList[2] = -1;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 21:
			temp->neighbourList[0] = 17;
			temp->neighbourList[1] = 22;
			temp->neighbourList[2] = 24;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 22:
			temp->neighbourList[0] = 17;
			temp->neighbourList[1] = 18;
			temp->neighbourList[2] = 21;
			temp->neighbourList[3] = 24;
			temp->neighbourList[4] = 25;
			temp->neighbourList[5] = -1;
			break;
		case 23:
			temp->neighbourList[0] = 19;
			temp->neighbourList[1] = 26;
			temp->neighbourList[2] = -1;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 24:
			temp->neighbourList[0] = 21;
			temp->neighbourList[1] = 22;
			temp->neighbourList[2] = 25;
			temp->neighbourList[3] = 27;
			temp->neighbourList[4] = 28;
			temp->neighbourList[5] = -1;
			break;
		case 25:
			temp->neighbourList[0] = 24;
			temp->neighbourList[1] = 22;
			temp->neighbourList[2] = 26;
			temp->neighbourList[3] = 28;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 26:
			temp->neighbourList[0] = 23;
			temp->neighbourList[1] = 25;
			temp->neighbourList[2] = -1;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		case 27:
			temp->neighbourList[0] = 24;
			temp->neighbourList[1] = 28;
			temp->neighbourList[2] = -1;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;

		case 28:
			temp->neighbourList[0] = 24;
			temp->neighbourList[1] = 25;
			temp->neighbourList[2] = 27;
			temp->neighbourList[3] = -1;
			temp->neighbourList[4] = -1;
			temp->neighbourList[5] = -1;
			break;
		default:
			break;
		}

		MushroomsArray.push_back(temp);

	}

	GetObjectByID("bg")->SetMouseUpCallback(CC_CALLBACK_1(L4_S1_PZMushroom::OnMouseUpOnBg, this));
	GetObjectByID("bg")->SetMouseClickCallback(CC_CALLBACK_1(L4_S1_PZMushroom::OnMouseUpOnBg, this));
	Mushroom_white[NUM_MUSHROOMS - 1]->SetMouseUpCallback(CC_CALLBACK_1(L4_S1_PZMushroom::OnMouseUpOnBg, this));


	OccupiedMushroomIndices[0] = 15;
	OccupiedMushroomIndices[1] = 23;
	OccupiedMushroomIndices[2] = 28;
	OccupiedMushroomIndices[3] = 22;
	OccupiedMushroomIndices[4] = 5;
	OccupiedMushroomIndices[5] = 13;
	OccupiedMushroomIndices[6] = 11;
	OccupiedMushroomIndices[7] = 1;
	OccupiedMushroomIndices[8] = 0;
	OccupiedMushroomIndices[9] = 4;

	for (int i = 0; i < NUM_SYMBOLS; i++)
	{
		std::string str = "sym_0";
		str += std::to_string(i);
		Symbols[i] = GetObjectByID(str);
		Symbols[i]->SetPos(Mushroom[OccupiedMushroomIndices[i]]->GetPosPointer());

		str = "Rsymbol_0";
		str += std::to_string(i);
		RedSymbols[i] = GetObjectByID(str);

		str = "Gsymbol_0";
		str += std::to_string(i);
		GreenSymbols[i] = GetObjectByID(str);
		GreenSymbols[i]->SetPos(RedSymbols[i]->GetPosPointer());
	}

	Lens = GetObjectByID("Lens_Large");
	Lens->SetEnable(false);

	// set solution path

	/*int solarray [] = {16,17,21,20,24,27,26,29,28,25,22,23,18,13,9,6,10,14,19,15,11,12,8,7,2,3,1,4,5};
	SolutionPath.clear();
	for (int i = 0; i < 29; i++)
	{
	SolutionPath.push_back(solarray[i]);
	}*/

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s1MushroomPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s1MushroomPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s1MushroomPuzzleAmb,true);
	}
	
	//MUSIC_SFX

	ResetPuzzle(START_NODE);

#ifdef TOUCH_UI
    PlayModeActive = true;
    TraversedPath.push_back(START_NODE);
    MushroomsVisited++;
    GreenSymbols[SymbolsVisited]->SetEnable(true);
    SymbolsVisited++;
#endif

}
void L4_S1_PZMushroom::ResetTouch()
{
	ResetPuzzle(START_NODE);

#ifdef TOUCH_UI
	PlayModeActive = true;
	TraversedPath.push_back(START_NODE);
	MushroomsVisited++;
	GreenSymbols[SymbolsVisited]->SetEnable(true);
	SymbolsVisited++;
#endif
}
void L4_S1_PZMushroom::ResetPuzzle(int startNode)
{
	PlayModeActive = false;
	MushroomsVisited = 0;
	SymbolsVisited = 0;
	TraversedPath.clear();

	for (int i = 0; i < NUM_MUSHROOMS; i++)
	{
		Mushroom_mouseOver[i]->SetEnable(true);
		Mushroom_mouseOver[i]->SetScale(1);
		Mushroom[i]->SetEnable(true);
	}

	for (int i = 0; i < NUM_SYMBOLS; i++)
	{
		GreenSymbols[i]->SetEnable(false);
		RedSymbols[i]->SetEnable(false);
	}

	Mushroom[startNode]->SetEnable(false);
	Mushroom_mouseOver[startNode]->SetEnable(false);
	Mushroom_mouseOver[startNode]->SetScale(1.1f);
	//Mushroom_mouseOver[startNode]->SetEnable(false);


}


void L4_S1_PZMushroom::OnMouseClickOnMushroom(CObject * object)
{
	ResetPuzzle(START_NODE);

	if (object->UID.compare("M16") == 0 && !PlayModeActive && !CheckIfPuzzleComplete())
	{
		PlayModeActive = true;
		TraversedPath.push_back(START_NODE);
		MushroomsVisited++;
		GreenSymbols[SymbolsVisited]->SetEnable(true);
		SymbolsVisited++;
	}
}

void L4_S1_PZMushroom::OnMouseOverMushroom(CObject *obj)
{
	if (PlayModeActive)
	{
		for (int i = 0; i < NUM_MUSHROOMS; i++)
		{
			std::string id = "M" + std::to_string(i + 1);

			if (obj->UID.compare(id) == 0)
			{
				bool isNeighbour = false;

				for (int j = 0; j < 6; j++)
				{
					int currNode = TraversedPath.at(MushroomsVisited - 1);

					if (MushroomsArray.at(currNode)->neighbourList[j] == i)//check for neoghbourhood
					{
						isNeighbour = true;
						//mushroom not visited
						if (Mushroom[i]->IsActive)
						{
							Mushroom[i]->IsActive = false;
							Mushroom[i]->SetEnable(false);
							Mushroom_mouseOver[i]->SetScale(1.1f);
							TraversedPath.push_back(i);
							MushroomsVisited++;

							for (int m = 0; m < NUM_SYMBOLS; m++)
							{
								if (OccupiedMushroomIndices[m] == i)
								{
									//a symbol in mushroom : order incorrect
									if ((Symbols[SymbolsVisited]->GetPosPointer()->x != Mushroom[i]->GetPosPointer()->x) || (Symbols[SymbolsVisited]->GetPosPointer()->y != Mushroom[i]->GetPosPointer()->y))
									{
										//ResetPuzzle(START_NODE);
										RedSymbols[m]->SetEnable(true);
										SymbolsVisited++;
									}
									else//a symbol in mushroom : order correct
									{
										GreenSymbols[m]->SetEnable(true);
										SymbolsVisited++;
									}
								}
							}

						}
						else//mushroom already visited
						{
							for (int k = 0; k < (int)(TraversedPath.size()); k++)
							{
								if (TraversedPath.at(k) == i)
								{
                                    int diff = (int) TraversedPath.size() - k;  //Sac: converted Implicit to explicit


									for (int l = 0; l < diff - 1; l++)//reverse traverse up to selected one
									{
										MushroomsVisited--;
										Mushroom[TraversedPath.at(MushroomsVisited)]->SetEnable(true);
										Mushroom_mouseOver[TraversedPath.at(MushroomsVisited)]->SetScale(1);
										for (int m = 0; m < NUM_SYMBOLS; m++)
										{
											if (OccupiedMushroomIndices[m] == TraversedPath.at(MushroomsVisited))//check for symbols in reverse traversal
											{
												if (GreenSymbols[m]->IsActive)
												{
													GreenSymbols[m]->SetEnable(false);
													SymbolsVisited--;
												}
												else
												{
													if (RedSymbols[m]->IsActive)
													{
														RedSymbols[m]->SetEnable(false);
														SymbolsVisited--;
													}
												}

											}
										}
										TraversedPath.pop_back();
									}
								}
							}
						}
						break;
					}
				}

				//if (!isNeighbour)
				//{
				//	//ResetPuzzle(START_NODE);
				//}

				break;
			}
		}
	}
}

void L4_S1_PZMushroom::OnMouseUpOnBg(CObject *obj)
{
#ifndef TOUCH_UI
	PlayModeActive = false;
#endif
	//SLOG::Log("CheckIfPuzzleComplete ");
	if (CheckIfPuzzleComplete())
	{
		AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;
		ProfileSystem::Instance->ActiveProfile.L4_S1_PuzzleSolved = true;
		Control::Audio->QuickLoadAndPlaySFX(aSFXHintDischarge);

		for (int i = 0; i < NUM_MUSHROOMS; i++)
		{
			Mushroom_mouseOver[i]->SetEnable(false);
			Mushroom_mouseOver[i]->SetScale(1);

			float valArray1[] = {0,Mushroom_white[i]->GetPosPointer()->x,Mushroom_white[i]->GetPosPointer()->y,1,0};
			Mushroom_white[i]->TweenTo(valArray1,1,0,false);
		}

		for (int i = 0; i < NUM_SYMBOLS; i++)
		{
			float valArray2[] = {0,Symbols[i]->GetPosPointer()->x,Symbols[i]->GetPosPointer()->y,1,0};
			Symbols[i]->TweenTo(valArray2,1,0,false);
		}

		Lens->SetAlpha(0);
		Lens->SetEnable(true);
		float valArray[] = {1,Lens->GetPosPointer()->x,Lens->GetPosPointer()->y,1,0};
		Lens->TweenTo(valArray,1,0,true,CC_CALLBACK_1(L4_S1_PZMushroom::OnTweenComplete,this));

	}
}

void L4_S1_PZMushroom::OnTweenComplete(CObject *obj)
{
	Lens->SetEnable(false);
	Hud->TakeInventoryItem(eInv_L4_S1_BinocularLens);
	ProfileSystem::Instance->ActiveProfile.L4_S1_InvBinocularLensTaken = true;
	Hud->InvBox_StopPuzzleMode();
	ClosePopup(1.0f);
}

void L4_S1_PZMushroom::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	ProfileSystem::Instance->ActiveProfile.L4_S1_PuzzleSolved = true;
	PlayModeActive = false;
	for (int i = 0; i < NUM_MUSHROOMS; i++)
	{
		Mushroom[i]->SetEnable(false);
		Mushroom_mouseOver[i]->SetEnable(false);
		Mushroom_white[i]->SetEnable(false);
	}

	for (int i = 0; i < NUM_SYMBOLS; i++)
	{
		RedSymbols[i]->SetEnable(false);
		GreenSymbols[i]->SetEnable(true);
		Symbols[i]->SetEnable(false);
	}
	Lens->SetAlpha(0);
	Lens->SetEnable(true);
	float valArray[] = {1,Lens->GetPosPointer()->x,Lens->GetPosPointer()->y,1,0};
	Lens->TweenTo(valArray,1,0,true,CC_CALLBACK_1(L4_S1_PZMushroom::OnTweenComplete,this));

	Hud->InvBox_StopPuzzleMode();
}


bool L4_S1_PZMushroom::CheckIfPuzzleComplete()
{
	bool puzComplete = true;

	for (int i = 0; i < NUM_MUSHROOMS; i++)
	{
		if (Mushroom[i]->IsActive)
		{
			puzComplete = false;
			//SLOG::Log("Mushroom active ", i);
		}
	}

	for (int i = 0; i < NUM_SYMBOLS; i++)
	{
		if (RedSymbols[i]->IsActive)
		{
			puzComplete = false;
		}
	}

	return puzComplete;
}

void L4_S1_PZMushroom::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();

	if(GetObjectByID("resetBtn"))
	{
		if(GetObjectByID("resetBtn")->GetWorldRect().Intersection(&pos))
		{
			if(Control::Input->LBclicked())
			{
				ResetPuzzle(START_NODE);
#ifdef TOUCH_UI
				PlayModeActive = true;
				TraversedPath.push_back(START_NODE);
				MushroomsVisited++;
				GreenSymbols[SymbolsVisited]->SetEnable(true);
				SymbolsVisited++;
#endif
			}
		}
	}

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L4_S1_PuzzleVisited)
	{
		ProfileSystem::Instance->ActiveProfile.L4_S1_PuzzleVisited = true;

		Hud->ShowBannerText("L4weird");
	}

	if (!_hintText2->IsActive)
	{
		_hintText1->SetEnable(true);
		_hintText2->SetEnable(true);
	}
}

float L4_S1_PZMushroom::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4_S1_PZMushroom::~L4_S1_PZMushroom()
{
	Hud->InvBoxCtrl->ResetToInvBox();

	SAFE_DELETE(_hintText1);
	SAFE_DELETE(_hintText2);

	for (int i = 0; i < 28; i++)
	{
		delete MushroomsArray[i];
	}

	MushroomsArray.clear();

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL4s1MushroomPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL4s1MushroomPuzzleAmb);
	//MUSIC_SFX
}

void L4_S1_PZMushroom::Required()
{
	if( Control::Audio->IsPlaying(aTrackL4theme) )
	{
		Control::Audio->Samples[aTrackL4theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL4theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL4theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL4puzzle) )
				  Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL4puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
	}
}

void L4_S1_PZMushroom::ClosePopup(float delayTime)
{
	_hintText1->SetEnable(false);
	_hintText2->SetEnable(false);
	Popup::ClosePopup(delayTime);
}

void L4_S1_PZMushroom::ReceiveMessage(int val)
{	

}

void L4_S1_PZMushroom::ProcessNonHOHint()
{


}