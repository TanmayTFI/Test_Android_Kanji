//====================================
// Framework
// Algorithms
//====================================

#ifndef FRMWRK_ALGORITHMS_DEF
#define FRMWRK_ALGORITHMS_DEF

#include "Framework.h"

namespace TT
{
	template<class T>
	T Clamp(T min, T val, T max)
	{
		if( val < min )
			return min;

		if( val > max )
			return max;

		return val;
	}

	//Positive wrap
	template<class T>
	T PosWrap(T val, T max)
	{
		if( val > max )
			return val - max;
		else if ( val < 0.0f )
			return max + val;

		return val;
	}

	template<class T>
	bool InRange(T min, T val, T max)
	{
		if( val < min || val > max )
			return false;

		return true;
	}
}

#endif