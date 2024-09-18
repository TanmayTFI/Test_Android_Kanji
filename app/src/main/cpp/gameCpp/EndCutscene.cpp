//====================================
// Framework
// EndCutscene Control
//====================================
#include "VideoDefs.h"

#include "EndCutscene.h"
#include "Resources.h"
#include "Object.h"
#include "Cursor.h"
#include "Application.h"
#include "ProfileSystem.h"
#include "Util.h"
#include "BitmapText.h"
#include "GameFlow.h"
#include "Fonts.h"
#include "LangDefs.h"
#include "TextureFactory.h"

//========================
//EndGame Cinematic Text 
//========================
#ifndef ENGLISH_BUILD
    #define MAX_END_CINE_SCENES 5
    #define MAX_END_CINE_STRINGS 2

	#if defined( GERMAN_BUILD )//LANG_TODO:
	char *EndCineStrings[MAX_END_CINE_SCENES][MAX_END_CINE_STRINGS] = 
	{
	
	};
	#elif defined( SPANISH_BUILD )
	char *EndCineStrings[MAX_END_CINE_SCENES][MAX_END_CINE_STRINGS] = 
	{
	
	};
	#elif defined( FRENCH_BUILD )
	char *EndCineStrings[MAX_END_CINE_SCENES][MAX_END_CINE_STRINGS] = 
	{
	
	};
	#elif defined( NEDERLANDS_BUILD )
	char *EndCineStrings[MAX_END_CINE_SCENES][MAX_END_CINE_STRINGS] = 
	{
	
	};
	#elif defined( ITALIAN_BUILD )
	char *EndCineStrings[MAX_END_CINE_SCENES][MAX_END_CINE_STRINGS] = 
	{
	
	};
	#elif defined( PORTUGUESE_BUILD )	
	char *EndCineStrings[MAX_END_CINE_SCENES][MAX_END_CINE_STRINGS] = 
	{
	
	};
	//Japanese will load directly from stringtable to avoid unicode compilation issues!!
	#endif

    float EC_SlideEndTimers[MAX_END_CINE_SCENES] = {0,0,0,0,0};
    #define FADEINOUT_GAP 1.0f
    #define XTRA_START_WAIT_GAP 0.5f
    enum { SLIDE_FADEIN, SLIDE_FADEOUT, SLIDE_STANDSTILL, SLIDE_OVER };

#endif



EndCutsceneSeries::EndCutsceneSeries()
{

#ifndef ENGLISH_BUILD
	for( int i = 0; i < 2; i++ )
		SlideTxts[i] = NULL;
#endif


	SetZ(1); 		
	GFInstance->UnloadAllAssets();
    
    //For iPad we will load here
	GFInstance->LoadEndCutscene();//Load if not closed before
    
#ifndef ENGLISH_BUILD
	//Load from ini........
	KIniReader *iniReader;
	iniReader = new KIniReader ;
	iniReader->setIni( FileSystem::charPath("CNM\\EndCnm.INI") ) ;	

	EC_SlideEndTimers[0] = iniReader->getFloatValue("Slides","S0");
	EC_SlideEndTimers[1] = iniReader->getFloatValue("Slides","S1");
	EC_SlideEndTimers[2] = iniReader->getFloatValue("Slides","S2");
	EC_SlideEndTimers[3] = iniReader->getFloatValue("Slides","S3");
	EC_SlideEndTimers[4] = iniReader->getFloatValue("Slides","S4");

	delete iniReader;
	iSlide = 0;
	SlideState = SLIDE_FADEIN;
    CreateSlideTxts();
	
#endif

	if( !GFInstance->IsEndCutsceneLoaded )
		Exit();
	else
	{
		GFInstance->lpEndCutsceneVideo->rewind();
		GFInstance->lpEndCutsceneVideo->readFrame();
		if( !ProfileSystem::Instance->ActiveProfile.IsMuted )
			Control::Audio->PlaySample(aTrackEndCutscene);

		
		
		Visible = true;//CRenderListEntity	
	}
}


#ifndef ENGLISH_BUILD		
void EndCutsceneSeries::CreateSlideTxts()
{
	CPoint pos;
	for( int i = 0; i < 2; i++ )
	{
		SlideTxts[i] = new CBitmapText();
		SlideTxts[i]->LoadFont(eFont_25);
		SlideTxts[i]->SetAlignment(Align_Center);
        pos = CPoint(512.0f,718+i*32.0f,0);
		SlideTxts[i]->SetPos(&pos);
		SlideTxts[i]->SetScale(1.0f);
		SlideTxts[i]->SetColor(1.0f,1.0f,1.0f); //white
		SlideTxts[i]->SetZ(3000);
		//SlideTxts[i]->SetText("");
		SlideTxts[i]->Show(true);		
		SlideTxts[i]->SetAlpha(0.0f);
		#ifdef JAPANESE_BUILD
				SlideTxts[i]->SetText(JapCineStr(0,i)); 		
		#else
				SlideTxts[i]->SetText(EndCineStrings[0][i]); 		
		#endif

		
	}
}

void EndCutsceneSeries::UpdateSlideTxts()
{
	double MSec = GFInstance->lpEndCutsceneVideo->getTime();
	switch( SlideState )
	{
		case SLIDE_OVER      :break;
		case SLIDE_FADEIN    :if( MSec <= 1.0f )//slowly fade in first time!!// < 1sec
							  {
								  for( int i = 0; i < 2; i++ )	
									SlideTxts[i]->SetAlpha((float)MSec);//0.0f->1.0f
							  }
							  else if( iSlide > 0 && MSec <= (XTRA_START_WAIT_GAP+EC_SlideEndTimers[iSlide-1]+FADEINOUT_GAP) )//slowly fade in first time!!// < 1sec
							  {
							  if( MSec > (XTRA_START_WAIT_GAP+EC_SlideEndTimers[iSlide-1]) )
								  for( int i = 0; i < 2; i++ )	
									SlideTxts[i]->SetAlpha((float)MSec-XTRA_START_WAIT_GAP-EC_SlideEndTimers[iSlide-1]);//0.0f->1.0f
							  }
							  else //end case!!			
							  {
								  SlideState = SLIDE_STANDSTILL;							  
								  for( int i = 0; i < 2; i++ )	
									SlideTxts[i]->SetAlpha(1.0f);
							  }
							  break;										  
		case SLIDE_STANDSTILL:if( MSec > (EC_SlideEndTimers[iSlide]-FADEINOUT_GAP) ) 
								  SlideState = SLIDE_FADEOUT;
							  break;											
		case SLIDE_FADEOUT   :if( MSec <= EC_SlideEndTimers[iSlide] ) 
							  {
								  for( int i = 0; i < 2; i++ )	
									SlideTxts[i]->SetAlpha(EC_SlideEndTimers[iSlide]-(float)MSec);//0.0f->1.0f
							  }
							  else
							  {
								  if( iSlide == (MAX_END_CINE_SCENES-1) )
									  SlideState = SLIDE_OVER;
								  else
								  {
									  iSlide++;
									  for( int i = 0; i < 2; i++ )
									  {
										  
										    #ifdef JAPANESE_BUILD
													SlideTxts[i]->SetText(JapCineStr(iSlide,i)); 		
											#else
													SlideTxts[i]->SetText(EndCineStrings[iSlide][i]); 		
											#endif
										  SlideTxts[i]->SetAlpha(0.0f);
									  }
									  SlideState = SLIDE_FADEIN;
								  }
							  }
							  break;	
							  
	}	
}
#endif

void EndCutsceneSeries::Update()
{	
	if( ProfileSystem::Instance->ActiveProfile.IsEndCutscenePlayed )
		return;	
		
	Control::MasterTmr->ClearCycles();

	Application::SetSingleUpdate(true);//Lock
    long PrevFrame = -1, CurFrame;
	int FrameCnt = 0;
	unsigned long VidStartTime = KMiscTools::getMilliseconds();
	int PlayTime;
    
    KVideo *CineVideo = GFInstance->lpEndCutsceneVideo;
	
	const CPoint pCam(0,0,0);
		
	//For Fast performance directly accessing!!
	while( true )
	{
		if( GFApp->Window->isQuit() )//inside loop check 
		{
			GFApp->AppActive = false;
			return;
		}	
		
		FrameCnt = (FrameCnt+1)%2;
        if( FrameCnt == 0 || !CineVideo->isVideoAvailable() )
                CineVideo->readFrame();
		
        PlayTime = (int) (KMiscTools::getMilliseconds() - VidStartTime)/1000; //Sac: added expicit conversion of Unsinged long to int
		

		
        if( CineVideo->isEndReached() )
		{
			#ifndef ENGLISH_BUILD
				for( int i = 0; i < 2; i++ )
					SlideTxts[i]->Show(false);
			#endif
			ProfileSystem::Instance->ActiveProfile.IsEndCutscenePlayed = true;
			Control::Input->ClearMouse();
			Exit();
			return;
		}

        //Avoid redrawing same frame "fps" times as no dynamic entity like cursor!!
		CurFrame = CineVideo->getFrame();
		if(  CurFrame != PrevFrame )
			{
				PrevFrame = CurFrame;
				//render cinematics!!
			
			#ifdef ENGLISH_BUILD
				//full videorect copy - hence full buffer not cleared!!
				GFApp->Window->setWorldView (GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, false); //no clear!!
				//CineVideo->getKGraphic()->blitRect(0, 0,1366,CineVideo->getFrameHeight(), 0,0);
			CineVideo->getKGraphic()->stretchAlphaRect(0,0,GFApp->video_width,GFApp->video_height,0,0,MAX_DISPLAY_WIDTH,MAX_DISPLAY_HEIGHT,eBlend_AlphaNormal,false,false,0,0);
			#else
				GFApp->Window->setWorldView (GFApp->ProjX, GFApp->ProjY, 0.0f, GFApp->ProjZoom, true); //clear!!
				//CineVideo->getKGraphic()->blitRect(0, 0,1366,CineVideo->getFrameHeight()-72, 0,0);
				CineVideo->getKGraphic()->stretchAlphaRect(0,0,GFApp->video_width,GFApp->video_height,0,0,MAX_DISPLAY_WIDTH,MAX_DISPLAY_HEIGHT,eBlend_AlphaNormal,false,false,0,0);
				if( SlideState != SLIDE_OVER )
				{
					UpdateSlideTxts();
					for( int i = 0; i < 2; i++ )
						SlideTxts[i]->Render(&pCam);
				}
			#endif

				GFApp->DrawProjEdgeCorrection();
				GFApp->Window->flipBackBuffer();
			}
    }
	Application::SetSingleUpdate(false);//Unlock
	Cursor::Show(true);
}


void EndCutsceneSeries::Render(const CPoint* pCam)
{

}




EndCutsceneSeries::~EndCutsceneSeries()
{	
	
	if( Control::Audio->IsPlaying(aTrackEndCutscene) )
		Control::Audio->StopSample(aTrackEndCutscene);
	
	if( GFInstance != NULL )   
        GFInstance->UnloadEndCutscene();//Unload anyway as little chance he will see this again in a sequence play!!		
	
		
#ifndef ENGLISH_BUILD
	for( int i = 0; i < 2; i++ )
		if( SlideTxts[i] != NULL )
		{
			delete SlideTxts[i];
			SlideTxts[i] = NULL;
		}	
#endif
}


#ifdef JAPANESE_BUILD
const char* EndCutsceneSeries::JapCineStr( int iSlide, int iRow )
{
	switch( iSlide )
	{
		default:return "";
		case 0:if( iRow == 0 )
				    return StringTable::getString("ECNM_00");
			   else
				    return StringTable::getString("ECNM_01");

		case 1:if( iRow == 0 )
				    return StringTable::getString("ECNM_10");
			   else
				    return "";
			   

		case 2:if( iRow == 0 )
				    return StringTable::getString("ECNM_20");
			   else
				    return StringTable::getString("ECNM_21");
			   

		case 3:if( iRow == 0 )
				   return StringTable::getString("ECNM_30");
			   else
				   return "";

		case 4:if( iRow == 0 )
				    return StringTable::getString("ECNM_40");
			   else
					return "";			   
	}
}
#endif
