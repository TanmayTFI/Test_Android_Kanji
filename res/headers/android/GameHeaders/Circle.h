//===========================================================
// 
// Framework
//===========================================================

#ifndef CIRCLE_H
#define CIRCLE_H

#include "Point.h"

class CCircle
{
	public:
		CCircle();
		CCircle( const CPoint* pos, float r );
		CCircle( float x, float y, float r );
		CCircle( const CCircle* circ );

		~CCircle();
		void Init( float x, float y, float r );
		void operator = (const CCircle& pos);

		bool operator == (const CCircle& pos);
		bool operator != (const CCircle& pos);

		bool Collision( const CCircle* circ ) const;
		bool Intersection( const CPoint* pos, int Gap = 0) const;

		CPoint center;
		float r;
};

#endif