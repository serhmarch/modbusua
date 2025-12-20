/*!\
 * \file   CnObject.h\
 * \brief  Contains the base class CnObject\
 *\
 * \author serhmarch\
 * \date   December 2024\
 */
#ifndef CNOBJECT_H
#define CNOBJECT_H

#include <ModbusObject.h>

#include "CnCore.h"
#include "CnEvent.h"
#include "CnSharedPointer.h"
#include "CnTypes.h"

class CnApp;
class CnThread;

/*! \brief Describes the base class CnObject.

    \details This class should be used as a base class for objects
    that require reference counting functionality,
    as well as functionality for using the Signal/Slot mechanism -
    generating signals and connecting functions/methods of classes to these signals,
    which will handle these signals during program execution.
 */

class CnObject : public ModbusObject
{
    CN_REF_COUNTING

public:
    /// \details Class constructor.
    CnObject();

    /// \details Class destructor.
    ~CnObject();

public:
    /// \details Returns a pointer to the thread in which the object lives.
    inline CnThread* thread() const { return m_thread; }

    /// \details Moves the object to the specified thread.
    inline void moveToThread(CnThread* thread) { m_thread = thread; }

public: // event interface
    /// \details This virtual function receives events to an object and
    /// should return true if the event e was recognized and processed.
    /// The event() function can be reimplemented to customize the behavior of an object.
    virtual bool event(CnEvent *event);

    /// \details Filters events if this object has been installed
    /// as an event filter for the watched object.
    /// In your reimplementation of this function, if you want to filter the event out,
    /// i.e. stop it being handled further, return true; otherwise return false.
    virtual bool eventFilter(CnObject *watched, CnEvent *event);

    /// \details Installs an event filter filterObj on this object. 
    /// For example: `monitoredObj->installEventFilter(filterObj);`
    /// 
    /// An event filter is an object that receives all events that are sent to this object.
    /// The filter can either stop the event or forward it to this object.
    /// The event filter filterObj receives events via its eventFilter() function.
    /// The eventFilter() function must return true if the event should be filtered,
    /// (i.e. stopped); otherwise it must return false.
    /// If multiple event filters are installed on a single object,
    /// the filter that was installed last is activated first.
    void installEventFilter(CnObject *filterObj);

    /// \details Removes the event filter filterObj from this object.
    /// An event filter must be removed from an object before it is deleted.
    /// removeEventFilter() does nothing if filterObj is not installed as an event filter for this object.
    /// removeEventFilter() must not be called from within the eventFilter() function of filterObj itself
    /// because this can lead to deadlocks.
    void removeEventFilter(CnObject *filterObj);

protected:
    friend class CnEventLoop;
    void processEvent(CnEvent *event);

protected:
    CnThread* m_thread;
    struct
    {
        mutable CnCriticalSection cs;
        CnList<CnEvent*> events;
    } m_events;

    struct
    {
        mutable CnCriticalSection cs;
        CnList<CnObject*> objects;
    } m_filter;
};

#endif // CNOBJECT_H