#include "CnUaDeviceMessageList.h"

#include "CnUaDeviceMessage.h"

#include <device/CnUaDevice.h>

// ------------------------------------------------------------------------------------------
// ------------------------------ CnUaDevicePokeMessageList ------------------------------
// ------------------------------------------------------------------------------------------

CnUaDevicePokeMessageList::CnUaDevicePokeMessageList(CnUaDevice *device, CnUaServer *server, const CnString &name) : CnDevicePokeMessageList(device),
    m_server(server),
    m_name(name)
{
}

void CnUaDevicePokeMessageList::initializeUa(const CnUa::NodeId &/*parentId*/, const CnUa::NodeId &/*refTypeId*/)
{
}

void CnUaDevicePokeMessageList::finalizeUa()
{
}

void CnUaDevicePokeMessageList::addMessageInner(CnDeviceMessageBase *m)
{
    CnDevicePokeMessageList::addMessageInner(m);
}

CnDeviceMessageBase *CnUaDevicePokeMessageList::createMessage(CnDeviceModbusItem *item)
{
    CnDeviceMessageBase *m = CnDevicePokeMessageList::createMessage(item);
    if (m)
        return new CnUaDevicePokeMessage(static_cast<CnDeviceMessage*>(m));
    return nullptr;
}

// ------------------------------------------------------------------------------------------
// ------------------------------ CnUaDevicePollMessageList ------------------------------
// ------------------------------------------------------------------------------------------

CnUaDevicePollMessageList::CnUaDevicePollMessageList(CnUaDevice *device, CnUaServer *server, const CnString &name) : CnDevicePollMessageList(device),
    m_server(server),
    m_name(name)
{
}

void CnUaDevicePollMessageList::initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId)
{
    CnCriticalSectionLocker _(&m_cs);
    m_nodeIdParent = parentId;
    m_nodeIdParentRef = refTypeId;
    innerInitUaNode();
}

void CnUaDevicePollMessageList::finalizeUa()
{
    CnCriticalSectionLocker _(&m_cs);
    innerClearUaNode();
    CnDevicePollMessageList::clearInner();
}

void CnUaDevicePollMessageList::addMessageInner(CnDeviceMessageBase *m)
{
    static_cast<CnUaDeviceMessage*>(m)->initializeUa(m_nodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
    CnDevicePollMessageList::addMessageInner(m);
}

CnDeviceMessageBase *CnUaDevicePollMessageList::createMessage(CnDeviceModbusItem *item)
{
    CnDeviceMessageBase *m = CnDevicePollMessageList::createMessage(item);
    if (m)
        return new CnUaDevicePollMessage(static_cast<CnDeviceMessage*>(m));
    return nullptr;
}

void CnUaDevicePollMessageList::innerInitUaNode()
{
    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);

    std::string sname = Cn::toStdString(m_name);
    attr.displayName = UA_LOCALIZEDTEXT(const_cast<char*>(""), const_cast<char*>(sname.data()));
    UA_Server_addObjectNode(m_server->server(), 
                            dummy, 
                            m_nodeIdParent,
                            m_nodeIdParentRef, 
                            UA_QUALIFIEDNAME(m_server->nsIdx(), const_cast<char*>(sname.data())),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), 
                            attr, 
                            NULL, 
                            m_nodeId.data());
}

void CnUaDevicePollMessageList::innerClearUaNode()
{
    UA_Server_deleteNode(m_server->server(), m_nodeId, true);
}
