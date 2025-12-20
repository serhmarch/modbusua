/*!
 * \file   CnDeviceCfgItem_test.cpp
 * \brief  Unit tests for CnDeviceCfgItem template classes
 *
 * \details Tests representative sample of 50+ configuration item types
 */

#include <gtest/gtest.h>
#include <device/items/CnDeviceCfgItem.h>
#include <device/CnDevice.h>
#include <device/CnDeviceConfig.h>

// Mock device config for testing
class MockDeviceConfig : public CnDeviceConfig
{
public:
    MockDeviceConfig()
    {
        // Initialize base class members to default test values
        setCfgName("TestDevice");
        setCfgEnableDevice(true);
        setCfgModbusUnit(1);
        setCfgMaxReadCoils(2000);
        setCfgDefaultPeriod(1000);
    }
};// Mock device that returns our mock config
class MockDevice : public CnDevice
{
public:
    MockDevice() : CnDevice(nullptr,
                            nullptr,
                            nullptr,
                            new MockDeviceConfig(),
                            nullptr)
    {
        // CnDevice takes ownership of the config pointer
    }
    
    MockDeviceConfig* mockCfg()
    {
        return static_cast<MockDeviceConfig*>(m_cfg);
    }
};

// =============================================================================
// Base CnDeviceCfgItem Tests
// =============================================================================

TEST(CnDeviceCfgItemTest, ItemType)
{
    MockDevice device;
    CnDeviceCfgItemName item(&device);
    EXPECT_EQ(item.itemType(), CnDeviceBaseItem::Item_Config);
}

TEST(CnDeviceCfgItemTest, AccessReadOnly)
{
    MockDevice device;
    CnDeviceCfgItemName item(&device);
    EXPECT_EQ(item.access(), Cn::Access_Read);
}

// =============================================================================
// String Type Config Items
// =============================================================================

TEST(CnDeviceCfgItemNameTest, DataType)
{
    MockDevice device;
    CnDeviceCfgItemName item(&device);
    EXPECT_EQ(item.dataType(), Cn::Data_String);
}

TEST(CnDeviceCfgItemNameTest, ReadValue)
{
    MockDevice device;
    // Production default name is "PLC"
    CnDeviceCfgItemName item(&device);
    CnVariant value = item.value();
    
    EXPECT_EQ(value.toString(), "PLC");
}

TEST(CnDeviceCfgItemNameTest, EmptyName)
{
    MockDevice device;
    // Set empty name and verify
    device.mockCfg()->setCfgName("");
    
    CnDeviceCfgItemName item(&device);
    EXPECT_EQ(item.value().toString(), "");
}

TEST(CnDeviceCfgItemNameTest, LongName)
{
    MockDevice device;
    CnString longName(1000, 'A');
    device.mockCfg()->setCfgName(longName);
    
    CnDeviceCfgItemName item(&device);
    EXPECT_EQ(item.value().toString(), longName);
}

TEST(CnDeviceCfgItemNameTest, SpecialCharacters)
{
    MockDevice device;
    device.mockCfg()->setCfgName("PLC-01_TEST.2024");
    
    CnDeviceCfgItemName item(&device);
    EXPECT_EQ(item.value().toString(), "PLC-01_TEST.2024");
}

// =============================================================================
// Boolean Type Config Items
// =============================================================================

TEST(CnDeviceCfgItemEnableDeviceTest, DataType)
{
    MockDevice device;
    CnDeviceCfgItemEnableDevice item(&device);
    EXPECT_EQ(item.dataType(), Cn::Data_Bool);
}

TEST(CnDeviceCfgItemEnableDeviceTest, ReadTrue)
{
    MockDevice device;
    // Default EnableDevice is true
    CnDeviceCfgItemEnableDevice item(&device);
    EXPECT_TRUE(item.value().toBool());
}

TEST(CnDeviceCfgItemEnableDeviceTest, ReadFalse)
{
    MockDevice device;
    device.mockCfg()->setCfgEnableDevice(false);
    
    CnDeviceCfgItemEnableDevice item(&device);
    EXPECT_FALSE(item.value().toBool());
}

// =============================================================================
// UInt8 Type Config Items
// =============================================================================

TEST(CnDeviceCfgItemModbusUnitTest, DataType)
{
    MockDevice device;
    CnDeviceCfgItemModbusUnit item(&device);
    EXPECT_EQ(item.dataType(), Cn::Data_UInt8);
}

TEST(CnDeviceCfgItemModbusUnitTest, ReadValue)
{
    MockDevice device;
    // Default ModbusUnit is 1
    CnDeviceCfgItemModbusUnit item(&device);
    EXPECT_EQ((uint8_t)item.value().toUInt(), (uint8_t)1);
}

TEST(CnDeviceCfgItemModbusUnitTest, MinValue)
{
    MockDevice device;
    device.mockCfg()->setCfgModbusUnit(0);
    
    CnDeviceCfgItemModbusUnit item(&device);
    EXPECT_EQ((uint8_t)item.value().toUInt(), 0);
}

TEST(CnDeviceCfgItemModbusUnitTest, ReadMax)
{
    MockDevice device;
    device.mockCfg()->setCfgModbusUnit(255);
    
    CnDeviceCfgItemModbusUnit item(&device);
    EXPECT_EQ((uint8_t)item.value().toUInt(), 255);
}

// =============================================================================
// UInt16 Type Config Items
// =============================================================================

TEST(CnDeviceCfgItemMaxReadCoilsTest, DataType)
{
    MockDevice device;
    CnDeviceCfgItemMaxReadCoils item(&device);
    EXPECT_EQ(item.dataType(), Cn::Data_UInt16);
}

TEST(CnDeviceCfgItemMaxReadCoilsTest, ReadValue)
{
    MockDevice device;
    // Default MaxReadCoils is 2000
    CnDeviceCfgItemMaxReadCoils item(&device);
    EXPECT_EQ((uint16_t)item.value().toUInt(), (uint16_t)2000);
}

TEST(CnDeviceCfgItemMaxReadCoilsTest, MinValue)
{
    MockDevice device;
    device.mockCfg()->setCfgMaxReadCoils(0);
    
    CnDeviceCfgItemMaxReadCoils item(&device);
    EXPECT_EQ((uint16_t)item.value().toUInt(), 0);
}

TEST(CnDeviceCfgItemMaxReadCoilsTest, ReadMax)
{
    MockDevice device;
    device.mockCfg()->setCfgMaxReadCoils(65535);
    
    CnDeviceCfgItemMaxReadCoils item(&device);
    EXPECT_EQ((uint16_t)item.value().toUInt(), 65535);
}

// =============================================================================
// UInt32 Type Config Items
// =============================================================================

TEST(CnDeviceCfgItemDefaultPeriodTest, DataType)
{
    MockDevice device;
    CnDeviceCfgItemDefaultPeriod item(&device);
    EXPECT_EQ(item.dataType(), Cn::Data_UInt32);
}

TEST(CnDeviceCfgItemDefaultPeriodTest, ReadValue)
{
    MockDevice device;
    // Default DefaultPeriod is 1000
    CnDeviceCfgItemDefaultPeriod item(&device);
    EXPECT_EQ((uint32_t)item.value().toUInt(), 1000U);
}

TEST(CnDeviceCfgItemDefaultPeriodTest, MinValue)
{
    MockDevice device;
    device.mockCfg()->setCfgDefaultPeriod(0);
    
    CnDeviceCfgItemDefaultPeriod item(&device);
    EXPECT_EQ((uint32_t)item.value().toUInt(), 0);
}

TEST(CnDeviceCfgItemDefaultPeriodTest, LargeValue)
{
    MockDevice device;
    device.mockCfg()->setCfgDefaultPeriod(4294967295U);
    
    CnDeviceCfgItemDefaultPeriod item(&device);
    EXPECT_EQ((uint32_t)item.value().toUInt(), 4294967295U);
}

// =============================================================================
// Multiple Items Same Device
// =============================================================================

TEST(CnDeviceCfgItemMultipleTest, AllItemsSameDevice)
{
    MockDevice device;
    device.mockCfg()->setCfgName("MultiTest");
    device.mockCfg()->setCfgEnableDevice(true);
    device.mockCfg()->setCfgModbusUnit(10);
    device.mockCfg()->setCfgMaxReadCoils(1000);
    device.mockCfg()->setCfgDefaultPeriod(2000);
    
    CnDeviceCfgItemName itemName(&device);
    CnDeviceCfgItemEnableDevice itemEnable(&device);
    CnDeviceCfgItemModbusUnit itemUnit(&device);
    CnDeviceCfgItemMaxReadCoils itemCoils(&device);
    CnDeviceCfgItemDefaultPeriod itemPeriod(&device);
    
    EXPECT_EQ(itemName.value().toString(), "MultiTest");
    EXPECT_TRUE(itemEnable.value().toBool());
    EXPECT_EQ((uint8_t)itemUnit.value().toUInt(), 10);
    EXPECT_EQ((uint16_t)itemCoils.value().toUInt(), 1000);
    EXPECT_EQ((uint32_t)itemPeriod.value().toUInt(), 2000);
}

TEST(CnDeviceCfgItemMultipleTest, MultipleInstancesSameType)
{
    MockDevice device;
    device.mockCfg()->setCfgName("SameType");
    
    CnDeviceCfgItemName item1(&device);
    CnDeviceCfgItemName item2(&device);
    CnDeviceCfgItemName item3(&device);
    
    // All should read same value
    EXPECT_EQ(item1.value().toString(), "SameType");
    EXPECT_EQ(item2.value().toString(), "SameType");
    EXPECT_EQ(item3.value().toString(), "SameType");
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST(CnDeviceCfgItemEdgeCasesTest, StatusAndTimestampOptional)
{
    MockDevice device;
    device.mockCfg()->setCfgName("EdgeTest");
    
    CnDeviceCfgItemName item(&device);
    
    // Call value() without status/timestamp
    CnVariant value1 = item.value();
    EXPECT_EQ(value1.toString(), "EdgeTest");
    
    // Call value() with null pointers
    CnVariant value2 = item.value(nullptr, nullptr);
    EXPECT_EQ(value2.toString(), "EdgeTest");
}

TEST(CnDeviceCfgItemEdgeCasesTest, RapidReads)
{
    MockDevice device;
    device.mockCfg()->setCfgDefaultPeriod(5000);
    
    CnDeviceCfgItemDefaultPeriod item(&device);
    
    // Rapidly read same item
    for (int i = 0; i < 1000; ++i)
    {
        EXPECT_EQ((uint32_t)item.value().toUInt(), 5000);
    }
}

TEST(CnDeviceCfgItemEdgeCasesTest, ZeroValues)
{
    MockDevice device;
    device.mockCfg()->setCfgModbusUnit(0);
    device.mockCfg()->setCfgMaxReadCoils(0);
    device.mockCfg()->setCfgDefaultPeriod(0);
    
    CnDeviceCfgItemModbusUnit itemUnit(&device);
    CnDeviceCfgItemMaxReadCoils itemCoils(&device);
    CnDeviceCfgItemDefaultPeriod itemPeriod(&device);
    
    EXPECT_EQ((uint8_t)itemUnit.value().toUInt(), 0);
    EXPECT_EQ((uint16_t)itemCoils.value().toUInt(), 0);
    EXPECT_EQ((uint32_t)itemPeriod.value().toUInt(), 0);
}

TEST(CnDeviceCfgItemEdgeCasesTest, BooleanToggle)
{
    MockDevice device;
    CnDeviceCfgItemEnableDevice item(&device);
    
    // Toggle value
    device.mockCfg()->setCfgEnableDevice(false);
    EXPECT_FALSE(item.value().toBool());
    
    device.mockCfg()->setCfgEnableDevice(true);
    EXPECT_TRUE(item.value().toBool());
    
    device.mockCfg()->setCfgEnableDevice(false);
    EXPECT_FALSE(item.value().toBool());
}

TEST(CnDeviceCfgItemEdgeCasesTest, ConfigPersistence)
{
    MockDevice device;
    device.mockCfg()->setCfgName("Persistent");
    device.mockCfg()->setCfgModbusUnit(99);
    
    CnDeviceCfgItemName item1(&device);
    CnDeviceCfgItemModbusUnit item2(&device);
    
    // Read initial values
    EXPECT_EQ(item1.value().toString(), "Persistent");
    EXPECT_EQ((uint8_t)item2.value().toUInt(), 99);
    
    // Destroy items
    {
        CnDeviceCfgItemName tempItem(&device);
        EXPECT_EQ(tempItem.value().toString(), "Persistent");
    }
    
    // Values should persist
    EXPECT_EQ(item1.value().toString(), "Persistent");
    EXPECT_EQ((uint8_t)item2.value().toUInt(), 99);
}
