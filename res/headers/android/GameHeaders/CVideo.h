#ifndef CVIDEO_H
#define CVIDEO_H

#include "KVideo.h"
#include "RenderListEntity.h"
#include "UpdateListEntity.h"
#include "Rect.h"
#include "Object.h"
#include "KSound.h"

class CVideo : public CRenderListEntity, UpdateListEntity
{
	int _origx, _origy;
	float _w, _h;
	float _scale;
	KVideo *_video;
	int _frameCount;

	bool _isFadingIn, _isFadingOut, _isAutoFadeOutAfterPlay;
	float _fadeAlpha, _fadeMultiplier;

	bool _isEndReached;

	KGraphic *_maskImage;

	KSound *_sndStream;

	CObject *_skipBtn;
	bool	_isSkipEnabled, _isSkipMouseOver, _isSkipClicked;
	float	_skipShowTimer;

	int _blendMode;

public:
	bool isLoaded, isPlaying;

	/**
    * Create CVideo Object
    *
    * \param fileName - name of file to load video from
	* \param vidRect - position and size of the Video
	* \param bLoopVideo - true to loop the video indefinitely, false to play it once and report the end using isEndReached()
	* \param zOrder - Zorder of the video
	* \param maskGraphic - Mask KGraphic Alpha PNG. Only alpha pixels will be displayed
	* \param lpSoundStream - sound instance to decode audio stream to, NULL to ignore any audio stream
	* \param isOGVInDataFolder - OGV file placed in data folder. Not in Assets folder path
    * \param bPreload - true to entirely load the compressed video file in memory (and avoid disk access during playback),
    *                 false to stream from disk as needed    
    */

	CVideo(const std::string fileName, CRectangle vidRect, bool bLoopVideo = false, int zOrder=0, KGraphic *maskGraphic=NULL, KSound *lpSoundStream = NULL, bool isOGVInDataFolder = false, bool bPreload = true);
	~CVideo(void);

	//Basic Object Methods
	virtual void Load(std::string Str);
	virtual void Load(std::string Str, bool IsInteractiveObj){}
	virtual void Update(float ds);
	virtual void Render(const CPoint* pCam);

	void SetBlendMode(int blendMode);

	void SetSkipActive(float skipStartDelay=3.5f);

	void SetScale(float scale);
	
	void StopVideo();
	void StopAndFadeOutVideo(float speedMultiplier);

	void PlayVideo();
	void PlayFadeInVideo(float speedMultiplier);
	
	void FadeOutVideo(float speedMultiplier);

	void SetAutoFade(float speedMultiplier){ _isAutoFadeOutAfterPlay = true; _fadeMultiplier = speedMultiplier;}

	bool isSkipMouseOver(){ return _isSkipMouseOver;}

	bool isEndReached();

	float GetTime();
};

#endif
