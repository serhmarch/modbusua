#include "CnDeviceMessage.h"

#include <CnStd_stdio.h>

#include <device/CnDevice.h>

// TODO: check thread synch while add new item to existing message

// ------------------------------------------------------------------------------------------
// --------------------------------- CnDeviceMessage ---------------------------------
// ------------------------------------------------------------------------------------------
CnDeviceMessage::CnDeviceMessage(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount)
{
    m_device = device;
    m_offset = item->offset();
    m_count = item->count();
    m_period = item->period();

    m_execOffset = 0;
    m_execCount = 0;

    m_maxCount = maxCount;
    if (m_count > m_maxCount)
        m_count = m_maxCount;
    m_status = Cn::Status_Uncertain;
    m_timestamp = CnDateTime::currentTimestamp();
    m_messageId = item->messageId();

    m_stat.GoodCount = 0;
    m_stat.BadCount = 0;
    m_stat.Status = Modbus::Status_Uncertain;
    m_stat.Timestamp = 0;
    m_stat.LastSuccessTimestamp = 0;
    m_stat.LastErrorStatus = Modbus::Status_Uncertain;
    m_stat.LastErrorTimestamp = 0;

    addItemInner(item);
}

CnDeviceMessage::~CnDeviceMessage()
{
    // TODO: DecrementRef for all items
}

const CnChar* CnDeviceMessage::fineName() const
{
    CnCriticalSectionLocker _(&m_cs); 
    int t = memoryType() % 10;
    CnStd::snprintf(m_fineName, sizeof(m_fineName), CnSTR("%i%05hu:%i%05hu"), t, offset() + 1, t, offset() + count());
    return m_fineName;
}

void CnDeviceMessage::beginProcess()
{
    CnCriticalSectionLocker _(&m_cs); 
    m_timestampBegin = CnDateTime::currentTimestamp();
    m_execOffset = m_offset;
    m_execCount = m_count;
}

void CnDeviceMessage::setComplete(Modbus::StatusCode status, CnTimestamp timestamp, const CnString &err)
{
    setComplete(Cn::ModbusToCnStatusCode(status), timestamp);
    setStatistics(status, timestamp, err);
}

void CnDeviceMessage::setComplete(Cn::StatusCode status, CnTimestamp timestamp)
{
    CnCriticalSectionLocker _(&m_cs); 
    m_status = status;
    m_timestamp = timestamp;
}

void CnDeviceMessage::setStatistics(Modbus::StatusCode status, CnTimestamp timestamp, const CnString &err)
{
    CnCriticalSectionLocker _(&m_stat.cs);
    m_stat.Status = status;
    m_stat.Timestamp = timestamp;
    if (Modbus::StatusIsGood(status))
    {
        m_stat.GoodCount++;
        m_stat.LastSuccessTimestamp = timestamp;
    }
    else
    {
        m_stat.BadCount++;
        m_stat.LastErrorStatus = status;
        m_stat.LastErrorTimestamp = timestamp;
        m_stat.LastErrorText = err;
    }
}

void CnDeviceMessage::addItemInner(CnDeviceModbusItem *item)
{
    m_items.push_back(item);
    item->setMessage(this);
}

void CnDeviceMessage::removeItemInner(CnDeviceModbusItem *item)
{
    signalRemovingItem(item);
    m_items.remove(item);
    item->setMessage(nullptr);
}

void CnDeviceMessage::writeToItems()
{
    if (Cn::StatusIsGood(m_status))
    {
        for (Items_t::iterator it = m_items.begin(); it != m_items.end(); it++)
        {
            CnDeviceModbusItem* mi = static_cast<CnDeviceModbusItem*>(*it);
            Cn::StatusCode status = m_status;
            uint16_t innerOffset = mi->offset() - m_execOffset;
            uint16_t count = mi->count();
            mi->lock();
            if (getDataInner(innerOffset, count, mi->data()))
            {
                mi->processDataUnsafe();
                mi->updateStatusTimestampUnsafe(status, m_timestamp);
            }
            mi->unlock();
        }
    }
    else
    {
        for (Items_t::iterator it = m_items.begin(); it != m_items.end(); it++)
        {
            CnDeviceModbusItem* mi = static_cast<CnDeviceModbusItem*>(*it);
            mi->updateStatusTimestamp(m_status, m_timestamp);
        }
    }
}

void CnDeviceMessage::readFromItems()
{
    for (Items_t::iterator it = m_items.begin(); it != m_items.end(); it++)
    {
        CnDeviceModbusItem* mi = static_cast<CnDeviceModbusItem*>(*it);
        uint16_t innerOffset = mi->offset() - m_execOffset;
        uint16_t count = mi->count();
        mi->lock();
        setDataInner(innerOffset, count, mi->data());
        mi->unlock();
    }
}

// ------------------------------------------------------------------------------------------
// ------------------------------ CnDevicePollMessage ------------------------------
// ------------------------------------------------------------------------------------------
CnDevicePollMessage::CnDevicePollMessage(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount) :
    CnDeviceMessage(device, item, maxCount)
{
}

CnDevicePollMessage::~CnDevicePollMessage()
{
}

void CnDevicePollMessage::initialize()
{
}    // Note: Base implementation do nothing

void CnDevicePollMessage::finalize()
{
    CnCriticalSectionLocker _(&m_cs);
    // Note: No need to delete items because it is a responsibility of CnDevice class
    for (auto item : m_items)
        item->setMessage(nullptr);
    m_items.clear();
}

bool CnDevicePollMessage::addItem(CnDeviceModbusItem * mi)
{
    CnCriticalSectionLocker _(&m_cs); 
    if (mi->memoryType() != memoryType())
    {
        m_device->logger()->logMessage(CnSTR("ModbusMessage(%s)::AddItem(name=%s): Unsuccessful (memoryType don't match)"), fineName(), mi->name().data());
        return false;
    }
    if (mi->period() != m_period)
    {
        m_device->logger()->logMessage(CnSTR("ModbusMessage(%s)::AddItem(name=%s): Unsuccessful (period don't match)"), fineName(), mi->name().data());
        return false;
    }
    if (mi->messageId() != m_messageId)
    {
        m_device->logger()->logMessage(CnSTR("ModbusMessage(%s)::AddItem(name=%s): Unsuccessful (messageId don't match)"), fineName(), mi->name().data());
        return false;
    }
    uint16_t itemOffset, itemCount, nextItemOffset;
    itemOffset = mi->offset();
    itemCount = mi->count();
    nextItemOffset = itemOffset + itemCount;
    if ((itemOffset >= m_offset) && (nextItemOffset <= (m_offset+m_maxCount))) // expand message to end
    {
        if (nextItemOffset >= (m_offset+m_count))
            m_count = nextItemOffset - m_offset;
    }
    else if ((itemOffset < m_offset) && ((itemOffset+m_maxCount) >= (m_offset+m_count))) // expand message from begin
    {
        m_count += (m_offset - itemOffset);
        m_offset = itemOffset;
    }
    else
    {
        if (messageId().empty())
        {
            m_device->logger()->logMessage(CnSTR("ModbusMessage(%s)::AddItem(name=%s): Unsuccessful. ModbusItem is out of message data range "), fineName(), mi->name().data());
            return false;
        }
        m_device->logger()->logWarning(CnSTR("ModbusMessage(%s)::AddItem(name=%s, msgid=%s): ModbusItem is out of message data range so it will be ignored"), fineName(), mi->name().data(), messageId().data());
    }
    addItemInner(mi);
    return true;
}

void CnDevicePollMessage::setComplete(Cn::StatusCode status, CnTimestamp timestamp)
{
    CnTimestamp requestTimeout;
    requestTimeout.addMilliseconds(m_device->CfgRequestTimeout());

    CnDeviceMessage::setComplete(status, timestamp);
    CnCriticalSectionLocker _(&m_cs);
    writeToItems();
    for (Items_t::iterator it = m_items.begin(); it != m_items.end(); )
    {
        CnDeviceModbusItem* mi = static_cast<CnDeviceModbusItem*>(*it);
        if (timestamp - mi->timestampLastRequest() >= requestTimeout.data())
        {
            signalRemovingItem(mi);
            mi->setMessage(nullptr);
            it = m_items.erase(it);
        }
        else
            it++;
    }
}

void CnDevicePollMessage::clearItems()
{
    CnCriticalSectionLocker _(&m_cs); 
    for (auto item : m_items)
    {
        signalRemovingItem(item);
        item->setMessage(nullptr);
    }
    m_items.clear();
}

// ------------------------------------------------------------------------------------------
// ------------------------------- CnDevicePokeMessage -------------------------------
// ------------------------------------------------------------------------------------------
CnDevicePokeMessage::CnDevicePokeMessage(CnDevice *device, CnDeviceModbusItem *item, uint16_t maxCount) :
    CnDeviceMessage(device, item, maxCount)
{
}

CnDevicePokeMessage::~CnDevicePokeMessage()
{
}

void CnDevicePokeMessage::initialize()
{
}    // Note: Base implementation do nothing

void CnDevicePokeMessage::finalize()
{
    CnCriticalSectionLocker _(&m_cs);
    for (auto item : m_items)
        delete item;
    m_items.clear();
}

bool CnDevicePokeMessage::addItem(CnDeviceModbusItem *mi)
{
    CnCriticalSectionLocker _(&m_cs); 
    if (mi->memoryType() != this->memoryType())
    {
        m_device->logger()->logMessage(CnSTR("ModbusPokeMessage(%s)::AddItem(name=%s): Unsuccessful (memoryType don't match)"), fineName(), mi->name().data());
        return false;
    }
    const uint16_t itemOffset = mi->offset();
    const uint16_t nextItemOffset = itemOffset + mi->count();
    const uint16_t messageItemOffset = this->offset();
    const uint16_t messageNextItemOffset = messageItemOffset + this->count();
    uint16_t offset = this->offset();
    uint16_t count = this->count();
    if ((itemOffset >= messageItemOffset) && (itemOffset <= messageNextItemOffset))
    {
        if (nextItemOffset > messageNextItemOffset)
            count += nextItemOffset - messageNextItemOffset;
    }
    else if ((nextItemOffset >= messageItemOffset) && (nextItemOffset <= messageNextItemOffset))
    {
        const uint16_t c = messageItemOffset - itemOffset;
        offset -= c;
        count += c;
    }
    else if ((itemOffset < messageItemOffset) && (nextItemOffset > messageNextItemOffset))
    {
        offset = mi->offset();
        count = mi->count();
    }
    else
    {
        m_device->logger()->logMessage(CnSTR("ModbusPokeMessage(%s)::AddItem(name=%s): Unsuccessful. ModbusItem is out of message data range"), fineName(), mi->name().data());
        return false;
    }
    if (count > this->m_maxCount)
    {
        m_device->logger()->logMessage(CnSTR("ModbusPokeMessage(%s)::AddItem(name=%s): Unsuccessful. ModbusMessage is too big"), fineName(), mi->name().data());
        return false;
    }
    this->m_offset = offset;
    this->m_count = count;
    addItemInner(mi);
    return true;
}

void CnDevicePokeMessage::beginProcess()
{
    CnDeviceMessage::beginProcess();
    CnCriticalSectionLocker _(&m_cs); 
    readFromItems();
}

void CnDevicePokeMessage::clearItems()
{
    CnCriticalSectionLocker _(&m_cs); 
    for (auto item : m_items)
    {
        signalRemovingItem(item);
        delete item;
    }
    m_items.clear();
}

// ------------------------------------------------------------------------------------------
// -------------------------- CnDevicePollMessageReadCoils -------------------------
// ------------------------------------------------------------------------------------------

CnDevicePollMessageReadCoils::CnDevicePollMessageReadCoils(CnDevice * device, CnDeviceModbusItem * item, uint16_t maxCount) :
    CnDevicePollMessage(device, item, maxCount)
{
    m_buff.resize((m_maxCount + 7) / 8);
}

bool CnDevicePollMessageReadCoils::getDataInner(uint16_t innerOffset, uint16_t count, void * buff) const
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::readMemBits(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}

bool CnDevicePollMessageReadCoils::setDataInner(uint16_t innerOffset, uint16_t count, const void * buff)
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::writeMemBits(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}


// ------------------------------------------------------------------------------------------
// --------------------- CnDevicePollMessageReadDiscreteInputs ---------------------
// ------------------------------------------------------------------------------------------

CnDevicePollMessageReadDiscreteInputs::CnDevicePollMessageReadDiscreteInputs(CnDevice * device, CnDeviceModbusItem * item, uint16_t maxCount) :
    CnDevicePollMessage(device, item, maxCount)
{
    m_buff.resize((m_maxCount + 7) / 8);
}

bool CnDevicePollMessageReadDiscreteInputs::getDataInner(uint16_t innerOffset, uint16_t count, void * buff) const
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::readMemBits(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}

bool CnDevicePollMessageReadDiscreteInputs::setDataInner(uint16_t innerOffset, uint16_t count, const void * buff)
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::writeMemBits(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}


// ------------------------------------------------------------------------------------------
// --------------------- CnDevicePollMessageReadInputRegisters ---------------------
// ------------------------------------------------------------------------------------------

CnDevicePollMessageReadInputRegisters::CnDevicePollMessageReadInputRegisters(CnDevice * device, CnDeviceModbusItem * item, uint16_t maxCount) :
    CnDevicePollMessage(device, item, maxCount)
{
    m_buff.resize(m_maxCount * 2);
}

bool CnDevicePollMessageReadInputRegisters::getDataInner(uint16_t innerOffset, uint16_t count, void * buff) const
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::readMemRegs(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}

bool CnDevicePollMessageReadInputRegisters::setDataInner(uint16_t innerOffset, uint16_t count, const void * buff)
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::writeMemRegs(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}


// ------------------------------------------------------------------------------------------
// --------------------- CnDevicePollMessageReadHoldingRegisters ---------------------
// ------------------------------------------------------------------------------------------

CnDevicePollMessageReadHoldingRegisters::CnDevicePollMessageReadHoldingRegisters(CnDevice * device, CnDeviceModbusItem * item, uint16_t maxCount) :
    CnDevicePollMessage(device, item, maxCount)
{
    m_buff.resize(m_maxCount * 2);
}

bool CnDevicePollMessageReadHoldingRegisters::getDataInner(uint16_t innerOffset, uint16_t count, void * buff) const
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::readMemRegs(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}

bool CnDevicePollMessageReadHoldingRegisters::setDataInner(uint16_t innerOffset, uint16_t count, const void * buff)
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::writeMemRegs(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}


// ------------------------------------------------------------------------------------------
// ---------------------- CnDevicePokeMessageWriteMultipleCoils ----------------------
// ------------------------------------------------------------------------------------------

CnDevicePokeMessageWriteMultipleCoils::CnDevicePokeMessageWriteMultipleCoils(CnDevice * device, CnDeviceModbusItem * item, uint16_t maxCount) :
    CnDevicePokeMessage(device, item, maxCount)
{
    m_buff.resize((m_maxCount + 7) / 8);
}

bool CnDevicePokeMessageWriteMultipleCoils::getDataInner(uint16_t innerOffset, uint16_t count, void * buff) const
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::readMemBits(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}

bool CnDevicePokeMessageWriteMultipleCoils::setDataInner(uint16_t innerOffset, uint16_t count, const void * buff)
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::writeMemBits(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}


// ------------------------------------------------------------------------------------------
// ------------------- CnDevicePokeMessageWriteMultipleRegisters -------------------
// ------------------------------------------------------------------------------------------

CnDevicePokeMessageWriteMultipleRegisters::CnDevicePokeMessageWriteMultipleRegisters(CnDevice * device, CnDeviceModbusItem * item, uint16_t maxCount) :
    CnDevicePokeMessage(device, item, maxCount)
{
    m_buff.resize(m_maxCount * 2);
}

bool CnDevicePokeMessageWriteMultipleRegisters::getDataInner(uint16_t innerOffset, uint16_t count, void * buff) const
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::readMemRegs(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}

bool CnDevicePokeMessageWriteMultipleRegisters::setDataInner(uint16_t innerOffset, uint16_t count, const void * buff)
{
    //CnCriticalSectionLocker _(&m_cs); 
    Modbus::StatusCode s = Modbus::writeMemRegs(innerOffset, count, buff, m_buff.data(), m_execCount);
    return Modbus::StatusIsGood(s);
}

namespace Cn {

CnDevicePollMessage *createDeviceModbusPollMessage(CnDevice *device, CnDeviceModbusItem *item)
{
    //device->logMessage(CnSTR("createDeviceModbusPollMessage. Item.Name='%s', Item.MemoryType=%i"), item->name().data(), item->memoryType());
    switch (item->memoryType())
    {
    case Modbus::Memory_0x:
        //device->logEntryExit(CnSTR("CnDeviceHierarchy::CreatePollMessage: CnDevicePollMessageReadCoils(item=%s,offset=%d)"), item->name().data(), item->offset());
        return new CnDevicePollMessageReadCoils(device, item, device->CfgMaxReadCoils());
    case Modbus::Memory_1x:
        //device->logEntryExit(CnSTR("CnDeviceHierarchy::CreatePollMessage: CnDevicePollMessageReadDiscreteInputs(item=%s,offset=%d)"), item->name().data(), item->offset());
        return new CnDevicePollMessageReadDiscreteInputs(device, item, device->CfgMaxReadDiscreteInputs());
    case Modbus::Memory_3x:
        //device->logEntryExit(CnSTR("CnDeviceHierarchy::CreatePollMessage: CnDevicePollMessageReadInputRegisters(item=%s,offset=%d)"), item->name().data(), item->offset());
        return new CnDevicePollMessageReadInputRegisters(device, item, device->CfgMaxReadInputRegisters());
    case Modbus::Memory_4x:
        //device->logEntryExit(CnSTR("CnDeviceHierarchy::CreatePollMessage: CnDevicePollMessageReadHoldingRegisters(item=%s,offset=%d)"), item->name().data(), item->offset());
        return new CnDevicePollMessageReadHoldingRegisters(device, item, device->CfgMaxReadHoldingRegisters());
    }
    return nullptr;
}

CnDevicePokeMessage *createDeviceModbusPokeMessage(CnDevice *device, CnDeviceModbusItem *item)
{
    switch (item->memoryType())
    {
    case Modbus::Memory_0x:
        //device->logEntryExit(CnSTR("CnDeviceHierarchy::CreatePokeMessage: CnDevicePokeMessageWriteMultipleCoils(item=%s,offset=%d)"), item->name().data(), item->offset());
        return new CnDevicePokeMessageWriteMultipleCoils(device, item, device->CfgMaxWriteMultipleCoils());
    case Modbus::Memory_4x:
        //device->logEntryExit(CnSTR("CnDeviceHierarchy::CreatePokeMessage: CnDevicePokeMessageWriteMultipleRegisters(item=%s,offset=%d)"), item->name().data(), item->offset());
        return new CnDevicePokeMessageWriteMultipleRegisters(device, item, device->CfgMaxWriteMultipleRegisters());
    }
    return nullptr;
}

} // namespace Cn
