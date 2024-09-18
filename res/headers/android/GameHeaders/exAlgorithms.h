//===========================================================
// ELIXIR
// Game framework and toolset
// 
//
// Elixir algorithms
//===========================================================

#ifndef ELIXIR_ALGORITHMS_DEF
#define ELIXIR_ALGORITHMS_DEF

#include <cmath> 

namespace Elixir
{
	//=================================================
	// Forces a value outside of the specified range
	// into the closer end value.
	//=================================================
	template<class T>
	T Clamp(T min, T val, T max)
	{
		if( val < min )
			return min;

		if( val > max )
			return max;

		return val;
	}

    
	//=================================================
	// Returns if the value is within the 
	// specified range, inclusive of end points.
	//=================================================
	template<class T>
	bool RangeI(T min, T val, T max)
	{
		if( val < min || val > max )
			return false;
        
		return true;
	}

	//=================================================
	// Wraps a value outside of the specified range.
	//=================================================
	template<class T>
	T Wrap(T min, T val, T max)
	{
		T out = val;

		if( val > max )
		{
			out = min;
			//+ abs((int)(val - max));
			if( val > max )
				out += (val-max);
			else 
				out += (max-val);

		}
		else if ( val < min )
		{
			out = max;
			//- abs((int)(min - val));
			if( min > val )
				out -= (min-val);
			else
				out -= (val-min);
		}
		
		if( RangeI(min,out,max)  )
			return out;
		
		return Wrap(min, out, max);
	}

	//=================================================
	// Returns if the value is within the 
	// specified range, exclusive of end points.
	//=================================================
	template<class T>
	bool RangeE(T min, T val, T max)
	{
		if( val <= min || val >= max )
			return false;

		return true;
	}
}

#endif