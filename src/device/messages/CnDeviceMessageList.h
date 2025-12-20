#ifndef CNDEVICEMESSAGELIST_H
#define CNDEVICEMESSAGELIST_H

#include <CnGlobal.h>

class CnDevice;
class CnDeviceModbusItem;
class CnDeviceMessageBase;

// ------------------------------------------------------------------------------------------
// --------------------------------- CnDeviceMessageList ---------------------------------
// ------------------------------------------------------------------------------------------

class CnDeviceMessageList
{
public:
    CnDeviceMessageList(CnDevice *device);
    virtual ~CnDeviceMessageList();

public:
    inline CnDeviceMessageBase* current() const { CnCriticalSectionLocker _(&m_cs); return m_current; }
    inline bool hasMessage() const  { CnCriticalSectionLocker _(&m_cs); return m_list.size(); }
    virtual void addItem(CnDeviceModbusItem *item);
    void addMessage(CnDeviceMessageBase *msg);
    void clear();
    virtual bool popMessage(CnDeviceMessageBase** msg) = 0;
    virtual void endMessage(CnDeviceMessageBase* msg) = 0;

protected:
    virtual void addMessageInner(CnDeviceMessageBase *msg);
    virtual void removeMessageInner(CnDeviceMessageBase *msg);
    virtual void clearInner() = 0;
    virtual CnDeviceMessageBase *createMessage(CnDeviceModbusItem *item) = 0;

protected:
    mutable CnCriticalSection m_cs;
    CnDevice *m_device;
    CnDeviceMessageBase* m_current;
    CnList<CnDeviceMessageBase*> m_list;
};

// ------------------------------------------------------------------------------------------
// ------------------------------- CnDevicePokeMessageList -------------------------------
// ------------------------------------------------------------------------------------------

class CnDevicePokeMessageList : public CnDeviceMessageList
{
public:
    CnDevicePokeMessageList(CnDevice *device);
    ~CnDevicePokeMessageList() override;

public:
    bool popMessage(CnDeviceMessageBase** msg) override;
    void endMessage(CnDeviceMessageBase* msg) override;

protected:
    void clearInner() override;
    CnDeviceMessageBase *createMessage(CnDeviceModbusItem *item) override;
};

// ------------------------------------------------------------------------------------------
// ------------------------------- CnDevicePollMessageList -------------------------------
// ------------------------------------------------------------------------------------------

class CnDevicePollMessageList : public CnDeviceMessageList
{
public:
    CnDevicePollMessageList(CnDevice *device);
    ~CnDevicePollMessageList() override;

public:
    bool popMessage(CnDeviceMessageBase** msg) override;
    void endMessage(CnDeviceMessageBase* msg) override;

protected:
    void clearInner() override;
    CnDeviceMessageBase *createMessage(CnDeviceModbusItem *item) override;

protected:
    CnDeviceMessageBase *findMessage();
};

#endif // CNDEVICEMESSAGELIST_H