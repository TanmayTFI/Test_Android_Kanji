//===========================================================
// 
// Framework
//===========================================================

#include "Object.h"
#include "Util.h"
#include "Sprite.h"
#include "IOStream.h"
#include "RenderList.h"
#include "Camera.h"
#include "GameFlow.h"
#include "InteractiveObjectList.h"
#include "SuperLog.h"


CObject::CObject()
	:	Dirty(false),
	Filename(""),
	UID("New ID"),
	Type(OBJECT),
	Pos(0.0f,0.0f,0.0f),
	pSprite(NULL)
{
	_mouseClickCallback = nullptr;
	_mouseDownCallback = nullptr;
	_mouseOverCallback = nullptr;
	_mouseOutCallback = nullptr;
	_mouseUpCallback = nullptr;
	_tweenCompleteCallback = nullptr;

	_interactionCircleRadius = 0.0f;

	_currentMouseState = eMouseState_Default;
	_isInteractionEnabled = true;
	_isAddedToInteractiveObjectList = false;

	InitPos = Pos;



	PublicValue = 0;
	Cam = NULL;

	IsFadeAnim = false;

	InitRot = 0;
	shakeTime = 0;
	phase = 0;

	//Tween Init
	IsTweening = false;
	TwEase = TW_EASE;
	TwTime = 0;
	TwTotalTime = 1.0f;
	for (int i = 0; i < 5; i++)
	{
		IsTweenActive[i] = false;
		TWStartValues[i] = -1;
		TWEndValues[i] = -1;
	}

	FadeOutSpeed = 1.0f;
	FadeInSpeed = 1.0f;
	FadeInOutDuration = 1.0f;

	Status = SPR_VISIBLE;

	Paused = false;
	IsActive = true;
	Visible = true;//CRenderListEntity

	_interactionArea = NULL;
	_interactionAreaSize.x = 0;
	_interactionAreaSize.y = 0;
}

CObject::~CObject()
{
	if( pSprite )
	{
		delete pSprite;
		pSprite = NULL;
	}

	CInteractiveObjectList::GetInstance()->Remove(this);
	_isAddedToInteractiveObjectList = false;
	_mouseClickCallback = nullptr;
	_mouseDownCallback = nullptr;
	_mouseOverCallback = nullptr;
	_mouseOutCallback = nullptr;
	_mouseUpCallback = nullptr;
	_tweenCompleteCallback = nullptr;

	SAFE_DELETE(_interactionArea);
	_interactionArea = NULL;
}

void CObject::Load(std::string Str)
{
}

void CObject::LoadInit(const std::string& sprPath, float x, float y, int zOrder, bool enable)
{
	if( pSprite )
	{
		delete pSprite;
		pSprite = NULL;
	}

	Filename = sprPath;

	//Filename to uppercase!!
	int strLen = (int)Filename.size();  //Sac: converted Implicit to explicit
	for( int i = 0; i < strLen; i++ )
		Filename[i] = toupper(Filename[i]);

	//Though the filename is getting converted to upper, it is not used after converted	
	// pSprite = new CSprite(sprPath);  
	// so chaing the above to use this
	pSprite = new CSprite(Filename); 

	if( pSprite->Failed() )
	{
		delete pSprite;
		pSprite = NULL;
		#ifdef K_WIN32
			WARN("Er0x1401- Failed to load Sprite");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1401- Failed to load Sprite");
		#endif		
		return;
	}

	
	//File loaded
	InitPos.x = Pos.x = x;
	InitPos.y = Pos.y = y;

	ZOrder = zOrder;

	SetEnable(enable);
}

void CObject::Shake( float angle, float maxOffset, float rotation, float duration )
{
	if(Status!=SPR_SHAKING)
	{
		shakeOffx = maxOffset*Elixir::Cos(angle);
		shakeOffy = maxOffset*Elixir::Sin(angle);

		shakeRotOff = rotation;
		shakeTime = duration;

		TwTime = 0;

		InitRot = GetRotate();

		Status = SPR_SHAKING;
	}
}

void CObject::Animate( float xOffset, float yOffset, float rotoffset, float scaleoffset, float wavelength, float phaseShift )
{
	shakeOffx = xOffset;
	shakeOffy = yOffset;

	phase = phaseShift;

	shakeRotOff = rotoffset;
	shakeScaleOff = scaleoffset;
	shakeTime = wavelength;

	InitScale = (pSprite!=NULL)?pSprite->Scale:1.0f;
	InitRot = GetRotate();

	//init values
	float sinval = Elixir::Sin((TwTime/shakeTime)*360.0f + phase);
	float cosval = Elixir::Cos((TwTime/shakeTime)*360.0f + phase);
	Pos.x = InitPos.x + shakeOffx * cosval;
	Pos.y = InitPos.y + shakeOffy * sinval;
	SetRotate(InitRot + shakeRotOff *sinval);
	SetScale(InitScale + shakeScaleOff *sinval);

	Status = SPR_ANIMATING;
}

void CObject::Animate( float minAlpha, float maxAlpha, float wavelength, float phaseShift, EBlend blendMode)
{
	SetBlendMode(blendMode);

	TwTime = 0;
	IsFadeAnim = true;
	alphaOffset = (maxAlpha-minAlpha)*0.5f;
	shakeTime = wavelength;

	phase = phaseShift;

	InitAlpha = minAlpha;

	float sinval = Elixir::Sin((TwTime/shakeTime)*360.0f+ phase);
	SetAlpha(InitAlpha + alphaOffset*(sinval+1.0f));

	Status = SPR_ANIMATING;
}

void CObject::ClearTween()
{
	_tweenCompleteCallback = nullptr;
	IsTweenAutoComplete = false;
	TwEase = 0;
	TwTime = 0;
	TwTotalTime = 0;
	for (int i = 0; i < 5; i++)
	{
		IsTweenActive[i] = false;
	}
	IsTweening = false;
}

void CObject::TweenTo(float valArr[],float tweenTime,int easeFun,bool IsAutoComplete, std::function<void (CObject*)> tweenCompleteCallback)
{
	ClearTween();
	_tweenCompleteCallback = tweenCompleteCallback;
	IsTweenAutoComplete = IsAutoComplete;
	TwEase = easeFun;
	TwTime = 0;
	TwTotalTime = tweenTime;
	for (int i = 0; i < 5; i++)
	{
		IsTweenActive[i] = false;
		if(valArr[i] != -1)
		{
			IsTweenActive[i] = true;
			TWEndValues[i] = valArr[i];
			switch (i)
			{
			case 0: TWStartValues[i] = pSprite->Alpha; break;
			case 1: TWStartValues[i] = Pos.x; break;
			case 2: TWStartValues[i] = Pos.y; break;
			case 3: TWStartValues[i] = pSprite->Scale; break;
			case 4: TWStartValues[i] = GetRotate(); break;
			default:break;
			}
		}
	}
	IsTweening = true;
}

void CObject::TweenPosTo(float xOffset,float yOffset, int fadeVal, float tweenTime, int easeFun,bool IsAutoComplete, std::function<void (CObject*)> tweenCompleteCallback)
{
	ClearTween();
	_tweenCompleteCallback = tweenCompleteCallback;
	IsTweenAutoComplete = IsAutoComplete;
	TwEase = easeFun;
	TwTime = 0;
	TwTotalTime = tweenTime;

	if(fadeVal>0)
	{
		TWStartValues[0] = pSprite->Alpha = 0;		 
		TWEndValues[0] = 1.0f;
		IsTweenActive[0] = true;
	}
	else if(fadeVal<0)
	{
		TWStartValues[0] = pSprite->Alpha = 1.0f;		 
		TWEndValues[0] = 0;
		IsTweenActive[0] = true;
	}

	TWStartValues[1] = Pos.x;
	TWEndValues[1] = Pos.x+xOffset;
	IsTweenActive[1] = true;

	TWStartValues[2] = Pos.y;
	TWEndValues[2] = Pos.y+yOffset;
	IsTweenActive[2] = true;

	IsTweening = true;
}

float CObject::easing(float t,float b,float c,float d)
{
	float val=c;
	float s=1.70158f;
	switch (TwEase)
	{
	case TW_EASE:	val = (c * t / d + b);		break;
	case TW_EASEQUADIN: t /= d;	val = (c*t*t + b); break;
	case TW_EASEQUADOUT: t /= d;val = (-c * t*(t-2.0f) + b); break;
	case TW_EASEBOUNCEOUT: 
		//Sac: this could be problematic, as we are assigning t and also using it for multiplication in same line
		// we better assign it first and then use it for multiplication
		//modifying as below...

		if ((t/=d) < (1.0f/2.75f)) {
			val = c*(7.5625f*t*t) + b;
		} else if (t < (2.0f/2.75f)) {
			t-=(1.5f/2.75f);
			val = c*(7.5625f*(t)*t + .75f) + b;
		} else if (t < (2.5f/2.75f)) {
			t-=(2.25f/2.75f);
			val = c*(7.5625f*(t)*t + .9375f) + b;
		} else {
			t-=(2.625f/2.75f);
			val = c*(7.5625f*(t)*t + .984375f) + b;
		}
		break;
	case TW_EASEELEASTICOUT:
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
	case TW_EASEBACKIN:
		t/=d;
		val = c*(t)*t*((s+1.0f)*t - s) + b;
		break;
	case TW_EASEBACKOUT:
		t=t/d-1.0f;
		val = c*((t)*t*((s+1.0f)*t + s) + 1.0f) + b;
		break;

	default: return 1.0f;
		break;
	}
	return val;
}

//Note: Make IsTweening false manually to check Tweening
bool CObject::IsTweenFinished()
{
	if(IsTweening)
	{
		if(TwTime>=TwTotalTime)
			return true;
	}
	return false;
}

void CObject::Update(float ds)
{
	if( Paused || !Visible || !IsActive )
		return;

	if( pSprite )
	{
		if( Status != SPR_INVISIBLE )
		{
			pSprite->Update(ds);

			if( IsTweening )
			{
				//Update Tweening
				TwTime += ds;
				if(TwTime<TwTotalTime)
				{
					float TWCurrentValues[5];
					for (int i = 0; i < 5; i++)
					{
						if(IsTweenActive[i])	//only the active ones
						{
							TWCurrentValues[i] = TWStartValues[i] + (TWEndValues[i]-TWStartValues[i])*easing(TwTime,0,1,TwTotalTime);
							switch (i)
							{
							case 0: SetAlpha(TWCurrentValues[i]); break;
							case 1: Pos.x = TWCurrentValues[i]; break;
							case 2: Pos.y = TWCurrentValues[i]; break;
							case 3: SetScale(TWCurrentValues[i]);	break;
							case 4: SetRotate(TWCurrentValues[i]); break;
							default:break;
							} 
						}
					}
				}
				else
				{
					
					//Set End values
					for (int i = 0; i < 5; i++)
					{
						if(IsTweenActive[i])
						{
							switch (i)
							{
							case 0: SetAlpha(TWEndValues[i]); break;
							case 1: Pos.x = TWEndValues[i]; break;
							case 2: Pos.y = TWEndValues[i]; break;
							case 3: SetScale(TWEndValues[i]);	break;
							case 4: SetRotate(TWEndValues[i]); break;
							default:break;
							} 
						}
					}

					if(IsTweenAutoComplete)
					{
						IsTweening = false;
						IsTweenAutoComplete = false;
						TwTime = 0;
						if (_tweenCompleteCallback)
						{
							_tweenCompleteCallback(this);
						}
						
					}
				}
			}
		}

		switch( Status )		
		{
		default:break;
		case SPR_ANIMATING:
			{
				TwTime += ds;
				if(IsFadeAnim)
				{
					float sinval = Elixir::Sin((TwTime/shakeTime)*360.0f+ phase);
					SetAlpha(InitAlpha + alphaOffset*(sinval+1.0f));//sinval + 1 is double

				}
				else
				{
					float sinval = Elixir::Sin((TwTime/shakeTime)*360.0f + phase);
					float cosval = Elixir::Cos((TwTime/shakeTime)*360.0f + phase);
					Pos.x = InitPos.x + shakeOffx * cosval;
					Pos.y = InitPos.y + shakeOffy * sinval;
					SetRotate(InitRot + shakeRotOff *sinval);
					SetScale(InitScale + shakeScaleOff *sinval);
				}
			}
			break;
		case SPR_SHAKING:
			{
				TwTime += ds;
				float sinval = Elixir::Sin((TwTime/shakeTime)*360.0f);
				Pos.x = InitPos.x + shakeOffx * sinval;
				Pos.y = InitPos.y + shakeOffy * sinval;
				SetRotate(InitRot + shakeRotOff *sinval);
				if( TwTime >= shakeTime )
				{
					Status = SPR_VISIBLE;
					Pos.x = InitPos.x;
					Pos.y = InitPos.y;
					SetRotate(InitRot);
				}
			}
			break;	
		case SPR_FADEIN:
			{
				float Alpha = pSprite->Alpha + 0.8f * FadeInSpeed * ds;
				if( Alpha >= 1.0f )
				{
					Alpha = 1.0f;
					Status = SPR_VISIBLE;
				}
				SetAlpha( Alpha );
			}
			break;

		case SPR_FADEOUT:	
			{
				float Alpha = pSprite->Alpha - 0.8f * FadeOutSpeed * ds;
				if( Alpha <= 0.15f )
				{
					Visible = false;
					Alpha = 0.0f;
					Status = SPR_INVISIBLE;
					pSprite->Hide();
				}
				SetAlpha( Alpha );
			}
			break;

		case SPR_FADEINOUT:
			{

				TwTime += ds;
				float sinval = Elixir::Sin((TwTime/FadeInOutDuration)*180.0f);
				SetAlpha(sinval);

				if( TwTime >= FadeInOutDuration )
				{
					Visible = false;
					SetAlpha( 0.0f);
					Status = SPR_INVISIBLE;
					pSprite->Hide();
				}
				
			}
			break;
		}

	}
}

void CObject::FadeOut(float fadeOutSpeed)
{
	if(Status == SPR_FADEOUT)
		return;
	Show(true);
	FadeOutSpeed = fadeOutSpeed; 
	if( pSprite )
	{
		if(Status != SPR_FADEOUT)
			pSprite->SetAlpha(1.0f);
		pSprite->Show();
	}
	Status = SPR_FADEOUT;
}
		
void CObject::FadeInOut(float fadeInOutDuration)
{
	if (Status == SPR_FADEINOUT)
		return;

	Show(true);
	FadeInOutDuration = fadeInOutDuration; 
	TwTime = 0.0f;

	if( pSprite )
	{
		pSprite->SetAlpha(0.0f);
		pSprite->Show();
	}
	Status = SPR_FADEINOUT;

}

void CObject::FadeIn( float fadeInSpeed)
{
	if(Status == SPR_FADEIN)
		return;
	Show(true);
	FadeInSpeed = fadeInSpeed;
	if( pSprite )
	{
		if(Status != SPR_FADEOUT)
			pSprite->SetAlpha(0.0f);
		pSprite->Show();
	}
	Status = SPR_FADEIN;
}

void CObject::Render( const CPoint* pCam  )
{
	if( !GFInstance->IsRenderingToRT )
		if( !Visible || !IsActive)
			return;

	if( pSprite )
	{
		CPoint originPos;

		if( Cam )
		{
			originPos = Cam->ViewOrigin;
		}

		CPoint screenPos = Pos - originPos;
		screenPos.x += pCam->x;
		screenPos.y += pCam->y;
		pSprite->Render(&screenPos, pCam->x);
	}
}

void CObject::ForceShow()
{
	Status = SPR_VISIBLE; 
	if( pSprite )
	{
		pSprite->SetAlpha(1.0f);
		pSprite->Show();
		Visible = true;
	}		
}

void CObject::StopAnim()
{
	Status = SPR_VISIBLE;
}

void CObject::SetAlpha(float alpha)
{
	if( pSprite )
	{
		alpha = alpha>1.0f?1.0f:alpha;
		pSprite->SetAlpha(alpha);
	}
}

void CObject::SetMaskRect( CRectangle maskrect)
{
	if( pSprite )
	{
		pSprite->SetMaskParams(maskrect.Left(),maskrect.Top(),maskrect.Right(),maskrect.Bottom());
	}
}

float CObject::GetScale()
{
	if( pSprite )
	{
		return pSprite->Scale;
	}
	return 1.0f;
}

void CObject::SetScale(float scale)
{
	if( pSprite )
	{
		pSprite->SetScale(scale);
	}
}

void CObject::SetTint(float r, float g, float b)
{
	if( pSprite )
	{
		pSprite->SetTint(r,g,b);
	}
}

void CObject::SetBlendMode(EBlend b)
{
	if( pSprite )
	{
		pSprite->SetBlendMode(b);
	}
}

void CObject::SetRotate(float rotate)
{
	if( pSprite )
	{
		pSprite->SetRotate(rotate);
	}
}

float CObject::GetRotate()
{
	if( pSprite )
	{
		return pSprite->Rotate;
	}

	return 0.0f;
}

float CObject::GetAlpha()
{
	if( pSprite )
	{
		return pSprite->Alpha;
	}

	return 0.0f;
}


void CObject::SetRelativeRotate(float rotate)
{
	if( pSprite )
	{
		pSprite->SetRotate(pSprite->Rotate+rotate);
	}
}

void CObject::SetEnable( bool enabled )
{
	IsActive = enabled;
}

void CObject::SetSprite( CSprite * Spr )
{
	pSprite = Spr;
}

//Sprite Methods
bool CObject::LoadSprite( const std::string& file )
{
	if( pSprite )
	{
		delete pSprite;
		pSprite = NULL;
	}

	Filename = file;
	//Filename to uppercase!!
	int strLen =(int) Filename.size(); //Sac: converted Implicit to explicit
	for( int i = 0; i < strLen; i++ )
		Filename[i] = toupper(Filename[i]);
	
	//Though the filename is getting converted to upper, it is not used after converted	
	// pSprite = new CSprite(file);
	// so chaing the above to use this

	pSprite = new CSprite(Filename);

	if( pSprite->Failed() )
	{
		delete pSprite;
		pSprite = NULL;
			#ifdef K_WIN32
			WARN("Er0x1402- Failed to load Sprite");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1402- Failed to load Sprite");
		#endif
		return false;
	}


	return true;
}

void CObject::ForcePlayAnimation(int anim)
{
	if( pSprite )
	{
		pSprite->ForcePlayAnimation(anim);
	}
}

void CObject::SetAnimation(int anim)
{
	if( pSprite )
	{
		pSprite->SetAnimation(anim);
	}
}

void CObject::PlayAnimation(int anim)
{
	if( pSprite )
	{
		pSprite->PlayAnimation(anim);
	}
}

bool CObject::IsAnimationFinished()
{
	if( pSprite )
	{
		return pSprite->IsAnimationFinished();
	}

	return true;
}

int CObject::GetAnimation()
{
	if( pSprite )
	{
		return pSprite->CurrentAnim;
	}

	return 0;
}

void CObject::SetFrame( int f )
{
	if( pSprite )
		pSprite->SetFrame( f );
}

void CObject::SetAnimSpeed(float speed)
{
	if( pSprite )
		pSprite->SetAnimSpeed(speed);
}

void CObject::Flip(bool f)
{
	if(pSprite)
	{
		pSprite->Flip(f);
	}
}

//Logic Methods
bool CObject::Collide(const CPoint* collidepos, int Offset)
{
	if( pSprite && Visible )
	{
		return pSprite->Collide(&Pos, collidepos, -1, Offset);
	}

	return false;
}


CRectangle CObject::GetLocalRect()
{
	CRectangle lrect;

	if(pSprite)
		lrect = pSprite->LocalRect();

	return lrect;
}

CRectangle CObject::GetWorldRect()
{
	CRectangle wrect;

	if(pSprite)	
		wrect = pSprite->LocalRect();
	wrect.topleft += Pos; //Offset to world space!!

	return wrect;
}

void CObject::SetPivot( float pX, float pY )
{
	if(pSprite)
	{
		pSprite->SetPivot(pX,pY);
	}
}

float CObject::GetCenterX(float offset )
{
	return(GetWorldRect().GetCenterX(offset));
}

float CObject::GetCenterY(float offset)
{
	return(GetWorldRect().GetCenterY(offset));
}

//Accessors
void CObject::SetXY( float x, float y )
{
	Pos.x = x;
	Pos.y = y;	
}

void CObject::SetRelativeX( float x )
{
	Pos.x += x;
}
void CObject::SetRelativeY( float y )
{
	Pos.y += y;	
}

void CObject::SetX( float x )
{
	Pos.x = x;
}
void CObject::SetY( float y )
{
	Pos.y = y;	
}

void CObject::SetPos( const CPoint* pos )
{	
	Pos = (*pos);	
	InitPos = Pos;
}

void CObject::SetRelativeXY( float x, float y )
{
	Pos.x += x;
	Pos.y += y;
}

void CObject::SetMouseInteractionArea(CPolygon* interactionArea)
{
	_interactionArea = interactionArea;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseInteractionArea(float width, float height)
{
	_interactionAreaSize.x = width;
	_interactionAreaSize.y = height;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseInteractionArea(float circleRadius)
{
	_interactionCircleRadius = circleRadius;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseClickCallback(std::function<void (CObject*)> mouseClickCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_mouseClickCallback = mouseClickCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);

}

void CObject::SetMouseClickProperties(CPolygon* interactionArea, std::function<void (CObject*)> mouseClickCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionArea = interactionArea;
	_mouseClickCallback = mouseClickCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseClickProperties(float width, float height, std::function<void (CObject*)> mouseClickCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionAreaSize.x = width;
	_interactionAreaSize.y = height;
	_mouseClickCallback = mouseClickCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseClickProperties(float circleRadius, std::function<void (CObject*)> mouseClickCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionCircleRadius = circleRadius;
	_mouseClickCallback = mouseClickCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}




void CObject::SetMouseDownCallback(std::function<void (CObject*)> mouseDownCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_mouseDownCallback = mouseDownCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseDownProperties(CPolygon* interactionArea, std::function<void (CObject*)> mouseDownCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionArea = interactionArea;
	_mouseDownCallback = mouseDownCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseDownProperties(float width, float height, std::function<void (CObject*)> mouseDownCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionAreaSize.x = width;
	_interactionAreaSize.y = height;
	_mouseDownCallback = mouseDownCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseDownProperties(float circleRadius, std::function<void (CObject*)> mouseDownCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionCircleRadius = circleRadius;
	_mouseDownCallback = mouseDownCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}



void CObject::SetMouseOverCallback(std::function<void (CObject*)> mouseOverCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_mouseOverCallback = mouseOverCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseOverProperties(CPolygon* interactionArea, std::function<void (CObject*)> mouseOverCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionArea = interactionArea;
	_mouseOverCallback = mouseOverCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseOverProperties(float width, float height, std::function<void (CObject*)> mouseOverCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionAreaSize.x = width;
	_interactionAreaSize.y = height;
	_mouseOverCallback = mouseOverCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseOverProperties(float circleRadius, std::function<void (CObject*)> mouseOverCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionCircleRadius = circleRadius;
	_mouseOverCallback = mouseOverCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}



void CObject::SetMouseOutCallback(std::function<void (CObject*)> mouseOutCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_mouseOutCallback = mouseOutCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseOutProperties(CPolygon* interactionArea, std::function<void (CObject*)> mouseOutCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionArea = interactionArea;
	_mouseOutCallback = mouseOutCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseOutProperties(float width, float height, std::function<void (CObject*)> mouseOutCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionAreaSize.x = width;
	_interactionAreaSize.y = height;
	_mouseOutCallback = mouseOutCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseOutProperties(float circleRadius, std::function<void (CObject*)> mouseOutCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionCircleRadius = circleRadius;
	_mouseOutCallback = mouseOutCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}


void CObject::SetMouseUpCallback(std::function<void (CObject*)> mouseUpCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_mouseUpCallback = mouseUpCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseUpProperties(CPolygon* interactionArea, std::function<void (CObject*)> mouseUpCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionArea = interactionArea;
	_mouseUpCallback = mouseUpCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseUpProperties(float width, float height, std::function<void (CObject*)> mouseUpCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionAreaSize.x = width;
	_interactionAreaSize.y = height;
	_mouseUpCallback = mouseUpCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}

void CObject::SetMouseUpProperties(float circleRadius, std::function<void (CObject*)> mouseUpCallback, CObject * callbackObject)
{
	_mouseInteractionCallbackObject = callbackObject;
	_interactionCircleRadius = circleRadius;
	_mouseUpCallback = mouseUpCallback;
	CInteractiveObjectList::GetInstance()->Add(this);
	_isAddedToInteractiveObjectList = true;
	SetIsInteractionEnabled(true);
}




void CObject::OnMouseOver()
{
	_currentMouseState = eMouseState_Over;
	if (_mouseOverCallback != nullptr)
	{
		CObject * callbackValue = NULL;
		if (_mouseInteractionCallbackObject)
		{
			callbackValue = _mouseInteractionCallbackObject;
		}
		else
		{
			callbackValue = this;
		}
		_mouseOverCallback(callbackValue);
	}
}

void CObject::OnMouseClick()
{
	_currentMouseState = eMouseState_Click;
	if (_mouseClickCallback != nullptr)
	{
		CObject * callbackValue = NULL;
		if (_mouseInteractionCallbackObject)
		{
			callbackValue = _mouseInteractionCallbackObject;
		}
		else
		{
			callbackValue = this;
		}
		_mouseClickCallback(callbackValue);
	}
}
void CObject::OnMouseDown()
{
	_currentMouseState = eMouseState_Down;
	if (_mouseDownCallback != nullptr)
	{
		CObject * callbackValue = NULL;
		if (_mouseInteractionCallbackObject)
		{
			callbackValue = _mouseInteractionCallbackObject;
		}
		else
		{
			callbackValue = this;
		}
		_mouseDownCallback(callbackValue);
	}
}

void CObject::OnMouseUp()
{
	if (_mouseUpCallback != nullptr)
	{
		CObject * callbackValue = NULL;
		if (_mouseInteractionCallbackObject)
		{
			callbackValue = _mouseInteractionCallbackObject;
		}
		else
		{
			callbackValue = this;
		}
		_mouseUpCallback(callbackValue);
	}

	_currentMouseState = eMouseState_Over;
}

void CObject::OnMouseOut()
{
	_currentMouseState = eMouseState_Out;
	if (_mouseOutCallback != nullptr)
	{
		CObject * callbackValue = NULL;
		if (_mouseInteractionCallbackObject)
		{
			callbackValue = _mouseInteractionCallbackObject;
		}
		else
		{
			callbackValue = this;
		}
		_mouseOutCallback(callbackValue);
	}
}

MouseState CObject::GetCurrentMouseState()
{
	return _currentMouseState;
}


bool CObject::IsIntersect(CPoint point)
{
	if (_interactionCircleRadius != 0 )
	{
		float distance = Elixir::Distance(&Pos, &point);
		if (distance < _interactionCircleRadius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (_interactionArea)
	{
		return _interactionArea->Intersection(&point);
	}
	else if(_interactionAreaSize.x > 0 || _interactionAreaSize.y > 0)
	{
		CRectangle areaRect;
		areaRect.topleft.x = Pos.x - (_interactionAreaSize.x / 2.0f);
		areaRect.topleft.y = Pos.y - (_interactionAreaSize.y / 2.0f);
		areaRect.w = _interactionAreaSize.x;
		areaRect.h = _interactionAreaSize.y;
		return(areaRect.Intersection(&point));
	}
	else
	{
		return GetWorldRect().Intersection(&point);
	}
	return false;
}

void CObject::HandleMouseInput(CInput * mouseInput)
{
	if (!_isInteractionEnabled)
	{
		return;
	}


	if(IsIntersect(mouseInput->Mpos()))
	{
		if (_currentMouseState != eMouseState_Over && _currentMouseState != eMouseState_Down)
		{
			OnMouseOver();
		}

		if(mouseInput->LBjustdown() && _currentMouseState != eMouseState_Down)
		{
			OnMouseDown();
		}

		if (mouseInput->LBclicked())
		{
			OnMouseClick();
		}

		if(!mouseInput->LBdown() && _currentMouseState == eMouseState_Down)
		{
			OnMouseUp();
		}
	}
	else
	{
		if (_currentMouseState != eMouseState_Out)
		{
			OnMouseOut();
		}
	}
}

void CObject::SetIsInteractionEnabled(bool isInteractionEnabled)
{
	_isInteractionEnabled = isInteractionEnabled;
	if (isInteractionEnabled && !_isAddedToInteractiveObjectList)
	{
		CInteractiveObjectList::GetInstance()->Add(this);
		_isAddedToInteractiveObjectList = true;
	}
}

bool CObject::GetIsInteractionEnabled()
{
	return _isInteractionEnabled;
}

void CObject::SetRelativePos( const CPoint* pos )
{
	Pos.x += pos->x;
	Pos.y += pos->y;
	Pos.z += pos->z;
}

const CPoint* CObject::GetPosPointer()
{
	return &Pos;
}


