/*!
 * \file   CnEvent.h
 * \brief  The `CnEvent` class represents a generic event in the system.
 * 
 * \author serhmarch
 * \date   June 2025
 */
#ifndef CNEVENT_H
#define CNEVENT_H

#define CN_EVENT_TYPE_BASE 0

/*! \brief The `CnEvent` class represents a generic event in the system.

    \details The `CnEvent` class is the base class for all events in the system.
    It provides a common interface for all events, allowing them to be processed
    by the event loop and dispatched to the appropriate object.

    Each event has a type, which is an integer value that identifies the type of event.
    The type can be used to determine how to process the event.

    To create a custom event, you should inherit from the `CnEvent` class and
    implement the `type()` method to return a unique type value for your event.

    \sa CnEventLoop, CnObject
*/
class CnEvent
{
public:
    /// \brief Virtual destructor.
    virtual ~CnEvent() {}

public:
    /// \details Returns the type of the event.
    /// This method must be overridden in derived classes to return a unique type value for the event.
    virtual int type() const = 0;
};

#endif // CNEVENT_H