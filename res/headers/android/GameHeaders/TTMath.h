//===========================================================
// 
// 
//===========================================================

#ifndef FRMWRK_MATH_H
#define FRMWRK_MATH_H

class CPoint;

class CMath
{
	public:
		static float Distance( const CPoint*, const CPoint* );
		static float Distance( const CPoint*, float x, float y, float z = 0.0f );

		//Return Degrees
		static float Angle( const CPoint*, const CPoint* );
		//Returns Degrees and assumes point is origin relative
		static float Angle( const CPoint* );

		//Conversion Functions
		static float ToDegrees( float radians );
		static float ToRadians( float degrees );

		//Ellipse Functions
		static float EllipseRadiusAtPoint( const CPoint* ellipseCenter, float ellipseMinRad, float ellipseMaxRad, const CPoint* point );
		static float EllipseRadiusAtAngle( float ellipseMinRad, float ellipseMaxRad, float incomingAngle );
};

#endif