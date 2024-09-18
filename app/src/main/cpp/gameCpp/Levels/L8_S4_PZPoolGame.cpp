//====================================
//  created by : Sreeraj
//  copyright  : Tuttifrutti Games
//====================================

#include "L8_S4_PZPoolGame.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
//#include "HOScene.h"
//#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
//#include "TextureFactory.h"
//#include "ParticleEffect.h"
//#include "Rect.h"
//#include "Polygon.h"
//#include "Resources.h"
//#include "Application.h"
//#include "SDrop.h"
//#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"

L8_S4_PZPoolGame::L8_S4_PZPoolGame()
{
	_particleTimer = 0.0f;
	_gameExitThreshold = 2.5f;
	_partileTimerThreshold = 1.0f;
	_isParticlesStarted	= false;
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S4\\PZ1\\S4.SCN");
	
	InitSceneElements();

	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

	ResetDia = new CHOScene();
	ResetDia->Load("UI\\RESETDIA.SCN");

	CObject* resetText = new CObject();
	resetText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\RESETCNF.SPR", 670, 308, 2);
	resetText->SetUID("resetText");
	ResetDia->PushObject(resetText);

	CObject* mask = new CObject();
	mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
	mask->SetUID("mask");
	mask->SetScale(2);
	ResetDia->PushObject(mask);

	CObject* btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", 590, 420, 5);
	btnText->SetUID("yesBTNText");
	ResetDia->PushObject(btnText);

	btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", 780, 420, 5);
	btnText->SetUID("noBTNText");
	ResetDia->PushObject(btnText);

	ResetDia->SetZOff(eZOrder_Popup);
	ResetDia->Show(false);
	IsResetPop = false;

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	/*Control::Audio->LoadSample(aAmbL8s4PoolAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL8s4PoolAmb) )		
	{
		Control::Audio->PlaySample(aAmbL8s4PoolAmb,true);
	}*/
	
	//MUSIC_SFX
	
	InitAnimations();

	Hud->InvBox_StartPuzzleMode();

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S4_PZPoolGame]  = true;
}

void L8_S4_PZPoolGame::InitAnimations()
{

	S4_PoolPuzzleComplete1FX = new SuperFX("PRT\\L8\\S4_PoolPuzzleComplete1.INI", PointSystem::CreateCPoint(682,404,0), 1100); 
	S4_PoolPuzzleComplete2FX = new SuperFX("PRT\\L8\\S4_PoolPuzzleComplete2.INI", PointSystem::CreateCPoint(680,271,0), 1100); 
	S4_PoolPuzzleComplete3FX = new SuperFX("PRT\\L8\\S4_PoolPuzzleComplete3.INI", PointSystem::CreateCPoint(616,337,0), 1100); 
	S4_PoolPuzzleComplete4FX = new SuperFX("PRT\\L8\\S4_PoolPuzzleComplete4.INI", PointSystem::CreateCPoint(747,339,0), 1100); 

	

	//S4_PoolPuzzleComplete1FX->AdvanceByTime(2.0f);
	//S4_PoolPuzzleComplete2FX->AdvanceByTime(2.0f);
	//S4_PoolPuzzleComplete3FX->AdvanceByTime(2.0f);
	//S4_PoolPuzzleComplete4FX->AdvanceByTime(2.0f);

}


void L8_S4_PZPoolGame::InitSceneElements()
{	

	SolvedState = HOScene->GetObjectByID("SolvedState");
	SolvedState->SetEnable(false);

	for (int i = 0; i < 24; i++)
	{
		BigFlowers[i] = new Flowers();
		SmallFlowers[i] = new Flowers();
	}


	BgLeaves[0]			= HOScene->GetObjectByID("BgLeaf1_0");
	BgLeaves[1]			= HOScene->GetObjectByID("BgLeaf1_1");
	BgLeaves[2]			= HOScene->GetObjectByID("BgLeaf1_2");
	BgLeaves[3]			= HOScene->GetObjectByID("BgLeaf1_3");
	BgLeaves[4]			= HOScene->GetObjectByID("BgLeaf1_4");
	BgLeaves[5]			= HOScene->GetObjectByID("BgLeaf1_5");
	BgLeaves[6]			= HOScene->GetObjectByID("BgLeaf2_0");
	BgLeaves[7]			= HOScene->GetObjectByID("BgLeaf2_1");
	BgLeaves[8]			= HOScene->GetObjectByID("BgLeaf2_2");
	BgLeaves[9]			= HOScene->GetObjectByID("BgLeaf3_0");
	BgLeaves[10]		= HOScene->GetObjectByID("BgLeaf3_1");
	BgLeaves[11]		= HOScene->GetObjectByID("BgLeaf3_2");
	BgLeaves[12]		= HOScene->GetObjectByID("BgLeaf3_3");
	BgLeaves[13]		= HOScene->GetObjectByID("BgLeaf3_4");
	BgLeaves[14]		= HOScene->GetObjectByID("BgLeaf3_5");
	BgLeaves[15]		= HOScene->GetObjectByID("BgLeaf3_6");
	BgLeaves[16]		= HOScene->GetObjectByID("BgLeaf3_7");
	BgLeaves[17]		= HOScene->GetObjectByID("BgLeaf4_0");
	BgLeaves[18]		= HOScene->GetObjectByID("BgLeaf4_1");
	BgLeaves[19]		= HOScene->GetObjectByID("BgLeaf4_2");
	BgLeaves[20]		= HOScene->GetObjectByID("BgLeaf4_3");
	BgLeaves[21]		= HOScene->GetObjectByID("BgLeaf4_4");
	BgLeaves[22]		= HOScene->GetObjectByID("BgLeaf4_5");
			
			
	BgFlowers[0]		= HOScene->GetObjectByID("BgFlower1_0");
	BgFlowers[1]		= HOScene->GetObjectByID("BgFlower1_1");
	BgFlowers[2]		= HOScene->GetObjectByID("BgFlower2_0");
	BgFlowers[3]		= HOScene->GetObjectByID("BgFlower2_1");
	BgFlowers[4]		= HOScene->GetObjectByID("BgFlower3_0");
	BgFlowers[5]		= HOScene->GetObjectByID("BgFlower3_0");

	for (int i = 0; i < 6; i++)
	{
		int RandomSeed = 8 + rand()%19;
		BgFlowers[i]->Animate(4.5f,3.5f,0.5f,0.0f,(float)RandomSeed); 
	}

	for (int i = 0; i < 23; i++)
	{
		int RandomSeed = 8 + rand()%19;
		BgLeaves[i]->Animate(4.5f,3.5f,0.5f,0.0f,(float)RandomSeed); 
	}

	BigFlowers[0]->obj = HOScene->GetObjectByID("Flower_Big_One_1");
	BigFlowers[1]->obj = HOScene->GetObjectByID("Flower_Big_One_2");
	BigFlowers[2]->obj = HOScene->GetObjectByID("Flower_Big_One_3");
	BigFlowers[3]->obj = HOScene->GetObjectByID("Flower_Big_One_4");
	BigFlowers[4]->obj = HOScene->GetObjectByID("Flower_Big_One_5");
	BigFlowers[5]->obj = HOScene->GetObjectByID("Flower_Big_One_6");
	BigFlowers[6]->obj = HOScene->GetObjectByID("Flower_Big_One_7");
	BigFlowers[7]->obj = HOScene->GetObjectByID("Flower_Big_One_8");
	BigFlowers[8]->obj = HOScene->GetObjectByID("Flower_Big_One_9");
	BigFlowers[9]->obj = HOScene->GetObjectByID("Flower_Big_One_10");
	BigFlowers[10]->obj = HOScene->GetObjectByID("Flower_Big_One_11");
	BigFlowers[11]->obj = HOScene->GetObjectByID("Flower_Big_One_12");

	BigFlowers[12]->obj = HOScene->GetObjectByID("Flower_Big_Two_1");
	BigFlowers[13]->obj = HOScene->GetObjectByID("Flower_Big_Two_2");
	BigFlowers[14]->obj = HOScene->GetObjectByID("Flower_Big_Two_3");
	BigFlowers[15]->obj = HOScene->GetObjectByID("Flower_Big_Two_4");
	BigFlowers[16]->obj = HOScene->GetObjectByID("Flower_Big_Two_5");
	BigFlowers[17]->obj = HOScene->GetObjectByID("Flower_Big_Two_6");
				  
	BigFlowers[18]->obj =  HOScene->GetObjectByID("Flower_Big_Three_1");
	BigFlowers[19]->obj =  HOScene->GetObjectByID("Flower_Big_Three_2");
	BigFlowers[20]->obj =  HOScene->GetObjectByID("Flower_Big_Three_3");
				  
	BigFlowers[21]->obj =  HOScene->GetObjectByID("Flower_Big_Four_1");
	BigFlowers[22]->obj =  HOScene->GetObjectByID("Flower_Big_Four_2");
	BigFlowers[23]->obj =  HOScene->GetObjectByID("Flower_Big_Four_3");
	
	for (int i = 0; i < 24; i++)
	{
		BigFlowers[i]->index = i;
		BigFlowers[i]->initialPosition = BigFlowers[i]->obj->Pos;
		BigFlowers[i]->isVisible = true;
		switch (i)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				BigFlowers[i]->flowerCount = 1;
				break;
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
				BigFlowers[i]->flowerCount = 2;
				break;
			case 18:
			case 19:
			case 20:
				BigFlowers[i]->flowerCount = 3;
				break;
			case 21:
			case 22:
			case 23:
				BigFlowers[i]->flowerCount = 4;
				break;
		}
	}
	
	SmallFlowers[0]->obj = HOScene->GetObjectByID("Flower_Small_One_1");
	SmallFlowers[1]->obj = HOScene->GetObjectByID("Flower_Small_One_2");
	SmallFlowers[2]->obj = HOScene->GetObjectByID("Flower_Small_One_3");
	SmallFlowers[3]->obj = HOScene->GetObjectByID("Flower_Small_One_4");
	SmallFlowers[4]->obj = HOScene->GetObjectByID("Flower_Small_One_5");
	SmallFlowers[5]->obj = HOScene->GetObjectByID("Flower_Small_One_6");
	SmallFlowers[6]->obj = HOScene->GetObjectByID("Flower_Small_One_7");
	SmallFlowers[7]->obj = HOScene->GetObjectByID("Flower_Small_One_8");
	SmallFlowers[8]->obj = HOScene->GetObjectByID("Flower_Small_One_9");
	SmallFlowers[9]->obj = HOScene->GetObjectByID("Flower_Small_One_10");
	SmallFlowers[10]->obj = HOScene->GetObjectByID("Flower_Small_One_11");
	SmallFlowers[11]->obj = HOScene->GetObjectByID("Flower_Small_One_12");

	SmallFlowers[12]->obj = HOScene->GetObjectByID("Flower_Small_Two_1");
	SmallFlowers[13]->obj = HOScene->GetObjectByID("Flower_Small_Two_2");
	SmallFlowers[14]->obj = HOScene->GetObjectByID("Flower_Small_Two_3");
	SmallFlowers[15]->obj = HOScene->GetObjectByID("Flower_Small_Two_4");
	SmallFlowers[16]->obj = HOScene->GetObjectByID("Flower_Small_Two_5");
	SmallFlowers[17]->obj = HOScene->GetObjectByID("Flower_Small_Two_6");

	SmallFlowers[18]->obj = HOScene->GetObjectByID("Flower_Small_Three_1");
	SmallFlowers[19]->obj = HOScene->GetObjectByID("Flower_Small_Three_2");
	SmallFlowers[20]->obj = HOScene->GetObjectByID("Flower_Small_Three_3");
	
	SmallFlowers[21]->obj = HOScene->GetObjectByID("Flower_Small_Four_1");
	SmallFlowers[22]->obj = HOScene->GetObjectByID("Flower_Small_Four_2");
	SmallFlowers[23]->obj = HOScene->GetObjectByID("Flower_Small_Four_3");

	for (int i = 0; i < 24; i++)
	{
		SmallFlowers[i]->obj->Show(false);
		SmallFlowers[i]->index = i;
		SmallFlowers[i]->isVisible = false;
	}

	ColSelection = HOScene->GetObjectByID("ColSelection");
	ColSelection->Show(false);

	glowRect[0][0] = HOScene->GetObjectByID("glowRect1");
	glowRect[0][1] = HOScene->GetObjectByID("glowRect2");
	glowRect[0][2] = HOScene->GetObjectByID("glowRect3");
	glowRect[1][0] = HOScene->GetObjectByID("glowRect4");
	glowRect[1][1] = HOScene->GetObjectByID("glowRect5");
	glowRect[1][2] = HOScene->GetObjectByID("glowRect6");
	glowRect[2][0] = HOScene->GetObjectByID("glowRect7");
	glowRect[2][1] = HOScene->GetObjectByID("glowRect8");
	glowRect[2][2] = HOScene->GetObjectByID("glowRect9");

	glowRectGreen[0][0] = HOScene->GetObjectByID("glowRectGreen1");
	glowRectGreen[0][1] = HOScene->GetObjectByID("glowRectGreen2");
	glowRectGreen[0][2] = HOScene->GetObjectByID("glowRectGreen3");
	glowRectGreen[1][0] = HOScene->GetObjectByID("glowRectGreen4");
	glowRectGreen[1][1] = HOScene->GetObjectByID("glowRectGreen5");
	glowRectGreen[1][2] = HOScene->GetObjectByID("glowRectGreen6");
	glowRectGreen[2][0] = HOScene->GetObjectByID("glowRectGreen7");
	glowRectGreen[2][1] = HOScene->GetObjectByID("glowRectGreen8");
	glowRectGreen[2][2] = HOScene->GetObjectByID("glowRectGreen9");

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			glowRect[i][j]->Show(false);
			glowRectGreen[i][j]->Show(false);
		}
	}

	leftText[0][0] = HOScene->GetObjectByID("Text_Yellow_Smal_Vert0");
	leftText[0][1] = HOScene->GetObjectByID("Text_Yellow_Smal_Vert3");
	leftText[1][0] = HOScene->GetObjectByID("Text_Yellow_Smal_Vert1");
	leftText[1][1] = HOScene->GetObjectByID("Text_Yellow_Smal_Vert4");
	leftText[2][0] = HOScene->GetObjectByID("Text_Yellow_Smal_Vert2");
	leftText[2][1] = HOScene->GetObjectByID("Text_Yellow_Smal_Vert5");

	topText[0][0] = HOScene->GetObjectByID("Text_Yellow_Small_Hor0");
	topText[0][1] = HOScene->GetObjectByID("Text_Yellow_Small_Hor3");
	topText[1][0] = HOScene->GetObjectByID("Text_Yellow_Small_Hor1");
	topText[1][1] = HOScene->GetObjectByID("Text_Yellow_Small_Hor4");
	topText[2][0] = HOScene->GetObjectByID("Text_Yellow_Small_Hor2");
	topText[2][1] = HOScene->GetObjectByID("Text_Yellow_Small_Hor5");

	vertText1[0] = HOScene->GetObjectByID("Text_Yellow_Large1");
	vertText1[1] = HOScene->GetObjectByID("Text_Yellow_Large2");

	vertText2[0] = HOScene->GetObjectByID("Text_Yellow_Large21");
	vertText2[1] = HOScene->GetObjectByID("Text_Yellow_Large22");
	//
	leftTextGreen[0][0] = HOScene->GetObjectByID("Text_Green_Small_Vert0");
	leftTextGreen[0][1] = HOScene->GetObjectByID("Text_Green_Small_Vert3");
	leftTextGreen[1][0] = HOScene->GetObjectByID("Text_Green_Small_Vert1");
	leftTextGreen[1][1] = HOScene->GetObjectByID("Text_Green_Small_Vert4");
	leftTextGreen[2][0] = HOScene->GetObjectByID("Text_Green_Small_Vert2");
	leftTextGreen[2][1] = HOScene->GetObjectByID("Text_Green_Small_Vert5");

	topTextGreen[0][0] = HOScene->GetObjectByID("Text_Green_Small_Hor0");
	topTextGreen[0][1] = HOScene->GetObjectByID("Text_Green_Small_Hor3");
	topTextGreen[1][0] = HOScene->GetObjectByID("Text_Green_Small_Hor1");
	topTextGreen[1][1] = HOScene->GetObjectByID("Text_Green_Small_Hor4");
	topTextGreen[2][0] = HOScene->GetObjectByID("Text_Green_Small_Hor2");
	topTextGreen[2][1] = HOScene->GetObjectByID("Text_Green_Small_Hor5");

	vertText1Green[0] = HOScene->GetObjectByID("Text_Green_Large1");
	vertText1Green[1] = HOScene->GetObjectByID("Text_Green_Large2");

	vertText2Green[0] = HOScene->GetObjectByID("Text_Green_Large21");
	vertText2Green[1] = HOScene->GetObjectByID("Text_Green_Large22");

	hideAllText();
	
	PuzzleSolvedMsg = HOScene->GetObjectByID("PuzzleSolvedMsg");
	PuzzleSolvedMsg->Show(false);
	isPuzzleSolved = false;

	isFlowerOnHand = false;
	isFlowerReversing = false;

	tweenTime = 0;
	swapTime = 0.50f;
	initBoardVlaues();

	if(ProfileSystem::Instance->ActiveProfile.L8_S4_IsPuzzleDataSaved == false)
	{
		updateBoardWithInitialFlowers();
		ProfileSystem::Instance->ActiveProfile.L8_S4_IsPuzzleDataSaved = true;
	}

	updatePuzzleStatusWithSavedValues();
}

void L8_S4_PZPoolGame::showGreenLight()
{
	SolvedState->SetAlpha(0.0f);
	SolvedState->SetEnable(true);
	SolvedState->FadeIn(1.5f);
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	isPuzzleSolved = true;
	Hud->InvBox_StopPuzzleMode();

	hideAllText();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			glowRect[i][j]->Show(false);
			//glowRectGreen[i][j]->Show(true);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			leftTextGreen[i][j]->Show(true);	
			topTextGreen[i][j]->Show(true);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		vertText1Green[i]->Show(true);
		vertText2Green[i]->Show(true);
	}
}

void L8_S4_PZPoolGame::hideAllText()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			leftText[i][j]->Show(false);	
			topText[i][j]->Show(false);

			leftTextGreen[i][j]->Show(false);	
			topTextGreen[i][j]->Show(false);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		vertText1[i]->Show(false);
		vertText2[i]->Show(false);

		vertText1Green[i]->Show(false);
		vertText2Green[i]->Show(false);
	}
}

void L8_S4_PZPoolGame::updateBoardWithInitialFlowers()
{
	for (int i = 0; i < 16; i++)
	{
		arrAddedIndex[i] = -1;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{

				if(initialFlowersOnBoard[i][j][k] > 0)
				{

					for (int m = 0; m < 24; m++)
					{
						if(BigFlowers[m]->flowerCount == initialFlowersOnBoard[i][j][k])
						{
							if(isIndexAdded(m)) 
							{
								continue;
							}
							else
							{

								flowersOnBoard[i][j][k] = SmallFlowers[m];
								flowersOnBoard[i][j][k]->index = m;
								flowersOnBoard[i][j][k]->isVisible = true;
								BigFlowers[m]->Show(false);
								SmallFlowers[m]->Show(true);
								SmallFlowers[m]->obj->Pos.x = smallBoardPositions[i][j][k]->x + 30;
								SmallFlowers[m]->obj->Pos.y = smallBoardPositions[i][j][k]->y + 30;
								ProfileSystem::Instance->ActiveProfile.L8_S4_FlowersOnBoardIndex[i][j][k] = m;
								for (int n = 0; n < 16; n++)
								{
									if(arrAddedIndex[n] == -1)
									{
										arrAddedIndex[n] = m;
										break;
									}
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}

bool L8_S4_PZPoolGame::isIndexAdded(int index)
{
	for (int i = 0; i < 16; i++)
	{
		if(arrAddedIndex[i] == index) return true;
	}

	return false;
}

void L8_S4_PZPoolGame::initBoardVlaues()
{
	board[0][0] = CRectangle (489.0f,141.0f,125.0f,125.0f);
	board[0][1] = CRectangle (622.0f,141.0f,123.0f,124.0f);
	board[0][2] = CRectangle (753.0f,140.0f,124.0f,127.0f);
	board[1][0] = CRectangle (489.0f,273.0f,123.0f,125.0f);
	board[1][1] = CRectangle (621.0f,272.0f,125.0f,128.0f);
	board[1][2] = CRectangle (754.0f,273.0f,122.0f,125.0f);
	board[2][0] = CRectangle (489.0f,405.0f,126.0f,123.0f);
	board[2][1] = CRectangle (620.0f,404.0f,125.0f,125.0f);
	board[2][2] = CRectangle (753.0f,406.0f,124.0f,125.0f);

	ColSelectionPos[0][0] = new CPoint (552,202,0.0f);
	ColSelectionPos[0][1] = new CPoint (684,202,0.0f);
	ColSelectionPos[0][2] = new CPoint (815,202,0.0f);
	ColSelectionPos[1][0] = new CPoint (552,334,0.0f);
	ColSelectionPos[1][1] = new CPoint (684,334,0.0f);
	ColSelectionPos[1][2] = new CPoint (815,334,0.0f);
	ColSelectionPos[2][0] = new CPoint (552,467,0.0f);
	ColSelectionPos[2][1] = new CPoint (684,467,0.0f);
	ColSelectionPos[2][2] = new CPoint (815,467,0.0f);


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				flowersOnBoard[i][j][k] = nullptr;
			}
		}
	}

	smallBoardPositions[0][0][0] = new CPoint(488.0f,139.0f,0.0f);
	smallBoardPositions[0][0][1] = new CPoint(551.0f,139.0f,0.0f);
	smallBoardPositions[0][0][2] = new CPoint(487.0f,200.0f,0.0f);
	smallBoardPositions[0][0][3] = new CPoint(553.0f,200.0f,0.0f);
	smallBoardPositions[0][1][0] = new CPoint(622.0f,140.0f,0.0f);
	smallBoardPositions[0][1][1] = new CPoint(681.0f,139.0f,0.0f);
	smallBoardPositions[0][1][2] = new CPoint(621.0f,201.0f,0.0f);
	smallBoardPositions[0][1][3] = new CPoint(682.0f,201.0f,0.0f);
	smallBoardPositions[0][2][0] = new CPoint(753.0f,140.0f,0.0f);
	smallBoardPositions[0][2][1] = new CPoint(814.0f,139.0f,0.0f);
	smallBoardPositions[0][2][2] = new CPoint(753.0f,201.0f,0.0f);
	smallBoardPositions[0][2][3] = new CPoint(816.0f,200.0f,0.0f);
	smallBoardPositions[1][0][0] = new CPoint(488.0f,271.0f,0.0f);
	smallBoardPositions[1][0][1] = new CPoint(554.0f,271.0f,0.0f);
	smallBoardPositions[1][0][2] = new CPoint(488.0f,334.0f,0.0f);
	smallBoardPositions[1][0][3] = new CPoint(555.0f,334.0f,0.0f);
	smallBoardPositions[1][1][0] = new CPoint(620.0f,274.0f,0.0f);
	smallBoardPositions[1][1][1] = new CPoint(682.0f,272.0f,0.0f);
	smallBoardPositions[1][1][2] = new CPoint(619.0f,335.0f,0.0f);
	smallBoardPositions[1][1][3] = new CPoint(682.0f,335.0f,0.0f);
	smallBoardPositions[1][2][0] = new CPoint(752.0f,272.0f,0.0f);
	smallBoardPositions[1][2][1] = new CPoint(815.0f,273.0f,0.0f);
	smallBoardPositions[1][2][2] = new CPoint(753.0f,336.0f,0.0f);
	smallBoardPositions[1][2][3] = new CPoint(815.0f,337.0f,0.0f);
	smallBoardPositions[2][0][0] = new CPoint(489.0f,406.0f,0.0f);
	smallBoardPositions[2][0][1] = new CPoint(554.0f,406.0f,0.0f);
	smallBoardPositions[2][0][2] = new CPoint(489.0f,466.0f,0.0f);
	smallBoardPositions[2][0][3] = new CPoint(553.0f,466.0f,0.0f);
	smallBoardPositions[2][1][0] = new CPoint(620.0f,404.0f,0.0f);
	smallBoardPositions[2][1][1] = new CPoint(684.0f,406.0f,0.0f);
	smallBoardPositions[2][1][2] = new CPoint(619.0f,466.0f,0.0f);
	smallBoardPositions[2][1][3] = new CPoint(684.0f,466.0f,0.0f);
	smallBoardPositions[2][2][0] = new CPoint(751.0f,405.0f,0.0f);
	smallBoardPositions[2][2][1] = new CPoint(814.0f,405.0f,0.0f);
	smallBoardPositions[2][2][2] = new CPoint(750.0f,466.0f,0.0f);
	smallBoardPositions[2][2][3] = new CPoint(814.0f,467.0f,0.0f);

	numberOfDrops[0][0] = 3;
	numberOfDrops[0][1] = 3;
	numberOfDrops[0][2] = 2;

	numberOfDrops[1][0] = 2;
	numberOfDrops[1][1] = 2;
	numberOfDrops[1][2] = 3;

	numberOfDrops[2][0] = 4;
	numberOfDrops[2][1] = 1;
	numberOfDrops[2][2] = 4;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				initialFlowersOnBoard[i][j][k] = 0;
			}
		}
	}

	initialFlowersOnBoard[0][0][0] = 2;
	initialFlowersOnBoard[0][0][1] = 1;
	initialFlowersOnBoard[0][0][2] = 1;

	initialFlowersOnBoard[0][2][0] = 1;
	initialFlowersOnBoard[0][2][1] = 1;

	initialFlowersOnBoard[2][0][0] = 4;
	initialFlowersOnBoard[2][0][1] = 1;
	initialFlowersOnBoard[2][0][2] = 2;
	initialFlowersOnBoard[2][0][3] = 1;

	initialFlowersOnBoard[2][2][0] = 2;
	initialFlowersOnBoard[2][2][1] = 2;
	initialFlowersOnBoard[2][2][2] = 1;
	initialFlowersOnBoard[2][2][3] = 1;


}

void L8_S4_PZPoolGame::updatePuzzleStatusWithSavedValues()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				int index = ProfileSystem::Instance->ActiveProfile.L8_S4_FlowersOnBoardIndex[i][j][k];

				if(index >= 0)
				{

					flowersOnBoard[i][j][k] = SmallFlowers[index];
					flowersOnBoard[i][j][k]->index = index;
					flowersOnBoard[i][j][k]->isVisible = true;
					BigFlowers[index]->Show(false);
					SmallFlowers[index]->Show(true);
					SmallFlowers[index]->obj->Pos.x = smallBoardPositions[i][j][k]->x + 30;
					SmallFlowers[index]->obj->Pos.y = smallBoardPositions[i][j][k]->y + 30;
				}
			}
		}
	}


	CheckPuzzleSolved();
}

void L8_S4_PZPoolGame::Required()
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

void L8_S4_PZPoolGame::Update()
{	
	#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif

	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if(isPuzzleSolved)
	{
		if( _particleTimer > _gameExitThreshold )
		{
			ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S4_Pool;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
		}
		if(( _particleTimer > _partileTimerThreshold ) && !_isParticlesStarted )
		{
			S4_PoolPuzzleComplete1FX->Start();
			S4_PoolPuzzleComplete2FX->Start();
			S4_PoolPuzzleComplete3FX->Start();
			S4_PoolPuzzleComplete4FX->Start();
			_isParticlesStarted = true;
		}

		_particleTimer += Control::LogicRate;
		return;
	}

	if(isFlowerReversing)
	{
		movePointerBackToOriginalPosition();
		return;
	}

	if(isFlowerOnHand)
	{
		flowerOnHand->obj->Pos = pos;

		ColSelection->Show(false);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if((i== 0 && j== 0 ) || (i == 0 && j == 2) || (i == 2 && j == 0) || (i == 2 && j == 2)) continue;

				if(board[i][j].Intersection(&pos))
				{
					ColSelection->Show(true);
					ColSelection->Pos.x = ColSelectionPos[i][j]->x;
					ColSelection->Pos.y = ColSelectionPos[i][j]->y;
					if(Control::Input->LBclicked())
					{
						for (int k = 0; k < 4; k++)
						{
							if(flowersOnBoard[i][j][k] == nullptr)
							{
								SmallFlowers[flowerOnHand->index]->Show(true);
								SmallFlowers[flowerOnHand->index]->obj->Pos.x = smallBoardPositions[i][j][k]->x + 30;
								SmallFlowers[flowerOnHand->index]->obj->Pos.y = smallBoardPositions[i][j][k]->y + 30;								

								flowersOnBoard[i][j][k] = SmallFlowers[flowerOnHand->index];
								ProfileSystem::Instance->ActiveProfile.L8_S4_FlowersOnBoardIndex[i][j][k] = flowerOnHand->index;
								isFlowerOnHand = false;
								flowerOnHand->isVisible = false;
								SmallFlowers[flowerOnHand->index]->isVisible = true;
								flowerOnHand->Show(false);
								ColSelection->Show(false);

								if(CheckPuzzleSolved())
								{
									showGreenLight();
								}
								return;
							}
						}
						isFlowerReversing = true;
						isFlowerOnHand = false;
						ColSelection->Show(false);
						return;
					}
				}
			}
		}

		if(Control::Input->LBclicked())
		{
			isFlowerReversing = true;
			isFlowerOnHand = false;
			return;
		}
	}

	for (int i = 0; i < 24; i++)
	{		
		if(BigFlowers[i]->obj->IsIntersect(pos) && isFlowerOnHand == false && BigFlowers[i]->isVisible == true)
		{
			Cursor::SetMode(CA_HandPointy);
			if(Control::Input->LBclicked())
			{
				isFlowerOnHand = true;
				flowerOnHand = BigFlowers[i];
				flowerOnHand->obj->SetZ(200);
				return;
			}
		}

		if(SmallFlowers[i]->obj->IsIntersect(pos) && isFlowerOnHand == false && SmallFlowers[i]->isVisible == true)
		{
			if( !isMouseOnDisabledTile(pos) ) 
			{
				Cursor::SetMode(CA_HandPointy);
				if(Control::Input->LBclicked())
				{
					isFlowerOnHand = true;
					flowerOnHand = BigFlowers[i];
					flowerOnHand->Show(true);
					flowerOnHand->obj->SetZ(200);
					removeSmallFlowerOnBoard();
					CheckPuzzleSolved();
					break;
				}
			}
		}

	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S4_Pool;
		SendMessage(ControlExit, ID, HOSM_NewArea);					
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L8_S4_Pool);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L8_S4_Pool);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S4_Pool, 2);
			}
#else			
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S4_Pool;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}
	}
}

bool L8_S4_PZPoolGame::isMouseOnDisabledTile(CPoint pos)
{
	if(board[0][0].Intersection(&pos) || board[0][2].Intersection(&pos) || board[2][0].Intersection(&pos) || board[2][2].Intersection(&pos))
	{
		return true;
	}
	return false;
}

bool L8_S4_PZPoolGame::CheckPuzzleSolved()
{
	hideAllText();
	int total = 0;
	int dropCount = 0;
	bool returnValue = true;
	int successLine = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			glowRect[i][j]->Show(false);
			total = 0;
			dropCount = 0;
			for (int k = 0; k < 4; k++)
			{
				if(flowersOnBoard[i][j][k] != nullptr)
				{
					dropCount++;
					total += BigFlowers[flowersOnBoard[i][j][k]->index]->flowerCount;
				}
			}
			numberOfPlayerDrops[i][j] = dropCount;
			totalFlowersInColumn[i][j] = total;
		}
	}

	
	total = totalFlowersInColumn[0][0] + totalFlowersInColumn[0][1] + totalFlowersInColumn[0][2];
	if(total == 15 && numberOfPlayerDrops[0][0] == numberOfDrops[0][0] && numberOfPlayerDrops[0][1] == numberOfDrops[0][1] && numberOfPlayerDrops[0][2] == numberOfDrops[0][2])
	{
		successLine++;
		showSuccessLineGlow(1,0);
		leftText[0][0]->Show(true);
		leftText[0][1]->Show(true);
	}

	total = totalFlowersInColumn[1][0] + totalFlowersInColumn[1][1] + totalFlowersInColumn[1][2];
	if(total == 15 && numberOfPlayerDrops[1][0] == numberOfDrops[1][0] && numberOfPlayerDrops[1][1] == numberOfDrops[1][1] && numberOfPlayerDrops[1][2] == numberOfDrops[1][2])
	{
		successLine++;
		showSuccessLineGlow(1,1);
		leftText[1][0]->Show(true);
		leftText[1][1]->Show(true);
	}

	total = totalFlowersInColumn[2][0] + totalFlowersInColumn[2][1] + totalFlowersInColumn[2][2];
	if(total == 15 && numberOfPlayerDrops[2][0] == numberOfDrops[2][0] && numberOfPlayerDrops[2][1] == numberOfDrops[2][1] && numberOfPlayerDrops[2][2] == numberOfDrops[2][2])
	{
		successLine++;
		showSuccessLineGlow(1,2);
		leftText[2][0]->Show(true);
		leftText[2][1]->Show(true);
	}

	total = totalFlowersInColumn[0][0] + totalFlowersInColumn[1][0] + totalFlowersInColumn[2][0];
	if(total == 15 && numberOfPlayerDrops[0][0] == numberOfDrops[0][0] && numberOfPlayerDrops[1][0] == numberOfDrops[1][0] && numberOfPlayerDrops[2][0] == numberOfDrops[2][0])
	{
		successLine++;
		showSuccessLineGlow(2,0);
		topText[0][0]->Show(true);
		topText[0][1]->Show(true);
	}

	total = totalFlowersInColumn[0][1] + totalFlowersInColumn[1][1] + totalFlowersInColumn[2][1];
	if(total == 15 && numberOfPlayerDrops[0][1] == numberOfDrops[0][1] && numberOfPlayerDrops[1][1] == numberOfDrops[1][1] && numberOfPlayerDrops[2][1] == numberOfDrops[2][1])
	{
		successLine++;
		showSuccessLineGlow(2,1);
		topText[1][0]->Show(true);
		topText[1][1]->Show(true);
	}

	total = totalFlowersInColumn[0][2] + totalFlowersInColumn[1][2] + totalFlowersInColumn[2][2];
	if(total == 15 && numberOfPlayerDrops[0][2] == numberOfDrops[0][2] && numberOfPlayerDrops[1][2] == numberOfDrops[1][2] && numberOfPlayerDrops[2][2] == numberOfDrops[2][2])
	{
		successLine++;
		showSuccessLineGlow(2,2);
		topText[2][0]->Show(true);
		topText[2][1]->Show(true);
	}

	total = totalFlowersInColumn[0][0] + totalFlowersInColumn[1][1] + totalFlowersInColumn[2][2];
	if(total == 15 && numberOfPlayerDrops[0][0] == numberOfDrops[0][0] && numberOfPlayerDrops[1][1] == numberOfDrops[1][1] && numberOfPlayerDrops[2][2] == numberOfDrops[2][2])
	{
		successLine++;
		showSuccessLineGlow(3,0);
		vertText1[0]->Show(true);
		vertText1[1]->Show(true);
	}

	total = totalFlowersInColumn[2][0] + totalFlowersInColumn[1][1] + totalFlowersInColumn[0][2];
	if(total == 15 && numberOfPlayerDrops[2][0] == numberOfDrops[2][0] && numberOfPlayerDrops[1][1] == numberOfDrops[1][1] && numberOfPlayerDrops[0][2] == numberOfDrops[0][2])
	{
		successLine++;
		showSuccessLineGlow(4,0);
		vertText2[0]->Show(true);
		vertText2[1]->Show(true);
	}
	
	if(successLine < 8)
	{
		returnValue = false;
	}

	//showGreenLight();
	
	return returnValue;
	
}

void L8_S4_PZPoolGame::showSuccessLineGlow(int state,int index = 0)
{
	switch (state)
	{
		case 1:
			glowRect[index][0]->Show(true);
			glowRect[index][1]->Show(true);
			glowRect[index][2]->Show(true);
			break;
		case 2:
			glowRect[0][index]->Show(true);
			glowRect[1][index]->Show(true);
			glowRect[2][index]->Show(true);
			break;
		case 3:
			glowRect[0][0]->Show(true);
			glowRect[1][1]->Show(true);
			glowRect[2][2]->Show(true);
			break;
		case 4:
			glowRect[2][0]->Show(true);
			glowRect[1][1]->Show(true);
			glowRect[0][2]->Show(true);
			break;
	}
}


void L8_S4_PZPoolGame::removeSmallFlowerOnBoard()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{	
			for (int k = 0; k < 4; k++)
			{
				if(flowersOnBoard[i][j][k] == SmallFlowers[flowerOnHand->index])
				{
					flowersOnBoard[i][j][k]->isVisible = false;
					flowersOnBoard[i][j][k]->Show(false);
					flowersOnBoard[i][j][k] = nullptr;
					ProfileSystem::Instance->ActiveProfile.L8_S4_FlowersOnBoardIndex[i][j][k] = -1;
					return;
				}
			}
		}
	}
}

void L8_S4_PZPoolGame::movePointerBackToOriginalPosition()
{
	tweenTime += Control::LogicRate;
	CPoint temp;
	temp.x = flowerOnHand->obj->Pos.x + (flowerOnHand->initialPosition.x - flowerOnHand->obj->Pos.x) * easing(tweenTime,0,1,swapTime);
	temp.y = flowerOnHand->obj->Pos.y + (flowerOnHand->initialPosition.y - flowerOnHand->obj->Pos.y) * easing(tweenTime,0,1,swapTime);

	flowerOnHand->obj->Pos = temp;

	if( tweenTime >= swapTime )
	{
		flowerOnHand->obj->Pos = flowerOnHand->initialPosition;
		SmallFlowers[flowerOnHand->index]->isVisible = false;
		BigFlowers[flowerOnHand->index]->isVisible = true;
		tweenTime = 0;
		isFlowerReversing = false;
	}
}

float L8_S4_PZPoolGame::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L8_S4_PZPoolGame::ReceiveMessage(int val)
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

void L8_S4_PZPoolGame::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	showGreenLight();
	/*ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved = true;
	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S4_Pool;
	SendMessage(ControlExit, ID, HOSM_NewArea);	*/
}

void L8_S4_PZPoolGame::ProcessNonHOHint()
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

L8_S4_PZPoolGame::~L8_S4_PZPoolGame()
{
	Hud->InvBox_StopPuzzleMode();

	for (int i = 0; i < 24; i++)
	{
		SAFE_DELETE(BigFlowers[i]);
		SAFE_DELETE(SmallFlowers[i]);	
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			SAFE_DELETE(ColSelectionPos[i][j])
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				SAFE_DELETE(smallBoardPositions[i][j][k])
			}
		}
	}

	SAFE_DELETE(ResetDia);
	
	SAFE_DELETE(S4_PoolPuzzleComplete1FX);
	SAFE_DELETE(S4_PoolPuzzleComplete2FX);
	SAFE_DELETE(S4_PoolPuzzleComplete3FX);
	SAFE_DELETE(S4_PoolPuzzleComplete4FX);

	//MUSIC_SFX
	
	//Control::Audio->StopSample(aAmbL8s4PoolAmb);
	//Control::Audio->UnloadSample(aAmbL8s4PoolAmb);
	//MUSIC_SFX
}
void L8_S4_PZPoolGame::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved)
	{
		ResetBtn->SetEnable(false);
		return;
	}
	ResetBtn->SetEnable(true);
	if(timer > 0)
	{
		timer -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
		ResetBtn->SetEnable(false);
		if(!IsResetPop)
		{
			if(!isResetDiaFadeOut)
				ResetDia->FadeOut(2);
			isResetDiaFadeOut = true;
		}
		return;
	}
	else
	{
		ResetBtn->SetEnable(true);	
		if(!isFadeIn)
			ResetBtn->FadeIn(1);
		isFadeIn = true;
	}
	if(ResetBtn->IsIntersect(Control::Input->Mpos()))
	{
		ResetBtn->SetAnimation(1);
		if(!IsResetPop)
			Cursor::SetMode(CA_HandPointy);
		if(KInput::getLeftButtonState() == ISDOWN)
		{
			IsResetPop = true;
		}
	}
	else
	{
		ResetBtn->SetAnimation(2);
		Cursor::SetMode(CA_Normal);
	}

	if(IsResetPop)
	{
		if(!isResetDiaFadeIn)
		{
			ResetDia->FadeIn(2);
		}
		if(!isFadeOut)
			ResetBtn->FadeOut(1);
		isFadeOut = true;
		isResetDiaFadeIn = true;
		CObject *resetYes = ResetDia->GetObjectByID("BtnYes");
		CObject *resetNo = ResetDia->GetObjectByID("BtnNo");

		if(Hud->CheckPopupButton(resetYes) == Popup_Click)
		{
			ProfileSystem::Instance->ActiveProfile.L8_S4_IsPuzzleDataSaved = false;
			ProfileSystem::Instance->ActiveProfile.L8_S4_IsPoolPuzzleSolved	= false;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					for (int k = 0; k < 4; k++)
						ProfileSystem::Instance->ActiveProfile.L8_S4_FlowersOnBoardIndex[i][j][k] = -1;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S4_PZPoolGame;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			IsResetPop = false;
		}
		if(Hud->CheckPopupButton(resetNo) == Popup_Click)
		{
			IsResetPop = false;
			timer = 10.0f;
			isFadeIn = false;
			isFadeOut = false;
			isResetDiaFadeOut = false;
			isResetDiaFadeIn = false;
		}
	}
	else
	{
		ResetBtn->SetEnable(true);
	}
}
void L8_S4_PZPoolGame::NavigateToScene()
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