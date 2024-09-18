//===========================================================
// ELIXIR
// Game framework and toolset
// 
//
// Elixir math functions
//===========================================================

#include "exMath.h"
#include "exAlgorithms.h"
#include "Point.h"

namespace Elixir
{
	static float SineTable[TABLESIZE] = {0};
	static float CosineTable[TABLESIZE] = {0};

	/*
	   Three control point Bezier interpolation
	   mu ranges from 0 to 1, start to end of the curve

	   This is equivalent to PTK kspline function which passes ctlrpoints controlP1,controlP2,controlP3 and position[mu] and returns kPoint
	   void getPointFromCtrlPoint( KPoint *kPoint , KPoint *controlP1, KPoint *controlP2 , KPoint *controlP3 , float position) ;
	*/
	CPoint Bezier3D(CPoint p1,CPoint p2,CPoint p3,float mu)
	{
	   float mum1,mum12,mu2;
	   CPoint p;

	   mu2 = mu * mu;
	   mum1 = 1 - mu;
	   mum12 = mum1 * mum1;
	   p.x = p1.x * mum12 + 2 * p2.x * mum1 * mu + p3.x * mu2;
	   p.y = p1.y * mum12 + 2 * p2.y * mum1 * mu + p3.y * mu2;
	   p.z = p1.z * mum12 + 2 * p2.z * mum1 * mu + p3.z * mu2;

	   return(p);
	}
	CPoint Bezier2D(float p1x, float p1y,float p2x, float p2y,float p3x, float p3y, float mu)
	{
	   float mum1,mum12,mu2;
	   CPoint p;

	   mu2 = mu * mu;
	   mum1 = 1 - mu;
	   mum12 = mum1 * mum1;
	   p.x = p1x * mum12 + 2 * p2x * mum1 * mu + p3x * mu2;
	   p.y = p1y * mum12 + 2 * p2y * mum1 * mu + p3y * mu2;
	   //since our effects are in 2 dimensional plain
	   p.z = 0;//p1z * mum12 + 2 * p2z * mum1 * mu + p3z * mu2;

	   return(p);
	}

	CPoint GetCubicBezierPoint(CPoint* p0,CPoint* p1,CPoint* p2,CPoint* p3,float t)
	{
		CPoint	bt;

		bt.x = (1 - t) * (1 - t) *(1 - t) * p0->x + 3 * t * (1 - t)*(1 - t)* p1->x + 3* t * t *(1-t)* p2->x + t*t*t*p3->x;
		bt.y = (1 - t) * (1 - t) *(1 - t) * p0->y + 3 * t * (1 - t)*(1 - t)* p1->y + 3* t * t *(1-t)* p2->y + t*t*t*p3->y;

		return bt;
	}

	float Easing(float t,float b,float c,float d)
	{
		return (c * t / d + b);
	}

	CPoint GetBezierPoint(CPoint* p1,CPoint* c1,CPoint* p2,float t)
	{
		CPoint	bt;

		bt.x = (1 - t) * (1 - t) * p1->x + 2 * t * (1 - t) * c1->x + t * t * p2->x;
		bt.y = (1 - t) * (1 - t) * p1->y + 2 * t * (1 - t) * c1->y + t * t * p2->y;

		return bt;
	}

	/*
	    Four control point Bezier interpolation
	    mu ranges from 0 to 1, start to end of curve

		This is equivalent to PTK kspline function which passes controlP1[x1,y1],controlP2[coefx1,coefy1],controlP3[coefx2,coefy2],controlP4[x2,y2] and position[mu] and returns CPoint
		void getPointFromCtrlPoint( KPoint *kPoint , KPoint *controlP1, KPoint *controlP2 , KPoint *controlP3 , float position) ;
	*/
	CPoint Bezier3D(CPoint p1,CPoint p2,CPoint p3,CPoint p4,float mu)
	{
	   float mum1,mum13,mu3;
	   CPoint p;

	   mum1 = 1 - mu;
	   mum13 = mum1 * mum1 * mum1;
	   mu3 = mu * mu * mu;

	   p.x = mum13*p1.x + 3*mu*mum1*mum1*p2.x + 3*mu*mu*mum1*p3.x + mu3*p4.x;
	   p.y = mum13*p1.y + 3*mu*mum1*mum1*p2.y + 3*mu*mu*mum1*p3.y + mu3*p4.y;
	   p.z = mum13*p1.z + 3*mu*mum1*mum1*p2.z + 3*mu*mu*mum1*p3.z + mu3*p4.z;

	   return(p);
	}

	CPoint Bezier2D(float p1x, float p1y,float p2x, float p2y,float p3x, float p3y,float p4x, float p4y,float mu)
	{
	   float mum1,mum13,mu3;
	   CPoint p;

	   mum1 = 1 - mu;
	   mum13 = mum1 * mum1 * mum1;
	   mu3 = mu * mu * mu;

	   p.x = mum13*p1x + 3*mu*mum1*mum1*p2x + 3*mu*mu*mum1*p3x + mu3*p4x;
	   p.y = mum13*p1y + 3*mu*mum1*mum1*p2y + 3*mu*mu*mum1*p3y + mu3*p4y;
	   //since our effects are in 2 dimensional plain
	   p.z = 0;//mum13*p1z + 3*mu*mum1*mum1*p2z + 3*mu*mu*mum1*p3z + mu3*p4z;

	   return(p);
	}

	void Bezier2D(CPoint *p, float p1x, float p1y,float p2x, float p2y,float p3x, float p3y,float p4x, float p4y,float mu)
	{
	   float mum1,mum13,mu3;
	   

	   mum1 = 1 - mu;
	   mum13 = mum1 * mum1 * mum1;
	   mu3 = mu * mu * mu;

	   p->x = mum13*p1x + 3*mu*mum1*mum1*p2x + 3*mu*mu*mum1*p3x + mu3*p4x;
	   p->y = mum13*p1y + 3*mu*mum1*mum1*p2y + 3*mu*mu*mum1*p3y + mu3*p4y;
	   //since our effects are in 2 dimensional plain
	   p->z = 0;//mum13*p1z + 3*mu*mum1*mum1*p2z + 3*mu*mu*mum1*p3z + mu3*p4z;
	}

	/*
	   General Bezier curve
	   Number of control points is n+1
	   0 <= mu < 1    IMPORTANT, the last point is not computed
	*/
	CPoint Bezier3D(CPoint *p,int n,float mu)
	{
	   int k,kn,nn,nkn;
	   float blend,muk,munk;
	   CPoint b = CPoint(0.0,0.0,0.0);

	   muk = 1;
	   munk = pow(1-mu,(float)n);

	   for (k=0;k<=n;k++) {
		  nn = n;
		  kn = k;
		  nkn = n - k;
		  blend = muk * munk;
		  muk *= mu;
		  munk /= (1-mu);
		  while (nn >= 1) {
			 blend *= nn;
			 nn--;
			 if (kn > 1) {
				blend /= (float)kn;
				kn--;
			 }
			 if (nkn > 1) {
				blend /= (float)nkn;
				nkn--;
			 }
		  }
		  b.x += p[k].x * blend;
		  b.y += p[k].y * blend;
		  b.z += p[k].z * blend;
	   }

	   return(b);
	}

	int Round( float f )
	{
		int min = (int)floor(f);
		if( f > (min+0.5f) )
			return (min+1);
		else
			return min;
	}

	/*
	   Determine the intersection point of two line segments
	   Return FALSE if the lines don't intersect
	*/
	int LineIntersect(
	float x1, float y1,
	float x2, float y2,
	float x3, float y3,
	float x4, float y4,
	float *x, float *y)
	{
	   float mua,mub;
	   float denom,numera,numerb;

	   denom  = (y4-y3) * (x2-x1) - (x4-x3) * (y2-y1);
	   numera = (x4-x3) * (y1-y3) - (y4-y3) * (x1-x3);
	   numerb = (x2-x1) * (y1-y3) - (y2-y1) * (x1-x3);

	   /* Are the line coincident? */
	   if (fabs(numera) < EPSILON && fabs(numerb) < EPSILON && fabs(denom) < EPSILON) {
		  *x = (x1 + x2) / 2;
		  *y = (y1 + y2) / 2;
		  return true;
	   }

	   /* Are the line parallel */
	   if (fabs(denom) < EPSILON) {
		  *x = 0;
		  *y = 0;
		  return false;
	   }

	   /* Is the intersection along the the segments */
	   mua = numera / denom;
	   mub = numerb / denom;
	   if (mua < 0 || mua > 1 || mub < 0 || mub > 1) {
		  *x = 0;
		  *y = 0;
		  return false;
	   }
	   *x = x1 + mua * (x2 - x1);
	   *y = y1 + mua * (y2 - y1);
	   return true;
	}

	//Point inside Polygon!!
	bool PointInsidePoly(int npol, int *xp, int *yp, int x, int y)
	{
	  int i, j;
	  bool c = 0;
	  for (i = 0, j = npol-1; i < npol; j = i++) {
		if ((((yp[i] <= y) && (y < yp[j])) ||
			 ((yp[j] <= y) && (y < yp[i]))) &&
			(x < (xp[j] - xp[i]) * (y - yp[i]) / (float)(yp[j] - yp[i]) + xp[i]))
		  c = !c;
	  }
	  return c;
	}	

	//=================================================
	// Creates sine and cosine look up for use with 
	// the trug functions.
	//=================================================
	void CreateTrigTables()
	{
		float m = 360.0f/(TABLESIZE-1);

		for( int i=0; i < TABLESIZE; ++i )
		{
			SineTable[i]	= (float)sin( ToRadians(i*m) );
			CosineTable[i]	= (float)cos( ToRadians(i*m) );
		}
	}

	
	//=================================================
	// Trig functions. Use a look up table to 
	// calculuate sine within half degree precision.
	//=================================================
	int IndexFromDegree(float angDegs)
	{
		angDegs = Wrap(0.0f, angDegs, 360.0f);

		int		whole		= static_cast<int>(angDegs);
		float	decimal		= angDegs - whole;
		int		decimalPart = static_cast<int>(decimal / TRIGPRECF);

		return (whole * TRIGPREC) + decimalPart;
	}

	float Sin(float angDegs)
	{
		return  SineTable[IndexFromDegree(angDegs)];
	}
	float Cos(float angDegs)
	{
		return  CosineTable[IndexFromDegree(angDegs)];
	}
	
	//=================================================
	// Distance functions. 
	//=================================================
	float Distance( int x1, int y1, int x2, int y2 )
	{
		return (float)sqrt( float(x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}
	float Distance( const CPoint* a, const CPoint* b)
	{
		return (float)sqrt( (b->x - a->x)*(b->x - a->x) + (b->y - a->y)*(b->y - a->y) + (b->z - a->z)*(b->z - a->z) );
	}
	float Distance( const CPoint* a, float x, float y, float z )
	{
		CPoint b(x,y,z);

		return Distance(a, &b);
	}

	//=================================================
	// Angle between two vectors using projection.
	//=================================================
	float Angle( const CPoint* a, const CPoint* b)
	{
		return (float)ToDegrees( (float)atan2( b->y - a->y, b->x - a->x ) );
	}
	//=================================================
	// Angle of vector relative to origin.
	//=================================================
	float Angle( const CPoint* a)
	{
		CPoint b(0,0,0);

		return Angle( a, &b );
	}

	CPoint RotatePointToAngle(const CPoint origin, const CPoint point, const float angle)
	{
		float dist = Distance(&origin, &point);
		return RotatePointToAngle(origin, dist, angle);
	}

	CPoint RotatePointByAngle(const CPoint origin, const CPoint point, const float angle)
	{
		CPoint retPoint;
		float dist = Distance(&origin, &point);
		float currentAngle = Angle(&origin, &point);
		retPoint.x = origin.x + Cos(currentAngle + angle) * dist;
		retPoint.y = origin.y - Sin(currentAngle + angle) * dist;
		return retPoint;
	}

	CPoint RotatePointToAngle(const CPoint origin, const float distance, const float angle)
	{
		CPoint retPoint;
		retPoint.x = origin.x + Cos(angle) * distance;
		retPoint.y = origin.y - Sin(angle) * distance;
		return retPoint;
	}

	double NormalizeAngle( const double value, const double start, const double end )
	{
		const double width       = end - start   ;
		const double offsetValue = value - start ;   
		return ( offsetValue - ( floor( offsetValue / width ) * width ) ) + start ;
	}

	//=================================================
	// Conversion functions.
	//=================================================
	float ToDegrees( float radians )
	{
		return radians * DpR;
	}

	float ToRadians( float degrees )
	{
		return degrees * RpD;
	}

	//=================================================
	// Comparison functions.
	//=================================================
	bool Equal(float a, float b, float eps)
	{
		if( fabs(a - b) < eps )
			return true;

		return false;
	}

	float Lerp(float startValue, float endValue, float t)
	{
		return (1 - t) * startValue + t * endValue;
	}
}