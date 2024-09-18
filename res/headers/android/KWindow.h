/**
 * Kanji rendering and I/O engine - base class for window definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_WINDOW_H
#define  _KANJI_WINDOW_H

/* Include definitions */
#include "KObjectBase.h"
#include "KInput.h"
#include "KMath3d.h"

/** Default window title */
#define  _KANJI_DEFAULT_TITLE ""

/** Event type */
enum {
   K_EVENT_NULL = 0,                /**< Empty event */
   K_EVENT_MOUSEMOVE = 1,           /**< Mouse moved */
   K_EVENT_MOUSEUP = 2,             /**< Mouse button released */
   K_EVENT_MOUSEDOWN = 3,           /**< Mouse button pressed */
   K_EVENT_CHAR = 5,                /**< Character input */
   K_EVENT_KEYDOWN = 6,             /**< Key pressed */
   K_EVENT_KEYUP = 7,               /**< Key released */
   K_EVENT_DBLCLICK = 8,            /**< Mouse double-clicked */
   K_EVENT_WHEEL = 11,              /**< Mouse wheel moved */
   K_EVENT_CONTEXTMENU = 12,        /**< Context menu activated */
   K_EVENT_ACTIVATED = 13,          /**< Window gained focus */
   K_EVENT_DEACTIVATED = 14,        /**< Window lost focus */
   K_EVENT_MONITORPOWEROFF = 15,    /**< Monitor powered off for screen saving purposes */
   K_EVENT_UNICHAR = 50,            /**< Unicode character input */
   K_EVENT_TOUCH = 100,             /**< Touchscreen pressed or released (iPhone, iPad, Android, or touchscreen simulation) */
   K_EVENT_ACCEL = 101,             /**< Accelerometer position changed (iPhone, iPad, Android, or touchscreen simulation) */
   K_EVENT_LOW_MEMORY = 102,        /**< Game received low memory warning (iPhone and iPad) */
   K_EVENT_ORIENTATION = 103,       /**< Device orientation changed (iPhone and iPad) */
   K_EVENT_SONGPICKER = 104,        /**< iPod song player status changed (iPhone and iPad) */
   K_EVENT_TEXTFIELD = 105,         /**< KSysiPhoneTextField event (iPhone and iPad) */
   K_EVENT_DBLTAP = 106,            /**< Touchscreen tapped twice (iPhone, iPad, Android, or touchscreen simulation) */
   K_EVENT_DRAW_DECORATIONS = 107,  /**< When aspect correction is enabled, time to draw decorations in place of black bars */
   K_EVENT_APPLE_GAMECENTER = 108,  /**< Game center notification (MacOS X, iPhone and iPad) */
   K_EVENT_IOS_GAMECENTER = K_EVENT_APPLE_GAMECENTER,
   K_EVENT_IOS_MAILCOMPOSER = 109,  /**< Mail composer dismissed (iPhone and iPad) */
   K_EVENT_APPLE_STORE = 110,       /**< In-app purchase (MacOS X, iPhone and iPad) */
   K_EVENT_IOS_STORE = K_EVENT_APPLE_STORE,
   K_EVENT_BLACKBERRY_STORE = K_EVENT_APPLE_STORE, /**< In-app purchase (Blackberry) */
   K_EVENT_GESTURE_SWIPE = 111,     /**< Swipe gesture */
   K_EVENT_GESTURE_ZOOM = 112,      /**< Zoom gesture */
   K_EVENT_KEYBOARD_DISMISS = 113,  /**< Hardware of virtual keyboard dismissed (Android, WebOS and Blackberry) */
   K_EVENT_ACHIEVEMENT_STATUS = 114, /**< Achievement status received (Blackberry) */
   K_EVENT_LEADERBOARD_SCORE = 115, /**< Leaderboard score received (Blackberry) */
   K_EVENT_VIDEO_ENDED = 116,       /**< Native video finished playing */
   K_EVENT_GESTURE_FLING = 117,     /**< Fling gesture */
   K_EVENT_RELOAD_ASSETS = 118,     /**< Reload all GPU assets now (if the game has created some fully on its own; normally not) */
   K_EVENT_SCREENSAVER_START = 119, /**< Screensaver started (for instance the dream mode on Android) */
   K_EVENT_SCREENSAVER_STOP = 120,  /**< Screensaver stopped (for instance the dream mode on Android) */
   K_EVENT_APP_TERMINATE = 121,     /**< Application will quit now */
   K_EVENT_HTTP_REQUEST = 122,      /**< HTTP request completed */
   K_EVENT_LOCAL_NOTIFICATION = 123,/**< Local notification */
   K_EVENT_LOCAL_NOTIFICATION_SETTINGS = 124, /**< Local notification settings updated */
   K_EVENT_JOYSTICK = 125,          /**< Joystick/game controller event */
};

/** Mouse button index */
enum {
   K_LBUTTON = 1,                   /**< Left */
   K_RBUTTON = 2,                   /**< Right */
   K_MBUTTON = 3,                   /**< Middle */
};

/** Touch event type */
enum {
   K_TOUCHBEGINS = 0,               /**< Begins */
   K_TOUCHCONTINUES = 1,            /**< Continues */
   K_TOUCHENDS = 2,                 /**< Ends */
   K_TOUCHCANCELLED = 3,            /**< Cancelled */
};

/** Joystick event type */
enum {
   K_JOYSTICKAXIS = 0,              /**< Stick axis value changed */
   K_JOYSTICKDPAD = 1,              /**< D-PAD direction pressed state changed */
   K_JOYSTICKBUTTON = 2,            /**< Button pressed state changed */
   K_JOYSTICKPAUSE = 3,             /**< Pause toggle */
   K_JOYSTICKCHANGE = 4,            /**< Joysticks connected or disconnected */
};

/** Joystick D-PAD direction */
enum {
   K_JOYSTICKDPAD_UP = 0,
   K_JOYSTICKDPAD_DOWN,
   K_JOYSTICKDPAD_LEFT,
   K_JOYSTICKDPAD_RIGHT,
};

/** Joystick axis */
enum {
   K_JOYSTICKAXIS_X = 0,
   K_JOYSTICKAXIS_Y,
};

/** KSysiPhoneTextField event */
enum {
   K_TEXTFIELD_EDITBEGINS = 0,      /**< Began editing */
   K_TEXTFIELD_EDITENDS = 1,        /**< Done editing */
};
   
/** MacOSX / iOS game center event */
enum {
   K_APPLE_GAMECENTER_AUTHENTICATED = 1,        /**< Player logged in */
   K_IOS_GAMECENTER_AUTHENTICATED = K_APPLE_GAMECENTER_AUTHENTICATED,
   K_APPLE_GAMECENTER_AUTHFAILURE = 2,          /**< Authentication failure */
   K_IOS_GAMECENTER_AUTHFAILURE = K_APPLE_GAMECENTER_AUTHFAILURE,
   K_APPLE_GAMECENTER_ACHIEVEMENTS = 3,         /**< Achievements downloaded */
   K_IOS_GAMECENTER_ACHIEVEMENTS = K_APPLE_GAMECENTER_ACHIEVEMENTS,
   K_APPLE_GAMECENTER_ACHIEVEMENTS_DESCR = 4,   /**< Achievement descriptions downloaded */
   K_IOS_GAMECENTER_ACHIEVEMENTS_DESCR = K_APPLE_GAMECENTER_ACHIEVEMENTS_DESCR,
};

/** MacOSX / iOS in-app purchase event */
enum {
   K_APPLE_STORE_PRODUCT = 1,         /**< Product information received */
   K_IOS_STORE_PRODUCT = K_APPLE_STORE_PRODUCT,
   K_APPLE_STORE_PURCHASED = 2,       /**< Product purchased */
   K_IOS_STORE_PURCHASED = K_APPLE_STORE_PURCHASED,
   K_APPLE_STORE_CANCELLED = 3,       /**< Transaction cancelled */
   K_IOS_STORE_CANCELLED = K_APPLE_STORE_CANCELLED,
   K_APPLE_STORE_ENDRESTORE = 4,      /**< End of restored transactions */
   K_IOS_STORE_ENDRESTORE = K_APPLE_STORE_ENDRESTORE,
};

/** Gesture event type */
enum KWindowEventGesture {
   K_GESTUREBEGINS = 0,             /**< Begins */
   K_GESTURECONTINUES = 1,          /**< Continues */
   K_GESTUREENDS = 2,               /**< Ends */
};

/** HTTP request event type */
enum KWindowEventHttpRequest {
   K_HTTPSUCCESS = 1,               /**< Request succeeded */
   K_HTTPFAILURE,                   /**< Request failed */
};

/** System event */
typedef struct KEvent {
   /** Event type */
   long type;
   
   /** X coordinate of the mouse */
   long mouseX;
   
   /** Y coordinate of the mouse */
   long mouseY;
   
   /** Button that was pressed or released */
   long buttonIndex;
   
   /** Mouse wheel position */
   short wheel;
   
   /** Character that was input */
   char character;
   
   /** System-dependent virtual code of the key that was pressed or released */
   long virtualKey;
   
   /** System-dependent event code as received in the OS message's contents */
   long nativeMessage;
   
   /** Portable code of the key that was pressed or released */
   EKeyboardLayout ptkKey;
   
   /** System-dependent event data as received in the OS message's contents */
   long nativeData;
   
   /** Unicode character that was input (K_EVENT_UNICHAR events) */
   unsigned long unicodeChar;

   /** Touch event type (K_EVENT_TOUCH): K_TOUCHxxx */
   long touchType;

   /** Current number of touches on the screen (K_EVENT_TOUCH) */
   long touchCurrent;
   
   /** Index of this touch event in a set of multitouch events being reported (K_EVENT_TOUCH) */
   long touchIndex;
   
   /** Number of touch events in a set of multitouch events being reported (K_EVENT_TOUCH) */
   long touchCount;
   
   /** X coordinate of a touch (K_EVENT_TOUCH) */
   long touchX;
   
   /** X coordinate of a touch (K_EVENT_TOUCH) */
   long touchY;
   
   /** Force of a touch, 0..1 if supported, otherwise always 0 (K_EVENT_TOUCH) */
   float touchZ;
   
   /** Previous X coordinate of a touch (K_EVENT_TOUCH) */
   long prevTouchX;
   
   /** Previous X coordinate of a touch (K_EVENT_TOUCH) */
   long prevTouchY;
   
   /** X position of accelerometer (K_EVENT_ACCEL events) */
   float accelX;
   
   /** Y position of accelerometer (K_EVENT_ACCEL events) */
   float accelY;
   
   /** Z position of accelerometer (K_EVENT_ACCEL events) */
   float accelZ;
   
   /** Device orientation (K_EVENT_ORIENTATION events) */
   long devOrientation;
   
   /** KSysiPhoneTextField event type (K_EVENT_TEXTFIELD events): K_TEXTFIELD_xxx */
   long textFieldEventType;
   
   /** iOS Game Center event type (K_IOS_GAMECENTER_xxx) */
   long gameCenterEventType;
   
   /** Number of achievements downloaded for K_IOS_GAMECENTER_ACHIEVEMENTS */
   long gameCenterAchievementsCount;
   
   /** Result from mail composer for K_EVENT_IOS_MAILCOMPOSER */
   long mailComposerResult;
   
   /** In-app purchase event type (K_APPLE_STORE_xxx) for K_EVENT_APPLE_STORE */
   long storeEventType;
   
   /** Product ID for K_EVENT_APPLE_STORE */
   const char *storeProductId;
   
   /** Product ID for K_EVENT_APPLE_STORE / K_APPLE_STORE_PRODUCT */
   const char *storeProductTitle;
   
   /** Product ID for K_EVENT_APPLE_STORE / K_APPLE_STORE_PRODUCT */
   const char *storeProductDescription;
   
   /** Price for K_EVENT_APPLE_STORE / K_APPLE_STORE_PRODUCT */
   double storeProductPrice;
   
   /** Price locale identifier for K_EVENT_APPLE_STORE / K_APPLE_STORE_PRODUCT */
   const char *storeProductLocaleId;
   
   /** Price in the device's local currency, for K_EVENT_APPLE_STORE / K_APPLE_STORE_PRODUCT */
   const char *storeProductLocalizedPrice;
   
   /** Type of event for K_EVENT_GESTURE_xxx */
   long gestureType;
   
   /** Starting X coordinate for K_EVENT_GESTURE_SWIPE */
   long swipeStartX;
   
   /** Starting Y coordinate for K_EVENT_GESTURE_SWIPE */
   long swipeStartY;
   
   /** X delta for K_EVENT_GESTURE_SWIPE */
   long swipeDeltaX;
   
   /** Y delta for K_EVENT_GESTURE_SWIPE */
   long swipeDeltaY;
   
   /** Zoom factor for K_EVENT_GESTURE_ZOOM */
   float zoomFactor;
   
   /** X coordinate of center for K_EVENT_GESTURE_ZOOM */
   long zoomCenterX;
   
   /** Y coordinate of center for K_EVENT_GESTURE_ZOOM */
   long zoomCenterY;
   
   /** Number of touches for K_EVENT_GESTURE_FLING */
   long flingTouchCurrent;
   
   /** Angle in degrees for K_EVENT_GESTURE_FLING */
   float flingAngle;
   
   /** Duration of the gesture in milliseconds for K_EVENT_GESTURE_FLING */
   long flingMillis;
   
   /** HTTP event type (K_EVENT_HTTP_REQUEST) */
   long httpEventType;
   
   /** HTTP request ID (K_EVENT_HTTP_REQUEST) */
   long httpRequestId;
   
   /** HTTP response code(K_EVENT_HTTP_REQUEST) */
   long httpResponseCode;
   
   /** HTTP request response data (K_EVENT_HTTP_REQUEST) */
   const void *httpResponseData;
   
   /** Size of HTTP request response, in bytes (K_EVENT_HTTP_REQUEST) */
   size_t httpResponseSize;
   
   /** User data for K_EVENT_LOCAL_NOTIFICATION */
   long notificationUserData;
   
   /** Joystick event type (K_EVENT_JOYSTICK) */
   long joystickType;
   
   /** ID of the joystick that sent the event, 0..n (K_EVENT_JOYSTICK) */
   long joystickIndex;
   
   /** Joystick D-PAD direction for K_JOYSTICKDPAD, such as K_JOYSTICKDPAD_UP (K_EVENT_JOYSTICK) */
   long joystickDirection;
   
   /** Joystick button index, 0..n for K_JOYSTICKBUTTON, for instance K_JOYBUTTON_A. Defined in KInput.h (K_EVENT_JOYSTICK) */
   long joystickButtonType;
   
   /** Joystick direction or button press value for K_JOYSTICKDPAD/K_JOYSTICKBUTTON, ISDOWN if down, ISUP if up (K_EVENT_JOYSTICK) */
   bool joystickPress;
   
   /** Joystick hat number for K_JOYSTICKAXIS, 0..n (K_EVENT_JOYSTICK) */
   long joystickHat;
   
   /** Joystick axis type for K_JOYSTICKAXIS, such as K_JOYSTICKAXIS_X (K_EVENT_JOYSTICK) */
   long joystickAxisType;
   
   /** Joystick axis value for K_JOYSTICKAXIS, in the range of -1.0f to 1.0f (K_EVENT_JOYSTICK) */
   float joystickAxisValue;
} KEvent;

/** Portable event callback */
typedef bool (*ptkNativeCallBak) (KEvent *lpEvent);

/** Correct spelling of ptkNativeCallBak - both are present for source compatibility purposes */
#define ptkNativeCallBack  ptkNativeCallBak

/** Image formats */
enum {
   K_IMAGE_UNKNOWN = -1,   /**< Unknown */
   K_IMAGE_BMP = 0,        /**< BMP */
   K_IMAGE_JPG = 1,        /**< Jpeg */
   K_IMAGE_TGA = 2,        /**< Targa */
   K_IMAGE_PNG = 3,        /**< PNG */
   K_IMAGE_GIF = 4,        /**< GIF */
   K_IMAGE_JEM = 5,        /**< JEM */
};

/** Display adapter features that can be checked by setRequirements() */
#define  K_WINDOW_FEATURE_RENDERTARGET 1  /**< Display adapter must support render targets */

/** Orientation (iPhone, iPad and Android) */
enum {
   K_PORTRAIT = 0,         /**< Portrait */
   K_LANDSCAPE = 1,        /**< Landscape */
   K_RPORTRAIT = 2,        /**< Reverse portrait */
   K_RLANDSCAPE = 3,       /**< Reverse landscape */
};
   
/** 3d culling */
enum KWindow3dCulling {
   K_CULLING_NONE = 0,     /**< No culling */
   K_CULLING_CW,           /**< Clockwise */
   K_CULLING_CCW,          /**< Counter-clockwise */
};

/** 3d transform */
enum KWindow3dTransform {
   K_TRANSFORM_PROJECTION = 0,   /**< Projection */
   K_TRANSFORM_CAMERA,           /**< Camera */
   K_TRANSFORM_WORLD,            /**< World */
};

/** 3d light type */
enum KWindow3dLightType {
   K_LIGHT_POINT = 0,            /**< Point light */
   K_LIGHT_DIR,                  /**< Directional light */
};

/** 3d light parameters */
typedef struct KWindow3dLight {
   KWindow3dLightType nType;     /**< Light type */
   float fColor[4];              /**< Diffuse color r, g, b, a */
   KVector3 vPosition;           /**< Position in world coordinates (for K_LIGHT_POINT) */
   KVector3 vDirection;          /**< Light direction (for K_LIGHT_DIR) */
   float fConstAttenuation;      /**< Constant attenuation factor (typically set to 1): attenuation = 1 / ( fConstAttenuation + fLinearAttenuation * d + fQuadAttenuation * d^2) */
   float fLinearAttenuation;     /**< Linear attenuation factor (typically set to 0) */
   float fQuadAttenuation;       /**< Quadratic attenuation factor (typically set to 0) */
} KWindow3dLight;

/* Forward declarations */
class KPixelShader;

/**
 * Base class for the game window
 */

class KWindow : public KObjectBase {
public:
   /** Destructor */
   virtual ~KWindow () { ; }

   /**
    * Enable or disable 4:3 aspect correction on widescreen displays. This method must be called before createGameWindow()
    *
    * \param bFixed true to maintain a 4:3 aspect ratio in fullscreen mode on widescreen displays (enable aspect correction), false to stretch a 4:3 fullscreen window to the whole screen (disable)
    */
   virtual void setFixedAspectRatio (bool bFixed) = 0;

   /**
    * Set minimum requirements. This method must be called before createGameWindow()
    *
    * \param nTexSize minimum texture width and height, in pixels, that the display adapter must support (defaults to 512)
    * \param nVidMemBytes minimum amount of VRAM that the display adapter must have (defaults to 4 * 1024 * 1024)
    * \param nFeatures features that the display adapter must support (K_WINDOW_FEATURE_xxx; defaults to 0)
    */
   virtual void setRequirements (long nTexSize, unsigned long nVidMemBytes, long nFeatures) = 0;

   /**
    * Get width of the user's desktop where the game window will appear or has appeared, in pixels
    *
    * \return desktop width in pixels
    */
   virtual long getDesktopWidth (void) = 0;
   
   /**
    * Get height of the user's desktop where the game window will appear or has appeared, in pixels
    *
    * \return desktop height in pixels
    */
   virtual long getDesktopHeight (void) = 0;
   
   /**
    * Get display frequency of the user's desktop, in Hertz
    *
    * \return display frequency, in hertz
    */
   virtual long getDesktopDisplayFrequency (void) = 0;
   
   /**
    * Check if a game window of the specified dimensions will fit on the desktop, including the extra space
    * required for the title bar and other decorations.
    *
    * \param nWidth desired width of the actual window contents, in pixels
    * \param nHeight desired height of the actual window contents, in pixels
    *
    * \return true if the window will fit on the desktop and windowed mode can be used, false if it won't fit and it
    *         it is recommended to use fullscreen mode
    */
   virtual bool checkWindowedMode (long nWidth, long nHeight) = 0;
   
   /**
    * Set bias for mipmapping, when mipmap generation is enabled
    *
    * \param fMipMapBias mipmap bias (negative is sharper)
    */
   virtual void setMipMapBias (float fMipMapBias) = 0;
   
   /**
    * Create game window
    *
    * \param nWidth width of the actual window contents, in pixels
    * \param nHeight height of the actual window contents, in pixels
    * \param nDepth window depth, in bits per pixel (-1 for default depth)
    * \param bWindowed true for windowed mode, false for fullscreen mode
    * \param lpszWindowTitle initial window title
    * \param nOrientation orientation of the game window (for iPhone and iPad; ignored on other platforms)
    *
    * \return true if the game window was succesfully created, false for failure
    */
   virtual bool createGameWindow (long nWidth, long nHeight, long nDepth, bool bWindowed, const char *lpszWindowTitle = _KANJI_DEFAULT_TITLE, long nOrientation = K_LANDSCAPE) = 0;

   /**
    * Switch to windowed or fullscreen mode
    *
    * \param bFullscreen true to switch to fullscreen mode, false to switch to windowed mode
    *
    * \return true for success, false for failure
    */
   virtual bool toggleFullScreen (bool bFullscreen) = 0;

   /**
    * Change the game window's title
    *
    * \param lpszWindowTitle new title
    */
   virtual void setTitle (const char *lpszWindowTitle) = 0;

   /**
    * Set the game window's gamma
    *
    * \param fGamma current gamma (0..1)
    */
   virtual void setGamma (float fGamma) = 0;

   /**
    * Set new window orientation
    *
    * \param nOrientation new orientation
    */
   virtual void setOrientation (long nOrientation) = 0;
   
   /**
    * Enable or disable multitouch events (iPhone and iPad)
    *
    * \param bEnable true to enable multitouch, false to disable (default)
    */
   virtual void enableMultiTouch (bool bEnable) = 0;
   
   /**
    * Check if the player has requested to close the game window, and the game should quit
    *
    * \return true if closed, false if still open
    */
   virtual bool isQuit (void) = 0;

   /**
    * Set whether the player has requested to close the game window
    *
    * \param bQuitState true to flag the game window as closing, false to flag it as open, as will be reported by isQuit()
    */
   virtual void setQuit (bool bQuitState) = 0;

   /**
    * Tell the window to close and send a quit message to the game
    */
   virtual void terminate (void) = 0;

   /**
    * Check if the game window currently has the input focus
    *
    * \return true if the window has the focus (input), false if not
    */
   virtual bool hasFocus (void) = 0;

   /**
    * Check whether the renderer of this game window uses hardware acceleration for drawing
    *
    * \return true if drawing is hardware accelerated, false if drawn in software
    */
   virtual bool getAccelerationCap (void) = 0;

   /**
    * Check if user's desktop is in a widescreen resolution (aspect ratio higher than 4:3) and it makes sense
    * to offer an aspect correction setting in the game
    *
    * \return true if desktop is widescreen, false if 4:3
    */
   virtual bool isWidescreenAvailable (void) = 0;

   /**
    * Minimize the game window to the taskbar
    */
   virtual void minimize (void) = 0;

   /**
    * Restore the minimized game window to its original size
    */
   virtual void restore (void) = 0;

   /**
    * Get logical width of the game window contents, in pixels, regardless of aspect correction; this will be the same as the width passed to createGameWindow()
    *
    * \return logical width, in pixels
    */
   virtual long getWindowWidth (void) = 0;

   /**
    * Get logical height of the game window contents, in pixels, regardless of aspect correction; this will be the same as the width passed to createGameWindow()
    *
    * \return logical height, in pixels
    */
   virtual long getWindowHeight (void) = 0;

   /**
    * Get actual width of the game window contents (will be different than the width passed to createGameWindow() if aspect correction is in use)
    *
    * \return actual width, in pixels
    */
   virtual long getActualWindowWidth (void) = 0;

   /**
    * Get actual height of the game window contents (will be different than the height passed to createGameWindow() if aspect correction is in use)
    *
    * \return actual height, in pixels
    */
   virtual long getActualWindowHeight (void) = 0;

   /**
    * Get actual number of bits per pixel used to draw the game window
    *
    * \return actual number of bits per pixel
    */
   virtual long getWindowDepth (void) = 0;
   
   /**
    * Get size of total RAM that can be used for textures, in kibibytes. This method must be called after createGameWindow()
    *
    * \return size of total texture RAM in kibibytes
    */
   virtual long getVidMem (void) = 0;

   /**
    * Get size of currently available RAM for textures, in kibibytes. This method must be called after createGameWindow()
    *
    * \return size of currently available texture RAM in kibibytes
    */
   virtual long getAvailVidMem (void) = 0;

   /**
    * Get the name of the display adapter used for rendering this game window. This method must be called after createGameWindow()
    *
    * \param lpBuffer buffer where to store the display adapter's name (will be zero-terminated)
    * \param nBufferLen size of buffer in characters (not bytes)
    */
   virtual void getAdapterName (char *lpBuffer, long nBufferLen) = 0;

   /**
    * Get the maximum hardware texture size that the display adapter supports. This method must be called after createGameWindow()
    *
    * \param nMaxWidth returned maximum width, in pixels
    * \param nMaxHeight returned maximum height, in pixels
    *
    * \return true if information was returned, false if it wasn't (maximum size is unknown)
    */
   virtual bool getMaxTextureSize (long &nMaxWidth, long &nMaxHeight) = 0;

   /**
    * Check whether the display adapter used to render the game window supports non-square textures. This method must be called after createGameWindow()
    *
    * \return true if display adapter supports non-square textures, false if it doesn't
    */
   virtual bool getRectangleTexCap (void) = 0;

   /**
    * Force the use of non-square textures. Ignored by Kanji and included for source compatibility.
    */
   virtual void setRectangleTexCap (bool bForceRectangleCap) = 0;

   /**
    * Throttle the renderer's framerate
    *
    * \param nDesiredFrameRate target framerate: -1 to disable vsync and throttling, 0 to enable vsync and disable throttling, > 0 for a specific framerate (for instance 60 for 60fps)
    */
   virtual void setMaxFrameRate (long nDesiredFrameRate) = 0;

   /**
    * Reset the clock for evaluating the time elapsed in the last frame. When using getFrameTime(), this method must be called before entering a rendering loop 
    */
   virtual void resetFrameTime (void) = 0;
   
   /**
    * Get how much time has elapsed since the last frame
    *
    * \return time elapsed, in milliseconds
    */
   virtual double getFrameTime (void) = 0;
   
   /**
    * Set color to use for clearing the window with setWorldView()
    *
    * \param r red component, 0..1
    * \param g green component, 0..1
    * \param b blue component, 0..1
    * \param a alpha component, 0..1 (ignored for the backbuffer, taken into account when drawing to a render target)
    */
   virtual void setClearColor (float r, float g, float b, float a) = 0;

   /**
    * Set current world transform
    *
    * \param fXTranslate translation over the X axis, in pixels
    * \param fYTranslate translation over the Y axis, in pixels
    * \param fZRotation angle of rotation over the Z axis, in degrees
    * \param fScale scaling factor (1.0f for 1:1 texel to pixel conversion)
    * \param bClearWorld true to clear the game window using the clear color, false not to
    */
   virtual void setWorldView (float fXTranslate, float fYTranslate, float fZRotation, float fScale, bool bClearWorld) = 0;

   /**
    * Set default world transform: no translation, no rotation, 1.0 scale and not clearing the window
    */
   virtual void setDefaultWorldView (void) = 0;

   /**
    * Set current projection transform as a matrix
    *
    * \param lpMatrix matrix to be applied as the current projection transform
    */
   virtual void setProjectionMatrix (KMatrix *lpMatrix) = 0;
   
   /**
    * Set current world transform as a matrix
    *
    * \param lpMatrix matrix to be applied as the current world transform
    */
   virtual void setWorldMatrix (KMatrix *lpMatrix) = 0;
   
   /**
    * Set clipping rectangle, in game window coordinates. When enabled, no drawing will occur outside of the specified rectangle
    *
    * \param bEnable true to enable, false to disable
    * \param x1 X coordinate of top, left of clipping rectangle, in game window coordinates
    * \param y1 Y coordinate of top, left of clipping rectangle, in game window coordinates
    * \param x2 X coordinate + 1 of bottom, right of clipping rectangle, in game window coordinates
    * \param y2 Y coordinate + 1 of bottom, right of clipping rectangle, in game window coordinates
    */
   virtual void setClipRect (bool bEnable, float x1, float y1, float x2, float y2) = 0;
   
   /**
    * Enable stencil testing. The window must have been created with a stencil buffer
    *
    * \param bClearStencilBuffer true to clear stencil buffer, false to leave its contents as they are
    * \param bEnableStencilWrites true to draw to the stencil buffer, false to draw to the color buffer
    * \param fMinAlpha when drawing to the stencil buffer, minimum alpha value to set a pixel in the stencil buffer
    * \param bReverse once the stencil buffer has been written to, true to only allow drawing pixels to the color buffer that don't match the stencil mask;f
    *                 alse to only allow drawing pixels that match the stencil mask
    */
   virtual void beginStencil (bool bClearStencilBuffer, bool bEnableStencilWrites, float fMinAlpha, bool bReverse) = 0;
   
   /**
    * Disable stencil testing
    */
   virtual void endStencil (void) = 0;
   
   /**
    * Begin rendering 3d triangles
    *
    * \param bClearDepthBuffer true to clear the depth buffer, false not to
    * \param bEnableDepthWrites true to enable writes to the depth buffer, false to disable them
    * \param nCullingMode triangle culling mode
    * \param bFrontToBack true if rendering front to back with alpha blending turned off
    */
   virtual void begin3d (bool bClearDepthBuffer, bool bEnableDepthWrites, KWindow3dCulling nCullingMode,
                         bool bFrontToBack = false) = 0;
   
   /**
    * Set depth (Z coordinate) to use for 2d blits
    *
    * \param fDepth new depth
    */
   virtual void setBlitDepth (float fDepth) = 0;
   
   /**
    * Set 3d projection, camera or world transformation matrix
    *
    * \param n3dTransform index of transform to be set (K_TRANSFORM_xxx)
    * \param lpMatrix matrix to be applied as the current projection
    */
   virtual void set3dMatrix (long n3dTransform, KMatrix *lpMatrix) = 0;
   
   /**
    * Set ambient light for 3d rendering
    *
    * \param r red component, 0..1
    * \param g green component, 0..1
    * \param b blue component, 0..1
    * \param a alpha component, 0..1
    */
   virtual void set3dAmbientLight (float r, float g, float b, float a) = 0;
   
   /**
    * Enable or disable 3d light and set its parameters
    *
    * \param nLightIdx index of 3d light to be set and enabled, or disabled (0-7)
    * \param lpLight parameters of light to be enabled, or NULL to disable
    */
   virtual void set3dLight (long nLightIdx, KWindow3dLight *lpLight) = 0;
   
   /**
    * Set current 3d diffuse material color
    *
    * \param r red component, 0..1
    * \param g green component, 0..1
    * \param b blue component, 0..1
    * \param a alpha component, 0..1
    */
   virtual void set3dMatColor (float r, float g, float b, float a) = 0;
   
   /**
    * Stop rendering 3d triangles
    */
   virtual void end3d (void) = 0;
   
   /**
    * Restore drawing state after the game has drawn directly to DirectX or OpenGL
    */
   virtual void restoreState (void) = 0;
   
   /**
    * Poll and process system events
    */
   virtual void processEvents (void) = 0;

   /**
    * Switch the contents of the back and front buffers so as to present the frame that was being drawn, and start drawing a new frame in the backbuffer
    *
    * \param bWaitForFocus true to suspend the game if focus is lost, until focus is restored; false not to
    * \param bProcessEvents true to process input events now, false to ignore them
    */
   virtual void flipBackBuffer (bool bWaitForFocus = true, bool bProcessEvents = true) = 0;

   /**
    * Install portable window event handler
    *
    * \param lpPortableHandler portable event handler, NULL to remove
    */
   virtual void setPTKCallBack (ptkNativeCallBak lpPortableHandler) = 0;
   
   /**
    * Enable or disable displaying software (blitted) mouse cursor
    *
    * \param bEnableSoftCursor true to enable software mouse cursor, false to disable
    */
   virtual void displayMouse (bool bEnableSoftCursor) = 0;

   /**
    * Is displaying software (blitted) mouse cursor?
    */
   virtual bool isDisplayingMouse (void) = 0;
   
   /**
    * If the game is hiding the OS cursor and drawing its own software one itself, not using displayMouse(), check if
    * the game's cursor should be drawn for the current frame. Kanji switches to the system cursor when moving out of the
    * game window, even when the system cursor is hidden, for the user's convenience, and the game software cursor shouldn't
    * be drawn then
    *
    * \return true if game's software cursor should be drawn, false if not
    */
   virtual bool canDisplayMouse (void) = 0;
   
   /**
    * Save the back buffer's contents as an image
    *
    * \param lpszFilename name of image to save backbuffer as
    * \param nImageFormat format to use for image (K_IMAGE_xxx)
    * \param nWidth width to resize image as (0 to use the backbuffer width)
    * \param nHeight height to resize image as (0 to use the backbuffer height)
    * \param nCropX X coordinate of top, left of cropping rectangle
    * \param nCropY Y coordinate of top, left of cropping rectangle
    * \param nCropWidth width of cropping rectangle (0 to use the backbuffer width)
    * \param nCropHeight height of cropping rectangle (0 to use the backbuffer height)
    *
    * \return true for success, false for failure
    */
   virtual bool saveBackBuffer (const char *lpszFilename, long nImageFormat = K_IMAGE_BMP, long nWidth = 0, long nHeight = 0, long nCropX = 0, long nCropY = 0, long nCropWidth = 0, long nCropHeight = 0) = 0;

   /** Make backbuffer the active render target - internal */
   virtual void bindBackbuffer (void) = 0;
   
   /**
    * Set specified pixel shader as active for the next blits
    *
    * \param lpPixelShader pixel shader to set, NULL to use the default fixed-function pipeline
    */
   virtual void setPixelShader (KPixelShader *lpPixelShader) = 0;
   
   /**
    * Set value of a parameter in the currently active pixel shader
    *
    * \param nParam index of parameter to set
    * \param fValue value (as four floats)
    */
   virtual void setPixelShaderParam (long nParam, float fValue[4]) = 0;
   
   /**
    * Get memory size currently used for textures
    *
    * \return total in bytes
    */
   virtual size_t getTotalTextureBytes (void) = 0;
   
   /**
    * Check if the game is being run on a touchscreen platform (real or simulated with simulateTouchscreen)
    *
    * \return true if touchscreen, false if mouse-based desktop
    */
   virtual bool isTouchscreen (void) = 0;   
   
   /**
    * Simulate a touchscreen platform such as the iPhone/iPad, on a desktop platform such as Windows and Mac, by sending simulated K_EVENT_TOUCH
    * events as well as re-generating emulated mouse events from them, like a touchscreen platform does. This method is ignored on an
    * actual touchscreen platform
    *
    * \param bEnable true to simulate, false for normal operation
    */
   virtual void simulateTouchscreen (bool bEnable) = 0;   
   
   /**
    * Get screen scaling factor
    *
    * \return scaling factor (2.0f for Retina displays on iPhone 4)
    */
   virtual float getScreenScale (void) = 0;   
   
   /**
    * Dispatch event to the game - internal
    *
    * \private
    *
    * \param lpEvent event to be dispatched
    */
   virtual void dispatchEvent (KEvent *lpEvent) = 0;
   
   /**
    * Enable or disable writing to the alpha component of the framebuffer
    *
    * \param bEnable true to enable, false to disable
    */
   virtual void enableAlphaWrites (bool bEnable) = 0;
   
   /**
    * Get orientation currently set by the game
    *
    * \return current orientation
    *
    * \private
    */
   virtual long getOrientation (void) = 0;
};

#endif   /* _KANJI_WINDOW_H */
