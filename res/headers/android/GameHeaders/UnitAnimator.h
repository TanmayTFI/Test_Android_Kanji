//===========================================================
// 
// Framework
// Unit Animator
// Handles the keys and calculation of single animatable object
// Faisal
//===========================================================

#ifndef BASE_UNITANIMATOR_H
#define BASE_UNITANIMATOR_H

#include "Object.h"
#include "SuperFX.h"

typedef struct UNITKEY
{
	float keytime;
	float alpha;
	int xpos, ypos, easeType;
	float rot, scale;
	int keyId;
}UNITKEY;

enum AnimEaseEnums
{
	eEase_COSINE,
	eEase_LINEAR,
	eEase_QUADIN,
	eEase_QUADOUT,
	eEase_BOUNCEOUT,
	eEase_ELASTICOUT,
	eEase_BACKOUT
	//Add future ease functions here
};

class UnitAnimator
{

	protected:
		CObject *_targetObj;

		SuperFX *_superFX;
		bool	_isASuperFX;

		bool	_paused;
		int		_offsetX, _offsetY;

		int		_currentKey, _numKeys;
		float	_animTime, _totalAnimTime;

		std::vector<UNITKEY *>	_animKeys;

		float _easing(int ease, float t,float b,float c,float d);

		void _setObjProperties(int keyID);

	public:

		UnitAnimator(CObject* obj);
		UnitAnimator(SuperFX* superFX);
		virtual ~UnitAnimator();
		void SetAlphaForObjects(float alpha);
		void FadeOutObject(float speed);
		void setEnableObject(bool f);
		void SetPosOffset(int x, int y);

		void Update(float ms);
		void SetAnimKeys(std::vector<UNITKEY *> keysArray, float totalAnimTime);

		CObject* GetAnimObject()	{ return _isASuperFX? _superFX: _targetObj; };

		bool isSuperFX()			{ return _isASuperFX;};

		void ResetAnim();
		void SetAnimTime(float animTime);
		UNITKEY * GetAnimKey(float animTime);
};

#endif