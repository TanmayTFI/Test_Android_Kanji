//====================================
// Framework
// Timer class
//====================================

#ifndef FRMWRK_TIMER_DEF
#define FRMWRK_TIMER_DEF

#include "Framework.h"

class Timer
{
	private:
		unsigned long   PreviousTime;

		unsigned long   AvgLogicCycleMS;
		unsigned long   TotalLogicCycle;
		unsigned long   NumLogicCycles;
		unsigned long   PreviousLogicTime;

		unsigned long   AvgRenderCycleMS;
		unsigned long   TotalRenderCycle;
		unsigned long   NumRenderCycles;
		unsigned long   PreviousRenderTime;

		
		unsigned long   TotalTime;
		unsigned long   NumFrames;

	public:
		bool IsShowingFPS, IsCalculateFPS;
		int FramesPerSecond;
		unsigned long   RealDMS;
		Timer();
		~Timer();

		void Update();
		void Reset();

		//Clock times saved in MS
		void ClearCycles();

		void StartLogicCycle();
		void EndLogicCycle();
		void StartRenderCycle();
		void EndRenderCycle();

		inline unsigned long    GetAvgLogic()	{ return AvgLogicCycleMS; }
		inline unsigned long    GetAvgRender()  { return AvgRenderCycleMS; }


};

#endif