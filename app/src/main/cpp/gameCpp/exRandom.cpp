//===========================================================
// ELIXIR
// Game framework and toolset
// 
//
// Elixir random functions
//===========================================================

//#include "elixir.h"
#include "Framework.h"
#include "exRandom.h"

namespace Elixir
{
	static const float rand_scaled = 1.0f / RAND_MAX;

	//=================================================
	// Seeds the random generator
	//=================================================
	void SeedRandom()
	{
		srand((unsigned int) time(NULL));
	}

	void SeedRandom(unsigned int seed)
	{
		srand(seed);
	}

	//=================================================
	// Random functions
	//=================================================
	//Returns a random percentage: 0.0 -> 1.0
	float Rand1f()
	{
		return rand() * rand_scaled;
	}

	//Min, max inclusive
	float Rand(float min, float max)
	{
		return (Rand1f() * (max-min)) + min;
	}
	
	//Min inclusive, max exclusive
	int Rand(int min, int max)
	{
		return int( (Rand1f() * (max-min)) + min );
	}
}