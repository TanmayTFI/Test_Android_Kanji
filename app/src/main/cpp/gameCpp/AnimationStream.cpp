//===========================================================
// 
// Framework
//===========================================================

#include "AnimationStream.h"
#include "TextureFactory.h"
#include "Util.h"
#include "IOStream.h"
#include "Constants.h"
#include "Application.h"
#include "GameFlow.h"

CAnimationStream::CAnimationStream()
{		
	FrontBuffer = NULL;
	BackBuffer = NULL;
	AnimType = ANIM_PNG; 
	IsAnimLoop = false;
	Width = 0;
	Height = 0;

	FileRoot = "invalid";

	NumFrames = 0;
	CurrentFrame = -1;
	msDuration = 0;
	Timer = 0;

	Finished = false;
	Paused = false;
	IsHrzFlip = false;
    
    LeftClip = 0.0f;
	TopClip = 0.0f;
	RightClip = 0.0f;
	BottomClip = 0.0f;
	IsClipped = false;

	Alpha = 1.0f;

	VerticalCrop = -1.0f;

	Visible = true;//CRenderListEntity
	
}

CAnimationStream::~CAnimationStream()
{
	Visible = false;
	FreeBuffers();
}

void CAnimationStream::SetPos( CPoint *pos )
{
	Pos = (*pos);
}

void CAnimationStream::SetParameters(std::string fileRoot, const CPoint *pos, int numFrames, float duration, int animType, bool isAnimLoop, bool isAnimAdditive, bool isHrzFlippable )
{
	FileRoot = fileRoot;
	IsHrzFlippable = isHrzFlippable;
	IsAnimAdditive = isAnimAdditive;
	AnimType = animType;
	IsAnimLoop = isAnimLoop;
	Pos = (*pos);
	NumFrames = numFrames;
	msDuration = duration;

	//if( NumFrames > 1000 )
	//	KMiscTools::messageBox((const char*)L"Debug Message", (const char*)L"CAnimationStream doesn't support more than 1000 frames");
}


void CAnimationStream::LoadFrameImage( int i )
{
	/* Skip library searching as we are already doing the same using !GFInstance->IsScnElmntsPreloaded[eArea_] check */
	CTexFactory::Instance->LoadImg( FrameName(i), (AnimType == ANIM_JPG_WITHMASK), true );
}
void CAnimationStream::FullImagesLoad()
{
	for( int i = 0; i < NumFrames; i++ )
		CTexFactory::Instance->LoadImg( FrameName(i), (AnimType == ANIM_JPG_WITHMASK) );
}

void CAnimationStream::FullImagesUnload()
{
	Visible = false;
	for( int i = 0; i < NumFrames; i++ )
		CTexFactory::Instance->FreeImage( FrameName(i) );		
}

void CAnimationStream::UnloadTillLastFrame()
{
	for( int i = 0; i < (NumFrames-1); i++ )
		CTexFactory::Instance->FreeImage( FrameName(i) );		
}

void CAnimationStream::LoadFrameBuffers(int index)
{
	Finished = false;
	CurrentFrame = index;

	FreeBuffers();

	FrontBuffer = CTexFactory::Instance->GetImage( FrameName(CurrentFrame), (AnimType == ANIM_JPG_WITHMASK) );
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		FrontBuffer->setTextureQuality(true);
	BackBuffer  = CTexFactory::Instance->GetImage( FrameName(CurrentFrame+1), (AnimType == ANIM_JPG_WITHMASK) );
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		BackBuffer->setTextureQuality(true);
	
	if( index == 0 )//Since we may update width/height on the way!!
	{
		Width = FrontBuffer->getWidth();
		Height = FrontBuffer->getHeight();
	}
	Timer = 0;
}


//Show animation endcase afterwards!!
void CAnimationStream::LoadLastFrameBuffer()
{
	Finished = true;
	CurrentFrame = NumFrames-1;

	FreeBuffers();

	FrontBuffer  = CTexFactory::Instance->GetImage( FrameName(CurrentFrame), (AnimType == ANIM_JPG_WITHMASK) );
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		FrontBuffer->setTextureQuality(true);
	Width = FrontBuffer->getWidth();
	Height = FrontBuffer->getHeight();

	BackBuffer = NULL;
	
	Timer = 0;
}

void CAnimationStream::SetXY( float x, float y )
{
	Pos.x = x;
	Pos.y = y;	
}

void CAnimationStream::SetDuration( float duration )
{
	msDuration = duration;
}

void CAnimationStream::Update(float ds)
{
	if( Paused || !Visible )
		return;

	if( GFInstance->IsActivePopup )
		return;

	Timer += ds;
	
	if( Timer >= msDuration )
	{
		Timer = 0;
		SwapBuffers();
	}
}

void CAnimationStream::Render(const CPoint* cam)
{	
	if(FrontBuffer)
	{
      if( IsClipped )
      {
          if( IsHrzFlippable )
          {
              FrontBuffer->setBlitColor(1.0f, 1.0f, 1.0f, Alpha);
              FrontBuffer->blitAlphaRectFxF( 0+LeftClip, 0+TopClip, Width-RightClip,Height-BottomClip,
                                            (Pos.x+LeftClip), Pos.y+TopClip,
                                            Rotate, Scale, Alpha, IsHrzFlip );
              FrontBuffer->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
          }
          else if( IsAnimAdditive )
          {
              FrontBuffer->setBlitColor(1.0f, 1.0f, 1.0f, Alpha);
              FrontBuffer->setAlphaMode(eBlend_AlphaAdditive);
              FrontBuffer->blitAlphaRectFxF( 0+LeftClip, 0+TopClip, Width-RightClip,Height-BottomClip,
                                            (Pos.x+LeftClip), Pos.y+TopClip,
                                            0.0f, 1.0f, Alpha );
              FrontBuffer->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
          }
          else if( VerticalCrop >= 0.0f )
          {
              FrontBuffer->blitAlphaRectFxF( 0+LeftClip, (FrontBuffer->getHeight() * VerticalCrop)+TopClip, Width-RightClip,Height-BottomClip,
                                            Pos.x+LeftClip, Pos.y+TopClip,
                                            0.0f, 1.0f, Alpha );
          }
          else if( Alpha < 1.0f )
          {
              FrontBuffer->blitAlphaRectFx(0+LeftClip, 0+TopClip,Width-RightClip,Height-BottomClip,
                                           (short)(Pos.x+LeftClip), (short)(Pos.y+TopClip), 0.0f, 1.0f, Alpha);
          }
          else
          {			
              FrontBuffer->blitAlphaRect(0+LeftClip, 0+TopClip,Width-RightClip,Height-BottomClip,
                                         (short)(Pos.x+LeftClip), (short)(Pos.y+TopClip));
          }
      }
      else
      {
		if( IsHrzFlippable )
		{			
			FrontBuffer->setBlitColor(1.0f, 1.0f, 1.0f, Alpha);
			FrontBuffer->blitAlphaRectFxF( 0, 0, Width,Height, 
												Pos.x, Pos.y, 
												Rotate, Scale, Alpha, IsHrzFlip );
			FrontBuffer->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if( IsAnimAdditive )
		{
			FrontBuffer->setBlitColor(1.0f, 1.0f, 1.0f, Alpha);
			FrontBuffer->setAlphaMode(eBlend_AlphaAdditive);
			FrontBuffer->blitAlphaRectFxF( 0, 0, Width,Height,  
												Pos.x, Pos.y, 
												0.0f, 1.0f, Alpha );
			FrontBuffer->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if( VerticalCrop >= 0.0f )
		{
		    FrontBuffer->blitAlphaRectFxF( 0, FrontBuffer->getHeight() * VerticalCrop, Width,Height, 
												Pos.x, Pos.y, 
												0.0f, 1.0f, Alpha );
		}
		else if( Alpha < 1.0f )
		{
			FrontBuffer->blitAlphaRectFx(0, 0,
									Width,Height,
									(short)Pos.x, (short)Pos.y, 0.0f, 1.0f, Alpha);
		}
		else
		{			
			FrontBuffer->blitAlphaRect(0, 0,
									Width,Height,
									(short)Pos.x, (short)Pos.y);			
		}
      }
	}
}

//floating values to avoid casting issues while rendering!!
void CAnimationStream::SetClipParams( float Left, float Top, float Right, float Bottom )
{
	LeftClip = Left;
	TopClip = Top;
	RightClip = Right;
	BottomClip = Bottom;
	IsClipped = true;
}

void CAnimationStream::SwapBuffers()
{
	if( CurrentFrame >= (NumFrames - 1) )
		return;

	CurrentFrame++;

	CTexFactory::Instance->FreeImage(&FrontBuffer);
	FrontBuffer = NULL;

	FrontBuffer = BackBuffer;

	BackBuffer = NULL;

	if( CurrentFrame < (NumFrames - 1) )
	{
		BackBuffer = CTexFactory::Instance->GetImage( FrameName(CurrentFrame+1), (AnimType == ANIM_JPG_WITHMASK) );
		if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
			BackBuffer->setTextureQuality(true);
	}
	else if( IsAnimLoop ) //CurrentFrame == (NumFrames - 1)
	{
		CurrentFrame = -1;		
		BackBuffer = CTexFactory::Instance->GetImage( FrameName(CurrentFrame+1), (AnimType == ANIM_JPG_WITHMASK) );		
		if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
			BackBuffer->setTextureQuality(true);
	}
	else
	{		
		Finished = true;
	}
}

void CAnimationStream::FreeBuffers()
{
	if(FrontBuffer)
		CTexFactory::Instance->FreeImage(&FrontBuffer);

	if(BackBuffer)
		CTexFactory::Instance->FreeImage(&BackBuffer);
}

std::string CAnimationStream::FrameName(int index)
{
	std::string numBuf = "";

	if( NumFrames > 100 )
	{
		 if( index < 10 )
			numBuf = "00";
		 else if( index < 100 )
			numBuf = "0";		 
	}
	else if( index < 10 )
		numBuf = "0";

	switch( AnimType )
	{
		case ANIM_JPG_WITHMASK:return std::string(FileRoot + numBuf + CUtil::ToString(index)); 
		case ANIM_JPG_OR_PNG:
				{
					std::string FileName = std::string(FileRoot + numBuf + CUtil::ToString(index) + ".JPG");
					std::string pathFileName = FileSystem::strPath(FileName);
					if( KMiscTools::fileExists(pathFileName.c_str()) )
						return FileName;
					else
						return std::string(FileRoot + numBuf + CUtil::ToString(index) + ".PNG");
				}				
				break;
		default:
		case ANIM_PNG:return std::string(FileRoot + numBuf + CUtil::ToString(index) + ".PNG");
		case ANIM_JPG:return std::string(FileRoot + numBuf + CUtil::ToString(index) + ".JPG"); 
	}
    return "null";
		
}