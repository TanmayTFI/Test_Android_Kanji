/**
 * Kanji rendering and I/O engine - Android OpenGL ES 1.x window definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_WINDOWGLES_H
#define  _KANJI_WINDOWGLES_H

/* Include definitions */
#include "KObjectBase.h"
#include "KWindow.h"
#include <GLES/gl.h>
#include <GLES/glext.h>

/* Forward declarations */
class KGraphicGLES;
class KBatchGLES;
class KInput;
class KMatrix;
class KPTK;

/**
 * Base class for the game window
 */

class KWindowGLES : public KWindow {
   friend class KGraphicGLES;
   friend class KBatchGLES;
   friend class KInput;
   friend class KPTK;

public:
   /**
    * Constructor
    *
    * \param bCreateDepthBuffer true to create a depth buffer (Z-buffer) for the game window, false not to
    * \param bCreateStencilBuffer true to create a stencil buffer for the window, false not to
    */
   KWindowGLES (bool bCreateDepthBuffer, bool bCreateStencilBuffer);

   /** Destructor */
   virtual ~KWindowGLES ();

   /**
    * Enable or disable 4:3 aspect correction on widescreen displays. This method must be called before createGameWindow()
    *
    * \param bFixed true to maintain a 4:3 aspect ratio in fullscreen mode on widescreen displays (enable aspect correction), false to stretch a 4:3 fullscreen window to the whole screen (disable)
    */
   virtual void setFixedAspectRatio (bool bFixed) { }

   /**
    * Set minimum requirements. This method must be called before createGameWindow()
    *
    * \param nTexSize minimum texture width and height, in pixels, that the display adapter must support (defaults to 512)
    * \param nVidMemBytes minimum amount of VRAM that the display adapter must have (defaults to 4 * 1024 * 1024)
    * \param nFeatures features that the display adapter must support (K_WINDOW_FEATURE_xxx; defaults to 0)
    */
   virtual void setRequirements (long nTexSize, unsigned long nVidMemBytes, long nFeatures);

   /**
    * Get width of the user's desktop where the game window will appear or has appeared, in pixels
    *
    * \return desktop width in pixels
    */
   virtual long getDesktopWidth (void);
   
   /**
    * Get height of the user's desktop where the game window will appear or has appeared, in pixels
    *
    * \return desktop height in pixels
    */
   virtual long getDesktopHeight (void);
   
   /**
    * Get display frequency of the user's desktop, in Hertz
    *
    * \return display frequency, in hertz
    */
   virtual long getDesktopDisplayFrequency (void) { return 60; }
   
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
   virtual bool checkWindowedMode (long nWidth, long nHeight);
   
   /**
    * Set bias for mipmapping, when mipmap generation is enabled
    *
    * \param fMipMapBias mipmap bias (negative is sharper)
    */
   virtual void setMipMapBias (float fMipMapBias);
   
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
   virtual bool createGameWindow (long nWidth, long nHeight, long nDepth, bool bWindowed, const char *lpszWindowTitle = _KANJI_DEFAULT_TITLE, long nOrientation = K_LANDSCAPE);

   /**
    * Switch to windowed or fullscreen mode
    *
    * \param bFullscreen true to switch to fullscreen mode, false to switch to windowed mode
    *
    * \return true for success, false for failure
    */
   virtual bool toggleFullScreen (bool bFullscreen) { return false; }

   /**
    * Change the game window's title
    *
    * \param lpszWindowTitle new title
    */
   virtual void setTitle (const char *lpszWindowTitle) { }

   /**
    * Set the game window's gamma
    *
    * \param fGamma current gamma (0..1)
    */
   virtual void setGamma (float fGamma);

   /**
    * Set new window orientation
    *
    * \param nOrientation new orientation
    */
   virtual void setOrientation (long nOrientation);
   
   /**
    * Enable or disable multitouch events (iPhone and iPad)
    *
    * \param bEnable true to enable multitouch, false to disable (default)
    */
   virtual void enableMultiTouch (bool bEnable);
   
   /**
    * Check if the player has requested to close the game window, and the game should quit
    *
    * \return true if closed, false if still open
    */
   virtual bool isQuit (void);

   /**
    * Set whether the player has requested to close the game window
    *
    * \param bQuitState true to flag the game window as closing, false to flag it as open, as will be reported by isQuit()
    */
   virtual void setQuit (bool bQuitState);

   /**
    * Tell the window to close and send a quit message to the game
    */
   virtual void terminate (void);

   /**
    * Check if the game window currently has the input focus
    *
    * \return true if the window has the focus (input), false if not
    */
   virtual bool hasFocus (void);

   /**
    * Check whether the renderer of this game window uses hardware acceleration for drawing
    *
    * \return true if drawing is hardware accelerated, false if drawn in software
    */
   virtual bool getAccelerationCap (void) { return true; }

   /**
    * Check if user's desktop is in a widescreen resolution (aspect ratio higher than 4:3) and it makes sense
    * to offer an aspect correction setting in the game
    *
    * \return true if desktop is widescreen, false if 4:3
    */
   virtual bool isWidescreenAvailable (void) { return false; }

   /**
    * Minimize the game window to the taskbar
    */
   virtual void minimize (void) { }

   /**
    * Restore the minimized game window to its original size
    */
   virtual void restore (void) { }

   /**
    * Get logical width of the game window contents, in pixels, regardless of aspect correction; this will be the same as the width passed to createGameWindow()
    *
    * \return logical width, in pixels
    */
   virtual long getWindowWidth (void) { return _nWindowWidth; }

   /**
    * Get logical height of the game window contents, in pixels, regardless of aspect correction; this will be the same as the width passed to createGameWindow()
    *
    * \return logical height, in pixels
    */
   virtual long getWindowHeight (void) { return _nWindowHeight; }

   /**
    * Get actual width of the game window contents (will be different than the width passed to createGameWindow() if aspect correction is in use)
    *
    * \return actual width, in pixels
    */
   virtual long getActualWindowWidth (void) { return (long) _nActualWidth; }

   /**
    * Get actual height of the game window contents (will be different than the height passed to createGameWindow() if aspect correction is in use)
    *
    * \return actual height, in pixels
    */
   virtual long getActualWindowHeight (void) { return (long) _nActualHeight; }

   /**
    * Get actual number of bits per pixel used to draw the game window
    *
    * \return actual number of bits per pixel
    */
   virtual long getWindowDepth (void) { return 32; }
   
   /**
    * Get size of total RAM that can be used for textures, in kibibytes. This method must be called after createGameWindow()
    *
    * \return size of total texture RAM in kibibytes
    */
   virtual long getVidMem (void);

   /**
    * Get size of currently available RAM for textures, in kibibytes. This method must be called after createGameWindow()
    *
    * \return size of currently available texture RAM in kibibytes
    */
   virtual long getAvailVidMem (void);

   /**
    * Get the name of the display adapter used for rendering this game window. This method must be called after createGameWindow()
    *
    * \param lpBuffer buffer where to store the display adapter's name (will be zero-terminated)
    * \param nBufferLen size of buffer in characters (not bytes)
    */
   virtual void getAdapterName (char *lpBuffer, long nBufferLen);

   /**
    * Get the maximum hardware texture size that the display adapter supports. This method must be called after createGameWindow()
    *
    * \param nMaxWidth returned maximum width, in pixels
    * \param nMaxHeight returned maximum height, in pixels
    *
    * \return true if information was returned, false if it wasn't (maximum size is unknown)
    */
   virtual bool getMaxTextureSize (long &nMaxWidth, long &nMaxHeight);

   /**
    * Check whether the display adapter used to render the game window supports non-square textures. This method must be called after createGameWindow()
    *
    * \return true if display adapter supports non-square textures, false if it doesn't
    */
   virtual bool getRectangleTexCap (void) { return true; }

   /**
    * Force the use of non-square textures. Ignored by Kanji and included for source compatibility.
    */
   virtual void setRectangleTexCap (bool bForceRectangleCap) { }

   /**
    * Throttle the renderer's framerate
    *
    * \param nDesiredFrameRate target framerate: -1 to disable vsync and throttling, 0 to enable vsync and disable throttling, > 0 for a specific framerate (for instance 60 for 60fps)
    */
   virtual void setMaxFrameRate (long nDesiredFrameRate);

   /**
    * Reset the clock for evaluating the time elapsed in the last frame. When using getFrameTime(), this method must be called before entering a rendering loop 
    */
   virtual void resetFrameTime (void);
   
   /**
    * Get how much time has elapsed since the last frame
    *
    * \return time elapsed, in milliseconds
    */
   virtual double getFrameTime (void);
   
   /**
    * Set color to use for clearing the window with setWorldView()
    *
    * \param r red component, 0..1
    * \param g green component, 0..1
    * \param b blue component, 0..1
    * \param a alpha component, 0..1 (ignored for the backbuffer, taken into account when drawing to a render target)
    */
   virtual void setClearColor (float r, float g, float b, float a);

   /**
    * Set current world transform
    *
    * \param fXTranslate translation over the X axis, in pixels
    * \param fYTranslate translation over the Y axis, in pixels
    * \param fZRotation angle of rotation over the Z axis, in degrees
    * \param fScale scaling factor (1.0f for 1:1 texel to pixel conversion)
    * \param bClearWorld true to clear the game window using the clear color, false not to
    */
   virtual void setWorldView (float fXTranslate, float fYTranslate, float fZRotation, float fScale, bool bClearWorld);

   /**
    * Set default world transform: no translation, no rotation, 1.0 scale and not clearing the window
    */
   virtual void setDefaultWorldView (void);

   /**
    * Set current world transform as a matrix
    *
    * \param lpMatrix matrix to be applied as the current world transform
    */
   virtual void setWorldMatrix (KMatrix *lpMatrix);
   
   /**
    * Set current projection transform as a matrix
    *
    * \param lpMatrix matrix to be applied as the current projection transform
    */
   virtual void setProjectionMatrix (KMatrix *lpMatrix);
   
   /**
    * Set clipping rectangle, in game window coordinates. When enabled, no drawing will occur outside of the specified rectangle
    *
    * \param bEnable true to enable, false to disable
    * \param x1 X coordinate of top, left of clipping rectangle, in game window coordinates
    * \param y1 Y coordinate of top, left of clipping rectangle, in game window coordinates
    * \param x2 X coordinate + 1 of bottom, right of clipping rectangle, in game window coordinates
    * \param y2 Y coordinate + 1 of bottom, right of clipping rectangle, in game window coordinates
    */
   virtual void setClipRect (bool bEnable, float x1, float y1, float x2, float y2);
   
   /**
    * Enable stencil testing. The window must have been created with a stencil buffer
    *
    * \param bClearStencilBuffer true to clear stencil buffer, false to leave its contents as they are
    * \param bEnableStencilWrites true to draw to the stencil buffer, false to draw to the color buffer
    * \param fMinAlpha when drawing to the stencil buffer, minimum alpha value to set a pixel in the stencil buffer
    * \param bReverse once the stencil buffer has been written to, true to only allow drawing pixels to the color buffer that don't match the stencil mask;f
    *                 alse to only allow drawing pixels that match the stencil mask
    */
   virtual void beginStencil (bool bClearStencilBuffer, bool bEnableStencilWrites, float fMinAlpha, bool bReverse);
   
   /**
    * Disable stencil testing
    */
   virtual void endStencil (void);
   
   /**
    * Begin rendering 3d triangles
    *
    * \param bClearDepthBuffer true to clear the depth buffer, false not to
    * \param bEnableDepthWrites true to enable writes to the depth buffer, false to disable them
    * \param nCullingMode triangle culling mode
    * \param bFrontToBack true if rendering front to back with alpha blending turned off
    */
   virtual void begin3d (bool bClearDepthBuffer, bool bEnableDepthWrites, KWindow3dCulling nCullingMode,
                         bool bFrontToBack = false);
   
   /**
    * Set depth (Z coordinate) to use for 2d blits
    *
    * \param fDepth new depth
    */
   virtual void setBlitDepth (float fDepth);
   
   /**
    * Set 3d projection, camera or world transformation matrix
    *
    * \param n3dTransform index of transform to be set (K_TRANSFORM_xxx)
    * \param lpMatrix matrix to be applied as the current projection
    */
   virtual void set3dMatrix (long n3dTransform, KMatrix *lpMatrix);
   
   /**
    * Set ambient light for 3d rendering
    *
    * \param r red component, 0..1
    * \param g green component, 0..1
    * \param b blue component, 0..1
    * \param a alpha component, 0..1
    */
   virtual void set3dAmbientLight (float r, float g, float b, float a);
   
   /**
    * Enable or disable 3d light and set its parameters
    *
    * \param nLightIdx index of 3d light to be set and enabled, or disabled (0-7)
    * \param lpLight parameters of light to be enabled, or NULL to disable
    */
   virtual void set3dLight (long nLightIdx, KWindow3dLight *lpLight);
   
   /**
    * Set current 3d diffuse material color
    *
    * \param r red component, 0..1
    * \param g green component, 0..1
    * \param b blue component, 0..1
    * \param a alpha component, 0..1
    */
   virtual void set3dMatColor (float r, float g, float b, float a);
   
   /**
    * Stop rendering 3d triangles
    */
   virtual void end3d (void);
   
   /**
    * Restore drawing state after the game has drawn directly to DirectX or OpenGL
    */
   virtual void restoreState (void);
   
   /**
    * Poll and process system events
    */
   virtual void processEvents (void);

   /**
    * Switch the contents of the back and front buffers so as to present the frame that was being drawn, and start drawing a new frame in the backbuffer
    *
    * \param bWaitForFocus true to suspend the game if focus is lost, until focus is restored; false not to
    * \param bProcessEvents true to process input events now, false to ignore them
    */
   virtual void flipBackBuffer (bool bWaitForFocus = true, bool bProcessEvents = true);

   /**
    * Install portable window event handler
    *
    * \param lpPortableHandler portable event handler, NULL to remove
    */
   virtual void setPTKCallBack (ptkNativeCallBak lpPortableHandler) { _lpPortableHandler = lpPortableHandler; }
   
   /**
    * Enable or disable displaying software (blitted) mouse cursor
    *
    * \param bEnableSoftCursor true to enable software mouse cursor, false to disable
    */
   virtual void displayMouse (bool bEnableSoftCursor) { }

   /**
    * Is displaying software (blitted) mouse cursor?
    */
   virtual bool isDisplayingMouse (void) { return false; }
   
   /**
    * If the game is hiding the OS cursor and drawing its own software one itself, not using displayMouse(), check if
    * the game's cursor should be drawn for the current frame. Kanji switches to the system cursor when moving out of the
    * game window, even when the system cursor is hidden, for the user's convenience, and the game software cursor shouldn't
    * be drawn then
    *
    * \return true if game's software cursor should be drawn, false if not
    */
   virtual bool canDisplayMouse (void) { return false; }
   
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
   virtual bool saveBackBuffer (const char *lpszFilename, long nImageFormat = K_IMAGE_BMP, long nWidth = 0, long nHeight = 0, long nCropX = 0, long nCropY = 0, long nCropWidth = 0, long nCropHeight = 0);

   /** Make backbuffer the active render target - internal */
   virtual void bindBackbuffer (void);
   
   /**
    * Set specified pixel shader as active for the next blits
    *
    * \param lpPixelShader pixel shader to set, NULL to use the default fixed-function pipeline
    */
   virtual void setPixelShader (KPixelShader *lpPixelShader);
   
   /**
    * Set value of a parameter in the currently active pixel shader
    *
    * \param nParam index of parameter to set
    * \param fValue value (as four floats)
    */
   virtual void setPixelShaderParam (long nParam, float fValue[4]);
   
   /**
    * Get memory size currently used for textures
    *
    * \return total in bytes
    */
   virtual size_t getTotalTextureBytes (void);
   
   /**
    * Check if the game is being run on a touchscreen platform (real or simulated with simulateTouchscreen)
    *
    * \return true if touchscreen, false if mouse-based desktop
    */
   virtual bool isTouchscreen (void) { return true; }
   
   /**
    * Simulate a touchscreen platform such as the iPhone/iPad, on a desktop platform such as Windows and Mac, by sending simulated K_EVENT_TOUCH
    * events as well as re-generating emulated mouse events from them, like a touchscreen platform does. This method is ignored on an
    * actual touchscreen platform
    *
    * \param bEnable true to simulate, false for normal operation
    */
   virtual void simulateTouchscreen (bool bEnable)  {;}
   
   /**
    * Get screen scaling factor
    *
    * \return scaling factor (2.0f for Retina displays on iPhone 4)
    */
   virtual float getScreenScale (void) { return 1.0f; }
   
   /**
    * Dispatch event to the game - internal
    *
    * \private
    *
    * \param lpEvent event to be dispatched
    */
   virtual void dispatchEvent (KEvent *lpEvent);
   
   /**
    * Enable or disable writing to the alpha component of the framebuffer
    *
    * \param bEnable true to enable, false to disable
    */
   virtual void enableAlphaWrites (bool bEnable);
   
   /**
    * Get orientation currently set by the game
    *
    * \return current orientation
    *
    * \private
    */
   virtual long getOrientation (void);
   
private:
   /** true for createGameWindow() to create a depth buffer (Z-buffer) for the game window, false not to */
   bool _bCreateDepthBuffer;
   
   /** true for createGameWindow() to create a stencil buffer for the game window, false not to */
   bool _bCreateStencilBuffer;
   
   /** Orientation requested by the game */
   long _nGameOrientation;

   /** Orientation currently set by the game */
   long _nOrientation;
   
   /** Current orientation of the device */
   long _nCurDeviceOrientation;
   
   /** Logical width in pixels */
   long _nWindowWidth;
   
   /** Logical height in pixels */
   long _nWindowHeight;
   
   /** Width in pixels after applying orientation */
   long _nRotatedWidth;
   
   /** Height in pixels after applying orientation */
   long _nRotatedHeight;
   
   /** Actual width in pixels */
   long _nActualWidth;
   
   /** Actual height in pixels */
   long _nActualHeight;
   
   /** Logical width of current render target (or backbuffer), in pixels */
   long _nCurTargetWidth;

   /** Logical height of current render target (or backbuffer), in pixels */
   long _nCurTargetHeight;
   
   /** true if setting the mipmap LOD bias is supported */
   bool _bMipMapBiasSupported;
   
   /** Global mipmap bias factor for all textures, negative is sharper */
   float _fMipMapBias;
   
   /** OpenGL ES identifier for the backbuffer */
   GLuint _nBackBufferId;
   
   /** OpenGL ES identifier of the depth buffer, when one is created for the game */
   GLuint _nDepthBufferId;
   
   /** OpenGL ES identifier of the stencil buffer, when one is created for the game */
   GLuint _nStencilBufferId;
   
   /** Game's maximum framerate (< 0 to disable vsync) */
   long _nTargetFrameRate;

   /** Clock value at the start of the current frame */
   unsigned long _nStartFrameClock;

   /** Duration of the last few frames, in milliseconds */
   double _fFrameTime[8];
   
   /** Number of past frames for which we have timing information */
   long _nTimedFrames;
   
   /** Averaged frame time */
   double _fAvgFrameTime;
   
   /** true if currently drawing to a render target */
   bool _bRenderingToTarget;

   /** true if the player has requested to close the game window */
   bool _bQuit;

   /** true if game currently has focus */
   bool _bHasFocus;
   
   /** true if we need to clear the framebuffers */
   bool _bResetFramebuffers;
   
   /** true if we need to reload all textures */
   bool _bReloadTextures;
   
   /** true if we are currently reloading all textures */
   bool _bReloadingTextures;
   
   /** true if the GL context was preserved while paused */
   bool _bContextPreserved;
   
   /** Window gamma, 0..1 */
   float _fGamma;

   /** Color for clearing the window, as rgba */
   float _fClearColor[4];
   
   /** true if clipping rectangle is currently enabled */
   bool _bClippingRectEnabled;
   
   /** 1 if writing to the alpha component of the framebuffer is enabled, 0 if not */
   long _nAlphaWritesEnabled;
   
   /** Number of frames left to clear the alpha component for, after disabling alpha writes */
   long _nAlphaResets;
   
   /** X coordinate to send for mouseup event emulation */
   long _nMouseUpX;
   
   /** Y coordinate to send for mouseup event emulation */
   long _nMouseUpY;
   
   /** Delay left in milliseconds before sending the mouseup event, -1 if it isn't to be sent */
   long _nMouseUpDelay;
   
   /** Timestamp of last mousedown event, -1 if none yet */
   long _nLastMouseDownTime;
   
   /** true to ignore input events, false not to */
   bool _bIgnoreInputEvents;
   
   /** Maximum width and height of textures supported by the GPU, in pixels */
   long _nMaxTexSize;
   
   /** Portable event handler */
   ptkNativeCallBak _lpPortableHandler;
   
   /** Depth (Z coordinate) for 2d blits */
   float _fBlitDepth;
   
   /** true if alpha blending is disabled while rendering opaque sections front to back */
   bool _bBlendingDisabled;
   
   /** true if writes to the depth buffer are currently enabled */
   bool _bDepthBufferWritesEnabled;
   
   /** Projection and camera matrices for 3d rendering */
   float _f3dMatrix[2][16];
   
   /** Display adapter name */
   char _szRendererName[256];
};

#endif   /* _KANJI_WINDOWGLES_H */
