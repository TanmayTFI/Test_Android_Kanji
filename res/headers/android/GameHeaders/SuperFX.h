//===========================================================
// Framework
// Super Particle FX
//===========================================================

#ifndef FRMWRK_SUPERFX_DEF
#define FRMWRK_SUPERFX_DEF

#include "Object.h"
#include "SParticle.h"
#include "Constants.h"

enum EffectSourceV2
{
	EffectV2_RECT,	
	EffectV2_CIRCLE,
	EffectV2_RADIAL,
	EffectV2_RADIALSQR,
	EffectV2_RADIALOUT,
	EffectV2_RADIALIN
};

enum ParticleEffectModeV2
{
	Particlev2_Linear,
	Particlev2_SineWave,	
	Particlev2_Wave
};

class SuperFX : public CObject
{
	protected:
		
		KBatch *lpBatch;
		std::vector<SParticle*>	Particles;
		std::string				EffectFile;
		float delayTimer;

		SParticle* Next();

		void SetStartPos(SParticle* p, int count);
		void SetVelocity(SParticle* p, int count);
		void SetVariant(SParticle* p);

		void UpdateParticle(SParticle* p, float ds);

	public:
		std::string texName; //Texture name for editor purposes

		//Full texture data
		float FullTexw;
		//Texture data
		float Tx, Ty, Tw, Th, THalfw, THalfh;
		bool HiDef;
		int Variants;
		int VariantCols;

		float Duration, TimeLapsed, Drag;

		EBlend BlendMode;
		CPoint InitPos;
		KGraphic*	Texture;				
		float	Timer;
		
		bool Active;
		bool IsEmitting;
		bool ParticlesActive;
		int PartPool;
		int Version;

		//Emission data
		EffectSourceV2 SourceType;
		float EmitSrcWidth; //radius or line length
		float EmitSrcHeight;
		float EmitSrcAngle;
		float EmitTimer; //how often emissions occur
		float EmitPartsPer; //num of particles per emit
		float EmitAngle;
		float EmitAngleVar;
		int	  UniformEmission;

		//SParticle data
		//LifeSpan
		float LifeSpan;
		float LifeSpanVar;

		//Velocity/Accel
		float Speed;
		float SpeedVar;
		
		float AccelX;
		float AccelY;
		
		//Alpha
		float StartAlpha;
		float StartAlphaVar;
		float EndAlpha;
		float EndAlphaVar;
		ParticleEffectModeV2 AlphaMode;

		//Rotation
		float StartRot;
		float StartRotVar;
		float EndRot;
		float EndRotVar;
		ParticleEffectModeV2 RotMode;

		//Tint
		float R,G,B;
		float RVar,GVar,BVar;
		float TR,TG,TB;
		float TRVar,TGVar,TBVar;
		
		//Scale
		float StartSize;
		float StartSizeVar;
		float EndSize;
		float EndSizeVar;
		ParticleEffectModeV2 ScaleMode;

#ifdef _PARTICLEEDITORENABLED
		bool isPoolChanged;
		int newPoolSize;
		void SetPoolSize(int newSize);
#endif // _PARTICLEEDITORENABLED

		SuperFX();
		SuperFX(const std::string& effectFile, const CPoint* pos, int zOrder);
		virtual ~SuperFX();

		//Inherited methods
		virtual void Update(float ds);
		virtual void Render(const CPoint* cam);

		void LoadEffectIni(const std::string& effectFile);

		void SetPos( const CPoint* pos );
		void ResetPos( const CPoint* pos );
		void InitRandomEmission();
		bool IsParticlesActive();
		void AdvanceByTime(float timeSecs);

		virtual void Start(float delay = 0.0f,CPoint *StartPos=NULL);
		virtual void StopImmediate(); // Stops everything, including all particles		
		virtual void Emit();
		//DeActivate
		virtual void StopEmit(); // Only stop emitting, but not all particles
};

#endif