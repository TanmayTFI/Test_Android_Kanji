//====================================
//DQFULP
// Extras Menu
// Hidden Object System
//====================================

#include "Application.h"
#include "ExtrasComicControl.h"
#include "HOScene.h"

#include "GameFlow.h"
#include "MenuSystem.h"

#include "Fonts.h"
#include "Util.h"
#include "Resources.h"

const CRectangle  CenterPageRect(428.0f,58.0f,547.0f,668.0f);
const CRectangle  BackButtonRect(178.0f,5.0f,223.0f,72.0f);

const CRectangle  NextPageRect(998.0f,78.0f,217.0f,660.0f);
const CRectangle  PrevPageRect(206.0f,87.0f,152.0f,645.0f);

const int MAX_COMIC_PAGES = 24;

ExtrasComicControl::ExtrasComicControl()
{
	PopupAlpha = 0;
	comicState = eComic_Inactive;

	pageIndex = 0;

	pageTurnTimer = 0;

	_closeComicCallback = nullptr;

	bazeZOrder = 100;
	_maskSpr = NULL;
	ExtrasComicScn = NULL;

	centerPos.SetXY(663,412);
	leftPagePos.SetXY(433,387);
	rightPagePos.SetXY(938,385);

	//_JAnim_PageTurn = NULL;

	backButton = NULL;
	backBtnTxt = NULL;

	pageLeft = NULL;
	pageRight = NULL;

	newLeftPage = NULL;
	newRightPage = NULL;

	isPageTurning = false;
	isActive = false;
}

void ExtrasComicControl::OnAnimationComplete(int animID)
{
	isPageTurning = false;

	if(newLeftPage!=NULL)
	{
		newLeftPage->SetAlpha(1.0f);
		SAFE_DELETE(pageLeft);
		SAFE_DELETE(pageRight);

		pageLeft = newLeftPage;
		pageRight = newRightPage;

		newRightPage = NULL;
		newLeftPage = NULL;
	}
}


ExtrasComicControl::~ExtrasComicControl()
{
	SAFE_DELETE(pageLeft);
	SAFE_DELETE(pageRight);

	SAFE_DELETE(_maskSpr);
	SAFE_DELETE(ExtrasComicScn);

	//SAFE_DELETE(_JAnim_PageTurn);

	_closeComicCallback = nullptr;
}

void ExtrasComicControl::Required()
{
	
}

void ExtrasComicControl::Update()
{
	if(!isActive)
		return;

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
			Control::Audio->QuickLoadAndPlaySFX(aSFXCrystal);
			comicState = eComic_FadingOut;
			return;
		}
	#endif

	Cursor::SetMode(CA_Normal);
	
	switch (comicState)
	{
	case eComic_Inactive:
		break;
	case eComic_FadingIn:
		PopupAlpha += 4.0f * Control::LogicRate;

		if( PopupAlpha >= 1.0f )
		{
			comicState = eComic_Active;
			PopupAlpha = 1.0f;			
		}
		setAllAlpha(PopupAlpha);
		break;
	case eComic_Active:
		updateComic();
		break;
	case eComic_FadingOut:
		PopupAlpha -= 4.0f * Control::LogicRate;
		if( PopupAlpha <= 0.0f )
		{
			PopupAlpha = 0.0f;
			dispose();
			if(_closeComicCallback!=nullptr)
				_closeComicCallback();
		}
		else
		{
			setAllAlpha(PopupAlpha);
		}
		break;
	default:
		break;
	}
}

void ExtrasComicControl::updateComic()
{
	CPoint pos = Control::Input->Mpos();
	// bool clicked = Control::Input->LBclicked();	 //Sac: Unused variable, so commented it

	if(isPageTurning)
	{
		pageTurnTimer += Control::LogicRate;
		if(pageTurnTimer>0.65f)
		{
			pageTurnTimer = 0;
			isPageTurning = false;

			if(newLeftPage!=NULL)
			{
				SAFE_DELETE(pageLeft);
				SAFE_DELETE(pageRight);

				pageLeft = newLeftPage;
				pageRight = newRightPage;

				newRightPage = NULL;
				newLeftPage = NULL;
			}
		}
		return;
	}

	if(backButton)
		backButton->PlayAnimation(0);
	if( BackButtonRect.Intersection(&pos) )
	{
		if(backButton)
			backButton->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			if(backButton)
				backButton->PlayAnimation(0);

			Control::Audio->QuickLoadAndPlaySFX(aSFXCrystal);
			comicState = eComic_FadingOut;
			return;
		}
	}	
		

	if(pageIndex==0)
	{
		if( CenterPageRect.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXCrystal);
				//_JAnim_PageTurn->PlayAnim();
				isPageTurning = true;

				pageLeft->FadeOut(1.5f);

				pageIndex += 1;
				LoadCurrentPages();
			}
		}
	}
	else
	{
		if( NextPageRect.Intersection(&pos) )
		{
			if(pageIndex<(MAX_COMIC_PAGES-1))
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXCrystal);
					//_JAnim_PageTurn->PlayAnim();
					isPageTurning = true;

					pageLeft->FadeOut(1.5f);
					pageRight->FadeOut(1.5f);				

					if(pageIndex==0)
						pageIndex += 1;
					else
						pageIndex += 2;

					LoadCurrentPages();
				} 
			}
		}
		else if( PrevPageRect.Intersection(&pos) )
		{
			if(pageIndex>0)
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXCrystal);

					if(pageIndex==1)
						pageIndex -= 1;
					else
					{
						pageLeft->FadeOut(1.5f);
						pageRight->FadeOut(1.5f);
						isPageTurning = true;
						pageIndex -= 2;
					}

					LoadCurrentPages();
				} 
			}
		}
	}
}

void ExtrasComicControl::setAllAlpha(float alpha)
{
	_maskSpr->SetAlpha(alpha);
	ExtrasComicScn->SetSceneAlpha(alpha);

	if(pageLeft!=NULL)
		pageLeft->SetAlpha(alpha);

	if(pageRight!=NULL)
		pageRight->SetAlpha(alpha);
}

void ExtrasComicControl::dispose()
{
	isActive = false;
	isPageTurning = false;

	comicState = eComic_Inactive;

	SAFE_DELETE(_maskSpr);
	SAFE_DELETE(ExtrasComicScn);

	//SAFE_DELETE(_JAnim_PageTurn);

	SAFE_DELETE(pageLeft);
	SAFE_DELETE(pageRight);
}

void ExtrasComicControl::LoadCurrentPages(bool isQuickShow)
{
	const std::string COMICSPATH = "LOCALIZED\\" + StringTable::GetMyLangauge() + "\\EXTRAS\\COMIC\\COMICPG";
	std::string filePath = "";
	if(pageIndex==0)
	{
		if(isQuickShow)
		{
			SAFE_DELETE(pageLeft);
			SAFE_DELETE(pageRight);
		}

		SAFE_DELETE(pageLeft);
		SAFE_DELETE(pageRight);

		filePath = COMICSPATH +MKSTR(pageIndex)+".SPR";
		pageLeft = new CObject();
		pageLeft->LoadInit(filePath, centerPos.x, centerPos.y, bazeZOrder+10);
		pageLeft->SetAlpha(0);

		ExtrasComicScn->GetObjectByID("bookopenbg")->SetEnable(false);

		if(isQuickShow)
			pageLeft->SetAlpha(1);
		else
			pageLeft->FadeIn(2.0f);
	}
	else
	{
		int leftPageIndex = pageIndex;
		int rightPageIndex = 1+pageIndex;

		filePath = COMICSPATH +MKSTR(leftPageIndex)+".SPR";

		newLeftPage = new CObject();
		newLeftPage->LoadInit(filePath, leftPagePos.x, leftPagePos.y, bazeZOrder+10);
		newLeftPage->SetAlpha(0);

		if(isQuickShow)
			newLeftPage->SetAlpha(1);

		filePath = COMICSPATH +MKSTR(rightPageIndex)+".SPR";

		newRightPage = new CObject();
		newRightPage->LoadInit(filePath, rightPagePos.x, rightPagePos.y, bazeZOrder+10);
		newRightPage->SetAlpha(0);

		if(isQuickShow)
			newRightPage->SetAlpha(1);
		else
		{
			newRightPage->FadeIn(2.0f);
			newLeftPage->FadeIn(2.0f);
		}

		if(!ExtrasComicScn->GetObjectByID("bookopenbg")->IsActive)
		{
			ExtrasComicScn->GetObjectByID("bookopenbg")->SetEnable(true);
			ExtrasComicScn->GetObjectByID("bookopenbg")->FadeIn(1.5f);
		}

		if(isQuickShow)
		{
			SAFE_DELETE(pageLeft);
			SAFE_DELETE(pageRight);

			pageLeft = newLeftPage;
			pageRight = newRightPage;

			newRightPage = NULL;
			newLeftPage = NULL;
		}
	}
}

void ExtrasComicControl::ShowComicScene()
{
	

	if(_maskSpr==NULL)
	{
		_maskSpr = new CObject();
		_maskSpr->LoadSprite("UI\\MASK.SPR");
		_maskSpr->SetXY(683,384);
		_maskSpr->SetScale(2.0);
		_maskSpr->SetZ(bazeZOrder);
	}

	_maskSpr->SetAlpha(0.0f);

	ExtrasComicScn = new CHOScene();
	ExtrasComicScn->Load("EXTRAS\\COMIC\\COMIC.SCN");	
	ExtrasComicScn->SetZOff(bazeZOrder+2);

	ExtrasComicScn->GetObjectByID("bg")->SetScale(2.5f);

	ExtrasComicScn->GetObjectByID("bookopenbg")->SetEnable(false);
	
	ExtrasComicScn->GetObjectByID("pagepeel0")->SetEnable(false);
	ExtrasComicScn->GetObjectByID("pagepeel1")->SetEnable(false);
	ExtrasComicScn->GetObjectByID("pagepeel2")->SetEnable(false);

	backButton = ExtrasComicScn->GetObjectByID("btnback");
	backBtnTxt = new CObject();
	backBtnTxt->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", backButton->Pos.x, backButton->Pos.y, backButton->ZOrder + 1);
	backBtnTxt->SetUID("backBtnTxt");
	ExtrasComicScn->PushObject(backBtnTxt);
	
	ExtrasComicScn->SetSceneAlpha(0);

	//_JAnim_PageTurn = new JSONAnimator(0,CC_CALLBACK_1(ExtrasComicControl::OnAnimationComplete,this));
	//_JAnim_PageTurn->parseAndLoadJSON("EXTRAS\\COMIC\\PAGETURN.JSON", ExtrasComicScn);

	pageIndex = 0;
	LoadCurrentPages();
    
	PopupAlpha = 0.0f;
	comicState = eComic_FadingIn;	
	isActive = true;
}
