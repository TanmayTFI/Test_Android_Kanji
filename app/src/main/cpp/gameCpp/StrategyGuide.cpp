//====================================
//DQFULP
// Hidden Object System
// Strategy Guide
//====================================

#include "Application.h"
#include "StrategyGuide.h"
#include "HOScene.h"
#include "Resources.h"
#include "Util.h"
#include "Hud.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Fonts.h"
#include "GameFlow.h"
#include "InventoryBox.h"
#include "Tutorial.h"
#include "TextureFactory.h"
#include "LangDefs.h"

extern HudControl* GFHud;

const CRectangle  BtnRectLeftButton(589.0f,617.0f,60.0f,55.0f);
const CRectangle  BtnRectRightButton(714.0f,614.0f,57.0f,56.0f);

const CRectangle  StrategyGuideAreaRect(181.0f,46.0f,960.0f,653.0f);
const CRectangle  PreviewBoundsRect(295.0f,129.0f,786.0f,449.0f);

const CRectangle  BtnRectClose(1103.0f,22.0f,72.0f,67.0f);

const CRectangle  ChapterL1Rect(286.0f,188.0f,287.0f,35.0f);
const CRectangle  ChapterL2Rect(284.0f,233.0f,291.0f,32.0f);
const CRectangle  ChapterL3Rect(283.0f,276.0f,282.0f,29.0f);
const CRectangle  ChapterL4Rect(283.0f,314.0f,308.0f,33.0f);
const CRectangle  ChapterL5Rect(281.0f,358.0f,319.0f,33.0f);
const CRectangle  ChapterL6Rect(283.0f,399.0f,341.0f,31.0f);
const CRectangle  ChapterL7Rect(283.0f,438.0f,344.0f,33.0f);
const CRectangle  ChapterL8Rect(284.0f,481.0f,284.0f,32.0f);
const CRectangle  ChapterCollectiblesRect(285.0f,521.0f,281.0f,37.0f);


const std::string GUIDEPATH = "MAPDIA\\GUIDE\\";

//const int NumL1_Thumbs[MAX_GUIDE_LV1_PAGES] = {0, 0, 3, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 2};
//const int NumL2_Thumbs[MAX_GUIDE_LV2_PAGES] = {2, 1, 1, 2, 2,  2, 2, 1, 1, 3, 1};
//const int NumL3_Thumbs[MAX_GUIDE_LV3_PAGES] = {2, 1, 1, 1, 1,  1, 1, 1, 1, 1, 2, 3, 1};
//const int NumL4_Thumbs[MAX_GUIDE_LV4_PAGES] = {2, 1, 1, 2, 2,  1, 1, 2, 1};
//const int NumL5_Thumbs[MAX_GUIDE_LV5_PAGES] = {1, 2, 3, 1, 1,  3, 1, 1};
//const int NumL6_Thumbs[MAX_GUIDE_LV6_PAGES] = {2, 2, 1, 1, 1,  2, 2};
//const int NumL7_Thumbs[MAX_GUIDE_LV7_PAGES] = {1, 3, 2, 1};
//const int NumL8_Thumbs[MAX_GUIDE_LV8_PAGES] = {1, 1, 2, 1, 1};


const int LastPageIndex[Level_Max] = {
	MAX_GUIDE_LV1_PAGES,
	MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES,
	MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES,
	MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES,
	MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES,
	MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES+MAX_GUIDE_LV6_PAGES,
	MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES+MAX_GUIDE_LV6_PAGES+MAX_GUIDE_LV7_PAGES,
	MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES+MAX_GUIDE_LV6_PAGES+MAX_GUIDE_LV7_PAGES+MAX_GUIDE_LV8_PAGES
};

#define MAX_GUIDE_COLLECTABLES_PAGES 7

const int Num_Extras_Thumbs[MAX_GUIDE_COLLECTABLES_PAGES] = 
{
	6, //LEVEL 1
	6, //LEVEL 2
	5, //LEVEL 3
	5, //LEVEL 4
	5, //LEVEL 5
	5, //LEVEL 6
	4  //LEVEL 7
};

const int Num_Thumbs[] = 
{
	0, 0, 3, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, //LEVEL 1
	2, 1, 1, 2, 2,  2, 2, 1, 1, 3, 1,										//LEVEL 2
	2, 1, 1, 1, 1,  1, 1, 1, 1, 1, 2, 3, 1,									//LEVEL 3
	2, 1, 1, 2, 2,  1, 1, 2, 1,												//LEVEL 4
	1, 2, 3, 1, 1,  3, 1, 1,												//LEVEL 5
	2, 2, 1, 1, 1,  2, 2,													//LEVEL 6
	1, 3, 2, 1,																//LEVEL 7
	3, 3, 3, 1, 1															//LEVEL 8
};

StrategyGuide::StrategyGuide()
{
	guideScene = GFInstance->LoadBufferScene(GUIDE_SCN);	

	if (!guideScene->GetObjectByID("btncontentsText"))
	{
		CObject* temp = new CObject();
		temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\CONTENTS.SPR", guideScene->GetObjectByID("btncontents")->Pos.x, guideScene->GetObjectByID("btncontents")->Pos.y, guideScene->GetObjectByID("btncontents")->ZOrder + 1);
		temp->SetUID("btncontentsText");
		guideScene->PushObject(temp);
	}

	PageTEXT = NULL;
	PageNo = NULL;

	guideScene->Show(false);
	if(guideScene->GetObjectByID("diarybg")!=NULL && guideScene->GetObjectByID("diarybg")->ZOrder<=0)
		guideScene->SetZOff(eZOrder_MapdiaryPopup);//Single time offsetting

	BgZOrder = guideScene->GetObjectByID("diarybg")->ZOrder;
	BtnClose = guideScene->GetObjectByID("closebtn");

	BtnClosePreview = guideScene->GetObjectByID("previewclosebtn");
	BtnClosePreview->SetZ(BgZOrder + 20);
	BtnClosePreview->SetEnable(false);

	BtnContents = guideScene->GetObjectByID("btncontents");

	activeLevel = ProfileSystem::Instance->ActiveProfile.CurrentLevel;

#ifdef _DEMOBUILD
	maxGuidePages = MAX_GUIDE_LV1_PAGES;
#else
	#ifdef K_ANDROID
		if (!GFApp->isUnlocked()) //Free Build
			maxGuidePages = MAX_GUIDE_LV1_PAGES;
		else
	#endif
		maxGuidePages = MAX_GUIDE_LV1_PAGES + MAX_GUIDE_LV2_PAGES + MAX_GUIDE_LV3_PAGES + MAX_GUIDE_LV4_PAGES + MAX_GUIDE_LV5_PAGES + MAX_GUIDE_LV6_PAGES + MAX_GUIDE_LV7_PAGES + MAX_GUIDE_LV8_PAGES;
#endif

	activePage = 0;
	currentPageIndex = 0;

	btnDownTimer = -1;
	
	BtnPrevPage = guideScene->GetObjectByID("BtnPrev");
	BtnPrevPage->PlayAnimation(0);
	BtnNextPage = guideScene->GetObjectByID("BtnNext");
	BtnNextPage->PlayAnimation(0);

	ChapterGlow = NULL;

	isPreviewActive = false;
	isPreviewTransitioning = false;
	isPreviewZoomIn = false;

	PageTextLeft = NULL;
	PageTextRight = NULL;

	previewMaskBg = NULL;

	for (int i = 0; i < 6; i++)
	{
		Thumbs[i] = NULL;
	}

	ThumbPreview = NULL;

	StrategyGuideState = JRN_IDLE;
	PopupAlpha = 0.0f;
	PopVal = 0;
}

StrategyGuide::~StrategyGuide()
{
	if (guideScene->GetObjectByID("btncontentsText"))
	{
		guideScene->GetObjectByID("btncontentsText")->Show(false);
	}
	guideScene->Show(false);
	guideScene = NULL;

	Dispose();

	if( GFInstance != NULL )   
	{
		if( !GFInstance->IsPartialCacheMem )
		{
			GFInstance->UnloadBufferScene(GUIDE_SCN);
		}
	}
}

void StrategyGuide::Required()
{
	
}

void StrategyGuide::Update()
{
	if( !GFHud->IsGuideShowing )
		return;

	//For debug version only
#ifdef DEBUG
	if (KInput::isPressed(K_VK_A))
	{
		AssetsStringTable::readAssetsStringTable();
		AssetsStringTable::ReadBNRtextSizes();
		LoadCurrentPage();
	}
#endif

	Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	//guideScene->GetObjectByID("btncontentsText")->SetEnable(BtnContents->IsActive);

	switch (StrategyGuideState)
	{
		case JRN_POPUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);
			if( PopVal >= 1.0f )
			{
				PopupAlpha = 1.0f;
				StrategyGuideState = JRN_POPPED;
			}
			SetGuideAlpha(PopupAlpha);
			GFInstance->UpdateBlurScene(PopupAlpha);
			return;
		break;
		case JRN_POPDOWN:
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			SetGuideAlpha(PopupAlpha);
			GFInstance->UpdateBlurScene(PopupAlpha);
			if( PopupAlpha <= 0.113f )
			{
				PopupAlpha = 0.0f;
				GFInstance->TurnOffBlurScene();

				guideScene->Show(false);
				Dispose();

				StrategyGuideState = JRN_IDLE;
				GFHud->IsGuideShowing = false;
				GFInstance->IsActivePopup = false;
			}
			return;
		break;

		default:
		break;
	}

	if(isPreviewActive)
	{
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				BtnClosePreview->PlayAnimation(0);
				BtnClosePreview->FadeOut(4.0f);
				previewMaskBg->FadeOut(4.0f);
				float valArr[5] = {0, ThumbPreview->InitPos.x, ThumbPreview->InitPos.y, 0.15f, 15.0f};
				ThumbPreview->TweenTo(valArr, 0.45f, TW_EASEQUADOUT, false);
				isPreviewTransitioning = true;
				isPreviewZoomIn = false;	
			}
		#endif
		if(isPreviewTransitioning)
		{
			if(ThumbPreview!=NULL && ThumbPreview->IsTweenFinished())
			{
				ThumbPreview->IsTweening = false;
				isPreviewTransitioning = false;

				if(!isPreviewZoomIn)
				{
					ThumbPreview->SetEnable(false);
					BtnClosePreview->SetEnable(false);
					isPreviewActive = false;
				}
			}
		}
		else if((BtnClosePreview->Collide(&pos) || !PreviewBoundsRect.Intersection(&pos)) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnClosePreview->PlayAnimation(1);
			if(clicked)
			{
				BtnClosePreview->PlayAnimation(0);

				BtnClosePreview->FadeOut(4.0f);
				previewMaskBg->FadeOut(4.0f);

				float valArr[5] = {0, ThumbPreview->InitPos.x, ThumbPreview->InitPos.y, 0.15f, 15.0f};
				ThumbPreview->TweenTo(valArr, 0.45f, TW_EASEQUADOUT, false);

				isPreviewTransitioning = true;
				isPreviewZoomIn = false;	
			}
		}
		else
			BtnClosePreview->PlayAnimation(0);
	}
	else
	{
		#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed()) {
				HideStrategyGuide();
				return;
			}
		#endif
		for (int i = 0; i < numThumbsInPage; i++)
		{			
			if(Thumbs[i]!=NULL && Thumbs[i]->Collide(&pos))
			{
				Cursor::SetMode(CA_Look);
				if(clicked)
				{
					//Preview the Page
					std::string filepath = "";
					filepath = pageFilePath + "preview_pg"+MKSTR(currentPageIndex)+"_"+MKSTR(i)+".SPR";

					SAFE_DELETE(ThumbPreview);

					ThumbPreview = new CObject();
					ThumbPreview->LoadInit(filepath, Thumbs[i]->Pos.x+120, Thumbs[i]->Pos.y+70, BgZOrder+15);
					ThumbPreview->SetScale(0.15f);
					ThumbPreview->SetAlpha(0);
					ThumbPreview->SetRotate(15.0f);

					float valArr[5] = {1.0f, 690, 355, 1.0, 0.0f};
					ThumbPreview->TweenTo(valArr, 0.55f, TW_EASEQUADOUT, false);

					previewMaskBg->FadeIn(4.0f);

					BtnClosePreview->FadeIn(4.0f);
					BtnClosePreview->SetEnable(true);

					isPreviewActive = true;

					isPreviewTransitioning = true;
					isPreviewZoomIn = true;
				}
				return;
			}			
		}

		if(BtnPrevPage->IsActive && btnDownTimer<0 && BtnRectLeftButton.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			BtnPrevPage->PlayAnimation(1);
			if(clicked)
			{
				BtnPrevPage->PlayAnimation(2);
				btnDownTimer = 0.25f;

				activePage -= 1;
				LoadCurrentPage();
			}
		}
		else
		{
			if(btnDownTimer<0)
			{
				btnDownTimer = -1;
				BtnPrevPage->PlayAnimation(0);
			}
			else
				btnDownTimer -= Control::LogicRate;
		}

		if(BtnNextPage->IsActive && btnDownTimer<0 && BtnRectRightButton.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			BtnNextPage->PlayAnimation(1);
			if(clicked)
			{
				BtnNextPage->PlayAnimation(2);
				btnDownTimer = 0.25f;

				activePage += 1;
				
				LoadCurrentPage();
			}
		}
		else
		{
			if(btnDownTimer<0)
			{
				btnDownTimer = -1;
				BtnNextPage->PlayAnimation(0);
			}
			else
				btnDownTimer -= Control::LogicRate;
		}

		if(BtnContents->IsActive && btnDownTimer<0 && BtnContents->Collide(&pos,-4))
		{
			Cursor::SetMode(CA_HandPointy);
			BtnContents->PlayAnimation(1);
			if(clicked)
			{
				BtnContents->PlayAnimation(2);
				btnDownTimer = 0.25f;

				activePage = 0;
				
				LoadCurrentPage();
			}
		}
		else
		{
			if(btnDownTimer<0)
			{
				btnDownTimer = -1;
				BtnContents->PlayAnimation(0);
			}
			else
				btnDownTimer -= Control::LogicRate;
		}
		
		#ifdef K_ANDROID
			if (!GFApp->isUnlocked()) //Free Build then this returns true
				{
					if(activePage == 0)
						{
							if(ChapterGlow!=NULL)
								ChapterGlow->SetEnable(false);

							if(ChapterL1Rect.Intersection(&pos))
							{
								if(ChapterGlow!=NULL)
								{
									ChapterGlow->SetEnable(true);
									ChapterGlow->SetY(ChapterL1Rect.Top()-10); 
								}

								Cursor::SetMode(CA_HandPointy);
								if(clicked)
								{
									activePage = 2;
									LoadCurrentPage();
								}
							}
							else if(ChapterCollectiblesRect.Intersection(&pos))
							{
								if(ChapterGlow!=NULL)
								{
									ChapterGlow->SetEnable(true);
									ChapterGlow->SetY(ChapterCollectiblesRect.Top()-10); 
								}

								Cursor::SetMode(CA_HandPointy);
								if(clicked)
								{
									activePage = maxGuidePages;
									LoadCurrentPage();
								} 
							}
						}
					
				}
			else
				{
					if(activePage == 0)
					{
						if(ChapterGlow!=NULL)
							ChapterGlow->SetEnable(false);

						if(ChapterL1Rect.Intersection(&pos))
						{
							if(ChapterGlow!=NULL)
							{
								ChapterGlow->SetEnable(true);
								ChapterGlow->SetY(ChapterL1Rect.Top()-10); 
							}

							Cursor::SetMode(CA_HandPointy);
							if(clicked)
							{
								activePage = 2;
								LoadCurrentPage();
							}
						}
						else if(ChapterL2Rect.Intersection(&pos))
						{
							if(ChapterGlow!=NULL)
							{
								ChapterGlow->SetEnable(true);
								ChapterGlow->SetY(ChapterL2Rect.Top()-10); 
							}

							Cursor::SetMode(CA_HandPointy);
							if(clicked)
							{
								activePage = MAX_GUIDE_LV1_PAGES;
								LoadCurrentPage();
							}
						}
						else if(ChapterL3Rect.Intersection(&pos))
						{
							if(ChapterGlow!=NULL)
							{
								ChapterGlow->SetEnable(true);
								ChapterGlow->SetY(ChapterL3Rect.Top()-10); 
							}

							Cursor::SetMode(CA_HandPointy);
							if(clicked)
							{
								activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES;
								LoadCurrentPage();
							}
						}
						else if(ChapterL4Rect.Intersection(&pos))
						{
							if(ChapterGlow!=NULL)
							{
								ChapterGlow->SetEnable(true);
								ChapterGlow->SetY(ChapterL4Rect.Top()-10); 
							}

							Cursor::SetMode(CA_HandPointy);
							if(clicked)
							{
								activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES;
								LoadCurrentPage();
							}
						}
						else if(ChapterL5Rect.Intersection(&pos))
						{
							if(ChapterGlow!=NULL)
							{
								ChapterGlow->SetEnable(true);
								ChapterGlow->SetY(ChapterL5Rect.Top()-10); 
							}

							Cursor::SetMode(CA_HandPointy);
							if(clicked)
							{
								activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES;
								LoadCurrentPage();
							}
						}
						else if(ChapterL6Rect.Intersection(&pos))
						{
							if(ChapterGlow!=NULL)
							{
								ChapterGlow->SetEnable(true);
								ChapterGlow->SetY(ChapterL6Rect.Top()-10); 
							}

							Cursor::SetMode(CA_HandPointy);
							if(clicked)
							{
								activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES;
								LoadCurrentPage();
							}
						}
						else if(ChapterL7Rect.Intersection(&pos))
						{
							if(ChapterGlow!=NULL)
							{
								ChapterGlow->SetEnable(true);
								ChapterGlow->SetY(ChapterL7Rect.Top()-10); 
							}

							Cursor::SetMode(CA_HandPointy);
							if(clicked)
							{
								activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES+MAX_GUIDE_LV6_PAGES;
								LoadCurrentPage();
							}
						}
						else if(ChapterL8Rect.Intersection(&pos))
						{
							if(ChapterGlow!=NULL)
							{
								ChapterGlow->SetEnable(true);
								ChapterGlow->SetY(ChapterL8Rect.Top()-10); 
							}

							Cursor::SetMode(CA_HandPointy);
							if(clicked)
							{
								activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES+MAX_GUIDE_LV6_PAGES+MAX_GUIDE_LV7_PAGES;
								LoadCurrentPage();
							}
						}  
						else if(ChapterCollectiblesRect.Intersection(&pos))
						{	
							if(ChapterGlow!=NULL)
							{
								ChapterGlow->SetEnable(true);
								ChapterGlow->SetY(ChapterCollectiblesRect.Top()-10); 
							}

							Cursor::SetMode(CA_HandPointy);
							if(clicked)
							{
								activePage = maxGuidePages;
								LoadCurrentPage();
							} 
						}
					}
				}
		#else
			if(activePage == 0)
			{
				if(ChapterGlow!=NULL)
					ChapterGlow->SetEnable(false);

				if(ChapterL1Rect.Intersection(&pos))
				{
					if(ChapterGlow!=NULL)
					{
						ChapterGlow->SetEnable(true);
						ChapterGlow->SetY(ChapterL1Rect.Top()-10); 
					}

					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						activePage = 2;
						LoadCurrentPage();
					}
				}
				#ifndef _DEMOBUILD
					else if(ChapterL2Rect.Intersection(&pos))
					{
						if(ChapterGlow!=NULL)
						{
							ChapterGlow->SetEnable(true);
							ChapterGlow->SetY(ChapterL2Rect.Top()-10); 
						}

						Cursor::SetMode(CA_HandPointy);
						if(clicked)
						{
							activePage = MAX_GUIDE_LV1_PAGES;
							LoadCurrentPage();
						}
					}
					else if(ChapterL3Rect.Intersection(&pos))
					{
						if(ChapterGlow!=NULL)
						{
							ChapterGlow->SetEnable(true);
							ChapterGlow->SetY(ChapterL3Rect.Top()-10); 
						}

						Cursor::SetMode(CA_HandPointy);
						if(clicked)
						{
							activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES;
							LoadCurrentPage();
						}
					}
					else if(ChapterL4Rect.Intersection(&pos))
					{
						if(ChapterGlow!=NULL)
						{
							ChapterGlow->SetEnable(true);
							ChapterGlow->SetY(ChapterL4Rect.Top()-10); 
						}

						Cursor::SetMode(CA_HandPointy);
						if(clicked)
						{
							activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES;
							LoadCurrentPage();
						}
					}
					else if(ChapterL5Rect.Intersection(&pos))
					{
						if(ChapterGlow!=NULL)
						{
							ChapterGlow->SetEnable(true);
							ChapterGlow->SetY(ChapterL5Rect.Top()-10); 
						}

						Cursor::SetMode(CA_HandPointy);
						if(clicked)
						{
							activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES;
							LoadCurrentPage();
						}
					}
					else if(ChapterL6Rect.Intersection(&pos))
					{
						if(ChapterGlow!=NULL)
						{
							ChapterGlow->SetEnable(true);
							ChapterGlow->SetY(ChapterL6Rect.Top()-10); 
						}

						Cursor::SetMode(CA_HandPointy);
						if(clicked)
						{
							activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES;
							LoadCurrentPage();
						}
					}
					else if(ChapterL7Rect.Intersection(&pos))
					{
						if(ChapterGlow!=NULL)
						{
							ChapterGlow->SetEnable(true);
							ChapterGlow->SetY(ChapterL7Rect.Top()-10); 
						}

						Cursor::SetMode(CA_HandPointy);
						if(clicked)
						{
							activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES+MAX_GUIDE_LV6_PAGES;
							LoadCurrentPage();
						}
					}
					else if(ChapterL8Rect.Intersection(&pos))
					{
						if(ChapterGlow!=NULL)
						{
							ChapterGlow->SetEnable(true);
							ChapterGlow->SetY(ChapterL8Rect.Top()-10); 
						}

						Cursor::SetMode(CA_HandPointy);
						if(clicked)
						{
							activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES+MAX_GUIDE_LV6_PAGES+MAX_GUIDE_LV7_PAGES;
							LoadCurrentPage();
						}
					}  
				#endif // !_DEMOBUILD
				else if(ChapterCollectiblesRect.Intersection(&pos))
				{
					if(ChapterGlow!=NULL)
					{
						ChapterGlow->SetEnable(true);
						ChapterGlow->SetY(ChapterCollectiblesRect.Top()-10); 
					}

					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						activePage = maxGuidePages;
						LoadCurrentPage();
					} 
				}
			}
		#endif

		if((BtnRectClose.Intersection(&pos) || !StrategyGuideAreaRect.Intersection(&pos)) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnClose->PlayAnimation(1);
			if(clicked)
			{
				BtnClose->PlayAnimation(2);
				HideStrategyGuide();
			}
		}
		else
			BtnClose->PlayAnimation(0);
	}
}

float StrategyGuide::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void StrategyGuide::Dispose()
{
	//Clear Previous Pointers
	SAFE_DELETE(PageTextLeft);
	SAFE_DELETE(PageTextRight);
	SAFE_DELETE(ChapterGlow);

	SAFE_DELETE(PageNo);
	SAFE_DELETE(PageTEXT);

	for (int i = 0; i < 6; i++)
	{
		SAFE_DELETE(Thumbs[i]);
	}
	SAFE_DELETE(ThumbPreview);
}

void StrategyGuide::LoadCurrentPage()
{
	BtnNextPage->SetEnable(true);
	BtnPrevPage->SetEnable(true);

	BtnContents->SetEnable(true);
	guideScene->GetObjectByID("btncontentsText")->SetEnable(true);
	if(activePage<=1)
	{
		BtnContents->SetEnable(false);
		guideScene->GetObjectByID("btncontentsText")->SetEnable(false);
	}

	if(activePage<=0)
	{
		activePage = 0;
		BtnPrevPage->SetEnable(false);
	}
#ifdef _CEBUILD
	#ifdef _DEMOBUILD
		else if(activePage>=(maxGuidePages))
		{
			activePage = (maxGuidePages);
			BtnNextPage->SetEnable(false);
		}
	#else
		#ifdef K_ANDROID
			else if(activePage>=(maxGuidePages + ((!GFApp->isUnlocked())?0:MAX_GUIDE_COLLECTABLES_PAGES-1))) // decide what to use based on Free or Full version
			{
				activePage = (maxGuidePages + ((!GFApp->isUnlocked())?0:MAX_GUIDE_COLLECTABLES_PAGES-1));
				BtnNextPage->SetEnable(false);
			}	
		#else
			else if(activePage>=(maxGuidePages+MAX_GUIDE_COLLECTABLES_PAGES-1))
			{
				activePage = (maxGuidePages+MAX_GUIDE_COLLECTABLES_PAGES-1);
				BtnNextPage->SetEnable(false);
			}
		#endif
	#endif 
#else
	else if(activePage>=(maxGuidePages-1))
	{
		activePage = (maxGuidePages-1);
		BtnNextPage->SetEnable(false);
	}
#endif 

	Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

	Dispose();

	std::string filepath = "";
	setPageFilePath();

	

#ifdef _CEBUILD
	if(activePage>(maxGuidePages-1))
	{
		numThumbsInPage = Num_Extras_Thumbs[currentPageIndex];
	}
	else
	{
		numThumbsInPage = Num_Thumbs[activePage];
	}
#else
	numThumbsInPage = Num_Thumbs[activePage];
#endif

	CPoint leftTextPos(282, 86, 0);	

	if(ChapterGlow!=NULL)
		SAFE_DELETE(ChapterGlow);


	ColorRGB diarytextColor = { 0.1015f, 0.1718f, 0.2656f };
	int _lineSpacing = 40;

	PageNo = new CBitmapText();
	PageNo->SetZ(BgZOrder + 10);
	PageNo->LoadFont(eFont_Diary);
	PageNo->SetText("-"+to_string(activePage - 1)+"-");
	PageNo->SetAlignment(Align_Center);
	PageNo->SetColor(diarytextColor.r, diarytextColor.g, diarytextColor.b);
	PageNo->SetXY(470 - (PageNo->StringWidth() / 2), 640);


	if (PageTEXT != NULL)
	{
		//jrnTxtL->Show(false);
		delete PageTEXT;
		PageTEXT = NULL;
	}
	PageTEXT = new MultilineTextBox(AssetsStringTable::getString(std::string("guideP" + to_string(activePage - 1)).c_str()), leftTextPos.x, leftTextPos.y, BgZOrder + 4, diarytextColor, eFont_Diary, _lineSpacing);

	//Load current Page
	if(activePage == 0)
	{
		ChapterGlow = new CObject();
		ChapterGlow->LoadInit(GUIDEPATH+"chapterglow.SPR", ChapterL1Rect.Left(), 0, BgZOrder+3, false);

		//filepath = GUIDEPATH + "guide_txt_toc1.SPR";

		//Show Table of Contents
		//PageTextLeft = new CObject();
		//PageTextLeft->LoadInit(filepath, leftTextPos.x, leftTextPos.y, BgZOrder+4);

		/*filepath = GUIDEPATH + "guide_txt_toc2.SPR";

		PageTextRight = new CObject();
		PageTextRight->LoadInit(filepath, 702, 86, BgZOrder+4);*/

		PageTEXT = new MultilineTextBox(AssetsStringTable::getString("guideTOC"), leftTextPos.x+10, leftTextPos.y + 50, BgZOrder + 4, diarytextColor, eFont_Diary, 41);
		PageTEXT->Show(true);

		PageNo->Show(false);
	}
	else if(activePage == 1)
	{
		//filepath = GUIDEPATH + "general_tips.SPR";

		//Show Table of Contents
		//PageTextLeft = new CObject();
		//PageTextLeft->LoadInit(filepath, leftTextPos.x, leftTextPos.y, BgZOrder+4);
		
		/*filepath = GUIDEPATH + "guide_txt_toc2.SPR";

		PageTextRight = new CObject();
		PageTextRight->LoadInit(filepath, 702, 86, BgZOrder+4);*/	
		PageTEXT = new MultilineTextBox(AssetsStringTable::getString("guideTips"), leftTextPos.x + 10, leftTextPos.y + 50, BgZOrder + 4, diarytextColor, eFont_Diary, 41);
		PageTEXT->Show(true);
		PageNo->Show(false);
	}
	else
	{
		bool isCollectablesPage = false;
		#ifdef _CEBUILD
			if(activePage>(maxGuidePages-1))
			{
				currentPageIndex += 1;
				isCollectablesPage = true;
			}
		#endif

		//filepath = pageFilePath + "guide_txt_pg"+MKSTR(currentPageIndex)+".SPR";

		//Show Left Text Page
		//PageTextLeft = new CObject();
		//PageTextLeft->LoadInit(filepath, leftTextPos.x, leftTextPos.y, BgZOrder+4);

		PageTEXT->Show(true);
		PageNo->Show(true);

		//Load Thumbnails
		CPoint ThumbPos(828, 68, 0);
		float thumbGap = 148.0f;
		float thumbWidth = 205.0f;

		if(isCollectablesPage)
		{
			thumbGap = 132;
			ThumbPos.SetXY(693,160);
		}

		for (int i = 0; i < numThumbsInPage; i++)
		{
			filepath = pageFilePath + "thumb_pg"+MKSTR(currentPageIndex)+"_"+MKSTR(i)+".SPR";

			//Page Thumbs
			Thumbs[i] = new CObject();
			if(isCollectablesPage && ((i+1)%2==0))
				Thumbs[i]->LoadInit(filepath, ThumbPos.x+thumbWidth, ThumbPos.y, BgZOrder+4);
			else
				Thumbs[i]->LoadInit(filepath, ThumbPos.x, ThumbPos.y, BgZOrder+4);

			if(isCollectablesPage)
			{
				if((i+1)%2==0)
					ThumbPos.y += thumbGap;
			}
			else
			{
				ThumbPos.y += thumbGap;
			}
		}
	}
}

void StrategyGuide::setPageFilePath()
{
	currentPageIndex = activePage;

#ifdef _CEBUILD
	if(activePage>(maxGuidePages-1))
	{
		pageFilePath = GUIDEPATH+"EXTRAS\\";
		currentPageIndex = activePage-maxGuidePages;
		return;
	}
#endif

	if( activePage < LastPageIndex[0] )
	{
		pageFilePath = GUIDEPATH+"LV1\\";
		currentPageIndex = activePage-1; //General Tips minus
	}
	else if( activePage < LastPageIndex[1] )
	{
		pageFilePath = GUIDEPATH+"LV2\\";
		currentPageIndex = activePage - MAX_GUIDE_LV1_PAGES+1;
	}
	else if( activePage < LastPageIndex[2] )
	{
		pageFilePath = GUIDEPATH+"LV3\\";
		currentPageIndex = activePage - MAX_GUIDE_LV1_PAGES - MAX_GUIDE_LV2_PAGES +1;
	}
	else if( activePage < LastPageIndex[3] )
	{
		pageFilePath = GUIDEPATH+"LV4\\";
		currentPageIndex = activePage - MAX_GUIDE_LV1_PAGES - MAX_GUIDE_LV2_PAGES - MAX_GUIDE_LV3_PAGES +1;
	}
	else if( activePage < LastPageIndex[4] )
	{
		pageFilePath = GUIDEPATH+"LV5\\";
		currentPageIndex = activePage - MAX_GUIDE_LV1_PAGES - MAX_GUIDE_LV2_PAGES - MAX_GUIDE_LV3_PAGES - MAX_GUIDE_LV4_PAGES +1;
	}
	else if( activePage < LastPageIndex[5] )
	{
		pageFilePath = GUIDEPATH+"LV6\\";
		currentPageIndex = activePage - MAX_GUIDE_LV1_PAGES - MAX_GUIDE_LV2_PAGES - MAX_GUIDE_LV3_PAGES - MAX_GUIDE_LV4_PAGES - MAX_GUIDE_LV5_PAGES +1;
	}
	else if( activePage < LastPageIndex[6] )
	{
		pageFilePath = GUIDEPATH+"LV7\\";
		currentPageIndex = activePage - MAX_GUIDE_LV1_PAGES - MAX_GUIDE_LV2_PAGES - MAX_GUIDE_LV3_PAGES - MAX_GUIDE_LV4_PAGES - MAX_GUIDE_LV5_PAGES -MAX_GUIDE_LV6_PAGES +1;
	}
	else if( activePage < LastPageIndex[7] )
	{
		pageFilePath = GUIDEPATH+"LV8\\";
		currentPageIndex = activePage - MAX_GUIDE_LV1_PAGES - MAX_GUIDE_LV2_PAGES - MAX_GUIDE_LV3_PAGES - MAX_GUIDE_LV4_PAGES - MAX_GUIDE_LV5_PAGES -MAX_GUIDE_LV6_PAGES -MAX_GUIDE_LV7_PAGES +1;
	}
}

void StrategyGuide::SetGuideAlpha(float alpha)
{
	guideScene->SetSceneAlpha(alpha);

	if(PageTextLeft!=NULL)
		PageTextLeft->SetAlpha(alpha);

	if(PageTextRight!=NULL)
		PageTextRight->SetAlpha(alpha);

	for (int i = 0; i < 6; i++)
	{
		if(Thumbs[i]!=NULL)
			Thumbs[i]->SetAlpha(alpha);
	}
}

void  StrategyGuide::SetActiveGuidePageToCurrentScene()
{
	activePage = 0; //Table of Contents, by Default
	if(activeLevel == Level_1)
	{
		//Level 1 - demo
		switch (ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
				default: break;
				case eArea_L1_S1_Beach:
				case eArea_L1_S1_PZKidsPainting:	activePage = 2; break;
						
				case eArea_L1_S2_Frontyard:			activePage = 3; break;
				case eArea_L1_S2_PZFuse:			activePage = 5; break;
				case eArea_L1_S3_Foyer:				activePage = 6; break;
				case eArea_L1_S3_PZShelf:			activePage = 7; break;
				case eArea_L1_S3_DoorHO:			activePage = 8; break;

				case eArea_L1_S4_PZDoor:			activePage = 12; break;
				case eArea_L1_S4_Bedroom:			activePage =9; break;
				case eArea_L1_S4_PZPainting:		activePage = 10; break;
				case eArea_L1_S5_TreasureRoom:		activePage = 13; break;

				case eArea_L1_S5_PZDynamiteMaking:
				case eArea_L1_S5_TunnelDoor:		activePage = 16; break;
				
				case eArea_L1_S5_HOTreasureRoom:	activePage = 14; break;
				case eArea_L1_S6_HOBoatShed:		activePage = 21; break;
				case eArea_L1_S6_BoatShed:			activePage = 19; break;
				case eArea_L1_S7_BoatDock:			activePage = 18; break;
		}
	}
	else if(activeLevel == Level_2)
	{
		activePage = MAX_GUIDE_LV1_PAGES;
		switch (ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
				default: break;
				case eArea_L2_S1_ForestEntrance:	activePage += 0; break; //n-1
				case eArea_L2_S1_UndergroundHO:		activePage += 1; break;

				case eArea_L2_S2_NagaTemple:
				case eArea_L2_S2_PZMango:
				case eArea_L2_S2_HONagaTemple:		activePage += 3; break;
	
				case eArea_L2_S3_CaveMouth:
				case eArea_L2_S3_PZStoneBeam:		activePage += 4; break;

				case eArea_L2_S4_Cave:
				case eArea_L2_S4_PZDeities:			activePage += 5; break;

				case eArea_L2_S5_MoopansDen:
				case eArea_L2_S5_PZEagleUnblock:	activePage += 6; break;
				case eArea_L2_S5_PZMedicine_Making:	activePage += 7; break;

				case eArea_L2_S6_TunnelEnd:
				case eArea_L2_S6_HOWaterfall:
				case eArea_L2_S6_PZTunnelClock:		activePage += 9; break;

				case eArea_L2_S7_PZSoldiers:		activePage += 10; break;
		}
	}
	else if(activeLevel == Level_3)
	{
		activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES;
		switch (ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
				default: break;
				case eArea_L3_S1_FortEntrance:
				case eArea_L3_S1_HOLadder:			activePage += 0; break; //n-1

				case eArea_L3_S1_PZLionHead:		activePage += 1; break;

				case eArea_L3_S2_FortInterior:		activePage += 2; break;

				case eArea_L3_S2_P1Room1Start:		activePage += 4; break;
				case eArea_L3_S2_P1Room2Wine:		activePage += 5; break;
				case eArea_L3_S2_P1Room3Lock:		activePage += 6; break;
				case eArea_L3_S2_P1Room4Match3:		activePage += 7; break;
				case eArea_L3_S2_P1Room5Pipe:		activePage += 8; break;
				case eArea_L3_S2_P1Room6Pillar:		activePage += 9; break;

				case eArea_L3_S2_P1DeadEnd:			activePage += 4; break;

				case eArea_L3_S3_Pool:
				case eArea_L3_S3_PZBlocks:
				case eArea_L3_S3_PZUnderwater:		activePage += 10; break;

				case eArea_L3_S4_FortFountain:
				case eArea_L3_S4_FortFountainHO:	activePage += 11; break;
				case eArea_L3_S5_Cliff:				activePage += 12; break;
		}
	}
	else if(activeLevel == Level_4)
	{
		activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES;
		switch (ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
				default: break;
				case eArea_L4_S1_SwampEntrance:
				case eArea_L4_S1_ShoeHO:			activePage += 0; break; //n-1
				case eArea_L4_S2_RiverSide:
				case eArea_L4_S2_PZAlligatorHead:
				case eArea_L4_S2_PZDoor:			activePage += 3; break;
				case eArea_L4_S3_Cave:				activePage += 7; break;
				case eArea_L4_S3_PZFireMaking:		activePage += 8; break;
				case eArea_L4_S3_PZDoor:			activePage += 7; break;
				case eArea_L4_S4_HillTop:			
				case eArea_L4_S4_BinocularHO:		activePage += 4; break;
				case eArea_L4_S5_HillSide:			
				case eArea_L4_S5_PZSwapTile:		activePage += 5; break;
		}
	}
	else if(activeLevel == Level_5)
	{
		activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES;
		switch (ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
				default: break;
				case eArea_L5_S1_TigerDistant:		activePage += 0; break; //n-1
				case eArea_L5_S2_TigerCloseup:
				case eArea_L5_S2_PZBlocks:			activePage += 1; break;
				case eArea_L5_S3_Pond:
				case eArea_L5_S3_PZDungeonDoor:
				case eArea_L5_S3_HOUnderwater:		activePage += 2; break;
				case eArea_L5_S4_Dungeon:			activePage += 3; break;
				case eArea_L5_S4_PZHandCuff:		activePage += 4; break;
				case eArea_L5_S5_MountainBelow:
				case eArea_L5_S5_HOGanapathi:
				case eArea_L5_S5_PZSunClock:		activePage += 5; break;
				case eArea_L5_S6_MountainTop:		activePage += 6; break;
				case eArea_L5_S6_HOTree:			activePage += 7; break;
		}
	}
	else if(activeLevel == Level_6)
	{
		activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES;
		switch (ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
				default: break;
				case eArea_L6_S1_FloatingIsland1:
				case eArea_L6_S1_DescriptionHO:		activePage += 0; break; //n-1
				case eArea_L6_S2_FloatingIsland2:
				case eArea_L6_S2_ShereHO:			activePage += 1; break;
				case eArea_L6_S2_PZPineGum:			activePage += 2; break;
				case eArea_L6_S3_FloatingIsland3:	
				case eArea_L6_S3_PZPathway:			activePage += 3; break;
				case eArea_L6_S3_GlowingHeartHO:	activePage += 4; break;
				case eArea_L6_S4_PortalChest:
				case eArea_L6_S4_PortalPuzzle:
				case eArea_L6_S4_FloatingIsland4:	activePage += 5; break;
				case eArea_L6_S5_MarysDream:
				case eArea_L6_S5_TabMiniGame:		activePage += 6; break;
		}
	}
	else if(activeLevel == Level_7)
	{
		activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES+MAX_GUIDE_LV6_PAGES;
		switch (ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
				default: break;
				case eArea_L7_S1_Volcano:
				case eArea_L7_S1_PZJigsawYellow:
				case eArea_L7_S1_PZStarPuzzle:	activePage += 0; break; //n-1
				case eArea_L7_S2_LasyaPrison:		
				case eArea_L7_S2_HORightHand:
				case eArea_L7_S2_HOLeftHand:
				case eArea_L7_S2_HOBrokenRightHand:
				case eArea_L7_S2_HOBrokenLeftHand:
				case eArea_L7_S2_PZEyePuzzle:
				case eArea_L7_S2_PZLasyaStatue:		activePage += 1; break;
				case eArea_L7_S3_Haven:			
				case eArea_L7_S3_PZDoorPuzzle:
				case eArea_L7_S3_PZJigsawRed:		activePage += 2; break;
				case eArea_L7_S4_Ameeha:	
				case eArea_L7_S4_PZJigsawBlue:
				case eArea_L7_S5_SophiaEnd:		activePage += 3; break;
		}
	}
	else if(activeLevel == Level_8)
	{
		activePage = MAX_GUIDE_LV1_PAGES+MAX_GUIDE_LV2_PAGES+MAX_GUIDE_LV3_PAGES+MAX_GUIDE_LV4_PAGES+MAX_GUIDE_LV5_PAGES+MAX_GUIDE_LV6_PAGES+MAX_GUIDE_LV7_PAGES;
		switch (ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
				default: break;
				case eArea_L8_S1_ShukrasPlace:
				case eArea_L8_S1_PZDoorHandPuzzle:
				case eArea_L8_S1_DoorHO:			activePage += 0; break; //n-1
				case eArea_L8_S2_Garden:			activePage += 1; break; //n-1
				case eArea_L8_S3_BanyanTree:		
				case eArea_L8_S3_BanyanTreeHO:		activePage += 2; break; //n-1
				case eArea_L8_S4_Pool:
				case eArea_L8_S4_PZPoolGame:		activePage += 3; break; //n-1
				case eArea_L8_S5_VolcanoLand:
				case eArea_L8_S5_VirtualMiniGame:	activePage += 4; break; //n-1
		}
	}
}

void StrategyGuide::ShowStrategyGuide()
{
	SetActiveGuidePageToCurrentScene();

	LoadCurrentPage();

	if(previewMaskBg == NULL)
	{
		previewMaskBg = new CObject();
		previewMaskBg->LoadSprite("UI\\MASK.SPR");
		previewMaskBg->SetXY(683,384);
		previewMaskBg->SetScale(2.0);
		previewMaskBg->SetZ(BgZOrder + 10);
		previewMaskBg->Show(false);
	}

	PopVal = 0;
	PopupAlpha = 0.0f;

	SetGuideAlpha(PopupAlpha);
	guideScene->Show(true);

	StrategyGuideState = JRN_POPUP;
	GFInstance->CreateGreyscaleScene();
}


void StrategyGuide::HideStrategyGuide()
{
	SAFE_DELETE(previewMaskBg);

	StrategyGuideState = JRN_POPDOWN;
	Control::Audio->QuickLoadAndPlaySFX(aSFXDiaClose);
	PageTEXT->Show(false);

	PageNo->Show(false);
}
