//===========================================================
// 
// Framework
//===========================================================

#include <iostream>
#include "Animation.h"
#include "GameFlow.h"

CAnimation::CAnimation()
:	msTimer(0),
	Paused(false),
	Active(false),
	Frame(0),
	Name("Animation"),
	NextAnimation("NULL"),
	Loop(false)
{
	Speed = 1.0f;
}

void CAnimation::SetFrame(int f)
{ 
	if( f < 0 || f >= (int)Frames.size() )
		return; 

	Frame = f; 
	msTimer = 0;
}

CAnimation::~CAnimation()
{
	int size =(int)Frames.size();
	for(int i=0; i < size; ++i)
	{
		delete Frames[i];
	}
}

bool CAnimation::Update(float ds)
{
	if( Paused )
		return false;
	

	msTimer += (int)(ds * 1000.0f);

	if( msTimer >= Frames[Frame]->msDuration * Speed )
	{
		//std::cout<<"Changing Anim Frames!\n";

		msTimer = 0;
		
		Frame++;

		if( Frame >= (int)Frames.size() )
		{
			if( Loop  )
				Frame = 0;
			else
			{
				Active = false;
				Frame = (int)(Frames.size() - 1);
			}
		}
	}

	return false;
}

void CAnimation::Play()
{
	msTimer = 0;
	Frame = 0;
	Active = true;
}

void CAnimation::Pause()
{
	Paused = !Paused;
}