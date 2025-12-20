#include "CnDeviceMessageList.h"

#include <algorithm>

#include "CnDeviceMessage.h"
#include <device/items/CnDeviceModbusItem.h>

// ------------------------------------------------------------------------------------------
// --------------------------------- CnDeviceMessageList ---------------------------------
// ------------------------------------------------------------------------------------------

CnDeviceMessageList::CnDeviceMessageList(CnDevice *device) :
    m_device(device),
    m_current(nullptr)
{
    // Note: m_device is exists for create messages only.
    //       Do not call it methods to prevent interlock
}

CnDeviceMessageList::~CnDeviceMessageList()
{
    for (auto it = m_list.begin(); it != m_list.end(); ++it)
    {
        CnDeviceMessageBase *m = *it;
        if (m == m_current)
            m_current = nullptr;
        delete m;
    }
    if (m_current)
        delete m_current;
}

void CnDeviceMessageList::addItem(CnDeviceModbusItem *item)
{
    CnCriticalSectionLocker _(&m_cs);
    if (item->message()) // Note: Added 16 Jan 2025 to prevent item belonging
        return;          //       to mulptiple messages simultaniously.
    for (CnDeviceMessageBase *m : m_list)
    {
        if (m->addItem(item))
            return;
    }
    CnDeviceMessageBase *m = this->createMessage(item);
    addMessageInner(m);
}

void CnDeviceMessageList::addMessage(CnDeviceMessageBase *msg)
{
    CnCriticalSectionLocker _(&m_cs);
    addMessageInner(msg); 
}

void CnDeviceMessageList::clear()
{
    CnCriticalSectionLocker _(&m_cs);
    clearInner();
}

void CnDeviceMessageList::addMessageInner(CnDeviceMessageBase *msg)
{
    m_list.push_back(msg);
    msg->initialize();
}

void CnDeviceMessageList::removeMessageInner(CnDeviceMessageBase *msg)
{
    msg->finalize();
}

// ------------------------------------------------------------------------------------------
// ------------------------------- CnDevicePokeMessageList -------------------------------
// ------------------------------------------------------------------------------------------

CnDevicePokeMessageList::CnDevicePokeMessageList(CnDevice *device) : CnDeviceMessageList(device)
{
}

CnDevicePokeMessageList::~CnDevicePokeMessageList()
{
}

bool CnDevicePokeMessageList::popMessage(CnDeviceMessageBase **m)
{
    CnCriticalSectionLocker _(&m_cs); 
    if (m_list.size())
    {
        CnDeviceMessageBase *msg = m_list.front();
        m_list.pop_front();
        *m = msg;
        if (m_current)
            removeMessageInner(m_current);
        m_current = msg;
        return true;
    }
    return false;
}

void CnDevicePokeMessageList::endMessage(CnDeviceMessageBase *msg)
{
    CnCriticalSectionLocker _(&m_cs); 
    if (!msg)
        return; // Gracefully ignore null message
    if ((m_current != msg) && m_current)
    {
        auto it = std::find(m_list.begin(), m_list.end(), m_current);
        if (it == m_list.end()) // Note: if message is absent in the list means it was not deleted previously
        {
            removeMessageInner(m_current);
            delete m_current;
        }
    }
    removeMessageInner(msg);
    delete msg;
    m_current = nullptr;
}

void CnDevicePokeMessageList::clearInner()
{
    for (CnDeviceMessageBase *m : m_list)
    {
        removeMessageInner(m);
        delete m;
    }
    m_list.clear();
}

CnDeviceMessageBase *CnDevicePokeMessageList::createMessage(CnDeviceModbusItem *item)
{
    return Cn::createDeviceModbusPokeMessage(m_device, item);;
}

// ------------------------------------------------------------------------------------------
// ------------------------------- CnDevicePollMessageList -------------------------------
// ------------------------------------------------------------------------------------------

CnDevicePollMessageList::CnDevicePollMessageList(CnDevice *device) : CnDeviceMessageList(device)
{
}

CnDevicePollMessageList::~CnDevicePollMessageList()
{
}

bool CnDevicePollMessageList::popMessage(CnDeviceMessageBase **msg)
{
    CnCriticalSectionLocker _(&m_cs); 
    if (CnDeviceMessageBase *m = findMessage())
    {
        *msg = m;
        return true;
    }
    return false;
}

void CnDevicePollMessageList::endMessage(CnDeviceMessageBase *msg)
{
    CnCriticalSectionLocker _(&m_cs); 
    if (!msg)
        return; // Gracefully ignore null message
    if (m_current == msg)
    {
        if (msg->itemCount() == 0) // if no items in the message need to delete message
        { 
            // Note: Try to prevent double deletion of the message
            auto it = std::find(m_list.begin(), m_list.end(), msg);
            if (it != m_list.end())
            {
                removeMessageInner(msg);
                m_list.erase(it);
            }
        }
    }
    else
        delete m_current;
    m_current = nullptr;
}

void CnDevicePollMessageList::clearInner()
{
    for (auto it = m_list.begin(); it != m_list.end(); ++it)
    {
        CnDeviceMessageBase *m = *it;
        removeMessageInner(m);
        if (m == m_current)
        {
            // Note: current message is still in process
            //       So clear its item list only.
            m->clearItems();
        }
        else
        {
            delete m;
        }
    }
    m_list.clear();
}

CnDeviceMessageBase *CnDevicePollMessageList::createMessage(CnDeviceModbusItem *item)
{
    return Cn::createDeviceModbusPollMessage(m_device, item);;
}

CnDeviceMessageBase *CnDevicePollMessageList::findMessage()
{
    CnTimestamp tm = CnTimestamp::current();
    for (auto it = m_list.begin(); it != m_list.end(); ++it)
    {
        CnDeviceMessageBase *m = *it;
        CnTimestamp tmdiff = tm - m->timestampBegin();
        if (tmdiff.milliseconds() >= m->period())
        {
            //CnString sb = CnDateTime(tm).toString();
            //CnString se = CnDateTime(m->timestampBegin()).toString();
            //logMessage(CnSTR("Current time: %s. Begin time: %s."), sb.data(), se.data());
            m_list.erase(it);    // remove it from the queue ...
            m_list.push_back(m); // and push it to the back of the queue
            if (m_current)
            {
                auto cit = std::find(m_list.begin(), m_list.end(), m_current);
                if (cit == m_list.end()) // Note: if message is absent in the list means it was not deleted previously
                {
                    removeMessageInner(m_current);   
                    delete m_current;
                }
            }
            m_current = m;
            return m;
        }
    }
    return nullptr;
}
