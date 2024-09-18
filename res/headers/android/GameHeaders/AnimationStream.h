//===========================================================
// 
// Framework
//===========================================================
//==============================================================
//
// Streaming Animation. Used for large, high-frame animations.
//
//==============================================================

#ifndef ANIMATIONSTREAM_H
#define ANIMATIONSTREAM_H

#include "Framework.h"
#include "Point.h"
#include "RenderListEntity.h"
#include "UpdateListEntity.h"

enum _AnimFormats{ ANIM_PNG, ANIM_JPG , ANIM_JPG_WITHMASK, ANIM_JPG_OR_PNG  };
enum{ ANIM_NOLOOP, ANIM_LOOP };
enum{ ANIM_STATIC, ANIM_DYNAMIC };
enum{ ANIM_ALPHABLEND, ANIM_ADDITIVEBLEND };
class CAnimationStream : public CRenderListEntity, UpdateListEntity
{
	private:
		KGraphic* FrontBuffer;
		KGraphic* BackBuffer;
		
		std::string FileRoot;

		
		float  msDuration;
		float  Timer;
		bool   Finished;		
		float  Scale;
		float  Rotate;
		bool   IsHrzFlip;
		float  VerticalCrop;

	public:
        bool	IsClipped;
        void    SetClipParams( float Left, float Top, float Right, float Bottom );
        float	LeftClip, TopClip, RightClip, BottomClip;

		void SetXY( float x, float y);
		CPoint Pos;
		bool   Paused;
		float  Alpha;
		int	   CurrentFrame;
		int    NumFrames;

		void SetRotate( float rotate ){ Rotate = rotate; }
		void SetScale( float scale ){ Scale = scale; }
		void SetHrzFlip( bool FlipStatus ){ IsHrzFlip = FlipStatus; }
		void SetPos( CPoint *pos );
		bool IsAnimLoop;
		float Width, Height;
		int	 AnimType;
		bool IsAnimAdditive;
		bool IsHrzFlippable;
		CAnimationStream();
		~CAnimationStream();

		void SetDuration( float duration );
		void SetParameters( std::string fileRoot, const CPoint* pos, int numFrames, float duration = 0.0167 , int isAnimType = ANIM_PNG, bool isAnimLoop = false, bool isAnimAdditive = false, bool isHrzFlippable = false);
		void SetAlphaFade(bool);
		void FullImagesLoad();
		void FullImagesUnload();
		void LoadFrameImage( int i );
		void UnloadTillLastFrame();
		void LoadFrameBuffers(int index);
		void LoadLastFrameBuffer();
		
		void Update(float ds);
		void Render(const CPoint* cam);

		bool IsFinished(){return Finished; };

		void SwapBuffers();
		void FreeBuffers();
		void SetAlpha( float f) { Alpha = f; }

		std::string FrameName(int index);


		inline void Show(bool f)		{ Visible = f; }
		inline void SetVerticalCrop(float f) {  VerticalCrop = f; }
};

#endif