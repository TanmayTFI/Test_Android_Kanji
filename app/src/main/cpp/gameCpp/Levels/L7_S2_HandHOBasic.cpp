
//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S2_HandHOBasic.h"
#include "Util.h"
#include "HOScene.h"

const CRectangle  ObjectMatchPos(587.0f,396.0f,190.0f,144.0f);
const CRectangle  LeftSide(435.0f,326.0f,111.0f,145.0f);
const CRectangle  RightSide(821.0f,321.0f,100.0f,157.0f);
const CRectangle  BoxSidesRect[3] = {LeftSide, ObjectMatchPos, RightSide};

const int NoOfObjects = 6;
const int ZTop = 100;

namespace Level7_Scene_2
{
	HiddenObject::HiddenObject (CObject* obj, CObject* fullObj)
	{
		LoadSprite(obj->Filename);
		SetPos(&obj->Pos);
		SetZ(obj->ZOrder);

		_fullVersion = fullObj;
		_fullVersion->SetScale(1.2f);
		_fullVersion->SetZ(ZTop);
		_fullVersion->SetAlpha(0);

		_isSelected = false;
		_isCompleted = false;

		obj->SetEnable(false);

	}

	HiddenObject::~HiddenObject ()
	{

	}

	void HiddenObject::CustomUpdate()
	{
		if (_isCompleted || _fullVersion->IsTweening)
			return;

		CPoint mPos = Control::Input->Mpos();

		if (_isSelected)
		{
			_fullVersion->SetPos(&mPos);
			return;
		}

		if (Control::Input->LBclicked() &&
			!_delegate->_selectedObj &&
			_areaPoly->Intersection(&mPos))
		{
			_isSelected = true;
			FadeOut(3);
			_fullVersion->FadeIn(3);
			_delegate->onSelection(this);
		}
	}

	void HiddenObject::setCompleted()
	{
		_isCompleted = true;
		SetEnable(false);
		_fullVersion->SetEnable(false);
	}

	void HiddenObject::OnComplete()
	{
		_fullVersion->FadeOut(3);
		_isCompleted = true;
	}

	void HiddenObject::BackToPosition()
	{
		float valArray1[5] = {-1, _actualPos.x, _actualPos.y, -1, -1};
		_fullVersion->TweenTo(valArray1, 0.25f, 0, true, CC_CALLBACK_1(HiddenObject::BackToPosComplete, this));
	}

	void HiddenObject::BackToPosComplete(CObject* obj)
	{
		_isSelected = false;
		FadeIn(4);
		_fullVersion->FadeOut(4);
	}




	Symbol::Symbol(int index, bool isComplete)
	{
		_currentSide = None;
		_isActive = false;
		_index = index;
		_isCompleted = isComplete;
	}

	Symbol::~Symbol()
	{
		
	}

	void Symbol::setMatchObject(HiddenObject* ho)
	{
		_matchObject = ho;

		if(_isCompleted)
			_matchObject->setCompleted();
	}

	void Symbol::SetSymbolForSide(CObject* symbol, CObject* doorGlow, SymbolSide side)
	{
		_symbolObjs.insert(std::make_pair(side, symbol));
		_doorUnlockGlowObjs.insert(std::make_pair(side, doorGlow));
		
		if (symbol)
		{
			if (side != _currentSide)
			{
				symbol->Show(false);
				doorGlow->Show(false);
			}

			if (!_isCompleted)
				doorGlow->Show(false);
			else
				symbol->SetAnimation(1);
		}
	}

	void Symbol::SetCurrentSide(SymbolSide side)
	{
		if (_currentSide != None)
		{
			if (_symbolObjs[side])
			{
				_symbolObjs[side]->Show(true);

				if (_isCompleted)
					_doorUnlockGlowObjs[side]->Show(true);
			}

			if (_symbolObjs[_currentSide])
			{
				_symbolObjs[_currentSide]->Show(false);
				_doorUnlockGlowObjs[_currentSide]->Show(false);
			}
		}
		
		_currentSide = side;
	}

	void Symbol::UpdateSymbol(CPoint mPos)
	{
		std::map<SymbolSide, CObject*>::iterator itr;
		for (itr = _symbolObjs.begin(); itr != _symbolObjs.end(); itr++)
		{
			if (itr->second && itr->second->Visible) 
			{
				if (itr->second->GetWorldRect().Intersection(&mPos))
					OnMouseOver();
				else
					OnMouseOut();
			}
		}
	}

	bool Symbol::CopyObject(HiddenObject* obj)
	{
		if (!_isActive)
			return false;

		if (_matchObject == obj)
		{
			OnMatchFound();
			return true;
		}

		Control::Audio->QuickLoadAndPlaySFX("s2ho2_itemplacedwrong");
		return false;
	}

	void Symbol::OnMouseOver()
	{
		if(!_isCompleted && _symbolObjs[_currentSide])
			_symbolObjs[_currentSide]->PlayAnimation(2);
	}

	void Symbol::OnMouseOut()
	{
		if(!_isCompleted && _symbolObjs[_currentSide] && _symbolObjs[_currentSide]->Visible)
			_symbolObjs[_currentSide]->PlayAnimation(0);
	}

	void Symbol::OnMatchFound()
	{
		Control::Audio->QuickLoadAndPlaySFX("s2ho2_itemplacedright");
		_isCompleted = true;
		_matchObject->OnComplete();

		if (_doorUnlockGlowObjs[_currentSide])
			_doorUnlockGlowObjs[_currentSide]->Show(true);

		std::map<SymbolSide, CObject*>::iterator itr;
		for (itr = _symbolObjs.begin(); itr != _symbolObjs.end(); itr++)
		{
			if (itr->second)
				itr->second->PlayAnimation(1);
		}
	}

	void Symbol::HideDoorUnlockGlows()
	{
		std::map<SymbolSide, CObject*>::iterator itr;
		for (itr = _doorUnlockGlowObjs.begin(); itr != _doorUnlockGlowObjs.end(); itr++)
		{
			if (itr->second)
				itr->second->Show(false);
		}
	}





	HOController::HOController()
	{
		_selectedObj = NULL;
		_activeSymbol = NULL;
		_delegate = NULL;
		_symbChangeDelay = 0;
	}

	HOController::~HOController()
	{
		for (int i = 0; i < (int)_objPolyList.size(); i++)
		{
			SAFE_DELETE(_objPolyList[i]);
		}

		for (int i = 0; i < (int)_symbols.size(); i++)
		{
			SAFE_DELETE(_symbols[i]);
		}
	}

	void HOController::InitGame(CHOScene* scene)
	{
		_isCompleted = false;

		for (int i = 0; i < NoOfObjects; i++)
		{
			CObject* object = scene->GetObjectByID("Object_"+MKSTR(i));
			CObject* fullObject = scene->GetObjectByID("Object_Full_"+MKSTR(i));
			HiddenObject* ho = new HiddenObject(object, fullObject);
			_hiddenObj.push_back(ho);
			ho->setID(i);
			ho->setAreaPoly(_objPolyList[i]);
			ho->setActualPos(object->Pos);
			ho->setDelegate(this);
			scene->PushObject(ho);
			_shiftList.push_back(i);

			Symbol *symbol = new Symbol(i, _delegate->getIsSymbolCompleted(i));
			symbol->setMatchObject(ho);
			_symbols.push_back(symbol);
			symbol->SetCurrentSide(SymbolSide(i));

			for (int j = 0; j < 6; j++)
			{
				CObject* objSymbol = scene->GetObjectByID("Symbol_"+MKSTR(i)+"_"+MKSTR(j));
				CObject* doorGlow = scene->GetObjectByID("DoorGlow_"+MKSTR(i)+"_"+MKSTR(j));
				SymbolSide side = SymbolSide(j);

				symbol->SetSymbolForSide(objSymbol, doorGlow, side);
			}
		}

		_btnLeftRotate = new UIButton(scene->GetObjectByID("ArrowLeft"), CC_CALLBACK_0(HOController::ShiftLeftAction, this));
		scene->PushObject(_btnLeftRotate);
		_btnRightRotate = new UIButton(scene->GetObjectByID("ArrowRight"), CC_CALLBACK_0(HOController::ShiftRightAction, this));
		scene->PushObject(_btnRightRotate);

		_blurBox = scene->GetObjectByID("motionBlur");
		_blurBox->Show(false);
		_hoState = State_GameStart;
	}

	void HOController::Update()
	{
		CPoint mPos = Control::Input->Mpos();

		switch (_hoState)
		{
		case State_GameStart:
			{
				std::list<int>::iterator itr = _shiftList.begin();
				for (int i = 0; i < (int)_shiftList.size(); i++, itr++)
				{
					if (i == 1)
					{
						_symbols[*itr]->setIsActive(true); 
						_activeSymbol = _symbols[*itr];
					}
					else
						_symbols[*itr]->setIsActive(false);
				}
				_hoState = State_WaitForInteraction;
			}
			break;

		case State_HoldingObject:
			{
				if (Control::Input->LBclicked())
				{
					_btnRightRotate->Enable();
					_btnLeftRotate->Enable();

					if(ObjectMatchPos.Intersection(&mPos))
					{
						if (_activeSymbol->CopyObject(((HiddenObject*)_selectedObj)))
						{
							_hoState = State_OnSymbolMatch;
							return;
						}
					}

					_hoState = State_OnSymbolMismatch;
				}

				std::list<int>::iterator itr = _shiftList.begin();
				for (itr = _shiftList.begin(); itr != _shiftList.end(); itr++)
				{
					_symbols[*itr]->UpdateSymbol(mPos);
				}
			}
		case State_WaitForInteraction:
			{
				if (_btnLeftRotate->GetWorldRect().Intersection(&mPos) || _btnRightRotate->GetWorldRect().Intersection(&mPos))
				{
					Cursor::SetMode(CA_HandPointy);
				}

				for (int i = 0; i < (int)_hiddenObj.size(); i++)
				{
					_hiddenObj[i]->CustomUpdate();
				}
			}
			break;

		case State_PlacingOnSymbol:
			break;

		case State_OnSymbolMatch:
			{
				_delegate->onSymbolComplete(_activeSymbol->getIndex());
				_selectedObj = NULL;
				_hoState = State_CheckIsCompleted;
			}
			break;

		case State_OnSymbolMismatch:
			{
				std::list<int>::iterator itr = _shiftList.begin();
				for (itr = _shiftList.begin(); itr != _shiftList.end(); itr++)
				{
					_symbols[*itr]->OnMouseOut();
				}

				((HiddenObject*)_selectedObj)->BackToPosition();
				_selectedObj = NULL;
				_hoState = State_WaitForInteraction;
			}
			break;

		case State_RotatingBox:
			{
				if (_symbChangeDelay > 0.0f)
				{
					_symbChangeDelay -= Control::LogicRate;
					return;
				}

				std::list<int>::iterator itr = _shiftList.begin();
				for (int i = 0; i < (int)_shiftList.size(); i++, itr++)
				{
					_symbols[*itr]->SetCurrentSide(SymbolSide(i));
					
					if (i == 1)
					{
						_symbols[*itr]->setIsActive(true); 
						_activeSymbol = _symbols[*itr];
					}
					else
						_symbols[*itr]->setIsActive(false);
				}

				_btnRightRotate->SetIsInteractionEnabled(true);
				_btnLeftRotate->SetIsInteractionEnabled(true);
				_symbChangeDelay = 0.0f;
				_hoState = State_WaitForInteraction; 
			}
			break;

		case State_SkipGame:
			{
				for (int i = 0; i < (int)_symbols.size(); i++)
				{
					_symbols[i]->setIsActive(true);
					_symbols[i]->CopyObject(_hiddenObj[i]);
					_delegate->onSymbolComplete(_symbols[i]->getIndex());
					_hiddenObj[i]->setCompleted();
				}

				_hoState = State_CheckIsCompleted;
			}
			break;

		case State_CheckIsCompleted:
			{
				for (int i = 0; i < (int)_symbols.size(); i++)
				{
					if (!_symbols[i]->getIsCompleted())
					{
						_hoState = State_WaitForInteraction;
						return;
					}
				}
				
				_hoState = State_OnComplete;
			}
			break;

		case State_OnComplete:
			{
				_isCompleted = true;
				OnComplete();
			}
			break;

		default:
			_hoState++;
			break;
		}
	}

	void HOController::OnComplete()
	{	
		Control::Audio->QuickLoadAndPlaySFX("s2ho2_hosolved");
		for (int i = 0; i < (int)_symbols.size(); i++)
		{
			_symbols[i]->HideDoorUnlockGlows();
		}

		if (_isCompleted)
		{
			_btnLeftRotate->FadeOut();
			_btnRightRotate->FadeOut();
			return;
		}

		_btnLeftRotate->SetEnable(false);
		_btnRightRotate->SetEnable(false);		
	}

	void HOController::ShiftLeftAction()
	{
		Control::Audio->QuickLoadAndPlaySFX("s2ho2_boxrotate");
		int index = _shiftList.front();
		_shiftList.pop_front();
		_shiftList.push_back(index);

		_blurBox->FadeInOut(0.25f);
		_symbChangeDelay = 0.12f;
		_btnLeftRotate->SetIsInteractionEnabled(false);
		_hoState = State_RotatingBox;
	}

	void HOController::ShiftRightAction()
	{
		Control::Audio->QuickLoadAndPlaySFX("s2ho2_boxrotate");
		int index = _shiftList.back();
		_shiftList.pop_back();
		_shiftList.push_front(index);

		_blurBox->FadeInOut(0.25f);
		_symbChangeDelay = 0.12f;
		_btnRightRotate->SetIsInteractionEnabled(false);
		_hoState = State_RotatingBox;
	}

	void HOController::onSelection(CObject *obj)
	{
		Control::Audio->QuickLoadAndPlaySFX("s2ho2_itemclicked");
		_selectedObj = obj;
		_hoState = State_HoldingObject;
		_btnRightRotate->Disable();
		_btnLeftRotate->Disable();
	}

	void HOController::SkipGame()
	{
		_hoState = State_SkipGame;
	}

	void HOController::SetObjPolys(std::vector<CPolygon*> polyList)
	{
		_objPolyList = polyList;
	}

}