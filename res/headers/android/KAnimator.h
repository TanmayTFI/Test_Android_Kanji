/**
 * Kanji game engine - value animator definitions
 *
 * (c) 2010
 *
 * \file KAnimator.h
 */

#ifndef  _KANJI_KANIMATOR_H
#define  _KANJI_KANIMATOR_H

/* Include definitions */
#include "KObjectBase.h"
#include <vector>

/** Functions that can be used for interpolating from one value to the other */
typedef enum {
   K_ANIMATOR_LINEAR = 0,         /**< Linear interpolation */
   K_ANIMATOR_EASEIN,             /**< Start slowly and then accelerate to the target value */
   K_ANIMATOR_EASEOUT,            /**< Start like the linear interpolation and then decelerate to the target value */
   K_ANIMATOR_EASEINOUT,          /**< Start slowly, accelerate and then decelerate to the target value */
   K_ANIMATOR_ACCELERATE,         /**< Linear acceleration to the target value */
   K_ANIMATOR_DECELERATE,         /**< Linear deceleration to the target value */
} KAnimatorFunction;

/** Number of loops to animate indefinitely */
#define K_ANIMATOR_INFINITE   (-1)

/**
 * Class that interpolates values according to keyframes
 */

class KAnimator {
public:
   /** Constructor */
   KAnimator();   

   /** Destructor */
   ~KAnimator(void);

   /** Empty the list of keyframes */
   void clearKeyframes (void);

   /**
    * Add keyframe
    *
    * \param fTime time at this keyframe
    * \param nFunction function used for interpolating to this keyframe (K_ANIMATOR_xxx)
    * \param nId keyframe identifier
    * \param fValue first value at this keyframe
    * \param fValue2 second value at this keyframe (optional)
    */
   void addKeyframe (double fTime, KAnimatorFunction nFunction, long nId, float fValue, float fValue2 = 0.0f);

   /**
    * Start animating the values according to the list of keyframes
    *
    * \param nLoops number of loops to execute (K_ANIMATOR_INFINITE to loop indefinitely)
    * \param fHoldoff number of milliseconds to wait before executing the first loop
    */
   void startAnimator (long nLoops, double fHoldoff = 0.0);

   /**
    * Stop animating
    */
   void stopAnimator (void);

   /**
    * Update this value animator's logic
    *
    * \param fElapsed time elapsed since the last update, in milliseconds
    */
   void move (double fElapsed);

   /**
    * Get current first value for this animator
    *
    * \return first value
    */
   float getCurrentValue (void) const;

   /**
    * Get current second value for this animator
    *
    * \return second value
    */
   float getCurrentValue2 (void) const;

   /**
    * Get current holdoff delay
    *
    * \return delay, > 0 if counter is holding off, 0 if it is running or if it has completed
    */
   double getHoldOff (void) const;

   /**
    * Get time into the current animation loop
    *
    * \return current time in milliseconds
    */
   double getTime (void) const;

   /**
    * Get number of loops left in this animation
    *
    * \return number of loops, or K_ANIMATOR_INFINITE if infinite
    */
   long getLoops (void) const;

   /**
    * Check if a keyframe has been reached while playing the animation
    *
    * \param nId keyframe identifier
    *
    * \return true if keyframe has been reached, false if not
    */
   bool isKeyframeReached (long nId) const;

   /**
    * Reset the flag that notes whether a keyframe has been reached
    *
    * \param nId keyframe identifier
    */
   void clearKeyframeReached (long nId);

   /**
    * Check if this animator has looped at least once yet since the animation started
    *
    * \return true if at least one loop has been executed, false if not
    */
   bool hasLooped (void) const;

   /**
    * Check if this animator has reached its last keyframe yet
    *
    * \return true if last keyframe has been reached, false if not yet
    */
   bool isCompleted(void) const;

   /**
    * Get game-specific state
    *
    * \return game-specific state
    */
   long getUserState(void) const;

   /**
    * Set game-specific state
    *
    * \param nState new state
    */
   void setUserState(long nState);

private:
   /** Data for one keyframe */
   struct Keyframe {
      /** Time at this keyframe */
      double fTime;

      /** Function used for interpolating to this keyframe */
      KAnimatorFunction nFunction;

      /** Keyframe identifier */
      long nId;

      /** First value at this keyframe */
      float fValue;

      /** Second value at this keyframe */
      float fValue2;

      /** true if this keyframe has been reached */
      bool bReached;
   };

   /** Number of loops left, or K_ANIMATOR_INFINITE */
   long _nLoops;

   /** Milliseconds left before starting to animate*/
   double _fHoldoff;

   /** Current time while animating */
   double _fTime;

   /** First value interpolated for the current time */
   float _fCurValue;

   /** Second value interpolated for the current time */
   float _fCurValue2;

   /** true if looped at least once */
   bool _bLooped;

   /** true if finished animating */
   bool _bCompleted;

   /** Game-specific state */
   long _nState;

   /** Last keyframe reached while playing the animation */
   long _nLastKeyframe;

   /** Keyframes */
   std::vector<Keyframe> _keyframes;
};

#endif   /* _KANJI_KANIMATOR_H */
