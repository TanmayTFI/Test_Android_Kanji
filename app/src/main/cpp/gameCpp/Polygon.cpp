//===========================================================
// 
// Framework
//===========================================================

#include "Polygon.h"
#include "Control.h"

void CPolygon::Init( int xArr[], int yArr[])
{
	int minRectX=1366;
	int minRectY=768;
	int maxRectX=0;
	int maxRectY=0;
	for (int i = 0; i < numPts; i++)
	{
		xPts[i] = xArr[i];
		yPts[i] = yArr[i];

		if(xArr[i]<minRectX)
			minRectX = xArr[i];
		if(xArr[i]>maxRectX)
			maxRectX = xArr[i];

		if(yArr[i]<minRectY)
			minRectY = yArr[i];
		if(yArr[i]>maxRectY)
			maxRectY = yArr[i];
	}

	polyRect = new CRectangle((float)minRectX,(float)minRectY,(float)(maxRectX-minRectX),(float)(maxRectY-minRectY));
	center.SetXY(polyRect->GetCenterX(), polyRect->GetCenterY());
}

void CPolygon::Init( int xyArr[] )
{
	int minRectX=1366;
	int minRectY=768;
	int maxRectX=0;
	int maxRectY=0;
	for (int i = 0; i < numPts; i++)
	{
		xPts[i] = xyArr[i*2];
		yPts[i] = xyArr[i*2+1];

		if(xPts[i]<minRectX)
			minRectX = xPts[i];
		if(xPts[i]>maxRectX)
			maxRectX = xPts[i];

		if(yPts[i]<minRectY)
			minRectY = yPts[i];
		if(yPts[i]>maxRectY)
			maxRectY = yPts[i];
	}

	polyRect = new CRectangle((float)minRectX,(float)minRectY,(float)(maxRectX-minRectX),(float)(maxRectY-minRectY));
	center.SetXY(polyRect->GetCenterX(), polyRect->GetCenterY());
}

CPoint CPolygon::getPoint(int i)
{
	CPoint pt(0,0,0);
	if(i<numPts)
	{
		pt.x = (float)xPts[i];
		pt.y = (float)yPts[i];
	}
	return pt;
}

bool CPolygon::Intersection( const CPoint* pos ) const
{
	if(polyRect->Intersection(pos))
	{
		return Elixir::PointInsidePoly(numPts,xPts,yPts,(int)pos->x,(int)pos->y);
	}
	else
	{
		return false;
	}
}

bool CPolygon::Intersection( const CPoint* pos , int offsetX, int offsetY) const
{
	bool collided = false;

	CRectangle tempRect = *polyRect;
	CPoint offset((float)offsetX, (float)offsetY,0);
	tempRect.topleft += offset;
	
	if(tempRect.Intersection(pos))
	{
		int *xTempPts = new int[numPts];
		int *yTempPts = new int[numPts];

		for (int i = 0; i < numPts; i++)
		{
			xTempPts[i] = xPts[i] + offsetX;
			yTempPts[i] = yPts[i] + offsetY;
		}

		collided = Elixir::PointInsidePoly(numPts,xTempPts,yTempPts,(int)pos->x,(int)pos->y);

		delete [] xTempPts;	
		delete [] yTempPts;
	}
	
	return collided;
}

