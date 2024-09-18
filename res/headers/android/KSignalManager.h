/**
 * Kanji game engine - signal manager definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSIGNALMANAGER_H
#define  _KANJI_KSIGNALMANAGER_H

/* Include definitions */
#include "KObjectListable.h"
#include "KHashTable.h"
#include "KList.h"
#include "KDictionary.h"
#include "KSysLock.h"

/* Forward definitions */
class KSignal;
class KSignalManager;
class KSignalManagerList;

/**
 * Signal listener
 */

class KSignalListener {
   friend class KSignalManager;
   friend class KSignalManagerList;

public:
   /** Constructor */
   KSignalListener ();

   /** Destructor */
   virtual ~KSignalListener ();

protected:
   /**
    * Receive dispatched signal
    *
    * \param lpszSignalName name of signal being dispatched
    * \param lpSignalData dictionary of data for signal being dispatched
    */
   virtual void receiveSignal (const char *lpszSignalName, KDictionary *lpSignalData) = 0;

private:
   /** Next listener in list */
   KSignalListener *_lpNextListener;

   /** Previous listener in list */
   KSignalListener *_lpPrevListener;
};

/**
 * Signal listener class for callic static listeners
 *
 * \private
 */

class KSignalStaticListener : public KObjectListable, public KSignalListener {
   friend class KSignalManager;

protected:
   /**
    * Constructor
    *
    * \param lpszSignalName name of signal that this listener is registered on
    * \param lpStaticReceiveSignal static function to call when a signal is received
    * \param lpStaticReceiverData data to pass to function
    */
   KSignalStaticListener (const char *lpszSignalName, void (*lpStaticReceiveSignal) (const char *lpszSignalName, KDictionary *lpSignalData, void *lpReceiverData), void *lpStaticReceiverData);

   /** Destructor */
   virtual ~KSignalStaticListener ();

   /**
    * Receive dispatched signal
    *
    * \param lpszSignalName name of signal being dispatched
    * \param lpSignalData dictionary of data for signal being dispatched
    */
   virtual void receiveSignal (const char *lpszSignalName, KDictionary *lpSignalData);

private:
   /** Name of signal that this listener is registered on */
   char *_lpszSignalName;

   /** Static function to call when a signal is received */
   void (*_lpStaticReceiveSignal) (const char *lpszSignalName, KDictionary *lpSignalData, void *lpReceiverData);

   /** Data to pass to function */
   void *_lpStaticReceiverData;
};

/**
 * Entry into a list of listeners on a signal
 *
 * \private
 */

class KSignalListenerNode : public KObjectListable {
   friend class KSignal;
   friend class KSignalManager;

private:
   /** Constructor */
   KSignalListenerNode (KSignalListener *lpListener);

   /** Destructor */
   virtual ~KSignalListenerNode ();

   /** Listener */
   KSignalListener *_lpListener;
};

/**
 * One signal to dispatch in a queue
 *
 * \private
 */

class KSignalQueueEntry : public KObjectListable {
public:
   /**
    * Constructor
    *
    * \param lpszSignalName name of signal to dispatch
    * \param lpSignalData dictionary of data for signal being dispatched
    * \param fDelay milliseconds to wait before dispatching this signal
    * \param fRepeatEvery number of milliseconds after which to repeat dispatching this signal (0 to dispatch only once)
    */
   KSignalQueueEntry (const char *lpszSignalName, KDictionary *lpDictionary, double fDelay, double fRepeatEvery);
   
   /** Destructor */
   virtual ~KSignalQueueEntry ();
   
   /** Name of signal to dispatch */
   char *_szSignalName;
   
   /** Signal parameters */
   KDictionary *_lpDictionary;
   
   /** Milliseconds left before dispatching this signal */
   double _fDelay;
   
   /** Number of milliseconds after which to repeat dispatching this signal (0 to dispatch only once) */
   double _fRepeatEvery;
};

/**
 * Queue of signals to dispatch at a later time
 */

class KSignalQueue : public KObjectBase {
public:
   /** Constructor */
   KSignalQueue ();
   
   /** Destructor */
   virtual ~KSignalQueue ();
   
   /** Clear all pending signals in this queue */
   void clear (void);
   
   /**
    * Enqueue signal to be dispatched when the queue is processed
    *
    * \param lpszSignalName name of signal to dispatch
    * \param lpSignalData dictionary of data for signal being dispatched
    * \param fDelay milliseconds to wait before dispatching this signal (0 to dispatch as soon as the queue is processed)
    * \param fRepeatEvery number of milliseconds after which to repeat dispatching this signal (0 to dispatch only once)
    */
   void enqueueSignal (const char *lpszSignalName, KDictionary *lpSignalData, double fDelay = 0.0, double fRepeatEvery = 0.0);
   
   /**
    * Enqueue signal to be dispatched when the queue is processed; if the same signal name is already in the queue,
    * don't enqueue it again
    *
    * \param lpszSignalName name of signal to dispatch
    * \param lpSignalData dictionary of data for signal being dispatched
    * \param fDelay milliseconds to wait before dispatching this signal (0 to dispatch as soon as the queue is processed)
    * \param fRepeatEvery number of milliseconds after which to repeat dispatching this signal (0 to dispatch only once)
    */
   void enqueueSignalOnce (const char *lpszSignalName, KDictionary *lpSignalData, double fDelay = 0.0, double fRepeatEvery = 0.0);
   
   /**
    * Check if the specified signal is currently pending
    *
    * \param lpszSignalName name of signal to check
    *
    * \return true if currently enqueued, false if not
    */
   bool isSignalEnqueued (const char *lpszSignalName);

   /**
    * Cancel a pending or repeating signal
    *
    * \param lpSignalName name of signal to cancel
    */
   void cancelSignal (const char *lpszSignalName);
   
   /**
    * Dispatch all pending signals in the queue
    *
    * \param fElapsed milliseconds elapsed since last call to processQueue()
    */
   void processQueue (double fElapsed);
   
private:
   /** Lock for accessing the signals list */
   KSysLock *_lpLock;
   
   /** List of signals to dispatch */
   KList<KSignalQueueEntry> _lEntries;
};

/**
 * Signal
 *
 * \private
 */

class KSignal : public KObjectHashable {	
public:
   /** Name of this signal */
	char *_szSignalName;

   /** List of listeners on this signal */
	KList<KSignalListenerNode> _lListeners;
};

/**
 * Signal manager
 */

class KSignalManager {
public:
   /**
    * Dispatch a signal
    *
    * \param lpszSignalName name of signal to dispatch
    * \param lpSignalData dictionary of data for signal being dispatched
    */
	static void dispatchSignal (const char *lpszSignalName, KDictionary *lpSignalData);

   /**
    * Register a listener to a specified signal
    *
    * \param lpszSignalName name of signal to start listening on
    * \param lpListener listener to register for this signal; its receiveSignal() method will be called when a signal is dispatched
    */
	static void registerListener (const char *lpszSignalName, KSignalListener *lpListener);

   /**
    * Unregister a listener from a specified signal
    *
    * \param lpszSignalName name of signal to stop listening on
    * \param lpFunction listener to unregister
    */
	static void unregisterListener (const char *lpszSignalName, KSignalListener *lpListener);

   /**
    * Unregister a listener from all signals
    *
    * \param lpFunction listener to unregister
    */
	static void unregisterListener (KSignalListener *lpListener);

   /**
    * Register a static method to listen to a signal
    *
    * \param lpszSignalName name of signal to start listening on
    * \param receiveSignal static method called when a signal is dispatched
    * \param lpReceiverData data passed back to static method when a signal is dispatched
    */
   static void registerStaticListener (const char *lpszSignalName, void (*receiveSignal) (const char *lpszSignalName, KDictionary *lpSignalData, void *lpReceiverData), void *lpReceiverData = NULL);

   /**
    * Unregister a static method from listening to a signal
    *
    * \param lpszSignalName name of signal to stop listening on
    * \param receiveSignal static method to remove
    */
   static void unregisterStaticListener (const char *lpszSignalName, void (*receiveSignal) (const char *lpszSignalName, KDictionary *lpSignalData, void *lpReceiverData));

   /**
    * Unregister a static method from all signals it is listening on
    *
    * \param receiveSignal static method to remove
    */
   static void unregisterStaticListener (void (*receiveSignal) (const char *lpszSignalName, KDictionary *lpSignalData, void *lpReceiverData));

   /** Free all resources used by signals */
   static void cleanup (void);

private:
   /** List of all signals that have listeners */
	static KList<KSignal> g_lSignals;

   /** Hashtable of signal names */
	static KHashTable g_hashSignals;

   /** List of listeners created for static functions */
	static KList<KSignalStaticListener> g_lStaticListeners;
};

#endif   /* _KANJI_KSIGNALMANAGER_H */

