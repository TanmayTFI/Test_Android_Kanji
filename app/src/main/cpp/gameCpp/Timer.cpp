//====================================
// Framework
// Timer class
//====================================

#include "Timer.h"

Timer::Timer()
{
	RealDMS = 0;
	FramesPerSecond = 0;
	IsShowingFPS = false;
	IsCalculateFPS = false;

	AvgLogicCycleMS = 0;
	TotalLogicCycle = 0;
	NumLogicCycles = 0;
	PreviousLogicTime = 0;

	AvgRenderCycleMS = 0;
	TotalRenderCycle = 0;
	NumRenderCycles = 0;
	PreviousRenderTime = 0;

	TotalTime = 0;
	NumFrames = 0;
	
	PreviousTime = KMiscTools::getMilliseconds();
}

Timer::~Timer()
{
}

void Timer::Update()
{
	unsigned long CurrentTime = KMiscTools::getMilliseconds();
	RealDMS = (CurrentTime - PreviousTime);	
	if( RealDMS > 250)//Clamp our delta, for when the application is out of focus.		
		RealDMS = 250;//Logger::Log("Delta anamoly in timer.");
	else if(RealDMS < 1)
		RealDMS = 1;
	TotalTime += RealDMS;
	
	if(IsShowingFPS || IsCalculateFPS)
	{
		NumFrames++;
		if( NumFrames > 0 && TotalTime > 0 )
			FramesPerSecond = 1000 / (TotalTime/NumFrames);
	}

	if(TotalTime >= 2000)	
		ClearCycles();
	PreviousTime = CurrentTime;
}



void Timer::Reset()
{
	PreviousTime = KMiscTools::getMilliseconds();
}

void Timer::StartLogicCycle()
{
	PreviousLogicTime = KMiscTools::getMilliseconds();
}

void Timer::EndLogicCycle()
{
	unsigned long deltaTime = KMiscTools::getMilliseconds() - PreviousLogicTime;

	TotalLogicCycle += deltaTime;
	NumLogicCycles++;

	AvgLogicCycleMS = TotalLogicCycle/NumLogicCycles;
}

void Timer::StartRenderCycle()
{
	PreviousRenderTime = KMiscTools::getMilliseconds();
}

void Timer::EndRenderCycle()
{
	unsigned long deltaTime = KMiscTools::getMilliseconds() - PreviousRenderTime;

	TotalRenderCycle += deltaTime;
	NumRenderCycles++;

	AvgRenderCycleMS = TotalRenderCycle/NumRenderCycles;
}

void Timer::ClearCycles()
{
	AvgLogicCycleMS = 0;
	TotalLogicCycle = 0;
	NumLogicCycles = 0;
	PreviousLogicTime = KMiscTools::getMilliseconds();

	AvgRenderCycleMS = 0;
	TotalRenderCycle = 0;
	NumRenderCycles = 0;
	PreviousRenderTime = KMiscTools::getMilliseconds();

	TotalTime = 0;
	NumFrames = 0;
}