#ifndef CNDEVICEITEMLIST_H
#define CNDEVICEITEMLIST_H

#include <CnGlobal.h>

class CnDeviceBaseItem;

class CnDeviceItemList
{
public:
    CnDeviceItemList();
    virtual ~CnDeviceItemList();

public:
    size_t count() const;
    bool hasItem(CnDeviceBaseItem *item) const;
    bool hasItem(const CnString &name) const;
    CnHash<CnString, CnDeviceBaseItem*> dict() const;
    void addItems(const CnList<CnDeviceBaseItem*> &items);
    void removeItems(const CnList<CnDeviceBaseItem*> &items);
    void clear();

protected:
    virtual void addItemInner(CnDeviceBaseItem *item);
    virtual void removeItemInner(CnDeviceBaseItem *item);
    virtual void clearInner();

protected:
    mutable CnCriticalSection m_cs;
    CnList<CnDeviceBaseItem*> m_list;
    CnSet<CnDeviceBaseItem*> m_hash;
    CnHash<CnString, CnDeviceBaseItem*> m_dict;
};

#endif // CNDEVICEITEMLIST_H