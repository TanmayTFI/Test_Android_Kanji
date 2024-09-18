//====================================
// Framework
// Rocket Control
//====================================

#ifndef DQFULP_ALPHA_CONTROL_DEF
#define DQFULP_ALPHA_CONTROL_DEF

#include "Control.h"

enum eAlphaControlState
{
	ACS_Inactive,
	ACS_TransIn,
	ACS_Active,
	ACS_TransOut,
};

class AlphaControl : public Control
{
	public:
		AlphaControl();
		virtual ~AlphaControl();

		//Inherited method
		void Update();

		void Activate();
		void Deactivate();

		bool isCtrlActive() { return (ACState == ACS_Active); }
	protected:		
		float _Alpha;
		eAlphaControlState ACState;
		
		//Abstract Methods
		virtual void SubUpdate() = 0;
		virtual void Close() = 0;
		virtual void Show(bool) = 0;
		virtual void SetAlpha(float) = 0;

		void UpdateTransIn();
		void UpdateTransOut();
		void End();
};

#endif