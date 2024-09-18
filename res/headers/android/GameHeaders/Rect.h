//===========================================================
// 
// Framework
//===========================================================

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Point.h"

class CRectangle
{
	public:
		CRectangle();
		CRectangle( const CPoint* pos, float w, float h );
		CRectangle( float x, float y, float w, float h );
		CRectangle( const CRectangle* rect );

		~CRectangle();
		virtual void Init( float x, float y, float w, float h );
		void operator = (const CRectangle& pos);

		bool operator == (const CRectangle& pos);
		bool operator != (const CRectangle& pos);

		bool Collision( const CRectangle* rect ) const;
		bool Intersection( const CPoint* pos, int Gap = 0) const;

		float GetCenterX( float OffX = 0 ) const;
		float GetCenterY( float OffY = 0 ) const;

		inline float Left()		const { return topleft.x; }
		inline float Right()	const { return topleft.x + w; }
		inline float Top()		const { return topleft.y; }
		inline float Bottom()	const { return topleft.y + h; }

		CPoint topleft;
		float w, h;
};

#endif