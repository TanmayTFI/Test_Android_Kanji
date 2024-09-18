//===========================================================
// 
// Framework
//===========================================================

#ifndef FRAME_H
#define FRAME_H

#include "Cell.h"

class CFrame
{
	public:
		CFrame()
		{
			msDuration = 60;
		};

		~CFrame()
		{
			int size = (int)Cells.size();
			for(int i=0; i < size; ++i)
			{
				delete Cells[i];
			}
		};

		std::vector< CCell *> Cells;
		int msDuration;
};

#endif