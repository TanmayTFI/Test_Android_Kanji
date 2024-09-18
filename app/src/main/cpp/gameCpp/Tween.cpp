//===========================================================
// Helpers
// Tween Class
// Usage:
// 
//===========================================================

#include "Tween.h"
#include "Util.h"

Tween::Tween(CObject* obj, std::function<void (CObject*)> tweenCompleteCallback)
{
	_tweenObj = obj;
	_tweenCompleteCallback = tweenCompleteCallback;
}

Tween::~Tween()
{
	_tweenObj = NULL;
	_tweenCompleteCallback = nullptr;
}

void Tween::Update()
{
	
}