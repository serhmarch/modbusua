/*!
 * \file   CnUaCore.h
 * \brief  
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNUACORE_H
#define CNUACORE_H

#include <codecvt>

#include <open62541/types.h>
#include <open62541/server.h>
#include <CnGlobal.h>

#define CN_UA_VERSION_STR CN_VERSION_STR_MAKE(UA_OPEN62541_VER_MAJOR,UA_OPEN62541_VER_MINOR,UA_OPEN62541_VER_PATCH)

#define CN_UA_TCP_PORT 4840

#define CN_UA_NAMESPACE CN_APP_NAME".serhmarch.github.com"

#define CN_UA_MAX_NAME 4096

class CnBaseItem;

namespace CnUa {

UA_StatusCode readDummyBool(UA_Server *server,
                            const UA_NodeId *sessionId,
                            void *sessionContext,
                            const UA_NodeId *nodeid,
                            void *nodeContext,
                            UA_Boolean sourceTimeStamp,
                            const UA_NumericRange *range,
                            UA_DataValue *data);

UA_StatusCode writeDummy(UA_Server *server,
                         const UA_NodeId *sessionId,
                         void *sessionContext,
                         const UA_NodeId *nodeId,
                         void *nodeContext,
                         const UA_NumericRange *range,
                         const UA_DataValue *data);

class NodeId
{
public:
    NodeId();
    NodeId(const UA_NodeId &uaNodeId);
    NodeId(UA_UInt16 nsIndex, UA_UInt32 identifier);
    NodeId(UA_UInt16 nsIndex, const char *chars);
    NodeId(const NodeId &other);
    NodeId(NodeId &&other);
    ~NodeId();

public:
    NodeId& operator=(const UA_NodeId &uaNodeId);
    NodeId& operator=(const NodeId &uaNodeId);
    NodeId& operator=(NodeId &&uaNodeId);
    inline operator UA_NodeId() const { return m_uaNodeId; }

public:
    inline UA_NodeIdType type() const { return m_uaNodeId.identifierType; }
    inline const UA_NodeId *constData() const { return &m_uaNodeId;}
    inline const UA_NodeId *data() const { return constData();}
    inline UA_NodeId *data() { return &m_uaNodeId;}
    inline bool isNull() const { return UA_NodeId_isNull(&m_uaNodeId); }
    inline UA_UInt16 namespaceIndex() const { return m_uaNodeId.namespaceIndex; }
    inline UA_UInt32 number() const { return m_uaNodeId.identifier.numeric; }
    inline UA_UInt32 hash() const { return UA_NodeId_hash(&m_uaNodeId); }
    inline void clear() {  UA_NodeId_clear(&m_uaNodeId); }

public:
    UA_ExpandedNodeId toExpandedNodeId() const;
    
private:
    UA_NodeId m_uaNodeId;
};

NodeId getNodeId(CnBaseItem *item);
void setNodeId(CnBaseItem *item, const NodeId &nodeId);
void clearNodeId(CnBaseItem *item);

UA_DateTime toUA_DateTime(CnTimestamp v);

UA_Variant toUA_Variant(bool v);
UA_Variant toUA_Variant(int8_t v);
UA_Variant toUA_Variant(uint8_t v);
UA_Variant toUA_Variant(int16_t v);
UA_Variant toUA_Variant(uint16_t v);
UA_Variant toUA_Variant(int32_t v);
UA_Variant toUA_Variant(uint32_t v);
UA_Variant toUA_Variant(int64_t v);
UA_Variant toUA_Variant(uint64_t v);
UA_Variant toUA_Variant(float v);
UA_Variant toUA_Variant(double v);
UA_Variant toUA_Variant(const CnString &v);
UA_Variant toUA_Variant(CnTimestamp v);
inline UA_Variant toUA_Variant(const CnDateTime &v) { return toUA_Variant(v.toTimestamp()); }
UA_Variant toUA_Variant(const CnVariant &v);

int toUaTypesIndex(Cn::DataType dataType);

CnVariant toVariant(const UA_Variant &v);

//UA_StatusCode toUA_StatusCode(Cn::StatusCode status);

} // namespace CnUa

#endif // CNUACORE_H
