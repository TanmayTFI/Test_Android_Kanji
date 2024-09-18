/**
 * Kanji rendering and I/O engine - Android system threads definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSYSTHREAD_H
#define  _KANJI_KSYSTHREAD_H

/* Include definitions */
#include "KObjectBase.h"
#include <pthread.h>
#include <jni.h>

/** Thread priority */
enum {
   KSYSTHREAD_LOW = -1,       /**< Lower than main thread */
   KSYSTHREAD_NORMAL,         /**< Normal priority (same as main thread) */
   KSYSTHREAD_HIGH,           /**< Higher than main thread */
};

/* Forward declarations */
class KPTK;

/**
 * Class for creating and managing system threads of execution on Win32.
 */

class KSysThread : public KObjectBase {
   friend class KPTK;
   
public:
   /**
    * Constructor
    *
    * \param lpEntryPoint thread's entry point method
    * \param lpParam pointer parameter to pass to entry point method
    * \param nParam integer parameter to pass to entry point method
    * \param nPriority thread's priority (KSYSTHREAD_xxx)
    */
   KSysThread (void (*lpEntryPoint)(void *lpParam, long nParam), void *lpParam, long nParam, long nPriority = KSYSTHREAD_NORMAL);

   /** Destructor */
   ~KSysThread ();

   /**
    * Change this thread's priority
    *
    * \param nPriority new priority (KSYSTHREAD_xxx)
    */
   void setThreadPriority (long nPriority);

   /** Wait for this thread to terminate */
   void waitForCompletion (void);

   /**
    * Check if this thread has terminated
    *
    * \return true if has terminated, false if not
    */
   bool hasCompleted (void);

   /**
    * Suspend the calling thread for the specified duration
    *
    * \param nMillis number of milliseconds to suspend calling thread for
    */
   static void sleep (long nMillis);

private:
   /** posix thread's main method */
   static void *mainThreadFunc (void *lpInstance);

   /** Free all resources */
   static void cleanup (void);
   
   /** Peer class for JNI */
   jclass _jniPeerClass;
   
   /** Instance of GLThread for JNI */
   jobject _jniCaller;
   
   /** Posix thread handle */
   pthread_t _nThread;

   /** Supplied entry point */
   void (*_lpEntryPoint)(void *lpParam, long nParam);

   /** Supplied pointer parameter to entry point */
   void *_lpParam;

   /** Supplied integer parameter to entry point */
   long _nParam;

   /** true if thread has completed execution */
   bool _bCompleted;   
};

#endif   /* _KANJI_KSYSTHREAD_H */
