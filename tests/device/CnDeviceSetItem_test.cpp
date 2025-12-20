/*!
 * \file   CnDeviceSetItem_test.cpp
 * \brief  Unit tests for CnDeviceSetItem template classes
 *
 * \details Tests all 3 setting item types: EnableDevice
 */

#include <gtest/gtest.h>
#include <device/items/CnDeviceSetItem.h>
#include <device/CnDevice.h>

// Mock device for testing
class MockDevice : public CnDevice
{
public:
    MockDevice() : CnDevice(nullptr,
                            nullptr,
                            nullptr,
                            nullptr,
                            nullptr)
    {
        // CnDevice base class handles settings in m_set
    }
};

// =============================================================================
// Base CnDeviceSetItem Tests
// =============================================================================

TEST(CnDeviceSetItemTest, ItemType)
{
    MockDevice device;
    CnDeviceSetItemEnableDevice item(&device);
    EXPECT_EQ(item.itemType(), CnDeviceBaseItem::Item_Setting);
}

TEST(CnDeviceSetItemTest, Access)
{
    MockDevice device;
    CnDeviceSetItemEnableDevice item(&device);
    EXPECT_EQ(item.access(), Cn::Access_ReadWrite);
}

// =============================================================================
// CnDeviceSetItemEnableDevice Tests
// =============================================================================

TEST(CnDeviceSetItemEnableDeviceTest, DataType)
{
    MockDevice device;
    CnDeviceSetItemEnableDevice item(&device);
    EXPECT_EQ(item.dataType(), Cn::Data_Bool);
}

TEST(CnDeviceSetItemEnableDeviceTest, ReadValue)
{
    MockDevice device;
    device.setSettingEnableDevice(true);
    
    CnDeviceSetItemEnableDevice item(&device);
    CnVariant value = item.value();
    
    EXPECT_TRUE(value.toBool());
}

TEST(CnDeviceSetItemEnableDeviceTest, WriteValue)
{
    MockDevice device;
    device.setSettingEnableDevice(false);
    
    CnDeviceSetItemEnableDevice item(&device);
    item.update(CnVariant(true), Cn::Status_Good, Cn::currentTimestamp());
    
    EXPECT_TRUE(device.SettingEnableDevice());
}

TEST(CnDeviceSetItemEnableDeviceTest, ReadWriteRoundTrip)
{
    MockDevice device;
    CnDeviceSetItemEnableDevice item(&device);
    
    // Write true
    item.update(CnVariant(true), Cn::Status_Good, Cn::currentTimestamp());
    EXPECT_TRUE(item.value().toBool());
    
    // Write false
    item.update(CnVariant(false), Cn::Status_Good, Cn::currentTimestamp());
    EXPECT_FALSE(item.value().toBool());
}

TEST(CnDeviceSetItemEnableDeviceTest, MultipleUpdates)
{
    MockDevice device;
    CnDeviceSetItemEnableDevice item(&device);
    
    for (int i = 0; i < 10; ++i) {
        bool value = (i % 2 == 0);
        item.update(CnVariant(value), Cn::Status_Good, Cn::currentTimestamp());
        EXPECT_EQ(item.value().toBool(), value);
    }
}

TEST(CnDeviceSetItemEnableDeviceTest, NonBooleanConversion)
{
    MockDevice device;
    CnDeviceSetItemEnableDevice item(&device);
    
    // Integer conversion
    item.update(CnVariant(1), Cn::Status_Good, Cn::currentTimestamp());
    EXPECT_TRUE(item.value().toBool());
    
    item.update(CnVariant(0), Cn::Status_Good, Cn::currentTimestamp());
    EXPECT_FALSE(item.value().toBool());
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST(CnDeviceSetItemEdgeCasesTest, StatusAndTimestampIgnored)
{
    MockDevice device;
    CnDeviceSetItemEnableDevice item(&device);
    
    // Status and timestamp parameters should not affect the write
    item.update(CnVariant(true), Cn::Status_Bad, CnTimestamp());
    EXPECT_TRUE(item.value().toBool());
}

TEST(CnDeviceSetItemEdgeCasesTest, EmptyVariant)
{
    MockDevice device;
    device.setSettingEnableDevice(true);
    
    CnDeviceSetItemEnableDevice item(&device);
    
    // Empty variant - fromVariant fails, so value should not change
    item.update(CnVariant(), Cn::Status_Good, Cn::currentTimestamp());
    EXPECT_TRUE(item.value().toBool()); // Should remain true
}

TEST(CnDeviceSetItemEdgeCasesTest, RapidToggling)
{
    MockDevice device;
    CnDeviceSetItemEnableDevice item(&device);
    
    // Rapidly toggle value
    for (int i = 0; i < 100; ++i) {
        bool value = (i % 2 == 0);
        item.update(CnVariant(value), Cn::Status_Good, Cn::currentTimestamp());
    }
    
    // Final value should be false (100 is even, so last value is true, but we start at 0)
    EXPECT_FALSE(item.value().toBool());
}

TEST(CnDeviceSetItemEdgeCasesTest, MultipleItemsSameDevice)
{
    MockDevice device;
    
    // Create multiple items on same device
    CnDeviceSetItemEnableDevice item1(&device);
    CnDeviceSetItemEnableDevice item2(&device);
    CnDeviceSetItemEnableDevice item3(&device);
    
    // All items should share the same underlying setting
    item1.update(CnVariant(true), Cn::Status_Good, Cn::currentTimestamp());
    
    EXPECT_TRUE(item1.value().toBool());
    EXPECT_TRUE(item2.value().toBool());
    EXPECT_TRUE(item3.value().toBool());
    
    // Update via different item
    item2.update(CnVariant(false), Cn::Status_Good, Cn::currentTimestamp());
    
    EXPECT_FALSE(item1.value().toBool());
    EXPECT_FALSE(item2.value().toBool());
    EXPECT_FALSE(item3.value().toBool());
}
