//===========================================================
// 
// Framework
//===========================================================

#ifndef POINT_H
#define POINT_H

struct SPoint
{
	float x, y, z;
};

class CPoint
{
	public:
		float x,y,z;

        CPoint();
		//CPoint( const CPoint* pos);
		CPoint( float x, float y, float z );
		void SetXY( float tx, float ty);
		void SetXYZ( float tx, float ty, float tz );
		CPoint( SPoint );

		~CPoint();

		void Zero();
		bool IsZero();

		float Length();
		void Normalize();
		void Normalize( float multiplier );

		void Constrain(float constraint);

		CPoint Negate();
		double Gap( float x1, float y1 );
		bool   SignsEqual( const CPoint& pos );
		bool   DirectionEqual( const CPoint& pos );

		void operator = (const CPoint& pos);

		CPoint operator + ( const CPoint& posright );
		CPoint operator +=( const CPoint& posright );
		CPoint operator - ( const CPoint& posright );

		bool operator == (const CPoint& pos);
		bool operator != (const CPoint& pos);

		bool Equal(const CPoint pos2, float Value);

};

namespace PointSystem
{
    extern CPoint gPoint;
    extern CPoint* CreateCPoint( float tx, float ty, float tz );
}

#endif
