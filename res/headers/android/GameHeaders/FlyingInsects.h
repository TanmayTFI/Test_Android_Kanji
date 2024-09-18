//===========================================================
// Helpers
// Flying Insects
//===========================================================

#ifndef HELPER_FLYINGINSECTS_DEF
#define HELPER_FLYINGINSECTS_DEF
#include "Object.h"

#define MAX_CR_GOLDEN_FLIES 12

typedef struct CR_GOLDENFLY
{
	CObject *Spr;
	float VelX, VelY;
	float MaxLeft, MaxRight, MaxTop, MaxBottom;
	bool IsAlphaIncr; 	
	int Alpha;
	bool IsWaiting;
	int WaitTicks;
}CR_GOLDENFLY;

class FlyingInsects
{
	private:
		CR_GOLDENFLY GoldenFlies[MAX_CR_GOLDEN_FLIES];
	public:
		//Methods
		void InitFlies(float Left,float Top, float Right, float Bottom);
		void Update();
		
		FlyingInsects();
		~FlyingInsects();
};

#endif