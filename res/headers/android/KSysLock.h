/**
 * Kanji rendering and I/O engine - Android system locks definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSYSLOCK_H
#define  _KANJI_KSYSLOCK_H

/* Include definitions */
#include "KObjectBase.h"
#include <pthread.h>

/* Forward declarations */
class KPTK;

/**
 * Class for modeling locks that handle mutual exclusion of threads to access a shared resource.
 */

class KSysLock : public KObjectBase {
   friend class KPTK;
   
public:
   /** Constructor */
   KSysLock ();

   /** Destructor */
   ~KSysLock ();

   /** Acquire. When the method exits, access to the shared resource is allowed */
   void acquire (void);

   /** Release. The resource must not be accessed anymore when this method is called */
   void release (void);

private:
   /** Free all resources */
   static void cleanup (void);
   
   /** Posix mutex */
   pthread_mutex_t _nMutex;
};

#endif   /* _KANJI_KSYSLOCK_H */
