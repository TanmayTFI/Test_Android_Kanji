#include "iPadDefs.h"


#ifdef K_IPAD
#include "Control.h"
#include "Rect.h"
#include "GameFlow.h"
#import "mp4_ipad.h"
#import "KanjiAppDelegate.h"
static MPMoviePlayerController *g_playerViewController = nil;
static CRectangle sysVideoRect;
static bool prevPauseStatus = false;
static bool prevHideStatus = false;
static bool isVideoLoaded = false;
static std::string g_VideoName ="";

CRectangle getSysVideoRect()
{
    return sysVideoRect;
}

bool unHideIfSysVideoLoaded()
{
 if( g_playerViewController && !GFInstance->IsActivePopup )
    if( g_playerViewController.view.hidden == YES && !prevPauseStatus && !prevHideStatus )
     if ( g_playerViewController.loadState == (MPMovieLoadStatePlaythroughOK | MPMovieLoadStatePlayable) && 
        ( g_playerViewController.playbackState == MPMoviePlaybackStatePlaying || g_playerViewController.playbackState == MPMoviePlaybackStatePaused ) )
         //loading is complete!!
     {
         g_playerViewController.currentPlaybackTime = 0;
         isVideoLoaded = true;
         g_playerViewController.view.hidden = NO;//time to unhide!!
         return true;
     }
    
    return false;
}

bool isPrevPauseStatus()
{
    return prevPauseStatus;
}

void sysVideoHidePause( bool Status )
{
    if( g_playerViewController )
    {
        if( Status )//going to pause
        {
            prevPauseStatus = isSysVideoPBStatePaused();
            prevHideStatus = isSysVideoHidden();
            if( !prevPauseStatus )
                [g_playerViewController pause];
            if( !prevHideStatus )
                g_playerViewController.view.hidden = YES;
        }
        else
        {
            if( !prevPauseStatus )//restore old state!!
                [g_playerViewController play];
            if( !prevHideStatus )//restore old state!!
                g_playerViewController.view.hidden = NO;
            prevPauseStatus = false;
            prevHideStatus = false;
        }
    }
}


bool isSysVideoLoaded()
{
    if( g_playerViewController )
        return isVideoLoaded;
    else
        return false;
}

bool isSysVideoHidden()
{
    if( g_playerViewController )
        return g_playerViewController.view.hidden;
    else 
        return true;
}

bool isSysVideoPBStatePaused()
{
    if( g_playerViewController )
        return (g_playerViewController.playbackState == MPMoviePlaybackStatePaused);
    else 
        return false;
}

void sysVideoPause( bool Status )
{
    if( g_playerViewController )
    {
        if( Status )
            [g_playerViewController pause];
        else
            [g_playerViewController play];            
    }
}


void sysVideoHide( bool Status )
{
    if( g_playerViewController )
    {
        if( Status )//going to pause
            g_playerViewController.view.hidden = YES;
        else
            g_playerViewController.view.hidden = NO;        
    }
}

bool isSysVideoCached( std::string lpszVideoName )
{
    if( g_playerViewController && isVideoLoaded )
        return (g_VideoName == lpszVideoName);
    else
        return false;
}


bool isSysVideoLoading( std::string lpszVideoName )
{
    if( g_playerViewController )
        return (g_VideoName == lpszVideoName);
    else
        return false;
}

void sysPlayVideo (KWindow *g_lpKWindow, const char *lpszVideoName, int StartX, int StartY,int Width, int Height, bool Repeat, bool Pause) {
   NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
   KanjiAppDelegate *delegate = (KanjiAppDelegate*)[[UIApplication sharedApplication] delegate];
   UIView *glView = (UIView*)[delegate getGlView];
    CGRect rcScreen;
   NSString *videoName = [[NSString alloc] initWithCString:lpszVideoName encoding:NSISOLatin1StringEncoding];
   NSString *url = [[NSBundle mainBundle] pathForResource:videoName ofType:@"mp4"];
    KWindow *lpKWindow = g_lpKWindow;
    float fScreenWidth = lpKWindow->getDesktopWidth (), fScreenHeight = lpKWindow->getDesktopHeight();
   
   // Stop current video, if any
   sysStopVideo ();
   
    
    
    // Define location of video on screen 
    rcScreen.origin.x = (CGFloat)StartX;
    rcScreen.origin.y = (CGFloat)StartY;
    rcScreen.size.width = (CGFloat)Width;    
    rcScreen.size.height = (CGFloat)Height;    
    rcScreen.origin.x *= (fScreenWidth / 1024);
    rcScreen.origin.y *= (fScreenHeight / 768);
    rcScreen.size.width *= (fScreenWidth / 1024);
    rcScreen.size.height *= (fScreenHeight / 768);


	sysVideoRect.topleft.x = (float)StartX;
	sysVideoRect.topleft.y = (float)StartY;
	sysVideoRect.w = (float)Width;
	sysVideoRect.h = (float)Height;
	
	
   // Create video controller and start playing 
   
   NSLog (@"Video: play %@", url);
   g_playerViewController = [[MPMoviePlayerController alloc] initWithContentURL:[NSURL fileURLWithPath:url]];
   g_playerViewController.view.frame = rcScreen;
    g_playerViewController.view.userInteractionEnabled = false;
    [glView addSubview:g_playerViewController.view];
   g_playerViewController.shouldAutoplay = TRUE;
   if( Repeat )
       g_playerViewController.repeatMode = MPMovieRepeatModeOne;
   else
       g_playerViewController.repeatMode = MPMovieRepeatModeNone;
    g_playerViewController.controlStyle = MPMovieControlStyleNone;
    g_playerViewController.scalingMode = MPMovieScalingModeNone;

    g_playerViewController.view.hidden = YES;
    
    g_VideoName = lpszVideoName;
    
    [g_playerViewController play];
    if( Pause )
        [g_playerViewController pause];//pause needs a prev play call!!
    
   
   [videoName release];
   [pool release];
   
   lpKWindow->processEvents();
   (void) sysHasVideoStopped();
}

float sysVideoPlaybackTime( void )//playback time in secs
{
	if( g_playerViewController ) 
		return (float)g_playerViewController.currentPlaybackTime;
	else
		return 0.0f;	
}

void sysStopVideo (void) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    if (g_playerViewController) {
        // Stop and release video controller 
        
        [g_playerViewController stop];
        [g_playerViewController.view removeFromSuperview];        
        [g_playerViewController release];
        g_playerViewController = nil;
    }
    
    g_VideoName = "";
    Control::IsSysVideoToStop = false;
    prevPauseStatus = false;
    prevHideStatus = false;
    isVideoLoaded = false;
    
    sysVideoRect.topleft.x = (float)0;
	sysVideoRect.topleft.y = (float)0;
	sysVideoRect.w = (float)0;
	sysVideoRect.h = (float)0;
    
    [pool release];
}

bool sysHasVideoStopped (void)
{
    if( g_playerViewController == NULL || !isVideoLoaded || prevPauseStatus || prevHideStatus )//Video need to be loaded to stop it!!
        return false;
    
    //once stopped it will go to MPMoviePlaybackStatePaused state!!
    return (g_playerViewController.playbackState != MPMoviePlaybackStatePlaying);
}

void sysVideoRewind()
{
    if (g_playerViewController)
        g_playerViewController.currentPlaybackTime = 0;
            
}
#endif
