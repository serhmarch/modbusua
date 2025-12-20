#include "CnObject.h"

#include "CnGlobal.h"
#include "CnEvent.h"
#include "CnThread.h"

CnObject::CnObject() : ModbusObject()
{
    CN_REF_INIT
    m_thread = CnThread::currentThread();
}

CnObject::~CnObject()
{
    for (auto e : m_events.events)
        delete e;
}

bool CnObject::event(CnEvent * /*event*/)
{
    return false;
}

bool CnObject::eventFilter(CnObject * /*watched*/, CnEvent * /*event*/)
{
    return false;
}

void CnObject::installEventFilter(CnObject *filterObj)
{
    CnCriticalSectionLocker _(&m_filter.cs);
    m_filter.objects.push_front(filterObj);
}

void CnObject::removeEventFilter(CnObject *filterObj)
{
    CnCriticalSectionLocker _(&m_filter.cs);
    for (auto it = m_filter.objects.begin(); it != m_filter.objects.end(); ++it)
    {
        if (*it == filterObj)
        {
            m_filter.objects.erase(it);
            return;
        }
    }
}

void CnObject::processEvent(CnEvent *e)
{
    bool allowed = true;
    m_filter.cs.lock();
    for (auto filterObj : m_filter.objects)
    {
        if (filterObj->eventFilter(this, e))
        {
            allowed = false;
            break;
        }
    }
    m_filter.cs.unlock();
    if (allowed)
        this->event(e);
}
