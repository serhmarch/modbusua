#include "CnUaDeviceItemList.h"

#include <device/items/CnDeviceModbusItem.h>
#include <device/CnUaDevice.h>

CnUaDeviceItemList::CnUaDeviceItemList(CnUaServer *server, const CnString &deviceName, const CnString &name) : CnDeviceItemList(),
    m_server(server),
    m_deviceName(deviceName),
    m_name(name)
{
}

CnUaDeviceItemList::~CnUaDeviceItemList()
{
    // Note: In case if items was not clear by `finalizeUa()` method
    for (CnDeviceBaseItem *item : m_list)
    {
        innerClearUaItem(item);
        delete item;
    }
    m_list.clear();
}

void CnUaDeviceItemList::initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId)
{
    CnCriticalSectionLocker _(&m_cs);
    m_nodeIdParent = parentId;
    m_nodeIdParentRef = refTypeId;
    innerInitUaNode();
    for (CnDeviceBaseItem *mi : m_list)
        innerAddUaItem(mi);
}

void CnUaDeviceItemList::finalizeUa()
{
    CnCriticalSectionLocker _(&m_cs);
    innerClearUaNode();
    clearInner();
}

void CnUaDeviceItemList::addItemInner(CnDeviceBaseItem *item)
{
    CnDeviceItemList::addItemInner(item);
    if (!m_nodeId.isNull())
        innerAddUaItem(item);
}

void CnUaDeviceItemList::removeItemInner(CnDeviceBaseItem *item)
{
    // Node: Need to clear UA_NodeId from item context before deleting item
    innerClearUaItem(item);
    CnDeviceItemList::removeItemInner(item);
}

void CnUaDeviceItemList::innerInitUaNode()
{
    char locale[] = { '\0' };

    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    UA_NodeId dummy = UA_NODEID_NUMERIC(m_server->nsIdx(),0);

    std::string sname = Cn::toStdString(m_name);
    attr.displayName = UA_LOCALIZEDTEXT(locale, const_cast<char*>(sname.data()));
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

void CnUaDeviceItemList::innerClearUaNode()
{
    // Note: No need to clear item context (delete NodeId pointer data)
    //       because it will be done indirectly in clearInner() method
    //for (CnDeviceBaseItem *item : m_list)
    //    innerClearUaItem(item);
    UA_Server_deleteNode(m_server->server(), m_nodeId, true);
}

void CnUaDeviceItemList::innerAddUaItem(CnDeviceBaseItem *item)
{
    char locale[] = { '\0' };

    std::string sname = Cn::toStdString(item->name());
    std::string sId = Cn::toStdString(m_deviceName)+".Items."+sname;

    int tpIndex = CnUa::toUaTypesIndex(item->dataType());

    UA_VariableAttributes attrVar = UA_VariableAttributes_default;
    UA_Variant_init(&attrVar.value);
    attrVar.displayName = UA_LOCALIZEDTEXT(locale, const_cast<char*>(sname.data()));
    std::string sDescr = Cn::toStdString(item->description());
    attrVar.description = UA_LOCALIZEDTEXT(locale, const_cast<char*>(sDescr.data()));
    attrVar.dataType = UA_TYPES[tpIndex].typeId;
    UA_DataSource callback;

    switch (item->itemType())
    {
    case CnDeviceBaseItem::Item_Modbus:
    {
        CnDeviceModbusItem *mi = static_cast<CnDeviceModbusItem*>(item);
        callback.read  = CnUaDevice::readModbusItem;
        if (mi->access() == Cn::Access_ReadWrite)
        {
            attrVar.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            callback.write = CnUaDevice::writeModbusItem;
        }
        else
        {
            attrVar.accessLevel = UA_ACCESSLEVELMASK_READ;
            callback.write = CnUa::writeDummy;
        }
    }
        break;
    default:
        return; // UA_NODEID_NULL;
    }

    CnUa::NodeId itemId = UA_NODEID_STRING(m_server->nsIdx(), const_cast<char*>(sId.data()));
    UA_Server_addVariableNode(m_server->server(), 
                              itemId, 
                              m_nodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), 
                              UA_QUALIFIEDNAME(m_server->nsIdx(), const_cast<char*>(sname.data())),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE), 
                              attrVar,
                              item, // Set Node Context to current item
                              nullptr);

    UA_Server_setVariableNode_dataSource(m_server->server(),
                                         itemId,
                                         callback);

    // Note: Set NodeId to item context
    CnUa::setNodeId(item, itemId);
}

void CnUaDeviceItemList::innerClearUaItem(CnDeviceBaseItem *item)
{
    CnUa::NodeId nodeId = CnUa::getNodeId(item);
    if (!nodeId.isNull())
    {
        UA_Server_deleteNode(m_server->server(), nodeId, true);
        // Note: Delete NodeId item context data pointer and set context to nullptr
        CnUa::clearNodeId(item);     
    }
}
