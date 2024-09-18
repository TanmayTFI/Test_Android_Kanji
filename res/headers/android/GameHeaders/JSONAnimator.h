//===========================================================
// 
// Framework
// Custom JSON Animator
// Used for Multiple Animations for single scene
// Faisal
//===========================================================

#ifndef BASE_JSONANIMATOR_H
#define BASE_JSONANIMATOR_H

#include "UnitAnimator.h"
#include "UpdateListEntity.h"
#include "HOScene.h"

class JSONAnimator : UpdateListEntity
{

	protected:
		std::vector<UnitAnimator *>	_unitAnimArray;

		bool _isPlaying, _isLooping;
		float _animTime, _totalAnimTime;

		bool _paused;

		bool _isSFXLoaded;
		std::string _sfxKey;

		int _animID;

		std::function<void (int)> _onCompleteCallback;

	public:
		std::string Filename;
		JSONAnimator(int animID=0, std::function<void (int)> onCompleteCallback=nullptr);
		virtual ~JSONAnimator();

		void parseAndLoadJSON(const std::string& jsonFileStr, CHOScene *hoScene, std::vector<SuperFX* >* superfxArray = NULL);

		virtual void Update(float ds);
		void SetAlphaForObjects(float alpha);
		void FadeOutObjects(float speed);
		void SetEnableObjects(bool f);
		void SetAnimTime(float animTime);
		void SetPosOffset(int x, int y );
		void PlayAnim(bool IsLoop=false);
		void StopAnim();
		bool IsAnimFinished();
		bool IsAnimPlaying();
		
};

#endif