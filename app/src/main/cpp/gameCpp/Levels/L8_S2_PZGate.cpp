
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8_S2_PZGate.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"
#include "exMath.h"

const CRectangle  GateKeyRect(612.0f,238.0f,151.0f,181.0f);
const CRectangle  SymbolBottomArea(615.0f,541.0f,150.0f,95.0f);
const CRectangle  PuzzleRect(346.0f,50.0f,681.0f,577.0f);


GateSymbol::GateSymbol(int id, int pos, CObject *symbol, CObject *bgSymbol,  CObject *glow)
{
	_id						= id;
	_pos					= pos;
	gateSymObj				= symbol;
	bgObject				= bgSymbol;
	glowObject				= glow;
	_state					= eGateSymbolState_Idle;
	_swapSymbol				= NULL;
	_swapBgObject			= NULL;
	_angleDiff				= 0;
	_angleIncrement			= 0;
	_direction				= 0;
	_nextPos				= 0;
	_SwapCompleteCallback	= nullptr;
}

float GateSymbol::GetRotate()
{
	return(bgObject->GetRotate() );
}


void GateSymbol::SetPosition(int posIndex, CPoint pos, CPoint symbolPos)
{
	_pos = posIndex;
	bgObject->SetPos(&pos);
	gateSymObj->SetPos( &symbolPos );
	glowObject->SetPos( &pos);
}

void GateSymbol::SetSymbolRotate( float rotation)
{
	bgObject->SetRotate( rotation);
	glowObject->SetRotate( rotation);
}

void GateSymbol::UpdateSymbolPos(CPoint symbolPos)
{
	//bgObject->SetPivot(x, y);
	gateSymObj->SetPos( &symbolPos );
}

void GateSymbol::SetOrigin(float x, float y)
{
	gateSymObj->SetPivot(x, y);
}


int GateSymbol::GetPositionIndex()
{
	return(_pos);
}

void GateSymbol::SetGlow(bool glowFlag)
{
	glowObject->SetPos(&bgObject->Pos);
	glowObject->SetEnable(glowFlag);
	_isSelected = glowFlag;
	SetHoverEffect(!glowFlag);
}

void GateSymbol::SetHoverEffect(bool glowFlag)
{
	if(glowFlag)
	{
		bgObject->SetScale(1.1f);
		gateSymObj->SetScale(1.1f);
	}
	else
	{
		bgObject->SetScale(1.0f);
		gateSymObj->SetScale(1.0f);
	}
}


void GateSymbol::SetPositionIndex(int pos)
{
	_pos = pos;
}

CRectangle GateSymbol::GetIntersectArea()
{
	return( gateSymObj->GetWorldRect());
}

int GateSymbol::GetId()
{
	return(_id);
}

bool GateSymbol::CheckIfSelected()
{
	return(_isSelected);
}


void GateSymbol::SetId(int Id)
{
	_id = Id;
}

void GateSymbol::DisableSymbol()
{
	if(gateSymObj != NULL )
		gateSymObj->SetEnable(false);
	if(bgObject != NULL )
		bgObject->SetEnable(false);
	if(glowObject != NULL )
		glowObject->SetEnable(false);
}


void GateSymbol::OnSkipActive(int swapId, int swapPos, std::function<void (int)> SwapCompleteCallback)
{
	_SwapCompleteCallback	= SwapCompleteCallback;
	_state					= eGateSymbolState_Skipping;
	_pos					= swapPos;
	_angleDiff  = abs( _id - swapId) * 36;
	if( swapId > _id )
			_direction			= 1;
	else
			_direction			= -1;
}

void GateSymbol::SwapSymbol(int swapId, int swapPos, CObject *swapSymbol, CObject *swapBgObject)
{
	Control::Audio->QuickLoadAndPlaySFX("s2pz_GateSwap");
	_swapSymbol				= swapSymbol;
	_swapBgObject			= swapBgObject;
	_state					= eGateSymbolState_Swapping;
	_angleDiff				= abs( _id - swapId);
	_pos					= swapPos;

	/*if( swapId > _id )
		_direction			= 1;
	else
		_direction			= -1;*/
	if( swapId > _id )
	{
		if( _angleDiff < 4 )
			_direction			= 1;
		else 
		{
			_angleDiff			= 10 - _angleDiff;
			_direction			= -1;
		}
	}
	else
	{
		if( _angleDiff < 4 )
			_direction			= -1;
		else 
		{
			_angleDiff			= 10 - _angleDiff;
			_direction			= 1;
		}
	}
	_angleDiff		= _angleDiff * 36;   
}

void GateSymbol::SwapSymbol(int swapId, int swapPos, CObject *swapSymbol, CObject *swapBgObject, std::function<void (int)> SwapCompleteCallback)
{
	_SwapCompleteCallback	= SwapCompleteCallback;
	SwapSymbol(swapId, swapPos, swapSymbol, swapBgObject);
}

void GateSymbol::UpdateSymbol()
{
	CPoint OriginPos	= CPoint(686.0f,340.0f, 0);
	CPoint RefPos		= CPoint(686.0f,110.0f, 0);
	CPoint RefSymbolPos = CPoint(686.0f,63.0f, 0);

	if( _angleIncrement < _angleDiff )
	{
		_angleIncrement		+= 1;
		CPoint rotatePoint	= Elixir::RotatePointByAngle(OriginPos, RefSymbolPos , ( _id * 36.0f ) + ( _angleIncrement * _direction) );
		CPoint rotateBgPoint	= Elixir::RotatePointByAngle(OriginPos, RefPos , ( _id * 36.0f ) + ( _angleIncrement * _direction) );
		gateSymObj->SetPos( &rotatePoint );
		bgObject->SetPos( &rotateBgPoint );
		bgObject->SetRotate(  180 + (_id * 36.0f) + ( _angleIncrement * _direction) );
	}
	else
	{
		OnSwapComplete();
	}

}

void GateSymbol::OnSwapComplete()
{
	gateSymObj			= _swapSymbol;
	bgObject			= _swapBgObject;
	_swapSymbol			= NULL;
	_swapBgObject		= NULL;
	_angleDiff			= 0;
	_angleIncrement		= 0;
	_direction			= 0;

	_state				= eGateSymbolState_Idle;
	if (_SwapCompleteCallback != nullptr)
	{
		_SwapCompleteCallback(_id);
	}
	
}

void GateSymbol::SetEnable(bool enableFlag)
{
	if( enableFlag )
		gateSymObj->PlayAnimation(0);
	else
		gateSymObj->PlayAnimation(1);
}


CPoint GateSymbol::GetOrigin()
{
	CPoint origin = CPoint(693.0f,353.0f, 0 );
	return( origin );
}

L8S2GateSymbolState GateSymbol::GetState()
{
	return(_state);
}

void GateSymbol::SetState(L8S2GateSymbolState state )
{
	_state = state;
}

GateSymbol::~GateSymbol()
{
	gateSymObj				= NULL;
	bgObject				= NULL;
	_swapSymbol				= NULL;
	_swapBgObject			= NULL;
	_SwapCompleteCallback	= nullptr;
}


GateSymbolManager::GateSymbolManager(CHOScene* scene)
{
	_lock				= scene->GetObjectByID(std::string("Lock"));
	_frame				= scene->GetObjectByID(std::string("Frame"));
	_mergedObject		= scene->GetObjectByID(std::string("Merged"));
	_mergedObject->SetEnable(false);
	symbolSelectionCount = 0;
	Selectedsymbols[0]	= -1;
	Selectedsymbols[1]  = -1;
	SkipCount			= -1;
	isSolved			= false;
	_isSwapping			= false;

	CPoint OriginPos		= CPoint(686.0f,320.0f, 0);
	CPoint RefPos			= CPoint(686.0f,90.0f, 0);
	CPoint RefSymbolPos		= CPoint(686.0f,43.0f, 0);
	for( int i = 0; i < 10; i++)
	{
		_glowBig[i]		= scene->GetObjectByID(std::string("glowLarge") + MKSTR(i));
		_glowSmall[i]	= scene->GetObjectByID(std::string("glowSmall") + MKSTR(i));
		_glowBig[i]->SetEnable(false);
		_glowSmall[i]->SetEnable(false);
		RandomArray[i] = i;;
	}

	for( int i = 0; i < 10; i++)
	{
		int randomVal = rand() % 10;
		int temp = RandomArray[i];
		RandomArray[i] = RandomArray[randomVal];
		RandomArray[randomVal] = temp;
	}

	for (int i = 0; i < 10; i++)
	{
		int j = -1;
		if( ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleSolved )
		{
			j = i;
			_glowSmall[i]->SetEnable(true);
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleStarted )
		{
			j = RandomArray[i];
			ProfileSystem::Instance->ActiveProfile.L8_S2_GateSymbolArray[i] = j;
		}
		else
		{
			j = ProfileSystem::Instance->ActiveProfile.L8_S2_GateSymbolArray[i];
			RandomArray[i] = j;
		}
		CObject	*SymbolObject	= scene->GetObjectByID(std::string("Symbol") + MKSTR(j));
		CObject	*BgObject		= scene->GetObjectByID(std::string("Needle")+MKSTR(i));
		CObject	*GlowObject		= scene->GetObjectByID(std::string("NeedleGlow")+MKSTR(i));
		
		if(i == j)
			_glowSmall[i]->SetEnable(true);

		GateSymbol *TempSymbol = new GateSymbol( i, j, SymbolObject, BgObject, GlowObject);
		CPoint TranslatePos	= Elixir::RotatePointByAngle( OriginPos, RefPos , (i) * 36.0f );
		CPoint TranslateSymbolPos	= Elixir::RotatePointByAngle( OriginPos, RefSymbolPos , (i) * 36.0f );
		TempSymbol->SetSymbolRotate(  180 + (i * 36.0f) );
		TempSymbol->SetPosition(j, TranslatePos, TranslateSymbolPos);
		TempSymbol->SetGlow(false);
		TempSymbol->SetHoverEffect(false);
		if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced && j != 0 )
			TempSymbol->SetEnable(false);
		else
			TempSymbol->SetEnable(true);

		_SymbolArray.push_back(TempSymbol);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleStarted && ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced )
		ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleStarted  = true;
}

bool GateSymbolManager::IsSwapping()
{
	return(_isSwapping);
}

void GateSymbolManager::UpdateSymbols()
{
	for (int i = 0; i < 10; i++)
	{
		if(_SymbolArray[i]->GetState() == eGateSymbolState_Swapping )
		{
			_SymbolArray[i]->UpdateSymbol();
			
		}
		else if(_SymbolArray[i]->GetState() == eGateSymbolState_Skipping )
		{
			_SymbolArray[i]->UpdateSymbol();
		}
	}
}

void GateSymbolManager::ToggleHoverEffect(int id, bool glowFlag)
{
	if( _SymbolArray[id]->CheckIfSelected() )
		return;
	_SymbolArray[id]->SetHoverEffect(glowFlag);
}


void GateSymbolManager::OnSymbolSelected(int id)
{
	Selectedsymbols[symbolSelectionCount] = id;
	if( symbolSelectionCount >= 1 )
	{
		_glowBig[Selectedsymbols[0]]->SetEnable(false);
		int swapPos1 = _SymbolArray[Selectedsymbols[1]]->GetPositionIndex();
		int swapPos2 = _SymbolArray[Selectedsymbols[0]]->GetPositionIndex();
		_SymbolArray[Selectedsymbols[0]]->SetGlow(false);
		_SymbolArray[Selectedsymbols[0]]->SwapSymbol(Selectedsymbols[1], swapPos1, _SymbolArray[Selectedsymbols[1]]->gateSymObj, _SymbolArray[Selectedsymbols[1]]->bgObject );
		_SymbolArray[Selectedsymbols[1]]->SwapSymbol(Selectedsymbols[0], swapPos2, _SymbolArray[Selectedsymbols[0]]->gateSymObj, _SymbolArray[Selectedsymbols[0]]->bgObject, 
			CC_CALLBACK_1(GateSymbolManager::OnSwapComplete,this));
		symbolSelectionCount = 0;
		Selectedsymbols[0]	= -1;
		Selectedsymbols[1]  = -1;
		_isSwapping			=true;
	}
	else
	{
		Control::Audio->QuickLoadAndPlaySFX("s2pz_GateSelected");
		_glowBig[id]->SetPos(&_SymbolArray[Selectedsymbols[0]]->gateSymObj->Pos);
		_glowBig[id]->SetEnable(true);
		_SymbolArray[Selectedsymbols[0]]->SetGlow(true);
		symbolSelectionCount++;
	}
}

void GateSymbolManager::OnSwapComplete(int id)
{
	if( SkipCount > -1 )
	{
		SkipCount++;
		if( SkipCount > 9 )
		{
			_isSwapping		= false;
			isSolved	= true;
			ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleSolved = true;
		}
	}
	else
	{
		_isSwapping		= false;
		for (int i = 0; i < 10; i++)
		{
			ProfileSystem::Instance->ActiveProfile.L8_S2_GateSymbolArray[i] = _SymbolArray[i]->GetPositionIndex();
			RandomArray[i] =  _SymbolArray[i]->GetPositionIndex();
		}
		if(CheckIfPuzzleComplete())
		{
			AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleSolved = true;
			isSolved = true;
		}
	}
}

bool GateSymbolManager::CheckIfPuzzleComplete()
{
	bool flag = true;
	
	for (int i = 0; i < 10; i++)
	{
		_glowSmall[i]->SetEnable(false);
		if(_SymbolArray[i]->GetId() != _SymbolArray[i]->GetPositionIndex() )
			flag = false; 
		else
			_glowSmall[i]->SetEnable(true);
	}

	return (flag);
}

void GateSymbolManager::SetGateSymbolsActive()
{
	for (int i = 0; i < 10; i++)
		_SymbolArray[i]->SetEnable(true);
}


void GateSymbolManager::AutoSolvePuzzle()
{
	_isSwapping			=true;
	SkipCount = 0;
	for (int i = 0; i < 10; i++)
	{
		int swapPos = _SymbolArray[RandomArray[i]]->GetPositionIndex();
		_SymbolArray[i]->OnSkipActive(RandomArray[i], swapPos, CC_CALLBACK_1(GateSymbolManager::OnSwapComplete,this));
		_glowSmall[i]->SetEnable(true);
		_glowSmall[i]->SetAlpha(0.0f);
		_glowSmall[i]->FadeIn(1.5f);
	}
}


GateSymbolManager::~GateSymbolManager()
{
	symbolSelectionCount = 0;
	Selectedsymbols[0]	= -1;
	Selectedsymbols[1]  = -1;
	for (int i = 0; i < 10; i++)
	{
		_SymbolArray[i] = NULL; 
	}
}

L8_S2_PZGate::L8_S2_PZGate()
{
	isAnimationPlaying	= false;
	isSkipActive		= false;
	SelectedSymbol		= -1;
	PrevSymbol			= -1;
	angleDiff			= 0;
	angleIncrement		= 0;
	SwapIndex[0]		= -1;
	SwapIndex[1]		= -1;
	SwapCount			= -1;
	isPuzzleSolved		=  false;
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S2\\PZ1\\PZ1.SCN");

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

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S2_PZGate]  = true;

	SymbolManager = new GateSymbolManager( HOScene );

	//InitSceneElements();
	InitControlPolys();

	GateOpenAnim = NULL;
	GateOpenAnim = new JSONAnimator( 1, CC_CALLBACK_1(L8_S2_PZGate::OnAnimationComplete, this ));
	GateOpenAnim->parseAndLoadJSON("ANIM\\L8\\S2_PZ1_GATEOPEN.JSON", HOScene );

	KeyTurnAnim = NULL;
	KeyTurnAnim = new JSONAnimator( 0, CC_CALLBACK_1(L8_S2_PZGate::OnAnimationComplete, this ));
	KeyTurnAnim->parseAndLoadJSON("ANIM\\L8\\S2GATEOPEN.JSON", HOScene );

	Key = HOScene->GetObjectByID("Key");

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced )
		Key->SetEnable(false);

	int zOrder = HOScene->GetObjectByID("Lock")->ZOrder + 1;
	S2_PzGateGlowFX = NULL;
	S2_PzGateGlowFX = new SuperFX("PRT\\L8\\S2_PzGateGlow.INI", PointSystem::CreateCPoint(688,318,0), zOrder); 
	S2_PzGateGlowFX->Start();
	S2_PzGateGlowFX->AdvanceByTime(2.0f);


	

	s2_PzGateCompleteFX = new SuperFX("PRT\\L8\\s2_PzGateComplete.INI", PointSystem::CreateCPoint(688,320,0), zOrder + 50); 
	



	if (ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleSolved )
	{
		Hud->InvBox_StartPuzzleMode(); 
	}

	TaskPendingUpdate();

}




void L8_S2_PZGate::InitControlPolys()
{
	
	int SymbolArray0[] = {673,471,637,533,628,602,685,653,744,606,735,529,698,470};
	SymbolPoly[0]=  new CPolygon(7);
	SymbolPoly[0]->Init(SymbolArray0);
	int SymbolArray1[] = {768,451,775,517,811,579,889,586,897,508,846,461,783,437};
	SymbolPoly[1]=  new CPolygon(7);
	SymbolPoly[1]->Init(SymbolArray1);
	int SymbolArray2[] = {830,378,874,425,947,453,1002,417,973,352,901,345,838,353};
	SymbolPoly[2]=  new CPolygon(7);
	SymbolPoly[2]->Init(SymbolArray2);
	int SymbolArray3[] = {836,280,904,294,984,273,1012,203,931,174,871,207,830,259};
	SymbolPoly[3]=  new CPolygon(7);
	SymbolPoly[3]->Init(SymbolArray3);
	int SymbolArray4[] = {787,199,855,174,909,108,885,50,798,60,768,122,766,187};
	SymbolPoly[4]=  new CPolygon(7);
	SymbolPoly[4]->Init(SymbolArray4);
	int SymbolArray5[] = {698,162,734,103,744,30,683,2,627,34,641,108,673,162};
	SymbolPoly[5]=  new CPolygon(7);
	SymbolPoly[5]->Init(SymbolArray5);
	int SymbolArray6[] = {602,186,600,118,571,58,490,54,478,124,522,177,584,200};
	SymbolPoly[6]=  new CPolygon(7);
	SymbolPoly[6]->Init(SymbolArray6);
	int SymbolArray7[] = {543,260,500,212,436,175,366,223,403,292,467,294,533,285};
	SymbolPoly[7]=  new CPolygon(7);
	SymbolPoly[7]->Init(SymbolArray7);
	int SymbolArray8[] = {533,357,467,338,393,348,369,433,451,468,506,434,542,382};
	SymbolPoly[8]=  new CPolygon(7);
	SymbolPoly[8]->Init(SymbolArray8);
	int SymbolArray9[] = {586,439,520,462,468,514,478,583,577,582,596,514,607,452};
	SymbolPoly[9]=  new CPolygon(7);
	SymbolPoly[9]->Init(SymbolArray9);



}

void L8_S2_PZGate::OnAnimationComplete(int index)
{	
	isAnimationPlaying = false;
	switch (index)
	{
	case 0:	OpenGate();
		break;
	case 1:	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		break;
	}

}

void L8_S2_PZGate::UpdateSceneElements( CPoint pos)
{
	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced ||  isSkipActive )
		return;
	for (int i = 0; i < 10; i++)
	{
		SymbolManager->ToggleHoverEffect(i,false);
		if(SymbolPoly[i]->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			SymbolManager->ToggleHoverEffect(i,true);
			if( Control::Input->LBclicked() )
			{
				Cursor::SetMode(CA_Normal);
				SymbolManager->OnSymbolSelected(i);
			}
		}
	}
}

void L8_S2_PZGate::Required()
{
	HiddenObjectSystem::Required();	//forced update
}

void L8_S2_PZGate::Update()
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

#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	SymbolManager->UpdateSymbols();

	if(isAnimationPlaying )
		return;

	if( !SymbolManager->IsSwapping() )
		UpdateSceneElements(pos);

	if( GateKeyRect.Intersection(&pos) && !SymbolManager->isSolved && ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced )
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			Control::Audio->QuickLoadAndPlaySFX("s2pz_GatekeyTurn");
			Cursor::SetMode(CA_Normal);
			Key->Shake( 0,0, 5, 0.3f); 
		}

	}

	if( !isPuzzleSolved )
	{
		if(SymbolManager->isSolved)
		{
			isPuzzleSolved = true;
			s2_PzGateCompleteFX->Start();
			ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleSolved = true;
			Control::Audio->QuickLoadAndPlaySFX("s2pz_GateCompleted");
			Hud->InvBox_StopPuzzleMode();
		}
	}


	if( ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened )
	{
		Cursor::SetMode(CA_HandPointy);
		if( GateKeyRect.Intersection(&pos) && Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			OnPuzzleComplete();
			return;
		}
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed() && !SymbolManager->IsSwapping())
	{
		Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	

	if( IsMouseOverGoBackArea(&pos) && !SymbolBottomArea.Intersection(&pos) && !SymbolManager->IsSwapping() )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L8_S2_Garden);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L8_S2_Garden);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S2_Garden, 2);
			}
#else			
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
		return;
	}

	if(PuzzleRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced)
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			Hud->ShowBannerText("L8gate");
		}
		return;
	}

}

void L8_S2_PZGate::OnPuzzleComplete()
{
	ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened = true;
	KeyTurnAnim->PlayAnim();
	TaskPendingUpdate();
}

void L8_S2_PZGate::OpenGate()
{
	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S4_Pool] = true;
	HOScene->GetObjectByID("Lock")->SetEnable(false);
	HOScene->GetObjectByID("Frame")->SetEnable(false);
	HOScene->GetObjectByID("Merged")->SetEnable(false);
	for (int i = 0; i < 10; i++)
	{
		HOScene->GetObjectByID("Needle" + MKSTR(i))->SetEnable(false); 
		HOScene->GetObjectByID("NeedleGlow" + MKSTR(i))->SetEnable(false); 
		HOScene->GetObjectByID("Symbol" + MKSTR(i))->SetEnable(false); 
		HOScene->GetObjectByID("glowLarge" + MKSTR(i))->SetEnable(false); 
		HOScene->GetObjectByID("glowSmall" + MKSTR(i))->SetEnable(false); 
	}
	Control::Audio->QuickLoadAndPlaySFX("s2p1_lockOpen");
	GateOpenAnim->PlayAnim();
	isAnimationPlaying = true;
	TaskPendingUpdate();
}


void L8_S2_PZGate::ReceiveMessage(int val)
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

			if( itemVal == eInv_L8_S3_CrystalKey && GateKeyRect.Intersection(&dropPos))
			{
				ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced = true;
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
				Key->SetEnable(true);
				SymbolManager->SetGateSymbolsActive();
				Control::Audio->QuickLoadAndPlaySFX("s2p1_placeKeyOnGate");
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S3_CrystalKey);
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_CrystalKey] = false;
				Hud->SendUpdateSceneMessage();
				Hud->InvBox_StartPuzzleMode();
			}
			else if(( itemVal == eInv_L8_S1_HalfKey || itemVal == eInv_L8_CB_FullKey || itemVal == eInv_L8_S2_Tool )&& GateKeyRect.Intersection(&dropPos))
			{
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
				Hud->ShowBannerText("L8oops");
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}


		
	}
	else if( val == INV_CLOSE )
	{		


	}
	TaskPendingUpdate();
}

void L8_S2_PZGate::ProcessSkip()
{
	if( SymbolManager->IsSwapping() )
		return;
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	isSkipActive = true;
	SymbolManager->AutoSolvePuzzle();
	ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleSolved = true;
	Control::Audio->QuickLoadAndPlaySFX("s2pz_GateCompleted");
	Hud->InvBox_StopPuzzleMode();
	
}

void L8_S2_PZGate::ProcessNonHOHint()
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
				if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_CrystalKey] )
				{
					Hud->ShowHintForInvItem(eInv_L8_S3_CrystalKey, &GateKeyRect);
					return;
				}
			break;
		case 1: ++iNonHOHint;
				if( ProfileSystem::Instance->ActiveProfile.L8_S2_GatePuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened )
				{
					Hud->InitHintTrailEmit( &GateKeyRect, false );
					return;
				}
			break;
		case 2: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 3: ++iNonHOHint;
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

void L8_S2_PZGate::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	

	if(!ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced && ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken )
	{
		sparklePointsVec.push_back(CPoint(GateKeyRect.GetCenterX(), GateKeyRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened && ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced  )
	{
		sparklePointsVec.push_back(CPoint(GateKeyRect.GetCenterX(), GateKeyRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
}

void L8_S2_PZGate ::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L8_S2_PZGate ::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened && ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken )
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] = isAnyTaskLeft;

}

L8_S2_PZGate::~L8_S2_PZGate()
{
	Hud->InvBox_StopPuzzleMode();
	TaskPendingUpdate();
	if( !Control::Audio->IsToLevelTrackRoom() )
		if( Control::Audio->IsPlaying(aTrackLevel5) )
			Control::Audio->StopSample(aTrackLevel5);

	SAFE_DELETE(S2_PzGateGlowFX);
	SAFE_DELETE(SymbolManager);
	SAFE_DELETE(GateOpenAnim);
	SAFE_DELETE(KeyTurnAnim);
	SAFE_DELETE(s2_PzGateCompleteFX);
}
void L8_S2_PZGate::NavigateToScene()
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