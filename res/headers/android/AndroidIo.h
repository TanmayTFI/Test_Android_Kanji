/**
 * Kanji rendering and I/O engine - Android system functions definition
 *
 * (c) 2010
 */

#ifndef  _KANJI_ANDROID_IO_H
#define  _KANJI_ANDROID_IO_H

/* Include definitions */
#include "KSysDefs.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

/* Forward declarations */
class AndroidSound;
class AndroidMediaPlayer;
class AndroidMoviePlayer;

/* Prototypes */
JavaVM *androidGetJVM (void);
jclass androidGetThreadPeerClass (void);
void androidSetThreadPeerClass (jclass threadPeerClass);
jobject androidGetThreadCaller (void);
void androidSetThreadCaller (jobject threadCaller);
long androidGetScreenWidth (void);
long androidGetScreenHeight (void);
long androidGetScreenInches (void);
bool androidHasScreenSizeChanged (void);
void androidEnableNativeEventsDelivery (void);
void androidProcessEvents (void);
long androidGetEventSource (void);
long androidGetEventDeviceId (void);
long androidGetDeviceEventSources (int nDeviceId);
long androidGetJoystickCount (void);
long androidGetJoystickEventSources (int nJoystickIdx);
bool androidHasQuit (void);
bool androidIsActivityPaused (void);
void androidBindBackBuffer (void);
void androidFlipBackBuffer (void);
void androidSetScreenOrientation (long nOrientation);
void androidCollectGarbage (void);
void androidLogMessage (const char *lpszMessage);
void androidSetVirtualKeyboardState (bool bShow);
void androidRestoreKeyboardState (void);
void androidCatchBackKey (bool bCatch);
void androidSendToBack (void);
void androidDisableIdleTimer (bool bDisable);
bool androidOpenDirectAssetFile (const char *lpszFilename, int &fd, size_t &nOffset, size_t &nSize, jobject &fileObj);
void androidCloseDirectAssetFile (jobject &fileObj);
bool androidOpenAssetFile (const char *lpszFilename, AAsset **ppAsset, size_t &nOffset);
void androidCloseAssetFile (AAsset *pAsset);
void androidJniInitWithAssetManager(JNIEnv* env, jobject obj, jobject assetManager);
void androidLaunchURL (const char *lpszUrl);
void androidShowMessageBox (const char *lpszCaption, const char *lpszBody);
long androidShowAlertBox (const char *lpszCaption, const char *lpszBody, const char *lpszButton1Text, const char *lpszButton2Text);
void androidShowWebBrowser (const char *lpszUrl, const char *lpszCloseButtonText);
long androidSendEmail (const char *lpszTo, const char *lpszCc, const char *lpszBcc, const char *lpszSubject, const char *lpszBody);
void androidVibrate (void);
long androidGetScreenRotation (void);
long androidGetMinSoundBufferSize (long nSampleRate, long nChannels, long nBitsPerSample);
AndroidSound *androidSoundCreate (long nSampleRate, long nChannels, long nBitsPerSample, long nBufferSize, bool bLoops, bool bStatic);
void androidSoundDestroy (AndroidSound *snd);
void androidSoundWriteSamples (AndroidSound *snd, const char *lpBuffer, size_t nSize);
void androidSoundSetVolume (AndroidSound *snd, float fLeftVolume, float fRightVolume);
void androidSoundSetRate (AndroidSound *snd, long nSampleRate);
void androidSoundSetPeriodicNotification (AndroidSound *snd, long nPeriod, long nUserData);
void androidSoundFlush (AndroidSound *snd);
void androidSoundPlay (AndroidSound *snd);
void androidSoundStop (AndroidSound *snd);
void androidSoundPause (AndroidSound *snd);
bool androidSoundIsReady (AndroidSound *snd);
bool androidSoundIsPlaying (AndroidSound *snd);
void androidSoundRelease (AndroidSound *snd);
AndroidMediaPlayer *androidMediaPlayerCreate (const char *lpszPath);
void androidMediaPlayerDestroy (AndroidMediaPlayer *mp);
void androidMediaPlayerSetVolume (AndroidMediaPlayer *mp, float fLeftVolume, float fRightVolume);
void androidMediaPlayerPlay (AndroidMediaPlayer *mp, bool bLoops);
void androidMediaPlayerStop (AndroidMediaPlayer *mp);
void androidMediaPlayerPause (AndroidMediaPlayer *mp);
bool androidMediaPlayerIsPlaying (AndroidMediaPlayer *mp);
void androidMediaPlayerRelease (AndroidMediaPlayer *mp);
AndroidMoviePlayer *androidMoviePlayerCreate (const char *lpszPath, int x1, int y1, int x2, int y2, bool bVideoLoops, float fLeftVolume, float fRightVolume, bool bPlayBehindKWindow);
void androidMoviePlayerDestroy (AndroidMoviePlayer *mp);
bool androidMoviePlayerIsVisible (AndroidMoviePlayer *mp);
bool androidMoviePlayerIsPlaying (AndroidMoviePlayer *mp);
void androidMoviePlayerSuspend (AndroidMoviePlayer *mp);
void androidMoviePlayerResume (AndroidMoviePlayer *mp);
void androidMoviePlayerUpdateVideo (AndroidMoviePlayer *mp);
void androidMoviePlayerSetVolume (AndroidMoviePlayer *mp, float fLeftVolume, float fRightVolume);
void androidMoviePlayerRelease (AndroidMoviePlayer *mp);
void androidAttachCurrentThread (void);
void androidDetachCurrentThread (void);
const char *androidGetFilesPath (void);
const char *androidGetOSString (long nStringIdx);
const char *androidGetPackageMetadata (const char *lpszPropertyName);
void androidShowBusyWaiting (bool bShow, const char *lpszMessage);
void androidShowToast (const char *lpzText, bool bLong);
bool androidIsInternetReachable (void);
void androidDoHttpGet (long nRequestId, const char *lpszUrl, const char *lpszResponseFilePath, bool bBypassCache);
void androidDoHttpPost (long nRequestId, const char *lpszUrl, const void *lpPayloadData, size_t nPayloadSize, const char *lpszResponseFilePath, bool bBypassCache);
void androidDoHttpDelete (long nRequestId, const char *lpszUrl, const char *lpszResponseFilePath, bool bBypassCache);
void androidScheduleLocalNotification (long nDaysFromNow, long nHours, long nMinutes, long nSeconds, bool bAbsoluteTime, const char *lpszMessage, long nUserData);
void androidCancelLocalNotifications ();
bool androidCheckPermissions (const char *lpszPermissions);
bool androidRequestPermissions (const char *lpszPermissions, const char *lpszRationale);

#endif   /* _KANJI_ANDROID_IO_H */
