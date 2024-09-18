//===========================================================
// 
// Framework
//===========================================================

#ifndef ANIMATION_H
#define ANIMATION_H

#include "Frame.h"

class CAnimation
{
	private:
		int msTimer;				
		bool Paused;
		float Speed;

	public:
		bool Active;
		int Frame;
		CAnimation();
		~CAnimation();

		bool Update(float ds);
		void Play();

		void Pause();

		inline void    SetSpeed(float f){ Speed = f; }		
		inline CFrame* CurrentFrame()   { return Frames[Frame]; }

		void    SetFrame( int f );
		std::string					Name;
		std::string					NextAnimation;
		std::vector< CFrame *>	Frames;
		bool							Loop;
};

#endif