//===========================================================
// Framework
// Particle
//===========================================================

#ifndef FRMWRK_PARTICLE_DEF
#define FRMWRK_PARTICLE_DEF
#include "KPTK.h"

enum ParticleEffectMode
{
	Particle_None,
	Particle_LinearBound,
	Particle_LinearWrap,
	Particle_Wave,
};

enum ParticleEndMode
{
	Particle_Perpetual,
	Particle_Alpha,
	Particle_Rot,
	Particle_Color,
	Particle_Time,
	Particle_Distance,
	Particle_Scale
};

class Particle
{
	private:
		//Private Members
		float Life;
		

	public:
		//Members
		bool Active;
		float X,Y;
		float DX, DY;
		float DistXY;

		float A;
		int AS;

		float Rot;
		int RS;

		float R,G,B;

		float S;
		int SS;

		int Variant;

		//Methods
		Particle();
		~Particle();

		void Activate();
		void Deactivate();
};


class SprayParticle : public KObjectListable {
public:
   /* Current center coordinates */
   float x, y;

   /* Velocity over x, y */
   float vx, vy;

   /* Color */
   float fColor[4];

   /* Scale */
   float fScale;

   /* Current age */
   float fAge;

   /* Age increase at each tick */
   float fAgeIncr;
};


#endif