//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 04 : BINOCULAR HO
//====================================

#include "L4_S4_BinocularHO.h"
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
#include "exMath.h"
#include "Circle.h"
#include <math.h>

#define CURRENT_Y_CHECK_LIMIT 600 //originally was hardcoded 

const char* BannerTexts[] = {"L4earl",
	"L4dee", 
	"L4rode", 
	"L4butterf", 
	"L4oliv",
	"L4cros"};

int SolPointX[NUMBER_OF_HO] = {85, -1284, -2265, -1684, -918, -413};
int SolPointY[NUMBER_OF_HO] = {-201, -473, -639, -1251, -412, -1267};

const CRectangle  BinocularRect(296.0f,139.0f,745.0f,394.0f);



L4_S4_BinocularHO::L4_S4_BinocularHO()
{
	if(GFApp->CurrentResolution.Height >= 2160)//4K
	{
		X_LOWER_LIMIT = -4200;
		X_UPPER_LIMIT = 303;
		Y_LOWER_LIMIT = -1820;
		Y_UPPER_LIMIT = 0;
	}
	else //everything else
	{
		X_LOWER_LIMIT = -2793;
		X_UPPER_LIMIT = 303;
		Y_LOWER_LIMIT = -1620;
		Y_UPPER_LIMIT = 133;
	}

	_hintText		= NULL;
	sophiaClickArea	= NULL;

	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Visible = false;
	FoundSophiaTimer = 0.0f;
	isMovingBinocular = false;
	wasPrevMouseDown = false;
	isFoundSophia = false;
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S4\\HO\\HO.SCN");

	CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);
	float vertGap = 26, horizontalGap = InvAreaRect.w/4.0f;
	CPoint pos(0,0,0);

	pos.x = InvAreaRect.Left() + horizontalGap*2;
	pos.y = InvAreaRect.Top() + vertGap*2;

	int ArrayPts[] = {518,300,616,316,636,417,532,434,484,371};
	sophiaClickArea =  new ClickablePolygon(0, 5, ArrayPts, CC_CALLBACK_1(L4_S4_BinocularHO::OnSophiaPolygonClick, this));

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	_hintText = new CBitmapText();
	_hintText->SetTextProps(StringTable::getStr("Exresemble"), pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_25, Align_Center);
	_hintText->SetZ(eZOrder_INVBOXITEMS);

	for (int i = 0; i < 9; i++)
	{
		BgScn[i] = new CHOScene();
		BgScn[i]->Load(std::string("LV4\\S4\\HO\\BG0") + MKSTR(i + 1) + std::string(".SCN"));

		for (int j = 0; j < NUMBER_OF_HO; j++)
		{
			CObject * obj = NULL;
			obj = BgScn[i]->GetObjectByID(std::string("found_obj_outline_0")+MKSTR(j));
			if (obj != NULL)
			{
				BlueOutLineSymbols[j] = obj;
				obj->SetAlpha(0);
			}
		}
		BgScn[i]->SetZOff(-1);
		BgScn[i]->SetPosOff((float)((i % 3) * 1366),(float)((i / 3) * 768));
	}
	InitPolygonArray();

	GFInstance->PostUpdate(2);

	Control::Audio->ClearUnusedTracks();

	currentPos.x = 0;
	currentPos.y = 0;

    targetX = 0;
    targetY = 0;

	#if defined (K_ANDROID) || defined (K_iOS)
		isTouchScreen = true;
	#else
		isTouchScreen = false;
	#endif

	/*if( !Control::Audio->IsPlaying(aTrackLevel2) )
	Control::Audio->PlaySample(aTrackLevel2,true);*/

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L4_S4_BinocularHO] = true;

	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;

	Visible = true;	//To render anything with renderlistentity
	numObjectsFound = 0;

	for (int i = 0; i < NUMBER_OF_HO; i++)
	{
		OutLineSymbols[i] = HOScene->GetObjectByID(std::string("hidden_obj_outline_0")+MKSTR(i));
		if (i)
		{
			OutLineSymbols[i]->SetPos(OutLineSymbols[0]->GetPosPointer());
		}

		if (ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularFound[i])
			numObjectsFound++;

		OutLineSymbols[i]->SetEnable(false);
		_hiddenObjs.push_back(OutLineSymbols[i]);
	}
	if (numObjectsFound < NUMBER_OF_HO)
		OutLineSymbols[numObjectsFound]->SetEnable(true);

	_compassBase = HOScene->GetObjectByID("CompassBase");
	_compassBase->Show(false);
	_compassNeedle = HOScene->GetObjectByID("CompassNeedle");
	_compassNeedle->Show(false);

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aVOL4MaryRefS4_2,	AudioVO);
	Control::Audio->LoadSample(aAmbL4s4BinocularHOAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s4BinocularHOAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s4BinocularHOAmb,true);
	}
	if (numObjectsFound == NUMBER_OF_HO)
	{
		Hud->InvBox_StopPuzzleMode();
		_hintText->Show(false);
		for (int i = 0; i < 9; i++)
		{
			BgScn[i]->SetPosOff(-413, -1267);
		}
	}
	//MUSIC_SFX
}

L4_S4_BinocularHO::~L4_S4_BinocularHO()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	//Cursor::Show(true);

	for (int i = 0; i < 9; i++)
	{
		SAFE_DELETE(BgScn[i]);
	}

	_hiddenObjs.clear();

	for (int i = 0; i < NUMBER_OF_HO; i++)
	{
		SAFE_DELETE(HOPolygonArray[i]);
	}

	/*_hintText->Show(false);*/

	SAFE_DELETE(_hintText);
	SAFE_DELETE(sophiaClickArea);

	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL4MaryRefS4_2);
	Control::Audio->StopSample(aAmbL4s4BinocularHOAmb);
	Control::Audio->UnloadSample(aAmbL4s4BinocularHOAmb);
	//MUSIC_SFX
}

void L4_S4_BinocularHO::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL4theme) )
	{
		Control::Audio->Samples[aTrackL4theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL4theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL4theme);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL4ho) )
				Control::Audio->PlaySample(aTrackL4ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL4ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL4ho, true);
		}
	}
}

float L4_S4_BinocularHO::easing(float t,float b,float c,float d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
}

void L4_S4_BinocularHO::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;
}

void L4_S4_BinocularHO::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		//TaskPendingUpdate();
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
		// back from puzzle scene
	}	
}

void L4_S4_BinocularHO::ProcessNonHOHint()
{
	if (numObjectsFound < NUMBER_OF_HO)
	{
		_compassBase->Show(true);
		_compassNeedle->Show(true);
		InitHintTrailEmit(&_compassNeedle->Pos);
		AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	}
	else
	{
		InitHintTrailEmit(&((CPolygon*)(sophiaClickArea))->center);
		AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	}
}

void L4_S4_BinocularHO::OnObjectFadeInComplete(CObject * obj)
{
	float tweenVals[5] = {0, -1, -1, -1, -1};
	obj->TweenTo(tweenVals, 0.5, TW_EASE, true, CC_CALLBACK_1(L4_S4_BinocularHO::OnObjectFadeOutComplete, this));
}

void L4_S4_BinocularHO::OnObjectFadeOutComplete(CObject* obj)
{
	if (numObjectsFound == NUMBER_OF_HO)
	{
		Cursor::Show(true);			
	}
	else
	{
		OutLineSymbols[numObjectsFound]->SetEnable(true);
		OutLineSymbols[numObjectsFound]->SetAlpha(0);
		float tweenVals[5] = {1, -1, -1, -1, -1};
		OutLineSymbols[numObjectsFound]->TweenTo(tweenVals, 0.5, TW_EASE, true);
	}
}

void L4_S4_BinocularHO::Update() {
	NavigateToScene();
    if (Hud->PopupState != Popup_Inactive)
        return;

    if (Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem || GFInstance->IsActivePopup ||
        Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
        return;

    if (ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularCompleted)
        return;


    if (isFoundSophia && FoundSophiaTimer < 3.0f) {
        FoundSophiaTimer += Control::LogicRate;
        if (FoundSophiaTimer >= 3.0f) {
            FoundSophiaTimer = 3.0f;
            OnFoundSophia();
            return;
        }
    }

	float dx = 0;
	float dy = 0;

    if(!isTouchScreen)
        currentPos = Control::Input->Mpos();

    #ifdef ENABLE_ANDROID_BACK_BUTTON
        if (Control::Input->isKeyBackSensed())
        {
            Hud->InvBoxCtrl->ResetToInvBox();
            ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S4_HillTop;
            SendMessage(ControlExit, ID, HOSM_NewArea);
        }
    #endif

    _pos = Control::Input->Mpos();

#ifdef TOUCH_UI
	if(IsMouseOverGoBackArea(&_pos))
#else
	if(!isMovingBinocular && IsMouseOverGoBackArea(&_pos))
#endif
	{
		if (numObjectsFound == NUMBER_OF_HO)
		{
			if (isFoundSophia)
			{
				
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L4_S4_HillTop);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L4_S4_HillTop);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S4_HillTop, 2);
			}
#else			
			Hud->InvBoxCtrl->ResetToInvBox();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S4_HillTop;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
					
					return;
				}
			}
		}
		else
		{
			//Cursor::Show(true);
			
			#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L4_S4_HillTop);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L4_S4_HillTop);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S4_HillTop, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				Hud->InvBoxCtrl->ResetToInvBox();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S4_HillTop;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
					
				return;
			}
		}

	}
#ifdef TOUCH_UI
	else if (currentPos.y < CURRENT_Y_CHECK_LIMIT)
#else
		else if (currentPos.y < CURRENT_Y_CHECK_LIMIT && !isMovingBinocular)
#endif
	{
		if(numObjectsFound < NUMBER_OF_HO)
		{
			Cursor::SetMode(CA_Look);
		}
		else
		{
			Cursor::SetMode(CA_Normal);
		}
	}
	else
	{
		Cursor::SetMode(CA_Normal);
	}


	if (numObjectsFound == NUMBER_OF_HO)
	{
		if (sophiaClickArea->GetCurrentMouseState() == eMouseState_Over)
		{
			Cursor::SetMode(CA_Help);
		}	 
	}
#ifndef TOUCH_UI
	if(Control::Input->LBclicked())
	{
		if (numObjectsFound != NUMBER_OF_HO)
		{
			if (isMovingBinocular)
			{
				isMovingBinocular = false;
				Cursor::Show(true);
			}
			else
			{
				if (currentPos.y < CURRENT_Y_CHECK_LIMIT)
				{
					isMovingBinocular = true;
					Cursor::Show(false);
				}
			}
		}

	}
#else
	if (numObjectsFound != NUMBER_OF_HO)
		isMovingBinocular = true;
#endif
	

	if (isMovingBinocular)
	{

		Cursor::Show(false);

        if(isTouchScreen)
		{
			if(Control::Input->LBdown())
			{
				if(!LBPressed)
				{
					LBPressed = true;
					prevPos.x = KInput::getMouseX();
					prevPos.y = KInput::getMouseY();
				}
				else
				{
					mouseDx = KInput::getMouseX() - prevPos.x;
					mouseDy = KInput::getMouseY() - prevPos.y;

					if(mouseDx < -50)
						mouseDx = -50;
					if(mouseDy < -50)
						mouseDy = -50;
					if(mouseDx > 50)
						mouseDx = 50;
					if(mouseDy > 50)
						mouseDy = 50;

					currentPos.x += (float)(mouseDx * 0.5);
					currentPos.y += (float)(mouseDy * 0.5);

					if(currentPos.x < 0)
						currentPos.x = 0;
					if(currentPos.y < 0)
						currentPos.y = 0;
					if(currentPos.x > 1000)
						currentPos.x = 1000;
					if(currentPos.y > CURRENT_Y_CHECK_LIMIT)
						currentPos.y = CURRENT_Y_CHECK_LIMIT;

					prevPos.x = KInput::getMouseX();
					prevPos.y = KInput::getMouseY();
				}
			}
			else
			{
				LBPressed = false;
			}
		}

	    //Sac changed this to accomadate Higher screen resolutions also...
		//if(GFApp->CurrentResolution.Height >= 768)
			//currentPos.x -= (GFApp->CurrentResolution.Width<1366)?((1366 - GFApp->CurrentResolution.Width) / 2.0f): 0.0f;
		//currentPos.x -= ((1366 - GFApp->CurrentResolution.Width) / 2.0f);
		float varX = currentPos.x / GFApp->CurrentResolution.Width;
		float varY = currentPos.y / GFApp->CurrentResolution.Height;
		int tempXLowerLimit = (GFApp->CurrentResolution.Width<1366)? X_LOWER_LIMIT: X_LOWER_LIMIT-GFApp->CurrentResolution.Width; //Sac: added to support Bigger than 1366 resolutions also
		int tempYLowerLimit =(GFApp->CurrentResolution.Height<768)? Y_LOWER_LIMIT:Y_LOWER_LIMIT-GFApp->CurrentResolution.Height; //Sac: added to support Bigger than 768 resolutions also
		targetX = Elixir::Lerp(X_UPPER_LIMIT, (float)tempXLowerLimit,  varX);
		//float targetX = Elixir::Lerp(X_UPPER_LIMIT, X_LOWER_LIMIT,  varX);
		targetY = Elixir::Lerp(Y_UPPER_LIMIT, (float)tempYLowerLimit, varY);

		if (targetX < tempXLowerLimit)
		{
			targetX =(float)tempXLowerLimit;
		}
		if (targetY < tempYLowerLimit)
		{
			targetY = (float)tempYLowerLimit;
		}
		if (targetX > X_UPPER_LIMIT)
		{
			targetX = X_UPPER_LIMIT;
		}
		if (targetY > Y_UPPER_LIMIT)
		{
			targetY = Y_UPPER_LIMIT;
		}

		dx = BgScn[0]->GetObjectByID("BG01")->GetPosPointer()->x - targetX;
		dy = BgScn[0]->GetObjectByID("BG01")->GetPosPointer()->y - targetY;
		if(targetX > X_LOWER_LIMIT && targetX < X_UPPER_LIMIT)
		{
			for (int i = 0; i < 9; i++)
			{
				wasPrevMouseDown = true;
				BgScn[i]->SetPosOff(-dx, 0);
			}
		}
		if(targetY > Y_LOWER_LIMIT && targetY < Y_UPPER_LIMIT)
		{
			for (int i = 0; i < 9; i++)
			{
				wasPrevMouseDown = true;
				BgScn[i]->SetPosOff(0, -dy);
			}
		}

		prevPos.x = KInput::getMouseX();
        prevPos.y = KInput::getMouseY();

	}

	if (dx < 1.0f && dy < 1.0f)
	{
		if (abs( BgScn[0]->GetObjectByID("BG01")->GetPosPointer()->x - SolPointX[numObjectsFound]) < 10 && abs( BgScn[0]->GetObjectByID("BG01")->GetPosPointer()->y - SolPointY[numObjectsFound]) < 10)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4ho_itemFound");
			OutLineSymbols[numObjectsFound]->SetEnable(false);

			float tweenVals[5] = {1, -1, -1, -1, -1};
			BlueOutLineSymbols[numObjectsFound]->TweenTo(tweenVals, 0.5, TW_EASE, true, CC_CALLBACK_1(L4_S4_BinocularHO::OnObjectFadeInComplete, this));

			ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularFound[numObjectsFound] = true;
			Hud->ShowBannerText(BannerTexts[numObjectsFound]);
			numObjectsFound++;
			_compassBase->Show(false);
			_compassNeedle->Show(false);
			if (numObjectsFound == NUMBER_OF_HO)
			{
				Hud->InvBox_StopPuzzleMode();
				_hintText->Show(false);

				isMovingBinocular = false;

			}
		}
	}

	if (numObjectsFound < NUMBER_OF_HO)
	{
		float angle = Elixir::Angle(OutLineSymbols[numObjectsFound]->GetPosPointer(), BlueOutLineSymbols[numObjectsFound]->GetPosPointer());
		_compassNeedle->SetRotate(180 -(angle - 90));
		float distance = Elixir::Distance(OutLineSymbols[numObjectsFound]->GetPosPointer(), BlueOutLineSymbols[numObjectsFound]->GetPosPointer());
		float alpha = distance / 100.0f;
		_compassNeedle->SetAlpha(alpha);
	}
}

void L4_S4_BinocularHO::OnSophiaPolygonClick(int id)
{
	if (numObjectsFound == NUMBER_OF_HO
	#ifndef TOUCH_UI
	&& Cursor::Visible
    #endif
	&& !isFoundSophia)
	{
		Hud->ShowBannerText("L4hurry");
		Control::Audio->PlaySample(aVOL4MaryRefS4_2);
		isFoundSophia = true;
	}
}

void L4_S4_BinocularHO::OnFoundSophia()
{
	if(_handleExtrasGameComplete(false))
		return;
	Hud->ReleaseDrag();
	Hud->CloseInventory(); 
	Hud->InvBoxCtrl->ResetToInvBox();
	AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_Binocular] = false;
	Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_CB_Binocular); 
	ProfileSystem::Instance->ActiveProfile.L4_S4_HOBinocularCompleted = true;
	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S4_HillTop;
	SendMessage(ControlExit, ID, HOSM_NewArea);	
}

void L4_S4_BinocularHO::InitPolygonArray()
{
	int PtsAr2[] = {259,37,238,58,223,95,229,103,257,97,281,49,258,34};//crow
	CPolygon* CPolyObj2=  new CPolygon(7);
	CPolyObj2->Init(PtsAr2);
	HOPolygonArray[0] = CPolyObj2;

	int PtsAr6[] = {701,275,709,257,702,257,699,264,692,266,691,251,683,258,687,279,694,296,710,293,698,273};//deer
	CPolygon* CPolyObj6=  new CPolygon(11);
	CPolyObj6->Init(PtsAr6);
	HOPolygonArray[1] = CPolyObj6;

	int PtsAr7[] = {1204,321,1217,314,1224,317,1237,312,1251,322,1277,337,1253,330,1216,344,1215,328,1201,322};//rat
	CPolygon* CPolyObj7=  new CPolygon(10);
	CPolyObj7->Init(PtsAr7);
	HOPolygonArray[2] = CPolyObj7;

	int PtsAr11[] = {841,558,863,538,864,569,847,575,833,569,818,546,839,554};//buttrfly
	CPolygon* CPolyObj11=  new CPolygon(7);
	CPolyObj11->Init(PtsAr11);
	HOPolygonArray[3] = CPolyObj11;

	int PtsAr12[] = {395,518,357,537,379,621,422,600,429,587,394,514};//door
	CPolygon* CPolyObj12=  new CPolygon(6);
	CPolyObj12->Init(PtsAr12);
	HOPolygonArray[4] = CPolyObj12;

	int PtsAr10[] = {504,231,528,228,542,218,558,228,573,216,590,228,627,217,633,255,609,268,588,255,570,274,589,303,578,316,563,325,543,319,525,304,533,278,546,297,547,265,519,268,503,256,501,230};
	CPolygon* CPolyObj10=  new CPolygon(22);//olive tree
	CPolyObj10->Init(PtsAr10);
	HOPolygonArray[5] = CPolyObj10;
}
void L4_S4_BinocularHO::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				//Cursor::SetMode(CA_ExitUp);

				Hud->InvBoxCtrl->ResetToInvBox();
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