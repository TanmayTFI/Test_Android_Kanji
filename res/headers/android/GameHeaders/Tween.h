//===========================================================
// Helpers
// Tween Class
//===========================================================

#ifndef HELPER_TWEEN_DEF
#define HELPER_TWEEN_DEF

#include "Object.h"

enum Transition
{
    LINEAR,
    QUAD,
    EXPO,
    ELASTIC,
    CUBIC,
    CIRCLE,
    BOUNCE,
    BACK
};

enum Equation
{
    EASE_IN, EASE_OUT, EASE_IN_OUT
};

class Tween
{
	protected:
		CObject *_tweenObj;
		std::function<void (CObject*)> _tweenCompleteCallback;

		Transition	_transition;
		Equation	_equation;

		float _time, _duration;

		float _easing(float t,float b,float c,float d);

	public:
		/*!
		 * Creates a new tween.
		 *
		 * @param transition
		 * @param equation
		 * @param variable
		 * @param initialValue
		 * @param endValue
		 */
		Tween(CObject* obj, std::function<void (CObject*)> tweenCompleteCallback=nullptr);
		~Tween();

		//Methods
		void Update();
};

#endif