/*!
 * \file   CnPortThread.h
 * \brief  Modbus connection thread for a single port
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNPORTTHREAD_H
#define CNPORTTHREAD_H

#include <CnThread.h>

class CnLoggerManager;
class CnPort;

/*! \brief Class `CnPortThread` runs one Modbus connection in a separate thread.

    \details The `CnPortThread` object manages a single Modbus connection
    in a separate application thread. The thread starts with `start()`
    and stops with `stop()` or `waitForStop()`.
*/

class CnPortThread : public CnThread
{
public:
    /// \details Constructs the Modbus-connection thread object.
    CnPortThread(CnPort *port);

    /// \details Destructs the Modbus-connection thread object.
    ~CnPortThread();

public:
    /// \details Returns the port object of this thread.
    CnPort *port() const { return m_port; }

public:
    /// \details Starts the Modbus connection thread and returns immediately.
    bool start() override;

    /// \details Requests thread stop and waits until it finishes, blocking the caller.
    void waitForStop() override;

protected:
    /// \details Implements the thread procedure for the Modbus connection.
    int run() override;

private:
    CnPort *m_port;
    CnLoggerManager *m_logger;
};

#endif // CNPORTTHREAD_H
