#include <gtest/gtest.h>
#include <core/CnBaseItem.h>

// ============================================================================
// Test Implementation of CnBaseItem (concrete class for testing)
// ============================================================================

class TestCnBaseItem : public CnBaseItem
{
public:
    TestCnBaseItem(const CnString &name = CnString()) 
        : CnBaseItem(name)
        , m_dataType(Cn::Data_Unknown)
        , m_access(Cn::Access_Read)
        , m_value()
        , m_status(Cn::Status_Good)
        , m_timestamp()
    {
    }

    // Override pure virtual functions
    Cn::DataType dataType() const override { return m_dataType; }
    
    Cn::Access access() const override { return m_access; }
    
    CnVariant value(Cn::StatusCode *status = nullptr, CnTimestamp *timestamp = nullptr) const override
    {
        if (status)
            *status = m_status;
        if (timestamp)
            *timestamp = m_timestamp;
        return m_value;
    }
    
    void update(const CnVariant &value, Cn::StatusCode status = Cn::Status_Good, CnTimestamp timestamp = CnTimestamp()) override
    {
        m_value = value;
        m_status = status;
        m_timestamp = timestamp;
    }
    
    // Bring base class overloads into scope
    using CnBaseItem::update;
    using CnBaseItem::updateStatusTimestamp;
    
    // Test setters
    void setDataType(Cn::DataType type) { m_dataType = type; }
    void setAccess(Cn::Access access) { m_access = access; }

private:
    Cn::DataType m_dataType;
    Cn::Access m_access;
    CnVariant m_value;
    Cn::StatusCode m_status;
    CnTimestamp m_timestamp;
};

// ============================================================================
// CnBaseItem Tests
// ============================================================================

TEST(CnBaseItemTest, DefaultConstructor)
{
    TestCnBaseItem item;
    
    EXPECT_TRUE(item.name().isEmpty());
    EXPECT_TRUE(item.description().isEmpty());
    EXPECT_EQ(item.context(), nullptr);
}

TEST(CnBaseItemTest, ConstructorWithName)
{
    TestCnBaseItem item(CnSTR("TestItem"));
    
    EXPECT_EQ(item.name(), CnSTR("TestItem"));
    EXPECT_TRUE(item.description().isEmpty());
    EXPECT_EQ(item.context(), nullptr);
}

TEST(CnBaseItemTest, SetAndGetNameString)
{
    TestCnBaseItem item;
    
    item.setName(CnSTR("NewName"));
    EXPECT_EQ(item.name(), CnSTR("NewName"));
}

TEST(CnBaseItemTest, SetAndGetNameCString)
{
    TestCnBaseItem item;
    
    item.setName(CnSTR("CStringName"));
    EXPECT_EQ(item.name(), CnSTR("CStringName"));
}

TEST(CnBaseItemTest, SetNameMultipleTimes)
{
    TestCnBaseItem item(CnSTR("Initial"));
    
    EXPECT_EQ(item.name(), CnSTR("Initial"));
    
    item.setName(CnSTR("Second"));
    EXPECT_EQ(item.name(), CnSTR("Second"));
    
    item.setName(CnSTR("Third"));
    EXPECT_EQ(item.name(), CnSTR("Third"));
}

TEST(CnBaseItemTest, SetAndGetDescriptionString)
{
    TestCnBaseItem item;
    
    item.setDescription(CnSTR("Test Description"));
    EXPECT_EQ(item.description(), CnSTR("Test Description"));
}

TEST(CnBaseItemTest, SetAndGetDescriptionCString)
{
    TestCnBaseItem item;
    
    item.setDescription(CnSTR("C String Description"));
    EXPECT_EQ(item.description(), CnSTR("C String Description"));
}

TEST(CnBaseItemTest, SetDescriptionMultipleTimes)
{
    TestCnBaseItem item;
    
    item.setDescription(CnSTR("First description"));
    EXPECT_EQ(item.description(), CnSTR("First description"));
    
    item.setDescription(CnSTR("Second description"));
    EXPECT_EQ(item.description(), CnSTR("Second description"));
}

TEST(CnBaseItemTest, EmptyDescription)
{
    TestCnBaseItem item;
    
    item.setDescription(CnSTR(""));
    EXPECT_TRUE(item.description().isEmpty());
}

TEST(CnBaseItemTest, SetAndGetContext)
{
    TestCnBaseItem item;
    int testData = 42;
    
    item.setContext(&testData);
    EXPECT_EQ(item.context(), &testData);
    EXPECT_EQ(*static_cast<int*>(item.context()), 42);
}

TEST(CnBaseItemTest, ContextNullByDefault)
{
    TestCnBaseItem item;
    
    EXPECT_EQ(item.context(), nullptr);
}

TEST(CnBaseItemTest, ChangeContext)
{
    TestCnBaseItem item;
    int data1 = 10;
    int data2 = 20;
    
    item.setContext(&data1);
    EXPECT_EQ(item.context(), &data1);
    
    item.setContext(&data2);
    EXPECT_EQ(item.context(), &data2);
}

TEST(CnBaseItemTest, ContextWithStructure)
{
    struct TestStruct {
        int value;
        CnString name;
    };
    
    TestCnBaseItem item;
    TestStruct testStruct = {100, CnSTR("test")};
    
    item.setContext(&testStruct);
    
    TestStruct *retrieved = static_cast<TestStruct*>(item.context());
    EXPECT_EQ(retrieved->value, 100);
    EXPECT_EQ(retrieved->name, CnSTR("test"));
}

TEST(CnBaseItemTest, DefaultAccessType)
{
    TestCnBaseItem item;
    
    EXPECT_EQ(item.access(), Cn::Access_Read);
}

TEST(CnBaseItemTest, SetAndGetDataType)
{
    TestCnBaseItem item;
    
    item.setDataType(Cn::Data_Int32);
    EXPECT_EQ(item.dataType(), Cn::Data_Int32);
}

TEST(CnBaseItemTest, AllDataTypes)
{
    TestCnBaseItem item;
    
    Cn::DataType types[] = {
        Cn::Data_Unknown,
        Cn::Data_Bool,
        Cn::Data_Int8,
        Cn::Data_UInt8,
        Cn::Data_Int16,
        Cn::Data_UInt16,
        Cn::Data_Int32,
        Cn::Data_UInt32,
        Cn::Data_Int64,
        Cn::Data_UInt64,
        Cn::Data_Int,
        Cn::Data_UInt,
        Cn::Data_Float,
        Cn::Data_Double,
        Cn::Data_String,
        Cn::Data_Timestamp,
        Cn::Data_DateTime,
        Cn::Data_Variant,
        Cn::Data_ModbusStatus,
        Cn::Data_CnStatus
    };
    
    for (Cn::DataType type : types)
    {
        item.setDataType(type);
        EXPECT_EQ(item.dataType(), type);
    }
}

TEST(CnBaseItemTest, AllAccessTypes)
{
    TestCnBaseItem item;
    
    item.setAccess(Cn::Access_Read);
    EXPECT_EQ(item.access(), Cn::Access_Read);
    
    item.setAccess(Cn::Access_Write);
    EXPECT_EQ(item.access(), Cn::Access_Write);
    
    item.setAccess(Cn::Access_ReadWrite);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
}

TEST(CnBaseItemTest, UpdateWithIntValue)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(123), Cn::Status_Good, CnTimestamp());
    
    Cn::StatusCode status;
    CnTimestamp timestamp;
    CnVariant value = item.value(&status, &timestamp);
    
    EXPECT_EQ(value.toInt(), 123);
    EXPECT_EQ(status, Cn::Status_Good);
}

TEST(CnBaseItemTest, UpdateWithBoolValue)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(true), Cn::Status_Good, CnTimestamp());
    
    CnVariant value = item.value();
    EXPECT_TRUE(value.toBool());
}

TEST(CnBaseItemTest, UpdateWithFloatValue)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(3.14f), Cn::Status_Good, CnTimestamp());
    
    CnVariant value = item.value();
    EXPECT_FLOAT_EQ(value.toFloat(), 3.14f);
}

TEST(CnBaseItemTest, UpdateWithDoubleValue)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(2.71828), Cn::Status_Good, CnTimestamp());
    
    CnVariant value = item.value();
    EXPECT_DOUBLE_EQ(value.toDouble(), 2.71828);
}

TEST(CnBaseItemTest, UpdateWithStringValue)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(CnSTR("TestString")), Cn::Status_Good, CnTimestamp());
    
    CnVariant value = item.value();
    EXPECT_EQ(value.toString(), CnSTR("TestString"));
}

TEST(CnBaseItemTest, UpdateStatusGood)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(100), Cn::Status_Good, CnTimestamp());
    
    Cn::StatusCode status;
    item.value(&status);
    
    EXPECT_EQ(status, Cn::Status_Good);
    EXPECT_TRUE(Cn::StatusIsGood(status));
}

TEST(CnBaseItemTest, UpdateStatusBad)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(100), Cn::Status_Bad, CnTimestamp());
    
    Cn::StatusCode status;
    item.value(&status);
    
    EXPECT_EQ(status, Cn::Status_Bad);
    EXPECT_TRUE(Cn::StatusIsBad(status));
}

TEST(CnBaseItemTest, UpdateStatusUncertain)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(100), Cn::Status_Uncertain, CnTimestamp());
    
    Cn::StatusCode status;
    item.value(&status);
    
    EXPECT_EQ(status, Cn::Status_Uncertain);
    EXPECT_TRUE(Cn::StatusIsUncertain(status));
}

TEST(CnBaseItemTest, UpdateAllStatusCodes)
{
    TestCnBaseItem item;
    
    Cn::StatusCode statuses[] = {
        Cn::Status_Processing,
        Cn::Status_Bad,
        Cn::Status_BadOutOfService,
        Cn::Status_BadNotConnected,
        Cn::Status_BadConfigure,
        Cn::Status_BadNoService,
        Cn::Status_Uncertain,
        Cn::Status_Good,
        Cn::Status_GoodNoData
    };
    
    for (Cn::StatusCode expectedStatus : statuses)
    {
        item.update(CnVariant(42), expectedStatus, CnTimestamp());
        
        Cn::StatusCode retrievedStatus;
        item.value(&retrievedStatus);
        
        EXPECT_EQ(retrievedStatus, expectedStatus);
    }
}

TEST(CnBaseItemTest, UpdateWithTimestamp)
{
    TestCnBaseItem item;
    CnTimestamp ts = CnTimestamp::current();
    
    item.update(CnVariant(999), Cn::Status_Good, ts);
    
    CnTimestamp retrievedTs;
    item.value(nullptr, &retrievedTs);
    
    EXPECT_EQ(retrievedTs, ts);
}

TEST(CnBaseItemTest, UpdateOnlyStatus)
{
    TestCnBaseItem item;
    item.update(CnVariant(123), Cn::Status_Good, CnTimestamp());
    
    // Update only status
    item.update(Cn::Status_Bad, CnTimestamp());
    
    Cn::StatusCode status;
    CnVariant value = item.value(&status);
    
    EXPECT_EQ(status, Cn::Status_Bad);
    // Value should not be set (empty variant)
    EXPECT_TRUE(value.isNull());
}

TEST(CnBaseItemTest, UpdateStatusTimestamp)
{
    TestCnBaseItem item;
    CnTimestamp ts = CnTimestamp::current();
    
    item.updateStatusTimestamp(Cn::Status_BadNotConnected, ts);
    
    Cn::StatusCode status;
    CnTimestamp retrievedTs;
    item.value(&status, &retrievedTs);
    
    EXPECT_EQ(status, Cn::Status_BadNotConnected);
    EXPECT_EQ(retrievedTs, ts);
}

TEST(CnBaseItemTest, ValueWithoutStatusParameter)
{
    TestCnBaseItem item;
    item.update(CnVariant(555), Cn::Status_Good, CnTimestamp());
    
    CnVariant value = item.value();
    EXPECT_EQ(value.toInt(), 555);
}

TEST(CnBaseItemTest, ValueWithoutTimestampParameter)
{
    TestCnBaseItem item;
    item.update(CnVariant(777), Cn::Status_Good, CnTimestamp());
    
    Cn::StatusCode status;
    CnVariant value = item.value(&status);
    
    EXPECT_EQ(value.toInt(), 777);
    EXPECT_EQ(status, Cn::Status_Good);
}

TEST(CnBaseItemTest, MultipleUpdates)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(100), Cn::Status_Good, CnTimestamp());
    EXPECT_EQ(item.value().toInt(), 100);
    
    item.update(CnVariant(200), Cn::Status_Good, CnTimestamp());
    EXPECT_EQ(item.value().toInt(), 200);
    
    item.update(CnVariant(300), Cn::Status_Bad, CnTimestamp());
    Cn::StatusCode status;
    CnVariant value = item.value(&status);
    EXPECT_EQ(value.toInt(), 300);
    EXPECT_EQ(status, Cn::Status_Bad);
}

TEST(CnBaseItemTest, CompleteItemConfiguration)
{
    TestCnBaseItem item;
    int contextData = 12345;
    CnTimestamp ts = CnTimestamp::current();
    
    item.setName(CnSTR("CompleteItem"));
    item.setDescription(CnSTR("A fully configured test item"));
    item.setContext(&contextData);
    item.setDataType(Cn::Data_Int32);
    item.setAccess(Cn::Access_ReadWrite);
    item.update(CnVariant(9999), Cn::Status_Good, ts);
    
    EXPECT_EQ(item.name(), CnSTR("CompleteItem"));
    EXPECT_EQ(item.description(), CnSTR("A fully configured test item"));
    EXPECT_EQ(item.context(), &contextData);
    EXPECT_EQ(item.dataType(), Cn::Data_Int32);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
    
    Cn::StatusCode status;
    CnTimestamp retrievedTs;
    CnVariant value = item.value(&status, &retrievedTs);
    
    EXPECT_EQ(value.toInt(), 9999);
    EXPECT_EQ(status, Cn::Status_Good);
    EXPECT_EQ(retrievedTs, ts);
}

TEST(CnBaseItemTest, NameWithSpecialCharacters)
{
    TestCnBaseItem item;
    
    item.setName(CnSTR("Device:Port1.Value[0]"));
    EXPECT_EQ(item.name(), CnSTR("Device:Port1.Value[0]"));
}

TEST(CnBaseItemTest, LongDescription)
{
    TestCnBaseItem item;
    CnString longDesc = CnSTR("This is a very long description that contains multiple sentences. ");
    longDesc += CnSTR("It describes the purpose and usage of the item in great detail. ");
    longDesc += CnSTR("This helps users understand what the item represents.");
    
    item.setDescription(longDesc);
    EXPECT_EQ(item.description(), longDesc);
}

TEST(CnBaseItemTest, UpdateWithZeroValue)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(0), Cn::Status_Good, CnTimestamp());
    
    EXPECT_EQ(item.value().toInt(), 0);
}

TEST(CnBaseItemTest, UpdateWithNegativeValue)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(-12345), Cn::Status_Good, CnTimestamp());
    
    EXPECT_EQ(item.value().toInt(), -12345);
}

TEST(CnBaseItemTest, UpdateWithMaxIntValue)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(2147483647), Cn::Status_Good, CnTimestamp());
    
    EXPECT_EQ(item.value().toInt(), 2147483647);
}

TEST(CnBaseItemTest, UpdateWithMinIntValue)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(-2147483647 - 1), Cn::Status_Good, CnTimestamp());
    
    EXPECT_EQ(item.value().toInt(), -2147483647 - 1);
}

TEST(CnBaseItemTest, EmptyVariant)
{
    TestCnBaseItem item;
    
    item.update(CnVariant(), Cn::Status_Good, CnTimestamp());
    
    CnVariant value = item.value();
    EXPECT_TRUE(value.isNull());
}

TEST(CnBaseItemTest, SequentialStatusChanges)
{
    TestCnBaseItem item;
    item.update(CnVariant(100), Cn::Status_Good, CnTimestamp());
    
    Cn::StatusCode status;
    item.value(&status);
    EXPECT_EQ(status, Cn::Status_Good);
    
    item.update(CnVariant(100), Cn::Status_Uncertain, CnTimestamp());
    item.value(&status);
    EXPECT_EQ(status, Cn::Status_Uncertain);
    
    item.update(CnVariant(100), Cn::Status_Bad, CnTimestamp());
    item.value(&status);
    EXPECT_EQ(status, Cn::Status_Bad);
    
    item.update(CnVariant(100), Cn::Status_Good, CnTimestamp());
    item.value(&status);
    EXPECT_EQ(status, Cn::Status_Good);
}

TEST(CnBaseItemTest, NameAndDescriptionIndependent)
{
    TestCnBaseItem item;
    
    item.setName(CnSTR("ItemName"));
    item.setDescription(CnSTR("ItemDescription"));
    
    EXPECT_EQ(item.name(), CnSTR("ItemName"));
    EXPECT_EQ(item.description(), CnSTR("ItemDescription"));
    
    item.setName(CnSTR("NewName"));
    EXPECT_EQ(item.name(), CnSTR("NewName"));
    EXPECT_EQ(item.description(), CnSTR("ItemDescription"));
}

TEST(CnBaseItemTest, SetContextToNull)
{
    TestCnBaseItem item;
    int data = 100;
    
    item.setContext(&data);
    EXPECT_NE(item.context(), nullptr);
    
    item.setContext(nullptr);
    EXPECT_EQ(item.context(), nullptr);
}

TEST(CnBaseItemTest, CopyConstructorNotAvailable)
{
    // CnBaseItem does not have a public copy constructor
    // This test documents the design decision
    
    TestCnBaseItem item1(CnSTR("Original"));
    // TestCnBaseItem item2(item1); // This would not compile
    
    EXPECT_EQ(item1.name(), CnSTR("Original"));
}

TEST(CnBaseItemTest, TimestampPreservation)
{
    TestCnBaseItem item;
    
    CnTimestamp ts1 = CnTimestamp::current();
    item.update(CnVariant(111), Cn::Status_Good, ts1);
    
    CnTimestamp retrieved1;
    item.value(nullptr, &retrieved1);
    EXPECT_EQ(retrieved1, ts1);
    
    // Wait a bit and update with new timestamp
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    CnTimestamp ts2 = CnTimestamp::current();
    item.update(CnVariant(222), Cn::Status_Good, ts2);
    
    CnTimestamp retrieved2;
    item.value(nullptr, &retrieved2);
    EXPECT_EQ(retrieved2, ts2);
    EXPECT_NE(retrieved1, retrieved2);
}
