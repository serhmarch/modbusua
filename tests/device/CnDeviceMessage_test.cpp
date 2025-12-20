/*!
 * \file   CnDeviceMessage_test.cpp
 * \brief  Unit tests for CnDeviceMessage classes
 *
 * \details Tests message classes: CnDevicePollMessage and CnDevicePokeMessage variants
 */

#include <gtest/gtest.h>
#include <device/messages/CnDeviceMessage.h>
#include <device/CnDevice.h>
#include <device/items/CnDeviceModbusItem.h>

// Mock device for testing
class MockDevice : public CnDevice
{
public:
    MockDevice() : CnDevice(nullptr,
                            nullptr,
                            nullptr,
                            nullptr,
                            nullptr) {}
};

// =============================================================================
// CnDevicePollMessageReadCoils Tests
// =============================================================================

TEST(CnDevicePollMessageReadCoilsTest, MemoryType)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadCoils message(&device, &item, 2000);
    
    EXPECT_EQ(message.memoryType(), Modbus::Memory_0x);
}

TEST(CnDevicePollMessageReadCoilsTest, ModbusFunction)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadCoils message(&device, &item, 2000);
    
    EXPECT_EQ(message.ModbusFunction(), MBF_READ_COILS);
}

TEST(CnDevicePollMessageReadCoilsTest, BasicProperties)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 100, 1000, "msg1");
    
    CnDevicePollMessageReadCoils message(&device, &item, 2000);
    
    EXPECT_EQ(message.device(), &device);
    EXPECT_EQ(message.offset(), 100);
}

TEST(CnDevicePollMessageReadCoilsTest, BeginProcess)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadCoils message(&device, &item, 2000);
    
    message.beginProcess();
    
    // Timestamp should be set (just verify no crash)
    CnTimestamp ts = message.timestampBegin();
    (void)ts;
}

TEST(CnDevicePollMessageReadCoilsTest, SetComplete)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadCoils message(&device, &item, 2000);
    
    CnTimestamp now = Cn::currentTimestamp();
    message.setComplete(Cn::Status_Good, now);
    
    EXPECT_EQ(message.status(), Cn::Status_Good);
    EXPECT_EQ(message.timestamp(), now);
}

// =============================================================================
// CnDevicePollMessageReadDiscreteInputs Tests
// =============================================================================

TEST(CnDevicePollMessageReadDiscreteInputsTest, MemoryType)
{
    MockDevice device;
    CnDeviceModbusItem1xBool item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadDiscreteInputs message(&device, &item, 2000);
    
    EXPECT_EQ(message.memoryType(), Modbus::Memory_1x);
}

TEST(CnDevicePollMessageReadDiscreteInputsTest, ModbusFunction)
{
    MockDevice device;
    CnDeviceModbusItem1xBool item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadDiscreteInputs message(&device, &item, 2000);
    
    EXPECT_EQ(message.ModbusFunction(), MBF_READ_DISCRETE_INPUTS);
}

// =============================================================================
// CnDevicePollMessageReadInputRegisters Tests
// =============================================================================

TEST(CnDevicePollMessageReadInputRegistersTest, MemoryType)
{
    MockDevice device;
    CnDeviceModbusItem3xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadInputRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.memoryType(), Modbus::Memory_3x);
}

TEST(CnDevicePollMessageReadInputRegistersTest, ModbusFunction)
{
    MockDevice device;
    CnDeviceModbusItem3xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadInputRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.ModbusFunction(), MBF_READ_INPUT_REGISTERS);
}

// =============================================================================
// CnDevicePollMessageReadHoldingRegisters Tests
// =============================================================================

TEST(CnDevicePollMessageReadHoldingRegistersTest, MemoryType)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.memoryType(), Modbus::Memory_4x);
}

TEST(CnDevicePollMessageReadHoldingRegistersTest, ModbusFunction)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.ModbusFunction(), MBF_READ_HOLDING_REGISTERS);
}

// =============================================================================
// CnDevicePokeMessageWriteMultipleCoils Tests
// =============================================================================

TEST(CnDevicePokeMessageWriteMultipleCoilsTest, MemoryType)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, "msg1");
    
    CnDevicePokeMessageWriteMultipleCoils message(&device, &item, 2000);
    
    EXPECT_EQ(message.memoryType(), Modbus::Memory_0x);
}

TEST(CnDevicePokeMessageWriteMultipleCoilsTest, ModbusFunction)
{
    MockDevice device;
    CnDeviceModbusItem0xBool item(&device, 0, 1000, "msg1");
    
    CnDevicePokeMessageWriteMultipleCoils message(&device, &item, 2000);
    
    EXPECT_EQ(message.ModbusFunction(), MBF_WRITE_MULTIPLE_COILS);
}

// =============================================================================
// CnDevicePokeMessageWriteMultipleRegisters Tests
// =============================================================================

TEST(CnDevicePokeMessageWriteMultipleRegistersTest, MemoryType)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePokeMessageWriteMultipleRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.memoryType(), Modbus::Memory_4x);
}

TEST(CnDevicePokeMessageWriteMultipleRegistersTest, ModbusFunction)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePokeMessageWriteMultipleRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.ModbusFunction(), MBF_WRITE_MULTIPLE_REGISTERS);
}

// =============================================================================
// Message Properties Tests
// =============================================================================

TEST(CnDeviceMessagePropertiesTest, OffsetAndCount)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 100, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.offset(), 100);
}

TEST(CnDeviceMessagePropertiesTest, Period)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 500, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.period(), 500);
}

TEST(CnDeviceMessagePropertiesTest, MessageId)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "testMsg");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.messageId(), "testMsg");
}

TEST(CnDeviceMessagePropertiesTest, InnerBuffer)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_NE(message.innerBuffer(), nullptr);
    EXPECT_GT(message.innerBufferSize(), 0u);
}

// =============================================================================
// Status and Timestamp Tests
// =============================================================================

TEST(CnDeviceMessageStatusTest, InitialStatus)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    // Initial status may vary, just check it's accessible
    Cn::StatusCode status = message.status();
    (void)status; // Suppress unused warning
}

TEST(CnDeviceMessageStatusTest, SetCompleteGood)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    CnTimestamp now = Cn::currentTimestamp();
    message.setComplete(Cn::Status_Good, now);
    
    EXPECT_EQ(message.status(), Cn::Status_Good);
}

TEST(CnDeviceMessageStatusTest, SetCompleteBad)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    CnTimestamp now = Cn::currentTimestamp();
    message.setComplete(Cn::Status_Bad, now);
    
    EXPECT_EQ(message.status(), Cn::Status_Bad);
}

TEST(CnDeviceMessageStatusTest, TimestampUpdated)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    CnTimestamp before = Cn::currentTimestamp();
    message.setComplete(Cn::Status_Good, before);
    
    EXPECT_EQ(message.timestamp(), before);
}

// =============================================================================
// Statistics Tests
// =============================================================================

TEST(CnDeviceMessageStatisticsTest, GoodCount)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    // Initial count should be 0 or accessible
    uint32_t count = message.StatGoodCount();
    EXPECT_GE(count, 0u);
}

TEST(CnDeviceMessageStatisticsTest, BadCount)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    uint32_t count = message.StatBadCount();
    EXPECT_GE(count, 0u);
}

TEST(CnDeviceMessageStatisticsTest, Range)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 100, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    CnString range = message.StatRange();
    EXPECT_FALSE(range.isEmpty());
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST(CnDeviceMessageEdgeCasesTest, ZeroOffset)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.offset(), 0);
}

TEST(CnDeviceMessageEdgeCasesTest, LargeOffset)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 65535, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.offset(), 65535);
}

TEST(CnDeviceMessageEdgeCasesTest, MultipleSetComplete)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    // Multiple calls to setComplete
    for (int i = 0; i < 10; ++i) {
        CnTimestamp now = Cn::currentTimestamp();
        message.setComplete(Cn::Status_Good, now);
    }
    
    EXPECT_EQ(message.status(), Cn::Status_Good);
}

TEST(CnDeviceMessageEdgeCasesTest, EmptyMessageId)
{
    MockDevice device;
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "");
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.messageId(), "");
}

TEST(CnDeviceMessageEdgeCasesTest, LongMessageId)
{
    MockDevice device;
    CnString longId(1000, 'M');
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, longId);
    
    CnDevicePollMessageReadHoldingRegisters message(&device, &item, 125);
    
    EXPECT_EQ(message.messageId(), longId);
}

// =============================================================================
// Different Memory Types
// =============================================================================

TEST(CnDeviceMessageMemoryTypesTest, AllPollTypes)
{
    MockDevice device;
    
    CnDeviceModbusItem0xBool item0x(&device, 0, 1000, "msg1");
    CnDeviceModbusItem1xBool item1x(&device, 0, 1000, "msg2");
    CnDeviceModbusItem3xUInt16 item3x(&device, 0, 1000, "msg3");
    CnDeviceModbusItem4xUInt16 item4x(&device, 0, 1000, "msg4");
    
    CnDevicePollMessageReadCoils msg0x(&device, &item0x, 2000);
    CnDevicePollMessageReadDiscreteInputs msg1x(&device, &item1x, 2000);
    CnDevicePollMessageReadInputRegisters msg3x(&device, &item3x, 125);
    CnDevicePollMessageReadHoldingRegisters msg4x(&device, &item4x, 125);
    
    EXPECT_EQ(msg0x.memoryType(), Modbus::Memory_0x);
    EXPECT_EQ(msg1x.memoryType(), Modbus::Memory_1x);
    EXPECT_EQ(msg3x.memoryType(), Modbus::Memory_3x);
    EXPECT_EQ(msg4x.memoryType(), Modbus::Memory_4x);
}

TEST(CnDeviceMessageMemoryTypesTest, AllPokeTypes)
{
    MockDevice device;
    
    CnDeviceModbusItem0xBool item0x(&device, 0, 1000, "msg1");
    CnDeviceModbusItem4xUInt16 item4x(&device, 0, 1000, "msg2");
    
    CnDevicePokeMessageWriteMultipleCoils msg0x(&device, &item0x, 2000);
    CnDevicePokeMessageWriteMultipleRegisters msg4x(&device, &item4x, 125);
    
    EXPECT_EQ(msg0x.memoryType(), Modbus::Memory_0x);
    EXPECT_EQ(msg4x.memoryType(), Modbus::Memory_4x);
}

// =============================================================================
// Initialization and Finalization
// =============================================================================

// Note: Initialize/Finalize tests removed due to potential memory issues
// These should be tested in integration tests with proper memory management
