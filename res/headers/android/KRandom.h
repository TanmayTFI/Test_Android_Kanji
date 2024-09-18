/**
 * Kanji rendering and I/O engine - random number generator definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KRANDOM_H
#define  _KANJI_KRANDOM_H

/* Include definitions */
#include "KObjectBase.h"

/**
 * Random number generator
 */

class KRandom {
public:
   /**
    * Seed random number generator. By default, the RNG is seeded with the current clock
    *
    * \param nSeed new seed
    */
   static void seed (unsigned long nSeed);

   /**
    * Get random number between 0 and 2^32-1
    *
    * \return random number
    */
   static unsigned long getRandom (void);

   /**
    * Get random float between 0 and 1
    *
    * \return random float
    */
   static float getRandomFloat (void);

   /**
    * Get random double between 0 and 1
    *
    * \return random double
    */
   static double getRandomDouble (void);

private:
   /** true if the RNG has been seeded yet */
   static bool g_bSeeded;

   /** Singleton */
   static KRandom g_random;
};

#endif   /* _KANJI_KRANDOM_H */

