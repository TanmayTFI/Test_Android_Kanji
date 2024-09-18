//===========================================================
// 
// Framework
//===========================================================

#ifndef HO_SCENE_H
#define HO_SCENE_H

#include "Scene2D.h"
#include "Background.h"
#include "HOObject.h"

class CHOScene : public CScene2D
{
	protected:
		int shakeOffX, shakeOffY;
		bool IsShaking;
		float ShakeTimer;
	public:
		CHOScene();
		~CHOScene();

		//HO Scene Methods
		CHOObject* pCollideWithHO(const CPoint* pMouse);	

		CObject* pGetAvailableObject();

		void CleanDirtyHOs();

		void SetZOff(int z);
		void SetPosOff(float x, float y);
		void FadeIn(float fadeSpeed=1.0f);
		void FadeOut(float fadeSpeed=1.0f);
		void SetEnable(bool enable);
		void PauseObjects(bool paused);
		void PushObject(CObject* obj);
		void InsertObject(CObject* obj,int offset);
		void PopObject();
		void RemoveObject( std::string UID );
		void RemoveObject( CObject *obj );

		//used for shaking stuff
		void Shake(int xOff, int yOff, float duration);
		void update(float ds);
};

#endif