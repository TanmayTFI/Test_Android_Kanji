//===========================================================
// Helpers
// Flying Insects
//===========================================================

#include "FlyingInsects.h"

FlyingInsects::FlyingInsects()
{
	for( int i = 0; i < MAX_CR_GOLDEN_FLIES ;i++ )
	{
		GoldenFlies[i].Spr = NULL;
		GoldenFlies[i].Spr = new CObject();
		GoldenFlies[i].Spr->LoadSprite("NPC\\GLOW.SPR");
		GoldenFlies[i].Spr->SetZ(100);
		GoldenFlies[i].Spr->SetBlendMode(eBlend_AlphaAdditive);
	}
}

FlyingInsects::~FlyingInsects()
{
	for( int i = 0; i < MAX_CR_GOLDEN_FLIES; i++ )
	{
		delete GoldenFlies[i].Spr;
		GoldenFlies[i].Spr = NULL;
	}
}

void FlyingInsects::InitFlies(float Left,float Top, float Right, float Bottom)
{
	for( int i = 0; i < MAX_CR_GOLDEN_FLIES ;i++ )
	{
		GoldenFlies[i].MaxLeft = Left;
		GoldenFlies[i].MaxTop = Top;
		GoldenFlies[i].MaxRight = Right;
		GoldenFlies[i].MaxBottom = Bottom;
		GoldenFlies[i].Spr->SetPos(PointSystem::CreateCPoint((GoldenFlies[i].MaxLeft+rand()%(int)(GoldenFlies[i].MaxRight-GoldenFlies[i].MaxLeft)),(GoldenFlies[i].MaxTop+rand()%(int)(GoldenFlies[i].MaxBottom-GoldenFlies[i].MaxTop)),0));
		GoldenFlies[i].VelX = (float)((rand()%2)?(rand()%3):-(rand()%3));
		GoldenFlies[i].VelY = (float)((rand()%2)?(rand()%3):-(rand()%3));	
		GoldenFlies[i].IsAlphaIncr = (rand()%2)?true:false;
		GoldenFlies[i].Alpha = 64+rand()%128;
		GoldenFlies[i].Spr->SetAlpha(GoldenFlies[i].Alpha/255.0f);
		GoldenFlies[i].IsWaiting = false;
		GoldenFlies[i].WaitTicks = 0;
	}
}

void FlyingInsects::Update()
{
	for( int i = 0; i < MAX_CR_GOLDEN_FLIES ;i++ )
	{

		if( GoldenFlies[i].IsWaiting )
		{
			GoldenFlies[i].WaitTicks--;
			if( GoldenFlies[i].WaitTicks <= 0 )
			{
				GoldenFlies[i].WaitTicks = 0;
				GoldenFlies[i].IsWaiting = false;
			}
		}
		else
		{
			
			if( GoldenFlies[i].IsAlphaIncr )
			{
				GoldenFlies[i].Alpha++;
				if( GoldenFlies[i].Alpha > 200 )
				{
					GoldenFlies[i].Alpha = 200;
					GoldenFlies[i].IsAlphaIncr = false;
				}
			}
			else
			{
				GoldenFlies[i].Alpha--;
				if( GoldenFlies[i].Alpha < 0 )
				{
					GoldenFlies[i].Alpha = 0;
					GoldenFlies[i].IsAlphaIncr = true;
					GoldenFlies[i].IsWaiting = true;
					GoldenFlies[i].WaitTicks = 10+rand()%300;
				}
			}
			GoldenFlies[i].Spr->SetAlpha(GoldenFlies[i].Alpha/255.0f);
				
			GoldenFlies[i].Spr->Pos.x += GoldenFlies[i].VelX;
			GoldenFlies[i].Spr->Pos.y += GoldenFlies[i].VelY;
			//Limit within world!!
			if( GoldenFlies[i].Spr->Pos.x > GoldenFlies[i].MaxRight )
			{
				GoldenFlies[i].Spr->Pos.x = GoldenFlies[i].MaxRight;
				GoldenFlies[i].VelX = -GoldenFlies[i].VelX;
			}
			else if( GoldenFlies[i].Spr->Pos.x < GoldenFlies[i].MaxLeft )
			{
				GoldenFlies[i].Spr->Pos.x = GoldenFlies[i].MaxLeft;
				GoldenFlies[i].VelX = -GoldenFlies[i].VelX;
			}

			if( GoldenFlies[i].Spr->Pos.y > GoldenFlies[i].MaxBottom )
			{
				GoldenFlies[i].Spr->Pos.y = GoldenFlies[i].MaxBottom;
				GoldenFlies[i].VelY = -GoldenFlies[i].VelY;
			}
			else if( GoldenFlies[i].Spr->Pos.y < GoldenFlies[i].MaxTop )
			{
				GoldenFlies[i].Spr->Pos.y = GoldenFlies[i].MaxTop;
				GoldenFlies[i].VelY = -GoldenFlies[i].VelY;
			}

			if( rand()%20 == 0 )
			{
				GoldenFlies[i].VelX = (float)((rand()%2)?(rand()%3):-(rand()%3));
				GoldenFlies[i].VelY = (float)((rand()%2)?(rand()%3):-(rand()%3));
			}
			//Stay there for sometime
			else if( rand()%100 == 0 )			
			{
				GoldenFlies[i].IsWaiting = true;
				GoldenFlies[i].WaitTicks = 10+rand()%300;
			}

		}
	}
}

