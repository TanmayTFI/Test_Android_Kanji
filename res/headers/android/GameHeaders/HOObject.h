//===========================================================
// 
// Framework
//===========================================================

#ifndef HO_OBJECT_H
#define HO_OBJECT_H

#include "Object.h"

#define MAX_INVBOXOBJ_SIZE 55
enum _ParticleVFX{ VFX_REDSPARKLE, VFX_FOUNDHO };
enum EHOStatus
{
		eHO_Hidden,
		eHO_Leaving,
	eHO_Dirty,
	eHO_Fading,
	eHO_StandStill,
	eHO_FoundHOFX,
		eHO_Finished	
};

class ParticleEffect;

class CHOObject : public CObject
{
	protected:				
		bool IsWaitTillPickup;		
		CPoint			Vel;		
		int ParticleVFXType;

	public:
		bool IsJrnPage;
		bool IsActive;
		int eInv_Id;
		int nFoundHOFX;
		bool IsFoundHOFXIncr;
		float FoundHOFXScale;

		std::string		HOName;

		CPoint	TargetPos;
		CPoint	ControlPos;
		float TrailTime;
		float TimeFactor;
		bool IsScalingUp;
		ParticleEffect *Effect;
		EHOStatus		Status;
		bool IsSlowMotion;
		int  MinScaleDim; 
		int  nSlowMotionFrames;

		CHOObject(int ParticleVFX = VFX_REDSPARKLE);
		virtual ~CHOObject();

		//Inherited methods
		virtual void Load(std::string Str);
		virtual void Load(std::string Str, bool IsInteractiveObj);
		
		void Update(float ds);
		void Render(const CPoint* pCam);
		bool Collide(const CPoint* pos);

		//HO Object Methods
		void SetHOType(const std::string& type);
		

		void InitFoundHOFX();
		void Remove();
		bool IsStartWait;
		float StartWaitTimer; 
		void Remove(const CPoint* pTarget, float velOff = 1.0f, bool isWaitTillPickup = false, int minScaleDim = 32,bool isStartWait = false, bool isCursorTurnOff = true, bool isJrnPage = false);

		void InstantRemove();

		inline void SetDirty() { Status = eHO_Dirty; Dirty = true;}		
};

#endif