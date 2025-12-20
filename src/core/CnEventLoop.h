/*!
 * \file   CnEventLoop.h
 * \brief  The `CnEventLoop` class is designed to execute the CnEvent event queue.
 * 
 * \author serhmarch
 * \date   June 2025
 */
#ifndef CNEVENTLOOP_H
#define CNEVENTLOOP_H

#include <utility>
#include "CnCore.h"

class CnThread;
class CnObject;
class CnEvent;

/*! \brief The `CnEventLoop` class is intended to implement the event queue processing loop.

    \details The `CnEventLoop` class is used to implement the event queue processing loop.
    It is used in conjunction with the `CnThread` class to process events in a separate thread.

    The `CnEventLoop` class is used internally by the `CnThread` class and
    is not intended for direct use in application code.
*/
class CnEventLoop
{
public:
    /// \details Class constructor.
    CnEventLoop();

    /// \details Class destructor.
    ~CnEventLoop();

public:
    /// \details Function processes all events in the event queue.
    /// \return Returns `true` if at least one event was processed, `false` otherwise.
    bool processEvents();

private:
    CnThread *m_thread;
};

#endif // CNEVENTLOOP_H
