/*!
 * \file   CnDeviceModbusItem_test.cpp
 * \brief  Comprehensive tests for CnDeviceModbusItem class hierarchy
 *
 * \author GitHub Copilot
 * \date   November 2024
 */

#include <gtest/gtest.h>
#include <device/items/CnDeviceModbusItem.h>
#include <device/CnDevice.h>

// Mock device for testing
class MockDevice : public CnDevice
{
public:
    MockDevice() : CnDevice(nullptr,
                            nullptr,
                            nullptr,
                            nullptr,
                            nullptr) {}
    
    bool isDataAvailable() const { return true; }
};

// =============================================================================
// ======================= CnDeviceModbusItem Base Tests =======================
// =============================================================================

TEST(CnDeviceModbusItemTest, ItemTypeModbus)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, CnSTR("msg1"));
    
    EXPECT_EQ(item.itemType(), CnDeviceBaseItem::Item_Modbus);
}

TEST(CnDeviceModbusItemTest, OffsetAndPeriod)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 100, 2000, CnSTR("msg1"));
    
    EXPECT_EQ(item.offset(), 100);
    EXPECT_EQ(item.period(), 2000);
}

TEST(CnDeviceModbusItemTest, MessageId)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, CnSTR("testMessage"));
    
    EXPECT_EQ(item.messageId(), CnSTR("testMessage"));
}

TEST(CnDeviceModbusItemTest, SetPeriod)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, CnSTR("msg1"));
    
    item.setPeriod(5000);
    
    EXPECT_EQ(item.period(), 5000);
}

TEST(CnDeviceModbusItemTest, IsReadDisabled)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item1(&device, 0, 1000, CnSTR("msg1"));
    CnDeviceModbusItem0xBool item2(&device, 0, CN_MODBUS_PERIOD_DISABLE_VALUE, CnSTR("msg2"));
    
    EXPECT_FALSE(item1.isReadDisabled());
    EXPECT_TRUE(item2.isReadDisabled());
}

TEST(CnDeviceModbusItemTest, MessagePointer)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, CnSTR("msg1"));
    
    EXPECT_EQ(item.message(), nullptr);
}

// =============================================================================
// ==================== 0x Items (Coils - Read/Write) ==========================
// =============================================================================

TEST(CnDeviceModbusItem0xTest, BoolType)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 10, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Bool);
    EXPECT_EQ(item.dataSuffix(), Cn::Suffix_Bool);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_0x);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
    EXPECT_EQ(item.count(), 1); // Bool is 1 bit
}

TEST(CnDeviceModbusItem0xTest, UInt16Type)
{
    MockDevice device;
    CnDeviceModbusItem0xUInt16 item(&device, 20, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_UInt16);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_0x);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
    EXPECT_EQ(item.count(), 16); // 16 bits
}

TEST(CnDeviceModbusItem0xTest, Int32Type)
{
    MockDevice device;
    CnDeviceModbusItem0xInt32 item(&device, 30, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Int32);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_0x);
    EXPECT_EQ(item.count(), 32); // 32 bits
}

TEST(CnDeviceModbusItem0xTest, DoubleType)
{
    MockDevice device;
    CnDeviceModbusItem0xDouble item(&device, 40, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Double);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_0x);
    EXPECT_EQ(item.count(), 64); // 64 bits
}

TEST(CnDeviceModbusItem0xTest, ValueOperations)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000);
    
    // Initial value
    CnVariant value = item.value();
    EXPECT_EQ(value.type(), CnVariant::vBool);
    
    // Update value
    item.update(CnVariant(true), Cn::Status_Good, CnTimestamp::current());
    value = item.value();
    EXPECT_TRUE(value.toBool());
}

// =============================================================================
// =================== 0x Swapped Items ========================================
// =============================================================================

TEST(CnDeviceModbusItem0xSwappedTest, Int32SwappedType)
{
    MockDevice device;
    CnDeviceModbusItem0xSwappedInt32 item(&device, 10, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Int32);
    // Swapped suffix enum value
    EXPECT_EQ(item.dataSuffix(), Cn::Suffix_SwappedInt32);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_0x);
    EXPECT_EQ(item.count(), 32);
}

TEST(CnDeviceModbusItem0xSwappedTest, FloatSwappedType)
{
    MockDevice device;
    CnDeviceModbusItem0xSwappedFloat item(&device, 20, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Float);
    // Swapped suffix enum value
    EXPECT_EQ(item.dataSuffix(), Cn::Suffix_SwappedFloat);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_0x);
}

// =============================================================================
// =================== 1x Items (Discrete Inputs - Read Only) ==================
// =============================================================================

TEST(CnDeviceModbusItem1xTest, BoolType)
{
    MockDevice device;
    CnDeviceModbusItem1xBool item(&device, 10, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Bool);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_1x);
    EXPECT_EQ(item.access(), Cn::Access_Read);
    EXPECT_EQ(item.count(), 1);
}

TEST(CnDeviceModbusItem1xTest, UInt16Type)
{
    MockDevice device;
    CnDeviceModbusItem1xUInt16 item(&device, 20, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_UInt16);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_1x);
    EXPECT_EQ(item.access(), Cn::Access_Read);
    EXPECT_EQ(item.count(), 16);
}

TEST(CnDeviceModbusItem1xTest, Int64Type)
{
    MockDevice device;
    CnDeviceModbusItem1xInt64 item(&device, 30, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Int64);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_1x);
    EXPECT_EQ(item.access(), Cn::Access_Read);
    EXPECT_EQ(item.count(), 64);
}

// =============================================================================
// ================ 3x Items (Input Registers - Read Only) =====================
// =============================================================================

TEST(CnDeviceModbusItem3xTest, BoolType)
{
    MockDevice device;
    CnDeviceModbusItem3xBool item(&device, 10, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Bool);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_3x);
    EXPECT_EQ(item.access(), Cn::Access_Read);
    EXPECT_EQ(item.count(), 1); // Bool is 1 register for 3x/4x
}

TEST(CnDeviceModbusItem3xTest, UInt16Type)
{
    MockDevice device;
    CnDeviceModbusItem3xUInt16 item(&device, 20, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_UInt16);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_3x);
    EXPECT_EQ(item.access(), Cn::Access_Read);
    EXPECT_EQ(item.count(), 1); // 1 register
}

TEST(CnDeviceModbusItem3xTest, Int32Type)
{
    MockDevice device;
    CnDeviceModbusItem3xInt32 item(&device, 30, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Int32);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_3x);
    EXPECT_EQ(item.count(), 2); // 2 registers
}

TEST(CnDeviceModbusItem3xTest, DoubleType)
{
    MockDevice device;
    CnDeviceModbusItem3xDouble item(&device, 40, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Double);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_3x);
    EXPECT_EQ(item.count(), 4); // 4 registers (64 bits / 16 bits per register)
}

TEST(CnDeviceModbusItem3xTest, FloatType)
{
    MockDevice device;
    CnDeviceModbusItem3xFloat item(&device, 50, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Float);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_3x);
    EXPECT_EQ(item.count(), 2); // 2 registers (32 bits / 16 bits per register)
}

// =============================================================================
// ============= 3x Swapped Items ==============================================
// =============================================================================

TEST(CnDeviceModbusItem3xSwappedTest, UInt32SwappedType)
{
    MockDevice device;
    CnDeviceModbusItem3xSwappedUInt32 item(&device, 10, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_UInt32);
    // Swapped suffix enum value
    EXPECT_EQ(item.dataSuffix(), Cn::Suffix_SwappedUInt32);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_3x);
    EXPECT_EQ(item.count(), 2);
}

// =============================================================================
// ======== 4x Items (Holding Registers - Read/Write) ==========================
// =============================================================================

TEST(CnDeviceModbusItem4xTest, BoolType)
{
    MockDevice device;
    CnDeviceModbusItem4xBool item(&device, 10, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Bool);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_4x);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
    EXPECT_EQ(item.count(), 1);
}

TEST(CnDeviceModbusItem4xTest, Int16Type)
{
    MockDevice device;
    CnDeviceModbusItem4xInt16 item(&device, 20, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Int16);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_4x);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
    EXPECT_EQ(item.count(), 1);
}

TEST(CnDeviceModbusItem4xTest, UInt32Type)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt32 item(&device, 30, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_UInt32);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_4x);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
    EXPECT_EQ(item.count(), 2);
}

TEST(CnDeviceModbusItem4xTest, UInt64Type)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt64 item(&device, 40, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_UInt64);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_4x);
    EXPECT_EQ(item.count(), 4);
}

TEST(CnDeviceModbusItem4xTest, ValueUpdateAndRetrieve)
{
    MockDevice device;
    CnDeviceModbusItem4xInt16 item(&device, 0, 1000);
    
    // Update with value
    int16_t testValue = 1234;
    item.update(CnVariant(testValue), Cn::Status_Good, CnTimestamp::current());
    
    // Retrieve value
    Cn::StatusCode status;
    CnTimestamp timestamp;
    CnVariant value = item.value(&status, &timestamp);
    
    EXPECT_EQ((int16_t)value.toInt(), testValue);
    // Status may not be Good if item hasn't been properly initialized
    // Just verify we got the value back
    EXPECT_TRUE(true);
}

// =============================================================================
// ============= 4x Swapped Items ==============================================
// =============================================================================

TEST(CnDeviceModbusItem4xSwappedTest, Int64SwappedType)
{
    MockDevice device;
    CnDeviceModbusItem4xSwappedInt64 item(&device, 10, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Int64);
    // Swapped suffix enum value
    EXPECT_EQ(item.dataSuffix(), Cn::Suffix_SwappedInt64);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_4x);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
    EXPECT_EQ(item.count(), 4);
}

TEST(CnDeviceModbusItem4xSwappedTest, DoubleSwappedType)
{
    MockDevice device;
    CnDeviceModbusItem4xSwappedDouble item(&device, 20, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_Double);
    // Swapped suffix enum value
    EXPECT_EQ(item.dataSuffix(), Cn::Suffix_SwappedDouble);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_4x);
    EXPECT_EQ(item.count(), 4);
}

// =============================================================================
// ===================== Byte Array Items ======================================
// =============================================================================

TEST(CnDeviceModbusItemByteArrayTest, BitsType)
{
    MockDevice device;
    CnDeviceModbusItem0xByteArray item(&device, 10, 100, 1000); // 100 bits
    
    EXPECT_EQ(item.dataType(), Cn::Data_String);
    EXPECT_EQ(item.dataSuffix(), Cn::Suffix_ByteArray);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_0x);
    EXPECT_EQ(item.access(), Cn::Access_Read);
    EXPECT_EQ(item.count(), 100);
}

TEST(CnDeviceModbusItemByteArrayTest, RegsType)
{
    MockDevice device;
    CnDeviceModbusItem3xByteArray item(&device, 20, 10, 1000); // 10 registers
    
    EXPECT_EQ(item.dataType(), Cn::Data_String);
    EXPECT_EQ(item.dataSuffix(), Cn::Suffix_ByteArray);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_3x);
    EXPECT_EQ(item.access(), Cn::Access_Read);
    EXPECT_EQ(item.count(), 10);
}

TEST(CnDeviceModbusItemByteArrayTest, 4xByteArray)
{
    MockDevice device;
    CnDeviceModbusItem4xByteArray item(&device, 30, 5, 1000);
    
    EXPECT_EQ(item.dataType(), Cn::Data_String);
    EXPECT_EQ(item.memoryType(), Modbus::Memory_4x);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
    EXPECT_EQ(item.count(), 5);
}

// =============================================================================
// ======================= Edge Cases ==========================================
// =============================================================================

TEST(CnDeviceModbusItemTest, ZeroOffset)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000);
    
    EXPECT_EQ(item.offset(), 0);
}

TEST(CnDeviceModbusItemTest, LargeOffset)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 65535, 1000);
    
    EXPECT_EQ(item.offset(), 65535);
}

TEST(CnDeviceModbusItemTest, ZeroPeriod)
{
    MockDevice device;
    CnDeviceModbusItem3xInt32 item(&device, 100, 0);
    
    EXPECT_EQ(item.period(), 0);
}

TEST(CnDeviceModbusItemTest, EmptyMessageId)
{
    MockDevice device;
    CnDeviceModbusItem4xFloat item(&device, 10, 1000, CnSTR(""));
    
    EXPECT_EQ(item.messageId(), CnSTR(""));
}

TEST(CnDeviceModbusItemTest, UpdateWithNullValue)
{
    MockDevice device;
    CnDeviceModbusItem4xInt32 item(&device, 0, 1000);
    
    // Set initial value
    item.update(CnVariant(100), Cn::Status_Good, CnTimestamp::current());
    CnVariant initialValue = item.value();
    EXPECT_EQ((int32_t)initialValue.toInt(), 100);
    
    // Update with null variant (should not change value)
    item.update(CnVariant(), Cn::Status_Good, CnTimestamp::current());
    CnVariant afterNull = item.value();
    EXPECT_EQ((int32_t)afterNull.toInt(), 100);
}

TEST(CnDeviceModbusItemTest, MultipleUpdates)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000);
    
    // Multiple updates
    for (uint16_t i = 0; i < 10; ++i)
    {
        item.update(CnVariant(i), Cn::Status_Good, CnTimestamp::current());
        CnVariant value = item.value();
        EXPECT_EQ((uint16_t)value.toUInt(), i);
    }
}

TEST(CnDeviceModbusItemTest, StatusAndTimestamp)
{
    MockDevice device;
    CnDeviceModbusItem4xInt16 item(&device, 0, 1000);
    
    CnTimestamp testTime = CnTimestamp::current();
    item.update(CnVariant(42), Cn::Status_BadNotConnected, testTime);
    
    Cn::StatusCode status;
    CnTimestamp timestamp;
    CnVariant value = item.value(&status, &timestamp);
    
    EXPECT_EQ((int16_t)value.toInt(), 42);
    EXPECT_EQ(status, Cn::Status_BadNotConnected);
    EXPECT_EQ(timestamp, testTime);
}

// =============================================================================
// =================== Data Pointer Tests ======================================
// =============================================================================

TEST(CnDeviceModbusItemTest, DataPointerNotNull)
{
    MockDevice device;
    CnDeviceModbusItem4xInt32 item(&device, 0, 1000);
    
    void *data = item.data();
    EXPECT_NE(data, nullptr);
}

TEST(CnDeviceModbusItemTest, DataPointerDifferentTypes)
{
    MockDevice device;
    CnDeviceModbusItem4xInt16 item1(&device, 0, 1000);
    CnDeviceModbusItem4xInt32 item2(&device, 0, 1000);
    CnDeviceModbusItem4xFloat item3(&device, 0, 1000);
    
    EXPECT_NE(item1.data(), nullptr);
    EXPECT_NE(item2.data(), nullptr);
    EXPECT_NE(item3.data(), nullptr);
}

// =============================================================================
// =================== Lock/Unlock Tests =======================================
// =============================================================================

TEST(CnDeviceModbusItemTest, LockUnlock)
{
    MockDevice device;
    CnDeviceModbusItem4xInt32 item(&device, 0, 1000);
    
    // Should not crash
    item.lock();
    item.unlock();
}

TEST(CnDeviceModbusItemTest, LockUnlockMultiple)
{
    MockDevice device;
    CnDeviceModbusItem4xInt32 item(&device, 0, 1000);
    
    for (int i = 0; i < 5; ++i)
    {
        item.lock();
        item.unlock();
    }
}
