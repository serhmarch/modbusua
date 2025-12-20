/*!
 * \file   CnUaDeviceMessage.h
 * \brief  Класи для роботи з Modbus-повідомленнями об'єкта пристрою/ПЛК.
 *
 * \author serhmarch
 * \date   January 2025
 */
#ifndef CNUADEVICEMODBUSMESSAGE_H
#define CNUADEVICEMODBUSMESSAGE_H

#include <device/messages/CnDeviceMessage.h>
#include "CnUaServer.h"

class CnUaDevice;

// ------------------------------------------------------------------------------------------
// ---------------------------------- CnUaDeviceMessage ----------------------------------
// ------------------------------------------------------------------------------------------

class CnUaDeviceMessage : public CnDeviceMessageBase
{
public:
    template <class R, R (CnDeviceMessage::*PMethod)() const>
    static UA_StatusCode readStat(UA_Server *server,
                                  const UA_NodeId *sessionId,
                                  void *sessionContext,
                                  const UA_NodeId *nodeid,
                                  void *nodeContext,
                                  UA_Boolean sourceTimeStamp,
                                  const UA_NumericRange *range,
                                  UA_DataValue *data)
    {
        CnDeviceMessage *message = reinterpret_cast<CnDeviceMessage*>(nodeContext);
        data->hasValue = true;
        data->value = CnUa::toUA_Variant((message->*PMethod)());
        return UA_STATUSCODE_GOOD;
    }

    template <class RT> 
    inline RT getMethodRT(RT(CnDeviceMessage::*)()const) { return RT(); }
    
public:
    CnUaDeviceMessage(CnDeviceMessage *innerMessage);
    ~CnUaDeviceMessage();

public: // CnDeviceMessage interface
    void initialize() override;
    void finalize() override;
    const CnChar* fineName() const override;
    uint16_t offset() const override;
    uint16_t count() const override;
    uint16_t execOffset() const override;
    uint16_t execCount() const override;
    int period() const override;
    void* innerBuffer() override;
    uint32_t innerBufferSize() const override;
    Cn::StatusCode status() const override;
    CnTimestamp timestamp() const override;
    CnTimestamp timestampBegin() const override;
    void beginProcess() override;
    bool addItem(CnDeviceModbusItem* item) override;
    void setComplete(Modbus::StatusCode status, CnTimestamp timestamp, const CnString &err = CnString()) override;
    Modbus::MemoryType memoryType() const override;
    int ModbusFunction() const override;
    bool getData(uint16_t innerOffset, uint16_t count, void* buff) const override;
    bool setData(uint16_t innerOffset, uint16_t count, const void* buff) override;
    uint32_t itemCount() const override;
    void clearItems() override;

public:
    void initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId);

protected:
    void innerInitUaNode();
    void innerClearUaNode();
    void innerInitUaItem(CnDeviceModbusItem *mi);
    void innerClearUaItem(CnDeviceModbusItem *mi);

protected:
    mutable CnCriticalSection m_cs;
    CnDeviceMessage *m_innerMessage;
    CnUaServer *m_server;
    CnUa::NodeId m_nodeId;
    CnUa::NodeId m_nodeIdParent;
    CnUa::NodeId m_nodeIdParentRef;
    CnUa::NodeId m_nodeIdStat;
    CnUa::NodeId m_nodeIdItems;

};

// ------------------------------------------------------------------------------------------
// -------------------------------- CnUaDevicePollMessage --------------------------------
// ------------------------------------------------------------------------------------------

/*!  \brief  Базовий клас для роботи з Modbus-повідомленнями читання для об'єкта пристрою/ПЛК.

    \details Клас повідомлення, що відноситься до об'єкта-пристрою/ПЛК, є групою змінних 'item reference', що читаються одночасно.
    Дані повідомлення вичитуються із віддаленого пристрою/ПЛК, оновлюючи при цьому мітку часу і якість даних 'item reference'.
*/
class CnUaDevicePollMessage : public CnUaDeviceMessage
{
public:
    CnUaDevicePollMessage(CnDeviceMessage *innerMessage);
    ~CnUaDevicePollMessage();

public:
    void initialize() override;
    void finalize() override;

public: // CnDeviceMessage interface
    bool addItem(CnDeviceModbusItem* item) override;
};


// ------------------------------------------------------------------------------------------
// -------------------------------- CnUaDevicePokeMessage --------------------------------
// ------------------------------------------------------------------------------------------

/*! \brief  Базовий клас для роботи з Modbus-повідомленнями запису для об'єкта пристрою/ПЛК.

    \details Клас повідомлення, що відноситься до об'єкта-пристрою/ПЛК, є групою змінних 'item reference', що записуються одночасно.
    Дані повідомлення записуються у віддалений пристрій/ПЛК, оновлюючи при цьому мітку часу і якість даних 'item reference'.
*/
class CnUaDevicePokeMessage : public CnUaDeviceMessage
{
public:
    CnUaDevicePokeMessage(CnDeviceMessage *innerMessage);
    ~CnUaDevicePokeMessage();
};

#endif // CNDEVICEMODBUSMESSAGE_H