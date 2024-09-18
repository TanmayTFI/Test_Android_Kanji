//===========================================================
// 
// Framework
// CustomAnimGroup
//===========================================================

#include "CustomAnimGroup.h"

CustomAnimGroup::CustomAnimGroup(int numObj, std::string animArray[], CObject* objArray[], bool autoplay, bool IsLoop)
{
	numAnimations = numObj;

	IsCustomAnimPlaying = autoplay;
	IsCustomAnimLoop = IsLoop;
	
	animTime=0;
	TotalAnimTime=0;

	for (int i = 0; i < numAnimations; i++)
	{
		CustomAnimator *animator = new CustomAnimator(animArray[i], objArray[i], autoplay, IsLoop);
		if(animator->TotalAnimTime > TotalAnimTime)
			TotalAnimTime = animator->TotalAnimTime;
		_anims.push_back(animator);
	}

	Paused = false;
}

CustomAnimGroup::~CustomAnimGroup()
{
	int size =(int)_anims.size();
	for(int i=0; i < size; ++i)
	{
		delete _anims[i];
	}
}

void CustomAnimGroup::PlayAnim(bool IsLoop)
{
	animTime=0;
	int size =(int)_anims.size();
	for(int i=0; i < size; ++i)
	{
		_anims[i]->PlayAnim(IsLoop);
	}
	IsCustomAnimPlaying = true;
}

void CustomAnimGroup::Update(float ds)
{
	if( Paused )
		return;
	if( IsCustomAnimPlaying )
	{
		animTime += ds*1000;
		if(IsCustomAnimLoop)
		{
			animTime = 0;
		}
	}
}

void CustomAnimGroup::SetEnableObjects(bool f)
{
	int size =(int)_anims.size();
	for(int i=0; i < size; ++i)
	{
		_anims[i]->setEnableObject(f);
	}
}

void CustomAnimGroup::SetFadeIn(float time)
{
	int size =(int)_anims.size();
	for(int i=0; i < size; ++i)
	{
		_anims[i]->setFadeIn(time);
	}
}

void CustomAnimGroup::SetFadeOut(float  time)
{
	int size =(int)_anims.size();
	for(int i=0; i < size; ++i)
	{
		_anims[i]->setFadeOut(time);
	}
}

void CustomAnimGroup::SetPosOffset(int x, int y )
{
	int size =(int)_anims.size();
	for(int i=0; i < size; ++i)
	{
		_anims[i]->SetPosOffset(x,y);
	}
}

bool CustomAnimGroup::IsAnimFinished()
{
	if(!IsCustomAnimLoop)
	{
		if( animTime > TotalAnimTime)
			return true;
	}
	return false;
}


