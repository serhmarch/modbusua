#include "CnUaDevice.h"

#include <device/items/CnDeviceCfgItem.h>
#include <device/items/CnDeviceStatItem.h>
#include <device/items/CnDeviceSetItem.h>
#include <device/items/CnDeviceCmdItem.h>
#include <device/items/CnDeviceModbusItem.h>

#include <device/CnDeviceConfig.h>

#include <CnUaServer.h>

using namespace CnUa;

UA_StatusCode CnUaDevice::readSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data)
{
    CnDeviceBaseItem *item = reinterpret_cast<CnDeviceBaseItem*>(nodeContext);
    data->hasValue = true;
    data->value = CnUa::toUA_Variant(item->value());
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode CnUaDevice::writeSimpleItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeId, void *nodeContext, const UA_NumericRange *range, const UA_DataValue *data)
{
    CnDeviceBaseItem *item = reinterpret_cast<CnDeviceBaseItem*>(nodeContext);
    item->update(CnUa::toVariant(data->value));
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode CnUaDevice::readItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data)
{
    CnDeviceBaseItem *item = reinterpret_cast<CnDeviceBaseItem*>(nodeContext);
    Cn::StatusCode status;
    CnTimestamp timestamp;
    data->hasValue = true;
    CnVariant v = item->value(&status, &timestamp);
    data->value = CnUa::toUA_Variant(v);
    data->hasStatus = true;
    data->status = CnUaServer::toUA_StatusCode(status);
    data->hasSourceTimestamp = true;
    data->sourceTimestamp = CnUa::toUA_DateTime(timestamp);
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode CnUaDevice::readModbusItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data)
{
    CnDeviceModbusItem *item = reinterpret_cast<CnDeviceModbusItem*>(nodeContext);
    if (!item->message())
    {
        CnDevice *d = item->device();
        d->addPollItem(item);
    }
    item->refreshTimestampLastRequest();
    Cn::StatusCode status;
    CnTimestamp timestamp;
    CnVariant v = item->value(&status, &timestamp);
    data->hasValue = true;
    data->value = CnUa::toUA_Variant(v);
    data->hasStatus = true;
    data->status = CnUaServer::toUA_StatusCode(status);
    //data->hasSourceTimestamp = true;
    //data->sourceTimestamp = CnUa::toUA_DateTime(timestamp);
    //data->hasServerTimestamp = false; // tmp
    //data->serverTimestamp = data->sourceTimestamp; // tmp
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode CnUaDevice::writeModbusItem(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeId, void *nodeContext, const UA_NumericRange *range, const UA_DataValue *data)
{
    CnDeviceModbusItem *item = reinterpret_cast<CnDeviceModbusItem*>(nodeContext);
    CnDevice *d = item->device();
    if (!item->message())
        d->addPollItem(item);
    item->refreshTimestampLastRequest();
    d->addPokeItem(item, CnUa::toVariant(data->value));
    return UA_STATUSCODE_GOOD;
}

CnUaDevice::CnUaDevice(CnUaServer *server, const CnString &nodeName) :
    CnDevice(new CnUaDeviceItemList(server, nodeName, CnSTR("Items")),
             new CnUaDevicePokeMessageList(this, server, CnSTR("")),
             new CnUaDevicePollMessageList(this, server, CnSTR("Messages")))
{
    m_server = server;
    m_cfg->setCfgName(nodeName);
}

CnUaDevice::~CnUaDevice()
{
}

void CnUaDevice::initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId)
{
    CnCriticalSectionLocker _(&m_cs);
    m_nodeIdParent = parentId;
    m_nodeIdParentRef = refTypeId;
}

void CnUaDevice::initialize()
{
    CnDevice::initialize();
    CnCriticalSectionLocker _(&m_cs);
    innerInitUaNode();
}

void CnUaDevice::finalize()
{
    CnCriticalSectionLocker _(&m_cs);
    innerClearUaNode();    
    for (CnDeviceBaseItem *item : m_itemsExt)
        delete item;
    m_itemsExt.clear();
    CnDevice::finalize();
}

void CnUaDevice::renewBaseItem(const CnCfgDeviceItem *cfg, CnDeviceBaseItem *item)
{
    char locale[] = { '\0' };

    CnDevice::renewBaseItem(cfg, item);
    CnUa::NodeId nodeId = CnUa::getNodeId(item);
    if (!nodeId.isNull())
    {
        std::string sDescr = Cn::toStdString(item->description());
        UA_Server_writeDescription(m_server->server(),
                                   *nodeId.constData(),
                                   UA_LOCALIZEDTEXT(locale, const_cast<char*>(sDescr.data())));
    }
}

void CnUaDevice::innerInitUaNode()
{
    char locale[] = { '\0' };

    std::string sname = Cn::toStdString(this->nodeName());
    std::string sId;
    CnUa::NodeId itemId;
    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    UA_VariableAttributes attrVar = UA_VariableAttributes_default;
    attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;
    UA_UInt32 arrayDims[2] = {0, 0};
    attrVar.arrayDimensions = arrayDims;
    UA_Variant_init(&attrVar.value);

    std::string sValue;

    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    UA_StatusCode sc;  
    //UA_Variant nullVariant;
    //UA_Variant_init(&nullVariant);

    // Device root node
    sname = Cn::toStdString(this->nodeName());
    attr.displayName = UA_LOCALIZEDTEXT(locale, const_cast<char*>(sname.data()));
    sc = UA_Server_addObjectNode(m_server->server(), 
                                 dummy, 
                                 m_nodeIdParent,
                                 m_nodeIdParentRef, 
                                 UA_QUALIFIEDNAME(m_server->nsIdx(), const_cast<char*>(sname.data())),
                                 UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), 
                                 attr, 
                                 NULL, 
                                 m_nodeId.data());
    if (sc != UA_STATUSCODE_GOOD)
    {
        logger()->logWarning(CnSTR("Can't initialize device '%s' UA node"), name().data());
        return;
    }
    //----------------------------------------------------------------------------------
    // Device Items node
    uaItems()->initializeUa(m_nodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));

    //----------------------------------------------------------------------------------
    // Device Poke Messages node
    //uaPokelMessages()->initializeUa(m_nodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));

    //----------------------------------------------------------------------------------
    // Device Poll Messages node
    uaPollMessages()->initializeUa(m_nodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));

    innerInitUaCfg();
    innerInitUaCmd();
    innerInitUaSet();
    innerInitUaStat();
}

void CnUaDevice::innerInitUaCfg()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    std::string sname = Cn::toStdString(nodeName());
    std::string sId;
    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(), 0);
    //UA_StatusCode sc;


    //---------------------------------------------------
    // Device Cfg node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Cfg");
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    attr.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    UA_Server_addVariableNode(m_server->server(), 
                              dummy, 
                              m_nodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                              UA_QUALIFIEDNAME(m_server->nsIdx(), namebuff),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), 
                              attr, 
                              NULL, 
                              m_nodeIdCfg.data());

    CnDeviceBaseItem *item;

#define INIT_CFG(name)    item = new CnDeviceCfgItem##name(this); item->setName(CnSTR(#name)); addItemExtInner(item);

INIT_CFG(Name                     )
INIT_CFG(EnableDevice             )
INIT_CFG(ModbusUnit               )
INIT_CFG(RepeatCount              )
INIT_CFG(RestoreTimeout           )
INIT_CFG(MaxReadCoils             )
INIT_CFG(MaxWriteMultipleCoils    )
INIT_CFG(MaxReadDiscreteInputs    )
INIT_CFG(MaxReadInputRegisters    )
INIT_CFG(MaxReadHoldingRegisters  )
INIT_CFG(MaxWriteMultipleRegisters)
INIT_CFG(DefaultPeriod            )
INIT_CFG(RequestTimeout           )
INIT_CFG(LogFlags                 )
INIT_CFG(LogOutput                )
INIT_CFG(LogFileFormat            )
INIT_CFG(LogFileTimeformat        )
INIT_CFG(LogFilePath              )
INIT_CFG(LogFileMaxCount          )
INIT_CFG(LogFileMaxSize           )
}

void CnUaDevice::innerInitUaCmd()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    std::string sname = Cn::toStdString(nodeName());
    std::string sId;
    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    //UA_StatusCode sc;

    //---------------------------------------------------
    // Device Cmd node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Cmd");
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    attr.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    UA_Server_addVariableNode(m_server->server(), 
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

    CnDeviceBaseItem *item;

#define INIT_CMD(name)    item = new CnDeviceCmdItem##name(this); item->setName(CnSTR(#name)); addItemExtInner(item);

INIT_CMD(StatClear)

}

void CnUaDevice::innerInitUaSet()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    std::string sname = Cn::toStdString(nodeName());
    std::string sId;
    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    //UA_StatusCode sc;


    //---------------------------------------------------
    // Device Set node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Set");
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    attr.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    UA_Server_addVariableNode(m_server->server(), 
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

    CnDeviceBaseItem *item;

#define INIT_SETTING(name)    item = new CnDeviceSetItem##name(this); item->setName(CnSTR(#name)); addItemExtInner(item);

INIT_SETTING(EnableDevice)

}

void CnUaDevice::innerInitUaStat()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    std::string sname = Cn::toStdString(nodeName());
    std::string sId;
    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    //UA_StatusCode sc;

    //---------------------------------------------------
    // Device Stat node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Stat");
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;
    attr.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    UA_Server_addVariableNode(m_server->server(), 
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
    
    CnDeviceBaseItem *item;

#define INIT_STAT(name)    item = new CnDeviceStatItem##name(this); item->setName(CnSTR(#name)); addItemExtInner(item);

INIT_STAT(State                     )
INIT_STAT(StateEnableDevice         )
INIT_STAT(StateConnectionAlive      )
INIT_STAT(StateIncluded             )
INIT_STAT(CommonGoodCount           )
INIT_STAT(CommonBadCount            )
INIT_STAT(CommonStatus              )
INIT_STAT(CommonTimestamp           )
INIT_STAT(CommonLastSuccessTimestamp)
INIT_STAT(CommonLastErrorStatus     )
INIT_STAT(CommonLastErrorTimestamp  )
INIT_STAT(CommonLastErrorText       )
INIT_STAT(PokeGoodCount             )
INIT_STAT(PokeBadCount              )
INIT_STAT(PokeStatus                )
INIT_STAT(PokeTimestamp             )
INIT_STAT(PokeLastSuccessTimestamp  )
INIT_STAT(PokeLastErrorStatus       )
INIT_STAT(PokeLastErrorTimestamp    )
INIT_STAT(PokeLastErrorText         )
INIT_STAT(PollGoodCount             )
INIT_STAT(PollBadCount              )
INIT_STAT(PollStatus                )
INIT_STAT(PollTimestamp             )
INIT_STAT(PollLastSuccessTimestamp  )
INIT_STAT(PollLastErrorStatus       )
INIT_STAT(PollLastErrorTimestamp    )
INIT_STAT(PollLastErrorText         )
INIT_STAT(InnerState                )
}

void CnUaDevice::innerClearUaNode()
{
    uaPollMessages()->finalizeUa();
    uaItems()->finalizeUa();
    for (CnDeviceBaseItem *item : m_itemsExt)
        innerClearUaItem(item);
    UA_Server_deleteNode(m_server->server(), m_nodeId, true);
}

void CnUaDevice::addItemExtInner(CnDeviceBaseItem *item)
{
    m_itemsExt.push_back(item);
    innerAddUaItem(item);
}

void CnUaDevice::innerAddUaItem(CnDeviceBaseItem *item)
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
    UA_DataSource callback;
    CnUa::NodeId nodeIdParent;

    switch (item->itemType())
    {
    case CnDeviceBaseItem::Item_Config:
    {
        nodeIdParent = m_nodeIdCfg;
        sId = Cn::toStdString(nodeName())+".Cfg."+sname;
        attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;
        callback.read  = CnUaDevice::readSimpleItem;
        callback.write = CnUa::writeDummy;
    }
        break;
    case CnDeviceBaseItem::Item_Statistic:
    {
        nodeIdParent = m_nodeIdStat;
        sId = Cn::toStdString(nodeName())+".Stat."+sname;
        attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;
        callback.read  = CnUaDevice::readSimpleItem;
        callback.write = CnUa::writeDummy;
    }
        break;
    case CnDeviceBaseItem::Item_Setting:
    {
        nodeIdParent = m_nodeIdSet;
        sId = Cn::toStdString(nodeName())+".Set."+sname;
        attrVar.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        callback.read  = CnUaDevice::readSimpleItem;
        callback.write = CnUaDevice::writeSimpleItem;
    }
        break;
    case CnDeviceBaseItem::Item_Command:
    {
        nodeIdParent = m_nodeIdCmd;
        sId = Cn::toStdString(nodeName())+".Cmd."+sname;
        attrVar.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        callback.read  = CnUa::readDummyBool;
        callback.write = CnUaDevice::writeSimpleItem;
    }
        break;
    default:
        return; // UA_NODEID_NULL;
    }

    CnUa::NodeId itemId = UA_NODEID_STRING(m_server->nsIdx(), const_cast<char*>(sId.data()));
    UA_Server_addVariableNode(m_server->server(), 
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

void CnUaDevice::innerClearUaItem(CnDeviceBaseItem *item)
{
    CnUa::NodeId nodeId = CnUa::getNodeId(item);
    if (!nodeId.isNull())
    {
        UA_Server_deleteNode(m_server->server(), nodeId, true);
        CnUa::clearNodeId(item);
    }
}