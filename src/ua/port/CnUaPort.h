/*!
 * \file   CnUaPort.h
 * \brief  Описує структуру, що зберігає всі параметри порту етапу виконання програми
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNUAPORT_H
#define CNUAPORT_H

#include <port/CnPort.h>
#include <CnUaServer.h>

class CnPortBaseItem;

/*! \brief Описує структуру, що зберігає всі параметри порту етапу виконання програми.

    \details Даний клас містить всі дані конфігурації, налаштувань, статистики, команди, що стосуються роботи даного порту.
    Також даний клас містить функції журналювання повідомлень для даного об'єкту.
 */
class CnUaPort : public CnPort
{
public:
    static UA_StatusCode readSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data);
    static UA_StatusCode writeSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeId, void *nodeContext, const UA_NumericRange *range, const UA_DataValue *data);

public:
    CnUaPort(CnUaServer *server, const CnString &nodeName);
    ~CnUaPort();

public:
    void initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId);
    void initialize() override;
    void finalize() override;

protected:
    void addDeviceInner(CnDevice* device) override;

private:
    void innerInitUaNode();
    void innerInitUaCfg();
    void innerInitUaCmd();
    void innerInitUaSet();
    void innerInitUaStat();
    void innerClearUaNode();
    void innerInitUaDevice(CnDevice *device);

protected:
    void addItem(CnPortBaseItem *item);

protected:
    inline CnString nodeName() const { return name(); }

private:
    void innerInitUaItem(CnPortBaseItem *item);
    void innerClearUaItem(CnPortBaseItem *item);

protected:
    CnUaServer *m_server;
    CnUa::NodeId m_nodeId;
    CnUa::NodeId m_nodeIdParent;
    CnUa::NodeId m_nodeIdParentRef;
    CnUa::NodeId m_nodeIdDevices;
    CnUa::NodeId m_nodeIdCfg;
    CnUa::NodeId m_nodeIdCmd;
    CnUa::NodeId m_nodeIdSet;
    CnUa::NodeId m_nodeIdStat;
    CnList<CnPortBaseItem*> m_items;
};

#endif // CNUAPORT_H