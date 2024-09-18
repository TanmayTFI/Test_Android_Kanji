//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S4_PZHandCuff.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

using namespace std;

namespace Level5_Scene4_PZ1
{
	// for solution
	const LockSolutionPattern Lock_1_Pattern = {Tooth_Bottom, Tooth_Middle, Tooth_Bottom, Tooth_Up}; 
	const LockSolutionPattern Lock_2_Pattern = {Tooth_Up, Tooth_Middle, Tooth_Bottom, Tooth_Up};
	const LockSolutionPattern Lock_3_Pattern = {Tooth_Bottom, Tooth_Up, Tooth_Bottom, Tooth_Middle};
	const LockSolutionPattern Solution_Patterns[] = {Lock_1_Pattern, Lock_2_Pattern, Lock_3_Pattern};

	const int Solution_Direction[3][6] = {{Key_Right, 1, Key_Left, 3, Key_Right, 1}, {Key_Right, 2, Key_Right, 1, Key_None, 0}, {Key_Left, 3, Key_Right, 1, Key_None, 0}};

	const CRectangle  Lock1Rect(452.0f,225.0f,161.0f,119.0f);
	const CRectangle  Lock2Rect(531.0f,349.0f,150.0f,140.0f);
	const CRectangle  Lock3Rect(390.0f,348.0f,137.0f,141.0f);
	const CRectangle  LockRects[] = {Lock1Rect, Lock2Rect, Lock3Rect};

	const CRectangle  MazeArea(388.0f,176.0f,306.0f,333.0f);

	const int TeethY[] = {484, 499, 511};
	const float TweenSpeed = 1;

	const int NO_OF_NOBS = 4;
	const int NO_OF_LOCKS = 3;

	const int LockLiverUnlockPosX = 970;

	KeyNob::KeyNob(CObject* scnObj)
	{
		LoadSprite(scnObj->Filename);
		SetPos(&scnObj->Pos);
		SetZ(scnObj->ZOrder);
		SetMouseInteractionArea(GetLocalRect().w, GetLocalRect().h);
		scnObj->SetEnable(false);

		__toothPosibleStates.push(Tooth_Bottom);
		__toothPosibleStates.push(Tooth_Middle);
		__toothPosibleStates.push(Tooth_Up);
		__toothPosibleStates.push(Tooth_Middle);
	}

	KeyNob::~KeyNob()
	{
		__toothPosibleStates = queue<KeyToothState>();
	}

	void KeyNob::CreateTooth(CHOScene* scene)
	{
		for (int i = 1; i <= 3; i++)
		{
			__tooths.push_back(scene->GetObjectByID("teeth_"+MKSTR(_index)+"_"+MKSTR(i)));
		}

		_toothState = __toothPosibleStates.front();
		__toothPosibleStates.pop();
		__toothPosibleStates.push(_toothState);
	}
	
	void KeyNob::OnMouseOver()
	{
		if (Visible)
		{
			CObject::OnMouseOver();
		}
	}

	void KeyNob::OnMouseOut()
	{
		CObject::OnMouseOut();
		Cursor::SetMode(CA_Normal);
	}

	void KeyNob::OnMouseClick()
	{
		CObject::OnMouseClick();
		Rotate();
	}

	void KeyNob::Rotate()
	{
		if (IsTweening)
			return;

		Control::Audio->QuickLoadAndPlaySFX("s4pz1_keytilerotate");

		float valArray[5] = {-1, -1, -1, -1, GetRotate() - 90};
		TweenTo(valArray, TweenSpeed/2, 0, true);

		UpdateToothState();

		for (int i = int(_toothState); i < (int)(__tooths.size()); i++)
		{
			int posY = TeethY[int(_toothState)];

			float valArray[5] = {-1, __tooths[i]->Pos.x, (float)posY, -1, 0};
			__tooths[i]->TweenTo(valArray, TweenSpeed/2, 0, true);
		}
	}

	void KeyNob::SetAlpha(float alpha)
	{
		CObject::SetAlpha(alpha);
		for (int i = 0; i < (int)(__tooths.size()); i++)
		{
			__tooths[i]->SetAlpha(alpha);
		}
	}

	void KeyNob::Show(bool show)
	{
		CObject::Show(show);
		for (int i = 0; i < (int)(__tooths.size()); i++)
		{
			__tooths[i]->Show(show);
		}
	}

	void KeyNob::UpdateToothState()
	{
		_toothState = __toothPosibleStates.front();
		__toothPosibleStates.pop();
		__toothPosibleStates.push(_toothState);
	}


	Key::Key(CObject* scnObj)
	{
		LoadSprite(scnObj->Filename);
		SetPos(&scnObj->Pos);
		SetZ(scnObj->ZOrder);
		scnObj->SetEnable(false);

		_isInHole = false;
		_active = false;
	}

	Key::~Key()
	{

	}

	void Key::CreateKey(CHOScene* scene)
	{
		for (int i = 1; i <= NO_OF_NOBS; i++)
		{
			KeyNob* nob = new KeyNob(scene->GetObjectByID("rotateknob_" + MKSTR(i)));
			nob->setIndex(i);
			nob->CreateTooth(scene);
			nob->SetAlpha(0);
			__nobs.push_back(nob);
			scene->PushObject(nob);
		}

		_disableMask = scene->GetObjectByID("KeyMask");
		_keyToPut = scene->GetObjectByID("keytoput");
		_keyToPut->SetAlpha(0);
		_disableMask->SetAlpha(0);
	}

	void Key::SetAlpha(float alpha)
	{
		CObject::SetAlpha(alpha);
		if (!_active)
			_disableMask->SetAlpha(alpha);

		for (int i = 0; i < (int)(__nobs.size()); i++)
		{
			__nobs[i]->SetAlpha(alpha);
		}
	}

	void Key::Show(bool show)
	{
		CObject::Show(show);
		_disableMask->Show(show);

		for (int i = 0; i < (int)(__nobs.size()); i++)
		{
			__nobs[i]->Show(show);
		}
	}

	LockSolutionPattern Key::getUnLockPattern()
	{
		LockSolutionPattern retVal = {__nobs[0]->getToothState(), __nobs[1]->getToothState(), __nobs[2]->getToothState(), __nobs[3]->getToothState()};
		return retVal;
	}

	void Key::setIsInHole(bool isinhole)
	{
		_isInHole = isinhole;

		if (_isInHole)
		{
			float ar[5] = {0, -1, -1, -1, -1};
			TweenTo(ar, 0.5f, 0, true);
			_keyToPut->SetAlpha(0);
		}
	}

	bool Key::isMouseOnNob()
	{
		for (int i = 0; i < (int)__nobs.size(); i++)
		{
			if (__nobs[i]->GetCurrentMouseState() == eMouseState_Over)
			{
				return true;
			}
		}

		return false;
	}

	void Key::SetEnableKey(bool enable)
	{
		_active = enable;
		if(enable)
			_disableMask->FadeOut();

		for (int i = 0; i < (int)__nobs.size(); i++)
		{
			__nobs[i]->SetIsInteractionEnabled(enable);
		}
	}




	Lock::Lock()
	{
		_currentKey = NULL;
		_isUnLocked = false;
		_numOfRotation = 0;
	}

	Lock::~Lock()
	{

	}

	void Lock::AddKeySolution(LockSolutionPattern Pattern)
	{
		_solution = Pattern;
	}

	void Lock::AddToSolutionKeyRotation(int rotateTo, int times)
	{
		pair<KeyRotateDirection, int> t = make_pair(KeyRotateDirection(rotateTo), times);
		_solutionRotation.push(t);
	}

	void Lock::CreateLock(CHOScene* scene)
	{
		_lockLiver = scene->GetObjectByID("locklever_"+MKSTR(_index));
		_keyOnHole = scene->GetObjectByID("keyonhole_"+MKSTR(_index));
		_holeGlow = scene->GetObjectByID("keyholeglow_"+MKSTR(_index));
		_rotateLeft = new UIButton(scene->GetObjectByID("keyholebtnrotateleft_"+MKSTR(_index)), CC_CALLBACK_1(Lock::onLeftRotate, this));
		_rotateRight = new UIButton(scene->GetObjectByID("keyholebtnrotateright_"+MKSTR(_index)), CC_CALLBACK_1(Lock::onRightRotate, this));
		scene->PushObject(_rotateLeft);
		scene->PushObject(_rotateRight);

		for (int i = 1; i <= 3; i++)
		{
			CObject* green = scene->GetObjectByID("greenglow_" + MKSTR(_index) + "_" + MKSTR(i));
			CObject* red = scene->GetObjectByID("redglow_" + MKSTR(_index) + "_" + MKSTR(i));
			_correctLight.push_back(green);	    
			_wrongLight.push_back(red);

			if (!_isUnLocked)
				green->SetEnable(false);

			red->SetEnable(false);
		}

		_keyOnHole->SetEnable(false);
		_holeGlow->SetEnable(false);
		_rotateLeft->SetEnable(false);
		_rotateRight->SetEnable(false);

		if (_isUnLocked)
		{
			_lockLiver->Pos.x = LockLiverUnlockPosX;
		}
	}

	bool Lock::PutKey(Key *key)
	{
		if (_solution == key->getUnLockPattern())
		{
			_keyOnHole->SetAlpha(0);
			_keyOnHole->SetEnable(true);
			float valArray[5] = {1, _keyOnHole->Pos.x, _keyOnHole->Pos.y, 1, _keyOnHole->GetRotate()};
			_keyOnHole->TweenTo(valArray, TweenSpeed/2, 0, true, CC_CALLBACK_1(Lock::onKeyFadeInComplete, this));

			OnKeyMatched();
			_currentKey = key;
			_currentKey->setIsInHole(true);
			return true;
		}

		return false;
	}

	void Lock::onKeyFadeInComplete(CObject* obj)
	{
		_rotateLeft->SetEnable(true);
		_rotateRight->SetEnable(true);

		_rotateLeft->FadeIn();
		_rotateRight->FadeIn();
	}

	void Lock::Reset()
	{
		for (int i = 0; i < (int)(_correctLight.size()); i++)
		{
			_correctLight[i]->SetEnable(false);
		}

		OnKeyMatched();
	}

	void Lock::OnKeyMatched()
	{
		_handleRotation = _solutionRotation;
		_currentRotation = _handleRotation.front();
		_handleRotation.pop();
		_numOfRotation = 0;
		_currentRotationIndex = 0;
	}

	void Lock::onLeftRotate(CObject* btn)
	{
		if (_keyOnHole->IsTweening)
			return;

		if (_currentRotation.first == Key_Left)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4pz1_KeyRotateCorrect");
			float valArray[5] = {1, _keyOnHole->Pos.x, _keyOnHole->Pos.y, 1, _keyOnHole->GetRotate() + 90};
			_keyOnHole->TweenTo(valArray, TweenSpeed/2, 0, true, CC_CALLBACK_1(Lock::onKeyRotateComplete, this));
		}
		else
		{
			Control::Audio->QuickLoadAndPlaySFX("s4pz1_KeyRotateWrong");
			_keyOnHole->SetRotate(0);
			_keyOnHole->Shake(0, 0, 10, 0.5f);

			_wrongLight[_currentRotationIndex]->SetEnable(true);
			_wrongLight[_currentRotationIndex]->FadeOut();
			Reset();
		}
	}

	void Lock::onRightRotate(CObject* btn)
	{
		if (_keyOnHole->IsTweening)
			return;

		if (_currentRotation.first == Key_Right)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4pz1_KeyRotateCorrect");
			float valArray[5] = {1, _keyOnHole->Pos.x, _keyOnHole->Pos.y, 1, _keyOnHole->GetRotate() - 90};
			_keyOnHole->TweenTo(valArray, TweenSpeed/2, 0, true, CC_CALLBACK_1(Lock::onKeyRotateComplete, this));
		}
		else
		{
			Control::Audio->QuickLoadAndPlaySFX("s4pz1_KeyRotateWrong");
			_keyOnHole->SetRotate(0);
			_keyOnHole->Shake(0, 0, 10, 0.5f);

			_wrongLight[_currentRotationIndex]->SetEnable(true);
			_wrongLight[_currentRotationIndex]->FadeOut();
			Reset();
		}
	}

	void Lock::onKeyRotateComplete(CObject* obj)
	{
		if(++_numOfRotation >= _currentRotation.second)
		{
			_correctLight[_currentRotationIndex]->SetEnable(true);
			_currentRotationIndex++;

			if (!_handleRotation.empty())
			{
				_currentRotation = _handleRotation.front();
				_handleRotation.pop();
				_numOfRotation = 0;

				if(_currentRotation.first == Key_None)
					onKeyRotateComplete(obj);
			}
			else
			{
				_currentKey->setIsInHole(false);
				_currentKey = NULL;
				Unlock();
			}
		}
	}

	void Lock::Unlock()
	{
		_isUnLocked = true;

		Control::Audio->QuickLoadAndPlaySFX("S4pz1_keyunlocklock");

		float valArray[5] = {1, LockLiverUnlockPosX, _lockLiver->Pos.y, 1, _lockLiver->GetRotate()};
		_lockLiver->TweenTo(valArray, TweenSpeed, 0, true, CC_CALLBACK_1(Lock::onLiverPullComplete, this));

		_keyOnHole->SetEnable(false);
		_rotateLeft->SetEnable(false);
		_rotateRight->SetEnable(false);

		ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffLockUnlocked[_index - 1] = true;
	}

	void Lock::onLiverPullComplete(CObject* obj)
	{
		_interface->OnUnlock();
	}

	void Lock::SkipInitiated()
	{
		Unlock();
	}

	const CRectangle  KeyHoleDoorDestination(515.0f,338.0f,34.0f,35.0f);

	ToothShape::ToothShape()
	{
		_isCompleted = false;
		_selected = false;
		_readyToCheck = false;
		_toothPoly = NULL;
		_keyHolePoly = NULL;
		_keyHoleSelect = NULL;
		_toothHihgLight = NULL; 
		_keyHoleDoor = NULL;
		_interface = NULL;
		_keyHoleHighLight = NULL;
		_wrongIndicateDisableTimer = 0.0f;
	}

	ToothShape::~ToothShape()
	{
		SAFE_DELETE(_toothPoly);
		SAFE_DELETE(_keyHolePoly);
	}

	void ToothShape::Init()
	{
		if(_keyHoleSelect)
			_keyHoleSelect->SetAlpha(0);
		
		if (_keyHoleHighLight)
			_keyHoleHighLight->SetEnable(false);

		_toothHihgLight->SetAlpha(0);
		
		if(_pathHihgLight)
			_pathHihgLight->SetAlpha(0);
		
		if(_toothCorrect)
			_toothCorrect->SetAlpha(0);
		
		_toothWrong->SetAlpha(0);

		if (_isCompleted && _keyHoleDoor)
			_keyHoleDoor->SetAlpha(0);
	}

	void ToothShape::Update()
	{
		if(_isCompleted)
			return;

		CPoint mpos = Control::Input->Mpos();

		if (_wrongIndicateDisableTimer>0.0f)
		{
			_wrongIndicateDisableTimer -= Control::LogicRate;
			if (_wrongIndicateDisableTimer<=0.0f)
				_toothWrong->SetEnable(false);
		}

		if (_readyToCheck)
		{
			if(_toothPoly->Intersection(&mpos))
			{
				Cursor::SetMode(CA_HandPointy);

				if (Control::Input->LBclicked())
				{
					if(_selected)
					{
						if(_keyHoleDoor)
						{
							float ar[5] = {-1, KeyHoleDoorDestination.GetCenterX(), KeyHoleDoorDestination.GetCenterY(), -1, 90};
							_keyHoleDoor->TweenTo(ar, 1, 0, true);
						}

						OnCompleted();
						_keyHoleSelect->SetAlpha(0);
						_interface->OnMatchFound(_index);
					}
					else
					{
						_wrongIndicateDisableTimer = 1.0f;
						_toothWrong->SetEnable(true);
						_toothWrong->Animate(0.4f, 1.0f, 0.5f);
						_interface->OnWrongSelection();
					}
				}
			}

			return;
		}

		if (_keyHolePoly && _keyHolePoly->Intersection(&mpos))
		{
			Cursor::SetMode(CA_HandPointy);
			if (Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s4pz1_keyholeclick");
				_interface->OnKeyHoleSelected();
				_keyHoleSelect->SetAlpha(1);
				_selected = true;
			}
		}
	}

	void ToothShape::EnableTooth(bool enable)
	{
		if(_isCompleted)
			return;

		if(_keyHoleSelect)
			_keyHoleSelect->SetAlpha(0);
		
		_selected = false;
		_readyToCheck = enable;

		_toothHihgLight->SetEnable(enable);

		if(enable)
			_toothHihgLight->Animate(0.5f, 1.0f, 1.5f);
	}

	void ToothShape::OnCompleted()
	{
		EnableKeyHole(false);
		_isCompleted = true;
		_toothCorrect->FadeIn(3);
		_pathHihgLight->FadeIn(3);
	}

	void ToothShape::EnableKeyHole(bool enable)
	{
		if(!_isCompleted && _keyHoleHighLight)
		{
			_keyHoleHighLight->SetEnable(enable);
			
			if(enable)
			{
				_keyHoleHighLight->Animate(0.5f, 1.0f, 1.5f);
			}
		}
	}

	
	PZ_Controller::PZ_Controller()
	{
		_isPZL1Solved = false;
		_isPZL2Solved = false;
		
		_locks.clear();
		_keyTooths.clear();
	}

	PZ_Controller::~PZ_Controller()
	{
		for (int i = 0; i < NO_OF_LOCKS; i++)
		{
			SAFE_DELETE(_locks[i]);
		}

		for (int i = 0; i < (int)_keyTooths.size(); i++)
		{
			SAFE_DELETE(_keyTooths[i]);
		}

		_locks.clear();
		_keyTooths.clear();
		_key = NULL;
	}

	void PZ_Controller::InitPuzzle(CHOScene* scene)
	{
		InitL1Data(scene);

		_key = new Key(scene->GetObjectByID("handwithkey"));
		_key->CreateKey(scene);
		scene->PushObject(_key);

		for (int i = 0; i < NO_OF_LOCKS; i++)
		{
			Lock *lock = new Lock();
			lock->setIndex(i+1);
			lock->setUnlocked(ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffLockUnlocked[i]);
			lock->setLockRect(LockRects[i]);
			lock->CreateLock(scene);
			lock->setInterface(this);
			_locks.push_back(lock);

			lock->AddKeySolution(Solution_Patterns[i]);
			
			for (int j = 0; j < 6; j+=2)
			{	
				lock->AddToSolutionKeyRotation(Solution_Direction[i][j], Solution_Direction[i][j+1]);
			}
		}
	}

	void PZ_Controller::InitL1Data(CHOScene *scene)
	{
		std::vector<CPolygon*> polysKeyHole;
		std::vector<CPolygon*> polysKeyTooth;

		int ArrayPts1[] = {391,474,344,504,298,438,356,415};
		CPolygon* CPolyTeeth1=  new CPolygon(4);
		CPolyTeeth1->Init(ArrayPts1);
		polysKeyTooth.push_back(CPolyTeeth1);

		int ArrayPts2[] = {355,289,313,263,348,200,388,228};
		CPolygon* CPolyTeeth2=  new CPolygon(4);
		CPolyTeeth2->Init(ArrayPts2);
		polysKeyTooth.push_back(CPolyTeeth2);

		int ArrayPts3[] = {499,167,496,119,571,117,569,163};
		CPolygon* CPolyTeeth3=  new CPolygon(4);
		CPolyTeeth3->Init(ArrayPts3);
		polysKeyTooth.push_back(CPolyTeeth3);

		int ArrayPts4[] = {677,227,718,199,753,267,711,292};
		CPolygon* CPolyTeeth4=  new CPolygon(4);
		CPolyTeeth4->Init(ArrayPts4);
		polysKeyTooth.push_back(CPolyTeeth4);

		int ArrayPts5[] = {712,415,748,438,716,500,676,477};
		CPolygon* CPolyTeeth5=  new CPolygon(4);
		CPolyTeeth5->Init(ArrayPts5);
		polysKeyTooth.push_back(CPolyTeeth5);

		int ArrayPts6[] = {498,536,493,580,569,582,567,537};
		CPolygon* CPolyTeeth6=  new CPolygon(4);
		CPolyTeeth6->Init(ArrayPts6);
		polysKeyTooth.push_back(CPolyTeeth6);

		int HoleArrayPts1[] = {503,291,517,340,544,340,562,289};
		CPolygon* CPolyKeyHole1=  new CPolygon(4);
		CPolyKeyHole1->Init(HoleArrayPts1);
		polysKeyHole.push_back(CPolyKeyHole1);

		polysKeyHole.push_back(NULL);

		int HoleArrayPts2[] = {549,345,598,370,570,404,533,363};
		CPolygon* CPolyKeyHole2=  new CPolygon(4);
		CPolyKeyHole2->Init(HoleArrayPts2);
		polysKeyHole.push_back(CPolyKeyHole2);

		int HoleArrayPts3[] = {527,364,494,412,460,365,509,343};
		CPolygon* CPolyKeyHole3=  new CPolygon(4);
		CPolyKeyHole3->Init(HoleArrayPts3);
		polysKeyHole.push_back(CPolyKeyHole3);
		
		polysKeyHole.push_back(NULL);
		polysKeyHole.push_back(NULL);

		for (int i = 0; i < (int)polysKeyTooth.size(); i++)
		{
			ToothShape* shape = new ToothShape();
			shape->setIndex(i);
			shape->setInterface(this);
			shape->setToothPoly(polysKeyTooth[i]);
			shape->setKeyHolePoly(polysKeyHole[i]);
			shape->setKeyHoleSelect(scene->GetObjectByID("KeyHoleSelect_"+MKSTR(i+1)));
			shape->setKeyHoleHighlight(scene->GetObjectByID("KeyHolehighlight_"+MKSTR(i+1)));
			shape->setToothHighlight(scene->GetObjectByID("KeyShapeHighlight_"+MKSTR(i+1)));
			shape->setToothWrong(scene->GetObjectByID("KeyShapeWrong_"+MKSTR(i+1)));
			shape->setToothCorrect(scene->GetObjectByID("KeyShapeComplete_"+MKSTR(i+1)));
			shape->setPathHighlight(scene->GetObjectByID("PathHighlight_"+MKSTR(i+1)));
			shape->setKeyHoleDoor(scene->GetObjectByID("KeyHoleDoor_"+MKSTR(i+1)));
			_keyTooths.push_back(shape);

			if(ProfileSystem::Instance->ActiveProfile.L5_S4_KeyMatchComplete[i])
			{
				shape->OnCompleted();
			}

			shape->Init();
		}

	}

	void PZ_Controller::ShowKey(bool show)
	{
		_key->Show(show);
	}

	void PZ_Controller::ActivateL1()
	{
		_key->FadeIn(1);
		_key->SetEnableKey(false);

		for (int i = 0; i < (int)_keyTooths.size(); i++)
		{
			_keyTooths[i]->EnableKeyHole(true);
		}
	}

	void PZ_Controller::ActivateL2()
	{
		ShowKey(true);
		_key->SetEnableKey(true);
	}

	void PZ_Controller::L1Update()
	{
		for (int i = 0; i < (int)_keyTooths.size(); i++)
		{
			_keyTooths[i]->Update();
		}
	}

	void PZ_Controller::L2Update()
	{
		if(_key->getIsInHole())
			return;

		CPoint mPos = Control::Input->Mpos();

		if (_key->isMouseOnNob())
		{
			Cursor::SetMode(CA_HandPointy);
		}

		if (MazeArea.Intersection(&mPos))
		{
			if(!_key->getIsInHole())
			{
				_key->SetAlpha(0.85f);
				_key->getKeyToPut()->SetAlpha(1);
				_key->getKeyToPut()->Pos = mPos + CPoint(100, 40, 0);
			}
			else
			{
				_key->getKeyToPut()->SetAlpha(0);
			}
		}
		else
		{
			_key->SetAlpha(1);
			_key->getKeyToPut()->SetAlpha(0);
		}

		for (int i = 0; i < (int)(_locks.size()); i++)
		{
			if (!_locks[i]->getUnlocked() && 
				!_locks[i]->getCurrentKey() &&
				_locks[i]->getLockRect().Intersection(&mPos))
			{
				_locks[i]->getHoleGlow()->SetEnable(true);

				if(Control::Input->LBclicked())
				{
					if (_locks[i]->PutKey(_key))
					{
						Control::Audio->QuickLoadAndPlaySFX("s4pz1_keyusedright");
					}
					else
					{
						GFHud->ShowBannerText(Banner_L5_S4_HandCuffWrongKey);
						Control::Audio->QuickLoadAndPlaySFX("s4pz1_keyusedwrong");
					}
				}
			}
			else
			{
				_locks[i]->getHoleGlow()->SetEnable(false);
			}
		}
	}

	void PZ_Controller::OnKeyHoleSelected()
	{
		for (int i = 0; i < (int)_keyTooths.size(); i++)
		{
			_keyTooths[i]->EnableTooth(true);
			_keyTooths[i]->EnableKeyHole(false);
		}
	}

	void PZ_Controller::OnMatchFound(int index)
	{
		Control::Audio->QuickLoadAndPlaySFX("s4pz1_mazepathright");
		ProfileSystem::Instance->ActiveProfile.L5_S4_KeyMatchComplete[index] = true;
		for (int i = 0; i < (int)_keyTooths.size(); i++)
		{
			_keyTooths[i]->EnableTooth(false);
			_keyTooths[i]->EnableKeyHole(true);
		}

		_isPZL1Solved = true;

		for (int i = 0; i < (int)_keyTooths.size(); i++)
		{
			if(_keyTooths[i]->getKeyHolePoly() && !_keyTooths[i]->getIsCompleted())
			{
				_isPZL1Solved = false;
				break;
			}
		}
	}

	void PZ_Controller::OnWrongSelection()
	{
		Control::Audio->QuickLoadAndPlaySFX("s4pz1_mazepathwrong");
		for (int i = 0; i < (int)_keyTooths.size(); i++)
		{
			_keyTooths[i]->EnableTooth(false);
			_keyTooths[i]->EnableKeyHole(true);
		}
	}

	void PZ_Controller::OnUnlock()
	{
		for (int i = 0; i < (int)(_locks.size()); i++)
		{
			if (!_locks[i]->getUnlocked())
			{
				_isPZL2Solved = false;
				return;
			}
		}

		_isPZL2Solved = true;
	}

	void PZ_Controller::Skip()
	{
		for (int i = 0; i < (int)(_locks.size()); i++)
		{
			_locks[i]->SkipInitiated();
		}

		_isPZL2Solved = true;
	}

}

using namespace Level5_Scene4_PZ1;
const CRectangle  PZSceneRect(172.0f,16.0f,1026.0f,526.0f);


L5_S4_PZHandCuff::L5_S4_PZHandCuff()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S4\\PZ1\\PZ1.SCN");

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

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s4HandCuffPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s4HandCuffPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s4HandCuffPuzzleAmb,true);
	}
	
	//MUSIC_SFX
	
	

	_pzState = ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffState;
	_stateMachine.push((L5_S4_PZState)_pzState);

	InitPuzzle();

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S4_PZHandCuff]  = true;
}

void L5_S4_PZHandCuff::InitPuzzle()
{
	_pzController = new Level5_Scene4_PZ1::PZ_Controller();
	_pzController->InitPuzzle(HOScene);

	switch (_pzState)
	{
	case L5_S4_PZ_Inactive:
		_pzController->ShowKey(false);
		break;

	case L5_S4_PZ_Level2Playing:
		{
			_pzController->ActivateL2();
			Hud->InvBox_StartPuzzleMode();
		}
		break;

	case L5_S4_PZ_Level1Playing:
	case L5_S4_PZ_Active:
		{
			_pzController->ActivateL1();
			Hud->InvBox_StartPuzzleMode();
		}
		break;

	case L5_S4_PZ_Solved:
		break;
	case L5_S4_PZ_Completed:
		break;
	default:
		break;
	}
}

void L5_S4_PZHandCuff::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL5theme) )
	{
		Control::Audio->Samples[aTrackL5theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL5theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL5theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL5puzzle) )
				  Control::Audio->PlaySample(aTrackL5puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL5puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL5puzzle, true);
		}
	}
}

void L5_S4_PZHandCuff::Update()
{
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

	if (_stateMachine.top() != _pzState)
	{
		ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffState = _pzState;
		_stateMachine.push((L5_S4_PZState)_pzState);
	}

	switch (_pzState)
	{
	case L5_S4_PZ_Inactive:
		{
			if (PZSceneRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L5_S4_HandCuffClickWithoutKey);
				}
			}
		}
		break;

	case L5_S4_PZ_Active:
		{
			_pzController->ActivateL1();
			Hud->InvBox_StartPuzzleMode();
			_pzState++;
		}
		break;
	case L5_S4_PZ_Level1Playing:
		{
			_pzController->L1Update();

			if (_pzController->getIsL1Solved())
			{
				_pzState = L5_S4_PZ_Level1Complete;
			}
		}
		break;

	case L5_S4_PZ_Level1Complete:
		{
			ProfileSystem::Instance->ActiveProfile.L5_S4_Level1Completed = true;
			_pzController->ActivateL2();
			Hud->InvBox_StartPuzzleMode();
			_pzState = L5_S4_PZ_Level2Playing;
		}
		break;

	case L5_S4_PZ_Level2Playing:
		{
			_pzController->L2Update();

			if (_pzController->getIsL2Solved())
			{
				_pzState = L5_S4_PZ_Solved;
			}
		}
		break;

	case L5_S4_PZ_Skip:
		{
			_pzController->Skip();
			_pzState = L5_S4_PZ_Solved;
		}
		break;

	case L5_S4_PZ_Solved:
		{
			Control::Audio->QuickLoadAndPlaySFX("s4pz1_puzzlecomplete");
			AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			if (_handleExtrasGameComplete())
			{
				_pzState = L5_S4_PZ_GameForExtrasComplete;
				return;
			}

			Hud->InvBox_StopPuzzleMode();
			_pzState++;
		}
		break;

	case L5_S4_PZ_Completed:
		{
			ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S4_Dungeon;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
			_pzState++;
		}
		break;

	case L5_S4_PZ_GameForExtrasComplete:
		break;

	default:
		_pzState++;
		break;
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S4_Dungeon;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	

	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L5_S4_Dungeon);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L5_S4_Dungeon);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S4_Dungeon, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S4_Dungeon;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}
}

void L5_S4_PZHandCuff::ReceiveMessage(int val)
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
			if (itemVal == eInv_L5_S5_Keyhandcuff)
			{
				_pzState = L5_S4_PZ_Active;

				Control::Audio->QuickLoadAndPlaySFX("s4pz1_invkeyfix");

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[itemVal] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(itemVal);
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}	
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L5_S4_PZHandCuff::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_pzState = L5_S4_PZ_Skip;
}

void L5_S4_PZHandCuff::ProcessNonHOHint()
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
				{
					if (_pzState == L5_S4_PZ_Inactive)
					{
						if (ProfileSystem::Instance->ActiveProfile.L5_S5_InvHandcuffKeyTaken)
						{
							Hud->ShowHintForInvItem(eInv_L5_S5_Keyhandcuff, &MazeArea);
							return;
						}
					}
				}


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

L5_S4_PZHandCuff::~L5_S4_PZHandCuff()
{
	SAFE_DELETE(_pzController);
	Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(ResetDia);

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL5s4HandCuffPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL5s4HandCuffPuzzleAmb);
	//MUSIC_SFX
}
void L5_S4_PZHandCuff::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(_pzState < 1)
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
			for (int i = 0; i < 6; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L5_S4_KeyMatchComplete[i] = false;
			}
			ProfileSystem::Instance->ActiveProfile.L5_S4_Level1Completed = false;
			ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffState = 1;
			ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffComplete = false;
			for (int i = 0; i < 3; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L5_S4_PZHandCuffLockUnlocked[i] = false;
			}
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S4_PZHandCuff;
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
void L5_S4_PZHandCuff::NavigateToScene()
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