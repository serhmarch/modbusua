#include "CnDeviceItemList.h"

#include <device/items/CnDeviceBaseItem.h>

CnDeviceItemList::CnDeviceItemList()
{
}

CnDeviceItemList::~CnDeviceItemList()
{
    for (CnDeviceBaseItem *item : m_list)
        delete item;
    m_list.clear();
}

size_t CnDeviceItemList::count() const
{
    return m_list.size();
}

CnHash<CnString, CnDeviceBaseItem*> CnDeviceItemList::dict() const
{
    CnCriticalSectionLocker _(&m_cs);
    return m_dict;
}

bool CnDeviceItemList::hasItem(CnDeviceBaseItem *item) const
{
    CnCriticalSectionLocker _(&m_cs);
    auto it = m_hash.find(item);
    return (it != m_hash.end());
}

bool CnDeviceItemList::hasItem(const CnString &name) const
{
    CnCriticalSectionLocker _(&m_cs);
    auto it = m_dict.find(name);
    return (it != m_dict.end());
}

void CnDeviceItemList::addItems(const CnList<CnDeviceBaseItem *> &items)
{
    CnCriticalSectionLocker _(&m_cs);
    for (auto item : items)
    {
        m_list.push_back(item);
        m_hash.insert(item);
        m_dict.insert({item->name(), item});
        addItemInner(item);
    }
}

void CnDeviceItemList::removeItems(const CnList<CnDeviceBaseItem*> &items)
{
    CnCriticalSectionLocker _(&m_cs);
    for (auto item : items)
    {
        removeItemInner(item);
        m_list.remove(item);
        m_hash.erase(item);
        m_dict.erase(item->name());
        delete item;
    }
}

void CnDeviceItemList::clear()
{
    CnCriticalSectionLocker _(&m_cs);
    clearInner();
}

void CnDeviceItemList::addItemInner(CnDeviceBaseItem *item)
{
    // Note: Base implementation do nothing
}

void CnDeviceItemList::removeItemInner(CnDeviceBaseItem *item)
{
    // Note: Base implementation do nothing
}

void CnDeviceItemList::clearInner()
{
    CnCriticalSectionLocker _(&m_cs);
    for (auto it = m_list.begin(); it != m_list.end(); ++it)
    {
        CnDeviceBaseItem *item = *it;
        removeItemInner(item);
        delete item;
    }
    m_list.clear();
    m_hash.clear();
}
