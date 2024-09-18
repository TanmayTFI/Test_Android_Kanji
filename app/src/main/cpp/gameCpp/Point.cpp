//===========================================================
// 
// Framework
//===========================================================

#include <math.h>
#include "Point.h"
#include "Util.h"


namespace PointSystem
{
    CPoint gPoint(0,0,0);
    CPoint* CreateCPoint( float tx, float ty, float tz )
    {
        gPoint.SetXYZ(tx,ty,tz);
        return &gPoint;
    }
}

CPoint::CPoint()
:	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

//CPoint::CPoint(const CPoint* pos)
//{
//	x = pos->x;
//	y = pos->y;
//	z = pos->z;
//}

CPoint::CPoint(float tx, float ty, float tz)
{
	x = tx;
	y = ty;
	z = tz;
}

void CPoint::SetXY( float tx, float ty )
{
	x = tx;
	y = ty;
	z = 0;
}
void CPoint::SetXYZ( float tx, float ty, float tz )
{
	x = tx;
	y = ty;
	z = tz;
}

CPoint::CPoint( SPoint point )
{
	CPoint(point.x, point.y, point.z);
}

CPoint::~CPoint()
{
}

void CPoint::Zero()
{
	x = y = z = 0.0f;
}

bool CPoint::IsZero()
{
	if( CUtil::Equal(x, 0.0f) &&
		 CUtil::Equal(y, 0.0f) &&
		 CUtil::Equal(z, 0.0f) )
	{
		return true;
	}

	return false;
}

float CPoint::Length()
{
	return (float)sqrt( x*x + y*y + z*z);
}

void CPoint::Normalize()
{
	float length = Length();

	if( length <= 0.0f )
		return;

	x /= length;
	y /= length;
	z /= length;
}

void CPoint::Normalize( float multiplier )
{
	Normalize();

	x *= multiplier;
	y *= multiplier;
	z *= multiplier;
}

void CPoint::Constrain(float constraint)
{
	if( Length() > constraint )
	{
		Normalize(constraint);
	}
}

CPoint CPoint::Negate()
{
	CPoint temp;

	temp.x = -x;
	temp.y = -y;
	temp.z = -z;

	return temp;
}


double CPoint::Gap( float x1, float y1 )
{
	return sqrt((x-x1)*(x-x1) + 
				(y-y1)*(y-y1));
}

bool CPoint::SignsEqual( const CPoint& pos )
{
	if( (x < 0 && pos.x > 0) ||
		 (x > 0 && pos.x < 0) ||
		 (y < 0 && pos.y > 0) ||
		 (y > 0 && pos.y < 0) ||
		 (z < 0 && pos.z > 0) ||
		 (z > 0 && pos.z < 0) )
	{
		return false;
	}

	return true;
}

bool CPoint::DirectionEqual( const CPoint& pos )
{
	CPoint thisVector = (*this);
	CPoint thatVector = pos;

	thisVector.Normalize();
	thatVector.Normalize();

	return(thisVector == thatVector);
}

void CPoint::operator = (const CPoint& pos)
{
	x = pos.x;
	y = pos.y;
	z = pos.z;
}

CPoint CPoint::operator + ( const CPoint& posright )
{
	CPoint sum;

	sum.x = x + posright.x;
	sum.y = y + posright.y;
	sum.z = z + posright.z;

	return sum;
}

CPoint CPoint::operator += ( const CPoint& posright )
{
	x += posright.x;
	y += posright.y;
	z += posright.z;

	return (*this);
}
		
CPoint CPoint::operator - ( const CPoint& posright )
{
	CPoint diff;

	diff.x = x - posright.x;
	diff.y = y - posright.y;
	diff.z = z - posright.z;

	return diff;
}

bool CPoint::operator == (const CPoint& pos)
{
	if( CUtil::Equal( x, pos.x ) &&
		 CUtil::Equal( y, pos.y ) &&
		 CUtil::Equal( z, pos.z ) )
	{
		return true;
	}

	return false;
}
	
bool CPoint::operator != (const CPoint& pos)
{
	return !( (*this) == pos );
}

bool CPoint::Equal(const CPoint pos2, float Value)
{
	return ((abs((int)(pos2.x - x)) + abs((int)(pos2.y - y))) < Value);
}