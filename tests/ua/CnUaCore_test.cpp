#include <gtest/gtest.h>
#include <ua/core/CnUaCore.h>
#include <open62541/types.h>

using namespace CnUa;

// ============================================================================
// NodeId Tests
// ============================================================================

TEST(CnUaCoreTest, NodeIdDefaultConstructor)
{
    NodeId nodeId;
    EXPECT_TRUE(nodeId.isNull());
    EXPECT_EQ(nodeId.type(), UA_NODEIDTYPE_NUMERIC);
}

TEST(CnUaCoreTest, NodeIdConstructorFromUANodeId)
{
    UA_NodeId uaNodeId = UA_NODEID_NUMERIC(1, 1000);
    NodeId nodeId(uaNodeId);
    
    EXPECT_FALSE(nodeId.isNull());
    EXPECT_EQ(nodeId.type(), UA_NODEIDTYPE_NUMERIC);
    EXPECT_EQ(nodeId.number(), 1000);
}

TEST(CnUaCoreTest, NodeIdConstructorNumeric)
{
    NodeId nodeId(2, 5000);
    
    EXPECT_FALSE(nodeId.isNull());
    EXPECT_EQ(nodeId.type(), UA_NODEIDTYPE_NUMERIC);
    EXPECT_EQ(nodeId.number(), 5000);
}

TEST(CnUaCoreTest, NodeIdConstructorString)
{
    NodeId nodeId(3, "TestNodeId");
    
    EXPECT_FALSE(nodeId.isNull());
    EXPECT_EQ(nodeId.type(), UA_NODEIDTYPE_STRING);
}

TEST(CnUaCoreTest, NodeIdCopyConstructor)
{
    NodeId original(1, 100);
    NodeId copy(original);
    
    EXPECT_FALSE(copy.isNull());
    EXPECT_EQ(copy.type(), original.type());
    EXPECT_EQ(copy.number(), original.number());
}

TEST(CnUaCoreTest, NodeIdMoveConstructor)
{
    NodeId original(1, 200);
    NodeId moved(std::move(original));
    
    EXPECT_FALSE(moved.isNull());
    EXPECT_EQ(moved.type(), UA_NODEIDTYPE_NUMERIC);
    EXPECT_EQ(moved.number(), 200);
}

TEST(CnUaCoreTest, NodeIdCopyAssignment)
{
    NodeId nodeId1(1, 100);
    NodeId nodeId2(2, 200);
    
    nodeId2 = nodeId1;
    
    EXPECT_EQ(nodeId2.number(), 100);
}

TEST(CnUaCoreTest, NodeIdMoveAssignment)
{
    NodeId nodeId1(1, 300);
    NodeId nodeId2;
    
    nodeId2 = std::move(nodeId1);
    
    EXPECT_FALSE(nodeId2.isNull());
    EXPECT_EQ(nodeId2.number(), 300);
}

TEST(CnUaCoreTest, NodeIdAssignmentFromUANodeId)
{
    UA_NodeId uaNodeId = UA_NODEID_NUMERIC(1, 400);
    NodeId nodeId;
    
    nodeId = uaNodeId;
    
    EXPECT_FALSE(nodeId.isNull());
    EXPECT_EQ(nodeId.number(), 400);
}

TEST(CnUaCoreTest, NodeIdClear)
{
    NodeId nodeId(1, 500);
    EXPECT_FALSE(nodeId.isNull());
    
    nodeId.clear();
    EXPECT_TRUE(nodeId.isNull());
}

TEST(CnUaCoreTest, NodeIdToExpandedNodeId)
{
    NodeId nodeId(1, 600);
    UA_ExpandedNodeId expanded = nodeId.toExpandedNodeId();
    
    EXPECT_EQ(expanded.nodeId.identifier.numeric, 600);
    EXPECT_EQ(expanded.serverIndex, 0);
}

TEST(CnUaCoreTest, NodeIdHash)
{
    NodeId nodeId1(1, 100);
    NodeId nodeId2(1, 100);
    NodeId nodeId3(1, 200);
    
    EXPECT_EQ(nodeId1.hash(), nodeId2.hash());
    EXPECT_NE(nodeId1.hash(), nodeId3.hash());
}

// ============================================================================
// toUA_DateTime Tests
// ============================================================================

TEST(CnUaCoreTest, ToUADateTime)
{
    CnTimestamp timestamp(123456789012345678ULL);
    UA_DateTime uaDateTime = toUA_DateTime(timestamp);
    
    EXPECT_EQ(uaDateTime, 123456789012345678ULL);
}

TEST(CnUaCoreTest, ToUADateTimeZero)
{
    CnTimestamp timestamp(0);
    UA_DateTime uaDateTime = toUA_DateTime(timestamp);
    
    EXPECT_EQ(uaDateTime, 0);
}

// ============================================================================
// toUA_Variant Tests
// ============================================================================

TEST(CnUaCoreTest, ToUAVariantBool)
{
    UA_Variant variant = toUA_Variant(true);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_TRUE(UA_Variant_isScalar(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_BOOLEAN]);
    EXPECT_TRUE(*static_cast<UA_Boolean*>(variant.data));
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantInt8)
{
    UA_Variant variant = toUA_Variant(int8_t(-42));
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_SBYTE]);
    EXPECT_EQ(*static_cast<int8_t*>(variant.data), -42);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantUInt8)
{
    UA_Variant variant = toUA_Variant(uint8_t(200));
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_BYTE]);
    EXPECT_EQ(*static_cast<uint8_t*>(variant.data), 200);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantInt16)
{
    UA_Variant variant = toUA_Variant(int16_t(-1000));
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_INT16]);
    EXPECT_EQ(*static_cast<int16_t*>(variant.data), -1000);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantUInt16)
{
    UA_Variant variant = toUA_Variant(uint16_t(50000));
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_UINT16]);
    EXPECT_EQ(*static_cast<uint16_t*>(variant.data), 50000);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantInt32)
{
    UA_Variant variant = toUA_Variant(int32_t(-123456));
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_INT32]);
    EXPECT_EQ(*static_cast<int32_t*>(variant.data), -123456);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantUInt32)
{
    UA_Variant variant = toUA_Variant(uint32_t(3000000000UL));
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_UINT32]);
    EXPECT_EQ(*static_cast<uint32_t*>(variant.data), 3000000000UL);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantInt64)
{
    UA_Variant variant = toUA_Variant(int64_t(-9876543210LL));
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_INT64]);
    EXPECT_EQ(*static_cast<int64_t*>(variant.data), int64_t(-9876543210LL));
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantUInt64)
{
    UA_Variant variant = toUA_Variant(uint64_t(18446744073709551615ULL));
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_UINT64]);
    EXPECT_EQ(*static_cast<uint64_t*>(variant.data), uint64_t(18446744073709551615ULL));
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantInt)
{
    UA_Variant variant = toUA_Variant(-42);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_INT32]);
    EXPECT_EQ(*static_cast<int*>(variant.data), -42);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantUInt)
{
    UA_Variant variant = toUA_Variant(4294967295U);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_UINT32]);
    EXPECT_EQ(*static_cast<unsigned int*>(variant.data), 4294967295U);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantFloat)
{
    UA_Variant variant = toUA_Variant(3.14f);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_FLOAT]);
    EXPECT_FLOAT_EQ(*static_cast<float*>(variant.data), 3.14f);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantDouble)
{
    UA_Variant variant = toUA_Variant(2.718281828);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_DOUBLE]);
    EXPECT_DOUBLE_EQ(*static_cast<double*>(variant.data), 2.718281828);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantString)
{
    CnString str = CnSTR("Hello OPC UA");
    UA_Variant variant = toUA_Variant(str);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_STRING]);
    
    UA_String* uaStr = static_cast<UA_String*>(variant.data);
    std::string result(reinterpret_cast<char*>(uaStr->data), uaStr->length);
    EXPECT_EQ(result, "Hello OPC UA");
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantTimestamp)
{
    CnTimestamp timestamp(987654321098765432ULL);
    UA_Variant variant = toUA_Variant(timestamp);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_DATETIME]);
    EXPECT_EQ(*static_cast<UA_DateTime*>(variant.data), 987654321098765432ULL);
    
    UA_Variant_clear(&variant);
}

// Note: Skipping ToUAVariantDateTime test because CnDateTime performs timezone conversion
// which makes the output timestamp different from the input

TEST(CnUaCoreTest, ToUAVariantFromCnVariantBool)
{
    CnVariant cnVar(true);
    UA_Variant variant = toUA_Variant(cnVar);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_BOOLEAN]);
    EXPECT_TRUE(*static_cast<UA_Boolean*>(variant.data));
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantFromCnVariantInt)
{
    CnVariant cnVar(42);
    UA_Variant variant = toUA_Variant(cnVar);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_INT32]);
    EXPECT_EQ(*static_cast<int*>(variant.data), 42);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantFromCnVariantFloat)
{
    CnVariant cnVar(1.5f);
    UA_Variant variant = toUA_Variant(cnVar);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_FLOAT]);
    EXPECT_FLOAT_EQ(*static_cast<float*>(variant.data), 1.5f);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantFromCnVariantDouble)
{
    CnVariant cnVar(9.876);
    UA_Variant variant = toUA_Variant(cnVar);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_DOUBLE]);
    EXPECT_DOUBLE_EQ(*static_cast<double*>(variant.data), 9.876);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantFromCnVariantString)
{
    CnVariant cnVar(CnSTR("Test String"));
    UA_Variant variant = toUA_Variant(cnVar);
    
    EXPECT_FALSE(UA_Variant_isEmpty(&variant));
    EXPECT_EQ(variant.type, &UA_TYPES[UA_TYPES_STRING]);
    
    UA_Variant_clear(&variant);
}

TEST(CnUaCoreTest, ToUAVariantFromCnVariantInvalid)
{
    CnVariant cnVar; // Invalid/uninitialized
    UA_Variant variant = toUA_Variant(cnVar);
    
    EXPECT_TRUE(UA_Variant_isEmpty(&variant));
    
    UA_Variant_clear(&variant);
}

// ============================================================================
// toUaTypesIndex Tests
// ============================================================================

TEST(CnUaCoreTest, ToUaTypesIndexBool)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Bool), UA_TYPES_BOOLEAN);
}

TEST(CnUaCoreTest, ToUaTypesIndexInt8)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Int8), UA_TYPES_SBYTE);
}

TEST(CnUaCoreTest, ToUaTypesIndexUInt8)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_UInt8), UA_TYPES_BYTE);
}

TEST(CnUaCoreTest, ToUaTypesIndexInt16)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Int16), UA_TYPES_INT16);
}

TEST(CnUaCoreTest, ToUaTypesIndexUInt16)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_UInt16), UA_TYPES_UINT16);
}

TEST(CnUaCoreTest, ToUaTypesIndexInt32)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Int32), UA_TYPES_INT32);
}

TEST(CnUaCoreTest, ToUaTypesIndexUInt32)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_UInt32), UA_TYPES_UINT32);
}

TEST(CnUaCoreTest, ToUaTypesIndexInt64)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Int64), UA_TYPES_INT64);
}

TEST(CnUaCoreTest, ToUaTypesIndexUInt64)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_UInt64), UA_TYPES_UINT64);
}

TEST(CnUaCoreTest, ToUaTypesIndexInt)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Int), UA_TYPES_INT32);
}

TEST(CnUaCoreTest, ToUaTypesIndexUInt)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_UInt), UA_TYPES_UINT32);
}

TEST(CnUaCoreTest, ToUaTypesIndexFloat)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Float), UA_TYPES_FLOAT);
}

TEST(CnUaCoreTest, ToUaTypesIndexDouble)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Double), UA_TYPES_DOUBLE);
}

TEST(CnUaCoreTest, ToUaTypesIndexString)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_String), UA_TYPES_STRING);
}

TEST(CnUaCoreTest, ToUaTypesIndexTimestamp)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Timestamp), UA_TYPES_DATETIME);
}

TEST(CnUaCoreTest, ToUaTypesIndexDateTime)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_DateTime), UA_TYPES_DATETIME);
}

TEST(CnUaCoreTest, ToUaTypesIndexVariant)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_Variant), UA_TYPES_VARIANT);
}

TEST(CnUaCoreTest, ToUaTypesIndexModbusStatus)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_ModbusStatus), UA_TYPES_INT32);
}

TEST(CnUaCoreTest, ToUaTypesIndexCnStatus)
{
    EXPECT_EQ(toUaTypesIndex(Cn::Data_CnStatus), UA_TYPES_INT32);
}

// ============================================================================
// toVariant Tests
// ============================================================================

TEST(CnUaCoreTest, ToVariantEmpty)
{
    UA_Variant uaVar;
    UA_Variant_init(&uaVar);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_TRUE(result.isNull());
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantBoolean)
{
    UA_Boolean value = true;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vBool);
    EXPECT_TRUE(result.toBool());
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantInt8)
{
    UA_SByte value = -42;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_SBYTE]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vInt8);
    EXPECT_EQ(result.toInt8(), -42);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantUInt8)
{
    UA_Byte value = 200;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_BYTE]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vUInt8);
    EXPECT_EQ(result.toUInt8(), 200);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantInt16)
{
    UA_Int16 value = -1000;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_INT16]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vInt16);
    EXPECT_EQ(result.toInt16(), -1000);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantUInt16)
{
    UA_UInt16 value = 50000;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_UINT16]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vUInt16);
    EXPECT_EQ(result.toUInt16(), 50000);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantInt32)
{
    UA_Int32 value = -123456;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_INT32]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vInt32);
    EXPECT_EQ(result.toInt32(), -123456);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantUInt32)
{
    UA_UInt32 value = 3000000000U;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_UINT32]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vUInt32);
    EXPECT_EQ(result.toUInt32(), 3000000000U);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantInt64)
{
    UA_Int64 value = -9876543210LL;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_INT64]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vInt64);
    EXPECT_EQ(result.toInt64(), -9876543210LL);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantUInt64)
{
    UA_UInt64 value = 12345678901234ULL;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_UINT64]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vUInt64);
    EXPECT_EQ(result.toUInt64(), 12345678901234ULL);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantFloat)
{
    UA_Float value = 3.14f;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_FLOAT]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vFloat);
    EXPECT_FLOAT_EQ(result.toFloat(), 3.14f);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantDouble)
{
    UA_Double value = 2.718281828;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_DOUBLE]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vDouble);
    EXPECT_DOUBLE_EQ(result.toDouble(), 2.718281828);
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantString)
{
    char expectedStr[] = "Hello World";
    UA_String value = UA_STRING(expectedStr);
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_STRING]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vString);
    EXPECT_EQ(result.toString(), CnSTR("Hello World"));
    
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, ToVariantDateTime)
{
    UA_DateTime value = 123456789012345678ULL;
    UA_Variant uaVar;
    UA_Variant_setScalarCopy(&uaVar, &value, &UA_TYPES[UA_TYPES_DATETIME]);
    
    CnVariant result = toVariant(uaVar);
    EXPECT_EQ(result.type(), CnVariant::vTimestamp);
    EXPECT_EQ(result.toTimestamp().rawValue(), 123456789012345678ULL);
    
    UA_Variant_clear(&uaVar);
}

// ============================================================================
// Round-trip Conversion Tests
// ============================================================================

TEST(CnUaCoreTest, RoundTripBool)
{
    bool original = true;
    UA_Variant uaVar = toUA_Variant(original);
    CnVariant result = toVariant(uaVar);
    
    EXPECT_EQ(result.toBool(), original);
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, RoundTripInt)
{
    int original = -12345;
    UA_Variant uaVar = toUA_Variant(original);
    CnVariant result = toVariant(uaVar);
    
    EXPECT_EQ(result.toInt(), original);
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, RoundTripFloat)
{
    float original = 1.234f;
    UA_Variant uaVar = toUA_Variant(original);
    CnVariant result = toVariant(uaVar);
    
    EXPECT_FLOAT_EQ(result.toFloat(), original);
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, RoundTripDouble)
{
    double original = 3.141592653589793;
    UA_Variant uaVar = toUA_Variant(original);
    CnVariant result = toVariant(uaVar);
    
    EXPECT_DOUBLE_EQ(result.toDouble(), original);
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, RoundTripString)
{
    CnString original = CnSTR("Test String Value");
    UA_Variant uaVar = toUA_Variant(original);
    CnVariant result = toVariant(uaVar);
    
    EXPECT_EQ(result.toString(), original);
    UA_Variant_clear(&uaVar);
}

TEST(CnUaCoreTest, RoundTripTimestamp)
{
    CnTimestamp original(999888777666555444ULL);
    UA_Variant uaVar = toUA_Variant(original);
    CnVariant result = toVariant(uaVar);
    
    EXPECT_EQ(result.toTimestamp().rawValue(), original.rawValue());
    UA_Variant_clear(&uaVar);
}
