//===========================================================
// 
// Framework
// UnitAnimator
//===========================================================

#include "UnitAnimator.h"
#include "Util.h"

UnitAnimator::UnitAnimator(CObject* obj)
{
	_targetObj = obj;
	_superFX = NULL;

	_isASuperFX = false;
	
	_animTime=0;
	_totalAnimTime=0;

	_numKeys = 0;
	_currentKey = 0;

	_offsetX = _offsetY = 0;

	_paused = false;
}

UnitAnimator::UnitAnimator(SuperFX* superFX)
{
	_targetObj = superFX;

	_superFX = superFX;
	_isASuperFX = true;

	_animTime=0;
	_totalAnimTime=0;

	_numKeys = 0;
	_currentKey = 0;

	_offsetX = _offsetY = 0;

	_paused = false;
}

void UnitAnimator::SetAnimKeys(std::vector<UNITKEY *> keysArray, float totalAnimTime)
{
	_animKeys = keysArray;
    _numKeys = (int) keysArray.size();  //Sac: converted Implicit to explicit


	_totalAnimTime = totalAnimTime;
}

void UnitAnimator::Update(float ms)
{
	if( _paused )
		return;

	if( _numKeys>0 )
	{
		_animTime += ms;//already in milliseconds
		if(_animTime<=_totalAnimTime)
		{
			int NextKey = _currentKey+1;
			if( NextKey<_numKeys && _animTime > _animKeys[_currentKey]->keytime )
			{
				//if _animTime covers NextKey
				if( _animTime >= _animKeys[NextKey]->keytime )
				{
					_currentKey = NextKey;
					NextKey += 1;
				}

				if( NextKey<_numKeys )
				{
					float disp = _animTime - _animKeys[_currentKey]->keytime;//displacement
					float timeSpan = _animKeys[NextKey]->keytime - _animKeys[_currentKey]->keytime;

					float easeValue = _easing(_animKeys[NextKey]->easeType, disp, 0, 1, timeSpan);

					if(_isASuperFX)
					{
						float val = _animKeys[_currentKey]->alpha+(_animKeys[NextKey]->alpha-_animKeys[_currentKey]->alpha)*easeValue;
						if(val > 0.5f)
						{
							if(!_superFX->Active)
								_superFX->Start();
						}
						else
						{
							if(_superFX->Active)
								_superFX->StopEmit();
						}
						_superFX->Pos.x = (float)(_animKeys[_currentKey]->xpos+(_animKeys[NextKey]->xpos-_animKeys[_currentKey]->xpos)*easeValue) + _offsetX;
						_superFX->Pos.y = (float)(_animKeys[_currentKey]->ypos+(_animKeys[NextKey]->ypos-_animKeys[_currentKey]->ypos)*easeValue) + _offsetY;
					}
					else
					{
						_targetObj->SetAlpha(_animKeys[_currentKey]->alpha+(_animKeys[NextKey]->alpha-_animKeys[_currentKey]->alpha)*easeValue);
						_targetObj->Pos.x = (float)(_animKeys[_currentKey]->xpos+(_animKeys[NextKey]->xpos-_animKeys[_currentKey]->xpos)*easeValue) + _offsetX;
						_targetObj->Pos.y = (float)(_animKeys[_currentKey]->ypos+(_animKeys[NextKey]->ypos-_animKeys[_currentKey]->ypos)*easeValue) + _offsetY;
						_targetObj->SetRotate((_animKeys[_currentKey]->rot+(_animKeys[NextKey]->rot-_animKeys[_currentKey]->rot)*easeValue));
						_targetObj->SetScale(_animKeys[_currentKey]->scale+(_animKeys[NextKey]->scale-_animKeys[_currentKey]->scale)*easeValue);
					}
				}
			}
		}
		else
		{
			_currentKey = _numKeys-1;
			_setObjProperties(_currentKey);
		}
	}
}

void UnitAnimator::SetAlphaForObjects(float alpha)
{
	if(_targetObj != NULL)
		_targetObj->SetAlpha(alpha);
}


void UnitAnimator::FadeOutObject(float speed)
{
	if(_targetObj != NULL)
		_targetObj->FadeOut(speed);
}


void UnitAnimator::setEnableObject(bool f)
{
	if(_targetObj != NULL)
		_targetObj->SetEnable(f);
}

void UnitAnimator::SetPosOffset(int x, int y )
{
	_offsetX = x;
	_offsetY = y;
}

UnitAnimator::~UnitAnimator()
{
	_targetObj = NULL;
	_superFX = NULL;
	int size =(int)_animKeys.size();
	for(int i=0; i < size; ++i)
	{
		delete _animKeys[i];
	}
	_animKeys.clear();
}

float UnitAnimator::_easing(int ease, float t,float b,float c,float d)
{
	float val=c;
	float s=1.70158f;
	switch (ease)
	{
	case eEase_COSINE:	val = (1-Elixir::Cos(t/d*180.0f))*0.5f; break;
	case eEase_LINEAR:	val = (c * t / d + b);		break;
	case eEase_QUADIN:	t /= d;	val = (c*t*t + b); break;
	case eEase_QUADOUT:	t /= d;val = (-c * t*(t-2.0f) + b); break;
	case eEase_BOUNCEOUT: 
		if ((t/=d) < (1.0f/2.75f)) {
			val = c*(7.5625f*t*t) + b;
		} else if (t < (2.0f/2.75f)) {
			//Sac: this could be problematic, as we are assigning t and also using it for multiplication in same line
			// we better assign it first and then use it for multiplication
			//modifying as below...
			//val = c*(7.5625f*(t-=(1.5f/2.75f))*t + .75f) + b;
			t-=(1.5f/2.75f);
			val = c*(7.5625f*(t)*t + .75f) + b;
		} else if (t < (2.5f/2.75f)) {
			//val = c*(7.5625f*(t-=(2.25f/2.75f))*t + .9375f) + b;
			t-=(2.25f/2.75f);
			val = c*(7.5625f*(t)*t + .9375f) + b;

		} else {
			//val = c*(7.5625f*(t-=(2.625f/2.75f))*t + .984375f) + b;
			t-=(2.625f/2.75f);
			val = c*(7.5625f*(t)*t + .984375f) + b;

		}
		break;
	case eEase_ELASTICOUT:
		if (CUtil::Equal(t, 0.0f))				val= b;
		else if (CUtil::Equal((t/=d), 1.0f))	val =b+c;
		else
		{
			float p = d*.3f;
			float a = c;
			float s = p/4.0f;

			val = (a*powf(2.0f,-10.0f*t) * Elixir::Sin( Elixir::ToDegrees((t*d-s)*(6.2832f)/p) ) + c + b);//2.0f*3.1416f
		}
		break;
	case eEase_BACKOUT:
		t=t/d-1.0f;
		val = c*((t)*t*((s+1.0f)*t + s) + 1.0f) + b;
		break;
	default:
		break;
	}
	return val;
}

void UnitAnimator::_setObjProperties(int keyID)
{
	if(_numKeys>0)
	{
		if(_isASuperFX)
		{
			//_superFX->StopImmediate();
			float val = _animKeys[keyID]->alpha;
			if(val > 0.5f)
			{
				if(!_superFX->Active)
					_superFX->Start();
			}
			else
			{
				if(_superFX->Active)
					_superFX->StopEmit();
			}
			_superFX->Pos.x = (float)_animKeys[keyID]->xpos + _offsetX;
			_superFX->Pos.y = (float)_animKeys[keyID]->ypos + _offsetY;
		}
		else
		{
			_targetObj->SetAlpha(_animKeys[keyID]->alpha);
			_targetObj->Pos.x = (float)_animKeys[keyID]->xpos + _offsetX;
			_targetObj->Pos.y = (float)_animKeys[keyID]->ypos + _offsetY;
			_targetObj->SetRotate(_animKeys[keyID]->rot);
			_targetObj->SetScale(_animKeys[keyID]->scale);
		}
	}
}

void UnitAnimator::ResetAnim()
{
	_currentKey = 0;
	_animTime = 0;

	_targetObj->SetEnable(true);

	_setObjProperties(_currentKey);
}

void UnitAnimator::SetAnimTime(float animTime)
{
	_currentKey = GetAnimKey(animTime)->keyId;
	_targetObj->SetEnable(true);
	_setObjProperties(_currentKey);
}

UNITKEY * UnitAnimator::GetAnimKey(float animTime)
{
	for (int i = 0; i < (int)(_animKeys.size()); i++)
	{
		float tempKeyTime1 = _animKeys[i]->keytime;
		if (i == (int) (_animKeys.size() - 1))
		{
			if (animTime == tempKeyTime1)
			{
				return _animKeys[i];;
			}
		}
		else
		{
			float tempKeyTime2 = _animKeys[i+1]->keytime;
			if (animTime >= tempKeyTime1 && animTime < tempKeyTime2)
			{
				return _animKeys[i];
			}
		}
	}

	return NULL;
}





