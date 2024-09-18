//===========================================================
// 
// Framework
//===========================================================

#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "Framework.h"
#include "Point.h"
#include "Constants.h"
#include "RenderListEntity.h"
#include "UpdateListEntity.h"
#include "Console.h"
#include "Rect.h"
#include "Sprite.h"
#include "MouseInterface.h"
#include <functional>
#include "Polygon.h"

class CSprite;
class CIOStream;
class Camera;

enum SprStatus {SPR_VISIBLE, SPR_FADEOUT, SPR_FADEIN, SPR_SHAKING, SPR_ANIMATING, SPR_INVISIBLE, SPR_FADEINOUT};
enum TWEaseFunction  {TW_EASE, TW_EASEQUADIN, TW_EASEQUADOUT, TW_EASEBOUNCEOUT, TW_EASEELEASTICOUT, TW_EASEBACKIN, TW_EASEBACKOUT};


class CObject : public CRenderListEntity, UpdateListEntity, public MouseInterface
{

	protected:		
		const Camera	*Cam;		
		bool		Dirty;
		//Tween
		float TwTime, TwTotalTime;
		float InitRot, InitScale;
		int TwEase;
		bool IsTweenAutoComplete;
		bool IsFadeAnim;

		float shakeOffx,shakeOffy,shakeRotOff, shakeScaleOff, alphaOffset, InitAlpha;
		float shakeTime, phase;

		CPolygon* _interactionArea;
		float _interactionCircleRadius;
		CPoint _interactionAreaSize;
		bool _isInteractionEnabled;
		bool _isAddedToInteractiveObjectList;

		std::function<void (CObject*)> _mouseClickCallback;
		std::function<void (CObject*)> _mouseDownCallback;
		std::function<void (CObject*)> _mouseOverCallback;
		std::function<void (CObject*)> _mouseOutCallback;
		std::function<void (CObject*)> _mouseUpCallback;

		CObject * _mouseInteractionCallbackObject;

		std::function<void (CObject*)> _tweenCompleteCallback;



		MouseState _currentMouseState;

	public:	
		CPoint		InitPos;

		bool		Paused;
		bool		IsActive;
		std::string Filename;
		std::string UID;
		std::string Type;
		CPoint		Pos;
		CSprite		*pSprite;
		
		SprStatus   Status;		

		void SetX( float x );
		void SetY( float y );
		void SetRelativeX( float x );
		void SetRelativeY( float y );

		float GetCenterX(float offset = 0.0f);
		float GetCenterY(float offset = 0.0f);

		void Flip(bool f);
		void SetXY( float X, float Y );
		void SetPivot( float pX, float pY );
		void SetRelativeXY( float x, float y );

		void SetMouseInteractionArea(CPolygon* interactionArea);
		void SetMouseInteractionArea(float width, float height);
		void SetMouseInteractionArea(float circleRadius);

		void SetMouseClickCallback(std::function<void (CObject*)> mouseClickCallback, CObject * callbackObject = NULL);
		void SetMouseDownCallback(std::function<void (CObject*)> mouseDownCallback, CObject * callbackObject = NULL);
		void SetMouseOverCallback(std::function<void (CObject*)> mouseOverCallback, CObject * callbackObject = NULL);
		void SetMouseOutCallback(std::function<void (CObject*)> mouseOutCallback, CObject * callbackObject = NULL);
		void SetMouseUpCallback(std::function<void (CObject*)> mouseUpCallback, CObject * callbackObject = NULL);

		void SetMouseClickProperties(CPolygon* clickArea, std::function<void (CObject*)> mouseClickCallback, CObject * callbackObject = NULL);
		void SetMouseDownProperties(CPolygon* clickArea, std::function<void (CObject*)> mouseDownCallback, CObject * callbackObject = NULL);
		void SetMouseOverProperties(CPolygon* clickArea, std::function<void (CObject*)> mouseOverCallback, CObject * callbackObject = NULL);
		void SetMouseOutProperties(CPolygon* clickArea, std::function<void (CObject*)> mouseOutCallback, CObject * callbackObject = NULL);
		void SetMouseUpProperties(CPolygon* clickArea, std::function<void (CObject*)> mouseUpCallback, CObject * callbackObject = NULL);

		void SetMouseClickProperties(float width, float height, std::function<void (CObject*)> mouseClickCallback, CObject * callbackObject = NULL);
		void SetMouseDownProperties(float width, float height, std::function<void (CObject*)> mouseDownCallback, CObject * callbackObject = NULL);
		void SetMouseOverProperties(float width, float height, std::function<void (CObject*)> mouseOverCallback, CObject * callbackObject = NULL);
		void SetMouseOutProperties(float width, float height, std::function<void (CObject*)> mouseOutCallback, CObject * callbackObject = NULL);
		void SetMouseUpProperties(float width, float height, std::function<void (CObject*)> mouseUpCallback, CObject * callbackObject = NULL);

		void SetMouseClickProperties(float circleRadius, std::function<void (CObject*)> mouseClickCallback, CObject * callbackObject = NULL);
		void SetMouseDownProperties(float circleRadius, std::function<void (CObject*)> mouseDownCallback, CObject * callbackObject = NULL);
		void SetMouseOverProperties(float circleRadius, std::function<void (CObject*)> mouseOverCallback, CObject * callbackObject = NULL);
		void SetMouseOutProperties(float circleRadius, std::function<void (CObject*)> mouseOutCallback, CObject * callbackObject = NULL);
		void SetMouseUpProperties(float circleRadius, std::function<void (CObject*)> mouseUpCallback, CObject * callbackObject = NULL);


		virtual void OnMouseOver();
		virtual void OnMouseClick();
		virtual void OnMouseDown();
		virtual void OnMouseUp();
		virtual void OnMouseOut();

		MouseState GetCurrentMouseState();

		bool IsIntersect(CPoint point);
		void SetIsInteractionEnabled(bool isInteractionEnabled);
		bool GetIsInteractionEnabled();

		virtual void HandleMouseInput(CInput * mouseInput);

		float easing(float t,float b,float c,float d);

		/**
		* Shake object in a particular angle with offset
		*
		* \param angle : Angle of shaking. 0-Right, 90-Up, 180-Left, 270-Down
		* \param maxOffset : Shake offset in pixels
		* \param rotation : If rotate should be applied
		* \param duration : Duration of shake in seconds. Lesser the better.
		*/
		void Shake( float angle, float maxOffset, float rotation, float duration );

		/**
		* Animate object with sinewave movements
		*
		* \param xOffset : Horizontal movement offset
		* \param yOffset : Vertical movement offset
		* \param rotoffset : Rotation offset. SetRotate to another value to rotate relative to that axis
		* \param wavelength : Duration of Sine wave. Lower means faster
		* \param phaseShift : Phase of Sine wave. waving at different times
		*/
		void Animate( float xOffset, float yOffset, float rotoffset, float scaleoffset, float wavelength, float phaseShift=0 );
		void Animate( float minAlpha, float maxAlpha, float wavelength, float phaseShift=0, EBlend blendMode = eBlend_AlphaNormal);

		bool IsTweening;
		// Tween Values for alpha, posx, posy, scale, rotation
		float TWStartValues[5], TWEndValues[5];
		//To Avoid multiple Calculations
		bool IsTweenActive[5];
		/*Tween to Object values in format
		float tweenVals[5] = {alpha, posx, posy, scale, rotation};
		tweenTime in seconds, Use -1 for inactive tweens
		Autocomplete false to check tween complete handling*/
		void TweenTo(float valArr[],float tweenTime, int easeFun,bool IsAutoComplete, std::function<void (CObject*)> tweenCompleteCallback = nullptr);

		/*Tween to a Shifted Position
		* \param xOffset : +ve to right
		* \param yOffset : +ve to down
		* \param fadeVal : +ve to Fade In , -ve to fadeout
		* \param tweenTime : Time for tweening
		* \param easeFun : Ease Function TW_ease
		* \param IsAutoComplete : Auto clear tweening
		*/
		void TweenPosTo(float xOffset,float yOffset, int fadeVal, float tweenTime, int easeFun,bool IsAutoComplete, std::function<void (CObject*)> tweenCompleteCallback = nullptr);

		void ClearTween();
		bool IsTweenFinished();
		
		CObject();
		virtual ~CObject();

		int PublicValue;

		float FadeInSpeed,FadeOutSpeed,FadeInOutDuration;
		void FadeIn( float fadeInSpeed = 1.0f );		
		void FadeOut(float fadeOutSpeed = 1.0f );
		void FadeInOut(float fadeInOutDuration = 1.0f );

		void ForceShow();
		void StopAnim();
		virtual void SetCam(const Camera* cam) { Cam = cam; }

		//Basic Object Methods
		virtual void Load(std::string Str);		
		virtual void Load(std::string Str, bool IsInteractiveObj){}
		virtual void Update(float ds);
		virtual void Render(const CPoint* pCam);

		void LoadInit(const std::string& sprPath, float x, float y, int zOrder, bool enable=true);

		//Sprite Methods
		void SetEnable(bool enabled);
		void SetSprite( CSprite * Spr );
		bool LoadSprite(const std::string& file);
		void ForcePlayAnimation(int anim);
		void SetAnimation(int anim);
		void PlayAnimation(int anim);
		int  GetAnimation();
		bool IsAnimationFinished();
		void SetAnimSpeed(float speed);
		void SetFrame( int f );
		
		//Logic Methods
		virtual bool Collide(const CPoint* pos, int Offset = 0);

		//Accessors
		virtual void SetPos(const CPoint* pos);		
		void SetRelativePos( const CPoint* pos );
		virtual void SetMaskRect( CRectangle maskrect);

		virtual CRectangle GetLocalRect(); 
		virtual CRectangle GetWorldRect();
		
		const CPoint*	GetPosPointer();
		float GetRotate();
		float GetAlpha();
		//Rendering Options
		virtual void SetAlpha(float alpha);
		virtual void SetRelativeRotate(float rotate);
		virtual void SetScale(float scale);
		virtual float GetScale();
		virtual void SetRotate(float rotate);
		virtual void SetTint(float r, float g, float b);
		virtual void SetBlendMode(EBlend b);
		virtual void Show(bool f)			{ Visible = f; }
		virtual bool IsDirty()				{ return Dirty; }
		virtual void SetDirty(bool f)		{ Dirty = f; }
		virtual void SetUID(std::string s)	{ UID = s; }
};

#endif