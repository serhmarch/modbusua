#include "CnUaApp.h"

#include <CnStd_iostream.h>
#include <cstdio>

#include "CnUaServer.h"

#include <cfg/CnCfgBuilder.h>
#include <cfg/CnCfgProject.h>
#include <cfg/CnCfgPort.h>
#include <cfg/CnCfgDevice.h>

#include <device/items/CnDeviceBaseItem.h>
#include <app/items/CnAppCfgItem.h>
#include <app/items/CnAppCmdItem.h>

#include <CnUaServer.h>
#include <port/CnUaPort.h>
#include <device/CnUaDevice.h>
#include "CnUaThread.h"

UA_StatusCode CnUaApp::readSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data)
{
    CnAppBaseItem *item = reinterpret_cast<CnAppBaseItem*>(nodeContext);
    data->hasValue = true;
    data->value = CnUa::toUA_Variant(item->value());
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode CnUaApp::writeSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeId, void *nodeContext, const UA_NumericRange *range, const UA_DataValue *data)
{
    CnAppBaseItem *item = reinterpret_cast<CnAppBaseItem*>(nodeContext);
    item->update(CnUa::toVariant(data->value));
    return UA_STATUSCODE_GOOD;
}

CnUaApp::CnUaApp() : CnApp()
{
    m_server = nullptr;
    m_thread = nullptr;
}

CnUaApp::~CnUaApp()
{
}

void CnUaApp::initialize(CnCfgProject *project)
{
    m_server = new CnUaServer();
    innerInitUaNode();
    CnApp::initialize(project);
    m_thread = new CnUaThread(m_server);
    m_thread->start();
}

void CnUaApp::beginToFinalize()
{
    m_thread->stop();
}

void CnUaApp::finalize()
{
    CnApp::finalize();
    m_thread->waitForStop();
    delete m_thread;
    m_thread = nullptr;
    delete m_server;
    m_server = nullptr;
}

void CnUaApp::printVersion()
{
    CnApp::printVersion();
    CnStd::cout << CnSTR("open62541 version: ") << CnSTR(CN_UA_VERSION_STR) << std::endl;
}

void CnUaApp::printHelp()
{
    CnApp::printHelp();
}

CnPort *CnUaApp::createPort(const CnCfgPort *cfg)   
{
    CnUaPort *port = new CnUaPort(m_server, cfg->name());
    port->setConfig(cfg->config());
    port->initializeUa(m_nodeIdPorts, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
    return port;
}

CnDevice *CnUaApp::createDevice(const CnCfgDevice *cfg)
{
    CnUaPort *port = static_cast<CnUaPort*>(cnPort(cfg->portName()));
    if (port)
    {
        CnUaDevice *dev = new CnUaDevice(m_server, cfg->name());
        return dev;
    }
    return nullptr;
}

void CnUaApp::setGroupConfig(const Cn::GroupConfig &groupConfig)
{
    auto it = groupConfig.find(CnUaServer::Strings::instance().cfgGroup);
    if (it != groupConfig.end())
        m_server->setConfig(it->second);
}

void CnUaApp::innerInitUaNode()
{
    char locale[] = { '\0' };
    char name[CN_UA_MAX_NAME];

    // System Cfg node
    std::snprintf(name, CN_UA_MAX_NAME, "%s", CN_APP_NAME);

    /* Add a variable node to the server */

    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);

    /* Define where the node shall be added with which browsename */
    UA_NodeId confNodeId = UA_NODEID_NUMERIC(m_server->nsIdx(),0);//UA_NODEID_STRING(nsIdx(), "Config");
    UA_QualifiedName confBrowseName = UA_QUALIFIEDNAME(m_server->nsIdx(), name);
    
    /* Add the node */
    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT(locale, name);

    UA_Server_addObjectNode(m_server->server(), 
                            confNodeId, 
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), 
                            confBrowseName,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), 
                            attr, 
                            NULL, 
                            m_nodeId.data());

    // Ports node
    std::snprintf(name, CN_UA_MAX_NAME, "%s", "Ports");
    attr.displayName = UA_LOCALIZEDTEXT(locale, name);
    UA_Server_addObjectNode(m_server->server(), 
                            dummy, 
                            m_nodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                            UA_QUALIFIEDNAME(m_server->nsIdx(), name),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), 
                            attr, 
                            NULL, 
                            m_nodeIdPorts.data());

    innerInitUaCfg();
    innerInitUaCmd();
}

void CnUaApp::innerInitUaCfg()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    UA_StatusCode s;

    //---------------------------------------------------
    // System Cfg node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Cfg");
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    attr.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    s = UA_Server_addVariableNode(m_server->server(), 
                                  dummy, 
                                  m_nodeId,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                                  UA_QUALIFIEDNAME(m_server->nsIdx(), namebuff),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), 
                                  attr, 
                                  NULL, 
                                  m_nodeIdCfg.data());

    UA_VariableAttributes attrVar = UA_VariableAttributes_default;
    attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_DataSource callback;
    callback.write = CnUa::writeDummy;

    CnAppBaseItem *item;

#define INIT_CFG(name)    item = new CnAppCfgItem##name(this); item->setName(CnSTR(#name)); innerInitUaItem(item);

INIT_CFG(LogFlags            )
INIT_CFG(LogOutput           )
INIT_CFG(LogDefaultFormat    )
INIT_CFG(LogDefaultTimeformat)
INIT_CFG(LogSystemFormat     )
INIT_CFG(LogSystemTimeformat )
INIT_CFG(LogConsoleFormat    )
INIT_CFG(LogConsoleTimeformat)
INIT_CFG(LogFileFormat       )
INIT_CFG(LogFileTimeformat   )
INIT_CFG(LogFilePath         )
INIT_CFG(LogFileMaxCount     )
INIT_CFG(LogFileMaxSize      )

}

void CnUaApp::innerInitUaCmd()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    UA_StatusCode s;

    //---------------------------------------------------
    // Port Cmd node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Cmd");
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    attr.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    s = UA_Server_addVariableNode(m_server->server(), 
                                  dummy, 
                                  m_nodeId,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                                  UA_QUALIFIEDNAME(m_server->nsIdx(), namebuff),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), 
                                  attr, 
                                  NULL, 
                                  m_nodeIdCmd.data());

    UA_VariableAttributes attrVar = UA_VariableAttributes_default;
    attrVar.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    attrVar.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;

    UA_DataSource callback;
    callback.read = CnUa::readDummyBool;

    CnAppBaseItem *item;

#define INIT_CMD(name)    item = new CnAppCmdItem##name(this); item->setName(CnSTR(#name)); innerInitUaItem(item);

INIT_CMD(ReloadConfig)
}

void CnUaApp::innerInitUaItem(CnAppBaseItem *item)
{
    char locale[] = { '\0' };

    m_items.push_back(item);

    std::string sname = Cn::toStdString(item->name());
    std::string sId;

    int tpIndex = CnUa::toUaTypesIndex(item->dataType());

    UA_VariableAttributes attrVar = UA_VariableAttributes_default;
    UA_Variant_init(&attrVar.value);
    attrVar.displayName = UA_LOCALIZEDTEXT(locale, const_cast<char*>(sname.data()));
    std::string sDescr = Cn::toStdString(item->description());
    attrVar.description = UA_LOCALIZEDTEXT(locale, const_cast<char*>(sDescr.data()));
    attrVar.dataType = UA_TYPES[tpIndex].typeId;
    UA_StatusCode sc;
    UA_DataSource callback;
    UA_NodeId nodeIdParent;

    switch (item->itemType())
    {
    case CnAppBaseItem::Item_Config:
    {
        nodeIdParent = m_nodeIdCfg;
        sId = CN_APP_NAME".Cfg."+sname;
        attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;
        callback.read  = CnUaPort::readSimpleItem;
        callback.write = CnUa::writeDummy;
    }
        break;
    case CnAppBaseItem::Item_Command:
    {
        nodeIdParent = m_nodeIdCmd;
        sId = CN_APP_NAME".Cmd."+sname;
        attrVar.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        callback.read  = CnUa::readDummyBool;
        callback.write = CnUaPort::writeSimpleItem;
    }
        break;
    }

    UA_NodeId itemId = UA_NODEID_STRING(m_server->nsIdx(), const_cast<char*>(sId.data()));
    sc = UA_Server_addVariableNode(m_server->server(), 
                                   itemId, 
                                   nodeIdParent,
                                   UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), 
                                   UA_QUALIFIEDNAME(m_server->nsIdx(), const_cast<char*>(sname.data())),
                                   UA_NODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE), 
                                   attrVar,
                                   item, // Set Node Context to current item
                                   nullptr);

    UA_Server_setVariableNode_dataSource(m_server->server(),
                                         itemId,
                                         callback);
}
