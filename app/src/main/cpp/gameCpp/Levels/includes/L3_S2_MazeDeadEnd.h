
#ifndef L3_S2_MazeDeadEnd_
#define L3_S2_MazeDeadEnd_

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

class L3_S2_MazeDeadEnd : public HiddenObjectSystem
{
	SuperFX* LightParticleFX;

public:
	L3_S2_MazeDeadEnd(void);
	virtual ~L3_S2_MazeDeadEnd(void);

private:

	void ProcessNonHOHint();
	void ReceiveMessage(int val);
	void Update();
	void NavigateToScene();
};

#endif

