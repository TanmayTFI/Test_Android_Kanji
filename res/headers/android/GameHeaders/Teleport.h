//====================================
// DQFULP
// Teleport Handling
//====================================

#ifndef DQFULP_TELEPORT_DEF
#define DQFULP_TELEPORT_DEF

#include "Control.h"
#include "SuperFX.h"
#include "HOScene.h"
#include <string>

class CObject;
class CHOScene;

class TeleportSystem : public Control
{
	public:
		//Scn Elements
		CObject *TeleBG, *TeleOverlay, *TeleSceneThumb, *TeleThumbGlow;
		CHOScene *TeleScene;

		float waveTimer, waveOffset;

		int targetAreaID;

		bool isActive, isBGTweening;

		SuperFX *VFX_BG;

		TeleportSystem();
		~TeleportSystem();

		//Inherited method
		void Update();
		void Required();

		void  ShowTeleport();
		void  HideTeleport();
		void  SetupTeleport();

		void SetupMorphTeleport();

	protected:		
};

#endif