//===========================================================
// 
// Bezier 2D 
//===========================================================

#ifndef BEZIER2D_H
#define BEZIER2D_H

#include "Point.h"
#include "stdio.h"

class Bezier2D
{
		bool IsCubic;
		//3 for quadratic, 4 for cubic bezier
		CPoint *p0,*p1,*p2,*p3;

	public:
		Bezier2D();

		//QUADRATIC BEZIER
		Bezier2D(CPoint* P0, CPoint* P1, CPoint* P2);
		Bezier2D(float sx, float sy, float cx, float cy, float ex, float ey);
		
		//CUBIC BEZIER
		Bezier2D(CPoint* P0, CPoint* P1, CPoint* P2, CPoint* P3);
		Bezier2D(float sx, float sy, float c1x, float c1y, float c2x, float c2y, float ex, float ey);

		CPoint GetPointAt(float t);

		~Bezier2D(){
			if(p0!=NULL) { delete p0; p0 = NULL; }
			if(p1!=NULL) { delete p1; p1 = NULL; }
			if(p2!=NULL) { delete p2; p2 = NULL; }
			if(p3!=NULL) { delete p3; p3 = NULL; }
		};

};

#endif