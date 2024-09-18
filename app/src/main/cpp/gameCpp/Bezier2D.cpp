//===========================================================
// 
// Framework
//===========================================================

#include "Bezier2D.h"

Bezier2D::Bezier2D()
{
	IsCubic = false;

	p0 = NULL;
	p1 = NULL;
	p2 = NULL;
	p3 = NULL;
}

Bezier2D::Bezier2D(float sx, float sy, float cx, float cy, float ex, float ey)
{
	IsCubic = false;

	p0 = new CPoint(sx, sy, 0);
	p1 = new CPoint(cx, cy, 0);
	p2 = new CPoint(ex, ey, 0);
	p3 = NULL;
}

Bezier2D::Bezier2D(float sx, float sy, float c1x, float c1y, float c2x, float c2y, float ex, float ey)
{
	IsCubic = false;

	p0 = new CPoint(sx, sy, 0);
	p1 = new CPoint(c1x, c1y, 0);
	p2 = new CPoint(c2x, c2y, 0);
	p3 = new CPoint(ex, ey, 0);
}

Bezier2D::Bezier2D(CPoint* P0, CPoint* P1, CPoint* P2)
{
	IsCubic = false;

	p0 = P0;
	p1 = P1;
	p2 = P2;
	p3 = NULL;
}

Bezier2D::Bezier2D(CPoint* P0, CPoint* P1, CPoint* P2, CPoint* P3)
{
	IsCubic = true;

	p0 = P0;
	p1 = P1;
	p2 = P2;
	p3 = P3;
}

CPoint Bezier2D::GetPointAt(float t)
{
	CPoint	bt;

	if(IsCubic)
	{
		bt.x = (1 - t) * (1 - t) *(1 - t) * p0->x + 3 * t * (1 - t)*(1 - t)* p1->x + 3* t * t *(1-t)* p2->x + t*t*t*p3->x;
		bt.y = (1 - t) * (1 - t) *(1 - t) * p0->y + 3 * t * (1 - t)*(1 - t)* p1->y + 3* t * t *(1-t)* p2->y + t*t*t*p3->y;
	}
	else
	{
		bt.x = (1 - t) * (1 - t) * p0->x + 2 * t * (1 - t) * p1->x + t * t * p2->x;
		bt.y = (1 - t) * (1 - t) * p0->y + 2 * t * (1 - t) * p1->y + t * t * p2->y;
	}
	return bt;
}


