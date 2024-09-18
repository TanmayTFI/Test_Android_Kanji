//===========================================================
// Framework
// SParticle
//===========================================================

#include "SParticle.h"

SParticle::SParticle()
{
	currentTime = 0.0f;
	totalTime = 1.0f;
	Active = false;

	X = Y = 0.0f;
//	VelX = VelX = 0.0f;  Sac: why two times ?? may be it is VelY
	VelX = VelY =  0.0f;

	Alpha = 1.0f;
	dAlpha = 1;

	Rot = 0.0f;
	dRot = 1;

	R = G = B = 1.0f;
	dR = dG = dB = 0.0f;

	Scale = 1.0f;
	dScale = 1;

	Variant = 1;	
}

SParticle::~SParticle()
{
}

void SParticle::Activate()
{
	currentTime = 0;
	Active = true;
}

void SParticle::Deactivate()
{
	Active = false;
}

