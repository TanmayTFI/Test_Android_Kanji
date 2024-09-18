//====================================
//	DQFYLH
//	HO System
//	MATCH3
//====================================

#include "Match3.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Fonts.h"
#include "AwardManager.h"

const int HO_SHEDROOM		= 1;
const int HO_METALDOORFOYER = 2;
const int HO_TREASUREROOM	= 3;
const int HO_NAGATEMPLE		= 4;
const int HO_TUNNELWATERHO	= 5;
const int HO_LADDER			= 6;
const int HO_FORTFOUNTAIN	= 7;
const int HO_PONDUNDERWATER	= 8;
const int HO_UNDERGROUND	= 9;
const int HO_GLOWINGHEART	= 10;
const int HO_SHOE			= 11;
const int HO_DESCRIPTION	= 12;
const int HO_SPHEREJIGSAW	= 13;
const int HO_L6TREASURE		= 14;
const int HO_L5GANAPATHI	= 15;
const int HO_L8DOOR			= 16;
const int HO_L8LEAF			= 17;

//DEADLOCKTICKS
#define MAX_DEADLOCK_TICKS 48
#define TILE_OFFSET 73

Match3::Match3()
{
}

Match3::Match3(int levelId)
{
	_isCompleted = false;
	LevelId = levelId;
	DeadLockTicks = MAX_DEADLOCK_TICKS;//DEADLOCKTICKS
	
	tweenTime = 0.0f;
	swapTime = 0.70f;	

	GFInstance->PostUpdate(2);

	Match3Scene = new CHOScene();

	if(levelId == HO_SHEDROOM)
	{
		Match3Scene->Load("MATCH3\\SHEDROOM\\MATCH3.SCN");
		path = "MATCH3\\SHEDROOM\\";
	}
	else if(levelId == HO_METALDOORFOYER)
	{
		Match3Scene->Load("MATCH3\\BEDROOM\\MATCH3.SCN");
		path = "MATCH3\\BEDROOM\\";
	}
	else if(levelId == HO_TREASUREROOM)
	{
		Match3Scene->Load("MATCH3\\TREASUREROOM\\MATCH3.SCN");
		path = "MATCH3\\TREASUREROOM\\";
	}
	else if(levelId == HO_NAGATEMPLE)
	{
		Match3Scene->Load("MATCH3\\L2NAGATEMPLE\\MATCH3.SCN");
		path = "MATCH3\\L2NAGATEMPLE\\";
	}
	else if(levelId == HO_TUNNELWATERHO)
	{
		Match3Scene->Load("MATCH3\\L2WATERHO\\MATCH3.SCN");
		path = "MATCH3\\L2WATERHO\\";
	}
	else if(levelId == HO_LADDER)
	{
		Match3Scene->Load("MATCH3\\L3LADDERHO\\MATCH3.SCN");
		path = "MATCH3\\L3LADDERHO\\";
	}
	else if(levelId == HO_FORTFOUNTAIN)
	{
		Match3Scene->Load("MATCH3\\L3FOUNTAINHO\\MATCH3.SCN");
		path = "MATCH3\\L3FOUNTAINHO\\";
	}
	else if(levelId == HO_PONDUNDERWATER)
	{
		Match3Scene->Load("MATCH3\\L5UNDERWATERHO\\MATCH3.SCN");
		path = "MATCH3\\L5UNDERWATERHO\\";
	}
	else if(levelId == HO_L5GANAPATHI)
	{
		Match3Scene->Load("MATCH3\\L5GANAPATHI\\MATCH3.SCN");
		path = "MATCH3\\L5GANAPATHI\\";
	}
	else if(levelId == HO_UNDERGROUND)
	{
		Match3Scene->Load("MATCH3\\L2UNDERGROUNDHO\\MATCH3.SCN");
		path = "MATCH3\\L2UNDERGROUNDHO\\";
	}
	else if(levelId == HO_GLOWINGHEART)//Not used!!
	{
		Match3Scene->Load("MATCH3\\L5UNDERWATERHO\\MATCH3.SCN");//redundant
		path = "MATCH3\\L5UNDERWATERHO\\";//redundant
	}
	else if(levelId == HO_SHOE)
	{
		Match3Scene->Load("MATCH3\\L4SHOEHO\\MATCH3.SCN");
		path = "MATCH3\\L4SHOEHO\\";
	}
	else if(levelId == HO_DESCRIPTION)
	{
		Match3Scene->Load("MATCH3\\L5UNDERWATERHO\\MATCH3.SCN");
		path = "MATCH3\\L5UNDERWATERHO\\";
	}
	else if(levelId == HO_SPHEREJIGSAW)
	{
		Match3Scene->Load("MATCH3\\L6SPHEREHO\\MATCH3.SCN");
		path = "MATCH3\\L6SPHEREHO\\";
	}
	else if(levelId == HO_L6TREASURE)
	{
		Match3Scene->Load("MATCH3\\L6TREASURE\\MATCH3.SCN");
		path = "MATCH3\\L6TREASURE\\";
	}
	else if(levelId == HO_L8DOOR)
	{
		Match3Scene->Load("MATCH3\\L8DOORHO\\MATCH3.SCN");
		path = "MATCH3\\L8DOORHO\\";
	}
	else if(levelId == HO_L8LEAF)
	{
		Match3Scene->Load("MATCH3\\L8LEAFHO\\MATCH3.SCN");
		path = "MATCH3\\L8LEAFHO\\";
	}
	else
	{
		#ifdef K_WIN32
			FATAL(StringTable::getString("ERR_ScnID")); ////Removed Wide string conversion for Windows	
		#else
			FATAL(StringTable::getString("ERR_ScnID"));
		#endif
	}

	txtInfo = NULL;
	
	CPoint pos;
	txtInfo = new CBitmapText();
	txtInfo->LoadFont(eFont_26);
	txtInfo->SetAlignment(Align_Center);
    pos = CPoint(683,716.0f,0);
	txtInfo->SetPos(&pos);
	txtInfo->SetScale(1.0f);
	txtInfo->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 
	txtInfo->SetZ(eZOrder_INVBOXITEMS);
	txtInfo->Show(true);
	txtInfo->SetText(StringTable::getStr("Exrem"));

	Match3Scene->SetZOff(eZOrder_PopupOverPopup);

	m3Image1	= Match3Scene->GetObjectByID("m3Image1");
	m3Image1->Show(false);
	m3Image1->SetEnable(false );

	m3Mask = Match3Scene->GetObjectByID("m3Mask");
	m3Mask->SetZ(eZOrder_PopupOverPopup+100);

	strGems[0] = path + "M3IMAGE1.SPR";
	strGems[1] = path + "M3IMAGE2.SPR";
	strGems[2] = path + "M3IMAGE3.SPR";
	strGems[3] = path + "M3IMAGE4.SPR";
	strGems[4] = path + "M3IMAGE5.SPR";

	InitPuzzle();

	PrevRandomId = rand()%4;
}

void Match3::Required()
{

}

void Match3::deleteMarkedGems()
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
				//Removing ice gems
				if(iceGems[i][j] != NULL)
				{
					iceGems[i][j]->Show(false);
					iceGems[i][j] = NULL;
				}
			}
		}
	}
	
	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			removableGems[i][j] = false;
		}
	}
}

void Match3::ShowGemBreakAnim()
{
	countGemsToRemove = 0;
	CObject *obj;
	indexBreakAnim = 0;

	for (int i = 0; i < MAX_MATCH3_BREAKS; i++)
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
				obj = new CObject();
				bool objSpriteLoaded = false;
				switch (gemType[i][j])
				{
					case 0:
						//obj = new CObject();
						objSpriteLoaded = obj->LoadSprite("MATCH3\\M4ORANGEBREAKANIM.SPR");
						break;
					case 1:
						//obj = new CObject();
						objSpriteLoaded = obj->LoadSprite("MATCH3\\M4PINKBREAKANIM.SPR");
						break;
					case 2:
						//obj = new CObject();
						objSpriteLoaded = obj->LoadSprite("MATCH3\\M4GREENBREAKANIM.SPR");
						break;
					case 3:
						//obj = new CObject();
						objSpriteLoaded = obj->LoadSprite("MATCH3\\M4REDBREAKANIM.SPR");
						break;
					case 4:
						//obj = new CObject();
						objSpriteLoaded = obj->LoadSprite("MATCH3\\M4SKYBLUEBREAKANIM.SPR");
						break;
				}
				if(obj != NULL && objSpriteLoaded)
				{
					obj->SetZ(eZOrder_PopupOverPopup + 30);
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

void Match3::markRemovableGems()
{
	int typeIndex;
	int x,y;
	int count;
//	int index = 0;  //Sac: Unused variable, so commented it
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
			if(x < MAX_MATCH3_ROWS)
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
			if(y < MAX_MATCH3_COLS)
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

bool Match3::isMatch3Found()
{
//	bool flag = false;  // Sac: Commenting Unused variable
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


void Match3::InitPuzzle()
{
	DeadLockTicks = MAX_DEADLOCK_TICKS;//DEADLOCKTICKS
	m3Image1->Show(false);
	m3Image1->SetEnable(false);
	float startX = m3Image1->Pos.x;
	float startY = m3Image1->Pos.y;

	float xPos = startX;
	float yPos = startY;
	//setting board tiles
	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		xPos = startX;
		yPos = startY + ( i * TILE_OFFSET);
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			gemType[i][j] = -1;
			board[i][j] = new CObject();
			board[i][j]->LoadSprite(path + "m3Image1.SPR");
			board[i][j]->SetZ(eZOrder_PopupOverPopup + 20);
			xPos = startX + ( j * TILE_OFFSET);

			board[i][j]->Pos.x = xPos;
			board[i][j]->Pos.y = yPos;
			board[i][j]->Show(false);

		}
	}

	m3MouseOver = new CObject();
	m3MouseOver->LoadSprite(path + "m3MouseOver.SPR");
	m3MouseOver->SetZ(eZOrder_PopupOverPopup + 22);
	
	m3MouseOver->Show(false);

	gemSel[0] = new CObject();
	gemSel[0]->LoadSprite(path + "m3GemSel.SPR");
	gemSel[0]->SetZ(eZOrder_PopupOverPopup + 22);
	
	gemSel[0]->Show(false);

	gemSel[1] = new CObject();
	gemSel[1]->LoadSprite(path + "m3GemSel.SPR");
	gemSel[1]->SetZ(eZOrder_PopupOverPopup + 22);
	
	gemSel[1]->Show(false);

	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			insertNewGem(i,j,(rand()%MAX_MATCH3_VARIETY));
			isSelected[i][j] = false;
			removableGems[i][j] = false;
		}
	}

	//START : Fill Ice Gems;
	int noOfIceGemsRequred = 35;
	int iceGemsCount = 0;

	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			iceGems[i][j] = NULL;
		}
	}

	for (; ;)
	{
		int x = rand() % MAX_MATCH3_ROWS;
		if( x == MAX_MATCH3_ROWS ) x--;
		int y = rand() % MAX_MATCH3_COLS;
		if( y == MAX_MATCH3_COLS ) y--;

		if(iceGems[x][y] == NULL)
		{
			iceGems[x][y] = new CObject();
			iceGems[x][y]->LoadSprite(path + "m3GemToRemove.SPR");
			iceGems[x][y]->SetZ(eZOrder_PopupOverPopup + 19);
			iceGems[x][y]->Pos = board[x][y]->Pos;
			iceGemsCount++;
		}

		if(iceGemsCount >= noOfIceGemsRequred)
		{
			break;
		}
	}
	//END : Fill Ice Gems;

	selIndex = 0;
	isTileSwaping = false;
	swapTime = 0.3f;
	dropTime = 0.2f;
	isGemsDroping = false;
	isRemovablesMarked = false;

	selIndex = 0;
	isPuzzleSolved = false;
	isBreakAnimPlaying = false;
	indexBreakAnim = 0;
	isGemsDroping = false;
	isBreakAnimPlaying = false;

	RemoveMatches();
}

void Match3::fillDropingGemsArray()
{
	int x;
	int index = 0;
	for (int i = 0; i < MAX_MATCH3_BREAKS; i++)
	{
		dropingGems[i] = NULL;
	}
	for (int c = 0; c  < MAX_MATCH3_ROWS; c ++)
	{
		for (int i = 0; i < MAX_MATCH3_ROWS; i++)
		{
			for (int j = 0; j < MAX_MATCH3_COLS; j++)
			{
				if(gems[i][j] != NULL)
				{
					x = i + 1;
					if(x < MAX_MATCH3_ROWS)
					{
						if(gems[x][j] == NULL)
						{
							bool isAlreadyContainDropingList = false;
							for (int k = 0; k < MAX_MATCH3_BREAKS; k++)
							{
								if(dropingGems[k] != NULL)
								{
									if(dropingGems[k] == gems[i][j])
									{
										isAlreadyContainDropingList = true;
									}
								}
							}
							if(isAlreadyContainDropingList == false)
							{
								dropingGems[index] = gems[i][j];
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
	}

	for (int y = 0; y < MAX_MATCH3_COLS; y++)
	{
		if(gems[0][y] == NULL)
		{
			insertNewGem(0,y,(rand()%MAX_MATCH3_VARIETY));
			dropingGems[index] = gems[0][y];
			gems[0][y]->Pos.y =  board[0][y]->Pos.y - 100;
			CPoint startPos;
			startPos.x = board[0][y]->Pos.x ;
			startPos.y = board[0][y]->Pos.y - 100;

			dropingGemsStartPos[index] =  startPos;
			dropingGemsEndPos[index] = board[0][y]->Pos;
			index++;
		}
	}
}
bool Match3::isGemAnimPlaying()
{
	if(isGemsDroping == true || isBreakAnimPlaying == true) return true;

	return false;
}

void Match3::Update()
{
	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing == false) return;
	CPoint temp;
	CPoint temp2;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

#ifdef _CHEATS
	if(Control::Input->Kpressed(K_VK_SPACE))
	{
		isPuzzleSolved = true;
		Control::Audio->QuickLoadAndPlaySFX(aSFXmatch3Reward);
	}
#endif // _CHEATS

	if(isPuzzleSolved)
	{
		tweenTime += Control::LogicRate;

		if(tweenTime >= 3.75f)
		{
			_isCompleted = true;
			AwardManager::GetInstance()->CompleteHOWithMatch3(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			clearBoard();
			if(LevelId ==  HO_SHEDROOM)
			{
				ProfileSystem::Instance->ActiveProfile.L1S6_HOCompletedWithMatch3 = true;
			}
			else if(LevelId ==  HO_METALDOORFOYER)
			{
				ProfileSystem::Instance->ActiveProfile.L1S3_HOCompletedWithMatch3 = true;
			}
			else if(LevelId ==  HO_TREASUREROOM)
			{
				ProfileSystem::Instance->ActiveProfile.L1S5_HOCompletedWithMatch3 = true;
			}
			else if(LevelId ==  HO_NAGATEMPLE)
			{
				ProfileSystem::Instance->ActiveProfile.L2S2_HONagaCompletedWithMatch3 = true;
			}
			else if(LevelId ==  HO_TUNNELWATERHO)
			{
				ProfileSystem::Instance->ActiveProfile.L2S6_HOWaterFallCompletedWithMatch3 = true;
			}
			else if(LevelId ==  HO_LADDER)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompletedWithMatch3 = true;
			}
			else if(LevelId ==  HO_FORTFOUNTAIN)
			{
				//handled with isCompleted flag
				//ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompletedWithMatch3 = true;
			}
		}
		return;
	}

	if(isBreakAnimPlaying)
	{
		if(breakAnim[0]->IsAnimationFinished())
		{
			for (int i = 0; i < MAX_MATCH3_BREAKS; i++)
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

	if(isGemsDroping)
	{
		tweenTime += Control::LogicRate;
		for (int i = 0; dropingGems[i] != NULL; i++)
		{
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
			// bool isGemFull = true;  // Sac: Commenting Unused variable

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

			//Checking all ice gems removed or not. If removed puzzle over
			bool isAllIceGemsRemoved = true;
			for (int i = 0; i < MAX_MATCH3_ROWS; i++)
			{
				for (int j = 0; j < MAX_MATCH3_COLS; j++)
				{
					if(iceGems[i][j] != NULL)
					{
						isAllIceGemsRemoved = false;
						break;
					}
				}
			}

			if(isAllIceGemsRemoved == true)
			{
				isPuzzleSolved = true;
				Control::Audio->QuickLoadAndPlaySFX(aSFXmatch3Reward);
			}

			isGemsDroping = false;
		}
		return;
	}

	if(isTileSwaping)
	{
		//swap
		tweenTime += Control::LogicRate;

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
			gemSel[0]->Show(false);
			gemSel[1]->Show(false);

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
				DeadLockTicks = MAX_DEADLOCK_TICKS;//DEADLOCKTICKS
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

	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			CRectangle rect(board[i][j]->Pos.x - 35, board[i][j]->Pos.y - 35, 70, 70);
			if(rect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				m3MouseOver->Show(true);
				m3MouseOver->Pos.x = board[i][j]->Pos.x;
				m3MouseOver->Pos.y = board[i][j]->Pos.y;

				if( Control::Input->LBclicked())
				{
					if(isSelected[i][j] == false && selIndex == 0)
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXMatch3Click);

						gemSel[selIndex]->Show(true);
						gemSel[selIndex]->Pos.x = board[i][j]->Pos.x;
						gemSel[selIndex]->Pos.y = board[i][j]->Pos.y;
						isSelected[i][j] = true;

						selX = i;
						selY = j;
						selIndex++;
					}
					else if(isSelected[i][j] == true && selIndex == 1)
					{
						gemSel[selIndex]->Show(false);
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

							gemSel[selIndex]->Show(true);
							gemSel[selIndex]->Pos.x = board[i][j]->Pos.x;
							gemSel[selIndex]->Pos.y = board[i][j]->Pos.y;
							isSelected[i][j] = true;
							fromX = selX;
							fromY = selY;
							toX = i;
							toY = j;
							tweenTime = 0.0f;
							isTileSwaping = true;
							DeadLockTicks = MAX_DEADLOCK_TICKS;//DEADLOCKTICKS
							isReverseSwaping = false;
							m3MouseOver->Show(false);
							//check to swap the tile and reverse or remove

							return;
						}
					}
				}

				return;
			}
			else
			{
				m3MouseOver->Show(false);
			}
		}
	}

	//DEADLOCKTICKS - if no scene activity for some time, it's time to check if deadlock!!
	if( DeadLockTicks )
		DeadLockTicks--;
	else
	{
		DeadLockTicks = MAX_DEADLOCK_TICKS;	
		HandleDeadLock();
	}

}


void Match3::ShowBoard()
{
	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			board[i][j]->Show(false);
			if(gems[i][j] != NULL)
				gems[i][j]->Show(true);
		}
	}
					
	for (int i = 0; i < MAX_MATCH3_BREAKS; i++)
	{
		dropingGems[i] = NULL;
	}

	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			if(iceGems[i][j] != NULL)
			{
				iceGems[i][j]->Show(true);
			}
		}
	}

	Match3Scene->Show(true);
	//m3Mask->Show(true);
	txtInfo->Show(true);
}
void Match3::clearBoard()
{
	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			board[i][j]->Show(false);
			if(gems[i][j] != NULL)
				gems[i][j]->Show(false);
			//SAFE_DELETE(board[i][j]);
			//SAFE_DELETE(gems[i][j]);
		}
	}
					
	for (int i = 0; i < MAX_MATCH3_BREAKS; i++)
	{
		dropingGems[i] = NULL;
	}

	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	m3MouseOver->Show(false);
	//SAFE_DELETE(m3MouseOver);
	gemSel[0]->Show(false);
	gemSel[1]->Show(false);
	//SAFE_DELETE(gemSel[0]);
	//SAFE_DELETE(gemSel[1]);

	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			if(iceGems[i][j] != NULL)
			{
				iceGems[i][j]->Show(false);
			}
		}
	}

	Match3Scene->Show(false);
	//m3Mask->Show(false);
	txtInfo->Show(false);
}

void Match3::PlaySuccessSFX()
{
	//K_LOG("count of break gems : %d", countGemsToRemove); 
	if(countGemsToRemove == 3)
		Control::Audio->QuickLoadAndPlaySFX(aSFXmatch3Good);
	else if(countGemsToRemove == 4)
		Control::Audio->QuickLoadAndPlaySFX(aSFXmatch3Better);
	else 
		Control::Audio->QuickLoadAndPlaySFX(aSFXmatch3Best);
	/*switch (PrevRandomId)
	{
	case 0:		Control::Audio->QuickLoadAndPlaySFX(aSFXMatch3Win1);		break;
	case 1:		Control::Audio->QuickLoadAndPlaySFX(aSFXMatch3Win2);		break;
	case 2:		Control::Audio->QuickLoadAndPlaySFX(aSFXMatch3Win3);		break;
	case 3:		Control::Audio->QuickLoadAndPlaySFX(aSFXMatch3Win4);		break;
	default:
		break;
	}

	int randId = rand()%4;	

	if(randId == PrevRandomId)
		randId+=1;
	
	if(randId>3)
		randId = 0;

	PrevRandomId = randId;*/
}

float Match3::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

Match3::~Match3()
{	
	DeleteMatch3Board();
	DisposeAssets();
}

void Match3::DisposeAssets()
{	
	SAFE_DELETE(Match3Scene);
	SAFE_DELETE(txtInfo);
}

void Match3::ReceiveMessage(int val)
{	
	
}

//DEADLOCKTICKS
void Match3::HandleDeadLock()
{
	// Check if triplets present ++
	for( int iRow = 0; iRow < MAX_MATCH3_ROWS; iRow++ )
		for( int iCol = 0; iCol < MAX_MATCH3_COLS; iCol++ )
		{
			    int TileId = gemType[iRow][iCol];
				//fixed x,y
				// Is there a x,x+1 hrizontal match!
				if( IsTileIdPresent(iRow, iCol+1, TileId ))
				{
					if( IsTileIdPresent(iRow, iCol+3, TileId) )
					{
						return;
					}
					if( IsTileIdPresent(iRow-1, iCol+2, TileId) )
					{
						return;
					}
					if( IsTileIdPresent(iRow+1, iCol+2, TileId) )
					{
						return;
					}
				}		
				// Is there a x,x+2 hrizontal match!
				if( IsTileIdPresent(iRow, iCol+2, TileId) ) 
				{
					if( IsTileIdPresent(iRow-1, iCol+1, TileId) )
					{
						return;
					}
					if( IsTileIdPresent(iRow+1, iCol+1, TileId) )
					{
						return;
					}
				}
				
				// Is there a y,y+1 vertical match!
				if( IsTileIdPresent(iRow+1, iCol, TileId) )
				{
					if( IsTileIdPresent(iRow+3, iCol, TileId) )
					{
						return;
					}
					if( IsTileIdPresent(iRow+2, iCol-1, TileId) )
					{
						return;
					}
					if( IsTileIdPresent(iRow+2, iCol+1, TileId) )
					{
						return;
					}
				}
				// Is there a y,y+2 vertical match!
				if( IsTileIdPresent(iRow+2, iCol, TileId) ) 
				{
					if( IsTileIdPresent(iRow+1, iCol-1, TileId) )
					{
						return;
					}
					if( IsTileIdPresent(iRow+1, iCol+1, TileId) )
					{
						return;
					}
				}					
			//right x,x+1 swap 
				//horizontal
				if( IsTileIdPresent(iRow, iCol+2, TileId) &&
					IsTileIdPresent(iRow, iCol+3, TileId) )
				{
					return;
				}
				//vertical  [rightside]
				if( IsTileIdPresent(iRow+1, iCol+1, TileId) &&
					IsTileIdPresent(iRow+2, iCol+1, TileId) )
				{
					return;
				}
				//vertical [leftside]
				if( IsTileIdPresent(iRow+1, iCol-1, TileId) &&
					IsTileIdPresent(iRow+2, iCol-1, TileId) )
				{
					return;
				}
			//bottom y,y+1 swap 
				//vertcical
				if( IsTileIdPresent(iRow+2, iCol, TileId) &&
					IsTileIdPresent(iRow+3, iCol, TileId) )
				{
					return;
				}
				//horizontal [downside]
				if( IsTileIdPresent(iRow+1, iCol+1, TileId) &&
					IsTileIdPresent(iRow+1, iCol+2, TileId) )
				{
					return;
				}
				//horizontal [upside]
				if( IsTileIdPresent(iRow-1, iCol+1, TileId) &&
					IsTileIdPresent(iRow-1, iCol+2, TileId) )
				{
					return;
				}
		}	
	
		DeleteMatch3Board();		
		Control::Audio->PlaySample(aSFXFolly);
		InitPuzzle();//Init Game Again to avoid deadlock situation!!				
}

//DEADLOCKTICKS
void Match3::DeleteMatch3Board()
{	
	for (int i = 0; i < MAX_MATCH3_ROWS; i++)
	{
		for (int j = 0; j < MAX_MATCH3_COLS; j++)
		{
			if( board[i][j] != NULL )
			{
				delete board[i][j];						
				board[i][j] = NULL;	
			}
			if( gems[i][j] != NULL )
			{
				delete gems[i][j];						
				gems[i][j] = NULL;	
			}
			if(iceGems[i][j] != NULL)
			{
				delete iceGems[i][j]; 
   				iceGems[i][j] = NULL;
			}
		}
	}
	delete m3MouseOver;
	m3MouseOver = NULL;
	delete gemSel[0]; gemSel[0] = NULL;  
	delete gemSel[1]; gemSel[1] = NULL;  
}
//DEADLOCKTICKS
bool Match3::IsTileIdPresent( int iRow, int iCol, int TileId )
{
	if( iRow < 0 || iCol < 0 || iRow > (MAX_MATCH3_ROWS-1) || iCol > (MAX_MATCH3_COLS-1) )
		return false;
	return	( gemType[iRow][iCol] == TileId );
}




void Match3::insertNewGem(int x, int y, int Type)
{	
	//DEADLOCKTICKS - deadlock testing pruposes	
		
	gemType[x][y] = Type;

	CObject *gem = new CObject();
	gem->LoadSprite(strGems[Type]);
	gem->SetZ(eZOrder_PopupOverPopup + 20);
	gem->Pos.x = board[x][y]->Pos.x;
	gem->Pos.y =  board[x][y]->Pos.y;

	gems[x][y] = gem;
}

void Match3::RemoveMatches()
{
    // Remove triplets++
	for( int iRow = 0; iRow < MAX_MATCH3_ROWS; iRow++ )
		for( int iCol = 0; iCol < MAX_MATCH3_COLS; iCol++ )
		{
			int TileId = gemType[iRow][iCol];
			// Is there a hrizontal match!
			if( (IsTileIdPresent(iRow, iCol+1, TileId)&&
				 IsTileIdPresent(iRow, iCol+2, TileId)) ||			   
	   			// Is there a vertical match!			
				(IsTileIdPresent(iRow+1, iCol, TileId)&&
				 IsTileIdPresent(iRow+2, iCol, TileId)) )
			{
				while( true )
				{
					TileId = (TileId+1)%MAX_MATCH3_VARIETY; /* 0-4 */
					if( !IsTileIdPresent(iRow,   iCol-1, TileId)&&
						!IsTileIdPresent(iRow,   iCol+1, TileId)&&
						!IsTileIdPresent(iRow-1, iCol,   TileId)&&								 
					    !IsTileIdPresent(iRow+1, iCol,   TileId))
						break;
				}
				if( gems[iRow][iCol] != NULL )
				{
					delete gems[iRow][iCol]; gems[iRow][iCol] = NULL;
				}
				insertNewGem(iRow, iCol, TileId);
			}
		}
}
