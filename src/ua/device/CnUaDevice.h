/*!
 * \file   CnUaDevice.h
 * \brief  Описує структуру, що зберігає всі параметри пристрою/ПЛК етапу виконання програми
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNUADEVICE_H
#define CNUADEVICE_H

#include <device/CnDevice.h>

#include "items/CnUaDeviceItemList.h"
#include "messages/CnUaDeviceMessageList.h"

#include "CnUaServer.h"

class CnDeviceBaseItem;
class CnDeviceModbusItem;
class CnUaPort;

/*! \brief 

    \details 
 */
class CnUaDevice : public CnDevice
{
public:
    // Items
    static UA_StatusCode readSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data);
    static UA_StatusCode writeSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeId, void *nodeContext, const UA_NumericRange *range, const UA_DataValue *data);

    static UA_StatusCode readItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data);

    static UA_StatusCode readModbusItem (UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data);
    static UA_StatusCode writeModbusItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeId, void *nodeContext, const UA_NumericRange *range, const UA_DataValue *data);

public:
    CnUaDevice(CnUaServer *server, const CnString &nodeName);
    ~CnUaDevice();

public:
    inline CnUaDeviceItemList *uaItems() const { return static_cast<CnUaDeviceItemList*>(m_items); }
    inline CnUaDevicePokeMessageList *uaPokeMessages() const { return static_cast<CnUaDevicePokeMessageList*>(m_poke); }
    inline CnUaDevicePollMessageList *uaPollMessages() const { return static_cast<CnUaDevicePollMessageList*>(m_poll); }

public:
    void initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId);
    void initialize() override;
    void finalize() override;

public:
    void renewBaseItem(const CnCfgDeviceItem *cfg, CnDeviceBaseItem *item) override;

private:
    void innerInitUaNode();
    void innerInitUaCfg();
    void innerInitUaCmd();
    void innerInitUaSet();
    void innerInitUaStat();
    void innerClearUaNode();

protected:
    void addItemExtInner(CnDeviceBaseItem *item);

private:
    void innerAddUaItem(CnDeviceBaseItem *item);
    void innerClearUaItem(CnDeviceBaseItem *item);

private:
    inline CnString nodeName() const { return name(); }

private:
    mutable CnCriticalSection m_cs;
    CnUaServer *m_server;
    CnUa::NodeId m_nodeId;
    CnUa::NodeId m_nodeIdParent;
    CnUa::NodeId m_nodeIdParentRef;
    CnUa::NodeId m_nodeIdCfg;
    CnUa::NodeId m_nodeIdCmd;
    CnUa::NodeId m_nodeIdSet;
    CnUa::NodeId m_nodeIdStat;
    CnUa::NodeId m_nodeIdItems;
    CnUa::NodeId m_nodeIdMessages;

    CnList<CnDeviceBaseItem*> m_itemsExt;
};

#endif // CNUADEVICE_H