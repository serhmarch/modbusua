#include "CnEventLoop.h"

#include <CnObject.h>
#include <CnThread.h>

CnEventLoop::CnEventLoop()
{
    m_thread = CnThread::currentThread();
}

CnEventLoop::~CnEventLoop()
{
}

bool CnEventLoop::processEvents()
{
    CnList<std::pair<CnObject*,CnEvent*> > queue;
    m_thread->popQueue(queue);
    for (auto it = queue.begin(); it != queue.end(); ++it)
    {
        CnObject *object = it->first;
        CnEvent *e = it->second;
        object->processEvent(e);
        delete e;
    }
    return queue.size() > 0;
}

