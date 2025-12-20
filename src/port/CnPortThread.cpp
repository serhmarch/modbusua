#include "CnPortThread.h"

#include <CnEventLoop.h>

#include "CnPort.h"
#include "CnPortRunnable.h"
#include <device/CnDevice.h>

#include <Modbus.h>

using namespace Modbus;

CnPortThread::CnPortThread(CnPort *port) :
    CnThread()
{
    port->moveToThread(this);
    for (CnDevice *d : port->devices())
        d->moveToThread(this);
    m_port = port;
    m_logger = m_port->logger();
    m_port->IncrementRef(); 
    m_logger->logCtorDtor(CnSTR("CnPortThread::CnPortThread()"));
}

CnPortThread::~CnPortThread()
{
    m_logger->logCtorDtor(CnSTR("CnPortThread::~CnPortThread()"));
    m_port->DecrementRef(); 
}

bool CnPortThread::start()
{
    m_logger->logThreadStartStop(CnSTR("CnPortThread::start()"));
    return CnThread::start();
}

void CnPortThread::waitForStop()
{
    m_logger->logThreadStartStop(CnSTR("CnPortThread::waitForStop()"));
    CnThread::waitForStop();
    m_logger->logThreadStartStop(CnSTR("CnPortThread: stopped"));
}

int CnPortThread::run()
{
    CnEventLoop loop;
    m_logger->logThreadStartStop(CnSTR("CnPortThread::run(): begin"));
    m_ctrlRun = true;
    {
        CnPortRunnable port(m_port);
        while (m_ctrlRun)
        {
            loop.processEvents();
            port.run();
            Cn::msleep(1); // Note: take a rest for current thread
        }
    }
    m_logger->logThreadStartStop(CnSTR("CnPortThread::run(): end"));
    return 0;
}


