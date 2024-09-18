//===========================================================
// Framework
// Particle Effect
//===========================================================

#ifndef FRMWRK_PARTICLEEFFECT_DEF
#define FRMWRK_PARTICLEEFFECT_DEF

#include "Object.h"
#include "Particle.h"
#include "Constants.h"

enum EffectEmissionSource
{
	Effect_Point,
	Effect_Circle,
	Effect_Horz,
	Effect_Vert
};

class ParticleEffect : public CObject
{
	public:
		
		KBatch *lpBatch;
		std::vector<Particle*>	Particles;
		std::string				EffectFile;
		
		
		
		//Texture data
		float Tx, Ty, Tw, Th;
		bool HiDef;
		int Variants;
		

		int OffsetX, OffsetY, Randomizer, nSteps;
		float Duration, TimeLapsed, MovementFactorX, MovementFactorY;
		

		//Emission data
		EffectEmissionSource SourceType;
		float EmitSourceValue; //radius or line length
		float EmitTimer; //how often emissions occur
		float EmitPartsPer; //num of particles per emit
		float MinEmitAngle;
		float MaxEmitAngle;
		int	  UniformEmission;

		//Particle data
		//Velocity/Accel
		float DX;
		float DY;
		float DDX;
		float DDY;
		//Alpha
		float IA;
		
		ParticleEffectMode EA;
		//Rotation
		float IR;
		float DR;
		ParticleEffectMode ER;
		//Tint
		float R,G,B;
		float TR,TG,TB;
		float DRc,DGc,DBc;
		//Scale
		float IS;
		float DS;
		ParticleEffectMode ES;
		//End
		ParticleEndMode EndMode;
		float EndValue;

		void CalcDeltaTint(float dc);
		Particle* Next();
		
		
		bool IsRandomEmission, IsSparkleFX;

		void SetStartPos(Particle* p, int count);
		void SetVelocity(Particle* p, int count);
		void SetVariant(Particle* p);

		void UpdateParticle(Particle* p, float ds);

	public:
		float ScaleFactor;
		EBlend BlendMode;
		CPoint InitPos;
		KGraphic*	Texture;				
		float	Timer;
		float   DA;
		bool Active;
		bool IsEmitting;
		bool ParticlesActive;
		int PartPool;
		int Version;
		int VarNUE;//Non unifrom emission

		ParticleEffect();
		ParticleEffect(const std::string& effectFile);
		virtual ~ParticleEffect();

		//Inherited methods
		virtual void Update(float ds);
		virtual void Render(const CPoint* cam);
		void FadeOut(float fadeOutSpeed);
		float FadeOutTimer;
		float FadeOutSpeed;
		bool IsFadingOut;

		//Particle effect methods
		void LoadEffectFile(const std::string& effectFile, int varNUE = 0);
		void LoadEffectIni(const std::string& effectFile, int varNUE = 0);
		void LoadSparkleEffectIni();
		void SetPos( const CPoint* pos );
		void ResetPos( const CPoint* pos );
		void ResetPosToGBHP( bool IsInvBoxOpen );
		void InitRandomEmission();
		bool IsParticlesActive();
		void StartVFX4( CPoint StartPos );
		virtual void Start();
		virtual void Stop(); // Stops everything, including all particles
		virtual void Deactivate();
		virtual void Emit();
		virtual void StopEmit(); // Only stop emitting, but not all particles
};

#endif