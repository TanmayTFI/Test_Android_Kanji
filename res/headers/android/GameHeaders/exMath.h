//===========================================================
// ELIXIR
// Game framework and toolset
// 
//
// Elixir math functions
//===========================================================

#ifndef ELIXIR_MATH_DEF
#define ELIXIR_MATH_DEF

#include <cmath> 
#include "Point.h"

#define PIE			3.14159265f
#define DpR			57.29577950f
#define RpD			0.01745329f
//Number of table lookups per degree in trig tables
//For increased precision (and memory use/access speed)
//increase this value.
#define TRIGPREC	2
#define TRIGPRECF	(1.0f/TRIGPREC)
#define TABLESIZE	((360*TRIGPREC) + 1)
#define EPSILON		0.0000001f

namespace Elixir
{
	/*
	   Three control point Bezier interpolation
	   mu ranges from 0 to 1, start to end of the curve

	   This is equivalent to PTK kspline function which passes ctlrpoints controlP1,controlP2,controlP3 and position[mu] and returns kPoint
	   void getPointFromCtrlPoint( KPoint *kPoint , KPoint *controlP1, KPoint *controlP2 , KPoint *controlP3 , float position) ;
	*/	
	CPoint Bezier3D(CPoint p1,CPoint p2,CPoint p3,float mu);
	CPoint Bezier2D(float p1x, float p1y,float p2x, float p2y,float p3x, float p3y, float mu);

	float Easing(float t,float b,float c,float d);
	/*
	GetBezierPoint from 2 points and one control point and time - added by monster
	*/
	CPoint GetBezierPoint(CPoint* p1,CPoint* c1,CPoint* p2,float t);
	/*
	Cubic Bezier from 2 points and 2 control points and time - added by monster
	*/
	CPoint GetCubicBezierPoint(CPoint* p0,CPoint* p1,CPoint* p2,CPoint* p3,float t);

	/*
	    Four control point Bezier interpolation
	    mu ranges from 0 to 1, start to end of curve

		This is equivalent to PTK kspline function which passes controlP1[x1,y1],controlP2[coefx1,coefy1],controlP3[coefx2,coefy2],controlP4[x2,y2] and position[mu] and returns CPoint
		void getPointFromCtrlPoint( KPoint *kPoint , KPoint *controlP1, KPoint *controlP2 , KPoint *controlP3 , float position) ;
	*/	
	CPoint Bezier3D(CPoint p1,CPoint p2,CPoint p3,CPoint p4,float mu);
	CPoint Bezier2D(float p1x, float p1y,float p2x, float p2y,float p3x, float p3y,float p4x, float p4y,float mu);
	void Bezier2D(CPoint *p, float p1x, float p1y,float p2x, float p2y,float p3x, float p3y,float p4x, float p4y,float mu);
	/*
	   General Bezier curve
	   Number of control points is n+1
	   0 <= mu < 1    IMPORTANT, the last point is not computed
	*/
	CPoint Bezier3D(CPoint *p,int n,float mu);

	int Round( float f );
	
	/*
	   Determine the intersection point of two line segments
	   Return FALSE if the lines don't intersect
	*/
	int LineIntersect(float x1, float y1,float x2, float y2,float x3, float y3,float x4, float y4,float *x, float *y);

	//Point inside Polygon!!
	bool PointInsidePoly(int npol, int *xp, int *yp, int x, int y);

	//=================================================
	// Creates sine and cosine look up for use with 
	// the trug functions.
	//=================================================
	void CreateTrigTables();


	//=================================================
	// Trig functions. Use a look up table to 
	// calculuate sine within half degree precision.
	// Parameter is accepted as degrees.
	//=================================================
	float Sin(float angDegs);
	float Cos(float angDegs);
	
	//=================================================
	// Distance functions. 
	//=================================================
	float Distance( int x1, int y1, int x2, int y2 );
	float Distance( const CPoint* a, const CPoint* b);
	float Distance( const CPoint* a, float x, float y, float z = 0.0f );

	//=================================================
	// Angle between two vectors using projection.
	//=================================================
	float Angle( const CPoint* a, const CPoint* b);
	//=================================================
	// Angle of vector relative to origin.
	//=================================================
	float Angle( const CPoint* a);

	CPoint RotatePointToAngle(const CPoint origin, const CPoint point, const float angle);
	CPoint RotatePointToAngle(const CPoint origin, const float distance, const float angle);
	CPoint RotatePointByAngle(const CPoint origin, const CPoint point, const float angle);
	double NormalizeAngle( const double value, const double startAngle = 0.0f, const double endAngle = 360.0f);

	//=================================================
	// Conversion functions.
	//=================================================
	float ToDegrees( float radians );
	float ToRadians( float degrees );

	//=================================================
	// Comparison functions.
	//=================================================
	bool Equal(float a, float b, float eps = EPSILON);
	float Lerp(float startValue, float endValue, float t);
}

#endif