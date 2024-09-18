#include"Popup.h"	
#include"InteractiveObjectList.h"

Popup::Popup()
{
	_scene = NULL;
	_sceneRT = NULL;
	_clickAreaToOpenPopup = NULL;
}

Popup::Popup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback)
{
	Hud = hud;

	_scene = scene;
	_popupOpenSFXId = -1;
	_popupCloseSFXId = -1;
	_popupId = popupId;
	_currentPopupState = ePopupState_Inactive;

	_sceneRT = NULL;
	_closeBtn = NULL;
	_clickAreaToOpenPopup = NULL;

	isMouseOverCloseButton = false;
	isAnimationPlaying = false;

	_popupAlpha = 0;
	_popupTransitionTimer = 0;
	_popupCloseDelayTimer = 0;
	SetZ(zOrder);
	_zoomOrigin.SetXY(zoomOrigin.x, zoomOrigin.y);
	_sparkleUpdatePoint.SetXY(zoomOrigin.x, zoomOrigin.y);
	_allowCloseOutsideArea = false;
	Visible = true;
	_popupCloseCallback = nullptr;
	CInteractiveObjectList::GetInstance()->Add((MouseInterface *)this);
	_popupCloseCallback = popupCloseCallback;
}

Popup::~Popup()
{
	//SAFE_DELETE(_closeBtn);
	_closeBtn = NULL;
	_scene = NULL;
	if (_sceneRT) _sceneRT->SetEnable(false);
	_sceneRT = NULL;
	CInteractiveObjectList::GetInstance()->Remove((MouseInterface *)this);
	SAFE_DELETE(_clickAreaToOpenPopup);
}

void Popup::SetFrameAndCloseButton(bool isCircularPopup)
{

	if (isCircularPopup)
	{
		//Add popup frame and close button
		CObject *popCirFrame = new CObject();
		popCirFrame->LoadInit("UI\\POPCIRCFRAME.SPR", 4, -9, eZOrder_PopFrame);
		popCirFrame->SetUID("popFrame");
		PushObject(popCirFrame);

		CObject *popCirCloseBtn = new CObject();
		popCirCloseBtn->LoadInit("UI\\CLOSEBTN.SPR", 145, -158, eZOrder_PopCloseBtn);
		popCirCloseBtn->SetUID("popClose");
		PushObject(popCirCloseBtn);

		_closeBtn = popCirCloseBtn;
	}
	else
	{
		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\POPRECTFRAME.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		PushObject(popFrame); //in Normal Cases

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\CLOSEBTN.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");	
		PushObject(popCloseBtn);

		_closeBtn = popCloseBtn;
	}	
}

void Popup::SetPopupState(PopupState popupState)
{
	if (_currentPopupState == popupState)
	{
		return;
	}

	switch (popupState)
	{
	case ePopupState_Inactive:
		{

		}
		break;
	case ePopupState_ScalingUp:
		{
			//SetZ(eZOrder_Popup);
			Show(false);
		}
		break;
	case ePopupState_Open:
		{
			Show(true);
		}
		break;
	case ePopupState_ScalingDown:
		{
			OnPopupCloseBtnClicked();
			//SetZ(eZOrder_Popup);
			Show(false);
		}
		break;
	case ePopupState_Closed:
		{
			Show(false);
		}
		break;
	default:
		break;
	}

	_currentPopupState = popupState;
}

PopupState Popup::GetPopupState()
{
	return _currentPopupState;
}

int Popup::GetPopupId()
{
	return _popupId;
}

void Popup::SetSparkleUpdate(bool flag)
{
	_isSparkleUpdatePending = flag;
}


bool Popup::GetSparkleUpdateStatus()
{
	return _isSparkleUpdatePending;
}

void Popup::SetSparkleUpdatePoint(CPoint sparkleUpdatePoint)
{
	_sparkleUpdatePoint = sparkleUpdatePoint;
}
CPoint Popup::GetSparkleUpdatePoint()
{
	return _sparkleUpdatePoint;
}
void Popup::OnScaleUpComplete()
{
	SetPopupState(ePopupState_Open);
	OnPopupOpened();
}

void Popup::OnScaleDownComplete()
{
	if (_scene)
	{
		_scene->PauseObjects(false);
		_scene->Show(true);
	}
	if (_sceneRT)
	{
		_sceneRT->SetEnable(false);
	}
	SetPopupState(ePopupState_Closed);
	if (_popupCloseCallback)
	{
		_popupCloseCallback(this);
	}

#ifdef _CEBUILD
	Hud->ShowMorphObjects();
#endif 
}

void Popup::ClosePopup(float delayTime)
{
	_popupCloseDelayTimer = delayTime;
	SetPopupState(ePopupState_ScalingDown);
	SetupPopupTransition(_zoomOrigin.x, _zoomOrigin.y);
	if (_popupCloseSFXId != -1)
	{
		Control::Audio->QuickLoadAndPlaySFX((eAudio)_popupCloseSFXId);
	}

}

void Popup::OnPopupOpened()
{

}

void Popup::OnPopupCloseBtnClicked()
{

}


void Popup::OpenPopup()
{
#ifdef _CEBUILD
	Hud->HideMorphObjects();
	Hud->HideMorphCollectedObject();
#endif 

	SetPopupState(ePopupState_ScalingUp);
	SetupPopupTransition(_zoomOrigin.x, _zoomOrigin.y);

#ifdef _CEBUILD
	Hud->ShowMorphCollectedObject();
#endif 

	if (_popupCloseSFXId != -1)
	{
		Control::Audio->QuickLoadAndPlaySFX((eAudio)_popupOpenSFXId);
	}
}

void Popup::UpdateBgGraphic()
{
	if (_scene)
	{
		GFInstance->UpdateBgGraphic(_scene);
	}
}



void Popup::SetupPopupTransition(float centerX, float centerY)
{
	if (_scene)
	{
		_scene->PauseObjects(true);
	//	_scene->Show(true);
		_sceneRT = GFInstance->UpdateBgGraphic(_scene);
	//	_scene->Show(false);
		_sceneRT->SetEnable(true);
	}


	if( GFInstance->IsRTCreated )
	{
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS	
		GFInstance->IsRenderingToRT = true;
    }
		RenderToTarget(-171,0);
    if( GFInstance->IsRTCreated )
    {
		GFInstance->IsRenderingToRT = false;
		GFInstance->RTGraphic->endRenderToTarget();
    }
		GFInstance->SetPopupTween( (int)(centerX-683) ,(int)(centerY-384 ),171,0);

		if (_currentPopupState == ePopupState_ScalingUp)
		{
			GFInstance->CreateGreyscaleScene();
		}
	
}

void Popup::Render(const CPoint* pCam)
{
	//No video show fullscene base    
	if(_currentPopupState == ePopupState_ScalingUp || _currentPopupState == ePopupState_ScalingDown)
	{

		//Init again if render target lost due to fullscreen-window switches or similar
		if( GFInstance->RTGraphic != NULL )
		{
			GFInstance->RTGraphic->blitAlphaRectFx(0, 0, 1366, 768,
				short(GFInstance->PStartX + float(GFInstance->PEndX - GFInstance->PStartX) * _popupAlpha),
				short(GFInstance->PStartY + float(GFInstance->PEndY - GFInstance->PStartY) * _popupAlpha),
				-5 * (1.0f - _popupAlpha), 
				_popupAlpha,
				_popupAlpha > 1.0f ? 1.0f : _popupAlpha);

			//Gradient alpha causes alpha on transition, leaking the bg scene through popups during transition
			GFInstance->RTGraphic->blitAlphaRectFx(0, 0, 1366, 768,
				short(GFInstance->PStartX + float(GFInstance->PEndX - GFInstance->PStartX) * _popupAlpha),
				short(GFInstance->PStartY + float(GFInstance->PEndY - GFInstance->PStartY) * _popupAlpha),
				-5 * (1.0f - _popupAlpha),
				_popupAlpha,
				_popupAlpha > 1.0f ? 1.0f : _popupAlpha);
        }
	}
}

void Popup::Update(float ds)
{
	CHOScene::update(ds);

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if(Control::Input->isKeyBackSensed() && _currentPopupState == ePopupState_Open)
		{
			// should we follow the convenstional popup closing is disabled or not is to be tested and only then we can uncomment the below lines	
		//	CPoint mousePos = Control::Input->Mpos();
			//bool isPopupCloseDisabled = Hud->IsMouseOverHUD(&mousePos) || GFInstance->IsActivePopup ||  Hud->IsComboInvPopupShowing || Hud->IsMapShowing || Hud->IsDiaryShowing;
			//if (!isPopupCloseDisabled)
			{
				ClosePopup();
			}
		}
	#endif

	if(GFInstance->IsSceneGraphicReloaded)
	{
		if (_scene)
		{
			if(_currentPopupState == ePopupState_ScalingUp || _currentPopupState == ePopupState_ScalingDown || _currentPopupState == ePopupState_Open)
			{
				_sceneRT = GFInstance->UpdateBgGraphic(_scene);
				GFInstance->IsSceneGraphicReloaded = false;
			}
		}
	}

	if (_currentPopupState == ePopupState_ScalingUp)
	{
		_popupTransitionTimer += POPUP_SCALE_FACTOR * Control::LogicRate;
		_popupAlpha = EasingQuadOut(_popupTransitionTimer,0,1,1.0f);

		if( _popupTransitionTimer >= 1.0f)
		{
			_popupAlpha = 1.0f;
			OnScaleUpComplete();
		}
		GFInstance->UpdateBlurScene(_popupAlpha > 1.0f ? 1.0f : _popupAlpha);

	}
	else if(_currentPopupState == ePopupState_ScalingDown)
	{
		_popupCloseDelayTimer -= 1.0f * Control::LogicRate;
		if( _popupCloseDelayTimer > 0.0f )
		{
			return;	
		}
		else
		{
			_popupCloseDelayTimer = 0.0;	
		}

		_popupAlpha -= POPUP_SCALEDOWN_FACTOR*Control::LogicRate;
		if( _popupAlpha <= 0.0f )
		{
			_popupTransitionTimer = 0;

			PauseObjects(false);
			_popupAlpha = 0.0f;
			GFInstance->TurnOffBlurScene();
			OnScaleDownComplete();
		}
		else
		{
			GFInstance->UpdateBlurScene(_popupAlpha);
		}
	}
	if (_scene && _sceneRT && (_currentPopupState == ePopupState_ScalingUp || _currentPopupState == ePopupState_ScalingDown))
	{
		if(!GFInstance->IsScnBg1024Res)
		{
			//1366 resolution
			_sceneRT->SetScale(1.0f + _popupAlpha * 0.25f);
			_sceneRT->SetXY(_zoomOrigin.x - (_zoomOrigin.x * (1.0f + _popupAlpha  * 0.25f)) ,_zoomOrigin.y - (_zoomOrigin.y * (1.0f + _popupAlpha * 0.25f)));
		}
		else
		{
			//1024 resolution
			_sceneRT->SetScale(1.0f + _popupAlpha * 0.25f);
			_sceneRT->SetXY(_zoomOrigin.x - (_zoomOrigin.x * (1.0f + _popupAlpha  * 0.25f)) +171.0f ,_zoomOrigin.y - (_zoomOrigin.y * (1.0f + _popupAlpha * 0.25f)));
		}
	}
}

void Popup::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void Popup::ProcessHint()
{
	if(_closeBtn)
	{
		Hud->InitHintTrailEmit(_closeBtn->Pos.x, _closeBtn->Pos.y, true);
	}
	else
	{
		#ifdef K_WIN32
			WARN("Er0x1901- [Popup_ProcessHint] : Undefined Close Button");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1901- [Popup_ProcessHint] : Undefined Close Button");
		#endif
	}
}

bool Popup::isTaskLeft()
{
	return false; //Task Left logic implemented in derived function
}

float Popup::EasingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void Popup::HandleMouseInput(CInput * mouseInput)
{
	if(_currentPopupState != ePopupState_Open)
	{
		return;
	}	

	if(_closeBtn && !isAnimationPlaying)
	{
		
		//isMouseOverCloseButton = _closeBtn->Collide(&mouseInput->Mpos());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CPoint mousePos = mouseInput->Mpos();
		isMouseOverCloseButton = _closeBtn->Collide(&mousePos);
		if (isMouseOverCloseButton )
		{
			Cursor::SetMode(CA_HandPointy);
			_closeBtn->PlayAnimation(1); 
		}
		else
		{
			_closeBtn->PlayAnimation(0);
		}
	}

	if (mouseInput->LBclicked())
	{
		CPoint mousePos = mouseInput->Mpos();
		bool isPopupCloseDisabled = Hud->IsMouseOverHUD(&mousePos) || GFInstance->IsActivePopup ||  Hud->IsComboInvPopupShowing || Hud->IsMapShowing || Hud->IsDiaryShowing;
		if (!isPopupCloseDisabled)
		{
			if(isMouseOverCloseButton)
			{
				ClosePopup();
			}
			else if( !isAnimationPlaying && _allowCloseOutsideArea && !_popupBoundingBox.Intersection(&mousePos) )
			{
				ClosePopup();
			}
		}
	}
}

void Popup::SetZoomOrigin(CPoint zoomOrigin)
{
	_zoomOrigin = zoomOrigin;
}

void Popup::SetPopupBoundingBox(CRectangle boundingBox)
{
	_popupBoundingBox = boundingBox;
	_allowCloseOutsideArea = true;
}

void Popup::SetPopupBoundingBox(CObject * boundingBoxRefObject)
{
	_popupBoundingBox = boundingBoxRefObject->GetWorldRect();
	_allowCloseOutsideArea = true;
}

void Popup::SetClickAreaToOpenPopup(ClickablePolygon * polygon)
{
	_clickAreaToOpenPopup = polygon;
}

ClickablePolygon * Popup::GetClickAreaToOpenPopup()
{
	return _clickAreaToOpenPopup;
}
