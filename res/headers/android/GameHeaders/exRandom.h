//===========================================================
// ELIXIR
// Game framework and toolset
// 
//
// Elixir random functions
//===========================================================

#ifndef ELIXIR_RANDOM_DEF
#define ELIXIR_RANDOM_DEF

namespace Elixir
{
	//=================================================
	// Seeds the random generator
	//=================================================
	void SeedRandom();
	void SeedRandom(unsigned int seed);

	//=================================================
	// Random functions
	//=================================================
	float   Rand1f();
	float	Rand(float min, float max);
	int		Rand(int min, int max);
}

#endif