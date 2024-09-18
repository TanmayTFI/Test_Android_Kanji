

#ifndef POPUP_H
#define POPUP_H

#include "HOScene.h"
#include "Hud.h"
#include "HOInventoryBox.h"
#include "GameFlow.h"
#include "MouseInterface.h"
#include"Application.h"
#include "ClickablePolygon.h"
#ifdef _DEBUG
	#include "SuperLog.h"
#endif

enum PopupState
{
	ePopupState_Inactive =-1,
	ePopupState_ScalingUp,
	ePopupState_Open,
	ePopupState_ScalingDown,
	ePopupState_Closed
};

class Popup: public CHOScene, public CRenderListEntity, UpdateListEntity, public MouseInterface
{

protected:

	CObject * _sceneRT;
	int _popupId;
	CHOScene * _scene;
	PopupState _currentPopupState;
	CRectangle  _popupBoundingBox;
	ClickablePolygon * _clickAreaToOpenPopup;
	
	CObject*	_closeBtn;

	float _popupAlpha;
	float _popupTransitionTimer;
	float _popupCloseDelayTimer;
	int _popupOpenSFXId;
	int _popupCloseSFXId;
	bool _allowCloseOutsideArea;
	bool _isSparkleUpdatePending;
	CPoint _zoomOrigin;
	CPoint _sparkleUpdatePoint;

	std::function<void (Popup*)> _popupCloseCallback;
	float EasingQuadOut(float t,float b,float c,float d);

	virtual void OnScaleUpComplete();
	virtual void OnScaleDownComplete();
	void SetupPopupTransition(float centerX, float centerY);

	//To udpate Popup from external scene
	virtual void SceneUpdate(){};

public:

	Popup();
	Popup(int popupId, HudControl *hud = NULL, CHOScene * scene = NULL, int zOrder = eZOrder_Popup, CPoint zoomOrigin = CPoint(Application::CurrentResolution.Width*0.5f, Application::CurrentResolution.Height*0.5f, 0), std::function<void (Popup*)> popupCloseCallback = nullptr);

	~Popup();

	HudControl *Hud;

	void SetPopupState(PopupState popupState);
	PopupState GetPopupState();

	int GetPopupId();
	bool GetSparkleUpdateStatus();
	void SetSparkleUpdate(bool flag);
	//set to prevent further updates while some animation is active
	bool isAnimationPlaying;
	bool isMouseOverCloseButton;

	/*void SetCloseBtn(UIButton closeBtn);
	UIButton* GetCloseBtn();*/

	void SetPopupBoundingBox(CRectangle boundingBox);
	void SetPopupBoundingBox(CObject * boundingBoxRefObject);

	void SetZoomOrigin(CPoint zoomOrigin);
	void SetSparkleUpdatePoint(CPoint sparkleUpdatePoint);
	CPoint GetSparkleUpdatePoint();

	void SetFrameAndCloseButton(bool isCircularPopup = false);void UpdateBgGraphic();	virtual void ClosePopup(float delayTime = 0);
	virtual void OpenPopup();

	virtual void OnPopupOpened();
	virtual void OnPopupCloseBtnClicked();

	virtual void Render(const CPoint* pCam);
	virtual void Update(float ds);

	virtual void HandleInventoryDrop(CPoint dropPos, int itemVal);

	virtual void ProcessHint();
	virtual bool isTaskLeft();

	void HandleMouseInput(CInput * mouseInput);
	void SetPopupCloseCallback(std::function<void (Popup*)> sceneClickAreaCallback);
	void SetClickAreaToOpenPopup(ClickablePolygon * Polygon);
	ClickablePolygon * GetClickAreaToOpenPopup();
};

#endif