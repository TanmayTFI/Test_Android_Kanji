//===========================================================
// Framework
// Particle
//===========================================================

#include "Particle.h"
#include "TTAlgorithms.h"
#include "Util.h"

Particle::Particle()
{
	Life = 0.0;
	DistXY = 0.0f;
	Active = false;

	X = Y = 0.0f;
	DX = DY = 0.0f;

	A = 1.0f;
	AS = 1;

	Rot = 0.0f;
	RS = 1;

	R = G = B = 1.0f;

	S = 1.0f;
	SS = 1;

	Variant = 1;	
}

Particle::~Particle()
{
}

void Particle::Activate()
{
	Life = 0.0f;
	DistXY = 0.0f;
	Active = true;
	AS = 1;
	RS = 1;
	SS = 1;
}

void Particle::Deactivate()
{
	Active = false;
}

