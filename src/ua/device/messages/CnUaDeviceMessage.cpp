#include "CnUaDeviceMessage.h"

#include <sstream>
#include <iomanip>

#include <device/items/CnDeviceModbusItem.h>

#include <CnUaServer.h>
#include <app/CnUaApp.h>
#include <device/CnUaDevice.h>

CnUaDeviceMessage::CnUaDeviceMessage(CnDeviceMessage *innerMessage) : 
    m_innerMessage(innerMessage)
{
    m_server = CnUaApp::globalUa()->server();
    m_innerMessage->connect(&CnDeviceMessage::signalRemovingItem, this, &CnUaDeviceMessage::innerClearUaItem);
}

CnUaDeviceMessage::~CnUaDeviceMessage()
{
    delete m_innerMessage;
}

void CnUaDeviceMessage::initialize()
{
    m_innerMessage->initialize();
}

void CnUaDeviceMessage::finalize()
{
    m_innerMessage->finalize();
}

const CnChar* CnUaDeviceMessage::fineName() const
{
    return m_innerMessage->fineName();
}

uint16_t CnUaDeviceMessage::offset() const
{
    return m_innerMessage->offset();
}

uint16_t CnUaDeviceMessage::count() const
{
    return m_innerMessage->count();
}

uint16_t CnUaDeviceMessage::execOffset() const
{
    return m_innerMessage->execOffset();
}

uint16_t CnUaDeviceMessage::execCount() const
{
    return m_innerMessage->execCount();
}

int CnUaDeviceMessage::period() const
{
    return m_innerMessage->period();
}

void* CnUaDeviceMessage::innerBuffer()
{
    return m_innerMessage->innerBuffer();
}

uint32_t CnUaDeviceMessage::innerBufferSize() const
{
    return m_innerMessage->innerBufferSize();
}

Cn::StatusCode CnUaDeviceMessage::status() const
{
    return m_innerMessage->status();
}

CnTimestamp CnUaDeviceMessage::timestamp() const
{
    return m_innerMessage->timestamp();
}

CnTimestamp CnUaDeviceMessage::timestampBegin() const
{
    return m_innerMessage->timestampBegin();
}

void CnUaDeviceMessage::beginProcess()
{
    m_innerMessage->beginProcess();
}

bool CnUaDeviceMessage::addItem(CnDeviceModbusItem* item)
{
    return m_innerMessage->addItem(item);
}

void CnUaDeviceMessage::setComplete(Modbus::StatusCode status, CnTimestamp timestamp, const CnString &err)
{
    // Note: make lock because of possible call of signalRemovingItem in subclass to prevent interlock
    CnCriticalSectionLocker _(&m_cs);
    m_innerMessage->setComplete(status, timestamp, err);
}

Modbus::MemoryType CnUaDeviceMessage::memoryType() const
{
    return m_innerMessage->memoryType();
}

int CnUaDeviceMessage::ModbusFunction() const
{
    return m_innerMessage->ModbusFunction();
}

bool CnUaDeviceMessage::getData(uint16_t innerOffset, uint16_t count, void* buff) const
{
    return m_innerMessage->getData(innerOffset, count, buff);
}

bool CnUaDeviceMessage::setData(uint16_t innerOffset, uint16_t count, const void* buff)
{
    return m_innerMessage->setData(innerOffset, count, buff);
}

uint32_t CnUaDeviceMessage::itemCount() const
{
    return m_innerMessage->itemCount();
}

void CnUaDeviceMessage::clearItems()
{
    return m_innerMessage->clearItems();
}

void CnUaDeviceMessage::initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId)
{
    CnCriticalSectionLocker _(&m_cs);
    m_nodeIdParent = parentId;
    m_nodeIdParentRef = refTypeId;
}

void CnUaDeviceMessage::innerInitUaNode()
{
    char locale[] = { '\0' };
    char namebuff[CN_UA_MAX_NAME];

    // Message root node
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << reinterpret_cast<uintptr_t>(this);
    std::string sname = ss.str();

    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);
    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT(locale, const_cast<char*>(sname.data()));
    UA_Server_addObjectNode(m_server->server(), 
                            dummy, 
                            m_nodeIdParent,
                            m_nodeIdParentRef, 
                            UA_QUALIFIEDNAME(m_server->nsIdx(), const_cast<char*>(sname.data())),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), 
                            attr, 
                            m_innerMessage, 
                            m_nodeId.data());

    //---------------------------------------------------
    // Message Items node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Items");
    attr.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    UA_Server_addObjectNode(m_server->server(), 
                            dummy, 
                            m_nodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                            UA_QUALIFIEDNAME(m_server->nsIdx(), namebuff),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), 
                            attr, 
                            NULL, 
                            m_nodeIdItems.data());

    //---------------------------------------------------
    // Message Stat node
    std::snprintf(namebuff, CN_UA_MAX_NAME, "%s", "Stat");
    UA_VariableAttributes attrVar = UA_VariableAttributes_default;
    attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;
    attrVar.displayName = UA_LOCALIZEDTEXT(locale, namebuff);
    UA_Server_addVariableNode(m_server->server(), 
                              dummy, 
                              m_nodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                              UA_QUALIFIEDNAME(m_server->nsIdx(), namebuff),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), 
                              attrVar, 
                              NULL, 
                              m_nodeIdStat.data());

    UA_DataSource callback;
    callback.write = CnUa::writeDummy;
    UA_NodeId itemId;

#define MSG_STAT(name) \
    sname = #name; \
    attrVar.displayName = UA_LOCALIZEDTEXT(locale, const_cast<char*>(sname.data())); \
    attrVar.dataType = UA_TYPES[CnUa::toUaTypesIndex(Cn::toDataType<decltype(getMethodRT(&CnDeviceMessage::Stat##name))>())].typeId; \
    itemId = UA_NODEID_NUMERIC(m_server->nsIdx(), 0); \
    UA_Server_addVariableNode(m_server->server(), \
                              dummy, \
                              m_nodeIdStat, \
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), \
                              UA_QUALIFIEDNAME(m_server->nsIdx(), const_cast<char*>(sname.data())), \
                              UA_NODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE), \
                              attrVar, \
                              m_innerMessage, \
                              &itemId); \
    callback.read  = CnUaDeviceMessage::readStat<decltype(getMethodRT(&CnDeviceMessage::Stat##name)), &CnDeviceMessage::Stat##name>; \
    UA_Server_setVariableNode_dataSource(m_server->server(), \
                                         itemId, \
                                         callback);

    MSG_STAT(Range               );
    MSG_STAT(GoodCount           );
    MSG_STAT(BadCount            );
    MSG_STAT(Status              );
    MSG_STAT(Timestamp           );
    MSG_STAT(LastSuccessTimestamp);
    MSG_STAT(LastErrorStatus     );
    MSG_STAT(LastErrorTimestamp  );
    MSG_STAT(LastErrorText       );
}

void CnUaDeviceMessage::innerClearUaNode()
{
    for (CnDeviceModbusItem *mi : m_innerMessage->items())
        innerClearUaItem(mi);
    UA_Server_deleteNode(CnUaApp::globalUa()->server()->server(), m_nodeId, true);
}

void CnUaDeviceMessage::innerInitUaItem(CnDeviceModbusItem *mi)
{
    CnUa::NodeId id = CnUa::getNodeId(mi);
    UA_ExpandedNodeId eid = id.toExpandedNodeId(); 
    UA_StatusCode s = UA_Server_addReference(CnUaApp::globalUa()->server()->server(),
                                             m_nodeIdItems,
                                             UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                             eid,
                                             true);
    s = s;                                         
}

void CnUaDeviceMessage::innerClearUaItem(CnDeviceModbusItem *mi)
{
    CnUa::NodeId id = CnUa::getNodeId(mi);
    if (id.isNull())
        return;
    UA_ExpandedNodeId eid = id.toExpandedNodeId();
    UA_Server_deleteReference(CnUaApp::globalUa()->server()->server(),
                              m_nodeIdItems,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              true,
                              eid,
                              true);
}

// ------------------------------------------------------------------------------------------
// ------------------------------ CnUaDevicePollMessage ------------------------------
// ------------------------------------------------------------------------------------------
CnUaDevicePollMessage::CnUaDevicePollMessage(CnDeviceMessage *innerMessage) :
    CnUaDeviceMessage(innerMessage)
{
}

CnUaDevicePollMessage::~CnUaDevicePollMessage()
{
}

void CnUaDevicePollMessage::initialize()
{
    CnCriticalSectionLocker _(&m_cs);
    m_innerMessage->initialize();
    innerInitUaNode();
    for (CnDeviceModbusItem *mi : m_innerMessage->items())
        innerInitUaItem(mi);
}

void CnUaDevicePollMessage::finalize()
{
    CnCriticalSectionLocker _(&m_cs);
    // Note: No need to delete items because it is a responsibility of CnUaDevice class
    innerClearUaNode();
    CnUaDeviceMessage::finalize();
}

bool CnUaDevicePollMessage::addItem(CnDeviceModbusItem * mi)
{
    CnCriticalSectionLocker _(&m_cs); 
    if (m_innerMessage->addItem(mi))
    {
        innerInitUaItem(mi);
        return true;
    }
    return false;
}

// ------------------------------------------------------------------------------------------
// ------------------------------- CnUaDevicePokeMessage -------------------------------
// ------------------------------------------------------------------------------------------
CnUaDevicePokeMessage::CnUaDevicePokeMessage(CnDeviceMessage *innerMessage) :
    CnUaDeviceMessage(innerMessage)
{
}

CnUaDevicePokeMessage::~CnUaDevicePokeMessage()
{
}
