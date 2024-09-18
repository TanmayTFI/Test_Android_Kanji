/**
 * Kanji game engine - base class for UI elements, definitions
 *
 * (c) 2010
 *
 * \file KUIElement.h
 */

#ifndef  _KANJI_UIELEMENT_H
#define  _KANJI_UIELEMENT_H

/* Include definitions */
#include "KHashTable.h"
#include "KList.h"
#include "KWindow.h"
#include "KGraphic.h"
#include "KBatch.h"
#include "KMath2d.h"
#include "KMath3d.h"
#include "KUIBounds.h"
#include "KCounter.h"

/** Maximum length of an element's name */
#define  K_UIELEMENT_MAXNAMELEN  100

/** Maximum depth of an element's class hierarchy */
#define  K_UIELEMENT_MAXCLASSES  8

/** Maximum length of an element's class name */
#define  K_UIELEMENT_MAXCLASSNAMELEN   32

/** Number of buckets for hashing elements by their identifier */
#define  K_UIELEMENT_IDBUCKETS   64

/** Width and height of a cell for the spatial hashing */
#define  K_UIELEMENT_SPATIALHASHSTEP   64

/**
 * Element state
 */

typedef enum {
   K_UIELEMENT_DISABLED = 0,           /**< Disabled, cannot be actioned */
   K_UIELEMENT_ENABLED,                /**< Enabled, can be actioned, mouse isn't over the element */
   K_UIELEMENT_MOUSEOVER,              /**< Enabled, mouse is over this element */
   K_UIELEMENT_DOWN,                   /**< Enabled, currently pressed down */
   K_UIELEMENT_SELECTED,               /**< Enabled, selected choice among multiple ones */

   K_UIELEMENT_STATES,                 /**< Number of states */
} KUIElementState;

/**
 * Horizontal alignment mode
 */

typedef enum {
   K_UIELEMENT_LEFT = 0,               /**< Align left */
   K_UIELEMENT_RIGHT,                  /**< Align right */
   K_UIELEMENT_HCENTER,                /**< Center horizontally */
} KUIHorizAlignMode;

/**
 * Vertical alignment mode
 */

typedef enum {
   K_UIELEMENT_TOP = 0,                /**< Align with the top of the element */
   K_UIELEMENT_BOTTOM,                 /**< Align with the bottom of the element */
   K_UIELEMENT_VCENTER,                /**< Center vertically */
} KUIVertAlignMode;

/* Forward definitions */
class KUIElement;
class KScript;

/**
 * Message sent to the handler set by setMessageCallBack
 */

typedef struct {
   /** Message (for instance K_UIELEMENT_MOUSEOVER) */
   long nMessage;

   /** Element that generated the message */
   KUIElement *lpElement;

   /** First optional parameter */
   float fParam1;

   /** Second optional parameter */
   float fParam2;

   /** Third optional parameter */
   long nParam3;
   
   /** Fourth optional parameter */
   const char *lpszParam4;

   /** Firth optional parameter */
   KUIElement *lpParam5;

   /** Reply, if the message expects one */
   long nReply;
} KUIMessage;

/** Base class for handling UI message callbacks as member methods */

class KUIMessageFunctionBase {
public:
   /** Destructor */
   virtual ~KUIMessageFunctionBase ();

   /**
    * Invoke callback when a UI message is dispatched
    *
    * \private
    *
    * \param lpMessage UI message
    */
   virtual void call (KUIMessage *lpMessage) = 0;

   /**
    * Compare instance and method with another function
    *
    * \private
    *
    * \return true if instance and method are equal, false if not
    */
   virtual bool compare (KUIMessageFunctionBase *lpOtherFunction) = 0;
};

/** Class used to pass UI message callbacks that are themselves member methods and not static */

template<class T> class KUIMessageFunction : public KUIMessageFunctionBase {
   /**
    * Member function pointer for callback
    *
    * \param lpMessage UI message
    */
   typedef void (T::*KUIMessageListenerCallback)(KUIMessage *lpMessage);

public:
   /** Constructor */
   KUIMessageFunction (T *lpInstance, KUIMessageListenerCallback lpCallback) :
         _lpInstance (lpInstance), _lpCallback (lpCallback) {
   }

   /**
    * Invoke callback
    *
    * \private
    *
    * \param lpMessage UI message
    */
   virtual void call (KUIMessage *lpMessage) {
      (_lpInstance->*_lpCallback)(lpMessage);
   }

   /**
    * Compare instance and method with another function
    *
    * \private
    *
    * \return true if instance and method are equal, false if not
    */
   virtual bool compare (KUIMessageFunctionBase *lpOtherFunction) {
      return _lpInstance == ((KUIMessageFunction<T> *)lpOtherFunction)->_lpInstance &&
             _lpCallback == ((KUIMessageFunction<T> *)lpOtherFunction)->_lpCallback;
   }

private:
   /** Instance to invoke callback in */
   T *_lpInstance;

   /** Method to be called in instance */
   KUIMessageListenerCallback _lpCallback;
};

/**
 * Message callback when using a static method
 *
 * \param lpMessage UI message
 */
typedef void (*kanjiUiMessageCallBack)(KUIMessage *lpMessage);

/**
 * Supply a member method as a callback for UI messages
 *
 * \param _class class name (for example: MyGame)
 * \param _method method name (for example: mySignalHandler)
 * \param _instance instance (for example: this)
 */
#define K_UIMSG_FUNCTION(_class,_method,_instance) new KUIMessageFunction<_class> (_instance, &_class::_method)

/** Create identifier for the first message sent by a class of elements */
#define K_UIMSG(__a,__b,__c) ((__a) << 24 | ((__b) << 16) | ((__c) << 8) | 1)

/** Messages sent by KUIElement */
enum {
   K_UIELEMENT_MSGMOUSEOVER = K_UIMSG ('k','e','l'),     /**< Mouse just moved over the element */
   K_UIELEMENT_MSGMOUSELEAVE,                            /**< Mouse just left the element */
   K_UIELEMENT_MSGDOWN,                                  /**< The element just started being pressed down */
   K_UIELEMENT_MSGCURRENTLYDOWN,                         /**< The element is currently pressed down */
   K_UIELEMENT_MSGUP,                                    /**< The element just stopped being pressed down */
   K_UIELEMENT_MSGSELECTED,                              /**< The element has just been selected */
   K_UIELEMENT_MSGDESELECTED,                            /**< The element has just been deselected */
   K_UIELEMENT_MSGROLLOVER,                              /**< Mouse just moved over a child of the element */
   K_UIELEMENT_MSGROLLOUT,                               /**< Mouse just left a child of the element */
   K_UIELEMENT_MSGDONEANIMATING,                         /**< The element has just completed animating its properties (scale, shear, position..) */
   K_UIELEMENT_MSGCOLLISIONWITH,                         /**< The element is colliding with another element. lpParam5 = element colliding with the one that generated the message */
   K_UIELEMENT_MSGHIT,                                   /**< Hit test, see onHit(). fParam1 = x, fParam2 = y. Reply 1 if coordinates are over the element, 0 if not */
   K_UIELEMENT_MSGCHILDDOWN,                             /**< A child of the element just started being pressed down */
   K_UIELEMENT_MSGCHILDUP,                               /**< A child of the element just stopped being pressed down */
   K_UIELEMENT_MSGSWIPE,                                 /**< A swipe gesture is being executed on the element. fParam1 = dx, fParam2 = dy, nParam3 = gesture type (K_GESTURE_xxx). Reply 1 if processed, 0 if not */
};

/** \cond private */

/** A script attached to a KUIElement */

class KUIElementScriptNode : public KObjectListable {
public:
   /** Destructor */
   ~KUIElementScriptNode ();

   /** Script */
   KScript *lpScript;
};

/** \endcond */

/**
 * Base class of all UI elements, that can be derived to implement containers, images, text, buttons and other elements
 */

class KUIElement : public KObjectHashable {
   friend class KScript;

public:
   /**
    * Initialize UI
    *
    * \param lpKWindow game window
    */
   static void initialize (KWindow *lpKWindow);

   /** Enable proper margins on touchscreens */
   static void enableTouchMargin (void);

   /** Free all resources used by the UI */
   static void cleanup (void);

   /** Delete all elements that have no parent */
   static void cleanupOrphans (void);

   /**
    * Get game window that all elements draw to
    *
    * \return game window
    */
   static KWindow *getKWindow (void);

   /**
    * Dispatch input event to all elements
    *
    * \param lpKEvent event
    *
    * \return true if processed, false if not
    */
   static bool dispatchEvent (KEvent *lpKEvent);

   /**
    * Dispatch user event to all elements
    *
    * \param lpszEvent event string to be dispatched
    * \param lpData event data
    *
    * \return true if processed, false if not
    */
   static bool dispatchUserEvent (const char *lpszEvent, const void *lpData);

   /**
    * Update logic for all elements
    */
   static void moveAll (void);

   /**
    * Begin drawing all UI elements as front-to-back (for opaque subrectangles) and back-to-front (for the rest)
    */
   static void beginDepthBufferBlit (void);

   /**
    * Draw opaque subrectangles of all elements in specified layer, starting at this root element
    *
    * \param nLayer layer number
    */
   static void blitAllFront (long nLayer);

   /**
    * End drawing all UI elements as front-to-back (for opaque subrectangles) and back-to-front (for the rest)
    */
   static void endDepthBufferBlit (void);

   /**
    * Draw all elements in specified layer
    *
    * \param nLayer layer number
    */
   static void blitAll (long nLayer);

   /**
    * Get current number of instanciated elements
    *
    * \return current number of instanciated elements
    */
   static long getTotalElementsCount (void);

   /**
    * Get number of elements moved in the last logic update
    *
    * \return number of updated elements
    */
   static long getUpdatedElementsCount (void);

   /**
    * Get number of elements blitted in the last logic update
    *
    * \return number of drawn elements
    */
   static long getDrawnElementsCount (void);

   /**
    * Get root element
    *
    * \return root element
    */
   static KUIElement *getRootElement (void);

   /**
    * Retrieve element from its case-sensitive name
    *
    * \param lpszElemName name of element to be retrieved
    *
    * \return element, or NULL if not found
    */
   static KUIElement *getElementByName (const char *lpszElemName);

   /**
    * Retrieve element from its case-insensitive name
    *
    * \param lpszElemName name of element to be retrieved
    *
    * \return element, or NULL if not found
    */
   static KUIElement *getElementByNameNoCase (const char *lpszElemName);

   /**
    * Retrieve element from its numerical ID
    *
    * \param nElemId numerical ID of element to be retrieved
    *
    * \return element, or NULL if not found
    */
   static KUIElement *getElementByIdentifier (long nElemId);

   /**
    * Get element currently under the mouse
    *
    * \return element currently under the mouse, NULL for none
    */
   static KUIElement *getMousedOverElement (void);

   /**
    * Get element currently being pressed down
    *
    * \return element being pressed down, NULL for none
    */
   static KUIElement *getDownElement (void);

   /**
    * Get element that currently has the keyboard focus
    *
    * \return element that has the keyboard focus, NULL for none
    */
   static KUIElement *getFocusedElement (void);

   /**
    * Find element that would receive input at the specified coordinates, expressed as coordinates in the specified root element
    *
    * \param lpRootElement root element to start searching at
    * \param x X coordinate to pick element at; if successful, the coordinate is updated to be in the returned element's coordinate space
    * \param y Y coordinate to pick element at; if successful, the coordinate is updated to be in the returned element's coordinate space
    * \param bAllElements true to search all elements, false to only search elements that currently accept input
    *
    * \return element found at these coordinates, NULL if none
    */
   static KUIElement *pickElement (KUIElement *lpRootElement, float &x, float &y, bool bAllElements = false);

   /**
    * Check if left mouse button was clicked since last check, and not absorbed by an element
    *
    * \return true if clicked, false if not
    */
   static bool isLeftButtonClicked (void);

   /**
    * Check if right mouse button was clicked since last check, and not absorbed by an element
    *
    * \return true if clicked, false if not
    */
   static bool isRightButtonClicked (void);

   /**
    * Check if middle mouse button was clicked since last check, and not absorbed by an element
    *
    * \return true if clicked, false if not
    */
   static bool isMiddleButtonClicked (void);

   /** New operator for this class */
   void *operator new (size_t size);
   
   /** Delete operator for this class */
   void operator delete (void *p);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUIElement (KUIElement *lpParentElem);

   /** Destructor */
   virtual ~KUIElement ();

   /**
    * Copy all parameters except for the name, identifier, user data, state, layer number, visibility, input and message delivery status, and message callback, from another element
    *
    * \param lpFromElement element to copy from
    */
   virtual void copyFromTemplate (KUIElement *lpFromElement);

   /**
    * Get this element's parent
    *
    * \return parent element (NULL if asking for the parent of the root element)
    */
   KUIElement *getParentElement (void) const;

   /**
    * Set or change this element's parent; the layer number of this element will also be copied from the new parent
    *
    * \param lpParentElem new parent element
    */
   void setParentElement (KUIElement *lpParentElem);

   /**
    * Get this element's first child
    *
    * \return this element's first child, NULL if none
    */
   KUIElement *getFirstChildElement (void);

   /**
    * Get this element's last child
    *
    * \return this element's last child, NULL if none 
    */
   KUIElement *getLastChildElement (void);

   /**
    * Get next sibling of this element
    *
    * \return this element's next sibling, NULL if none
    */
   KUIElement *getNextSiblingElement (void);

   /**
    * Make this element the first sibling in its parents' list: it will be drawn first, and will be considered last for input,
    * amongst its siblings
    */
   void moveToHead (void);

   /**
    * Make this element the last sibling in its parents' list: it will be drawn last, and will be considered first for input,
    * amongst its siblings
    */
   void moveToTail (void);

   /**
    * Make this element appear before the specified sibling in its parents' list: it will be drawn before the sibling, and will be
    * considered after the sibling for input
    *
    * \param lpSiblingElement sibling element that will be right after this element in the parents' list
    */
   void moveBeforeSibling (KUIElement *lpSiblingElement);

   /**
    * Make this element appear after the specified sibling in its parents' list: it will be drawn after the sibling, and will be
    * considered before the sibling for input
    *
    * \param lpSiblingElement sibling element that will be right before this element in the parents' list
    */
   void moveAfterSibling (KUIElement *lpSiblingElement);

   /**
    * Get the index of this element in its parent's list
    *
    * \return index, starting at 0
    */
   long getSiblingIndex (void);

   /**
    * Make this element the n(th) element in its parent's list
    *
    * \param nIndex new position of this element in its parent's list, starting at 0
    */
   void moveToIndex (long nIndex);

   /**
    * Swap the position of this element and a sibling element in its parents' list: the drawing order and input processing order
    * of the two elements will be swapped
    *
    * \param lpSiblingElement sibling element to swap positions with, in this element parent's list
    */
   void swapWithSibling (KUIElement *lpSiblingElement);

   /**
    * Get a count of how many children this element currently has
    *
    * \return number of children for this element
    */
   long getChildrenCount (void) const;

   /** Make this element the one that currently has the keyboard focus */
   void setFocus (void);

   /**
    * Get this element's name
    *
    * \return element's name
    */
   const char *getName (void) const;

   /**
    * Set a name for this element, for identification purposes
    *
    * \param lpszElemName element's name
    */
   virtual void setName (const char *lpszElemName);

   /**
    * Check if this element is named like the specified string
    *
    * \param lpszElemName name to check this element's name against
    *
    * \return true if element name matches, false if not
    */
   bool isNamed (const char *lpszElemName) const;

   /**
    * Get this element's numerical ID
    *
    * \return element's ID
    */
   long getIdentifier (void) const;

   /**
    * Set a numerical ID for this element, for identification purposes
    *
    * \param nElemId element's numerical ID
    */
   virtual void setIdentifier (long nElemId);

   /**
    * Get user data associated with this element
    *
    * \return user data
    */
   void *getUserData (void) const;

   /**
    * Associate user data with this element
    *
    * \param lpUserData user data to associate with this element
    */
   virtual void setUserData (void *lpUserData);

   /**
    * Get current state
    *
    * \return current state
    */
   KUIElementState getState (void) const;

   /**
    * Set new state
    *
    * \param nState state
    */
   virtual void setState (KUIElementState nState);

   /**
    * Get layer number for this element
    *
    * \return layer number
    */
   long getLayer (void) const;

   /**
    * Set layer number for this element
    *
    * \param nLayer layer number (defaults to the parent element's)
    */
   virtual void setLayer (long nLayer);
   
   /**
    * Check if processing of input events is enabled for this element
    *
    * \return true if enabled, false if disabled
    */
   bool isElementInputEnabled (void) const;

   /**
    * Enable or disable processing of input events by this element
    *
    * \param bEnable true to enable, false to disable
    */
   virtual void enableElementInput (bool bEnable);

   /**
    * Check if processing of input events is enabled for this element and its children
    *
    * \return true if enabled, false if disabled
    */
   bool isTreeInputEnabled (void) const;

   /**
    * Enable or disable processing of input events by this element and its children
    *
    * \param bEnable true to enable, false to disable
    */
   virtual void enableTreeInput (bool bEnable);

   /**
    * Set callback function that is sent messages for this element as well as all children that
    * don't se a message handler themselves
    *
    * \param lpMessageCallBack message handler callback
    */
   virtual void setMessageCallBack (kanjiUiMessageCallBack lpMessageCallBack);

   /**
    * Set callback function that is sent messages for this element as well as all children that
    * don't se a message handler themselves
    *
    * \param lpMessageFunction message handler instance and callback, supplied as K_UIMSG_FUNCTION (class name, method name, class instance)
    */
   virtual void setMessageFunction (KUIMessageFunctionBase *lpMessageFunction);

   /**
    * Get X coordinate of leftmost point of this element
    *
    * \return X coordinate
    */
   float getPositionX (void) const;

   /**
    * Get Y coordinate of topmost point of this element
    *
    * \return Y coordinate
    */
   float getPositionY (void) const;

   /**
    * Set position of this element, relative to the parent
    *
    * \param x X coordinate of top,left
    * \param y Y coordinate of top,left
    */
   virtual void setPosition (float x, float y);

   /**
    * Get X offset that is applied locally to this element's position
    *
    * \return position's X offset (defaults to 0, placing the X coordinate of the position as the left edge of the element)
    */
   float getPositionOffsetX (void) const;

   /**
    * Get Y offset that is applied locally to this element's position
    *
    * \return position's Y offset (defaults to 0, placing the Y coordinate of the position as the top edge of the element)
    */
   float getPositionOffsetY (void) const;

   /**
    * Set offset to apply locally to this element's position (defaults to 0, 0, setting the position as the left, top edge of the element)
    *
    * \param fPositionOffsetX X component of offset
    * \param fPositionOffsetY Y component of offset
    */
   virtual void setPositionOffset (float fPositionOffsetX, float fPositionOffsetY);

   /**
    * Animate the position of this element. If you need a different animation curve than what KCounterFunction supplies, use setPosition() for each step and suspend/resume inputs
    * for the animating element with enableTreeInput()
    *
    * \param fStartX starting X coordinate of top,left
    * \param fStartY starting Y coordinate of top,left
    * \param fEndX ending X coordinate of top,left
    * \param fEndY ending Y coordinate of top,left
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending position
    * \param nFunction function to be applied for progressing from the starting to the ending position (K_COUNTER_xxx)
    */
   virtual void animatePosition (float fStartX, float fStartY, float fEndX, float fEndY, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Animate the position of this element from its current location
    *
    * \param fEndX ending X coordinate of top,left
    * \param fEndY ending Y coordinate of top,left
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending position
    * \param nFunction function to be applied for progressing from the starting to the ending position (K_COUNTER_xxx)
    */
   virtual void animatePositionTo (float fEndX, float fEndY, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Get absolute position (position relative to the root element) of coordinates local to this element
    *
    * \param fRelX X coordinate to get absolute position of, in element coordinates
    * \param fRelY Y coordinate to get absolute position of, in element coordinates
    * \param fAbsX returned absolute X coordinate
    * \param fAbsY returned absolute Y coordinate
    */
   void getAbsPosition (float fRelX, float fRelY, float &fAbsX, float &fAbsY);

   /**
    * Get the width of this element
    *
    * \return width in pixels
    */
   float getWidth (void) const;

   /**
    * Get the height of this element
    *
    * \return height in pixels
    */
   float getHeight (void) const;

   /**
    * Set size of this element
    *
    * \param fWidth width in pixels
    * \param fHeight height in pixels
    */
   virtual void setSize (float fWidth, float fHeight);

   /**
    * Get X offset of center of rotation and scaling (pivot), relative to the element's anchor point
    *
    * \return X offset
    */
   float getCenterX (void);

   /**
    * Get Y offset of center of rotation and scaling (pivot), relative to the element's anchor point
    *
    * \return Y offset
    */
   float getCenterY (void);

   /**
    * Set center of rotation and scaling (pivot) for this element, relative to the element's anchor point (defaults to 0, 0)
    *
    * \param cx X offset for center
    * \param cy Y offset for center
    */
   virtual void setCenter (float cx, float cy);

   /**
    * Set center of rotation and scaling (pivot) for this element, in normalized coordinates, relative to the element's anchor point (defaults to 0,0)
    *
    * \param fNormalizedXCenter X offset for center (divided by the element's width; if the anchor isn't changed, for instance, 0 for center, -0.5f for leftmost, 0.5f for rightmost)
    * \param fNormalizedYCenter Y offset for center (divided by the element's height; if the anchor isn't changed, for instance, 0 for center, -0.5f for topmost, 0.5f for bottommost)
    */
   virtual void setNormalizedCenter (float fNormalizedXCenter, float fNormalizedYCenter);

   /**
    * Get X coordinate of the anchor point (defaults to width / 2)
    *
    * \return anchor X coordinate
    */
   float getAnchorX (void);

   /**
    * Get Y coordinate of the anchor point (defaults to height / 2)
    *
    * \return anchor Y coordinate
    */
   float getAnchorY (void);

   /**
    * Set coordinates of the anchor point, that the pivot offsets against (defaults to the center, ie. width / 2, height / 2)
    *
    * \param fXAnchor anchor X coordinate
    * \param fYAnchor anchor Y coordinate
    */
   void setAnchor (float fXAnchor, float fYAnchor); 

   /**
    * Set coordinates of the anchor point, that the pivot offsets against, in normalized coordinates (defaults to the center, ie. 0.5f, 0.5f)
    *
    * \param fNormalizedXAnchor normalized anchor X coordinate (divided by the element's width; for instance 0.5f=center, 1=width)
    * \param fNormalizedYAnchor normalized anchor Y coordinate (divided by the element's height; for instance 0.5f=center, 1=height)
    */
   void setNormalizedAnchor (float fNormalizedXAnchor, float fNormalizedYAnchor); 

   /**
    * Get blending factor for this element and its children
    *
    * \return blending factor, 0..1
    */
   float getBlend (void);

   /**
    * Set blending factor for this element and its children
    *
    * \param fBlend blending factor, 0..1
    */
   virtual void setBlend (float fBlend);
   
   /**
    * Animate the blending factor of this element
    *
    * \param fStartBlend starting blending factor
    * \param fEndBlend ending blending factor
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending blending factor
    * \param nFunction function to be applied for progressing from the starting to the ending blending factor (K_COUNTER_xxx)
    */
   virtual void animateBlend (float fStartBlend, float fEndBlend, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Animate the blending factor of this element from its current value
    *
    * \param fEndBlend ending blending factor
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending blending factor
    * \param nFunction function to be applied for progressing from the starting to the ending blending factor (K_COUNTER_xxx)
    */
   virtual void animateBlendTo (float fEndBlend, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Check whether this element is entirely hidden (regardless of blending factor), or if it is shown (subject to the blending factor)
    *
    * \return true if visible, false if hidden
    */
   bool getVisibility (void) const;

   /**
    * Set whether this element is entirely hidden (regardless of blending factor), or if it is shown (subject to the blending factor)
    *
    * \param bVisible true to show, false to hide (elements are shown by default)
    */
   virtual void setVisibility (bool bVisible);

   /**
    * Check whether this element receives input even when it is entirely hidden
    *
    * \return true if it receives input when hidden, false if not
    */
   bool getDispatchInputIfInvisible (void) const;

   /**
    * Set whether this element receives input even when it is entirely hidden
    *
    * \param bDispatch true to receive input when hidden (false by default)
    */
   virtual void setDispatchInputIfInvisible (bool bDispatch);

   /**
    * Get horizontal scaling factor for this element and its children
    *
    * \return horizontal scaling factor
    */
   float getScaleX (void) const;

   /**
    * Get vertical scaling factor for this element and its children
    *
    * \return horizontal scaling factor
    */
   float getScaleY (void) const;

   /**
    * Set both horizontal and vertical scaling factor for this element and its children to the same value
    *
    * \param fScale scaling factor
    */
   virtual void setScale (float fScale);

   /**
    * Set seperate horizontal and vertical scaling factors for this element and its children
    *
    * \param fScaleX horizontal scaling factor
    * \param fScaleY vertical scaling factor
    */
   virtual void setScaleXY (float fScaleX, float fScaleY);

   /**
    * Set scaling factors based on explicit width/height dimensions
    *
    * \param fWidth width after scaling
    * \param fHeight height after scaling
    */
   virtual void setScaleToDimensions (float fWidth, float fHeight);

   /**
    * Set scaling factor based maximum width/height dimension
    *
    * \param fMaxDimension maximum width or height after scaling
    */
   virtual void setScaleToMaxDimension (float fMaxDimension);

   /**
    * Animate the scaling factor of this element using a single value for both horizontal and vertical scaling
    *
    * \param fStartScale starting scaling factor
    * \param fEndScale ending scaling factor
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending scaling factor
    * \param nFunction function to be applied for progressing from the starting to the ending scaling factor (K_COUNTER_xxx)
    */
   virtual void animateScale (float fStartScale, float fEndScale, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Animate the scaling factor of this element using a single value for both horizontal and vertical scaling, from its current value
    *
    * \param fEndScale ending scaling factor
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending scaling factor
    * \param nFunction function to be applied for progressing from the starting to the ending scaling factor (K_COUNTER_xxx)
    */
   virtual void animateScaleTo (float fEndScale, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Animate the scaling factors of this element
    *
    * \param fStartScaleX starting horizontal scaling factor
    * \param fStartScaleY starting vertical scaling factor
    * \param fEndScaleX ending horizontal scaling factor
    * \param fEndScaleY ending vertical scaling factor
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending scaling factor
    * \param nFunction function to be applied for progressing from the starting to the ending scaling factor (K_COUNTER_xxx)
    */
   virtual void animateScaleXY (float fStartScaleX, float fStartScaleY, float fEndScaleX, float fEndScaleY, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Animate the scaling factors of this element, from their current values
    *
    * \param fEndScaleX ending horizontal scaling factor
    * \param fEndScaleY ending vertical scaling factor
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending scaling factor
    * \param nFunction function to be applied for progressing from the starting to the ending scaling factor (K_COUNTER_xxx)
    */
   virtual void animateScaleXYTo (float fEndScaleX, float fEndScaleY, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Get Z rotation angle for this element and its children, in degrees
    *
    * \return rotation angle in degrees
    */
   float getZAngle (void) const;

   /**
    * Set Z rotation angle for this element and its children, in degrees
    *
    * \param fZAngle rotation angle in degrees
    */
   virtual void setZAngle (float fZAngle);

   /**
    * Animate the Z rotation angle of this element
    *
    * \param fStartZAngle starting Z rotation angle
    * \param fEndZAngle ending Z rotation angle
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending Z rotation angle
    * \param nFunction function to be applied for progressing from the starting to the ending Z rotation angle (K_COUNTER_xxx)
    */
   virtual void animateZAngle (float fStartZAngle, float fEndZAngle, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Animate the Z rotation angle of this element, from its current value
    *
    * \param fEndZAngle ending Z rotation angle
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending Z rotation angle
    * \param nFunction function to be applied for progressing from the starting to the ending Z rotation angle (K_COUNTER_xxx)
    */
   virtual void animateZAngleTo (float fEndZAngle, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Get shear factor over the X axis
    *
    * \return shear factor over the X axis
    */
   float getShearX (void) const;

   /**
    * Get shear factor over the Y axis
    *
    * \return shear factor over the Y axis
    */
   float getShearY (void) const;

   /**
    * Set shear factors over the X and Y axis
    *
    * \param fShearX shear factor over the X axis
    * \param fShearY shear factor over the Y axis
    */
   virtual void setShear (float fShearX, float fShearY);

   /**
    * Animate the shear factors of this element
    *
    * \param fStartShearX starting shear factor over the X axis
    * \param fStartShearY starting shear factor over the Y axis
    * \param fEndShearX ending shear factor over the X axis
    * \param fEndShearY ending shear factor over the Y axis
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending shear factors
    * \param nFunction function to be applied for progressing from the starting to the ending shear factors (K_COUNTER_xxx)
    */
   virtual void animateShear (float fStartShearX, float fStartShearY, float fEndShearX, float fEndShearY, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Animate the shear factors of this element, from their current value
    *
    * \param fEndShearX ending shear factor over the X axis
    * \param fEndShearY ending shear factor over the Y axis
    * \param fHoldoff delay in milliseconds before animating
    * \param fDuration duration in milliseconds to go from the starting to the ending shear factors
    * \param nFunction function to be applied for progressing from the starting to the ending shear factors (K_COUNTER_xxx)
    */
   virtual void animateShearTo (float fEndShearX, float fEndShearY, double fHoldoff, double fDuration, KCounterFunction nFunction = K_COUNTER_LINEAR);

   /**
    * Check if the position, blending factor, scaling factor or Z rotation angle are currently being animated for this element
    *
    * \return true if animating, false if not
    */
   bool isAnimating (void);

   /**
    * Set whether or not to dispatch events regardless of the animation status of an element
    *
    * \param bIgnore true to ignore the animation status and dispatch events anyway, false to avoid dispatching events to an animating element (default)
    */
   void setIgnoreAnimatingForMouseClicks (bool bIgnore);
   
   /**
    * Set whether or not to dispatch events and change element state when leaving and re-entering an element while the mouse or touch is still in progress
    *
    * \param bIgnore true to avoid sending events and changing element state on leaving and re-entry, false to send the events and change element state (default)
    */
   void setIgnoreReentry (bool bIgnore);

   /**
    * Get user transformation matrix for this element
    *
    * \return transformation matrix for this element
    */
   KMatrix getMatrix (void);

   /**
    * Set user transformation matrix for this element. The matrix is applied before scaling, shearing and other transforms
    *
    * \param mat transformation matrix for this element
    */
   virtual void setMatrix (const KMatrix &mat);

   /**
    * Get absolute element-to-screen transformation matrix that combines parent transforms, the user transformation matrix, and shearing, scaling, translation and other transforms
    *
    * \return element-to-screen matrix
    */
   KMatrix getAbsMatrix (void);

   /**
    * Get parent element to this element's transformation matrix that combines the user transformation matrix, and shearing, scaling, translation and other transforms
    *
    * \return element-to-screen matrix
    */
   KMatrix getLocalMatrix (void);

   /**
    * Get axis-aligned bounding rectangle, in screen space, for this element
    *
    * \return axis-aligned bounding rectangle
    */
   KRectangle getAbsBoundingRect (void);

   /**
    * Get axis-aligned bounding rectangle, in screen space, for this element and all its children
    *
    * \return axis-aligned bounding rectangle
    */
   KRectangle getAbsTreeBoundingRect (void);

   /**
    * Checks if this element's bounding hierarchy intersects with another specified elements' bounding hierarchy
    *
    * \param lpCheckedUIElement element to check against
    *
    * \return true if a collision is occuring, false if not
    */
   bool collidesWithElement (KUIElement *lpCheckedUIElement);

   /**
    * Get extra margin around this element that reacts to input events on touchscreens, in pixels. It is much harder to exactly hit
    * an area on a touchscreen, so a margin is recommended; it defaults to 10 pixels
    *
    * \return margin around element, in pixels
    */
   float getTouchMargin (void);

   /**
    * Set extra margin around this element that reacts to input events on touchscreens, in pixels. It is much harder to exactly hit
    * an area on a touchscreen, so a margin is recommended; it defaults to 10 pixels
    *
    * \param fTouchMargin new margin around element, in pixels
    */
   virtual void setTouchMargin (float fTouchMargin);

   /**
    * Set which mouse button triggers clicks on this element
    *
    * \param nButtonIndex index of button to use for triggering clicks on this element: K_LBUTTON, K_MBUTTON or K_RBUTTON
    */
   virtual void setButtonIndex (long nButtonIndex);

   /**
    * Get root primitive of the bounding hierarchy of this element
    *
    * \return root bounding primitive, or NULL if no hierarchy is set and the element uses the default bounding rectangle
    */
   virtual KUIBounds *getBounds (void);

   /**
    * Set root primitive of the bounding hierarchy of this element. By default, a bounding rectangle of (0, 0, width, height) is set.
    * The bounding hierarchy being set will also automatically be deleted when the element is deleted
    *
    * \param lpBounds root bounding primitive, NULL to remove hierarchy and use the default bounding rectangle
    * \param bDoNotFree true to avoid freeing previous bounding hierarchy if any; false to free previous hierarchy (default)
    */
   virtual void setBounds (KUIBounds *lpBounds, bool bDoNotFree = false);

   /**
    * Enable or disable drawing the bounding hierarchy for this element
    *
    * \param bEnable true to enable, false to disable
    * \param r red component of drawing color
    * \param g green component of drawing color
    * \param b blue component of drawing color
    * \param a alpha component of drawing color
    */
   virtual void enableDrawingBounds (bool bEnable, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

   /**
    * Enable or disable processing of collisions by this element. When collisions are enabled, this element is
    * reported collisions against all other elements through onCollisionWith()
    *
    * \param bEnable true to enable, false to disable
    */
   virtual void enableCollisions (bool bEnable);

   /**
    * Get clipping rectangle for this element, in screen space
    *
    * \param rcClipping returned clipping rectangle
    *
    * \return true if clipping is enabled for this element (and a clipping rectangle was returned), false if clipping is disabled
    */
   bool getClippingRect (KRectangle &rcClipping);

   /**
    * Enable or disable clipping of this element and its children by its screen space bounding box
    *
    * \param bEnable true to enable, false to disable
    */
   virtual void enableClipping (bool bEnable);

   /**
    * Enable or disable clipping of this element and its children using a fixed bounding box in element coordinates
    *
    * \param bEnable true to enable, false to disable
    * \param x1 X coordinate of top, left of clipping box, in element space
    * \param y1 Y coordinate of top, left of clipping box, in element space
    * \param x2 X coordinate of bottom, right of clipping box, in element space, +1
    * \param y2 Y coordinate of bottom, right of clipping box, in screen space, +1
    */
   virtual void enableElementSpaceClipping (bool bEnable, float x1 = 0, float y1 = 0, float x2 = 0, float y2 = 0);

   /**
    * Enable or disable clipping of this element and its children using a fixed bounding box in screen coordinates
    *
    * \param bEnable true to enable, false to disable
    * \param x1 X coordinate of top, left of clipping box, in screen space
    * \param y1 Y coordinate of top, left of clipping box, in screen space
    * \param x2 X coordinate of bottom, right of clipping box, in screen space, +1
    * \param y2 Y coordinate of bottom, right of clipping box, in screen space, +1
    */
   virtual void enableScreenSpaceClipping (bool bEnable, float x1 = 0, float y1 = 0, float x2 = 0, float y2 = 0);

   /**
    * Enable or disable accepting input events only in the clipped area of this element and its children
    *
    * \param bEnable true to only accept mouse and touch input events inside the clipping box (element or screen space,
    *                whichever is enabled), false to accept all events (default)
    */
   virtual void enableInputClipping (bool bEnable);

   /**
    * Enable or disable drawing all siblings of this element's children first, before their own children are drawn
    *
    * \param bEnable true to enable, false to disable
    */
   virtual void enableDrawingSiblingsFirst (bool bEnable);

   /**
    * Allow or disallow the bounding hierarchy to accept input coordinates outside of the element (0,0,width,height) rectangle
    *
    * \param bAllow true to allow, false to disallow (default)
    */
   virtual void allowBoundsOutsideElement (bool bAllow);

   /**
    * Check whether this element is a container
    *
    * \return true if this element is a container, false if not
    */
   bool isContainer (void) const;

   /**
    * Set or unset this element as a container
    *
    * \param bIsContainer true to flag this element as a container (allow drawing element and its children even if the width and height are 0), false not to (default)
    */
   virtual void setAsContainer (bool bIsContainer);

   /**
    * Check whether this element and all its children's logic is disabled from updating
    *
    * \return true if logic updates are disabled for this element all its children, false if they are enabled (default)
    */
   bool isFrozen (void) const;

   /**
    * Disable or enable updating this element and all its children's logic
    *
    * \param bFreeze true to disable updating this element and all its children's logic, false to enable updates (default)
    */
   virtual void freeze (bool bFreeze);

   /**
    * Define whether the rotation is applied before the scale in the matrix transform of this element, or if the scale is applied first
    *
    * \param bRotationBeforeScale true to apply rotation before scale, false to apply scale before rotation (default)
    */
   virtual void applyRotationBeforeScale (bool bRotationBeforeScale);

   /**
    * Define whether this element only uses its parent's position for matrix transforms, and not the scale, shear or other factors; or if it uses all factors
    *
    * \param bUseParentPositionOnly true to use only the parent's position, false to use its whole matrix (default)
    */
   virtual void useParentPositionOnly (bool bUseParentPositionOnly);

   /**
    * Set whether this element will draw itself aligned on the pixel grid, ignoring subpixel coordinates
    *
    * \param bEnablePixelGridAlignment true to enable pixel grid alignment, false to disable (default)
    */
   virtual void setPixelGridAlignment(bool bEnablePixelGridAlignment);

   /**
    * Check if this element's footprint has changed (translated, rescaled, shear modified..) since the last time this method was called
    *
    * \return true if changed, false if not
    */
   bool hasElementChanged (void);

   /**
    * Set or unset this element as a stencil mask
    *
    * \param bIsStencilMask true to render this element to the stencil buffer as a mask, false to render normally as graphics to the color buffer (default)
    * \param fAlphaThreshold when this element is set as a stencil mask, minimum alpha value to write to the stencil buffer
    * \param bReverseMask true if pixels lit by this element are masked out; false if pixels not lit by this element are masked out
    */
   virtual void setAsStencilMask (bool bIsStencilMask, float fAlphaThreshold = 0.01f, bool bReverseMask = false);

   /**
    * Set pixel shader to use for drawing this element and optionally its children
    *
    * \param lpPixelShader pixel shader, or NULL for none
    * \param bSetForChildren true to use this pixel shader for drawing the element's children (unless they set their own shader), false not to
    */
   virtual void setPixelShader (KPixelShader *lpPixelShader, bool bSetForChildren);

   /**
    * Set value of a parameter in the pixel shader used to draw this element
    *
    * \param nParam index of parameter to set
    * \param fValue1 first component of value
    * \param fValue2 second component of value
    * \param fValue3 third component of value
    * \param fValue4 fourth component of value
    */
   virtual void setPixelShaderParam (long nParam, float fValue1, float fValue2 = 0, float fValue3 = 0, float fValue4 = 0);

   /**
    * During an event dispatch, get element that triggered the event (normally the element itself, but can be a child of a container as well, where the event bubbled up to
    * the parent container)
    *
    * \return caught element
    */
   KUIElement *getCaughtElement (void);

   /** 
    * Check if scripting language VM instances will be called for this element when an event occurs
    *
    * \return true if this element has scripts attached, false if not
    */
   bool hasScripts (void);

   /** 
    * Attach scripting language VM instance whose functions will be called when an event occurs. For instance, the "onMouseOver" function will
    * be called in the specified VM when onMouseOver() is dispatched
    *
    * \param lpScript script to be added, NULL to remove
    */
   virtual void addScript (KScript *lpScript);

   /** 
    * Detach scripting language VM instance from this element
    *
    * \param lpScript script to be removed, NULL to remove
    */
   virtual void removeScript (KScript *lpScript);

   /**
    * While in a script, get element that performed the call
    *
    * \return caller element
    */
   static KUIElement *getScriptCallerElement (void);

   /**
    * Execute script code in some or all scripts attached to this element
    *
    * \param lpszCode code to be executed
    * \param bCallAllScripts true to call all scripts attached to this element, false to call scripts only until the execution in one is successful
    */
   void executeScript (const char *lpszCode, bool bCallAllScripts = true);

   /**
    * Add specified class to the hierarchy of classes that this element is part of; this is used
    * to handle the isXXX() methods, for instance KUIButton::isButton() 
    *
    * \param lpszClassName name of class to declare for this element
    */
   void addClass (const char *lpszClassName);

   /**
    * Check if this element is of the specified class, somewhere in its hierarchy; this is used
    * to handle the isXXX() methods, for instance KUIButton::isButton() 
    *
    * \param lpszClassName name of class to be checked
    *
    * \return true if element is of the specified class, false if not
    */
   bool isOfClass (const char *lpszClassName);

   /**
    * Get name of a class that this element is part of
    *
    * \param nDepth depth to inspect in the class stack (0=last entry added by addClass(), 1=second to last, ...)
    *
    * \return class name, NULL if reached beyond maximum depth
    */
   const char *getClassName (long nDepth = 0);

   /**
    * Disable sending messages for this element
    *
    * \param bDisable true to disable, false to re-enable
    */
   void disableMessages (bool bDisable);

   /**
    * Call a function in the script attached to this element, if one is set, with no parameters
    *
    * \param lpszFunction name of function to be called in script
    */
   void callScript (const char *lpszFunction);

   /**
    * Call a function in the script attached to this element, if one is set, with an integer parameter
    *
    * \param lpszFunction name of function to be called in script
    * \param nValue value passed to function
    */
   void callScriptInt (const char *lpszFunction, long nValue);

   /**
    * Call a function in the script attached to this element, if one is set, with a string parameter
    *
    * \param lpszFunction name of function to be called in script
    * \param lpszValue value passed to function
    */
   void callScriptStr (const char *lpszFunction, const char *lpszValue);

   /**
    * Call function in the script defined for this element, taking and returning a variable set of parameters
    *
    * \param lpszFuncName name of function to be called
    * \param nParams number of input parameters to pass to the script function
    * \param lpszParamType string containing the type of each input parameter, in order; 's' for string, 'i' for integer and 'f' for float. A function taking a float as first parameter and string as second would be coded as "fs"
    * \param param array of input parameters
    * \param nRetVals number of output values returned by the script function
    * \param lpszRetValType string containing the type of each output value, in order; a function returning two integers would be coded as "ii"
    * \param retVal array of output values
    *
    * \return true for success, false for failure
    */
   bool callScriptVar (const char *lpszFuncName, long nParams, const char *lpszParamType, void *param[], long nRetVals, const char *lpszRetValType, void *retVal[]);

   /**
    * Draw this element and all its children, recursively
    *
    * \param nLayer layer number
    * \param lpMatrix world transformation matrix applied for this element and its children
    */
   void blitElements (long nLayer, KMatrix *lpMatrix);

   /** 
    * Get graphic for drawing boxes and lines
    *
    * \private
    *
    * \return graphic
    */
   static KGraphic *getBoxGraphic (void);

   /**
    * Set number of vertices in the vertex buffer
    *
    * \param nVertexCount new number of vertices
    */
   static void setBatchVertexCount (long nVertexCount);

   /**
    * Get vertex buffer
    *
    * \private
    *
    * \return vertex buffer
    */
   static KBatch *getBatch (void);

   /**
    * Begin batches for the specified texture
    *
    * \param lpGraphic texture to use
    * \param nSrcAlphaOp source alpha blending operation to use for texture
    * \param nDstAlphaOp destination alpha blending operation to use for texture
    * \param bModifyAlphaOp true if the alpha blending operation will need to be restored
    * \param bModifyColor true if the texture color will need to be restored
    */
   static void setBatchedTexture (KGraphic *lpGraphic, eKanjiAlphaOp nSrcAlphaOp = K_ALPHAOP_SRC_ALPHA, eKanjiAlphaOp nDstAlphaOp = K_ALPHAOP_ONE_MINUS_SRC_ALPHA,
                                  bool bModifyAlphaOp = false, bool bModifyColor = false);

   /**
    * Begin batches for the specified texture, using seperate color and alpha blending modes
    *
    * \param lpGraphic texture to use
    * \param nSrcAlphaOp source alpha blending operation to use for the texture's color channel
    * \param nDstAlphaOp destination alpha blending operation to use for the texture's color channel
    * \param nAlphaSrcAlphaOp source alpha blending operation to use for the texture's alpha channel
    * \param nAlphaDstAlphaOp destination alpha blending operation to use for the texture's alpha channel
    * \param bModifyAlphaOp true if the alpha blending operation will need to be restored
    * \param bModifyColor true if the texture color will need to be restored
    */
   static void setBatchedTextureExt (KGraphic *lpGraphic, eKanjiAlphaOp nSrcAlphaOp = K_ALPHAOP_SRC_ALPHA, eKanjiAlphaOp nDstAlphaOp = K_ALPHAOP_ONE_MINUS_SRC_ALPHA,
                                     eKanjiAlphaOp nAlphaSrcAlphaOp = K_ALPHAOP_SRC_ALPHA, eKanjiAlphaOp nAlphaDstAlphaOp = K_ALPHAOP_ONE_MINUS_SRC_ALPHA,
                                     bool bModifyAlphaOp = false, bool bModifyColor = false);

   /** Reset number of batching operations */
   static void resetBatchingOpsCount (void);

   /**
    * Get number of batching operations (texture changes) since resetBatchingOpsCount() was called
    *
    * \return number of batching operations
    */
   static long getBatchingOpsCount (void);

protected:
   /**
    * Handle that the mouse just moved over this element. Derive this method in a child element
    */
   virtual void onMouseOver (void);

   /**
    * Handle that the mouse just left this element. Derive this method in a child element
    */
   virtual void onMouseLeave (void);

   /**
    * Handle that this element just started being pressed down. Derive this method in a child element
    */
   virtual void onDown (void);

   /**
    * Handle that this element is currently being pressed down. Derive this method in a child element
    */
   virtual void onCurrentlyDown (void);

   /**
    * Handle that this element just stopped being pressed down. Derive this method in a child element
    */
   virtual void onUp (void);

   /**
    * Handle that this element has just been selected. Derive this method in a child element
    */
   virtual void onSelected (void);

   /**
    * Handle that this element has just been deselected. Derive this method in a child element
    */
   virtual void onDeselected (void);

   /**
    * Handle that the mouse just moved over a child of this element. Derive this method in a child element
    */
   virtual void onRollOver (void);

   /**
    * Handle that the mouse just left a child of this element. Derive this method in a child element
    */
   virtual void onRollOut (void);

   /**
    * Handle that a child of this element just started being pressed down. Derive this method in a child element
    */
   virtual void onChildDown (void);

   /**
    * Handle that a child of this element just stopped being pressed down. Derive this method in a child element
    */
   virtual void onChildUp (void);

   /**
    * Handle that this element just completed animating its properties, following an animateScaleXY(), animatePosition().. Derive this method in a child element
    */
   virtual void onDoneAnimating (void);

   /**
    * Handle that this element is currently colliding with another one
    *
    * \param lpCollidingElement elemenet colliding with this one
    */
   virtual void onCollisionWith (KUIElement *lpCollidingElement);

   /**
    * Handle hit test: check whether the supplied coordinates are actually over the element or not. The default
    * implementation always returns true, which makes the entire element's rectangle active. This method can be
    * derived to implement elements such as circular buttons
    *
    * \param x X coordinate (expressed in the element's coordinate space)
    * \param y Y coordinate (expressed in the element's coordinate space)
    *
    * \return true if coordinates are over the element, false if not
    */
   virtual bool onHit (float x, float y);

   /**
    * Handle that a swipe gesture is being executed on this element
    *
    * \param nGestureType current stage for the gesture (K_GESTUREBEGINS, K_GESTURECONTINUES, K_GESTUREENDS)
    * \param dx X displacement, in pixels
    * \param dy Y displacement, in pixels
    *
    * \return true if this element handled the gesture, false if not and the gesture will be passed on to the parent elements
    */
   virtual bool onSwipe (long nGestureType, float dx, float dy);

   /**
    * Handle input event for this element
    *
    * \param lpKEvent event
    *
    * \return true if event was processed by this element, false if it wasn't
    */
   virtual bool handleEvent (KEvent *lpKEvent);

   /**
    * Handle input event for this element
    *
    * \param lpszEvent event string
    * \param lpData event data
    *
    * \return true if event was processed by this element, false if it wasn't
    */
   virtual bool handleUserEvent (const char *lpszEvent, const void *lpData);

   /**
    * Handle the loss of the mouse capture, when this element was clicked, then the mouse button is finally released anywhere on screen
    */
   virtual void handleMouseCaptureLoss (void);

   /**
    * Handle keyboard focus
    *
    * \param bAcquire true if acquiring it, false if losing it
    *
    * \return if acquiring the focus, true if focus was accepted, false if it wasn't (element doesn't handle the keyboard) ; ignored when losing the focus
    */
   virtual bool handleFocus (bool bAcquire);

   /**
    * Handle a child having been added, removed or resized
    */
   virtual void handleChildUpdate (void);

   /**
    * Enable or disable setting the absolute transformation matrix of this element before blit() and blitPost()
    *
    * \param bSet true to set it (default), false not to
    */
   void setBlitMatrix (bool bSet);

   /** Update logic for this element */
   virtual void move (void);

   /**
    * Check if this element has opaque parts. If it does, and drawing with the depth buffer is enabled, and this element is fully blended in, blitOpaque() will be called to
    * draw the opaque parts front-to-back, and blitTransparent() will be called to draw the transparent parts back-to-front, instead of blit(). Otherwise, blit() will be
    * called (default)
    *
    * \return true if element has opaque parts to be seperately drawn front-to-back if possible, false if not
    */
   virtual bool hasOpaqueParts (void);

   /**
    * Draw this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blit (float fBlend, KMatrix *lpMatrix);

   /**
    * Draw opaque parts of this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blitOpaque (float fBlend, KMatrix *lpMatrix);

   /**
    * Draw transparent parts of this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blitTransparent (float fBlend, KMatrix *lpMatrix);

   /**
    * Draw once this element and its children have been drawn
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blitPost (float fBlend, KMatrix *lpMatrix);

   /**
    * Send a message to a message callback
    *
    * \param nMessage message to be sent
    * \param fParam1 first optional parameter
    * \param fParam2 second optional parameter
    * \param nParam3 third optional parameter
    * \param nDefaultReply default reply to return, if no callback processed this message
    * \param lpszParam4 fourth optional parameter
    * \param lpParam5 fifth optional parameter
    *
    * \return reply
    */
   long sendMessage (long nMessage, float fParam1 = 0, float fParam2 = 0, long nParam3 = 0, long nDefaultReply = 0, const char *lpszParam4 = NULL, KUIElement *lpParam5 = NULL);

   /** X coordinate of top,left of element, relative to parent */
   float _x;

   /** Y coordinate of top,left of element, relative to parent */
   float _y;

   /** Width of element in pixels */
   float _fWidth;

   /** Height of element in pixels */
   float _fHeight;

   /** Game window */
   static KWindow *g_lpKWindow;

   /** Root element */
   static KUIElement *g_lpRootElem;

   /** Graphic for drawing boxes and lines */
   static KGraphic *g_lpBoxGraphic;

private:
   /**
    * Dispatch mouse input event to this element's children and then this element, until it is handled
    *
    * \param lpKEvent event
    * \param mat current transformation matrix at this stage
    * \param lpMatchElement element that the event is to be delivered to exclusively, NULL to dispatch it until handled
    * \param x X coordinate of mouse event in screen space
    * \param y Y coordinate of mouse event in screen space
    * \param bParentIsContainer true if a parent of this element is a container
    *
    * \return true if processed
    */
   bool dispatchMouseEventToSubTree (KEvent *lpKEvent, KUIElement *lpMatchElement, float x, float y, bool bParentIsContainer);

   /**
    * Dispatch touch input event to this element's children and then this element, until it is handled
    *
    * \param lpKEvent event
    * \param mat current transformation matrix at this stage
    * \param lpMatchElement element that the event is to be delivered to exclusively, NULL to dispatch it until handled
    * \param x X coordinate of touch event in screen space
    * \param y Y coordinate of touch event in screen space
    * \param bParentIsContainer true if a parent of this element is a container
    *
    * \return true if processed
    */
   bool dispatchTouchEventToSubTree (KEvent *lpKEvent, KUIElement *lpMatchElement, float x, float y, bool bParentIsContainer);

   /**
    * Dispatch other (not mouse or touch) input event to this element's children and then this element, until it is handled
    *
    * \param lpKEvent event
    * \param mat current transformation matrix at this stage
    * \param lpMatchElement element that the event is to be delivered to exclusively, NULL to dispatch it until handled
    * \param bParentIsContainer true if a parent of this element is a container
    *
    * \return true if processed
    */
   bool dispatchOtherEventToSubTree (KEvent *lpKEvent, KUIElement *lpMatchElement, bool bParentIsContainer);

   /**
    * Dispatch user event to this element's children and then this element, until it is handled
    *
    * \param lpszEvent event string to be dispatched
    * \param lpData event data
    *
    * \return true if processed
    */
   bool dispatchUserEventToSubTree (const char *lpszEvent, const void *lpData);

   /**
    * Handle input event for this element if it wasn't processed by a subclass
    *
    * \param lpKEvent event
    *
    * \return true if event was processed by this element, false if it wasn't
    */
   virtual bool handleEventInternal (KEvent *lpKEvent);

   /**
    * Find element that would receive input at the specified coordinates, checking this element and its children, until it is found
    *
    * \param x X coordinate to pick element at; if successful, the coordinate is updated to be in the returned element's coordinate space
    * \param Y Y coordinate to pick element at; if successful, the coordinate is updated to be in the returned element's coordinate space
    * \param bAllElements true to search all elements, false to only search elements that currently accept input
    *
    * \return element found at these coordinates, NULL if none
    */
   KUIElement *pickElementInSubTree (float &x, float &y, bool bAllElements);

   /** Copy this element and its children's linkage, before doing an update or event dispatch, without updating the footprint of elements that changed */
   void copySubTreeNoFootprint (void);

   /**
    * Update this element's footprint if it changed and the element is visible, as well as its children's, recursively
    *
    * \param bMatChanged true if any of the parent's matrices has changed
    * \param fBlend current blending factor at this stage
    * \param lpParentMatrix parent's absolute screen to element transformation matrix
    */
   void updateFootprintSubTree (bool bMatChanged, float fBlend, const KMatrix *lpParentMatrix);

   /**
    * Update this element's footprint if it changed, whether the element is visible or not, after first updating the parent's, recursively
    *
    * \return true if this element's footprint was updated, (or recursively, its parent's), false if not
    */
   bool updateFootprintAndParent (void);

   /**
    * Copy this element and its children's linkage, before doing an update or event dispatch, and also update this
    * element's forward matrix, axis-aligned bounding box and spatial hash if they changed, as well as its children's, recursively
    *
    * \param bMatChanged true if any of the parent's matrices has changed
    * \param fParentBlend current blending factor at this stage
    * \param lpParentMatrix parent's absolute screen to element transformation matrix
    */
   void copyUpdateFootprintSubTree (bool bMatChanged, float fParentBlend, const KMatrix *lpParentMatrix);

   /**
    * Update logic for this element and its children
    */
   void moveSubTree (void);

   /**
    * Draw this element and its children
    *
    * \param nLayer layer number
    * \param bMatChanged true if any of the parent's matrices has changed
    * \param fBlend current blending factor at this stage
    * \param lpParentMatrix parent's absolute screen to element transformation matrix
    * \param nChildDrawMode child drawing mode: 0 = normal, 1 = draw child's siblings only, 2 = draw child's children only
    */
   void blitSubTree (long nLayer, bool bMatChanged, float fBlend, const KMatrix *lpParentMatrix, long nChildDrawMode);
   
   /**
    * Set depth value for this element and its children
    */
   void setDepthValueForSubTree (void);

   /**
    * Draw opaque parts of this element and its children
    *
    * \param nLayer layer number
    * \param bMatChanged true if any of the parent's matrices has changed
    * \param fBlend current blending factor at this stage
    * \param lpParentMatrix parent's absolute screen to element transformation matrix
    * \param nChildDrawMode child drawing mode: 0 = normal, 1 = draw child's siblings only, 2 = draw child's children only
    */
   void blitOpaqueSubTree (long nLayer, bool bMatChanged, float fBlend, const KMatrix *lpParentMatrix, long nChildDrawMode);

   /**
    * Draw transparent parts of this element and its children
    *
    * \param nLayer layer number
    * \param bMatChanged true if any of the parent's matrices has changed
    * \param fBlend current blending factor at this stage
    * \param lpParentMatrix parent's absolute screen to element transformation matrix
    * \param nChildDrawMode child drawing mode: 0 = normal, 1 = draw child's siblings only, 2 = draw child's children only
    */
   void blitTransparentSubTree (long nLayer, bool bMatChanged, float fBlend, const KMatrix *lpParentMatrix, long nChildDrawMode);

   /**
    * Recursively check if the specified element is this one or one of its children
    *
    * \param lpCheckedElement element to be checked
    *
    * \return true if element is part of the subtree, false if not
    */
   bool isElementInSubtree (KUIElement *lpCheckedElement);

   /**
    * Recursively replace layer number in subtree
    *
    * \param nPrevLayer layer number to replace
    * \param nNewLayer layer number to replace it by
    */
   void replaceLayerInSubtree (long nPrevLayer, long nNewLayer);

   /**
    * Update this element's forward (element space to screen space) matrix, axis aligned bounding box and spatial hash
    *
    * \param lpParentMatrix forward matrix of this element's parent
    */
   void updateFootprint (const KMatrix *lpParentMatrix);

   /** Perform collisions on this element */
   void collide (void);

   /**
    * Recursively remove a script from all elements, if it is deleted without being removed and before the elements using it are deleted
    *
    * \param lpScript script to removed
    */
   void unlinkScript (KScript *lpScript);

   /**
    * Insert this element in a spatial hash bucket
    *
    * \param nBucketIdx index of bucket to insert element in
    */
   inline void insertInSpatialHash (long nBucketIdx);

   /**
    * Remove this element from the spatial hash bucket it is currently in
    */
   inline void removeFromSpatialHash (void);

   /** Begin an update or event dispatch and update the footprint of visible elements that have changed */
   static void beginUpdateWithFootprint (void);

   /** Begin an update or event dispatch and don't update the footprint of elements that have changed */
   static void beginUpdateNoFootprint (void);

   /** Finish an update or event dispatch */
   static void endUpdate (void);

   /** true if this element has been deleted (but not collected yet) */
   bool _bDeleted;

   /** Number of classes in the element's hierarchy */
   long _nClasses;

   /** Classes in the element's hierarchy */
   char _szClass[K_UIELEMENT_MAXCLASSES][K_UIELEMENT_MAXCLASSNAMELEN];

   /** Parent element (NULL only for the root) */
   KUIElement *_lpParentElem;

   /** List of child elements */
   KList<KUIElement> _lChildren;

   /** During an update or event dispatch, parent element (NULL only for the root) */
   KUIElement *_lpUpdateParentElem;

   /** During an update or event dispatch, first child in list */
   KUIElement *_lpUpdateFirstChild;

   /** During an update or event dispatch, last object in list */
   KUIElement *_lpUpdateLastChild;

   /** Next sibling in list */
   KUIElement *_lpUpdateNextSibling;

   /** Previous sibling in list */
   KUIElement *_lpUpdatePrevSibling;

   /** Index of spatial hash that this element is inserted in */
   long _nSpatialHashBucketIdx;

   /** Next in spatial hash bucket */
   KUIElement *_lpSpatialHashNext;

   /** Previous in spatial hash bucket */
   KUIElement *_lpSpatialHashPrev;

   /** For elements that have an identifier, next into list of elements that share the same hashtable bucket */
   KUIElement *_lpNextInIdBucket;

   /** Current state */
   KUIElementState _nState;

   /** Layer number for this element */
   long _nLayer;

   /** true if this element processes input events */
   bool _bElemInputEnabled;

   /** true if this element's hierarchy processes input events */
   bool _bTreeInputEnabled;

   /** true if this element and its children do not update during moveAll() (and also do not receive input) */
   bool _bFrozen;

   /** true to apply rotation before scale, false to apply scale before rotation (default) */
   bool _bApplyRotationBeforeScale;

   /** true to only use the parent position for matrix transforms, and not the scale, shear or other factors */
   bool _bUseParentPositionOnly;

   /** true if this element draw itself aligned to the pixel grid, ignoring subpixel coordinates */
   bool _bEnablePixelGridAlignment;

   /** true if this element is tested for collisions */
   bool _bCollisionsEnabled;

   /** true if clipping this element and its children using this element's screen space bounding box */
   bool _bBoundingBoxClippingEnabled;

   /** true if clipping this element and its children using a fixed bounding box in element space */
   bool _bFixedElemClippingEnabled;

   /** true if clipping this element and its children using a fixed bounding box in screen space */
   bool _bFixedScrClippingEnabled;

   /** true to also accept mouse and touch input events only inside the clipped area, either the bounding box or fixed screen space box, whichever is enabled */
   bool _bClipInput;

   /** true to allow the bounding hierarchy to accept input coordinates outside of the element (0,0,width,height) rectangle, false not to (default) */
   bool _bAllowBoundsOutsideElem;

   /** true to draw all siblings of this element's children first, before their own children are drawn */
   bool _bDrawSiblingsFirst;

   /** true if this element is a stencil buffer mask, false if not */
   bool _bIsStencilMask;

   /** When this element is set as a stencil buffer mask, minimum alpha value to set pixels in the stencil buffer */
   float _fStencilAlphaThreshold;

   /** When this element is set as a stencil buffer mask, true if pixels lit by this element let pixels through, false if pixels not lit by this element do */
   bool _bReverseStencilMask;

   /** true to draw this element and its children even if the width and height are 0 */
   bool _bIsContainer;

   /** Child element, if this container element received an event that bubbled up from a child that doesn't have inputs enabled, during an event dispatch */
   KUIElement *_lpContainerHitChildElem;

   /** Pixel shader to use for drawing this element, NULL for none */
   KPixelShader *_lpPixelShader;

   /** true if pixel shader is used for drawing this element's children as well */
   bool _bSetPixelShaderForChildren;

   /** Parameters to pass to the pixel shader */
   float _fPixelShaderParam[4][4];

   /** Value to use for this element's depth, when drawing with the depth buffer */
   float _fDepthValue;

   /** Static callback receiving messages for this element and its children, NULL to look at parent's */
   kanjiUiMessageCallBack _lpMessageCallBack;

   /** Callback method and instance receiving messages for this element and its children, NULL to look at parent's */
   KUIMessageFunctionBase *_lpMessageFunction;

   /** true if this element doesn't send messages */
   bool _bDisableMessages;

   /** List of scripting language VMs to be called for default handling of on..XXX() */
   KList<KUIElementScriptNode> _lScripts;

   /** true if center was set from normalized coordinates and needs to be updated when size changes */
   bool _bCenterNormalized;

   /** X offset from center, for rotation and scaling */
   float _cx;

   /** Y offset from center, for rotation and scaling */
   float _cy;

   /** X offset from center, in normalized coordinates, if set */
   float _fNormalizedXCenter;

   /** Y offset from center, in normalized coordinates, if set */
   float _fNormalizedYCenter;

   /** X offset to apply locally to this element's position (defaults to 0, setting the position as the left edge of the element) */
   float _fPositionOffsetX;

   /** Y offset to apply locally to this element's position (defaults to 0, setting the position as the top edge of the element) */
   float _fPositionOffsetY;

   /** true if anchor has been defined, false if it follows width / 2, height / 2 */
   bool _bAnchorDefined;

   /** true if anchor was set from normalized coordinates and needs to be updated when size changes */
   bool _bAnchorNormalized;

   /** X coordinate of the anchor, that the pivot is offset against, for rotation and scaling */
   float _fXAnchor;

   /** Y coordinate of the anchor, that the pivot is offset against, for rotation and scaling */
   float _fYAnchor;

   /** X coordinate of the anchor, in normalized coordinates, if set */
   float _fNormalizedXAnchor;

   /** Y coordinate of the anchor, in normalized coordinates, if set */
   float _fNormalizedYAnchor;

   /** Shear factor over the X axis */
   float _fShearX;

   /** Shear factor over the Y axis */
   float _fShearY;

   /** Blending factor, 0..1 */
   float _fBlend;

   /** true if element is visible */
   bool _bVisible;

   /** This element receives input even if it isn't visible */
   bool _bDispatchInputIfInvisible;

   /** Z rotation angle in degrees */
   float _fZAngle;

   /** Horizontal scaling factor, > 0 */
   float _fScaleX;

   /** Vertical scaling factor, > 0 */
   float _fScaleY;

   /** User transformation matrix */
   KMatrix _matUser;

   /** Forward matrix to go from the parent coordinates space to this element's space (includes transformations such as translation and scaling, as well as the user matrix) */
   KMatrix _matLocal;

   /** Extra active margin around this element when using a touchscreen */
   float _fTouchMargin;
   
   /** Index of mouse button that triggers clicks on this element (defaults to K_LBUTTON) */
   long _nButtonIndex;

   /** Accumulated (absolute) blending factor for this element; ie. its local blending factor multiplied by all the parents' */
   float _fAccBlend;

   /** Root bounding primitive in the bounding hierarchy for this element */
   KUIBounds *_lpRootBounds;

   /** Default bounding hierarchy */
   KUIBounds _defaultBounds;

   /** Axis-aligned bounding box for this element, in screen space */
   KRectangle _rcBoundingBox;

   /** Axis-aligned bounding box for this element and all its children, in screen space */
   KRectangle _rcTreeBoundingBox;

   /** true if the matrices for this element and all its children, children's children, etc. need to be updated */
   bool _bMatTreeChanged;

   /** true if the forward and inverse matrix for this element need to be updated */
   bool _bMatChanged;

   /** true if the inverse matrix for this element needs to be updated */
   bool _bMatInverseChanged;

   /** true if this element has changed (its matrix was updated) since the last check */
   bool _bElementChanged;

   /** true to set matrix before calling blit() and blitPost() (default), false not to */
   bool _bSetBlitMatrix;

   /** Forward matrix for transforming coordinates from element space to screen space */
   KMatrix _mat;

   /** Inverse matrix for transforming coordinates from screen space to element space */
   KMatrix _matInverse;

   /** true if this element's bounds are to be drawn */
   bool _bDrawElemBounds;

   /** Color for drawing this element's bou nds */
   float _fElemBoundsColor[4];

   /** Element's name, as set by the game */
   char _szElemName[K_UIELEMENT_MAXNAMELEN];

   /** Element's ID, as set by the game */
   long _nElemId;

   /** User data associated with this element */
   void *_lpUserData;

   /** Mask of running animation counters */
   long _nAnimating;

   /** true if ignoring the animation status and dispatch events anyway, false if not dispatching events to an animating element (default) */
   bool _bIgnoreAnimatingStatus;
   
   /** true to avoid sending events and changing element state on leaving and re-entry, false to send the events and change element state (default) */
   bool _bIgnoreReentry;

   /** Counter for animating the X coordinate of the position */
   KCounter _counterPosX;

   /** Counter for animating the Y coordinate of the position */
   KCounter _counterPosY;

   /** Counter for animating the blending factor */
   KCounter _counterBlend;

   /** Counter for animating the horizontal scale */
   KCounter _counterScaleX;

   /** Counter for animating the vertical scale */
   KCounter _counterScaleY;

   /** Counter for animating the Z angle */
   KCounter _counterZAngle;

   /** Counter for animating the shear factor over the X axis */
   KCounter _counterShearX;

   /** Counter for animating the shear factor over the Y axis */
   KCounter _counterShearY;

   /** Fixed bounding box for clipping in element space */
   KRectangle _rcFixedElemBoundingBox;

   /** Fixed bounding box for clipping in screen space */
   KRectangle _rcFixedScrBoundingBox;

   /** true if any element set a message handler callback */
   static bool g_bMessageCallBackSet;

   /** Correct touch margin behavior is enabled */
   static bool g_bTouchMarginEnabled;

   /** Parent of elements that don't have a parent set yet */
   static KUIElement *g_lpOrphansRootElem;

   /** Element currently under the mouse, NULL for none */
   static KUIElement *g_lpMousedOverElem;

   /** Element currently being pressed down, NULL for none */
   static KUIElement *g_lpDownElem;

   /** Element currently being swiped, NULL for none */
   static KUIElement *g_lpSwipedElem;

   /** Element that currently has the keyboard focus, NULL for none */
   static KUIElement *g_lpHasFocusElem;

   /** Element currently calling a script, NULL for none */
   static KUIElement *g_lpScriptCallerElem;

   /** > 0 to defer collecting element's memory until it is safe to do so */
   static long g_nDeferDeleteDepth;

   /** Array of elements whose deletion was deferred during an update or event dispatch, and that need to be collected */
   static KUIElement **g_lpDeletedElemArray;

   /** Current number of elements to be deleted when it is safe to do so */
   static size_t g_nDeletedElemArraySize;

   /** Current allocated size of the array of elements to be deleted */
   static size_t g_nDeletedElemArrayMaxSize;

   /** Identity matrix */
   static KMatrix g_matIdentity;

   /** true if left mouse button was clicked since last check, and not absorbed by an element */
   static bool g_bLeftButtonClicked;

   /** true if right mouse button was clicked since last check, and not absorbed by an element */
   static bool g_bRightButtonClicked;

   /** true if middle mouse button was clicked since last check, and not absorbed by an element */
   static bool g_bMiddleButtonClicked;

   /** On touchscreens, true when a touch is in progress */
   static bool g_bTouching;

   /** true if we need to recalculate which element is under the mouse */
   static bool g_bRefreshMousedOverElem;

   /** While rendering, current number of stacked elements that have enabled the stencil buffer */
   static long g_nStencilBufferDepth;

   /** Pixel shader currently used for drawing, NULL for none */
   static KPixelShader *g_lpCurPixelShader;

   /** Parameters for currently used pixel shader */
   static float g_fCurPixelShaderParam[4][4];

   /** Current number of instances of KUIElement */
   static long g_nInstances;

   /** Number of elements moved in the last logic update */
   static long g_nUpdatedElements;

   /** Number of elements blitted in the last frame */
   static long g_nDrawnElements;

   /** Number of vertices in the vertex buffer */
   static long g_nBatchVertexCount;

   /** Vertex buffer */
   static KBatch *g_lpBatch;

   /** Currently batched texture */
   static KGraphic *g_lpCurBatchedGraphic;

   /** Current alpha blending ops for batched texture */
   static eKanjiAlphaOp g_nCurBatchedAlphaOp[4];

   /** true if currently batched texture's alpha blending ops need to be restored */
   static bool g_bBatchedGraphicAlphaOpsModified;

   /** Original alpha blending ops for batched texture */
   static eKanjiAlphaOp g_nOrigBatchedAlphaOp[4];

   /** true if currently batched texture's color needs to be restored */
   static bool g_bBatchedGraphicColorModified;

   /** Original color of batched texture */
   static float g_fBatchedOrigColor[4];

   /** Number of batching operations since the counter was reset */
   static long g_nBatchingOpsCount;

   /** true if drawing with the depth buffer is enabled */
   static bool g_bDepthBufferEnabled;

   /** When drawing with the depth buffer, next depth value to be assigned */
   static float g_fCurDepthValue;

   /** How much to increase depth value for, for each element */
   static float g_fDepthValueStep;

   /** true if currently clipping */
   static bool g_bClippingEnabled;

   /** Current clipping rectangle, if enabled */
   static KRectangle g_rcClipping;

   /** Hashtable of all elements that have a name */
   static KHashTable g_hashNamedElements;

   /** First hash entry into list of elements with an identifier, that share the same bucket number */
   static KUIElement *g_lpFirstIdentifiedElem[K_UIELEMENT_IDBUCKETS];

   /** Offset into spatial hash buckets array for each cell size */
   static long g_nBucketOffset[6];

   /** First hash entry into doubly linked list of elements for the spatial hash, that share the same cell */
   static KUIElement *g_lpFirstInSpatialHash[32*32+16*16+8*8+4*4+2*2+1*1+1];

   /** Last hash entry into doubly linked list of elements for the spatial hash, that share the same cell */
   static KUIElement *g_lpLastInSpatialHash[32*32+16*16+8*8+4*4+2*2+1*1+1];
};

#endif   /* _KANJI_UIELEMENT_H */

