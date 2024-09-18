//====================================
//  created by : Sreeraj
//  copyright  : Tuttifrutti Games
//====================================

#include "L8_S1_PZDoor.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

RingPart::RingPart(void)
{
	obj = NULL;
}


RingPart::~RingPart(void)
{
}

L8_S1_PZDoor::L8_S1_PZDoor()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S1\\PZ1\\PZ1.SCN");

	swapPart1New = NULL;
	swapPart2New = NULL;

	path = "LV8\\S1\\PZ1\\";
	InitSceneElements();

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	Control::Audio->ClearUnusedTracks();
	if( !Control::Audio->IsPlaying(aTrackLevel5) )
		Control::Audio->PlaySample(aTrackLevel5,true);
	//if( !Control::Audio->IsPlaying(aLightRainAmb) )		
	//{
	//	Control::Audio->Samples[aLightRainAmb]->SetVolume(70);
	//	Control::Audio->PlaySample(aLightRainAmb,true);//will be paused in iPad popups!!
	//}
	//else //ensure 70
	//	Control::Audio->Samples[aLightRainAmb]->SetVolume(70);

	//if( !Control::Audio->IsPlaying(aAmbBeachLoop) )		
	//{
	//	Control::Audio->PlaySample(aAmbBeachLoop,true);//will be paused in iPad popups!!
	//}

	Hud->InvBox_StartPuzzleMode();

	/*Janim_doorOpen = new JSONAnimator(0,CC_CALLBACK_1(L8_S1_PZDoor::OnAnimComplete,this));
	Janim_doorOpen->parseAndLoadJSON("ANIM\\L8\\L8_S1_PZ_DOOROPENING.JSON",HOScene);*/

	_delayTimer = -1;

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S1_PZDoorHandPuzzle]  = true;
}

void L8_S1_PZDoor::OnAnimComplete(int animID)
{
	ProfileSystem::Instance->ActiveProfile.L8_S1_PZDoorOpened = true;
}

void L8_S1_PZDoor::InitSceneElements()
{
	isHandMoveAnimationRunning = false;
	isSwaping = false;
	isSwapingSameCol = false;
	tweenTime = 0;
	swapTime = 0.50f;
	swapPart1 = NULL;
	swapPart2 = NULL;
	puzzleSoved = false;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			ringParts[i][j] = new RingPart();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			visibleRings[i][j] = NULL;
		}
	}

	ringParts[0][0]->obj =  HOScene->GetObjectByID("LeftRing_A1");
	ringParts[0][1]->obj =  HOScene->GetObjectByID("LeftRing_A2");
	ringParts[0][2]->obj =  HOScene->GetObjectByID("LeftRing_A3");
	ringParts[0][3]->obj =  HOScene->GetObjectByID("LeftRing_A4");
	ringParts[0][4]->obj =  HOScene->GetObjectByID("LeftRing_B1");
	ringParts[0][5]->obj =  HOScene->GetObjectByID("LeftRing_B2");
	ringParts[0][6]->obj =  HOScene->GetObjectByID("LeftRing_B3");
	ringParts[0][7]->obj =  HOScene->GetObjectByID("LeftRing_B4");
	ringParts[0][8]->obj =  HOScene->GetObjectByID("LeftRing_C1");
	ringParts[0][9]->obj =  HOScene->GetObjectByID("LeftRing_C2");
	ringParts[0][10]->obj = HOScene->GetObjectByID("LeftRing_C3");
	ringParts[0][11]->obj = HOScene->GetObjectByID("LeftRing_C4");

	int row = -1;
	for (int i = 0; i < 12; i++)
	{
		row ++;
		ringParts[0][i]->index = i;
		ringParts[0][i]->col = 0;
		ringParts[0][i]->row = row;
		ringParts[0][i]->currentRow = row;
		ringParts[0][i]->isVisible = false;
		ringParts[0][i]->obj->Show(false);
		if(row == 3) row = -1;
	}

	ringParts[1][0]->obj =  HOScene->GetObjectByID("MiddleRing_A1");
	ringParts[1][1]->obj =  HOScene->GetObjectByID("MiddleRing_A2");
	ringParts[1][2]->obj =  HOScene->GetObjectByID("MiddleRing_A3");
	ringParts[1][3]->obj =  HOScene->GetObjectByID("MiddleRing_A4");
	ringParts[1][4]->obj =  HOScene->GetObjectByID("MiddleRing_B1");
	ringParts[1][5]->obj =  HOScene->GetObjectByID("MiddleRing_B2");
	ringParts[1][6]->obj =  HOScene->GetObjectByID("MiddleRing_B3");
	ringParts[1][7]->obj =  HOScene->GetObjectByID("MiddleRing_B4");
	ringParts[1][8]->obj =  HOScene->GetObjectByID("MiddleRing_C1");
	ringParts[1][9]->obj =  HOScene->GetObjectByID("MiddleRing_C2");
	ringParts[1][10]->obj =  HOScene->GetObjectByID("MiddleRing_C3");
	ringParts[1][11]->obj =  HOScene->GetObjectByID("MiddleRing_C4");

	row = -1;
	for (int i = 0; i < 12; i++)
	{
		row ++;
		ringParts[1][i]->index = i;
		ringParts[1][i]->col = 1;
		ringParts[1][i]->row = row;
		ringParts[1][i]->currentRow = row;
		ringParts[1][i]->isVisible = false;
		ringParts[1][i]->obj->Show(false);

		if(row == 3) row = -1;
	}

	ringParts[2][0]->obj =  HOScene->GetObjectByID("RightRing_A1");
	ringParts[2][1]->obj =  HOScene->GetObjectByID("RightRing_A2");
	ringParts[2][2]->obj =  HOScene->GetObjectByID("RightRing_A3");
	ringParts[2][3]->obj =  HOScene->GetObjectByID("RightRing_A4");
	ringParts[2][4]->obj =  HOScene->GetObjectByID("RightRing_B1");
	ringParts[2][5]->obj =  HOScene->GetObjectByID("RightRing_B2");
	ringParts[2][6]->obj =  HOScene->GetObjectByID("RightRing_B3");
	ringParts[2][7]->obj =  HOScene->GetObjectByID("RightRing_B4");
	ringParts[2][8]->obj =  HOScene->GetObjectByID("RightRing_C1");
	ringParts[2][9]->obj =  HOScene->GetObjectByID("RightRing_C2");
	ringParts[2][10]->obj = HOScene->GetObjectByID("RightRing_C3");
	ringParts[2][11]->obj = HOScene->GetObjectByID("RightRing_C4");

	row = -1;
	for (int i = 0; i < 12; i++)
	{
		row ++;
		ringParts[2][i]->index = i;
		ringParts[2][i]->col = 2;
		ringParts[2][i]->row = row;
		ringParts[2][i]->currentRow = row;
		ringParts[2][i]->isVisible = false;
		ringParts[2][i]->obj->Show(false);
		if(row == 3) row = -1;
	}

	symbolGlow[0] = HOScene->GetObjectByID("Btn_01Glow");
	symbolGlow[1] = HOScene->GetObjectByID("Btn_02Glow");
	symbolGlow[2] = HOScene->GetObjectByID("Btn_03GLow");
	for (int i = 0; i < 3; i++)
	{
		symbolGlow[i]->Show(false);
	}

	handWithGlow = HOScene->GetObjectByID("Hand2");
	handWithGlow->Show(false);

	hand = HOScene->GetObjectByID("Hand");

	MouseOver[0] = HOScene->GetObjectByID("MouseOver1");
	MouseOver[1] = HOScene->GetObjectByID("MouseOver2");
	MouseOver[2] = HOScene->GetObjectByID("MouseOver3");
	for (int i = 0; i < 3; i++)
	{
		MouseOver[i]->Show(false);
	}

	Selection[0] = HOScene->GetObjectByID("Selection1");
	Selection[1] = HOScene->GetObjectByID("Selection2");
	Selection[2] = HOScene->GetObjectByID("Selection3");
	for (int i = 0; i < 3; i++)
	{
		Selection[i]->Show(false);
	}

	for (int i = 0; i < 12; i++)
	{
		ringIndices[i] = -1;
	}
	ringPieceCount = 0;

	for (int i = 0; i < 4; i++)
	{
		rowContInColumn[i] = 0;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ringPositions[i][j] = ringParts[i][j]->obj->Pos;
		}
	}

	int ArrayPts0[] = {442,490,540,439,551,464,454,512};
	positionPoly[0][0] =  new CPolygon(4);
	positionPoly[0][0]->Init(ArrayPts0);

	int ArrayPts1[] = {467,536,455,513,551,465,563,487};
	positionPoly[0][1] =  new CPolygon(4);
	positionPoly[0][1]->Init(ArrayPts1);

	int ArrayPts2[] = {468,537,565,490,577,512,480,563};
	positionPoly[0][2] =  new CPolygon(4);
	positionPoly[0][2]->Init(ArrayPts2);

	int ArrayPts3[] = {480,563,577,513,590,539,493,587};
	positionPoly[0][3] =  new CPolygon(4);
	positionPoly[0][3]->Init(ArrayPts3);

	int ArrayPts4[] = {623,432,623,457,734,458,734,427};
	positionPoly[1][0] =  new CPolygon(4);
	positionPoly[1][0]->Init(ArrayPts4);

	int ArrayPts5[] = {623,459,624,483,734,483,734,458};
	positionPoly[1][1] =  new CPolygon(4);
	positionPoly[1][1]->Init(ArrayPts5);

	int ArrayPts6[] = {625,485,733,485,735,509,622,509};
	positionPoly[1][2] =  new CPolygon(4);
	positionPoly[1][2]->Init(ArrayPts6);

	int ArrayPts7[] = {624,513,623,540,735,540,734,513};
	positionPoly[1][3] =  new CPolygon(4);
	positionPoly[1][3]->Init(ArrayPts7);

	int ArrayPts8[] = {791,474,806,447,901,509,884,532};
	positionPoly[2][0] =  new CPolygon(4);
	positionPoly[2][0]->Init(ArrayPts8);

	int ArrayPts9[] = {777,498,791,473,885,534,869,556};
	positionPoly[2][1] =  new CPolygon(4);
	positionPoly[2][1]->Init(ArrayPts9);

	int ArrayPts10[] = {762,522,776,499,869,558,856,579};
	positionPoly[2][2] =  new CPolygon(4);
	positionPoly[2][2]->Init(ArrayPts10);

	int ArrayPts11[] = {747,545,761,523,854,580,840,603};
	positionPoly[2][3] =  new CPolygon(4);
	positionPoly[2][3]->Init(ArrayPts11);

	/*PuzzleSolvedMsg = HOScene->GetObjectByID("PuzzleSolvedMsg");
	PuzzleSolvedMsg->Show(false);*/

	SetWinParts();

	setPositionForRingPieces();

	if(ProfileSystem::Instance->ActiveProfile.L8_S5_IsPuzzleDataSaved)
	{
		SetRingsPartWithSaveData();
	}
	else
	{
		GenerateRingPieces();
		SaveData();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				visibleRings[i][j]->obj->Show(false);
			}
		}
		handEndY = hand->Pos.y;
		hand->Pos.y += 400;
		handStartY = hand->Pos.y;
		isHandMoveAnimationRunning = true;
		tweenTime = 0.0f;
	}
}

void L8_S1_PZDoor::SetRingsPartWithSaveData()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if(ProfileSystem::Instance->ActiveProfile.L8_S5_RingPiece_Visibility[i][j] == true)
			{
				ringParts[i][j]->isVisible = true;
				ringParts[i][j]->obj->Show(true);
				ringParts[i][j]->currentRow = ProfileSystem::Instance->ActiveProfile.L8_S5_RingPiece_CurrentRow[i][j];
				ringParts[i][j]->obj->Pos = ringPositions[i][ringParts[i][j]->currentRow];
				ringParts[i][j]->ringPositionPoly = positionPoly[i][ringParts[i][j]->currentRow];
			}
		}
	}
}

bool L8_S1_PZDoor::isPuzzleSolved()
{
	int solvedPieceCount = 0;
	bool retValue = false;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if(ringParts[i][j]->isWinPart == true)
			{
				if(ringParts[i][j]->isVisible == true && ringParts[i][j]->currentRow == ringParts[i][j]->row)
				{
					//K_LOG ("i = %d, j = %d Col ---> %d Row ---> %d currentRow = %d",i,j,ringParts[i][j]->col,ringParts[i][j]->row, ringParts[i][j]->currentRow);
					solvedPieceCount++;
				}
			}
		}
	}
	//K_LOG ("solvedPieceCount = %d",solvedPieceCount);
	if(solvedPieceCount >= 12) 
	{
		retValue = true;
	}
	else
	{
		 retValue = false;
	}

	return retValue;
}

void L8_S1_PZDoor::SetWinParts()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			ringParts[i][j]->isWinPart = false;
		}
	}

	ringParts[0][0]->isWinPart = true;
	ringParts[0][1]->isWinPart = true;
	ringParts[0][2]->isWinPart = true;
	ringParts[0][3]->isWinPart = true;

	ringParts[1][4]->isWinPart = true;
	ringParts[1][5]->isWinPart = true;
	ringParts[1][6]->isWinPart = true;
	ringParts[1][7]->isWinPart = true;

	ringParts[2][8]->isWinPart = true;
	ringParts[2][9]->isWinPart = true;
	ringParts[2][10]->isWinPart = true;
	ringParts[2][11]->isWinPart = true;
}

void L8_S1_PZDoor::setPositionForRingPieces()
{
	int index;
	for (int i = 0; i < 3; i++)
	{
		index = 0;
		for (int j = 0; j < 12; j++)
		{
			ringParts[i][j]->ringPartPos = ringPositions[i][index];
			ringParts[i][j]->ringPositionPoly = positionPoly[i][index];
			ringParts[i][j]->currentRow = index;
			index++;
			if(index == 4) index = 0;
		}
	}
}
void L8_S1_PZDoor::swapRingPieces(RingPart *part1,RingPart *part2)
{
	if (swapPart1New)
		{SAFE_DELETE(swapPart1New->obj);}
	
	SAFE_DELETE(swapPart1New);

	if (swapPart2New)
	{	SAFE_DELETE(swapPart2New->obj);}
	
	SAFE_DELETE(swapPart2New);

	swapPart1New = new RingPart();
	swapPart1New->obj = new CObject();

	swapPart1New->obj->LoadSprite(getSpriteName(part1));
	swapPart1New->obj->SetZ(200);
	swapPart1New->obj->SetPos(&part1->obj->Pos);
	part1->obj->Show(false);
	part1->isVisible = false;
	swapPart1New->obj->Show(true);

	swapPart2New = new RingPart();
	swapPart2New->obj = new CObject();
	swapPart2New->obj->LoadSprite(getSpriteName(part2));
	swapPart2New->obj->SetZ(200);
	swapPart2New->obj->SetPos(&part2->obj->Pos);
	part2->obj->Show(false);
	part2->isVisible = false;
	swapPart2New->obj->Show(true);
	isSwaping = true;
}

std::string L8_S1_PZDoor::getSpriteName(RingPart *obj)
{
	std::string name;
	switch (obj->col)
	{
		case 0:
			name = "LeftRing";
			break;
		case 1:
			name = "MiddleRing";
			break;
		case 2:
			name = "RightRing";
			break;
	}

	if(obj->index <= 3)
	{
		name =	name + "_A" + MKSTR(obj->row+1).c_str() + ".SPR";
	}
	else if(obj->index <= 7)
	{
		name = name + "_B" + MKSTR(obj->row+1).c_str() + ".SPR";
	}
	else if(obj->index <= 11)
	{
		name = name + "_C" + MKSTR(obj->row+1).c_str() + ".SPR";
	}

//	K_LOG ("Path name " , path + name);

	return path + name;
}

void L8_S1_PZDoor::GenerateRingPieces()
{
	while (true)
	{
		int index =  rand() % 12;
		if(index == 12) index = 11;
		
		if(ringIndices[index] != -1) continue;
		int colIndex;
		while (true)
		{
			colIndex = rand() % 3;
			if(colIndex == 3) colIndex = 2;
			if(rowContInColumn[colIndex] >= 4) continue;
			break;
		}
		rowContInColumn[colIndex]++;
		ringIndices[index] = index;

		ringParts[colIndex][index]->obj->Show(true); 
		ringParts[colIndex][index]->isVisible = true;
		ringParts[colIndex][index]->currentRow = index;
		
		visibleRings[colIndex][rowContInColumn[colIndex] - 1] = ringParts[colIndex][index];
		ringPieceCount ++;

		if(ringPieceCount >= 12) break;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			visibleRings[i][j]->obj->Pos = ringPositions[i][j];
			visibleRings[i][j]->currentRow = j;
			visibleRings[i][j]->ringPositionPoly = positionPoly[i][j];
		}
	}
}

void L8_S1_PZDoor::SaveData()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			 ProfileSystem::Instance->ActiveProfile.L8_S5_RingPiece_Visibility[i][j] = ringParts[i][j]->isVisible;
			 ProfileSystem::Instance->ActiveProfile.L8_S5_RingPiece_CurrentRow[i][j] = ringParts[i][j]->currentRow;
		}
	}

	 ProfileSystem::Instance->ActiveProfile.L8_S5_IsPuzzleDataSaved = true;
}

void L8_S1_PZDoor::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL8theme) )
	{
		Control::Audio->Samples[aTrackL8theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL8theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL8theme);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL8puzzle) )
				Control::Audio->PlaySample(aTrackL8puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL8puzzle))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL8puzzle, true);
		}
	}
}

float L8_S1_PZDoor::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L8_S1_PZDoor::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if (_delayTimer > -1)
	{
		_delayTimer++;

		if (_delayTimer >= 400)
		{
			/*Janim_doorOpen->PlayAnim();*/
			Hud->InvBox_StopPuzzleMode();
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
			SendMessage(ControlExit, ID, HOSM_NewArea);		

			_delayTimer = -1;
		}
	}

	if(!puzzleSoved)
	{
		if(isHandMoveAnimationRunning)
		{

			hand->Pos.y -= 5;
			handStartY = hand->Pos.y;
		//	K_LOG ("hand position ************** %d ",handStartY);

			if( hand->Pos.y <= handEndY )
			{
				hand->Pos.y = handEndY;
				isHandMoveAnimationRunning = false;

				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						visibleRings[i][j]->obj->Show(true);
					}
				}
			}
			return;
		}
		if(isSwaping)
		{
			tweenTime += Control::LogicRate;

			CPoint temp;
			CPoint temp2;
			temp.x = swapPart2->obj->Pos.x + (swapPart1->obj->Pos.x - swapPart2->obj->Pos.x) * easing(tweenTime,0,1,swapTime);
			temp.y = swapPart2->obj->Pos.y + (swapPart1->obj->Pos.y - swapPart2->obj->Pos.y) * easing(tweenTime,0,1,swapTime);

			swapPart2New->obj->Pos = temp;

			temp2.x = swapPart1->obj->Pos.x + (swapPart2->obj->Pos.x - swapPart1->obj->Pos.x) * easing(tweenTime,0,1,swapTime);
			temp2.y = swapPart1->obj->Pos.y + (swapPart2->obj->Pos.y - swapPart1->obj->Pos.y) * easing(tweenTime,0,1,swapTime);

			swapPart1New->obj->Pos = temp2;

			if( tweenTime >= swapTime )
			{
				swapPart1New->obj->Pos = swapPart2->obj->Pos;
				swapPart2New->obj->Pos = swapPart1->obj->Pos;
				swapPart1New->obj->Show(false);
				swapPart2New->obj->Show(false);


				RingPart *temp1,*temp2;

				if(swapPart1->col == swapPart2->col)
				{
					CPoint tempPos = swapPart1->obj->Pos;
					int tempRow = swapPart1->currentRow;
				
					swapPart1->obj->Pos = swapPart2->obj->Pos;
					swapPart1->currentRow = swapPart2->currentRow;
					swapPart1->ringPositionPoly = swapPart2->ringPositionPoly;
					swapPart1->obj->Show(true);
					swapPart1->isVisible = true;

					swapPart2->obj->Pos = tempPos;
					swapPart2->currentRow = tempRow;
					swapPart2->ringPositionPoly = positionPoly[swapPart2->col][tempRow];
					swapPart2->obj->Show(true);
					swapPart2->isVisible = true;
				}
				else
				{
					temp1 = ringParts[swapPart1->col][swapPart2->index];
					temp1->obj->Show(true);
					temp1->isVisible = true;
					temp1->obj->Pos = ringPositions[temp1->col][swapPart1->currentRow];
					temp1->currentRow = swapPart1->currentRow;
					temp1->ringPositionPoly = positionPoly[temp1->col][temp1->currentRow];

					temp2 = ringParts[swapPart2->col][swapPart1->index];
					temp2->obj->Show(true);
					temp2->isVisible = true;
					temp2->obj->Pos = ringPositions[temp2->col][swapPart2->currentRow];
					temp2->currentRow = swapPart2->currentRow;
					temp2->ringPositionPoly = positionPoly[temp2->col][temp2->currentRow];
				}

				SaveData();

				if(isPuzzleSolved())
				{
					//PuzzleSolvedMsg->Show(true);
					puzzleSoved = true;
					ProfileSystem::Instance->ActiveProfile.L8_S1_HandPuzzleSolved= true;
					hand->Show(false);
					handWithGlow->Show(true);
					OnPuzzleComplete();
				}

				Selection[0]->Show(false);
				Selection[1]->Show(false);
				Selection[2]->Show(false);

				tweenTime = 0;
				isSwaping = false;
				swapPart1 = NULL;
				swapPart2 = NULL;
			}
			return;
		}

		for (int i = 0; i < 3; i++)
		{
			MouseOver[i]->Show(false);
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				if(ringParts[i][j]->ringPositionPoly->Intersection(&pos) &&  ringParts[i][j]->isVisible == true)
				{
					Cursor::SetMode(CA_HandPointy);
					UpdateMousePointer(i,j);
					if(Control::Input->LBclicked())
					{
						Control::Audio->QuickLoadAndPlaySFX("s5pz1_TakePuzzlePiece");
						if(swapPart1 == NULL)
						{
							swapPart1 = ringParts[i][j];
							Selection[i]->Show(true);
							Selection[i]->Pos = ringPositions[i][swapPart1->currentRow];
						}
						else
						{
							if(swapPart1 == ringParts[i][j])
							{
								Selection[i]->Show(false);
								Selection[i]->Pos = ringPositions[i][swapPart1->currentRow];
								swapPart1 = NULL;
							}
							else
							{
								swapPart2 = ringParts[i][j];
								Selection[i]->Show(true);
								Selection[i]->Pos = ringPositions[i][swapPart2->currentRow];

								swapRingPieces(swapPart1,swapPart2);
							}
						}
					}
				}
			}
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			Hud->InvBox_StopPuzzleMode();
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
			SendMessage(ControlExit, ID, HOSM_NewArea);							
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L8_S1_ShukrasPlace);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L8_S1_ShukrasPlace);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				Hud->InvBox_StopPuzzleMode();
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S1_ShukrasPlace, 2);
			}
#else			
			Hud->InvBox_StopPuzzleMode();
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
							
		}
	}
}

void L8_S1_PZDoor::UpdateMousePointer(int col,int row)
{
	MouseOver[col]->Show(true);
	MouseOver[col]->Pos = ringParts[col][row]->obj->Pos;
}

void L8_S1_PZDoor::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L8_S1_PZDoor::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	puzzleSoved = true;
	ProfileSystem::Instance->ActiveProfile.L8_S1_HandPuzzleSolved = true;
	OnPuzzleComplete();
}

void L8_S1_PZDoor::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: ++iNonHOHint;

			break;
		case 1: ++iNonHOHint;
			/*if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S3_CaveMouth])
			{
			InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
			return;
			}*/
			break;
		case 2: ++iNonHOHint;

			break;
		case 3: ++iNonHOHint;

			break;
		case 4: ++iNonHOHint;

			break;
		case 5: ++iNonHOHint;

			break;
		case 6: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 7: ++iNonHOHint;
				{
					Hud->teleportSystem->ShowTeleport();
					return;
				}
				break;

		}
	}
	while( iNonHOHint < MaxNonHOHints );

	Hud->ShowNothingToDoMsgBanner();
}

L8_S1_PZDoor::~L8_S1_PZDoor()
{
	Hud->InvBox_StopPuzzleMode();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			SAFE_DELETE(ringParts[i][j]);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//SAFE_DELETE( visibleRings[i][j])
			SAFE_DELETE( positionPoly[i][j])
		}
	}

	if (swapPart1New)
		{SAFE_DELETE(swapPart1New->obj);}
	
	SAFE_DELETE(swapPart1New);

	if (swapPart2New)
	{	SAFE_DELETE(swapPart2New->obj);}
	
	SAFE_DELETE(swapPart2New);
}

void L8_S1_PZDoor::OnPuzzleComplete()
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Hud->InvBox_StopPuzzleMode();

	MouseOver[0]->Show(false);
	MouseOver[1]->Show(false);
	MouseOver[2]->Show(false);

	HOScene->GetObjectByID("Btn_01")->SetEnable(false);
	HOScene->GetObjectByID("Btn_02")->SetEnable(false);
	HOScene->GetObjectByID("Btn_03")->SetEnable(false);

	for (int i = 0; i < 3; i++)
	{
		symbolGlow[i]->FadeInOut(4);
	}

	handWithGlow->FadeInOut(4);

	HOScene->GetObjectByID("Hand")->FadeOut(0.5);

	for (int i = 1; i < 5; i++)
	{
		HOScene->GetObjectByID("LeftRing_C" + MKSTR(i))->SetEnable(false);
		HOScene->GetObjectByID("LeftRing_B" + MKSTR(i))->SetEnable(false);
		HOScene->GetObjectByID("LeftRing_A" + MKSTR(i))->SetEnable(false);
		HOScene->GetObjectByID("MiddleRing_C" + MKSTR(i))->SetEnable(false);
		HOScene->GetObjectByID("MiddleRing_B" + MKSTR(i))->SetEnable(false);
		HOScene->GetObjectByID("MiddleRing_A" + MKSTR(i))->SetEnable(false);
		HOScene->GetObjectByID("RightRing_C" + MKSTR(i))->SetEnable(false);
		HOScene->GetObjectByID("RightRing_B" + MKSTR(i))->SetEnable(false);
		HOScene->GetObjectByID("RightRing_A" + MKSTR(i))->SetEnable(false);
	}

	_delayTimer = 0;
}

void L8_S1_PZDoor::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				//Cursor::SetMode(CA_ExitUp);
				
				//GFHud->InvBoxCtrl->NavigateText->Show(false);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = GFHud->InvBoxCtrl->travelArea;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
				
			GFHud->InvBoxCtrl->NavigateText->Text = "";
			GFHud->InvBoxCtrl->travelArea = -1;
			GFHud->InvBoxCtrl->navTapped = false;
			GFHud->InvBoxCtrl->Navigate->Show(false);					
			GFHud->InvBoxCtrl->NavArrow->Show(false);
		}
	}
}