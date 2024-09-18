//===========================================================
// 
// Framework
//===========================================================

#include "Circle.h"
#include "Util.h"
#include "Control.h"
#include "iPadDefs.h"
#include "ProfileSystem.h"

CCircle::CCircle()
	:	r(0.0f)
{
	center.x = 0;
	center.y = 0;
	r = 0 ;
}

CCircle::CCircle( const CPoint* pos, float r)
{
	center = (*pos);
	r = r;
}

CCircle::CCircle( float x, float y, float r)
{
	center.x = x;
	center.y = y;
	r = r;
}


CCircle::CCircle( const CCircle* circ )
{
	(*this) = (*circ);
}

CCircle::~CCircle()
{
}

void CCircle::Init( float x, float y, float r )
{
	center.x = x;
	center.y = y;
	r = r;
}

void CCircle::operator = (const CCircle& circ)
{
	center = circ.center;
	r = circ.r;
}

bool CCircle::operator == (const CCircle& circ)
{
	if( CUtil::Equal( r, circ.r ) && center == circ.center )
	{
		return true;
	}

	return false;
}
	
bool CCircle::operator != (const CCircle& circ)
{
	return !( (*this) == circ );
}

bool CCircle::Collision( const CCircle* circ ) const
{
	if(  Elixir::Distance(&circ->center,center.x,center.y) > (2*r))
	{
		return false;
	}
	
	return true;
}

bool CCircle::Intersection( const CPoint* pos, int Gap ) const
{
	if( Elixir::Distance(pos,&center) < r )
		return true;

	return false;
}


