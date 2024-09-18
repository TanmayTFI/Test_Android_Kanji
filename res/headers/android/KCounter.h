/**
 * Kanji game engine - value progressing from A to B over a period of time, definitions
 *
 * (c) 2010
 *
 * \file KCounter.h
 */

#ifndef  _KANJI_KCOUNTER_H
#define  _KANJI_KCOUNTER_H

/* Include definitions */
#include "KObjectBase.h"

/** Functions that can be used for interpolating from one value to the other */
typedef enum {
   K_COUNTER_LINEAR = 0,         /**< Linear interpolation */
   K_COUNTER_EASEIN,             /**< Start slowly and then accelerate to the target value */
   K_COUNTER_EASEOUT,            /**< Start like the linear interpolation and then decelerate to the target value */
   K_COUNTER_EASEINOUT,          /**< Start slowly, accelerate and then decelerate to the target value */
} KCounterFunction;

/**
 * Counter that goes from value A to B over a period of time
 */

class KCounter {
public:
   /** Constructor */
   KCounter ();

   /** Destructor */
   ~KCounter ();

   /**
    * Start counting from one value towards the other
    *
    * \param fFromValue value to start counting from
    * \param fToValue target value
    * \param fHoldoff delay in milliseconds before the count starts, 0 to start immediately
    * \param fDuration how many milliseconds it takes to go from one value to the other
    * \param nFunction function to be applied for progressing from one value to the other
    */
   void startCounter (float fFromValue, float fToValue, double fHoldoff, double fDuration, KCounterFunction nFunction);

   /**
    * Stop counter at its current value
    */
   void stopCounter (void);

   /**
    * Update this counter
    *
    * \param fElapsed number of milliseconds elapsed since last call to move()
    */
   void move (double fElapsed);

   /**
    * Get current value of this counter
    *
    * \return current value
    */
   float getCurrentValue (void) const;

   /**
    * Check if this counter has reached its target value yet
    *
    * \return true if target value has been reached, false if not yet
    */
   bool isCompleted (void) const;

   /**
    * Get starting value
    *
    * \return starting value
    */
   float getFromValue (void) const;

   /**
    * Get ending value
    *
    * \return ending value
    */
   float getToValue (void) const;

   /**
    * Get current holdoff delay
    *
    * \return delay, > 0 if counter is holding off, 0 if it is running or if it has completed
    */
   double getHoldOff (void) const;

   /**
    * Get how many milliseconds it takes to go from one value to the other
    *
    * \return programmed duration
    */
   double getDuration (void) const;

   /**
    * Get function applied to this counter
    *
    * \return function
    */
   KCounterFunction getFunction (void) const;

private:
   /** Value to count from */
   float _fFromValue;

   /** Value to count to */
   float _fToValue;

   /** Current value */
   float _fCurValue;

   /** Milliseconds left before starting to count towards the target value */
   double _fHoldoff;

   /** Total duration of going from one value to the other, in milliseconds */
   double _fDuration;

   /** Milliseconds elapsed so far when counting */
   double _fElapsed;

   /** Function used for interpolating */
   long _nFunction;

   /** true if counter has reached the target value yet */
   bool _bCompleted;
};

#endif   /* _KANJI_KCOUNTER_H */
