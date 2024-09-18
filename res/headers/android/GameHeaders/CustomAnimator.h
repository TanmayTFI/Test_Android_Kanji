//===========================================================
// 
// Framework
// Custom Animator
// Used for complex animations loaded 
// Faisal
//===========================================================

#ifndef BASE_CUSTOM_ANIMATOR_H
#define BASE_CUSTOM_ANIMATOR_H

#include "Object.h"
#include "UpdateListEntity.h"
#include "Console.h"

typedef struct ANIMKEY
{
	float keytime;
	float alpha;
	int xpos, ypos, easeType;
	float rot, scale;
}ANIMKEY;

class CustomAnimator : public UpdateListEntity
{

	protected:

		CObject *targetObj;
		
		std::vector<ANIMKEY *>	AnimKeys;
		bool IsCustomAnimPlaying, IsCustomAnimLoop;
		int CurrentKey, NumKeys;

		bool Paused;

	public:
		CustomAnimator(std::string Str,CObject* obj, bool autoplay=false, bool IsLoop=false);
		virtual ~CustomAnimator();

		float animTime, TotalAnimTime;

		void PlayAnim(bool IsLoop=false);

		void setEnableObject(bool f);
		void setFadeIn(float t);
		void setFadeOut(float t);
		int offsetX, offsetY;
		void SetPosOffset(int x, int y );

		//Basic Object Methods
		virtual void Load(std::string Str);
		virtual void Load(std::string Str, int SF, bool IsInteractiveObj = false){} 

		virtual void Update(float ds);

		bool IsPlaying(){	return IsCustomAnimPlaying;	};
		void Stop(){ IsCustomAnimPlaying = false; };

		bool IsAnimFinished();
};

#endif