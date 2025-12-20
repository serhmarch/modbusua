#include "CnUaPort.h"

#include <port/items/CnPortCfgItem.h>
#include <port/items/CnPortStatItem.h>
#include <port/items/CnPortSetItem.h>
#include <port/items/CnPortCmdItem.h>

#include <app/CnUaApp.h>
#include <device/CnUaDevice.h>

#include <port/CnPortConfig.h>

using namespace CnUa;

UA_StatusCode CnUaPort::readSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data)
{
    CnPortBaseItem *item = reinterpret_cast<CnPortBaseItem*>(nodeContext);
    data->hasValue = true;
    data->value = CnUa::toUA_Variant(item->value());
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode CnUaPort::writeSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeId, void *nodeContext, const UA_NumericRange *range, const UA_DataValue *data)
{
    CnPortBaseItem *item = reinterpret_cast<CnPortBaseItem*>(nodeContext);
    item->update(CnUa::toVariant(data->value));
    return UA_STATUSCODE_GOOD;
}


CnUaPort::CnUaPort(CnUaServer *server, const CnString &nodeName) : CnPort()
{
    m_server = server;
    m_cfg->setCfgName(nodeName);
}

CnUaPort::~CnUaPort()
{
}

void CnUaPort::initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId)
{
    CnCriticalSectionLocker _(&m_vars.cs);
    m_nodeIdParent = parentId;
    m_nodeIdParentRef = refTypeId;
}

void CnUaPort::initialize()
{
    CnPort::initialize();
    CnCriticalSectionLocker _(&m_vars.cs);
    innerInitUaNode();
    for (CnDevice *device : m_vars.devices)
        innerInitUaDevice(device);
}

void CnUaPort::finalize()
{
    CnCriticalSectionLocker _(&m_vars.cs);
    innerClearUaNode();
    for (CnPortBaseItem * item : m_items)
        delete item;
    m_items.clear();
    CnPort::finalize();
}

void CnUaPort::addDeviceInner(CnDevice *device)
{
    if (!m_nodeId.isNull())
        innerInitUaDevice(device);
    CnPort::addDeviceInner(device);
}

void CnUaPort::innerInitUaNode()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    UA_VariableAttributes attrVar = UA_VariableAttributes_default;
    attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;
    std::string sValue;

    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);

    // Port root node
    std::string sname = Cn::toStdString(name());
    attr.displayName = UA_LOCALIZEDTEXT(locale, const_cast<char*>(sname.data()));
    UA_Server_addObjectNode(m_server->server(), 
                            dummy, 
                            m_nodeIdParent,
                            m_nodeIdParentRef, 
                            UA_QUALIFIEDNAME(m_server->nsIdx(), const_cast<char*>(sname.data())),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), 
                            attr, 
                            NULL, 
                            m_nodeId.data());

    // Port PLCs node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Devices");
    attr.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    UA_Server_addObjectNode(m_server->server(), 
                            dummy, 
                            m_nodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                            UA_QUALIFIEDNAME(m_server->nsIdx(), namebuff),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), 
                            attr, 
                            NULL, 
                            m_nodeIdDevices.data());

    innerInitUaCfg();
    innerInitUaCmd();
    innerInitUaSet();
    innerInitUaStat();
}

void CnUaPort::innerClearUaNode()
{
    for (CnPortBaseItem * item : m_items)
        innerClearUaItem(item);
    UA_Server_deleteNode(m_server->server(), m_nodeId, true);
}

void CnUaPort::innerInitUaDevice(CnDevice *device)
{
    static_cast<CnUaDevice*>(device)->initializeUa(m_nodeIdDevices, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
}

void CnUaPort::innerInitUaCfg()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    UA_StatusCode s;

    //---------------------------------------------------
    // Port Cfg node
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

    CnPortBaseItem *item;

#define INIT_CFG(name)    item = new CnPortCfgItem##name(this); item->setName(CnSTR(#name)); addItem(item);

INIT_CFG(Name             )
INIT_CFG(Enable           )
INIT_CFG(Type             )
INIT_CFG(Host             )
INIT_CFG(Port             )
INIT_CFG(Timeout          )
INIT_CFG(SerialPortName   )
INIT_CFG(BaudRate         )
INIT_CFG(DataBits         )
INIT_CFG(Parity           )
INIT_CFG(StopBits         )
INIT_CFG(FlowControl      )
INIT_CFG(TimeoutFirstByte )
INIT_CFG(TimeoutInterByte )
INIT_CFG(LogFlags         )
INIT_CFG(LogOutput        )
INIT_CFG(LogFileFormat    )
INIT_CFG(LogFileTimeformat)
INIT_CFG(LogFilePath      )
INIT_CFG(LogFileMaxCount  )
INIT_CFG(LogFileMaxSize   )

}

void CnUaPort::innerInitUaCmd()
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

    CnPortBaseItem *item;

#define INIT_CMD(name)    item = new CnPortCmdItem##name(this); item->setName(CnSTR(#name)); addItem(item);

INIT_CMD(StatClear)
}

void CnUaPort::innerInitUaSet()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    UA_StatusCode s;

    //---------------------------------------------------
    // Port Set node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Set");
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
                                  m_nodeIdSet.data());

    UA_VariableAttributes attrVar = UA_VariableAttributes_default;
    attrVar.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    attrVar.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;

    CnPortBaseItem *item;

#define INIT_SETTING(name)    item = new CnPortSetItem##name(this); item->setName(CnSTR(#name)); addItem(item);

INIT_SETTING(Enable)

}

void CnUaPort::innerInitUaStat()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    UA_StatusCode sc;

    //---------------------------------------------------
    // Port Stat node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Stat");
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    attr.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    sc = UA_Server_addVariableNode(m_server->server(), 
                                   dummy, 
                                   m_nodeId,
                                   UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                                   UA_QUALIFIEDNAME(m_server->nsIdx(), namebuff),
                                   UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), 
                                   attr, 
                                   NULL, 
                                   m_nodeIdStat.data());

    UA_VariableAttributes attrVar = UA_VariableAttributes_default;
    attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_DataSource callback;
    callback.write = CnUa::writeDummy;

    CnPortBaseItem *item;

#define INIT_STAT(name)    item = new CnPortStatItem##name(this); item->setName(CnSTR(#name)); addItem(item);

INIT_STAT(ThreadCycleCount    )
INIT_STAT(ThreadLastCycle     )
INIT_STAT(ThreadMinCycle      )
INIT_STAT(ThreadMaxCycle      )
INIT_STAT(ThreadAvgCycle      )
INIT_STAT(GoodCount           )
INIT_STAT(BadCount            )
INIT_STAT(Status              )
INIT_STAT(Timestamp           )
INIT_STAT(LastSuccessTimestamp)
INIT_STAT(LastErrorStatus     )
INIT_STAT(LastErrorTimestamp  )
INIT_STAT(LastErrorText       )

}

void CnUaPort::addItem(CnPortBaseItem *item)
{
    m_items.push_back(item);
    innerInitUaItem(item);
}

void CnUaPort::innerInitUaItem(CnPortBaseItem *item)
{
    char locale[] = { '\0' };

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
    case CnPortBaseItem::Item_Config:
    {
        nodeIdParent = m_nodeIdCfg;
        sId = Cn::toStdString(nodeName())+".Cfg."+sname;
        attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;
        callback.read  = CnUaPort::readSimpleItem;
        callback.write = CnUa::writeDummy;
    }
        break;
    case CnPortBaseItem::Item_Statistic:
    {
        nodeIdParent = m_nodeIdStat;
        sId = Cn::toStdString(nodeName())+".Stat."+sname;
        attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;
        callback.read  = CnUaPort::readSimpleItem;
        callback.write = CnUa::writeDummy;
    }
        break;
    case CnPortBaseItem::Item_Setting:
    {
        nodeIdParent = m_nodeIdSet;
        sId = Cn::toStdString(nodeName())+".Set."+sname;
        attrVar.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        callback.read  = CnUaPort::readSimpleItem;
        callback.write = CnUaPort::writeSimpleItem;
    }
        break;
    case CnPortBaseItem::Item_Command:
    {
        nodeIdParent = m_nodeIdCmd;
        sId = Cn::toStdString(nodeName())+".Cmd."+sname;
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
    CnUa::setNodeId(item, itemId);
}

void CnUaPort::innerClearUaItem(CnPortBaseItem *item)
{
    CnUa::NodeId nodeId = CnUa::getNodeId(item);
    if (!nodeId.isNull())
    {
        UA_Server_deleteNode(m_server->server(), nodeId, true);
        CnUa::clearNodeId(item);
    }
}
