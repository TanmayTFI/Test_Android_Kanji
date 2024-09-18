/**
 * Kanji rendering and I/O engine - Android specific features definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSYSANDROID_H
#define  _KANJI_KSYSANDROID_H

/* Include definitions */
#include "KObjectBase.h"
#include <jni.h>

/* Forward declarations */
class KWindowGLES;
class KWindowGLES2;

/**
 * Class modeling Android specific features
 */

class KSysAndroid : public KObjectBase {
   friend class KWindowGLES;
   friend class KWindowGLES2;
   
public:
   /** Constructor */
   KSysAndroid ();
   
   /** Destructor */
   ~KSysAndroid ();
   
   /**
    * Get Java VM 
    *
    * \return java VM
    */
   static JavaVM *getJVM (void);
   
   /**
    * Execute Java VM garbage collector immediately
    */
   static void collectGarbage (void);
   
   /**
    * Get the preferred language for the device
    *
    * \return language
    */
   static const char *getDeviceLanguage (void);
   
   /**
    * Get unique ID for this device
    *
    * \return id
    */
   static const char *getDeviceUniqueId (void);
   
   /**
    * Get path for storing large files that will be uninstalled when the game is uninstalled. This
    * will be the SDCard if available, otherwise internal storage
    *
    * \return path
    */
   static const char *getStoragePath (void);
   
   /**
    * Get package name, as defined in the android manifest, for instance com.example.mygame
    *
    * \return package name
    */
   static const char *getPackageName (void);
   
   /**
    * Get version name, as defined in the android manifest, for instance 1.0.5
    *
    * \return version name
    */
   static const char *getVersionName (void);
   
   /**
    * Get version code, as defined in the android manifest, for instance 37
    *
    * \return version code
    */
   static long getVersionCode (void);
   
   /**
    * Get the value of a meta-data property in the android manifest. The property must be declared in AndroidManifest.xml inside the <application>
    * tag, such as: <meta-data android:name="testProperty" android:value="bar123456"></meta-data>
    *
    *
    * \param lpszPropertyName name of property to get value of
    *
    * \return value, or "" if not found
    */
   static const char *getPackageMetadata (const char *lpszPropertyName);
   
   /**
    * Get absolute path of the main .obb expansion file
    *
    * \return main obb's absolute path
    */
   static const char *getMainObbExpansionFilePath (void);
   
   /**
    * Get absolute path of the patch .obb expansion file
    *
    * \return patch obb's absolute path
    */
   static const char *getPatchObbExpansionFilePath (void);
   
   /**
    * Get number of free megabytes left on the storage device that supports the .obb expansion files
    *
    * \return number of free megabytes left
    */
   static long getObbExpansionFilesPathFreeMegs (void);
   
   /**
    * Get this device's IPV4 address
    *
    * \param lpAddress returned address if successful
    *
    * \return true if successful, false if failed (for instance if the device is in airplane mode)
    */
   static bool getDeviceIpv4Address (unsigned long *lpAddress);
   
   /**
    * Get approximate screen size in inches
    *
    * \return approximate screen size in inches, -1 if it couldn't be measured
    */
   static long getScreenInches (void);
   
   /**
    * Send e-mail through a mail composer application
    *
    * \param lpszTo list of to: addresses, NULL terminated
    * \param lpszCc list of cc: addresses, NULL terminated
    * \param lpszBcc list of bcc: addresses, NULL terminated
    * \param lpszSubject e-mail subject
    * \param lpszBody e-mail body
    */
   static void sendEmail (const char **lpszTo, const char **lpszCc, const char **lpszBcc, const char *lpszSubject, const char *lpszBody);
   
   /**
    * Open in-game web browser and navigate to the specified URL, if the in-game browser is available, otherwise do nothing. The game is suspended until
    * the browser is dismissed
    *
    * \param lpszUrl url to navigate to
    * \param lpszDismissButtonText text to put on the button used to close the web browser
    */   
   static void webViewBrowse (const char *lpszUrl, const char *lpszDismissButtonText);
   
   /**
    * Show or hide virtual keyboard, if needed
    *
    * \param bShow true to show, false to hide
    *
    * \return true for success, false for failure
    */   
   static bool setVirtualKeyboardStatus (bool bShow);
   
   /**
    * Enable or disable catching a 'back' key event. Back key events are sent with the K_VK_PRIOR keycode
    *
    * \param bCatch true to enable; false to disable, and the game will be minimized when back is tapped (default)
    */
   static void catchBackKey (bool bCatch);
   
   /**
    * Send game activity to the background, producing the same effect as if an uncaught back key is tapped
    */
   static void sendToBack (void);
   
   /**
    * Disable or enable idle timer; when disabled, the device doesn't reduce power, lock, or go to sleep while the game is running. This
    * is useful when the game only uses the accelerometer as input for instance
    *
    * \param bDisable true to disable timer, false to enable
    */
   static void disableIdleTimer (bool bDisable);
   
   /**
    * Play specified video using hardware acceleration. Only one video can be played by the hardware at a time
    *
    * \param lpszVideoFilename filename of video to play
    * \param rcWindow rectangle where the video is to play, in game window coordinates
    * \param nVolume audio volume (0-100)
    * \param bPlayBehindKWindow true to play the video behind the game window, which will need to do a setClearColor to (0,0,0,0) and be
    *                           partially transparent for the video to show through; false to play the video in front of the game window (default)
    * \param bLoopVideo true to loop the video indefinitely, false not to
    */
   static void playVideo (const char *lpszVideoFilename, KRectangle rcWindow, long nVolume, bool bPlayBehindKWindow = false,
                          bool bLoopVideo = false);
   
   /** Stop playing current video, if any */
   static void stopVideo (void);
   
   /**
    * Check if video is currently playing
    *
    * \return true if video is playing, false if it has stopped
    */
   static bool isVideoPlaying (void);
   
   /**
    * Check if video is currently visible
    *
    * \return true if video is visible, false if it isn't
    */
   static bool isVideoVisible (void);
   
   /**
    * Update audio volume of currently playing video
    *
    * \param nVolume new audio volume (0-100)
    */
   static void setVideoVolume (long nVolume);
   
   /**
    * Start or stop displaying a busy-waiting animation
    *
    * \param bShow true to start displaying the animation, false to stop displaying
    * \param lpszMessage message to display together with the animation
    */
   static void showBusyWaiting (bool bShow, const char *lpszMessage = NULL);
   
   /**
    * Show toast message over the game
    *
    * \param lpszText text to show
    * \param bLong true for a long message, false for a short message
    */
   static void showToast (const char *lpszText, bool bLong);
   
   /**
    * Schedule local notification for a specified date and time, to be displayed even if the application isn't
    * running
    *
    * \param nDaysFromNow days added to current date and time for scheduling the notification
    * \param nHours hours added to current date and time, or absolute hour, for scheduling the notification
    * \param nMinutes minutes added to current date and time, or absolute minute, for scheduling the notification
    * \param nSeconds seconds added to current date and time, or absolute seconds, for scheduling the notification
    * \param bAbsoluteTime true if hours, minutes and seconds are absolute, false if they are relative (added to current).
    *        set to true to schedule a notification at a specific hour (for instance during daytime, to avoid waking the user)
    * \param lpszMessage notification message
    * \param nUserData user value, passed back with K_EVENT_LOCAL_NOTIFICATION if the app is woken up from the notification
    *
    * \return true if notification was scheduled, false if failed (on Android, scheduling always succeeds)
    */
   static bool scheduleLocalNotification (long nDaysFromNow, long nHours, long nMinutes, long nSeconds,
                                          bool bAbsoluteTime,
                                          const char *lpszMessage, long nUserData);
   
   /** Cancel scheduled local notifications */
   static void cancelLocalNotifications (void);
   
   /**
    * Get Android event source for certain events (such as K_EVENT_KEYDOWN/KEYUP), while in the event handler
    *
    * \return event source bitmask, for instance InputDevice.SOURCE_TOUCHSCREEN, or 0 for unknown
    */
   static long getEventSource (void);
   
   /**
    * Get ID of device that triggered the event, for certain events (such as K_EVENT_KEYDOWN/KEYUP), while in the event handler
    *
    * \return device ID, 0 for none
    */
   static long getEventDeviceId (void);
   
   /**
    * Get the bitmask of the total event sources supplied by a device
    *
    * \param nDeviceId device ID as returned by getEventDeviceId()
    *
    * \return sources bitmask
    */
   static long getDeviceEventSources (int nDeviceId);
   
   /**
    * Check if all of the specified dynamic permissions are granted, for Android API level 23 (6.0) and up
    *
    * \param lpszPermissions permissions to check for, separated by a semicolon, for instance:
    *                        android.permission.ACCESS_FINE_LOCATION;android.permission.WRITE_EXTERNAL_STORAGE
    *
    * \return true if all permissions are granted, false if not. On Android API levels 22 and less, this always
    *              returns true
    */
   static bool checkPermissions (const char *lpszPermissions);
   
   /**
    * Request dynamic permissions, for Android API level 23 (6.0) and up
    *
    * \param lpszPermissions permissions to request, separated by a semicolon, for instance:
    *                        android.permission.ACCESS_FINE_LOCATION;android.permission.WRITE_EXTERNAL_STORAGE
    * \param lpszRationale text to show the user when requesting the permissions, if they previously refused them
    *
    * \return true if any permissions was granted by the user, false if not. On Android API levels 22 and less, this always
    *              returns true and shows no dialog
    */
   static bool requestPermissions (const char *lpszPermissions, const char *lpszRationale);
   
private:
   /** Suspend all activity, for instance a native video when losing focus */
   static void suspend (void);
   
   /** Resume all activity, for instance a native video when losing focus */
   static void resume (void);
   
   /** Update native video logic, if needed */
   static void updateVideo (void);
};

#endif   /* _KANJI_KSYSANDROID_H */
