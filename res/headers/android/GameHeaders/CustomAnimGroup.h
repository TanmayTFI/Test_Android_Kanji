//===========================================================
// 
// Framework
// Custom Animator
// Used for Multiple Animations 
// Faisal
//===========================================================

#ifndef BASE_CUSTOM_ANIM_GROUP_H
#define BASE_CUSTOM_ANIM_GROUP_H

#include "CustomAnimator.h"
#include "UpdateListEntity.h"

class CustomAnimGroup : UpdateListEntity
{

	protected:
		bool _isSequential;
		
		std::vector<CustomAnimator *>	_anims;

		bool IsCustomAnimPlaying, IsCustomAnimLoop;
		float animTime, TotalAnimTime;

		bool Paused;
	public:
		CustomAnimGroup(int numObj, std::string animArray[], CObject* objArray[], bool autoplay=false, bool IsLoop=false);
		virtual ~CustomAnimGroup();

		int numAnimations;

		virtual void Update(float ds);
		void SetFadeOut(float t);
		void SetFadeIn(float t);
		void SetEnableObjects(bool f);
		void SetPosOffset(int x, int y );
		void PlayAnim(bool IsLoop=false);
		bool IsAnimFinished();
};

#endif