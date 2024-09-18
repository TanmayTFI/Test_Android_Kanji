//===========================================================
// 
// Framework
//===========================================================

#ifndef POLYGON_H
#define POLYGON_H

#include "Point.h"
#include "Rect.h"
#include "stdio.h"

class CPolygon
{
		/*
		class Test { 
			int *a;
		public:
			Test(int size) {
			   a = new int[size];
			}
			~Test() { delete [] a; }
		private:
			Test(const Test& other);
			Test& operator=(const Test& other);
		};

		double getAverage(int arr[], int size)
		{
		  int    i, sum = 0;       
		  double avg;          

		  for (i = 0; i < size; ++i)
		  {
			sum += arr[i];
		   }

		  avg = double(sum) / size;

		  return avg;
		}
		*/

		//Co-ordinate Arrays
protected:
		int *xPts, *yPts;
		//Simple rect for initial detection
		CRectangle *polyRect;
	public:
		int numPts;
		CPolygon()
		{

		}
		CPolygon( int numPoints ){
			polyRect=NULL;
			numPts = numPoints;
			xPts = new int[numPoints];
			yPts = new int[numPoints];
		};

		virtual ~CPolygon(){	
			delete [] xPts;	
			delete [] yPts;
			//if(polyRect!=NULL)delete polyRect;  by Sac
		};

		CPoint getPoint(int i);

		void Init( int xArr[], int yArr[] );
		void Init( int xyArr[] );

		bool Intersection( const CPoint* pos ) const;
		//Offset X to move poly to right, offsetY to move Poly to Left
		bool Intersection( const CPoint* pos , int offsetX, int offsetY) const;

		CPoint center;
		float r;
};

#endif