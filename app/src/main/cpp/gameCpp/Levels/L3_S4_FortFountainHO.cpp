//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 04 : Fountain HO
//====================================

#include "L3_S4_FortFountainHO.h"
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

#define NUMBER_OF_HO 16

const CRectangle  Match3BtnRect(550.0f,1.0f,252.0f,39.0f);

const char* itemNames[] = {"Lantern",
	"Seeds",
	"Ink_Bottle", 
	"Candle", 
	"Oil_Can", 
	"Mortar", 
	"Key", 
	"Door_Knob", 
	"Shoe", 
	"Sword", 
	"Bracelet", 
	"Eye", 
	"Broken_Heart", 
	"Tusk", 
	"Arrow_Head", 
	"Nail"};

/*--------------L3S4HiddenObject-------------------*/

L3S4HiddenObject::L3S4HiddenObject()
{
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_isAnimComplete = false;
	_startPoint = NULL;
	_endPoint = NULL;
	_ctrlPoint1 = NULL;
	_ctrlPoint2 = NULL;
	_objectPos = CPoint();

	_name = "";
	_nameText = NULL;
	_toFind = NULL;
	_completeObject = NULL;
	_polygonArea = NULL;
	_isFound = false;
	hoScaleTimer = hoFlyingTimer = 0;

	HOTrailFX = NULL;
	waterHOFX = NULL;
}

L3S4HiddenObject::~L3S4HiddenObject()
{
	_startPoint = NULL;
	_endPoint = NULL;
	SAFE_DELETE(_ctrlPoint1);
	SAFE_DELETE(_ctrlPoint2);
	_name = "";
	SAFE_DELETE(_nameText);
	_toFind = NULL;
	_completeObject = NULL;
	SAFE_DELETE(_polygonArea);
	_isFound = false;
	SAFE_DELETE(HOTrailFX);
	SAFE_DELETE(waterHOFX);
}

void L3S4HiddenObject::InitObject()
{
	InitInvNameText();
	_objectPos = _toFind->Pos;
	_startPoint = &_toFind->Pos;
	_endPoint = &_nameText->Pos;
	_ctrlPoint1 = new CPoint(_startPoint->x, (_startPoint->y + _endPoint->y)/3.0f, 0);
	_ctrlPoint2 = new CPoint(_endPoint->x, (_ctrlPoint1->y + _endPoint->y)*0.5f, 0);

	_ctrlPoint1->y += (200 + rand() % 200);
	_ctrlPoint2->y -= (200 + rand() % 200);

	_completeObject->SetEnable(false);

	HOTrailFX = NULL;
	HOTrailFX = new SuperFX("PRT\\L2\\HOTrailFX.INI", PointSystem::CreateCPoint(1065,122,0), 50);

	waterHOFX = new SuperFX("PRT\\L2\\waterHO.INI", PointSystem::CreateCPoint(564,368,0), 1100); 
	waterHOFX->SetPos(_startPoint);
}

void L3S4HiddenObject::InitInvNameText()
{
	CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);
	float vertGap = 26, horizontalGap = InvAreaRect.w/4.0f;
	CPoint pos(0,0,0);

	pos.x = InvAreaRect.Left() + horizontalGap*0.5f + horizontalGap*(_index%4);
	pos.y = InvAreaRect.Top() + vertGap*0.5f + vertGap*(_index/4);

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	_nameText = new CBitmapText();
	_nameText->SetTextProps(_name, pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_Small, Align_Center);
	_nameText->SetZ(eZOrder_INVBOXITEMS);
}


bool L3S4HiddenObject::MouseClicked(CPoint pos)
{
	if (_isFound)
	{
		return false;
	}

	if (_polygonArea->Intersection(&pos))
	{
		OnFound();
		return true;
	}

	return false;
}

void L3S4HiddenObject::OnFound()
{
	_toFind->FadeOut(3.0f);
	_completeObject->SetEnable(true);
	_completeObject->FadeIn(4.0f);
	_completeObject->SetZ(eZOrder_INVBOXITEMS);
	_isFound = true;
	waterHOFX->Start();
	_isObjectScaling = true;
}

void L3S4HiddenObject::Update()
{
	if (_isAnimComplete || !_isFound)
		return;

	if(_isObjectScaling)
	{
		//scaling anim
		hoScaleTimer += Control::LogicRate;
		_completeObject->SetScale(0.85f+Elixir::Sin(90+hoScaleTimer*360.0f)*0.15f);
		if(hoScaleTimer > 1.0f)
		{
			//start flying
			_isObjectScaling = false;
		}
	}
	else
	{
		if(!HOTrailFX->IsEmitting)
			HOTrailFX->Start();

		FXTimer += 1;
		if(FXTimer%3==0)
			HOTrailFX->Emit();

		//flying to HUD
		hoFlyingTimer += Control::LogicRate;

		CPoint temp = Elixir::GetCubicBezierPoint(_startPoint, _ctrlPoint1, _ctrlPoint2, _endPoint, hoFlyingTimer);
		_completeObject->SetPos(&temp);
		_completeObject->SetScale(1.0f-hoFlyingTimer*0.75f);
		HOTrailFX->SetPos(&temp);

		if(hoFlyingTimer>1.0f)
		{
			HOTrailFX->StopEmit();
			_nameText->SetStrikethrough(true);
			_completeObject->SetEnable(false);
			_isAnimComplete = true;
		}
	}
}

void L3S4HiddenObject::AlreadyFound()
{
	_isFound = true;
	_isAnimComplete = true;
	_toFind->SetEnable(false);
	_completeObject->SetEnable(false);
	InitInvNameText();
	_nameText->SetAlpha(0.5f);
}

void L3S4HiddenObject::SwitchMatch3()
{
	_nameText->Show(false);
}

void L3S4HiddenObject::SwitchHO()
{
	_nameText->Show(true);
}

/*--------------L3_S4FortFoundationHO-------------------*/

L3_S4FortFoundationHO::L3_S4FortFoundationHO()
{
	Visible = false;
	musicFadeOutTimer = 0;
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S4\\HO\\L3S4FORTFOUDATIONHO.SCN");

	CObject* light = HOScene->GetObjectByID("Light");
	light->Animate(0.4f, 0.8f, 5);
	light->SetBlendMode(eBlend_AlphaAdditive);

	InitPolygonArray();
	InitHO();

	GFInstance->PostUpdate(2);

	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s4FountainHO,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s4FountainHO) )		
	{
		Control::Audio->PlaySample(aAmbL3s4FountainHO,true);
	}
	
	//MUSIC_SFX
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L3_S4_FortFountainHO] = true;

	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;

	_match3Ctrl = NULL;


	match3Back = new CObject();
	match3Back->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
	match3Back->SetUID("match3Back");
	match3Back->Show(false);
	HOScene->PushObject(match3Back);

	match3BackText = new CObject();
	match3BackText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
	match3BackText->SetUID("match3BackText");
	match3BackText->Show(false);
	HOScene->PushObject(match3BackText);
#ifndef M3_BUTTON
	match3Back->SetEnable(false);
    match3BackText->SetEnable(false);
#endif
	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		match3Back->SetEnable(false);
		match3BackText->SetEnable(false);
	}
	_btnMatch3 = new CObject();
	_btnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 680, 21, 34);
	_btnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(_btnMatch3);

	_btnSwitchToHO = new CObject();
	_btnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 680, 21, 35);
	_btnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(_btnSwitchToHO);

	//_btnMatch3 = HOScene->GetObjectByID("btnMatch3");
	_btnMatch3->PlayAnimation(0);
	//_btnSwitchToHO = HOScene->GetObjectByID("btnSwitchToHO");
	_btnSwitchToHO->Show(false);
	_btnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);

	S3FortFountaionHOLightFX = new SuperFX("PRT\\L3\\S3FortFountaionHOLight.INI", PointSystem::CreateCPoint(690,83,0), 1100); 
	S3FortFountaionHOLightFX->Start();
	S3FortFountaionHOLightFX->AdvanceByTime(2.0f);

	S3FortFountaionHOLeafFX = new SuperFX("PRT\\L3\\S3FortFountaionHOLeaf.INI", PointSystem::CreateCPoint(593,-4,0), 1100); 
	S3FortFountaionHOLeafFX->Start();
	S3FortFountaionHOLeafFX->AdvanceByTime(2.0f);

	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	Visible = true;	//To render anything with renderlistentity

	_gameState = State_Initilizing;
}

void L3_S4FortFoundationHO::InitHO()
{
	for (int i = 0; i < NUMBER_OF_HO; i++)
	{
		CObject* toFind = HOScene->GetObjectByID(std::string("hidden_object_")+MKSTR(i+1));
		CObject* completed = HOScene->GetObjectByID(std::string("complete_object_")+MKSTR(i+1));

		L3S4HiddenObject *obj = new L3S4HiddenObject();
		obj->setIndex(i);
		obj->setName(StringTable::getStr(itemNames[i]));
		obj->setToFind(toFind);
		obj->setCompleteObject(completed);
		obj->setPolygonArea(HOPolygonArray[i]);
		_hiddenObjs.push_back(obj);

		if (ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainFound[i])
		{
			obj->AlreadyFound();
			_foundObjs.push_back(obj);
			continue;
		}
		obj->InitObject();
	}
}


L3_S4FortFoundationHO::~L3_S4FortFoundationHO()
{
	Hud->InvBoxCtrl->ResetToInvBox();

	for (int i = 0; i < NUMBER_OF_HO; i++)
	{
		delete _hiddenObjs[i];
	}

	_hiddenObjs.clear();
	_foundObjs.clear();

	SAFE_DELETE(S3FortFountaionHOLightFX);
	SAFE_DELETE(S3FortFountaionHOLeafFX);

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s4FountainHO);
	Control::Audio->UnloadSample(aAmbL3s4FountainHO);
	//MUSIC_SFX
}

void L3_S4FortFoundationHO::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL3theme) )
	{
		Control::Audio->Samples[aTrackL3theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL3theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL3theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL3ho) )
				  Control::Audio->PlaySample(aTrackL3ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL3ho))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL3ho, true);
		}
	}

}

float L3_S4FortFoundationHO::easing(float t,float b,float c,float d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
}

void L3_S4FortFoundationHO::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;
}

void L3_S4FortFoundationHO::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
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
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S4_FortFountain;
		SendMessage(ControlExit, ID, HOSM_NewArea);
	}	
}

void L3_S4FortFoundationHO::ProcessNonHOHint()
{
	if (_gameState == State_StartMatch3)
	{
		return;
	}

	bool haveHint = false;

	int size = (int)_hiddenObjs.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		if (!_hiddenObjs[i]->getIsFound())
		{
			_gameState = State_OnHint;
			haveHint = true;
			InitHintTrailEmit(_hiddenObjs[i]->getToFind()->Pos.x, _hiddenObjs[i]->getToFind()->Pos.y);
			AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			break;
		}
	}

	if (!haveHint)
		Hud->ShowNothingToDoMsgBanner();
}

void L3_S4FortFoundationHO::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if(!_btnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		_btnMatch3->Show(false);
		_btnSwitchToHO->Show(false);
	}

	int size = (int)_foundObjs.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		_foundObjs[i]->Update();
	}

	_btnSwitchToHO->PlayAnimation(0);
	_btnMatch3->PlayAnimation(0);
	
	if (_gameState < State_Match3Completed && Match3BtnRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);

		if (ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		{
			_btnSwitchToHO->PlayAnimation(1);

			if (Control::Input->LBclicked())
			{
				_match3Ctrl->clearBoard();
				_btnMatch3->Show(true);
				_btnSwitchToHO->Show(false);

				for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
				{
					_hiddenObjs[i]->SwitchHO();
				}

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_gameState = State_StopMatch3;
			}
		}
		else
		{
			_btnMatch3->PlayAnimation(1);

			if (Control::Input->LBclicked())
			{
				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(true);

				if (!_match3Ctrl)
				{
					_match3Ctrl = new Match3(7);
					AddControl(_match3Ctrl);
				}
				else
				{
					_match3Ctrl->ShowBoard();
				}

				for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
				{
					_hiddenObjs[i]->SwitchMatch3();
				}
				
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;
				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				_gameState = State_StartMatch3;
			}
		}

	}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(_match3Ctrl!=NULL)
					_match3Ctrl->clearBoard();

				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S4_FortFountain;
			SendMessage(ControlExit, ID, HOSM_NewArea);							
		}	
		#endif
	if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay && ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if( Control::Input->LBclicked() )
		{
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(_match3Ctrl!=NULL)
					_match3Ctrl->clearBoard();

				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S4_FortFountain;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	if( IsMouseOverGoBackArea(&pos)
		#ifdef M3_BUTTON
		&& _btnMatch3->Visible
        #endif
        )
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L3_S4_FortFountain);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L3_S4_FortFountain);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L3_S4_FortFountain, 2);
			}
#else
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(_match3Ctrl!=NULL)
					_match3Ctrl->clearBoard();

				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S4_FortFountain;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
	}

	switch ((GameState)_gameState)
	{
	case State_Initilizing:
		{
			if (_hiddenObjs.size() == _foundObjs.size())
			{
				_gameState = State_HOCompleted;
			}
			else
			{
				_gameState = State_GameStarted;
			}
		}
		break;

	case State_GameStarted:
		{
			_gameState = State_WaitForInteraction;
		}
		break;

	case State_WaitForInteraction:
		{
			if( Control::Input->LBclicked() )
			{
				_gameState = State_OnInteraction;			
			}
		}
		break;

	case State_OnInteraction:
		{
			bool stateChange = false;

			for (int i = 0; i < NUMBER_OF_HO; i++)
			{
				if (_hiddenObjs[i]->MouseClicked(pos))
				{
					_foundObjs.push_back(_hiddenObjs[i]);
					ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainFound[i] = true;
					_gameState = State_FoundObject;
					stateChange = true;

					Control::Audio->QuickLoadAndPlaySFX("s4ho_objectsclicked");
					break;
				}
			}

			if (!stateChange)
			{
				_gameState = State_WaitForInteraction;
			}
		}
		break;

	case State_OnHint:
		{
			_gameState = State_WaitForInteraction;
		}
		break;

	case State_FoundObject:
		{
			if (_foundObjs.size() == _hiddenObjs.size())
			{
				_gameState = State_FoundAllObjects;
			}
			else
			{
				_gameState = State_WaitForInteraction;
			}
		}
		break;

	case State_FoundAllObjects:
		{
			_gameState++; // waiting to complete the animation
		}
		break;

	case State_Match3Completed:
	case State_HOCompleted:
		{
			if (_handleExtrasGameComplete(false))
			{
				_gameState = State_L3S4GameInExtrasComplete;
				return;
			}

			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsHOScene = false;

			Hud->TakeInventoryItem(eInv_L3S4_Nail);
			for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
			{
				_hiddenObjs[i]->SwitchMatch3();
			}
			ProfileSystem::Instance->ActiveProfile.L3_S4_HOFortFountainCompleted = true;
			AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			_gameState = State_GameComplete;
		}
		break;

	case State_StartMatch3:
		{
			if (_match3Ctrl->getIsCompleted())
			{
				for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
				{
					_hiddenObjs[i]->SwitchHO();
				}

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_gameState = State_Match3Completed;
			}
		}

		break;

	case State_StopMatch3:
		_gameState = State_WaitForInteraction;
		break;

	case State_GameComplete:
		break;

	case State_L3S4GameInExtrasComplete:
		break;

	default:
		_gameState++;
		break;
	}
}

void L3_S4FortFoundationHO::InitPolygonArray()
{
int PtsAr1[] = {331,542,366,519,370,532,401,529,470,609,464,636,413,634,360,617};
CPolygon* CPolyObj1=  new CPolygon(8);
CPolyObj1->Init(PtsAr1);
HOPolygonArray[0] = CPolyObj1;

int PtsAr2[] = {197,115,213,136,248,124,246,104,208,95};
CPolygon* CPolyObj2=  new CPolygon(5);
CPolyObj2->Init(PtsAr2);
HOPolygonArray[1] = CPolyObj2;

int PtsAr3[] = {849,199,874,213,909,201,911,165,898,141,863,139,848,164};
CPolygon* CPolyObj3=  new CPolygon(7);
CPolyObj3->Init(PtsAr3);
HOPolygonArray[2] = CPolyObj3;

int PtsAr4[] = {606,113,644,47,679,70,640,137};
CPolygon* CPolyObj4=  new CPolygon(4);
CPolyObj4->Init(PtsAr4);
HOPolygonArray[3] = CPolyObj4;

int PtsAr5[] = {822,387,842,404,890,387,898,368,874,325,859,317,840,242,826,244,837,321,822,334};
CPolygon* CPolyObj5=  new CPolygon(10);
CPolyObj5->Init(PtsAr5);
HOPolygonArray[4] = CPolyObj5;

int PtsAr6[] = {181,239,219,281,273,264,284,220,262,199,261,181,232,172,220,201,191,212};
CPolygon* CPolyObj6=  new CPolygon(9);
CPolyObj6->Init(PtsAr6);
HOPolygonArray[5] = CPolyObj6;

int PtsAr7[] = {683,512,686,521,702,515,727,522,744,511,736,495,773,486,780,495,794,494,795,474,773,483,735,491,717,488,705,495};
CPolygon* CPolyObj7=  new CPolygon(14);
CPolyObj7->Init(PtsAr7);
HOPolygonArray[6] = CPolyObj7;

int PtsAr8[] = {528,169,579,167,597,145,580,112,554,118,548,138,531,134};
CPolygon* CPolyObj8=  new CPolygon(7);
CPolyObj8->Init(PtsAr8);
HOPolygonArray[7] = CPolyObj8;

int PtsAr9[] = {329,496,378,502,430,479,435,465,360,437,330,432,274,406,262,422,276,451,308,438,301,463,322,475};
CPolygon* CPolyObj9=  new CPolygon(12);
CPolyObj9->Init(PtsAr9);
HOPolygonArray[8] = CPolyObj9;

int PtsAr10[] = {1029,493,1078,418,1120,378,1129,383,1097,449,1035,530,1029,496,1017,514,1023,543,1002,583,992,598,986,598,959,646,942,656,934,649,934,627,950,619,968,591,966,577,970,564,977,567,1012,515};
CPolygon* CPolyObj10=  new CPolygon(22);
CPolyObj10->Init(PtsAr10);
HOPolygonArray[9] = CPolyObj10;

int PtsAr11[] = {482,527,481,540,498,541,517,521,514,505,501,503,501,489,525,491,535,512,521,541,494,557,474,559,463,537};
CPolygon* CPolyObj11=  new CPolygon(13);
CPolyObj11->Init(PtsAr11);
HOPolygonArray[10] = CPolyObj11;

int PtsAr12[] = {1089,159,1095,169,1107,153,1120,158,1135,157,1151,139,1155,105,1141,111,1134,107,1103,130,1103,138};
CPolygon* CPolyObj12=  new CPolygon(11);
CPolyObj12->Init(PtsAr12);
HOPolygonArray[11] = CPolyObj12;

int PtsAr13[] = {345,153,323,125,329,108,345,106,351,116,350,148,354,154,356,144,357,123,371,113,382,114,385,130,356,158};
CPolygon* CPolyObj13=  new CPolygon(13);
CPolyObj13->Init(PtsAr13);
HOPolygonArray[12] = CPolyObj13;

int PtsAr14[] = {580,490,565,506,568,523,588,526,643,515,682,492,696,467,709,424,700,425,689,436,670,466,643,484,597,493};
CPolygon* CPolyObj14=  new CPolygon(13);
CPolyObj14->Init(PtsAr14);
HOPolygonArray[13] = CPolyObj14;

int PtsAr15[] = {1038,293,1041,301,1102,292,1103,280,1118,270,1106,257,1096,260,1088,248};
CPolygon* CPolyObj15=  new CPolygon(8);
CPolyObj15->Init(PtsAr15);
HOPolygonArray[14] = CPolyObj15;

int PtsAr16[] = {823,132,840,138,847,134,845,124,899,89,895,82,839,116,826,112};
CPolygon* CPolyObj16=  new CPolygon(8);
CPolyObj16->Init(PtsAr16);
HOPolygonArray[15] = CPolyObj16;
}
void L3_S4FortFoundationHO::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);

				if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
				{
					if(_match3Ctrl!=NULL)
						_match3Ctrl->clearBoard();

					_btnMatch3->Show(false);
					_btnSwitchToHO->Show(false);
					ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				}

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