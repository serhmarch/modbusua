#include "CnUaCore.h"

#include <device/items/CnDeviceModbusItem.h>

namespace CnUa {

UA_StatusCode readDummyBool(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext, const UA_NodeId *nodeid, void *nodeContext, UA_Boolean sourceTimeStamp, const UA_NumericRange *range, UA_DataValue *data)
{
    bool v = false;
    UA_Variant_setScalarCopy(&data->value, &v, &UA_TYPES[UA_TYPES_BOOLEAN]);
    data->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode writeDummy(UA_Server *, const UA_NodeId *, void *, const UA_NodeId *, void *, const UA_NumericRange *, const UA_DataValue *)
{
    return UA_STATUSCODE_GOOD;
}

NodeId::NodeId()
{
    m_uaNodeId = UA_NODEID_NULL;
}

NodeId::NodeId(const UA_NodeId &uaNodeId)
{
    UA_NodeId_copy(&uaNodeId, &m_uaNodeId);
}

NodeId::NodeId(UA_UInt16 nsIndex, UA_UInt32 identifier) :
    m_uaNodeId(UA_NODEID_NUMERIC(nsIndex, identifier))
{
}

NodeId::NodeId(UA_UInt16 nsIndex, const char *chars) :
    m_uaNodeId(UA_NODEID_STRING_ALLOC(nsIndex, chars))
{
}

NodeId::NodeId(const NodeId &other)
{
    UA_NodeId_copy(&other.m_uaNodeId, &m_uaNodeId);
}

NodeId::NodeId(NodeId &&other)
{
    m_uaNodeId = other.m_uaNodeId;
    other.m_uaNodeId.identifierType = UA_NODEIDTYPE_NUMERIC; // Note: To prevent deletion
}

NodeId::~NodeId()
{
    clear();
}

NodeId &NodeId::operator=(const UA_NodeId &uaNodeId)
{
    if (&this->m_uaNodeId != &uaNodeId)
    {
        clear();
        UA_NodeId_copy(&uaNodeId, &m_uaNodeId);
    }
    return *this;
}

NodeId &NodeId::operator=(const NodeId &other)
{
    if (this != &other)
    {
        clear();
        UA_NodeId_copy(&other.m_uaNodeId, &m_uaNodeId);
    }
    return *this;
}

NodeId &NodeId::operator=(NodeId &&other)
{
    if (&this->m_uaNodeId != &other.m_uaNodeId)
    {
        clear();
        m_uaNodeId = other.m_uaNodeId;
        other.m_uaNodeId.identifierType = UA_NODEIDTYPE_NUMERIC; // Note: To prevent deletion
    }
    return *this;
}

UA_ExpandedNodeId NodeId::toExpandedNodeId() const
{
    UA_ExpandedNodeId id;
    id.nodeId = m_uaNodeId;
    id.serverIndex = 0;
    id.namespaceUri = UA_STRING_NULL;
    return id;
}

NodeId getNodeId(CnBaseItem *item)
{
    if (item->context())
        return *reinterpret_cast<NodeId*>(item->context());
    return NodeId();
}

void setNodeId(CnBaseItem *item, const NodeId &nodeId)
{
    if (item->context())
        *reinterpret_cast<NodeId*>(item->context()) = nodeId;
    else
        item->setContext(new NodeId(nodeId));
}

void clearNodeId(CnBaseItem *item)
{
    void *c = item->context();
    if (c)
    {
        delete reinterpret_cast<NodeId*>(c);
        item->setContext(nullptr);
    }
}

UA_DateTime toUA_DateTime(CnTimestamp v)
{
    return v.rawValue();
}

UA_Variant toUA_Variant(bool v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_BOOLEAN]);
    return res;
}

UA_Variant toUA_Variant(int8_t v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_SBYTE]);
    return res;
}

UA_Variant toUA_Variant(uint8_t v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_BYTE]);
    return res;
}

UA_Variant toUA_Variant(int16_t v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_INT16]);
    return res;
}

UA_Variant toUA_Variant(uint16_t v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_UINT16]);
    return res;
}

UA_Variant toUA_Variant(int32_t v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_INT32]);
    return res;
}

UA_Variant toUA_Variant(uint32_t v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_UINT32]);
    return res;
}

UA_Variant toUA_Variant(int64_t v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_INT64]);
    return res;
}

UA_Variant toUA_Variant(uint64_t v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_UINT64]);
    return res;
}

UA_Variant toUA_Variant(float v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_FLOAT]);
    return res;
}

UA_Variant toUA_Variant(double v)
{
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &v, &UA_TYPES[UA_TYPES_DOUBLE]);
    return res;
}

UA_Variant toUA_Variant(const CnString &v)
{
    std::string ses = Cn::toStdString(v);
    UA_String uaString = UA_STRING(const_cast<char*>(ses.data())); // TODO:
    UA_Variant res;
    UA_Variant_setScalarCopy(&res, &uaString, &UA_TYPES[UA_TYPES_STRING]);
    return res;
}

UA_Variant toUA_Variant(CnTimestamp v)
{
    UA_Variant res;
    UA_DateTime time = static_cast<UA_DateTime>(v.rawValue());
    UA_Variant_setScalarCopy(&res, &time, &UA_TYPES[UA_TYPES_DATETIME]);
    return res;
}

UA_Variant toUA_Variant(const CnVariant &v)
{
    switch (v.type())
    {
    case CnVariant::vBool      : return toUA_Variant(v.toBool     ());
    case CnVariant::vInt8      : return toUA_Variant(v.toInt8     ());
    case CnVariant::vUInt8     : return toUA_Variant(v.toUInt8    ());
    case CnVariant::vInt16     : return toUA_Variant(v.toInt16    ());
    case CnVariant::vUInt16    : return toUA_Variant(v.toUInt16   ());
    case CnVariant::vInt32     : return toUA_Variant(v.toInt32    ());
    case CnVariant::vUInt32    : return toUA_Variant(v.toUInt32   ());
    case CnVariant::vInt64     : return toUA_Variant(v.toInt64    ());
    case CnVariant::vUInt64    : return toUA_Variant(v.toUInt64   ());
    case CnVariant::vFloat     : return toUA_Variant(v.toFloat    ());
    case CnVariant::vDouble    : return toUA_Variant(v.toDouble   ());
    case CnVariant::vString    : return toUA_Variant(v.toString   ());
    case CnVariant::vTimestamp : return toUA_Variant(v.toTimestamp());
    case CnVariant::vDateTime  : return toUA_Variant(v.toDateTime ());
    default:
    {
        UA_Variant res;
        UA_Variant_init(&res);
        return res;
    }
    }
}

int toUaTypesIndex(Cn::DataType dataType)
{
    switch (dataType)
    {
    case Cn::Data_Bool        : return UA_TYPES_BOOLEAN ;
    case Cn::Data_Int8        : return UA_TYPES_SBYTE   ;        
    case Cn::Data_UInt8       : return UA_TYPES_BYTE    ;
    case Cn::Data_Int16       : return UA_TYPES_INT16   ;
    case Cn::Data_UInt16      : return UA_TYPES_UINT16  ;
    case Cn::Data_Int32       : return UA_TYPES_INT32   ;
    case Cn::Data_UInt32      : return UA_TYPES_UINT32  ;
    case Cn::Data_Int64       : return UA_TYPES_INT64   ;
    case Cn::Data_UInt64      : return UA_TYPES_UINT64  ;
    case Cn::Data_Int         : return UA_TYPES_INT32   ;
    case Cn::Data_UInt        : return UA_TYPES_UINT32  ;
    case Cn::Data_Float       : return UA_TYPES_FLOAT   ;
    case Cn::Data_Double      : return UA_TYPES_DOUBLE  ;
    case Cn::Data_String      : return UA_TYPES_STRING  ;
    case Cn::Data_Timestamp   : return UA_TYPES_DATETIME;
    case Cn::Data_DateTime    : return UA_TYPES_DATETIME;
    case Cn::Data_Variant     : return UA_TYPES_VARIANT ;
    case Cn::Data_ModbusStatus: return UA_TYPES_INT32   ;
    case Cn::Data_CnStatus : return UA_TYPES_INT32   ;
    default                   : return UA_TYPES_VARIANT ;    
    }
}

CnVariant toVariant(const UA_Variant &v)
{
    if (UA_Variant_isEmpty(&v))
        return CnVariant();
    const UA_DataType *type = v.type;
    // Handle scalar types
    if (UA_Variant_isScalar(&v)) 
    {
        if (type == &UA_TYPES[UA_TYPES_BOOLEAN])
            return CnVariant(*static_cast<const UA_Boolean *>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_SBYTE])
            return CnVariant(*static_cast<const UA_SByte*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_BYTE])
            return CnVariant(*static_cast<const UA_Byte*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_INT16])
            return CnVariant(*static_cast<const UA_Int16*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_UINT16])
            return CnVariant(*static_cast<const UA_UInt16*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_INT32])
            return CnVariant(*static_cast<const UA_Int32*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_UINT32])
            return CnVariant(*static_cast<const UA_UInt32*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_INT64])
            return CnVariant(*static_cast<const UA_Int64*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_UINT64])
            return CnVariant(*static_cast<const UA_UInt64*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_FLOAT])
            return CnVariant(*static_cast<const UA_Float*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_DOUBLE])
            return CnVariant(*static_cast<const UA_Double*>(v.data));
        else if (type == &UA_TYPES[UA_TYPES_STRING]) 
        {
            const UA_String *uaString = static_cast<const UA_String *>(v.data);
            return Cn::toString(reinterpret_cast<const char *>(uaString->data), uaString->length);
        }
        else if (type == &UA_TYPES[UA_TYPES_DATETIME]) 
        {
            UA_DateTime uaDateTime = *static_cast<const UA_DateTime *>(v.data);
            return CnTimestamp::fromRawValue(uaDateTime);
        }
        // Add more types as needed
    }
    return CnVariant();
}

//UA_StatusCode toUA_StatusCode(Cn::StatusCode status)
//{
//    if (Cn::StatusIsGood(status))
//        return UA_STATUSCODE_GOOD;
//    if (Cn::StatusIsBad(status))
//        return UA_STATUSCODE_BAD;
//    // Note: Considering Uncertain as Good
//    return UA_STATUSCODE_UNCERTAIN;
//}

} // namespace CnUa