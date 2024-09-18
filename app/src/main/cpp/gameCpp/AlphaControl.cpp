//====================================
// Framework
// Rocket Control
//====================================

#include "AlphaControl.h"

#define ALPHA_SPEED 2.0f

AlphaControl::AlphaControl()
{
	ACState = ACS_Inactive;
	_Alpha = 0.0f;
}

AlphaControl::~AlphaControl()
{
}

void AlphaControl::Update()
{
	switch(ACState)
	{
		case ACS_Inactive:
			return;
		case ACS_TransIn:
			UpdateTransIn();
			break;
		case ACS_TransOut:
			UpdateTransOut();
			break;
		case ACS_Active:
			SubUpdate();
			break;
	}
}

void AlphaControl::Deactivate()
{
	_Alpha = 0.0f;
	SetAlpha(_Alpha);
	Show(false);
	SetModal(false);
}

void AlphaControl::Activate()
{
	_Alpha = 0.0f;
	SetAlpha(_Alpha);

	Show(true);

	ACState = ACS_TransIn;
}

void AlphaControl::UpdateTransIn()
{
	_Alpha += Control::LogicRate * ALPHA_SPEED;

	if( _Alpha > 1.0f )
	{
		_Alpha  = 1.0f;
		ACState = ACS_Active;
	}

	SetAlpha(_Alpha);
}

void AlphaControl::UpdateTransOut()
{
	_Alpha -= Control::LogicRate * ALPHA_SPEED;

	if(_Alpha < 0.0f)
	{
		_Alpha = 0.0f;
		ACState = ACS_Inactive;
	}

	SetAlpha(_Alpha);

	if(ACState == ACS_Inactive)
	{
		Show(false);
		Close();
	}
}

void AlphaControl::End()
{
	ACState = ACS_TransOut;
}