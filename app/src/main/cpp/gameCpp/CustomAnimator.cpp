//===========================================================
// 
// Framework
// CustomAnimator
//===========================================================

#include "CustomAnimator.h"
#include "Util.h"
#include "IOStream.h"
#include "RenderList.h"
#include "Camera.h"
#include "Console.h"
#include "Resources.h"

CustomAnimator::CustomAnimator(std::string Str, CObject* obj, bool autoplay, bool IsLoop)
{
	targetObj = obj;

	IsCustomAnimPlaying=autoplay;
	IsCustomAnimLoop = IsLoop;
	
	animTime=0;
	TotalAnimTime=0;

	NumKeys =0;
	CurrentKey =0;

	offsetX = offsetY = 0;

	Paused = false;
		

	//Filename to uppercase!!
    int strLen = (int) Str.size();  //Sac: converted Implicit to explicit

	for( int i = 0; i < strLen; i++ )
		Str[i] = toupper(Str[i]);

	//Converts unsigned char_array to stream
	unsigned char *lpCompressedData = NULL;
	size_t nCompressedDataSize = 0;
	if(! KResource::loadResource (FileSystem::charPath(Str), lpCompressedData, nCompressedDataSize) )
	{
		std::string TmpStr = std::string("Er0x2101- AnimFile failed to open: " + FileSystem::strPath(Str)).c_str();	
		#ifdef K_WIN32
			WARN((TmpStr).c_str()) ;//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
		#else
			WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS	
		#endif
		return;
	}
	/* Copy contents and add a terminal 0 */
    char *_lpfileBytes = new char [nCompressedDataSize+1];
    memcpy (_lpfileBytes, lpCompressedData, nCompressedDataSize);//copy unsigned_chararray to const_chararray
    _lpfileBytes[nCompressedDataSize] = 0;
	std::stringstream stream(_lpfileBytes);//create stream from "const"_chararray 
    delete [] _lpfileBytes;
    _lpfileBytes = NULL;
	/////////////////////////////////////////////////////////////////////
	
	
	if( stream.fail() )
	{
		std::string TmpStr = std::string("Er0x2102- AnimFile failed to open: " + FileSystem::strPath(Str)).c_str();	
		#ifdef K_WIN32
			WARN((TmpStr).c_str()) ;//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
		#else
			WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS	
		#endif
		return;
	}

	std::string animData;
	stream >> animData;//anim name - discard
	
	stream >> NumKeys;//Number of animation keys	
	for(int i=0; i < NumKeys; ++i)
	{
		ANIMKEY *key = new ANIMKEY();
		stream >> animData;	//discard added just for clarity
		stream >> key->keytime;
		stream >> key->alpha;
		stream >> key->xpos;
		stream >> key->ypos;
		stream >> key->rot;
		stream >> key->scale;
		stream >> key->easeType;	//ease type of next key - not implemented now

		if(i==NumKeys-1)
			TotalAnimTime = key->keytime;

		AnimKeys.push_back(key);
	}
	CurrentKey = 0;
}

CustomAnimator::~CustomAnimator()
{
	targetObj = NULL;
	int size =(int)AnimKeys.size();
	for(int i=0; i < size; ++i)
	{
		delete AnimKeys[i];
	}
}

void CustomAnimator::Update(float ds)
{
	if( Paused )
		return;
	if( IsCustomAnimPlaying && NumKeys>0 )
	{
		animTime += ds*1000;//in milliseconds
		if(animTime<=TotalAnimTime)
		{
			int NextKey = CurrentKey+1;
			if( NextKey<NumKeys && animTime > AnimKeys[CurrentKey]->keytime )
			{
				//if AnimTime covers NextKey
				if( animTime >= AnimKeys[NextKey]->keytime )
				{
					CurrentKey = NextKey;
					NextKey += 1;
				}

				if( NextKey<NumKeys )
				{
					float disp = animTime - AnimKeys[CurrentKey]->keytime;//displacement
					float timeSpan = AnimKeys[NextKey]->keytime - AnimKeys[CurrentKey]->keytime;
					//Cosine Interpolate
					// const float PI=3.14151f;  //Sac: Unused variable, so commented it
					float mu = (disp/timeSpan);
					float mu2=(1-Elixir::Cos(mu*180.0f))/2;

					targetObj->SetAlpha(AnimKeys[CurrentKey]->alpha*(1-mu2)+AnimKeys[NextKey]->alpha*mu2);
					targetObj->Pos.x = AnimKeys[CurrentKey]->xpos*(1-mu2)+AnimKeys[NextKey]->xpos*mu2 + offsetX;
					targetObj->Pos.y = AnimKeys[CurrentKey]->ypos*(1-mu2)+AnimKeys[NextKey]->ypos*mu2 + offsetY;
					targetObj->SetRotate(AnimKeys[CurrentKey]->rot*(1-mu2)+AnimKeys[NextKey]->rot*mu2);
					targetObj->SetScale(AnimKeys[CurrentKey]->scale*(1-mu2)+AnimKeys[NextKey]->scale*mu2);
				}
			}
		}
		else
		{			
			if(IsCustomAnimLoop)
			{
				CurrentKey = 0;
				animTime = 0;
			}
		}
	}
}

void CustomAnimator::setFadeIn(float t)
{
	if(targetObj != NULL)
		targetObj->FadeIn(t);
}

void CustomAnimator::setFadeOut(float t)
{
	if(targetObj != NULL)
		targetObj->FadeOut(t);
}

void CustomAnimator::setEnableObject(bool f)
{
	if(targetObj != NULL)
		targetObj->SetEnable(f);
}


void CustomAnimator::SetPosOffset(int x, int y )
{
	offsetX = x;
	offsetY = y;

}

void CustomAnimator::PlayAnim(bool IsLoop)
{
	IsCustomAnimLoop = IsLoop;
	IsCustomAnimPlaying = true;
	CurrentKey = 0;
	animTime = 0;

	targetObj->SetAlpha(AnimKeys[CurrentKey]->alpha);
	targetObj->Pos.x = (float)AnimKeys[CurrentKey]->xpos + offsetX;
	targetObj->Pos.y = (float)AnimKeys[CurrentKey]->ypos + offsetY;
	targetObj->SetRotate(AnimKeys[CurrentKey]->rot);
	targetObj->SetScale(AnimKeys[CurrentKey]->scale);

	targetObj->SetEnable(true);
}

void CustomAnimator::Load(std::string Str)
{

}

bool CustomAnimator::IsAnimFinished()
{
	if(!IsCustomAnimLoop)
	{
		if( animTime > TotalAnimTime)
			return true;
	}
	return false;
}


