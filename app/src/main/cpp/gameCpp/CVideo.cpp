/*
*	CVideo is a simple implementation of KVideo.
*	Author : Faisal
*   Test Usage (with Mask. Null for non masked Video): alphaTestCVideo = new CVideo("OGV\\ACTIONVIDEO.OGV", VidRectAlphaTest, false, 1,CTexFactory::Instance->GetImage("OGV\\L1S4\\POPVIDMASK.PNG"));
*/

#include "CVideo.h"
#include "Resources.h"
#include "Application.h"
#include "TextureFactory.h"
#include "ProfileSystem.h"

#include "AwardManager.h"

CVideo::CVideo(const std::string fileName, CRectangle vidRect, bool bLoopVideo, int zOrder, KGraphic *maskGraphic, KSound *lpSoundStream, bool isOGVInDataFolder, bool bPreload)
{
	isLoaded = false;

	_skipBtn = NULL;
	_isSkipEnabled = _isSkipMouseOver = _isSkipClicked = false;
	_skipShowTimer = 0;

	_video = NULL;
	_video = new KVideo();	

	_frameCount = 0;

	ZOrder = zOrder;

	_blendMode = 1;

	_isFadingIn = false;
	_isFadingOut = false;
	_fadeAlpha = 0;

	_isAutoFadeOutAfterPlay = false;
	_isEndReached = false;

	_fadeMultiplier = 1.0f;

	//Set size and width
	_origx = (int)vidRect.topleft.x;
	_origy = (int)vidRect.topleft.y;
	_w = vidRect.w;
	_h = vidRect.h;

	_scale = 1.0f;

	isPlaying = false;

	_maskImage = maskGraphic;

	_sndStream = lpSoundStream;


	if (_sndStream==NULL)
	{
		KSound *_soundChannel=NULL;
		#ifdef K_WIN32
		{
			if( !KSound::g_bErrSoundDeviceInit )
			{
				_soundChannel = new KSound();
			}
		}
		#else
		_soundChannel = new KSound();
		#endif

		if(_soundChannel != NULL)
			_soundChannel->setVolume((long)(ProfileSystem::Instance->ActiveProfile.VoiceVol*0.85f));

		_sndStream = _soundChannel;
	}
		
	//Control::Audio->TurnOffLvlMscTracks();//turns off music-snd carryover....

	bool bSuccess = false;

	if(isOGVInDataFolder)
		bSuccess = _video->openVideo( KMiscTools::makeFilePath(fileName.c_str()), bPreload, NULL, _sndStream, bLoopVideo, true );
	else
		bSuccess = _video->openVideo( FileSystem::charPath(fileName), bPreload, NULL, _sndStream, bLoopVideo, true );

	if (!bSuccess) 
	{
		KMiscTools::messageBox ("Video loading error! Couldn't open ", fileName.c_str() );
		exit(0);
	}

	Visible = true;//CRenderListEntity
	isLoaded = true;
}

void CVideo::SetSkipActive(float skipStartDelay)
{
	_skipBtn = new CObject();
	//SKIPIMAGE_XOFFSET_1366 1244
	_skipBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\SKIPBTN.SPR", (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y, ZOrder+10);
	_skipBtn->Show(false);

	_isSkipEnabled = true;
	_isSkipMouseOver = _isSkipClicked = false;

	_skipShowTimer = skipStartDelay;
}

void CVideo::Render( const CPoint* pCam  )
{
	if(!isLoaded)
		return;

	_frameCount = (_frameCount+1)%2;

	if( _isFadingIn || _isFadingOut )
	{
		//_video->getKGraphic()->blitAlphaRectFx (0, 0, _w, _h, _origx, _origy, 0, _scale, _fadeAlpha);
		_video->getKGraphic()->stretchAlphaRect(0, 0, _w, _h, 0, 0, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, _fadeAlpha);
		return;
	}

	if ( isPlaying )
	{
		if( _frameCount  || !_video->isVideoAvailable() )
			_video->readFrame();
		
		if(_maskImage)
		{
			_video->getKGraphic()->blitAlphaRectMaskedF (0, 0, _w, _h, _maskImage, 0, 0, _w, _h, (float)_origx, (float)_origy, _origx+_w, _origy+_h);
		}
		else
		{
			if(_blendMode!=1)
			{
				_video->getKGraphic()->setAlphaMode(_blendMode);
				//_video->getKGraphic()->blitAlphaRectFx (0, 0, _w, _h, _origx, _origy, 0, _scale, 1.0f);
				_video->getKGraphic()->stretchAlphaRect(0, 0, _w, _h, 0, 0, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, 1.0f);
			}
			else
			{
				//_video->getKGraphic()->blitAlphaRectFx (0, 0, _w, _h, _origx, _origy, 0, _scale, 1.0f);
				_video->getKGraphic()->stretchAlphaRect(0, 0, _w, _h, 0, 0, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, 1.0f);
			}
		}

	}
}

void CVideo::SetBlendMode(int blendMode)
{
	_blendMode = blendMode;
}

void CVideo::PlayVideo()
{
	if(isLoaded)
	{
		Control::Audio->TurnOffLvlMscTracks();
		isPlaying = true;	
	}
}

void CVideo::StopVideo()
{
	if(isLoaded)
	{
		isPlaying = false;
		if(_sndStream!=NULL)
			_sndStream->stopStream();

		if(_skipBtn)
		{
			if(_skipBtn->Visible)
				_skipBtn->Show(false);
		}
	}
}

void CVideo::SetScale(float scale)
{
	_scale = scale;
}

void CVideo::StopAndFadeOutVideo(float speedMultiplier)
{
	if(isLoaded)
	{
		if(_skipBtn)
		{
			if(_skipBtn->Visible)
				_skipBtn->Show(false);
		}

		isPlaying = false;
		if(_sndStream!=NULL)
			_sndStream->stopStream();
		FadeOutVideo(speedMultiplier);
	}
}

bool CVideo::isEndReached()
{
	return (_isEndReached || _isSkipClicked);
}

void CVideo::PlayFadeInVideo(float speedMultiplier)
{
	if(isLoaded)
	{
		if( !_video->isVideoAvailable() )
			_video->readFrame();
	}

	isPlaying = true;
	Control::Audio->TurnOffLvlMscTracks();

	_fadeMultiplier = speedMultiplier;
	_isFadingIn = true;
	_fadeAlpha = 0.0f;
}

void CVideo::FadeOutVideo(float speedMultiplier)
{
	_fadeMultiplier = speedMultiplier;
	_isFadingOut = true;
	_fadeAlpha = 1.0f;
}

float CVideo::GetTime()
{
	return _video->getTime();
}

void CVideo::Update(float ds)
{
	if( isLoaded && isPlaying && (_video->isEndReached() || (_video->getTime() >= _video->getTotalTime()-0.1f)))
		_isEndReached = true;

	if(_isSkipEnabled && isPlaying)
	{
		if(_skipShowTimer>0)
		{
			_skipShowTimer -= ds;
		}
		else
		{
			if(_skipBtn)
			{
				if(!_skipBtn->Visible)
					_skipBtn->Show(true);

				CPoint mousePos = Control::Input->Mpos();
			//	_isSkipMouseOver = _skipBtn->Collide(&Control::Input->Mpos());
			// Linux: gives error of taking address of temporary also potential crash factor
			// below is fix
				_isSkipMouseOver = _skipBtn->Collide(&mousePos);
				if(	_isSkipMouseOver )
				{
					Cursor::SetMode(CA_HandPointy);
					_skipBtn->PlayAnimation(1);
					if(Control::Input->LBclicked())
					{
						_isSkipEnabled = false;

						_isSkipClicked = true;
						_skipBtn->Show(false);
						Control::Audio->PlaySample(aSFXClick);
						Control::Input->ClearMouse();
						AwardManager::GetInstance()->SkipDialogueOrCutScene();
					}
				}
				else
				{
					_skipBtn->PlayAnimation(0);
				}
			}
		}
	}

	if( _isFadingIn )
	{
		_fadeAlpha += _fadeMultiplier*ds;
		if(_fadeAlpha>=1.0)
		{
			_fadeAlpha = 1.0;
			_isFadingIn = false;
		}
	}
	else if( _isFadingOut )
	{
		_fadeAlpha -= _fadeMultiplier*ds;
		if(_fadeAlpha<=0)
		{
			_fadeAlpha = 0;
			_isFadingOut = false;

			if(_isAutoFadeOutAfterPlay)
				_isAutoFadeOutAfterPlay = false;
		}
	}
	else if( _isAutoFadeOutAfterPlay && _isEndReached)
	{
		FadeOutVideo(_fadeMultiplier);
	}
}

CVideo::~CVideo(void)
{
	if( _maskImage )
	{
		CTexFactory::Instance->FreeImage(&_maskImage);
		_maskImage = NULL;	
	}

	if( _video!=NULL )
	{
		delete _video;
		_video = NULL;
	}

	if(_sndStream!=NULL)
	{
		delete _sndStream;
		_sndStream = NULL;
	}

	if(_skipBtn!=NULL)
	{
		delete _skipBtn;
		_skipBtn = NULL;
	}
}

void CVideo::Load(std::string Str )
{
	
}
