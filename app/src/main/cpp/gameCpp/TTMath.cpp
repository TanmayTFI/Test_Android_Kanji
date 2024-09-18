//===========================================================
// 
// 
//===========================================================

#include <cmath>
#include "Point.h"
#include "TTMath.h"

float CMath::Distance(const CPoint *a, const CPoint *b)
{
	return (float)sqrt( (b->x - a->x)*(b->x - a->x) + (b->y - a->y)*(b->y - a->y) + (b->z - a->z)*(b->z - a->z) );
}

float CMath::Distance(const CPoint *a, float x, float y, float z)
{
	CPoint b(x,y,z);

	return CMath::Distance(a, &b);
}

float CMath::Angle( const CPoint* a, const CPoint* b)
{
	return (float)ToDegrees( (float)atan2( b->y - a->y, b->x - a->x ) );
}

float CMath::Angle( const CPoint* b)
{
	CPoint a(0,0,0);

	return Angle( &a, b );
}

float CMath::ToDegrees( float radians )
{
	return radians * 57.29577950f;
}

float CMath::ToRadians( float degrees )
{
	return degrees * 0.01745329250f;
}

//If the point is not on the ellipse, the result is based on the point projected onto the ellipse.
float CMath::EllipseRadiusAtPoint( const CPoint* ellipseCenter, float ellipseMinRad, float ellipseMaxRad, const CPoint* point )
{
	float angleBetween = CMath::Angle( ellipseCenter, point );

	return EllipseRadiusAtAngle( ellipseMinRad, ellipseMaxRad, angleBetween );
}

//Assumes incoming angle is 0-359 bound and in degrees
float CMath::EllipseRadiusAtAngle( float ellipseMinRad, float ellipseMaxRad, float incomingAngle )
{
	int quadrant = (int)(incomingAngle/90.0f);

	//Calculate the percent of the next axis the angle represents
	float PercentMajor = (incomingAngle - quadrant*90.0f) / 90.0f;

	//Invert the percentage when the angle's
	//next axis is a minor axis.
	if(quadrant == 0 || quadrant == 2)
		PercentMajor = 1.0f - PercentMajor;

	return ellipseMinRad + (ellipseMaxRad-ellipseMinRad)*PercentMajor;
}