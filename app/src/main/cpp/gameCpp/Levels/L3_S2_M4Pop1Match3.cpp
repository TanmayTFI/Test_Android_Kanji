//====================================
//  created by : Faisal Rasak
//  copyright  : Tuttifrutti Games
//====================================

#include "L3_S2_M4Pop1Match3.h"
#include "Hud.h"
#include "Util.h"

const CRectangle  MaskRect(443.0f,105.0f,483.0f,452.0f);

L3_S2_M4Pop1Match3::L3_S2_M4Pop1Match3(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S2\\MAZE4\\POP1\\P1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	s2m4match3endFX = NULL;
	s2m4match3endFX = new SuperFX("PRT\\L3\\s2m4match3end.INI", PointSystem::CreateCPoint(678,329,0), 1100);

	InitPopup();
}

void L3_S2_M4Pop1Match3::InitPopup()
{
	m4Image1	= GetObjectByID("m4Image1");
	m4Image1->SetEnable(false);

	strGems[0] = "LV3\\S2\\MAZE4\\POP1\\m4Image1.SPR";
	strGems[1] = "LV3\\S2\\MAZE4\\POP1\\m4Image2.SPR";
	strGems[2] = "LV3\\S2\\MAZE4\\POP1\\m4Image3.SPR";
	strGems[3] = "LV3\\S2\\MAZE4\\POP1\\m4Image4.SPR";
	strGems[4] = "LV3\\S2\\MAZE4\\POP1\\m4Image5.SPR";
	strGems[5] = "LV3\\S2\\MAZE4\\POP1\\m4Image6.SPR";

	for (int i = 0; i < MAX_MATCH3_LEFTSLIDERS; i++)
	{
		leftSlider[i] = GetObjectByID(std::string("m4LeftLockSlider" + MKSTR(i)).c_str());
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_M4SliderLeftRemoved[i])
			leftSlider[i]->SetEnable(false);
	}

	for (int i = 0; i < MAX_MATCH3_RIGHTSLIDERS; i++)
	{	
		rightSlider[i] = GetObjectByID(std::string("m4RightLockSlider" + MKSTR(i)).c_str());
		
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_M4SliderRightRemoved[i])
			rightSlider[i]->SetEnable(false);
	}
}

void L3_S2_M4Pop1Match3::InitPuzzle()
{
	float startX = m4Image1->Pos.x;
	float startY = m4Image1->Pos.y;

	float xPos = startX;
	float yPos = startY;
	//setting board tiles
	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		xPos = startX;
		yPos = startY + ( i * 56) + (i * 2) + 2;
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			
			gemType[i][j] = -1;
			board[i][j] = new CObject();
			board[i][j]->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4Image1.SPR");
			board[i][j]->SetZ(eZOrder_Popup + 20);
			xPos = startX + ( j * 61) + j + (.5f * j);

			board[i][j]->Pos.x = xPos;
			board[i][j]->Pos.y = yPos;
			board[i][j]->SetEnable(false);

			gems[i][j] = NULL;

		}
	}

	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			insertNewGem(i,j);
			isSelected[i][j] = false;
			removableGems[i][j] = false;
		}
	}

	for (int i = 0; i < MAX_BREAK_ANIMS; i++)
	{
		breakAnim[i] = NULL;
	}

	m4MouseOver = new CObject();
	m4MouseOver->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4MouseOver.SPR");
	m4MouseOver->SetZ(eZOrder_Popup + 22);
	
	m4MouseOver->SetEnable(false);

	gemSel[0] = new CObject();
	gemSel[0]->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4GemSel.SPR");
	gemSel[0]->SetZ(eZOrder_Popup + 22);
	
	gemSel[0]->SetEnable(false);

	gemSel[1] = new CObject();
	gemSel[1]->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4GemSel.SPR");
	gemSel[1]->SetZ(eZOrder_Popup + 22);
	
	gemSel[1]->SetEnable(false);

	selIndex = 0;
	isTileSwaping = false;
	swapTime = 0.3f;
	dropTime = 0.2f;
	isGemsDroping = false;
	isRemovablesMarked = false;

	for (int i = 0; i < MAX_MATCH3_SLIDERS; i++)
	{
		openingSliders[i] = NULL;
		m4SliderOpenAnim[i] = NULL;
		timerSliders[i] = -1;
	}

	for (int i = 0; i < MAX_MATCH3_TILES; i++)
	{
		dropingGems[i] = NULL;
	}
	
	sliderPos = 0;
	sliderIndex = 0;
	isSliding = false;
	isPuzzleSolved = false;
	isBreakAnimPlaying = false;
	indexBreakAnim = 0;

	Hud->InvBox_StartPuzzleMode();
}

void L3_S2_M4Pop1Match3::Required()
{

}

void L3_S2_M4Pop1Match3::SkipPuzzle()
{
	if (!isPuzzleSolved)
	{
		tweenTime = 0.0f;
		isPuzzleSolved = true;
		isSliding = false;
		isAnimationPlaying = true;
		s2m4match3endFX->Start(); 
		Control::Audio->QuickLoadAndPlaySFX("s2m4_puzzlecomplete");
		
		Hud->InvBox_StopPuzzleMode();
	}
}


void L3_S2_M4Pop1Match3::AddOpeningSliders()
{
	int index;
	bool isAllRightSlidesOpen = true;
	bool isAllLeftSlidesOpen = true;
	
	if(sliderPos == 0)
	{
		sliderPos = 1;
			
		for (int i = 0; i < MAX_MATCH3_LEFTSLIDERS; i++)
		{
			if(leftSlider[i]->IsActive)
			{
				isAllLeftSlidesOpen = false;
				break;
			}
		}
		if(isAllLeftSlidesOpen == false)
		{
			
			if( sliderIndex < MAX_MATCH3_SLIDERS)
			{
				do
				{
					index = rand()%MAX_MATCH3_LEFTSLIDERS;
				}
				while( !leftSlider[index]->IsActive ); //loop till we find an active lock to be unlocked!! 


				openingSliders[sliderIndex] = leftSlider[index];
				openingSlidersToXPos[sliderIndex] = leftSlider[index]->Pos.x - 30;
				openingSlidersFromXPos[sliderIndex] = leftSlider[index]->Pos.x;
				timerSliders[sliderIndex] = 0.0f;
				
				
				tempSliderOpenAnim = new CObject();
				tempSliderOpenAnim->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4SliderOpenAnim.SPR");
				tempSliderOpenAnim->SetZ(eZOrder_Popup + 30);
				tempSliderOpenAnim->Pos.x = leftSlider[index]->Pos.x - 52;
				tempSliderOpenAnim->Pos.y = leftSlider[index]->Pos.y - 28;
				tempSliderOpenAnim->ForcePlayAnimation(0);
				tempSliderOpenAnim->SetRotate(180);
				m4SliderOpenAnim[sliderIndex] = tempSliderOpenAnim;
				sliderIndex ++;

				ProfileSystem::Instance->ActiveProfile.L3_S2_M4SliderLeftRemoved[index] = true;

				leftSlider[index]->SetEnable(false);
				isSliding = true;
			}
			
		}
		else
		{
			isAllRightSlidesOpen = true;
			for (int i = 0; i < MAX_MATCH3_RIGHTSLIDERS; i++)
			{
				if(rightSlider[i]->IsActive)
				{
					isAllRightSlidesOpen = false;
					break;
				}
			}
			if(isAllRightSlidesOpen)
			{
					
					if( sliderIndex < MAX_MATCH3_SLIDERS)
					{
						do
						{
							index = rand()%MAX_MATCH3_RIGHTSLIDERS;
						}
						while( !rightSlider[index]->IsActive ); //loop till we find an active lock to be unlocked!! 
				
						openingSliders[sliderIndex] = rightSlider[index];
						openingSlidersToXPos[sliderIndex] = rightSlider[index]->Pos.x + 30;
						openingSlidersFromXPos[sliderIndex] = rightSlider[index]->Pos.x;
						timerSliders[sliderIndex] = 0.0f;
						
						
						tempSliderOpenAnim = new CObject();
						tempSliderOpenAnim->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4SliderOpenAnim.SPR");
						tempSliderOpenAnim->SetZ(eZOrder_Popup + 30);
						tempSliderOpenAnim->Pos.x = rightSlider[index]->Pos.x - 20;
						tempSliderOpenAnim->Pos.y = rightSlider[index]->Pos.y - 25;
						tempSliderOpenAnim->ForcePlayAnimation(0);
						tempSliderOpenAnim->SetRotate(0);
						m4SliderOpenAnim[sliderIndex] = tempSliderOpenAnim;

						sliderIndex ++;
						ProfileSystem::Instance->ActiveProfile.L3_S2_M4SliderRightRemoved[index] = true;

						rightSlider[index]->SetEnable(false);						
						isSliding = true;
					}

			 }
			
		}
		return;
	}
	else
	{
		sliderPos = 0;

		isAllRightSlidesOpen = true;
		for (int i = 0; i < MAX_MATCH3_RIGHTSLIDERS; i++)
		{
			if(rightSlider[i]->IsActive)
			{
				isAllRightSlidesOpen = false;
				break;
			}
		}
		if(isAllRightSlidesOpen == false)
		{
				if( sliderIndex < MAX_MATCH3_SLIDERS)
				{
					do
					{
						index = rand()%MAX_MATCH3_RIGHTSLIDERS;
					}
					while( !rightSlider[index]->IsActive ); //loop till we find an active lock to be unlocked!! 

			
					//K_LOG ("Loop 3....index = %d",index);				
					if(rightSlider[index]->IsActive)
					{
						openingSliders[sliderIndex] = rightSlider[index];
						openingSlidersToXPos[sliderIndex] = rightSlider[index]->Pos.x + 30;
						openingSlidersFromXPos[sliderIndex] = rightSlider[index]->Pos.x;
						timerSliders[sliderIndex] = 0.0f;
					
					
						tempSliderOpenAnim = new CObject();
						tempSliderOpenAnim->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4SliderOpenAnim.SPR");
						tempSliderOpenAnim->SetZ(eZOrder_Popup + 30);
						tempSliderOpenAnim->Pos.x = rightSlider[index]->Pos.x - 20;
						tempSliderOpenAnim->Pos.y = rightSlider[index]->Pos.y - 25;
						tempSliderOpenAnim->ForcePlayAnimation(0);
						tempSliderOpenAnim->SetRotate(0);
						m4SliderOpenAnim[sliderIndex] = tempSliderOpenAnim;
						sliderIndex ++;
						ProfileSystem::Instance->ActiveProfile.L3_S2_M4SliderRightRemoved[index] = true;

						rightSlider[index]->SetEnable(false);
						isSliding = true;
					
					}
				}

		}
		else
		{
			isAllLeftSlidesOpen = true;
			for (int i = 0; i < MAX_MATCH3_LEFTSLIDERS; i++)
			{
				if(leftSlider[i]->IsActive)
				{
					isAllLeftSlidesOpen = false;
					break;
				}
			}
			if(isAllLeftSlidesOpen)
			{
					if( sliderIndex < MAX_MATCH3_SLIDERS)
					{
						do
						{
							index = rand()%MAX_MATCH3_LEFTSLIDERS;
						}
						while( !leftSlider[index]->IsActive ); //loop till we find an active lock to be unlocked!! 

					
						if(leftSlider[index]->IsActive)
						{
							openingSliders[sliderIndex] = leftSlider[index];
							openingSlidersToXPos[sliderIndex] = leftSlider[index]->Pos.x - 30;
							openingSlidersFromXPos[sliderIndex] = leftSlider[index]->Pos.x;
							timerSliders[sliderIndex] = 0.0f;
						
						
							tempSliderOpenAnim = new CObject();
							tempSliderOpenAnim->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4SliderOpenAnim.SPR");
							tempSliderOpenAnim->SetZ(eZOrder_Popup + 30);
							tempSliderOpenAnim->Pos.x = leftSlider[index]->Pos.x - 52;
							tempSliderOpenAnim->Pos.y = leftSlider[index]->Pos.y - 28;
							tempSliderOpenAnim->ForcePlayAnimation(0);
							tempSliderOpenAnim->SetRotate(180);
							m4SliderOpenAnim[sliderIndex] = tempSliderOpenAnim;

							sliderIndex ++;
							ProfileSystem::Instance->ActiveProfile.L3_S2_M4SliderLeftRemoved[index] = true;

							leftSlider[index]->SetEnable(false);
							isSliding = true;
						
						}
					}
			}
		}
		return;
	}
}

void L3_S2_M4Pop1Match3::deleteMarkedGems()
{
	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			if(removableGems[i][j] == true)
			{
				gemType[i][j] = -1;
				if(gems[i][j] != NULL)
				{
					gems[i][j]->Show(false);
					gems[i][j] = NULL;
				}
			}
		}
	}
	//K_LOG ("countGemsToRemove-----------------------> %d",countGemsToRemove);
	
	if (countGemsToRemove >= 5)
	{
		for (int i = 0; i < 2; i++)
		{
			AddOpeningSliders();	
		}
	}
	else if(countGemsToRemove == 3 || countGemsToRemove == 4)
	{
		AddOpeningSliders();
	}
	

	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			removableGems[i][j] = false;
		}
	}
}

void L3_S2_M4Pop1Match3::ShowGemBreakAnim()
{
	if( indexBreakAnim >= MAX_BREAK_ANIMS )//MUST NOT ALLOW OVERFLOW
		return;

	countGemsToRemove = 0;
	CObject *obj;
	indexBreakAnim = 0;	

	for (int i = 0; i < MAX_BREAK_ANIMS; i++)
	{
		breakAnim[i] = NULL;
	}
	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			if(removableGems[i][j] == true)
			{
				countGemsToRemove++;
                // Sac: I have extracted the obj creation out of switch case and am adding extra check if the sprite is loaded or not for the below if condition,
                obj = new CObject();
                bool objSpriteLoaded = false;
				switch (gemType[i][j])
				{
					case 0:
						//obj = new CObject();
						objSpriteLoaded=obj->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4RedBreakAnim.SPR");
						break;
					case 1:
						//obj = new CObject();
						objSpriteLoaded=obj->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4SkyBlueBreakAnim.SPR");
						break;
					case 2:
						//obj = new CObject();
						objSpriteLoaded=obj->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4GreenBreakAnim.SPR");
						break;
					case 3:
						//obj = new CObject();
						objSpriteLoaded=obj->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4OrangeBreakAnim.SPR");
						break;
					case 4:
						//obj = new CObject();
						objSpriteLoaded=obj->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4BlueBreakAnim.SPR");
						break;
					case 5:
						//obj = new CObject();
						objSpriteLoaded=obj->LoadSprite("LV3\\S2\\MAZE4\\POP1\\m4PinkBreakAnim.SPR");
						break;
					
				}
				if(obj != NULL && objSpriteLoaded)
				{
					obj->SetZ(eZOrder_Popup + 30);
					obj->Pos.x = board[i][j]->Pos.x - 44;
					obj->Pos.y = board[i][j]->Pos.y - 44;
					obj->ForcePlayAnimation(0);
					obj->Show(true);
					breakAnim[indexBreakAnim] = obj;
					indexBreakAnim++;
					isBreakAnimPlaying = true;
				}
			}
		}
	}
}

void L3_S2_M4Pop1Match3::markRemovableGems()
{
	int typeIndex;
	int x,y;
	int count;
    // int index = 0; Sac: Var unused anywhere... so commenting
	countGemsToRemove = 0;

	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			typeIndex = gemType[i][j];
			count = 1;
			x = i - 1;
			if(x >= 0)
			{
				if(gemType[x][j] == typeIndex)
					count++;
			}
			x = i + 1;
			if(x <= (MAX_MATCH3_ROWS-1))
			{
				if(gemType[x][j] == typeIndex)
					count++;
			}

			if(count == 3)
			{
				removableGems[i - 1][j] = true;
				removableGems[i][j] = true;
				removableGems[i + 1][j] = true;
				isRemovablesMarked = true;
			}
			
			count = 1;
			y = j - 1;
			if(y >= 0)
			{
				if(gemType[i][y] == typeIndex)
					count++;
			}
			y = j + 1;
			if(y <= (MAX_MATCH3_COLS-1))
			{
				if(gemType[i][y] == typeIndex)
					count++;
			}

			if(count == 3)
			{
				removableGems[i][j - 1] = true;
				removableGems[i][j] = true;
				removableGems[i][j + 1] = true;
				isRemovablesMarked = true;
			}
		}
	}
}

bool L3_S2_M4Pop1Match3::isMatch3Found()
{
    //	bool flag = false; Sac: Unused var
	int count = 1;
	int typeIndex = gemType[toX][toY];
	for (int i = toX + 1; i <= (toX + 2); i++)
	{
		if(i < MAX_MATCH3_ROWS)
		{
			if(gemType[i][toY] == typeIndex)
				count++;
		}
	}
	
	if(count == 3) return true;

	count = 1;
	for (int i = toX - 1; i >= (toX - 2); i--)
	{
		if(i >= 0)
		{
			if(gemType[i][toY] == typeIndex)
				count++;
		}
	}
	
	if(count == 3) return true;

	count = 1;
	for (int i = toY + 1; i <= (toY + 2); i++)
	{
		if(i < MAX_MATCH3_COLS)
		{
			if(gemType[toX][i] == typeIndex)
				count++;
		}
	}
	
	if(count == 3) return true;

	count = 1;
	for (int i = toY - 1; i >= (toY - 2); i--)
	{
		if(i >= 0)
		{
			if(gemType[toX][i] == typeIndex)
				count++;
		}
	}
	
	if(count == 3) return true;

	count = 1;
	if(gemType[toX - 1][toY] == typeIndex) count++;
	if(gemType[toX + 1][toY] == typeIndex) count++;

	if(count ==  3) return true;

	count = 1;
	if(gemType[toX][toY + 1] == typeIndex) count++;
	if(gemType[toX][toY - 1] == typeIndex) count++;

	if(count ==  3) return true;

	count = 1;
	typeIndex = gemType[fromX][fromY];
	for (int i = fromX + 1; i <= (fromX + 2); i++)
	{
		if(i < MAX_MATCH3_ROWS)
		{
			if(gemType[i][fromY] == typeIndex)
				count++;
		}
	}

	if(count == 3) return true;

	count = 1;
	for (int i = fromX - 1; i >= (fromX - 2); i--)
	{
		if(i >= 0)
		{
			if(gemType[i][fromY] == typeIndex)
				count++;
		}
	}
	
	if(count == 3) return true;

	count = 1;
	for (int i = fromY + 1; i <= (fromY + 2); i++)
	{
		if(i < MAX_MATCH3_COLS)
		{
			if(gemType[fromX][i] == typeIndex)
				count++;
		}
	}
	
	if(count == 3) return true;

	count = 1;
	for (int i = fromY - 1; i >= (fromY - 2); i--)
	{
		if(i >= 0)
		{
			if(gemType[fromX][i] == typeIndex)
				count++;
		}
	}
	
	if(count == 3) return true;

	count = 1;
	if(gemType[fromX - 1][fromY] == typeIndex) count++;
	if(gemType[fromX + 1][fromY] == typeIndex) count++;

	if(count ==  3) return true;

	count = 1;
	if(gemType[fromX][fromY + 1] == typeIndex) count++;
	if(gemType[fromX][fromY - 1] == typeIndex) count++;

	if(count ==  3) return true;

	return false;
}

void L3_S2_M4Pop1Match3::insertNewGem(int x, int y)
{
	CObject *gem;
    // bool flag = false; Sac: Unused var
	int count = 0;
	for (; ;)
	{
		int index = rand() % MAX_MATCH3_VARIETY;
		
		count = 0;
		for (int i = x - 1; i >= (x - 2)  ; i--)
		{
			if(i >= 0)
			{
				if(gemType[i][y] == index)
					count ++;
			}
		}
		
		if(count >= 2) continue;
		count = 0;
		for (int i = y - 1; i >= (y - 2)  ; i--)
		{
			if(i >= 0)
			{
				if(gemType[x][i] == index)
					count ++;
			}
		}
		
		if(count >= 2) continue;

		gemType[x][y] = index;

		gem = new CObject();
		gem->LoadSprite(strGems[index]);
		gem->SetZ(eZOrder_Popup + 20);
		gem->Pos.x = board[x][y]->Pos.x;
		gem->Pos.y =  board[x][y]->Pos.y;
		gem->SetMaskRect(MaskRect);

		gem->FadeIn(4.0f);

		gems[x][y] = gem;
		return;
	}
}

void L3_S2_M4Pop1Match3::fillDropingGemsArray()
{
	int x;
	int index = 0;
	for (int i = 0; i < MAX_MATCH3_TILES; i++)
	{
		dropingGems[i] = NULL;
	}
	
		for (int i = 0; i < MAX_MATCH3_ROWS; i++)
		{
			for (int j = 0; j < MAX_MATCH3_COLS; j++)
			{
				if(gems[i][j] != NULL)
				{
					x = i + 1;
					if(x <= (MAX_MATCH3_ROWS-1))
					{
						if(gems[x][j] == NULL)
						{
							bool isAlreadyContainDropingList = false;
							for (int k = 0; k < MAX_MATCH3_TILES; k++)
							{
								if(dropingGems[k] != NULL)
								{
									if(dropingGems[k] == gems[i][j])
									{
										//K_LOG ("Already added in dropping list...");
										isAlreadyContainDropingList = true;
									}
								}
							}
							if(isAlreadyContainDropingList == false)
							{
								dropingGems[index] = gems[i][j];
								//mask the gems
								dropingGemsStartPos[index] = board[i][j]->Pos;
								dropingGemsEndPos[index] = board[x][j]->Pos;
								index++;

								gems[x][j] = gems[i][j];
								gems[i][j] = NULL;
								gemType[x][j] = gemType[i][j];
								gemType[i][j] = -1;
							}
						}
					}
				}
			}
		}
	

	for (int y = 0; y < MAX_MATCH3_COLS; y++)
	{
		if(gems[0][y] == NULL)
		{
			insertNewGem(0,y);
			dropingGems[index] = gems[0][y];
			gems[0][y]->Pos.y =  board[0][y]->Pos.y - 60;
			CPoint startPos;
			startPos.x = board[0][y]->Pos.x ;
			startPos.y = board[0][y]->Pos.y - 60;

			dropingGemsStartPos[index] =  startPos;
			dropingGemsEndPos[index] = board[0][y]->Pos;
			index++;

		}
	}
}

void L3_S2_M4Pop1Match3::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(isAnimationPlaying || ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened )
	{
		Cursor::SetMode(CA_Normal);
	//	return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	//handle interactions here
	if(isPuzzleSolved)
	{
		tweenTime += Control::LogicRate;

		if(tweenTime >= 2.5f)
		{
			ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened = true;

			s2m4match3endFX->StopEmit();
			isAnimationPlaying = false;
			//Close Popup
			
			ClosePopup();		
			
		}
		return;
	}
	else
	{
		if(isBreakAnimPlaying)
		{
			if(breakAnim[0]->IsAnimationFinished())
			{
				for (int i = 0; i < MAX_BREAK_ANIMS; i++)
				{
					if(breakAnim[i] != NULL)
					{
						breakAnim[i]->Show(false);
						SAFE_DELETE(breakAnim[i]);
						indexBreakAnim = 0;
						isBreakAnimPlaying = false;
					}
				}
			}
			return;
		}

		if(isSliding)
		{
			//K_LOG ("IS SLIDING STARTED.....");
			int isAllSlidingComplete = true;
			for (int i = 0; i < MAX_MATCH3_SLIDERS; i++)
			{
				if(openingSliders[i] != NULL)
				{
					isAllSlidingComplete = false;
					timerSliders[i] += Control::LogicRate;
					openingSliders[i]->Pos.x = openingSlidersFromXPos[i] + (openingSlidersToXPos[i] - openingSlidersFromXPos[i]) * easing(timerSliders[i],0,1,dropTime);

					if(timerSliders[i] >= dropTime)
					{
						openingSliders[i] = NULL;
					}
				}

				if(m4SliderOpenAnim[i] != NULL)
				{
					if(m4SliderOpenAnim[i]->IsAnimationFinished())
					{
						m4SliderOpenAnim[i]->Show(false);
						SAFE_DELETE(m4SliderOpenAnim[i]);
						m4SliderOpenAnim[i] = NULL;
					}
				}
			}

			if(isAllSlidingComplete)
			{
				//K_LOG ("All puzzles Completed...");
                //	bool flag = true; Sac: Unused Var
				int nullCount = 0;
				for (int i = 0; i < MAX_MATCH3_LEFTSLIDERS; i++)
				{
					if(leftSlider[i]->IsActive == false)
					{
						nullCount ++;
					}
				}
				for (int i = 0; i < MAX_MATCH3_RIGHTSLIDERS; i++)
				{
					if(rightSlider[i]->IsActive == false)
					{
						nullCount ++;
					}
				}

				if(nullCount >= MAX_MATCH3_SLIDERS)
				{
					//K_LOG ("Puzzle solved.....");
					tweenTime = 0.0f;
					isPuzzleSolved = true;
					Control::Audio->QuickLoadAndPlaySFX("s2m4_puzzlecomplete");
					isSliding = false;
					isAnimationPlaying = true;
					s2m4match3endFX->Start();

					Hud->InvBox_StopPuzzleMode();
					return;
				}
			}
		}

		if(isGemsDroping)
		{
			tweenTime += Control::LogicRate;
			for (int i = 0; dropingGems[i] != NULL; i++)
			{
				CPoint temp;
				temp.x = dropingGemsStartPos[i].x + (dropingGemsEndPos[i].x - dropingGemsStartPos[i].x) * easing(tweenTime,0,1,dropTime);
				temp.y = dropingGemsStartPos[i].y + (dropingGemsEndPos[i].y - dropingGemsStartPos[i].y) * easing(tweenTime,0,1,dropTime);

				dropingGems[i]->Pos = temp;

				if( tweenTime >= dropTime )
				{
					dropingGems[i]->Pos = dropingGemsEndPos[i];
				}		
			}

			if( tweenTime >= dropTime )
			{
			//	bool isGemFull = true;  // Sac: Commenting Unused variable

				for (int i = 0; i < MAX_MATCH3_ROWS; i++)
				{
					for (int j = 0; j < MAX_MATCH3_COLS; j++)
					{
						if(gems[i][j] == NULL)
						{
							fillDropingGemsArray();
							tweenTime = 0.0f;// edit here
							return;
						}
					}
				}
				isRemovablesMarked = false;
				markRemovableGems();
				if(isRemovablesMarked)
				{
					ShowGemBreakAnim();
					PlaySuccessSFX();
					deleteMarkedGems();
					fillDropingGemsArray();
					isGemsDroping = true;
					tweenTime = 0.0f;
					return;
				}
				isGemsDroping = false;
			}
			return;
		}

		if(isTileSwaping)
		{
			//swap
			tweenTime += Control::LogicRate;
			CPoint temp, temp2;

			temp.x = board[fromX][fromY]->Pos.x + (board[toX][toY]->Pos.x - board[fromX][fromY]->Pos.x) * easing(tweenTime,0,1,swapTime);
			temp.y = board[fromX][fromY]->Pos.y + (board[toX][toY]->Pos.y - board[fromX][fromY]->Pos.y) * easing(tweenTime,0,1,swapTime);

			gems[fromX][fromY]->Pos = temp;

			temp2.x = board[toX][toY]->Pos.x + (board[fromX][fromY]->Pos.x - board[toX][toY]->Pos.x) * easing(tweenTime,0,1,swapTime);
			temp2.y = board[toX][toY]->Pos.y + (board[fromX][fromY]->Pos.y - board[toX][toY]->Pos.y) * easing(tweenTime,0,1,swapTime);

			gems[toX][toY]->Pos = temp2;

			if( tweenTime >= swapTime )
			{
				gems[toX][toY]->Pos = board[fromX][fromY]->Pos;
				gems[fromX][fromY]->Pos = board[toX][toY]->Pos;

				CObject *temp = gems[toX][toY];
				gems[toX][toY] = gems[fromX][fromY];
				gems[fromX][fromY] = temp;

				int tempType = gemType[toX][toY];
				gemType[toX][toY] = gemType[fromX][fromY];
				gemType[fromX][fromY] = tempType;

				tweenTime = 0;
				isTileSwaping = false;
				gemSel[0]->SetEnable(false);
				gemSel[1]->SetEnable(false);

				selIndex = 0;
				for (int i = 0; i < MAX_MATCH3_ROWS; i++)
				{
					for (int j = 0; j < MAX_MATCH3_COLS; j++)
					{
						isSelected[i][j] = false;
					}
				}

				//Checking for match 3 tiles, if not reverse swap

				if(!isMatch3Found() && isReverseSwaping == false)
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXMatch3WrongClick);
					//swap code
					tweenTime = 0.0f;
					isTileSwaping = true;
					isReverseSwaping = true;
					return;
				}

				if(isMatch3Found())
				{
					markRemovableGems();
					ShowGemBreakAnim();
					PlaySuccessSFX();
					deleteMarkedGems();
					fillDropingGemsArray();
					isGemsDroping = true;
					tweenTime = 0.0f;
					return;
				}
			}
			return;
		}

		if( !isTileSwaping && !isGemsDroping )
		{
			CRectangle rect = new CRectangle(0,0,0,0);
			for (int i = 0; i < MAX_MATCH3_ROWS; i++)
			{
				for (int j = 0; j < MAX_MATCH3_COLS; j++)
				{
					rect.topleft.x = board[i][j]->Pos.x - 30;
					rect.topleft.y =  board[i][j]->Pos.y - 30;
					rect.w = 60;
					rect.h = 60;
					if(rect.Intersection(&pos))
					{
						Cursor::SetMode(CA_HandPointy);
						m4MouseOver->SetEnable(true);
						m4MouseOver->Pos.x = board[i][j]->Pos.x;
						m4MouseOver->Pos.y = board[i][j]->Pos.y;

						if( Control::Input->LBclicked())
						{
							if(isSelected[i][j] == false && selIndex == 0)
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXMatch3Click);

								gemSel[selIndex]->SetEnable(true);
								gemSel[selIndex]->Pos.x = board[i][j]->Pos.x;
								gemSel[selIndex]->Pos.y = board[i][j]->Pos.y;
								isSelected[i][j] = true;

								selX = i;
								selY = j;
								selIndex++;
							}
							else if(isSelected[i][j] == true && selIndex == 1)
							{
								gemSel[selIndex]->SetEnable(false);
								isSelected[i][j] = false;
								return;
							}
							else if(selIndex == 1 && isSelected[i][j] == false)
							{
								if(abs(selX - i) > 1  || abs(selY - j) > 1  || (selX != i && selY != j))
								{
									Control::Audio->QuickLoadAndPlaySFX(aSFXMatch3Click);

									selIndex = 0;
									gemSel[selIndex]->Pos.x = board[i][j]->Pos.x;
									gemSel[selIndex]->Pos.y = board[i][j]->Pos.y;
									isSelected[selX][selY] = false;

									isSelected[i][j] = true;

									selX = i;
									selY = j;
									selIndex++;
								}
								else
								{
									Control::Audio->QuickLoadAndPlaySFX(aSFXMatch3Click);

									gemSel[selIndex]->SetEnable(true);
									gemSel[selIndex]->Pos.x = board[i][j]->Pos.x;
									gemSel[selIndex]->Pos.y = board[i][j]->Pos.y;
									isSelected[i][j] = true;
									fromX = selX;
									fromY = selY;
									toX = i;
									toY = j;
									tweenTime = 0.0f;
									isTileSwaping = true;
									isReverseSwaping = false;
									m4MouseOver->SetEnable(false);
									//check to swap the tile and reverse or remove

									return;
								}
							}
						}

						return;
					}
					else
					{
						m4MouseOver->SetEnable(false);
					}
				}
			}
		}
	}
}

void L3_S2_M4Pop1Match3::PlaySuccessSFX()
{
	if(countGemsToRemove == 3)
		Control::Audio->QuickLoadAndPlaySFX(aSFXmatch3Good);
	else if(countGemsToRemove == 4)
		Control::Audio->QuickLoadAndPlaySFX(aSFXmatch3Better);
	else 
		Control::Audio->QuickLoadAndPlaySFX(aSFXmatch3Best);
}

float L3_S2_M4Pop1Match3::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L3_S2_M4Pop1Match3::OnPopupOpened()
{
	InitPuzzle();
}

void L3_S2_M4Pop1Match3::DisposeObjects()
{
	//clear match3 elements
	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			SAFE_DELETE(board[i][j]);
			SAFE_DELETE(gems[i][j]);
		}
	}

	for (int i = 0; i < MAX_BREAK_ANIMS; i++)
	{
		SAFE_DELETE(breakAnim[i]);
	}

	for (int i = 0; i < MAX_MATCH3_SLIDERS; i++)
	{
		SAFE_DELETE(m4SliderOpenAnim[i]);
		openingSliders[i] = NULL;
	}
	for (int i = 0; i < MAX_MATCH3_TILES; i++)
	{
		dropingGems[i] = NULL;
	}

	SAFE_DELETE(m4MouseOver);

	SAFE_DELETE(gemSel[0]);
	SAFE_DELETE(gemSel[1]);
	
}

void L3_S2_M4Pop1Match3::OnPopupCloseBtnClicked()
{
	DisposeObjects();

	Hud->InvBox_StopPuzzleMode();
}

void L3_S2_M4Pop1Match3::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L3_S2_M4Pop1Match3::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened)
	{
		SkipPuzzle();
		//Hud->InitHintTrailEmit(&MaskRect, true);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3_S2_M4Pop1Match3::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_M4DoorsOpened)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L3_S2_M4Pop1Match3::~L3_S2_M4Pop1Match3()
{
	SAFE_DELETE(s2m4match3endFX);

	if(GetPopupState() == ePopupState_Open)
	{
		DisposeObjects();
	}
}
