/*!
 * \file   CnUaApp.h
 * \brief  
 *
 * \author serhmarch
 * \date   January 2025
 */
#ifndef CNUASYSTEM_H
#define CNUASYSTEM_H

#include <app/CnApp.h>
#include <CnUaServer.h>

class CnAppBaseItem;
class CnUaThread;

class CnUaApp : public CnApp
{
public:
    static UA_StatusCode readSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data);
    static UA_StatusCode writeSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeId, void *nodeContext, const UA_NumericRange *range, const UA_DataValue *data);

public:
    inline static CnUaApp *globalUa() { return static_cast<CnUaApp*>(CnApp::global()); }

public:
    CnUaApp();
    ~CnUaApp();

public:
    inline CnUaServer *server() const { return m_server; } 

protected:
    void initialize(CnCfgProject *project) override;
    void beginToFinalize() override;
    void finalize() override;

protected:
    void printVersion() override;
    void printHelp() override;
    
protected:
    CnPort *createPort(const CnCfgPort *cfg) override;
    CnDevice *createDevice(const CnCfgDevice *cfg) override;
    void setGroupConfig(const Cn::GroupConfig &groupConfig) override;
    
protected:
    void innerInitUaNode();
    void innerInitUaCfg();
    void innerInitUaCmd();
    void innerInitUaItem(CnAppBaseItem *item);

protected:
    CnUaServer *m_server;
    CnUaThread *m_thread;
    CnUa::NodeId m_nodeId;
    CnUa::NodeId m_nodeIdPorts;
    CnUa::NodeId m_nodeIdCfg;
    CnUa::NodeId m_nodeIdCmd;
    CnList<CnAppBaseItem*> m_items;
};

#endif // CNUASYSTEM_H