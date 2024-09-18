//===========================================================
// Framework
// Super Particle FX
// Inspired and uses many code from Starling Particle System
// Also from http://natureofcode.com/book/chapter-4-particle-systems/
// Faisal
//===========================================================

#include "SuperFX.h"
#include "TextureFactory.h"
#include "IOStream.h"
#include "TTMath.h"
#include "TTAlgorithms.h"
#include "Util.h"
#include "exMath.h"
#include "DataEnums.h"
#include "exRandom.h"
#include "Application.h"
#include "ProfileSystem.h"

SuperFX::SuperFX()
{
	EffectFile = "none";
	Texture = NULL;
	lpBatch = NULL;
	Active = false;
	IsEmitting = false;

#ifdef _PARTICLEEDITORENABLED
	isPoolChanged = false;
	newPoolSize = 0;
#endif // _PARTICLEEDITORENABLED
	
	Timer = 0.0f;
	PartPool = 0; 
	Version = 1;
	InitPos = CPoint(-1,-1,-1);

	Tx = Ty = 0.0f;
	Tw = Th = 16.0f;
	THalfw = THalfh = 8.0f;

	Variants = 1;
	VariantCols = 1;
	BlendMode = eBlend_AlphaNormal;
	HiDef = false;

	SourceType = EffectV2_RECT;

	LifeSpan = 0.5f;
	LifeSpanVar = 0;

	EmitSrcWidth = 0;
	EmitSrcHeight = 0;
	EmitSrcAngle = 0;
	
	EmitTimer = 0.025f;
	EmitPartsPer = 6;
	EmitAngle = 0;
	EmitAngleVar = 0;
	UniformEmission = 0;

	Speed = 0.0f;
	SpeedVar = 0.0f;
	
	AccelX = AccelY = 0.0f;

	StartAlpha = 1.0f;
	StartAlphaVar = 0.0f;
	EndAlpha = 0;
	EndAlphaVar = 0;
	AlphaMode = Particlev2_Linear;

	StartRot = 0.0f;
	StartRotVar = 0;
	EndRot = 0.0f;
	EndRotVar = 0;
	RotMode = Particlev2_Linear;

	R = G = B = 1.0f;
	RVar = GVar = BVar = 0.0f;
	TR = TG = TB = 1.0f;
	TRVar = TGVar = TBVar = 0.0f;

	StartSize = 1.0f;
	StartSizeVar = 0;
	EndSize = 0.0f;
	EndSizeVar = 0;
	ScaleMode = Particlev2_Linear;	

	Duration = 0.0f;
	TimeLapsed = 0.0f;

	Visible = false;//CRenderListEntity
}

SuperFX::SuperFX(const std::string& effectFile, const CPoint* pos, int zOrder)
{
	EffectFile = "none";
	Texture = NULL;
	lpBatch = NULL;
	Active = false;
	IsEmitting = false;

#ifdef _PARTICLEEDITORENABLED
	isPoolChanged = false;
	newPoolSize = 0;
#endif // _PARTICLEEDITORENABLED
	
	Timer = 0.0f;
	PartPool = 0; 
	Version = 1;
	InitPos = CPoint(-1,-1,-1);

	Tx = Ty = 0.0f;
	Tw = Th = 16.0f;
	THalfw = THalfh = 8.0f;

	Variants = 1;
	VariantCols = 1;
	BlendMode = eBlend_AlphaNormal;
	HiDef = false;

	SourceType = EffectV2_RECT;

	LifeSpan = 0.5f;
	LifeSpanVar = 0;

	EmitSrcWidth = 0;
	EmitSrcHeight = 0;
	EmitSrcAngle = 0;
	
	EmitTimer = 0.025f;
	EmitPartsPer = 6;
	EmitAngle = 0;
	EmitAngleVar = 0;
	UniformEmission = 0;

	Speed = 0.0f;
	SpeedVar = 0.0f;
	
	AccelX = AccelY = 0.0f;

	StartAlpha = 1.0f;
	StartAlphaVar = 0.0f;
	EndAlpha = 0;
	EndAlphaVar = 0;
	AlphaMode = Particlev2_Linear;

	StartRot = 0.0f;
	StartRotVar = 0;
	EndRot = 0.0f;
	EndRotVar = 0;
	RotMode = Particlev2_Linear;

	R = G = B = 1.0f;
	RVar = GVar = BVar = 0.0f;
	TR = TG = TB = 1.0f;
	TRVar = TGVar = TBVar = 0.0f;

	StartSize = 1.0f;
	StartSizeVar = 0;
	EndSize = 0.0f;
	EndSizeVar = 0;
	ScaleMode = Particlev2_Linear;	

	Duration = 0.0f;
	TimeLapsed = 0.0f;

	Visible = false;//CRenderListEntity

	LoadEffectIni(effectFile);
	SetZ(zOrder);
	SetPos(pos);
}

SuperFX::~SuperFX()
{
	StopImmediate();
	if( lpBatch != NULL )
	{
		lpBatch->freeBuffer();
		delete lpBatch; lpBatch = NULL;
	}
	int size = (int)Particles.size();
	for(int i=0; i < size; ++i)
	{
		if( Particles[i] != NULL )
		{
			delete Particles[i];
			Particles[i] = NULL;
		}
	}	
	Particles.clear();

	CTexFactory::Instance->FreeImage(&Texture);
}

void SuperFX::Render(const CPoint* cam)
{
	if( !Visible )
		return;
	else if( GFApp )
		if( GFApp->IsTurnOffParticles )
			return;

	
	if( Texture == NULL )
		return;

	SParticle *p;
	float tx1, tx2, ty2;
	
	Texture->setAlphaMode(BlendMode);
	//Texture->setAlphaOp(K_ALPHAOP_SRC_ALPHA,K_ALPHAOP_DST_ALPHA);
	
	lpBatch->beginBatch (Texture);
    int size =(int) Particles.size();  //Sac: converted Implicit to explicit

	for(int i=0; i < size; ++i)
	{
		p = Particles[i];

		if( !p->Active )
			continue;

		//Only checking linear length variants only
		//Checking if width exceeds texture width
		//move to next row Later
		tx1 = Tx + p->Variant * Tw;
		tx2 = tx1 + Tw;
		ty2 = Ty + Th;

		Texture->setBlitColor(p->R, p->G, p->B, p->Alpha);
        lpBatch->blitAlphaRectFx( tx1, Ty, tx2, ty2,
								   p->X - THalfw, p->Y - THalfh,
								   p->Rot,
								   (p->Scale),
								   p->Alpha);
	}
	lpBatch->endBatch();
	Texture->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
}

#ifdef _PARTICLEEDITORENABLED

void SuperFX::SetPoolSize(int newSize)
{
	//Clear all pool
	int size = (int)Particles.size();
	for(int i=0; i < size; ++i)
	{
		if( Particles[i] != NULL )
		{
			delete Particles[i];
			Particles[i] = NULL;
		}
	}	
	Particles.clear();

	PartPool = newPoolSize;
	Particles.reserve(PartPool);	

	for(int i=0; i < PartPool; ++i)
	{
		Particles.push_back(new SParticle());
	}

	if( lpBatch != NULL )
	{
		lpBatch->freeBuffer();
		delete lpBatch; lpBatch = NULL;
	}

	///* Create vertex buffer */
    lpBatch = KPTK::createKBatch ();
    lpBatch->allocateBuffer (PartPool);

	isPoolChanged = false;
}

#endif // _PARTICLEEDITORENABLED

void SuperFX::SetPos( const CPoint* pos )
{
	Pos = (*pos);
	if( InitPos == CPoint(-1,-1,-1) )//One time init!!
		InitPos = Pos;
}

void SuperFX::ResetPos( const CPoint* pos )
{
	Pos = (*pos);
	InitPos = Pos;
}

void SuperFX::LoadEffectIni(const std::string& effectFile)
{
	if( Particles.size() > 0 )
	{
		
		std::string SaveErrorMsg = std::string("Er0x2801- Attempting to reload Visual FX: " + effectFile).c_str();
		K_LOG(SaveErrorMsg.c_str());
		#ifdef K_WIN32
			FATAL(SaveErrorMsg.c_str()); ////Removed Wide string conversion for Windows	
		#else
			FATAL((const char*)(SaveErrorMsg.c_str()) );
		#endif
		return;
	}

	Filename = effectFile;
	//Filename to uppercase!!
    int strLen = (int)Filename.size();  //Sac: converted Implicit to explicit

	for( int i = 0; i < strLen; i++ )
		Filename[i] = toupper(Filename[i]);

	//Though the filename is getting converted to upper, it is not used after converted	
	// if( !iniReader->setIni( FileSystem::charPath(effectFile) )  )
	// so chaing the above to use this

	//Load from ini........
	KIniReader *iniReader;
	iniReader = new KIniReader ;
	if( !iniReader->setIni( FileSystem::charPath(Filename) )  )
	{
		std::string SaveErrorMsg = std::string("Er0x2802- Unable to open visual FX file: " + effectFile).c_str();
		K_LOG(SaveErrorMsg.c_str());
		#ifdef K_WIN32
			FATAL(SaveErrorMsg.c_str());
		#else
			FATAL((const char*)(SaveErrorMsg.c_str()) );
		#endif

		return;
	}
	
	EffectFile = effectFile;
	Filename = effectFile; // Sac : to retain the case of the object name as it is used as is for object creation

	std::string actualFileName;
	size_t fileNamePos = Filename.find_last_of("\\");
	if(fileNamePos != std::string::npos)
		actualFileName.assign(Filename.begin() + fileNamePos + 1, Filename.end());
	else
		actualFileName = Filename;
	actualFileName.resize( actualFileName.length() - 4 );

	SetUID(actualFileName);

	//=====================
	// Read Emitter Props SECTION
	//=====================
	Version = (int)iniReader->getValue("EMT_PROP","ver");
	

	//Read and load texture
	char lpStrBuffer[256];
	iniReader->getString("EMT_PROP","tex",lpStrBuffer,256);
	texName = lpStrBuffer;
	Texture = CTexFactory::Instance->GetImage(texName,false,true);//Hiquality SParticle effects
	

	Tx = iniReader->getFloatValue("EMT_PROP","tx");
	Ty = iniReader->getFloatValue("EMT_PROP","ty");
	Tw = iniReader->getFloatValue("EMT_PROP","pw");
	Th = iniReader->getFloatValue("EMT_PROP","ph");

	THalfw = Tw*0.5f;
	THalfh = Th*0.5f;

	Variants	= (int)iniReader->getValue("EMT_PROP","var");
	VariantCols = (int)iniReader->getValue("EMT_PROP","cols");

	BlendMode	= (EBlend)iniReader->getValue("EMT_PROP","blend");

	SourceType = (EffectSourceV2)iniReader->getValue("EMT_PROP","srctype");

	EmitSrcWidth = iniReader->getFloatValue("EMT_PROP","srcWidth");
	EmitSrcHeight = iniReader->getFloatValue("EMT_PROP","srcHeight");
	EmitTimer		= iniReader->getFloatValue("EMT_PROP","emittime");
	
	EmitPartsPer	= iniReader->getFloatValue("EMT_PROP","emitparts");
	EmitAngle	= iniReader->getFloatValue("EMT_PROP","emitAngle");
	EmitAngleVar	= iniReader->getFloatValue("EMT_PROP","emitAngleVar");
	
	UniformEmission = (int)iniReader->getValue("EMT_PROP","uniform");

	PartPool = (int)iniReader->getValue("EMT_PROP","partpool");
	Particles.reserve(PartPool);	

	for(int i=0; i < PartPool; ++i)
	{
		Particles.push_back(new SParticle());
	}

	//=====================
	// Read SParticle Props SECTION
	//====================

	//Life Span
	LifeSpan = iniReader->getFloatValue("PRT_PROP","life");
	LifeSpanVar = iniReader->getFloatValue("PRT_PROP","lifevar");

	//Velocity Magnitude
	Speed = iniReader->getFloatValue("PRT_PROP","Speed");
	SpeedVar = iniReader->getFloatValue("PRT_PROP","SpeedVar");

	//Accel
	AccelX	= iniReader->getFloatValue("PRT_PROP","AccelX");
	AccelY	= iniReader->getFloatValue("PRT_PROP","AccelY");

	//Alpha
	StartAlpha = iniReader->getFloatValue("PRT_PROP","StartAlpha");
	StartAlphaVar = iniReader->getFloatValue("PRT_PROP","StartAlphaVar");
	EndAlpha = iniReader->getFloatValue("PRT_PROP","EndAlpha");
	EndAlphaVar = iniReader->getFloatValue("PRT_PROP","EndAlphaVar");
	AlphaMode = (ParticleEffectModeV2)iniReader->getValue("PRT_PROP","AlphaMode");

	//Rot
	StartRot = iniReader->getFloatValue("PRT_PROP","StartRot");
	StartRotVar = iniReader->getFloatValue("PRT_PROP","StartRotVar");
	EndRot = iniReader->getFloatValue("PRT_PROP","EndRot");
	EndRotVar = iniReader->getFloatValue("PRT_PROP","EndRotVar");
	RotMode = (ParticleEffectModeV2)iniReader->getValue("PRT_PROP","RotMode");

	//Scale
	StartSize = iniReader->getFloatValue("PRT_PROP","StartSize");	
	StartSizeVar = iniReader->getFloatValue("PRT_PROP","StartSizeVar");
	EndSize = iniReader->getFloatValue("PRT_PROP","EndSize");
	EndSizeVar = iniReader->getFloatValue("PRT_PROP","EndSizeVar");
	ScaleMode = (ParticleEffectModeV2)iniReader->getValue("PRT_PROP","ScaleMode");

	//Tint
	R = iniReader->getFloatValue("PRT_PROP","R");
	G = iniReader->getFloatValue("PRT_PROP","G");
	B = iniReader->getFloatValue("PRT_PROP","B");

	RVar = iniReader->getFloatValue("PRT_PROP","Rvar");
	GVar = iniReader->getFloatValue("PRT_PROP","Gvar");
	BVar = iniReader->getFloatValue("PRT_PROP","Bvar");

	TR = iniReader->getFloatValue("PRT_PROP","TR");
	TG = iniReader->getFloatValue("PRT_PROP","TG");
	TB = iniReader->getFloatValue("PRT_PROP","TB");

	TRVar = iniReader->getFloatValue("PRT_PROP","TRvar");
	TGVar = iniReader->getFloatValue("PRT_PROP","TGvar");
	TBVar = iniReader->getFloatValue("PRT_PROP","TBvar");
	
	Duration = iniReader->getFloatValue("PRT_PROP","Drn");
	Drag = iniReader->getFloatValue("PRT_PROP","Drag");
	if(Drag<=0)
		Drag = 1.0f;

	EmitSrcAngle = iniReader->getFloatValue("PRT_PROP", "srcAngle");



	iniReader->getString("PRT_PROP","HiDef",lpStrBuffer,256);
	HiDef = ( !strcmp(lpStrBuffer,"true") )?true:false;
	Texture->setTextureQuality(HiDef);

	////////////////////////
	delete iniReader;

	/* Create vertex buffer */
    lpBatch = KPTK::createKBatch ();
    lpBatch->allocateBuffer (PartPool);
}

SParticle* SuperFX::Next()
{
    int size = (int)Particles.size();	 //Sac: converted Implicit to explicit

	for(int i=0; i < size; ++i)
	{
		if( Particles[i]->Active == false )
		{
			return Particles[i];
		}
	}

	return NULL;
}

// Do a full stop, emiter stops and particles
void SuperFX::StopImmediate()
{
	Visible = false;
	if( !IsEmitting && !Active )
		return;//stop if there was a "emit" or a "start(active)" call

	IsEmitting = false;
	Active = false;	
	Timer = 0.0f;
	 

    int size = (int)Particles.size();  //Sac: converted Implicit to explicit

	for(int i=0; i < size; ++i)
	{
		Particles[i]->Deactivate();
	}
}

// Let the particles live on, but stop the emitter
void SuperFX::StopEmit()
{
	Active = false;
}

void SuperFX::SetStartPos(SParticle* p, int count)
{
	
	switch(SourceType)
	{
		case EffectV2_RADIALSQR:
		case EffectV2_RECT:
			if(UniformEmission == 1)
			{
				switch(count % 4)
				{
				case 0:
					p->X = Pos.x - EmitSrcWidth*0.5f;
					p->Y = Pos.y + Elixir::Rand(-EmitSrcHeight*0.5f, EmitSrcHeight*0.5f);
					break;
				case 1:
					p->X = Pos.x + Elixir::Rand(-EmitSrcWidth*0.5f, EmitSrcWidth*0.5f);
					p->Y = Pos.y - EmitSrcHeight*0.5f;
					break;
				case 2:
					p->X = Pos.x + EmitSrcWidth*0.5f;
					p->Y = Pos.y + Elixir::Rand(-EmitSrcHeight*0.5f, EmitSrcHeight*0.5f);
					break;
				case 3:
					p->X = Pos.x + Elixir::Rand(-EmitSrcWidth*0.5f, EmitSrcWidth*0.5f);
					p->Y = Pos.y + EmitSrcHeight*0.5f;
					break;
				}
			}
			else
			{
				p->X = Pos.x + Elixir::Rand(-EmitSrcWidth*0.5f, EmitSrcWidth*0.5f);
				p->Y = Pos.y + Elixir::Rand(-EmitSrcHeight*0.5f, EmitSrcHeight*0.5f);
				if(EmitSrcAngle > 0.0F && EmitSrcAngle < 360.0F)
				{
					CPoint partPos;
					partPos.SetXY(p->X, p->Y);
					CPoint rotatedPos = Elixir::RotatePointByAngle(Pos, partPos, EmitSrcAngle);

					p->X = rotatedPos.x;
					p->Y = rotatedPos.y;
				}
			}
			break;
		case EffectV2_CIRCLE:
			if(UniformEmission == 1)
			{
				float angle = count * (360.0f / EmitPartsPer) ;
				p->X = Pos.x + Elixir::Sin(angle) * EmitSrcWidth;
				p->Y = Pos.y + Elixir::Cos(angle) * EmitSrcHeight;
			}
			else
			{
				float angle = Elixir::Rand(0.0f, 359.0f);
				p->X = Pos.x + Elixir::Sin(angle) * Elixir::Rand(1.0f, EmitSrcWidth);
				p->Y = Pos.y + Elixir::Cos(angle) * Elixir::Rand(1.0f, EmitSrcHeight);
			}
			break;
		case EffectV2_RADIALOUT:
		case EffectV2_RADIALIN:
		case EffectV2_RADIAL:
			if(UniformEmission == 1)
			{
				float angle = count * (360.0f / EmitPartsPer) ;
				p->X = Pos.x + Elixir::Sin(angle) * EmitSrcWidth;
				p->Y = Pos.y + Elixir::Cos(angle) * EmitSrcHeight;
			}
			else
			{
				float angle = Elixir::Rand(0.0f, 359.0f);
				p->X = Pos.x + Elixir::Sin(angle) * Elixir::Rand(1.0f, EmitSrcWidth);
				p->Y = Pos.y + Elixir::Cos(angle) * Elixir::Rand(1.0f, EmitSrcHeight);
			}
			break;

	}
}

void SuperFX::SetVelocity(SParticle* p, int count)
{
	// Old Code
	//if( angleRange >= 1.0f )
	//{
	//	if( UniformEmission == 1)
	//	{
	//		preangle = TT::PosWrap(EmitAngle + count * (angleRange / EmitPartsPer) , 360.0f);
	//		angle = preangle;
	//	}
	//	else
	//	{
	//		preangle = EmitAngle + rand()%int(angleRange);//TT::PosWrap(EmitAngle + rand()%int(angleRange), 360.0f);
	//		angle = preangle;
	//	}
	//}

	if(SourceType == EffectV2_RADIAL || SourceType == EffectV2_RADIALSQR)
	{
		//Radially rotating or Inward
		//angle is based on position relative to center
		float angle = atan2f(p->Y-Pos.y,p->X-Pos.x);
		angle = Elixir::ToDegrees(angle);

		float speed = Speed + SpeedVar*Elixir::Rand(0.0f, 1.0f);

		p->VelX = Elixir::Sin(-angle) * speed;
		p->VelY = Elixir::Cos(angle) * speed;
	}
	else if(SourceType == EffectV2_RADIALOUT)
	{
		//Radially Outward or Inward
		//angle is based on position relative to center
		float angle = atan2f(p->Y-Pos.y,p->X-Pos.x);
		angle = Elixir::ToDegrees(angle);

		float speed = Speed + SpeedVar*Elixir::Rand(0.0f, 1.0f);

		p->VelX = Elixir::Sin(angle+90) * speed;
		p->VelY = Elixir::Cos(angle-90) * speed;
	}
	else if(SourceType == EffectV2_RADIALIN)
	{
		//Radially Outward or Inward
		//angle is based on position relative to center
		float angle = atan2f(p->Y-Pos.y,p->X-Pos.x);
		angle = Elixir::ToDegrees(angle);

		float speed = Speed + SpeedVar*Elixir::Rand(0.0f, 1.0f);

		p->VelX = -Elixir::Sin(angle+90) * speed;
		p->VelY = -Elixir::Cos(angle-90) * speed;
	}
	else
	{
		//Normal Cases
		float angleRange = EmitAngleVar*Elixir::Rand(-1.0f, 1.0f);
		float angle = EmitAngle + Elixir::Rand( 0.0f, angleRange);

		float speed = Speed + SpeedVar*Elixir::Rand(0.0f, 1.0f);

		p->VelX = Elixir::Sin(angle) * speed;
		p->VelY = Elixir::Cos(angle) * speed;
	}
}

void SuperFX::SetVariant(SParticle* p)
{
	if( Variants <= 1 )
		p->Variant = 0;
	else
	{
		p->Variant = rand()%Variants;
	}
}

void SuperFX::UpdateParticle(SParticle* p, float ds)
{
	//Update velocity, acceleration, distance travelled
	p->VelX		+= AccelX * ds;
	p->VelY		+= AccelY * ds;
	p->X		+= p->VelX * ds;
	p->Y		+= p->VelY * ds;

	if(Drag<1.0f)
	{
		p->VelX *= Drag;
		p->VelY *= Drag;
	}

	/*if(true)
	{
		p->VelX = 40.0f*Elixir::Sin(p->currentTime/p->totalTime*360.0f);
		p->VelY = 20.0f*Elixir::Cos(180+p->currentTime/p->totalTime*360.0f);
	}*/

	p->currentTime += ds;

	//Handle Alpha
	switch (AlphaMode)
	{
	case Particlev2_Linear:		p->Alpha += p->dAlpha*ds;	break;
	case Particlev2_SineWave:	
		p->Alpha = TT::Clamp(0.0f,2.0f*Elixir::Sin(p->currentTime/p->totalTime*180.0f)*p->dAlpha,1.0f);
								break;//p->Alpha = Elixir::Sin(p->currentTime/p->totalTime*180.0f)*p->dAlpha*ds;	break;
	case Particlev2_Wave:		break;
	default:
		break;
	}

	//Handle Rotation
	switch (RotMode)
	{
	case Particlev2_Linear:		p->Rot += p->dRot * ds;	break;
	case Particlev2_SineWave:	break;
	case Particlev2_Wave:		break;
	default:
		break;
	}

	//Handle scale
	switch (ScaleMode)
	{
	case Particlev2_Linear:		p->Scale += p->dScale * ds;	break;
	case Particlev2_SineWave:	p->Scale = TT::Clamp(0.0f,1.0f*Elixir::Sin(p->currentTime/p->totalTime*180.0f)*p->dScale,p->dScale);	break;
	case Particlev2_Wave:		break;
	default:
		break;
	}

	//Handle Tint
	if( ! CUtil::Equal( p->R, TR ) )
		p->R += p->dR * ds;
	if( ! CUtil::Equal( p->G, TG ) )
		p->G += p->dG * ds;
	if( ! CUtil::Equal( p->B, TB ) )
		p->B += p->dB * ds;

	//Check for end of LifeSpan
	if(p->currentTime >= p->totalTime)
	{
		p->Deactivate();
	}
}

void SuperFX::Update(float ds)
{
	if( !Visible )
		return;
	
	if( Active  )
	{		
		if(delayTimer>0)
		{
			delayTimer -= ds;
			return;
		}

		if( TimeLapsed < Duration )
		{	
			TimeLapsed += ds;
			if( TimeLapsed >= Duration )
			{
				StopEmit();	
				return;
			}
		}

		Timer += ds;
		if( Timer >= EmitTimer )
		{
			Timer = 0.0f;
			Emit();
		}
	}


	

    int size = (int) Particles.size();	 //Sac: converted Implicit to explicit

	for(int i=0; i < size; ++i)
		if( Particles[i]->Active )			
			UpdateParticle( Particles[i], ds );

	#ifdef _PARTICLEEDITORENABLED
	if(isPoolChanged)
	{
		SetPoolSize(newPoolSize);
	}
	#endif // _PARTICLEEDITORENABLED
}

bool SuperFX::IsParticlesActive()
{
    int size = (int)Particles.size();	 //Sac: converted Implicit to explicit

	for(int i=0; i < size; i++)
		if( Particles[i]->Active )	
			return true;

	return false;		
}

void SuperFX::Start(float delay,CPoint *StartPos)
{
	if (StartPos!=NULL)
		SetPos(StartPos); 
	
	Visible = true;
	Active = true;

	TimeLapsed = 0.0f;
	Timer = 0.0f;

	delayTimer = delay;

	if( !(delayTimer>0) )
	{
		Emit();
	}
}

void SuperFX::AdvanceByTime(float timeSecs)
{
	//advance it quickly.
	//Update(timeSecs);
	//or advance each particle by time diff at 30 fps
	int numDiff = (int)(timeSecs/0.034f);
	for (int i = 0; i < numDiff; i++)
	{
		Update(0.034f);
	}
}

void SuperFX::Emit()
{
	IsEmitting = true;
	Visible = true;//CRenderListEntity
	SParticle *p;

	for(int i=0; i < EmitPartsPer; ++i)
	{
		p = Next();

		if( p == NULL )
			return;

		SetStartPos( p, i );
		SetVelocity( p, i );
		SetVariant( p );

		p->totalTime = LifeSpan + LifeSpanVar*Elixir::Rand(0.0f, 1.0f);

		
		float startR,startG,startB;
		float endR,endG,endB;

		startR = p->R		= R+ RVar*Elixir::Rand(0.0f, 1.0f);
		startG = p->G		= G+ GVar*Elixir::Rand(0.0f, 1.0f);
		startB = p->B		= B+ BVar*Elixir::Rand(0.0f, 1.0f);

		endR = TR+ TRVar*Elixir::Rand(0.0f, 1.0f);
		endG = TG+ TGVar*Elixir::Rand(0.0f, 1.0f);
		endB = TB+ TBVar*Elixir::Rand(0.0f, 1.0f);
		
		p->dR		= (endR - startR)/p->totalTime;
		p->dG		= (endG - startG)/p->totalTime;
		p->dB		= (endB - startB)/p->totalTime;
		
		float startSize = StartSize + StartSizeVar*Elixir::Rand(0.0f, 1.0f);
		float endSize = EndSize + EndSizeVar*Elixir::Rand(0.0f, 1.0f);

		p->Scale	= StartSize;
		p->dScale	= (endSize - startSize)/p->totalTime;

		float startAlpha = StartAlpha + StartAlphaVar*Elixir::Rand(0.0f, 1.0f);
		float endAlpha = EndAlpha + EndAlphaVar*Elixir::Rand(0.0f, 1.0f);

		p->Alpha		= startAlpha;
		p->dAlpha		= (endAlpha-startAlpha)/p->totalTime;

		float initRot = StartRot + StartRotVar*Elixir::Rand(0.0f, 1.0f);
		float endRot = EndRot + EndRotVar*Elixir::Rand(0.0f, 1.0f);

		p->Rot		= initRot;
		p->dRot		= (endRot-StartRot)/p->totalTime;

		p->Activate();
	}	
}
