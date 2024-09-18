//===========================================================
// 
// Framework
//===========================================================
//===========================================================
//
// Simple static, full-screen background. Useful for menus
// and as a static parallax layer.
//
//===========================================================

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Object.h"
#include "Rect.h"

class CBackground : public CObject
{
	private:		
		float	  Alpha;
		float     Rotate;

	public:
        bool	IsClipped;
        void    SetClipParams( float Left, float Top, float Right, float Bottom );
        float	LeftClip, TopClip, RightClip, BottomClip;
    

		KGraphic* Image;
		CBackground();
		~CBackground();
		KGraphic* GetImage(){ return Image; } 
		//Inherited methods
		virtual void	Load(std::string Str);
		virtual void	Load(std::string Str, bool IsInteractiveObj){}
		virtual void	Update(float ds);
		virtual void	Render(const CPoint* pCam);
		virtual void	SetAlpha(float alpha);
		virtual void    SetRotate(float rot);
		virtual bool	Collide(const CPoint* pos);

		CRectangle GetLocalRect();

		//Background methods
		void LoadSplashImage(const std::string& strFile);
		void LoadImg(const std::string& strFile);
};

#endif