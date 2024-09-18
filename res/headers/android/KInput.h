/**
 * Kanji rendering and I/O engine - Android input polling definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KINPUT_H
#define  _KANJI_KINPUT_H

/* Include definitions */
#include "KObjectBase.h"
#include "KKeyTable.h"

/** Key or mouse button is pressed down */
#define  ISDOWN   true

/** Key or mouse button is released */
#define  ISUP     false

/** Number of supported joystick IDs */
#define  K_MAXJOYSTICKS 16

/** Joystick button index for KInput::joyButtonN() */

enum {
   K_JOYBUTTON_A = 0,
   K_JOYBUTTON_B,
   K_JOYBUTTON_C,
   K_JOYBUTTON_X,
   K_JOYBUTTON_Y,
   K_JOYBUTTON_Z,
   K_JOYBUTTON_L1,
   K_JOYBUTTON_R1,
   K_JOYBUTTON_L2,
   K_JOYBUTTON_R2,
   K_JOYBUTTON_THUMBL,
   K_JOYBUTTON_THUMBR,
   K_JOYBUTTON_START,
   K_JOYBUTTON_SELECT,
   K_JOYBUTTON_MODE,
};

/** Joystick properties bitmask */
#define K_JOYPROPERTY_CONNECTED  1     /**< Joystick is present */
#define K_JOYPROPERTY_DPAD       2     /**< Has a D-PAD and standard buttons */
#define K_JOYPROPERTY_EXTENDED   4     /**< Has sticks and extended buttons */
#define K_JOYPROPERTY_MOTION     8     /**< Has motion support */

/* Forward definitions */
class KWindow;
class KWindowGLES;
class KWindowGLES2;

/**
 * Class for polling inputs
 */

class KInput : public KObjectBase {
   friend class KWindow;
   friend class KWindowGLES;
   friend class KWindowGLES2;
   
public:
   /**
    * Get current X coordinate of the mouse pointer
    *
    * \return mouse X coordinate
    */
   static short getMouseX (void);

   /**
    * Get current Y coordinate of the mouse pointer
    *
    * \return mouse Y coordinate
    */
   static short getMouseY (void);

   /**
    * Get current state of the left mouse button
    *
    * \return true(ISDOWN) if pressed down, false(ISUP) if released
    */
   static bool getLeftButtonState (void);

   /**
    * Get current state of the right mouse button
    *
    * \return true(ISDOWN) if pressed down, false(ISUP) if released
    */
   static bool getRightButtonState (void) { return ISUP; }

   /**
    * Get current state of the middle mouse button
    *
    * \return true(ISDOWN) if pressed down, false(ISUP) if released
    */
   static bool getMiddleButtonState (void) { return ISUP; }

   /**
    * Get current value of the mousewheel
    *
    * \return mouse wheel value
    */
   static short getWheelValue (void) { return 0; }

   /**
    * Show operating system mouse pointer (default)
    */
   static void showPointer (void) { }

   /**
    * Hide operating system mouse pointer (to use a blitted game-specific mouse pointer instead)
    */
   static void hidePointer (void) { }

   /**
    * Move mouse to the specified game window coordinates
    *
    * \param mx new X coordinate (in game window space)
    * \param my new Y coordinate (in game window space)
    */
   static void mousePointerTo (long mx, long my) { }

   /**
    * Get current state of the specified key
    *
    * \param nPortableKey portable keycode (K_VK_xxx)
    *
    * \return true(ISDOWN) if pressed down, false(ISUP) if released
    */
   static bool isPressed (EKeyboardLayout nPortableKey) { return ISUP; }

   /**
    * Wait until the specified key is released (up)
    *
    * \param nPortableKey portable keycode (K_VK_xxx) to wait for
    */
   static void waitForKeyRelease (EKeyboardLayout nPortableKey) { }

   /**
    * Get portable keycode, from native operating system virtual key
    *
    * \param nNativeKey OS-specific virtual keycode
    *
    * \return portable keycode
    */
   static EKeyboardLayout getPTKKey (long nNativeKey) { return K_VK_ERROR; }
   
   /**
    * Get the properties of the specified joystick
    *
    * \return properties mask
    */
   static unsigned long joyPropertiesMask (char nJoyId);
   
   /**
    * Get the number of managed joysticks
    *
    * \return number of managed joysticks
    */
   static long joyCount (void);
   
   /**
    * Set player indicator on joystick
    *
    * \param nJoyID ID of joystick to set player indicator for
    * \param nPlayerIdx player number: -1 to unset, 0..n for player number 1..n+1
    */
   static void joySetPlayerIndicator (char nJoyId, long nPlayerIdx) { }
   
   /**
    * Capture and enable reading the specified joystick
    *
    * \param nJoyId ID of joystick to capture and enable (0-15)
    *
    * \return true if successfully captured and can be read, false if not
    */
   static bool joyEnable (char nJoyId) { return true; }
   
   /**
    * Release and disable reading the specified joystick
    *
    * \param nJoyId ID of joystick to be released (0-15)
    */
   static void joyDisable (char nJoyId) { }
   
   /**
    * Test if specified joystick's up direction is pushed
    *
    * \param nJoyId ID of joystick to be tested (0-15)
    *
    * \return true(ISDOWN) is pushed up, false(ISUP) if not
    */
   static bool joyUp (char nJoyId);
   
   /**
    * Test if specified joystick's down direction is pushed
    *
    * \param nJoyId ID of joystick to be tested (0-15)
    *
    * \return true(ISDOWN) is pushed down, false(ISUP) if not
    */
   static bool joyDown (char nJoyId);
   
   /**
    * Test if specified joystick's left direction is pushed
    *
    * \param nJoyId ID of joystick to be tested (0-15)
    *
    * \return true(ISDOWN) is pushed left, false(ISUP) if not
    */
   static bool joyLeft (char nJoyId);
   
   /**
    * Test if specified joystick's right direction is pushed
    *
    * \param nJoyId ID of joystick to be tested (0-15)
    *
    * \return true(ISDOWN) is pushed right, false(ISUP) if not
    */
   static bool joyRight (char nJoyId);
   
   /**
    * Test if specified joystick's action button is pushed
    *
    * \param nJoyId ID of joystick to be tested (0-15)
    * \param nButtonId ID of action button to be tested (0-31)
    *
    * \return true(ISDOWN) is action button is pushed, false(ISUP) if not
    */
   static bool joyButtonN (char nJoyId, char nButtonId);
   
   /**
    * Read X axis of specified joystick's hat or Z axis value 
    *
    * \param nJoyId ID of joystick to read hat axis value from (0-15)
    * \param nHatId ID of value to read (0=first hat's X axis, 1=second hat's X axis or Z axis)
    *
    * \return hat X axis value
    */
   static long joyX (char nJoyId, char nHatId);
   
   /**
    * Read Y axis of specified joystick's hat or rudder value 
    *
    * \param nJoyId ID of joystick to read hat axis value from (0-15)
    * \param nHatId ID of value to read (0=first hat's Y axis, 1=second hat's X axis or rudder value)
    *
    * \return hat Y axis value
    */
   static long joyY (char nJoyId, char nHatId);
   
   /**
    * Return point-of-view value of specified joystick
    *
    * \param nJoyId ID of joystick to read point-of-view value of (0-15)
    *
    * \return point of view value
    */
   static long joyPOV (char nJoyId) { return 65535; }
   
   /**
    * Wait until the specified joystick's action button is released
    *
    * \param nJoyId ID of joystick to wait on (0-15)
    * \param nButtonId ID of action button to wait on (0-31)
    */
   static void joyWaitForJoyButtonUp (char nJoyId, char nButtonId) { }
   
   /**
    * Rescan joystick devices. For source compatibility only; device installation and removal is automatically handled by Kanji
    *
    * \private
    */
   static void RescanJoysticks (void) { }
   
   /**
    * Update joystick axis coordinates - internal
    *
    * \param nHatId hat ID (0..7)
    * \param x new X position (-1..1)
    * \param y new Y position (-1..1)
    *
    * \private
    */
   static void processJoystickAxis (long nJoyId, long nHatId, float x, float y);
   
   /**
    * Update joystick keys status - internal
    *
    * \param nJoyId ID of joystick to update (0-15)
    * \param nKeyIdx joystick key index
    * \param bIsDown true if key went down, false if it went up
    *
    * \private
    */
   static void processJoystickKey (long nJoyId, long nKeyIdx, bool bIsDown);
   
private:
   /**
    * Update mouse emulation when a touchscreen event occurs
    *
    * \param nJoyId ID of joystick to update (0-15)
    * \param nXTouch X coordinate of the touch
    * \param nYTouch Y coordinate of the touch
    * \param nTouchType touch type (K_TOUCHxxx)
    */
   static void processTouchDown (long nXTouch, long nYTouch, long nTouchType);
   
   /** X coordinate of last touch */
   static long g_nXMouse;
   
   /** Y coordinate of last touch */
   static long g_nYMouse;
   
   /** true if we're currently reporting to the game that the left mouse button is pressed */
   static bool g_bMouseButtonDown;
   
   /** For each joystick key, true if it is currently down */
   static bool g_bJoystickKeyState[K_MAXJOYSTICKS][36];
   
   /** Value of each joystick axis, 0..65535 */
   static long g_nJoystickAxis[K_MAXJOYSTICKS][8][2];
};

#endif   /* _KANJI_KINPUT_H */
