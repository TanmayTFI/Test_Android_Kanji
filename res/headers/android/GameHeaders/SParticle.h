//===========================================================
// Framework
// SParticle
// Used for Super FX particle System
//===========================================================

#ifndef FRMWRK_SUPERPARTICLE_DEF
#define FRMWRK_SUPERPARTICLE_DEF
#include "KPTK.h"

class SParticle
{
	private:
	public:
		//Members
		float currentTime;
		float totalTime;

		bool Active;
		float X,Y;
		float VelX, VelY;

		float Alpha;
		float dAlpha;

		float Rot;
		float dRot;

		float R,G,B;
		float dR,dG,dB;

		float Scale;
		float dScale;

		int Variant;

		//Methods
		SParticle();
		~SParticle();

		void Activate();
		void Deactivate();
};

#endif