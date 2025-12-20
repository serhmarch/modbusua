/*!
 * \file   CnThread.h
 * \brief  The abstract class `CnThread` is intended for creating and managing a software thread.
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNTHREAD_H
#define CNTHREAD_H

#include "CnTypes.h"

class CnApp;
class CnThreadData;
class CnEventLoop;
class CnObject;
class CnEvent;

/*! \brief The abstract class `CnThread` is designed for creating and managing a software thread.

    \details The `CnThread` object manages a single software thread within the program.\n
    The thread is started using the `start()` function and stopped using `stop()` or `waitForStop()`.
    The thread code is defined in derived classes by implementing the pure virtual function `run()`.
*/
class CnThread
{
public:
    /// \details Type for thread identifier.
    typedef void* Handle;

public:
    /// \details Static function returns the system identifier of the active (running) thread.
    static Handle currentThreadId();

    /// \details Static function returns a pointer to the active (running) thread.
    static CnThread* currentThread();

public:
    /// \details Constructor creates a software thread object. It can then be started using the `start()` function.
    CnThread();

    /// \details Destructor destroys the software thread object and releases associated operating system resources.
    /// The thread must be stopped before destruction.
    virtual ~CnThread();

public:
    /// \details Function returns the thread identifier.
    inline Handle id() const { return m_threadHandle; }

    /// \details Function returns `true` if the software thread is running, `false` otherwise.
    bool isRunning() const;

    /// \details Function starts a separate software thread and immediately returns control.
    virtual bool start();

    /// \details Function issues a command to stop the thread and immediately returns control.
    virtual void stop();

    /// \details Function issues a command to stop the thread and waits until the thread finishes, blocking control.
    virtual void waitForStop();

protected:
    /// \details Virtual function in which the thread code is implemented. Must be defined in the derived class.
    virtual int run();

protected:
    /// \details Function posts an event to the specified object.
    /// The event will be processed in the thread to which the object belongs.
    void postEvent(CnObject *object, CnEvent *event);

    /// \details Function retrieves and removes all events from the event queue and places them in the provided list `q`.
    /// \return Returns `true` if at least one event was retrieved, `false` otherwise.
    bool popQueue(CnList<std::pair<CnObject*,CnEvent*> > &q);

protected:
    /// \cond
    friend class CnApp;
    friend class CnThreadData;
    friend class CnEventLoop;

    Handle m_threadHandle;
    bool m_ctrlRun;

    CnCriticalSection m_csEventQueue;
    CnList<std::pair<CnObject*,CnEvent*> > m_eventQueue;
    /// \endcond
};

#endif // CNTHREAD_H
