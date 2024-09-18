
#include "L8_S2_PZBubble.h"
#include "Util.h"

namespace L8_S1_BubblePuzzle
{
	Bubble::Bubble(int index)
	{
		_index = index;
		_symbol = NULL;
		_delay = 0.0f;
		_symbolMinAlpha = 0.0f;
		_symbolMaxAlpha = 1.0f;
		_state = State_Hide;

		SetIsInteractionEnabled(false);

		_breakfx1 = NULL;
		_breakfx2 = NULL;
	}

	Bubble::~Bubble()
	{
		SAFE_DELETE(_symbol);
		_breakfx1 = NULL;
		_breakfx2 = NULL;
	}

	void Bubble::CreateSymbol(SceneData data, int id)
	{
		_symbol = new Symbol(id);
		_symbol->LoadInit(data.spriteString, (float)data.x, (float)data.y, ZOrder-1);
		_symbol->UID = data.uid + MKSTR(_index);
		_symbol->SetAlpha(0.0f);
	}

	void Bubble::ActivateWith(float Delay)
	{
		_delay = Delay;
		_symbol->SetAlpha(_symbolMinAlpha);
		_state = State_ReadyToActivate;
	}

	void Bubble::Distroy()
	{
		Status = SPR_VISIBLE;
		_breakfx1->Pos = Pos;
		_breakfx2->Pos = Pos;

		_breakfx1->Start();
		_breakfx2->Start();

		float valbub[5] = {-1, -1, -1, 0, -1};
		TweenTo(valbub, 0.2f, TW_EASEQUADOUT, true);

		Control::Audio->QuickLoadAndPlaySFX("s2p2_BubbleBreak");
		// Bubble distroy Animation
	}

	bool Bubble::OnClick()
	{
		if (_state != State_Active)
			return false;

		_symbol->PlayAnimation(0);
		_symbol->SetAlpha(_symbolMaxAlpha);
		_state = State_Selected;
		return true;
	}

	void Bubble::OnMousOver()
	{
		if(_state != State_Active)
			return;

		Cursor::SetMode(CA_HandPointy);
	}

	void Bubble::setIsMatched(bool m)
	{
		if (m)
		{
			_symbol->PlayAnimation(1);
			_delay = 0.4f;
			_state = State_Match;
		}
		else
		{
			_symbol->PlayAnimation(2);
			_delay = 0.8f;
			_state = State_NotMatch;
		}
	}

	void Bubble::SetAlpha(float alpha)
	{
		CObject::SetAlpha(alpha);
		if (alpha <= _symbolMinAlpha || _state == State_Selected)
		{
			_symbol->SetAlpha(alpha);
		}
	}

	void Bubble::Update(float ds)
	{
		CObject::Update(ds);

		if (_symbol)
		{
			_symbol->Pos = Pos;
			_symbol->SetScale(pSprite->Scale);
		}

		switch (_state)
		{
		case State_Hide:
			break;
		case State_ReadyToActivate:
			{
				if (_delay > 0.0f)
				{
					_delay -= ds;
					if (_delay <= 0.0f)
					{
						float valbub[5] = {-1, -1, -1, 1, -1};
						TweenTo(valbub, 1, TW_EASEBACKOUT, true);
						_state = State_Activating;
					}
				}
			}
			break;

		case State_Activating:
			{
				if (!IsTweening)
				{
					SetIsInteractionEnabled(true);
					Animate(5.0f, 5.0f, 0.0f, 0.0f, 5.0f, 45.0f * _index);
					_state = State_Active;
				}
			}
			break;

		case State_Active:
			{

			}
			break;

		case State_Selected:
			break;

		case State_Match:
			{
				if (_delay > 0.0f)
				{
					_delay -= ds;
					if (_delay <= 0.0f)
					{
						Distroy();
						_state = State_Complete;
					}
				}
			}
			break;

		case State_NotMatch:
			{
				if (_delay > 0.0f)
				{
					_delay -= ds;
					if (_delay <= 0.0f)
					{
						_symbol->PlayAnimation(0);
						_symbol->SetAlpha(_symbolMinAlpha);
						_state = State_Active;
					}
				}
			}

			break;

		case State_Shuffling:
			{
				 if (!IsTweening)
				{
					SetPos(&Pos);
					Animate(5.0f, 5.0f, 0.0f, 0.0f, 5.0f, 45.0f * _index);
					_state = State_Active;
				}
			}
			break;

		case State_Complete:
			break;

		default:
			break;
		}
	}

	void Bubble::Reset()
	{
		_state = State_Hide;
	}
	
	void Bubble::setBackToActive()
	{
		if (getIsComplete())
			return;

		_symbol->PlayAnimation(0);
		_symbol->SetAlpha(_symbolMinAlpha);
		_state = State_Active;
	}

	void Bubble::InitForShuffling(CPoint newPos)
	{
		if (_state == State_Complete)
			return;
		
		Status = SPR_VISIBLE;

		float val[5] = {-1, newPos.x, newPos.y, -1, -1};
		TweenTo(val, 1.0f, TW_EASEQUADIN, true);

		_state = State_Shuffling;
	}

	bool Bubble::getIsComplete()
	{
		if (_state == State_Complete || _state == State_Match)
			return true;

		return false;
	}




	PZBubbleScene::PZBubbleScene()
	{
		_currentLevel = PZLevel_None;
		_delegate = NULL;
		_bubbles.clear();
		_bubblePosList.clear();
		_shuffleDelay = 0.0f;
		_selectedBubbles.clear();
		_prevSelectedBubbles.clear();

		s2BubbleBreakfx1FX = new SuperFX("PRT\\L8\\s2BubbleBreakfx1.INI", PointSystem::CreateCPoint(682,172,0), 1100); 
		s2BubbleBreakfx2FX = new SuperFX("PRT\\L8\\s2BubbleBreakfx2.INI", PointSystem::CreateCPoint(682,172,0), 1100); 
	}

	PZBubbleScene::~PZBubbleScene()
	{
		SAFE_DELETE(s2BubbleBreakfx1FX);
		SAFE_DELETE(s2BubbleBreakfx2FX);

		_bubbles.clear();
		_bubblePosList.clear();
	}

	static int SymbolForBubble[] = {3, 4, 1, 0, 4, 0, 3, 2, 1, 2};

	void PZBubbleScene::Load(const std::string& sceneStr)
	{
		CHOScene::Load(sceneStr);
		SetZOff(50);

		_mask = GetObjectByID("Mask");
		_mask->SetScale(5);
		_mask->SetAlpha(0);

		for (int i = 0; i < (int)_bubbles.size(); i++)
		{
			_bubbles[i]->CreateSymbol(_symbolsData[SymbolForBubble[i]], SymbolForBubble[i]);
			_bubbles[i]->SetScale(0);
		}
	}


	void PZBubbleScene::LoadLevel(int level)
	{
		_currentLevel = (PZLevel)level;
		ShuffleBubbles(false);

		switch (_currentLevel)
		{
		case PZLevel_None:
			break;
		case PZLevel_1:
			{
				float valMask[5] = {1, -1, -1, -1, -1};
				_mask->TweenTo(valMask, 1, 0, true);

				for (int i = 0; i < (int)_bubbles.size(); i++)
				{
					_bubbles[i]->setSymbolMinAlpha(0.4f);
					_bubbles[i]->ActivateWith(float((i+1)*0.1f));
				}
			}
			break;

		case PZLevel_2:
			{
				float valMask[5] = {1, -1, -1, -1, -1};
				_mask->TweenTo(valMask, 1, 0, true);

				for (int i = 0; i < (int)_bubbles.size(); i++)
				{
					_bubbles[i]->setSymbolMinAlpha(0.0f);
					_bubbles[i]->ActivateWith(float((i+1)*0.1f));
				}
			}
			break;

		case PZLevel_3:
			{
				_shuffleDelay = 0.0f;
				float valMask[5] = {1, -1, -1, -1, -1};
				_mask->TweenTo(valMask, 1, 0, true);

				for (int i = 0; i < (int)_bubbles.size(); i++)
				{
					_bubbles[i]->setSymbolMinAlpha(0.0f);
					_bubbles[i]->ActivateWith(float((i+1)*0.1f));
				}
			}
			break;
		
		default:
			break;
		}
	}


	void PZBubbleScene::update(float ds)
	{
		CPoint mpos = Control::Input->Mpos();
		Cursor::SetMode(CA_Normal);

		for (int i = 0; i < (int)_bubbles.size(); i++)
		{
			if (_bubbles[i]->getState() == State_Activating)
				return;
		}

		Result result = Result_Nothing;

		if (Control::Input->LBclicked())
		{
			for (int i = 0; i < (int)_prevSelectedBubbles.size(); i++)
			{
				_prevSelectedBubbles[i]->setBackToActive();
			}

			_prevSelectedBubbles.clear();
		}

		if (_shuffleDelay <= 0.0f)
		{
			for (int i = 0; i < (int)_bubbles.size(); i++)
			{
				if (_bubbles[i]->GetCurrentMouseState() == eMouseState_Click && _bubbles[i]->OnClick())
				{
					_selectedBubbles.push_back(_bubbles[i]);

					if ((int)_selectedBubbles.size() == 2)
					{
						result = (_selectedBubbles.back()->getSymbol()->getID() == _selectedBubbles.front()->getSymbol()->getID()) ? Result_Match : Result_NotMatch;
						_selectedBubbles.front()->setIsMatched(bool(result != Result_NotMatch));
						_selectedBubbles.back()->setIsMatched(bool(result != Result_NotMatch));

						_prevSelectedBubbles = _selectedBubbles;
						_selectedBubbles.clear();
					}
					else
					{
						Control::Audio->QuickLoadAndPlaySFX("s2pz2_BubbleSelect");	
					}

					break;
				}
				else if (_bubbles[i]->GetCurrentMouseState() == eMouseState_Over)
				{
					_bubbles[i]->OnMousOver();
				}
			}
		}
		

		if (result == Result_Match)
		{
			bool levelCompleted = true;
			for (int i = 0; i < (int)_bubbles.size(); i++)
			{
				if(!_bubbles[i]->getIsComplete())
				{
					levelCompleted = false;
				}
			}

			if (levelCompleted)
			{
				HandleComplete();
				return;
			}

			Control::Audio->QuickLoadAndPlaySFX("s2p2_BubbleCorrect");
		}
		else if(result == Result_NotMatch)
		{
			Control::Audio->QuickLoadAndPlaySFX("s2pz2_BubbleWrong");
		}

		switch (_currentLevel)
		{
		case PZLevel_None:
			break;
		case PZLevel_1:
			{
				
			}
			break;
		
		case PZLevel_2:
			{
				
			}
			break;

		case PZLevel_3:
			{
				if (result == Result_NotMatch)
				{
					_shuffleDelay = 1.0f;
				}

				if (_shuffleDelay > 0.0f)
				{
					_shuffleDelay -= ds;
					if (_shuffleDelay <= 0.0f)
					{
						ShuffleBubbles(true);
					}
				}
			}
			break;

		default:
			break;
		}
	}

	CObject* PZBubbleScene::CreateObject(SceneData data)
	{
		if (data.objType == "Symbol")
		{
			_symbolsData.push_back(data);
			return NULL;
		}
		else if(data.objType == "Bubble")
		{
			Bubble *bubble = new Bubble((int)_bubbles.size());
			bubble->LoadInit(data.spriteString, (float)data.x, (float)data.y, data.ZOrder+10);
			bubble->UID = data.uid;
			bubble->SetMouseInteractionArea(bubble->GetWorldRect().w/2);
			bubble->setBreakfx1(s2BubbleBreakfx1FX);
			bubble->setBreakfx2(s2BubbleBreakfx2FX);
			_bubbles.push_back(bubble);

			CPoint pos((float)data.x, (float)data.y, 0);
			_bubblePosList.push_back(pos);

			return bubble;
		}
		else
		{
			return CScene2D::CreateObject(data);
		}

		return NULL;
	}

	void PZBubbleScene::ShuffleBubbles(bool tween)
	{
		std::random_shuffle ( _bubblePosList.begin(), _bubblePosList.end() );

		for (int i = 0; i < (int)_bubbles.size(); i++)
		{
			if (tween)
			{
				Control::Audio->QuickLoadAndPlaySFX("s2p2_ShufflingBubble");
				_bubbles[i]->InitForShuffling(_bubblePosList[i]);
			}
			else
			{
				_bubbles[i]->SetPos(&_bubblePosList[i]);
			}
		}
	}

	void PZBubbleScene::HandleComplete()
	{
		_delegate->OnLevelComplete((int)_currentLevel);

		float valMask[5] = {0, -1, -1, -1, -1};
		_mask->TweenTo(valMask, 1, 0, true);
	}

}