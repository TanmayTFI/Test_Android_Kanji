//===========================================================
// 
// Framework
//===========================================================

#include "HOObject.h"
#include "Sprite.h"
#include "IOStream.h"
#include "ParticleEffect.h"
#include "Application.h"
#include "GameFlow.h"
#include "Hud.h"
#include "ProfileSystem.h"
#include "iPadDefs.h"

#define START_WAIT_TIMER 0.35f
#define HO_SPEED 400.0f
#define MAX_FOUNDHO_FX 5

CHOObject::CHOObject( int ParticleVFX )
:	HOName("Unknown Item"),
	Status(eHO_Hidden)
{
	IsActive = false;
	IsJrnPage = false;
	nFoundHOFX = 0;
	IsFoundHOFXIncr = false;
	FoundHOFXScale = 0.0f;
	Type = HIDDEN_OBJECT;
	eInv_Id = -1;

	TrailTime = 0;
	TimeFactor = 1.0f;


	IsStartWait = false;
	IsScalingUp = false;
	StartWaitTimer = 0.0f;
	IsWaitTillPickup = false;
	MinScaleDim = 32;
	Effect = NULL;
	Effect = new ParticleEffect();
	IsSlowMotion = false;
	//switch( ParticleVFX )
	{
		//case VFX_REDSPARKLE: Effect->LoadEffectIni("PRT\\ho_rsparkle.INI"); 
		//					break;		
		//case VFX_FOUNDHO:   Effect->LoadEffectIni("PRT\\ls5.INI"); break;//YELLOW
		//					break;
		//default           :	Effect->LoadEffectIni("PRT\\ho_rsparkle.INI"); //blue star!!!!
		//					break;
	}
	ParticleVFXType = ParticleVFX;
	
	nSlowMotionFrames = 0;
}



CHOObject::~CHOObject()
{
	Effect->Stop();
	delete Effect;
	Effect = NULL;
	//Let the base class clean up our sprite
}

void CHOObject::Load(std::string Str )
{
	HOName = Str;
}

void CHOObject::Load(std::string Str, bool IsInteractiveObj )
{
	HOName = Str;
	IsActive = !IsInteractiveObj;
}


void CHOObject::Update(float ds)
{
	if( !Visible )
		return;

	if( IsStartWait )
	{
		StartWaitTimer -= Control::LogicRate;
		if( StartWaitTimer <= 0.0f )
		{
			StartWaitTimer = 0.0f;
			IsStartWait = false;
		}
		return;
	}

	CObject::Update(ds);	
	
	if(Status == eHO_Leaving)
	{		

		if( IsSlowMotion )
			nSlowMotionFrames = (nSlowMotionFrames+1)%5;
		else 
			nSlowMotionFrames = (nSlowMotionFrames+1)%2;		

		Effect->SetZ(ZOrder);//For every updation effects
		Effect->SetPos(PointSystem::CreateCPoint(Pos.x-15,Pos.y-20,0));
			
		if( nSlowMotionFrames == 0 )
			Effect->Emit();

		TrailTime += (TimeFactor*ds);

		//t is the current time (or position) of the tween. This can be seconds or frames, steps, seconds, ms, whatever as long as the unit is the same as is used for the total time [3]
		//b is the beginning value of the property
		//c is the change between the beginning and destination value of the property
		//d is the total time of the tween
		float easedTrailTime= Elixir::Sin(TrailTime*90.0f); //Sin ease out - Sin(d* PI/2);
		Pos = Elixir::GetBezierPoint(&InitPos, &ControlPos, &TargetPos, easedTrailTime);

		//Pos.x += Vel.x * ds;
		//Pos.y += Vel.y * ds;

		CPoint dif = TargetPos - Pos;
		dif.x = (float)abs((int)dif.x);
		dif.y = (float)abs((int)dif.y);

		if( !IsWaitTillPickup )
		{
			float scale = pSprite->Scale;
			float ScaleDim =  (pSprite->LocalRect().w < pSprite->LocalRect().h)? pSprite->LocalRect().w: pSprite->LocalRect().h; 
			if(  /*scale > 0.5f || */(ScaleDim * pSprite->Scale) > MinScaleDim )
			{
				scale -= 1.0f * ds;
				//scale = max(scale, 0.5f);

				pSprite->SetScale(scale);
			}
		}
		if( IsScalingUp )
		{
			float scale = pSprite->Scale;
			if( scale < 1.0f )
				scale += 0.25f*ds;				
			else
			{
				IsScalingUp = false;
				scale = 1.0f;
			}
			pSprite->SetScale(scale);
		}

		bool IsTargetReached = false;
		if( TrailTime > 1.0f )
			IsTargetReached = true;	
		else if( !IsWaitTillPickup )//A object flying to HUD [non world object]
		{
			if( Vel.x > 0 )//rightside
			{
				if( Vel.y < 0 )//right-top 
				{
					//PuzzleOff - HO
					if( TargetPos.x == PuzzleOff.x && TargetPos.y == PuzzleOff.y )
					{
						if( Pos.x >= PuzzleOff.x && Pos.y <= PuzzleOff.y )
							IsTargetReached = true;
					}
				}			
				else//right-bottom
				{
					//invbox - HO & SPD
					/* if( TargetPos.x == InvBoxTargetHitPos.x && TargetPos.y == InvBoxTargetHitPos.y )
					{
						if( Pos.x >= InvBoxTargetHitPos.x && Pos.y >= InvBoxTargetHitPos.y )
							IsTargetReached = true;
					}
					//MapButtonPos - Only HO uses this one					
					else */if( TargetPos.x == MapButtonPos.x && TargetPos.y == MapButtonPos.y )
					{	
						if( Pos.x >= MapButtonPos.x && Pos.y >= MapButtonPos.y )
							IsTargetReached = true;
					}
				}			
			}
			else if( Vel.x < 0 )//leftside
			{
				if( Vel.y < 0 )//left-top
				{				
					//menu not used!!
				}
				else //left-bottom
				{
					//PuzzleOffb - SPD
					if( TargetPos.x == PuzzleOffb.x && TargetPos.y == PuzzleOffb.y )
					{
						if( Pos.x <= PuzzleOffb.x && Pos.y >= PuzzleOffb.y )
							IsTargetReached = true;
					}					
					//JrnHOPos - HO
					else if( TargetPos.x == JrnHOPos.x && TargetPos.y == JrnHOPos.y )
					{
						if( Pos.x <= JrnHOPos.x && Pos.y >= JrnHOPos.y )
							IsTargetReached = true;
					}													
				}
			}
		}
		
		//Forced ending if out of scene
		//Since some museum objects start from negative start pos!!
		if( Vel.x < 0 )
		{
			if( Pos.x < 0 ) 
				IsTargetReached = true;
		}
		else if( Vel.x > 0 )
		{
			if( Pos.x > 1366 ) 
				IsTargetReached = true;
		}
		if( Vel.y < 0 )
		{
			if( Pos.y < 0 ) 
				IsTargetReached = true;
		}
		else if( Vel.y > 0 )
		{
			if( Pos.y > 768 ) 
				IsTargetReached = true;
		}
		
		if( IsJrnPage )
		{
			if( pSprite->Scale < 0.5f )
				pSprite->SetAlpha(2*pSprite->Scale); 		
			if( pSprite->Scale <= 0.025f )
				IsTargetReached = true;
		}
		if( IsTargetReached )
		{			
		//	K_LOG("Inv Fly Target reached");
			if( IsWaitTillPickup )	
			{
				Status = eHO_StandStill;			
				Control::Audio->PlaySample(aSFXMagic);//notify user that he just arrived!!
			}
			else
			{
				Status = eHO_Finished;
				pSprite->Hide();				
			}
			SetPos(&TargetPos);
			Cursor::Show(true);
			if( GFHud )
				GFHud->EnableInput(true);
			Application::SetSingleUpdate(false);
			Effect->StopEmit();	//No need of further emission!!
		}
	}
	else if( Status == eHO_FoundHOFX )
	{
		if( IsFoundHOFXIncr )
		{
			FoundHOFXScale += 1.0f*ds;
			if( FoundHOFXScale >= 0.25f )//0->0.25f
			{
				nFoundHOFX++; 
				FoundHOFXScale = 0.25f;
				IsFoundHOFXIncr = false;
				if( nFoundHOFX >= MAX_FOUNDHO_FX )//even numbers => orig scale/alpha
				{
					Effect->StopEmit();
					Status = eHO_Fading;			
				}
			}	
		}
		else
		{
			FoundHOFXScale -= 1.0f*ds;
			if( FoundHOFXScale <= 0.0f )//0.25f->0
			{
				nFoundHOFX++;
				FoundHOFXScale = 0.0f;
				IsFoundHOFXIncr = true;
				if( nFoundHOFX >= MAX_FOUNDHO_FX )//even numbers => orig scale/alpha
				{
					Effect->StopEmit();
					Status = eHO_Fading;			
				}
				else if( nFoundHOFX == (MAX_FOUNDHO_FX-1) )
				{
					Effect->StopEmit();//FADING OUT!!					
				}
			}
		}		
		pSprite->SetAlpha( 1.0f - 0.25f*FoundHOFXScale );//1.0<->0.9375f
		pSprite->SetScale( 1.0f + FoundHOFXScale);//1.0f<->1.25f
	}
	else if(Status == eHO_Fading)
	{
		pSprite->SetAlpha( pSprite->Alpha - 0.8f * ds );

		if( pSprite->Alpha <= 0.15f )
		{
			pSprite->SetAlpha(0.0f);

			Status = eHO_Finished;

			pSprite->Hide();
		}
	}
}

void CHOObject::Render(const CPoint* pCam)
{
	CObject::Render(pCam);
	//Add fancy stuff here later
}

bool CHOObject::Collide(const CPoint* pos)
{
	if(Status != eHO_Hidden)
		return false;
    return CObject::Collide(pos);
}

void CHOObject::SetHOType(const std::string& type)
{
	HOName = type;
}
		
void CHOObject::Remove()
{
	Status = eHO_Fading;

	pSprite->SetAlpha(1.0f);	
}


void CHOObject::Remove(const CPoint* pTarget, float velOff, bool isWaitTillPickup, int minScaleDim, bool isStartWait, bool isCursorTurnOff, bool isJrnPage)
{
	if( isCursorTurnOff )
	{
		Cursor::Show(false);	
		if( GFHud )
			GFHud->EnableInput(false);
	}
	IsJrnPage = isJrnPage;
	Cursor::SetMode(CA_Normal);//Turn off old handclick - when it reaches target user can start with normal cursor!!
	Application::SetSingleUpdate(true);
	pSprite->Show();//Forcily show incase hide off!!
	pSprite->Image->setTextureQuality(true);
	
	IsStartWait = isStartWait;
	if( IsStartWait )
		StartWaitTimer = START_WAIT_TIMER;
	else
		StartWaitTimer = 0.0f;
	

	MinScaleDim = minScaleDim;
	IsWaitTillPickup = isWaitTillPickup;

	if( velOff < 1.0f )
		IsSlowMotion = true;

	CPoint pos = Pos;
	Vel = TargetPos - Pos;

	float maxFlyingTime = 1.25f;//Maximum time taken by inv to fly to inv box
	float distance = Vel.Length();

	TimeFactor = (distance/1000.0f)*maxFlyingTime;

	//cut off min time to be around 0.75 secs
	if(TimeFactor<0.8f)
		TimeFactor = 0.8f;

	float randValue = rand()%200 + TimeFactor*100.0f;

	//To multiply with Logic Rate, therby slowing down the flying we take inverse value
	TimeFactor = 1/TimeFactor;

	Status = eHO_Leaving;
	TargetPos = (*pTarget);
	InitPos = Pos;

	TrailTime = 0;

	ControlPos.x = (TargetPos.x + InitPos.x)*0.5f;// mid point
	ControlPos.y = (TargetPos.y + InitPos.y)*0.5f;// mid point

	
	if( InitPos.x > TargetPos.x )
	{
		ControlPos.x -= randValue;
	}
	else
	{
		ControlPos.x += randValue;
	}
	
	if( !IsJrnPage )
		Vel.Normalize(HO_SPEED*velOff);	
	nSlowMotionFrames = 0;	

	Effect->SetPos(&pos);
	Effect->SetZ(ZOrder);
	Effect->Emit();
}

void CHOObject::InstantRemove()
{
	Status = eHO_Finished;
	Show(false);
}

void CHOObject::InitFoundHOFX()
{
	Status = eHO_FoundHOFX;
	IsFoundHOFXIncr = true;
	Effect->SetZ(24);//25+1
	Effect->SetXY(Pos.x-96,Pos.y-96);
	
	Effect->Start();	
	pSprite->SetAlpha(1.0f);
}
