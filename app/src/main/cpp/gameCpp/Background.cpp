//===========================================================
// 
// Framework
//===========================================================

#include "Background.h"
#include "TextureFactory.h"
#include "IOStream.h"
#include "Util.h"
#include "Camera.h"
#include "Rect.h"
#include "Application.h"

// #define min(a,b)            (((a) < (b)) ? (a) : (b))
// Sac: why redfine min ?
CBackground::CBackground()
:	Image(NULL)
{	
	Alpha = 1.0f;
	Type = BACKGROUND;
	pSprite = NULL;
	Rotate = 0.0f;
    
    LeftClip = 0.0f;
	TopClip = 0.0f;
	RightClip = 0.0f;
	BottomClip = 0.0f;
	IsClipped = false;
    
	Visible = true;//CRenderListEntity
}

CBackground::~CBackground()
{
	CTexFactory::Instance->FreeImage(&Image);
	Image = NULL;
}

void CBackground::Load(std::string Str)
{
	LoadImg(Str);	
}

void CBackground::Update(float ds)
{
	if( Visible )
		CObject::Update(ds);
}

void CBackground::Render(const CPoint* pCam)
{	
	CPoint screenPos;
	CPoint originPos;

	if( Cam )
	{
		originPos = Cam->ViewOrigin;
	}

	if(Image)
	{
      screenPos = Pos - originPos;

      //NEED TO IMPLEMENT -- CLIPPING and ACCURATE DRAW :) - cheap draw functions
	  //Sac: changed all min to g_min as defined in constants.h
      if( IsClipped )
      {
          if( Alpha != 1.0f || Rotate != 1.0f )
          {
              Image->setBlitColor(1.0f,1.0f,1.0f,Alpha);
              Image->blitAlphaRectFx(0+LeftClip, 0+TopClip,
                                     g_min(1366, Image->getWidth()-RightClip),
                                     g_min(768, Image->getHeight()-BottomClip),
                                     (short)(screenPos.x+LeftClip), (short)(screenPos.y+TopClip),Rotate, 1.0f, Alpha );
              Image->setBlitColor(1.0f,1.0f,1.0f,1.0f);
          }
          else
              Image->blitRect(0+LeftClip, 0+TopClip,g_min(1366, Image->getWidth()-RightClip),
                              g_min(768, Image->getHeight()-BottomClip),
                              (short)(screenPos.x+LeftClip), (short)(screenPos.y+TopClip));
      }
      else
      {
		if( Alpha != 1.0f || Rotate != 1.0f )
		{
			Image->setBlitColor(1.0f,1.0f,1.0f,Alpha);
			Image->blitAlphaRectFx(0, 0,
								 g_min(1366, Image->getWidth()),
								 g_min(768, Image->getHeight()),
								 (short)screenPos.x, (short)screenPos.y,Rotate, 1.0f, Alpha );			
			Image->setBlitColor(1.0f,1.0f,1.0f,1.0f);
		}
		else
			Image->blitRect(0, 0,g_min(1366, Image->getWidth()),
								 g_min(768, Image->getHeight()),
								 (short)screenPos.x, (short)screenPos.y);
      }
	}
}


//floating values to avoid casting issues while rendering!!
void CBackground::SetClipParams( float Left, float Top, float Right, float Bottom )
{
	LeftClip = Left;
	TopClip = Top;
	RightClip = Right;
	BottomClip = Bottom;
	IsClipped = true;
}

CRectangle CBackground::GetLocalRect()
{
	CRectangle rect;

	if(Image != NULL)
	{
		rect.w =Image->getWidth();
		rect.h =Image->getHeight();
	}

	return rect;
}

void CBackground::SetAlpha(float alpha)
{
	Alpha = alpha;
}

void CBackground::SetRotate(float rot)
{
	Rotate = rot;
}

void CBackground::LoadSplashImage(const std::string& strFile)
{
	CTexFactory::Instance->FreeImage(&Image);
	
	Image = CTexFactory::Instance->GetImage(strFile);
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		Image->setTextureQuality(true);
}

void CBackground::LoadImg(const std::string& strFile)
{
	Image = CTexFactory::Instance->GetImage(strFile);
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		Image->setTextureQuality(true);
}

bool CBackground::Collide(const CPoint* collidepos)
{
	if( Visible )
	{
		CRectangle rect(Pos.x, Pos.y, Image->getWidth(), Image->getHeight());

		return rect.Intersection(collidepos);
	}

	return false;
}
