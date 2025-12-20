#include "CnThread.h"

#include <CnUtils.h>
#include <CnEventLoop.h>

CnThread::CnThread()
{
    m_threadHandle = 0;
    m_ctrlRun = false;
}

void CnThread::postEvent(CnObject *object, CnEvent *event)
{
    CnCriticalSectionLocker _(&m_csEventQueue);
    m_eventQueue.push_back(std::make_pair(object, event));
}

bool CnThread::popQueue(CnList<std::pair<CnObject*,CnEvent*> > &q)
{
    m_csEventQueue.lock();
    m_eventQueue.swap(q);
    m_csEventQueue.unlock();
    return q.size() > 0;
}

int CnThread::run()
{
    CnEventLoop loop;
    m_ctrlRun = true;
    while (m_ctrlRun)
    {
        loop.processEvents();
        Cn::msleep(1);
    }
    return 0;
}