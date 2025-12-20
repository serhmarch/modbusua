/*!
 * \file   CnDeviceStatItem_test.cpp
 * \brief  Unit tests for CnDeviceStatItem template classes
 *
 * \details Tests representative sample of 100+ statistics item types
 */

#include <gtest/gtest.h>
#include <device/items/CnDeviceStatItem.h>
#include <device/CnDevice.h>
#include <device/CnDeviceStat.h>

// Mock device stat - just inherits, base class handles storage
class MockDeviceStat : public CnDeviceStat
{
public:
    MockDeviceStat()
    {
        // Base class CnDeviceStat handles all storage and access
    }
};

// Mock device that returns our mock stat
class MockDevice : public CnDevice
{
public:
    MockDevice() : CnDevice(nullptr,
                            nullptr,
                            nullptr,
                            nullptr,
                            new MockDeviceStat()) 
    {
        // CnDevice takes ownership of the stat pointer
    }
    
    MockDeviceStat* mockStat() { return static_cast<MockDeviceStat*>(m_stat); }
};

// =============================================================================
// Base CnDeviceStatItem Tests
// =============================================================================

TEST(CnDeviceStatItemTest, ItemType)
{
    MockDevice device;
    CnDeviceStatItemInnerState item(&device);
    EXPECT_EQ(item.itemType(), CnDeviceBaseItem::Item_Statistic);
}

TEST(CnDeviceStatItemTest, AccessReadOnly)
{
    MockDevice device;
    CnDeviceStatItemInnerState item(&device);
    EXPECT_EQ(item.access(), Cn::Access_Read);
}

// =============================================================================
// UInt32 Type Statistic Items
// =============================================================================

TEST(CnDeviceStatItemInnerStateTest, MinValue)
{
    MockDevice device;
    device.mockStat()->setStatInnerState(0);
    
    CnDeviceStatItemInnerState item(&device);
    EXPECT_EQ((uint32_t)item.value().toUInt(), 0);
}

TEST(CnDeviceStatItemInnerStateTest, LargeValue)
{
    MockDevice device;
    device.mockStat()->setStatInnerState(2147483647); // MAX_INT32
    
    CnDeviceStatItemInnerState item(&device);
    EXPECT_EQ((uint32_t)item.value().toUInt(), 2147483647);
}

TEST(CnDeviceStatItemCommonGoodCountTest, DataType)
{
    MockDevice device;
    CnDeviceStatItemCommonGoodCount item(&device);
    EXPECT_EQ(item.dataType(), Cn::Data_UInt32);
}

TEST(CnDeviceStatItemCommonGoodCountTest, ReadValue)
{
    MockDevice device;
    // Default CommonGoodCount is 0
    CnDeviceStatItemCommonGoodCount item(&device);
    EXPECT_EQ((uint32_t)item.value().toUInt(), 0U);
}

TEST(CnDeviceStatItemInnerStateTest, DataType)
{
    MockDevice device;
    CnDeviceStatItemInnerState item(&device);
    auto dt = item.dataType();
    EXPECT_EQ(dt, Cn::Data_Int32);
}

// =============================================================================
// Boolean Type Statistic Items (from State)
// =============================================================================

TEST(CnDeviceStatItemStateEnableDeviceTest, DataType)
{
    MockDevice device;
    CnDeviceStatItemStateEnableDevice item(&device);
    EXPECT_EQ(item.dataType(), Cn::Data_Bool);
}

TEST(CnDeviceStatItemStateEnableDeviceTest, ReadTrue)
{
    MockDevice device;
    // Set bit 0 to test true
    device.mockStat()->setStatStateEnableDevice(true);
    
    CnDeviceStatItemStateEnableDevice item(&device);
    EXPECT_TRUE(item.value().toBool());
}

TEST(CnDeviceStatItemStateEnableDeviceTest, ReadFalse)
{
    MockDevice device;
    // CnDevice constructor sets EnableDevice=true by default, so set to false explicitly
    device.mockStat()->setStatStateEnableDevice(false);
    
    CnDeviceStatItemStateEnableDevice item(&device);
    EXPECT_FALSE(item.value().toBool());
}

TEST(CnDeviceStatItemDynamicTest, CounterIncrement)
{
    MockDevice device;
    CnDeviceStatItemCommonGoodCount item(&device);
    
    // Simulate counter incrementing by calling setStatPollResult
    for (uint32_t i = 0; i < 100; ++i) {
        device.mockStat()->setStatPollResult(Modbus::Status_Good, CnTimestamp::current());
    }
    // After 100 good results, CommonGoodCount should be 100
    EXPECT_EQ((uint32_t)item.value().toUInt(), 100);
}

TEST(CnDeviceStatItemDynamicTest, BooleanToggling)
{
    MockDevice device;
    CnDeviceStatItemStateEnableDevice item(&device);
    
    // Toggle value
    for (int i = 0; i < 10; ++i) {
        bool expected = (i % 2 == 0);
        device.mockStat()->setStatStateEnableDevice(expected);
        EXPECT_EQ(item.value().toBool(), expected);
    }
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST(CnDeviceStatItemEdgeCasesTest, StateFlags)
{
    MockDevice device;
    CnDeviceStatItemStateEnableDevice item(&device);
    
    // Set EnableDevice flag
    device.mockStat()->setStatStateEnableDevice(true);
    EXPECT_TRUE(item.value().toBool());
    
    // Clear EnableDevice flag
    device.mockStat()->setStatStateEnableDevice(false);
    EXPECT_FALSE(item.value().toBool());
    
    // Set EnableDevice flag again
    device.mockStat()->setStatStateEnableDevice(true);
    EXPECT_TRUE(item.value().toBool());
}
