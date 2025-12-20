/*!
 * \file   CnDeviceCmdItem_test.cpp
 * \brief  Unit tests for CnDeviceCmdItem template classes
 *
 * \details Tests all 4 command item types: TimeSynch, Init, CommunicationAllow, StatClear
 */

#include <gtest/gtest.h>
#include <device/items/CnDeviceCmdItem.h>
#include <device/CnDevice.h>

// Mock config that enables command features
class MockDeviceConfig : public CnDeviceConfig
{
public:
    MockDeviceConfig() {
    }
};

// Mock device for testing - uses real CnDevice command implementations
class MockDevice : public CnDevice
{
public:
    MockDevice() : CnDevice(nullptr,
                            nullptr,
                            nullptr,
                            new MockDeviceConfig(),
                            nullptr)
    {
    }
};

// =============================================================================
// Base CnDeviceCmdItem Tests
// =============================================================================

TEST(CnDeviceCmdItemTest, ItemType)
{
    MockDevice device;
    CnDeviceCmdItemStatClear item(&device);
    EXPECT_EQ(item.itemType(), CnDeviceBaseItem::Item_Command);
}

TEST(CnDeviceCmdItemTest, Access)
{
    MockDevice device;
    CnDeviceCmdItemStatClear item(&device);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
}

TEST(CnDeviceCmdItemTest, ReadValueAlwaysFalse)
{
    MockDevice device;
    CnDeviceCmdItemStatClear item(&device);
    
    // Read value should always return false
    EXPECT_FALSE(item.value(nullptr, nullptr).toBool());
    
    // Even after executing command
    item.update(CnVariant(true), Cn::Status_Good, Cn::currentTimestamp());
    EXPECT_FALSE(item.value(nullptr, nullptr).toBool());
}

// =============================================================================
// CnDeviceCmdItemStatClear Tests
// =============================================================================

TEST(CnDeviceCmdItemStatClearTest, DataType)
{
    MockDevice device;
    CnDeviceCmdItemStatClear item(&device);
    EXPECT_EQ(item.dataType(), Cn::Data_Bool);
}
