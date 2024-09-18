//===========================================================
// 
// Framework
//===========================================================

#ifndef SPRITE_H
#define SPRITE_H

#include "Animation.h"
#include "Rect.h"
#include "Constants.h"
#include "exMath.h"

class CSprite
{
	private:
		
		bool		Flipped;
		bool		Visible;
		
		
		float	R,G,B;
		float	pivotX,pivotY;

		std::vector< CAnimation*>	Animations;

		bool FailFlag;
		EBlend BlendMode;

	public:
		bool	IsMasked; 
		void    SetMaskParams( float Left, float Top, float Right, float Bottom );
		float		LeftMask, TopMask, RightMask, BottomMask;

		int			CurrentAnim;	
		float		Scale;
		float		Rotate;
		float		Alpha;
		KGraphic	*Image;		

		explicit CSprite( std::string Name );
		~CSprite();

		void Update(float ds);
		void Render( const CPoint* pScreenPos , float xcamOffset=0);

		void Hide();
		void Show();

		void ForcePlayAnimation(int anim);
		void SetAnimation(int anim);
		void PlayAnimation(int anim);
		bool IsAnimationFinished();
		void SetAnimSpeed(float speed);
		void SetFrame( int f );
		int  GetFrame();

		bool Collide(const CPoint* screenpos, const CPoint* collidepos, int anim = -1, int Offset = 0);

		CRectangle LocalRect();

		void Load(std::string Name);
		void Write(std::string Name);
		
		inline void	Flip(bool Flip)						{ Flipped = Flip; }
		inline int	NumAnimations()						{ return (int)Animations.size(); }		
		inline void SetAlpha(float a)					{ Alpha = a; }
		inline void SetScale(float s)					{ Scale = s; }
		inline void SetRotate(float r)					{ Rotate = r;}
		inline void SetTint(float r, float g, float b)	{ R = r; G = g; B = b; }
		inline void SetPivot(float pivx, float pivy)	{ pivotX = pivx; pivotY = pivy; }
		inline void SetBlendMode(EBlend b)				{ BlendMode = b; }
		inline bool Failed()							{ return FailFlag; }
};

#endif