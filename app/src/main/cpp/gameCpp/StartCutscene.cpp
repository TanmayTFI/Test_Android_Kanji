//====================================
// Framework
// StartCutscene Control
//====================================
#include "VideoDefs.h"
 
#include "StartCutscene.h"
#include "Resources.h"
#include "Object.h"
#include "Sprite.h"
#include "Application.h"
#include "TextureFactory.h"
#include "ProfileSystem.h"
#include "Util.h"
#include "BitmapText.h"
#include "GameFlow.h"
#include "Constants.h"
#include "Fonts.h"
#include "KVideo.h"

#define MAX_CLEARTOP_FRAMES 3
#define SKIPIMAGE_CURSORBOTTOM 128 //13+60+  55[7+48]

//========================
//StartGame Cinematic Text
//========================


StartCutsceneSeries::StartCutsceneSeries()
{

    SetZ(1);
    CoreImage   = NULL;
    CoreImage   = CTexFactory::Instance->GetImage("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\CORE.PNG");
    CoreImage->setTextureQuality(true);


	letterBase = NULL;
	letterBase = CTexFactory::Instance->GetImage("OGV\\CREEKLETTER.PNG");
	letterBase->setTextureQuality(true);

	letterTxt = NULL;
	letterTxt = CTexFactory::Instance->GetImage("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S1\\CREEKLETTERMARY.PNG");
	letterTxt->setTextureQuality(true);

	gulfTxt = NULL;
	gulfTxt = CTexFactory::Instance->GetImage("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\VIDEO\\GULF.png");
	gulfTxt->setTextureQuality(true);

    BG = NULL;
    BG = CTexFactory::Instance->GetImage(("UI\\BLACKBG.PNG"));
    BG->setTextureQuality(true);
    BG->blitRect(0,0,1366,768, 0, 0);
    
    //Sac: See if the person is just starting the game or in level 1, then play cutscene
    // or else dont make player bored to see the cutscene again and agan.
    if(ProfileSystem::Instance->ActiveProfile.CurrentLevel<=ELevel::Level_1)
    {
        GFInstance->LoadStartCutscene();//Load if not closed before
    }
    
    if( !GFInstance->IsStartCutsceneLoaded )
        Exit();
    else
    {
        Visible = true;//CRenderListEntity
    }

	time = 0;
	subIndex = 0;
	subMax = 6;
		
	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("start1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("start2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("start3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("start4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("start5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("start6"));

}


StartCutsceneSeries::~StartCutsceneSeries()
{
    Visible = false;


    #ifdef ENGLISH_BUILD
            if( GFInstance != NULL )
                if( GFInstance->lpStartCutsceneSndStream != NULL && GFInstance->IsStartCutsceneLoaded )
                    if( GFInstance->lpStartCutsceneSndStream->isPlaying() )
                        GFInstance->lpStartCutsceneSndStream->stopSample();
        //if( Control::Audio->IsPlaying(aTrackCutscnGameIntro) )
        //    Control::Audio->StopSample(aTrackCutscnGameIntro);
    #else
        if( Control::Audio->IsPlaying(aTrackCutscnGameIntro) )
            Control::Audio->StopSample(aTrackCutscnGameIntro);
    #endif

    if( GFInstance != NULL )
        GFInstance->UnloadStartCutscene();//sysstopvideo done here
    
    if( CoreImage )
    {
        CTexFactory::Instance->FreeImage(&CoreImage);
        CoreImage = NULL;
    }

	if (letterBase)
	{
		CTexFactory::Instance->FreeImage(&letterBase);
		letterBase = NULL;
	}

	if (letterTxt)
	{
		CTexFactory::Instance->FreeImage(&letterTxt);
		letterTxt = NULL;
	}

	if (gulfTxt)
	{
		CTexFactory::Instance->FreeImage(&gulfTxt);
		gulfTxt = NULL;
	}

    if(BG)
    {
        BG = NULL;
    }

}


void StartCutsceneSeries::Update()
{
    if( ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed )
        return;

    CPoint mosPos;
    const CPoint pCam(0,0,0);
    static int CineFrameCnt = 0;
    // bool IsSkipDraw = false;  // Sac: Commenting Unused variable
    bool isMosHover = false;
         
    float GFAppProjX = GFApp->ProjX,
          GFAppProjY = GFApp->ProjY,
          GFAppProjZoom = GFApp->ProjZoom;

    Control::MasterTmr->ClearCycles();
    
    KVideo *CineVideo = GFInstance->lpStartCutsceneVideo;

    #ifndef ENGLISH_BUILD
        Control::Audio->PlaySample(aTrackCutscnGameIntro);
    #endif
    //Control::Audio->PlaySample(aTrackCutscnGameIntro,false);
    
    Application::SetSingleUpdate(true);

    //For Fast performance directly accessing!!
    while( true )
    {
        CineFrameCnt = (CineFrameCnt+1)%2;
        mosPos = Control::Input->Mpos();

#ifdef TOUCH_UI
        mosPos.x = (KInput::getMouseX() / GFApp->ProjZoom) + std::abs((GFApp->ProjX - GFApp->projXMax)/GFApp->ProjZoom);
        mosPos.y = (KInput::getMouseY() / GFApp->ProjZoom) + std::abs((GFApp->ProjY + GFApp->projYMax)/GFApp->ProjZoom);
#endif

            if( CineFrameCnt  || !CineVideo->isVideoAvailable() )
            {
                //CHECK IF TIME TO QUIT GAMEWINDOW - AS INSIDE WHILE LOOP!!
                if( GFApp->Window->isQuit() )
                {
                    GFApp->AppActive = false;
                    return;
                }

                //CHECK FOR EXIT!!
                CineVideo->readFrame();
                if( CineVideo->isEndReached() || CineVideo->getFrame() >= 2856 )
                {
                    #ifndef ENGLISH_BUILD
                        for( int i = 0; i < 2; i++ )
                            SlideTxts[i]->Show(false);
                    #endif
                    Control::Input->ClearMouse();
                    
                    ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
                    Exit();
                    return;
                }
            }
    
            isMosHover = ( (mosPos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0))) &&
                mosPos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
                mosPos.y >= SKIPIMAGE_Y &&
                mosPos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) + SKIPIMAGE_WIDTH) );

            //render cinematics!!
            GFApp->Window->setWorldView(GFAppProjX, GFAppProjY, 0.0f, GFAppProjZoom, false);
			BG->blitRect(0, 0, 1366, 768, 0, 0);

			//Subtitles Code
			time = CineVideo->getTime();

			if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
			{
				subtitleText = subtitles[subIndex].text;
			}
			else
				subtitleText = "";

			if (time > subtitles[subIndex].end)
			{
				if (subMax - 1 > subIndex)
					subIndex++;
			}

			

			CineVideo->getKGraphic()->stretchAlphaRect(0, 0, GFApp->video_width, GFApp->video_height, 0, 0, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, eBlend_AlphaNormal, false, false, 0, 0);

			if (StringTable::GetMyLangauge() != "EN")
			{
				if (time >= 8.5 && time < 19)
				{
					letterBase->blitAlphaRect(0, 0, 1366, 768, 0, 0);
					//letterTxt->blitAlphaRect(0, 0, 445, 328, 450, 200);
					letterTxt->blitAlphaRectFx(0, 0, 445, 328, 460, 250, 0, 1.25, 1);
				}

				if (time >= 0 && time < 4)
				{
					gulfTxt->blitAlphaRectFx(0, 0, 448, 110, 440, 325, 0, 1.5, 1);
				}
			}
			//GFApp->lpFont22->drawStringFromRight(subtitleText.c_str(), 635, 700);
			//GFApp->lpFont20.dr
			GFApp->lpFont22->drawStringCentered(subtitleText.c_str(), 483,883, 720);
            //update cinematics!!
            if( isMosHover )
            {
                Control::Input->LBtnUpdate();
                CoreImage->blitAlphaRectF( 64,0,128,32,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);
                if( Control::Input->LBclicked() || KInput::isPressed (K_VK_RETURN) )
                {
                    CoreImage->blitAlphaRectF( 64,0,128,32,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);
                    Control::Audio->PlaySample(aSFXClick);
                    Control::Input->ClearMouse();
                    
                    ProfileSystem::Instance->ActiveProfile.IsStartCutscenePlayed = true;
                    Exit();
                    return;
                }
                else
                {
                    CoreImage->blitAlphaRectF( 64,0,128,32,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);
                }
                CoreImage->blitAlphaRect( 192,0,256,64,(short)mosPos.x,(short)mosPos.y);//Cursor
            }
            else //Cursor not over skip region!!
            {
                CoreImage->blitAlphaRectF( 0,0,64,32,(SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y);
                    #ifndef TOUCH_UI
                        CoreImage->blitAlphaRect( 128,0,192,64,(short)mosPos.x,(short)mosPos.y);//Cursor
                    #endif
            }
            GFApp->DrawProjEdgeCorrection();
            GFApp->Window->flipBackBuffer();
    }
}


