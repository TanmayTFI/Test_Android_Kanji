//===========================================================
// 
// Framework
//===========================================================

#include "Rect.h"
#include "Util.h"
#include "Control.h"
#include "iPadDefs.h"
#include "ProfileSystem.h"

CRectangle::CRectangle()
:	w(0.0f),
	h(0.0f)
{
	topleft.x = 0;
	topleft.y = 0;
}

CRectangle::CRectangle( const CPoint* pos, float tw, float th )
{
	topleft = (*pos);
	w = tw;
	h = th;
}

CRectangle::CRectangle( float x, float y, float tw, float th )
{
	topleft.x = x;
	topleft.y = y;
	w = tw;
	h = th;
}


CRectangle::CRectangle( const CRectangle* rect )
{
	(*this) = (*rect);
}

CRectangle::~CRectangle()
{
}

void CRectangle::Init( float x, float y, float tw, float th )
{
	topleft.x = x;
	topleft.y = y;
	w = tw;
	h = th;
}

void CRectangle::operator = (const CRectangle& rect)
{
	topleft = rect.topleft;
	w = rect.w;
	h = rect.h;
}

bool CRectangle::operator == (const CRectangle& rect)
{
	if( CUtil::Equal( w, rect.w ) &&
		 CUtil::Equal( h, rect.h ) &&
		 topleft == rect.topleft )
	{
		return true;
	}

	return false;
}
	
bool CRectangle::operator != (const CRectangle& rect)
{
	return !( (*this) == rect );
}

bool CRectangle::Collision( const CRectangle* rect ) const
{
    if(  topleft.x > (rect->topleft.x + rect->w) ||
		 (topleft.x + w) < rect->topleft.x ||
		 topleft.y > (rect->topleft.y + rect->h) ||
		 (topleft.y + h) < rect->topleft.y )
	{
		return false;
	}
	
	/*
	float  X1, X2 ;
	// If it is outside our Vertical Height limit simply neglect it! 
	if(Bottom() < rect->Top() ||
	      Top() > rect->Bottom() )
		return false;
	else
	{
	     X1 = ( Left()  > rect->Left()  )? Left() :  rect->Left();
	     X2 = ( Right() < rect->Right() )? Right() : rect->Right();

		 if (X1 > X2)
             return false;			  
	}
	*/
	return true;
}

bool CRectangle::Intersection( const CPoint* pos, int Gap ) const
{
	
    if( Gap != 0 )
    {
        if(pos->x < (topleft.x-Gap)	 ||
           pos->y < (topleft.y-Gap) ||
           pos->x > (topleft.x + w+Gap) ||
           pos->y > (topleft.y + h+Gap) )
            return false;
	}
    else
    {
        if(pos->x < topleft.x	 ||
           pos->y < topleft.y ||
           pos->x > (topleft.x + w) ||
           pos->y > (topleft.y + h) )
            return false;
	}

	return true;
}

float CRectangle::GetCenterX( float OffX ) const
{
	return (topleft.x + w*0.5f + OffX);
}

float CRectangle::GetCenterY( float OffY ) const
{
	return (topleft.y + h*0.5f + OffY);
}
