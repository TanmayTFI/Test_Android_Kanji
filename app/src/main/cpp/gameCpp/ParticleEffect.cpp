//===========================================================
// Framework
// Particle Effect
//===========================================================

#include "ParticleEffect.h"
#include "TextureFactory.h"
#include "IOStream.h"
#include "TTMath.h"
#include "TTAlgorithms.h"
#include "Util.h"
#include "exMath.h"
#include "DataEnums.h"
#include "exRandom.h"
#include "GameFlow.h"
#include "Application.h"
#include "ProfileSystem.h"

#define SPARKLE_VFX0 "PRT\\sparkle.INI" //4 sec

ParticleEffect::ParticleEffect()
{
	EffectFile = "none";
	Texture = NULL;
	lpBatch = NULL;
	Active = false;
	IsEmitting = false;
	ScaleFactor = 1.0f;
	Timer = 0.0f;
	PartPool = 0; 
	Version = 1;
	InitPos = CPoint(-1,-1,-1);
	IsRandomEmission = false;
	IsSparkleFX = false;
	FadeOutTimer = 0.0f;
	FadeOutSpeed = 0.0f;
	IsFadingOut = false;

	Tx = Ty = 0.0f;
	Tw = Th = 16.0f;

	Variants = 1;
	BlendMode = eBlend_AlphaNormal;
	HiDef = false;

	SourceType = Effect_Point;
	EmitSourceValue = 1.0f;
	EmitTimer = 0.025f;
	EmitPartsPer = 6;
	MinEmitAngle = 0;
	MaxEmitAngle = 360;
	UniformEmission = 0;

	DX = 0.0f;
	DY = 0.0f;
	DDX = DDY = 0.0f;

	IA = 1.0f;
	DA = 0.0f;
	EA = Particle_None;

	IR = 0.0f;
	DR = 0.0f;
	ER = Particle_None;

	R = G = B = 1.0f;
	TR = TG = TB = 1.0f;
	DRc = DBc = DGc = 0.0f;

	IS = 1.0f;
	DS = 0.0f;
	ES = Particle_None;

	EndMode = Particle_Perpetual;
	EndValue = 0.0f;

	OffsetX = 0;
	OffsetY = 0;
	Randomizer = 0;
	nSteps = 0;
	Duration = 0.0f;
	TimeLapsed = 0.0f;
	MovementFactorX = 0.0f;
	MovementFactorY = 0.0f;
	Visible = false;//CRenderListEntity
}

ParticleEffect::ParticleEffect(const std::string& effectFile)
{	
	VarNUE = 0;//small non uniform emission
	EffectFile = "none";
	Texture = NULL;
	Active = false;
	Timer = 0.0f;

	Tx = Ty = 0.0f;
	Tw = Th = 16.0f;

	Variants = 1;
	BlendMode = eBlend_AlphaNormal;
	HiDef = false;

	SourceType = Effect_Point;
	EmitSourceValue = 1.0f;
	EmitTimer = 0.025f;
	EmitPartsPer = 6;
	MinEmitAngle = 0;
	MaxEmitAngle = 360;
	UniformEmission = 0;

	DX = 0.0f;
	DY = 0.0f;
	DDX = DDY = 0.0f;

	IA = 1.0f;
	DA = 0.0f;
	EA = Particle_None;

	IR = 0.0f;
	DR = 0.0f;
	ER = Particle_None;

	R = G = B = 1.0f;
	TR = TG = TB = 1.0f;
	DRc = DBc = DGc = 0.0f;

	IS = 1.0f;
	DS = 0.0f;
	ES = Particle_None;

	EndMode = Particle_Perpetual;
	EndValue = 0.0f;

	OffsetX = 0;
	OffsetY = 0;
	Randomizer = 0;
	nSteps = 0;
	Duration = 0.0f;
	TimeLapsed = 0.0f;
	MovementFactorX = 0.0f;
	MovementFactorY = 0.0f;
	
	if( effectFile == SPARKLE_VFX0 )
		LoadSparkleEffectIni();
	else
		LoadEffectIni(effectFile);
}


ParticleEffect::~ParticleEffect()
{
	Stop();
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

void ParticleEffect::Render(const CPoint* cam)
{
	if( !Visible )
		return;
	else if( GFApp )
		if( GFApp->IsTurnOffParticles )
			return;

	
	if( Texture == NULL )
		return;

	Particle *p;
	float tx1, tx2, ty2;

	
	Texture->setAlphaMode(BlendMode);
	
	lpBatch->beginBatch (Texture);
	int size = (int) Particles.size();	 //Sac: converted Implicit to explicit
	float ParAlpha;
	for(int i=0; i < size; ++i)
	{
		p = Particles[i];

		if( !p->Active )
			continue;

		tx1 = Tx + p->Variant * Tw;
		tx2 = tx1 + Tw;
		ty2 = Ty + Th;

		if( IsFadingOut )
			ParAlpha = p->A*FadeOutTimer;
		else
			ParAlpha = p->A;

		Texture->setBlitColor(p->R, p->G, p->B, p->A);
        if( IsSparkleFX )
            lpBatch->blitAlphaRectFx( tx1, Ty, tx2, ty2,
                                     p->X, p->Y, 
                                     p->Rot,
                                     (p->S*ScaleFactor*0.75f), 
                                     ParAlpha);
        else
            lpBatch->blitAlphaRectFx( tx1, Ty, tx2, ty2,
								   p->X, p->Y, 
								   p->Rot,
								   (p->S*ScaleFactor), 
								   ParAlpha);
	}
	lpBatch->endBatch();
	Texture->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ParticleEffect::SetPos( const CPoint* pos )
{
	Pos = (*pos);
	if( InitPos == CPoint(-1,-1,-1) )//One time init!!
		InitPos = Pos;
}

void ParticleEffect::ResetPos( const CPoint* pos )
{
	Pos = (*pos);
	InitPos = Pos;
}

void ParticleEffect::ResetPosToGBHP( bool IsInvBoxOpen )
{
	if( IsInvBoxOpen )
	{
		Pos.SetXYZ(512,666,0);
		InitPos = Pos;
	}
	else
	{
		Pos.SetXYZ(512,666+65,0);
		InitPos = Pos;
	}
}

void ParticleEffect::LoadEffectFile(const std::string& effectFile, int varNUE)
{

	if( Particles.size() > 0 )
	{
		std::string TmpStr = std::string("Er0x1702- Attempting to reload VFX: " + effectFile).c_str();
		#ifdef K_WIN32
			FATAL(TmpStr.c_str());//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
		#else
			FATAL( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
		#endif		
		return;
	}

	Filename = effectFile;
	//Filename to uppercase!!
	int strLen = (int) Filename.size();  //Sac: converted Implicit to explicit
	for( int i = 0; i < strLen; i++ )
		Filename[i] = toupper(Filename[i]);

	CIOStream *stream = new CIOStream();
	if( !stream->Open(FileSystem::strPath(Filename), File_Read) )//to Pak_Read
	{
		delete stream;
		std::string TmpStr = std::string("Er0x1703- Unable to open visual FX file: " + effectFile).c_str();		
		#ifdef K_WIN32
			FATAL(TmpStr.c_str());//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
		#else
			FATAL( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
		#endif		
		return;
	}

	VarNUE = varNUE;//small non uniform emission
	
	EffectFile = effectFile;

	//=====================
	// Read Emitter Props
	//=====================
	//Discard intro string
	std::string texName;
	stream->ReadString();

	Version = stream->ReadIntSplit('=');

	//Read and load texture
	texName = stream->ReadStringSplit('=');
	Texture = CTexFactory::Instance->GetImage(texName,false,true);//Hiquality particle effects

	Tx = stream->ReadFloatSplit('=');
	Ty = stream->ReadFloatSplit('=');
	Tw = stream->ReadFloatSplit('=');
	Th = stream->ReadFloatSplit('=');

	Variants	= stream->ReadIntSplit('=');
	BlendMode	= (EBlend)stream->ReadIntSplit('=');

	SourceType = (EffectEmissionSource)stream->ReadIntSplit('=');

	EmitSourceValue = stream->ReadFloatSplit('=');
	EmitTimer		= stream->ReadFloatSplit('=');

	if( effectFile == SPARKLE_VFX0 )
	{
		IsSparkleFX = true;
		IsRandomEmission = true; 		
	}
	else 
	{
		IsSparkleFX = true;
		IsRandomEmission = true; 
	}
	
	
	EmitPartsPer	= stream->ReadFloatSplit('=');
	MinEmitAngle	= stream->ReadFloatSplit('=');
	MaxEmitAngle	= stream->ReadFloatSplit('=');
	
	UniformEmission = stream->ReadIntSplit('=');

	PartPool = stream->ReadIntSplit('=');
	Particles.reserve(PartPool);	

	for(int i=0; i < PartPool; ++i)
	{
		Particles.push_back(new Particle());
	}

	//=====================
	// Read Particle Props
	//=====================
	//Discard intro string
	stream->ReadString();

	//Velocity Magnitude
	DX = stream->ReadFloatSplit('=');
	DY = DX;
	//Accel
	DDX	= stream->ReadFloatSplit('=');
	DDY	= stream->ReadFloatSplit('=');
	//Alpha
	IA = stream->ReadFloatSplit('=');
	DA = stream->ReadFloatSplit('=');
	EA = (ParticleEffectMode)stream->ReadIntSplit('=');
	//Rot
	IR = stream->ReadFloatSplit('=');
	DR = stream->ReadFloatSplit('=');
	ER = (ParticleEffectMode)stream->ReadIntSplit('=');
	//Tint
	R = stream->ReadFloatSplit('=');
	G = stream->ReadFloatSplit('=');
	B = stream->ReadFloatSplit('=');
	TR = stream->ReadFloatSplit('=');
	TG = stream->ReadFloatSplit('=');
	TB = stream->ReadFloatSplit('=');
	float dc = stream->ReadFloatSplit('=');
	//Scale
	IS = stream->ReadFloatSplit('=');	
	DS = stream->ReadFloatSplit('=');
	ES = (ParticleEffectMode)stream->ReadIntSplit('=');
	//End 
	EndMode	= (ParticleEndMode)stream->ReadIntSplit('=');
	EndValue = stream->ReadFloatSplit('=');

	if(Version >= 2)
	{
		//Discard VERSION 2 string
		stream->ReadString();
		DY = stream->ReadFloatSplit('=');
	}

	if(Version >= 3)
	{
		//Discard VERSION 3 string
		stream->ReadString();
		HiDef = stream->ReadBoolSplit('=');
	}
	Texture->setTextureQuality(HiDef);

	if(Version >= 4)
	{
		//Discard VERSION 4 string
		stream->ReadString();
		OffsetX	  = stream->ReadIntSplit('=');
		OffsetY	  = stream->ReadIntSplit('=');
		Randomizer= stream->ReadIntSplit('=');
		nSteps	  = stream->ReadIntSplit('=');
		Duration = stream->ReadFloatSplit('=');		
	}

	stream->Close();
	//Destroy the stream
	delete stream;

	CalcDeltaTint(dc);

	/* Create vertex buffer */
    lpBatch = KPTK::createKBatch ();
    lpBatch->allocateBuffer (PartPool);
}

void ParticleEffect::LoadEffectIni(const std::string& effectFile, int varNUE)
{
	if( Particles.size() > 0 )
	{
		std::string TmpStr = std::string("Er0x1704- Attempting to reload Visual FX: " + effectFile).c_str();		
		#ifdef K_WIN32
			FATAL(TmpStr.c_str());//WIDESTR WARNINGS		////Removed Wide string conversion for Windows	
		#else
			FATAL( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
		#endif		
		return;
	}

	Filename = effectFile;
	//Filename to uppercase!!
	int strLen = (int) Filename.size();  //Sac: converted Implicit to explicit
	for( int i = 0; i < strLen; i++ )
		Filename[i] = toupper(Filename[i]);


	//Load from ini........
	KIniReader *iniReader;
	iniReader = new KIniReader ;
	if( !iniReader->setIni( FileSystem::charPath(Filename) )  )
	{
		std::string TmpStr = std::string("Er0x1705- Unable to open visual FX file: " + effectFile).c_str();		
		#ifdef K_WIN32
			FATAL(TmpStr.c_str());//WIDESTR WARNINGS		////Removed Wide string conversion for Windows	
		#else
			FATAL( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
		#endif		
		return;
	}
	
	

	VarNUE = varNUE;//small non uniform emission
	
	EffectFile = effectFile;

	//=====================
	// Read Emitter Props SECTION
	//=====================
	Version = (int)iniReader->getValue("EMT_PROP","ver");
	

	//Read and load texture
	char lpStrBuffer[256];
	iniReader->getString("EMT_PROP","tex",lpStrBuffer,256);
	std::string texName = lpStrBuffer;
	Texture = CTexFactory::Instance->GetImage(texName,false,true);//Hiquality particle effects
	

	Tx = iniReader->getFloatValue("EMT_PROP","tx");
	Ty = iniReader->getFloatValue("EMT_PROP","ty");
	Tw = iniReader->getFloatValue("EMT_PROP","pw");
	Th = iniReader->getFloatValue("EMT_PROP","ph");

	Variants	= (int)iniReader->getValue("EMT_PROP","var");
	BlendMode	= (EBlend)iniReader->getValue("EMT_PROP","blend");

	SourceType = (EffectEmissionSource)iniReader->getValue("EMT_PROP","srctype");

	EmitSourceValue = iniReader->getFloatValue("EMT_PROP","srcval");
	EmitTimer		= iniReader->getFloatValue("EMT_PROP","emittime");

	if( effectFile == SPARKLE_VFX0 )
	{		
		IsSparkleFX = true;
		IsRandomEmission = true; 
	}
	else 
	{
		IsSparkleFX = true;
		IsRandomEmission = true; 
	}
	
	
	
	EmitPartsPer	= iniReader->getFloatValue("EMT_PROP","emitparts");
	MinEmitAngle	= iniReader->getFloatValue("EMT_PROP","emitminangle");
	MaxEmitAngle	= iniReader->getFloatValue("EMT_PROP","emitmaxangle");
	
	UniformEmission = (int)iniReader->getValue("EMT_PROP","uniform");

	PartPool = (int)iniReader->getValue("EMT_PROP","partpool");
	Particles.reserve(PartPool);	

	for(int i=0; i < PartPool; ++i)
	{
		Particles.push_back(new Particle());
	}

	//=====================
	// Read Particle Props SECTION
	//=====================

	//Velocity Magnitude
	DX = iniReader->getFloatValue("PRT_PROP","DXY");
	DY = DX;
	//Accel
	DDX	= iniReader->getFloatValue("PRT_PROP","DDX");
	DDY	= iniReader->getFloatValue("PRT_PROP","DDY");
	//Alpha
	IA = iniReader->getFloatValue("PRT_PROP","IA");
	DA = iniReader->getFloatValue("PRT_PROP","DA");
	EA = (ParticleEffectMode)iniReader->getValue("PRT_PROP","EA");
	//Rot
	IR = iniReader->getFloatValue("PRT_PROP","IROT");
	DR = iniReader->getFloatValue("PRT_PROP","DROT");
	ER = (ParticleEffectMode)iniReader->getValue("PRT_PROP","EROT");
	//Tint
	R = iniReader->getFloatValue("PRT_PROP","R");
	G = iniReader->getFloatValue("PRT_PROP","G");
	B = iniReader->getFloatValue("PRT_PROP","B");
	TR = iniReader->getFloatValue("PRT_PROP","TR");
	TG = iniReader->getFloatValue("PRT_PROP","TG");
	TB = iniReader->getFloatValue("PRT_PROP","TB");
	float dc = iniReader->getFloatValue("PRT_PROP","DC");
	//Scale
	IS = iniReader->getFloatValue("PRT_PROP","IS");	
	DS = iniReader->getFloatValue("PRT_PROP","DS");
	ES = (ParticleEffectMode)iniReader->getValue("PRT_PROP","ES");
	//End 
	EndMode	= (ParticleEndMode)iniReader->getValue("PRT_PROP","End");
	EndValue = iniReader->getFloatValue("PRT_PROP","EndVal");

	if(Version >= 2)
	{
		//VERSION 2 SECTION
		DY = iniReader->getFloatValue("VER2","DY");
	}

	if(Version >= 3)
	{
		//VERSION 3 SECTION
		iniReader->getString("VER3","HiDef",lpStrBuffer,256);
		HiDef = ( !strcmp(lpStrBuffer,"true") )?true:false;
	}
	Texture->setTextureQuality(HiDef);

	if(Version >= 4)
	{
		//VERSION 4 SECTION
		OffsetX	  = (int)iniReader->getValue("VER4","Offx");
		OffsetY	  = (int)iniReader->getValue("VER4","Offy");
		Randomizer= (int)iniReader->getValue("VER4","Rnd");
		nSteps	  = (int)iniReader->getValue("VER4","nStps");
		Duration = iniReader->getFloatValue("VER4","Drn");		
	}

	////////////////////////
	delete iniReader;

	CalcDeltaTint(dc);

	/* Create vertex buffer */
    lpBatch = KPTK::createKBatch ();
    lpBatch->allocateBuffer (PartPool);
}

void ParticleEffect::CalcDeltaTint(float dc)
{
	CPoint tint;

	if( dc <= 0.0f )
		return;

	tint.x = TR - R;
	tint.y = TG - G;
	tint.z = TB - B;

	if(tint.IsZero())
		return;

	tint.Normalize(dc);

	DRc = tint.x;
	DGc = tint.y;
	DBc = tint.y;
}

Particle* ParticleEffect::Next()
{
	int size =(int) Particles.size(); //Sac: converted Implicit to explicit
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
void ParticleEffect::Stop()
{
	Visible = false;
	if( !IsEmitting && !Active )
		return;//stop if there was a "emit" or a "start(active)" call

	IsEmitting = false;
	Active = false;	
	Timer = 0.0f;
	 

	int size =(int) Particles.size();  //Sac: converted Implicit to explicit
	for(int i=0; i < size; ++i)
	{
		Particles[i]->Deactivate();
	}
}

void ParticleEffect::FadeOut(float fadeOutSpeed)
{
	IsFadingOut = true;
	FadeOutSpeed = fadeOutSpeed;
	FadeOutTimer = 1.0f;	
}

// Let the particles live on, but stop the emitter
void ParticleEffect::StopEmit()
{
	Active = false;
}

void ParticleEffect::Deactivate()
{
	Active = false;
}



void ParticleEffect::SetStartPos(Particle* p, int count)
{
	
	switch(SourceType)
	{
		case Effect_Point:
			p->X = Pos.x;
			p->Y = Pos.y;
			break;
		case Effect_Circle:

			if(UniformEmission == 1)
			{
				float angle = /*CMath::ToRadians( */count * (360.0f / EmitPartsPer) /*)*/;
				p->X = -Tw/2 + Pos.x + Elixir::Sin(angle)/*sin(angle)*/ * EmitSourceValue;
				p->Y = -Th/2 + Pos.y + Elixir::Cos(angle)/*cos(angle)*/ * EmitSourceValue;
			}
			else
			{
				float angle = Elixir::Rand(0.0f, 359.0f);
				p->X = Pos.x + Elixir::Sin(angle)/*sin(angle)*/ * Elixir::Rand(1.0f, EmitSourceValue);
				p->Y = Pos.y + Elixir::Cos(angle)/*cos(angle)*/ * Elixir::Rand(1.0f, EmitSourceValue);
			}
			
			break;
		case Effect_Horz:
			p->X = Pos.x + (EmitSourceValue/2 - rand()%int(EmitSourceValue));
			p->Y = Pos.y;
			break;
		case Effect_Vert:
			p->X = Pos.x;
			p->Y = Pos.y + (EmitSourceValue/2 - rand()%int(EmitSourceValue));
			break;
	}
}

void ParticleEffect::SetVelocity(Particle* p, int count)
{
	float angleRange = TT::PosWrap(MaxEmitAngle - MinEmitAngle, 360.0f);
	float angle = MinEmitAngle;
	float preangle;

	if( angleRange >= 2.0f )
	{
		if( UniformEmission == 1)
		{
			preangle = TT::PosWrap(MinEmitAngle + count * (angleRange / EmitPartsPer) , 360.0f);
			angle = preangle;
		}
		else
		{
			preangle = TT::PosWrap(MinEmitAngle + rand()%int(angleRange) , 360.0f);
			angle = preangle;
		}
	}

	p->DX = Elixir::Sin(angle) * DX;
	p->DY = Elixir::Cos(angle) * DY;
}

void ParticleEffect::SetVariant(Particle* p)
{
	if( Variants <= 1 )
		p->Variant = 0;
	else
	{
		p->Variant = rand()%Variants;
	}
}

void ParticleEffect::UpdateParticle(Particle* p, float ds)
{
	//Update velocity, acceleration, distance travelled
	p->DX		+= DDX * ds;
	p->DY		+= DDY * ds;
	p->X		+= p->DX * ds;
	p->Y		+= p->DY * ds;
	p->DistXY	+= (float)sqrt( (p->DX * ds)*(p->DX * ds) + (p->DY * ds)*(p->DY * ds));

	//Handle Alpha
	p->A += p->AS * DA * ds;
	if( EA == Particle_LinearBound )
	{
		p->A = TT::Clamp(0.0f, p->A, 1.0f);
	}
	else if( EA == Particle_LinearWrap )
	{
		p->A = TT::PosWrap(p->A, 1.0f);
	}
	else if( EA == Particle_Wave )
	{
		if( p->A >= 1.0f )
		{
			p->A = 1.0f;
			p->AS *= -1;
		}
		else if( p->A <= 0.0f )
		{
			p->A = 0.0f;
			p->AS *= -1;
		}
	}

	//Handle Rotation
	p->Rot += p->RS * DR * ds;
	if( ER == Particle_LinearBound )
	{
		p->Rot = TT::Clamp(0.0f, p->Rot, 360.0f);
	}
	else if( ER == Particle_LinearWrap )
	{
		p->Rot = TT::PosWrap(p->Rot, 360.0f);
	}
	else if( ER == Particle_Wave )
	{
		if( p->Rot >= 360.0f )
		{
			p->Rot = 360.0f;
			p->RS *= -1;
		}
		else if( p->Rot <= 0.0f )
		{
			p->Rot = 0.0f;
			p->RS *= -1;
		}
	}

	//Handle scale
	p->S += p->SS * DS * ds;
	if( ES == Particle_LinearBound )
	{
		if( p->S < 0.0f )
			p->S = 0.0f;
		//p->S = TT::Clamp(0.0f, p->S, 1.0f);
	}
	else if( ES == Particle_LinearWrap )
	{
		p->S = TT::PosWrap(p->S, 1.0f);
	}
	else if( ES == Particle_Wave )
	{
		if( p->S >= 1.0f )
		{
			p->S = 1.0f;
			p->SS *= -1;
		}
		else if( p->S <= 0.0f )
		{
			p->S = 0.0f;
			p->SS *= -1;
		}
	}

	//Handle Tint
	if( ! CUtil::Equal( p->R, TR ) )
		p->R += DRc * ds;
	if( ! CUtil::Equal( p->G, TG ) )
		p->G += DGc * ds;
	if( ! CUtil::Equal( p->B, TB ) )
		p->B += DBc * ds;

	//Check for end mode
	switch( EndMode )
	{
		case Particle_Perpetual:
			break;
		case Particle_Alpha:
			if( CUtil::Equal( p->A, EndValue ) )
			{
				p->Deactivate();
			}
			break;
		case Particle_Rot:
			if( CUtil::Equal( p->Rot, EndValue ) )
			{
				p->Deactivate();
			}
			break;
		case Particle_Color:
			if( CUtil::Equal( p->R, TR ) &&
				CUtil::Equal( p->G, TG ) &&
				CUtil::Equal( p->B, TB ) )
			{
				p->Deactivate();
			}
			break;
		case Particle_Time:
			#ifdef K_WIN32
				WARN("Er0x1701- Particle delimiter based on time not implemented!");////Removed Wide string conversion for Windows	
			#else
				WARN((const char*)"Er0x1701- Particle delimiter based on time not implemented!");
			#endif

			break;
		case Particle_Distance:
			if( p->DistXY >= EndValue )
			{
				p->Deactivate();
			}
			break;
		case Particle_Scale:
			if( (CUtil::Equal( 0.0f, EndValue ) &&
				 CUtil::Equal( p->S, 0.0f )) ||
				(p->S >= EndValue) )
			{
				p->Deactivate();
			}
			break;
	}
}

void ParticleEffect::Update(float ds)
{
	if( IsFadingOut )
	{
		FadeOutTimer -= FadeOutSpeed* Control::LogicRate;
		if( FadeOutTimer <= 0.0f )
		{			
			FadeOutTimer = 0.0f;
			FadeOutSpeed = 0.0f;
			IsFadingOut = false;
			Stop();//Time to stop effects
			return;
		}		
	}

	if( !Visible )
		return;

	
	if( Active  )
	{		

		if( TimeLapsed < Duration )//vERSION 4
		{	
			TimeLapsed += ds;
			if( TimeLapsed >= Duration )
			{			
				Stop();	
				return;
			}
		}

		Timer += ds;
		if( Timer >= EmitTimer )
		{
			Timer = 0.0f;						

			if( nSteps ) //Updation of position for Ver4
			{				
				CPoint CurPos = Pos;
				CurPos.x += MovementFactorX * ds;
				CurPos.y += MovementFactorY * ds;
				SetPos(&CurPos);
			}
			//Randomize effect start location!!
			else if( IsRandomEmission )
				InitRandomEmission();			
			
			//They need to be shown over all popups
			if( !GFInstance->IsActivePopup )
				Emit();
		}				
	}

	int size = (int) Particles.size();  //Sac: converted Implicit to explicit
	for(int i=0; i < size; ++i)
		if( Particles[i]->Active )			
			UpdateParticle( Particles[i], ds );
		
	
}

bool ParticleEffect::IsParticlesActive()
{
	int size = (int)Particles.size();  //Sac: converted Implicit to explicit
	for(int i=0; i < size; i++)
		if( Particles[i]->Active )	
			return true;

	return false;		
}



void ParticleEffect::InitRandomEmission()
{
	if( InitPos.x == -1 && InitPos.y == -1 && InitPos.z == -1 )//One time init!!
		return;

	CPoint CurPos = CPoint(InitPos.x-32,InitPos.y-32,0);
	
	
	if( VarNUE )
	{
		CurPos.x += -1*VarNUE+rand()%(2*VarNUE);
		CurPos.y += -1*VarNUE+rand()%(2*VarNUE);
	}
	
	
	SetPos(&CurPos);
}




void ParticleEffect::StartVFX4( CPoint StartPos )
{	
	Active = true;
	Timer = 0.0f;

	TimeLapsed = 0.0f;
	CPoint EndPos;
	if( Randomizer )
	{
		EndPos.x = StartPos.x + OffsetX -rand()%(Randomizer/2)+rand()%Randomizer;
		EndPos.y = StartPos.x + OffsetY -rand()%(Randomizer/2)+rand()%Randomizer;
	}
	else
	{
		EndPos.x = StartPos.x + OffsetX;
		EndPos.y = StartPos.x + OffsetY;
	}
	MovementFactorX = (EndPos.x - StartPos.x)/(float)nSteps;
	MovementFactorY = (EndPos.y - StartPos.y)/(float)nSteps;
	SetPos(&StartPos);
	Emit();	
}

void ParticleEffect::Start()
{
	//If user dislike sparkles don't disturn him!!
	if( IsSparkleFX && ProfileSystem::Instance->ActiveProfile.IsHideSparkles )
	{
		Visible = false;
		return;
	}

	Visible = true;
	Active = true;
	//Fridgefog timer will be initialized from game
	if( IsRandomEmission )
	{
		Timer = (rand()%(int)(100*EmitTimer))/100.0f;		
		if( Timer < 1.0f )
			Timer += 1.0f;
		InitRandomEmission();	
	}
	else
	{
		Timer = 0.0f;	
		Emit();
	}
}


void ParticleEffect::Emit()
{
	IsEmitting = true;
	Visible = true;//CRenderListEntity
	Particle *p;

	for(int i=0; i < EmitPartsPer; ++i)
	{
		p = Next();

		if( p == NULL )
			return;

		SetStartPos( p, i );
		SetVelocity( p, i );
		SetVariant( p );
		
		p->A		= IA;
		p->Rot		= IR;
		p->R		= R;
		p->G		= G;
		p->B		= B;
		p->S		= IS;

		p->Activate();
	}	
}


//Hard code for faster speed level traversal!!
void ParticleEffect::LoadSparkleEffectIni()
{
	VarNUE = 0;//small non uniform emission
	
	EffectFile = SPARKLE_VFX0;//save file name!!

	//=====================
	// Read Emitter Props SECTION
	//=====================
	Version = 3;
	

	//Read and load texture
	Texture = CTexFactory::Instance->GetImage("PRT\\sparkle.jpa",false,true);//Hiquality particle effects
	

	Tx = 0.0f;
	Ty = 0.0f;
	Tw = 64.0f;
	Th = 64.0f;

	Variants	= 1;
	BlendMode	= (EBlend)0;

	SourceType = (EffectEmissionSource)0;

	EmitSourceValue = 32.0f;
	EmitTimer		= 4.0f;

	IsRandomEmission = true;
	IsSparkleFX = true;//since this is a hardcoded sparkle
	
	EmitPartsPer	= 1.0f;
	MinEmitAngle	= 0.0f;
	MaxEmitAngle	= 0.0f;
	
	UniformEmission = 0;

	PartPool = 1;
	Particles.reserve(PartPool);	

	for(int i=0; i < PartPool; ++i)
	{
		Particles.push_back(new Particle());
	}

	//=====================
	// Read Particle Props SECTION
	//=====================

	//Velocity Magnitude
	DX = 0.0f;
	DY = DX;
	//Accel
	DDX	= 0.0f;
	DDY	= 0.0f;
	//Alpha
	IA = 0.1f;
	DA = 0.4f;
	EA = (ParticleEffectMode)3;
	//Rot
	IR = 0.0f;
	DR = 120.0f;
	ER = (ParticleEffectMode)2;
	//Tint
	R = 1.0f;
	G = 1.0f;
	B = 1.0f;
	TR = 1.0f;
	TG = 1.0f;
	TB = 1.0f;
	float dc = 0.0f;
	//Scale
	IS = 0.5f;	
	DS = 0.1f;
	ES = (ParticleEffectMode)3;
	//End 
	EndMode	= (ParticleEndMode)1;
	EndValue = 0.0f;

	//VERSION 2 SECTION
	DY = 0.0f;

	//VERSION 3 SECTION
	HiDef = false;
	
	Texture->setTextureQuality(HiDef);

	CalcDeltaTint(dc);

	/* Create vertex buffer */
    lpBatch = KPTK::createKBatch ();
    lpBatch->allocateBuffer (PartPool);
}
