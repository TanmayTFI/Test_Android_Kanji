/**
 * Kanji rendering and I/O engine - Android system semaphores definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSYSSEMAPHORE_H
#define  _KANJI_KSYSSEMAPHORE_H

/* Include definitions */
#include "KObjectBase.h"
#include <semaphore.h>

/**
 * Class for modeling semaphores that provide thread synchronization.
 */

class KSysSemaphore : public KObjectBase {
public:
   /**
    * Constructor
    *
    * \param nCount initial count
    */
   KSysSemaphore (long nCount);
   
   /** Destructor */
   ~KSysSemaphore ();
   
   /**
    * Decrease semaphore count, and block if it is negative, until it is positive
    */
   void down (void);
   
   /**
    * Increase semaphore count, and wake up other threads that were blocked on it, if count is now positive
    */
   void up (void);
   
private:
   /** Posix semaphore */
   sem_t _sem;
};

#endif   /* _KANJI_KSYSSEMAPHORE_H */
