//===========================================================
// 
// Framework
//===========================================================

#ifndef CELL_H
#define CELL_H

#include "Framework.h"

class CCell
{
	public:
		CCell()
		{
			tx = ty = 0.0f;
			rx = ry = 0.0f;
			w = h = 0.0f;
			rot = 0.0f;
			alpha = 0.0f;
			mirror = false;
		};
		
		float tx, ty, rx, ry, w, h;
		float rot, alpha;
		bool  mirror;
};

#endif